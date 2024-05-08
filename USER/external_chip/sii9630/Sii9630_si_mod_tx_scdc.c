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

#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_mod_tx_scdc.h"
#include "Sii9630_si_mod_tx_hdmi.h"
#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_platform.h"
#include "Sii9630_si_timer.h"


/* scramble states */
#define SCRAMBLE_INITIAL	0
#define SCRAMBLE_WAIT		1
#define SCRAMBLE_ENABLED	2
#define SCRAMBLE_NONE		3

#define DDC_OFFSET__SCDC_SINK_VERSION				0x01
#define DDC_OFFSET__SCDC_SOURCE_VERSION				0x02

#define DDC_OFFSET__SCDC_UPDATE_0				0x10
#define BIT_MASK__UPDATE_0__STATUS_UPDATE			0x01
#define BIT_MASK__UPDATE_0__CED_UPDATE				0x02
#define BIT_MASK__UPDATE_0__RR_TEST				0x04

#define DDC_OFFSET__SCDC_UPDATE_1				0x11

#define DDC_OFFSET__SCDC_TMDS_CONFIG				0x20
#define BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE			0x01
#define BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO			0x02

#define DDC_OFFSET__SCDC_TMDS_STATUS				0x21
#define BIT_MASK__TMDS_STATUS__SCRAMBLE_STATUS			0x01

#define DDC_OFFSET__SCDC_CONGIG_0				0x30
#define BIT_MASK__CONGIG_0__RR_ENABLE				0x01

#define DDC_OFFSET__SCDC_STATUS_FLAGS_0				0x40
#define BIT_MASK__STATUS_FLAGS__CLK_DETECTED			0x01
#define BIT_MASK__STATUS_FLAGS__CH0_LOCKED			0x02
#define BIT_MASK__STATUS_FLAGS__CH1_LOCKED			0x04
#define BIT_MASK__STATUS_FLAGS__CH2_LOCKED			0x08

#define DDC_OFFSET__SCDC_CHANNEL_0_ERROR_COUNT_L		0x50
#define DDC_OFFSET__SCDC_CHANNEL_0_ERROR_COUNT_H		0x51
#define DDC_OFFSET__SCDC_CHANNEL_1_ERROR_COUNT_L		0x52
#define DDC_OFFSET__SCDC_CHANNEL_1_ERROR_COUNT_H		0x53
#define DDC_OFFSET__SCDC_CHANNEL_2_ERROR_COUNT_L		0x54
#define DDC_OFFSET__SCDC_CHANNEL_2_ERROR_COUNT_H		0X55


#define DDC_OFFSET__SCDC_TEST_CONFIG_0				0xC0
#define BIT_MASK__TEST_CONFIG_0__TEST_READ_REQUEST		0x80
#define BIT_MASK__TEST_CONFIG_0__TRR_DELAY_MS			0x7F

#define DDC_OFFSET__SCDC_MANF_OUI_1				0xD0
#define DDC_OFFSET__SCDC_MANF_OUI_2				0xD1
#define DDC_OFFSET__SCDC_MANF_OUI_3				0xD2

#define DDC_OFFSET__SCDC_MANF_DEV_ID_STR			0xD3

#define DDC_OFFSET__SCDC_MANF_DEV_ID_HW_REV			0xDB
#define BIT_MASK__MANF_DEV_ID_HW_REV__MAJOR			0xF0
#define BIT_MASK__MANF_DEV_ID_HW_REV__MINOR			0x0F

#define DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MAJOR_REV		0xDC
#define DDC_OFFSET__SCDC_MANF_DEV_ID_SW_MINOR_REV		0xDD

#if 0
/*! DDC bus access error codes */
enum ddc_com_err {
	SI_TX_DDC_ERROR_CODE_NO_ERROR = 0x00,
	SI_TX_DDC_ERROR_CODE_TIMEOUT = 0x01,
	SI_TX_DDC_ERROR_CODE_NO_ACK = 0x02,
	SI_TX_DDC_ERROR_CODE_BUSY = 0x03,
	SI_TX_DDC_ERROR_CODE_TX_HW = 0x04,
	SI_TX_DDC_ERROR_CODE_LIM_EXCEED = 0x05,
	SI_TX_DDC_ERROR_CODE_FIFO_EMPTY = 0x06,
};
#endif

struct tx_scdc_obj {
	/* static instance configuration*/
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;

	/* max wait for scrambling enabled*/
	void *timer_scramble_expire;
	/* Banner Source SCDC version */
	uint8_t src_version;
	/* Banner Source Read Request capability */
	uint8_t b_read_req_cap;
	/* Output TMDS clock */
	uint32_t tmds_clock;
	/* Connected Sink Capabilities */
	struct sii_drv_tx_scdc_sink_caps sink_caps;
	struct sii_drv_tx_scdc_status_update_regs update_registers;

	struct sii_drv_tx_scdc_manufacturer_status peer_manf_status;
	struct sii_drv_tx_scdc_register_status scdcreg_status;
	enum sii_drv_scdc_source_scrmble_status source_scmble_status;
	bool scramble_debug;
#ifdef CONFIG_ENHANCED_MODE_PROT
	struct em_packet em_packet;
	uint8_t ds_em_cap;
	uint8_t ds_emm_resp;
	uint8_t expected_msg;
	uint8_t emm_req_mode;
	enum sii_tmds_mode tx_tmds_mode;
	void *rcv_stat_timer;
	int retry;
#endif
	void (*event_notify_fn)(void *, uint32_t);

};

static void s_scramble_timeout(void *p_scdc_obj);
static void s_downstream_scdc_reg_read(struct tx_scdc_obj *p_obj);
static void s_scdc_manufacturer_reg_read(struct tx_scdc_obj *p_obj);
static void s_scdc_source_scrambling_status(struct tx_scdc_obj *p_obj);
static void s_scdc_scramble_ctrl(struct tx_scdc_obj *p_obj, bool enable);
static void s_scdc_read_request_test(struct tx_scdc_obj *p_obj,
					uint32_t ms_delay);
static void s_scdc_source_version_set(struct tx_scdc_obj *p_obj, uint8_t ver);
static void s_scdc_read_request_enable(struct tx_scdc_obj *p_obj,
	uint8_t enable);
static void s_scdc_tmds_clk_enable(struct tx_scdc_obj *p_obj,
	uint32_t tmds_clk);
static void s_scdc_scramble_timeout_set(struct tx_scdc_obj *p_obj,
					bool scramble_timeout);
static void s_scdc_read_update_registers(struct tx_scdc_obj *p_obj);
static void s_scdc_sink_cap_set(struct tx_scdc_obj *p_obj,
				struct sii_drv_tx_scdc_sink_caps *sink_caps);
static void s_scdc_reset(struct tx_scdc_obj *p_obj);
#ifdef CONFIG_ENHANCED_MODE_PROT
static void poll_scdc_rcv_status(void *obj);
#endif
static struct tx_scdc_obj sg_sttx_scdc_obj;

