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

#include "Sii9630_si_datatypes.h"
#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_platform.h"
#include "Sii9630_si_lib_edid.h"
#include "Sii9630_si_lib_log.h"

struct edid_data_container_t {
	uint8_t					extensions;
	uint8_t					*pi_edid_raw;
	struct sii_lib_edid_parsed_data	*po_edid_parsed;
	enum sii_lib_edid_err_code_t		error;
};

static void s_parse_hdr_stat_md_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len);

static void s_parse_hdr_dyn_md_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len);

#if PARSE_EDID_PRINT
void s_parsed_edid_print(struct sii_lib_edid_parsed_data *po_edid_parsed)
{
	uint8_t i = 0;
	sii_log_print("\n---- Block 0 ----\n");
	sii_log_debug("EDID Version : %02X.%02X\n",
		po_edid_parsed->edid_version >> 8,
		po_edid_parsed->edid_version & 0x0F);

	sii_log_debug("Product Info : ");
	for (i = 0; i < SII_LIB_EDID_BL0_LEN__PROD_DETAILS; i++) {
		sii_log_print("%02X ", po_edid_parsed->product_details[i]);
		;
	}
	sii_log_print("\n");

	sii_log_debug("Monitor Name : %s\n",
		po_edid_parsed->monitor_name_str);

	sii_log_debug("Chromaticity : ");
	for (i = 0; i < SII_LIB_EDID_BL0_LEN__CHROMATICITY; i++)
		sii_log_print("%02X ", po_edid_parsed->chromaticity[i]);
	sii_log_print("\n");

	sii_log_debug("Est Timings  : %02X %02X %02X\n",
		po_edid_parsed->est_timings[0], po_edid_parsed->est_timings[1],
		po_edid_parsed->est_timings[2]);

	sii_log_debug("Std Timings  : ");
	for (i = 0; i < SII_LIB_EDID_BL0_LEN__STANDARD_TIMING; i++)
		sii_log_print("%02X ", po_edid_parsed->std_timings[i]);
	sii_log_print("\n");

	sii_log_debug("DTD List\n");
	for (i = 0; i < 2; i++) {
		sii_log_print("----- DTD - %d -----\n", i);
		sii_log_debug("Pixel Freq    : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].pixel_freq_10khz);
		sii_log_debug("H Active      : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_active);
		sii_log_debug("H Blank       : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_blank);
		sii_log_debug("V Active      : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_active);
		sii_log_debug("V Blank       : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_blank);
		sii_log_debug("H Front Porch : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_sync_offset);
		sii_log_debug("H Sync        : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_sync_pw);
		sii_log_debug("V Front Porch : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_sync_offset);
		sii_log_debug("V Sync        : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_sync_pw);
		sii_log_debug("H Image Size  : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_image_size);
		sii_log_debug("V Image Size  : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_image_size);
		sii_log_debug("H Border Size : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_border);
		sii_log_debug("H Border Size : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_border);
		sii_log_debug("Display Type  : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].display_type);
		sii_log_debug("Interface Type: %d\n",
			po_edid_parsed->dtdb.vid_timing[i].interface_type);
		sii_log_debug("Interlaced    : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].b_interlaced);
	}

	/*------------------------------------------------------------
	 Block 1
	 ------------------------------------------------------------*/
	sii_log_print("\n---- Block 1 ----\n");
	sii_log_debug("Basic Audio Support : %d\n",
		po_edid_parsed->b_basic_audio);
	sii_log_debug("IT Underscan        : %d\n",
		po_edid_parsed->b_it_underscan);
	sii_log_debug("YCC444 support      : %d\n", po_edid_parsed->b_ycc444);
	sii_log_debug("YCC422 support      : %d\n", po_edid_parsed->b_ycc422);
	sii_log_debug("Native DTD No: %d\n", po_edid_parsed->native_dtd_cnt);

	sii_log_print("\n---- Video Data Block ----\n");
	for (i = 0; i < po_edid_parsed->vdb.size; i++) {
		sii_log_debug("VIC[%d] : %d,	Native : %d\n", i,
			po_edid_parsed->vdb.svd[i].vic,
			po_edid_parsed->vdb.svd[i].b_native);
	}
	sii_log_debug("Number of SVDs : %d\n", po_edid_parsed->vdb.size);

	sii_log_print("\n---- Audio Data Block ----\n");
	for (i = 0; i < po_edid_parsed->adb.size; i++) {
		sii_log_debug("SAD[%d] : %02X %02X %02X\n", i,
			po_edid_parsed->adb.sad[i].audio_data[0],
			po_edid_parsed->adb.sad[i].audio_data[1],
			po_edid_parsed->adb.sad[i].audio_data[2]);
	}
	sii_log_debug("Number of SADs : %d\n", po_edid_parsed->adb.size);

	sii_log_print("\n---- Speaker Allocation Data Block ----\n");
	sii_log_debug("%02X, %02X, %02X\n", po_edid_parsed->spkdb.spk_alloc[0],
		po_edid_parsed->spkdb.spk_alloc[1],
		po_edid_parsed->spkdb.spk_alloc[2]);

	sii_log_print("\n---- Vendor Specific Data Block ----\n");

	sii_log_debug("CEC Address	: %02d.%02d.%02d.%02d\n",
		po_edid_parsed->vsdb.cec_addr[0],
		po_edid_parsed->vsdb.cec_addr[1],
		po_edid_parsed->vsdb.cec_addr[2],
		po_edid_parsed->vsdb.cec_addr[3]);

	sii_log_debug("Max TMDS Clock	: %d MHz\n",
		po_edid_parsed->max_tmds_5mhz * 5);

	sii_log_debug("Content Type	: 0x%02X\n",
		po_edid_parsed->vsdb.content_type);

	sii_log_debug("Video Latency	: %d\n",
		po_edid_parsed->vsdb.lipsync.video_latency);
	sii_log_debug("Audio Latency	: %d\n",
		po_edid_parsed->vsdb.lipsync.audio_latency);
	sii_log_debug("I_Video Latency	: %d\n",
		po_edid_parsed->vsdb.lipsync.i_video_latency);
	sii_log_debug("I_Audio Latency	: %d\n",
		po_edid_parsed->vsdb.lipsync.i_audio_latency);

	{
		struct sii_lib_edid_vsdb_t *p_vsdb = &po_edid_parsed->vsdb;
		struct sii_lib_edid_video_db_t *p_vdb = &po_edid_parsed->vdb;

		sii_log_debug("3D Support	: %d\n", p_vsdb->b_3d_present);
		sii_log_debug("3D Multi	: %d\n", p_vsdb->multi_3d_type);
		sii_log_debug("Image Size	: %d\n", p_vsdb->image_size);
		sii_log_debug("HDMI VIC length	: %d\n", p_vsdb->hdmi_vic_len);
		sii_log_debug("HDMI 3D length	: %d\n", p_vsdb->hdmi_3d_len);
		if (p_vsdb->hdmi_vic_len)
			sii_log_debug("\nHDMI VIC	:\n");
		for (i = 0; i < p_vsdb->hdmi_vic_len; i++)
			sii_log_debug("VIC[%d] : %2d\n", i,
				p_vsdb->hdmi_vic[i]);

		sii_log_print("\n");
		sii_log_debug("3D Struct ALL	: 0x%04X\n",
			p_vsdb->struct_3d_all);

		for (i = 0; i < 16; i++) {
			sii_log_debug("3D MASKED VICS	: 0x%04X\n",
				p_vdb->svd[i].b_3d_present);
		}

		/*
		 sii_log_debug("\nIndividual 3D modes :\n");

		 sii_log_debug("2D VIC order : %d	3D Struct : 0x%02X",
		 vic_2d_order,
		 p_vdb->svd[vic_2d_order].indiv_3d[j].struct_3d);

		 sii_log_debug("3D Detail : 0x%02X\n",
		 p_vdb->svd[vic_2d_order].indiv_3d[j - 1].detail_3d);
		 */

	}

	sii_log_print("\n---- Video Capability Data Block ----\n");
	sii_log_debug("QY    : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x80) ? true : false);
	sii_log_debug("QS    : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x40) ? true : false);
	sii_log_debug("S_PT1 : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x20) ? true : false);
	sii_log_debug("S_PT0 : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x10) ? true : false);
	sii_log_debug("S_IT1 : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x08) ? true : false);
	sii_log_debug("S_IT0 : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x04) ? true : false);
	sii_log_debug("S_CE1 : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x02) ? true : false);
	sii_log_debug("S_CE0 : %d\n",
		(po_edid_parsed->vid_cap_db.scan_info & 0x01) ? true : false);

	sii_log_print("\n---- Vendor Specific Video Data Block ----\n");
	sii_log_debug("IEEE ID       : 0x%08X\n",
		po_edid_parsed->vs_vid_db.ieee_oui);

	if (po_edid_parsed->vs_vid_db.length)
		sii_log_debug("Payload bytes : ");
	for (i = 0; i < po_edid_parsed->vs_vid_db.length; i++)
		sii_log_print("%d ", po_edid_parsed->vs_vid_db.ptr_array[i]);

	sii_log_print("\n---- Vendor Specific Audio Data Block ----\n");
	sii_log_debug("IEEE ID       : 0x%08X\n",
		po_edid_parsed->vs_aud_db.ieee_oui);
	if (po_edid_parsed->vs_aud_db.length)
		sii_log_debug("Payload bytes : ");
	for (i = 0; i < po_edid_parsed->vs_aud_db.length; i++)
		sii_log_print("%d ", po_edid_parsed->vs_aud_db.ptr_array[i]);

	sii_log_print("\n---- Y420 Video Data Block ----\n");
	for (i = 0; i < po_edid_parsed->ycc_420_vid_db.length; i++) {
		sii_log_debug("VIC[%d] : %d, Native : %d\n", i,
			po_edid_parsed->ycc_420_vid_db.svd_data[i] & 0x7F,
			((po_edid_parsed->ycc_420_vid_db.svd_data[i] & 0x80) ?
				true : false));
	}

	sii_log_print("\n---- Y420 Capability Map Data Block ----\n");
	for (i = 0; i < po_edid_parsed->vdb.size; i++) {
		if (po_edid_parsed->vdb.svd[i].b_420_mapped) {
			sii_log_debug("VIC[%d] : %d, Native : %d\n", i,
				po_edid_parsed->vdb.svd[i].vic,
				po_edid_parsed->vdb.svd[i].b_native);
		}
	}

	for (i = 2; i < po_edid_parsed->dtdb.size; i++) {
		sii_log_print("\n----- DTD - %d -----\n", i);
		sii_log_debug("Pixel Freq    : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].pixel_freq_10khz);
		sii_log_debug("H Active      : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_active);
		sii_log_debug("H Blank       : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_blank);
		sii_log_debug("V Active      : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_active);
		sii_log_debug("V Blank       : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_blank);
		sii_log_debug("H Front Porch : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_sync_offset);
		sii_log_debug("H Sync        : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_sync_pw);
		sii_log_debug("V Front Porch : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_sync_offset);
		sii_log_debug("V Sync        : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_sync_pw);
		sii_log_debug("H Image Size  : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_image_size);
		sii_log_debug("V Image Size  : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_image_size);
		sii_log_debug("H Border Size : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].h_border);
		sii_log_debug("H Border Size : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].v_border);
		sii_log_debug("Display Type  : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].display_type);
		sii_log_debug("Interface Type: %d\n",
			po_edid_parsed->dtdb.vid_timing[i].interface_type);
		sii_log_debug("Interlaced    : %d\n",
			po_edid_parsed->dtdb.vid_timing[i].b_interlaced);
	}

}
#endif

static uint8_t *s_raw_ptr_get(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr)
{
	uint16_t offset = ((uint16_t)block * SII_EDID_BLOCK_SIZE) + addr;

	return p->pi_edid_raw + offset;
}

static bool s_chksum_pass(const uint8_t *pi_edid_raw, uint8_t block)
{
	uint8_t addr;
	uint8_t check_sum = 0;

	pi_edid_raw += ((uint16_t)block * SII_EDID_BLOCK_SIZE);

	for (addr = 0; addr < SII_EDID_BLOCK_SIZE; addr++) {
		check_sum += pi_edid_raw[addr];
		;
	}

	//return (0x00 == check_sum) ? true : false;
	return true;
}

static void s_parse_chksum(struct edid_data_container_t *p, uint8_t block)
{
	uint8_t *p_data = NULL;

	p_data = s_raw_ptr_get(p, block, 0);

	if (s_chksum_pass(p->pi_edid_raw, block)) {
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("CheckSum OK!!\n");
#endif
	} else {
		p->error = SII_LIB_EDID_ERR_CODE__CHECKSUM;
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("BAD CheckSum!!!!!\n");
#endif
	}
    UNUSED(p_data);
}

static void s_parse_header(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0, SII_LIB_EDID_BL0_ADR__HEADER);
	uint8_t i = 0;

	if (0x00 != p_data[0]) {
		p->error = SII_LIB_EDID_ERR_CODE__BAD_HEADER;

#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("BAD Header!!!!\n");
#endif

		return;
	}

	for (i = 1; i < 7; i++) {
		if (p_data[i] != 0xFF) {
			p->error = SII_LIB_EDID_ERR_CODE__BAD_HEADER;

#if PARSE_EDID_PRINT_INLINE
			sii_log_debug("BAD Header!!!!\n");
#endif

			return;
		}
	}
}

static void s_parse_product_info(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0,
		SII_LIB_EDID_BL0_ADR__MANUFACTURER_ID);

	memcpy(p->po_edid_parsed->product_details, p_data,
		SII_LIB_EDID_BL0_LEN__PROD_DETAILS);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("Product Info : ");
		for (i = 0; i < SII_LIB_EDID_BL0_LEN__PROD_DETAILS; i++) {
			sii_log_debug("%02X ",
				p->po_edid_parsed->product_details[i]);
		}
		sii_log_debug("\n");
	}
#endif

}

#if SII_LIB_EDID_PAR__VERSION
static void s_parse_edid_version(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0, SII_LIB_EDID_BL0_ADR__VERSION);

	p->po_edid_parsed->edid_version = ((uint16_t)p_data[0] << 8) |
		(p_data[1]);

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("EDID Version : %02X.%02X\n",
		p->po_edid_parsed->edid_version >> 8,
		p->po_edid_parsed->edid_version & 0x0F);
#endif

}
#endif

#if SII_LIB_EDID_PAR__DISPLAY_AR
static void s_parse_basic_param(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0,
		SII_LIB_EDID_BL0_ADR__BASIC_DISPLAY_PARAMS);
	memcpy(&p->po_edid_parsed->basic_parameters, p_data,
		SII_LIB_EDID_BL0_LEN__BASIC_DISPLAY_PARAMS);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("Basic Param  : ");
		for (i = 0; i < SII_LIB_EDID_BL0_ADR__BASIC_DISPLAY_PARAMS;
			i++) {
			sii_log_debug("%02X ",
					p->po_edid_parsed->basic_parameters[i]);
		}
		sii_log_debug("\n");
	}
#endif

}
#endif

static void s_parse_chromaticity(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0,
		SII_LIB_EDID_BL0_ADR__CHROMATICITY);

	memcpy(&p->po_edid_parsed->chromaticity, p_data,
		SII_LIB_EDID_BL0_LEN__CHROMATICITY);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("Chromaticity : ");
		for (i = 0; i < SII_LIB_EDID_BL0_LEN__CHROMATICITY; i++) {
			sii_log_debug("%02X ",
				p->po_edid_parsed->chromaticity[i]);
		}
		sii_log_debug("\n");
	}
#endif

}

static void s_parse_est_timings(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0,
		SII_LIB_EDID_BL0_ADR__ESTABLISHED_TIMING_1);

	memcpy(&p->po_edid_parsed->est_timings, p_data, 3);

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("Est Timings  : %02X %02X %02X\n",
		p->po_edid_parsed->est_timings[0],
		p->po_edid_parsed->est_timings[1],
		p->po_edid_parsed->est_timings[2]);
#endif

}

static void s_parse_standard_timing_ids(struct edid_data_container_t *p)
{
	uint8_t *p_data = s_raw_ptr_get(p, 0,
		SII_LIB_EDID_BL0_ADR__STANDARD_TIMING);

	memcpy(&p->po_edid_parsed->std_timings, p_data,
		SII_LIB_EDID_BL0_ADR__STANDARD_TIMING);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("Std Timings  : ");
		for (i = 0; i < SII_LIB_EDID_BL0_LEN__STANDARD_TIMING; i++) {
			sii_log_debug("%02X ",
				p->po_edid_parsed->std_timings[i]);
		}
		sii_log_debug("\n");
	}
#endif

}

static void s_parse_dtd(struct edid_data_container_t *p, uint8_t *p_dtd,
	uint8_t block)
{
	struct sii_lib_edid_video_timing_t vid_tim;

	vid_tim.b_valid = true;
	vid_tim.pixel_freq_10khz = p_dtd[SII_LIB_EDID_DTD_ADR__PIX_CLK_LOW] |
		(p_dtd[SII_LIB_EDID_DTD_ADR__PIX_CLK_HIGH] << 8);
	vid_tim.h_active = p_dtd[SII_LIB_EDID_DTD_ADR__H_ACTIVE_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_HIGH] & 0xF0) << 4);
	vid_tim.h_blank = p_dtd[SII_LIB_EDID_DTD_ADR__H_BLANK_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_HIGH] & 0x0F) << 8);
	vid_tim.v_active = p_dtd[SII_LIB_EDID_DTD_ADR__V_ACTIVE_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__V_HIGH] & 0xF0) << 4);
	vid_tim.v_blank	= p_dtd[SII_LIB_EDID_DTD_ADR__V_BLANK_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__V_HIGH] & 0x0F) << 8);
	vid_tim.h_sync_offset = p_dtd[SII_LIB_EDID_DTD_ADR__H_SYNC_OFFSET_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_V_SYNC_HIGH] & 0xC0) << 2);
	vid_tim.h_sync_pw = p_dtd[SII_LIB_EDID_DTD_ADR__H_SYNC_WIDTH_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_V_SYNC_HIGH] & 0x30) << 4);
	vid_tim.v_sync_offset =
		((p_dtd[SII_LIB_EDID_DTD_ADR__V_SYNC_LOW] & 0xF0) >> 4) |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_V_SYNC_HIGH] & 0x0C) << 2);
	vid_tim.v_sync_pw = (p_dtd[SII_LIB_EDID_DTD_ADR__V_SYNC_LOW] & 0x0F) |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_V_SYNC_HIGH] & 0x03) << 4);
	vid_tim.h_image_size = p_dtd[SII_LIB_EDID_DTD_ADR__H_SIZE_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_V_SIZE_HIGH] & 0xF0) << 4);
	vid_tim.v_image_size = p_dtd[SII_LIB_EDID_DTD_ADR__V_SIZE_LOW] |
		((p_dtd[SII_LIB_EDID_DTD_ADR__H_V_SIZE_HIGH] & 0x0F) << 8);
	vid_tim.h_border = p_dtd[SII_LIB_EDID_DTD_ADR__H_BORDER];
	vid_tim.v_border = p_dtd[SII_LIB_EDID_DTD_ADR__V_BORDER];


	vid_tim.b_interlaced = (p_dtd[SII_LIB_EDID_DTD_ADR__FLAGS] & 0x80) ?
		(true) : (false);
	vid_tim.display_type = (p_dtd[SII_LIB_EDID_DTD_ADR__FLAGS] & 0x01) |
		((p_dtd[SII_LIB_EDID_DTD_ADR__FLAGS] & 0x60) >> 4);
	vid_tim.interface_type =
		(p_dtd[SII_LIB_EDID_DTD_ADR__FLAGS] & 0x1E) >> 1;


	if (SII_LIB_EDID__DTD_MAX > p->po_edid_parsed->dtdb.size) {
		p->po_edid_parsed->dtdb.vid_timing[p->po_edid_parsed->dtdb.size] = vid_tim;
		p->po_edid_parsed->dtdb.size++;

		switch (block) {
		case EDID_BLOCK_0:
			p->po_edid_parsed->dtdb.num_vesa_timing_dtds++;
			break;
		case EDID_BLOCK_2_3:
			p->po_edid_parsed->dtdb.num_cea_861_timing_dtds++;
			break;
		default:
			break;
		}
	}

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n ----- DTD -----\n");
	sii_log_debug("Pixel Freq    : %d\n", vid_tim.pixel_freq_10khz);
	sii_log_debug("H Active      : %d\n", vid_tim.h_active);
	sii_log_debug("H Blank       : %d\n", vid_tim.h_blank);
	sii_log_debug("V Active      : %d\n", vid_tim.v_active);
	sii_log_debug("V Blank       : %d\n", vid_tim.v_blank);
	sii_log_debug("H Front Porch : %d\n", vid_tim.h_sync_offset);
	sii_log_debug("H Sync        : %d\n", vid_tim.h_sync_pw);
	sii_log_debug("V Front Porch : %d\n", vid_tim.v_sync_offset);
	sii_log_debug("V Sync        : %d\n", vid_tim.v_sync_pw);
	sii_log_debug("H Image Size  : %d\n", vid_tim.h_image_size);
	sii_log_debug("V Image Size  : %d\n", vid_tim.v_image_size);
	sii_log_debug("H Border Size : %d\n", vid_tim.h_border);
	sii_log_debug("H Border Size : %d\n", vid_tim.v_border);
	sii_log_debug("Display Type  : %d\n", vid_tim.display_type);
	sii_log_debug("Interface Type: %d\n", vid_tim.interface_type);
	sii_log_debug("Interlaced    : %d\n", vid_tim.b_interlaced);
#endif

}

#if (SII_LIB_EDID_PAR__MONITOR_NAME)
static void s_parse_first_monitor_desc_block(struct edid_data_container_t *p,
						uint8_t *p_data)
{
	memcpy(p->po_edid_parsed->monitor_name_str,
		&p_data[SII_LIB_EDID_DTD_ADR__DATA],
		SII_LIB_DTD_DESCRIPTOR_DATA_LENGTH);

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\nMonitor Name : %s\n",
		p->po_edid_parsed->monitor_name_str);
#endif

}
#endif

static void s_parse_second_monitor_desc_block(struct edid_data_container_t *p,
						uint8_t *p_data)
{
	memcpy(&p->po_edid_parsed->monitor_ranges, &p_data[0],
		SII_LIB_EDID_LEN__DTD);

	p->po_edid_parsed->max_pixel_clock_10mhz =
		p_data[SII_LIB_EDID_MRL_ADR__PCLK_MAX];

}

static void s_parse_dtd_block(struct edid_data_container_t *p, uint8_t block,
				uint8_t dtd_addr)
{
	uint8_t *p_dtd = s_raw_ptr_get(p, block, dtd_addr);

	if (!p_dtd[SII_LIB_EDID_DTD_ADR__PIX_CLK_LOW] &&
		!p_dtd[SII_LIB_EDID_DTD_ADR__PIX_CLK_HIGH] &&
		!p_dtd[SII_LIB_EDID_DTD_ADR__H_HIGH]) {

			if (SII_LIB_EDID_DTD_TAG__NAME ==
				p_dtd[SII_LIB_EDID_DTD_ADR__DATA_TYPE_TAG]) {
				#if (SII_LIB_EDID_PAR__MONITOR_NAME)
				s_parse_first_monitor_desc_block(p, p_dtd);
                #endif

			} else if (SII_LIB_EDID_DTD_TAG__RANGE_LIMITS ==
				p_dtd[SII_LIB_EDID_DTD_ADR__DATA_TYPE_TAG]) {
					s_parse_second_monitor_desc_block(p,
							p_dtd);
			}
	} else
		s_parse_dtd(p, p_dtd, block);
}

static void s_find_extensions(struct edid_data_container_t *p)
{
	p->extensions = p->pi_edid_raw[SII_LIB_EDID_BL0_ADR__EXTENSION_NMB];

	/* 2 extension blocks is impossible*/
	/* either 1 or 3, 4 etc */
	if (2 == p->extensions) {
		p->error = SII_LIB_EDID_ERR_CODE__NOEXTENSIONS;

#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("NO Extensions\n");
#endif

		return;
	}

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("Number of Extensions : %02X\n", p->extensions);
#endif

}

static void s_parse_basic_info(struct edid_data_container_t *p)
{
	uint8_t i = 0;
	uint8_t dtd_addr = SII_LIB_EDID_BL0_ADR__DTD1;

	/* Check EDID Header */
	s_parse_header(p);
	if (p->error)
		return;

	/* Check Product Information */
	s_parse_product_info(p);
	if (p->error)
		return;

#if SII_LIB_EDID_PAR__VERSION
	/* check EDID version */
	s_parse_edid_version(p);
	if (p->error)
		return;
#endif

#if SII_LIB_EDID_PAR__DISPLAY_AR
	/* Check Basic Display Parameters */
	s_parse_basic_param(p);
	if (p->error)
		return;
#endif

	/* Parse Chromaticity Parameters */
	s_parse_chromaticity(p);
	if (p->error)
		return;

	/* Established VESA Timings */
	s_parse_est_timings(p);

	/* Standard VESA Timing Identification */
	s_parse_standard_timing_ids(p);
	if (p->error)
		return;

	/* Check Detailed Decriptor Blocks */
	for (i = 0; i < 4; i++) {
		s_parse_dtd_block(p, 0, dtd_addr);
		if (p->error)
			return;

		dtd_addr += 18;
	}

	/* Find number of extensions */
	s_find_extensions(p);
	if (p->error)
		return;
}

static void s_check_861b(struct edid_data_container_t *p, uint8_t block)
{
	uint8_t *p_data = s_raw_ptr_get(p, block, 0x00);

	if (2 != p_data[SII_LIB_EDID_EXT_ADR__TAG]) {
		p->error = SII_LIB_EDID_ERR_CODE__CEA_TAG;
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("\nBAD Tag!!!\n");
#endif
		return;
	}

	/* CEA 861B version */
	if (3 != p_data[SII_LIB_EDID_EXT_ADR__REVISION]) {
		p->error = SII_LIB_EDID_ERR_CODE__NO_861B;
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("\nBAD Revision!!!\n");
#endif
		return;
	}
}

static void s_parse_cea_ext3_byte3(struct edid_data_container_t *p,
					uint8_t block)
{
	uint8_t *p_data = s_raw_ptr_get(p, block, 0x03);

	p->po_edid_parsed->b_it_underscan =
		(p_data[0] & 0x80) ? (true) : (false);
	p->po_edid_parsed->b_basic_audio =
		(p_data[0] & 0x40) ? (true) : (false);
	p->po_edid_parsed->b_ycc444 = (p_data[0] & 0x20) ? (true) : (false);
	p->po_edid_parsed->b_ycc422 = (p_data[0] & 0x10) ? (true) : (false);
	p->po_edid_parsed->native_dtd_cnt = (p_data[0] & 0x0F);

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\nBasic Audio Support : %d\n",
		p->po_edid_parsed->b_basic_audio);
	sii_log_debug("IT Underscan        : %d\n",
		p->po_edid_parsed->b_it_underscan);
	sii_log_debug("YCC444 support      : %d\n",
		p->po_edid_parsed->b_ycc444);
	sii_log_debug("YCC422 support      : %d\n",
		p->po_edid_parsed->b_ycc422);
#endif
}

static void s_parse_sad_table(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{

	uint8_t *p_data = NULL;

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n---- Audio Data Block ----\n");
#endif
	while (SII_LIB_EDID_SAD__LENGTH <= len) {

		struct sii_lib_edid_sad_t short_aud_desc;

		p_data = s_raw_ptr_get(p, block, addr);

		memcpy((uint8_t *)&short_aud_desc, &p_data[0],
			SII_LIB_EDID_SAD__LENGTH);

#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("SAD : %02X %02X %02X\n",
			short_aud_desc.audio_data[0],
			short_aud_desc.audio_data[1],
			short_aud_desc.audio_data[2]);
#endif
		if (p->po_edid_parsed->adb.size < SII_LIB_EDID__SAD_MAX) {

			p->po_edid_parsed->adb.
				sad[p->po_edid_parsed->adb.size] =
					short_aud_desc;
			p->po_edid_parsed->adb.size++;
		}

		addr += SII_LIB_EDID_SAD__LENGTH;
		len -= SII_LIB_EDID_SAD__LENGTH;
	}
#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("Number of SADs : %d\n", p->po_edid_parsed->adb.size);
#endif
}

static void s_parse_svd_table(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data = NULL;

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n---- Video Data Block ----\n");
#endif
	while (SII_LIB_EDID_SVD__LENGTH <= len) {

		uint8_t cntr = 0;
		struct sii_lib_edid_short_video_desc_t svd = {0};

		p_data = s_raw_ptr_get(p, block, addr);

		svd.vic = p_data[0] & 0x7F;
		svd.b_native = (p_data[0] & 0x80) ? 1 : 0;
		svd.b_valid = true;

		/*
		 * make sure 3D related fields are false
		 * they will be update while parsing vendor specific data block
		 */
		svd.b_masked_3d = false;
		svd.b_3d_present = false;
		svd.b_indiv_3d_present = false;
		for (cntr = 0;
			cntr < SII_LIB_EDID__INDIV_3D_MODES_PER_VIC_MAX;) {
			svd.indiv_3d[cntr].b_indiv_3d_present = false;
			cntr++;
		}

		/*
		 * to accomodate all required field in CEA - EXT
		 * only one video block has taken into consideration
		 */

		if (p->po_edid_parsed->vdb.size < SII_LIB_EDID__SVD_MAX) {
			p->po_edid_parsed->vdb.
				svd[p->po_edid_parsed->vdb.size] = svd;

#if PARSE_EDID_PRINT_INLINE
			sii_log_debug("VIC  : %d,	Native  : %d\n",
				p->po_edid_parsed->vdb.svd[p->po_edid_parsed->
					vdb.size].vic,
				p->po_edid_parsed->vdb.svd[p->po_edid_parsed->
					vdb.size].b_native);
#endif

			p->po_edid_parsed->vdb.size++;
		}

		addr += SII_LIB_EDID_SVD__LENGTH;
		len -= SII_LIB_EDID_SVD__LENGTH;
	}
#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("Number of SVDs : %d\n", p->po_edid_parsed->vdb.size);
#endif
}

static void s_parse_spk_alloc(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data = NULL;
#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n---- Speaker Allocation Data Block ----\n");
#endif
	p->po_edid_parsed->spkdb.b_present = true;
	while (SII_LIB_EDID_SPK__LENGTH <= len) {
		p_data = s_raw_ptr_get(p, block, addr);
		memcpy(p->po_edid_parsed->spkdb.spk_alloc, p_data,
			SII_LIB_EDID_SPK__LENGTH);

#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("%02X, %02X, %02X\n",
			p->po_edid_parsed->spkdb.spk_alloc[0],
			p->po_edid_parsed->spkdb.spk_alloc[1],
			p->po_edid_parsed->spkdb.spk_alloc[2]);
#endif

		addr += SII_LIB_EDID_SPK__LENGTH;
		len -= SII_LIB_EDID_SPK__LENGTH;
	}
}

static void s_get_cec_addr(uint8_t *p_cec_addr, uint8_t *p_data)
{
	p_cec_addr[0] = (p_data[0] >> 4) & 0xF;
	p_cec_addr[1] = (p_data[0] >> 0) & 0xF;
	p_cec_addr[2] = (p_data[1] >> 4) & 0xF;
	p_cec_addr[3] = (p_data[1] >> 0) & 0xF;
}

static void s_parse_vsdb_hdmi14(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data = NULL;
	uint8_t flags = 0;

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n---- Vendor Specific Data Block ----\n");
#endif
	p->po_edid_parsed->is_vsdb_present = true;
	p->po_edid_parsed->vsdb.cec_addr_ptr = (block * 128) + addr;

	if (2 > len)
		return;

	p_data = s_raw_ptr_get(p, block, addr);
	len -= 2;
	addr += 2;

	s_get_cec_addr(p->po_edid_parsed->vsdb.cec_addr, p_data);

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("CEC Address	: %02d.%02d.%02d.%02d\n",
		p->po_edid_parsed->vsdb.cec_addr[0],
		p->po_edid_parsed->vsdb.cec_addr[1],
		p->po_edid_parsed->vsdb.cec_addr[2],
		p->po_edid_parsed->vsdb.cec_addr[3]);
#endif

	if (len) {
		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		p->po_edid_parsed->vsdb.b_supports_ai =
			(p_data[0] & 0x80) ? (true) : (false);
		p->po_edid_parsed->vsdb.b_y444_dc10 =
			(p_data[0] & 0x10) ? (true) : (false);
		p->po_edid_parsed->vsdb.b_y444_dc12 =
			(p_data[0] & 0x20) ? (true) : (false);
		p->po_edid_parsed->vsdb.b_y444_dc16 =
			(p_data[0] & 0x40) ? (true) : (false);
		p->po_edid_parsed->vsdb.b_y444 =
			(p_data[0] & 0x08) ? (true) : (false);
	}

	if (!len)
		return;

	p_data = s_raw_ptr_get(p, block, addr);
	addr++;
	len--;

	p->po_edid_parsed->max_tmds_5mhz = p_data[0];
#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\nMax TMDS Clock	: %d MHz\n",
		p->po_edid_parsed->max_tmds_5mhz * 5);
#endif

	if (!len)
		return;

	p_data = s_raw_ptr_get(p, block, addr);
	addr++;
	len--;

	flags = p_data[0];
	p->po_edid_parsed->vsdb.content_type = flags & 0x0F;
#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("Content Type	: 0x%02X\n",
		p->po_edid_parsed->vsdb.content_type);
#endif

	if (flags & 0x80) {

		if (!len)
			return;

		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		p->po_edid_parsed->vsdb.lipsync.b_latency_present = true;
		p->po_edid_parsed->vsdb.lipsync.video_latency = p_data[0];
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("Video Latency	: %d\n",
			p->po_edid_parsed->vsdb.lipsync.video_latency);
#endif

		if (!len)
			return;

		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		p->po_edid_parsed->vsdb.lipsync.audio_latency = p_data[0];
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("Audio Latency	: %d\n",
			p->po_edid_parsed->vsdb.lipsync.audio_latency);
#endif
	}

	if (flags & 0x40) {

		if (!len)
			return;

		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		p->po_edid_parsed->vsdb.lipsync.b_i_latency_present = true;
		p->po_edid_parsed->vsdb.lipsync.i_video_latency = p_data[0];
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("I_Video Latency	: %d\n",
			p->po_edid_parsed->vsdb.lipsync.i_video_latency);
#endif

		if (!len)
			return;

		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		p->po_edid_parsed->vsdb.lipsync.i_audio_latency = p_data[0];
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("I_Audio Latency	: %d\n",
			p->po_edid_parsed->vsdb.lipsync.i_audio_latency);
#endif
	}

	if (flags & 0x20) {

		/* Parse 3D information */
		uint8_t hdmi_vic_len = 0;
		uint8_t len_3d = 0;
		uint8_t multi_3d = 0;
		uint8_t detail_3d = 0x00;
		uint8_t struct_3d_indiv = 0x00;
		uint16_t struct_3d_all = 0x0000;

		struct sii_lib_edid_vsdb_t *p_vsdb = &p->po_edid_parsed->vsdb;
		struct sii_lib_edid_video_db_t *p_vdb = &p->po_edid_parsed->vdb;

		if (!len)
			return;

		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		multi_3d = (p_data[0] & 0x60) >> 5;

		p_vsdb->multi_3d_type = (p_data[0] & 0x60) >> 5;
		p_vsdb->b_3d_present = (p_data[0] & 0x80) ? true : false;
		p_vsdb->image_size = (p_data[0] & 0x18) >> 3;
#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("3D Support	: %d\n", p_vsdb->b_3d_present);
		sii_log_debug("3D Multi	: %d\n", p_vsdb->multi_3d_type);
		sii_log_debug("Image Size	: %d\n", p_vsdb->image_size);
#endif
		if (!len)
			return;

		p_data = s_raw_ptr_get(p, block, addr);
		addr++;
		len--;

		p_vsdb->hdmi_vic_len = (p_data[0] & 0xE0) >> 5;
		p_vsdb->hdmi_3d_len = p_data[0] & 0x1F;
		hdmi_vic_len = p_vsdb->hdmi_vic_len;
		len_3d = p_vsdb->hdmi_3d_len;

#if PARSE_EDID_PRINT_INLINE
		sii_log_debug("HDMI VIC length	: %d\n", p_vsdb->hdmi_vic_len);
		sii_log_debug("HDMI 3D length	: %d\n", p_vsdb->hdmi_3d_len);
		if (p_vsdb->hdmi_vic_len)
			sii_log_debug("\nHDMI VIC	:\n");
#endif

		while (hdmi_vic_len) {
			p_data = s_raw_ptr_get(p, block, addr);
			addr++;
			len--;

			p_vsdb->hdmi_vic[hdmi_vic_len - 1] = p_data[0];
			hdmi_vic_len--;

#if PARSE_EDID_PRINT_INLINE
			sii_log_debug("   VIC : %2d\n",
				p_vsdb->hdmi_vic[hdmi_vic_len]);
#endif
		}

		if (2 <= len_3d) {
			if ((0x01 == multi_3d) || (0x02 == multi_3d)) {

				uint8_t i = 0;
				if ((0x01 == multi_3d))
					for (i = 0; i < 16; i++)
						p_vdb->svd[i].b_3d_present
						= true;

				if (!len)
					return;

				p_data = s_raw_ptr_get(p, block, addr);
				addr++;
				len--;
				len_3d--;

				struct_3d_all = (p_data[0] << 8) | p_data[1];
				p_vsdb->struct_3d_all = struct_3d_all;

				addr++;
				len--;
				len_3d--;

#if PARSE_EDID_PRINT_INLINE
				sii_log_debug("\n3D Struct ALL	: 0x%04X\n",
					p_vsdb->struct_3d_all);
#endif
			}
		}

		if (2 <= len_3d) {
			if (0x02 == multi_3d) {

				uint16_t mask_3d;
				uint8_t bit_pos = 0;

				if (!len)
					return;

				p_data = s_raw_ptr_get(p, block, addr);
				addr++;
				len--;
				len_3d--;

				mask_3d = (p_data[0] << 8) | p_data[1];
				addr++;
				len--;
				len_3d--;
#if PARSE_EDID_PRINT_INLINE
				sii_log_debug("3D MASK	: 0x%04X\n", mask_3d);
#endif
				for (bit_pos = 0; bit_pos < 16; bit_pos++) {
					if ((1 << bit_pos) & mask_3d) {
						p_vdb->svd[bit_pos].b_3d_present = true;
					}
				}
			}
		}

#if PARSE_EDID_PRINT_INLINE
		if (len_3d)
			sii_log_debug("\nIndividual 3D modes :\n");
#endif
		while (len_3d) {

			/* Parse individual 3D mode information */
			uint8_t j = 0;
			uint8_t vic_2d_order = 0;

			if (!len)
				return;

			p_data = s_raw_ptr_get(p, block, addr);
			addr++;
			len--;
			len_3d--;

			vic_2d_order = (p_data[0] & 0xF0) >> 4;
			struct_3d_indiv = p_data[0] & 0x0F;

			for (j = 0;
				j < SII_LIB_EDID__INDIV_3D_MODES_PER_VIC_MAX;
				j++) {
				if (p_vdb->svd[vic_2d_order].indiv_3d[j].b_indiv_3d_present)
					continue;
				/* continue to find next available struct */
				else {
					p_vdb->svd[vic_2d_order].b_indiv_3d_present = true;
					p_vdb->svd[vic_2d_order].indiv_3d[j].b_indiv_3d_present = true;
					p_vdb->svd[vic_2d_order].indiv_3d[j].struct_3d = struct_3d_indiv;
					break;
				}
			}
#if PARSE_EDID_PRINT_INLINE
			sii_log_debug("2D VIC order : %d	",
					vic_2d_order);
			sii_log_debug("3D Struct : 0x%02X	",
				p_vdb->svd[vic_2d_order].indiv_3d[j].struct_3d);
#endif

			if ((0x07 < struct_3d_indiv) && len_3d) {
				if (!len)
					return;

				p_data = s_raw_ptr_get(p, block, addr);
				addr++;
				len--;
				len_3d--;

				detail_3d = (p_data[0] & 0xF0) >> 4;
				p_vdb->svd[vic_2d_order].indiv_3d[j - 1].
					detail_3d = detail_3d;
#if PARSE_EDID_PRINT_INLINE
				sii_log_debug("3D Detail : 0x%02X\n",
					detail_3d);
#endif
			} else {
#if PARSE_EDID_PRINT_INLINE
				sii_log_debug("\n");
#endif
			}

		}
	}
}

static void s_parse_vsdb_hdmi_forum(struct edid_data_container_t *p,
					uint8_t block, uint8_t addr,
					uint8_t len)
{
	uint8_t *p_data = s_raw_ptr_get(p, block, addr);

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n---- HF Vendor Specific Data Block ----\n");
	sii_log_debug("Version 0x%02X\n", p_data[0]);
#endif
	len--;

	p->po_edid_parsed->is_hfvsdb_present = true;
	p->po_edid_parsed->hfvsdb.vclk_mb = (uint32_t)p_data[1] * 5;
	len--;

	p->po_edid_parsed->hfvsdb.b_scdc_present =
		(p_data[2] & 0x80) ? (true) : (false);
	p->po_edid_parsed->hfvsdb.b_read_req_capable =
		(p_data[2] & 0x40) ? (true) : (false);
	p->po_edid_parsed->hfvsdb.b_lte_340mscs_scramble =
		(p_data[2] & 0x08) ? (true) : (false);

	p->po_edid_parsed->hfvsdb.b_independent_view =
		(p_data[2] & 0x04) ? (true) : (false);
	p->po_edid_parsed->hfvsdb.b_dual_view =
		(p_data[2] & 0x02) ? (true) : (false);
	p->po_edid_parsed->hfvsdb.b_3d_osd_disparity =
		(p_data[2] & 0x01) ? (true) : (false);
	len--;

	p->po_edid_parsed->hfvsdb.b_dc48bit420 =
		(p_data[3] & 0x04) ? (true) : (false);
	p->po_edid_parsed->hfvsdb.b_dc36bit420 =
		(p_data[3] & 0x02) ? (true) : (false);
	p->po_edid_parsed->hfvsdb.b_dc30bit420 =
		(p_data[3] & 0x01) ? (true) : (false);
	len--;

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("Max_TMDS_Character_Rate	: %d\n",
		p->po_edid_parsed->hfvsdb.vclk_mb);
	sii_log_debug("SCDC_Present            : %d\n",
		p->po_edid_parsed->hfvsdb.b_scdc_present);
	sii_log_debug("RR_Capable              : %d\n",
		p->po_edid_parsed->hfvsdb.b_read_req_capable);
	sii_log_debug("LTE_340Mcsc_scramble    : %d\n",
		p->po_edid_parsed->hfvsdb.b_lte_340mscs_scramble);
	sii_log_debug("Independent_view        : %d\n",
		p->po_edid_parsed->hfvsdb.b_independent_view);
	sii_log_debug("Dual_View               : %d\n",
		p->po_edid_parsed->hfvsdb.b_dual_view);
	sii_log_debug("3D_OSD_Disparity        : %d\n",
		p->po_edid_parsed->hfvsdb.b_3d_osd_disparity);
	sii_log_debug("DC_48bit_420            : %d\n",
		p->po_edid_parsed->hfvsdb.b_dc48bit420);
	sii_log_debug("DC_36bit_420            : %d\n",
		p->po_edid_parsed->hfvsdb.b_dc36bit420);
	sii_log_debug("DC_30bit_420            : %d\n",
		p->po_edid_parsed->hfvsdb.b_dc30bit420);
#endif
}

static void s_parse_vsdb(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data = NULL;
	uint32_t ieee = 0x000000;

	if (3 > len)
		return;

	p_data = s_raw_ptr_get(p, block, addr);
	len -= 3;
	addr += 3;

	ieee = ((uint32_t)p_data[0] << 0) + ((uint32_t)p_data[1] << 8) +
		   ((uint32_t)p_data[2] << 16);

	p->po_edid_parsed->ieee_id = ieee;

	switch (ieee) {
	case SII_LIB_EDID_VSB_DAT__HDMI_SIGNATURE:
		s_parse_vsdb_hdmi14(p, block, addr, len);
		p->po_edid_parsed->hdmi_sink = true;
		break;

	case SII_LIB_EDID_VSB_DAT__HDMI_FORUM_SIGNATURE:
		s_parse_vsdb_hdmi_forum(p, block, addr, len);
		break;

	default:
		return;
	}
}

static void s_parse_vid_cap_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr)
{
	uint8_t *p_data;

	p_data = s_raw_ptr_get(p, block, addr);
	p->po_edid_parsed->vid_cap_db.b_present = true;
	p->po_edid_parsed->vid_cap_db.scan_info = p_data[0];

#if PARSE_EDID_PRINT_INLINE
	sii_log_debug("\n---- Video Capability Data Block ----\n");
	sii_log_debug("QY    : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x80) ?
			true : false);
	sii_log_debug("QS    : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x40) ?
			true : false);
	sii_log_debug("S_PT1 : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x20) ?
			true : false);
	sii_log_debug("S_PT0 : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x10) ?
			true : false);
	sii_log_debug("S_IT1 : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x08) ?
			true : false);
	sii_log_debug("S_IT0 : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x04) ?
			true : false);
	sii_log_debug("S_CE1 : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x02) ?
			true : false);
	sii_log_debug("S_CE0 : %d\n",
		(p->po_edid_parsed->vid_cap_db.scan_info & 0x01) ?
			true : false);
#endif
}

