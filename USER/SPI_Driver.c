#include "includes.h"

extern const stSpiDeclare_t stProjectSpiTable[];

void spi_delay(uint16_t delayTime)
{
    while(delayTime)
    {
        delayTime--;
    }
}

void spi_pin_set(pstGpioConfig_t pGpio, ePinLevel_t e_level)
{
    if(e_level == ePinLevel_High)
    {
        GPIO_SetBits(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);
    }
    else
    {
        GPIO_ResetBits(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);
    }
}

ePinLevel_t spi_pin_get(pstGpioConfig_t pGpio)
{
    #if (defined(GD32F4XX))
    return  (gpio_input_bit_get(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin)==SET)\
            ?ePinLevel_High: ePinLevel_Low;
    #else
    return (GPIO_ReadInputDataBit(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin) == \
        SET)?ePinLevel_High: ePinLevel_Low;
    #endif
}

#if (defined(GD32F4XX))
void spi_dio_type_set(pstGpioConfig_t pGpio, eSpiDioDirect_t eDioDirect)
{
    if(eDioDirect == eSpiDioDir_Output)
    {
        gpio_mode_set(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],
                    GPIO_Mode_OUT,
                    pGpio->eGpioPuPdType,
                    pGpio->u16GpioPin);
        gpio_output_options_set(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],
                        pGpio->eGpioOType,
                        GPIO_OSPEED_50MHZ,
                        pGpio->u16GpioPin);

    }
    else if(eDioDirect == eSpiDioDir_Input)
    {
        gpio_mode_set(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],
                    GPIO_Mode_IN,
                    pGpio->eGpioPuPdType,
                    pGpio->u16GpioPin);
    }
}

#else
void spi_dio_type_set(pstGpioConfig_t pGpio, eSpiDioDirect_t eDioDirect)
{
#ifdef N32G45X
    GPIO_InitType       GPIO_InitStructure;
#else
    GPIO_InitTypeDef    GPIO_InitStructure;
#endif

#ifdef N32G45X
    GPIO_InitStructure.Pin = pGpio->u16GpioPin;
#else
    GPIO_InitStructure.GPIO_Pin = pGpio->u16GpioPin;
#endif
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    if(eDioDirect == eSpiDioDir_Output)
    {
        #ifdef STM32F2XX_HD
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
        #else
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        #endif
    }
    else if(eDioDirect == eSpiDioDir_Input)
    {
        #ifdef STM32F2XX_HD
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
        #else
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        #endif
    }
#ifdef N32G45X
    GPIO_InitPeripheral(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
#else
    GPIO_Init(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
#endif
}
#endif

void spi_send_byte(uint8_t u8dev, uint8_t send_byte)
{
    uint8_t i = 0;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8dev];

    for(i=0; i<8; i++)
    {
        if(pSpi->u8SpiClkMode==1)
        {
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_Low);
            spi_delay(pSpi->u16SpiDelayClk);
            if(send_byte&0x80)
            {
                spi_pin_set(pSpi->pstSpiMosi, ePinLevel_High);
            }
            else
            {
                spi_pin_set(pSpi->pstSpiMosi, ePinLevel_Low);
            }
            send_byte<<=1;
            spi_delay(pSpi->u16SpiDelayClk);
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_High);
            spi_delay(pSpi->u16SpiDelayClk);
        }
        else
        {
            if(send_byte&0x01)
            {
                spi_pin_set(pSpi->pstSpiMosi, ePinLevel_High);
            }
            else
            {
                spi_pin_set(pSpi->pstSpiMosi, ePinLevel_Low);
            }
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_Low);
            spi_delay(pSpi->u16SpiDelayClk);
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_High);
            spi_delay(pSpi->u16SpiDelayClk);
            send_byte >>= 1;
        }
    }
}

uint8_t spi_get_byte(uint8_t u8dev)
{
    uint8_t i=0, temp=0x00;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8dev];

    for(i=0; i<8; i++)
    {
        if(pSpi->u8SpiClkMode==1)
        {
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_Low);
            spi_delay(pSpi->u16SpiDelayClk);
            temp<<=1;
            if(pSpi->pstSpiMiso == NULL)
            {
                if(spi_pin_get(pSpi->pstSpiMosi))//dio mode
                {
                    temp|=0x01;
                }
            }
            else
            {
                if(spi_pin_get(pSpi->pstSpiMiso))//normal mode
                {
                    temp|=0x01;
                }
            }
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_High);
            spi_delay(pSpi->u16SpiDelayClk);
        }
        else
        {
            temp >>= 1;
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_Low);
            spi_delay(pSpi->u16SpiDelayClk);
            spi_pin_set(pSpi->pstSpiClk, ePinLevel_High);
            spi_delay(pSpi->u16SpiDelayClk);
            if(pSpi->pstSpiMiso == NULL)
            {
                if(spi_pin_get(pSpi->pstSpiMosi))//dio mode
                {
                    temp |= 0x80;
                }
            }
            else
            {
                if(spi_pin_get(pSpi->pstSpiMiso))//normal mode
                {
                    temp |= 0x80;
                }
            }
        }
    }

    return temp;
}

