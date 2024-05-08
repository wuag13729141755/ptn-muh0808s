///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Remote.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
#ifndef __IT6602_REMOTE_H__
#define __IT6602_REMOTE_H__

#ifdef Enable_IR
//****************************************************************************
//
// Remote.h
//
// NEC Remocon Key code define
//
//****************************************************************************

//#define ITE_IR


#ifdef ITE_IR

//ITE Remote control define !!
#define CUSTOM_CODE	0xC0
#define CUSTOM_BAR	 	0x3F
//
#define IR_PWR_CODE		0x50

#define IR_NO0_CODE 0x08
#define IR_HDMI1_CODE		0x88
#define IR_HDMI2_CODE		0x48
//#define IR_NO1_CODE 0x88
//#define IR_NO2_CODE 0x48
#define IR_NO3_CODE 0xC8
#define IR_NO4_CODE 0x28
#define IR_NO5_CODE 0xA8
#define IR_NO6_CODE 0x68
#define IR_NO7_CODE 0xE8
#define IR_NO8_CODE 0x18
#define IR_NO9_CODE 0x98


#define IR_ChUp_CODE		0x7A
#define IR_ChDown_CODE		0x20

#define IR_VolUp_CODE		0xD8
#define IR_VolDown_CODE		0x32
#define IR_Menu_CODE		0xE0
#define IR_Exit_CODE			0xA0

#define IR_Up_CODE			0x00
#define IR_Down_CODE		0x80
#define IR_Left_CODE			0xC0
#define IR_Right_CODE		0x40
#define IR_Sel_CODE			0xF8

#define IR_REW_CODE		0xB2
#define IR_FFORWARD_CODE	0xF0
#define IR_PLAY_CODE		0xB0
#define IR_STOP_CODE		0x70
//#define IR_PAUSE_CODE		0xB0 //the same as play key
#define IR_PAUSE_CODE		0xBA	// last key

#define IR_MP3_CODE		0xC2	// mp3 key
#define IR_VIDEO_CODE		0xD0	// video key




#define IR_RETURN_CODE 		0xA0
#define IR_EFFECT_CODE 		0x86
#define IR_VFD_CODE 		0x01

#define IR_SPDIF_CODE 		0x52
#define IR_AUX1_CODE 		0xD2
#define IR_AUX2_CODE 		0xFA


#define IR_Mute_CODE		0x58
#define IR_Info_CODE			0x52 // i key

#define IR_SLEEP_CODE 		0xA2	// red
#define IR_MODE_CODE 		0x62	// green
#define IR_LIGHT_CODE 		0x12	// yellow
#define IR_BLUE_CODE 		0x92	//blue





#else

//Zinwell  remote control define !!
#define CUSTOM_CODE	0x00
#define CUSTOM_BAR	 	0x1F





#define IR_PWR_CODE		0xE1
#define IR_Mute_CODE		0x71
#define IR_Menu_CODE		0xF1

#define IR_Up_CODE			0x41
#define IR_Down_CODE		0xA1
#define IR_Left_CODE			0xC1
#define IR_Right_CODE		0x81
#define IR_Sel_CODE			0x21

#define IR_REW_CODE		0x5A
#define IR_FFORWARD_CODE	0xBA
#define IR_PLAY_CODE		0xDA
#define IR_STOP_CODE		0xBA
#define IR_PAUSE_CODE		0x3A

#define IR_HDMI1_CODE		0xA6
#define IR_HDMI2_CODE		0x7A

#define IR_SLEEP_CODE 		0x06
#define IR_MODE_CODE 		0xB1
#define IR_ChDown_CODE 		0x31

#define IR_Exit_CODE 		0xD1
#define IR_EFFECT_CODE 		0x86
#define IR_ChUp_CODE 		0x01

#define IR_SPDIF_CODE 		0x52
#define IR_VolUp_CODE 		0xD2
#define IR_VolDown_CODE 	0xFA



#endif

#define NO_KEY				0xFF

#define KEYPAD_IN1			0xFE
#define KEYPAD_IN2			0xFD
#define KEYPAD_IN3			0xFB
#define KEYPAD_IN4			0xF7
#define KEYPAD_IN5			0xEF
#define KEYPAD_IN6			0xDF
#define	KEYPAD_PWR		0xBF


//========================================================
void Remote_Receive_Pro(void);
void Rmo_Receive_Err(void);

void IRHandler(void);
void initialTimer0(void);
void initial_INT1(void);


#endif

#endif
