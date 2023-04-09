#include <libk/new.hpp>

void* operator new(size_t s, char* buf)
{
    return buf;
}

void* operator new[](size_t s, char* buf)
{
    return buf;
}