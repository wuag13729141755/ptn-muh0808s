///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <mcu.h>
//   @author Max.Kao@ite.com.tw
//   @date   2011/12/01
//   @fileversion: IT6623_Sample_1.04
//******************************************/

#ifndef _IT6602_MCU_H_
#define _IT6602_MCU_H_
#include <stdio.h>
#include <string.h>

#define 	_MCU_
//#include "stm32f4xx.h"

//#define 	BYTE    u8

#define  Uart_Debug    0

#define ADDR_24C32          0xA0


#define YES                      1
#define NO                       0




#define RS232_SELECT(x)       //(x == 1) ? (GPIOC->BSRRL = GPIO_Pin_6):(GPIOC->BSRRH = GPIO_Pin_6)

#define IT66021_N1_RESET(x)		//(x == 1) ? (GPIOD->BSRRL = GPIO_Pin_0):(GPIOD->BSRRH = GPIO_Pin_0)
#define IT66021_N2_RESET(x)		//(x == 1) ? (GPIOD->BSRRL = GPIO_Pin_1):(GPIOD->BSRRH = GPIO_Pin_1)

extern void wait_fpga_init_done(void);
extern void video_led_set_status(unsigned char on_off, unsigned char id);
extern signed char video_led_get_status(unsigned char id);

extern uint8_t input_link_det(uint8_t in);

#endif
