#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>
#include <kernel/modules.hpp>

extern "C" void _start()
{
    Graphics::Init();
    Terminal::Init();
    Terminal::Printf("%#Hello world! Welcome to %#%s", 0xFF00FF, 0xFFFF00, "viperOS");
    for(;;);
}