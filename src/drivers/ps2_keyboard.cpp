#include <drivers/ps2_keyboard.hpp>
#include <cpu/interrupt/irq.hpp>
#include <drivers/ui/terminal.hpp>
#include <drivers/ports.hpp>
#include <drivers/pic.hpp>

constexpr uint8_t PS2_KEYBOARD_IRQ = 1;
constexpr uint8_t PS2_KEYBOARD_DATA = 0x60;
constexpr uint8_t KEY_UP = 0x80;

void KeyboardHandler(ISRXFrame*)
{
    Port<uint8_t> data = PS2_KEYBOARD_DATA;
    uint8_t scancode = data.Read();
    if(!(scancode & KEY_UP))
        Terminal::Print("Keypress detected\n");
}

namespace PS2Keyboard
{
    void Init()
    {
        InstallIRQHandler(PS2_KEYBOARD_IRQ, KeyboardHandler);
    }
    
    void Disable()
    {
        PIC::Mask(PS2_KEYBOARD_IRQ);
    }
}