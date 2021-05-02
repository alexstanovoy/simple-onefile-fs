#ifndef FIND_INODE_H
#define FIND_INODE_H

#include <fs/headers.h>


inode_header* find_inode_file_system(file_system* fs, const char* path);

#endif
