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

#ifndef __SI_LIB_EDID_API_H__
#define __SI_LIB_EDID_API_H__


#define PARSE_EDID_PRINT_INLINE       0
#define PARSE_EDID_PRINT              0

#define EDID_BLOCK_0		0
#define EDID_BLOCK_2_3		1

#define VIDEO_DATA_BLOCK              1
#define AUDIO_DATA_BLOCK              1
#define SPK_ALLOC_BLOCK               1
#define EXTENDED_BLOCKS               1
#define HF_VSDB                       1
#define VENDOR_SPECIFIC_DATA_BLOCK    1

#define SII_LIB_EDID__HDMI_VIC_MAX      4
#define SII_LIB_EDID__SAD_MAX           6
#define SII_LIB_EDID__SVD_MAX          31
#define SII_LIB_EDID__DTD_MAX          15

#define SII_LIB_EDID__VENDOR_SPECIFIC_PAYLOAD_MAX   12
#define SII_LIB_EDID__INDIV_3D_MODES_PER_VIC_MAX     3
#define SII_LIB_EDID__420_SUPP_SVD_MAX               6

#define SII_LIB_EDID_PAR__VERSION              1
#define SII_LIB_EDID_PAR__MONITOR_NAME         1
#define SII_LIB_EDID_PAR__DISPLAY_AR           1
#define SII_LIB_EDID_PAR__CEC_ADDR             1
#define SII_LIB_EDID_PAR__VDB                  1
#define SII_LIB_EDID_PAR__SADB                 1
#define SII_LIB_EDID_PAR__ADB                  1
#define SII_LIB_EDID_PAR__DTDB                 1
#define SII_LIB_EDID_PAR__AUD_FRM              1
#define SII_LIB_EDID_PAR__MAX_TMDS             1
#define SII_LIB_EDID_PAR__3D                   1
#define SII_LIB_EDID_PAR__DEEP_CLR             1
#define SII_LIB_EDID_PAR__CLR_SPACE            1
#define SII_LIB_EDID_PAR__IEEE                 1
#define SII_LIB_EDID_PAR__HFVSDB               1
#define SII_LIB_EDID_PAR__LIPSYNC              1

#define SII_EDID_BLOCK_SIZE 128

/* EDID structure definitions */

#define SII_LIB_EDID_BL0_ADR__HEADER					0x00
#define SII_LIB_EDID_BL0_LEN__HEADER					8

#define SII_LIB_EDID_BL0_ADR__MANUFACTURER_ID				0x08
#define SII_LIB_EDID_BL0_LEN__MANUFACTURER_ID				2
#define SII_LIB_EDID_BL0_ADR__PRODUCT_ID				0x0A
#define SII_LIB_EDID_BL0_LEN__PRODUCT_ID				2
#define SII_LIB_EDID_BL0_ADR__SHORT_SERIAL_NUMBER			0x0C
#define SII_LIB_EDID_BL0_LEN__SHORT_SERIAL_NUMBER			4
#define SII_LIB_EDID_BL0_ADR__MANUFACTURE_WEEK				0x10
#define SII_LIB_EDID_BL0_ADR__MANUFACTURE_YEAR				0x11
#define SII_LIB_EDID_BL0_LEN__PROD_DETAILS				10

#define SII_LIB_EDID_BL0_ADR__VERSION					0x12
#define SII_LIB_EDID_BL0_ADR__REVISION					0x13

#define SII_LIB_EDID_BL0_ADR__BASIC_DISPLAY_PARAMS			0x14
#define SII_LIB_EDID_BL0_LEN__BASIC_DISPLAY_PARAMS			5

#define SII_LIB_EDID_BL0_ADR__CHROMATICITY				0x19
#define SII_LIB_EDID_BL0_LEN__CHROMATICITY				10

#define SII_LIB_EDID_BL0_ADR__ESTABLISHED_TIMING_1			0x23
#define SII_LIB_EDID_BL0_ADR__ESTABLISHED_TIMING_2			0x24
#define SII_LIB_EDID_BL0_ADR__MANUFACTURER_RESERVED_TIMING		0x25

