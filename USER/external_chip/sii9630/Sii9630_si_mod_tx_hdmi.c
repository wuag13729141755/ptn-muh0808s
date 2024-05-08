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

#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_drv_cp8630.h"
#include "Sii9630_si_mod_tx_hdmi.h"
#include "Sii9630_si_mod_tx_scdc.h"
#include "Sii9630_si_lib_edid.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_lib_edid.h"
#include "Sii9630_si_timer.h"

#define TIMER_START__TX_INTR				0
#define TIMER_START__TX_INTR_PRI			255
#define TIMER_START__TX_INTR_REPEAT			100

#define TIMER_START__HW_UPDATE				100
#define TIMER_START__HW_UPDATE_PRI			254
#define TIMER_START__HW_UPDATE_200MS_WAIT		200
#define TIMER_START__HW_UPDATE_50MS_WAIT		50
#define TIMER_START__HW_UPDATE_5MS_WAIT			5
#define TIMER_START__HW_UPDATE_100MS_WAIT		100

#define LEN_TPI__IF_BUFFER_LENGTH			31

/* page 6:0xb_f - REG_ADDR__TPI_INFO_FSEL*/
#define BIT_ENUM__TPI_INFO_FSEL__AVI			0x00
#define BIT_ENUM__TPI_INFO_FSEL__GBD			0x01
#define BIT_ENUM__TPI_INFO_FSEL__AUDIO			0x02
#define BIT_ENUM__TPI_INFO_FSEL__SPD			0x03
#define BIT_ENUM__TPI_INFO_FSEL__MPEG			0x04
#define BIT_ENUM__TPI_INFO_FSEL__VSIF			0x05
/* use this for ISRC*/
#define BIT_ENUM__TPI_INFO_FSEL__GEN1			0x06
/* use this for ISRC2*/
#define BIT_ENUM__TPI_INFO_FSEL__GEN2			0x07
/* use this for ACP*/
#define BIT_ENUM__TPI_INFO_FSEL__GEN3			0x08

#define BIT_ENUM__TPI_INFO_FSEL__GEN4			0x09
#define BIT_ENUM__TPI_INFO_FSEL__GEN5			0x0A
#define BIT_ENUM__TPI_INFO_FSEL__VSIF1			0x0B
#define BIT_ENUM__TPI_INFO_FSEL__VSIF2			0x0C
#define BIT_ENUM__TPI_INFO_FSEL__VSIF3			0x0D
#define BIT_ENUM__TPI_INFO_FSEL__VSIF4			0x0E


static void s_virtual_isr_handler(struct hdmi_tx_obj *p_obj, uint8_t intreg);
static void s_rsense_update(struct hdmi_tx_obj *p_obj);
static void s_plug_status_update(struct hdmi_tx_obj *p_obj);
static enum ddc_com_err s_update_edid(struct hdmi_tx_obj *p_obj,
	uint8_t segment_index, uint8_t reg_addr,
	uint8_t *p_buf, uint16_t length);
static bool s_wait_for_ddc_bus(struct hdmi_tx_obj *p_obj);
/*static enum sii_lib_edid_err_code s_parse_edid(struct hdmi_tx_obj *p_obj);*/
static void s_apply_info_frame(struct hdmi_tx_obj *p_obj, uint8_t b_apply);
/*static void s_infoframes_set(struct hdmi_tx_obj *p_obj);*/
static void s_infoframe_data_set(struct hdmi_tx_obj *p_obj,
					struct sii_info_frame *p_info_frame);
static void s_update_infoframes(struct hdmi_tx_obj *p_obj,
		enum sii_info_frame_id info_id);
/*static void s_audioformat_set(struct hdmi_tx_obj *p_obj);*/
/*static void s_update_audioformat(struct hdmi_tx_obj *p_obj);*/
/*static void s_channel_state_set(struct hdmi_tx_obj *p_obj);*/
/*static void s_update_channel_state(struct hdmi_tx_obj *p_obj);*/
static void s_put_tmds_on_off(struct hdmi_tx_obj *p_obj, bool on_off);
static void s_tmds_mode_set(struct hdmi_tx_obj *p_obj);
static void s_av_mute_set(struct hdmi_tx_obj *p_obj, uint8_t b_av_mute);
/*static void s_update_av_mute(struct hdmi_tx_obj *p_obj);*/
static void s_update_hot_plug(struct hdmi_tx_obj *p_obj);
static void s_update_rsense(struct hdmi_tx_obj *p_obj);
static void s_update_output_bit_depth(struct hdmi_tx_obj *p_obj,
					enum sii_drv_bit_depth bit_depth);
static void s_dump_buffer(uint8_t *p_data, uint16_t len);
/*static enum sii_lib_edid_err_code s_parse_edid(struct hdmi_tx_obj *p_obj);*/
/* notify functions*/
static void s_notify_hdmi_state(struct hdmi_tx_obj *p_obj);
static void s_ddc_clear_fifo(struct hdmi_tx_obj *p_obj, bool b_ddc_clear_en);
static void s_mod_tx_hdmi_clear_info_frame(struct hdmi_tx_obj *p_obj,
	enum sii_info_frame_id if_id);
static void s_drop_pkt(struct hdmi_tx_obj *p_obj,
	enum sii_info_frame_id if_id, bool clear);

static void reset_states(struct hdmi_tx_obj *p_hdmi_obj)
{
	/* initialize user request states */
	p_hdmi_obj->tmds_mode = SII_TMDS_MODE__NONE;
	p_hdmi_obj->tmds_mode = SII_TMDS_MODE__NONE;
	p_hdmi_obj->prev_tmds_mode = SII_TMDS_MODE__NONE;
	p_hdmi_obj->hv_sync_pol = SII_DRV_HV_SYNC_POL__HPVP;
	p_hdmi_obj->e_state = SII_DRV_HDMI_EXT_STATUS__TMDS_OFF;
	p_hdmi_obj->prev_i_state = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
	p_hdmi_obj->i_state = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
	p_hdmi_obj->b_tmds_on_req = FALSE;

	/* set this to false, so it will check the h/w status*/
	p_hdmi_obj->b_init_hot_plug = false;

	/* initialize user status */
	p_hdmi_obj->b_hot_plug = false;
	p_hdmi_obj->b_rsen = false;

	/* initialize interrupts status	*/
	p_hdmi_obj->int_stat.reg0 = 0x00;
	p_hdmi_obj->int_stat.reg1 = 0x00;

	/* clear infoframes*/
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__AVI);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__AUDIO);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__VS);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__SPD);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__GBD);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__MPEG);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__ISRC);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__ISRC2);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__GCP);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__ACP);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__ACR);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj,
		SII_INFO_FRAME_ID__AUD_METDAT);
#ifdef CONFIG_DSC_EN
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__VSIF1);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__VSIF2);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__VSIF3);
	s_mod_tx_hdmi_clear_info_frame(p_hdmi_obj, SII_INFO_FRAME_ID__VSIF4);
#endif
}

