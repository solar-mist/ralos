#include <kernel/kernel.hpp>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <cpu/gdt/gdt.hpp>
#include <cpu/gdt/tss.hpp>
#include <cpu/interrupt/idt.hpp>
#include <mm/pmm.hpp>
#include <mm/paging.hpp>

extern "C" void enter_usermode(void (*func)(), void* stack);

extern "C" void _start()
{
    InitGDT();
    InitTSS();
    InitIDT();

    Graphics::Init();
    Terminal::Init();

    PMM::Init();
    Paging::Init();

    PIC::Init();
    PIT::Init(1197);

    Terminal::Printf(0xFF00, "Welcome to viperOS\n");

    Paging::MapPage((uint64_t)PMM::GetPage(), 0x400000, 7); // Stack
    Paging::MapPage((uint64_t)PMM::GetPage(), 0x4001000, 7); // Process code
    *(uint16_t*)0x4001000 = 0xfeeb; // Loop forever - eb fe
    
    enter_usermode((void(*)())0x4001000, (void*)0x400FFF);

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