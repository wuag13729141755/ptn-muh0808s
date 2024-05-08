/**
* @file  api_dsc.h
* @brief HDMI_RX模块相关数据结构及函数定义
*/

/**
 * @defgroup API_DSC DSC module api
 *
 * @brief 视频压缩传输
 * @{
 */
#ifndef __API_DSC_H__
#define __API_DSC_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"

#define DSC_SLINE_NUMS          (2)
#define DSC_BP_ENABLE           (1)
#define DSC_SLOA                (512)
#define DSC_RC_MOD_BUFF_SIZ     (8192)
#define DSC_MIN_VERSION         (2)

/** DSC select*/
typedef enum _DSC_SEL_E
{
    DSC_SEL_1 = 0,                  /**<  选择DSC1 */
    DSC_SEL_2 ,                     /**<  选择DSC2 */
    DSC_SEL_3 ,                     /**<  选择DSC3 */
    DSC_SEL_4 ,                     /**<  选择DSC4 */
    DSC_SEL_NUM
}DSC_SEL_E;

/** DSC 错误类型*/
typedef enum _DSC_ERROR_TYPE_E
{
    DSC_NO_ERR = 0,
    DSC_BPC_ERR ,
    DSC_MULT_MODE_ERR ,
    DSC_LINE_BUFFER_DEPTH_ERR ,
    DSC_MNV_ERR ,
    DSC_PIC_HEIGHT_ERR ,
    DSC_PIC_WIDTH_ERR ,
    DSC_SLICE_NUM_ERR ,
    DSC_SLICE_HEIGHT_ERR ,
    DSC_SLICE_WIDTH_ERR ,
    DSC_SECOND_LINE_BPG_ERR ,
    DSC_NON_SECOND_LINE_BPG_ERR ,
    DSC_PPS_ID_ERR ,
    DSC_BPP_ERR ,
    DSC_RC_BUFFER_OVERFLOW_ERR,
    DSC_RC_BUFFER_UNDERFLOW_ERR,
    DSC_ICH_ERR ,
    DSC_UNSUCC_RESET_CYCLE
}DSC_ERROR_TYPE_E;

/** DSC 时钟分频*/
typedef enum _DSC_DATA_PATH_CLK_E
{
    PXL_CLK_DIV1 = 0,
    PXL_CLK_DIV2 ,
    PXL_CLK_DIV3 ,
    PXL_CLK_DIV4 ,
    PXL_CLK_DIV5 ,
    PXL_CLK_DIV6 ,
    PXL_CLK_DIV7 ,
    PXL_CLK_DIV8 ,
    PXL_CLK_DIV9 ,
    PXL_CLK_DIV10 ,
    PXL_CLK_DIV11 ,
    PXL_CLK_DIV12 ,
    PXL_CLK_DIV13 ,
    PXL_CLK_DIV14 ,
    PXL_CLK_DIV15 ,
    PXL_CLK_DIV16
}DSC_DATA_PATH_CLK_E;


/** DSC 压缩比*/
typedef enum _DSC_RATIO_E
{
    DSC_RATIO_2X = 0,           /**<  DSC 2倍压缩 */
    DSC_RATIO_3X ,              /**<  DSC 3倍压缩 */
    DSC_RATIO_4X                /**<  DSC 4倍压缩 */
}DSC_RATIO_E;



/** DSC 参数配置结构体*/
typedef struct _DSC_CONFIG_PARAM_T
{
    UINT8   u8_color_space;       /**< color space, @ref HDMI_COLORSPACE_E                 */
    UINT16  u16_hactive;          /**< 一行的有效点数                                      */
    UINT16  u16_vactive;          /**< 一列的有效行数                                      */
    UINT8   u8_dsc_data_path_clk; /**< 时钟分频，默认选择div2, @ref DSC_DATA_PATH_CLK_E    */
    UINT8   u8_dsc_ratio;         /**< DSC压缩比  @ref DSC_RATIO_E */
} DSC_CONFIG_PARAM_T;

/** DSC 工作状态结构体*/
typedef struct _DSC_STATUS_T
{
    UINT32  u32_err_code_word;                     /**< 错误字*/
    UINT8   u8_err_status;                         /**< 错误类型, @ref DSC_ERROR_TYPE_E*/
} DSC_STATUS_T;

/** DSC 模块结构体*/
typedef struct _DSC_DEV_T
{
    UINT8 u8_sel;                                  /**< dsc 选择  @ref DSC_SEL_E*/
    DSC_CONFIG_PARAM_T st_dsc_config_param;        /**< dsc 参数配置 @ref DSC_CONFIG_PARAM_T*/
    DSC_STATUS_T st_dsc_err_status;                /**< dsc 状态 @ref DSC_STATUS_T*/
}DSC_DEV_T;

/**
* @brief        dsc encoder模块对象实例初始化
* @param [in]   pst_dsc_dev : dsc encoder模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_encoder_instance_init(DSC_DEV_T *pst_dsc_dev, UINT8 u8_sel);

/**
* @brief        dsc decoder模块对象实例初始化
* @param [in]   pst_dsc_dev : dsc decoder模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_decoder_instance_init(DSC_DEV_T *pst_dsc_dev, UINT8 u8_sel);

/**
* @brief        dsc encoder 配置
* @param [in]   pst_dsc_dev : dsc 编码器模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_encoder_set(DSC_DEV_T *pst_dsc_dev);

/**
* @brief        dsc decoder 配置
* @param [in]   pst_dsc_dev : dsc 解码器模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_decoder_set(DSC_DEV_T *pst_dsc_dev);

/**
* @brief        dsc encoder 输出使能
* @param [in]   pst_tx_dev : dsc encoder 对象指针 涉及参数:pst_dsc_dev->u8_sel
*               b_enable : MS_TRUE or false
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_encoder_enable(DSC_DEV_T *pst_dsc_dev,BOOL b_enable);

/**
* @brief        dsc decoder 输出使能
* @param [in]   pst_tx_dev : dsc decoder模块对象指针 涉及参数:pst_dsc_dev->u8_sel
*               b_enable : MS_TRUE or false
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_decoder_enable(DSC_DEV_T *pst_dsc_dev,BOOL b_enable);

/**
* @brief        dsc encoder 运行状态获取
* @param [in]   pst_dsc_dev : dsc encoder模块对象指针  涉及参数:pst_dsc_dev->st_dsc_err_status
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_encoder_status_get(DSC_DEV_T *pst_dsc_dev);

/**
* @brief        dsc decoder 运行状态获取
* @param [in]   pst_dsc_dev : dsc decoder模块对象指针  涉及参数:pst_dsc_dev->st_dsc_err_status
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_decoder_status_get(DSC_DEV_T *pst_dsc_dev);

/**
* @brief        无缝切换模式下配置更新dsc decoder参数
* @param [in]   pst_dsc_dev : dsc decoder 模块对象指针  涉及参数:pst_dsc_dev下所有参数
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_decoder_pps_seamless_set(DSC_DEV_T *pst_dsc_dev);

/**
* @brief        无缝切换模式下trigger dsc decoder参数
* @param [in]   pst_dsc_dev : dsc decoder 模块对象指针  涉及参数:pst_dsc_dev->u8_sel
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_dsc_decoder_misc_seamless_set(DSC_DEV_T *pst_dsc_dev);
#endif

/**
 * @}
 */
