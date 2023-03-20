OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

SECTIONS
{
	. = 2M;

	_kernel_start = .;

	.text : ALIGN(4K)
	{
		*(.text .text.*)
	}

	. += CONSTANT(MAXPAGESIZE);

	.rodata : ALIGN(4K)
	{
		*(.rodata .rodata.*)
	}

	. += CONSTANT(MAXPAGESIZE);

	.data : ALIGN(4K)
	{
		*(.data .data.*)
	}

	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss .bss.*)
	}

	_kernel_end = .;
}