#include "t_utils.h"
#include "t_memory.h"
#include "t_io.h"
#include "asm_bindings.h"

static bit8_t g_hexMap[17] = "0123456789abcdef";

bit8_t time_get_hours(void);
bit8_t time_get_minutes(void);
bit8_t time_get_seconds(void);
bit8_t time_get_century(void);
bit8_t time_get_year(void);
bit8_t time_get_month(void);
bit8_t time_get_date(void);
bit8_t *time_get_date_str(void);
bit8_t *time_get_time_str(void);
bit8_t bcd_to_dec(bit8_t bcd_value);
bit8_t ascii_to_dec(bit8_t ascii_value);
bit8_t dec_to_ascii(bit8_t dec_value);
bit8_t *dec_to_str(bit16_t dec_value);
bit8_t *sdec_to_str(sbit16_t sdec_value);
bit8_t *hex_to_str(bit16_t hex_value);
bit16_t str_to_dec(bit8_t *dec_str);
bool_t char_is_printable(bit8_t character);
bool_t str_cmp(bit8_t *str1, bit8_t *str2);
size_t str_get_size(bit8_t *str);

bit8_t time_get_hours(void) {
  return (bcd_to_dec(_time_get_hours()));
}

bit8_t time_get_minutes(void) {
  return (bcd_to_dec(_time_get_minutes()));
}

bit8_t time_get_seconds(void) {
  return (bcd_to_dec(_time_get_seconds()));
}

bit8_t time_get_century(void) {
  return (bcd_to_dec(_time_get_century()));
}

bit8_t time_get_year(void) {
  return (bcd_to_dec(_time_get_year()));
}

bit8_t time_get_month(void) {
  return (bcd_to_dec(_time_get_month()));
}

bit8_t time_get_date(void) {
  return (bcd_to_dec(_time_get_date()));
}

bit8_t *time_get_date_str(void) {
  bit8_t *time_str = NULL_PTR;
  bit8_t time_str_size = 11;
  bit8_t century = time_get_century();
  bit8_t year = time_get_year();
  bit8_t month = time_get_month();
  bit8_t date = time_get_date();
  
  time_str = heap_alloc(time_str_size);

  if (time_str == NULL_PTR) {
    return NULL_PTR;
  }

  time_str[0] = dec_to_ascii(century / 10);
  time_str[1] = dec_to_ascii(century % 10);
  time_str[2] = dec_to_ascii(year / 10);
  time_str[3] = dec_to_ascii(year % 10);
  time_str[4] = '-';
  time_str[5] = dec_to_ascii(month / 10);
  time_str[6] = dec_to_ascii(month % 10);
  time_str[7] = '-';
  time_str[8] = dec_to_ascii(date / 10);
  time_str[9] = dec_to_ascii(date % 10);
  time_str[10] = '\0';

  return time_str;
}

bit8_t *time_get_time_str(void) {
  bit8_t *time_str = NULL_PTR;
  bit8_t time_str_size = 6;
  bit8_t hours = time_get_hours();
  bit8_t minutes = time_get_minutes();
  bit8_t seconds = time_get_seconds();
  
  time_str = heap_alloc(time_str_size);

  if (time_str == NULL_PTR) {
    return NULL_PTR;
  }

  time_str[0] = dec_to_ascii(hours / 10);
  time_str[1] = dec_to_ascii(hours % 10);
  time_str[2] = ':';
  time_str[3] = dec_to_ascii(minutes / 10);
  time_str[4] = dec_to_ascii(minutes % 10);
  time_str[5] = '\0';

  return time_str;
}

bit8_t bcd_to_dec(bit8_t bcd_value) {
  return ((bcd_value >> 4) * 10 + (bcd_value & 0xf));
}

bit8_t ascii_to_dec(bit8_t ascii_value) {
  return (ascii_value - '0');
}

bit8_t dec_to_ascii(bit8_t dec_value) {
  return (dec_value + '0');
}

