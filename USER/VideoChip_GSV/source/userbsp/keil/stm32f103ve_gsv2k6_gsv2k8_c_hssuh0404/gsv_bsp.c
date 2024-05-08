/**
 * @file bsp.c
 *
 * @brief sample bsp support
 */
#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "gsv_bsp.h"
#if AvEnableKeyInput
//extern UART_HandleTypeDef huart1;
static uint8 KeyValue=0;
static uint8 KeyDelay=0;
//static uint8 Keylast=0;
#endif
extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];

AvRet ManI2cWrite(uint32 devAddress, uint32 regAddress, uint8 *i2cdata, uint16 count, uint8 index, uint8 Flag16bit);
AvRet ManI2cRead(uint32 devAddress, uint32 regAddress, uint8 *i2cdata, uint16 count, uint8 index, uint8 Flag16bit);

uint8 u8Gsv2kxDeviceTable[GsvTotalDevNumber] =
{
    #if (AvGsv2k6DeviceNumber>=1)
    _I2C_TABLE_INDEX_GSV_2K6_0,
    #endif
    #if (AvGsv2k6DeviceNumber>=2)
    _I2C_TABLE_INDEX_GSV_2K6_1,
    #endif
    #if (AvGsv2k6DeviceNumber>=3)
    _I2C_TABLE_INDEX_GSV_2K6_2,
    #endif
    #if (AvGsv2k6DeviceNumber>=4)
    _I2C_TABLE_INDEX_GSV_2K6_3,
    #endif

    #if (AvGsv2k8DeviceNumber>=1)
    _I2C_TABLE_INDEX_GSV_2K8_0,
    #endif
    #if (AvGsv2k8DeviceNumber>=2)
    _I2C_TABLE_INDEX_GSV_2K8_1,
    #endif
    #if (AvGsv2k8DeviceNumber>=3)
    _I2C_TABLE_INDEX_GSV_2K8_2,
    #endif
    #if (AvGsv2k8DeviceNumber>=4)
    _I2C_TABLE_INDEX_GSV_2K8_3,
    #endif

    #if (AvGsv2k2DeviceNumber>=1)
    _I2C_TABLE_INDEX_GSV_2K2_0,
    #endif
    #if (AvGsv2k2DeviceNumber>=2)
    _I2C_TABLE_INDEX_GSV_2K2_1,
    #endif
    #if (AvGsv2k2DeviceNumber>=3)
    _I2C_TABLE_INDEX_GSV_2K2_2,
    #endif
    #if (AvGsv2k2DeviceNumber>=4)
    _I2C_TABLE_INDEX_GSV_2K2_3,
    #endif

    #if (AvGsv2k1DeviceNumber>=1)
    _I2C_TABLE_INDEX_GSV_2K1_0,
    #endif
    #if (AvGsv2k1DeviceNumber>=2)
    _I2C_TABLE_INDEX_GSV_2K1_1,
    #endif
    #if (AvGsv2k1DeviceNumber>=3)
    _I2C_TABLE_INDEX_GSV_2K1_2,
    #endif
    #if (AvGsv2k1DeviceNumber>=4)
    _I2C_TABLE_INDEX_GSV_2K1_3,
    #endif
    #if (AvGsv2k1DeviceNumber>=5)
    _I2C_TABLE_INDEX_GSV_2K1_4,
    #endif
    #if (AvGsv2k1DeviceNumber>=6)
    _I2C_TABLE_INDEX_GSV_2K1_5,
    #endif
    #if (AvGsv2k1DeviceNumber>=7)
    _I2C_TABLE_INDEX_GSV_2K1_6,
    #endif
    #if (AvGsv2k1DeviceNumber>=8)
    _I2C_TABLE_INDEX_GSV_2K1_7,
    #endif

    #if (AvGsv2k11DeviceNumber>=1)
    _I2C_TABLE_INDEX_GSV_2K11_0,
    #endif
    #if (AvGsv2k11DeviceNumber>=2)
    _I2C_TABLE_INDEX_GSV_2K11_1,
    #endif
    #if (AvGsv2k11DeviceNumber>=3)
    _I2C_TABLE_INDEX_GSV_2K11_2,
    #endif
    #if (AvGsv2k11DeviceNumber>=4)
    _I2C_TABLE_INDEX_GSV_2K11_3,
    #endif
    #if (AvGsv2k11DeviceNumber>=5)
    _I2C_TABLE_INDEX_GSV_2K11_4,
    #endif
    #if (AvGsv2k11DeviceNumber>=6)
    _I2C_TABLE_INDEX_GSV_2K11_5,
    #endif
    #if (AvGsv2k11DeviceNumber>=7)
    _I2C_TABLE_INDEX_GSV_2K11_6,
    #endif
    #if (AvGsv2k11DeviceNumber>=8)
    _I2C_TABLE_INDEX_GSV_2K11_7,
    #endif
    #if (AvGsv2k11DeviceNumber>=9)
    _I2C_TABLE_INDEX_GSV_2K11_8,
    #endif
    #if (AvGsv2k11DeviceNumber>=10)
    _I2C_TABLE_INDEX_GSV_2K11_9,
    #endif
    #if (AvGsv2k11DeviceNumber>=11)
    _I2C_TABLE_INDEX_GSV_2K11_10,
    #endif
    #if (AvGsv2k11DeviceNumber>=12)
    _I2C_TABLE_INDEX_GSV_2K11_11,
    #endif
    #if (AvGsv2k11DeviceNumber>=13)
    _I2C_TABLE_INDEX_GSV_2K11_12,
    #endif
    #if (AvGsv2k11DeviceNumber>=14)
    _I2C_TABLE_INDEX_GSV_2K11_13,
    #endif
    #if (AvGsv2k11DeviceNumber>=15)
    _I2C_TABLE_INDEX_GSV_2K11_14,
    #endif
    #if (AvGsv2k11DeviceNumber>=16)
    _I2C_TABLE_INDEX_GSV_2K11_15,
    #endif

    #if (AvGsv5k1DeviceNumber>=1)
    _I2C_TABLE_INDEX_GSV_5K1_0,
    #endif
    #if (AvGsv5k1DeviceNumber>=2)
    _I2C_TABLE_INDEX_GSV_5K1_1,
    #endif
    #if (AvGsv5k1DeviceNumber>=3)
    _I2C_TABLE_INDEX_GSV_5K1_2,
    #endif
    #if (AvGsv5k1DeviceNumber>=4)
    _I2C_TABLE_INDEX_GSV_5K1_3,
    #endif
};

