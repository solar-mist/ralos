#ifndef MEM_PMM_HPP
#define MEM_PMM_HPP
#include <stdint.h>
#include <viperBoot.h>

#define PAGE_SIZE 0x1000
#define NPAGES(num) (((num) + PAGE_SIZE - 1) / PAGE_SIZE)

namespace PMM
{
    void Init(VBMemoryMapEntry* memoryMap);

    bool Alloc(void* address, uint32_t npages);

    void* GetPage();
    void FreePage(void* address);

    void* GetPages(uint32_t count);
    void FreePages(void* address, uint32_t count);

    void DumpStats();
}

#endif