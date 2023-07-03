#ifndef VIPER_HAF_GRAPHICS_FRAMEBUFFER_H
#define VIPER_HAF_GRAPHICS_FRAMEBUFFER_H 1

#include <stdint.h>

namespace haf
{
    struct LinearFramebuffer
    {
        void* mBase{ nullptr };

        uint32_t mWidth{ 0 };
        uint32_t mHeight{ 0 };

        uint32_t mPitch{ 0 };
    };

    LinearFramebuffer GetFramebuffer();
}

#endif