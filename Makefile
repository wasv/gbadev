TARGET  := mygame
SOURCES  = main.c
CFLAGS 	 = -O3
CFLAGS  += -mthumb -mthumb-interwork
LDFLAGS  = --specs=gba.specs
LDFLAGS += $(CFLAGS)

all : $(TARGET).gba

%.gba : %.elf
	arm-none-eabi-objcopy -O binary $^ $@

%.elf : $(SOURCES:.c=.o) gba_crt0.o
	arm-none-eabi-gcc $(LDFLAGS) -o $@ $(filter-out gba_crt0.o,$^)

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $^ -o $@

%.o: %.s
	arm-none-eabi-gcc $(CFLAGS) -c $^ -o $@

clean:
	rm -f *.o *.elf
