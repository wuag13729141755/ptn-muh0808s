///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IT6635_CecSys_TV.c>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2019/06/24
//   @fileversion: IT6635_SAMPLE_1.20
//******************************************/
#include "IT6635/chip/IT6635_Cec.h"
#if ( (IT6635_EN_CEC == iTE_TRUE))

#define _CEC_LOOKUP_TAB_

#if EN_IT6601_ARC
//#include "../chip/it6601.h"
#endif

#include "IT6635/Customer/IT6635_CecSys.h"

CecSys stCecSta;
CecSys *pstCecSta = &stCecSta;
iTE_u8	u8CecSysCmd[20];
iTE_u8	g_u8TxCurHeader, g_u8TxCurOpCode;
iTE_u8	u8CecList[16][3] = {	// PaAB, PaCD, CecType
							{0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF},
							{0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF}
						};

#define CEC_TX_QUEUE_SIZE	(0x20)
#define CEC_RX_QUEUE_SIZE	(0x40)

iTE_u8	u8TxQueue[CEC_TX_QUEUE_SIZE];
iTE_u8	u8Rxqueue[CEC_RX_QUEUE_SIZE];

iTE_u8	g_u8Volume 	;
BOOL	g_bMute		;
BOOL	g_bAudioMode ;

stCecQ	stTxQ, stRxQ;

//****************************************************************************
iTE_ps8 /*_CODE_3K*/const pu8RcpString[0x80]={
	"Select/OK",			"Up",	 					"Down", 						"Left",		 			"Right", 				"Right-Up", 			"Right-Down",				"Left-Up",
	"Left-Down", 			"Root Menu",					"Setup Menu", 				"Contents Menu", 			"Favorite Menu",	 	"Back",				NULL,					NULL,
	"Media Top Menu",		"Media Contextsensitive Menu",	NULL,						NULL,					NULL,				NULL,				NULL,					NULL,
	NULL,				NULL,						NULL,						NULL,					NULL,				"Number Entry Mode",	"Number 11",				"Number 12",
	"Numeric 0/10",		"Numeric 1",					"Numeric 2",					"Numeric 3",				"Numeric 4",			"Numeric 5",			"Numeric 6",				"Numeric 7",
	"Numeric 8",			"Numeric 9",					"Dot",						"Enter",					"Clear",				NULL,				NULL,					"Next Favorite",
	"Channel Up",			"Channel Down",				"Previous Channel",			"Sound Select",			"Input Select", 		"Display Information",	"Help",					"Page Up",
	"Page Down",			NULL,						NULL,						NULL,					NULL,				NULL,				NULL,					NULL,
	"Power",				"Volume Up",					"Volume Down",				"Mute",					"Play",				"Stop",				"Pause",					"Record",
	"Rewind",				"Fast Forward",				"Eject",						"Skip Forward",			"Skip Backward",		"Stop-Record",		"Pause-Record",			NULL,
	"Angle",				"Subpicture",					"Video on Demand",			"Electronic Program Guide",	"Timer Programming",	"Initial Configuration",	"Select Broadcast Type",	"Select Sound Presentation",
	"Audio Descritption",	"Internet",					"3D mode",					NULL,					NULL,				NULL,				NULL,					NULL,
	"Play Function",		"Pause Play Function",			"Record Function",				"Pause Record Function",	"Stop Function",		"Mute Function",		"Restore Volume Funation",	"Tune Function",
	"Select Media Function", "Select A/V Input Function",		"Select Audio Input Function",	"Power Toggle Function",	"Power Off Function",	"Power On Function",	NULL,					NULL,
	NULL,				"F1 (Blue)",					"F2 (Red)",					"F3 (Green)",				"F4 (Yellow)",			"F5",					"Data",					NULL,
	NULL,				NULL,						NULL,						NULL,					NULL,				NULL,				NULL,					NULL
};

