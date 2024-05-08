/*********************************************************************
 * Copyright (c) 2023-2024  PTN Electronics Limited. All rights reserved.
 * System  :
 * FileName: drv_LT8644.c
 * Description:
 * Version :
 * Writer  : wf8421
 * Date    : 2023-08-11 19:40
 * History :
 * Date          Version          Rewriter          Description
**********************************************************************/

#include "includes.h"

#if (_EN_CHIP_LT8644_USE == _MACRO_ENABLE)
#define DBG_LT8644      dbg_printf

StartupLT8644Init pLT8644InitFunc = NULL;
AppLT8644Process  pLT8644Process = NULL;
extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];

uint8_t write_LT8644(uint8_t u8DevID, uint8_t u8RegAdd, uint8_t u8Value)
{
    uint8_t i,ret;
    pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[u8DevID];
    for(i=0;i<5;i++)
    {
        if(pstLT8644Config->u8DevAddr == 0)
            break;
        ret=i2c_write_bytes(pstLT8644Config->u8I2cDevID, pstLT8644Config->u8DevAddr, u8RegAdd, &u8Value, 1, FALSE);
        if(ret)
            break;
//        delay_us(1000);
    }
    return  ret;
}
uint8_t read_LT8644(uint8_t u8DevID, uint8_t u8RegAdd)
{
    uint8_t i,ret,l_u8Value;
    pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[u8DevID];
    for(i=0;i<5;i++)
    {
        if(pstLT8644Config->u8DevAddr == 0)
            break;
        ret=i2c_read_bytes(pstLT8644Config->u8I2cDevID, pstLT8644Config->u8DevAddr, u8RegAdd, &l_u8Value, 1, FALSE);
        if(ret)
            break;
//        delay_us(1000);
    }
    return l_u8Value;
}

void InitLT8644(void)
{
    u8 i;

    LT8644PinReset();

    if(pLT8644InitFunc != NULL)
    {
        pLT8644InitFunc();
    }
    else
    {
        uint8_t j;
        uint16_t id_h,id_l,ID;
        for(i=0;i<DEV_NUM_LT8644;i++)
        {
            id_h=read_LT8644(i,0x00);
            id_l=read_LT8644(i,0x01);
            ID = (id_h<<8)|id_l;
            DBG_LT8644("ID[%d %x %x %x]\r\n",i,id_h,id_l,ID);

            if(ID!=0x8644)
            {
                DBG_LT8644("LT8644[%d]iic fail\r\n",i);
            }

            for(j=0x02;j<=0x11;j++)
            {
                /*EQ Level   0    1    2    3    4    5    6    7    8    9    10   11   12   13
                  RQ Value  0xFF 0x01 0xFE 0x02 0xFC 0x9C 0x1C 0x0C 0x14 0x84 0x04 0xF8 0x78 0xF0
                  EQ Level   14   15   16   17   18   19   20   21   22   23   24   25   26   27
                  RQ Value  0x70 0xB0 0xD0 0xE0 0x30 0x90 0x60 0xA0 0xC0 0x08 0x10 0x20 0x40 0x80
                  */
                write_LT8644(i,j,0xF8);//…Ë÷√ Rx µƒ EQ µ»º∂Œ™ 10
            }
            for(j=0x5F;j<=0x6E;j++)
            {
                write_LT8644(i,j,0x1F);//…Ë÷√ Tx µƒ Swing ÷µŒ™ 500mv µÁ—π
            }
            write_LT8644(i,0x73,0x01);
            DBG_LT8644("LT8644[%d] Init\r\n",i);
        }
    }
}

void LT8644UpdateReg(uint8_t u8DevID)
{
    //Reg
    write_LT8644(u8DevID,0x73,0x05);//œ»≈‰÷√ºƒ¥Ê∆˜
    write_LT8644(u8DevID,0x73,0x01);//œ»≈‰÷√ºƒ¥Ê∆˜
    write_LT8644(u8DevID,0x73,0x05);//œ»≈‰÷√ºƒ¥Ê∆˜
    DBG_LT8644("LT8644UpdataReg[%d]\r\n",u8DevID);
}


