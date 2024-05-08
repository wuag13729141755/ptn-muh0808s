/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed “AS ISEWITHOUT ANY WARRANTY of any kind,
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

#include "Sii9630_si_lib_edid.h"
#include "Sii9630_si_lib_edid_video_tables.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_infoframe.h"
#include "Sii9630_si_drv_cp8630.h"

static uint8_t common_3d_pix_freq_mult_get(
	struct sii_lib_edid_parsed_data *parsed_ds_edid)
{

	uint8_t pix_clk_mult_3d = 1;
	uint16_t struct_3d_all = parsed_ds_edid->vsdb.struct_3d_all;

	if (struct_3d_all & 0x20)
		pix_clk_mult_3d = 4;
	else if (struct_3d_all & 0x1F)
		pix_clk_mult_3d = 2;

	return pix_clk_mult_3d;
}

static void check_indiv_3d_for_svd(struct sii_lib_edid_video_db_t *vid_db,
		uint8_t pos, uint32_t f_pix_max)
{
	/* check for multiple individual
	* 3D supported modes */

	uint32_t indiv_pix_freq = 0;
	uint8_t j = 0;
	const struct video_mode_type *video_timing;

	for (j = 0; j < SII_LIB_EDID__INDIV_3D_MODES_PER_VIC_MAX; j++) {

		video_timing =
			sii_vid_timing_from_vic(vid_db->svd[pos].vic, 0);

		if (NULL != video_timing)
			indiv_pix_freq = (video_timing->pix_clk_10khz)/100;

		if (vid_db->svd[pos].indiv_3d[j].b_indiv_3d_present) {
			switch (vid_db->svd[pos].indiv_3d[j].struct_3d) {
			case 0x01:
			case 0x02:
			case 0x04:
			case 0x08:
			case 0x10:
			case 0x40:
			case 0x80:
				indiv_pix_freq *= 2;
				break;

			case 0x20:
				indiv_pix_freq *= 4;
				break;

			default:
				break;
			}
		}

		if (indiv_pix_freq > f_pix_max)
			vid_db->svd[pos].indiv_3d[j].b_indiv_3d_present =
			false;
	}
}

static void invalidate_overclked_svds(
	struct sii_lib_edid_parsed_data *parsed_ds_edid,
	struct sii_lib_edid_config_device_t *config_struct)
{
	/* max supported pixel freq */
	uint32_t f_pix_max_mhz = config_struct->max_pix_clk / 100;

	/* multiplication factor to be applied, in case of supported 3D format*/
	uint32_t pix_freq_mult_3d = common_3d_pix_freq_mult_get(parsed_ds_edid);

	uint32_t format_pix_freq = 0;
	const struct video_mode_type *video_timing;

	struct sii_lib_edid_video_db_t *vid_db = &parsed_ds_edid->vdb;
	uint8_t i = 0;

	for (i = 0; i < vid_db->size; i++) {
		if (vid_db->svd[i].b_valid) {

			video_timing =
				sii_vid_timing_from_vic(vid_db->svd[i].vic, 0);

			if (NULL != video_timing)
				format_pix_freq = (video_timing->pix_clk_10khz)/100;

			/*
			* if the pixel frequency is greater than max supported
			* do not add this svd in EDID
			*/
			if (format_pix_freq > f_pix_max_mhz)
				vid_db->svd[i].b_valid = false;

			if ((vid_db->svd[i].b_3d_present) &&
				(vid_db->svd[i].b_valid)) {


				format_pix_freq *= pix_freq_mult_3d;

				/* if exceeds max supported frequency,
				* remove 3D capability for this format
				*/
				if (format_pix_freq > f_pix_max_mhz)
					vid_db->svd[i].b_3d_present = false;
			}

			if ((vid_db->svd[i].b_indiv_3d_present) &&
				(vid_db->svd[i].b_valid)) {
				check_indiv_3d_for_svd(vid_db, i,
					f_pix_max_mhz);
			}


		}
	}
}

static void invalidate_overclked_dtds(
	struct sii_lib_edid_parsed_data *parsed_ds_edid,
	struct sii_lib_edid_config_device_t *config_struct)
{
	/*
	 * if the pixel freq is greater than max supported,
	 * then do not add that dtd to EDID
	 */
	uint32_t f_pix_max = config_struct->max_pix_clk;
	uint8_t i = 0;

	for (i = 0; i < parsed_ds_edid->dtdb.size; i++) {
		if (parsed_ds_edid->dtdb.vid_timing[i].pixel_freq_10khz >
				f_pix_max)
			parsed_ds_edid->dtdb.vid_timing[i].b_valid = false;
	}
}

static void update_edid_config_struct(
	struct sii_lib_edid_parsed_data *dev_edid,
	struct sii_lib_edid_config_device_t *config_struct)
{
	config_struct->max_pix_clk = dev_edid->max_pixel_clock_10mhz * 10 * 100;
	config_struct->max_tmds_clk = dev_edid->max_tmds_5mhz * 5 * 100;
}

static void s_update_header(uint8_t *edid_blk0)
{
	uint8_t i = 1;

	for (i = 1; i < 7; i++)
		edid_blk0[i] = 0xFF;
}

