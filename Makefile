KERNEL=kernel/kernel.elf
TARGET=ralos.hdd

.PHONY: all kernel

all: $(TARGET)

kernel:
	$(MAKE) -C $@

viper-boot:
	git clone https://github.com/viper-org/viper-boot
	$(MAKE) -C $@

ovmf:
	mkdir -p $@
	cd ovmf && curl -Lo OVMF-X64.zip https://efi.akeo.ie/OVMF/OVMF-X64.zip && unzip OVMF-X64.zip

$(TARGET): kernel viper-boot
	cp viper-boot/BOOTX64.EFI viper-boot/build ./
	mkdir -p boot
	cp viper.cfg $(KERNEL) font.sff boot/
	./build boot $@
	rm -rf boot

.PHONY: run clean distclean

run: $(TARGET) ovmf
	qemu-system-x86_64 -D qemu.out -monitor stdio -bios ovmf/OVMF.fd -net none -M q35 -M smm=off -d int -drive file=$<,format=raw,if=ide,index=0,media=disk -m 2G -smp cores=4

debug: $(TARGET) ovmf
	qemu-system-x86_64 -s -S -D qemu.out -monitor stdio -bios ovmf/OVMF.fd -net none -M q35 -M smm=off -d int -drive file=$<,format=raw,if=ide,index=0,media=disk -m 2G -smp cores=4

clean:
	$(MAKE) -C kernel clean

distclean: clean
	rm -rf viper-boot boot build BOOTX64.EFI