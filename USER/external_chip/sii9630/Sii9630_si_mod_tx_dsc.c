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

#include "Sii9630_si_datatypes.h"
#if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)

#include "Sii9630_si_mod_tx_dsc.h"
#include "Sii9630_si_drv_cp8630_regs.h"
#include "Sii9630_si_platform.h"
#include "Sii9630_si_lib_log.h"

#ifndef COMPRESSION_2X_420_10
#define COMPRESSION_2X_420_10
#endif

#ifndef CONFIG_DEFINED_MACRO
#define CONFIG_DEFINED_MACRO
#endif

/* positive pulse*/
#define	POS		 0
/* negative pulse*/
#define	NEG		 1
#define	COMPRESSION_2X			2
#define	COMPRESSION_3X			3

struct dsc_obj {
	//struct cp8630_driver_context *drv_context;
    pstSii9630_Device_t pstDevice;
	struct sii_drv_dsc_caps wrap_caps;
	bool onoff;
#ifndef CONFIG_DSC_HDMI_VSIF
	struct sii_info_frame vsif[4];
#else
	struct sii_info_frame vsif[1];
#endif
	struct sii_info_frame avi_if;
};

struct sizes_pix {
	/* number of horizontal pixels*/
	uint16_t h_pixels;
	/* number of vertical pixels*/
	uint16_t v_pixels;
};

struct compressvideo_mode {
	const char *_label_str;
	/* in hz*/
	uint32_t pixel_frq;
	/* CEA VIC for 4:3 picture aspect rate, 0 if not available*/
	uint8_t vic4x3;
	/* CEA VIC for 16:9 picture aspect rate, 0 if not available*/
	uint8_t vic16x9;
	/* VIC for 64:27 picture aspect rate, 0 if not avaliable */
	uint8_t vic_64x27;
	/* VIC for 256:135 picture aspect rate,  0 if not avaliable */
	uint8_t vic_256x135;
	/* HDMI VIC for 16:9 picture aspect rate, 0 if not available*/
	uint8_t hdmi_vic;
	/* number of active pixels per frame*/
	struct sizes_pix active;
	/* total number of pixels per frame*/
	struct sizes_pix total;
	/*sizes_pix_t	  blank; // blankingof sync pulse per frame*/
	/* offset of sync pulse per frame front_porch*/
	struct sizes_pix sync_offset;
	/* width of sync pulse per frame*/
	struct sizes_pix sync_width;
	/* true for interlaced video*/
	/*uint8_t interlaced;*/
	/* true on negative polarity for horizontal pulses*/
	uint8_t h_pol;
	/* true on negative polarity for vertical pulses*/
	uint8_t v_pol;
	/*Post Compression Rd Dly Cnt*/
	int8_t dly_cnt;
};


