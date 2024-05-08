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

#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_drv_cp8630.h"
#include "Sii9630_si_mod_tx_hdcp.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_timer.h"

/*Donot change this value. Some repeaters take more time*/
#define HDCP2X_RETRY_THRESHOLD			5

#define HDCP2X_CONTENT_TYPE_SET_MAX_TIMEOUT	10
#define SII_HDCP2X_RCVID_LENGTH			5

#define LEN_TPI__DDC_FIFO_SIZE			16

#define TIMER_START__TX_HDCP__INTR		150
#define TIMER_START__TX_HDCP__STATE		40
#define TIMER_START__TX_HDCP__INTR_INTVAL	10
#define TIMER_START__TX_HDCP__STATE_INTVAL	20
#define TIMER_START__TX_HDCP__RPT_RCVID_FAIL	3000
#define TIMER_START__TX_HDCP__RPT_FAIL		1500
#define TIMER_START__TX_HDCP__RPT_REAUTH_REQ	100
#define TIMER_START__TX_HDCP__DDC_FAIL		2000
#define TIMER_START__TX_HDCP__REAUTH_WAIT	200
/*! HDCP part 2 is successfully done */
#define PART2_DONE (BIT_MSK__TPI_COPP_DATA1__REG_COPP_GPROT | \
	BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT)
#define PART1_DONE BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT
/* page 8:0x0_c - REG_ADDR__TX_HDCP2X_AUTH_STAT*/
#define BIT_ENUM__HDCP2X_AUTH_STAT__AUTH_DONE			0x01
#define BIT_ENUM__HDCP2X_AUTH_STAT__AUTH_FAIL			0x02
#define BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_DONE			0x10
#define BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_FAIL			0x20
#define HDCP2X_CCHK_FAIL	(BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_DONE | \
	BIT_ENUM__HDCP2X_AUTH_STAT__CCHK_FAIL)

/* page 6:0x29 - REG_ADDR__TPI_COPP_DATA1*/
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_NORMAL		0x00
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_LOST		  0x10
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_FAILED		0x20
#define BIT_ENUM__TPI_HDCP_QUERY__STATUS_SUSPENDED	 0x30

#define HDCP_ENABLE__ENCRY			0x04
#define HDCP1X_KSV_FIFO_READY	0x20

/**
 * @brief HDCP state
 */
enum sii_drv_tx_hdcp_internal_state {
	SII_MOD_HDCP_EVENT__OFF,
	SII_MOD_HDCP_EVENT__WAIT_FOR_START,
	SII_MOD_HDCP_EVENT__WAIT_FOR_DONE,
	SII_MOD_HDCP_EVENT__WAIT_FIFO_READY,
	SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE,
	SII_MOD_HDCP_EVENT__AUTHENTICATED,
	SII_MOD_HDCP_EVENT__V_CHECK,
	SII_MOD_HDCP_EVENT__FAILED,

};

#if 0
/*! DDC bus access error codes */
enum ddc_com_err {
	SI_TX_DDC_ERROR_CODE_NO_ERROR = 0x00,
	SI_TX_DDC_ERROR_CODE_TIMEOUT = 0x01,
	SI_TX_DDC_ERROR_CODE_NO_ACK = 0x02,
	SI_TX_DDC_ERROR_CODE_BUSY = 0x03,
	SI_TX_DDC_ERROR_CODE_TX_HW = 0x04,
	SI_TX_DDC_ERROR_CODE_LIM_EXCEED = 0x05
};
#endif

struct sii_hdcp_int_stat {
	uint8_t reg0;
	uint8_t reg1;
	uint8_t reg2;
};

struct hdcp_obj {
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;
	struct sii_hdcp_int_stat hdcp1x_int_stat;
	struct sii_hdcp_int_stat hdcp2x_int_stat;
	uint8_t auth_fail_counter;

	/*--------------------------------*/
	/* user request states			*/
	/*--------------------------------*/
	bool is_auth_requested;
	/*!< set if downstream authentication is requested */
	enum sii_drv_hdcp_status hdcp_status;

	/*--------------------------------*/
	/* internal states				*/
	/*--------------------------------*/
	enum sii_drv_tx_hdcp_internal_state auth_state;
	/*!< authentication state sii_drv_hdcp_state_t */
	enum sii_drv_tx_hdcp_internal_state prev_auth_state;
	/*!< previous value of auth_state */
	struct sii_drv_hdcp_ksv_list ksv_list;
	struct sii_drv_hdcp_topology hdcp_topology;

	void *timer_ddc_poll; /*!< timer for ddc status check */
	void *timer1x_re_auth;
#ifdef CONFIG_POWER_DOWN_EN
	void *timer_hdcp2x_pwr_up_done_poll;
#endif
	void (*evnt_notify_fn)(void *, uint32_t);

	struct sii_drv_hdcp2x_rpt_msg b_tx_2x_rpt_msg;
	enum sii_drv_hdcp_failure error_state;
	enum sii_drv_hdcp_failure prev_error_state;
	uint8_t is_ds_hdcp22_supported; /*!< down stream HDCP2.2 capability */
	/*!< hdcp2.2 content type */
	enum sii_drv_hdcp_content_type hdcp_content_type;
	struct sii_drv_hdcp2x_rpt_rcv_id_list hdcp2x_rpt_rcv_id_list;
	void *timer_rpt_rcv_id_poll;
	void *timer2x_rpt_re_auth;
	bool rpt_reauth_done;
	bool tx_hpd_status;
	bool isddcfailed;
	struct sii_drv_tx_strmng_2x_msg strmng_2x_msg;
	enum sii_drv_hdcp_link_version ds_auth_support;
	enum sii_drv_hdcp_link_version hdcp_version;
	bool hdcp2x_repeater_ready;
#ifdef CONFIG_POWER_DOWN_EN
	bool hdcp2x_power_up_done;
#endif
};

static void s_update_hdcp_state(struct hdcp_obj *p_hdcp_obj);
#ifdef CONFIG_TX_HDCP2X_EN

static void s_notify_hdcp_status_to_upstream(struct hdcp_obj *p_hdcp_obj,
						uint32_t event_flags);
static bool is_d_sdevice_hdcp2_repeater(struct hdcp_obj *p_hdcp_obj);
static void s_hdcp2_content_type_set(
	struct hdcp_obj *p_hdcp_obj,
	enum sii_drv_hdcp_content_type p_content_type);
static void s_hdcp2_content_type_get(
	struct hdcp_obj *p_hdcp_obj,
	enum sii_drv_hdcp_content_type *p_content_type);
static bool s_hdcp2x_intr_handler(struct hdcp_obj *p_hdcp_obj);
static bool s_wait_for_ddc_bus(struct hdcp_obj *p_hdcp_obj);
static void hdcp2x_auth_start(struct hdcp_obj *p_hdcp_obj);
static void s_tpi_hdcp2x_protection_enable(struct hdcp_obj *p_hdcp_obj,
			bool is_enabled);
#ifndef CONFIG_HDCP_REPEATER_EN
static void s_hdcp2x_non_rpt_intr_handler(struct hdcp_obj *p_hdcp_obj);
static void s_rcv_id_list_get(struct hdcp_obj *p_hdcp_obj);
static void s_hdcp2x_receiver_rcvids(struct hdcp_obj *p_hdcp_obj);
#else

static void supdate_hdcp2_rpt_rcvids(struct hdcp_obj *p_hdcp_obj,
					uint8_t *p_rcv_ids);
static void s_tx2x_rpt_rcv_id_timeout_handler(void *p_obj);
static void s_tx2x_repeater_re_auth_handler(void *p_obj);
static bool s_rpt_rcvd_list(struct hdcp_obj *p_hdcp_obj);
static void s_hdcp2x_rpt_intr_handler(struct hdcp_obj *p_hdcp_obj);
static void s_hdcp2x_rpt_re_auth_send(struct hdcp_obj *p_hdcp_obj,
					bool re_auth_delay_check);
static void s_hdcp2x_receiver_rcvids(struct hdcp_obj *p_hdcp_obj);
#endif
#if (CONFIG_SOURCE_ONLY == 1)
static void s_hdcp2x_send_strmng_2x_msg(struct hdcp_obj *p_hdcp_obj);
#endif
static void s_hdcp2x_fill_strmng_2x_msg(struct hdcp_obj *p_hdcp_obj,
		struct sii_drv_tx_strmng_2x_msg *p_stmng);
#endif
static enum ddc_com_err s_read_ds_hdcp22_capability(
	struct hdcp_obj *p_hdcp_obj);
static enum sii_drv_hdcp_link_version s_ds_hdcp_ver_get(
	struct hdcp_obj *p_hdcp_obj);

static void s_reset_ksv_fifo(struct hdcp_obj *p_hdcp_obj);
static void s_clear_sw_tpi_bit(struct hdcp_obj *p_hdcp_obj);
static int s_start_hdcp(struct hdcp_obj *p_hdcp_obj, bool b_enable);
static void s_tpi_hdcp_protection_enable(struct hdcp_obj *p_hdcp_obj,
						bool is_enabled);
static bool s_tpi_is_downstream_hdcp_available(struct hdcp_obj *p_hdcp_obj);
static uint8_t s_tpi_hdcp_status_get(struct hdcp_obj *p_hdcp_obj);
static bool s_tpi_hdcp_is_part2_done(struct hdcp_obj *p_hdcp_obj);
static void s_tpi_bksv_get(struct hdcp_obj *p_hdcp_obj);
static bool s_tpi_ksv_list_portion_size_get(struct hdcp_obj *p_hdcp_obj,
						uint8_t *p_bytes_to_read);
static void s_tpi_ksv_list_get(struct hdcp_obj *p_hdcp_obj, uint8_t *p_buffer,
				uint8_t length);
static enum sii_drv_tx_hdcp_internal_state s_tpi_get_ksv_list(
	struct hdcp_obj *p_hdcp_obj, uint8_t ds_bstatus[2]);
static void s_tpi_b_status_get(struct hdcp_obj *p_hdcp_obj,
				uint8_t *p_ds_b_status);
static void s_print_hdcp_status(struct hdcp_obj *p_hdcp_obj);
static void s_print_hdcp_query_status(struct hdcp_obj *p_hdcp_obj,
					uint8_t query);
static bool s_hdcp_state_machine_handler(void *p_obj);
static bool s_hdcp1x_intr_handler(struct hdcp_obj *p_hdcp_obj);
static void s_tpi_hdcp_encryption_enable(struct hdcp_obj *p_hdcp_obj,
						bool is_enable);
static bool ds_hdcp22_support_get(struct hdcp_obj *p_hdcp_obj);

static void s_update_hdcp_topology(struct hdcp_obj *p_hdcp_obj);
/* notify functions*/
static void s_notify_hdcp_status(struct hdcp_obj *p_hdcp_obj);
static void s_hdcp_ddc_ack_poll(void *p_hdcp_obj);
static void s_hdcp_restart_1xauth(void *p_hdcp_obj);
#ifdef CONFIG_POWER_DOWN_EN
static void s_hdcp2x_pwr_up_monitor(void *p_hdcp_obj);
static void hdcp2x_pdn_mode_auth_start(struct hdcp_obj *p_hdcp_obj);
#endif
static void s_process_hdcp_ds_repeater(struct hdcp_obj *p_hdcp_obj);
static void s_reset_tpi_state_machine(struct hdcp_obj *p_hdcp_obj);
static void s_set_sw_tpi_bit(struct hdcp_obj *p_hdcp_obj);

static void reset_states(struct hdcp_obj *p_hdcp_obj)
{
	p_hdcp_obj->is_auth_requested = false;
	p_hdcp_obj->hdcp_status = SII_DRV_HDCP_STATUS__OFF;
	p_hdcp_obj->ksv_list.length = 0;
	p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__OFF;
	p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__NONE;
	p_hdcp_obj->is_ds_hdcp22_supported = false;
	p_hdcp_obj->hdcp2x_int_stat.reg0 = 0;
	p_hdcp_obj->hdcp2x_int_stat.reg1 = 0;
	p_hdcp_obj->auth_fail_counter = 0;
	p_hdcp_obj->isddcfailed = false;

	/*set default HDCP topology*/
	p_hdcp_obj->hdcp_topology.depth = 0;
	p_hdcp_obj->hdcp_topology.device_count = 0;
	p_hdcp_obj->hdcp_topology.hdcp1x_device_ds = 0;
	p_hdcp_obj->hdcp_topology.hdcp20_repeater_ds = 0;
	p_hdcp_obj->hdcp_topology.max_cascade_exceeded = 0;
	p_hdcp_obj->hdcp_topology.max_devs_exceeded = 0;
	p_hdcp_obj->strmng_2x_msg.stream_k = 0x01;
	p_hdcp_obj->strmng_2x_msg.content_type = SII_DRV_HDCP_CONTENT_TYPE__0;
	p_hdcp_obj->hdcp_version = SII_DRV_HDCP_LINK_VER__NONE;
	p_hdcp_obj->hdcp2x_repeater_ready = false;
#ifdef CONFIG_POWER_DOWN_EN
	p_hdcp_obj->hdcp2x_power_up_done = false;
#endif
	memset(p_hdcp_obj->ksv_list.p_list_start, 0,
		sizeof(SII_DRV_MAX_DS_DEV_SUPPORT
		* sizeof(struct sii_drv_hdcp_ksv)));
}

static struct sii_drv_hdcp_ksv sg_SiiDrvHdcpKsv[SII_DRV_MAX_DS_DEV_SUPPORT];
static struct hdcp_obj sg_st_hdcp_obj;

/* public functions */
void *sii_mod_tx_hdcp_init(pstSii9630_Device_t pstDevice,
			void (*event_notify_fn)(void *, uint32_t))
{
	struct hdcp_obj *p_hdcp_obj = &sg_st_hdcp_obj;
	int tstatus = 0;
	/*removed typecast to avoid checkpatch warning */
	/* allocate memory for object */
	//p_hdcp_obj = kzalloc(sizeof(struct hdcp_obj),GFP_KERNEL);
	memset(&sg_st_hdcp_obj,0,sizeof(struct hdcp_obj));

	if (!p_hdcp_obj)
		return NULL;

    //sii_log_debug("Hdcp Tx Init start\n");
    p_hdcp_obj->pstDevice = pstDevice;
	p_hdcp_obj->evnt_notify_fn = event_notify_fn;

#if (CONFIG_SOURCE_ONLY == 1)
	sii_log_debug("HDCP SOURCE ONLY Mode\n");
#else
	sii_log_debug("HDCP REPEATER Mode\n");
#endif

	//p_hdcp_obj->ksv_list.p_list_start = p_hdcp_obj->ksv_list.p_list =
	//		kzalloc(SII_DRV_MAX_DS_DEV_SUPPORT
	//		* sizeof(struct sii_drv_hdcp_ksv), GFP_KERNEL);
	memset(&sg_SiiDrvHdcpKsv,0,sizeof(struct sii_drv_hdcp_ksv)*SII_DRV_MAX_DS_DEV_SUPPORT);
	p_hdcp_obj->ksv_list.p_list_start = p_hdcp_obj->ksv_list.p_list = (uint8_t *)&sg_SiiDrvHdcpKsv[0];
	if (!p_hdcp_obj->ksv_list.p_list)
		goto free_mem;

#ifdef CONFIG_HDCP_REPEATER_EN
	tstatus = sii_timer_create(s_tx2x_rpt_rcv_id_timeout_handler,
		p_hdcp_obj, &p_hdcp_obj->timer_rpt_rcv_id_poll,eTimerSil_tx_hdcp_tx2x_rpt_rcv_id_timeout);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate");
		sii_log_debug("HDCP_2x_Repeater_RCVID_Check timer!\n");
		goto free_mem;
	}

	tstatus = sii_timer_create(s_tx2x_repeater_re_auth_handler,
		p_hdcp_obj, &p_hdcp_obj->timer2x_rpt_re_auth,eTimerSil_tx_hdcp_tx2x_repeater_re_auth);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate");
		sii_log_debug("HDCP_2x_Repeater_Reauth_Check timer!\n");
		goto free_mem;
	}
