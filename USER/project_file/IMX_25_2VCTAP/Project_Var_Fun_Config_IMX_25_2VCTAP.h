#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__
#include "usart.h"

#define SaveSpecificVarInit             IMX_25_2VCTAP_SpecificVarInit
#define EepSpecVarInvalid               IMX_25_2VCTAP_SpecVarValidCheck
#define PrjStatePowerOnInit             IMX_25_2VCTAP_PrjStatePowerOnInit
//#define SwitcherSuspend_LoopRunning       IMX_25_2VCTAP_SuspendLoopRunning
//#define SwitcherPrj_LoopRunning           IMX_25_2VCTAP_FuncWork_LoopRunning
//#define mapp_SetPowerDownFunction       IMX_25_2VCTAP_SetPowerDownFunction
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage                  mapp_IMX_25_2VCTAPStartupMessage
#define SaveSpecificVarDefault          IMX_25_2VCTAP_SpecificVarDefault
//#define mapp_SetTypeCResetPinFunction   IMX_25_2VCTAP_SetTypeCResetPinFunction
//#define mapp_EdidInit                   mapp_IMX_25_2VCTAPEdidInit
#define PrjTaskC1LoopFun                __NOP


/////==============================================================================

typedef enum _led_index_name_t_
{
    led_name_ActiveState = 0,
    led_name_Led1D7,
    led_name_Led2D8,
    led_name_Led3D9,
    led_name_Led4D10,
    led_name_LedSwitchD11,
    led_name_LedCallD12,

    led_name_LedInput1R,
    led_name_LedInput1G,
    led_name_LedInput1B,
    led_name_LedInput2R,
    led_name_LedInput2G,
    led_name_LedInput2B,
    led_name_LedInput3R,
    led_name_LedInput3G,
    led_name_LedInput3B,
    led_name_LedInput4R,
    led_name_LedInput4G,
    led_name_LedInput4B,
    led_name_LedInput5R,
    led_name_LedInput5G,
    led_name_LedInput5B,
    led_name_LedInput6R,
    led_name_LedInput6G,
    led_name_LedInput6B,
    led_name_LedInput7R,
    led_name_LedInput7G,
    led_name_LedInput7B,
    led_name_LedInput8R,
    led_name_LedInput8G,
    led_name_LedInput8B,
    led_name_LedInput9R,
    led_name_LedInput9G,
    led_name_LedInput9B,
    led_name_LedInput10R,
    led_name_LedInput10G,
    led_name_LedInput10B,
    led_name_LedInput11R,
    led_name_LedInput11G,
    led_name_LedInput11B,
    led_name_LedInput12R,
    led_name_LedInput12G,
    led_name_LedInput12B,
    led_name_LedInput13R,
    led_name_LedInput13G,
    led_name_LedInput13B,
    led_name_LedInput14R,
    led_name_LedInput14G,
    led_name_LedInput14B,
    led_name_LedInput15R,
    led_name_LedInput15G,
    led_name_LedInput15B,
    led_name_LedInput16R,
    led_name_LedInput16G,
    led_name_LedInput16B,

    led_name_LedOutput1R,
    led_name_LedOutput1G,
    led_name_LedOutput1B,
    led_name_LedOutput2R,
    led_name_LedOutput2G,
    led_name_LedOutput2B,
    led_name_LedOutput3R,
    led_name_LedOutput3G,
    led_name_LedOutput3B,
    led_name_LedOutput4R,
    led_name_LedOutput4G,
    led_name_LedOutput4B,
    led_name_LedOutput5R,
    led_name_LedOutput5G,
    led_name_LedOutput5B,
    led_name_LedOutput6R,
    led_name_LedOutput6G,
    led_name_LedOutput6B,
    led_name_LedOutput7R,
    led_name_LedOutput7G,
    led_name_LedOutput7B,
    led_name_LedOutput8R,
    led_name_LedOutput8G,
    led_name_LedOutput8B,
    led_name_LedOutput9R,
    led_name_LedOutput9G,
    led_name_LedOutput9B,
    led_name_LedOutput10R,
    led_name_LedOutput10G,
    led_name_LedOutput10B,
    led_name_LedOutput11R,
    led_name_LedOutput11G,
    led_name_LedOutput11B,
    led_name_LedOutput12R,
    led_name_LedOutput12G,
    led_name_LedOutput12B,
    led_name_LedOutput13R,
    led_name_LedOutput13G,
    led_name_LedOutput13B,
    led_name_LedOutput14R,
    led_name_LedOutput14G,
    led_name_LedOutput14B,
    led_name_LedOutput15R,
    led_name_LedOutput15G,
    led_name_LedOutput15B,
    led_name_LedOutput16R,
    led_name_LedOutput16G,
    led_name_LedOutput16B,

    led_name_LedCallR,
    led_name_LedCallG,
    led_name_LedCallB,
    led_name_LedSwitchR,
    led_name_LedSwitchG,
    led_name_LedSwitchB,
    led_name_LedSaveR,
    led_name_LedSaveG,
    led_name_LedSaveB,
    led_name_LedModeR,
    led_name_LedModeG,
    led_name_LedModeB,
    led_name_LedPageUpR,
    led_name_LedPageUpG,
    led_name_LedPageUpB,
    led_name_LedPageDownR,
    led_name_LedPageDownG,
    led_name_LedPageDownB,

    led_name_End,
    led_name_max_number
}led_index_name_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_S1 = 0,
    emKeyName_S2,
    emKeyName_S3,
    emKeyName_S4,
    emKeyName_S5,
    emKeyName_S6,
    emKeyName_S7,
    emKeyName_S8,
    emKeyName_S9,
    emKeyName_S10,
    emKeyName_S11,
    emKeyName_S12,
    emKeyName_S13,
    emKeyName_S14,
    emKeyName_S15,
    emKeyName_S16,
    emKeyName_S17,
    emKeyName_S18,
    emKeyName_S19,
    emKeyName_S20,
    emKeyName_S21,
    emKeyName_S22,
    emKeyName_S23,
    emKeyName_S24,
    emKeyName_S25,
    emKeyName_S26,
    emKeyName_S27,
    emKeyName_S28,
    emKeyName_S29,
    emKeyName_S30,
    emKeyName_S31,
    emKeyName_S32,
    emKeyName_S33,
    emKeyName_S34,
    emKeyName_S35,
    emKeyName_S36,
    emKeyName_S37,
    emKeyName_S38,

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
//    stCustomEdid_t      stCustomEdidPre1;
//    stCustomEdid_t      stCustomEdidPre2;
//    stCustomEdid_t      stCustomEdidPre3;
//    stCustomEdid_t      stCustomEdidPre4;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stTxEdidData_t_
{
    uint8_t         u8EdidRecv[256];
    uint8_t         u8RecvOk;
}stTxEdidData_t, *pstTxEdidData_t;

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

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void IMX_25_2VCTAP_SpecificVarInit(void);
INTERFACE void IMX_25_2VCTAP_SpecBasicVarDefault(void);
INTERFACE void IMX_25_2VCTAP_SpecCustomEdidVarDefault(void);
INTERFACE bool IMX_25_2VCTAP_SpecVarValidCheck(void);
INTERFACE void SetUserData_SpecBasicVar(void);
INTERFACE void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex);
// the status when power on device
INTERFACE void IMX_25_2VCTAP_PrjStatePowerOnInit(void);
INTERFACE void IMX_25_2VCTAP_AllLedOff(void);
INTERFACE void mapp_IMX_25_2VCTAPStartupMessage(void);
INTERFACE uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len);
INTERFACE u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar);
INTERFACE void mapp_FirmwareUpdateTimeOutFunction(void);
INTERFACE uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len);

#undef INTERFACE
#endif
