ASM=nasm
CC=gcc

SRC_DIR=src
TOOLS_DIR=tools
BUILD_DIR=build

.PHONY: all floppy_image kernel bootloader clean always tools_fat

all: floppy_image tools_fat

#
# Floppy image
#
floppy_image: $(BUILD_DIR)/main_floppy.img

$(BUILD_DIR)/main_floppy.img: bootloader kernel
	# Create blank floppy image
	dd if=/dev/zero of=$@ bs=512 count=2880 status=none

	# Format FAT12 + install bootloader in one reliable step
	mformat -i $@ -f 1440 -v "NBOS" -B $(BUILD_DIR)/bootloader.bin ::

	# Copy files to the FAT filesystem
	mcopy -i $@ $(BUILD_DIR)/kernel.bin "::kernel.bin"
	mcopy -i $@ test.txt "::test.txt"

#
# Bootloader
#
bootloader: $(BUILD_DIR)/bootloader.bin

$(BUILD_DIR)/bootloader.bin: always
	$(ASM) $(SRC_DIR)/bootloader/boot.asm -f bin -o $@

#
# Kernel
#
kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	$(ASM) $(SRC_DIR)/kernel/main.asm -f bin -o $@

#
# Tools
#
tools_fat: $(BUILD_DIR)/tools/fat
$(BUILD_DIR)/tools/fat: always
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -g -o $@ $(TOOLS_DIR)/fat/fat.c

#
# Always
#
always:
	mkdir -p $(BUILD_DIR)

#
# Clean
#
clean:
	rm -rf $(BUILD_DIR)/*
