#include <string.h>
#include <stdlib.h>
#include <fs/utility.h>
#include <fs/ls/ls.h>
#include <fs/find_inode/find_inode.h>


char** ls_file_system(file_system* fs, const char* path) {
  if (verify_path(path) < 0) {
    return NULL;
  }
  directory_inode* dir = (directory_inode*)find_inode_file_system(fs, path);
  if (dir == NULL || dir->inode_info.type != Directory) {
    return NULL;
  }
  char** entries = malloc(sizeof(char*) * (dir->entry_count + 1));
  if (entries == NULL) {
    exit(1);
  }
  entries[dir->entry_count] = NULL;
  char** ptr = entries;
  for (size_t index = 0; index < MAX_DIRECTORY_ENTRIES; ++index) {
    if (dir->entries[index] != NULL) {
      *ptr = malloc(MAX_NAME_LENGTH + 1);
      if (*ptr == NULL) {
        exit(1);
      }
      strcpy(*ptr, dir->entries[index]->name);
      ++ptr;
    }
  }
  return entries;
}

void free_ls_file_system(char** content) {
  if (content == NULL) {
    return;
  }
  for (char** cur = content; *cur != NULL; ++cur) {
    free(*cur);
  }
  free(content);
}
