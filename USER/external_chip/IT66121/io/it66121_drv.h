///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <main.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/

#ifndef _IT66121_DRV_H_
#define _IT66121_DRV_H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Begin of main.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IT66121/src/IT66121_typedef.h"
#include "IT66121/src/IT66121_hdmitx.h"
#include "IT66121/src/IT66121_debug.h"
#include "IT66121/src/IT66121_hdmitx.h"
#include "IT66121/io/IT66121_io.h"

#include "IT66121/src/IT66121_config.h"
#include "IT66121/src/IT66121_hdmitx_drv.h"
#include "IT66121/src/IT66121_hdmitx_sys.h"
#include "IT66121/src/IT66121_hdmitx_cec.h"

#include "delay.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Data Type Definition
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
	unsigned char u8DevAddr;
    unsigned char u8BusId;
} IT66121_I2C_CONFIG;

extern  BYTE        ucCurrentHDMIPort;
extern  BYTE        RemoteSetHDMIPort;

extern IT66121_I2C_CONFIG const cg_stIT66121_I2CConfig[IT66121_DEV_NUM];
extern u8 g_u8It66121DevAddr;
extern u8 g_u8It66121BusID;

void IT66121_Process(void);
void IT66121_init(void);



extern  void it66121_delay_ms(unsigned int ms);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End of main.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif // _MAIN_H_
