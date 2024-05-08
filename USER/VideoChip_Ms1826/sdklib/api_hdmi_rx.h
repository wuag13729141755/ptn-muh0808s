/**
* @file  api_hdmi_rx.h
* @brief HDMI_RX模块相关数据结构及函数定义
*/

/**
 * @defgroup API_HDMI_RX hdmi rx module api
 *
 * @brief hdmi rx module spi
 * @{
 */


#ifndef __HDMI_RX_API_H__
#define __HDMI_RX_API_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"

#define HDMI_RX_EDID_SIZE         (256)  /**< defined edid size*/
#define HDMI_RX_HDCP1_4_BKSV_SIZE (5)    /**< defined hdcp1.4 bksv size*/
#define HDMI_RX_HDCP1_4_KEY_SIZE  (280)  /**< defined hdcp1.4 key size*/

/** hdmi rx interrupts*/
typedef struct _RX_INTERRUPTS_T
{
    /*Packet Decoder Interrupt*/
    BOOL b_pd_fifo_th_min_pass;
    BOOL b_pd_fifo_th_max_pass;
    BOOL b_pd_fifo_th_start_pass;
    BOOL b_pd_fifo_underflow;
    BOOL b_pd_fifo_overflow;
    BOOL b_pd_fifo_new_entry;
    BOOL b_ntscvbi_rcv;       /**<  NTSC VBI packet received */
    BOOL b_ntscvbi_cks_chg;   /**< NTSC VBI packet checksum change */
    BOOL b_amp_rcv;           /**< Audio multi stream packet received */
    BOOL b_amp_chg;
    BOOL b_vsi_rcv;           /**< VSI packet received*/
    BOOL b_gcp_rcv;           /**< GCP packet received*/
    BOOL b_acr_rcv;           /**< ACR packet received*/
    BOOL b_avi_rcv;           /**< AVI packet received*/
    BOOL b_aif_rcv;           /**< AIF packet received*/
    BOOL b_gmd_rcv;           /**< GMD packet received*/
    BOOL b_gcp_av_mute_chg;
    BOOL b_acr_cts_chg;
    BOOL b_acr_n_chg;
    BOOL b_avi_cks_chg;
    BOOL b_aif_cks_chg;
    BOOL b_gmd_chs_chg;
    BOOL b_vsi_cks_chg;
    BOOL b_dvidet;
    BOOL b_aud_type_chg;
    BOOL b_drm_rcv;
    BOOL b_drm_cks_chg;

    /*Audio Interrupt*/
    BOOL b_ctsn_cnt; //CTS/N updated
    BOOL b_aud_rcv;
    BOOL b_aud_lost;

    /*Audio FIFO Interrupt*/
    BOOL b_afif_th_min;
    BOOL b_afif_th_max;
    BOOL b_afif_th_pass;
    BOOL b_afif_underflow;
    BOOL b_afif_overflow;

    /*Mode Detection Interrupt*/
    BOOL b_hs_act;
    BOOL b_vs_act;
    BOOL b_de_activity; /**< Data enable active */
    BOOL b_ilace;       /**< Interlaced mode active */
    BOOL b_htot32_clk;  /**< htot32_clk_ists interrupt threshold passed */
    BOOL b_hs_clk;
    BOOL b_hact_pix;
    BOOL b_vtot_clk;
    BOOL b_vs_clk;
    BOOL b_vact_lin;
    BOOL b_vtot_lin;
    BOOL b_vofs_lin;
    BOOL b_rx_timing_stb;
    BOOL b_rx_timing_unstb;

    /*HDMI Interrupt*/
    BOOL b_freq_unlock; //rx pll lock
    BOOL b_freq_lock;   //rx pll unlock
    BOOL b_clk_change;  //HDMI line clock rate changed and is stable
    BOOL b_hs_pol_adj;
    BOOL b_vs_pol_adj;
    BOOL b_ctl0_change;
    BOOL b_ctl1_change;
    BOOL b_ctl2_change;
    BOOL b_ctl3_change;
    BOOL b_dcm_gcp_zero_fields_pass;
    BOOL b_dcm_ph_diff_cnt_overfl;
    BOOL b_dcm_current_mode_chg; //Deep color mode changed
    BOOL b_scdc_cfg_change;
    BOOL b_scdc_scramb_sts_change;
    BOOL b_scdc_tmds_cfg_change;
    BOOL b_ceavid_fifo_full;
    BOOL b_ceavid_fifo_empty;
    BOOL b_aksv_rcv;
    BOOL b_vsync_act_edge;
    BOOL b_vs_thr_reached;
    BOOL b_i2cmpdone; //I2C master PHY done
    BOOL b_i2cmpnack; //I2C master PHY NACK
    BOOL b_i2cmp_apblost; //I2C master PHY arbitration lost

    /*CBUS interrupt*/

} RX_INTERRUPTS_T;

