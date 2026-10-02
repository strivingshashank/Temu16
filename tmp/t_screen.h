#ifndef T_SCREEN_INCLUDED
#define T_SCREEN_INCLUDED

#include "t_types.h"

#define VGA_SEGMENT 0xb800
#define VGA_LINE_COUNT 25
#define VGA_COLUMN_COUNT 80
#define VGA_CELL_SIZE 2

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

// Scan codes (Key code)
#define KEY_ENTER 28
#define KEY_BACKSPACE 14

// Character codes (ASCII)
#define CHARACTER_BACKSPACE 0x08
#define CHARACTER_LINE_FEED 0x0a
#define CHARACTER_CARRIAGE_RETURN 0x0d
#define CHARACTER_HORIZONTAL_TAB 0x09

#define TAB_SIZE 2

void screen_init(void);
void screen_update(void);

bit8_t screen_get_display_page(void);
bit8_t screen_get_line(void);
bit8_t screen_get_column(void);
bit16_t screen_get_value_at(bit8_t line, bit8_t column);

void screen_set_cursor(bit8_t line_index, bit8_t column_index);

void screen_set_display_page(bit8_t display_page_index);
void screen_set_line(bit8_t line_index);
void screen_set_column(bit8_t column_index);

void screen_cursor_inc(void);
void screen_cursor_dec(void);
void screen_cursor_update(void);

bit8_t screen_read_char(void);
bit8_t screen_read_dec(void);
// void screen_read_string(bit8_t *input_string, bit16_t string_length);

void screen_write_char(bit8_t value);
void screen_write_dec(bit16_t value);
void screen_write_hex(bit16_t value);
void screen_write_string(bit8_t *string_ptr);
// void screen_write_hstring(hptr_t string_hptr);

void screen_write_newline(void);
void screen_write_backspace(void);

void screen_scroll(void);

void screen_clear(void);

bool_t screen_char_is_printable(bit8_t character);

#endif

