///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_HDCP_Repeater_Callback.c>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#include "iTE6615_Global.h"

#if _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_

extern iTE6615_GlobalDATA   iTE6615_DATA;

void iTE6615_Get_DownstreamHDCPStatus_CB(unsigned char *valid, unsigned char *HDCPStatus)
{
	HDCP_DEBUG_PRINTF(("in iTE6615_Get_DownstreamHDCPStatus_CB \r\n\r\n"));

	if(iTE6615_DATA.HDCP_Enable_iTE6615_AS_HDCPRepeater == 0){return;}

	if(iTE6615_DATA.Info_HDCPStatus == 0)
	{
		*valid = 0;
		HDCP_DEBUG_PRINTF(("valid = 0 \r\n"));
	}
	else
	{
		HDCP_DEBUG_PRINTF(("valid = 1 \r\n"));
		HDCP_DEBUG_PRINTF(("HDCPStatus = 0x%02X \r\n", iTE6615_DATA.Info_HDCPStatus));
		*valid = 1;

		*HDCPStatus = iTE6615_DATA.Info_HDCPStatus;
	}
}

void iTE6615_Set_DownstreamHDCPVersion_CB(unsigned char HDCP_Version)
{
	// 1 = HDCP 1.X
	// 2 = HDCP 2.X
	iTE6615_DATA.HDCP_Enable_HDCPVersion = HDCP_Version;

	if(iTE6615_DATA.HDCP_Enable_HDCPVersion > 0)	// HDCP Enable and video outputed but HDCP delay
	{
		if(iTE6615_DATA.STATES == iTE6615_STATES_RxSenON && iTE6615_DATA.STATEV == iTE6615_STATEV_VidOutEnable && iTE6615_DATA.STATEH == STATEH_Reset)
		{
			iTE6615_hdcp_chg( STATEH_CPGoing );
		}
	}
}
#endif
