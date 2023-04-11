#include <syscall/file.hpp>
#include <fs/vfs.hpp>
#include <proc/process.hpp>
#include <mm/kheap.hpp>
#include <kernel/sched/scheduler.hpp>

int SysClose(int fd)
{
    Process* proc = Scheduler::CurrentProcess();
    ProcFile* file = &proc->fdTable[fd];

    file->flags = 0;
    file->node = nullptr;

    return 0;
}