/* public functions */
void *sii_mod_tx_scdc_init(pstSii9630_Device_t pstDevice,
				void (*event_notify_fn)(void *, uint32_t))
{
	struct tx_scdc_obj *p_obj = &sg_sttx_scdc_obj;
	int tstatus = 0;
	/* allocate memory for object */
	//p_obj = kzalloc(sizeof(struct tx_scdc_obj), GFP_KERNEL);
	memset(&sg_sttx_scdc_obj,0,sizeof(struct tx_scdc_obj));

	if (!p_obj)
		return NULL;

	//p_obj->drv_context = drv_context;
    p_obj->pstDevice = pstDevice;
	p_obj->event_notify_fn = event_notify_fn;
	p_obj->src_version = 1;
	p_obj->b_read_req_cap = true;

	/* create timers for scrambling control */
	tstatus = sii_timer_create(s_scramble_timeout, p_obj,
		&p_obj->timer_scramble_expire,eTimerSil_tx_scdc_scramble_timeout);
	if (tstatus != 0) {
		sii_log_debug(
			"Failed to allocate SCDC SCR_Wait_Expire timer!\n");
		goto exit;
	}

#ifdef CONFIG_ENHANCED_MODE_PROT
	tstatus = sii_timer_create(poll_scdc_rcv_status, p_obj,
		&p_obj->rcv_stat_timer,eTimerSil_tx_scdc_scdc_rcv_status);
	if (tstatus != 0) {
		sii_log_debug(
			"Failed to allocate SCDC_RCV_STAT Polling timer!\n");
		goto free_scamble_timeout;
	}
#endif
	return p_obj;

#ifdef CONFIG_ENHANCED_MODE_PROT
free_scamble_timeout:
		if (p_obj->timer_scramble_expire) {
			sii_timer_stop(&p_obj->timer_scramble_expire);
			sii_timer_delete(&p_obj->timer_scramble_expire);
			p_obj->timer_scramble_expire = NULL;
		}
#endif
exit:
	//kfree(p_obj);
	return NULL;
}

#if 0
void sii_mod_tx_scdc_exit(void *p_scdc_obj)
{
	struct tx_scdc_obj *p_obj = (struct tx_scdc_obj *)p_scdc_obj;
#ifdef CONFIG_ENHANCED_MODE_PROT
	if (p_obj->rcv_stat_timer) {
		sii_timer_stop(&p_obj->rcv_stat_timer);
		sii_timer_delete(&p_obj->rcv_stat_timer);
		p_obj->rcv_stat_timer = NULL;
	}
#endif
	if (p_obj->timer_scramble_expire) {
		sii_timer_stop(&p_obj->timer_scramble_expire);
		sii_timer_delete(&p_obj->timer_scramble_expire);
		p_obj->timer_scramble_expire = NULL;
	}

	kfree(p_obj);
}
#endif

int init_scdc_tx_regs(void *p_obj)
{
	struct tx_scdc_obj *p_scdc_obj = (struct tx_scdc_obj *)p_obj;
    pstSii9630_Device_t pstDev = p_scdc_obj->pstDevice;
	int status = 0;

	p_scdc_obj->scramble_debug = false;
	sii_platform_set_bit8(pstDev, REG_ADDR__SCDC_CTL,
		BIT_MSK__SCDC_CTL__REG_SCDC_AUTO_REPLY);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SCDC_INTR0_MASK,
		(BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK2
		| BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK3
		| BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK4
		| BIT_MSK__SCDC_INTR0_MASK__REG_SCDC_INTR0_MASK5));

	return status;
}

int sii_mod_tx_scdc_stop_timers(void *p_obj)
{
	struct tx_scdc_obj *p_scdc_obj = (struct tx_scdc_obj *)p_obj;
	int status = 0;

	if (!p_scdc_obj)
		return 1;

	/*stop all timers*/
	if (p_scdc_obj->timer_scramble_expire) {
		sii_timer_stop(&p_scdc_obj->timer_scramble_expire);
		/*sii_timer_delete(&p_scdc_obj->timer_scramble_expire);
		p_scdc_obj->timer_scramble_expire = NULL;*/
	}
	return status;
}

int sii_mod_tx_scdc_set(void *p_scdc_obj,
	enum sii_mod_scdc_internal_opcode opcode, const void *in_data)
{
	struct tx_scdc_obj *p_obj = (struct tx_scdc_obj *)p_scdc_obj;
	int ret_status = 0;

	if (p_scdc_obj) {
		switch (opcode) {
		case SII_MOD_SCDC_OPCODE__READ_REQ_CAP_SET:
		{
			uint8_t enable = *(uint8_t *)in_data;

			s_scdc_read_request_enable(p_obj, enable);
			break;
		}
		case SII_MOD_SCDC_OPCODE__ENABLE_READ_REQ_TEST:
		{
			uint32_t delay = *(uint32_t *)in_data;

			s_scdc_read_request_test(p_obj, delay);
			break;
		}
		case SII_MOD_SCDC_OPCODE__SCDC_SCRAMBLE_EN:
		{
			uint8_t enable_scarmble =
				(*(uint8_t *)in_data) ? true : false;

			s_scdc_scramble_ctrl(p_obj, (bool)enable_scarmble);
			break;
		}
		case SII_MOD_SCDC_OPCODE__SCDC_RESET_CAP:
		{
			s_scdc_reset(p_obj);
			break;
		}
		case SII_MOD_SCDC_OPCODE__SCDC_SNK_CAP:
		{
			struct sii_lib_edid_hfvsdb_t *hfvsdb =
				(struct sii_lib_edid_hfvsdb_t *)in_data;
			struct sii_drv_tx_scdc_sink_caps sink_capability;

			sink_capability.b_lte340_mscs_scramble =
				hfvsdb->b_lte_340mscs_scramble;
			sink_capability.b_read_req_capable =
				hfvsdb->b_read_req_capable;
			sink_capability.b_scdc_present = hfvsdb->b_scdc_present;
			sink_capability.vclk_mb = hfvsdb->vclk_mb;

			s_scdc_sink_cap_set(p_obj, &sink_capability);
			break;
		}
		case SII_MOD_SCDC_OPCODE__SCDC_TMDS_CLK_SET:
		{
			uint32_t tmds_clk = *(uint32_t *)in_data;

			s_scdc_tmds_clk_enable(p_obj, tmds_clk);
			break;
		}
		case SII_MOD_SCDC_OPCODE__SCDC_SCRAMBLE_TIMEOUT_SET:
		{
			bool scramble_timeout = *(bool *)in_data;

			s_scdc_scramble_timeout_set(p_obj,
						    scramble_timeout);
			break;
		}
#ifdef CONFIG_ENHANCED_MODE_PROT
		case SII_MOD_SCDC_OPCODE__EMM_REQ_MODE:
		{
			p_obj->emm_req_mode = *(uint8_t *)in_data;
			break;
		}
		case SII_MOD_SCDC_OPCODE__TX_TMDS_MODE:
			p_obj->tx_tmds_mode = *(enum sii_tmds_mode *)in_data;
			break;
#endif
		default:
			ret_status = 1;
			break;
		}
		return ret_status;
	}
	return 1;
}