static void s_parse_vs_vid_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data;

	p_data = s_raw_ptr_get(p, block, addr);
	len -= 3;
	addr += 3;

	p->po_edid_parsed->vs_vid_db.b_present = true;

	p->po_edid_parsed->vs_vid_db.ieee_oui = ((uint32_t)p_data[0] << 0) +
						((uint32_t)p_data[1] << 8) +
						((uint32_t)p_data[2] << 16);

	if (len > 0) {
		p_data = s_raw_ptr_get(p, block, addr);

		p->po_edid_parsed->vs_vid_db.length = len;
		memcpy(&p->po_edid_parsed->vs_vid_db.ptr_array[0], &p_data[0],
			len);
	}
#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("\n---- Vendor Specific Video Data Block ----\n");
		sii_log_debug("IEEE ID       : 0x%08X\n",
			p->po_edid_parsed->vs_vid_db.ieee_oui);
		if (len)
			sii_log_debug("Payload bytes : ");
		for (i = 0; i < len; i++)
			sii_log_debug("%d ",
				p->po_edid_parsed->vs_vid_db.ptr_array[i]);
	}
#endif
}

static void s_parse_vs_aud_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data;

	p_data = s_raw_ptr_get(p, block, addr);
	len -= 3;
	addr += 3;

	p->po_edid_parsed->vs_aud_db.b_present = true;

	p->po_edid_parsed->vs_aud_db.ieee_oui = ((uint32_t)p_data[0] << 0)
		+ ((uint32_t)p_data[1] << 8) + ((uint32_t)p_data[2] << 16);
	if (len > 0) {
		p_data = s_raw_ptr_get(p, block, addr);

		p->po_edid_parsed->vs_aud_db.length = len;
		memcpy(&p->po_edid_parsed->vs_aud_db.ptr_array[0], &p_data[0],
			len);
	}

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("\n---- Vendor Specific Audio Data Block ----\n");
		sii_log_debug("IEEE ID       : 0x%08X\n",
			p->po_edid_parsed->vs_aud_db.ieee_oui);
		if (len)
			sii_log_debug("Payload bytes : ");
		for (i = 0; i < len; i++)
			sii_log_debug("%d ",
				p->po_edid_parsed->vs_aud_db.ptr_array[i]);
	}
