#include <cpu/interrupt/irq.hpp>
#include <drivers/pic.hpp>

constexpr int IRQ_OFFSET = 32;

InterruptHandler handlers[16];

extern "C" void IRQHandler(ISRXFrame* frame)
{
    InterruptHandler handler = handlers[frame->BaseFrame.vector - IRQ_OFFSET];
    if(handler)
        handler(frame);

    PIC::SendEOI(frame->BaseFrame.vector - IRQ_OFFSET);
}


void InstallIRQHandler(unsigned int irq, InterruptHandler handler)
{
    handlers[irq] = handler;

    PIC::Unmask(irq);
}