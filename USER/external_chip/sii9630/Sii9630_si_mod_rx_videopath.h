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

#ifndef __SI_MOD_RX_VIDEOPATH_H_
#define __SI_MOD_RX_VIDEOPATH_H_

#include "Sii9630_si_drv_cp8630.h"

enum sii_mod_vid_path_opcode {
	SII_MOD_VIDPATH_OPCODE__IN_CLRSPC,
	SII_MOD_VIDPATH_OPCODE__OUT_CLRSPC,
	SII_MOD_VIDPATH_OPCODE__OUT_CONVSTD,
	SII_MOD_VIDPATH_OPCODE__OUT_CLRDEPTH,
	SII_MOD_VIDPATH_OPCODE__IN_VIDINFO,
	SII_MOD_VIDPATH_OPCODE__HVSYNC,
	SII_MOD_VIDPATH_OPCODE__OUT_HVSYNC_POLARITY,
	SII_MOD_VIDPATH_OPCODE__COLORCONVEN,
	SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS,
	SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS_UPDATE,
	SII_MOD_VIDPATH_OPCODE__VTG_ENABLE,
	SII_MOD_VIDPATH_OPCODE__ENABLE_REF_CLOCK,
	SII_MOD_VIDPATH_OPCODE__PIN_MAPPING_CONFIG,
};

/* public functions */
void *sii_mod_vid_path_init(pstSii9630_Device_t pstDevice,
	void (*event_notify_fn)(void *, uint32_t));

void sii_mod_vid_path_exit(void *vidpath_obj);

int sii_mod_vid_path_set(void *vidpath_obj,
	enum sii_mod_vid_path_opcode op_code, void *in_data);

int sii_mod_vid_path_get(void *vidpath_obj,
	enum sii_mod_vid_path_opcode op_code, void *out_data);

int sii_mod_vid_path_intr_handler(void *vidpath_obj, bool rx_pll_status);

int init_videopath_regs(void *vidpath_obj);

int sii_mod_vid_path_stop_timers(void *vidpath_obj);

void sii_mod_vid_path_intr_mask_enable(void *vidpath_obj,
		bool enable);

#endif /* __SI_MOD_RX_VIDEOPATH_H_*/

