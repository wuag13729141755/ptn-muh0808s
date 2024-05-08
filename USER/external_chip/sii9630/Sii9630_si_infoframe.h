/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS IS” WITHOUT ANY WARRANTY of any kind,
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

#ifndef __SI_INFOFRAME_H__
#define __SI_INFOFRAME_H__

#define SI_PACKING 0

#if SI_PACKING
#define SI_PACK_THIS_STRUCT __attribute__((__packed__))
#else
#define SI_PACK_THIS_STRUCT
#endif

/* VSIF - HDMI/MHL3 */
#define VSIF_TYPE			0x81
#define AVIF_TYPE			0x82
#define AUDIOIF_TYPE		0x84
#define ACRIF_TYPE			0x01
#define HDMI_VSIF_VERSION	0x01
#define MHL2_VSIF_VERSION	0x01
#define MHL3_VSIF_VERSION	0x03
#define MHL3_VSIF_LENGTH	0x0F
#define IEEE_OUI_HDMI	0x000C03
#define IEEE_OUI_MHL	0x7CA61D

#define IEEE_OUI(x)	((uint32_t)x[0] | \
			((uint32_t)x[1] << 8) | \
			((uint32_t)x[2] << 16))

#define PB4_MASK_MHL_VID_FMT		0x03
#define PB4_MASK_MHL_3D_FMT_TYPE	0x1C
#define PB4_MASK_SEP_AUD			0x20
#define PB4_MASK_RESERVED			0xC0

#define MHL2_VSIF_PB4(vid_fmt, _3d_fmt_type)	\
	((uint8_t)((vid_fmt) & PB4_MASK_MHL_VID_FMT) | \
	((_3d_fmt_type) & PB4_MASK_MHL_3D_FMT_TYPE))

#define MHL3_VSIF_PB4(vid_fmt, _3d_fmt_type, sep_aud) \
	(uint8_t)(((vid_fmt) & PB4_MASK_MHL_VID_FMT) | \
	((_3d_fmt_type) & PB4_MASK_MHL_3D_FMT_TYPE) | \
	((sep_aud) & PB4_MASK_SEP_AUD))

#define PB6_MASK_MHL_HEV_FMT	0x03
#define PB6_MASK_RESERVED		0xFC
#define MHL3_VSIF_PB6(hev_fmt) (uint8_t)((hev_fmt) & PB6_MASK_MHL_HEV_FMT)

#define PB9_MASK_AV_DELAY_SYNC_19_16 0x0F
#define PB9_MASK_AV_DELAY_DIR 0x10
#define PB9_MASK_RESERVED 0xE0
#define MHL3_VSIF_PB9(delay_sync, delay_dir) \
	(uint8_t)((((delay_sync) >> 16) & PB9_MASK_AV_DELAY_SYNC_19_16) | \
	((delay_dir) & PB9_MASK_AV_DELAY_DIR))

/* AVI */
/**
* @brief video color formats
*/
enum sii_drv_clr_spc {
	SII_DRV_CLRSPC__PASSTHRU,
	SII_DRV_CLRSPC__YC444_601,
	SII_DRV_CLRSPC__YC422_601,
	SII_DRV_CLRSPC__YC420_601,
	SII_DRV_CLRSPC__YC444_709,
	SII_DRV_CLRSPC__YC422_709,
	SII_DRV_CLRSPC__YC420_709,
	SII_DRV_CLRSPC__XVYCC444_601,
	SII_DRV_CLRSPC__XVYCC422_601,
	SII_DRV_CLRSPC__XVYCC420_601,
	SII_DRV_CLRSPC__XVYCC444_709,
	SII_DRV_CLRSPC__XVYCC422_709,
	SII_DRV_CLRSPC__XVYCC420_709,
	SII_DRV_CLRSPC__YC444_2020,
	SII_DRV_CLRSPC__YC422_2020,
	SII_DRV_CLRSPC__YC420_2020,
	SII_DRV_CLRSPC__RGB_DEFAULT,
	SII_DRV_CLRSPC__RGB_FULL,
	SII_DRV_CLRSPC__RGB_LIMITED
};

/**
* @brief video conversion standard
*/
enum sii_drv_conv_std {
	SII_DRV_CONV_STD__BT_709,
	SII_DRV_CONV_STD__BT_601,
	SII_DRV_CONV_STD__BT_2020_NON_CONSTANT_LUMINOUS,
	SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS
};

enum avi_color_space {
	ACS__RGB,
	ACS__YCBCR422,
	ACS__YCBCR444,
	ACS__YCBCR420
};

