#ifndef _MS1826_APP_H
#define _MS1826_APP_H

#include "ms1826_api.h"
#include "ms1826_timing_table.h"
#include "ms1826_font.h"
//#include "mapp_GlobalStruct.h"

#ifndef DEV_NUM_MS1826
#define DEV_NUM_MS1826      1
#endif

#ifndef DEV_MS1826_WORK_MODE
#define DEV_MS1826_WORK_MODE      MATRIX_SWITCHER
#endif

#define MS1826_4X4DEMO_SW_VERSION   "MS1826_HDMI4X4_Demo_V1.2"

#ifndef SEAMLESS_MATRIX_USE_DSC
#define SEAMLESS_MATRIX_USE_DSC         0
#endif

#ifndef MS1826_OSD_ENABLE
#define MS1826_OSD_ENABLE               0
#endif

#ifndef MS1826_OSD_MAX_LEN
#define MS1826_OSD_MAX_LEN              15
#endif

#ifndef MS1826_RX_SUPPORT_ONLY_PCM
#define MS1826_RX_SUPPORT_ONLY_PCM      FALSE
#endif

#ifndef MS1826_RX_SUPPORT_INTERLACE
#define MS1826_RX_SUPPORT_INTERLACE     TRUE
#endif

#ifndef MS1826_USER_OSD_STRING_ENABLE
#define MS1826_USER_OSD_STRING_ENABLE   1
#endif

#ifndef MS1826_VIPSS1_DEINT
#define MS1826_VIPSS1_DEINT             0
#endif

#ifndef ENABLE_MS1826_FRC_MODE_SWITCH
#define ENABLE_MS1826_FRC_MODE_SWITCH   1
#endif

#ifndef ENABLE_MS1826_FRAME_LOCK
#define ENABLE_MS1826_FRAME_LOCK       0
#endif

#ifndef ENABLE_MS1826_OUT_MODE_RELOAD_EDID
#define ENABLE_MS1826_OUT_MODE_RELOAD_EDID  0
#endif

#ifndef MS1826_PATTERN_SWITCH_ENABLE
#define MS1826_PATTERN_SWITCH_ENABLE        1
#endif

#ifndef MS1826_SPLITTER_USER_MODE_ENABLE
#define MS1826_SPLITTER_USER_MODE_ENABLE    0
#endif

#ifndef MS1826_SPLITTER_OSD_ENABLE
#define MS1826_SPLITTER_OSD_ENABLE          0
#endif

#ifndef MS1826_RX_MANUAL_EQ_ENABLE
#define MS1826_RX_MANUAL_EQ_ENABLE          0
#endif

#ifndef MS1826_RX_EQ_VALUE_CH1
#define MS1826_RX_EQ_VALUE_CH1              0
#endif

#ifndef MS1826_RX_EQ_VALUE_CH2
#define MS1826_RX_EQ_VALUE_CH2              0
#endif

#ifndef MS1826_RX_EQ_VALUE_CH3
#define MS1826_RX_EQ_VALUE_CH3              0
#endif

#ifndef MS1826_TX_DRIVER_USER_CONFIG_ENABLE
#define MS1826_TX_DRIVER_USER_CONFIG_ENABLE     0
#endif

#ifndef MS1826_DRIVER_VALUE_TX_LEVEL
#define MS1826_DRIVER_VALUE_TX_LEVEL        (0)
#endif

#ifndef MS1826_DRIVER_VALUE_CLOCK_LEVEL
#define MS1826_DRIVER_VALUE_CLOCK_LEVEL     (0x18)
#endif

//extern int dbg_printf(const char *fmt, ...);
extern int ms1826_printf(const char *fmt, ...);
extern int ms1826_printfwu(const char *fmt, ...);

#if 1
#define log   ms1826_printf//dbg_printf
#define p   ms1826_printfwu//dbg_printf

#else
#define log(x)   //dbg_printf
#endif

