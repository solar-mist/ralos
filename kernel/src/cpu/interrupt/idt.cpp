#include <cpu/interrupt/idt.hpp>
#include <stdint.h>

static IDTEntry idt[256];
static IDTPointer idtr;

static bool vectors[256];

extern "C" uint64_t ExceptionStubTable[];

IDTEntry::IDTEntry(uint64_t isr, uint8_t flags, uint8_t ist)
{
    OffsetLow = isr & 0xFFFF;
    KernelCS = 0x8;
    IST = ist;
    Attributes = flags;
    OffsetMid = (isr >> 16) & 0xFFFF;
    OffsetHigh = (isr >> 32) & 0xFFFFFFFF;
    Reserved = 0;
}
IDTEntry::IDTEntry()
    :OffsetLow(0), KernelCS(0), IST(0), Attributes(0), OffsetMid(0), OffsetHigh(0), Reserved(0)
{}

constexpr uint8_t DEFUALT_FLAGS = 0x8E;
constexpr uint8_t TRAP_GATE     = 0x01;

void InitIDT()
{
    idtr.Limit = sizeof(idt) - 1;
    idtr.Base = &idt[0];

    for(uint8_t vector = 0; vector < 32; vector++)
    {
        idt[vector] = IDTEntry(ExceptionStubTable[vector], DEFUALT_FLAGS | TRAP_GATE, 0);
        vectors[vector] = true;
    }

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}