#include <fs/vfs.hpp>
#include <libk/mem.hpp>

namespace VFS
{
    Filesystem* filesystems = nullptr;

    void RegisterFileSystem(Filesystem* fs)
    {
        fs->next = filesystems;
        filesystems = fs;
    }

    Filesystem* GetFileSystem(const char* name)
    {
        Filesystem* fs = filesystems;
        while(fs != nullptr)
        {
            if(!strcmp(name, fs->name))
                return fs;
        }
        return nullptr;
    }
}