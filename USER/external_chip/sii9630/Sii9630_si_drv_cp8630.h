/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed ìAS ISÅEWITHOUT ANY WARRANTY of any kind,
* whether express or implied, including without limitation the implied warranty
* of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE or NON-INFRINGEMENT.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*
* You may contact Lattice Semiconductor Corporation at 111 SW 5th Ave,
* Suite 700, Portland, OR 97204
*/

#ifndef __SI_DRV_CP8630_H__
#define __SI_DRV_CP8630_H__

#ifndef CONFIG_SOURCE_ONLY
#define CONFIG_SOURCE_ONLY  0
#endif

#include "Sii9630_si_lib_edid.h"
#include "Sii9630_si_platform.h"
#include "Sii9630_si_infoframe.h"


// Compile option for remove 4096 VIC
#ifndef REMOVE_4096x2160_VIC
#define REMOVE_4096x2160_VIC
#define _4096_VIC_98 98
#define _4096_VIC_102 102
#endif

//#define BIT0	0x00000001
//#define BIT1	0x00000002
//#define BIT2	0x00000004
//#define BIT3	0x00000008
//#define BIT4	0x00000010
//#define BIT5	0x00000020
//#define BIT6	0x00000040
//#define BIT7	0x00000080

#define SII_INFOFRAME_MAX_LEN	31
#define SII_EDID_BLOCK_SIZE	128
#define SII_EDID_MAX_BLOCKS	4
#define SII_EDID_MAX_LEN	(SII_EDID_MAX_BLOCKS * SII_EDID_BLOCK_SIZE)
#define SII_EDID_RX_MAX_BLOCKS	2
#define SII_EDID_RX_MAX_LEN	(SII_EDID_RX_MAX_BLOCKS * SII_EDID_BLOCK_SIZE)

#define SII_MAX_KSV_SUPPORTED	0x63
#define SII_MAX_DEPTH_SUPPORTED	0x07
#define KSV_NUM_BYTES	 5

#define SII_DRV_MAX_DS_DEV_SUPPORT 127
#define RCVRID_LENGTH		5
#define MAX_DEVICE_COUNT	31
#define MAX_DEPTH		4

/* ske__send__eks is 200ms before authencation done */
#define AUTH_WAIT		200
/* 3s timer */
#define RPT_READY_WAIT		3000
#define REAUTHEN_WAIT		1000

/* HDCP control flags */
#define HDCP_CTRL_MODE (0 \
	| BIT_MSK__TPI_COPP_DATA2__REG_COPP_PROTLEVEL \
	| BIT_MSK__TPI_COPP_DATA2__REG_DDC_SHORT_RI_RD \
	| BIT_MSK__TPI_COPP_DATA2__REG_DOUBLE_RI_CHECK \
	| BIT_MSK__TPI_COPP_DATA2__REG_KSV_FORWARD \
	)

/*DDC_CMD register enums*/
#define LEN_TPI__DDC_FIFO_SIZE					16
#define BIT_ENUM__DDC_CMD__SEQUENTIAL_READ			0x02
#define BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ			0x04
#define BIT_ENUM__DDC_CMD__SEQUENTIAL_WRITE			0x06
#define BIT_ENUM__DDC_CMD__CLEAR_FIFO				0x09
#define BIT_ENUM__DDC_CMD__CLOCK_SCL                0x0A
#define BIT_ENUM__DDC_CMD__ABORT_TRANSACTION			0x0F

#define HDMI_TX_UEVENT_CONNECTION		0x00000001
#define HDMI_TX_UEVENT_DISCONNECTION		0x00000002
#define MHL_TX_UEVENT_CONNECTION		0x00000004
#define MHL_TX_UEVENT_DISCONNECTION		0x00000008
#define MHL_TX_UEVENT_RCP_RECEIVED		0x00000010
#define MHL_TX_UEVENT_RCPK_RECEIVED		0x00000020
#define MHL_TX_UEVENT_RCPE_RECEIVED		0x00000040
#define MHL_TX_UEVENT_UCP_RECEIVED		0x00000080
#define MHL_TX_UEVENT_UCPK_RECEIVED		0x00000100
#define MHL_TX_UEVENT_UCPE_RECEIVED		0x00000200
#define MHL_TX_UEVENT_RBP_RECEIVED		0x00000400
#define MHL_TX_UEVENT_RBPK_RECEIVED		0x00000800
#define MHL_TX_UEVENT_RBPE_RECEIVED		0x00001000
#define MHL_TX_UEVENT_SPAD_RECEIVED		0x00002000
#define MHL_TX_UEVENT_POW_BIT_CHG		0x00004000
#define MHL_TX_UEVENT_RAP_RECEIVED		0x00008000
#define MHL_TX_UEVENT_BIST_READY_RECEIVED	0x00010000
#define MHL_TX_UEVENT_BIST_TEST_DONE		0x00020000
#define MHL_TX_UEVENT_BIST_STATUS_RECEIVED	0x00040000
#define MHL_TX_UEVENT_T_RAP_MAX_EXPIRED		0x00080000
#define MHL_TX_UEVENT_AUD_DELAY_RCVD		0x00100000

#ifdef CONFIG_ENHANCED_MODE_PROT
#define RX_DESCRAMBLE_THROUGH_API

#define EMM_YC444_RGB		0x00
#define EMM_YC422		0x40
#define EMM_YC420		0x80

#define EMC_HDMI2_NONE		0x0000	/* Do not support Enhanced Mode. */
#define EMC_HDMI2_DSC_720P60	0x0001	/* DSC 720P60 is supported. */
#define EMC_HDMI2_DSC_1080P60	0x0002	/* DSC 1080P60 is supported. */
#define EMC_HDMI2_DSC_4K50	0x0004	/* DSC 4K50 is supported. */
#define EMC_HDMI2_DSC_4K60	0x0008	/* DSC 4K60 is supported. */
#define EMC_HDMI2_DSC_4K30	0x0010	/* DSC 4K30 is supported. */
#define EMC_HDMI2_DSC_4K25	0x0020	/* DSC 4K25 is supported. */
#define EMC_HDMI2_DSC_4K24	0x0040	/* DSC 4K24 is supported. */

