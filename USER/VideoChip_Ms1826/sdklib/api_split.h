/**
* @file  api_split.h
* @brief SPLIT模块相关数据结构及函数定义
*/

/**
 * @defgroup API_SPLIT Split Module api
 *
 * @brief 用于对输入视频图像进行Split操作
 * @{
 */

#ifndef __API_SPLIT__
#define __API_SPLIT__

#include "ms_typedef.h"

/** split index，共有四个split模块*/
typedef enum _SPLIT_INDEX_E
{
    SPLIT_1 = 0,    /**< split1 */
    SPLIT_2,        /**< split2 */
    SPLIT_3,        /**< split3 */
    SPLIT_4,        /**< split4 */

    SPLIT_BUTT,
}SPLIT_INDEX_E;

/** split位置结构体*/
typedef struct _SPLIT_POSITION_T
{
    UINT16 u16_horizontal_start;    /**<  H方向split起始位置，最小为1 */
    UINT16 u16_horizontal_active;   /**<  H方向split有效宽度 */
    UINT16 u16_vertical_start;      /**<  V方向split起始位置，最小为1 */
    UINT16 u16_vertical_active;     /**<  V方向split有效宽度 */
}SPLIT_POSITION_T;

/** split模块配置结构体*/
typedef struct _SPLIT_DEV_T
{
    UINT8 u8_split_idx;             /**<  split索引号，详见SPLIT_INDEX_E */
    BOOL b_enable;                  /**<  split输出使能 */
    SPLIT_POSITION_T st_split_pos;  /**<  split位置 */
}SPLIT_DEV_T;

/**
* @brief        SPLIT模块结构体初始化
* @param [in]   pst_split_dev : SPLIT DEV参数指针
*               idx : split索引号
* @return       specific error types if call failed
* @note         仅对结构体初始化，并未对模块初始化配置
*/
ERR_TYPE_E api_split_instance_init(SPLIT_DEV_T *pst_split_dev, UINT8 idx);

/**
* @brief        SPLIT模块工作使能
* @param [in]   pst_split_dev : SPLIT DEV参数指针
*               b_enable：0-失能 非0-使能
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_split_enable(SPLIT_DEV_T *pst_split_dev);
/**
* @brief        SPLIT模块设置
* @param [in]   pst_split_dev : SPLIT DEV参数指针
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_split_config(SPLIT_DEV_T *pst_split_dev);


#endif // _SPLIT_H__

/**
 * @}
 */


