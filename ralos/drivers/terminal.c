#include <drivers/terminal.h>
#include <stddef.h>
#include <boot/limine.h>

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

void t_write(const char* data, int size)
{
    terminal_request.response->write(terminal_request.response->terminals[0], data, size);
}

void t_print(const char* data)
{
    terminal_request.response->write(terminal_request.response->terminals[0], data, strlen(data));
}