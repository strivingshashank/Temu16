#include "t_shell.h"
#include "t_screen.h"
#include "t_kboard.h"
#include "t_utils.h"
#include "t_disk.h"
#include "asm_bindings.h"

#define PROMPT_LINE_INDEX 23
#define PROMPT_COLUMN_INDEX 4
#define PROMPT_BUFFER_LIMIT 75

#define CANVAS_BASE_LINE_INDEX 3
#define CANVAS_BASE_COLUMN_INDEX 0
#define CANVAS_END_LINE_INDEX 22
#define CANVAS_END_COLUMN_INDEX 80

typedef enum {
  COMMAND_EXIT,
  COMMAND_CLEAR,
  COMMAND_HELP,
  COMMAND_UNKNOWN
} shell_command_t;

typedef struct {
  bit8_t line_index;
  bit8_t column_index;
} canvas_cursor_t;

// static bit16_t canvas_column_offset = 0;

static bit8_t prompt_buffer[PROMPT_BUFFER_LIMIT];
static bit8_t prompt_buffer_index = 0;

static canvas_cursor_t canvas_cursor;

/* Declarations */
void shell_init(void);

static void shell_loop(void);
static void shell_draw_ui(void);
// static void shell_draw_time(void);
static void shell_draw_top_bar(void);
static void shell_draw_canvas(void);
static void shell_clear_canvas(void);
static void shell_draw_prompt(void);
static void shell_clear_prompt(void);
static void shell_get_prompt(void);
static void shell_process_prompt(void);

/* Testing... */
void try_loading(void);

/* Definitions */
void shell_init(void) {
  screen_init();

  canvas_cursor.line_index = 0;
  canvas_cursor.column_index = 0;

  shell_draw_ui();
}

void shell_update(void) {
  shell_get_prompt();
}

// static void shell_draw_time(void) {
//   bit8_t hours = time_get_hours();
//   bit8_t minutes = time_get_minutes();
//   bit8_t seconds = time_get_seconds();

//   screen_set_line_index(1);
//   screen_set_column_index(73);
  
//   if (hours < 10) {
//     screen_write_char('0');
//   }
  
//   screen_write_dec(hours);
//   screen_write_char(':');

//   if (minutes < 10) {
//     screen_write_char('0');
//   }
    
//   screen_write_dec(minutes);
// }

static void shell_draw_top_bar(void) {
  screen_set_line_index(0);
  screen_set_column_index(0);
  screen_write_string("+------------------------------------------------------------------------------+");
  screen_write_char('|');

  screen_set_column_index(37);
  screen_write_string("TShell");

  screen_set_column_index(79);
  screen_write_char('|');
  screen_write_string("+------------------------------------------------------------------------------+");
}

static void shell_draw_canvas(void) {
  canvas_cursor.line_index = 0;

  while ((CANVAS_BASE_LINE_INDEX + canvas_cursor.line_index) <= CANVAS_END_LINE_INDEX - 1) {
    screen_set_line_index(CANVAS_BASE_LINE_INDEX + canvas_cursor.line_index);
    
    screen_set_column_index(CANVAS_BASE_COLUMN_INDEX);
    screen_write_char('|');

    screen_set_column_index(CANVAS_END_COLUMN_INDEX - 1);
    screen_write_char('|');
    
    canvas_cursor.line_index++;
  }
}

static void shell_draw_prompt(void) {
  screen_set_line_index(22);
  screen_set_column_index(0);
  screen_write_string("+------------------------------------------------------------------------------+");

  screen_set_line_index(24);
  screen_set_column_index(0);
  screen_write_string("+------------------------------------------------------------------------------+");

  screen_set_line_index(23);
  screen_set_column_index(0);
  screen_write_char('|');
  screen_write_char(' ');
  screen_write_char('$');
  screen_write_char(' ');

  screen_set_column_index(VGA_COLUMN_COUNT - 1);
  screen_write_char('|');
}

static void shell_draw_ui(void) {
  // shell_draw_top_bar();
  // shell_draw_prompt();
  // shell_draw_canvas();
}

