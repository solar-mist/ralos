#ifndef CPU_INTERRUPT_IDT_HPP
#define CPU_INTERRUPT_IDT_HPP
#include <stdint.h>

struct [[gnu::packed]] IDTEntry
{
    IDTEntry(uint64_t isr, uint8_t flags, uint8_t ist);
    IDTEntry();

    uint16_t OffsetLow;
    uint16_t KernelCS;
    uint8_t  IST;
    uint8_t  Attributes;
    uint16_t OffsetMid;
    uint32_t OffsetHigh;
    uint32_t Reserved;
};

struct [[gnu::packed]] IDTPointer
{
    uint16_t Limit;
    void* Base;
};

void InitIDT();

#endif