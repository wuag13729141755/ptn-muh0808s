#ifndef _MS1826_APP_SPLITTER_MODE_H_
#define _MS1826_APP_SPLITTER_MODE_H_

#include "ms1826_app.h"

#define  SPLITTER_BUF_NUM                     	TRI_BUF

typedef enum _SPLITTER_MODE_E
{
	MODE_SIGNLE_1,                  // 0
	MODE_SIGNLE_2,
	MODE_SIGNLE_3,
	MODE_SIGNLE_4,
	MODE_DUAL_UP1_DOWN2,
	MODE_DUAL_UP1_DOWN2_4_3,        // 5
	MODE_DUAL_UP3_DOWN4,
	MODE_DUAL_UP3_DOWN4_4_3,
	MODE_DUAL_LEFT1_RIGHT2,
	MODE_DUAL_LEFT1_RIGHT2_4_3,
	MODE_DUAL_LEFT3_RIGHT4,         // 10
	MODE_DUAL_LEFT3_RIGHT4_4_3,
	MODE_PIP_12_1_3,
	MODE_PIP_21_1_3,
	MODE_PIP_34_1_3,
	MODE_PIP_43_1_3,                // 15
	MODE_THREE_EQUALIZE,
	MODE_FOUR_MAIN1_SUB234,
	MODE_FOUR_MAIN2_SUB134,
	MODE_FOUR_MAIN3_SUB124,
	MODE_FOUR_MAIN4_SUB123,         // 20
	MODE_FOUR_H_SUB1234,
	MODE_FOUR_EQUALIZE
}SPLITTER_MODE_E;

typedef enum _emMultiViewModeType_t_
{
    emMvMode_1Win_Full=0,
    emMvMode_2Win_PBP,
    emMvMode_3Win_2U1D,
    emMvMode_4Win_SameSize,
    emMvMode_2Win_PIP_LU,
    emMvMode_2Win_PIP_LD,
    emMvMode_2Win_PIP_RU,
    emMvMode_2Win_PIP_RD,
    emMvMode_4Win_PBP_3L1R,
    emMvMode_4Win_PBP_1L3R,
    emMvMode_4Win_PBP_3U1D,     // 10
    emMvMode_4Win_PBP_1U3D,
    emMvMode_4Win_PIP_1F3L,
    emMvMode_4Win_PIP_1F3R,
    emMvMode_4Win_PIP_1F3U,
    emMvMode_4Win_PIP_1F3D,
    emMvMode_xWin_ByUser1,
    emMvMode_xWin_ByUser2,
    emMvMode_xWin_ByUser3,
    emMvMode_xWin_ByUser4,

    emMvMode_max,
}emMultiViewModeType_t;

typedef enum _SPLITTER_AUDIO_SRC_E
{
	AUDIO_HDMI_RX1,
	AUDIO_HDMI_RX2,
	AUDIO_HDMI_RX3,
	AUDIO_HDMI_RX4,
}SPLITTER_AUDIO_SRC_E;

typedef struct _stWinTypeFactorType_t_
{
/**********************************
**StartX = (u8StX_sf1*Dst_H)/u8StX_sf2
**StartY = (u8StY_sf1*Dst_V)/u8StY_sf2
**Width  = (u8Width_sf1*Dst_H)/u8Width_sf2
**Height = (u8Height_sf1*Dst_V)/u8Height_sf2
**********************************/
    u8 u8StX_sf1;
    u8 u8StX_sf2;
    u8 u8StY_sf1;
    u8 u8StY_sf2;
    u8 u8Width_sf1;
    u8 u8Width_sf2;
    u8 u8Height_sf1;
    u8 u8Height_sf2;
}stWinTypeFactorType_t,*pstWinTypeFactorType_t;

typedef struct _SPLITTER_VIPSS_T
{
	UINT8               u8_vipss_sel;
	MISC_VIDEO_MUX_T    st_vipss_mux_dev;
	SDN4K_DEV_T 		st_sdn4k_dev;
	SDN_DEV_T           st_splitter_sdn_dev;
	VWDMA_DEV_T         st_splitter_vwdma_dev;
	DMA_COMMON_DEV_T    st_splitter_com_wdma_dev;
	SS_DEV_T            st_splitter_ss_vipss_dev;
}SPLITTER_VIPSS_T;

typedef struct _SPLITTER_VOPSS_T
{
	UINT8               u8_vopss_sel;
	DMA_COMMON_DEV_T    st_splitter_com_rdma_dev;
	VRDMA_DEV_T         st_splitter_vrdma_dev;
	SU_DEV_T            st_splitter_su_dev;
	SS_DEV_T            st_splitter_ss_vopss_dev;
}SPLITTER_VOPSS_T;

typedef struct _SPLITTER_APP_T
{
	UINT8               u8_splitter_mode;
	UINT8               u8_dst_sel;
	BOOL                b_splitter_sw;
	VIDEO_WIN_T         st_video_win[4];
	SCALE_SIZE_T        st_scale_size[4];
	DMA_START_ADDR_T    st_dma_addr[4];

}SPLITTER_APP_T;

extern SPLITTER_APP_T g_st_app_splitter_dev[DEV_NUM_MS1826];
extern SPLITTER_VIPSS_T gst_splitter_vipss_dev[DEV_NUM_MS1826][4];
extern SPLITTER_VOPSS_T gst_splitter_vopss_dev[DEV_NUM_MS1826][4];
extern stMs1826Wincfg_T g_stMs1826WinTable[DEV_NUM_MS1826][4];

VOID app_splitter_output_resolution(UINT8 u8dev,UINT8 u8_vic, SPLITTER_APP_T *pst_app_splitter_dev);
VOID app_splitter_mode_config(UINT8 u8dev,SPLITTER_APP_T *pst_app_splitter_dev);
VOID app_splitter_process(UINT8 u8dev,UINT8 idx);
VOID app_splitter_mode_video_freeze(SPLITTER_APP_T *pst_app_splitter_dev);
VOID app_splitter_mode_audio_src(UINT8 u8dev);
VOID splitter_video_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute);
VOID app_splitter_rx_rotate(UINT8 u8dev,UINT8 idx, UINT8 u8_rotate);
VOID app_splitter_osd_init(UINT8 u8dev);
VOID app_splitter_osd_config(UINT8 u8dev);
VOID _splitter_video_unfreeze(UINT8 u8dev);
VOID _splitter_video_freeze(UINT8 u8dev,UINT8 u8_path_sel);
void app_splitter_user_mode_win_config(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bIsDsc,BOOL bUpdate,BOOL bIsModeSw);
void app_splitter_user_mode_config(UINT8 u8dev,SPLITTER_APP_T *pst_app_splitter_dev);
void app_splitter_user_process(UINT8 u8dev,UINT8 idx);
VOID app_splitter_mode_rx_switch(UINT8 u8dev,UINT8 u8_rx, UINT8 u8_tx);
void app_splitter_user_mode_win_update(UINT8 u8dev,BOOL bIsDsc,UINT8 u8Path);

#endif
