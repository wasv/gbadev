# If you're running a different distro you'll likely have
# to change the GCC_VERSION variable. It can be found by running
# arm-none-eabi-gcc --version
GCC_VERSION = $(shell arm-none-eabi-gcc --version | grep -o '[0-9]*\.[0-9]*\.[0-9]*' | head -n1)

CROSS   := arm-none-eabi-
AS      := $(CROSS)as
CC      := $(CROSS)gcc
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy

ARMINC = /usr/arm-none-eabi/include
ARMLIB = /usr/arm-none-eabi/lib
GCCLIB = /usr/lib/gcc/arm-none-eabi/$(GCC_VERSION)

CFLAGS   += -Wall -Werror -pedantic -Wextra

MODEL    = -mthumb-interwork -mthumb

CFLAGS  += $(MODEL) -mlong-calls
CFLAGS  += -MMD -MP -I $(ARMINC) -I src/
CFLAGS 	+= -O3

LDFLAGS += -nostartfiles -L $(ARMLIB) \
	  -L $(ARMLIB)/thumb \
	  -L $(GCCLIB) \
	  -T gba_cart.ld

TARGET  := mygame
SOURCES  = src/main.c src/gfx/sprite.c
OBJS = $(SOURCES:.c=.o) 

all : $(TARGET).gba

%.gba : %.elf
	$(OBJCOPY) -O binary $^ $@

%.elf : $(OBJS) gba_crt0.o $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc -lc

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o: %.s
	$(AS) $(MODEL) -c $^ -o $@

clean:
	rm -f *.o *.elf
