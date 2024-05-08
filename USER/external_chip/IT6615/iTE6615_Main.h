///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Main.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#ifndef __IT6615_MAIN_H__
#define __IT6615_MAIN_H__

#ifdef _MCU_8051_
void InitMessage(void);
void HoldSystem(void);
#else
extern bool g_bEnableHdcpSwitch;
extern u8   g_u8OutputHdcpSta;
extern u8   g_pu8It6615TxEdidBuffer[256];
#if _ENABLE_SKIP_NO_AUDIO_INTERRUPT
extern bool g_bEnableIt6615PowerOffFlag;
#endif

void mapp_Ite6615SetPowerOffState(bool bSta);
bool mapp_Ite6615GetPowerState(void);
void iTE6615_Main( void );
void ITE6615_process(void);
void ITE6615_Init(void);
void ITE6615_SetReadTxEdidFlag(bool bSta);
bool ITE6615_GetReadTxEdidFlag(void);
void mapp_Ite6615SetSignalInputChange(bool bChange);
bool mapp_Ite6615GetSignalInputChange(void);
void mapp_Ite6615SetOutputVic(u16 u16Vic);
u16 mapp_Ite6615GetOutputVic(void);
void mapp_Ite6615SetAvMute(bool bSta);

#endif

#endif

