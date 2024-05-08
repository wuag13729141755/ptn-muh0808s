///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IT6635_cec.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2019/06/24
//   @fileversion: IT6635_SAMPLE_1.20
//******************************************/
#ifndef _IT6634_CEC_H_
#define _IT6634_CEC_H_
//#include "../Customer/iTE_Typedef.h"
#include "IT6635_platform.h"
#include "it6635_drv.h"
#include "it6635.h"
#include "IT6635_config.h"

extern void (*Cec_TxDoneCb)(iTE_u8 u8TxSta1, iTE_u8 u8TxSta2, iTE_u8 u8TxHeader, iTE_u8 u8TxOpCode);
extern void (*Cec_RxDoneCb)(iTE_u8 *u8CecCmd, iTE_u8 u8Size);
void Cec_Init(iTE_u8 u8TimerUnit);
void Cec_Irq(void);
void Cec_CmdFire(iTE_u8 *u8CecCmd, iTE_u8 u8Len);
void Cec_LaSet(iTE_u8 u8La);
void Cec_Fire(void);

#define CEC_RX_SELF_DIS				(1)
#define CEC_RST						(0)
#define CEC_NACK_EN					(0)
#define CEC_CAL_CNT					(1)
#define CEC_RE_FIRE_MAX			(3)

#define Cec_Wb(u8Offset, u8Count, pu8Data)	it6635_i2c_write(CECAddr, u8Offset, u8Count, pu8Data)
#define Cec_Rb(u8Offset, u8Count, pu8Data)	it6635_i2c_read(CECAddr, u8Offset, u8Count, pu8Data)
#define Cec_Set(u8Offset, u8InvMask, u8Data)	cecset(u8Offset, u8InvMask, u8Data)
#define Cec_W(u8Offset, u8Data)					cecwr(u8Offset, u8Data)
#define Cec_R(u8Offset)							cecrd(u8Offset)

#define iTE_Msg(x)	pr_info x
#define iTE_Cec_Msg(x) pr_info("[Cec]") ; pr_info x ; pr_info("\r\n")
#define iTE_MsgE(x) pr_err ("ERROR:") ; pr_err x ; pr_err("\r\n")

#define CEC_PORT (OUT_TX0)

#define STA_CHANGE(a, b, c)		do{(a) = ((a) & (~(b))) | ((c) &(b));} while(0)

#endif
