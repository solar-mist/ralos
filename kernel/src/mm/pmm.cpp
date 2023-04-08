#include <mm/pmm.hpp>
#include <drivers/terminal.hpp>
#include <viper.h>
#include <stddef.h>

void* operator new(size_t s, char* buf)
{
    return buf;
}

void* operator new[](size_t s, char* buf)
{
    return buf;
}

extern void memset(uint8_t*, uint8_t, uint32_t);

namespace PMM
{
    struct MemoryRegion
    {
        MemoryRegion(uint64_t size)
            :totalSize(size), bitmap(nullptr), next(nullptr), lastPage(nullptr) {}

        uint8_t* bitmap;
        uint8_t* base;
        uint8_t* lastPage;
        MemoryRegion* next;
        uint64_t totalSize;
        uint64_t totalFree;
    };

    void BmpMarkUsed(MemoryRegion* region, void* addr, uint32_t npages)
    {
        uint64_t address = (uint8_t*)addr - region->base;

        for(uint64_t i = address; i < address + (npages * PAGE_SIZE); i += PAGE_SIZE)
            region->bitmap[i / (PAGE_SIZE * 8)] &= ~(1 << ((i / PAGE_SIZE) % 8));
    }

    bool BmpTest(MemoryRegion* region, void* addr, uint32_t npages)
    {
        bool result = true;
        uint64_t address = (uint8_t*)addr - region->base;

        for(uint64_t i = address; i < address + (npages * PAGE_SIZE); i += PAGE_SIZE)
        {
            result = region->bitmap[i / (PAGE_SIZE * 8)] & (1 << ((i / PAGE_SIZE) % 8));
            if (!result)
                break;
        }

        return result;
    }

    static volatile ViperMemmapRequest MemmapRequest = {
        .id = VIPER_MEMMAP
    };

    static MemoryRegion* regions = nullptr;

    ViperMemmapResponse* MemMap;

    void Init()
    {
        MemMap = MemmapRequest.response;

        for(size_t i = 0; i < MemmapRequest.response->count; i++)
        {
            ViperMemmapEntry* entry = MemmapRequest.response->entries + i;

            if(entry->base + entry->size < 0x100000)
                continue;

            if(entry->type == ViperMemmapUsable)
            {
                MemoryRegion* region = new((char*)entry->base) MemoryRegion(entry->size);
                uint64_t bitmapSize = region->totalSize / (PAGE_SIZE * 8);

                region->bitmap = new((char*)entry->base + sizeof(MemoryRegion)) uint8_t[bitmapSize];

                region->base = (uint8_t*)(((entry->base + (bitmapSize) + sizeof(MemoryRegion)) & ~0xFFF) + 0x1000);
                region->totalSize -= NPAGES(entry->base + (bitmapSize) + sizeof(MemoryRegion));
                region->totalFree = region->totalSize;

                memset(region->bitmap, 0xFF, bitmapSize);

                if(!regions)
                    regions = region;
                else
                {
                    region->next = regions;
                    regions = region;
                }
            }
        }
    }

    void* GetPage()
    {
        return GetPages(1);
    }

    void FreePage(void* address)
    {
        FreePages(address, 1);
    }
    
    bool Alloc(MemoryRegion* region, void* address, uint32_t npages)
    {
        if(!BmpTest(region, address, npages))
            return false;
        
        BmpMarkUsed(region, address, npages);
        region->totalFree -= npages * PAGE_SIZE;
        region->lastPage = (uint8_t*)address;
        return true;
    }

    void* GetPages(uint32_t npages)
    {
        if(npages == 0)
            return nullptr;
        
        MemoryRegion* region = regions;
        while(region != nullptr)
        {
            if(region->lastPage != nullptr)
            {
                for(uint8_t* addr = region->lastPage; addr < region->base + region->totalSize; addr += PAGE_SIZE)
                {
                    if(Alloc(region, addr, npages))
                        return addr;
                }
            }
            for(uint8_t* addr = region->base; addr < region->base + region->totalSize; addr += PAGE_SIZE)
            {
                if(Alloc(region, addr, npages))
                    return addr;
            }
            region = region->next;
        }
        return nullptr;
    }

    void FreePages(void* address, uint32_t npages)
    {
        if(address == nullptr || npages == 0)
            return;

        MemoryRegion* region = regions;
        while(region != nullptr)
        {
            if(address >= region->base && address <= region->base + region->totalSize)
            {
                for(uint64_t i = (uint64_t)address; i < (uint64_t)address + npages * PAGE_SIZE; i += PAGE_SIZE)
                {
                    if(!BmpTest(region, (void*)i, 1))
                        region->totalFree += npages * PAGE_SIZE;

                    uint64_t addr = i - (uint64_t)region->base;
                    region->bitmap[addr / (PAGE_SIZE / 8)] |= 1 << ((addr / PAGE_SIZE) % 8);
                }
                return;
            }
            region = region->next;
        }
    }

    void DumpStats()
    {
        uint64_t total = 0;
        uint64_t free = 0;
        MemoryRegion* region = regions;
        while(region != nullptr)
        {
            total += region->totalSize;
            free += region->totalFree;

            region = region->next;
        }
        uint64_t used = total - free;

        Terminal::Printf(0x00FFFF, "\nTotal memory: %d KiB(%d MiB)", total / 0x400, total / 0x400 / 0x400);
        Terminal::Printf(0x00FFFF, "\nFree memory:  %d KiB(%d MiB)", free / 0x400, free / 0x400 / 0x400);
        Terminal::Printf(0x00FFFF, "\nUsed memory:  %d KiB(%d MiB)", used / 0x400, used / 0x400 / 0x400);
    }
}