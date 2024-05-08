#ifndef __USART_H_
#define __USART_H_

#include <stdint.h>

typedef enum _emUartPortNum_t_
{
#if (_ENABLE_UART_1 == _MACRO_ENABLE)
    eUartNum1,
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
    eUartNum2,
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
    eUartNum3,
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
    eUartNum4,
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
    eUartNum5,
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
    eUartNum6,
#endif

    eUartNumMax
}emUartPortNum_t;

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
    #elif (defined(N32G45X))
    GPIO_Module*        pRxGpioPort;
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

#define  INT_DEFAULT_GROUP   			NVIC_PriorityGroup_4

#define RS232_printf 					Uart_printf
#if 0
#if (_DEBUG_PORT_SEL==_UART_PORT_1)
#define dbg_printf 						Uart1_printf
#elif (_DEBUG_PORT_SEL==_UART_PORT_2)
#define dbg_printf 						Uart2_printf
#elif (_DEBUG_PORT_SEL==_UART_PORT_3)
#define dbg_printf 						Uart3_printf
#elif (defined(STM32F2XX_HD)&&(_DEBUG_PORT_SEL==_UART_PORT_4))
#define dbg_printf 						Uart4_printf
#elif (defined(STM32F2XX_HD)&&(_DEBUG_PORT_SEL==_UART_PORT_5))
#define dbg_printf 						Uart5_printf
#elif (defined(STM32F2XX_HD)&&(_DEBUG_PORT_SEL==_UART_PORT_6))
#define dbg_printf 						Uart6_printf
#else
#define dbg_printf 						Uart_printf
#endif
#endif
#define DBG_printf(x) 					dbg_printf x

typedef void (*set_baudrate_t)(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
typedef void (*send_bytes_t)(uint8_t *, uint16_t, bool);
typedef void (*send_string_t)(char *, bool);
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
typedef int (*send_printf)(const char *fmt, ...);
#endif
#if _ENABLE_MULTI_COMMAND_MATCH_FUNCTION
typedef struct _stMultiCmdType_t_
{
    char    s8MultiCmd[100];
    BYTE    u8CmdCounter;
}stMultiCmdType_t;
#endif

typedef struct _stUartCom_t_
{
	set_baudrate_t 			SetBaudRate;
	send_bytes_t			SendBytes;
	send_string_t			SendString;
    #if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    send_printf             SendPrintf;
    #endif
	volatile uint32_t 		u32RecTimeout;//unit is 100us
	volatile bool 			bTimeOutValid;//cmd is valid or invalid when time out
	volatile uint16_t 		u16RecCnt;
	char					*pRecBuffer;
	volatile bool 			bRecCmdFlg;
#if _ENABLE_MULTI_COMMAND_MATCH_FUNCTION
    stMultiCmdType_t        stMultiCmdType[MaxCmdlist];
    bool                    bIsMultiCmdDetect;
    uint8_t                 u8MatchCmdCounter;
#endif
}stUartCom_t, *pstUartCom_t;

void USARTx_Configuration(pstUartDeclare_t pstUartPort);
void USARTx_send_bytes(pstUartDeclare_t pstUartPort, uint8_t *pu8Buffer, uint16_t u16Len);
void USARTx_send_string(pstUartDeclare_t pstUartPort, char *s);
#if (defined(N32G45X))
void USARTx_DirectSend_bytes(USART_Module * pstUart, uint8_t *pu8Buffer, uint16_t u16Len);
void USARTx_set_baudrate(USART_Module* USARTx,
                uint32_t USART_BaudRate,
                uint16_t USART_WordLength,
                uint16_t USART_StopBits,
                uint16_t USART_Parity,
                uint16_t USART_Mode,
                uint16_t USART_HardwareFlowControl);
#elif (defined(GD32F4XX))
void USARTx_DirectSend_bytes(uint32_t pstUart, uint8_t *pu8Buffer, uint16_t u16Len);
void USARTx_set_baudrate(uint32_t USARTx,
                uint32_t USART_BaudRate,
                uint16_t USART_WordLength,
                uint16_t USART_StopBits,
                uint16_t USART_Parity,
                uint16_t USART_Mode,
                uint16_t USART_HardwareFlowControl);

#else
void USARTx_DirectSend_bytes(USART_TypeDef * pstUart, uint8_t *pu8Buffer, uint16_t u16Len);
void USARTx_set_baudrate(USART_TypeDef* USARTx,
                uint32_t USART_BaudRate,
                uint16_t USART_WordLength,
                uint16_t USART_StopBits,
                uint16_t USART_Parity,
                uint16_t USART_Mode,
                uint16_t USART_HardwareFlowControl);
#endif

#endif



