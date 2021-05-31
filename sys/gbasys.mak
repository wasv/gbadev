CROSS   := arm-none-eabi-
AS      := $(CROSS)as
AR      := $(CROSS)ar
CC      := $(CROSS)gcc
LD      := $(CROSS)ld
OBJCOPY := $(CROSS)objcopy
GBAFIX  := gbafix

GCC_VERSION = $(shell arm-none-eabi-gcc --version | grep -o '[0-9]*\.[0-9]*\.[0-9]*' | head -n1)

ARMINC 	 = /usr/arm-none-eabi/include
ARMLIB 	 = /usr/arm-none-eabi/lib
GCCLIB 	 = /usr/lib/gcc/arm-none-eabi/$(GCC_VERSION)

MODEL    = -mthumb-interwork -mthumb

CFLAGS  += $(MODEL) -mlong-calls
CFLAGS  += -MMD -MP -I $(ARMINC) -I $(GBASYS_DIR)/include

LDFLAGS += -T $(GBASYS_DIR)/res/gba_cart.ld
LDFLAGS += -nostartfiles -L $(ARMLIB) \
	  -L $(ARMLIB)/thumb \
	  -L $(GCCLIB)

OBJS += $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o

LIBGBASYS := $(O)/gbasys/gbasys.a

%.gba: %.elf
	@mkdir -p $(@D)
	$(OBJCOPY) -O binary $< $@
	$(GBAFIX) $@ -t$(TARGET) -m$(MAKER) -c$(GAME) -r$(REV)

%.elf: $(OBJS) $(LIBGBASYS)
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc -lc

$(O)/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(O)/%.s.o: %.s
	@mkdir -p $(@D)
	$(AS) $(MODEL) -c $< -o $@
