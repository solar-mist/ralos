#include <kernel/user/syscall.hpp>
#include <drivers/ui/terminal.hpp>

extern "C" void SystemCallHandler(ISRXFrame* frame)
{
    if(frame->GeneralRegisters.rax == 0x0)
    {
        Terminal::PutString((char*)frame->GeneralRegisters.rdi, frame->GeneralRegisters.rsi, 0xFF00FF);
    }
}