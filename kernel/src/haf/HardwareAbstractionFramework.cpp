#include <haf/HardwareAbstractionFramework.h>

namespace haf
{
    Renderer* Framework::getRenderer()
    {
        return &mRenderer;
    }
}