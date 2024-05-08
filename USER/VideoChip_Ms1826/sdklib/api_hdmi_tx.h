/**
* @file  api_hdmi_tx.h
* @brief HDMI_TX模块相关数据结构及函数定义
*/

/**
 * @defgroup API_HDMI_TX hdmi tx module api
 *
 * @brief hdmi tx 模块配置
 * @{
 */
#ifndef __HDMI_TX_API_H__
#define __HDMI_TX_API_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"

#define HDMI_TX_HDCP1_4_BKSV_SIZE (5)
#define HDMI_TX_HDCP1_4_KEY_SIZE  (280)

#define HDMI_TX_CEC_ENABLE_CTRL   (1)

/**
 * hdmi tx index
 * for MS1826 and MS1826B ,HDMI_TX_A/B/C/D are all used;
 * for MS1826A,Only HDMI_TX_C is used;
*/
typedef enum _HDMI_TX_INDEX_E
{
    HDMI_TX_A = 0,
    HDMI_TX_B = 1,
    HDMI_TX_C = 2,
    HDMI_TX_D = 3,
    HDMI_TX_NUM
} HDMI_TX_INDEX_E;

typedef enum _HDMI_AUDIO_SRC_SEL_
{
    FROM_HDMI_RX_AUDIO     = 0x00,
    FROM_I2S_PAD           = 0x01,
    FROM_SPDIF_PAD         = 0x02
}HDMI_AUDIO_SRC_SEL_E;

typedef enum _HDMI_VIDEO_CLK_SRC_SEL_
{
    FROM_RX_OR_PLL_PIXEL_CLK     = 0x00,
    FROM_RX_TMDS_CLK             = 0x01
}HDMI_VIDEO_CLK_SRC_SEL_E;

typedef struct _HDMI_TX_DEV_T
{
    UINT8 u8_txIndex;                                   /**< tx index @ref HDMI_TX_INDEX_E*/

    BOOL b_is_hdmi;                                     /**< MS_TRUE: hdmi false: dvi*/
    BOOL b_full_edid;                                   /**< 1:512byte edid 0:256byte edid*/
    UINT8 u8_color_depth;                               /**< color depth, @ref HDMI_COLORDEPTH_E*/
    UINT8 u8_audio_mode;                                /**< audio mode @ref HDMI_AUDIO_STREAM_TYPE_E*/
    UINT8 u8_audio_src;                                 /**< audio source select @HDMI_AUDIO_SRC_SEL_E*/
    UINT8 u8_video_clk_src;                             /**< video clk source select @HDMI_VIDEO_CLK_SRC_SEL_E, Bypass select FROM_RX_TMDS_CLK; others select FROM_RX_OR_PLL_PIXEL_CLK*/
    UINT32 u32_tmds_clk;                                /**< tx tmds clk 10khz*/
    UINT8 *pu8_edid_buf;                                /**< if b_full_edid = 1,512 byte array; if b_full_edid = 0,256 byte array; */
    UINT8 *pu8_bksv_buf;                                /**< 5 byte hdcp1.4 ksv*/
    UINT8 *pu8_key_buf;                                 /**< 280 byte hdcp1.4 key*/
    HDMI_EDID_FLAG_T*  pst_edid_info;
    HDMI_INFOFRAMES_PACKETS_T st_infoframes_packets;    /**< packets and infoframes  @ref HDMI_INFOFRAMES_PACKETS_T*/
} HDMI_TX_DEV_T;

/*
total ouput current     tx_symon     tx_traon   tx_trbon
0*i                     0            0          0
36*i                    1            0          0
36*i+3*i                1            0          1
36*i + 6*i              1            1          0
36*i + 6*i + 3*i        1            1          1
*/
typedef struct _HDMI_TX_DRI_PRE_PRE_EMPHASIS_T
{
    UINT8 u8_txIndex;                                    /**< tx index @ref HDMI_TX_INDEX_E*/
    UINT8 u8_tx_symon;
    UINT8 u8_tx_traon;
    UINT8 u8_tx_trbon;
}HDMI_TX_DRI_PRE_PRE_EMPHASIS_T;

/*
    Lower u8_sup_tx_lvl/u8_sup_ck_lvl values result in higher signal amplitudes, while higher TXLVL/CKLVL values
    result in lower signal amplitudes.
*/
typedef struct _HDMI_TX_VOLT_LEVEL_T
{
    UINT8 u8_txIndex;                                   /**< tx index @ref HDMI_TX_INDEX_E*/
    UINT8 u8_sup_tx_lvl;                                /**< tx_lvl[5:0], 3pair data channel volt*/
    UINT8 u8_sup_ck_lvl;                                /**< ck_lvl[5:0], 1pair clk channel volt*/
}HDMI_TX_VOLT_LEVEL_T;

