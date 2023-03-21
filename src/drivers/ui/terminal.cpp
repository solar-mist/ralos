#include <drivers/ui/terminal.hpp>
#include <font.h>

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

namespace Terminal
{
    static unsigned int col = 0;
    static unsigned int row = 0;
    
    static unsigned int maxCol;
    static unsigned int maxRow;

    Status Init(unsigned int horiz, unsigned int vert)
    {
        maxCol = horiz;
        maxRow = vert;
        return Status::Success;
    }

    Status PutChar(char c, int x, int y, Graphics::Color color)
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
        return Status::Success;
    }

    
    Status PutChar(char c, Graphics::Color color)
    {
        switch(c)
        {
        case '\n':
            col = 0;
            row += 9;
            return Status::Success;
        default:
            Status result = PutChar(c, col, row, color);
            col += 9; // Extra space for fonts that don't include it
            if(col >= maxCol)
            {
                col = 0;
                row += 9;
            }
            return result;
        }
    }

    Status PutString(char* data, unsigned int size, Graphics::Color color)
    {
        for(unsigned int i = 0; i < size; i++)
        {
            Status status = PutChar(data[i], color);
            if(status != Status::Success)
                return status;
        }
        return Status::Success;
    }

    Status Print(char* data, Graphics::Color color)
    {
        return PutString(data, StrLen(data), color);
    }

    Status Print(const char* data, Graphics::Color color)
    {
        return Print(const_cast<char*>(data), color);
    }


    Status PrintInt(unsigned int value, int radix, Graphics::Color color)
    {
        char buffer[16];
        int n = IToA(value, buffer, radix);
        return PutString(buffer, n, color);
    }
}