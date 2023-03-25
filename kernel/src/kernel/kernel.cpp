#include <kernel/kernel.hpp>
#include <cpu/gdt/gdt.hpp>
#include <drivers/io/graphics.hpp>
#include <drivers/io/terminal.hpp>
#include <limine.h>

static volatile limine_bootloader_info_request LoaderInfoRequest = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

namespace Kernel
{
    [[noreturn]] void Panic(const char *fmt, ...)
    {
        Terminal::Printf(fmt);
        for(;;)
            asm volatile("cli; hlt");
    }

    extern "C" void KMain()
    {
        Graphics::Init();
        Terminal::Init();
        Terminal::Clear(0x008080);

        Terminal::Printf("%#Booting with %s version %#%s\n", 0x2A3F55, LoaderInfoRequest.response->name, 0xE35F5F, LoaderInfoRequest.response->version);

        InitGDT();
        Terminal::Printf("%#Initialized Global Descriptor Table", 0x2A3F55);
        Kernel::Panic("%#Test", 0xFF0000);
        for(;;);
    }
}