#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__
#include "av_main.h"
#include "usart.h"

#define SaveSpecificVarInit             MHD1616STX_SpecificVarInit
#define EepSpecVarInvalid               MHD1616STX_SpecVarValidCheck
#define PrjStatePowerOnInit             MHD1616STX_PrjStatePowerOnInit
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage                  mapp_MHD1616STXStartupMessage
#define SaveSpecificVarDefault          MHD1616STX_SpecificVarDefault


typedef enum _led_index_name_t_
{
    led_name_ActiveState = 0,

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
    emUserTimer_FirmwareUpdate,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _emFpgaCmdType_t_
{
    eFpgaRegOutTiming           = 0x20,
    eFpgaRegOutWinCfg           = 0x12,
    eFpgaRegOutUpdate           = 0x13,
    eFpgaRegAllPortBright       = 0x22,
    eFpgaRegOnePortBright       = 0x48,
    eFpgaRegVolumeAdj           = 0x39,
    eFpgaRegAudioMuteEn         = 0x3A,
    eFpgaRegWinCfgDenoise       = 0x46,
    eFpgaRegWinCfgAudioMuteEn   = 0x51,
    eFpgaRegOutReset            = 0xF9,
    eFpgaRegAudioSwitch         = 0x3B,
    eFpgaRegGetVersion          = 0x03,
}emFpgaCmdType_t;

typedef enum _emBDMcuCmdType_t_
{
    eMcuReg_CardType            = 0x01,
    eMcuReg_Version             = 0x02,
    eMcuReg_LinkSta             = 0x03,
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
    eMcuReg_SetDebugEn          = 0xF1,
    eMcuReg_UpdateMcu           = 0xF2,
    eMcuReg_UpdateFpga          = 0xF3,
}emBDMcuCmdType_t;

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

//    emRes_BestByDisplay,
    emRes_max,
}emResolutionType_T;

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

typedef __packed struct _stBasicSpecVar_t_
{
    bool            bPowerOnSta;
    bool            bIsLock;
    uint8_t         u8ResolutionId[Def_outport_num];
    stTimingTable_t stCurTimingVar[Def_outport_num];
    stWindowsConfig_t stWinCfgVar[Def_outport_num];
    uint8_t         u8TxHdcpMode[Def_outport_num];
    uint8_t         u8TxFormat[Def_outport_num];
    bool            bIsDebugEn;
    uint8_t         u8TxSrc[Def_outport_num];

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
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stWindowType_t_
{
    u16 u16StartX;      //  start x of the window
    u16 u16StartY;      //  start y of the window
    u16 u16Width;       //  width of the window
    u16 u16Height;      //  height of the window

}stWindowType_t,*pstWindowType_t;

typedef struct _stResolution_t_
{
    u16 u16Width;
    u16 u16Height;
    bool bChangeFlag;
}stResolution_t;

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

extern u8 g_u8BdAddr;

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void MHD1616STX_SpecificVarInit(void);
INTERFACE void MHD1616STX_SpecBasicVarDefault(void);
INTERFACE void MHD1616STX_SpecCustomEdidVarDefault(void);
INTERFACE bool MHD1616STX_SpecVarValidCheck(void);
// the status when power on device
INTERFACE void MHD1616STX_PrjStatePowerOnInit(void);
INTERFACE void mapp_MHD1616STXMainFunction(void);
INTERFACE void mapp_MHD1616STXWindowCfg(uint8_t out_ch,uint8_t in_ch);
INTERFACE void mapp_MHD1616STX_TimingConfig(uint8_t ch,uint8_t idx);
INTERFACE void mapp_MHD1616STXFpgaWinCmdUpdate(u32 u32DelayMs);
INTERFACE void mapp_MHD1616STXResolutionConfig(u8 u8Out,u8 u8ResID);
INTERFACE void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID);
INTERFACE void mapp_MHD1616STXStartupToRecoveryState(void);
INTERFACE void mapp_MHD1616STXSendCmdToSWBD(u8 u8cmd,u8 *pu8Data, u16 u16Len);
INTERFACE void mapp_MHD1616STXWindowCfg2(uint8_t out_ch,pstWindowsConfig_t pstWinConfig);
INTERFACE void mapp_MHD1616STX_TimingConfig2(uint8_t ch,pstTimingTable_t pstTiming);
INTERFACE void mapp_MHD1616STXResolutionConfig2(u8 u8Out,pstTimingTable_t pstTiming);
INTERFACE void SwitchResolutionToRefreshWindowns2(u8 u8Out,pstTimingTable_t pstTiming);
INTERFACE uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len);
INTERFACE u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar);
INTERFACE void mapp_FirmwareUpdateTimeOutFunction(void);
INTERFACE void mapp_MHD1616STXWincfgNoiseMute(bool bMute);

#undef INTERFACE
#endif
