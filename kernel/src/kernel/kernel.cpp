#include <kernel/kernel.hpp>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <cpu/gdt/gdt.hpp>

extern "C" void _start()
{
    InitGDT();
    Graphics::Init();
    Terminal::Init();
    Terminal::Printf(0xFF00FF, "Hello world! Welcome to %#%s", 0xFFFF00, "viperOS");
    for(;;);
}

namespace Kernel
{
    [[noreturn]] void Panic()
    {
        for(;;)
            asm volatile("cli; hlt");
    }

    [[noreturn]] void Panic(const char* message)
    {
        Terminal::Printf(0xFF0000, "Kernel panic: %s", message);
        for(;;)
            asm volatile("cli; hlt");
    }

    [[noreturn]] void Panic(const char* format, ...)
    {
        Terminal::Printf(0xFF0000, "Kernel panic: ");
        va_list arg;
        va_start(arg, format);
        Terminal::VPrintf(0xFF0000, format, arg);
        va_end(arg);
        for(;;)
            asm volatile("cli; hlt");
    }
}