#include <viperBoot.h>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <cpu/gdt/gdt.hpp>
#include <cpu/tss.hpp>
#include <cpu/interrupt/idt.hpp>
#include <mem/pmm.hpp>
#include <mem/kheap.hpp>
#include <drivers/pit.hpp>
#include <drivers/ata.hpp>
#include <drivers/ps2_keyboard.hpp>

extern "C" void KMain(VBInfo* BootInfo)
{
    InitGDT();
    InitTSS();
    InitIDT();
    Graphics::Init(BootInfo);
    Terminal::Init(BootInfo->Framebuffer.Horiz, BootInfo->Framebuffer.Vert);
    PMM::Init(BootInfo->MemoryMap);
    Terminal::Print("Hello World!\n", 0xFFFF00);
    PIT::Init(11967);
    char* buffer = (char*)KMalloc(512);
    ATA::Read(2, 1, buffer);
    Terminal::Print(buffer);
    for(;;);
}