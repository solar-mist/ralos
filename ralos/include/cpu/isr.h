#ifndef RALOS_CPU_ISR_H
#define RALOS_CPU_ISR_H
#include <stdint.h>

typedef struct
{
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rbp, rdi, rsi, rdx, rcx, rbx, rax;

    uint32_t vector, err_code;

    uint64_t rip, cs, rflags, rsp, ss;
} registers_t;

#endif