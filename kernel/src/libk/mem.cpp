#include <libk/mem.hpp>

void memcpy(void* dest, const void* src, size_t count)
{
    for(size_t i = 0; i < count; i++)
        ((char*)dest)[i] = ((char*)src)[i];
}

void memset(uint8_t* buffer, uint8_t n, uint32_t size)
{
    for(uint32_t i = 0; i < size; i++)
        buffer[i] = n;
}

int memcmp(uint8_t* m1, uint8_t *m2, size_t count)
{
    for (size_t i = 0; i < count; i++, m1++, m2++)
    {
        if (*m1 < *m2)
            return -1;
        else if (*m1 > *m2)
            return 1;
    }

    return 0;
}

void strcpy(char* dest, const char* src)
{
    while(*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
}

size_t strlen(const char* str)
{
    size_t ret = 0;
    while(str[ret])
        ret++;
    return ret;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}