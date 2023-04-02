#ifndef DRIVERS_GRAPHICS_HPP
#define DRIVERS_GRAPHICS_HPP
#include <stdint.h>

namespace Graphics
{
    struct FB
    {
        void* base;
        uint64_t pitch;
        uint64_t horiz;
        uint64_t vert;
    };

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

    void Init();

    void PutPixel(unsigned int x, unsigned int y, Color color);
}

#endif