int sii_mod_tx_scdc_get(void *p_scdc_obj,
			enum sii_mod_scdc_internal_opcode opcode,
			void *out_data)
{
	int ret_status = 0;
	struct tx_scdc_obj *p_obj = (struct tx_scdc_obj *)p_scdc_obj;

	if (p_obj) {
		switch (opcode) {
		case SII_MOD_SCDC_OPCODE__PEER_MANF_STATUS:
			s_scdc_manufacturer_reg_read(p_obj);

			memcpy(out_data, &p_obj->peer_manf_status,
			sizeof(struct sii_drv_tx_scdc_manufacturer_status));

			break;
		case SII_MOD_SCDC_OPCODE__PEER_SCDC_STATUS:
			s_downstream_scdc_reg_read(p_obj);

			memcpy(out_data, &p_obj->scdcreg_status,
			sizeof(struct sii_drv_tx_scdc_register_status));

			break;
		case SII_MOD_SCDC_OPCODE__SOURCE_SCRAMBLE_STATUS:
			s_scdc_source_scrambling_status(p_obj);

			memcpy(out_data, &p_obj->source_scmble_status,
			sizeof(enum sii_drv_scdc_source_scrmble_status));

			break;
		case SII_MOD_SCDC_OPCODE__READ_UPDATE_REGISTER:
			s_scdc_read_update_registers(p_obj);

			memcpy(out_data, &p_obj->update_registers,
			sizeof(struct sii_drv_tx_scdc_status_update_regs));

			break;
#ifdef CONFIG_ENHANCED_MODE_PROT
		case SII_MOD_SCDC_OPCODE__DS_EM_CAPS:
			memcpy(out_data, &p_obj->ds_em_cap,
				sizeof(p_obj->ds_em_cap));
			break;
		case SII_MOD_SCDC_OPCODE__DS_EMM_RESP:
			memcpy(out_data, &p_obj->ds_emm_resp,
				sizeof(p_obj->ds_emm_resp));
			break;
#endif
		default:
			ret_status = 1;
			break;
		}
		return ret_status;
	}
	return 1;
}

static void s_scdc_reset(struct tx_scdc_obj *p_obj)
{
	memset(&p_obj->sink_caps, 0, sizeof(struct sii_drv_tx_scdc_sink_caps));
}

