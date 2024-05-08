#if ((IT663XX_VER != IT663XX_VER_1_07)&&(IT663XX_VER != IT663XX_NONE))
#ifndef __ITE663XX_COMM_API_H_
#define __ITE663XX_COMM_API_H_

//#include "IT6634_Sys.h"
#include "../USER/video_chip/IT663xx_1.21/IT6634_Customer/IT6634_Sys.h"

#define I2C_SMBusD0						(iTE_u8)_I2C_TABLE_INDEX_IT663XX_0
#define I2C_SMBusD1						(iTE_u8)_I2C_TABLE_INDEX_IT663XX_1
#define I2C_SMBusD2						(iTE_u8)_I2C_TABLE_INDEX_IT663XX_2
#define I2C_SMBusD3						(iTE_u8)_I2C_TABLE_INDEX_IT663XX_3
#define I2C_SMBusD4						(iTE_u8)_I2C_TABLE_INDEX_IT663XX_4


#define IT66341_Initialize				IT6634_Main

#define IT66341_REMOVE_WARNING_EN		_MACRO_ENABLE

#define RX_PORT_BIT_IND_1				(1<<0)
#define RX_PORT_BIT_IND_2				(1<<1)
#define RX_PORT_BIT_IND_3				(1<<2)
#define RX_PORT_BIT_IND_4				(1<<3)
#define RX_PORT_BIT_IND_5				(1<<4)
#define RX_PORT_BIT_IND_6				(1<<5)
#define RX_PORT_BIT_IND_7				(1<<6)
#define RX_PORT_BIT_IND_8				(1<<7)
#define RX_PORT_BIT_IND_9				(1<<8)
#define RX_PORT_BIT_IND_10				(1<<9)
#define RX_PORT_BIT_IND_11				(1<<10)
#define RX_PORT_BIT_IND_12				(1<<11)
#define RX_PORT_BIT_IND_13				(1<<12)
#define RX_PORT_BIT_IND_14				(1<<13)
#define RX_PORT_BIT_IND_15				(1<<14)
#define RX_PORT_BIT_IND_16				(1<<15)


#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
typedef struct _stInputRxIndex_t_
{
    u8 u8DevNum;
    u8 u8PortIndex;
}stInputRxIndex_t;
#endif

typedef void (*FuncAutoSwitchHook)(uint8_t);

//--------------------------------
extern iTE_u8  gIte66341CurEdidMode;
extern iTE_u16 g_u16Ite66341TxHdcpStatus[NUM_IT663XX_USE];
extern const iTE_u8 hdmi_input_logic_index_table[];
extern const iTE_u8 hdmi_input_node_rx_table[];
#if _ENABLE_IT663XX_MULTI_CHIP_IN_USE
extern const stInputRxIndex_t RxPortMap[];
#endif
extern bool g_bAutoSwPowerOnStatus;
#if Project_SCU61E_66341_AUTO_SWITCH
extern volatile iTE_u32 g_vu32AutoDectTimeCnt;
#endif

#if (_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)
extern bool g_bCecOnTVDetectedNewSignal;
#endif
extern bool g_bIt663xxSpdifEnable, g_bIt663xxIISEnable;

extern FuncAutoSwitchHook		FuncAutoSwHook;

extern sTxInfo** g_ppstTxInfoComm;
extern sRxInfo** g_ppstRxInfoComm;
extern iTE_u16*  g_pu16TxPortStaComm;
extern iTE_u16*  g_pu16TxVdoStaComm;
extern iTE_u8*	 g_pu8RxEdidComm;
extern iTE_u16*  g_pu16TxFunComm;
extern iTE_u8*   g_pu8AudCodecComm;
extern iTE_u32*  g_pu32RxAdoDecInfoComm;
extern iTE_u32*  g_pu32TxAdoEncConfComm;
extern iTE_u8*   g_pu8TxAdoEncSettingComm;
extern iTE_u8*   g_pu8RxInfoCAComm;
extern iTE_u8*   g_pu8TxInfoCAComm;
extern iTE_u8*	 g_pu8RxSelComm;
extern iTE_u8*	 g_pu8TxSelComm;

extern iTE_u8*   g_pu8RxHdcp2StaComm;
extern bool*	 g_pbInAudChgFlgComm;
extern iTE_u16*  g_pu16CurTxFunComm;
#if (EN_MHL == iTE_TRUE)
extern sMhlInfo* g_pstMRxComm;
extern iTE_u8*	 g_pu83DInforComm;
#endif
extern iTE_u8*	 g_pu8RxReg44Comm;
extern iTE_u32*  g_pu32RCLKComm;
extern iTE_u8*   g_pu8Hdcp2ChkCntComm;
extern iTE_u8*   g_pu8CurRxAdrComm;
extern iTE_u8*   g_pu8CurActiveRxComm;
extern iTE_u8*   g_pu8CurRxPortComm;
extern iTE_u8*   g_pu8ConvSplComm;
extern iTE_u8*	 g_pu8TxHdcp2StaComm;
extern iTE_u8*	 g_pu8FsChkCntComm;
extern iTE_u8*	 g_pu8SysModeCntComm;
extern iTE_u8*	 g_pu8CurEdidAdrComm;
extern iTE_u8*	 				g_pu8CurTxPortComm;
extern iTE_u8*   				g_pu8RxHdmiModeComm;
extern iTE_u8*   g_pu8CurRxFunCom;
extern iTE_u8*   g_pu8RxFunComm;
extern iTE_u8*   g_pu8CurTxAdrCom;
#if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
extern bool*    g_pbSwitchAudChgFlgComm;
#endif

