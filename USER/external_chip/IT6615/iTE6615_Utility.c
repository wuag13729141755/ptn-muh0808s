///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Utility.c>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/

#include "iTE6615_Config.h"
#include "iTE6615_Typedef.h"

#ifdef _MCU_8051_
#include "iTE6615_mcu.h"
#include "iTE6615_IO.h"
#endif

#include "iTE6615_Utility.h"
#include "stdio.h"

#ifdef _MCU_8051_
_IDATA u16 ucTickCount = 0;
_IDATA u16 loopTicCount = 0;
_IDATA u16 loopTicCount2 = 0;
_IDATA u16 prevTickCount;
_IDATA u16 MsdelayCnt = 0;

void init_printf(void)
{
    SCON = 0x52;
    T2CON = 0x34;
    TR2 = FALSE;
    TL2 = BAUD_SETTING;
    TH2 = BAUD_SETTING >> 8;
    RCAP2L = BAUD_SETTING;
    RCAP2H = BAUD_SETTING >> 8;

    TR2 = TRUE;
    TI = 1;
    RI = 0;
    //EA=FALSE;
}

void initialTimer0(void)
{
    TR0 = 0; // temporarily stop timer 0

    //TMOD &= 0x0F;    // Timer 1, Mode 0, 13 bit
    //TMOD |= 0x10;    // Timer 1, Mode 1, 16 bit

    TMOD &= 0xF0;    // Timer 0, Mode 0, 13 bit
    TMOD |= 0x01;    // Timer 0, Mode 1, 16 bit

    TH0 = Tick1ms / 256;
    TL0 = Tick1ms % 256;

    TR0 = 1;     // restart the timer
    ET0 = 1;       // Enable Timer Interrupt 0
}

void initialTimer1(void)
{
    TR1 = 0; // temporarily stop timer 1

    //TMOD &= 0x0F;     //Timer 1, Mode 0, 13 bit
    //TMOD |= 0x11;     //Timer 1, Mode 1, 16 bit

    TMOD = 0x11;     //Timer 1, Mode 1, 16 bit

    TH1 = Tick1ms / 256;
    TL1 = Tick1ms % 256;

    ucTickCount = 0;

    TR1 = 1;     // restart the timer
    ET1 = 1;       // Enable Timer Interrupt 0
    EA = 1;
}

void system_tick(void) interrupt 3 {

    TR1 = 0; // temporarily stop timer 0

    TH1 = Tick1ms / 256;
    TL1 = Tick1ms % 256;

    ucTickCount++;

    TR1 = 1; // restart the timer
}


u16 getloopTicCount()
{
    _IDATA u16 loopms;

    if (loopTicCount > ucTickCount)
    {
        loopms = (0xffff - (loopTicCount - ucTickCount));
    }
    else
    {
        loopms = (ucTickCount - loopTicCount);
    }
    loopTicCount = ucTickCount;
    //    HDMITX_DEBUG_PRINT(" loop ms  = %u\r\n",loopms));
    return  loopms;
}

u16 getloopTicCount2()
{
    _IDATA u16 loopms2;

    if (loopTicCount2 > ucTickCount)
    {
        loopms2 = (0xffff - (loopTicCount2 - ucTickCount));
    }
    else
    {
        loopms2 = (ucTickCount - loopTicCount2);
    }
    loopTicCount2 = ucTickCount;

    //IT6615_MSG((" loop ms  = %u\r\n",loopms2));
    return  loopms2;
}

u16 CalTimer(u16 SetupCnt)
{
    if (SetupCnt > ucTickCount)
    {
        return (0xffff - (SetupCnt - ucTickCount));
    }
    else
    {
        return (ucTickCount - SetupCnt);
    }
}

u16 TimeOutCheck(u16 timer, u16 x)
{
    if (CalTimer(timer) >= x)
    {
        return TRUE;
    }
    return FALSE;
}


u1 IsTimeOut(u16 x)
{
    if (CalTimer(prevTickCount) >= x)
    {
        prevTickCount = ucTickCount;
        return TRUE;
    }
    return FALSE;
}

u16 GetCurrentVirtualTime()
{
    return ucTickCount;
}


void It6615_delay1ms(u16 ms)
{
    u16 ucStartTickCount, diff;
    ucStartTickCount = ucTickCount;
    do
    {
        if (ucTickCount < ucStartTickCount)
        {
            diff = 0xffff - (ucStartTickCount - ucTickCount);
        }
        else
        {
            diff = ucTickCount - ucStartTickCount;
        }
    } while (diff < ms);
}

#elif 0//defined( WIN32)

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\\src\\usbi2c.h"

void
It6615_delay1ms(unsigned short ms)
{
    LARGE_INTEGER Freq ;
    LARGE_INTEGER Counter ;
    LARGE_INTEGER count,limit ;

    QueryPerformanceFrequency(&Freq) ;
    count.QuadPart = (ULONGLONG)ms * Freq.QuadPart / (ULONGLONG)1000 ;
    QueryPerformanceCounter(&Counter) ;
    limit.QuadPart = Counter.QuadPart + count.QuadPart ;

    while(limit.QuadPart > Counter.QuadPart)
    {
        QueryPerformanceCounter(&Counter) ;
    }

    return ;
}

u8 i2c_write_byte(u8 address, u8 offset, u8 byteno, u8* wrdata, u8 DEV)
{
    return i2c_writeN( address, offset, byteno, wrdata );
}
u8 i2c_read_byte(u8 address, u8 offset, u8 byteno, u8* rddata, u8 DEV)
{
    // need to check u8 or u32 , or it will be brust read fail
    u8 i = 0 , j[128], k ;
    k = i2c_read( address, offset, byteno,  j );

    if(k)
    {
        for(i = 0 ; i < byteno; i++)
        {
             rddata[i] = j[i];
        }
    }
    return k;
}
#endif
void It6615_delay1ms(u16 ms)
{
    delay_ms(ms);
}

