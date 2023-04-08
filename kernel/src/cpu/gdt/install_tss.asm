[global install_tss]
install_tss:
    mov ax, (5 * 8) | 0
    ltr ax

    ret