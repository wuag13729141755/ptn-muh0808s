#ifndef __PROJECT_VAR_FUN_CONFIG_H__
#define __PROJECT_VAR_FUN_CONFIG_H__
#include "usart.h"

#define SaveSpecificVarInit				MHD1616SRX_SpecificVarInit
#define EepSpecVarInvalid				MHD1616SRX_SpecVarValidCheck
#define PrjStatePowerOnInit				MHD1616SRX_PrjStatePowerOnInit
#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#define mapp_NewKeyPadProcess           mapp_KeyFunctionProcess
#endif
#define StartupMessage 					mapp_MHD1616SRXStartupMessage
#define SaveSpecificVarDefault          MHD1616SRX_SpecificVarDefault


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
    emUserTimer_ReportTiming,
    emUserTimer_StartupTime,
    emUserTimer_FirmwareUpdate,

    emUserTimer_Max,
}emUserTimerType_t;

typedef enum _emBDMcuCmdType_t_
{
    eMcuReg_CardType            = 0x01,
    eMcuReg_Version             = 0x02,
    eMcuReg_LinkSta             = 0x03,
    eMcuReg_ResetFactory        = 0x08,
    eMcuReg_PowerState          = 0x0B,
    eMcuReg_SetDebugEn          = 0xF1,
    eMcuReg_UpdateMcu           = 0xF2,
    eMcuReg_UpdateFpga          = 0xF3,
    //Rx
    eMcuReg_GetRxResolution     = 0x81,
    eMcuReg_SetRxPortEdid       = 0x82,
}emBDMcuCmdType_t;

typedef __packed struct _stBasicSpecVar_t_
{
	bool			bPowerOnSta;
	bool			bIsLock;
    bool            bIsDebugEn;
    uint8_t         u8InputEdid[_HDMI_INPUT_PORT_USED_NUMBER][256];

	uint8_t			u8CheckSum;
}stBasicSpecVar_t, *pstBasicSpecVar_t;

typedef struct _stProjectSpecVar_t_
{
	stBasicSpecVar_t	stBasSpecVar;
}stProjectSpecVar_t, *pstProjectSpecVar_t;

typedef struct _stInputTimingVar_t_
{
	u16             u16HActive;
	u16             u16VActive;
	u8              u8VFreq;
	u8              u8ScanMode;
}stInputTimingVar_t, *pstInputTimingVar_t;

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

extern stInputTimingVar_t g_stRxResolution[_HDMI_INPUT_PORT_USED_NUMBER];

#ifdef __PROJECT_VAR_FUN_CONFIG_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void MHD1616SRX_SpecificVarInit(void);
INTERFACE void MHD1616SRX_SpecBasicVarDefault(void);
INTERFACE void MHD1616SRX_SpecCustomEdidVarDefault(void);
INTERFACE bool MHD1616SRX_SpecVarValidCheck(void);
// the status when power on device
INTERFACE void MHD1616SRX_PrjStatePowerOnInit(void);
INTERFACE void mapp_MHD1616SRXMainFunction(void);
INTERFACE uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len);
INTERFACE u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar);
INTERFACE void mapp_FirmwareUpdateTimeOutFunction(void);

#undef INTERFACE
#endif
