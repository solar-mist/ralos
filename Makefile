CC=x86_64-elf-g++
CFLAGS=-c -ffreestanding -O2 -std=c++20 -Wall -Wextra -Iinclude -fno-rtti -fno-exceptions

AS=nasm
ASFLAGS=-felf64

LD=x86_64-elf-ld
LDFLAGS=-nostdlib -Tlink.ld -static -melf_x86_64 -zmax-page-size=0x1000

CXXSRCS=$(shell find src -name '*.cpp')
ASSRCS=$(shell find src -name '*.asm')

OBJS=${CXXSRCS:.cpp=.o} ${ASSRCS:.asm=.o}

KERNEL=kernel.elf
TARGET=viperOS.hdd

all: $(TARGET)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

viper-boot:
	git clone https://github.com/viper-org/viper-boot
	make -C viper-boot/efi

ovmf:
	mkdir -p ovmf
	cd ovmf && curl -o OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && unzip OVMF-X64.zip

$(KERNEL): $(OBJS)
	$(LD) $^ -o $@ $(LDFLAGS)

$(TARGET): viper-boot $(KERNEL)
	cp viper-boot/efi/build viper-boot/efi/BOOTX64.EFI . 
	./build $(KERNEL) $@

run: $(TARGET) ovmf
	qemu-system-x86_64 -bios ovmf/OVMF.fd -drive file=$<,format=raw,if=ide,index=0,media=disk -M q35 -net none -M smm=off -d int

clean:
	rm $(OBJS) $(KERNEL)

distclean:
	rm -rf viper-boot ovmf