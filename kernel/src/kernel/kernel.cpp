#include <cpu/gdt/gdt.hpp>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <limine.h>

static volatile limine_bootloader_info_request LoaderInfoRequest = {
    .id = LIMINE_BOOTLOADER_INFO_REQUEST,
    .revision = 0
};

extern "C" void KMain()
{
    InitGDT();
    Graphics::Init();
    Terminal::Init();
    Terminal::Clear(0x018281);
    Terminal::Printf("Booting with %s version %s", LoaderInfoRequest.response->name, LoaderInfoRequest.response->version);
    for(;;);
}