#define MAX_FIFO_SIZE 16
static enum ddc_com_err s_write_scdc_reg8(struct tx_scdc_obj *p_obj,
	uint8_t addr, uint8_t *val, uint8_t length)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	enum ddc_com_err ds_ddc_error = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint8_t val_t = 0;
	uint8_t time_out = 40;
	uint8_t retry = 2;

	if (length > MAX_FIFO_SIZE) {
		sii_log_err("length exceeded Max FIFO size\n");
		/* Todo: Implement multiple fifo_write */
		return SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
	}
	do { /*retry loop*/
		retry--;
		time_out = 40;
		do {
			val_t = (sii_platform_rd_reg8(pstDev,
				REG_ADDR__SCDC_INTR0) &
			BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2);

			if (0 == val_t) {
				break;
				;
			} else {
				/* clear DDC conflict interrupt*/
				sii_platform_wr_reg8(pstDev,
					REG_ADDR__SCDC_INTR0,
				BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2);
				/* clear SCDC DDC selection*/
				sii_platform_clr_bit8(pstDev,
					REG_ADDR__SCDC_CTL,
					BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);
				/* select SCDC DDC cycle*/
				sii_platform_set_bit8(pstDev,
					REG_ADDR__SCDC_CTL,
					BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);
				usleep(1000);
			}
		} while (--time_out && val_t);

		if (time_out <= 0)
			sii_log_debug("DDC busy: retry...\n");
		else
			break;

	} while (retry);

	if (time_out <= 0) {
		ds_ddc_error = SI_TX_DDC_ERROR_CODE_TIMEOUT;
		sii_log_debug("DDC busy: scdc reg read fail\n");
		sii_drv_tx_ddc_reset(&g_stdrv_obj, true);
		return ds_ddc_error;
	}

	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
		BIT_ENUM__DDC_CMD__CLEAR_FIFO);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_ADDR, 0xA8);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_SEGM, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_OFFSET, addr);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT1, length);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT2, 0);
	sii_platform_block_write8(pstDev, REG_ADDR__DDC_DATA, val, length);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
		BIT_ENUM__DDC_CMD__SEQUENTIAL_WRITE);

	/* disable DDC master*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
		BIT_ENUM__DDC_CMD__ABORT_TRANSACTION );
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
		BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);

	// sii_platform_clr_bit8(pdata, REG_ADDR__DDC_CMD, 0x7F);

	return ds_ddc_error;
}

static bool s_wait_for_ddc_bus(pstSii9630_Device_t pstDev)
{
	uint8_t val = 0;
	uint8_t time_out = 20;
	bool ret_val = false;

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

static enum ddc_com_err s_read_scdc_reg8(struct tx_scdc_obj *p_obj,
	uint8_t addr, uint8_t *reg_read, uint8_t length)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t val_t = 0;
	uint8_t time_out = 40;
	uint8_t retry = 2;
	enum ddc_com_err ds_ddc_error = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	uint16_t fifo_size = 0;

	if (!s_wait_for_ddc_bus(pstDev)) {
			ds_ddc_error = SI_TX_DDC_ERROR_CODE_BUSY;
			sii_log_debug("scdc DDC busy try later\n");
			sii_drv_tx_ddc_reset(&g_stdrv_obj, true);
			return ds_ddc_error;
		}

	do { /*retry loop*/
		retry--;
		time_out = 40;
		do {
			val_t = (sii_platform_rd_reg8(pstDev,
				REG_ADDR__SCDC_INTR0) &
				BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2);

			if (0 == val_t) {
				break;
			} else {
				/* clear DDC conflict interrupt*/
				sii_platform_wr_reg8(pstDev,
					REG_ADDR__SCDC_INTR0,
				BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2);
				/* clear SCDC DDC selection*/
				sii_platform_clr_bit8(pstDev, REG_ADDR__SCDC_CTL,
					BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);
				/* select SCDC DDC cycle*/
				sii_platform_set_bit8(pstDev,
					REG_ADDR__SCDC_CTL,
					BIT_MSK__SCDC_CTL__REG_SCDC_ACCESS);
				usleep(1000);
			}
		} while (--time_out && val_t);

		if (time_out <= 0)
			sii_log_debug("DDC busy: retry...\n");
		else
			break;

	} while (retry);

	if (time_out <= 0) {
		ds_ddc_error = SI_TX_DDC_ERROR_CODE_TIMEOUT;
		sii_log_debug("DDC busy: scdc reg read fail\n");
		sii_drv_tx_ddc_reset(&g_stdrv_obj, true);
		return ds_ddc_error;
	}

	sii_platform_set_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
		BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_ADDR, 0xA8);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_SEGM, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_OFFSET, addr);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT2, 0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_DIN_CNT1, length);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
		BIT_ENUM__DDC_CMD__CLEAR_FIFO );
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
		BIT_ENUM__DDC_CMD__SEQUENTIAL_READ );

	usleep(2000);

	fifo_size = sii_platform_rd_reg8(pstDev, REG_ADDR__DDC_DOUT_CNT)
			& BIT_MSK__DDC_DOUT_CNT__DDC_DATA_OUT_CNT;
	if (fifo_size) {
		if (fifo_size > length) {
			sii_log_debug("DDC error: FIFO size exceeded required length\n");
			ds_ddc_error = SI_TX_DDC_ERROR_CODE_LIM_EXCEED;
		} else {
			sii_platform_block_read8(pstDev,
				REG_ADDR__DDC_DATA, reg_read, fifo_size);
		}
	} else {
		sii_log_debug("FIFO is empty. Try again\n");
		ds_ddc_error = SI_TX_DDC_ERROR_CODE_FIFO_EMPTY;
	}

	/* disable DDC master*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DDC_CMD,
		BIT_ENUM__DDC_CMD__ABORT_TRANSACTION );
	sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_DDC_MASTER_EN,
		BIT_MSK__TPI_DDC_MASTER_EN__REG_HW_DDC_MASTER);

	//sii_platform_clr_bit8(pdata, REG_ADDR__DDC_CMD, 0x7F);
	return ds_ddc_error;
}

static void s_wait_for_scramble_status(struct tx_scdc_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t scramble_status = 0;
	uint8_t time_out = 3;


	do {
		s_read_scdc_reg8(p_obj,
			DDC_OFFSET__SCDC_TMDS_STATUS, &scramble_status, 1);
		if (scramble_status & BIT_MASK__TMDS_STATUS__SCRAMBLE_STATUS)
			break;
		msleep(100);
	} while (--time_out);

	if (scramble_status == BIT_MASK__TMDS_STATUS__SCRAMBLE_STATUS) {
		sii_log_debug("TMDS link is ok for SCRAMBLE data\n");

	} else {
		sii_log_debug("TMDS link is not functional.\n");
		if (p_obj->tmds_clock <= 340000000) {
			uint8_t scdc_tmds_cfg = 0;
			sii_log_debug("Disable SCRAMBLING\n");
			scdc_tmds_cfg = 0;
			s_write_scdc_reg8(p_obj,
				DDC_OFFSET__SCDC_TMDS_CONFIG,
				&scdc_tmds_cfg, 1);

			sii_platform_put_bit8(pstDev,
				REG_ADDR__TXC_DATA_DIV,
				BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV, 0x00);
			sii_platform_clr_bit8(pstDev, REG_ADDR__SCRCTL,
				BIT_MSK__SCRCTL__REG_SCR_ON);
		}
	}
}

static void s_scdc_scramble_ctrl(struct tx_scdc_obj *p_obj, bool enable)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t val = 0;
	if (enable) {
		sii_log_debug("Tx TMDS Clock: %d.%03d MHz\n",
			p_obj->tmds_clock/1000000,
			(p_obj->tmds_clock % 1000000)/1000);
		if (p_obj->tmds_clock > 340000000) {

			uint8_t status_scramble_clk = 0;

			s_read_scdc_reg8(p_obj,
					DDC_OFFSET__SCDC_TMDS_CONFIG,
					&status_scramble_clk, 1);
			sii_log_debug("status_scramble_clk before: %d\n",
				status_scramble_clk);
			/* Write into Sink's SCDCS if clk is>340 Mhz */
			val = BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE
				| BIT_MASK__TMDS_CONFIG__BIT_CLOCK_RATIO;
			s_write_scdc_reg8(p_obj,
				DDC_OFFSET__SCDC_TMDS_CONFIG, &val, 1);

			s_read_scdc_reg8(p_obj,
			DDC_OFFSET__SCDC_TMDS_CONFIG, &status_scramble_clk, 1);
			sii_log_debug("status_scramble_clk after: %d\n",
				status_scramble_clk);
			/* Set TX Clock devider value */
			/*	00:  TXC divide by 1 (default)
			 01:  TXC divide by 2
			 10:  TXC divide by 4
			 11:  TXC divide by 8*/
			sii_platform_put_bit8(pstDev, REG_ADDR__TXC_DATA_DIV,
				BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV, 0x02);

			/* Enable scrambling in TX and TMDS mode HDMI2 */
			sii_platform_set_bit8(pstDev, REG_ADDR__SCRCTL,
				BIT_MSK__SCRCTL__REG_SCR_ON |
					BIT_MSK__SCRCTL__REG_HDMI2_ON);

			sii_log_debug("SCRAMBLE ON\n");
			/* QD980B takes around 5 seconds to
			descramble,So this timer will not
			work for them */
			/* as per specset timer for 200 ms
			 * (will check scramble status when hit) */
			if (!p_obj->scramble_debug)
				s_wait_for_scramble_status(p_obj);
				/*sii_timer_start(&p_obj->timer_scramble_expire,
					200);*/
			else
				sii_log_debug("SCRAMBLE ON: No DS Check\n");


		} else {
			uint8_t bit_msk_scramble_en =
				BIT_MASK__TMDS_CONFIG__SCRAMBLE_ENABLE;
			uint8_t bit_msk_txc_div =
				BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV;

			if (p_obj->sink_caps.b_scdc_present) {

				if (p_obj->sink_caps.b_lte340_mscs_scramble) {

					/* Set Scrambling_Enable bit in
					 * Sink's SCDCS */
					/* Reset TMDS_Bit_Clock_Ratio bit in
					 * Sink's SCDCS */
					s_write_scdc_reg8(p_obj,
						DDC_OFFSET__SCDC_TMDS_CONFIG,
						&bit_msk_scramble_en, 1);

					/* Set TX Clock devider value */
					/*
					 00:  TXC divide by 1 (default)
					 01:  TXC divide by 2
					 10:  TXC divide by 4
					 11:  TXC divide by 8*/
					sii_platform_put_bit8(pstDev,
						REG_ADDR__TXC_DATA_DIV,
						bit_msk_txc_div,
						0x00);

					/*Enable scrambling in TX*/
					sii_platform_set_bit8(pstDev,
						REG_ADDR__SCRCTL,
						BIT_MSK__SCRCTL__REG_SCR_ON |
						BIT_MSK__SCRCTL__REG_HDMI2_ON);
					sii_log_debug("SCRAMBLE ON\n");
					/* QD980B takes around 5 seconds to
					descramble,So this timer will not
					work for them */
					/* as per spec:set timer for 200 ms
					 (will check scramble status when hit)*/
					if (!p_obj->scramble_debug)
						s_wait_for_scramble_status(
						p_obj);
						/*sii_timer_start(
						&p_obj->timer_scramble_expire,
						200);*/
					else
						sii_log_debug(
						"SCRAMBLE ON: No DS Check\n");

				} else {
					/* Reset Scrambling_Enable bit in
					 * Sink's SCDCS */
					/* Reset TMDS_Bit_Clock_Ratio bit in
					 * Sink's SCDCS */
					 val = 0;
					s_write_scdc_reg8(p_obj,
						DDC_OFFSET__SCDC_TMDS_CONFIG,
						&val, 1);

					/* Set TX Clock devider value */
					/*	00:  TXC divide by 1 (default)
						01:  TXC divide by 2
						10:  TXC divide by 4
						11:  TXC divide by 8*/
					sii_platform_put_bit8(pstDev,
					    REG_ADDR__TXC_DATA_DIV,
					    BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV,
					    0x00);

					/*Disable scrambling in TX */
					sii_platform_clr_bit8(pstDev,
						REG_ADDR__SCRCTL,
						BIT_MSK__SCRCTL__REG_SCR_ON);

					sii_log_debug("SCRAMBLE NONE\n");
				}
			} else {
				/* Set TX Clock devider value */
				/*	00:  TXC divide by 1 (default)
					01:  TXC divide by 2
					10:  TXC divide by 4
					11:  TXC divide by 8*/
				sii_platform_put_bit8(pstDev,
					REG_ADDR__TXC_DATA_DIV,
					BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV,
					0x00);

				/*Disable scrambling in TX */
				sii_platform_clr_bit8(pstDev, REG_ADDR__SCRCTL,
					BIT_MSK__SCRCTL__REG_SCR_ON);

				sii_log_debug("SCRAMBLE NONE\n");
			}
		}
	} else {
		/* Reset Scrambling_Enable bit in Sink's SCDCS */
		/* Reset TMDS_Bit_Clock_Ratio bit in Sink's SCDCS */
		if ((p_obj->sink_caps.b_scdc_present) ||
			(p_obj->tmds_clock > 340000000))
			val = 0;
			s_write_scdc_reg8(p_obj,
			DDC_OFFSET__SCDC_TMDS_CONFIG,
				&val, 1);

		/* Set TX Clock devider value */
		/*	00:  TXC divide by 1 (default)
			01:  TXC divide by 2
			10:  TXC divide by 4
			11:  TXC divide by 8*/
		sii_platform_put_bit8(pstDev, REG_ADDR__TXC_DATA_DIV,
			BIT_MSK__TXC_DATA_DIV__REG_TXC_DIV,
			0x00);

		/*Disable scrambling in TX */
		sii_platform_clr_bit8(pstDev, REG_ADDR__SCRCTL,
			BIT_MSK__SCRCTL__REG_SCR_ON);

		sii_log_debug("SCRAMBLE NONE\n");
	}
}

