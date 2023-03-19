[section .bss]
stack_bottom:
    resb 0x1000
[global stack_top]
stack_top:

[section .text]
[global _start]
_start:
    mov rsp, stack_top

    [extern KMain]
    call KMain
    jmp $