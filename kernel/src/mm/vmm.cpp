#include <mm/vmm.hpp>
#include <mm/kheap.hpp>
#include <mm/pmm.hpp>
#include <drivers/terminal.hpp>
#include <cpu/interrupt/exception.hpp>
#include <kernel/sched/scheduler.hpp>
#include <libk/new.hpp>

namespace VMM
{
    void PageFaultHandler(InterruptFrame* frame)
    {
        Paging::AddressSpace addrspace = Paging::AddressSpace{ (uint64_t*)frame->ControlRegisters.cr3, nullptr };
        uint64_t flags = Paging::GetEntry(&addrspace, frame->ControlRegisters.cr2) | 0x1;
        if(flags & (1 << 10))
        {
            Paging::MapPage(&addrspace, (uint64_t)PMM::GetPage(), frame->ControlRegisters.cr2, flags & ~(1 << 10));
            frame->ControlRegisters.cr2 = 0;
        }
        else
        {
            if((frame->BaseFrame.cs & 3) == 3)
            {
                Terminal::Printf(0xFF0000, "\nPage fault in process %d: CR2=%x", Scheduler::CurrentProcess()->pid, frame->ControlRegisters.cr2);
                for(;;);
            }
            Terminal::Printf(0xFF0000, "\nPage fault - Flags: %d", flags & 0xFFE);
            for(;;);
        }
    }

    void Init()
    {
        RegisterExceptionHandler(0xE, PageFaultHandler);
    }

    void* GetPages(Paging::AddressSpace* addrspace, uint32_t npages, uint16_t flags)
    {
        if(npages == 0)
            return (void*)-1;
        
        if(addrspace == nullptr)
            addrspace = Paging::KernelAddrSpace();
        
        flags &= 0xFFE;
        flags |= (1 << 10);

        Paging::Node* current = addrspace->freeListBegin;
        Paging::Node* previous = nullptr;
        while(current != nullptr)
        {
            if(current->npages >= npages)
            {
                if(current->npages == npages)
                {
                    if(previous)
                        previous->next = current->next;
                    else
                        addrspace->freeListBegin = current->next;
                    
                    void* addr = current->base;
                    free((void*)current);
                    Paging::MapPages(addrspace, 0, (uint64_t)addr, flags, npages);
                    return addr;
                }
                else
                {
                    if(previous)
                        previous->next = current->next;
                    else
                        addrspace->freeListBegin = current->next;

                    void* addr = current->base;
                    uint32_t newNpages = current->npages - npages;

                    current->npages = newNpages;
                    current->base = (char*)current->base + npages * PAGE_SIZE;
                    current->next = addrspace->freeListBegin;

                    addrspace->freeListBegin = current;

                    Paging::MapPages(addrspace, 0, (uint64_t)addr, flags, npages);
                    return addr;
                }
            }

            previous = current;
            current = current->next;
        }
        return (void*)-1;
    }

    void FreePages(Paging::AddressSpace* addrspace, void* addr, uint32_t npages)
    {
        if(addr == nullptr || npages == 0)
            return;
        
        for(uint32_t i = 0; i < npages * PAGE_SIZE; i += PAGE_SIZE)
        {
            if(uint64_t entry = Paging::GetEntry(addrspace, (uint64_t)addr + i) & ~0xFFF)
                PMM::FreePage((void*)entry);
        }

        Paging::UnmapPages(addrspace, (uint64_t)addr, npages);

        Paging::Node* newNode = (Paging::Node*)malloc(sizeof(Paging::Node));
        newNode->base = addr;
        newNode->npages = npages;
        newNode->next = addrspace->freeListBegin;
        addrspace->freeListBegin = newNode;
    }

    bool MarkUsed(Paging::AddressSpace* addrspace, void* addr, uint32_t npages)
    {
        Paging::Node* current = addrspace->freeListBegin;
        Paging::Node* previous = nullptr;
        while(current != nullptr)
        {
            if(addr >= current->base && (char*)addr + npages * PAGE_SIZE <= (char*)current->base + current->npages * PAGE_SIZE)
            {
                if(addr == current->base && (char*)addr + npages * PAGE_SIZE == (char*)current->base + current->npages * PAGE_SIZE)
                {
                    if(previous)
                        previous->next = current->next;
                    else
                        addrspace->freeListBegin = current->next;
                    free((void*)current);
                    return false;
                }
                
                current->npages = NPAGES((char*)addr - (char*)current->base);

                Paging::Node* newNode = (Paging::Node*)malloc(sizeof(Paging::Node));

                newNode->base = (char*)addr + npages * PAGE_SIZE;
                newNode->npages = (char*)current->base + current->npages * PAGE_SIZE - (char*)addr + npages * PAGE_SIZE;
                newNode->next = addrspace->freeListBegin;

                addrspace->freeListBegin = newNode;
                return false;
            } // TODO: Check if across multiple nodes
            previous = current;
            current = current->next;
        }
        return true;
    }
}