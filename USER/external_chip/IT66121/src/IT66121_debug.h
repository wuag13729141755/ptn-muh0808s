///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <debug.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/
#ifndef _IT66121_DEBUG_H_
#define _IT66121_DEBUG_H_

//#include "IT66121_typedef.h"


//#define Debug_message 1

extern  int dbg_printf(const char *fmt, ...);


#ifndef Debug_message
#define Debug_message 0
#endif

#if Debug_message

    #define IT66121_HDMITX_DEBUG_PRINTF(x)   dbg_printf x
    #define IT66121_HDCP_DEBUG_PRINTF(x)     dbg_printf x
    #define IT66121_EDID_DEBUG_PRINTF(x)     dbg_printf x
    #define HDMITX_DEBUG_INFO(x)     dbg_printf x
#else
    #define IT66121_HDMITX_DEBUG_PRINTF(x)
    #define IT66121_HDCP_DEBUG_PRINTF(x)
    #define IT66121_EDID_DEBUG_PRINTF(x)
    #define HDMITX_DEBUG_INFO(x)
#endif


#if( Debug_message & (1<<1))
    #define HDMITX_DEBUG_PRINTF1(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF1(x)     dbg_printf x
    #define EDID_DEBUG_PRINTF1(x)     dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF1(x)
    #define HDCP_DEBUG_PRINTF1(x)
    #define EDID_DEBUG_PRINTF1(x)
#endif

#if( Debug_message & (1<<2))
    #define HDMITX_DEBUG_PRINTF2(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF2(x) dbg_printf x
    #define EDID_DEBUG_PRINTF2(x) dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF2(x)
    #define HDCP_DEBUG_PRINTF2(x)
    #define EDID_DEBUG_PRINTF2(x)
#endif

#if( Debug_message & (1<<3))
    #define HDMITX_DEBUG_PRINTF3(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF3(x) dbg_printf x
    #define EDID_DEBUG_PRINTF3(x) dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF3(x)
    #define HDCP_DEBUG_PRINTF3(x)
    #define EDID_DEBUG_PRINTF3(x)
#endif

#if( Debug_message & (1<<4))
    #define HDMITX_DEBUG_PRINTF4(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF4(x) dbg_printf x
    #define EDID_DEBUG_PRINTF4(x) dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF4(x)
    #define HDCP_DEBUG_PRINTF4(x)
    #define EDID_DEBUG_PRINTF4(x)
#endif

#if( Debug_message & (1<<5))
    #define HDMITX_DEBUG_PRINTF5(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF5(x) dbg_printf x
    #define EDID_DEBUG_PRINTF5(x) dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF5(x)
    #define HDCP_DEBUG_PRINTF5(x)
    #define EDID_DEBUG_PRINTF5(x)
#endif

#if( Debug_message & (1<<6))
    #define HDMITX_DEBUG_PRINTF6(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF6(x) dbg_printf x
    #define EDID_DEBUG_PRINTF6(x) dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF6(x)
    #define HDCP_DEBUG_PRINTF6(x)
    #define EDID_DEBUG_PRINTF6(x)
#endif

#if( Debug_message & (1<<7))
    #define HDMITX_DEBUG_PRINTF7(x) dbg_printf x
    #define HDCP_DEBUG_PRINTF7(x)     dbg_printf x
    #define EDID_DEBUG_PRINTF7(x)     dbg_printf x
#else
    #define HDMITX_DEBUG_PRINTF7(x)
    #define HDCP_DEBUG_PRINTF7(x)
    #define EDID_DEBUG_PRINTF7(x)
#endif


#endif//  _DEBUG_H_
