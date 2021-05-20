#include <fs/utility.h>
#include <fs/chmod/chmod.h>
#include <fs/find_inode/find_inode.h>


int chmod_file_system(file_system* fs, const char* path, mode_t mode) {
  if (verify_path(path) < 0) {
    return -1;
  }
  inode_header* inode = find_inode_file_system(fs, path);
  if (inode == NULL) {
    return -1;
  }
  inode->mode = mode;
  return 0;
}
