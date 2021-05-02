#include <stddef.h>
#include <fs/utility.h>
#include <fs/find/find.h>
#include <fs/find_inode/find_inode.h>


int find_file_system(file_system* fs, const char* path) {
  if (verify_path(path) < 0) {
    return -1;
  }
  return find_inode_file_system(fs, path) == NULL ? -1 : 0;
}

int is_directory_file_system(file_system* fs, const char* path) {
  if (verify_path(path) < 0) {
    return -1;
  }
  inode_header* inode = find_inode_file_system(fs, path);
  return inode == NULL || inode->type != Directory ? -1 : 0;
}

int is_file_file_system(file_system* fs, const char* path) {
  if (verify_path(path) < 0) {
    return -1;
  }
  inode_header* inode = find_inode_file_system(fs, path);
  return inode == NULL || inode->type != File ? -1 : 0;
}
