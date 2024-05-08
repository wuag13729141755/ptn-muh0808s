/**
* @file  api_video_path_process.h  
* @brief 视频通路配置函数                                                          
*/

/**
 * @defgroup SS_API video path process api
 *
 * @brief 用于实现视频通路配置
 * @{
 */
#ifndef __VIDEO_PROCESS_API_H__
#define __VIDEO_PROCESS_API_H__

#include "ms_typedef.h"

typedef enum _SS_CH_SEL_E
{
    CHANNEL1_with_dsc  = 1,         /**< 表示视频通路1经过DSC的path    */
    CHANNEL1_without_dsc  = 2,      /**< 表示视频通路1不经过DSC的path  */
    CHANNEL2_with_dsc  = 4,         /**< 表示视频通路2经过DSC的path    */
    CHANNEL2_without_dsc  = 8,      /**< 表示视频通路2不经过DSC的path  */
    CHANNEL3_with_dsc  = 16,        /**< 表示视频通路2经过DSC的path    */
    CHANNEL3_without_dsc  = 32,     /**< 表示视频通路3不经过DSC的path  */
    CHANNEL4_with_dsc  = 64,        /**< 表示视频通路4经过DSC的path    */
    CHANNEL4_without_dsc  = 128,    /**< 表示视频通路4不经过DSC的path  */
}SS_CH_SEL_E;

typedef enum _SS_VIPSS_PATH_SEL_E
{
    VIPSS1  = 0,   /**< 表示视频通路1 */
    VIPSS2  = 1,   /**< 表示视频通路2 */
    VIPSS3  = 2,   /**< 表示视频通路3 */
    VIPSS4  = 3,   /**< 表示视频通路4 */
}SS_VIPSS_PATH_SEL_E;

typedef enum _SS_VOPSS_PATH_SEL_E
{
    VOPSS1  = 0,   /**< 表示视频通路1 */
    VOPSS2  = 1,   /**< 表示视频通路2 */
    VOPSS3  = 2,   /**< 表示视频通路3 */
    VOPSS4  = 3,   /**< 表示视频通路4 */
}SS_VOPSS_PATH_SEL_E;


/** 使用VOPSS 中的trigger时，选择trigger方式*/
typedef enum __SS_TRIGGER_MODE
{
    FORCE_TRIGGER = 0,
    VB_INTERVAL_TRIGGER = 1,
    NEXT_VB_RISING_EDGE_TRIGGER = 2
}SS_TRIGGER_MODE;

/** 使用TIMGEN trigger时，选择trigger哪一路TIMGEN*/
typedef enum __SS_TRIGGER_TMG_E
{
    VDS_TMG1 = 0,
    VDS_TMG2 = 1,
    VDS_TMG3 = 2,
    VDS_TMG4 = 3
}SS_TRIGGER_TMG_E;

/** 选择使用哪种trigger方式*/
typedef enum __SS_TRIGGER_SEL_E
{
    TRIGGER_BY_VOPSS_TRIGGER = 0,              /**< 使用vopss中各自的globle trigger  */ 
    TRIGGER_BY_VDS_TMG                         /**< 多路输出使用同一个timgen，并且同时trigger，refer to SS_TRIGGER_TMG_E  */ 
}SS_TRIGGER_SEL_E;

/** VOPSS trigger 设置*/
typedef struct __SS_VOPSS_TRIGGER_SET_T
{
    UINT8 u8_trigger_sel;                      /**< VDS TMG trigger或者VOPSS trigger，refer to SS_TRIGGER_SEL_E  */  
    UINT8 u8_trigger_tmg;                      /**< SS_TRIGGER_TMG_E,only effctive for TRIGGER_BY_VDS_TMG  */
    UINT8 u8_trigger_mode;                     /**< SS_TRIGGER_MODE,only effctive for TRIGGER_BY_VOPSS_TRIGGER */    
}SS_VOPSS_TRIGGER_SET_T;

typedef struct _SS_DEV_T
{
    UINT8 u8_sel;                                       /**< 需要操作的视频通道，参考SS_CH_SEL_E，选择多通道时可以"与"操作   */
    UINT8 u8_vopss_src_vb_sel;                          /**< 选择VOPSS中FRC 的 src vb 来源，SS_VIPSS_PATH_SEL_E  */
    UINT8 u8_vipss_wdma_id_sel;                         /**< 输入视频通路wdma buf id sel，refer to SS_VOPSS_PATH_SEL_E    */
    UINT8 u8_vopss_dst_buf_id_sel;                      /**< VOPSS中FRC 的dst buf id 选择,refer to SS_VOPSS_PATH_SEL_E  */
    UINT8 u8_vipss_trigger_mode;                        /**< VIPSS trigger mode sesl,refer to SS_TRIGGER_MODE  */
    SS_VOPSS_TRIGGER_SET_T st_vopss_trigger_mode;       /**< VOPSS trigger mode sesl,refer to SS_VOPSS_TRIGGER_SET_T  */
}SS_DEV_T;


/**
* @brief        SS模块初始化
* @param [in]   pst_vrdma_dev :  related para:u8_ss_sel,refer to SS_CH_SEL_E;	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_instance_init(SS_DEV_T *pst_ss_dev,UINT8 u8_ss_sel);

/**
* @brief		vipss trigger
* @param [in]   pst_ss_dev : related para:1、u8_ch_sel; 2、u8_vipss_trigger_mode
* @note         触发SDN、WDwpackageTATE参数配置生效,通路可以通过“或”的方式同时配置多路     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vipss_trig(SS_DEV_T *pst_ss_dev);

/**
* @brief		vopss trigger mode control,只是配置触发方式，不触发
* @param [in]   pst_ss_dev : related para:1、u8_ch_sel; 2、st_ss_vopss_trigger_mode
* @note         触发VROTATE、VRDMA、DSC_DE、SU参数配置生效,通路可以通过“或”的方式同时配置多路     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_trig_mode_cfg(SS_DEV_T *pst_ss_dev);

/**
* @brief		vopss trigger
* @param [in]   pst_ss_dev : related para:1、u8_ch_sel; 2、st_ss_vopss_trigger_mode
* @note         触发VROTATE、VRDMA、DSC_DE、SU参数配置生效,通路可以通过“或”的方式同时配置多路     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_trig(SS_DEV_T *pst_ss_dev);

/**
* @brief		vopss trigger
* @param [in]   pst_ss_dev : related para: 1、st_ss_vopss_trigger_mode
* @note         同时trigger多条视频通路     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_simultaneous_trig(SS_DEV_T *pst_ss_dev);


/**
* @brief		VOPSS中的FRC src vb sel
* @param [in]   pst_ss_dev : related para:1、u8_ch_sel; 2、u8_vopss_src_vb_sel
* @note         通路可以通过“或”的方式同时配置多路      
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_src_vb_sel(SS_DEV_T *pst_ss_dev);

/**
* @brief		WDMA buffer id sel
* @param [in]   pst_ss_dev : related para:1、u8_ch_sel; 2、u8_vipss_wdma_id_sel
* @note         通路可以通过“或”的方式同时配置多路     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vipss_wdma_buf_sel(SS_DEV_T *pst_ss_dev);


/**
* @brief		VOPSS 中的FRC dst buf id sel
* @param [in]   pst_ss_dev : related para:1、u8_ch_sel; 2、u8_vopss_dst_buf_id_sel
* @note         用于FRC生成buff id，通路可以通过“或”的方式同时配置多路      
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_dst_buf_id_sel(SS_DEV_T *pst_ss_dev);

#endif // _SSH__

/**
 * @}
 */