typedef enum E_MS1826_WORKING_MODE_
{
	SPLITTER               = 0,
	SPLICER                = 1,
	MATRIX_SWITCHER     = 2,
	MS1826_BYPASS_MATRIX_SWITCHER       = 3,
	MATRIX_SWITCHER_OUT = 4,

}MS1826_WORKING_MODE;

typedef enum E_MS1826_PICTURE_ADJUST_
{
	MS1826_ADJUST_BRIGHTNESS            = 0x01,
	MS1826_ADJUST_CONTRAST              = 0x02,
	MS1826_ADJUST_SATURATION            = 0x04,
	MS1826_ADJUST_HUE                   = 0x08,
	MS1826_ADJUST_SHARPNESS             = 0x10,
	MS1826_ADJUST_NOISE                 = 0x20,
}MS1826_PICTURE_ADJUST;

typedef enum E_MS1826_TX_FORMAT_
{
	MS1826_TX_DVI                       = 0,
	MS1826_TX_HDMI                      = 1,
}MS1826_TX_FORMAT;

typedef enum E_MS1826_AUDIO_SOURCE_TYPE_
{
	MS1826_AUDIO_SRC_HDMI1            = 0x00,
	MS1826_AUDIO_SRC_HDMI2,
	MS1826_AUDIO_SRC_HDMI3,
	MS1826_AUDIO_SRC_HDMI4,
	MS1826_AUDIO_SRC_DIGITAL_I2S1,
	MS1826_AUDIO_SRC_DIGITAL_I2S2,
	MS1826_AUDIO_SRC_DIGITAL_I2S3,
	MS1826_AUDIO_SRC_DIGITAL_I2S4,
	MS1826_AUDIO_SRC_DIGITAL_SPDIF1,
	MS1826_AUDIO_SRC_DIGITAL_SPDIF2,
	MS1826_AUDIO_SRC_DIGITAL_SPDIF3,
	MS1826_AUDIO_SRC_DIGITAL_SPDIF4,
}MS1826_AUDIO_SOURCE_TYPE;

typedef struct _T_MS_VIDEO_SIZE_
{
	UINT16 u16_h;
	UINT16 u16_v;
} VIDEOSIZE_T;

typedef struct _T_SCALE_SIZE_
{
	VIDEOSIZE_T tInSize;
	VIDEOSIZE_T tOutSize;
	VIDEOSIZE_T tMemSize;
} SCALE_SIZE_T;

typedef struct _T_WIN_BORDER_
{
    INT16 top;
    INT16 bottom;
    INT16 left;
    INT16 right;
} WINBORDER_T;

typedef struct _stMs1826Wincfg_T_
{
    UINT8 u8Rx;     // input signal select
    UINT8 u8Tx;     // output port select
    UINT16 u16X;    // win start x
    UINT16 u16Y;    // win start y
    UINT16 u16W;    // win active width
    UINT16 u16H;    // win active height
}stMs1826Wincfg_T;