#endif
}

static void s_parse_hdr_stat_md_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data;
	if (len > SII_LIB_EDID_HDR_SMD__LENGTH) {
		sii_log_debug("\nError: HDR SMD DB Max Size Exceeds ----\n");
		return;
	}
	p_data = s_raw_ptr_get(p, block, addr);

	p->po_edid_parsed->hdr_stat_md_db.length = len;
	memcpy(&p->po_edid_parsed->hdr_stat_md_db.hdr_stat_md[0], &p_data[0], len);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("\n---- HDR Static MD Data Block ----\n");
		for (i = 0; i < len; i++) {
			sii_log_debug("%x\n",p_data[i]);
		}
	}
#endif
}

static void s_parse_hdr_dyn_md_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data;
	if (len > SII_LIB_EDID_HDR_DMD__LENGTH) {
		sii_log_debug("\nError: HDR DMD DB Max Size Exceeds ----\n");
		return;
	}

	p_data = s_raw_ptr_get(p, block, addr);

	p->po_edid_parsed->hdr_dyn_md_db.length = len;
	memcpy(&p->po_edid_parsed->hdr_dyn_md_db.hdr_dyn_md[0], &p_data[0], len);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("\n---- HDR Dynamic MD Data Block ----\n");
		for (i = 0; i < len; i++) {
			sii_log_debug("%x\n",p_data[i]);
		}
	}
