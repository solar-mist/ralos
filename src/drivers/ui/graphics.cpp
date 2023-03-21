#include <drivers/ui/graphics.hpp>

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

    static VBFramebuffer Framebuffer;
    Status Init(VBInfo* BootInfo)
    {
        Framebuffer = BootInfo->Framebuffer;
        return Status::Success;
    }
    
    Status PutPixel(unsigned int x, unsigned int y, Color color)
    {
        if(Framebuffer.Base + Framebuffer.Pitch * x + 4 * y > Framebuffer.Base + Framebuffer.Size)
            return Status::Failed;

        unsigned char* screen = (unsigned char*)Framebuffer.Base;
        unsigned int where = x * 4 + y * Framebuffer.Pitch;
        screen[where] = color.r();
        screen[where + 1] = color.g();
        screen[where + 2] = color.b();

        return Status::Success;
    }

    Status FillRect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Color color)
    {
        unsigned char* screen = (unsigned char*)Framebuffer.Base;
        for(unsigned int i = x1; i < x2; i++)
        {
            for(unsigned int j = y1; j < y2; j++)
            {
                screen[j*4] = color.r();
                screen[j*4+1] = color.g();
                screen[j*4+2] = color.b();
            }
            screen += Framebuffer.Pitch;
        }
        return Status::Success;
    }
}