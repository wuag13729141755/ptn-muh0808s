///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <debug.h>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2019/06/24
//   @fileversion: IT6635_SAMPLE_1.20
//******************************************/
#ifndef _DEBUG_H_
#define _DEBUG_H_

#define EANBLE_IT6635_DBG
#ifdef EANBLE_IT6635_DBG
#define PR_INFO(x) pr_info x
#define PR_INFO2(x) pr_info2 x
#define PR_ERR(x) pr_err x
#define PR_AEQ(x) {pr_info x ;}

#define DBG_TXOE_1(x) {  }
#define DBG_TXOE_0(x) {  }

#define DBG_CLKSTABLE_0(x) {  }
#define DBG_CLKSTABLE_1(x) {  }

#define DBG_SYMLOCK_0(x) {  }
#define DBG_SYMLOCK_1(x) {  }
#else
#define PR_INFO(x) //pr_info x
#define PR_INFO2(x) //pr_info2 x
#define PR_ERR(x) //pr_err x
#define PR_AEQ(x) //{pr_info x ;}

#define DBG_TXOE_1(x) {  }
#define DBG_TXOE_0(x) {  }

#define DBG_CLKSTABLE_0(x) {  }
#define DBG_CLKSTABLE_1(x) {  }

#define DBG_SYMLOCK_0(x) {  }
#define DBG_SYMLOCK_1(x) {  }

#endif

enum
{
    RX_SWITCH_PORT,
    RX_HPD_HIGH,
    RX_HPD_LOW,
    CLK_STABLE,
    CLK_UNSTABLE,
    AEQ_TOGGLE_HPD,
    TXOE0,
    TXOE1,
};

#define DBG_TM(n) //{ __debug_set_io(n); }
int set_port( int portnum, int wrmask, int wrdata );
void __debug_set_io(u8 n);
#endif
