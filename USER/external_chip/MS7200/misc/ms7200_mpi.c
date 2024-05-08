/**
******************************************************************************
* @file    ms7200_mpi.c
* @author
* @version V1.0.0
* @date    15-Nov-2014
* @brief   MacroSilicon Programming Interface source file
* @history
*
* Copyright (c) 2009-2014, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#include "MS7200/inc/ms7200_comm.h"
#include "MS7200/misc/ms7200_mpi.h"

static UINT8 s_u8chipAddr = MS7200_I2C_ADDR;

extern SemaphoreHandle_t xSemaphore_IIC[NUM_OF_I2C_BUS_MAX];

u8 g_u8MS7200_DeviceID = _I2C_TABLE_INDEX_MS7200_0;

/***************************************************************
*  Function name:   ms7200_delay_ms
*  Description:     delay ms
*  Entry:           [in]u8_ms, uint ms
*
*  Returned value:  None
*  Remark:
***************************************************************/
VOID ms7200_delay_ms(UINT8 u8_ms)
{
    delay_ms(u8_ms);
}

/***************************************************************
*  Function name:   ms7200_delay_us
*  Description:     delay us
*  Entry:           [in]u8_us, uint us
*
*  Returned value:  None
*  Remark:
***************************************************************/
VOID ms7200_delay_us(UINT8 u8_us)
{
    delay_us(u8_us);
}

/***************************************************************
*  Function name:   ms7200_i2c_read_8bidx8bval
*  Description:     read back 8 bits register value with 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u8_index:  8 bits register index
*
*  Returned value: UINT8 type register value
*  Remark:
***************************************************************/
UINT8 ms7200_i2c_read_8bidx8bval(UINT8 u8_address, UINT8 u8_index)
{
    UINT8 l_u8RValue=0;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, &l_u8RValue, 1, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, &l_u8RValue, 1, FALSE);
#endif

	return l_u8RValue;
}

/***************************************************************
*  Function name:   ms7200_i2c_write_8bidx8bval
*  Description:     write 8 bits register value to 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  8 bits register index
*                   [in]u8_value:   8 bits register value
*
*  Returned value: if write success return MS_TRUE, else return MS_FALSE
*  Remark:
***************************************************************/
BOOL ms7200_i2c_write_8bidx8bval(UINT8 u8_address, UINT8 u8_index, UINT8 u8_value)
{
	uint8_t ret_val;

#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	ret_val = i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, &u8_value, 1, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	ret_val = i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, &u8_value, 1, FALSE);
#endif

	return (ret_val)?FALSE:TRUE;
}

/***************************************************************
*  Function name:   ms7200_i2c_burstread_8bidx8bval
*  Description:     8 bits i2c burst read for 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u8_index:  8 bits register index
*                   [in]u8_length: 8 bits length to read
*                   [in]pu8_value:  read buffer
*
*  Returned value: None
*  Remark:
***************************************************************/
VOID ms7200_i2c_burstread_8bidx8bval(UINT8 u8_address, UINT8 u8_index, UINT8 u8_length, UINT8 *pu8_value)
{
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
#endif
}

/***************************************************************
*  Function name:   ms7200_i2c_burstwrite_8bidx8bval
*  Description:     8 bits i2c burst read for 8 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  8 bits register index
*                   [in]u16_length: 8 bits length to read
*                   [in]pu8_value:  write buffer
*
*  Returned value: None
*  Remark:
***************************************************************/
VOID ms7200_i2c_burstwrite_8bidx8bval(UINT8 u8_address, UINT8 u8_index, UINT8 u8_length, UINT8 *pu8_value)
{
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)u8_index, pu8_value, u8_length, FALSE);
#endif
}

/***************************************************************
*  Function name:   ms7200_i2c_read_16bidx8bval
*  Description:     read back 8 bits register value with 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*
*  Returned value: UINT8 type register value
*  Remark:
***************************************************************/
UINT8 ms7200_i2c_read_16bidx8bval(UINT8 u8_address, UINT16 u16_index)
{
    UINT8 l_u8RValue=0;
    UINT16 l_u16Offect;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, &l_u8RValue, 1, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, &l_u8RValue, 1, TRUE);
#endif

	return l_u8RValue;
}

/***************************************************************
*  Function name:   ms7200_i2c_write_16bidx8bval
*  Description:     write 8 bits register value to 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*                   [in]u8_value:   8 bits register value
*
*  Returned value: if write success return TRUE, else return FALSE
*  Remark:
***************************************************************/
BOOL ms7200_i2c_write_16bidx8bval(UINT8 u8_address, UINT16 u16_index, UINT8 u8_value)
{
	uint8_t ret_val;
    UINT16 l_u16Offect;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	ret_val = i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, &u8_value, 1, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	ret_val = i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, &u8_value, 1, TRUE);
#endif

	return (ret_val)?FALSE:TRUE;

}

