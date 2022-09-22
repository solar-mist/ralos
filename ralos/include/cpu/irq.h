#ifndef RALOS_CPU_IRQ_H
#define RALOS_CPU_IRQ_H
#include <cpu/isr.h>

void install_irq_handler(int, void(*)(registers_t*));
void irq_handler(registers_t*);

#endif