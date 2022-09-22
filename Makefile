override KERNEL := ralos.elf
override ISO    := ralos.iso

define DEFAULT_VAR =
    ifeq ($(origin $1),default)
        override $(1) := $(2)
    endif
    ifeq ($(origin $1),undefined)
        override $(1) := $(2)
    endif
endef

CC := x86_64-elf-gcc
LD := x86_64-elf-ld

CFLAGS ?= -g -O2 -pipe -Wall -Wextra

CPPFLAGS ?=

ASFLAGS ?= -F dwarf -g -f elf64

LDFLAGS ?=

override CFLAGS +=       \
    -std=c11             \
    -ffreestanding       \
    -fno-stack-protector \
    -fno-stack-check     \
    -fno-lto             \
    -fno-pie             \
    -fno-pic             \
    -m64                 \
    -march=x86-64        \
    -mabi=sysv           \
    -mno-80387           \
    -mno-mmx             \
    -mno-sse             \
    -mno-sse2            \
    -mno-red-zone        \
    -mcmodel=kernel      \
    -MMD                 \
    -I ralos/include

override LDFLAGS +=         \
    -nostdlib               \
    -static                 \
    -m elf_x86_64           \
    -z max-page-size=0x1000 \
    -T link.ld

ifeq ($(shell $(LD) --help 2>&1 | grep 'no-pie' >/dev/null 2>&1; echo $$?),0)
    override LDFLAGS += -no-pie
endif

override NASMFLAGS += \
    -f elf64

override CFILES := $(shell find ralos -type f -name '*.c')
override ASFILES := $(shell find ralos -type f -name '*.asm')
override OBJ := $(CFILES:.c=.o) $(ASFILES:.asm=.o)
override HEADER_DEPS := $(CFILES:.c=.d)

.PHONY: all
all: $(ISO)

$(ISO): $(KERNEL)
	cp $< isodir/
	xorriso -as mkisofs -b limine-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table \
        --efi-boot limine-cd-efi.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        isodir -o $@
	isodir/limine-deploy ralos.iso

$(KERNEL): $(OBJ)
	$(LD) $(OBJ) $(LDFLAGS) -o $@

-include $(HEADER_DEPS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.asm
	nasm $(ASFLAGS) $< -o $@

.PHONY: run clean
run: $(ISO)
	qemu-system-x86_64 -d int -M smm=off $<
clean:
	rm -rf $(KERNEL) $(OBJ) $(HEADER_DEPS)