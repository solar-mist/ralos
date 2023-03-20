#include <mem/vmm.hpp>
#include <mem/pmm.hpp>
#include <drivers/terminal.hpp>

void memset(uint8_t* buffer, uint32_t size, uint8_t n)
{
    for(uint32_t i = 0; i < size; i++)
        buffer[i] = n;
}

namespace VMM
{
    uint64_t* pml4;

    #define PRESENT_WRITABLE 0x3

    void Init()
    {
        pml4 = (uint64_t*)PMM::GetPage();
        MapPages(0, 0, 3, NPAGES(PMM::GetPhysLimit()));
        asm volatile("mov %0, %%rax; " \
                    "mov %%rax, %%cr3" : "=g"(pml4) : : "rax");
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

        uint64_t shift = 48;
        uint64_t* pt = pml4;
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
                pt[idx] |= 7;
                pt[idx] &= ~0xFFFF000000000FFF;
            }
            pt = Entry(pt[idx]);
        }
        pt[idx] = PhysAddr | Flags | 1;
        pt[idx] &= ~0xFFFF000000000000;
        if((pt[idx] & ~0xFFF) != (PhysAddr))
        {
            Terminal::Print("Page Map Error", 0xFF0000);
            Terminal::PrintInt(pt[idx] & ~0xFFF, 16, 0x00FFFF);
            asm volatile("cli; hlt");
        }
    }

    void MapPages(uint64_t PhysAddr, uint64_t VirtAddr, uint16_t Flags, uint64_t Npages)
    {
        for(uint64_t i = 0; i < Npages * PAGE_SIZE; i += PAGE_SIZE)
            MapPage(PhysAddr + i, VirtAddr + i, Flags);
    }


    uint64_t GetPhysAddr(uint64_t VirtAddr)
    {
        VirtAddr &= ~0xFFF;

        uint64_t shift = 48;
        uint64_t* pt = (uint64_t*)((uint64_t)pml4 >> 12);
        uint64_t idx;
        for(char i = 0; i < 4; i++)
        {
            shift -= 9;
            idx = (VirtAddr >> shift) & 0x1FF;
            if(i == 3)
                break;
            
            if(!(pt[idx] & 0x1))
                return -(4 - i);

            pt = Entry(pt[idx]);
        }

        Terminal::PrintInt(pt[idx], 16, 0xFF00FF);

        return pt[idx] & ~0xFFF;
    }
}