void LT8644UpdatePin(void)
{
    // update pin
    u8 i;
    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstUpdatePin != NULL)
            write_LT8644(i,0x73,0x07);//œ»≈‰÷√ºƒ¥Ê∆˜
    }

    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstUpdatePin != NULL)
        {
            if(pstLT8644Config->pstUpdatePin->eInitLevel == ePinLevel_High)
            {
                GPIO_SetBits(STM32_GPIO_PORT[pstLT8644Config->pstUpdatePin->u8PortType-PORT_GPIOA], pstLT8644Config->pstUpdatePin->u16GpioPin);
            }
            else
            {
                GPIO_ResetBits(STM32_GPIO_PORT[pstLT8644Config->pstUpdatePin->u8PortType-PORT_GPIOA], pstLT8644Config->pstUpdatePin->u16GpioPin);
            }
        }
    }
    delay_us(5000);
    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstUpdatePin != NULL)
        {
            if(pstLT8644Config->pstUpdatePin->eInitLevel == ePinLevel_High)
            {
                GPIO_ResetBits(STM32_GPIO_PORT[pstLT8644Config->pstUpdatePin->u8PortType-PORT_GPIOA], pstLT8644Config->pstUpdatePin->u16GpioPin);
            }
            else
            {
                GPIO_SetBits(STM32_GPIO_PORT[pstLT8644Config->pstUpdatePin->u8PortType-PORT_GPIOA], pstLT8644Config->pstUpdatePin->u16GpioPin);
            }
        }
    }
    delay_us(10000);
    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstUpdatePin != NULL)
        {
            if(pstLT8644Config->pstUpdatePin->eInitLevel == ePinLevel_High)
            {
                GPIO_SetBits(STM32_GPIO_PORT[pstLT8644Config->pstUpdatePin->u8PortType-PORT_GPIOA], pstLT8644Config->pstUpdatePin->u16GpioPin);
            }
            else
            {
                GPIO_ResetBits(STM32_GPIO_PORT[pstLT8644Config->pstUpdatePin->u8PortType-PORT_GPIOA], pstLT8644Config->pstUpdatePin->u16GpioPin);
            }
        }
    }
    delay_us(5000);

    DBG_LT8644("LT8644UpdataPin\r\n");
}

void LT8644PinReset(void)
{
    u8 i;

    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstResetPin != NULL)
        {
            if(pstLT8644Config->eRstLevel == ePinLevel_High)
            {
                GPIO_ResetBits(STM32_GPIO_PORT[pstLT8644Config->pstResetPin->u8PortType-PORT_GPIOA], pstLT8644Config->pstResetPin->u16GpioPin);
            }
            else
            {
                GPIO_SetBits(STM32_GPIO_PORT[pstLT8644Config->pstResetPin->u8PortType-PORT_GPIOA], pstLT8644Config->pstResetPin->u16GpioPin);
            }
        }
    }
    delay_us(10000);
    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstResetPin != NULL)
        {
            if(pstLT8644Config->eRstLevel == ePinLevel_High)
            {
                GPIO_SetBits(STM32_GPIO_PORT[pstLT8644Config->pstResetPin->u8PortType-PORT_GPIOA], pstLT8644Config->pstResetPin->u16GpioPin);
            }
            else
            {
                GPIO_ResetBits(STM32_GPIO_PORT[pstLT8644Config->pstResetPin->u8PortType-PORT_GPIOA], pstLT8644Config->pstResetPin->u16GpioPin);
            }
        }
    }
    delay_us(50000);
    for(i=0;i<DEV_NUM_LT8644;i++)
    {
        pstDrvLT8644VarConfig pstLT8644Config = (pstDrvLT8644VarConfig)&stProjectLT8644Config[i];
        if(pstLT8644Config->pstResetPin != NULL)
        {
            if(pstLT8644Config->eRstLevel == ePinLevel_High)
            {
                GPIO_ResetBits(STM32_GPIO_PORT[pstLT8644Config->pstResetPin->u8PortType-PORT_GPIOA], pstLT8644Config->pstResetPin->u16GpioPin);
            }
            else
            {
                GPIO_SetBits(STM32_GPIO_PORT[pstLT8644Config->pstResetPin->u8PortType-PORT_GPIOA], pstLT8644Config->pstResetPin->u16GpioPin);
            }
        }
    }
    delay_us(30000);

    DBG_LT8644("LT8644Reset\r\n");
}

void mapp_LT8644Process(void)
{
    if(pLT8644Process != NULL)
    {
        pLT8644Process();
    }
}
#endif











