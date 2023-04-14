all:
	x86_64-elf-gcc -nostdlib -fPIC -fPIE test.c -c
	x86_64-elf-ld -nostdlib test.o -o TEST.EXE -Tlink.ld -zmax-page-size=0x1000 -L../libc -l:LIBC.SO