#ifndef CPU_INTERRUPT_IDT_HPP
#define CPU_INTERRUPT_IDT_HPP

struct [[gnu::packed]] IDTEntry
{
    IDTEntry(unsigned long long isr, unsigned char flags, unsigned char ist);
    IDTEntry();

    unsigned short OffsetLow;
    unsigned short KernelCS;
    unsigned char  IST;
    unsigned char  Attributes;
    unsigned short OffsetMid;
    unsigned int   OffsetHigh;
    unsigned int   Reserved;
};

struct [[gnu::packed]] IDTPointer
{
    unsigned short Limit;
    void* Base;
};

void InitIDT();

#endif