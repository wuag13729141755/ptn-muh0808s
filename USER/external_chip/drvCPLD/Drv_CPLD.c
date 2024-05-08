#include "includes.h"

#if (_ENABLE_CPLD_FUNCTION == _MACRO_ENABLE)
extern SemaphoreHandle_t xSemaphore_SPI[NUM_OF_SPI_BUS_MAX];
extern const stSpiDeclare_t stProjectSpiTable[];

void mapp_DataReverse(pu8 pu8InData, pu8 pu8OutData, u8 u8Len)
{
    u8 i,j,l_u8Temp=0;

    for(i=0;i<u8Len;i++)
    {
        l_u8Temp = 0;
        for(j=0;j<8;j++)
        {
            l_u8Temp = l_u8Temp<<1;
            l_u8Temp |= (((*pu8InData)>>j)&BIT0)?1:0;
        }
        pu8OutData[u8Len-i-1] = l_u8Temp;
    }
}

#if 1
void drvCPLD_WriteData(uint8_t addr,uint8_t data,bool bInvData)
{
    u8 l_u8RealAddr = addr&0x7f;

#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_SPI[eSpiDevice_Cpld], portMAX_DELAY);
#endif
    if(bInvData)
    {
        mapp_DataReverse(&l_u8RealAddr,&l_u8RealAddr,1);
        mapp_DataReverse(&data,&data,1);
    }
    drv_spi_bus_write(eSpiDevice_Cpld,l_u8RealAddr,1,&data,eSpiU8Addr);
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_SPI[eSpiDevice_Cpld]);
#endif
}

uint8_t drvCPLD_ReadData(uint8_t addr,bool bInvData)
{
    u8 l_u8RealAddr = addr|0x80;
    uint8_t ReadValue;

#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_SPI[eSpiDevice_Cpld], portMAX_DELAY);
#endif
    if(bInvData)
    {
        mapp_DataReverse(&l_u8RealAddr,&l_u8RealAddr,1);
    }
    drv_spi_bus_read(eSpiDevice_Cpld,l_u8RealAddr,1,&ReadValue,eSpiU8Addr);
    if(bInvData)
    {
        mapp_DataReverse(&ReadValue,&ReadValue,1);
    }
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_SPI[eSpiDevice_Cpld]);
#endif
    return ReadValue;
}
#else
void CPLD_delay(uint16_t delayTime)
{
    while(delayTime)
    {
        delayTime--;
    }
}

void CPLD_pin_set(pstGpioConfig_t pGpio, ePinLevel_t e_level)
{
    if(e_level == ePinLevel_High)
    {
        //GPIO_SetBits(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);

        #ifdef STM32F2XX_HD
		STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BSRRL= pGpio->u16GpioPin;
        #else
		STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BSRR = pGpio->u16GpioPin;
        #endif
    }
    else
    {
        //GPIO_ResetBits(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);

#ifdef STM32F2XX_HD
		STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BSRRH = pGpio->u16GpioPin;
#else
		STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->BRR = pGpio->u16GpioPin;
#endif
    }
}

ePinLevel_t CPLD_pin_get(pstGpioConfig_t pGpio)
{
    //GPIO_ReadInputDataBit(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA],pGpio->u16GpioPin);
    return ((STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA]->IDR & pGpio->u16GpioPin) ==\
        pGpio->u16GpioPin)?ePinLevel_High: ePinLevel_Low;
}

void CPLD_dio_type_set(pstGpioConfig_t pGpio, eSpiDioDirect_t eDioDirect)
{
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = pGpio->u16GpioPin;
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
    GPIO_Init(STM32_GPIO_PORT[pGpio->u8PortType-PORT_GPIOA], &GPIO_InitStructure);
}

void CPLD_send_byte(uint8_t u8Dev, uint8_t send_byte)
{
    uint8_t i = 0;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8Dev];

    for(i=0; i<8; i++)
    {
        if(send_byte&0x80)
        {
            CPLD_pin_set(pSpi->pstSpiMosi, ePinLevel_High);
        }
        else
        {
            CPLD_pin_set(pSpi->pstSpiMosi, ePinLevel_Low);
        }
        CPLD_pin_set(pSpi->pstSpiClk, ePinLevel_Low);
        CPLD_delay(pSpi->u16SpiDelayClk);
        CPLD_pin_set(pSpi->pstSpiClk, ePinLevel_High);
        CPLD_delay(pSpi->u16SpiDelayClk);
        send_byte <<= 1;
    }
}

