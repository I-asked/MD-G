GCC_PATH = 

ifdef DEBUG
	NO_ECHO :=
else
	NO_ECHO := @
endif

CC      = $(GCC_PATH)arm-none-eabi-gcc
LD      = $(GCC_PATH)arm-none-eabi-ld
OCPY    = $(GCC_PATH)arm-none-eabi-objcopy
ODUMP   = $(GCC_PATH)arm-none-eabi-objdump
SZ      = $(GCC_PATH)arm-none-eabi-size
MKDIR   = mkdir


CFLAGS +=   -nostdlib\
			-ffreestanding \
			-march=armv4t \
			-mtune=arm7tdmi \
			-mthumb-interwork -mthumb\
			-ffunction-sections \
			-Oz
			
#-fdata-sections

LDFLAGS += -T cxd2680.ld --just-symbols cxd2680.ld.sym 
SRCS = xfer.c 
INCLUDES += ./

TARGET_VERSION=S16

CFLAGS += $(foreach i,$(INCLUDES),-I$(i)) -DFW_${TARGET_VERSION}=1
OBJS = $(patsubst %.c,build/objs/%.o,$(SRCS))

all: build/lyrics.bin
	hexdump -v -e '"\\" "x" 1/1 "%02X"' build/lyrics.bin > hex_builds/lyrics_${TARGET_VERSION}.shex
	hexdump -v -e '1/1 "%02X"' build/lyrics.bin > hex_builds/lyrics_${TARGET_VERSION}.hex
	wc -c build/lyrics.bin
	cp build/lyrics.bin build/lyrics_${TARGET_VERSION}.bin

build:
	mkdir -p build

build/objs:
	mkdir -p build/objs/

build/objs/%.o: %.c build/objs
	@echo "Compiling $<"
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

build/lyrics.bin: build/lyrics.elf build/lyrics.lst
	$(OCPY) $< $@ -O binary
	$(SZ) $<

build/lyrics.lst: build/lyrics.elf build
	$(ODUMP) -D $< > $@

build/lyrics.elf: $(OBJS)
	@echo "Linking $@"
	$(LD) $^ $(LDFLAGS) -o $@

clean:
	rm -rf build/objs build/lyrics.*