enum avi_rgb_quant_range {
	AQR__DEFAULT = 0,
	AQR__LIMITED_RANGE,
	AQR__FULL_RANGE,
	AQR__RESERVED
};

enum bits_content {
	CONTENT__GRAPHICS,
	CONTENT__PHOTO,
	CONTENT__CINEMA,
	CONTENT__GAME
};

enum avi_quantization {
	AVI_QUANT__DEFAULT,
	AVI_QUANT__LIMITEDRANGE,
	AVI_QUANT__FULLRANGE,
};

/* AUDIO */
/**
* @brief audio sampling frequency
*/
enum sii_drv_aud_sampling_frq {
	SII_DRV_AUDIO_SAMPLG_FRQ__32KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__44_1_KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__48KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__88_2_KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__96KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__176_4KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__192KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__256KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__352_8KHZ,
	SII_DRV_AUDIO_SAMPLG_FRQ__384KHZ,
};

/**
* @brief audio channel count
*/
enum sii_drv_aud_channel_count {
	SII_DRV_AUDIO_CHANNEL_COUNT__2CHNL,
	SII_DRV_AUDIO_CHANNEL_COUNT__3CHNL,
	SII_DRV_AUDIO_CHANNEL_COUNT__4CHNL,
	SII_DRV_AUDIO_CHANNEL_COUNT__5CHNL,
	SII_DRV_AUDIO_CHANNEL_COUNT__6CHNL,
	SII_DRV_AUDIO_CHANNEL_COUNT__7CHNL,
	SII_DRV_AUDIO_CHANNEL_COUNT__8CHNL,
};

/**
* @brief audio sample size
*/
enum sii_drv_aud_sample_size {
	SII_DRV_AUDIO_SAMPLE_SIZE__16BIT,
	SII_DRV_AUDIO_SAMPLE_SIZE__20BIT,
	SII_DRV_AUDIO_SAMPLE_SIZE__24BIT,
};

/* VSIF */
enum hdmi_video_format {
	HVF__NO_ADDITIONAL_HDMI_VIDEO_FORMAT_PRESENT,
	HVF__EXTENDED_RESOLUTION_FORMAT_PRESENT,
	HVF__3D_FORMAT_INDICATION_PRESENT
};

enum _3d_structure_format {
	_3D_STRUCTURE__FRMPACKING,
	_3D_STRUCTURE__FIELDALTERNATE,
	_3D_STRUCTURE__LINEALTERNATE,
	_3D_STRUCTURE__SIDEBYSIDEFULL,
	_3D_STRUCTURE__LDEPTH,
	_3D_STRUCTURE__LDEPTHGRAPHICSDEPTH,
	_3D_STRUCTURE__TOPANDBOTTOM,
	_3D_STRUCTURE__SIDEBYSIDEHALF = 0x08,
};

enum _3d_extended_data {
	_3D_EXT_DATA__HORIZONTAL_SUB_SAMPLING,
	_3D_EXT_DATA__QUINCUNX_ODD_LEFT_ODD_RIGHT = 0x04,
	_3D_EXT_DATA__QUINCUNX_ODD_LEFT_EVEN_RIGHT,
	_3D_EXT_DATA__QUINCUNX_EVEN_LEFT_ODD_RIGHT,
	_3D_EXT_DATA__QUINCUNX_EVEN_LEFT_EVEN_RIGHT,
};

enum mhl_video_format {
	MHL_VID_FMT_NO_ADDITIONAL,
	MHL_VID_FMT_3D_FMT_PRESENT,
	MHL_VID_FMT_MULTI_VIEW,
	MHL_VID_FMT_DUAL_3D
};

enum mhl_3d_format_type {
	MHL_3D_FMT_TYPE_FS,             /* Frame Sequential */
	MHL_3D_FMT_TYPE_TB = 0x04,      /* Top-Bottom */
	MHL_3D_FMT_TYPE_LR = 0x08,      /* Left-Right */
	MHL_3D_FMT_TYPE_FS_TB = 0x0C,   /* Frame Sequential Top-Bottom */
	MHL_3D_FMT_TYPE_FS_LR = 0x10,   /* Frame Sequential Left-Right */
	MHL_3D_FMT_TYPE_TBLR = 0x14     /* Top-Bottom-Left-Right */
};

enum mhl_seperate_audio {
	MHL_SEP_AUDIO_NOT_AVAILABLE,
	MHL_SEP_AUDIO_AVAILABLE = 0x20
};

