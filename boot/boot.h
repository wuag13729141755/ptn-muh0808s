/*****************************************************************************
Copyright       : PTN. Co., Ltd.
File name       : boot.h
Description     : 升级功能相关头文件声明
Author          : 靳凯凯
Version         : V1.0.0
Date            : 2021-6-23
History         :
*****************************************************************************/
#ifndef _BOOT_H
#define _BOOT_H

#include "Datatype.h"
//#include "debug_usart.h"
#include "copy_app.h"
#include "jump_app.h"



#if (defined(GD32F4XX))
typedef struct _stUartGpio_t_
{
    uint32_t            UARTx;
    uint32_t            pGpioPort;
	uint16_t			u16GpioTx;
    uint32_t            pRxGpioPort;
	uint16_t			u16GpioRx;
	uint32_t 			u32RccAhbPeriph_GpioTx;
	uint32_t 			u32RccAhbPeriph_GpioRx;
    uint8_t             u8AFConfig;
	uint32_t			u32RccApbPeriph;
	uint8_t				IRQChannel;
	uint8_t				IRQChannelPreemptionPriority;
	uint8_t 			IRQChannelSubPriority;
}stUartGpio_t, *pstUartGpio_t;

#else
typedef struct _stUartGpio_t_
{
#if (defined(N32G45X))
    USART_Module *      UARTx;
    GPIO_Module*        pGpioPort;
#else
	USART_TypeDef *		UARTx;
	GPIO_TypeDef*		pGpioPort;
#endif
	uint16_t			u16GpioTx;
    #ifdef STM32F2XX_HD
	GPIO_TypeDef*		pRxGpioPort;
    #endif
	uint16_t			u16GpioRx;
    #ifdef STM32F2XX_HD
	uint8_t 			u8GpioTxPinSrc;
	uint8_t 			u8GpioRxPinSrc;
	uint32_t 			u32RccAhbPeriph_Gpio;
    uint8_t             u8AFConfig;
	#endif
	uint32_t			u32RccApbPeriph;
	uint8_t				IRQChannel;
	uint8_t				IRQChannelPreemptionPriority;
	uint8_t 			IRQChannelSubPriority;
}stUartGpio_t, *pstUartGpio_t;
#endif

typedef enum _emUartNum_t_
{
    emUartNum_1 = 0,
    emUartNum_2,
    emUartNum_3,
    emUartNum_4,
    emUartNum_5,
    emUartNum_6,

    emUartNum_Max
}emUartNum_t;

int dbg_printf(const char *fmt, ...);

#endif

