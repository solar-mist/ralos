#include <kernel/util.h>

void* memset(void* dst, int val, int len)
{
    unsigned char *ptr = dst;
    while(len-- > 0)
        *ptr++ = val;
    return dst;
}