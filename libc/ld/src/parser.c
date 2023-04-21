#include <parser.h>
#include <syscalls.h>
#include <mem.h>
#include <elf.h>

Elf64_Shdr* ELFShdr(Elf64_Ehdr* ehdr)
{
    return (Elf64_Shdr*)((char*)ehdr + ehdr->e_shoff);
}

Elf64_Shdr* ElfSection(Elf64_Ehdr* ehdr, int idx)
{
    return ELFShdr(ehdr)+idx;
}

char* ELFShStrTab(Elf64_Ehdr* ehdr)
{
    if(ehdr->e_shstrndx == SHN_UNDEF)
        return NULL;
    
    return (char*)ehdr + ElfSection(ehdr, ehdr->e_shstrndx)->sh_offset;
}

char* LookupShString(char* tab, int off)
{
    if(!tab)
        return NULL;
    
    return tab + off;
}

Elf64_Shdr* GetSection(Elf64_Ehdr* ehdr, const char* name)
{
    char* shstrtab = ELFShStrTab(ehdr);
    if(!shstrtab)
        return NULL;
    
    for(int i = 0; i < ehdr->e_shnum; i++)
    {
        Elf64_Shdr* s = ElfSection(ehdr, i);
        if(!s)
            continue;
        if(!strcmp(LookupShString(shstrtab, s->sh_name), name))
            return s;
    }
    return NULL;
}

void* ParseExec(void* buffer)
{
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;

    Elf64_Shdr* dynamic = GetSection(ehdr, ".dynamic");
    Elf64_Shdr* dynstr = GetSection(ehdr, ".dynstr");
    Elf64_Shdr* dynsym = GetSection(ehdr, ".dynsym");
    Elf64_Shdr* rela = GetSection(ehdr, ".rela.dyn");

    Elf64_Dyn* dyn = buffer + dynamic->sh_offset;
    for(int i = 0; i < dynamic->sh_size / dynamic->sh_entsize; i++, dyn = (Elf64_Dyn*)((char*)dyn + dynamic->sh_entsize))
    {
        if(dyn->d_tag == DT_NEEDED)
        {
            char* library = LookupShString(buffer + dynstr->sh_offset, dyn->d_un.d_val);
            char path[100] = { '/' };
            strcpy(path + 1, library);
            
            int fd = open(path, 1);
            size_t size = stat(fd);
            void* buf = (void*)mmap(NULL, size);
            read(fd, buf, size);
            close(fd);

            ParseLib(buf);
            Elf64_Ehdr* libEhdr = (Elf64_Ehdr*)buf;
            Elf64_Shdr* libDynsym = GetSection(libEhdr, ".dynsym");
            Elf64_Shdr* libDynstr = GetSection(libEhdr, ".dynstr");

            Elf64_Rela* reloc = buffer + rela->sh_offset;
            for(int j = 0; j < rela->sh_size / rela->sh_entsize; j++, reloc = (Elf64_Rela*)((char*)reloc + rela->sh_entsize))
            {
                Elf64_Sym* sym = buffer + dynsym->sh_offset + (ELF64_R_SYM(reloc->r_info) * rela->sh_entsize);
                char* name = LookupShString(buffer + dynstr->sh_offset, sym->st_name);
                
                Elf64_Sym* libSym = buf + libDynsym->sh_offset;
                for(int k = 0; k < libDynsym->sh_size / libDynsym->sh_entsize; k++, libSym = (Elf64_Sym*)((char*)libSym + libDynsym->sh_entsize))
                {
                    char* symName = LookupShString(buf + libDynstr->sh_offset, libSym->st_name);
                    if(!strcmp(name, symName))
                        *(uint64_t*)reloc->r_offset = libSym->st_value;
                }
            }
        }
    }

    return (void*)ehdr->e_entry;
}

void ParseLib(void* buffer)
{
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
    Elf64_Phdr* phdr = (Elf64_Phdr*)(buffer + ehdr->e_phoff);
    for(uint32_t i = 0; i < ehdr->e_phnum; i++, phdr++)
    {
        if(phdr->p_type != PT_LOAD)
            continue;
        
        char* vaddr = (char*)phdr->p_vaddr;

        (void)mmap(vaddr, phdr->p_memsz);

        char* fileOff = buffer + phdr->p_offset;
        for(uint64_t j = 0; j < phdr->p_filesz; j++)
            *(vaddr + j) = *(fileOff + j);

        vaddr += phdr->p_filesz;
        for(uint64_t j = 0; j < phdr->p_memsz - phdr->p_filesz; j++)
            *(vaddr + j) = 0;
    }
}