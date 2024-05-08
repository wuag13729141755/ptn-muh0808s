/*
* SiI8630/SiI8632/SiI9630 Linux Driver
*
* 2016 © Lattice Semiconductor Corporation
*
* This program is free software; you can redistribute it and/or
* modify it only under the terms of  version 2 of the GNU General Public License
* as published by the Free Software Foundation, and not any later version.
*
* This program is distributed "AS IS" EWITHOUT ANY WARRANTY of any kind,
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

#include "Sii9630_si_drv_cp8630.h"
#include "Sii9630_si_mod_tx_dsc.h"
#include "Sii9630_si_mod_rx_hdmi.h"
#include "Sii9630_si_mod_tx_hdcp.h"
#include "Sii9630_si_mod_rx_videopath.h"
#include "Sii9630_si_mod_tx_hdmi.h"
//#include "Sii9630_si_mod_tx_hdcp.h"
#include "Sii9630_si_mod_tx_scdc.h"
#include "Sii9630_si_lib_edid.h"
#include "Sii9630_si_lib_log.h"
#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_timer.h"
#include "Sii9630_si_lib_edid_video_tables.h"
#include "Sii9630_si_platform.h"
#if (CONFIG_SOURCE_ONLY == 0)
#include "Sii9630_spiderman.h"
#endif

// Configure INT pin as Open drain, undef to push/pull
#define CONFIG_INT_OPENDRAIN

/* Code under this macro is to fix the Issue with BT2020 colorimetry input
 * Note: Banner doesn't support BT2020, As of now the issue is fixed with
 * these changes so keeping this code.*/
#define CSC_AS_PER_INPUT_STANDARD

/*HDCP arbitration masks*/
#define HDCP_ENABLE__APP 0x01
#define HDCP_ENABLE__API 0x02
#if (CONFIG_SOURCE_ONLY == 1)
#define HDCP_ENABLE__MASK HDCP_ENABLE__API
#else
#define HDCP_ENABLE__MASK 0
#endif

#define SII_DRIVER_MINOR_MAX 1

#define AUDIO_N_CTS_MAX_ENTRY 14

#define HDCP_ENABLE__HDMI 0x01
#define HDCP_ENABLE__MHL  0x02

#define OSCILLATOR_RATE  0x04

/**
 * the whole driver module depends on few RX and TX states that are not under
 firmware control
 * 1. RX connection status - RPWR
 * 2. TX connection status - HPD
 * 3. RX TMDS mode - SCDT/CKDT
 * 4. RX HDCP status - auth done in HDCP 1.4 and ske init rcvd in HDCP2.2
 * number of possible states is 2^4
 *
 * POSSIBLE STATES
 *==================
 * | TX CONN | RX HDCP | RXTMDS | RX CONN | internal state
 * -------------------------------------------------------
 * |	0    |	  0    |   0	|    0	  | ISTATE0 --> intial state.
 * -------------------------------------------------------
 * |	0    |	  0    |   0	|    1	  | ISTATE1
 * -------------------------------------------------------
 * |	0    |	  0    |   1	|    1	  | ISTATE3
 * -------------------------------------------------------
 * |	0    |	  1    |   1	|    1	  | ISTATE7
 * -------------------------------------------------------
 * |	1    |	  0    |   0	|    0	  | ISTATE8
 * -------------------------------------------------------
 * |	1    |	  0    |   0	|    1	  | ISTATE9
 * -------------------------------------------------------
 * |	1    |	  0    |   1	|    1	  | ISTATE11
 * -------------------------------------------------------
 * |	1    |	  1    |   1	|    1	  | ISTATE15
 * -------------------------------------------------------
 *
 * possible state Changes:	0->0, 0->1, 0->8,
 *				1->0, 1->3, 1->9,
 *				3->0, 3->1, 3->7, 3->11
 *				7->0, 7->1, 7->3, 7->15,
 *				8->0, 8->9,
 *				9->1, 9->8, 9->11,
 *				11->3, 11->8, 11->9, 11->15,
 *				15->7, 15->8, 15->9, 15->11
 * -----------------------------------------------------------------
 */


static struct sii_drv_vid_timing_params s_vix_frmts[VIXS_FMT__TOTAL_FMTS] = {
/* detected timing*/
/*{340, 60, 3840, 160, 2160, 90, 8, 10, 72},*/
/*	4kx2k60_hz - 4:2:0 -10bit*/
{31500000, 7, 2400
/*((3840/2) * 1.25)*/, 100 /*((160/2) * 1.25)*/, 2160, 90, 8, 10, 72},
/*	4kx2k60_hz - 4:2:0 -10bit*/
{340000000, 60, 2400
/*((3840/2) * 1.25)*/, 100 /*((160/2) * 1.25)*/, 2160, 90, 8, 10, 72},
/*	4kx2k50_hz - 4:2:0 -10bit*/
{340000000, 50, 2400
/*((3840/2) * 1.25)*/, 600 /*((160/2) * 1.25)*/, 2160, 90, 8, 10, 72}, };

static struct sii_drv_vtg_params s_vtg_timing_params = {0x0057, 0x017F, 0x0896,
	0x107F, 0x112E, 0x0013, 0x0099, 0x00A3, 0x1183, 0x1193, 0x03, 0x00c6, };

#ifdef CONFIG_SWAY_CONFIG_POST_CONV
static uint32_t vixs_fmt_out_pixel_clk_map[VIXS_FMT__TOTAL_FMTS][2] = {
	{VIXS_FMT__EMULATION, 74250000},
	{VIXS_FMT__4K_2K_60HZ, 540000000},
	{VIXS_FMT__4K_2K_50HZ, 540000000}
};
#endif

static bool sg_bEdidSwitchFlag = FALSE;
static u8 *sg_pu8Sii9630Edidbuf = NULL;
static u16 sg_u16Sii9630EdidLen = 0;

/*By giving one block EDID some sources are behaving wrong
 *So setting 2 block EDID, refer bug 45219*/
#define BUG_45219
#ifndef BUG_45219
/* This table is to be used as DS EDID if our device is unable to read
	 real DS EDID */
static uint8_t default_ds_edid[SII_EDID_BLOCK_SIZE] = {
/* (00H)      Header*/
	0x00,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0xFF,  0x00,

/*	(08H-09H)  ID Manufacturer Name _________________________    = Unknown
	(0AH-0BH)  Product ID Code ______________________________    = Unknown
	(0CH-0FH)  Last 4 Digits of Serial Number _______________    = NOT USED
	(10H)      Week of Manufacture __________________________    = 0
	(11H)      Year of Manufacture __________________________    = 1990 */
	0x01, 0x01, /* EDID__MANUFACTURER_ID, */
	0x01, 0x01, /*EDID__PRODUCT_ID, */
	0x01,  0x01,  0x01,  0x01,
	0x00,
	0x00,

/* (12H)      EDID version 1.3 */
	0x01,  0x03,

/*             Basic Display Parameters
	(14H)      VIDEO INPUT DEFINITION: Digital Signal
	(15H)      Maximum Horizontal Image Size ________    =   undefined
	(16H)      Maximum Vertical Image Size __________    =   undefined
	(17H)      Display Gamma ________________________    =   2.2 (BT.709)
	(18H)      DPMS and Supported Feature(s):  */
	/*Display Type = RGB Color, Preferred Timing Mode */
	0x80,  0x00,  0x00,  0x78,  0x0A,

/*(19H-22H)  CHROMA INFO: */

/*         Red x - 0.640 Green x - 0.300 Blue x - 0.150 White x - 0.313
*         Red y - 0.330 Green y - 0.600 Blue y - 0.060 White y - 0.329
*        sRGB */
	0xEE,  0x91,  0xA3,  0x54,  0x4C,  0x99,  0x26,  0x0F,  0x50,  0x54,


/* (23H)      ESTABLISHED TIMING I:*/
	0x00, /* None Specified */

/*(24H)      ESTABLISHED TIMING II: */
	0x00, /* None Specified */

/*(25H)      Manufacturer's Reserved Timing: */
	0x00, /* None Specified */

/*(26H-35H)  Standard Timing Identification:
*            Not Used*/
	0x01,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,
	0x01,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,  0x01,

/* Since at least one preferred timing structure is required in EDID 1.3 */

/*(36H-47H) Detailed Timing / Descriptor Block 1: */
/* NTSC */
/* Detailed Timing: 720x480p 59.94Hz 4:3 */
	0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00,
	0x13, 0x8E, 0x21, 0x00, 0x00, 0x18,

/*(48H-59H) Detailed Timing / Descriptor Block 2: */
/* Descriptor: ASCII string with dummy data (just to fill out the DTD space)*/
	0x00, 0x00, 0x00, 0xFE, 0x00, /* header */
	'D', 'U', 'M', 'M', 'Y', 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,

/*(5AH-6BH) Detailed Timing / Descriptor Block 3: */
/* Descriptor: Monitor Name string */
	0x00,  0x00,  0x00,  0xFC,  0x00, /* header */
	'S', 'I', 'I', ' ', 'T', 'O', 'R', 'O', 'N', 'T', 'O', ' ', 0x0A,

/*(6CH-7DH) Detailed Timing / Descriptor Block 4: */
/*         Monitor Range Limits: */
	0x00,  0x00,  0x00,  0xFD,  0x00, /* header */
	48,          /* Min Vertical Freq */
	62,          /* Max Vertical Freq */
	14,          /* Min Horiz. Freq */
	69,          /* Max Horiz. Freq */
	8,           /* Pixel Clock in 10MHz */
	0x00,        /* GTF - Not Used */
	0x0A,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20, /* padding */

/*(7EH)    Number Of Extension Blocks */
/*         No Extension EDID Block(s) */
	0x00,

/*(7FH)    Check Sum */
	0x65
};
#else
/*This is exact copy of banner_edid, BUT removed 4096 SVDs*/
static uint8_t default_ds_edid[SII_EDID_RX_MAX_LEN] = {
    #if 1
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
    0x55,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
    0x32,0x1C,0x01,0x03,0x80,0x73,0x41,0x78,
    0x0A,0xCF,0x74,0xA3,0x57,0x4C,0xB0,0x23,
    0x09,0x48,0x4C,0x01,0x08,0x00,0xD1,0xC0,
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x01,0x04,0x74,
    0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,
    0x8A,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,
    0x58,0x2C,0x45,0x00,0xC4,0x8E,0x21,0x00,
    0x00,0x1E,0x00,0x00,0x00,0xFC,0x00,0x48,
    0x44,0x20,0x56,0x69,0x64,0x65,0x6F,0x0A,
    0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,
    0x00,0x3B,0x46,0x1F,0x8C,0x3C,0x00,0x0A,
    0x20,0x20,0x20,0x20,0x20,0x20,0x01,0xC0,
    0x02,0x03,0x23,0xF2,0x4E,0x05,0x84,0x13,
    0x14,0x90,0x1F,0x20,0x22,0x5D,0x5F,0x62,
    0x64,0x5E,0x63,0x23,0x09,0x1F,0x07,0x83,
    0x01,0x00,0x00,0x67,0x03,0x0C,0x00,0x10,
    0x00,0x00,0x44,0x8C,0x0A,0xD0,0x8A,0x20,
    0xE0,0x2D,0x10,0x10,0x3E,0x96,0x00,0xC4,
    0x8E,0x21,0x00,0x00,0x18,0x8C,0x0A,0xA0,
    0x14,0x51,0xF0,0x16,0x00,0x26,0x7C,0x43,
    0x00,0xC4,0x8E,0x21,0x00,0x00,0x98,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,
    #else
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
	0x4D, 0x29, 0x30, 0xA0, 0x01, 0x00, 0x00, 0x00,
	0x06, 0x19, 0x01, 0x03, 0x80, 0x80, 0x48, 0x78,
	0x0A, 0xDA, 0xFF, 0xA3, 0x58, 0x4A, 0xA2, 0x29,
	0x17, 0x49, 0x4B, 0x21, 0x08, 0x00, 0x31, 0x40,
	0x45, 0x40, 0x61, 0x40, 0x81, 0x80, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0xE8,
	0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
	0x8A, 0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x1E,
	0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
	0x58, 0x2C, 0x45, 0x00, 0xBA, 0x88, 0x21, 0x00,
	0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x38,
	0x36, 0x2F, 0x39, 0x36, 0x33, 0x30, 0x0A, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x17, 0x3D, 0x0F, 0x88, 0x3C, 0x00, 0x0A,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xCE,

	0x02 ,0x03 ,0x3D ,0xF0 ,0x4D ,0x10 ,0x05 ,0x20,
	0x22 ,0x04 ,0x03 ,0x02 ,0x07 ,0x06 ,0x61 ,0x5D,
	0x5F ,0x60 ,0x23 ,0x09 ,0x07 ,0x01 ,0x77 ,0x03,
	0x0C ,0x00 ,0x40 ,0x00 ,0xB8 ,0x3C ,0x2F ,0xC8,
	0x6A ,0x01 ,0x03 ,0x04 ,0x81 ,0x41 ,0x00 ,0x16,
	0x06 ,0x08 ,0x00 ,0x56 ,0x58 ,0x00 ,0x67 ,0xD8,
	0x5D ,0xC4 ,0x01 ,0x78 ,0x88 ,0x03 ,0xE2 ,0x00,
	0x4B ,0xE3 ,0x0F ,0x00 ,0x12 ,0x01 ,0x1D ,0x80,
	0x18 ,0x71 ,0x1C ,0x16 ,0x20 ,0x58 ,0x2C ,0x25,
	0x00 ,0xBA ,0x88 ,0x21 ,0x00 ,0x00 ,0x9E ,0x56,
	0x5E ,0x00 ,0xA0 ,0xA0 ,0xA0 ,0x29 ,0x50 ,0x30,
	0x20 ,0x35 ,0x00 ,0xBA ,0x88 ,0x21 ,0x00 ,0x00,
	0x1A ,0x66 ,0x21 ,0x56 ,0xAA ,0x51 ,0x00 ,0x1E,
	0x30 ,0x46 ,0x8F ,0x33 ,0x00 ,0xBA ,0x88 ,0x21,
	0x00 ,0x00 ,0x1E ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
	0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x36,
	#endif
};

#endif

/*Banner 1.0 does not support 420 deep color format*/
static uint8_t banner_edid[SII_EDID_RX_MAX_LEN] = {
	0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
	0x4D, 0x29, 0x30, 0xA0, 0x01, 0x00, 0x00, 0x00,
	0x06, 0x19, 0x01, 0x03, 0x80, 0x80, 0x48, 0x78,
	0x0A, 0xDA, 0xFF, 0xA3, 0x58, 0x4A, 0xA2, 0x29,
	0x17, 0x49, 0x4B, 0x21, 0x08, 0x00, 0x31, 0x40,
	0x45, 0x40, 0x61, 0x40, 0x81, 0x80, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, 0xE8,
	0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
	0x8A, 0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x1E,
	0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
	0x58, 0x2C, 0x45, 0x00, 0xBA, 0x88, 0x21, 0x00,
	0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x38,
	0x36, 0x2F, 0x39, 0x36, 0x33, 0x30, 0x0A, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
	0x00, 0x17, 0x3D, 0x0F, 0x88, 0x3C, 0x00, 0x0A,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xCE,

	0x02, 0x03, 0x41, 0xF0, 0x51, 0x10, 0x05, 0x20,
	0x22, 0x04, 0x03, 0x02, 0x07, 0x06, 0x61, 0x5D,
	0x5F, 0x66, 0x62, 0x64, 0x60, 0x65, 0x23, 0x09,
	0x07, 0x01, 0x77, 0x03, 0x0C, 0x00, 0x40, 0x00,
	0xB8, 0x3C, 0x2F, 0xC8, 0x6A, 0x01, 0x03, 0x04,
	0x81, 0x41, 0x00, 0x16, 0x06, 0x08, 0x00, 0x56,
	0x58, 0x00, 0x67, 0xD8, 0x5D, 0xC4, 0x01, 0x78,
	0x88, 0x03, 0xE2, 0x00, 0x4B, 0xE3, 0x0F, 0x00,
	0x12, 0x01, 0x1D, 0x80, 0x18, 0x71, 0x1C, 0x16,
	0x20, 0x58, 0x2C, 0x25, 0x00, 0xBA, 0x88, 0x21,
	0x00, 0x00, 0x9E, 0x56, 0x5E, 0x00, 0xA0, 0xA0,
	0xA0, 0x29, 0x50, 0x30, 0x20, 0x35, 0x00, 0xBA,
	0x88, 0x21, 0x00, 0x00, 0x1A, 0x66, 0x21, 0x56,
	0xAA, 0x51, 0x00, 0x1E, 0x30, 0x46, 0x8F, 0x33,
	0x00, 0xBA, 0x88, 0x21, 0x00, 0x00, 0x1E, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9D,
};


static struct audio_N_CTS_table audio_N_32khz[] = {
	/*tmds_rate, sampling_freq, N_values */
	{ 25174825, 4576 }, { 25200000, 4096 }, { 27000000, 4096 },
	{ 26973026, 4096 }, { 54000000, 4096 }, { 53946053, 4096 },
	{ 74175824, 11648 }, { 74250000, 4096 }, { 148351648, 11648 },
	{ 148500000, 4096 }, { 296703296, 5824 }, { 297000000, 4096 },
	{ 593406593, 5824 }, { 594000000, 4096 }, { 0, 4096 },
};
static struct audio_N_CTS_table audio_N_441khz[] = {
	/*for 44.1 khz */
	{ 25174825, 7007 }, { 25200000, 6272 }, { 27000000, 6272 },
	{ 26973026, 6272 }, { 54000000, 6272 }, { 53946053, 6272 },
	{ 74175824, 17836 }, { 74250000, 6272 }, { 148351648, 8918 },
	{ 148500000, 6272 }, { 296703296, 4459 }, { 297000000, 4704 },
	{ 593406593, 8918 }, { 594000000, 9408 }, { 0, 6272 },
};

static struct audio_N_CTS_table audio_N_48khz[] = {
	/*for 48 khz */
	{ 25174825, 6864 }, { 25200000, 6144 }, { 27000000, 6144 },
	{ 26973026, 6144 }, { 54000000, 6144 }, { 53946053, 6144 },
	{ 74175824, 11648 }, { 74250000, 6144 }, { 148351648, 5824 },
	{ 148500000, 6144 }, { 296703296, 5824 }, { 297000000, 5120 },
	{ 593406593, 5824 }, { 594000000, 6144 }, { 0, 6144 },
};

static struct audio_N_CTS_table audio_N_882khz[] = {
	/*for 88.2 khz */
	{ 25174825, 14014 }, { 25200000, 12544 }, { 27000000, 12544 },
	{ 26973026, 12544 }, { 54000000, 12544 }, { 53946053, 12544 },
	{ 74175824, 35672 }, { 74250000, 12544 }, { 148351648, 17836 },
	{ 148500000, 12544 }, { 296703296, 8918 }, { 297000000, 9408 },
	{ 593406593, 17836 }, { 594000000, 18816 }, { 0, 12544 },
};
static struct audio_N_CTS_table audio_N_96khz[] = {
	/*for 96 khz */
	{ 25174825, 13728 }, { 25200000, 12288 }, { 27000000, 12288 },
	{ 26973026, 12288 }, { 54000000, 12288 }, { 53946053, 12288 },
	{ 74175824, 23296 }, { 74250000, 12288 }, { 148351648, 11648 },
	{ 148500000, 12288 }, { 296703296, 11648 }, { 297000000, 10240 },
	{ 593406593, 11648 }, { 594000000, 12288 }, { 0, 12288 },
};
static struct audio_N_CTS_table audio_N_1764khz[] = {
	/*for 176.4 khz */
	{ 25174825, 28028 }, { 25200000, 25088 }, { 27000000, 25088 },
	{ 26973026, 25088 }, { 54000000, 25088 }, { 53946053, 25088 },
	{ 74175824, 71344 }, { 74250000, 25088 }, { 148351648, 35672 },
	{ 148500000, 25088 }, { 296703296, 17836 }, { 297000000, 18816 },
	{ 593406593, 35672 }, { 594000000, 37632 }, { 0, 25088 },
};
static struct audio_N_CTS_table audio_N_192khz[] = {
	/*for 192 khz */
	{ 25174825, 27456 }, { 25200000, 24576 }, { 27000000, 24576 },
	{ 26973026, 24576 }, { 54000000, 24576 }, { 53946053, 24576 },
	{ 74175824, 46592 }, { 74250000, 24576 }, { 148351648, 23296 },
	{ 148500000, 24576 }, { 296703296, 23296 }, { 297000000, 20480 },
	{ 593406593, 23296 }, { 594000000, 24576 }, { 0, 24576 },
};
static struct audio_N_CTS_table audio_N_256khz[] = {
	/*for 192 khz */
	{ 25174825, 36608 }, { 25200000, 32768 }, { 27000000, 32768 },
	{ 26973026, 32768 }, { 54000000, 32768 }, { 53946053, 32768 },
	{ 74175824, 93184 }, { 74250000, 32768 }, { 148351648, 93184 },
	{ 148500000, 32768 }, { 296703296, 46592 }, { 297000000, 32768 },
	{ 593406593, 46592 }, { 594000000, 32768 }, { 0, 32768 },
};
static struct audio_N_CTS_table audio_N_3528khz[] = {
	/*for 192 khz */
	{ 25174825, 56056 }, { 25200000, 50176 }, { 27000000, 50176 },
	{ 26973026, 50176 }, { 54000000, 50176 }, { 53946053, 50176 },
	{ 74175824, 142688 }, { 74250000, 50176 }, { 148351648, 71344 },
	{ 148500000, 50176 }, { 296703296, 35672 }, { 297000000, 37632 },
	{ 593406593, 71344 }, { 594000000, 75264 }, { 0, 50176 },
};
static struct audio_N_CTS_table audio_N_384khz[] = {
	/*for 192 khz */
	{ 25174825, 54912 }, { 25200000, 49152 }, { 27000000, 49152 },
	{ 26973026, 49152 }, { 54000000, 49152 }, { 53946053, 49152 },
	{ 74175824, 93184 }, { 74250000, 49152 }, { 148351648, 46592 },
	{ 148500000, 49152 }, { 296703296, 46592 }, { 297000000, 40960 },
	{ 593406593, 46592 }, { 594000000, 49152 }, { 0, 49152 },
};

static void s_temp_tdms_handler(void *pobj);
static void s_hdcp_auth_deglitch(void *pobj);

static void s_rx_tmds_mode_change_handler(struct driver_obj *drv_obj);
static void s_update_edid(struct driver_obj *drv_obj);
static void s_dump_buffer(uint8_t *p_data, uint16_t len);
static void s_tx_hpd_handler(struct driver_obj *drv_obj);
static void s_tx_counter_handler(struct driver_obj *drv_obj);
#ifndef CONFIG_HDCP_REPEATER_EN
static void s_tx_hdcp_handler(struct driver_obj *drv_obj);
#endif
static enum sii_tmds_mode s_get_ds_tmds_mode(struct driver_obj *drv_obj);
static void s_mod_handle_tx_hdcp_events(struct driver_obj *drv_obj,
						uint32_t event_flags);
static void s_tx_zone_enable(struct driver_obj *drv_obj, bool enable);
static void s_rx_zone_enable(struct driver_obj *drv_obj, bool enable);
static void s_rx_pll_lock_poll(struct driver_obj *drv_obj);
static void s_tx_pll_lock_poll(struct driver_obj *drv_obj);
static void s_tx_tmds_enable(struct driver_obj *drv_obj, bool enable);
static void s_clock_ratio40(struct driver_obj *p_obj, bool on_off);
static void s_rx_manual_zone_enable(struct driver_obj *drv_obj, bool enable);
static void s_tx_hdcp_enable(struct driver_obj *drv_obj, bool enable);
static uint32_t s_get_input_tmds_clock(struct driver_obj *drv_obj);
static void s_tx_pll_lock_monitor(struct driver_obj *drv_obj);
static int dsc_configure(struct driver_obj *drv_obj, uint8_t compression);
#ifdef CONFIG_ENHANCED_MODE_PROT
static int switch_to_dsc_mode(struct driver_obj *drv_obj, uint8_t compression);
static int switch_to_non_dsc_mode(struct driver_obj *drv_obj);
static bool is_ds_device_spiderman2(struct driver_obj *drv_obj);
static int handle_enhanced_mode(struct driver_obj *drv_obj);
#endif
static int s_rx_hot_plug_toggle(void *p_obj);

static int s_tx_hdcp_prot_set(struct driver_obj *drv_obj)
{
	int ret = 0;
	bool b_enable;

	if (drv_obj->chip_id == 0x8632) {
		/* 0x8632 doesn't support HDCP */
		return 0;
	}

	if (drv_obj->tx_hdcp_auth_type == SII_DRV_HDCP_LINK_VER__NONE)
		b_enable = false;
	else if (drv_obj->hdcp_arbitration_mask
		== (HDCP_ENABLE__APP | HDCP_ENABLE__API))
		b_enable = true;
	else
		b_enable = false;

	/* Set HDCP Auth Type */

	if ((drv_obj->tx_tmds_mode == SII_TMDS_MODE__DVI) &&
		(drv_obj->tx_hdcp_auth_type == SII_DRV_HDCP_LINK_VER__AUTO)) {
		enum sii_drv_hdcp_link_version force_to_hdcp14
			= SII_DRV_HDCP_LINK_VER__HDCP1X;
		sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__DS_AUTH_TYPE,
			&force_to_hdcp14);
	} else {
		sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__DS_AUTH_TYPE,
			&drv_obj->tx_hdcp_auth_type);
	}

	/* Enable/Disable HDCP */
	ret =
	    sii_mod_tx_hdcp_set(drv_obj->hdcp_obj, SII_MOD_HDCP_OPCODE__ENABLE,
		&b_enable);
	/*sii_drv_tx_av_mute_set(drv_obj, false);*/
	/*sii_platform_wr_reg8(drv_obj->pstDevice, 0x0048, 0xF0);*/

	return ret;
}

static void tx_start(struct driver_obj *drv_obj, enum tx_requester requester,
	enum sii_tmds_mode tx_tmds_mode)
{
	if (drv_obj->tx_tmds_mode_user != SII_TMDS_MODE__AUTO)
		return;

	if (drv_obj->bist_owner != SII_DRV_BIST_OWNER__API) {
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_MODE,
			&tx_tmds_mode);
	}
}

static void s_tx_stop(struct driver_obj *drv_obj, enum tx_requester requester)
{
	enum sii_tmds_mode tmds_mode = SII_TMDS_MODE__NONE;

	if (drv_obj->tx_tmds_mode_user != SII_TMDS_MODE__AUTO)
		return;

	if ((drv_obj->bist_owner == SII_DRV_BIST_OWNER__API) &&
		(requester != TX_REQUESER__BIST))
		return;

	if (drv_obj->bist_owner != SII_DRV_BIST_OWNER__API) {
		drv_obj->b_tx_is_hdcp_req = false;
		sii_timer_stop(&drv_obj->timer_tx_tmds_enable_poll);
		s_tx_hdcp_enable(drv_obj, false);
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_MODE, &tmds_mode);
	}
}

static void s_tx_tmds_mode_update(struct driver_obj *drv_obj)
{
	s_update_edid(drv_obj);
	drv_obj->tx_tmds_mode = s_get_ds_tmds_mode(drv_obj);
}

static void s_rx_hdcp_fail_handler(struct driver_obj *drv_obj)
{
	if (drv_obj->rx_hdcp_status == SII_DRV_HDCP_STATUS__FAILED)
		s_tx_stop(drv_obj, TX_REQUESER__DRIVER);

}

static void bist_enable(struct driver_obj *drv_obj, bool enable)
{
	struct sii_drv_bist_config bist_cfg;
	if (enable) {
		sii_log_debug("Enabling TPG\n");
		bist_cfg.pat_type = SII_DRV_BIST_PATTERN_TYPE__TEST;
		bist_cfg.test_pat = SII_DRV_BIST_TEST_PATTERN__STATIC_2;
		bist_cfg.bist_tmg = SII_DRV_BIST_TIMING__720P60;
		bist_cfg.clr_depth = SII_DRV_BIST_COLOR_DEPTH__8;
	} else {
		sii_log_debug("Disabling TPG\n");
		bist_cfg.pat_type = SII_DRV_BIST_PATTERN_TYPE__NONE;
	}
	bist_cfg.requester = SII_DRV_BIST_OWNER__DRIVER;
	sii_drv_bist_pattern_generate(drv_obj, &bist_cfg);
}

