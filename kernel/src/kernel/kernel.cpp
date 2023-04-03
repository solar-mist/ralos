#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <cpu/gdt/gdt.hpp>

extern "C" void _start()
{
    InitGDT();
    Graphics::Init();
    Terminal::Init();
    Terminal::Printf("%#Hello world! Welcome to %#%s", 0xFF00FF, 0xFFFF00, "viperOS");
    for(;;);
}