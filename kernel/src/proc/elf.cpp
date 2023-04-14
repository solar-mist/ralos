#include <proc/elf.hpp>
#include <mm/pmm.hpp>
#include <mm/paging.hpp>
#include <mm/vmm.hpp>
#include <mm/kheap.hpp>
#include <libk/mem.hpp>
#include <linux/elf.h>
#include <kernel/kernel.hpp>
#include <fs/vfs.hpp>
#include <drivers/terminal.hpp>

namespace ELF
{
    inline Elf64_Shdr* ELFShdr(Elf64_Ehdr* ehdr)
    {
        return (Elf64_Shdr*)((char*)ehdr + ehdr->e_shoff);
    }

    inline Elf64_Shdr* ElfSection(Elf64_Ehdr* ehdr, int idx)
    {
        return ELFShdr(ehdr)+idx;
    }

    inline char* ELFShStrTab(Elf64_Ehdr* ehdr)
    {
        if(ehdr->e_shstrndx == SHN_UNDEF)
            return nullptr;
        
        return (char*)ehdr + ElfSection(ehdr, ehdr->e_shstrndx)->sh_offset;
    }

    inline char* LookupShString(char* tab, int off)
    {
        if(!tab)
            return nullptr;
        
        return tab + off;
    }

    inline Elf64_Shdr* GetSection(Elf64_Ehdr* ehdr, const char* name)
    {
        for(int i = 0; i < ehdr->e_shnum; i++)
        {
            if(!strcmp(LookupShString(ELFShStrTab(ehdr), ElfSection(ehdr, i)->sh_name), name))
                return ElfSection(ehdr, i);
        }
        return nullptr;
    }

    Executable ParseELFExec(void* buffer, Paging::AddressSpace addrspace, void* interpAddr)
    {
        char* buf = (char*)buffer;
        //Elf64_Ehdr* volatile ehdr = (Elf64_Ehdr*)buf;
        Elf64_Ehdr ehdr;
        memcpy(&ehdr, buffer, sizeof(Elf64_Ehdr));
        //Terminal::Printf(0xffffff, "%x\n", ehdr->e_ident[EI_MAG0] != 0x7F);
        if(ehdr.e_ident[EI_MAG0] != 0x7F)
            return Executable { 0, E_MAG };
        else if(ehdr.e_ident[EI_MAG1] != 'E')
            return Executable { 0, E_MAG };
        else if(ehdr.e_ident[EI_MAG2] != 'L')
            return Executable { 0, E_MAG };
        else if(ehdr.e_ident[EI_MAG3] != 'F')
            return Executable { 0, E_MAG };
        else if(ehdr.e_ident[EI_CLASS] == ELFCLASS32)
            return Executable { 0, E_32 };
        else if(ehdr.e_ident[EI_DATA] != 1)
            return Executable { 0, E_BE };
        else if(ehdr.e_type != ET_EXEC)
            return Executable { 0, E_EXEC };
        else if(ehdr.e_machine != 0x3E)
            return Executable { 0, E_MACHINE };
        // ELF header is valid

        Elf64_Phdr* phdr = (Elf64_Phdr*)(buf + ehdr.e_phoff);
        bool isInterp = false;
        for(uint32_t i = 0; i < ehdr.e_phnum; i++, phdr++)
        {
            if(phdr->p_type == PT_INTERP)
                isInterp = true;
            if(phdr->p_type != PT_LOAD)
                continue;
            
            char* vaddr = (char*)phdr->p_vaddr;
            uint64_t paddr = (uint64_t)PMM::GetPages(NPAGES(phdr->p_memsz));
            Paging::MapPages(&addrspace, paddr, (uint64_t)vaddr, 7, NPAGES(phdr->p_memsz));
            if(VMM::MarkUsed(&addrspace, vaddr, NPAGES(phdr->p_memsz)))
                Kernel::Panic("Kernel panic: %s", "Could not reserve space for program"); // TODO: Exit safely
            char* fileOff = buf + phdr->p_offset;
            for(uint64_t j = 0; j < phdr->p_filesz; j++)
                *(vaddr + j) = *(fileOff + j);
            
            vaddr += phdr->p_filesz;
            for(uint64_t j = 0; j < phdr->p_memsz - phdr->p_filesz; j++)
                *(vaddr + j) = 0;
        }

        Paging::MapPages(&addrspace, (uint64_t)PMM::GetPages(NPAGES(5416)), 0x600000, 7, NPAGES(5416));
        
        if(isInterp)
        {
            VFS::Filesystem* tmpfs = VFS::GetFileSystem("tmpfs");
            VFS::Node out;
            tmpfs->lookup("/LD.SO", &out);
            size_t count;
            tmpfs->size(&out, &count);
            void* buf = malloc(count);
            tmpfs->read(&out, buf, &count, 0);

            return ParseELFExec(buf, addrspace, (void*)ehdr.e_entry);
        }

        return Executable {
            (void(*)())ehdr.e_entry, 0, interpAddr
        };
    }
}