static struct hdmi_tx_obj sg_st_hdmi_obj;
/* public functions */
void *sii_mod_tx_hdmi_init(pstSii9630_Device_t pstDevice,
				void (*event_notify_fn)(void *, uint32_t))
{
	struct hdmi_tx_obj *p_hdmi_obj = &sg_st_hdmi_obj;
	/*int tstatus = 0;*/

	/* allocate memory for hdmi_tx object */
	//p_hdmi_obj = kzalloc(sizeof(struct hdmi_tx_obj), GFP_KERNEL);
    memset(&sg_st_hdmi_obj,0,sizeof(struct hdmi_tx_obj));

	if (!p_hdmi_obj)
		return NULL;
    //sii_log_debug("Hdmi Tx Init start\n");

	//p_hdmi_obj->drv_context = drv_context;
	p_hdmi_obj->pstDevice = pstDevice;
	p_hdmi_obj->event_notify_fn = event_notify_fn;

#ifdef CONFIG_SII9630_HDMI_TX
	{
		/* creating SCDC object*/
		p_hdmi_obj->tx_scdc_obj = sii_mod_tx_scdc_init(pstDevice,
						event_notify_fn);
		if (!p_hdmi_obj)
			goto free_scdc;

	}
#endif
	goto done;

#ifdef CONFIG_SII9630_HDMI_TX
free_scdc:
	 //kfree(p_hdmi_obj);
#endif
	return NULL;
done:

    //sii_log_debug("Hdmi Tx Init End\n");
	return p_hdmi_obj;
}

#if 0
void sii_mod_tx_hdmi_exit(void *p_obj)
{
	struct hdmi_tx_obj *p_hdmi_obj = (struct hdmi_tx_obj *)p_obj;

/*	if (p_hdmi_obj->tx_scdc_obj)
		sii_mod_tx_scdc_exit(p_hdmi_obj->tx_scdc_obj);*/
	kfree(p_hdmi_obj);
}
#endif

int init_hdmi_tx_regs(void *p_obj)
{
	struct hdmi_tx_obj *p_hdmi_obj = (struct hdmi_tx_obj *)p_obj;
    pstSii9630_Device_t pstDev = p_hdmi_obj->pstDevice;
	int status = 0;

	reset_states(p_hdmi_obj);
	/* enable packets pass-through mode*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DIPT_CNTL,
		BIT_MSK__DIPT_CNTL__REG_PKT_BYP_MODE);
	/* enable HDMI on TX output*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TEST_TXCTRL,
		BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
	/*HDMI mode for harry PHY*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__MHL_TOP_CTL,
		BIT_MSK__MHL_TOP_CTL__REG_HDMI_EN);
	/*hdmi_clk_freq = 1*pixelclock*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_ZONE_CTL4, 0x04);

	sii_platform_clr_bit8(pstDev, REG_ADDR__ACR_CTRL ,
		BIT_MSK__ACR_CTRL__REG_CTS_REQ_EN);

	sii_platform_clr_bit8(pstDev, REG_ADDR__HARRY_CTRL_1,
			BIT_MSK__HARRY_CTRL_1__REG_DP_OE1);

	/* enable intr1 interrupts.*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__INTR1_MASK,
		BIT_MSK__INTR1_MASK__REG_INTR1_MASK5
		| BIT_MSK__INTR1_MASK__REG_INTR1_MASK6);

	init_scdc_tx_regs(p_hdmi_obj->tx_scdc_obj);

	return status;
}

bool sii_mod_tx_hdmi_interrupt_handler(void *p_hdmi_obj)
{

	struct hdmi_tx_obj *p_obj;
    pstSii9630_Device_t pstDev;// = p_hdmi_obj->pstDevice;
	struct sii_hdmi_int_stat int_stat;
	uint8_t mask;
	uint8_t l1_intr_stat_0 = 0;
	uint8_t bit_msk_l1_stat_B6 = 0;

	p_obj = (struct hdmi_tx_obj *)p_hdmi_obj;
	if (!p_obj)
		return FALSE;

    pstDev = p_obj->pstDevice;

	l1_intr_stat_0 = sii_platform_rd_reg8(pstDev, REG_ADDR__L1_INTR_STAT_0);
	bit_msk_l1_stat_B6 = BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B6;

	if ((l1_intr_stat_0 & bit_msk_l1_stat_B6))
		sii_mod_tx_scdc_interrupt_handler(p_obj->tx_scdc_obj);

	/* capture and mask HPD(STAT6) and RSEN(STAT5) interrupt status bits.*/
	mask = BIT_MSK__INTR1__REG_INTR1_STAT6
		| BIT_MSK__INTR1__REG_INTR1_STAT5;

	int_stat.reg0 = sii_platform_rd_reg8(pstDev, REG_ADDR__INTR1);
	int_stat.reg0 &= mask;

	if (memcmp(&p_obj->int_stat, &int_stat,
		sizeof(struct sii_hdmi_int_stat))) {
		uint8_t intreg = 0;

		/* find any interrupt status bit that changed to '1'*/
		intreg |= ((p_obj->int_stat.reg0 ^ int_stat.reg0)
			& int_stat.reg0);

		p_obj->int_stat = int_stat;
		/* clear all pending hardware TX interrupts*/
		if (p_obj->int_stat.reg0) {
			sii_platform_wr_reg8(pstDev, REG_ADDR__INTR1,
				p_obj->int_stat.reg0);
			p_obj->int_stat.reg0 = 0x00;
		}
		if (intreg) {
			/* call derived interupt handler*/
			s_virtual_isr_handler(p_obj, intreg);
		}
	}
	return TRUE;
}

int sii_mod_tx_hdmi_set(void *p_hdmi_obj,
	enum sii_mod_hdmi_tx_internal_opcode opcode, const void *in_data)
{
	struct hdmi_tx_obj *p_obj = (struct hdmi_tx_obj *)p_hdmi_obj;
	int status = 0;

	if (p_obj) {
		switch (opcode) {
		case SII_MOD_HDMI_TX_OPCODE__TMDS_MODE:
			memcpy(&p_obj->tmds_mode, in_data,
				sizeof(enum sii_tmds_mode));
			s_tmds_mode_set(p_obj);
			break;
		case SII_MOD_HDMI_TX_OPCODE__AVMUTE:
			memcpy(&p_obj->b_av_mute, in_data, sizeof(bool));
			s_av_mute_set(p_obj, p_obj->b_av_mute);
			break;
		case SII_MOD_HDMI_TX_OPCODE__IF_DATA:
		{
			struct sii_info_frame *p_infoframe =
				(struct sii_info_frame *)in_data;

			s_infoframe_data_set(p_obj, p_infoframe);
			break;
		}
		case SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH:
		{
			enum sii_drv_bit_depth bit_depth =
				*(enum sii_drv_bit_depth *)in_data;

			s_update_output_bit_depth(p_obj, bit_depth);
			break;
		}
		case SII_MOD_HDMI_TX_OPCODE__HW_UPDATE_START:
		{
			/*sii_timer_start(&p_obj->timer_hw_update,
				TIMER_START__HW_UPDATE_5MS_WAIT);*/
			break;
		}
		case SII_MOD_HDMI_TX_OPCODE__EDID:
		{
			struct sii_edid *p_edid =
				(struct sii_edid *)in_data;

			p_obj->edid.size = p_edid->size;
			memcpy(&p_obj->edid.b, &p_edid->b, p_edid->size);
			sii_lib_edid_parse(&p_obj->parse_edid,
					   p_obj->edid.b);
			break;
		}
		case SII_MOD_HDMI_TX_OPCODE__SCDC_ENABLE_READ_REQ_TEST:
			{
			uint8_t *requestdelay = (uint8_t *)in_data;

			sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__ENABLE_READ_REQ_TEST,
				requestdelay);
			break;
			}
		case SII_MOD_HDMI_TX_OPCODE__SCDC_READ_REQ_CAP_SET:
			{
			uint8_t *readreqenable = (uint8_t *)in_data;
			sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__READ_REQ_CAP_SET,
				readreqenable);
			break;
			}
		case SII_MOD_HDMI_TX_OPCODE__DDC_RESET:
			s_ddc_clear_fifo(p_obj, *((bool *)(in_data)) ?
				true : false);
			break;
		case SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF:
		{
			bool tmds_on = *((bool *)(in_data)) ? true : false;

			s_put_tmds_on_off(p_obj, tmds_on);
			break;
		}
		case SII_MOD_HDMI_TX_OPCODE__SCDC_TMDS_CLOCK_SET:
#ifdef CONFIG_SII9630_HDMI_TX
		{
			uint32_t *tmds_clk = (uint32_t *)in_data;

			sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__SCDC_TMDS_CLK_SET,
				tmds_clk);
		}
