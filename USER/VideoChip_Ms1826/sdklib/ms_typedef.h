/**
* @file  ms_typedef.h  
* @brief SDK Data type defines                                                        
*/

/**
 * @defgroup API_TYPE_DEF SDK Data type defines
 *
 * @brief define SDK data type
 * @{
 */


#ifndef __MS_TYPEDEF_H__
#define __MS_TYPEDEF_H__

#include "ms_sdk_typedef.h"

/*
* generic mask macro definitions
*/
#define MSRT_BIT0                   (0x01)
#define MSRT_BIT1                   (0x02)
#define MSRT_BIT2                   (0x04)
#define MSRT_BIT3                   (0x08)
#define MSRT_BIT4                   (0x10)
#define MSRT_BIT5                   (0x20)
#define MSRT_BIT6                   (0x40)
#define MSRT_BIT7                   (0x80)
    
#define MSRT_MSB8BITS               MSRT_BIT7
#define MSRT_LSB                    MSRT_BIT0
    
// Bit7 ~ Bit0
#define MSRT_BITS7_6                (0xC0)
#define MSRT_BITS7_5                (0xE0)
#define MSRT_BITS7_4                (0xF0)
#define MSRT_BITS7_3                (0xF8)
#define MSRT_BITS7_2                (0xFC)
#define MSRT_BITS7_1                (0xFE)
#define MSRT_BITS7_0                (0xff)

#define MSRT_BITS6_5                (0x60)
#define MSRT_BITS6_4                (0x70)
#define MSRT_BITS6_2                (0x7c)
#define MSRT_BITS6_1                (0x7e)
#define MSRT_BITS6_0                (0x7f)

#define MSRT_BITS5_4                (0x30)
#define MSRT_BITS5_3                (0x38)
#define MSRT_BITS5_2                (0x3c)
#define MSRT_BITS5_0                (0x3f)

#define MSRT_BITS4_3                (0x18)
#define MSRT_BITS4_2                (0x1c)
#define MSRT_BITS4_1                (0x1e)
#define MSRT_BITS4_0                (0x1f)

#define MSRT_BITS3_2                (0x0C)
#define MSRT_BITS3_1                (0x0E)
#define MSRT_BITS3_0                (0x0F)

#define MSRT_BITS2_1                (0x06)
#define MSRT_BITS2_0                (0x07)

#define MSRT_BITS1_0                (0x03)

#define MSRT_BIT_MASK(n)            (1UL << (n))
#define MSRT_BITS_MASK(h,l)         ((((~0UL)-(1UL << (l))) + 1) & ((~0UL) >> (32-1-(h))))
#define BITS_VAL(x,h,l)             (((x)>>(l)) & MSRT_BITS_MASK(((h)-(l)),0))
#define BIT_VAL(x,l)                (((x)>>(l)) & MSRT_BIT_MASK(0))
#define SET_BITS_VAL(x,h,l)         (((x) & MSRT_BITS_MASK(((h)-(l)),0)) << (l))
#define SET_BIT_VAL(x,l)            (((x) & MSRT_BIT_MASK(0)) << (l))
#define IS_TRUE(x)                  ((x)?MS_TRUE:false)
#define TOGGLE_BOOL_VAL(x)          ((x)?(x)=false:(x)=MS_TRUE)
#define VAR_NAME(name)              (#name)
#define ARR_SIZE(arr)               (sizeof(arr)/sizeof(arr[0]))

/**
 * @def ALIGN_UP(size, align)
 * Return the most contiguous size aligned at specified width. ALIGN_UP(13, 4)
 * would return 16.
 */
#define ALIGN_UP(size, align)        (((size) + (align) - 1) & ~((align) - 1))

 /**
  * @def ALIGN_DOWN(size, align)
  * Return the down number of aligned at specified width. ALIGN_DOWN(13, 4)
  * would return 12.
  */
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

 /**
  * @def roundup(x, y) 
  * 用于将一个数向上取整到该数的指定数值的倍数
  * 例如roundup(63,8) 会把63向上取整为8的倍数，值会返回64
  */
#define roundup(x, y) ((((x) + (y - 1)) / y) * y)

 /**
  * @def ceil(x, y) 
  * ceil(13,4) = 4
  */
#define CEIL(M,N)  ((M - 1) / N + 1)

#endif  //__MS_TYPEDEF_H__

/**
 * @}
 */
