#ifndef LS_H
#define LS_H

#include <fs/headers.h>


char** ls_file_system(file_system* fs, const char* path);

void free_ls_file_system(char** content);

#endif