eSpiResult_t drv_spi_bus_write(uint8_t u8dev,
                                uint32_t u32CmdAddress,
                                uint16_t u16Count,
                                uint8_t* pu8Buffer,
                                eSpiAdrWide_t wFlags)
{
    uint16_t i;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8dev];

    spi_pin_set(pSpi->pstSpiCs, ePinLevel_Low);
    //send command or address
    if(wFlags == eSpiU16Addr)
    {
        spi_send_byte(u8dev, ((u32CmdAddress>>8)&0xFF));
    }
    else if(wFlags == eSpiU32Addr)
    {
        spi_send_byte(u8dev, ((u32CmdAddress>>24)&0xFF));
        spi_send_byte(u8dev, ((u32CmdAddress>>16)&0xFF));
        spi_send_byte(u8dev, ((u32CmdAddress>>8)&0xFF));
    }
    else if(wFlags == eSpiU24Addr)
    {
        spi_send_byte(u8dev, ((u32CmdAddress>>16)&0xFF));
        spi_send_byte(u8dev, ((u32CmdAddress>>8)&0xFF));
    }
    spi_send_byte(u8dev, (u32CmdAddress&0xFF));
    //send data
    for(i=0; i<u16Count; i++)
    {
        spi_send_byte(u8dev, *pu8Buffer++);
    }
    spi_pin_set(pSpi->pstSpiCs, ePinLevel_High);

    return eSpiSuccess;
}

eSpiResult_t drv_spi_bus_read(uint8_t u8dev,
                                uint32_t u32CmdAddress,
                                uint16_t u16Count,
                                uint8_t* pu8Buffer,
                                eSpiAdrWide_t rFlags)
{
    uint16_t i;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8dev];

    spi_pin_set(pSpi->pstSpiCs, ePinLevel_Low);
    spi_delay(pSpi->u16SpiDelayByte);
    //send command or address
    if(rFlags == eSpiU16Addr)
    {
        spi_send_byte(u8dev, ((u32CmdAddress>>8)&0xFF));
        spi_delay(pSpi->u16SpiDelayByte);
    }
    else if(rFlags == eSpiU32Addr)
    {
        spi_send_byte(u8dev, ((u32CmdAddress>>24)&0xFF));
        spi_delay(pSpi->u16SpiDelayByte);
        spi_send_byte(u8dev, ((u32CmdAddress>>16)&0xFF));
        spi_delay(pSpi->u16SpiDelayByte);
        spi_send_byte(u8dev, ((u32CmdAddress>>8)&0xFF));
        spi_delay(pSpi->u16SpiDelayByte);
    }
    else if(rFlags == eSpiU24Addr)
    {
        spi_send_byte(u8dev, ((u32CmdAddress>>16)&0xFF));
        spi_delay(pSpi->u16SpiDelayByte);
        spi_send_byte(u8dev, ((u32CmdAddress>>8)&0xFF));
        spi_delay(pSpi->u16SpiDelayByte);
    }
    spi_send_byte(u8dev, (u32CmdAddress&0xFF));
    spi_delay(pSpi->u16SpiDelayByte);
    if(pSpi->pstSpiMiso == NULL) spi_dio_type_set(pSpi->pstSpiMosi, eSpiDioDir_Input);
    spi_delay(pSpi->u16SpiDelayByte);

    for(i=0; i<u16Count; i++)
    {
        *pu8Buffer++ = spi_get_byte(u8dev);
        spi_delay(pSpi->u16SpiDelayByte);
    }
    spi_pin_set(pSpi->pstSpiCs, ePinLevel_High);
    if(pSpi->pstSpiMiso == NULL) spi_dio_type_set(pSpi->pstSpiMosi, eSpiDioDir_Output);

    return eSpiSuccess;
}

eSpiResult_t drv_spi_direct_write(uint8_t u8dev,
                                uint16_t u16Count,
                                uint8_t* pu8Buffer)
{
    uint16_t i;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8dev];

    spi_pin_set(pSpi->pstSpiCs, ePinLevel_Low);
    //send data
    for(i=0; i<u16Count; i++)
    {
        spi_send_byte(u8dev, *pu8Buffer++);
    }
    spi_pin_set(pSpi->pstSpiCs, ePinLevel_High);

    return eSpiSuccess;
}































