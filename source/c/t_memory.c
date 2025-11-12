#include "t_memory.h"
#include "asm_bindings.h"
#include "t_types.h"

#define HEAP_MAP_SIZE (HEAP_SIZE / 8)

void heap_init(void);
void *heap_alloc(size_t requested_bytes);
void heap_free(void *dead_pointer, size_t allocated_bytes);

void memory_copy_far(size_t source_segment, size_t source_offset, size_t destination_segment, size_t destination_offset, size_t bytes);
void memory_copy(void *source, void *destination, size_t length);

bit16_t memory_get_size(void);
bit16_t heap_get_used_bytes(void);

/* Heap bitmap */
bit8_t heap_map[HEAP_MAP_SIZE];
/* Heap array */
bit8_t heap[HEAP_SIZE];

void heap_init(void) {
  static bool_t heap_initialized = FALSE;
  size_t heap_map_index = 0;
  
  if (heap_initialized == TRUE) {
    return;
  }
  
  for (heap_map_index = 0; heap_map_index < HEAP_MAP_SIZE; heap_map_index++) {
    heap_map[heap_map_index] = 0;
  }
  
  heap_initialized = TRUE;
}

void *heap_alloc(size_t requested_bytes) {
  /* bits_per_byte = 8 */
  size_t free_bytes_count = 0;
  size_t free_bytes_start_index = 0;
  size_t byte_idx = 0;
  
  for (byte_idx = 0; byte_idx < HEAP_MAP_SIZE; byte_idx++) {
    size_t bit_idx = 0;

    for (bit_idx = 0; bit_idx < 8; bit_idx++) {
      bit8_t bit_value = heap_map[byte_idx] >> bit_idx;

      /* Is bytes free? */
      if ((bit_value & 1) == 0) {
        if (free_bytes_count == 0) {
          free_bytes_start_index = (byte_idx * 8) + bit_idx;
        }
        
        free_bytes_count++;
        
        /* Are requested bytes available? */
        if (requested_bytes <= free_bytes_count) {
          size_t count = 0;
          
          /* Reserve the bits in map */
          for (count = 0; count < requested_bytes; count++) {
            byte_idx = (free_bytes_start_index + count) / 8;
            bit_idx = (free_bytes_start_index + count) % 8;
            
            heap_map[byte_idx] |= (1 << bit_idx);
          }

          /* Return physical address */
          return (void *)(heap + free_bytes_start_index);
        }
      } else {
        free_bytes_count = 0;
      }
    }
  }

  return NULL_PTR;
}

void heap_free(void *dead_pointer, size_t allocated_bytes) {
  size_t bytes_start_index = ((bit8_t *)(dead_pointer)) - heap;
  size_t count = 0;

  for (count = 0; count < allocated_bytes; count++) {
    size_t byte_idx = (bytes_start_index + count) / 8;
    size_t bit_idx = (bytes_start_index + count) % 8;
    
    heap_map[byte_idx] &= ~(1 << bit_idx);
  }
}

void memory_copy_far(size_t source_segment, size_t source_offset, size_t destination_segment, size_t destination_offset, size_t bytes) {
  _mem_copy(source_segment, source_offset, destination_segment, destination_offset, bytes);
}

void memory_copy(void *source, void *destination, size_t length) {
  bit8_t *source8 = (bit8_t *)(source);
  bit8_t *destination8 = (bit8_t *)(destination);
  
  while (length--) {
    *destination8++ = *source8++;
  }
}

bit16_t memory_get_size(void) {
  return _mem_get_size();
}

bit16_t heap_get_used_bytes(void) {
  size_t byte_idx;
  size_t bit_idx;
  bit16_t used_bytes = 0;

  for (byte_idx = 0; byte_idx < HEAP_MAP_SIZE; byte_idx++) {
    bit8_t map_byte = heap_map[byte_idx];

    if (map_byte == 0) {
      continue;
    }

    for (bit_idx = 0; bit_idx < 8; bit_idx++) {
      if (map_byte & (1 << bit_idx)) {
        used_bytes++;
      }
    }
  }

  return used_bytes;
}
