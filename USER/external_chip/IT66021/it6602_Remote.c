///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Remote.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
#ifdef Enable_IR

//****************************************************************************
// Remote.c
//
// NEC format
//****************************************************************************
#include "Remote.h"
#include "mcu_conf.h"
#include "Utility.h"
#include "stdio.h"
#include "it6602.h"

#define InitBitReceiveReg() {	\
        Rmote_cnt++;			\
        In8bits = 0;			\
		g8bits_cnt = 0x80;		\
        Remote_State += 1;		\
								}
typedef enum {
	Header_In = 0,
	Header_9ms,
	Header_4ms,
	No_Header,
	Get_Custom_8bits,
	Custom_8bits,
	Save_CustomCode,
	CustomBar_8bits,
	Save_CustomBarCode,
	Data_8bits,
	Save_DataCode,
	DataBar_8bits,
	Save_DataBarCode,
	DataCode_Chk
}IR_Data;

BYTE idata   Remote_State = 0;
BYTE idata   In8bits;
BYTE idata   g8bits_cnt;
BYTE idata   Rmote_cnt = 0;
//WORD   RmoteWidth_cnt = 1080;
BYTE idata   k, SampleValue;
BYTE idata   IRCode[4];
BYTE idata   Byte32Code[32];
BYTE idata   g_keyQue = NO_KEY;


#define MAXKEY	23

_CODE BYTE IR2RCPTable[MAXKEY][MAXKEY]=
{

	{  IR_PWR_CODE,			RCP_SETUP_MENU },	// 0
	{  IR_Mute_CODE,			RCP_MUTE},				// 1
	{  IR_Menu_CODE,			RCP_ROOT_MENU },		// 2

	{  IR_Up_CODE,			RCP_UP },				// 3
	{  IR_Down_CODE,		RCP_DOWN },				// 4
	{  IR_Left_CODE,			RCP_LEFT },				// 5
	{  IR_Right_CODE,			RCP_RIGHT },				// 6
	{  IR_Sel_CODE,			RCP_SELECT },				// 7

	{  IR_REW_CODE,			RCP_REWIND},			// 8
	{  IR_FFORWARD_CODE,	RCP_FAST_FWD},			// 9
	{  IR_PLAY_CODE,			RCP_PLAY },				// 10
	{  IR_STOP_CODE,			RCP_STOP },				// 11
	{  IR_PAUSE_CODE,		RCP_PAUSE },			// 12

	{  IR_SLEEP_CODE,		RCP_SETUP_MENU },		// 13
	{  IR_MODE_CODE,		RCP_FAVORITE_MENU},		// 14
	//{  IR_LIGHT_CODE,		RCP_FWD},			// 15

	{  IR_Exit_CODE,			RCP_EXIT },				// 16
	{  IR_EFFECT_CODE,		RCP_ENTER},			// 17
//	{  IR_VFD_CODE,			RCP_BKWD},		// 18

	{  IR_VolUp_CODE,		RCP_VOL_UP},		// 19
	{  IR_VolDown_CODE,		RCP_VOL_DOWN},		// 20


	{  IR_ChUp_CODE,			RCP_FAVORITE_MENU},		// 21
	{  IR_ChDown_CODE,		RCP_CONTENTS_MENU}		// 22


//	{  IR_HDMI1_CODE,		RCP_RSVD },
//	{  IR_HDMI2_CODE,		RCP_RSVD },
};


