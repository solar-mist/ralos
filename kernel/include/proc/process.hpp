#ifndef PROC_PROCESS_HPP
#define PROC_PROCESS_HPP
#include <proc/fd.hpp>
#include <cpu/interrupt/isr.hpp>
#include <mm/paging.hpp>

struct Process
{
    Process(uint64_t entry, Paging::AddressSpace addrspace, char* interpPath);

    uint64_t stack;
    uint64_t stackSize;
    uint64_t pid;
    InterruptFrame state;
    Paging::AddressSpace addrspace;
    ProcFile* fdTable;
    char* interpPath;
};

Process* ActiveProcess();
void SetActiveProcess(Process* proc);

#endif