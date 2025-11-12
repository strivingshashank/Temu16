#include "t_io.h"
#include "t_memory.h"
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

#define DEFAULT_FG COLOR_LIGHT_GREY
#define DEFAULT_BG COLOR_BLACK

bit16_t get_key(void);
bit16_t get_key_blocking(void);
bit8_t read_char(void);
bit16_t read_dec(void);
void read_str(bit8_t *buffer, size_t max_length);
void write_char(bit8_t character);
void write_dec(bit16_t dec_value);
void write_hex(bit16_t hex_value);
void write_str(bit8_t *str);
void clear_screen(void);
void wait_for_char(bit8_t character);

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

  read_str(dec_str, sizeof(dec_str));
  return str_to_dec(dec_str);
}

void read_str(bit8_t *buffer, size_t max_length) {
  size_t index = 0;

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
  bit8_t *dec_str = dec_to_str(dec_value);

  if (dec_str == NULL_PTR) {
    return;
  }

  write_str(dec_str);
  heap_free(dec_str, sizeof(bit8_t));
}

void write_sdec(sbit16_t dec_value) {
  bit8_t *sdec_str = sdec_to_str(dec_value);

  if (sdec_str == NULL_PTR) {
    return;
  }

  write_str(sdec_str);
  heap_free(sdec_str, sizeof(bit8_t));
}

void write_hex(bit16_t hex_value) {
  bit8_t *hex_str = hex_to_str(hex_value);

  if (hex_str == NULL_PTR) {
    return;
  }
  
  write_str(hex_str);
  heap_free(hex_str, sizeof(bit8_t));
}

void write_str(bit8_t *str) {
  if (str == NULL_PTR) {
    return;
  }
  
  while (*str) {
    write_char(*str++);
  }
}

void clear_screen(void) {
  size_t index = 0;
  _cursor_set(0, 0, 0);

  while (index < VGA_PAGE_SIZE) {
    write_char(' ');
    index++;
  }

  _cursor_set(0, 0, 0);
}

void wait_for_char(bit8_t character) {
  while ((bit8_t)(get_key_blocking()) != character) {
    if ((bit8_t)(get_key_blocking()) == character) {
      break;
    }
  }
}