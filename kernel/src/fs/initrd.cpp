#include <fs/initrd.hpp>
#include <fs/vfs.hpp>
#include <kernel/modules.hpp>
#include <drivers/terminal.hpp>
#include <libk/mem.hpp>

namespace InitRd
{
    unsigned int GetSize(const char* sz)
    {

        unsigned int size = 0;
        unsigned int j;
        unsigned int count = 1;

        for (j = 11; j > 0; j--, count *= 8)
            size += ((sz[j - 1] - '0') * count);

        return size;

    }

    void Init()
    {
        ViperModule* mod = GetModule(1);
        char* start = (char*)mod->address;
        TARHeader* hdr = (TARHeader*)start;
        VFS::Filesystem* fs = VFS::GetFileSystem("tmpfs");
        while(!memcmp((uint8_t*)"ustar", (uint8_t*)hdr->magic, 5))
        {
            char name[100] = {'/'};
            strcpy(name + 1, hdr->name);
            fs->create(name);
            VFS::Node out;
            fs->lookup(name, &out);
            fs->write(&out, (char*)hdr + 512, GetSize(hdr->size));

            hdr = (TARHeader*) ((uintptr_t) hdr + ((GetSize(hdr->size) + 511) / 512 + 1) * 512);
        }
    }
}