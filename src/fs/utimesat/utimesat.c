#include <string.h>
#include <fs/find_inode/find_inode.h>


int utimesat_file_system(file_system* fs, const char* path,
                         const struct timespec times[2]) {
  inode_header* inode = find_inode_file_system(fs, path);
  if (inode == NULL) {
    return -1;
  }
  memcpy(&inode->st_atim, &times[0], sizeof(struct timespec));
  memcpy(&inode->st_mtim, &times[1], sizeof(struct timespec));
  return 0;
}


int utimensat_file_system(file_system* fs, const char* path,
                         const struct timespec times[2]) {
  inode_header* inode = find_inode_file_system(fs, path);
  if (inode == NULL) {
    return -1;
  }
  inode->st_atim.tv_nsec = times[0].tv_nsec;
  inode->st_mtim.tv_nsec = times[0].tv_nsec;
  return 0;
}
