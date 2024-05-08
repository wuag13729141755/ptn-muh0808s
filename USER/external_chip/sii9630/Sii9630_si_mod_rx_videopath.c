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

#include "Sii9630_si_datatypes.h"

#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_mod_rx_videopath.h"
#include "Sii9630_si_mod_rx_videopath_regs.h"

#define SELECT_VIDEO_PATH_PAGE(x) sii_platform_wr_reg8(pstDev, 0x1315, x)

#define BIT_MASK__VP__CMS_CSC0_420_422_CONFIG__SIZE 1
#define BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE 1
#define BIT_MASK__VP__CMS_CSC0_MULTI_CSC_CONFIG__SIZE 2
#define BIT_MASK__VP__CMS__CSC0__DITHERING_CONFIG__SIZE 1
#define BIT_MASK__VP__CMS_CSC1_444_422_CONFIG__SIZE 1
#define BIT_MASK__VP__INPUT_FORMAT__SIZE 2
#define BIT_MASK__VP__INPUT_SYNC_ADJUST_CONFIG__SIZE 1
#define BIT_MASK__VP__OUTPUT_SYNC_CONFIG__SIZE	1
#define BIT_MASK__VP__DEGEN_CONFIG__SIZE 1
#define BIT_MASK__VP__DEGEN_PIXEL_DELAY__SIZE 2
#define BIT_MASK__VP__DEGEN_LINE_DELAY__SIZE 2
#define BIT_MASK__VP__OUTPUT_FORMAT__SIZE 2
#define BIT_MASK__VP__OUTPUT_MUTE__SIZE 1
#define BIT_MASK__VP__FDET_STATUS__SIZE 1
#define BIT_MASK__VP__FDET_IRQ_STAT__SIZE 3
#define BIT_MASK__VP__FDET_PIXEL_COUNT__SIZE 2
#define BIT_MASK__VP__FDET_LINE_COUNT__SIZE 2
#define BIT_MASK__VP__SOFT_RESET__SIZE 1
#define BIT_MASK__VP__FDET_FRAMERATE_DELTA_THRESHOLD__SIZE 3
#define BIT_MASK__VP__INPUT_PIN_MAPPING 2

#define PAGE_CORE 0
#define PAGE_CMS  1
#define PAGE_CSC  2

#define VP__INPUT_MAPPING__SELECT_Y 3
#define VP__INPUT_MAPPING__SELECT_CB 6
#define VP__INPUT_MAPPING__SELECT_CR 9

enum color_space {
	RGB = 0,
	y_cb_cr422,
	y_cb_cr444,
	y_cb_cr420
};

enum quantization_level {
	/* 0 - 255*/
	QUANTIZATION_VIDEO_LEVELS,
	/* 16 - 235*/
	QUANTIZATION_PC_LEVELS
};

struct rate_convert_cfg {
	uint8_t mux_420_enable;
};

struct embedde_sync_cfg {
	uint8_t tmp;
};

struct video_balnk_cfg {
	uint8_t tmp;
};

struct sampler_cfg {
	enum color_space input_clr_spc;
	enum color_space output_clr_spc;
};

struct csc_config {
	enum color_space input_clr_spc;
	enum color_space output_clr_spc;
	enum quantization_level in_quantization;
	enum quantization_level out_quantization;
	enum sii_drv_conv_std in_conv_std;
	enum sii_drv_conv_std out_conv_std;
};

struct data_path_mux_cfg {
	enum avi_color_space color_space;
};

enum sync_pol {
	SYNCPOL_NEGATIVE, SYNCPOL_POSITIVE
};

struct sync_pol_adjust_cfg {
	uint8_t b_auto_adjust;
	enum sync_pol hsync_pol;
	enum sync_pol vsync_pol;
};

struct video_color_info {
	enum sii_drv_conv_std conv_std;
	enum color_space clr_spc;
	enum sii_drv_bit_depth vid_dc_depth;
	enum quantization_level quntization;
};

enum dither_cfg {
	DITHER__12_TO_10, DITHER__12_TO_8, DITHER__10_TO_8, DITHER__NOCHNG,
};

struct cms_cfg {
	uint8_t b_enable_420422;
	uint8_t b_enable_422444;
	uint8_t b_enable_csc0;
	uint8_t b_enable_444422;
	uint8_t b_enable_dithering;
	struct video_color_info in_clr_info;
	struct video_color_info out_clr_info;
};

struct video_path_config {
	uint8_t b_enable_idp_mux;
	uint8_t b_enable_irc;
	uint8_t b_enable_in_spa;
	uint8_t b_enable_cms;
	uint8_t b_enable_vb;
	uint8_t b_enable_orc;
	uint8_t b_enable_ese;
	uint8_t b_enable_out_spa;
	uint8_t b_enable_odp_mux;
	uint8_t b_enable_dithering;

	struct video_color_info out_clr_info;
	struct video_color_info in_clr_info;
	enum sii_hv_sync_pol output_h_v_sync_pol;
};

struct video_path_param {
	uint8_t b_color_conv_en;
	uint8_t b_dithering_en;
	enum sii_hv_sync_pol hv_sync_pol;
	enum sii_hv_sync_pol out_h_v_sync_pol;
	struct video_color_info in_clr_info;
	struct video_color_info out_clr_info;

	uint32_t events_flag;
	void (*event_notify_fn)(void *, uint32_t);

	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;
	uint32_t vidpath_events;
	bool b_set_h_v_sync_pol;
	struct sii_drv_vid_timing_params input_video_timing;
	bool b_check_vix_format;
	bool b_enable_ref_clock;
};

static int s_vid_path_params_update(void *vidpath_obj);
static void s_get_input_video_color_info(
	struct video_path_param *p_vid_path_param,
	struct video_color_info *p_vid_clr_info);
static void s_set_video_pathdefaults(struct video_path_param *p_vid_path_param);

static void s_video_path_core_config(struct video_path_param *p_vid_path_param,
					struct video_path_config *p_config);
static void s_input_data_path_mux_config(
	struct video_path_param *p_vid_path_param,
	struct data_path_mux_cfg *pin_map);
static void s_input_rate_converter_config(
	struct video_path_param *p_vid_path_param,
	struct rate_convert_cfg *p_cfg);
static void s_in_sync_polarity_adjustment_config(
	struct video_path_param *p_vid_path_param,
	struct sync_pol_adjust_cfg *p_cfg);
static void s_video_blanking_config(struct video_path_param *p_vid_path_param,
					struct video_balnk_cfg *p_cfg);
static void s_output_rate_converter_config(
	struct video_path_param *p_vid_path_param,
	struct rate_convert_cfg *p_cfg);
static void s_embedded_sync_encoder_config(
	struct video_path_param *p_vid_path_param,
	struct embedde_sync_cfg *p_cfg);
static void s_out_sync_polarity_adjustment_config(
	struct video_path_param *p_vid_path_param,
	struct sync_pol_adjust_cfg *p_cfg);
static void s_outnput_data_path_mux_config(
	struct video_path_param *p_vid_path_param,
	struct data_path_mux_cfg *pin_map);
static void s_video_path_cms_config(struct video_path_param *p_vid_path_param,
					struct cms_cfg *p_cfg);
static void s_up_sampler_config(struct video_path_param *p_vid_path_param,
				struct sampler_cfg *p_cfg);
static void s_video_path_csc0_config(struct video_path_param *p_vid_path_param,
					struct csc_config *p_cfg);
static void s_down_sampler_config(struct video_path_param *p_vid_path_param,
					struct sampler_cfg *p_cfg);
static void s_output_mute(struct video_path_param *p_vid_path_param,
				bool en_mute);
static void s_decode_color_info(struct video_path_param *p_vid_path_param,
				enum sii_drv_clr_spc out_clr_spc,
				struct video_color_info *out_clr_info);
static void s_update_h_v_sync_pol(struct video_path_param *p_vid_path_param);
#ifdef CONFIG_SWAY_CONFIG_POST_CONV
static void s_vidpath_soft_reset(struct video_path_param *p_vid_path_param);
#endif
/*static void s_video_path_intr_handler(sii_inst_t);*/

void sii_mod_vid_path_reg_read(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t *ptrdata, size_t r_size)
{
	uint8_t i;

	for (i = 0; i < r_size; i++)
		*(ptrdata + i) = sii_platform_rd_reg8(pstDev, addr + i);
}

/* this function is implemeted as per the limitation of video path register
 access*/
/* any change in the register takes effet only after the last byte is written*/
void sii_mod_vid_path_reg_write(pstSii9630_Device_t pstDev,
	uint16_t addr, uint8_t *ptrdata, size_t w_size)
{
	uint8_t i;

	for (i = 0; i < w_size; i++)
		sii_platform_wr_reg8(pstDev, addr + i, *(ptrdata + i));
}

