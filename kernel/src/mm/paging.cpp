#include <mm/paging.hpp>
#include <mm/pmm.hpp>
#include <drivers/terminal.hpp>
#include <viper.h>
#include <stddef.h>

void memset(uint8_t* buffer, uint32_t size, uint8_t n)
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

[[gnu::always_inline]] constexpr inline uint32_t VirtToPhys(uint64_t addr)
{
    return addr - 0xFFFF800000000000;
}
[[gnu::always_inline]] constexpr inline uint32_t PhysToVirt(uint64_t addr)
{
    return addr + 0xFFFF800000000000;
}

extern char _kernel_start[];
extern char _kernel_end[];

namespace Paging
{
    uint64_t* pml4;

    #define PRESENT_WRITABLE 0x3

    void Init()
    {
        pml4 = (uint64_t*)PMM::GetPage();
        memset((uint8_t*)pml4, PAGE_SIZE, 0);
        uint64_t tmp = (uint64_t)pml4;
        tmp &= ~0xFFFF000000000FFF;
        pml4 = (uint64_t*)tmp;

        MapPages((uint64_t)kernelLocation.response->physicalBase, (uint64_t)kernelLocation.response->virtualBase, 3, NPAGES(_kernel_end - _kernel_start));
        //MapPages(0, 0xFFFF800000000000, 3, 2);
        MapPages(0x1000, 0x1000, 3, 2);
        /*
        for(size_t i = 0; i < PMM::MemMap->count; i++)
        {
            ViperMemmapEntry* entry = PMM::MemMap->entries + i;

            if(entry->base + entry->size < 0x400000)
                continue;

            MapPages(entry->base, entry->base + 0xFFFF800000000000, 3, NPAGES(entry->size));
        }*/

        MapPages((uint64_t)Graphics::framebuffer.base, (uint64_t)Graphics::framebuffer.base + 0xFFFF800000000000, 3, NPAGES(Graphics::framebuffer.size));
 
        asm volatile("mov %0, %%rax; mov %%rax, %%cr3" : : "m"(pml4));
        for(;;);
    }

    static inline uint64_t* Entry(uint64_t pt)
    {
        return (uint64_t*)(pt & ~0xFFFF000000000FFF);
    }

    void MapPage(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags)
    {
        PhysAddr &= ~0xFFF;
        VirtAddr &= ~0xFFF;
        Flags    &=  0xFFF;

        if(VirtAddr & ((uint64_t)1 << 47))
            VirtAddr |= 0xFFFF000000000000;

        uint64_t shift = 48;
        uint64_t* pt = Entry((uint64_t)pml4);
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
                memset((uint8_t*)Entry(pt[idx]), PAGE_SIZE, 0);
                pt[idx] |= Flags;
            }
            pt = Entry(pt[idx]);
        }
        pt[idx] = PhysAddr | Flags;
        pt[idx] &= ~0xFFFF000000000000;
    }

    void MapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages)
    {
        for(uint64_t i = 0; i < Npages * PAGE_SIZE; i += PAGE_SIZE)
            MapPage(PhysAddr + i, VirtAddr + i, Flags);
    }
}