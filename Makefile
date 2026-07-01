# ============================================================
# HexOS_! — Makefile
# ============================================================

CC      = gcc
CFLAGS  = -m32 -ffreestanding -nostdlib -nostdinc -fno-builtin \
           -fno-stack-protector -fno-pie -fno-pic \
           -Wall -Wextra -O2 -I./kernel

AS      = nasm
ASFLAGS = -f elf32

LD      = ld
LDFLAGS = -m elf_i386 -T linker.ld --oformat elf32-i386

KERNEL_SRCS = \
    kernel/kernel.c \
    kernel/screen.c \
    kernel/keyboard.c \
    kernel/memory.c \
    kernel/string.c \
    kernel/shell.c \
    kernel/idt.c

BOOT_ASM = \
    boot/multiboot.asm \
    boot/isr.asm

BUILD   = build
ISO_DIR = iso/boot
GRUB_DIR = iso/boot/grub

KERNEL_OBJS = $(patsubst kernel/%.c,$(BUILD)/%.o,$(KERNEL_SRCS))
BOOT_OBJS   = $(patsubst boot/%.asm,$(BUILD)/%.o,$(BOOT_ASM))

.PHONY: all clean iso run

all: iso/HexOS.iso

# C files
$(BUILD)/%.o: kernel/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# ASM files
$(BUILD)/%.o: boot/%.asm | $(BUILD)
	$(AS) $(ASFLAGS) $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# Kernel ELF
$(BUILD)/kernel.bin: $(BOOT_OBJS) $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# ISO
iso/HexOS.iso: $(BUILD)/kernel.bin
	mkdir -p $(ISO_DIR) $(GRUB_DIR)
	cp $(BUILD)/kernel.bin $(ISO_DIR)/kernel.bin
	cp boot/grub.cfg $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $@ iso 2>/dev/null
	@echo ""
	@echo "  ✓ ISO pronta: iso/HexOS.iso"
	@echo "  Lancia con: qemu-system-i386 -cdrom iso/HexOS.iso"
	@echo ""

run: iso/HexOS.iso
	qemu-system-i386 -cdrom iso/HexOS.iso

clean:
	rm -rf $(BUILD) iso/HexOS.iso iso/boot
