#include <drivers/terminal.h>

void k_main()
{
    t_print("hej");
    while(1)
        __asm("hlt");
}