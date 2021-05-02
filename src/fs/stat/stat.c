#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fs/utility.h>
#include <fs/stat/stat.h>
#include <fs/fs_utility/fs_utility.h>
#include <fs/find_inode/find_inode.h>


int stat_file_system(file_system* fs, const char* path, struct stat* st) {
  if (verify_path(path) < 0) {
    return -1;
  }
  inode_header* inode = find_inode_file_system(fs, path);
  if (inode == NULL) {
    return -1;
  }
  *st = (struct stat){
    .st_dev = 0,
    .st_ino = inode_number_file_system(fs, inode),
    .st_mode = S_IFREG | 0777,
    .st_nlink = 1,
    .st_uid = 0,
    .st_gid = 0,
    .st_rdev = 0,
    .st_blksize = 0,
    .st_blocks = 0,
    .st_atim = {0},
    .st_mtim = {0},
    .st_ctim = {0},
  };
  switch (inode->type) {
    case Directory:
      ++st->st_nlink;
      directory_inode* dir = (directory_inode*)inode;
      for (size_t index = 0; index < MAX_DIRECTORY_ENTRIES; ++index) {
        if (dir->entries[index] != NULL && dir->entries[index]->type == Directory) {
          ++st->st_nlink;
        }
      }
      st->st_size = PAGE_SIZE;
      break;
    case File:
      st->st_size = ((file_inode*)inode)->size;
      break;
    default:
      exit(1);
  }
  return 0;
}