#endif
	tstatus = sii_timer_create(s_hdcp_ddc_ack_poll,
		p_hdcp_obj, &p_hdcp_obj->timer_ddc_poll,eTimerSil_tx_hdcp_hdcp_ddc_ack_poll);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate HDCP DDC ACK handler timer!\n");
		goto free_mem;
	}

	tstatus = sii_timer_create(s_hdcp_restart_1xauth,
		p_hdcp_obj, &p_hdcp_obj->timer1x_re_auth,eTimerSil_tx_hdcp_hdcp_restart_1xauth);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate HDCP 1.4 re_auth_timer!\n");
		goto free_mem;
	}
#ifdef CONFIG_POWER_DOWN_EN
	tstatus = sii_timer_create(s_hdcp2x_pwr_up_monitor, p_hdcp_obj,
		&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll,eTimerSil_tx_hdcp_hdcp2x_pwr_up_monitor);
	if (tstatus != 0) {
		sii_log_debug("Failed to alloc Rx pll monitor timer!\n");
		goto free_mem;
	}
#endif
	goto done;
free_mem:
	//sii_mod_tx_hdcp_exit(p_hdcp_obj);
	return NULL;
done:
    //sii_log_debug("Hdcp Tx Init End\n");
	return p_hdcp_obj;
}

#if 0
void sii_mod_tx_hdcp_exit(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;

	if (!p_hdcp_obj)
		return;
#ifdef CONFIG_POWER_DOWN_EN
	if (p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll) {
		sii_timer_stop(
			&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll);
		sii_timer_delete(
			&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll);
		p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll = NULL;
	}
#endif
	if (p_hdcp_obj->timer1x_re_auth) {
		sii_timer_stop(
			&p_hdcp_obj->timer1x_re_auth);
		sii_timer_delete(
			&p_hdcp_obj->timer1x_re_auth);
		p_hdcp_obj->timer1x_re_auth = NULL;
	}
	if (p_hdcp_obj->timer_ddc_poll) {
		sii_timer_stop(
			&p_hdcp_obj->timer_ddc_poll);
		sii_timer_delete(
			&p_hdcp_obj->timer_ddc_poll);
		p_hdcp_obj->timer_ddc_poll = NULL;
	}
	if (p_hdcp_obj->timer2x_rpt_re_auth) {
		sii_timer_stop(
			&p_hdcp_obj->timer2x_rpt_re_auth);
		sii_timer_delete(
			&p_hdcp_obj->timer2x_rpt_re_auth);
		p_hdcp_obj->timer2x_rpt_re_auth = NULL;
	}
	if (p_hdcp_obj->timer_rpt_rcv_id_poll) {
		sii_timer_stop(
			&p_hdcp_obj->timer_rpt_rcv_id_poll);
		sii_timer_delete(
			&p_hdcp_obj->timer_rpt_rcv_id_poll);
		p_hdcp_obj->timer_rpt_rcv_id_poll = NULL;
	}
	/*removed the check condition to avoid checkpatch
	 * warning : kfree is safe*/
	/*if (p_hdcp_obj->ksv_list.p_list_start)*/
	//kfree(p_hdcp_obj->ksv_list.p_list_start);

	//kfree(p_hdcp_obj);
}
#endif

int init_hdcp_tx_regs(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	int status = 0;

	reset_states(p_hdcp_obj);

	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTL_1,
		BIT_MSK__TX_HDCP2X_CTL_1__REG_HDCP2X_HPD_OVR
		| BIT_MSK__TX_HDCP2X_CTL_1__REG_HDCP2X_HPD_SW
		| BIT_MSK__TX_HDCP2X_CTL_1__REG_HDCP2X_REAUTH_SW);

	/* HDCP2X TP0=2*/
    // 24MHz is divided by 8
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP0, 0x02);
	/* HDCP2X TP1=24, 20_m_hz->98, 24_m_hz->117*/
    // (24MHz / 8 / 256 x 12 )/1000 = 140.625, Setting to 0x8C=11.947ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP1, 0x8C);
	/* HDCP2X TP2=1*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP2, 0x01);
	/* HDCP2X TP3=50*/
    // DDC hang timeout 0x32 = 597ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP3, 0x32);
	/* HDCP2X TP4=30*/
    // H check timeout, storedkm case
    // 0x1E = 358ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP4, 0x1E);
	/* HDCP2X TP5=120*/
    // H check timeout, no storedkm case
    // 0x78 = 1433ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP5, 0x78);
	/* HDCP2X TP6=2*/
    //Locality check timeout
    // 0x02 = 24ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP6, 0x02);
	/* HDCP2X TP7=10*/
    // M check timeout
    // 0x0A = 119ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP7, 0x0A);
	/* HDCP2X TP8=10 cert read timeout*/
    // 0x0a = 119ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP8, 0x0A);
	/* HDCP2X TP9=20*/
    // Pairing timeout
    // 0x14 = 239ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP9, 0x14);
	/* HDCP2X TP10=22*/
    // 0x16 = 263ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP10, 0x16);
	/* HDCP2X TP11=200*/
    // V check timeout
    // 0xc8 = 2389ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP11, 0xC8);
	/* HDCP2X TP12=150*/
    // Retry wait time
    // 0x96 = 1792ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP12, 0x96);
	/* HDCP2X TP13=16*/
    // Stream_ready to Enc_En wait time
    // 0x10 = 191ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP13, 0x10);
	/* HDCP2X TP14=200*/
    // wait time for V
    // 0xC8 was wrong it is only 2.4sec. Must be bigger than 3sec
    // 0xFE = 3034ms
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP14, 0xFE);
	/* HDCP2X TP15=0*/
	/* Reduce R0 calculation time */
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_HW_OPT1,	0x03);

	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TP15, 0x00);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_MISC, 0x00);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_GP_IN2, 0x80);
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL,
		BIT_MSK__TX_HDCP2X_AESCTL__RI_AES_RST_AUTHDONE);

	p_hdcp_obj = p_hdcp_obj;
	return status;
}

bool sii_mod_tx_hdcp_interrupt_handler(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj;
	struct sii_hdcp_int_stat int_stat = {0};
    pstSii9630_Device_t pstDev;// = p_hdcp_obj->pstDevice;

	p_hdcp_obj = (struct hdcp_obj *)p_obj;
	if (!p_hdcp_obj)
		return FALSE;

	pstDev = p_hdcp_obj->pstDevice;

#ifdef CONFIG_TX_HDCP2X_EN
	if (ds_hdcp22_support_get(p_hdcp_obj) &&
		(p_hdcp_obj->hdcp_version == SII_DRV_HDCP_LINK_VER__HDCP22)) {
		s_hdcp2x_intr_handler(p_hdcp_obj);
		s_hdcp_state_machine_handler(p_hdcp_obj);
		return FALSE;
	}
#endif
	/* capture and mask interrupt status bits. (ignore status bits and
	 non-serving interrupts)*/
    int_stat.reg0 = sii_platform_rd_reg8(pstDev, REG_ADDR__TPI_INTR_ST0);
    sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INTR_ST0,
        int_stat.reg0);

	int_stat.reg1 = int_stat.reg0 & BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST3;
    int_stat.reg0 = int_stat.reg0 & (BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST7
                                    | BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST2);


	if (memcmp(&p_hdcp_obj->hdcp1x_int_stat, &int_stat,
		sizeof(struct sii_hdcp_int_stat))) {
		uint8_t intreg = 0;

		/* find any interrupt status bit that changed to '1'*/
		intreg |= ((p_hdcp_obj->hdcp1x_int_stat.reg0 ^ int_stat.reg0)
			& int_stat.reg0);
		intreg |= ((p_hdcp_obj->hdcp1x_int_stat.reg1 ^ int_stat.reg1)
			& int_stat.reg1);

		p_hdcp_obj->hdcp1x_int_stat = int_stat;
		if (intreg) {
			/* call derived interupt handler*/
			s_hdcp1x_intr_handler(p_hdcp_obj);
			s_hdcp_state_machine_handler(p_hdcp_obj);
		}
	}
	return TRUE;
}

