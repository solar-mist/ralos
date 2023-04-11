#include <syscall/file.hpp>
#include <fs/vfs.hpp>
#include <proc/process.hpp>
#include <mm/kheap.hpp>
#include <kernel/sched/scheduler.hpp>

int SysOpen(const char* filename, uint8_t flags)
{
    VFS::Filesystem* fs = VFS::GetFileSystem("tmpfs");
    VFS::Node* out = (VFS::Node*)malloc(sizeof(VFS::Node));
    fs->lookup(filename, out);
    Process* proc = Scheduler::CurrentProcess();

    uint32_t i;
    int fd;
    for(i = 3; i < FD_MAX; i++)
    {
        if(proc->fdTable[i].node == nullptr)
        {
            fd = i;
            break;
        }
    }

    proc->fdTable[fd] = ProcFile{ out, 0, flags };
    return i;
}