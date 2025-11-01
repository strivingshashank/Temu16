#ifndef T_DISK_INCLUDED
#define T_DISK_INCLUDED

#include "t_types.h"

bit8_t disk_read(bit16_t linear_block_address, bit16_t destination_segement, bit16_t destination_offset, bit16_t sectors_to_read);

#endif