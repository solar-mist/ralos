#include <syscall/file.hpp>
#include <fs/vfs.hpp>
#include <proc/process.hpp>
#include <kernel/sched/scheduler.hpp>

int SysRead(int fd, char* buf, size_t count)
{
    Process* proc = Scheduler::CurrentProcess();
    ProcFile* file = &proc->fdTable[fd];
    
    VFS::Filesystem* fs = file->node->fs;
    fs->read(file->node, buf, &count);

    return count;
}