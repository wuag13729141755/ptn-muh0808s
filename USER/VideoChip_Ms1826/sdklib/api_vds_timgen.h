/**
* @file  api_vds_timgen.h
* @brief 输出时序配置模块数据及函数定义
*/

/**
 * @defgroup VDS_TIMGEN_API Timgen module api
 *
 * @brief 用于配置输出的Timing
 * @{
 */
#ifndef __VDS_TIMGEN_API_H__
#define __VDS_TIMGEN_API_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"
#include "ms1826_timing_table.h"

/** TIMGEN index，一共有4组Timgen*/
typedef enum _TIMGEN_INDEX_E
{
    TIMGEN_1,      /**<  Timgen 1,表示第一个Timgen */
    TIMGEN_2,      /**<  Timgen 2,表示第二个Timgen */
    TIMGEN_3,      /**<  Timgen 3,表示第三个Timgen */
    TIMGEN_4,      /**<  Timgen 4,表示第四个Timgen */
    TIMGEN_NUM,    /**<  表示Timgen的总数          */
}TIMDEN_INDEX_E;

/** Timgen的工作模式*/
typedef enum _TIMGEN_WORK_MODE_E
{
	FreeRun_Mode,                /**< 用户自定义输出T时钟、行频场频等时序信息                                         */
	Casecade_Mode,               /**< 芯片级联方式 */
}TIMGEN_WORK_MODE_E;

/** 输入时序的极性列表*/
typedef enum _TIMGEN_INPUT_SYNC_POLARITY_E
{
    VPosHPos,          /**< V方向正极，H方向正极 */
    VPosHNeg,          /**< V方向正极，H方向负极 */
    VNegHPos,          /**< V方向负极，H方向正极 */
    VNegHNeg           /**< V方向负极，H方向负极 */
} TIMGEN_INPUT_SYNC_POLARITY_E;/** Timgen config参数*/
typedef struct _VDS_TMG_DEV_T
{
    UINT8		 		      	    u8_timgen_id;        /**< 选择不同的Timgen                                                    */
    BOOL                            b_timgen_en;        /**< 使能Timgen                                                          */
    UINT8				            u8_workmode;         /**< Timgen 产生timing的工作模式选择，参考 TIMGEN_WORK_MODE_E              */
    HDMI_VIDEO_TIMING_T* 		    pst_timing;          /**< 配置Timgen输出 timing的参数                                          */
} VDS_TMG_DEV_T;


/**
* @brief        Timgen模块初始化配置
* @param [in]   pst_tmg_dev : Timgen模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_timgen_instance_init(VDS_TMG_DEV_T *pst_tmg_dev,UINT8 u8_timgen_index);
/**
* @brief		Timgen 初始化
* @param [in]   pst_tmg_dev : Timgen config参数指针
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vds_tmg_init(VDS_TMG_DEV_T *pst_tmg_dev);

/**
* @brief		Timgen 配置函数
* @param [in]   pst_tmg_dev : Timgen config参数指针
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vds_tmg_config(VDS_TMG_DEV_T *pst_tmg_dev);




#endif // _VDS_TIMGEN_API_H__

/**
 * @}
 */