/** hdmi rx packet status*/
typedef struct _RX_PDEC_STS_T
{
    /*Packet Decoder Status*/
    BOOL b_drm_cks_chg;
    BOOL b_drm_rcv;
    BOOL b_ntscvbi_cks_chg;
    BOOL b_dvidet;
    BOOL b_vsi_cks_chg;
    BOOL b_gmd_cks_chg;
    BOOL b_aif_cks_chg;
    BOOL b_avi_cks_chg;
    BOOL b_acr_n_chg;
    BOOL b_acr_cts_chg;
    BOOL b_gcp_av_mute_chg;
    BOOL b_gmd_rcv;
    BOOL b_aif_rcv;
    BOOL b_avi_rcv;
    BOOL b_acr_rcv;
    BOOL b_gcp_rcv;
    BOOL b_vsi_rcv;
    BOOL b_amp_rcv;
    BOOL b_ntscvbi_rcv;
    BOOL b_obr_layout;
    BOOL b_auds_layout;
    BOOL b_pd_fifo_new_entry;
    BOOL b_pd_fifo_voerfl;
    BOOL b_pd_fifo_underfl;
    BOOL b_pd_fifo_th_start_pass;
    BOOL b_pd_fifo_th_max_pass;
    BOOL b_pd_fifo_th_min_pass;

    /*Packet Decoder Audio Status*/
    BOOL b_mas_rcv;
    BOOL b_obm_rcv;
    BOOL b_hbr_rcv;
    BOOL b_dst_rcv;
    BOOL b_oba_rcv;
    BOOL b_auds_rcv;
    BOOL b_aduio_present;

    /*Audio fifo status*/
    BOOL b_afif_th_min;
    BOOL b_afif_th_max;
    BOOL b_afif_th_pass;
    BOOL b_afif_underflow;
    BOOL b_afif_overflow;
} RX_PDEC_STS_T;

/** values or status of hdmi configurations*/
typedef struct _RX_HDMI_STS_T
{
    UINT8 u8_color_depth_cur;
    UINT8 u8_last_pixel_phase;
    UINT8 u8_phase_diff_cnt;
    BOOL  b_phase_diff_cnt_overflow;
    BOOL  b_gcp_zero_fileds_passed;
    BOOL  b_ctl3_set;
    BOOL  b_ctl2_set;
    BOOL  b_ctl1_set;
    BOOL  b_ctl0_set;
    BOOL  b_vs_pol_adj_active;
    BOOL  b_hs_pol_adj_active;
    UINT8 u8_dcm_cur_mode_sts;
    UINT8 u8_packing_phase_cur;
} RX_HDMI_STS_T;

/** status of mode detect */
typedef struct _RX_MDT_STS_T
{
    BOOL b_hs_act_sts;
    BOOL b_vs_act_sts;
    BOOL b_de_act_sts;
    BOOL b_ilace_sts;
    BOOL b_hactive_stable;
    BOOL b_hblank_stable;
    BOOL b_hs_width_stable;
    BOOL b_hfront_stable;
    BOOL b_vactive_stable;
    BOOL b_vblank_stable;
    BOOL b_vs_width_stalbe;
} RX_MDT_STS_T;

/**
 * hdmi rx index
 * for MS1826 and MS1826A ,HDMI_RX_A/B/C/D are all used;
 * for MS1826B,Only HDMI_RX_A is used;
*/
typedef enum _HDMI_RX_INDEX_E
{
    HDMI_RX_A = 0,  /**< hdmi rx1*/
    HDMI_RX_B = 1,  /**< hdmi rx2*/
    HDMI_RX_C = 2,  /**< hdmi rx3*/
    HDMI_RX_D = 3,  /**< hdmi rx4*/
    HDMI_RX_NUM    /**< hdmi rx numbers*/
} HDMI_RX_INDEX_E;

/** hdmi rx state*/
typedef enum _HDMI_RX_STATE_E
{
    RX_STATE_CABLE_PLUGOUT = 0,   /**< cable plugout */
    RX_STATE_CABLE_PLUGIN,        /**< cable plugin */
    RX_STATE_POWER_UP,            /**< module reset */
    RX_STATE_CONFIG_EDID,         /**< config edid */
    RX_STATE_CONFIG_HDCP,         /**< config hdcp */
    RX_STATE_CONFIG_PHY,          /**< config phy */
    RX_STATE_CONFIG_CONTROLLER,   /**< config controller */
    RX_STATE_RUNNING,             /**< hdmi rx is config ok */
    RX_STATE_VIDEO_CHANGED,       /**< video has changed */
    RX_STATE_AUDIO_CHANGED,       /**< audio has changed */
    RX_STATE_NUM                  /**< hdmi rx state numbers*/
} HDMI_RX_STATE_E;