#endif
}

static void s_parse_420_vid_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data;

	p_data = s_raw_ptr_get(p, block, addr);

	p->po_edid_parsed->ycc_420_vid_db.length = len;
	memcpy(&p->po_edid_parsed->ycc_420_vid_db.svd_data[0], &p_data[0], len);

#if PARSE_EDID_PRINT_INLINE
	{
		uint8_t i = 0;

		sii_log_debug("\n---- Y420 Video Data Block ----\n");
		for (i = 0; i < len; i++) {
			sii_log_debug("VIC	: %d, Native	: %d\n",
				p->po_edid_parsed->ycc_420_vid_db.svd_data[i] & 0x7F,
				((p->po_edid_parsed->ycc_420_vid_db.svd_data[i] & 0x80) ? true : false));
		}
	}

#endif
}

static void s_parse_420_cap_map_db(struct edid_data_container_t *p,
	uint8_t block, uint8_t addr, uint8_t len)
{
	uint8_t i, j;
	uint8_t data = 0;
	uint8_t *p_data;
	uint8_t num_svds = len * 8;

	p_data = s_raw_ptr_get(p, block, addr);
	if (len == 0) {
		i = 0;
		while (i < SII_LIB_EDID__SVD_MAX) {
			p->po_edid_parsed->vdb.svd[i].b_420_mapped = true;
			i++;
		}
	} else {
		for (i=0; i < num_svds; i += 8)
		{
			data = p_data[i/8];
			for (j = 0; j < 8; j++) {
				if (data & (1 << j))
					p->po_edid_parsed->vdb.svd[i+j].b_420_mapped = true;
			}
		}
	}
	p->po_edid_parsed->_420_cap_map_data_blk_len = len;

#if PARSE_EDID_PRINT_INLINE

	sii_log_debug("\n---- Y420 Capability Map Data Block ----\n");
	for (i = 0; i < p->po_edid_parsed->vdb.size; i++) {
		if (p->po_edid_parsed->vdb.svd[i].b_420_mapped) {
			sii_log_debug("VIC	: %d, Native	: %d\n",
				p->po_edid_parsed->vdb.svd[i].vic,
				p->po_edid_parsed->vdb.svd[i].b_native);
		}
	}

#endif
}