#endif
			break;
		case SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_TIMEOUT_SET:
#ifdef CONFIG_SII9630_HDMI_TX
		{
			bool *scramble_timout = (bool *)in_data;

			sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__SCDC_SCRAMBLE_TIMEOUT_SET,
				scramble_timout);
		}
#endif
			break;
		case SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE:
#ifdef CONFIG_SII9630_HDMI_TX
		{
			uint8_t scrambleindata = (*(uint8_t *)in_data) ?
				true : false;

			sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__SCDC_SCRAMBLE_EN,
				&scrambleindata);
			}
#endif
			break;
#ifdef CONFIG_ENHANCED_MODE_PROT
		case SII_MOD_HDMI_TX_OPCODE__SCDC_XMIT_EMM:
			sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__SCDC_XMIT_EMM,
				in_data);
			break;
#endif
		default:
			status = 1;
			break;
		}
		return status;
	}
	return 1;
}

int sii_mod_tx_hdmi_get(void *p_hdmi_obj,
	enum sii_mod_hdmi_tx_internal_opcode opcode, void *out_data)
{
	struct hdmi_tx_obj *p_obj = (struct hdmi_tx_obj *)p_hdmi_obj;
	uint16_t cec_addr = 0;
	int status = 0;

	if (p_obj) {
		switch (opcode) {
		case SII_MOD_HDMI_TX_OPCODE__EDID:
			if (p_obj->b_is_ds_edid_available) {
				memcpy(out_data, &(p_obj->edid),
					sizeof(struct sii_edid));
			} else
				status = 1;
			break;
		case SII_MOD_HDMI_TX_OPCODE__HOTPLUG:
			memcpy(out_data, &(p_obj->b_hot_plug), sizeof(bool));
			break;
		case SII_MOD_HDMI_TX_OPCODE__RSEN:
			memcpy(out_data, &(p_obj->b_rsen), sizeof(bool));
			break;
		case SII_MOD_HDMI_TX_OPCODE__HDMI_STATE:
			memcpy(out_data, &(p_obj->e_state), sizeof(enum
				sii_drv_hdmi_tx_external_state));
			break;
		case SII_MOD_HDMI_TX_OPCODE__TMDS_MODE:
			memcpy(out_data, &(p_obj->tmds_mode),
				sizeof(enum sii_tmds_mode));
			break;
		case SII_MOD_HDMI_TX_OPCODE__CEC_PHY_ADDR:
			cec_addr = ((p_obj->parse_edid.vsdb.cec_addr[0] << 12) |
				(p_obj->parse_edid.vsdb.cec_addr[1] << 8) |
				(p_obj->parse_edid.vsdb.cec_addr[2] << 4) |
				p_obj->parse_edid.vsdb.cec_addr[3]);
			memcpy(out_data, &(cec_addr), sizeof(uint16_t));
			break;
		case SII_MOD_HDMI_TX_OPCODE__EDID_LIPSYNC:
			memcpy(out_data, &(p_obj->parse_edid.lip_sync),
				sizeof(struct sii_edid_lip_sync));
			break;
		case SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_MANF_STATUS:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__PEER_MANF_STATUS,
				out_data);
			break;
		case SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_REG_STATUS:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__PEER_SCDC_STATUS,
				out_data);
			break;
		case SII_MOD_HDMI_TX_OPCODE__SCDC_SOURCE_SCRAMBLE_STATUS:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__SOURCE_SCRAMBLE_STATUS,
				out_data);
			break;
		case SII_MOD_HDMI_TX_OPCODE__READ_UPDATE_REGISTER:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__READ_UPDATE_REGISTER,
				out_data);
		break;
		case SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF:
			memcpy(out_data, &p_obj->b_tmds_on_req, sizeof(bool));
			break;
		case SII_MOD_HDMI_TX_OPCODE__DSC_COMPRESSION_STATUS:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__SOURCE_SCRAMBLE_STATUS,
				out_data);
			break;

#ifdef CONFIG_ENHANCED_MODE_PROT
		case SII_MOD_HDMI_TX_OPCODE__DS_EM_CAPS:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__DS_EM_CAPS,
				out_data);
			break;
		case SII_MOD_HDMI_TX_OPCODE__DS_EMM_RESP:
			sii_mod_tx_scdc_get(p_obj->tx_scdc_obj,
				SII_MOD_SCDC_OPCODE__DS_EMM_RESP,
				out_data);
			break;
#endif
		default:
			status = 1;
			break;
		}
		return status;
	}
	return 1;
}

int sii_mod_tx_hdmi_stop_timers(void *p_obj)
{
	struct hdmi_tx_obj *p_hdmi_obj = (struct hdmi_tx_obj *)p_obj;
	int status = 0;
	if (!p_hdmi_obj)
		return 1;

	/* stop all timers */
	if (p_hdmi_obj->tx_scdc_obj)
		status = sii_mod_tx_scdc_stop_timers(p_hdmi_obj->tx_scdc_obj);

	return status;
}

static void s_mod_tx_hdmi_clear_info_frame(struct hdmi_tx_obj *p_obj,
						enum sii_info_frame_id if_id)
{
	if (if_id < SII_INFO_FRAME_ID__TOTAL) {
		p_obj->if_data[if_id].is_if_on = false;
		memset(p_obj->if_data[if_id].b, 0, SII_INFOFRAME_MAX_LEN);
		p_obj->if_data[if_id].if_id = if_id;
	}
}

static void s_virtual_isr_handler(struct hdmi_tx_obj *p_obj, uint8_t intreg)
{
	if (intreg & BIT_MSK__INTR1__REG_INTR1_STAT5)
		s_rsense_update(p_obj);
	/* donot handle HDMI interrupts in MHL2/3 mode. will be called
	 * after receiving set_hpd*/
	if (intreg & BIT_MSK__INTR1__REG_INTR1_STAT6)
		s_plug_status_update(p_obj);

}

static void s_set_sw_tpi_bit(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;

	sii_log_debug("Set SW TPI bit!!\n");
	sii_platform_set_bit8(pstDev, REG_ADDR__LM_DDC,
		BIT_MSK__LM_DDC__REG_SW_TPI_EN);
}

