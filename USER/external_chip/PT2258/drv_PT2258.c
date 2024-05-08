/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:     drv_PT2258.c
**Writer:       wf8421
**Create Date:  2020-09-07
**Rewriter:     wf8421
**Rewrite Date: 2020-09-07
**Description:  PT2258Çý¶¯
**History:
**Version:      V1.0.0
******************************************************/

#if _ENABLE_CHIP_PT2258
///PT2258
#define PT2258_ADDR                     0x80
//#define _I2C_TABLE_INDEX_PT2258_1                    1

#define PT2258_Reg_6CH_1db_STEP         0xE0        // 1110 xxxx  6-Channel, -1dB/step
#define PT2258_Reg_6CH_10db_STEP        0xD0        // 1101 0xxx  6-Channel, -10dB/step

#define PT2258_Reg_CH1_1db_STEP         0x90        // 1001 xxxx  Channel No. 1, -1dB/step
#define PT2258_Reg_CH1_10db_STEP        0x80        // 1000 0xxx  Channel No. 1, -10dB/step

#define PT2258_Reg_CH2_1db_STEP         0x50        // 0101 xxxx  Channel No. 2, -1dB/step
#define PT2258_Reg_CH2_10db_STEP        0x40        // 0000 0xxx  Channel No. 2, -10dB/step

#define PT2258_Reg_CH3_1db_STEP         0x10        // 0001 xxxx  Channel No. 3, -1dB/step
#define PT2258_Reg_CH3_10db_STEP        0x00        // 0000 0xxx  Channel No. 3, -10dB/step

#define PT2258_Reg_CH4_1db_STEP         0x30        // 0011 xxxx  Channel No. 4, -1dB/step
#define PT2258_Reg_CH4_10db_STEP        0x20        // 0010 0xxx  Channel No. 4, -10dB/step

#define PT2258_Reg_CH5_1db_STEP         0x70        // 0111 xxxx  Channel No. 5, -1dB/step
#define PT2258_Reg_CH5_10db_STEP        0x60        // 0110 0xxx  Channel No. 5, -10dB/step

#define PT2258_Reg_CH6_1db_STEP         0xB0        // 1011 xxxx  Channel No. 6, -1dB/step
#define PT2258_Reg_CH6_10db_STEP        0xA0        // 1010 0xxx  Channel No. 6, -10dB/step

#define PT2258_Reg_6CH_MUTE             0xF8        // 1111 100x  6-Channel, Mute When M= 1, MUTE ON
                                                    //                                                           When M= 0, MUTE OFF
#define PT2258_Reg_Clear                0xC0        // 1100 0000  Clear Register

#if 0//(MAX_VOLUME==100)
u8 VlumeValue_PT2258[61]=
{
/*     0   1   2   3   4   5   6   7   8   9*/
/*0*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*1*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*2*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*3*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*4*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*5*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*6*/ 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,
/*7*/
/*8*/
/*9*/
/*10*/
};
#else
u8 VlumeValue_PT2258[61]=
{
/*     0   1   2   3   4   5   6   7   8   9*/
/*0*/ 79, 59, 58, 57, 56, 55, 54, 53, 52, 51,
/*1*/ 50, 49, 48, 47, 46, 45, 44, 43, 42, 41,
/*2*/ 40, 39, 38, 37, 36, 35, 34, 33, 32, 31,
/*3*/ 30, 29, 28, 27, 26, 25, 24, 23, 22, 21,
/*4*/ 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,
/*5*/ 10,  9,  8,  7,  6,  5,  4,  3,  2,  1,
/*6*/  0,
};
#endif //add by wf8421

extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];
bool PT2258_i2c_read_byte( u8 address, u8 u8RegAddr, u8 byteno, u8 *p_data, u8 device )
{
	bool ret_val;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[device], portMAX_DELAY);
#endif
	ret_val = i2c_read_bytes(device, address,(uint16_t)u8RegAddr, p_data, byteno, FALSE);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_IIC[device]);
#endif

	return (ret_val)?FALSE:TRUE;
}

bool PT2258_i2c_write_byte( u8 address, u8 u8RegAddr, u8 byteno, u8 *p_data, u8 device )
{
	bool ret_val;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[device], portMAX_DELAY);
#endif
	ret_val = i2c_write_bytes(device, address,(uint16_t)u8RegAddr, p_data, byteno, FALSE);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_IIC[device]);
