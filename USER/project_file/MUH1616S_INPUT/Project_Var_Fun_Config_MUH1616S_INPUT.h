#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__

#define SaveSpecificVarInit				MUH1616S_INPUT_SpecificVarInit
#define EepSpecVarInvalid				MUH1616S_INPUT_SpecVarValidCheck
#define PrjStatePowerOnInit				MUH1616S_INPUT_PrjStatePowerOnInit
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage 					mapp_MUH1616S_INPUTStartupMessage
#define SaveSpecificVarDefault          MUH1616S_INPUT_SpecificVarDefault

typedef enum
{
    EDID_MODE_NONE = 0,

    EDID_MODE_INTERNAL1_1024x768_60,
    EDID_MODE_INTERNAL2_1280x720_60,
    EDID_MODE_INTERNAL3_1360x768_60,
    EDID_MODE_INTERNAL4_1920x1200_60,
    EDID_MODE_INTERNAL5_1600x1200_60,
    EDID_MODE_INTERNAL6_1920x1080_60,
    EDID_MODE_INTERNAL7_3840x2160_30,

    EDID_MODE_USER1,
    EDID_MODE_USER2,
    EDID_MODE_USER3,
    EDID_MODE_USER4,

    EDID_MODE_COPYTX1,
    EDID_MODE_COPYTX2,
    EDID_MODE_COPYTX3,
    EDID_MODE_COPYTX4,
    EDID_MODE_COPYTX5,
    EDID_MODE_COPYTX6,
    EDID_MODE_COPYTX7,
    EDID_MODE_COPYTX8,
    EDID_MODE_COPYTX9,
    EDID_MODE_COPYTX10,
    EDID_MODE_COPYTX11,
    EDID_MODE_COPYTX12,
    EDID_MODE_COPYTX13,
    EDID_MODE_COPYTX14,
    EDID_MODE_COPYTX15,
    EDID_MODE_COPYTX16,

    EDID_MODE_MAX,
}emEdidModeType;

typedef enum _led_index_name_t_
{
    led_name_ActiveLed= 0,
    led_name_LedInput1,
    led_name_LedInput2,
    led_name_LedInput3,
    led_name_LedInput4,
    led_name_LedInput5,
    led_name_LedInput6,
    led_name_LedInput7,
    led_name_LedInput8,
    led_name_LedInput9,
    led_name_LedInput10,
    led_name_LedInput10Plus,

    led_name_LedOutput1,
    led_name_LedOutput2,
    led_name_LedOutput3,
    led_name_LedOutput4,
    led_name_LedOutput5,
    led_name_LedOutput6,
    led_name_LedOutput7,
    led_name_LedOutput8,
    led_name_LedOutput9,
    led_name_LedOutput10,
    led_name_LedOutput10Plus,

    led_name_LedAll,
    led_name_LedLock,
    led_name_LedMode,
    led_name_LedEnter,
    led_name_LedScene,
    led_name_LedClear,

    led_name_End,
	led_name_max_number
}led_index_name_t;


