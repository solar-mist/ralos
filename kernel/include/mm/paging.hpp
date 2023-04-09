#ifndef MEM_PAGING_HPP
#define MEM_PAGING_HPP
#include <stdint.h>

namespace Paging
{
    struct AddressSpace
    {
        uint64_t* pml4;
    };

    void Init();

    void MapPage(AddressSpace addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
    void MapPages(AddressSpace addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);

    AddressSpace CreateAddressSpace();

    void SwitchAddrSpace(AddressSpace* addrspace);
}

#endif