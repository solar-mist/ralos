#ifndef DRIVERS_IO_TERMINAL_HPP
#define DRIVERS_IO_TERMINAL_HPP
#include <drivers/io/graphics.hpp>
#include <stdarg.h>

namespace Terminal
{
    constexpr uint32_t DEFAULT_COLOR = 0xFFFFFF;

    void Init();
    void Clear(Graphics::Color color = 0);

    void PutChar(char c, int x, int y, Graphics::Color color = DEFAULT_COLOR);

    void PutChar(char c, Graphics::Color color = DEFAULT_COLOR);
    void PutString(char* data, unsigned int size, Graphics::Color color = DEFAULT_COLOR);

    void Print(char* data, Graphics::Color color = DEFAULT_COLOR);
    void Print(const char* data, Graphics::Color color = DEFAULT_COLOR);

    void PrintInt(unsigned int value, int radix = 10, Graphics::Color color = DEFAULT_COLOR);

    void Printf(const char* fmt, ...);
}

#endif