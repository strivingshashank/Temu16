/* Under-development */

#include "t_io.h"
#include "t_utils.h"
#include "t_types.h"
#include "t_memory.h"

void greeting(void);
void program_main(void);

void _program_init(void) {
  heap_init();
  program_main();
}

void program_main(void) {
  bit8_t temp_str[100];
  bit16_t index = 0;
  bit16_t *test_ptr;
  
  greeting();

  test_ptr = (bit16_t *)(heap_alloc(100));

  index = 0;

  while (index < 100) {
    test_ptr[index] = index;
    index++;
  }

  index = 0;
  
  while (index < 100) {
    write_dec(test_ptr[index]);
    index++;
  }

  read_char();

  // while (1) {
  //   write_string(" > ");
  //   index = 0 = read_dec();
  //   write_char('\n');
  //   write_dec(index = 0);
  //   write_char('\n');

  //   // write_hex(get_key_blocking());
  //   // read_char();
  //   // if (get_key_blocking() == 0x1b) {
  //   //   break;
  //   // }
  // }

  write_string("Exited : TCalc\n");
}

void greeting(void) {
  write_string("----- TCalc -----\n");
  write_string("Expression evaluating calculator.\n");
}