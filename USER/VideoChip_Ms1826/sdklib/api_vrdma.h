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
    UINT16 u16_h_size;                              /**<  ָʾˮƽ����pixel����*/
    UINT16 u16_v_size;                              /**<  ָʾ��ֱ����line����*/
}RDMA_OUT_SIZE_T;

typedef struct _RDMA_FIFO_STATUS_T
{
    UINT8 u8_rdma_fifo_underflow;                   /**<  ָʾRDMA�ڵĻ����Ƿ�д���ˣ�1 ��ʾд���ˣ� 0��ʾδд��*/
}RDMA_FIFO_STATUS_T;


/** VRDMA �豸�ṹ��*/
typedef struct _VRDMA_DEV_T
{
    UINT8  u8_sel;                                  /**<  ѡ�������һ·VRDMA ,DMA_SEL_E */
    BOOL   b_enable;                                /**<  VRDMA ģ���ʹ�ܺ͹ر�*/
    UINT16 u16_line_offset;                         /**<  ����ָʾһ����ɺ���һ�����ݿ�ʼ��λ��*/
    UINT16 u16_request_len;                         /**<  ����VRDAMģ�鵥����MEM��������ĸ��� */
    UINT8  u8_mirror_cfg;                           /**< ��������,�ο� DMA_MIRROR_CFG_E  */
    UINT8  u8_mem_color_space;                      /**< ���뵽mem��ɫ�ʸ�ʽ,�ο� HDMI_COLORSPACE_E  */
    DMA_START_ADDR_T    st_vrdma_start_addr;        /**<  Frame buffer��ʼ��ַ���� */
    RDMA_OUT_SIZE_T     st_vrdma_out_size;          /**<  SU ģ������SIZE */
    UINT8               u8_dsc_ratio;              /**<  DSC ѹ���������ã�refer to DSC_RATIO_E*/
    RDMA_FIFO_STATUS_T  st_rdma_fifo_status;        /**<  read dma buf status*/
}VRDMA_DEV_T;


/**
* @brief        VRDMA ģ��ĳ�ʼ��
* @param [in]   pst_vrdma_dev :  related para:u8_rdma_sel,refer to DMA_SEL_E;	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_instance_init(VRDMA_DEV_T *pst_vrdma_dev,UINT8 u8_rdma_sel);

/**
* @brief        VRDMA ģ���ʹ�ܺ͹ر�
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��b_enable	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_enable_set(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        ����ÿ·DMA��Frame buffer����ʼ��ַ����ַ������8�ı���
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��st_vrdma_start_addr		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_start_addr_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��u16_line_offset	;3��u8_mem_color_space	
* @return       ERR_INV_PARM if input pointer is null
* @note         ������ʾͼ��1������ռ���ڴ�ռ�
*/
ERR_TYPE_E api_vrdma_line_offset_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        ����DMA��memory control ���η��͵Ķ�����ĸ�����һ��Ӧ�ñ���Ĭ��ֵ����
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��u16_request_len	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  ����DMA��memory control ���η��͵Ķ�����ĸ������ڶ������������������£����ֵԽ�󣬱�ʾÿ��Ҫ��������ݾͶ࣬����Ĵ������٣�
*  ���ֵԽС����ʾÿ��Ҫ��������ݾ��٣�����Ĵ����Ͷࡣ ��DMA��˵����ֵԽСԽ���������Ƕ԰������DMA��ϵͳ��˵��DMA֮����л�Խ��
*  Ч��Խ�ߣ�������ǵ���DMA��ϣ��ռ��̫��ʱ�䣬����ֵ��������Ҫ�ۺ�����ϵͳ�����ǡ�
*/
ERR_TYPE_E api_vrdma_request_len_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        burst num ����
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��u8_color_space��3��st_vrdma_out_size 	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vrdma_burst_num_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        rdma mirror����
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel;2��st_vrdma_start_addr;3��u16_line_offset;4��st_vrdma_out_size��5��u8_mem_color_space��6��u8_mirror_cfg		
* @return       ERR_INV_PARM if input pointer is null
* @note
*    1��	DSC_D�Ƕ�DSC_Eѹ��������������н��룬������û�����صĸ�������޷�֧��rotation �� mirror
*    2��	MS1826֧��ǰ��mirror�ͺ��mirror�������뵽memʱ��mirror���Ǵ�mem��ȡ֮������mirror���˴��Ǻ��mirror
*/
ERR_TYPE_E api_vrdma_mirror_cfg(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        ��ȡvrdma fifo status�Դ����жϴ����Ƿ��㹻
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��st_rdma_fifo_status	 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_fifo_status_get(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        ���vrdma fifo status
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel;  	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_fifo_status_clear(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        ѡ��rdma buf id �Ƿ����ⲿģ�����
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��b_enable 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_fb_swith_external(VRDMA_DEV_T *pst_vrdma_dev);

/**
* @brief        vrdma module trigger
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vrdma_trigger(VRDMA_DEV_T *pst_vrdma_dev);

#endif // _VRDMA_H__

/**
 * @}
 */

