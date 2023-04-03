#include <cpu/gdt/gdt.hpp>

extern "C" void install_gdt(GDTPointer* gdtr);

GDTDescriptor::GDTDescriptor(uint32_t base, uint16_t limit, uint8_t access, uint8_t flags)
{
    BaseLow = base & 0xFFFF;
    BaseMid = (base >> 16) & 0xFF;
    BaseHigh = (base >> 24) & 0xFF;

    Limit = limit;

    Access = access;
    Flags = flags;
}
GDTDescriptor::GDTDescriptor(){}

constexpr uint8_t CODE_ACCESS = 0x9A;
constexpr uint8_t DATA_ACCESS = 0x92;
constexpr uint8_t CODE_FLAGS  = 0xA0;
constexpr uint8_t DATA_FLAGS  = 0xC0;
constexpr uint8_t USER_MODE   = 0x60;
constexpr uint8_t TSS         = 0x0;

GDTDescriptor gdt[7];
static GDTPointer gdtr;

void InitGDT()
{
    gdt[0] = GDTDescriptor(0, 0, 0,    0);
    gdt[1] = GDTDescriptor(0, 0, CODE_ACCESS, CODE_FLAGS);
    gdt[2] = GDTDescriptor(0, 0, DATA_ACCESS, DATA_FLAGS);
    gdt[3] = GDTDescriptor(0, 0, CODE_ACCESS | USER_MODE, CODE_FLAGS);
    gdt[4] = GDTDescriptor(0, 0, DATA_ACCESS | USER_MODE, DATA_FLAGS);
    gdt[5] = GDTDescriptor(TSS, TSS, TSS, TSS);
    gdt[6] = GDTDescriptor(TSS, TSS, TSS, TSS);

    gdtr.Limit = sizeof(gdt) - 1;
    gdtr.Base = gdt;

    install_gdt(&gdtr);
}