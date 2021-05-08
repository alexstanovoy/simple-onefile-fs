#include <string.h>
#include <stdlib.h>
#include <fs/headers.h>


directory_inode* init_directory_inode(directory_inode* dir, const char* name) {
  dir->inode_info.type = Directory;
  strcpy(dir->inode_info.name, name);
  dir->entry_count = 0;
  return dir;
}

file_inode* init_file_inode(file_inode* file, const char* name) {
  file->inode_info.type = File;
  file->next_inode = NULL;
  strcpy(file->inode_info.name, name);
  file->size = 0;
  return file;
}

void cleanup_free(void* ptr) {
  free(*(void**)ptr);
}
