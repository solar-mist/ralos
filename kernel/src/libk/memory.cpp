#include <libk/memory.hpp>

void* memcpy(void* dst, const void* src, size_t n)
{
    uint8_t* pdst = (uint8_t*)dst;
    const uint8_t* psrc = (const uint8_t*)src;
    for(size_t i = 0; i < n; i++)
        pdst[i] = psrc[i];
    return dst;
}

void* memset(void* dst, char c, size_t n)
{
    uint8_t* pdst = (uint8_t*)dst;
    for(size_t i = 0; i < n; i++)
        pdst[i] = c;
    return dst;
}

void* memmove(void* dst, const void* src, size_t n)
{
    uint8_t* pdst = (uint8_t*)dst;
    const uint8_t* psrc = (const uint8_t*)src;
    if(src > dst)
    {
        for(size_t i = 0; i < n; i++)
            pdst[i] = psrc[i];
    }
    else if(src < dst)
    {
        for(size_t i = n; i > 0; i--)
            pdst[i - 1] = psrc[i - 1];
    }
    return dst;
}

int memcmp(const void* dst, const void* src, size_t n)
{
    const uint8_t* pdst = (const uint8_t*)dst;
    const uint8_t* psrc = (const uint8_t*)src;
    for(size_t i = 0; i < n; i++)
    {
        if(pdst[i] != psrc[i])
            return pdst[i] < psrc[i] ? -1 : 1;
    }
    return 0;
}

size_t strlen(const char* s)
{
    size_t ret = 0;
    while(s[ret])
        ret++;
    return ret;
}