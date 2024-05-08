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

#ifndef _SI_MOD_TX_HDCP_H_
#define _SI_MOD_TX_HDCP_H_

#include "Sii9630_si_drv_cp8630.h"

/* ! opcodes for internal module variables. */
enum sii_mod_hdcp_internal_opcode {
	SII_MOD_HDCP_OPCODE__ENABLE,
	SII_MOD_HDCP_OPCODE__STATUS,
	SII_MOD_HDCP_OPCODE__VERSION,
	SII_MOD_HDCP_OPCODE__BKSV_LIST,
	SII_MOD_HDCP_OPCODE__TOPOLOGY,
	SII_MOD_HDCP_OPCODE__CONTENT_TYPE,
	SII_MOD_HDCP_OPCODE__BKSV_LIST_APPROVAL,
	SII_MOD_HDCP_OPCODE__RCVID_SET,
	SII_MOD_HDCP_OPCODE__RCVID_GET,
	SII_MOD_HDCP_OPCODE__RPT_RCVID_GET,
	SII_MOD_HDCP_OPCODE__STMNG_SET,
	SII_MOD_HDCP_OPCODE__UPDATE_HPD_STATUS,
	SII_MOD_HDCP_OPCODE__OTP_MUTE_ENABLE,
	SII_MOD_HDCP_OPCODE__DS_AUTH_TYPE,
	SII_MOD_HDCP_OPCODE__POWER_UP_STATUS,
	SII_MOD_HDCP_OPCODE__RPTR_MODE,
	SII_MOD_HDCP_OPCODE__FAIL_REASON,
};

/* public functions */
void *sii_mod_tx_hdcp_init(pstSii9630_Device_t pstDevice,
		void (*event_notify_fn)(void *, uint32_t));

void sii_mod_tx_hdcp_exit(void *hdcp_obj);

bool sii_mod_tx_hdcp_interrupt_handler(void *hdcp_obj);

int sii_mod_tx_hdcp_set(void *hdcp_obj,
	enum sii_mod_hdcp_internal_opcode opcode, const void *in_data);

int sii_mod_tx_hdcp_get(void *hdcp_obj,
	enum sii_mod_hdcp_internal_opcode opcode, void *out_data);

int init_hdcp_tx_regs(void *hdcp_obj);

int sii_mod_tx_hdcp_stop_timers(void *hdcp_obj);

void mapp_Sil9630TxHdcpHandler(void *p_obj);

#endif /* _SI_MOD_TX_HDCP_H_*/