#ifdef CONFIG_FPGA_PLATFORM
static  struct compressvideo_mode s_video_mode_table2x[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "1080P25",	37125000, 0, 33, 0, 0, 0, {960, 1080},
		{1320, 1125}, {264 , 4}, {22, 5}, POS, POS, 0},

	{ "2160P60",	37125000, 0, 97, 107, 0, 0, {1920, 2160},
		{2200, 2250}, {88 , 8}, {44, 10}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

static  struct compressvideo_mode s_video_mode_table3x[] = {
	/*label			fpix	  VIC, HVIC, {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/

	{ "1080P25",	24750000, 0, 33, 0, 0, 0, {640, 1080},
		{880, 1125}, {176 , 4}, {14, 5}, POS, POS, 0},

	{ "2160P50",	198000000, 0, 96, 106, 0, 0, {1280, 2160},
		{1760, 2250}, {352 , 8}, {30, 10}, POS, POS, 0 },

	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

static  struct compressvideo_mode s_video_mode_table2x_420[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "2160P24",	742500000, 0, 93, 103, 0, 3, {1920, 1080},
		{2750, 1125}, {638 , 4}, {44, 5}, POS, POS, 0},
	{ "2160P50",	148500000, 0, 96, 106, 0, 0, {1920, 1080},
		{2200, 1125}, {88 , 4}, {44, 5}, POS, POS, 0 },
	{ "2160P60",	148500000, 0, 97, 107, 0, 0, {1920, 1080},
		{2200, 1125}, {88, 4}, {44, 5}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

static  struct compressvideo_mode s_video_mode_table3x_420[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "2160P24",	495000000, 0, 93, 103, 0, 3, {1280, 1080},
		{2750, 2250}, {638 , 8}, {44, 10}, POS, POS, 0},
	{ "2160P50",	99000000, 0, 96, 106, 0, 0, {1280, 1080},
		{2640, 2250}, {528 , 8}, {44, 10}, POS, POS, 0 },
	{ "2160P60",	99000000, 0, 97, 107, 0, 0, {1280, 1080},
		{1760, 1125}, {352 , 4}, {30, 5}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};



#else

static  struct compressvideo_mode s_video_mode_table2x[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "1080P50",	74250000, 0, 31, 0, 0, 0, {960, 1080},
		{1320, 1125}, {264, 4}, {22, 5}, POS, POS, 0},
	{ "1080P60",	77220000, 0, 16, 0, 0, 0, {960, 1080},
		{1144, 1125}, {90, 4}, {22, 5}, POS, POS, 39},

	{ "2160P30",	148500000, 0, 95, 105, 0, 1, {1920, 2160},
		{2200, 2250}, {90, 8}, {44 , 10}, POS, POS, 0},
	{ "2160P25",	148500000, 0, 94, 104, 0,  2, {1920, 2160},
		{2640, 2250}, {528, 8}, {44, 10}, POS, POS, 0},
	{ "2160P24",	148500000, 0, 93, 103, 0, 3, {1920, 2160},
		{2750, 2250}, {638, 8}, {44, 10}, POS, POS, 0},
	{ "2160P50",	297000000, 0, 96, 106, 101, 0, {1920, 2160},
		{2640, 2250}, {528, 8}, {44, 10}, POS, POS, 0 },
	{ "2160P60",	297000000, 0, 97, 107, 102, 0, {1920, 2160},
		{2200, 2250}, {90, 8}, {44, 10}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

static  struct compressvideo_mode s_video_mode_table3x[] = {
	/*label			fpix	  VIC, HVIC, {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "1080P50",	49500000, 0, 31, 0, 0, 0, {640, 1080},
		{880, 1125}, {176 , 4}, {14, 5}, POS, POS, 0},
	{ "1080P60",	54945000, 0, 16, 0, 0, 0, {640, 1080},
		{814, 1125}, {74 , 4}, {34, 5}, POS, POS, 71},

	{ "2160P30",	99495000, 0, 95, 105, 0, 1, {1280, 2160},
		{1474, 2250}, {65 , 8}, {30 , 10}, POS, POS, 7},
	{ "2160P25",	99000000, 0, 94, 104, 0, 2, {1280, 2160},
		{1760, 2250}, {352 , 8}, {30, 10}, POS, POS, 0},
	{ "2160P24",	89100000, 0, 93, 103, 0, 3, {1280, 2160},
		{1650, 2250}, {255 , 8}, {25, 10}, POS, POS, -128},
	{ "2160P50",	198000000, 0, 96, 106, 101, 0, {1280, 2160},
		{1760, 2250}, {352 , 8}, {30, 10}, POS, POS, 0 },
	{ "2160P60",	198990000, 0, 97, 107, 102, 0, {1280, 2160},
		{1474, 2250}, {65 , 8}, {30, 10}, POS, POS, 7 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};


static  struct compressvideo_mode s_video_mode_table2x_420[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "2160P30",	74250000, 0, 95, 105, 0, 1, {1920, 1080},
		{2200, 1125}, {88 , 4}, {44, 5}, POS, POS, 0},
	{ "2160P50",	148500000, 0, 96, 106, 101, 0, {1920, 1080},
		{2640, 1125}, {528 , 4}, {44, 5}, POS, POS, 0 },
	{ "2160P60",	148500000, 0, 97, 107, 102, 0, {1920, 1080},
		{2200, 1125}, {88, 4}, {44, 5}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

static  struct compressvideo_mode s_video_mode_table3x_420[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "2160P30",	49747500, 0, 95, 105, 0, 1, {1280, 1080},
		{1474, 1125}, {63 , 4}, {30, 5}, POS, POS, 0},
	{ "2160P50",	99000000, 0, 96, 106, 101, 0, {1280, 1080},
		{1760, 1125}, {352 , 4}, {30, 5}, POS, POS, 0 },
	{ "2160P60",	99495000, 0, 97, 107, 102, 0, {1280, 1080},
		{1474, 1125}, {63 , 4}, {30, 5}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

#endif

static  struct compressvideo_mode s_hbr_table2x[] = {
	/*label  fpix	  VIC, HVIC {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "1080P50",	74250000, 0, 31, 0, 0, 0, {960, 1080},
		{1320, 1125}, {264 , 4}, {22, 5}, POS, POS, 0},
	{ "1080P60",	83160000, 0, 16, 0, 0, 0, {960, 1080},
		{1232, 1125}, {176 , 4}, {22, 5}, POS, POS, 116},

	{ "2160P30",	148500000, 0, 95, 105, 0, 1, {1920, 2160},
		{2200, 2250}, {88 , 8}, {44 , 10}, POS, POS, 0},
	{ "2160P25",	148500000, 0, 94, 104, 0,  2, {1920, 2160},
		{2640, 2250}, {528 , 8}, {44, 10}, POS, POS, 0},
	{ "2160P24",	148500000, 0, 93, 103, 0, 3, {1920, 2160},
		{2750, 2250}, {638 , 8}, {44, 10}, POS, POS, 0},
	{ "2160P50",	297000000, 0, 96, 106, 101, 0, {1920, 2160},
		{2640, 2250}, {528 , 8}, {44, 10}, POS, POS, 0 },
	{ "2160P60",	297000000, 0, 97, 107, 102, 0, {1920, 2160},
		{2200, 2250}, {88 , 8}, {44, 10}, POS, POS, 0 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

static  struct compressvideo_mode s_hbr_table3x[] = {
	/*label			fpix	  VIC, HVIC, {h_pixels/v_pixels active},
	{h_pixels/v_pixels  total}, {_h_v_sync_o},{_h_v_sync_w}, h_pol, v_pol*/
	{ "1080P50",	52875000, 0, 31, 0, 0, 0, {640, 1080},
		{940, 1125}, {236 , 4}, {14, 5}, POS, POS, 44},
	{ "1080P60",	62370000, 0, 16, 0, 0, 0, {640, 1080},
		{924, 1125}, {220 , 4}, {34, 5}, POS, POS, /*167*/127},

	{ "2160P30",	105435000, 0, 95, 105, 0, 1, {1280, 2160},
		{1562, 2250}, {154 , 8}, {30 , 10}, POS, POS, 84},
	{ "2160P25",	99000000, 0, 94, 104, 0, 2, {1280, 2160},
		{1760, 2250}, {352 , 8}, {30, 10}, POS, POS, 0},
	{ "2160P24",	89100000, 0, 93, 103, 0, 3, {1280, 2160},
		{1650, 2250}, {255 , 8}, {25, 10}, POS, POS, -128},
	{ "2160P50",	198000000, 0, 96, 106, 101, 0, {1280, 2160},
		{1760, 2250}, {352 , 8}, {30, 10}, POS, POS, 0 },
	{ "2160P60",	198990000, 0, 97, 107, 102, 0, {1280, 2160},
		{1474, 2250}, {65 , 8}, {30, 10}, POS, POS, 7 },
	/* end of table mark*/
	{("EndofTable"), 0, 0, 0, 0, 0, 0, {0, 0},
		{0, 0}, {0, 0}, {0, 0}, 0, 0, 0},
};

#ifndef CONFIG_DSC_HDMI_VSIF
/*dsc infoframe*/
static uint8_t frame_dsc1[SII_INFOFRAME_MAX_LEN] = {
    /*id, ver, length*/
    0x81, 0x01, 0x1b,
    /*SP0, pps0 start from PB6*/
    0x00, 0x03, 0x0c, 0x00, 0x00, 0x91, 0x11,
    /*SP1*/
    0x00, 0x00, 0x89, 0x10, 0x80, 0x04, 0x38,
    /*SP2*/
    0x07, 0x80, 0x00, 0x08, 0x07, 0x80, 0x07,
    /*SP3	*/
    0x80, 0x02, 0x00, 0x04, 0xc0, 0x00, 0x20,
};

static uint8_t frame_dsc2[SII_INFOFRAME_MAX_LEN] = {0x81, 0x01, 0x1b,
    /*SP0, pps22 start from PB6*/
    0x00, 0x03, 0x0c, 0x00, 0x00, 0x92, 0x01, 0x1e, 0x00, 0x1a, 0x00, 0x0c, 0x0d,
	0xb7, 0x03, 0x94, 0x18, 0x00, 0x10, 0xf0, 0x03, 0x0c, 0x20, 0x00, 0x06,
	0x0b, 0x0b, 0x33};

static uint8_t frame_dsc3[SII_INFOFRAME_MAX_LEN] = {0x81, 0x01, 0x1b,
    /*SP0, pps44 start from PB6*/
    0x00, 0x03, 0x0c, 0x00, 0x00, 0x93, 0x0e, 0x1c, 0x2a, 0x38, 0x46, 0x54, 0x62,
	0x69, 0x70, 0x77, 0x79, 0x7b, 0x7d, 0x7e, 0x01, 0x02, 0x01, 0x00, 0x09,
	0x40, 0x09, 0xbe
};

static uint8_t frame_dsc4[SII_INFOFRAME_MAX_LEN] = {0x81, 0x01, 0x1b,
    /*SP0, pps66 start from PB6*/
    0x00, 0x03, 0x0c, 0x00, 0x00, 0x9c, 0x19, 0xfc, 0x19, 0xfa, 0x19, 0xf8, 0x1a,
	0x38, 0x1a, 0x78, 0x1a, 0xb6, 0x2a, 0xf6, 0x2b, 0x34, 0x2b, 0x74, 0x3b,
	0x74, 0x6b, 0xf4
};
#else
/*HDMI VSIF*/
static uint8_t frame_dsc1[SII_INFOFRAME_MAX_LEN] = {
/*id, ver, length*/
0x81, 0x01, 0x08,
/*chksm, 24 IEEE id*/
0x00, 0x03, 0x0c, 0x00,
/*no need to change*/
0x00, 0x00, 0x00,
/*Pb7:[for VIC information]*/
0x00,
/*Pb8:[compression ratio]*/
0x00
};

#endif

static int s_dscconfig_wrap_registers(struct dsc_obj *p_obj,
					struct sii_drv_dsc_caps *wrap_caps);
static void s_dscconfig_pixel_frequency(
	struct dsc_obj *p_obj, struct sii_drv_dsc_caps *pixel_frequency);
static void s_set_dsc_en(struct dsc_obj *p_obj, bool onoff);
static void s_dscconfigureinfoframe(struct dsc_obj *p_obj,
					struct sii_drv_vsif_caps *vsif_caps);

static struct dsc_obj sg_stdsc_obj;
/* public functions */
void *sii_mod_tx_dsc_init(pstSii9630_Device_t pstDevice,
	void (*event_notify_fn)(void *, uint32_t))
{
	struct dsc_obj *p_obj = &sg_stdsc_obj;//kzalloc(sizeof(struct dsc_obj), GFP_KERNEL);

    memset(&sg_stdsc_obj,0,sizeof(struct dsc_obj));
	if (!p_obj)
		return NULL;
    //sii_log_debug("Dsc Tx Init start\n");

    p_obj->pstDevice = pstDevice;

    //sii_log_debug("Dsc Tx Init End\n");
	return p_obj;
}

#if 0
void sii_mod_dsc_delete(void *dsc_obj)
{
	struct dsc_obj *p_obj = (struct dsc_obj *)dsc_obj;

	//kfree(p_obj);
}
#endif

int sii_mod_dsc_get(void *pdsc_obj, enum sii_mod_dsc_op_code op_code,
			void *out_data)
{
	struct dsc_obj *p_obj = (struct dsc_obj *)pdsc_obj;
	int16_t status = 0;

	if (p_obj) {
		switch (op_code) {
		case SII_MOD_DSC_OPCODE__INFOFRAME_DATA:
		{
			/*request must be made with info frame id filled in*/
			struct sii_info_frame *p_if_data =
				(struct sii_info_frame *)out_data;
			memcpy(out_data,
				&p_obj->vsif[p_if_data->if_id -
				SII_INFO_FRAME_ID__VSIF1],
				sizeof(struct sii_info_frame));
			break;
		}
		case SII_MOD_DSC_OPCODE__INFOFRAME_AVIF:
			memcpy(out_data, &p_obj->avi_if,
				sizeof(struct sii_info_frame));
			break;
		case SII_MOD_DSC_OPCODE__COMPRESSED_PIXEL_CLOCK:
		{
			memcpy(out_data, &p_obj->wrap_caps.pixel_freq,
				sizeof(uint32_t));
			break;
		}
		case SII_MOD_DSC_OPCODE__COMPRESSED_STATUS:
		{
			memcpy(out_data, &p_obj->onoff,
				sizeof(bool));
			break;
		}

		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

int sii_mod_dsc_set(void *pdsc_obj, enum sii_mod_dsc_op_code op_code,
			void *in_data)
{
	struct dsc_obj *p_obj = (struct dsc_obj *)pdsc_obj;
	int16_t status = 0;

	if (p_obj) {
		switch (op_code) {
		case SII_MOD_DSC_OPCODE__WRAP_CONFIGURE:
		{
			struct sii_drv_dsc_caps *dsc_capability =
				(struct sii_drv_dsc_caps *)in_data;

			status = s_dscconfig_wrap_registers(p_obj,
				dsc_capability);

			if (status)
				return status;

			break;
		}
		case SII_MOD_DSC_OPCODE__SET_DSC_EN:
		{
			bool enable_dsc = *(bool *)in_data;
			s_set_dsc_en(p_obj, enable_dsc);
			break;
		}
		case SII_MOD_DSC_OPCODE__INFOFRAME_UPDATE_VSIF:
		{
			struct sii_drv_vsif_caps *vsif_capability =
				(struct sii_drv_vsif_caps *)in_data;
			s_dscconfigureinfoframe(p_obj, vsif_capability);

			break;
		}
		case SII_MOD_DSC_OPCODE__INFOFRAME_AVIF:
			memcpy(&p_obj->avi_if, in_data,
				sizeof(struct sii_info_frame));
			break;
		case SII_MOD_DSC_OPCODE__UPDATE_PIXEL_CLOCK:
		{
			struct sii_drv_dsc_caps *dsc_capability =
				(struct sii_drv_dsc_caps *)in_data;

			s_dscconfig_pixel_frequency(p_obj, dsc_capability);
			break;
		}
		default:
			status = 1;
		}
		return status;
	}
	return 1;
}

int sii_mod_dsc_stop_timers(void *p_obj)
{
	struct dsc_obj *p_dsc_obj = (struct dsc_obj *)p_obj;
	/* stop all timers */
	p_dsc_obj = p_dsc_obj;
	return 0;
}

static void s_set_dsc_en(struct dsc_obj *p_obj, bool onoff)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	p_obj->onoff = onoff;

	if (p_obj->onoff) {
		sii_platform_set_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_ENC_EN);

		sii_platform_set_bit8(pstDev, 0x004, 0x40);
		msleep(25);
		sii_platform_clr_bit8(pstDev, 0x004, 0x40);
	}
	else {
		sii_platform_clr_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_ENC_EN);
		sii_platform_set_bit8(pstDev, 0x004, 0x40);
		msleep(25);
		sii_platform_clr_bit8(pstDev, 0x004, 0x40);
	}
}

static void s_dscconfigureinfoframe(struct dsc_obj *p_obj,
	struct sii_drv_vsif_caps *vsif_caps)
{

#ifndef CONFIG_DSC_HDMI_VSIF
	uint8_t checksum = 0;
	uint8_t i = 0, j = 0;
	uint16_t sum = 0;
	uint8_t hactive_low, hactive_high;
	uint8_t vactive_low, vactive_high;
	enum sii_drv_bit_depth clrdepth;
	enum avi_color_space    color_space;
	uint8_t compression_ratio;
	uint8_t hbr_enable;

	clrdepth = p_obj->wrap_caps.colordepth;
	color_space = p_obj->wrap_caps.color_space;

	hactive_low = (vsif_caps->h_active & 0xFF);
	hactive_high = ((vsif_caps->h_active >> 8) & 0xff);

	vactive_low = (vsif_caps->v_active & 0xFF);
	vactive_high = ((vsif_caps->v_active >> 8) & 0xff);

	memcpy(p_obj->vsif[0].b, frame_dsc1, 31);
	memcpy(p_obj->vsif[1].b, frame_dsc2, 31);
	memcpy(p_obj->vsif[2].b, frame_dsc3, 31);
	memcpy(p_obj->vsif[3].b, frame_dsc4, 31);

	p_obj->vsif[0].if_id = SII_INFO_FRAME_ID__VSIF1;
	p_obj->vsif[1].if_id = SII_INFO_FRAME_ID__VSIF2;
	p_obj->vsif[2].if_id = SII_INFO_FRAME_ID__VSIF3;
	p_obj->vsif[3].if_id = SII_INFO_FRAME_ID__VSIF4;

	p_obj->vsif[0].b[15] = vactive_high;
	p_obj->vsif[0].b[16] = vactive_low;
	p_obj->vsif[0].b[17] = hactive_high;
	p_obj->vsif[0].b[18] = hactive_low;

	compression_ratio = p_obj->wrap_caps.compression;
	hbr_enable = p_obj->wrap_caps.hbr_status;

	for (i = 0; i < 4; i++) {
		if (compression_ratio == COMPRESSION_2X)
			p_obj->vsif[i].b[7] = (0x1 << 1);
		else if (compression_ratio == COMPRESSION_3X)
			p_obj->vsif[i].b[7] = (0x1 << 2);
		if (hbr_enable)
			p_obj->vsif[i].b[7] |= (0x1 << 0);
	}

	if (compression_ratio == COMPRESSION_2X) {
		p_obj->vsif[0].b[14] = 0xC0;

		p_obj->vsif[0].b[23] = 0x0b;
		/*chunk size high*/
		p_obj->vsif[0].b[24] = 0x40;
		/*xmit_delay */
		p_obj->vsif[0].b[25] = 0x1;
		/*xmit_delay */
		p_obj->vsif[0].b[26] = 0x55;
		p_obj->vsif[0].b[27] = 0x5;
		p_obj->vsif[0].b[28] = 0x20;
		p_obj->vsif[0].b[29] = 0x0;
		p_obj->vsif[0].b[30] = 0x0a;

		p_obj->vsif[1].b[9] = 0x0;
		p_obj->vsif[1].b[10] = 0xa0;
		p_obj->vsif[1].b[11] = 0x01;
		p_obj->vsif[1].b[12] = 0x40;
		p_obj->vsif[1].b[13] = 0x00;
		p_obj->vsif[1].b[14] = 0xf;
		p_obj->vsif[1].b[15] = 0x11;
		p_obj->vsif[1].b[16] = 0x25;
		p_obj->vsif[1].b[17] = 0x09;
		p_obj->vsif[1].b[18] = 0xfa;
		p_obj->vsif[1].b[19] = 0x08;
		p_obj->vsif[1].b[22] = 0xf4;

		if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT) {
			p_obj->vsif[1].b[23] = 0x07;
			p_obj->vsif[1].b[24] = 0x10;
			p_obj->vsif[1].b[28] = 0x0F;
			p_obj->vsif[1].b[29] = 0x0F;

			p_obj->vsif[2].b[25] = 0x11;
			p_obj->vsif[2].b[26] = 0x40;
			p_obj->vsif[2].b[27] = 0x19;
			p_obj->vsif[2].b[28] = 0xC0;
			p_obj->vsif[2].b[29] = 0x22;
			p_obj->vsif[2].b[30] = 0x3E;

			p_obj->vsif[3].b[9] = 0x32;
			p_obj->vsif[3].b[10] = 0x7C;
			p_obj->vsif[3].b[11] = 0x3A;
			p_obj->vsif[3].b[12] = 0xBA;
			p_obj->vsif[3].b[13] = 0x3A;
			p_obj->vsif[3].b[14] = 0xF8;
			p_obj->vsif[3].b[15] = 0x3B;
			p_obj->vsif[3].b[16] = 0x38;
			p_obj->vsif[3].b[17] = 0x3B;
			p_obj->vsif[3].b[18] = 0x78;
			p_obj->vsif[3].b[19] = 0x3B;
			p_obj->vsif[3].b[20] = 0xB6;
			p_obj->vsif[3].b[21] = 0x4B;
			p_obj->vsif[3].b[22] = 0xF6;
			p_obj->vsif[3].b[23] = 0x4C;
			p_obj->vsif[3].b[24] = 0x34;
			p_obj->vsif[3].b[25] = 0x4C;
			p_obj->vsif[3].b[26] = 0x74;
			p_obj->vsif[3].b[27] = 0x5C;
			p_obj->vsif[3].b[28] = 0x74;
			p_obj->vsif[3].b[29] = 0x8C;
			p_obj->vsif[3].b[30] = 0xF4;

		}

		p_obj->vsif[2].b[23] = 0x00;
		p_obj->vsif[2].b[24] = 0x82;
	}

	switch (clrdepth) {
	case SII_DRV_BIT_DEPTH__8_BIT:
		p_obj->vsif[0].b[12] = ((0x8 << 4) |
			(p_obj->vsif[0].b[12] & 0xF));
		break;
	case SII_DRV_BIT_DEPTH__10_BIT:
		p_obj->vsif[0].b[12] = ((0xA << 4) |
			(p_obj->vsif[0].b[12] & 0xF));
		break;
	case SII_DRV_BIT_DEPTH__12_BIT:
		p_obj->vsif[0].b[12] = ((0xC << 4) |
			(p_obj->vsif[0].b[12] & 0xF));
		break;
	default:
		break;
	}

	if ((color_space == ACS__YCBCR420) ||
		(color_space == ACS__YCBCR444))
		p_obj->vsif[0].b[13] = ((0x1 << 4) & 0x0F);
	else
		p_obj->vsif[0].b[13] = (0x1 << 4);



	for (j = 0; j < 4; j++) {
		/*calculate checksum*/
		i = 0;
		sum = 0;
		checksum = 0;
		while (i < SII_INFOFRAME_MAX_LEN) {
			sum += p_obj->vsif[j].b[i];
			i = i + 1;
		}
		checksum = (((~sum) + 1) & 0xFF);
		p_obj->vsif[j].b[3] = checksum;
	}
#else
	uint8_t checksum = 0;
	uint8_t i = 0;
	uint16_t sum = 0;
	uint8_t compression_ratio;
	uint8_t vic_id;


	compression_ratio = p_obj->wrap_caps.compression;
	vic_id = p_obj->wrap_caps.vic_id.cea_vic;

	memcpy(p_obj->vsif[0].b, frame_dsc1, 12);
	p_obj->vsif[0].if_id = SII_INFO_FRAME_ID__VSIF1;
	p_obj->vsif[0].b[11] = compression_ratio;
	p_obj->vsif[0].b[10] = vic_id;

	/*calculate checksum*/
	i = 0;
	while (i < SII_INFOFRAME_MAX_LEN) {
		sum += p_obj->vsif[0].b[i];
		i = i + 1;
	}
	checksum = (((~sum) + 1) & 0xFF);
	p_obj->vsif[0].b[3] = checksum;
#endif

}

