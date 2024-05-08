///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.h>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/

#ifndef _IT6602_IO_h_
#define _IT6602_IO_h_
#include "it6602_typedef.h"


#ifdef SUPPORT_UART_CMD
void UartCommand();
#endif


//BYTE HDMI_IIC_Read( BYTE RegAddr);
//BOOL HDMI_IIC_Write( BYTE RegAddr,BYTE DataIn);
//BYTE HDMI_IIC_SET( BYTE offset, BYTE mask, BYTE datain );


//BOOL i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );
//BOOL i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );

#ifdef Enable_IT6602_CEC
#include "it6602_debug.h"
BYTE IT6602_CEC_ReadI2C_Byte(BYTE RegAddr);
unsigned char IT6602_CEC_WriteI2C_Byte(BYTE offset,BYTE buffer );
#endif


//#define hdmirxrd(x) HDMI_IIC_Read(x)
//#define hdmirxwr(x,y) HDMI_IIC_Write(x,y)



void SetEDIDWp(BOOL ENA);


void HotPlug(BYTE Enable);

void init_printf(void);

void SetintActive(BOOL bactive);


#endif
