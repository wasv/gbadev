
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
	  -L $(GCCLIB)

.SECONDARY:
