#ifndef OPENFS_H
#define OPENFS_H

#include <fs/headers.h>


file_system* open_file_system(char* file_name);

file_system* init_file_system(char* file_name);

void close_file_system(file_system* fs);

#endif
