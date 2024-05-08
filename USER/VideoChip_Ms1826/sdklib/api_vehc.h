/**
* @file  api_vehc.h
* @brief ��Ƶ���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ���
*/

/**
 * @defgroup VEHC_API VEHC module api
 *
 * @brief ��Ƶ���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ���
 * @{
 */
#ifndef __VEHC_API_H__
#define __VEHC_API_H__

#include "ms_typedef.h"
 /** VEHC select*/
typedef enum _VEHC_INDEX_E
{
	VEHC_VOPSS_1 = 0,     /**<  ��ʾ��Ƶ���1��VEHC*/
	VEHC_VOPSS_2 = 1,     /**<  ��ʾ��Ƶ���2��VEHC*/
	VEHC_VOPSS_3 = 2,     /**<  ��ʾ��Ƶ���3��VEHC*/
	VEHC_VOPSS_4 = 3,     /**<  ��ʾ��Ƶ���4��VEHC*/
	VEHC_VIPSS_1 = 4,     /**<  ��ʾ��Ƶǰ��1��VEHC*/
	VEHC_VIPSS_2 = 5,     /**<  ��ʾ��Ƶǰ��2��VEHC*/
	VEHC_VIPSS_3 = 6,     /**<  ��ʾ��Ƶǰ��3��VEHC*/
	VEHC_VIPSS_4 = 7,     /**<  ��ʾ��Ƶǰ��4��VEHC*/
	VEHC_NUM = 8,         /**<  ��ʾVEHC��������   */
}VEHC_INDEX_E;

/** ���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ���ʹ��*/
typedef enum _VEHC_BCSH_E
{
	BCSH_BYPASS = 0X00,   /**<  ������BCSHֵ   */
	BRI_EN      = 0X01,   /**<  ����brightness */
	CON_EN      = 0X02,   /**<  ����contrast   */
	SAT_EN      = 0X04,   /**<  ����saturation */
	HUE_EN      = 0X08,   /**<  ����hue        */
}VEHC_BCSH_E;

/** ���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ��ڲ���*/
typedef struct _VEHC_BCSH_T
{
    UINT8 u8_bcsh_en_flag;   /**<  �ο�VEHC_BCSH_E��ö��Ԫ�ؼ���Խ�������� */
    INT8  s8_bcsh_bri;       /**<  ��Ҫ�������ȵ�ֵ   (-127~127)            */
    UINT8 u8_bcsh_con;       /**<  ��Ҫ���ڶԱȶȵ�ֵ(0~255)                 */
    UINT8 u8_bcsh_sat;       /**<  ��Ҫ���ڱ��Ͷȵ�ֵ(0~255)                 */
    UINT8 u8_bcsh_hue;       /**<  ��Ҫ����ɫ�ȵ�ֵ(0~255)                  */
}VEHC_BCSH_T;

/** Peaking ���ڲ���*/
typedef struct _VEHC_PEAKING_T
{
	BOOL   b_pk_en;      /**<  ME_TRUE��ʾ��Peaking���ܣ�MS_FLASE��ʾ�ر�                */
	UINT8  u8_pk_gain;   /**<  ϸ����ǿ��ֵԽ��ϸ������Ч��Խ����(0~255)                                */
	UINT8  u8_pk_nkl;    /**<  �������������Ƴ̶ȣ�ֵԽ����������Ч��Խ�ã�ͬʱ������ϸ����ǿ��Ч��*/
}VEHC_PEAKING_T;

/** DYN ���ڲ���*/
typedef struct _VEHC_DYN_T
{
	BOOL    b_en;
	UINT8   u8_dyn_vr_gain;     /**< ����V/Rͨ����gainֵ  */
	UINT8   u8_dyn_ub_gain;     /**< ����U/Bͨ����gainֵ  */
	UINT8   u8_dyn_yg_gain;     /**< ����Y/Gͨ����gainֵ  */
	INT16  s16_dyn_vr_offset;   /**< ����V/Rͨ����offsetֵ*/
	INT16  s16_dyn_ub_offset;   /**< ����U/Bͨ����offsetֵ*/
	INT16  s16_dyn_yg_offset;   /**< ����Y/Gͨ����offsetֵ*/
}VEHC_DYN_T;

