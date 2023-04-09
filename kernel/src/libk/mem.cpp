#include <libk/mem.hpp>

void memcpy(void* dest, void* src, size_t count)
{
    for(size_t i = 0; i < count; i++)
        ((char*)dest)[i] = ((char*)src)[i];
}

void memset(uint8_t* buffer, uint8_t n, uint32_t size)
{
    for(uint32_t i = 0; i < size; i++)
        buffer[i] = n;
}