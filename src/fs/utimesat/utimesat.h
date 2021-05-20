#ifndef UTIMES_H
#define UTIMES_H

#include <sys/times.h>
#include <fs/headers.h>


int utimesat_file_system(file_system* fs, const char* path,
                         const struct timespec times[2]);

int utimensat_file_system(file_system* fs, const char* path,
                          const struct timespec times[2]);

#endif