static void reset_states(struct video_path_param *p_vid_path_param)
{
	p_vid_path_param->in_clr_info.clr_spc = RGB;
	p_vid_path_param->in_clr_info.conv_std = SII_DRV_CONV_STD__BT_601;
	p_vid_path_param->in_clr_info.vid_dc_depth = SII_DRV_BIT_DEPTH__8_BIT;
	p_vid_path_param->in_clr_info.quntization = QUANTIZATION_PC_LEVELS;
	p_vid_path_param->b_enable_ref_clock = false;
	p_vid_path_param->b_color_conv_en = 0;
}

static struct video_path_param sg_st_vid_path_param;

void *sii_mod_vid_path_init(
    pstSii9630_Device_t pstDevice,
	void (*event_notify_fn)(void *parent, uint32_t event_flags))
{
	struct video_path_param *p_vid_path_param = &sg_st_vid_path_param;
	//p_vid_path_param = kzalloc(sizeof(struct video_path_param), GFP_KERNEL);
    memset(&sg_st_vid_path_param,0,sizeof(struct video_path_param));
	if (!p_vid_path_param)
		return NULL;
    //sii_log_debug("Video Path Init start\n");

    p_vid_path_param->pstDevice = pstDevice;
	p_vid_path_param->event_notify_fn = event_notify_fn;

    //sii_log_debug("Video Path Init End\n");
	return p_vid_path_param;
}

#if 0
void sii_mod_vid_path_exit(void *vidpath_obj)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
	kfree(p_vid_path_param);
}
#endif

int init_videopath_regs(void *p_obj)
{
	struct video_path_param *p_vid_path_param =
			(struct video_path_param *)p_obj;
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	int status = 0;
	struct video_path_config vid_path_cfg;
	uint32_t vp__fdet_irq_stat = 0;

	uint32_t vp__fdet_framerate_delta_threshold = 0x000010;


	reset_states(p_vid_path_param);

	vid_path_cfg.b_enable_idp_mux = 0;
	vid_path_cfg.b_enable_odp_mux = 0;
	vid_path_cfg.b_enable_ese = 0;
	vid_path_cfg.b_enable_in_spa = 1;
	vid_path_cfg.b_enable_out_spa = 1;
	vid_path_cfg.b_enable_vb = 0;
	vid_path_cfg.b_enable_orc = 0;
	vid_path_cfg.b_enable_irc = 0;
	vid_path_cfg.b_enable_cms = 0;
	vid_path_cfg.b_enable_dithering = 0;
	s_video_path_core_config(p_vid_path_param, &vid_path_cfg);

	/*Clear if any interruipts are there already*/
	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__FDET_IRQ_STATUS,
		(uint8_t *)&vp__fdet_irq_stat,
		BIT_MASK__VP__FDET_IRQ_STAT__SIZE);
	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__FDET_IRQ_STATUS,
		(uint8_t *)&vp__fdet_irq_stat,
		BIT_MASK__VP__FDET_IRQ_STAT__SIZE);

	sii_mod_vid_path_intr_mask_enable(p_obj, true);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__FDET_FRAME_RATE_DELTA_THRESHOLD,
		(uint8_t *)&vp__fdet_framerate_delta_threshold,
		BIT_MASK__VP__FDET_FRAMERATE_DELTA_THRESHOLD__SIZE);

	return status;
}

int sii_mod_vid_path_stop_timers(void *vidpath_obj)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;

	/* stop all timers */
	p_vid_path_param = p_vid_path_param;
	return 0;
}


void sii_mod_vid_path_intr_mask_enable(void *vidpath_obj,
		bool enable)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;

	uint8_t vid_path_intr_mask[3] = {
		BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_POLARITY |
		BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_POLARITY |
		BIT_MSK__VP__FDET_IRQ_MASK__FRAME_RATE |
		BIT_MSK__VP__FDET_IRQ_MASK__PIXEL_COUNT |
		BIT_MSK__VP__FDET_IRQ_MASK__LINE_COUNT |
		BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_LOW_COUNT,

		(BIT_MSK__VP__FDET_IRQ_MASK__HSYNC_HIGH_COUNT |
		BIT_MSK__VP__FDET_IRQ_MASK__HFRONT_COUNT |
		BIT_MSK__VP__FDET_IRQ_MASK__HBACK_COUNT |
		BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_LOW_COUNT_EVEN |
		BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_HIGH_COUNT_EVEN |
		BIT_MSK__VP__FDET_IRQ_MASK__VFRONT_COUNT_EVEN |
		BIT_MSK__VP__FDET_IRQ_MASK__VBACK_COUNT_EVEN |
		BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_LOW_COUNT_ODD) >> 8,

		(BIT_MSK__VP__FDET_IRQ_MASK__VSYNC_HIGH_COUNT_ODD |
		BIT_MSK__VP__FDET_IRQ_MASK__VFRONT_COUNT_ODD |
		BIT_MSK__VP__FDET_IRQ_MASK__VBACK_COUNT_ODD) >> 16
	};
	uint8_t vid_path_intr_mask_clr[3] = {0};

	if(enable)
		sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__FDET_IRQ_MASK,
		vid_path_intr_mask, BIT_MASK__VP__FDET_IRQ_STAT__SIZE);
	else
		sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__FDET_IRQ_MASK,
		vid_path_intr_mask_clr, BIT_MASK__VP__FDET_IRQ_STAT__SIZE);
}

/*----------------------------------------------------------------------------*/

static void s_vid_path_hv_sync_polarity_set(
	void *vidpath_obj, enum sii_hv_sync_pol hv_sync_pol)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
	struct video_path_config vid_path_cfg;

	memset(&vid_path_cfg, 0, sizeof(struct video_path_config));
	sii_log_info("Setting HV_SYNC Polarity to: %02X\n", hv_sync_pol);
	p_vid_path_param->b_set_h_v_sync_pol = true;
	vid_path_cfg.output_h_v_sync_pol = hv_sync_pol;
	vid_path_cfg.b_enable_out_spa = 1;
	s_video_path_core_config(p_vid_path_param, &vid_path_cfg);
}

/*----------------------------------------------------------------------------*/

void sii_mod_vid_path_output_color_space_set(void *vidpath_obj,
						enum sii_drv_clr_spc clr_spc)
{
	struct video_path_config vid_path_cfg;
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;

	memset(&vid_path_cfg, 0, sizeof(struct video_path_config));

	s_decode_color_info(p_vid_path_param, clr_spc,
		&(vid_path_cfg.out_clr_info));
/*	if (vid_path_cfg.out_clr_info.clr_spc == y_cb_cr420) {
		sii_log_debug("Conversion to 420 output not possible\n");

		sii_log_debug("Only Paasthorugh of 420 is permitted\n");

	} else*/ {
		p_vid_path_param->b_color_conv_en = 1;
		vid_path_cfg.out_clr_info.vid_dc_depth = p_vid_path_param->out_clr_info.vid_dc_depth;

		p_vid_path_param->out_clr_info = vid_path_cfg.out_clr_info;

		s_get_input_video_color_info(p_vid_path_param,
			&vid_path_cfg.in_clr_info);

		if (vid_path_cfg.in_clr_info.clr_spc == y_cb_cr420)
			vid_path_cfg.b_enable_irc =
				p_vid_path_param->b_color_conv_en ? 1 : 0;
		else
			vid_path_cfg.b_enable_irc = 0;

		vid_path_cfg.b_enable_cms = 1;

		s_output_mute(p_vid_path_param, true);
		s_set_video_pathdefaults(p_vid_path_param);
		if (vid_path_cfg.in_clr_info.clr_spc == y_cb_cr420) {
			if (p_vid_path_param->event_notify_fn) {
				p_vid_path_param->b_enable_ref_clock =
				 (vid_path_cfg.in_clr_info.clr_spc !=
				  vid_path_cfg.out_clr_info.clr_spc) ?
				  true : false;
				p_vid_path_param->event_notify_fn(
					&g_stdrv_obj,
					SII_DRV_RX_VIDPATH_EVENT__REQ_REF_CLK);
				/*s_vidpath_soft_reset(p_vid_path_param);*/
			}
		}
		s_video_path_core_config(p_vid_path_param, &vid_path_cfg);
		s_output_mute(p_vid_path_param, false);
		p_vid_path_param->b_color_conv_en =
			(clr_spc == SII_DRV_CLRSPC__PASSTHRU) ? 0 : 1;
	}
}

/*----------------------------------------------------------------------------*/
#if 0
void sii_mod_vid_path_output_bit_depth_set(
	void *vidpath_obj, enum sii_drv_bit_depth bit_depth)
{
	struct video_path_config vid_path_cfg;
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
	memset(&vid_path_cfg, 0, sizeof(struct video_path_config));
	s_get_input_video_color_info(p_vid_path_param,
		&vid_path_cfg.in_clr_info);

	if ((bit_depth != SII_DRV_BIT_DEPTH__PASSTHOUGH)
		&& (bit_depth <= p_vid_path_param->in_clr_info.vid_dc_depth)) {
		vid_path_cfg.b_enable_dithering = 1;
		if (vid_path_cfg.in_clr_info.clr_spc == y_cb_cr420) {
			vid_path_cfg.b_enable_irc =
				p_vid_path_param->b_color_conv_en ? 1 : 0;
		} else
			vid_path_cfg.b_enable_irc = 0;

		p_vid_path_param->out_clr_info.vid_dc_depth = bit_depth;
		memcpy(&vid_path_cfg.out_clr_info,
			&p_vid_path_param->out_clr_info,
			sizeof(struct video_color_info));

		p_vid_path_param->b_dithering_en = true;
		vid_path_cfg.b_enable_cms = 1;

		s_output_mute(p_vid_path_param, true);
		s_set_video_pathdefaults(p_vid_path_param);
		s_video_path_core_config(p_vid_path_param, &vid_path_cfg);
		s_output_mute(p_vid_path_param, false);
	} else {
		/*need to recover when higher bit depth is set.*/
		p_vid_path_param->out_clr_info.vid_dc_depth = bit_depth;
		p_vid_path_param->b_dithering_en = true;
	}
}
#endif
/*---------------------------------------------------------------------------*/

