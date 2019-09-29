-include res/rules.mak

REV := 0
MAKER := 01
GAME := MYGM

CFLAGS += -g
LDFLAGS += -T res/gba_cart.ld

TARGET  := mygame
SOURCES  = src/main.c src/gfx/sprite.c res/gba_crt0.s
OBJS = $(patsubst %,build/%.o,$(SOURCES))

.PHONY: all
all : build/$(TARGET).gba

%.gba : %.elf
	@mkdir -p $(@D)
	$(OBJCOPY) -O binary $^ $@
	$(GBAFIX) $@ -t$(TARGET) -m$(MAKER) -c$(GAME) -r$(REV)

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
