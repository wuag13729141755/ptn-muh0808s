///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Utility.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/

#ifndef _ITE6615_Utility_h_

#define _ITE6615_Utility_h_
#ifdef _MCU_8051_
#include "iTE6615_mcu.h"

#define CLOCK                   27000000L//22118400L//11059200L//24000000L////27000000L//12000000L //15000000L
#define MachineCycle            6//12
#define BAUD_RATE               57600L    //57600L    //  115200L    //     19200L    //  57600L
#define BAUD_SETTING            (65536L - (CLOCK / (32L * BAUD_RATE)))


#define MICROSECONDS_TIMER(microseconds)    (((CLOCK /1000000)* (microseconds))/MachineCycle)
#define COUNTER1ms                          MICROSECONDS_TIMER(1000)             // 1000 us = 1 ms
#define COUNTER10ms                         MICROSECONDS_TIMER(10000)             // 10000 us = 10 ms
#define Tick100us                           MICROSECONDS_TIMER(100)                //Timer 0 100us(IR sampling time)
#define Tick1ms                             (65536 - COUNTER1ms)
#define Tick10ms                            (65536 - COUNTER10ms)
#define IR_COUNTER_VALUE                    (65536 - Tick100us)


void HoldSystem();
void DelayUS(u16 us);
void It6615_delay1ms(u16 ms);
void init_printf(void);
u16 getloopTicCount();
u16 getloopTicCount2();

u16 TimeOutCheck(u16 timer, u16 x);
u16 GetCurrentVirtualTime();

void initialTimer1(void);
#elif defined( WIN32)

#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "..\\src\\usbi2c.h"

void It6615_delay1ms(u16 ms);
u8 i2c_write_byte(u8 address, u8 offset, u8 byteno, u8* wrdata, u8 DEV);
u8 i2c_read_byte(u8 address, u8 offset, u8 byteno, u8* rddata, u8 DEV);


#endif
void It6615_delay1ms(u16 ms);

#endif
