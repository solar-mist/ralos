#include <stddef.h>
int write(int fd, const void* buf, size_t count)
{
    int ret;
    asm volatile("mov $2, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %3, %%rdx; int $0x69" : "=a"(ret) : "m"(fd), "m"(buf), "m"(count));
    return ret;
}