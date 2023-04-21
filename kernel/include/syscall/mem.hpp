#ifndef SYSCALL_MEM_HPP
#define SYSCALL_MEM_HPP
#include <stddef.h>
#include <stdint.h>

uint64_t SysMmap(void* addr, size_t bytes);

#endif