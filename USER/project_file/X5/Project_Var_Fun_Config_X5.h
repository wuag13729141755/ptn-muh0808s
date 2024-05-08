#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__
//#include "Av_config.h"

#define PrjStatePowerOnInit				mapp_EMX5RX_PowerOnInit
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#define StartupMessage 					mapp_EMX5RX_StartupMessage

#define SaveSpecificVarInit				EMX5RX_SpecificVarInit
#define SaveSpecificVarDefault          EMX5RX_SpecificVarDefault

/////==============================================================================
#define FACTORY_EDID_STAR   1
#define FACTORY_EDID_LAST   8
#define USER_EDID_STAR      9
#define USER_EDID_LAST      9
#define LOAD_EDID_LAST      9

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
    EDID_MODE_INTERNAL11,
    EDID_MODE_INTERNAL12,
    EDID_MODE_INTERNAL13,
    EDID_MODE_INTERNAL14,
    EDID_MODE_INTERNAL15,

    EDID_MODE_USER1 = USER_EDID_STAR,
    EDID_MODE_USER2,
    EDID_MODE_USER3,
    EDID_MODE_USER4,
    EDID_MODE_USER5,

    EDID_MODE_MAX,
    EDID_MODE_INTER_MAX = (EDID_MODE_INTERNAL1 +(FACTORY_EDID_LAST-FACTORY_EDID_STAR)),
    EDID_MODE_USER_MAX = (EDID_MODE_USER1 +(USER_EDID_LAST-USER_EDID_STAR)),
}emEdidModeType;

typedef enum _led_index_name_t_
{
    led_name_VDa0 = 0,
    led_name_VDb0,
    led_name_VDc0,
    led_name_VDd0,
    led_name_VDe0,
    led_name_VDf0,
    led_name_VDg0,
    led_name_VDCom1,
    led_name_VDCom2,
    led_name_VDCom3,
    led_name_VDCom4,
    led_name_ActiveLed,

//    led_name_End,
	led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_TxKeyLeft= 0,
    emKeyName_TxKeyRight,
    emKeyName_RxKeyLeft,
    emKeyName_RxKeyRight,

    emKeyName_max,
}key_counter_index_name_t;
#endif

typedef enum _TxHdcpActiveModeType_t_
{
    emTxHdcpActiveMode_BySrc = 0,
    emTxHdcpActiveMode_BySink,

    emTxHdcpActiveMode_Max,
}TxHdcpActiveModeType_t;

typedef enum _eSourceInputType_t_
{
    emSrcInput_Hdmi1 = 0,
    emSrcInput_Hdmi2,

    emSrcInput_Max,
}eSourceInputType_t;

typedef enum _emUserTimerType_t_
{
    emUserTimer_AutoSwitch = 0,
    emUserTimer_CheckDeviceType,
    emUserTimer_IDSetTimer,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _emMsgUartSelectType_t_
{
    emMsgUartType_Local = 0,
    emMsgUartType_Remote,
    emMsgUartType_LocalAndRemote,

    emMsgUartType_Max,
}emMsgUartSelectType_t;

typedef enum _emCmdToSigmastarType_t_
{
    emCmd2Sigma_Ok = 0,
    emCmd2Sigma_Error,
    emCmd2Sigma_SwitchInput,
    emCmd2Sigma_GetInputID,
    emCmd2Sigma_GetLocalID,
    emCmd2Sigma_SetLocalID,
    emCmd2Sigma_SetKeyLock,
    emCmd2Sigma_SetEncrypt,
    emCmd2Sigma_SetEdidData,
    emCmd2Sigma_SetUartDataBypass,
    emCmd2Sigma_SetIOMode,
    emCmd2Sigma_SetIOOutStatus,
    emCmd2Sigma_GetIOInputStatus,
    emCmd2Sigma_SendIRData,
    emCmd2Sigma_GetVersion,
    emCmd2Sigma_GetDeviceType,

    emCmd2Sigma_Max,
}emCmdToSigmastarType_t;

typedef enum _emIOModeType_T
{
    emIOMode_None = 0,
    emIOMode_IR_Receive,
    emIOMode_IR_Send,
    emIOMode_IO,
}emIOModeType_T;

typedef __packed struct _stGuiIp_t_
{
	uint8_t pu8IpContent[16];
	uint8_t u8ValidLen;
}stGuiIp_t, *pstGuiIp_t;

typedef __packed struct _stBasicSpecVar_t_
{
    bool			bPowerOnSta;
    u8              u8TXID;
    u8              u8RXID;
    bool            bKeyPadLock;
    u8              u8InputChannel;
    u8              u8RxEDID[256];
    u8              u8IOMode;

	uint8_t			u8CheckSum;
}stBasicSpecVar_t, *pstBasicSpecVar_t;

typedef __packed struct _stCustomEdid_t_
{
	uint8_t 		pu8EdidContext[256];

	uint8_t			u8CheckSum;
}stCustomEdid_t, *pstCustomEdid_t;

typedef struct _stProjectSpecVar_t_
{
	stBasicSpecVar_t	stBasSpecVar;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stEdidUpgrData_t_
{
	uint8_t 		u8EdidRecv[256];
	uint16_t		u16RecvLen;
}stEdidUpgrData_t, *pstEdidUpgrData_t;

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

extern bool g_bEnableSwitchHdcp;
extern bool g_bReAutherHdcp;
extern u8   g_u8DispID[4];

INTERFACE void EMX5RX_SpecificVarInit(void);
INTERFACE void EMX5RX_SpecificVarDefault(void);
INTERFACE void EMX5RX_SpecBasicVarDefault(void);
INTERFACE bool EMX5RX_SpecVarValidCheck(void);
INTERFACE void SaveUserData_SpecBasicVar(void);
INTERFACE void mapp_EMX5RX_PlatformTimerSet( uint8_t index, u32 m_sec );
INTERFACE u32 mapp_EMX5RX_PlatformTimerGet(uint8_t eTimer);
INTERFACE uint8_t mapp_EMX5RX_PlatformTimerExpired ( uint8_t timer );
INTERFACE void mapp_EMX5RX_PowerOnInit(void);
INTERFACE void mapp_EMX5RX_MainFunction(void);
INTERFACE void mapp_EMX5RX_StartupMessage(void);
INTERFACE void mapp_EMX5RX_SendCMDToSigmastar(u8 u8Cmd,u16 u16Len,pu8 pu8Data);
INTERFACE void mapp_EMX5RX_SetIDProcess(void);
INTERFACE void mapp_EMX5RX_SetLocalID(void);
INTERFACE void mapp_EMX5RX_SetInputID(void);
INTERFACE void mapp_SetIOMode(u8 u8Mode);
INTERFACE void mapp_IRLearnOkFunction(void);


#undef INTERFACE
#endif
