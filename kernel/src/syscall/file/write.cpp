#include <syscall/file.hpp>
#include <fs/vfs.hpp>
#include <proc/process.hpp>
#include <kernel/sched/scheduler.hpp>
#include <drivers/terminal.hpp>

int SysWrite(int fd, const void* buf, size_t count)
{
    Process* proc = Scheduler::CurrentProcess();
    ProcFile* file = &proc->fdTable[fd];

    VFS::Filesystem* fs = file->node->fs;

    if(file->flags & OpenMode::W)
        fs->write(file->node, buf, count);
    else if(file->flags & OpenMode::A)
        fs->append(file->node, buf, count);
    else
        return -1;

    return count;
}