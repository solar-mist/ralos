[section .text]

[extern k_init]
[extern k_main]

[global _start]

_start:
    call k_init
    call k_main