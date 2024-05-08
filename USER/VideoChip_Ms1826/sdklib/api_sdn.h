/**
* @file  sdn.h  
* @brief scale down 模块相关配置                                                           
*/

/**
 * @defgroup SDN sdn api
 *
 * @brief 
 * @verbatim
 *  1）SDN模块支持1 ~ 1/256倍的缩小
 *  2）SDN模块输入的color space支持RGB和YUV444
 *      -当输入为RGB时，输出必须选择RGB
 *      -当输入为YUV444时，输出必须是YUV444
 * @endverbatim
 * @{
 */

#ifndef __SDN_H__
#define __SDN_H__

#include "ms_typedef.h"

/** SDN select*/
typedef enum _SDN_SEL_E
{
    SDN_SEL_SDN1 = 0,               /**<  选择SDN1 */
    SDN_SEL_SDN2 = 1,               /**<  选择SDN2 */
    SDN_SEL_SDN3 = 2,               /**<  选择SDN3 */
    SDN_SEL_SDN4 = 3,               /**<  选择SDN4 */
    SDN_NUM
}SDN_SEL_E;

/** SDN SIZE 表示方法*/
typedef struct _VIDEO_SIZE_T
{
    UINT16 u16_h;                   /**<  水平方向pixel个数 */
    UINT16 u16_v;                   /**<  垂直方向line个数 */
} VIDEO_SIZE_T;

/** SDN 输入、输出size*/
typedef struct _SDN_VIDEO_SIZE_T
{
    VIDEO_SIZE_T st_InRect;         /**<  SDN输入SIZE */
    VIDEO_SIZE_T st_MemRect;        /**<  SDN输出SIZE */
} SDN_VIDEO_SIZE_T;


/** SDN PEAKING配置*/
typedef struct _PEAKING_PARAM_T
{
    BOOL b_enable;                  /**<  SDN peaking enable or not */
    UINT8 u8_nkl;                   /**<  用于设置噪声的抑制程度，值越大噪声抑制效果越好，但同时也会削弱细节的增强效果,范围值0 ~ 15*/
    UINT8 u8_gain;                  /**<  more big more sharpness ，范围值[0 ~ 255]*/
}PEAKING_PARAM_T;

/** SDN 模块结构体*/
typedef struct _SDN_DEV_T
{
    UINT8 u8_sel;                           /**<  选择操作哪一路SDN ,参考SDN_SEL_E*/
    UINT8 b_enable;                         /**<  SDN的使能 */
    SDN_VIDEO_SIZE_T st_video_size;         /**<  SDN输入、输出size配置 */
    UINT8            u8_sdn_color_space;   /**<  设置sdn color space,refer to HDMI_COLORSPACE_E */
    PEAKING_PARAM_T  st_peaking;            /**<  SDN peaking 配置 */
}SDN_DEV_T;

/**
* @brief        SDN 模块的初始化
* @param [in]   pst_sdn_dev : related para:1、SDN_DEV_T 2、u8_sdn_sel：refer to SDN_SEL_E;		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sdn_instance_init(SDN_DEV_T *pst_sdn_dev,UINT8 u8_sdn_sel);

/**
* @brief        SDN 模块的使能和关闭
* @param [in]   pst_sdn_dev : 指向SDN模块的指针		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sdn_enable_set(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN 模块设置输入、输出size参数
* @param [in]   pst_sdn_dev : related para:1、u8_sel; 2、st_video_size		
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_sdn_scaler_size_set(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN 模块获取输入、输出size参数
* @param [in]   pst_sdn_dev : related para:1、u8_sel; 2、st_video_size		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sdn_scaler_size_get(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN 模块CSC配置
* @param [in]   pst_sdn_dev : related para:1、u8_sel; 2、u8_color_space		 
* @return       ERR_INV_PARM if input pointer is null
* @note         必须为RGB或者YUV444 Color space 
*/
ERR_TYPE_E api_sdn_csc_set(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN 模块PEAKING配置
* @param [in]   pst_sdn_dev : related para:1、u8_sel; 2、st_peaking		 
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_sdn_csc_peaking_config(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN 模块优化配置,当图像出现缺少边界信息时，根据实际效果可以尝试打开或者关闭此功能
* @param [in]   pst_sdn_dev : related para:1、u8_sel; 2、b_enable		 
* @return       ERR_INV_PARM if input pointer is null
* @note         1、当使能该功能时，配置SDN时，需要把输出size的水平及垂直的值都减1(st_MemRect)
*               2、DSC通路不能使能该功能
*/
ERR_TYPE_E api_sdn_optimize_config(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN 模块trigger
* @param [in]   pst_sdn_dev : related para:1、u8_sel; 		 
* @return       ERR_INV_PARM if input pointer is null
* @note
*  配置SDN模块scaler系数之后， trigger之后方能生效
*/
ERR_TYPE_E api_sdn_trigger(SDN_DEV_T *pst_sdn_dev);

#endif // _SDN_H__

/**
 * @}
 */
