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
#include "Sii9630_si_mod_rx_hdmi.h"
#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_drv_cp8630.h"
#include "Sii9630_si_timer.h"

/*enable this while doing HDMI 2.0 sink CTS*/
/*#define CONFIG_RX_CTS*/
#define NMB_OF_RX_INTERRUPT_REGS	11
#define NMB_OF_AON_INT_REGS		5
#define NMB_OF_DPCK_INT_REGS		7

#define INFOFRAME_MAX_SIZE		31
#define AVI_MAX_SIZE			19

#define INT1						0
#define INT2						1
#define INT3						2
#define INT4						3
#define INT5						4
#define INT6						5
#define INT7						6
#define INT8						7
#define INT9						8
#define INT10						9
#define INT11						10
#define INT13						12
#define INT2_AON					0
#define INT6_AON					1
#define INT7_AON					2
#define INT8_AON					3

/*#define EDID_BLOCK_SIZE 256*/

/*local events */
#define DRV_RX_EVENT__AAC_DONE					0x00000001
/*!< audio exception occurred */
#define DRV_RX_EVENT__NEW_AUDIO_FORMAT			0x00000004
/*!< audio format changed */
#define DRV_RX_EVENT__CLK_SYNC_DETECT_CHANGED	0x00000010
/*!< SYNC detection status changed */
#define DRV_RX_EVENT__HDMI_MODE_CHANGED			0x00000020
/*!< HDMI vs. DVI mode changed */
#define DRV_RX_EVENT__AV_MUTE_CHANGED			0x00000040
/*!< incoming AV mute status changed */
#define DRV_RX_EVENT__NEW_HDCP_AUTH				0x00000100
/*!< new authentication started */
#define DRV_RX_EVENT__HDCP_ERRORS				0x00000200
/*!< HDCP errors detected */
#define DRV_RX_EVENT__HDCP_RECOVERED			0x00000400
/*!< HDCP errors ceased */
#define DRV_RX_EVENT__INFO_FRAME_EVENT			0x00000800
/*!< info_frame/packet event pending */
#define DRV_RX_EVENT__H_V_SYNC_CHANGE			0x00001000
/*!< horizontal/vertical timing changed */
#define DRV_RX_EVENT__CABLE_CHANGED				0x00020000
/*!< cable (+5V) status changed */
#define DRV_RX_EVENT__DEEP_COLOR_CHANGED		0x00040000
/*!< deep color mode changed */
#define DRV_RX_EVENT__ALL					0xFFFFFFFF
/*!< all events */

/*rx events*/
/* input cable connected/disconnected*/
#define SII_DRV_RX_EVENT__CABLE_CHANGED			0x00001000

/*video states*/
#define INITIAL_STATE   0
#define VIDEO_ON	1
#define SYNC_IN_CHANGE  2

#define SYNC_CHANGE_TIMEOUT		20
#define HDCP2X_CODE_UPDATE_TIMEOUT	1000
#define BOOTING_TIMEOUT			100
#define SYNC_VALIDATE			1000
#define NUM_MANUFACTRER_SPECIFIC_REGS 34

enum color_space {
	RGB = 0,
	YCC_422,
	YCC_444,
	YCC_420
};

struct reg_info {
	uint16_t addr;
	uint8_t mask;
};

struct reg_info depack_intr_masks[NMB_OF_DPCK_INT_REGS] = { {
	REG_ADDR__RX_DEPACK_INTR0_MASK,
	BIT_MSK__RX_DEPACK_INTR0_MASK__REG_INTR0_MASK2
		| BIT_MSK__RX_DEPACK_INTR0_MASK__REG_INTR0_MASK5}, {
	REG_ADDR__RX_DEPACK_INTR1_MASK, 0x00}, {REG_ADDR__RX_DEPACK_INTR2_MASK,
	BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK0
		| BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK1
		| BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK2
		| BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK3
		| BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK4
		| BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK6
		/*Commenting this as of now we are not using
		 *anything regarding GCP data in our driver.
		 *Enabling this is causing GCP interrupt
		 *raising contineously if the US devices is SA*/
		/*| BIT_MSK__RX_DEPACK_INTR2_MASK__REG_INTR2_MASK7*/}, {
	REG_ADDR__RX_DEPACK_INTR3_MASK,
	BIT_MSK__RX_DEPACK_INTR3_MASK__REG_INTR3_MASK0
		| BIT_MSK__RX_DEPACK_INTR3_MASK__REG_INTR3_MASK1
		| BIT_MSK__RX_DEPACK_INTR3_MASK__REG_INTR3_MASK2}, {
	REG_ADDR__RX_DEPACK_INTR4_MASK,
	BIT_MSK__RX_DEPACK_INTR4_MASK__REG_INTR4_MASK0
		| BIT_MSK__RX_DEPACK_INTR4_MASK__REG_INTR4_MASK1
		| BIT_MSK__RX_DEPACK_INTR4_MASK__REG_INTR4_MASK2
		| BIT_MSK__RX_DEPACK_INTR4_MASK__REG_INTR4_MASK3
		| BIT_MSK__RX_DEPACK_INTR4_MASK__REG_INTR4_MASK4
		| BIT_MSK__RX_DEPACK_INTR4_MASK__REG_INTR4_MASK5}, {
	REG_ADDR__RX_DEPACK_INTR5_MASK,
	BIT_MSK__RX_DEPACK_INTR5_MASK__REG_INTR5_MASK0
		| BIT_MSK__RX_DEPACK_INTR5_MASK__REG_INTR5_MASK2
		| BIT_MSK__RX_DEPACK_INTR5_MASK__REG_INTR5_MASK3
		| BIT_MSK__RX_DEPACK_INTR5_MASK__REG_INTR5_MASK4
		| BIT_MSK__RX_DEPACK_INTR5_MASK__REG_INTR5_MASK5
		| BIT_MSK__RX_DEPACK_INTR5_MASK__REG_INTR5_MASK7}, {
	REG_ADDR__RX_DEPACK_INTR6_MASK,
	BIT_MSK__RX_DEPACK_INTR6_MASK__REG_INTR6_MASK0
		| BIT_MSK__RX_DEPACK_INTR6_MASK__REG_INTR6_MASK1
		| BIT_MSK__RX_DEPACK_INTR6_MASK__REG_INTR6_MASK2} };

struct reg_info depack_intrs[NMB_OF_DPCK_INT_REGS] = { {
	REG_ADDR__RX_DEPACK_INTR0, 0x00}, {REG_ADDR__RX_DEPACK_INTR1, 0x00}, {
	REG_ADDR__RX_DEPACK_INTR2, 0x00}, {REG_ADDR__RX_DEPACK_INTR3, 0x00}, {
	REG_ADDR__RX_DEPACK_INTR4, 0x00}, {REG_ADDR__RX_DEPACK_INTR5, 0x00}, {
	REG_ADDR__RX_DEPACK_INTR6, 0x00} };

struct scdc_obj {
	void *parent;
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;
	/*sink sersion <read only>*/
	uint8_t sink_ver;
	/*source version (r/w)*/
	uint8_t src_ver;
	/*update flags like: RR_TEST(bit_2),
	 c_e_d_update(bit_1), status_update(bit_0) and this field
	 is r/w*/
	uint8_t update_flags[2];
	/*tmds bit clock ratio(bit_1) and
	 scrambling_enable(bit_0) and this field is r/w*/
	uint8_t tmds_config;
	/*supports scrambled control code sequences or
	 not and this field is read only*/
	uint8_t scrambler_status;
	/* r_r__enable or not and this field is r/w*/
	uint8_t config_0;
	/*this is used for checking valid clock and
	 channel decoding and this field is read only*/
	uint8_t status_flags[2];
	/*checksum and character error rate and this
	 field is read only*/
	uint8_t error_det_0_l[7];
	/* delay in read request and this field is r/w*/
	uint8_t test_config_0;
	/*this is for manufacturer IEEE OUI octates and
	 this field is readonly*/
	uint8_t mf_ieee_oui[3];
	/*this is for device id and this field is read only*/
	uint8_t device_id[8];
	/*manufacturer specific regusters and this field is readonly*/
	uint8_t mf_specific;
	uint32_t scdc_events;
	/* SCDCS Clock Ratio set by source */
	uint8_t clock_ratio;

	void (*event_notify_fn)(void *, uint32_t);
};

struct rx_hdmi_obj {
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;
	struct scdc_obj *p_scdc_obj;
	/*--------------------------------*/
	/* user request states			*/
	/*--------------------------------*/
	struct sii_edid edid;
	bool is_hot_plug;

	/*--------------------------------*/
	/* user status					*/
	/*--------------------------------*/

	enum sii_tmds_mode tmds_mode;
	bool is_av_mute;
	bool is_plus5_v;
	bool clk_detect;
	bool sync_detect;
	bool ckdt_scdt_timer_started;
	enum sii_drv_clr_spc clr_spc;
	enum sii_drv_conv_std conv_std;
	enum sii_drv_bit_depth bit_depth;

	struct sii_info_frame if_data[SII_INFO_FRAME_ID__TOTAL];
	enum sii_drv_hdcp_link_version hdcp_link_ver;
	bool is_hdcp_proc;
	enum sii_drv_hdcp_status hdcp_stat;
	enum sii_drv_hdcp_content_type content_type;

	/*--------------------------------*/
	/* internal states				*/
	/*--------------------------------*/
	void *inst_timer_hpd;
	void *inst_timer_clk_sync;
	void *inst_timer_video_state;
	void *inst_timer_stable_video_timings;

	bool edid_ready;
	bool is_rx_pll_locked;
	bool is_video_state_handler_timer_call;
	uint32_t rx_hdmi_events;
	uint8_t rcv_id[5];
	struct sii_drv_tx_strmng_2x_msg rx_stmng_dat;
	enum sii_drv_hdcp_link_version rx_hdcp_ver;
	struct sii_drv_hdcp2x_rpt_msg hdcp_rx_rpt_msg;
	bool rx_rcv_id_send;
	uint32_t rx_scdc_events;
	struct sii_drv_vic_id vic_id;
	void (*event_notify_fn)(void *, uint32_t);
};

struct sii_mod_scdc_msd {
	uint8_t mf_info[NUM_MANUFACTRER_SPECIFIC_REGS];
};

struct sii_mod_scdc_msd s_scdc_manuf_info = {
	"SiliconImageInc.SiliconImageInc.."};


static int init_scdc_rx_regs(void *p_obj);
static void s_hpd_timer_call_back(void *p_obj);
static void s_info_frame_on_flags_clear(struct rx_hdmi_obj *p_obj);
static void s_video_state_handler(void *p_obj);
static void s_stable_video_timing_handler(void *p_obj);
static void s_update_edid(struct rx_hdmi_obj *p_obj);
static void s_update_hot_plug(struct rx_hdmi_obj *p_obj);
static void s_update_rx_auth_status(struct rx_hdmi_obj *p_obj,
		uint8_t int_stat);
static void s_update_color_depth(struct rx_hdmi_obj *p_obj);
static void s_rx_hdmi_hot_plug_set(struct rx_hdmi_obj *p_obj, bool on_off);
static void s_rx_hdmi_hdcp_status_reset(struct rx_hdmi_obj *p_obj);
static void s_mod_rx_hdmi_clear_info_frame(struct rx_hdmi_obj *p_obj,
						enum sii_info_frame_id if_id);
static void s_rx_hdmi_edid_set(struct rx_hdmi_obj *p_obj,
				struct sii_edid *p_edid);
static void s_update_mute(struct rx_hdmi_obj *p_obj);
static void s_rx_hdmi_hdcp_repeater_set(struct rx_hdmi_obj *p_obj,
					bool repeater_en);
#if 0
// banner doesn's support HDCP 2.2 Rx
static void s_rx_hdmi2x_hdcp_rcvid_set(
	struct rx_hdmi_obj *p_obj,
	struct sii_drv_hdcp2x_rpt_rcv_id_list *p_rcvid_list);
static void s_rx_hdcp2x_intr_handler(struct rx_hdmi_obj *p_obj);
#ifdef CONFIG_HDCP_REPEATER_EN
static void s_update_content_type(struct rx_hdmi_obj *p_obj);
static void s_hdcp2_rx_send_receiver_i_d_list(struct rx_hdmi_obj *p_obj);
static void s_hdcp2x_rx_sendnext_seqnum(struct rx_hdmi_obj *p_obj);
static void s_hdcp2_rx_set_reauth_req(struct rx_hdmi_obj *p_obj);
#endif
static enum sii_drv_hdcp_link_version s_rx_hdcp_get_version(
					struct rx_hdmi_obj *p_obj);
static void s_rx_hdmi2x_hdcp_reset(struct rx_hdmi_obj *p_obj, bool enable);
#endif
static void s_rx_hdcp_set_version(struct rx_hdmi_obj *p_obj, uint8_t hdcp_ver);
static void s_rx_hdmi_scdc_call_back(void *parent, uint32_t scdc_events);
static void s_deep_colour_fifo_reset(struct rx_hdmi_obj *p_obj);


static void s_enable420_input(struct rx_hdmi_obj *p_obj, bool b420_en);
static void s_update_blanking_levels(struct rx_hdmi_obj *p_obj);
void *sii_mod_rx_scdc_init(pstSii9630_Device_t pstDevice,
	void *parent, void (*event_notify_fn)(void *, uint32_t));
//static void sii_mod_rx_scdc_delete(struct scdc_obj *p_obj);
static bool sii_mod_rx_scdc_interrupt_handler(struct scdc_obj *p_obj);
static void s_rx_hdmi_hdcp_repeater_set(struct rx_hdmi_obj *p_obj,
					bool repeater_en);
