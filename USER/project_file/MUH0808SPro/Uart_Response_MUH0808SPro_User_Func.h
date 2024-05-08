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

typedef enum _uart_baud_type_t_
{
    baud_rate_9600 = 1,
    baud_rate_19200,
    baud_rate_38400,
    baud_rate_57600,
    baud_rate_115200,

	baud_rate_max,
}uart_baud_type_t;

typedef enum _emOsdColorType_
{
    emOsdColor_None = 0,
    emOsdColor_White,
    emOsdColor_Red,
    emOsdColor_Green,
    emOsdColor_Blue,
    emOsdColor_Black,

	emOsdColor_Max,
}emOsdColorType;

typedef enum _emOsdPositionType_
{
    emOsdPos_None = 0,
    emOsdPos_LeftUp,
    emOsdPos_RightUp,
    emOsdPos_LeftDown,
    emOsdPos_RightDown,

	emOsdPos_Max,
}emOsdPositionType;


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
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
INTERFACE void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar);
#endif
uint32_t mapp_GetRealBaudrate(uint8_t u8BaudrateID);
INTERFACE void mapp_SetPowerState(bool bPwrSta);

#undef INTERFACE
#endif

