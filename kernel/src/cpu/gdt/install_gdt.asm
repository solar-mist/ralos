ENTRY_SIZE equ 0x8
CODE_SEL equ (1 * ENTRY_SIZE)
DATA_SEL equ (2 * ENTRY_SIZE)
SUPERVISOR equ 0x0

[section .text]
[global install_gdt]
install_gdt:
    cli
    lgdt [rdi]
    push CODE_SEL | SUPERVISOR
    lea rax, [rel .reload_segments]
    push rax
    retfq
.reload_segments:
    mov ax, DATA_SEL | SUPERVISOR
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret