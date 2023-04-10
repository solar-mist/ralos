#include <proc/process.hpp>
#include <mm/vmm.hpp>
#include <mm/pmm.hpp>

static uint64_t pidCount = 1;

Process::Process(uint64_t entry, Paging::AddressSpace addrspace)
    :addrspace(addrspace), pid(pidCount++)
{
    state.BaseFrame.rip = entry;
    state.BaseFrame.rsp = (uint64_t)VMM::GetPages(&addrspace, 4, 7) + (4 * PAGE_SIZE) - 1;
    state.BaseFrame.rbp = state.BaseFrame.rbp;
    state.ControlRegisters.cr3 = (uint64_t)addrspace.pml4;

    stack = state.BaseFrame.rsp;
    stackSize = 4;
}