static int s_dscconfig_wrap_registers(struct dsc_obj *p_obj,
					struct sii_drv_dsc_caps *wrap_caps)
{
    pstSii9630_Device_t pstDev = p_obj->pstDevice;
	/*uint8_t pps_buf[88];
	uint8_t j = 0;*/
//	uint8_t vic_id = 0;
	uint8_t compression = 0;
	uint32_t h_v_damt = 0;
	uint16_t htotal = 0, vtotal = 0, hblank = 0, hactive = 0, hfront = 0,
		hwidth = 0, hbackporch = 0;
	uint16_t vactive = 0, vbackporch = 0, vblank = 0, vfront = 0,
		vwidth = 0;
	uint16_t hactive_uncompressed = 0;
	uint16_t hblank_uncompressed = 0;
	uint8_t htotal_low = 0, vtotal_low = 0;
	uint8_t htotal_high = 0, vtotal_high = 0;
	uint8_t hfront_low = 0, vfront_low = 0;
	uint8_t hfront_high = 0, vfront_high = 0;
	uint8_t hback_low = 0, vback_low = 0;
	uint8_t hback_high = 0, vback_high = 0;
	uint8_t hwidth_low = 0, vheight_low = 0;
	uint8_t hwidth_high = 0, vheight_high = 0;
	uint8_t vactive_low = 0, vactive_high = 0,
		hactive_uncompressed_high = 0;
	uint8_t hactive_low = 0, hactive_high = 0, hactive_uncompressed_low = 0;
	uint16_t add_xmit_delay = 0;
	uint8_t add_xmit_delay_low = 0;
	uint8_t add_xmit_delay_high = 0;
	enum sii_drv_bit_depth clrdepth;
	int8_t post_dly_cnt = 0;
	uint8_t h_v_damt_low = 0;
	uint8_t h_v_damt_mid = 0;
	uint8_t h_v_damt_high = 0;
	uint8_t temp_value = 0;
	uint8_t vic_id_supported = 0;
	int status = 0;
	uint8_t hpolarity = 0, vpolarity = 0;
	uint8_t i = 0;
	struct compressvideo_mode *p_video_mode_table;
	uint8_t avi_chksum = p_obj->avi_if.b[3];
	uint8_t avi_data_byte4_pixel_cnt = p_obj->avi_if.b[7] +
			p_obj->avi_if.b[15] + p_obj->avi_if.b[16];
	enum avi_color_space    color_space;