typedef struct _stMs1826FunctionEvent_t
{
    BOOL bPortChangeFlag[4];
    u8 u8OutputSel[4];

    BOOL bFreezeChangeFlag[4];
    u8 u8FreezeSta[4];

    BOOL bTxResolutionChangeFlag[4];
    u8 u8TxResolutionID[4];

    BOOL bBrightnessAdjustFlag[4];
    u8 u8Brightness[4];
    BOOL bContrastAdjustFlag[4];
    u8 u8Contrast[4];
    BOOL bSaturationAdjustFlag[4];
    u8 u8Saturation[4];
    BOOL bHueAdjustFlag[4];
    u8 u8Hue[4];

    BOOL bRxEdidChangeFlag[4];
    u8 u8EdidBuf[4][256];

    BOOL bFreezeSwitchFlag[4];
    u16 u16FreezeDelayTime;
    BOOL bFreezeTimeEnd;

    BOOL bTxCfgChangeFlag[4];
    u8 u8TxFormat[4];
    u8 u8TxColorSpace[4];

    BOOL bTxHdcp14ChangeFlag[4];
    u8 u8TxHdcp14Sta[4];

    BOOL bReadTxEdidFlag[4];

    BOOL bTxDigitalAudioChangeFlag[4];
    u8 u8TxDigitalAudioSta[4];
    u8 u8TxDigitalAudioSource[4];

    BOOL bTxAudioSourceChangeFlag[4];
    u8 u8TxAudioSourceSta[4];

    BOOL bTxPatternChangeFlag[4];
    u8 u8TxPatternID[4];
    u8 u8TxPatternEnable[4];

    BOOL bColorChangeFlag[4];
    u8 u8ColorEnable[4];
    u8 u8RedValue[4];
    u8 u8GreenValue[4];
    u8 u8BlueValue[4];

    BOOL bTxTimingChangeFlag[4];
    stTimingTable_t stTiming[4];

    BOOL bSplitChangeFlag[4];
    u8 u8SplitEnable[4];
    SPLIT_POSITION_T stSplitPosision[4];

    BOOL bBypassPortChangeFlag[4];
    u8 u8BypassOutputSel[4];

    BOOL bWorkModeSwitch;
    u8 u8WorkMode;
    u8 u8SubMode;

    BOOL bOsdStringChangeFlag[4];
    u8 u8OsdStr[4][MS1826_OSD_MAX_LEN];
    BOOL bOsdStringIsAlignRight[4];

    BOOL bOsdStringDisplayChangeFlag[4];
    BOOL bOsdStringDisplayEnable[4];

    BOOL bOsdStringColorChangeFlag[4];
    u8 bOsdStringColorRed[4];
    u8 bOsdStringColorGreen[4];
    u8 bOsdStringColorBlue[4];

    BOOL bOsdStringPositionChangeFlag[4];
    u16 u16OsdStringPositionX[4];
    u16 u16OsdStringPositionY[4];

    BOOL bVideoMuteChangeFlag[4];
    BOOL bVideoMuteSta[4];

    BOOL bTxHdmiAudioMuteChangeFlag[4];
    u8 u8TxHdmiAudioMuteSta[4];

    BOOL bSplicerChangeFlag[4];
    stMs1826Wincfg_T stSplicerVar[4];
    BOOL bSplicerEnable[4];

#if ENABLE_MS1826_FRAME_LOCK
    BOOL bTxFrameLockChangeFlag[4];
    u8 u8TxFrameLockRxSelect[4];
#endif
    BOOL bSplitterModeChangeFlag;
    u8 u8SplitterMode;

    BOOL bSplitUpdateChangeFlag;
    u8 u8SplitPath;
}stMs1826FunctionEvent_t;

extern UINT8 const g_u8_rx_idx[4];
extern UINT8 const g_u8_csc_idx[4];
extern UINT8 const g_u8_misc_module_rx[4];
extern UINT8 const g_u8_4ksdn_idx[4];
extern UINT8 const g_u8_sdn_idx[4];
extern UINT8 const g_u8_dsc_idx[4];
extern UINT8 const g_u8_dma_idx[4];
extern UINT8 const g_u8_dma_dsc_idx[4];
extern UINT8 const g_u8_misc_module_vipss[4];
extern UINT8 const g_u8_misc_module_vopss[4];
extern UINT8 const g_u8_su_idx[4];
extern UINT8 const g_u8_su_dsc_idx[4];
extern UINT8 const g_u8_mixer_idx[4];
extern UINT8 const g_u8_mixer_layer_src[4];
extern UINT8 const g_u8_mixer_layer_dsc_src[4];
extern UINT8 const g_u8_misc_mixer[4];
extern UINT8 const g_u8_misc_timgen[4];
extern UINT8 const g_u8_misc_su[4];
extern UINT8 const g_u8_misc_dsc_su[4];
extern UINT8 const g_u8_misc_module_tx[4];
extern UINT8 const g_u8_split_idx[4];
extern UINT8 const g_u8_ss_ch_idx[4];
extern UINT8 const g_u8_ss_ch_dsc_idx[4];
extern UINT8 const g_u8_vb_path[4];
extern UINT8 const g_u8_vb_dsc_path[4];
extern UINT8 const g_u8_vb_rx[4];
extern UINT8 const g_u8_vb_vdstmg[4];
extern UINT8 const g_u8_intsrc_rx[4];
extern UINT8 const g_u8_intsrc_rxvb[4];
extern UINT8 const g_u8_misc_clksrc[4];
extern UINT8 const g_u8_osd_idx[4];
extern UINT8 const g_u8_misc_osd_idx[4];
extern UINT8 const g_u8_i2s_idx[4];
extern UINT8 const g_u8_spdif_idx[4] ;
extern UINT8 const g_u8_i2s_audio_idx[4];
extern UINT8 const g_u8_spdif_audio_idx[4] ;
extern UINT8 const g_u8_i2s_audio[4];
extern UINT8 const g_u8_spdif_audio[4];


