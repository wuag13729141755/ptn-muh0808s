#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__

#define SaveSpecificVarInit				F1H2RX_SpecificVarInit
#define EepSpecVarInvalid				F1H2RX_SpecVarValidCheck
#define PrjStatePowerOnInit				F1H2RX_PrjStatePowerOnInit
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage 					mapp_F1H2RXStartupMessage
#define SaveSpecificVarDefault          F1H2RX_SpecificVarDefault


typedef enum _led_index_name_t_
{
    led_name_ActiveLed= 0,

    led_name_End,
	led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_Source = 0,

    emKeyName_max,
}key_counter_index_name_t;
#endif

typedef enum _emUserTimerType_t_
{
    emUserTimer_LedActive = 0,
    emUserTimer_StartupTime,
    emUserTimer_TimingChange,
    emUserTimer_ReportTiming,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _emFpgaCmdType_t_
{
    eFpgaRegReportInputTiming   = 0x52,

    eFpgaRegOutTiming           = 0x20,
    eFpgaRegOutWinCfg           = 0x12,
    eFpgaRegOutUpdate           = 0x13,
    eFpgaRegAllPortBright       = 0x22,
    eFpgaRegTxReset             = 0xF9,
    eFpgaRegAudioSwitch         = 0x3B,

    eFpgaRegGetVersion          = 0x03,
}emFpgaCmdType_t;

typedef enum _emResolutionType_T_
{
    emRes_1920x1200_60 = 0,
    emRes_1920x1080_60,
    emRes_1280x720_60,
    emRes_1360x768_60,
    emRes_1280x1024_60,
    emRes_1024x768_60,
    emRes_1600x1200_60,
    emRes_1400x900_60,
    emRes_1600x900_60,
    emRes_1280x720_50,
    emRes_1920x1080_50,
    emRes_3840x2160_30,
    emRes_3840x2160_60,
    emRes_3840x2160_50,

    emRes_max,
}emResolutionType_T;

typedef enum _emHdmiTxFormat_t_
{
    emTxFormat_Hdmi = 0,
    emTxFormat_DVI,

    emTxFormat_max,
}emHdmiTxFormat_t;

typedef enum _eStartupLoopType_t_
{
    eSt_SInit,
    eSt_GetTxVersion,
    eSt_SetTxResolution,
    eSt_None,
}eStartupLoopType_t;

typedef __packed struct _stWindowsConfig_t_
{
    u8  u8SrcAddr;          // 原地址
    u8  u8LayerID;          // 指定图层
    u8  u8Layer_D0;         // 图层1    //input src 1
    u8  u8Layer_D1;         // 图层2    //input src 2
    u8  u8Layer_D2;         // 图层3    //input src 3
    u8  u8Layer_D3;         // 图层4    //input src 4
    u16 u16SrcAct_H;        // 源有效横向区域 H
    u16 u16SrcAct_V;        // 源有效纵向区域 V
    u16 u16Src_X;           // 视频源起始地址 X
    u16 u16Src_Y;           // 视频源起始地址 Y
    u16 u16Src_H;           // 视频源显示宽度 H
    u16 u16Src_V;           // 视频源显示高度 V
    u32 u32Wnd_X;           // 视频输出位置 X
    u32 u32Wnd_Y;           // 视频输出位置 Y
    u32 u32Wnd_H;           // 视频输出宽度 H
    u32 u32Wnd_V;           // 视频输出宽度 V
    u32 u32Srf_H;           // 视频横向缩放系数 H
    u32 u32Srf_V;           // 视频纵向缩放系数 V
    u8  u8SrcInput;         // 输入源地址
}stWindowsConfig_t, *pstWindowsConfig_t;

typedef struct _stWinTypeFactorType_t_
{
/**********************************
**StartX = (u8StX_sf1*Dst_H)/u8StX_sf2
**StartY = (u8StY_sf1*Dst_V)/u8StY_sf2
**Width  = (u8Width_sf1*Dst_H)/u8Width_sf2
**Height = (u8Height_sf1*Dst_V)/u8Height_sf2
**********************************/
    u8 u8StX_sf1;
    u8 u8StX_sf2;
    u8 u8StY_sf1;
    u8 u8StY_sf2;
    u8 u8Width_sf1;
    u8 u8Width_sf2;
    u8 u8Height_sf1;
    u8 u8Height_sf2;
}stWinTypeFactorType_t,*pstWinTypeFactorType_t;

typedef struct _stWindowType_t_
{
    u16 u16StartX;      //  start x of the window
    u16 u16StartY;      //  start y of the window
    u16 u16Width;       //  width of the window
    u16 u16Height;      //  height of the window

}stWindowType_t,*pstWindowType_t;

typedef struct _stWinConfigVar_t_
{
    u8 u8SrcPort;
    u8 u8WinNum;
    stWindowType_t stSrcWin;
    stWindowType_t stDstWin;
    stWindowType_t stClipWin;
    bool bEnClip;
    bool bEnWin;
}stWinConfigVar_t,*pstWinConfigVar_t;

typedef struct _stResolution_t_
{
    u16 u16Width;
    u16 u16Height;
    u16 u16VFreq;
    u8  u8ScanMode;
    bool bChangeFlag;
}stResolution_t;

typedef __packed struct _stBasicSpecVar_t_
{
	bool			bPowerOnSta;
	bool			bIsLock;
    uint8_t         u8TxResolutionID[Def_outport_num];
    uint8_t         u8TxFormatSta[Def_outport_num];
    uint8_t         u8TxHdcpMode[Def_outport_num];
    uint8_t         u8InputSrc;

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
	stCustomEdid_t		stCustomEdidPre1;
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

INTERFACE void F1H2RX_SpecificVarInit(void);
INTERFACE void F1H2RX_SpecBasicVarDefault(void);
INTERFACE void F1H2RX_SpecCustomEdidVarDefault(void);
INTERFACE bool F1H2RX_SpecVarValidCheck(void);
// the status when power on device
INTERFACE void F1H2RX_PrjStatePowerOnInit(void);
INTERFACE void F1H2RX_LedWork_Loop(void);
INTERFACE void F1H2RX_FuncWork_LoopRunning(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
INTERFACE void mapp_F1H2RXStartupMessage(void);

#undef INTERFACE
#endif