static int rx_scdc_manufacturer_specific_data_set(
	struct scdc_obj *p_obj, struct sii_mod_scdc_msd *p_msd);
static void s_clk_sync_timeout_handler(void *p_rx_hdmi_obj);
#ifdef RX_DESCRAMBLE_THROUGH_API
static void s_rx_scdc_tmds_config(struct rx_hdmi_obj *p_rx_obj, bool scramble_en,
	bool tmds_bit_clk_ratio);
#endif

static struct rx_hdmi_obj sg_strx_hdmi_obj;
/* public functions */
void *sii_mod_rx_hdmi_init(pstSii9630_Device_t pstDevice,
				void (*event_notify_fn)(void *, uint32_t))
{
	struct rx_hdmi_obj *p_obj = &sg_strx_hdmi_obj;
	int tstatus = 0;

    memset(&sg_strx_hdmi_obj,0,sizeof(struct rx_hdmi_obj));
	/* allocate memory for object */
	//p_obj = kzalloc(sizeof(struct rx_hdmi_obj), GFP_KERNEL);
	if (!p_obj)
		return NULL;
    //sii_log_debug("Hdmi Rx Init start\n");
    p_obj->pstDevice = pstDevice;
	p_obj->event_notify_fn = event_notify_fn;

	tstatus = sii_timer_create(s_hpd_timer_call_back, p_obj,
		&p_obj->inst_timer_hpd,eTimerSil_rx_hdmi_hpd_timer);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate RX HPD timer!\n");
		goto free_mem;
	}
	tstatus = sii_timer_create(s_video_state_handler, p_obj,
		&p_obj->inst_timer_video_state,eTimerSil_rx_hdmi_video_state);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate RxHdmi Video state timer!\n");
		goto free_mem;
	}

	tstatus = sii_timer_create(s_stable_video_timing_handler, p_obj,
		&p_obj->inst_timer_stable_video_timings,eTimerSil_rx_hdmi_stable_video_timing);
	if (tstatus != 0) {
		sii_log_debug(
		"Failed to allocate stable video timing handler timer!\n");
		goto free_mem;
	}

	tstatus = sii_timer_create(s_clk_sync_timeout_handler, p_obj,
		&p_obj->inst_timer_clk_sync,eTimerSil_rx_hdmi_clk_sync_timeout);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate RxClock Sync timer!\n");
		goto free_mem;
	}

	p_obj->p_scdc_obj = sii_mod_rx_scdc_init(pstDevice,
				p_obj, s_rx_hdmi_scdc_call_back);
	if (!p_obj->p_scdc_obj)
		goto free_mem;

    //sii_log_debug("Hdmi Rx Init End\n");
	return p_obj;
free_mem:
	//sii_mod_rx_hdmi_exit(p_obj);
	return NULL;
}

/*---------------------------------------------------------------------------*/

#if 0
void sii_mod_rx_hdmi_exit(void *rx_hdmi_obj)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)rx_hdmi_obj;

	if (!p_obj)
		return;

	sii_mod_rx_scdc_delete(p_obj->p_scdc_obj);
	if (p_obj->inst_timer_clk_sync) {
		sii_timer_stop(&p_obj->inst_timer_clk_sync);
		sii_timer_delete(&p_obj->inst_timer_clk_sync);
		p_obj->inst_timer_clk_sync = NULL;
	}
	if (p_obj->inst_timer_stable_video_timings) {
		sii_timer_stop(&p_obj->inst_timer_stable_video_timings);
		sii_timer_delete(&p_obj->inst_timer_stable_video_timings);
		p_obj->inst_timer_stable_video_timings = NULL;
	}
	if (p_obj->inst_timer_video_state) {
		sii_timer_stop(&p_obj->inst_timer_video_state);
		sii_timer_delete(&p_obj->inst_timer_video_state);
		p_obj->inst_timer_video_state = NULL;
	}
	if (p_obj->inst_timer_hpd) {
		sii_timer_stop(&p_obj->inst_timer_hpd);
		sii_timer_delete(&p_obj->inst_timer_hpd);
		p_obj->inst_timer_hpd = NULL;
	}
	//kfree(p_obj);
}
#endif

static void reset_states(struct rx_hdmi_obj *p_obj)
{
	memset(&p_obj->edid, 0, sizeof(struct sii_edid));
	p_obj->is_hot_plug = false;
	p_obj->tmds_mode = SII_TMDS_MODE__NONE;
	p_obj->clk_detect = false;
	p_obj->sync_detect = false;
	p_obj->conv_std = SII_DRV_CONV_STD__BT_601;
	p_obj->clr_spc = SII_DRV_CLRSPC__RGB_DEFAULT;
	p_obj->bit_depth = SII_DRV_BIT_DEPTH__8_BIT;
	p_obj->is_av_mute = false;
	p_obj->is_plus5_v = false;
	p_obj->hdcp_link_ver = SII_DRV_HDCP_LINK_VER__NONE;
	p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__OFF;
	p_obj->is_hdcp_proc = false;
	p_obj->content_type = SII_DRV_HDCP_CONTENT_TYPE__UNKNOWN;
	p_obj->edid_ready = false;
	p_obj->is_rx_pll_locked = false;
	p_obj->hdcp_rx_rpt_msg.rcv_id_change = false;
	p_obj->hdcp_rx_rpt_msg.ske_event = false;
	p_obj->hdcp_rx_rpt_msg.rcv_id_enable = false;
	p_obj->rx_rcv_id_send = false;
	p_obj->is_video_state_handler_timer_call = true;

	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__AVI);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__AUDIO);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__VS);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__SPD);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__GBD);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__MPEG);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__ISRC);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__ISRC2);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__GCP);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__ACP);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__ACR);
	s_mod_rx_hdmi_clear_info_frame(p_obj, SII_INFO_FRAME_ID__AUD_METDAT);

	s_rx_hdcp_set_version(p_obj, SII_DRV_HDCP_LINK_VER__NONE);
	/* make sure HPD is low for enough time*/
	msleep(500);
}

int init_hdmi_rx_regs(void *p_obj)
{
	struct rx_hdmi_obj *p_rx_obj = (struct rx_hdmi_obj *)p_obj;
    pstSii9630_Device_t pstDev = p_rx_obj->pstDevice;
	uint8_t rg, i;
	int status = 0;

	reset_states(p_rx_obj);
#ifdef CONFIG_AUTO_SW_RST_EN
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_AON_SRST,
			BIT_MSK__RX_AON_SRST__REG_SW_RST_AUTO);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_PWD_SRST,
			BIT_MSK__RX_PWD_SRST__REG_AUD_FIFO_RST_AUTO);
#endif
	/*--------------------------------*/
	/* static hardware configuration  */
	/*--------------------------------*/
#ifdef CONFIG_HDCP_REPEATER_EN
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_INTR13_MASK,
		BIT_MSK__RX_INTR13_MASK__REG_INTR13_MASK1);
#endif
#if 0
// Banner doesn't support HDCP 2.2 Rx
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_CTRL,
		BIT_MSK__RX_HDCP2X_CTRL__RI_HDCP2X_EN);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_INTR1_MASK,
		BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B0
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B1
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B2
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B3
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B4);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_CTRL_0,
		BIT_MSK__RX_HDCP2X_CTRL_0__RI_HDCP2RX_REPEATER);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_AESCTL,
		BIT_MSK__RX_HDCP2X_AESCTL__RI_AES_RST_AUTHDONE);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_INTR0_MASK,
		BIT_MSK__RX_HDCP2X_INTR0_MASK__INTR0_MASK_B0
			| BIT_MSK__RX_HDCP2X_INTR0_MASK__INTR0_MASK_B1
			| BIT_MSK__RX_HDCP2X_INTR0_MASK__INTR0_MASK_B2
			| BIT_MSK__RX_HDCP2X_INTR0_MASK__INTR0_MASK_B6);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DEPTH, 0x00);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_AESCTL,
		BIT_MSK__RX_HDCP2X_AESCTL__RI_AES_RST_AUTHDONE);
	/*auto receiver id list send and rx reauth capability*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_GP_IN2, 0x08);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_CTRL_1, 0xE0);
	/*BIT_MASK__RX_HDCP2X_ECC_CTRL__FRM_ECC is still not known -
	 * its vallus is to be updated after confirmation.*/
	sii_platform_put_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_ECC_CTRL,
		BIT_MSK__RX_HDCP2X_RX_ECC_CTRL__RI_ECC_CHK_MODE, 0x00);
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_ECC_CTRL,
		BIT_MSK__RX_HDCP2X_RX_ECC_CTRL__RI_ECC_CHK_EN);
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__RX_HDCP2X_RX_ECC_FRM_ERR_THR_0, 0xFF);
#endif

	/* set HDCP 1.4 ECC error threshold*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP_THRES, 0xFF);
	/* hot plug enable */
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HPD_C_CTRL, 0);
	/*sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HPD_OEN_CTRL, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HPD_PE_CTRL,
		BIT_MSK__RX_HPD_PE_CTRL__REG_HPD_PE_CTRL);*/

	/* reprogram MPEG buffer to receive GBD infoframe*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__MPEG_DEC,
		BIT_MSK__MPEG_DEC__REG_MPEG_DEC);
	/* not required to remove hpd at initialization.
	 It will be done whenever edid is updated*/
	/* enable/configure DDC*/
	rg = BIT_MSK__RX_SYS_SWTCHC__DDC_FILTER_SEL
		| BIT_MSK__RX_SYS_SWTCHC__REG_DDC_EDID_EN
		| BIT_MSK__RX_SYS_SWTCHC__REG_DDC_HDCP_EN
		| BIT_MSK__RX_SYS_SWTCHC__REG_DDC_SCDC_EN
		| BIT_MSK__RX_SYS_SWTCHC__REG_DDC_SDA_OUT_DEL_EN
		| BIT_MSK__RX_SYS_SWTCHC__REG_DDC_SDA_IN_DEL_EN;

	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_SYS_SWTCHC, rg);

	sii_platform_wr_reg8(pstDev, REG_ADDR__SCDCS_CNTL,
		BIT_MSK__SCDCS_CNTL__REG_SCDCS_ENABLED);

	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ACR_CTS_MASK,
		BIT_MSK__RX_ACR_CTS_MASK__REG_ACR_CTS_THRESHOLD);

	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_SYS_CTRL1, 0x03);

	/* capture new infoframes*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INT_IF_CTRL, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INT_IF_CTRL2, 0);

	/* clear AIF packet header*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__AIF_PKT_ID,
		BIT_MSK__AIF_PKT_ID__REG_AIF_HEADER);

	/* write audio infoframe header to get AUDIF
	 data in unrecognized pkt*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_UNREC_DEC, 0x84);

	/* enable this for Depacketizer to check for the packet header */
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_UNREC_CTRL,
		BIT_MSK__RX_UNREC_CTRL__REG_CATCH_SPECIFIC_PKT_EN);

	/* enable interrupt masks*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR2_MASK_AON,
		BIT_MSK__RX_INTR2_MASK_AON__REG_INTR2_MASK3_AON
		| BIT_MSK__RX_INTR2_MASK_AON__REG_INTR2_MASK4_AON);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR7_MASK_AON,
		BIT_MSK__RX_INTR7_MASK_AON__REG_INTR7_MASK3_AON
			| BIT_MSK__RX_INTR7_MASK_AON__REG_INTR7_MASK4_AON
			| BIT_MSK__RX_INTR7_MASK_AON__REG_INTR7_MASK6_AON
			| BIT_MSK__RX_INTR7_MASK_AON__REG_INTR7_MASK7_AON);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR1_MASK,
		BIT_MSK__RX_INTR1_MASK__REG_INTR1_MASK0
			| BIT_MSK__RX_INTR1_MASK__REG_INTR1_MASK1
			| BIT_MSK__RX_INTR1_MASK__REG_INTR1_MASK2
			| BIT_MSK__RX_INTR1_MASK__REG_INTR1_MASK6);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR5_MASK,
		BIT_MSK__RX_INTR5_MASK__REG_INTR5_MASK0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR8_MASK,
		BIT_MSK__RX_INTR8_MASK__REG_INTR8_MASK7);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR13_MASK,
		BIT_MSK__RX_INTR13_MASK__REG_INTR13_MASK0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__VID_BLANK0, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__VID_BLANK1, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__VID_BLANK2, 0);
	for (i = 0; i < NMB_OF_DPCK_INT_REGS; i++) {
		sii_platform_wr_reg8(pstDev, depack_intr_masks[i].addr,
			depack_intr_masks[i].mask);
	}
	sii_platform_wr_reg8(pstDev, REG_ADDR__SCDCS_100MS_IN_1MS_CNT, 0xFF);
	status = init_scdc_rx_regs(p_rx_obj->p_scdc_obj);

	return status;
}

int sii_mod_rx_hdmi_stop_timers(void *rx_hdmi_obj)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)rx_hdmi_obj;
	int status = 0;

	if (!p_obj)
		return 1;

	/* stop all timers */
	if (p_obj->inst_timer_clk_sync) {
		sii_timer_stop(&p_obj->inst_timer_clk_sync);
		/*sii_timer_delete(&p_obj->inst_timer_clk_sync);
		p_obj->inst_timer_clk_sync = NULL;*/
	}
	if (p_obj->inst_timer_stable_video_timings) {
		sii_timer_stop(&p_obj->inst_timer_stable_video_timings);
		/*sii_timer_delete(&p_obj->inst_timer_stable_video_timings);
		p_obj->inst_timer_stable_video_timings = NULL;*/
	}
	if (p_obj->inst_timer_video_state) {
		sii_timer_stop(&p_obj->inst_timer_video_state);
		/*sii_timer_delete(&p_obj->inst_timer_video_state);
		p_obj->inst_timer_video_state = NULL;*/
	}
	if (p_obj->inst_timer_hpd) {
		sii_timer_stop(&p_obj->inst_timer_hpd);
		/*sii_timer_delete(&p_obj->inst_timer_hpd);
		p_obj->inst_timer_hpd = NULL;*/
	}
	return status;
}