void sii_mod_vid_path_color_info_config(
	void *vidpath_obj, struct sii_drv_color_info_cfg *p_clr_info)
{
	struct video_path_config vid_path_cfg;
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;

	memset(&vid_path_cfg, 0, sizeof(struct video_path_config));

	/*Update new clr_info into videopath object*/
	s_decode_color_info(p_vid_path_param, p_clr_info->input_clr_spc,
		&p_vid_path_param->in_clr_info);
	p_vid_path_param->in_clr_info.vid_dc_depth = p_clr_info->input_vid_dc_depth;

	/*Update the latest in_clr_info into vid_path_cfg from videopath_obj*/
	s_get_input_video_color_info(p_vid_path_param,
		&vid_path_cfg.in_clr_info);

	if (vid_path_cfg.in_clr_info.clr_spc == y_cb_cr420)
		vid_path_cfg.b_enable_irc =
			p_vid_path_param->b_color_conv_en ? 1 : 0;
	else
		vid_path_cfg.b_enable_irc = 0;

	if (p_vid_path_param->b_color_conv_en
		|| p_vid_path_param->b_dithering_en) {
		vid_path_cfg.b_enable_cms = 1;
		/*Update the last out_clr_info into vid_path_config from videopath_obj*/
		memcpy(&vid_path_cfg.out_clr_info,
			&p_vid_path_param->out_clr_info,
			sizeof(struct video_color_info));
	} else {
		vid_path_cfg.out_clr_info = vid_path_cfg.in_clr_info;
		memcpy(&p_vid_path_param->out_clr_info,
			&vid_path_cfg.out_clr_info,
			sizeof(struct video_color_info));
	}

	vid_path_cfg.b_enable_dithering = p_vid_path_param->b_dithering_en;
	/* below change is done considering the fact that for input colour
	 * depth has
	 to be followed in following cases:*/
	/* 1. when dithering is disabled*/
	/* 2. disthering is enabled and input bit depth is less than required
	 dithering output.*/
	/* in other cases colour depth has to be recovered to the desired colour
	 depth*/
	if (!(p_vid_path_param->b_dithering_en
		&& (vid_path_cfg.in_clr_info.vid_dc_depth
			>= p_vid_path_param->out_clr_info.vid_dc_depth))) {
		vid_path_cfg.out_clr_info.vid_dc_depth =
			vid_path_cfg.in_clr_info.vid_dc_depth;
	}

	/* CMS is always enabled to make any incoming input videodepth to
	 * 12 bit.*/
	/* for this if we wan to let the video in pas trough mode, dithering
	 * has to
	 be enabled to match the input color depth.*/
	vid_path_cfg.b_enable_cms = 1;

	s_output_mute(p_vid_path_param, true);
	s_set_video_pathdefaults(p_vid_path_param);
	s_video_path_core_config(p_vid_path_param, &vid_path_cfg);
	s_output_mute(p_vid_path_param, false);

}

/*---------------------------------------------------------------------------*/

static void s_get_input_video_color_info(
	struct video_path_param *p_vid_path_param,
	struct video_color_info *p_vid_clr_info)
{
	p_vid_clr_info->clr_spc = p_vid_path_param->in_clr_info.clr_spc;
	p_vid_clr_info->conv_std = p_vid_path_param->in_clr_info.conv_std;
	p_vid_clr_info->vid_dc_depth =
		p_vid_path_param->in_clr_info.vid_dc_depth;
	p_vid_clr_info->quntization = p_vid_path_param->in_clr_info.quntization;
}

/*---------------------------------------------------------------------------*/

static void s_set_video_pathdefaults(struct video_path_param *p_vid_path_param)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint16_t vp__input_format;
	uint16_t vp__output_format;
	uint8_t vp__cms__csc0__c420_c422_config;
	uint8_t vp__cms__csc0__c422_c444_config;
	uint8_t vp__cms__csc1__c444_c422_config;
	uint16_t vp__cms__csc0__multi_csc_config;
	uint8_t vp__cms__csc0__dither_config;

    sii_log_debug("video path default\n");

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);

	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__INPUT_FORMAT,
		(uint8_t *)&vp__input_format, BIT_MASK__VP__INPUT_FORMAT__SIZE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__OUTPUT_FORMAT,
		(uint8_t *)&vp__output_format,
		BIT_MASK__VP__OUTPUT_FORMAT__SIZE);

	vp__input_format &= ~BIT_MSK__VP__INPUT_FORMAT__MUX_420_ENABLE;
	vp__output_format &= ~BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_420_ENABLE;

	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__INPUT_FORMAT,
		(uint8_t *)&vp__input_format, BIT_MASK__VP__INPUT_FORMAT__SIZE);
	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__OUTPUT_FORMAT,
		(uint8_t *)&vp__output_format,
		BIT_MASK__VP__OUTPUT_FORMAT__SIZE);

	SELECT_VIDEO_PATH_PAGE(PAGE_CSC);

	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC0__C420_C422_CONFIG,
		(uint8_t *)&vp__cms__csc0__c420_c422_config,
		BIT_MASK__VP__CMS_CSC0_420_422_CONFIG__SIZE);
	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG,
		(uint8_t *)&vp__cms__csc0__c422_c444_config,
		BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE);
	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC0__MULTI_CSC_CONFIG,
		(uint8_t *)&vp__cms__csc0__multi_csc_config,
		BIT_MASK__VP__CMS_CSC0_MULTI_CSC_CONFIG__SIZE);
	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC0__DITHER_CONFIG,
		(uint8_t *)&vp__cms__csc0__dither_config,
		BIT_MASK__VP__CMS__CSC0__DITHERING_CONFIG__SIZE);
	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC1__C444_C422_CONFIG,
		(uint8_t *)&vp__cms__csc1__c444_c422_config,
		BIT_MASK__VP__CMS_CSC1_444_422_CONFIG__SIZE);

	vp__cms__csc0__c420_c422_config |=
		BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__BYPASS;
	vp__cms__csc0__c420_c422_config &=
		~BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__ENABLE;

	vp__cms__csc0__c422_c444_config &=
		~BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__ENABLE;

	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__ENABLE;

	vp__cms__csc0__dither_config |= 0x03;

	vp__cms__csc1__c444_c422_config &=
		~BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__ENABLE;

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__C420_C422_CONFIG,
		(uint8_t *)&vp__cms__csc0__c420_c422_config,
		BIT_MASK__VP__CMS_CSC0_420_422_CONFIG__SIZE);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG,
		(uint8_t *)&vp__cms__csc0__c422_c444_config,
		BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__MULTI_CSC_CONFIG,
		(uint8_t *)&vp__cms__csc0__multi_csc_config,
		BIT_MASK__VP__CMS_CSC0_MULTI_CSC_CONFIG__SIZE);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__DITHER_CONFIG,
		(uint8_t *)

		&vp__cms__csc0__dither_config,
		BIT_MASK__VP__CMS__CSC0__DITHERING_CONFIG__SIZE);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC1__C444_C422_CONFIG,
		(uint8_t *)&vp__cms__csc1__c444_c422_config,
		BIT_MASK__VP__CMS_CSC1_444_422_CONFIG__SIZE);
}

static void s_decode_color_info(struct video_path_param *p_vid_path_param,
				enum sii_drv_clr_spc clr_spc,
				struct video_color_info *p_clr_info)
{
	struct video_color_info in_clr_info;

	s_get_input_video_color_info(p_vid_path_param, &in_clr_info);

