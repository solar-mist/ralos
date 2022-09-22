#include <cpu/idt.h>
#include <drivers/terminal.h>

void k_init()
{
    install_idt();
    __asm__("int $1");
}

void k_main()
{
    t_print("hej");
    while(1)
        __asm("hlt");
}