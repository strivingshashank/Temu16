#include "t_utils.h"
#include "asm_bindings.h"

bit8_t time_get_hours(void) {
  bit8_t hours_bcd = _time_get_hours();
  bit8_t hours_dec = hours_bcd / 16;
  hours_dec = hours_dec * 10;
  hours_dec += hours_bcd % 16;
  return hours_dec;
}

bit8_t time_get_minutes(void) {
  bit8_t minutes_bcd = _time_get_minutes();
  bit8_t minutes_dec = minutes_bcd / 16;
  minutes_dec = minutes_dec * 10;
  minutes_dec += minutes_bcd % 16;
  return minutes_dec;
}

bit8_t time_get_seconds(void) {
  bit8_t seconds_bcd = _time_get_seconds();
  bit8_t seconds_dec = seconds_bcd / 16;
  seconds_dec = seconds_dec * 10;
  seconds_dec +=seconds_bcd % 16;
  return seconds_dec;
}

bit8_t ascii_to_dec(bit8_t ascii_value) {
  return (ascii_value - 48);
}

bit8_t dec_to_ascii(bit8_t dec_value) {
  return (dec_value + 48);
}

// String operations
// bit16_t k_getStringLength(char *string) {
//   bit16_t stringLength = 0;

//   while (*string++) {
//     stringLength++;
//   } 

//   return stringLength;
// }

bool_t string_compare(bit8_t *string1, bit8_t *string2) {
  while (*string1 == *string2) {
    if (*string1 == NULL) {
      return TRUE;
    }

    string1++;
    string2++;
  }

  return FALSE;
}