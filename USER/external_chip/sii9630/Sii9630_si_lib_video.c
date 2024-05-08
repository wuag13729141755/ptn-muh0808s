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

//#include "Sii9630_si_lib_video_api.h"
//#include "Sii9630_si_platform_api.h"
#include "Sii9630_si_datatypes.h"
#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_lib_video.h"
#include "Sii9630_si_platform.h"

#define SII_UINT_FRAC(num, den)         (((((num) << 1) / (den)) + 1) >> 1)

#define SII_INT_FRAC(num, den)          ((0 > (((int)num) * ((int)den))) ? \
		((((((int)num) * 2) / ((int)den))-1)/2) :\
		((((((int)num) * 2)/((int)den)) + 1) / 2))

#define   d_array(p_mtx, l_row, l_col)   (((float *) &\
	p_mtx->p_arr)[(l_row) * p_mtx->col+(l_col)])

#define   d_is_na_nf(flt)           (((0x7f800000 - ((*(uint32_t *)&(flt)) & \
	0x7fffffff)) >> 31) ? (TRUE) : (FALSE)) /* check if float number */

#define AR_SQP   SII_LIB_VIDEO_PICTAR__SQP
#define AR_4X3   SII_LIB_VIDEO_PICTAR__4X3
#define AR_16X9  SII_LIB_VIDEO_PICTAR__16X9
#define AR_NONE  SII_LIB_VIDEO_PICTAR__NONE

/* repetition factor*/
/* x1 (no repetition)*/
#define	RP1		 0x01
/* x2 (doubled)*/
#define	RP2		 0x02
/* x4*/
#define	RP4		 0x04
/* any from x1 to x10*/
#define	RP_ANY	 0x80

/* progressive scan*/
#ifndef PROG
#define	PROG	 0
#endif
/* interlaced scan*/
#ifndef INTL
#define INTL	 1
#endif

/* positive pulse*/
#define	POS		 0
/* negative pulse*/
#define	NEG		 1

/* NTSC system (60_hz)*/
#define NTSC	 1
/* PAL system (50_hz)*/
#define PAL		 2

static bool s_is_correct_vm(enum sii_lib_video_v_m v_m);
static uint32_t s_hfreq_get(enum sii_lib_video_v_m v_m);
static uint32_t s_vfreq_get(enum sii_lib_video_v_m v_m);

struct sizes_pix {
	/* number of horizontal pixels*/
	uint16_t h;
	/* number of vertical pixels*/
	uint16_t v;
};

struct video_mode {
	enum sii_lib_video_v_m v_m;
	const char      *_label_str;
	/* in hz*/
	uint32_t        pixel_frq;
	enum sii_lib_video_pict_a_r pict_ar;
	/* CEA VIC for 4:3 picture aspect rate, 0 if not available*/
	uint8_t		vic4x3;
	/* CEA VIC for 16:9 picture aspect rate, 0 if not available*/
	uint8_t		vic16x9;
	/* HDMI VIC for 16:9 picture aspect rate, 0 if not available*/
	uint8_t		hdmi_vic;
	/* number of active pixels per frame*/
	struct sizes_pix      active;
	/* total number of pixels per frame*/
	struct sizes_pix      total;
	/* offset of sync pulse per frame*/
	struct sizes_pix      sync_offset;
	/* width of sync pulse per frame*/
	struct sizes_pix      sync_width;
	/* true for interlaced video*/
	uint8_t       interlaced;
	/* true on negative polarity for horizontal pulses*/
	uint8_t       h_pol;
	/* true on negative polarity for vertical pulses*/
	uint8_t       v_pol;
	/* 60/120/240_hz (1) or 50/100/200_hz (2) or unknown (0) t_vs*/
	uint8_t	ntsc_pal;
	/* allowed video pixel repetition*/
	uint8_t      repetition;
	/* maximum allowed audio sample rate for 8 channel audio in k_hz*/
	uint8_t	     max_audio_s_r8_ch;
};

static  float s_rgb_to_yc601[9] =

{
	0.299000f,  0.587000f,  0.114000f,
	-0.168736f, -0.331264f,  0.500000f,
	0.500000f, -0.418688f, -0.081312f
};

static  float s_r_g_bto_y_c709[9] =

{
	0.212600f,  0.715200f,  0.072200f,
	-0.114572f, -0.385428f,  0.500000f,
	0.500000f, -0.454153f, -0.045847f
};

static  float s_r_g_bto_r_g_b[9] =

{
	0.0000f,  1.0000f,  0.0000f,
	0.0000f,  0.0000f,  1.0000f,
	1.0000f,  0.0000f,  0.0000f
};


struct video_mode s_video_mode_table[] = {
	/*video mode              label            fpix      AR       VIC, HVIC
 {H/V active}, {H/V  total}, {_h_v_sync_o}, {_h_v_sync_w}, i/_p, h_pol, v_pol,
		syst, repetition  ,audio*/
	/* CEA-861D video modes*/
	{ SII_LIB_VIDEO_VM__VGA60,        "VGA60",     25174826, AR_SQP,
		1, 0, 0, {640, 480}, {800, 525}, {16, 10}, {96, 2},
		PROG, NEG, NEG, 0, RP1, 48},

	{ SII_LIB_VIDEO_VM__480P,         "480P",      27000000, AR_4X3,
		2, 3, 0, {720, 480}, {858, 525}, {16, 9}, {62, 6},
		PROG, NEG, NEG, NTSC, RP1, 48},

	{ SII_LIB_VIDEO_VM__720P60,       "720P60",    74175824, AR_16X9,
		0, 4, 0, {1280, 720}, {1650, 750}, {110, 5}, {40, 5},
		PROG, POS, POS, NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080I60,      "1080I60",   74175824, AR_16X9,
		0, 5, 0, {1920, 1080}, {2200, 1125}, {88, 5}, {44, 10},
		INTL, POS, POS, NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__480I2,        "480I2",     27000000, AR_4X3,
		6, 7, 0, {1440, 480}, {1716, 525}, {38, 9}, {124, 6},
		INTL, NEG, NEG, NTSC, RP2,  48},

