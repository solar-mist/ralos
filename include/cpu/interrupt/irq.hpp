#ifndef CPU_INTERRUPT_IRQ_HPP
#define CPU_INTERRUPT_IRQ_HPP
#include <cpu/interrupt/isr.hpp>

typedef void (*InterruptHandler)(ISRXFrame* frame);

void InstallIRQHandler(unsigned int irq, InterruptHandler handler);

#endif