static void s_place_dtd_in_edid(uint8_t *edid_blk,
				struct sii_lib_edid_video_timing_t *timing,
				uint8_t addr_in_blk)
{
	uint8_t flags = 0x00;
	uint8_t tmp_dtd[SII_LIB_EDID_LEN__DTD];

	memset(&tmp_dtd, 0, SII_LIB_EDID_LEN__DTD);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__PIX_CLK_LOW] =
		(uint8_t)timing->pixel_freq_10khz;
	tmp_dtd[SII_LIB_EDID_DTD_ADR__PIX_CLK_HIGH] =
		(uint8_t)(timing->pixel_freq_10khz >> 8);
	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_ACTIVE_LOW] =
		(uint8_t)(timing->h_active);
	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_BLANK_LOW] = (uint8_t)(timing->h_blank);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_HIGH] =
		((timing->h_active >> 4) & 0xF0) |
		((timing->h_blank >> 8) & 0x0F);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__V_ACTIVE_LOW] =
		(uint8_t)(timing->v_active);
	tmp_dtd[SII_LIB_EDID_DTD_ADR__V_BLANK_LOW] =
		(uint8_t)(timing->v_blank);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__V_HIGH] =
		((timing->v_active >> 4) & 0xF0) |
		((timing->v_blank >> 8) & 0x0F);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_SYNC_OFFSET_LOW] =
		(uint8_t)timing->h_sync_offset;
	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_SYNC_WIDTH_LOW] =
		(uint8_t)timing->h_sync_pw;

	tmp_dtd[SII_LIB_EDID_DTD_ADR__V_SYNC_LOW] =
		((timing->v_sync_offset << 4) & 0xF0) |
		((timing->v_sync_pw) & 0x0F);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_V_SYNC_HIGH] =
		((timing->h_sync_offset >> 2) & 0xC0) |
		((timing->h_sync_pw >> 4) & 0x30) |
		((timing->v_sync_offset >> 2) & 0x0C) |
		((timing->v_sync_pw >> 4) & 0x03);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_SIZE_LOW] =
		(uint8_t)timing->h_image_size;
	tmp_dtd[SII_LIB_EDID_DTD_ADR__V_SIZE_LOW] =
		(uint8_t)timing->v_image_size;

	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_V_SIZE_HIGH] =
		((timing->h_image_size >> 4) & 0xF0) |
	    ((timing->v_image_size >> 8) & 0x0F);

	tmp_dtd[SII_LIB_EDID_DTD_ADR__H_BORDER] = timing->h_border;
	tmp_dtd[SII_LIB_EDID_DTD_ADR__V_BORDER] = timing->v_border;

	flags = (timing->interface_type << 1) & 0x1E;

	if (timing->b_interlaced)
		flags |= 0x80;

	if (timing->display_type & 0x01)
		flags |= 0x01;
	if (timing->display_type & 0x02)
		flags |= 0x20;
	if (timing->display_type & 0x04)
		flags |= 0x40;

	tmp_dtd[SII_LIB_EDID_DTD_ADR__FLAGS] = flags;
	memcpy(&edid_blk[addr_in_blk], &tmp_dtd[0], SII_LIB_EDID_LEN__DTD);
}

static void s_compose_edid_blk0(
	struct sii_lib_edid_parsed_data *parsed_dev_edid,
	struct sii_lib_edid_parsed_data *parsed_ds_edid,
	uint8_t *out_edid_blk)
{
	uint8_t cntr = 0;
	uint8_t num_of_curr_dtds = 0;
	uint8_t addr_in_blk = SII_LIB_EDID_BL0_ADR__DTD1;

	memset(out_edid_blk, 0, SII_EDID_BLOCK_SIZE);

	/* Add EDID header */
	s_update_header(&out_edid_blk[SII_LIB_EDID_BL0_ADR__HEADER]);

	/* Add product and manufacture details */
	memcpy(&out_edid_blk[SII_LIB_EDID_BL0_ADR__MANUFACTURER_ID],
		&parsed_dev_edid->product_details[0],
		SII_LIB_EDID_BL0_LEN__PROD_DETAILS);

	/* Add EDID version and revision */
	out_edid_blk[SII_LIB_EDID_BL0_ADR__VERSION] =
		(uint8_t)(parsed_ds_edid->edid_version >> 8);
	out_edid_blk[SII_LIB_EDID_BL0_ADR__REVISION] =
		(uint8_t)parsed_ds_edid->edid_version;

	/* Add display parameters */
	memcpy(&out_edid_blk[SII_LIB_EDID_BL0_ADR__BASIC_DISPLAY_PARAMS],
		&parsed_ds_edid->basic_parameters[0],
		SII_LIB_EDID_BL0_LEN__BASIC_DISPLAY_PARAMS);

	/* Add chromaticity fields */
	memcpy(&out_edid_blk[SII_LIB_EDID_BL0_ADR__CHROMATICITY],
		&parsed_ds_edid->chromaticity[0],
		SII_LIB_EDID_BL0_LEN__CHROMATICITY);

	/* Add established timings */
	memcpy(&out_edid_blk[SII_LIB_EDID_BL0_ADR__ESTABLISHED_TIMING_1],
		&parsed_ds_edid->est_timings[0], 3);

	/* Add standard timings */
	memcpy(&out_edid_blk[SII_LIB_EDID_BL0_ADR__STANDARD_TIMING],
		&parsed_ds_edid->std_timings[0],
		SII_LIB_EDID_BL0_LEN__STANDARD_TIMING);

	/* Add the first two valid DTDs */
	for (cntr = 0; cntr < parsed_ds_edid->dtdb.size; cntr++) {

		if (num_of_curr_dtds == 2)
			break;

		if (parsed_ds_edid->dtdb.vid_timing[cntr].b_valid) {

			s_place_dtd_in_edid(out_edid_blk,
				&parsed_ds_edid->dtdb.vid_timing[cntr],
				addr_in_blk);

			addr_in_blk += 18;
			num_of_curr_dtds++;
		}
	}

	/* Add manufacture name DTD */
	out_edid_blk[addr_in_blk + SII_LIB_EDID_DTD_ADR__DATA_TYPE_TAG] =
		SII_LIB_EDID_DTD_TAG__NAME;
	memcpy(&out_edid_blk[addr_in_blk + SII_LIB_EDID_DTD_ADR__DATA],
		&parsed_dev_edid->monitor_name_str[0],
		SII_LIB_DTD_DESCRIPTOR_DATA_LENGTH);

	addr_in_blk += 18;

	/* Add monitor range limits DTD */
	out_edid_blk[addr_in_blk + SII_LIB_EDID_DTD_ADR__DATA_TYPE_TAG] =
		SII_LIB_EDID_DTD_TAG__RANGE_LIMITS;
	memcpy(&out_edid_blk[addr_in_blk + SII_LIB_EDID_DTD_ADR__DATA],
		&parsed_ds_edid->monitor_ranges[SII_LIB_EDID_DTD_ADR__DATA],
		SII_LIB_DTD_DESCRIPTOR_DATA_LENGTH);

}