/** test pattern ������*/
typedef enum _VEHC_MUTE_PATTERN_E
{
	VEHC_MUTE_PATTERN_PURE_BLACK = 0,             /**< ����ɫͼ            */
	VEHC_MUTE_PATTERN_PURE_BLUE = 1,              /**< ����ɫͼ            */
	VEHC_MUTE_PATTERN_PURE_GREEN = 2,             /**< ����ɫͼ            */
	VEHC_MUTE_PATTERN_PURE_RED = 3,               /**< ����ɫͼ            */
	VEHC_MUTE_PATTERN_PURE_WHITE = 4,             /**< ����ɫͼ            */
	VEHC_MUTE_PATTERN_HOR_RAMP = 6,               /**< ˮƽ�仯�Ļҽ�ͼ        */
	VEHC_MUTE_PATTERN_COLOR_BAR = 8,              /**< ��ɫ��             */
	VEHC_MUTE_PATTERN_HOR_GRAY_SCALE = 9,         /**< ˮƽ���λҽ�ͼ         */
	VEHC_MUTE_PATTERN_INIERLACE_BLACK = 13,       /**< ���к�ɫͼ           */
	VEHC_MUTE_PATTERN_INTERLACE_RED = 14,         /**< ���к�ɫͼ           */
	VEHC_MUTE_PATTERN_INTERLACE_GREEN = 15,       /**< ������ɫͼ           */
	VEHC_MUTE_PATTERN_INTERLACE_BLUE = 16         /**< ������ɫͼ           */
}VEHC_MUTE_PATTERN_E;

/** Test pattern �����ò���*/
typedef struct _VEHC_TES_PATTERN_T
{
	BOOL b_test_pattern_en;                   /**< ME_TRUE��ʾʹ�����test pattern��MS_FLASE��ʾ�ر� */
	UINT8 u8_pattern_sel;                     /**< �ο�VEHC_MUTE_PATTERN_E                             */
}VEHC_TEST_PATTERN_T;

/** VEHC ģ���config����*/
typedef struct _VEHC_DEV_T
{
	UINT8                 u8_vehc_idx;       /**< VEHCģ���index         */
	BOOL                  b_yuv;             /**< �����Ƿ�ΪYUV��ʽ       */
	VEHC_BCSH_T           st_bcsh;           /**< �ο�VEHC_BCSH_T         */
	VEHC_PEAKING_T        st_peaking;        /**< �ο�VEHC_PEAKING_T      */
	VEHC_TEST_PATTERN_T   st_pattern;        /**< �ο�VEHC_TEST_PATTERN_T  */
	//VEHC_DYN_T            st_dyn;            /**< ����DYN������@ref VEHC_DYN_T*/
}VEHC_DEV_T;

/**
* @brief        VEHCģ���ʼ������
* @param [in]   pst_vehc : VEHCģ�����
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_vehc_instance_init(VEHC_DEV_T* pst_vehc, UINT8 u8_vehc_idx);

/**
* @brief		VEHC ���ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ������ú���
* @param [in]   pst_tmg_dev : vehc config����ָ��
* @note         ����ΪYUV444�źſɽ������ȡ��Աȶȡ����Ͷȡ�ɫ�ȵ��ڣ�����ΪRGB�źſɽ������ȺͶԱȶȵ���
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_bcsh_config(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC DYN ���ú���
* @param [in]   pst_tmg_dev : vehc config����ָ�룬vehc dyn config����ָ��
* @note         ����gain��offestֵ����Dynamic range����
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_dyn_config(VEHC_DEV_T* pst_vehc, VEHC_DYN_T* pst_vehc_dyn);


/**
* @brief		��ȡ��ǰ���ȡ��Աȶȡ����Ͷȡ�ɫ�Ⱥ���
* @param [in]   pst_tmg_dev : vehc config����ָ�룬DYN ���ڲ���
* @note         ����ΪYUV444�źŻ�ȡ���ȡ��Աȶȡ����Ͷȡ�ɫ��ֵ������ΪRGB�źŻ�ȡ���ȡ��Աȶ�ֵ
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_get_bcsh(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC Peaking ���ú���
* @param [in]   pst_tmg_dev : vehc config����ָ��
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_peaking_config(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC Peaking ��ȡ����
* @param [in]   pst_tmg_dev : vehc config����ָ��
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_get_peaking(VEHC_DEV_T* pst_vehc);

/**
* @brief		VEHC ���pattern ���ú���
* @param [in]   pst_tmg_dev : vehc config����ָ��
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vehc_pattern_config(VEHC_DEV_T* pst_vehc);

#endif
