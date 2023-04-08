#ifndef CPU_INTERRUPT_EXCEPTION_HPP
#define CPU_INTERRUPT_EXCEPTION_HPP
#include <cpu/interrupt/isr.hpp>

void RegisterExceptionHandler(uint8_t vector, InterruptHandler handler);

#endif