static uint16_t s_get_3d_mask(struct sii_lib_edid_parsed_data *parsed_edid)
{
	uint16_t mask = 0x0000;
	uint16_t slider = 0x0001;
	uint8_t valid_vics_cnt = 0;
	uint8_t i = 0;

	for (i = 0; i < SII_LIB_EDID__SVD_MAX; i++) {
		/* only first 16 svds have 3d support */
		if (valid_vics_cnt > 15)
			break;

		/*
		 * if svd is valid and masked,
		 * update bit position in the mask
		 * to indicate svd mapping
		 */
		if (parsed_edid->vdb.svd[i].b_valid) {
			if (parsed_edid->vdb.svd[i].b_3d_present)
				mask |= slider;

			valid_vics_cnt++;
			slider <<= 1;
		}
	}

	return mask;
}

static void insert_indiv_3d_modes(
	struct sii_lib_edid_parsed_data *parsed_edid,
	uint8_t *out_edid_blk, uint8_t *curr_addr_in_edid,
	uint8_t pos, uint8_t *temp_3d_length)
{
	uint8_t j = 0;
	struct sii_lib_edid_indiv_3d_mode_t *ptr_data = NULL;

	for (j = 0; j < SII_LIB_EDID__INDIV_3D_MODES_PER_VIC_MAX; j++) {

		ptr_data = &parsed_edid->vdb.svd[pos].indiv_3d[j];

		if (ptr_data->b_indiv_3d_present) {

			out_edid_blk[*curr_addr_in_edid] = pos << 4;
			out_edid_blk[(*curr_addr_in_edid)++] |=
				ptr_data->struct_3d;
			(*temp_3d_length)++;

			if (ptr_data->struct_3d	> 7) {
				out_edid_blk[(*curr_addr_in_edid)++] =
					ptr_data->detail_3d;
				(*temp_3d_length)++;
			}
		}
	}
	ptr_data = NULL;
}
static uint8_t s_place_indiv_3d_modes(
	struct sii_lib_edid_parsed_data *parsed_edid, uint8_t *out_edid_blk,
	uint8_t curr_addr_in_edid, uint8_t *temp_3d_length)
{
	uint8_t valid_vics_cnt = 0;
	uint8_t i = 0;

	for (i = 0; i < SII_LIB_EDID__SVD_MAX; i++) {
		/* only first 16 svds have 3d support */
		if (valid_vics_cnt > 15)
			break;

		if (parsed_edid->vdb.svd[i].b_valid) {
			if (parsed_edid->vdb.svd[i].b_indiv_3d_present) {
				/*
				 * check if address is exceeding the limit
				 * to accomodate
				 * atleast one dtd
				 */
				if ((curr_addr_in_edid + 1) <
					SII_LIB_EDID_DBC_ADDR__MAX_OCCUPANCY) {

					insert_indiv_3d_modes(parsed_edid,
						out_edid_blk,
						&curr_addr_in_edid, i,
						temp_3d_length);
				}
			}
			valid_vics_cnt++;
		}
	}
	return curr_addr_in_edid;
}


/*static void s_create_additional_blocks
	(struct sii_lib_edid_parsed_data* parsed_edid,
	 uint8_t* out_edid_blk, uint8_t num_of_dtds_left)
{

}*/


static uint8_t s_get_curr_valid_dtds(
	struct sii_lib_edid_parsed_data *parsed_edid)
{
	uint8_t count = 0;
	uint8_t i = 0;

	for (i = 0; i < parsed_edid->dtdb.size; i++) {
		if (parsed_edid->dtdb.vid_timing[i].b_valid)
			count++;
	}
	return count;
}

static bool s_is_vic_present_for_dtd(
	struct sii_lib_edid_parsed_data *parsed_edid,
	struct sii_lib_edid_video_timing_t *timing)
{
	/*
	 * check if a valid vic is present in Video Data Block for this dtd
	 * timing. If present do not add the dtd to EDID
	 */
	uint8_t i = 0;
	uint8_t vic1 = 0x00;
	uint8_t vic2 = 0x00;
	const struct video_mode_type *video_timing;

	for (; i < 108; i++) {

		video_timing = sii_vid_timing_from_vic(i, 0);

		if (NULL == video_timing) {
			return false;
			;
		}

		if (timing->h_active == video_timing->active.H) {
			if (timing->v_active == video_timing->active.V) {
				if (timing->h_blank == video_timing->blank.H) {
					if (timing->pixel_freq_10khz ==
						video_timing->pix_clk_10khz) {

						vic1 = video_timing->vic_16x9;
						vic2 = video_timing->vic_4x3;
						break;
					}
				}
			}
		}
	}

	for (i = 0; i < parsed_edid->vdb.size; i++) {
		if (parsed_edid->vdb.svd[i].b_valid) {
			if ((vic1 == parsed_edid->vdb.svd[i].vic) |
				(vic2 == parsed_edid->vdb.svd[i].vic)) {
				return true;
			}
		}
	}
	return false;
}

static uint8_t s_get_num_svds_with_low_freq(
	struct sii_lib_edid_video_db_t *vid_db)
{
	uint8_t count = 0;
	uint8_t i = 0;
	uint16_t pix_freq = 0x0000;
	const struct video_mode_type *video_timing;

	for (; i < vid_db->size; i++) {
		/* if it is valid and not native */
		if (vid_db->svd[i].b_valid && !vid_db->svd[i].b_native) {

			video_timing =
				sii_vid_timing_from_vic(vid_db->svd[i].vic, 0);
			if (NULL != video_timing) {
				pix_freq = (video_timing->pix_clk_10khz)/100;
				if (pix_freq < 7500) {
					count++;
					;
				}
			}
		}
	}
	return count;
}

