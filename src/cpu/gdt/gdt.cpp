#include <cpu/gdt/gdt.hpp>

extern "C" void install_gdt(GDTPointer* gdtr);

GDTDescriptor::GDTDescriptor(unsigned int base, unsigned short limit, unsigned char access, unsigned char flags)
{
    BaseLow = base & 0xFFFF;
    BaseMid = (base >> 16) & 0xFF;
    BaseHigh = (base >> 24) & 0xFF;

    Limit = limit;

    Access = access;
    Flags = flags;
}
GDTDescriptor::GDTDescriptor(){}

#define DEFAULT_ACCESS (0x80 | 0x2 | 0x10)
#define DEFAULT_FLAGS (0x20 | 0x80)

GDTDescriptor gdt[5];
static GDTPointer gdtr;

void InitGDT()
{
    gdt[0] = GDTDescriptor(0, 0, 0, 0);
    gdt[1] = GDTDescriptor(0, 0, DEFAULT_ACCESS | 0x8, DEFAULT_FLAGS);
    gdt[2] = GDTDescriptor(0, 0, DEFAULT_ACCESS, DEFAULT_FLAGS);
    gdt[3] = GDTDescriptor(0, 0, 0, 0);
    gdt[4] = GDTDescriptor(0, 0, 0, 0);

    gdtr.Limit = sizeof(gdt) - 1;
    gdtr.Base = gdt;

    install_gdt(&gdtr);
}