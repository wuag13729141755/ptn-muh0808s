#include "includes.h"
#include "ms1826_app.h"

extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];

u8 g_u8MS1826_I2CDevID = _I2C_TABLE_INDEX_MS1826_0;

/***************************************************************
*  Function name:   mculib_delay_ms
*  Description:     delay ms
*  Entry:           [in]u8_ms, uint ms
*
*  Returned value:  None
*  Remark:
***************************************************************/
VOID mculib_delay_ms(UINT8 u8_ms)
{
    delay_ms(u8_ms);
}

/***************************************************************
*  Function name:   mculib_delay_us
*  Description:     delay us
*  Entry:           [in]u8_us, uint us
*
*  Returned value:  None
*  Remark:
***************************************************************/
VOID mculib_delay_us(UINT8 u8_us)
{
    delay_us(u8_us);
}

/***************************************************************
*  Function name:   mculib_i2c_read_8bidx8bval
*  Description:     read back 8 bits register value with 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u8_index:  8 bits register index
*
*  Returned value: UINT8 type register value
*  Remark:
***************************************************************/
UINT8 mculib_i2c_read_8bidx8bval(UINT8 u8_address, UINT8 u8_index)
{
    UINT8 l_u8RValue=0;
	uint8_t ret_val;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, &l_u8RValue, 1, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, &l_u8RValue, 1, FALSE);
#endif
    if(ret_val)
    {
        log("read_8bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%02x] value[0x%02x]\r\n",g_u8MS1826_I2CDevID,u8_address,u8_index,l_u8RValue);
    }

	return l_u8RValue;
}

/***************************************************************
*  Function name:   mculib_i2c_write_8bidx8bval
*  Description:     write 8 bits register value to 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  8 bits register index
*                   [in]u8_value:   8 bits register value
*
*  Returned value: if write success return MS_TRUE, else return false
*  Remark:
***************************************************************/
BOOL mculib_i2c_write_8bidx8bval(UINT8 u8_address, UINT8 u8_index, UINT8 u8_value)
{
	uint8_t ret_val;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, &u8_value, 1, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, &u8_value, 1, FALSE);
#endif
    if(ret_val)
    {
        log("write_8bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%02x] value[0x%02x]\r\n",g_u8MS1826_I2CDevID,u8_address,u8_index,u8_value);
    }

	return (ret_val)?FALSE:TRUE;
}

/***************************************************************
*  Function name:   mculib_i2c_burstread_8bidx8bval
*  Description:     8 bits i2c burst read for 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u8_index:  8 bits register index
*                   [in]u8_length: 8 bits length to read
*                   [in]pu8_value:  read buffer
*
*  Returned value: None
*  Remark:
***************************************************************/
VOID mculib_i2c_burstread_8bidx8bval(UINT8 u8_address, UINT8 u8_index, UINT8 u8_length, UINT8 *pu8_value)
{
	uint8_t ret_val;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
#endif
    if(ret_val)
    {
        log("i2c_burstread_8bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%02x] len[0x%02x]\r\n",g_u8MS1826_I2CDevID,u8_address,u8_index,u8_length);
    }
}

/***************************************************************
*  Function name:   mculib_i2c_burstwrite_8bidx8bval
*  Description:     8 bits i2c burst read for 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  8 bits register index
*                   [in]u16_length: 8 bits length to read
*                   [in]pu8_value:  write buffer
*
*  Returned value: None
*  Remark:
***************************************************************/
VOID mculib_i2c_burstwrite_8bidx8bval(UINT8 u8_address, UINT8 u8_index, UINT8 u8_length, UINT8 *pu8_value)
{
	uint8_t ret_val;
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
#endif
    if(ret_val)
    {
        log("i2c_burstwrite_8bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%02x] len[0x%02x]\r\n",g_u8MS1826_I2CDevID,u8_address,u8_index,u8_length);
    }
}

/***************************************************************
*  Function name:   mculib_i2c_read_16bidx8bval
*  Description:     read back 8 bits register value with 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*
*  Returned value: UINT8 type register value
*  Remark:
***************************************************************/
UINT8 mculib_i2c_read_16bidx8bval(UINT8 u8_address, UINT16 u16_index)
{
    UINT8 l_u8RValue=0;
    UINT16 l_u16Offect;
	uint8_t ret_val;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, &l_u8RValue, 1, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, &l_u8RValue, 1, TRUE);
#endif
    if(ret_val)
    {
        log("read_16bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%04x] value[0x%02x]\r\n",g_u8MS1826_I2CDevID,u8_address,u16_index,l_u8RValue);
    }

	return l_u8RValue;
}

