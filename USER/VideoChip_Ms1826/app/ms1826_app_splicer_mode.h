#ifndef __MS1826_APP_SPLICER_MODE_H_
#define __MS1826_APP_SPLICER_MODE_H_

#include "ms1826_app.h"

#define SPLICER_BUF_NUM					DUAL_BUF

typedef enum _SPLICER_MODE_E
{
//	SPLICER_MODE_1X1,
	SPLICER_MODE_1X2,
	SPLICER_MODE_2X1,
	SPLICER_MODE_1X3,
	SPLICER_MODE_3X1,
	SPLICER_MODE_2X2,
	SPLICER_MODE_1X4,
	SPLICER_MODE_4X1
}SPLICER_MODE_E;

typedef enum _SPLICER_MIRROR_E
{
	SPLICER_ALL_NO_MIRROR  = 0x00,
	SPLICER_TX1_MIRROR     = 0x01,
	SPLICER_TX2_MIRROR     = 0x02,
	SPLICER_TX3_MIRROR     = 0x04,
	SPLICER_TX4_MIRROR     = 0x08
}SPLICER_MIRROR_E;

typedef struct _SPLICER_VIPSS_T
{
	UINT8               u8_vipss_sel;
	MISC_VIDEO_MUX_T    st_vipss_mux_dev;
	SDN4K_DEV_T 		st_sdn4k_dev;
	SDN_DEV_T           st_splicer_sdn_dev;
	VWDMA_DEV_T         st_splicer_vwdma_dev;
	DMA_COMMON_DEV_T    st_splicer_com_wdma_dev;
	SS_DEV_T            st_splicer_ss_vipss_dev;
}SPLICER_VIPSS_T;

typedef struct _SPLICER_VOPSS_T
{
	UINT8               u8_vopss_sel;
	DMA_COMMON_DEV_T    st_splicer_com_rdma_dev;
	VRDMA_DEV_T         st_splicer_vrdma_dev;
	SU_DEV_T            st_splicer_su_dev;
	SS_DEV_T            st_splicer_ss_vopss_dev;
}SPLICER_VOPSS_T;

typedef struct _SPLICER_APP_T
{
	UINT8               u8_splicer_mode;
	UINT8               u8_src_sel;
	BOOL                b_splicer_sw;
	UINT8               u8_tx_mirror;
	VIDEOSIZE_T 		tSdn_InSize;
	VIDEOSIZE_T 		tSdn_MemSize;
	DMA_START_ADDR_T    st_wdma_addr;
	WINBORDER_T			st_winborder[4];
}SPLICER_APP_T;
extern SPLICER_APP_T   gst_splicer_app_dev[DEV_NUM_MS1826];
extern UINT8 u8_h_border_fusion_r[DEV_NUM_MS1826][4];
extern UINT8 u8_h_border_fusion_l[DEV_NUM_MS1826][4];
extern UINT8 u8_v_border_fusion_t[DEV_NUM_MS1826][4];
extern UINT8 u8_v_border_fusion_b[DEV_NUM_MS1826][4];

VOID app_splicer_mode_config(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev);
VOID app_splicer_process(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev);
VOID splicer_video_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute);
VOID app_splicer_rx_smeless_switch(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev);
VOID app_splicer_output_resolution(UINT8 u8dev, SPLICER_APP_T *pst_app_splicer_dev);
VOID app_splicer_mirror_config(UINT8 u8dev,UINT8 u8_tx_sel, BOOL b_mirror_en);
VOID app_splicer_osd_init(UINT8 u8dev);
VOID app_splicer_osd_config(UINT8 u8dev);
VOID _splicer_video_freeze(UINT8 u8dev,UINT8 u8_vipss_sel);
VOID _splicer_video_unfreeze(UINT8 u8dev,UINT8 u8_vipss_sel);

#endif
