///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.c>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/

#include "includes.h"



extern  unsigned  char i2c_IT66121_read_one_bytes(unsigned char dev_addr, unsigned char reg_addr, unsigned int size, unsigned char *pbuf);
extern  unsigned  char i2c_IT66121_write_one_bytes(unsigned char dev_addr, unsigned char reg_addr, unsigned int size, unsigned char *pbuf);



BYTE HDMITX_ReadI2C_Byte(unsigned char RegAddr)
{
    BYTE  p_data;

    i2c_read_byte(g_u8It66121DevAddr, RegAddr, 1, &p_data, g_u8It66121BusID);
    return p_data;
}


SYS_STATUS HDMITX_WriteI2C_Byte(unsigned char RegAddr,unsigned char d)
{
    BYTE  flag;

    flag = i2c_write_byte(g_u8It66121DevAddr, RegAddr, 1, &d, g_u8It66121BusID);
    return flag?ER_SUCCESS:ER_FAIL;
}

SYS_STATUS HDMITX_ReadI2C_ByteN(unsigned char RegAddr,unsigned char *pData,unsigned int N)
{
    BYTE flag;

    flag=i2c_read_byte(g_u8It66121DevAddr, RegAddr, N, pData, g_u8It66121BusID);
	return flag?ER_SUCCESS:ER_FAIL;
}


SYS_STATUS HDMITX_SetI2C_Byte(unsigned char Reg,unsigned char Mask,unsigned char Value)
{
    BYTE Temp;
    BYTE flag;
    if( Mask != 0xFF )
    {
        Temp=HDMITX_ReadI2C_Byte(Reg);
        Temp&=(~Mask);
        Temp|=Value&Mask;
    }
    else
    {
        Temp=Value;
    }

    flag=HDMITX_WriteI2C_Byte(Reg,Temp);

    return flag?ER_SUCCESS:ER_FAIL;

}

void HDMITX_SetI2C_Byte1(unsigned char Reg,unsigned char Mask,unsigned char Value)
{
	HDMITX_SetI2C_Byte(Reg,Mask,Value);
}

//cec 没时间搞，暂时不移植。
BYTE CEC_ReadI2C_Byte(BYTE RegAddr)
{
    BYTE  p_data;

    //i2c_read_byte(TX0CECADR,RegAddr,1,&p_data,TX0DEV);
    return p_data;
}

SYS_STATUS CEC_WriteI2C_Byte(BYTE RegAddr,BYTE d)
{
    BOOL  flag = TRUE;

    //flag=i2c_write_byte(TX0CECADR,RegAddr,1,&d,TX0DEV);
    return flag?ER_SUCCESS:ER_FAIL;
}


SYS_STATUS CEC_SetI2C_Byte(BYTE Reg,BYTE Mask,BYTE Value)
{
    BYTE Temp;
    Temp=CEC_ReadI2C_Byte(Reg);
    Temp&=(~Mask);
    Temp|=Value&Mask;

    return CEC_WriteI2C_Byte(Reg,Temp);
}

