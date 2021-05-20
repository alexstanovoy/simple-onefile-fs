#ifndef WRITE_H
#define WRITE_H

#include <stdint.h>
#include <sys/types.h>
#include <fs/headers.h>


ssize_t write_file_system(file_system* fs, const char* path, const void* buf,
                          size_t count, size_t off);

ssize_t read_file_system(file_system* fs, const char* path, void* buf,
                         size_t count, size_t off);

#endif
