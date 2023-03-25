#ifndef DRIVERS_TERMINAL_HPP
#define DRIVERS_TERMINAL_HPP
#include <drivers/graphics.hpp>

namespace Terminal
{
    void Init();
    void Clear(Graphics::Color color = 0);

    void PutChar(char c, int x, int y, Graphics::Color color = 0xFFFFFF);

    void PutChar(char c, Graphics::Color color = 0xFFFFFF);
    void PutString(char* data, unsigned int size, Graphics::Color color = 0xFFFFFF);

    void Print(char* data, Graphics::Color color = 0xFFFFFF);
    void Print(const char* data, Graphics::Color color = 0xFFFFFF);

    void PrintInt(unsigned int value, int radix = 10, Graphics::Color color = 0xFFFFFF);
}

#endif