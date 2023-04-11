#ifndef PROC_FD_HPP
#define PROC_FD_HPP
#include <fs/vfs.hpp>
#include <stddef.h>
#include <stdint.h>

constexpr uint32_t FD_MAX = 64;

enum OpenMode : uint8_t
{
    R=1,
    W=2,
    A=3,
};

struct ProcFile
{
    VFS::Node* node;
    size_t pos;
    uint8_t flags;
};

#endif