LIBC=../libc
USER=../user

.PHONY: all
all:
	rm -rf tar
	mkdir -p tar

	$(MAKE) -C $(LIBC)
	cp $(LIBC)/LIBC.SO $(LIBC)/ld/LD.SO tar

	$(MAKE) -C $(USER)
	cp $(USER)/TEST.EXE tar

	cd tar; tar -cf ../INITRD.TAR *