int sii_mod_tx_hdcp_set(void *p_obj, enum sii_mod_hdcp_internal_opcode opcode,
			const void *in_data)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;
	int status = 0;

	if (p_hdcp_obj) {
		switch (opcode) {
		case SII_MOD_HDCP_OPCODE__ENABLE:
		{
			uint8_t hdcp_enable = *(uint8_t *)in_data;

			status = s_start_hdcp(p_hdcp_obj, (bool)hdcp_enable);
			break;
		}
		case SII_MOD_HDCP_OPCODE__CONTENT_TYPE:
		{
#ifdef CONFIG_TX_HDCP2X_EN
			enum sii_drv_hdcp_content_type content_type =
			*(enum sii_drv_hdcp_content_type *)in_data;
			s_hdcp2_content_type_set(p_hdcp_obj, content_type);
#else
			sii_log_info("Hdcp2.2 is Disabled on TX\n");
			status = 1;
#endif
			break;
		}
		case SII_MOD_HDCP_OPCODE__STMNG_SET:
#ifdef CONFIG_TX_HDCP2X_EN
			s_hdcp2x_fill_strmng_2x_msg(p_hdcp_obj, (struct sii_drv_tx_strmng_2x_msg *)in_data);
#else
			sii_log_info("Hdcp2.2 is Disabled on TX\n");
			status = 1;
#endif
			break;
		case SII_MOD_HDCP_OPCODE__RCVID_SET:
#ifdef CONFIG_TX_HDCP2X_EN
#ifdef CONFIG_HDCP_REPEATER_EN
			supdate_hdcp2_rpt_rcvids(p_hdcp_obj,
					(uint8_t *)in_data);
#endif
#else
			sii_log_info("Hdcp2.2 is Disabled on TX\n");
			status = 1;
#endif
			break;
		case SII_MOD_HDCP_OPCODE__UPDATE_HPD_STATUS:
			memcpy(&p_hdcp_obj->tx_hpd_status, in_data,
					sizeof(bool));
			if (!p_hdcp_obj->tx_hpd_status) {
				if (p_hdcp_obj->timer1x_re_auth)
					sii_timer_stop(&p_hdcp_obj->timer1x_re_auth);
			}
			break;
		case SII_MOD_HDCP_OPCODE__DS_AUTH_TYPE:
			memcpy(&p_hdcp_obj->ds_auth_support,
				in_data, sizeof(uint8_t));
			break;
#ifdef CONFIG_POWER_DOWN_EN
		case SII_MOD_HDCP_OPCODE__POWER_UP_STATUS:
			memcpy(&p_hdcp_obj->hdcp2x_power_up_done,
				in_data,
				sizeof(bool));
			/*hdcp2x_pdn_mode_auth_start(p_hdcp_obj);*/
			break;
#endif
		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

int sii_mod_tx_hdcp_get(void *p_obj, enum sii_mod_hdcp_internal_opcode opcode,
			void *out_data)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;
	int status = 0;

	if (p_hdcp_obj) {
		switch (opcode) {
		case SII_MOD_HDCP_OPCODE__STATUS:
			/*s_update_hdcp_state(p_hdcp_obj);*/
			memcpy(out_data, &(p_hdcp_obj->hdcp_status),
				sizeof(p_hdcp_obj->hdcp_status));
			break;
		case SII_MOD_HDCP_OPCODE__VERSION:
		{
			enum sii_drv_hdcp_link_version ds_hdcp_ver =
				p_hdcp_obj->hdcp_version;
			memcpy(out_data, &ds_hdcp_ver, sizeof(ds_hdcp_ver));
			break;
		}
		case SII_MOD_HDCP_OPCODE__BKSV_LIST:
			memcpy(out_data, &(p_hdcp_obj->ksv_list),
				sizeof(p_hdcp_obj->ksv_list));
			break;
		case SII_MOD_HDCP_OPCODE__TOPOLOGY:
			s_update_hdcp_topology(p_hdcp_obj);
			memcpy(out_data, &(p_hdcp_obj->hdcp_topology),
				sizeof(p_hdcp_obj->hdcp_topology));
			break;
		case SII_MOD_HDCP_OPCODE__RPT_RCVID_GET:
#ifdef CONFIG_TX_HDCP2X_EN
			memcpy(out_data,
				&(p_hdcp_obj->hdcp2x_rpt_rcv_id_list),
				sizeof(p_hdcp_obj->hdcp2x_rpt_rcv_id_list));
#else
			sii_log_info("Hdcp2.2 is Disabled on TX\n");
			status = 1;
#endif
			break;
		case SII_MOD_HDCP_OPCODE__RCVID_GET:
				memcpy(out_data,
					&(p_hdcp_obj->ksv_list.p_list_start),
					RCVRID_LENGTH);
			break;
		case SII_MOD_HDCP_OPCODE__CONTENT_TYPE:
		{
#ifdef CONFIG_TX_HDCP2X_EN
			s_hdcp2_content_type_get(p_hdcp_obj,
				(enum sii_drv_hdcp_content_type *)out_data);
#else
			sii_log_info("Hdcp2.2 is Disabled on TX\n");
			status = 1;
#endif
			break;
		}
		case SII_MOD_HDCP_OPCODE__RPTR_MODE:
		{
			enum sii_drv_hdcp_link_version ds_hdcp_ver =
				p_hdcp_obj->hdcp_version;
			enum hdcp_repeater_mode rptr_mode = TX_HDCP_RPTR_NONE;
			if(ds_hdcp_ver == SII_DRV_HDCP_LINK_VER__HDCP1X) {
				if (sii_platform_rd_reg8(p_hdcp_obj->pstDevice,
						REG_ADDR__TPI_COPP_DATA1)
						& BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP)
					rptr_mode = TX_HDCP_RPTR_1X;
			} else if (ds_hdcp_ver == SII_DRV_HDCP_LINK_VER__HDCP22) {
				if (sii_platform_rd_reg8(p_hdcp_obj->pstDevice,
						REG_ADDR__TX_HDCP2X_GEN_STATUS)
						& BIT_MSK__TX_HDCP2X_GEN_STATUS__RO_HDCP2_REPEATER)
					rptr_mode = TX_HDCP_RPTR_22;
			} else {
				rptr_mode = TX_HDCP_RPTR_NONE;
			}
			memcpy(out_data, &rptr_mode, sizeof(enum hdcp_repeater_mode));
			break;
		}
		case SII_MOD_HDCP_OPCODE__FAIL_REASON:
			memcpy(out_data, &p_hdcp_obj->error_state,
				sizeof(enum sii_drv_hdcp_failure));
			break;
		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

int sii_mod_tx_hdcp_stop_timers(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;
	int status = 0;

	if (!p_hdcp_obj)
		return 1;

	/*stop all timers*/
#ifdef CONFIG_POWER_DOWN_EN
	if (p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll) {
		sii_timer_stop(
			&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll);
		/*sii_timer_delete(
			&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll);
		p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll = NULL;*/
	}
#endif
	if (p_hdcp_obj->timer1x_re_auth) {
		sii_timer_stop(
			&p_hdcp_obj->timer1x_re_auth);
		/*sii_timer_delete(
			&p_hdcp_obj->timer1x_re_auth);
		p_hdcp_obj->timer1x_re_auth = NULL;*/
	}
	if (p_hdcp_obj->timer_ddc_poll) {
		sii_timer_stop(
			&p_hdcp_obj->timer_ddc_poll);
		/*sii_timer_delete(
			&p_hdcp_obj->timer_ddc_poll);
		p_hdcp_obj->timer_ddc_poll = NULL;*/
	}
	if (p_hdcp_obj->timer2x_rpt_re_auth) {
		sii_timer_stop(
			&p_hdcp_obj->timer2x_rpt_re_auth);
		/*sii_timer_delete(
			&p_hdcp_obj->timer2x_rpt_re_auth);
		p_hdcp_obj->timer2x_rpt_re_auth = NULL;*/
	}
	if (p_hdcp_obj->timer_rpt_rcv_id_poll) {
		sii_timer_stop(
			&p_hdcp_obj->timer_rpt_rcv_id_poll);
		/*sii_timer_delete(
			&p_hdcp_obj->timer_rpt_rcv_id_poll);
		p_hdcp_obj->timer_rpt_rcv_id_poll = NULL;*/
	}
	return status;
}

static void s_dump_buffer(uint8_t *p_data, uint16_t len)
{
	while (len--) {
		sii_log_print("%02X ", *p_data);
		p_data++;
	}
	sii_log_print("\n");
}
/*****************************************************************************/
/**
 * @brief HDCP enable/_disable control
 *
 * @param[in]  p_hdcp_obj	handle to HDCP object
 * @param[in]  b_enable	set 'true' to request an HDCP protected link.
 *
 *****************************************************************************/
static void s_update_hdcp_state(struct hdcp_obj *p_hdcp_obj)
{
    //pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	enum sii_drv_hdcp_link_version ds_hdcp_ver =
				s_ds_hdcp_ver_get(p_hdcp_obj);
	switch (p_hdcp_obj->auth_state) {
	case SII_MOD_HDCP_EVENT__OFF:
		p_hdcp_obj->hdcp_status = SII_DRV_HDCP_STATUS__OFF;
		//sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD, 0x7F);
		break;
	case SII_MOD_HDCP_EVENT__WAIT_FOR_START:
	case SII_MOD_HDCP_EVENT__WAIT_FOR_DONE:
	case SII_MOD_HDCP_EVENT__WAIT_FIFO_READY:
	case SII_MOD_HDCP_EVENT__V_CHECK:
		p_hdcp_obj->hdcp_status = SII_DRV_HDCP_STATUS__AUTHENTICATING;
		break;
	case SII_MOD_HDCP_EVENT__AUTHENTICATED:
		if (SII_DRV_HDCP_LINK_VER__HDCP22 == ds_hdcp_ver)
			p_hdcp_obj->hdcp_status =
				SII_DRV_HDCP_STATUS__SUCCESS_22;
		else
			p_hdcp_obj->hdcp_status =
				SII_DRV_HDCP_STATUS__SUCCESS_1X;
		break;
	case SII_MOD_HDCP_EVENT__FAILED:
		p_hdcp_obj->hdcp_status = SII_DRV_HDCP_STATUS__FAILED;
		break;
	default:
		break;
	}
}

static enum sii_drv_hdcp_link_version s_ds_hdcp_ver_get(
	struct hdcp_obj *p_hdcp_obj)
{
	enum sii_drv_hdcp_link_version ret_val = SII_DRV_HDCP_LINK_VER__HDCP1X;

	if (ds_hdcp22_support_get(p_hdcp_obj))
		ret_val = SII_DRV_HDCP_LINK_VER__HDCP22;
	else
		ret_val = SII_DRV_HDCP_LINK_VER__HDCP1X;

	return ret_val;
}

static void s_update_hdcp_topology(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	/* defined to abide 80 character rule */
	uint8_t bit_msk_max_dev_exceeded =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC;
	uint8_t bit_msk_max_casc_exceeded =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC;
	uint8_t bit_msk_rpt_hdcp1dev_dstrm =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP1DEV_DSTRM;
	uint8_t bit_msk_rpt_hdcp2rpt_dstrm =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP20RPT_DSTRM;

	if (ds_hdcp22_support_get(p_hdcp_obj)) {
		uint8_t hdcp_misc_status;

		p_hdcp_obj->hdcp_topology.depth =
			sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_RPT_DEPTH);
		p_hdcp_obj->hdcp_topology.device_count =
			sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_RPT_DEVCNT);

		hdcp_misc_status = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_RPT_DETAIL);
		p_hdcp_obj->hdcp_topology.max_devs_exceeded =
			(hdcp_misc_status & bit_msk_max_dev_exceeded) ?
				true : false;
		p_hdcp_obj->hdcp_topology.max_cascade_exceeded =
			(hdcp_misc_status & bit_msk_max_casc_exceeded) ?
				true : false;
		p_hdcp_obj->hdcp_topology.hdcp20_repeater_ds =
			(hdcp_misc_status & bit_msk_rpt_hdcp2rpt_dstrm) ?
				true : false;
		p_hdcp_obj->hdcp_topology.hdcp1x_device_ds =
			(hdcp_misc_status & bit_msk_rpt_hdcp1dev_dstrm) ?
				true : false;
	} else {
		uint8_t a_ds_b_status[2];

		s_tpi_b_status_get(p_hdcp_obj, a_ds_b_status);

		p_hdcp_obj->hdcp_topology.depth = (a_ds_b_status[1]
			& BIT_MSK__TPI_BSTATUS2__REG_DS_DEPTH);
		p_hdcp_obj->hdcp_topology.device_count = (a_ds_b_status[0]
			& BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT);

		p_hdcp_obj->hdcp_topology.max_devs_exceeded =
			(a_ds_b_status[0]
				& BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_EXCEED) ?
				true : false;
		p_hdcp_obj->hdcp_topology.max_cascade_exceeded =
			(a_ds_b_status[1]
				& BIT_MSK__TPI_BSTATUS2__REG_DS_CASC_EXCEED) ?
				true : false;
		p_hdcp_obj->hdcp_topology.hdcp20_repeater_ds = false;
		p_hdcp_obj->hdcp_topology.hdcp1x_device_ds = false;
	}
}

static void s_reset_ksv_fifo(struct hdcp_obj *p_hdcp_obj)
{
	sii_log_debug("Resetting KSV fifo!!\n");
	p_hdcp_obj->ksv_list.length = 0;
	p_hdcp_obj->ksv_list.p_list = p_hdcp_obj->ksv_list.p_list_start;
}

static void s_set_sw_tpi_bit(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_log_debug("Set SW TPI bit!!\n");

	sii_platform_set_bit8(pstDev, REG_ADDR__LM_DDC,
		BIT_MSK__LM_DDC__REG_SW_TPI_EN);
}

static void s_clear_sw_tpi_bit(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_log_debug("Clear SW TPI bit!!\n");
	sii_platform_clr_bit8(pstDev, REG_ADDR__LM_DDC,
		BIT_MSK__LM_DDC__REG_SW_TPI_EN);
}

static void s_stop_hdcp1x_auth(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	if (p_hdcp_obj->timer1x_re_auth) {
		sii_timer_stop(
			&p_hdcp_obj->timer1x_re_auth);
	}
	sii_timer_stop(
		&p_hdcp_obj->timer_ddc_poll);

	p_hdcp_obj->isddcfailed = false;
	s_tpi_hdcp_encryption_enable(p_hdcp_obj, false);
	/* disable_hdcp1x intr masks*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_INTR_EN, 0x0);
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_COPP_DATA2,
		BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN);

	sii_platform_set_bit8(pstDev, REG_ADDR__TPI_COPP_DATA2,
		BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN);
	s_tpi_hdcp_protection_enable(p_hdcp_obj, false);
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_COPP_DATA2,
		BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN);
	/* prog DDC delay counter*/
	/*sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_HW_OPT2, 0x46);*/
}

static void s_start_hdcp1x_auth(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;

	/* prog DDC delay counter*/
	/*sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_HW_OPT2, 0x40);*/

	s_clear_sw_tpi_bit(p_hdcp_obj);
	s_reset_tpi_state_machine(p_hdcp_obj);

	/* Enable HW HDCP state machine*/
	sii_platform_set_bit8(pstDev,
		REG_ADDR__TPI_COPP_DATA2,
		BIT_MSK__TPI_COPP_DATA2__REG_TPI_HDCP_PREP_EN);
	/* Enable interrupt for BKSV read done */
	sii_platform_set_bit8(pstDev, REG_ADDR__TPI_INTR_EN,
		BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B2);

	p_hdcp_obj->auth_state =
		SII_MOD_HDCP_EVENT__WAIT_FOR_START;
	p_hdcp_obj->hdcp_version = SII_DRV_HDCP_LINK_VER__HDCP1X;
	/* timer for checking DDC status */
	if (p_hdcp_obj->isddcfailed == false) {
		sii_timer_start(&p_hdcp_obj->timer_ddc_poll,
			TIMER_START__TX_HDCP__DDC_FAIL);
		p_hdcp_obj->isddcfailed = true;
	}
}

static void s_stop_hdcp2x_auth(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t int_status = 0;
	uint8_t bit_mask_hdcp2x_smng_xfer_start =
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START;
	/* disable h_d_c_p2x intr masks*/
	/* hdcp2x interrupt0 mask register*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0_MASK, 0x0);
	/* hdcp2x interrupt1 mask register*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1_MASK, 0x0);
#ifdef CONFIG_HDCP_REPEATER_EN
	sii_timer_stop(
		&p_hdcp_obj->timer2x_rpt_re_auth);
	sii_timer_stop(
		&p_hdcp_obj->timer_rpt_rcv_id_poll);
	p_hdcp_obj->b_tx_2x_rpt_msg.b_hdcp2_rx_ready = false;
	p_hdcp_obj->b_tx_2x_rpt_msg.ske_event = false;
	p_hdcp_obj->b_tx_2x_rpt_msg.smng_enable = false;
	p_hdcp_obj->b_tx_2x_rpt_msg.b_hdcp2_rx_ready = false;
	p_hdcp_obj->b_tx_2x_rpt_msg.rcv_id_change = false;
	p_hdcp_obj->b_tx_2x_rpt_msg.smng_enable = false;
	p_hdcp_obj->rpt_reauth_done = false;
#endif
#ifdef CONFIG_POWER_DOWN_EN
	sii_timer_stop(&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll);
#endif
	/* clearing all hdcp2x interrupts */
	int_status = sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0, int_status);

	int_status = sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1, int_status);

	/* diasabling HDCP2.2 encryption*/
	sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
		BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_EN |
		BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);

	s_tpi_hdcp2x_protection_enable(p_hdcp_obj, false);
	p_hdcp_obj->is_ds_hdcp22_supported = 0;
	sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
		bit_mask_hdcp2x_smng_xfer_start);
}

static int s_start_hdcp2x_auth(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	int status = 0;
	/* check DDC 0x50 to read the HDCP2.2
	 * capability*/
	//if (p_hdcp_obj->is_ds_hdcp22_supported != 0x04)
    {
		s_set_sw_tpi_bit(p_hdcp_obj);
		s_reset_tpi_state_machine(p_hdcp_obj);
		if (SI_TX_DDC_ERROR_CODE_NO_ERROR
			!= s_read_ds_hdcp22_capability(p_hdcp_obj)) {
			sii_log_debug("DDC failed. try again\n");
			s_reset_tpi_state_machine(p_hdcp_obj);
			/* second attempt to make sure*/
			if (SI_TX_DDC_ERROR_CODE_NO_ERROR !=
				s_read_ds_hdcp22_capability(p_hdcp_obj)) {
				sii_log_debug("Error reading HDCP2.2 Capability\n");
			}
		}
	}

	if (ds_hdcp22_support_get(p_hdcp_obj)) {
		sii_log_info("Downstream is HDCP2.2 capable\n");
		s_reset_tpi_state_machine(p_hdcp_obj);
		sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG1,
			BIT_MSK__SW_RST_REG1__REG_TX_DDCM2_SW_RST
			| BIT_MSK__SW_RST_REG1__REG_TX_DDCM1_SW_RST);
		usleep(20000);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG1,
			BIT_MSK__SW_RST_REG1__REG_TX_DDCM2_SW_RST
			| BIT_MSK__SW_RST_REG1__REG_TX_DDCM1_SW_RST);

		if (p_hdcp_obj->is_auth_requested) {
			s_tpi_hdcp2x_protection_enable(p_hdcp_obj, true);
			hdcp2x_auth_start(p_hdcp_obj);
		} else {
			p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__OFF;
		}
	} else if (p_hdcp_obj->ds_auth_support ==
					SII_DRV_HDCP_LINK_VER__AUTO) {
		sii_log_info("Downstream is HDCP1.4 capable\n");
		s_start_hdcp1x_auth(p_hdcp_obj);
	} else {
		sii_log_info("Downstream is NOT HDCP2.2 capable\n");
		status = 1;
	}

	return status;
}

static void s_hdcp2x_redo(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
#ifdef CONFIG_UNDEFINED_MACRO
	/*bugid - 40229
	 * below s_hdcp2x_redo is not working
	 * for now disable and enable hdcp
	 * todo: remove this later
	 */
	s_start_hdcp(p_hdcp_obj, false);
	s_start_hdcp(p_hdcp_obj, true);
	pstDev = pstDev;
#else

	sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
		BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);
#ifdef CONFIG_POWER_DOWN_EN
	p_hdcp_obj->hdcp2x_power_up_done = false;
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL, 0x32);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL, 0x22);
	msleep(50);
	/* gate off p_clk, mhl1/2/3 //powerdown is enabled.*/
	sii_platform_set_bit8(pstDev, REG_ADDR__PD_REG1,
		BIT_MSK__PD_REG1__REG_HDCP2TX_REAUTH_REQ);