/**
* tx_clk - if output tx clk has to follow input clk, then pass it as 0
*          else pass the required output clk.
*/
static void update_tx_tmds_clock(struct driver_obj *drv_obj, uint32_t tx_clk)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	if ((tx_clk == 0) && drv_obj->b_rx_tmds_enabled)
		tx_clk = drv_obj->output_tmds_clk =
			s_get_input_tmds_clock(drv_obj);

	if (drv_obj->output_tmds_clk > 340000000) {
		/*sii_log_debug("configure swing skew
		and slew for hdmi2.0\n");*/
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0_SWING_CTL, 0xD5);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D1_SWING_CTL, 0xD5);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2_SWING_CTL, 0xD5);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__MHL_DP_CTL10, 0xC8);

		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0_DRV_CTL, 0x66);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D1_DRV_CTL, 0x66);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2_DRV_CTL, 0x66);

		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0_MISMATCH_CTL, 0x00);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D1_MISMATCH_CTL, 0x00);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2_MISMATCH_CTL, 0x00);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__CK_MISMATCH_CTL, 0xF2);

		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0D1_SLEW_CTL, 0x99);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2CK_SLEW_CTL, 0x09);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__TXZONE_CTL_4, 0xA0);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__PLL_ICP_CTL , 0x0F);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0D1D2_TERM_SEL, 0x3F);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__MHL_DP_CTL2, 0x0F);

	} else {
		/*sii_log_debug("configure swing skew and
		slew for hdmi1.4\n");*/
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0_SWING_CTL, 0x39);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D1_SWING_CTL, 0x3D);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2_SWING_CTL, 0x39);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__MHL_DP_CTL10, 0x36);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0_DRV_CTL, 0x66);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D1_DRV_CTL, 0x66);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2_DRV_CTL, 0x66);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0_MISMATCH_CTL, 0x00);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D1_MISMATCH_CTL, 0x00);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2_MISMATCH_CTL, 0x10);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__CK_MISMATCH_CTL, 0x30);

		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0D1_SLEW_CTL, 0x97);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D2CK_SLEW_CTL, 0x01);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__TXZONE_CTL_4, 0x50);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__PLL_ICP_CTL , 0x0A);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__D0D1D2_TERM_SEL, 0x15);
			sii_platform_wr_reg8(pstDev,
				REG_ADDR__MHL_DP_CTL2, 0x0F);
	}

	/*Intimate Output TMDS clock to Tx module */
	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_TMDS_CLOCK_SET,
		(void *)&tx_clk);
}

static void s_txoff_rx_off_on_change_handler(struct driver_obj *drv_obj)
{
	struct sii_edid rx_edid;

	rx_edid.size = SII_EDID_RX_MAX_LEN;

	memcpy(rx_edid.b, banner_edid, rx_edid.size);
}

static void s_rxoff_tx_off_on_change_handler(struct driver_obj *drv_obj)
{
	s_tx_tmds_mode_update(drv_obj);
	bist_enable(drv_obj, true);
}

static void s_txoff_rx_on_off_change_handler(struct driver_obj *drv_obj)
{
	sii_drv_rx_hot_plug_set(drv_obj, false);
}

static void s_rxon_tx_off_on_change_handler(struct driver_obj *drv_obj)
{
	/*drv_obj->rx_tmds_mode will be NONE here*/
	s_tx_tmds_mode_update(drv_obj);
	sii_drv_rx_edid_set(drv_obj, &drv_obj->edid);
	/* Tmds should be enabled after scdt/ckdt arrives*/
	/*Rx zone enable will be done after hpd is on */
}

static void s_txoff_rx_tmds_on_off_change_handler(struct driver_obj *drv_obj)
{
	tx_start(drv_obj, TX_REQUESER__DRIVER, drv_obj->rx_tmds_mode);
}

static void s_txoff_rx_hdcp_off_on_change_handler(struct driver_obj *drv_obj)
{
	/* nothing is to be done here*/
}

static void s_rxtmdson_tx_off_on_change_handler(struct driver_obj *drv_obj)
{
	s_tx_tmds_mode_update(drv_obj);
	sii_drv_rx_edid_set(drv_obj, &drv_obj->edid);
}

static void s_txoff_rx_hdcp_on_off_change_handler(struct driver_obj *drv_obj)
{
	s_txoff_rx_hdcp_off_on_change_handler(drv_obj);
}

static void s_rxhdcpon_tx_off_on_change_handler(struct driver_obj *drv_obj)
{
	s_rxtmdson_tx_off_on_change_handler(drv_obj);
}

static void s_rxoff_tx_on_off_change_handler(struct driver_obj *drv_obj)
{
	bist_enable(drv_obj, false);
	s_tx_stop(drv_obj, TX_REQUESER__DRIVER);
}

static void s_txon_rx_off_on_change_handler(struct driver_obj *drv_obj)
{
	struct sii_edid rx_edid;

	bist_enable(drv_obj, false);

	rx_edid.size = drv_obj->edid.size;
	memcpy(rx_edid.b, &drv_obj->edid.b, rx_edid.size);

#ifndef CONFIG_HDCP_REPEATER_EN
	sii_drv_rx_edid_set(drv_obj, &rx_edid);
#else
	sii_drv_rx_edid_set(drv_obj, &drv_obj->edid);
#endif
}

static void s_rxon_tx_on_off_change_handler(struct driver_obj *drv_obj)
{
	s_tx_stop(drv_obj, TX_REQUESER__DRIVER);
	sii_drv_rx_hot_plug_set(drv_obj, false);
}

static void s_txon_rx_on_off_change_handler(struct driver_obj *drv_obj)
{
	bist_enable(drv_obj, true);
}

static void s_txon_rxtmds_off_on_change_handler(struct driver_obj *drv_obj)
{
	update_tx_tmds_clock(drv_obj, 0);

	switch (drv_obj->rx_tmds_mode) {
	case SII_TMDS_MODE__NONE:
	case SII_TMDS_MODE__DVI:
		drv_obj->tx_tmds_mode = drv_obj->rx_tmds_mode;
		break;
	case SII_TMDS_MODE__HDMI1:
		if (drv_obj->output_tmds_clk <= 340000000)
			drv_obj->tx_tmds_mode = drv_obj->rx_tmds_mode;
		else
			drv_obj->tx_tmds_mode = SII_TMDS_MODE__HDMI2;
		break;
	default:
		break;
	}
	tx_start(drv_obj, TX_REQUESER__DRIVER, drv_obj->tx_tmds_mode);
}

static void s_rxtmdson_tx_on_off_change_handler(struct driver_obj *drv_obj)
{
	s_tx_stop(drv_obj, TX_REQUESER__DRIVER);
	sii_drv_rx_hot_plug_set(drv_obj, false);
}

static void s_txon_rx_tmds_on_off_change_handler(struct driver_obj *drv_obj)
{
	s_tx_stop(drv_obj, TX_REQUESER__DRIVER);
}

static void s_txon_rx_hdcp_off_on_change_handler(struct driver_obj *drv_obj)
{
	/*bool is_tx_tmds_en;*/

	/*s_tx_hdcp_prot_set(drv_obj, true);*/
}

static void s_rxhdcpon_tx_on_off_change_handler(struct driver_obj *drv_obj)
{
	s_tx_stop(drv_obj, TX_REQUESER__DRIVER);
	s_txoff_rx_on_off_change_handler(drv_obj);
}

static void s_txon_rx_hdcp_on_off_change_handler(struct driver_obj *drv_obj)
{
	s_rx_hdcp_fail_handler(drv_obj);
}

static void s_update_drv_state(struct driver_obj *drv_obj)
{
	enum sii_drv_i_state constructed_i_state =
		SII_DRV_ISTATE_INVALID;

	switch (drv_obj->drv_istate) {
	case SII_DRV_TX_OFF__RX_OFF:
		if (drv_obj->b_rx_hdmi_connected) {
			s_txoff_rx_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_ON;
		} else if (drv_obj->b_tx_connected) {
			s_rxoff_tx_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_OFF;
		} else {
		}
		break;
	case SII_DRV_TX_OFF__RX_ON:
		if (!drv_obj->b_rx_hdmi_connected) {
			s_txoff_rx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_OFF;
		} else if (drv_obj->b_rx_tmds_enabled) {
			constructed_i_state = SII_DRV_TX_OFF__RX_TMDS_ON;
		} else if (drv_obj->b_tx_connected) {
			s_rxon_tx_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_ON;
		} else {
		}
		break;
	case SII_DRV_TX_OFF__RX_TMDS_ON:
		if (!drv_obj->b_rx_hdmi_connected) {
			s_txoff_rx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_OFF;
		} else if (!drv_obj->b_rx_tmds_enabled) {
			s_txoff_rx_tmds_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_ON;
		} else if (drv_obj->b_tx_connected) {
			s_rxtmdson_tx_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_TMDS_ON;
		} else if (drv_obj->b_rx_hdcp_enabled) {
			s_txoff_rx_hdcp_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_HDCP_ON;
		} else {
		}
		break;
	case SII_DRV_TX_OFF__RX_HDCP_ON:
		if (!drv_obj->b_rx_hdmi_connected) {
			s_txoff_rx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_OFF;
		} else if (!drv_obj->b_rx_tmds_enabled) {
			s_txon_rx_tmds_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_ON;
		} else if (!drv_obj->b_rx_hdcp_enabled) {
			s_txoff_rx_hdcp_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_TMDS_ON;
		} else if (drv_obj->b_tx_connected) {
			s_rxhdcpon_tx_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_HDCP_ON;
		} else {
		}
		break;
	case SII_DRV_TX_ON__RX_OFF:
		if (!drv_obj->b_tx_connected) {
			s_rxoff_tx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_OFF;
		} else if (drv_obj->b_rx_hdmi_connected) {
			s_txon_rx_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_ON;
		} else {
		}
		break;
	case SII_DRV_TX_ON__RX_ON:
		if (!drv_obj->b_tx_connected) {
			s_rxon_tx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_ON;
		} else if (!drv_obj->b_rx_hdmi_connected) {
			s_txon_rx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_OFF;
		} else if (drv_obj->b_rx_tmds_enabled) {
			s_txon_rxtmds_off_on_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_TMDS_ON;
		} else {
		}
		break;
	case SII_DRV_TX_ON__RX_TMDS_ON:
		if (!drv_obj->b_tx_connected) {
			s_rxtmdson_tx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_TMDS_ON;
		} else if (!drv_obj->b_rx_tmds_enabled) {
				s_txon_rx_tmds_on_off_change_handler(drv_obj);
				constructed_i_state = SII_DRV_TX_ON__RX_ON;
			} else if (drv_obj->b_rx_hdcp_enabled) {
				s_txon_rx_hdcp_off_on_change_handler(drv_obj);
				constructed_i_state = SII_DRV_TX_ON__RX_HDCP_ON;
			 } else {
		}
		break;
	case SII_DRV_TX_ON__RX_HDCP_ON:
		if (!drv_obj->b_tx_connected) {
			s_rxhdcpon_tx_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_OFF__RX_HDCP_ON;
		} else if (!drv_obj->b_rx_hdcp_enabled) {
			s_txon_rx_hdcp_on_off_change_handler(drv_obj);
			constructed_i_state = SII_DRV_TX_ON__RX_TMDS_ON;
		} else {
		}
		break;
	default:
		break;
	}
	if ((SII_DRV_ISTATE_INVALID != constructed_i_state)
		&& (constructed_i_state != drv_obj->drv_istate)) {
		drv_obj->drv_istate = constructed_i_state;
	}
}

/*Input TMDS clock calculation*/
static uint32_t s_get_input_tmds_clock(struct driver_obj *drv_obj)
{
	uint64_t input_tmds_clk = 0;
	uint64_t input_pxl_clk = 0;
	uint8_t vic = drv_obj->avi_data.payload.pb4.vic;
	uint8_t hdmi_vic = drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;
	enum avi_color_space clrspc = drv_obj->avi_data.payload.pb1.colorSpace;

	const struct video_mode_type *video_timing;
	struct sii_drv_vid_timing_params ip_vid_timing_params;

	drv_obj->upsampled_to_422 = false;
	video_timing = sii_vid_timing_from_vic(vic, hdmi_vic);

	if (video_timing) {
		enum sii_drv_bit_depth input_vid_dc_depth;

		input_pxl_clk = video_timing->pix_clk_10khz * 10000;
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
			&input_vid_dc_depth);
		switch (input_vid_dc_depth) {
		default:
		case SII_DRV_BIT_DEPTH__8_BIT:
			input_tmds_clk = input_pxl_clk;
			break;
		case SII_DRV_BIT_DEPTH__10_BIT:
			/*input_tmds_clk =
				get_int_division((input_pxl_clk * 10), 8);*/
			input_tmds_clk = (input_pxl_clk / 8) * 10;
			break;
		case SII_DRV_BIT_DEPTH__12_BIT:
			/*input_tmds_clk =
				get_int_division((input_pxl_clk * 12), 8);*/
			input_tmds_clk = (input_pxl_clk / 8) * 12;
			break;
		}

		if (drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vid_fmt ==
				HVF__3D_FORMAT_INDICATION_PRESENT) {
				switch (drv_obj->vsif_hdmi_or_mhl3.payload.hdmi._3d_structure) {
				case _3D_STRUCTURE__FRMPACKING:
					input_tmds_clk *= 2;
					break;
				case _3D_STRUCTURE__TOPANDBOTTOM:
				case _3D_STRUCTURE__SIDEBYSIDEHALF:
				default:
					break;
				}
		}
		if (drv_obj->avi_data.payload.pb5.pixel_repetition_factor) {
			struct avi_info_frame_data_byte_5 pb5 =
				drv_obj->avi_data.payload.pb5;
			input_tmds_clk *=
				(pb5.pixel_repetition_factor + 1);
		}
		if (clrspc == ACS__YCBCR420) {
			sii_mod_vid_path_get(drv_obj->vidpath_obj,
				SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS,
				&ip_vid_timing_params);
			if ((vic == 96) || (vic == 97) || (vic == 106) || (vic == 107)) {
				if ((ip_vid_timing_params.h_active >= (3840 - 20))
					&& (ip_vid_timing_params.h_active <= (3840 + 20))) {
					/*upsampled to YC422*/
					input_tmds_clk = (ip_vid_timing_params.v_freq *
						(((ip_vid_timing_params.v_active +
							ip_vid_timing_params.v_blank) *
							(ip_vid_timing_params.h_active +
								ip_vid_timing_params.h_blank))));
					drv_obj->upsampled_to_422 = true;
				} else {
					drv_obj->upsampled_to_422 = false;
					input_tmds_clk /= 2;
				}
			} else {
				input_tmds_clk /= 2;
			}
		}

	} else {
		sii_log_debug("Calculate using hardware totals\n");
		sii_mod_vid_path_get(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS,
			&ip_vid_timing_params);
        sii_log_debug("v_freq   = %d\n",ip_vid_timing_params.v_freq);
        sii_log_debug("v_freq_mult_factor   = %d\n",ip_vid_timing_params.v_freq_mult_factor);
        sii_log_debug("v_active = %d\n",ip_vid_timing_params.v_active);
        sii_log_debug("v_blank  = %d\n",ip_vid_timing_params.v_blank);
        sii_log_debug("h_active = %d\n",ip_vid_timing_params.h_active);
        sii_log_debug("h_blank  = %d\n",ip_vid_timing_params.h_blank);
        #if 0
		input_tmds_clk = ((ip_vid_timing_params.v_freq +
            (ip_vid_timing_params.v_freq_mult_factor+10)/100)*
			(((ip_vid_timing_params.v_active +
			ip_vid_timing_params.v_blank) *
			(ip_vid_timing_params.h_active +
			ip_vid_timing_params.h_blank))));
        #else
        input_tmds_clk = (ip_vid_timing_params.v_freq*
			(((ip_vid_timing_params.v_active +
			ip_vid_timing_params.v_blank) *
			(ip_vid_timing_params.h_active +
			ip_vid_timing_params.h_blank))));
        #endif
	}
	return input_tmds_clk;
}