static void s_parse_calorimetry_db(struct edid_data_container_t *p,
					uint8_t block, uint8_t addr)
{
	uint8_t *p_data;

	p_data = s_raw_ptr_get(p, block, addr);

	p->po_edid_parsed->calorimetry_db.calorimetry_present = true;
	p->po_edid_parsed->calorimetry_db.calorimetry_byte = p_data[0];

}

static void s_parse_extended_db(struct edid_data_container_t *p, uint8_t block,
				uint8_t addr, uint8_t len)
{
	uint8_t *p_data;

	p_data = s_raw_ptr_get(p, block, addr);
	addr++;
	len--;

	switch (p_data[0]) {
	case 0x00:
		/* Extended video capability data block */
		s_parse_vid_cap_db(p, block, addr);
		break;

	case 0x01:
		/* Extended vendor specific video data block */
		s_parse_vs_vid_db(p, block, addr, len);
		break;

	case 0x02:
		/* Extended vendor specific audio data block */
		s_parse_vs_aud_db(p, block, addr, len);
		break;

	case 0x05:
		/* Extended calorimetry data block */
		s_parse_calorimetry_db(p, block, addr);
		break;

	case 0x06:
		/* Extended HDR Static Metadata Data Block*/
		s_parse_hdr_stat_md_db(p, block, addr, len);
		break;

	case 0x07:
		/* Extended HDR Dynamic Metadata Data Block*/
		s_parse_hdr_dyn_md_db(p, block, addr, len);
		break;

	case 0x0E:
		/* Extended YCC 420 video data block */
		s_parse_420_vid_db(p, block, addr, len);
		break;

	case 0x0F:
		/* Extended YCC 420 capability map data block */
		s_parse_420_cap_map_db(p, block, addr, len);
		break;

	default:
		break;
	}
}

