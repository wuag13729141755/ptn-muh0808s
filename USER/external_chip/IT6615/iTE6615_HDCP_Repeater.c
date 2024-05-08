///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_HDCP_Repeater.c>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#include "iTE6615_Global.h"

#if _Enable_6805_CB_
#include "..\IT6805\iTE6805_HDCP_Repeater_Callback.h"
#endif

#if _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_

extern iTE6615_GlobalDATA   iTE6615_DATA;

void iTE6615_HDCP_Repeater_INIT(void)
{
	iTE6615_DATA.Flag_NoNeedEDID = 0;
	iTE6615_DATA.data_buffer_is_EDID = 0;
	iTE6615_DATA.Info_HDCPStatus = IT6615_HDCP_INIT;
	iTE6615_DATA.HDCP_Enable_HDCPVersion = 0;
	#if _Enable_6805_CB_
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_HPD_Unplug_CB = iTE6805_Inform_HPD_Unplug_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_HDCP1_KSVListReady_CB = iTE6805_Inform_HDCP1_KSVListReady_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_HDCP2_KSVListReady_CB = iTE6805_Inform_HDCP2_KSVListReady_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_HDCPDone_CB = iTE6805_Inform_HDCPDone_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_EDIDReadyBlock0_CB = iTE6805_Inform_EDIDReadyBlock0_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_EDIDReadyBlock1_CB = iTE6805_Inform_EDIDReadyBlock1_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_EDIDReadyNoNeedUpdate_CB = iTE6805_Inform_EDIDReadyNoNeedUpdate_CB;
	iTE6615_DATA.CB_HDCP.iTE6615_Inform_HDCP_EnableRepeaterMode = iTE6805_Inform_HDCP_EnableRepeaterMode;
	#endif
}

void iTE6615_HDCP_EnableRepeaterMode(u8 Enable)
{
	if(Enable)
	{
		iTE6615_DATA.HDCP_Enable_iTE6615_AS_HDCPRepeater = 1;
	}
	else
	{
		iTE6615_DATA.HDCP_Enable_iTE6615_AS_HDCPRepeater = 0;
	}
}
#endif
