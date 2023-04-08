[section .text]
[global _start]
_start:
    mov rax, 0
    mov rdi, msg
    mov rsi, 21
    int 0x69
    jmp $

[section .data]
msg: db "Hello Syscall World!", 10