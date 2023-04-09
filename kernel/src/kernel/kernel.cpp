#include <kernel/kernel.hpp>
#include <kernel/modules.hpp>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <drivers/pic.hpp>
#include <drivers/pit.hpp>
#include <cpu/gdt/gdt.hpp>
#include <cpu/gdt/tss.hpp>
#include <cpu/interrupt/idt.hpp>
#include <mm/pmm.hpp>
#include <mm/paging.hpp>
#include <mm/vmm.hpp>
#include <proc/elf.hpp>
#include <proc/process.hpp>

extern "C" void _start()
{
    InitGDT();
    InitTSS();
    InitIDT();

    Graphics::Init();
    Terminal::Init();

    PMM::Init();
    Paging::Init();
    VMM::Init();

    Paging::AddressSpace addrspace = Paging::CreateAddressSpace();
    Paging::SwitchAddrSpace(&addrspace);
    ELF::Executable code = ELF::ParseELF(GetModule(1)->address, addrspace);
    Process proc = Process((uint64_t)code.entry, addrspace);
    proc.Launch();

    PIC::Init();
    PIT::Init(1197);

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