/*Input Audio sampling frequency calculation*/
static int s_get_input_audio_sampling_frequency(struct driver_obj *drv_obj)
{
	uint32_t input_pixel_clk;
	uint32_t input_tmds_clk;
	uint32_t input_audio_sampling_frequency = 0;
	uint32_t input_audio_sampling_frequency_temp;
	int status = 0;

	input_pixel_clk = s_get_input_tmds_clock(drv_obj);

	if ((input_pixel_clk == 0) ||
		(drv_obj->acr_if_data.aud_params.audio_cts == 0)) {
		sii_log_err("Invalid: i/p pxl clk = %ld, audio_cts = %ld\n",
			input_pixel_clk,
			drv_obj->acr_if_data.aud_params.audio_cts);
		status = 1;
		return status;
	} else {
		if (input_pixel_clk <= 340000000) {
			input_tmds_clk = input_pixel_clk;
			input_audio_sampling_frequency_temp =
				(input_tmds_clk * 7) /
				(drv_obj->acr_if_data.aud_params.audio_cts);
			input_audio_sampling_frequency_temp /= 128;
			input_audio_sampling_frequency =
				(input_audio_sampling_frequency_temp *
				drv_obj->acr_if_data.aud_params.audio_n);
			input_audio_sampling_frequency /= 7;
		} else {
			/*input_tmds_clk = input_pixel_clk/OSCILLATOR_RATE;*/
			input_tmds_clk = input_pixel_clk;
			input_audio_sampling_frequency_temp =
				(input_tmds_clk /
				drv_obj->acr_if_data.aud_params.audio_cts);
			input_audio_sampling_frequency_temp *=
				drv_obj->acr_if_data.aud_params.audio_n;
			input_audio_sampling_frequency =
				(input_audio_sampling_frequency_temp / 128);
			}
	}

	if ((input_audio_sampling_frequency >= 31000) &&
		(input_audio_sampling_frequency <= 33000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__32KHZ;

	else if ((input_audio_sampling_frequency >= 43000) &&
		(input_audio_sampling_frequency <= 45000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__44_1_KHZ;

	else if ((input_audio_sampling_frequency >= 46000) &&
		(input_audio_sampling_frequency <= 50000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__48KHZ;

	else if ((input_audio_sampling_frequency >= 86000) &&
		(input_audio_sampling_frequency <= 90000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__88_2_KHZ;

	else if ((input_audio_sampling_frequency >= 94000) &&
		(input_audio_sampling_frequency <= 98000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__96KHZ;

	else if ((input_audio_sampling_frequency >= 174000) &&
		(input_audio_sampling_frequency <= 178000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__176_4KHZ;

	else if ((input_audio_sampling_frequency >= 189000) &&
		(input_audio_sampling_frequency <= 193000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__192KHZ;

	else if ((input_audio_sampling_frequency >= 255000) &&
		(input_audio_sampling_frequency <= 257000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__256KHZ;

	else if ((input_audio_sampling_frequency >= 351000) &&
		(input_audio_sampling_frequency <= 353000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__352_8KHZ;

	else if ((input_audio_sampling_frequency >= 383000) &&
		(input_audio_sampling_frequency <= 385000))
		drv_obj->aud_samplg_freq = SII_DRV_AUDIO_SAMPLG_FRQ__384KHZ;

	else {
		sii_log_err("i/p sampling frequency not in valid range\n",
			input_audio_sampling_frequency);
		status = 1;
		return status;
	}
	return status;
}


/**
* Enable or disable sway pll
*/
static void s_sway_pll_enable(struct driver_obj *drv_obj, bool enable)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	if (enable) {
		/*power on swaypll*/
		sii_platform_set_bit8(pstDev, REG_ADDR__TMDS_CTRL2,
			BIT_MSK__TMDS_CTRL2__REG_PWRON_HSIC);
		sii_platform_set_bit8(pstDev, REG_ADDR__SWAY_FPLL_CTRL_0,
			BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_BIAS_CP |
			BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_I2C_PD |
			BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_PDB);
		/*before strobe swaypll to settle*/
		msleep(100);
		sii_platform_set_bit8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_WP,
			BIT_MSK__SWAY_FPLL_FB_DIV_WP__REG_SWAY_PLL_FB_DIV_WP);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_WP,
			BIT_MSK__SWAY_FPLL_FB_DIV_WP__REG_SWAY_PLL_FB_DIV_WP);
		sii_log_debug("Enabled swaypll\n");
	} else {
		sii_platform_clr_bit8(pstDev, REG_ADDR__SWAY_FPLL_CTRL_0,
			BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_BIAS_CP |
			BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_I2C_PD |
			BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_PDB);
		drv_obj->sway_owner = SWAY_INUSE_FOR__NONE;
		sii_log_debug("Disabled swaypll\n");
		/*power down swaypll*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__TMDS_CTRL2,
			BIT_MSK__TMDS_CTRL2__REG_PWRON_HSIC);
	}
}

void s_get_sway_dividers(uint32_t ip_clk, uint32_t op_clk,
	uint8_t *pre_div, uint16_t *fb_div, uint8_t *post_div,
	uint32_t *fb_div_fract)
{
	uint8_t pre_div_min = 1, pre_div_max = 1,
		post_div_min = 1, post_div_max = 1;
	uint16_t fb_div_min = 1, fb_div_max = 1;

	uint8_t i, j;
	uint16_t k;
	uint32_t l, lpossible;
	uint64_t count, count2, delta, diff_max, diff,
		op_clk_mult, ip_clk_mult, xl;

	/* choose min pre-div value such that ip_clk can be made < 100 MHz */
	while ((pre_div_min * 100 * 1000000) < ip_clk) {
		pre_div_min++;
		;
	}
	pre_div_max = pre_div_min;

	/* choose max pre-div value such that ip_clk will be no less than
	 * 24 MHz and also pre_div should not be > 15 (constraint) */
	while ((pre_div_max * 24 * 1000000) <= ip_clk) {
		pre_div_max++;
		if (pre_div_max > 14) {
			break;
			;
		}
	}
	pre_div_max -= 1; /* since it has incremented before
			     loop became false */

	/* choose min post-div value such that vco range is > 800 MHz */
	for (post_div_min = 1; post_div_min <= 255; post_div_min++) {
		count = (uint64_t)op_clk * post_div_min;
		if (count > (800 * 1000000))
			break;
	}

	/* choose max post-div value such that vco range is < 1800 MHz */
	for (post_div_max = post_div_min; post_div_max <= 255; post_div_max++) {
		count = (uint64_t)op_clk * post_div_max;
		if (count > (1800 * 1000000))
			break;
	}
	post_div_max -= 1; /* since it has incremented before
			      loop became false */

	/* Finding range of fb-div */
	count = (uint64_t)op_clk * pre_div_min * post_div_min;
	for (fb_div_min = 1; fb_div_min <= 65535; fb_div_min++) {
		count2 = (uint64_t)ip_clk * fb_div_min;
		if (count2 > count)
			break;
	}
	/*-1 is required because decimal part of fb-div is allowed*/
	fb_div_min -= 1;
	fb_div_max = fb_div_min;

	count = (uint64_t)op_clk * pre_div_max * post_div_max;
	for (fb_div_max = fb_div_min; fb_div_max <= 65535; fb_div_max++) {
		count2 = (uint64_t)ip_clk * fb_div_max;
		if (count2 > count)
			break;
	}


	/* find in brute force method */
	diff_max = (((uint64_t)op_clk * pre_div_max * post_div_max) -
		((uint64_t)op_clk * pre_div_min * post_div_min)) << 20;

	for (i = post_div_min; i < post_div_max; i++) {
		for (j = pre_div_min; j <= pre_div_max; j++) {
			op_clk_mult = (uint64_t)op_clk * j * i;

			/* this gives the integer part of fb_div */
			/*k = get_int_division(op_clk_mult, ip_clk);*/
			k = (uint16_t)(op_clk_mult / ip_clk);

			ip_clk_mult = (uint64_t)ip_clk * k;

			/*
			* This difference gives ip_clk times the fractional
			* part of fb_divider
			*/
			delta = (op_clk_mult - ip_clk_mult) << 20;

			/*
			* Left shift ensures we get only the MSB nibble
			* of fractional part which is represented as the
			* 20 bits floating point. Only 1 nibble is allowed
			* to be programmed for clock stability
			*/
			/*lpossible = get_int_division(delta, ip_clk);*/
			lpossible = delta / ip_clk;

			for (l = lpossible; l < (lpossible + 2); l++) {
				xl = (uint64_t)ip_clk * l;
				diff = abs(delta - xl);
				if (diff < diff_max) {
					diff_max = diff;
					*post_div = i;
					*pre_div = j;
					*fb_div = k;
					*fb_div_fract = l;
				}
			}
		}
	}
}
/*
* This function is a generic function to configure sway PLL
* This function internally calculates the register configuration
* values based on the input tmds clock and output pixel clock
* requirement.
* known_op_pxl_clk_mult_factor - 0 & known_op_pxl_clk != 0
* when output pixel clk is already known
* known_op_pxl_clk = 0 & known_op_pxl_clk_mult_factor != 0
* when output pixel clk is not known
*/
static int s_sway_pll_configure(struct driver_obj *drv_obj,
	enum sway_pll_owner sway_req,
	uint8_t known_op_pxl_clk_mult_factor,
	uint32_t known_op_pxl_clk)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	uint8_t fpll_pre_divider = 1,
		fpll_post_divider = 1,
		fpll_divider_integer_part_l = 0,
		fpll_divider_integer_part_h = 0,
		fpll_divider_fraction_part_l = 0,
		fpll_divider_fraction_part_m = 0,
		fpll_divider_fraction_part_h = 0;
	uint16_t feedback_fpll_divider = 1;
	uint32_t feedback_fpll_divider_fract = 1;
	uint32_t input_tmds_clk;
	uint32_t output_pixel_clk;
	enum sii_drv_bit_depth input_vid_dc_depth;
	uint32_t dc_mult_factor = 1;
	uint8_t rx_szone, fpll_pre_div_zone_multiplier = 1;

	/* Sway pll is common for 420-422 and DSC and can be used only for
	* either one of them. Arbitration shall be required. */

	if ((drv_obj->sway_owner != SWAY_INUSE_FOR__NONE) &&
		(drv_obj->sway_owner != sway_req)) {
			;
			return 1;
	}
	drv_obj->sway_owner = sway_req;

	if (drv_obj->sway_owner == SWAY_INUSE_FOR__BIST) {
		/*for BISt on board 24 MHZ crystal clock is used
		 *irrespective of input clk*/
		input_tmds_clk = 24000000;
		;
	} else {
		/* calculate input TMDS clock and output pixel clock */
		input_tmds_clk = s_get_input_tmds_clock(drv_obj);
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
			&input_vid_dc_depth);

		switch (input_vid_dc_depth) {
		case SII_DRV_BIT_DEPTH__8_BIT:
			dc_mult_factor = 8;
			break;
		case SII_DRV_BIT_DEPTH__10_BIT:
			dc_mult_factor = 10;
			break;
		case SII_DRV_BIT_DEPTH__12_BIT:
			dc_mult_factor = 12;
			break;
		default:
			dc_mult_factor = 8;
			break;
		}
	}

	/*update output tmds clock*/
	if (known_op_pxl_clk_mult_factor) {
		uint64_t tmp_clk;
		drv_obj->output_tmds_clk = input_tmds_clk *
			known_op_pxl_clk_mult_factor;
		tmp_clk = (((uint64_t)drv_obj->output_tmds_clk) * 8);
		/*output_pixel_clk =
			get_int_division(tmp_clk , dc_mult_factor);*/
		output_pixel_clk = (tmp_clk / dc_mult_factor);
	} else {
		/* not a DSC supported resolution */
		if (0 == known_op_pxl_clk) {
			;
			return 1;
		}
		drv_obj->output_tmds_clk = known_op_pxl_clk;
		output_pixel_clk = known_op_pxl_clk;
	}

	/* Intimate Output TMDS clock to Tx module */
	update_tx_tmds_clock(drv_obj, drv_obj->output_tmds_clk);

	if (drv_obj->sway_owner != SWAY_INUSE_FOR__BIST) {
		rx_szone = (sii_platform_rd_reg8(pstDev,
			REG_ADDR__RX_ZONE_STATUS_0) &
			BIT_MSK__RX_ZONE_STATUS_0__RO_SZONE) >> 1;
		switch (rx_szone) {
		case 1:
			fpll_pre_div_zone_multiplier = 2;
			break;
		case 2:
			fpll_pre_div_zone_multiplier = 4;
			break;
		case 3:
			fpll_pre_div_zone_multiplier = 8;
			break;
		default:
			fpll_pre_div_zone_multiplier = 1;
			break;
		}
	}

	s_get_sway_dividers(input_tmds_clk, output_pixel_clk,
		&fpll_pre_divider, &feedback_fpll_divider, &fpll_post_divider,
		&feedback_fpll_divider_fract);

	fpll_pre_divider *= fpll_pre_div_zone_multiplier;

	fpll_divider_integer_part_l =
		feedback_fpll_divider & 0xFF;
	fpll_divider_integer_part_h =
		(feedback_fpll_divider >> 8) & 0xFF;
	fpll_divider_fraction_part_l =
		feedback_fpll_divider_fract & 0xFF;
	fpll_divider_fraction_part_m =
		(feedback_fpll_divider_fract >> 8) & 0xFF;
	fpll_divider_fraction_part_h =
		(feedback_fpll_divider_fract >> 16) & 0xFF;


	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_CTRL_1, 0xB6);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_CTRL_2, 0x0A);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_SIGMADELTA_CTL, 0x02);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_CP_S, 0x01);

	sii_platform_wr_reg8(pstDev,
		REG_ADDR__SWAY_FPLL_REF_DIV, fpll_pre_divider);
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__SWAY_FPLL_PLL_CFG_3, fpll_post_divider);

	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_INT_1,
		fpll_divider_integer_part_l);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_INT_2,
		fpll_divider_integer_part_h);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_FRAC_1,
		fpll_divider_fraction_part_l);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_FRAC_2,
		fpll_divider_fraction_part_m);
	sii_platform_wr_reg8(pstDev, REG_ADDR__SWAY_FPLL_FB_DIV_FRAC_3,
		fpll_divider_fraction_part_h);

	s_sway_pll_enable(drv_obj, true);

	return 0;
}


static bool s_is_vixs_format_detected(struct driver_obj *drv_obj,
	enum vixs_fmt *p_vixs_id)
{
	struct sii_drv_vid_timing_params ip_vid_timing_params;
	uint8_t in_clr_spc;
	enum sii_drv_bit_depth input_vid_dc_depth;
	bool ret_val = false;

	sii_mod_vid_path_get(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS, &ip_vid_timing_params);
	sii_mod_vid_path_get(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__IN_CLRSPC, (void *)&in_clr_spc);
	sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH, &input_vid_dc_depth);
	switch (input_vid_dc_depth) {
	case SII_DRV_BIT_DEPTH__10_BIT:
		ip_vid_timing_params.pixel_frequency =
			(((ip_vid_timing_params.v_freq *
			(((ip_vid_timing_params.v_active +
			ip_vid_timing_params.v_blank) *
			(ip_vid_timing_params.h_active +
			ip_vid_timing_params.h_blank)))) * 8) /
			10);
		break;
	default:
		return false;
	}
	if (in_clr_spc != 3) /*check for 420*/
		return false;
	if (ip_vid_timing_params.pixel_frequency
		!= s_vix_frmts[0].pixel_frequency)
		return false;
	if (ip_vid_timing_params.v_freq == s_vix_frmts[0].v_freq) {
		if ((ip_vid_timing_params.h_active
			+ ip_vid_timing_params.h_blank)
			!= (s_vix_frmts[0].h_active + s_vix_frmts[0].h_blank))
			return false;
		/*if (ip_vid_timing_params.h_active != s_vix_frmts[0].h_active)
		 return false;
		if (ip_vid_timing_params.h_blank != s_vix_frmts[0].h_blank)
			return false;*/
		if (ip_vid_timing_params.v_active != s_vix_frmts[0].v_active)
			return false;
		if (ip_vid_timing_params.v_back != s_vix_frmts[0].v_back)
			return false;
		if (ip_vid_timing_params.v_front != s_vix_frmts[0].v_front)
			return false;
		if (ip_vid_timing_params.v_sync != s_vix_frmts[0].v_sync)
			return false;
		if (ip_vid_timing_params.v_blank != s_vix_frmts[0].v_blank)
			return false;
		*p_vixs_id = VIXS_FMT__EMULATION;
		ret_val = true;
	} else if (ip_vid_timing_params.v_freq == s_vix_frmts[1].v_freq) {
		if ((ip_vid_timing_params.h_active
			+ ip_vid_timing_params.h_blank)
			!= (s_vix_frmts[1].h_active + s_vix_frmts[1].h_blank))
			return false;
		/*if (ip_vid_timing_params.h_active != s_vix_frmts[1].h_active)
		 return false;
		if (ip_vid_timing_params.h_blank != s_vix_frmts[1].h_blank)
			return false;*/
		if (ip_vid_timing_params.v_active != s_vix_frmts[1].v_active)
			return false;
		if (ip_vid_timing_params.v_back != s_vix_frmts[1].v_back)
			return false;
		if (ip_vid_timing_params.v_front != s_vix_frmts[1].v_front)
			return false;
		if (ip_vid_timing_params.v_sync != s_vix_frmts[1].v_sync)
			return false;
		if (ip_vid_timing_params.v_blank != s_vix_frmts[1].v_blank)
			return false;
		*p_vixs_id = VIXS_FMT__4K_2K_60HZ;
		ret_val = true;
	} else if (ip_vid_timing_params.v_freq == s_vix_frmts[2].v_freq) {
		if ((ip_vid_timing_params.h_active
			+ ip_vid_timing_params.h_blank)
			!= (s_vix_frmts[2].h_active + s_vix_frmts[2].h_blank))
			return false;
		/*if (ip_vid_timing_params.h_active != s_vix_frmts[2].h_active)
		 return false;
		if (ip_vid_timing_params.h_blank != s_vix_frmts[2].h_blank)
			return false;*/
		if (ip_vid_timing_params.v_active != s_vix_frmts[2].v_active)
			return false;
		if (ip_vid_timing_params.v_back != s_vix_frmts[2].v_back)
			return false;
		if (ip_vid_timing_params.v_front != s_vix_frmts[2].v_front)
			return false;
		if (ip_vid_timing_params.v_sync != s_vix_frmts[2].v_sync)
			return false;
		if (ip_vid_timing_params.v_blank != s_vix_frmts[2].v_blank)
			return false;
		*p_vixs_id = VIXS_FMT__4K_2K_50HZ;
		ret_val = true;
	}
	return ret_val;
}

static int s_audio_n_cts_configure(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	struct audio_N_CTS_table *p_audi_table;
	uint8_t samplg_freq = 0;
	uint32_t nvalues = 0;
	uint8_t nvalues_low = 0;
	uint8_t nvalues_mid = 0;
	uint8_t nvalues_high = 0;
	int i = 0;
	int status = 0;
	uint32_t pixel_freq = 0;

	struct sii_drv_vid_timing_params ip_vid_timing_params;
	sii_mod_vid_path_get(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS,
		&ip_vid_timing_params);
	if (ip_vid_timing_params.v_freq_mult_factor) {
		/*this logic is to avoid the float point operation
		 *and 32 bit variable limitation*/
		uint32_t quotient = drv_obj->output_tmds_clk / 1001;
		uint32_t remainder = drv_obj->output_tmds_clk % 1001;
		uint32_t float_value = (remainder * 1000) / 1001;

		pixel_freq = (quotient * 1000) + float_value;
	} else {
		pixel_freq = drv_obj->output_tmds_clk;
	}
	samplg_freq = drv_obj->aud_samplg_freq;
	switch (samplg_freq) {
	case SII_DRV_AUDIO_SAMPLG_FRQ__32KHZ:
		p_audi_table = &audio_N_32khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__44_1_KHZ:
		p_audi_table = &audio_N_441khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__48KHZ:
		p_audi_table = &audio_N_48khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__88_2_KHZ:
		p_audi_table = &audio_N_882khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__96KHZ:
		p_audi_table = &audio_N_96khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__176_4KHZ:
		p_audi_table = &audio_N_1764khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__192KHZ:
		p_audi_table = &audio_N_192khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__256KHZ:
		p_audi_table = &audio_N_256khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__352_8KHZ:
		p_audi_table = &audio_N_3528khz[0];
		break;
	case SII_DRV_AUDIO_SAMPLG_FRQ__384KHZ:
		p_audi_table = &audio_N_384khz[0];
		break;
	default:
		status = 1;
		return status;
	}


	for (i = 0; i < AUDIO_N_CTS_MAX_ENTRY; i++) {
		if (pixel_freq == p_audi_table[i].tmds_rate) {
			nvalues = p_audi_table[i].values_N;
			break;
		}
	}
	if (i == AUDIO_N_CTS_MAX_ENTRY) {
		sii_log_err("Suitable range not found\n");
		nvalues = p_audi_table[AUDIO_N_CTS_MAX_ENTRY].values_N;
	}
	sii_log_debug("Audio_N = %ld\n", nvalues);
	nvalues_low = (nvalues & 0xFF);
	nvalues_mid = ((nvalues & 0xFF00) >> 8);
	nvalues_high = ((nvalues & 0xFF0000) >> 16);

	/* nvalues low*/
	sii_platform_wr_reg8(pstDev,
			REG_ADDR__N_SVAL1, nvalues_low);
	/* nvalues mid*/
	sii_platform_wr_reg8(pstDev,
			REG_ADDR__N_SVAL2, nvalues_mid);
	/* nvalues high*/
	sii_platform_wr_reg8(pstDev,
			REG_ADDR__N_SVAL3, nvalues_high);

	sii_platform_put_bit8(pstDev,
			REG_ADDR__AIP_HDMI2MHL,
			BIT_MSK__AIP_HDMI2MHL__REG_AFIFO_TEST, 0x08);
	sii_platform_set_bit8(pstDev, REG_ADDR__AUDP_TXCTRL,
		BIT_MSK__AUDP_TXCTRL__REG_LAYOUT);

	return status;

}

static uint8_t calculate_generic_checksum(void *if_data_parm, uint8_t checksum,
	uint8_t length)
{
	uint8_t i;
	uint8_t *info_frame_data = (uint8_t *) if_data_parm;

	for (i = 0; i < length; i++)
		checksum += info_frame_data[i];

	checksum = 0x100 - checksum;

	return checksum;
}

static int s_parse_audioif(struct driver_obj *drv_obj,
	struct sii_info_frame *if_audio)
{
	uint8_t checksum = 0;
	uint8_t channel_count = 0;
	uint8_t sampling_frequency = 0;
	uint8_t sample_size = 0;

	if (if_audio->if_id != SII_INFO_FRAME_ID__AUDIO) {
		sii_log_debug("Invalid Infoframe\n");
		return 1;
	}

	/* validate checksum */
	checksum = calculate_generic_checksum(if_audio->b, 0,
		SII_INFOFRAME_MAX_LEN);

	if (0 != checksum) {
		sii_log_debug(
			"AIF Checksum (including checksum) is: 0x%02x	should be 0\n",
			checksum);
		return 1;
	}

	memset(&drv_obj->aif_data, 0,
		sizeof(struct audio_info_frame));

	memcpy(&drv_obj->aif_data.header, if_audio->b,
		sizeof(struct info_frame_header));

	if (AUDIOIF_TYPE != drv_obj->aif_data.header.type_code) {
		sii_log_debug("Invalid Audio IF type code: 0x%02x\n",
			drv_obj->aif_data.header.type_code);
		return 1;
	}

	channel_count = (if_audio->b[4] & 0x07);
	sampling_frequency = (if_audio->b[5] & 0x1C) >> 2;
	sample_size = (if_audio->b[5] & 0x03);

	/*please refer sec8.2.2 of HDMI specification 14B*/
	switch (sampling_frequency) {
	case 0x00:
	case 0x01:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__32KHZ;
		break;
	case 0x02:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__44_1_KHZ;
		break;
	case 0x03:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__48KHZ;
		break;
	case 0x04:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__88_2_KHZ;
		break;
	case 0x05:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__96KHZ;
		break;
	case 0x06:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__176_4KHZ;
		break;
	case 0x07:
		drv_obj->aif_data.payload.sampling_frq =
			SII_DRV_AUDIO_SAMPLG_FRQ__192KHZ;
		break;
	}
	switch (channel_count) {
	case 0x00:
	case 0x01:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__2CHNL;
		break;
	case 0x02:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__3CHNL;
		break;
	case 0x03:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__4CHNL;
		break;
	case 0x04:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__5CHNL;
		break;
	case 0x05:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__6CHNL;
		break;
	case 0x06:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__7CHNL;
		break;
	case 0x07:
		drv_obj->aif_data.payload.channel_count =
			SII_DRV_AUDIO_CHANNEL_COUNT__8CHNL;
		break;
	}

	switch (sample_size) {
	case 0x00:
	case 0x01:
		drv_obj->aif_data.payload.sample_size =
			SII_DRV_AUDIO_SAMPLE_SIZE__16BIT;
		break;
	case 0x02:
		drv_obj->aif_data.payload.sample_size =
			SII_DRV_AUDIO_SAMPLE_SIZE__20BIT;
		break;
	case 0x03:
		drv_obj->aif_data.payload.sample_size =
			SII_DRV_AUDIO_SAMPLE_SIZE__24BIT;
		break;
	}
	return 0;
}

static int s_parse_audioclkgeneration(struct driver_obj *drv_obj,
	struct sii_info_frame *if_acr)
{
	struct acr_info_frame acr_if_data = { {0} };

	if (if_acr->if_id != SII_INFO_FRAME_ID__ACR) {
		sii_log_debug("Invalid ACR Infoframe\n");
		return 1;
	}

	/*memset(&drv_obj->acr_if_data, 0,
		sizeof(struct acr_info_frame));*/
	memset(&acr_if_data, 0,
		sizeof(struct acr_info_frame));

	memcpy(&acr_if_data.header, if_acr->b,
		sizeof(acr_if_data.header));

	if (ACRIF_TYPE != acr_if_data.header.type_code) {
		sii_log_debug("Invalid ACR type code: 0x%02x\n",
			acr_if_data.header.type_code);
		return 1;
	}

	/*please refer sec 5.3.3 of HDMI specification 14B*/
	acr_if_data.aud_params.audio_cts =
		((if_acr->b[4] & 0x0F) << 16) |
		((if_acr->b[5] & 0xFF) << 8) |
		(if_acr->b[6] & 0xFF);

	acr_if_data.aud_params.audio_n =
		((if_acr->b[7] & 0x0F) << 16) |
		((if_acr->b[8] & 0xFF) << 8) |
		(if_acr->b[9] & 0xFF);

	if (memcmp(&drv_obj->acr_if_data, &acr_if_data,
		sizeof(struct acr_info_frame))) {

		int32_t cts_diff = drv_obj->acr_if_data.aud_params.audio_cts
				- acr_if_data.aud_params.audio_cts;
		int32_t n_diff = drv_obj->acr_if_data.aud_params.audio_n
				- acr_if_data.aud_params.audio_n;
		cts_diff = cts_diff * (((cts_diff > 0) << 1) - 1);

		if ((cts_diff > 10) || (n_diff)) {
			memcpy(&drv_obj->acr_if_data, &acr_if_data,
				sizeof(struct acr_info_frame));
			return 0;
		}
	}
	/*N and CTS values same as previous values*/
	return 1;
}

#define IEEE_OUI(x)	((uint32_t)x[0] | \
			((uint32_t)x[1] << 8) | \
			((uint32_t)x[2] << 16))

static int s_parse_vsif(struct driver_obj *drv_obj,
	       struct sii_info_frame *if_vs)
{
	uint8_t checksum;
	uint32_t ieee_oui;
	uint8_t hdmi_videoformat = 0;
	uint8_t structure3d = 0;
	struct vsif_common_header *vsif_common;
	struct hdmi_vendor_specific_payload *hdmi;

	if (if_vs->if_id != SII_INFO_FRAME_ID__VS) {
		sii_log_debug("Invalid VSIF Infoframe\n");
		return 1;
	}

	/* validate checksum */
	checksum = calculate_generic_checksum(if_vs->b, 0,
		SII_INFOFRAME_MAX_LEN);

	if (0 != checksum) {
		sii_log_debug(
			"VSIF Checksum (including checksum) is: 0x%02x should be 0\n",
			checksum);
		return 1;
	}

	memset(&drv_obj->vsif_hdmi_or_mhl3, 0,
		sizeof(struct vsif_hdmi_or_mhl3));

	memcpy(&drv_obj->vsif_hdmi_or_mhl3.common, if_vs->b,
		sizeof(drv_obj->vsif_hdmi_or_mhl3.common));

	vsif_common = &drv_obj->vsif_hdmi_or_mhl3.common;
	if (VSIF_TYPE != vsif_common->header.type_code) {
		sii_log_debug("Invalid VSIF type code: 0x%02x\n",
			vsif_common->header.type_code);
		return 1;
	}

	ieee_oui = IEEE_OUI(vsif_common->ieee_oui);

	switch (ieee_oui) {
	case IEEE_OUI_HDMI:
		if (HDMI_VSIF_VERSION != vsif_common->header.version_number) {
			sii_log_debug("Invalid VSIF version: 0x%02x\n",
				vsif_common->header.version_number);
			return 1;
		}

		hdmi = &drv_obj->vsif_hdmi_or_mhl3.payload.hdmi;

		hdmi_videoformat = (if_vs->b[7] & 0xE0) >> 5;

		/*please refer sec8.2.3 of HDMI specification 14B*/
		switch (hdmi_videoformat) {
		case 0x00:
			sii_log_debug(
				"No Additional HDMI video format is present in this packet\n"
				);

			hdmi->hdmi_vid_fmt =
				HVF__NO_ADDITIONAL_HDMI_VIDEO_FORMAT_PRESENT;
			break;
		case 0x01:
			sii_log_debug("One byte of HDMI VIC value follows\n");

			hdmi->hdmi_vid_fmt =
				HVF__EXTENDED_RESOLUTION_FORMAT_PRESENT;
			hdmi->hdmi_vic =
				(if_vs->b[8] & 0xFF);
			break;
		case 0x02:
			sii_log_debug("3D format indication present\n");

			hdmi->hdmi_vid_fmt =
				HVF__3D_FORMAT_INDICATION_PRESENT;
			hdmi->_3d_meta_present = if_vs->b[8] & 0x08 >> 3;

			structure3d = (if_vs->b[8] & 0xF0) >> 4;
			switch (structure3d) {
			case 0x00:
				hdmi->_3d_structure =
					_3D_STRUCTURE__FRMPACKING;
				break;
			case 0x01:
				hdmi->_3d_structure =
					_3D_STRUCTURE__FIELDALTERNATE;
				break;
			case 0x02:
				hdmi->_3d_structure =
					_3D_STRUCTURE__LINEALTERNATE;
				break;
			case 0x03:
				hdmi->_3d_structure =
					_3D_STRUCTURE__SIDEBYSIDEFULL;
				break;
			case 0x04:
				hdmi->_3d_structure =
					_3D_STRUCTURE__LDEPTH;
				break;
			case 0x05:
				hdmi->_3d_structure =
					_3D_STRUCTURE__LDEPTHGRAPHICSDEPTH;
				break;
			case 0x06:
				hdmi->_3d_structure =
					_3D_STRUCTURE__TOPANDBOTTOM;
				break;
			case 0x08:
				hdmi->_3d_structure =
					_3D_STRUCTURE__SIDEBYSIDEHALF;
				break;
			default:
				break;
			}
			hdmi->_3d_ext_data = (enum _3d_extended_data)
				((if_vs->b[9] & 0xF0) >> 4);
			break;
		}

		break;
	case IEEE_OUI_MHL:
		if (MHL3_VSIF_VERSION != vsif_common->header.version_number) {
			sii_log_debug("Invalid VSIF version: 0x%02x\n",
				vsif_common->header.version_number);
			return 1;
		}
		break;

	default:
		sii_log_debug("Invalid IEEE OUI: 0x%06x\n", ieee_oui);
	}

	return 0;
}

static int s_parse_aviif(struct driver_obj *drv_obj,
	       struct sii_info_frame *if_avi)
{
	uint8_t checksum = 0;
	uint8_t colorimetry;
	uint8_t ext_colorimetry;
	uint8_t vid_clr_info;
	uint8_t quant;

	if (if_avi->if_id != SII_INFO_FRAME_ID__AVI) {
		sii_log_debug("Invalid Infoframe\n");
		return 1;
	}

	/* validate checksum */
	checksum = calculate_generic_checksum(if_avi->b, 0,
		SII_INFOFRAME_MAX_LEN);

	if (0 != checksum) {
		sii_log_debug(
			"AVIF Checksum (including checksum) is: 0x%02x should be 0\n",
			checksum);
		return 1;
	}

	memset(&drv_obj->avi_data, 0,
		sizeof(struct avi_info_frame));

	memcpy(&drv_obj->avi_data.header, if_avi->b,
		sizeof(drv_obj->avi_data.header));

	if (AVIF_TYPE != drv_obj->avi_data.header.type_code) {
		sii_log_debug("Invalid AVIF type code: 0x%02x\n",
			drv_obj->avi_data.header.type_code);
		return 1;
	}

	switch ((if_avi->b[4] & 0xE0) >> 5) {
	case 0x00:
		drv_obj->avi_data.payload.pb1.colorSpace = ACS__RGB;
		break;
	case 0x01:
		drv_obj->avi_data.payload.pb1.colorSpace = ACS__YCBCR422;
		break;
	case 0x02:
		drv_obj->avi_data.payload.pb1.colorSpace = ACS__YCBCR444;
		break;
	case 0x03:
		drv_obj->avi_data.payload.pb1.colorSpace = ACS__YCBCR420;
		break;
	default:
		drv_obj->avi_data.payload.pb1.colorSpace = ACS__RGB;
		break;
	}

	colorimetry = (if_avi->b[5] & 0xC0) >> 6;
	ext_colorimetry = (if_avi->b[6] & 0x70) >> 4;

	vid_clr_info = ((if_avi->b[4] & 0xE0) | (colorimetry << 3)
		| (ext_colorimetry));

	quant = (if_avi->b[6] & 0x0C) >> 2;

	/*please refer sec6.4 of draft CEA-861-F*/
	switch (vid_clr_info) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
	case 0x1C:
	case 0x10:
	case 0x08:
		if (quant & 0x01)
			drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__RGB_LIMITED;
		else if (quant & 0x02)
			drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__RGB_FULL;
		else
			drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__RGB_DEFAULT;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_601;
		break;
	case 0x1E:
		if (quant & 0x01)
			drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__RGB_LIMITED;
		else if (quant & 0x02)
			drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__RGB_FULL;
		else
			drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__RGB_DEFAULT;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_NON_CONSTANT_LUMINOUS;
		break;
	case 0x20:
	case 0x21:
	case 0x22:
	case 0x23:
	case 0x24:
	case 0x25:
	case 0x26:
	case 0x27:
	case 0x28:
	case 0x29:
	case 0x2A:
	case 0x2B:
	case 0x2C:
	case 0x2D:
	case 0x2E:
	case 0x2F:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC422_601;
		/*even BT709 can also be used till case 0x27*/
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_601;
		break;
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC422_709;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_709;
		break;
	case 0x38:
	case 0x3A:
	case 0x3B:
		drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__XVYCC422_601;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_601;
		break;
	case 0x39:
		drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__XVYCC422_709;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_709;
		break;
	case 0x3D:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC422_2020;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS;
		break;
	case 0x3E:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC422_2020;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_NON_CONSTANT_LUMINOUS;
		break;
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
	case 0x44:
	case 0x45:
	case 0x46:
	case 0x47:
	case 0x48:
	case 0x49:
	case 0x4A:
	case 0x4B:
	case 0x4C:
	case 0x4D:
	case 0x4E:
	case 0x4F:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC444_601;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_601;
		break;
	case 0x58:
	case 0x5A:
	case 0x5B:
		drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__XVYCC444_601;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_601;
		break;
	case 0x50:
	case 0x51:
	case 0x52:
	case 0x53:
	case 0x54:
	case 0x55:
	case 0x56:
	case 0x57:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC444_709;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_709;
		break;
	case 0x59:
		drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__XVYCC444_709;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_709;
		break;
		//break;
	case 0x5D:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC444_2020;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS;
		break;
	case 0x5E:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC444_2020;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_NON_CONSTANT_LUMINOUS;
		break;
	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6A:
	case 0x6B:
	case 0x6C:
	case 0x6D:
	case 0x6E:
	case 0x6F:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC420_601;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_601;
		break;
	case 0x78:
	case 0x7A:
	case 0x7B:
		drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__XVYCC420_601;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_601;
		break;
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
	case 0x74:
	case 0x75:
	case 0x76:
	case 0x77:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC420_709;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_709;
		break;
	case 0x79:
		drv_obj->avi_data.payload.clr_spc =
			SII_DRV_CLRSPC__XVYCC420_709;
		drv_obj->avi_data.payload.conv_std = SII_DRV_CONV_STD__BT_709;
		break;
	case 0x7D:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC420_2020;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS;
		break;
	case 0x7E:
		drv_obj->avi_data.payload.clr_spc = SII_DRV_CLRSPC__YC422_2020;
		drv_obj->avi_data.payload.conv_std =
			SII_DRV_CONV_STD__BT_2020_NON_CONSTANT_LUMINOUS;
		break;
	}
	drv_obj->avi_data.payload.pb4.vic = if_avi->b[7];
	return 0;
}

static void update_input_color_info(struct driver_obj *drv_obj)
{
	struct sii_drv_color_info_cfg clr_info_cfg;
	bool need_csc = false;
	if (!drv_obj->is_tpg_enabled/* && drv_obj->rx_tmds_mode*/) {
		clr_info_cfg.input_clr_spc =
			drv_obj->avi_data.payload.clr_spc;
		clr_info_cfg.input_clr_conv_std =
			drv_obj->avi_data.payload.conv_std;
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
			&clr_info_cfg.input_vid_dc_depth);
		if (drv_obj->upsampled_to_444) {
			sii_log_debug("CSC has done to 444 previously\n");
            return;
		} else {
			need_csc = false;
		}
		/*if (drv_obj->upsampled_to_444)*/

		if (need_csc)
			sii_mod_vid_path_set(drv_obj->vidpath_obj,
				SII_MOD_VIDPATH_OPCODE__OUT_CLRSPC,
				(void *)&drv_obj->out_clr_spc);
		else
			sii_mod_vid_path_set(drv_obj->vidpath_obj,
				SII_MOD_VIDPATH_OPCODE__IN_VIDINFO,
				(void *)&clr_info_cfg);
	}
}

static void s_rx_hdmi_call_back(void *p_obj, uint32_t events)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (events & SII_DRV_RX_EVENT__SCDC_EVENT) {
		uint32_t scdc_events;

		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__SCDC_EVENTS,
			(void *)&scdc_events);
		if (scdc_events & SII_DRV_RX_EVENT__SCDCS_CLK_RATIO_CHNG) {
			uint8_t clock_ratio;

			sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
				SII_MOD_RX_HDMI_OPCODE__SCDCS_CLK_RATIO,
				(void *)&clock_ratio);

			s_clock_ratio40(drv_obj, (bool)clock_ratio);
		}
	}
	if (events & SII_DRV_RX_EVENT__AVI_IF) {
		struct sii_info_frame if_avi;

		if_avi.if_id = SII_INFO_FRAME_ID__AVI;
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IF_DATA,
			&if_avi);
		if (if_avi.is_if_on) {
			if (s_parse_aviif(drv_obj, &if_avi)) {
				/* avi parsing failed */
				sii_log_debug("failed to parse avi\n");
			} else {
			    bool is_dsc_enabled;
                struct sii_info_frame if_avi_dsc;
			    sii_log_debug("avi: update_input_color_info (%d)\n", (int)drv_obj->out_clr_spc);
                sii_mod_dsc_get(drv_obj->dsc_obj, SII_MOD_DSC_OPCODE__COMPRESSED_STATUS,
                        &is_dsc_enabled);
				if (is_dsc_enabled)
				{
					// dsc is enabed already,
					// retrieve AVI infoframe from DSC module
					sii_mod_dsc_get(drv_obj->dsc_obj, SII_MOD_DSC_OPCODE__INFOFRAME_AVIF,
						&if_avi_dsc);
					if (((drv_obj->upsampled_to_444 == false) &&
						(drv_obj->avi_data.payload.pb1.colorSpace == ACS__YCBCR422))
						||
						((drv_obj->upsampled_to_444 == true) &&
						((drv_obj->avi_data.payload.pb1.colorSpace == ACS__YCBCR444) ||
							(drv_obj->avi_data.payload.pb1.colorSpace == ACS__RGB))) )
					{
						/*Toggle tx tmds, this is to handle the case of AVI change
						*without changing rx tmds, by doing this DSC will be restarted*/
						if (drv_obj->tx_tmds_enabled) {
							sii_log_debug("Toggling tx tmds for RX AVI change infoframe change\n");
							sii_drv_tx_tmds_mode_set(drv_obj, SII_TMDS_MODE__NONE);
							msleep(30); /*30ms is just for safe, no reason*/
							sii_drv_tx_tmds_mode_set(drv_obj, SII_TMDS_MODE__AUTO);
#if (CONFIG_SOURCE_ONLY == 0)
                            /*
                            it is for the issue with Oppo AVI change without clock change....
In this case we have put tx Tmds toggling, so TX HDCP will start again without disturbing RX HDCP(btwn SA and BA).... So there will not be any new STream mange msg from SA
for that we should have some flag to send the same stream mange msg
and this is code is for that
                            */
							set_need_to_send_strm_mng_msg_flag();
#endif
						}
					} else {
						uint8_t dsc_csc_info = 0;
						if (drv_obj->upsampled_to_444) {
							// this is expected and may be changed 709 to 2020 or vice versa
							// Then just update incoming infoframe 422 to 444
							// and remain 709 or 2020
							// No other changes are required
							// Change CSC from 422 to 444
							if_avi.b[4] = (if_avi.b[4] & ~0x20) | 0x40;

							dsc_csc_info = 0x02;
						} else {
							dsc_csc_info = 0x01;
						}

						/*Put EMM data in AVI version byte*/
						if_avi.b[1] &= 0xCF; /*clear bit 4 and 5*/
						if_avi.b[1] |= (dsc_csc_info << 4);

						if_avi.b[3] = 0;
						if_avi.b[3] = calculate_generic_checksum(
							if_avi.b, 0, SII_INFOFRAME_MAX_LEN);
						if_avi.is_if_on = true;

						memcpy(&drv_obj->avi_out_data, &if_avi,
							sizeof(drv_obj->avi_out_data));
					}
				}
                sii_log_debug("Set avi\n");
				sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
					SII_MOD_HDMI_TX_OPCODE__IF_DATA, &if_avi);
				update_input_color_info(drv_obj);
			}
		} else {
		    sii_log_debug("avi: Clear!!!\n");
			memset(&drv_obj->avi_data, 0,
				sizeof(struct avi_info_frame));
            sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		        SII_MOD_HDMI_TX_OPCODE__IF_DATA, &if_avi);
        }

		/*Clearing ACR data here will configs N and
		 *CTS Values in tx_counter_interrupt_handler*/
		memset(&drv_obj->acr_if_data, 0,
			sizeof(struct acr_info_frame));
	}

	if (events & SII_DRV_RX_EVENT__NO_AUDIO) {
		memset(&drv_obj->acr_if_data, 0,
			sizeof(struct acr_info_frame));
	}

	if (events & SII_DRV_RX_EVENT__VS_IF) {

		struct sii_info_frame if_vs;

		if_vs.if_id = SII_INFO_FRAME_ID__VS;
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
				SII_MOD_RX_HDMI_OPCODE__IF_DATA,
				&if_vs);
		if (if_vs.is_if_on) {
			if (s_parse_vsif(drv_obj, &if_vs)) {
				/* vsif parsing failed */
				sii_log_debug("failed to parse VSIF\n");
			}
		} else {
			memset(&drv_obj->vsif_hdmi_or_mhl3, 0,
				sizeof(struct vsif_hdmi_or_mhl3));
			;
		}

	}

	if (events & SII_DRV_RX_EVENT__AUDIO_IF) {

		struct sii_info_frame if_audio;

		if_audio.if_id = SII_INFO_FRAME_ID__AUDIO;
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
				SII_MOD_RX_HDMI_OPCODE__IF_DATA,
				&if_audio);
		if (if_audio.is_if_on) {
			s_parse_audioif(drv_obj, &if_audio);
			;
		} else {
			memset(&drv_obj->aif_data, 0,
				sizeof(struct audio_info_frame));
			;
		}

		/*Clearing ACR data here will configs N and
		 *CTS Values in tx_counter_interrupt_handler*/
		memset(&drv_obj->acr_if_data, 0,
			sizeof(struct acr_info_frame));
	}

	if (events & SII_DRV_RX_EVENT__TMDS_MODE_CHNG) {
		/* call rx_tmds_mode change handler */
		s_rx_tmds_mode_change_handler(drv_obj);

        // Reset output color space to default PASS THROUGH.
		drv_obj->is_420toany_conv_en = false;
        sii_drv_output_color_space_set(drv_obj, SII_DRV_CLRSPC__PASSTHRU);
	}

	if (events & SII_DRV_RX_EVENT__RX_CONN_CHNG) {
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__RPWR,
			(void *)&drv_obj->b_rx_hdmi_connected);

			if (drv_obj->b_rx_hdcp_enabled) {
				drv_obj->b_rx_hdcp_enabled = false;
				s_update_drv_state(drv_obj);
			}
			if (drv_obj->b_rx_tmds_enabled) {
				drv_obj->b_rx_tmds_enabled = false;
				s_update_drv_state(drv_obj);
			}
		s_update_drv_state(drv_obj);
	}
