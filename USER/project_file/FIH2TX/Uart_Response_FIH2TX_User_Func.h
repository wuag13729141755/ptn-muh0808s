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

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
#define DISPLAY_ON_TEXT             "#SET_ON_"
#define DISPLAY_OFF_TEXT            "#SET_OF_"
#define DISPLAY_HEX_ON_TEXT         "#SET_H_ON_"
#define DISPLAY_HEX_OFF_TEXT        "#SET_H_OF_"

#define USER_CMD_DISPLAY_ON(buffer)         (strncmp(buffer,DISPLAY_ON_TEXT,strlen(DISPLAY_ON_TEXT))==0)
#define USER_CMD_DISPLAY_OFF(buffer)        (strncmp(buffer,DISPLAY_OFF_TEXT,strlen(DISPLAY_OFF_TEXT))==0)
#define USER_HEX_CMD_DISPLAY_ON(buffer)     (strncmp(buffer,DISPLAY_HEX_ON_TEXT,strlen(DISPLAY_HEX_ON_TEXT))==0)
#define USER_HEX_CMD_DISPLAY_OFF(buffer)    (strncmp(buffer,DISPLAY_HEX_OFF_TEXT,strlen(DISPLAY_HEX_OFF_TEXT))==0)

#define USER_CMD_HEAD(buffer)     (USER_CMD_DISPLAY_ON(buffer)\
                                    ||USER_CMD_DISPLAY_OFF(buffer)\
                                    ||USER_HEX_CMD_DISPLAY_ON(buffer)\
                                    ||USER_HEX_CMD_DISPLAY_OFF(buffer))
#endif


#if _ENABLE_SET_DISPALY_OFF_TIME    /*Ìí¼Ó2019.2.14*/
extern u8 g_DetectSetTimer;
extern u8 g_SwitchSetTimer;
#endif        /*Ìí¼Ó2019.2.14*/

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
extern bool StartEdidCostumFlag;
extern u8 g_u8UserEDIDIndex;
extern bool g_bInternalEdidFlag;
#if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
extern u8 g_u8InternalEDIDIndex;
#endif
#endif

#ifdef __UART_RESPON_USER_FUNC_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void mapp_UsartSystemCmdFun(pstUartCom_t pCommVar);

#if 0//_ENABLE_USER_EDID_LEARNING_FUNCTION
INTERFACE void mapp_UartUserEdidReceive(puart_com_t pUartCommVar);
#endif
#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
INTERFACE void mapp_UsartUserCmdFun(pstUartCom_t pCommVar);
INTERFACE bool mapp_UserUartLearningMessageFunction(u8 u8Index, pstUartCom_t pCommVar, u8 u8HeadIndex, bool bIsHex);
INTERFACE void UserHexDataSendFunction(u8 u8Port, u32 u32Baud, u16 u16Len, char *pData);
INTERFACE void UserDataSendFunction(u8 u8Port, u32 u32Baud, u16 u16Len, char *pData);
INTERFACE void MApp_SendPowerOnMessage(void);
#endif
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
INTERFACE void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar);
#endif
INTERFACE void mapp_SetPowerState(bool bPwrSta);
INTERFACE uint32_t mapp_GetRealBaudrate(uint8_t u8BaudrateID);
INTERFACE void KeyPrintTiming(bool bIsRxPort, u8 *pu8Timing);

#undef INTERFACE
#endif