#endif
	msleep(50);
	/*enabling re-authentication --- one pulse*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_1, 0xE1);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_1, 0xE0);

	p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__FAILED;
#endif
}

static void s_stop_hdcp_hw_state_machine(struct hdcp_obj *p_hdcp_obj)
{
	if (p_hdcp_obj->hdcp_version == SII_DRV_HDCP_LINK_VER__HDCP1X) {
		s_stop_hdcp1x_auth(p_hdcp_obj);
		sii_log_debug("HDCP 1x auth is disabled\n");
	} else if (p_hdcp_obj->hdcp_version == SII_DRV_HDCP_LINK_VER__HDCP22) {
		s_stop_hdcp2x_auth(p_hdcp_obj);
		sii_log_debug("HDCP 2x auth is disabled\n");
	}

	sii_platform_set_bit8(p_hdcp_obj->pstDevice,
		REG_ADDR__SW_RST_REG1,
		BIT_MSK__SW_RST_REG1__REG_TX_DDCM2_SW_RST
		| BIT_MSK__SW_RST_REG1__REG_TX_DDCM1_SW_RST);
	usleep(20000);
	sii_platform_clr_bit8(p_hdcp_obj->pstDevice,
		REG_ADDR__SW_RST_REG1,
		BIT_MSK__SW_RST_REG1__REG_TX_DDCM2_SW_RST
		| BIT_MSK__SW_RST_REG1__REG_TX_DDCM1_SW_RST);

	s_reset_ksv_fifo(p_hdcp_obj);
	p_hdcp_obj->auth_fail_counter = 0;
	p_hdcp_obj->hdcp_version = SII_DRV_HDCP_LINK_VER__NONE;
	p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__OFF;
	s_update_hdcp_state(p_hdcp_obj);
	s_print_hdcp_status(p_hdcp_obj);
	s_notify_hdcp_status(p_hdcp_obj);
}

static int s_start_hdcp_hw_state_machine(struct hdcp_obj *p_hdcp_obj,
		enum sii_drv_hdcp_link_version ds_auth_support)
{
	int status = 0;

	if (ds_auth_support == SII_DRV_HDCP_LINK_VER__NONE) {
		sii_log_debug("Version to be supported is None !!!\n");
	} else if (ds_auth_support == SII_DRV_HDCP_LINK_VER__HDCP22) {
		sii_log_debug("Version to be supported is HDCP 2.2 !!!\n");
		status = s_start_hdcp2x_auth(p_hdcp_obj);
	} else if (ds_auth_support == SII_DRV_HDCP_LINK_VER__HDCP1X) {
		sii_log_debug("Version to be supported is HDCP 1.4 !!!\n");
		s_start_hdcp1x_auth(p_hdcp_obj);
	} else if (ds_auth_support == SII_DRV_HDCP_LINK_VER__AUTO) {
		sii_log_debug("Version to be supported is AUTO !!!\n");
		status = s_start_hdcp2x_auth(p_hdcp_obj);
	}

	return status;
}

static int s_start_hdcp(struct hdcp_obj *p_hdcp_obj, bool b_enable)
{
	int status = 0;

	if (b_enable) {
		if (p_hdcp_obj->is_auth_requested == false) {
			p_hdcp_obj->is_auth_requested = b_enable;
			status = s_start_hdcp_hw_state_machine(p_hdcp_obj,
					p_hdcp_obj->ds_auth_support);
		}
	} else {
		if (p_hdcp_obj->is_auth_requested == true) {
			p_hdcp_obj->is_auth_requested = b_enable;
			s_stop_hdcp_hw_state_machine(p_hdcp_obj);
		}
		sii_timer_stop(&p_hdcp_obj->timer1x_re_auth);
	}

	return status;
}

static void s_tpi_hdcp_protection_enable(struct hdcp_obj *p_hdcp_obj,
						bool is_enabled)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_platform_put_bit8(pstDev, REG_ADDR__TPI_COPP_DATA2, HDCP_CTRL_MODE,
		is_enabled ? HDCP_CTRL_MODE : 0x00);
}

static uint8_t s_tpi_hdcp_status_get(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t hdcp_status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__TPI_COPP_DATA1);

	return hdcp_status;
}

static bool s_tpi_is_downstream_hdcp_available(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t hdcp_status = sii_platform_rd_reg8(pstDev,
					REG_ADDR__TPI_COPP_DATA1);

	 return ((hdcp_status &
			BIT_MSK__TPI_COPP_DATA1__REG_COPP_PROTYPE) != 0) ?
			true : false;
}

static bool s_tpi_hdcp_is_part1_done(struct hdcp_obj *p_hdcp_obj)
{
	uint8_t hdcp_status = s_tpi_hdcp_status_get(p_hdcp_obj);
	bool ret_val = (PART1_DONE == (hdcp_status & PART1_DONE)) ?
		true : false;

	sii_log_debug("is part1 done: %s\n", ret_val ? "YES" : "NO");
	return ret_val;
}

static bool s_tpi_hdcp_is_part2_done(struct hdcp_obj *p_hdcp_obj)
{
	uint8_t hdcp_status = s_tpi_hdcp_status_get(p_hdcp_obj);
	bool ret_val = (bool)(PART2_DONE == (hdcp_status & PART2_DONE));

	sii_log_debug("is part2 done: %s\n", ret_val ? "YES" : "NO");
	return ret_val;
}

static void s_tpi_bksv_get(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_platform_block_read8(pstDev, REG_ADDR__TPI_WR_BKSV_1,
		p_hdcp_obj->ksv_list.p_list, KSV_NUM_BYTES);

	p_hdcp_obj->ksv_list.p_list += KSV_NUM_BYTES;
	p_hdcp_obj->ksv_list.length += KSV_NUM_BYTES;
}

static bool s_tpi_ksv_list_portion_size_get(struct hdcp_obj *p_hdcp_obj,
						uint8_t *p_bytes_to_read)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	bool is_done;
	uint8_t fifo_status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__TPI_KSV_FIFO_STAT);

	*p_bytes_to_read = fifo_status
		& BIT_MSK__TPI_KSV_FIFO_STAT__KSV_FIFO_BYTES;
	is_done = (bool)((fifo_status & BIT_MSK__TPI_KSV_FIFO_STAT__KSV_FIFO_LAST) != 0);

	return is_done;
}

static void s_tpi_ksv_list_get(struct hdcp_obj *p_hdcp_obj, uint8_t *p_buffer,
				uint8_t length)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	/* note: this FIFO register is a special case. while reading from it
	 * in burst mode the slave I2C interface don't increment the offset
	 * after every single reading. content of the register gets immediately
	 * updated by data waiting in the FIFO after every reading.
	 */
	sii_platform_block_read8(pstDev, REG_ADDR__TPI_KSV_FIFO_FORW, p_buffer,
		length);
}

static enum sii_drv_tx_hdcp_internal_state s_tpi_get_ksv_list(
	struct hdcp_obj *p_hdcp_obj, uint8_t *p_bstatus)
{
	uint8_t bytes_to_read;
	uint8_t ds_count = p_bstatus[0]
		& BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT;
	uint8_t ds_depth = p_bstatus[1] & BIT_MSK__TPI_BSTATUS2__REG_DS_DEPTH;
	int16_t fifo_byte_counter;
	bool is_done;

	if ((ds_count >= 0x7F) || (ds_depth >= 0x07)) {
		sii_log_debug(
			"Error::DS device count: %02X, DS device depth: %02X\n",
			ds_count, ds_depth);
		p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__OTHER;
	}

	p_hdcp_obj->ksv_list.p_list =
		(p_hdcp_obj->ksv_list.p_list_start + KSV_NUM_BYTES);
	p_hdcp_obj->ksv_list.length = KSV_NUM_BYTES;

	fifo_byte_counter = (ds_count * KSV_NUM_BYTES);

	is_done = s_tpi_ksv_list_portion_size_get(p_hdcp_obj, &bytes_to_read);

	while ((fifo_byte_counter > 0) && (bytes_to_read != 0)) {
		/* get DS BKSV list*/
		s_tpi_ksv_list_get(p_hdcp_obj, p_hdcp_obj->ksv_list.p_list,
			bytes_to_read);
		p_hdcp_obj->ksv_list.p_list += bytes_to_read;
		fifo_byte_counter -= bytes_to_read;
		/* no of bytes left in fifo*/
		is_done = s_tpi_ksv_list_portion_size_get(p_hdcp_obj,
			&bytes_to_read);
	}
	p_hdcp_obj->ksv_list.p_list = p_hdcp_obj->ksv_list.p_list_start;
	p_hdcp_obj->ksv_list.length += (ds_count * KSV_NUM_BYTES);

    UNUSED(is_done);
	return SII_MOD_HDCP_EVENT__AUTHENTICATED;
}

static void s_tpi_b_status_get(struct hdcp_obj *p_hdcp_obj,
				uint8_t *p_ds_b_status)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_platform_block_read8(pstDev, REG_ADDR__TPI_BSTATUS1,
		p_ds_b_status, 2);
}

static void s_print_hdcp_status(struct hdcp_obj *p_hdcp_obj)
{
	if (p_hdcp_obj->prev_auth_state != p_hdcp_obj->auth_state) {
		switch (p_hdcp_obj->auth_state) {
		case SII_MOD_HDCP_EVENT__OFF:
			sii_log_info("DS HDCP: OFF\n");
			break;
		case SII_MOD_HDCP_EVENT__WAIT_FOR_START:
			sii_log_debug("DS HDCP: WAIT FOR START\n");
			break;
		case SII_MOD_HDCP_EVENT__WAIT_FOR_DONE:
			sii_log_debug("DS HDCP: WAIT FOR DONE\n");
			break;
		case SII_MOD_HDCP_EVENT__WAIT_FIFO_READY:
			sii_log_debug("DS HDCP: WAIT FOR FIFO READY\n");
			break;
		case SII_MOD_HDCP_EVENT__AUTHENTICATED:
			sii_log_info("DS HDCP: AUTHENTICATED\n");
			break;
		case SII_MOD_HDCP_EVENT__FAILED:
			sii_log_err("DS HDCP: FAILED\n");
			break;
		case SII_MOD_HDCP_EVENT__V_CHECK:
			sii_log_debug("DS HDCP: V-CHECK\n");
			break;
		default:
			break;
		}
		p_hdcp_obj->prev_auth_state = p_hdcp_obj->auth_state;
	}
	if (p_hdcp_obj->prev_error_state != p_hdcp_obj->error_state) {
		if (p_hdcp_obj->error_state) {
			sii_log_debug("HDCP errors:\n");
			if (p_hdcp_obj->error_state == SII_DRV_HDCP_FAILURE__NACK)
				sii_log_debug("No ACK at downstream HDCP registers\n");
			if (p_hdcp_obj->error_state == SII_DRV_HDCP_FAILURE__DEV_EXC)
				sii_log_debug("Device limit exceeded\n");
			if (p_hdcp_obj->error_state == SII_DRV_HDCP_FAILURE__CAS_EXC)
				sii_log_debug("Cascaded limit exceeded\n");
			if (p_hdcp_obj->error_state == SII_DRV_HDCP_FAILURE__V)
				sii_log_debug("Downstream V' != V\n");
			if (p_hdcp_obj->error_state == SII_DRV_HDCP_FAILURE__TIMEOUT)
				sii_log_debug("FIFO READY was not set\n");
			if (p_hdcp_obj->error_state == SII_DRV_HDCP_FAILURE__OTHER)
				sii_log_debug("Other errors\n");
		} else {
			sii_log_debug("No HDCP Errors\n");
		}

		p_hdcp_obj->prev_error_state = p_hdcp_obj->error_state;
	}
}

static void s_print_hdcp_query_status(struct hdcp_obj *p_hdcp_obj,
					uint8_t query)
{
	sii_log_debug("HDCP Authentication status:\n");
	sii_log_print("Link: ");

	switch (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_LINK_STATUS) {
	case BIT_ENUM__TPI_HDCP_QUERY__STATUS_NORMAL:
		sii_log_print("Normal\n");
		break;
	case BIT_ENUM__TPI_HDCP_QUERY__STATUS_LOST:
		sii_log_print("Lost\n");
		break;
	case BIT_ENUM__TPI_HDCP_QUERY__STATUS_FAILED:
		sii_log_print("Failed\n");
		break;
	default:
		break;
	}

	sii_log_print("DS HDCP: ");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_PROTYPE)
		sii_log_print("Available\n");
	else
		sii_log_print("Unavailable\n");

	sii_log_print("Repeater: ");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP)
		sii_log_print("Yes\n");
	else
		sii_log_print("No\n");

	sii_log_print("Connected Sink Protection: ");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT)
		sii_log_print("Yes\n");
	else
		sii_log_print("No\n");

	sii_log_print("Connected Repeater Protection: ");
	if (query & BIT_MSK__TPI_COPP_DATA1__REG_COPP_GPROT)
		sii_log_print("Yes\n");
	else
		sii_log_print("No\n");
}

static bool s_hdcp_state_machine_handler(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj;
    pstSii9630_Device_t pstDev;// = p_hdcp_obj->pstDevice;

	p_hdcp_obj = (struct hdcp_obj *)p_obj;

	if (!p_hdcp_obj)
		return FALSE;

	if (!(p_hdcp_obj->tx_hpd_status))
		return FALSE;

	 pstDev = p_hdcp_obj->pstDevice;
	switch (p_hdcp_obj->auth_state) {
	case SII_MOD_HDCP_EVENT__OFF:
	case SII_MOD_HDCP_EVENT__WAIT_FOR_START:
		/* DDC ACK check*/
		if (s_tpi_is_downstream_hdcp_available(p_hdcp_obj)) {
			sii_log_debug("Downstream Supports HDCP1.4\n");

			sii_timer_stop(
				&p_hdcp_obj->timer_ddc_poll);
			/* enable masks for hdcp1x interrupts*/
			sii_platform_set_bit8(pstDev,
				REG_ADDR__TPI_INTR_EN,
				BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B3 |
				BIT_MSK__TPI_INTR_EN__REG_TPI_INTR_MASK_B7);

			if (p_hdcp_obj->is_auth_requested) {
				/* just in case*/
				sii_platform_set_bit8(pstDev,
				  REG_ADDR__TPI_COPP_DATA2,
				  BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN);
				s_tpi_hdcp_protection_enable(p_hdcp_obj, false);
				/* start authentication	*/
				s_tpi_hdcp_protection_enable(p_hdcp_obj, true);

				sii_platform_clr_bit8(pstDev,
				  REG_ADDR__TPI_COPP_DATA2,
				  BIT_MSK__TPI_COPP_DATA2__REG_CANCEL_PROT_EN);

				p_hdcp_obj->error_state =
					SII_DRV_HDCP_FAILURE__NONE;
				p_hdcp_obj->auth_state =
					SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
			} else {
				p_hdcp_obj->auth_state =
					SII_MOD_HDCP_EVENT__OFF;
			}
		} else {
			p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__NACK;
			if (p_hdcp_obj->isddcfailed == false) {
				sii_timer_start(
					&p_hdcp_obj->timer_ddc_poll,
					TIMER_START__TX_HDCP__DDC_FAIL);
				p_hdcp_obj->isddcfailed = true;
			}
		}
		break;
	case SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE:
		if (s_tpi_hdcp_is_part2_done(p_hdcp_obj)) {
			p_hdcp_obj->auth_state =
				SII_MOD_HDCP_EVENT__AUTHENTICATED;
			s_tpi_hdcp_encryption_enable(
				p_hdcp_obj, true);
#if (CONFIG_SOURCE_ONLY == 0)
					s_notify_hdcp_status_to_upstream(
					p_hdcp_obj,
					SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED);
#endif // (CONFIG_SOURCE_ONLY == 0)
		}
		break;
	case SII_MOD_HDCP_EVENT__AUTHENTICATED:
		break;
	case SII_MOD_HDCP_EVENT__FAILED:
		if (!ds_hdcp22_support_get(p_hdcp_obj)) {
			s_start_hdcp(p_hdcp_obj, false);
			sii_timer_start(&p_hdcp_obj->timer1x_re_auth,
				TIMER_START__TX_HDCP__REAUTH_WAIT);
		}
		break;
	default:
		break;
	}
	s_update_hdcp_state(p_hdcp_obj);
	s_print_hdcp_status(p_hdcp_obj);
	s_notify_hdcp_status(p_hdcp_obj);
	return TRUE;
}

