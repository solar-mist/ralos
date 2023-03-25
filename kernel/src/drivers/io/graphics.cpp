#include <drivers/io/graphics.hpp>

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
        return (m_B << 16) + (m_G << 8) + m_R;
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

    static volatile limine_framebuffer_request FramebufferRequest = {
        .id = LIMINE_FRAMEBUFFER_REQUEST,
        .revision = 0
    };

    limine_framebuffer* Framebuffer;

    void Init()
    {
        if(FramebufferRequest.response == nullptr || FramebufferRequest.response->framebuffer_count < 1)
            asm volatile("cli; hlt"); // TODO: Panic
        
        Framebuffer = FramebufferRequest.response->framebuffers[0];
    }

    void ClearScreen(Color color)
    {
        unsigned char* screen = (unsigned char*)Framebuffer->address;
        for(unsigned int i = 0; i < Framebuffer->width; i++)
        {
            for(unsigned int j = 0; j < Framebuffer->height * 2; j++)
                ((uint32_t*)screen)[j] = color;
            screen += Framebuffer->pitch;
        }
    }
    
    void PutPixel(unsigned int x, unsigned int y, Color color)
    {
        unsigned char* screen = (unsigned char*)Framebuffer->address;
        unsigned int where = x * 4 + y * Framebuffer->pitch;
        screen[where] = color.r();
        screen[where + 1] = color.g();
        screen[where + 2] = color.b();
    }

    void FillRect(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Color color)
    {
        unsigned char* screen = (unsigned char*)Framebuffer->address;
        for(unsigned int i = x1; i < x2; i++)
        {
            for(unsigned int j = y1; j < y2; j++)
                ((uint32_t*)screen)[j] = color;
            screen += Framebuffer->pitch;
        }
    }
}