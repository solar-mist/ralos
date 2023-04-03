#include <cpu/interrupt/isr.hpp>
#include <drivers/terminal.hpp>

constexpr const char* exceptions[] = {
    "Division by Zero #DE",
    "Debug #DB",
    "Non-maskable Interrupt",
    "Breakpoint #BP",
    "Overflow #OF",
    "Bound Range Exceeded #BR",
    "Invalid Opcode #UD",
    "Device Not Available #NM",
    "Double Fault #DF",
    "Coprocessor Segment Overrun",
    "Invalid TSS #TS",
    "Segment Not Present #NP",
    "Stack-Segment Fault #SS",
    "General Protection Fault #GP",
    "Page Fault #PF",
    "Reserved",
    "x87 Floating-Point Exception #MF",
    "Alignment Check #AC",
    "Machine Check #MC",
    "SIMD Floating-Point Exception #XM",
    "Virtualization Exception #VE",
    "Control Protection Exception #CP",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Hypervisor Injection Exception #HV",
    "VMM Communication Exception #VC",
    "Security Exception #SX",
    "Reserved"
};

extern "C" void ISRExceptionHandler(ISRXFrame* frame)
{
    Terminal::Printf(0xFF0000, "\nAn exception occurred: %d - %s", frame->BaseFrame.vector, exceptions[frame->BaseFrame.vector]);
    for(;;)
        asm volatile("cli; hlt");
}