	p_obj->wrap_caps = *wrap_caps;

//	vic_id = p_obj->wrap_caps.vic_id.hdmi_vic ?
//			p_obj->wrap_caps.vic_id.hdmi_vic :
//			p_obj->wrap_caps.vic_id.cea_vic;
	compression = p_obj->wrap_caps.compression;
	color_space = p_obj->wrap_caps.color_space;
	clrdepth = p_obj->wrap_caps.colordepth;

	switch (clrdepth) {
	case SII_DRV_BIT_DEPTH__8_BIT:
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__DSC_ENC_IIF_CLR_DEPTH, 0x04);
		break;
	case SII_DRV_BIT_DEPTH__10_BIT:
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__DSC_ENC_IIF_CLR_DEPTH, 0x05);
		break;
	case SII_DRV_BIT_DEPTH__12_BIT:
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__DSC_ENC_IIF_CLR_DEPTH, 0x06);
		break;
	default:
		break;
	}

	hactive_uncompressed = p_obj->wrap_caps.h_active;
	hblank_uncompressed = p_obj->wrap_caps.h_blank;

	hactive_uncompressed_low = (hactive_uncompressed & 0xFF);
	hactive_uncompressed_high = ((hactive_uncompressed & ~0xff) >> 8);

	if (!p_obj->wrap_caps.hbr_status) {
		if (compression == COMPRESSION_2X) {
			if ((color_space == ACS__RGB) ||
				(color_space == ACS__YCBCR444))
				p_video_mode_table = &s_video_mode_table2x[0];
			else
				p_video_mode_table =
				&s_video_mode_table2x_420[0];
		} else {
			if ((color_space == ACS__RGB) ||
				(color_space == ACS__YCBCR444))
				p_video_mode_table = &s_video_mode_table3x[0];
			else
				p_video_mode_table =
				&s_video_mode_table3x_420[0];
		}
	} else {
		if (compression == COMPRESSION_2X)
			p_video_mode_table = &s_hbr_table2x[0];
		else
			p_video_mode_table = &s_hbr_table3x[0];

	}

	if ((p_obj->wrap_caps.vic_id.hdmi_vic)) {
		for (i = 0; i < 7; i++) {
			if ((p_obj->wrap_caps.vic_id.hdmi_vic ==
				p_video_mode_table->hdmi_vic)) {
				/* TODO: CEA VIC corresponding
				to HDMI VIC is updated here in
				AVI Infoframe. What if DS is not
				confirmed to CEA861-F? In this case,
				we need to update HDMI VIC in VSIF */
				p_obj->avi_if.b[7] =
					p_video_mode_table->vic16x9;
				p_obj->avi_if.b[15] =
					(p_video_mode_table->active.h_pixels
					& 0xFF);
				p_obj->avi_if.b[16] =
					((p_video_mode_table->active.h_pixels &
						~0xff) >> 8);
				hpolarity = p_video_mode_table->h_pol;
				vpolarity = p_video_mode_table->v_pol;

				vtotal = p_video_mode_table->total.v_pixels;
				htotal = p_video_mode_table->total.h_pixels;

				htotal_low =
					(p_video_mode_table->total.h_pixels
					& 0xFF);
				htotal_high = ((p_video_mode_table->total.h_pixels & ~0xff) >> 8);

				hfront = p_video_mode_table->sync_offset.h_pixels;
				hfront_low =
					(p_video_mode_table->sync_offset.h_pixels & 0xFF);
				hfront_high = ((p_video_mode_table->sync_offset.h_pixels & ~0xff) >> 8);

				hwidth = p_video_mode_table->sync_width.h_pixels;
				hwidth_low = (p_video_mode_table->sync_width.h_pixels & 0xFF);
				hwidth_high =
					((p_video_mode_table->sync_width.h_pixels & ~0xff) >> 8);

				hactive = p_video_mode_table->active.h_pixels;
				hblank = htotal - hactive;
				hbackporch = hblank - hfront - hwidth;

				hback_low = (hbackporch & 0xFF);
				hback_high = ((hbackporch & ~0xff) >> 8);

				vtotal_low = (p_video_mode_table->total.v_pixels & 0xFF);
				vtotal_high = ((p_video_mode_table->total.v_pixels & ~0xff) >> 8);

				vfront = p_video_mode_table->sync_offset.v_pixels;
				vfront_low = (p_video_mode_table->sync_offset.v_pixels & 0xFF);
				vfront_high = ((p_video_mode_table->sync_offset.v_pixels & ~0xff) >> 8);

				vwidth = p_video_mode_table->sync_width.v_pixels;
				vheight_low = (p_video_mode_table->sync_width.v_pixels & 0xFF);
				vheight_high = ((p_video_mode_table->sync_width.v_pixels & ~0xff) >> 8);

				vactive = p_video_mode_table->active.v_pixels;
				vblank = vtotal - vactive;
				vbackporch = vblank - vfront - vwidth;

				vback_low = (vbackporch & 0xFF);
				vback_high = ((vbackporch & ~0xff) >> 8);

				vactive_low = (p_video_mode_table->active.v_pixels & 0xFF);
				vactive_high = ((p_video_mode_table->active.v_pixels & ~0xff) >> 8);

				hactive_low = (p_video_mode_table->active.h_pixels & 0xFF);
				hactive_high = ((p_video_mode_table->active.h_pixels & ~0xff) >> 8);

				h_v_damt = ((vtotal * htotal) - 1);

				h_v_damt_low = h_v_damt & 0x0000ff;
				h_v_damt_mid = ((h_v_damt & 0xff00) >> 8);
				h_v_damt_high =
					(uint8_t)((h_v_damt & 0xff0000) >> 16);
				post_dly_cnt = p_video_mode_table->dly_cnt;

				vic_id_supported = 1;
				break;
			} else {
				p_video_mode_table++;
			}
		}
	}
    if( vic_id_supported == 0 ) {

		for (i = 0; i < 7; i++) {
			if ((p_obj->wrap_caps.vic_id.cea_vic ==
				p_video_mode_table->vic4x3) ||
				(p_obj->wrap_caps.vic_id.cea_vic ==
				p_video_mode_table->vic16x9) ||
				(p_obj->wrap_caps.vic_id.cea_vic ==
				p_video_mode_table->vic_64x27) ||
				(p_obj->wrap_caps.vic_id.cea_vic ==
				p_video_mode_table->vic_256x135)) {
				p_obj->avi_if.b[7] =
					p_obj->wrap_caps.vic_id.cea_vic;
				p_obj->avi_if.b[15] = (p_video_mode_table->active.h_pixels & 0xFF);
				p_obj->avi_if.b[16] = ((p_video_mode_table->active.h_pixels & ~0xff) >> 8);
				hpolarity = p_video_mode_table->h_pol;
				vpolarity = p_video_mode_table->v_pol;

				vtotal = p_video_mode_table->total.v_pixels;
				htotal = p_video_mode_table->total.h_pixels;

				htotal_low = (p_video_mode_table->total.h_pixels & 0xFF);
				htotal_high = ((p_video_mode_table->total.h_pixels & ~0xff) >> 8);

				hfront = p_video_mode_table->sync_offset.h_pixels;
				hfront_low =
					(p_video_mode_table->sync_offset.h_pixels & 0xFF);
				hfront_high =
					((p_video_mode_table->sync_offset.h_pixels & ~0xff) >> 8);

				hwidth = p_video_mode_table->sync_width.h_pixels;
				hwidth_low = (p_video_mode_table->sync_width.h_pixels & 0xFF);
				hwidth_high =
					((p_video_mode_table->sync_width.h_pixels & ~0xff) >> 8);

				hactive = p_video_mode_table->active.h_pixels;
				hblank = htotal - hactive;
				hbackporch = hblank - hfront - hwidth;

				hback_low = (hbackporch & 0xFF);
				hback_high = ((hbackporch & ~0xff) >> 8);

				vtotal_low =
					(p_video_mode_table->total.v_pixels & 0xFF);
				vtotal_high = ((p_video_mode_table->total.v_pixels & ~0xff) >> 8);

				vfront = p_video_mode_table->sync_offset.v_pixels;
				vfront_low =
					(p_video_mode_table->sync_offset.v_pixels & 0xFF);
				vfront_high =
					((p_video_mode_table->sync_offset.v_pixels & ~0xff) >> 8);

				vwidth = p_video_mode_table->sync_width.v_pixels;
				vheight_low =
					(p_video_mode_table->sync_width.v_pixels & 0xFF);
				vheight_high =
					((p_video_mode_table->sync_width.v_pixels & ~0xff) >> 8);

				vactive = p_video_mode_table->active.v_pixels;
				vblank = vtotal - vactive;
				vbackporch = vblank - vfront - vwidth;

				vback_low = (vbackporch & 0xFF);
				vback_high = ((vbackporch & ~0xff) >> 8);

				vactive_low = (p_video_mode_table->active.v_pixels & 0xFF);
				vactive_high = ((p_video_mode_table->active.v_pixels & ~0xff) >> 8);

				hactive_low = (p_video_mode_table->active.h_pixels & 0xFF);
				hactive_high = ((p_video_mode_table->active.h_pixels & ~0xff) >> 8);

				h_v_damt = ((vtotal * htotal) - 1);

				h_v_damt_low = h_v_damt & 0x0000ff;
				h_v_damt_mid = ((h_v_damt & 0xff00) >> 8);
				h_v_damt_high =
					(uint8_t)((h_v_damt & 0xff0000) >> 16);
				post_dly_cnt = p_video_mode_table->dly_cnt;

				vic_id_supported = 1;
				break;
			} else {
				p_video_mode_table++;
			}
		}
	}

	if (vic_id_supported == 0) {
		sii_log_debug("vic id not supported for compression\n");
		return 1;
	}

	{
		avi_chksum += avi_data_byte4_pixel_cnt - (p_obj->avi_if.b[7] +
				p_obj->avi_if.b[15] + p_obj->avi_if.b[16]);
		p_obj->avi_if.b[3] = avi_chksum;
	}

