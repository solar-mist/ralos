#include <kernel/user/syscall.hpp>
#include <drivers/ui/terminal.hpp>
#include <kernel/sched/scheduler.hpp>

extern "C" void SystemCallHandler(ISRXFrame* frame)
{
    switch(frame->GeneralRegisters.rax)
    {
        case 0x0:
            Terminal::PutString((char*)frame->GeneralRegisters.rdi, frame->GeneralRegisters.rsi, 0xFF00FF);
            break;
        case 0x1:
            Scheduler::EndCurrentTask(frame);
            break;
    }
}