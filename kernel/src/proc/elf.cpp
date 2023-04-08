#include <proc/elf.hpp>
#include <kernel/modules.hpp>
#include <mm/pmm.hpp>
#include <mm/paging.hpp>

namespace ELF
{
    constexpr int PT_LOAD = 1;
    struct Elf64_Ehdr
    {
        uint8_t	e_ident[16];
        uint16_t e_type;
        uint16_t e_machine;
        uint32_t e_version;
        uint64_t e_entry;
        uint64_t e_phoff;
        uint64_t e_shoff;
        uint32_t e_flags;
        uint16_t e_ehsize;
        uint16_t e_phentsize;
        uint16_t e_phnum;
        uint16_t e_shentsize;
        uint16_t e_shnum;
        uint16_t e_shstrndx;
    };

    struct Elf64_Phdr {
        uint32_t p_type;
        uint32_t p_flags;
        uint64_t p_offset;
        uint64_t p_vaddr;
        uint64_t p_paddr;
        uint64_t p_filesz;
        uint64_t p_memsz;
        uint64_t p_align;
    };

    Executable ParseELF(void* buffer)
    {
        char* buf = (char*)buffer;
        Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
        Elf64_Phdr* phdr = (Elf64_Phdr*)(buf + ehdr->e_phoff);

        for(uint32_t i = 0; i < ehdr->e_phnum; i++, phdr++)
        {
            if(phdr->p_type != PT_LOAD)
                continue;
            
            char* vaddr = (char*)phdr->p_vaddr;
            uint64_t paddr = (uint64_t)PMM::GetPages(NPAGES(phdr->p_memsz));
            Paging::MapPages(paddr, (uint64_t)vaddr, 7, NPAGES(phdr->p_memsz));
            char* fileOff = buf + phdr->p_offset;
            for(uint64_t j = 0; j < phdr->p_filesz; j++)
                *(vaddr + j) = *(fileOff + j);
            
            vaddr += phdr->p_filesz;
            for(uint64_t j = 0; j < phdr->p_memsz - phdr->p_filesz; j++)
                *(vaddr + j) = 0;
        }

        return Executable {
            (void(*)())ehdr->e_entry
        };
    }
}