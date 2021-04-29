cc_library(
  name = 'raw-vector',
  srcs = ['raw_vector.c'],
  hdrs = ['raw_vector.h'],
  includes = ['.'],
)

cc_library(
  name = 'char-vector',
  srcs = ['char_vector.c'],
  hdrs = [
    'raw_vector.h',
    'char_vector.h'
  ],
  includes = ['.'],
  deps = ['//src/vector:raw-vector'],
)
