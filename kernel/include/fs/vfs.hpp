#ifndef FS_VFS_HPP
#define FS_VFS_HPP
#include <stddef.h>

namespace VFS
{
    struct Filesystem;
    struct Node;

    // File operations
    using Create = int(*)(const char* path);
    using Open = int(*)(const char* path, Node* out);
    using Read = int(*)(Node* node, void* buffer, size_t* count);
    using Write = int(*)(Node* node, const void* buffer, size_t count);
    using Append = int(*)(Node* node, const void* buffer, size_t count);

    // Directory operations
    using ReadDir = int(*)(Node* node, char** files, size_t* count);

    struct Node
    {
        const char* path;
        Filesystem* fs;
    };

    struct Filesystem
    {
        const char* name;
        Node* root;

        Create create;
        Open open;
        Read read;
        Write write;
        Append append;
        ReadDir readDir;

        Filesystem* next;
    };

    void RegisterFileSystem(Filesystem* fs);
    Filesystem* GetFileSystem(const char* name);
}

#endif