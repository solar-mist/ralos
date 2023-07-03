#include <kernel/CoreFramework.h>

#include <drivers/terminal/Terminal.h>

#include <haf/HardwareAbstractionFramework.h>

namespace kernel
{
    Framework::Framework()
        : mRenderer(mHardwareFramework), mTerminal(*this ,drivers::terminal::Font::FromSFF(haf::Module::Get(0)))
    {
    }

    haf::Framework& Framework::getHardwareFramework()
    {
        return mHardwareFramework;
    }

    drivers::graphics::Renderer* Framework::getRenderer()
    {
        return &mRenderer;
    }

    drivers::terminal::Terminal* Framework::getTerminal()
    {
        return &mTerminal;
    }
}