cc_library(
  name = 'fs',
  srcs = [
    'fs/open/open.c',
  ],
  hdrs = [
    'fs/fs.h',
    'fs/headers.h',
    'fs/open/open.h',
  ],
  includes = ['.'],
)

cc_binary(
  name = 'hw1',
  srcs = [
    'main/hw1.c',
    '//src:fs',
  ],
  defines = [
    'FUSE_USE_VERSION=30',
    '_FILE_OFFSET_BITS=64',
  ],
  includes = ['fs'],
  deps = ['//src:fs'],
)
