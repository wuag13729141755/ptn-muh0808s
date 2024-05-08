#ifndef __UART_RESPONSE_USER_FUNCTION_H__
#define __UART_RESPONSE_USER_FUNCTION_H__
#include "usart.h"


#ifdef __UART_RESPON_USER_FUNC_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void mapp_UsartSystemCmdFun(pstUartCom_t pCommVar);
INTERFACE void mapp_MultiCmdMatchFunction(pstUartCom_t pCommVar);
INTERFACE void mapp_Usart1SystemCmdFun(pstUartCom_t pCommVar);

#undef INTERFACE
#endif