static void s_reset_tpi_state_machine(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;

	sii_log_debug("Reset TPI state machine(hdmi)\n");
	sii_platform_set_bit8(pstDev, REG_ADDR__TPI_HW_OPT0,
		BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_HW_OPT0,
		BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);

	sii_platform_set_bit8(pstDev, REG_ADDR__SCDC_CTL,
		0x80);
	sii_platform_clr_bit8(pstDev, REG_ADDR__SCDC_CTL,
		0x80);
}

/*! @brief	  update rx_sense status */
static void s_rsense_update(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t sys_status = 0;

	sys_status = sii_platform_rd_reg8(pstDev, REG_ADDR__TPI_HPD_RSEN);

	if (sys_status & BIT_MSK__TPI_HPD_RSEN__RSEN) {
		if ((sys_status
			& BIT_MSK__TPI_HPD_RSEN__RSEN_STATE) == 0x40) {
				p_obj->b_rsen = true;
		} else if ((sys_status
			& BIT_MSK__TPI_HPD_RSEN__RSEN_STATE) == 0x20) {
				/* try again */
				msleep(100);
				sys_status = sii_platform_rd_reg8(pstDev,
					REG_ADDR__TPI_HPD_RSEN);
				if ((sys_status	& 0x60) == 0x40)
					p_obj->b_rsen = true;
				else
					p_obj->b_rsen = false;
		} else {
			p_obj->b_rsen = false;
		}
	} else {
		p_obj->b_rsen = false;
	}

	s_update_rsense(p_obj);
}

/*! @brief	  update HPD status */
static void s_plug_status_update(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	bool b_hot_plug = p_obj->b_hot_plug;
	uint8_t extensions = 0;
	uint8_t sys_status = 0;
	enum ddc_com_err ddc_err;
	enum sii_lib_edid_err_code_t edid_error =
			SII_LIB_EDID_ERR_CODE__NO_ERROR;
    struct sii_edid l_stedid;
	struct sii_edid *edid = &l_stedid;//kzalloc(sizeof(struct sii_edid), GFP_KERNEL);
	uint8_t hpd_mask = 0;
	bool missed_hpd = false;

    memset(&l_stedid,0,sizeof(struct sii_edid));
	p_obj->b_is_ds_edid_available = false;

	hpd_mask = ((BIT_MSK__TPI_HPD_RSEN__HPD_STATE & 0x04)
		| BIT_MSK__TPI_HPD_RSEN__HPD);

	/* donot read HPD(HDMI) status if MHL is enabled.*/
	sys_status = sii_platform_rd_reg8(pstDev, REG_ADDR__TPI_HPD_RSEN);

	if ((sys_status & hpd_mask) == hpd_mask) {

		b_hot_plug = true;

		if (p_obj->b_hot_plug == true) {
			sii_log_err("Missed HPD.\n");
			b_hot_plug = false;
			missed_hpd = true;
		}
	} else {
		b_hot_plug = false;
	}

	if (b_hot_plug) {
        struct sii_lib_edid_parsed_data stds_edid;
		struct sii_lib_edid_parsed_data *ds_edid = &stds_edid;
			//malloc(sizeof(struct sii_lib_edid_parsed_data));

        memset(&stds_edid,0,sizeof(struct sii_lib_edid_parsed_data));

		sii_log_info("HOT PLUG asserted\n");

		/* read EDID*/
		memset(&p_obj->edid, 0, sizeof(struct sii_edid));
		s_set_sw_tpi_bit(p_obj);
		s_reset_tpi_state_machine(p_obj);
		/* read block# 0*/
		ddc_err = s_update_edid(p_obj, 0, 0, &(edid->b[0]),
			SII_EDID_BLOCK_SIZE);
		if (ddc_err == SI_TX_DDC_ERROR_CODE_NO_ERROR) {
			/*find how many extension blocks are there,
			 * read them and store.*/
			extensions = edid->b[0x7E];
			edid->size = SII_EDID_BLOCK_SIZE;

			if (extensions >= SII_EDID_MAX_BLOCKS) {
				sii_log_debug("****Warning****:: EDID:");
				sii_log_debug(" DS supports %d Extensions.\n",
					extensions);
				sii_log_debug(
					"But Banner Supports %d extensions!!\n",
					(SII_EDID_MAX_BLOCKS - 1));
				extensions = (SII_EDID_MAX_BLOCKS - 1);
			}

            /* 2 extension blocks is impossible*/
            /* either 1 or 3, 4 etc */
            if (extensions == 2) {
				extensions = 1;
            }

			if (extensions) {
				ddc_err = s_update_edid(p_obj, 0,
				SII_EDID_BLOCK_SIZE,
				&(edid->b[SII_EDID_BLOCK_SIZE]),
				(SII_EDID_BLOCK_SIZE));

				if (ddc_err == SI_TX_DDC_ERROR_CODE_NO_ERROR)
						edid->size +=
						SII_EDID_BLOCK_SIZE;

				extensions--;

				if ((extensions) && (ddc_err ==
					SI_TX_DDC_ERROR_CODE_NO_ERROR)) {
					uint8_t segment = 0;
					uint16_t size = 0;
					uint8_t *p_edid_b =
						&edid->b[2*SII_EDID_BLOCK_SIZE];

					for (; extensions > 0;
						extensions -= 2) {
						segment++;
					if ((extensions % 2) == 0)
						size = 2 * SII_EDID_BLOCK_SIZE;
					else
						size = SII_EDID_BLOCK_SIZE;

					ddc_err = s_update_edid(p_obj, segment,
							0,	p_edid_b, size);

					if (ddc_err ==
						SI_TX_DDC_ERROR_CODE_NO_ERROR)
							edid->size += size;

						p_edid_b += size;
					}
				}
			}
		}

		if (ddc_err == SI_TX_DDC_ERROR_CODE_NO_ERROR) {
			sii_log_debug("EDID has been read successfully!!\n");

			/* Save Composed EDID in the object */
			p_obj->edid.size = edid->size;
			memcpy(p_obj->edid.b, edid->b, edid->size);

			/* flag to represent EDID has read succesfully*/
			p_obj->b_is_ds_edid_available = true;

			sii_log_debug("DownStream EDID::\n");
			s_dump_buffer(&p_obj->edid.b[0], p_obj->edid.size);

			/* parse downstream EDID*/
			edid_error = sii_lib_edid_parse(ds_edid, (edid->b));

			{
			enum sii_info_frame_id acp = SII_INFO_FRAME_ID__ACP;
			enum sii_info_frame_id isrc1 = SII_INFO_FRAME_ID__ISRC;
			enum sii_info_frame_id isrc2 = SII_INFO_FRAME_ID__ISRC2;

			if (ds_edid->vsdb.b_supports_ai) {
				s_drop_pkt(p_obj, acp, false);
				s_drop_pkt(p_obj, isrc1, false);
				s_drop_pkt(p_obj, isrc2, false);

			} else {
				s_drop_pkt(p_obj, acp, true);
				s_drop_pkt(p_obj, isrc1, true);
				s_drop_pkt(p_obj, isrc2, true);
			}
			}
			/* Set Sink Caps to SCDC module if
			 * DS supports SCDC*/
			if (!edid_error) {
				struct sii_lib_edid_hfvsdb_t hfvsdb;

				hfvsdb = ds_edid->hfvsdb;
				sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
					SII_MOD_SCDC_OPCODE__SCDC_SNK_CAP,
					&hfvsdb);
			}
		} else {
			sii_log_debug("Error happened during EDID read!!\n");
			p_obj->b_is_ds_edid_available = false;
		}

		//kfree(ds_edid);

	} else {
		sii_log_info("HOT PLUG de-asserted\n");

		sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
			SII_MOD_SCDC_OPCODE__SCDC_RESET_CAP,
			NULL);


		/* Reset DDC whenever HPD is de-asserted to
			avoid DDC hanging issues*/
		/*sii_platform_set_bit8(pdata, REG_ADDR__TPI_HW_OPT0,
			BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);
		sii_platform_clr_bit8(pdata, REG_ADDR__TPI_HW_OPT0,
			BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);*/
		/*sii_platform_clr_bit8(pdata, REG_ADDR__DDC_CMD, 0x7F);*/
		sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG1,
			BIT_MSK__SW_RST_REG1__REG_TX_DDCM2_SW_RST
			| BIT_MSK__SW_RST_REG1__REG_TX_DDCM1_SW_RST);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG1,
			BIT_MSK__SW_RST_REG1__REG_TX_DDCM2_SW_RST
			| BIT_MSK__SW_RST_REG1__REG_TX_DDCM1_SW_RST);

	}

	if ((b_hot_plug != p_obj->b_hot_plug)
		|| p_obj->b_is_ds_edid_available) {
		p_obj->b_hot_plug = b_hot_plug;
		s_update_hot_plug(p_obj);
	}

	//kfree(edid);
	if (missed_hpd) {
		sii_log_err("Calling HPD update handler\n");
		s_plug_status_update(p_obj);
	}
}