	switch (clr_spc) {
	case SII_DRV_CLRSPC__PASSTHRU:
		p_clr_info->conv_std = in_clr_info.conv_std;
		p_clr_info->clr_spc = in_clr_info.clr_spc;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__RGB_DEFAULT:
	case SII_DRV_CLRSPC__RGB_FULL:
		p_clr_info->conv_std = in_clr_info.conv_std;
		p_clr_info->clr_spc = RGB;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__RGB_LIMITED:
		p_clr_info->conv_std = in_clr_info.conv_std;
		p_clr_info->clr_spc = RGB;
		p_clr_info->quntization = QUANTIZATION_PC_LEVELS;
		break;
	case SII_DRV_CLRSPC__XVYCC420_601:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_601;
		p_clr_info->clr_spc = y_cb_cr420;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__XVYCC420_709:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_709;
		p_clr_info->clr_spc = RGB;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__XVYCC422_601:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_601;
		p_clr_info->clr_spc = y_cb_cr422;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__XVYCC422_709:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_709;
		p_clr_info->clr_spc = y_cb_cr422;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__XVYCC444_601:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_601;
		p_clr_info->clr_spc = y_cb_cr444;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__XVYCC444_709:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_709;
		p_clr_info->clr_spc = y_cb_cr444;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC420_2020:
		p_clr_info->conv_std =
			SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS;
		p_clr_info->clr_spc = y_cb_cr420;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC420_601:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_601;
		p_clr_info->clr_spc = y_cb_cr420;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC420_709:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_709;
		p_clr_info->clr_spc = y_cb_cr420;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC422_2020:
		p_clr_info->conv_std =
			SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS;
		p_clr_info->clr_spc = y_cb_cr422;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC422_601:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_601;
		p_clr_info->clr_spc = y_cb_cr422;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC422_709:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_709;
		p_clr_info->clr_spc = y_cb_cr422;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC444_2020:
		p_clr_info->conv_std =
			SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS;
		p_clr_info->clr_spc = y_cb_cr444;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC444_601:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_601;
		p_clr_info->clr_spc = y_cb_cr444;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	case SII_DRV_CLRSPC__YC444_709:
		p_clr_info->conv_std = SII_DRV_CONV_STD__BT_709;
		p_clr_info->clr_spc = y_cb_cr444;
		p_clr_info->quntization = QUANTIZATION_VIDEO_LEVELS;
		break;
	}
	if (p_clr_info->clr_spc == y_cb_cr420)
		p_vid_path_param->b_check_vix_format = true;
	else
		p_vid_path_param->b_check_vix_format = false;
}

#ifdef CONFIG_SWAY_CONFIG_POST_CONV
static void s_vidpath_soft_reset(struct video_path_param *p_vid_path_param)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__soft_reset;

	vp__soft_reset = BIT_MSK__VP__SOFT_RESET__RESET_CLK_IN |
		BIT_MSK__VP__SOFT_RESET__RESET_CLK_CORE |
		BIT_MSK__VP__SOFT_RESET__RESET_CLK_OUT;
	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__SOFT_RESET,
		(uint8_t *)&vp__soft_reset, BIT_MASK__VP__SOFT_RESET__SIZE);
	vp__soft_reset = 0x00;
	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__SOFT_RESET,
		(uint8_t *)&vp__soft_reset, BIT_MASK__VP__SOFT_RESET__SIZE);
}
#endif

/*----------------------------------------------------------------------------*/

static void s_video_path_core_config(struct video_path_param *p_vid_path_param,
					struct video_path_config *p_cfg)
{
	struct data_path_mux_cfg dpm_cfg;
	struct rate_convert_cfg rc_cfg;
	struct sync_pol_adjust_cfg spa_cfg;
	struct cms_cfg cms_cfg;
	struct video_balnk_cfg vid_balnk_cfg;
	struct embedde_sync_cfg ese_cfg;

