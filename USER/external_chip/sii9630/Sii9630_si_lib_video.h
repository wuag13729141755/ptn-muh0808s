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

#ifndef __SI_LIB_VIDEO_H__
#define __SI_LIB_VIDEO_H__



#define SII_LIB_VIDEO_MOD3D__NONE             0x0000
#define SII_LIB_VIDEO_MOD3D__FP               0x0001
#define SII_LIB_VIDEO_MOD3D__FA               0x0002
#define SII_LIB_VIDEO_MOD3D__TB               0x0004
#define	SII_LIB_VIDEO_MOD3D__SBSF             0x0008
#define SII_LIB_VIDEO_MOD3D__LA               0x0010
#define SII_LIB_VIDEO_MOD3D__LD               0x0020
#define SII_LIB_VIDEO_MOD3D__LDG              0x0040
#define	SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_OL_OR 0x0100
#define	SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_OL_ER 0x0200
#define	SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_EL_OR 0x0400
#define	SII_LIB_VIDEO_MOD3D__SBSH_HORIZ_EL_ER 0x0800
#define	SII_LIB_VIDEO_MOD3D__SBSH_QUINX_OL_OR 0x1000
#define	SII_LIB_VIDEO_MOD3D__SBSH_QUINX_OL_ER 0x2000
#define	SII_LIB_VIDEO_MOD3D__SBSH_QUINX_EL_OR 0x4000
#define	SII_LIB_VIDEO_MOD3D__SBSH_QUINX_EL_ER 0x8000

#define SII_LIB_VIDEO_ASPRAT__125             ((float)(1.25))
#define SII_LIB_VIDEO_ASPRAT__133             \
	((float)(4.0/3.0))
#define SII_LIB_VIDEO_ASPRAT__155             ((float)(1.55))\
	       /* 14.0/9.0 */
#define SII_LIB_VIDEO_ASPRAT__166             ((float)(1.66))\
	       /* 15.0/9.0 */
#define SII_LIB_VIDEO_ASPRAT__178             \
	((float)(16.0/9.0))
#define SII_LIB_VIDEO_ASPRAT__185             ((float)(1.85))
#define SII_LIB_VIDEO_ASPRAT__235             ((float)(2.35))