#define SII_LIB_EDID_BL0_ADR__STANDARD_TIMING				0x26
#define SII_LIB_EDID_BL0_LEN__STANDARD_TIMING				16

#define SII_LIB_EDID_BL0_ADR__DTD1					0x36
#define SII_LIB_EDID_BL0_ADR__DTD2					0x48
#define SII_LIB_EDID_BL0_ADR__DTD3					0x5A
#define SII_LIB_EDID_BL0_ADR__DTD4					0x6C
#define SII_LIB_EDID_LEN__DTD						18

#define SII_LIB_EDID_BL0_ADR__EXTENSION_NMB				0x7E

#define SII_LIB_EDID_ADR__CHECK_SUM					0x7F

/* Extension Tags */
#define SII_LIB_EDID_EXT_TAG__LCD_TIMINGS				0x01
#define SII_LIB_EDID_EXT_TAG__ADDITIONAL_TIMINGS			0x02
#define SII_LIB_EDID_EXT_TAG__EDID_2_0					0x20
#define SII_LIB_EDID_EXT_TAG__COLOR_INFO_0				0x30
#define SII_LIB_EDID_EXT_TAG__DVI_FEATURE				0x40
#define SII_LIB_EDID_EXT_TAG__TOUCH_SCREEN				0x50
#define SII_LIB_EDID_EXT_TAG__BLOCK_MAP					0xF0
#define SII_LIB_EDID_EXT_TAG__BY_MANUFACTURER				0xFF

/* types of alternative DTD structures */
/* monitor serial number */
#define SII_LIB_EDID_DTD_TAG__SN					0xFF
/* ASCII string */
#define SII_LIB_EDID_DTD_TAG__STRING					0xFE
/* monitor range limits */
#define SII_LIB_EDID_DTD_TAG__RANGE_LIMITS				0xFD
/* monitor name */
#define SII_LIB_EDID_DTD_TAG__NAME					0xFC
/* color point */
#define SII_LIB_EDID_DTD_TAG__COLOR_POINT				0xFB
/* standard timing identifiers */
#define SII_LIB_EDID_DTD_TAG__STI					0xFA
/* offset in DTD structure where the tags are placed */
#define SII_LIB_EDID_DTD_ADR__DATA_TYPE_TAG				0x03
/* offset in DTD structure where data starts */
#define SII_LIB_EDID_DTD_ADR__DATA					0x05

/* DTD structure when used for Timings */
#define SII_LIB_EDID_DTD_ADR__PIX_CLK_LOW				0x00
#define SII_LIB_EDID_DTD_ADR__PIX_CLK_HIGH				0x01
#define SII_LIB_EDID_DTD_ADR__H_ACTIVE_LOW				0x02
#define SII_LIB_EDID_DTD_ADR__H_BLANK_LOW				0x03
#define SII_LIB_EDID_DTD_ADR__H_HIGH					0x04
#define SII_LIB_EDID_DTD_ADR__V_ACTIVE_LOW				0x05
#define SII_LIB_EDID_DTD_ADR__V_BLANK_LOW				0x06
#define SII_LIB_EDID_DTD_ADR__V_HIGH					0x07
#define SII_LIB_EDID_DTD_ADR__H_SYNC_OFFSET_LOW				0x08
#define SII_LIB_EDID_DTD_ADR__H_SYNC_WIDTH_LOW				0x09
#define SII_LIB_EDID_DTD_ADR__V_SYNC_LOW				0x0A
#define SII_LIB_EDID_DTD_ADR__H_V_SYNC_HIGH				0x0B
#define SII_LIB_EDID_DTD_ADR__H_SIZE_LOW				0x0C
#define SII_LIB_EDID_DTD_ADR__V_SIZE_LOW				0x0D
#define SII_LIB_EDID_DTD_ADR__H_V_SIZE_HIGH				0x0E
#define SII_LIB_EDID_DTD_ADR__H_BORDER					0x0F
#define SII_LIB_EDID_DTD_ADR__V_BORDER					0x10
#define SII_LIB_EDID_DTD_ADR__FLAGS					0x11

