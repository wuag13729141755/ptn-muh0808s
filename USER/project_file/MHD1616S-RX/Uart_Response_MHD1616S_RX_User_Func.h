#ifndef __UART_RESPONSE_USER_FUNCTION_H__
#define __UART_RESPONSE_USER_FUNCTION_H__
#include "usart.h"

#define FACTORY_EDID_STAR   1
#define FACTORY_EDID_LAST   10
#define USER_EDID_STAR      11
#define USER_EDID_LAST      15
#define LOAD_EDID_LAST      15

typedef enum
{

    EDID_MODE_BYPASS = 0,

    EDID_MODE_INTERNAL1 = FACTORY_EDID_STAR,
    EDID_MODE_INTERNAL2,
    EDID_MODE_INTERNAL3,
    EDID_MODE_INTERNAL4,
    EDID_MODE_INTERNAL5,
    EDID_MODE_INTERNAL6,
    EDID_MODE_INTERNAL7,
    EDID_MODE_INTERNAL8,
    EDID_MODE_INTERNAL9,
    EDID_MODE_INTERNAL10,

    EDID_MODE_USER1 = USER_EDID_STAR,
    EDID_MODE_USER2,
    EDID_MODE_USER3,
    EDID_MODE_USER4,
    EDID_MODE_USER5,

    EDID_MODE_MAX,
    EDID_MODE_INTER_MAX = (EDID_MODE_INTERNAL1 +(FACTORY_EDID_LAST-FACTORY_EDID_STAR)),
    EDID_MODE_USER_MAX = (EDID_MODE_USER1 +(USER_EDID_LAST-USER_EDID_STAR)),
}emEdidModeType;

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
typedef enum _emUserEdidType_
{
    emUserEdid1 = 0,
    emUserEdid2,
    emUserEdid3,
    emUserEdid4,
    emUserEdid5,
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


extern u8 g_u8VersionReceive;

#ifdef __UART_RESPON_USER_FUNC_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void mapp_UsartSystemCmdFun(pstUartCom_t pCommVar);
INTERFACE void mapp_Usart2SystemCmdFun(pstUartCom_t pCommVar);

INTERFACE void mapp_SetPowerState(bool bPwrSta);

#undef INTERFACE
#endif

