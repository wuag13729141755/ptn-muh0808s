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

#ifndef _SI_MOD_TX_HDMI_H_
#define _SI_MOD_TX_HDMI_H_

#include "Sii9630_si_drv_cp8630.h"

/* ! opcodes for internal module variables.*/
enum sii_mod_hdmi_tx_internal_opcode {
	SII_MOD_HDMI_TX_OPCODE__HOTPLUG,
	SII_MOD_HDMI_TX_OPCODE__RSEN,
	SII_MOD_HDMI_TX_OPCODE__EDID,
	SII_MOD_HDMI_TX_OPCODE__HDMI_STATE,
	SII_MOD_HDMI_TX_OPCODE__TMDS_MODE,
	SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
	SII_MOD_HDMI_TX_OPCODE__AVMUTE,
	SII_MOD_HDMI_TX_OPCODE__IF_DATA,
	SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH,
	SII_MOD_HDMI_TX_OPCODE__CEC_PHY_ADDR,
	SII_MOD_HDMI_TX_OPCODE__EDID_LIPSYNC,
	SII_MOD_HDMI_TX_OPCODE__HW_UPDATE_START,
	SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_MANF_STATUS,
	SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_REG_STATUS,
	SII_MOD_HDMI_TX_OPCODE__SCDC_SOURCE_SCRAMBLE_STATUS,
	SII_MOD_HDMI_TX_OPCODE__SCDC_ENABLE_READ_REQ_TEST,
	SII_MOD_HDMI_TX_OPCODE__SCDC_READ_REQ_CAP_SET,
	SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
	SII_MOD_HDMI_TX_OPCODE__SCDC_TMDS_CLOCK_SET,
	SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_TIMEOUT_SET,
	SII_MOD_HDMI_TX_OPCODE__DDC_RESET,
	SII_MOD_HDMI_TX_OPCODE__READ_UPDATE_REGISTER,
	SII_MOD_HDMI_TX_OPCODE__EDID_PARSE,
	SII_MOD_HDMI_TX_OPCODE__SCDC_CONFIG,
	SII_MOD_HDMI_TX_OPCODE__DSC_COMPRESSION_STATUS,
	SII_MOD_HDMI_TX_OPCODE__DSC_COMPRESSION_RATIO,
#ifdef CONFIG_ENHANCED_MODE_PROT
	SII_MOD_HDMI_TX_OPCODE__DS_EM_CAPS,
	SII_MOD_HDMI_TX_OPCODE__SCDC_XMIT_EMM,
	SII_MOD_HDMI_TX_OPCODE__DS_EMM_RESP,
#endif
};

/**
 * @brief tx states
 */
enum sii_mod_hdmi_tx_internal_state {
	SII_MOD_HDMI_TX_EVENT__TMDS_OFF,
	SII_MOD_HDMI_TX_EVENT__TMDS_ON,
	SII_MOD_HDMI_TX_EVENT__UPDATE_TMDS,
};

#if 0
/*! DDC bus access error codes*/
enum ddc_com_err {
	SI_TX_DDC_ERROR_CODE_NO_ERROR = 0x00, /*!< success */
	SI_TX_DDC_ERROR_CODE_TIMEOUT = 0x01,
	/*!< DDC bus is not granted within timeout */
	SI_TX_DDC_ERROR_CODE_NO_ACK = 0x02, /*!< no ACK from DDC device */
	SI_TX_DDC_ERROR_CODE_BUSY = 0x03, /*!< DDC bus is busy */
	SI_TX_DDC_ERROR_CODE_TX_HW = 0x04,
	SI_TX_DDC_ERROR_CODE_LIM_EXCEED = 0x05,
    SI_TX_DDC_ERROR_CODE_FIFO_EMPTY = 0x06,
};
#endif

struct sii_hdmi_int_stat {
	uint8_t reg0;
	uint8_t reg1;
};

/* hdmi tx driver data*/
struct hdmi_tx_obj {
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;

