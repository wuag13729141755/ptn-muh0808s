#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__
#include "usart.h"

#define SaveSpecificVarInit             MHD1616SSWBD_SpecificVarInit
#define EepSpecVarInvalid               MHD1616SSWBD_SpecVarValidCheck
#define PrjStatePowerOnInit             MHD1616SSWBD_PrjStatePowerOnInit
//#define SwitcherSuspend_LoopRunning       MHD1616SSWBD_SuspendLoopRunning
//#define SwitcherPrj_LoopRunning           MHD1616SSWBD_FuncWork_LoopRunning
//#define mapp_SetPowerDownFunction       MHD1616SSWBD_SetPowerDownFunction
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage                  mapp_MHD1616SSWBDStartupMessage
#define SaveSpecificVarDefault          MHD1616SSWBD_SpecificVarDefault
//#define mapp_SetTypeCResetPinFunction   MHD1616SSWBD_SetTypeCResetPinFunction
//#define mapp_EdidInit                   mapp_MHD1616SSWBDEdidInit
#define PrjTaskC1LoopFun                __NOP


/////==============================================================================
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
    led_name_ActiveState = 0,

    led_name_End,
    led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_DIPKey1= 0,
    emKeyName_max,
}key_counter_index_name_t;
#endif

typedef enum _emHdmiTxFormat_t_
{
    emTxFormat_Hdmi = 0,
    emTxFormat_DVI,

    emTxFormat_max,
}emHdmiTxFormat_t;

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

    emRes_max,
}emResolutionType_T;