bool sii_mod_rx_hdmi_interrupt_handler(void *rx_hdmi_obj)
{
	struct rx_hdmi_obj *p_obj;
    pstSii9630_Device_t pstDev;// = p_rx_obj->pstDevice;
	uint8_t interrupts[NMB_OF_RX_INTERRUPT_REGS] = {0}, i = 0;
	uint8_t aon_ints[NMB_OF_AON_INT_REGS] = {0};
	uint8_t dpck_ints[NMB_OF_DPCK_INT_REGS] = {0};
#if 0
// Banner doesn't support HDCP 2.2 Rx
    uint8_t bit_msk_hdcp2x_ecc_out_of_sync_intr =
		BIT_MSK__RX_HDCP2X_INTR__REG_ECC_OUT_OF_SYNC_INTR;
#endif

	p_obj = (struct rx_hdmi_obj *)rx_hdmi_obj;
	if (NULL == p_obj)
		return FALSE;

    pstDev = p_obj->pstDevice;

	/*SCDC status change monitoring - no group interrupt*/

	/*Rx SCDC interrupts shall be processed even before rx pll
	is locked. For input TMDS clock > 340 MHz, Rx Phy shall
	be programmed for > 340 MHz to get Rx Pll locked.
	Without Rx Phy configuration, rx pll will not be
	locked */
	sii_mod_rx_scdc_interrupt_handler(p_obj->p_scdc_obj);

	/* get interrupt requests*/
	sii_platform_block_read8(pstDev, REG_ADDR__RX_INTR1, interrupts,
		NMB_OF_RX_INTERRUPT_REGS);
	sii_platform_block_read8(pstDev, REG_ADDR__RX_INTR2_AON, aon_ints,
		NMB_OF_AON_INT_REGS);
	for (i = 0; i < NMB_OF_DPCK_INT_REGS; i++)
		dpck_ints[i] =
			sii_platform_rd_reg8(pstDev, depack_intrs[i].addr);

	/* clear interrupt requests*/
	sii_platform_block_write8(pstDev, REG_ADDR__RX_INTR1, interrupts,
		NMB_OF_RX_INTERRUPT_REGS);

	/*Do not clear Sync Detect interrupt if rx_pll is not locked*/
	if(!p_obj->is_rx_pll_locked) {
		if(sii_platform_rd_reg8(pstDev, REG_ADDR__RX_STATE_AON)
			& BIT_MSK__RX_STATE_AON__SCDT)
			aon_ints[0] &= ~BIT_MSK__RX_INTR2_AON__REG_INTR2_STAT3_AON;
	}

	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR2_AON, aon_ints[INT2_AON]);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR6_AON, aon_ints[INT6_AON]);
	/* Donot clear INTR7_AON and INTR9_AON */
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR8_AON, aon_ints[INT8_AON]);

	for (i = 0; i < NMB_OF_DPCK_INT_REGS; i++)
			sii_platform_wr_reg8(pstDev, depack_intrs[i].addr,
					dpck_ints[i]);

	if (aon_ints[INT8_AON] & BIT_MSK__RX_INTR8_AON__REG_INTR8_STAT1_AON) {
		bool is_rx_connected =
			sii_platform_rd_reg8(pstDev, REG_ADDR__RX_STATE_AON)
				& (BIT_MSK__RX_STATE_AON__HDMI_TX_CONNECTED) ?
				true : false;

		sii_mod_rx_hdmi_set(p_obj, SII_MOD_RX_HDMI_OPCODE__RPWR,
			(void *)&is_rx_connected);
		p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__RX_CONN_CHNG;
	}
	if (aon_ints[INT6_AON] & BIT_MSK__RX_INTR6_AON__REG_INTR6_STAT0_AON) {
		bool is_rx_connected =
			sii_platform_rd_reg8(pstDev, REG_ADDR__RX_STATE_AON)
				& (BIT_MSK__RX_STATE_AON__HDMI_TX_CONNECTED) ?
				true : false;

		sii_mod_rx_hdmi_set(p_obj, SII_MOD_RX_HDMI_OPCODE__RPWR,
			(void *)&is_rx_connected);
		p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__RX_CONN_CHNG;
	}
	if (!p_obj->is_rx_pll_locked) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR8_AON,
			BIT_MSK__RX_INTR8_AON__REG_INTR8_STAT1_AON);
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR6_AON,
			BIT_MSK__RX_INTR6_AON__REG_INTR6_STAT0_AON);
		if (p_obj->rx_hdmi_events && p_obj->event_notify_fn) {
			p_obj->event_notify_fn(&g_stdrv_obj,
				p_obj->rx_hdmi_events);
			p_obj->rx_hdmi_events = 0;
		}
		return TRUE;
	}

	if (interrupts[INT5] & BIT_MSK__RX_INTR5__REG_INTR5_STAT0)
		p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AUDIO_FRM_CHNG;

	if (interrupts[INT8] & BIT_MSK__RX_INTR8__REG_INTR8_STAT7)
		p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__CHAN_STAT_CHNG;

	/* infoframes */
	if (interrupts[INT1] & BIT_MSK__RX_INTR1__REG_INTR1_STAT2) {

		/* AVI */
		if (dpck_ints[2] & BIT_MSK__RX_DEPACK_INTR2__REG_INTR2_STAT0) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AVI_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__AVI].is_if_on = true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL);

			if (if_ctrl &
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_AVI_ONLY) {
				/* now catch only new AVI*/
				sii_platform_clr_bit8(pstDev,
				    REG_ADDR__RX_INT_IF_CTRL,
				    BIT_MSK__RX_INT_IF_CTRL__REG_NEW_AVI_ONLY);
			}
            memset(p_obj->if_data[SII_INFO_FRAME_ID__AVI].b, 0, SII_INFOFRAME_MAX_LEN);
			sii_platform_block_read8(pstDev, REG_ADDR__AVIRX_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__AVI].b,
				AVI_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__AVI].b;
			s_update_blanking_levels(p_obj);
			sii_log_debug("AVI:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4],
				p[5], p[6], p[7], p[8]);
            UNUSED(p);

		}

		if (dpck_ints[5] & BIT_MSK__RX_DEPACK_INTR5__REG_INTR5_STAT0) {
			sii_log_debug("-AVI\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AVI_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__AVI].is_if_on = false;
		}

		/* Unrecognized packet for audioif */
		if (dpck_ints[2] & BIT_MSK__RX_DEPACK_INTR2__REG_INTR2_STAT4) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AUDIO_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__AUDIO].is_if_on =
				true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL);

			if (if_ctrl
				& BIT_MSK__RX_INT_IF_CTRL__REG_NEW_UNREC_ONLY) {
				/* now catch only new Unrecognized InfoFrames*/
				sii_platform_clr_bit8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL,
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_UNREC_ONLY);
			}
			sii_platform_block_read8(pstDev,
				REG_ADDR__RX_UNREC_BYTE1,
				p_obj->if_data[SII_INFO_FRAME_ID__AUDIO].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__AUDIO].b;

			sii_log_debug("AUD:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            UNUSED(p);
		}
		if (dpck_ints[5] & BIT_MSK__RX_DEPACK_INTR5__REG_INTR5_STAT6) {
			sii_log_debug("-Unrecognized InfoFrames:AUD\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AUDIO_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__AUDIO].is_if_on =
				false;
		}
		/* VSI */
		if (dpck_ints[3] & BIT_MSK__RX_DEPACK_INTR3__REG_INTR3_STAT2) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__VS_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__VS].is_if_on = true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL);

			if (if_ctrl &
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_VSI_ONLY) {
				/* now catch only new VSIF*/
				sii_platform_clr_bit8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL,
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_VSI_ONLY);
			}
			sii_platform_block_read8(pstDev, REG_ADDR__VSIRX_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__VS].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__VS].b;
			sii_log_debug("VSI:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4],
				p[5], p[6], p[7], p[8]);
            UNUSED(p);

		}

		if (dpck_ints[5] & BIT_MSK__RX_DEPACK_INTR5__REG_INTR5_STAT2) {

			sii_log_debug("-VSI\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__VS_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__VS].is_if_on = false;
		}

		/* SPD */
		if (dpck_ints[2] & BIT_MSK__RX_DEPACK_INTR2__REG_INTR2_STAT1) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__SPD_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__SPD].is_if_on = true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL);

			if (if_ctrl &
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_SPD_ONLY) {
				/* now catch only new SPD*/
				sii_platform_clr_bit8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL,
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_SPD_ONLY);
			}
			sii_platform_block_read8(pstDev, REG_ADDR__SPDRX_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__SPD].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__SPD].b;

			sii_log_debug("SPD:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            UNUSED(p);
		}

		if (dpck_ints[5] & BIT_MSK__RX_DEPACK_INTR5__REG_INTR5_STAT4) {

			sii_log_debug("-SPD\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__SPD_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__SPD].is_if_on = false;
		}

		/* GBD */
		if (dpck_ints[2] & BIT_MSK__RX_DEPACK_INTR2__REG_INTR2_STAT3) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__GBD_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__MPEG].is_if_on =
				true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL);

			if (if_ctrl &
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_MPEG_ONLY) {
				/* now catch only new GBD*/
				sii_platform_clr_bit8(pstDev,
				  REG_ADDR__RX_INT_IF_CTRL,
				  BIT_MSK__RX_INT_IF_CTRL__REG_NEW_MPEG_ONLY);
			}
			sii_platform_block_read8(pstDev, REG_ADDR__MPEGRX_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__MPEG].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__MPEG].b;
			sii_log_debug("GBD:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            UNUSED(p);
		}

		if (dpck_ints[5] & BIT_MSK__RX_DEPACK_INTR5__REG_INTR5_STAT5) {

			sii_log_debug("-GBD\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__GBD_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__MPEG].is_if_on =
				false;
		}

		if (dpck_ints[1] & BIT_MSK__RX_DEPACK_INTR1__REG_INTR1_STAT0) {
      			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__NO_AUDIO;
		}

		/* ISRC */
		if (dpck_ints[3] & BIT_MSK__RX_DEPACK_INTR3__REG_INTR3_STAT0) {
			uint8_t if_ctrl, *p;
			uint8_t bit_mask_new_isrc1 =
				BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC1_ONLY;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__ISRC_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__ISRC].is_if_on =
				true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL2);

			if (if_ctrl & bit_mask_new_isrc1) {
				/* now catch only new ISRC*/
				sii_platform_clr_bit8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL2,
				BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC1_ONLY);
			}
			sii_platform_block_read8(pstDev, REG_ADDR__RX_ISRC1_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__ISRC].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__ISRC].b;

			sii_log_debug("ISRC:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            UNUSED(p);
		}

		if (dpck_ints[6] & BIT_MSK__RX_DEPACK_INTR6__REG_INTR6_STAT0) {

			sii_log_debug("-ISRC:\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__ISRC_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__ISRC].is_if_on =
				false;
		}

		/* ISRC2 */
		if (dpck_ints[3] & BIT_MSK__RX_DEPACK_INTR3__REG_INTR3_STAT1) {
			uint8_t if_ctrl, *p;
			uint8_t bit_mask_new_isrc2 =
				BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC2_ONLY;


			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__ISRC2_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__ISRC2].is_if_on =
				true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL2);

			if (if_ctrl & bit_mask_new_isrc2) {
				/* now catch only new ISRC*/
				sii_platform_clr_bit8(pstDev,
				 REG_ADDR__RX_INT_IF_CTRL2,
				 BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC2_ONLY);
			}
			sii_platform_block_read8(pstDev, REG_ADDR__RX_ISRC2_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__ISRC2].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__ISRC2].b;

			sii_log_debug("ISRC2:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            UNUSED(p);
		}

		if (dpck_ints[6] & BIT_MSK__RX_DEPACK_INTR6__REG_INTR6_STAT1) {
			sii_log_debug("-ISRC2\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__ISRC2_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__ISRC2].is_if_on =
				false;

		}

		/* ACP */
		if (dpck_ints[2] & BIT_MSK__RX_DEPACK_INTR2__REG_INTR2_STAT6) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__ACP_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__ACP].is_if_on = true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL);

			if (if_ctrl &
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_ACP_ONLY) {
				/* now catch only new ACP*/
				sii_platform_clr_bit8(pstDev,
				    REG_ADDR__RX_INT_IF_CTRL,
				    BIT_MSK__RX_INT_IF_CTRL__REG_NEW_ACP_ONLY);
			}
			sii_platform_block_read8(pstDev, REG_ADDR__RX_ACP_BYTE1,
				p_obj->if_data[SII_INFO_FRAME_ID__ACP].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__ACP].b;

			sii_log_debug(
				"ACP:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            UNUSED(p);
		}

		if (dpck_ints[5] & BIT_MSK__RX_DEPACK_INTR5__REG_INTR5_STAT7) {
			sii_log_debug("-ACP\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__ACP_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__ACP].is_if_on =
				false;
		}

		/* GCP */
		if (dpck_ints[2] & BIT_MSK__RX_DEPACK_INTR2__REG_INTR2_STAT7) {
			uint8_t if_ctrl, *p;

			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__GCP_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__GCP].is_if_on = true;

			if_ctrl = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_INT_IF_CTRL2);

			if (if_ctrl &
				BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_GCP_ONLY) {
				/* now catch only new GCP*/
				sii_platform_clr_bit8(pstDev,
				    REG_ADDR__RX_INT_IF_CTRL2,
				    BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_GCP_ONLY);
			}
			s_update_mute(p_obj);
			sii_platform_block_read8(pstDev, REG_ADDR__RX_GCP_TYPE,
				p_obj->if_data[SII_INFO_FRAME_ID__GCP].b,
				INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__GCP].b;

			/*sii_log_debug(
				"GCP:%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
				p[0], p[1], p[2], p[3], p[4], p[5], p[6]);*/
			UNUSED(p);
		}

		if (dpck_ints[6] & BIT_MSK__RX_DEPACK_INTR6__REG_INTR6_STAT2) {
			sii_log_debug("-GCP\n");
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__GCP_IF;
			p_obj->if_data[SII_INFO_FRAME_ID__GCP].is_if_on =
				false;
		}

		/* 3D audio */
		if (dpck_ints[4] & BIT_MSK__RX_DEPACK_INTR4__REG_INTR4_STAT0) {
			/*SII_LIB_LOG_DEBUG1(p_obj, ("+3D audio\n"));*/
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__3D_ADUIO_PKT;
		}

		/* 1 bit 3D audio */
		if (dpck_ints[4] & BIT_MSK__RX_DEPACK_INTR4__REG_INTR4_STAT1) {
			/*SII_LIB_LOG_DEBUG1(p_obj,
			 * ("+ONE BIT 3D audio\n"));*/
			p_obj->rx_hdmi_events |=
				SII_DRV_RX_EVENT__ONEBIT_3D_ADUIO_PKT;
		}

		/* audio metadata */
		if (dpck_ints[4] & BIT_MSK__RX_DEPACK_INTR4__REG_INTR4_STAT2) {
			uint8_t *p;

			p_obj->rx_hdmi_events |=
				SII_DRV_RX_EVENT__ADUIO_METADATA_PKT;
			p_obj->if_data[SII_INFO_FRAME_ID__AUD_METDAT].is_if_on
				= true;
			sii_platform_block_read8(pstDev,
				REG_ADDR__METADATA_DBYTE0,
				p_obj->if_data[SII_INFO_FRAME_ID__AUD_METDAT].b, INFOFRAME_MAX_SIZE);
			p = p_obj->if_data[SII_INFO_FRAME_ID__AUD_METDAT].b;

			/*  sii_log_debug(
			 *  "AMD:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x\n",
			 *  p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);*/
			 UNUSED(p);
		}

		/* no audio metadata */
		if (dpck_ints[6] & BIT_MSK__RX_DEPACK_INTR6__REG_INTR6_STAT4) {
			p_obj->rx_hdmi_events |=
				SII_DRV_RX_EVENT__ADUIO_METADATA_PKT;
			p_obj->if_data[SII_INFO_FRAME_ID__AUD_METDAT].is_if_on = false;
		}

		/* multi stream aduio */
		if (dpck_ints[4] & BIT_MSK__RX_DEPACK_INTR4__REG_INTR4_STAT3) {
			p_obj->rx_hdmi_events |=
				SII_DRV_RX_EVENT__MULTISTREAM_ADUIO_PKT;
		}

		/* one bit multi stream audio */
		if (dpck_ints[4] & BIT_MSK__RX_DEPACK_INTR4__REG_INTR4_STAT4) {
			p_obj->rx_hdmi_events |=
				SII_DRV_RX_EVENT__ONEBIT_MULTISTREAM_ADUIO_PKT;
		}

		/* DC mode change */
		if (BIT_MSK__RX_DEPACK_INTR0__REG_INTR0_STAT5 & dpck_ints[0]) {
			s_update_mute(p_obj);
			s_update_color_depth(p_obj);
			s_deep_colour_fifo_reset(p_obj);
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__DC_CHNG;
		}


		/* HDCP1.4 ECC error check */
		if ((BIT_MSK__RX_DEPACK_INTR0__REG_INTR0_STAT2
				& dpck_ints[0]))  {
			/*s_rx_hdmi_hot_plug_set(p_obj, false);*/
			sii_platform_set_bit8(pstDev, REG_ADDR__RX_ECC_CTRL,
				BIT_MSK__RX_ECC_CTRL__REG_CAPTURE_CNT);

			sii_log_debug("num of HDCP1.4 ECC errors  = %d\n",
			    sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP_ERR));

			/*sii_timer_start(&p_obj->inst_timer_hpd, 1500);*/

			sii_platform_wr_reg8(pstDev, REG_ADDR__RX_DEPACK_INTR0,
				BIT_MSK__RX_DEPACK_INTR0__REG_INTR0_STAT2);

			sii_platform_clr_bit8(pstDev, REG_ADDR__RX_ECC_CTRL,
				BIT_MSK__RX_ECC_CTRL__REG_CAPTURE_CNT);
		}
	}

	if (aon_ints[INT2_AON]
		& (BIT_MSK__RX_INTR2_AON__REG_INTR2_STAT4_AON
		| BIT_MSK__RX_INTR2_AON__REG_INTR2_STAT3_AON)) {

		uint8_t clk_detect = 0, sync_detect = 0;
		uint8_t rx_state = sii_platform_rd_reg8(pstDev,
					REG_ADDR__RX_STATE_AON);

		clk_detect = rx_state & BIT_MSK__RX_STATE_AON__CKDT;
		sync_detect = rx_state & BIT_MSK__RX_STATE_AON__SCDT;

		/* below logic is a work around if we miss a CKDT/SCDT
		 * loss interrupt*/
		if (SII_TMDS_MODE__NONE != p_obj->tmds_mode) {
			/* returning to initial state (video off) */
			p_obj->tmds_mode = SII_TMDS_MODE__NONE;
			p_obj->clk_detect = false;
			p_obj->sync_detect = false;
			s_info_frame_on_flags_clear(p_obj);
			p_obj->rx_hdmi_events |=
				SII_DRV_RX_EVENT__TMDS_MODE_CHNG;

			/* catch any infoframe */
			sii_platform_set_bit8(pstDev, REG_ADDR__RX_INT_IF_CTRL,
				BIT_MSK__RX_INT_IF_CTRL__REG_NEW_AVI_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_SPD_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_AUD_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_MPEG_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_ACP_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_VSI_ONLY);

			sii_platform_set_bit8(pstDev, REG_ADDR__RX_INT_IF_CTRL2,
				BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC1_ONLY
				| BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC2_ONLY
				| BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_GCP_ONLY
				| BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ACR_ONLY);
			s_rx_hdmi_hdcp_status_reset(p_obj);

			sii_log_debug("CKDT/SCDT OFF\n");
			sii_log_debug("TMDS_MODE:NONE\n");
		}

		if (clk_detect && sync_detect) {
			sii_log_debug("Got CKDT/SCDT High Interrupt\n");
			if (p_obj->ckdt_scdt_timer_started) {
				sii_log_debug("Rescheduling the timer\n");
				/*Stop the timer and restart again,
				 *As we have to wait on stable scdt/ckdt high signal*/
				sii_timer_stop(&p_obj->inst_timer_video_state);
				sii_timer_start(&p_obj->inst_timer_video_state,
					SYNC_CHANGE_TIMEOUT);
			} else {
				sii_log_debug("Staring timer\n");
				p_obj->ckdt_scdt_timer_started = true;
				/* 500ms for clk stability */
				sii_timer_start(&p_obj->inst_timer_video_state,
					SYNC_CHANGE_TIMEOUT);
			}
		} else if (p_obj->ckdt_scdt_timer_started) {
			sii_log_debug("Got CKDT/SCDT Low interrupt, stopping the timer\n");
			p_obj->ckdt_scdt_timer_started = false;
			sii_timer_stop(&p_obj->inst_timer_video_state);
		}
	}