void Remote_Receive_Pro(void)
{
	switch (Remote_State)
	{
		case Header_In:
			if (Remote_Receive_Pin == 0)
			{
				if (++Rmote_cnt<30)
					break;
				else
				{
					Remote_State = Header_9ms;
					break;
				}
			}
			else if (Rmote_cnt > 4)
			{
				SampleValue = 16;
				Remote_State = No_Header;
				break;
			}
			else
				{
				Rmo_Receive_Err();
				}
			break;
		case No_Header:
			if (Remote_Receive_Pin == 1)
				Rmote_cnt++;
			else
			{
				IE1 = 0;
				if (Rmote_cnt > SampleValue)
					In8bits = 0x80;
				else
					In8bits = 0x00;
				Byte32Code[0]=Rmote_cnt;
				k=1;
				g8bits_cnt = 0x40;
				Remote_State = Get_Custom_8bits;
				Rmote_cnt = 0;


//					IT6602_MSG(("!2\n"));
			}
			break;
		case Header_9ms:


			Rmote_cnt++;
			if (Remote_Receive_Pin == 0)
				break;
			else
			{
				if (Rmote_cnt < 50 || Rmote_cnt >= 110)
					Rmo_Receive_Err();
				else
				{
					Rmote_cnt = 0;
					Remote_State = Header_4ms;
				}
			}
			break;
		case Header_4ms:						// check 24>gRmo_cnt>8 --> 4ms

			Rmote_cnt++;
			if (Remote_Receive_Pin == 0)
			{
				if (Rmote_cnt < 20)
					Rmo_Receive_Err();
				else
				{
		                    IE1 = 0; 					//INT1 edge trigger flag --> clear
		                    k=0;
					Rmote_cnt = 0;
					In8bits = 0;
					g8bits_cnt = 0x80;
					Remote_State = Custom_8bits;
					SampleValue = 16;	// 12 for 11M crystal, 16 for 27M crystal
				}
			}
			else{
				if (Rmote_cnt < 60)
					break;
				else
					Rmo_Receive_Err();
			}
			break;
		case Get_Custom_8bits:

			if (IE1 == 1)					   //INT1 edge trigger flag assert?
			{
				IE1 = 0;					   //INT1 edge trigger flag --> clear
				if (Rmote_cnt <= 5)
					break;
				else
				{
					if (Rmote_cnt >SampleValue)
						In8bits = In8bits + g8bits_cnt;
					Byte32Code[k]=Rmote_cnt;
					Rmote_cnt = 0;
					g8bits_cnt >>= 1;
					k++;
					if (g8bits_cnt)
						break;
					else
						Remote_State = Save_CustomCode;
				}
			}
			else
			{
				if (++Rmote_cnt<40)
					break;
				else
				{
					Rmote_cnt = 0;
					Remote_State = Header_In;
				}
			}
			break;
		case Custom_8bits:
		case CustomBar_8bits:
		case Data_8bits:
		case DataBar_8bits:
			if (IE1 == 1)					   //INT1 edge trigger flag assert?
			{
				IE1 = 0;					   //INT1 edge trigger flag --> clear
				if (Rmote_cnt <= 5)
					break;
				else
				{
					if (Rmote_cnt >SampleValue)
						In8bits = In8bits + g8bits_cnt;
					Byte32Code[k]=Rmote_cnt;
					Rmote_cnt = 0;
					g8bits_cnt >>= 1;
					k++;
					if (g8bits_cnt)
						break;
					else
						Remote_State += 1;
				}
			}
			else
			{
				if (++Rmote_cnt<30)
					break;
				else
				{
					In8bits = In8bits + g8bits_cnt;
					Byte32Code[k]=Rmote_cnt;
					Rmote_cnt = 0;
					g8bits_cnt >>= 1;
					k++;
					if (g8bits_cnt)
						break;
					else
					{
						Remote_State += 1;
					}
				}
			}
			break;
		case Save_CustomCode:
			IRCode[0]=In8bits;
			InitBitReceiveReg();
			break;
		case Save_CustomBarCode:
			IRCode[1]=In8bits;
			InitBitReceiveReg();
			break;
		case Save_DataCode:
			IRCode[2]=In8bits;
			InitBitReceiveReg();
			break;
		case Save_DataBarCode:
			IRCode[3]=In8bits;
			InitBitReceiveReg();
			break;
		case DataCode_Chk:


			if ( IRCode[0]==CUSTOM_CODE && IRCode[1]==CUSTOM_BAR )
			{
				g_keyQue = IRCode[2];
			}
// disable ->			else
// disable ->			{
// disable ->				IT6602_MSG((" IRCode[0] %X \r\n",IRCode[0]));
// disable ->				IT6602_MSG((" IRCode[1] %X \r\n",IRCode[1]));
// disable ->			}


			Rmote_cnt = 0;
			Remote_State = Header_In;

			EX1 = 1;		// Enable ExInt0 for remote control
			IE1 = 0;
			TR0 = 0;
			break;
		default:
			break;
	}
}

