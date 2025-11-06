#include "t_utils.h"
#include "asm_bindings.h"

void memory_copy_far(bit16_t source_segment, bit16_t source_offset, bit16_t destination_segment, bit16_t destination_offset, bit16_t bytes);
bit8_t time_get_hours(void);
bit8_t time_get_minutes(void);
bit8_t time_get_seconds(void);
bit8_t ascii_to_dec(bit8_t ascii_value);
bit8_t dec_to_ascii(bit8_t dec_value);
bool_t char_is_printable(bit8_t character);
bool_t string_compare(bit8_t *string1, bit8_t *string2);
bit16_t string_get_length(bit8_t *string);

void memory_copy_far(bit16_t source_segment, bit16_t source_offset, bit16_t destination_segment, bit16_t destination_offset, bit16_t bytes) {
  _mem_copy(source_segment, source_offset, destination_segment, destination_offset, bytes);
}

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
  return (ascii_value - '0');
}

bit8_t dec_to_ascii(bit8_t dec_value) {
  return (dec_value + '0');
}

bool_t char_is_printable(bit8_t character) {
  return (32 <= character && character <= 128);
}

bool_t string_compare(bit8_t *string1, bit8_t *string2) {
  while (*string1 == *string2) {
    if (*string1 == '\0') {
      return TRUE;
    }

    string1++;
    string2++;
  }

  return FALSE;
}

bit16_t string_get_length(bit8_t *string) {
  bit16_t length = 0;

  while (*string++) {
    length++;
  }

  return length + 1;
}

