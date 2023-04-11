#include "mm/kheap.hpp"
#include <proc/process.hpp>
#include <mm/vmm.hpp>
#include <mm/pmm.hpp>

static uint64_t pidCount = 1;
static Process* activeProcess;

Process::Process(uint64_t entry, Paging::AddressSpace addrspace)
    :addrspace(addrspace), pid(pidCount++), fdTable((ProcFile*)malloc(sizeof(ProcFile) * FD_MAX)), fdNum(3)
{
    state.BaseFrame.rip = entry;
    state.BaseFrame.rsp = (uint64_t)VMM::GetPages(&addrspace, 4, 7) + (4 * PAGE_SIZE) - 1;
    state.BaseFrame.rbp = state.BaseFrame.rbp;
    state.ControlRegisters.cr3 = (uint64_t)addrspace.pml4;

    stack = state.BaseFrame.rsp;
    stackSize = 4;

    for(uint32_t i = 3; i < FD_MAX; i++)
        fdTable[i].node = nullptr;
}

Process* ActiveProcess()
{
    return activeProcess;
}

void SetActiveProcess(Process* proc)
{
    activeProcess = proc;
}