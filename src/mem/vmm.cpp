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

    void Init(VBInfo* BootInfo)
    {
        pml4 = (uint64_t*)PMM::GetPage();
        memset((uint8_t*)pml4, PAGE_SIZE, 0);
        uint64_t tmp = (uint64_t)pml4;
        tmp &= ~0xFFFF000000000FFF;
        pml4 = (uint64_t*)tmp;
        MapPages(0, 0, 7, NPAGES(0x400000));
        uint8_t* mmap_start = (uint8_t*)BootInfo->MemoryMap;
        uint8_t* mmap_end = (uint8_t*)mmap_start + BootInfo->MemoryMap->MapSize;

        for(uint8_t* offset = mmap_start; offset < mmap_end; offset += BootInfo->MemoryMap->EntrySize)
        {
            VBMemoryMapEntry* entry = (VBMemoryMapEntry*)offset;

            if(entry->Base + entry->Size < 0x400000)
                continue;

            MapPages(entry->Base, entry->Base, 7, entry->Size / 4096);
        }
        MapPages((uint64_t)BootInfo->Framebuffer.Base, (uint64_t)BootInfo->Framebuffer.Base, 7, NPAGES(BootInfo->Framebuffer.Size));
        asm volatile("mov %0, %%cr3" : : "r"(pml4));
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
                pt[idx] |= 7;
                pt[idx] &= ~0xFFFF000000000000;
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

    void Dump(uint64_t idx)
    {
        uint64_t* pml3 = (uint64_t*)Entry((uint64_t)(pml4))[idx];
        uint64_t* pml2 = (uint64_t*)Entry((uint64_t)(pml3))[idx];
        uint64_t* pml1 = (uint64_t*)Entry((uint64_t)(pml2))[idx];
        uint64_t  paddr = Entry((uint64_t)(pml1))[idx];
        Terminal::Print("\nPML3: ");
        Terminal::PrintInt((uint64_t)pml3);
        Terminal::Print("\nPML2: ");
        Terminal::PrintInt((uint64_t)pml2);
        Terminal::Print("\nPML1: ");
        Terminal::PrintInt((uint64_t)pml1);
        Terminal::Print("\nPaddr: ");
        Terminal::PrintInt(paddr);
    }


    uint64_t GetPhysAddr(uint64_t VirtAddr)
    {
        VirtAddr &= ~0xFFF;

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
                Terminal::PrintInt(pt[idx], 10, 0xFF00FF);
                return -(3 - i);
            }
            pt = Entry(pt[idx]);
        }

        return pt[idx];
    }
}