static void s_scramble_timeout(void *p_scdc_obj)
{
	struct tx_scdc_obj *p_obj = (struct tx_scdc_obj *)p_scdc_obj;
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t scramble_status = 0;

	if (1) {
		/*read scramble status at sink */
		s_read_scdc_reg8(p_obj,
			DDC_OFFSET__SCDC_TMDS_STATUS, &scramble_status, 1);
		if (scramble_status) {
			sii_log_debug(
				"TMDS link is ok for SCRAMBLE data\n");
		} else {
			sii_log_debug("TMDS link is not functional. ");
			sii_log_print("Disable SCRAMBLING\n");
			sii_platform_clr_bit8(pstDev, REG_ADDR__SCRCTL,
				BIT_MSK__SCRCTL__REG_SCR_ON);
		}
		//up(&p_obj->drv_context->isr_lock);
	}
}

static void s_scdc_read_request_test(struct tx_scdc_obj *p_obj,
					uint32_t ms_delay)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	uint8_t val = 0;
	/* clear slave read requrest interrupt*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__SCDC_INTR0,
		BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3);

	val = BIT_MASK__TEST_CONFIG_0__TEST_READ_REQUEST | (uint8_t)ms_delay;
	s_write_scdc_reg8(p_obj, DDC_OFFSET__SCDC_TEST_CONFIG_0, &val, 1);

	/* delay for 5 msec.*/
	msleep(ms_delay + 2);
}

static void s_scdc_source_version_set(struct tx_scdc_obj *p_obj, uint8_t ver)
{
	s_write_scdc_reg8(p_obj, DDC_OFFSET__SCDC_SOURCE_VERSION, &ver, 1);
}

static void s_scdc_read_request_enable(struct tx_scdc_obj *p_obj,
	uint8_t enable)
{

	uint8_t val = 0;

	val = enable ? 0x01 : 0x00;
	s_write_scdc_reg8(p_obj, DDC_OFFSET__SCDC_CONGIG_0, &val, 1);
}

static void s_scdc_tmds_clk_enable(struct tx_scdc_obj *p_obj, uint32_t tmds_clk)
{
	p_obj->tmds_clock = tmds_clk;
}

static void s_scdc_scramble_timeout_set(struct tx_scdc_obj *p_obj,
	bool scramble_timeout)
{
	p_obj->scramble_debug = scramble_timeout;
}

static void s_scdc_read_update_registers(struct tx_scdc_obj *p_obj)
{
	s_read_scdc_reg8(p_obj,
		DDC_OFFSET__SCDC_UPDATE_0,
		&p_obj->update_registers.scdc_update_0, 1);
	s_write_scdc_reg8(p_obj, DDC_OFFSET__SCDC_UPDATE_0,
		&p_obj->update_registers.scdc_update_0, 1);
	sii_log_debug("Reset update flag registers %02x\n",
		p_obj->update_registers.scdc_update_0);
}

static void s_scdc_source_scrambling_status(struct tx_scdc_obj *p_obj)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;

	if (sii_platform_rd_reg8(pstDev, REG_ADDR__SCRCTL) ==
	BIT_MSK__SCRCTL__REG_SCR_ON) {
		p_obj->source_scmble_status =
			SII_DRV_SCDC_SOURCE_SCRAMBLE_ON;
		sii_log_info("Scrambling in source: ON\n");
	} else {
		p_obj->source_scmble_status =
			SII_DRV_SCDC_SOURCE_SCRAMBLE_OFF;
		sii_log_info("Scrambling in source: OFF\n");
	}

}

static void s_scdc_sink_cap_set(struct tx_scdc_obj *p_obj,
				struct sii_drv_tx_scdc_sink_caps *sink_caps)
{
	p_obj->sink_caps.b_scdc_present = sink_caps->b_scdc_present;
	p_obj->sink_caps.b_read_req_capable = sink_caps->b_read_req_capable;
	p_obj->sink_caps.b_lte340_mscs_scramble =
		sink_caps->b_lte340_mscs_scramble;
	p_obj->sink_caps.vclk_mb = sink_caps->vclk_mb;

	if (p_obj->sink_caps.b_scdc_present) {
		s_scdc_source_version_set(p_obj, p_obj->src_version);
		;
	}

	if (p_obj->sink_caps.b_read_req_capable) {
		s_scdc_read_request_enable(p_obj, p_obj->b_read_req_cap);
		;
	}
}

