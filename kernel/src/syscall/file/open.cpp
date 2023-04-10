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
    uint32_t fd = proc->fdNum;
    proc->fdTable[proc->fdNum++] = ProcFile{ out, 0, flags };
    return fd;
}