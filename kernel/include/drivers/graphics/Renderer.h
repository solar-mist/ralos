#ifndef VIPER_DRIVERS_GRAPHICS_RENDERER_H
#define VIPER_DRIVERS_GRAPHICS_RENDERER_H 1

#include <stdint.h>

namespace haf
{
    class Framework;
}

namespace drivers
{
    namespace graphics
    {
        class Renderer
        {
        public:
            Renderer(haf::Framework& hardwareFramework);

            uint32_t getWidth()  const;
            uint32_t getHeight() const;

            void putPixel(uint32_t x, uint32_t y, uint32_t color);
            void drawBitmap(void* bitmap, uint32_t sizeX, uint32_t sizeY, uint32_t x, uint32_t y, uint32_t fg, uint32_t bg);

        private:
            haf::Framework& mHardwareFramework;

            uint32_t mWidth{ 0 };
            uint32_t mHeight{ 0 };
        };
    }
}

#endif