/***************************************************************
*  Function name:   mculib_i2c_write_16bidx8bval
*  Description:     write 8 bits register value to 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*                   [in]u8_value:   8 bits register value
*
*  Returned value: if write success return TRUE, else return FALSE
*  Remark:
***************************************************************/
BOOL mculib_i2c_write_16bidx8bval(UINT8 u8_address, UINT16 u16_index, UINT8 u8_value)
{
	uint8_t ret_val;
    UINT16 l_u16Offect;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, &u8_value, 1, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, &u8_value, 1, TRUE);
#endif
    if(ret_val)
    {
        log("write_16bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%04x] value[0x%02x]\r\n",g_u8MS1826_I2CDevID,u8_address,u16_index,u8_value);
    }

	return (ret_val)?FALSE:TRUE;

}

/***************************************************************
*  Function name:   mculib_i2c_burstread_16bidx8bval
*  Description:     8 bits i2c burst read for 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*                   [in]u16_length: 16 bits length to read
*                   [in]pu8_value:  read buffer
*
*  Returned value: if write success return TRUE, else return FALSE
*  Remark:
***************************************************************/
VOID  mculib_i2c_burstread_16bidx8bval(UINT8 u8_address, UINT16 u16_index, UINT16 u16_length, UINT8 *pu8_value)
{
    UINT16 l_u16Offect;
	uint8_t ret_val;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_read_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
#endif
    if(ret_val)
    {
        log("i2c_burstread_16bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%04x] len[0x%04x]\r\n",g_u8MS1826_I2CDevID,u8_address,u16_index,u16_length);
    }
}

/***************************************************************
*  Function name:   mculib_i2c_burstwrite_16bidx8bval
*  Description:     8 bits i2c burst write for 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*                   [in]u16_length: 16 bits length to write
*                   [in]pu8_value:  data buffer to write
*
*  Returned value: if write success return TRUE, else return FALSE
*  Remark:
***************************************************************/
VOID  mculib_i2c_burstwrite_16bidx8bval(UINT8 u8_address, UINT16 u16_index, UINT16 u16_length, UINT8 *pu8_value)
{
    UINT16 l_u16Offect;
	uint8_t ret_val;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS1826_I2CDevID], portMAX_DELAY);
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS1826_I2CDevID]);
#else
	ret_val = i2c_write_bytes(g_u8MS1826_I2CDevID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
#endif
    if(ret_val)
    {
        log("i2c_burstwrite_16bidx8bval err I2CDEV[%d] addr[0x%02x] offset[0x%04x] len[0x%04x]\r\n",g_u8MS1826_I2CDevID,u8_address,u16_index,u16_length);
    }
}

void MS1826_Reset(void)
{
    //u32 i;
    pstMS1826ChipResetPinCfg_t pstMS1826Rst,pstMS1826RstOrg = (pstMS1826ChipResetPinCfg_t)&stProjectMS1826ChipResetTable[0];

    pstMS1826Rst = pstMS1826RstOrg;
    while(pstMS1826Rst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstMS1826Rst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }

        pstMS1826Rst ++;
    }
    delay_ms(5);
    pstMS1826Rst = pstMS1826RstOrg;
    while(pstMS1826Rst->pstGpioCfgTable != NULL)   // pull low
    {
        if(pstMS1826Rst->eRstLevel == ePinLevel_Low)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }

        pstMS1826Rst ++;
    }
    delay_ms(10);
    pstMS1826Rst = pstMS1826RstOrg;
    while(pstMS1826Rst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstMS1826Rst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }

        pstMS1826Rst ++;
    }
    delay_ms(20);   // delay 20ms to reset chip ok
}

void MS1826_CHipReset(u8 u8DevID)
{
    //u32 i;
    pstMS1826ChipResetPinCfg_t pstMS1826Rst = (pstMS1826ChipResetPinCfg_t)&stProjectMS1826ChipResetTable[u8DevID];

    if(pstMS1826Rst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstMS1826Rst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
    }
    delay_ms(5);
    if(pstMS1826Rst->pstGpioCfgTable != NULL)   // pull low
    {
        if(pstMS1826Rst->eRstLevel == ePinLevel_Low)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
    }
    delay_ms(10);
    if(pstMS1826Rst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstMS1826Rst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS1826Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS1826Rst->pstGpioCfgTable->u16GpioPin);
        }
    }
    delay_ms(20);   // delay 20ms to reset chip ok
}


