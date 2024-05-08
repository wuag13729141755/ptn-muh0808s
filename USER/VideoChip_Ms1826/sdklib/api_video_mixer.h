/**
* @file  api_video_mixer.h
* @brief  支持8层视频混合，每层8路视频可选
*/

/**
 * @defgroup VIDEO_MIXER_API Video mixer module api
 *
 * @brief 支持8层视频混合，每层8路视频可选
 * @{
 */
#ifndef __VIDEO_MIXER_API_H__
#define __VIDEO_MIXER_API_H__

#include "ms_typedef.h"

/** Video Mixer select*/
typedef  enum _VIDEO_MIXER_INDEX_E
{
    VIDEO_MIXER_1,   /**< 表示第一个Video Mixer */
    VIDEO_MIXER_2,   /**< 表示第二个Video Mixer */
    VIDEO_MIXER_3,   /**< 表示第三个Video Mixer */
    VIDEO_MIXER_4,   /**< 表示第四个Video Mixer */
    VIDEO_MIXER_NUM, /**< 表示Video Mixer总数   */
}VIDEO_MIXER_INDEX_E;

/** 8种信号源类型*/
typedef enum _VIDEO_MIXER_SOURCE_E
{
    VOPSS1_DSC_SU, /**< 通路1经DSC和SU输出 */
    VOPSS1_SU,     /**< 通路1经SU输出      */
    VOPSS2_DSC_SU, /**< 通路2经DSC和SU输出 */
    VOPSS2_SU,     /**< 通路2经SU输出      */
    VOPSS3_DSC_SU, /**< 通路3经DSC和SU输出 */
    VOPSS3_SU,     /**< 通路3经SU输出      */
    VOPSS4_DSC_SU, /**< 通路4经DSC和SU输出 */
    VOPSS4_SU,     /**< 通路4经SU输出      */
}VIDEO_MIXER_SOURCE_E;

/** 每layer层信号源选择结构体，每层layer都已选择8种信号源的任意一种，信号源参考VIDEO_MIXER_SOURCE_E*/
typedef struct _VIDEO_MIXER_LAYER_SEL_T
{
    UINT8 u8_layer0_src_sel;  /**< layer0选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer1_src_sel;  /**< layer1选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer2_src_sel;  /**< layer2选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer3_src_sel;  /**< layer3选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer4_src_sel;  /**< layer4选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer5_src_sel;  /**< layer5选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer6_src_sel;  /**< layer6选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer7_src_sel;  /**< layer7选择的信号源，信号源参考VIDEO_MIXER_SOURCE_E */
}VIDEO_MIXER_LAYER_SEL_T;

/** 不同layer层之间alf调节参数结构体*/
typedef struct _VIDEO_MIXER_ALF_CONFIG_T
{
    UINT8  u8_VOPSS1_DSC_SU_alf_ratio;   /**< 调节信号源 VOPSS1_DSC_SU 的alf值   */
    UINT8  u8_VOPSS1_SU_alf_ratio;       /**< 调节信号源 VOPSS1_SU 的alf值       */
    UINT8  u8_VOPSS2_DSC_SU_alf_ratio;   /**< 调节信号源 VOPSS2_DSC_SU 的alf值   */
    UINT8  u8_VOPSS2_SU_alf_ratio;       /**< 调节信号源 VOPSS2_SU 的alf值       */
    UINT8  u8_VOPSS3_DSC_SU_alf_ratio;   /**< 调节信号源 VOPSS3_DSC_SU 的alf值   */
    UINT8  u8_VOPSS3_SU_alf_ratio;       /**< 调节信号源 VOPSS3_SU 的alf值       */
    UINT8  u8_VOPSS4_DSC_SU_alf_ratio;   /**< 调节信号源 VOPSS4_DSC_SU 的alf值   */
    UINT8  u8_VOPSS4_SU_alf_ratio;       /**< 调节信号源 VOPSS4_SU 的alf值       */
}VIDEO_MIXER_ALF_CONFIG_T;

