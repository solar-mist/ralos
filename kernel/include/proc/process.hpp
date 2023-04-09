#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP
#include <cpu/interrupt/isr.hpp>
#include <mm/paging.hpp>

struct Process
{
    Process(uint64_t entry);

    InterruptFrame frame;
    Paging::AddressSpace addrspace;
};

#endif