enum dsc_mode {
	EMM_HDMI2_NORMAL,	/* Normal HDMI2 Mode */
	EMM_HDMI2_DSC_4K50_2T1,	/* DSC 4K50 over HDMI2 */
	EMM_HDMI2_DSC_4K50_3T1,
	EMM_HDMI2_DSC_4K60_2T1,	/* DSC 4K60 over HDMI2 */
	EMM_HDMI2_DSC_4K60_3T1,
	EMM_HDMI2_DSC_4K30_2T1,	/* DSC 4K30 over HDMI2 */
	EMM_HDMI2_DSC_4K30_3T1,
	EMM_HDMI2_DSC_4K25_2T1,	/* DSC 4K25 over HDMI2 */
	EMM_HDMI2_DSC_4K25_3T1,
	EMM_HDMI2_DSC_4K24_2T1,	/* DSC 4K24 over HDMI2 */
	EMM_HDMI2_DSC_4K24_3T1,
	EMM_HDMI2_DSC_1080P60_2T1,	/* DSC 1080P60 over HDMI2 */
	EMM_HDMI2_DSC_1080P60_3T1,
	EMM_HDMI2_DSC_720P60_2T1,	/* DSC 720P60 over HDMI2 */
	EMM_HDMI2_DSC_720P60_3T1,

};
#endif

enum sii_drv_event_type {
	SII_DRV_EVENT_TYPE__GENERAL,
	SII_DRV_EVENT_TYPE__VIDPATH,
	SII_DRV_EVENT_TYPE__RX_SCDC,
	SII_DRV_EVENT_TYPE__TX_HDMI,
	SII_DRV_EVENT_TYPE__TX_HDCP,
};

enum sii_drv_rx_gen_event {
	/* AVI info fame is changed*/
	SII_DRV_RX_EVENT__AVI_IF = 0x00000001,
	/* audio info fame is changed*/
	SII_DRV_RX_EVENT__AUDIO_IF = 0x00000002,
	/* VS info fame is changed*/
	SII_DRV_RX_EVENT__VS_IF = 0x00000004,
	/* SPD info fame is changed*/
	SII_DRV_RX_EVENT__SPD_IF = 0x00000008,
	/* GBD info fame is changed*/
	SII_DRV_RX_EVENT__GBD_IF = 0x00000010,
	/* MPEG info fame is changed*/
	SII_DRV_RX_EVENT__MPEG_IF = 0x00000020,
	/* ISRC info fame is changed*/
	SII_DRV_RX_EVENT__ISRC_IF = 0x00000040,
	/* i_s_r_c2info fame is changed*/
	SII_DRV_RX_EVENT__ISRC2_IF = 0x00000080,
	/* g_c_pinfo fame is changed*/
	SII_DRV_RX_EVENT__GCP_IF = 0x00000100,
	/* ACP info fame is changed*/
	SII_DRV_RX_EVENT__ACP_IF = 0x00000200,
	/* AV mute state is changed*/
	SII_DRV_RX_EVENT__AVMUTE_CHNG = 0x00000400,
	/* TMDS mode is changed*/
	SII_DRV_RX_EVENT__TMDS_MODE_CHNG = 0x00000800,
	/* channel status is changed*/
	SII_DRV_RX_EVENT__CHAN_STAT_CHNG = 0x00001000,
	/* audio format is changed*/
	SII_DRV_RX_EVENT__AUDIO_FRM_CHNG = 0x00002000,
	/* input connection changed*/
	SII_DRV_RX_EVENT__RX_CONN_CHNG = 0x00004000,
	/* deep color changed*/
	SII_DRV_RX_EVENT__DC_CHNG = 0x00008000,
	/* authentication changed*/
	SII_DRV_RX_EVENT__AUTH_CHNG = 0x00010000,
	/* h_d_c_p2x content type changed*/
	SII_DRV_RX_EVENT__HDCP2X_CONTENT_TYPE_CHNG = 0x00020000,
	/* 3D audio sample packet received*/
	SII_DRV_RX_EVENT__3D_ADUIO_PKT = 0x00040000,
	/* one bit 3D audio sample packet received*/
	SII_DRV_RX_EVENT__ONEBIT_3D_ADUIO_PKT = 0x00080000,
	/* 3D audio or multi stream audio, additional iformation packet
	received*/
	SII_DRV_RX_EVENT__ADUIO_METADATA_PKT = 0x00100000,
	/* multi stream audio sample packet received*/
	SII_DRV_RX_EVENT__MULTISTREAM_ADUIO_PKT = 0x00200000,
	/* one bit multi stream audio sample packet received*/
	SII_DRV_RX_EVENT__ONEBIT_MULTISTREAM_ADUIO_PKT = 0x00400000,
	/* sync and clock is changed*/
	SII_DRV_RX_EVENT__SYNC_CKDT_CHANGE = 0x00800000,
	/* SCDC event*/
	SII_DRV_RX_EVENT__SCDC_EVENT = 0x01000000,
	/* ske init event*/
	SII_DRV_RX_EVENT__SKE_EVENT = 0x02000000,
	SII_DRV_RX_EVENT__HPD_CHANGE = 0x04000000,
	/* ACR is changed*/
	SII_DRV_RX_EVENT__ACR_IF = 0x08000000,
    /* No Audio Packet */
    SII_DRV_RX_EVENT__NO_AUDIO = 0x10000000,

};

