#include "t_memory.h"
#include "asm_bindings.h"
#include "t_types.h"

#define HEAP_SIZE 16384 /* 16 kib */
#define HEAP_MAP_SIZE (HEAP_SIZE / 8)

void heap_init(void);
void *heap_alloc(bit16_t requested_bytes);
void heap_free(void *heap_ptr, bit16_t allocated_bytes);

/* Heap bitmap */
bit8_t heap_map[HEAP_MAP_SIZE];
/* Heap array */
bit8_t heap[HEAP_SIZE];

void heap_init(void) {
  static bool_t heap_initialized = FALSE;
  bit16_t heap_map_index = 0;
  
  if (heap_initialized == TRUE) {
    return;
  }
  
  for (heap_map_index = 0; heap_map_index < HEAP_MAP_SIZE; heap_map_index++) {
    heap_map[heap_map_index] = 0;
  }
  
  heap_initialized = TRUE;
}

void *heap_alloc(bit16_t requested_bytes) {
  /* bits_per_byte = 8 */
  bit16_t free_bytes_count = 0;
  bit16_t free_bytes_start_index = 0;
  bit16_t byte_index = 0;
  
  for (byte_index = 0; byte_index < HEAP_MAP_SIZE; byte_index++) {
    bit16_t bit_index = 0;

    for (bit_index = 0; bit_index < 8; bit_index++) {
      bit8_t bit_value = heap_map[byte_index] >> bit_index;

      /* Is bytes free? */
      if ((bit_value & 1) == 0) {
        if (free_bytes_count == 0) {
          free_bytes_start_index = (byte_index * 8) + bit_index;
        }
        
        free_bytes_count++;
        
        /* Are requested bytes available? */
        if (requested_bytes <= free_bytes_count) {
          bit16_t count = 0;
          
          /* Reserve the bits in map */
          for (count = 0; count < requested_bytes; count++) {
            byte_index = (free_bytes_start_index + count) / 8;
            bit_index = (free_bytes_start_index + count) % 8;
            
            heap_map[byte_index] |= (1 << bit_index);
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

void heap_free(void *heap_ptr, bit16_t allocated_bytes) {
  bit16_t bytes_start_index = ((bit8_t *)(heap_ptr)) - heap;
  bit16_t count = 0;

  for (count = 0; count < allocated_bytes; count++) {
    bit16_t byte_index = (bytes_start_index + count) / 8;
    bit8_t bit_index = (bytes_start_index + count) % 8;

    heap_map[byte_index] &= ~(1 << bit_index);
  }
}

