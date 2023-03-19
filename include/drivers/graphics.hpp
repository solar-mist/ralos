#ifndef DRIVERS_GRAPHICS_HPP
#define DRIVERS_GRAPHICS_HPP
#include <viperBoot.h>
#include <status.h>

namespace Graphics
{
    class Color
    {
    public:
        Color(unsigned char r, unsigned char g, unsigned char b);
        Color(unsigned int hex);

        operator unsigned int();

        unsigned char r() const;
        unsigned char g() const;
        unsigned char b() const;
    private:
        unsigned char m_R, m_G, m_B;
    };

    Status Init(VBInfo* BootInfo);

    Status PutPixel(unsigned int x, unsigned int y, Color color);
    Status FillRect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Color color);
}

#endif