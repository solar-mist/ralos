#include <drivers/io/terminal.hpp>
#include <misc/font.h>
#include <libk/memory.hpp>

int IToA(int value, char *sp, int radix)
{
    char tmp[16];
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'A' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

namespace Graphics
{
    extern limine_framebuffer* Framebuffer;
}

namespace Terminal
{
    static unsigned int col = 0;
    static unsigned int row = 0;
    
    static unsigned int maxCol;
    static unsigned int maxRow;
    

    void Init()
    {
        maxCol = Graphics::Framebuffer->width - 8;
        maxRow = Graphics::Framebuffer->height;
    }

    void Clear(Graphics::Color color)
    {
        Graphics::ClearScreen(color);
    }

    void PutChar(char c, int x, int y, Graphics::Color color)
    {
        unsigned char* font_data = (unsigned char*)font_vga_8x8.data;
        for(int i = c * 8, a = 0; i < c * 8 + 8; i++, a++)
        {
            unsigned char data = font_data[i];
            for(int b = 0; b < 8; b++)
            {
                if(data >> (8 - b) & 0x1)
                    PutPixel(b+x, a+y, color);
            }
        }
    }

    
    void PutChar(char c, Graphics::Color color)
    {
        switch(c)
        {
        case '\n':
            col = 0;
            row += 9;
            break;
        default:
            PutChar(c, col, row, color);
            col += 9; // Extra space for fonts that don't include it
            if(col >= maxCol)
            {
                col = 0;
                row += 9;
            }
            if(row >= maxRow)
            {

            }
        }
    }

    void PutString(char* data, unsigned int size, Graphics::Color color)
    {
        for(unsigned int i = 0; i < size; i++)
            PutChar(data[i], color);
    }

    void Print(char* data, Graphics::Color color)
    {
        PutString(data, strlen(data), color);
    }

    void Print(const char* data, Graphics::Color color)
    {
        Print(const_cast<char*>(data), color);
    }


    void PrintInt(unsigned int value, int radix, Graphics::Color color)
    {
        char buffer[16];
        int n = IToA(value, buffer, radix);
        PutString(buffer, n, color);
    }

    void Printf(const char* fmt, ...)
    {
        va_list arg;
        va_start(arg, fmt);
        int i = 0;
        Graphics::Color color = DEFAULT_COLOR;
        while(fmt[i])
        {
            if (fmt[i] == '%')
            {
                if (fmt[++i] == '%')
                    PutChar(fmt[i++], color);
                else if (fmt[i] == 'd' || fmt[i] == 'i')
                {
                    char buf[16];
                    char* str = buf;
                    int n = va_arg(arg, int);
                    int count = IToA(n, buf, 10);
                    while(count--)
                        PutChar(*str++, color);
                    i++;
                }
                else if (fmt[i] == 'c')
                {
                    int ch_int = va_arg(arg, int);
                    char ch = (char)ch_int;
                    PutChar(ch, color);
                    i++;
                }
                else if (fmt[i] == 's')
                {
                    char* str = va_arg(arg, char*);
                    Print(str, color);
                    i++;
                }
                else if(fmt[i] == 'x')
                {
                    char buf[16];
                    char* str = buf;
                    int n = va_arg(arg, int);
                    int count = IToA(n, buf, 16);
                    while(count--)
                        PutChar(*str++, color);
                    i++;
                }
                else if(fmt[i] == '#')
                {
                    color = va_arg(arg, int);
                    i++;
                }
            }
            else
                PutChar(fmt[i++], color);
        }
        va_end(arg);
    }
}