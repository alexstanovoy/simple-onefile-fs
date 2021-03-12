#pragma once

#include <stdbool.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define MAX_PATH_LENGTH 4096
#define OBJECT_NAME_MAX_LENGTH 32

enum ObjectType {
  Directory,
  File,
};

struct Object {
  enum ObjectType type;
  char name[OBJECT_NAME_MAX_LENGTH + 1];
  size_t total_size;
  size_t next_object_index;
  size_t size_here;
};

struct File {
  enum ObjectType type;
  char name[OBJECT_NAME_MAX_LENGTH + 1];
  size_t total_size;
  size_t next_object_index;
  size_t size_here;
  char content[];
};

struct Directory {
  enum ObjectType type;
  char name[OBJECT_NAME_MAX_LENGTH + 1];
  size_t total_size;
  size_t next_object_index;
  size_t size_here;
  size_t childs_count_here;
  size_t childs[];
};

struct FileSystem {
  size_t size;
  size_t object_count;
  struct Object* first_object;
  void* file_descriptors;
  bool is_object_used[];
};
