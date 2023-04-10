#include <mm/kheap.hpp>
#include <mm/pmm.hpp>
#include <libk/new.hpp>
#include <drivers/terminal.hpp>

struct Header
{
    size_t size;
    Header* next;
};

Header* freeList;

void InitKHeap()
{
    void* fl = (void*)PhysToVirt((uint64_t)PMM::GetPages(32));
    freeList = new((char*)fl) Header;
    freeList->size = 32 * PAGE_SIZE;
    freeList->next = nullptr;
}

void* malloc(size_t count)
{
    if(!count)
        return nullptr;
    
    count += sizeof(Header);
    Header* current = freeList;
    Header* previous = nullptr;
    while(current)
    {
        if(current->size >= count)
        {
            if(current->size == count)
            {
                if(previous)
                    previous->next = current->next;
                else
                    freeList = current->next;
                
                return current + 1;
            }
            size_t newSize = current->size - count;
            Header* newHeader = new((char*)current + count + sizeof(Header)) Header;
            newHeader->size = newSize;
            if(previous)
                previous->next = current->next;
            else
                freeList = current->next;
            
            newHeader->next = freeList;
            freeList = newHeader;

            current->size = count;
            return current + 1;
        }
        previous = current;
        current = current->next;
    }
    return nullptr;
}

void free(void* addr)
{
    if(addr == nullptr)
        return;
    
    Header* header = (Header*)addr - 1;
    header->next = freeList;
    freeList = header;
}