#ifndef LIBK_MEM_HPP
#define LIBK_MEM_HPP
#include <stdint.h>
#include <stddef.h>

void memcpy(void* dest, const void* src, size_t count);
void memset(uint8_t* buffer, uint8_t n, uint32_t size);

void strcpy(char* dest, const char* src);
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);

#endif