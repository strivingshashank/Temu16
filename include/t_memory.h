#ifndef T_MEMORY_INCLUDED
#define T_MEMORY_INCLUDED

#include "t_types.h"

#define SEGMENT_SIZE_KB 64
#define NULL_PTR ((void *) (0))
#define HEAP_SIZE 16384 /* 16 kib */

void heap_init(void);
void *heap_alloc(size_t requested_blocks);
void heap_free(void *heap_ptr, size_t allocated_blocks);

void memory_copy_far(size_t source_segment, size_t source_offset, size_t destination_segment, size_t destination_offset, size_t bytes);
void memory_copy(void *source, void *destination, size_t length);

bit16_t memory_get_size(void);
bit16_t heap_get_used_bytes(void);

#endif