extern tx_hdcp_force_t*  		g_ptIt663xxTxHdcpTypeComm;
extern tx_hdcp_force_t*  		g_ptPreIt663xxTxHdcpTypeComm;
extern iTE_u16* 		  		g_pu16Ite663xxTxHdcpStatusComm;
extern iTE_u8	 g_it663xx_device;
extern bool		g_bIt663xxAutoSwRunFlag;

#if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
extern iTE_u8 g_u8EdidDviMode[NUM_IT663XX_USE][IN_RXn];
extern iTE_u8*  g_pu8EdidDviModeComm;
#endif
#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
extern bool g_bLoadIteChipRegisterFlag;
#endif
#if ENABLE_IT6634_SWITCH_EDID_LONG_TIME_HPD_LOW_TO_HIGH
extern iTE_u1 g_u1IsEdidSwitchMode[4];
#endif

//---------------------------------
iTE_u1 HRx_Select(iTE_u8 u8RxPort);
void ExtInt_Enable(iTE_u1 bEnable);
#if(SUH4AU_H2_EDID_HANDLE == _MACRO_ENABLE)
void IT663XX_Reset_for_Edid(eIT663xxGetEdidSrc_t eCopySrc);
#else
void IT663XX_Reset_for_Edid(eIT663xxGetEdidSrc_t eCopySrc, uint32_t u32BitIndRx);
#endif
u8 IT663xx_GetInputPortIndex(u8 u8DevId, u8 u8RxPort);
bool isIt663xxDeviceRxTypecPort(iTE_u8 u8Dev, iTE_u8 u8Rx);

#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidSrc_t eCopySrc, u8 u8InputPort);
#endif
void IT6634_Main(void);
void IT663XX_EdidWrite(iTE_u8 u8RxPort, iTE_u8 *u8Edid);
#if (Project_SCU61E_PS176_HDCP_BUG_PRO == _MACRO_ENABLE)
bool PS176PortInputState(iTE_u8 u8ChipInRx);
#endif

#if (Project_663xx_MANY_CHIPS_SIGNAL_REBOOT==_MACRO_ENABLE)
void It663xxRepairSnowInSwitch(iTE_u8 u8TxPort,iTE_u8 u8DevIndex, iTE_u8 u8RxPort);
#endif
void PortEdidMax4K30(iTE_u8 *pBuffer, iTE_u8 u8Dev, iTE_u8 u8RxPort);
iTE_u8 IT663XX_GetPhyAdrByLogic(iTE_u8 logic);
void IT663XX_SetRxSelByLogic(iTE_u8 logicAdr);
iTE_u8 IT663XX_GetDeviceIndexByID(iTE_u8 u8Dev);
void ITE663XX_I2sDisable(void);
void ITE663XX_I2sEnable(void);
void ITE663XX_SpdifDisable(void);
void ITE663XX_SpdifEnable(void);
void ITE663XX_AudioDecEnable(void);
void NumIT663XXSpdifOnOff(bool bEnable);
void NumIT663XXIISOnOff(bool bEnable);
#if ((_EN_CEC_POWER_ON_TV_DETECTED_SIGNAL==_MACRO_ENABLE)&&(_PRJ_TYPE_SWITCH_SPLITER==_PRJ_SWITCHER_ONLY))
void IT663XX_PwrOnTvDtdSngl(void);
#endif
void IT663XX_SwitchChannelByIndex(iTE_u8 u8LogicRxSel);

#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
void mapp_Ite66341RegWriteByte(iTE_u8 u8DevID, iTE_u8 u8address, iTE_u16 u16offset, iTE_u8 u8data);
void mapp_Ite66341RegReadByte(iTE_u8 u8DevID, iTE_u8 u8address, iTE_u16 u16offset, iTE_u8 *pu8data);
#endif  //#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER

/*
iTE_u1 i2c_read_byte( iTE_u8 address, iTE_u8 offset, iTE_u8 byteno, iTE_u8 *p_data, iTE_u8 device);
iTE_u1 i2c_write_byte( iTE_u8 address, iTE_u8 offset, iTE_u8 byteno, iTE_u8 *p_data, iTE_u8 device);
*/
#endif

#elif (IT663XX_VER == IT663XX_VER_1_07)
#include "../USER/video_chip/IT663xx_1.07/ITE663XX_comm_api.h"
#endif