	if (p_cfg->b_enable_idp_mux) {
		/*set configuration params*/
		s_input_data_path_mux_config(p_vid_path_param, &dpm_cfg);
	}
	if (p_cfg->b_enable_irc) {

		if (p_cfg->in_clr_info.clr_spc ==
			p_cfg->out_clr_info.clr_spc) {
				/*set configuration params*/
				rc_cfg.mux_420_enable = 0;

		} else {
			rc_cfg.mux_420_enable = 1;
		}

		s_input_rate_converter_config(p_vid_path_param, &rc_cfg);
	}
	if (p_cfg->b_enable_in_spa) {
		spa_cfg.b_auto_adjust = 0;
		spa_cfg.hsync_pol = SYNCPOL_POSITIVE;
		spa_cfg.vsync_pol = SYNCPOL_POSITIVE;
		/*disable polarity while tesing DSC from 804A
		as its not sending correct polarity*/
		s_in_sync_polarity_adjustment_config(p_vid_path_param,
			&spa_cfg);
	}
	if (p_cfg->b_enable_cms) {
		memset(&cms_cfg, 0, sizeof(struct cms_cfg));
		if (p_vid_path_param->b_color_conv_en) {
			switch (p_cfg->in_clr_info.clr_spc) {
			case y_cb_cr420:
				switch (p_cfg->out_clr_info.clr_spc) {
				case y_cb_cr422:
					cms_cfg.b_enable_420422 = 1;
					cms_cfg.b_enable_422444 = 0;
					cms_cfg.b_enable_444422 = 0;
					break;
				case RGB:
					cms_cfg.b_enable_csc0 = 1;
				case y_cb_cr444:
					cms_cfg.b_enable_420422 = 1;
					cms_cfg.b_enable_422444 = 1;
					cms_cfg.b_enable_444422 = 0;
					p_cfg->b_enable_dithering = 0;
					break;
				default:
					break;
				}
				break;
			case y_cb_cr422:
				switch (p_cfg->out_clr_info.clr_spc) {
				case RGB:
					cms_cfg.b_enable_csc0 = 1;
				case y_cb_cr444:
					cms_cfg.b_enable_420422 = 0;
					cms_cfg.b_enable_422444 = 1;
					cms_cfg.b_enable_444422 = 0;
					p_cfg->b_enable_dithering = 0;
					break;
				default:
					break;
				}
				break;
			case y_cb_cr444:
				switch (p_cfg->out_clr_info.clr_spc) {
				case y_cb_cr422:
					cms_cfg.b_enable_444422 = 1;
					cms_cfg.b_enable_420422 = 0;
					cms_cfg.b_enable_422444 = 0;
					break;
				case RGB:
					cms_cfg.b_enable_420422 = 0;
					cms_cfg.b_enable_422444 = 0;
					cms_cfg.b_enable_444422 = 0;
					p_cfg->b_enable_dithering = 0;
					cms_cfg.b_enable_csc0 = 1;
					break;
				default:
					break;
				}
				break;
			case RGB:
				switch (p_cfg->out_clr_info.clr_spc) {
				case y_cb_cr422:
					cms_cfg.b_enable_444422 = 1;
					cms_cfg.b_enable_420422 = 0;
					cms_cfg.b_enable_422444 = 0;
					cms_cfg.b_enable_csc0 = 1;
					break;
				case y_cb_cr444:
					cms_cfg.b_enable_444422 = 0;
					cms_cfg.b_enable_420422 = 0;
					cms_cfg.b_enable_422444 = 0;
					p_cfg->b_enable_dithering = 0;
					cms_cfg.b_enable_csc0 = 1;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			memcpy(&cms_cfg.out_clr_info, &p_cfg->out_clr_info,
				sizeof(struct video_color_info));
			memcpy(&cms_cfg.in_clr_info, &p_cfg->in_clr_info,
				sizeof(struct video_color_info));
		}
		if ((p_cfg->b_enable_dithering)
			|| (p_cfg->out_clr_info.vid_dc_depth
				!= p_cfg->in_clr_info.vid_dc_depth))
			cms_cfg.b_enable_dithering = p_cfg->b_enable_dithering;
		else
			cms_cfg.b_enable_dithering = 0;
		/*notify colour conversion to controller so that info frames
		 * can be
		 changed even before actual colour is outputted*/
		if (p_vid_path_param->b_color_conv_en)

			p_vid_path_param->event_notify_fn(
				&g_stdrv_obj,
				SII_DRV_RX_VIDPATH_EVENT__COLOR_CONV_DONE);

		s_video_path_cms_config(p_vid_path_param, &cms_cfg);
#ifdef CONFIG_SWAY_CONFIG_POST_CONV
		s_vidpath_soft_reset(p_vid_path_param);
#endif
	}
	if (p_cfg->b_enable_vb) {
		/*set configuration params*/
		s_video_blanking_config(p_vid_path_param, &vid_balnk_cfg);
	}
	if (p_cfg->b_enable_orc) {
		/*set configuration params*/
		rc_cfg.mux_420_enable = 1;
		s_output_rate_converter_config(p_vid_path_param, &rc_cfg);
	}
	if (p_cfg->b_enable_ese) {
		/*set configuration params*/
		s_embedded_sync_encoder_config(p_vid_path_param, &ese_cfg);
	}
	if (p_cfg->b_enable_out_spa) {
		if (p_vid_path_param->b_set_h_v_sync_pol) {
			spa_cfg.b_auto_adjust = 0;
			spa_cfg.hsync_pol = SYNCPOL_POSITIVE;
			spa_cfg.vsync_pol = SYNCPOL_POSITIVE;
			p_vid_path_param->out_h_v_sync_pol = p_cfg->output_h_v_sync_pol;
			switch (p_cfg->output_h_v_sync_pol) {
			case SII_DRV_HV_SYNC_POL__HPVP:
				spa_cfg.hsync_pol = SYNCPOL_POSITIVE;
				spa_cfg.vsync_pol = SYNCPOL_POSITIVE;
				break;
			case SII_DRV_HV_SYNC_POL__HPVN:
				spa_cfg.hsync_pol = SYNCPOL_POSITIVE;
				spa_cfg.vsync_pol = SYNCPOL_NEGATIVE;
				break;
			case SII_DRV_HV_SYNC_POL__HNVP:
				spa_cfg.hsync_pol = SYNCPOL_NEGATIVE;
				spa_cfg.vsync_pol = SYNCPOL_POSITIVE;
				break;
			case SII_DRV_HV_SYNC_POL__HNVN:
				spa_cfg.hsync_pol = SYNCPOL_NEGATIVE;
				spa_cfg.vsync_pol = SYNCPOL_NEGATIVE;
				break;
			}
		} else
			spa_cfg.b_auto_adjust = 1;
		s_out_sync_polarity_adjustment_config(p_vid_path_param,
			&spa_cfg);
	}
	if (p_cfg->b_enable_odp_mux) {
		/*set configuration params*/
		s_outnput_data_path_mux_config(p_vid_path_param, &dpm_cfg);
	}
}

/*---------------------------------------------------------------------------*/

static void s_video_path_cms_config(struct video_path_param *p_vid_path_param,
					struct cms_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	struct sampler_cfg up_sampler_cfg;
	struct sampler_cfg down_sampler_cfg;
	struct csc_config csc0_cfg;

	SELECT_VIDEO_PATH_PAGE(PAGE_CMS);
	p_vid_path_param->b_enable_ref_clock = false;
	if (p_cfg->b_enable_420422 || p_cfg->b_enable_422444) {
		/**
		 * conversion from 420->422/422->444
		 */
		up_sampler_cfg.input_clr_spc = p_cfg->in_clr_info.clr_spc;
		up_sampler_cfg.output_clr_spc =
			(p_cfg->out_clr_info.clr_spc == y_cb_cr422) ?
				y_cb_cr422 : y_cb_cr444;
		s_up_sampler_config(p_vid_path_param, &up_sampler_cfg);
	}

	if (p_cfg->b_enable_csc0) {
		/**
		 * conversion from  YCBCR444->RGB
		 */
		csc0_cfg.in_conv_std = p_cfg->in_clr_info.conv_std;
		csc0_cfg.input_clr_spc = p_cfg->in_clr_info.clr_spc;
		csc0_cfg.output_clr_spc = p_cfg->out_clr_info.clr_spc;
		csc0_cfg.in_quantization = p_cfg->in_clr_info.quntization;
		csc0_cfg.out_conv_std = p_cfg->out_clr_info.conv_std;
		csc0_cfg.out_quantization = p_cfg->out_clr_info.quntization;

		s_video_path_csc0_config(p_vid_path_param, &csc0_cfg);

	}

	if (p_cfg->b_enable_444422) {
		/* conversion from 444->422 */
		down_sampler_cfg.input_clr_spc = y_cb_cr444;
		down_sampler_cfg.output_clr_spc = p_cfg->out_clr_info.clr_spc;
		s_down_sampler_config(p_vid_path_param, &down_sampler_cfg);
	}
}

static void s_up_sampler_config(struct video_path_param *p_vid_path_param,
				struct sampler_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__cms__csc0__c420_c422_config;
	uint8_t vp__cms__csc0__c422_c444_config;

	uint8_t bit_msk_csc_422_444_cfg_en =
		BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__ENABLE;
	uint8_t bit_msk_csc_422_444_cfg_use_cb_or_cr =
		BIT_MSK__VP__CMS__CSC0__C422_C444_CONFIG__USE_CB_OR_CR;
	uint8_t bit_msk_csc_420_422_cfg_bypass =
		BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__BYPASS;
	uint8_t bit_msk_csc_420_422_cfg_en =
		BIT_MSK__VP__CMS__CSC0__C420_C422_CONFIG__ENABLE;

    sii_log_debug("CSC: %02x->%02x\n", (int)p_cfg->input_clr_spc, (int)p_cfg->output_clr_spc);

	SELECT_VIDEO_PATH_PAGE(PAGE_CSC);

	if (p_cfg->input_clr_spc == y_cb_cr420) {
		switch (p_cfg->output_clr_spc) {
		case y_cb_cr444:
			sii_mod_vid_path_reg_read(pstDev,
				REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG,
				(uint8_t *)&vp__cms__csc0__c422_c444_config,
				BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE);
			vp__cms__csc0__c422_c444_config |=
				bit_msk_csc_422_444_cfg_en;
			vp__cms__csc0__c422_c444_config |=
				bit_msk_csc_422_444_cfg_use_cb_or_cr;
			sii_mod_vid_path_reg_write(pstDev,
				REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG,
				(uint8_t *)&vp__cms__csc0__c422_c444_config,
				BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE);
			/*fall through*/
		case y_cb_cr422:
			sii_mod_vid_path_reg_read(pstDev,
				REG_ADDR__VP__CMS__CSC0__C420_C422_CONFIG,
				(uint8_t *)&vp__cms__csc0__c420_c422_config,
				BIT_MASK__VP__CMS_CSC0_420_422_CONFIG__SIZE);
			vp__cms__csc0__c420_c422_config &=
				~bit_msk_csc_420_422_cfg_bypass;
			vp__cms__csc0__c420_c422_config |=
				bit_msk_csc_420_422_cfg_en;
			sii_mod_vid_path_reg_write(pstDev,
				REG_ADDR__VP__CMS__CSC0__C420_C422_CONFIG,
				(uint8_t *)&vp__cms__csc0__c420_c422_config,
				BIT_MASK__VP__CMS_CSC0_420_422_CONFIG__SIZE);
			break;
		default:
			break;
		}
	}
	if (p_cfg->input_clr_spc == y_cb_cr422) {
		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG,
			(uint8_t *)&vp__cms__csc0__c422_c444_config,
			BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE);

		switch (p_cfg->output_clr_spc) {
		case y_cb_cr444:
			vp__cms__csc0__c422_c444_config |=
				bit_msk_csc_422_444_cfg_en;
			vp__cms__csc0__c422_c444_config |=
				bit_msk_csc_422_444_cfg_use_cb_or_cr;
			break;
		default:
			break;
		}
		sii_mod_vid_path_reg_write(pstDev,
			REG_ADDR__VP__CMS__CSC0__C422_C444_CONFIG,
			(uint8_t *)&vp__cms__csc0__c422_c444_config,
			BIT_MASK__VP__CMS_CSC0_422_444_CONFIG__SIZE);
	}
}

static void s_down_sampler_config(struct video_path_param *p_vid_path_param,
					struct sampler_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__cms__csc1__c444_c422_config;
	uint8_t bit_msk_csc_444_422_cfg_en =
		BIT_MSK__VP__CMS__CSC1__C444_C422_CONFIG__ENABLE;

	SELECT_VIDEO_PATH_PAGE(PAGE_CSC);

	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC1__C444_C422_CONFIG,
		(uint8_t *)&vp__cms__csc1__c444_c422_config,
		BIT_MASK__VP__CMS_CSC1_444_422_CONFIG__SIZE);

	if (p_cfg->input_clr_spc == y_cb_cr444) {
		switch (p_cfg->output_clr_spc) {
		case y_cb_cr422:
			vp__cms__csc1__c444_c422_config |=
				bit_msk_csc_444_422_cfg_en;
			break;
		default:
			break;
		}
	}

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC1__C444_C422_CONFIG,
		(uint8_t *)&vp__cms__csc1__c444_c422_config,
		BIT_MASK__VP__CMS_CSC1_444_422_CONFIG__SIZE);

}

/*---------------------------------------------------------------------------*/

static void s_video_path_csc0_config(struct video_path_param *p_vid_path_param,
					struct csc_config *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint16_t vp__cms__csc0__multi_csc_config;

	SELECT_VIDEO_PATH_PAGE(PAGE_CSC);

	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__CMS__CSC0__MULTI_CSC_CONFIG,
		(uint8_t *)&vp__cms__csc0__multi_csc_config,
		BIT_MASK__VP__CMS_CSC0_MULTI_CSC_CONFIG__SIZE);

	/*-----------configure csc0----------------*/
	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_STD;
	vp__cms__csc0__multi_csc_config |= (p_cfg->in_conv_std << 6);
	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_RGB;
	vp__cms__csc0__multi_csc_config |=
		(p_cfg->input_clr_spc == RGB) ?
			BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_RGB : 0;
	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_PC;
	if (p_cfg->in_quantization == QUANTIZATION_PC_LEVELS)
		vp__cms__csc0__multi_csc_config |=
			BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__IN_PC;

	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_STD;
	vp__cms__csc0__multi_csc_config |= (p_cfg->out_conv_std << 2);
	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_RGB;
	vp__cms__csc0__multi_csc_config |=
		(p_cfg->output_clr_spc == RGB) ?
			BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_RGB : 0;
	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_PC;
	if (p_cfg->out_quantization == QUANTIZATION_PC_LEVELS)
		vp__cms__csc0__multi_csc_config |=
			BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__OUT_PC;

	vp__cms__csc0__multi_csc_config &=
		~BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__DITHER_ENABLE;

	vp__cms__csc0__multi_csc_config |=
		(BIT_MSK__VP__CMS__CSC0__MULTI_CSC_CONFIG__ENABLE & 0x0001);

	/*--------------------------------------------------------*/
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__MULTI_CSC_CONFIG,
		(uint8_t *)&vp__cms__csc0__multi_csc_config,
		BIT_MASK__VP__CMS_CSC0_MULTI_CSC_CONFIG__SIZE);
}

