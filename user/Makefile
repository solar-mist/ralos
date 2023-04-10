all:
	gcc -nostdlib test.c -o TEST.EXE -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -fno-pie -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -Wl,-Ttext 0x400000 -Wl,-no-pie -Wl,-static

	nasm -felf64 test2.asm
	ld -nostdlib test2.o -o TEST2.EXE