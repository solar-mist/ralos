#include <haf/HardwareAbstractionFramework.h>
#ifndef VIPER_KERNEL_CORE_FRAMEWORK_H
#define VIPER_KERNEL_CORE_FRAMEWORK_H 1

#include <drivers/graphics/Renderer.h>
#include <drivers/terminal/Terminal.h>

#include <haf/HardwareAbstractionFramework.h>

namespace kernel
{
    class Framework
    {
    public:
        Framework();

        haf::Framework& getHardwareFramework();

        drivers::graphics::Renderer* getRenderer();
        drivers::terminal::Terminal* getTerminal();
    
    private:
        haf::Framework mHardwareFramework;

        drivers::graphics::Renderer mRenderer;
        drivers::terminal::Terminal mTerminal;
    };
}

#endif