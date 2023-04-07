#include <drivers/terminal.hpp>
#include <kernel/modules.hpp>

unsigned int StrLen(char* str)
{   
    unsigned int i = 0;
    while(str[i])
        i++;
    return i;
}

int IToA(int64_t value, char *sp, int radix)
{
    char tmp[32];
    char *tp = tmp;
    int64_t i;
    int64_t v;

    int64_t sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (int64_t)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'A' - 10;
    }

    int64_t len = tp - tmp;

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
    extern struct FB framebuffer;
}

namespace Terminal
{
    static unsigned int col = 0;
    static unsigned int row = 0;
    
    static unsigned int maxCol;
    static unsigned int maxRow;

    void Init()
    {
        maxCol = Graphics::framebuffer.horiz;
        maxRow = Graphics::framebuffer.vert;
    }

    void PutChar(char c, int x, int y, Graphics::Color color)
    {
        unsigned char* font_data = (unsigned char*)GetModule(0)->address;
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
            row += 8;
            break;
        default:
            PutChar(c, col, row, color);
            col += 8;
            if(col >= maxCol)
            {
                col = 0;
                row += 8;
            }
            break;
        }
    }

    void PutString(char* data, unsigned int size, Graphics::Color color)
    {
        for(unsigned int i = 0; i < size; i++)
            PutChar(data[i], color);
    }

    void Print(char* data, Graphics::Color color)
    {
        PutString(data, StrLen(data), color);
    }

    void Print(const char* data, Graphics::Color color)
    {
        Print(const_cast<char*>(data), color);
    }


    void VPrintf(Graphics::Color color, const char* format, va_list arg)
    {
        int i = 0;
        while(format[i])
        {
            if (format[i] == '%')
            {
                if (format[++i] == '%')
                    PutChar(format[i++], color);
                else if (format[i] == 'd' || format[i] == 'i')
                {
                    char buf[32];
                    int64_t n = va_arg(arg, int64_t);
                    int count = IToA(n, buf, 10);
                    PutString(buf, count, color);
                    i++;
                }
                else if(format[i] == 'x')
                {
                    char buf[32] = "0x";
                    int64_t n = va_arg(arg, int64_t);
                    int count = IToA(n, buf + 2, 16);
                    PutString(buf, count + 2, color);
                    i++;
                }
                else if (format[i] == 'c')
                {
                    int ch_int = va_arg(arg, int);
                    char ch = (char)ch_int;
                    PutChar(ch, color);
                    i++;
                }
                else if (format[i] == 's')
                {
                    char* str = va_arg(arg, char*);
                    Print(str, color);
                    i++;
                }
                else if(format[i] == '#')
                {
                    unsigned int newColor = va_arg(arg, unsigned int);
                    color = newColor;
                    i++;
                }
            }
            else
                PutChar(format[i++], color);
        }
    }

    void Printf(Graphics::Color color, const char* format, ...)
    {
        va_list arg;
        va_start(arg, format);
        VPrintf(color, format, arg);
        va_end(arg);
    }
}