#ifdef CONFIG_UNDEFINED_MACRO
static void s_print_rcvd_list(struct hdcp_obj *p_hdcp_obj)
{
	uint8_t i = 0;
	uint8_t j = 0;

	for (j = 0; j < p_hdcp_obj->hdcp2x_rpt_rcv_id_list.devcnt; j++) {

		sii_log_debug(" ");
		for (i = 0; i < RCVRID_LENGTH; i++) {
			sii_log_debug("%x ",
			p_hdcp_obj->hdcp2x_rpt_rcv_id_list.rpt_rcvid[j].id[i]));
		}

		sii_log_debug("\n");
	}
}
#endif
/* check for DDC available */
static void s_hdcp_ddc_ack_poll(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;

	if (!p_hdcp_obj)
		return;

	if (1) {

		if (s_tpi_is_downstream_hdcp_available(p_hdcp_obj)) {
			s_hdcp_state_machine_handler(p_hdcp_obj);
		} else {
			sii_log_print("\n Downstream DDC failed\n");
			s_start_hdcp(p_hdcp_obj, false);
			/* restart HDCP after 500 ms*/
			sii_timer_start(&p_hdcp_obj->timer1x_re_auth,
				TIMER_START__TX_HDCP__REAUTH_WAIT);
			p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__NACK;
		}
		p_hdcp_obj->isddcfailed = false;

		//up(&p_hdcp_obj->drv_context->isr_lock);
	}
}

static void s_hdcp_restart_1xauth(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;

	if (!p_hdcp_obj)
		return;

	if (1) {

		if (p_hdcp_obj->tx_hpd_status) {
			if (p_hdcp_obj->auth_state == SII_MOD_HDCP_EVENT__OFF)
				s_start_hdcp(p_hdcp_obj, true);
		}

		//up(&p_hdcp_obj->drv_context->isr_lock);
	}
}

#ifdef CONFIG_POWER_DOWN_EN
static void s_hdcp2x_pwr_up_monitor(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;

	if (!p_hdcp_obj)
		return;

	if (1) {

		if (p_hdcp_obj->tx_hpd_status) {
			if (ds_hdcp22_support_get(p_hdcp_obj)) {
				if (p_hdcp_obj->hdcp2x_power_up_done) {
					hdcp2x_pdn_mode_auth_start(p_hdcp_obj);
					p_hdcp_obj->hdcp2x_power_up_done =
						false;
					sii_log_debug("Power up done. Start HDCP 2x!\n");
				}
			}
		}
		//up(&p_hdcp_obj->drv_context->isr_lock);
	}
}
#endif

static void s_process_hdcp_ds_repeater(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t a_ds_b_status[2];
	uint8_t ds_bcaps = 0x00;
	uint8_t dev_exceed = 0, casc_exceed = 0;
	/* defined to abide 80 character rule */
	uint8_t bit_msk_bstatus2_ds_casc_exceed =
		BIT_MSK__TPI_BSTATUS2__REG_DS_CASC_EXCEED;
	uint8_t bit_msk_bstatus1_ds_dev_exceed =
		BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_EXCEED;
	uint8_t bit_msk_bstatus1_dev_cnt =
		BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_CNT;
	uint8_t bit_msk_tpi_intr_st3 =
		BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST3;
	enum sii_drv_tx_hdcp_internal_state fifo_rd_done =
		SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE;

	ds_bcaps = sii_platform_rd_reg8(pstDev, REG_ADDR__TPI_DS_BCAPS);

	if (ds_bcaps & HDCP1X_KSV_FIFO_READY) {
		s_tpi_b_status_get(p_hdcp_obj, a_ds_b_status);

		sii_log_debug("bstatus: 0x%x 0x%x\n",
			a_ds_b_status[0], a_ds_b_status[1]);

		dev_exceed = a_ds_b_status[0] &
			bit_msk_bstatus1_ds_dev_exceed;
		casc_exceed = a_ds_b_status[1] &
			bit_msk_bstatus2_ds_casc_exceed;
		if (dev_exceed || casc_exceed) {
			if (dev_exceed) {
				sii_log_debug("Max DS Devices");
				sii_log_print(" Exceeded...\n");
			}
			if (casc_exceed) {
				sii_log_debug("Max Cascaded");
				sii_log_print(" Devices");
				sii_log_print(" Exceeded...\n");
			}
			p_hdcp_obj->auth_state =
				SII_MOD_HDCP_EVENT__FAILED;
		} else if ((a_ds_b_status[0]
				& bit_msk_bstatus1_dev_cnt)) {
			if (p_hdcp_obj->hdcp1x_int_stat.reg1
					& bit_msk_tpi_intr_st3) {
				/* read KSV fifo*/
				/*s_reset_ksv_fifo(p_hdcp_obj);*/
				p_hdcp_obj->auth_state =
					s_tpi_get_ksv_list(p_hdcp_obj,
						a_ds_b_status);
				sii_log_debug("Repeater KSV FIFO: ");
				s_dump_buffer(p_hdcp_obj->ksv_list.p_list_start + KSV_NUM_BYTES,
					p_hdcp_obj->ksv_list.length - KSV_NUM_BYTES);
/* #if (CONFIG_SOURCE_ONLY == 0)
*					s_notify_hdcp_status_to_upstream(
*					p_hdcp_obj,
*					SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED);
*#endif // (CONFIG_SOURCE_ONLY == 0)
*/
				if (s_tpi_hdcp_is_part1_done(
					p_hdcp_obj)) {
					/* part1 authentication
						* is done*/
					p_hdcp_obj->auth_state = fifo_rd_done;
				}
			}
		} else if ((a_ds_b_status[0] & bit_msk_bstatus1_dev_cnt) == 0) {
#if (CONFIG_SOURCE_ONLY == 0)
                    p_hdcp_obj->hdcp_topology.device_count = 0;
					s_notify_hdcp_status_to_upstream(
					p_hdcp_obj,
					SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED);
#endif // (CONFIG_SOURCE_ONLY == 0)
				if (s_tpi_hdcp_is_part1_done(
					p_hdcp_obj)) {
					/* part1 authentication
						* is done*/
					p_hdcp_obj->auth_state = fifo_rd_done;
				}
		} else if (a_ds_b_status[1]) {
			p_hdcp_obj->auth_state =
				SII_MOD_HDCP_EVENT__FAILED;
			/*s_update_hdcp_state(p_hdcp_obj);
			s_print_hdcp_status(p_hdcp_obj);
			s_notify_hdcp_status(p_hdcp_obj);*/
		}
	}
}

static void s_reset_tpi_state_machine(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_log_debug("Reset TPI state machine(hdcp)\n");
	sii_platform_set_bit8(pstDev, REG_ADDR__TPI_HW_OPT0,
		BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_HW_OPT0,
		BIT_MSK__TPI_HW_OPT0__REG_HW_TPI_SM_RST);

	sii_platform_set_bit8(pstDev, REG_ADDR__SCDC_CTL, 0x80);
	sii_platform_clr_bit8(pstDev, REG_ADDR__SCDC_CTL, 0x80);
}

static bool s_hdcp1x_intr_handler(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	/* defined to abide 80 character rule */
	uint8_t bit_msk_bstatus2_ds_casc_exceed =
		BIT_MSK__TPI_BSTATUS2__REG_DS_CASC_EXCEED;
	uint8_t bit_msk_bstatus1_ds_dev_exceed =
		BIT_MSK__TPI_BSTATUS1__REG_DS_DEV_EXCEED;
	uint8_t bit_msk_tpi_copp_data1_lprot =
		BIT_MSK__TPI_COPP_DATA1__REG_COPP_LPROT;
	uint8_t bit_msk_tpi_copp_data1_hdcdp_rep =
		BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP;
	uint8_t hdcp_authed = SII_MOD_HDCP_EVENT__AUTHENTICATED;

	if (p_hdcp_obj->hdcp1x_int_stat.reg0
		& BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST2) {
		sii_log_debug("BKSV read done\n");

	}
	if (p_hdcp_obj->hdcp1x_int_stat.reg0
		& BIT_MSK__TPI_INTR_ST0__TPI_INTR_ST7) {
		uint8_t hdcp_status;

		hdcp_status = s_tpi_hdcp_status_get(p_hdcp_obj);
		s_print_hdcp_query_status(p_hdcp_obj, hdcp_status);
		switch (hdcp_status
			& BIT_MSK__TPI_COPP_DATA1__REG_COPP_LINK_STATUS) {
		case BIT_ENUM__TPI_HDCP_QUERY__STATUS_NORMAL:
			if (p_hdcp_obj->auth_state
				== SII_MOD_HDCP_EVENT__WAIT_FOR_DONE) {
        		s_reset_ksv_fifo(p_hdcp_obj);
        		s_tpi_bksv_get(p_hdcp_obj);
        		sii_log_debug("Sink BKSV: ");
        		s_dump_buffer(p_hdcp_obj->ksv_list.p_list_start,
        			p_hdcp_obj->ksv_list.length);

				if (hdcp_status &
					bit_msk_tpi_copp_data1_hdcdp_rep) {
					p_hdcp_obj->auth_state =
					SII_MOD_HDCP_EVENT__WAIT_FIFO_READY;
				} else {
					if (!(hdcp_status &
					bit_msk_tpi_copp_data1_lprot)) {
						p_hdcp_obj->auth_state =
						SII_MOD_HDCP_EVENT__FAILED;
					} else {
						/* part 1 is done and
						 * no downstream part 2 needs
						 * to be done*/
						p_hdcp_obj->auth_state =
							(enum sii_drv_tx_hdcp_internal_state) hdcp_authed;
						s_tpi_hdcp_encryption_enable(
							p_hdcp_obj, true);
						/*Currently BKSV_CHANGED event serves the purpose here
						 *May be we need to add/modify this event if requir*/
#if (CONFIG_SOURCE_ONLY == 0)
							s_notify_hdcp_status_to_upstream(p_hdcp_obj,
								SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED);
#endif // (CONFIG_SOURCE_ONLY == 0)
					}
				}
			}
			break;

		case BIT_ENUM__TPI_HDCP_QUERY__STATUS_FAILED:
		case BIT_ENUM__TPI_HDCP_QUERY__STATUS_LOST:
			if (hdcp_status
				& BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP) {
				uint8_t a_ds_b_status[2];

				s_tpi_b_status_get(p_hdcp_obj, a_ds_b_status);
				/* note, in case of R0 failure (for example)
				 * the BSTATUS shadow registers*/
				/* are not read, but since the registers value
				 *  is automatically cleared*/
				/* on clearing 0x2A.0, the EXCEED bits are not
				 *  set. by checking the EXCEED
				 bits,*/
				/* it is possible to distinguish the DEV_EXCCED
				 *  and CASCADE_EXCEED*/
				/* failures from any other failure.*/

				if (a_ds_b_status[0] &
					bit_msk_bstatus1_ds_dev_exceed) {
					sii_log_debug("Device Limit exceed\n");
					p_hdcp_obj->error_state =
						SII_DRV_HDCP_FAILURE__DEV_EXC;

#if (CONFIG_SOURCE_ONLY == 0)
						s_notify_hdcp_status_to_upstream(p_hdcp_obj,
							SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED); //RG using this flag to inform errors...
#endif // (CONFIG_SOURCE_ONLY == 0)
				}
				if (a_ds_b_status[1] &
					bit_msk_bstatus2_ds_casc_exceed) {
					sii_log_debug("Cascaded Limit exceed\n");
					p_hdcp_obj->error_state =
						SII_DRV_HDCP_FAILURE__CAS_EXC;

#if (CONFIG_SOURCE_ONLY == 0)
						s_notify_hdcp_status_to_upstream(p_hdcp_obj,
							SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED); //RG using this flag to inform errors...
#endif // (CONFIG_SOURCE_ONLY == 0)
				}
			}

			if ((p_hdcp_obj->auth_state
				== SII_MOD_HDCP_EVENT__WAIT_FIFO_READ_DONE)
				|| (p_hdcp_obj->auth_state
					== SII_MOD_HDCP_EVENT__AUTHENTICATED)) {
				/* error happened during the KSV list reading,
				* need to restart the DDC access state machine.
				* most probably the error is caused by V
				* mismatch.*/
				sii_log_debug("Error in ksv list reading");
				sii_log_print(" -- DDC reset\n");

				p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__V;
				/*sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
					BIT_ENUM__DDC_CMD__CLEAR_FIFO);*/

			} else if (SII_MOD_HDCP_EVENT__WAIT_FIFO_READY
				== p_hdcp_obj->auth_state) {
				/* error happened while waiting for KSV
				 * FIFO READY bit*/
				sii_log_debug("Error in waiting");
				sii_log_print(" ksv fifo ready bit\n");
				p_hdcp_obj->error_state =
					SII_DRV_HDCP_FAILURE__TIMEOUT;
			}
			p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__FAILED;
			/*s_notify_hdcp_status(p_hdcp_obj);*/
			break;
		}
		/*s_update_hdcp_state(p_hdcp_obj);
		s_print_hdcp_status(p_hdcp_obj);
		if (p_hdcp_obj->auth_state
			== SII_MOD_HDCP_EVENT__AUTHENTICATED) {
			s_notify_hdcp_status(p_hdcp_obj);
		}*/
	}

	if (s_tpi_hdcp_status_get(p_hdcp_obj) &
		BIT_MSK__TPI_COPP_DATA1__REG_COPP_HDCP_REP) {
		if (p_hdcp_obj->auth_state
			== SII_MOD_HDCP_EVENT__WAIT_FIFO_READY) {
			if (sii_platform_rd_reg8(pstDev, REG_ADDR__TPI_DS_BCAPS)
				& 0x40) {
				/* after receiving first byte of ksv*/
				s_process_hdcp_ds_repeater(p_hdcp_obj);
			}
		}
	}

	/* clear all pending HDCP interrupts*/
	p_hdcp_obj->hdcp1x_int_stat.reg0 = 0x00;
	p_hdcp_obj->hdcp1x_int_stat.reg1 = 0x00;
	return TRUE;
}