#ifdef CONFIG_UNDEFINED_MACRO
	/*DSC soft reset*/
	sii_platform_set_bit8(pstDev, REG_ADDR__SW_RST_REG0,
			BIT_MSK__SW_RST_REG0__REG_DSC_ENC_SW_RST);
	/*DSC soft reset*/
	sii_platform_clr_bit8(pstDev, REG_ADDR__SW_RST_REG0,
			BIT_MSK__SW_RST_REG0__REG_DSC_ENC_SW_RST);
#endif
	/*for DSC core to enable this is required */
	/*sii_platform_set_bit8(pstDev, REG_ADDR__PD_REG1, 0x02);*/

	/*420 functionality enabled or not in DSC*/
	if (color_space == ACS__YCBCR420) {
		sii_platform_set_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_420_MODE_EN);
	} else {
		sii_platform_clr_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_420_MODE_EN);
	}

	if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT) {
			sii_platform_clr_bit8(pstDev, REG_ADDR__BANNER_CONFIG5,
				BIT_MSK__BANNER_CONFIG5__REG_DC_BYPASS);
			sii_platform_set_bit8(pstDev, REG_ADDR__PKT_FILTER_0,
				BIT_MSK__PKT_FILTER_0__REG_DROP_GCP_PKT);
	}

	/*hsync polarity*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_CTRL, hpolarity);

	/* htotal low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HTOTAL_1, htotal_low);
	/* htotal high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HTOTAL_2, htotal_high);
	/* hwidth low  //how to get*/

	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HWIDTH_1, hwidth_low);
	/* hwidth high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HWIDTH_2, hwidth_high);
	/* hfront low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HS_FRNT_1, hfront_low);
	/* hfront high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HS_FRNT_2, hfront_high);

	/* hback low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HS_BACK_1, hback_low);
	/* hback high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_HS_BACK_2, hback_high);

	/*vtotal low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VTOTAL_1, vtotal_low);
	/*vtotal high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VTOTAL_2, vtotal_high);
	/*vheight low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VHEIGHT_1, vheight_low);
	/*vheight high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VHEIGHT_2, vheight_high);

	/*vfront low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VS_FRNT_1, vfront_low);
	/*vfront high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VS_FRNT_2, vfront_high);

	/*vback low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VS_BACK_1, vback_low);
	/*vback high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_VS_BACK_2, vback_high);

	/*sii_platform_wr_reg8(pstDev,
	 REG_ADDR__DSC_ENC_IIF_CLR_DEPTH, clrdepth);*/
	/*DSC support 8 bit only */

	/*sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_ENC_IIF_CLR_DEPTH, 0x04);*/

	/* after these many data writes into output fifo, start reading it (post
	 compression)*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__PRE_COMP_HALF_MAX_PXL_1, 0x80);
	/*added in silicon*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__PRE_COMP_HALF_MAX_PXL_2, 0x07);
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__POST_COMP_RD_DLY_CNT, post_dly_cnt);

	/*(htot*vtot)-1 of compressed format, lsb*/
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__DSC_WRAP_VS_PERIOD_0, h_v_damt_low);
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__DSC_WRAP_VS_PERIOD_1, h_v_damt_mid);
	/*(htot*vtot)-1 of compressed format, MSB*/

	sii_platform_wr_reg8(pstDev,
		REG_ADDR__DSC_WRAP_VS_PERIOD_2, h_v_damt_high);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_WRAP_THRESHOLD, 0x10);

