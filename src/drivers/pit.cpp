#include <drivers/pit.hpp>
#include <drivers/terminal.hpp>
#include <drivers/ports.hpp>
#include <cpu/interrupt/irq.hpp>
#include <drivers/pic.hpp>

constexpr int PIT_IRQ = 0;

static int count = 0;

void PITHandler(ISRXFrame*)
{
    count++;
    if(count % 100 == 0)
    {
        Terminal::Print("\nTimer");
        count = 0;
    }
}

namespace PIT
{
    void Init(uint16_t rate)
    {
        InstallIRQHandler(PIT_IRQ, PITHandler);
        Port<uint8_t> PITData0(0x40);
        Port<uint8_t> PITCtrl(0x43);

        asm volatile("cli");
        PITCtrl.Write(0x34);
        PITData0.Write(rate & 0xFF);
        PITData0.Write((rate & 0xFF00) >> 8);
        asm volatile("sti");
    }

    void Disable()
    {
        PIC::Mask(PIT_IRQ);
    }
}