static void shell_clear_prompt(void) {
  screen_set_line_index(PROMPT_LINE_INDEX);
  screen_set_column_index(PROMPT_COLUMN_INDEX);

  // Clear prompt with ' ' (spaces)
  screen_write_string("                                                                           ");

  prompt_buffer[0] = NULL;
}

static void shell_clear_canvas(void) {
  bit8_t canvas_line_count = CANVAS_END_LINE_INDEX - CANVAS_BASE_LINE_INDEX;
  bit8_t canvas_column_count = CANVAS_END_COLUMN_INDEX - CANVAS_BASE_COLUMN_INDEX;
  bit16_t loop_index = 0;
  
  screen_set_line_index(CANVAS_BASE_LINE_INDEX);
  screen_set_column_index(CANVAS_BASE_COLUMN_INDEX);

  while (loop_index <= (canvas_line_count * canvas_column_count - 1)) {
    screen_write_char(' ');
    loop_index++;
  }

  shell_draw_canvas();
}

static void shell_get_prompt(void) {
  bit16_t key;
  bit8_t charCode;
  bit8_t scanCode;
  
  screen_set_line_index(PROMPT_LINE_INDEX);
  screen_set_column_index(PROMPT_COLUMN_INDEX + prompt_buffer_index);

  key = kboard_get_key();
  charCode = key & LIMIT_8;
  scanCode = (key >> 8) & LIMIT_8;    
  
  // Submit prompt
  if (scanCode == KEY_ENTER) {
    prompt_buffer[prompt_buffer_index] = NULL;
    prompt_buffer_index = 0;
    
    // Process prompt
    shell_process_prompt();
        
    shell_clear_prompt();
    return;
  }

  // Backspace
  if (scanCode == KEY_BACKSPACE) {
    if (prompt_buffer_index != 0) {
      prompt_buffer_index--;
      screen_write_char('\b');
    }

    return;
  }
  
  if (screen_char_is_printable(charCode)) {
    prompt_buffer[prompt_buffer_index] = charCode;
    screen_write_char(charCode);
    prompt_buffer_index = (prompt_buffer_index + 1) % PROMPT_BUFFER_LIMIT;
  }
}

static void shell_process_prompt(void) {
  shell_command_t command;
  
  if (prompt_buffer[0] == NULL) {
    return;
  }

  if (string_compare(prompt_buffer, "exit")) {
    command = COMMAND_EXIT;
  } else if (string_compare(prompt_buffer, "clear")) {
    command = COMMAND_CLEAR;
    shell_clear_canvas();
    shell_draw_canvas();
  } else if (string_compare(prompt_buffer, "tcalc")) {
    try_loading();      
  } else if (string_compare(prompt_buffer, "help")) {
    command = COMMAND_HELP;
    screen_set_line_index(5);
    screen_set_column_index(5);
    screen_write_string("Hi Milan!");
  }
  else {
    command = COMMAND_UNKNOWN;
  }
}

void try_loading(void) {
  bit16_t linear_block_address = 129;
  bit16_t program_segment = 0x4000;
  bit16_t program_offset = 0x00;
  bit8_t sectors_to_load = 128;
  bit8_t disk_load_status = disk_read(linear_block_address, program_segment, program_offset, sectors_to_load);

  if (disk_load_status != 0) {
    screen_write_string("Disk error: ");
    screen_write_hex(disk_load_status);
    screen_write_char('\n');
  } 
  else {
    /* Debug prompt */
    screen_write_string("Disk load successful.\n");
    screen_write_string("Initiating far jump...\n");
    
    _jump_far(program_segment, program_offset);
     
    /* Un-reachable, if jump succeeds */

    screen_set_display_page(0);
    screen_set_line_index(0);
    screen_set_column_index(0);

    screen_write_string("Program load failed.\n");
    screen_write_string("Entering hang state\n");
    while (1);
  }
}

static void shell_loop(void) {
  while (TRUE) {
    screen_write_string("$ ");
  }
}