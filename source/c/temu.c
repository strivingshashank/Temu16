#include "t_shell.h"
#include "t_types.h"
#include "t_disk.h"

#define TEMP_BUFFER_SIZE_LIMIT 100

// bool_t g_kernel_halt_status = FALSE;
bit8_t g_test_string[TEMP_BUFFER_SIZE_LIMIT];
bit8_t g_bit8;

void _t_init(void);
void t_update(void);
void t_main(void);

void t_main(void) {
  while (TRUE) {
    t_update();
  }
}

void _t_init(void) {
  heap_init();
  shell_init();
  t_main();
}

void t_update(void) {
  shell_update();
}

