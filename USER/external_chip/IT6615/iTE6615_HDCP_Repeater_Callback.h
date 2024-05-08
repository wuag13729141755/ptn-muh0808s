///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_HDCP_Repeater_Callback.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#ifndef _iTE6615_HDCP_REPEATER_CALLBACK_H_
#define _iTE6615_HDCP_REPEATER_CALLBACK_H_


#if _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_
void iTE6615_Get_DownstreamHDCPStatus_CB(unsigned char *valid, unsigned char *HDCPStatus);
void iTE6615_Set_DownstreamHDCPVersion_CB(unsigned char HDCP_Version);
#endif

#endif
