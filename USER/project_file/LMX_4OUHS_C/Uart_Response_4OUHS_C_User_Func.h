#ifndef __UART_RESPONSE_USER_FUNCTION_H__
#define __UART_RESPONSE_USER_FUNCTION_H__
#include "usart.h"

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
typedef enum _emUserEdidType_
{
    emUserEdid1 = 0,
    emUserEdid2,
    emUserEdid3,
    emUserEdid4,
    emUserEdidMax,
}emUserEdidType;
#endif


#if _ENABLE_USER_EDID_LEARNING_FUNCTION
extern bool StartEdidCostumFlag;
extern u8 g_u8UserEDIDIndex;
extern bool g_bInternalEdidFlag;
#endif

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