/**
* @brief video format (resolution)
*/
enum sii_lib_video_v_m {
	/*!<  0 = vm1_640x480p,*/
	SII_LIB_VIDEO_VM__VGA60           ,
	/*!<  1 = vm2_3_720x480p,*/
	SII_LIB_VIDEO_VM__480P            ,
	/*!<  2 = vm4_1280x720p,*/
	SII_LIB_VIDEO_VM__720P60          ,
	/*!<  3 = vm5_1920x1080i,*/
	SII_LIB_VIDEO_VM__1080I60         ,
	/*!<  4 = vm6_7_720_1440x480i,*/
	SII_LIB_VIDEO_VM__480I2           ,
	/*!<  5 = vm8_9_720_1440x240p_1,*/
	SII_LIB_VIDEO_VM__240P2A          ,
	/*!<  6 = vm8_9_720_1440x240p_2,*/
	SII_LIB_VIDEO_VM__240P2B          ,
	/*!<  7 = vm10_11_2880x480i,*/
	SII_LIB_VIDEO_VM__480I4           ,
	/*!<  8 = vm12_13_2880x240p_1,*/
	SII_LIB_VIDEO_VM__240P4A          ,
	/*!<  9 = vm12_13_2880x240p_2,*/
	SII_LIB_VIDEO_VM__240P4B          ,
	/*!< 10 = vm14_15_1440x480p,*/
	SII_LIB_VIDEO_VM__480P2           ,
	/*!< 11 = vm16_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P60         ,
	/*!< 12 = vm17_18_720x576p,*/
	SII_LIB_VIDEO_VM__576P            ,
	/*!< 13 = vm19_1280x720p,*/
	SII_LIB_VIDEO_VM__720P50          ,
	/*!< 14 = vm20_1920x1080i,*/
	SII_LIB_VIDEO_VM__1080I50         ,
	/*!< 15 = vm21_22_720_1440x576i,*/
	SII_LIB_VIDEO_VM__576I2           ,
	/*!< 16 = vm23_24_720_1440x288p_1,*/
	SII_LIB_VIDEO_VM__288P2A          ,
	/*!< 17 = vm23_24_720_1440x288p_2,*/
	SII_LIB_VIDEO_VM__288P2B          ,
	/*!< 18 = vm23_24_720_1440x288p_3,*/
	SII_LIB_VIDEO_VM__288P2C          ,
	/*!< 19 = vm25_26_2880x576i,*/
	SII_LIB_VIDEO_VM__576I4           ,
	/*!< 20 = vm27_28_2880x288p_1,*/
	SII_LIB_VIDEO_VM__288P4A          ,
	/*!< 21 = vm27_28_2880x288p_2,*/
	SII_LIB_VIDEO_VM__288P4B          ,
	/*!< 22 = vm27_28_2880x288p_3,*/
	SII_LIB_VIDEO_VM__288P4C          ,
	/*!< 23 = vm29_30_1440x576p,*/
	SII_LIB_VIDEO_VM__576P2           ,
	/*!< 24 = vm31_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P50         ,
	/*!< 25 = vm32_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P24         ,
	/*!< 26 = vm33_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P25         ,
	/*!< 27 = vm34_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P30         ,
	/*!< 28 = vm35_36_2880x480p,*/
	SII_LIB_VIDEO_VM__480P4           ,
	/*!< 29 = vm37_38_2880x576p,*/
	SII_LIB_VIDEO_VM__576P4           ,
	/*!< 30 = vm39_1920x1080i_1250_total,*/
	SII_LIB_VIDEO_VM__1080I50A        ,
	/*!< 31 = vm40_1920x1080i,*/
	SII_LIB_VIDEO_VM__1080I100        ,
	/*!< 32 = vm41_1280x720p,*/
	SII_LIB_VIDEO_VM__720P100         ,
	/*!< 33 = vm42_43_720x576p,*/
	SII_LIB_VIDEO_VM__576P100         ,
	/*!< 34 = vm44_45_720_1440x576i,*/
	SII_LIB_VIDEO_VM__576I100         ,
	/*!< 35 = vm46_1920x1080i,*/
	SII_LIB_VIDEO_VM__1080I120        ,
	/*!< 36 = vm47_1280x720p,*/
	SII_LIB_VIDEO_VM__720P120         ,
	/*!< 37 = vm48_49_720x480p,*/
	SII_LIB_VIDEO_VM__480P120         ,
	/*!< 38 = vm50_51_720_1440x480i,*/
	SII_LIB_VIDEO_VM__480I120         ,
	/*!< 39 = vm52_53_720x576p,*/
	SII_LIB_VIDEO_VM__576P200         ,
	/*!< 40 = vm54_55_720_1440x576i,*/
	SII_LIB_VIDEO_VM__576I200         ,
	/*!< 41 = vm56_57_720x480p,*/
	SII_LIB_VIDEO_VM__480P240         ,
	/*!< 42 = vm58_59_720_1440x480i,*/
	SII_LIB_VIDEO_VM__480I240         ,
	/*!< 43 = vm60_1280x720p,*/
	SII_LIB_VIDEO_VM__720P24          ,
	/*!< 44 = vm61_1280x720p,*/
	SII_LIB_VIDEO_VM__720P25          ,
	/*!< 45 = vm62_1280x720p,*/
	SII_LIB_VIDEO_VM__720P30          ,
	/*!< 46 = vm63_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P120        ,
	/*!< 47 = vm64_1920x1080p,*/
	SII_LIB_VIDEO_VM__1080P100        ,
	/*!< 48 = vm_3840x2160p,*/
	SII_LIB_VIDEO_VM__2160P30         ,
	/*!< 49 = vm_3840x2160p,*/
	SII_LIB_VIDEO_VM__2160P25         ,
	/*!< 50 = vm_3840x2160p,*/
	SII_LIB_VIDEO_VM__2160P24         ,
	/*!< 51 = vm_4096x2160p,*/
	SII_LIB_VIDEO_VM__2160P24S        ,
	/*!< 52 = VM_PC_VGA72,*/
	SII_LIB_VIDEO_VM__VGA72           ,
	/*!< 53 = VM_PC_VGA75,*/
	SII_LIB_VIDEO_VM__VGA75           ,
	/*!< 54 = VM_PC_VGA85_1,*/
	SII_LIB_VIDEO_VM__VGA85_1         ,
	/*!< 55 = VM_PC_VGA85_2,*/
	SII_LIB_VIDEO_VM__VGA85_2         ,
	/*!< 56 = VM_PC_SVGA56,*/
	SII_LIB_VIDEO_VM__SVGA56          ,
	/*!< 57 = VM_PC_SVGA60,*/
	SII_LIB_VIDEO_VM__SVGA60          ,
	/*!< 58 = VM_PC_SVGA72,*/
	SII_LIB_VIDEO_VM__SVGA72          ,
	/*!< 59 = VM_PC_SVGA75,*/
	SII_LIB_VIDEO_VM__SVGA75          ,
	/*!< 60 = VM_PC_SVGA85,*/
	SII_LIB_VIDEO_VM__SVGA85          ,
	/*!< 61 = VM_PC_XGAI87,*/
	SII_LIB_VIDEO_VM__XGAI87          ,
	/*!< 62 = VM_PC_XGA60,*/
	SII_LIB_VIDEO_VM__XGA60           ,
	/*!< 63 = VM_PC_XGA70,*/
	SII_LIB_VIDEO_VM__XGA70           ,
	/*!< 64 = VM_PC_XGA75_1,*/
	SII_LIB_VIDEO_VM__XGA75_1         ,
	/*!< 65 = VM_PC_XGA85,*/
	SII_LIB_VIDEO_VM__XGA85           ,
	/*!< 66 = VM_PC_XGA75_2,*/
	SII_LIB_VIDEO_VM__XGA75_2         ,
	/*!< 67 = VM_PC_WXGA60_1,*/
	SII_LIB_VIDEO_VM__WXGA60_1        ,
	/*!< 68 = VM_PC_WXGA60_2,*/
	SII_LIB_VIDEO_VM__WXGA60_2        ,
	/*!< 69 = VM_PC_WXGA75,*/
	SII_LIB_VIDEO_VM__WXGA75          ,
	/*!< 70 = VM_PC_WXGA60_800,*/
	SII_LIB_VIDEO_VM__WXGA60_800      ,
	/*!< 71 = VM_PC_WXGA60_3,*/
	SII_LIB_VIDEO_VM__WXGA60_3        ,
	/*!< 72 = VM_PC_SXGA60,*/
	SII_LIB_VIDEO_VM__SXGA60          ,
	/*!< 73 = VM_PC_WSXGA60_1,*/
	SII_LIB_VIDEO_VM__WSXGA60_1       ,
	/*!< 74 = VM_PC_WSXGA60_2,*/
	SII_LIB_VIDEO_VM__WSXGA60_2       ,
	/*!< 75 = VM_PC_WXGA85_1,*/
	SII_LIB_VIDEO_VM__WXGA85_1        ,
	/*!< 76 = VM_PC_WXGA85_2,*/
	SII_LIB_VIDEO_VM__WXGA85_2        ,
	/*!< 77 = VM_PC_SXGA75,*/
	SII_LIB_VIDEO_VM__SXGA75          ,
	/*!< 78 = VM_PC_SXGA85,*/
	SII_LIB_VIDEO_VM__SXGA85          ,
	/*!< 79 = VM_PC_HD60,*/
	SII_LIB_VIDEO_VM__HD60            ,
	/*!< 80 = VM_PC_WSXGA60,*/
	SII_LIB_VIDEO_VM__WSXGA60         ,
	/*!< 81 = VM_PC_WSXGA75,*/
	SII_LIB_VIDEO_VM__WSXGA75         ,
	/*!< 82 = VM_PC_WSXGA50,*/
	SII_LIB_VIDEO_VM__WSXGA50         ,
	/*!< 83 = VM_PC_HD50_PLUS,*/
	SII_LIB_VIDEO_VM__HD50_PLUS       ,
	/*!< 84 = VM_PC_UXGA60,*/
	SII_LIB_VIDEO_VM__UXGA60          ,
	/*!< 85 = VM_PC_WSXGA50_PLUS,*/
	SII_LIB_VIDEO_VM__WSXGA50_PLUS    ,
	/*!< 86 = VM_PC_WUXGA50,*/
	SII_LIB_VIDEO_VM__WUXGA50         ,
	/*!< 87 = VM_PC_WUXGA60,*/
	SII_LIB_VIDEO_VM__WUXGA60         ,
	/*!< 88 = v_m_720_x480i,*/
	SII_LIB_VIDEO_VM__480I            ,
	/*!< 89 = v_m_720_x576i,*/
	SII_LIB_VIDEO_VM__576I            ,
	/*!< 90 = VM_960X540,*/
	SII_LIB_VIDEO_VM__540X60		  ,

