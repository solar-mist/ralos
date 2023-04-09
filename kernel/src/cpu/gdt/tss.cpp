#include <cpu/gdt/tss.hpp>
#include <cpu/gdt/gdt.hpp>
#include <mm/pmm.hpp>
#include <libk/mem.hpp>
#include <stddef.h>

TSSDescriptor tss;

extern GDTDescriptor gdt[];
extern "C" void install_tss();

void InitTSS()
{
    tss.rsp0 = PhysToVirt((uint64_t)PMM::GetPage()) + 0xFFF;

    uint32_t tssLow  = ((uint64_t)&tss) & 0x00000000000000000000000000FFFFFFFF;
    uint32_t tssHigh = (((uint64_t)&tss) >> 32) & 0xFFFFFFFF;

    gdt[5] = GDTDescriptor(tssLow,  sizeof(tss), 0x89, 0);
    memcpy(&gdt[6], &tssHigh, sizeof(tssHigh));

    install_tss();
}