/*---------------------------------------------------------------------------*/

static void s_input_data_path_mux_config(
	struct video_path_param *p_vid_path_param,
	struct data_path_mux_cfg *pin_map)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint16_t input_pin_mapping;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);

	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__INPUT_MAPPING,
	(uint8_t *)&input_pin_mapping, BIT_MASK__VP__INPUT_PIN_MAPPING);

	if (pin_map->color_space == ACS__YCBCR420) {

		input_pin_mapping = ((0x01 << VP__INPUT_MAPPING__SELECT_CR) |
			(0x2 << VP__INPUT_MAPPING__SELECT_Y));
		sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__INPUT_MAPPING,
		(uint8_t *)&input_pin_mapping,
		BIT_MASK__VP__INPUT_PIN_MAPPING);

	} else if (pin_map->color_space == ACS__YCBCR444) {
		input_pin_mapping = ((0x1 << VP__INPUT_MAPPING__SELECT_CR)|
			(0x2 << VP__INPUT_MAPPING__SELECT_CB));
		sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__INPUT_MAPPING,
		(uint8_t *)&input_pin_mapping,
		BIT_MASK__VP__INPUT_PIN_MAPPING);

	} else if (pin_map->color_space == ACS__RGB) {
		input_pin_mapping = ((0x2 << VP__INPUT_MAPPING__SELECT_CR)|
			(0x0 << VP__INPUT_MAPPING__SELECT_Y)|
			(0x1 << VP__INPUT_MAPPING__SELECT_CB));
		sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__INPUT_MAPPING,
		(uint8_t *)&input_pin_mapping,
		BIT_MASK__VP__INPUT_PIN_MAPPING);
	} else if (pin_map->color_space == ACS__YCBCR422) {
		input_pin_mapping = ((0x3 << VP__INPUT_MAPPING__SELECT_CR)|
			(0x3 << VP__INPUT_MAPPING__SELECT_Y)|
			(0x3 << VP__INPUT_MAPPING__SELECT_CB));
		sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__INPUT_MAPPING,
		(uint8_t *)&input_pin_mapping,
		BIT_MASK__VP__INPUT_PIN_MAPPING);
	}

	sii_log_debug("input_pin_mapping Selected: 0x%x\n", input_pin_mapping);
}

static void s_input_rate_converter_config(
	struct video_path_param *p_vid_path_param,
	struct rate_convert_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint16_t vp_input_format;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__INPUT_FORMAT,
		(uint8_t *)&vp_input_format, BIT_MASK__VP__INPUT_FORMAT__SIZE);

	vp_input_format &= ~BIT_MSK__VP__INPUT_FORMAT__MUX_420_ENABLE;
	vp_input_format |=
		p_cfg->mux_420_enable ?
			BIT_MSK__VP__INPUT_FORMAT__MUX_420_ENABLE : 0;

	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__INPUT_FORMAT,
		(uint8_t *)&vp_input_format, BIT_MASK__VP__INPUT_FORMAT__SIZE);
}

static void s_in_sync_polarity_adjustment_config(
	struct video_path_param *p_vid_path_param,
	struct sync_pol_adjust_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__input_sync_adjust_config;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);

	sii_mod_vid_path_reg_read(pstDev,
		REG_ADDR__VP__INPUT_SYNC_ADJUST_CONFIG,
		(uint8_t *)&vp__input_sync_adjust_config,
		BIT_MASK__VP__INPUT_SYNC_ADJUST_CONFIG__SIZE);

	vp__input_sync_adjust_config &=
		~BIT_MSK__VP__INPUT_SYNC_ADJUST_CONFIG__AUTO_DISABLE;
	vp__input_sync_adjust_config |=
		p_cfg->b_auto_adjust ?
			0 : BIT_MSK__VP__INPUT_SYNC_ADJUST_CONFIG__AUTO_DISABLE;

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__INPUT_SYNC_ADJUST_CONFIG,
		(uint8_t *)&vp__input_sync_adjust_config,
		BIT_MASK__VP__INPUT_SYNC_ADJUST_CONFIG__SIZE);
}

static void s_video_blanking_config(struct video_path_param *p_vid_path_param,
					struct video_balnk_cfg *p_cfg)
{
}

static void s_output_rate_converter_config(
	struct video_path_param *p_vid_path_param,
	struct rate_convert_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint16_t vp__output_format;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__OUTPUT_FORMAT,
		(uint8_t *)&vp__output_format,
		BIT_MASK__VP__OUTPUT_FORMAT__SIZE);

	vp__output_format &= ~BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_420_ENABLE;
	vp__output_format |=
		p_cfg->mux_420_enable ?
			BIT_MSK__VP__OUTPUT_FORMAT__DEMUX_420_ENABLE : 0;

	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__OUTPUT_FORMAT,
		(uint8_t *)&vp__output_format,
		BIT_MASK__VP__OUTPUT_FORMAT__SIZE);
}

static void s_embedded_sync_encoder_config(
	struct video_path_param *p_vid_path_param,
	struct embedde_sync_cfg *p_cfg)
{
}

static void s_out_sync_polarity_adjustment_config(
	struct video_path_param *p_vid_path_param,
	struct sync_pol_adjust_cfg *p_cfg)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	if (!p_cfg->b_auto_adjust) {
		uint8_t vp__output_sync_config = 0;

		SELECT_VIDEO_PATH_PAGE(PAGE_CORE);

		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__OUTPUT_SYNC_CONFIG,
			(uint8_t *)&vp__output_sync_config,
			BIT_MASK__VP__OUTPUT_SYNC_CONFIG__SIZE);

		vp__output_sync_config &=
			~(BIT_MSK__VP__OUTPUT_SYNC_CONFIG__VSYNC_POLARITY |
			BIT_MSK__VP__OUTPUT_SYNC_CONFIG__HSYNC_POLARITY);

		vp__output_sync_config |=
			p_cfg->vsync_pol ?
				0 :
				BIT_MSK__VP__OUTPUT_SYNC_CONFIG__VSYNC_POLARITY;
		vp__output_sync_config |=
			p_cfg->hsync_pol ?
				0 :
				BIT_MSK__VP__OUTPUT_SYNC_CONFIG__HSYNC_POLARITY;

		sii_mod_vid_path_reg_write(pstDev,
			REG_ADDR__VP__OUTPUT_SYNC_CONFIG,
			&vp__output_sync_config,
			BIT_MASK__VP__OUTPUT_SYNC_CONFIG__SIZE);
	}
}

static void s_outnput_data_path_mux_config(
	struct video_path_param *p_vid_path_param,
	struct data_path_mux_cfg *pin_map)
{
}

static void s_output_mute(struct video_path_param *p_vid_path_param,
				bool en_mute)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__output_mute;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__OUTPUT_MUTE,
		(uint8_t *)&vp__output_mute, BIT_MASK__VP__OUTPUT_MUTE__SIZE);

	vp__output_mute = en_mute ? 0xFF : 0x00;

	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__OUTPUT_MUTE,
		(uint8_t *)&vp__output_mute, BIT_MASK__VP__OUTPUT_MUTE__SIZE);
}

static void s_update_h_v_sync_pol(struct video_path_param *p_vid_path_param)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__fdet_status;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__FDET_STATUS,
		(uint8_t *)&vp__fdet_status, BIT_MASK__VP__FDET_STATUS__SIZE);

	if (vp__fdet_status & BIT_MSK__VP__FDET_STATUS__HSYNC_POLARITY) {
		p_vid_path_param->hv_sync_pol =
			(vp__fdet_status
				& BIT_MSK__VP__FDET_STATUS__VSYNC_POLARITY) ?
				SII_DRV_HV_SYNC_POL__HPVP :
				SII_DRV_HV_SYNC_POL__HPVN;
	} else {
		p_vid_path_param->hv_sync_pol =
			(vp__fdet_status
				& BIT_MSK__VP__FDET_STATUS__VSYNC_POLARITY) ?
				SII_DRV_HV_SYNC_POL__HNVP :
				SII_DRV_HV_SYNC_POL__HNVN;
	}
}

static void s_vtg_enable(struct video_path_param *p_vid_path_param,
				bool b_vtg_enable)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint8_t vp__cmd__csc0__vtg_config = 0x00;

	if (b_vtg_enable) {
		vp__cmd__csc0__vtg_config |=
			(BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__ENABLE
				| BIT_MSK__VP__CMS__CSC0__VTG_CONFIG__RESYNC);
	}
	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__CMS__CSC0__VTG_CONFIG,
		(uint8_t *)&vp__cmd__csc0__vtg_config, 1);
}

