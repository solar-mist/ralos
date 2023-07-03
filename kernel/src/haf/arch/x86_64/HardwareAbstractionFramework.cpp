#include <haf/HardwareAbstractionFramework.h>

namespace haf
{
    Framework::Framework()
        :mFramebuffer(GetFramebuffer())
    {
    }
}