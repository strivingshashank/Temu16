# PROG_FILE="$1"
# BASE_PROG_FILE="${PROG_FILE%.*}"
# PROG_OBJ="$PROG_DIR/$BASE_PROG_FILE.o"
# BUILD_DIR="build/"
# PROG_DIR="programs/"
# OS_IMAGE="bin/TemuOS.img"
# PROG_LOADER="$PROG_DIR/program_loader"

# nasm -f as86 "$PROG_LOADER.asm" -o "$BIN_DIR/$PROG_LOADER.bin"

# bcc -c -ansi -0 -Iinclude "$PROG_DIR/$PROG_FILE" -o "$PROG_OBJ"

# ld86 -0 -d -M -o "$PROG_DIR/$BASE_PROG_FILE.bin" "$BIN_DIR/$PROG_LOADER.bin" "$PROG_OBJ" 



# # nasm -f as86 programs/program_loader.asm -o programs/program_loader.bin

# # bcc -c -ansi -0 -Iinclude programs/program.c -o programs/program.o 

# # ld86 -0 -d -M -o programs/final_program.bin programs/program_loader.bin programs/program.o build/t_screen.o build/asm_routines.o build/t_kboard.o

# # # nasm -f bin demo_asm.asm -o demo_asm.bin

# # dd if=programs/final_program.bin of=bin/TemuOS.img bs=512 seek=10 conv=notrunc

#!/bin/bash
# -------------------------------------------------------
# TemuOS Program Builder (for a single program in development)
# Usage:
#   ./build_program.sh <program.c> [LBA]
#
# Example:
#   ./build_program.sh program.c 11
# -------------------------------------------------------

set -e

# --- CONFIGURATION ---
PROG_SRC="$1"
LBA_ADDR="${2:-11}"   # Default to LBA 11 if not provided

if [[ -z "$PROG_SRC" ]]; then
  echo "Usage: $0 <program_file.c> [LBA_address]"
  exit 1
fi

# --- PATHS ---
PROG_DIR="programs"
BUILD_DIR="build"
BIN_DIR="bin"
INCLUDE_DIR="include"
OS_IMAGE="$BIN_DIR/TemuOS.img"
PROG_LOADER="$PROG_DIR/program_loader.asm"

# --- DERIVED NAMES ---
BASE_NAME="$(basename "$PROG_SRC" .c)"
PROG_OBJ="$BUILD_DIR/$BASE_NAME.o"
PROG_BIN="$BIN_DIR/$BASE_NAME.bin"
PROG_LOADER_BIN="$BUILD_DIR/program_loader.bin"

# --- SYSTEM MODULES (auto-link with every program) ---
SYSTEM_OBJS=(
  "$BUILD_DIR/t_io.o"
  "$BUILD_DIR/asm_routines.o"
  "$BUILD_DIR/t_utils.o"
  "$BUILD_DIR/t_memory.o"
  "$BUILD_DIR/t_structs.o"
)

# --- CHECK ENVIRONMENT ---
[[ -f "$PROG_DIR/$PROG_SRC" ]] || { echo "❌ Source file not found: $PROG_DIR/$PROG_SRC"; exit 1; }
[[ -f "$OS_IMAGE" ]] || { echo "❌ OS image not found: $OS_IMAGE"; exit 1; }

mkdir -p "$BUILD_DIR"

# --- BUILD STEPS ---

echo "🧱 [1/4] Assembling program loader..."
nasm -f as86 "$PROG_LOADER" -o "$PROG_LOADER_BIN"

echo "💻 [2/4] Compiling program source: $PROG_SRC..."
bcc -c -ansi -0 -I"$INCLUDE_DIR" "$PROG_DIR/$PROG_SRC" -o "$PROG_OBJ"

echo "🔗 [3/4] Linking program with TemuOS runtime..."
ld86 -0 -d -M -o "$PROG_BIN" \
  "$PROG_LOADER_BIN" \
  "$PROG_OBJ" \
  "${SYSTEM_OBJS[@]}"

BASE_OFFSET=129
LBA_SECTOR_SIZE=128   # sectors per program (64 KB)
TARGET_LBA=$(( BASE_OFFSET + LBA_ADDR * LBA_SECTOR_SIZE ))

echo "💾 [4/4] Writing $BASE_NAME.bin to TemuOS.img at LBA $TARGET_LBA..."
dd if="$PROG_BIN" of="$OS_IMAGE" bs=512 seek="$TARGET_LBA" conv=notrunc status=none

echo ""
echo "✅ Build complete!"
echo "📦 Program:   $BASE_NAME"
echo "📀 Image:     $OS_IMAGE"
echo "📍 LBA:       $TARGET_LBA"
echo ""
echo "Run TemuOS and load this program with your shell command:"
echo "   load"
echo ""

