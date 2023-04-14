#include <stdint.h>
#include <stddef.h>

#define PT_LOAD 1
typedef struct
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
} Elf64_Ehdr;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} Elf64_Phdr;


int open(const char* filename, unsigned char flags)
{
    int ret;
    asm volatile("mov $0, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %2, %%rdx; int $0x69" : "=a"(ret) : "m"(filename), "m"(flags));
    return ret;
}

int read(int fd, void* buf, size_t count)
{
    int ret;
    asm volatile("mov $1, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %3, %%rdx; int $0x69" : "=a"(ret) : "m"(fd), "m"(buf), "m"(count));
    return ret;
}

int write(int fd, const void* buf, size_t count)
{
    int ret;
    asm volatile("mov $2, %%rax; mov %1, %%rdi; mov %2, %%rsi; mov %3, %%rdx; int $0x69" : "=a"(ret) : "m"(fd), "m"(buf), "m"(count));
    return ret;
}

int close(int fd)
{
    int ret;
    asm volatile("mov $3, %%rax; mov %1, %%rdi; int $0x69" : "=a"(ret) : "m"(fd));
    return ret;
}

void _start(void* addr)
{
    int libcFd = open("/LIBC.SO", 1);
    char buffer[5416];
    int count = read(libcFd, buffer, 5416);
    close(libcFd);
    Elf64_Ehdr* ehdr = (Elf64_Ehdr*)buffer;
    Elf64_Phdr* phdr = (Elf64_Phdr*)(buffer + ehdr->e_phoff);
    for(uint32_t i = 0; i < ehdr->e_phnum; i++, phdr++)
    {
        if(phdr->p_type != PT_LOAD)
            continue;
        
        char* vaddr = (char*)phdr->p_vaddr;

        char* fileOff = buffer + phdr->p_offset;
        for(uint64_t j = 0; j < phdr->p_filesz; j++)
            *(vaddr + j) = *(fileOff + j);

        vaddr += phdr->p_filesz;
        for(uint64_t j = 0; j < phdr->p_memsz - phdr->p_filesz; j++)
            *(vaddr + j) = 0;
    }
    *(uint64_t*)(0x8002400) = 0x600000;

    void(*a)() = (void(*)())addr;
    a();
    for(;;)asm volatile("pause");
}