typedef enum _emUserTimerType_t_
{
    emUserTimer_LedActive = 0,
    emUserTimer_StartupLinkInfo,
    emUserTimer_LcdBacklightTimeout,
    emUserTimer_LedRefreshTime,
    emUserTimer_LcdRefreshTime,
    emUserTimer_FirmwareUpdate,
    emUserTimer_ScenePolling,

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

typedef enum _emMcuMessageType_t_
{
    eMcuReg_CardType            = 0x01,
    eMcuReg_Version             = 0x02,
    eMcuReg_LinkSta             = 0x03,
    eMcuReg_SetFormat           = 0x04,
    eMcuReg_TxResolution        = 0x05,
    eMcuReg_TxWinConfig         = 0x06,
    eMcuReg_GetTxPortEdid       = 0x07,
    eMcuReg_ResetFactory        = 0x08,
    eMcuReg_SetBritness         = 0x09,
    eMcuReg_SetContrast         = 0x0A,
    eMcuReg_PowerState          = 0x0B,
    eMcuReg_SetHdcp             = 0x0C,
    eMcuReg_SendResID           = 0x0D,
    eMcuReg_SetOutPortFreeze    = 0x0E,
    eMcuReg_SwitchFreeze        = 0x0F,
    eMcuReg_SetOutputPattern    = 0x10,
    eMcuReg_SetDigitalAudioMute = 0x11,
    eMcuReg_MsgFreezeOk         = 0x12,
    eMcuReg_RebootSystem        = 0x13,
    eMcuReg_TxWinClear          = 0x14,

    eMcuReg_SetDebugEn          = 0xF1,
    eMcuReg_UpdateMcu           = 0xF2,
}emMcuMessageType_t;

typedef enum _eCheckCardInfoType_t_
{
    eCheck_GetTxVersion,
    eCheck_GetTxLinkSta,
    eCheck_ReportLinkStaChange,
    eCheck_ReportInputResolutionChange,
    eCheck_End,
}eCheckCardInfoType_t;

typedef enum _eUpdateIndexType_t_
{
    eUpdID_None = 0,
    eUpdID_InputMCU,
    eUpdID_OutputMCU,
    eUpdID_Max,
}eUpdateIndexType_t;

typedef struct _stLinkSta_t_
{
    u8 u8InLinkSta[_HDMI_INPUT_PORT_USED_NUMBER];
    u8 u8OutLinkSta[Def_outport_num];
    u8 u8ResCfgOk[Def_outport_num];
    u8 u8WinCfgOk[Def_outport_num];
    bool bInLinkChangeFlag[_HDMI_INPUT_PORT_USED_NUMBER];
    bool bOutLinkChangeFlag[Def_outport_num];
    stTimingTable_t stInTming[_HDMI_INPUT_PORT_USED_NUMBER];
    bool bInTimingChangeFlag[_HDMI_INPUT_PORT_USED_NUMBER];
}stLinkSta_t;

typedef __packed struct _stSplicerGroupVar_t_
{
    bool            bGroupTxEnable[Def_outport_num];
    uint8_t         u8SplicerWidth;
    uint8_t         u8SplicerHeight;
    uint8_t         u8SplicerPosX[Def_outport_num];
    uint8_t         u8SplicerPosY[Def_outport_num];
    uint8_t         u8BorderSizeWidth;
    uint8_t         u8BorderSizeHeight;
}stSplicerGroupVar_t, *pstSplicerGroupVar_t;

typedef __packed struct _stSceneSaveVar_t_
{
    uint8_t         u8SceneSaveFlag;
    uint8_t         u8VideoCh[Def_outport_num];
    uint16_t        u16SplicerX[Def_outport_num];
    uint16_t        u16SplicerY[Def_outport_num];
    uint16_t        u16SplicerWidth[Def_outport_num];
    uint16_t        u16SplicerHeight[Def_outport_num];
    bool            bSplicerEnable[Def_outport_num];
    uint16_t        u16BorderX[Def_outport_num];
    uint16_t        u16BorderY[Def_outport_num];
    uint16_t        u16BorderWidth[Def_outport_num];
    uint16_t        u16BorderHeight[Def_outport_num];
//    uint16_t        u16BorderSizeW[Def_outport_num];
//    uint16_t        u16BorderSizeH[Def_outport_num];
    stSplicerGroupVar_t stGroupVar[2];
}stSceneSaveVar_t, *pstSceneSaveVar_t;

typedef __packed struct _stIPAddressVar_t_
{
    uint8_t u8IPAddr[4];
    uint8_t u8NetMask[4];
    uint8_t u8NetGate[4];
    uint16_t u16SrcPort;
}stIPAddressVar_t, *pstIPAddressVar_t;

typedef __packed struct _stScenePollingVar_t_
{
    bool            bPollingEnable;
    uint16_t        u16PollingTime;
    bool            bPollingListEnable[32];
}stScenePollingVar_t, *pstScenePollingVar_t;

typedef __packed struct _stAutoTimingVar_t_
{
    bool            bEnable;
    stTimingTable_t stTimingVar;
}stAutoTimingVar_t, *pstAutoTimingVar_t;

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
    bool            bSplicerEnable[Def_outport_num];
    uint16_t        u16BorderX[Def_outport_num];
    uint16_t        u16BorderY[Def_outport_num];
    uint16_t        u16BorderWidth[Def_outport_num];
    uint16_t        u16BorderHeight[Def_outport_num];
//    uint16_t        u16BorderSizeW[Def_outport_num];
//    uint16_t        u16BorderSizeH[Def_outport_num];
    uint8_t         u8RxEdid[_HDMI_INPUT_PORT_USED_NUMBER][256];
    uint8_t         u8RxOsdStr[_HDMI_INPUT_PORT_USED_NUMBER][15];
    uint8_t         u8RxOsdStrLen[_HDMI_INPUT_PORT_USED_NUMBER];
    bool            bRxOsdStrIsAlignRight[_HDMI_INPUT_PORT_USED_NUMBER];
    bool            bRxOsdStrEnable[_HDMI_INPUT_PORT_USED_NUMBER];
    uint16_t        u16RxOsdStrX[_HDMI_INPUT_PORT_USED_NUMBER];
    uint16_t        u16RxOsdStrY[_HDMI_INPUT_PORT_USED_NUMBER];
    uint8_t         u8RxOsdStrColorR[_HDMI_INPUT_PORT_USED_NUMBER];
    uint8_t         u8RxOsdStrColorG[_HDMI_INPUT_PORT_USED_NUMBER];
    uint8_t         u8RxOsdStrColorB[_HDMI_INPUT_PORT_USED_NUMBER];
    bool            bIsLock;
    uint8_t         u8InRs232BaudrateID;
    uint8_t         u8OutRs232BaudrateID;
    uint8_t         u8UartMode;
    stIPAddressVar_t    stIPVar;
    stSceneSaveVar_t    stScene[32];
    stSplicerGroupVar_t stSplicerGroup[2];
    stScenePollingVar_t stScenePollingVar;
    stAutoTimingVar_t   stAutoTimingTable[Def_outport_num];
    uint8_t         u8InputPatternSta[_HDMI_INPUT_PORT_USED_NUMBER];
    uint8_t         u8SplitterMode[4];
    uint8_t         u8SplitterWinSrc[4][4];

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

typedef struct image_header {
	uint32_t    ih_magic;   /* Image Header Magic Number    */
	uint32_t    ih_hcrc;    /* Image Header CRC Checksum    */
	uint32_t    ih_time;    /* Image Creation Timestamp */
	uint32_t    ih_size;    /* Image Data Size      */
	uint32_t    ih_load;    /* Data  Load  Address      */
	uint32_t    ih_ep;      /* Entry Point Address      */
	uint32_t    ih_dcrc;    /* Image Data CRC Checksum  */
	uint32_t	ih_version_software;	/* Software Version  */
	uint32_t	ih_version_hardware;	/* Hardware Version  */
	uint8_t     ih_os;      /* Operating System     */
	uint8_t     ih_arch;    /* CPU architecture     */
	uint8_t     ih_type;    /* Image Type           */
	uint8_t     ih_comp;    /* Compression Type     */
	uint8_t     ih_name[28];  /* Image Name       */
	uint8_t     ih_reserved[8];
} image_header_t;

typedef struct _stLT8644SwitchTable_T_
{
    u8 u8OutCh[Def_outport_num];
    bool bUpdateFlag[Def_outport_num];
    bool bCloseEnableFlag[Def_outport_num];

}stLT8644SwitchTable_T;

extern stLinkSta_t    g_stLinkSta;
extern stLinkSta_t    g_stPreLinkSta;
extern u8 g_u8SceneStartIndex;
extern bool g_bMessageFreezeOkFlag;

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif
INTERFACE void GetResolutionText(char *ps8ResText,u8 u8ResID);
INTERFACE void MUH1616S_INPUT_SpecificVarInit(void);
INTERFACE void MUH1616S_INPUT_SpecBasicVarDefault(void);
INTERFACE void MUH1616S_INPUT_SpecCustomEdidVarDefault(void);
INTERFACE bool MUH1616S_INPUT_SpecVarValidCheck(void);
// the status when power on device
INTERFACE void MUH1616S_INPUT_PrjStatePowerOnInit(void);
INTERFACE void MUH1616S_INPUT_LedWork_Loop(void);
INTERFACE void MUH1616S_INPUT_FuncWork_LoopRunning(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
INTERFACE void mapp_MUH1616S_INPUTStartupMessage(void);
INTERFACE void app_MUH1616S_INPUTSetSplicerPixelDisplay(u8 u8Dev,u8 u8Tx,u16 u16X,u16 u16Y,u16 u16Width,u16 u16Height);
INTERFACE void app_MUH1616S_INPUTSetSplicerDisplay(u8 u8Dev,u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index);
INTERFACE void app_MUH1616S_INPUTPictureAdjust(u8 u8Dev,u8 u8Tx,u8 u8Value,u8 u8Mode);
INTERFACE void app_MUH1616S_INPUTSetSplicerBorder(u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index,u16 u16BorderWidth,u16 u16BorderHeight);
INTERFACE void app_MUH1616S_INPUTSwitchSourceFreeze(u8 u8Dev,u8 u8TxPath,bool bEnFreeze);
INTERFACE void app_MUH1616S_INPUTSourceSwitch(u8 u8Out,u8 u8In);
INTERFACE void app_MUH1616S_INPUTSceneSave(u8 u8SceneID);
INTERFACE void app_MUH1616S_INPUTSceneCall(u8 u8SceneID);
INTERFACE void app_MUH1616S_INPUTScenePollingFunction(void);
INTERFACE void mapp_MUH1616S_InputSendCmdToOutput(u8 u8cmd,u8 *pu8Data, u16 u16Len);
INTERFACE void app_MUH1616S_INPUTFreezeOutPort(void);
INTERFACE void app_MUH1616S_INPUTUnfreezeOutPort(void);

#undef INTERFACE
#endif