iTE_u1 CecSys_PushToQueue(iTE_u8 *pu8Cmd, iTE_u8 u8Len, stCecQ *pstQ)
{
	iTE_u8	u8RPtr = pstQ->u8RPtr;
	iTE_u8	u8WPtr = pstQ->u8WPtr;
	iTE_pu8	pu8Queue = pstQ->pu8Buf;
	iTE_u8 	u8QMaxSize = pstQ->u8MaxSize;
	iTE_u8	u8FreeSize;

	if(u8WPtr < u8RPtr){
		u8FreeSize = u8RPtr - u8WPtr;
	}else{
		u8FreeSize = u8QMaxSize - u8WPtr + u8RPtr;
	}

	if(u8Len){
		if(u8FreeSize < (u8Len + 1)){
			iTE_MsgE(("[Cec] TX Queue Full, %X, %X, %X\n", u8WPtr, u8RPtr, u8Len));
			pstQ->u8RPtr = 0;
			pstQ->u8WPtr = 0;
			return iTE_FALSE;
		}

		pu8Queue[u8WPtr++ & (u8QMaxSize - 1)] = u8Len;
		while(u8Len--){
			pu8Queue[u8WPtr++ & (u8QMaxSize - 1)] = *pu8Cmd++;
		}
		pstQ->u8WPtr = u8WPtr & (u8QMaxSize - 1);
		iTE_Cec_Msg(("-CecSys_PushToQueue wPtr=%02X, rPtr=%02X, Len=%X\n", u8WPtr, u8RPtr, u8Len));
		return iTE_TRUE;
	}
	return iTE_FALSE;
}
//****************************************************************************
void Cec_SwitchPort(iTE_u8 *u8CecSysCmd)
{
		switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_OTPFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
		case	eActiveSource:
			Cec_SwitchPort(u8CecSysCmd);
			break;
	}
}
//****************************************************************************
void Cec_RoutingCtlFeature(iTE_u8 *u8CecSysCmd)
{
	//iTE_u16	u16Temp, u16CecPa;
	switch(u8CecSysCmd[1]){
		case	eRoutingChange:

			break;

	}
}
//****************************************************************************
void Cec_StandByFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
		case	eStandBy:
		//		IT6634_SwitchPort(CEC_PORT, IN_RXn);
			break;
	}
}
//****************************************************************************
void Cec_OTRFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;

	switch(u8CecSysCmd[1]){
		case	eRecordOff:
				// ToDo
			break;
		case	eRecordOn:
				// ToDo
			break;
		case	eRecordStatus:
				// ToDo
			break;
		case	eRecordTVScreen:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eFeatureAbort;
				u8CecSysCmd[2] = eRecordTVScreen;
				u8CecSysCmd[3] = CEC_ABORT_REASON_CAN_NOT_PROVIDE_SOURCE;
				CecSys_PushToQueue(u8CecSysCmd, 4, &stTxQ);
			break;
	}
}
//****************************************************************************
void Cec_TimerProgFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_SysInfoFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;
	switch(u8CecSysCmd[1]){
		case	eCECVersioin:
			// ToDo
			break;
		case	eGetCECVersion:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eCECVersioin;
				u8CecSysCmd[2] = CEC_VERISION_2_0;
				CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
			break;
		case	eGivePhysicalAddress:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
				u8CecSysCmd[1] = eReportPhysicalAddress;
				u8CecSysCmd[2] = pstCecSta->u8PaAB;
				u8CecSysCmd[3] = pstCecSta->u8PaCD;
				u8CecSysCmd[4] = pstCecSta->u8Type;
				CecSys_PushToQueue(u8CecSysCmd, 5, &stTxQ);
			break;
		case	eGetMenuLanguage:
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
				u8CecSysCmd[1] = eSetMenuLanguage;
				u8CecSysCmd[2] = 'c';
				u8CecSysCmd[3] = 'h';
				u8CecSysCmd[4] = 'i';
				CecSys_PushToQueue(u8CecSysCmd, 5, &stTxQ);
			break;
		case	eReportPhysicalAddress:
				u8CecList[u8CecSysCmd[0] >> 4][0] = u8CecSysCmd[2];
				u8CecList[u8CecSysCmd[0] >> 4][1] = u8CecSysCmd[3];
				u8CecList[u8CecSysCmd[0] >> 4][2] = u8CecSysCmd[4];
			break;
		case	eSetMenuLanguage:
			// ToDo
			break;
		case	eReportFeatures:
			// ToDo
			break;
		case	eGiveFeatures:
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
				u8CecSysCmd[1] = eReportFeatures;
				u8CecSysCmd[2] = CEC_VERISION_2_0;
			break;
	}
}
//****************************************************************************
void Cec_DeckCtrlFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_TunerCtrlFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_VenSpecCmdFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
		iTE_u8	u8Temp;
		case eGiveDeviceVendorID:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eDeviceVendorID;
				//type vendor ID here
				u8CecSysCmd[2] = 0x00;
				u8CecSysCmd[3] = 0x00;
				u8CecSysCmd[4] = 0x00;
				CecSys_PushToQueue(u8CecSysCmd, 5, &stTxQ);
			break ;
		case eDeviceVendorID:
			//record device vendor ID here
			break ;
	}
}
//****************************************************************************
void Cec_OSDDisplayFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){

	}
}
//****************************************************************************
void Cec_DevOSDTransferFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;

	switch(u8CecSysCmd[1]){
		case	eGiveOSDName:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eSetOSDName;
				u8CecSysCmd[2] = 'I';
				u8CecSysCmd[3] = 'T';
				u8CecSysCmd[4] = '6';
				u8CecSysCmd[5] = '6';
				u8CecSysCmd[6] = '3';
				u8CecSysCmd[7] = '4';
				CecSys_PushToQueue(u8CecSysCmd, 8, &stTxQ);
			break;
		case	eSetOSDName:
			break;
	}
}
//****************************************************************************
void Cec_DevMenuCtrlFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_RemoteCtlPassthroughFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;
	switch(u8CecSysCmd[1]){
		case eUserControlPressed:
			switch(u8CecSysCmd[2]){
				case eVolumeUp:
				// Volume Up Here

				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eReportAudioStatus;
				u8CecSysCmd[2] = CEC_AUDIO_MUTE_OFF << 7; //Mute Status + Volume Status
				CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
				break ;
				case eVolumeDown:
				// volume down Here

				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eReportAudioStatus;
				u8CecSysCmd[2] = CEC_AUDIO_MUTE_OFF << 7; //Mute Status + Volume Status
				CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
				break ;
			}
			break ;
		case eUserControlReleased:
			break ;
	}
}
//****************************************************************************
void Cec_PowerStatusFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;
	switch(u8CecSysCmd[1]){
		case	eGiveDevicePowerStatus:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eReportPowerStatus;
				u8CecSysCmd[2] = CEC_POWER_STATE_ON;
				CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
			break;
		case	eReportPowerStatus:
			break;
	}
}
//****************************************************************************
void Cec_GeneralProtocolMessage(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;
	switch(u8CecSysCmd[1]){
		case	eFeatureAbort:

		if(u8CecSysCmd[2] == eSetSystemAudioMode)
		{
			iTE_Cec_Msg(("SetSystemAudioMode =>Broadcast OFF\n"));
			//mute speaker here

			u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
			u8CecSysCmd[1] = eSetSystemAudioMode;
			u8CecSysCmd[2] = CEC_SYS_AUDIO_STATE_OFF;
			CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
		}
			break;
		case	eAbort:
				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eFeatureAbort;
				u8CecSysCmd[2] = eAbort	;
				u8CecSysCmd[3] = CEC_ABORT_REASON_REFUSED;
				CecSys_PushToQueue(u8CecSysCmd, 4, &stTxQ);
			break;
	}
}
//****************************************************************************
void Cec_SysAudioCtlFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;
	switch(u8CecSysCmd[1]){
		case eGiveSystemAudioModeStatus:
			break ;
		case eSystemAudioModeRequest:
			break ;
		case eGiveAudioStatus:
			break ;
		case eReportAudioStatus :
			//record audio volume and mute status here

			g_u8Volume = u8CecSysCmd[2] & 0x7F;
			g_bMute	   = (u8CecSysCmd[2] & 0x80) >> 7 ;
			iTE_Cec_Msg(("Audio Volume %d, Mute: %d\n", g_u8Volume, g_bMute));

			if(g_u8Volume >= 0x64) //maximum
				g_u8Volume = 0x64 ;
			else if(g_u8Volume <= 0x00) //minimum
				g_u8Volume = 0x00 ;

			break ;
		case eSystemAudioModeStatus:
			//record current status of system audio mode
			g_bAudioMode = u8CecSysCmd[2] & 0x01;
			break ;
		case eSetSystemAudioMode:
			u8Temp = u8CecSysCmd[0] & 0x0F;

			if(u8Temp == DEVICE_ID_BROADCAST)
			{
				if(u8CecSysCmd[2] == CEC_SYS_AUDIO_STATE_OFF){
					//unmutes Tv's speaker

					//request ARC termination
					if((u8CecSysCmd[0] >> 4) == DEVICE_ID_AUDIO)
					{
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
						u8CecSysCmd[1] = eRequestARCTermination;
						CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
					}
				}

				if(u8CecSysCmd[2] ==  CEC_SYS_AUDIO_STATE_ON){
					//mutes Tv's speaker

					//request ARC Initiation
					if((u8CecSysCmd[0] >> 4) == DEVICE_ID_AUDIO)
					{
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
						u8CecSysCmd[1] = eRequestARCInitiation;
						CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
					}
				}
			}
			break ;
	}
}
//****************************************************************************
void Cec_AudioRateCtrlFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_ARCFeature(iTE_u8 *u8CecSysCmd)
{
	iTE_u8	u8Temp;

	switch(u8CecSysCmd[1]){
		case	eInitiateARC:
				#if EN_IT6601_ARC
				// function implement for ARC Tx initiation
				IT6601_ARCTxInitiation() ;
				#endif

				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eReportARCInitiated;
				CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
			break;
		case	eReportARCInitiated:
			break;
		case	eReportARCTerminated:
			break;
		case	eRequestARCInitiation:
			break;
		case	eRequestARCTermination:
			break;
		case	eTerminateARC:
				#if EN_IT6601_ARC
				// function implement for ARC Tx termination
				IT6601_ARCTxTermination() ;
				#endif

				u8Temp = u8CecSysCmd[0] >> 4;
				u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8Temp;
				u8CecSysCmd[1] = eReportARCTerminated;
				CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
			break;
	}
}
//****************************************************************************
void Cec_DynAutoLipsyncFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_HECFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
	}
}
//****************************************************************************
void Cec_AudioFormatFeature(iTE_u8 *u8CecSysCmd)
{
	switch(u8CecSysCmd[1]){
		case eRequestShortAudioDescription:

		//report audio type here
		break ;
	}
}
//****************************************************************************
iTE_u1 CecSys_PullFromQueue(iTE_pu8 pu8Cmd, iTE_pu8 pu8Len, stCecQ *pstQ)
{
	iTE_u8	u8RPtr = pstQ->u8RPtr;
	iTE_u8	u8WPtr = pstQ->u8WPtr;
	iTE_pu8	pu8Queue = pstQ->pu8Buf;
	iTE_u8 	u8QMaxSize = pstQ->u8MaxSize;
	iTE_u8	u8FreeSize;

	if(u8WPtr < u8RPtr){
		u8FreeSize = u8QMaxSize - u8RPtr + u8WPtr;
	}else{
		u8FreeSize = u8WPtr - u8RPtr;
	}

	if(u8FreeSize){
		iTE_u8	u8Len;

		u8Len = pu8Queue[u8RPtr++ & (u8QMaxSize - 1)];
		*pu8Len = u8Len;
		if(u8Len > (u8FreeSize - 1)){
			iTE_MsgE(("[Cec] Queue Empty, %X, %X, %X\n", u8WPtr, u8RPtr, u8Len));
			pstQ->u8RPtr = 0;
			pstQ->u8WPtr = 0;
			return iTE_FALSE;
		}
		u8FreeSize = u8Len;
		while(u8FreeSize--){
			*pu8Cmd++ = pu8Queue[u8RPtr++ & (u8QMaxSize - 1)];
		}
		pstQ->u8RPtr = u8RPtr & (u8QMaxSize - 1);
		iTE_Cec_Msg(("-CecSys_PullFromQueue wPtr=%02X, rPtr=%02X, u8Len=%X\n", u8WPtr, u8RPtr, *pu8Len));
		return iTE_TRUE;
	}
	*pu8Len = 0;
	return iTE_FALSE;
}
//****************************************************************************
void CecSys_TxCmdFire(CecSys *pstCecSta, stCecQ *pstTxQ)
{
	iTE_u8	u8Len;

	if(CecSys_PullFromQueue(u8CecSysCmd, &u8Len, pstTxQ) == iTE_TRUE){
		Cec_CmdFire(u8CecSysCmd, u8Len);
		STA_CHANGE(pstCecSta->u8Sta, CEC_TX_FIRE_MASK, CEC_TX_FIRE_SET);
		pstCecSta->u8TxReFireCnt = 0;
	}
}
//****************************************************************************
void CecSys_TxPollingMsg(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La & 0xF) | (pstCecSta->u8La << 4);

	Cec_CmdFire(u8CecSysCmd, 1);
	STA_CHANGE(pstCecSta->u8Sta, CEC_TX_FIRE_MASK, CEC_TX_FIRE_SET);
	pstCecSta->u8TxReFireCnt = 0;
	STA_CHANGE(pstCecSta->u8Sta, CEC_TX_POLL_MASK, CEC_TX_POLL_SET);
}
//****************************************************************************
void CecSys_TxReFire(CecSys *pstCecSta)
{
	if(pstCecSta->u8TxReFireCnt < CEC_RE_FIRE_MAX){
		Cec_Fire();
		pstCecSta->u8TxReFireCnt++;
		STA_CHANGE(pstCecSta->u8Sta, CEC_TX_FIRE_MASK, CEC_TX_FIRE_SET);
	}else{
		iTE_MsgE(("[Cec] Fire Faill > %d\n", CEC_RE_FIRE_MAX));
		pstCecSta->u8TxReFireCnt = 0;
	}
}
//****************************************************************************
void CecSys_TxReportPA(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
	u8CecSysCmd[1] = eReportPhysicalAddress;
	u8CecSysCmd[2] = pstCecSta->u8PaAB;
	u8CecSysCmd[3] = pstCecSta->u8PaCD;
	u8CecSysCmd[4] = pstCecSta->u8Type;
	CecSys_PushToQueue(u8CecSysCmd, 5, &stTxQ);
}
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
void CecSys_TxStandby(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
	u8CecSysCmd[1] = eStandBy;
	CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
}

