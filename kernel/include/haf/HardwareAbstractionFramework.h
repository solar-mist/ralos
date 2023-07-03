#ifndef VIPER_HAF_HARDWARE_ABSTRACTION_FRAMEWORK_H
#define VIPER_HAF_HARDWARE_ABSTRACTION_FRAMEWORK_H 1

#include <haf/fs/Module.h>

#include <haf/graphics/Renderer.h>

namespace haf
{
    class Framework
    {
    public:
        Framework();

        Renderer* getRenderer();
    
    private:
        Renderer mRenderer;
    };
}

#endif