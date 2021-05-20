#ifndef CHOWN_H
#define CHOWN_H

#include <sys/types.h>
#include <fs/headers.h>


int chown_file_system(file_system* fs, const char* path, uid_t uid, gid_t gid);

#endif
