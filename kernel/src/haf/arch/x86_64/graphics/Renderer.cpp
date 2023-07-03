#include <haf/graphics/Renderer.h>

#include <viper.h>

ViperFramebufferRequest framebufferRequest = {
    VIPER_FRAMEBUFFER,
    nullptr
};

namespace haf
{
    Renderer::Renderer()
    {
        if (framebufferRequest.response)
        {
            mWidth = framebufferRequest.response->width;
            mHeight = framebufferRequest.response->height;
        }
        else
        {
            asm volatile("int3");
        }
    }

    uint32_t Renderer::getWidth() const
    {
        return mWidth;
    }

    uint32_t Renderer::getHeight() const
    {
        return mHeight;
    }
    
    void Renderer::putPixel(uint32_t x, uint32_t y, uint32_t color)
    {
        uint8_t* screen = (uint8_t*)framebufferRequest.response->base;
        uint32_t where = x * 4 + y * framebufferRequest.response->pitch;
        screen[where]     =  color         & 0xFF;
        screen[where + 1] = (color >> 8)   & 0xFF;
        screen[where + 2] = (color >> 16)  & 0xFF;
    }

    void Renderer::drawBitmap(void* bitmap, uint32_t sizeX, uint32_t sizeY, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg)
    {
        uint8_t* screen = (uint8_t*)framebufferRequest.response->base;
        uint32_t where = x * 4 + y * framebufferRequest.response->pitch;

        uint8_t* bitmapData = (uint8_t*)bitmap;

        for(uint32_t i = 0; i < sizeY; i++)
        {
            uint8_t data = bitmapData[i];
            
            for(uint32_t j = 0; j < sizeX; j++)
            {
                uint32_t color = (data >> (sizeX - j) & 0x1) ? fg : bg;

                screen[where]     =  color         & 0xFF;
                screen[where + 1] = (color >> 8)   & 0xFF;
                screen[where + 2] = (color >> 16)  & 0xFF;
                
                where += 4;
            }
            where += framebufferRequest.response->pitch;
            where -= sizeX * 4;
        }
    }
}