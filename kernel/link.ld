/* Tell the linker that we want an x86_64 binary */
OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

/* Set the entry point function to _start */
ENTRY(_start)

/* Describe the sections in our kernel */
SECTIONS
{
    /* Place the kernel at address 0xffffffff80000000*/
    . = 0xffffffff80000000;

    .text : {
        *(.text .text.*)
    }

    .rodata : {
        *(.rodata .rodata.*)
    }

    .data : {
        *(.data .data.*)
    }

    /* Ensure the .bss section is the final section in the binary */
    .bss : {
        *(COMMON)
        *(.bss .bss.*)
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.note .note.*)
    }
}