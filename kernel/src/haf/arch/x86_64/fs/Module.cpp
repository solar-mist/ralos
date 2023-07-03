#include <haf/fs/Module.h>

#include <viper.h>

ViperModuleRequest moduleRequest = {
    .id = VIPER_MODULE,
    .response = nullptr
};

namespace haf
{
    Module::Module(void* address, uint64_t size)
        : mAddress(address)
        , mSize(size)
    {
    }

    void* Module::getAddress()
    {
        return mAddress;
    }

    uint64_t Module::getSize() const
    {
        return mSize;
    }

    Module Module::Get(uint32_t index)
    {
        if (!moduleRequest.response)
        {
            return Module(nullptr, 0);
        }
        else
        {
            if (index > moduleRequest.response->count)
            {
                return Module(nullptr, 0);
            }
            else
            {
                const ViperModule& bootloaderModule = moduleRequest.response->modules[index];
                
                return Module(bootloaderModule.address, bootloaderModule.size);
            }
        }
    }
}