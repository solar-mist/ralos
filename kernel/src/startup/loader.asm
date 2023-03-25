[extern KMain]
[global loader]
loader:
    call KMain
    jmp $ ; In case kernel returns unexpectedly