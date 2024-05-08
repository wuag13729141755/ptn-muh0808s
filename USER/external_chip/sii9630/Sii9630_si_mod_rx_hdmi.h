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

#ifndef _SI_MOD_RX_HDMI_H_
#define _SI_MOD_RX_HDMI_H_


#include "Sii9630_si_drv_cp8630.h"

enum sii_mod_rx_hdmi_opcode {
	SII_MOD_RX_HDMI_OPCODE__RPWR,
	SII_MOD_RX_HDMI_OPCODE__SCDT,
	SII_MOD_RX_HDMI_OPCODE__CKDT,
	SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
	SII_MOD_RX_HDMI_OPCODE__HDCP_CONTENT_TYPE,
	SII_MOD_RX_HDMI_OPCODE__HDCP_PROTECTION,
	SII_MOD_RX_HDMI_OPCODE__HDCP_STAT,
	SII_MOD_RX_HDMI_OPCODE__LINK_VERSION,
	SII_MOD_RX_HDMI_OPCODE__AVMUTE,
	SII_MOD_RX_HDMI_OPCODE__TMDS_MODE,
	SII_MOD_RX_HDMI_OPCODE__IF_DATA,
	SII_MOD_RX_HDMI_OPCODE__IF_CLEAR,
	SII_MOD_RX_HDMI_OPCODE__EDID,
	SII_MOD_RX_HDMI_OPCODE__HPD,
	SII_MOD_RX_HDMI_OPCODE__CKDT_STATUS,
	SII_MOD_RX_HDMI_OPCODE__BKSV_SET,
	SII_MOD_RX_HDMI_OPCODE__REPEATER_SET,
	SII_MOD_RX_HDMI_OPCODE__HDCP_RCVID_SET,
	SII_MOD_RX_HDMI_OPCODE__HDCP_RPT_RCVID_SET,
	SII_MOD_RX_HDMI_OPCODE__STMNG,
	SII_MOD_RX_HDMI_OPCODE__SCDC_EVENTS,
	SII_MOD_RX_HDMI_OPCODE__HDCP_RESET_FLAGS,
	SII_MOD_RX_HDMI_OPCODE__HDCP2X_RPT_STATUS,
	SII_MOD_RX_HDMI_OPCODE__INPUT_420_EN,
	SII_MOD_RX_HDMI_OPCODE__SCDC_SINKVERSION,
	SII_MOD_RX_HDMI_OPCODE__SCDC_SRCVERSION,
	SII_MOD_RX_HDMI_OPCODE__RX_PLL_LOCK,
	SII_MOD_RX_HDMI_OPCODE__SCDCS_CLK_RATIO,
	SII_MOD_RX_HDMI_OPCODE__HPD_TOGGLE
#ifdef RX_DESCRAMBLE_THROUGH_API
	,SII_MOD_RX_HDMI_OPCODE__RX_SCDC_CLK_RATIO
	,SII_MOD_RX_HDMI_OPCODE__RX_SCDC_SCRAMBLE_STAT
#endif
};

/* public functions */
void *sii_mod_rx_hdmi_init(pstSii9630_Device_t pstDevice,
	void (*event_notify_fn)(void *, uint32_t));

void sii_mod_rx_hdmi_exit(void *rx_hdmi_obj);

bool sii_mod_rx_hdmi_interrupt_handler(void *rx_hdmi_obj);

int sii_mod_rx_hdmi_set(void *rx_hdmi_obj,
	enum sii_mod_rx_hdmi_opcode op_code, void *in_data);

int sii_mod_rx_hdmi_get(void *rx_hdmi_obj,
	enum sii_mod_rx_hdmi_opcode op_code, void *out_data);

int init_hdmi_rx_regs(void *rx_hdmi_obj);

int sii_mod_rx_hdmi_stop_timers(void *rx_hdmi_obj);

void mapp_sil9630RxHdmiHandler(void *p_obj);

#endif /*_SI_MOD_RX_HDMI_H_*/