	enum sii_tmds_mode tmds_mode;
	enum sii_tmds_mode prev_tmds_mode;
	enum sii_mod_hdmi_tx_internal_state prev_i_state;
	enum sii_hv_sync_pol hv_sync_pol;
	uint8_t b_is_scdc;
	bool b_av_mute;
	uint8_t b_is_ds_edid_available;
	struct sii_info_frame if_data[SII_INFO_FRAME_ID__TOTAL];
	enum sii_drv_bit_depth input_bit_depth;
	enum sii_drv_bit_depth output_bit_depth;
	enum sii_drv_clr_spc input_clr_spc;
	enum sii_drv_clr_spc output_clr_spc;
	struct sii_edid edid;
	bool b_hot_plug;
	bool b_rsen;
	uint8_t b_init_hot_plug;
	void *timer_hw_update;
	void *tx_scdc_obj;
	struct sii_hdmi_int_stat int_stat;
	enum sii_mod_hdmi_tx_internal_state i_state;
	enum sii_drv_hdmi_tx_external_state e_state;
	struct sii_lib_edid_parsed_data parse_edid;
	bool b_tmds_on_req;

	void (*event_notify_fn)(void *, uint32_t);
};

/* public functions */
void *sii_mod_tx_hdmi_init(pstSii9630_Device_t pstDevice,
		void (*event_notify_fn)(void *, uint32_t));

void sii_mod_tx_hdmi_exit(void *tx_hdmi_obj);

bool sii_mod_tx_hdmi_interrupt_handler(void *tx_hdmi_obj);

int sii_mod_tx_hdmi_set(void *tx_hdmi_obj,
	enum sii_mod_hdmi_tx_internal_opcode opcode, const void *in_data);

int sii_mod_tx_hdmi_get(void *tx_hdmi_obj,
	enum sii_mod_hdmi_tx_internal_opcode opcode, void *out_data);

int init_hdmi_tx_regs(void *tx_hdmi_obj);

int sii_mod_tx_hdmi_stop_timers(void *tx_hdmi_obj);

#ifdef CONFIG_ENHANCED_MODE_PROT
#define DDC_OFFSET_SCDC_RCV_BUF		0xF0
#define MAX_SCDC_BUF_SIZE		0x0F

#define LSCC_ADOPTER_ID		322

/* The request cannot be recognized or supported. */
#define EMM_REQ_NACK		0x00
/* The request has been accepted and executed successfully. */
#define EMM_REQ_SUCCESS		0x01
/* The request has been recognized, but the execution failed. */
#define EMM_REQ_FAILURE		0x02

#define MIN_PACKET_LENGTH	5
#define MAX_PAYLOAD_LENGTH	11

enum pps_status {
	PPS_SEND_NACK,
	PPS_SEND_SUCCESS,
	PPS_SEND_FAILURE,
	PPS_SEND_ACK,
	PPS_SEND_UNKNOWN,
};

enum hdmi2_enhanced_mode_command {
	EM_CMD_QUERY_CAP = 0x00,
	EM_RPL_QUERY_CAP = 0x01,
	EM_CMD_REQ_MODE = 0x02,
	EM_RPL_REQ_MODE = 0x03,
	EM_CMD_SEND_PPS_FULL = 0x04,
	EM_RPL_SEND_PPS_FULL = 0x05,
	EM_CMD_SEND_PPS_PART = 0x06,
	EM_RPL_SEND_PPS_PART = 0x07
};

struct em_message {
	uint8_t msg_id;
	uint8_t msg_length;
	uint8_t msg_chksm;
	uint8_t payload[128];
};

struct em_packet {
	uint8_t adopterid[2];
	uint8_t seq_num_frag_cnt;
	uint8_t length;
	uint8_t checksum;
	uint8_t payload[11];
};

int sii_mod_tx_hdmi_request_em_caps(void *tx_hdmi_obj);
int sii_mod_tx_hdmi_request_emm(void *tx_hdmi_obj, uint8_t req_mode);
#endif
#endif /* _SI_MOD_TX_HDMI_H_*/
