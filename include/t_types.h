#ifndef T_TYPES_INCLUDED
#define T_TYPES_INCLUDED

#define NULL 0

#define FALSE 0
#define TRUE 1

#define LIMIT_4 0xf
#define LIMIT_8 0xff
#define LIMIT_16 0xffff

#define LIMIT_S16_MIN -32768
#define LIMIT_S16_MAX 32767

typedef unsigned char bool_t; /* TRUE / FALSE */

// Unsigned
typedef unsigned char bit8_t; /* 0 to 255 */
typedef unsigned short bit16_t; /* 0 to 65535 */
typedef unsigned short size_t; 

// Signed
typedef signed char sbit8_t; /* -128 to +127 */
typedef signed short sbit16_t; /* -32768 to +32767 */

#endif