static bool s_wait_for_ddc_bus(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t val = 0;
	uint8_t time_out = LEN_TPI__DDC_FIFO_SIZE + 1;
	bool ret_val = false;
	/*to avoid checkpatch warning : else is not useful after return */

	/* time_out is time in ms, which is proportional to the FIFO size.*/
	/* since the time required to transmit one byte is 100 u_s,
	 * the time_out*/
	/* is as much as 10 times longer. + 1 is additional time due
	 * to s_i__sleep()
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

static enum ddc_com_err s_read_ds_hdcp22_capability(
	struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	enum ddc_com_err ds_ddc_error = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint16_t fifo_size;
	uint16_t time_out_ms;
	bool loop = false;
	uint8_t segment_index = 0xFF;
	uint8_t reg_addr = 0x50;
	uint8_t *p_buf = &p_hdcp_obj->is_ds_hdcp22_supported;
	uint16_t length = 1;
	bool flag = true;
	/* to avoid checkpatch warning : else is not useful after break*/
	do {
		if (length == 0)
			break;

		if (!p_buf)
			break;

		if (!s_wait_for_ddc_bus(p_hdcp_obj)) {
			ds_ddc_error = SI_TX_DDC_ERROR_CODE_BUSY;
			sii_log_debug("DDC busy try later\n");
			sii_drv_tx_ddc_reset(&g_stdrv_obj, true);
			return ds_ddc_error;
		}
		sii_platform_set_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
			BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_ADDR,
			(0xFF != segment_index) ? 0xA0 : 0x74);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_SEGM,
			(0xFF != segment_index) ? segment_index : 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_OFFSET, reg_addr);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT2, 0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT1,
			(uint8_t)length);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			BIT_ENUM__DDC_CMD__CLEAR_FIFO);
		/* sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			segment_index ?
			BIT_ENUM__DDC_CMD__ENHANCED_DDC_READ :
			BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);
		 */
		sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
			BIT_ENUM__DDC_CMD__SEQUENTIAL_READ);

		/* timeout is proportional to length*/
		time_out_ms = length + 3;

		/* wait until the FIFO is filled with several bytes*/
		/* also makes time aligning*/
		usleep(2000);

		do {
			fifo_size =
				sii_platform_rd_reg8(pstDev,
					REG_ADDR__DDC_DOUT_CNT) &
					BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT;

			if (fifo_size) {
				/* if the FIFO has some bytes*/
				if (fifo_size > length) {
					ds_ddc_error =
						SI_TX_DDC_ERROR_CODE_TX_HW;
					flag = false;
				} else if (fifo_size > LEN_TPI__DDC_FIFO_SIZE) {
					ds_ddc_error =
						SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
					flag = false;
				} else {
					/* read fifo_size bytes*/
					sii_platform_fifo_read8(pstDev,
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
			ds_ddc_error = SI_TX_DDC_ERROR_CODE_TIMEOUT;
			sii_log_debug("DDC timeout\n");
		}
	} while (loop);

	if (ds_ddc_error) {
		sii_log_debug("DDC Error: %d\n", ds_ddc_error);
		sii_drv_tx_ddc_reset(&g_stdrv_obj, true);
	}

	/* disable DDC master*/
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
		BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
	//sii_platform_clr_bit8(pstDev, REG_ADDR__DDC_CMD, 0x7F);

	return ds_ddc_error;
}

static bool ds_hdcp22_support_get(struct hdcp_obj *p_hdcp_obj)
{
	return (p_hdcp_obj->is_ds_hdcp22_supported == 0x04) ? true : false;
}

static void s_tpi_hdcp_encryption_enable(struct hdcp_obj *p_hdcp_obj,
						bool is_enable)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	sii_platform_put_bit8(pstDev, REG_ADDR__TPI_COPP_DATA2,
		BIT_MSK__TPI_COPP_DATA2__REG_INTR_ENCRYPTION,
		is_enable ? 0x00 : 0xFF);

}

static void s_notify_hdcp_status(struct hdcp_obj *p_hdcp_obj)
{
	if (p_hdcp_obj->evnt_notify_fn)
		p_hdcp_obj->evnt_notify_fn(&g_stdrv_obj,
			SII_DRV_HDCP_EVENT__HDCP_STATUS_CHNG);
}

#ifdef CONFIG_TX_HDCP2X_EN
static void s_notify_hdcp_status_to_upstream(struct hdcp_obj *p_hdcp_obj,
						uint32_t event_flags)
{
	if (p_hdcp_obj->evnt_notify_fn)
		p_hdcp_obj->evnt_notify_fn(&g_stdrv_obj,
			event_flags);
}
static bool is_d_sdevice_hdcp2_repeater(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t val;
	bool ret_val;

	val = sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_GEN_STATUS);
	ret_val =
		(val & BIT_MSK__TX_HDCP2X_GEN_STATUS__RO_HDCP2_REPEATER) ?
			true : false;

	return ret_val;
}

static bool s_hdcp2x_intr_handler(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	struct sii_hdcp_int_stat int_stat = {0};

	int_stat.reg0 = sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0);
	/* check for rcvid change intr*/
	int_stat.reg1 = sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1);

	int_stat.reg2 = sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR2);

	if (memcmp(&p_hdcp_obj->hdcp2x_int_stat, &int_stat,
		sizeof(struct sii_hdcp_int_stat))) {
		uint8_t intreg0 = 0;
		uint8_t intreg1 = 0;
		uint8_t intreg2 = 0;

		/* find any interrupt status bit that changed to '1'*/
		intreg0 |= ((p_hdcp_obj->hdcp2x_int_stat.reg0 ^ int_stat.reg0)
			& int_stat.reg0);
		intreg1 |= ((p_hdcp_obj->hdcp2x_int_stat.reg1 ^ int_stat.reg1)
			& int_stat.reg1);
		intreg2 |= ((p_hdcp_obj->hdcp2x_int_stat.reg2 ^ int_stat.reg2)
			& int_stat.reg2);

		p_hdcp_obj->hdcp2x_int_stat = int_stat;
		if (intreg0 | intreg1 | intreg2) {

			/* call derived interrupt handler*/
#ifdef CONFIG_HDCP_REPEATER_EN
			s_hdcp2x_rpt_intr_handler(p_hdcp_obj);
#else
			s_hdcp2x_non_rpt_intr_handler(p_hdcp_obj);
#endif
			/* clear the interrupts*/
			sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0,
				p_hdcp_obj->hdcp2x_int_stat.reg0);
			sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1,
				p_hdcp_obj->hdcp2x_int_stat.reg1);
			sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR2,
				p_hdcp_obj->hdcp2x_int_stat.reg2);
			p_hdcp_obj->hdcp2x_int_stat.reg0 = 0;
			p_hdcp_obj->hdcp2x_int_stat.reg1 = 0;
			p_hdcp_obj->hdcp2x_int_stat.reg2 = 0;
		}
	}
	return TRUE;
}

#if 0
static void s_hdcp2_content_type_set(
	struct hdcp_obj *p_hdcp_obj,
	enum sii_drv_hdcp_content_type p_content_type)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t timeout;
    uint8_t cnt_retry_xfer = 2;

	/* defined to abide 80 character rule */
	uint8_t bit_msk_rpt_smng_xfer_start =
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START;
	uint8_t bit_msk_rpt_smng_wr_start =
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR_START;
	uint8_t bit_msk_rpt_smng_xfer_done =
		BIT_MSK__TX_HDCP2X_TX_STATUS__RO_HDCP2TX_RPT_SMNG_XFER_DONE;

	if (SII_DRV_HDCP_LINK_VER__HDCP22 == s_ds_hdcp_ver_get(p_hdcp_obj)) {
		uint8_t seq_num_m0 = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_0);
		uint8_t seq_num_m1 = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_1);
		uint8_t seq_num_m2 = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_2);

		if (seq_num_m0 == 255) {
			if (seq_num_m1 == 255)
				seq_num_m2++;
			else
				seq_num_m1++;
		} else
			seq_num_m0++;

		/* smng_xfer_start=0-just to make sure*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			bit_msk_rpt_smng_xfer_start);
		/* seq_num_m0*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_0,
			seq_num_m0);
		/* seq_num_m1*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_1,
			seq_num_m1);
		/* seq_num_m2*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_2,
			seq_num_m2);
		/* k -> always 0x01 -- REG_ADDR_HDCP2X_RPT_SMNG_K*/

		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RPT_SMNG_K,
			0x01);
		/* smng_wr_start=1*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			bit_msk_rpt_smng_wr_start);
		/* smng_in (stream ID)*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_RPT_SMNG_IN,
			0x00);
		/* smng_wr=1*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);
		/* smng_wr=0*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);
		/* smng_in (type)*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_RPT_SMNG_IN,
			p_content_type);
		/* smng_wr=1*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);
		/* smng_wr=0*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);
		/* smng_xfer_start=1-rising edge will start transfer*/

        while( cnt_retry_xfer-- > 0 )
        {
    		sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
    			bit_msk_rpt_smng_xfer_start);

    		usleep(5000);
    		timeout = HDCP2X_CONTENT_TYPE_SET_MAX_TIMEOUT;
    		/* wait smng_xfer_done*/
    		while (--timeout
    			&& !(sii_platform_rd_reg8(pstDev,
    				REG_ADDR__TX_HDCP2X_TX_STATUS)
    				& bit_msk_rpt_smng_xfer_done)) {
    			usleep(1000);
    		}
    		if (!timeout) {
    			sii_log_err("Unable to set contentType\n");
    			sii_log_err("Stream Manage Transfer failed\n");
        		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
        			bit_msk_rpt_smng_xfer_start);

    		} else {
    			sii_log_info("ContentType set to %s\n",
    				p_content_type ?
    					"SII_DRV_HDCP_CONTENT_TYPE__1" :
    					"SII_DRV_HDCP_CONTENT_TYPE__0");
                break;
    		}
        }

		/* smng_xfer_start=0*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			bit_msk_rpt_smng_xfer_start);
		/* smng_wr_start=0*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			bit_msk_rpt_smng_xfer_start);
		p_hdcp_obj->strmng_2x_msg.content_type = p_content_type;
	} else {
		sii_log_err("Unable to set contentType\n");
		sii_log_debug("DS Device is not HDCP2.2 Capable\n");
	}

}
#else
static void s_hdcp2_content_type_set(
	struct hdcp_obj *p_hdcp_obj,
	enum sii_drv_hdcp_content_type p_content_type)
{
    p_hdcp_obj->strmng_2x_msg.content_type = (uint8_t) p_content_type;
    s_hdcp2x_fill_strmng_2x_msg(p_hdcp_obj,
        (struct sii_drv_tx_strmng_2x_msg *)
        &p_hdcp_obj->strmng_2x_msg);
}
#endif

static void s_hdcp2_content_type_get(
	struct hdcp_obj *p_hdcp_obj,
	enum sii_drv_hdcp_content_type *p_content_type)
{
	*p_content_type = (enum sii_drv_hdcp_content_type)p_hdcp_obj->strmng_2x_msg.content_type;
}

static void s_tpi_hdcp2x_protection_enable(struct hdcp_obj *p_hdcp_obj,
						bool is_enabled)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	if (is_enabled) {
		/* enabling HDCP2.2 control reg for tx*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_0,
			BIT_MSK__TX_HDCP2X_CTRL_0__RI_HDCP2_HDCPTX |
			BIT_MSK__TX_HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE);
	} else {
		/* revert to HDCP1 mode*/
		uint8_t prot_mask = HDCP_CTRL_MODE;

		sii_platform_put_bit8(pstDev, REG_ADDR__TPI_COPP_DATA2,
			prot_mask, is_enabled ? 0xFF : 0x00);

		/* disabling HDCP2.2 mode*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
			BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_EN |
			BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);
		/* disabling HDCP2.2 control reg for tx*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_0,
			BIT_MSK__TX_HDCP2X_CTRL_0__RI_HDCP2_HDMIMODE);
	}
}

#ifdef CONFIG_POWER_DOWN_EN
static void hdcp2x_pdn_mode_auth_start(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	if (p_hdcp_obj->auth_state != SII_MOD_HDCP_EVENT__AUTHENTICATED) {
		/* clearing auth done, fail interrupts */
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0, 0x03);
		/* clearing recv__id_chng intr*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1, 0x02);

		/*enabling re-authentication --- one pulse*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_1, 0xE1);

		/* clear reauth_sw*/
		/* disabling re-authentication*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_1, 0xE0);
		/*for resolving issue when downstream is switched between
		 * 1.4 to 2.2*/

		s_tpi_hdcp_encryption_enable(p_hdcp_obj, true);
		p_hdcp_obj->hdcp2x_repeater_ready = false;
		/* enable h_d_c_p2x interrupts*/
		/*HDCP interrupt0 mask register*/
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_INTR0_MASK,
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B0 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B1 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B6 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B2 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B3);
		/*HDCP interrupt1 mask register*/
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_INTR1_MASK,
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B0 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B1 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B2 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B5);

		p_hdcp_obj->auth_state =
			SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
	}
}
#endif

static void hdcp2x_auth_start(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	/* if 0x50 response with HDCP2.2 capable*/
	if (ds_hdcp22_support_get(p_hdcp_obj)) {
		p_hdcp_obj->hdcp_version = SII_DRV_HDCP_LINK_VER__HDCP22;

		/* clearing auth done, fail interrupts */
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0, 0xFF);
		/* clearing recv__id_chng intr*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR1, 0xFF);

		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
			BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_EN);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL, 0xB2);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL, 0x22);
#ifdef CONFIG_POWER_DOWN_EN
		p_hdcp_obj->hdcp2x_power_up_done = false;
		/* gate off p_clk, mhl1/2/3 //powerdown is enabled.*/
		sii_platform_set_bit8(pstDev, REG_ADDR__PD_REG1,
			BIT_MSK__PD_REG1__REG_HDCP2TX_REAUTH_REQ);
		sii_timer_start(&p_hdcp_obj->timer_hdcp2x_pwr_up_done_poll, 10);
#else
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL,
			0x20);

		/*enabling re-authentication --- one pulse*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_1, 0xE1);

		/* clear reauth_sw*/
		/* disabling re-authentication*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_1, 0xE0);
		/*for resolving issue when downstream is switched between
		 * 1.4 to 2.2*/

		/* Workaround: Allow some time to receive Reauth request
		   from Downstream spiderman and ignore it*/
		usleep(5000);

		if ((sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_AUTH_STAT)) &
				BIT_MSK__TX_HDCP2X_AUTH_STAT__REAUTH_REQ) {
			sii_log_debug("HDCP2x: Reauth Request - Ignore\n");
			/* Initiate re-authentication */
			/*enabling re-authentication --- one pulse*/
			sii_platform_wr_reg8(pstDev,
					REG_ADDR__TX_HDCP2X_CTRL_1, 0xE1);
			/* clear reauth_sw*/
			/* disabling re-authentication*/
			sii_platform_wr_reg8(pstDev,
					REG_ADDR__TX_HDCP2X_CTRL_1, 0xE0);
			/*for resolving issue when downstream is
			 switched between 1.4 to 2.2*/
		}

		/* Clear reauth request interrupt */
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_INTR0,
			BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT6);

		s_tpi_hdcp_encryption_enable(p_hdcp_obj, true);
		p_hdcp_obj->hdcp2x_repeater_ready = false;

		/* enable h_d_c_p2x interrupts*/
		/*HDCP interrupt0 mask register*/
		/*HDCP interrupt0 mask register*/
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_INTR0_MASK,
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B0 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B1 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B6 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B2 |
			BIT_MSK__TX_HDCP2X_INTR0_MASK__INTR0_MASK_B3);
		/*HDCP interrupt1 mask register*/
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_INTR1_MASK,
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B0 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B1 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B2 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B3 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B5 |
			BIT_MSK__TX_HDCP2X_INTR1_MASK__INTR1_MASK_B6);

		p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__NONE;
		p_hdcp_obj->auth_state =
			SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
#endif
	}
}

#ifndef CONFIG_HDCP_REPEATER_EN
static void s_hdcp2x_rd_seq_num(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v =
		sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_V_2);
	p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v =
		(p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v << 8) |
		sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_V_1);
	p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v =
		(p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v << 8) |
		sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_V_0);
}

