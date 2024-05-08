/**
* @file  vrdma.h  
* @brief vrdma module configure                                                          
*/

/**
 * @defgroup VRDMA vrdma api
 *
 * @brief 
 * @verbatim
 *  
 * @endverbatim
 * @{
 */

#ifndef __VRDMA_API_H__
#define __VRDMA_API_H__

#include "api_dma_common.h"

typedef struct _RDMA_OUT_SIZE_T
{
    UINT16 u16_h_size;                              /**<  指示水平方向pixel个数*/
    UINT16 u16_v_size;                              /**<  指示垂直方向line行数*/
}RDMA_OUT_SIZE_T;

typedef struct _RDMA_FIFO_STATUS_T
{
    UINT8 u8_rdma_fifo_underflow;                   /**<  指示RDMA内的缓存是否被写满了，1 表示写满了， 0表示未写满*/
}RDMA_FIFO_STATUS_T;


/** VRDMA 设备结构体*/
typedef struct _VRDMA_DEV_T
{
    UINT8  u8_sel;                                  /**<  选择操作哪一路VRDMA ,DMA_SEL_E */
    BOOL   b_enable;                                /**<  VRDMA 模块的使能和关闭*/
    UINT16 u16_line_offset;                         /**<  用来指示一行完成后，下一行数据开始的位置*/
    UINT16 u16_request_len;                         /**<  调节VRDAM模块单次向MEM发送请求的个数 */
    UINT8  u8_mirror_cfg;                           /**< 镜像配置,参考 DMA_MIRROR_CFG_E  */
    UINT8  u8_mem_color_space;                      /**< 存入到mem中色彩格式,参考 HDMI_COLORSPACE_E  */
    DMA_START_ADDR_T    st_vrdma_start_addr;        /**<  Frame buffer起始地址分配 */
    RDMA_OUT_SIZE_T     st_vrdma_out_size;          /**<  SU 模块输入SIZE */
    UINT8               u8_dsc_ratio;              /**<  DSC 压缩比例设置，refer to DSC_RATIO_E*/
    RDMA_FIFO_STATUS_T  st_rdma_fifo_status;        /**<  read dma buf status*/
}VRDMA_DEV_T;


/**
* @brief        VRDMA 模块的初始化
* @param [in]   pst_vrdma_dev :  related para:u8_rdma_sel,refer to DMA_SEL_E;	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_instance_init(VRDMA_DEV_T *pst_vrdma_dev,UINT8 u8_rdma_sel);

/**
* @brief        VRDMA 模块的使能和关闭
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、b_enable	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_enable_set(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        设置每路DMA的Frame buffer的起始地址，地址必须是8的倍数
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、st_vrdma_start_addr		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_start_addr_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、u16_line_offset	;3、u8_mem_color_space	
* @return       ERR_INV_PARM if input pointer is null
* @note         用来表示图像1行数据占用内存空间
*/
ERR_TYPE_E api_vrdma_line_offset_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        调节DMA向memory control 单次发送的读请求的个数，一般应用保持默认值即可
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、u16_request_len	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  调节DMA向memory control 单次发送的读请求的个数。在读数据总量不变的情况下，这个值越大，表示每次要求读的数据就多，请求的次数就少；
*  这个值越小，表示每次要求读的数据就少，请求的次数就多。 对DMA来说，该值越小越有利，但是对包含多个DMA的系统来说，DMA之间的切换越少
*  效率越高，另外就是单个DMA不希望占用太长时间，具体值的配置需要综合整个系统来考虑。
*/
ERR_TYPE_E api_vrdma_request_len_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        burst num 配置
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、u8_color_space；3、st_vrdma_out_size 	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_burst_num_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        rdma mirror配置
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel;2、st_vrdma_start_addr;3、u16_line_offset;4、st_vrdma_out_size；5、u8_mem_color_space；6：u8_mirror_cfg		
* @return       ERR_INV_PARM if input pointer is null
* @note
*    1）	DSC_D是对DSC_E压缩后的数据流进行解码，数据流没有像素的概念，所以无法支持rotation 和 mirror
*    2）	MS1826支持前端mirror和后端mirror，即进入到mem时做mirror还是从mem读取之后再做mirror，此处是后端mirror
*/
ERR_TYPE_E api_vrdma_mirror_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        获取vrdma fifo status以此来判断带宽是否足够
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、st_rdma_fifo_status	 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_fifo_status_get(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        清除vrdma fifo status
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel;  	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_fifo_status_clear(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        选择rdma buf id 是否受外部模块控制
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、b_enable 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_fb_swith_external(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        vrdma module trigger
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_trigger(VRDMA_DEV_T *pst_vrdma_dev);

#endif // _VRDMA_H__

/**
 * @}
 */

