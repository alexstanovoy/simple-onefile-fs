#pragma once

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  char* ptr;
  size_t count;
  size_t capacity;
} Vector;

void VectorInit(Vector* vec) {
  vec->count = 0;
  vec->capacity = 10;
  vec->ptr = malloc(vec->capacity);
}

void VectorPushBack(Vector* vec, char value) {
  if (vec->count == vec->capacity) {
    vec->capacity *= 2;
    vec->ptr = realloc(vec->ptr, vec->capacity);
  }
  vec->ptr[vec->count++] = value;
}

void VectorAppend(Vector* vec, char* ptr) {
  while (*ptr != '\0') {
    VectorPushBack(vec, *ptr);
  }
}

void VectorDestroy(Vector* vec) {
  free(vec->ptr);
}