extern UINT8 const g_arrOutputTable[6];
extern UINT8 const g_u8_HDMI_RX_PROT[4];
extern UINT8 const g_u8_HDMI_TX_PROT[4];

extern UINT16 const str_osd_input_channel[4][8];
extern UINT16 const str_osd_input_channel_audio[4][8];
extern UINT16 const str_osd_output_channel[4][5];
extern char const str_osd_res[3][15];

extern HDMI_RX_DEV_T g_st_hdmi_rx[DEV_NUM_MS1826][4];
extern HDMI_VIDEO_TIMING_T	Video_HDMIRx_Timing[DEV_NUM_MS1826][4];
extern MISC_POLARITY_T	g_st_rx_polarity[DEV_NUM_MS1826][4];
extern CSC_DEV_T g_st_vipss_csc[DEV_NUM_MS1826][4];
extern BOOL b_RxVideoChg[DEV_NUM_MS1826][4];
extern UINT8 wdma_buf_id[DEV_NUM_MS1826];
extern UINT8 rdma_buf_id[DEV_NUM_MS1826];

extern HDMI_TX_DEV_T g_st_hdmi_tx[DEV_NUM_MS1826][4];
extern HDMI_VIDEO_TIMING_T	g_stVideo_HDMITx_Timing[DEV_NUM_MS1826][4];
extern MISC_TX_MUX_T g_st_tx_mux[DEV_NUM_MS1826][4];
extern CSC_DEV_T g_st_txcsc_dev[DEV_NUM_MS1826][4];
extern VDS_TMG_DEV_T g_st_timgen[DEV_NUM_MS1826][4];
extern UINT8 hdmi_tx_edid[DEV_NUM_MS1826][4][256];
extern HDMI_EDID_FLAG_T g_hdmi_edid_flag[DEV_NUM_MS1826][4];
extern BOOL b_hpd_bak[DEV_NUM_MS1826][4];

extern UINT8 display_mode[DEV_NUM_MS1826];
extern UINT8 display_mode_bak[DEV_NUM_MS1826];
extern UINT8 g_u8_output_index[DEV_NUM_MS1826][4];
extern UINT8 Matrix_Tx_Mapping[DEV_NUM_MS1826][4];
extern UINT8 Mapp_Rx[DEV_NUM_MS1826][4];
extern UINT8 Bypass_Tx_Status[DEV_NUM_MS1826][4];
extern UINT8 u8_Tx_audio_sel[DEV_NUM_MS1826][4];
extern UINT8 g_u8TxFormat[DEV_NUM_MS1826][4];
extern UINT8 g_u8SplitterDispMapping[DEV_NUM_MS1826][4];

extern DEINT_DEV_T g_st_deint_dev[DEV_NUM_MS1826];

