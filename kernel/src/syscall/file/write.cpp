#include <syscall/file.hpp>
#include <fs/vfs.hpp>
#include <proc/process.hpp>
#include <kernel/sched/scheduler.hpp>

int SysWrite(int fd, const void* buf, size_t count)
{
    Process* proc = Scheduler::CurrentProcess();
    ProcFile* file = &proc->fdTable[fd];
    
    VFS::Filesystem* fs = file->node->fs;
    fs->write(file->node, buf, count);

    return count;
}