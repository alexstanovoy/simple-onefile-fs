#ifndef MKFILE_H
#define MKFILE_H

#include <fs/headers.h>


int mkfile_file_system(file_system* fs, const char* path);

int rmfile_file_system(file_system* fs, const char* path);

#endif