/* Block 0 has alvays 4 DTDs, but extensions can have more.
 * The end of DTD area
 * is marked by 5 zeros.
 */
#define SII_LIB_EDID_EXT_ADR__LAST_DTD_START (EDID_ADR__CHECK_SUM-5)
/* This is a start address in extended block where last DTD at the block
 * could be placed.
 * Even a DTD size is 18 uint8_ts, 5 uint8_ts is the minimum length of a
 * DTD structure.
 * This shorten 5-uint8_ts stucture is used to mark the end of the DTD area.
 * Note, if this type of DTD structure exsist, all the uint8_ts must be zeros.
 */

/* Monitor Range Limits (MRL) structure - one of the alternative DTDs */
#define SII_LIB_EDID_MRL_ADR__V_MIN					0x05
#define SII_LIB_EDID_MRL_ADR__V_MAX					0x06
#define SII_LIB_EDID_MRL_ADR__H_MIN					0x07
#define SII_LIB_EDID_MRL_ADR__H_MAX					0x08
#define SII_LIB_EDID_MRL_ADR__PCLK_MAX					0x09

/* extension block structure */
/* should be set to EDID_EXT_TAG__ADDITIONAL_TIMINGS */
#define SII_LIB_EDID_EXT_ADR__TAG					0x00
/* revision (at least 2) */
#define SII_LIB_EDID_EXT_ADR__REVISION					0x01
/* where DTD area starts */
#define SII_LIB_EDID_EXT_ADR__D						0x02
/* total number of native DTDs & options */
#define SII_LIB_EDID_EXT_ADR__DTD_OPT					0x03
/* Data Block Collection start address */
#define SII_LIB_EDID_EXT_ADR__DBC					0x04

/* extended tags for CEA data block collection */
#define SII_LIB_EDID_DBC_TAG__AUDIO				1
#define SII_LIB_EDID_DBC_TAG__VIDEO				2
#define SII_LIB_EDID_DBC_TAG__VENDOR_SPECIFIC			3
#define SII_LIB_EDID_DBC_TAG__SPEAKER_ALLOCATION		4
#define SII_LIB_EDID_DBC_TAG__VESA_DTC				5
#define SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG			7

/* etended tags for CEA data block collection */
#define SII_LIB_EDID_DBC_ETG__MISC_VIDEO_FIELDS			0
#define SII_LIB_EDID_DBC_ETG__VENDOR_SPECIFIC_VIDEO		1
#define SII_LIB_EDID_DBC_ETG__CALORIMETRY			5
#define SII_LIB_EDID_DBC_ETG__HDR_STATIC_MD			6
#define SII_LIB_EDID_DBC_ETG__HDR_DYNAMIC_MD			7
#define SII_LIB_EDID_DBC_ETG__YCC420_VIDEO			14
#define SII_LIB_EDID_DBC_ETG__YCC420_CAPABILITY_MAP		15
#define SII_LIB_EDID_DBC_ETG__MISC_AUDIO_FIELDS			16
#define SII_LIB_EDID_DBC_ETG__VENDOR_SPECIFIC_AUDIO		17

#define SII_LIB_EDID_SAD__LENGTH				3
#define SII_LIB_EDID_SPK__LENGTH				3
#define SII_LIB_EDID_SVD__LENGTH				1

#define SII_LIB_EDID_HDR_SMD__LENGTH				6
#define SII_LIB_EDID_HDR_DMD__LENGTH				31

#define SII_LIB_EDID_SPK__LENGTH				3

#define SII_LIB_EDID_VSB_DAT__HDMI_SIGNATURE			0x000C03
#define SII_LIB_EDID_VSB_DAT__HDMI_FORUM_SIGNATURE		0xC45DD8