uint8_t CPLD_get_byte(uint8_t u8Dev)
{
    uint8_t i=0, temp=0x00;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8Dev];

    for(i=0; i<8; i++)
    {
        temp <<= 1;
        CPLD_pin_set(pSpi->pstSpiClk, ePinLevel_Low);
        CPLD_delay(pSpi->u16SpiDelayClk);
        CPLD_pin_set(pSpi->pstSpiClk, ePinLevel_High);
        CPLD_delay(pSpi->u16SpiDelayClk);
        if(pSpi->pstSpiMiso == NULL)
        {
            if(CPLD_pin_get(pSpi->pstSpiMosi))//dio mode
            {
                temp |= 0x01;
            }
        }
        else
        {
            if(CPLD_pin_get(pSpi->pstSpiMiso))//normal mode
            {
                temp |= 0x01;
            }
        }
    }

    return temp;
}

eSpiResult_t drv_CPLD_bus_write(uint8_t u8Dev,
                                uint16_t u16CmdAddress,
                                uint16_t u16Count,
                                uint8_t* pu8Buffer,
                                eSpiAdrWide_t wFlags)
{
    uint16_t i;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8Dev];

    CPLD_pin_set(pSpi->pstSpiCs, ePinLevel_Low);
    //send command or address
    if(wFlags == eSpiU16Addr)
    {
        CPLD_send_byte(u8Dev, ((u16CmdAddress>>8)&0xFF));
    }
    CPLD_send_byte(u8Dev, (u16CmdAddress&0xFF));
    //send data
    for(i=0; i<u16Count; i++)
    {
        CPLD_send_byte(u8Dev, *pu8Buffer++);
    }
    CPLD_pin_set(pSpi->pstSpiCs, ePinLevel_High);

    return eSpiSuccess;
}

eSpiResult_t drv_CPLD_bus_read(uint8_t u8Dev,
                                uint16_t u16CmdAddress,
                                uint16_t u16Count,
                                uint8_t* pu8Buffer,
                                eSpiAdrWide_t rFlags)
{
    uint16_t i;
    pstSpiDeclare_t pSpi = (pstSpiDeclare_t)&stProjectSpiTable[u8Dev];

    CPLD_pin_set(pSpi->pstSpiCs, ePinLevel_Low);
    CPLD_delay(pSpi->u16SpiDelayByte);
    //send command or address
    if(rFlags == eSpiU16Addr)
    {
        CPLD_send_byte(u8Dev, ((u16CmdAddress>>8)&0xFF));
        CPLD_delay(pSpi->u16SpiDelayByte);
    }
    CPLD_send_byte(u8Dev, (u16CmdAddress&0xFF));
    if(pSpi->pstSpiMiso == NULL) CPLD_dio_type_set(pSpi->pstSpiMosi, eSpiDioDir_Input);
    CPLD_delay(pSpi->u16SpiDelayByte);

    for(i=0; i<u16Count; i++)
    {
        *pu8Buffer++ = CPLD_get_byte(u8Dev);
        CPLD_delay(pSpi->u16SpiDelayByte);
    }
    CPLD_pin_set(pSpi->pstSpiCs, ePinLevel_High);
    if(pSpi->pstSpiMiso == NULL) CPLD_dio_type_set(pSpi->pstSpiMosi, eSpiDioDir_Output);

    return eSpiSuccess;
}

void drvCPLD_WriteData(uint8_t addr,uint8_t data)
{
    xSemaphoreTake(xSemaphore_SPI[eSpiDevice_Cpld], portMAX_DELAY);
    drv_CPLD_bus_write(eSpiDevice_Cpld,addr&0x7f,1,&data,eSpiU8Addr);
    xSemaphoreGive(xSemaphore_SPI[eSpiDevice_Cpld]);
}

uint8_t drvCPLD_ReadData(uint8_t addr)
{
    uint8_t ReadValue;

    xSemaphoreTake(xSemaphore_SPI[eSpiDevice_Cpld], portMAX_DELAY);
    drv_CPLD_bus_read(eSpiDevice_Cpld,addr|0x80,1,&ReadValue,eSpiU8Addr);
    xSemaphoreGive(xSemaphore_SPI[eSpiDevice_Cpld]);
    return ReadValue;
}

#endif //add by wf8421

#endif



