#ifndef T_UTILS_INCLUDED
#define T_UTILS_INCLUDED

#include "t_types.h"

#define BITS_IN_BYTES(bytes) (bytes * 8)

bit8_t time_get_hours(void);
bit8_t time_get_minutes(void);
bit8_t time_get_seconds(void);
bit8_t time_get_century(void);
bit8_t time_get_year(void);
bit8_t time_get_month(void);
bit8_t time_get_date(void);
bit8_t *time_get_date_str(void);
bit8_t *time_get_time_str(void);

bit8_t bcd_to_dec(bit8_t bcd_value);
bit8_t ascii_to_dec(bit8_t ascii_value);
bit8_t dec_to_ascii(bit8_t dec_value);
bit8_t *dec_to_str(bit16_t dec_value);
bit8_t *sdec_to_str(sbit16_t sdec_value);
bit8_t *hex_to_str(bit16_t hex_value);
bit16_t str_to_dec(bit8_t *dec_str);
bool_t char_is_printable(bit8_t character);

bool_t str_cmp(bit8_t *str1, bit8_t *str2);
size_t str_get_size(bit8_t *str);

#endif

