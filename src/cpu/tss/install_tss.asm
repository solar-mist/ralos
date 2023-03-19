[extern stack_top]
[global install_tss]
install_tss:
    mov ax, (3 * 8) | 0
    ltr ax

    ret