void CecSys_TxVolumeDown(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
	u8CecSysCmd[1] = eUserControlPressed;
	u8CecSysCmd[2] = eVolumeDown ;
	CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
}

void CecSys_TxVolumeUp(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
	u8CecSysCmd[1] = eUserControlPressed;
	u8CecSysCmd[2] = eVolumeUp ;
	CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
}

void CecSys_TxVolumeMute(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
	u8CecSysCmd[1] = eUserControlPressed;
	u8CecSysCmd[2] = eMute ;
	CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
}

void CecSys_TxPowerOn(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
	u8CecSysCmd[1] = eUserControlPressed;
	u8CecSysCmd[2] = ePowerOnFunction ;
	CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
}

void CecSys_TxPowerOff(CecSys *pstCecSta)
{
	u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_AUDIO;
	u8CecSysCmd[1] = eUserControlPressed;
	u8CecSysCmd[2] = ePowerOffFunction ;
	CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
}

//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
//****************************************************************************
void CecSys_TxDoneCb(iTE_u8 u8TxSta1, iTE_u8 u8TxSta2, iTE_u8 u8TxHeader, iTE_u8 u8TxOpCode)
{
	STA_CHANGE(pstCecSta->u8Sta, CEC_TX_STA_MASK | CEC_TX_DONE_MASK, (u8TxSta1 << CEC_TX_STA_SHIFT) | CEC_TX_DONE_SET);
	if(u8TxSta1 == 0x03){
		if(u8TxSta2 == 0x02){
			STA_CHANGE(pstCecSta->u8Sta, CEC_TX_STA_MASK | CEC_TX_DONE_MASK, CEC_TX_STA_NACK | CEC_TX_DONE_SET);
		}else if(u8TxSta2 == 0x01){
			STA_CHANGE(pstCecSta->u8Sta, CEC_TX_STA_MASK | CEC_TX_DONE_MASK, CEC_TX_STA_ACK | CEC_TX_DONE_SET);
		}
	}
	STA_CHANGE(pstCecSta->u8Sta, CEC_TX_FIRE_MASK, CEC_TX_FIRE_CLR);
	g_u8TxCurHeader = u8TxHeader;
	g_u8TxCurOpCode = u8TxOpCode;
}
//****************************************************************************
void CecSys_RxDoneCb(iTE_pu8 pu8CecSysCmd, iTE_u8 u8Size)
{
//	iTE_pu8	pu8Ptr = u8CecSysCmd;
	CecSys_PushToQueue(pu8CecSysCmd, u8Size, &stRxQ);

	iTE_Cec_Msg(("*********\n"));
	while(u8Size--){
		iTE_Cec_Msg(("%02X \n", *pu8CecSysCmd++));
	}
	iTE_Cec_Msg(("*********\n"));

}
//****************************************************************************
void CecSys_Init(iTE_u8 u8PaAB, iTE_u8 u8PaCD, iTE_u8 u8SelPort)
{
	iTE_u8	u8Temp;
	iTE_pu8	pu8Ptr;
//	iTE_u16	u16Temp;
	iTE_Cec_Msg(("CecSys_Init %02X%02X, %x\n", u8PaAB, u8PaCD, u8SelPort));

	Cec_TxDoneCb = CecSys_TxDoneCb;
	Cec_RxDoneCb = CecSys_RxDoneCb;

	stTxQ.u8RPtr = 0;
	stTxQ.u8WPtr = 0;
	stTxQ.pu8Buf = u8TxQueue;
	stTxQ.u8MaxSize = CEC_TX_QUEUE_SIZE;

	stRxQ.u8RPtr = 0;
	stRxQ.u8WPtr = 0;
	stRxQ.pu8Buf = u8Rxqueue;
	stRxQ.u8MaxSize = CEC_RX_QUEUE_SIZE;

	u8Temp = sizeof(u8CecList);
	pu8Ptr = (iTE_pu8)u8CecList;
	while(u8Temp--){
		*pu8Ptr++ = 0xFF;
	}
	// reset CEC eng

	pstCecSta->u8PaAB = u8PaAB;
	pstCecSta->u8PaCD = u8PaCD;
	pstCecSta->u8La = DEVICE_ID_TV;
	pstCecSta->u8Type = CEC_DEV_TYPE_TV;
	pstCecSta->u8Sta = 0;

#if 0
	u16Temp = u8PaAB;
	u16Temp <<= 8;
	u16Temp |= u8PaCD;
	for(u8Temp = 0; u8Temp < 5; u8Temp++){
		if((u16Temp & (~u16PaMask[u8Temp])) == 0x00){
			pstCecSta->u8Stage = u8Temp;
		}
	}
#else
	if(u8PaCD){
		if(u8PaCD & 0x0F){
			pstCecSta->u8Stage = 4;
		}else{
			pstCecSta->u8Stage = 3;
		}
	}else{
		if(u8PaAB){
			if(u8PaAB & 0x0F){
				pstCecSta->u8Stage = 2;
			}else{
				pstCecSta->u8Stage = 1;
			}
		}else{
			pstCecSta->u8Stage = 0;
		}
	}
#endif
	CecSys_TxPollingMsg(pstCecSta);
}
//****************************************************************************
void CecSys_TxHandler(void)
{
	if((pstCecSta->u8Sta & CEC_TX_DONE_MASK) == CEC_TX_DONE_SET){
		iTE_u8 u8DesLA = g_u8TxCurHeader & 0x0F ;
		STA_CHANGE(pstCecSta->u8Sta, CEC_TX_DONE_MASK, CEC_TX_DONE_CLR);
		//if((pstCecSta->u8Sta & CEC_TX_POLL_MASK) == CEC_TX_POLL_SET){
		if(u8DesLA == pstCecSta->u8La){
			iTE_Cec_Msg(("Tx polling set\n")) ;
			STA_CHANGE(pstCecSta->u8Sta, CEC_TX_POLL_MASK, CEC_TX_POLL_CLR);
			switch(pstCecSta->u8Sta & CEC_TX_STA_MASK){
				case	CEC_TX_STA_ACK:
						iTE_Cec_Msg(("SwitchLA\n")) ;
						pstCecSta->u8La = u8SwitchLA[pstCecSta->u8La];
						if(pstCecSta->u8La != 0x0F){
							CecSys_TxPollingMsg(pstCecSta);
						}else{
							iTE_MsgE(("[Cec] no LogAdr\n"));
						}
					break;
				case	CEC_TX_STA_NACK:
						STA_CHANGE(pstCecSta->u8Sta, CEC_TX_INIT_MASK, CEC_TX_INIT_SET);
						Cec_LaSet(pstCecSta->u8La);
						CecSys_TxReportPA(pstCecSta);
					break;
			}
		}
		else if(u8DesLA == DEVICE_ID_AUDIO){
			if((pstCecSta->u8Sta & CEC_TX_STA_MASK) != CEC_TX_STA_ACK){
				CecSys_TxReFire(pstCecSta);
			}
			else{
				switch(g_u8TxCurOpCode){
					case eRequestARCInitiation:
						iTE_Cec_Msg(("Tx Cmd =>Give System Audio mode status\n"));
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8DesLA;
						u8CecSysCmd[1] = eGiveSystemAudioModeStatus;
						CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
					break ;
					case eGiveSystemAudioModeStatus:
						iTE_Cec_Msg(("Tx Cmd =>Give Device Vendor ID\n"));
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8DesLA;
						u8CecSysCmd[1] = eGiveDeviceVendorID;
						CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
					break ;
					case eGiveDeviceVendorID:
						iTE_Cec_Msg(("Tx Cmd =>Give OSD Name\n"));
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8DesLA;
						u8CecSysCmd[1] = eGiveOSDName;
						CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
					break ;
					case eGiveOSDName:
						iTE_Cec_Msg(("Tx Cmd =>Give Audio Status\n"));
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | u8DesLA;
						u8CecSysCmd[1] = eGiveAudioStatus;
						CecSys_PushToQueue(u8CecSysCmd, 2, &stTxQ);
					break ;
				}
			}
		}
		else{
			if((pstCecSta->u8Sta & CEC_TX_STA_MASK) != CEC_TX_STA_ACK){
				CecSys_TxReFire(pstCecSta);
			}
		}
	}
	if((pstCecSta->u8Sta & (CEC_TX_DONE_MASK | CEC_TX_FIRE_MASK)) == (CEC_TX_DONE_CLR | CEC_TX_FIRE_CLR)){
		// Fire new Cmd;
		CecSys_TxCmdFire(pstCecSta, &stTxQ);
	}
}
//****************************************************************************

