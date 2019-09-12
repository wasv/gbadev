-include res/rules.mak

LDFLAGS += -T res/gba_cart.ld

TARGET  := mygame
SOURCES  = src/main.c src/gfx/sprite.c res/gba_crt0.s
OBJS = $(patsubst %,build/%.o,$(SOURCES))

.PHONY: all
all : build/$(TARGET).gba

%.gba : %.elf
	@mkdir -p $(@D)
	$(OBJCOPY) -O binary $^ $@

%.elf : $(OBJS) $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o
	@mkdir -p $(@D)
	$(LD) $(LDFLAGS) -o $@ $^ -lgcc -lc


build/%.c.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

build/%.s.o: %.s
	@mkdir -p $(@D)
	$(AS) $(MODEL) -c $^ -o $@

clean:
	rm -rf build
