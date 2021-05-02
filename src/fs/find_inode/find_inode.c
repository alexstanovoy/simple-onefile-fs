#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <fs/headers.h>
#include <fs/utility.h>


inode_header* get_inode_from_directory(directory_inode* dir, const char* name) {
  for (uint32_t index = 0; index < MAX_DIRECTORY_ENTRIES; ++index) {
    if (dir->entries[index] == NULL) {
      continue;
    }
    if (strcmp(dir->entries[index]->name, name) == 0) {
      return dir->entries[index];
    }
  }
  return NULL;
}

inode_header* find_inode_file_system(file_system* fs, const char* path) {
  size_t length = strlen(path);
  RAII(cleanup_free) char* path_copy = malloc(length + 1);
  if (path_copy == NULL) {
    exit(1);
  }
  strcpy(path_copy, path);
  char* next_token = strtok(path_copy, PATH_DELIMITER_STR);
  inode_header* inode = (inode_header*)fs->root_directory;
  while (next_token != NULL) {
    switch (inode->type) {
      case Directory:
        inode = get_inode_from_directory((directory_inode*)inode, next_token);
        if (inode == NULL) {
          return NULL;
        }
        break;
      case File:
        return NULL;
      default:
        exit(1);
    }
    next_token = strtok(NULL, PATH_DELIMITER_STR);
  }
  return inode;
}
