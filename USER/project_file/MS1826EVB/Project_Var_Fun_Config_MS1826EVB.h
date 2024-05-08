#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__

#define SaveSpecificVarInit				MS1826EVB_SpecificVarInit
#define EepSpecVarInvalid				MS1826EVB_SpecVarValidCheck
#define PrjStatePowerOnInit				MS1826EVB_PrjStatePowerOnInit
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage 					mapp_MS1826EVBStartupMessage
#define SaveSpecificVarDefault          MS1826EVB_SpecificVarDefault

typedef enum
{
    EDID_MODE_NONE = 0,

    EDID_MODE_INTERNAL1_1024x768_60,
    EDID_MODE_INTERNAL2_1280x720_60,
    EDID_MODE_INTERNAL3_1360x768_60,
    EDID_MODE_INTERNAL4_1920x1200_60,
    EDID_MODE_INTERNAL5_1600x1200_60,
    EDID_MODE_INTERNAL6_1920x1080_60,

    EDID_MODE_USER1,
    EDID_MODE_USER2,
    EDID_MODE_USER3,
    EDID_MODE_USER4,

    EDID_MODE_COPYTX1,
    EDID_MODE_COPYTX2,
    EDID_MODE_COPYTX3,
    EDID_MODE_COPYTX4,

    EDID_MODE_MAX,
}emEdidModeType;

typedef enum _led_index_name_t_
{
    led_name_LedHdmiIn1 = 0,
    led_name_LedHdmiIn2,
    led_name_LedHdmiIn3,
    led_name_LedHdmiIn4,
    led_name_LedHdmiOut1,
    led_name_LedHdmiOut2,
    led_name_LedHdmiOut3,
    led_name_LedHdmiOut4,
    led_name_LedPwrOn,
    led_name_LedPwrOff,

    led_name_End,
	led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_HdmiMode1 = 0,
    emKeyName_HdmiMode2,
    emKeyName_HdmiMode3,
    emKeyName_HdmiMode4,
    emKeyName_Resolution,
    emKeyName_SplicingMode,
    emKeyName_AudioSwitch,
    emKeyName_WorkMode,
    emKeyName_Power,

    emKeyName_max,
}key_counter_index_name_t;
#endif

typedef enum _emUserTimerType_t_
{
    emUserTimer_LedActive = 0,
    emUserTimer_SwitchPortTime,
    emUserTimer_AutoSwitch,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _emResolutionType_T_
{
    emRes_1920x1200_60 = 0,
    emRes_1920x1080_60,
    emRes_1280x720_60,
    emRes_1360x768_60,
    emRes_1280x1024_60,
    emRes_1024x768_60,
    emRes_1600x1200_60,
    emRes_1440x900_60,
    emRes_1600x900_60,
    emRes_1280x720_50,
    emRes_1920x1080_50,
    emRes_3840x2160_30,
#if _ENABLE_RES_4096x2160_30
    emRes_4096x2160_30,
#endif

    emRes_max,
}emResolutionType_T;

typedef enum _emHdmiTxFormat_t_
{
    emTxFormat_Hdmi = 0,
    emTxFormat_DVI,

    emTxFormat_max,
}emHdmiTxFormat_t;

typedef __packed struct _stBasicSpecVar_t_
{
	bool			bPowerOnSta;
	bool			bIsDebugEn;
    uint8_t         u8TxResolutionID[Def_outport_num];
    uint8_t         u8TxFormatSta[Def_outport_num];
    uint8_t         u8TxHdcpMode[Def_outport_num];
    uint8_t         u8FreezeSta[Def_outport_num];
    uint8_t         u8Brightness[Def_outport_num];
    uint8_t         u8Contrast[Def_outport_num];
    uint8_t         u8Saturation[Def_outport_num];
    uint8_t         u8Hue[Def_outport_num];
    uint8_t         u8AudioMute[Def_outport_num];
    uint8_t         u8BlackScreenEnable[Def_outport_num];
    uint8_t         u8PatternSta[Def_outport_num];
    uint8_t         u8RedValue[Def_outport_num];
    uint8_t         u8GreenValue[Def_outport_num];
    uint8_t         u8BlueValue[Def_outport_num];
    uint16_t        u16SplicerX[Def_outport_num];
    uint16_t        u16SplicerY[Def_outport_num];
    uint16_t        u16SplicerWidth[Def_outport_num];
    uint16_t        u16SplicerHeight[Def_outport_num];

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
    stCustomEdid_t      stCustomEdidPre1;
    stCustomEdid_t      stCustomEdidPre2;
    stCustomEdid_t      stCustomEdidPre3;
    stCustomEdid_t      stCustomEdidPre4;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stEdidUpgrData_t_
{
	uint8_t 		u8EdidRecv[256];
	uint16_t		u16RecvLen;
}stEdidUpgrData_t, *pstEdidUpgrData_t;

extern stTimingTable_t g_stUserTimingData;


#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif
INTERFACE void GetResolutionText(char *ps8ResText,u8 u8ResID);
INTERFACE void MS1826EVB_SpecificVarInit(void);
INTERFACE void MS1826EVB_SpecBasicVarDefault(void);
INTERFACE void MS1826EVB_SpecCustomEdidVarDefault(void);
INTERFACE bool MS1826EVB_SpecVarValidCheck(void);
// the status when power on device
INTERFACE void MS1826EVB_PrjStatePowerOnInit(void);
INTERFACE void MS1826EVB_LedWork_Loop(void);
INTERFACE void MS1826EVB_FuncWork_LoopRunning(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
INTERFACE void mapp_MS1826EVBStartupMessage(void);
INTERFACE void app_MS1826EVBSetSplicerPixelDisplay(u8 u8Tx,u16 u16X,u16 u16Y,u16 u16Width,u16 u16Height);
INTERFACE void app_MS1826EVBSetSplicerDisplay(u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index);
INTERFACE void app_MS1826EVBPictureAdjust(u8 u8Tx,u8 u8Value,u8 u8Mode);

#undef INTERFACE
#endif