	{ SII_LIB_VIDEO_VM__240P2A,       "240P2A",    27000000, AR_4X3,
		8, 9, 0, {1440, 240}, {1716, 262}, {38, 4}, {124, 3},
		PROG, NEG, NEG, NTSC, RP2,  48},

	{ SII_LIB_VIDEO_VM__240P2B,       "240P2B",    27000000, AR_4X3,
		8, 9, 0, {1440, 240}, {1716, 263}, {38, 5}, {124, 3},
		PROG, NEG, NEG, NTSC, RP2,  48},

	{ SII_LIB_VIDEO_VM__480I4,        "480I4",     54000000, AR_4X3,
		10, 11, 0, {2880, 480}, {3432, 525}, {76,  9}, {248, 6},
		INTL, NEG, NEG, NTSC, RP_ANY,  96},

	{ SII_LIB_VIDEO_VM__240P4A,       "240P4A",    54000000, AR_4X3,
		12, 13, 0, {2880, 240}, {3432, 262}, {76,  4}, {248, 3},
		PROG, NEG, NEG, NTSC, RP_ANY,  96},

	{ SII_LIB_VIDEO_VM__240P4B,       "240P4B",    54000000, AR_4X3,
		12, 13, 0, {2880, 240}, {3432, 263}, {76,  5}, {248, 3},
		PROG, NEG, NEG, NTSC, RP_ANY,  96},

	{ SII_LIB_VIDEO_VM__480P2,        "480P2",     54000000, AR_4X3,
		14, 15, 0, {1440, 480}, {1716, 525}, {32,  9}, {124, 6},
		PROG, NEG, NEG, NTSC, RP1 | RP2, 96},

	{ SII_LIB_VIDEO_VM__1080P60,      "1080P60",  148351648, AR_16X9,
		0, 16, 0, {1920, 1080}, {2200, 1125}, {88,  4}, {44, 5},
		PROG, POS, POS, NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__576P,         "576P",      27000000, AR_4X3,
		17, 18, 0, {720, 576}, {864, 625}, {12,  5}, {64, 5},
		PROG, NEG, NEG, PAL, RP1, 48},

	{ SII_LIB_VIDEO_VM__720P50,       "720P50",    74250000, AR_16X9,
		0, 19, 0, {1280, 720}, {1980, 750}, {440, 5}, {40, 5},
		PROG, POS, POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080I50,      "1080I50",   74250000, AR_16X9,
		0, 20, 0, {1920, 1080}, {2640, 1125}, {528, 5}, {44, 10},
		INTL, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__576I2,        "576I2",     27000000, AR_4X3,
		21, 22, 0, {1440, 576}, {1728, 625}, {24,  5}, {126, 6},
		INTL, NEG,  NEG,  PAL, RP2, 48},

	{ SII_LIB_VIDEO_VM__288P2A,       "288P2A",    27000000, AR_4X3,
		23, 24, 0, {1440, 288}, {1728, 312}, {24,  2}, {126, 3},
		PROG, NEG,  NEG,  PAL, RP2, 48},

	{ SII_LIB_VIDEO_VM__288P2B,       "288P2B",    27000000, AR_4X3,
		23, 24, 0, {1440, 288}, {1728, 313}, {24,  3}, {126, 3},
		PROG, NEG,  NEG,  PAL, RP2, 48},

	{ SII_LIB_VIDEO_VM__288P2C,       "288P2C",    27000000, AR_4X3,
		23, 24, 0, {1440, 288}, {1728, 314}, {24,  4}, {126, 3},
		PROG, NEG,  NEG,  PAL, RP2, 48},

	{ SII_LIB_VIDEO_VM__576I4,        "576I4",     54000000, AR_4X3,
		25, 26, 0, {2880, 576}, {3456, 625}, {48,  5}, {252, 6},
		INTL, NEG,  NEG,  PAL, RP_ANY, 96},

	{ SII_LIB_VIDEO_VM__288P4A,       "288P4A",    54000000, AR_4X3,
		27, 28, 0, {2880, 288}, {3456, 312}, {48,  2}, {252, 3},
		PROG, NEG,  NEG,  PAL, RP_ANY, 96},

	{ SII_LIB_VIDEO_VM__288P4B,       "288P4B",    54000000, AR_4X3,
		27, 28, 0, {2880, 288}, {3456, 313}, {48,  3}, {252, 3},
		PROG, NEG,  NEG,  PAL, RP_ANY, 96},

	{ SII_LIB_VIDEO_VM__288P4C,       "288P4C",    54000000, AR_4X3,
		27, 28, 0, {2880, 288}, {3456, 314}, {48,  4}, {252, 3},
		PROG, NEG,  NEG,  PAL, RP_ANY, 96},

	/* h-neg, v-pos*/
	{ SII_LIB_VIDEO_VM__576P2,        "576P2",         54000000, AR_4X3,
		29, 30, 0, {1440, 576}, {1728, 625}, {24,  5}, {128, 5},
		PROG, NEG, POS,  PAL, RP1 | RP2,  96},

	{ SII_LIB_VIDEO_VM__1080P50,      "1080P50",      148500000, AR_16X9,
		0, 31, 0, {1920, 1080}, {2640, 1125}, {528,  4}, {44, 5},
		PROG, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080P24,      "1080P24",       74175824, AR_16X9,
		0, 32, 0, {1920, 1080}, {2750, 1125}, {638,  4}, {44, 5},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080P25,      "1080P25",       74250000, AR_16X9,
		0, 33, 0, {1920, 1080}, {2640, 1125}, {528,  4}, {44, 5},
		PROG, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080P30,      "1080P30",       74175824, AR_16X9,
		0, 34, 0, {1920, 1080}, {2200, 1125}, {88,  4}, {44, 5},
		PROG, POS,  POS,  NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__480P4,        "480P4",        108000000, AR_4X3,
		35, 36, 0, {2880, 480}, {3432, 525}, {96,  9}, {248, 6},
		PROG, NEG,  NEG,  NTSC, RP1 | RP2 | RP4, 192},