/***************************************************************
*  Function name:   ms7200_i2c_burstread_16bidx8bval
*  Description:     8 bits i2c burst read for 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*                   [in]u16_length: 16 bits length to read
*                   [in]pu8_value:  read buffer
*
*  Returned value: if write success return TRUE, else return FALSE
*  Remark:
***************************************************************/
VOID  ms7200_i2c_burstread_16bidx8bval(UINT8 u8_address, UINT16 u16_index, UINT16 u16_length, UINT8 *pu8_value)
{
    UINT16 l_u16Offect;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	i2c_read_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
#endif
}

/***************************************************************
*  Function name:   ms7200_i2c_burstwrite_16bidx8bval
*  Description:     8 bits i2c burst write for 16 bits specified index
*  Entry:           [in]u8_address: 8 bits I2C slave address
*                   [in]u16_index:  16 bits register index
*                   [in]u16_length: 16 bits length to write
*                   [in]pu8_value:  data buffer to write
*
*  Returned value: if write success return TRUE, else return FALSE
*  Remark:
***************************************************************/
VOID  ms7200_i2c_burstwrite_16bidx8bval(UINT8 u8_address, UINT16 u16_index, UINT16 u16_length, UINT8 *pu8_value)
{
    UINT16 l_u16Offect;

    l_u16Offect = ((u16_index&0xFF00)>>8)|((u16_index&0xFF)<<8);
#if _SYSTEM_SUPPORT_RTOS
	xSemaphoreTake(xSemaphore_IIC[g_u8MS7200_DeviceID], portMAX_DELAY);
	i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
	xSemaphoreGive(xSemaphore_IIC[g_u8MS7200_DeviceID]);
#else
	i2c_write_bytes(g_u8MS7200_DeviceID, u8_address,(uint16_t)l_u16Offect, pu8_value, u16_length, TRUE);
#endif
}

VOID ms7200_HAL_SetChipAddr(UINT8 u8_address)
{
    s_u8chipAddr = u8_address;
}

UINT8 ms7200_HAL_GetChipAddr(VOID)
{
    return s_u8chipAddr;
}

UINT8 ms7200_HAL_ReadByte(UINT16 u16_index)
{
    return ms7200_i2c_read_16bidx8bval(s_u8chipAddr, u16_index);
}

VOID ms7200_HAL_WriteByte(UINT16 u16_index, UINT8 u8_value)
{
    ms7200_i2c_write_16bidx8bval(s_u8chipAddr, u16_index, u8_value);
}

UINT16 ms7200_HAL_ReadWord(UINT16 u16_index)
{
    return (HAL_ReadByte(u16_index) + ((UINT16)HAL_ReadByte(u16_index + 1) << 8));
}

VOID ms7200_HAL_WriteWord(UINT16 u16_index, UINT16 u16_value)
{
    HAL_WriteByte(u16_index, (UINT8)u16_value);
    HAL_WriteByte(u16_index + 1, (UINT8)(u16_value >> 8));
}

VOID ms7200_HAL_ModBits(UINT16 u16_index, UINT8 u8_mask, UINT8 u8_value)
{
    UINT8 tmp;
    tmp  = HAL_ReadByte(u16_index);
    tmp &= ~u8_mask;
    tmp |= (u8_value & u8_mask);
    HAL_WriteByte(u16_index, tmp);
}

VOID ms7200_HAL_SetBits(UINT16 u16_index, UINT8 u8_mask)
{
    HAL_ModBits(u16_index, u8_mask, u8_mask);
}

VOID ms7200_HAL_ClrBits(UINT16 u16_index, UINT8 u8_mask)
{
    HAL_ModBits(u16_index, u8_mask, 0x00);
}

VOID ms7200_HAL_ToggleBits(UINT16 u16_index, UINT8 u8_mask, BOOL b_set)
{
    (b_set) ? HAL_SetBits(u16_index, u8_mask) : HAL_ClrBits(u16_index, u8_mask);
}

UINT32 ms7200_HAL_ReadDWord(UINT16 u16_index)
{
    return (HAL_ReadWord(u16_index) + ((UINT32)HAL_ReadWord(u16_index + 2) << 16));
}

VOID ms7200_HAL_WriteDWord(UINT16 u16_index, UINT32 u32_value)
{
    HAL_WriteWord(u16_index, (UINT16)u32_value);
    HAL_WriteWord(u16_index + 2, (UINT16)(u32_value >> 16));
}


VOID ms7200_HAL_ReadBytes(UINT16 u16_index, UINT16 u16_length, UINT8 *p_u8_value)
{
    ms7200_i2c_burstread_16bidx8bval(s_u8chipAddr, u16_index, u16_length, p_u8_value);
}

VOID ms7200_HAL_WriteBytes(UINT16 u16_index, UINT16 u16_length, UINT8 *p_u8_value)
{
    ms7200_i2c_burstwrite_16bidx8bval(s_u8chipAddr, u16_index, u16_length, p_u8_value);
}


#if MS7200_EXT_APIS
UINT32 ms7200_HAL_ReadRange(UINT16 u16_index, UINT8 u8_bitpos, UINT8 u8_length)
{
    UINT32 u32_data = 0;
    UINT32 u32_mask = 0xffffffff >> (32 - u8_length);

    u32_mask <<= u8_bitpos;

    u32_data = HAL_ReadDWord(u16_index);
    u32_data &= u32_mask;
    u32_data >>= u8_bitpos;

    return u32_data;
}

