#include <mem/pmm.hpp>
#include <drivers/terminal.hpp>
#include <stddef.h>
#include <stdint.h>

uint8_t* Bitmap;
uint64_t PhysLimit = 0;
uint64_t TotalMemory = 0;
uint64_t FreeMemory = 0;

void BmpMarkUsed(void* addr, uint32_t npages)
{
    for(uint64_t i = (uint64_t)addr; i < (uint64_t)addr + (npages * PAGE_SIZE); i += PAGE_SIZE)
        Bitmap[i / (PAGE_SIZE * 8)] &= ~(1 << ((i / PAGE_SIZE) % 8));
}

[[gnu::noinline]] bool BmpTest(void* addr, uint32_t npages)
{
    bool result = true;

    for(uint32_t i = (uint64_t)addr; i < (uint64_t)addr + (npages * PAGE_SIZE); i += PAGE_SIZE)
    {
        result = Bitmap[i / (PAGE_SIZE * 8)] & (1 << ((i / PAGE_SIZE) % 8));
        if (!result)
            break;
    }

    return result;
}

namespace PMM
{
    void Init(VBMemoryMapEntry* memoryMap)
    {
        uint8_t* mmap_start = (uint8_t*)memoryMap;
        uint8_t* mmap_end = (uint8_t*)mmap_start + memoryMap->MapSize;

        for(uint8_t* offset = mmap_start; offset < mmap_end; offset += memoryMap->EntrySize)
        {
            VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

            if(entry->Base + entry->Size < 0x100000)
                continue;

            uint64_t NewPhysLimit = entry->Base + entry->Size;
            if(NewPhysLimit > PhysLimit)
                PhysLimit = NewPhysLimit;

            if(entry->Type == VBMemoryUsable)
                TotalMemory += entry->Size;
        }

        uint32_t BitmapSize = PhysLimit / (PAGE_SIZE * 8);
        for(uint8_t* offset = mmap_start; offset < mmap_end; offset += memoryMap->EntrySize)
        {
            VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

            if(entry->Base + entry->Size < 0x100000)
                continue;
            
            if(entry->Size >= BitmapSize && (entry->Type == VBMemoryUsable))
            {
                Bitmap = (uint8_t*)entry->Base;
                break;
            }
        }

        for(uint8_t* offset = mmap_start; offset < mmap_end; offset += memoryMap->EntrySize)
        {
            VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

            if(entry->Base + entry->Size < 0x100000)
                continue;

            if(entry->Type == VBMemoryUsable)
            {
                FreePages((void*)entry->Base, NPAGES(entry->Size));
            }
        }

        Alloc(Bitmap, BitmapSize);
    }

    bool Alloc(void* address, uint32_t npages)
    {
        if(!BmpTest(address, npages))
            return false;

        BmpMarkUsed(address, npages);
        FreeMemory -= npages * PAGE_SIZE;
        return true;
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
        static uint32_t last_used_page;

        for(uint64_t i = last_used_page; i < PhysLimit; i += PAGE_SIZE)
        {
            if(Alloc((void*)i, npages))
                return (void*)i;
        }

        for(uint64_t i = 0; i < last_used_page; i += PAGE_SIZE)
        {
            if(Alloc((void*)i, npages))
                return (void*)i;
        }

        // TODO: Panic - out of physical memory
        for(;;);
    }

    void FreePages(void* address, uint32_t npages)
    {
        for(uint64_t i = (uint64_t)address; i < (uint64_t)address + (npages * PAGE_SIZE); i += PAGE_SIZE)
        {
            if(!BmpTest((void*)i, 1))
                FreeMemory += PAGE_SIZE;
            
            Bitmap[i / (PAGE_SIZE * 8)] |= 1 << ((i / PAGE_SIZE) % 8);
        }
    }

    void DumpStats()
    {
        uint32_t used = TotalMemory - FreeMemory;
        Terminal::Print("\nTotal(KiB): ");
        Terminal::PrintInt(TotalMemory / 1024, 10, 0x00FFFF);
        Terminal::PutChar('\n', 0);

        Terminal::Print("Free(KiB):  ");
        Terminal::PrintInt(FreeMemory / 1024, 10, 0x00FFFF);
        Terminal::PutChar('\n', 0);

        Terminal::Print("Used(KiB):  ");
        Terminal::PrintInt(used / 1024, 10, 0x00FFFF);
        Terminal::PutChar('\n', 0);
    }
}