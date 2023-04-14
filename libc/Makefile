CC=x86_64-elf-gcc
CFLAGS=-nostdlib -fPIC -fPIE -ffreestanding

LD=x86_64-elf-ld
LDFLAGS=-nostdlib -shared -Tlink.ld -zmax-page-size=0x1000

CSRCS=$(shell find src -name '*.c' | grep -v ld)
OBJS=${CSRCS:.c=.o}

TARGET=LIBC.SO

.PHONY: all
all: $(TARGET)
	$(MAKE) -C ld

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)
	$(MAKE) -C ld clean