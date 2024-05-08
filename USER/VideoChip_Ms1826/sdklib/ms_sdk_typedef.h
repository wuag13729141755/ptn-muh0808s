/**
* @file  ms_sdk_typedef.h
* @brief SDK Data type defines
*/

/**
 * @defgroup API_TYPE_DEF SDK Data type defines
 *
 * @brief define SDK data type
 * @{
 */


#ifndef __MS_SDK_TYPEDEF_H__
#define __MS_SDK_TYPEDEF_H__

#include "Datatype.h"

/**
*  Global typedefs.
*/

#ifndef NULL
#define NULL ((void*)0)
#endif

// For ARM platform
#if 1//defined (_PLATFORM_ARM_)
#define  __CODE const
#define  __XDATA
#define  __DATA
#define  __IDATA
#define  __NEAR
//#define  __IO volatile

//typedef _Bool BOOL;

#elif 0//defined (__STD_GCC__)
#define  __CODE const
#define  __XDATA
#define  __DATA
#define  __IDATA
#define  __NEAR
#define  __IO volatile

typedef _Bool BOOL;

#elif 0//defined (_PLATFORM_WINDOWS_)
#define  __CODE
#define  __XDATA
#define  __DATA
#define  __IDATA
#define  __NEAR
#define  __IO

#elif 0//defined (__KEIL_C__)
#define __CODE code
#define __XDATA xdata
#define __DATA data
#define __IDATA idata
#define __NEAR
#define __IO volatile

//bool bype
typedef unsigned char BOOL;

#elif 0//defined (__CSMC__)
#define __CODE const
#define __XDATA
#define __DATA
#define __IDATA
#define __NEAR @near
#define __IO volatile

//bool bype
typedef _Bool BOOL;
#elif 0//defined (_IAR_)
#define __CODE const
#define __XDATA
#define __DATA
#define __IDATA
#define __NEAR @near
#define __IO volatile

//bool bype
typedef _Bool BOOL;
#else
#define __CODE const

#endif // end of compiler platform define

//unsigned integer type
typedef unsigned char UINT8;
typedef char          CHAR;
typedef unsigned short UINT16;

//signed integer type
typedef signed char INT8;
typedef signed short INT16;

//32bit type
#if defined (_PLATFORM_ARM_) || defined (_PLATFORM_WINDOWS_)
typedef unsigned int UINT32;
typedef signed int INT32;
#else
typedef unsigned long int UINT32;
typedef signed long int INT32;
#endif

#define VOID void

#define false   FALSE
#define MS_TRUE    TRUE

#define MS_DISABLE FALSE
#define MS_ENABLE  TRUE

#define MS_LOW     0
#define MS_HIGH    1

#define MS_OFF     0
#define MS_ON      1

#define ENUM(enum_var)   ((UINT8)enum_var)

/**< SDK return error type*/
typedef enum _ERR_TYPE_E
{
    MS_ERR_OK = 0,          /**< ok*/
    MS_ERR_FAIL,            /**< failed*/
    MS_ERR_INV_PARM         /**< parameter invalid */
} ERR_TYPE_E;

#ifdef MS_USE_ASSERT
#define MS_ASSERT_PARAM(expr) ((expr) ? (void)0 : ms_assert_failed((UINT8 *)__FILE__, __LINE__))
    void ms_assert_failed(UINT8* file, UINT32 line);
#else
    #define MS_ASSERT_PARAM(expr) ((void)0)
#endif /* MS_USE_ASSERT */

#define MS_ASSERT_PTR(ptr) do{if(!(ptr)){return MS_ERR_INV_PARM;}}while(0)

// Helper macros.
#define MS_UNUSED(arg)     ((arg) = (arg))

#ifndef MS_COUNTOF
#define MS_COUNTOF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#endif

#ifndef MS_MAX
#define MS_MAX(a, b)   (((a)>(b))?(a):(b))
#endif

#ifndef MS_MIN
#define MS_MIN(a, b)   (((a)<(b))?(a):(b))
#endif

#endif //__MS_SDK_TYPEDEF_H__

/**
 * @}
 */
