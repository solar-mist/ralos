#ifndef MEM_KHEAP_HPP
#define MEM_KHEAP_HPP
#include <stddef.h>

void* KMalloc(size_t size);
void  KFree(void* address);

#endif