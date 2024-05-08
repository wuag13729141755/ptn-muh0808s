///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_HDCP_Repeater_DEFINE.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#ifndef _iTE6615_HDCP_REPEATER_DEFINE_H_
#define _iTE6615_HDCP_REPEATER_DEFINE_H_

#if _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_

typedef void (*_iTE6615_Inform_HPD_Unplug_CB_)(void);
typedef void (*_iTE6615_Inform_HDCP1_KSVListReady_CB_)(unsigned char *BKSV, unsigned char *BStatus, unsigned char *KSV_List)	;
typedef void (*_iTE6615_Inform_HDCP2_KSVListReady_CB_)(unsigned char *RxID, unsigned char *RxInfo,  unsigned char *KSV_List)	;
typedef void (*_iTE6615_Inform_HDCPDone_CB_)(unsigned char *BKSV_RxID);
typedef void (*_iTE6615_Inform_EDIDReadyBlock0_CB_)(unsigned char *EDID);
typedef void (*_iTE6615_Inform_EDIDReadyBlock1_CB_)(unsigned char *EDID);
typedef void (*_iTE6615_Inform_EDIDReadyNoNeedUpdate_CB_)(void);
typedef void (*_iTE6615_Inform_HDCP_EnableRepeaterMode_)(unsigned char Enable);


typedef struct{
		_iTE6615_Inform_HPD_Unplug_CB_				iTE6615_Inform_HPD_Unplug_CB;
		_iTE6615_Inform_HDCP1_KSVListReady_CB_		iTE6615_Inform_HDCP1_KSVListReady_CB;
		_iTE6615_Inform_HDCP2_KSVListReady_CB_		iTE6615_Inform_HDCP2_KSVListReady_CB;
		_iTE6615_Inform_HDCPDone_CB_				iTE6615_Inform_HDCPDone_CB;
		_iTE6615_Inform_EDIDReadyBlock0_CB_			iTE6615_Inform_EDIDReadyBlock0_CB;
		_iTE6615_Inform_EDIDReadyBlock1_CB_			iTE6615_Inform_EDIDReadyBlock1_CB;
		_iTE6615_Inform_EDIDReadyNoNeedUpdate_CB_	iTE6615_Inform_EDIDReadyNoNeedUpdate_CB;
		_iTE6615_Inform_HDCP_EnableRepeaterMode_	iTE6615_Inform_HDCP_EnableRepeaterMode;
}_CB6615_HDCP_;
#endif  // #if _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_

#endif

