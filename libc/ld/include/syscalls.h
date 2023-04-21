#ifndef SYSCALL_H
#define SYSCALL_H
#include <stddef.h>
#include <stdint.h>

int open(const char* filename, unsigned char flags);
int read(int fd, void* buf, size_t count);
int write(int fd, const void* buf, size_t count);
int close(int fd);
uint64_t mmap(void* addr, size_t count);
size_t stat(int fd);

#endif