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

#ifndef __SI_LIB_EDID_VIDEO_TABLES_H__
#define __SI_LIB_EDID_VIDEO_TABLES_H__

struct pixs_type {
	uint16_t	H;	/* Number of horizontal pixels */
	uint16_t	V;	/* Number of vertical pixels */
};
#define MEMORY_OPTIMIZE_VALENS
#ifdef MEMORY_OPTIMIZE_VALENS
struct video_mode_type {
	/* VIC for 4:3 picture aspect rate, 0 if not avaliable */
	uint8_t			vic_4x3;

	/* VIC for 16:9 picture aspect rate,  0 if not avaliable */
	uint8_t			vic_16x9;

	/* Number of active pixels*/
	struct pixs_type		active;
	/* Number of blank pixels */
	struct pixs_type		blank;
	uint16_t		pix_clk_10khz;	/* in 10kHz units */
};
#else
struct video_mode_type {

	/* VIC for 4:3 picture aspect rate, 0 if not avaliable */
	uint8_t			vic_4x3;

	/* VIC for 16:9 picture aspect rate,  0 if not avaliable */
	uint8_t			vic_16x9;

	/* VIC for 64:27 picture aspect rate, 0 if not avaliable */
	uint8_t			vic_64x27;

	/* VIC for 256:135 picture aspect rate,  0 if not avaliable */
	uint8_t			vic_256x135;

	/* Number of active pixels*/
	struct pixs_type		active;
	/* Total number of pixels */
	struct pixs_type		total;
	/* Number of blank pixels */
	struct pixs_type		blank;
	/* Offset of sync pulse */
	struct pixs_type		sync_offset;
	/* Width of sync pulse */
	struct pixs_type		sync_width;

	uint8_t			h_freq;		/* in kHz */
	uint8_t			v_freq;		/* in Hz */
	uint16_t		pix_freq_mhz;	/* in MHz */
	uint16_t		pix_clk_10khz;	/* in 10kHz units */
	uint8_t			interlaced;	/* true for interlaced video */
	/* true on negative polarity for horizontal pulses */
	uint8_t			h_pol;
	/* true on negative polarity for vertical pulses */
	uint8_t			v_pol;
	/* 60/120/240Hz (false) or 50/100/200Hz (true) TVs */
	uint8_t			ntsc_pal;
	/* Allowed video pixel repetition */
	uint8_t			repetition;
	/* maximum allowed audio sample rate for 8 channel audio in kHz */
	uint8_t			max_audio_sr_8ch;
};
#endif



struct hdmi_to_cea_vic {
	uint8_t hdmi_vic;
	uint8_t cea_vic1;
	uint8_t cea_vic2;
};


/* repetition factor */
#define	RP1		0x01	/* x1 (no repetition) */
#define	RP2		0x02	/* x2 (doubled) */
#define	RP4		0x04	/* x4 */
#define	RP5		0x08	/* x5 */
#define	RP7		0x10	/* x7 */
#define	RP8		0x20	/* x8 */
#define	RP10	0x40	/* x10 */

#ifndef PROG
#define	PROG	0	/* progressive scan */
#endif
#ifndef INTL
#define INTL	1	/* interlaced scan */
#endif
#define	POS		0	/* positive pulse */
#define	NEG		1	/* negative pulse */

#define NTSC	1	/* NTSC system (60Hz) */
#define PAL		2	/* PAL system (50Hz) */

#define LAST_KNOWN_VIC	108

const struct video_mode_type *sii_vid_timing_from_vic(uint8_t cea_vic,
	uint8_t hdmi_vic);

uint8_t sii_map_hdmi_vic_to_cea_vic(uint8_t hdmi_vic);

#endif /* __SI_LIB_EDID_VIDEO_TABLES_H__*/