static enum ddc_com_err s_update_edid(struct hdmi_tx_obj *p_obj,
	uint8_t segment_index, uint8_t reg_addr,
	uint8_t *p_buf, uint16_t length)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	enum ddc_com_err ds_ddc_error = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint16_t fifo_size;
	uint16_t time_out_ms;
	bool loop = false;
	bool flag = true;

	do {
		if (length == 0)
			break;

		if (!p_buf)
			break;

		if (!s_wait_for_ddc_bus(p_obj)) {
            s_ddc_clear_fifo(p_obj, true);
			sii_log_debug("DDC error:: busy. try later\n");
			ds_ddc_error = SI_TX_DDC_ERROR_CODE_BUSY;
			return ds_ddc_error;
		}

		sii_platform_set_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
			BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_ADDR,
			(0xFF != segment_index) ? 0xA0 : 0x74);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_SEGM,
			(0xFF != segment_index) ? segment_index : 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_OFFSET, reg_addr);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT2,
			((length >> 8) & 0xFF));
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT1,
			(length & 0xFF));
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			BIT_ENUM__DDC_CMD__CLEAR_FIFO);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			segment_index ?
			BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ :
			BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);

		/*sii_platform_wr_reg8(pdata, REG_ADDR__DDC_CMD,
			BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);*/

		/* timeout is proportional to length*/
		time_out_ms = length + 3;

		/* wait until the FIFO is filled with several bytes*/
		/* also makes time aligning*/
		usleep(2000);

		do {
			fifo_size = sii_platform_rd_reg8(pstDev,
					REG_ADDR__DDC_DOUT_CNT) &
					BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT;

			if (fifo_size) {
				/* if the FIFO has some bytes*/
				if (fifo_size > length) {
					sii_log_debug("DDC error: FIFO");
					sii_log_debug("size exceeded length\n");
					ds_ddc_error =
						SI_TX_DDC_ERROR_CODE_TX_HW;
					flag = false;
				} else if (fifo_size > LEN_TPI__DDC_FIFO_SIZE) {
					sii_log_debug("DDC error: FIFO");
					sii_log_debug(" size exceeded max");
					sii_log_debug(" FIFO size\n");
					ds_ddc_error =
						SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
					flag = false;
				} else {
					/* read fifo_size bytes*/
					sii_platform_block_read8(pstDev,
						REG_ADDR__DDC_DATA, p_buf,
						fifo_size);
					length -= fifo_size;
					p_buf += fifo_size;
				}
			} else {
				/* note, the time is aligned*/
				usleep(1000);
				time_out_ms--;
			}
		} while (length && time_out_ms && flag);

		if (ds_ddc_error)
			break;

		if (0 == time_out_ms) {
			sii_log_debug("DDC error:: timedout\n");
			ds_ddc_error = SI_TX_DDC_ERROR_CODE_TIMEOUT;
			sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
				BIT_ENUM__DDC_CMD__CLEAR_FIFO);
		}
	} while (loop);

	if (ds_ddc_error) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			BIT_ENUM__DDC_CMD__ABORT_TRANSACTION);
        s_ddc_clear_fifo(p_obj, true);
	}

	/* disable DDC master*/
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
		BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
	/* sii_platform_clr_bit8(pdata, REG_ADDR__DDC_CMD, 0x7F);*/

	return ds_ddc_error;
}

/*---------------------------------------------------------------------------*/


/*! @brief	  wait for DS DDC operation to finish. */

static bool s_wait_for_ddc_bus(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t val;
	uint8_t time_out = LEN_TPI__DDC_FIFO_SIZE + 1;
	bool ret_val = false;

	/* time_out is time in ms, which is proportional to the FIFO size.*/
	/* since the time required to transmit one byte is 100 u_s,
	 * the time_out*/
	/* is as much as 10 times longer. +1 is additional time
	 * due to s_i__sleep()
	 function precision*/
	/* allows error +0-1 ms.*/
	do {
		val = sii_platform_rd_reg8(pstDev, REG_ADDR__DDC_STATUS) &
		BIT_MSK__DDC_STATUS__DDC_I2C_IN_PROG;

		if (0 == val) {
			ret_val = true;
			;
		} else {
			usleep(1000);
		}
	} while (--time_out && val);

	return ret_val;
}

/*----------------------------------------------------------------------------*/

/*! @brief	  parse edid after reading edid.
 *!
 *!			 the function can be used for parsing downstream EDID.
 *!
 *! @return	 error code, @see sii_lib_edid_err_code_t */

