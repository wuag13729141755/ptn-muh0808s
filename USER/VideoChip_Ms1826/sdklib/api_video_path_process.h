/**
* @file  api_video_path_process.h  
* @brief ��Ƶͨ·���ú���                                                          
*/

/**
 * @defgroup SS_API video path process api
 *
 * @brief ����ʵ����Ƶͨ·����
 * @{
 */
#ifndef __VIDEO_PROCESS_API_H__
#define __VIDEO_PROCESS_API_H__

#include "ms_typedef.h"

typedef enum _SS_CH_SEL_E
{
    CHANNEL1_with_dsc  = 1,         /**< ��ʾ��Ƶͨ·1����DSC��path    */
    CHANNEL1_without_dsc  = 2,      /**< ��ʾ��Ƶͨ·1������DSC��path  */
    CHANNEL2_with_dsc  = 4,         /**< ��ʾ��Ƶͨ·2����DSC��path    */
    CHANNEL2_without_dsc  = 8,      /**< ��ʾ��Ƶͨ·2������DSC��path  */
    CHANNEL3_with_dsc  = 16,        /**< ��ʾ��Ƶͨ·2����DSC��path    */
    CHANNEL3_without_dsc  = 32,     /**< ��ʾ��Ƶͨ·3������DSC��path  */
    CHANNEL4_with_dsc  = 64,        /**< ��ʾ��Ƶͨ·4����DSC��path    */
    CHANNEL4_without_dsc  = 128,    /**< ��ʾ��Ƶͨ·4������DSC��path  */
}SS_CH_SEL_E;

typedef enum _SS_VIPSS_PATH_SEL_E
{
    VIPSS1  = 0,   /**< ��ʾ��Ƶͨ·1 */
    VIPSS2  = 1,   /**< ��ʾ��Ƶͨ·2 */
    VIPSS3  = 2,   /**< ��ʾ��Ƶͨ·3 */
    VIPSS4  = 3,   /**< ��ʾ��Ƶͨ·4 */
}SS_VIPSS_PATH_SEL_E;

typedef enum _SS_VOPSS_PATH_SEL_E
{
    VOPSS1  = 0,   /**< ��ʾ��Ƶͨ·1 */
    VOPSS2  = 1,   /**< ��ʾ��Ƶͨ·2 */
    VOPSS3  = 2,   /**< ��ʾ��Ƶͨ·3 */
    VOPSS4  = 3,   /**< ��ʾ��Ƶͨ·4 */
}SS_VOPSS_PATH_SEL_E;


/** ʹ��VOPSS �е�triggerʱ��ѡ��trigger��ʽ*/
typedef enum __SS_TRIGGER_MODE
{
    FORCE_TRIGGER = 0,
    VB_INTERVAL_TRIGGER = 1,
    NEXT_VB_RISING_EDGE_TRIGGER = 2
}SS_TRIGGER_MODE;

/** ʹ��TIMGEN triggerʱ��ѡ��trigger��һ·TIMGEN*/
typedef enum __SS_TRIGGER_TMG_E
{
    VDS_TMG1 = 0,
    VDS_TMG2 = 1,
    VDS_TMG3 = 2,
    VDS_TMG4 = 3
}SS_TRIGGER_TMG_E;

/** ѡ��ʹ������trigger��ʽ*/
typedef enum __SS_TRIGGER_SEL_E
{
    TRIGGER_BY_VOPSS_TRIGGER = 0,              /**< ʹ��vopss�и��Ե�globle trigger  */ 
    TRIGGER_BY_VDS_TMG                         /**< ��·���ʹ��ͬһ��timgen������ͬʱtrigger��refer to SS_TRIGGER_TMG_E  */ 
}SS_TRIGGER_SEL_E;

/** VOPSS trigger ����*/
typedef struct __SS_VOPSS_TRIGGER_SET_T
{
    UINT8 u8_trigger_sel;                      /**< VDS TMG trigger����VOPSS trigger��refer to SS_TRIGGER_SEL_E  */  
    UINT8 u8_trigger_tmg;                      /**< SS_TRIGGER_TMG_E,only effctive for TRIGGER_BY_VDS_TMG  */
    UINT8 u8_trigger_mode;                     /**< SS_TRIGGER_MODE,only effctive for TRIGGER_BY_VOPSS_TRIGGER */    
}SS_VOPSS_TRIGGER_SET_T;

