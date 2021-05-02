#ifndef PAGE_ALLOC_H
#define PAGE_ALLOC_H

#include <fs/headers.h>


page_header* page_alloc_file_system(file_system* fs);

void page_free_file_system(file_system* fs, page_header* page);

#endif
