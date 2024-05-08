#ifndef __UART_RESPONSE_USER_FUNCTION_H__
#define __UART_RESPONSE_USER_FUNCTION_H__
#include "usart.h"

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
typedef enum _emUserEdidType_
{
    emUserEdid1 = 0,
    emUserEdidMax,
}emUserEdidType;
#endif

typedef enum _uart_baud_type_t_
{
    baud_rate_115200 = 0,
    baud_rate_57600,
    baud_rate_38400,
    baud_rate_19200,
    baud_rate_9600,
    baud_rate_4800,
    baud_rate_2400,

    baud_rate_max,
}uart_baud_type_t;

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
void mapp_Usart1SystemCmdFun(pstUartCom_t pCommVar);

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
INTERFACE void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar);
#endif
INTERFACE void mapp_SetPowerState(bool bPwrSta);
INTERFACE uint32_t mapp_GetRealBaudrate(uint8_t u8BaudrateID);

#undef INTERFACE
#endif