VOID ms7200_HAL_WriteRange(UINT16 u16_index, UINT8 u8_bitpos, UINT8 u8_length, UINT32 u32_value)
{
    UINT32 u32_data = HAL_ReadDWord(u16_index);
    UINT32 u32_val = u32_value << u8_bitpos;
    UINT32 u32_mask = 0xffffffff >> (32 - u8_length);

    u32_mask <<= u8_bitpos;

    u32_data &= ~u32_mask;
    u32_val  &= u32_mask;
    u32_data |= u32_val;

    HAL_WriteDWord(u16_index, u32_data);
}

UINT32 ms7200_HAL_ReadRange_Ex(UINT16 u16_index, UINT8 u8_bitpos, UINT8 u8_length)
{
    UINT32 u32_data = 0;
    UINT32 u32_mask = 0xffffffff >> (32 - u8_length);

    u32_mask <<= u8_bitpos;

    u32_data = HAL_ReadDWord_Ex(u16_index);
    u32_data &= u32_mask;
    u32_data >>= u8_bitpos;

    return u32_data;
}

VOID ms7200_HAL_WriteRange_Ex(UINT16 u16_index, UINT8 u8_bitpos, UINT8 u8_length, UINT32 u32_value)
{
    UINT32 u32_data = HAL_ReadDWord_Ex(u16_index);
    UINT32 u32_val = u32_value << u8_bitpos;
    UINT32 u32_mask = 0xffffffff >> (32 - u8_length);

    u32_mask <<= u8_bitpos;

    u32_data &= ~u32_mask;
    u32_val  &= u32_mask;
    u32_data |= u32_val;

    HAL_WriteDWord_Ex(u16_index, u32_data);
}

#endif

//extend APIs is for HDMI RX register access only
VOID ms7200_HAL_ModBits_Ex(UINT16 u16_index, UINT32 u32_mask, UINT32 u32_value)
{
    UINT32 tmp;
    tmp  = HAL_ReadDWord_Ex(u16_index);
    tmp &= ~u32_mask;
    tmp |= (u32_value & u32_mask);
    HAL_WriteDWord_Ex(u16_index, tmp);
}

VOID ms7200_HAL_SetBits_Ex(UINT16 u16_index, UINT32 u32_mask)
{
    HAL_ModBits_Ex(u16_index, u32_mask, u32_mask);
}

VOID ms7200_HAL_ClrBits_Ex(UINT16 u16_index, UINT32 u32_mask)
{
    HAL_ModBits_Ex(u16_index, u32_mask, 0x00);
}

VOID ms7200_HAL_ToggleBits_Ex(UINT16 u16_index, UINT32 u32_mask, BOOL b_set)
{
    (b_set) ? HAL_SetBits_Ex(u16_index, u32_mask) : HAL_ClrBits_Ex(u16_index, u32_mask);
}

UINT32 ms7200_HAL_ReadDWord_Ex(UINT16 u16_index)
{
    UINT32 temp;
    UINT8 buff[4];
    HAL_ReadBytes(u16_index, 4, buff);
    temp = buff[3];
    temp = temp << 8 | buff[2];
    temp = temp << 8 | buff[1];
    temp = temp << 8 | buff[0];
    return temp;
}

VOID ms7200_HAL_WriteDWord_Ex(UINT16 u16_index, UINT32 u32_value)
{
    UINT8 buff[4];
    buff[0] = (UINT8)(u32_value);
    buff[1] = (UINT8)(u32_value >> 8);
    buff[2] = (UINT8)(u32_value >> 16);
    buff[3] = (UINT8)(u32_value >> 24);
    HAL_WriteBytes(u16_index, 4, buff);
}

void ms7200_SystemReset(void)
{
    //u32 i;
    pstChipResetPinCfg_t pstMS7200Rst,pstMS7200RstOrg = (pstChipResetPinCfg_t)&stProjectMs7200ChipResetTable[0];

    pstMS7200Rst = pstMS7200RstOrg;
    while(pstMS7200Rst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstMS7200Rst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS7200Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS7200Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS7200Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS7200Rst->pstGpioCfgTable->u16GpioPin);
        }

        pstMS7200Rst ++;
    }
    delay_ms(5);
    pstMS7200Rst = pstMS7200RstOrg;
    while(pstMS7200Rst->pstGpioCfgTable != NULL)   // pull low
    {
        if(pstMS7200Rst->eRstLevel == ePinLevel_Low)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS7200Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS7200Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS7200Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS7200Rst->pstGpioCfgTable->u16GpioPin);
        }

        pstMS7200Rst ++;
    }
    delay_ms(10);
    pstMS7200Rst = pstMS7200RstOrg;
    while(pstMS7200Rst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstMS7200Rst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstMS7200Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS7200Rst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstMS7200Rst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstMS7200Rst->pstGpioCfgTable->u16GpioPin);
        }

        pstMS7200Rst ++;
    }
    delay_ms(20);   // delay 20ms to reset chip ok
}


