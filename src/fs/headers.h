#ifndef HEADERS_H
#define HEADERS_H

#include <stdbool.h>
#include <stdint.h>

#define PAGE_SIZE                ((uint32_t)4096)
#define MAX_NAME_LENGTH          ((uint32_t)255)
#define MAX_PATH_LENGTH          ((uint32_t)65535)
#define MINIMUM_FILESYSTEM_SIZE  (10 * PAGE_SIZE)
#define PATH_DELIMITER           '/'
#define PATH_DELIMITER_STR       "/"


typedef struct {} page_header;

typedef enum {
  Directory,
  File,
} INodeType;

typedef struct {
  page_header page_info;
  INodeType type;
  char name[MAX_NAME_LENGTH + 1];
} inode_header;

typedef struct {
  inode_header inode_info;
  uint32_t entry_count;
  inode_header* entries[];
} directory_inode;

typedef struct __continuation_inode {
  page_header page_info;
  struct __continuation_inode* next_inode;
  int8_t content[];
} continuation_inode;

typedef struct {
  inode_header inode_info;
  continuation_inode* next_inode;
  uint32_t size;
  uint8_t content[];
} file_inode;

typedef struct {
  directory_inode* root_directory;
  uint32_t source_size;
  uint32_t pages_count;
  uint32_t free_pages_count;
  uint8_t is_page_free[];
} file_system;


directory_inode* init_directory_inode(directory_inode* dir, const char* name);

file_inode* init_file_inode(file_inode* file, const char* name);

void cleanup_free(void* ptr);

#define MAX_DIRECTORY_ENTRIES  \
    ((uint32_t)((PAGE_SIZE - sizeof(directory_inode)) / sizeof(inode_header*)))
#define RAII(destructor)  __attribute__((cleanup(destructor)))

#endif
