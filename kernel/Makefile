CXX=x86_64-elf-g++
CXXFLAGS+=-std=c++20 -ffreestanding -fno-stack-protector -fno-stack-check -fno-lto -fno-pic -fno-pie -m64 -march=x86-64 -mabi=sysv -mno-80387 -mno-mmx -mno-sse -mno-sse2 -mno-red-zone -mcmodel=kernel -Iinclude

LD=x86_64-elf-ld
LDFLAGS=-nostdlib -static -melf_x86_64 -zmax-page-size=0x1000 -Tlink.ld -no-pie

AS=nasm
ASFLAGS=-felf64

CXXSRCS=$(shell find src -name '*.cpp')
ASSRCS=$(shell find src -name '*.asm')

OBJS=${CXXSRCS:.cpp=.o} ${ASSRCS:.asm=.o}

TARGET=kernel.elf

.PHONY: all viper.h

all: $(TARGET)

viper.h:
	curl -Lo include/$@ https://raw.githubusercontent.com/viper-org/viper-boot/master/include/viper.h

%.o: %.cpp viper.h
	$(CXX) $(CXXFLAGS) $< -c -o $@

%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

clean:
	rm -rf $(OBJS) $(TARGET)