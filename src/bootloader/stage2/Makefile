BUILD_DIR ?= build/
ASM ?= nasm
CC16 ?= /home/baralekogyan/Downloads/ow/binl64/wcc
LD16 ?= /home/baralekogyan/Downloads/ow/binl64/wlink
WATCOM ?= /home/baralekogyan/Downloads/ow

CFLAGS16 ?= -0 -4 -d3 -s -wx -ms -zl -zq -bt=dos \
            -I$(WATCOM)/h \
            -I$(WATCOM)/h/dos

ASMFLAGS ?= -f obj

SOURCES_C = $(wildcard *.c)
SOURCES_ASM = $(wildcard *.asm)

OBJECTS_C = $(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))
OBJECTS_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))

.PHONY: all stage2 clean

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C) | always
	@echo "FILE $(abspath $(OBJECTS_ASM))" > $(BUILD_DIR)/stage2/link.rsp
	@echo "FILE $(abspath $(OBJECTS_C))" >> $(BUILD_DIR)/stage2/link.rsp
	$(LD16) NAME $@ \
	        OPTION MAP=$(BUILD_DIR)/stage2.map \
	        @$(BUILD_DIR)/stage2/link.rsp \
	        @linker.lnk

$(BUILD_DIR)/stage2/c/%.obj: %.c | always
	@mkdir -p $(dir $@)
	$(CC16) $(CFLAGS16) -fo=$@ $<

$(BUILD_DIR)/stage2/asm/%.obj: %.asm | always
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) -o $@ $<

always:
	@mkdir -p $(BUILD_DIR)/stage2/c
	@mkdir -p $(BUILD_DIR)/stage2/asm

clean:
	rm -rf $(BUILD_DIR)/stage2
