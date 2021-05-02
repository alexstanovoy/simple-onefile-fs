#include <fs/headers.h>
#include <fs/fs_utility/fs_utility.h>


uint32_t inode_number_file_system(file_system* fs, inode_header* inode) {
  return ((uint8_t*)fs->root_directory - (uint8_t*)inode) / PAGE_SIZE;
}

uint32_t total_size_file_system(file_system* fs) {
  return fs->pages_count * (PAGE_SIZE - sizeof(file_inode));
}

uint32_t total_free_file_system(file_system* fs) {
  return fs->free_pages_count * (PAGE_SIZE - sizeof(file_inode));
}

uint32_t total_used_file_system(file_system* fs) {
  return (fs->pages_count - fs->free_pages_count) * (PAGE_SIZE - sizeof(file_inode));
}
