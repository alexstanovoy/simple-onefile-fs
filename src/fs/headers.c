#include <string.h>
#include <stdlib.h>
#include <fs/headers.h>


inode_header* init_inode(inode_header* inode, INodeType type,
                         const char* name) {
  inode->type = type;
  strcpy(inode->name, name);
  return inode;
}

directory_inode* init_directory_inode(directory_inode* dir, const char* name) {
  (void)init_inode((inode_header*)dir, Directory, name);
  dir->entry_count = 0;
  return dir;
}

file_inode* init_file_inode(file_inode* file, const char* name) {
  (void)init_inode((inode_header*)file, File, name);
  file->next_inode = NULL;
  file->size = 0;
  return file;
}

void cleanup_free(void* ptr) {
  free(*(void**)ptr);
}