typedef enum _emUserTimerType_t_
{
    emUserTimer_LedActive = 0,
    emUserTimer_StartupTime,
    emUserTimer_CardInfo,
    emUserTimer_TimingChange,
    emUserTimer_FirmwareUpdate,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _emBDMcuCmdType_t_
{
    eMcuReg_CardType            = 0x01,
    eMcuReg_Version             = 0x02,
    eMcuReg_LinkSta             = 0x03,
    eMcuReg_UpdateMcu           = 0xF2,
    eMcuReg_UpdateFpga          = 0xF3,

    //Tx
    eMcuReg_HdcpAndFormat       = 0x04,
    eMcuReg_TxResolution        = 0x05,
    eMcuReg_TxWinConfig         = 0x06,
    eMcuReg_GetTxPortEdid       = 0x07,
    eMcuReg_ResetFactory        = 0x08,
    eMcuReg_SetBritness         = 0x09,
    eMcuReg_UpdateWincfg        = 0x0A,
    eMcuReg_PowerState          = 0x0B,
    eMcuReg_SendRxTiming        = 0x0C,
    eMcuReg_SendResID           = 0x0D,
    eMcuReg_SendInputChannel    = 0x0E,

    //Rx
    eMcuReg_GetRxResolution     = 0x81,
    eMcuReg_SetRxPortEdid       = 0x82,
}emBDMcuCmdType_t;

typedef enum _emCardType_t_
{
    emCardType_Tx = 0,
    emCardType_Rx1,
    emCardType_Rx2,

    emCardType_Max,
}emCardType_t;

typedef enum _eStartupLoopType_t_
{
    eSt_SInit,
    eSt_GetTxCardType,
    eSt_GetTxVersion,
    eSt_SetTxResolution,
    eSt_SetTxConfigEnd,
    eSt_GetRx1CardType,
    eSt_GetRx1Version,
    eSt_GetRx2CardType,
    eSt_GetRx2Version,
    eSt_None,
}eStartupLoopType_t;

typedef enum _eCheckCardInfoType_t_
{
    eCheck_TxLinkSta,
    eCheck_Rx1LinkSta,
    eCheck_Rx1InputTinming,
    eCheck_Rx2LinkSta,
    eCheck_Rx2InputTinming,
    eCheck_GetTxEdid,
    eCheck_End,
}eCheckCardInfoType_t;

typedef enum _eUpdateIndexType_t_
{
    eUpdID_None = 0,
    eUpdID_SwitchBDMCU,
    eUpdID_RX1BDMCU,
    eUpdID_RX2BDMCU,
    eUpdID_TXBDMCU,
    eUpdID_RX1BDFPGA1,
    eUpdID_RX1BDFPGA2,
    eUpdID_RX2BDFPGA1,
    eUpdID_RX2BDFPGA2,
    eUpdID_TXBDFPGA1,
    eUpdID_TXBDFPGA2,
    eUpdID_TXBDFPGA3,
    eUpdID_TXBDFPGA4,
    eUpdID_Max,
}eUpdateIndexType_t;

typedef __packed struct _stSceneSaveVar_t_
{
    uint8_t u8SceneSaveFlag;
    uint8_t u8VideoCh[Def_outport_num];
}stSceneSaveVar_t, *pstSceneSaveVar_t;

typedef __packed struct _stIPAddressVar_t_
{
    uint8_t u8IPAddr[4];
    uint8_t u8NetMask[4];
    uint8_t u8NetGate[4];
    uint16_t u16SrcPort;
}stIPAddressVar_t, *pstIPAddressVar_t;

typedef __packed struct _stBasicSpecVar_t_
{
    bool            bPowerOnSta;
    bool            bIsLock;
    uint8_t         u8InRs232BaudrateID;
    uint8_t         u8OutRs232BaudrateID;
    stSceneSaveVar_t    stScene[32];
    uint8_t         u8TxFormatSta[Def_outport_num];
    uint8_t         u8TxResolutionID[Def_outport_num];
    uint8_t         u8UartMode;
    uint8_t         u8ExternalAudioMute[2];
    uint8_t         u8AudioSource[Def_outport_num+2];
    uint8_t         u8TxHdcpMode[Def_outport_num];
    stIPAddressVar_t    stIPVar;
    bool            bIsDebugEn;
    uint8_t         u8CheckSum;
}stBasicSpecVar_t, *pstBasicSpecVar_t;

typedef __packed struct _stCustomEdid_t_
{
    uint8_t         pu8EdidContext[256];

    uint8_t         u8CheckSum;
}stCustomEdid_t, *pstCustomEdid_t;

typedef struct _stProjectSpecVar_t_
{
    stBasicSpecVar_t    stBasSpecVar;
    stCustomEdid_t      stCustomEdidPre1;
    stCustomEdid_t      stCustomEdidPre2;
    stCustomEdid_t      stCustomEdidPre3;
    stCustomEdid_t      stCustomEdidPre4;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stTxEdidData_t_
{
    uint8_t         u8EdidRecv[256];
    uint8_t         u8RecvOk;
}stTxEdidData_t, *pstTxEdidData_t;

typedef struct _stADN4604SwitchTable_T_
{
    u8 u8OutCh[Def_outport_num];
    bool bUpdateFlag[Def_outport_num];
    bool bCloseEnableFlag[Def_outport_num];

}stADN4604SwitchTable_T;

typedef struct _stResolution_t_
{
    u16 u16Width;
    u16 u16Height;
    u16 u16VFreq;
    u8  u8ScanMode;
    bool bChangeFlag;
}stResolution_t;

typedef struct _stLinkSta_t_
{
    u8 u8InLinkSta[_HDMI_INPUT_PORT_USED_NUMBER];
    u8 u8OutLinkSta[Def_outport_num];
    u8 u8ResCfgOk[Def_outport_num];
    u8 u8WinCfgOk[Def_outport_num];
}stLinkSta_t;

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

/////////////////////////////////////////////////////////////////////////////
extern stResolution_t g_stRxResTable[_HDMI_INPUT_PORT_USED_NUMBER];
extern u8 g_u8CardType[3];
extern stTxEdidData_t g_stTxEdidVar[Def_outport_num];
extern unsigned char const  _1_1024x768p60[256];
extern unsigned char const  _2_1280x720p60[256];
extern unsigned char const  _3_1360x768p60[256];
extern unsigned char const  _4_1920x1200p60[256];
extern unsigned char const  _5_1600x1200p60[256];
extern unsigned char const  _6_1920x1080p60[256];

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void MHD1616SSWBD_SpecificVarInit(void);
INTERFACE void MHD1616SSWBD_SpecBasicVarDefault(void);
INTERFACE void MHD1616SSWBD_SpecCustomEdidVarDefault(void);
INTERFACE bool MHD1616SSWBD_SpecVarValidCheck(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
// the status when power on device
INTERFACE void MHD1616SSWBD_PrjStatePowerOnInit(void);
INTERFACE void MHD1616SSWBD_AllLedOff(void);
INTERFACE void mapp_MHD1616SSWBDStartupMessage(void);
INTERFACE void mapp_MHD1616SSWBD_SetAudioSource(u8 u8Out, u8 u8AudioSrc);
INTERFACE void mapp_SendCmdToInOutCard(u8 u8DevAddr,u8 u8Cmd,pu8 pu8Data, u16 u16Len);
INTERFACE void mapp_SendDataToInOutCard(pu8 pu8Data, u16 u16Len);
INTERFACE void mapp_MHD1616SSWBDSendCmdToTXBD(u8 u8TxID,u8 u8cmd,u8 *pu8Data, u16 u16Len);
INTERFACE void mapp_MHD1616SSWBDSendCmdToRXBD(u8 u8TxID,u8 u8cmd,u8 *pu8Data, u16 u16Len);
INTERFACE void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID);
INTERFACE void mapp_MHD1616SSWBDStartupToRecoveryState(void);
INTERFACE void mapp_MHD1616SSWBDMainFunction(void);
INTERFACE void mapp_MHD1616SSWBDWindowCfg(uint8_t out_ch,uint8_t in_ch);
INTERFACE void mapp_MHD1616SSWBDFpgaWinCmdUpdate(u32 u32DelayMs);
INTERFACE void mapp_MHD1616SSWBD_TimingConfig(uint8_t ch,uint8_t idx);
INTERFACE uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len);
INTERFACE u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar);
INTERFACE void mapp_FirmwareUpdateTimeOutFunction(void);
INTERFACE uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len);

#undef INTERFACE
#endif
