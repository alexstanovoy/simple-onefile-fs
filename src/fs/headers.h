#ifndef HEADERS_H
#define HEADERS_H

#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

#define PAGE_SIZE                ((uint32_t)4096)
#define MAX_NAME_LENGTH          ((uint32_t)255)
#define MAX_PATH_LENGTH          ((uint32_t)65535)
#define MINIMUM_FILESYSTEM_SIZE  (10 * PAGE_SIZE)
#define PATH_DELIMITER           '/'
#define PATH_DELIMITER_STR       "/"
#define DEFAULT_MODE             0644


typedef struct {} page_header;

typedef enum {
  Directory,
  File,
} INodeType;

typedef struct {
  page_header page_info;
  INodeType type;
  char name[MAX_NAME_LENGTH + 1];
  uid_t uid;
  gid_t gid;
  mode_t mode;
  struct timespec st_atim;
  struct timespec st_mtim;
  struct timespec st_ctim;
} inode_header;

typedef struct {
  inode_header inode_info;
  uint64_t entry_count;
  inode_header* entries[];
} directory_inode;

typedef struct __continuation {
  page_header page_info;
  struct __continuation* next;
  int8_t content[];
} continuation;

typedef struct {
  inode_header inode_info;
  continuation* next;
  uint64_t size;
  uint8_t content[];
} file_inode;

typedef struct {
  directory_inode* root_directory;
  uint64_t source_size;
  uint64_t pages_count;
  uint64_t free_pages_count;
  uint8_t is_page_free[];
} file_system;


inode_header* init_inode(inode_header* inode, INodeType type,
                         const char* name);

directory_inode* init_directory_inode(directory_inode* dir, const char* name);

file_inode* init_file_inode(file_inode* file, const char* name);

void cleanup_free(void* ptr);

#define MAX_DIRECTORY_ENTRIES  \
    ((uint32_t)((PAGE_SIZE - sizeof(directory_inode)) / sizeof(inode_header*)))
#define RAII(destructor)  __attribute__((cleanup(destructor)))

#endif
