///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <sha1.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
///*****************************************
//  Copyright (C)2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <sha1.h>
//   @author Jau-chih.Tseng@ite.com.tw
//   @date   2010/06/04
//   @fileversion: COMMON_FILE_1.01
//******************************************/

#ifndef _IT6602_SHA_1_H_
#define _IT6602_SHA_1_H_

#ifdef _MCU_8051_
    #include "it6602_mcu_conf.h"
#endif

#include <string.h>
#include "it6602_debug.h"
#include "it6602_config.h"
#include "it6602_typedef.h"
#if It6602_Debug_message
    #include <stdio.h>
#endif


#ifndef HDCP_DEBUG_PRINTF
    #define HDCP_DEBUG_PRINTF(x)
#endif //HDCP_DEBUG_PRINTF

#ifndef HDCP_DEBUG_PRINTF1
    #define HDCP_DEBUG_PRINTF1(x)
#endif //HDCP_DEBUG_PRINTF1

#ifndef HDCP_DEBUG_PRINTF2
    #define HDCP_DEBUG_PRINTF2(x)
#endif //HDCP_DEBUG_PRINTF2


#ifndef DISABLE_HDCP
void SHA_Simple(void *p,WORD len,BYTE *output);
void SHATransform(ULONG * h);
#endif

#endif // _SHA_1_H_
