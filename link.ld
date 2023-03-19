OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)
ENTRY(_start)

SECTIONS
{
	. = 2M;

	_kernel_start = .;

	/*. += 0xFFFF800000000000;*/

	.text : ALIGN(4K) /* : AT (ADDR (.text) - 0xFFFF800000000000) */
	{
		*(.text)
	}

	.rodata : ALIGN(4K)
	{
		*(.rodata)
	}
	

	.data : ALIGN(4K)
	{
		*(.data)
	}

	.bss : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

	_kernel_end = .;
}