#ifndef WRITE_H
#define WRITE_H

#include <stdint.h>
#include <fs/headers.h>


int write_file_system(file_system* fs, const char* path, const void* buf,
                      uint32_t count, uint32_t off);

int read_file_system(file_system* fs, const char* path, void* buf,
                     uint32_t count, uint32_t off);

#endif
