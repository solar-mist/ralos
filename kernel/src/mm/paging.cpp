#include <mm/paging.hpp>
#include <mm/pmm.hpp>
#include <drivers/terminal.hpp>
#include <kernel/kernel.hpp>
#include <viper.h>
#include <stddef.h>

void memset(uint8_t* buffer, uint8_t n, uint32_t size)
{
    for(uint32_t i = 0; i < size; i++)
        buffer[i] = n;
}

namespace PMM
{
    extern ViperMemmapResponse* MemMap;
}

namespace Graphics
{
    extern FB framebuffer;
}

static volatile ViperKernelLocationRequest kernelLocation = {
    .id = VIPER_KERNEL_LOCATION
};

extern char _kernel_start[];
extern char _kernel_end[];

namespace Paging
{
    uint64_t* pml4;

    #define PRESENT_WRITABLE 0x3

    static inline uint64_t Entry(uint64_t pt)
    {
        return (pt & ~0xFFFF000000000FFF);
    }

    void Init()
    {
        pml4 = (uint64_t*)PMM::GetPage();
        memset((uint8_t*)pml4, 0, PAGE_SIZE);
        pml4 = (uint64_t*)Entry((uint64_t)pml4);

        MapPages((uint64_t)kernelLocation.response->physicalBase, (uint64_t)kernelLocation.response->virtualBase, 3, NPAGES(_kernel_end - _kernel_start));
        MapPages(4096, 0xFFFF800000000000, 3, NPAGES(0x40000000));
        
        for(size_t i = 0; i < PMM::MemMap->count; i++)
        {
            ViperMemmapEntry* entry = PMM::MemMap->entries + i;

            if(entry->base + entry->size < 0x400000)
                continue;

            MapPages(entry->base, entry->base + 0xFFFF800000000000, 3, NPAGES(entry->size));
        }

        MapPages((uint64_t)Graphics::framebuffer.base - 0xFFFF800000000000, (uint64_t)Graphics::framebuffer.base, 3, NPAGES(Graphics::framebuffer.size));
 
        asm volatile("mov %0, %%rax; mov %%rax, %%cr3" : : "m"(pml4));
    }

    void MapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags)
    {
        PhysAddr &= ~0xFFF;
        VirtAddr &= ~0xFFF;
        Flags    &=  0xFFF;

        if(VirtAddr & ((uint64_t)1 << 47))
        {
            if((VirtAddr & 0xFFFF000000000000) != 0xFFFF000000000000)
                Kernel::Panic("Failed to map page - Non-canonical address: %x", VirtAddr);
        }

        uint64_t shift = 48;
        uint64_t* pt = (uint64_t*)PhysToVirt(Entry((uint64_t)pml4));
        uint64_t idx;
        for(char i = 0; i < 4; i++)
        {
            shift -= 9;
            idx = (VirtAddr >> shift) & 0x1FF;
            if(i == 3)
                break;
            
            if(!(pt[idx] & 0x1))
            {
                pt[idx] = (uint64_t)PMM::GetPage();
                memset((uint8_t*)PhysToVirt(Entry(pt[idx])), 0, PAGE_SIZE);
                pt[idx] |= Flags;
            }
            pt = (uint64_t*)PhysToVirt(Entry(pt[idx]));
        }
        pt[idx] = PhysAddr | Flags;
    }

    void MapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages)
    {
        for(uint64_t i = 0; i < Npages * PAGE_SIZE; i += PAGE_SIZE)
            MapPage(PhysAddr + i, VirtAddr + i, Flags);
    }
}