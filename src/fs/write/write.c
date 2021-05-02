#include <assert.h>
#include <string.h>
#include <fs/utility.h>
#include <fs/write/write.h>
#include <fs/page_alloc/page_alloc.h>
#include <fs/find_inode/find_inode.h>
#include <fs/fs_utility/fs_utility.h>


continuation_inode* reduce_off(file_system* fs, file_inode* file, uint32_t* off) {
  // if (fs == NULL)  ->  new pages are not created
  // off after usage is from __page__ begin! 
  if (*off < PAGE_SIZE - sizeof(file_inode)) {
    *off += sizeof(file_inode);
    return (continuation_inode*)file;
  }
  *off -= PAGE_SIZE - sizeof(file_inode);
  continuation_inode* cont = file->next_inode;
  while (*off >= PAGE_SIZE - sizeof(continuation_inode)) {
    *off -= PAGE_SIZE - sizeof(continuation_inode);
    if (cont->next_inode == NULL) {
      if (fs == NULL) {
        return NULL;
      }
      cont->next_inode = (continuation_inode*)page_alloc_file_system(fs);
      assert(cont->next_inode != NULL);
      cont = cont->next_inode;
      cont->next_inode = NULL;
      memset(cont->content, 0, PAGE_SIZE - sizeof(continuation_inode));
    }
  }
  *off += sizeof(continuation_inode);
  return 0;
}

int write_file_system(file_system* fs, const char* path, const void* buf,
                      uint32_t count, uint32_t off) {
  if (verify_path(path) < 0) {
    return -1;
  }
  file_inode* file = (file_inode*)find_inode_file_system(fs, path);
  if (file == NULL || file->inode_info.type != File ||
      (off >= file->size && total_free_file_system(fs) <= off - file->size)) {
    return -1;
  }
  continuation_inode* cont = reduce_off(fs, file, &off);
  if (cont == NULL) {
    return 0;
  }
  uint32_t ready = 0;
  uint32_t to_copy = min(count, PAGE_SIZE - off);
  memcpy((void*)(cont + off), buf, to_copy);
  count -= to_copy;
  buf += to_copy;
  ready += to_copy;
  while (count > 0) {
    if (cont->next_inode == NULL) {
      cont->next_inode = (continuation_inode*)page_alloc_file_system(fs);
      if (cont->next_inode == NULL) {
        return ready;
      }
    }
    cont = cont->next_inode;
    uint32_t to_copy = min(count, PAGE_SIZE - sizeof(continuation_inode));
    memcpy((void*)cont, buf, to_copy);
    count -= to_copy;
    buf += to_copy;
    ready += to_copy;
  }
  return ready;
}

int read_file_system(file_system* fs, const char* path, void* buf,
                     uint32_t count, uint32_t off) {
  if (verify_path(path) < 0) {
    return -1;
  }
  file_inode* file = (file_inode*)find_inode_file_system(fs, path);
  if (file == NULL || file->inode_info.type != File) {
    return -1;
  }
  continuation_inode* cont = reduce_off(fs, file, &off);
  if (cont == NULL) {
    return -1;
  }
  uint32_t ready = 0;
  uint32_t to_copy = min(count, PAGE_SIZE - off);
  memcpy(buf, (void*)(cont + off), to_copy);
  count -= to_copy;
  buf += to_copy;
  ready += to_copy;
  while (count > 0) {
    if (cont->next_inode == NULL) {
      return ready;
    }
    cont = cont->next_inode;
    to_copy = min(count, PAGE_SIZE - off);
    memcpy(buf, (void*)(cont + off), to_copy);
    count -= to_copy;
    buf += to_copy;
    ready += to_copy;
  }
  return ready;
}
