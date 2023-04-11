#include <cpu/interrupt/isr.hpp>
#include <syscall/file.hpp>
#include <drivers/terminal.hpp>

extern "C" void SystemCallHandler(InterruptFrame* frame)
{
    switch(frame->GeneralRegisters.rax)
    {
        case 0: // open
        {
            const char* filename = (const char*)frame->GeneralRegisters.rdi;
            uint8_t flags = frame->GeneralRegisters.rsi;
            frame->GeneralRegisters.rax = SysOpen(filename, flags);
            break;
        }
        case 1: // read
        {
            int fd = frame->GeneralRegisters.rdi;
            char* buf = (char*)frame->GeneralRegisters.rsi;
            size_t count = frame->GeneralRegisters.rdx;
            frame->GeneralRegisters.rax = SysRead(fd, buf, count);
            break;
        }
        case 2: // write
        {
            int fd = frame->GeneralRegisters.rdi;
            const void* buf = (const void*)frame->GeneralRegisters.rsi;
            size_t count = frame->GeneralRegisters.rdx;
            frame->GeneralRegisters.rax = SysWrite(fd, buf, count);
            break;
        }
        case 3: // close
        {
            int fd = frame->GeneralRegisters.rdi;
            frame->GeneralRegisters.rax = SysClose(fd);
            break;
        }
        default:
            break;
    }
}