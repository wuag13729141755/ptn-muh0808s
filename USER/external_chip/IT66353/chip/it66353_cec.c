///*****************************************
//  Copyright (C) 2009-2020
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <it6635_cec.c>
//   @author Kenneth.Hung@ite.com.tw
//   @date   2020/12/30
//   @fileversion: IT66353_SAMPLE_1.06
//******************************************/
#include "IT66353/chip/IT66353_Cec.h"
#if (IT6635_EN_CEC == iTE_TRUE)
iTE_ps8	_CODE	sCecTxStatus[4] = {"ACK", "NACK", "RETRY", "FAIL"};
//iTE_u8	u8CecRxHeader[3];
iTE_u8	u8CecCmd[20];


//****************************************************************************
void (*Cec_TxDoneCb)(iTE_u8 u8TxSta1, iTE_u8 u8TxSta2, iTE_u8 u8TxHeader, iTE_u8 u8TxOpCode) = NULL;
void (*Cec_RxDoneCb)(iTE_u8 *u8CecCmd, iTE_u8 u8Size) = NULL;
#if 0
//****************************************************************************
void Cec_TxDoneCb(iTE_u8 u8TxSta1, iTE_u8 u8TxSta2)
{
	iTE_Cec_Msg((""));
}
//****************************************************************************
void Cec_RxDoneCb(iTE_u8 *u8CecCmd, iTE_u8 u8Size)
{
	iTE_pu8	pu8Ptr = u8CecCmd;
	iTE_Cec_Msg(("\\\\\\\\\\\\n"));
	while(u8Size--){
		iTE_Cec_Msg(("%02X \n", *pu8Ptr++));
	}
	iTE_Cec_Msg(("\\\\\\\\\\\\n"));
}
//****************************************************************************
void Cec_FireStatus(iTE_u1 bTxDone)
{
	iTE_u8	u8Initiator, u8Follower;
	iTE_u8	u8Sta;

	u8Sta = Cec_R(0x54);

	iTE_Cec_Msg(("OpCode[%02X] %s, %02X\n", Cec_R(0x11), sCecTxStatus[(Cec_R(0x44) & 0x0C) >> 2], u8Sta));

	u8Initiator = Cec_R(0x10);
	u8Follower = u8Initiator & 0x0F;
	u8Initiator >>= 4;

	if(u8Initiator == u8Follower){
		u8Sta &= 0x03;
		if(bTxDone){
			if(u8Sta == 0x01){	// ACK
				return iTE_FALSE;
			}
		}else{
			if(u8Sta != 0x02){		// NACK
				return iTE_FALSE;
			}
		}
	}
	return iTE_TRUE;
}
#endif
//****************************************************************************
void Cec_RxFifoReset(void)
{
	Cec_W(0x52, 0x60);
	Cec_W(0x52, 0xE0);
	Cec_W(0x52, 0x80);
//	u8CecRxHeader[0] = 0;
}
//****************************************************************************
void Cec_RxCmdPush(iTE_u8 *u8Header)
{
	iTE_u8	u8Size;
	iTE_pu8	pu8Ptr;

	u8Size = u8Header[2] & 0x1F;

	if(u8Size < 2){
		iTE_MsgE(("[Cec] Rx Cmd Fail %X\n", u8Header[2] & 0x60));
	}else{
		pu8Ptr = &u8Header[2];
		u8Size -= 2;
		iTE_Cec_Msg(("Size = %d\n", u8Size));
		if(u8Size){
			Cec_Rb(0x50, u8Size, pu8Ptr);
		}
		if(Cec_RxDoneCb){
			Cec_RxDoneCb(u8Header, u8Size+2);
		}else{
			iTE_MsgE(("[Cec] CecSys_Init Fail\n"));
		}
	}
}
//****************************************************************************
void Cec_RxCmdGet(void)
{
	iTE_u8	u8Reg51[3];

	Cec_Rb(0x51, 0x03, u8Reg51);
	u8Reg51[1] &= 0x0F;
	do{
		if(u8Reg51[0] & 0xCC){
			iTE_MsgE(("[Cec] CecRx_FifoReset\n"));
			Cec_RxFifoReset();
			return;
		}else{
			iTE_Cec_Msg(("%02X, %02X, %02X\n", u8Reg51[0], u8Reg51[1], u8Reg51[2]));
			while(u8Reg51[1]--){
				Cec_Rb(0x4D, 0x03, u8CecCmd);
				Cec_RxCmdPush(u8CecCmd);
			}
			Cec_Rb(0x51, 0x03, u8Reg51);
			u8Reg51[1] &= 0x0F;
		}
	}while(u8Reg51[1]);
}
//****************************************************************************
void Cec_Init(iTE_u8 u8TimerUnit)
{
	iTE_Cec_Msg(("Cec_Init %X\n", u8TimerUnit));

	Cec_W(0x08, 0x4C);
	Cec_W(0x08, 0x48);
	Cec_W(0x22, 0x0F);	// default LogAdr
	Cec_W(0x09, 0x40 | (CEC_RX_SELF_DIS << 5));
	Cec_W(0x0B, 0x14);
	Cec_W(0x0C, u8TimerUnit);
	Cec_Set(0x08, 0x04, CEC_RST << 2);
	Cec_Set(0x09, 0x02, 0x00);
	Cec_W(0x06, 0x00);
	Cec_Set(0x08, 0x01, 0x01);
	Cec_RxFifoReset();
//	Cec_W(0x0A, 0x03);
	Cec_W(0x0A, 0x23);
	Cec_Set(0x0A, 0x40, 0x40);
}
//****************************************************************************
void Cec_Irq(void)
{
	iTE_u8	u8CecSta = Cec_R(0x4C);

	iTE_Cec_Msg(("Cec_Irq %02X\n", u8CecSta));
	Cec_W(0x4C, u8CecSta);
	if(u8CecSta & 0x28){			// Cec Initiator int
		iTE_u8	u8TxSta1 = (Cec_R(0x44) & 0x0C) >> 2;
		iTE_u8	u8TxSta2 = Cec_R(0x54) & 0x03;

		iTE_Cec_Msg(("TxOpCode[%02X] %s, %d\n", Cec_R(0x11), sCecTxStatus[u8TxSta1], u8TxSta2));
		if(Cec_TxDoneCb){
			Cec_TxDoneCb(u8TxSta1, u8TxSta2, Cec_R(0x10), Cec_R(0x11));
		}else{
			iTE_MsgE(("[Cec] CecSys_Init Fail\n"));
		}
	}
	if(u8CecSta & 0xD4){			// Cec receiver int
		if(u8CecSta & 0x04){
			iTE_MsgE(("[Cec] Rx Fail\n"));
			Cec_RxCmdGet();
		}
		if(u8CecSta & 0xC0){
			iTE_u8	u8Reg51[3];
			iTE_MsgE(("[Cec] Rx FIFO overflow %X\n", u8CecSta));
			Cec_Rb(0x51, 0x03, u8Reg51);
			iTE_Cec_Msg(("%02X, %02X, %02X\n", u8Reg51[0], u8Reg51[1], u8Reg51[2]));
			Cec_RxFifoReset();
		}
		if(u8CecSta & 0x10){
			iTE_Cec_Msg(("Rx Done\n"));
			Cec_RxCmdGet();
		}
	}
}
//****************************************************************************
void Cec_CmdFire(iTE_u8 *u8CecCmd, iTE_u8 u8Len)
{
	iTE_Cec_Msg(("Cec_CmdFire\n"));
	if(u8Len && (u8Len < 19)){
	//#if iTE_Msg_Print
		iTE_pu8	pu8Ptr = u8CecCmd;
	//#endif
		Cec_Wb(0x10, u8Len, u8CecCmd);
		Cec_W(0x23, u8Len);
		while(u8Len--){
			iTE_Msg(("%02x ", *pu8Ptr++));
		}
		iTE_Cec_Msg(("\n"));
		Cec_Fire();
	}
}
//****************************************************************************
void Cec_LaSet(iTE_u8 u8La)
{
	Cec_W(0x22, u8La);
}
//****************************************************************************
void Cec_Fire(void)
{
	iTE_u8	u8Temp;

	u8Temp = Cec_R(0x08) | 0x88;
	Cec_W(0x08, u8Temp & 0x7F);
	Cec_W(0x08, u8Temp);
}
//****************************************************************************
#endif
