/**
* @file  api_deint.h
* @brief 输入I视频转P视频数据
*/

/**
 * @defgroup DEINT_API Deinterlace module api
 *
 * @brief 用于输入视频从I制向P制转换
 * @{
 */

#ifndef __DEINT_API_H__
#define __DEINT_API_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"


/** Deint config参数*/
typedef struct _DEINT_DEV_T
{
    BOOL                    b_deint_en;            /**< 使能Deint的时钟和复位                        */
    UINT16                  u16_vactive;           /**< The number of valid pixels in a column      */
    BOOL                    b_hd_full_mode;        /**< 是否1920*1080格式输出                        */
    BOOL                    b_yuv444;              /**< 输入的时序是否为YUV444格式                    */
    UINT32                  u32_buff0_start_addr;  /**< 设置Deint的视频数据在buff0的存储起始地址,单位:Byte  */
    UINT32                  u32_buff1_start_addr;  /**< 设置Deint的视频数据在buff1的存储起始地址,注意一个buf分配的至少空间为for 480i:720x240x2 byte; for 576i:720x288x2 byte;for 1080i half mode:960x540x2 byte;for 1080 full mode :1920x540x2 byte*/
}DEINT_DEV_T;


/**
* @brief        DEINT模块初始化配置
* @param [in]   pst_deint_dev : DEINT模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_deint_instance_init(DEINT_DEV_T *pst_deint_dev);

/**
* @brief		Deinterlace int函数
* @param [in]   pst_deint_dev : Deint config参数指针,详见 DEINT_DEV_T
* @note         主要是用来使能deint的时钟
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_deint_init(DEINT_DEV_T *pst_deint_dev);

/**
* @brief		Deinterlace config函数
* @param [in]   pst_deint_dev : Deint config参数指针,详见 DEINT_DEV_T
* @note         主要是用来配置Deint模块的输入参数至Deint正常工作
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_deint_config(DEINT_DEV_T *pst_deint_dev);



#endif // _DEINT_API_H__



/**
 * @}
 */