enum mhl_hev_format {
	MHL_HEV_FMT_NO_ADDITIONAL,
	MHL_HEV_FMT_HEV_PRESENT,
	MHL_HEV_FMT_RESERVED_2,
	MHL_HEV_FMT_RESERVED_3
};

enum mhl_av_delay_dir {
	MHL_AV_DELAY_DIR_AUDIO_EARLIER,
	MHL_AV_DELAY_DIR_VIDEO_EARLIER = 0x10
};

/* Common Header */
struct info_frame_header {
	uint8_t type_code;
	uint8_t version_number;
	uint8_t length;
};

/* AVI Info Frame Structure */
struct avi_info_frame_data_byte_1 {
	uint8_t scan_info;
	uint8_t bar_info;
	uint8_t active_format_info_present;
	enum avi_color_space colorSpace;
};

struct avi_info_frame_data_byte_2 {
	uint8_t active_format_aspect_ratio;
	uint8_t picture_aspect_ratio;
	uint8_t colorimetry;
};

struct avi_info_frame_data_byte_3 {
	uint8_t non_uniform_picture_scaling;
	uint8_t rgb_quantization_range;
	uint8_t extended_colorimetry;
	uint8_t it_content;
};

struct avi_info_frame_data_byte_4 {
	uint8_t vic;
};

struct avi_info_frame_data_byte_5 {
	uint8_t pixel_repetition_factor;
	enum bits_content content;
	enum avi_quantization quantization;
};

struct avi_payload {
	uint8_t checksum;
	struct avi_info_frame_data_byte_1 pb1;
	struct avi_info_frame_data_byte_2 pb2;
	struct avi_info_frame_data_byte_3 pb3;
	struct avi_info_frame_data_byte_4 pb4;
	struct avi_info_frame_data_byte_5 pb5;
	uint8_t linenumend_topbarlow;
	uint8_t linenumend_topbarhigh;
	uint8_t linenumstart_bottombarlow;
	uint8_t linenumstart_bottombarhigh;
	uint8_t linenumend_leftbarlow;
	uint8_t linenumend_leftbarhigh;
	uint8_t linenumstart_rightbarlow;
	uint8_t linenumstart_rightbarhigh;
	/* Additional Data */
	enum sii_drv_clr_spc clr_spc;
	enum sii_drv_conv_std conv_std;
};

struct avi_info_frame {
	struct info_frame_header header;
	struct avi_payload payload;
};

#define VSIF_COMMON_FIELDS \
struct info_frame_header header; \
	uint8_t checksum; \
	uint8_t ieee_oui[3];

/*HDMI Payload*/
struct hdmi_vendor_specific_payload {
	enum hdmi_video_format hdmi_vid_fmt;
	uint8_t hdmi_vic;
	uint8_t _3d_meta_present;
	enum _3d_structure_format _3d_structure;
	enum _3d_extended_data _3d_ext_data;
	uint8_t _3d_meta_data_length;
	uint8_t _3d_meta_data_type;
};

struct hdmi_vsif {
	VSIF_COMMON_FIELDS
	struct hdmi_vendor_specific_payload payload;
};

/* MHL3 Payload */
struct mhl3_vendor_specific_payload {
	enum mhl_video_format mhl_vid_fmt;
	enum mhl_3d_format_type mhl_3d_fmt_type;
	enum mhl_seperate_audio sep_audio;

	enum mhl_hev_format mhl_hev_fmt;
	uint8_t mhl_hev_fmt_type_h;
	uint8_t mhl_hev_fmt_type_l;

	uint8_t acrfs_div;
	enum mhl_av_delay_dir av_delay_dir;
	uint8_t av_delay_sync_16_19;
	uint8_t av_delay_sync_h;
	uint8_t av_delay_sync_l;
};

struct mhl3_vsif {
	VSIF_COMMON_FIELDS
	struct mhl3_vendor_specific_payload payload;
};

struct vsif_common_header {
	VSIF_COMMON_FIELDS
};

/* Audio */
struct aif_payload {
	enum sii_drv_aud_sampling_frq sampling_frq;
	enum sii_drv_aud_channel_count channel_count;
	enum sii_drv_aud_sample_size sample_size;
};

struct audio_info_frame {
	struct info_frame_header header;
	struct aif_payload payload;
};

/* ACR */
struct acr_payload {
	uint32_t audio_cts;
	uint32_t audio_n;
};

struct acr_info_frame {
	struct info_frame_header header;
	struct acr_payload aud_params;
};

#endif /* __SI_INFOFRAME_H__ */
