[section .text]
[global InstallGDT]
InstallGDT:
    cli
    lgdt [rdi]
    push (1 * 8) | 0
    lea rax, [rel .ReloadSegments]
    push rax
    retfq
.ReloadSegments:
    mov ax, (2 * 8) | 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret