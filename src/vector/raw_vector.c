#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <raw_vector.h>


void raw_vector_init(raw_vector* vec) {
  assert(vec != NULL);
  vec->count = 0;
  vec->capacity = 10;
  vec->ptr = malloc(vec->capacity);
}

void raw_vector_push_back(raw_vector* vec, uint8_t value) {
  assert(vec != NULL);
  if (vec->count == vec->capacity) {
    vec->capacity *= 2;
    vec->ptr = realloc(vec->ptr, vec->capacity);
  }
  vec->ptr[vec->count++] = value;
}

void raw_vector_destroy(raw_vector* vec) {
  assert(vec != NULL);
  free(vec->ptr);
}

uint8_t* raw_vector_get_data(raw_vector* vec) {
  assert(vec != NULL);
  return vec->ptr;
}

size_t raw_vector_count(raw_vector* vec) {
  assert(vec != NULL);
  return vec->count;
}
