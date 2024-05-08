/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         drv_LM74LS138.c
**Writer:           wf8421
**Create Date:  2019-11-11
**Rewriter:         wf8421
**Rewrite Date: 2019-11-11
**Description:  138驱动及数据功能处理
**History:
**Version:          V1.0.0
******************************************************/
#include "includes.h"

#if _ENABLE_CHIP_LM74HC138

void drv138SetPin(pstGpioConfig_t pGpio, ePinLevel_t e_level)
{
    if(e_level == ePinLevel_High)
    {
        if(IS_MCU_GPIO(pGpio->u8PortType))
        {
            GPIO_WriteBit(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],
                        pGpio->u16GpioPin,
                        Bit_SET);
        }
    }
    else
    {
        if(IS_MCU_GPIO(pGpio->u8PortType))
        {
            GPIO_WriteBit(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],
                        pGpio->u16GpioPin,
                        Bit_RESET);
        }
    }
}

void drvSend138Data(u8 u8Dev,u8 u8Dat)
{
    pstDrvLM74LS138Ctrl_t l_pDrv138 = (pstDrvLM74LS138Ctrl_t)&stProject74LS138ConfigTable[u8Dev];

    if(l_pDrv138->pstCtrlPortA != NULL)
    {
        switch(u8Dat)
        {
            case BIT0:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_Low);
                }
                break;
            case BIT1:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_Low);
                }
                break;
            case BIT2:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_Low);
                }
                break;
            case BIT3:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_Low);
                }
                break;
            case BIT4:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_High);
                }
                break;
            case BIT5:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_High);
                }
                break;
            case BIT6:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_Low);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_High);
                }
                break;
            case BIT7:
                {
                    drv138SetPin(l_pDrv138->pstCtrlPortA,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortB,ePinLevel_High);
                    drv138SetPin(l_pDrv138->pstCtrlPortC,ePinLevel_High);
                }
                break;
        }
    }
}

void mapp_Set138Value(u16 u16Data)
{
    drvSend138Data(_CHIP_TABLE_INDEX_74HC138,(u8)u16Data);
}
#endif

