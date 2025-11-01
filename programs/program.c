/* Under-development */

#include "t_screen.h"
#include "t_kboard.h"
#include "t_utils.h"
#include "t_types.h"

bit8_t *string1;
bit8_t num_1;
bit8_t num_2;
bit8_t sum;

void program_main(void);
static void draw_top_bar(void);

void _program_init(void) {
  program_main();
}

void program_main(void) {
  screen_clear();
  screen_set_display_page(0);
  screen_set_line_index(0);
  screen_set_column_index(0);
  
  draw_top_bar();

  screen_set_line_index(5);
  screen_set_column_index(0);

  screen_write_char('y');
  screen_write_char(0xdb);
  screen_write_char(151);
  screen_write_char(151);
  screen_write_char(151);
  
  // while (1) {
  //   screen_set_line_index(3);
  //   screen_set_column_index(0);
  //   num_1 = screen_read_dec();
  //   screen_write_char('\n');
  //   num_2 = screen_read_dec();
  
  //   screen_write_char('\n');
    
  //   screen_write_string("Sum: ");
  //   screen_write_dec(num_1 + num_2);
  //   kboard_get_key_blocking();
  //   screen_clear();
  // }


  
  // while (*string1) {
  //   *string1 = kboard_get_char_blocking();
  //   screen_write_char(*string1);
  // }

  
  
  while (1);
}

static void draw_top_bar(void) {
  screen_set_line_index(0);
  screen_set_column_index(0);
  screen_write_string("+------------------------------------------------------------------------------+");
  screen_write_char('|');

  screen_set_column_index(37);
  screen_write_string("TCalc");

  screen_set_column_index(79);
  screen_write_char('|');
  screen_write_string("+------------------------------------------------------------------------------+");
}