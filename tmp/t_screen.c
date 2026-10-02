#include "t_screen.h"
#include "t_kboard.h"
#include "t_utils.h"
#include "asm_bindings.h"

static screen_cursor_t screen_cursor;
static bit8_t hexMap[17] = "0123456789abcdef";

/* Definitions */
void screen_cursor_update(void) {
  _cursor_set(screen_get_display_page(), screen_cursor.line_index, screen_cursor.column_index);
}

void screen_init(void) {
  // Cursor init
  screen_set_display_page(0);
  screen_cursor.line_index = 0;
  screen_cursor.column_index = 0;
  screen_cursor_update();
  screen_clear();
}

void screen_update(void) {
  // _cursor_set(screen_cursor.page_index, screen_cursor.line_index, screen_cursor.column_index);
}

bit8_t screen_get_display_page(void) {
  return _get_display_page();
}

bit8_t screen_get_line(void) {
  return screen_cursor.line_index;
}

bit8_t screen_get_column(void) {
  return screen_cursor.column_index;
}

bit16_t screen_get_value_at(bit8_t line, bit8_t column) {
  bit16_t offset = (line * VGA_COLUMN_COUNT + column) * VGA_CELL_SIZE;

  _mem_read16(VGA_SEGMENT, offset);
}

void screen_set_cursor(bit8_t line_index, bit8_t column_index) {
  screen_cursor.line_index = line_index;
  screen_cursor.column_index = column_index;
  screen_cursor_update();
}

void screen_set_display_page(bit8_t display_page_index) {
  _set_display_page(display_page_index);
}

void screen_set_line(bit8_t line_index) {
  screen_cursor.line_index = line_index;
  screen_cursor_update();
}

void screen_set_column(bit8_t column_index) {
  screen_cursor.column_index = column_index;
  screen_cursor_update();
}

void screen_cursor_inc(void) {
  bool_t isLastIndex = (screen_cursor.line_index == VGA_LINE_COUNT - 1) && (screen_cursor.column_index == VGA_COLUMN_COUNT - 1);
  bool_t isLineFull = (screen_cursor.column_index + 1) / VGA_COLUMN_COUNT;

  if (isLastIndex) {
    return;
  }
    
  screen_cursor.line_index = (isLineFull) ? (screen_cursor.line_index + 1) : screen_cursor.line_index;
  screen_cursor.column_index = (screen_cursor.column_index + 1) % VGA_COLUMN_COUNT;

  screen_cursor_update();
}

void screen_cursor_dec(void) {
  bool_t isFirstIndex = (screen_cursor.line_index == 0) && (screen_cursor.column_index == 0);
  bool_t isLineEmpty = (screen_cursor.column_index + VGA_COLUMN_COUNT) / VGA_COLUMN_COUNT;
  
  if (isFirstIndex) {
    return;
  }

  if (screen_cursor.column_index == 0) {
    screen_cursor.line_index--;
    screen_cursor.column_index = VGA_COLUMN_COUNT - 1;
    return;
  }

  screen_cursor.column_index--;
  screen_cursor_update();
}

bit8_t screen_read_char(void) {
  bit8_t buffer_char = _kboard_get_key_blocking();
  screen_write_char(buffer_char);
  return buffer_char;
}

bit8_t screen_read_dec(void) {
  return ascii_to_dec(screen_read_char());
}

void screen_read_string(bit8_t *input_string, bit16_t string_length) {
  while (1 < string_length) {
    bit8_t character = screen_read_char();

    if (character == CHARACTER_LINE_FEED || character == CHARACTER_CARRIAGE_RETURN) {
      break;
    }

    if (character == CHARACTER_BACKSPACE) {
      continue;
    }

    *input_string = character;
    *input_string++;
    string_length--;
  }

  *input_string = '\0';
}