#ifdef CONFIG_HDCP_REPEATER_EN
	if (events & SII_DRV_RX_EVENT__AUTH_CHNG) {

		sii_drv_rx_hdcp_status_get(drv_obj, &drv_obj->rx_hdcp_status);
		/* get hdcp status from hdcp module*/
		sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__STATUS,
			&drv_obj->tx_hdcp_status);
		if ((drv_obj->rx_hdcp_status
			== SII_DRV_HDCP_STATUS__SUCCESS_1X)
			|| (drv_obj->rx_hdcp_status
				== SII_DRV_HDCP_STATUS__SUCCESS_22)) {

			sii_log_debug("RX HDCP AUTHENTICATED\n");
			drv_obj->b_rx_hdcp_enabled = true;
			s_update_drv_state(drv_obj);
		} else {
			if (SII_DRV_HDCP_STATUS__FAILED
				== drv_obj->rx_hdcp_status)
				sii_log_debug("RX HDCP AUTH FAILED\n");
			else if (SII_DRV_HDCP_STATUS__OFF
				== drv_obj->rx_hdcp_status)
				sii_log_debug("RX HDCP AUTH OFF\n");
			drv_obj->b_rx_hdcp_enabled = false;
			s_update_drv_state(drv_obj);
		}
	}

#if 0
    // Banner doesn't support HDCP2.2 Rx in hardware
	if (events & SII_DRV_RX_EVENT__SKE_EVENT) {
		if (drv_obj->b_tx_hdmi_connected) {
			drv_obj->b_rx_hdcp_enabled = true;
			s_update_drv_state(drv_obj);
		}
	}

	if (events & SII_DRV_RX_EVENT__HDCP2X_CONTENT_TYPE_CHNG) {
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__STMNG,
			(struct sii_drv_tx_strmng_2x_msg *)&drv_obj->stmng_dat.stream_k);
		/* set av_mute*/
		sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__STMNG_SET,
			(struct sii_drv_tx_strmng_2x_msg *)&drv_obj->stmng_dat.stream_k);
	}
#endif
#endif //CONFIG_HDCP_REPEATER_EN
	/*if (events & SII_DRV_RX_EVENT__DC_CHNG) {
		enum sii_drv_bit_depth input_depth;
		sii_log_debug("RX Deep colour changed\n");
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
			&input_depth);
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH,
				&input_depth);
	}*/
}

static void s_vid_path_call_back(void *p_obj, uint32_t events)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

#ifndef CONFIG_SWAY_CONFIG_POST_CONV
	if (events & SII_DRV_RX_VIDPATH_EVENT__REQ_REF_CLK) {
		bool sway_pll_enable;

		sii_mod_vid_path_get(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__ENABLE_REF_CLOCK,
				&sway_pll_enable);
		sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
				SII_MOD_RX_HDMI_OPCODE__INPUT_420_EN,
				&sway_pll_enable);
		if (sway_pll_enable) {
			s_sway_pll_configure(drv_obj,
				SWAY_INUSE_FOR__420_422, 2, 0);
			;
		} else {
			/* Intimate Output TMDS clock to Tx module */
			s_sway_pll_enable(drv_obj, false);
			update_tx_tmds_clock(drv_obj, 0);
			;
		}
	}

	else if (events & SII_DRV_RX_VIDPATH_EVENT__COLOR_CONV_DONE) {
#else
	if (events & SII_DRV_RX_VIDPATH_EVENT__COLOR_CONV_DONE) {
#endif
		{
			enum vixs_fmt vixs_id = VIXS_FMT__TOTAL_FMTS;
#ifdef CONFIG_SWAY_CONFIG_POST_CONV
			bool sway_pll_enable;
#endif

			if (s_is_vixs_format_detected(drv_obj, &vixs_id)) {
				/* VIXS pass through does not need VTG
				 * configuration*/
				if (!drv_obj->b_is_vixs_format_handled) {
					drv_obj->b_is_vixs_format_handled =
						true;
					sii_mod_vid_path_set(
						drv_obj->vidpath_obj,
					SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS,
					&s_vtg_timing_params);
					sii_mod_vid_path_set(
						drv_obj->vidpath_obj,
					SII_MOD_VIDPATH_OPCODE__VTG_ENABLE,
					&drv_obj->b_is_vixs_format_handled);
				}
			} else {
				drv_obj->b_is_vixs_format_handled = false;
				sii_mod_vid_path_set(drv_obj->vidpath_obj,
					SII_MOD_VIDPATH_OPCODE__VTG_ENABLE,
					&drv_obj->b_is_vixs_format_handled);
			}
#ifdef CONFIG_SWAY_CONFIG_POST_CONV
			sii_mod_vid_path_get(drv_obj->vidpath_obj,
				SII_MOD_VIDPATH_OPCODE__ENABLE_REF_CLOCK,
					&sway_pll_enable);
			if (sway_pll_enable) {
				sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
					SII_MOD_RX_HDMI_OPCODE__INPUT_420_EN,
					&sway_pll_enable);
				if (VIXS_FMT__TOTAL_FMTS == vixs_id) {
					s_sway_pll_configure(drv_obj,
						SWAY_INUSE_FOR__420_422, 2, 0);
				} else {
					s_sway_pll_configure(drv_obj,
						SWAY_INUSE_FOR__420_422, 0,
						vixs_fmt_out_pixel_clk_map
							[vixs_id][1]);
				}
			} else {
				s_sway_pll_enable(drv_obj, false);
				;
			}
#else
			/*s_sway_pll_enable(drv_obj, true);*/
#endif
		}
		if (drv_obj->b_rx_hdmi_connected) {
			struct sii_info_frame iframe;
			struct sii_info_frame acr_if;
			uint8_t chksum = 0;
			//uint8_t avi_data_byte1, avi_data_byte2, avi_data_byte3;

			iframe.if_id = SII_INFO_FRAME_ID__AVI;
			acr_if.if_id = SII_INFO_FRAME_ID__ACR;
			sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
				SII_MOD_RX_HDMI_OPCODE__IF_DATA, &iframe);
			sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
				SII_MOD_RX_HDMI_OPCODE__IF_DATA, &acr_if);
			//avi_data_byte1 = iframe.b[4];
			//avi_data_byte2 = iframe.b[5];
			//avi_data_byte3 = iframe.b[6];

			switch (drv_obj->out_clr_spc) {
			default:
			case SII_DRV_CLRSPC__PASSTHRU:
				memset(&drv_obj->avi_out_data, 0,
					sizeof(struct sii_info_frame));
				break;
			case SII_DRV_CLRSPC__RGB_DEFAULT:
				/*set color space*/
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x00 << 5));
				/*set colorimetry*/
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x00);
				/*set extended colorimetry*/
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				/*set quantization level*/
				iframe.b[6] = ((iframe.b[6] & ~0x0c) | 0x00);
				break;
			case SII_DRV_CLRSPC__RGB_FULL:
				/*set color space*/
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x00 << 5));
				/*set colorimetry*/
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x00);
				/*set extended colorimetry*/
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				/*set quantization level*/
				iframe.b[6] = ((iframe.b[6] & ~0x0c) | 0x08);
				break;
			case SII_DRV_CLRSPC__RGB_LIMITED:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x00 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x00);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				iframe.b[6] = ((iframe.b[6] & ~0x0c) | 0x04);
				break;
			case SII_DRV_CLRSPC__XVYCC420_601:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x07 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				break;
			case SII_DRV_CLRSPC__XVYCC420_709:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x07 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x10);
				break;
			case SII_DRV_CLRSPC__XVYCC422_601:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x01 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				break;
			case SII_DRV_CLRSPC__XVYCC422_709:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x01 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x10);
				break;
			case SII_DRV_CLRSPC__XVYCC444_601:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x02 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				break;
			case SII_DRV_CLRSPC__XVYCC444_709:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x02 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x10);
				break;
			case SII_DRV_CLRSPC__YC420_2020:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x07 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x50);
				break;
			case SII_DRV_CLRSPC__YC420_601:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x07 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x40);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				break;
			case SII_DRV_CLRSPC__YC420_709:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x07 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x80);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x10);
				break;
			case SII_DRV_CLRSPC__YC422_2020:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x01 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x50);
				break;
			case SII_DRV_CLRSPC__YC422_601:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x01 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x40);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				break;
			case SII_DRV_CLRSPC__YC422_709:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x01 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x80);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x10);
				break;
			case SII_DRV_CLRSPC__YC444_2020:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x02 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0xc0);
#ifndef CSC_AS_PER_INPUT_STANDARD
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x50);
#endif
				break;
			case SII_DRV_CLRSPC__YC444_601:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x02 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x40);
				iframe.b[6] = ((iframe.b[6] & ~0x70) | 0x00);
				break;
			case SII_DRV_CLRSPC__YC444_709:
				iframe.b[4] = ((iframe.b[4] & ~0xe0) |
					(0x02 << 5));
				iframe.b[5] = ((iframe.b[5] & ~0xc0) | 0x80);
				break;
			}

			if (drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic)
				iframe.b[7] = sii_map_hdmi_vic_to_cea_vic(
						drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic);

			iframe.b[3] = 0;
			chksum = calculate_generic_checksum(iframe.b, 0,
					SII_INFOFRAME_MAX_LEN);
			iframe.b[3] = chksum;

			if (drv_obj->out_clr_spc != SII_DRV_CLRSPC__PASSTHRU) {
				iframe.is_if_on = true;
				if (drv_obj->is_420toany_conv_en)
					acr_if.is_if_on = true;
			} else {
				iframe.is_if_on = false;
				acr_if.is_if_on = false;
			}

			memcpy(&drv_obj->avi_out_data, &iframe,
				sizeof(struct sii_info_frame));
			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__IF_DATA,
				&iframe);
			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__IF_DATA,
				&acr_if);
		}
	}

	if (events & SII_DRV_RX_VIDPATH_EVENT__HSYNC_HIGH_CNT_CHG) {
		uint32_t input_tmds_clk = s_get_input_tmds_clock(drv_obj);
		if (input_tmds_clk <= 340000000) {
			drv_obj->input_unstable_counter = 0;
			return;
		}
		drv_obj->input_unstable_counter++;
		if (drv_obj->input_unstable_counter > 32) {
			drv_obj->input_unstable_counter = 0;
			sii_log_debug("Error: Input is Not stable\n");
			/* Input is not stable. Toggle HPD*/
			s_rx_hot_plug_toggle(p_obj);
		}
	}

}

static void s_clock_ratio40(struct driver_obj *drv_obj, bool on_off)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	/*s_rx_zone_enable(p_obj, false);*/
	/*SW reset to RX Zone control loogic*/
	sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG5,
		BIT_MSK__SW_RST_REG5__REG_RXZONE_SW_RST);

	s_rx_manual_zone_enable(drv_obj, on_off);

	sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG5,
		BIT_MSK__SW_RST_REG5__REG_RXZONE_SW_RST);
	if (on_off)
		sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_BW_CFG1, 0x88);
	else
		sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_BW_CFG1, 0x22);

	/*s_rx_zone_enable(p_obj, true);*/
}

#ifdef CONFIG_ENHANCED_MODE_PROT
static int transmit_dsc_vsif_packets(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	struct sii_info_frame  if_vsif;
	uint8_t *p;
	int status = 0, i = 0;

	sii_platform_set_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
		BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT);
	for (i = 0; i < 4; i++) {
		/*get vsifinfoframe updated values*/
		if_vsif.if_id = (enum sii_info_frame_id)
			(SII_INFO_FRAME_ID__VSIF1 + i);
		status = sii_mod_dsc_get(drv_obj->dsc_obj,
			SII_MOD_DSC_OPCODE__INFOFRAME_DATA,
			(void *)&(if_vsif));
		if (status) {
			sii_log_err("failed to get DSC VSIFs\n");
			return status;
		}
		if_vsif.is_if_on = true;
		/*send updated vsifinfoframe values to downstream*/
		status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__IF_DATA,
			(void *)&(if_vsif));
		if (status) {
			sii_log_err("failed to set DSC VSIFs\n");
			return status;
		}
		sii_log_print("HDMI DSC VSIF[%d]: ", i);
		p = if_vsif.b;
		s_dump_buffer((uint8_t *)p, SII_INFOFRAME_MAX_LEN);
	}
	return status;
}
#endif

/* tx driver's callback function - called by internal modules to notify
 respective events.*/
void s_tx_hdmi_call_back(void *p_obj, uint32_t event_flags)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (event_flags & SII_DRV_TX_EVENT__OUTPUT_TMDS_CHNG_REQ) {
		drv_obj->tx_tmds_enabled = false;
	drv_obj->is_emc_queried = false;
	drv_obj->ds_em_caps = 0;
	drv_obj->emm_response_received = false;
	drv_obj->emm_response = 0;
	drv_obj->upsampled_to_444 = false;
	drv_obj->upsampled_to_422 = false;

		sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
			&drv_obj->tx_tmds_enabled);

		if (drv_obj->tx_tmds_enabled) {
			enum avi_color_space ip_clr_spc =
				drv_obj->avi_data.payload.pb1.colorSpace;
			drv_obj->b_tx_tmds_on_req = true;
			s_tx_zone_enable(drv_obj, true);
			if (ip_clr_spc != ACS__YCBCR420)
				sii_drv_output_color_space_set(drv_obj,
				SII_DRV_CLRSPC__PASSTHRU);
		} else {
			if (drv_obj->is_dsc_enabled) {
				sii_log_debug("disabling dsc\n");
				sii_drv_tx_dsc_enable(drv_obj, false);
			}
			if (drv_obj->is_420toany_conv_en) {
				drv_obj->is_420toany_conv_en = false;
				sii_drv_output_color_space_set(drv_obj,
					SII_DRV_CLRSPC__PASSTHRU);
			}
			s_tx_zone_enable(drv_obj, false);
			drv_obj->b_tx_tmds_on_req = false;
			s_tx_tmds_enable(drv_obj, false);


		}
	}
#ifndef CONFIG_HDCP_REPEATER_EN
	if (event_flags & SII_DRV_TX_EVENT__HDMI_STATE_CHNG) {
#ifdef CONFIG_ENHANCED_MODE_PROT
		if (drv_obj->tx_tmds_enabled
			&& !(is_ds_device_spiderman2(drv_obj)))
			s_tx_hdcp_handler(drv_obj);
		else if (!drv_obj->tx_tmds_enabled)
			s_tx_hdcp_handler(drv_obj);
#else
		s_tx_hdcp_handler(drv_obj);
#endif
	}
#endif

	if (event_flags & SII_DRV_TX_EVENT__RSEN_CHNG) {
		sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__RSEN,
			&drv_obj->tx_rsen);
	}

	if (event_flags & SII_DRV_TX_EVENT__HOT_PLUG_CHNG) {
		sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__HOTPLUG,
			&drv_obj->b_tx_connected);

#ifdef CONFIG_ENHANCED_MODE_PROT
		if (!drv_obj->b_tx_connected) {
			memset(&drv_obj->manf_spec_data,
				0, sizeof(drv_obj->manf_spec_data));
			drv_obj->is_emc_queried = false;
			drv_obj->emm_response_received = false;
			drv_obj->ds_em_caps = 0;
			drv_obj->emm_response = 0;
			drv_obj->upsampled_to_444 = false;
			drv_obj->upsampled_to_422 = false;
		}
#endif
		s_tx_hpd_handler(drv_obj);
	}

#ifdef CONFIG_ENHANCED_MODE_PROT
	if (event_flags & SII_DRV_TX_EVENT__SCDC_EM_CAP_CHG) {
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
		struct sii_info_frame em_vsif /*= {0}*/;
#endif
		uint8_t vic = 0;
		uint8_t hdmi_vic = 0;
		sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__DS_EM_CAPS,
			&drv_obj->ds_em_caps);
		drv_obj->is_emc_queried = true;

#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
        memset(&em_vsif,0,sizeof(struct sii_info_frame));
		/* stop EM VSIF */
		em_vsif.if_id = SII_INFO_FRAME_ID__VSIF4;
		em_vsif.is_if_on = false;
		sii_log_debug("stopping emc vsif\n");
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__IF_DATA, &em_vsif);
#endif
		vic = drv_obj->avi_data.payload.pb4.vic;
		hdmi_vic = drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;

		if (!(drv_obj->is_dsc_enabled)
			&& (drv_obj->output_tmds_clk <= 340000000)) {
			switch_to_non_dsc_mode(drv_obj);
		} else {
			if ((vic == 97) || (vic == 107)) {
				if (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K60) {

					//msleep(100);
					switch_to_dsc_mode(drv_obj,
						drv_obj->dsc_compression);
				} else {
					sii_log_debug("DSC: 4K60 is not supported\n");
					sii_drv_tx_dsc_enable(drv_obj, false);
				}
			} else if ((vic == 96) || (vic == 106)) {
				if (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K50) {

					//msleep(100);
					switch_to_dsc_mode(drv_obj,
						drv_obj->dsc_compression);
				} else {
					sii_log_debug("DSC: 4K50 is not supported\n");
					sii_drv_tx_dsc_enable(drv_obj, false);
				}
			} else if ((vic == 95) || (hdmi_vic == 1)) {
				if (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K30) {

					//msleep(100);
					switch_to_dsc_mode(drv_obj,
						drv_obj->dsc_compression);
				} else {
					sii_log_debug("DSC: 4K30 is not supported\n");
					sii_drv_tx_dsc_enable(drv_obj, false);
				}
			} else if ((vic == 94) || (hdmi_vic == 2)) {
				if (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K25) {

					//msleep(100);
					switch_to_dsc_mode(drv_obj,
						drv_obj->dsc_compression);
				} else {
					sii_log_debug("DSC: 4K25 is not supported\n");
					sii_drv_tx_dsc_enable(drv_obj, false);
				}
			} else if ((vic == 93) || (hdmi_vic == 3)) {
				if (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K24) {

					//msleep(100);
					switch_to_dsc_mode(drv_obj,
						drv_obj->dsc_compression);
				} else {
					sii_log_debug("DSC: 4K24 is not supported\n");
					sii_drv_tx_dsc_enable(drv_obj, false);
				}
			}
		}
	}

	if (event_flags & SII_DRV_TX_EVENT__SCDC_EMM_RESP_CHG) {
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
		struct sii_info_frame em_vsif /*= {0}*/;
#endif
		sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__DS_EMM_RESP,
			&drv_obj->emm_response);
		drv_obj->emm_response_received = true;

#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
        memset(&em_vsif,0,sizeof(struct sii_info_frame));
		/* stop EM VSIF */
		em_vsif.if_id = SII_INFO_FRAME_ID__VSIF4;
		em_vsif.is_if_on = false;
		sii_log_debug("stopping emm vsif\n");
    		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
    			SII_MOD_HDMI_TX_OPCODE__IF_DATA, &em_vsif);
#endif
		if (drv_obj->emm_response & EMM_REQ_SUCCESS) {
			switch (drv_obj->dsc_mode) {
			case EMM_HDMI2_NORMAL:
				s_tx_hdcp_enable(drv_obj, true);
				break;
			case EMM_HDMI2_DSC_4K50_2T1:
			case EMM_HDMI2_DSC_4K50_3T1:
			case EMM_HDMI2_DSC_4K60_2T1:
			case EMM_HDMI2_DSC_4K60_3T1:
			case EMM_HDMI2_DSC_4K30_2T1:
			case EMM_HDMI2_DSC_4K30_3T1:
			case EMM_HDMI2_DSC_4K25_2T1:
			case EMM_HDMI2_DSC_4K25_3T1:
			case EMM_HDMI2_DSC_4K24_2T1:
			case EMM_HDMI2_DSC_4K24_3T1:
				transmit_dsc_vsif_packets(drv_obj);
				s_tx_hdcp_enable(drv_obj, true);
				break;
			default:
				break;
			}
		} else if (drv_obj->emm_response & EMM_REQ_FAILURE) {
			switch (drv_obj->dsc_mode) {
			case EMM_HDMI2_NORMAL:
				break;
			case EMM_HDMI2_DSC_4K50_2T1:
			case EMM_HDMI2_DSC_4K50_3T1:
			case EMM_HDMI2_DSC_4K60_2T1:
			case EMM_HDMI2_DSC_4K60_3T1:
			case EMM_HDMI2_DSC_4K30_2T1:
			case EMM_HDMI2_DSC_4K30_3T1:
			case EMM_HDMI2_DSC_4K25_2T1:
			case EMM_HDMI2_DSC_4K25_3T1:
			case EMM_HDMI2_DSC_4K24_2T1:
			case EMM_HDMI2_DSC_4K24_3T1:
				if (drv_obj->is_dsc_enabled) {
					sii_drv_tx_dsc_enable(drv_obj, false);
					handle_enhanced_mode(drv_obj);
				}
				break;
			default:
				break;
			}
		}
	}

	if (event_flags & SII_DRV_TX_EVENT__SCDC_EM_NO_RESP) {
		struct sii_info_frame em_vsif;// = {0};
		memset(&em_vsif,0,sizeof(struct sii_info_frame));
		em_vsif.if_id = SII_INFO_FRAME_ID__VS;
		em_vsif.is_if_on = false;
		sii_log_debug("stopping emc/emm vsif\n");
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__IF_DATA, &em_vsif);

		s_tx_hdcp_enable(drv_obj, true);
	}
#endif
}

static void s_tx_counter_handler(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	struct sii_info_frame acr_if;
	int status = 0;

	sii_platform_clr_bit8(pstDev, REG_ADDR__SYS_CTRL3,
		BIT_MSK__SYS_CTRL3__REG_SYS_CNTR);

	if ((drv_obj->is_dsc_enabled) ||
		(drv_obj->is_420toany_conv_en))
		sii_platform_set_bit8(pstDev, REG_ADDR__SYS_CTRL3,
		BIT_MSK__SYS_CTRL3__REG_SYS_CNTR);

	memset(&acr_if, 0, sizeof(struct sii_info_frame));
	acr_if.if_id = SII_INFO_FRAME_ID__ACR;
	sii_platform_block_read8(pstDev, REG_ADDR__RX_ACR_HEADER0,
			acr_if.b, SII_INFOFRAME_MAX_LEN);

	if (memcmp(&drv_obj->acr_if.b, &acr_if.b, SII_INFOFRAME_MAX_LEN)) {
		memcpy(&drv_obj->acr_if,
			&acr_if, sizeof(struct sii_info_frame));
		status = s_parse_audioclkgeneration(drv_obj, &acr_if);

		if (status == 0) {
			/*sii_log_debug("ACR: CTS = %ld, N = %ld\n",
				drv_obj->acr_if_data.aud_params.audio_cts,
				drv_obj->acr_if_data.aud_params.audio_n);*/

			acr_if.is_if_on = true;
			status = s_get_input_audio_sampling_frequency(drv_obj);
			if (status == 0) {
				s_audio_n_cts_configure(drv_obj);
			} else {
				/*should not reach here*/
				sii_log_err("failed to get i/p audio sampling frequency\n");
				sii_log_err("unable to configure N value\n");
			}

			/*drop ACR packets to downstream*/
			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__IF_DATA,
					(void *)&(acr_if));
		}
	}
}

/* tx driver's HDCP callback function - called by HDCP module to notify
 events.*/
void s_tx_hdcp_call_back(void *p_obj, uint32_t event_flags)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	/*enum sii_drv_hdcp_link_version ds_hdcp_ver;*/
#if (CONFIG_SOURCE_ONLY == 0)
	uint32_t processed_events;
#endif

	/*Below if block is only for Lattice external repeater implementation
	 *with SPiderman A host code. All these HDCP events will be
	 *handled in Spiderman host code to work spiderman A + Banner
	 *as a external HDCP repeater.*/
	/*Valens has to handle these events to work SA+Banner as a
	 *hdcp Repeater. See SA Host code handling of these events
	 *for reference*/
#if (CONFIG_SOURCE_ONLY == 0)
		drv_obj->tx_hdcp_events |=
			(enum sii_drv_hdcp_gen_event)event_flags;
#endif // (CONFIG_SOURCE_ONLY == 0)

	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_STATUS_CHNG) {

		/* get hdcp status from hdcp module*/
		sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__STATUS,
			&drv_obj->tx_hdcp_status);

		/* get DS hdcp version from hdcp module*/
		/*sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__VERSION, &ds_hdcp_ver);*/

		/*sii_log_debug("TX_HDCP_STATUS: %d\n",
			drv_obj->tx_hdcp_status);*/
		/* update tx hdcp status in hdmi module*/
		/*sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__HDCP_STATUS,
			&drv_obj->tx_hdcp_status);*/

		if ((drv_obj->tx_hdcp_status == SII_DRV_HDCP_STATUS__SUCCESS_1X)
			|| (drv_obj->tx_hdcp_status
				== SII_DRV_HDCP_STATUS__SUCCESS_22)) {
			sii_log_info("HDCP PASS\n");
			/*sii_drv_tx_av_mute_set(drv_obj, false);*/
		} else if (drv_obj->tx_hdcp_status
			== SII_DRV_HDCP_STATUS__FAILED) {
			sii_log_err("HDCP FAIL\n");
			/*sii_drv_tx_av_mute_set(drv_obj, true);*/
		}

		/*if (drv_obj->tx_hdcp_status == SII_DRV_HDCP_STATUS__OFF) {
			bool ddc_clear = false;

			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__DDC_RESET, &ddc_clear);
		}*/
	}
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_RESET) {

		sii_log_debug("TX_HDCP: TX_MOD_HDCP_EVENT__HDCP_FAILURE\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_RESET);
	}
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED) {

		sii_log_debug("TX_HDCP: RX_MOD_HDCP_STATUS__BSTATUS_CHANGE\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_BSTATUS_CHANGED);
	}
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_BSTATUS_PROCESSED) {

		sii_log_debug("TX_HDCP: RX_MOD_HDCP_STATUS__BSTATUS_UPDATED\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_BSTATUS_PROCESSED);
	}

#ifdef CONFIG_HDCP_REPEATER_EN
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED) {

		sii_log_debug("TX_HDCP: RX_MOD_HDCP__BKSV_UPDATED\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_BKSV_CHANGED);
	}
#endif
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_AUTHENTICATED) {

		sii_log_debug("TX_HDCP: rRX_MOD_HDCP__AUTHENTICATED\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_AUTHENTICATED);
	}
#ifdef CONFIG_HDCP_REPEATER_EN
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED) {

		sii_log_debug("TX_HDCP: RX_MOD_HDCP2.2__RCVID_CHANGED\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED);
	}
#endif
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_SKE_EVENT) {

		sii_log_debug("TX_HDCP: TX_MOD_HDCP2.2_SKE_SENT_EVENT\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_SKE_EVENT);
	}
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_RESET_FLAGS) {

		sii_log_debug("TX_HDCP: TX_MOD_HDCP2.2__HDCP_RESET\n");
		s_mod_handle_tx_hdcp_events(drv_obj,
			SII_DRV_HDCP_EVENT__HDCP_RESET_FLAGS);
	}
