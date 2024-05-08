/*********************************************************************
 * Copyright (c) 2023-2024  PTN Electronics Limited. All rights reserved.
 * System  :
 * FileName: drv_ADN4604.c
 * Description:
 * Version :
 * Writer  : wf8421
 * Date    : 2023-08-11 19:40
 * History :
 * Date          Version          Rewriter          Description
**********************************************************************/

#include "includes.h"

#if (_EN_CHIP_ADN4604_USE == _MACRO_ENABLE)
StartupADN4604Init pADN4604InitFunc = NULL;
AppADN4604Process  pADN4604Process = NULL;
extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];
uint8_t write_4604(uint8_t u8DevID, uint8_t u8RegAdd, uint8_t u8Value)
{
    uint8_t i,ret;
    pstDrvADN4604VarConfig pstAdn4604Config = (pstDrvADN4604VarConfig)&stProjectAdn4604Config[u8DevID];
    for(i=0;i<5;i++)
    {
        if(pstAdn4604Config->u8DevAddr == 0)
            break;
        ret=i2c_write_bytes(pstAdn4604Config->u8I2cDevID, pstAdn4604Config->u8DevAddr, u8RegAdd, &u8Value, 1, FALSE);
        if(ret)
            break;
        delay_us(1000);
    }
    return  ret;
}
uint8_t read_4604(uint8_t u8DevID, uint8_t u8RegAdd)
{
    uint8_t i,ret,l_u8Value;
    pstDrvADN4604VarConfig pstAdn4604Config = (pstDrvADN4604VarConfig)&stProjectAdn4604Config[u8DevID];
    for(i=0;i<5;i++)
    {
        if(pstAdn4604Config->u8DevAddr == 0)
            break;
        ret=i2c_read_bytes(pstAdn4604Config->u8I2cDevID, pstAdn4604Config->u8DevAddr, u8RegAdd, &l_u8Value, 1, FALSE);
        if(ret)
            break;
        delay_us(1000);
    }
    return l_u8Value;
}

void InitAdn4604(void)
{
    u8 i;

    if(pADN4604InitFunc != NULL)
    {
        pADN4604InitFunc();
    }
    else
    {
        for(i=0;i<DEV_NUM_ADN4604;i++)
        {
            write_4604(i,0x00,0x01);      // Reset ADN4604 register
            delay_us(3000);
            write_4604(i,0x81,ADN4604_MAP_SELECT);      // First register mapping : 0,high->low  1,low->high
            delay_us(3000);
            write_4604(i,0xF0,0x00);      // Enable in and out port resistance
            delay_us(3000);
            write_4604(i,0x12,0x00);      // signal not invert
            delay_us(3000);
            write_4604(i,0x13,0x00);      // signal not invert
            delay_us(3000);
            {
                u8 j;
                for(j=0x30;j<=0x4E;j=j+2)
                {
                    write_4604(i,j,0xFF);       //Tx Advandced Drive 0
                    delay_us(2000);
                    write_4604(i,j+1,0xFF);   //Tx Advandced Drive 1
                    delay_us(2000);
                }
            }
//            dbg_printf("read_4604 [%d][0xff]=[%d]\r\n",i,read_4604(i,0xFF));
//            dbg_printf("read_4604 [%d][0xfe]=[%d]\r\n",i,read_4604(i,0xFE));
        }
    }
}

void mapp_ADN604Process(void)
{
    if(pADN4604Process != NULL)
    {
        pADN4604Process();
    }
}
#endif//#if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)