/*
static enum sii_lib_edid_err_code_t s_parse_edid(struct hdmi_tx_obj *p_obj)
{
	return sii_lib_edid_parse(&p_obj->parse_edid, (const uint8_t *)&
		p_obj->edid);
}
*/
static void s_apply_info_frame(struct hdmi_tx_obj *p_obj, uint8_t b_apply)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (b_apply) {
		/*start inforframes*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_EN, 0xC0);

	} else {
		/*stop inforframes*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_INFO_EN,
			BIT_MSK__TPI_INFO_EN__REG_TPI_INFO_RPT |
			BIT_MSK__TPI_INFO_EN__REG_TPI_INFO_EN);
	}
}

static void s_drop_pkt(struct hdmi_tx_obj *p_obj,
	enum sii_info_frame_id if_id, bool clear)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	switch (if_id) {
	case SII_INFO_FRAME_ID__AVI:
		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
			BIT_MSK__PKT_FILTER_0__REG_DROP_AVI_PKT,
			clear ? BIT_MSK__PKT_FILTER_0__REG_DROP_AVI_PKT : 0);
		break;
	case SII_INFO_FRAME_ID__AUDIO:
		break;
	case SII_INFO_FRAME_ID__ACR:
		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
			BIT_MSK__PKT_FILTER_0__REG_DROP_CTS_PKT,
			clear ? BIT_MSK__PKT_FILTER_0__REG_DROP_CTS_PKT : 0);
		break;
	case SII_INFO_FRAME_ID__VS:
		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
			BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT,
			clear ? BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT : 0);
		break;
	case SII_INFO_FRAME_ID__SPD:
		break;
	case SII_INFO_FRAME_ID__GBD:
		break;
	case SII_INFO_FRAME_ID__MPEG:
		break;
	case SII_INFO_FRAME_ID__ISRC:
		if (clear)
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__DROP_GEN_TYPE_0, 0x05);
		else
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__DROP_GEN_TYPE_0, 0x00);

		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
			BIT_MSK__PKT_FILTER_1__REG_DROP_GEN_PKT,
			clear ? BIT_MSK__PKT_FILTER_1__REG_DROP_GEN_PKT : 0);

		break;
	case SII_INFO_FRAME_ID__ISRC2:
		if (clear)
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__DROP_GEN_TYPE_1, 0x06);
		else
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__DROP_GEN_TYPE_1, 0x00);

		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
			BIT_MSK__PKT_FILTER_1__REG_DROP_GEN2_PKT,
			clear ? BIT_MSK__PKT_FILTER_1__REG_DROP_GEN2_PKT : 0);
		break;
	case SII_INFO_FRAME_ID__GCP:
		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
			BIT_MSK__PKT_FILTER_0__REG_DROP_GCP_PKT,
			clear ? BIT_MSK__PKT_FILTER_0__REG_DROP_GCP_PKT : 0);
		break;
	case SII_INFO_FRAME_ID__ACP:
		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
			BIT_MSK__PKT_FILTER_0__REG_DROP_CEA_CP_PKT,
			clear ? BIT_MSK__PKT_FILTER_0__REG_DROP_CEA_CP_PKT : 0);
		break;
	case SII_INFO_FRAME_ID__AUD_METDAT:
		break;
#ifdef CONFIG_DSC_EN
#ifndef CONFIG_DSC_HDMI_VSIF
	case SII_INFO_FRAME_ID__VSIF1:
	case SII_INFO_FRAME_ID__VSIF2:
	case SII_INFO_FRAME_ID__VSIF3:
	case SII_INFO_FRAME_ID__VSIF4:
		break;
#else
	case SII_INFO_FRAME_ID__VSIF1:
		sii_platform_put_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
			BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT,
			clear ? BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT : 0);
		break;

#endif
#endif
	default:
		break;
	}
}

static void s_infoframe_data_set(struct hdmi_tx_obj *p_obj,
					struct sii_info_frame *p_info_frame)
{
	/*uint8_t *ptr = p_info_frame->b;*/
	memcpy(&p_obj->if_data[p_info_frame->if_id], p_info_frame,
			sizeof(struct sii_info_frame));
	s_drop_pkt(p_obj, p_info_frame->if_id, p_info_frame->is_if_on);
	/*if (p_info_frame->is_if_on)
		s_dump_buffer((uint8_t *)ptr, SII_INFOFRAME_MAX_LEN);*/

	s_update_infoframes(p_obj, p_info_frame->if_id);
}

static void s_update_infoframes(struct hdmi_tx_obj *p_obj,
		enum sii_info_frame_id info_id)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
#ifdef CONFIG_DSC_EN
	uint8_t bit_msk_vsif_comp_one_per_frame =
		BIT_MSK__VSIF_COMP_CNTL__REG_VSIF_COMP_ONE_PER_FRAME;
#endif
	switch (info_id) {
	case SII_INFO_FRAME_ID__AVI:
		/*AVI infoframe*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_FSEL,
			BIT_ENUM__TPI_INFO_FSEL__AVI);
		break;
	case SII_INFO_FRAME_ID__VS:
		/*VSIF infoframe*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_FSEL,
			BIT_ENUM__TPI_INFO_FSEL__VSIF);
		break;
#ifdef CONFIG_DSC_EN
	case SII_INFO_FRAME_ID__VSIF1:
		/*VSIF1 infoframe*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_FSEL,
			BIT_ENUM__TPI_INFO_FSEL__VSIF1);
		sii_platform_clr_bit8(pstDev, REG_ADDR__VSIF_COMP_CNTL,
			bit_msk_vsif_comp_one_per_frame);
		break;
	case SII_INFO_FRAME_ID__VSIF2:
		/*VSIF2 infoframe*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_FSEL,
			BIT_ENUM__TPI_INFO_FSEL__VSIF2);
		sii_platform_clr_bit8(pstDev, REG_ADDR__VSIF_COMP_CNTL,
			bit_msk_vsif_comp_one_per_frame);
		break;
	case SII_INFO_FRAME_ID__VSIF3:
		/*VSIF3 infoframe*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_FSEL,
			BIT_ENUM__TPI_INFO_FSEL__VSIF3);
		sii_platform_clr_bit8(pstDev, REG_ADDR__VSIF_COMP_CNTL,
			bit_msk_vsif_comp_one_per_frame);
		break;
	case SII_INFO_FRAME_ID__VSIF4:
		/*VSIF4 infoframe*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INFO_FSEL,
			BIT_ENUM__TPI_INFO_FSEL__VSIF4);
		sii_platform_clr_bit8(pstDev, REG_ADDR__VSIF_COMP_CNTL,
			bit_msk_vsif_comp_one_per_frame);
		break;
	case SII_INFO_FRAME_ID__ACR:
		if (p_obj->if_data[info_id].is_if_on) {
			/*CTS Generate with MCLK*/
			sii_platform_clr_bit8(pstDev,
			    REG_ADDR__ACR_CTRL,
			    BIT_MSK__ACR_CTRL__REG_NO_MCLK_CTSGEN_SEL);

			sii_platform_set_bit8(pstDev,
			    REG_ADDR__ACR_CTRL,
			    BIT_MSK__ACR_CTRL__REG_CTS_REQ_EN);

			/*MCLK input mode: 000 = MCLK is 128*Fs*/
			sii_platform_put_bit8(pstDev,
			    REG_ADDR__FREQ_SVAL,
			    BIT_MSK__FREQ_SVAL__REG_FM_IN_VAL_SW ,
			    0x00);
			/*audio split enable*/
			sii_platform_set_bit8(pstDev,
			    REG_ADDR__DIPT_CNTL,
			    BIT_MSK__DIPT_CNTL__REG_AUD_SPLIT_EN
			    | BIT_MSK__DIPT_CNTL__REG_AUD_BYP_MODE);
		} else {
			sii_platform_set_bit8(pstDev,
			    REG_ADDR__ACR_CTRL,
			    BIT_MSK__ACR_CTRL__REG_NO_MCLK_CTSGEN_SEL);

			sii_platform_clr_bit8(pstDev,
			    REG_ADDR__DIPT_CNTL,
			    BIT_MSK__DIPT_CNTL__REG_AUD_SPLIT_EN
			    | BIT_MSK__DIPT_CNTL__REG_AUD_BYP_MODE);
			sii_platform_clr_bit8(pstDev,
			    REG_ADDR__ACR_CTRL,
			    BIT_MSK__ACR_CTRL__REG_CTS_REQ_EN);
		}
		/*there is no packet buffer selection for ACR
		So return from here only */
		return;
