#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__

#define SaveSpecificVarInit				SCU41_MV_SpecificVarInit
#define EepSpecVarInvalid				SCU41_MV_SpecVarValidCheck
#define PrjStatePowerOnInit				SCU41_MV_PrjStatePowerOnInit
#define SwitcherSuspend_LoopRunning		SCU41_MV_SuspendLoopRunning
#define SwitcherPrj_LoopRunning			SCU41_MV_FuncWork_LoopRunning
#define mapp_SetPowerDownFunction       SCU41_MV_SetPowerDownFunction
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_NewKeyScan
#endif
#define StartupMessage 					mapp_SCU41_MVStartupMessage
#define SaveSpecificVarDefault          SCU41_MV_SpecificVarDefault
//#define mapp_SetTypeCResetPinFunction   SCU41_MV_SetTypeCResetPinFunction

#define FACTORY_EDID_STAR   1
#define FACTORY_EDID_LAST   3
#define USER_EDID_STAR      4
#define USER_EDID_LAST      4
#define LOAD_EDID_LAST      4


#define FPGA_REG_RES        0x20
#define FPGA_REG_WIN        0x12
#define FPGA_REG_UPD        0x13
#define FPGA_REG_ADSRC      0x30
#define FPGA_REG_ADMIX      0x31
#define FPGA_REG_ADMUTE     0x32

