#ifndef T_MEMORY_INCLUDED
#define T_MEMORY_INCLUDED

#include "t_types.h"

#define NULL_PTR (((void *) 0))

void heap_init(void);
void *heap_alloc(bit16_t requested_blocks);
void heap_free(void *heap_ptr, bit16_t allocated_blocks);

#endif

