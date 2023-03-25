override CXX=x86_64-elf-g++
override CXXFLAGS+=-std=c++20 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -fno-pie -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -Iinclude

override LD=x86_64-elf-ld
override LDFLAGS=-nostdlib -static -melf_x86_64 -zmax-page-size=0x1000 -Tlink.ld -no-pie

override AS=nasm
override ASFLAGS=-felf64

override CXXSRCS=$(shell find src -name '*.cpp')
override ASSRCS=$(shell find src -name '*.asm')

override OBJS=${CXXSRCS:.cpp=.o} ${ASSRCS:.asm=.o}

TARGET=viperOS.elf

all: $(TARGET)

include/limine.h:
	curl https://raw.githubusercontent.com/limine-bootloader/limine/trunk/limine.h -o include/$@

%.o: %.cpp limine.h
	$(CXX) $(CXXFLAGS) $< -c -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJS) $(TARGET)