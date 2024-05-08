/**
* @file  api_csc.h
* @brief cscģ��������ݽṹ����������
*/

/**
 * @defgroup API_CSC CSC module api
 *
 * @brief ��ɫ�ռ�ת��
 * @{
 */

#ifndef __API_CSC_H__
#define __API_CSC_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"

/** csc ģ��ѡ��*/
typedef enum _CSC_SEL_E
{
    CSC_SEL_1 = 0,
    CSC_SEL_2 ,
    CSC_SEL_3 ,
    CSC_SEL_4 ,
    CSC_SEL_NUM
}CSC_SEL_E;

/** csc color rangeѡ��*/
typedef enum CSC_COLOR_RANGE
{
    CSC_RGB_16_235 = 0,
    CSC_RGB_0_255,
    CSC_YUV444_BT601,
    CSC_YUV444_BT709,
} CSC_COLOR_RANGE_E;

/** csc ��������*/
typedef struct _CSC_CONFIG_PARAM_T
{
    UINT8  csc_in_colorspace;           /**< csc �������ɫ�ռ� @ref CSC_HDMI_COLOR_SPACE_E*/
    UINT8  csc_out_colorspace;          /**< csc �������ɫ�ռ� @ref CSC_HDMI_COLOR_SPACE_E*/
    UINT8  csc_in_color_range;          /**< csc �����ɫ��Χ @ref CSC_COLOR_RANGE_E*/
    UINT8  csc_out_color_range;         /**< csc �����ɫ��Χ @ref CSC_COLOR_RANGE_E*/
    UINT8  u8_pixel_repeat;             /**< csc �������Ƶrepeater������ֻ��tx csc��Ҫ���ã�rx csc����Ҫ���ã�
                                             ��txѡ��video mixer���ʱ����Ҫ����u8_pixel_repeat=0������pixel repeat��
                                             ��txѡ��rx1/2/3/4 videoʱ����Ҫ����u8_pixel_repeat�Ͷ�Ӧ��rx�������pixel repeatһ�¡�
                                             @ref HDMI_PIXEL_REPEAT_E*/
} CSC_CONFIG_PARAM_T;

/** csc ģ��ṹ��*/
typedef struct _CSC_DEV_T
{
    UINT8 u8_sel;                                 /**< csc index @ref CSC_SEL_E*/
    CSC_CONFIG_PARAM_T st_csc_config_param;       /**< csc config param @ref CSC_CONFIG_PARAM_T*/
}CSC_DEV_T;

/**
* @brief        csc rxģ�����ʵ����ʼ��
* @param [in]   pst_csc_dev : cscģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_rx_instance_init(CSC_DEV_T *pst_csc_dev, UINT8 u8_rx_sel);

/**
* @brief        csc txģ�����ʵ����ʼ��
* @param [in]   pst_csc_dev : cscģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_instance_init(CSC_DEV_T *pst_csc_dev, UINT8 u8_tx_sel);

/**
* @brief        csc rxģ������
* @param [in]   pst_csc_dev : cscģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if successed
*/
ERR_TYPE_E api_csc_rx_set(CSC_DEV_T *pst_csc_dev);

/**
* @brief        csc txģ������
* @param [in]   pst_csc_dev : cscģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_set(CSC_DEV_T *pst_csc_dev);

/**
* @brief        csc rx yuv422��yuv444ת�������˲�����bypass
* @param [in]   pst_csc_dev : cscģ�����ָ��  related para:u8_sel
*               b_enable : TRUE ��ʾbypass FALSE ��ʾ����filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_rx_yuv422_to_yuv444_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

/**
* @brief        csc rx yuv444��yuv422ת���������˲�����bypass
* @param [in]   pst_csc_dev : cscģ�����ָ��  related para:u8_sel
*               b_enable : TRUE ��ʾbypass FALSE ��ʾ����filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_rx_yuv444_to_yuv422_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

/**
* @brief        csc tx yuv422��yuv444ת�������˲�����bypass
* @param [in]   pst_csc_dev : cscģ�����ָ��  related para:u8_sel
*               b_enable : TRUE ��ʾbypass FALSE ��ʾ����filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_yuv422_to_yuv444_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

/**
* @brief        csc tx yuv444��yuv422ת���������˲�����bypass
* @param [in]   pst_csc_dev : cscģ�����ָ��  related para:u8_sel
*               b_enable : TRUE ��ʾbypass FALSE ��ʾ����filter
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_csc_tx_yuv444_to_yuv422_filter_byps(CSC_DEV_T *pst_csc_dev,BOOL b_enable);

#endif

/**
 * @}
 */
