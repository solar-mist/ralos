#ifndef CPU_GDT_HPP
#define CPU_GDT_HPP
#include <stdint.h>

struct [[gnu::packed]] GDTDescriptor
{
    uint16_t Limit;
    uint16_t BaseLow;
    uint8_t  BaseMid;
    uint8_t  Access;
    uint8_t  Flags;
    uint8_t  BaseHigh;

    GDTDescriptor();
    GDTDescriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags);
};

struct [[gnu::packed]] GDTPointer
{
    uint16_t Limit;
    void* Base;
};

extern "C" void InitGDT();

#endif