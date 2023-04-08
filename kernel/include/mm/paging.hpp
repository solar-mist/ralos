#ifndef MEM_PAGING_HPP
#define MEM_PAGING_HPP
#include <stdint.h>

namespace Paging
{
    void Init();

    void MapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
    void MapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);
}

#endif