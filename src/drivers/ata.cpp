#include <drivers/ata.hpp>
#include <drivers/ports.hpp>

namespace ATA
{
    void Read(uint32_t lba, uint8_t sectors, void* buffer)
    {
        Port<uint8_t> port = 0x1F6;
        port.Write((lba >> 24) | 0xE0);

        port = 0x1F2;
        port.Write(sectors);

        port = 0x1F3;
        port.Write(lba);

        port = 0x1F4;
        port.Write(lba >> 8);

        port = 0x1F5;
        port.Write(lba >> 16);

        port = 0x1F7;
        port.Write(0x20);

        while(!(port.Read() & 8))
            asm("pause");

        Port<uint16_t> readPort = 0x1F0;
        for(int i = 0; i < 256 * sectors; i++)
            ((uint16_t*)buffer)[i] = readPort.Read();
    }
}