/**
* @file  api_4ksdn.h  
* @brief 4KSDN模块相关数据结构及函数定义                                                           
*/

/**
 * @defgroup API_SDN4K 4KSDN module api
 *
 * @brief 用于对输入4K视频进行ScaleDown To 2k
 * @{
 */

#ifndef __API_4KSDN__
#define __API_4KSDN__

#include "ms_typedef.h"
#include "ms_hdmi.h"

/** 4ksdn index*/
typedef enum _SDN4K_INDEX_E 
{
    SDN4K_1 = 0,    /**< 4ksdn1 */
    SDN4K_2,        /**< 4ksdn2 */
    SDN4K_3,        /**< 4ksdn3 */
    SDN4K_4,        /**< 4ksdn4 */
    
    SDN4K_BUTT,
}SDN4K_INDEX_E;

/** 4ksdn工作模式*/
typedef enum _SDN4K_MODE_E
{
    SDN4K_MODE_BYPASS = 0,  /**<  BYPASS（Default）  */
    SDN4K_MODE_AUTO,        /**<  AUTO模式，适用输入为4k@60hz_444/4k@60hz_420/4k422@30hz */
    SDN4K_MODE_MANUAL,      /**<  MANUAL模式，适用分辨率小于4k@30hz/4k@60_420 */
    SDN4K_MODE_2Kx2K,       /**<  2Kx2K模式，适用特殊处理需求 */

    SDN4K_MODE_BUTT,
}SDN4K_MODE_E;

/** 4ksdn模块配置信息参数*/
typedef struct _SDN4K_DEV_T
{
    UINT8 u8_sdn4k_idx;                         /**<  4KSDN模块索引，详见SDN4K_INDEX_E */
    UINT8 u8_sdn4k_mode;                        /**<  4KSDN模块工作模式，详见SDN4K_MODE_E定义 */
    UINT8 u8_input_colorSpace;                  /**<  输入颜色空间格式，详见HDMI_COLORSPACE_E定义 */ 
    HDMI_VIDEO_TIMING_T st_input_videoTiming;   /**<  输入Timing参数 */
    UINT8 u8_output_colorSpace;                 /**<  输出色彩空间格式，无需配置，详见HDMI_COLORSPACE_E定义 */
    HDMI_VIDEO_TIMING_T st_output_videoTiming;  /**<  输出Timing参数，无需配置 */
}SDN4K_DEV_T;

/** 4KSDN TIMING配置参数 */
typedef struct _SDN4K_TIMEING_PARAM_T
{
    UINT16 u16_hsync_width;     /**<  h_blank宽度 */
    UINT16 u16_hblank_posedge;  /**<  h_blank上升沿位置 */
    UINT16 u16_hblank_negedge;  /**<  h_blank下降沿位置 */
}SDN4K_TIMEING_PARAM_T;

/**
* @brief        4KSDN模块结构体初始化
* @param [in]   pst_sdn4k_dev : 4KSDN参数指针 
*               idx : 模块索引号
* @return       specific error types if call failed
* @note         仅对结构体初始化，并未对模块初始化配置
*/
ERR_TYPE_E api_4ksdn_instance_init(SDN4K_DEV_T *pst_sdn4k_dev, UINT8 idx);

/**
* @brief        4KSDN模块初始化
* @param [in]   pst_sdn4k_dev : 4KSDN DEV参数指针 
* @return       specific error types if call failed
* @note         初始化后默认为BYPASS模式        
*/
ERR_TYPE_E api_sdn4k_init(SDN4K_DEV_T *pst_sdn4k_dev);
/**
* @brief        4KSDN模块使能
* @param [in]   pst_sdn4k_dev : 4KSDN DEV参数指针 
*               b_enable : 是否使能
* @return       specific error types if call failed
* @note                 
*/
ERR_TYPE_E api_sdn4k_enable(SDN4K_DEV_T *pst_sdn4k_dev, BOOL b_enable);
/**
* @brief        4KSDN 配置函数
* @param [in]   pst_sdn4k_dev : 4KSDN DEV参数指针
* @return       specific error types if call failed
* @note                    
*/
ERR_TYPE_E api_sdn4k_config(SDN4K_DEV_T *pst_sdn4k_dev);

#endif // _4KSDN_H__

/**
 * @}
 */


