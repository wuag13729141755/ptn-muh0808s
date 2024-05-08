///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <mcu.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/

#ifndef _IT6602_MCU_CONF_H_
#define _IT6602_MCU_CONF_H_

//#include "515xram.h"
#include "it6602_typedef.h"

//#define MEGAWIN82516 TRUE	//IT6602 Demo board for TI442x platform

#ifdef MEGAWIN82516
//#include "REG_MPC82G516.H"
#else
//#include "Reg_c51.h"
#endif

#if 0//def Enable_IR
#include "Remote.h"
#endif



#define _IT6602_BOARD_



#ifndef _MCU_8051_
//#define _MCU_8051_
#endif

//#define SUPPORT_UART_CMD

//#define 		Enable_IT6602_CEC		//for ARC and HEAC Function

#ifdef Enable_IT6602_CEC

	#ifdef Enable_IT6602_CEC
		#include "it6602_cec.h"
	#endif

	#define	DEBUG_IT6602_CEC
	#define IT6602CECGPIOid		0		// for identife IT6602_CEC use Pin 1.0 & 1.1
#endif



/****************************************************************************/

/****************************************************************************/

#define IT6602A0_HDMI_ADDR 0x90	//Hardware Fixed I2C address of IT6602 HDMI
#define IT6602B0_HDMI_ADDR 0x92	//Hardware Fixed I2C address of IT6602 HDMI
#define MHL_ADDR 0xE0	//Software programmable I2C address of IT6602 MHL
#define EDID_ADDR 0xA8	//Software programmable I2C address of IT6602 EDID RAM
#define CEC_ADDR 0xC8	//Software programmable I2C address of IT6602 CEC


#define DP_ADDR 0x90
#define ADC_ADDR 0x90



#define RXDEV           0
#define MAXRXDEV        1
/****************************************************************************/

/****************************************************************************/
//#define HDMI_DEV		0
#define SWADR	 	0x96
#define EDID_HEAD	0xA0

#define DELAY_TIME        1
#define IDLE_TIME        100

#define HIGH            1
#define LOW                0
#define ACTIVE          1
//#define DISABLE         0

/****************************************************************************/
/****************************************************************************/
#if 0
#ifdef MEGAWIN82516
sbit Hold_Pin           		= P2^0 ;	//for it6602 with MEGAWIN82516
sbit  SEL_PORT_1 		= P3^0;
#else
sbit Hold_Pin           		= P1^5 ;	//for it6602 demo board
sbit SEL_PORT_1           	= P1^6 ;	//for it6602 demo board
#define _HPDMOS_
#endif

sbit  EDID_WP1 	= P0^4;
sbit  EDID_WP2 	= P0^7;
sbit  EDID_WP3 	= P1^4;

//sbit  gpHPD0 	= P1^7;

sbit  gpHPD0 	= P4^3;



sbit VsyncToggle = P0^2;

sbit  LED1 		= P2^7;
//sbit  LED1 		= P2^0;
sbit  LED2 		= P2^6;


sbit  AFE_RESET_PIN 		= P3^5;


#define 	Remote_Receive_Pin 	P3_3

#ifdef _HPDMOS_
    #define HPDON        0
    #define HPDOFF        1
#else
    #define HPDON        1
    #define HPDOFF        0
#endif

#else
#define SEL_PORT_1	1
#define Remote_Receive_Pin	0
#endif

#endif