typedef enum
{
    #if 0//(_DISABLE_EDID_BYPASS == DISABLE)
    EDID_MODE_BYPASS = 0,
    #endif

    EDID_MODE_INTERNAL1 = FACTORY_EDID_STAR,
    EDID_MODE_INTERNAL2,
    EDID_MODE_INTERNAL3,
    //EDID_MODE_INTERNAL4,
    //EDID_MODE_INTERNAL5,
    //EDID_MODE_INTERNAL6,
    //EDID_MODE_INTERNAL7,
    //EDID_MODE_INTERNAL8,
    //EDID_MODE_INTERNAL9,
    //EDID_MODE_INTERNAL10,

    EDID_MODE_USER1 = USER_EDID_STAR,
    EDID_MODE_USER2,
    EDID_MODE_USER3,
    EDID_MODE_USER4,
    //EDID_MODE_USER5,

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

typedef enum _emMultiViewModeType_t_
{
    emMvMode_1Win_Full=0,
    emMvMode_2Win_PBP,
    emMvMode_3Win_2U1D,
    emMvMode_4Win_SameSize,
    emMvMode_2Win_PIP_LU,
    emMvMode_2Win_PIP_LD,
    emMvMode_2Win_PIP_RU,
    emMvMode_2Win_PIP_RD,
    emMvMode_4Win_PBP_3L1R,
    emMvMode_4Win_PBP_1L3R,
    emMvMode_4Win_PBP_3U1D,     // 10
    emMvMode_4Win_PBP_1U3D,
    emMvMode_4Win_PIP_1F3L,
    emMvMode_4Win_PIP_1F3R,
    emMvMode_4Win_PIP_1F3U,
    emMvMode_4Win_PIP_1F3D,
    emMvMode_xWin_ByUser1,
    emMvMode_xWin_ByUser2,
    emMvMode_xWin_ByUser3,
    emMvMode_xWin_ByUser4,

    emMvMode_max,
}emMultiViewModeType_t;

typedef enum _emResizeModeType_t_
{
    emResize_None = 0,
    emResize_2Win_PBP_1,
    emResize_2Win_PBP_2,
    emResize_2Win_PBP_3,
    emResize_2Win_PIP_LU_1,
    emResize_2Win_PIP_LU_2,
    emResize_2Win_PIP_LD_1,
    emResize_2Win_PIP_LD_2,
    emResize_2Win_PIP_RU_1,
    emResize_2Win_PIP_RU_2,
    emResize_2Win_PIP_RD_1,     // 10
    emResize_2Win_PIP_RD_2,
    emResize_4Win_PBP_3L1R_1,
    emResize_4Win_PBP_3L1R_2,
    emResize_4Win_PBP_1L3R_1,
    emResize_4Win_PBP_1L3R_2,
    emResize_4Win_PBP_3U1D_1,
    emResize_4Win_PBP_3U1D_2,
    emResize_4Win_PBP_1U3D_1,
    emResize_4Win_PBP_1U3D_2,

    emResize_MaxNum,
}emResizeModeType_t;

typedef enum _emWindowsNumType_t_
{
    emWinNum_1 = 0,
    emWinNum_2,
    emWinNum_3,
    emWinNum_4,

    emWinNumMax,
}emWindowsNumType_t;

typedef enum _emWindowsDisplayType_t_
{
    emWinType_A = 0,
    emWinType_B,
    emWinType_C,
    emWinType_D,

    emWinTypeMax,
}emWindowsDisplayType_t;

typedef enum _emSourceInputType_t_
{
    emSrcInput_Hdmi1 = 0,
    emSrcInput_Hdmi2,
    emSrcInput_Hdmi3,
    emSrcInput_Hdmi4,

    emSrcInputMax,
}emSourceInputType_t;

typedef enum _emWinLedLightMode_t_
{
    emWinLightMode_Off = 0,
    emWinLightMode_On,
    emWinLightMode_NomalBlink,
    emWinLightMode_FastBlink,
    emWinLightMode_SlowBlink,
    #if _ENABLE_LED_WORK_MOMENT_3S
    emWinLightMode_Moment_on_3s,
    emWinLightMode_Moment_off_3s,
    #endif

    emWinLightMode_max,
}emWinLedLightMode_t;

typedef enum _led_index_name_t_
{
    led_name_power_red = 0,
    led_name_power_Green,
    led_name_hdmi_input_yellow_1,
    led_name_hdmi_input_blue_1,
    led_name_hdmi_input_yellow_2,
    led_name_hdmi_input_blue_2,
    led_name_hdmi_input_yellow_3,
    led_name_hdmi_input_blue_3,
    led_name_hdmi_input_yellow_4,
    led_name_hdmi_input_blue_4,
    led_name_switch_auto,
    led_name_window_A,
    led_name_window_B,
    led_name_window_C,
    led_name_window_D,
    led_name_swap,
    led_name_resize,
    led_name_multiview_mode_1,
    led_name_multiview_mode_2,
    led_name_multiview_mode_3,

    led_name_End,
	led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_Input1 = 0,
    emKeyName_Input2,
    emKeyName_Input3,
    emKeyName_Input4,
    emKeyName_Auto,
    emKeyName_WinA,
    emKeyName_WinB,
    emKeyName_WinC,
    emKeyName_WinD,
    emKeyName_Swap,
    emKeyName_Resize,
    emKeyName_MVMode1,
    emKeyName_MVMode2,
    emKeyName_MVMode3,

    emKeyName_max,
}key_counter_index_name_t;
#endif

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
typedef enum _uart_study_key_type_t_
{
    uart_study_key_type_dis_on = 0,
    uart_study_key_type_dis_off,
    //uart_study_key_type_volume_mute,
    //uart_study_key_type_volume_up,
    //uart_study_key_type_volume_down,

    uart_study_key_type_max,
}uart_study_key_type_t;
#endif

typedef enum _emResolutionType_T_
{
    emRes_1024x768_60 = 0,
    emRes_1280x720_60,
    emRes_1360x768_60,
    emRes_1600x1200_60,
    emRes_1920x1080_60,
    emRes_1920x1200_60,
    emRes_3840x2160_30,     // 6

#if _ENABLE_RES_1600x900_60
    emRes_1600x900_60,
#endif
#if _ENABLE_RES_1600x900_60
    emRes_1680x1050_60,
#endif
#if _ENABLE_RES_1400x1050_60
    emRes_1400x1050_60,
#endif
#if _ENABLE_RES_1280x800_60
    emRes_1280x800_60,      // 10
#endif
#if _ENABLE_RES_1280x720_50
    emRes_1280x720_50,
#endif
#if _ENABLE_RES_1920x1080_50
    emRes_1920x1080_50,
#endif

#if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
    emRes_BestByDisplay,
#endif

    emRes_max,
}emResolutionType_T;

typedef enum _emCfgModeType_t_
{
    emCfgMode_None = 0,
    emCfgMode_Percent,
    emCfgMode_Pixel,

    emCfgModeMax,
}emCfgModeType_t;

typedef __packed struct _stGuiIp_t_
{
	uint8_t pu8IpContent[16];
	uint8_t u8ValidLen;
}stGuiIp_t, *pstGuiIp_t;

typedef __packed struct _stUserWindowsConfig_t_
{
    uint8_t u8UserSrc[4];
    uint8_t u8StartX[4];
    uint8_t u8StartY[4];
    uint8_t u8EndX[4];
    uint8_t u8EndY[4];
    uint8_t u8EnWinSta[4];
}stUserWindowsConfig_t,*pstUserWindowsConfig_t;

typedef __packed struct _stUserWinPixelCfg_t_
{
    uint8_t u8UserSrc[4];
    uint16_t u16StartX[4];
    uint16_t u16StartY[4];
    uint16_t u16Width[4];
    uint16_t u16Heigh[4];
    uint8_t u8EnWinSta[4];
}stUserWinPixelCfg_t,*pstUserWinPixelCfg_t;

typedef __packed struct _stBasicSpecVar_t_
{
	bool			bPowerOnSta;
	bool			bIsLock;
	bool			bIsSpdifOutOn;
	bool			bIsIISOutOn;
    uint8_t         u8baudrateId;

    #if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
    bool    bActionToSendCecFlag;
    bool    bActionToSendRs232CmdFlag;
    u16     u16DisplayTime;         // 1s unit
    #if _ENABLE_AUTO_POWER_FUNCTION
    bool    bActionToAutoPowerOnOffFlag;
    #endif
    #endif
    uint8_t         u8MvMode;
    uint8_t         u8WinSel[4];
    uint8_t         u8ResizeMode;
    uint8_t         u8SwapMode;
    uint8_t         u8WindowNum;
    uint8_t         u8ResolutionID;
    uint8_t         u8AudioSel;
    bool            bAudioMute;
    bool            bAudioMix;
    uint8_t         u8HdcpMode;
    #if _ENABLE_SWITCH_PORT_AND_AUDIO
    bool            bFullModeToSwitchAudio;
    #endif
    stUserWindowsConfig_t   stUserWinsConfig[4];

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
	stCustomEdid_t		stCustomEdidPre2;
	stCustomEdid_t		stCustomEdidPre3;
	stCustomEdid_t		stCustomEdidPre4;
	//stCustomEdid_t		stCustomEdidPre5;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stEdidUpgrData_t_
{
	uint8_t 		u8EdidRecv[256];
	uint16_t		u16RecvLen;
}stEdidUpgrData_t, *pstEdidUpgrData_t;

typedef enum _emWinDataIndex_t_
{
    WIN_Src             = (0),
    WIN_Port            = (1),
    WIN_Layer_D0        = (2),
    WIN_Layer_D1        = (3),
    WIN_Layer_D2        = (4),
    WIN_Layer_D3        = (5),
    WIN_HSrc_H          = (6),
    WIN_HSrc_L          = (7),
    WIN_VSrc_H          = (8),
    WIN_VSrc_L          = (9),
    WIN_XSrc_H          = (10),
    WIN_XSrc_L          = (11),
    WIN_YSrc_H          = (12),
    WIN_YSrc_L          = (13),
    WIN_HSrcClip_H      = (14),
    WIN_HSrcClip_L      = (15),
    WIN_VSrcClip_H      = (16),
    WIN_VSrcClip_L      = (17),
    WIN_XDisplay_H      = (18),
    WIN_XDisplay_L      = (19),
    WIN_YDisplay_H      = (20),
    WIN_YDisplay_L      = (21),
    WIN_HDisplay_H      = (22),
    WIN_HDisplay_L      = (23),
    WIN_VDisplay_H      = (24),
    WIN_VDisplay_L      = (25),
    WIN_HScaler_1       = (26),
    WIN_HScaler_2       = (27),
    WIN_HScaler_3       = (28),
    WIN_HScaler_4       = (29),
    WIN_VScaler_1       = (30),
    WIN_VScaler_2       = (31),
    WIN_VScaler_3       = (32),
    WIN_VScaler_4       = (33),
}emWinDataIndex_t;

typedef struct _stWindowsConfig_t_
{
    u8  u8Src;              // 原地址
    u8  u8Port;             // 通道
    u8  u8Layer_D0;         // 图层1    //input src 1
    u8  u8Layer_D1;         // 图层2    //input src 2
    u8  u8Layer_D2;         // 图层3    //input src 3
    u8  u8Layer_D3;         // 图层4    //input src 4
    u16 u16Src_H;           // 源有效横向区域 H
    u16 u16Src_V;           // 源有效纵向区域 V
    u16 u16Src_X;           // 视频源起始地址 X
    u16 u16Src_Y;           // 视频源起始地址 Y
    u16 u16SrcClip_H;       // 视频切割宽度 H
    u16 u16SrcClip_V;       // 视频切割高度 V
    u16 u16Display_X;       // 视频输出位置 X
    u16 u16Display_Y;       // 视频输出位置 Y
    u16 u16Display_H;       // 视频输出宽度 H
    u16 u16Display_V;       // 视频输出宽度 V
    u32 u32Scaler_H;        // 视频横向缩放系数 H
    u32 u32Scaler_V;        // 视频纵向缩放系数 V
}stWindowsConfig_t, *pstWindowsConfig_t;

typedef struct _stTimingConfig_t_
{
    u8 u8HFP_H;             // 水平消隐前肩 H
    u8 u8HFP_L;             // 水平消隐前肩 L
    u8 u8HPol;              // 水平同步极性
    u8 u8HSync;             // 水平同步宽度
    u8 u8HBP_H;             // 水平消隐后肩 H
    u8 u8HBP_L;             // 水平消隐后肩 L
    u8 u8HActive_H;         // 水平显示宽度 H
    u8 u8HActive_L;         // 水平显示宽度 L
    u8 u8HTotal_H;          // 水平信号总宽度 H
    u8 u8HTotal_L;          // 水平信号总宽度 L
    u8 u8VFP_H;             // 垂直消隐前肩 H
    u8 u8VFP_L;             // 垂直消隐前肩 L
    u8 u8VPol;              // 垂直同步极性
    u8 u8VSync;             // 垂直同步宽度
    u8 u8VBP_H;             // 垂直消隐后肩 H
    u8 u8VBP_L;             // 垂直消隐后肩 L
    u8 u8VActive_H;         // 垂直显示宽度 H
    u8 u8VActive_L;         // 垂直显示宽度 L
    u8 u8VTotal_H;          // 垂直信号总宽度 H
    u8 u8VTotal_L;          // 垂直信号总宽度 L
}stTimingConfig_t, *pstTimingConfig_t;

typedef struct _stResolutionTable_t_
{
    u16 u16HFP;             // 水平消隐前肩
    u8  u8HPol;             // 水平同步极性
    u8  u8HSync;            // 水平同步宽度
    u16 u16HBP;             // 水平消隐后肩
    u16 u16HActive;         // 水平显示宽度
    u16 u16HTotal;          // 水平信号总宽度
    u16 u16VFP;             // 垂直消隐前肩
    u8  u8VPol;             // 垂直同步极性
    u8  u8VSync;            // 垂直同步宽度
    u16 u16VBP;             // 垂直消隐后肩
    u16 u16VActive;         // 垂直显示宽度
    u16 u16VTotal;          // 垂直信号总宽度
    u8  u8Vic;
    u16 u16PClk;                // u16PClk(KHz) = (pixel clk )/1000
    u8  u8VClk;
}stResolutionTable_t,*pstResolutionTable_t;

typedef struct _stWindowType_t_
{
    u16 u16StartX;      //  start x of the window
    u16 u16StartY;      //  start y of the window
    u16 u16Width;       //  width of the window
    u16 u16Height;      //  height of the window

}stWindowType_t,*pstWindowType_t;

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

extern const stResolutionTable_t TimingTable[];
extern stWinConfigVar_t   g_stWinConfigVar[4];
extern stResolutionTable_t  g_stTiming;

#define max_Resolution emRes_max

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void SCU41_MV_SpecificVarInit(void);
INTERFACE void SCU41_MV_SpecBasicVarDefault(void);
INTERFACE void SCU41_MV_SpecCustomEdidVarDefault(void);
INTERFACE bool SCU41_MV_SpecVarValidCheck(void);
INTERFACE void SCU41_MV_AutoSw_HookFun(uint8_t u8Index);
// the status when power on device
INTERFACE void SCU41_MV_PrjStatePowerOnInit(void);
INTERFACE void SCU41_MV_TxPlugInOrOutPro_Loop(void);
INTERFACE void SCU41_MV_LedWork_Loop(void);
INTERFACE void SCU41_MV_SpdifIisWork_Loop(void);
INTERFACE u8 mapp_SCU41MVGetEdidIndex(u8 u8InPort);
INTERFACE void SCU41_MV_EDIDAssignByID(uint8_t u8ID,u8 u8InPort);
INTERFACE void SetSCU41_MVEdidUpdCnt(uint32_t u32Cnt);
INTERFACE uint32_t PollSCU41_MVEdidUpdCnt(void);
INTERFACE void SCU41_MVEdidUpdCntLoop(void);
INTERFACE void SCU41_MV_EDIDDipWork_Loop(void);
//When suspend, loop runing
INTERFACE void SCU41_MV_SetPowerStateFlag(bool bstate);
INTERFACE bool SCU41_MV_GetPowerStateFlag(void);
INTERFACE void SCU41_MV_SuspendLoopRunning(void);
// the status of led when running system
INTERFACE void SCU41_MV_FuncWork_LoopRunning(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
INTERFACE void SCU41_MV_AllLedOff(void);
INTERFACE void mapp_SCU41MV_SetOutPort5V(bool bEnable);
INTERFACE void SCU41_MV_SetPowerDownFunction(void);
INTERFACE void mapp_SCU41_MVStartupMessage(void);
#if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
INTERFACE void SCU41_MV_SetSwitchPort(u8 u8CurPort);
INTERFACE u8 SCU41_MV_GetSwitchPort(void);
INTERFACE void SCU41_MV_SetSwitchPortOnly(u8 u8CurPort);
INTERFACE void SCU41_MV_SwitchPortFunction(void);
INTERFACE void SCU41_MV_SetInputPortSelect(u8 u8InPort);
INTERFACE bool SCU41_MV_GetSelectPortFlag(void);
INTERFACE void SCU41_MV_SaveInputPortKeyPress(u8 u8InputPort);
INTERFACE u8 SCU41_MV_GetInputPortSelect(void);
INTERFACE void SCU41_MV_SelectPortToWin(u8 u8Win);
INTERFACE u8 SCU41_MV_GetWinPortSelect(void);
INTERFACE void SCU41_MV_SetWinPortSelect(u8 u8WinPort);
INTERFACE u8 mapp_SCU41MVGetTheWinOfCurSrc(u8 u8Src);
INTERFACE void mapp_SCU41MVSetCheckPortFlag(bool bEn);
INTERFACE bool mapp_SCU41MVGetCheckPortFlag(void);
INTERFACE void mapp_SCU41MVSetCheckWinToSrcPort(u8 u8Port);
INTERFACE u8 mapp_SCU41MVGetCheckWinToSrcPort(void);
#if _ENABLE_SWITCH_LED_TIMER
INTERFACE void mapp_SCU41MVSetSwitchLedFlag(bool bEn);
INTERFACE bool mapp_SCU41MVGetSwitchLedFlag(void);
#endif
#endif
INTERFACE void SCU41_MV_SetWinLedMode(u8 u8Mode,u8 u8WinSel);
INTERFACE void SCU41_MV_SetDisplayWin(u8 u8Input,u8 u8Win,bool bUpd);
INTERFACE void SCU41_MV_SetSwapFunction(void);
INTERFACE void mapp_SCU41MVCopyUserModeSrcToNomalMode(u8 *pu8UserSrc);
INTERFACE void SCU41_MV_SetMultiViewMode(u8 u8WinMode);
INTERFACE void SCU41_MV_UpdateWindowNum(u8 u8MvMode);
INTERFACE void SCU41_MV_SetResizeFunction(u8 u8WinMode);
INTERFACE void SCU41_MV_SendDataToFpag(pu8 pu8Data, u16 u16Len);
INTERFACE void SCU41_MV_SendCmd(u8 u8Addr, u8 u8Cmd, u16 u16Len, pu8 pu8Data);
INTERFACE void mapp_SCU41MVResolutionSwitch(u8 u8ResId);
INTERFACE void mapp_SCU41MVChangeTimingData(pstResolutionTable_t pstResTable,pstTimingConfig_t pstOutTiming);
INTERFACE void mapp_SCU41MVSetAudioInput(u8 u8ADSrc);
INTERFACE void mapp_SCU41MVSetAudioMix(bool bEn);
INTERFACE void mapp_SCU41MVSetAudioMute(bool bEn);
INTERFACE void mapp_SCU41MV_AudioOutputMute(bool bEn);
INTERFACE void mapp_SCU41MVAudioSrcSwitchEnable(bool bEn);
INTERFACE bool mapp_SCU41MVGetAudioSrcSwitchStatus(void);
INTERFACE void mapp_SCU41MVAudioSrcSwitchProcess(void);
INTERFACE void mapp_SCU41MVChangeWinSize(u8 u8UserMode,
                                        u8 u8WinPort,
                                        u8 u8Src,
                                        u8 u8StartX,
                                        u8 u8StartY,
                                        u8 u8EndX,
                                        u8 u8EndY);
INTERFACE void mapp_SCU41MVChangeCfgPixelWinSize(u8 u8UserMode,
                                        u8 u8WinPort,
                                        u8 u8Src,
                                        u16 u16StartX,
                                        u16 u16StartY,
                                        u16 u16Width,
                                        u16 u16Height);
INTERFACE void mapp_SCU41MVFpgaWinCmdUpdate(void);
INTERFACE void mapp_SCU41MVSetWindows(u8 u8Addr,pstWindowsConfig_t pstWinConfigVar);
INTERFACE void mapp_SCU41MVWindowConfigure(u8 u8SrcPort,
                                         u8 u8WinNum,
                                         pstWindowType_t pstSrcWin,
                                         pstWindowType_t pstDstWin,
                                         pstWindowType_t pstClipWin,
                                         bool bEnClip,
                                         bool bEnWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinStartX(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinStartY(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinWidth(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinHeight(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinUserModeStartX(u8 u8UserType, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinUserModeStartY(u8 u8UserType, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinUserModeWidth(u8 u8UserType, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetMultiWinUserModeHeight(u8 u8UserType, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetResizeWinStartX(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetResizeWinStartY(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetResizeWinWidth(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetResizeWinHeight(u8 u8Type, BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserConfigWinStartX(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserConfigWinStartY(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserConfigWinWidth(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserConfigWinHeight(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserWinPixelCfgStartX(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserWinPixelCfgStartY(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserWinPixelCfgWidth(BYTE u8DispWin);
INTERFACE u16 mapp_SCU41MVGetUserWinPixelCfgHeight(BYTE u8DispWin);
INTERFACE bool mapp_SCU41MVPixelOutOfRange(u16 u16StartX,u16 u16StartY,u16 u16Width,u16 u16Height);
INTERFACE bool mapp_SCU41MVGetWindowSize(u8 u8Type, BYTE u8DispWin, pstWindowType_t pstDispWin);
INTERFACE bool mapp_SCU41MVGetResize(u8 u8SubType, BYTE u8DispWin, pstWindowType_t pstDispWin);
INTERFACE bool mapp_SCU41MVGetCfgSize( BYTE u8DispWin, pstWindowType_t pstDispWin);
INTERFACE void mapp_SCU41MVGetSourceSize(u8 u8Src, pstWindowType_t pstSrcWin);
INTERFACE void mapp_SCU41MVGetSourceClipSize(bool bClipWin, pstWindowType_t pstClipWin);
INTERFACE void mapp_SCU41MVSrcChangeToSetWinProcess(void);
INTERFACE bool mapp_SCU41MVMultiWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable);
INTERFACE bool mapp_SCU41MVResizeWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable);
INTERFACE bool mapp_SCU41MVConfigModeWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable);
INTERFACE bool mapp_SCU41MVPixelModeWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable);
INTERFACE void mapp_SCU41MVMultiWinUpdate(u8 u8Src,u8 u8WinType,bool bUpd);
INTERFACE void mapp_SetCfgMode(u8 u8CfgMode);
INTERFACE u8 mapp_GetCfgMode(void);
INTERFACE void mapp_SCU41MVCleanWin(u8 u8Win);
INTERFACE bool mapp_SCU41MVIsMvUserMode(u8 u8MvMode);
INTERFACE bool mapp_SCU41MVLimitBaudwidth(u8 u8Src,u8 u8Win,pstWindowType_t pstSrcWin,pstWindowType_t pstDstWin);
INTERFACE void mapp_SCU41MVSetLayerLevel(u8 u8ID,u8 u8Lv1,u8 u8Lv2,u8 u8Lv3,u8 u8Lv4);
INTERFACE void mapp_SCU41MVSetWinCfgUpdateFlag(u32 u32DlyTime, bool bEn);
INTERFACE bool mapp_SCU41MVGetWinCfgUpdateFlag(void);
INTERFACE void mapp_SCU41MVGetWinCfgUpdProcess(void);
#if _ENABLE_AUTO_POWER_FUNCTION
INTERFACE void mapp_SCU41MVAutoPowerProcess(void);
#endif

#undef INTERFACE
#endif
