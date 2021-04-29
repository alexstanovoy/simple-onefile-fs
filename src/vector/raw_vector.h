#ifndef RAW_VECTOR_H
#define RAW_VECTOR_H

#include <stdint.h>
#include <stdlib.h>


typedef struct {
  uint8_t* ptr;
  size_t element_size;
  size_t count;
  size_t capacity;
} raw_vector;

void raw_vector_init(raw_vector* vec);

void raw_vector_push_back(raw_vector* vec, uint8_t element);

void raw_vector_destroy(raw_vector* vec);

uint8_t* raw_vector_get_data(raw_vector* vec);

size_t raw_vector_count(raw_vector* vec);

#endif
