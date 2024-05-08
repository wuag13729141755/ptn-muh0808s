/*
  2016 (C) Lattice Semiconductor Corporation

  No part of this work may be reproduced, modified, distributed, transmitted,
  transcribed, or translated into any language or computer format, in any form
  or by any means without written permission of Lattice Semiconductor, Inc.,
  111 SW 5th Ave, Suite 700, Portland, OR 97204.


*/

#ifndef _SII_API_H
#define _SII_API_H

#include "Sii9396_sii_common.h"

enum sii_api_status {
	SII_API_SUCCESS = 0,
	SII_API_FAILURE = 1
};

enum sii_boot_status
{
	SII_BOOT_STAT__SUCCESS,
	SII_BOOT_STAT__IN_PROGRESS,
	SII_BOOT_STAT__FAILURE,
};

enum sii_av_link_type
{
	SII_AV_LINK__NONE,
	SII_AV_LINK__HDMI1,
	SII_AV_LINK__HDMI2,
	SII_AV_LINK__DVI,
};

enum sii_tmds_pin_swap
{
	SII_TMDS_PIN_SWAP__NONE,
	SII_TMDS_PIN_SWAP__DATA,
	SII_TMDS_PIN_SWAP__FULL,
};

enum sii_hdcp_status
{
	SII_HDCP_STATUS__OFF,
	SII_HDCP_STATUS__SUCCESS_1X,
	SII_HDCP_STATUS__SUCCESS_22,
	SII_HDCP_STATUS__AUTHENTICATING,
	SII_HDCP_STATUS__FAILED,
};

enum sii_hdcp_version
{
	SII_HDCP_VERSION__NONE,
	SII_HDCP_VERSION__1X,
	SII_HDCP_VERSION__20,
	SII_HDCP_VERSION__22
};

struct sii_hdcp_ksv
{
	uint8_t key[5];
};

struct sii_hdcp_topology {
	uint32_t seq_num_v;
	bool hdcp1_device_ds;
	bool hdcp2_device_ds;
	bool max_cascade_exceeded;
	bool max_device_exceeded;
	uint8_t device_count;
	uint8_t depth;
};

struct sii_hdcp_stream_manage_info {
	uint32_t seq_num_m;
	uint16_t k;
	uint16_t stream_id_type[1];
};

enum sii_flash_status {
	SII_FLASH_STATUS__SUCCESS,
	SII_FLASH_STATUS__IN_PROGRESS,
	SII_FLASH_STATUS__FAILURE,
};

enum sii_event {
	SII_EVENT__BOOT_DONE,
	SII_EVENT__RX_AV_LINK_CHNG,
	SII_EVENT__AV_MUTE_CHNG,
	SII_EVENT__SCDT_CHNG,
	SII_EVENT__DS_CONNECT_STATUS_CHNG,
	SII_EVENT__DS_EDID_STATUS_CHNG,
	SII_EVENT__RX_HDCP_STATUS_CHNG,
	SII_EVENT__HDR_REC,
	SII_EVENT__VSIF_REC,
	SII_EVENT__RX_VIDEO_RESOLUTION_CHNG,
	SII_EVENT__RX_TMDS_CLOCK_CHNG,
	SII_EVENT__TX_RXSENSE_STATUS_CHNG,
	SII_EVENT__TX_HDCP_STATUS_CHNG,
	SII_EVENT__RX_HDCP_AUTH_START,
	SII_EVENT__RX_HDCP_AUTH_DONE,
	SII_EVENT__RX_HDCP_RCVID_DONE,
	SII_EVENT__RX_HDCP_SMNG_CHNG,
	SII_EVENT__RX_HDCP_AUTH_FAIL,
	SII_EVENT__TX_HDCP_AUTH_DONE,
	SII_EVENT__TX_HDCP_RCVID_CHNG,
	SII_EVENT__TX_HDCP_SMNG_DONE,
	SII_EVENT__TX_HDCP_AUTH_FAIL,
	SII_EVENT__AVI_REC,
	SII_EVENT__AIF_REC,
	SII_EVENT__RX_SCRAMBLE_STATUS_CHNG,
	SII_EVENT__BASE = 32
};

enum sii_infoframe_type {
	SII_INFOFRAME__VSIF,
	SII_INFOFRAME__HDR,
	SII_INFOFRAME__AVI,
	SII_INFOFRAME__AIF
};

