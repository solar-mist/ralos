#ifndef VIPER_HAF_FS_MODULE
#define VIPER_HAF_FS_MODULE 1

#include <stdint.h>

namespace haf
{
    class Module
    {
    public:
        Module(void* address, uint64_t size);

        void* getAddress();
        uint64_t getSize() const;

        static Module Get(uint32_t index);

    private:
        void* mAddress{ nullptr };
        uint64_t mSize{ 0 };
    };
}

#endif