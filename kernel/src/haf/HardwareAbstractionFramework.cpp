#include <haf/HardwareAbstractionFramework.h>

namespace haf
{
    LinearFramebuffer* Framework::getFramebuffer()
    {
        return &mFramebuffer;
    }
}