#ifndef LIBK_MEMORY_HPP
#define LIBK_MEMORY_HPP
#include <stdint.h>
#include <stddef.h>

void* memcpy(void* dst, const void* src, size_t n);
void* memset(void* dst, char c, size_t n);
void* memmove(void* dst, const void* src, size_t n);
int   memcmp(const void* dst, const void* src, size_t n);

size_t strlen(const char* s);

#endif