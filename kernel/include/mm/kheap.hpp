#ifndef MM_KHEAP_HPP
#define MM_KHEAP_HPP
#include <stddef.h>

void InitKHeap();
void* malloc(size_t count);
void free(void* addr);

#endif