#define SII_LIB_EDID_DBC_ADDR__MAX_OCCUPANCY (SII_LIB_EDID_ADR__CHECK_SUM + \
						SII_EDID_BLOCK_SIZE - \
						SII_LIB_EDID_LEN__DTD)

#define SII_LIB_DTD_DESCRIPTOR_DATA_LENGTH	(18-5)
#define SII_LIB_DTD_DESCRIPTOR_DATA_START	5

/* error codes for defective EDID raw data */
enum sii_lib_edid_err_code_t {
	SII_LIB_EDID_ERR_CODE__NO_ERROR,
	SII_LIB_EDID_ERR_CODE__BAD_HEADER,
	SII_LIB_EDID_ERR_CODE__CHECKSUM,
	SII_LIB_EDID_ERR_CODE__NOEXTENSIONS,
	SII_LIB_EDID_ERR_CODE__CEA_TAG,
	SII_LIB_EDID_ERR_CODE__NO_861B,
	SII_LIB_EDID_ERR_CODE__UNKNOWN_TAG,
	SII_LIB_EDID_ERR_CODE__VENDERBLOCK
};

struct cea_short_descriptor_t {
	uint8_t vic;
};

struct sii_lib_edid_indiv_3d_mode_t {

	uint8_t   b_indiv_3d_present;
	uint8_t   struct_3d;
	uint8_t   detail_3d;
};

struct sii_lib_edid_short_video_desc_t {

	uint8_t		b_valid;
	uint8_t		b_native;
	uint8_t        b_masked_3d;
	uint8_t        b_indiv_3d_present;
	uint8_t        b_3d_present;
	uint8_t        b_420_mapped;
	uint8_t		vic;

	struct sii_lib_edid_indiv_3d_mode_t
		indiv_3d[SII_LIB_EDID__INDIV_3D_MODES_PER_VIC_MAX];
};

struct sii_lib_edid_video_db_t {

	uint8_t				          size;
	struct sii_lib_edid_short_video_desc_t	  svd[SII_LIB_EDID__SVD_MAX];
};

struct sii_lib_edid_spk_alloc_db_t {
	bool b_present;
	uint8_t spk_alloc[SII_LIB_EDID_SPK__LENGTH];
};

struct sii_lib_edid_hdr_stat_md_db_t {
	uint8_t length;
	uint8_t hdr_stat_md[SII_LIB_EDID_HDR_SMD__LENGTH];
};

struct sii_lib_edid_hdr_dyn_md_db_t {
	uint8_t length;
	uint8_t hdr_dyn_md[SII_LIB_EDID_HDR_DMD__LENGTH];
};

struct sii_lib_edid_sad_t {

	uint8_t audio_data[SII_LIB_EDID_SAD__LENGTH];
};

struct sii_lib_edid_audio_db_t {

	uint8_t				size;
	struct sii_lib_edid_sad_t	sad[SII_LIB_EDID__SAD_MAX];
};

struct sii_lib_edid_video_timing_t {

	bool b_interlaced;
	bool b_valid;

	uint8_t v_border;
	uint8_t h_border;

	uint8_t display_type;
	uint8_t interface_type;

	uint16_t v_active;
	uint16_t v_blank;
	uint16_t v_sync_pw;
	uint16_t v_sync_offset;
	uint16_t v_image_size;

	uint16_t h_active;
	uint16_t h_blank;
	uint16_t h_sync_pw;
	uint16_t h_sync_offset;
	uint16_t h_image_size;

	uint32_t pixel_freq_10khz;
};

struct sii_lib_edid_dtd_t {

	uint8_t	size;
	uint8_t num_vesa_timing_dtds;
	uint8_t num_cea_861_timing_dtds;
	struct sii_lib_edid_video_timing_t vid_timing[SII_LIB_EDID__DTD_MAX];

};

struct sii_lib_edid_lipsync_info_t {

