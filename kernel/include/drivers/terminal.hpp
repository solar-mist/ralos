#ifndef DRIVERS_TERMINAL_HPP
#define DRIVERS_TERMINAL_HPP
#include <drivers/graphics.hpp>
#include <stdarg.h>

namespace Terminal
{
    void Init();

    void PutChar(char c, int x, int y, Graphics::Color color);

    void PutChar(char c, Graphics::Color color);
    void PutString(char* data, unsigned int size, Graphics::Color color);

    void Print(char* data, Graphics::Color color = 0xFFFFFF);
    void Print(const char* data, Graphics::Color color = 0xFFFFFF);

    void VPrintf(Graphics::Color color, const char* format, va_list arg);
    void Printf(Graphics::Color color, const char* format, ...);
}

#endif