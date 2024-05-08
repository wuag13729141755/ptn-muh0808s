///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <debug.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/

#ifndef _IT6602_DEBUG_H_
#define _IT6602_DEBUG_H_

#define It6602_Debug_message    1

#ifndef It6602_Debug_message
#define It6602_Debug_message   (1&&_EN_UART_DEBUG_PRINTF)
#endif


#if It6602_Debug_message
//extern int UART2_printf(const char *fmt, ...);
    #define IT6602_printf               dbg_printf
    #define IT6602_MSG(x)               do{IT6602_printf("[IT6602][0x%x]",it66021_idx);IT6602_printf x;}while(0)
    #define IT6602_MSGA(x)              do{IT6602_printf x;}while(0)
	#define MHLRX_DEBUG_PRINTF(x)       IT6602_MSG(x)
	#define EQ_DEBUG_PRINTF(x)          IT6602_MSG(x)
	#define EQ_PORT0_PRINTF(x)          IT6602_MSG(x)
	#define EQ_PORT1_PRINTF(x)          IT6602_MSG(x)
	#define VIDEOTIMNG_DEBUG_PRINTF(x)  IT6602_MSG(x)
	#define IT6602_DEBUG_INT_PRINTF(x)  IT6602_MSG(x)
	#define IT6602_MHL_DEBUG_PRINTF(x)  IT6602_MSG(x)
	#define MHL_MSC_DEBUG_PRINTF(x)     IT6602_MSG(x)
	#define HDMIRX_VIDEO_PRINTF(x)      IT6602_MSG(x)
	#define HDMIRX_AUDIO_PRINTF(x)      IT6602_MSG(x)
	#define HDMIRX_DEBUG_PRINT(x)       IT6602_MSG(x)
	#define CEC_DEBUG_PRINTF(x)         //IT6602_MSG(x)
	#define EDID_DEBUG_PRINTF(x)        IT6602_MSG(x)
	#define ITEHDMI_DEBUG_PRINTF(x)     IT6602_MSG(x)
	#define VSDB_DEBUG_PRINTF(x)        //IT6602_MSG(x)
	#define RCP_DEBUG_PRINTF(x)         IT6602_MSG(x)
	#define MHL3D_DEBUG_PRINTF(x)       //IT6602_MSG(x)
	#define CBUS_CAL_PRINTF(x)          IT6602_MSG(x)
	#define MHL_INT_PRINTF(x)           IT6602_MSG(x)
#else
    #define IT6602_printf(x)
    #define IT6602_MSG(x)
    #define IT6602_MSGA(x)
	#define MHLRX_DEBUG_PRINTF(x)
	#define EQ_DEBUG_PRINTF(x)
	#define EQ_PORT0_PRINTF(x)
	#define EQ_PORT1_PRINTF(x)
	#define VIDEOTIMNG_DEBUG_PRINTF(x)
	#define IT6602_DEBUG_INT_PRINTF(x)
	#define IT6602_MHL_DEBUG_PRINTF(x)
	#define MHL_MSC_DEBUG_PRINTF(x)
	#define HDMIRX_VIDEO_PRINTF(x)
	#define HDMIRX_AUDIO_PRINTF(x)
	#define HDMIRX_DEBUG_PRINT(x)
	#define CEC_DEBUG_PRINTF(x)
	#define EDID_DEBUG_PRINTF(x)
	#define ITEHDMI_DEBUG_PRINTF(x)
	#define VSDB_DEBUG_PRINTF(x)
	#define RCP_DEBUG_PRINTF(x)
	#define MHL3D_DEBUG_PRINTF(x)
	#define CBUS_CAL_PRINTF(x)
	#define MHL_INT_PRINTF(x)
#endif




#endif
