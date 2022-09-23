#include <kernel/kernel.h>
#include <mem/pmm.h>
#include <cpu/idt.h>
#include <drivers/keyboard.h>
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
    physaddr_t addr = pmm_get_pages(600);
    pmm_dump_stats();
    pmm_free_pages(addr, 600);
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