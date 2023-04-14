#include <stddef.h>

int strlen(const char* str)
{
    int ret = 0;
    while(str[ret])
        ret++;
    return ret;
}

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

int write(int fd, const void* buf, size_t count);

int close(int fd)
{
    int ret;
    asm volatile("mov $3, %%rax; mov %1, %%rdi; int $0x69" : "=a"(ret) : "m"(fd));
    return ret;
}

void _start()
{
    int fd = open("/test", 1);
    char buffer[32];
    int count = read(fd, buffer, 32);
    close(fd);
    if(count != -1)
        write(1, buffer, count);
    else
        write(1, "error", strlen("error"));
    for(;;);
}