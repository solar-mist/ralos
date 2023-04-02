#include <kernel/modules.hpp>

static volatile struct ViperModuleRequest modules = {
    .id = VIPER_MODULE
};

ViperModule* GetModule(int moduleNumber)
{
    if(moduleNumber >= modules.response->count)
        return nullptr;
    return &modules.response->modules[moduleNumber];
}