enum sii_reg_page {
	SII_CEC_PAGE,
	SII_SCDC_PAGE
};

enum sii_color_space {
	SII_COLOR_SPACE__UNKNOWN,
	SII_COLOR_SPACE__RGB,
	SII_COLOR_SPACE__YCBCR444,
	SII_COLOR_SPACE__YCBCR422,
	SII_COLOR_SPACE__YCBCR420,
};

enum sii_hdcp_content_type {
	SII_HDCP_CONTENT_TYPE__TYPE0,
	SII_HDCP_CONTENT_TYPE__TYPE1
};

enum sii_audio_mode {
	SII_AUDIO__NONE,
	SII_AUDIO__I2S,
	SII_AUDIO__TDM,
	SII_AUDIO__SPDIF
};

struct sii_audio_format {
    uint8_t aifData[10]; //!< Audio Info-Frame data. Refer to CEA861 specification for details.
    uint8_t csData[7];   //!< Channel Status data. Refer to IEC 60958-3 specification for details.
    uint8_t audioMode;
};

enum sii_audio_mclk_ratio {
	SII_AUDIO_MCLK_RATIO__128X,
	SII_AUDIO_MCLK_RATIO__256X,
	SII_AUDIO_MCLK_RATIO__384X,
	SII_AUDIO_MCLK_RATIO__512X
};

enum sii_scramble_state {
	SII_SCRAMBLE_OFF,
	SII_SCRAMBLE_ON_ABOVE_340M,
	SII_SCRAMBLE_ON_BELOW_340M
};

struct sii_tmds_lock_status {
	bool clock_detected;
	bool ch0_locked;
	bool ch1_locked;
	bool ch2_locked;
};

struct sii_char_error_count {
	bool	ch0_valid;
	bool	ch1_valid;
	bool	ch2_valid;
	uint8_t	checksum;
	uint16_t ch0_char_error_count;
	uint16_t ch1_char_error_count;
	uint16_t ch2_char_error_count;
};

struct sii_device_id_string {
	uint8_t str[8];
};

struct sii_audio_info {
	uint32_t n;
	uint32_t cts;
	bool layout1;
	bool dsd;
	bool hbra;
	uint8_t ch_stat[5];
};

enum sii_tmds_clock_ratio {
	SII_TMDS_CLK_RATIO__1_10,
	SII_TMDS_CLK_RATIO__1_40
};

struct sii_video_info {
	uint16_t h_res_total;
	uint16_t v_res_total;
	uint16_t h_res_active;
	uint16_t v_res_active;
	uint16_t tmds_clock;
	bool interlace;
	uint8_t color_depth;
	uint8_t color_space;
	uint8_t ratio;
};

struct sii_tmds_swing_level {
	uint8_t clk_level;
	uint8_t data0_level;
	uint8_t data1_level;
	uint8_t data2_level;
};

enum sii_product_id {
	SII_PRODUCT_ID__NONE,
	SII_PRODUCT_ID__RX,
	SII_PRODUCT_ID__TX,
	SII_PRODUCT_ID__RPT_SINGLE,
};

enum sii_eq_source {
	SII_EQ_SRC__HDMI1,
	SII_EQ_SRC__HDMI2,
};

struct sii_eq_setting {
	uint8_t source;
	uint8_t main_eq[8];
	uint8_t extend_eq[6];
};

#define SII_EDID_BLOCK_SIZE					128
#define SII_RX_INFOFRAME_MAX_LENGTH			32
#define SII_RX_EQ_SIZE						8

#define SII_EVENT_MASK__BOOT_DONE			1 << SII_EVENT__BOOT_DONE
#define SII_EVENT_MASK__RX_AV_LINK_CHNG		1 << SII_EVENT__RX_AV_LINK_CHNG
#define SII_EVENT_MASK__AV_MUTE_CHNG		1 << SII_EVENT__AV_MUTE_CHNG
#define SII_EVENT_MASK__SCDT_CHNG			1 << SII_EVENT__SCDT_CHNG
#define SII_EVENT_MASK__DS_CONNECT_STATUS_CHNG \
										1 << SII_EVENT__DS_CONNECT_STATUS_CHNG
