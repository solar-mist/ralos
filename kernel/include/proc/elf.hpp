#ifndef PROC_ELF_HPP
#define PROC_ELF_HPP
#include <mm/paging.hpp>

namespace ELF
{
    struct Executable
    {
        void(*entry)();
    };

    Executable ParseELF(void* buffer, Paging::AddressSpace addrspace);
}

#endif