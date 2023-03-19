#include <drivers/pic.hpp>
#include <drivers/ports.hpp>

namespace PIC
{
    constexpr uint16_t PIC_MASTER_CTRL = 0x20;
    constexpr uint16_t PIC_SLAVE_CTRL  = 0xA0;
    constexpr uint16_t PIC_MASTER_DATA = 0x21;
    constexpr uint16_t PIC_SLAVE_DATA  = 0xA1;

    void Init()
    {
        Port<unsigned char> MasterCommand = PIC_MASTER_CTRL;
        Port<unsigned char> MasterData    = PIC_MASTER_DATA;
        Port<unsigned char> SlaveCommand  = PIC_SLAVE_CTRL;
        Port<unsigned char> SlaveData     = PIC_SLAVE_DATA;

        unsigned char a1, a2;
        a1 = MasterData.Read();
        a2 = SlaveData.Read();

        MasterCommand.Write(0x11);
        IOWait();
        SlaveCommand.Write(0x11);
        IOWait();

        MasterData.Write(0x20);
        IOWait();
        SlaveData.Write(0x28);
        IOWait();

        MasterData.Write(4);
        IOWait();
        SlaveData.Write(2);
        IOWait();

        MasterData.Write(1);
        IOWait();
        SlaveData.Write(1);
        IOWait();

        MasterData.Write(a1);
        SlaveData.Write(a2);
    }

    void SendEOI(uint8_t irq)
    {
        Port<unsigned char> MasterCommand = PIC_MASTER_CTRL;
        Port<unsigned char> SlaveCommand  = PIC_SLAVE_CTRL;
        if(irq >= 8)
            SlaveCommand.Write(0x20);
        MasterCommand.Write(0x20);
    }

    void Mask(uint8_t irq)
    {
        Port<uint8_t> port = (irq < 8) ? PIC_MASTER_DATA : PIC_SLAVE_DATA;
        if(irq >= 8)
            irq -= 8;
        
        uint8_t value = port.Read() | (1 << irq);
        port.Write(value);
    }

    void Unmask(uint8_t irq)
    {
        Port<uint8_t> port = (irq < 8) ? PIC_MASTER_DATA : PIC_SLAVE_DATA;
        if(irq >= 8)
            irq -= 8;
        
        uint8_t value = port.Read() & ~(1 << irq);
        port.Write(value);
    }
}