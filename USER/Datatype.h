
#ifndef _DATATYPE_H_
#define _DATATYPE_H_

#ifdef STM32F2XX_HD
#include "Stm32f2xx.h"
#elif (defined(N32G45X))
#include "n32g45x.h"
#elif (defined(GD32F4XX))
#include "gd32f4xx.h"
#else
#include "Stm32f10x.h"
#endif

#ifdef __cplusplus
 extern "C" {
#endif

#ifndef BIT0
#define BIT0	0x0001
#endif
#ifndef BIT1
#define BIT1	0x0002
#endif
#ifndef BIT2
#define BIT2	0x0004
#endif
#ifndef BIT3
#define BIT3	0x0008
#endif
#ifndef BIT4
#define BIT4	0x0010
#endif
#ifndef BIT5
#define BIT5	0x0020
#endif
#ifndef BIT6
#define BIT6	0x0040
#endif
#ifndef BIT7
#define BIT7	0x0080
#endif
#ifndef BIT8
#define BIT8	0x0100
#endif
#ifndef BIT9
#define BIT9	0x0200
#endif
#ifndef BIT10
#define BIT10	0x0400
#endif
#ifndef BIT11
#define BIT11	0x0800
#endif
#ifndef BIT12
#define BIT12	0x1000
#endif
#ifndef BIT13
#define BIT13	0x2000
#endif
#ifndef BIT14
#define BIT14	0x4000
#endif
#ifndef BIT15
#define BIT15	0x8000
#endif
#ifndef BIT16
#define BIT16	0x10000
#endif
#ifndef BIT17
#define BIT17	0x20000
#endif
#ifndef BIT18
#define BIT18	0x40000
#endif
#ifndef BIT19
#define BIT19	0x80000
#endif
#ifndef BIT20
#define BIT20	0x100000
#endif
#ifndef BIT21
#define BIT21	0x200000
#endif
#ifndef BIT22
#define BIT22	0x400000
#endif
#ifndef BIT23
#define BIT23	0x800000
#endif
#ifndef BIT24
#define BIT24	0x1000000
#endif
#ifndef BIT25
#define BIT25	0x2000000
#endif
#ifndef BIT26
#define BIT26	0x4000000
#endif
#ifndef BIT27
#define BIT27	0x8000000
#endif
#ifndef BIT28
#define BIT28	0x10000000
#endif
#ifndef BIT29
#define BIT29	0x20000000
#endif
#ifndef BIT30
#define BIT30	0x40000000
#endif
#ifndef BIT31
#define BIT31	0x80000000
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#if defined(GD32F4XX)
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */
#endif

typedef unsigned char  BYTE, *PBYTE;
typedef unsigned short WORD, *PWORD;
typedef const BYTE *PCBYTE;
typedef short INT, *PINT, **PPINT;   // 16BIT
typedef unsigned  int DWORD, *PDWORD;              // 32BIT
typedef void VOID, *PVOID;

typedef char *ps8;
typedef unsigned char *pu8;
typedef short *ps16;
typedef unsigned short *pu16;
typedef int *ps32;
typedef unsigned int  *pu32;

typedef char CHAR, *PCHAR ;
typedef unsigned char uchar, *puchar ;
typedef unsigned char UCHAR, *PUCHAR ;
typedef unsigned char byte, *pbyte ;
//typedef unsigned char BYTE, *PBYTE ;

typedef short SHORT, *PSHORT ;
typedef unsigned short ushort, *pushort ;
typedef unsigned short USHORT, *PUSHORT ;
typedef unsigned short word, *pword ;
//typedef unsigned short WORD, *PWORD ;

typedef long LONG, *PLONG ;
typedef unsigned long ulong, *pulong ;
typedef unsigned long ULONG, *PULONG ;
typedef unsigned long dword, *pdword ;
//typedef unsigned long DWORD, *PDWORD ;

typedef unsigned int UINT, uint, *PUINT, *puint ;

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#define iTE_FALSE	    0
#define iTE_TRUE		1

#define HIGH            1
#define LOW             0

#define ITE_SUCCESS 	0
#define ITE_FAIL 		1

#define ON              1
#define OFF             0

#ifndef ITE_FALSE
#define ITE_FALSE   0
#endif
#ifndef ITE_TRUE
#define ITE_TRUE    1
#endif

#ifdef _MCU_8051_
	typedef bit iTE_u1 ;
	#define _CODE code
	#define _CODE_3K code
#else
	typedef unsigned char iTE_u1 ;
#if(IT663XX_VER == IT663XX_VER_1_07)
	#define _CODE       //const//__attribute__ ((section ("._OEM_BU1_RODATA ")))   // modify by wf8421 20180428
	#define _CODE_3K    //const//__attribute__ ((section ("._3K_RODATA ")))
#else
    #define _CODE       const//__attribute__ ((section ("._OEM_BU1_RODATA ")))   // modify by wf8421 20180428
	#define _CODE_3K    const//__attribute__ ((section ("._3K_RODATA ")))
#endif
#endif // _MCU_8051_

typedef enum _iTE_STATUS {
    iTE_SUCCESS = 0,
    iTE_FAIL
//    SYS_RESERVED
} iTE_STATUS;

#ifndef NULL
	#define NULL ((void *) 0)
#endif

typedef char iTE_s8, *iTE_ps8;
typedef unsigned char iTE_u8, *iTE_pu8;
typedef short iTE_s16, *iTE_ps16;
typedef unsigned short iTE_u16, *iTE_pu16;


#ifndef _MCU_8051_				// IT6350
typedef unsigned int iTE_u32, *iTE_pu32;
typedef int iTE_s32, *iTE_ps32;
#else
typedef unsigned long iTE_u32, *iTE_pu32;
typedef long iTE_s32, *iTE_ps32;
#endif

#if (defined(N32G45X))
typedef enum {FALSE = 0, TRUE = !FALSE} bool_t;
#else
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#endif

#ifndef false
#define false FALSE
#endif

#ifndef true
#define true TRUE
#endif

#if (!defined(N32G45X))
typedef bool bool_t;
#endif
typedef bool u1;
typedef bool BOOL;

#define FLG_SHORT               0x01    // Used for Ri Short Read
#define FLG_NOSTOP              0x02    // Don't release IIC Bus
#define FLG_CONTD               0x04    // Continued from previous operation
#define FLG_WADDR               0x08    // reg addr is word type

// command list Flag define
//#define UserOffset      255
//#define MaxCmdNum       255
//#define UseStrlen       0
//#define NoString        NULL

//#define Flag_End        0
//#define Flag_BackCodeNomal  0x01
//#define Flag_BackCodeValue  0x02
//#define Flag_BackCodeFunc   0x04
//#define Flag_ShowHelpCommand    0x08

#define PROG		(1)
#define INTR		(0)
#define Vneg		(0)
#define Hneg		(0)
#define Vpos		(1)
#define Hpos		(1)

// TM1623 USE
#define SEG1        BIT0
#define SEG2        BIT1
#define SEG3        BIT2
#define SEG4        BIT3
#define SEG5        BIT4
#define SEG6        BIT5
#define SEG7        BIT6
#define SEG8        BIT7
#define SEG9        BIT0
#define SEG10       BIT1
#define SEG11       BIT2
#define SEG12       BIT3
#define SEG13       BIT4
#define SEG14       BIT5

#define KS1_K1      BIT0
#define KS1_K2      BIT1
#define KS1_K3      BIT2
#define KS2_K1      BIT3
#define KS2_K2      BIT4
#define KS2_K3      BIT5
#define KS3_K1      BIT0
#define KS3_K2      BIT1
#define KS3_K3      BIT2
#define KS4_K1      BIT3
#define KS4_K2      BIT4
#define KS4_K3      BIT5
#define KS5_K1      BIT0
#define KS5_K2      BIT1
#define KS5_K3      BIT2
#define KS6_K1      BIT3
#define KS6_K2      BIT4
#define KS6_K3      BIT5
#define KS7_K1      BIT0
#define KS7_K2      BIT1
#define KS7_K3      BIT2
#define KS8_K1      BIT3
#define KS8_K2      BIT4
#define KS8_K3      BIT5
#define KS9_K1      BIT0
#define KS9_K2      BIT1
#define KS9_K3      BIT2
#define KS10_K1     BIT3
#define KS10_K2     BIT4
#define KS10_K3     BIT5

#if (defined(N32G45X))
#define GPIO_Pin_0          GPIO_PIN_0
#define GPIO_Pin_1          GPIO_PIN_1
#define GPIO_Pin_2          GPIO_PIN_2
#define GPIO_Pin_3          GPIO_PIN_3
#define GPIO_Pin_4          GPIO_PIN_4
#define GPIO_Pin_5          GPIO_PIN_5
#define GPIO_Pin_6          GPIO_PIN_6
#define GPIO_Pin_7          GPIO_PIN_7
#define GPIO_Pin_8          GPIO_PIN_8
#define GPIO_Pin_9          GPIO_PIN_9
#define GPIO_Pin_10         GPIO_PIN_10
#define GPIO_Pin_11         GPIO_PIN_11
#define GPIO_Pin_12         GPIO_PIN_12
#define GPIO_Pin_13         GPIO_PIN_13
#define GPIO_Pin_14         GPIO_PIN_14
#define GPIO_Pin_15         GPIO_PIN_15
#define USART_WordLength_8b                 USART_WL_8B
#define USART_WordLength_9b                 USART_WL_9B
#define USART_StopBits_1                    USART_STPB_1
#define USART_StopBits_0_5                  USART_STPB_0_5
#define USART_StopBits_2                    USART_STPB_2
#define USART_StopBits_1_5                  USART_STPB_1_5
#define USART_Parity_No                     USART_PE_NO
#define USART_Parity_Even                   USART_PE_EVEN
#define USART_Parity_Odd                    USART_PE_ODD
#define USART_Mode_Rx                       USART_MODE_RX
#define USART_Mode_Tx                       USART_MODE_TX
#define USART_HardwareFlowControl_None      USART_HFCTRL_NONE
#define USART_HardwareFlowControl_RTS       USART_HFCTRL_RTS
#define USART_HardwareFlowControl_CTS       USART_HFCTRL_CTS
#define USART_HardwareFlowControl_RTS_CTS   USART_HFCTRL_RTS_CTS
#elif (defined(GD32F4XX))
#define GPIO_Mode_OUT       GPIO_MODE_OUTPUT
#define GPIO_Mode_IN        GPIO_MODE_INPUT
#define GPIO_Mode_AF        GPIO_MODE_AF
#define GPIO_Mode_AN        GPIO_MODE_ANALOG
#define GPIO_PuPd_NOPULL    GPIO_PUPD_NONE
#define GPIO_PuPd_UP        GPIO_PUPD_PULLUP
#define GPIO_PuPd_DOWN      GPIO_PUPD_PULLDOWN
#define GPIO_OType_PP       GPIO_OTYPE_PP
#define GPIO_OType_OD       GPIO_OTYPE_OD
#define GPIO_Pin_0          GPIO_PIN_0
#define GPIO_Pin_1          GPIO_PIN_1
#define GPIO_Pin_2          GPIO_PIN_2
#define GPIO_Pin_3          GPIO_PIN_3
#define GPIO_Pin_4          GPIO_PIN_4
#define GPIO_Pin_5          GPIO_PIN_5
#define GPIO_Pin_6          GPIO_PIN_6
#define GPIO_Pin_7          GPIO_PIN_7
#define GPIO_Pin_8          GPIO_PIN_8
#define GPIO_Pin_9          GPIO_PIN_9
#define GPIO_Pin_10         GPIO_PIN_10
#define GPIO_Pin_11         GPIO_PIN_11
#define GPIO_Pin_12         GPIO_PIN_12
#define GPIO_Pin_13         GPIO_PIN_13
#define GPIO_Pin_14         GPIO_PIN_14
#define GPIO_Pin_15         GPIO_PIN_15
#define USART_WordLength_8b                 USART_WL_8BIT
#define USART_WordLength_9b                 USART_WL_9BIT
#define USART_StopBits_1                    USART_STB_1BIT
#define USART_StopBits_0_5                  USART_STB_0_5BIT
#define USART_StopBits_2                    USART_STB_2BIT
#define USART_StopBits_1_5                  USART_STB_1_5BIT
#define USART_Parity_No                     USART_PM_NONE
#define USART_Parity_Even                   USART_PM_EVEN
#define USART_Parity_Odd                    USART_PM_ODD
#define USART_Mode_Rx                       USART_RECEIVE_ENABLE
#define USART_Mode_Tx                       USART_TRANSMIT_ENABLE
#define USART_HardwareFlowControl_None      (USART_RTS_DISABLE&USART_CTS_DISABLE)
#define USART_HardwareFlowControl_RTS       USART_RTS_ENABLE
#define USART_HardwareFlowControl_CTS       USART_CTS_ENABLE
#define USART_HardwareFlowControl_RTS_CTS   (USART_RTS_ENABLE|USART_CTS_ENABLE)
#define Bit_SET                             SET
#define Bit_RESET                           RESET
#define GPIO_WriteBit                       gpio_bit_write
#define GPIO_SetBits                        gpio_bit_set
#define GPIO_ResetBits                      gpio_bit_reset
#define GPIO_ReadOutputDataBit              gpio_output_bit_get
#define GPIO_ReadOutputData                 gpio_output_port_get
#define GPIO_ReadInputDataBit               gpio_input_bit_get
#define GPIO_ReadInputData                  gpio_input_port_get
#define GPIO_Write                          gpio_port_write
//#define GPIO_SetBits                        gpio_bit_set
//#define GPIO_SetBits                        gpio_bit_set
#endif

#ifndef abs
#define abs(x) (((x)>=0)?(x):(-(x)))
#endif

#define VERSION_NUM(a,b,c)((((a)&0x7)<<5)|(((b)&0x3)<<3)|((c)&0x7)) //Ver a.b.c, a - bit[7:5], b - bit[4:3], c - bit[2:0]
#define VERSION_STR(a)((((a[1]-'0')&0x7)<<5)|(((a[3]-'0')&0x3)<<3)|((a[5]-'0')&0x7))// V1.0.0

#ifdef __cplusplus
}
#endif

#endif

