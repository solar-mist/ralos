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

void test()
{
    for(;;);
}

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

    void* stack = PMM::GetPage();
    Paging::MapPage((uint64_t)stack, 0x400000, 7);

    enter_usermode(test, (void*)0x400FFF);
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