typedef struct _SS_DEV_T
{
    UINT8 u8_sel;                                       /**< ��Ҫ��������Ƶͨ�����ο�SS_CH_SEL_E��ѡ���ͨ��ʱ����"��"����   */
    UINT8 u8_vopss_src_vb_sel;                          /**< ѡ��VOPSS��FRC �� src vb ��Դ��SS_VIPSS_PATH_SEL_E  */
    UINT8 u8_vipss_wdma_id_sel;                         /**< ������Ƶͨ·wdma buf id sel��refer to SS_VOPSS_PATH_SEL_E    */
    UINT8 u8_vopss_dst_buf_id_sel;                      /**< VOPSS��FRC ��dst buf id ѡ��,refer to SS_VOPSS_PATH_SEL_E  */
    UINT8 u8_vipss_trigger_mode;                        /**< VIPSS trigger mode sesl,refer to SS_TRIGGER_MODE  */
    SS_VOPSS_TRIGGER_SET_T st_vopss_trigger_mode;       /**< VOPSS trigger mode sesl,refer to SS_VOPSS_TRIGGER_SET_T  */
}SS_DEV_T;


/**
* @brief        SSģ���ʼ��
* @param [in]   pst_vrdma_dev :  related para:u8_ss_sel,refer to SS_CH_SEL_E;	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_instance_init(SS_DEV_T *pst_ss_dev,UINT8 u8_ss_sel);

/**
* @brief		vipss trigger
* @param [in]   pst_ss_dev : related para:1��u8_ch_sel; 2��u8_vipss_trigger_mode
* @note         ����SDN��WDwpackageTATE����������Ч,ͨ·����ͨ�����򡱵ķ�ʽͬʱ���ö�·     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vipss_trig(SS_DEV_T *pst_ss_dev);

/**
* @brief		vopss trigger mode control,ֻ�����ô�����ʽ��������
* @param [in]   pst_ss_dev : related para:1��u8_ch_sel; 2��st_ss_vopss_trigger_mode
* @note         ����VROTATE��VRDMA��DSC_DE��SU����������Ч,ͨ·����ͨ�����򡱵ķ�ʽͬʱ���ö�·     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_trig_mode_cfg(SS_DEV_T *pst_ss_dev);

/**
* @brief		vopss trigger
* @param [in]   pst_ss_dev : related para:1��u8_ch_sel; 2��st_ss_vopss_trigger_mode
* @note         ����VROTATE��VRDMA��DSC_DE��SU����������Ч,ͨ·����ͨ�����򡱵ķ�ʽͬʱ���ö�·     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_trig(SS_DEV_T *pst_ss_dev);

/**
* @brief		vopss trigger
* @param [in]   pst_ss_dev : related para: 1��st_ss_vopss_trigger_mode
* @note         ͬʱtrigger������Ƶͨ·     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_simultaneous_trig(SS_DEV_T *pst_ss_dev);


/**
* @brief		VOPSS�е�FRC src vb sel
* @param [in]   pst_ss_dev : related para:1��u8_ch_sel; 2��u8_vopss_src_vb_sel
* @note         ͨ·����ͨ�����򡱵ķ�ʽͬʱ���ö�·      
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_src_vb_sel(SS_DEV_T *pst_ss_dev);

/**
* @brief		WDMA buffer id sel
* @param [in]   pst_ss_dev : related para:1��u8_ch_sel; 2��u8_vipss_wdma_id_sel
* @note         ͨ·����ͨ�����򡱵ķ�ʽͬʱ���ö�·     
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vipss_wdma_buf_sel(SS_DEV_T *pst_ss_dev);


/**
* @brief		VOPSS �е�FRC dst buf id sel
* @param [in]   pst_ss_dev : related para:1��u8_ch_sel; 2��u8_vopss_dst_buf_id_sel
* @note         ����FRC����buff id��ͨ·����ͨ�����򡱵ķ�ʽͬʱ���ö�·      
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_ss_vopss_dst_buf_id_sel(SS_DEV_T *pst_ss_dev);

#endif // _SSH__

/**
 * @}
 */