#if 0
// Banner doesn't support HDCP 2.2 Rx
	/* HDCP2.2 frame ECC error check */
	if (BIT_MSK__RX_HDCP2X_INTR__REG_ECC_OUT_OF_SYNC_INTR
		& sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR)) {
		uint8_t ecc_cont[2] = {0};

		s_rx_hdmi_hot_plug_set(p_obj, false);

		sii_platform_block_read8(pstDev,
			REG_ADDR__RX_HDCP2X_RX_ECC_CNT2CHK_0,
			ecc_cont, 2);

		sii_log_debug(
			"num of HDCP2.2 ECC errors  = %d\n",
			(ecc_cont[1] << 8) | ecc_cont[0]);

		sii_timer_start(&p_obj->inst_timer_hpd, 1500);

		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR,
			bit_msk_hdcp2x_ecc_out_of_sync_intr);

		ecc_cont[0] = 0;
		ecc_cont[1] = 0;
		sii_platform_block_write8(pstDev,
			REG_ADDR__RX_HDCP2X_RX_ECC_CNT2CHK_0,
			ecc_cont, 2);
	}
#endif

	/*---------------RX HDCP auth status--------------*/
#if 0
// Banner doesn't support HDCP 2.2 Rx
	if (((BIT_MSK__RX_INTR1__REG_INTR1_STAT1
		| BIT_MSK__RX_INTR1__REG_INTR1_STAT0) & interrupts[INT1])
		|| ((BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT0
			| BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT1)
			& sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_HDCP2X_INTR0))) {
		s_update_rx_auth_status(p_obj, interrupts[INT1]);
		p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AUTH_CHNG;
	}
#else
	if ((BIT_MSK__RX_INTR1__REG_INTR1_STAT1 | BIT_MSK__RX_INTR1__REG_INTR1_STAT0) & interrupts[INT1])
	{
		s_update_rx_auth_status(p_obj, interrupts[INT1]);
		p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AUTH_CHNG;
	}
#endif
#if 0
// Banner doesn't support HDCP 2.2 Rx
	if ((sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR1))
		| (sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR0)))
		s_rx_hdcp2x_intr_handler(p_obj);
#endif
	if (p_obj->rx_hdmi_events && p_obj->event_notify_fn) {
		p_obj->event_notify_fn(&g_stdrv_obj,
			p_obj->rx_hdmi_events);
		p_obj->rx_hdmi_events = 0;
	}

	return TRUE;
}

