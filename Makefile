KERNEL=kernel/kernel.elf
MODULES=$(shell find modules -type f)
TARGET=viperOS.hdd

all: $(TARGET)

.PHONY: kernel user libc
kernel:
	$(MAKE) -C kernel

user:
	$(MAKE) -C user
	cp user/TEST.EXE modules/

libc:
	$(MAKE) -C libc
	cp libc/LIBC.SO libc/ld/LD.SO modules/

viper-boot:
	git clone https://github.com/viper-org/viper-boot
	$(MAKE) -C viper-boot

ovmf:
	mkdir -p $@
	cd ovmf && curl -Lo OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && unzip OVMF-X64.zip

$(TARGET): kernel libc user viper-boot
	cp viper-boot/BOOTX64.EFI viper-boot/build ./
	mkdir -p boot
	cp viper.cfg $(KERNEL) $(MODULES) boot/
	./build boot $@
	rm -rf boot

run: $(TARGET) ovmf
	qemu-system-x86_64 -bios ovmf/OVMF.fd -net none -M q35 -M smm=off -d int -drive file=$<,format=raw,if=ide,index=0,media=disk -m 2G

clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C user clean
	$(MAKE) -C libc clean

distclean: clean
	rm -rf viper-boot boot build BOOTX64.EFI