#if (CONFIG_SOURCE_ONLY == 0)
	processed_events = sii_tx_hdcp_event_handler(drv_obj->tx_hdcp_events);
	drv_obj->tx_hdcp_events &= !processed_events;
#endif // (CONFIG_SOURCE_ONLY == 0)
}

static void s_update_edid(struct driver_obj *drv_obj)
{
	bool read_edid_done = false;
    struct sii_edid stds_edid;
    struct sii_edid stcomposed_edid;
    struct sii_lib_edid_parsed_data stparsed_dev_edid;
    struct sii_lib_edid_parsed_data stparsed_ds_edid;
	struct sii_edid *ds_edid = &stds_edid;
	struct sii_edid *composed_edid = &stcomposed_edid;
	struct sii_lib_edid_parsed_data *parsed_dev_edid = &stparsed_dev_edid;
	struct sii_lib_edid_parsed_data *parsed_ds_edid = &stparsed_ds_edid;

    memset(&stds_edid,0,sizeof(struct sii_edid));
    memset(&stcomposed_edid,0,sizeof(struct sii_edid));
    memset(&stparsed_dev_edid,0,sizeof(struct sii_lib_edid_parsed_data));
    memset(&stparsed_ds_edid,0,sizeof(struct sii_lib_edid_parsed_data));

	if (!ds_edid || !composed_edid || !parsed_dev_edid || !parsed_ds_edid) {
		pr_err("Memory could not be allocated in update edid\n");
		return;
	}

	if (!sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__EDID, ds_edid))
		read_edid_done = true;
	if (read_edid_done) {
		enum sii_lib_edid_err_code_t edid_error =
			SII_LIB_EDID_ERR_CODE__NO_ERROR;
		/* Parse Downstream EDID */
		edid_error = sii_lib_edid_parse(parsed_ds_edid, (ds_edid->b));

		if (edid_error)
			goto exit;
#ifdef CONFIG_ENHANCED_MODE_PROT
#ifndef REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
		/* Get SCDC Manufacture Specific Data */
		if (parsed_ds_edid->hfvsdb.b_scdc_present) {

			sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_MANF_STATUS,
				&drv_obj->manf_spec_data);
		} else {
			memset(&drv_obj->manf_spec_data,
				0, sizeof(drv_obj->manf_spec_data));
		}
#endif
#endif
		/* Parse Local EDID */
		edid_error = sii_lib_edid_parse(parsed_dev_edid,
				banner_edid);
		if (edid_error) {
			sii_log_debug("\nError: Local EDID.\n");
			drv_obj->edid.size = ds_edid->size;
			memcpy(drv_obj->edid.b, ds_edid->b,
				drv_obj->edid.size);
		} else {
			parsed_dev_edid->product_details[3] =
				(drv_obj->chip_id >> 8) & 0xFF;
			parsed_dev_edid->product_details[2] =
				(drv_obj->chip_id) & 0xFF;

#if 0//def CONFIG_DEFINED_MACRO
			/* Compose EDID based on Local and DS EDID */
			sii_lib_edid_compose(parsed_dev_edid,
					parsed_ds_edid,
					composed_edid->b);
			composed_edid->size = (composed_edid->b[0x7E] *
					SII_EDID_BLOCK_SIZE)
					+ SII_EDID_BLOCK_SIZE;
#else
			composed_edid->size = ds_edid->size;
			memcpy(composed_edid->b, ds_edid->b, ds_edid->size);
#endif
			drv_obj->edid.size = composed_edid->size;
			memcpy(drv_obj->edid.b, composed_edid->b,
				composed_edid->size);
		}

		/* Parse Composed EDID */
		sii_lib_edid_parse(&drv_obj->parse_edid, drv_obj->edid.b);
		drv_obj->b_ds_edid_available = true;
		goto exit_2;
	}
exit:
	sii_log_err("Failed to read DS EDID or Invalid EDID\n");
	sii_log_err("set Default EDID\n");
	/* set the default (DVI) edid.*/
	drv_obj->edid.size = ARRAY_SIZE(default_ds_edid);
	memcpy(drv_obj->edid.b, default_ds_edid, drv_obj->edid.size);
	/* Parse default EDID */
	sii_lib_edid_parse(&drv_obj->parse_edid, drv_obj->edid.b);
	drv_obj->b_ds_edid_available = false;

exit_2:
	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__EDID,
		&(drv_obj->edid));


}

static void s_tx_hpd_handler(struct driver_obj *drv_obj)
{
	sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__UPDATE_HPD_STATUS,
		&drv_obj->b_tx_connected);
	s_update_drv_state(drv_obj);
}

enum sii_tmds_mode s_get_ds_tmds_mode(struct driver_obj *drv_obj)
{
	enum sii_tmds_mode tx_tmds_mode = SII_TMDS_MODE__NONE;

	if (drv_obj->b_ds_edid_available) {
		/* HDMI1.4 VSDB */
		if ((drv_obj->parse_edid.ieee_id == 0x000C03)
			|| (drv_obj->parse_edid.ieee_id == 0xC45DD8)
			/* HDMIforum VSDB */) {

			/*HDMI1 supports upto 300 m_hz and HDMI2 supports upto
			 * 600 m_hz*/
			if (drv_obj->parse_edid.hfvsdb.vclk_mb > 300) {
				tx_tmds_mode = SII_TMDS_MODE__HDMI2;
				sii_log_debug("Tmds Mode : HDMI2\n");
			} else {
				tx_tmds_mode = SII_TMDS_MODE__HDMI1;
				sii_log_debug("Tmds Mode : HDMI1\n");
			}
		} else {
			tx_tmds_mode = SII_TMDS_MODE__DVI;
			sii_log_debug("Tmds Mode : DVI\n");
		}
	} else {
		/* setting HDMI1 default*/
		tx_tmds_mode = SII_TMDS_MODE__HDMI1;
		sii_log_debug("Setting Tx Tmds Mode to HDMI1(Default)\n");
	}

	return tx_tmds_mode;
}

static void s_dump_buffer(uint8_t *p_data, uint16_t len)
{
	int i = 0;

	sii_log_print("\n");
	while (len--) {
		sii_log_print("%02x ", *p_data);
		p_data++;
		if (++i == 0x10) {
			sii_log_print("\n");
			i = 0;
		}

	}
	sii_log_print("\n");
}

static void s_tx_hdcp_enable(struct driver_obj *drv_obj, bool enable)
{
	if (enable && drv_obj->tx_tmds_enabled)
		drv_obj->hdcp_arbitration_mask |= HDCP_ENABLE__APP;
	else if (!enable)
		drv_obj->hdcp_arbitration_mask &= ~HDCP_ENABLE__APP;
	else
		drv_obj->hdcp_arbitration_mask &= ~HDCP_ENABLE__APP;

		s_tx_hdcp_prot_set(drv_obj);
}


#ifndef CONFIG_HDCP_REPEATER_EN
void s_tx_hdcp_handler(struct driver_obj *drv_obj)
{
	enum sii_drv_hdmi_tx_external_state hdmi_state =
		SII_DRV_HDMI_EXT_STATUS__TMDS_OFF;

	/* get HDMI external state*/
	sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__HDMI_STATE, &hdmi_state);

	if (hdmi_state == SII_DRV_HDMI_EXT_STATUS__TMDS_ON)
		drv_obj->b_tx_is_hdcp_req = true;
	else if (hdmi_state == SII_DRV_HDMI_EXT_STATUS__TMDS_OFF)
		s_tx_hdcp_enable(drv_obj, false);
}
#endif

static void s_rx_pll_lock_monitor(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	s_rx_pll_lock_poll(drv_obj);
	sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__RX_PLL_LOCK,
		&drv_obj->b_rx_pll_lock_status);
	if (!drv_obj->b_rx_pll_lock_status) {
		sii_log_err("RX PLL lock lost\n");

		if (drv_obj->b_rx_hdcp_enabled) {
			drv_obj->b_rx_hdcp_enabled = false;
			s_update_drv_state(drv_obj);
		}
		if (drv_obj->b_rx_tmds_enabled) {
			drv_obj->b_rx_tmds_enabled = false;
			s_update_drv_state(drv_obj);
		}
	} else {
		uint8_t rx_zone_value = sii_platform_rd_reg8(pstDev,
					REG_ADDR__RX_ZONE_STATUS_0);
		sii_log_info("RX PLL locked\n");
		sii_log_debug("zone value: 0x%02X\n", rx_zone_value);
		/*let rx settle*/
		msleep(100);
	}
	/* Enabiling/disabling videopath interrupts based
	 * on the rx_pll_lock status*/
	sii_mod_vid_path_intr_mask_enable(drv_obj->vidpath_obj,
		drv_obj->b_rx_pll_lock_status);
}
#ifdef CONFIG_ENHANCED_MODE_PROT
static int switch_to_non_dsc_mode(struct driver_obj *drv_obj)
{
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
	enum avi_color_space colorspace =
			drv_obj->avi_data.payload.pb1.colorSpace;
	uint8_t mode = 0;

	drv_obj->dsc_mode = EMM_HDMI2_NORMAL;
	mode = (uint8_t)drv_obj->dsc_mode;

	switch (colorspace) {
	case ACS__RGB:
	case ACS__YCBCR444:
		mode |= EMM_YC444_RGB;
		break;
	case ACS__YCBCR422:
		mode |= EMM_YC422;
		break;
	case ACS__YCBCR420:
		mode |= EMM_YC420;
		break;
	default:
		break;
	}
	msleep(100);
	sii_log_debug("sending EMM\n");
	return sii_mod_tx_hdmi_request_emm(drv_obj->tx_hdmi_obj, mode);
#else
	drv_obj->dsc_mode = EMM_HDMI2_NORMAL;

	drv_obj->avi_out_data.b[1] &= 0xCF;
	drv_obj->avi_out_data.is_if_on = true;

	drv_obj->avi_out_data.b[3] = 0;
	drv_obj->avi_out_data.b[3] = calculate_generic_checksum(
		drv_obj->avi_out_data.b, 0, SII_INFOFRAME_MAX_LEN);

	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__IF_DATA,
		&drv_obj->avi_out_data);

	sii_log_debug("sending EMM data in AVI: %d\n",
		(drv_obj->avi_out_data.b[1] & 0x30) >> 4);
	return sii_mod_tx_hdmi_request_emm(drv_obj->tx_hdmi_obj, drv_obj->dsc_mode);
#endif
}

#ifdef CSC_AS_PER_INPUT_STANDARD
static void s_upsampling_to_444(struct driver_obj *drv_obj)
{
	enum sii_drv_clr_spc clr_spc =
		SII_DRV_CLRSPC__YC444_709;
	enum sii_drv_conv_std conv_std =
		drv_obj->avi_data.payload.conv_std;
	if ((conv_std == SII_DRV_CONV_STD__BT_2020_NON_CONSTANT_LUMINOUS)
		|| (conv_std == SII_DRV_CONV_STD__BT_2020_CONSTANT_LUMINOUS))
		clr_spc = SII_DRV_CLRSPC__YC444_2020;
	sii_drv_output_color_space_set(drv_obj,	clr_spc);
}
#endif

static int handle_enhanced_mode(struct driver_obj *drv_obj)
{
	int status = 0;
	uint8_t vic = 0;
	uint8_t hdmi_vic;
	bool enable_dsc = false;
	uint8_t compression = 0;
	enum avi_color_space colorspace =
			drv_obj->avi_data.payload.pb1.colorSpace;

	drv_obj->emm_response_received = false;
	drv_obj->emm_response = 0;
	vic = drv_obj->avi_data.payload.pb4.vic;
	hdmi_vic = drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;

	if (drv_obj->output_tmds_clk <= 340000000) {
		bool enable = true;
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
			&enable);

		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
			&enable);
		status = switch_to_non_dsc_mode(drv_obj);

	} else if (((vic == 97) || (vic == 107))
		&& (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K60)) {
		/*Dont change the sequence*/
		if (colorspace == ACS__YCBCR420) {
			if (drv_obj->upsampled_to_422)
				colorspace = ACS__YCBCR422;
		}
		if (colorspace == ACS__YCBCR422) {
			sii_log_debug("upsampling to YC444\n");
#ifdef CSC_AS_PER_INPUT_STANDARD
			s_upsampling_to_444(drv_obj);
#else
			sii_drv_output_color_space_set(drv_obj,
				SII_DRV_CLRSPC__YC444_709);
#endif
			msleep(100);
		}
		compression = 2;
		enable_dsc = true;
	} else if (((vic == 96) || (vic == 106))
		&& (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K50)) {
		/*Dont change the sequence*/
		if (colorspace == ACS__YCBCR420) {
			if (drv_obj->upsampled_to_422)
				colorspace = ACS__YCBCR422;
		}
		if (colorspace == ACS__YCBCR422) {
			sii_log_debug("upsampling to YC444\n");
#ifdef CSC_AS_PER_INPUT_STANDARD
			s_upsampling_to_444(drv_obj);
#else
			sii_drv_output_color_space_set(drv_obj,
				SII_DRV_CLRSPC__YC444_709);
#endif
			msleep(100);
		}
		compression = 2;
		enable_dsc = true;
	} else if (((vic == 95) || (hdmi_vic == 1))
		&& (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K30)) {
			/*Probably this one is 10bit*/
			compression = 2;
			enable_dsc = true;
	} else if (((vic == 94) || (hdmi_vic == 2))
		&& (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K25)) {
			/*Probably this one is 10bit*/
			compression = 2;
			enable_dsc = true;
	} else if (((vic == 93) || (hdmi_vic == 3))
		&& (drv_obj->ds_em_caps & EMC_HDMI2_DSC_4K24)) {
			/*Probably this one is 10bit*/
			compression = 2;
			enable_dsc = true;
	} else {
		sii_log_err("pxl clk too high\n");
		enable_dsc = false;
	}

	if (enable_dsc) {
		status = dsc_configure(drv_obj, compression);
		if (status)
			sii_log_debug("Failed to configure DSC(OK)\n");
		else {
			bool enable = true;
			status = sii_drv_tx_dsc_enable(drv_obj, true);
			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
				&enable);

			if (status) {
				sii_log_err("failed to enable dsc\n");
			} else {
				msleep(300);
				switch_to_dsc_mode(drv_obj,
					drv_obj->dsc_compression);
			}
		}
	}
	return status;
}

static int handle_emc_query(struct driver_obj *drv_obj, bool enable)
{
	uint8_t vic = drv_obj->avi_data.payload.pb4.vic;
	uint8_t hdmi_vic = drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;
	enum avi_color_space colorspace =
			drv_obj->avi_data.payload.pb1.colorSpace;
	uint8_t compression = 0;
	bool enable_dsc = false;
	int status = 0;

	if (drv_obj->output_tmds_clk <= 340000000) {
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
			&enable);

		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
			&enable);

		msleep(100);
		sii_mod_tx_hdmi_request_em_caps(drv_obj->tx_hdmi_obj);

	} else if ((vic == 96) || (vic == 97) || (vic == 106) || (vic == 107)) {
		/*Dont change the sequence*/
		if (colorspace == ACS__YCBCR420) {
			if (drv_obj->upsampled_to_422)
				colorspace = ACS__YCBCR422;
		}
		if (colorspace == ACS__YCBCR422) {
			sii_log_debug("upsampling to YC444\n");
#ifdef CSC_AS_PER_INPUT_STANDARD
			s_upsampling_to_444(drv_obj);
#else
			sii_drv_output_color_space_set(drv_obj,
				SII_DRV_CLRSPC__YC444_709);
#endif
			msleep(100);
		}
		compression = 2;
		enable_dsc = true;

	} else if ((vic == 93) || (vic == 94) || (vic == 95)
		|| (hdmi_vic == 1) || (hdmi_vic == 2) || (hdmi_vic == 3)) {
		/*Probably this one is 10bit*/
		compression = 2;
		enable_dsc = true;
	} else {
		sii_log_err("pxl clk too high\n");
		enable_dsc = false;
	}

	if (enable_dsc) {
		status = dsc_configure(drv_obj, compression);
		if (status)
			sii_log_err("dsc configure failed\n");
		else {
			status = sii_drv_tx_dsc_enable(drv_obj, true);
			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
				&enable);
			if (status) {
				sii_log_err("dsc enable failed\n");
			} else if (!drv_obj->is_emc_queried) {
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
				/* Original delay is 80ms */
				msleep(80);
#endif
				sii_mod_tx_hdmi_request_em_caps(
						drv_obj->tx_hdmi_obj);
			} else {
				msleep(80);
				switch_to_dsc_mode(drv_obj,
					drv_obj->dsc_compression);
			}
		}
	}
	return status;
}
#endif

static void s_tx_tmds_enable(struct driver_obj *drv_obj, bool enable)
{
	/*just in case if hdcp is enabled, disable it*/
	if (drv_obj->tx_hdcp_status)
		s_tx_hdcp_enable(drv_obj, false);

#ifdef CONFIG_ENHANCED_MODE_PROT
	if (enable) {
		uint8_t l_u8vic = drv_obj->avi_data.payload.pb4.vic;
		uint8_t hdmi_vic =
			drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;

		sii_log_debug("CEA_VIC = %d	HDMI_VIC = %d\n",
			l_u8vic, hdmi_vic);
		sii_log_debug("Output TMDS CLK = %lu\n",
			drv_obj->output_tmds_clk);

		/*if ((vic == 0) && (hdmi_vic == 0)
			&& !(drv_obj->is_tpg_enabled)) {

			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
				&enable);
			sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
				&enable);
			return;
		}*/

		if (!drv_obj->is_emc_queried) {
			drv_obj->ds_em_caps = 0;
			if (is_ds_device_spiderman2(drv_obj)
                && (drv_obj->output_tmds_clk > 340000000 )
                ) {
				handle_emc_query(drv_obj, enable);
			} else {
				drv_obj->is_emc_queried = true;
				drv_obj->ds_em_caps = 0;

				sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
					SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
					&enable);

				sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				  SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
				  &enable);
				sii_log_debug("Allow HDCP to start\n");
                s_tx_hdcp_enable(drv_obj, true);

			}
		} else if (drv_obj->is_emc_queried) {
			if (is_ds_device_spiderman2(drv_obj)) {
				handle_enhanced_mode(drv_obj);
			} else {
				sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				  SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
				  &enable);

				sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				  SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
				  &enable);
			}
		}
        UNUSED(hdmi_vic);
        UNUSED(l_u8vic);
	} else {
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
			&enable);

		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
			&enable);
	}
#else
	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__TMDS_ON_OFF,
		&enable);

	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
		&enable);
#endif
}

void reset_60mhz_osc(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG4,
		BIT_MSK__SW_RST_REG4__REG_OSC_RST);
	sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG4,
		BIT_MSK__SW_RST_REG4__REG_OSC_RST);
}

static void s_tx_tmds_enable_handler(void *p_obj)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (1) {
		if (drv_obj->b_tx_is_hdcp_req) {
			s_tx_hdcp_enable(drv_obj, true);
			drv_obj->b_tx_is_hdcp_req = false;
		} else if (drv_obj->tx_hdcp_status) {
			sii_timer_stop(&drv_obj->timer_tx_tmds_enable_poll);
		} else {
			if (drv_obj->b_tx_pll_lock_status)
				sii_timer_start(
					&drv_obj->timer_tx_tmds_enable_poll, 100);
		}
		//up(&drv_obj->drv_context->isr_lock);
	}
}

static void s_tx_pll_lock_monitor(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	bool tmds_p_stable;

	s_tx_pll_lock_poll(drv_obj);
	if (drv_obj->b_tx_pll_lock_status) {
		sii_log_info("Tx PLL Locked\n");
		/* No need to disable tx zone
		as tx is in Auto zone */
		/*s_tx_zone_enable(drv_obj, false);*/
		tmds_p_stable = (bool)(sii_platform_rd_reg8(pstDev, REG_ADDR__SYS_STAT)
			& BIT_MSK__SYS_STAT__P_STABLE);

		if (tmds_p_stable ) {
			sii_log_debug("pStable is present\n");
			if (drv_obj->b_tx_tmds_on_req)
			{
				sii_log_debug("Enabling TMDS\n");
				s_tx_tmds_enable(drv_obj, true);
				drv_obj->b_tx_tmds_on_req = false;
			}

		} else {
			sii_log_debug("pStable is Not-Present\n");
			/* PLL is locked but pstable is not present
			   So, Reset Pclk or disable tx zone*/
			s_tx_zone_enable(drv_obj, false);
			msleep(100);
			s_tx_zone_enable(drv_obj, true);
		}
		if (tmds_p_stable || drv_obj->is_tpg_enabled) {
				sii_timer_start(&drv_obj->timer_tx_tmds_enable_poll, 100);
		}
	} else {
		sii_log_err("TX PLL lock lost\n");
		sii_timer_stop(&drv_obj->timer_tx_tmds_enable_poll);
	}
}

static void s_hdcp_auth_deglitch(void *p_obj)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (1) {
		if ((drv_obj->tx_hdcp_status ==
				SII_DRV_HDCP_STATUS__SUCCESS_1X) ||
			(drv_obj->tx_hdcp_status ==
				SII_DRV_HDCP_STATUS__SUCCESS_22))
			s_tx_hdcp_enable(drv_obj, true);
		//up(&drv_obj->drv_context->isr_lock);
	}
}

static void s_temp_tdms_handler(void *p_obj)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (1) {
		s_rx_tmds_mode_change_handler(drv_obj);

		//up(&drv_obj->drv_context->isr_lock);
	}
}

/*disable tx tmds and hdcp on scdt/ckdt low*/
/*set the output color based on color convertion*/
/*and enable tx tmds and hdcp on scdt/ckdt high*/
static void s_rx_tmds_mode_change_handler(struct driver_obj *drv_obj)
{
	sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__TMDS_MODE, &drv_obj->rx_tmds_mode);

	if (drv_obj->rx_tmds_mode > SII_TMDS_MODE__NONE) {
		sii_mod_vid_path_set(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS_UPDATE,
			NULL);
		drv_obj->b_rx_tmds_enabled = true;
		update_tx_tmds_clock(drv_obj, 0);
	} else {
		drv_obj->b_rx_tmds_enabled = false;
		drv_obj->b_rx_hdcp_enabled = drv_obj->b_rx_tmds_enabled;
		memset(&drv_obj->acr_if.b, 0, SII_INFOFRAME_MAX_LEN);
		memset(&drv_obj->acr_if_data, 0,
			sizeof(struct acr_info_frame));
		drv_obj->input_unstable_counter = 0;
		s_update_drv_state(drv_obj);
	}
	s_update_drv_state(drv_obj);
}

static void rx_pll_lock_intr_handler(struct driver_obj *drv_obj)
{
	s_rx_pll_lock_monitor(drv_obj);
}

static void tx_pll_lock_intr_handler(struct driver_obj *drv_obj)
{
	s_tx_pll_lock_monitor(drv_obj);
}
/*
 * Interrupt handler for CP 8630 interrupts.
 *
 * @irq:	The number of the asserted IRQ line that caused
 *		this handler to be called.
 * @data:	Data pointer passed when the interrupt was enabled,
 *		which in this case is a pointer to an
 *		cp8630_driver_context structure.
 *
 * Always returns IRQ_HANDLED.
 */
static bool cp8630_gen_interrupt_handler(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev;// = drv_obj->pstDevice;

	uint8_t grp_intr_stat = 0, l1_intr_stat_0 = 0, l1_intr_stat_1 = 0;
	uint8_t intr0_stat = 0, l1_intr = 0;
	uint8_t intr_stat3;

	uint8_t bit_msk_rx_int13_stat0 = BIT_MSK__RX_INTR13__REG_INTR13_STAT0;
	uint8_t bit_msk_l1_stat_B0 = BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B0;
	uint8_t bit_msk_l1_stat_B4 = BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B4;
#ifdef CONFIG_SII9630_HDMI_TX
	uint8_t bit_msk_l1_stat_B6 = BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B6;
#endif
	uint8_t bit_msk_l1_stat_B7 = BIT_MSK__L1_INTR_STAT_0__L1_INTR_STAT_B7;
#if (CONFIG_SOURCE_ONLY == 0)
    uint8_t ipc_queue = 1;
#endif
#ifdef CONFIG_POWER_DOWN_EN
    uint8_t hdcp_2x_power_status;
#endif

	if (NULL == drv_obj)
		return FALSE;

	if (false == drv_obj->init_done)
		return FALSE;

	pstDev = drv_obj->pstDevice;

	if (1) {
#if (CONFIG_SOURCE_ONLY == 0)
        // Insert Spiderman A handler here
        //
        //
        //sii_log_print("%s:%d start\n", __func__, __LINE__);
        while(ipc_queue != 0){
            uint8_t sa_reg2;
            sii_ipc_check_notify(ex_sp_pdata->rx_inst, &sa_reg2);
            if( sa_reg2 & SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK ){
                sii_log_print("%s:%d start\n", __func__, __LINE__);
                sii_ipc_host_handler(ex_sp_pdata->rx_inst);
            }
            ipc_queue = sii_handle(ex_sp_pdata->rx_inst, ex_sp_pdata);
#endif
    		intr0_stat = sii_platform_rd_reg8(pstDev, REG_ADDR__INTR_STAT0);
    		intr_stat3 = sii_platform_rd_reg8(pstDev,
    				REG_ADDR__INTR_STAT_MASK3);
#ifdef CONFIG_POWER_DOWN_EN
    		hdcp_2x_power_status =
    			sii_platform_rd_reg8(pstDev, REG_ADDR__INTR_STAT2);

    		if (hdcp_2x_power_status &
    			BIT_MSK__INTR_STAT2__REG_INTR2_STAT2) {
    			/*uint8_t pwr_ctrl_status = sii_platform_rd_reg8(pstDev,
    						REG_ADDR__PWR_CTRL_STATUS2);

    			if ((pwr_ctrl_status & BIT6) == 0x0)
    				sii_log_debug("HDCP 2X TX:: Power down==>\n");*/

    			sii_platform_wr_reg8(pstDev, REG_ADDR__INTR_STAT2,
    				BIT_MSK__INTR_STAT2__REG_INTR2_STAT2);
    			sii_log_debug("HDCP 2X TX:: Power down==>\n");
    		}
    		if (hdcp_2x_power_status &
    			BIT_MSK__INTR_STAT2__REG_INTR2_STAT6) {
    			/*uint8_t pwr_ctrl_status = sii_platform_rd_reg8(pstDev,
    						REG_ADDR__PWR_CTRL_STATUS2);*/
    			sii_platform_wr_reg8(pstDev, REG_ADDR__INTR_STAT2,
    				BIT_MSK__INTR_STAT2__REG_INTR2_STAT6);
    			/*if ((pwr_ctrl_status & BIT6) == BIT6)*/ {
    				bool enable = true;
    				sii_log_debug("HDCP 2X TX:: Power up==>\n");
    				sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
    					SII_MOD_HDCP_OPCODE__POWER_UP_STATUS,
    					&enable);
    			}
    		}
#endif
    		if (intr0_stat) {
    			sii_platform_wr_reg8(pstDev, REG_ADDR__INTR_STAT0,
    				((BIT_MSK__INTR_STAT0__REG_INTR0_STAT0 |
    				BIT_MSK__INTR_STAT0__REG_INTR0_STAT1) &
    				intr0_stat));

    			if (intr0_stat &
    				BIT_MSK__INTR_STAT0__REG_INTR0_STAT5) {
    				/* Rx PLL Lock Intr */
    				rx_pll_lock_intr_handler(drv_obj);
    				sii_platform_wr_reg8(pstDev,
    					REG_ADDR__INTR_STAT0,
    					BIT_MSK__INTR_STAT0__REG_INTR0_STAT5);
    			}

    			/* interrupts for RX*/
    			if (intr0_stat & BIT_MSK__INTR_STAT0__REG_INTR0_STAT0) {

    				sii_mod_rx_hdmi_interrupt_handler(
    					drv_obj->rx_hdmi_obj);

    				grp_intr_stat = sii_platform_rd_reg8(pstDev,
    					REG_ADDR__RX_INTR13);
    				if (grp_intr_stat & bit_msk_rx_int13_stat0) {
    					sii_mod_vid_path_intr_handler(
    						drv_obj->vidpath_obj,
    						drv_obj->b_rx_pll_lock_status);
    				}
    			}
    			/*grp_intr_stat = sii_platform_rd_reg8(pstDev,
    			 * REG_ADDR__RX_INTR7_AON);*/

    			/*interrupts for TX*/
    			if (intr0_stat & BIT_MSK__INTR_STAT0__REG_INTR0_STAT1) {

    				l1_intr = sii_platform_rd_reg8(pstDev,
    					REG_ADDR__INTR1);
    				if (l1_intr & BIT_MSK__INTR1__REG_INTR1_STAT7) {
    					sii_platform_wr_reg8(pstDev,
    					    REG_ADDR__INTR1,
    					    BIT_MSK__INTR1__REG_INTR1_STAT7);
    					s_tx_counter_handler(drv_obj);
    				}

    				/* read all group interrupts for TX here. */
    				l1_intr_stat_0 = sii_platform_rd_reg8(pstDev,
    					REG_ADDR__L1_INTR_STAT_0);
    				l1_intr_stat_1 = sii_platform_rd_reg8(pstDev,
    					REG_ADDR__L1_INTR_STAT_1);

    				/* check for INTR1 interrupts*/
    				if ((l1_intr_stat_0 & bit_msk_l1_stat_B0)
    				|| (l1_intr_stat_0 & bit_msk_l1_stat_B6)) {
    					sii_mod_tx_hdmi_interrupt_handler(
    						drv_obj->tx_hdmi_obj);
    				}

    				/* check for hdcp(TPI IP)
    				 * group interrupts*/
    				if (l1_intr_stat_0 &
    					(bit_msk_l1_stat_B4 |
    					bit_msk_l1_stat_B7)) {
    					sii_mod_tx_hdcp_interrupt_handler(
    						drv_obj->hdcp_obj);
    				}
    			}

    			if (intr0_stat &
    				BIT_MSK__INTR_STAT0__REG_INTR0_STAT3) {
    				/* COC Interrupt Status */
    				sii_platform_wr_reg8(pstDev,
    					REG_ADDR__INTR_STAT0,
    					BIT_MSK__INTR_STAT0__REG_INTR0_STAT3);
    			}

    			if (intr0_stat &
    				BIT_MSK__INTR_STAT0__REG_INTR0_STAT7) {
    				/* Tx PLL Lock Intr */
    				sii_platform_wr_reg8(pstDev,
    					REG_ADDR__INTR_STAT0,
    					BIT_MSK__INTR_STAT0__REG_INTR0_STAT7);
    				tx_pll_lock_intr_handler(drv_obj);
    			}
    		}
#if (CONFIG_SOURCE_ONLY == 0)
        }
#endif
//irq_done:
        //sii_log_print("%s:%d done\n", __func__, __LINE__);
	}

    UNUSED(intr_stat3);
    UNUSED(l1_intr_stat_1);
	return TRUE;
}