/** hdmi rx callback function type */
typedef void (*HDMI_RX_CALLBACK_FUNCTION)(void *pInstance);

/** hdmi rx device type*/
typedef struct _HDMI_RX_DEV_T
{
    UINT8 idx;                                       /**< hdmi rx index @ref HDMI_RX_INDEX_E*/
    UINT8 u8_rx_state;                                     /**< hdmi rx current state @ref HDMI_RX_STATE_E*/

    UINT8 u8_package_type;                                 /**< chip package type @ref MS1826_PACKAGE_TYPE_E*/
    BOOL b_is_connected;                                   /**< hdmi rx cable is connected or not*/
    BOOL b_5vdet_override_enable;                          /**< ignore 5v detect enable*/
    BOOL b_is_hdmi;                                        /**< is hdmi signal or dvi signal*/
    BOOL b_hdcp1_4_enable;                                 /**< hdcp1.4 function enable*/
    BOOL b_timing_stable;                                  /**< hdmi rx mode detect timing stable*/
    BOOL b_video_changed;                                  /**< video changed happened*/
    BOOL b_audio_changed;                                  /**< audio changed happened*/
    BOOL b_hdp_override_en;                                /**< hpd override enable*/
    BOOL b_audio_pll_manu_cfg;                             /**< rx audio pll manual config*/


    UINT8 *pu8_edid_buf;                                   /**< edid buffer pointer*/
    UINT16 u16_edid_size;                                  /**< edid buffer size*/
    UINT8 *pu8_bksv_buf;                                   /**< hdcp1.4 bksv buffer pointer*/
    UINT8 *pu8_key_buf;                                    /**< hdcp1.4 rx key buffer pointer*/

    UINT8 u8_vic;                                          /**< hdmi vic code*/
    UINT16 u16_tmds_clk;                                   /**< hdmi rx video tmds clock, uint: 10kHz*/
    UINT8 u8_color_depth;                                  /**< color depth, @ref HDMI_COLORDEPTH_E*/
    UINT8 u8_color_space;                                  /**< color space, @ref HDMI_COLORSPACE_E*/

    /*packets and infoframes*/
    HDMI_INFOFRAMES_PACKETS_T st_packets_infoframes;       /**< hdmi packets and infoframes */
    UINT8 u8_avi_info_raw[24];                             /**< avi infoframe raw data, PB0-PB13*/
    UINT8 u8_vsi_info_raw[14];                             /**< vsi infoframe raw data, PB0-PB23*/
    UINT8 u8_aud_info_raw[6];                              /**< audio infoframe raw data, PB0-PB5*/

    /*callback functions*/
    HDMI_RX_CALLBACK_FUNCTION power_up_reset_callback;     /**< user defined: do something in power up*/
    HDMI_RX_CALLBACK_FUNCTION video_changed_callback;      /**< user defined: do something in video changed */
    HDMI_RX_CALLBACK_FUNCTION audio_changed_callback;      /**< user defined: do something in audio changed*/

    /*rx hardware state*/
    RX_INTERRUPTS_T     st_rx_ints;                        /**< hdmi rx hardware interrupts*/
    HDMI_VIDEO_TIMING_T st_rx_video_timing;                /**< hdmi rx video timing parse from hardware*/
    RX_PDEC_STS_T       st_rx_pdec_sts;                    /**< hdmi rx packet decoder status */
    RX_HDMI_STS_T       st_rx_hdmi_sts;                    /**< values or status of HDMI rx configurations*/
    RX_MDT_STS_T        st_rx_mdt_sts;                     /**< mode detect status*/

} HDMI_RX_DEV_T;

/**
* @brief        HDMI_RX模块初始化配置
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_hdmi_rx_init(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        HDMI_RX模块对象实例初始化
* @param [in]   pst_rx_dev : RX模块对象指针
* @param [in]   idx  : hdmi rx index
* @note         该接口仅对RX模块指针对象实例初始化，如结构体清零及赋初始默认值，并没有对模块做初始化动作。
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if init successed
*/
ERR_TYPE_E api_hdmi_rx_instance_init(HDMI_RX_DEV_T *pst_rx_dev, UINT8 idx);

