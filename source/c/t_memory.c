#include "t_memory.h"
#include "asm_bindings.h"

bit8_t heap_map[HEAP_BLOCK_COUNT];
bit16_t heap_index;

void heap_init(void) {
  // Setup heap-map
  for (heap_index = 0; heap_index < HEAP_BLOCK_COUNT; heap_index++) {
    heap_map[heap_index] = 0;
  }

  heap_index = 0;
}

hptr_t heap_alloc(void) {
  for (heap_index = 1; heap_index < HEAP_BLOCK_COUNT; heap_index++) {
    if (heap_map[heap_index] == 0) {
      heap_map[heap_index] = 1;
      return heap_index * HEAP_BLOCK_SIZE;
    }
  }

  // No memory available.
  return NULL;
}

void heap_free(hptr_t free_hptr) {
  free_hptr %= HEAP_BLOCK_SIZE;
  heap_map[free_hptr] = 0;
}

bit8_t heap_read8(hptr_t read_hptr, bit16_t index) {
  return _mem_read8(HEAP_SEGMENT, read_hptr + index);
}

bit16_t heap_read16(hptr_t read_hptr, bit16_t index) {
  return _mem_read16(HEAP_SEGMENT, read_hptr + index);
}

void heap_write8(hptr_t write_hptr, bit16_t index, bit8_t value) {
  _mem_write8(HEAP_SEGMENT, write_hptr + index, value);
}

void heap_write16(hptr_t write_hptr, bit16_t index, bit16_t value) {
  _mem_write16(HEAP_SEGMENT, write_hptr + index, value);
}

// void heap_dump(void) {
//   bit16_t used_block_count = 0;
//   bit16_t index = 0;

//   konsole_write_string("Heap map:\n");
//   konsole_write_string("Block\tStatus\n");
  
//   for (index = 0; index < HEAP_BLOCK_COUNT; index++) {
//     bit8_t blockStatus = heap_map[index];
//     (blockStatus) ? used_block_count++ : used_block_count;
//     konsole_write_char(index);
//     konsole_write_string("\t\t\t");
//     konsole_write_char(blockStatus);
//     konsole_write_char('\n');
//   }

//   konsole_write_string("Heap blocks used: ");
//   konsole_write_dec(used_block_count);
//   konsole_write_char('\n');
//   konsole_write_string("Heap blocks free: ");
//   konsole_write_dec(HEAP_BLOCK_COUNT - used_block_count);
//   konsole_write_char('\n');
// }

// void heap_dump(void) {
//   bit16_t used_count = 0;
//   bit16_t index;
  
//   konsole_write_string("Heap map:\n");

//   for (index = 0; index < HEAP_BLOCK_COUNT; index++) {
//     bit8_t state = heap_map[index];
//     konsole_write_char(state ? '1' : '0');
//     konsole_write_char(' ');

//     if (state) used_count++;

//     // new line every 32 blocks
//     if ((index + 1) % 32 == 0) {
//       konsole_write_char('\n');

//       konsole_getKey_blocking();
//     }
//   }

//   konsole_write_string("\nStats:\n");
//   konsole_write_string("Total blocks: ");
//   konsole_write_dec(HEAP_BLOCK_COUNT);
//   konsole_write_char('\n');

//   konsole_write_string("Used blocks : ");
//   konsole_write_dec(used_count);
//   konsole_write_char('\n');

//   konsole_write_string("Free blocks : ");
//   konsole_write_dec(HEAP_BLOCK_COUNT - used_count);
//   konsole_write_char('\n');
// }

