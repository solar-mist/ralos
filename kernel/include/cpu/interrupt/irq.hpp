#ifndef CPU_INTERRUPT_IRQ_HPP
#define CPU_INTERRUPT_IRQ_HPP
#include <cpu/interrupt/isr.hpp>

void RegisterIRQHandler(uint8_t irq, InterruptHandler handler);

#endif