#include <syscalls.h>

int open(const char* filename, unsigned char flags)
{
    int ret;
    asm volatile("mov $0, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %2, %%rdx; int $0x69" : "=a"(ret) : "m"(filename), "m"(flags));
    return ret;
}

int read(int fd, void* buf, size_t count)
{
    int ret;
    asm volatile("mov $1, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %3, %%rdx; int $0x69" : "=a"(ret) : "m"(fd), "m"(buf), "m"(count));
    return ret;
}

int write(int fd, const void* buf, size_t count)
{
    int ret;
    asm volatile("mov $2, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %3, %%rdx; int $0x69" : "=a"(ret) : "m"(fd), "m"(buf), "m"(count));
    return ret;
}

int close(int fd)
{
    int ret;
    asm volatile("mov $3, %%rax; mov %1, %%rdi; int $0x69" : "=a"(ret) : "m"(fd));
    return ret;
}

uint64_t mmap(void* addr, size_t count)
{
    uint64_t ret;
    asm volatile("mov $4, %%rax; mov %1, %%rdi; mov %2, %%rsi; int $0x69" : "=a"(ret) : "m"(addr), "m"(count));
    return ret;
}

size_t stat(int fd)
{
    size_t ret;
    asm volatile("mov $5, %%rax; mov %1, %%rdi; int $0x69" : "=a"(ret) : "m"(fd));
    return ret;
}