bool sii_mod_tx_scdc_interrupt_handler(void *p_scdc_obj)
{
	struct tx_scdc_obj *p_obj = (struct tx_scdc_obj *)p_scdc_obj;
    pstSii9630_Device_t pstDev;// = p_obj->pstDevice;
	uint8_t scdc_int = 0;

	if (!p_scdc_obj)
		return FALSE;
    pstDev = p_obj->pstDevice;

	scdc_int = sii_platform_rd_reg8(pstDev, REG_ADDR__SCDC_INTR0);
	/*clear interrupt status*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__SCDC_INTR0, scdc_int);

	if (scdc_int & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT0)
		sii_log_debug("DDC read/write access done received\n");

	if (scdc_int & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT1)
		sii_log_debug("SCDC DDC read update flag  done received\n");

	if (scdc_int & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT2)
		sii_log_debug("DDC bus conflicting\n");

	if (scdc_int & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT3)
		sii_log_debug("SCDC slave read request interrupt received\n");

	if (scdc_int & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT4)
		sii_log_debug("DDC update flag change\n");

	if (scdc_int & BIT_MSK__SCDC_INTR0__REG_SCDC_INTR0_STAT5)
		sii_log_debug("SCDC DDC stall request acknowledged\n");

	return TRUE;
}

static void s_downstream_scdc_reg_read(struct tx_scdc_obj *p_obj)
{
	uint8_t reg_read = 0;
	uint8_t version_regs[2] = {0};
	uint8_t update_regs[2] = {0};
	uint8_t err_det_regs[7] = {0};

	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_SINK_VERSION, version_regs, 2);
	p_obj->scdcreg_status.sink_ver = version_regs[0];
	sii_log_debug("SCDC REG: 0x01 : DDC_OFFSET__SCDC_SINK_VERSION: %2x\n",
		version_regs[0]);

	p_obj->scdcreg_status.source_ver = version_regs[1];
	sii_log_debug("SCDC REG: 0x02 : DDC_OFFSET__SCDC_SOURCE_VERSION: %2x\n",
		version_regs[1]);

	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_UPDATE_0, update_regs, 2);
	p_obj->scdcreg_status.update_0 = update_regs[0];
	sii_log_debug("SCDC REG: 0x10 : UPDATE_0: %2x :: RR_TEST = %d\n",
		update_regs[0], (update_regs[0] & 0x04));
	sii_log_debug("ced_update = %d, status_update = %d\n",
		(update_regs[0] & 0x02), (update_regs[0] & 0x01));

	p_obj->scdcreg_status.update_1 = update_regs[1];
	sii_log_debug("SCDC REG: 0x11 : DDC_OFFSET__SCDC_UPDATE_1 : %2x\n",
		update_regs[1]);

	reg_read = 0;
	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_TMDS_CONFIG, &reg_read, 1);
	p_obj->scdcreg_status.tmds_config = reg_read;
	sii_log_debug("SCDC REG: 0x20 : TMDS_CONFIG : %2x\n", reg_read);
	sii_log_debug("tmds_bit_clk_rate = %d\n", (reg_read & 0x02));
	sii_log_debug("scramble__enable = %d\n", (reg_read & 0x01));

	reg_read = 0;
	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_TMDS_STATUS, &reg_read, 1);
	p_obj->scdcreg_status.tmds_status = reg_read;
	sii_log_debug("SCDC REG: 0x21 : SCRAMBLER_STATUS : %2x\n", reg_read);
	sii_log_debug("scrambler__status = %d\n", (reg_read & 0x01));

	reg_read = 0;
	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_CONGIG_0, &reg_read, 1);
	p_obj->scdcreg_status.config_0 = reg_read;
	sii_log_debug("SCDC REG: 0x30 : SCDC_CONGIG_0 : %2x\n", reg_read);
	sii_log_debug("rr__enable =  %d\n", (reg_read & 0x01));

	reg_read = 0;
	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_STATUS_FLAGS_0, &reg_read, 1);
	p_obj->scdcreg_status.status_flag_0 = reg_read;
	sii_log_debug("SCDC REG: 0x40 : STATUS_FLAGS_0 : %2x:: ch2__lock = %d\n",
		reg_read, (reg_read & 0x08));
	sii_log_debug("ch1__lock = %d, ch0__lock = %d, clock__det = %d\n",
		(reg_read & 0x04), (reg_read & 0x02), (reg_read & 0x01));

	reg_read = 0;
	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_TEST_CONFIG_0, &reg_read, 1);
	p_obj->scdcreg_status.test_config_0 = reg_read;
	sii_log_debug("SCDC REG: 0x40 : TEST_CONFIG_0 : %2x\n", reg_read);

	s_read_scdc_reg8(p_obj,
		DDC_OFFSET__SCDC_CHANNEL_0_ERROR_COUNT_L, err_det_regs, 7);

	sii_log_debug("SCDC REG: 0x50 : CHAN0_ERROR_COUNT_L : %2x\n",
		err_det_regs[0]);
	sii_log_debug("SCDC REG: 0x51 : CHAN0_ERROR_COUNT_H : %2x\n",
		err_det_regs[1]);
	p_obj->scdcreg_status.chnl0_error_cnt =
		(((uint16_t)err_det_regs[1] << 8) | err_det_regs[0]);

	sii_log_debug("SCDC REG: 0x52 : CHAN1_ERROR_COUNT_L : %2x\n",
		err_det_regs[2]);
	sii_log_debug("SCDC REG: 0x53 : CHAN1_ERROR_COUNT_H : %2x\n",
		err_det_regs[3]);
	p_obj->scdcreg_status.chnl1_error_cnt =
		(((uint16_t)err_det_regs[3] << 8) | err_det_regs[2]);

	sii_log_debug("SCDC REG: 0x54 : CHAN2_ERROR_COUNT_L : %2x\n",
		err_det_regs[4]);
	sii_log_debug("SCDC REG: 0x55 : CHAN2_ERROR_COUNT_H : %2x\n",
		err_det_regs[5]);
	p_obj->scdcreg_status.chnl2_error_cnt =
		(((uint16_t)err_det_regs[5] << 8) | err_det_regs[4]);
}

static void s_scdc_manufacturer_reg_read(struct tx_scdc_obj *p_obj)
{
	uint8_t man_spec_regs[16] = {0};

	s_read_scdc_reg8(p_obj, DDC_OFFSET__SCDC_MANF_OUI_1, man_spec_regs, 16);
	p_obj->peer_manf_status.oui_1 = man_spec_regs[0];
	p_obj->peer_manf_status.oui_2 = man_spec_regs[1];
	p_obj->peer_manf_status.oui_3 = man_spec_regs[2];

	memcpy(p_obj->peer_manf_status.dev_id_str, &man_spec_regs[3],
		sizeof(p_obj->peer_manf_status.dev_id_str));

	p_obj->peer_manf_status.dev_id_hw_rev = man_spec_regs[11];
	p_obj->peer_manf_status.dev_id_sw_major_rev = man_spec_regs[12];
	p_obj->peer_manf_status.dev_id_sw_minor_rev = man_spec_regs[13];

	sii_log_debug("SCDC REG: 0xD0 : DDC_OFFSET__SCDC_MANF_OUI_0 : %2x\n",
		man_spec_regs[0]);
	sii_log_debug("SCDC REG: 0xD1 : DDC_OFFSET__SCDC_MANF_OUI_2 : %2x\n",
		man_spec_regs[1]);
	sii_log_debug("SCDC REG: 0xD2 : DDC_OFFSET__SCDC_MANF_OUI_3 : %2x\n",
		man_spec_regs[2]);

	sii_log_debug("SCDC REG:0xD3-0xDA:DDC_OFFSET__SCDC_MANF_DEV_ID_STR: %c%c%c%c%c%c%c%c\n",
		man_spec_regs[3], man_spec_regs[4], man_spec_regs[5],
		man_spec_regs[6], man_spec_regs[7], man_spec_regs[8],
		man_spec_regs[9], man_spec_regs[10]);
	sii_log_print("\n");
	sii_log_debug("SCDC REG:0xDB:DDC_OFFSET__SCDC_MANF_DEV_ID_HW_REV:%2x\n",
		man_spec_regs[11]);

	sii_log_debug("SCDC REG:0xDC:SCDC_MANF_DEV_ID_SW_MAJOR_REV:%2x\n",
		man_spec_regs[12]);

	sii_log_debug("SCDC REG: 0xDD : SCDC_MANF_DEV_ID_SW_MINOR_REV : %2x\n",
		man_spec_regs[13]);
}

#ifdef CONFIG_ENHANCED_MODE_PROT
#ifndef REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
static uint8_t calculate_generic_checksum(uint8_t *data, uint8_t checksum,
	uint8_t length)
{
	uint8_t i;

	for (i = 0; i < length; i++)
		checksum += data[i];

	checksum = 0x100 - checksum;
	return checksum;
}

static int parse_em_packet(struct tx_scdc_obj *scdc_obj)
{
	int status = 0;
	uint32_t event = 0;
	struct em_packet *packet = &scdc_obj->em_packet;
	struct em_message *msg;
	uint16_t adopterid = 0;
	adopterid = (((uint16_t)packet->adopterid[0] << 8)
			| packet->adopterid[1]);
	if (LSCC_ADOPTER_ID != adopterid) {
		sii_log_err("Invalid EM Packet\n");
		return 1;
	}

	if (calculate_generic_checksum((uint8_t *)packet, 0, packet->length)) {
		sii_log_err("Invalid EM Packet Checksum\n");
		return 1;
	}

	msg = (struct em_message *)packet->payload;

	if (calculate_generic_checksum((uint8_t *)msg, 0, msg->msg_length)) {
		sii_log_err("Invalid EM message Checksum\n");
		return 1;
	}

	if (scdc_obj->expected_msg != msg->msg_id) {
		sii_log_err("Invalid EM Packet\n");
		return 1;
	}

	switch (msg->msg_id) {
	case EM_RPL_QUERY_CAP:
		scdc_obj->ds_em_cap = msg->payload[0];
		if (scdc_obj->ds_em_cap) {
			if (scdc_obj->ds_em_cap & EMC_HDMI2_DSC_4K50)
				sii_log_debug("DS RCV - DSC 4K50 is supported.\n");
			if (scdc_obj->ds_em_cap & EMC_HDMI2_DSC_4K60)
				sii_log_debug("DS RCV - DSC 4K60 is supported.\n");
			if (scdc_obj->ds_em_cap & EMC_HDMI2_DSC_4K30)
				sii_log_debug("DS RCV - DSC 4K30 is supported.\n");
			if (scdc_obj->ds_em_cap & EMC_HDMI2_DSC_4K25)
				sii_log_debug("DS RCV - DSC 4K25 is supported.\n");
			if (scdc_obj->ds_em_cap & EMC_HDMI2_DSC_4K24)
				sii_log_debug("DS RCV - DSC 4K24 is supported.\n");

			event = SII_DRV_TX_EVENT__SCDC_EM_CAP_CHG;
		} else {
			sii_log_debug("DS RCV - Do not support Enhanced Mode.\n");
		}
		break;
	case EM_RPL_REQ_MODE:
		/*if (scdc_obj->emm_req_mode != msg->payload[0]) {
			sii_log_err("Invalid EMM Response\n");
			return 1;
		}*/
		switch (msg->payload[0]) {
		case EMM_REQ_NACK:
			sii_log_err("Reply to EMM Req: The request cannot be recognized or supported.\n");
			break;
		case EMM_REQ_SUCCESS:
			sii_log_debug("Reply to EMM Req: The request has been accepted and executed successfully.\n");
			break;
		case EMM_REQ_FAILURE:
			sii_log_err("Reply to EMM Req: The request has been recognized, but the execution failed.\n");
			break;
		default:
			sii_log_debug("Invalid Reply to EMM Req\n");
			return 1;
		}
		scdc_obj->ds_emm_resp = msg->payload[0];
		event = SII_DRV_TX_EVENT__SCDC_EMM_RESP_CHG;
		break;
	case EM_RPL_SEND_PPS_FULL:
		switch (msg->payload[0]) {
		case PPS_SEND_NACK:
			break;
		case PPS_SEND_SUCCESS:
			break;
		case PPS_SEND_FAILURE:
			break;
		case PPS_SEND_ACK:
			break;
		case PPS_SEND_UNKNOWN:
			break;
		default:
			break;
		}
		break;
	case EM_RPL_SEND_PPS_PART:
		break;
	default:
		sii_log_err("Invalid Message ID\n");
		return 1;
	}

	if (scdc_obj->event_notify_fn)
		scdc_obj->event_notify_fn(&g_stdrv_obj,
			event);

	return status;
}
#endif // REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
static void poll_scdc_rcv_status(void *obj)
{
	struct tx_scdc_obj *scdc_obj = (struct tx_scdc_obj *)obj;
#ifdef REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
	uint32_t events = 0;
	switch (scdc_obj->expected_msg) {
	case EM_RPL_QUERY_CAP:
		scdc_obj->ds_em_cap = EMC_HDMI2_DSC_4K50 | EMC_HDMI2_DSC_4K60;
		events = SII_DRV_TX_EVENT__SCDC_EM_CAP_CHG;
		break;
	case EM_RPL_REQ_MODE:
		scdc_obj->ds_emm_resp = EMM_REQ_SUCCESS;
		events = SII_DRV_TX_EVENT__SCDC_EMM_RESP_CHG;
		break;
	}

	if (scdc_obj->event_notify_fn)
		scdc_obj->event_notify_fn(&g_stdrv_obj,
			events);
#else
	enum ddc_com_err ds_ddc_error = SI_TX_DDC_ERROR_CODE_NO_ERROR;
	int status = 0;

	if (scdc_obj->tx_tmds_mode == SII_TMDS_MODE__NONE)
		return;

	memset(&scdc_obj->em_packet, 0, sizeof(struct em_packet));
	sii_log_debug("Read EM Packet\n");
	/*Read EM Packet*/
	ds_ddc_error = s_read_scdc_reg8(scdc_obj,
			DDC_OFFSET_SCDC_RCV_BUF,
			(uint8_t *)&scdc_obj->em_packet,
			sizeof(struct em_packet));
	if (ds_ddc_error) {
		sii_log_err("failed to read SCDC RCV Buffer: %d\n",
			ds_ddc_error);
		/*return;*/
	}

	sii_log_debug("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n",
	scdc_obj->em_packet.adopterid[0],
	scdc_obj->em_packet.adopterid[1],
	scdc_obj->em_packet.seq_num_frag_cnt,
	scdc_obj->em_packet.length,
	scdc_obj->em_packet.checksum,
	scdc_obj->em_packet.payload[0],
	scdc_obj->em_packet.payload[1],
	scdc_obj->em_packet.payload[2],
	scdc_obj->em_packet.payload[3]);

	status = parse_em_packet(scdc_obj);
	if (status && --scdc_obj->retry) {
		sii_log_debug("Polling SCDC RCV Buffer again.\n");
		sii_timer_start(&scdc_obj->rcv_stat_timer, 100);
	}

	if (0 == scdc_obj->retry) {
		sii_log_err("SCDC Polling: Retry limit exceeded.\n");
		if (scdc_obj->event_notify_fn)
			scdc_obj->event_notify_fn(
				&g_stdrv_obj,
				SII_DRV_TX_EVENT__SCDC_EM_NO_RESP);
	}
#endif
}

