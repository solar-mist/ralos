#include <cpu/interrupt/irq.hpp>
#include <drivers/pic.hpp>

static InterruptHandler handlers[16];

extern "C" void IRQHandler(InterruptFrame* frame)
{
    if(handlers[frame->BaseFrame.vector - 32])
        handlers[frame->BaseFrame.vector - 32](frame);

    PIC::SendEOI(frame->BaseFrame.vector - 32);
}

void RegisterIRQHandler(uint8_t irq, InterruptHandler handler)
{
    handlers[irq] = handler;
    PIC::Unmask(irq);
}