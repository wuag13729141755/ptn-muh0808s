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

#ifndef __SI_DRV_TX_SCDC_API_H__
#define __SI_DRV_TX_SCDC_API_H__

#include "Sii9630_si_drv_cp8630.h"

enum sii_mod_scdc_internal_opcode {
	SII_MOD_SCDC_OPCODE__PEER_MANF_STATUS,
	SII_MOD_SCDC_OPCODE__PEER_SCDC_STATUS,
	SII_MOD_SCDC_OPCODE__SOURCE_SCRAMBLE_STATUS,
	SII_MOD_SCDC_OPCODE__READ_UPDATE_REGISTER,
	SII_MOD_SCDC_OPCODE__READ_REQ_CAP_SET,
	SII_MOD_SCDC_OPCODE__ENABLE_READ_REQ_TEST,
	SII_MOD_SCDC_OPCODE__SCDC_SCRAMBLE_EN,
	SII_MOD_SCDC_OPCODE__SCDC_RESET_CAP,
	SII_MOD_SCDC_OPCODE__SCDC_SNK_CAP,
	SII_MOD_SCDC_OPCODE__SCDC_TMDS_CLK_SET,
	SII_MOD_SCDC_OPCODE__SCDC_SCRAMBLE_TIMEOUT_SET,
#ifdef CONFIG_ENHANCED_MODE_PROT
	SII_MOD_SCDC_OPCODE__DS_EM_CAPS,
	SII_MOD_SCDC_OPCODE__SCDC_XMIT_EMM,
	SII_MOD_SCDC_OPCODE__EMM_REQ_MODE,
	SII_MOD_SCDC_OPCODE__DS_EMM_RESP,
	SII_MOD_SCDC_OPCODE__TX_TMDS_MODE,

#endif
};


/* public functions */
void *sii_mod_tx_scdc_init(pstSii9630_Device_t pstDevice,
		void (*event_notify_fn)(void *, uint32_t));

void sii_mod_tx_scdc_exit(void *scdc_obj);

int sii_mod_tx_scdc_get(void *scdc_obj,
	enum sii_mod_scdc_internal_opcode opcode, void *out_data);

int sii_mod_tx_scdc_set(void *scdc_obj,
	enum sii_mod_scdc_internal_opcode opcode, const void *indata);

bool sii_mod_tx_scdc_interrupt_handler(void *scdc_obj);

int init_scdc_tx_regs(void *scdc_obj);

int sii_mod_tx_scdc_stop_timers(void *scdc_obj);

#ifdef CONFIG_ENHANCED_MODE_PROT
void sii_mod_tx_scdc_poll_em_buf(void *scdc_obj, uint8_t expected_msg);
#endif
void mapp_Sil9630TxScdcHandler(void *p_obj);

#endif /* __SI_DRV_TX_SCDC_API_H__*/
