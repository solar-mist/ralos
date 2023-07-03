#include <kernel/CoreFramework.h>

#include <drivers/terminal/Terminal.h>

#include <haf/HardwareAbstractionFramework.h>

namespace kernel
{
    Framework::Framework()
        : mTerminal(mHardwareFramework ,drivers::terminal::Font::FromSFF(haf::Module::Get(0)))
    {
    }

    haf::Framework& Framework::getHardwareFramework()
    {
        return mHardwareFramework;
    }

    drivers::terminal::Terminal& Framework::getTerminal()
    {
        return mTerminal;
    }
}