#define SII_EVENT_MASK__DS_EDID_STATUS_CHNG	1 << SII_EVENT__DS_EDID_STATUS_CHNG
#define SII_EVENT_MASK__RX_HDCP_STATUS_CHNG	1 << SII_EVENT__RX_HDCP_STATUS_CHNG
#define SII_EVENT_MASK__HDR_REC				1 << SII_EVENT__HDR_REC
#define SII_EVENT_MASK__VSIF_REC			1 << SII_EVENT__VSIF_REC
#define SII_EVENT_MASK__RX_VIDEO_RESOLUTION_CHNG \
									1 << SII_EVENT__RX_VIDEO_RESOLUTION_CHNG
#define SII_EVENT_MASK__RX_TMDS_CLOCK_CHNG	1 << SII_EVENT__RX_TMDS_CLOCK_CHNG
#define SII_EVENT_MASK__TX_RXSENSE_STATUS_CHNG \
									1 << SII_EVENT__TX_RXSENSE_STATUS_CHNG
#define SII_EVENT_MASK__TX_HDCP_STATUS_CHNG	1 << SII_EVENT__TX_HDCP_STATUS_CHNG
#define SII_EVENT_MASK__RX_HDCP_AUTH_START	1 << SII_EVENT__RX_HDCP_AUTH_START
#define SII_EVENT_MASK__RX_HDCP_AUTH_DONE	1 << SII_EVENT__RX_HDCP_AUTH_DONE
#define SII_EVENT_MASK__RX_HDCP_RCVID_DONE	1 << SII_EVENT__RX_HDCP_RCVID_DONE
#define SII_EVENT_MASK__RX_HDCP_SMNG_CHNG	1 << SII_EVENT__RX_HDCP_SMNG_CHNG
#define SII_EVENT_MASK__RX_HDCP_AUTH_FAIL	1 << SII_EVENT__RX_HDCP_AUTH_FAIL
#define SII_EVENT_MASK__TX_HDCP_AUTH_DONE	1 << SII_EVENT__TX_HDCP_AUTH_DONE
#define SII_EVENT_MASK__TX_HDCP_RCVID_CHNG	1 << SII_EVENT__TX_HDCP_RCVID_CHNG
#define SII_EVENT_MASK__TX_HDCP_SMNG_DONE	1 << SII_EVENT__TX_HDCP_SMNG_DONE
#define SII_EVENT_MASK__TX_HDCP_AUTH_FAIL	1 << SII_EVENT__TX_HDCP_AUTH_FAIL
#define SII_EVENT_MASK__AVI_REC				1 << SII_EVENT__AVI_REC
#define SII_EVENT_MASK__AIF_REC				1 << SII_EVENT__AIF_REC
#define SII_EVENT_MASK__RX_SCRAMBLE_STATUS_CHNG \
									1 << SII_EVENT__RX_SCRAMBLE_STATUS_CHNG

struct sii_config {
	bool device_reset;
	uint32_t reset_time;
};

sii_inst_t sii_create(sii_hwid_t id,
	void (*callback_function)(sii_inst_t inst, uint8_t event),
	struct sii_config *config);
enum sii_api_status sii_delete(sii_inst_t inst);

bool sii_update(sii_inst_t inst);

#if 0
enum sii_api_status sii_get_configure(
	sii_inst_t inst, struct sii_config *config);
enum sii_api_status sii_set_configure(
	sii_inst_t inst, struct sii_config *config);
#endif
enum sii_api_status sii_set_event_mask(
	sii_inst_t inst, uint32_t mask);
enum sii_api_status sii_get_event_mask(
	sii_inst_t inst, uint32_t *mask);
enum sii_api_status sii_handle(sii_inst_t inst);
enum sii_api_status sii_wait_for_boot_done(sii_inst_t inst);