static void s_hdcp2x_non_rpt_intr_handler(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t hdcp2_auth_status = 0;
	uint8_t hdcp2_auth_state_status = 0;
	enum sii_drv_tx_hdcp_internal_state hdcp_evnt_authnticated
		= SII_MOD_HDCP_EVENT__AUTHENTICATED;


	/* defined to abide 80 character rule */
	uint8_t bit_msk_hdcp2x_encrypt_en =
		BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN;

	hdcp2_auth_status = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_AUTH_STAT);
	hdcp2_auth_state_status = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_STATE);

	/*sii_log_debug("interrupts:%x %x\n", p_hdcp_obj->hdcp2x_int_stat.reg0,
		p_hdcp_obj->hdcp2x_int_stat.reg1);
	sii_log_debug("authstate:%x %x\n", hdcp2_auth_status,
		hdcp2_auth_state_status);*/

	if (p_hdcp_obj->hdcp2x_int_stat.reg1 &
		BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT2) {
		sii_log_debug("AKE init sent\n");
        // Need to update the HDCP status
        // for supporting the case of reauth_req from ds
        p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__WAIT_FOR_DONE;
        p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__NONE;
        s_update_hdcp_state(p_hdcp_obj);
	}
	if (p_hdcp_obj->hdcp2x_int_stat.reg1 &
		BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT6) {
		sii_log_debug("Cert Rcvd\n");
		s_notify_hdcp_status_to_upstream(p_hdcp_obj,
    		SII_DRV_HDCP_EVENT__HDCP_CERT_RCVD);
	}

	if (p_hdcp_obj->hdcp2x_int_stat.reg1
			& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT3) {
			sii_log_debug("SKE event received\n");
#if (CONFIG_SOURCE_ONLY == 0)
			s_notify_hdcp_status_to_upstream(p_hdcp_obj,
				SII_DRV_HDCP_EVENT__SKE_SEND_EKS);
#endif // (CONFIG_SOURCE_ONLY == 0)
			s_reset_ksv_fifo(p_hdcp_obj);
			s_hdcp2x_receiver_rcvids(p_hdcp_obj);

			sii_log_debug("Sink ReceiverID: ");
			s_dump_buffer(p_hdcp_obj->ksv_list.p_list_start,
				p_hdcp_obj->ksv_list.length);
			/*if(!p_hdcp_obj->hdcp_source_only) {
				s_notify_hdcp_status_to_upstream(p_hdcp_obj,
				SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED);
			}*/
	}

	if (p_hdcp_obj->hdcp2x_int_stat.reg2 &
		BIT_MSK__TX_HDCP2X_INTR2__INTR2_STAT0) {
		sii_log_debug("Km sent\n");
#ifdef CONFIG_POWER_DOWN_EN
		sii_platform_clr_bit8(pstDev, REG_ADDR__PD_REG1,
		BIT_MSK__PD_REG1__REG_HDCP2TX_REAUTH_REQ);
#endif
	}
	/* repeater ready interrupt*/
	if (p_hdcp_obj->hdcp2x_int_stat.reg0 &
			BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT2) {
		sii_log_debug("Repeater Ready\n");
		p_hdcp_obj->hdcp2x_repeater_ready = true;
	}
	/* receiver_id changed interrupt */
	if (p_hdcp_obj->hdcp2x_int_stat.reg1
		& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT0) {
		sii_log_debug("Receiver ID Changed\n");

		s_hdcp2x_rd_seq_num(p_hdcp_obj);
		/* store seq num b in topology structure to make availble
		when topology is queried. */
		p_hdcp_obj->hdcp_topology.seq_num_v =
			p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v;

		sii_log_debug("seq_num = 0X%04x\n",
			p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v);

		if (p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v == 0xFFFFFF) {
			sii_platform_clr_bit8(pstDev,
				REG_ADDR__TX_HDCP2X_CTL_0,
				bit_msk_hdcp2x_encrypt_en);
			msleep(100);
			hdcp2_auth_state_status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_STATE);
			if (0x3C != hdcp2_auth_state_status) {
				sii_platform_set_bit8(pstDev,
					REG_ADDR__TX_HDCP2X_CTL_0,
					bit_msk_hdcp2x_encrypt_en);
			}
		}

		/* read receiver_id list */
		s_rcv_id_list_get(p_hdcp_obj);
		sii_log_debug("Repeater ReceiverID List: ");
		s_dump_buffer(p_hdcp_obj->ksv_list.p_list_start + RCVRID_LENGTH,
			p_hdcp_obj->ksv_list.length - RCVRID_LENGTH);
		s_hdcp2x_rd_seq_num(p_hdcp_obj);

#if (CONFIG_SOURCE_ONLY == 1)
			if (p_hdcp_obj->hdcp2x_repeater_ready)
				if ((p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v ==
					0x0000) &&
					(p_hdcp_obj->auth_state !=
						SII_MOD_HDCP_EVENT__AUTHENTICATED)){
					s_hdcp2x_fill_strmng_2x_msg(p_hdcp_obj,
						(struct sii_drv_tx_strmng_2x_msg *)
						&p_hdcp_obj->strmng_2x_msg);
			}
#else
			s_notify_hdcp_status_to_upstream(p_hdcp_obj,
			SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED);
#endif // (CONFIG_SOURCE_ONLY == 1)
	}

	/* smng_xfer_done interrupt */
	if (p_hdcp_obj->hdcp2x_int_stat.reg1
		& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT5) {
		sii_log_debug("smng xfer done\n");
#if (CONFIG_SOURCE_ONLY == 0)
			s_notify_hdcp_status_to_upstream(p_hdcp_obj,
				SII_DRV_HDCP_EVENT__HDCP_STMNG_EVENT);
#endif // (CONFIG_SOURCE_ONLY == 0)
	}

	/* authenctication done interrupt*/
	if (p_hdcp_obj->hdcp2x_int_stat.reg0
		& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT0) {
		hdcp2_auth_status = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_AUTH_STAT);
		hdcp2_auth_state_status = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_STATE);
		if ((0x01 & hdcp2_auth_status)
			&& (0x2B == hdcp2_auth_state_status)) {

			if (p_hdcp_obj->hdcp2x_int_stat.reg0 &
				BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT3) {
				/* hash fail don't do this serve
				hash fail interrupt
				*/
				/* due to interrupt polling mode
				sometimes both
				interrupts are coming at a time
				*/
			} else {
				p_hdcp_obj->auth_fail_counter = 0;
				msleep(200);
				sii_platform_set_bit8(pstDev,
					REG_ADDR__TX_HDCP2X_CTL_0,
					bit_msk_hdcp2x_encrypt_en);
				p_hdcp_obj->auth_state = hdcp_evnt_authnticated;
				sii_timer_stop(&p_hdcp_obj->timer_ddc_poll);
				s_update_hdcp_state(p_hdcp_obj);
				s_print_hdcp_status(p_hdcp_obj);
				s_notify_hdcp_status(p_hdcp_obj);
				sii_log_debug("Authentication Done\n");
			}
		}
	}
	if (p_hdcp_obj->hdcp2x_int_stat.reg0
		& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT1) {
		p_hdcp_obj->auth_fail_counter++;

		/*Update the failure reason*/
		if (p_hdcp_obj->auth_state ==
			SII_MOD_HDCP_EVENT__WAIT_FOR_DONE) {
			if(!is_d_sdevice_hdcp2_repeater(p_hdcp_obj)) {
				if (!p_hdcp_obj->hdcp2x_repeater_ready) {
					/*Error in waiting for rptr_ready*/
					p_hdcp_obj->error_state =
						SII_DRV_HDCP_FAILURE__TIMEOUT;
				}
				s_update_hdcp_topology(p_hdcp_obj);
				if(p_hdcp_obj->hdcp_topology.max_devs_exceeded)
					p_hdcp_obj->error_state =
					SII_DRV_HDCP_FAILURE__DEV_EXC;
				if(p_hdcp_obj->hdcp_topology.max_cascade_exceeded)
					p_hdcp_obj->error_state =
					SII_DRV_HDCP_FAILURE__CAS_EXC;
			} else {
				/*Error in receiving/readng KSV. most
				 *probably the error is caused by V mismatch
				 */
				p_hdcp_obj->error_state =
					SII_DRV_HDCP_FAILURE__V;
			}
		}

		if (p_hdcp_obj->auth_fail_counter >= HDCP2X_RETRY_THRESHOLD) {
			p_hdcp_obj->auth_fail_counter = 0;
			sii_log_debug("auth fail\n");
			p_hdcp_obj->error_state =
				SII_DRV_HDCP_FAILURE__AUTH_FAIL;
			p_hdcp_obj->auth_state = SII_MOD_HDCP_EVENT__FAILED;
				p_hdcp_obj->hdcp2x_repeater_ready = false;
				s_hdcp2x_redo(p_hdcp_obj);
 		}
   		s_notify_hdcp_status_to_upstream(p_hdcp_obj,
			SII_DRV_HDCP_EVENT__HDCP_REAUTH_REQ);

	}

	if (p_hdcp_obj->hdcp2x_int_stat.reg0 &
		BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT3) {
		sii_log_debug("hash fail\n");
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
			BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN);
#ifdef CONFIG_POWER_DOWN_EN
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_AESCTL, 0x62);
#endif
		p_hdcp_obj->error_state = SII_DRV_HDCP_FAILURE__HASH;
#if (CONFIG_SOURCE_ONLY == 1)
			s_hdcp2x_fill_strmng_2x_msg(p_hdcp_obj,
				(struct sii_drv_tx_strmng_2x_msg *)
				&p_hdcp_obj->strmng_2x_msg);
#else
			s_notify_hdcp_status_to_upstream(p_hdcp_obj,
				(SII_DRV_HDCP_EVENT__HDCP_HASH_FAIL_EVENT|
				SII_DRV_HDCP_EVENT__HDCP_RESET_FLAGS));
#endif // (CONFIG_SOURCE_ONLY == 1)
	}
	if (p_hdcp_obj->hdcp2x_int_stat.reg0
		& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT6) {
			hdcp2_auth_status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_AUTH_STAT);
			if (hdcp2_auth_status & 0x40) {
				sii_log_debug("Reauth Request\n");
				p_hdcp_obj->error_state =
					SII_DRV_HDCP_FAILURE__REAUTH_REQ;
				p_hdcp_obj->hdcp2x_repeater_ready = false;
				s_hdcp2x_redo(p_hdcp_obj);
        		s_notify_hdcp_status_to_upstream(p_hdcp_obj,
            		SII_DRV_HDCP_EVENT__HDCP_REAUTH_REQ);
			}
	}
}

static void s_rcv_id_list_get(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t ds_count = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RPT_DEVCNT);
	uint16_t fifo_byte_counter;

	p_hdcp_obj->ksv_list.p_list =
		(p_hdcp_obj->ksv_list.p_list_start + RCVRID_LENGTH);
	p_hdcp_obj->ksv_list.length = RCVRID_LENGTH;

	fifo_byte_counter = ds_count * SII_HDCP2X_RCVID_LENGTH;

	/*rcvid_rd_start=1*/
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START);
	while (fifo_byte_counter--) {
		*(p_hdcp_obj->ksv_list.p_list) = sii_platform_rd_reg8(pstDev,
			REG_ADDR__TX_HDCP2X_TX_RPT_RCVID_OUT);
		p_hdcp_obj->ksv_list.p_list += 1;
		/*rcvid_rd=1*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD);
		/*rcvid_rd=0*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD);
	}
	/*rcvid_rd_start=0*/
	sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START);

	p_hdcp_obj->ksv_list.p_list = p_hdcp_obj->ksv_list.p_list_start;
	p_hdcp_obj->ksv_list.length += (ds_count * SII_HDCP2X_RCVID_LENGTH);
}
#endif

static void s_hdcp2x_receiver_rcvids(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;

	p_hdcp_obj->ksv_list.p_list[4] = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_ID_CORE_0);
	p_hdcp_obj->ksv_list.p_list[3] = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_ID_CORE_1);
	p_hdcp_obj->ksv_list.p_list[2] = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_ID_CORE_2);
	p_hdcp_obj->ksv_list.p_list[1] = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_ID_CORE_3);
	p_hdcp_obj->ksv_list.p_list[0] = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RX_ID_CORE_4);

	p_hdcp_obj->ksv_list.length += RCVRID_LENGTH;
	p_hdcp_obj->ksv_list.p_list += RCVRID_LENGTH;
}

#ifdef CONFIG_HDCP_REPEATER_EN
static void s_hdcp2x_rpt_intr_handler(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t hdcp2_auth_status = 0;
	uint8_t hdcp2_auth_state_status = 0;

	uint8_t bit_msk_hdcp2x_encrypt_en =
		BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN;

	sii_log_debug("TxsHdcp2xRpt = %x %x\n",
			p_hdcp_obj->hdcp2x_int_stat.reg0,
			p_hdcp_obj->hdcp2x_int_stat.reg1);

	/* for downstream repeater cts case issue...h_p_d is deasserted
	 * after entering
	 tinto state machine and before executing hdcp off this instructions
	 will execute...so hpd control is placed..*/
	if (p_hdcp_obj->tx_hpd_status == true) {
		if (p_hdcp_obj->hdcp2x_int_stat.reg1
			& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT2) {
			/* AKE sent received */
			sii_timer_stop(
				&p_hdcp_obj->timer_ddc_poll);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_0, 0);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_1, 0);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_2, 0);

			p_hdcp_obj->b_tx_2x_rpt_msg.ake_init_rcvd = true;
			p_hdcp_obj->b_tx_2x_rpt_msg.ske_event = false;
			p_hdcp_obj->b_tx_2x_rpt_msg.b_hdcp2_rx_ready = false;
			p_hdcp_obj->b_tx_2x_rpt_msg.rcv_id_change = false;

			s_notify_hdcp_status_to_upstream(p_hdcp_obj,
				SII_DRV_HDCP_EVENT__HDCP_RESET_FLAGS);
			sii_log_debug("AKE SENT received\n");
		}
		/*ske sent received changed interrupt*/
		if (p_hdcp_obj->hdcp2x_int_stat.reg1
			& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT3) {
			sii_log_debug("SKE event received\n");
			p_hdcp_obj->b_tx_2x_rpt_msg.ske_event = true;
			s_hdcp2x_receiver_rcvids(p_hdcp_obj);
			supdate_hdcp2_rpt_rcvids(p_hdcp_obj,
					p_hdcp_obj->ksv_list.p_list_start);
			/* make this call for all set cases	  */
			sii_timer_start(&p_hdcp_obj->timer_rpt_rcv_id_poll,
				TIMER_START__TX_HDCP__RPT_RCVID_FAIL);
		}
		/* receiver_i_d changed interrupt*/
		if (p_hdcp_obj->hdcp2x_int_stat.reg1
			& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT0) {
			sii_log_debug("Receiver ID changed\n");
			p_hdcp_obj->b_tx_2x_rpt_msg.b_hdcp2_rx_ready = true;
			p_hdcp_obj->b_tx_2x_rpt_msg.rcv_id_change = true;

			/* read receiver_i_d list*/
			if (s_rpt_rcvd_list(p_hdcp_obj))
				s_notify_hdcp_status_to_upstream(p_hdcp_obj,
					SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED);
			if (p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v == 0)
				s_hdcp2x_send_strmng_2x_msg(p_hdcp_obj);
		}
		/*stream manage message ready*/
		if (p_hdcp_obj->hdcp2x_int_stat.reg1
			& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT5) {
			sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
				bit_msk_hdcp2x_encrypt_en);
			sii_log_debug("Stream Manage message ready\n");
		}
		/*stream manage message ready*/
		if (p_hdcp_obj->hdcp2x_int_stat.reg1
			& BIT_MSK__TX_HDCP2X_INTR1__INTR1_STAT1) {
			sii_log_debug("Stream Manage message changed\n");
		}
		/* authenctication done interrupt*/
		if (p_hdcp_obj->hdcp2x_int_stat.reg0
			& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT0) {
			hdcp2_auth_status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_AUTH_STAT);
			hdcp2_auth_state_status = sii_platform_rd_reg8(pstDev,
				REG_ADDR__TX_HDCP2X_STATE);
#ifndef CONFIG_POWER_DOWN_EN
			if ((0x81 == hdcp2_auth_status)
					&& (0x2B == hdcp2_auth_state_status))
