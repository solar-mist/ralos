#include <drivers/graphics.hpp>
#include <viper.h>

namespace Graphics
{
    Color::Color(unsigned char r, unsigned char g, unsigned char b)
        :m_R(r), m_G(g), m_B(b)
    {
    }

    Color::Color(unsigned int hex)
    {
        m_B = (hex >> 16) & 0xFF;
        m_G = (hex >> 8)  & 0xFF;
        m_R = hex & 0xFF;
    }

    Color::operator unsigned int()
    {
        return (m_R << 16) + (m_G << 8) + m_B;
    }

    unsigned char Color::r() const
    {
        return m_R;
    }
    unsigned char Color::g() const
    {
        return m_G;
    }
    unsigned char Color::b() const
    {
        return m_B;
    }

    static volatile ViperFramebufferRequest fbReq = {
        .id = VIPER_FRAMEBUFFER
    };

    FB framebuffer;

    void Init()
    {
        framebuffer = {
            fbReq.response->base, fbReq.response->pitch,
            fbReq.response->width, fbReq.response->height,
            fbReq.response->size
        };
    }
    
    void PutPixel(unsigned int x, unsigned int y, Color color)
    {
        unsigned char* screen = (unsigned char*)framebuffer.base;
        unsigned int where = x * 4 + y * framebuffer.pitch;
        screen[where] = color.r();
        screen[where + 1] = color.g();
        screen[where + 2] = color.b();
    }
}