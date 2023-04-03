#ifndef KERNEL_KERNEL_HPP
#define KERNEL_KERNEL_HPP

namespace Kernel
{
    [[noreturn]] void Panic();
    [[noreturn]] void Panic(const char* message);
    [[noreturn]] extern void Panic(const char* format, ...);
}

#endif