	/*!< 91 = vm_3840x2160p,*/
	SII_LIB_VIDEO_VM__2160P50         ,
	/*!< 92 = vm_3840x2160p,*/
	SII_LIB_VIDEO_VM__2160P60         ,

	/*!< no video*/
	SII_LIB_VIDEO_VM__NONE
};

/**
* @brief picture aspect ratio
*/
enum sii_lib_video_pict_a_r {
	/*!< unknown or default*/
	SII_LIB_VIDEO_PICTAR__NONE,
	/*!< 4 x 3*/
	SII_LIB_VIDEO_PICTAR__4X3,
	/*!< 16 x 9*/
	SII_LIB_VIDEO_PICTAR__16X9,
	/*!< reserved*/
	SII_LIB_VIDEO_PICTAR__FUTURE,
	/*!< square pixel (number of pixels per line x number of lines)*/

		SII_LIB_VIDEO_PICTAR__SQP,

};

/**
* @brief video timing description
*/
struct sii_lib_video_timing {
	/*!< total number of lines per video frame*/
	uint16_t     v_total;

	/*!< number of active lines per video frame*/
	uint16_t
		v_active;
	/*!< v_sync width in lines*/
	uint16_t     v_sync;
	/*!< V front porch width in lines*/
	uint16_t     v_front_porch;
	/*!< V back porch width in lines*/
	uint16_t     v_back_porch;

