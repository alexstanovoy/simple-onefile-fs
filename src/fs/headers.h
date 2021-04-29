#ifndef HEADERS_H
#define HEADERS_H

#define PAGE_SIZE        ((uint32_t)4096)
#define NAME_MAX_LENGTH  ((uint32_t)255)

#define MINIMUM_FILESYSTEM_SIZE (10 * PAGE_SIZE)

#include <stdbool.h>
#include <stdint.h>


typedef struct {
  uint32_t used_bytes_count;
  void* next_page_ptr;
} page_header;

typedef enum {
  Directory,
  File,
} INodeType;

typedef struct {
  page_header page_info;
  INodeType type;
  char name[NAME_MAX_LENGTH + 1];
} inode_header;

typedef struct {
  inode_header inode_info;
  uint8_t entry_count;
  inode_header* entries[];
} directory_inode;

typedef struct {
  inode_header inode_info;
  uint32_t size;
  uint8_t data[];
} file_inode;

typedef struct {
  directory_inode* root_directory;
  uint32_t source_size;
  uint32_t pages_count;
  uint8_t free_pages_mask[];
} file_system;

#endif
