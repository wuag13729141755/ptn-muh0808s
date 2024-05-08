///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/

#ifndef _IO_h_
#define _IO_h_

#include "IT66121/src/IT66121_typedef.h"


///////////////////////////////////////////////////////////////////////////////
// Start: I2C for 8051
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// I2C for original function call
///////////////////////////////////////////////////////////////////////////////



//BOOL i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );
//BOOL i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );


BYTE HDMITX_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS HDMITX_WriteI2C_Byte(BYTE RegAddr,BYTE d);
SYS_STATUS HDMITX_ReadI2C_ByteN(unsigned char RegAddr,unsigned char *pData,unsigned int N);
SYS_STATUS HDMITX_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS HDMITX_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value);


BYTE CEC_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS CEC_WriteI2C_Byte(BYTE RegAddr,BYTE d);
SYS_STATUS CEC_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS CEC_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N);
SYS_STATUS CEC_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value);



#endif
