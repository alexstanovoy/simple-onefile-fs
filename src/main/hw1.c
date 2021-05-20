#include <fuse3/fuse.h>
#include <fs/fs.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>


typedef struct {
  char* cmd;
  char* file_name;
} my_options_t;

my_options_t my_options;

file_system* fs;

void* my_init(struct fuse_conn_info* conn, struct fuse_config* cf) {
  (void)conn;
  (void)cf;
  if (strcmp(my_options.cmd, "open") == 0) {
    fs = open_file_system(my_options.file_name);
  } else if (strcmp(my_options.cmd, "init") == 0) {
    fs = init_file_system(my_options.file_name);
  } else {
    exit(1);
  }
  return NULL;
}

void my_destroy(void* private_data) {
  (void)private_data;
  close_file_system(fs);
}

int my_readdir(const char* path, void* out, fuse_fill_dir_t filler, off_t off,
               struct fuse_file_info* fi, enum fuse_readdir_flags flags) {
  (void)off;
  (void)fi;
  (void)flags;
  char** entries = ls_file_system(fs, path);
  if (entries == NULL) {
    return -ENOENT;
  }
  filler(out, ".", NULL, 0, 0);
  filler(out, "..", NULL, 0, 0);
  for (char** ptr = entries; *ptr != NULL; ++ptr) {
    filler(out, *ptr, NULL, 0, 0);
  }
  free_ls_file_system(entries);
  return 0;
}

int my_stat(const char* path, struct stat *st, struct fuse_file_info* fi) {
  puts("stat!");
  puts(path);
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

int my_open(const char* path, struct fuse_file_info* fi) {
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

int my_write(const char* path, const char* buf, size_t size, off_t off,
             struct fuse_file_info* fi) {
  (void)fi;
  if (is_file_file_system(fs, path) < 0) {
    return -ENOENT;
  }
  return write_file_system(fs, path, buf, size, off);
}

int my_mkdir(const char* path, mode_t mode) {
  (void)mode;
  return mkdir_file_system(fs, path);
}

int my_rmdir(const char* path) {
  return rmdir_file_system(fs, path);
}

int my_create(const char* path, mode_t mode, struct fuse_file_info* fi) {
  (void)mode;
  (void)fi;
  return mkfile_file_system(fs, path);
}

struct fuse_operations operations = {
  .readdir = my_readdir,
  .getattr = my_stat,
  .open = my_open,
  .read = my_read,
  .write = my_write,
  .mkdir = my_mkdir,
  .rmdir = my_rmdir,
  .create = my_create,
  .destroy = my_destroy,
  .init = my_init,
};

int main(int argc, char** argv) {
  struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

  struct fuse_opt opt_specs[] = {
    { "--open-type %s", offsetof(my_options_t, cmd), 0 },
    { "--file %s", offsetof(my_options_t, file_name), 0 },
    FUSE_OPT_END
  };

  if (fuse_opt_parse(&args, &my_options, opt_specs, NULL) < 0) {
    exit(1);
  }

  if (fuse_main(args.argc, args.argv, &operations, NULL) < 0) {
    exit(1);
  }
}
