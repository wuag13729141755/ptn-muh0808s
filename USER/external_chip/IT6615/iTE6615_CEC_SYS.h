///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_CEC_SYS.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#ifndef _iTE6615_CEC_SYS_H_
#define _iTE6615_CEC_SYS_H_
#include "iTE6615_Config.h"

#if (_ENABLE_ITE6615_CEC_== ITE_TRUE)
#include "iTE6615_DEV_DEFINE.h"
void    iTE6615_CEC_irq(void);

void    iTE6615_CEC_fsm(void);
void    iTE6615_CEC_chg(iTE6615_STATEC_Type NewState);

  u8    iTE6615_CEC_CMD_Check(pCEC6615_FRAME CEC6615_FRAME);
void    iTE6615_CEC_CMD_Print(pCEC6615_FRAME CEC6615_FRAME);
void    iTE6615_CEC_CMD_Push_To_Queue_Handler(void);
void    iTE6615_CEC_CMD_Ready_To_Fire(void);

void    iTE6615_CEC_INIT(void);

#endif
#endif

