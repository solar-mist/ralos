#include <drivers/terminal/Color.h>
#include <haf/HardwareAbstractionFramework.h>
#include <kernel/CoreFramework.h>

namespace kernel
{
    extern "C" void kmain()
    {
        Framework coreFramework;

        coreFramework.getTerminal()->print("Hello");

        asm("jmp .");
    }
}