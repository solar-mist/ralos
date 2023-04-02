#ifndef DRIVERS_TERMINAL_HPP
#define DRIVERS_TERMINAL_HPP
#include <drivers/graphics.hpp>

namespace Terminal
{
    void Init();

    void PutChar(char c, int x, int y, Graphics::Color color);

    void PutChar(char c, Graphics::Color color);
    void PutString(char* data, unsigned int size, Graphics::Color color);

    void Print(char* data, Graphics::Color color = 0xFFFFFF);
    void Print(const char* data, Graphics::Color color = 0xFFFFFF);
}

#endif