#ifndef SYSCALL_FILE_HPP
#define SYSCALL_FILE_HPP
#include <stdint.h>
#include <stddef.h>

int SysOpen(const char* filename, uint8_t flags); // 0
int SysRead(int fd, char* buf, size_t count); // 1
int SysWrite(int fd, const void* buf, size_t count); // 2
int SysClose(int fd); // 3

#endif