enum sii_drv_tx_gen_event {
	/* hot-_plug status is changed*/
	SII_DRV_TX_EVENT__HOT_PLUG_CHNG = 0x00000001,
	/* rsen status is changed*/
	SII_DRV_TX_EVENT__RSEN_CHNG = 0x00000002,
	/* downstream EDID is changed*/
	SII_DRV_TX_EVENT__EDID_CHNG = 0x00000004,
	/* TMDS mode is changed/updated*/
	SII_DRV_TX_EVENT__HDMI_STATE_CHNG = 0x00000008,
	/* HDCP status changed*/
	SII_DRV_TX_HDCP_EVENT__HDCP_STATE_CHNG = 0x00000010,
	/* downstream BKSV is changed*/
	SII_DRV_TX_HDCP_EVENT__HDCP_BKSV_CHNG = 0x00000020,
	/* output tmds changed */
	SII_DRV_TX_EVENT__OUTPUT_TMDS_CHNG_REQ = 0x00000040,
	/* SCDC - EMC Received */
	SII_DRV_TX_EVENT__SCDC_EM_CAP_CHG = 0x00000080,
	SII_DRV_TX_EVENT__SCDC_EMM_RESP_CHG = 0x00000100,
	SII_DRV_TX_EVENT__SCDC_EM_NO_RESP = 0x00000200,

};

/*HDCP events*/
enum sii_drv_hdcp_gen_event {
	SII_DRV_HDCP_EVENT__NONE = 0x00000000,
	/* HDCP status is changed*/
	SII_DRV_HDCP_EVENT__HDCP_STATUS_CHNG = 0x00000001,
	/* HDCP is failed*/
	SII_DRV_HDCP_EVENT__HDCP_RESET = 0x00000002,
	/* downstream bstatus is updated*/
	SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED = 0X00000004,
	/* downstream bksv is updated..*/
	SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED = 0x00000008,
	/* downstream is authenticated*/
	SII_DRV_HDCP_EVENT__HDCP_AUTHENTICATED = 0x00000010,
	/*HDCP bstatusis processed*/
	SII_DRV_HDCP_EVENT__HDCP_BSTATUS_PROCESSED = 0x00000020,
	/* HDCP 2.2 RCVID change interrupt received*/
	SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED = 0x00000400,
	/* HDCP 2.2 stream manage to be sent*/
	SII_DRV_HDCP_EVENT__HDCP_STMNG_EVENT = 0x00008000,
	/* HDCP 2.2 stream manage to be sent*/
	SII_DRV_HDCP_EVENT__HDCP_SKE_EVENT = 0x00010000,
	/* HDCP 2.2 stream manage to be sent*/
	SII_DRV_HDCP_EVENT__HDCP_RESET_FLAGS = 0x00020000,
	/* hash fail occured*/
	SII_DRV_HDCP_EVENT__HDCP_HASH_FAIL_EVENT = 0x00040000,
	/* SKE_SEND_EKS */
	SII_DRV_HDCP_EVENT__SKE_SEND_EKS = 0x00080000,
	/* Cert Rcvd */
	SII_DRV_HDCP_EVENT__HDCP_CERT_RCVD = 0x00100000,
	/* Reauth_Req Rcvd */
	SII_DRV_HDCP_EVENT__HDCP_REAUTH_REQ = 0x00200000,
};

/* rx SCDCS events */
enum sii_drv_rx_hdmi_scdcs_event {
	/* scrambling status changed in SCDCS*/
	SII_DRV_RX_EVENT__SCDCS_SCRAMBLING_CHNG = 0x00000001,
	/* clock ratio changed in SCDCS*/
	SII_DRV_RX_EVENT__SCDCS_CLK_RATIO_CHNG = 0x00000002,
};

enum sii_drv_rx_vid_path_event {
	/* video path interrupt received*/
	SII_DRV_RX_VIDPATH_EVENT__SYNC_CHANGE = 0x00000001,
	/* color conversion done*/
	SII_DRV_RX_VIDPATH_EVENT__COLOR_CONV_DONE = 0x00000002,
	/* input video parameters changed*/
	SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG = 0x00000004,
	/* Request reference clock*/
	SII_DRV_RX_VIDPATH_EVENT__REQ_REF_CLK = 0x00000008,
	/* Input is not stable. Continuous input change interrupt*/
	SII_DRV_RX_VIDPATH_EVENT__HSYNC_HIGH_CNT_CHG = 0x00000010,
};

enum sii_drv_rx_input_type {
	SII_DRV_INPUT_TYPE__NONE,
	SII_DRV_INPUT_TYPE__HDMI,
	SII_DRV_INPUT_TYPE__MHL
};

enum sii_tmds_mode {
	SII_TMDS_MODE__NONE,
	SII_TMDS_MODE__DVI,
	SII_TMDS_MODE__HDMI1,
	SII_TMDS_MODE__HDMI2,
	SII_TMDS_MODE__AUTO
};

/* hdmi external states */
enum sii_drv_hdmi_tx_external_state {
	SII_DRV_HDMI_EXT_STATUS__TMDS_OFF,
	SII_DRV_HDMI_EXT_STATUS__TMDS_ON,
};

enum sii_info_frame_id {
	SII_INFO_FRAME_ID__AVI,
	SII_INFO_FRAME_ID__AUDIO,
	SII_INFO_FRAME_ID__VS,
	SII_INFO_FRAME_ID__SPD,
	SII_INFO_FRAME_ID__GBD,
	SII_INFO_FRAME_ID__MPEG,
	SII_INFO_FRAME_ID__ISRC,
	SII_INFO_FRAME_ID__ISRC2,
	SII_INFO_FRAME_ID__GCP,
	SII_INFO_FRAME_ID__ACP,
	SII_INFO_FRAME_ID__AUD_METDAT,
#ifdef CONFIG_DSC_EN
	SII_INFO_FRAME_ID__VSIF1,
	SII_INFO_FRAME_ID__VSIF2,
	SII_INFO_FRAME_ID__VSIF3,
	SII_INFO_FRAME_ID__VSIF4,
#endif
	SII_INFO_FRAME_ID__ACR,
	SII_INFO_FRAME_ID__TOTAL
};

struct sii_info_frame {
	enum sii_info_frame_id if_id;
	bool is_if_on;
	uint8_t b[SII_INFOFRAME_MAX_LEN];
};

struct sii_edid {
	uint16_t size;
	uint8_t b[SII_EDID_MAX_LEN];
};