static void s_set_vtg_params(struct video_path_param *p_vid_path_param,
				struct sii_drv_vtg_params *p_vtg_timing_params)
{
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;
	uint16_t vtg_video_h_active_end =
		p_vtg_timing_params->vtg_horizontal_active_video_end;
	uint16_t vtg_video_v_active_start =
		p_vtg_timing_params->vtg_vertical_active_video_start;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	s_vtg_enable(p_vid_path_param, false);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_SYNC_END,
		(uint8_t *)&p_vtg_timing_params->vtg_horizontal_sync_end, 2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_ACTIVE_VIDEO_START,
		(uint8_t *)&p_vtg_timing_params->vtg_horizontal_active_video_start, 2);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_HALFLINE,
		(uint8_t *)&p_vtg_timing_params->vtg_halfline, 2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_HORIZONTAL_ACTIVE_VIDEO_END,
		(uint8_t *)&vtg_video_h_active_end,
		2);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_END_OF_LINE,
		(uint8_t *)&p_vtg_timing_params->vtg_end_of_line, 2);
	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_SYNC_END,
		(uint8_t *)&p_vtg_timing_params->vtg_vertical_sync_end, 2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_TRIGGER_START,
		(uint8_t *)&p_vtg_timing_params->vtg_trigger_start, 2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_ACTIVE_VIDEO_START,
		(uint8_t *)&vtg_video_v_active_start,
		2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_ACTIVE_VIDEO_END,
		(uint8_t *)&p_vtg_timing_params->vtg_vertical_active_video_end,
		2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_VERTICAL_END_OF_FRAME,
		(uint8_t *)&p_vtg_timing_params->vtg_vertical_end_of_frame, 2);

	sii_mod_vid_path_reg_write(pstDev,
		REG_ADDR__VP__CMS__CSC0__VTG_CYCLE_DELAY,
		(uint8_t *)&p_vtg_timing_params->vtg_cycle_delay, 2);
	{
		uint8_t vp__cms__csc0__vtg_bank_config =
			BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__UPDATE_MODE;
		uint8_t vp__cms__csc0__vtg_update_request = 0x00;

		sii_mod_vid_path_reg_write(pstDev,
			REG_ADDR__VP__CMS__CSC0__VTG_BANK_CONFIG,
			(uint8_t *)&vp__cms__csc0__vtg_bank_config, 1);
		sii_mod_vid_path_reg_write(pstDev,
			REG_ADDR__VP__CMS__CSC0__VTG_UPDATE_REQUEST,
			(uint8_t *)&vp__cms__csc0__vtg_update_request, 1);
		vp__cms__csc0__vtg_bank_config &=
			~BIT_MSK__VP__CMS__CSC0__VTG_BANK_CONFIG__UPDATE_MODE;
		sii_mod_vid_path_reg_write(pstDev,
			REG_ADDR__VP__CMS__CSC0__VTG_BANK_CONFIG,
			(uint8_t *)&vp__cms__csc0__vtg_bank_config, 1);
	}
}

int sii_mod_vid_path_set(void *vidpath_obj,
	enum sii_mod_vid_path_opcode op_code, void *in_data)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
	int status = 0;

	if (p_vid_path_param) {
		switch (op_code) {
		case SII_MOD_VIDPATH_OPCODE__IN_VIDINFO:
			sii_mod_vid_path_color_info_config(vidpath_obj,
				(struct sii_drv_color_info_cfg *)in_data);
			break;
		case SII_MOD_VIDPATH_OPCODE__OUT_CLRSPC:
			sii_mod_vid_path_output_color_space_set(vidpath_obj,
				*((enum sii_drv_clr_spc *)in_data));
			break;
		case SII_MOD_VIDPATH_OPCODE__OUT_CLRDEPTH:
			break;
		case SII_MOD_VIDPATH_OPCODE__OUT_HVSYNC_POLARITY:
		{
			enum sii_hv_sync_pol hv_sync_pol =
				*(enum sii_hv_sync_pol *)in_data;

			s_vid_path_hv_sync_polarity_set(vidpath_obj,
				hv_sync_pol);
			break;
		}
		case SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS:
			s_set_vtg_params(p_vid_path_param,
				(struct sii_drv_vtg_params *)in_data);
			break;
		case SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS_UPDATE:
			s_vid_path_params_update(vidpath_obj);
			break;
		case SII_MOD_VIDPATH_OPCODE__VTG_ENABLE:
			s_vtg_enable(p_vid_path_param,
				*((bool *)in_data) ? true : false);
			break;
		case SII_MOD_VIDPATH_OPCODE__PIN_MAPPING_CONFIG:
			s_input_data_path_mux_config(p_vid_path_param,
				(struct data_path_mux_cfg *)in_data);
			break;
		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

int sii_mod_vid_path_get(void *vidpath_obj,
				enum sii_mod_vid_path_opcode op_code,
				void *out_data)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
	int status = 0;

	if (p_vid_path_param) {
		switch (op_code) {
		case SII_MOD_VIDPATH_OPCODE__IN_VIDINFO:
			break;
		case SII_MOD_VIDPATH_OPCODE__HVSYNC:
			memcpy(out_data, &p_vid_path_param->hv_sync_pol,
				sizeof(enum sii_hv_sync_pol));
			break;
		case SII_MOD_VIDPATH_OPCODE__COLORCONVEN:
			memcpy(out_data, &p_vid_path_param->b_color_conv_en,
				sizeof(uint8_t));
			break;
		case SII_MOD_VIDPATH_OPCODE__IN_CLRSPC:
			memcpy(out_data,
				&p_vid_path_param->in_clr_info.clr_spc,
				sizeof(uint8_t));
			break;
		case SII_MOD_VIDPATH_OPCODE__OUT_CLRSPC:
			memcpy(out_data,
				&p_vid_path_param->out_clr_info.clr_spc,
				sizeof(uint8_t));
			break;
		case SII_MOD_VIDPATH_OPCODE__OUT_CONVSTD:
			memcpy(out_data,
				&p_vid_path_param->out_clr_info.conv_std,
				sizeof(enum sii_drv_conv_std));
			break;
		case SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS:
			memcpy(out_data, &p_vid_path_param->input_video_timing,
				sizeof(struct sii_drv_vid_timing_params));
			break;
		case SII_MOD_VIDPATH_OPCODE__ENABLE_REF_CLOCK:
			memcpy(out_data, &p_vid_path_param->b_enable_ref_clock,
				sizeof(bool));
			break;

		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

static int s_vid_path_params_update(void *vidpath_obj)
{
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;

	if (!p_vid_path_param)
		return 1;


	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	/*Update HSYNC/VSYNC*/
	{
		s_update_h_v_sync_pol(p_vid_path_param);
	}

	/*Update Frame rate*/
	{
		uint32_t vp__fdet_frame_rate = 0x00000000;
		uint8_t roundoff = 0;
		sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__FDET_FRAME_RATE,
			(uint8_t *)&vp__fdet_frame_rate, 3);
		if (0 == vp__fdet_frame_rate)
			return 0;

		p_vid_path_param->input_video_timing.v_freq =
			(24000000 / vp__fdet_frame_rate);

		/*This block is to calculate the floating point of the v_freq*/
		p_vid_path_param->input_video_timing.v_freq_mult_factor =
			(((24000000 % vp__fdet_frame_rate) * 1000)
			/ (vp__fdet_frame_rate));
		roundoff = (p_vid_path_param->input_video_timing.v_freq_mult_factor % 10);
		if (roundoff <= 5)
			p_vid_path_param->input_video_timing.v_freq_mult_factor -= roundoff;
		else
			p_vid_path_param->input_video_timing.v_freq_mult_factor +=
			(10 - roundoff);
		p_vid_path_param->input_video_timing.v_freq_mult_factor /= 10;
		if (p_vid_path_param->input_video_timing.v_freq_mult_factor
			== 100) {
			p_vid_path_param->input_video_timing.v_freq++;
			p_vid_path_param->input_video_timing.v_freq_mult_factor = 0;
		}

		sii_log_debug("Frame rate value: %d.%d Hz\n",
				p_vid_path_param->input_video_timing.v_freq,
				p_vid_path_param->input_video_timing.v_freq_mult_factor);
	}
	{
		uint16_t vp__fdet_hback_count = 0;
		uint16_t vp__fdet_hfront_count = 0;
		uint16_t vp__fdet_hsync_high_count = 0;
		uint16_t vp__fdet_hsync_low_count = 0;
		uint32_t hsync = 0;

		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_HBACK_COUNT,
			(uint8_t *)&vp__fdet_hback_count, 2);
		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_HFRONT_COUNT,
			(uint8_t *)&vp__fdet_hfront_count, 2);
		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_HSYNC_LOW_COUNT,
			(uint8_t *)&hsync, 4);
		vp__fdet_hsync_high_count = hsync & 0xFFFF;
		vp__fdet_hsync_low_count = (hsync >> 16) & 0xFFFF;

		if ((SII_DRV_HV_SYNC_POL__HPVP ==
			p_vid_path_param->hv_sync_pol)
			|| (SII_DRV_HV_SYNC_POL__HPVN ==
			p_vid_path_param->hv_sync_pol)) {

			p_vid_path_param->input_video_timing.h_blank =
				vp__fdet_hback_count + vp__fdet_hfront_count
				+ vp__fdet_hsync_low_count;
		} else if ((SII_DRV_HV_SYNC_POL__HNVP ==
			p_vid_path_param->hv_sync_pol)
			|| (SII_DRV_HV_SYNC_POL__HNVN ==
			p_vid_path_param->hv_sync_pol)) {

			p_vid_path_param->input_video_timing.h_blank =
				vp__fdet_hback_count + vp__fdet_hfront_count
				+ vp__fdet_hsync_high_count;
		}

	}

	{
		uint16_t vp__fdet_line_count;

		sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__FDET_LINE_COUNT,
			(uint8_t *)&vp__fdet_line_count, 2);

		p_vid_path_param->input_video_timing.v_active =
			vp__fdet_line_count;
	}

	{
		uint16_t vp__fdet_pixel_count;

		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_PIXEL_COUNT,
			(uint8_t *)&vp__fdet_pixel_count, 2);

		p_vid_path_param->input_video_timing.h_active =
			vp__fdet_pixel_count;
	}

	{
		uint16_t vp__fdet_vsync_low_count_even = 0;
		uint16_t vp__fdet_vsync_high_count_even = 0;
		uint16_t vp__fdet_vsync_low_count_odd = 0;
		uint16_t vp__fdet_vsync_high_count_odd = 0;
		uint16_t vp__fdet_vfront_count_even = 0;
		uint16_t vp__fdet_vfront_count_odd = 0;
		uint16_t vp__fdet_vback_count_even = 0;
		uint16_t vp__fdet_vback_count_odd = 0;
		uint32_t vsync_even = 0;
		uint32_t vsync_odd = 0;

		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_VSYNC_LOW_COUNT_EVEN,
			(uint8_t *)&vsync_even, 4);
		vp__fdet_vsync_high_count_even = vsync_even & 0xFFFF;
		vp__fdet_vsync_low_count_even = (vsync_even >> 16) & 0xFFFF;

		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_VSYNC_LOW_COUNT_ODD,
			(uint8_t *)&vsync_odd, 4);
		vp__fdet_vsync_high_count_odd = vsync_odd & 0xFFFF;
		vp__fdet_vsync_low_count_odd = (vsync_odd >> 16) & 0xFFFF;

		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_VFRONT_COUNT_EVEN,
			(uint8_t *)&vp__fdet_vfront_count_even, 2);
		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_VBACK_COUNT_EVEN,
			(uint8_t *)&vp__fdet_vback_count_even, 2);
		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_VFRONT_COUNT_ODD,
			(uint8_t *)&vp__fdet_vfront_count_odd, 2);
		sii_mod_vid_path_reg_read(pstDev,
			REG_ADDR__VP__FDET_VBACK_COUNT_ODD,
			(uint8_t *)&vp__fdet_vback_count_odd, 2);

		p_vid_path_param->input_video_timing.v_front =
			vp__fdet_vfront_count_even + vp__fdet_vfront_count_odd;
		p_vid_path_param->input_video_timing.v_back =
			vp__fdet_vback_count_even + vp__fdet_vback_count_odd;

		if ((SII_DRV_HV_SYNC_POL__HPVP ==
			p_vid_path_param->hv_sync_pol)
			|| (SII_DRV_HV_SYNC_POL__HNVP ==
			p_vid_path_param->hv_sync_pol)) {

			p_vid_path_param->input_video_timing.v_sync =
				vp__fdet_vsync_low_count_even
				+ vp__fdet_vsync_low_count_odd;
		} else if ((SII_DRV_HV_SYNC_POL__HPVN ==
			p_vid_path_param->hv_sync_pol)
			|| (SII_DRV_HV_SYNC_POL__HNVN ==
			p_vid_path_param->hv_sync_pol)) {

			p_vid_path_param->input_video_timing.v_sync =
				vp__fdet_vsync_high_count_even
				+ vp__fdet_vsync_high_count_odd;
		}
		p_vid_path_param->input_video_timing.v_blank =
			p_vid_path_param->input_video_timing.v_front
			+ p_vid_path_param->input_video_timing.v_back
			+ p_vid_path_param->input_video_timing.v_sync;
	}
	 return 0;
}

