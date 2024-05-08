/**
* @file  api_i2c2sdram.h
* @brief I2C2SDRAM模块头文件
*/

/**
 * @defgroup API_I2C2SDRAM i2c2sdram module api
 *
 * @brief i2c2sdram module spi
 * @{
 */


#ifndef __I2C2SDRAM_API_H__
#define __I2C2SDRAM_API_H__

/**
* @brief           从SDRAM中读取指定长度的数据
* @param [in]      u32_mem_addr:sdram的首地址
* @param [in&out]  pu8_value:读取的数据存放的首地址
* @param [in]      u32_len:读取数据的长度，单位BYTE
* @note            pu8_value的内存大小和长度需用户保证一致，不会造成内存越界
* @return          ERR_INV_PARM if input pointer is null
*                  MS_ERR_OK if init successed
*/
ERR_TYPE_E api_i2c2sdram_read(UINT32 u32_mem_addr, UINT8 *pu8_value, UINT32 u32_len);

/**
* @brief        向SDRAM中写入指定长度的数据
* @param [in]   u32_mem_addr:sdram的首地址
* @param [in]   pu8_value:写入数据存放的首地址
* @param [in]   u32_len:写入数据的长度，单位BYTE
* @return       ERR_INV_PARM if input pointer is null
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_i2c2sdram_write(UINT32 u32_mem_addr, UINT8 *pu8_value, UINT32 u32_len);


#endif // __I2C2SDRAM_API_H__


/**
 * @}
 */