	bool           b_latency_present;
	bool           b_i_latency_present;
	uint8_t        video_latency;
	uint8_t        audio_latency;
	uint8_t        i_video_latency;
	uint8_t        i_audio_latency;

};

struct sii_lib_edid_vsdb_t {

	bool		b_supports_ai;
	bool		b_y444_dc10;
	bool		b_y444_dc12;
	bool		b_y444_dc16;
	bool		b_y444;
	bool		b_3d_present;

	uint8_t		cec_addr_ptr;
	uint8_t		cec_addr[4];
	uint8_t		multi_3d_type;
	uint8_t		content_type;
	uint8_t		image_size;
	uint8_t		hdmi_vic_len;
	uint8_t		hdmi_3d_len;
	uint8_t		hdmi_vic[SII_LIB_EDID__HDMI_VIC_MAX];

	uint16_t	struct_3d_all;
	uint16_t	mask_3d;

	struct sii_lib_edid_lipsync_info_t   lipsync;

};

struct sii_lib_edid_hfvsdb_t {

	bool       b_3d_osd_disparity;
	bool       b_dual_view;
	bool       b_independent_view;
	bool       b_lte_340mscs_scramble;
	bool       b_read_req_capable;
	bool       b_scdc_present;
	bool       b_dc30bit420;
	bool       b_dc36bit420;
	bool       b_dc48bit420;
	uint32_t   vclk_mb;

};

struct sii_lib_edid_vid_cap_db_t {
	bool	b_present;
	uint8_t scan_info;
};

struct sii_lib_edid_vs_ext_db_t {
	bool	b_present;
	uint32_t    ieee_oui;
	uint8_t     length;
	uint8_t     ptr_array[SII_LIB_EDID__VENDOR_SPECIFIC_PAYLOAD_MAX];
};

struct sii_lib_edid_420_supp_t {
	uint8_t     length;
	uint8_t     svd_data[SII_LIB_EDID__420_SUPP_SVD_MAX];
};

struct sii_lib_edid_calorimetry_db_t {
	bool	calorimetry_present;
	uint8_t    calorimetry_byte;
};

/**
 * @brief e_d_i_d's lip_sync info
 */
struct sii_edid_lip_sync {
	bool latency_present;
	bool i_latency_present;
	uint8_t video_latency;
	uint8_t audio_latency;
	uint8_t i_video_latency;
	uint8_t i_audio_latency;
};

#ifdef CONFIG_DEFINED_MACRO
struct CEA_extension_version_1_t {
	uint8_t reserved_0;
	uint8_t reserved[123];
};

struct CEA_extension_2_3_misc_support_t {
	uint8_t total_number_native_dtds_in_entire_EDID:4;
	uint8_t YCrCb422_support:1;
	uint8_t YCrCb444_support:1;
	uint8_t basic_audio_support:1;
	uint8_t underscan_IT_formats_by_default:1;
};

struct CEA_extension_version_2_t {
	struct CEA_extension_2_3_misc_support_t misc_support;
	uint8_t reserved[123];
};

struct CEA_extension_version_3_t {
	struct CEA_extension_2_3_misc_support_t misc_support;
	union {
		uint8_t data_block_collection[123];
		uint8_t reserved[123];
	} Offset4_u;
};

struct CEA_extension_t {
	uint8_t tag;
	uint8_t revision;
	uint8_t byte_offset_to_18_byte_descriptors;
	union {
		struct CEA_extension_version_1_t version1;
		struct CEA_extension_version_2_t version2;
		struct CEA_extension_version_3_t version3;
	} version_u;
	uint8_t checksum;
};
#endif

/******************************************************************************
 * structure for containing the parsed EDID data
 *****************************************************************************/
struct sii_lib_edid_parsed_data {

	bool b_basic_audio;
	bool b_it_underscan;
	bool b_ycc444;
	bool b_ycc422;
	uint8_t native_dtd_cnt;
	char monitor_name_str[14];

