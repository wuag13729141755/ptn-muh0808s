/**
* @file  su.h  
* @brief scale up 模块相关配置                                                           
*/

/**
 * @defgroup SU su api
 *
 * @brief 
 * @verbatim
 *  1）SU支持放大倍数1 ~ +∞
 *  2）SU模块输出的color space支持RGB、YUV444
 *     当输入为RGB时，输出必须选择RGB\n
 *     当输入为YUV444时，输出必须选择YUV444\n
 * @endverbatim
 * @{
 */

#ifndef __SU_H__
#define __SU_H__

#include "ms_typedef.h"

/** SU select*/
typedef enum _SU_SEL_E
{
    SU_SEL_SU1 = 0,                     /**<  选择SU1 */
    SU_SEL_SU2,                         /**<  选择SU2 */
    SU_SEL_SU3,                         /**<  选择SU3 */
    SU_SEL_SU4,                         /**<  选择SU4 */
    SU_SEL_SU5,                         /**<  选择SU5 */
    SU_SEL_SU6,                         /**<  选择SU6 */
    SU_SEL_SU7,                         /**<  选择SU7 */
    SU_SEL_SU8,                         /**<  选择SU8 */
    SU_NUM
}SU_SEL_E;

typedef enum _SU_PHASE_ORDER_SEL_E
{
    SU_PHASE_1 = 0,
    SU_PHASE_3
}SU_PHASE_ORDER_SEL_E;

/** 输出视频窗口 表示方法*/
typedef struct _VIDEO_WIN_T
{
    UINT16 u16_hde_st;                  /**<  水平方向起始地址 */
    UINT16 u16_hde_sp;                  /**<  水平方向结束地址 */
    UINT16 u16_vde_st;                  /**<  垂直方向起始地址 */
    UINT16 u16_vde_sp;                  /**<  垂直方向结束地址 */
} VIDEO_WIN_T;

/** 输出显示窗口 表示方法*/
typedef struct _DIS_WIN_T
{
    UINT8  u8_dis_win_en;
    UINT16 u16_hde_st;                  /**<  水平方向起始地址 */
    UINT16 u16_hde_sp;                  /**<  水平方向结束地址 */
    UINT16 u16_vde_st;                  /**<  垂直方向起始地址 */
    UINT16 u16_vde_sp;                  /**<  垂直方向结束地址 */
}DIS_WIN_T;

/** SU SIZE 表示方法*/
typedef struct _SU_SIZE_T
{
    UINT16 u16_h;                       /**<  水平方向pixel个数 */
    UINT16 u16_v;                       /**<  垂直方向line个数 */
} SU_SIZE_T;

/** SU 输入、输出size*/
typedef struct _SU_VIDEO_SIZE_T
{
    SU_SIZE_T st_MemRect;               /**<  SU输入SIZE */
    SU_SIZE_T st_OutRect;               /**<  SU输出SIZE */
} SU_VIDEO_SIZE_T;

/** SU Border Size*/
typedef struct _SU_BORDER_SIZE_T
{
    UINT16 u16_border_h;
    UINT16 u16_border_v;
} SU_BORDER_SIZE_T;

/** SU Border color*/
typedef struct _SU_BORDER_COLOR_T
{
    UINT8 u8YG;
    UINT8 u8UB;
    UINT8 u8VR;
} SU_BORDER_COLOR_T;

/** SU border 表示方法*/
typedef struct _SU_VIDEO_BORDER_T
{
    BOOL bd_enable;                         /**<  border enable */
    BOOL bd_cover;                          /**<  Border is cover the picture or outside of the picture */
    SU_BORDER_SIZE_T  border_size;          /**<  border size */
    SU_BORDER_COLOR_T border_color;         /**<  border color */
} SU_VIDEO_BORDER_T;


/** SU 模块结构体*/
typedef struct _SU_DEV_T
{
    UINT8 u8_sel;                           /**<  选择操作哪一路SU ,参考SU_SEL_E*/
    BOOL b_enable;                          /**<  SU的使能 */
    UINT8             u8_phase_order_sel;   /**<  SU 阶数选择 */
    UINT8             u8_line_buf_bypass_en;/**<  SU line buf bypass */
    DIS_WIN_T         st_dis_win;           /**<  设置视频显示窗口 */
    VIDEO_WIN_T       st_video_win;         /**<  设置视频窗口 */
    SU_VIDEO_SIZE_T   st_video_size;        /**<  设置SU模块、输入输出SIZE */
    UINT8             u8_su_color_space;   /**<  设置su color space,refer to HDMI_COLORSPACE_E */
    SU_VIDEO_BORDER_T st_video_border;      /**<  设置视频边框 */
}SU_DEV_T;

/**
* @brief        SU 模块的init
* @param [in]   pst_su_dev : related para:u8_su_sel refer to SU_SEL_E;
* @return       ERR_INV_PARM if input pointer is null
*/

ERR_TYPE_E api_su_instance_init(SU_DEV_T *pst_su_dev,UINT8 u8_su_sel);
/**
* @brief        SU 模块的使能和关闭
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、b_enable		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_enable_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块阶数选择，默认为1阶
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、u8_phase_order_sel	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_phase_order_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块设置输入、输出size参数
* @param [in]   pst_su_dev :: related para:1、u8_sel; 2、st_video_size		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_scaler_size_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块获取输入、输出size参数
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、st_video_size		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_scaler_size_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块设置video window 起始和结束位置
* @param [in]   pst_su_dev related para:1、u8_sel; 2、st_video_win		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_video_win_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块获取video window 起始和结束位置
* @param [in]   pst_su_dev related para:1、u8_sel; 2、st_video_win	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_video_win_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块设置display window 起始和结束位置,主要用于截取视频显示内容，一般应用无需设置
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、st_dis_win	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_display_win_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块获取display window 起始和结束位置
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、st_dis_win		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_display_win_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块CSC配置
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、u8_color_space		 
* @return       ERR_INV_PARM if input pointer is null
* @note         必须为RGB或者YUV444 Color space 
*/
ERR_TYPE_E api_su_csc_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块video border配置
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、st_video_border			 
* @return       ERR_INV_PARM if input pointer is null
* @note 
*/
ERR_TYPE_E api_su_video_border_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块video border 获取
* @param [in]   pst_su_dev : related para:1、u8_sel; 2、st_video_border			 
* @return       ERR_INV_PARM if input pointer is null
* @note 
*/
ERR_TYPE_E api_su_video_border_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU 模块Trigger
* @param [in]   pst_su_dev : related para:1、u8_sel;	 
* @return       ERR_INV_PARM if input pointer is null
* @note
*  当scale up系数发生变化或者输出window发生改变，trigger之后方能生效
*/
ERR_TYPE_E api_su_trigger(SU_DEV_T *pst_su_dev);

#endif // _SUH__

/**
 * @}
 */
 
 