int sii_mod_rx_hdmi_set(void *prx_hdmi_obj, enum sii_mod_rx_hdmi_opcode op_code,
			void *in_data)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)prx_hdmi_obj;
	int status = 0;

	if (p_obj) {
		switch (op_code) {
		case SII_MOD_RX_HDMI_OPCODE__RPWR:
			p_obj->is_plus5_v = (bool)(*((uint8_t *)(in_data)));
			if (!p_obj->is_plus5_v) {
				s_rx_hdmi_hdcp_status_reset(p_obj);
				s_rx_hdmi_hot_plug_set(p_obj, false);
			}
			break;
		case SII_MOD_RX_HDMI_OPCODE__EDID:
		{
			bool delay_hpd = p_obj->is_hot_plug;

			if (delay_hpd) {
				s_rx_hdmi_hot_plug_set(p_obj, false);
				;
			}
			s_rx_hdmi_edid_set(p_obj, (struct sii_edid *)in_data);

			/*make sure HPD is low for significant time*/
			sii_timer_start(&p_obj->inst_timer_hpd,
				delay_hpd ? 500/*100*/ : 1);
		}
			break;
		case SII_MOD_RX_HDMI_OPCODE__HPD:
			s_rx_hdmi_hot_plug_set(p_obj,
				*((bool *)(in_data)) ? true : false);
			break;
		case SII_MOD_RX_HDMI_OPCODE__HPD_TOGGLE:
			s_rx_hdmi_hot_plug_set(p_obj, false);
			sii_timer_start(&p_obj->inst_timer_hpd, 100);
			break;
		case SII_MOD_RX_HDMI_OPCODE__IF_CLEAR:
			s_mod_rx_hdmi_clear_info_frame(p_obj,
				*((enum sii_info_frame_id *)in_data));
			break;
		case SII_MOD_RX_HDMI_OPCODE__REPEATER_SET:
			s_rx_hdmi_hdcp_repeater_set(p_obj,
				*((bool *)in_data) ? true : false);
			break;
		case SII_MOD_RX_HDMI_OPCODE__HDCP_RCVID_SET:
			memcpy(&p_obj->rcv_id, in_data,
				sizeof(p_obj->rcv_id));
			break;
#if 0
// banner doesn's support HDCP 2.2 Rx
		case SII_MOD_RX_HDMI_OPCODE__HDCP_RPT_RCVID_SET:
			s_rx_hdmi2x_hdcp_rcvid_set(p_obj,
				(struct sii_drv_hdcp2x_rpt_rcv_id_list *)
								in_data);
			break;
		case SII_MOD_RX_HDMI_OPCODE__HDCP_RESET_FLAGS:
			s_rx_hdmi2x_hdcp_reset(p_obj, *(bool *)in_data);
			break;
#endif
		case SII_MOD_RX_HDMI_OPCODE__INPUT_420_EN:
			s_enable420_input(p_obj,
				*((bool *)in_data) ? true : false);
			break;
		case SII_MOD_RX_HDMI_OPCODE__RX_PLL_LOCK:
			p_obj->is_rx_pll_locked =
				*((bool *)in_data) ? true : false;
			break;
#ifdef RX_DESCRAMBLE_THROUGH_API
		case SII_MOD_RX_HDMI_OPCODE__RX_SCDC_CLK_RATIO:
			s_rx_scdc_tmds_config(p_obj,
				((bool *)in_data)[0], ((bool *)in_data)[1]);
			break;
#endif
		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

int sii_mod_rx_hdmi_get(void *rx_hdmi_obj, enum sii_mod_rx_hdmi_opcode op_code,
			void *out_data)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)rx_hdmi_obj;
	int status = 0;

	if (p_obj) {
		switch (op_code) {
		case SII_MOD_RX_HDMI_OPCODE__RPWR:
			memcpy(out_data, &(p_obj->is_plus5_v), sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH:
			memcpy(out_data, &(p_obj->bit_depth),
				sizeof(enum sii_drv_bit_depth));
			break;
		case SII_MOD_RX_HDMI_OPCODE__HDCP_CONTENT_TYPE:
			memcpy(out_data, &(p_obj->content_type),
				sizeof(enum sii_drv_hdcp_content_type));
			break;
		case SII_MOD_RX_HDMI_OPCODE__HDCP_PROTECTION:
			memcpy(out_data, &(p_obj->is_hdcp_proc),
				sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__HDCP_STAT:
			memcpy(out_data, &(p_obj->hdcp_stat),
				sizeof(enum sii_drv_hdcp_status));
			break;
		case SII_MOD_RX_HDMI_OPCODE__LINK_VERSION:
			memcpy(out_data, &(p_obj->hdcp_link_ver),
				sizeof(enum sii_drv_hdcp_link_version));
			break;
		case SII_MOD_RX_HDMI_OPCODE__AVMUTE:
			memcpy(out_data, &(p_obj->is_av_mute), sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__TMDS_MODE:
			memcpy(out_data, &(p_obj->tmds_mode),
				sizeof(enum sii_tmds_mode));
			break;
		case SII_MOD_RX_HDMI_OPCODE__IF_DATA:
		{
			/* info frame id should be passed by requesting layer*/
			struct sii_info_frame *p_if_data =
				(struct sii_info_frame *)out_data;
			if (p_if_data->if_id < SII_INFO_FRAME_ID__TOTAL)
				memcpy(out_data,
				&p_obj->if_data[p_if_data->if_id],
					sizeof(struct sii_info_frame));
			else
				status = 1;
			break;
		}
		case SII_MOD_RX_HDMI_OPCODE__CKDT_STATUS:
			memcpy(out_data, &(p_obj->clk_detect),
				sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__STMNG:
			memcpy(out_data, &(p_obj->rx_stmng_dat),
				sizeof(struct sii_drv_tx_strmng_2x_msg));
			break;
		case SII_MOD_RX_HDMI_OPCODE__SCDC_EVENTS:
			memcpy(out_data, &p_obj->rx_scdc_events,
				sizeof(uint32_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__HDCP2X_RPT_STATUS:
			memcpy(out_data, &(p_obj->hdcp_rx_rpt_msg.ske_event),
				sizeof(p_obj->hdcp_rx_rpt_msg.ske_event));
			break;
		case SII_MOD_RX_HDMI_OPCODE__SCDC_SINKVERSION:
			memcpy(out_data, &(p_obj->p_scdc_obj->sink_ver),
				sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__SCDC_SRCVERSION:
			memcpy(out_data, &(p_obj->p_scdc_obj->src_ver),
				sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__HPD:
			memcpy(out_data, &(p_obj->is_hot_plug),
				sizeof(uint8_t));
			break;
		case SII_MOD_RX_HDMI_OPCODE__SCDCS_CLK_RATIO:
			memcpy(out_data, &(p_obj->p_scdc_obj->clock_ratio),
				sizeof(uint8_t));
			break;
#ifdef RX_DESCRAMBLE_THROUGH_API
		case SII_MOD_RX_HDMI_OPCODE__RX_SCDC_SCRAMBLE_STAT:
		{
			uint8_t scramble_status = sii_platform_rd_reg8(
				p_obj->pstDevice,
				REG_ADDR__SCDCS_STATUS1);
			scramble_status &=
				BIT_MSK__SCDCS_STATUS1__SCDCS_SCRBL_STATUS;
			scramble_status >>= 3;
			memcpy(out_data, &scramble_status,
				sizeof(uint8_t));
		}
			break;
#endif
		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

struct scdc_obj g_stscdc_obj;
void *sii_mod_rx_scdc_init(pstSii9630_Device_t pstDevice,
	void *parent, void (*event_notify_fn)(void *, uint32_t))
{
	struct scdc_obj *p_obj = &g_stscdc_obj;

	//p_obj = kzalloc(sizeof(struct scdc_obj), GFP_KERNEL);
    memset(&g_stscdc_obj,0,sizeof(struct scdc_obj));
	if (!p_obj)
		return NULL;

	p_obj->scdc_events = 0;
	p_obj->event_notify_fn = event_notify_fn;
	//p_obj->drv_context = drv_context;
    p_obj->pstDevice = pstDevice;
	p_obj->parent = parent;
	return p_obj;
}

#if 0
static void sii_mod_rx_scdc_delete(struct scdc_obj *p_obj)
{
	//kfree(p_obj);
}
#endif

static int init_scdc_rx_regs(void *p_obj)
{
	struct scdc_obj *p_scdc_obj = (struct scdc_obj *)p_obj;
    pstSii9630_Device_t pstDev = p_scdc_obj->pstDevice;
	int status = 0;

	p_scdc_obj->scdc_events = 0;
	p_scdc_obj->sink_ver = sii_platform_rd_reg8(pstDev,
				REG_ADDR__SINK_VERSION);
	p_scdc_obj->src_ver = sii_platform_rd_reg8(pstDev,
				REG_ADDR__SOURCE_VERSION);
	status = rx_scdc_manufacturer_specific_data_set(p_scdc_obj,
			&s_scdc_manuf_info);

	return status;
}


#ifdef RX_DESCRAMBLE_THROUGH_API
/* This function is to enable the descrambleing if there
 * is no SCDC negotiation from Source.
 * scramble_en tmds_clk_ratio
 *	0	   x		--> Clear scramble_enable
 *	1	   0		--> Set scramble_enable for HDMI1.4 mode
 *	1	   1		--> Set scramble_enable for HDMI2.0 mode
 */
static void s_rx_scdc_tmds_config(struct rx_hdmi_obj *p_rx_obj, bool scramble_en,
	bool tmds_bit_clk_ratio)
{
    pstSii9630_Device_t pstDev = p_rx_obj->pstDevice;

	if (!scramble_en) {
		sii_platform_clr_bit8(pstDev, REG_ADDR__HDMI2_MODE_CTRL,
			BIT_MSK__HDMI2_MODE_CTRL__REG_SCRAMBLE_ON_OVR |
			BIT_MSK__HDMI2_MODE_CTRL__REG_SCRAMBLE_ON_VAL |
			BIT_MSK__HDMI2_MODE_CTRL__REG_HDMI2_ON_OVR |
			BIT_MSK__HDMI2_MODE_CTRL__REG_HDMI2_ON_VAL);
		p_rx_obj->p_scdc_obj->clock_ratio = false;
		sii_log_debug("Rx Descramble enable Clr.\n");
	} else {
		sii_log_debug("Rx Descramble enable Set.\n");
		if (tmds_bit_clk_ratio) {
			p_rx_obj->p_scdc_obj->clock_ratio = true;
			sii_platform_set_bit8(pstDev, REG_ADDR__HDMI2_MODE_CTRL,
				BIT_MSK__HDMI2_MODE_CTRL__REG_SCRAMBLE_ON_OVR |
				BIT_MSK__HDMI2_MODE_CTRL__REG_SCRAMBLE_ON_VAL |
				BIT_MSK__HDMI2_MODE_CTRL__REG_HDMI2_ON_OVR |
				BIT_MSK__HDMI2_MODE_CTRL__REG_HDMI2_ON_VAL);
		} else {
			p_rx_obj->p_scdc_obj->clock_ratio = false;
			sii_platform_set_bit8(pstDev, REG_ADDR__HDMI2_MODE_CTRL,
				BIT_MSK__HDMI2_MODE_CTRL__REG_SCRAMBLE_ON_OVR |
				BIT_MSK__HDMI2_MODE_CTRL__REG_SCRAMBLE_ON_VAL);
		}
	}

	p_rx_obj->p_scdc_obj->scdc_events |=
		SII_DRV_RX_EVENT__SCDCS_CLK_RATIO_CHNG |
		SII_DRV_RX_EVENT__SCDCS_SCRAMBLING_CHNG;
	if (p_rx_obj->p_scdc_obj->scdc_events &&
		p_rx_obj->p_scdc_obj->event_notify_fn) {
		p_rx_obj->p_scdc_obj->event_notify_fn(
			p_rx_obj->p_scdc_obj->parent,
			p_rx_obj->p_scdc_obj->scdc_events);
		p_rx_obj->p_scdc_obj->scdc_events = 0;
	}

	/*Notify the scdc events to the rx_hdmi module
	 *To configure the correcponding tmds_clock_ratio*/
	if (p_rx_obj->rx_hdmi_events && p_rx_obj->event_notify_fn) {
		p_rx_obj->event_notify_fn(&g_stdrv_obj,
			p_rx_obj->rx_hdmi_events);
		p_rx_obj->rx_hdmi_events = 0;
	}
}
#endif

static bool sii_mod_rx_scdc_interrupt_handler(struct scdc_obj *p_obj)
{
    pstSii9630_Device_t pstDev;// = p_rx_obj->pstDevice;
	uint8_t scdcinterrupts[2];
	uint8_t status;
	uint8_t scdcs_status1;
	uint8_t hdmi2_on_over = BIT_MSK__HDMI2_MODE_CTRL__REG_HDMI2_ON_OVR;

	if (NULL == p_obj)
		return FALSE;

    pstDev = p_obj->pstDevice;
	/* Read SCDC Interrupts */
	scdcinterrupts[0] = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_INTR9_AON);
	scdcinterrupts[1] = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_INTR7_AON);

	/* Clear SCDC Interrupts */
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR9_AON, scdcinterrupts[0]);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR7_AON, scdcinterrupts[1]);
#ifdef CONFIG_UNDEFINED_MACRO
	if (scdcinterrupts[0] & BIT_MSK__RX_INTR9_AON__REG_INTR9_STAT0_AON)
		sii_log_debug("src_ver value or rr_enable changed.\n");
	if (scdcinterrupts[0] & BIT_MSK__RX_INTR9_AON__REG_INTR9_STAT1_AON)
		sii_log_debug("status_update change.\n");
	if (scdcinterrupts[0] & BIT_MSK__RX_INTR9_AON__REG_INTR9_STAT2_AON)
		sii_log_debug("ced_update change.\n");
	if (scdcinterrupts[0] & BIT_MSK__RX_INTR9_AON__REG_INTR9_STAT3_AON)
		sii_log_debug("rr_test changed.\n");
	if (scdcinterrupts[0] & BIT_MSK__RX_INTR9_AON__REG_INTR9_STAT6_AON)
		sii_log_debug("char_errdet_cnt change.\n");
	if (scdcinterrupts[0] & BIT_MSK__RX_INTR9_AON__REG_INTR9_STAT7_AON)
		sii_log_debug("test_rd_req change.\n");
#endif

	if (scdcinterrupts[1] & BIT_MSK__RX_INTR7_AON__REG_INTR7_STAT6_AON) {
		uint8_t scdcs_status1;

		p_obj->scdc_events |= SII_DRV_RX_EVENT__SCDCS_SCRAMBLING_CHNG;
		sii_log_debug("scramble_en change.\n");

		scdcs_status1 = sii_platform_rd_reg8(pstDev,
					REG_ADDR__SCDCS_CONFIG_STATUS);
		if (scdcs_status1 &
			BIT_MSK__SCDCS_CONFIG_STATUS__SCDCS_SCRBL_ENABLE) {
			sii_platform_set_bit8(pstDev, REG_ADDR__HDMI2_MODE_CTRL,
				hdmi2_on_over);
			sii_platform_set_bit8(pstDev, REG_ADDR__DPLL_HDMI2,
				BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_OVR |
				BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_VAL);
			sii_log_debug("Rx scramble Detected.\n");
		} else {
			sii_platform_clr_bit8(pstDev, REG_ADDR__HDMI2_MODE_CTRL,
				hdmi2_on_over);
			sii_platform_clr_bit8(pstDev, REG_ADDR__DPLL_HDMI2,
				BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_OVR |
				BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_VAL);
			sii_log_debug("Rx scramble Not Detected.\n");
		}
	}
	if (scdcinterrupts[1] & BIT_MSK__RX_INTR7_AON__REG_INTR7_STAT7_AON) {
		sii_log_debug("tmds_clk_ratio change.\n");
		status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__SCDCS_CONFIG_STATUS);
		p_obj->scdc_events |= SII_DRV_RX_EVENT__SCDCS_CLK_RATIO_CHNG;

		if (status &
			BIT_MSK__SCDCS_CONFIG_STATUS__SCDCS_TMDS_BCLK_RATIO) {
			/* (TMDS bit
			 period)/(TMDS clock period) ratio is 1/40*/
			 p_obj->clock_ratio = true;
			 sii_platform_clr_bit8(pstDev, REG_ADDR__HDMI2_MODE_CTRL,
				hdmi2_on_over);
			 sii_platform_clr_bit8(pstDev, REG_ADDR__DPLL_HDMI2,
				BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_OVR |
				BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_VAL);
		} else {
			/* (TMDS bit period)/(TMDS clock period) ratio is 1/10*/
			p_obj->clock_ratio = false;
			scdcs_status1 = sii_platform_rd_reg8(pstDev,
						REG_ADDR__SCDCS_CONFIG_STATUS);
			if (scdcs_status1 &
			BIT_MSK__SCDCS_CONFIG_STATUS__SCDCS_SCRBL_ENABLE) {
				sii_platform_set_bit8(pstDev,
					REG_ADDR__HDMI2_MODE_CTRL,
					hdmi2_on_over);
				sii_platform_set_bit8(pstDev,
					REG_ADDR__DPLL_HDMI2,
					BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_OVR |
					BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_VAL);
				} else {
					sii_platform_clr_bit8(pstDev,
						REG_ADDR__HDMI2_MODE_CTRL,
					hdmi2_on_over);
					sii_platform_clr_bit8(pstDev,
					REG_ADDR__DPLL_HDMI2,
					BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_OVR |
					BIT_MSK__DPLL_HDMI2__RI_HDMI2_ON_VAL);
					}
		}
	}
	if (p_obj->scdc_events && p_obj->event_notify_fn) {
		p_obj->event_notify_fn(p_obj->parent, p_obj->scdc_events);
		p_obj->scdc_events = 0;
	}
	return TRUE;
}

static void s_rx_hdmi_hdcp_repeater_set(struct rx_hdmi_obj *p_obj,
					bool repeater_en)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (repeater_en) {
#if 0
// banner doesn's support HDCP 2.2 Rx
		sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_CTRL_0,
			BIT_MSK__RX_HDCP2X_CTRL_0__RI_HDCP2RX_REPEATER);
#endif
		sii_platform_set_bit8(pstDev, REG_ADDR__RX_BCAPS_SET,
			BIT_MSK__RX_BCAPS_SET__REG_REPEATER
				| BIT_MSK__RX_BCAPS_SET__REG_HDMI_CAPABLE);
	}
}

static int rx_scdc_manufacturer_specific_data_set(
	struct scdc_obj *p_obj, struct sii_mod_scdc_msd *p_msd)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t i;
	int status = 0;

	for (i = 0; i < NUM_MANUFACTRER_SPECIFIC_REGS; i++) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__SCDCS_MFCTR_SPCF0 + i,
			p_msd->mf_info[i]);
	}

	return status;
}

static void s_rx_hdmi_scdc_call_back(void *p_obj, uint32_t scdc_events)
{
	struct rx_hdmi_obj *rx_obj = (struct rx_hdmi_obj *)p_obj;

	rx_obj->rx_scdc_events = scdc_events;
	rx_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__SCDC_EVENT;
}

static void s_rx_hdmi_hot_plug_set(struct rx_hdmi_obj *p_obj, bool on_off)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	/* detect rising edge hot-plug request*/
	if (on_off && !p_obj->is_hot_plug) {
		p_obj->is_av_mute = false;

		s_info_frame_on_flags_clear(p_obj);

		/* catch any infoframe */
		sii_platform_set_bit8(pstDev, REG_ADDR__RX_INT_IF_CTRL,
			BIT_MSK__RX_INT_IF_CTRL__REG_NEW_AVI_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_SPD_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_AUD_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_MPEG_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_UNREC_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_ACP_ONLY
				| BIT_MSK__RX_INT_IF_CTRL__REG_NEW_VSI_ONLY);

		sii_platform_set_bit8(pstDev, REG_ADDR__RX_INT_IF_CTRL2,
			BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC1_ONLY
				| BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ISRC2_ONLY
				| BIT_MSK__RX_INT_IF_CTRL2__REG_NEW_ACR_ONLY);

	}

	p_obj->is_hot_plug = on_off;
	s_update_hot_plug(p_obj);
}

