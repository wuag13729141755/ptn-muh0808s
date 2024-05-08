/**
* @file  api_split.h
* @brief SPLITģ��������ݽṹ����������
*/

/**
 * @defgroup API_SPLIT Split Module api
 *
 * @brief ���ڶ�������Ƶͼ�����Split����
 * @{
 */

#ifndef __API_SPLIT__
#define __API_SPLIT__

#include "ms_typedef.h"

/** split index�������ĸ�splitģ��*/
typedef enum _SPLIT_INDEX_E
{
    SPLIT_1 = 0,    /**< split1 */
    SPLIT_2,        /**< split2 */
    SPLIT_3,        /**< split3 */
    SPLIT_4,        /**< split4 */

    SPLIT_BUTT,
}SPLIT_INDEX_E;

/** splitλ�ýṹ��*/
typedef struct _SPLIT_POSITION_T
{
    UINT16 u16_horizontal_start;    /**<  H����split��ʼλ�ã���СΪ1 */
    UINT16 u16_horizontal_active;   /**<  H����split��Ч��� */
    UINT16 u16_vertical_start;      /**<  V����split��ʼλ�ã���СΪ1 */
    UINT16 u16_vertical_active;     /**<  V����split��Ч��� */
}SPLIT_POSITION_T;

/** splitģ�����ýṹ��*/
typedef struct _SPLIT_DEV_T
{
    UINT8 u8_split_idx;             /**<  split�����ţ����SPLIT_INDEX_E */
    BOOL b_enable;                  /**<  split���ʹ�� */
    SPLIT_POSITION_T st_split_pos;  /**<  splitλ�� */
}SPLIT_DEV_T;

/**
* @brief        SPLITģ��ṹ���ʼ��
* @param [in]   pst_split_dev : SPLIT DEV����ָ��
*               idx : split������
* @return       specific error types if call failed
* @note         ���Խṹ���ʼ������δ��ģ���ʼ������
*/
ERR_TYPE_E api_split_instance_init(SPLIT_DEV_T *pst_split_dev, UINT8 idx);

/**
* @brief        SPLITģ�鹤��ʹ��
* @param [in]   pst_split_dev : SPLIT DEV����ָ��
*               b_enable��0-ʧ�� ��0-ʹ��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_split_enable(SPLIT_DEV_T *pst_split_dev);
/**
* @brief        SPLITģ������
* @param [in]   pst_split_dev : SPLIT DEV����ָ��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_split_config(SPLIT_DEV_T *pst_split_dev);


#endif // _SPLIT_H__

/**
 * @}
 */


