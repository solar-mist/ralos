#include <viperBoot.h>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <cpu/gdt/gdt.hpp>
#include <cpu/tss.hpp>
#include <cpu/interrupt/idt.hpp>
#include <mem/pmm.hpp>
#include <mem/vmm.hpp>
#include <drivers/pit.hpp>
#include <drivers/ata.hpp>
#include <drivers/ps2_keyboard.hpp>

extern "C" void JumpUsermode(void (*func)(), void* stack);

extern "C" uint64_t SyscallHandler;
void TestUserFunc()
{
    const char* data = "Hello Syscall World!";
    asm volatile("mov $0, %rax");
    asm volatile("mov %0, %%rdi" : : "m"(data));
    asm volatile("mov $20, %rsi");
    asm volatile("int $0x69");
    for(;;);
}

extern "C" void KMain(VBInfo* BootInfo)
{
    InitGDT();
    InitIDT();
    Graphics::Init(BootInfo);
    Terminal::Init(BootInfo->Framebuffer.Horiz, BootInfo->Framebuffer.Vert);
    PMM::Init(BootInfo->MemoryMap);
    InitTSS();
    PMM::DumpStats();
    VMM::Init(BootInfo);
    Terminal::Print("Hello World!\n", 0xFFFF00);
    //Terminal::PrintInt(SyscallHandler, 16, 0x00FFFF);
    JumpUsermode(TestUserFunc, PMM::GetPage());
    for(;;);
}