static uint8_t s_get_len_of_db(uint8_t *edid, uint8_t db_tag)
{
	uint16_t addr = SII_EDID_BLOCK_SIZE + SII_LIB_EDID_EXT_ADR__DBC;
	uint8_t ret_val = 0;

	for (; addr < SII_EDID_BLOCK_SIZE + SII_LIB_EDID_ADR__CHECK_SUM;
		addr++) {
		if (((edid[addr] & 0xE0) >> 5) == db_tag)
			ret_val = (edid[addr] & 0x1F);
		else
			addr += (edid[addr] & 0x1F);
		addr++;
	}
	return ret_val;
}

static int8_t s_get_addr_of_db(uint8_t *edid, uint8_t db_tag)
{
	uint16_t addr = SII_EDID_BLOCK_SIZE + SII_LIB_EDID_EXT_ADR__DBC;
	int8_t ret_val = -1;

	for (; addr < SII_EDID_BLOCK_SIZE + SII_LIB_EDID_ADR__CHECK_SUM;
		addr++) {
		if (((edid[addr] & 0xE0) >> 5) == db_tag)
			ret_val = addr;
		else
			addr += (edid[addr] & 0x1F);
		addr++;
	}
	return ret_val;
}

static uint8_t s_prune_svd_list(uint8_t *out_edid_blk)
{
	uint16_t pix_freq = 0x0000;
	uint8_t start_of_dtd = out_edid_blk[SII_EDID_BLOCK_SIZE +
					 SII_LIB_EDID_EXT_ADR__D];
	uint8_t len = s_get_len_of_db(out_edid_blk,
		SII_LIB_EDID_DBC_TAG__VIDEO);
	int8_t addr = s_get_addr_of_db(out_edid_blk,
		SII_LIB_EDID_DBC_TAG__VIDEO);

	const struct video_mode_type *video_timing;

	if (addr < 0)
		return start_of_dtd;

	addr += 17; /* since first 16 are mandatory for 3d support indication */
	for (; addr < len; addr++) {
		/* do not prune if it is native */
		if ((out_edid_blk[addr] < 198) && (out_edid_blk[addr] > 128))
			continue;

		video_timing =
			sii_vid_timing_from_vic(out_edid_blk[addr], 0);

		if (NULL != video_timing) {
			pix_freq = (video_timing->pix_clk_10khz)/100;
			/* less than 75 MHz */
			if (pix_freq < 7500) {
				memmove(&out_edid_blk[addr],
					&out_edid_blk[addr + 1],
					start_of_dtd - addr);
				start_of_dtd--;
			}
		}
	}
	return start_of_dtd;
}

static void s_filter_out_data_blocks(
	struct sii_lib_edid_parsed_data *parsed_edid, uint8_t *out_edid_blk,
	uint8_t num_of_dtds_left, uint8_t num_of_dtds_placed)
{
	uint8_t i = 0;
	uint8_t start_of_dtd =
		out_edid_blk[SII_EDID_BLOCK_SIZE + SII_LIB_EDID_EXT_ADR__D];
	uint8_t curr_addr_in_edid = start_of_dtd;
	uint8_t bytes_available = 0x7F - start_of_dtd -
		(num_of_dtds_placed * SII_LIB_EDID_LEN__DTD);
	struct sii_lib_edid_dtd_t *p_dtdb =  &(parsed_edid->dtdb);

	for (; i < p_dtdb->size; i++) {
		if (p_dtdb->vid_timing[i].b_valid) {

			if (s_is_vic_present_for_dtd(parsed_edid,
				&p_dtdb->vid_timing[i])) {
				/*
				 * if it is already present in video data block,
				 * ignore the dtd
				 */
				p_dtdb->vid_timing[i].b_valid = false;
			}

			if (p_dtdb->vid_timing[i].pixel_freq_10khz < 7000) {
				/* ignore the dtd with low frequency */
				p_dtdb->vid_timing[i].b_valid = false;
			}
		}
	}

	/*
	 * if the dtds to be placed are still more in number
	 * and still cannot be accomodated
	 */
	if (s_get_curr_valid_dtds(parsed_edid) >
			num_of_dtds_left + num_of_dtds_placed) {

		uint8_t cntr = 2;
		uint8_t num_of_dtds_in_blk1 = 0x00;
		uint8_t num_svds_low_freq = s_get_num_svds_with_low_freq(
			&parsed_edid->vdb);

		bytes_available += num_svds_low_freq;
		/*
		 * if we can reduce the number of svds,
		 * so that dtd can fit then prune the svd list
		 */
		if (bytes_available > SII_LIB_EDID_LEN__DTD)
			start_of_dtd = s_prune_svd_list(out_edid_blk);


		/* shift the start of dtd byte by number of svds pruned */
		out_edid_blk[SII_EDID_BLOCK_SIZE + SII_LIB_EDID_EXT_ADR__D] =
			start_of_dtd;

		/* start placing dtds again overwriting any previousl
		 * placed dtds */
		curr_addr_in_edid = 128 + start_of_dtd;
		while (SII_LIB_EDID_LEN__DTD < (0xFF - curr_addr_in_edid)) {

			if (cntr < p_dtdb->size) {
				if (p_dtdb->vid_timing[num_of_dtds_in_blk1 + 2].b_valid) {

					s_place_dtd_in_edid(out_edid_blk,
						&p_dtdb->vid_timing[
							num_of_dtds_in_blk1
							+ 2],
						curr_addr_in_edid);

					curr_addr_in_edid += 18;
					num_of_dtds_in_blk1++;
				}
			} else
				break;
			/* break after iterating through all the available
			 * dtds */

			cntr++;
		}

	}

}