/* video bit depth */
enum sii_drv_bit_depth {
	SII_DRV_BIT_DEPTH__PASSTHOUGH = 0,
	SII_DRV_BIT_DEPTH__8_BIT = 8,
	SII_DRV_BIT_DEPTH__10_BIT = 10,
	SII_DRV_BIT_DEPTH__12_BIT = 12,
	SII_DRV_BIT_DEPTH__16_BIT = 16
};

enum sii_hv_sync_pol {
	SII_DRV_HV_SYNC_POL__HPVP,
	SII_DRV_HV_SYNC_POL__HPVN,
	SII_DRV_HV_SYNC_POL__HNVP,
	SII_DRV_HV_SYNC_POL__HNVN
};

/* video path color info configuration data */
struct sii_drv_color_info_cfg {
	enum sii_drv_conv_std input_clr_conv_std;
	enum sii_drv_clr_spc input_clr_spc;
	enum sii_drv_bit_depth input_vid_dc_depth;
};

/* bist pattern Type */
enum sii_drv_bist_pattern_type {
	SII_DRV_BIST_PATTERN_TYPE__NONE,
	SII_DRV_BIST_PATTERN_TYPE__TEST,
	SII_DRV_BIST_PATTERN_TYPE__DISABLE,
};

/* TMDS bist test patterns */
enum sii_drv_bist_test_pattern {
	SII_DRV_BIST_TEST_PATTERN__WALKING_ONE,
	SII_DRV_BIST_TEST_PATTERN__RAMP,
	SII_DRV_BIST_TEST_PATTERN__LFSR_1,
	SII_DRV_BIST_TEST_PATTERN__LFSR_2,
	SII_DRV_BIST_TEST_PATTERN__STATIC_1,
	SII_DRV_BIST_TEST_PATTERN__STATIC_2,
	SII_DRV_BIST_TEST_PATTERN__MAX_SWITCH,
	SII_DRV_BIST_TEST_PATTERN__TWO_COLUMN_MAX_SWITCH,
	SII_DRV_BIST_TEST_PATTERN__SDVO_TMDS_HALF_CLK,
	SII_DRV_BIST_TEST_PATTERN__SDVO_TMDS_8BIT_STATIC,
	SII_DRV_BIST_TEST_PATTERN__10BIT_STATIC,
	SII_DRV_BIST_TEST_PATTERN__4TMDS_PATTERN,
};

/* TMDS bist stpg patterns */
enum sii_drv_stpg_pattern {
	SII_DRV_BIST_STPG_PATTERN__RED,
	SII_DRV_BIST_STPG_PATTERN__GREEN,
	SII_DRV_BIST_STPG_PATTERN__BLUE,
	SII_DRV_BIST_STPG_PATTERN__BLACK,
	SII_DRV_BIST_STPG_PATTERN__WHITE,
	SII_DRV_BIST_STPG_PATTERN__RAMP,
	SII_DRV_BIST_STPG_PATTERN__CHESS,
	SII_DRV_BIST_STPG_PATTERN__COLOR_BAR,
	SII_DRV_BIST_STPG_PATTERN__SIMP92,
};

/* TMDS bist timing detials */
enum sii_drv_bist_timing {
	SII_DRV_BIST_TIMING__720P60,
	SII_DRV_BIST_TIMING__576P50,
	SII_DRV_BIST_TIMING__480P60,
	SII_DRV_BIST_TIMING__1080P50,
	SII_DRV_BIST_TIMING__1080P60,
	SII_DRV_BIST_TIMING__4K2KP30,
	SII_DRV_BIST_TIMING__4K2KP24,
	SII_DRV_BIST_TIMING__4K2KP25,
	SII_DRV_BIST_TIMING__FULLY_PROGRAMMABLE,
	SII_DRV_BIST_TIMING__VGA,
	SII_DRV_BIST_TIMING__SVGA,
	SII_DRV_BIST_TIMING__XGA,
	SII_DRV_BIST_TIMING__SXGA,
	SII_DRV_BIST_TIMING__UXGA,
};

/* TMDS bist color depth */
enum sii_drv_bist_color_depth {
	SII_DRV_BIST_COLOR_DEPTH__8,
	SII_DRV_BIST_COLOR_DEPTH__10,
	SII_DRV_BIST_COLOR_DEPTH__12,
};

/* BIST owner */
enum sii_drv_bist_owner {
	SII_DRV_BIST_OWNER__NONE,
	SII_DRV_BIST_OWNER__DRIVER,
	SII_DRV_BIST_OWNER__API,
};

/* TMDS bist configuration */
struct sii_drv_bist_config {
	enum sii_drv_bist_pattern_type pat_type;
	enum sii_drv_bist_test_pattern test_pat;
	enum sii_drv_stpg_pattern stpg_pat;
	enum sii_drv_bist_timing bist_tmg;
	enum sii_drv_bist_color_depth clr_depth;
	enum sii_drv_bist_owner requester;
};

/* input video timing parameters */
struct sii_drv_vid_timing_params {
	uint32_t pixel_frequency;
	uint32_t v_freq;
	uint32_t v_freq_mult_factor;
	uint16_t h_active;
	uint16_t h_blank;
	uint16_t v_active;
	uint16_t v_blank;
	uint16_t v_front;
	uint16_t v_sync;
	uint16_t v_back;
	uint32_t tmds_clk;
};

/* VTG timing details */
struct sii_drv_vtg_params {
	uint16_t vtg_horizontal_sync_end;
	uint16_t vtg_horizontal_active_video_start;
	uint16_t vtg_halfline;
	uint16_t vtg_horizontal_active_video_end;
	uint16_t vtg_end_of_line;
	uint16_t vtg_vertical_sync_end;
	uint16_t vtg_trigger_start;
	uint16_t vtg_vertical_active_video_start;
	uint16_t vtg_vertical_active_video_end;
	uint16_t vtg_vertical_end_of_frame;
	uint8_t  vtg_config;
	uint16_t vtg_cycle_delay;
};

struct audio_N_CTS_table {
	uint32_t tmds_rate;
	uint32_t values_N;
};