/**
 * @brief  init bsp
 * @return AvOk - success
 */
AvRet BspInit(void)
{
    AvRet ret = AvOk;
    return ret;
}
AvRet BspIrdaGetByte(uint8 *data)
{
    AvRet ret = AvOk;
    return ret;
}
/**
 * @brief  bsp i2c read function, support bus/dev address, 8/16 register address
 * @return AvOk - success
 */
AvRet BspI2cRead(uint32 devAddress, uint32 regAddress, uint8 *data, uint16 count)
{
    AvRet ret = AvOk;
    uint8 deviceAddress = (uint8)AvGetI2cDeviceAddress(devAddress);
    uint8 busAddress = (uint8)AvGetI2cBusAddress(devAddress);
    uint8 regAddressWidth = (uint8)AvGetRegAddressWidth(devAddress);
    uint16 regAdress = (uint16)((AvGetRegAddress(devAddress)<<8) | AvGetRegAddress(regAddress));
    regAddressWidth = (regAddressWidth == 0) ? 0 : 1;
    //ManI2cRead(deviceAddress, regAdress, data, count, busAddress, regAddressWidth);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[u8Gsv2kxDeviceTable[busAddress]], portMAX_DELAY);
#endif
    i2c_read_bytes(u8Gsv2kxDeviceTable[busAddress],deviceAddress,regAdress,data,count,(bool)regAddressWidth);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_IIC[u8Gsv2kxDeviceTable[busAddress]]);
#endif
    return ret;
}

/**
 * @brief  bsp i2c write function, support bus/dev address, 8/16 register address
 * @return AvOk - success
 */
