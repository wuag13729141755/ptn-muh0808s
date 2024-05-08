#ifndef __UART_RESPONSE_H__
#define __UART_RESPONSE_H__
#include "usart.h"

#define DEF_UART_DATA_CONFIG    USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None

#ifdef __UART_RESPONSE_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif


#if (_ENABLE_UART_1 == _MACRO_ENABLE)
extern stUartCom_t			g_stUart1CommVar;
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
extern stUartCom_t			g_stUart2CommVar;
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
extern stUartCom_t			g_stUart3CommVar;
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
extern stUartCom_t			g_stUart4CommVar;
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
extern stUartCom_t			g_stUart5CommVar;
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
extern stUartCom_t			g_stUart6CommVar;
#endif

#if (_ENABLE_UART_1 == _MACRO_ENABLE)
INTERFACE void Uart1ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart1ParserMethod(void);
INTERFACE void Uart1VarInit(void);
INTERFACE void Uart1VarTimeOut(void);
INTERFACE void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart1_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#endif

#if (_ENABLE_UART_2 == _MACRO_ENABLE)
INTERFACE void Uart2ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart2ParserMethod(void);
INTERFACE void Uart2VarInit(void);
INTERFACE void Uart2VarTimeOut(void);
INTERFACE void uart2_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart2_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart2_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#endif

#if (_ENABLE_UART_3 == _MACRO_ENABLE)
INTERFACE void Uart3ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart3ParserMethod(void);
INTERFACE void Uart3VarInit(void);
INTERFACE void Uart3VarTimeOut(void);
INTERFACE void uart3_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart3_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart3_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#endif

#if (_ENABLE_UART_4 == _MACRO_ENABLE)
INTERFACE void Uart4ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart4ParserMethod(void);
INTERFACE void Uart4VarInit(void);
INTERFACE void Uart4VarTimeOut(void);
INTERFACE void uart4_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart4_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart4_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#endif

#if (_ENABLE_UART_5 == _MACRO_ENABLE)
INTERFACE void Uart5ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart5ParserMethod(void);
INTERFACE void Uart5VarInit(void);
INTERFACE void Uart5VarTimeOut(void);
INTERFACE void uart5_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart5_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart5_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#endif

#if (_ENABLE_UART_6 == _MACRO_ENABLE)
INTERFACE void Uart6ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart6ParserMethod(void);
INTERFACE void Uart6VarInit(void);
INTERFACE void Uart6VarTimeOut(void);
INTERFACE void uart6_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart6_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart6_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#endif

#if defined(GD32F4XX)
INTERFACE void mdrv_USART_SendByte(uint32_t USARTx, u16 Data);
#else
INTERFACE void mdrv_USART_SendByte(USART_TypeDef* USARTx, u16 Data);
#endif
int USER_printf(const char *fmt, ...);

#undef INTERFACE
#endif

