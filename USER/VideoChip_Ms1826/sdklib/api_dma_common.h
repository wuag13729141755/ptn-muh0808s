/**
* @file  dma_common.h  
* @brief dma module configure                                                          
*/

/**
 * @defgroup DMA dma api
 *
 * @brief  dma configure
 * @{
 */

#ifndef __DMA_API_H__
#define __DMA_API_H__

#include "ms_typedef.h"

/** DMA select*/
typedef enum _DMA_SEL_E
{
    DMA_SEL_PATH1_DSC = 0,                          /**<  选择DMA0 */
    DMA_SEL_PATH1     = 1,                          /**<  选择DMA1 */
    DMA_SEL_PATH2_DSC = 2,                          /**<  选择DMA2 */
    DMA_SEL_PATH2     = 3,                          /**<  选择DMA3 */
    DMA_SEL_PATH3_DSC = 4,                          /**<  选择DMA4 */
    DMA_SEL_PATH3     = 5,                          /**<  选择DMA5 */
    DMA_SEL_PATH4_DSC = 6,                          /**<  选择DMA6 */
    DMA_SEL_PATH4     = 7,                          /**<  选择DMA7 */
    DMA_NUM
} DMA_SEL_E;

/** Mirror 配置选项*/
typedef enum _DMA_MIRROR_CFG_E
{
    DMA_H_V_MIRROR_ALL_DISABLE=0,                   /**<  No mirror */
    DMA_H_MIRROR_ENABLE,                            /**<  H mirror */
    DMA_V_MIRROR_ENABLE,                            /**<  V mirror */
    DMA_H_V_MIRROR_ALL_ENALBE                       /**<  H&V mirror */
} DMA_MIRROR_CFG_E;

/** 图像旋转角度 配置选项*/
typedef enum _DMA_ROTATION_DEG_E
{
    ROTATION_0_DEG  = 0,                             /**<  图像旋转0度 */
    ROTATION_90_DEG = 1,                                /**<  图像旋转90度 */ 
    //ROTATION_180_DEG,                               /**<  图像旋转180度 */
    ROTATION_270_DEG = 3                               /**<  图像旋转270度 */
} DMA_ROTATION_DEG_E;  

/** frame buffer 地址配置选择*/
typedef struct _DMA_START_ADDR_T
{
    UINT32 u32_dma_start_addr_0;                    /**<  Frame buffer 0 起始地址 */
    UINT32 u32_dma_start_addr_1;                    /**<  Frame buffer 1 起始地址 */
    UINT32 u32_dma_start_addr_2;                    /**<  Frame buffer 2 起始地址 */
} DMA_START_ADDR_T;


/** frame buffer num sel 选择*/
typedef enum _DMA_BUFFER_NUM_SEL_E
{
    SINGLE_BUF = 0,                                 /**<  单buf */
    DUAL_BUF,                                       /**<  双buf */
    TRI_BUF                                         /**<  三buf */
} DMA_BUFFER_NUM_SEL_E;

/** frame buffer ID 选择*/
typedef enum _DMA_BUFFER_ID_E
{
    DMA_BUFFER_A = 0,                               /**<  选择buffer 0 */
    DMA_BUFFER_B,                                   /**<  选择buffer 1 */
    DMA_BUFFER_C                                    /**<  选择buffer 2 */
} DMA_BUFFER_ID_E;

/** buf id 调整模式 选择*/
typedef enum _DMA_BUF_SWITCH_MODE_E
{
	DMA_BUF_SWITCH_AUTO = 0,
	DMA_BUF_SWITCH_MANU
}DMA_BUF_SWITCH_MODE;

/** 帧率转换模式选择*/
typedef enum _DMA_FRAME_RATE_DETECT_E
{
    MANU_MODE = 0,                                  /**<  手动配置输入、输出帧率之间关系 */
    AUTO_MODE = 1,                                  /**<  自动检测输入、输出帧率之间关系 */
} DMA_FRAME_RATE_DETECT_E;

/** 输入输出帧率关系选择，只在MANU_MODE有效*/
typedef enum _DMA_FRAME_RATE_E
{
    L2H_OR_EQUAL = 0,                               /**<  capture video frame rate less than or equal playback video frame rate */
    H2L,                                            /**<  capture video frame rate more than playback video frame rate */
} DMA_FRAME_RATE_E;

/** 帧率转换 选择*/
typedef struct _DMA_FRAME_RATE_CFG_T
{
    UINT8 u8_frame_rate_mode;                       /**<  帧率配置机制，DMA_FRAME_RATE_DETECT_E */
    UINT8 u8_frame_rate;                            /**<  帧率，只在MANU_MODE生效，此种工作情况下，需要手动配置帧率是H2L,还是L2H,refer to DMA_FRAME_RATE_E */
}DMA_FRAME_RATE_CFG_T;

/** 帧率状态,如果在2^24个系统时钟周期内帧数小于4帧，则判定帧率为0，just for debug*/
typedef struct _DMA_FRAME_RATA_STATUS_T
{
    UINT8 u8_in_frame_rate_is_zero;                 /**<  为1表示输入视频帧率为0，为0则表示输入视频帧率不为0； */
    UINT8 u8_out_frame_rate_is_zero;                /**<  为1表示输出视频帧率为0，为0则表示输出帧率不为0. */
}DMA_FRAME_RATE_STATUS_T;

