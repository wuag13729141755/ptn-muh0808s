///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <hdmitx_input.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/
#ifndef _IT66121_HDMITX_DEBUG_H_
#define _IT66121_HDMITX_DEBUG_H_

//#include "IT66121_typedef.h"

#ifdef HDMITX_INPUT_INFO
LONG CalcPCLK(BYTE dev);
LONG CalcAudFS(BYTE dev);
LONG CalcRCLK(void);
BYTE hdmitx_getAudioCount(void) ;

USHORT hdmitx_getInputHTotal(void);
USHORT hdmitx_getInputVTotal(void);
BOOL hdmitx_isInputInterlace(void);
#endif

#endif
