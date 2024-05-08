/**
* @file  api_i2c2sdram.h
* @brief I2C2SDRAMģ��ͷ�ļ�
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
* @brief           ��SDRAM�ж�ȡָ�����ȵ�����
* @param [in]      u32_mem_addr:sdram���׵�ַ
* @param [in&out]  pu8_value:��ȡ�����ݴ�ŵ��׵�ַ
* @param [in]      u32_len:��ȡ���ݵĳ��ȣ���λBYTE
* @note            pu8_value���ڴ��С�ͳ������û���֤һ�£���������ڴ�Խ��
* @return          ERR_INV_PARM if input pointer is null
*                  MS_ERR_OK if init successed
*/
ERR_TYPE_E api_i2c2sdram_read(UINT32 u32_mem_addr, UINT8 *pu8_value, UINT32 u32_len);

/**
* @brief        ��SDRAM��д��ָ�����ȵ�����
* @param [in]   u32_mem_addr:sdram���׵�ַ
* @param [in]   pu8_value:д�����ݴ�ŵ��׵�ַ
* @param [in]   u32_len:д�����ݵĳ��ȣ���λBYTE
* @return       ERR_INV_PARM if input pointer is null
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_i2c2sdram_write(UINT32 u32_mem_addr, UINT8 *pu8_value, UINT32 u32_len);


#endif // __I2C2SDRAM_API_H__


/**
 * @}
 */