static void s_rx_pll_lock_poll(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	uint8_t rx_zone_status = sii_platform_rd_reg8(pstDev,
		REG_ADDR__RX_ZONE_STATUS_0);
	uint8_t rx_zone_done_intr_status = sii_platform_rd_reg8(pstDev,
		REG_ADDR__RX_ZONE_INTR);

	if ((rx_zone_status & BIT_MSK__RX_ZONE_STATUS_0__RO_PLL_LK)
			|| (rx_zone_done_intr_status &
				BIT_MSK__RX_ZONE_INTR__REG_ZONE_DONE)) {
		drv_obj->b_rx_pll_lock_status = true;
		/* if interupt status triggerd clear it */
		if (rx_zone_done_intr_status &
			BIT_MSK__RX_ZONE_INTR__REG_ZONE_DONE) {
			sii_platform_wr_reg8(pstDev,
			REG_ADDR__RX_ZONE_INTR,
			BIT_MSK__RX_ZONE_INTR__REG_ZONE_DONE);
		}
	} else {
		drv_obj->b_rx_pll_lock_status = false;
	}

}
static void s_tx_pll_lock_poll(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	uint8_t tx_zone_status = sii_platform_rd_reg8(pstDev,
		REG_ADDR__TX_ZONE_STATUS_0);

	if (tx_zone_status & BIT_MSK__TX_ZONE_STATUS_0__RO_PLL_LK) {
		drv_obj->b_tx_pll_lock_status = true;
		;
	} else {
		drv_obj->b_tx_pll_lock_status = false;
	}
}

static void s_rx_zone_enable(struct driver_obj *drv_obj, bool enable)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	if (enable) {
		sii_log_debug("rx_zone_enabled\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_LKDT_CTRL_0, 0xCC);
	} else {
		sii_log_debug("rx_zone_disabled\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_LKDT_CTRL_0, 0x00);
	}
}

static void s_rx_manual_zone_enable(struct driver_obj *drv_obj, bool enable)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	if (enable) {
		/* configure the analog PHY for 600_mhz */
		/*sii_log_debug("Enable Rx Manual zone for 600Mhz\n");*/
		sii_log_debug("Enable Rx Auto zone for 600Mhz\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_PLL_MODE0,
			BIT_MSK__RX_PLL_MODE0__RI_HDMI_MHLN_SEL_OW_EN);
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_SZONE_OW, 0x00);
		/*sii_platform_wr_reg8(pstDev, REG_ADDR__RX_SZONE_OW,
			BIT_MSK__RX_SZONE_OW__RI_ZONE_OW_EN);*/
		/*sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_RSV1, 0x01);*/
		/*0x4097 is now RX_ZONE_CTRL_2*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_CTRL_2, 0x01);
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_CTRL1, 0x57);
	} else {
		/* configure the analog PHY for 340_mhz */
		sii_log_debug("Enable Rx Auto zone for 340Mhz\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_PLL_MODE0, 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_SZONE_OW, 0x00);
		/*sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_RSV1, 0x00);*/
		/*0x4097 is now RX_ZONE_CTRL_2*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_CTRL_2, 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__RX_CTRL1, 0x0F);
	}
}

static void s_tx_zone_enable(struct driver_obj *drv_obj, bool enable)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	if (enable) {
		/*tx pclk reset*/
		sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG4,
			BIT_MSK__SW_RST_REG4__REG_TX_PCLK_RST);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG4,
			BIT_MSK__SW_RST_REG4__REG_TX_PCLK_RST);
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_LKDT_CTRL_0, 0xCC);
		/*Apply TxZone reset*/
		sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG5,
			BIT_MSK__SW_RST_REG5__REG_TXZONE_SW_RST);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG5,
			BIT_MSK__SW_RST_REG5__REG_TXZONE_SW_RST);
		sii_log_debug("Enable Tx Auto zone\n");
	} else {
		sii_log_debug("disable Tx zone\n");
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_LKDT_CTRL_0, 0x00);
	}
}

static void s_configure_analog_phy(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;

	/*sii_platform_wr_reg8(pstDev, REG_ADDR__OSC_CALIB_CTRL, 0x00);*/

	sii_platform_wr_reg8(pstDev, REG_ADDR__PLL_VCOCAL, 0x1C);
	/* RX Ch0,1,2,clk enable; [6:4] for CLK???*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TMDSRX_PD_SYS_CTRL, 0x7F);
	/*Enable PLL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_CTRL1, 0x0F);

	/* Term select*/
	/*sii_platform_wr_reg8(pstDev, ANAPHY_RXDPLL, 0x07, 0x44);*/
	/* Normal clock in MHL. Not required in HDMI?*/
	/*write ("AnalogPhy", 0x20, 0x00);           */
	/* TX Term select for HDMI 1.4 300 ohms*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__MHL_DP_CTL2, 0x0F);
	/*Enable HDMI clk 1x=1x pxl*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__MHL_PLL_CTL0, 0x04);
	/*Enable HDMI TX Output config*/
	/*sii_platform_wr_reg8(pstDev, ANAPHY_RXDPLL, 0x21, 0xB5);           */

	/*Enable HDMI TX Output co*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__ZCTRL_1, 0x28);
	/*COC overwrite CTRL0*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__COC_OVERWRITE_CTRL0, 0x60);
	/*COC overwrite CTRL1*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__COC_OVERWRITE_CTRL1, 0x4A);
	/*COC overwrite CTRL2*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__COC_OVERWRITE_CTRL2, 0x00);
	/*COC overwrite CTRL3*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__COC_OVERWRITE_CTRL3, 0x00);
	/*TX Zone CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TXZONE_CTL_1, 0x82);
	/*TX Zone CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TXZONE_CTL_2, 0x20);
	/*TX Zone CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TXZONE_CTL_3, 0xA0);
	/*TX Zone CTRL*/
	/*sii_platform_wr_reg8(pstDev, ANAPHY_RXDPLL, 0x62, 0xA0);           */
	/*Pre DIV CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TMDSRX_PRE_DIV_CTL, 0x00);
	/*Zone CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_CTRL3, 0x02);
	/*Zone CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_CTRL2, 0x00);
	/*Zone CTRL*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_ZONE_CTRL1, 0x18);

	sii_platform_wr_reg8(pstDev, REG_ADDR__MHL_MISC_CTL1, 0x4D);
	sii_platform_wr_reg8(pstDev, REG_ADDR__MHL_MISC_CTL2, 0x03);

	sii_platform_wr_reg8(pstDev, REG_ADDR__CLKDET_CTL_OVR, 0x68);
	sii_platform_set_bit8(pstDev, REG_ADDR__SWAY_FPLL_CTRL_0,
		BIT_MSK__SWAY_FPLL_CTRL_0__REG_SWAY_FPLL_PDB);

	sii_platform_wr_reg8(pstDev, REG_ADDR__FUNC_SEL,
		BIT_MSK__FUNC_SEL__REG_TX_HDMI_EN);
}

/*updates upstream about downstream status*/
/*controls upstream events depending upon downstream events*/
void s_mod_handle_tx_hdcp_events(struct driver_obj *drv_obj,
	      uint32_t event_flags)
{
	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_RCVID_CHANGED) {
		uint8_t p_buff[RCVRID_LENGTH];

		sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__RCVID_GET,
			(struct sii_drv_hdcp2x_rpt_rcv_id_list *)&p_buff);
		sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__HDCP_RCVID_SET,
			(uint8_t *)&p_buff);
		sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__RPT_RCVID_GET,
			(struct sii_drv_hdcp2x_rpt_rcv_id_list *)&drv_obj->hdcp2x_rpt_ksv);
		sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__HDCP_RPT_RCVID_SET,
			(struct sii_drv_hdcp2x_rpt_rcv_id_list *)&drv_obj->hdcp2x_rpt_ksv);
	}

	if (event_flags & SII_DRV_HDCP_EVENT__HDCP_RESET_FLAGS) {
		bool ake_event_set = false;

		sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__HDCP_RESET_FLAGS,
			(uint8_t *)&ake_event_set);
	}
}

static int stop_all_timers(struct driver_obj *drv_obj)
{
	int status = 0;

#ifdef CONFIG_HANDLE_BOARD_RESET
	if (drv_obj->chip_reset_poll) {
		sii_timer_stop(&drv_obj->chip_reset_poll);
	}
#endif

	if (drv_obj->timer_tx_tmds_enable_poll) {
		sii_timer_stop(&drv_obj->timer_tx_tmds_enable_poll);
	}
	if (drv_obj->inst_auth_deglitch) {
		sii_timer_stop(
			&drv_obj->inst_auth_deglitch);
	}
	if (drv_obj->inst_temp_tmds_handler) {
		sii_timer_stop(
			&drv_obj->inst_temp_tmds_handler);
	}
	if (drv_obj->dsc_obj)
		sii_mod_dsc_stop_timers(drv_obj->dsc_obj);
	if (drv_obj->vidpath_obj)
		sii_mod_vid_path_stop_timers(drv_obj->vidpath_obj);
	if (drv_obj->hdcp_obj)
		sii_mod_tx_hdcp_stop_timers(drv_obj->hdcp_obj);
	if (drv_obj->tx_hdmi_obj)
		sii_mod_tx_hdmi_stop_timers(drv_obj->tx_hdmi_obj);
	if (drv_obj->rx_hdmi_obj)
		sii_mod_rx_hdmi_stop_timers(drv_obj->rx_hdmi_obj);

	return status;
}

static int dmode_chip_shutdown(void *p_obj)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	int status = 0;

	sii_log_info("SiI%X Driver Shutdown.\n", drv_obj->chip_id);
	status = stop_all_timers(drv_obj);
	/*set_pin(DEV_FW_WAKE_INDEX, 0);*/

	return status;
}

static void reset_states(struct driver_obj *drv_obj)
{
	drv_obj->drv_istate = SII_DRV_TX_OFF__RX_OFF;
	drv_obj->b_rx_hdmi_connected = false;
	drv_obj->rx_hdcp_status = SII_DRV_HDCP_STATUS__OFF;
	drv_obj->tx_rsen = false;
	drv_obj->b_tx_connected = false;
	drv_obj->tx_tmds_enabled = false;
	drv_obj->tx_pstable = 0;
	drv_obj->b_rx_hdcp_enabled = false;
	drv_obj->b_rx_tmds_enabled = false;
	drv_obj->b_is_vixs_format_handled = false;
	drv_obj->b_rx_pll_lock_status = false;
	drv_obj->b_tx_pll_lock_status = false;
	drv_obj->b_tx_is_hdcp_req = false;
	drv_obj->b_tx_tmds_on_req = false;
	drv_obj->is_tpg_enabled = false;
	drv_obj->bist_owner = SII_DRV_BIST_OWNER__NONE;
	/*s_set_drv_i_state_change_handler_handlers(drv_obj);*/
#if (CONFIG_SOURCE_ONLY == 1)
		drv_obj->hdcp_arbitration_mask = HDCP_ENABLE__MASK;
		drv_obj->tx_hdcp_auth_type = SII_DRV_HDCP_LINK_VER__AUTO;
		drv_obj->tx_tmds_mode_user = SII_TMDS_MODE__AUTO;
#else
		drv_obj->hdcp_arbitration_mask = 0;
		drv_obj->tx_hdcp_auth_type = SII_DRV_HDCP_LINK_VER__NONE;
#ifdef CONFIG_TX_TMDS_MODE_CTRL
		drv_obj->tx_tmds_mode_user = SII_TMDS_MODE__NONE;
#else
		drv_obj->tx_tmds_mode_user = SII_TMDS_MODE__AUTO;
#endif // CONFIG_TX_TMDS_MODE_CTRL
#endif // (CONFIG_SOURCE_ONLY == 1)
	drv_obj->out_clr_spc = SII_DRV_CLRSPC__PASSTHRU;
	drv_obj->sway_owner = SWAY_INUSE_FOR__NONE;
	drv_obj->tx_hdcp_status = SII_DRV_HDCP_STATUS__OFF;
	drv_obj->dsc_support = false;
	drv_obj->is_dsc_reqd = false;
	drv_obj->dsc_compression = SII_DRV_DSC_COMPRESSION_RATIO__NONE;
	drv_obj->init_done = false;
	drv_obj->input_unstable_counter = 0;
#ifdef CONFIG_ENHANCED_MODE_PROT
	drv_obj->is_emc_queried = false;
	drv_obj->ds_em_caps = 0;
	drv_obj->emm_response_received = false;
	drv_obj->emm_response = 0;
	drv_obj->upsampled_to_444 = false;
	drv_obj->upsampled_to_422 = false;
#endif
	memset(&drv_obj->avi_out_data, 0, sizeof(struct sii_info_frame));
}

static int dmode_chip_initialize(struct driver_obj *drv_obj)
{
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	uint8_t chip_id[2];
	int status = 0;

	reset_states(drv_obj);
	/*set_pin(DEV_FW_WAKE_INDEX, 1);*/

	/* Apply HW Reset */
	sii_platform_hardware_reset(pstDev);

	/* Apply soft_reset */
	status = sii_platform_wr_reg8(pstDev, REG_ADDR__SW_RST_REG0, 0x01);
	if (status < 0)
		return status;
	sii_platform_wr_reg8(pstDev, REG_ADDR__SW_RST_REG0, 0x00);

	/*Read Device ID*/
	status = sii_platform_block_read8(pstDev, REG_ADDR__BANNER_CHIP_LID,
			chip_id, 2);
	drv_obj->chip_id = ((chip_id[1] << 8) & 0xFF00) | (chip_id[0] & 0xFF);
	sii_log_debug("CHIP ID: 0x%02x\n", drv_obj->chip_id);

	if (0x00 == drv_obj->chip_id) {
		sii_log_debug("Invalid Chip ID\n");
		/*return 1;*/
	}

#ifdef CONFIG_INT_OPENDRAIN

    // set INT pin as open drain
    sii_platform_wr_reg8(pstDev, REG_ADDR__INTR_CTRL,
        BIT_MSK__INTR_CTRL__REG_INTR_OPENDRAIN | BIT_MSK__INTR_CTRL__REG_INTR_POLARITY);
    sii_platform_wr_reg8(pstDev, REG_ADDR__GP_CTRL0_REG, 0x80 ); // this is necessary!

#endif

	//if (drv_obj->chip_id == 0x8632)
	sii_platform_wr_reg8(pstDev, REG_ADDR__OTP_MUTE_CTRL, 0xF0);

#ifdef CONFIG_POWER_DOWN_EN
	/*powerdown is enabled.*/
	/* gate off p_clk, mhl1/2/3 */
	sii_platform_wr_reg8(pstDev, REG_ADDR__PD_REG, 0xB8);
#else
	/*powerdown is enabled.*/
	/* gate off p_clk, mhl1/2/3 */
	/* not required power down mode for hdcp 2.2 and mhl*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__PD_REG, 0x38);
#endif

	/* prog DDC delay counter*/
	/*sii_platform_wr_reg8(pstDev, REG_ADDR__TPI_HW_OPT2, 0x40);*/

	/* select tx as HDMI*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__FUNC_SEL, 0x01);
	sii_platform_wr_reg8(pstDev, REG_ADDR__BANNER_CONFIG0, 0x06);

	/* Uses 24MHz crystal for vidpath registers access */
	sii_platform_set_bit8(pstDev, REG_ADDR__BANNER_CONFIG5,
		BIT_MSK__BANNER_CONFIG5__REG_USE_XTAL_FOR_VIDREG_EN);
	/* Banner supports only deepcolor bypass
	for DSC 420 10 cases clear this */
	sii_platform_set_bit8(pstDev, REG_ADDR__BANNER_CONFIG5,
		BIT_MSK__BANNER_CONFIG5__REG_DC_BYPASS);

	/*select channel 2 */
	sii_platform_wr_reg8(pstDev, REG_ADDR__HDMICTL4, 0x03);

	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL0, 0x1B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL1, 0x3B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL2, 0x5B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL3, 0x7B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL4, 0x9B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL5, 0xBB);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL6, 0xDB);
	sii_platform_wr_reg8(pstDev, REG_ADDR__PEQ_VAL7, 0xFB);

	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_IPS_VAL0, 0xBB);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_IPS_VAL1, 0x9B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_IPS_VAL2, 0x7B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_IPS_VAL3, 0x5B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_IPS_VAL4, 0x3B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_IPS_VAL5, 0x1B);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_CTRL0, 0x03);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DPLL_CFG1, 0x22);
	sii_platform_wr_reg8(pstDev, REG_ADDR__BV_VAL, 0x0D);

	sii_platform_wr_reg8(pstDev, REG_ADDR__TMDSRX_TERMCTRL, 0x44);

	/* power down swaypll */
	sii_platform_clr_bit8(pstDev, REG_ADDR__TMDS_CTRL2,
		BIT_MSK__TMDS_CTRL2__REG_PWRON_HSIC);

	/* Adjust the threshold of oclk5m counter */
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_OCLK_CNT_THR, 0x10);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_OCLK_CNT_THR, 0x10);

	/* drop NULL packets */
	sii_platform_set_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
		BIT_MSK__PKT_FILTER_1__REG_DROP_NULL_PKT);

	s_configure_analog_phy(drv_obj);

	if (drv_obj->rx_hdmi_obj)
		status = init_hdmi_rx_regs(drv_obj->rx_hdmi_obj);
	if (drv_obj->tx_hdmi_obj)
		status = init_hdmi_tx_regs(drv_obj->tx_hdmi_obj);
	if (drv_obj->hdcp_obj)
		status = init_hdcp_tx_regs(drv_obj->hdcp_obj);
	if (drv_obj->vidpath_obj) {
		/* work around for videopath register access */
		sii_platform_wr_reg8(pstDev, REG_ADDR__PI_TO_LIMIT_L, 0x0F);
		sii_platform_wr_reg8(pstDev, REG_ADDR__PI_TO_LIMIT_H, 0x00);
		status = init_videopath_regs(drv_obj->vidpath_obj);
	}

	/* enable interrupt masks */
	sii_platform_set_bit8(pstDev, REG_ADDR__INTR_MASK0,
			BIT_MSK__INTR_MASK0__REG_INTR0_MASK0|
			BIT_MSK__INTR_MASK0__REG_INTR0_MASK1 |
			BIT_MSK__INTR_MASK0__REG_INTR0_MASK3 |
			BIT_MSK__INTR_MASK0__REG_INTR0_MASK5 |
			BIT_MSK__INTR_MASK0__REG_INTR0_MASK7);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR8_MASK_AON,
		BIT_MSK__RX_INTR8_MASK_AON__REG_INTR8_MASK1_AON);
	sii_platform_wr_reg8(pstDev, REG_ADDR__RX_INTR6_MASK_AON,
		BIT_MSK__RX_INTR6_MASK_AON__REG_INTR6_MASK0_AON);


#ifdef CONFIG_POWER_DOWN_EN
	sii_platform_wr_reg8(pstDev, REG_ADDR__INTR_MASK2,
		BIT_MSK__INTR_MASK2__REG_INTR2_MASK2 |
		BIT_MSK__INTR_MASK2__REG_INTR2_MASK6);

#endif
	s_rx_zone_enable(drv_obj, true);
	drv_obj->init_done = true;

	return status;
}

#ifdef CONFIG_HANDLE_BOARD_RESET
static void chip_reset_handler(void *p_obj)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	int default_val = 0;

	/* Unreliable depending on PLL_VCOCAL reg */
	default_val = sii_platform_rd_reg8(pstDev, REG_ADDR__PLL_VCOCAL);
	if (default_val < 0) {
		sii_log_debug("chip_reset_handler.default_val==0x%x\n",default_val);
		//goto done;
	}

	if (0x39 == default_val) {
		sii_log_err("Board got reset. restart driver\n");

		dmode_chip_shutdown(drv_obj);
		dmode_chip_initialize(drv_obj);
	}
//done:
	sii_timer_start(&drv_obj->chip_reset_poll, 1000);
}
#endif

struct driver_obj g_stdrv_obj;

/* public function definitions */
int sii_drv_init(void)
{
	//struct cp8630_driver_context stdrv_context;
	//struct cp8630_driver_context *drv_context = &stdrv_context;
	struct driver_obj *drv_obj = &g_stdrv_obj;
    pstSii9630_Device_t pstDevice = &g_stSii9630_Device;
	int tstatus = 0;
	int ret = 0;

    pstDevice->u8DevId = _I2C_TABLE_INDEX_Sil9630;
    pstDevice->u8SlaveAddr = SII_DRV_DEVICE_I2C_ADDR1;

    memset(&g_stdrv_obj,0,sizeof(struct driver_obj));

	//drv_context = kzalloc(sizeof(struct cp8630_driver_context),GFP_KERNEL);
	//if (!drv_context) {
	//	/*dev_err(dev, "failed to allocate driver data\n");*/
	//	return -ENOMEM;
	//}

	//drv_context->pstDev = dev->platform_data;
	//dev_set_drvdata(dev, drv_context);

	//drv_obj = kzalloc(sizeof(struct driver_obj), GFP_KERNEL);
	//if (!drv_obj) {
		/*dev_err(dev, "failed to allocate device data\n");*/
	//	ret = 1;
	//	goto free_mem;
	//}

	//drv_context->drv_obj = drv_obj;
	//drv_obj->drv_context = drv_context;
    drv_obj->pstDevice = pstDevice;
	drv_obj->input_unstable_counter = 0;

#if 0//def CONFIG_CAPTURE_REGISTER_DUMP
	sii_lib_log_init();
#endif

	sii_log_debug("CP8630 Driver initialization started\n");
	sii_log_debug("PHY mode: %s\n", "HDMI");

	//drv_context->gen_irq = gpio_to_irq(drv_context->pstDev->sk_gpios[DEV_GEN_INT_INDEX].gpio);
	/*drv_context->emsc_irq = gpio_to_irq(drv_context->pstDev->
				sk_gpios[DEV_EMSC_INT_INDEX].gpio);*/

    #if 0
	drv_context->isr_lock = FALSE;
	INIT_LIST_HEAD(&drv_context->timer_list);
	drv_context->timer_wrk_queue = create_workqueue(SII_DRV_NAME);
	if (drv_context->timer_wrk_queue == NULL)
		goto free_mem;
	ret = sii_drv_sysfs_init(drv_context, dev);
	if (ret) {
		sii_log_err("sysfs initialization failed\n");
		goto free_mem;
	}
    #endif

#if (CONFIG_SOURCE_ONLY == 0)
	/*tell spiderman drv_context*/
	sii_drv_spiderman_init(drv_obj);
#endif
	//ret = request_threaded_irq(drv_context->gen_irq, NULL,
	//	cp8630_gen_interrupt_handler, IRQF_TRIGGER_LOW | IRQF_ONESHOT,
	//	SII_DEVICE_NAME, drv_context);
    //cp8630_gen_interrupt_handler(drv_obj);

	//if (ret < 0) {
	//	sii_log_debug("request_threaded_irq failed, status: %d\n", ret);
	//	goto free_mem;
	//}

	/*rx create*/
	drv_obj->rx_hdmi_obj = sii_mod_rx_hdmi_init(pstDevice,
		s_rx_hdmi_call_back);
	if (!drv_obj->rx_hdmi_obj) {
		sii_log_debug("failed to create RX device\n");
		ret = 1;
		goto free_mem;
	}

	/*tx create*/
	drv_obj->tx_hdmi_obj = sii_mod_tx_hdmi_init(pstDevice,
		s_tx_hdmi_call_back);
	if (!drv_obj->tx_hdmi_obj) {
		sii_log_debug("failed to create TX HDMI device\n");
		ret = 1;
		goto free_mem;
	}

	/*HDCP create*/
	drv_obj->hdcp_obj = sii_mod_tx_hdcp_init(pstDevice,
		s_tx_hdcp_call_back);
	if (!drv_obj->hdcp_obj) {
		sii_log_debug("failed to create HDCP device\n");
		ret = 1;
		goto free_mem;
	}

	/*video path create*/
	drv_obj->vidpath_obj = sii_mod_vid_path_init(pstDevice,
		s_vid_path_call_back);
	if (!drv_obj->vidpath_obj) {
		sii_log_debug("failed to create VIDEOPATH device\n");
		ret = 1;
		goto free_mem;
	}

#ifdef CONFIG_DSC_EN
	/*DSC create */
	drv_obj->dsc_obj = sii_mod_tx_dsc_init(pstDevice, NULL);
	if (!drv_obj->dsc_obj) {
		sii_log_debug("failed to create DSC device\n");
		ret = 1;
		goto free_mem;
	}
#endif
	tstatus = sii_timer_create(s_temp_tdms_handler, drv_obj,
		&drv_obj->inst_temp_tmds_handler,eTimerSil_temp_tdms_handler);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate Temp Tmds timer!\n");
		ret = tstatus;
		goto free_mem;
	}

	tstatus = sii_timer_create(s_hdcp_auth_deglitch, drv_obj,
		&drv_obj->inst_auth_deglitch,eTimerSil_hdcp_auth_deglitch);
	if (tstatus != 0) {
		sii_log_debug("Failed to allocate HDCP Auth Deglitch timer!\n");
		ret = tstatus;
		goto free_mem;
	}

	tstatus = sii_timer_create(s_tx_tmds_enable_handler, drv_obj,
		&drv_obj->timer_tx_tmds_enable_poll,eTimerSil_tx_tmds);
	if (tstatus != 0) {
		sii_log_debug("Failed to alloc Tx tmds monitor timer!\n");
		ret = tstatus;
		goto free_mem;
	}

	/* Initialize dualmode transmitter */
	ret = dmode_chip_initialize(drv_obj);
	if (ret < 0) {
		sii_log_debug("Chip Initialization failed\n");
		goto free_mem;
	}
#ifdef CONFIG_HANDLE_BOARD_RESET
	else {
        #if SIL9630_USE_TIMER_PROCESS
		tstatus = sii_timer_create(chip_reset_handler, drv_obj,
				&drv_obj->chip_reset_poll,eTimerSil_chip_reset);
		if (tstatus != 0) {
			sii_log_debug("Failed to alloc Chip Reset Polling timer!\n");
			ret = tstatus;
			goto free_mem;
		} else {
			sii_timer_start(&drv_obj->chip_reset_poll, 1000);
		}
        #else
        chip_reset_handler(drv_obj);
        #endif
	}
#endif
	goto done;
free_mem:
	//sii_drv_exit(dev);
done:
	return ret;
}

