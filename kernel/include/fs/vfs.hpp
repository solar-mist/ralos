#ifndef FS_VFS_HPP
#define FS_VFS_HPP
#include <stddef.h>

namespace VFS
{
    struct Filesystem;
    struct Node;

    using Lookup = int(*)(const char* path, Node* out);

    // File operations
    using Create = int(*)(const char* path);
    using Read = int(*)(Node* node, void* buffer, size_t* count, int position);
    using Write = int(*)(Node* node, const void* buffer, size_t count);
    using Append = int(*)(Node* node, const void* buffer, size_t count);
    using Size = int(*)(Node* node, size_t* size);

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
        Lookup lookup;
        Read read;
        Write write;
        Append append;
        Size size;
        ReadDir readDir;

        Filesystem* next;
    };

    void RegisterFileSystem(Filesystem* fs);
    Filesystem* GetFileSystem(const char* name);
}

#endif