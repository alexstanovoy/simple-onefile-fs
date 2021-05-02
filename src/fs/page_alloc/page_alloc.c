#include <fs/headers.h>
#include <stddef.h>


page_header* page_alloc_file_system(file_system* fs) {
  for (uint32_t page_index = 0; page_index < fs->pages_count; ++page_index) {
    if (fs->is_page_free[page_index]) {
      fs->is_page_free[page_index] = 0;
      --fs->free_pages_count;
      return (page_header*)((uint8_t*)fs->root_directory + PAGE_SIZE * page_index);
    }
  }
  return NULL;
}

void page_free_file_system(file_system* fs, page_header* page) {
  uint32_t page_index = ((uint8_t*)page - (uint8_t*)fs->root_directory) / PAGE_SIZE;
  fs->is_page_free[page_index] = 0;
  ++fs->free_pages_count;
}
