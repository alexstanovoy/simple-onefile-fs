#ifndef MKDIR_H
#define MKDIR_H

#include <fs/headers.h>


int mkdir_file_system(file_system* fs, const char* path);

int rmdir_file_system(file_system* fs, const char* path);

#endif
