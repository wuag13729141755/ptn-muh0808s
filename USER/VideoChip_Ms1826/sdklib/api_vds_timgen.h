/**
* @file  api_vds_timgen.h
* @brief ���ʱ������ģ�����ݼ���������
*/

/**
 * @defgroup VDS_TIMGEN_API Timgen module api
 *
 * @brief �������������Timing
 * @{
 */
#ifndef __VDS_TIMGEN_API_H__
#define __VDS_TIMGEN_API_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"
#include "ms1826_timing_table.h"

/** TIMGEN index��һ����4��Timgen*/
typedef enum _TIMGEN_INDEX_E
{
    TIMGEN_1,      /**<  Timgen 1,��ʾ��һ��Timgen */
    TIMGEN_2,      /**<  Timgen 2,��ʾ�ڶ���Timgen */
    TIMGEN_3,      /**<  Timgen 3,��ʾ������Timgen */
    TIMGEN_4,      /**<  Timgen 4,��ʾ���ĸ�Timgen */
    TIMGEN_NUM,    /**<  ��ʾTimgen������          */
}TIMDEN_INDEX_E;

/** Timgen�Ĺ���ģʽ*/
typedef enum _TIMGEN_WORK_MODE_E
{
	FreeRun_Mode,                /**< �û��Զ������Tʱ�ӡ���Ƶ��Ƶ��ʱ����Ϣ                                         */
	Casecade_Mode,               /**< оƬ������ʽ */
}TIMGEN_WORK_MODE_E;

/** ����ʱ��ļ����б�*/
typedef enum _TIMGEN_INPUT_SYNC_POLARITY_E
{
    VPosHPos,          /**< V����������H�������� */
    VPosHNeg,          /**< V����������H���򸺼� */
    VNegHPos,          /**< V���򸺼���H�������� */
    VNegHNeg           /**< V���򸺼���H���򸺼� */
} TIMGEN_INPUT_SYNC_POLARITY_E;/** Timgen config����*/
typedef struct _VDS_TMG_DEV_T
{
    UINT8		 		      	    u8_timgen_id;        /**< ѡ��ͬ��Timgen                                                    */
    BOOL                            b_timgen_en;        /**< ʹ��Timgen                                                          */
    UINT8				            u8_workmode;         /**< Timgen ����timing�Ĺ���ģʽѡ�񣬲ο� TIMGEN_WORK_MODE_E              */
    HDMI_VIDEO_TIMING_T* 		    pst_timing;          /**< ����Timgen��� timing�Ĳ���                                          */
} VDS_TMG_DEV_T;


/**
* @brief        Timgenģ���ʼ������
* @param [in]   pst_tmg_dev : Timgenģ�����ָ��
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_timgen_instance_init(VDS_TMG_DEV_T *pst_tmg_dev,UINT8 u8_timgen_index);
/**
* @brief		Timgen ��ʼ��
* @param [in]   pst_tmg_dev : Timgen config����ָ��
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vds_tmg_init(VDS_TMG_DEV_T *pst_tmg_dev);

/**
* @brief		Timgen ���ú���
* @param [in]   pst_tmg_dev : Timgen config����ָ��
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vds_tmg_config(VDS_TMG_DEV_T *pst_tmg_dev);




#endif // _VDS_TIMGEN_API_H__

/**
 * @}
 */
