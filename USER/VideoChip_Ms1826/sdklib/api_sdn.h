/**
* @file  sdn.h  
* @brief scale down ģ���������                                                           
*/

/**
 * @defgroup SDN sdn api
 *
 * @brief 
 * @verbatim
 *  1��SDNģ��֧��1 ~ 1/256������С
 *  2��SDNģ�������color space֧��RGB��YUV444
 *      -������ΪRGBʱ���������ѡ��RGB
 *      -������ΪYUV444ʱ�����������YUV444
 * @endverbatim
 * @{
 */

#ifndef __SDN_H__
#define __SDN_H__

#include "ms_typedef.h"

/** SDN select*/
typedef enum _SDN_SEL_E
{
    SDN_SEL_SDN1 = 0,               /**<  ѡ��SDN1 */
    SDN_SEL_SDN2 = 1,               /**<  ѡ��SDN2 */
    SDN_SEL_SDN3 = 2,               /**<  ѡ��SDN3 */
    SDN_SEL_SDN4 = 3,               /**<  ѡ��SDN4 */
    SDN_NUM
}SDN_SEL_E;

/** SDN SIZE ��ʾ����*/
typedef struct _VIDEO_SIZE_T
{
    UINT16 u16_h;                   /**<  ˮƽ����pixel���� */
    UINT16 u16_v;                   /**<  ��ֱ����line���� */
} VIDEO_SIZE_T;

/** SDN ���롢���size*/
typedef struct _SDN_VIDEO_SIZE_T
{
    VIDEO_SIZE_T st_InRect;         /**<  SDN����SIZE */
    VIDEO_SIZE_T st_MemRect;        /**<  SDN���SIZE */
} SDN_VIDEO_SIZE_T;


/** SDN PEAKING����*/
typedef struct _PEAKING_PARAM_T
{
    BOOL b_enable;                  /**<  SDN peaking enable or not */
    UINT8 u8_nkl;                   /**<  �����������������Ƴ̶ȣ�ֵԽ����������Ч��Խ�ã���ͬʱҲ������ϸ�ڵ���ǿЧ��,��Χֵ0 ~ 15*/
    UINT8 u8_gain;                  /**<  more big more sharpness ����Χֵ[0 ~ 255]*/
}PEAKING_PARAM_T;

/** SDN ģ��ṹ��*/
typedef struct _SDN_DEV_T
{
    UINT8 u8_sel;                           /**<  ѡ�������һ·SDN ,�ο�SDN_SEL_E*/
    UINT8 b_enable;                         /**<  SDN��ʹ�� */
    SDN_VIDEO_SIZE_T st_video_size;         /**<  SDN���롢���size���� */
    UINT8            u8_sdn_color_space;   /**<  ����sdn color space,refer to HDMI_COLORSPACE_E */
    PEAKING_PARAM_T  st_peaking;            /**<  SDN peaking ���� */
}SDN_DEV_T;

/**
* @brief        SDN ģ��ĳ�ʼ��
* @param [in]   pst_sdn_dev : related para:1��SDN_DEV_T 2��u8_sdn_sel��refer to SDN_SEL_E;		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sdn_instance_init(SDN_DEV_T *pst_sdn_dev,UINT8 u8_sdn_sel);

/**
* @brief        SDN ģ���ʹ�ܺ͹ر�
* @param [in]   pst_sdn_dev : ָ��SDNģ���ָ��		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sdn_enable_set(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN ģ���������롢���size����
* @param [in]   pst_sdn_dev : related para:1��u8_sel; 2��st_video_size		
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_sdn_scaler_size_set(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN ģ���ȡ���롢���size����
* @param [in]   pst_sdn_dev : related para:1��u8_sel; 2��st_video_size		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sdn_scaler_size_get(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN ģ��CSC����
* @param [in]   pst_sdn_dev : related para:1��u8_sel; 2��u8_color_space		 
* @return       ERR_INV_PARM if input pointer is null
* @note         ����ΪRGB����YUV444 Color space 
*/
ERR_TYPE_E api_sdn_csc_set(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN ģ��PEAKING����
* @param [in]   pst_sdn_dev : related para:1��u8_sel; 2��st_peaking		 
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_sdn_csc_peaking_config(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN ģ���Ż�����,��ͼ�����ȱ�ٱ߽���Ϣʱ������ʵ��Ч�����Գ��Դ򿪻��߹رմ˹���
* @param [in]   pst_sdn_dev : related para:1��u8_sel; 2��b_enable		 
* @return       ERR_INV_PARM if input pointer is null
* @note         1����ʹ�ܸù���ʱ������SDNʱ����Ҫ�����size��ˮƽ����ֱ��ֵ����1(st_MemRect)
*               2��DSCͨ·����ʹ�ܸù���
*/
ERR_TYPE_E api_sdn_optimize_config(SDN_DEV_T *pst_sdn_dev);

/**
* @brief        SDN ģ��trigger
* @param [in]   pst_sdn_dev : related para:1��u8_sel; 		 
* @return       ERR_INV_PARM if input pointer is null
* @note
*  ����SDNģ��scalerϵ��֮�� trigger֮������Ч
*/
ERR_TYPE_E api_sdn_trigger(SDN_DEV_T *pst_sdn_dev);

#endif // _SDN_H__

/**
 * @}
 */