#ifdef COMPRESSION_2X_420_10
	if ((color_space == ACS__YCBCR420)
		&& (clrdepth == SII_DRV_BIT_DEPTH__10_BIT)) {
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__DSC_WRAP_THRESHOLD, 0x0A);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__DSC_WRAP_EOF_OUT_CNT, 0x03);
		sii_platform_wr_reg8(pstDev,
			REG_ADDR__DSC_WRAP_EOF_AFTR_CYCLES, 0x01);
	}
#endif

	if (clrdepth == SII_DRV_BIT_DEPTH__8_BIT)
		sii_platform_put_bit8(pstDev,
			REG_ADDR__DSC_IIF_MEM1_RD_INIT_CTRL, 0x0F, 0x0F);
	else if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT)
		sii_platform_put_bit8(pstDev,
			REG_ADDR__DSC_IIF_MEM1_RD_INIT_CTRL, 0x0F, 0x0E);

	if ((color_space == ACS__YCBCR420)
		&& (clrdepth == SII_DRV_BIT_DEPTH__10_BIT)) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_WRAP_CTRL_0, 0xCC);
	} else {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_WRAP_CTRL_0, 0x8C);
	}

	/*configure DSC core logic:*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8192, 0x11);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8193, 0x00);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8194, 0x00);

	if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT)
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8195, 0xA9);
	else
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8195, 0x89);

	if ((color_space == ACS__YCBCR420) || (color_space == ACS__YCBCR444)) {

		sii_platform_clr_bit8(pstDev, REG_ADDR__DSC_PPS_8196,
		BIT_MSK__DSC_PPS_8196__PPS_CONVERT_RGB);
	} else {
		sii_platform_set_bit8(pstDev, REG_ADDR__DSC_PPS_8196,
		BIT_MSK__DSC_PPS_8196__PPS_CONVERT_RGB);
	}


	if (compression == COMPRESSION_3X)
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8197, 0x80);
	else
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8197, 0xC0);

	/*v_actice low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8198, vactive_high);
	/*v_actice high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8199, vactive_low);

	/*h_actice low  non compressed*/
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__DSC_PPS_8200, hactive_uncompressed_high);
	/*h_actice high non compressed*/
	sii_platform_wr_reg8(pstDev,
		REG_ADDR__DSC_PPS_8201, hactive_uncompressed_low);

	/*slice height low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8202, 0x0);
	/*slice height high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8203, 0x8);
	/*slice width low*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8204, 0x7);
	/*slice width high*/
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8205, 0x80);

	if (compression == COMPRESSION_3X) {
		/*chunk size low*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8206, 0x7);
		/*chunk size high*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8207, 0x80);
		/*xmit_delay */
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8208, 0x2);
		/*xmit_delay */
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8209, 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8210, 0x4);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8211, 0xc0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8212, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8213, 0x20);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8214, 0x1);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8215, 0x1e);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8216, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8217, 0x1a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8218, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8219, 0xc);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8220, 0xd);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8221, 0xb7);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8222, 0x3);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8223, 0x94);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8224, 0x18);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8227, 0xf0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8250, 0x1);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8251, 0x2);
	} else {
		/*chunk size low*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8206, 0x0b);
		/*chunk size high*/
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8207, 0x40);
		/*xmit_delay */
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8208, 0x1);
		/*xmit_delay */
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8209, 0x55);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8210, 0x5);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8211, 0x20);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8212, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8213, 0x0a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8214, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8215, 0xa0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8216, 0x01);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8217, 0x40);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8218, 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8219, 0xf);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8220, 0x11);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8221, 0x25);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8222, 0x09);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8223, 0xfa);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8224, 0x08);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8227, 0xf4);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8250, 0x00);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8251, 0x82);
	}

	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8225, 0x00);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8226, 0x10);

	if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8228, 0x07);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8229, 0x10);
	} else {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8228, 0x03);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8229, 0x0C);
	}

	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8230, 0x20);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8231, 0x0);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8232, 0x6);

	if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8233, 0x0F);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8234, 0x0F);
	} else {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8233, 0x0B);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8234, 0x0B);
	}

	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8235, 0x33);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8236, 0xe);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8237, 0x1c);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8238, 0x2a);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8239, 0x38);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8240, 0x46);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8241, 0x54);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8242, 0x62);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8243, 0x69);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8244, 0x70);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8245, 0x77);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8246, 0x79);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8247, 0x7b);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8248, 0x7d);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8249, 0x7e);

	if (clrdepth == SII_DRV_BIT_DEPTH__10_BIT) {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8252, 0x11);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8253, 0x40);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8254, 0x19);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8255, 0xc0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8256, 0x22);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8257, 0x3e);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8258, 0x32);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8259, 0x7c);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8260, 0x3a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8261, 0xba);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8262, 0x3a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8263, 0xf8);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8264, 0x3b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8265, 0x38);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8266, 0x3b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8267, 0x78);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8268, 0x3b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8269, 0xb6);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8270, 0x4b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8271, 0xf6);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8272, 0x4c);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8273, 0x34);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8274, 0x4c);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8275, 0x74);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8276, 0x5c);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8277, 0x74);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8278, 0x8c);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8279, 0xf4);
	} else {
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8252, 0x1);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8253, 0x0);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8254, 0x9);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8255, 0x40);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8256, 0x9);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8257, 0xbe);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8258, 0x19);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8259, 0xfc);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8260, 0x19);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8261, 0xfa);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8262, 0x19);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8263, 0xf8);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8264, 0x1a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8265, 0x38);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8266, 0x1a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8267, 0x78);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8268, 0x1a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8269, 0xb6);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8270, 0x2a);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8271, 0xf6);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8272, 0x2b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8273, 0x34);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8274, 0x2b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8275, 0x74);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8276, 0x3b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8277, 0x74);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8278, 0x6b);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_PPS_8279, 0xf4);
	}

	if ((p_obj->wrap_caps.vic_id.cea_vic == 16)
		|| (p_obj->wrap_caps.vic_id.cea_vic == 31)) {

		/* for 1080 P*/
		sii_platform_clr_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_ENC_SLICE_OPERATION);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_1, 0x1);
		/*add_xmit_delay = HORIZONTAL_BLANK/2 + 16'd800 1 slice(1080p)
		*/
		add_xmit_delay = hblank_uncompressed + 0x320;
		add_xmit_delay_high = (add_xmit_delay & 0xFF);
		add_xmit_delay_low = ((add_xmit_delay & ~0xff) >> 8);

		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_2,
			add_xmit_delay_low);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_3,
			add_xmit_delay_high);
	} else {
		/* for 4k2k P*/
		sii_platform_set_bit8(pstDev, REG_ADDR__FUNC_SEL,
			BIT_MSK__FUNC_SEL__REG_DSC_ENC_SLICE_OPERATION);
		/*add_xmit_delay = HORIZONTAL_BLANK/2 + 16'd1400 2 slices (4k)
		*/
		add_xmit_delay = (hblank_uncompressed)/2 + 0x578;
		add_xmit_delay_high = (add_xmit_delay & 0xFF);
		add_xmit_delay_low = ((add_xmit_delay & ~0xff) >> 8);

		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_1, 0x03);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_2,
			add_xmit_delay_low);
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_3,
			add_xmit_delay_high);
	}

	if (color_space == ACS__YCBCR420) {
		temp_value = sii_platform_rd_reg8(pstDev, REG_ADDR__DSC_CTRL_1);
		temp_value |= 0x10;
		sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_1, temp_value);
	}

