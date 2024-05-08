/**
* @file  api_vehc.h
* @brief 视频亮度、对比度、饱和度、色度调节
*/

/**
 * @defgroup VEHC_API VEHC module api
 *
 * @brief 视频亮度、对比度、饱和度、色度调节
 * @{
 */
#ifndef __VEHC_API_H__
#define __VEHC_API_H__

#include "ms_typedef.h"
 /** VEHC select*/
typedef enum _VEHC_INDEX_E
{
	VEHC_VOPSS_1 = 0,     /**<  表示视频后端1的VEHC*/
	VEHC_VOPSS_2 = 1,     /**<  表示视频后端2的VEHC*/
	VEHC_VOPSS_3 = 2,     /**<  表示视频后端3的VEHC*/
	VEHC_VOPSS_4 = 3,     /**<  表示视频后端4的VEHC*/
	VEHC_VIPSS_1 = 4,     /**<  表示视频前端1的VEHC*/
	VEHC_VIPSS_2 = 5,     /**<  表示视频前端2的VEHC*/
	VEHC_VIPSS_3 = 6,     /**<  表示视频前端3的VEHC*/
	VEHC_VIPSS_4 = 7,     /**<  表示视频前端4的VEHC*/
	VEHC_NUM = 8,         /**<  表示VEHC的总数量   */
}VEHC_INDEX_E;

/** 亮度、对比度、饱和度、色度调节使能*/
typedef enum _VEHC_BCSH_E
{
	BCSH_BYPASS = 0X00,   /**<  不调节BCSH值   */
	BRI_EN      = 0X01,   /**<  调节brightness */
	CON_EN      = 0X02,   /**<  调节contrast   */
	SAT_EN      = 0X04,   /**<  调节saturation */
	HUE_EN      = 0X08,   /**<  调节hue        */
}VEHC_BCSH_E;

/** 亮度、对比度、饱和度、色度调节参数*/
typedef struct _VEHC_BCSH_T
{
    UINT8 u8_bcsh_en_flag;   /**<  参考VEHC_BCSH_E，枚举元素间可以进行与操作 */
    INT8  s8_bcsh_bri;       /**<  需要调节亮度的值   (-127~127)            */
    UINT8 u8_bcsh_con;       /**<  需要调节对比度的值(0~255)                 */
    UINT8 u8_bcsh_sat;       /**<  需要调节饱和度的值(0~255)                 */
    UINT8 u8_bcsh_hue;       /**<  需要调节色度的值(0~255)                  */
}VEHC_BCSH_T;

/** Peaking 调节参数*/
typedef struct _VEHC_PEAKING_T
{
	BOOL   b_pk_en;      /**<  ME_TRUE表示打开Peaking功能，MS_FLASE表示关闭                */
	UINT8  u8_pk_gain;   /**<  细节增强，值越大细节增加效果越明显(0~255)                                */
	UINT8  u8_pk_nkl;    /**<  设置噪声的抑制程度，值越大，噪声抑制效果越好，同时会消弱细节增强的效果*/
}VEHC_PEAKING_T;

/** DYN 调节参数*/
typedef struct _VEHC_DYN_T
{
	BOOL    b_en;
	UINT8   u8_dyn_vr_gain;     /**< 设置V/R通道的gain值  */
	UINT8   u8_dyn_ub_gain;     /**< 设置U/B通道的gain值  */
	UINT8   u8_dyn_yg_gain;     /**< 设置Y/G通道的gain值  */
	INT16  s16_dyn_vr_offset;   /**< 设置V/R通道的offset值*/
	INT16  s16_dyn_ub_offset;   /**< 设置U/B通道的offset值*/
	INT16  s16_dyn_yg_offset;   /**< 设置Y/G通道的offset值*/
}VEHC_DYN_T;

