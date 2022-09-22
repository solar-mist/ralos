#include <cpu/irq.h>
#include <cpu/isr.h>
#include <drivers/ports.h>

#define PIC_MASTER_CTRL 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CTRL  0xA0
#define PIC_SLAVE_DATA  0xA1

void(*interupt_handlers[16])(registers_t*);

void irq_handler(registers_t* regs)
{
    void(*handler)(registers_t*);

    handler = interupt_handlers[regs->vector - 32];
    if (handler)
        handler(regs);
    
    if(regs->vector >= 40)
        outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

static uint16_t ocw1 = 0xFFFB;

void install_irq_handler(int irq, void(*handler)(registers_t*))
{
    interupt_handlers[irq] = handler;
    ocw1 &= (uint16_t)~((1 << irq));
    if (irq < 8) {
        outb(PIC_MASTER_DATA, (uint8_t)(ocw1 & 0xFF));
    }
    else {
        outb(PIC_SLAVE_DATA, (uint8_t)(ocw1 >> 8));
    }
}