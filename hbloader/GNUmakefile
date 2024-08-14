GCC_PREFIX = arm-none-eabi-

MODLDGEN ?= ../modldgen

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
					-Wl,-T,$(dir $(MAKEFILE_LIST))cxd2680.ld

LDLIBS += -lgcc
LDFLAGS += \
					 -static \
					 -nostdlib \
					 -ffreestanding \
					 $(CFLAGS)


OBJS = \
			 hbl.o S16.o \
			 atol.o atoll.o \
			 bsearch.o bzero.o \
			 qsort.o \
			 inline.o \
			 lz4.o \
			 memfile.o \
			 sprintf.o sscanf.o \
			 strtoumax.o strntoumax.o \
			 lrand48.o jrand48.o srand48.o mrand48.o nrand48.o \
			 strcasecmp.o strchr.o strcspn.o strlcat.o strlcpy.o strncasecmp.o strncmp.o strnlen.o strntoimax.o strpbrk.o strrchr.o strsep.o strspn.o strstr.o strtoimax.o strtok.o strtok_r.o strtoll.o strtoull.o \
			 memccpy.o memchr.o memmem.o memmove.o memrchr.o memswap.o \
			 vsprintf.o vsscanf.o tinyprintf.o
			 # memcmp.o memcpy.o memset.o
			 #atoi.o strcat.o strcmp.o strcpy.o strdup.o strlen.o strncat.o strncpy.o strndup.o strtol.o strtoul.o
			 #fgets.o \
			 #tinyprintf.o vprintf.o 
			 #asprintf.o
			 #vasprintf.o
			 #calloc.o \
			 #realloc.o \
			 #malloc.o \

DEPS = $(OBJS:.o=.d)

vpath %.c $(dir $(MAKEFILE_LIST))
vpath %.s $(dir $(MAKEFILE_LIST))

hbl.ld: hbl
	$(MODLDGEN) $< $@

hbl.bin: hbl
	$(OCPY) $< $@ -O binary

hbl: $(OBJS)

.PHONY: clean
clean:
	$(RM) -rf xfer.elf xfer.bin $(OBJS) $(DEPS)

-include $(DEPS)
