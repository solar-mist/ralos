#ifndef MEM_PAGING_HPP
#define MEM_PAGING_HPP
#include <stdint.h>

namespace Paging
{
    struct Node
    {
        uint32_t npages;
        void* base;
        Node* next;
    };

    struct AddressSpace
    {
        uint64_t* pml4;
        Node* freeListBegin;
    };

    void Init();

    void MapPage(AddressSpace* addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags);
    void MapPages(AddressSpace* addrspace, uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages);

    void UnmapPages(AddressSpace* addrspace, uint64_t VirtAddr, uint64_t Npages);

    uint64_t GetEntry(AddressSpace* addrspace, uint64_t VirtAddr);

    AddressSpace CreateAddressSpace();

    void SwitchAddrSpace(AddressSpace* addrspace);
}

#endif