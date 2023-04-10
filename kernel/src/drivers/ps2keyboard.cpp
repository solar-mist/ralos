#include <drivers/ps2keyboard.hpp>
#include <drivers/pic.hpp>
#include <drivers/ports.hpp>
#include <drivers/terminal.hpp>
#include <cpu/interrupt/irq.hpp>
#include <cpu/interrupt/isr.hpp>

namespace PS2Keyboard
{
    constexpr uint8_t BACKSPACE = 0x0E;
    constexpr uint8_t ENTER_KEY = 0x1C;
    constexpr uint8_t ARROW_KEY = 0xE0;
    constexpr uint8_t SHIFT_KEY = 0x2A;

    bool shift_down;

    char scancode_map[][2] =
    {
        {0, 0}, {0, 0}, // No key, ESC
        {'1', '!'}, {'2', '"'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'}, {0, 0}, // Backspace
        {'\t'}, {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'}, {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'}, {'\n', '\n'}, {0, 0}, // Left-ctrl
        {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'}, {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '@'}, {'#', '~'},
        {0, 0}, {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'}, {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'}, {0, 0}, // Left-shift, Right-shift
        {'*', '*'}, {0, 0}, {' ', ' '}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, // Alt, Caps-lock, F1 - F10, Num-lock, Scroll-lock
        {0, 0}, {0, 0}, {0, 0},
        {'-', '-'},
        {0, 0}, {0, 0}, {0, 0}, {'+', '+'},
        {0, 0}, {0, 0}, {0, 0},
        {0, 0}, {0, 0}

    };

    void KeyboardHandler(InterruptFrame* frame)
    {
        Port<uint8_t> port = 0x60;
        uint8_t scancode = port.Read();
        switch(scancode)
        {
            case SHIFT_KEY:
            {
                shift_down = true;
                break;
            }
            case 0xAA:
            {
                shift_down = false;
                break;
            }
            case ENTER_KEY:
            { 
                Terminal::PutChar('\n', 0x00FF00);
                break;
            }
            case BACKSPACE:
                Terminal::Print("\b \b");
                break;
            default:
            {
                if(scancode_map[scancode][shift_down] && !(scancode & 0x80))
                {
                    Terminal::PutChar(scancode_map[scancode][shift_down], 0x00FF00);
                }
                break;
            }
        }
    }

    void Init()
    {
        RegisterIRQHandler(1, KeyboardHandler);
    }
}