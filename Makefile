KERNEL=kernel/viperOS.elf
KERNEL_FILES=$(shell find kernel -type f)
TARGET=viperOS.iso

all: $(TARGET)

.PHONY: kernel
kernel:
	$(MAKE) -C kernel

limine:
	git clone https://github.com/limine-bootloader/limine --branch=v4.x-branch-binary --depth=1
	$(MAKE) -C limine

$(TARGET): kernel limine
	$(MAKE) -C limine
	rm -rf isodir
	mkdir -p isodir
	cp $(KERNEL) limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-cd-efi.bin isodir/
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-cd-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		isodir -o $@
	limine/limine-deploy $@
	rm -rf isodir

run: $(TARGET)
	qemu-system-x86_64 -M q35 -hda $< -d int -M smm=off

clean:
	$(MAKE) -C kernel clean

distclean:
	rm -rf limine isodir