#include <mem/pmm.h>
#include <drivers/io/terminal.hpp>
#include <libk/memory.hpp>
#include <limine.h>
#include <stddef.h>

constexpr uint32_t PAGE_SIZE = 0x1000;
[[gnu::always_inline]] constexpr inline uint32_t NPAGES(uint64_t n)
{
    return (n + PAGE_SIZE - 1) / PAGE_SIZE;
}

namespace PMM
{
    struct Header
    {
        Header()
            :next(nullptr), size(0) {}
        Header(uint64_t size)
            :size(size) {}
        Header* next;
        uint64_t size;
    };

    Header* FreeListBegin;

    uint64_t PhysLimit;
    uint64_t TotalMem;
    uint64_t FreeMem;

    static volatile limine_memmap_request MemmapRequest = {
        .id = LIMINE_MEMMAP_REQUEST,
        .revision = 0
    };

    void Init()
    {
        FreeListBegin = nullptr;
        for(size_t i = 0; i < MemmapRequest.response->entry_count; i++)
        {
            limine_memmap_entry* entry = MemmapRequest.response->entries[i];

            if(entry->base + entry->length < 0x100000)
                continue;

            uint64_t newPhysLimit = entry->base + entry->length;
            if(newPhysLimit > PhysLimit)
                PhysLimit = newPhysLimit;
            
            if(entry->type == LIMINE_MEMMAP_USABLE)
                TotalMem += entry->length;
        }
        FreeMem = TotalMem;

        Header* current = FreeListBegin;
        for(size_t i = 0; i < MemmapRequest.response->entry_count; i++)
        {
            limine_memmap_entry* entry = MemmapRequest.response->entries[i];

            if(entry->base + entry->length < 0x100000)
                continue;

            if(entry->type == LIMINE_MEMMAP_USABLE)
            {
                if(FreeListBegin == nullptr)
                {
                    current = (Header*)entry;
                    FreeListBegin = current;
                }
                else
                    current->next = (Header*)entry;
            }
        }

        //Header* entry = FreeListBegin;
        //for(size_t i = 0; i < 2; i++)
        //{
        //    entry = entry->next;
        //    Terminal::Printf("\nFreeList Entry - Size: %d", entry->size);
        //}
    }

    void* GetPage()
    {
        return GetPages(1);
    }

    void FreePage(void* address)
    {
        FreePages(address, 1);
    }

    void* GetPages(uint32_t npages)
    {
        if(npages == 0)
            return nullptr;
        
        Header* current = FreeListBegin;
        Header* previous = FreeListBegin;

        uint64_t size = npages * PAGE_SIZE + sizeof(Header);

        for(;;)
        {
            if(current->size >= size)
            {
                if(current->size == size)
                    previous->next = current->next;
                else
                {
                    *(Header*)((char*)current + size) = Header(current->size - size);
                    FreeListBegin = (Header*)((char*)current + size);
                    current->size = size;
                }
                FreeMem -= size;
                return current + 1;
            }

            previous = current;
            current = current->next;
        }
        return nullptr;
    }

    void FreePages(void* address, uint32_t npages)
    {
        if(address == nullptr)
            return;

        Header* header = (Header*)address - 1;
        header->next = FreeListBegin;
        FreeMem += header->size;
        FreeListBegin = header;
    }

    void DumpStats()
    {
        uint64_t used = TotalMem - FreeMem;
        Terminal::Printf("\nTotal memory: %d KiB(%d MiB)", TotalMem / 0x400, TotalMem / 0x400 / 0x400);
        Terminal::Printf("\nFree memory:  %d KiB(%d MiB)", FreeMem / 0x400, FreeMem / 0x400 / 0x400);
        Terminal::Printf("\nUsed memory:  %d KiB(%d MiB)", used / 0x400, used / 0x400 / 0x400);
    }
}