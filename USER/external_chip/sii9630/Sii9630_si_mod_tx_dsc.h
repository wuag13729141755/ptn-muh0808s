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

#ifndef __SI_MOD_TX_DSC_H__
#define __SI_MOD_TX_DSC_H__

#include "Sii9630_si_drv_cp8630.h"

enum sii_mod_dsc_op_code {
	SII_MOD_DSC_OPCODE__WRAP_CONFIGURE,
	SII_MOD_DSC_OPCODE__SET_DSC_EN,
	SII_MOD_DSC_OPCODE__INFOFRAME_ID,
	SII_MOD_DSC_OPCODE__INFOFRAME_DATA,
	SII_MOD_DSC_OPCODE__INFOFRAME_UPDATE_VSIF,
	SII_MOD_DSC_OPCODE__INFOFRAME_AVIF,
	SII_MOD_DSC_OPCODE__UPDATE_PIXEL_CLOCK,
	SII_MOD_DSC_OPCODE__COMPRESSED_PIXEL_CLOCK,
	SII_MOD_DSC_OPCODE__COMPRESSED_STATUS,
};

/* public functions */
void *sii_mod_tx_dsc_init(pstSii9630_Device_t pstDevice,
	void (*event_notify_fn)(void *, uint32_t));

void sii_mod_dsc_delete(void *dsc_obj);

int sii_mod_dsc_set(void *dsc_obj,
	enum sii_mod_dsc_op_code op_code, void *in_data);

int sii_mod_dsc_get(void *dsc_obj,
	enum sii_mod_dsc_op_code op_code, void *out_data);

int sii_mod_dsc_stop_timers(void *dsc_obj);
#endif /* __SI_MOD_TX_DSC_H__ */
