GCC_PREFIX ?= arm-none-eabi-

AS      = $(GCC_PREFIX)as
CC      = $(GCC_PREFIX)gcc
LD      = $(GCC_PREFIX)ld
OCPY    = $(GCC_PREFIX)objcopy
ODUMP   = $(GCC_PREFIX)objdump
SZ      = $(GCC_PREFIX)size

CFLAGS += \
		-march=armv4t \
		-mtune=arm7tdmi \
		-mthumb-interwork -mthumb \
		-ffunction-sections \
		-Oz \
		-isystem $(dir $(MAKEFILE_LIST))../include \
		-I$(dir $(MAKEFILE_LIST))templates \
		-DNO_UNISTD_H \
		-Wl,-T,$(dir $(MAKEFILE_LIST))../cxd2680.ld,--just-symbols,hbl.ld

LDLIBS += -lgcc
LDFLAGS += \
					 -static \
					 -nostdlib \
					 -ffreestanding \
					 $(CFLAGS)

PROJECT := hbl

TARGET_VERSION ?= S16

OBJS = lyrics_$(TARGET_VERSION).o lyrics.o meta_lrc.o util.o
DEPS = $(OBJS:.o=.d)

vpath %.c $(dir $(MAKEFILE_LIST))
vpath %.s $(dir $(MAKEFILE_LIST))

$(PROJECT).bin: $(PROJECT)
	$(OCPY) $< $@ -O binary

$(PROJECT): $(OBJS)

.PHONY: clean
clean:
	$(RM) -rf $(PROJECT) $(PROJECT).bin $(OBJS) $(DEPS)

-include $(DEPS)
