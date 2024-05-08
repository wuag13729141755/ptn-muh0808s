#if ((IT663XX_VER != IT663XX_VER_1_07)&&(IT663XX_VER != IT663XX_NONE))
#ifndef __ITE663XX_FUNC_API_H_
#define __ITE663XX_FUNC_API_H_

//#include "iTE_Typedef.h"
#include "../USER/video_chip/IT663xx_1.21/IT6634_Customer/iTE_Typedef.h"

#define TIME_SRC_HDCP_TYPE_DET				10//unit is 5ms

typedef enum _tx_out_status_t_
{
	tx_out_enable_stu = 0,
	tx_out_disable_stu,

	tx_out_stu_max,
}tx_out_status_t;

typedef struct _ite663xx_func_api_t_
{
	void (*SysKeyIn)(iTE_u8);
	void (*SysFuncSel)(iTE_u8);
	void (*TxHdcpForceEffect)(void);
	void (*TxOutDisable)(void);
	void (*TxOutEnable)(void);
	void (*TxAdoDisable)(void);
	void (*TxAdoEnable)(void);
	void (*AudioProcess)(void);
	EDID_STAT (*CopyEdidFromTx)(iTE_u8, iTE_u8, EdidInfo *, bool);
	void (*SetRxEdid)(void);

}ite663xx_func_api_t, *pite663xx_func_api_t;

//variable
extern pite663xx_func_api_t pITE663xx_STM32_api;
extern iTE_u8	g_u8RxSel[NUM_IT663XX_USE];
extern iTE_u8	g_u8TxSel[NUM_IT663XX_USE];
extern tx_hdcp_force_t g_tIt663xxTxHdcpType[NUM_IT663XX_USE], g_tPreIt663xxTxHdcpType[NUM_IT663XX_USE];
extern tx_out_status_t it66341_tx_out_status, pre_it66341_tx_out_status;
extern tx_out_status_t it66341_tx_ado_status, pre_it66341_tx_ado_status;
extern iTE_u16	g_u16CurTxFun[NUM_IT663XX_USE];
extern sTxInfo* g_pstCurTxInfo;
extern iTE_u16	g_u16TxFun[NUM_IT663XX_USE][3];
/*extern iTE_u16	g_u16IteFunc;*/
extern iTE_u8 	gCurSysStatus;
#if _PRJ_TYPE_SWITCH_SPLITER == _PRJ_SWITCHER_AND_SPLITER
extern tx_hdcp_force_t prj_suh2x_h2_tx_hdcp_type;
#endif
extern bool g_bInAudChgFlg[NUM_IT663XX_USE];
extern bool g_bIt663xxSetRxEdidFlg[_HDMI_INPUT_PORT_USED_NUMBER];
extern bool g_bIt663xxChangeRxEdidFlag[_HDMI_INPUT_PORT_USED_NUMBER];
#if _ENABLE_EDID_SWITCH_USE_NEW_MODE
extern bool g_bRxEdidChgFlg;
#endif
#if _ENABLE_EDID_SWITCH_UNREBOOT
extern bool g_bUserEdidChange;
#endif
#if _ENABLE_SWITCH_PORT_TO_MUTE_I2S_AUDIO
extern bool g_bSwitchAudChgFlg[NUM_IT663XX_USE];
#endif
#if _ENABLE_FAST_TO_SWITCH_PORT
extern iTE_u1   g_bSwUnplugHpdSta;
#endif
#if _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22
extern iTE_u1   g_bEnableSwHDCP22Flag;
#endif

//function
void IT66341_DefaultFunSelect(iTE_u16 u16FunSel);
void IT663XX_STM32_SysFuncSel(iTE_u8 u8FuncKey);
iTE_u16 IT66341_TxHdcpForceSet(tx_hdcp_force_t forceType);
void IT663XX_TxHdcpForceEffect(void);
void IT663XX_TxOutDisable(void);
void IT663XX_TxOutEnable(void);
void IT663XX_TxAdoDisable(void);
void IT663XX_TxAdoEnable(void);
void SetAudioProDlyTick(iTE_u8 u8DevIdx, iTE_u16 u16Tick);
iTE_u16 PollAudioProDlyTick(iTE_u8 u8DevIdx);
void AudioProDlyTickLoop(void);
void IT663XX_AudioProcess(void);
EDID_STAT IT663XX_CopyEdidFromTx(iTE_u8 u8RxPort, iTE_u8 u8TxPort, EdidInfo *pstEdidInfo, bool bJustRead);
void IT663XX_SetRxEdid(void);

#if (IS_IT663XX==IT66341)
void IT66341_SysKeyIn(iTE_u8 u8Key);
void IT66341_SwitchPort(iTE_u8 u8RxPort);
iTE_u8	IT66341_GetRxSta(iTE_u8 u8RxPort);
iTE_u8 IT66341_GetTxSta(void);
#elif (IS_IT663XX==IT6634)
void IT6634_SysKeyIn(iTE_u8 u8Key);
void IT6634_SwitchPort(iTE_u8 u8TxPort, iTE_u8 u8RxPort);
iTE_u8	IT6634_GetRxSta(iTE_u8 u8RxPort);
iTE_u8	IT6634_GetTxSta(iTE_u8 u8TxPort);
#elif (IS_IT663XX==IT66321)
void IT66321_SysKeyIn(iTE_u8 u8Key);
void IT66321_SwitchPort(iTE_u8 u8RxPort);
iTE_u8	IT66321_GetRxSta(iTE_u8 u8RxPort);
iTE_u8 IT66321_GetTxSta(void);
#endif
#if ENABLE_IT66341_DVI_EDID_TO_CHANGE_HDCP14
void mapp_SwitchDviEdidModeToChangeHdcpVer(u8 u8RxPort);
#endif
iTE_u8 mapp_GetCurrentRxPort(iTE_u8 u8DevID, iTE_u8 u8CurPort);

#if _ENABLE_EDID_SWITCH_USE_NEW_MODE
void mapp_RxPortEdidSwitch(void);
void mapp_EdidChange(void);
void mapp_TxPortReConnectToEdidChange(void);
#endif

#if _ENABLE_EDID_SWITCH_UNREBOOT
void IT6634_SysEdidChange(void);
#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void IT6634_SysEdidChangeBySighalChannel(void);
#endif
#endif
iTE_u8 IT663XX_GetTxTiming(iTE_u8 u8DevID,iTE_pu8 pu8Timing);
iTE_u8 IT663XX_GetRxTiming(iTE_u8 u8RxPort,iTE_pu8 pu8Timing);
#if _ENABLE_INPUT_PORT_HDCP_FOURCE_ON_14_OR_22
void mapp_IT663xxSoftReboot(void);
#endif

#endif

#elif (IT663XX_VER == IT663XX_VER_1_07)
#include "../USER/video_chip/IT663xx_1.07/ITE663XX_func_api.h"
#endif