/*for 420 8
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_2, 0x08);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_3, 0x48);
	bit 3x and 2x and 420 10 bit this value has used in Macro
	REG_ADDR__DSC_CTRL_1:Reason dont know!!!
	No description in register doc
	REG_ADDR__DSC_CTRL_2:REG_ADDR__DSC_CTRL_3
	As the input file is of reduced pixels so uncompressed
	hblank can not be taken from "video_mode_table"*/

	/*sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_1, 0x13);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_2, 0x06);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_CTRL_3, 0x90);*/

	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_SW_RST, 0x07);
	msleep(100);
	sii_platform_wr_reg8(pstDev, REG_ADDR__DSC_SW_RST, 0x00);


	/*sii_platform_block_read8(pstDev, REG_ADDR__DSC_PPS_8192, pps_buf, 88);

	for (j = 0; j < 90; j++) {
		sii_log_print("REG_ADDR__DSC_PPS_%02x 0x%02x \n", j, pps_buf[j]);
	}*/
    UNUSED(vpolarity);
    UNUSED(hactive_high);
    UNUSED(hactive_low);

	return status;
}

static void s_dscconfig_pixel_frequency(
	struct dsc_obj *p_obj, struct sii_drv_dsc_caps *pixel_frequency)
{
	uint8_t vic_id = 0;
	uint8_t compression = 0;
	uint32_t pixel_clock = 0;
	uint8_t i = 0;
	enum avi_color_space color_space;
	struct compressvideo_mode *p_video_mode_table;
	p_obj->wrap_caps = *pixel_frequency;

