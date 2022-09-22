#ifndef RALOS_DRIVERS_PORTS_H
#define RALOS_DRIVERS_PORTS_H
#include <stdint.h>

void outb(uint16_t, uint8_t);
uint8_t inb(uint16_t);

void outw(uint16_t, uint16_t);
uint16_t inw(uint16_t);

#endif