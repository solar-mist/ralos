#ifndef LIBK_MEM_HPP
#define LIBK_MEM_HPP
#include <stdint.h>
#include <stddef.h>

void memcpy(void* dest, void* src, size_t count);
void memset(uint8_t* buffer, uint8_t n, uint32_t size);

#endif