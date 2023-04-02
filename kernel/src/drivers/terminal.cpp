#include <drivers/terminal.hpp>
#include <kernel/modules.hpp>

unsigned int StrLen(char* str)
{   
    unsigned int i = 0;
    while(str[i])
        i++;
    return i;
}

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
        //asm volatile("mov %0, %%rax; hlt" : : "m"(GetModule(0)->size));
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
        default:
            PutChar(c, col, row, color);
            col += 8;
            if(col >= maxCol)
            {
                col = 0;
                row += 8;
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
        PutString(data, StrLen(data), color);
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
}