#endif

	return (ret_val)?FALSE:TRUE;
}

void PT2258_Mute_6CH_Function(bool bMute)
{
    if(bMute)
    {
        PT2258_i2c_write_byte(PT2258_ADDR, PT2258_Reg_6CH_MUTE|BIT0, 0, NULL, _I2C_TABLE_INDEX_PT2258_1);
    }
    else
    {
        PT2258_i2c_write_byte(PT2258_ADDR, PT2258_Reg_6CH_MUTE&(~BIT0), 0, NULL, _I2C_TABLE_INDEX_PT2258_1);
    }
}

void PT2258_AdjustFunction(u8 uCh, u8 uValue)
{
    u8 VolumeVaule_10db = 0;
    u8 VolumeVaule_1db = 0;
    bool flag = false;

    if(uCh == 1)
    {
        VolumeVaule_1db = PT2258_Reg_CH1_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_CH1_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
    }
    else if(uCh == 2)
    {
        VolumeVaule_1db = PT2258_Reg_CH2_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_CH2_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
    }
    else if(uCh == 3)
    {
        VolumeVaule_1db = PT2258_Reg_CH3_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_CH3_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
        //Uart2_printf("flag3 = %d\n",flag);
    }
    else if(uCh == 4)
    {
        VolumeVaule_1db = PT2258_Reg_CH4_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_CH4_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
        //Uart2_printf("flag4 = %d\n",flag);
    }
    else if(uCh == 5)
    {
        VolumeVaule_1db = PT2258_Reg_CH5_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_CH5_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
    }
    else if(uCh == 6)
    {
        VolumeVaule_1db = PT2258_Reg_CH6_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_CH6_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
    }
    else if(uCh == 0)
    {
        VolumeVaule_1db = PT2258_Reg_6CH_1db_STEP|((VlumeValue_PT2258[uValue]%10)&0xF);
        VolumeVaule_10db = PT2258_Reg_6CH_10db_STEP|((VlumeValue_PT2258[uValue]/10)&0x7);
        flag=PT2258_i2c_write_byte(PT2258_ADDR,VolumeVaule_10db, 1, &VolumeVaule_1db,_I2C_TABLE_INDEX_PT2258_1);
    }
    else
    {
        // error channal
    }
}

void PT2258_VolumeAdjust(u8 uAudioCh,u8 uVolume)
{
    if(uAudioCh == PT2258Ch_Audio1)
    {
        PT2258_AdjustFunction(1,uVolume);
        PT2258_AdjustFunction(2,uVolume);
    }
    else if(uAudioCh == PT2258Ch_Audio2)
    {
        PT2258_AdjustFunction(3,uVolume);
        PT2258_AdjustFunction(4,uVolume);
    }
    else if(uAudioCh == PT2258Ch_Audio3)
    {
        PT2258_AdjustFunction(5,uVolume);
        PT2258_AdjustFunction(6,uVolume);
    }
    else if(uAudioCh == PT2258Ch_All)
    {
        PT2258_AdjustFunction(0,uVolume);
    }
}

void PT2258_VolumeMute(u8 uAudioCh,bool tbMute,u8 u8Vloume)
{
    if(tbMute)
    {
        if(uAudioCh==PT2258Ch_All)
        {
            PT2258_VolumeAdjust(uAudioCh,0);
            PT2258_Mute_6CH_Function(TRUE);
        }
        else
        {
            PT2258_VolumeAdjust(uAudioCh,0);
        }
    }
    else
    {
        if(uAudioCh == PT2258Ch_Audio1)
        {
            PT2258_VolumeAdjust(PT2258Ch_Audio1,u8Vloume);
        }
        else if(uAudioCh == PT2258Ch_Audio2)
        {
            PT2258_VolumeAdjust(PT2258Ch_Audio2,u8Vloume);
        }
        else if(uAudioCh == PT2258Ch_Audio3)
        {
            PT2258_VolumeAdjust(PT2258Ch_Audio3,u8Vloume);
        }
        else if(uAudioCh == PT2258Ch_All)
        {
            PT2258_VolumeAdjust(PT2258Ch_All,u8Vloume);
            PT2258_Mute_6CH_Function(FALSE);
        }
    }
}

void Init_PT2258(void)
{
    PT2258_i2c_write_byte(PT2258_ADDR, PT2258_Reg_Clear, 1, NULL, _I2C_TABLE_INDEX_PT2258_1);
}

#endif //add by wf8421

