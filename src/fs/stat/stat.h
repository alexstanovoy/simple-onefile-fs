#ifndef STAT_H
#define STAT_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fs/headers.h>


int stat_file_system(file_system* fs, const char* path, struct stat* st);

#endif