	/*!< total number of pixels per video line*/
	uint16_t     h_total;

	/*!< number of active pixels per video line*/
	uint16_t
		h_active;
	/*!< h_sync width in pixels*/
	uint16_t     h_sync;
	/*!< H front porch width in pixels*/
	uint16_t     h_front_porch;

	/*!< H back porch width in pixels*/
	uint16_t     h_back_porch;

	/*!< pixel frequency*/
	uint32_t     pixel_freq;
	/*!< line (horizontal) frequency*/
	uint32_t     line_freq;
	/*!< frame (vertical) frequency*/
	uint32_t     v_freq;
	/*!< picture aspect ratio*/
	enum sii_lib_video_pict_a_r  pict_ar;
	/*!< 3d mode*/
	uint16_t   mode3_d;

	/*!< true for interlaced format, false for progressive*/
	uint8_t
		b_interlaced;
	/*!< true for negative V polarity, false for positive*/
	uint8_t
		b_vpol;
	/*!< true for negative H polarity, false for positive*/
	uint8_t
		b_hpol;
};

struct sii_lib_video_r_g_b {
	float f_r;
	float f_g;
	float f_b;
};

struct sii_lib_video_y_u_v {
	float f_y;
	float f_cb;
	float f_cr;
};

/**
* @brief color space and colorimetry
*/
enum sii_lib_video_clr_spc {
	/*!< unknown*/
	SII_LIB_VIDEO_CLRSPC__NONE,
	/*!< y_cb_cr ITU-601*/
	SII_LIB_VIDEO_CLRSPC__601,
	/*!< y_cb_cr ITU-709*/
	SII_LIB_VIDEO_CLRSPC__709,
	/*!< xv_y_c_c ITU-601*/
	SII_LIB_VIDEO_CLRSPC__XV601,
	/*!< xv_y_c_c ITU-709*/
	SII_LIB_VIDEO_CLRSPC__XV709,
	/*!< RGB*/
	SII_LIB_VIDEO_CLRSPC__RGB
};

/**
* @brief color space and colorimetry
*/
enum sii_lib_video_chroma_smpl {
	/*!< 4:4:4*/
	SII_LIB_VIDEO_CHROMASMPL__444,
	/*!< 4:2:2*/
	SII_LIB_VIDEO_CHROMASMPL__422,
	/*!< 4:2:0*/
	SII_LIB_VIDEO_CHROMASMPL__420
};

/* public functions */
void sii_lib_video_full_rgb_to_limited_rgb(struct sii_lib_video_r_g_b *p_rgb_o,
	 const struct sii_lib_video_r_g_b *p_rgb_i);
void sii_lib_video_rgb_to_yc709(struct sii_lib_video_y_u_v *p_yuv_o, const
	struct sii_lib_video_r_g_b *p_rgb_i);
void sii_lib_video_quantize_yc(struct sii_lib_video_y_u_v *p_yuv, uint8_t bits
	);
void sii_lib_video_rgb_to_yc601(struct sii_lib_video_y_u_v *p_yuv_o, const
	struct sii_lib_video_r_g_b *p_rgb_i);
void sii_lib_video_rgb_to_r_g_b(struct sii_lib_video_y_u_v *p_yuv_o, const
	struct sii_lib_video_r_g_b *p_rgb_i);
void sii_lib_video_quantize_rgb(struct sii_lib_video_r_g_b *p_rgb,
	uint8_t bits);
void sii_lib_video_timing_detail_get(enum sii_lib_video_v_m v_m,
	uint16_t mode3_d, bool per_field, struct sii_lib_video_timing
	*p_timing);
void sii_lib_c_e_a_video_mode_get(uint8_t vic_i_d, enum sii_lib_video_v_m
	*p_vid_mode);
void sii_lib_h_d_m_i_video_mode_get(uint8_t vic_i_d, enum sii_lib_video_v_m
	*p_vid_mode);

#endif /* __SI_LIB_VIDEO_H__*/

