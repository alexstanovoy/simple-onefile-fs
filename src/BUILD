cc_library(
  name = 'fs',
  srcs = [
    'fs/find_inode/find_inode.c',
    'fs/find/find.c',
    'fs/headers.c',
    'fs/ls/ls.c',
    'fs/mkdir/mkdir.c',
    'fs/mkfile/mkfile.c',
    'fs/open_fs/open_fs.c',
    'fs/page_alloc/page_alloc.c',
    'fs/utility.c',
    'fs/fs_utility/fs_utility.c',
    'fs/stat/stat.c',
    'fs/write/write.c',
    'fs/utimesat/utimesat.c',
    'fs/chown/chown.c',
    'fs/chmod/chmod.c',
  ],
  hdrs = [
    'fs/find_inode/find_inode.h',
    'fs/find/find.h',
    'fs/fs.h',
    'fs/headers.h',
    'fs/ls/ls.h',
    'fs/mkdir/mkdir.h',
    'fs/mkfile/mkfile.h',
    'fs/open_fs/open_fs.h',
    'fs/page_alloc/page_alloc.h',
    'fs/utility.h',
    'fs/fs_utility/fs_utility.h',
    'fs/stat/stat.h',
    'fs/write/write.h',
    'fs/utimesat/utimesat.h',
    'fs/chown/chown.h',
    'fs/chmod/chmod.h',
  ],
  includes = ['.'],
)

cc_binary(
  name = 'hw1-debug',
  srcs = [
    'main/hw1.c',
    '//src:fs',
  ],
  defines = [
    '_FILE_OFFSET_BITS=64',
    'FUSE_USE_VERSION=30',
  ],
  copts = [
    '-O0',
    '-g'
  ],
  deps = ['//src:fs'],
  linkopts = [
    '-lfuse3',
    '-lpthread',
  ],
)

cc_binary(
  name = 'hw1',
  srcs = [
    'main/hw1.c',
    '//src:fs',
  ],
  defines = [
    '_FILE_OFFSET_BITS=64',
    'FUSE_USE_VERSION=30',
  ],
  deps = ['//src:fs'],
  linkopts = [
    '-lfuse3',
    '-lpthread',
  ],
)
