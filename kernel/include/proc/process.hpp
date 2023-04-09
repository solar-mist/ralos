#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP
#include <cpu/interrupt/isr.hpp>
#include <mm/paging.hpp>

struct Process
{
    Process(uint64_t entry, Paging::AddressSpace addrspace);

    InterruptFrame state;
    Paging::AddressSpace addrspace;
};

#endif