static void s_compose_hdmi_ext_blks(
	struct sii_lib_edid_parsed_data *parsed_dev_edid,
	struct sii_lib_edid_parsed_data *parsed_edid, uint8_t *out_edid_blk)
{
	uint16_t new_3d_mask = 0x0000;
	uint8_t addr_in_edid_blk1 = SII_EDID_BLOCK_SIZE;
	uint8_t num_of_valid_svds = 0;
	uint8_t num_of_valid_dtds = 0;
	uint8_t num_of_dtds_in_blk1 = 0;
	uint8_t tmp_addr = 0;
	uint8_t addr_of_lipsync_field = 0;
	uint8_t cntr = 0;
	struct sii_lib_edid_dtd_t *p_dtdb = &(parsed_edid->dtdb);

	memset(&out_edid_blk[addr_in_edid_blk1], 0, SII_EDID_BLOCK_SIZE);

	/* Check and return if there are no extension blocks */
	if (!parsed_edid->num_ext_blocks)
		return;

	out_edid_blk[addr_in_edid_blk1 + SII_LIB_EDID_EXT_ADR__TAG] = 2;
	out_edid_blk[addr_in_edid_blk1 + SII_LIB_EDID_EXT_ADR__REVISION] = 3;
	out_edid_blk[addr_in_edid_blk1 + SII_LIB_EDID_EXT_ADR__DTD_OPT] |=
		(parsed_edid->b_it_underscan << 7) |
			(parsed_edid->b_ycc444 << 5) |
			(parsed_edid->b_ycc422 << 4);

	addr_in_edid_blk1 += SII_LIB_EDID_EXT_ADR__DBC;

#if VIDEO_DATA_BLOCK
#ifdef REMOVE_4096x2160_VIC
	/*Loop to invalidate 4096 VICS*/
	for (cntr = 0; cntr < parsed_edid->vdb.size; cntr++) {

		if((parsed_edid->vdb.svd[cntr].vic >= _4096_VIC_98)
				&& (parsed_edid->vdb.svd[cntr].vic <= _4096_VIC_102)){

			parsed_edid->vdb.svd[cntr].b_valid = false;
		}

	}
#endif

	for (cntr = 0; cntr < parsed_edid->vdb.size; cntr++) {
		/* iterate through all the svds */
		if (parsed_edid->vdb.svd[cntr].b_valid) {
			/* place only the valid svds */
			out_edid_blk[addr_in_edid_blk1 + 1 + num_of_valid_svds] =
				parsed_edid->vdb.svd[cntr].vic;
			if (parsed_edid->vdb.svd[cntr].b_native)
				out_edid_blk[addr_in_edid_blk1 + 1 + num_of_valid_svds] |=
					0x80;

			num_of_valid_svds++;
		}

	}

	out_edid_blk[addr_in_edid_blk1] = (SII_LIB_EDID_DBC_TAG__VIDEO << 5)
		| num_of_valid_svds;

	addr_in_edid_blk1 += num_of_valid_svds + 1;
#endif /* VIDEO_DATA_BLOCK */

#if AUDIO_DATA_BLOCK
	if (parsed_edid->b_basic_audio) {
		/* indicate basic audio support */
		out_edid_blk[SII_EDID_BLOCK_SIZE +
			SII_LIB_EDID_EXT_ADR__DTD_OPT] |= 0x40;

		tmp_addr = addr_in_edid_blk1 + 1;

		for (cntr = 0; cntr < parsed_edid->adb.size; cntr++) {
			/* copy sads one by one */
			memcpy(&out_edid_blk[tmp_addr],
				&parsed_edid->adb.sad[cntr].audio_data[0],
				SII_LIB_EDID_SAD__LENGTH);

			tmp_addr += SII_LIB_EDID_SAD__LENGTH;
		}
		out_edid_blk[addr_in_edid_blk1] =
			(SII_LIB_EDID_DBC_TAG__AUDIO << 5) |
			(parsed_edid->adb.size * SII_LIB_EDID_SAD__LENGTH);

		addr_in_edid_blk1 = tmp_addr;

		if (parsed_edid->spkdb.b_present) {
			/* add a speaker allocation data block if
			 * audio block is present */
			out_edid_blk[addr_in_edid_blk1] =
				(SII_LIB_EDID_DBC_TAG__SPEAKER_ALLOCATION <<
				 5) | SII_LIB_EDID_SPK__LENGTH;

			addr_in_edid_blk1++;
			memcpy(&out_edid_blk[addr_in_edid_blk1],
				&parsed_edid->spkdb.spk_alloc[0],
				SII_LIB_EDID_SPK__LENGTH);

			addr_in_edid_blk1 += SII_LIB_EDID_SPK__LENGTH;
		}
	}
#endif /* AUDIO_DATA_BLOCK */

#if VENDOR_SPECIFIC_DATA_BLOCK /* Contains both 1.4 and hfvsdb blocks */