void s_rx_hdmi_hdcp_topology_set(
	struct rx_hdmi_obj *p_obj,
	const struct sii_drv_hdcp_topology *p_topology)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;

#if 0
// banner doesn's support HDCP 2.2 Rx
uint8_t bit_msk_max_dev_exceeded =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC;
	uint8_t bit_msk_max_casc_exceeded =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC;
	uint8_t bit_msk_rpt_hdcp1dev_dstrm =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP1DEV_DSTRM;
	uint8_t bit_msk_rpt_hdcp2rpt_dstrm =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP20RPT_DSTRM;

	if (BIT_MSK__RX_HDCP2X_GEN_STATUS__RO_HDCP2RX_MODE_SEL
		& sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_GEN_STATUS)) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DEVCNT,
			p_topology->device_count);
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DEPTH,
			p_topology->depth);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DETAIL,
			bit_msk_max_dev_exceeded,
			p_topology->max_devs_exceeded);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DETAIL,
			bit_msk_max_casc_exceeded,
			p_topology->max_cascade_exceeded);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DETAIL,
			bit_msk_rpt_hdcp1dev_dstrm,
			p_topology->hdcp1x_device_ds);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DETAIL,
			bit_msk_rpt_hdcp2rpt_dstrm,
			p_topology->hdcp20_repeater_ds);
	} else {
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS1,
			BIT_MSK__RX_SHD_BSTATUS1__REG_DEV_CNT,
			p_topology->device_count);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS2,
			BIT_MSK__RX_SHD_BSTATUS2__REG_DEPTH, p_topology->depth);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS1,
			BIT_MSK__RX_SHD_BSTATUS1__REG_DEV_EXCEED,
			p_topology->max_devs_exceeded);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS2,
			BIT_MSK__RX_SHD_BSTATUS2__RPT_CASC_EXCEED,
			p_topology->max_cascade_exceeded);
	}
#else
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS1,
			BIT_MSK__RX_SHD_BSTATUS1__REG_DEV_CNT,
			p_topology->device_count);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS2,
			BIT_MSK__RX_SHD_BSTATUS2__REG_DEPTH, p_topology->depth);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS1,
			BIT_MSK__RX_SHD_BSTATUS1__REG_DEV_EXCEED,
			p_topology->max_devs_exceeded);
		sii_platform_put_bit8(pstDev, REG_ADDR__RX_SHD_BSTATUS2,
			BIT_MSK__RX_SHD_BSTATUS2__RPT_CASC_EXCEED,
			p_topology->max_cascade_exceeded);
#endif
}

static void s_rx_hdmi_hdcp_status_reset(struct rx_hdmi_obj *p_obj)
{
#if 0
// banner doesn's support HDCP 2.2 Rx
#ifdef CONFIG_RX_HDCP2X_EN
#ifdef CONFIG_HDCP_REPEATER_EN
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	sii_platform_clr_bit8(pstDev, REG_ADDR__RX_HDCP2X_INTR1_MASK,
		BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B0
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B1
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B2
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B3
			| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B4);
#endif
#endif
#endif
	p_obj->hdcp_link_ver = SII_DRV_HDCP_LINK_VER__NONE;
	p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__OFF;
	p_obj->is_hdcp_proc = false;
}

static void s_hpd_timer_call_back(void *pobj)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)pobj;

	if (1) {
		s_rx_hdmi_hot_plug_set(p_obj, true);

		//up(&p_obj->drv_context->isr_lock);
	}
}

static void s_update_edid(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint16_t i, sum;
	uint8_t *p;

	/* make sure checksums are correct */
	for (i = 0, p = p_obj->edid.b, sum = 0; i < SII_EDID_BLOCK_SIZE - 1;
		i++)
		sum += p[i];
	p[SII_EDID_BLOCK_SIZE - 1] = ((~sum) + 1) & 0xFF;
	for (i = 0, p = p_obj->edid.b + SII_EDID_BLOCK_SIZE, sum = 0;
		i < SII_EDID_BLOCK_SIZE - 1; i++)
		sum += p[i];
	p[SII_EDID_BLOCK_SIZE - 1] = ((~sum) + 1) & 0xFF;

	sii_platform_fifo_write8(pstDev,
		REG_ADDR__EDID_FIFO_WR_DATA, p_obj->edid.b,
		2 * SII_EDID_BLOCK_SIZE);
	sii_log_debug("RX EDID set\n");
}

static void s_info_frame_on_flags_clear(struct rx_hdmi_obj *p_obj)
{
	uint8_t id;
	for (id = SII_INFO_FRAME_ID__AVI; id < SII_INFO_FRAME_ID__TOTAL; id++)
		p_obj->if_data[id].is_if_on = false;
	/* Notify top level to clear infoframe data */
	if (p_obj->event_notify_fn)
		p_obj->event_notify_fn(&g_stdrv_obj,
			SII_DRV_RX_EVENT__AVI_IF
			| SII_DRV_RX_EVENT__VS_IF
			| SII_DRV_RX_EVENT__AUDIO_IF
			| SII_DRV_RX_EVENT__ACR_IF);
}

static void s_enable420_input(struct rx_hdmi_obj *p_obj, bool b420_en)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	/*sii_platform_put_bit8(pstDev, REG_ADDR__FUNC_SEL,
		BIT_MSK__FUNC_SEL__REG_420MODE_EN,
		b420_en ? BIT_MSK__FUNC_SEL__REG_420MODE_EN : 0);*/
	/*todo: check if this what intended*/
	sii_platform_put_bit8(pstDev, REG_ADDR__FUNC_SEL,
		BIT_MSK__FUNC_SEL__REG_420_UPSMPL_EN,
		b420_en ? BIT_MSK__FUNC_SEL__REG_420_UPSMPL_EN : 0);
}

static void s_update_mute(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (BIT_MSK__RX_AUDP_STAT__HDMI_MUTE
		& sii_platform_rd_reg8(pstDev, REG_ADDR__RX_AUDP_STAT)) {

		if (!p_obj->is_av_mute) {
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AVMUTE_CHNG;
			p_obj->is_av_mute = true;
			sii_log_debug("+AVMUTE\n");
		}
	} else {
		if (p_obj->is_av_mute) {
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__AVMUTE_CHNG;
			p_obj->is_av_mute = false;
			sii_log_debug("-AVMUTE\n");
		}
	}
}

static void s_deep_colour_fifo_reset(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_PWD_SRST,
		BIT_MSK__RX_PWD_SRST__REG_DC_FIFO_RST);
	//usleep(1000);
	sii_platform_clr_bit8(pstDev, REG_ADDR__RX_PWD_SRST,
		BIT_MSK__RX_PWD_SRST__REG_DC_FIFO_RST);
}

static void s_update_color_depth(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t dc_clk;

	dc_clk = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_VIDIN_STS) &
	BIT_MSK__RX_VIDIN_STS__RO_COLOUR_DEPTH;

	switch (dc_clk) {
	case 0x04:
		p_obj->bit_depth = SII_DRV_BIT_DEPTH__8_BIT;
		break;
	case 0x05:
		p_obj->bit_depth = SII_DRV_BIT_DEPTH__10_BIT;
		break;
	case 0x06:
		p_obj->bit_depth = SII_DRV_BIT_DEPTH__12_BIT;
		break;
	default:
		p_obj->bit_depth = SII_DRV_BIT_DEPTH__8_BIT;
		break;
	}
	if (p_obj->bit_depth > SII_DRV_BIT_DEPTH__8_BIT) {
		sii_platform_set_bit8(pstDev, REG_ADDR__VIDEO_MODE_CTRL,
			BIT_MSK__VIDEO_MODE_CTRL__REG_DC_BYPASS);
	} else {
		sii_platform_clr_bit8(pstDev, REG_ADDR__VIDEO_MODE_CTRL,
			BIT_MSK__VIDEO_MODE_CTRL__REG_DC_BYPASS);
	}
}

static void s_video_state_handler(void *p_rx_hdmi_obj)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)p_rx_hdmi_obj;
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t rg;

	if (1) {

		/*Bug_45176: This timer is being called unexpectdly,
		 *so this is to overcome that*/
		if (p_obj->clk_detect && p_obj->sync_detect) {
			enum sii_tmds_mode tmds_mode = SII_TMDS_MODE__NONE;
			sii_drv_tx_tmds_mode_status_get(
				&g_stdrv_obj, &tmds_mode);
			if  (tmds_mode != SII_TMDS_MODE__NONE) {
				sii_log_debug(
					"s_video_state_handler called from no-where\n");
				//up(&p_obj->drv_context->isr_lock);
				return;
			}
		}

		p_obj->ckdt_scdt_timer_started = false;
		rg = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_STATE_AON);

		p_obj->clk_detect = rg & BIT_MSK__RX_STATE_AON__CKDT?TRUE:FALSE;
		p_obj->sync_detect = rg & BIT_MSK__RX_STATE_AON__SCDT?TRUE:FALSE;

		if (p_obj->clk_detect && p_obj->sync_detect) {
			sii_timer_stop(&p_obj->inst_timer_clk_sync);
			if (!(sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_HPD_C_CTRL)
				& BIT_MSK__RX_HPD_C_CTRL__REG_HPD_C_CTRL)) {
				/* Rx HPD is not yet set.
				 * Will handle this later
				 */
				//up(&p_obj->drv_context->isr_lock);
				return;
			}
			sii_log_debug("CKDT/SCDT ON\n");
			if (BIT_MSK__RX_AUDP_STAT__HDMI_MODE_EN
				& sii_platform_rd_reg8(pstDev,
					REG_ADDR__RX_AUDP_STAT)) {

				sii_log_debug("RX_TMDS_MODE:HDMI\n");
				p_obj->tmds_mode = SII_TMDS_MODE__HDMI1;
			} else {
				sii_log_debug("RX_TMDS_MODE:DVI\n");
				p_obj->tmds_mode = SII_TMDS_MODE__DVI;
			}

			/* give a DC fifo reset to avoid any
			 * disturbances in video*/
            s_deep_colour_fifo_reset(p_obj);

			/*If AVI with standard CEA VIC is not rcvd then
			 *Will consider the video is non-standard resolution
			 *and wait for 100ms to get stable video timings*/
			if (!(p_obj->if_data[SII_INFO_FRAME_ID__AVI].is_if_on
				&& p_obj->if_data[SII_INFO_FRAME_ID__AVI].b[7]
				)) {
				/*sii_log_debug(
				"Starting stable video timings timer\n");*/
				sii_timer_start(&p_obj->inst_timer_stable_video_timings,
					100);
				p_obj->rx_hdmi_events |=
					(SII_DRV_RX_EVENT__CHAN_STAT_CHNG |
					SII_DRV_RX_EVENT__AUDIO_FRM_CHNG);
			} else {
				p_obj->rx_hdmi_events |=
					(SII_DRV_RX_EVENT__TMDS_MODE_CHNG
					| SII_DRV_RX_EVENT__CHAN_STAT_CHNG
					| SII_DRV_RX_EVENT__AUDIO_FRM_CHNG);
			}
			s_update_blanking_levels(p_obj);
			if (p_obj->event_notify_fn) {
				p_obj->event_notify_fn(
					&g_stdrv_obj,
					p_obj->rx_hdmi_events);
				p_obj->rx_hdmi_events = 0;
			}
