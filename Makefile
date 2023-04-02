KERNEL=kernel/kernel.elf
KERNEL_FILES=$(shell find kernel -type f)
TARGET=viperOS.hdd

all: $(TARGET)

.PHONY: kernel
kernel:
	$(MAKE) -C kernel

viper-boot:
	git clone https://github.com/viper-org/viper-boot
	$(MAKE) -C viper-boot

ovmf:
	mkdir -p $@
	cd ovmf && curl -Lo OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && unzip OVMF-X64.zip

$(TARGET): kernel viper-boot
	$(MAKE) -C viper-boot
	cp viper-boot/BOOTX64.EFI viper-boot/build ./
	mkdir -p boot
	cp viper.cfg $(KERNEL) boot/
	./build boot $@

run: $(TARGET) ovmf
	qemu-system-x86_64 -bios ovmf/OVMF.fd -net none -M q35 -M smm=off -d int -drive file=$<,format=raw,if=ide,index=0,media=disk

clean:
	$(MAKE) -C kernel clean

distclean: clean
	rm -rf viper-boot boot build BOOTX64.EFI