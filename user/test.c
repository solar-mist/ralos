#include <stddef.h>

int strlen(const char* str)
{
    int ret = 0;
    while(str[ret])
        ret++;
    return ret;
}

int write(int fd, const void* buf, size_t count)
{
    asm volatile("mov $2, %%rax; mov %0, %%rdi; mov %1, %%rsi; mov %2, %%rdx; int $0x69" :: "m"(fd), "m"(buf), "m"(count));
    return 0;
}

void _start()
{
    const char* data = "hello";
    write(1, data, strlen(data));
    for(;;);
}