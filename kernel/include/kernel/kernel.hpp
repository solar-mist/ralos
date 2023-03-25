#ifndef KERNEL_KERNEL_HPP
#define KERNEL_KERNEL_HPP

namespace Kernel
{
    [[noreturn]] void Panic(const char* fmt, ...);
}

#endif