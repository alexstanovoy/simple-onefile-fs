#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <fs/utility.h>
#include <fs/write/write.h>
#include <fs/page_alloc/page_alloc.h>
#include <fs/find_inode/find_inode.h>
#include <fs/fs_utility/fs_utility.h>


ssize_t write_file_system(file_system* fs, const char* path, const void* buf,
                          size_t count, size_t off) {
  if (count == 0) {
    return 0;
  }
  if (verify_path(path) < 0) {
    return -1;
  }
  file_inode* file = (file_inode*)find_inode_file_system(fs, path);
  if (file == NULL || file->inode_info.type != File) {
    return -1;
  }
  if (off >= PAGE_SIZE - sizeof(file_inode)) {
    return -1;
  }
  if (off > file->size) {
    memset((void*)file->content + file->size, 0, off - file->size);
  }
  size_t to_write = min(count, PAGE_SIZE - sizeof(file_inode) - off);
  memcpy((void*)(file->content + off), buf, to_write);
  file->size = max(file->size, off + to_write);
  return to_write;
}

ssize_t read_file_system(file_system* fs, const char* path, void* buf,
                         size_t count, size_t off) {
  if (count == 0) {
    return 0;
  }
  if (verify_path(path) < 0) {
    return -1;
  }
  file_inode* file = (file_inode*)find_inode_file_system(fs, path);
  if (file == NULL || file->inode_info.type != File) {
    return -1;
  }
  if (file->size == 0) {
    return 0;
  }
  if (off >= file->size) {
    return -1;
  }
  size_t to_read = min(count, file->size - off);
  memcpy(buf, (void*)(file->content + off), to_read);
  return to_read;
}
