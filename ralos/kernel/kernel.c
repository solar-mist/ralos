#include <kernel/kernel.h>
#include <cpu/idt.h>
#include <drivers/keyboard.h>
#include <drivers/terminal.h>

void k_init()
{
    install_idt();
    install_keyboard();
}

void k_main()
{
    while(1)
        __asm__("hlt");
}

void k_panic(const char* msg)
{
    t_print(msg);
    while(1)
        __asm__("cli; hlt");
}