/* peer's manufacturer status */
struct sii_drv_tx_scdc_manufacturer_status {
	uint8_t oui_1;
	uint8_t oui_2;
	uint8_t oui_3;
	char dev_id_str[8];
	uint8_t dev_id_hw_rev;
	uint8_t dev_id_sw_major_rev;
	uint8_t dev_id_sw_minor_rev;
};

/* peer's SCDC reg status */
struct sii_drv_tx_scdc_register_status {
	uint8_t sink_ver;
	uint8_t source_ver;
	uint8_t update_0;
	uint8_t update_1;
	uint8_t tmds_config;
	uint8_t tmds_status;
	uint8_t config_0;
	uint8_t status_flag_0;
	uint8_t test_config_0;
	uint16_t chnl0_error_cnt;
	uint16_t chnl1_error_cnt;
	uint16_t chnl2_error_cnt;
};

/* SCDC source scramble status */
enum sii_drv_scdc_source_scrmble_status {
	SII_DRV_SCDC_SOURCE_SCRAMBLE_OFF,
	SII_DRV_SCDC_SOURCE_SCRAMBLE_ON,
};

/* SCDC sink scramble status */
enum sii_drv_tx_scdc_sink_scrmble_status {
	SII_DRV_SCDC_SINK_DESCRAMBLE_OK,
	SII_DRV_SCDC_SINK_DESCRAMBLE_NOK,
};


/* SCDC status update registers */
struct sii_drv_tx_scdc_status_update_regs {
	uint8_t scdc_update_0;
	uint8_t scdc_update_1;
};

struct sii_drv_tx_scdc_sink_caps {
	bool b3_d_osd_disparity;
	bool b_dual_view;
	bool b_independent_view;
	bool b_lte340_mscs_scramble;
	bool b_read_req_capable;
	bool b_scdc_present;
	bool b_dc30bit420;
	bool b_dc36bit420;
	bool b_dc48bit420;
	uint32_t vclk_mb;
};

enum sii_drv_hdcp_link_version {
	SII_DRV_HDCP_LINK_VER__NONE,
	SII_DRV_HDCP_LINK_VER__HDCP1X,
	SII_DRV_HDCP_LINK_VER__HDCP22,
	SII_DRV_HDCP_LINK_VER__AUTO
};

/* HDCP status */
enum sii_drv_hdcp_status {
	/* authentication is not enabled*/
	SII_DRV_HDCP_STATUS__OFF,
	/* authentication succeeded for HDCP 1.X*/
	SII_DRV_HDCP_STATUS__SUCCESS_1X,
	/* authentication succeeded for HDCP 2.2*/
	SII_DRV_HDCP_STATUS__SUCCESS_22,
	/* authentication is in progress*/
	SII_DRV_HDCP_STATUS__AUTHENTICATING,
	/* authentication failed and does not re-try*/
	SII_DRV_HDCP_STATUS__FAILED,
};

/* downstream HDCP failure reason */
enum sii_drv_hdcp_failure {
	/* no failure detected so far*/
	SII_DRV_HDCP_FAILURE__NONE,
	/* downstream device does not acknowledge HDCP registers;
	firmware continues trying*/
	SII_DRV_HDCP_FAILURE__NACK,
	/* too many devices; firmware does not try until HPD low to
	high transition*/
	SII_DRV_HDCP_FAILURE__DEV_EXC,
	/* cascade exceeded error; firmware does not try until HPD
	low to high transition*/
	SII_DRV_HDCP_FAILURE__CAS_EXC,
	/* V verification failed; firmware continues trying*/
	SII_DRV_HDCP_FAILURE__V,
	/* authentication timed out; firmware continues trying*/
	SII_DRV_HDCP_FAILURE__TIMEOUT,
	/* Content stream M check failure*/
	SII_DRV_HDCP_FAILURE__HASH,
	/* R0, H!=H' or L!==L'; firmware continues trying*/
	SII_DRV_HDCP_FAILURE__AUTH_FAIL,
	/* reauth request received; firmware continues trying*/
	SII_DRV_HDCP_FAILURE__REAUTH_REQ,
	/* other authentication errors; firmware continues trying*/
	SII_DRV_HDCP_FAILURE__OTHER,
};

/* HDCP 2.2 content type */
enum sii_drv_hdcp_content_type {
	/* HDCP for content type 0*/
	SII_DRV_HDCP_CONTENT_TYPE__0,
	/* HDCP for content type 1*/
	SII_DRV_HDCP_CONTENT_TYPE__1,
	/* unknown content type*/
	SII_DRV_HDCP_CONTENT_TYPE__UNKNOWN
};

/* Repeater type */
enum hdcp_repeater_mode {
	TX_HDCP_RPTR_NONE,
	TX_HDCP_RPTR_1X,
	TX_HDCP_RPTR_22,
};

/* KSV data object */
struct sii_drv_hdcp_ksv {
	uint8_t d[KSV_NUM_BYTES];
};

/* KSV data object */
struct sii_drv_hdcp_ksv_list {
	uint16_t length;
	uint8_t *p_list;
	uint8_t *p_list_start;
	uint8_t last_byte;
};

struct sii_drv_hdcp_rpt_b_ksv_list {
	uint16_t length;
	uint8_t p_list[10];
};

struct sii_drv_hdcp2x_rpt_msg {
	bool b_hdcp2_rx_ready;
	bool ske_event;
	bool smng_enable;
	bool rcv_id_change;
	bool rcv_id_enable;
	bool ake_init_rcvd;
	uint32_t seq_num__v;
	uint32_t seq_num__m;
};

struct sii_drv_tx_strmng_2x_msg {
	uint32_t seq_num_m;
	uint8_t stream_k;
	uint8_t stream_i_d;
	uint8_t content_type;
};

struct hdcp2_rcvid {
	uint8_t id[RCVRID_LENGTH];
};

struct sii_drv_hdcp2x_rpt_rcv_id_list {
	uint8_t devcnt;
	uint8_t depth;
	uint16_t length;
	bool dev_exceed;
	bool casc_exceed;
	bool hdcp2ds_rpt;
	bool hdcp1ds_rpt;
	struct hdcp2_rcvid rpt_rcvid[MAX_DEVICE_COUNT];
};