void Rmo_Receive_Err(void)
{
    Rmote_cnt = 0;
    Remote_State = Header_In;
    EX1 = 1;		// Enable ExInt0 for remote control
    IE1 = 0;
    TR0 = 0;
}

//***********************************************************************
//  Function   : MAIN_Timer
//  Params     : void
//  Description: Service routine for timer T0.
//  Returns    : void
//***********************************************************************
void Timer0_Interrupt_Handler (void) interrupt 1 using 1
{
	TR0 = 0;	// stop timer
	TH0 = IR_COUNTER_VALUE >> 8;
	TL0 = IR_COUNTER_VALUE & 0xFF;
	TR0 = 1; // start timer
	Remote_Receive_Pro();

	//	P2_0=~P2_0;
	//	P2_1=~P2_1;


    return;
}
#ifndef SUPPORT_I2C_SLAVE
//***********************************************************************
//  Function   : external interrupt 1
//  Params     : void
//  Description: Service ExInt1
//  Returns    : void
//***********************************************************************
void ExInt1 (void) interrupt 2 using 2
{
	EX1 = 0;		// Disable ExInt0
	TR0 = 1;		// Enable timer 0
	ET0 = 1;		// Enable timer 0 overflow interrupt

//		P2_0=~P2_0;
//		P2_1=~P2_1;


}
#endif

void initial_INT1(void)
{
	EX1 = 1;	// Enable ExInt1 for IR functino
	IT1 = 1;	// set ExInt1 --> edge trigger for IR functino
}



//***********************************************************************
//  Function   : IRHandler
//  Params     : void
//  Description: IR command handler
//  Returns    : void
//***********************************************************************
void IRHandler(void)
{
#if 1
	unsigned char i;

	if(g_keyQue!=NO_KEY)
	{
		MHLRX_DEBUG_PRINTF(("!!! IRHandler() !!!g_keyQue= %X     \n", (int) g_keyQue));
		switch(g_keyQue)
		{
			case  IR_HDMI1_CODE:
				it6602PortSelect(0);
				break;

			case  IR_HDMI2_CODE:
				it6602PortSelect(1);
				break;

//			case  IR_AUX2_CODE:
//FIX_ID_003 xxxxx	//Add IT6602 Video Output Configure Table
//				IT6602ChangeTTLVideoOutputMode();
//FIX_ID_003 xxxxx
//				break;


	#ifdef Enable_IT6602_CEC
			case  IR_PWR_CODE:
				IT6602_CECPowerOffByITEHDMI(1);
				break;

			case  IR_NO4_CODE:
				IT6602_CECPowerOnByITEHDMI(0);
			case  IR_NO5_CODE:
			case  IR_NO6_CODE:
				IT6602_CECPowerOnByITEHDMI(1);
				break;
	#endif
//FIX_ID_005 xxxxx	//Add Cbus Event Handler
// disable ->			case IR_REW_CODE:		IT6602MSCCmdTest(B_DevCapChange); break;
// disable ->			case IR_PLAY_CODE:		IT6602MSCCmdTest(B_HPD); break;
// disable ->			case IR_PAUSE_CODE:		IT6602MSCCmdTest(B_PATH_EN); break;
// disable ->			case IR_FFORWARD_CODE:	IT6602MSCCmdTest(B_Set_DCAP_RDY); break;

// disable ->			case IR_Up_CODE:		IT6602MSCCmdTest(0x10); break;
// disable ->			case IR_Down_CODE:		IT6602MSCCmdTest(0x20); break;
// disable ->			case IR_Left_CODE:		IT6602MSCCmdTest(0x40); break;
// disable ->			case IR_Right_CODE:		IT6602MSCCmdTest(0x80); break;
//FIX_ID_005 xxxxx


			default:
				for(i=0;i<MAXKEY;i++)
				{
					if(IR2RCPTable[i][0]==g_keyQue)
						break;
				}
				if(i<MAXKEY)
					RCPKeyPush(IR2RCPTable[i][1]);

				break;

		}
		g_keyQue=NO_KEY;


	}
#endif
}



#endif
