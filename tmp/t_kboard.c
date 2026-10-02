#include "t_kboard.h"
#include "t_screen.h"
#include "asm_bindings.h"

bit16_t kboard_get_key(void) {
  return _kboard_get_key_buffer();
}

bit16_t kboard_get_key_blocking(void) {
  return _kboard_get_key_blocking();
}

