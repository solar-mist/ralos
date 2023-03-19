#include <mem/kheap.hpp>
#include <mem/pmm.hpp>

struct MemoryControlBlock
{
    MemoryControlBlock(size_t size, size_t npages)
        :Size(size), NPages(npages) {}
    size_t Size;
    size_t NPages;  
};

void* KMalloc(size_t size)
{
    size += sizeof(MemoryControlBlock);
    MemoryControlBlock* mcb = (MemoryControlBlock*)PMM::GetPages(NPAGES(size));
    mcb->Size = size;
    mcb->NPages = NPAGES(size);
    return (char*)mcb + sizeof(MemoryControlBlock);
}

void KFree(void* address)
{
    MemoryControlBlock* mcb = (MemoryControlBlock*)((char*)address - sizeof(MemoryControlBlock));
    PMM::FreePages(mcb, mcb->NPages);
}