	tmp_addr = addr_in_edid_blk1;
	tmp_addr++;
	if (parsed_edid->is_vsdb_present) {

		uint8_t temp_3d_lengh = 0;
		uint8_t temp1_addr = 0;
		out_edid_blk[tmp_addr++] =
			(uint8_t)(SII_LIB_EDID_VSB_DAT__HDMI_SIGNATURE & 0xFF);
		out_edid_blk[tmp_addr++] =
			(uint8_t)(SII_LIB_EDID_VSB_DAT__HDMI_SIGNATURE >> 8);
		out_edid_blk[tmp_addr++] =
			(uint8_t)(SII_LIB_EDID_VSB_DAT__HDMI_SIGNATURE >> 16);

		out_edid_blk[tmp_addr++] =
			(parsed_edid->vsdb.cec_addr[0] << 4) |
			(parsed_edid->vsdb.cec_addr[1] << 0);
		out_edid_blk[tmp_addr++] =
			(parsed_edid->vsdb.cec_addr[2] << 4) |
			(parsed_edid->vsdb.cec_addr[3] << 0);

		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.b_supports_ai << 7;
		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.b_y444_dc16 << 6;
		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.b_y444_dc12 << 5;
		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.b_y444_dc10 << 4;
		out_edid_blk[tmp_addr++] |= parsed_edid->vsdb.b_y444 << 3;

		out_edid_blk[tmp_addr++] =
			(uint8_t)(parsed_edid->max_tmds_5mhz);
		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.content_type;

		addr_of_lipsync_field = tmp_addr;
		tmp_addr++;

		/* latency fields */
		if (parsed_edid->vsdb.lipsync.b_latency_present) {
			out_edid_blk[addr_of_lipsync_field] |= 0x80;
			out_edid_blk[tmp_addr++] = parsed_edid->vsdb.lipsync.video_latency;
			out_edid_blk[tmp_addr++] = parsed_edid->vsdb.lipsync.audio_latency;
		}

		/* interlaced latency fields */
		if (parsed_edid->vsdb.lipsync.b_i_latency_present) {
			out_edid_blk[addr_of_lipsync_field] |= 0x40;
			out_edid_blk[tmp_addr++] = parsed_edid->vsdb.lipsync.i_video_latency;
			out_edid_blk[tmp_addr++] = parsed_edid->vsdb.lipsync.i_audio_latency;
		}

		/* indicate HDMI_Video_present field */
		out_edid_blk[addr_of_lipsync_field] |= 0x20;

		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.b_3d_present << 7;
		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.multi_3d_type << 5;
		out_edid_blk[tmp_addr++] |= parsed_edid->vsdb.image_size << 3;

		out_edid_blk[tmp_addr] |= parsed_edid->vsdb.hdmi_vic_len << 5;
		/*out_edid_blk[tmp_addr++] |= parsed_edid->vsdb.hdmi_3d_len;*/

		temp1_addr = tmp_addr++;

		/* place all the hdmi vics */
		if (parsed_edid->vsdb.hdmi_vic_len > 0) {
			uint8_t i = 0;

			for (i = parsed_edid->vsdb.hdmi_vic_len; i > 0; i--)
				out_edid_blk[tmp_addr++] =
					parsed_edid->vsdb.hdmi_vic[i - 1];
		}

		/* fill the 3D struct and 3D mask fields */
		if ((parsed_edid->vsdb.multi_3d_type == 0x01) |
			(parsed_edid->vsdb.multi_3d_type == 0x02)) {

			out_edid_blk[tmp_addr++] =
				parsed_edid->vsdb.struct_3d_all >> 8;
			out_edid_blk[tmp_addr++] =
				(uint8_t)parsed_edid->vsdb.struct_3d_all;
			temp_3d_lengh += 2;
		}

		if (parsed_edid->vsdb.multi_3d_type == 0x02) {
			/* generate new 3D mask according to the
			 now valid SVDs with 3D support */
			new_3d_mask = s_get_3d_mask(parsed_edid);
			out_edid_blk[tmp_addr++] = new_3d_mask >> 8;
			out_edid_blk[tmp_addr++] = (uint8_t)new_3d_mask;
			temp_3d_lengh += 2;
		}
		/* place the svds mapping indicating individual 3d modes*/
		tmp_addr = s_place_indiv_3d_modes(parsed_edid, out_edid_blk,
					tmp_addr, &temp_3d_lengh);


		out_edid_blk[temp1_addr] |= temp_3d_lengh;
		out_edid_blk[addr_in_edid_blk1] =
			(SII_LIB_EDID_DBC_TAG__VENDOR_SPECIFIC << 5) |
			(tmp_addr - 1 - addr_in_edid_blk1);
	}
#if HF_VSDB
	/* add a hdmi forum vsdb */
	if (parsed_edid->is_hfvsdb_present) {
		out_edid_blk[tmp_addr++] =
			(SII_LIB_EDID_DBC_TAG__VENDOR_SPECIFIC << 5) | 0x07;
		out_edid_blk[tmp_addr++] =
			SII_LIB_EDID_VSB_DAT__HDMI_FORUM_SIGNATURE & 0xFF;
		out_edid_blk[tmp_addr++] =
			(SII_LIB_EDID_VSB_DAT__HDMI_FORUM_SIGNATURE >> 8) &
				0xFF;
		out_edid_blk[tmp_addr++] =
			(SII_LIB_EDID_VSB_DAT__HDMI_FORUM_SIGNATURE >> 16) &
				0xFF;
		out_edid_blk[tmp_addr++] = 0x01; /* version */

		out_edid_blk[tmp_addr++] =
			(uint8_t)(((parsed_dev_edid->hfvsdb.vclk_mb <
				parsed_edid->hfvsdb.vclk_mb) ?
				parsed_dev_edid->hfvsdb.vclk_mb :
				parsed_edid->hfvsdb.vclk_mb)/5);

		/*SCDCs capabilities (b_scdc_present and b_read_req_capable)
		will be as per Banner capability not based on DS*/
		out_edid_blk[tmp_addr++] =
			(uint8_t)((parsed_dev_edid->hfvsdb.b_scdc_present << 7) |
			(parsed_dev_edid->hfvsdb.b_read_req_capable << 6) |

			((parsed_dev_edid->hfvsdb.b_lte_340mscs_scramble &
			parsed_edid->hfvsdb.b_lte_340mscs_scramble) << 3) |
			((parsed_dev_edid->hfvsdb.b_independent_view &
				parsed_edid->hfvsdb.b_independent_view) << 2) |
			((parsed_dev_edid->hfvsdb.b_dual_view &
				parsed_edid->hfvsdb.b_dual_view) << 1) |
			((parsed_dev_edid->hfvsdb.b_3d_osd_disparity &
				parsed_edid->hfvsdb.b_3d_osd_disparity) << 0));

		out_edid_blk[tmp_addr++] =
			(uint8_t)(((parsed_dev_edid->hfvsdb.b_dc48bit420 &
				parsed_edid->hfvsdb.b_dc48bit420) << 2) |
				((parsed_dev_edid->hfvsdb.b_dc36bit420 &
				parsed_edid->hfvsdb.b_dc36bit420) << 1) |
				((parsed_dev_edid->hfvsdb.b_dc30bit420 &
				parsed_edid->hfvsdb.b_dc30bit420) << 0));

		/*addr_in_edid_blk1 = tmp_addr;*/
	}
#endif /* HF_VSDB */
	addr_in_edid_blk1 = tmp_addr;
#endif /* VENDOR_SPECIFIC_DATA_BLOCK */

#if EXTENDED_BLOCKS

