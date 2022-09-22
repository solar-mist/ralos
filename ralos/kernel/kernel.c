#include <cpu/idt.h>
#include <drivers/terminal.h>

void k_init()
{
    install_idt();
    *(char*)0x12345678910 = 'A';
}

void k_main()
{
    t_print("hej");
    while(1)
        __asm("hlt");
}