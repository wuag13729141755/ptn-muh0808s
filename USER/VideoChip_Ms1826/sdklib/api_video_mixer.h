/**
* @file  api_video_mixer.h
* @brief  ֧��8����Ƶ��ϣ�ÿ��8·��Ƶ��ѡ
*/

/**
 * @defgroup VIDEO_MIXER_API Video mixer module api
 *
 * @brief ֧��8����Ƶ��ϣ�ÿ��8·��Ƶ��ѡ
 * @{
 */
#ifndef __VIDEO_MIXER_API_H__
#define __VIDEO_MIXER_API_H__

#include "ms_typedef.h"

/** Video Mixer select*/
typedef  enum _VIDEO_MIXER_INDEX_E
{
    VIDEO_MIXER_1,   /**< ��ʾ��һ��Video Mixer */
    VIDEO_MIXER_2,   /**< ��ʾ�ڶ���Video Mixer */
    VIDEO_MIXER_3,   /**< ��ʾ������Video Mixer */
    VIDEO_MIXER_4,   /**< ��ʾ���ĸ�Video Mixer */
    VIDEO_MIXER_NUM, /**< ��ʾVideo Mixer����   */
}VIDEO_MIXER_INDEX_E;

/** 8���ź�Դ����*/
typedef enum _VIDEO_MIXER_SOURCE_E
{
    VOPSS1_DSC_SU, /**< ͨ·1��DSC��SU��� */
    VOPSS1_SU,     /**< ͨ·1��SU���      */
    VOPSS2_DSC_SU, /**< ͨ·2��DSC��SU��� */
    VOPSS2_SU,     /**< ͨ·2��SU���      */
    VOPSS3_DSC_SU, /**< ͨ·3��DSC��SU��� */
    VOPSS3_SU,     /**< ͨ·3��SU���      */
    VOPSS4_DSC_SU, /**< ͨ·4��DSC��SU��� */
    VOPSS4_SU,     /**< ͨ·4��SU���      */
}VIDEO_MIXER_SOURCE_E;

/** ÿlayer���ź�Դѡ��ṹ�壬ÿ��layer����ѡ��8���ź�Դ������һ�֣��ź�Դ�ο�VIDEO_MIXER_SOURCE_E*/
typedef struct _VIDEO_MIXER_LAYER_SEL_T
{
    UINT8 u8_layer0_src_sel;  /**< layer0ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer1_src_sel;  /**< layer1ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer2_src_sel;  /**< layer2ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer3_src_sel;  /**< layer3ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer4_src_sel;  /**< layer4ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer5_src_sel;  /**< layer5ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer6_src_sel;  /**< layer6ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
    UINT8 u8_layer7_src_sel;  /**< layer7ѡ����ź�Դ���ź�Դ�ο�VIDEO_MIXER_SOURCE_E */
}VIDEO_MIXER_LAYER_SEL_T;

/** ��ͬlayer��֮��alf���ڲ����ṹ��*/
typedef struct _VIDEO_MIXER_ALF_CONFIG_T
{
    UINT8  u8_VOPSS1_DSC_SU_alf_ratio;   /**< �����ź�Դ VOPSS1_DSC_SU ��alfֵ   */
    UINT8  u8_VOPSS1_SU_alf_ratio;       /**< �����ź�Դ VOPSS1_SU ��alfֵ       */
    UINT8  u8_VOPSS2_DSC_SU_alf_ratio;   /**< �����ź�Դ VOPSS2_DSC_SU ��alfֵ   */
    UINT8  u8_VOPSS2_SU_alf_ratio;       /**< �����ź�Դ VOPSS2_SU ��alfֵ       */
    UINT8  u8_VOPSS3_DSC_SU_alf_ratio;   /**< �����ź�Դ VOPSS3_DSC_SU ��alfֵ   */
    UINT8  u8_VOPSS3_SU_alf_ratio;       /**< �����ź�Դ VOPSS3_SU ��alfֵ       */
    UINT8  u8_VOPSS4_DSC_SU_alf_ratio;   /**< �����ź�Դ VOPSS4_DSC_SU ��alfֵ   */
    UINT8  u8_VOPSS4_SU_alf_ratio;       /**< �����ź�Դ VOPSS4_SU ��alfֵ       */
}VIDEO_MIXER_ALF_CONFIG_T;

