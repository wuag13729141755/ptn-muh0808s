///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.c>
//   @author Max.Kao@ite.com.tw
//   @date   2014/09/11
//   @fileversion: ITE_MHLRX_SAMPLE_V1.01
//******************************************/
//#include "config.h"
//#include "MCU.h"
//#include <stdio.h>
//#include "IIC_Driver.h"
#if 0
BYTE IIC_IT66021=0;

BYTE i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{

	unsigned char i;

	I2C_Start(IIC_IT66021);

	I2C_SendByte(address, IIC_IT66021);
	if(!I2C_Ackword_E)
	{
		return 0;
	}

	I2C_SendByte(offset, IIC_IT66021);
	if(!I2C_Ackword_E)
	{
		return 0;
	}

	for(i = 0; i < byteno; i++)
	{
		I2C_SendByte(p_data[i], IIC_IT66021);
		if(!I2C_Ackword_E)
		{
			return 0;
		}
	}

	I2C_Stop(IIC_IT66021);
	//delay_nms(1);

	return 1;

}

BYTE i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device )
{

	unsigned char i = 0;

	I2C_Start(IIC_IT66021);

	I2C_SendByte(address, IIC_IT66021);
	if(!I2C_Ackword_E)
	{
		return 0;
	}

	I2C_SendByte(offset, IIC_IT66021);
	if(!I2C_Ackword_E)
	{
		return 0;
	}

	I2C_Start(IIC_IT66021);

	I2C_SendByte(address+1, IIC_IT66021);
	if(!I2C_Ackword_E)
	{
		return 0;
	}

	for(i = 0; i < byteno-1; i++)
	{
		p_data[i] = I2C_ReceiveByte(IIC_IT66021);
		I2C_Ackn(0, IIC_IT66021); //·µ»Øack
	}

	p_data[i] = I2C_ReceiveByte(IIC_IT66021);
	I2C_Ackn(1, IIC_IT66021); //²»·µ»Øack

	I2C_Stop(IIC_IT66021);

	return 1;


}
#endif

