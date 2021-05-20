#include <fs/headers.h>
#include <fs/fs_utility/fs_utility.h>


uint32_t inode_number_file_system(file_system* fs, inode_header* inode) {
  return ((uint8_t*)inode - (uint8_t*)fs->root_directory) / PAGE_SIZE;
}

uint32_t pages_count_file_system(file_system* fs) {
  return fs->pages_count;
}

uint32_t free_pages_count_file_system(file_system* fs) {
  return fs->free_pages_count;
}