AvRet BspI2cWrite(uint32 devAddress, uint32 regAddress, uint8 *data, uint16 count)
{
    AvRet ret = AvOk;
    uint8 deviceAddress = (uint8)AvGetI2cDeviceAddress(devAddress);
    uint8 busAddress = (uint8)AvGetI2cBusAddress(devAddress);
    uint8 regAddressWidth = (uint8)AvGetRegAddressWidth(devAddress);
    uint16 regAdress = (uint16)((AvGetRegAddress(devAddress)<<8) | AvGetRegAddress(regAddress));
    regAddressWidth = (regAddressWidth == 0) ? 0 : 1;
    //ManI2cWrite(deviceAddress, regAdress, data, count, busAddress, regAddressWidth);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[u8Gsv2kxDeviceTable[busAddress]], portMAX_DELAY);
#endif
    i2c_write_bytes(u8Gsv2kxDeviceTable[busAddress],deviceAddress,regAdress,data,count,(bool)regAddressWidth);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreGive(xSemaphore_IIC[u8Gsv2kxDeviceTable[busAddress]]);
#endif
    return ret;
}

#if 0
void UART_Transmit(uint8_t *pData, uint16_t Size)
{
    while(Size > 0)
    {
     Size--;
     while((USART2->SR&0x40)==0);
      USART2->DR = (*pData++);
    }
  /* Check that a Tx process is not already ongoing */
}
HAL_StatusTypeDef UART_Receive(uint8_t *pData, uint16_t Size)
{
    while(Size > 0)
    {
      Size--;
      if(USART2->SR&(1<<5))
      {
         *pData++ = USART2->DR;
         return HAL_OK;
      }
    }
    return HAL_BUSY;
}
#endif

/**
 * @brief  send one byte from uart
 * @return AvOk - success
 */
AvRet BspUartSendByte(uint8 *data, uint16 size)
{
    AvRet ret = AvOk;
#if 0//AvEnableDMAUart
    BspUartDMASendByte(data, size);
#elif 0//AvEnableUartInput
    HAL_UART_Transmit(&huart1, data, size, 100);
#endif
	pstUartDeclare_t 	pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

	while(pstUart->u8UartPort != _UART_PORT_NONE)
	{
		if(pstUart->bIsDebugPort)
		{
			break;
		}
		pstUart ++;
	}

	if(pstUart->u8UartPort == _UART_PORT_NONE)
	{
		return AvError;
	}
    USARTx_send_bytes(pstUart, data, size);

    return ret;
}

/**
 * @brief  get one byte from uart
 * @return AvOk - success
 */
AvRet BspUartGetByte(uint8 *data)
{
    AvRet ret = AvOk;
    //if(HAL_UART_Receive(&huart1, data, 1, 0) != HAL_OK)
    //    ret = AvError;
    return ret;
}

/**
 * @brief  get current time in ms
 * @return AvOk - success
 */
AvRet BspGetMilliSecond(uint32 *ms)
{
    AvRet ret = AvOk;
    //*ms = HAL_GetTick();
    return ret;
}

AvRet BspGetKey(uint8 *data)
{
    AvRet ret = AvNotAvailable;
#if 0//AvEnableKeyInput
    uint8 value;
    uint8 CombinedValue = 0;

    value = HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin);
    CombinedValue = CombinedValue | ((1-value)<<0);
    value = HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
    CombinedValue = CombinedValue | ((1-value)<<1);
    value = HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
    CombinedValue = CombinedValue | ((1-value)<<2);
    value = HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
    CombinedValue = CombinedValue | ((1-value)<<3);

    if(KeyValue != CombinedValue)
    {
        KeyValue = CombinedValue;
        KeyDelay = 0;
    }
    else if(KeyValue == CombinedValue)
    {
        if(KeyDelay <= AvKeyDelayThreshold)
            KeyDelay = KeyDelay + 1;
        if(KeyDelay == AvKeyDelayThreshold)
        {
            *data = KeyValue;
            ret = AvOk;
        }
    }
#endif

    return ret;
}

//#include "i2c.c"

#endif

