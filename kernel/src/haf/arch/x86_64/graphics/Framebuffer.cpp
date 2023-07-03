#include <haf/graphics/Framebuffer.h>

#include <viper.h>

namespace haf
{
    ViperFramebufferRequest framebufferRequest = {
        .id = VIPER_FRAMEBUFFER,
        .response = nullptr,
    };

    LinearFramebuffer GetFramebuffer()
    {
        LinearFramebuffer ret;

        if (framebufferRequest.response == nullptr)
        {
            return ret;
        }

        ret.mBase = framebufferRequest.response->base;
        ret.mWidth = framebufferRequest.response->width;
        ret.mHeight = framebufferRequest.response->height;
        ret.mPitch = framebufferRequest.response->pitch;

        return ret;
    }
}