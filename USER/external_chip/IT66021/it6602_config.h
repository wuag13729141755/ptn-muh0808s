///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <config.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define _MCU_8051_

#ifndef _MCU_8051_
//#include "platform.h"
#endif

#define _IT66021_	//add by wangjie, 2017.7.28

#ifndef IT66021_DEV_NUM
#define IT66021_DEV_NUM             1
#endif

#ifndef IT66021_PIN_ADDR_LOW0
#define IT66021_PIN_ADDR_LOW0       1
#endif

#ifndef IT66021_PIN_ADDR_LOW1
#define IT66021_PIN_ADDR_LOW1       1
#endif

#ifndef IT66021_PIN_ADDR_LOW2
#define IT66021_PIN_ADDR_LOW2       1
#endif

#ifndef IT66021_PIN_ADDR_LOW3
#define IT66021_PIN_ADDR_LOW3       1
#endif

#ifndef IT66021_PIN_ADDR_LOW4
#define IT66021_PIN_ADDR_LOW4       1
#endif

#ifndef IT66021_PIN_ADDR_LOW5
#define IT66021_PIN_ADDR_LOW5       1
#endif

#ifndef IT66021_PIN_ADDR_LOW6
#define IT66021_PIN_ADDR_LOW6       1
#endif

#ifndef IT66021_PIN_ADDR_LOW7
#define IT66021_PIN_ADDR_LOW7       1
#endif


#ifndef _IT66023_
#ifndef _IT6602_
#ifndef _IT66021_
#define _IT6602_
#endif
#endif
#endif

#ifdef _IT66023_
//#pragma message("defined _IT66023_")
//#define ENABLE_IT66023	// only for IT66023 Usage
#endif


#ifdef _IT6602_
//#pragma message("defined _IT6602_")
#endif

#ifdef _IT66021_
//#pragma message("defined _IT66021_")
#endif

//#define SUPPORT_I2C_SLAVE
#ifdef SUPPORT_I2C_SLAVE
//#pragma message ("SUPPORT_I2C_SLAVE defined")
#endif

#ifndef IT_I2S
#define IT_I2S      0
#endif

#ifndef IT_SPDIF
#define IT_SPDIF    1
#endif

#ifndef IT66021_AUDIO_OUT
#define IT66021_AUDIO_OUT           IT_SPDIF   //I2S
#endif

#define _EN_DUAL_PIXEL_CTRL_

#define _EN_BLOCK_PWRDN_

#define SUPPORT_OUTPUTRGB

#define SUPPORT_INPUTYUV

#endif // _CONFIG_H_