/** bored属性结构体*/
typedef struct  _VIDEO_MIXER_BD_ATTR_T
{
    UINT16  u16_bd_lt_width;   /**< border左边边框的宽度  */
    UINT16  u16_bd_rt_width;   /**< border右边边框的宽度  */
    UINT16  u16_bd_bot_height; /**< border底部边框的宽度  */
    UINT16  u16_bd_top_height; /**< border顶部边框的宽度  */
}VIDEO_MIXER_BD_ATTR_T;

/** bored & 背景层颜色属性结构体*/
typedef struct  _VIDEO_MIXER_COLOR_T
{
    UINT8   u8_yg;   /**< border或背景层颜色Y分量或G分量的值   */
    UINT8   u8_ub;   /**< border或背景层颜色U分量或B分量的值   */
    UINT8   u8_vr;   /**< border或背景层颜色V分量或R分量的值   */
}VIDEO_MIXER_COLOR_T;

/** Video Mixer配置参数结构体*/
typedef struct _VIDEO_MIXER_DEV_T
{
    UINT8                        u8_video_mixer_idx;  /**< Video Mixer 模块的index                           */
    BOOL                         b_videomixer_en;     /**< 使能Video Mixer 模块                              */
    VIDEO_MIXER_LAYER_SEL_T*     pst_layer_sel;       /**< Layer源选择结构体指针，参考VIDEO_MIXER_LAYER_SEL_T */
    VIDEO_MIXER_ALF_CONFIG_T*    pst_alf;             /**< alf调节结构体指针，参考VIDEO_MIXER_ALF_CONFIG_T    */
    VIDEO_MIXER_BD_ATTR_T*       pst_bd_attr;         /**< border属性结构体指针，参考VIDEO_MIXER_BD_ATTR_T     */
    VIDEO_MIXER_COLOR_T*         pst_bd_color;        /**< border颜色结构体指针，参考VIDEO_MIXER_COLOR_T       */
    VIDEO_MIXER_COLOR_T*         pst_bg_color;        /**< 背景层颜色结构体指针，参考VIDEO_MIXER_COLOR_T       */
}VIDEO_MIXER_DEV_T;


/**
* @brief        video mixer模块初始化配置
* @param [in]   pst_video_mixer : video mixer模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_video_mixer_instance_init(VIDEO_MIXER_DEV_T* pst_video_mixer,UINT8 u8_mix_sel);
/**
* @brief		Video mixer 使能
* @param [in]   pst_video_mixer : releated para: 1、u8_video_mixer_idx 2、b_videomixer_en
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_enable(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer layer配置函数
* @param [in]   pst_video_mixer : video mixer config参数指针,releated para: 1、u8_video_mixer_idx 2、pst_layer_sel  3、pst_alf
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_layer_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer layer source配置函数,只配置layer 的source，不配置source的alf值。
* @param [in]   pst_video_mixer : video mixer config参数指针,releated para: 1、u8_video_mixer_idx 2、pst_layer_sel
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_layer_only_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer layer source的alf配置函数,只配置source的alf值，不配置layer的source。
* @param [in]   pst_video_mixer : video mixer config参数指针,releated para: 1、u8_video_mixer_idx 2、pst_alf
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_alf_only_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer 边框配置函数
* @param [in]   pst_video_mixer : video mixer config参数指针,releated para: 1、u8_video_mixer_idx 2、pst_bd_attr  3、pst_bd_color
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_bd_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer 背景颜色配置函数
* @param [in]   pst_video_mixer : video mixer config参数指针,releated para: 1、u8_video_mixer_idx 2、pst_bg_color
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_bg_color_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer 获取当前layer层对应信号源函数
* @param [in]   pst_video_mixer : video mixer config参数指针,releated para: 1、u8_video_mixer_idx 2、pst_layer_sel
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_get_layer_source(VIDEO_MIXER_DEV_T* pst_video_mixer);

#endif
