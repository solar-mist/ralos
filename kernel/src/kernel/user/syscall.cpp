#include <cpu/interrupt/isr.hpp>
#include <drivers/terminal.hpp>
#include <kernel/sched/scheduler.hpp>

extern "C" void SystemCallHandler(InterruptFrame* frame)
{
    switch(frame->GeneralRegisters.rax)
    {
        case 0:
            Terminal::PutString((char*)frame->GeneralRegisters.rdi, frame->GeneralRegisters.rsi, 0x00FF00);
            break;
        case 1:
            Scheduler::EndCurrentProcess(frame);
            break;
        case 2:
            Terminal::Printf(0xFF00FF, "\n%d\n", Scheduler::RunningProcesses());
        default:
            break;
    }
}