/* downstream device HDCP topology information */
struct sii_drv_hdcp_topology {
	uint32_t seq_num_v;
	uint8_t hdcp1x_device_ds;
	uint8_t hdcp20_repeater_ds;
	uint8_t max_cascade_exceeded;
	uint8_t max_devs_exceeded;
	uint8_t device_count;
	uint8_t depth;

};

struct vsif_hdmi_or_mhl3 {
	struct vsif_common_header common;
	union {
		struct hdmi_vendor_specific_payload hdmi;
		struct mhl3_vendor_specific_payload mhl3;
	} payload;
};

/* VSIF configuration */
struct sii_drv_vsif_caps {
	uint16_t h_active;
	uint16_t v_active;
};

enum sii_drv_dsc_compression_ratio {
	SII_DRV_DSC_COMPRESSION_RATIO__NONE = 0x0,
	SII_DRV_DSC_COMPRESSION_RATIO__2X = 0x2,
	SII_DRV_DSC_COMPRESSION_RATIO__3X = 0x3,
};

/* DSC wrap values */
struct sii_drv_dscwrap {
	uint8_t color_depth;
	uint8_t hsync_polarity;
	uint8_t htotal_high;
	uint8_t htotal_low;
	uint8_t hwidt_high;
	uint8_t hwidth_low;
	uint8_t hfront_high;
};

/* DSC core values */
struct sii_drv_dsccore {
	uint8_t vactive_high;
	uint8_t vactive_low;
	uint8_t hactive_high;
	uint8_t hactive_low;
};

struct sii_drv_vic_id {
	uint8_t cea_vic;
	uint8_t hdmi_vic;
};

/* DSC configuration */
struct sii_drv_dsc_caps {
	struct sii_drv_vic_id vic_id;
	uint8_t compression;
	enum sii_drv_bit_depth colordepth;
	uint16_t h_active;
	uint16_t h_blank;
	uint32_t v_freq;
	uint32_t pixel_freq;
	uint8_t hbr_status;
	enum avi_color_space color_space;
};
#if 0
struct cp8630_driver_context {
	struct sii8630_platform_data *pdata;
	void *drv_obj;
	bool isr_lock;
	struct workqueue_struct *timer_wrk_queue;
	/* single threaded (st) workqueue*/
	struct workqueue_struct *hid_st_wrk_queue;
	struct list_head timer_list;
	int gen_irq;
	int emsc_irq;
	struct cdev cp8630_cdev;
	struct device *cp8630_dev;
	uint8_t dev_flags;
#define DEV_FLAG_SHUTDOWN	0x01	/* Device is shutting down */
#define DEV_FLAG_COMM_MODE	0x02	/* Halt INTR processing */

	uint16_t debug_i2c_address;
	uint16_t debug_i2c_offset;
	uint16_t debug_i2c_xfer_length;

	dev_t dev_num;
	struct class *cp8630_class;
};
#endif

enum sii_drv_i_state {
	SII_DRV_TX_OFF__RX_OFF,
	SII_DRV_TX_OFF__RX_ON,
	SII_DRV_TX_OFF__RX_TMDS_ON,
	SII_DRV_TX_OFF__RX_HDCP_ON,
	SII_DRV_TX_ON__RX_OFF,
	SII_DRV_TX_ON__RX_ON,
	SII_DRV_TX_ON__RX_TMDS_ON,
	SII_DRV_TX_ON__RX_HDCP_ON,
	SII_DRV_ISTATE_INVALID
};
enum sway_pll_owner {
	SWAY_INUSE_FOR__NONE,
	SWAY_INUSE_FOR__420_422,
	SWAY_INUSE_FOR__DSC,
	SWAY_INUSE_FOR__BIST,
};

enum tx_requester {
	TX_REQUESER__NONE,
	TX_REQUESER__DRIVER,
	TX_REQUESER__BIST,
};

enum vixs_fmt {
	VIXS_FMT__EMULATION,
	VIXS_FMT__4K_2K_60HZ,
	VIXS_FMT__4K_2K_50HZ,
	VIXS_FMT__TOTAL_FMTS,
};

typedef struct driver_obj {
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;
	void *rx_hdmi_obj;
	void *tx_hdmi_obj;
	void *vidpath_obj;
	void *dsc_obj;
	void *hdcp_obj;
	uint16_t chip_id;
	struct sii_edid edid;
	struct sii_lib_edid_parsed_data parse_edid;
	bool b_ds_edid_available;
	enum sii_drv_rx_input_type rx_connection_type;
	enum sii_tmds_mode rx_tmds_mode;
	enum sii_tmds_mode tx_tmds_mode;
	enum sii_tmds_mode tx_tmds_mode_user;
	bool b_rx_hdmi_connected;
	bool tx_rsen;
	bool b_tx_connected;
	bool tx_tmds_enabled;
	uint8_t tx_pstable;
	bool b_rx_hdcp_enabled;
	bool b_rx_tmds_enabled;
	enum sii_drv_hdcp_status rx_hdcp_status;
	enum sii_drv_hdcp_status tx_hdcp_status;
	enum sii_drv_hdcp_gen_event tx_hdcp_events;
	enum sii_drv_hdcp_link_version tx_hdcp_auth_type;
	struct sii_drv_hdcp2x_rpt_rcv_id_list hdcp2x_rpt_ksv;
	struct sii_drv_tx_strmng_2x_msg stmng_dat;
	enum sii_drv_i_state drv_istate;
	/*enum sii_drv_i_states_change_handler drv_i_state_change_handler;*/

	/*void (*sii_drv_istate_change_handler_handler
	      [SII_DRV_ISTATE_change_handler__TOTAL_STATES])(
		void *);*/
	enum sii_drv_clr_spc out_clr_spc;
	struct sii_info_frame avi_out_data;
	uint8_t hdcp_arbitration_mask;
	struct sii_drv_hdcp_ksv_list ksvlist;
	bool b_is_vixs_format_handled;
	void *inst_temp_tmds_handler;
	void *inst_auth_deglitch;
	void *s_tx_tmds_enable_handler;
	void *timer_tx_tmds_enable_poll;
	bool b_rx_pll_lock_status;
	bool b_tx_pll_lock_status;
	bool b_tx_is_hdcp_req;
	bool b_tx_tmds_on_req;
	bool is_tpg_enabled;
	enum sii_drv_bist_owner bist_owner;
	enum sway_pll_owner sway_owner;
	uint32_t input_tmds_clk;
	uint32_t output_tmds_clk;
	struct avi_info_frame avi_data;
	struct audio_info_frame aif_data;
	struct sii_info_frame acr_if;
	struct acr_info_frame acr_if_data;
	struct vsif_hdmi_or_mhl3 vsif_hdmi_or_mhl3;
	bool dsc_support;
	/*flag to enable dsc or not*/
	bool is_dsc_reqd;
	bool is_dsc_enabled;
	enum sii_drv_dsc_compression_ratio dsc_compression;
	bool is_420toany_conv_en;
	enum sii_drv_aud_sampling_frq aud_samplg_freq;
#ifdef CONFIG_HANDLE_BOARD_RESET
	void *chip_reset_poll;
#endif
#ifdef CONFIG_ENHANCED_MODE_PROT
	struct sii_drv_tx_scdc_manufacturer_status manf_spec_data;
	bool is_emc_queried;
	uint8_t ds_em_caps;
	enum dsc_mode dsc_mode;
	bool emm_response_received;
	uint8_t emm_response;
#endif
	/*upsampled from 422 to 444 by banner*/
	bool upsampled_to_444;
	/*upsampled from 420 to 422 by spiderman*/
	bool upsampled_to_422;
	bool init_done;
	uint16_t input_unstable_counter;
}stdriver_obj_t,*pstdriver_obj_t;


