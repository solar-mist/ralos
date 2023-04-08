
#ifndef MEM_PMM_H
#define MEM_PMM_H
#include <stdint.h>

constexpr int PAGE_SIZE = 0x1000;
[[gnu::always_inline]] constexpr inline uint32_t NPAGES(uint64_t n)
{
    return (n + PAGE_SIZE - 1) / PAGE_SIZE;
}

[[gnu::always_inline]] constexpr inline uint64_t VirtToPhys(uint64_t addr)
{
    return addr - 0xFFFF800000000000;
}
[[gnu::always_inline]] constexpr inline uint64_t PhysToVirt(uint64_t addr)
{
    return addr + 0xFFFF800000000000;
}

namespace PMM
{
    void Init();

    void* GetPage();
    void  FreePage(void* address);

    void* GetPages(uint32_t npages);
    void  FreePages(void* address, uint32_t npages);

    void DumpStats();
}

#endif