bit8_t *dec_to_str(bit16_t dec_value) {
  bit8_t *buffer = NULL_PTR;
  size_t idx = 0;

  buffer = heap_alloc(6);

  if (buffer == NULL_PTR) {
    return NULL_PTR;
  }

  if (dec_value == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return buffer;
  }
  
  while (0 < dec_value && idx < 5) {
    buffer[idx] = '0' + dec_value % 10;
    dec_value /= 10;
    idx++;
  }

  buffer[idx] = '\0';

  {
    size_t reverse_idx = 0;
    size_t str_size = idx - 1;
    
    while (reverse_idx < str_size) {
      bit8_t dec_char = buffer[reverse_idx];
      buffer[reverse_idx] = buffer[str_size];
      buffer[str_size] = dec_char;
      reverse_idx++;
      str_size--;
    }
  }

  return buffer;
}

bit8_t *sdec_to_str(sbit16_t sdec_value) {
  bit8_t *buffer = NULL_PTR;
  bool_t is_negative = FALSE;
  size_t idx = 0;

  buffer = heap_alloc(8);

  if (buffer == NULL_PTR) {
    return NULL_PTR;
  }

  if (sdec_value == 0) {
    buffer[0] = '0';
    buffer[1] = '\0';
    return buffer;
  }

  /* Special case */
  if (sdec_value == (sbit16_t)(-32768)) {
    buffer[0] = ('-');
    buffer[1] = ('3');
    buffer[2] = ('2');
    buffer[3] = ('7');
    buffer[4] = ('6');
    buffer[5] = ('8');
    buffer[6] = '\0';
    return buffer;
  }

  if (sdec_value < 0) {
    is_negative = TRUE;
    sdec_value = -(sdec_value);
  }
  
  while (0 < sdec_value && idx < 7) {
    buffer[idx] = dec_to_ascii(sdec_value % 10);
    sdec_value /= 10;
    idx++;
  }

  if (is_negative == TRUE) {
    buffer[idx++] = '-';
  }

  buffer[idx] = '\0';

  {
    size_t reverse_idx = 0;
    size_t str_size = idx - 1;

    while (reverse_idx < str_size) {
      bit8_t dec_char = buffer[reverse_idx];
      buffer[reverse_idx] = buffer[str_size];
      buffer[str_size] = dec_char;
      reverse_idx++;
      str_size--;
    }
  }

  return buffer;
}

bit8_t *hex_to_str(bit16_t hex_value) {
  bit8_t *buffer;

  buffer = heap_alloc(7);

  if (buffer == NULL_PTR) {
    return NULL_PTR;
  }
  
  buffer[0] = '0';
  buffer[1] = 'x';
  buffer[2] = g_hexMap[(hex_value >> 12) & LIMIT_4];
  buffer[3] = g_hexMap[(hex_value >> 8) & LIMIT_4];
  buffer[4] = g_hexMap[(hex_value >> 4) & LIMIT_4];
  buffer[5] = g_hexMap[(hex_value) & LIMIT_4];
  buffer[6] = 0;

  return buffer;
}

bit16_t str_to_dec(bit8_t *dec_str) {
  size_t idx = 0;
  bit16_t dec_value = 0;
  
  if (dec_str == NULL_PTR) {
    return 0;
  }

  while ('0' <= dec_str[idx] && dec_str[idx] <= '9') {
    dec_value = (dec_value * 10) + (dec_str[idx] - '0');
    idx++;
  }

  return dec_value;
}

bool_t char_is_printable(bit8_t character) {
  return (32 <= character && character <= 128);
}

bool_t str_cmp(bit8_t *str1, bit8_t *str2) {
  while (*str1 == *str2) {
    if (*str1 == '\0') {
      return TRUE;
    }

    str1++;
    str2++;
  }

  return FALSE;
}

size_t str_get_size(bit8_t *str) {
  size_t length = 0;

  while (*str++) {
    length++;
  }

  return length;
}

