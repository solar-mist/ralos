#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP
#include <cpu/interrupt/isr.hpp>
#include <mm/paging.hpp>

struct Process
{
    Process(uint64_t entry, Paging::AddressSpace addrspace);

    uint64_t stack;
    uint64_t stackSize;
    uint64_t pid;
    InterruptFrame state;
    Paging::AddressSpace addrspace;
};

#endif