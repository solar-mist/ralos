#ifndef MEM_PMM_H
#define MEM_PMM_H
#include <stdint.h>

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