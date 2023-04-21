#include <syscalls.h>
#include <parser.h>
#include <elf.h>

void _start(int execFd)
{
    size_t execSize = stat(execFd);
    char* execBuffer = (char*)mmap(NULL, execSize);
    int code = read(execFd, execBuffer, execSize);
    close(execFd);
    void(*entry)() = ParseExec(execBuffer);

    entry();
}