	if (parsed_edid->vid_cap_db.b_present) {
		/* Video Capability Data Block */
		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) | (0x02);
		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__MISC_VIDEO_FIELDS;
		out_edid_blk[addr_in_edid_blk1++] =
			parsed_edid->vid_cap_db.scan_info;
	}

	if (parsed_edid->vs_vid_db.b_present) {
		/* Vendor Specific Video */
		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
			(parsed_edid->vs_vid_db.length + 4);
		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__VENDOR_SPECIFIC_VIDEO;

		out_edid_blk[addr_in_edid_blk1++] =
			(uint8_t)parsed_edid->vs_vid_db.ieee_oui;
		out_edid_blk[addr_in_edid_blk1++] =
			(uint8_t)(parsed_edid->vs_vid_db.ieee_oui >> 8);
		out_edid_blk[addr_in_edid_blk1++] =
			(uint8_t)(parsed_edid->vs_vid_db.ieee_oui >> 16);

		memcpy(&out_edid_blk[addr_in_edid_blk1],
			   &parsed_edid->vs_vid_db.ptr_array[0],
			   parsed_edid->vs_vid_db.length);

		addr_in_edid_blk1 += parsed_edid->vs_vid_db.length;
	}

	if (parsed_edid->vs_aud_db.b_present) {
		/* Vendor Specific Audio */
		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
			(parsed_edid->vs_aud_db.length + 4);
		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__VENDOR_SPECIFIC_AUDIO;

		out_edid_blk[addr_in_edid_blk1++] =
			(uint8_t)parsed_edid->vs_vid_db.ieee_oui;
		out_edid_blk[addr_in_edid_blk1++] =
			(uint8_t)(parsed_edid->vs_vid_db.ieee_oui >> 8);
		out_edid_blk[addr_in_edid_blk1++] =
			(uint8_t)(parsed_edid->vs_vid_db.ieee_oui >> 16);

		memcpy(&out_edid_blk[addr_in_edid_blk1],
			   &parsed_edid->vs_aud_db.ptr_array[0],
			   parsed_edid->vs_aud_db.length);

		addr_in_edid_blk1 += parsed_edid->vs_aud_db.length;
	}

	if (parsed_edid->calorimetry_db.calorimetry_present) {
		/* Calorimetry Data Block */
		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) | (0x03);
		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__CALORIMETRY;
		out_edid_blk[addr_in_edid_blk1++] =
			parsed_edid->calorimetry_db.calorimetry_byte;

		out_edid_blk[addr_in_edid_blk1++] = 0x00; /* reserved */
	}

	/* 420 support */
	if (parsed_edid->ycc_420_vid_db.length) {

		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
			(parsed_edid->ycc_420_vid_db.length + 1);

		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__YCC420_VIDEO;

		memcpy(&out_edid_blk[addr_in_edid_blk1],
			&parsed_edid->ycc_420_vid_db.svd_data[0],
			parsed_edid->ycc_420_vid_db.length);

		addr_in_edid_blk1 += parsed_edid->ycc_420_vid_db.length;

	} else {
		uint8_t i ,j;
		bool counter = false;

		/* check if any of the vics are 420 mapped */
		for (i = 0; i < SII_LIB_EDID__SVD_MAX; i++) {
			if (parsed_edid->vdb.svd[i].b_420_mapped) {
				counter = true;
				break;
			}
		}

		if (counter) {
			tmp_addr = addr_in_edid_blk1;
			addr_in_edid_blk1++;
			addr_in_edid_blk1++;
#ifdef REMOVE_4096x2160_VIC
			/*Check if len of mapping block is 0,
			 *If TRUE, means all the svds support 420*/
			if (parsed_edid->_420_cap_map_data_blk_len != 0) {
				uint8_t num_svd = parsed_edid->_420_cap_map_data_blk_len * 8;

				uint8_t svd_bit_pos = 0;
				for(i = 0; i < num_svd; i += 8)
				{
					for (j = 0; j < 8; j++) {

						if(i+j == SII_LIB_EDID__SVD_MAX)
							break;
						if(parsed_edid->vdb.svd[i+j].b_valid){
							out_edid_blk[addr_in_edid_blk1] |=
							(parsed_edid->vdb.svd[i+j].b_420_mapped << svd_bit_pos);
							svd_bit_pos++;
							if(svd_bit_pos%8 == 0){
								svd_bit_pos = 0;
								addr_in_edid_blk1++;
							}
						}
					}
				}
				if(svd_bit_pos%8)
					addr_in_edid_blk1++;

			}
			out_edid_blk[tmp_addr] =
				(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
				(addr_in_edid_blk1-tmp_addr-1);
			out_edid_blk[tmp_addr + 1] =
				SII_LIB_EDID_DBC_ETG__YCC420_CAPABILITY_MAP;
#else
			/*Check if len of mapping block is 0,
			 *If TRUE, means all the svds support 420*/
			if (parsed_edid->_420_cap_map_data_blk_len != 0) {
				uint8_t num_svd = parsed_edid->_420_cap_map_data_blk_len * 8;
				for(i = 0; i < num_svd; i += 8)
				{
					for (j = 0; j < 8; j++) {

						if(i+j == SII_LIB_EDID__SVD_MAX)
							break;

						out_edid_blk[addr_in_edid_blk1] |=
						(parsed_edid->vdb.svd[i+j].b_420_mapped << j);
					}
					addr_in_edid_blk1++;
				}
			}
			out_edid_blk[tmp_addr] =
				(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
				(parsed_edid->_420_cap_map_data_blk_len + 1);
			out_edid_blk[tmp_addr + 1] =
				SII_LIB_EDID_DBC_ETG__YCC420_CAPABILITY_MAP;
#endif
		}

	}

	if (parsed_edid->hdr_stat_md_db.length) {
		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
			(parsed_edid->hdr_stat_md_db.length + 1);

		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__HDR_STATIC_MD;

		memcpy(&out_edid_blk[addr_in_edid_blk1],
			&parsed_edid->hdr_stat_md_db.hdr_stat_md[0],
			   parsed_edid->hdr_stat_md_db.length);

		addr_in_edid_blk1 += parsed_edid->hdr_stat_md_db.length;
	}

	if (parsed_edid->hdr_dyn_md_db.length) {
		out_edid_blk[addr_in_edid_blk1++] =
			(SII_LIB_EDID_DBC_TAG__USE_EXTENDED_TAG << 5) |
			(parsed_edid->hdr_dyn_md_db.length + 1);

		out_edid_blk[addr_in_edid_blk1++] =
			SII_LIB_EDID_DBC_ETG__HDR_DYNAMIC_MD;

		memcpy(&out_edid_blk[addr_in_edid_blk1],
			&parsed_edid->hdr_dyn_md_db.hdr_dyn_md[0],
			   parsed_edid->hdr_dyn_md_db.length);

		addr_in_edid_blk1 += parsed_edid->hdr_dyn_md_db.length;
	}
#endif /* EXTENDED_BLOCKS */

	/* indicate the start of dtd */
	out_edid_blk[SII_EDID_BLOCK_SIZE + SII_LIB_EDID_EXT_ADR__D] =
		addr_in_edid_blk1 - SII_EDID_BLOCK_SIZE;

	cntr = 2; /* since 2 dtds are already placed in block 0 */
	while (SII_LIB_EDID_LEN__DTD < (0xFF - addr_in_edid_blk1)) {

		if (cntr < parsed_edid->dtdb.size) {
			if (p_dtdb->vid_timing[num_of_dtds_in_blk1 + 2].b_valid) {

				/* place remaining valid DTDs in block 1*/
				s_place_dtd_in_edid(out_edid_blk,
					&parsed_edid->dtdb.vid_timing[
						num_of_dtds_in_blk1 + 2],
					addr_in_edid_blk1);

				addr_in_edid_blk1 += 18;
				num_of_dtds_in_blk1++;
			}
		} else
			break;
		/* break after iterating through all the available dtds */

		cntr++;
	}

	/* TODO : no of native dtds in block */
	out_edid_blk[SII_EDID_BLOCK_SIZE + SII_LIB_EDID_EXT_ADR__DTD_OPT] |=
		parsed_edid->native_dtd_cnt;

	num_of_valid_dtds = s_get_curr_valid_dtds(parsed_edid);

	out_edid_blk[SII_LIB_EDID_BL0_ADR__EXTENSION_NMB] = 0x01;

	/* if there are more dtds to be accomodated */
	if (num_of_dtds_in_blk1 + 2 < num_of_valid_dtds) {

#ifdef FOUR_BLOCK_EDID_SUPPORTED
		/*s_create_additional_blocks(parsed_edid, out_edid_blk,
		 parsed_edid->dtdb.size - num_of_dtds_in_blk1 - 2);*/
#else
		s_filter_out_data_blocks(parsed_edid, out_edid_blk,
			num_of_valid_dtds - num_of_dtds_in_blk1 - 2,
			num_of_dtds_in_blk1);
#endif
	}
}

static void s_calculate_checksums(uint8_t *ptr_edid)
{
	uint16_t i = 0;
	uint8_t sum = 0;

	for (i = 0; i < (SII_EDID_BLOCK_SIZE - 1); i++)
		sum += ptr_edid[i];
	ptr_edid[i] = (~sum) + 1;

	if (ptr_edid[SII_EDID_BLOCK_SIZE - 2]) {
		sum = 0;
		for (i = SII_EDID_BLOCK_SIZE;
			i < (2 * SII_EDID_BLOCK_SIZE - 1);
			i++)
			sum += ptr_edid[i];
		ptr_edid[i] = (~sum) + 1;
	}

#ifdef FOUR_BLOCK_EDID_SUPPORTED
	sum = 0;
	for (; i < (3 * SII_EDID_BLOCK_SIZE); i++)
		sum += ptr_edid[i];
	ptr_edid[i - 1] = (~sum) + 1;

	sum = 0;
	for (; i < (4 * SII_EDID_BLOCK_SIZE); i++)
		sum += ptr_edid[i];
	ptr_edid[i - 1] = (~sum) + 1;
#endif
}

uint8_t sii_lib_edid_compose(
	struct sii_lib_edid_parsed_data *pi_parsed_dev_edid,
	struct sii_lib_edid_parsed_data *pi_parsed_ds_edid,
	uint8_t *po_composed_edid)
{

	struct sii_lib_edid_config_device_t config_struct;
	struct sii_lib_edid_config_device_t *ptr_config = &config_struct;

	memset(&config_struct, 0, sizeof(config_struct));
	memset(po_composed_edid, 0, sizeof(config_struct));

	/*
	* if the first argument is device edid, the composed EDID will have the
	* product details of device and capabilities of downstream are filtered
	* based on clock settings of device edid
	*/
	update_edid_config_struct(pi_parsed_dev_edid, ptr_config);

	invalidate_overclked_svds(pi_parsed_ds_edid, ptr_config);
	invalidate_overclked_dtds(pi_parsed_ds_edid, ptr_config);

	s_compose_edid_blk0(pi_parsed_dev_edid, pi_parsed_ds_edid,
		po_composed_edid);
	s_compose_hdmi_ext_blks(pi_parsed_dev_edid, pi_parsed_ds_edid,
		po_composed_edid);

	s_calculate_checksums(po_composed_edid);

	return 0;
}

#endif //#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
