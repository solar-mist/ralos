[global JumpUsermode]
JumpUsermode:
    mov ax, (4 * 8) | 3
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    push (4 * 8) | 3
    push rsi
    push 0x202
    push (3 * 8) | 3
    push rdi
    iretq