/** test pattern 的种类*/
typedef enum _VEHC_MUTE_PATTERN_E
{
	VEHC_MUTE_PATTERN_PURE_BLACK = 0,             /**< 纯黑色图            */
	VEHC_MUTE_PATTERN_PURE_BLUE = 1,              /**< 纯蓝色图            */
	VEHC_MUTE_PATTERN_PURE_GREEN = 2,             /**< 纯绿色图            */
	VEHC_MUTE_PATTERN_PURE_RED = 3,               /**< 纯红色图            */
	VEHC_MUTE_PATTERN_PURE_WHITE = 4,             /**< 纯白色图            */
	VEHC_MUTE_PATTERN_HOR_RAMP = 6,               /**< 水平变化的灰阶图        */
	VEHC_MUTE_PATTERN_COLOR_BAR = 8,              /**< 彩色条             */
	VEHC_MUTE_PATTERN_HOR_GRAY_SCALE = 9,         /**< 水平条形灰阶图         */
	VEHC_MUTE_PATTERN_INIERLACE_BLACK = 13,       /**< 逐行黑色图           */
	VEHC_MUTE_PATTERN_INTERLACE_RED = 14,         /**< 逐行红色图           */
	VEHC_MUTE_PATTERN_INTERLACE_GREEN = 15,       /**< 逐行绿色图           */
	VEHC_MUTE_PATTERN_INTERLACE_BLUE = 16         /**< 逐行蓝色图           */
}VEHC_MUTE_PATTERN_E;

/** Test pattern 的配置参数*/
typedef struct _VEHC_TES_PATTERN_T
{
	BOOL b_test_pattern_en;                   /**< ME_TRUE表示使能输出test pattern，MS_FLASE表示关闭 */
	UINT8 u8_pattern_sel;                     /**< 参考VEHC_MUTE_PATTERN_E                             */
}VEHC_TEST_PATTERN_T;

/** VEHC 模块的config参数*/
typedef struct _VEHC_DEV_T
{
	UINT8                 u8_vehc_idx;       /**< VEHC模块的index         */
	BOOL                  b_yuv;             /**< 输入是否为YUV格式       */
	VEHC_BCSH_T           st_bcsh;           /**< 参考VEHC_BCSH_T         */
	VEHC_PEAKING_T        st_peaking;        /**< 参考VEHC_PEAKING_T      */
	VEHC_TEST_PATTERN_T   st_pattern;        /**< 参考VEHC_TEST_PATTERN_T  */
	//VEHC_DYN_T            st_dyn;            /**< 调节DYN参数，@ref VEHC_DYN_T*/
}VEHC_DEV_T;

/**
* @brief        VEHC模块初始化配置
* @param [in]   pst_vehc : VEHC模块对象
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_vehc_instance_init(VEHC_DEV_T* pst_vehc, UINT8 u8_vehc_idx);

/**
* @brief		VEHC 亮度、对比度、饱和度、色度调节配置函数
* @param [in]   pst_tmg_dev : vehc config参数指针
* @note         输入为YUV444信号可进行亮度、对比度、饱和度、色度调节，输入为RGB信号可进行亮度和对比度调节
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_bcsh_config(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC DYN 配置函数
* @param [in]   pst_tmg_dev : vehc config参数指针，vehc dyn config参数指针
* @note         输入gain与offest值进行Dynamic range调节
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_dyn_config(VEHC_DEV_T* pst_vehc, VEHC_DYN_T* pst_vehc_dyn);


/**
* @brief		获取当前亮度、对比度、饱和度、色度函数
* @param [in]   pst_tmg_dev : vehc config参数指针，DYN 调节参数
* @note         输入为YUV444信号获取亮度、对比度、饱和度、色度值，输入为RGB信号获取亮度、对比度值
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_get_bcsh(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC Peaking 配置函数
* @param [in]   pst_tmg_dev : vehc config参数指针
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_peaking_config(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC Peaking 获取函数
* @param [in]   pst_tmg_dev : vehc config参数指针
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_get_peaking(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC 输出pattern 配置函数
* @param [in]   pst_tmg_dev : vehc config参数指针
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_pattern_config(VEHC_DEV_T* pst_vehc);

#endif
