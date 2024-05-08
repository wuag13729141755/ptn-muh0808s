/**
* @file  su.h  
* @brief scale up ģ���������                                                           
*/

/**
 * @defgroup SU su api
 *
 * @brief 
 * @verbatim
 *  1��SU֧�ַŴ���1 ~ +��
 *  2��SUģ�������color space֧��RGB��YUV444
 *     ������ΪRGBʱ���������ѡ��RGB\n
 *     ������ΪYUV444ʱ���������ѡ��YUV444\n
 * @endverbatim
 * @{
 */

#ifndef __SU_H__
#define __SU_H__

#include "ms_typedef.h"

/** SU select*/
typedef enum _SU_SEL_E
{
    SU_SEL_SU1 = 0,                     /**<  ѡ��SU1 */
    SU_SEL_SU2,                         /**<  ѡ��SU2 */
    SU_SEL_SU3,                         /**<  ѡ��SU3 */
    SU_SEL_SU4,                         /**<  ѡ��SU4 */
    SU_SEL_SU5,                         /**<  ѡ��SU5 */
    SU_SEL_SU6,                         /**<  ѡ��SU6 */
    SU_SEL_SU7,                         /**<  ѡ��SU7 */
    SU_SEL_SU8,                         /**<  ѡ��SU8 */
    SU_NUM
}SU_SEL_E;

typedef enum _SU_PHASE_ORDER_SEL_E
{
    SU_PHASE_1 = 0,
    SU_PHASE_3
}SU_PHASE_ORDER_SEL_E;

/** �����Ƶ���� ��ʾ����*/
typedef struct _VIDEO_WIN_T
{
    UINT16 u16_hde_st;                  /**<  ˮƽ������ʼ��ַ */
    UINT16 u16_hde_sp;                  /**<  ˮƽ���������ַ */
    UINT16 u16_vde_st;                  /**<  ��ֱ������ʼ��ַ */
    UINT16 u16_vde_sp;                  /**<  ��ֱ���������ַ */
} VIDEO_WIN_T;

/** �����ʾ���� ��ʾ����*/
typedef struct _DIS_WIN_T
{
    UINT8  u8_dis_win_en;
    UINT16 u16_hde_st;                  /**<  ˮƽ������ʼ��ַ */
    UINT16 u16_hde_sp;                  /**<  ˮƽ���������ַ */
    UINT16 u16_vde_st;                  /**<  ��ֱ������ʼ��ַ */
    UINT16 u16_vde_sp;                  /**<  ��ֱ���������ַ */
}DIS_WIN_T;

/** SU SIZE ��ʾ����*/
typedef struct _SU_SIZE_T
{
    UINT16 u16_h;                       /**<  ˮƽ����pixel���� */
    UINT16 u16_v;                       /**<  ��ֱ����line���� */
} SU_SIZE_T;

/** SU ���롢���size*/
typedef struct _SU_VIDEO_SIZE_T
{
    SU_SIZE_T st_MemRect;               /**<  SU����SIZE */
    SU_SIZE_T st_OutRect;               /**<  SU���SIZE */
} SU_VIDEO_SIZE_T;

/** SU Border Size*/
typedef struct _SU_BORDER_SIZE_T
{
    UINT16 u16_border_h;
    UINT16 u16_border_v;
} SU_BORDER_SIZE_T;

/** SU Border color*/
typedef struct _SU_BORDER_COLOR_T
{
    UINT8 u8YG;
    UINT8 u8UB;
    UINT8 u8VR;
} SU_BORDER_COLOR_T;

/** SU border ��ʾ����*/
typedef struct _SU_VIDEO_BORDER_T
{
    BOOL bd_enable;                         /**<  border enable */
    BOOL bd_cover;                          /**<  Border is cover the picture or outside of the picture */
    SU_BORDER_SIZE_T  border_size;          /**<  border size */
    SU_BORDER_COLOR_T border_color;         /**<  border color */
} SU_VIDEO_BORDER_T;


/** SU ģ��ṹ��*/
typedef struct _SU_DEV_T
{
    UINT8 u8_sel;                           /**<  ѡ�������һ·SU ,�ο�SU_SEL_E*/
    BOOL b_enable;                          /**<  SU��ʹ�� */
    UINT8             u8_phase_order_sel;   /**<  SU ����ѡ�� */
    UINT8             u8_line_buf_bypass_en;/**<  SU line buf bypass */
    DIS_WIN_T         st_dis_win;           /**<  ������Ƶ��ʾ���� */
    VIDEO_WIN_T       st_video_win;         /**<  ������Ƶ���� */
    SU_VIDEO_SIZE_T   st_video_size;        /**<  ����SUģ�顢�������SIZE */
    UINT8             u8_su_color_space;   /**<  ����su color space,refer to HDMI_COLORSPACE_E */
    SU_VIDEO_BORDER_T st_video_border;      /**<  ������Ƶ�߿� */
}SU_DEV_T;

/**
* @brief        SU ģ���init
* @param [in]   pst_su_dev : related para:u8_su_sel refer to SU_SEL_E;
* @return       ERR_INV_PARM if input pointer is null
*/

ERR_TYPE_E api_su_instance_init(SU_DEV_T *pst_su_dev,UINT8 u8_su_sel);
/**
* @brief        SU ģ���ʹ�ܺ͹ر�
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��b_enable		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_enable_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ�����ѡ��Ĭ��Ϊ1��
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��u8_phase_order_sel	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_phase_order_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ���������롢���size����
* @param [in]   pst_su_dev :: related para:1��u8_sel; 2��st_video_size		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_scaler_size_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ���ȡ���롢���size����
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��st_video_size		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_scaler_size_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ������video window ��ʼ�ͽ���λ��
* @param [in]   pst_su_dev related para:1��u8_sel; 2��st_video_win		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_video_win_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ���ȡvideo window ��ʼ�ͽ���λ��
* @param [in]   pst_su_dev related para:1��u8_sel; 2��st_video_win	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_video_win_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ������display window ��ʼ�ͽ���λ��,��Ҫ���ڽ�ȡ��Ƶ��ʾ���ݣ�һ��Ӧ����������
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��st_dis_win	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_display_win_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ���ȡdisplay window ��ʼ�ͽ���λ��
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��st_dis_win		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_su_display_win_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ��CSC����
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��u8_color_space		 
* @return       ERR_INV_PARM if input pointer is null
* @note         ����ΪRGB����YUV444 Color space 
*/
ERR_TYPE_E api_su_csc_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ��video border����
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��st_video_border			 
* @return       ERR_INV_PARM if input pointer is null
* @note 
*/
ERR_TYPE_E api_su_video_border_set(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ��video border ��ȡ
* @param [in]   pst_su_dev : related para:1��u8_sel; 2��st_video_border			 
* @return       ERR_INV_PARM if input pointer is null
* @note 
*/
ERR_TYPE_E api_su_video_border_get(SU_DEV_T *pst_su_dev);

/**
* @brief        SU ģ��Trigger
* @param [in]   pst_su_dev : related para:1��u8_sel;	 
* @return       ERR_INV_PARM if input pointer is null
* @note
*  ��scale upϵ�������仯�������window�����ı䣬trigger֮������Ч
*/
ERR_TYPE_E api_su_trigger(SU_DEV_T *pst_su_dev);

#endif // _SUH__

/**
 * @}
 */
 
 
