#include <kernel/kernel.hpp>
#include <kernel/modules.hpp>
#include <kernel/sched/scheduler.hpp>
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
#include <mm/kheap.hpp>
#include <proc/elf.hpp>
#include <proc/process.hpp>
#include <fs/vfs.hpp>
#include <fs/tmpfs.hpp>

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
    InitKHeap();

    Scheduler::Init();

    /*Paging::AddressSpace addrspace = Paging::CreateAddressSpace();
    Paging::SwitchAddrSpace(&addrspace);
    ELF::Executable code = ELF::ParseELF(GetModule(1)->address, addrspace);
    Process proc = Process((uint64_t)code.entry, addrspace);
    
    Paging::AddressSpace addrspace2 = Paging::CreateAddressSpace();
    Paging::SwitchAddrSpace(&addrspace2);
    ELF::Executable code2 = ELF::ParseELF(GetModule(2)->address, addrspace2);
    Process proc2 = Process((uint64_t)code2.entry, addrspace2);

    PIC::Init();
    PIT::Init(1197);

    Scheduler::AddProcess(proc);
    Scheduler::AddProcess(proc2);

    Scheduler::Start();*/
    TmpFS::Init();
    VFS::Filesystem* tmpfs = VFS::GetFileSystem("tmpfs");
    tmpfs->create("/test");
    VFS::Node node;
    tmpfs->open("/test", &node);
    tmpfs->write(&node, "hello", 5);
    char* buffer = (char*)malloc(6);
    tmpfs->read(&node, buffer, 5);
    buffer[5] = 0;
    Terminal::Print(buffer);

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