#ifndef SYSCALL_MEM_HPP
#define SYSCALL_MEM_HPP
#include <stddef.h>

int SysMmap(void* addr, size_t bytes);

#endif