void screen_write_char(bit8_t character) {  
  bit16_t offset = (screen_cursor.line_index * VGA_COLUMN_COUNT + screen_cursor.column_index) * VGA_CELL_SIZE;
  bit16_t attribute = (COLOR_BLACK << 4) | COLOR_GREEN;
  // bit16_t attribute = (COLOR_BLACK << 4) | COLOR_LIGHT_GREEN;
  // bit16_t attribute = (COLOR_BLACK << 4) | COLOR_LIGHT_GREY;
  bit16_t value = (attribute << 8) | character;

  // Newline
  if (character == CHARACTER_LINE_FEED || character == CHARACTER_CARRIAGE_RETURN) {
    bit8_t currentLineIndex = screen_get_line();
    
    if (currentLineIndex + 1 < VGA_LINE_COUNT) {
      screen_set_line(currentLineIndex + 1);
      screen_set_column(0);
    }

    return;
  }

  // Backspace
  if (character == CHARACTER_BACKSPACE) {
    screen_cursor_dec();
    screen_write_char(' ');
    screen_cursor_dec();
    return;
  }

  // Tab
  // if (character == CHARACTER_HORIZONTAL_TAB) {
  //   bit8_t index = 0;

  //   for (index = 0; index < TAB_SIZE; index++) {
  //     screen_write_char(' ');
  //   }
    
  //   return;
  // }

  // Is character printable?
  if (screen_char_is_printable(character)) {
    _mem_write16(VGA_SEGMENT, offset, value);
    screen_cursor_inc();
  }
}

void screen_write_dec(bit16_t value) {
  static bit8_t buffer_ptr[5];
  bit16_t BUFFER_SIZE = 5; // MAX_DEC = 65535 (length 5) + null terminator

  bit16_t index = 0; 

  while (0 < value && index < BUFFER_SIZE) {
    buffer_ptr[index++] = '0' + value % 10;
    value /= 10;
  }

  while (index--) {
    screen_write_char(buffer_ptr[index]);
  }
}

void screen_write_hex(bit16_t value) {
  static bit8_t buffer_ptr[7];

  buffer_ptr[0] = '0';
  buffer_ptr[1] = 'x';
  buffer_ptr[2] = hexMap[(value >> 12) & LIMIT_4];
  buffer_ptr[3] = hexMap[(value >> 8) & LIMIT_4];
  buffer_ptr[4] = hexMap[(value >> 4) & LIMIT_4];
  buffer_ptr[5] = hexMap[(value) & LIMIT_4];
  buffer_ptr[6] = 0;

  screen_write_string(buffer_ptr);
}

void screen_write_string(bit8_t *string_ptr) {
  while (*string_ptr) {
    screen_write_char(*string_ptr++);
  }
}

// void screen_write_hstring(hptr_t string_hptr) {
//   bit16_t index = 0;
  
//   while (heap_read8(string_hptr, index) != NULL) {
//     screen_write_char(heap_read8(string_hptr, index++));
//   }
// }

void screen_write_newline(void) {

}

void screen_write_backspace(void) {

}

void screen_scroll(void) {
  bit8_t line = 1;
  bit16_t line_buffer[VGA_COLUMN_COUNT];

  while (line < VGA_LINE_COUNT) {
    bit8_t column = 0;

    /* Read line to the buffer */
    while (column < VGA_COLUMN_COUNT) {
      line_buffer[column] = screen_get_value_at(line, column);
      column++;
    }

    // column = 0;
    screen_set_line(line - 1);
    screen_set_column(0);

    screen_write_string(line_buffer);
    
    /* Write line to the previous index */
    // while (column < VGA_COLUMN_COUNT) {

    // }

    line++;
  }
}

void screen_clear(void) {
  bit16_t loopIndex = 0;

  // Reset cursor
  screen_set_line(0);
  screen_set_column(0);

  while (loopIndex < VGA_LINE_COUNT * VGA_COLUMN_COUNT) {
    screen_write_char(' ');
    loopIndex++;
  }

  // Reset cursor
  screen_set_line(0);
  screen_set_column(0);
  screen_cursor_update();
}

bool_t screen_char_is_printable(bit8_t character) {
  return (32 <= character && character <= 126);
}

