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
    UINT16 u16_h_size;                              /**<  ָʾˮƽ����pixel����*/
    UINT16 u16_v_size;                              /**<  ָʾ��ֱ����line����*/
}WDMA_IN_SIZE_T;

typedef struct _WDMA_INTERLACE_CFG_T
{
    UINT8 b_interlace_input;                         /**< �����Ƿ���I��ʽ  */
    UINT8 b_field_flip;                              /**<  Ĭ�������field index Ϊ0 ʱ��Ӧodd, Ϊ1ʱ��Ӧeven. ���ø�λΪ1�����Խ������field index����ȡ����  */
}WDMA_INTERLACE_CFG_T;

typedef struct _WDMA_FIFO_STATUS_T
{
    UINT8 u8_wdma_fifo_overflow;                      /**<  ָʾWDMA�ڵĻ����Ƿ�д���ˣ�1 ��ʾд���ˣ� 0��ʾδд��*/
    UINT8 u8_wdma_video_miss;                         /**<  ��ʾ��ǰ֡������Ҫ��ʼд��ʱ����һ֡�����Ƿ��б���ȫ��д�ꡣΪ1��ʾ��������֡�����ڵ�ǰ֡Ҫ��ʼд��ʱ����Ȼδд��������*/
}WDMA_FIFO_STATUS_T;

/** VWDMA �豸�ṹ��*/
typedef struct _VWDMA_DEV_T
{
    UINT8  u8_sel;                                  /**<  ѡ�������һ·VWDMA ,DMA_SEL_E */
    BOOL b_enable;                                  /**<  VWDMA ģ���ʹ�ܺ͹ر�*/
    UINT16 u16_request_len;                         /**<  ����VRDAMģ�鵥����MEM��������ĸ��� */
    UINT8 u8_mirror_cfg;                            /**< ��������,�ο� VWDMA_MIRROR_CFG_E  */
    UINT8 u8_color_space;                           /**< ���뵽VWDMA�е�color space,�ο� HDMI_COLORSPACE_E  */
    UINT16 u16_line_offset;                         /**< ����ָʾһ����ɺ���һ�����ݿ�ʼ��λ��*/
    WDMA_INTERLACE_CFG_T st_vwdma_interlace_cfg;    /**< I��ʽ�Ĵ���  */
    UINT8                u8_vwdma_rotatation_deg;   /**< ��ת�Ƕ�,�ο� DMA_ROTATION_DEG_E  */
    DMA_START_ADDR_T     st_vwdma_start_addr;       /**<  Frame buffer��ʼ��ַ���� */
    WDMA_IN_SIZE_T       st_vwdma_in_size;          /**<  ���뵽DMA�е�size */
    UINT8                u8_dsc_ratio;              /**<  DSC ѹ���������ã�refer to DSC_RATIO_E*/
    WDMA_FIFO_STATUS_T   st_wdma_fifo_status;       /**<  write dma buf status*/
}VWDMA_DEV_T;

/**
* @brief        VWDMA ģ��ĳ�ʼ��
* @param [in]   pst_vwdma_dev :  related para:1��u8_wdma_sel:reger to DMA_SEL_E; 
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_instance_init(VWDMA_DEV_T *pst_vwdma_dev,UINT8 u8_wdma_sel);


/**
* @brief        VWDMA ģ���ʹ�ܺ͹ر�
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��b_enable 
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_enable_set(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        ����ÿ·DMA��Frame buffer����ʼ��ַ
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��st_vwdma_start_addr 	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_start_addr_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        Dram buffer address offset for one video line pixel data
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��u16_line_offset��3��u8_color_space 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_line_offset_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        ����DMA��memory control ���η��͵Ķ�����ĸ���
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��u16_request_len	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  ����DMA��memory control ���η��͵Ķ�����ĸ������ڶ������������������£����ֵԽ�󣬱�ʾÿ��Ҫ��������ݾͶ࣬����Ĵ������٣�
*  ���ֵԽС����ʾÿ��Ҫ��������ݾ��٣�����Ĵ����Ͷࡣ ��DMA��˵����ֵԽСԽ���������Ƕ԰������DMA��ϵͳ��˵��DMA֮����л�Խ��
*  Ч��Խ�ߣ�������ǵ���DMA��ϣ��ռ��̫��ʱ�䣬����ֵ��������Ҫ�ۺ�����ϵͳ�����ǡ�һ��Ŀ����ó�128��
*/
ERR_TYPE_E api_vwdma_request_len_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        ���뵽mem�е����ݸ�ʽ�Ƿ���I��ʽ,�����I��ʽ�����I����ż�������д��ͬһ��frame buf��
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��st_vwdma_interlace_cfg	
* @return       ERR_INV_PARM if input pointer is null
* @note
*/
ERR_TYPE_E api_vwdma_interlace_mode_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        burst num ����
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��u8_color_space��3��st_vwdma_in_size	
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_burst_num_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        mirror����
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��st_vwdma_start_addr;3��u16_line_offset��4��st_vwdma_in_size ��5��u8_color_space��6��u8_mirror_cfg	
* @return       ERR_INV_PARM if input pointer is null
* @note
*    1��	DSC_D�Ƕ�DSC_Eѹ��������������н��룬������û�����صĸ�������޷�֧��rotation �� mirror
*    2��	MS1826֧��ǰ��mirror�ͺ��mirror�������뵽memʱ��mirror���Ǵ�mem��ȡ֮������mirror���˴���ǰ��mirror
*/
ERR_TYPE_E api_vwdma_mirror_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        rotation����
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 2��st_vwdma_start_addr;3��u16_line_offset��4��st_vwdma_in_size ��5��u8_color_space��6��u8_vwdma_rotatation_deg		
* @return       ERR_INV_PARM if input pointer is null
*/
ERR_TYPE_E api_vwdma_rotation_cfg(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        pst_vwdma_dev :  related para:1��u8_sel;
* @param [in]   pst_vwdma_dev :  pointer to VWDMA_DEV_T 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_fifo_status_get(VWDMA_DEV_T *pst_vwdma_dev);

/**
* @brief        wdma fifo status clear
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_fifo_status_clear(VWDMA_DEV_T *pst_vwdma_dev);


/**
* @brief        ѡ��wdma buf id �Ƿ����ⲿģ�����
* @param [in]   pst_vrdma_dev :  related para:1��u8_sel; 2��b_enable 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_fb_swith_external(VWDMA_DEV_T *pst_vwdma_dev);
/**
* @brief        wdma module trigger
* @param [in]   pst_vwdma_dev :  related para:1��u8_sel; 	
* @return       ERR_INV_PARM if input pointer is null
* @note
*  
*/
ERR_TYPE_E api_vwdma_trigger(VWDMA_DEV_T *pst_vwdma_dev);

#endif // _VWDMA_H__

/**
 * @}
 */

