#ifndef T_TYPES_INCLUDED
#define T_TYPES_INCLUDED

#define NULL 0

#define FALSE 0
#define TRUE 1

#define LIMIT_4 0xf
#define LIMIT_8 0xff
#define LIMIT_16 0xffff

typedef short bool_t;

// Unsigned
typedef unsigned char bit8_t; 
typedef unsigned short bit16_t; 

// Signed
typedef signed char sbit8_t;
typedef signed short sbit16_t;

// Screen cursor
typedef struct {
  bit8_t page_index;
  bit16_t line_index;
  bit16_t column_index;
} screen_cursor_t;

// Heap pointer
typedef bit16_t hptr_t;

// Disk pointer
typedef bit16_t dptr_t;

#endif

