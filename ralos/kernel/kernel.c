#include <kernel/kernel.h>
#include <mem/pmm.h>
#include <cpu/idt.h>
#include <drivers/keyboard.h>
#include <mem/kmalloc.h>
#include <drivers/terminal.h>

void k_init()
{
    install_pmm();
    install_idt();
    //install_keyboard();
}

void k_main()
{
    pmm_dump_stats();
    int* i = k_malloc(sizeof(int));
    pmm_dump_stats();
    k_free(i);
    pmm_dump_stats();

    while(1)
        __asm__("hlt");
}

void k_panic(const char* msg)
{
    t_print(msg);
    while(1)
        __asm__("cli; hlt");
}