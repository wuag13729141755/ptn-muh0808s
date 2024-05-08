/**
* @file  api_deint.h
* @brief ����I��ƵתP��Ƶ����
*/

/**
 * @defgroup DEINT_API Deinterlace module api
 *
 * @brief ����������Ƶ��I����P��ת��
 * @{
 */

#ifndef __DEINT_API_H__
#define __DEINT_API_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"


/** Deint config����*/
typedef struct _DEINT_DEV_T
{
    BOOL                    b_deint_en;            /**< ʹ��Deint��ʱ�Ӻ͸�λ                        */
    UINT16                  u16_vactive;           /**< The number of valid pixels in a column      */
    BOOL                    b_hd_full_mode;        /**< �Ƿ�1920*1080��ʽ���                        */
    BOOL                    b_yuv444;              /**< �����ʱ���Ƿ�ΪYUV444��ʽ                    */
    UINT32                  u32_buff0_start_addr;  /**< ����Deint����Ƶ������buff0�Ĵ洢��ʼ��ַ,��λ:Byte  */
    UINT32                  u32_buff1_start_addr;  /**< ����Deint����Ƶ������buff1�Ĵ洢��ʼ��ַ,ע��һ��buf��������ٿռ�Ϊfor 480i:720x240x2 byte; for 576i:720x288x2 byte;for 1080i half mode:960x540x2 byte;for 1080 full mode :1920x540x2 byte*/
}DEINT_DEV_T;


/**
* @brief        DEINTģ���ʼ������
* @param [in]   pst_deint_dev : DEINTģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_deint_instance_init(DEINT_DEV_T *pst_deint_dev);

/**
* @brief		Deinterlace int����
* @param [in]   pst_deint_dev : Deint config����ָ��,��� DEINT_DEV_T
* @note         ��Ҫ������ʹ��deint��ʱ��
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_deint_init(DEINT_DEV_T *pst_deint_dev);

/**
* @brief		Deinterlace config����
* @param [in]   pst_deint_dev : Deint config����ָ��,��� DEINT_DEV_T
* @note         ��Ҫ����������Deintģ������������Deint��������
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_deint_config(DEINT_DEV_T *pst_deint_dev);



#endif // _DEINT_API_H__



/**
 * @}
 */



