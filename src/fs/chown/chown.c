#include <fs/utility.h>
#include <fs/chown/chown.h>
#include <fs/find_inode/find_inode.h>


int chown_file_system(file_system* fs, const char* path, uid_t uid, gid_t gid) {
  if (verify_path(path) < 0) {
    return -1;
  }
  inode_header* inode = find_inode_file_system(fs, path);
  if (inode == NULL) {
    return -1;
  }
  inode->uid = uid;
  inode->gid = gid;
  return 0;
}
