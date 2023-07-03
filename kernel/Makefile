override CXXC := x86_64-elf-g++
override LD := x86_64-elf-ld
override AS := nasm

override CXXFLAGS :=     \
	-std=gnu++20   		 \
	-ffreestanding 		 \
	-fno-stack-protector \
	-fno-stack-check 	 \
	-fno-exceptions		 \
	-fno-rtti			 \
	-fno-lto 			 \
	-fno-pic 			 \
	-fno-pie			 \
	-m64				 \
	-march=x86-64		 \
	-mno-80387			 \
	-mno-mmx			 \
	-mno-sse			 \
	-mno-sse2			 \
	-mno-red-zone		 \
	-mcmodel=kernel		 \
	-Wall				 \
	-Wextra				 \
	-Wpedantic			 \
	-ggdb3				 \
	-Og

override CPPFLAGS :=	 \
	-isystem include	 \
	-MMD				 \
	-MP					 \

override LDFLAGS :=		 \
	-nostdlib			 \
	-static              \
	-m elf_x86_64        \
	-T linker.ld         \
	-no-pie

override ASFLAGS :=      \
	-f elf64

override TARGET := kernel.elf

override CXXFILES := $(shell find src -name '*.cpp')
override NASMFILES := $(shell find src -name '*.asm')

override DIRS := $(shell echo $(shell find src/ -type d) | sed 's/src/build/g')
override HEADERS := $(shell echo ${CXXFILES:.cpp=.d} | sed 's/src/build/g')
override OBJS := $(shell echo ${CXXFILES:.cpp=.o} ${NASMFILES:.asm=.o} | sed 's/src/build/g')

.PHONY: all build

all: build $(TARGET)

viper.h:
	curl -Lo include/$@ https://raw.githubusercontent.com/viper-org/viper-boot/master/include/viper.h

build:
	for dir in $(DIRS) ; do \
		mkdir -p $$dir ; \
	done

-include $(HEADERS)

build/%.o: src/%.cpp viper.h
	$(CXXC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

build/%.o: src/%.asm
	$(AS) $(ASFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean distclean

clean:
	rm -rf build $(TARGET)