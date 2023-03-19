#ifndef DRIVERS_ATA_HPP
#define DRIVERS_ATA_HPP
#include <stdint.h>

namespace ATA
{
    void Read(uint32_t lba, uint8_t sectors, void* buffer);
}

#endif