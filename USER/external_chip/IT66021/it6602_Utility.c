///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Utility.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
//#include "bsp.h"
#include "it6602_config.h"
#include "it6602_mcu_conf.h"
#include "it6602_IO.h"
#include "it6602_Utility.h"
#include <stdio.h>



USHORT ucTickCount=0;
USHORT loopTicCount = 0;
USHORT prevTickCount;
USHORT MsdelayCnt =0;
#if 0
void init_printf(void)
{
#if 0
	SCON = 0x52;
	T2CON = 0x34;
	TR2 = FALSE;
#ifdef MEGAWIN82516
	TL2 = 0xFD;	//BAUD_SETTING;
	TH2 = 0xFF;	//BAUD_SETTING>>8;
	RCAP2L = 0xFD;	//BAUD_SETTING;
	RCAP2H = 0xFF;	//BAUD_SETTING>>8;
#else
	TL2 = BAUD_SETTING;
	TH2 = BAUD_SETTING>>8;
	RCAP2L = BAUD_SETTING;
	RCAP2H = BAUD_SETTING>>8;
#endif
	TR2 = TRUE;
	TI = 1;
	RI = 0;
	//EA=FALSE;
#endif
}

void initialTimer0(void)
{
#if 0
	TR0=0; // temporarily stop timer 0

	//TMOD &= 0x0F;	// Timer 1, Mode 0, 13 bit
	//TMOD |= 0x10;	// Timer 1, Mode 1, 16 bit

	TMOD &= 0xF0;	// Timer 0, Mode 0, 13 bit
	TMOD |= 0x01;	// Timer 0, Mode 1, 16 bit

	TH0=Tick1ms / 256;
	TL0=Tick1ms % 256;

	TR0	= 1; 	// restart the timer
	ET0	= 1;   	// Enable Timer Interrupt 0
#endif
}

void initialTimer1(void)
{
#if 0
	TR1=0; // temporarily stop timer 1

	//TMOD &= 0x0F;	 //Timer 1, Mode 0, 13 bit
	//TMOD |= 0x11;	 //Timer 1, Mode 1, 16 bit

	TMOD = 0x11;	 //Timer 1, Mode 1, 16 bit

	TH1= Tick1ms / 256;
	TL1= Tick1ms % 256;

	ucTickCount=0;

	TR1	= 1; 	// restart the timer
	ET1	= 1;   	// Enable Timer Interrupt 0
	EA=1;
#endif
}
#if 0
void system_tick(void) interrupt 3 {

	TR1=0; // temporarily stop timer 0

	TH1=Tick1ms / 256;
	TL1=Tick1ms % 256;

	ucTickCount++;

	TR1=1; // restart the timer
}
#endif

USHORT getloopTicCount(void)
{
    USHORT loopms;

    if(loopTicCount>ucTickCount)
    {
        loopms =  (0xffff-(loopTicCount-ucTickCount));
    }
    else
    {
        loopms =  (ucTickCount-loopTicCount);
    }
    loopTicCount = ucTickCount;
//    MHLRX_DEBUG_PRINTF(" loop ms  = %u \n",loopms));
    return  loopms;

}



USHORT CalTimer(USHORT SetupCnt)
{
    if(SetupCnt>ucTickCount)
    {
        return (0xffff-(SetupCnt-ucTickCount));
    }
    else
    {
        return (ucTickCount-SetupCnt);
    }
}

int TimeOutCheck(USHORT timer, USHORT x)
{
    if(CalTimer(timer)>=x)
    {
        return TRUE ;
    }
    return FALSE ;
}


BOOL IsTimeOut(USHORT x)
{
    if(CalTimer(prevTickCount) >= x )
    {
        prevTickCount = ucTickCount ;
        return TRUE ;
    }
    return FALSE ;
}

USHORT GetCurrentVirtualTime(void)
{
    return ucTickCount ;
}
#endif

void It6602_delay1ms(USHORT ms)
{
#if 0
    USHORT ucStartTickCount,diff;



    ucStartTickCount=ucTickCount;
        do
        {
            if(ucTickCount<ucStartTickCount)
                {
                    diff=0xffff-(ucStartTickCount-ucTickCount);
                }
            else
                {
                    diff=ucTickCount-ucStartTickCount;
                }
        }while(diff<ms);
#else
	delay_ms(ms);
#endif
}

