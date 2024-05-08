/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName:         drv_LM74HC595.c
**Writer:           wf8421
**Create Date:  2018-12-3
**Rewriter:         wf8421
**Rewrite Date: 2018-12-3
**Description:  595驱动及数据功能处理
**History:
**Version:          V1.0.0
******************************************************/
#define __DRV_LM74HC595_C__
#include "includes.h"
#if (_ENABLE_CHIP_LM74HC595 == _MACRO_ENABLE)
static u32 sg_u32Lm74hc595Value;
extern SemaphoreHandle_t xSemaphore_SPI[NUM_OF_SPI_BUS_MAX];

void mapp_Set595Value(u32 u32Value)
{
    sg_u32Lm74hc595Value = u32Value;
}

u32 mapp_Get595Value(void)
{
    return sg_u32Lm74hc595Value;
}


void mdrv_595Init(void)
{
    sg_u32Lm74hc595Value = 0xFFFFFFFF;
    mdrv_595SendData(_SPI_TABLE_INDEX_74HC595,sg_u32Lm74hc595Value);
}

void mdrv_595SendData(u8 u8Id,u32 u32data)
{
    u8 l_u8RealData[4];
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_SPI[_SPI_TABLE_INDEX_74HC595], portMAX_DELAY);
#endif

    l_u8RealData[0] = (u32data>>24)&0xFF;
    l_u8RealData[1] = (u32data>>16)&0xFF;
    l_u8RealData[2] = (u32data>>8)&0xFF;
    l_u8RealData[3] = (u32data>>0)&0xFF;
    drv_595_bus_direct_write(u8Id, 4, l_u8RealData);
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_SPI[_SPI_TABLE_INDEX_74HC595]);
#endif
}

void mdrv_595SendByte(u8 u8Id,u8 u8data)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_SPI[_SPI_TABLE_INDEX_74HC595], portMAX_DELAY);
#endif
    drv_595_bus_direct_write(u8Id, 1, &u8data);
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_SPI[_SPI_TABLE_INDEX_74HC595]);
#endif
}

void mdrv_595Send2Byte(u8 u8Id,u16 u16data)
{
    u8 l_u8RealData[2];
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_SPI[_SPI_TABLE_INDEX_74HC595], portMAX_DELAY);
#endif
    l_u8RealData[0] = (u16data>>8)&0xFF;
    l_u8RealData[1] = (u16data>>0)&0xFF;
    drv_595_bus_direct_write(u8Id, 2, l_u8RealData);
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_SPI[_SPI_TABLE_INDEX_74HC595]);
#endif
}

bool mapp_Change595ValueProcess(void)
{
    static u32 sl_u32PreLm74hc595Value = 0;
    bool l_bRtState = FALSE;

    if(sl_u32PreLm74hc595Value!= mapp_Get595Value())
    {
        sl_u32PreLm74hc595Value = mapp_Get595Value();
        mdrv_595SendData(_SPI_TABLE_INDEX_74HC595,sl_u32PreLm74hc595Value);

        l_bRtState = TRUE;
    }

    return l_bRtState;
}

void drv595_delay(uint16_t delayTime)
{
    while(delayTime)
    {
        delayTime--;
    }
}

void drv595_pin_set(pstGpioConfig_t pGpio, ePinLevel_t e_level)
{
    if(e_level == ePinLevel_High)
    {
//    #ifdef STM32F2XX_HD
//        STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BSRRL = pGpio->u16GpioPin;
//    #else
//        STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BSRR = pGpio->u16GpioPin;
//    #endif
        GPIO_SetBits(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);
    }
    else
    {
//    #ifdef STM32F2XX_HD
//        STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BSRRH = pGpio->u16GpioPin;
//    #else
//        STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BRR = pGpio->u16GpioPin;
//    #endif
        GPIO_ResetBits(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);
    }
}

ePinLevel_t drv595_pin_get(pstGpioConfig_t pGpio)
{
//    return ((STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->IDR & pGpio->u16GpioPin) ==\
//        pGpio->u16GpioPin)?ePinLevel_High: ePinLevel_Low;
    return (GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin) == \
        pGpio->u16GpioPin)?ePinLevel_High: ePinLevel_Low;

}

uint8_t SwitchMSBToLSB(uint8_t u8val)
{
    uint8_t i;
    uint8_t l_u8temp=0;
    for(i=0;i<8;i++)
    {
        l_u8temp <<=1;
        if(u8val&0x01)
            l_u8temp |= 0x01;
        else
            l_u8temp &= (~0x01);
        u8val >>=1;
    }
    return l_u8temp;
}

eSpiResult_t drv_595_bus_direct_write(uint8_t u8Dev,
                                                uint16_t u16Count,
                                                uint8_t* pu8Buffer)
{
    uint16_t i;
    uint8_t j=0;
    uint8_t send_byte;
    pstSpiDeclare_t l_pDrv595 = (pstSpiDeclare_t)&stProjectSpiTable[u8Dev];

    drv595_pin_set(l_pDrv595->pstSpiCs, ePinLevel_Low);
    drv595_pin_set(l_pDrv595->pstSpiMosi, ePinLevel_Low);
    drv595_pin_set(l_pDrv595->pstSpiClk, ePinLevel_Low);

    for(i=0; i<u16Count; i++)
    {
        //spi_send_byte(u8Dev, *pu8Buffer++);
        send_byte = *pu8Buffer;
        send_byte = SwitchMSBToLSB(send_byte);
        for(j=0; j<8; j++)
        {
            if(send_byte&0x01)
            {
                drv595_pin_set(l_pDrv595->pstSpiMosi, ePinLevel_High);
            }
            else
            {
                drv595_pin_set(l_pDrv595->pstSpiMosi, ePinLevel_Low);
            }
            drv595_pin_set(l_pDrv595->pstSpiClk, ePinLevel_High);
            drv595_delay(l_pDrv595->u16SpiDelayClk);
            drv595_pin_set(l_pDrv595->pstSpiClk, ePinLevel_Low);
            drv595_delay(l_pDrv595->u16SpiDelayClk);
            send_byte >>= 1;
        }
        *pu8Buffer++;
    }

    drv595_pin_set(l_pDrv595->pstSpiCs, ePinLevel_High);
    drv595_delay(l_pDrv595->u16SpiDelayClk);
    drv595_pin_set(l_pDrv595->pstSpiCs, ePinLevel_Low);

    return eSpiSuccess;
}

#endif  // #if (_ENABLE_CHIP_LM74HC595 == _MACRO_ENABLE)

