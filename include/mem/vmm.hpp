#ifndef MEM_VMM_HPP
#define MEM_VMM_HPP
#include <stdint.h>

namespace VMM
{
    void Init();

    void MapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
    void MapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);

    uint64_t GetPhysAddr(uint64_t VirtAddr);
}

#endif