///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <platform.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//#include <stdio.h>
//#include "..\..\IT6350\code\include.h"
//#include "..\..\IT6350\code\api\debug\debug_print.h"
/*
typedef char s8 ;
typedef unsigned char u8 ;
typedef unsigned short u16;
typedef unsigned long u32;

#ifndef __cplusplus
#ifndef bool
typedef unsigned char bool ;
#endif
#endif
*/
//#define CONST const

//#ifndef true
//#define true 1
//#endif

//#ifndef false
//#define false 0
//#endif
typedef unsigned long __tick;

//
//  assign the print function
//
#define pr_err    Uart_printf
#define pr_info   Uart_printf
#define pr_info2  Uart_printf


//---------- for CEC
/*
#define iTE_FALSE 0
#define iTE_TRUE  1

#ifdef _MCU_8051_
	typedef bit iTE_u1 ;
	#define _CODE code
	#define _CODE_3K code

#elif defined (WIN32)
	typedef int iTE_u1 ;
	#define _CODE const
	#define _CODE_3K const

#elif defined (_MCU_IT6350_)
	typedef unsigned char iTE_u1 ;
	#define _CODE  __attribute__ ((section ("._OEM_BU1_RODATA ")))
	#define _CODE_3K __attribute__ ((section ("._3K_RODATA ")))

#elif defined (__WIN32__)
	typedef unsigned char iTE_u1 ;
	#define _CODE const
	#define _CODE_3K const

#else
	#error("Please define this section by your platform")
	typedef int iTE_u1 ;
	#define _CODE
	#define _CODE_3K
#endif // _MCU_8051_
*/

// output TXOE state on JP47 (GPC5)
// by nVidia's clock detect request
#define REPORT_TXOE_0(x) //{ GPDRC &= ~0x20;} //GPC5=0;
#define REPORT_TXOE_1(x) //{ GPDRC |= 0x20; } //GPC5=1;

//typedef char iTE_s8, *iTE_ps8;
//typedef unsigned char iTE_u8, *iTE_pu8;
//typedef short iTE_s16, *iTE_ps16;
//typedef unsigned short iTE_u16, *iTE_pu16;

//------------------

#endif




