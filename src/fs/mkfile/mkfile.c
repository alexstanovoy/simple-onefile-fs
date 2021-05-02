#include <stddef.h>
#include <string.h>
#include <fs/utility.h>
#include <fs/page_alloc/page_alloc.h>
#include <fs/find_inode/find_inode.h>
#include <fs/mkfile/mkfile.h>


file_inode* mkfile(file_system* fs, directory_inode* dir, const char* name) {
  if (dir->entry_count == MAX_DIRECTORY_ENTRIES) {
    return NULL;
  }
  size_t index = 0;
  while (1) {
    if (dir->entries[index] != NULL) {
      ++index;
      continue;
    }
    dir->entries[index] = (inode_header*)page_alloc_file_system(fs);
    if (dir->entries[index] == NULL) {
      return NULL;
    }
    ++dir->entry_count;
    break;
  }
  return init_file_inode((file_inode*)dir->entries[index], name);
}

int mkfile_file_system(file_system* fs, const char* path) {
  if (verify_path(path) < 0 || strcmp(path, "/") == 0 ||
      find_inode_file_system(fs, path) != NULL) {
    return -1;
  }
  RAII(cleanup_free) char* parents = get_parents_only(path);
  RAII(cleanup_free) char* name = get_name_only(path);
  directory_inode* dir = (directory_inode*)find_inode_file_system(fs, parents);
  if (dir == NULL || dir->inode_info.type != Directory) {
    return -1;
  }
  return mkfile(fs, dir, name) == NULL ? -1 : 0;
}

int rmfile(file_system* fs, directory_inode* dir, const char* name) {
  size_t index = 0;
  while (index < MAX_DIRECTORY_ENTRIES) {
    if (dir->entries[index] == NULL ||
        strcmp(dir->entries[index]->name, name) != 0) {
      ++index;
      continue;
    }
    file_inode* del_file = (file_inode*)dir->entries[index];
    if (del_file->inode_info.type != File) {
      return -1;
    }
    continuation_inode* cur_header = (continuation_inode*)del_file->next_inode;
    page_free_file_system(fs, (page_header*)del_file);
    while (cur_header != NULL) {
      continuation_inode* next_header = cur_header->next_inode;
      page_free_file_system(fs, (page_header*)cur_header);
      cur_header = next_header;
    }
    dir->entries[index] = NULL;
    --dir->entry_count;
    return 0;
  }
  return -1;
}

int rmfile_file_system(file_system* fs, const char* path) {
  if (verify_path(path) < 0 || strcmp(path, "/") == 0 ||
      find_inode_file_system(fs, path) == NULL) {
    return -1;
  }
  RAII(cleanup_free) char* parents = get_parents_only(path);
  RAII(cleanup_free) char* name = get_name_only(path);
  directory_inode* dir = (directory_inode*)find_inode_file_system(fs, parents);
  if (dir == NULL || dir->inode_info.type != Directory) {
    return -1;
  }
  return rmfile(fs, dir, name);
}
