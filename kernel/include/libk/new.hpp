#ifndef LIBK_NEW_HPP
#define LIBK_NEW_HPP
#include <stddef.h>

void* operator new(size_t s, char* buf);

void* operator new[](size_t s, char* buf);

#endif