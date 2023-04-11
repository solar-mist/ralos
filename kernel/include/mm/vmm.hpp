#ifndef MM_VMM_HPP
#define MM_VMM_HPP
#include <mm/paging.hpp>
#include <stdint.h>

namespace VMM
{
    void Init();

    void* GetPages(Paging::AddressSpace* addrspace, uint32_t npages, uint16_t flags);
    void FreePages(Paging::AddressSpace* addrspace, void* addr, uint32_t npages);

    bool MarkUsed(Paging::AddressSpace* addrspace, void* addr, uint32_t npages);
}

#endif