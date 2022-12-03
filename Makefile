ASM=nasm
CC=gcc

SRC_DIR=src
Tools_DIR=tools
BUILD_DIR=build

.PHONY:all floppy_image kernel bootloader clean always tools_fat

all:floppy_image tools_fat
#
#Floppy image
#
floppy_image: $(BUILD_DIR)/main_floppy.img

$(BUILD_DIR)/main_floppy.img: bootloader kernel
	
	dd if=/dev/zero of=$(BUILD_DIR)/main_floppy.img bs=512 count=2880
	mkfs.fat -F 12 -n "NBOS" $(BUILD_DIR)/main_floppy.img
	dd if=$(BUILD_DIR)/bootloader.bin of=$(BUILD_DIR)/main_floppy.img conv=notrunc
	mcopy -i $(BUILD_DIR)/main_floppy.img $(BUILD_DIR)/kernel.bin "::kernel.bin"

#
#Bootloader
#
bootloader: $(BUILD_DIR)/bootloader.bin

$(BUILD_DIR)/bootloader.bin:always
	$(ASM) $(SRC_DIR)/bootloader/boot.asm -f bin -o $(BUILD_DIR)/bootloader.bin
#
#Kernel
#ker
kernel:$(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: always
	$(ASM) $(SRC_DIR)/kernel/main.asm -f bin -o $(BUILD_DIR)/kernel.bin

#
#Tools
#
tools_fat:$(BUILD_DIR)/tools/fat
$(BUILD_DIR)/tools/fat:always $(Tools_DIR)/fat/fat.c
	mkdir -p $(BUILD_DIR)/tools
	$(CC) -g -o $(BUILD_DIR)/tools/fat	$(Tools_DIR)/fat/fat.c
#
#always
# -p是递归创建多级目录
always: 
	mkdir -p $(BUILD_DIR)

#
#Clean
#
clean:
	rm -f $(BUILD_DIR)/*