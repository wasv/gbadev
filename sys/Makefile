O := build

CSRC  = $(shell find src/ -name "*.c")
SSRC  = $(shell find src/ -name "*.s")
OBJS  = $(patsubst %,$(O)/%.o,$(CSRC) $(SSRC))

GBASYS_DIR=.
include gbasys.mak

.SECONDARY:

.PHONY: all clean
all: $(O)/gbasys.a

$(O)/gbasys.a: $(OBJS)
	@mkdir -p $(@D)
	$(AR) -r $@ $^

clean:
	rm -rf $(O)

-include $(patsubst %,$(O)/%.d,$(CSRC))
