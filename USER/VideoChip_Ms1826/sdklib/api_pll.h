/**
* @file  api_pll.h
* @brief PLL模块相关数据结构及函数定义
*/

/**
 * @defgroup API_PLL pll api
 *
 * @brief pll module spi
 * @{
 */


#ifndef __PLL_API_H__
#define __PLL_API_H__

#include "ms_typedef.h"

typedef enum _PLL_E_
    {
        PLLV1    =  0,    //base: 0x6800
        PLLV2    =  1,    //base: 0x6900
        PLLV3    =  2,    //base: 0x6a00
        PLLV4    =  3,    //base: 0x6b00
        PLLM    =  4,     //base: 0x6c00
        PLLDEINT =  5     //base: 0x6d00
}PLL_E;


typedef struct _PLL_DEV_T
{
    UINT8 u8_pll_idx;                                      /**< pll index @ref PLL_E*/
    BOOL b_osc_input;                                    /**< is pll input from crystal*/

    UINT32 u32_clk_in;                                     /**< pll input clock, uint: Hz*/
    UINT32 u32_clk_out;                                    /**< pll output clock, uint: Hz*/

} PLL_DEV_T;

/**
* @brief        PLL模块结构体初始化
* @param [in]   pst_pll_dev : PLL模块对象指针
*               idx : 模块索引号
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
* @note         仅对结构体初始化，并未对模块初始化配置
*/
ERR_TYPE_E api_pll_instance_init(PLL_DEV_T *pst_pll_dev, UINT8 idx);

/**
* @brief        PLL模块配置
* @param [in]   pst_pll_dev : PLL模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_pll_config(PLL_DEV_T *pst_pll_dev);

/**
* @brief        关闭PLL模块
* @param [in]   u8_pll_idx : PLL模块对象， 参考PLL_E
* @return       ERR_INV_PARM if input is invalid\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_pll_powerdown(UINT8 u8_pll_idx);



#endif // __PLL_API_H__


/**
 * @}
 */