extern OSD_DEV_T g_st_osd_dev[DEV_NUM_MS1826][4];
extern MISC_OSD_MUX_T g_st_misc_osd_mux[DEV_NUM_MS1826][4];
extern OSD_WINSIZE_T st_osd_win_size[DEV_NUM_MS1826];
extern OSD_STRING_T st_str[DEV_NUM_MS1826];
extern OSD_FONT_T st_font[DEV_NUM_MS1826];
extern char p_str[15];
extern BOOL OSDShown[DEV_NUM_MS1826];
extern UINT8 OSDCount[DEV_NUM_MS1826];
extern BOOL g_user_osd_str_enable[DEV_NUM_MS1826][4];
extern UINT8 g_user_osd_string_color[DEV_NUM_MS1826][4][3];
extern OSD_WINSIZE_T st_user_osd_win_size[DEV_NUM_MS1826][4];
extern UINT16 g_u16UserStrDispPos[DEV_NUM_MS1826][4][2];

extern SPLIT_DEV_T g_st_split_var[DEV_NUM_MS1826][4];

extern HDMI_INFOFRAMES_PACKETS_T gst_inf_pack[DEV_NUM_MS1826];

extern VIDEO_MIXER_DEV_T g_st_video_mixer_dev[DEV_NUM_MS1826][4];
extern VIDEO_MIXER_LAYER_SEL_T g_st_mixer_layer_sel[DEV_NUM_MS1826];
extern VIDEO_MIXER_ALF_CONFIG_T g_st_mixer_alf_cfg[DEV_NUM_MS1826];
extern VIDEO_MIXER_COLOR_T g_st_bg_color;
extern UINT8 g_u8_output_index_bak[DEV_NUM_MS1826][4];

extern VEHC_DEV_T g_st_vivehc[DEV_NUM_MS1826][4];
extern VEHC_DEV_T g_st_vovehc[DEV_NUM_MS1826][4];

extern BOOL b_audio_mute[DEV_NUM_MS1826][4];
extern BOOL b_digital_audio_mute[DEV_NUM_MS1826][4];
extern UINT16 g_u16_timer_out;
extern stMs1826FunctionEvent_t Ms1826event[DEV_NUM_MS1826];
extern UINT8 g_u8MS1826DevIndex;
extern BOOL g_bSplicerEnable[DEV_NUM_MS1826][4];
extern BOOL g_bSplitterEnable[DEV_NUM_MS1826][4];
#if ENABLE_MS1826_FRAME_LOCK
extern UINT8 g_u8FrameLockRxTable[DEV_NUM_MS1826][4];
#endif
extern BOOL g_bPreTimingStable[DEV_NUM_MS1826][4];
extern BOOL g_bDigitalAudioSwByHdmiTx[DEV_NUM_MS1826][4];

void app_ms1826_init(UINT8 u8dev,UINT8 u8WorkMode);
UINT16 return_minof_three(UINT16 u16dat1, UINT16 u16dat2, UINT16 u16dat3);
UINT16 return_com_multiple(UINT16 u16dat, UINT8 u8_data);
VOID app_video_vipss_trig(UINT8 u8dev,UINT8 u8_vipss_sel);
VOID app_video_vopss_trig(UINT8 u8dev,UINT8 u8_vopss_sel);
VOID app_tx_audio_cfg(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_tx);
VOID app_digital_audio_cfg(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_digital_audio_id, BOOL b_status);
VOID app_bcsh_adjust(UINT8 u8dev,UINT8 u8_vehc_idx);
BOOL video_ms1826_reset(UINT8 u8dev,UINT8 u8_dis_cur, UINT8 u8_dis_bak);
VOID app_digital_input_hdmi_output_cfg(UINT8 u8dev,UINT8 g_u8_digital_idx,UINT8 u8_tx);
VOID app_hdmi_input_hdmi_output_cfg(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_tx);
VOID app_mode_switch_cfg(UINT8 u8dev,UINT8 u8Mode,UINT8 u8SubMode);
VOID display_mode_cfg(UINT8 u8dev,UINT8 u8Mode,UINT8 u8SubMode);


