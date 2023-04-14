#include <syscall/mem.hpp>
#include <mm/vmm.hpp>
#include <mm/paging.hpp>
#include <mm/pmm.hpp>
#include <kernel/sched/scheduler.hpp>

int SysMmap(void* addr, size_t count)
{
    Paging::MapPages(&Scheduler::CurrentProcess()->addrspace, 0, (uint64_t)addr, 0x407, NPAGES(count));
    return 0;
}