typedef struct _DMA_COMMON_STATUS_T
{
    UINT8 wdma_buf_id;           /**<  WDMA 正在写的buffer ID 选择,参考 DMA_BUFFER_ID_E*/                         
    UINT8 rdma_buf_id;           /**<  RDMA 正在读的buffer ID 选择,参考 DMA_BUFFER_ID_E*/                
    BOOL b_drop_frame_happed;       /**<  0-normal,1-drop frame is happening*/               
    BOOL b_h2l_status;              /**<  0-capture frame rate less than playback frame rate,1-capture frame rate more than playback frame rate*/
    DMA_FRAME_RATE_STATUS_T st_frame_rate_status;
}DMA_COMMON_STATUS_T;

typedef struct _FRC_CFG_T
{
    BOOL  b_frc_ctl_ext;                            /**<  FRC use external input buffer ID which generated by other modules,Used for the case that current SU need fetch the data from the same buffer as other modules. */
    UINT8 u8_src_buf_id_mode;                       /**<  输入(source）buf id的获取方式，refer to DMA_BUF_SWITCH_MODE */
    UINT8 u8_dst_buf_id_mode;                       /**<  输出(dest）buf id的获取方式，refer to DMA_BUF_SWITCH_MODE */
    UINT8 u8_src_buf_id_sel;                        /**<  指定输入(source）buf id的获取方式，refer to DMA_FRAME_RATE_DETECT_E */
    UINT8 u8_dst_buf_id_sel;                        /**<  手动输入(dest）buf id，refer to DMA_BUFFER_ID_E */
}FRC_CFG_T;

/** RGB/YUV444 data selection in packing to 64bit.
 * For each 24bit, high byte, middle byte and low byte sequence as following
 */
typedef enum _ROTATE_DATA_SEL_444_E
{
    YG_UB_VR = 0,                       
    YG_VR_UB,                           
    UB_YG_VR,                          
    UB_VR_YG,                          
    VR_UB_YG,                           
    YVR_YG_UB,                          
    DATA_NUM
}ROTATE_DATA_SEL_444_E;

/** DSC CSC CFG.*/
typedef struct _DSC_CSC_CFG_T
{
    UINT8 u8_uv_swap_yuv422;                        /**<  Swap the U and V data which output from DSC 1: enable swap 0: not swap*/
    UINT8 u8_csc_mode;                              /**<  DSC 工作在哪种Color space.refer to HDMI_COLORSPACE_E*/
}DSC_CSC_CFG_T;

/** DMA 设备结构体*/
typedef struct _DMA_DEV_T
{
    UINT8 u8_sel;                                   /**<  选择操作哪一路DMA ,DMA_SEL_E */
    UINT8 u8_dma_buf_num_sel;                       /**<  指定DMA buffer个数 ,DMA_BUFFER_NUM_SEL_E */
    DMA_FRAME_RATE_CFG_T st_frame_rate_cfg;         /**<  帧率配置*/
    FRC_CFG_T            st_frc_cfg;                /**<  FRC配置*/
    DSC_CSC_CFG_T        st_dsc_csc_cfg;            /**< DSC CSC CFG.*/
    DMA_COMMON_STATUS_T  st_dma_status;             /**<  DMA 的状态信息*/
}DMA_COMMON_DEV_T;

/**
* @brief        dma commom init
* @param [in]   pst_dma_common_dev : related para:u8_dma_com_sel,refer to DMA_SEL_E ;
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_dma_common_instance_init(DMA_COMMON_DEV_T *pst_dma_dev,UINT8 u8_dma_com_sel);

/**
* @brief        指定buffer个数 ,DMA_BUFFER_NUM_SEL_E
* @param [in]   pst_vrdma_dev :  related para:1、u8_sel; 2、u8_dma_buf_num_sel
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_dma_common_buf_num_set(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        进出DMA帧率配置，默认情况下是自动检测模式，无须配置输入、输出帧率
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_frame_rate_cfg	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_frame_rate_cfg(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        DSC通路color space 配置
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_dsc_csc_cfg		
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_dsc_csc_cfg(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        buf id 模式设置
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_frc_cfg.u8_src_buf_id_mode;3、st_frc_cfg.u8_dst_buf_id_mode  	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_comom_buf_id_mode_cfg(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        手动切换buf id 模式下，配置 src buf id
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、u8_src_buf_id_sel	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_comom_wdma_buf_id_set(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        手动切换buf id 模式下，配置dst buf id
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、u8_dst_buf_id_sel	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_comom_rdma_buf_id_set(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        manu模式下，src_buf_id / dst_buf_id 设置   
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_frc_cfg.u8_src_buf_id_sel 3、st_frc_cfg.u8_dst_buf_id_sel
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_comom_buf_id_set(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief    buf id control by other moudle or not    
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_frc_cfg.b_frc_ctl_ext
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_comom_buf_frc_ctl_ext(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        获取正在写的buf id    
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_dma_status.wdma_buf_id
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_wdma_buf_id_get(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        获取正在写的buf id    
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_dma_status.rdma_buf_id
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_rdma_buf_id_get(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        获取正在读写的buf id    
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_dma_status.wdma_buf_id 3、st_dma_status.rdma_buf_id
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_buf_id_get(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        获取dma fifo 状态
* @param [in]   pst_dma_common_dev : related para:1、u8_sel; 2、st_dma_status	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_fifo_status_get(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        DMA Common trigger
* @param [in]   pst_vrdma_dev :  pst_dma_common_dev : related para:1、u8_sel;
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_trigger(DMA_COMMON_DEV_T *pst_dma_dev);

/**
* @brief        DMA Common trigger,同时trigger多条视频通路
* @param [in]   
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_dma_common_simultaneous_trigger(VOID);


#endif //__DMA_API_H__



/**
 * @}
 */