/** bored���Խṹ��*/
typedef struct  _VIDEO_MIXER_BD_ATTR_T
{
    UINT16  u16_bd_lt_width;   /**< border��߱߿�Ŀ��  */
    UINT16  u16_bd_rt_width;   /**< border�ұ߱߿�Ŀ��  */
    UINT16  u16_bd_bot_height; /**< border�ײ��߿�Ŀ��  */
    UINT16  u16_bd_top_height; /**< border�����߿�Ŀ��  */
}VIDEO_MIXER_BD_ATTR_T;

/** bored & ��������ɫ���Խṹ��*/
typedef struct  _VIDEO_MIXER_COLOR_T
{
    UINT8   u8_yg;   /**< border�򱳾�����ɫY������G������ֵ   */
    UINT8   u8_ub;   /**< border�򱳾�����ɫU������B������ֵ   */
    UINT8   u8_vr;   /**< border�򱳾�����ɫV������R������ֵ   */
}VIDEO_MIXER_COLOR_T;

/** Video Mixer���ò����ṹ��*/
typedef struct _VIDEO_MIXER_DEV_T
{
    UINT8                        u8_video_mixer_idx;  /**< Video Mixer ģ���index                           */
    BOOL                         b_videomixer_en;     /**< ʹ��Video Mixer ģ��                              */
    VIDEO_MIXER_LAYER_SEL_T*     pst_layer_sel;       /**< LayerԴѡ��ṹ��ָ�룬�ο�VIDEO_MIXER_LAYER_SEL_T */
    VIDEO_MIXER_ALF_CONFIG_T*    pst_alf;             /**< alf���ڽṹ��ָ�룬�ο�VIDEO_MIXER_ALF_CONFIG_T    */
    VIDEO_MIXER_BD_ATTR_T*       pst_bd_attr;         /**< border���Խṹ��ָ�룬�ο�VIDEO_MIXER_BD_ATTR_T     */
    VIDEO_MIXER_COLOR_T*         pst_bd_color;        /**< border��ɫ�ṹ��ָ�룬�ο�VIDEO_MIXER_COLOR_T       */
    VIDEO_MIXER_COLOR_T*         pst_bg_color;        /**< ��������ɫ�ṹ��ָ�룬�ο�VIDEO_MIXER_COLOR_T       */
}VIDEO_MIXER_DEV_T;


/**
* @brief        video mixerģ���ʼ������
* @param [in]   pst_video_mixer : video mixerģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_video_mixer_instance_init(VIDEO_MIXER_DEV_T* pst_video_mixer,UINT8 u8_mix_sel);
/**
* @brief		Video mixer ʹ��
* @param [in]   pst_video_mixer : releated para: 1��u8_video_mixer_idx 2��b_videomixer_en
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_enable(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer layer���ú���
* @param [in]   pst_video_mixer : video mixer config����ָ��,releated para: 1��u8_video_mixer_idx 2��pst_layer_sel  3��pst_alf
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_layer_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer layer source���ú���,ֻ����layer ��source��������source��alfֵ��
* @param [in]   pst_video_mixer : video mixer config����ָ��,releated para: 1��u8_video_mixer_idx 2��pst_layer_sel
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_layer_only_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer layer source��alf���ú���,ֻ����source��alfֵ��������layer��source��
* @param [in]   pst_video_mixer : video mixer config����ָ��,releated para: 1��u8_video_mixer_idx 2��pst_alf
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_alf_only_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer �߿����ú���
* @param [in]   pst_video_mixer : video mixer config����ָ��,releated para: 1��u8_video_mixer_idx 2��pst_bd_attr  3��pst_bd_color
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_bd_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer ������ɫ���ú���
* @param [in]   pst_video_mixer : video mixer config����ָ��,releated para: 1��u8_video_mixer_idx 2��pst_bg_color
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_bg_color_config(VIDEO_MIXER_DEV_T* pst_video_mixer);

/**
* @brief		Video mixer ��ȡ��ǰlayer���Ӧ�ź�Դ����
* @param [in]   pst_video_mixer : video mixer config����ָ��,releated para: 1��u8_video_mixer_idx 2��pst_layer_sel
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_video_mixer_get_layer_source(VIDEO_MIXER_DEV_T* pst_video_mixer);

#endif