#if 0
// banner doesn's support HDCP 2.2 Rx
#ifdef CONFIG_RX_HDCP2X_EN
#ifdef CONFIG_HDCP_REPEATER_EN
			sii_platform_set_bit8(pstDev,
				REG_ADDR__RX_HDCP2X_INTR1_MASK,
				BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B0
				| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B1
				| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B2
				| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B3
				| BIT_MSK__RX_HDCP2X_INTR1_MASK__INTR1_MASK_B4
				);
#endif
#endif
#endif
		}
		//up(&p_obj->drv_context->isr_lock);
	}
}

static void s_stable_video_timing_handler(void *p_obj)
{
	struct rx_hdmi_obj *p_rx_hdmi_obj = (struct rx_hdmi_obj *)p_obj;
	if (1) {
		/*sii_log_debug(
		"Notifying rx tmds mode change event\n");*/
		if (p_rx_hdmi_obj->event_notify_fn) {
			p_rx_hdmi_obj->event_notify_fn(
				&g_stdrv_obj,
				SII_DRV_RX_EVENT__TMDS_MODE_CHNG);
		}
	}
}

static void s_clk_sync_timeout_handler(void *p_rx_hdmi_obj)
{
	struct rx_hdmi_obj *p_obj = (struct rx_hdmi_obj *)p_rx_hdmi_obj;
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t rg;

	if (1) {
		rg = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_STATE_AON);
		p_obj->clk_detect = rg & BIT_MSK__RX_STATE_AON__CKDT?TRUE:FALSE;
		p_obj->sync_detect = rg & BIT_MSK__RX_STATE_AON__SCDT?TRUE:FALSE;
		if (p_obj->clk_detect && p_obj->sync_detect) {
			sii_log_debug("clock sync detected start tx forcefully..\n");

			p_obj->tmds_mode = SII_TMDS_MODE__NONE;
			p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__OFF;
			p_obj->rx_hdmi_events |=
				(SII_DRV_RX_EVENT__TMDS_MODE_CHNG |
				SII_DRV_RX_EVENT__AUTH_CHNG);

			if (p_obj->event_notify_fn) {
				p_obj->event_notify_fn(
					&g_stdrv_obj,
					p_obj->rx_hdmi_events);
				p_obj->rx_hdmi_events = 0;
			}

			//up(&p_obj->drv_context->isr_lock);
			s_video_state_handler(p_obj);
		} else {
			//up(&p_obj->drv_context->isr_lock);
		}
	}
}

static void s_update_hot_plug(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	if (p_obj->is_hot_plug) {
		if (p_obj->edid_ready) {

			sii_log_debug("RX_HPD ON\n");
			sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HPD_C_CTRL, 0);
			usleep(20000);
			sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HPD_C_CTRL,
				BIT_MSK__RX_HPD_C_CTRL__REG_HPD_C_CTRL);

			if (p_obj->clk_detect && p_obj->sync_detect) {
				p_obj->tmds_mode = SII_TMDS_MODE__NONE;
				p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__OFF;
				p_obj->rx_hdmi_events |=
					(SII_DRV_RX_EVENT__TMDS_MODE_CHNG |
					SII_DRV_RX_EVENT__AUTH_CHNG);

				if (p_obj->event_notify_fn) {
					p_obj->event_notify_fn(
						&g_stdrv_obj,
						p_obj->rx_hdmi_events);
					p_obj->rx_hdmi_events = 0;
				}
				/* 500ms for clk stability */
				sii_timer_start(&p_obj->inst_timer_video_state,
					SYNC_CHANGE_TIMEOUT);
			}
		} else
			sii_log_debug("RX_HPD OFF as RX EDID is not ready\n");
	} else {
		sii_log_debug("RX_HPD OFF\n");
#ifndef CONFIG_RX_CTS
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HPD_C_CTRL, 0);
#endif
		s_rx_hdmi_hdcp_status_reset(p_obj);
		sii_timer_stop(&p_obj->inst_timer_clk_sync);
	}
	/* notify to controller that RX hpd is changed */
	if (p_obj->event_notify_fn) {
		p_obj->event_notify_fn(&g_stdrv_obj,
			SII_DRV_RX_EVENT__HPD_CHANGE);
	}
}

static void s_update_blanking_levels(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t *avi_data = p_obj->if_data[SII_INFO_FRAME_ID__AVI].b;
	enum color_space clrspc = (enum color_space)(avi_data[4] >> 5);
	if ((p_obj->tmds_mode == SII_TMDS_MODE__HDMI1)
		|| (p_obj->tmds_mode == SII_TMDS_MODE__HDMI2)) {

		if ((avi_data[4] >> 5) == RGB) {
			sii_log_debug("RGB - ");
			sii_log_print("%dbit - ", p_obj->bit_depth);
			if (((avi_data[6] & 0x0C) >> 2) == 0)
				sii_log_print("Default\n");
			else if (((avi_data[6] & 0x0C) >> 2) == 1)
				sii_log_print("Limited\n");
			else if (((avi_data[6] & 0x0C) >> 2) == 2)
				sii_log_print("Full\n");

			sii_platform_wr_reg8(pstDev,
				REG_ADDR__VID_BLANK0, 0x0);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__VID_BLANK1, 0x0);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__VID_BLANK2, 0x0);

		} else if (((avi_data[4] >> 5) == YCC_422)
			|| ((avi_data[4] >> 5) == YCC_444)
			|| ((avi_data[4] >> 5) == YCC_420)) {

			sii_log_debug("YCC%s - ",
				(clrspc == YCC_444) ? "444"
				: (clrspc == YCC_422) ? "422" : "420");
			sii_log_print("%dbit - ", p_obj->bit_depth);

			if ((avi_data[8] >> 6) == 0) {
				sii_log_print("Limited\n");
				if (p_obj->bit_depth ==
					SII_DRV_BIT_DEPTH__8_BIT) {
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK0, 0x80);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK1, 0x0);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK2, 0x80);
				} else if (p_obj->bit_depth ==
					SII_DRV_BIT_DEPTH__10_BIT) {
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK0, 0x66);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK1, 0x0);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK2, 0x66);
				} else if (p_obj->bit_depth ==
					SII_DRV_BIT_DEPTH__12_BIT) {
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK0, 0xA4);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK1, 0x0);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK2, 0xA4);
				}

			} else if ((avi_data[8] >> 6) == 1) {
				sii_log_print("Full\n");
				if (p_obj->bit_depth ==
					SII_DRV_BIT_DEPTH__8_BIT) {
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK0, 0x80);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK1, 0x10);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK2, 0x80);
				} else if (p_obj->bit_depth ==
					SII_DRV_BIT_DEPTH__10_BIT) {
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK0, 0x66);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK1, 0x0);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK2, 0x66);
				} else if (p_obj->bit_depth ==
					SII_DRV_BIT_DEPTH__12_BIT) {
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK0, 0xA4);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK1, 0x0);
					sii_platform_wr_reg8(pstDev,
						REG_ADDR__VID_BLANK2, 0xA4);
				}
			}
		}

	} else if (p_obj->tmds_mode == SII_TMDS_MODE__DVI) {
		sii_log_debug("Updating Blanking levels in DVI Mode\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__VID_BLANK0, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__VID_BLANK1, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__VID_BLANK2, 0x0);
	}

    UNUSED(clrspc);
}

static void s_update_rx_auth_status(struct rx_hdmi_obj *p_obj,
					uint8_t inst_stat)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
    uint8_t stat;
#if 0
// banner doesn's support HDCP 2.2 Rx
	uint8_t stat = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_HDCP2X_GEN_STATUS);

	if (BIT_MSK__RX_HDCP2X_GEN_STATUS__RO_HDCP2RX_MODE_SEL & stat) {

		p_obj->hdcp_link_ver = SII_DRV_HDCP_LINK_VER__HDCP22;
		stat = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR0);
		if (stat & BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT0) {
			if (stat & BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT1) {
				sii_log_debug(
					"RX:HDCP2.2 authentication failed\n");
				p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__FAILED;
				p_obj->is_hdcp_proc = false;
			} else {
				sii_log_debug("RX:HDCP2.2 authenticated\n");
				p_obj->hdcp_stat =
					SII_DRV_HDCP_STATUS__SUCCESS_22;
				p_obj->is_hdcp_proc = true;
				s_rx_hdcp_set_version(p_obj,
					SII_DRV_HDCP_LINK_VER__HDCP22);
			}
		}
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR0,
			stat & (BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT0 |
				BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT1));
	} else
#endif
	{
		p_obj->hdcp_link_ver = SII_DRV_HDCP_LINK_VER__HDCP1X;
		if (inst_stat & BIT_MSK__RX_INTR1__REG_INTR1_STAT1) {
			sii_log_debug("RX:HDCP1.4 authenticating\n");
			p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__AUTHENTICATING;
		}
		if (inst_stat & BIT_MSK__RX_INTR1__REG_INTR1_STAT0) {
			stat = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_HDCP_STAT);

			if (stat & BIT_MSK__RX_HDCP_STAT__HDCP_AUTHENTICATED) {
				sii_log_debug("RX:HDCP1.4 authenticated\n");
				p_obj->hdcp_stat =
					SII_DRV_HDCP_STATUS__SUCCESS_1X;
				s_rx_hdcp_set_version(p_obj,
					SII_DRV_HDCP_LINK_VER__HDCP1X);

			} else {
				sii_log_debug(
					"RX:HDCP1.4 authentication failed\n");

				p_obj->hdcp_stat = SII_DRV_HDCP_STATUS__FAILED;
				s_rx_hdcp_set_version(p_obj,
					SII_DRV_HDCP_LINK_VER__NONE);
			}
			if (stat & BIT_MSK__RX_HDCP_STAT__HDCP_DECRYPTING)
				p_obj->is_hdcp_proc = true;
			else
				p_obj->is_hdcp_proc = false;
		}
	}
	/*if (p_obj->tmds_mode == SII_TMDS_MODE__NONE) {
		sii_timer_stop(&p_obj->inst_timer_video_state);
		p_obj->is_video_state_handler_timer_call = false;
		s_video_state_handler(p_obj);
		p_obj->is_video_state_handler_timer_call = true;
	}*/
}

static void s_mod_rx_hdmi_clear_info_frame(struct rx_hdmi_obj *p_obj,
						enum sii_info_frame_id if_id)
{
	if (if_id < SII_INFO_FRAME_ID__TOTAL) {
		p_obj->if_data[if_id].is_if_on = false;
		memset(p_obj->if_data[if_id].b, 0, SII_INFOFRAME_MAX_LEN);
		p_obj->if_data[if_id].if_id = if_id;
	}
}

#if 0
// banner doesn's support HDCP 2.2 Rx
static void s_rx_hdcp2x_intr_handler(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t rx_auth_status;
	uint8_t rx_msg_status;

	rx_msg_status = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR1);
	rx_auth_status = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR1, rx_msg_status);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_INTR0, rx_auth_status);
	sii_log_debug("RX_HDCP:rxAuthStatus = %x rxMsgStatus = %x",
		rx_auth_status, rx_msg_status);
