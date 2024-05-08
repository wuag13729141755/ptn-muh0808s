/**
* @file  api_dmc.h
* @brief DMC模块头文件
*/

/**
 * @defgroup API_DMC DMC module api
 *
 * @brief dmc module spi
 * @{
 */


#ifndef __DMC_API_H__
#define __DMC_API_H__

#include "ms_typedef.h"

 /** sdram speed bin (speed grade) selection*/
typedef enum _SDRAM_SPDBIN_E
{
    SDRAM_SPDBIN_1600M = 1600,        /**< 1600MT/s */
    SDRAM_SPDBIN_1333M = 1333,        /**< 1333MT/s */
    SDRAM_SPDBIN_1066M = 1066,        /**< 1066MT/s */
    SDRAM_SPDBIN_800M  = 800,         /**< 800MT/s */
    SDRAM_SPDBIN_NUM                  /**< sdram speed bin numbers*/
} SDRAM_SPDBIN_E;

/** dmc init structure*/
typedef struct _DMC_T
{
    UINT32 u32_sdram_spdbin;     /**< ddr3 transmission rate, uint: MT/s*/
    UINT32 u32_apb_freq;       /**< xtal clock, uint: MHz*/
} DMC_T;

/**
* @brief        DMC模块初始化
* @param [in]   pst_dmc_cfg: ddr 的配置信息
* @note         完成DMC模块的初始化工作
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
UINT8 api_dmc_init(DMC_T *pst_dmc_cfg);


#endif // __DMC_API_H__


/**
 * @}
 */