enum sii_api_status sii_get_rx_plus5v(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_rx_hpd(
	sii_inst_t inst, bool *hpd);
enum sii_api_status sii_get_rx_av_mute(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_rx_link_type(
	sii_inst_t inst, enum sii_av_link_type *type);
enum sii_api_status sii_get_rx_edid_content(
	sii_inst_t inst,
	uint8_t block_num, uint8_t *edid_buffer);
enum sii_api_status sii_get_rx_info_frame_hdr(
	sii_inst_t inst,
	uint8_t *infoframe, uint8_t *size);
enum sii_api_status sii_get_rx_info_frame_vsif(
	sii_inst_t inst,
	uint8_t *infoframe, uint8_t *size);
enum sii_api_status sii_get_rx_info_frame_avi(
		sii_inst_t inst,
		uint8_t *infoframe, uint8_t *size);
enum sii_api_status sii_get_rx_info_frame_aif(
		sii_inst_t inst,
		uint8_t *infoframe, uint8_t *size);
enum sii_api_status sii_get_rx_tmds_lock_status(
	sii_inst_t inst,
	struct sii_tmds_lock_status *status);
enum sii_api_status sii_get_rx_device_id_string(
	sii_inst_t inst,
	struct sii_device_id_string *string);
enum sii_api_status sii_get_rx_char_error_count(
	sii_inst_t inst,
	struct sii_char_error_count *count);
enum sii_api_status sii_get_rx_video_info(
	sii_inst_t inst, struct sii_video_info *info);
enum sii_api_status sii_get_rx_audio_info(
	sii_inst_t inst, struct sii_audio_info *info);
enum sii_api_status sii_get_cec_reg(
	sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value);
enum sii_api_status sii_get_rx_scdc_reg(
	sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value);
enum sii_api_status sii_set_rx_hpd(
	sii_inst_t inst, bool hpd);
enum sii_api_status sii_set_rx_term(
	sii_inst_t inst, bool term);
enum sii_api_status sii_set_rx_edid_content(
	sii_inst_t inst,
	uint8_t block_num, uint8_t *edid_buffer);
enum sii_api_status sii_set_rx_eq(
	sii_inst_t inst, struct sii_eq_setting *setting);
enum sii_api_status sii_set_cec_reg(
	sii_inst_t inst, uint8_t offset, uint8_t length, uint8_t *value);
enum sii_api_status sii_set_rx_scdc_reg(
	sii_inst_t inst, uint8_t offset, uint8_t value);
enum sii_api_status sii_set_rx_audio_mclk_ratio(
	sii_inst_t inst, enum sii_audio_mclk_ratio ratio);
enum sii_api_status sii_set_rx_audio_extraction(
	sii_inst_t inst, enum sii_audio_mode mode);
enum sii_api_status sii_get_rx_scramble_full(
	sii_inst_t inst, enum sii_scramble_state *state);
enum sii_api_status sii_get_tx_tmds_status(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_tx_av_mute(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_tx_tmds_pin_swap(
	sii_inst_t inst, bool *pin_swap);
	// enum sii_tmds_pin_swap *pin_swap);
enum sii_api_status sii_get_tx_hpd(
	sii_inst_t inst, bool *hpd);
enum sii_api_status sii_get_tx_rxsense(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_tx_ds_edid_status(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_tx_ds_edid_content(
	sii_inst_t inst,
	uint8_t block_num, uint8_t *edid_buffer);
enum sii_api_status sii_get_tx_tmds_swing_level(
	sii_inst_t inst,
	uint8_t *clk_level, uint8_t *data_level);
enum sii_api_status sii_set_tx_tmds_pin_swap(
	sii_inst_t inst, enum sii_tmds_pin_swap pin_swap);
enum sii_api_status sii_set_tx_tmds_status(
	sii_inst_t inst, bool status);
enum sii_api_status sii_set_tx_av_mute(
	sii_inst_t inst, bool status);
enum sii_api_status sii_set_tx_tmds_swing_level(
	sii_inst_t inst,
	struct sii_tmds_swing_level *swing_level);
enum sii_api_status sii_get_rx_hdcp_ddc(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_get_rx_hdcp_status(
	sii_inst_t inst, enum sii_hdcp_status *status);
enum sii_api_status sii_get_rx_hdcp_us_version(
	sii_inst_t inst,
	enum sii_hdcp_version *version);
enum sii_api_status sii_get_rx_hdcp_rxid(
	sii_inst_t inst, struct sii_hdcp_ksv *rxid);
enum sii_api_status sii_get_rx_hdcp_bksv(
	sii_inst_t inst, struct sii_hdcp_ksv *bksv);
enum sii_api_status sii_set_rx_hdcp_ddc(
	sii_inst_t inst, bool status);
enum sii_api_status sii_get_rx_hdcp_repeater_mode(
	sii_inst_t inst, bool *enable);
enum sii_api_status sii_set_rx_hdcp_repeater_mode(
	sii_inst_t inst, bool enable);
enum sii_api_status sii_trigger_rx_hdcp_rxid_list_to_send(
	sii_inst_t inst);
enum sii_api_status sii_set_rx_hdcp_rxid_list(
	sii_inst_t inst, uint8_t device_count,
	struct sii_hdcp_ksv *rxid_list);
enum sii_api_status sii_set_rx_hdcp_topology(
	sii_inst_t inst,
	struct sii_hdcp_topology *topology);
enum sii_api_status sii_get_rx_hdcp_stream_manage_info(
	sii_inst_t inst,
	struct sii_hdcp_stream_manage_info *info);
enum sii_api_status sii_get_product_id(
	sii_inst_t inst, enum sii_product_id *id);
enum sii_api_status sii_get_chip_id(
	sii_inst_t inst, uint16_t *chip_id);
enum sii_api_status sii_get_chip_revision(
	sii_inst_t inst, uint8_t *revison);
enum sii_api_status sii_get_firmware_version(
	sii_inst_t inst, uint8_t *fw_ver);
enum sii_api_status sii_get_release_timestamp(
	sii_inst_t inst, uint8_t *timestamp);
enum sii_api_status sii_get_boot_status(
	sii_inst_t inst, enum sii_boot_status *status);
enum sii_api_status sii_get_host_mode(
	sii_inst_t inst, bool *status);
enum sii_api_status sii_set_host_mode(
	sii_inst_t inst, bool status);
enum sii_api_status sii_flash_init(
	sii_inst_t inst);
enum sii_api_status sii_get_flash_status(
	sii_inst_t inst, enum sii_flash_status *status);
enum sii_api_status sii_flash_update(
	sii_inst_t inst, const char* file_name);
void sii_reset_device(sii_inst_t inst, uint32_t time_msec);
enum sii_api_status sii_platform_init(uint8_t *dev_num,
	sii_hwid_t* hwid, uint32_t *dev_id);
enum sii_api_status sii_platform_term(void);
enum sii_api_status sii_set_tx_scramble(
	sii_inst_t inst, bool enable);
enum sii_api_status sii_set_tx_scramble_full(
	sii_inst_t inst, enum sii_scramble_state state);
enum sii_api_status sii_get_tx_char_error_count(
	sii_inst_t inst,
	struct sii_char_error_count *count);
enum sii_api_status sii_get_tx_device_id_string(
	sii_inst_t inst,
	struct sii_device_id_string *string);
enum sii_api_status sii_set_tx_color_space(
	sii_inst_t inst, enum sii_color_space type);
enum sii_api_status sii_set_tx_hdcp_protection(
	sii_inst_t inst, bool enable);
enum sii_api_status sii_set_tx_hdcp_content_type(
	sii_inst_t inst,
	enum sii_hdcp_content_type type);
enum sii_api_status sii_get_tx_hdcp_ds_version(
	sii_inst_t inst,
	enum sii_hdcp_version *version);
enum sii_api_status sii_get_tx_hdcp_status(
	sii_inst_t inst, enum sii_hdcp_status *status);
enum sii_api_status sii_set_tx_audio_insertion(
	sii_inst_t inst,
	struct sii_audio_format *format);
enum sii_api_status sii_get_tx_hdcp_rcvid(
	sii_inst_t inst, struct sii_hdcp_ksv *rcvid);
enum sii_api_status sii_get_tx_hdcp_ds_rxid_list(
	sii_inst_t inst, struct sii_hdcp_ksv *ds_rxid_list, uint8_t length);
enum sii_api_status sii_get_tx_hdcp_ds_repeater_bit(
	sii_inst_t inst, bool *repeaterBit);
enum sii_api_status sii_get_tx_hdcp_ds_topology(
	sii_inst_t inst, struct sii_hdcp_topology *topology);
enum sii_api_status sii_set_tx_hdcp_stream_manage_info(
	sii_inst_t inst, struct sii_hdcp_stream_manage_info *info);

/* Type mapping to AR style */
//typedef sii_inst_t SiiInst_t;
//typedef sii_hwid_t SiiHwid_t;
typedef enum sii_api_status SiiRetVal_t;
typedef void (*SiiEventCallbackFunc_t)(sii_inst_t inst, uint8_t eventFlags);
typedef enum sii_boot_status SiiBootStat_t;
typedef enum sii_event SiiEvent_t;
typedef struct sii_config SiiConfig_t;
typedef enum sii_product_id SiiProductId_t;
typedef enum sii_flash_status SiiFlashStatus_t;
typedef enum sii_av_link_type SiiAvLink_t;
typedef struct sii_video_info SiiVideoInfo_t;
typedef struct sii_audio_info SiiAudioInfo_t;
typedef enum sii_tmds_pin_swap SiiTmdsPinSwap_t;
typedef struct sii_hdcp_ksv SiiHdcpKsv_t;
typedef enum sii_hdcp_status SiiHdcpStatus_t;
typedef enum sii_hdcp_version SiiHdcpVer_t;
typedef struct sii_tmds_lock_status SiiTmdsLockStatus_t;
typedef struct sii_char_error_count SiiCharErrorCount_t;
typedef struct sii_device_id_string SiiDeviceIdString_t;
typedef enum sii_color_space SiiColorSpace_t;
typedef enum sii_hdcp_content_type SiiHdcpContentType_t;
typedef enum sii_audio_mode SiiAudioMode_t;
typedef enum sii_audio_mclk_ratio SiiAudioMclkRatio_t;
typedef struct sii_tmds_swing_level SiiTmdsSwingLevel_t;
typedef struct sii_hdcp_topology SiiHdcpTopology_t;
typedef struct sii_hdcp_stream_manage_info SiiHdcpStreamManageInfo_t;
typedef struct sii_eq_setting SiiEqSetting_t;
typedef enum sii_scramble_state SiiScrambleState_t;

/* Function name mapping to AR style */
#define SiiCreate					sii_create
#define SiiDelete					sii_delete
#if 0
#define SiiConfigureGet				sii_get_configure
#define SiiConfigureSet				sii_set_configure
#endif
#define SiiHandle					sii_handle
#define SiiEventFlagsMaskGet		sii_get_event_mask
#define SiiEventFlagsMaskSet		sii_set_event_mask

#define SiiProductIdGet				sii_get_product_id
#define SiiChipIdGet				sii_get_chip_id
#define SiiChipRevisionGet			sii_get_chip_revision
#define SiiFirmwareVersionGet		sii_get_firmware_version
#define SiiReleaseTimeStampGet		sii_get_release_timestamp
#define SiiBootStatusGet			sii_get_boot_status
#define SiiHostModeGet				sii_get_host_mode
#define SiiHostModeSet				sii_set_host_mode
#define SiiFlashInit				sii_flash_init
#define SiiFlashUpdate				sii_flash_update
#define SiiFlashStatusGet			sii_get_flash_status
#define SiiDeviceReset				sii_reset_device
#define SiiWaitForBootDone			sii_wait_for_boot_done
#define SiiRxCECRegRead				sii_get_rx_cec_reg
#define SiiRxCECRegWrite			sii_set_rx_cec_reg

#define SiiRxPlus5vGet				sii_get_rx_plus5v
#define SiiRxEdidGet				sii_get_rx_edid_content
#define SiiRxEdidSet				sii_set_rx_edid_content
#define SiiRxHpdGet					sii_get_rx_hpd
#define SiiRxHpdSet					sii_set_rx_hpd
#define SiiRxAvLinkGet				sii_get_rx_link_type
#define SiiRxAvMuteGet				sii_get_rx_av_mute
#define SiiRxInfoFrameHdrGet		sii_get_rx_info_frame_hdr
#define SiiRxInfoFrameVsifGet		sii_get_rx_info_frame_vsif
#define SiiRxInfoFrameAviGet		sii_get_rx_info_frame_avi
#define SiiRxInfoFrameAifGet		sii_get_rx_info_frame_aif
#define SiiRxEqSet					sii_set_rx_eq
#define SiiRxSCDCRegRead			sii_get_rx_scdc_reg
#define SiiRxSCDCRegWrite			sii_set_rx_scdc_reg
#define SiiRxCharacterErrorCountGet	sii_get_rx_char_error_count
#define SiiRxTmdsLockStatusGet		sii_get_rx_tmds_lock_status
#define SiiRxDeviceIdStringGet		sii_get_rx_device_id_string
#define SiiRxVideoInfoGet			sii_get_rx_video_info
#define SiiRxAudioInfoGet			sii_get_rx_audio_info
#define SiiRxAudioMclkRatioSet		sii_set_rx_audio_mclk_ratio
#define SiiRxAudioExtractionSet		sii_set_rx_audio_extraction
#define SiiRxScrambleFullGet		sii_get_rx_scramble_full

#define SiiTxTmdsEnableGet			sii_get_tx_tmds_status
#define SiiTxTmdsEnableSet			sii_set_tx_tmds_status
#define SiiTxAvMuteGet				sii_get_tx_av_mute
#define SiiTxAvMuteSet				sii_set_tx_av_mute
#define SiiTxTmdsSwingLevelGet		sii_get_tx_tmds_swing_level
#define SiiTxTmdsSwingLevelSet		sii_set_tx_tmds_swing_level
#define SiiTxTmdsPinSwapGet			sii_get_tx_tmds_pin_swap
#define SiiTxTmdsPinSwapSet			sii_set_tx_tmds_pin_swap
#define SiiTxHpdStatusGet			sii_get_tx_hpd
#define SiiTxRxSenseStatusGet		sii_get_tx_rxsense
#define SiiTxDsEdidStatusGet		sii_get_tx_ds_edid_status
#define SiiTxDsEdidGet				sii_get_tx_ds_edid_content
#define SiiTxScrambleSet			sii_set_tx_scramble
#define SiiTxScrambleFullSet		sii_set_tx_scramble_full
#define SiiTxCharacterErrorCountGet	sii_get_tx_char_error_count
#define SiiTxDeviceIdStringGet		sii_get_tx_device_id_string
#define SiiTxColorSpaceSet			sii_set_tx_color_space
#define SiiTxAudioInsertionSet		sii_set_tx_audio_insertion

#define SiiRxHdcpDdcGet				sii_get_rx_hdcp_ddc
#define SiiRxHdcpDdcSet				sii_set_rx_hdcp_ddc
#define SiiRxHdcpBksvGet			sii_get_rx_hdcp_bksv
#define SiiRxHdcpRxIdGet			sii_get_rx_hdcp_rxid
#define SiiRxHdcpStatusGet			sii_get_rx_hdcp_status
#define SiiRxHdcpUsVerGet			sii_get_rx_hdcp_us_version

#define SiiRxHdcpRepeaterModeGet		sii_get_rx_hdcp_repeater_mode
#define SiiRxHdcpRepeaterModeSet		sii_set_rx_hdcp_repeater_mode
#define SiiRxHdcpRxIdListTriggerToSend	sii_trigger_rx_hdcp_rxid_list_to_send
#define SiiRxHdcpRxIdListSet			sii_set_rx_hdcp_rxid_list
#define SiiRxHdcpTopologySet			sii_set_rx_hdcp_topology
#define SiiRxHdcpStreamManageMsgGet		sii_get_rx_hdcp_stream_manage_info

#define SiiTxHdcpProtectionSet		sii_set_tx_hdcp_protection
#define SiiTxHdcpContentTypeSet		sii_set_tx_hdcp_content_type
#define SiiTxHdcpDsVerGet			sii_get_tx_hdcp_ds_version
#define SiiTxHdcpStatusGet			sii_get_tx_hdcp_status

#define SiiTxHdcpRcvIdGet				sii_get_tx_hdcp_rcvid
#define SiiTxHdcpDsRxIdListGet			sii_get_tx_hdcp_ds_rxid_list
#define SiiTxHdcpDsRepeaterBitGet		sii_get_tx_hdcp_ds_repeater_bit
#define SiiTxHdcpDsTopologyGet			sii_get_tx_hdcp_ds_topology
#define SiiTxHdcpStreamManageMsgSet		sii_set_tx_hdcp_stream_manage_info

#define SiiPlatformInit				sii_platform_init
#define SiiPlatformTerm				sii_platform_term

void Init_9396(void);
void process_9396(BYTE cInstance);

void SiiSetRXHpdLow2High(BYTE cInstance);
bool SiiTxHpdGet(BYTE cInstance);
BOOL Sii9396RxConnectGet(BYTE cInstance);
void SiiTxEdidGetEdidData(BYTE cInstance,PBYTE cpEdid);
void SiiSetRxEdidSramWrite(BYTE cInstance, uint8_t  *pData, uint16_t length, eEDIDINPORTSELECT cINPORTSELECT);

BOOL Sii9396RxHDCPGet(BYTE cInstance);
BOOL Sii9396TxHDCPGet(BYTE cInstance);
void Sii9396TXHDCPSetStart(BYTE cInstance,BOOL bEnable);
BOOL  Sii9396RXResolutionGet(BYTE cInstance,INT* pHorizPixel, INT *pVertLine,INT *pTmdsClock,bool_t* pbInterlace);
void get_EDID(ePOWER_STATUS POWER_STATUS,unsigned char *p_data);
void Sii9396TxSpdifAudioOnOff(BYTE cInstance,enum sii_audio_mode mode);

#endif /* !_SII_API_H */