int sii_mod_vid_path_intr_handler(void *vidpath_obj, bool rx_pll_status)
{
	uint32_t vp__fdet_irq_stat = 0;
	struct video_path_param *p_vid_path_param =
		(struct video_path_param *)vidpath_obj;
    pstSii9630_Device_t pstDev = p_vid_path_param->pstDevice;

	if (!p_vid_path_param)
		return 1;

	p_vid_path_param->events_flag = 0;

	SELECT_VIDEO_PATH_PAGE(PAGE_CORE);
	sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__FDET_IRQ_STATUS,
		(uint8_t *)&vp__fdet_irq_stat,
		BIT_MASK__VP__FDET_IRQ_STAT__SIZE);
	sii_mod_vid_path_reg_write(pstDev, REG_ADDR__VP__FDET_IRQ_STATUS,
		(uint8_t *)&vp__fdet_irq_stat,
		BIT_MASK__VP__FDET_IRQ_STAT__SIZE);

	if (!rx_pll_status)
		/* Got spurious Videopath Interrupts, So not serving*/
		return 0;

	/*sii_log_debug("SWARUPA changed\n");*/

	/* The video timings are not stable right after videopath interrupts.
	 * So to update these timngs in videopath module we have separate
	 * logic to read and store whenever videopath module is stable.
	 * see inst_timer_stable_video_timings timer for this logic.
	 */

	if (vp__fdet_irq_stat
		& (BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_POLARITY
			| BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_POLARITY)) {
		sii_log_debug("HSYNC/VSYNC changed\n");
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__SYNC_CHANGE;
	}
	if (vp__fdet_irq_stat & BIT_MSK__VP__FDET_IRQ_STATUS__FRAME_RATE) {
		uint32_t vp__fdet_frame_rate = 0x00000000;

		sii_mod_vid_path_reg_read(pstDev, REG_ADDR__VP__FDET_FRAME_RATE,
			(uint8_t *)&vp__fdet_frame_rate, 3);
		if (0 == vp__fdet_frame_rate)
			return 0;
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG;
	}

	if (vp__fdet_irq_stat
		& (BIT_MSK__VP__FDET_IRQ_STATUS__HBACK_COUNT
			| BIT_MSK__VP__FDET_IRQ_STATUS__HFRONT_COUNT
			| BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_HIGH_COUNT
			| BIT_MSK__VP__FDET_IRQ_STATUS__HSYNC_LOW_COUNT)) {

		/*sii_log_debug("HBACK/HFRONT/HSYNC_HIGH count changed\n");*/
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG;
		/* Specific HSYNC High count change notification */
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__HSYNC_HIGH_CNT_CHG;
	}

	/*if (vp__fdet_irq_stat & BIT_MSK__VP__FDET_IRQ_STATUS__INTERLACED) {
		sii_log_debug("Interlaced changed\n");
		;
	}*/

	if (vp__fdet_irq_stat & BIT_MSK__VP__FDET_IRQ_STATUS__LINE_COUNT) {

		sii_log_debug("LINE count changed\n");
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG;
	}
	if (vp__fdet_irq_stat & BIT_MSK__VP__FDET_IRQ_STATUS__PIXEL_COUNT) {

		/*sii_log_debug("PIXEL count changed\n");*/
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG;
	}
	if (vp__fdet_irq_stat
		& (BIT_MSK__VP__FDET_IRQ_STATUS__VBACK_COUNT_EVEN
		| BIT_MSK__VP__FDET_IRQ_STATUS__VBACK_COUNT_ODD
		| BIT_MSK__VP__FDET_IRQ_STATUS__VFRONT_COUNT_EVEN
		| BIT_MSK__VP__FDET_IRQ_STATUS__VFRONT_COUNT_ODD
		| BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_HIGH_COUNT_EVEN
		| BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_LOW_COUNT_EVEN
		| BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_HIGH_COUNT_ODD
		| BIT_MSK__VP__FDET_IRQ_STATUS__VSYNC_LOW_COUNT_ODD)) {

		sii_log_debug("VBACK/VFRONT/VSNC_HIGH count EVEN changed\n");
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG;
	}
	if (vp__fdet_irq_stat & BIT_MSK__VP__FDET_IRQ_STATUS__VIDEO656) {

		sii_log_debug("VIDEO656 changed\n");
		p_vid_path_param->events_flag |=
			SII_DRV_RX_VIDPATH_EVENT__IP_VID_PARAMS_CHNG;
	}
	if (p_vid_path_param->event_notify_fn
		&& p_vid_path_param->events_flag) {

		p_vid_path_param->event_notify_fn(
			&g_stdrv_obj,
			p_vid_path_param->events_flag);
		p_vid_path_param->events_flag = 0;
	}
	return 0;
}

#endif

