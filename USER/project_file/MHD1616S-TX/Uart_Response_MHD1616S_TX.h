#ifndef __UART_RESPONSE_H__
#define __UART_RESPONSE_H__
#include "usart.h"

#define DEF_UART_DATA_CONFIG    USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None

#ifdef __UART_RESPONSE_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

extern stUartCom_t  g_stUart2CommVar, g_stUart1CommVar,g_stUart3CommVar;

INTERFACE void uart2_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart2_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart2_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
INTERFACE void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart1_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
INTERFACE void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
#if (defined(N32G45X))
INTERFACE void mdrv_USART_SendByte(USART_Module* USARTx, u16 Data);
#else
INTERFACE void mdrv_USART_SendByte(USART_TypeDef* USARTx, u16 Data);
#endif
INTERFACE void UartParserMethod(void);
INTERFACE void Uart1ParserMethod(void);
INTERFACE void uart3_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect);
INTERFACE void uart3_send_string_A(char *s, bool bFunSelect);
INTERFACE void uart3_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl);
INTERFACE void Uart3ParserMethod(void);


#undef INTERFACE
#endif