#ifdef CONFIG_UNDEFINED_MACRO
static int trasmit_em_packet(struct tx_scdc_obj *scdc_obj,
	struct em_message *msg)
{
	struct em_packet l_stpacket;
	struct em_packet *packet = &l_stpacket;
	uint8_t frag_cnt = 0;
	uint8_t seq_num = 0;
	int msg_length = 0;
	uint8_t payload_length = 0;
	int i = 0;
	int status = 0;
	uint8_t val = 0;
	int timeout = 0;
	enum ddc_com_err ds_ddc_error = SI_TX_DDC_ERROR_CODE_NO_ERROR;

	msg_length = msg->msg_length;
	if (msg_length > MAX_PAYLOAD_LENGTH) {
		if (msg_length % MAX_PAYLOAD_LENGTH)
			frag_cnt = msg_length / MAX_PAYLOAD_LENGTH + 1;
		else
			frag_cnt = msg_length / MAX_PAYLOAD_LENGTH;
	} else {
		frag_cnt = 1;
		seq_num = 0;
	}

	if (frag_cnt > 0x0F) {
		sii_log_err("Max Fragments Exceeded: %d\n", frag_cnt);
		return 1;
	}

	//packet = kzalloc(sizeof(struct em_packet) * frag_cnt, GFP_KERNEL);
    memset(l_stpacket,0,sizeof(struct em_packet));

