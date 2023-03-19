#ifndef CPU_GDT_HPP
#define CPU_GDT_HPP

struct [[gnu::packed]] GDTDescriptor
{
    unsigned short Limit;
    unsigned short BaseLow;
    unsigned char  BaseMid;
    unsigned char  Access;
    unsigned char  Flags;
    unsigned char  BaseHigh;

    GDTDescriptor();
    GDTDescriptor(unsigned int base, unsigned short limit, unsigned char access, unsigned char flags);
};

struct [[gnu::packed]] GDTPointer
{
    unsigned short Limit;
    void* Base;
};

extern "C" void InitGDT();

#endif