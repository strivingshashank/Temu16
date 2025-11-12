/* Under-development */

#include "t_io.h"
#include "t_utils.h"
#include "t_types.h"
#include "t_memory.h"
#include "t_structs.h"
#include "asm_bindings.h"

void greeting(void);
void body(void);
void program_main(void);

void _program_init(void) {
  heap_init();
  program_main();
}

void program_main(void) {
  greeting();
  body();

  write_char('\n');
  return;
}

void greeting(void) {
  write_str("----- TInfo -----\n");
  write_str("System information\n\n");
}

void body(void) {
  bit8_t *time_str = NULL_PTR;
  bit8_t *date_str = NULL_PTR;
  
  write_str("CPU: Intel 8086 (16-bit Real Mode)\n");
  
  write_str("RAM: ");
  write_dec(memory_get_size());
  write_str(" KB (Max)\n");

  write_str("Disk: 1.44 MB Floppy (CHS)\n");

  time_str = time_get_time_str();
  write_str("Time: ");
  write_str(time_str);
  write_char('\n');
  heap_free(time_str, str_get_size(time_str));

  date_str = time_get_date_str();
  write_str("Date: ");
  write_str(date_str);
  write_char('\n');
  heap_free(date_str, str_get_size(date_str));
}

// void tcalc(void) {
  
// }

