#include <cpu/gdt/gdt.hpp>
#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>

extern "C" void KMain()
{
    InitGDT();
    Graphics::Init();
    Terminal::Init();
    Terminal::Clear(0x018281);
    for(int i = 0; i < 256; i++)
        Terminal::Print("Hello world\n", 0x606060);
    for(;;);
}  