# =================================================================
# HexOS_! Build Script for Windows PowerShell
# =================================================================

$TOOLCHAIN = "i686-elf"
$CC = "$TOOLCHAIN-gcc"
$CFLAGS = "-m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pie -fno-pic -Wall -Wextra -O2 -I./kernel"

$AS = "$TOOLCHAIN-gcc"
$ASFLAGS = "-m32 -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-stack-protector -fno-pie -fno-pic -x assembler"

$LD = "$TOOLCHAIN-ld"
$LDFLAGS = "-m elf_i386 -T linker.ld --oformat elf32-i386"

$BUILD = "build"
$ISO_DIR = "iso\boot"
$GRUB_DIR = "iso\boot\grub"

# Helper functions
function Build-Object {
    param([string]$Source, [string]$Output, [string]$Flags)
    Write-Host "Compiling: $Source"
    if ($Source -like "*.c") {
        & $CC $Flags -c $Source -o $Output
    } else {
        & $AS $ASFLAGS -c $Source -o $Output
    }
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Build failed for $Source"
        exit 1
    }
}

function Link-Kernel {
    Write-Host "Linking kernel..."
    & $LD $LDFLAGS -o "$BUILD\kernel.bin" @args
    if ($LASTEXITCODE -ne 0) {
        Write-Host "ERROR: Linking failed"
        exit 1
    }
}

# Clean and create directories
Write-Host "Cleaning and preparing..."
Remove-Item -Force -ErrorAction SilentlyContinue -Recurse $BUILD
New-Item -ItemType Directory -Force $BUILD | Out-Null
New-Item -ItemType Directory -Force $ISO_DIR | Out-Null
New-Item -ItemType Directory -Force $GRUB_DIR | Out-Null

# Compile C files
Build-Object "kernel\kernel.c" "$BUILD\kernel.o" $CFLAGS
Build-Object "kernel\screen.c" "$BUILD\screen.o" $CFLAGS
Build-Object "kernel\keyboard.c" "$BUILD\keyboard.o" $CFLAGS
Build-Object "kernel\memory.c" "$BUILD\memory.o" $CFLAGS
Build-Object "kernel\string.c" "$BUILD\string.o" $CFLAGS
Build-Object "kernel\shell.c" "$BUILD\shell.o" $CFLAGS
Build-Object "kernel\idt.c" "$BUILD\idt.o" $CFLAGS
Build-Object "kernel\auth.c" "$BUILD\auth.o" $CFLAGS
Build-Object "kernel\security.c" "$BUILD\security.o" $CFLAGS

# Compile ASM files
Build-Object "boot\multiboot.asm" "$BUILD\multiboot.o" $ASFLAGS
Build-Object "boot\isr.asm" "$BUILD\isr.o" $ASFLAGS

# Linking
Link-Kernel "$BUILD\multiboot.o" "$BUILD\isr.o" "$BUILD\kernel.o" "$BUILD\screen.o" "$BUILD\keyboard.o" "$BUILD\memory.o" "$BUILD\string.o" "$BUILD\shell.o" "$BUILD\idt.o" "$BUILD\auth.o" "$BUILD\security.o"

# ISO
Write-Host "Creating ISO..."
Copy-Item "$BUILD\kernel.bin" "$ISO_DIR\kernel.bin" -Force
Copy-Item "boot\grub.cfg" "$GRUB_DIR\grub.cfg" -Force
& grub-mkrescue -o "iso\HexOS.iso" iso 2>&1 | Out-Null

if ($LASTEXITCODE -eq 0) {
    Write-Host ""
    Write-Host "ISO ready: iso\HexOS.iso"
    Write-Host "Run it with: qemu-system-i386 -cdrom iso\HexOS.iso"
    Write-Host ""
} else {
    Write-Host "ERROR: grub-mkrescue failed"
    exit 1
}