	uint8_t product_details[SII_LIB_EDID_BL0_LEN__PROD_DETAILS];
	uint8_t basic_parameters[SII_LIB_EDID_BL0_LEN__BASIC_DISPLAY_PARAMS];
	uint8_t chromaticity[SII_LIB_EDID_BL0_LEN__CHROMATICITY];
	uint8_t est_timings[3];
	uint8_t std_timings[SII_LIB_EDID_BL0_ADR__STANDARD_TIMING];
	uint8_t monitor_ranges[SII_LIB_EDID_LEN__DTD];


	uint8_t max_tmds_5mhz;
	uint16_t edid_version;

	uint8_t num_ext_blocks;

	bool is_hfvsdb_present;
	struct sii_lib_edid_hfvsdb_t hfvsdb;
	struct sii_lib_edid_video_db_t vdb;
	struct sii_lib_edid_dtd_t dtdb;
	bool is_vsdb_present;
	struct sii_lib_edid_vsdb_t vsdb;
	struct sii_lib_edid_spk_alloc_db_t spkdb;
	struct sii_lib_edid_audio_db_t adb;

#if EXTENDED_BLOCKS
	struct sii_lib_edid_vid_cap_db_t vid_cap_db;
	struct sii_lib_edid_vs_ext_db_t vs_vid_db;
	struct sii_lib_edid_vs_ext_db_t vs_aud_db;
	struct sii_lib_edid_420_supp_t ycc_420_vid_db;
	struct sii_lib_edid_calorimetry_db_t calorimetry_db;
	struct sii_lib_edid_hdr_stat_md_db_t hdr_stat_md_db;
	struct sii_lib_edid_hdr_dyn_md_db_t hdr_dyn_md_db;
	uint8_t _420_cap_map_data_blk_len;
#endif

	uint32_t ieee_id;
	uint32_t max_pixel_clock_10mhz;
	struct sii_edid_lip_sync lip_sync;

	bool hdmi_sink;
};

/* public functions */

/******************************************************************************
 * configuration parameters to be filled before composing EDID
 * based on these parameters, capabilities are filled in EDID
 *****************************************************************************/
struct sii_lib_edid_config_device_t {

	uint16_t     min_vert_hz;
	uint16_t     max_vert_hz;
	uint16_t     min_horiz_khz;
	uint16_t     max_horiz_khz;
	uint32_t     max_pix_clk;
	uint32_t     max_tmds_clk;

};

/******************************************************************************
 * @brief raw EDID parsing
 *
 * takes raw EDID data and parses the data.
 * also can print the decoded data from raw EDID
 *
 * @param[in]  raw_edid             raw EDID data bytes
 * @param[out] ptr_parsed_edid	    structure containing the parsed EDID data
 *
 * @retval sii_lib_edid_err_code_t   error code as defined in the enum
 *
 *****************************************************************************/
enum sii_lib_edid_err_code_t sii_lib_edid_parse(
	struct sii_lib_edid_parsed_data *po_parsed_edid,
	uint8_t *pi_raw_edid);

/******************************************************************************
 * @brief compose raw EDID data from two parsed EDID structures
 *
 * takes two parsed EDID structures and creates a config struct based
 * on the user choice of dependent EDID. Based on the capabilities of
 * the dependent EDID, the other parsed EDID data are filtered and composed
 * to form the EDID raw data
 *
 * @param[in]  pi_parsed_dev_edid ptr to input struct of parsed device EDID
 * @param[in]  pi_parsed_ds_edid  ptr to input struct of parsed downstream EDID
 * @param[out] po_composed_edid	  ptr to output EDID raw data
 *
 * @retval
 *
 *****************************************************************************/
uint8_t sii_lib_edid_compose(
	struct sii_lib_edid_parsed_data *pi_parsed_dev_edid,
	struct sii_lib_edid_parsed_data *pi_parsed_ds_edid,
	uint8_t *po_composed_edid);

#endif /* SI_LIB_EDID_API_H*/