/**
* @brief        HDMI_RX模块配置FSM
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_config(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        HDMI_RX模块中断状态更新（获取状态），具体可参考结构体:
*               #RX_INTERRUPTS_T \n
*               #RX_PDEC_STS_T \n
*               #RX_HDMI_STS_T \n
*               #RX_MDT_STS_T \n
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_update_state(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        HDMI_RX模块中断状态处理
* @param [in]   pst_rx_dev : RX模块对象指针
* @note         对中断状态的分发处理
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_isr(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        HDMI_RX模块自动配置任务
* @param [in]   pst_rx_dev : RX模块对象指针
* @note         需要周期性调用
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_task(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        配置更新EDID
* @param [in]   pst_rx_dev : RX模块对象指针
* @param [in]   pu8_edid_buff : EDID buff指针
* @param [in]   pu8_edid_buff : EDID buff大小
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_edid_update(HDMI_RX_DEV_T *pst_rx_dev, UINT8 *pu8_edid_buff, UINT16 u16_edid_size);

/**
* @brief        获取HIDMI_RX模块解析到的avi infoframe
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_get_avi_infoframe(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取HIDMI_RX模块解析到的aud infoframe
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_get_aud_infoframe(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取HIDMI_RX模块解析到的drm infoframe
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_get_raw_drm_infoframe(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取HIDMI_RX模块解析到的gcp infoframe
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_get_gcp_packet(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取HIDMI_RX模块解析到的timing信息
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       ERR_INV_PARM if input pointer is null\n
*               MS_ERR_OK if config ok
*/
ERR_TYPE_E api_hdmi_rx_get_video_timing(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取video是否发生改变
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       MS_TRUE if video changed\n
*/
BOOL api_hdmi_rx_is_video_changed(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取audio是否发生改变
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       MS_TRUE if audio changed\n
*/
BOOL api_hdmi_rx_is_audio_changed(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        获取avi\vsi\audio infoframe raw data
* @param [in]   pst_rx_dev : RX模块对象指针
* @return       MS_TRUE if get successed\n
*/
BOOL api_hdmi_rx_get_avi_vsi_aud_infoframe_raw_data(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief        Set hpd output high level
* @param [in]   pst_rx_dev : RX模块对象指针
* @param [in]   b_set_high : MS_TRUE is high
* @return       NULL\n
*/
void api_hdmi_rx_set_hpd(HDMI_RX_DEV_T *pst_rx_dev, BOOL b_set_high);

/**
* @brief        Set hdmi rxphy EQ gain, each channel EQ range is from 0 to 7
* @param [in]   pst_rx_dev : RX模块对象指针
* @param [in]   u8_eq_gain_ch0 : channel0 eq setting
* @param [in]   u8_eq_gain_ch1 : channel1 eq setting
* @param [in]   u8_eq_gain_ch2 : channel2 eq setting
* @param [in]   b_manual_eq_en : enable manual EQ or not
* @return       NULL\n
*/
void api_hdmi_rx_manual_eq(HDMI_RX_DEV_T *pst_rx_dev, UINT8 u8_eq_gain_ch0, UINT8 u8_eq_gain_ch1, UINT8 u8_eq_gain_ch2, BOOL b_manual_eq_en);

/**
* @brief        Reconfigure RX audio
* @param [in]   pst_rx_dev : RX模块对象指针,releated para:idx
* @return
*/
void api_hdmi_rx_audio_reconfig(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief    set hdmi rx hdmi interrupt to int pin or not
* @param [in]   pst_rx_dev : releated para 1:idx 2:st_rx_ints(from b_freq_unlock to b_i2cmp_apblost)
* @return
*/
void api_hdmi_rx_hdmi_ints_cfg(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief    clear hdmi rx hdmi interrupt
* @param [in]   pst_rx_dev : releated para 1:idx 2:st_rx_ints(from b_freq_unlock to b_i2cmp_apblost)
* @return
*/
void api_hdmi_rx_hdmi_ints_clr(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief    set hdmi rx mdt interrupt to int pin or not
* @param [in]   pst_rx_dev : releated para 1:idx 2:st_rx_ints(from b_hs_act to b_rx_timing_unstb)
* @return
*/
void api_hdmi_rx_mdt_ints_cfg(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief    clear hdmi rx mdt interrupt
* @param [in]   pst_rx_dev : releated para 1:idx 2:st_rx_ints(from b_hs_act to b_rx_timing_unstb)
* @return
*/
void api_hdmi_rx_mdt_ints_clr(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief    set hdmi rx pdec interrupt to int pin or not
* @param [in]   pst_rx_dev : releated para 1:idx 2:st_rx_ints(from b_pd_fifo_th_min_pass to b_drm_cks_chg)
* @return
*/
void api_hdmi_rx_pdec_ints_cfg(HDMI_RX_DEV_T *pst_rx_dev);

/**
* @brief    clear hdmi rx pdec interrupt
* @param [in]   pst_rx_dev : releated para 1:idx 2:st_rx_ints(from b_pd_fifo_th_min_pass to b_drm_cks_chg)
* @return
*/
void api_hdmi_rx_pdec_ints_clr(HDMI_RX_DEV_T *pst_rx_dev);

#endif // __HDMI_RX_API_H__


/**
 * @}
 */