#if 0
void sii_drv_exit(struct device *dev)
{
	struct driver_obj *drv_obj;
	struct cp8630_driver_context *drv_context =
		(struct cp8630_driver_context *)dev_get_drvdata(dev);

	if (!drv_context)
		return;

	drv_obj = (struct driver_obj *)drv_context->drv_obj;
	if (!drv_obj) {
		//kfree(drv_context);
		return;
	}

	if (drv_context->gen_irq)
	{
		free_irq(drv_context->gen_irq, drv_context);
	}

	drv_context->dev_flags |= DEV_FLAG_SHUTDOWN;

#ifdef CONFIG_HANDLE_BOARD_RESET
	if (drv_obj->chip_reset_poll) {
		sii_timer_stop(&drv_obj->chip_reset_poll);
		sii_timer_delete(&drv_obj->chip_reset_poll);
		drv_obj->chip_reset_poll = NULL;
	}
#endif
	if (drv_obj->timer_tx_tmds_enable_poll) {
		sii_timer_stop(&drv_obj->timer_tx_tmds_enable_poll);
		sii_timer_delete(&drv_obj->timer_tx_tmds_enable_poll);
		drv_obj->timer_tx_tmds_enable_poll = NULL;
	}
	if (drv_obj->inst_auth_deglitch) {
		sii_timer_stop(
			&drv_obj->inst_auth_deglitch);
		sii_timer_delete(
			&drv_obj->inst_auth_deglitch);
		drv_obj->inst_auth_deglitch = NULL;
	}
	if (drv_obj->inst_temp_tmds_handler) {
		sii_timer_stop(
			&drv_obj->inst_temp_tmds_handler);
		sii_timer_delete(
			&drv_obj->inst_temp_tmds_handler);
		drv_obj->inst_temp_tmds_handler = NULL;
	}

	if (drv_obj->dsc_obj)
		sii_mod_dsc_delete(drv_obj->dsc_obj);
	if (drv_obj->vidpath_obj)
		sii_mod_vid_path_exit(drv_obj->vidpath_obj);
	if (drv_obj->hdcp_obj)
		sii_mod_tx_hdcp_exit(drv_obj->hdcp_obj);
	if (drv_obj->tx_hdmi_obj)
		sii_mod_tx_hdmi_exit(drv_obj->tx_hdmi_obj);
	if (drv_obj->rx_hdmi_obj)
		sii_mod_rx_hdmi_exit(drv_obj->rx_hdmi_obj);
	if (drv_obj->drv_context->cp8630_dev != NULL)
		sii_drv_sysfs_exit(drv_obj->drv_context);

#if 0//def CONFIG_CAPTURE_REGISTER_DUMP
	sii_lib_log_exit();
#endif
	//kfree(drv_obj);
	//kfree(drv_context);
	pr_info("CP8630 driver unloaded Successfully\n");
}
#endif

#if 0
int sii_drv_handle_power_change_request(struct device *parent_dev,
	bool power_up)
{
	struct cp8630_driver_context *drv_context;
	int status = 0;

	drv_context = dev_get_drvdata(parent_dev);
	status = (drv_context->isr_lock);
	if (status) {
		sii_log_err("failed to acquire semaphore lock. status: %d\n",
			status);
		goto done;
	}

	if (power_up)
		status = dmode_chip_initialize(drv_context->drv_obj);
	else
		status = dmode_chip_shutdown(drv_context->drv_obj);

	//up(&drv_context->isr_lock);
done:
	return status;
}
#endif

int sii_drv_rx_hv_sync_polarity_get(void *p_obj,
		enum sii_hv_sync_pol *hv_sync_pol)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_vid_path_get(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__HVSYNC, (void *)hv_sync_pol);
}

int sii_drv_rx_out_h_v_sync_polarity_set(
	void *p_obj, enum sii_hv_sync_pol hv_sync_pol)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_vid_path_set(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__OUT_HVSYNC_POLARITY,
		(void *)&hv_sync_pol);
}

int sii_drv_rx_hdcp2_content_type_get(
	void *p_obj, enum sii_drv_hdcp_content_type *pcontent_type)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__HDCP_CONTENT_TYPE,
		(void *)pcontent_type);
}

int sii_drv_rx_hdcp_protection_get(void *p_obj, bool *b_hdcp_proc)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__HDCP_PROTECTION, (void *)b_hdcp_proc);
}

int sii_drv_rx_hdcp_status_get(void *p_obj,
				enum sii_drv_hdcp_status *p_hdcp_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__HDCP_STAT, (void *)p_hdcp_status);
}

int sii_drv_rx_av_mute_get(void *p_obj, bool *b_av_mute)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__AVMUTE, (void *)&b_av_mute);
}

int sii_drv_rx_infoframe_get(void *p_obj, struct sii_info_frame *p_info_frame)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__IF_DATA, (void *)p_info_frame);
}

int sii_drv_tx_dsc_enable(void *p_obj, bool on_off)
{
#ifdef CONFIG_DSC_EN
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	struct sii_info_frame  if_vsif;
#ifdef CONFIG_UNDEFINED_MACRO
	struct sii_drv_vid_timing_params ip_video_timing_params;
#endif
	struct sii_drv_vsif_caps vsifconfig = {0};
	const struct video_mode_type *video_timing;
	struct sii_info_frame avi_if;
	struct sii_info_frame acr_if;
	int status = 0;
	uint8_t i;
	bool enable = false;
	enum avi_color_space clr_spc = drv_obj->avi_data.payload.pb1.colorSpace;
	enum sii_drv_bit_depth colordepth = (enum sii_drv_bit_depth)0;

	s_tx_hdcp_enable(drv_obj, false);
	status = sii_mod_dsc_set(drv_obj->dsc_obj,
		SII_MOD_DSC_OPCODE__SET_DSC_EN, &on_off);
	if (status) {
		sii_log_err("failed to set DSC_ENC_EN\n");
		/*return status;*/
	}

	if (on_off) {
		if (!(drv_obj->avi_data.payload.pb4.vic ||
			drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic)) {
			sii_log_debug("No I/P Video Identification data available\n");
			return 1;
		}

		enable = false;
		status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
			&enable);
		if (status) {
			sii_log_err("set HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE failed\n");
			/*return status;*/
		}

		/* 0x00F2 -- 0xF2000 -- 0d991232
		 * 2Mhz clock so 0.5usec
		 * 991232 * 0.5 = 495.616msecs*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__SYS_CNTR_0, 0xF2);
		sii_platform_wr_reg8(pstDev, REG_ADDR__SYS_CNTR_1, 0x00);
		sii_platform_set_bit8(pstDev, REG_ADDR__SYS_CTRL3,
			BIT_MSK__SYS_CTRL3__REG_SYS_CNTR);
		/* enable intr1 interrupt for Counter.*/
		sii_platform_set_bit8(pstDev, REG_ADDR__INTR1_MASK,
			BIT_MSK__INTR1_MASK__REG_INTR1_MASK7);
		/*to modify PB6 TO PB8 in VSIF*/
		video_timing = sii_vid_timing_from_vic
			(drv_obj->avi_data.payload.pb4.vic,
			drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic);

		if (video_timing) {
			vsifconfig.h_active = video_timing->active.H;
			vsifconfig.v_active = video_timing->active.V;
		}

		if (clr_spc == ACS__YCBCR420)
			vsifconfig.v_active = vsifconfig.v_active >> 1;

		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH, &colordepth);
		if ((colordepth == SII_DRV_BIT_DEPTH__10_BIT)
			|| (colordepth == SII_DRV_BIT_DEPTH__12_BIT)) {
			sii_platform_set_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
				BIT_MSK__PKT_FILTER_0__REG_DROP_GCP_PKT);
		}

#ifdef CONFIG_UNDEFINED_MACRO
		status = sii_mod_vid_path_get(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS,
			&ip_video_timing_params);
		if (status)
			return status;
		vsifconfig.h_active = ip_video_timing_params.h_active;
		vsifconfig.v_active = ip_video_timing_params.v_active;
#endif
		status = sii_mod_dsc_set(drv_obj->dsc_obj,
			SII_MOD_DSC_OPCODE__INFOFRAME_UPDATE_VSIF,
			(void *)&vsifconfig);
		if (status) {
			sii_log_err("set DSC_OPCODE__INFOFRAME_UPDATE_VSIF failed\n");
			/*return status;*/
		}

		drv_obj->is_dsc_enabled = true;
		sii_log_info("DSC Enabled %dT1\n", drv_obj->dsc_compression);

#ifndef CONFIG_DSC_HDMI_VSIF
#ifdef CONFIG_ENHANCED_MODE_PROT
		if (!(is_ds_device_spiderman2(drv_obj)))
			transmit_dsc_vsif_packets(drv_obj);
#endif
#else
		{
			uint8_t *pl_u8b = NULL;
			/*get vsifinfoframe updated values*/
			if_vsif.if_id = (enum sii_info_frame_id)(SII_INFO_FRAME_ID__VSIF1);
			status = sii_mod_dsc_get(drv_obj->dsc_obj,
				SII_MOD_DSC_OPCODE__INFOFRAME_DATA,
				(void *)&(if_vsif));
			if (status)
				return status;
			if_vsif.is_if_on = true;
			/*send updated vsifinfoframe values to downstream*/
			status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__IF_DATA,
				(void *)&(if_vsif));
			if (status)
				return status;
			sii_log_print("HDMI VSIF block\n");
			pl_u8b = if_vsif.b;
			s_dump_buffer((uint8_t *)pl_u8b, SII_INFOFRAME_MAX_LEN);
		}
#endif
		if (!drv_obj->is_420toany_conv_en) {
			avi_if.if_id = SII_INFO_FRAME_ID__AVI;
			sii_mod_dsc_get(drv_obj->dsc_obj,
					SII_MOD_DSC_OPCODE__INFOFRAME_AVIF,
						&avi_if);
			avi_if.is_if_on = true;
			status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
					SII_MOD_HDMI_TX_OPCODE__IF_DATA,
					&avi_if);
			if (status) {
				sii_log_err("set HDMI_TX_OPCODE__IF_DATA failed\n");
				return status;
			}
		}

		sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG5,
			BIT_MSK__SW_RST_REG5__REG_TXZONE_SW_RST);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG5,
			BIT_MSK__SW_RST_REG5__REG_TXZONE_SW_RST);

		if (drv_obj->b_tx_connected && drv_obj->tx_tmds_enabled) {
#ifdef CONFIG_ENHANCED_MODE_PROT
			if (!(is_ds_device_spiderman2(drv_obj)))
				s_tx_hdcp_enable(drv_obj, true);
#else
			s_tx_hdcp_enable(drv_obj, true);
#endif
		}

	} else {
		enum avi_color_space color_space;
		/*clear acr if data to capture next time*/
		memset(&drv_obj->acr_if.b, 0, SII_INFOFRAME_MAX_LEN);
		memset(&drv_obj->acr_if_data, 0, sizeof(struct acr_info_frame));

		sii_platform_clr_bit8(pstDev, REG_ADDR__SYS_CTRL3,
			BIT_MSK__SYS_CTRL3__REG_SYS_CNTR);

		/* enable intr1 interrupt for Counter.*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__INTR1_MASK,
		BIT_MSK__INTR1_MASK__REG_INTR1_MASK7);

		/*go back default Pin mapping for videopath*/
		color_space = ACS__RGB;
		sii_mod_vid_path_set(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__PIN_MAPPING_CONFIG,
			(void *)&(color_space));

		/*Stop VSIF*/
		for (i = 0; i < 4; i++) {
			/*get vsifinfoframe id*/
			if_vsif.if_id = (enum sii_info_frame_id)
				(SII_INFO_FRAME_ID__VSIF1 + i);
			if_vsif.is_if_on = false;
			/*stop vsif*/
			status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__IF_DATA,
				(void *)&(if_vsif));
			if (status) {
				sii_log_err("set HDMI_TX_OPCODE__IF_DATA failed\n");
				/*return status;*/
			}
		}
		/*Clear DSC_420_MODE_EN*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_420_MODE_EN);
		/*passthrough gcp*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
				BIT_MSK__PKT_FILTER_0__REG_DROP_GCP_PKT);
		sii_platform_set_bit8(pstDev, REG_ADDR__BANNER_CONFIG5,
				BIT_MSK__BANNER_CONFIG5__REG_DC_BYPASS);
		/*passthrough vsif*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__PKT_FILTER_1,
			BIT_MSK__PKT_FILTER_1__REG_DROP_VSIF_PKT);
		/*passthrough AVI*/
		avi_if.if_id = SII_INFO_FRAME_ID__AVI;
		avi_if.is_if_on = false;
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__IF_DATA, &avi_if);
		/*passthrough ACR*/
		acr_if.if_id = SII_INFO_FRAME_ID__ACR;
		acr_if.is_if_on = false;
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__IF_DATA, &acr_if);
		s_sway_pll_enable(drv_obj, false);

		update_tx_tmds_clock(drv_obj, 0);

		if (drv_obj->b_tx_connected
			&& drv_obj->tx_tmds_enabled) {
			bool enable = true;
			status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
				&enable);
			if (status) {
				sii_log_err("set HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE failed\n");
				/*return status;*/
			}
		}

		drv_obj->is_dsc_enabled = false;
		drv_obj->dsc_compression = SII_DRV_DSC_COMPRESSION_RATIO__NONE;
		if (drv_obj->upsampled_to_444) {
			sii_drv_output_color_space_set(drv_obj, SII_DRV_CLRSPC__PASSTHRU);
			drv_obj->upsampled_to_444 = false;
			drv_obj->upsampled_to_422 = false;
		}

		sii_log_info("DSC disabled\n");
		/*sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG5,
			BIT_MSK__SW_RST_REG5__REG_TXZONE_SW_RST);
		sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG5,
			BIT_MSK__SW_RST_REG5__REG_TXZONE_SW_RST);*/

#ifdef CONFIG_ENHANCED_MODE_PROT
		if (is_ds_device_spiderman2(drv_obj)) {
			if (drv_obj->tx_tmds_enabled)
				switch_to_non_dsc_mode(drv_obj);
		} else if (drv_obj->b_tx_connected
			&& drv_obj->tx_tmds_enabled) {
			s_tx_hdcp_enable(drv_obj, true);
		}
#endif
	}

	return status;
#else
	sii_log_debug("DSC feature not enabled\n");
	return 1;
#endif
}

#ifdef CONFIG_ENHANCED_MODE_PROT
static bool is_ds_device_spiderman2(struct driver_obj *drv_obj)
{
/*not checking spiderman B manufacture id*/
#ifdef REMOVE_SCDC_DEPENDENCY_ON_EM_PROTOCOL
	drv_obj = drv_obj;
	return true;
#else
	char *pid = drv_obj->manf_spec_data.dev_id_str;
	if ((pid[0] == '9') && (pid[1] == '3')
		&& (pid[2] == '9') && (pid[3] == '6'))
		return true;
	else
		return false;
#endif
}

static int switch_to_dsc_mode(struct driver_obj *drv_obj, uint8_t compression)
{
	uint8_t vic = drv_obj->avi_data.payload.pb4.vic;
	uint8_t hdmi_vic = drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
	enum avi_color_space colorspace =
		drv_obj->avi_data.payload.pb1.colorSpace;
#endif
	uint8_t mode = 0;

#ifdef AVIF_INSTEAD_OF_VSIF_FOR_EMM
	uint8_t dsc_csc_info = 0;
#endif
	drv_obj->dsc_mode = EMM_HDMI2_NORMAL;

	if (vic) {
	switch (vic) {
	case 97:
	case 107:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K60_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K60_3T1;
		break;
	case 96:
	case 106:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K50_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K50_3T1;
		break;
	case 93:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K24_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K24_3T1;
		break;
	case 94:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K25_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K25_3T1;
		break;
	case 95:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K30_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K30_3T1;
		break;
	default:
		sii_log_err("DS doesn't support DSC for %d\n", vic);
		sii_log_err("switching to normal mode\n");
		break;
		}
	}

	if (hdmi_vic) {
		switch (hdmi_vic) {
		case 0x03:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K24_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K24_3T1;
		break;
	case 0x02:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K25_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K25_3T1;
		break;
	case 0x01:
		if (compression == SII_DRV_DSC_COMPRESSION_RATIO__2X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K30_2T1;
		else if (compression ==
			SII_DRV_DSC_COMPRESSION_RATIO__3X)
			drv_obj->dsc_mode = EMM_HDMI2_DSC_4K30_3T1;
		break;
	default:
		sii_log_err("DS doesn't support DSC for %d\n", vic);
		sii_log_err("switching to normal mode\n");
		break;
		}
	}
#ifndef AVIF_INSTEAD_OF_VSIF_FOR_EMM
	switch (colorspace) {
	case ACS__RGB:
	case ACS__YCBCR444:
		mode = ((uint8_t)drv_obj->dsc_mode | EMM_YC444_RGB);
		break;
	case ACS__YCBCR422:
		mode = ((uint8_t)drv_obj->dsc_mode | EMM_YC422);
		if (drv_obj->upsampled_to_444) {
			/*set conversion bit*/
			mode |= (1 << 5);
		}
		break;
	case ACS__YCBCR420:
		mode = ((uint8_t)drv_obj->dsc_mode | EMM_YC420);
		if (drv_obj->upsampled_to_422 && drv_obj->upsampled_to_444) {
			/*set conversion bit*/
			mode |= (1 << 5);
		}
		break;
	default:
		break;
	}

	sii_log_debug("sending EMM\n");
	return sii_mod_tx_hdmi_request_emm(drv_obj->tx_hdmi_obj, mode);
#else
	if (drv_obj->upsampled_to_444)
		dsc_csc_info = 0x02;
	else
		dsc_csc_info = 0x01;
	drv_obj->avi_out_data.b[1] |= (dsc_csc_info << 4);

	drv_obj->avi_out_data.b[3] = 0;
	drv_obj->avi_out_data.b[3] = calculate_generic_checksum(
		drv_obj->avi_out_data.b, 0, SII_INFOFRAME_MAX_LEN);
	drv_obj->avi_out_data.is_if_on = true;

	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__IF_DATA,
		&drv_obj->avi_out_data);
	sii_log_debug("sending EMM data in AVI: %d\n",
		(drv_obj->avi_out_data.b[1] & 0x30) >> 4);
	return sii_mod_tx_hdmi_request_emm(drv_obj->tx_hdmi_obj, mode);
#endif

}
#endif

static int dsc_configure(struct driver_obj *drv_obj, uint8_t compression)
{
#ifdef CONFIG_DSC_EN
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
#ifdef CONFIG_UNDEFINED_MACRO
	struct sii_drv_vid_timing_params ip_video_timing_params;
#endif
	struct sii_drv_dsc_caps dscconfig = { {0} };
	const struct video_mode_type *video_timing;
	int status = 0;
	uint32_t pixel_freq = 0;
	struct sii_info_frame avi_if;

	if (compression == 0) {
		sii_log_err("Invalid compression ratio\n");
		return 1;
	}

	dscconfig.compression = compression;
	drv_obj->dsc_compression = (enum sii_drv_dsc_compression_ratio)compression;

	dscconfig.hbr_status = (sii_platform_rd_reg8(pstDev,
		REG_ADDR__RX_AUDP_STAT)
		& BIT_MSK__RX_AUDP_STAT__HDMI_HBRA_ON)>>6;
	if (!(drv_obj->avi_data.payload.pb4.vic ||
			drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic)) {
		sii_log_debug("No I/P Video Identification data available\n");
		return 1;
	}

	video_timing = sii_vid_timing_from_vic
		(drv_obj->avi_data.payload.pb4.vic,
			drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic);

	if (video_timing) {
		dscconfig.h_active = video_timing->active.H;
		dscconfig.h_blank = video_timing->blank.H;
		dscconfig.color_space =
			drv_obj->avi_data.payload.pb1.colorSpace;

		/*Dont change the clrspc sequence*/
		if (dscconfig.color_space == ACS__YCBCR420) {
			if (drv_obj->is_420toany_conv_en) {
				/*this feature for compression followed by
				420-444 conversion*/
				dscconfig.color_space = ACS__YCBCR444;
			}

			if (drv_obj->upsampled_to_422) {
				/*upsampled by upstream spiderman*/
				dscconfig.color_space = ACS__YCBCR422;
			}
		}
		if (dscconfig.color_space == ACS__YCBCR422) {
			if (drv_obj->upsampled_to_444) {
				/*It is upsampled to 444*/
				dscconfig.color_space = ACS__YCBCR444;
			}
		}
	}

	if (!drv_obj->upsampled_to_444) {
		sii_mod_vid_path_set(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__PIN_MAPPING_CONFIG,
			&(dscconfig.color_space));
	} else if (drv_obj->avi_data.payload.pb1.colorSpace == ACS__YCBCR422) {
		sii_mod_vid_path_set(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__PIN_MAPPING_CONFIG,
			&(drv_obj->avi_data.payload.pb1.colorSpace));
	}

#ifdef CONFIG_UNDEFINED_MACRO
	status = sii_mod_vid_path_get(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__VIDEO_PARAMS, &ip_video_timing_params);
	if (status)
		return status;

	dscconfig.h_active = ip_video_timing_params.h_active;
	dscconfig.h_blank = ip_video_timing_params.h_blank;
#endif

	dscconfig.vic_id.cea_vic = drv_obj->avi_data.payload.pb4.vic;
	dscconfig.vic_id.hdmi_vic =
		drv_obj->vsif_hdmi_or_mhl3.payload.hdmi.hdmi_vic;

	status = sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
		(void *)&(dscconfig.colordepth));

	if (status) {
		sii_log_err("set RX_HDMI_OPCODE__IN_CLRDEPTH failed\n");
		return status;
	}

	if ((dscconfig.color_space == ACS__YCBCR444)
		&& (drv_obj->upsampled_to_444)) {
		dscconfig.colordepth = SII_DRV_BIT_DEPTH__10_BIT;
	}

	/*upate pixel clk*/
	status = sii_mod_dsc_set(drv_obj->dsc_obj,
		SII_MOD_DSC_OPCODE__UPDATE_PIXEL_CLOCK, (void *)&(dscconfig));
	if (status) {
		sii_log_err("set DSC_OPCODE__UPDATE_PIXEL_CLOCK failed\n");
		return status;
	}

	/*get output pixel clk*/
	status = sii_mod_dsc_get(drv_obj->dsc_obj,
			SII_MOD_DSC_OPCODE__COMPRESSED_PIXEL_CLOCK,
			(void *)&(dscconfig.pixel_freq));
	if (status) {
		sii_log_err("get DSC_OPCODE__COMPRESSED_PIXEL_CLOCK failed\n");
		return status;
	}
	pixel_freq = dscconfig.pixel_freq;

#ifdef CONFIG_DEFINED_MACRO
	/*if already enabled during convertion
	this feature for compression followed by
	420-444 conversion */
	if (!drv_obj->is_420toany_conv_en)
		s_sway_pll_configure(drv_obj, SWAY_INUSE_FOR__DSC, 0,
		pixel_freq);
#endif

	avi_if.if_id = SII_INFO_FRAME_ID__AVI;
	if (drv_obj->out_clr_spc == SII_DRV_CLRSPC__PASSTHRU) {
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IF_DATA,
			&avi_if);
	} else {
		memcpy(&avi_if, &drv_obj->avi_out_data,
			sizeof(struct sii_info_frame));
	}
	status = sii_mod_dsc_set(drv_obj->dsc_obj,
		SII_MOD_DSC_OPCODE__INFOFRAME_AVIF,
		&avi_if);
	if (status) {
		sii_log_err("set DSC_OPCODE__INFOFRAME_AVIF failed\n");
		return status;
	}

	status = sii_mod_dsc_set(drv_obj->dsc_obj,
		SII_MOD_DSC_OPCODE__WRAP_CONFIGURE,
		(void *)&dscconfig);

		return status;

#else
	sii_log_debug("DSC feature not enabled\n");
	return 1;
#endif
}

int sii_drv_tx_dsc_configure(void *p_obj, uint8_t compression)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	int status = 0;

#ifdef CONFIG_ENHANCED_MODE_PROT
	drv_obj->emm_response = 0;
	drv_obj->emm_response_received = false;
#endif
	/*s_tx_hdcp_enable(drv_obj, false);*/

	if (compression && drv_obj->is_dsc_enabled) {
		sii_log_err("dsc is already enabled\n");
		return 1;
	}

	switch (compression) {
	case SII_DRV_DSC_COMPRESSION_RATIO__2X:
	case SII_DRV_DSC_COMPRESSION_RATIO__3X:
		status = dsc_configure(drv_obj, compression);
		if (status)
			sii_log_debug("Failed to configure DSC\n");
		else {
			status = sii_drv_tx_dsc_enable(drv_obj, true);
#ifdef CONFIG_ENHANCED_MODE_PROT
			if (status == 0) {
				if (is_ds_device_spiderman2(drv_obj)) {
					msleep(300);
					switch_to_dsc_mode(drv_obj,
						drv_obj->dsc_compression);
				}
			}
#endif
		}
		break;
	case SII_DRV_DSC_COMPRESSION_RATIO__NONE:
		status = sii_drv_tx_dsc_enable(drv_obj, false);
		break;
	default:
		sii_log_err("Invalid compression rate\n");
		status = 1;
	}
	return status;
}

int sii_drv_rx_infoframe_on_off_get(void *p_obj,
	enum sii_info_frame_id if_id, bool *b_on_off)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	struct sii_info_frame if_data;

	if (p_obj) {
		if_data.if_id = if_id;
		if (!sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IF_DATA, (void *)&if_data)) {
				*b_on_off = if_data.is_if_on;
				return 0;
		}
	}
	return 1;
}

static int s_rx_hot_plug_toggle(void *p_obj)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	bool on_off = true;

	return sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__HPD_TOGGLE, (void *)&on_off);
}

int sii_drv_rx_hot_plug_set(void *p_obj, bool on_off)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__HPD, (void *)&on_off);
}

int sii_drv_rx_edid_set(void *p_obj, struct sii_edid *p_edid)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
    #ifdef CONFIG_EDID_SWITCH
    struct sii_edid l_stSiiEdid;
    #endif
	sii_log_debug("EDID being set to RX:\n");

    #ifdef CONFIG_EDID_SWITCH
    memset(&l_stSiiEdid,0,sizeof(struct sii_edid));
    if((sg_pu8Sii9630Edidbuf != NULL) && (sg_u16Sii9630EdidLen != 0))
    {
        l_stSiiEdid.size = sg_u16Sii9630EdidLen;
        memcpy(l_stSiiEdid.b,sg_pu8Sii9630Edidbuf,sg_u16Sii9630EdidLen);
        if(p_edid->b[0x7E])
        {
            mapp_CopyEdidPhysicalAddress(p_edid->b,l_stSiiEdid.b);
        }
    }
    else
    {
        l_stSiiEdid.size = p_edid->size;
        memcpy(l_stSiiEdid.b,p_edid->b,p_edid->size);
    }
    s_dump_buffer(l_stSiiEdid.b, l_stSiiEdid.size);
    return sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
        SII_MOD_RX_HDMI_OPCODE__EDID, (void *)&l_stSiiEdid);
    #else
	s_dump_buffer(p_edid->b, p_edid->size);
	return sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__EDID, (void *)p_edid);
    #endif
}

int sii_drv_rx_tmds_mode_get(void *p_obj, enum sii_tmds_mode *tmds_mode)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__TMDS_MODE, (void *)tmds_mode);
}

int sii_drv_rx_plus5v_status_get(void *p_obj, void *plus5v_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__RPWR, (void *)&plus5v_status);
}

int sii_drv_rx_hdcp_link_ver_get(
	void *p_obj, enum sii_drv_hdcp_link_version *p_hdcp_link_ver)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__LINK_VERSION, (void *)p_hdcp_link_ver);
}

int sii_drv_rx_clear_info_frame(void *p_obj, enum sii_info_frame_id *if_id)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__IF_CLEAR, (void *)if_id);
}

/* get input colorspace */
int sii_drv_rx_input_color_space_get(void *p_obj,
	enum sii_drv_clr_spc *p_clr_spc)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	*p_clr_spc = drv_obj->avi_data.payload.clr_spc;
	return 0;
}

int sii_drv_output_color_space_set(void *p_obj, enum sii_drv_clr_spc clr_spc)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	enum sii_drv_clr_spc temp_out_clr_spc = drv_obj->out_clr_spc;
	int32_t status;
	enum sii_drv_bit_depth ip_dc_mode;
	enum avi_color_space ip_clr_spc =
		drv_obj->avi_data.payload.pb1.colorSpace;
	struct sii_drv_color_info_cfg clr_info_cfg;
	drv_obj->upsampled_to_444 = false;

	sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
		&ip_dc_mode);

	if (clr_spc && (ip_dc_mode > SII_DRV_BIT_DEPTH__8_BIT)) {
		sii_log_err("Conversions in deep color - not supported\n");
		return 1;
	}
	drv_obj->out_clr_spc = clr_spc;
	if (ip_clr_spc == ACS__YCBCR420) {

		if (drv_obj->out_clr_spc != SII_DRV_CLRSPC__PASSTHRU) {
			s_tx_hdcp_enable(drv_obj, false);
			drv_obj->is_420toany_conv_en = true;
		} else {
			sii_platform_clr_bit8(pstDev, REG_ADDR__SYS_CTRL3,
				BIT_MSK__SYS_CTRL3__REG_SYS_CNTR);
			/*clear acr if data to capture next time*/
			memset(&drv_obj->acr_if.b, 0, SII_INFOFRAME_MAX_LEN);
			memset(&drv_obj->acr_if_data, 0,
					sizeof(struct acr_info_frame));
			/* clear intr1 interrupt for Counter.*/
			sii_platform_clr_bit8(pstDev, REG_ADDR__INTR1_MASK,
				BIT_MSK__INTR1_MASK__REG_INTR1_MASK7);
			drv_obj->is_420toany_conv_en = false;
		}
	}

	if (drv_obj->is_tpg_enabled) {
		clr_info_cfg.input_clr_spc =
			SII_DRV_CLRSPC__RGB_FULL;
		clr_info_cfg.input_clr_conv_std =
			SII_DRV_CONV_STD__BT_709;
		clr_info_cfg.input_vid_dc_depth =
			SII_DRV_BIT_DEPTH__8_BIT;
		sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
			SII_MOD_RX_HDMI_OPCODE__IN_CLRDEPTH,
			&clr_info_cfg.input_vid_dc_depth);
		sii_mod_vid_path_set(drv_obj->vidpath_obj,
			SII_MOD_VIDPATH_OPCODE__IN_VIDINFO,
			(void *)&clr_info_cfg);
	}

	if (!sii_mod_vid_path_set(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__OUT_CLRSPC,
		(void *)&clr_spc)) {
		status = 0;
	} else {
		drv_obj->out_clr_spc = temp_out_clr_spc;
		status = 1;
	}

	if (ip_clr_spc == ACS__YCBCR422) {
#ifdef CSC_AS_PER_INPUT_STANDARD
		if ((drv_obj->out_clr_spc == SII_DRV_CLRSPC__YC444_709)
			|| (drv_obj->out_clr_spc == SII_DRV_CLRSPC__YC444_2020))
#else
		if (drv_obj->out_clr_spc == SII_DRV_CLRSPC__YC444_709)
#endif
			drv_obj->upsampled_to_444 = true;
	}

	if (ip_clr_spc == ACS__YCBCR420) {
		bool enable = true;

		if (drv_obj->is_420toany_conv_en == true) {

			/* 0x00F2 -- 0xF2000 -- 0d991232
			* 2Mhz clock so 0.5usec
			* 991232 * 0.5 = 495.616msecs*/
			sii_platform_wr_reg8(pstDev, REG_ADDR__SYS_CNTR_0, 0xF2);
			sii_platform_wr_reg8(pstDev, REG_ADDR__SYS_CNTR_1, 0x00);

			sii_platform_set_bit8(pstDev, REG_ADDR__SYS_CTRL3,
				BIT_MSK__SYS_CTRL3__REG_SYS_CNTR);
			/* enable intr1 interrupt for Counter.*/
			sii_platform_set_bit8(pstDev, REG_ADDR__INTR1_MASK,
				BIT_MSK__INTR1_MASK__REG_INTR1_MASK7);

		}
		status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE,
			&enable);
		if (status) {
			sii_log_err("set HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE failed\n");
			return status;
		}
		/*enable HDCP again*/
		if (drv_obj->is_420toany_conv_en == true)
			s_tx_hdcp_enable(drv_obj, true);
	}

	return status;
}

