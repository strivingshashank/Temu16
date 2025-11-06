#include "t_io.h"
#include "t_utils.h"
#include "asm_bindings.h"

/* VGA Macros */
#define VGA_SEGMENT 0xb800
#define VGA_LINE_COUNT 25
#define VGA_COLUMN_COUNT 80
#define VGA_CELL_SIZE 2
#define VGA_PAGE_SIZE (VGA_LINE_COUNT * VGA_COLUMN_COUNT)

// Scan codes (Key code)
#define KEY_ENTER 28
#define KEY_BACKSPACE 14

// Character codes (ASCII)
#define CHARACTER_WHITESPACE 0x20
#define CHARACTER_BACKSPACE 0x08
#define CHARACTER_LINE_FEED 0x0a
#define CHARACTER_CARRIAGE_RETURN 0x0d
#define CHARACTER_HORIZONTAL_TAB 0x09

#define DEFAULT_FG COLOR_LIGHT_GREY
#define DEFAULT_BG COLOR_BLACK

bit16_t get_key(void);
bit16_t get_key_blocking(void);
bit8_t read_char(void);
bit16_t read_dec(void);
void read_string(bit8_t *buffer, bit16_t max_length);
void write_char(bit8_t character);
void write_dec(bit16_t dec_value);
void write_hex(bit16_t hex_value);
void write_string(bit8_t *string);
void clear_screen(void);

static bit8_t g_hexMap[17] = "0123456789abcdef";

/* Definitions */
bit16_t get_key(void) {
  return _kboard_get_key_buffer();
}

bit16_t get_key_blocking(void) {
  return _kboard_get_key_blocking();
}

bit8_t read_char(void) {
  while (TRUE) {
    bit8_t character = get_key_blocking();

    if (char_is_printable(character)) {
      write_char(character);
      return character;
    }

    if (character == '\r' || character == '\n') {
      write_char('\n');
      return '\n';
    }      
  }
}

bit16_t read_dec(void) {
  bit8_t dec_str[6];
  bit8_t index = 0;
  bit8_t string_length = 0;
  bit16_t dec_value = 0;
  
  read_string(dec_str, sizeof(dec_str));

  while ('0' <= dec_str[index] && dec_str[index] <= '9') {
    dec_value = dec_value * 10 + (dec_str[index] - '0');

    index++;
  }

  return dec_value;
}

void read_string(bit8_t *buffer, bit16_t max_length) {
  bit16_t index = 0;

  /* Clear buffer */
  while (index < (max_length - 1)) {
    buffer[index++] = '\0';
  }

  index = 0;
  
  while (index < (max_length - 1)) {
    bit8_t character = get_key_blocking();
    // bit8_t character = read_char();

    if (character == '\r' || character == '\n') {
      write_char('\n');
      return;
    }

    if (character == '\b') {
      if (0 < index) {
        index--;
        write_char('\b');
        write_char(' ');
        write_char('\b');
      }

      continue;
    }

    if (char_is_printable(character)) {
      buffer[index++] = character;
      write_char(character);
    }
  }

  buffer[index] = '\0';
}

void write_char(bit8_t character) {
  if (character == '\r' || character == '\n') {
    _screen_write_char('\r');
    _screen_write_char('\n');
    return;
  }
  
  _screen_write_char(character);
}

void write_dec(bit16_t dec_value) {
  bit8_t buffer[5];
  bit16_t BUFFER_SIZE = 5; // MAX_DEC = 65535 (length 5) + null terminator

  bit16_t index = 0; 

  if (dec_value == 0) {
    write_char('0');
    return;
  }
  
  while (0 < dec_value && index < BUFFER_SIZE) {
    buffer[index++] = '0' + dec_value % 10;
    dec_value /= 10;
  }

  while (index--) {
    _screen_write_char(buffer[index]);
  }
}

void write_hex(bit16_t hex_value) {
  bit8_t buffer[7];

  buffer[0] = '0';
  buffer[1] = 'x';
  buffer[2] = g_hexMap[(hex_value >> 12) & LIMIT_4];
  buffer[3] = g_hexMap[(hex_value >> 8) & LIMIT_4];
  buffer[4] = g_hexMap[(hex_value >> 4) & LIMIT_4];
  buffer[5] = g_hexMap[(hex_value) & LIMIT_4];
  buffer[6] = 0;

  write_string(buffer);
}

void write_string(bit8_t *string) {
  while (*string) {
    write_char(*string++);
  }
}

void clear_screen(void) {
  bit16_t index = 0;
  _cursor_set(0, 0, 0);

  while (index < VGA_PAGE_SIZE) {
    write_char(' ');
    index++;
  }

  _cursor_set(0, 0, 0);
}

