/**
* @file  api_csc.h
* @brief csc模块相关数据结构及函数定义
*/

/**
 * @defgroup API_CSC CSC module api
 *
 * @brief 颜色空间转换
 * @{
 */

#ifndef __API_CSC_H__
#define __API_CSC_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"

/** csc 模块选择*/
typedef enum _CSC_SEL_E
{
    CSC_SEL_1 = 0,
    CSC_SEL_2 ,
    CSC_SEL_3 ,
    CSC_SEL_4 ,
    CSC_SEL_NUM
}CSC_SEL_E;

/** csc color range选择*/
typedef enum CSC_COLOR_RANGE
{
    CSC_RGB_16_235 = 0,
    CSC_RGB_0_255,
    CSC_YUV444_BT601,
    CSC_YUV444_BT709,
} CSC_COLOR_RANGE_E;

/** csc 参数设置*/
typedef struct _CSC_CONFIG_PARAM_T
{
    UINT8  csc_in_colorspace;           /**< csc 输入的颜色空间 @ref CSC_HDMI_COLOR_SPACE_E*/
    UINT8  csc_out_colorspace;          /**< csc 输出的颜色空间 @ref CSC_HDMI_COLOR_SPACE_E*/
    UINT8  csc_in_color_range;          /**< csc 输入的色域范围 @ref CSC_COLOR_RANGE_E*/
    UINT8  csc_out_color_range;         /**< csc 输出的色域范围 @ref CSC_COLOR_RANGE_E*/
    UINT8  u8_pixel_repeat;             /**< csc 输入的视频repeater倍数，只有tx csc需要配置，rx csc不需要配置，
                                             当tx选择video mixer输出时，需要配置u8_pixel_repeat=0，不做pixel repeat。
                                             当tx选择rx1/2/3/4 video时，需要配置u8_pixel_repeat和对应的rx的输入的pixel repeat一致。
                                             @ref HDMI_PIXEL_REPEAT_E*/
} CSC_CONFIG_PARAM_T;

/** csc 模块结构体*/
typedef struct _CSC_DEV_T
{
    UINT8 u8_sel;                                 /**< csc index @ref CSC_SEL_E*/
    CSC_CONFIG_PARAM_T st_csc_config_param;       /**< csc config param @ref CSC_CONFIG_PARAM_T*/
}CSC_DEV_T;

/**
* @brief        csc rx模块对象实例初始化
* @param [in]   pst_csc_dev : csc模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_rx_instance_init(CSC_DEV_T *pst_csc_dev, UINT8 u8_rx_sel);

/**
* @brief        csc tx模块对象实例初始化
* @param [in]   pst_csc_dev : csc模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_instance_init(CSC_DEV_T *pst_csc_dev, UINT8 u8_tx_sel);

/**
* @brief        csc rx模块配置
* @param [in]   pst_csc_dev : csc模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if successed
*/
ERR_TYPE_E api_csc_rx_set(CSC_DEV_T *pst_csc_dev);

/**
* @brief        csc tx模块配置
* @param [in]   pst_csc_dev : csc模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_set(CSC_DEV_T *pst_csc_dev);

/**
* @brief        csc rx yuv422到yuv444转换过程滤波功能bypass
* @param [in]   pst_csc_dev : csc模块对象指针  related para:u8_sel
*               b_enable : TRUE 表示bypass FALSE 表示开启filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_rx_yuv422_to_yuv444_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

/**
* @brief        csc rx yuv444到yuv422转换过程中滤波功能bypass
* @param [in]   pst_csc_dev : csc模块对象指针  related para:u8_sel
*               b_enable : TRUE 表示bypass FALSE 表示开启filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_rx_yuv444_to_yuv422_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

/**
* @brief        csc tx yuv422到yuv444转换过程滤波功能bypass
* @param [in]   pst_csc_dev : csc模块对象指针  related para:u8_sel
*               b_enable : TRUE 表示bypass FALSE 表示开启filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_yuv422_to_yuv444_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

/**
* @brief        csc tx yuv444到yuv422转换过程中滤波功能bypass
* @param [in]   pst_csc_dev : csc模块对象指针  related para:u8_sel
*               b_enable : TRUE 表示bypass FALSE 表示开启filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_yuv444_to_yuv422_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

#endif

/**
 * @}
 */