	{ SII_LIB_VIDEO_VM__576P4,        "576P4",        108000000, AR_4X3,
		37, 38, 0, {2880, 576}, {3456, 625}, {48,  5}, {256, 5},
		PROG, NEG,  NEG,  PAL, RP1 | RP2 | RP4, 192},

	/* h-pos, v-neg, 1, 2 blanks are same = 85*/
	{ SII_LIB_VIDEO_VM__1080I50A,      "1080I50A",      72000000, AR_16X9,
		0, 39, 0, {1920, 1080}, {2304, 1250}, {32, 46}, {168, 10},
		INTL, POS,  NEG,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080I100,     "1080I100",     148500000, AR_16X9,
		0, 40, 0, {1920, 1080}, {2640, 1125}, {528,  5}, {44, 10},
		INTL, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__720P100,      "720P100",      148500000, AR_16X9,
		0, 41, 0, {1280, 720}, {1980, 750}, {440,  5}, {40, 5},
		PROG, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__576P100,      "576P100",       54000000, AR_4X3,
		42, 43, 0, {720, 576}, {864, 625}, {12,  5}, {64, 5},
		PROG, NEG,  NEG,  PAL, RP1,  96},

	{ SII_LIB_VIDEO_VM__576I100,      "576I100",       54000000, AR_4X3,
		44, 45, 0, {1440, 576}, {1728, 625}, {24,  5}, {126, 6},
		INTL, NEG,  NEG,  PAL, RP2,  96},

	{ SII_LIB_VIDEO_VM__1080I120,     "1080I120",     148351648, AR_16X9,
		0, 46, 0, {1920, 1080}, {2200, 1125}, {88,  5}, {44, 10},
		INTL, POS,  POS,  NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__720P120,      "720P120",      148351648, AR_16X9,
		0, 47, 0, {1280, 720}, {1650, 750}, {110,  5}, {40, 5},
		PROG, POS,  POS,  NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__480P120,      "480P120",       54000000, AR_4X3,
		48, 49, 0, {720, 480}, {858, 525}, {16,  9}, {62, 6},
		PROG, NEG,  NEG,  NTSC, RP1,  96},

	{ SII_LIB_VIDEO_VM__480I120,      "480I120",       54000000, AR_4X3,
		50, 51, 0, {1440, 480}, {1716, 525}, {38,  9}, {124, 6},
		INTL, NEG,  NEG,  NTSC, RP2,  96},

	{ SII_LIB_VIDEO_VM__576P200,      "576P200",      108000000, AR_4X3,
		52, 53, 0, {720, 576}, {864, 625}, {12,  5}, {64, 5},
		PROG, NEG,  NEG,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__576I200,      "576I200",      108000000, AR_4X3,
		54, 55, 0, {1440, 576}, {1728, 625}, {24,  5}, {126, 6},
		INTL, NEG,  NEG,  PAL, RP2, 192},

	{ SII_LIB_VIDEO_VM__480P240,      "480P240",      108000000, AR_4X3,
		56, 57, 0, {720, 480}, {858, 525}, {16,  9}, {62, 6},
		PROG, NEG,  NEG,  NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__480I240,      "480I240",      108000000, AR_4X3,
		58, 59, 0, {1440, 480}, {1716, 525}, {38,  9}, {124, 6},
		INTL, NEG,  NEG,  NTSC, RP2, 192},

	/* CEA-861E video modes*/
	{ SII_LIB_VIDEO_VM__720P24,       "720P24",        59400000, AR_16X9,
		0, 60, 0, {1280, 720}, {3300, 750}, {1760, 5}, { 40, 5},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__720P25,       "720P25",        74250000, AR_16X9,
		0, 61, 0, {1280, 720}, {3960, 750}, {2420, 5}, { 40, 5},
		PROG, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__720P30,       "720P30",        74250000, AR_16X9,
		0, 62, 0, {1280, 720}, {3300, 750}, {1760, 5}, { 40, 5},
		PROG, POS,  POS,  NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080P120,     "1080P120",     297000000, AR_16X9,
		0, 63, 0, {1920, 1080}, {2200, 1125}, {  88, 4}, { 44, 5},
		PROG, POS,  POS,  NTSC, RP1, 192},

	{ SII_LIB_VIDEO_VM__1080P100,     "1080P100",     297000000, AR_16X9,
		0, 64, 0, {1920, 1080}, {2640, 1125}, { 528, 4}, { 44, 5},
		PROG, POS,  POS,  PAL, RP1, 192},

	/* HDMI 1.4a video modes*/
	{ SII_LIB_VIDEO_VM__2160P30,      "2160P30",      296703298, AR_16X9,
		0, 0, 1, {3840, 2160}, {4400, 2250}, {176, 8}, {88, 10},
		PROG, POS,  POS,  NTSC, RP1,  96},

	{ SII_LIB_VIDEO_VM__2160P25,      "2160P25",      297000000, AR_16X9,
		0, 0, 2, {3840, 2160}, {5280, 2250}, {1056, 8}, {88, 10},
		PROG, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__2160P24,      "2160P24",      296703298, AR_16X9,
		0, 0, 3, {3840, 2160}, {5500, 2250}, {1276, 8}, {88, 10},
		PROG, POS,  POS,  0, RP1, 192},

	/* SMPTE (same pixel count as in 2160p24)*/
	{ SII_LIB_VIDEO_VM__2160P24S,     "2160P24S",     296703298, AR_16X9,
		0, 0, 4, {4096, 2160}, {5500, 2250}, {1020, 8}, {88, 10},
		PROG, POS,  POS,  0, RP1, 192},

