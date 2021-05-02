#include <fuse3/fuse.h>
#include <fs/fs.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


typedef struct {
  char* open_type;
  char* file;
} my_options_t;

file_system* fs;

int my_readdir(const char *path, void *out, fuse_fill_dir_t filler, off_t off,
               struct fuse_file_info* fi, enum fuse_readdir_flags flags) {
  (void)off;
  (void)fi;
  (void)flags;
  filler(out, ".", NULL, 0, 0);
  filler(out, "..", NULL, 0, 0);
  char** entries = ls_file_system(fs, path);
  if (entries == NULL) {
    return -1;
  }
  for (char** ptr = entries; *ptr != NULL; ++ptr) {
    filler(out, *ptr, NULL, 0, 0);
  }
  free_ls_file_system(entries);
  return 0;
}

int my_stat(const char *path, struct stat *st, struct fuse_file_info* fi) {
  (void)fi;
  if (strcmp(path, "/") == 0) {
    st->st_mode = S_IFDIR | 0555;
    st->st_nlink = 2;
    return 0;
  }
  if (stat_file_system(fs, path, st) < 0) {
    return -ENOENT;
  }
  return 0;
}

int my_open(const char *path, struct fuse_file_info* fi) {
  if (is_file_file_system(fs, path) < 0) {
    return -ENOENT;
  }
  if (O_RDONLY != (fi->flags & O_ACCMODE)) {
    return -EACCES;
  }
  return 0;
}

int my_read(const char* path, char* out, size_t size, off_t off,
            struct fuse_file_info* fi) {
  (void)fi;
  if (is_file_file_system(fs, path) < 0) {
    return -ENOENT;
  }
  return read_file_system(fs, path, out, size, off);
}

int my_mkdir(const char* path, mode_t mode) {
  (void)mode;
  return mkdir_file_system(fs, path);
}

struct fuse_operations operations = {
  .readdir = my_readdir,
  .getattr = my_stat,
  .open = my_open,
  .read = my_read,
  //.write
  .mkdir = my_mkdir,
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
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  my_options_t my_options;
  memset(&my_options, 0, sizeof(my_options));

  struct fuse_opt opt_specs[] = {
    { "--open-type %s", offsetof(my_options_t, open_type), 0 },
    { "--file %s", offsetof(my_options_t, file), 0 },
    FUSE_OPT_END
  };

  if (fuse_opt_parse(&args, &my_options, opt_specs, NULL) < 0) {
    exit(1);
  }

  fs = parse_args(my_options.open_type, my_options.file);
  if (fuse_main(args.argc, args.argv, &operations, NULL) < 0) {
    exit(1);
  }
  close_file_system(fs);
}