void CecSys_RxHandler(void)
{
	iTE_u8	u8Len;
	if(CecSys_PullFromQueue(u8CecSysCmd, &u8Len, &stRxQ) == iTE_TRUE){
		iTE_u8	u8Cnt;

		iTE_Cec_Msg(("Cec header = %02X, %02X\n", u8CecSysCmd[0], u8CecSysCmd[1]));
		for(u8Cnt = 0; u8Cnt < SizeOfLookUpTable_CEC; u8Cnt++){
			if(u8CecSysCmd[1] == LookUpTable_CEC[u8Cnt].cmd){
				iTE_u8	u8Size =LookUpTable_CEC[u8Cnt].size;
				iTE_Cec_Msg(("CEC Rx Cmd => %s\n", LookUpTable_CEC[u8Cnt].CmdString));
				if(u8Len != (u8Size & 0x1F)){
					if((u8CecSysCmd[1] == eSystemAudioModeRequest) && (u8Len == 2)){
						u8CecSysCmd[0] = (pstCecSta->u8La << 4) | DEVICE_ID_BROADCAST;
						u8CecSysCmd[1] = eSetSystemAudioMode;
						u8CecSysCmd[2] = CEC_SYS_AUDIO_STATE_OFF	;

						//Mute Speaker here

						CecSys_PushToQueue(u8CecSysCmd, 3, &stTxQ);
					}
					else if(u8CecSysCmd[1] == eSetOSDName){
						//record OSD name here
					}
					else{
						iTE_MsgE(("[Cec] Invalid Size %d != %d\n", u8Len, (u8Size & 0x1F)));
					}
				}else{
					iTE_u8	u8Follower = u8CecSysCmd[0] & 0xF;
					iTE_u8	u8Temp = u8Size & 0xE0;
					if(((u8Temp == eDirectly) && (u8Follower == DEVICE_ID_BROADCAST)) || ((u8Temp == eBroadcast) && (u8Follower != DEVICE_ID_BROADCAST))){
						iTE_MsgE(("[Cec] Wrong Follower\n"));
					}else{

						switch(LookUpTable_CEC[u8Cnt].eType){
							case	eOTPFeature:
									Cec_OTPFeature(u8CecSysCmd);
								break;
							case	eRoutingCtlFeature:
									Cec_RoutingCtlFeature(u8CecSysCmd);
								break;
							case	eStandByFeature:
									Cec_StandByFeature(u8CecSysCmd);
								break;
							case	eOTRFeature:
									Cec_OTRFeature(u8CecSysCmd);
								break;
							case	eTimerProgFeature:
									Cec_TimerProgFeature(u8CecSysCmd);
								break;
							case	eSysInfoFeature:
									Cec_SysInfoFeature(u8CecSysCmd);
								break;
							case	eDeckCtrlFeature:
									Cec_DeckCtrlFeature(u8CecSysCmd);
								break;
							case	eTunerCtrlFeature:
									Cec_TunerCtrlFeature(u8CecSysCmd);
								break;
							case	eVenSpecCmdFeature:
									Cec_VenSpecCmdFeature(u8CecSysCmd);
								break;
							case	eOSDDisplayFeature:
									Cec_OSDDisplayFeature(u8CecSysCmd);
								break;
							case	eDevOSDTransferFeature:
									Cec_DevOSDTransferFeature(u8CecSysCmd);
								break;
							case	eDevMenuCtrlFeature:
									Cec_DevMenuCtrlFeature(u8CecSysCmd);
								break;
							case	eRemoteCtlPassthroughFeature:
									Cec_RemoteCtlPassthroughFeature(u8CecSysCmd);
								break;
							case	ePowerStatusFeature:
									Cec_PowerStatusFeature(u8CecSysCmd);
								break;
							case	eGeneralProtocolMessage:
									Cec_GeneralProtocolMessage(u8CecSysCmd);
								break;
							case	eSysAudioCtlFeature:
									Cec_SysAudioCtlFeature(u8CecSysCmd);
								break;
							case	eAudioRateCtrlFeature:
									Cec_AudioRateCtrlFeature(u8CecSysCmd);
								break;
							case	eARCFeature:
									Cec_ARCFeature(u8CecSysCmd);
								break;
							case	eDynAutoLipsyncFeature:
									Cec_DynAutoLipsyncFeature(u8CecSysCmd);
								break;
							case	eHECFeature:
									Cec_HECFeature(u8CecSysCmd);
								break;
								case eAudioFormatFeature:
									Cec_AudioFormatFeature(u8CecSysCmd) ;
							default:
								break;
						}
					}
				}
				break;
			}
		}
	}
}
//****************************************************************************
void CecSys_TxTest(iTE_u8 u8Cmd)
{
	switch(u8Cmd){
		case	0:	CecSys_TxStandby(pstCecSta);			break ;
		case	1:	CecSys_TxVolumeDown(pstCecSta) ;		break ;
		case	2:	CecSys_TxVolumeUp(pstCecSta) ;			break ;
		case	3:	CecSys_TxVolumeMute(pstCecSta) ;		break ;
		case	4: 	CecSys_TxVolumeMute(pstCecSta) ;		break ;
		case 	5:	CecSys_TxPowerOn(pstCecSta) ;			break ;
		case	6:	CecSys_TxPowerOff(pstCecSta) ;			break ;
	}
}
#endif