extern struct driver_obj g_stdrv_obj;

/* dualmode tx driver constructor */
int sii_drv_init(void);

/* dualmode tx driver destructor */
//void sii_drv_exit(struct device *dev);

//int sii_drv_handle_power_change_request(struct device *parent_dev,
//	bool power_up);

/* get +5V power input status */
int sii_drv_rx_plus5v_status_get(void *p_obj, void *plus5v_status);

/* set upstream edid */
int sii_drv_rx_edid_set(void *p_obj, struct sii_edid *p_edid);

/* upstream hot_plug control. */
int sii_drv_rx_hot_plug_set(void *p_obj, bool on_off);

/* upstream TMDS mode status. */
int sii_drv_rx_tmds_mode_get(void *p_obj, enum sii_tmds_mode *tmds_mode);

/* upstream av_mute status. */
int sii_drv_rx_av_mute_get(void *p_obj, bool *b_av_mute);

/* upstream info-frame status. */
int sii_drv_rx_infoframe_get(void *p_obj, struct sii_info_frame
	*p_info_frame);

/* upstream info-frame enable/disable status. */
int sii_drv_rx_infoframe_on_off_get(void *p_obj, enum sii_info_frame_id if_id,
		bool *b_on_off);

/* clear info-frame status. */
int sii_drv_rx_clear_info_frame(void *p_obj, enum sii_info_frame_id *if_id);

/* get input colorspace */
int sii_drv_rx_input_color_space_get(void *p_obj, enum sii_drv_clr_spc
		*p_clr_spc);

/* get input color conversion standard. */
int sii_drv_rx_input_color_conv_std_get(void *p_obj, enum sii_drv_conv_std
	*p_conv_std);

/* get input color depth. */
int sii_drv_rx_input_color_depth_get(void *p_obj, enum sii_drv_bit_depth
	*p_clr_depth);

/* upstream hdcp link version get. */
int sii_drv_rx_hdcp_link_ver_get(void *p_obj,
		enum sii_drv_hdcp_link_version *p_hdcp_link_ver);

/* upstream content protection status. */
int sii_drv_rx_hdcp_protection_get(void *p_obj, bool *b_hdcp_proc);

/* upstream HDCP status. */
int sii_drv_rx_hdcp_status_get(void *p_obj,
		enum sii_drv_hdcp_status *p_hdcp_status);

/* send downstream BKSV list to upstream device (for repeater) */
int sii_drv_rx_hdcp_bksv_list_set(void *p_obj,
		const struct sii_drv_hdcp_ksv_list *p_bksv_list);

/* upstream HDCP topology configuration control. */
int sii_drv_rx_hdcp_topology_set(void *p_obj,
		const struct sii_drv_hdcp_topology *p_topology);

/* set output color space format. */
int sii_drv_output_color_space_set(void *p_obj, enum sii_drv_clr_spc
	clr_spc);

/* output video bit depth control. */
int sii_drv_output_bit_depth_set(void *p_obj, enum sii_drv_bit_depth
	*bit_depth);

/* get hv_sync polarity status. */
int sii_drv_rx_hv_sync_polarity_get(void *p_obj,
		enum sii_hv_sync_pol *hv_sync_pol);

/* set hv_sync polarity. */
int sii_drv_rx_out_h_v_sync_polarity_set(void *p_obj,
		enum sii_hv_sync_pol hv_sync_pol);

/* downstream edid interrogation. */
int sii_drv_tx_edid_get(void *p_obj, struct sii_edid *p_edid);

/* downstream edid's lip_sync interrogation. */
int sii_drv_tx_lip_sync_info_get(void *p_obj,
		struct sii_edid_lip_sync *lip_sync);

/* downstream connection state */
int sii_drv_tx_connection_state_get(void *p_obj, bool *connection_state);

/* downstream hot-plug status. */
int sii_drv_tx_hot_plug_get(void *p_obj, bool *b_hot_plug);

/* downstream r_sen (TMDS input impedance) status. */
int sii_drv_tx_rsen_get(void *p_obj, bool *b_rsen);

/* downstream TMDS mode control. */
int sii_drv_tx_tmds_mode_set(void *p_obj, enum sii_tmds_mode tmds_mode);