#endif
			{
				sii_platform_set_bit8(pstDev,
					REG_ADDR__TX_HDCP2X_CTL_0,
					bit_msk_hdcp2x_encrypt_en);
				sii_log_debug("HDCP authenticated ");
				sii_log_debug("successfully-- ");
				sii_log_debug("notify to application\n");
				p_hdcp_obj->auth_state =
					SII_MOD_HDCP_EVENT__AUTHENTICATED;
				s_update_hdcp_state(p_hdcp_obj);
				s_print_hdcp_status(p_hdcp_obj);
				s_notify_hdcp_status(p_hdcp_obj);
			}
		}
		/*authentication fails*/
		if (p_hdcp_obj->hdcp2x_int_stat.reg0
			& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT1) {
			/*for cts issue*/
			sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_CTL_0,
				bit_msk_hdcp2x_encrypt_en);
			sii_log_debug("HDCP authentication fail--notify to ");
			sii_log_debug("application\n");
		}
		if (p_hdcp_obj->hdcp2x_int_stat.reg0
			& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT6) {
			s_hdcp2x_rpt_re_auth_send(p_hdcp_obj, false);
			sii_log_debug("Reauth Request\n");
		}
	} else
		sii_log_debug("HPD-DE-ASSERTED\n");
}

static void s_hdcp2x_rpt_re_auth_send(struct hdcp_obj *p_hdcp_obj,
					bool re_auth_delay_check)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	if ((sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_CTRL_0)
		& BIT_MSK__TX_HDCP2X_CTRL_0__RI_HDCP2_HDCPTX)
		&& (p_hdcp_obj->rpt_reauth_done == false)) {

		p_hdcp_obj->rpt_reauth_done = true;
		if (p_hdcp_obj->hdcp2x_int_stat.reg0
			& BIT_MSK__TX_HDCP2X_INTR0__INTR0_STAT1) {

			sii_timer_start(&p_hdcp_obj->timer2x_rpt_re_auth,
					re_auth_delay_check ?
					TIMER_START__TX_HDCP__RPT_FAIL :
					TIMER_START__TX_HDCP__RPT_REAUTH_REQ);
		}
		/*	sii_log_seq_timer_start(p_hdcp_obj->timer2x_rpt_re_auth,
		 re_auth_delay_check?TIMER_START__TX_HDCP__RPT_FAIL :
		 TIMER_START__TX_HDCP__RPT_REAUTH_REQ, 0);*/
	}
}

static bool s_rpt_rcvd_list(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t hdcp_misc_status;
	uint32_t seq_num_v = 0;

	/* defined to abide 80 character rule */
	uint8_t bit_msk_max_dev_exceeded =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_DEVS_EXC;
	uint8_t bit_msk_max_casc_exceeded =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_MX_CASC_EXC;
	uint8_t bit_msk_rpt_hdcp1dev_dstrm =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP1DEV_DSTRM;
	uint8_t bit_msk_rpt_hdcp2rpt_dstrm =
		BIT_MSK__TX_HDCP2X_RPT_DETAIL__RI_HDCP2RX_RPT_HDCP20RPT_DSTRM;
	uint8_t bit_msk_rpt_hdcp2x_rcvid_rd =
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD;

	hdcp_misc_status = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RPT_DETAIL);

	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.casc_exceed =
		(hdcp_misc_status & bit_msk_max_casc_exceeded) ? true : false;

	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.dev_exceed =
		(hdcp_misc_status & bit_msk_max_dev_exceeded) ? true : false;

	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.hdcp1ds_rpt =
		(hdcp_misc_status & bit_msk_rpt_hdcp1dev_dstrm) ? true : false;

	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.hdcp2ds_rpt =
		(hdcp_misc_status & bit_msk_rpt_hdcp2rpt_dstrm) ? true : false;

	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.depth = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RPT_DEPTH);
	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.devcnt = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_HDCP2X_RPT_DEVCNT);

	p_hdcp_obj->hdcp2x_rpt_rcv_id_list.length =
		p_hdcp_obj->ksv_list.length;

	sii_log_debug("Received ID Changed\n");
	p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v =
		sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_V_2);
	p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v =
		(p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v << 8) |
		sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_V_1);
	p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v =
		(p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v << 8) |
		sii_platform_rd_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_V_0);

	sii_log_debug("Sequence Num: %x\n",
		p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v);

	if (p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v == 0xffffff)
		/* seq_num_v is neither valid nor equal to expected seq_num_v*/

		sii_log_debug("Invalid Sequence number\n");

	/* if it's first time downstream receive receiver_i_d list,
	 * need to send smng
	 if upstream is ready.*/
	if (p_hdcp_obj->b_tx_2x_rpt_msg.seq_num__v != seq_num_v)
		sii_log_debug("Invalid Sequence number-- not matched\n");

	/*rcvid_rd_start=1*/
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START);
	sii_platform_clr_bit8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
		BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_RCVID_RD_START);

	for (j = 0; j < p_hdcp_obj->hdcp2x_rpt_rcv_id_list.devcnt; j++) {
		for (i = 0; i < RCVRID_LENGTH; i++) {
			p_hdcp_obj->hdcp2x_rpt_rcv_id_list.rpt_rcvid[j].id[i] =
				sii_platform_rd_reg8(pstDev,
					REG_ADDR__TX_HDCP2X_TX_RPT_RCVID_OUT);
			/*rcvid_rd=1*/
			sii_platform_set_bit8(pstDev,
				REG_ADDR__TX_HDCP2X_TX_CTRL_0,
				bit_msk_rpt_hdcp2x_rcvid_rd);
			/*rcvid_rd=1*/
			sii_platform_clr_bit8(pstDev,
				REG_ADDR__TX_HDCP2X_TX_CTRL_0,
				bit_msk_rpt_hdcp2x_rcvid_rd);
		}
	}

	sii_log_debug("Reading Received ID Completed\n");
	return true;

}

static void s_tx2x_repeater_re_auth_handler(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;
    pstSii9630_Device_t pstDev;// = p_hdcp_obj->pstDevice;
	uint8_t bit_msk_hdcp2x_encrypt_en =
		BIT_MSK__TX_HDCP2X_CTL_0__REG_HDCP2X_ENCRYPT_EN;
	uint8_t bit_msk_hdcp2x_reauth_sw =
		BIT_MSK__TX_HDCP2X_CTL_1__REG_HDCP2X_REAUTH_SW;

	if (NULL == p_hdcp_obj)
		return;

	pstDev = p_hdcp_obj->pstDevice;

	if (!down_interruptible(&p_hdcp_obj->drv_context->isr_lock)) {
		if (p_hdcp_obj->is_auth_requested == true) {
			sii_log_debug("Auth failed\n");
			/*for cts issue*/
			sii_platform_clr_bit8(pstDev,
				REG_ADDR__TX_HDCP2X_CTL_0,
				bit_msk_hdcp2x_encrypt_en);

			sii_platform_set_bit8(pstDev,
				REG_ADDR__TX_HDCP2X_CTRL_0,
				bit_msk_hdcp2x_reauth_sw);

			sii_platform_clr_bit8(pstDev,
				REG_ADDR__TX_HDCP2X_CTRL_0,
				bit_msk_hdcp2x_reauth_sw);

			p_hdcp_obj->rpt_reauth_done = false;
		}
		up(&p_hdcp_obj->drv_context->isr_lock);
	}
}

static void s_tx2x_rpt_rcv_id_timeout_handler(void *p_obj)
{
	struct hdcp_obj *p_hdcp_obj = (struct hdcp_obj *)p_obj;
    pstSii9630_Device_t pstDev;// = p_hdcp_obj->pstDevice;
	uint16_t reg_tx_hdcp2x_ctrl_0 = REG_ADDR__TX_HDCP2X_CTRL_0;
	uint8_t bit_msk_reauth_sw =
		BIT_MSK__TX_HDCP2X_CTL_1__REG_HDCP2X_REAUTH_SW;
	if (NULL == p_hdcp_obj)
		return;
	pstDev = p_hdcp_obj->pstDevice;

	if (!down_interruptible(&p_hdcp_obj->drv_context->isr_lock)) {
		if ((p_hdcp_obj->tx_hpd_status)
			&& (p_hdcp_obj->b_tx_2x_rpt_msg.ske_event)) {

			if (is_d_sdevice_hdcp2_repeater(p_hdcp_obj)) {
				if (p_hdcp_obj->b_tx_2x_rpt_msg.rcv_id_change) {
					;
				} else {
					sii_log_debug("timeout-- ");
					sii_log_debug("Receiver ID ");
					sii_log_debug("is not ");
					sii_log_debug("recieved ");
					sii_log_debug("before 3 sec ");
					sii_log_debug("of SKE ");
					sii_log_debug("arrived!!!\n");

					p_hdcp_obj->auth_state =
					SII_MOD_HDCP_EVENT__FAILED;

					s_reset_ksv_fifo(p_hdcp_obj);
					/*disable h_d_c_p2x intr masks*/

					sii_platform_set_bit8(pstDev,
						reg_tx_hdcp2x_ctrl_0,
						bit_msk_reauth_sw);
					s_notify_hdcp_status(
							p_hdcp_obj);
				}
			}
		}
		up(&p_hdcp_obj->drv_context->isr_lock);
	}
}

static void supdate_hdcp2_rpt_rcvids(struct hdcp_obj *p_hdcp_obj,
					uint8_t *p_rcv_ids)
{
	if (is_d_sdevice_hdcp2_repeater(p_hdcp_obj)) {
		sii_log_debug("supdateHdcp2RptRcvids -- Downstream repeater\n");
	} else {
		uint8_t i = 0;

		sii_log_debug("supdateHdcp2RptRcvids -- Downstream receiver\n");
		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.casc_exceed = false;
		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.dev_exceed = false;

		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.hdcp1ds_rpt = false;
		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.hdcp2ds_rpt = false;

		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.depth = 0;
		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.devcnt = 0;

		p_hdcp_obj->hdcp2x_rpt_rcv_id_list.length = 5;

		for (i = 0; i < p_hdcp_obj->hdcp2x_rpt_rcv_id_list.length;
			i++) {
			p_hdcp_obj->hdcp2x_rpt_rcv_id_list.rpt_rcvid[0].id[i] =
				*p_rcv_ids++;
		}
		if ((p_hdcp_obj->b_tx_2x_rpt_msg.ske_event))
			s_notify_hdcp_status_to_upstream(p_hdcp_obj,
				SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED);
	}
}
#endif
#if (CONFIG_SOURCE_ONLY == 1)
static void s_hdcp2x_send_strmng_2x_msg(struct hdcp_obj *p_hdcp_obj)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;

#ifdef CONFIG_HDCP_REPEATER_EN
	if ((p_hdcp_obj->b_tx_2x_rpt_msg.ske_event)
		&& (p_hdcp_obj->b_tx_2x_rpt_msg.b_hdcp2_rx_ready)
		&& (p_hdcp_obj->b_tx_2x_rpt_msg.smng_enable))
#endif
	{
		sii_log_debug("Sending Stmng Message..\n");

		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_0,
			(uint8_t)p_hdcp_obj->strmng_2x_msg.seq_num_m);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_1,
			(uint8_t)(p_hdcp_obj->strmng_2x_msg.seq_num_m >> 8));
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_2,
			(uint8_t)(p_hdcp_obj->strmng_2x_msg.seq_num_m >> 16));

		p_hdcp_obj->strmng_2x_msg.seq_num_m++;

		if (p_hdcp_obj->strmng_2x_msg.seq_num_m == 0xFFFFFF) {
			p_hdcp_obj->strmng_2x_msg.seq_num_m = 0;
		}
	}
}
#endif // (CONFIG_SOURCE_ONLY == 1)
static void s_hdcp2x_fill_strmng_2x_msg(struct hdcp_obj *p_hdcp_obj,
		struct sii_drv_tx_strmng_2x_msg *p_stmng)
{
    pstSii9630_Device_t pstDev = p_hdcp_obj->pstDevice;
	uint8_t timeout;
	uint8_t retry = 2;

	/* defined to avoid 80 character rule */
	uint8_t bit_msk_rpt_smng_xfer_done =
		BIT_MSK__TX_HDCP2X_TX_STATUS__RO_HDCP2TX_RPT_SMNG_XFER_DONE;

	while(retry -- != 0 ){
		/* smng_xfer_start=0-just to make sure*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
		    0);

    		p_hdcp_obj->b_tx_2x_rpt_msg.smng_enable = true;

    		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RPT_SMNG_K,
    			p_stmng->stream_k);

#if (CONFIG_SOURCE_ONLY == 1)
		s_hdcp2x_send_strmng_2x_msg(p_hdcp_obj);
#else
		sii_log_debug("Sending Stmng Message..\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_0,
			(uint8_t) ((p_stmng->seq_num_m>>0) & 0x0FF));
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_1,
			(uint8_t) ((p_stmng->seq_num_m>>8) & 0x0FF));
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_RX_SEQ_NUM_M_2,
			(uint8_t) ((p_stmng->seq_num_m>>16) & 0x0FF));
#endif // (CONFIG_SOURCE_ONLY == 1)

    		/* smng_wr_start=1*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR_START);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			0);

		/* smng_in (stream ID)*/
    		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_RPT_SMNG_IN,
    			p_stmng->stream_i_d);
    		/* smng_wr */
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			0);

    		/* smng_in (type)*/
    		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_RPT_SMNG_IN,
    			(uint8_t) p_stmng->content_type);
    		/* smng_wr */
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_WR);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			0);
    		/* smng_xfer_start=1-rising edge will start transfer*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			BIT_MSK__TX_HDCP2X_TX_CTRL_0__RI_HDCP2TX_RPT_SMNG_XFER_START);


    		usleep(5000);
    		timeout = HDCP2X_CONTENT_TYPE_SET_MAX_TIMEOUT;
    		/* wait smng_xfer_done*/
    		while (--timeout
    			&& !(sii_platform_rd_reg8(pstDev,
    				REG_ADDR__TX_HDCP2X_TX_STATUS)
    				& bit_msk_rpt_smng_xfer_done)) {
    			usleep(1000);
    		}
		if(timeout){
			break;
		}else{
			sii_log_err("...retrying contentType\n");
		}

		if (!timeout) {
			sii_log_err("Unable to set contentType\n");
			sii_log_err("Stream Manage Transfer failed\n");
		} else
			sii_log_info("ContentType set to %s\n",
		p_stmng->content_type ?
					"SII_DRV_HDCP_CONTENT_TYPE__1" :
					"SII_DRV_HDCP_CONTENT_TYPE__0");
		/* smng_xfer_start=0*/
		/* smng_wr_start=0*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_HDCP2X_TX_CTRL_0,
			0);
		p_hdcp_obj->strmng_2x_msg.content_type = p_stmng->content_type;
	}
}
#endif

void mapp_Sil9630TxHdcpHandler(void *p_obj)
{
    #ifdef CONFIG_HDCP_REPEATER_EN
    s_tx2x_rpt_rcv_id_timeout_handler(p_obj);
    s_tx2x_repeater_re_auth_handler(p_obj);
    #endif
    s_hdcp_ddc_ack_poll(p_obj);
    s_hdcp_restart_1xauth(p_obj);
    #ifdef CONFIG_POWER_DOWN_EN
    s_hdcp2x_pwr_up_monitor(p_obj);
    #endif
}

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

