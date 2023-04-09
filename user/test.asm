[section .text]
[global _start]
_start:
    mov rcx, 0
begin:
    mov rax, 0
    lea rdi, [msg + rcx]
    mov rsi, 21
    sub rsi, rcx
    int 0x69
    add rcx, 1
    cmp rcx, 21
    jl begin
    jmp $

[section .data]
msg: db "Hello Syscall World!", 10