#endif
	default:
		return;
	}

	if (p_obj->if_data[info_id].is_if_on) {
		/* info_frame data update*/
		sii_platform_block_write8(pstDev, REG_ADDR__TPI_INFO_B0,
			(uint8_t *)p_obj->if_data[info_id].b,
			LEN_TPI__IF_BUFFER_LENGTH);

	}
	/*make the changes effective*/
	s_apply_info_frame(p_obj, p_obj->if_data[info_id].is_if_on);
}

static void s_put_tmds_on_off(struct hdmi_tx_obj *p_obj, bool on_off)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (on_off) {
		sii_platform_set_bit8(pstDev, REG_ADDR__HARRY_CTRL_1,
			BIT_MSK__HARRY_CTRL_1__REG_DP_OE1);
		sii_log_info("TMDS output enabled\n");
	} else {
		sii_platform_clr_bit8(pstDev, REG_ADDR__HARRY_CTRL_1,
			BIT_MSK__HARRY_CTRL_1__REG_DP_OE1);
		sii_log_info("TMDS output disabled\n");
	}
}

static void s_tmds_mode_set(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	bool tmds_on = true;
	if (!p_obj->b_hot_plug)
		p_obj->tmds_mode = SII_TMDS_MODE__NONE;

	if (p_obj->tmds_mode) {
		p_obj->i_state = SII_MOD_HDMI_TX_EVENT__TMDS_ON;
		p_obj->e_state = SII_DRV_HDMI_EXT_STATUS__TMDS_ON;
		tmds_on = true;
	} else {
		p_obj->i_state = SII_MOD_HDMI_TX_EVENT__TMDS_OFF;
		p_obj->e_state = SII_DRV_HDMI_EXT_STATUS__TMDS_OFF;
		tmds_on = false;
	}

	sii_log_debug("Tx TMDS mode: ");
	switch (p_obj->tmds_mode) {
	case SII_TMDS_MODE__NONE:
		sii_log_print("NONE\n");
		break;
	case SII_TMDS_MODE__DVI:
		/* enable HDMI on TX output*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TEST_TXCTRL,
			BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
		sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_SC,
			BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0);
		sii_log_print("DVI\n");
		break;
	case SII_TMDS_MODE__HDMI1:
		/* enable HDMI on TX output*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TEST_TXCTRL,
			BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
		sii_platform_set_bit8(pstDev, REG_ADDR__TPI_SC,
			BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0);
		sii_log_print("HDMI1\n");
		break;
	case SII_TMDS_MODE__HDMI2:
		/* enable HDMI on TX output*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TEST_TXCTRL,
			BIT_MSK__TEST_TXCTRL__REG_HDMI_MODE);
		sii_platform_set_bit8(pstDev, REG_ADDR__TPI_SC,
			BIT_MSK__TPI_SC__REG_TPI_OUTPUT_MODE_B0);
		sii_log_print("HDMI2\n");
		break;
	default:
		break;
	}

#ifdef CONFIG_ENHANCED_MODE_PROT
	sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
		SII_MOD_SCDC_OPCODE__TX_TMDS_MODE, &p_obj->tmds_mode);
#endif
	if (p_obj->b_tmds_on_req != tmds_on) {
		p_obj->b_tmds_on_req = tmds_on;
		if (p_obj->event_notify_fn) {
			p_obj->event_notify_fn(
				&g_stdrv_obj,
				SII_DRV_TX_EVENT__OUTPUT_TMDS_CHNG_REQ);
		}
	}

	/* notify tmds mode change to controller */
	if (p_obj->prev_i_state != p_obj->i_state) {
		p_obj->prev_i_state = p_obj->i_state;
		s_notify_hdmi_state(p_obj);
	}
}

static void s_av_mute_set(struct hdmi_tx_obj *p_obj, uint8_t b_av_mute)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (p_obj->tmds_mode == SII_TMDS_MODE__HDMI1
		|| p_obj->tmds_mode == SII_TMDS_MODE__HDMI2) {
		sii_log_debug("Setting AVMUTE:: %s\n",
			b_av_mute ? "ON" : "OFF");
		if (b_av_mute) {
			sii_platform_set_bit8(pstDev, REG_ADDR__TPI_SC,
				BIT_MSK__TPI_SC__REG_TPI_AV_MUTE);
		} else {
			sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_SC,
				BIT_MSK__TPI_SC__REG_TPI_AV_MUTE);
		}
	}
}

static void s_ddc_clear_fifo(struct hdmi_tx_obj *p_obj, bool b_ddc_clear_en)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (b_ddc_clear_en) {
		sii_log_debug("DDC clear FIFO\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			BIT_ENUM__DDC_CMD__CLEAR_FIFO);
		if (sii_platform_rd_reg8(pstDev, REG_ADDR__DDC_STATUS) &
			(BIT_MSK__DDC_STATUS__REG_DDC_BUS_LOW |
				BIT_MSK__DDC_STATUS__REG_DDC_NO_ACK)) {
			/*clear the DDC command done interrupt if it asserted already*/
			sii_platform_wr_reg8(pstDev, REG_ADDR__INTR3,
				BIT_MSK__INTR3__REG_INTR3_STAT3);

			/*Give Clock SCL,
			*This is to make the DDC client(sink) to release BUS low*/
			sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
				BIT_ENUM__DDC_CMD__CLOCK_SCL);
			/*1ms sleep is require to give time to complete the Clock SCL*/
			usleep(1000);
			if (sii_platform_rd_reg8(pstDev, REG_ADDR__INTR3) &
				BIT_MSK__INTR3__REG_INTR3_STAT3)
				/*Clear DDC Command done interrupt*/
				sii_platform_wr_reg8(pstDev, REG_ADDR__INTR3,
					BIT_MSK__INTR3__REG_INTR3_STAT3);
			else
				sii_log_debug(
					"Error: DDC recovery by Clock SCL not completed\n");

			sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_STATUS, 0x00);
		}
	}
}

static void s_update_hot_plug(struct hdmi_tx_obj *p_obj)
{
	if (p_obj->event_notify_fn)
		p_obj->event_notify_fn(&g_stdrv_obj,
			SII_DRV_TX_EVENT__HOT_PLUG_CHNG);
}

static void s_update_rsense(struct hdmi_tx_obj *p_obj)
{
	if (p_obj->event_notify_fn)
		p_obj->event_notify_fn(&g_stdrv_obj,
			SII_DRV_TX_EVENT__RSEN_CHNG);
}

static void s_output_bit_depth_set(struct hdmi_tx_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t reg_value = 0;
	switch (p_obj->output_bit_depth) {
	case SII_DRV_BIT_DEPTH__8_BIT:
		reg_value = 0x00;
		break;
	case SII_DRV_BIT_DEPTH__10_BIT:
		reg_value = 0x01;
		break;
	case SII_DRV_BIT_DEPTH__12_BIT:
		reg_value = 0x02;
		break;
	case SII_DRV_BIT_DEPTH__16_BIT:
		reg_value = 0x03;
		break;
	default:
		break;
	}
	sii_platform_put_bit8(pstDev, REG_ADDR__P2T_CTRL,
		BIT_MSK__P2T_CTRL__REG_PACK_MODE, reg_value);
}

