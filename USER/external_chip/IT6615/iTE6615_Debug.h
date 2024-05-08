///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Debug.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/

#include <stdio.h>
#include <stdarg.h>

#ifndef _IT6615_DEBUG_H_
#define _IT6615_DEBUG_H_

#define ENABLE_IT6615_DEBUG   0

#ifndef ENABLE_IT6615_DEBUG
#define ENABLE_IT6615_DEBUG   (1&&_EN_UART_DEBUG_PRINTF)
#endif

#if ENABLE_IT6615_DEBUG
//extern int UART2_printf(const char *fmt, ...);
#define ITE6615_PRINTF dbg_printf
    #define IT6615_MSG(x)               do{ITE6615_PRINTF("[IT6615]");ITE6615_PRINTF x;}while(0)
    #define HDMITX_INT_PRINTF(x)  IT6615_MSG(x)
    #define HDMITX_AUDIO_PRINTF(x)  IT6615_MSG(x)//kuro
    #define HDMITX_DEBUG_PRINTF(x) IT6615_MSG(x)//kuro
    #define HDMITX_SCDC_PRINTF(x) IT6615_MSG(x)//kuro
    #define CEC_DEBUG_PRINTF(x) //IT6615_MSG(x)
    #define HDCP_DEBUG_PRINTF(x) //IT6615_MSG(x)
    #define REG_PRINTF(x) //IT6615_MSG(x)
    #define PRINTF_EDID(x) //IT6615_MSG(x)
    #define PRINTF_BUFFER(x) //IT6615_MSG(x)
    #define HWRD_MSG(x)

#else
    #define IT6615_MSG(x)
    #define HDMITX_INT_PRINTF(x)
    #define HDMITX_AUDIO_PRINTF(x)
    #define HDMITX_DEBUG_PRINTF(x)
    #define HDMITX_SCDC_PRINTF(x)
    #define CEC_DEBUG_PRINTF(x)
    #define HDCP_DEBUG_PRINTF(x)
    #define REG_PRINTF(x)
    #define PRINTF_EDID(x)
    #define PRINTF_BUFFER(x)
    #define HWRD_MSG(x)
#endif




#endif
