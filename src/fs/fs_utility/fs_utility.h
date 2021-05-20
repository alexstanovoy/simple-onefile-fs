#ifndef INODE_UTILITY_H
#define INODE_UTILITY_H

#include <stdint.h>
#include <fs/headers.h>


uint32_t inode_number_file_system(file_system* fs, inode_header* inode);

uint32_t pages_count_file_system(file_system* fs);

uint32_t free_pages_count_file_system(file_system* fs);

#endif
