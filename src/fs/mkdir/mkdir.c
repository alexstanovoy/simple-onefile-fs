#include <fs/find_inode/find_inode.h>
#include <fs/headers.h>
#include <fs/mkdir/mkdir.h>
#include <fs/page_alloc/page_alloc.h>
#include <fs/utility.h>
#include <stddef.h>
#include <string.h>


directory_inode* mkdir(file_system* fs, directory_inode* dir, const char* name) {
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
  return init_directory_inode((directory_inode*)dir->entries[index], name);
}

int mkdir_file_system(file_system* fs, const char* path) {
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
  return mkdir(fs, dir, name) == NULL ? -1 : 0;
}

int rmdir(file_system* fs, directory_inode* dir, const char* name) {
  size_t index = 0;
  while (index < MAX_DIRECTORY_ENTRIES) {
    if (dir->entries[index] == NULL ||
        strcmp(dir->entries[index]->name, name) != 0) {
      ++index;
      continue;
    }
    directory_inode* del_dir = (directory_inode*)dir->entries[index];
    if (del_dir->inode_info.type != Directory || del_dir->entry_count > 0) {
      return -1;
    }
    page_free_file_system(fs, (page_header*)del_dir);
    dir->entries[index] = NULL;
    --dir->entry_count;
    return 0;
  }
  return -1;
}

int rmdir_file_system(file_system* fs, const char* path) {
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
  return rmdir(fs, dir, name);
}
