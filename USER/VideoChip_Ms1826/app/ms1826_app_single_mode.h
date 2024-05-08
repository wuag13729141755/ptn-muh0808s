#ifndef __MS1826_APP_SINGLE_MODE_H_
#define __MS1826_APP_SINGLE_MODE_H_

#include "ms1826_app.h"

#define  SINGLE_BUF_NUM                TRI_BUF//DUAL_BUF

typedef struct _SINGLE_VIPSS_T
{
	BOOL 	            b_is_dsc;
    UINT8               u8_vipss_sel;
    MISC_VIDEO_MUX_T    st_vipss_mux_dev;
	SDN4K_DEV_T 		st_sdn4k_dev;
    SDN_DEV_T           st_single_sdn_dev;
    VWDMA_DEV_T         st_single_vwdma_dev;
	DSC_DEV_T           st_encoder_dev;
    DMA_COMMON_DEV_T    st_single_com_wdma_dev;
    SS_DEV_T            st_single_ss_vipss_dev;
}SINGLE_VIPSS_T;

typedef struct _SINGLE_VOPSS_T
{
	BOOL 				b_is_dsc;
    UINT8               u8_vopss_sel;
    DMA_COMMON_DEV_T    st_single_com_rdma_dev;
    VRDMA_DEV_T         st_single_vrdma_dev;
	DSC_DEV_T           st_decoder_dev;
    SU_DEV_T            st_single_su_dev;
    SS_DEV_T            st_single_ss_vopss_dev;
}SINGLE_VOPSS_T;

typedef struct _SINGLE_WIN_T
{
    UINT16              u16_xpos;
    UINT16              u16_ypos;
    UINT16              u16_width;
    UINT16              u16_hight;
}SINGLE_WIN_T;

typedef struct _SINGLE_APP_T
{
	UINT8               u8_src_sel;
	UINT8               u8_tx_mirror;
	SINGLE_WIN_T	    st_win_size;
	SCALE_SIZE_T        st_scale_size;
	VIDEO_WIN_T         st_video_win;
	DMA_START_ADDR_T    st_dma_addr;
}SINGLE_APP_T;

extern SINGLE_VIPSS_T gst_single_vipss_dev[DEV_NUM_MS1826][4];
extern SINGLE_VOPSS_T gst_single_vopss_dev[DEV_NUM_MS1826][4];
extern BOOL b_freeze_status[DEV_NUM_MS1826][4];

VOID app_single_output_resolution(UINT8 u8dev);
VOID app_single_mode_config(UINT8 u8dev);
VOID SINGLE_PROCESS(UINT8 u8dev,UINT8 idx);
VOID single_video_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute);
VOID app_single_mode_rx_switch(UINT8 u8dev,UINT8 u8_rx, UINT8 u8_tx);
VOID app_single_osd_init(UINT8 u8dev);
VOID app_single_osd_config(UINT8 u8dev,UINT8 u8_index);
VOID _single_cfg(UINT8 u8dev,BOOL b_is_dsc,UINT8 u8_vipss_sel);
VOID _single_vopss_cfg(UINT8 u8dev,BOOL b_is_dsc,UINT8 u8_vopss_sel);
VOID _single_video_freeze(UINT8 u8dev,UINT8 u8_path_sel);
VOID _single_video_unfreeze(UINT8 u8dev,UINT8 u8_path_sel);
VOID _video_freeze(UINT8 u8dev,UINT8 u8_path);
VOID _video_freeze_multichip(UINT8 u8ChipNum,UINT8 u8StartDev,UINT8* pu8_path);
VOID _video_unfreeze_multichip(UINT8 u8ChipNum,UINT8 u8StartDev,UINT8* pu8_path);
VOID _video_unfreeze(UINT8 u8dev,UINT8 u8_path);
VOID _video_freeze_out(UINT8 u8dev,UINT8 u8_path);
VOID _video_unfreeze_out(UINT8 u8dev,UINT8 u8_path);
VOID app_user_osd_config(UINT8 u8dev,UINT8 u8_index);
void app_single_user_mode_win_config(UINT8 u8dev,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bUpdate);
void app_single_user_splitter_mode_win_config(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bUpdate,BOOL bIsModeSw);
VOID app_single_mode_switch_config(UINT8 u8dev,BOOL bIsOutCfg);

#endif