static void s_cea_data_block_collection(struct edid_data_container_t *p,
					uint8_t block)
{
	uint8_t addr = 4;
	uint8_t start_of_dtd = 4;
	uint8_t *p_data;
	uint8_t tag = 0;
	uint8_t len = 0;

	/* Check CEA 861 tag and ext revision */
	s_check_861b(p, block);
	if (p->error)
		return;

	s_parse_cea_ext3_byte3(p, block);
	if (p->error)
		return;

	/* Read End address of data blocks */
	p_data = s_raw_ptr_get(p, block, 0x02);
	start_of_dtd = p_data[0];

	while (start_of_dtd > addr) {

		p_data = s_raw_ptr_get(p, block, addr);
		tag = (p_data[0] & 0xE0) >> 5;
		len = (p_data[0] & 0x1F);

		addr++; /* increment to skip data block header */

		switch (tag) {
		case 0x01:
			/* CEA Short Audio Descriptor Block */
			s_parse_sad_table(p, block, addr, len);
			break;

		case 0x02:
			/* CEA Short Video Descriptor */
			s_parse_svd_table(p, block, addr, len);
			break;

		case 0x03:
			/* Vendor Specific Data Block */
			s_parse_vsdb(p, block, addr, len);
			break;

		case 0x04:
			/* Speaker Allocation data Block */
			s_parse_spk_alloc(p, block, addr, len);
			break;

		case 0x07:
			/* Extended data Blocks */
			s_parse_extended_db(p, block, addr, len);
			break;

		default:
			break;
		}

		addr += len;
	}
}