	/* PC resolutions*/
	{ SII_LIB_VIDEO_VM__VGA72,        "VGA72",         31500000, AR_SQP,
		0, 0, 0, {640, 480}, {832, 520},   {24,  9}, {40, 3},
		PROG, NEG,  NEG,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__VGA75,        "VGA75",         31500000, AR_SQP,
		0, 0, 0, {640, 480}, {840, 500},   {16,  1}, {64, 3},
		PROG, NEG,  NEG,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__VGA85_1,      "VGA85_1",       36000000, AR_SQP,
		0, 0, 0, {640, 480}, {832, 509},   {56,  1}, {56, 3},
		PROG, NEG,  NEG,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__VGA85_2,      "VGA85_2",       35500000, AR_SQP,
		0, 0, 0, {720, 400}, {936, 446},   {36,  1}, {72, 3},
		PROG, NEG,  POS,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__SVGA56,       "SVGA56",        36000000, AR_SQP,
		0, 0, 0, {800, 600}, {1024, 625},  {24,  1}, {72, 2},
		PROG, POS,  POS,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__SVGA60,       "SVGA60",        39750330, AR_SQP,
		0, 0, 0, {800, 600}, {1056, 628},  {40,  1}, {128, 4},
		PROG, POS,  POS,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__SVGA72,       "SVGA72",        50000000, AR_SQP,
		0, 0, 0, {800, 600}, {1040, 666},  {56, 37}, {120, 6},
		PROG, POS,  POS,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__SVGA75,       "SVGA75",        50000000, AR_SQP,
		0, 0, 0, {800, 600}, {1056, 625},  {16,  1}, {80, 3},
		PROG, POS,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__SVGA85,       "SVGA85",        56250000, AR_SQP,
		0, 0, 0, {800, 600}, {1048, 631},  {32,  1}, {64, 3},
		PROG, POS,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__XGAI87,       "XGAI87",        44900000, AR_SQP,
		0, 0, 0, {1024, 768}, {1264, 817},  {8,  1}, {176, 8},
		INTL, POS,  POS,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__XGA60,        "XGA60",         64930910, AR_SQP,
		0, 0, 0, {1024, 768}, {1344, 806},  {24,  3}, {136, 6},
		PROG, NEG,  NEG,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__XGA70,        "XGA70",         75000000, AR_SQP,
		0, 0, 0, {1024, 768}, {1328, 806},  {24,  3}, {136, 6},
		PROG, NEG,  NEG,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__XGA75_1,      "XGA75_1",       78750000, AR_SQP,
		0, 0, 0, {1024, 768}, {1312, 800},  {16,  1}, {96, 3},
		PROG, POS,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__XGA85,        "XGA85",         94500000, AR_SQP,
		0, 0, 0, {1024, 768}, {1376, 808},  {48,  1}, {96, 3},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__XGA75_2,      "XGA75_2",      108108000, AR_SQP,
		0, 0, 0, {1152, 864}, {1600, 900},  {64,  1}, {128, 3},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__WXGA60_1,     "WXGA60_1",      68250000, AR_SQP,
		0, 0, 0, {1280, 768}, {1440, 790},  {48,  3}, {32, 7},
		PROG, POS,  NEG,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__WXGA60_2,     "WXGA60_2",      79500000, AR_SQP,
		0, 0, 0, {1280, 768}, {1664, 798},  {64,  3}, {128, 7},
		PROG, NEG,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__WXGA75,       "WXGA75",       102300000, AR_SQP,
		0, 0, 0, {1280, 768}, {1696, 805},  {80,  3}, {128, 7},
		PROG, NEG, POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__WXGA60_800,   "WXGA60_800",    83500000, AR_SQP,
		0, 0, 0, {1280, 800}, {1680, 831},  {72,  3}, {128, 6},
		PROG, NEG,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__WXGA60_3,     "WXGA60_3",     108108000, AR_SQP,
		0, 0, 0, {1280, 960}, {1800, 1000}, {96,  1}, {112, 3},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__SXGA60,       "SXGA60",       107856624, AR_SQP,
		0, 0, 0, {1280, 1024}, {1688, 1066}, {48,  1}, {112, 3},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__WSXGA60_1,    "WSXGA60_1",     85500000, AR_SQP,
		0, 0, 0, {1360, 768}, {1792, 795},  {64,  3}, {112, 6},
		PROG, POS,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__WSXGA60_2,    "WSXGA60_2",    101000000, AR_SQP,
		0, 0, 0, {1400, 1050}, {1560, 1080}, {48,  3}, {32, 4},
		PROG, POS,  NEG,  0, RP1,  48},

	{ SII_LIB_VIDEO_VM__WXGA85_1,     "WXGA85_1",     117500000, AR_SQP,
		0, 0, 0, {1280, 768}, {1712, 809},  {80,  3}, {136, 7},
		PROG, NEG,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__WXGA85_2,     "WXGA85_2",     148500000, AR_SQP,
		0, 0, 0, {1280, 960}, {1728, 1011}, {64,  1}, {160, 3},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__SXGA75,       "SXGA75",       135000000, AR_SQP,
		0, 0, 0, {1280, 1024}, {1688, 1066}, {16,  1}, {144, 3},
		PROG, POS,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__SXGA85,       "SXGA85",       157500000, AR_SQP,
		0, 0, 0, {1280, 1024}, {1728, 1072}, {64,  1}, {160, 3},
		PROG, POS,  POS,  0, RP1, 192},

	/* TODO: verify sync polarities*/
	{ SII_LIB_VIDEO_VM__HD60,         "HD60",         85860000, AR_SQP,
		0, 0, 0, {1366, 768}, {1800, 795},  {72,  1}, {144, 3},
		PROG, NEG,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__WSXGA60,      "WSXGA60",      121750000, AR_SQP,
		0, 0, 0, {1400, 1050}, {1864, 1089}, {88,  3}, {144, 4},
		PROG, NEG,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__WSXGA75,      "WSXGA75",      156000000, AR_SQP,
		0, 0, 0, {1400, 1050}, {1896, 1099}, {104,  3}, {144, 4},
		PROG, NEG,  POS,  0, RP1, 192},

