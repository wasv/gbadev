-include res/rules.mak

REV := 0
MAKER := 01
GAME := MYGM

CFLAGS += -g
LDFLAGS += -T res/gba_cart.ld

TARGET  := mygame
SOURCES  = res/gba_crt0.s $(shell find src/ -name "*.c")
OBJS = $(patsubst %,build/%.o,$(SOURCES))

.PHONY: all run clean
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

run: build/$(TARGET).gba
	vbam $^

clean:
	rm -rf build
