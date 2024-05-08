/**
* @file  api_sysram.h  
* @brief 芯片内系统内存驱动及函数定义                                                           
*/

/**
 * @defgroup SYSRAM_API Sysram module api
 *
 * @brief sysram config api system.
 * @{
 */
#ifndef __SYSRAM_API_H__
#define __SYSRAM_API_H__

#include "ms_typedef.h"

/** System memory block size定义*/
#define SYSRAM_BLOCK_SIZE (UINT8)128


/** System memory block index，一共有32组Blcok (ID: 0 - 31)*/
#define SYSRAM_ID_MAX (UINT8)31

/** System memory 参数接口定义*/
typedef struct _SYSRAM_DEV_T
{
    UINT8   u8BlkID;
    UINT8* pu8Data;
    UINT8  u8DatLength;
} SYSRAM_DEV_T;


#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief        system memory 读数据函数
* @param [in]   pst_sysram_dev : 参数指针        
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sysram_read(SYSRAM_DEV_T *pst_sysram_dev);

/**
* @brief        system memory 写数据函数
* @param [in]   pst_sysram_dev : 参数指针        
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_sysram_write(SYSRAM_DEV_T *pst_sysram_dev);


#ifdef __cplusplus
}
#endif

#endif // __SYSRAM_API_H__

/**
 * @}
 */

