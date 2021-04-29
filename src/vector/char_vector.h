#ifndef CHAR_VECTOR_H
#define CHAR_VECTOR_H

#include <raw_vector.h>


typedef raw_vector char_vector;

void char_vector_init(char_vector* vec);

void char_vector_push_back(char_vector* vec, char element);

void char_vector_destroy(char_vector* vec);

void char_vector_get_data(char_vector* vec);

void char_vector_count(char_vector* vec);

#endif
