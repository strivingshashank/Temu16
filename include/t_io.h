#ifndef T_IO_INCLUDED
#define T_IO_INCLUDED

#include "t_types.h"

// Color codes
#define COLOR_BLACK 0x0
#define COLOR_BLUE 0x1
#define COLOR_GREEN 0x2
#define COLOR_CYAN 0x3
#define COLOR_RED 0x4
#define COLOR_MAGENTA 0x5
#define COLOR_BROWN 0x6
#define COLOR_LIGHT_GREY 0x7
#define COLOR_DARK_GREY 0x8
#define COLOR_LIGHT_BLUE 0x9
#define COLOR_LIGHT_GREEN 0xa 
#define COLOR_LIGHT_CYAN 0xb
#define COLOR_LIGHT_RED 0xc
#define COLOR_LIGHT_MAGENTA 0xd
#define COLOR_LIGHT_BROWN 0xe
#define COLOR_WHITE 0xf

bit16_t get_key(void);
bit16_t get_key_blocking(void);

bit8_t read_char(void); /* This is a blocking function */
bit16_t read_dec(void);
void read_string(bit8_t *buffer, bit16_t max_length);

void write_char(bit8_t character);
void write_dec(bit16_t dec_value);
void write_hex(bit16_t hex_value);
void write_string(bit8_t *string);

void clear_screen(void);

#endif

