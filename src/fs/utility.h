#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>
#include <fs/headers.h>


int verify_path(const char* path);

char* get_parents_only(const char* path);

char* get_name_only(const char* path);

uint32_t min(uint32_t a, uint32_t b);

uint32_t max(uint32_t a, uint32_t b);

#endif
