/*
 * basedefs.h
 *
 *  Created on: Jul 20, 2021
 *      Author: djek-sweng
 */

#ifndef BASEDEFS_H_
#define BASEDEFS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/* includes */
/*--------------------------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

/*--------------------------------------------------------------------------------------------------------------------*/
/* defines */
/*--------------------------------------------------------------------------------------------------------------------*/
/* Logical bitwise operations. */
/* Exclusive OR */
#define XOR(x,y)                      ((x) ^ (y))

/* Sizes (in Kbytes). */
#define SIZE_1K                       (0x00000400)
#define SIZE_2K                       (0x00000800)
#define SIZE_4K                       (0x00001000)
#define SIZE_8K                       (0x00002000)
#define SIZE_16K                      (0x00004000)
#define SIZE_32K                      (0x00008000)
#define SIZE_64K                      (0x00010000)
#define SIZE_96K                      (0x00018000)
#define SIZE_128K                     (0x00020000)
#define SIZE_256K                     (0x00040000)
#define SIZE_384K                     (0x00060000)
#define SIZE_512K                     (0x00080000)
#define SIZE_1024K                    (0x00100000)

/* Get size (in bytes). */
#define SIZE(x)                       ((int) sizeof(x))

/* Get number of elements in array. */
#define NUMEL(x)                      ((int) (sizeof(x)/sizeof(x[0])))

/* Read data from register address. */
#define READ_U32(adr)                 ( *( (uint32_t*) adr ) )
#define READ_U16(adr)                 ( *( (uint16_t*) adr ) )
#define READ_U8(adr)                  ( *( ( uint8_t*) adr ) )

/* Write data to register address. */
#define WRITE_U32(adr,dat)            do {                               \
                                        uint32_t* ptr = (uint32_t*) adr; \
                                        *ptr = (uint32_t) dat;           \
                                      } while(0U)
#define WRITE_U16(adr,dat)            do {                               \
                                        uint16_t* ptr = (uint16_t*) adr; \
                                        *ptr = (uint16_t) dat;           \
                                      } while(0U)
#define WRITE_U8(adr,dat)             do {                               \
                                        uint8_t* ptr = (uint8_t*) adr;   \
                                        *ptr = (uint8_t) dat;            \
                                      } while(0U)

/* Clear register. */
#define REG_CLR(reg,mask)             (reg &= ~(mask))

/* Stop here (and hold in endless loop). */
#define STOP_HERE()                   do {            \
                                        for (;;) {    \
                                          __NOP();    \
                                        }             \
                                      } while(0U)

/* Optional keywords. */
#define CONST                         const
#define VOLATILE                      volatile
#define STATIC                        static
#define INLINE                        inline
#define EXTERN                        extern

/* Avoid GCC warnings. */
#define NO_WARNING(x)                 ((void) x)

/* Print formated string. */
#define SPRINTF(str, ...)             (snprintf(&str[0], NUMEL(str), __VA_ARGS__))

/* get minimal and maximal value. */
#define GET_MIN(a,b)                  (((a) < (b)) ? (a) : (b))
#define GET_MAX(a,b)                  (((a) > (b)) ? (a) : (b))

/* Logical values. */
typedef enum LogicalValue
{
  FALSE = 0,
  TRUE = !FALSE
} LogicalValue_t;

/* Return states. */
typedef enum ReturnState
{
  NO_FAILURE = 0,
  FAILURE = -1,
  TIMEOUT = -2
} ReturnState_t;

/* Attributes. */
#if defined ( __GNUC__ )              /* GNU compiler */

/* This attribute tells the compiler that the function is an embedded assembly function.
 * You can write the body of the function entirely in assembly code using __asm statements. */
#define A_NAKED                       __attribute__((naked))

/* Informs the compiler that the function does not return.
 * The compiler can then perform optimizations by removing code that is never reached. */
#define A_NORETURN                    __attribute__((noreturn))

#endif

/*--------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* BASEDEFS_H_ */
