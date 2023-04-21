#include <syscall/mem.hpp>
#include <mm/vmm.hpp>
#include <mm/paging.hpp>
#include <mm/pmm.hpp>
#include <kernel/sched/scheduler.hpp>

uint64_t SysMmap(void* addr, size_t count)
{
    if(addr == nullptr)
        addr = VMM::GetPages(&Scheduler::CurrentProcess()->addrspace, NPAGES(count), 7);
    else
        Paging::MapPages(&Scheduler::CurrentProcess()->addrspace, 0, (uint64_t)addr, 0x407, NPAGES(count));
    return (uint64_t)addr;
}