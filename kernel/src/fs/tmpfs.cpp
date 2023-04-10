#include <fs/tmpfs.hpp>
#include <mm/pmm.hpp>
#include <mm/kheap.hpp>
#include <libk/mem.hpp>
#include <stdint.h>
#include <drivers/terminal.hpp>

namespace TmpFS
{
    VFS::Filesystem fs;

    struct Node
    {
        const char* path;
        bool type : 1;
        uint64_t size : 63;
        void* data;
        VFS::Node vnode;
    };

    static Node* table;
    static uint32_t idx = 0;

    constexpr bool DIRECTORY = false;
    constexpr bool FILE      = true;

    int Create(const char* path)
    {
        table[idx++] = Node{ path, FILE, 0, nullptr, VFS::Node{ path, &fs } };

        char* ptr = (char*)PhysToVirt((uint64_t)PMM::GetPage());
        memcpy(ptr, (void*)path, strlen(path));

        int i;
        for(i = 0; ptr[i] != '/'; i++);
        ptr[i] = 0;

        for(uint32_t i = 0; i < idx; i++)
        {
            if(!strcmp(table[i].path, ptr))
            {
                void* old = table[i].data;
                table[i].data = malloc(table[i].size + strlen(path) + 1);
                memcpy(table[i].data, old, table[i].size);
                memcpy((char*)table[i].data + table[i].size, const_cast<char*>(path), strlen(path) + 1);
                table[i].size += strlen(path) + 1;
                free(old);
                return 0;
            }
        }
        return 0;
    }

    int Lookup(const char* path, VFS::Node* out)
    {
        for(uint32_t i = 0; i < idx; i++)
        {
            if(!strcmp(table[i].path, path))
            {
                *out = table[i].vnode;
                return 0;
            }
        }
        return 1;
    }

    int Read(VFS::Node* node, void* buffer, size_t* count)
    {
        for(uint32_t i = 0; i < idx; i++)
        {
            if(!strcmp(table[i].path, node->path))
            {
                if(table[i].type == FILE)
                {
                    if(*count > table[i].size)
                        *count = table[i].size;
                    memcpy(buffer, table[i].data, *count);
                    return 0;
                }
                else
                    return 1;
            }
        }
        return 1;
    }

    int Write(VFS::Node* node, const void* buffer, size_t count)
    {
        if(!strcmp(node->path, "/stdout"))
        {
            char* data = (char*)buffer;
            Terminal::PutString(data, count, 0x00FF00);
            return 0;
        }

        for(uint32_t i = 0; i < idx; i++)
        {
            if(!strcmp(table[i].path, node->path))
            {
                if(table[i].type == FILE)
                {
                    free(table[i].data);
                    table[i].data = malloc(count);
                    table[i].size = count;
                    memcpy(table[i].data, buffer, count);
                    return 0;
                }
                else
                    return 1;
            }
        }
        return 1;
    }

    int Append(VFS::Node* node, const void* buffer, size_t count)
    {
        for(uint32_t i = 0; i < idx; i++)
        {
            if(!strcmp(table[i].path, node->path))
            {
                if(table[i].type == FILE)
                {
                    void* old = table[i].data;
                    size_t oldSize = table[i].size;
                    table[i].data = malloc(oldSize + count);
                    memcpy(table[i].data, old, oldSize);
                    free(old);

                    memcpy((char*)table[i].data + oldSize, buffer, count);
                    return 0;
                }
                else
                    return 1;
            }
        }
        return 1;
    }

    int ReadDir(VFS::Node* node, char** files, size_t* count)
    {
        for(uint32_t i = 0; i < idx; i++)
        {
            if(!strcmp(table[i].path, node->path))
            {
                if(table[i].type != DIRECTORY)
                    return 1;
                if(table[i].size == 0 || table[i].data == nullptr)
                {
                    *count = 0;
                    return 0;
                }
                char* data = (char*)table[i].data;
                int j = 0;
                int k = 0;
                while(j < table[i].size)
                {
                    data += j;
                    char* path = (char*)malloc(strlen(data) + 1);
                    memcpy(path, data, strlen(data) + 1);
                    j += strlen(data) + 1;
                    files[k++] = path;
                }
                *count = k;
                return 0;
            }
        }
        return 1;
    }

    void Init()
    {
        table = (Node*)PhysToVirt((uint64_t)PMM::GetPages(4));
        table[idx++] = Node{ "", DIRECTORY, 0, nullptr, VFS::Node{ "", &fs } };
        table[idx++] = Node{ "/stdout", FILE, 0, nullptr, VFS::Node{ "/stdout", &fs } };
        fs = VFS::Filesystem{ "tmpfs", &table[0].vnode, Create, Lookup, Read, Write, Append, ReadDir };
        VFS::RegisterFileSystem(&fs);
    }
}