#ifndef KERNEL_KERNEL_HPP
#define KERNEL_KERNEL_HPP
#include <drivers/io/terminal.hpp>

namespace Kernel
{
    [[noreturn]] [[gnu::always_inline]] inline void Panic(const char* fmt, ...)
    {
        Terminal::Printf(fmt, __builtin_va_arg_pack());
        for(;;)
            asm volatile("cli; hlt");
    }
}

#endif