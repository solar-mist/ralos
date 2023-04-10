void _start()
{
    const char* path = "/test";
    int fd;
    const char* data = "hello";
    asm volatile("mov $0, %%rax; mov %1, %%rdi; mov $0, %%rsi; int $0x69; mov %%rax, %0" : "=m"(fd) : "m"(path));
    asm volatile("mov $2, %%rax; mov %0, %%rdi; mov %1, %%rsi; mov $5, %%rdx; int $0x69" :: "m"(fd), "m"(data));
    for(;;);
}