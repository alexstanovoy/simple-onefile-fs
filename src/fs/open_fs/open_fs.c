#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <stddef.h>
#include <fs/headers.h>
#include <fs/open_fs/open_fs.h>


file_system* open_file_system(char* file_name) {
  if (file_name == NULL) {
    return NULL;
  }
  int fd = open(file_name, O_RDWR);
  if (fd < 0) {
    return NULL;
  }
  struct stat fl;
  fstat(fd, &fl);
  file_system* fs = (file_system*)mmap(
      NULL, (size_t)fl.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  close(fd);
  if (fs == NULL) {
    return NULL;
  }
  fs->source_size = (uint32_t)fl.st_size;
  return fs;
}

file_system* init_file_system(char* file_name) {
  file_system* fs = open_file_system(file_name);
  if (fs == NULL) {
    return NULL;
  }
  fs->pages_count = (
      fs->source_size - sizeof(file_system)) / (PAGE_SIZE + 1);
  fs->free_pages_count = fs->pages_count - 1;
  fs->root_directory = (directory_inode*)(
      (uint8_t*)fs + sizeof(file_system) + fs->pages_count);
  memset(fs->is_page_free, 1, fs->pages_count);
  fs->is_page_free[0] = 0;
  init_directory_inode(fs->root_directory, "");
  return fs;
}

void close_file_system(file_system* fs) {
  if (fs != NULL) {
    return;
  }
  munmap(fs, fs->source_size);
}
