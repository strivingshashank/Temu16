#ifndef T_MEMORY_INCLUDED
#define T_MEMORY_INCLUDED

#include "t_types.h"

#define HEAP_SEGMENT 0x3000
#define HEAP_BLOCK_SIZE 64
#define HEAP_BLOCK_COUNT 1024

void heap_init(void);

bit16_t heap_alloc(void);
void heap_free(hptr_t free_hptr);

bit8_t heap_read8(hptr_t read_hptr, bit16_t index);
bit16_t heap_read16(hptr_t read_hptr, bit16_t index);

void heap_write8(hptr_t read_hptr, bit16_t index, bit8_t value);
void heap_write16(hptr_t read_hptr, bit16_t index, bit16_t value);

// void heap_dump(void);

#endif