	/* TODO: verify sync polarities*/
	{ SII_LIB_VIDEO_VM__WSXGA50,      "WSXGA50",       86750000, AR_SQP,
		0, 0, 0, {1440, 900}, {1872, 929},  {72,  3}, {144, 6},
		PROG, NEG,  POS,  0, RP1,  96},

	/* TODO: verify sync polarities*/
	{ SII_LIB_VIDEO_VM__HD50_PLUS,    "HD50_PLUS",     96500000, AR_SQP,
		0, 0, 0, {1600, 900}, {2080, 929},  {80,  3}, {160, 5},
		PROG, NEG,  POS,  0, RP1,  96},

	{ SII_LIB_VIDEO_VM__UXGA60,       "UXGA60",       161838162, AR_SQP,
		0, 0, 0, {1600, 1200}, {2160, 1250}, {64,  1}, {192, 3},
		PROG, POS,  POS,  0, RP1, 192},

	/* TODO: verify sync polarities*/
	{ SII_LIB_VIDEO_VM__WSXGA50_PLUS, "WSXGA50_PLUS",   119500000, AR_SQP,
		0, 0, 0, {1680, 1050}, {2208, 1083}, {88, 3}, {176, 6},
		PROG, NEG,  POS,  0, RP1, 192},

	/* TODO: verify sync polarities*/
	{ SII_LIB_VIDEO_VM__WUXGA50,      "WUXGA50",      158250000, AR_SQP,
		0, 0, 0, {1920, 1200}, {2560, 1238}, {120,  3}, {200, 6},
		PROG, NEG,  POS,  0, RP1, 192},

	{ SII_LIB_VIDEO_VM__WUXGA60,      "WUXGA60",      154000000, AR_SQP,
		0, 0, 0, {1920, 1200}, {2080, 1235}, {48,  3}, {32, 6},
		PROG, POS,  NEG,  0, RP1,  48},

	/* non-_p_c and non-_h_d_m_i resolutions*/
	{ SII_LIB_VIDEO_VM__480I,         "720X480i",      13500000, AR_4X3,
		0, 0, 0, {720, 480}, {858, 525}, {16,  9}, {62, 6},
		INTL, NEG,  NEG,  NTSC, RP1,  48},

	{ SII_LIB_VIDEO_VM__576I,         "720X576i",      13500000, AR_4X3,
		0, 0, 0, {720, 576}, {864, 625}, {12,  5}, {64, 5},
		INTL, NEG,  NEG,  PAL, RP1,  48},

	{ SII_LIB_VIDEO_VM__540X60,       "960X540",       39600000, AR_4X3,
		0, 0, 0, {960, 540}, {1100, 600}, {16, 20}, {32, 10},
		PROG, POS,  POS,  0, RP1,  48},

	/* HDMI 2.0 video modes*/
	{ SII_LIB_VIDEO_VM__2160P50,      "2160P50",      594000000, AR_16X9,
		0, 96, 2, {3840, 2160}, {5280, 2250}, {1056, 8}, {88, 10},
		PROG, POS,  POS,  PAL, RP1, 192},

	{ SII_LIB_VIDEO_VM__2160P60,      "2160P60",      593406594, AR_16X9,
		0, 97, 3, {3840, 2160}, {4400, 2250}, {176,  8}, {88, 10},
		PROG, POS,  POS,  NTSC, RP1, 192},

	/* end of table mark*/
	/* end of table mark*/
	{ SII_LIB_VIDEO_VM__NONE,         "EndOfTable",   0, AR_NONE,
		0, 0, 0, {0, 0}, {0, 0}, {0,  0}, {0, 0},
		0, 0,  0,  0, 0,   0}
};

struct mtx {
	uint8_t  row;
	uint8_t  col;
	const char     *p_str;
	float          *p_arr;
};

