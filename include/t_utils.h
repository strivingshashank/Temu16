#ifndef T_UTILS_INCLUDED
#define T_UTILS_INCLUDED

#include "t_types.h"

bit8_t time_get_hours(void);
bit8_t time_get_minutes(void);
bit8_t time_get_seconds(void);

bit8_t ascii_to_dec(bit8_t ascii_value);
bit8_t dec_to_ascii(bit8_t dec_value);

// bit16_t k_getStringLength(char *string);
bool_t string_compare(bit8_t *string1, bit8_t *string2);

#endif

