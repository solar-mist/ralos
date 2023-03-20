#include <cpu/interrupt/idt.hpp>
#include <drivers/pic.hpp>
#include <stdint.h>

static IDTEntry idt[256];
static IDTPointer idtr;

static bool vectors[256];

extern "C" uint64_t ExceptionStubTable[];
extern "C" uint64_t IRQStubTable[];
extern "C" uint64_t SyscallHandler;

constexpr int IRQ_OFFSET = 32;

IDTEntry::IDTEntry(unsigned long long isr, unsigned char flags, unsigned char ist)
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

void InitIDT()
{
    idtr.Limit = sizeof(idt) - 1;
    idtr.Base = &idt[0];

    for(unsigned char vector = 0; vector < 32; vector++)
    {
        idt[vector] = IDTEntry(ExceptionStubTable[vector], 0x8F, 0);
        vectors[vector] = true;
    }
    for(uint8_t vector = IRQ_OFFSET; vector < IRQ_OFFSET + 15; vector++)
    {
        idt[vector] = IDTEntry(IRQStubTable[vector - IRQ_OFFSET], 0x8E, 0);
        vectors[vector] = true;
    }

    idt[0x69] = IDTEntry((uint64_t)&SyscallHandler, 0xEE, 0);
    vectors[0x69] = true;

    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");

    PIC::Init();
}