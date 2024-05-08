#ifndef __UART_RESPONSE_H__
#define __UART_RESPONSE_H__
#include "usart.h"

#define DEF_UART_DATA_CONFIG    USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None

#ifdef __UART_RESPONSE_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

extern stUartCom_t			g_stUart1CommVar;
extern stUartCom_t			g_stUart2CommVar;
extern stUartCom_t			g_stUart3CommVar;
extern stUartCom_t			g_stUart4CommVar;
extern stUartCom_t			g_stUart5CommVar;
extern stUartCom_t			g_stUart6CommVar;

INTERFACE void Uart1ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart1ParserMethod(void);
INTERFACE void Uart1VarInit(void);
INTERFACE void Uart1VarTimeOut(void);
INTERFACE void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart1_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

INTERFACE void Uart2ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart2ParserMethod(void);
INTERFACE void Uart2VarInit(void);
INTERFACE void Uart2VarTimeOut(void);
INTERFACE void uart2_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart2_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart2_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

INTERFACE void Uart3ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart3ParserMethod(void);
INTERFACE void Uart3VarInit(void);
INTERFACE void Uart3VarTimeOut(void);
INTERFACE void uart3_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart3_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart3_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

INTERFACE void Uart4ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart4ParserMethod(void);
INTERFACE void Uart4VarInit(void);
INTERFACE void Uart4VarTimeOut(void);
INTERFACE void uart4_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart4_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart4_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

INTERFACE void Uart5ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart5ParserMethod(void);
INTERFACE void Uart5VarInit(void);
INTERFACE void Uart5VarTimeOut(void);
INTERFACE void uart5_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart5_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart5_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

INTERFACE void Uart6ProtocalDataReceive(uint8_t u8RecChar);
INTERFACE void Uart6ParserMethod(void);
INTERFACE void Uart6VarInit(void);
INTERFACE void Uart6VarTimeOut(void);
INTERFACE void uart6_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart6_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart6_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);

#if defined(GD32F4XX)
INTERFACE void mdrv_USART_SendByte(uint32_t USARTx, u16 Data);
#else
INTERFACE void mdrv_USART_SendByte(USART_TypeDef* USARTx, u16 Data);
#endif

#undef INTERFACE
#endif

