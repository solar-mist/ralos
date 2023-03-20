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

extern "C" void KMain(VBInfo* BootInfo)
{
    InitGDT();
    InitTSS();
    InitIDT();
    Graphics::Init(BootInfo);
    Terminal::Init(BootInfo->Framebuffer.Horiz, BootInfo->Framebuffer.Vert);
    PMM::Init(BootInfo->MemoryMap);
    VMM::Init();
    //VMM::MapPage(0, 0, 3);
    //VMM::MapPage(0x1000, 0x1000, 3);
    Terminal::Print("Hello World!\n", 0xFFFF00);
    VMM::Dump(0);
    for(;;);
}