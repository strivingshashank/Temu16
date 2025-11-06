#ifndef T_UTILS_INCLUDED
#define T_UTILS_INCLUDED

#include "t_types.h"

#define BITS_IN_BYTES(bytes) (bytes * 8)

// void memory_copy();
void memory_copy_far(bit16_t source_segment, bit16_t source_offset, bit16_t destination_segment, bit16_t destination_offset, bit16_t bytes);

bit8_t time_get_hours(void);
bit8_t time_get_minutes(void);
bit8_t time_get_seconds(void);

bit8_t ascii_to_dec(bit8_t ascii_value);
bit8_t dec_to_ascii(bit8_t dec_value);

// bit16_t k_getStringLength(char *string);
bool_t char_is_printable(bit8_t character);
bool_t string_compare(bit8_t *string1, bit8_t *string2);
bit16_t string_get_length(bit8_t *string);

#endif

