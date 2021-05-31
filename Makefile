O := build

REV := 0
MAKER := 01
GAME := MYGM
TARGET := mygame

CSRC  = $(shell find src/ -name "*.c")
SSRC  = $(shell find src/ -name "*.s")
OBJS = $(patsubst %,$(O)/%.o,$(CSRC) $(SSRC))

CFLAGS 	+= -O3 -g
CFLAGS += -Wall -Werror -pedantic -Wextra

.DEFAULT_GOAL := all

GBASYS_DIR=sys
include sys/gbasys.mak

.SECONDARY:
.PHONY: all run clean

all: $(O)/$(TARGET).gba

run: $(O)/$(TARGET).gba
	vbam $<

$(LIBGBASYS): $(GBASYS_DIR)/Makefile
	$(MAKE) -C $(GBASYS_DIR) O=$(PWD)/$(@D)

clean:
	rm -rf $(O)

-include $(patsubst %,$(O)/%.d,$(CSRC))
