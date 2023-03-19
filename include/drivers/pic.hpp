#ifndef DRIVERS_PIC_HPP
#define DRIVERS_PIC_HPP
#include <stdint.h>

namespace PIC
{
    void Init();

    void SendEOI(uint8_t irq);

    void Mask(uint8_t irq);
    void Unmask(uint8_t irq);
}

#endif