///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Global.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#ifdef WIN32
#include <windows.h>
#include "..\\src\\USBI2C.h"
#endif
#include "includes.h"

#include <string.h>    // for memcmp..

#include "iTE6615_Typedef.h"
#include "iTE6615_Config.h"
#include "iTE6615_Debug.h"

#if (_HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_)
#include "iTE6615_HDCP_Repeater_DEFINE.h"
#endif

#include "iTE6615_DEV_DEFINE.h"

#if (_ENABLE_ITE6615_CEC_== ITE_TRUE)
#include "iTE6615_CEC_DEFINE.h"
#endif

#include "iTE6615_DRV_TX.h"

#include "iTE6615_I2C_RDWR.h"

#include "iTE6615_EDID_PARSER.h"


#ifdef _MCU_8051_
// this file don't need add to PC Code
    #if _MCU_8051_EVB_
        #include "iTE6615_EVB_Debug.h"
    #endif
//#include "iTE6615_IO.h"
#include "iTE6615_mcu.h"

#endif

#include "iTE6615_Utility.h"
#include "iTE6615_SYS_FLOW.h"

#if (_ENABLE_ITE6615_CEC_== ITE_TRUE)
#include "iTE6615_CEC_SYS.h"
#endif

#if (_HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_)
	#include "iTE6615_HDCP_Repeater.h"
	#include "iTE6615_HDCP_Repeater_Callback.h"
#endif