/* downstream current used TMDS mode status. */
int sii_drv_tx_tmds_mode_status_get(void *p_obj, enum sii_tmds_mode *tmds_mode);

/* downstream av_mute control. */
int sii_drv_tx_av_mute_set(void *p_obj, bool on_off);

/* downstream info-frame control. */
int sii_drv_tx_infoframe_set(void *p_obj,
	const struct sii_info_frame *p_info_frame);

/* downstream info-frame enable/disable control. */
int sii_drv_tx_infoframe_on_off_set(void *p_obj, enum sii_info_frame_id
	if_id, bool on_off);

/*int sii_drv_tx_hdcp_auth_type_set(void *p_obj,
	enum sii_drv_hdcp_link_version hdcp_link_ver);*/

/* downstream content protection control. */
int sii_drv_tx_hdcp_protection_set(void *p_obj,
	enum sii_drv_hdcp_link_version hdcp_link_ver);

/* get downstream HDCP status. */
int sii_drv_tx_hdcp_status_get(void *p_obj,
		enum sii_drv_hdcp_status *p_hdcp_status);

/*get downstream HDCP failure reason*/
int sii_drv_tx_hdcp_failure_reason_get(void *p_obj,
	enum sii_drv_hdcp_failure *p_hdcp_fail_reason);

/* get downstream HDCP version. */
int sii_drv_tx_hdcp_version_get(void *p_obj,
			enum sii_drv_hdcp_link_version *p_ds_hdcp_ver);

/* get downstream HDCP events */
int sii_drv_tx_hdcp_events_get(void *p_obj,
	enum sii_drv_hdcp_gen_event *p_hdcp_events);

/* stream manage msg set */
int sii_drv_tx_hdcp_stream_manage_msg_set(void *p_obj,
	struct sii_drv_tx_strmng_2x_msg *stream_manage_msg);

/* interrogates downstream BKSV list from downstream device */
int sii_drv_tx_hdcp_bksv_list_get(void *p_obj,
		struct sii_drv_hdcp_ksv_list *p_bksv_list);

/* user acknowledge for KSV list approval */
int sii_drv_tx_hdcp_bksv_list_approval_set(void *p_obj, bool b_approved);

/* downstream HDCP topology configuration status. */
int sii_drv_tx_hdcp_topology_get(void *p_obj,
		struct sii_drv_hdcp_topology *p_topology);

/* downstream HDCP2.2 content type configuration status. */
int sii_drv_tx_hdcp2_content_type_set(void *p_obj,
		enum sii_drv_hdcp_content_type *p_content_type);

int sii_drv_tx_hdcp2_content_type_get(void *p_obj,
		enum sii_drv_hdcp_content_type *p_content_type);

int sii_drv_tx_hdcp_repeater_info(
	void *p_obj, enum hdcp_repeater_mode *repeater_mode);

/* output video bit depth control. */
int sii_drv_tx_output_bit_depth_set(void *p_obj, enum sii_drv_bit_depth
	bit_depth);

/* hv_sync polarity set. */
int sii_drv_tx_hv_sync_polarity_set(void *p_obj,
		enum sii_hv_sync_pol *hv_sync_pol);

/* configure video path input color space,
 * bit depth and color conversion standard.
 */
int sii_drv_color_info_config(void *p_obj,
		struct sii_drv_color_info_cfg clr_info);

/* reads and prints the manufacturer SCDC registers */
int sii_drv_tx_scdc_manufacturer_registerstatus(void *p_obj,
		struct sii_drv_tx_scdc_manufacturer_status *manf_status);

/* reads and prints the peer SCDC registers */
int sii_drv_tx_scdc_peer_registerstatus(void *p_obj,
		struct sii_drv_tx_scdc_register_status *peer_status);

/* reads source scramble status */
int sii_drv_tx_scdc_source_scramble_status(void *p_obj,
		enum sii_drv_scdc_source_scrmble_status *scramble_status);

/* read and reset peer update status registers */
int sii_drv_tx_scdc_read_update_registers(void *p_obj,
	struct sii_drv_tx_scdc_status_update_regs *status_update_values);

/* enables SCDC read request */
int sii_drv_tx_scdc_read_request_enable(void *p_obj, bool enable);

/* enables SCDC test read request test */
int sii_drv_tx_scdc_read_request_test(void *p_obj, bool msdelay);

/* enables SCDC scrambling */
int sii_drv_tx_scdc_scramble_enable(void *p_obj, bool enable);

int sii_drv_bist_pattern_generate(void *p_obj,
	struct sii_drv_bist_config *bist_cfg);

/* set SCDC scramble timeout value */
int sii_drv_tx_scdc_debug_enable(void *p_obj, bool debug_enable);

/* configure DCS encoder */
int sii_drv_tx_dsc_configure(void *p_obj, uint8_t compression);

/* enable DCS encoder */
int sii_drv_tx_dsc_enable(void *p_obj, bool on_off);

/* reads DSC compression status */
int sii_drv_tx_dsc_compression_status(void *p_obj, bool *compression_status);

/* reads DSC compression ratio */
int sii_drv_tx_dsc_compression_ratio(void *p_obj,
	enum sii_drv_dsc_compression_ratio *compression_ratio);

#if 0
int sii_drv_sysfs_init(struct cp8630_driver_context *drv_context,
				struct device *parent_dev);
int sii_drv_sysfs_exit(struct cp8630_driver_context *drv_context);

void sii_drv_tx_uevent_notify(struct cp8630_driver_context *drv_context,
	u32 event, u32 event_param, void *data);
#endif

#ifdef RX_DESCRAMBLE_THROUGH_API
int sii_drv_rx_scdc_tmds_config(void *p_obj, bool scramble_en,
	bool tmds_bit_clk_ratio);
int sii_drv_rx_scramble_status(void *p_obj, uint8_t *scramble_stat);
#endif
void mapp_Sii9630EdidSetForExternal(u8 *pu8Edid,u16 u16Len);

int sii_drv_tx_ddc_reset(void *p_obj, bool ddc_reset_en);
void mapp_Sil9630_main(void);

#endif /* __SI_DRV_CP8630_H__*/