	vic_id =
		p_obj->wrap_caps.vic_id.hdmi_vic ?
			p_obj->wrap_caps.vic_id.hdmi_vic :
			p_obj->wrap_caps.vic_id.cea_vic;

	compression = p_obj->wrap_caps.compression;
	color_space = p_obj->wrap_caps.color_space;

	if (!p_obj->wrap_caps.hbr_status) {
		if (compression == COMPRESSION_2X) {
			if ((color_space == ACS__RGB) ||
				(color_space == ACS__YCBCR444))
				p_video_mode_table = &s_video_mode_table2x[0];
			else
				p_video_mode_table =
				&s_video_mode_table2x_420[0];
		} else {
			if ((color_space == ACS__RGB) ||
				(color_space == ACS__YCBCR444))
				p_video_mode_table = &s_video_mode_table3x[0];
			else
				p_video_mode_table =
				&s_video_mode_table3x_420[0];
		}
	} else {
		if (compression == COMPRESSION_2X)
			p_video_mode_table = &s_hbr_table2x[0];
		else
			p_video_mode_table = &s_hbr_table3x[0];

	}

/*
	if (!p_obj->wrap_caps.hbr_status) {
		if (compression == COMPRESSION_2X)
			p_video_mode_table = &s_video_mode_table2x[0];
		else
			p_video_mode_table = &s_video_mode_table3x[0];
	} else {
		if (compression == COMPRESSION_2X)
			p_video_mode_table = &s_hbr_table2x[0];
		else
			p_video_mode_table = &s_hbr_table3x[0];
	}
*/

	if ((p_obj->wrap_caps.vic_id.hdmi_vic)) {
		for (i = 0; i < 7; i++) {
			if ((p_obj->wrap_caps.vic_id.hdmi_vic ==
					p_video_mode_table->hdmi_vic)) {
				pixel_clock = p_video_mode_table->pixel_frq;
				break;
			} else {
				p_video_mode_table++;
			}
		}
	} else {
		for (i = 0; i < 7; i++) {
			if ((p_obj->wrap_caps.vic_id.cea_vic ==
					p_video_mode_table->vic4x3) ||
					(p_obj->wrap_caps.vic_id.cea_vic ==
					p_video_mode_table->vic16x9) ||
					(p_obj->wrap_caps.vic_id.cea_vic ==
					p_video_mode_table->vic_64x27) ||
					(p_obj->wrap_caps.vic_id.cea_vic ==
					p_video_mode_table->vic_256x135)) {

				pixel_clock = p_video_mode_table->pixel_frq;
				break;
			} else {
				p_video_mode_table++;
			}
		}
	}

	memcpy(&p_obj->wrap_caps.pixel_freq, &pixel_clock, sizeof(uint32_t));
    UNUSED(vic_id);
}

#endif  // #if (_ENABLE_CHIP_SIL9630 == _MACRO_ENABLE)
/** END of file *********************************************************/
