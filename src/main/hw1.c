#define FUSE_USE_VERSION 30

#include <fs/fs.h>
#include <fuse.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


struct fuse_operations operations = {
  //.readdir = my_readdir,
  //.getattr = my_stat,
  //.open = my_open,
  //.read = my_read,
};

file_system* parse_args(char* cmd, char* file_name) {
  if (strcmp(cmd, "open") == 0) {
    return open_file_system(file_name);
  } else if (strcmp(cmd, "init") == 0) {
    return init_file_system(file_name);
  }
  exit(1);
}

int main(int argc, char** argv) {
  assert(argc == 3);
  file_system* fs = parse_args(argv[1], argv[2]);
  close_file_system(fs);
}
