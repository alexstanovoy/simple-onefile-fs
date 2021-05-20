#ifndef CHMOD_H
#define CHMOD_H

#include <sys/types.h>
#include <fs/headers.h>


int chmod_file_system(file_system* fs, const char* path, mode_t mode);

#endif
