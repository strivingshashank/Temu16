#ifndef ASM_BINDINGS_INCLUDED
#define ASM_BINDINGS_INCLUDED

#include "t_types.h"

// Needs work here
extern void _cursor_set(bit8_t page_index, bit8_t line_index, bit8_t column_index);
extern void _set_display_page(bit8_t display_page_index);
extern bit8_t _get_display_page(void);

/* BIOS screen */
extern void _screen_scroll(void);
extern void _screen_clear(void);
extern void _screen_write_char(bit8_t character);

/* memory */
extern void _mem_write8(bit16_t segment, bit16_t offset, bit8_t value);
extern void _mem_write16(bit16_t segment, bit16_t offset, bit16_t value);
extern bit8_t _mem_read8(bit16_t segment, bit16_t offset);
extern bit16_t _mem_read16(bit16_t segment, bit16_t offset);
extern void _mem_copy(bit16_t source_segment, bit16_t source_offset, bit16_t destination_segment, bit16_t destination_offset, bit16_t bytes);

/* kboard */
extern bit16_t _kboard_get_key_buffer(void);
extern bit16_t _kboard_get_key_blocking(void);

/* disk */
extern bit8_t _disk_read(bit8_t cylinder_index, bit8_t head_index, bit8_t sector, bit16_t destination_segement, bit16_t destination_offset, bit16_t sectors_to_read);

/* time */
extern bit8_t _time_get_hours(void);
extern bit8_t _time_get_minutes(void);
extern bit8_t _time_get_seconds(void);

/* jump far to a segment:offset */
extern void _jump_far(bit16_t code_segment, bit16_t instruction_pointer);

#endif

