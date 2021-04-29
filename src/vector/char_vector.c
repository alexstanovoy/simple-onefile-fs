#include <raw_vector.h>
#include <char_vector.h>


void char_vector_init(char_vector* vec) {
  raw_vector_init(vec);
}

void char_vector_push_back(char_vector* vec, char element) {
  raw_vector_push_back(vec, element);
}

void char_vector_destroy(char_vector* vec) {
  raw_vector_destroy(vec);
}

void char_vector_get_data(char_vector* vec) {
  
}

void char_vector_count(char_vector* vec) {

}
