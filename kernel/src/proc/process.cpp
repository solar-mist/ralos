#include <proc/process.hpp>
#include <mm/vmm.hpp>
#include <mm/pmm.hpp>

extern "C" void enter_usermode(void (*func)(), void* stack);

Process::Process(uint64_t entry, Paging::AddressSpace addrspace)
{
    frame.BaseFrame.rip = entry;
    frame.BaseFrame.rsp = (uint64_t)VMM::GetPages(&addrspace, 4, 7) + (4 * PAGE_SIZE) - 1;
    frame.BaseFrame.rbp = frame.BaseFrame.rbp;
}

void Process::Launch()
{
    enter_usermode((void (*)())frame.BaseFrame.rip, (void*)frame.BaseFrame.rsp);
}