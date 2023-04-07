
#ifndef MEM_PMM_H
#define MEM_PMM_H
#include <stdint.h>

constexpr int PAGE_SIZE = 0x1000;
[[gnu::always_inline]] constexpr inline uint32_t NPAGES(uint64_t n)
{
    return (n + PAGE_SIZE - 1) / PAGE_SIZE;
}

namespace PMM
{
    void Init();

    void* GetPage();
    void  FreePage(void* address);

    void* GetPages(uint32_t npages);
    void  FreePages(void* address);

    void DumpStats();
}

#endif