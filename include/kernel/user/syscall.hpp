#ifndef KERNEL_SYSCALL_HPP
#define KERNEL_SYSCALL_HPP
#include <cpu/interrupt/isr.hpp>

extern "C" void SystemCallHandler(ISRXFrame* frame);

#endif