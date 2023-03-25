#include <drivers/graphics.hpp>
#include <drivers/terminal.hpp>

extern "C" void KMain()
{
    Graphics::Init();
    Terminal::Init();
    Terminal::Clear(0xFF0000);
    for(int i = 0; i < 256; i++)
        Terminal::Print("Hello world\n");
    for(;;);
}  