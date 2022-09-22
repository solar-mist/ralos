#include <drivers/keyboard.h>
#include <drivers/ports.h>
#include <drivers/terminal.h>
#include <cpu/irq.h>
#include <stdbool.h>

#define BACKSPACE 0x0E
#define ENTER_KEY 0x02
#define ARROW_KEY 0xE0

#define SHIFT_KEY 0x2A

bool shift_down;
uint8_t line_pos;
char current_line[115];

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

void keyboard_handler(registers_t* regs)
{
    (void)regs;
    uint8_t scancode = inb(0x60);
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
        case BACKSPACE:
        {
            t_print("\b \b");
            break;
        }
        case ARROW_KEY:
        {
            uint8_t arrow = inb(0x60);
            switch(arrow)
            {
                case 0x48:
                {
                    t_print("\x1B[1A");
                    return;
                }
                case 0x4B:
                {
                    t_print("\x1B[1D");
                    return;
                }
                case 0x50:
                {
                    t_print("\x1B[1B");
                    return;
                }
                case 0x4D:
                {
                    t_print("\x1B[1C");
                    return;
                }
                default:
                    return;
            }
        }
        case ENTER_KEY:
        { 
            t_putchar('\n');
            return;
        }
        default:
        {
            if(scancode_map[scancode] && !(scancode & 0x80))
            {
                t_putchar(scancode_map[scancode][shift_down]);
                current_line[line_pos] = scancode_map[scancode][shift_down];
                line_pos++;
            }
        }
    }
}

void install_keyboard()
{
    install_irq_handler(1, keyboard_handler);
    line_pos = 0;
    shift_down = false;
}