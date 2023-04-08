#include <cpu/interrupt/isr.hpp>
#include <drivers/terminal.hpp>

extern "C" void SystemCallHandler(InterruptFrame* frame)
{
    switch(frame->GeneralRegisters.rax)
    {
        case 0:
            Terminal::PutString((char*)frame->GeneralRegisters.rdi, frame->GeneralRegisters.rsi, 0x00FF00);
            break;
        default:
            break;
    }
}