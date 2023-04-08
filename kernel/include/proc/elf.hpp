#ifndef PROC_ELF_HPP
#define PROC_ELF_HPP

namespace ELF
{
    struct Executable
    {
        void(*entry)();
    };

    Executable ParseELF(void* buffer);
}

#endif