/**
* @brief        HDMI_TX模块对象实例初始化
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_hdmi_tx_instance_init(HDMI_TX_DEV_T *pst_tx_dev, UINT8 e_tx_idx);

/**
* @brief        HDMI_TX模块初始化
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_hdmi_tx_init(HDMI_TX_DEV_T *pst_tx_dev);

/**
* @brief        HDMI_TX参数配置
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       None
*/
ERR_TYPE_E api_hdmi_tx_config_output(HDMI_TX_DEV_T *pst_tx_dev);

/**
* @brief        HDMI_TX audio参数配置,当audio 信息发生改变，需要重新配置audio
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       None
*/
ERR_TYPE_E api_hdmi_tx_audio_change(HDMI_TX_DEV_T *pst_tx_dev);

/**
* @brief        HDMI_TX 输出使能
* @param [in]   pst_tx_dev : TX模块对象指针
*               b_enable : enable or disable
* @return       None
*/
ERR_TYPE_E api_hdmi_tx_output_en(HDMI_TX_DEV_T *pst_tx_dev, BOOL b_enable);

/**
* @brief        HDMI_TX video mute
* @param [in]   pst_tx_dev : TX模块对象指针
*               b_enable : enable or disable mute
* @return       None
*/
ERR_TYPE_E api_hdmi_tx_video_mute(HDMI_TX_DEV_T *pst_tx_dev, BOOL b_enable);

/**
* @brief        HDMI_TX audio mute
* @param [in]   pst_tx_dev : TX模块对象指针
*               b_enable : enable or disable mute
* @return       None
*/
ERR_TYPE_E api_hdmi_tx_audio_mute(HDMI_TX_DEV_T *pst_tx_dev, BOOL b_enable);

/**
* @brief        HDMI_TX edid get
* @param [in]   pst_tx_dev : TX模块对象指针
*               p_u8_edid_buf : edid buff
*               pst_edid_info : edid info
*               b_full_edid : MS_TRUE represent 512byte; or represent 256 byte
* @return       BOOL : success or failed
*/
BOOL api_hdmi_tx_get_edid(HDMI_TX_DEV_T *pst_tx_dev, UINT8 *p_u8_edid_buf,HDMI_EDID_FLAG_T *pst_edid_info, BOOL b_full_edid);

/**
* @brief        HDMI_TX hpd get
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       BOOL : MS_TRUE represent hpd is high, or represent low
*/
BOOL api_hdmi_tx_get_hpd_status(HDMI_TX_DEV_T *pst_tx_dev);

/**
* @brief        HDMI_TX hdcp 初始化
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       BOOL : MS_TRUE represent init success; or failed
*/
BOOL api_hdmi_tx_hdcp14_init(HDMI_TX_DEV_T *pst_tx_dev);

/**
* @brief        HDMI_TX hdcp get satus
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       BOOL : MS_TRUE represent verify ok; or failed
*/
BOOL api_hdmi_tx_hdcp14_get_status(HDMI_TX_DEV_T *pst_tx_dev);

/**
* @brief        HDMI_TX hdcp enable, if api_hdmi_tx_hdcp14_get_status return success,then call api_hdmi_tx_hdcp14_enable
* @param [in]   pst_tx_dev : TX模块对象指针
* @return       None
*/
ERR_TYPE_E api_hdmi_tx_hdcp14_enable(HDMI_TX_DEV_T *pst_tx_dev,BOOL b_enable);

/**
* @brief        驱动电流设置
* @param [in]   HDMI_TX_DRI_PRE_PRE_EMPHASIS_T : TX模块对象指针
* @return       None
*/
VOID api_hdmi_tx_set_driver_pre_emphasis(HDMI_TX_DRI_PRE_PRE_EMPHASIS_T *pst_tx_driver_pre_emphasis);

/**
* @brief        驱动电压设置
* @param [in]   HDMI_TX_VOLT_LEVEL_T : TX模块对象指针
* @return       None
*/
VOID api_hdmi_tx_set_driver_volt_level(HDMI_TX_VOLT_LEVEL_T *pst_tx_volt_level);

#endif // __HDMI_RX_API_H__

/**
 * @}
 */
