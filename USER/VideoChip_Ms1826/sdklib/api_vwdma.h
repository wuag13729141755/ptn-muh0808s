/**
* @file  VWDMA.h  
* @brief VWDMA module configure                                                          
*/

/**
 * @defgroup VWDMA VWDMA api
 *
 * @brief 
 * @verbatim
 *  
 * @endverbatim
 * @{
 */

#ifndef __VWDMA_API_H__
#define __VWDMA_API_H__

#include "ms_typedef.h"
#include "api_dma_common.h"

typedef struct _WDMA_IN_SIZE_T
{
    UINT16 u16_h_size;                              /**<  指示水平方向pixel个数*/
    UINT16 u16_v_size;                              /**<  指示垂直方向line行数*/
}WDMA_IN_SIZE_T;

typedef struct _WDMA_INTERLACE_CFG_T
{
    UINT8 b_interlace_input;                         /**< 输入是否是I格式  */
    UINT8 b_field_flip;                              /**<  默认输入的field index 为0 时对应odd, 为1时对应even. 设置该位为1，可以将输入的field index极性取反。  */
}WDMA_INTERLACE_CFG_T;

typedef struct _WDMA_FIFO_STATUS_T
{
    UINT8 u8_wdma_fifo_overflow;                      /**<  指示WDMA内的缓存是否被写满了，1 表示写满了， 0表示未写满*/
    UINT8 u8_wdma_video_miss;                         /**<  表示当前帧的数据要开始写的时候，上一帧数据是否有被完全被写完。为1表示发生过上帧数据在当前帧要开始写的时候依然未写完的情况；*/
}WDMA_FIFO_STATUS_T;

/** VWDMA 设备结构体*/
typedef struct _VWDMA_DEV_T
{
    UINT8  u8_sel;                                  /**<  选择操作哪一路VWDMA ,DMA_SEL_E */
    BOOL b_enable;                                  /**<  VWDMA 模块的使能和关闭*/
    UINT16 u16_request_len;                         /**<  调节VRDAM模块单次向MEM发送请求的个数 */
    UINT8 u8_mirror_cfg;                            /**< 镜像配置,参考 VWDMA_MIRROR_CFG_E  */
    UINT8 u8_color_space;                           /**< 进入到VWDMA中的color space,参考 HDMI_COLORSPACE_E  */
    UINT16 u16_line_offset;                         /**< 用来指示一行完成后，下一行数据开始的位置*/
    WDMA_INTERLACE_CFG_T st_vwdma_interlace_cfg;    /**< I格式的处理  */
    UINT8                u8_vwdma_rotatation_deg;   /**< 旋转角度,参考 DMA_ROTATION_DEG_E  */
    DMA_START_ADDR_T     st_vwdma_start_addr;       /**<  Frame buffer起始地址分配 */
    WDMA_IN_SIZE_T       st_vwdma_in_size;          /**<  输入到DMA中的size */
    UINT8                u8_dsc_ratio;              /**<  DSC 压缩比例设置，refer to DSC_RATIO_E*/
    WDMA_FIFO_STATUS_T   st_wdma_fifo_status;       /**<  write dma buf status*/
}VWDMA_DEV_T;

/**
* @brief        VWDMA 模块的初始化
* @param [in]   pst_vwdma_dev :  related para:1、u8_wdma_sel:reger to DMA_SEL_E; 
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_instance_init(VWDMA_DEV_T *pst_vwdma_dev,UINT8 u8_wdma_sel);


/**
* @brief        VWDMA 模块的使能和关闭
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、b_enable 
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_enable_set(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        设置每路DMA的Frame buffer的起始地址
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、st_vwdma_start_addr 	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_start_addr_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        Dram buffer address offset for one video line pixel data
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、u16_line_offset；3、u8_color_space 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_line_offset_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        调节DMA向memory control 单次发送的读请求的个数
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、u16_request_len	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  调节DMA向memory control 单次发送的读请求的个数。在读数据总量不变的情况下，这个值越大，表示每次要求读的数据就多，请求的次数就少；
*  这个值越小，表示每次要求读的数据就少，请求的次数就多。 对DMA来说，该值越小越有利，但是对包含多个DMA的系统来说，DMA之间的切换越少
*  效率越高，另外就是单个DMA不希望占用太长时间，具体值的配置需要综合整个系统来考虑。一般的可设置成128。
*/
ERR_TYPE_E api_vwdma_request_len_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        输入到mem中的数据格式是否是I格式,如果是I格式，会把I的奇偶场交替的写到同一块frame buf中
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、st_vwdma_interlace_cfg	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_vwdma_interlace_mode_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        burst num 配置
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、u8_color_space；3、st_vwdma_in_size	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_burst_num_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        mirror配置
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、st_vwdma_start_addr;3、u16_line_offset；4、st_vwdma_in_size ；5、u8_color_space；6、u8_mirror_cfg	
* @return       ERR_INV_PARM if input pointer is null
* @note
*    1）	DSC_D是对DSC_E压缩后的数据流进行解码，数据流没有像素的概念，所以无法支持rotation 和 mirror
*    2）	MS1826支持前端mirror和后端mirror，即进入到mem时做mirror还是从mem读取之后再做mirror，此处是前端mirror
*/
ERR_TYPE_E api_vwdma_mirror_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        rotation配置
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 2、st_vwdma_start_addr;3、u16_line_offset；4、st_vwdma_in_size ；5、u8_color_space；6、u8_vwdma_rotatation_deg		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_rotation_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        pst_vwdma_dev :  related para:1、u8_sel;
* @param [in]   pst_vwdma_dev :  pointer to VWDMA_DEV_T 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_fifo_status_get(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        wdma fifo status clear
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_fifo_status_clear(VWDMA_DEV_T *pst_vwdma_dev);


/**
* @brief        选择wdma buf id 是否受外部模块控制
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、b_enable 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_fb_swith_external(VWDMA_DEV_T *pst_vwdma_dev);
/**
* @brief        wdma module trigger
* @param [in]   pst_vwdma_dev :  related para:1、u8_sel; 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_trigger(VWDMA_DEV_T *pst_vwdma_dev);

#endif // _VWDMA_H__

/**
 * @}
 */

