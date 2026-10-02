#!/bin/bash
set -e

# ---------------------------
# Directories & files
# ---------------------------
SRC_DIR="source"
ASM_DIR="$SRC_DIR/asm"
C_DIR="$SRC_DIR/c"
BUILD_DIR="build"
BIN_DIR="bin"
OS_IMAGE="$BIN_DIR/TemuOS.img"

mkdir -p "$BUILD_DIR" "$BIN_DIR"

# ---------------------------
# Assemble bootloader (raw 512 bytes)
# ---------------------------
nasm -f bin "$ASM_DIR/bootloader.asm" -o "$BUILD_DIR/bootloader.bin"

# ---------------------------
# Assemble t_loader.asm (as86 object)
# ---------------------------
nasm -f as86 "$ASM_DIR/t_loader.asm" -o "$BUILD_DIR/t_loader.o"

# ---------------------------
# Assemble all other ASM files (excluding bootloader.asm and t_loader.asm)
# ---------------------------
for asm_file in "$ASM_DIR"/*.asm; do
    name=$(basename "$asm_file" .asm)
    if [[ "$name" == "bootloader" || "$name" == "t_loader" ]]; then
        continue
    fi
    echo "Assembling $name.asm..."
    nasm -f as86 "$asm_file" -o "$BUILD_DIR/$name.o"
done

# ---------------------------
# Compile all C files
# ---------------------------
for c_file in "$C_DIR"/*.c; do
    if [[ ! -f "$c_file" ]]; then
        continue  # skip if no files match
    fi
    name=$(basename "$c_file" .c)
    echo "Compiling $name.c..."
    bcc -c -ansi -0 -Iinclude "$c_file" -o "$BUILD_DIR/$name.o"
done

# ---------------------------
# Link kernel objects (t_loader.o first)
# ---------------------------
t_loader_OBJ="$BUILD_DIR/t_loader.o"
OTHER_OBJS=($(find "$BUILD_DIR" -maxdepth 1 -name "*.o" ! -name "t_loader.o" | sort))

echo "Linking kernel..."
ld86 -0 -d -o "$BUILD_DIR/kernel.bin" "$t_loader_OBJ" "${OTHER_OBJS[@]}" -M

# ---------------------------
# Create floppy image
# ---------------------------
if [[ "$1" == "--clean" ]]; then
    echo "Rebuilding new floppy image..."
    rm -f "$OS_IMAGE"
fi

# Create floppy image if not exists
if [[ ! -f "$OS_IMAGE" ]]; then
    echo "Creating new floppy image..."
    dd if=/dev/zero of="$OS_IMAGE" bs=512 count=2880 status=none
else
    echo "Reusing existing floppy image..."
fi

# Always refresh bootloader + kernel sectors
dd if="$BUILD_DIR/bootloader.bin" of="$OS_IMAGE" bs=512 count=1 conv=notrunc status=none
dd if="$BUILD_DIR/kernel.bin"     of="$OS_IMAGE" bs=512 seek=1 conv=notrunc status=none

# ---------------------------
# Launch QEMU
# ---------------------------
# qemu-system-i386 -drive file="$OS_IMAGE",format=raw,if=floppy -rtc clock=vm,base=localtime -device sb16,audiodev=pa -audiodev pa,id=pa -s -S
qemu-system-i386 -drive file="$OS_IMAGE",format=raw,if=floppy -rtc clock=vm,base=localtime -device sb16,audiodev=pa -audiodev pa,id=pa
# qemu-system-i386 -drive file="$OS_IMAGE",format=raw,if=floppy -rtc clock=vm,base=localtime -S -s

