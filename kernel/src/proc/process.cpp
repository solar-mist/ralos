#include <proc/process.hpp>
#include <mm/vmm.hpp>
#include <mm/pmm.hpp>

Process::Process(uint64_t entry, Paging::AddressSpace addrspace)
{
    state.BaseFrame.rip = entry;
    state.BaseFrame.rsp = (uint64_t)VMM::GetPages(&addrspace, 4, 7) + (4 * PAGE_SIZE) - 1;
    state.BaseFrame.rbp = state.BaseFrame.rbp;
    state.ControlRegisters.cr3 = (uint64_t)addrspace.pml4;
}