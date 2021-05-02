#ifndef FIND_H
#define FIND_H

#include <fs/headers.h>


int find_file_system(file_system* fs, const char* path);

int is_directory_file_system(file_system* fs, const char* path);

int is_file_file_system(file_system* fs, const char* path);

#endif