int sii_drv_output_bit_depth_set(void *p_obj, enum sii_drv_bit_depth *bit_depth)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_vid_path_set(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__OUT_CLRDEPTH, (void *)bit_depth);
}

int sii_drv_color_info_config(void *p_obj,
				struct sii_drv_color_info_cfg clr_info)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_vid_path_set(drv_obj->vidpath_obj,
		SII_MOD_VIDPATH_OPCODE__IN_VIDINFO, (void *)&clr_info);
}

int sii_drv_tx_edid_get(void *p_obj, struct sii_edid *p_edid)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (p_obj) {
		memcpy(p_edid, &drv_obj->edid,	sizeof(struct sii_edid));
		return 0;
	}
	return 1;
}

int sii_drv_tx_lip_sync_info_get(void *p_obj,
					struct sii_edid_lip_sync *lip_sync)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__EDID_LIPSYNC, lip_sync);
}

int sii_drv_tx_connection_state_get(void *p_obj, bool *connection_state)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__HOTPLUG, connection_state);
}

int sii_drv_tx_hot_plug_get(void *p_obj, bool *b_hot_plug)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__HOTPLUG, (void *)b_hot_plug);
}

int sii_drv_tx_rsen_get(void *p_obj, bool *b_rsen)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__RSEN, (void *)b_rsen);
}

int sii_drv_tx_tmds_mode_set(void *p_obj, enum sii_tmds_mode tmds_mode)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	int status = 0;

	drv_obj->tx_tmds_mode_user = tmds_mode;

	if (tmds_mode == SII_TMDS_MODE__AUTO) {
		status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_MODE,
			(void *)&drv_obj->tx_tmds_mode);
	} else {
		if (tmds_mode == SII_TMDS_MODE__NONE) {
			drv_obj->b_tx_is_hdcp_req = false;
			sii_timer_stop(&drv_obj->timer_tx_tmds_enable_poll);
			s_tx_hdcp_enable(drv_obj, false);
		}

		status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__TMDS_MODE,
			(void *)&drv_obj->tx_tmds_mode_user);
	}

	return status;
}

int sii_drv_tx_tmds_mode_status_get(void *p_obj, enum sii_tmds_mode *tmds_mode)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__TMDS_MODE, (void *)tmds_mode);
}

int sii_drv_tx_av_mute_set(void *p_obj, bool on_off)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;

	sii_log_debug("Setting AVMUTE:: %s\n", on_off ? "ON" : "OFF");
	if (on_off) {
		sii_platform_set_bit8(pstDev, REG_ADDR__TPI_SC,
			BIT_MSK__TPI_SC__REG_TPI_AV_MUTE);
	} else {
		sii_platform_clr_bit8(pstDev, REG_ADDR__TPI_SC,
			BIT_MSK__TPI_SC__REG_TPI_AV_MUTE);
	}
	return 0;
}

int sii_drv_tx_infoframe_set(void *p_obj,
	const struct sii_info_frame *p_info_frame)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	sii_log_debug("SiiDrvTxInfoframeSet()::\n");
	return sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__IF_DATA, p_info_frame);
}

int sii_drv_tx_infoframe_on_off_set(void *p_obj,
	enum sii_info_frame_id if_id, bool on_off)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	/*={if_id, on_off? 0x01 : 0x00};*/
	struct sii_info_frame if_data;

	if_data.is_if_on = on_off;

	sii_log_debug("SiiDrvTxInfoframeOnOffSet():: ifId: %i, onOff: %s\n",
			if_id, on_off ? "ON" : "OFF");
	return sii_drv_tx_infoframe_set(drv_obj->tx_hdmi_obj, &if_data);
}

int sii_drv_tx_output_bit_depth_set(void *p_obj,
					enum sii_drv_bit_depth bit_depth)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	sii_log_debug("SiiDrvTxOutputBitDepthSet():: bitDepth : %i\n",
			bit_depth);
	return sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__OUTPUT_BIT_DEPTH, &bit_depth);
}
#if 0
int sii_drv_tx_hdcp_auth_type_set(void *p_obj,
	enum sii_drv_hdcp_link_version hdcp_link_ver)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	int status = 0;

	if (hdcp_link_ver) {
		drv_obj->tx_hdcp_auth_type = hdcp_link_ver;
		/*if (drv_obj->tx_hdcp_status)*/ {
			s_tx_hdcp_enable(drv_obj, false);
			s_tx_hdcp_enable(drv_obj, true);
		}
	} else {
		sii_log_err("Invalid HDCP Version\n");
		status = 1;
	}
	return status;
}
#endif
int sii_drv_tx_hdcp_protection_set(void *p_obj,
			enum sii_drv_hdcp_link_version hdcp_link_ver)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	int status = 0;

	drv_obj->tx_hdcp_auth_type = hdcp_link_ver;

	if (hdcp_link_ver)
		drv_obj->hdcp_arbitration_mask |= HDCP_ENABLE__API;
	else
		drv_obj->hdcp_arbitration_mask &= ~HDCP_ENABLE__API;

	if (hdcp_link_ver)
		sii_platform_wr_reg8(drv_obj->pstDevice,
			REG_ADDR__OTP_MUTE_CTRL, 0x00);
	else
		sii_platform_wr_reg8(drv_obj->pstDevice,
			REG_ADDR__OTP_MUTE_CTRL, 0xF0);

	status = s_tx_hdcp_prot_set(drv_obj);

	if (status)
		sii_platform_wr_reg8(drv_obj->pstDevice,
			REG_ADDR__OTP_MUTE_CTRL, 0xF0);

	return status;
}

int sii_drv_tx_hdcp_status_get(void *p_obj,
				enum sii_drv_hdcp_status *p_hdcp_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (drv_obj) {
		memcpy(p_hdcp_status, &(drv_obj->tx_hdcp_status),
		sizeof(drv_obj->tx_hdcp_status));
		return 0;
	}
	return 1;
}

int sii_drv_tx_hdcp_failure_reason_get(void *p_obj,
	enum sii_drv_hdcp_failure *p_hdcp_fail_reason)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (drv_obj) {
		sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__FAIL_REASON,
			(void*)p_hdcp_fail_reason);
		return 0;
	}
	return 1;
}


int sii_drv_tx_hdcp_version_get(void *p_obj,
			enum sii_drv_hdcp_link_version *p_ds_hdcp_ver)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (drv_obj) {
		sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
				SII_MOD_HDCP_OPCODE__VERSION,
				(void*)p_ds_hdcp_ver);
		return 0;
	}
	return 1;
}

/*Calling this will return current HDCP events then clears it*/
int sii_drv_tx_hdcp_events_get(void *p_obj,
				enum sii_drv_hdcp_gen_event *p_hdcp_events)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (drv_obj) {
		memcpy(p_hdcp_events, &(drv_obj->tx_hdcp_events),
		sizeof(drv_obj->tx_hdcp_events));
		drv_obj->tx_hdcp_events = SII_DRV_HDCP_EVENT__NONE;
		return 0;
	}
	return 1;
}


int sii_drv_tx_hdcp_stream_manage_msg_set(void *p_obj, struct sii_drv_tx_strmng_2x_msg* stream_manage_msg)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return	sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
			SII_MOD_HDCP_OPCODE__STMNG_SET,
			(void*)stream_manage_msg);
}

int sii_drv_tx_hdcp_bksv_list_get(
	void *p_obj, struct sii_drv_hdcp_ksv_list *p_bksv_list)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__BKSV_LIST, p_bksv_list);
}

int sii_drv_tx_hdcp_bksv_list_approval_set(void *p_obj, bool b_approved)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__BKSV_LIST_APPROVAL, &b_approved);
}

int sii_drv_tx_hdcp_topology_get(
	void *p_obj, struct sii_drv_hdcp_topology *p_topology)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__TOPOLOGY, p_topology);
}

int sii_drv_tx_hdcp_repeater_info(
	void *p_obj, enum hdcp_repeater_mode *repeater_mode)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__RPTR_MODE, repeater_mode);
}

int sii_drv_tx_hdcp2_content_type_set(
	void *p_obj, enum sii_drv_hdcp_content_type *p_content_type)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdcp_set(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__CONTENT_TYPE, p_content_type);
}

int sii_drv_tx_hdcp2_content_type_get(
	void *p_obj, enum sii_drv_hdcp_content_type *p_content_type)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdcp_get(drv_obj->hdcp_obj,
		SII_MOD_HDCP_OPCODE__CONTENT_TYPE, p_content_type);
}

int sii_drv_tx_scdc_manufacturer_registerstatus(
	void *p_obj, struct sii_drv_tx_scdc_manufacturer_status *manf_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_MANF_STATUS, manf_status);
}

int sii_drv_tx_scdc_peer_registerstatus(
	void *p_obj, struct sii_drv_tx_scdc_register_status *peer_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_PEER_REG_STATUS, peer_status);
}

int sii_drv_tx_scdc_source_scramble_status(
	void *p_obj,
	enum sii_drv_scdc_source_scrmble_status *scramble_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_SOURCE_SCRAMBLE_STATUS,
		scramble_status);
}

int sii_drv_tx_dsc_compression_status(void *p_obj, bool *compression_status)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	uint8_t status = 0;

	memcpy(compression_status, &drv_obj->is_dsc_enabled, sizeof(bool));
	sii_log_debug("compression status: %02d\n", *compression_status);

	return status;
}

int sii_drv_tx_dsc_compression_ratio(void *p_obj,
	 enum sii_drv_dsc_compression_ratio *compression_ratio)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	uint8_t status = 0;

	memcpy(compression_ratio, &(drv_obj->dsc_compression), sizeof(enum
				sii_drv_dsc_compression_ratio));

	return status;
}

int sii_drv_tx_scdc_read_request_test(void *p_obj, bool msdelay)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_ENABLE_READ_REQ_TEST, &msdelay);
}

int sii_drv_tx_scdc_read_request_enable(void *p_obj, bool enable)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_READ_REQ_CAP_SET, &enable);
}

int sii_drv_tx_scdc_debug_enable(void *p_obj, bool debug_enable)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_TIMEOUT_SET,
			&debug_enable);
}

int sii_drv_tx_scdc_scramble_enable(void *p_obj, bool enable)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	uint8_t in_data;
	int status = 0;

	in_data = enable ? 0x01 : 0x00;
	/*disable HDCP before enable scramble */
	if (drv_obj->tx_hdcp_status) {
		/*status = sii_drv_tx_hdcp_protection_set(p_obj, false);*/
		s_tx_hdcp_enable(drv_obj, false);
		/*if (status) {
			sii_log_err("set hdcp_protection failed\n");
			return status;
		}*/
		drv_obj->b_tx_is_hdcp_req = true;
	}

	status = sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE, &in_data);
	if (status) {
		sii_log_err("set HDMI_TX_OPCODE__SCDC_SCRAMBLE_ENABLE failed\n");
		return status;
	}

	if (drv_obj->b_tx_is_hdcp_req) {
		drv_obj->b_tx_is_hdcp_req = false;
		/*enable HDCP after enable scramble */
		/*status = sii_drv_tx_hdcp_protection_set(p_obj, true);*/
		s_tx_hdcp_enable(drv_obj, true);
	}

	return status;
}

int sii_drv_tx_scdc_read_update_registers(void *p_obj,
		struct sii_drv_tx_scdc_status_update_regs *status_update_values)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_get(drv_obj->tx_hdmi_obj,
			SII_MOD_HDMI_TX_OPCODE__READ_UPDATE_REGISTER,
			status_update_values);

}

#if 0
int sii_drv_cp8630_force_stop(void *p_obj, bool enable)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	if (enable) {
		if (drv_obj->drv_context->gen_irq)
			disable_irq_nosync(drv_obj->drv_context->gen_irq);
	} else {
		enable_irq(drv_obj->drv_context->gen_irq);
	}

	return 0;
}
#endif

/*TMDS BIST pattern generate*/
int sii_drv_bist_pattern_generate(void *p_obj,
	struct sii_drv_bist_config *bist_cfg)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
    pstSii9630_Device_t pstDev = drv_obj->pstDevice;
	uint32_t op_tmds_clk;
	uint8_t bist_tmg_val = 0x00;
	enum sii_tmds_mode tx_tmds_mode = SII_TMDS_MODE__HDMI1;
	uint8_t test_pat = 0x00;
	bool is_bist_disabled;
	struct sii_info_frame avi_tpg;
	uint8_t vic = 0;
	uint8_t checksum = 0;

	if (drv_obj == NULL)
		return 1;

	if (bist_cfg == NULL)
		return 1;

	if ((drv_obj->bist_owner == SII_DRV_BIST_OWNER__API) &&
		(bist_cfg->requester != SII_DRV_BIST_OWNER__API))
		return 1;

	if (bist_cfg->pat_type == SII_DRV_BIST_PATTERN_TYPE__DISABLE) {
		is_bist_disabled = false;
		bist_cfg->pat_type = SII_DRV_BIST_PATTERN_TYPE__NONE;
	} else
		is_bist_disabled = true;

	drv_obj->bist_owner = SII_DRV_BIST_OWNER__NONE;
	drv_obj->is_tpg_enabled = false;

	sii_platform_wr_reg8(pstDev, REG_ADDR__CLKDET_CTL_OVR, 0x68);

	switch (bist_cfg->pat_type) {
	default:
	case SII_DRV_BIST_PATTERN_TYPE__NONE:
		if (is_bist_disabled)
			s_tx_stop(drv_obj, TX_REQUESER__DRIVER);
		else
			drv_obj->bist_owner = SII_DRV_BIST_OWNER__API;
		/*remove force ckdt*/
		/*reading TX_BIST_CTRL register will give unkown value so,
		* writng value directly into it*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__TX_BIST_CTRL, 0x00);
		s_sway_pll_enable(drv_obj, false);
		update_input_color_info(drv_obj);
		/*passthrough AVI*/
		avi_tpg.if_id = SII_INFO_FRAME_ID__AVI;
		avi_tpg.is_if_on = false;
		sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
				SII_MOD_HDMI_TX_OPCODE__IF_DATA,
				&avi_tpg);
		/*disable 420 or dsc to cut sway output going to TX*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_ENC_EN);
		sii_platform_clr_bit8(pstDev, REG_ADDR__TX_CLK_CHAR_CTRL,
			BIT_MSK__TX_CLK_CHAR_CTRL__REG_XTAL_SWAY_REF_EN);
		sii_log_debug("TPG Disabled\n");
		return 0;
	case SII_DRV_BIST_PATTERN_TYPE__TEST:
		switch (bist_cfg->test_pat) {
		default:
		case SII_DRV_BIST_TEST_PATTERN__WALKING_ONE:
			test_pat |= TEST_WALKING_ONE;
			break;
		case SII_DRV_BIST_TEST_PATTERN__RAMP:
			test_pat |= TEST_RAMPING;
			break;
		case SII_DRV_BIST_TEST_PATTERN__LFSR_1:
			test_pat |= TEST_LFSR1;
			break;
		case SII_DRV_BIST_TEST_PATTERN__LFSR_2:
			test_pat |= TEST_LFSR2;
			break;
		case SII_DRV_BIST_TEST_PATTERN__STATIC_1:
			test_pat |= TEST_STATIC1;
			break;
		case SII_DRV_BIST_TEST_PATTERN__STATIC_2:
			test_pat |= TEST_STATIC2;
			break;
		case SII_DRV_BIST_TEST_PATTERN__MAX_SWITCH:
			test_pat |= TEST_MAX_SWITCH;
			break;
		case SII_DRV_BIST_TEST_PATTERN__TWO_COLUMN_MAX_SWITCH:
			test_pat |= TEST_2COL_SWITCH;
			break;
		case SII_DRV_BIST_TEST_PATTERN__SDVO_TMDS_HALF_CLK:
			test_pat |= TEST_SVDO_TMDS_HALF_CLK;
			break;
		case SII_DRV_BIST_TEST_PATTERN__SDVO_TMDS_8BIT_STATIC:
			test_pat |= TEST_SVDO_TMDS_8BIT_STATIC;
			break;
		case SII_DRV_BIST_TEST_PATTERN__10BIT_STATIC:
			test_pat |= TEST_SVDO_TMDS_10BIT_STATIC;
			bist_tmg_val |= CLR_DEPTH_10;
			break;
		case SII_DRV_BIST_TEST_PATTERN__4TMDS_PATTERN:
			test_pat |= TEST_ALL_4TMDS;
			break;
		}
		break;
	}

	switch (bist_cfg->bist_tmg) {
	case SII_DRV_BIST_TIMING__720P60:
		bist_tmg_val |= TIME_MODE__720P60;
		op_tmds_clk = 74250000;
		vic = 4;
		break;
	case SII_DRV_BIST_TIMING__576P50:
		bist_tmg_val |= TIME_MODE__576P50;
		op_tmds_clk = 27030000;
		vic = 17;
		break;
	case SII_DRV_BIST_TIMING__480P60:
		bist_tmg_val |= TIME_MODE__480P60;
		op_tmds_clk = 27030000;
		vic = 03;
		break;
	case SII_DRV_BIST_TIMING__1080P50:
		bist_tmg_val |= (TIME_MODE__1080P50 | REFRESH_RATE__1080P_50);
		op_tmds_clk = 148500000;
		vic = 31;
		break;
	case SII_DRV_BIST_TIMING__1080P60:
		bist_tmg_val |= (TIME_MODE__1080P50 | REFRESH_RATE__1080P_60);
		op_tmds_clk = 148500000;
		vic = 16;
		break;
	case SII_DRV_BIST_TIMING__4K2KP30:
		bist_tmg_val |= (TIME_MODE__4K2KP30 | REFRESH_RATE__4k2K_30);
		op_tmds_clk = 297000000;
		vic = 95;
		break;
	case SII_DRV_BIST_TIMING__4K2KP25:
		bist_tmg_val |= (TIME_MODE__4K2KP30 | REFRESH_RATE__4k2K_25);
		op_tmds_clk = 297000000;
		vic = 94;
		break;
	case SII_DRV_BIST_TIMING__4K2KP24:
		bist_tmg_val |= (TIME_MODE__4K2KP30 | REFRESH_RATE__4k2K_24);
		op_tmds_clk = 297000000;
		vic = 93;
		break;
	case SII_DRV_BIST_TIMING__FULLY_PROGRAMMABLE:
		/*not supported in FW, so setting it to default 720p*/
		bist_tmg_val |= TIME_MODE__720P60;
		op_tmds_clk = 74250000;
		vic = 4;
		break;
	case SII_DRV_BIST_TIMING__VGA:
		bist_tmg_val |= TIME_MODE__VGA;
		op_tmds_clk = 25100000;
		vic = 1;
		break;
	case SII_DRV_BIST_TIMING__SVGA:
		bist_tmg_val |= TIME_MODE__SVGA;
		op_tmds_clk = 49500000;
		break;
	case SII_DRV_BIST_TIMING__XGA:
		bist_tmg_val |= TIME_MODE__XGA;
		op_tmds_clk = 65000000;
		break;
	case SII_DRV_BIST_TIMING__SXGA:
		bist_tmg_val |= TIME_MODE__SXGA;
		op_tmds_clk = 106930000;
		break;
	case SII_DRV_BIST_TIMING__UXGA:
		bist_tmg_val |= TIME_MODE__UXGA;
		op_tmds_clk = 160000000;
		break;
	default:
		bist_tmg_val |= TIME_MODE__720P60;
		op_tmds_clk = 74250000;
		vic = 4;
		break;
	};
	switch (bist_cfg->clr_depth) {
	default:
	case SII_DRV_BIST_COLOR_DEPTH__8:
		bist_tmg_val |= CLR_DEPTH_8;
		break;
	case SII_DRV_BIST_COLOR_DEPTH__10:
		bist_tmg_val |= CLR_DEPTH_10;
		op_tmds_clk *= 10;
		op_tmds_clk /= 8;
		break;
	case SII_DRV_BIST_COLOR_DEPTH__12:
		bist_tmg_val |= CLR_DEPTH_12;
		op_tmds_clk *= 12;
		op_tmds_clk /= 8;
		break;
	}
	/* Construct Infoframe */
	avi_tpg.if_id = SII_INFO_FRAME_ID__AVI;
	avi_tpg.is_if_on = true;
	memset(avi_tpg.b, 0, SII_INFOFRAME_MAX_LEN);
	avi_tpg.b[0] = 0x82;
	avi_tpg.b[1] = 0x02;
	avi_tpg.b[2] = 0x0D;
	avi_tpg.b[3] = 0x00;
	avi_tpg.b[4] = 0x00;
	avi_tpg.b[5] = 0x28,
	avi_tpg.b[6] = 0x00;
	avi_tpg.b[7] = vic;
	avi_tpg.b[8] = 0x00;

	checksum = calculate_generic_checksum(avi_tpg.b, 0,
		SII_INFOFRAME_MAX_LEN);
	avi_tpg.b[3] = checksum;

	if (bist_cfg->requester == SII_DRV_BIST_OWNER__API)
		s_tx_stop(drv_obj, TX_REQUESER__BIST);
	update_tx_tmds_clock(drv_obj, op_tmds_clk);
	drv_obj->is_tpg_enabled = true;

	/*force ckdt*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__CLKDET_CTL_OVR, 0x03);

	/*reading TX_BIST_CTRL register will give unkown value so,
	* writng value directly into it*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_BIST_CTRL,
			BIT_MSK__TX_BIST_CTRL__REG_BIST_RESET);

	/*enable tx tbist -- mode Tx*/
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_TBIST_CTRL_21,
			BIT_MSK__TX_TBIST_CTRL_21__TX_TBIST);

	/*continous generation*/
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_BIST_DURATION0,
			BIT_MSK__TX_BIST_DURATION0__CONTINOUS_EN);

	/*disable stpg*/
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_BIST_CTRL2,
			BIT_MSK__TX_BIST_CTRL2__REG_OUT_SEL);

	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_BIST_TEST_SEL,
			test_pat);
	/* timing details*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_BIST_TIMING_CTRL,
			bist_tmg_val);
	/*start bist pattern */
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_BIST_CTRL,
			BIT_MSK__TX_BIST_CTRL__REG_BIST_ENABLE |
			BIT_MSK__TX_BIST_CTRL__REG_BIST_CONT_PROG_DURAT);
	sii_platform_wr_reg8(pstDev, REG_ADDR__TX_BIST_CTRL,
			BIT_MSK__TX_BIST_CTRL__REG_BIST_ENABLE |
			BIT_MSK__TX_BIST_CTRL__REG_BIST_CONT_PROG_DURAT |
			BIT_MSK__TX_BIST_CTRL__REG_BIST_START);

	/* select 24MHZ crystal as source to sway*/
	sii_platform_set_bit8(pstDev, REG_ADDR__TX_CLK_CHAR_CTRL,
			BIT_MSK__TX_CLK_CHAR_CTRL__REG_XTAL_SWAY_REF_EN);

	/*enable 420 or dsc for sway output to go to TX*/
	sii_platform_set_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_ENC_EN);

	s_sway_pll_configure(drv_obj, SWAY_INUSE_FOR__BIST, 0, op_tmds_clk);
	drv_obj->bist_owner = bist_cfg->requester;
	/*changed to here to enable autozone at Tx*/
	tx_start(drv_obj, TX_REQUESER__BIST, tx_tmds_mode);
	sii_log_debug("TPG Enabled\n");

	/*Trasmit avi infoframe*/
	sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__IF_DATA, &avi_tpg);
	return 0;
}

#ifdef RX_DESCRAMBLE_THROUGH_API
int sii_drv_rx_scdc_tmds_config(void *p_obj, bool scramble_en,
	bool tmds_bit_clk_ratio)
{
	int status = 0;
	bool p_buff[2] = {FALSE,FALSE};
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;
	p_buff[0] = scramble_en;
	p_buff[1] = tmds_bit_clk_ratio;
	status = sii_mod_rx_hdmi_set(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__RX_SCDC_CLK_RATIO,
		(void *)p_buff);
	return status;
}

int sii_drv_rx_scramble_status(void *p_obj, uint8_t *scramble_stat)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_rx_hdmi_get(drv_obj->rx_hdmi_obj,
		SII_MOD_RX_HDMI_OPCODE__RX_SCDC_SCRAMBLE_STAT,
		scramble_stat);
}
#endif

int sii_drv_tx_ddc_reset(void *p_obj, bool ddc_reset_en)
{
	struct driver_obj *drv_obj = (struct driver_obj *)p_obj;

	return sii_mod_tx_hdmi_set(drv_obj->tx_hdmi_obj,
		SII_MOD_HDMI_TX_OPCODE__DDC_RESET, &ddc_reset_en);
}

void mapp_Sii9630EdidSetForExternal(u8 *pu8Edid,u16 u16Len)
{
    sg_bEdidSwitchFlag = TRUE;
    sg_u16Sii9630EdidLen = u16Len;
    //memcpy(sg_u8Sii9630Edidbuf,pu8Edid,u16Len);
    sg_pu8Sii9630Edidbuf = pu8Edid;
}

int mapp_sii_rx_edid_switch_process(struct driver_obj *drv_obj)
{
    if(sg_bEdidSwitchFlag)
    {
        //struct sii_edid l_stEdid;
        //memset(&l_stEdid,0,sizeof(struct sii_edid));

        sg_bEdidSwitchFlag = false;
        //l_stEdid.size = sg_u16Sii9630EdidLen;
        //memcpy(l_stEdid.b,sg_pu8Sii9630Edidbuf,l_stEdid.size);
        sii_drv_rx_edid_set(drv_obj,(struct sii_edid *)(&drv_obj->edid));
    }
    return 0;
}

void mapp_sil9396_main(void)
{
    uint8_t ipc_queue = 1;

    while(ipc_queue != 0){
        uint8_t sa_reg2;
        sii_ipc_check_notify(ex_sp_pdata->rx_inst, &sa_reg2);
        if( sa_reg2 & SII_BIT_IPC_SLAVE_STATUS2_NTF_STATE_MASK ){
            sii_log_print("%s:%d start\n", __func__, __LINE__);
            sii_ipc_host_handler(ex_sp_pdata->rx_inst);
        }
        ipc_queue = sii_handle(ex_sp_pdata->rx_inst, ex_sp_pdata);
    }
}
void mapp_Sil9630_main(void)
{
#if 1
	struct driver_obj *drv_obj = &g_stdrv_obj;
    cp8630_gen_interrupt_handler(drv_obj);
#if !SIL9630_USE_TIMER_PROCESS

    mapp_sil9630RxHdmiHandler(drv_obj->rx_hdmi_obj);
    s_temp_tdms_handler((void *)drv_obj);
    s_hdcp_auth_deglitch((void *)drv_obj);
    s_tx_tmds_enable_handler((void *)drv_obj);
    mapp_Sil9630TxScdcHandler(drv_obj->tx_hdmi_obj);
    mapp_Sil9630TxHdcpHandler(drv_obj->hdcp_obj);
    #if 0//def CONFIG_HANDLE_BOARD_RESET
    chip_reset_handler((void *)drv_obj);
    #endif
#else
    s_timer_thread();
#endif
#else
    mapp_sil9396_main();
#endif
    mapp_sii_rx_edid_switch_process(drv_obj);
}

#endif
