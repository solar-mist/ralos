#include <viperBoot.h>
#include <drivers/ui/graphics.hpp>
#include <drivers/ui/terminal.hpp>
#include <cpu/gdt/gdt.hpp>
#include <cpu/tss.hpp>
#include <cpu/interrupt/idt.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <drivers/pit.hpp>
#include <kernel/sched/scheduler.hpp>

void TestUserFunc()
{
    for(;;)
    {
    const char* data = "0";
    asm volatile("mov $0, %rax");
    asm volatile("mov %0, %%rdi" : : "m"(data));
    asm volatile("mov $1, %rsi");
    asm volatile("int $0x69");
    }
}

void OtherTestUserFunc()
{
    for(;;)
    {
        const char* data = "X";
        asm volatile("mov $0, %rax");
        asm volatile("mov %0, %%rdi" : : "m"(data));
        asm volatile("mov $1, %rsi");
        asm volatile("int $0x69");
    }
}

extern "C" void KMain(VBInfo* BootInfo)
{
    InitGDT();
    InitIDT();
    Graphics::Init(BootInfo);
    Terminal::Init(BootInfo->Framebuffer.Horiz, BootInfo->Framebuffer.Vert);
    PMM::Init(BootInfo->MemoryMap);
    PIT::Init(1197);
    InitTSS();
    PMM::DumpStats();
    VMM::Init(BootInfo);
    Terminal::Print("Hello World!\n", 0xFFFF00);
    Scheduler::AddTask(Scheduler::Task((void*)TestUserFunc));
    Scheduler::AddTask(Scheduler::Task((void*)OtherTestUserFunc));
    Terminal::PrintInt((uint64_t)(void*)OtherTestUserFunc, 16);
    Scheduler::Init();
    for(;;);
}