#ifndef _VIDEO_PRCESS_H
#define _VIDEO_PRCESS_H
#include "ms1826_app.h"
#include "ms1826_app_splitter_mode.h"
#include "ms1826_app_splicer_mode.h"
#include "ms1826_app_single_mode.h"
#include "ms1826_app_bypass_mode.h"
#include "ms1826_app_osdprocess.h"

#if MS1826_VIPSS1_DEINT
#define CHIP_INTERNAL_PROCESS_COLORSPACE  	HDMI_COLORSPACE_YCBCR444
#define CHIP_INTERNAL_PROCESS_COLORRANGE  	CSC_YUV444_BT601
#else
#define CHIP_INTERNAL_PROCESS_COLORSPACE  	HDMI_COLORSPACE_RGB
#define CHIP_INTERNAL_PROCESS_COLORRANGE  	CSC_RGB_16_235//CSC_RGB_0_255
#endif

#define CHIP_TX_CSC_OUT_COLORSPACE  		HDMI_COLORSPACE_RGB
#define CHIP_TX_CSC_OUT_COLORRANGE  		CSC_RGB_16_235//CSC_RGB_0_255

//1fps 1920x1080 RGB/YUV444: 1920*1080*3 = 6220800	YUV422: 1920*1080*2 = 4147200
//1fps 3840x2160 RGB/YUV444: 3840*2160*3 = 24883200	YUV422:	3840*2160*2 = 16588800
//MS1826 MEMORY 128M: 128*1024*1024 = 134217728

#define  MEMORY_BUF_ADDR_START       		0

#define  SINGLE_BUF_MEMORY_1080P_SIZE       6220800
#define  SINGLE_BUF_MEMORY_4K_SIZE       	24883200

#define	MEMORY_4K_H_SIZE_MAX				3840
#define	MEMORY_4K_V_SIZE_MAX				2160

#define	MEMORY_1080P_H_SIZE_MAX				1920
#define	MEMORY_1080P_V_SIZE_MAX				1080

typedef enum _E_VIDEO_PATTERN_CHANNEL_
{
	HDMI_RX_1   = 0,
	HDMI_RX_2   = 1,
	HDMI_RX_3   = 2,
	HDMI_RX_4   = 3,
	HDMI_RX_ALL = 4
}VIDEO_PATTERN_CHANNEL_E;

typedef enum _E_VIDEO_OUTPUT_CHANNEL_
{
	HDMI_TX_1   = 0,
	HDMI_TX_2   = 1,
	HDMI_TX_3   = 2,
	HDMI_TX_4   = 3,
	HDMI_TX_ALL = 4
}VIDEO_OUTPUT_CHANNEL_E;

VOID video_process_init(UINT8 u8dev);
VOID video_process_cfg(UINT8 u8dev);
VOID video_process(UINT8 u8dev);
VOID _audio_switch_process(VOID);
VOID _Video_port_hdmi_tx_init(VOID);
VOID _video_misc_process(UINT8 u8dev,UINT8 u8_rx_id);
VOID osd_service(UINT8 u8dev);
VOID VIDEOMUTE(UINT8 u8dev,UINT8 u8_path, BOOL b_mute);
VOID _video_timgen_cfg(UINT8 u8dev,UINT8 u8_timgen_id);
#if ENABLE_MS1826_FRAME_LOCK
VOID _video_timgen_cfg_frame_lock(UINT8 u8dev,UINT8 idx, UINT8 u8_timgen_id);
#endif
VOID _video_port_hdmi_tx_driver_config(UINT8 u8dev,u8 tx_id);
#endif//_VIDEO_PRCESS_H
