#ifndef PROC_ELF_HPP
#define PROC_ELF_HPP
#include <mm/paging.hpp>

namespace ELF
{
    constexpr uint32_t E_MAG = 0;
    constexpr uint32_t E_32 = 1;
    constexpr uint32_t E_BE = 2;
    constexpr uint32_t E_EXEC = 3;
    constexpr uint32_t E_MACHINE = 4;

    struct Executable
    {
        void(*entry)();
        uint32_t error;
        char* interpPath;
    };

    Executable ParseELFExec(void* buffer, Paging::AddressSpace addrspace, const char* path, bool interp = false);
}

#endif