#ifdef CONFIG_HDCP_REPEATER_EN
	if (rx_auth_status || rx_msg_status) {
		/*ake sent received*/
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT2) {
			s_rx_hdcp_set_version(p_obj,
				SII_DRV_HDCP_LINK_VER__HDCP22);
			sii_log_debug("RX_HDCP:AKE sent received\n");
			p_obj->hdcp_rx_rpt_msg.ske_event = false;
			p_obj->hdcp_rx_rpt_msg.rcv_id_change = false;
			p_obj->hdcp_rx_rpt_msg.ake_init_rcvd = true;
			if (p_obj->hdcp_rx_rpt_msg.rcv_id_enable == false) {
				/* sii_platform_wr_reg8(pstDev,
				 REG_ADDR__RX_HDCP2X_RPT_DEPTH, 0x00);*/
			}
		}
		/* rx ske sent received*/
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT3) {
			p_obj->hdcp_rx_rpt_msg.ske_event = true;
			s_hdcp2_rx_send_receiver_i_d_list(p_obj);
			p_obj->rx_hdmi_events |= SII_DRV_RX_EVENT__SKE_EVENT;
			sii_log_debug("RX_HDCP:SKE sent received\n");
		}
		/*ro_rpt_rcvid_xfer_done*/
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT4) {
			s_hdcp2x_rx_sendnext_seqnum(p_obj);
			sii_log_debug("RX_HDCP:Rx receiver xfer done\n");
		}
		/*repeater stream manamge msg changed*/
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT1) {
			s_update_content_type(p_obj);
			sii_log_debug("RX_HDCP:Stream Manage Received\n");
		}
		/*repeater stream manamge xfer done*/
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT5)
			sii_log_debug("RX_HDCP:Stream Manage xfer done\n");
		/*rx receiver id changed*/
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT0) {
			p_obj->hdcp_rx_rpt_msg.rcv_id_change = true;
			sii_log_debug(
				"RX_HDCP:Rx receiver change id received\n");
		}
		if (rx_msg_status & BIT_MSK__RX_HDCP2X_INTR1__INTR1_STAT6)
			sii_log_debug("RX_HDCP:Rx Ske Certi sent received\n");
		if (rx_auth_status & BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT2)
			sii_log_debug("RX_HDCP: !!! repeater ready !!!!\n");
		if (rx_auth_status & BIT_MSK__RX_HDCP2X_INTR0__INTR0_STAT6)
			sii_log_debug("RX_HDCP: !!! re_auth request !!!!\n");

		if (p_obj->hdcp_stat == SII_DRV_HDCP_STATUS__SUCCESS_22)
			sii_log_debug("RX_HDCP: HDCP 2.2 auth success\n");
		else if (p_obj->hdcp_stat == SII_DRV_HDCP_STATUS__FAILED)
			sii_log_debug(
				"RX_HDCP:Rx Failed -- send receiver ID's\n");
	}
#endif

}
#endif

#if 0
// banner doesn's support HDCP 2.2 Rx
#ifdef CONFIG_HDCP_REPEATER_EN
static void s_hdcp2_rx_set_reauth_req(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	sii_log_info("RX_HDCP:Rx Reauthentication send.\n");
	sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_CTRL_1,
		BIT_MSK__RX_HDCP2X_CTRL_1__RI_HDCP2_REAUTH_SW);
}

static void s_update_content_type(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint32_t seq_m = 0;
	uint8_t bit_msk_smng_rd_start =
		BIT_MSK__RX_HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_SMNG_RD_START;

	p_obj->rx_stmng_dat.stream_k = sii_platform_rd_reg8(pstDev,
		REG_ADDR__RX_HDCP2X_RPT_SMNG_K);
	sii_platform_block_read8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_M_0,
		(uint8_t *)&p_obj->hdcp_rx_rpt_msg.seq_num__m, 3);

	if ((p_obj->hdcp_rx_rpt_msg.seq_num__m != seq_m)
		|| (p_obj->rx_stmng_dat.stream_k != 1)) {
		sii_log_info("RX_HDCP:Receiver Invalid K or Invalid M.\n");
		s_hdcp2_rx_set_reauth_req(p_obj);
	} else {
		p_obj->hdcp_rx_rpt_msg.smng_enable = true;
		p_obj->hdcp_rx_rpt_msg.seq_num__m++;

		sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
				bit_msk_smng_rd_start);
		p_obj->rx_stmng_dat.stream_i_d = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_HDCP2X_RX_RPT_SMNG_OUT);
		sii_platform_clr_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
				bit_msk_smng_rd_start);

		sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
				bit_msk_smng_rd_start);
		p_obj->rx_stmng_dat.content_type = sii_platform_rd_reg8(pstDev,
				REG_ADDR__RX_HDCP2X_RX_RPT_SMNG_OUT);
		sii_platform_clr_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
				bit_msk_smng_rd_start);

		p_obj->content_type = p_obj->rx_stmng_dat.content_type;

		sii_log_debug("Content Type:%x\n",
			p_obj->rx_stmng_dat.content_type);
		p_obj->rx_hdmi_events |=
			SII_DRV_RX_EVENT__HDCP2X_CONTENT_TYPE_CHNG;
	}
}

static void s_hdcp2x_rx_sendnext_seqnum(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint32_t ul_seq_num = 0;

	ul_seq_num = sii_platform_rd_reg8(pstDev,
			REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_0);
	ul_seq_num = ((ul_seq_num << 8)
		| sii_platform_rd_reg8(pstDev,
			REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_1));
	ul_seq_num = ((ul_seq_num << 8)
		| sii_platform_rd_reg8(pstDev,
			REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_1));

	ul_seq_num++;

	sii_log_debug("Sending rcvid done Message..\n");
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_0,
		(p_obj->hdcp_rx_rpt_msg.seq_num__v & 0xff));
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_1,
		((p_obj->hdcp_rx_rpt_msg.seq_num__v >> 8) & 0xff));
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_2,
		((p_obj->hdcp_rx_rpt_msg.seq_num__v >> 16) & 0xff));

}

/*---------------------------------------------------------------------------*/

static void s_hdcp2_rx_send_receiver_i_d_list(struct rx_hdmi_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t bit_msk_hdcp2x_rpt_rcvid_xfer_strt =
		BIT_MSK__RX_HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_XFER_START;
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_0, 0x00);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_1, 0x00);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RX_SEQ_NUM_V_2, 0x00);

	if (p_obj->hdcp_rx_rpt_msg.rcv_id_enable == true) {
		sii_platform_clr_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
			bit_msk_hdcp2x_rpt_rcvid_xfer_strt);
		sii_platform_set_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
			bit_msk_hdcp2x_rpt_rcvid_xfer_strt);
		sii_log_debug("RX_HDCP:start rcvd transfer\n");
	}
}
#endif // CONFIG_HDCP_REPEATER_EN
#endif
/*---------------------------------------------------------------------------*/

static void s_rx_hdcp_set_version(struct rx_hdmi_obj *p_obj, uint8_t hdcp_ver)
{
	p_obj->rx_hdcp_ver = (enum sii_drv_hdcp_link_version)(hdcp_ver);
}

/*--------------------------------------------------------------------------*/
#if 0
enum sii_drv_hdcp_link_version s_rx_hdcp_get_version(struct rx_hdmi_obj *p_obj)
{
	return p_obj->rx_hdcp_ver;
}
#endif
/*--------------------------------------------------------------------------*/

#if 0
// banner doesn's support HDCP 2.2 Rx
static void s_rx_hdmi2x_hdcp_rcvid_set(
	struct rx_hdmi_obj *p_obj,
	struct sii_drv_hdcp2x_rpt_rcv_id_list *p_rpt_rcvid)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	bool is_rxrpt_mx_casc_exc = false;
	bool is_rxrpt_mx_devs_exc = false;
	uint8_t rx_rpt_misc = 0;
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t temp;

	uint8_t bit_msk_max_dev_exceeded =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC;
	uint8_t bit_msk_max_casc_exceeded =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC;
	uint8_t bit_msk_rpt_hdcp1dev_dstrm =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP1DEV_DSTRM;
	uint8_t bit_msk_rpt_hdcp2rpt_dstrm =
		BIT_MSK__RX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP20RPT_DSTRM;
	uint8_t bit_msk_rcvid_wr_start =
		BIT_MSK__RX_HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_WR_START;
	uint8_t bit_msk_rcvid_wr =
		BIT_MSK__RX_HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_WR;
	uint16_t reg_addr_rcvid_in = REG_ADDR__RX_HDCP2X_RX_RPT_RCVID_IN;
	uint8_t bit_msk_hdcp2x_rpt_rcvid_xfer_strt =
		BIT_MSK__RX_HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_XFER_START;

	temp = sii_platform_rd_reg8(pstDev, REG_ADDR__RX_HDCP2X_STATE);
	sii_log_debug("RX_HDCP: rx auth state  =%x\n", temp);

	if ((p_obj->hdcp_rx_rpt_msg.ake_init_rcvd == true)
		&& (s_rx_hdcp_get_version(p_obj)
			== SII_DRV_HDCP_LINK_VER__HDCP22)) {
		sii_log_info("Device Count! %02x  Device Depth %02x\n",
			p_rpt_rcvid->devcnt, p_rpt_rcvid->depth);

		if ((p_rpt_rcvid->depth == MAX_DEPTH)
			|| (p_rpt_rcvid->casc_exceed))
			is_rxrpt_mx_casc_exc = true;
		if ((p_rpt_rcvid->devcnt == MAX_DEVICE_COUNT)
			|| (p_rpt_rcvid->dev_exceed))
			is_rxrpt_mx_devs_exc = true;
		rx_rpt_misc |=
			is_rxrpt_mx_devs_exc ?
				bit_msk_max_dev_exceeded :
				false;
		rx_rpt_misc |=
			is_rxrpt_mx_casc_exc ?
				bit_msk_max_casc_exceeded :
				false;
		rx_rpt_misc |=
			(p_rpt_rcvid->hdcp2ds_rpt) ?
				bit_msk_rpt_hdcp2rpt_dstrm :
				false;
		rx_rpt_misc |=
			(p_rpt_rcvid->hdcp1ds_rpt) ?
				bit_msk_rpt_hdcp1dev_dstrm :
				false;

		sii_platform_put_bit8(pstDev, (REG_ADDR__RX_HDCP2X_RPT_DETAIL),
				(bit_msk_rpt_hdcp1dev_dstrm |
				 bit_msk_rpt_hdcp2rpt_dstrm |
				 bit_msk_max_dev_exceeded |
				 bit_msk_max_casc_exceeded),
			rx_rpt_misc);
		if (!(p_rpt_rcvid->casc_exceed || p_rpt_rcvid->dev_exceed)) {
			sii_platform_clr_bit8(pstDev,
				REG_ADDR__RX_HDCP2X_RX_CTRL_0,
				bit_msk_rcvid_wr_start);
			sii_platform_set_bit8(pstDev,
				REG_ADDR__RX_HDCP2X_RX_CTRL_0,
				bit_msk_rcvid_wr_start);
			for (i = 0; i < RCVRID_LENGTH; i++) {
				sii_platform_wr_reg8(pstDev,
					reg_addr_rcvid_in,
					p_obj->rcv_id[i]);
				sii_platform_set_bit8(pstDev,
					REG_ADDR__RX_HDCP2X_RX_CTRL_0,
					bit_msk_rcvid_wr);
				sii_platform_clr_bit8(pstDev,
					REG_ADDR__RX_HDCP2X_RX_CTRL_0,
					bit_msk_rcvid_wr);
			}
			for (j = 0; j < p_rpt_rcvid->devcnt; j++) {
				for (i = 0; i < RCVRID_LENGTH; i++) {
					sii_platform_wr_reg8(pstDev,
						reg_addr_rcvid_in,
						p_rpt_rcvid->rpt_rcvid[j].
							id[i]);
					sii_platform_set_bit8(pstDev,
						REG_ADDR__RX_HDCP2X_RX_CTRL_0,
						bit_msk_rcvid_wr);
					sii_platform_clr_bit8(pstDev,
						REG_ADDR__RX_HDCP2X_RX_CTRL_0,
						bit_msk_rcvid_wr);
				}
			}
			sii_log_info(
				"Writing receiver ID's into Fifo done..\n");
		}
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DEVCNT,
			(p_rpt_rcvid->devcnt + 1));
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DEPTH,
			(p_rpt_rcvid->depth + 1));
		sii_log_info("transfer reciever id's done:%x\n",
			p_rpt_rcvid->devcnt + 1);

		if (p_obj->rx_rcv_id_send == true) {
			if (p_obj->hdcp_rx_rpt_msg.ske_event == true) {
				sii_platform_clr_bit8(pstDev,
					REG_ADDR__RX_HDCP2X_RX_CTRL_0,
					bit_msk_hdcp2x_rpt_rcvid_xfer_strt);
				sii_platform_set_bit8(pstDev,
					REG_ADDR__RX_HDCP2X_RX_CTRL_0,
					bit_msk_hdcp2x_rpt_rcvid_xfer_strt);
				sii_log_info(
					"Receiver ID's transfer initiated..\n");
			} else {

				sii_log_info("Rx is not ready--");
				sii_log_info("SKE event is not received yet\n");
			}
		} else
			p_obj->rx_rcv_id_send = true;

		p_obj->hdcp_rx_rpt_msg.rcv_id_enable = true;
	}

}
#endif
/*---------------------------------------------------------------------------*/

static void s_rx_hdmi_edid_set(struct rx_hdmi_obj *p_obj,
				struct sii_edid *p_edid)
{
	p_obj->edid = *p_edid;
	s_update_edid(p_obj);
	p_obj->edid_ready = true;
}


/*---------------------------------------------------------------------------*/

#if 0
// banner doesn's support HDCP 2.2 Rx
static void s_rx_hdmi2x_hdcp_reset(struct rx_hdmi_obj *p_obj, bool enable)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t bit_msk_hdcp2x_rpt_rcvid_xfer_strt =
		BIT_MSK__RX_HDCP2X_RX_CTRL_0__RI_HDCP2RX_RPT_RCVID_XFER_START;
	if (p_obj->rx_rcv_id_send) {
		p_obj->hdcp_rx_rpt_msg.rcv_id_enable =
			(enable) ? true : false;
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_HDCP2X_RPT_DEPTH, 0);
	sii_platform_clr_bit8(pstDev, REG_ADDR__RX_HDCP2X_RX_CTRL_0,
		bit_msk_hdcp2x_rpt_rcvid_xfer_strt);
	}
}
#endif

void mapp_sil9630RxHdmiHandler(void *p_obj)
{
    s_hpd_timer_call_back(p_obj);
    s_video_state_handler(p_obj);
    s_stable_video_timing_handler(p_obj);
    s_clk_sync_timeout_handler(p_obj);
}

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
