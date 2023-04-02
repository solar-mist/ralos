#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>

extern "C" void _start()
{
    Graphics::Init();
    Terminal::Init();
    Terminal::Print("Hello World!", 0x00FFFF);
    for(;;);
}