	for (i = 0; i < frag_cnt; i++) {
		packet[i].adopterid = LSCC_ADOPTER_ID;
		seq_num = i;
		packet[i].seq_num_frag_cnt = (i << 4) | (frag_cnt & 0x0F);
		packet[i].checksum = 0;
		payload_length = (msg_length > MAX_PAYLOAD_LENGTH ?
					MAX_PAYLOAD_LENGTH : msg_length);
		packet[i].length = MIN_PACKET_LENGTH + payload_length;
		memcpy(&packet[i].payload[0], &msg[i * MAX_PAYLOAD_LENGTH],
			payload_length);
		msg_length -= MAX_PAYLOAD_LENGTH;
		packet[i].checksum =
			calculate_generic_checksum((uint8_t *)&packet[i], 0,
			packet[i].length);

		/*set req_wrt*/
		val = HDMI_SRC_STAT_REQ_WRT;
		sii_log_debug("Send REQ_WRT\n");
		ds_ddc_error = s_write_scdc_reg8(scdc_obj,
			DDC_OFFSET_SCDC_SRC_STAT, &val, 1);
		val = 0;
		s_read_scdc_reg8(scdc_obj, DDC_OFFSET_SCDC_SRC_STAT, &val, 1);
		sii_log_debug("HDMI_SRC_STAT: 0x%02X\n", val);
		/*poll grt_wrt*/
		timeout = 10;
		do {
			msleep(100);
			val = 0;
			sii_log_debug("Poll GRT_WRT\n");
			ds_ddc_error = s_read_scdc_reg8(scdc_obj,
				DDC_OFFSET_SCDC_RCV_STAT, &val, 1);
			if (ds_ddc_error) {
				sii_log_debug("failed to read SCDC_RCV status\n");
				status = -11;
				break;
			}

			if (val == HDMI_RCV_STAT_GRT_WRT)
				break;
			else
				sii_log_debug("GRT_WRT not received: 0x%02X\n",
					val);

		} while (timeout--);

		/*send packet[i] and set_int*/
		if (val == HDMI_RCV_STAT_GRT_WRT) {
			sii_log_debug("Fill EM Packet\n");
			ds_ddc_error = s_write_scdc_reg8(scdc_obj,
				DDC_OFFSET_SCDC_SRC_BUF,
				(uint8_t *)&packet[i], packet[i].length);
			if (ds_ddc_error) {
				sii_log_debug("failed to write EM Packet\n");
				status = -11;
				break;
			}
			/* trigger dscr_chg int*/
			val = HDMI_SRC_STAT_DSCR_CHG;
			sii_log_debug("Send DSCR_CHG\n");
			ds_ddc_error = s_write_scdc_reg8(scdc_obj,
				DDC_OFFSET_SCDC_SRC_STAT, &val, 1);

			/* clear SCDC_SRC_STAT */
			msleep(100);
			val = 0;
			sii_log_debug("Clear HDMI_SRC_STAT\n");
			ds_ddc_error = s_write_scdc_reg8(scdc_obj,
				DDC_OFFSET_SCDC_SRC_STAT, &val, 1);
		}
	}
	//kfree(packet);
	return status;
}

#define PPS_LENGTH	90
#define MSG_MIN_LENGTH	3

int sii_mod_tx_scdc_request_em_caps(void *obj)
{
	struct tx_scdc_obj *scdc_obj = (struct tx_scdc_obj *)obj;
	/*step1: Construct "Query EMC" message and packet*/
	struct em_message msg = {0};
	int status = 0;

	msg.msg_id = EM_CMD_QUERY_CAP;
	msg.msg_length = MSG_MIN_LENGTH;
	msg.msg_chksm = 0;
	msg.msg_chksm = calculate_generic_checksum((uint8_t *)&msg,
			0, msg.msg_length);

	status = trasmit_em_packet(scdc_obj, &msg);

	sii_timer_start(&scdc_obj->rcv_stat_timer, 10);

	return status;
}

static int xmit_emm_request(void *obj, uint8_t req_mode)
{
	struct tx_scdc_obj *scdc_obj = (struct tx_scdc_obj *)obj;
	/*step1: Construct "Query EMC" message and packet*/
	struct em_message msg = {0};
	int status = 0;

	msg.msg_id = EM_CMD_REQ_MODE;
	msg.msg_length = MSG_MIN_LENGTH + 1;
	msg.msg_chksm = 0;
	memcpy(msg.payload, &req_mode, 1);
	msg.msg_chksm = calculate_generic_checksum((uint8_t *)&msg,
			0, msg.msg_length);

	status = trasmit_em_packet(scdc_obj, &msg);

	sii_timer_start(&scdc_obj->rcv_stat_timer, 10);

	return status;
}

int sii_mod_tx_scdc_send_pps_settings(void *obj, uint8_t *pps)
{
	struct tx_scdc_obj *scdc_obj = (struct tx_scdc_obj *)obj;
	struct em_message msg = {0};
	int status = 0;

	msg.msg_id = EM_CMD_SEND_PPS_FULL;
	msg.msg_length = MSG_MIN_LENGTH + PPS_LENGTH;
	msg.msg_chksm = 0;
	memcpy(msg.payload, pps, PPS_LENGTH);

	msg.msg_chksm = calculate_generic_checksum((uint8_t *)&msg,
			0, msg.msg_length);

	status = trasmit_em_packet(scdc_obj, &msg);

	sii_timer_start(&scdc_obj->rcv_stat_timer, 10);

	return status;
}
#endif // CONFIG_UNDEFINED_MACRO

void sii_mod_tx_scdc_poll_em_buf(void *obj, uint8_t expected_msg)
{
	struct tx_scdc_obj *scdc_obj = (struct tx_scdc_obj *)obj;

	scdc_obj->expected_msg = expected_msg;
#ifdef REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL

#ifdef	AVIF_INSTEAD_OF_VSIF_FOR_EMM
	if (scdc_obj)
		sii_timer_start(&scdc_obj->rcv_stat_timer, 0);
#else
    /*100ms timeout*/
    if (scdc_obj)
        sii_timer_start(&scdc_obj->rcv_stat_timer, 100);
#endif

#else
	scdc_obj->retry = 17;
	if (scdc_obj)
		sii_timer_start(&scdc_obj->rcv_stat_timer, 80);
#endif // REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
}
#endif // CONFIG_ENHANCED_MODE_PROT

void mapp_Sil9630TxScdcHandler(void *p_obj)
{
	struct hdmi_tx_obj *p_hdmi_obj = (struct hdmi_tx_obj *)p_obj;
	struct tx_scdc_obj *scdc_obj = (struct tx_scdc_obj *)(p_hdmi_obj->tx_scdc_obj);
    s_scramble_timeout((void *)scdc_obj);
    poll_scdc_rcv_status((void *)scdc_obj);
}
#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