static void s_update_output_bit_depth(struct hdmi_tx_obj *p_obj,
					enum sii_drv_bit_depth bit_depth)
{
	switch (bit_depth) {
	case SII_DRV_BIT_DEPTH__8_BIT:
		p_obj->output_bit_depth = SII_DRV_BIT_DEPTH__8_BIT;
		break;
	case SII_DRV_BIT_DEPTH__10_BIT:
		p_obj->output_bit_depth = SII_DRV_BIT_DEPTH__10_BIT;
		break;
	case SII_DRV_BIT_DEPTH__12_BIT:
		p_obj->output_bit_depth = SII_DRV_BIT_DEPTH__12_BIT;
		break;
	case SII_DRV_BIT_DEPTH__16_BIT:
		p_obj->output_bit_depth = SII_DRV_BIT_DEPTH__16_BIT;
		break;
	default:
		sii_log_debug("Error:: Wrong Output Bit Depth : %02X\n",
			bit_depth);
		break;
	}

	sii_log_debug("Setting Output Bit Depth to: %02X\n",
		p_obj->output_bit_depth);

	s_output_bit_depth_set(p_obj);
}

static void s_dump_buffer(uint8_t *p_data, uint16_t len)
{
	int i = 0;

	sii_log_print("\n");
	while (len--) {
		sii_log_print(" %02X", *p_data);
		p_data++;
		if (++i == 0x10) {
			sii_log_print("\n");
			i = 0;
		}

	}
	sii_log_print("\n");
}

void s_notify_hdmi_state(struct hdmi_tx_obj *p_obj)
{
	if (p_obj->event_notify_fn)
		p_obj->event_notify_fn(&g_stdrv_obj,
			SII_DRV_TX_EVENT__HDMI_STATE_CHNG);
}

/* Enhance Mode Protocol Implementation */
#ifdef CONFIG_ENHANCED_MODE_PROT
#define MSG_MIN_LENGTH	3
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
static uint8_t calculate_generic_checksum(uint8_t *data, uint8_t checksum,
	uint8_t length)
{
	uint8_t i;

	for (i = 0; i < length; i++)
		checksum += data[i];

	checksum = 0x100 - checksum;
	return checksum;
}

static int trasmit_em_packet(struct hdmi_tx_obj *p_obj,
	struct em_message *msg)
{
	struct em_packet l_stpacket;
	struct em_packet *packet = &l_stpacket;
	uint8_t frag_cnt = 0;
	uint8_t seq_num = 0;
	uint8_t payload_length = 0;
	int status = 0;
	struct sii_info_frame em_vsif;
	uint8_t checksum = 0;

	frag_cnt = 1;
	seq_num = 0;

	//packet = kzalloc(sizeof(struct em_packet) * frag_cnt, GFP_KERNEL);
    memset(&l_stpacket,0,sizeof(struct em_packet));

	packet->adopterid[0] = (LSCC_ADOPTER_ID >> 8) & 0xFF;
	packet->adopterid[1] = LSCC_ADOPTER_ID & 0xFF;
	packet->seq_num_frag_cnt = (seq_num << 4) | (frag_cnt & 0x0F);
	packet->checksum = 0;
	payload_length = msg->msg_length;
	packet->length = MIN_PACKET_LENGTH + payload_length;
	memcpy(packet->payload, msg, payload_length);

	packet->checksum =
		calculate_generic_checksum((uint8_t *)packet, 0,
		packet->length);

	/* send vsif to query EMC */
	em_vsif.if_id = SII_INFO_FRAME_ID__VSIF4; // Original : SII_INFO_FRAME_ID__VS;
	em_vsif.is_if_on = true;
	memset(em_vsif.b, 0, SII_INFOFRAME_MAX_LEN);
	em_vsif.b[0] = VSIF_TYPE;
	em_vsif.b[1] = HDMI_VSIF_VERSION;
	em_vsif.b[2] = (10 + packet->length);
	em_vsif.b[3] = 0;
	em_vsif.b[4] = (uint8_t) (IEEE_OUI_HDMI & 0xFF);
	em_vsif.b[5] =
		(uint8_t) ((IEEE_OUI_HDMI >> 8) & 0xFF);
	em_vsif.b[6] =
		(uint8_t) ((IEEE_OUI_HDMI >> 16) & 0xFF);

	/*placing the packet at pb10*/
	memcpy(&em_vsif.b[10], packet, packet->length);
	checksum = calculate_generic_checksum(em_vsif.b,
		checksum, SII_INFOFRAME_MAX_LEN);
	em_vsif.b[3] = checksum;

	sii_log_debug("VSIF with EM Packet: ");
	s_dump_buffer((uint8_t *)em_vsif.b, SII_INFOFRAME_MAX_LEN);
	s_infoframe_data_set(p_obj, &em_vsif);

	//kfree(packet);
	return status;
}
#endif

static int request_em_caps(struct hdmi_tx_obj *p_obj)
{
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
	struct em_message msg = {0};
	int status = 0;

	msg.msg_id = EM_CMD_QUERY_CAP;
	msg.msg_length = MSG_MIN_LENGTH;
	msg.msg_chksm = 0;
	msg.msg_chksm = calculate_generic_checksum((uint8_t *)&msg,
			0, msg.msg_length);

	status = trasmit_em_packet(p_obj, &msg);
	if (status) {
		sii_log_err("failed to trasmit EM Packet\n", status);
		return status;
	}

	sii_log_debug("EMC Packet Sent\n");
	sii_mod_tx_scdc_poll_em_buf(p_obj->tx_scdc_obj, EM_RPL_QUERY_CAP);

	return status;
#else
	sii_mod_tx_scdc_poll_em_buf(p_obj->tx_scdc_obj, EM_RPL_QUERY_CAP);
	return 0;
#endif
}

int sii_mod_tx_hdmi_request_em_caps(void *tx_hdmi_obj)
{
	struct hdmi_tx_obj *p_obj = (struct hdmi_tx_obj *)tx_hdmi_obj;

	if (SII_TMDS_MODE__NONE == p_obj->tmds_mode)
		return 1;
	return request_em_caps(p_obj);
}

int sii_mod_tx_hdmi_request_emm(void *tx_hdmi_obj, uint8_t req_mode)
{
	struct hdmi_tx_obj *p_obj = (struct hdmi_tx_obj *)tx_hdmi_obj;
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM

	struct em_message msg = {0};
	int status = 0;

	if (SII_TMDS_MODE__NONE == p_obj->tmds_mode)
		return -11;

	msg.msg_id = EM_CMD_REQ_MODE;
	msg.msg_length = MSG_MIN_LENGTH + 1;
	msg.msg_chksm = 0;
	memcpy(msg.payload, &req_mode, 1);
	msg.msg_chksm = calculate_generic_checksum((uint8_t *)&msg,
			0, msg.msg_length);

	status = trasmit_em_packet(p_obj, &msg);
	if (status) {
		sii_log_err("failed to trasmit EM Packet\n", status);
		return status;
	}
	sii_log_debug("EMM Packet Sent\n");
	sii_mod_tx_scdc_set(p_obj->tx_scdc_obj,
		SII_MOD_SCDC_OPCODE__EMM_REQ_MODE, (uint8_t *)&req_mode);
	sii_mod_tx_scdc_poll_em_buf(p_obj->tx_scdc_obj, EM_RPL_REQ_MODE);

	return status;
#else
	req_mode = req_mode;
	sii_mod_tx_scdc_poll_em_buf(p_obj->tx_scdc_obj, EM_RPL_REQ_MODE);
	return 0;
#endif
}

#endif

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
