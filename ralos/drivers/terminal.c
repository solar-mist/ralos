#include <drivers/terminal.h>
#include <boot/limine.h>
#include <stdarg.h>
#include <stddef.h>

static volatile struct limine_terminal_request terminal_request = {
    .id = LIMINE_TERMINAL_REQUEST,
    .revision = 0
};

size_t strlen(const char* data)
{
    size_t i = 0;
    while(data[i])
        i++;
    return i;
}

int itoa(int value, char *sp, int radix)
{
    char tmp[16];
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);    
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix;
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign) 
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}

void t_write(const char* data, int size)
{
    terminal_request.response->write(terminal_request.response->terminals[0], data, size);
}

void t_print(const char* data)
{
    terminal_request.response->write(terminal_request.response->terminals[0], data, strlen(data));
}

void t_putchar(char c)
{
    char buf[1] = { c };
    t_write(buf, 1);
}

void t_printf(const char* format, ...)
{
    va_list arg;
    va_start(arg, format);
    int i = 0;
    while(format[i])
    {
        if (format[i] == '%')
        {
            if (format[++i] == '%')
                t_putchar(format[i++]);
            else if (format[i] == 'd' || format[i] == 'i')
            {
                char buf[16];
                char* str = buf;
                int n = va_arg(arg, int);
                int count = itoa(n, buf, 10);
                while(count--)
                     t_putchar(*str++);
                i++;
            }
            else if(format[i] == 'x')
            {
                char buf[16] = "0x";
                int n = va_arg(arg, int);
                itoa(n, buf + 2, 16);
                t_print(buf);
                i++;
            }
            else if (format[i] == 'c')
            {
                int ch_int = va_arg(arg, int);
                char ch = (char)ch_int;
                t_putchar(ch);
                i++;
            }
            else if (format[i] == 's')
            {
                char* str = va_arg(arg, char*);
                t_print(str);
                i++;
            }
        }
        else
            t_putchar(format[i++]);
    }
    va_end(arg);
}