////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
* 
*
*  
*	   
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

#ifndef _STD_DEFS_H
#define _STD_DEFS_H

#include "stdlib.h"
#include "string.h"
#include "stdio.h"

////////////////////////////////////////////////////////////////////////////////
#define uchar unsigned char
#define uint unsigned short
#define ulong unsigned long

#ifndef u8
   #define u8    unsigned char
   #define u16   unsigned short
   #define u32   unsigned long
   #define s8    signed char
   #define s16   signed short
   #define s32   signed long
	 #define f32   float
	 #define f64   double
   #define u8_t  unsigned char
   #define u16_t unsigned short
   #define u32_t unsigned long
   #define s8_t  signed char
   #define s16_t signed short
   #define s32_t signed long
#endif

#ifndef __u8
   #define __u8    unsigned char
   #define __u16   unsigned short
   #define __u32   unsigned long
#endif
#ifndef U8
   #define U8    unsigned char
   #define U16   unsigned short
   #define U32   unsigned long
   #define S8    signed char
   #define S16   signed short
   #define S32   signed long
   #define U8_T  unsigned char
   #define U16_T unsigned short
   #define U32_T unsigned long
   #define S8_T  signed char
   #define S16_T signed short
   #define S32_T signed long
#endif

#ifndef uint8_t
	#define uint8_t  u8_t
	#define uint16_t u16_t
	#define uint32_t u32_t
#endif

#define diff(x,y) (((x)>(y))?(x-y):(y-x))

#define UNUSED_PARA(x) x=x

#define MSB(x)    (((x) >> 8) & 0xff)
#define LSB(x)    ((x) & 0xff)

#define xdata
#define code const

#ifdef WIN32
#define far 
#endif
	
#endif /* define _STD_DEFS_H */
////////////////////////////////////////////////////////////////////////////////