VOID app_ms1826_picture_adjust(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_value,u8 u8_adjust_mode);
void app_ms1826_colortemp_adjust(UINT8 u8dev,BOOL b_en,UINT8 u8_vehc_idx, UINT8 u8_r_gain, UINT8 u8_g_gain, UINT8 u8_b_gain);
void app_ms1826_output_resolution(UINT8 u8dev,UINT8 u8TxID,UINT8 u8_vic);
void app_ms1826_rx_edid_update(UINT8 u8dev,UINT8 u8RxID,UINT8 *pu8EdidBuf,UINT16 u16Size);
void app_ms1826_tx_format_config(UINT8 u8dev,UINT8 u8TxID, BOOL bIsHDMI, UINT8 u8ColorSpace);
void app_ms1826_tx_hdcp14_enable(UINT8 u8dev,UINT8 u8TxID, BOOL bEn);
void app_ms1826_tx_get_edid(UINT8 u8dev,UINT8 u8TxID,UINT8 *pu8Edidbuf);
void app_ms1826_tx_timing_config(UINT8 u8dev,UINT8 u8TxID,pstTimingTable_t pstTiming);
UINT16 app_ms1826_get_display_horizontal(UINT8 u8dev,UINT8 u8TxID);
UINT16 app_ms1826_get_display_vertical(UINT8 u8dev,UINT8 u8TxID);
UINT16 app_ms1826_get_display_frame_rate(UINT8 u8dev,UINT8 u8TxID);
void app_ms1826_audio_input_switch(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_tx);
void app_ms1826_audio_output_mute(UINT8 u8dev,UINT8	u8_audio_src, UINT8 u8_tx, BOOL bIsMute);
void app_ms1826_digital_output_hdmi_input_cfg(UINT8 u8dev,UINT8	u8_audio_src, UINT8 u8_tx);
void app_ms1826_pattern_config(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_pattern_id,BOOL b_enable);
void app_ms18226_rx_pattern_config(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_pattern_id,BOOL b_enable);
UINT8 app_ms1826_get_tx_hpd_sta(UINT8 u8dev,UINT8 u8_tx);
UINT8 app_ms1826_get_rx_signal_sta(UINT8 u8dev,UINT8 u8_rx);
void app_ms1826_get_rx_timing(u8 u8dev,UINT8 u8_rx,pstTimingTable_t pstRxTiming);
void app_ms1826_get_tx_edid_data(UINT8 u8dev,UINT8 u8_tx,UINT8* pu8_edidbuf);
void app_ms1826_tx_split_config(u8 u8dev,u8 u8WinID,u8 u8Update,u16 u16X,u16 u16Y,u16 u16W,u16 u16H);
void app_ms1826_tx_split_update(u8 u8dev,u8 u8Path);
void app_ms1826_get_osd_string(u8 u8dev,u8 u8Rx);
void app_ms1826_display_user_string(u8 u8dev,u8 u8Rx);
void app_ms1826_set_display_user_string_enable(u8 u8dev,u8 u8Rx, bool bEnable);
void app_ms1826_set_user_string_color(u8 u8dev,u8 u8Rx,u8 u8R,u8 u8G,u8 u8B);
void app_ms1826_set_user_string_position(u8 u8dev,u8 u8Rx,u16 u16X,u16 u16Y);
void app_ms1826_tx_splicer_config(u8 u8dev,u8 u8PortIndex,u16 u16X,u16 u16Y,u16 u16W,u16 u16H);
#if ENABLE_MS1826_FRAME_LOCK
void app_ms1826_frame_lock_select(u8 u8dev,u8 u8RxIndex,u8 u8TxIndex);
#endif
void app_ms1826_tx_output_enable(u8 u8dev,u8 u8Tx,bool bSta);
void app_ms1826_reg_read(u8 u8dev,u16 u16AddrOffect,u8 *u8RVal);
void app_ms1826_reg_write(u8 u8dev,u16 u16AddrOffect,u8 u8RVal);

