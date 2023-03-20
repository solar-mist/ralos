#ifndef DRIVERS_TERMINAL_HPP
#define DRIVERS_TERMINAL_HPP
#include <status.h>
#include <drivers/graphics.hpp>

namespace Terminal
{
    Status Init(unsigned int horiz, unsigned int vert);

    Status PutChar(char c, int x, int y, Graphics::Color color);

    Status PutChar(char c, Graphics::Color color);
    Status PutString(char* data, unsigned int size, Graphics::Color color);

    Status Print(char* data, Graphics::Color color = 0xFFFFFF);
    Status Print(const char* data, Graphics::Color color = 0xFFFFFF);

    Status PrintInt(unsigned long value, int radix = 10, Graphics::Color color = 0xFFFFFF);
}

#endif