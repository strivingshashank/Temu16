#ifndef T_BOARD_INCLUDED
#define T_BOARD_INCLUDED

#include "t_types.h"

// #define KBOARD_BUFFER_SIZE 16

bit16_t kboard_get_key(void);
bit16_t kboard_get_key_blocking(void);

#endif

