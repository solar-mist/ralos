#include <cpu/idt.h>
#include <drivers/ports.h>
#include <stdbool.h>

#define PIC_MASTER_CTRL 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CTRL  0xA0
#define PIC_SLAVE_DATA  0xA1

static idt_entry_t idt[256];
static bool vectors[256];
static idtr_t idtr;

extern void* isr_stub_table[32];
extern void* irq_stub_table[16];

void set_idt_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    idt_entry_t* desc = &idt[vector];
    
    desc->isr_low    = (uint64_t)isr & 0xFFFF;
    desc->kernel_cs  = 0x28;
    desc->ist        = 0;
    desc->attributes = flags;
    desc->isr_mid    = ((uint64_t)isr >> 16) & 0xFFFF;
    desc->isr_high   = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    desc->reserved   = 0;
}

void install_idt()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;

    for(uint8_t v = 0; v < 32; v++)
    {
        set_idt_descriptor(v, isr_stub_table[v], 0x8E);
        vectors[v] = true;
    }

    outb(PIC_MASTER_CTRL, 0x11);
    outb(PIC_SLAVE_CTRL, 0x11);
    outb(PIC_MASTER_DATA, 0x20);
    outb(PIC_SLAVE_DATA, 0x28);
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_SLAVE_DATA, 0x02);
    outb(PIC_MASTER_DATA, 0x01);
    outb(PIC_SLAVE_DATA, 0x01);
    outb(PIC_MASTER_DATA, 0xFF);
    outb(PIC_SLAVE_DATA, 0xFF);

    __asm__ volatile("nop");

    for(uint8_t v = 32; v < 48; v++)
    {
        set_idt_descriptor(v, irq_stub_table[v - 32], 0x8E);
        vectors[v] = true;
    }

    __asm__ volatile("lidt %0" : : "m"(idtr));
    __asm__ volatile("sti");
}