static void s_zero(struct mtx *p_mtx)
{
	uint8_t c, r;

	/* check for valid matrix */
	SII_PLATFORM_DEBUG_ASSERT(p_mtx);

	/* zero all matrix values */
	for (r = 0; r < p_mtx->row; r++) {
		for (c = 0; c < p_mtx->col; c++)
			d_array(p_mtx, r, c) = 0.0;
	}
}
static void s_check_same_size(struct mtx *p_mtx1, struct mtx *p_mtx2)
{
	if ((p_mtx1->row != p_mtx2->row) || (p_mtx1->col != p_mtx2->col)) {
		/*DEBUG_PRINT(MSG_ALWAYS, "ERROR: [%s] and [%s] are not the
		 * same size.\n", pMtx1->pStr, pMtx2->pStr);*/
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
}

static void s_check_multiply(struct mtx *p_mtx_d, struct mtx *p_mtx_s1,
				struct mtx *p_mtx_s2)
{
	if (p_mtx_s1->col != p_mtx_s2->row) {
		/*DEBUG_PRINT(MSG_ALWAYS, "ERROR: [%s] can not be multiplied
		 * by [%s].\n",
		 * pMtxS1->pStr, pMtxS2->pStr);*/
		SII_PLATFORM_DEBUG_ASSERT(0);
	} else if ((p_mtx_d->col != p_mtx_s2->col) ||
			(p_mtx_d->row != p_mtx_s1->row)) {
		/*DEBUG_PRINT(MSG_ALWAYS, "ERROR: result of multiplication
		 * doesn't fit into
		 * [%s].\n", pMtxD->pStr);*/
		SII_PLATFORM_DEBUG_ASSERT(0);
	}
}

static void s_copy(struct mtx *p_mtx_d, struct mtx *p_mtx_s)
{
	uint8_t c, r;

	/* check for valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(p_mtx_s);
	SII_PLATFORM_DEBUG_ASSERT(p_mtx_d);

	/* check same size */
	s_check_same_size(p_mtx_d, p_mtx_s);

	/* copy matrix content */
	for (r = 0; r < p_mtx_s->row; r++)
		for (c = 0; c < p_mtx_s->col; c++)
			d_array(p_mtx_d, r, c) = d_array(p_mtx_s, r, c);
}

static struct mtx sg_stmtx;
static struct mtx *s_create(uint8_t row, uint8_t col, const char *p_str)
{
	struct mtx *p_mtx = &sg_stmtx;
	//uint16_t size = sizeof(struct mtx) + sizeof(float) * col * row;

	//p_mtx = kzalloc(sizeof(size), GFP_KERNEL);
	memset(&sg_stmtx,0,sizeof(struct mtx));
	SII_PLATFORM_DEBUG_ASSERT(p_mtx);

	p_mtx->col = col;
	p_mtx->row = row;
	p_mtx->p_str = p_str;

	/* zero matrix */
	s_zero(p_mtx);

	return p_mtx;
}

static void s_delete(struct mtx *p_mtx)
{
	SII_PLATFORM_DEBUG_ASSERT(p_mtx);
	//kfree(p_mtx);
}

static void s_multiply(struct mtx *p_mtx_d, struct mtx *p_mtx_s1,
			struct mtx *p_mtx_s2)
{
	struct mtx *p_mtx_t;
	uint8_t c, r, s;

	/* check for valid matrices */
	SII_PLATFORM_DEBUG_ASSERT(p_mtx_d);
	SII_PLATFORM_DEBUG_ASSERT(p_mtx_s1);
	SII_PLATFORM_DEBUG_ASSERT(p_mtx_s2);

	p_mtx_t = s_create(p_mtx_s2->row, p_mtx_s2->col, "T");

	/* check if matrices can be multiplied */
	s_check_multiply(p_mtx_t, p_mtx_s1, p_mtx_s2);

	/* multiply destination matrix by source matrix */
	for (r = 0; r < p_mtx_s1->row; r++) {
		for (c = 0; c < p_mtx_s2->col; c++) {
			register float f_add = 0.0;

			for (s = 0; s < p_mtx_s2->row; s++)
				f_add += d_array(p_mtx_s1, r, s) *
					d_array(p_mtx_s2, s, c);

			d_array(p_mtx_t, r, c) = f_add;
		}
	}
	s_copy(p_mtx_d, p_mtx_t);
	s_delete(p_mtx_t);
}

static struct mtx *mtx_create(uint8_t row, uint8_t col, const char *p_str)
{
	return s_create(row, col, p_str);
}

static void mtx_delete(struct mtx *p_mtx)
{
	s_delete(p_mtx);
}

static void mtx_load(struct mtx *p_mtx, const float *p_arr)
{
	uint8_t r, c;

	SII_PLATFORM_DEBUG_ASSERT(p_mtx);
	for (r = 0; r < p_mtx->row; r++)
		for (c = 0; c < p_mtx->col; c++)
			d_array(p_mtx, r, c) = *(p_arr++);
}

static void mtx_set_coef(struct mtx *p_mtx, uint8_t row, uint8_t col,
				float value)
{
	d_array(p_mtx, row, col) = value;
}

static float mtx_get_coef(struct mtx *p_mtx, uint8_t row, uint8_t col)
{
	return d_array(p_mtx, row, col);
}

static void mtx_multiply(struct mtx *p_mtx_d, struct mtx *p_mtx_s1,
				struct mtx *p_mtx_s2)
{
	s_multiply(p_mtx_d, p_mtx_s1, p_mtx_s2);
}

void sii_lib_video_full_rgb_to_limited_rgb(
	struct sii_lib_video_r_g_b *p_rgb_o,
	const struct sii_lib_video_r_g_b *p_rgb_i)
{
	float val1 = (float)(219.0 / 255.0);
	float val2 = (float)(16.0 / 255.0);

	p_rgb_o->f_r = p_rgb_i->f_r * val1 + val2;
	p_rgb_o->f_g = p_rgb_i->f_g * val1 + val2;
	p_rgb_o->f_b = p_rgb_i->f_b * val1 + val2;
}

void sii_lib_video_rgb_to_yc709(struct sii_lib_video_y_u_v *p_yuv_o,
				const struct sii_lib_video_r_g_b *p_rgb_i)
{
	struct mtx *p_mtx = mtx_create(3, 3, "pMtx");
	struct mtx *p_vec = mtx_create(3, 1, "pVec");

	mtx_set_coef(p_vec, 0, 0, p_rgb_i->f_r);
	mtx_set_coef(p_vec, 1, 0, p_rgb_i->f_g);
	mtx_set_coef(p_vec, 2, 0, p_rgb_i->f_b);

	mtx_load(p_mtx, s_r_g_bto_y_c709);

	mtx_multiply(p_vec, p_mtx, p_vec);

	p_yuv_o->f_y = mtx_get_coef(p_vec, 0, 0);
	p_yuv_o->f_cb = mtx_get_coef(p_vec, 1, 0);
	p_yuv_o->f_cr = mtx_get_coef(p_vec, 2, 0);

	mtx_delete(p_vec);
	mtx_delete(p_mtx);
}

void sii_lib_video_quantize_yc(struct sii_lib_video_y_u_v *p_yuv, uint8_t bits)
{
	uint32_t msk = ((1 << bits) - 1);

	/* upper limit clipping */
	if (1.0 < p_yuv->f_y)
		p_yuv->f_y = 0.0 + 1.0;
	if (+0.5 < p_yuv->f_cb)
		p_yuv->f_cb = 0.0 + 0.5;
	if (+0.5 < p_yuv->f_cr)
		p_yuv->f_cr = 0.0 + 0.5;

	/* lower limit clipping */
	if (0.0 > p_yuv->f_y)
		p_yuv->f_y = 0.0;
	if (-0.5 > p_yuv->f_cb)
		p_yuv->f_cb = 0.0 - 0.5;
	if (-0.5 > p_yuv->f_cr)
		p_yuv->f_cr = 0.0 - 0.5;

	/* apply binary offset to cb and cr */
	p_yuv->f_cb += 0.5;
	p_yuv->f_cr += 0.5;

	/* quantization */
	p_yuv->f_y = (float)((uint32_t)(p_yuv->f_y * msk + 0.5));
	p_yuv->f_cb = (float)((uint32_t)(p_yuv->f_cb * msk + 0.5));
	p_yuv->f_cr = (float)((uint32_t)(p_yuv->f_cr * msk + 0.5));
}

void sii_lib_video_rgb_to_yc601(struct sii_lib_video_y_u_v *p_yuv_o,
				const struct sii_lib_video_r_g_b *p_rgb_i)
{
	struct mtx *p_mtx = mtx_create(3, 3, "pMtx");
	struct mtx *p_vec = mtx_create(3, 1, "pVec");

	mtx_set_coef(p_vec, 0, 0, p_rgb_i->f_r);
	mtx_set_coef(p_vec, 1, 0, p_rgb_i->f_g);
	mtx_set_coef(p_vec, 2, 0, p_rgb_i->f_b);

	mtx_load(p_mtx, s_rgb_to_yc601);

	mtx_multiply(p_vec, p_mtx, p_vec);

	p_yuv_o->f_y = mtx_get_coef(p_vec, 0, 0);
	p_yuv_o->f_cb = mtx_get_coef(p_vec, 1, 0);
	p_yuv_o->f_cr = mtx_get_coef(p_vec, 2, 0);

	mtx_delete(p_vec);
	mtx_delete(p_mtx);
}

void sii_lib_video_rgb_to_r_g_b(struct sii_lib_video_y_u_v *p_yuv_o,
				const struct sii_lib_video_r_g_b *p_rgb_i)
{
	struct mtx *p_mtx = mtx_create(3, 3, "pMtx");
	struct mtx *p_vec = mtx_create(3, 1, "pVec");

	mtx_set_coef(p_vec, 0, 0, p_rgb_i->f_r);
	mtx_set_coef(p_vec, 1, 0, p_rgb_i->f_g);
	mtx_set_coef(p_vec, 2, 0, p_rgb_i->f_b);

	mtx_load(p_mtx, s_r_g_bto_r_g_b);

	mtx_multiply(p_vec, p_mtx, p_vec);

	p_yuv_o->f_y = mtx_get_coef(p_vec, 0, 0);
	p_yuv_o->f_cb = mtx_get_coef(p_vec, 1, 0);
	p_yuv_o->f_cr = mtx_get_coef(p_vec, 2, 0);

	mtx_delete(p_vec);
	mtx_delete(p_mtx);
}

void sii_lib_video_quantize_rgb(struct sii_lib_video_r_g_b *p_rgb, uint8_t bits)
{
	uint32_t msk = ((1 << bits) - 1);

	/* upper limit clipping */
	if (1.0 < p_rgb->f_r)
		p_rgb->f_r = 1.0;
	if (1.0 < p_rgb->f_g)
		p_rgb->f_g = 1.0;
	if (1.0 < p_rgb->f_b)
		p_rgb->f_b = 1.0;

	/* lower limit clipping */
	if (0.0 > p_rgb->f_r)
		p_rgb->f_r = 0.0;
	if (0.0 > p_rgb->f_g)
		p_rgb->f_g = 0.0;
	if (0.0 > p_rgb->f_b)
		p_rgb->f_b = 0.0;

	/* quantization */
	p_rgb->f_r = (float)((uint32_t)(p_rgb->f_r * msk + 0.5));
	p_rgb->f_g = (float)((uint32_t)(p_rgb->f_g * msk + 0.5));
	p_rgb->f_b = (float)((uint32_t)(p_rgb->f_b * msk + 0.5));
}

void sii_lib_c_e_a_video_mode_get(uint8_t vic_i_d,
					enum sii_lib_video_v_m *p_vid_mode)
{
	uint8_t i;

	for (i = 0; i < SII_LIB_VIDEO_VM__NONE; i++) {
		if ((vic_i_d == s_video_mode_table[i].vic4x3) ||
			(vic_i_d == s_video_mode_table[i].vic16x9)) {
			p_vid_mode = &(s_video_mode_table[i].v_m);
		}
	}
    UNUSED(p_vid_mode);
}

void sii_lib_h_d_m_i_video_mode_get(uint8_t vic_i_d,
					enum sii_lib_video_v_m *p_vid_mode)
{
	uint8_t i;

	for (i = 0; i < SII_LIB_VIDEO_VM__NONE; i++) {
		if (vic_i_d == s_video_mode_table[i].hdmi_vic)
			p_vid_mode = &(s_video_mode_table[i].v_m);
	}
    UNUSED(p_vid_mode);
}

/*-------------------------------------------------------------------*/
/*
 *! @brief      return video format timing details.
 *!
 *! note: for video formats allowing both 59.94 and 60 hz timings
 *!       sii_lib_video_timing_detail_get() returns timing for 59.94 hz formats
 *! note: some video formats allow more than one aspect ratio.
 *!       for such formats the function returns a default one.
 *!
 *! @param[in]  v_m       video format ID
 *! @param[in]  mode3_d   3D format specifier
 *! @param[in]  per_field true- return timing per video field;
 *		false-per video frame
 *! @param[out] p_timing  pointer to a timing structure to fill the result in
 */

/*--------------------------x-----------------------------------------*/

void sii_lib_video_timing_detail_get(enum sii_lib_video_v_m v_m,
					uint16_t mode3_d, bool per_field,
					struct sii_lib_video_timing *p_timing)
{
	/* make sure that correct v_m index is supplied */
	if (s_is_correct_vm(v_m)) {
		/* get data from the table (to be modified according
		 * to 3D mode)*/
		uint32_t pixel_freq = s_video_mode_table[v_m].pixel_frq;
		uint32_t h_freq = s_hfreq_get(v_m);
		uint32_t v_freq = s_vfreq_get(v_m);
		bool interlaced = (s_video_mode_table[v_m].interlaced == INTL)?TRUE:FALSE;
		bool v_pol = (bool)s_video_mode_table[v_m].v_pol;
		bool h_pol = (bool)s_video_mode_table[v_m].h_pol;
		uint16_t v_total = s_video_mode_table[v_m].total.v;
		uint16_t h_total = s_video_mode_table[v_m].total.h;
		uint16_t v_sync = s_video_mode_table[v_m].sync_width.v;
		uint16_t v_front_porch = s_video_mode_table[v_m].sync_offset.v;
		uint16_t v_active = s_video_mode_table[v_m].active.v;
		uint16_t h_sync = s_video_mode_table[v_m].sync_width.h;
		uint16_t h_front_porch = s_video_mode_table[v_m].sync_offset.h;
		uint16_t h_active = s_video_mode_table[v_m].active.h;
		uint16_t v_blank = v_total - v_active;

		/* correct data taken from the table according to 3D mode*/
		switch (mode3_d) {
		/* frame packing: progressive/interlaced*/
		case SII_LIB_VIDEO_MOD3D__FP:
			v_total *= 2;
			pixel_freq *= 2;
			if (interlaced) {
				/* interlaced*/
				v_active = (v_active * 2) + v_blank +
					(v_blank + 1) / 2;

				/* convert v_sync and v_front_porch
				 * frame information into field
				 * information */
				v_sync /= 2;
				v_front_porch /= 2;

				/* one frame contains even and odd images
				 * (all 4 fields)
				 * and even it is interlaced from a scaler
				 * perspective, it appears as progressive
				 * format for HDMI receivers/transmitters.*/
			} else {
				/* progressive*/
				v_active = (v_active * 2) + v_blank;
			}
			break;

			/* field alternative: interlaced only*/
		case SII_LIB_VIDEO_MOD3D__FA:
			/* frame: left odd  with V sync,
			 *		right odd  without V sync,*/
			/*        left even with V sync,
			 *		right even without V sync.*/
			/* the timing returning by the function corresponds to*/
			/* the entire frame that includes the 4 fields.*/
			v_active *= 2;
			v_total *= 2;
			pixel_freq *= 2;
			break;

			/* line alternative: progressive only*/
		case SII_LIB_VIDEO_MOD3D__LA:
			v_sync *= 2;
			v_front_porch *= 2;
			v_active *= 2;
			v_total *= 2;
			pixel_freq *= 2;
			break;

			/* side-by-_side (full): progressive/interlaced*/
		case SII_LIB_VIDEO_MOD3D__SBSF:
			h_sync *= 2;
			h_front_porch *= 2;
			h_active *= 2;
			h_total *= 2;
			pixel_freq *= 2;
			break;

			/* L + depth: progressive only*/
		case SII_LIB_VIDEO_MOD3D__LD:
			v_active = v_active * 2 + v_blank;
			v_total *= 2;
			pixel_freq *= 2;
			break;

			/* L + depth + graphics + graphics-depth:
			 * progressive only*/
		case SII_LIB_VIDEO_MOD3D__LDG:
			v_active = v_active * 4 + v_blank * 3;
			v_total *= 4;
			pixel_freq *= 4;
			break;

		default:
			break;
		}

		/* re-calculate v_blank as the no.of lines may have changed*/
		v_blank = v_total - v_active;

		p_timing->v_total         = v_total;
		p_timing->v_front_porch   = v_front_porch;
		p_timing->v_sync          = v_sync;
		p_timing->v_back_porch    = v_blank - v_front_porch - v_sync;
		p_timing->v_active        = v_active;
		p_timing->h_total         = h_total;
		p_timing->h_front_porch   = h_front_porch;
		p_timing->h_sync          = h_sync;
		p_timing->h_back_porch    = h_total - h_active -
						h_front_porch - h_sync;
		p_timing->h_active        = h_active;
		p_timing->b_interlaced    = interlaced;
		p_timing->b_vpol          = v_pol;
		p_timing->b_hpol          = h_pol;
		p_timing->pixel_freq      = pixel_freq;
		p_timing->line_freq       = h_freq;
		p_timing->v_freq          = v_freq;
		/* default AR*/
		p_timing->pict_ar = s_video_mode_table[v_m].pict_ar;

		p_timing->mode3_d = mode3_d;

		if (per_field && p_timing->b_interlaced &&
				(SII_LIB_VIDEO_MOD3D__FP != mode3_d)) {
			/* convert data from "per frame" to "per field"*/
			p_timing->v_freq *= 2;
			p_timing->v_total /= 2;
			p_timing->v_front_porch /= 2;
			p_timing->v_sync /= 2;
			p_timing->v_active /= 2;
			p_timing->v_back_porch = p_timing->v_total -
				p_timing->v_active - p_timing->v_front_porch -
				p_timing->v_sync;
		}
	} else {
		/*SII_PLATFORM_DEBUG_ASSERT(0);*/
		memset(&p_timing, 0, sizeof(struct sii_lib_video_timing));
	}
}

static uint32_t s_hfreq_get(enum sii_lib_video_v_m v_m)
{
	struct video_mode *p_vm_tbl = &s_video_mode_table[v_m];

	return SII_UINT_FRAC(p_vm_tbl->pixel_frq, p_vm_tbl->total.h);
}

static uint32_t s_vfreq_get(enum sii_lib_video_v_m v_m)
{
	struct video_mode *p_vm_tbl = &s_video_mode_table[v_m];

	return SII_UINT_FRAC(p_vm_tbl->pixel_frq,
		p_vm_tbl->total.h * p_vm_tbl->total.v);
}

/*------------------------------------------------------------------*/
/*
 *! @brief      verifies if video ID is valid or not.
 *!
 *! @param[in]  v_m video mode index
 *!
 *! @retval     true  given video format is valid
 *! @retval     false given video format is invalid
 */
/*-------------------------------------------------------------------*/

static bool s_is_correct_vm(enum sii_lib_video_v_m v_m)
{
	if (SII_LIB_VIDEO_VM__NONE <= v_m)
		return false;

	if (s_video_mode_table[v_m].v_m != v_m) {
		/*DEBUG_PRINT(MSG_ALWAYS,
		 * "si_lib_video_stdfrm: VideoModeTable[%d].%d = %d\n",
		 v_m, video_mode_table[v_m].v_m);*/
		return false;
	}

	return true;
}

#endif  //#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
/***** end of file ***********************************************************/