void app_MS1826FunctionProcess(UINT8 u8dev);
void app_MS1826Switch(UINT8 u8dev,u8 u8Rx, u8 u8Tx);
void app_MS1826BypassSwitch(u8 u8dev,u8 u8Rx, u8 u8Tx);
void app_MS1826FreezeChange(UINT8 u8dev,u8 u8Tx, u8 u8Sta);
void app_MS1826TxResolutionChange(UINT8 u8dev,u8 u8Tx, u8 u8ResID);
void app_MS1826SwitchFreeze(UINT8 u8dev,u8 u8Tx, u16 u16Time);
void app_MS1826SwFreezeTimer(u8 u8dev);
void app_MS1826PictureAdjust(UINT8 u8dev,u8 u8Tx,u8 u8Value,u8 u8AdjMode);
void app_MS1826TxConfig(UINT8 u8dev,u8 u8Tx,u8 u8Value);
void app_MS1826SetTxColorSpace(u8 u8dev,u8 u8Tx,u8 u8Value);
void app_MS1826TxHdcpEnable(UINT8 u8dev,u8 u8Tx,u8 u8Value);
void app_MS1826RxEdidUpdate(UINT8 u8dev,u8 u8Tx,pu8 pu8Edidbuf,u16 u16Size);
void app_MS1826GetTxEdid(UINT8 u8dev,u8 u8Tx);
void app_MS1826SetUserTiming(UINT8 u8dev,u8 u8Tx,pstTimingTable_t pstTiming);
void app_MS1826TxDigitalAudioMute(UINT8 u8dev,u8 u8Tx,u8 u8Value);
void app_MS1826TxDigitalAudioSourceSelect(u8 u8dev,u8 u8Tx,u8 u8AudioSource);
void app_MS1826TxAudioSourceSwitch(UINT8 u8dev,u8 u8Tx,u8 u8Value);
void app_MS1826TxPatternConfig(UINT8 u8dev,u8 u8Tx,u8 u8PatternID,u8 u8En);
void app_MS1826PictureColorConfig(UINT8 u8dev,u8 u8Tx,u8 u8RVal,u8 u8GVal,u8 u8BVal,u8 u8En);
void app_MS1826SplitConfig(u8 u8dev,u8 u8PortID,u8 u8Enable,u16 u16X,u16 u16Y,u16 u16W,u16 u16H);
void app_MS1826WorkModeSwitch(u8 u8dev,u8 u8WorkMode,u8 u8SubMode);
void app_MS1826SetOsdString(u8 u8dev,u8 u8Rx,u8 u8Len,u8 *pu8Str,bool bIsAlignRight);
void app_MS1826SetVideoMute(u8 u8dev,u8 u8Rx,bool bSta);
void app_MS1826SetUserOsdStringEnable(u8 u8dev,u8 u8Rx, bool bEnable);
void app_MS1826SetUserStringColor(u8 u8dev,u8 u8Rx, u8 u8R,u8 u8G,u8 u8B);
void app_MS1826SetUserOsdStringPosition(u8 u8dev,u8 u8Rx, u16 u16PosX,u16 u16PosY);
void app_MS1826TxHdmiAudioMute(u8 u8dev,u8 u8Tx,u8 u8Value);
void app_MS1826SplicerConfig(u8 u8dev,u8 u8PortID,u16 u16X,u16 u16Y,u16 u16W,u16 u16H);
void app_MS1826SplicerEnable(u8 u8dev,u8 u8PortID,bool bEn);
#if ENABLE_MS1826_FRAME_LOCK
void app_MS1826FrameLockSelect(u8 u8dev,u8 u8RxIndex,u8 u8TxIndex);
#endif
void app_MS1826SetSplitterMode(u8 u8dev,u8 u8Mode);
void app_MS1826SetSplitterUpdate(u8 u8dev,u8 u8Path);
void app_MS1826SetStandby(u8 u8dev,bool bOn);
bool app_MS1826GetStandby(u8 u8dev);
void app_MS1826StandbyProcess(u8 u8dev);

#endif

