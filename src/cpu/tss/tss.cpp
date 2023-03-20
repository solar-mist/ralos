#include <cpu/tss.hpp>
#include <cpu/gdt/gdt.hpp>
#include <mem/pmm.hpp>

TSSDescriptor tss;

extern GDTDescriptor gdt[];
extern "C" void install_tss();

void InitTSS()
{
    tss.rsp0 = (uint64_t)PMM::GetPage();

    gdt[5] = GDTDescriptor(((unsigned long long)&tss) & 0xFFFFFFFF, sizeof(tss), 0x89, 0);
    gdt[6] = GDTDescriptor(((((unsigned long long)&tss) >> 32) & 0xFFFFFFFF), 0, 0, 0);

    install_tss();
}