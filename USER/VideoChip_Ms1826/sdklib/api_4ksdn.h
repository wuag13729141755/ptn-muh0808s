/**
* @file  api_4ksdn.h  
* @brief 4KSDNģ��������ݽṹ����������                                                           
*/

/**
 * @defgroup API_SDN4K 4KSDN module api
 *
 * @brief ���ڶ�����4K��Ƶ����ScaleDown To 2k
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

/** 4ksdn����ģʽ*/
typedef enum _SDN4K_MODE_E
{
    SDN4K_MODE_BYPASS = 0,  /**<  BYPASS��Default��  */
    SDN4K_MODE_AUTO,        /**<  AUTOģʽ����������Ϊ4k@60hz_444/4k@60hz_420/4k422@30hz */
    SDN4K_MODE_MANUAL,      /**<  MANUALģʽ�����÷ֱ���С��4k@30hz/4k@60_420 */
    SDN4K_MODE_2Kx2K,       /**<  2Kx2Kģʽ���������⴦������ */

    SDN4K_MODE_BUTT,
}SDN4K_MODE_E;

/** 4ksdnģ��������Ϣ����*/
typedef struct _SDN4K_DEV_T
{
    UINT8 u8_sdn4k_idx;                         /**<  4KSDNģ�����������SDN4K_INDEX_E */
    UINT8 u8_sdn4k_mode;                        /**<  4KSDNģ�鹤��ģʽ�����SDN4K_MODE_E���� */
    UINT8 u8_input_colorSpace;                  /**<  ������ɫ�ռ��ʽ�����HDMI_COLORSPACE_E���� */ 
    HDMI_VIDEO_TIMING_T st_input_videoTiming;   /**<  ����Timing���� */
    UINT8 u8_output_colorSpace;                 /**<  ���ɫ�ʿռ��ʽ���������ã����HDMI_COLORSPACE_E���� */
    HDMI_VIDEO_TIMING_T st_output_videoTiming;  /**<  ���Timing�������������� */
}SDN4K_DEV_T;

/** 4KSDN TIMING���ò��� */
typedef struct _SDN4K_TIMEING_PARAM_T
{
    UINT16 u16_hsync_width;     /**<  h_blank��� */
    UINT16 u16_hblank_posedge;  /**<  h_blank������λ�� */
    UINT16 u16_hblank_negedge;  /**<  h_blank�½���λ�� */
}SDN4K_TIMEING_PARAM_T;

/**
* @brief        4KSDNģ��ṹ���ʼ��
* @param [in]   pst_sdn4k_dev : 4KSDN����ָ�� 
*               idx : ģ��������
* @return       specific error types if call failed
* @note         ���Խṹ���ʼ������δ��ģ���ʼ������
*/
ERR_TYPE_E api_4ksdn_instance_init(SDN4K_DEV_T *pst_sdn4k_dev, UINT8 idx);

/**
* @brief        4KSDNģ���ʼ��
* @param [in]   pst_sdn4k_dev : 4KSDN DEV����ָ�� 
* @return       specific error types if call failed
* @note         ��ʼ����Ĭ��ΪBYPASSģʽ        
*/
ERR_TYPE_E api_sdn4k_init(SDN4K_DEV_T *pst_sdn4k_dev);
/**
* @brief        4KSDNģ��ʹ��
* @param [in]   pst_sdn4k_dev : 4KSDN DEV����ָ�� 
*               b_enable : �Ƿ�ʹ��
* @return       specific error types if call failed
* @note                 
*/
ERR_TYPE_E api_sdn4k_enable(SDN4K_DEV_T *pst_sdn4k_dev, BOOL b_enable);
/**
* @brief        4KSDN ���ú���
* @param [in]   pst_sdn4k_dev : 4KSDN DEV����ָ��
* @return       specific error types if call failed
* @note                    
*/
ERR_TYPE_E api_sdn4k_config(SDN4K_DEV_T *pst_sdn4k_dev);

#endif // _4KSDN_H__

/**
 * @}
 */


