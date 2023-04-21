#include <syscall/file.hpp>
#include <fs/vfs.hpp>
#include <proc/process.hpp>
#include <kernel/sched/scheduler.hpp>

size_t SysStat(int fd)
{
    Process* proc = Scheduler::CurrentProcess();
    ProcFile* file = &proc->fdTable[fd];
    
    VFS::Filesystem* fs = file->node->fs;
    size_t count;
    fs->size(file->node, &count);

    return count;
}