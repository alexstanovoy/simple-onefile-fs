#include <fs/headers.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


int verify_path(char* path) {
  if (path[0] != PATH_DELIMITER) {
    return -1;
  }
  if (strcmp(path, "/") == 0) {
    return 0;
  }
  size_t index = 1;
  size_t name_length = 0;
  while (index <= MAX_PATH_LENGTH && path[index] != '\0') {
    if (path[index] == PATH_DELIMITER) {
      if (0 < name_length && name_length <= MAX_NAME_LENGTH) {
        name_length = 0;
      } else {
        return -1;
      }
    } else {
      ++name_length;
    }
    ++index;
  }
  if (index <= MAX_PATH_LENGTH && path[index] == '\0' &&
      0 < name_length && name_length <= MAX_NAME_LENGTH) {
    return 0;
  }
  return -1;
}

const char* get_parents_only_ptr(const char* path) {
  const char* ptr = NULL;
  while (*path != '\0') {
    if (*path == PATH_DELIMITER) {
      ptr = path;
    }
    ++path;
  }
  return ptr;
}

char* get_parents_only(const char* path) {
  const char* ptr = get_parents_only_ptr(path);
  size_t length = ptr - path;
  char* parents = malloc(length + 1);
  if (parents == NULL) {
    exit(1);
  }
  strncpy(parents, path, length);
  parents[length] = '\0';
  return parents;
}

char* get_name_only(const char* path) {
  const char* ptr = get_parents_only_ptr(path) + 1;
  size_t length = strlen(ptr);
  char* name = malloc(length + 1);
  if (name == NULL) {
    exit(1);
  }
  strncpy(name, ptr, length);
  name[length] = '\0';
  return name;
}

uint32_t min(uint32_t a, uint32_t b) {
  return a < b ? a : b;
}

uint32_t max(uint32_t a, uint32_t b) {
  return a > b ? a : b;
}
