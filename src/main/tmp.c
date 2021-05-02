#include <stdio.h>
#include <fs/fs.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>


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

  int cmd_lim = 256;
  int path_lim = 256;
  char cmd[cmd_lim];
  char path[path_lim];
  while (true) {
    scanf("%s", cmd);
    if (strcmp(cmd, "mkdir") == 0) {
      scanf("%s", path);
      printf("result: %d\n", mkdir_file_system(fs, path));
    } else if (strcmp(cmd, "rmdir") == 0) {
      scanf("%s", path);
      printf("result: %d\n", rmdir_file_system(fs, path));
    } else if (strcmp(cmd, "find") == 0) {
      scanf("%s", path);
      printf("result: %d\n", find_file_system(fs, path));
    } else if (strcmp(cmd, "mkfile") == 0) {
      scanf("%s", path);
      printf("result: %d\n", mkfile_file_system(fs, path));
    } else if (strcmp(cmd, "rmfile") == 0) {
      scanf("%s", path);
      printf("result: %d\n", rmfile_file_system(fs, path));
    } else if (strcmp(cmd, "ls") == 0) {
      scanf("%s", path);
      char** entries = ls_file_system(fs, path);
      if (entries == NULL) {
        puts("ls error!");
        continue;
      }
      for (char** ptr = entries; *ptr != NULL; ++ptr) {
        puts(*ptr);
      }
      free_ls_file_system(entries);
    } else {
      puts("unknown command!");
      break;
    }
  }

  close_file_system(fs);
}
