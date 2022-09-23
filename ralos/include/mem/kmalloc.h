#ifndef RALOS_MEM_KMALLOC_H
#define RALOS_MEM_KMALLOC_H
#include <stddef.h>

void* k_malloc(size_t);
void k_free(void*);

#endif