static void s_parse_edid(struct edid_data_container_t *edid_data_param)
{
	uint8_t block = 0;
	uint8_t start_addr_dtd = 0;

	/* Check CheckSum first Block */
	s_parse_chksum(edid_data_param, 0);
	if (edid_data_param->error)
		return;

	/* Parse first block */
	s_parse_basic_info(edid_data_param);
	if (edid_data_param->error)
		return;
	edid_data_param->po_edid_parsed->num_ext_blocks =
		edid_data_param->extensions;
	/* Extention Blocks */
	if (0 != edid_data_param->extensions) {

		uint8_t *p_data;
		/* check checksums of all blocks */

		for (block = 1; block <= edid_data_param->extensions; block++) {
			s_parse_chksum(edid_data_param, block);
			if (edid_data_param->error)
				return;
		}

		/* go to the next hdmi block */
		block = (1 == edid_data_param->extensions) ? (1) : (2);

		/* start of DTDs in the block */
		p_data = s_raw_ptr_get(edid_data_param, block,
			SII_LIB_EDID_EXT_ADR__D);
		start_addr_dtd = p_data[0];

		/* Check for available CEA Data Blocks */
		if (SII_LIB_EDID_EXT_ADR__DBC != start_addr_dtd) {
			s_cea_data_block_collection(edid_data_param, block);
			if (edid_data_param->error)
				return;
		}

		/* Check for available Detailed Decriptor Blocks */
		if (0 != start_addr_dtd) {
			/* Check Detailed Decriptor Blocks */
			while ((SII_LIB_EDID_ADR__CHECK_SUM -
				SII_LIB_EDID_LEN__DTD) >= start_addr_dtd) {
				s_parse_dtd_block(edid_data_param, block,
					start_addr_dtd);

				if (edid_data_param->error)
					return;

				start_addr_dtd += 18;
			}
		}
	}
}

enum sii_lib_edid_err_code_t sii_lib_edid_parse(
	struct sii_lib_edid_parsed_data *p_parsed_edid, uint8_t *raw_edid)
{
	struct edid_data_container_t edid_data;
	struct edid_data_container_t *p_edid_data = &edid_data;

	memset((void *)p_parsed_edid, 0,
		sizeof(struct sii_lib_edid_parsed_data));
	p_edid_data->po_edid_parsed = p_parsed_edid;
	p_edid_data->pi_edid_raw = raw_edid;
	p_edid_data->error = SII_LIB_EDID_ERR_CODE__NO_ERROR;

	s_parse_edid(p_edid_data);

#if PARSE_EDID_PRINT
	s_parsed_edid_print(p_parsed_edid);
#endif

	return p_edid_data->error;
}

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

