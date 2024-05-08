/**
* @file  ms_hdmi.h  
* @brief HDMI协议相关数据结构定义                                                           
*/

/**
 * @defgroup MS_HDMI hdmi spec defines
 *
 * @brief hdmi defines
 * @{
 */


#ifndef __MS_HDMI_H__
#define __MS_HDMI_H__

#include "ms_typedef.h"

/** HDMI TMDS channel */
typedef enum _HDMI_TMDS_CHN_E
{
    HDMI_TMDS_DATA_CH0,
    HDMI_TMDS_DATA_CH1,
    HDMI_TMDS_DATA_CH2,
    HDMI_TMDS_DATA_CH_NUM,
} HDMI_TMDS_CH_E;

/** HDMI packet type*/
typedef enum _HDMI_PACKET_TYPE_E 
{
    HDMI_PACKET_TYPE_NONE = 0,
    HDMI_PACKET_TYPE_ACR,
    HDMI_PACKET_TYPE_AUD_SAMPLE,
    HDMI_PACKET_TYPE_GCP,
    HDMI_PACKET_TYPE_ACP,
    HDMI_PACKET_TYPE_ISRC1,
    HDMI_PACKET_TYPE_ISRC2,
    HDMI_PACKET_TYPE_ONE_BIT_AUD,
    HDMI_PACKET_TYPE_DST_AUD,
    HDMI_PACKET_TYPE_HBR_AUD,
    HDMI_PACKET_TYPE_GMP,
} HDMI_PACKET_TYPE_E;

/** HDMI infoframe type*/
typedef enum _HDMI_INFOFRAME_TYPE_E 
{
    HDMI_INFOFRAME_TYPE_VENDOR = 0x81,
    HDMI_INFOFRAME_TYPE_AVI    = 0x82,
    HDMI_INFOFRAME_TYPE_SPD    = 0x83,
    HDMI_INFOFRAME_TYPE_AUDIO  = 0x84,
    HDMI_INFOFRAME_TYPE_MPEG   = 0x85,
    HDMI_INFOFRAME_TYPE_DRM    = 0x87,
} HDMI_INFOFRAME_TYPE_E;

/** HDMI colorspace type*/
typedef enum _HDMI_COLORSPACE_E 
{
    HDMI_COLORSPACE_RGB,
    HDMI_COLORSPACE_YCBCR422,
    HDMI_COLORSPACE_YCBCR444,
    HDMI_COLORSPACE_YCBCR420,
    HDMI_COLORSPACE_RESERVED4,
    HDMI_COLORSPACE_RESERVED5,
    HDMI_COLORSPACE_RESERVED6,
    HDMI_COLORSPACE_IDO_DEFINED,
} HDMI_COLORSPACE_E;

/** HDMI scan mode type*/
typedef enum _HDMI_SCAN_MODE_E 
{
    HDMI_SCAN_MODE_NONE,
    HDMI_SCAN_MODE_OVERSCAN,
    HDMI_SCAN_MODE_UNDERSCAN,
    HDMI_SCAN_MODE_RESERVED,
} HDMI_SCAN_MODE_E;

/** HDMI colorimetry type*/
typedef enum _HDMI_COLORIMETRY_E 
{
    HDMI_COLORIMETRY_NONE,
    HDMI_COLORIMETRY_ITU_601,
    HDMI_COLORIMETRY_ITU_709,
    HDMI_COLORIMETRY_EXTENDED,
} HDMI_COLORIMETRY_E;

/** HDMI picture aspect type*/
typedef enum _HDMI_PICTURE_ASPECT_E
{
    HDMI_PICTURE_ASPECT_NONE,
    HDMI_PICTURE_ASPECT_4_3,
    HDMI_PICTURE_ASPECT_16_9,
    HDMI_PICTURE_ASPECT_64_27,
    HDMI_PICTURE_ASPECT_256_135,
    HDMI_PICTURE_ASPECT_RESERVED,
} HDMI_PICTURE_ASPECT_E;

/** HDMI active aspect type*/
typedef enum _HDMI_ACTIVE_ASPECT_E 
{
    HDMI_ACTIVE_ASPECT_16_9_TOP      = 2,
    HDMI_ACTIVE_ASPECT_14_9_TOP      = 3,
    HDMI_ACTIVE_ASPECT_16_9_CENTER   = 4,
    HDMI_ACTIVE_ASPECT_PICTURE       = 8,
    HDMI_ACTIVE_ASPECT_4_3           = 9,
    HDMI_ACTIVE_ASPECT_16_9          = 10,
    HDMI_ACTIVE_ASPECT_14_9          = 11,
    HDMI_ACTIVE_ASPECT_4_3_SP_14_9   = 13,
    HDMI_ACTIVE_ASPECT_16_9_SP_14_9  = 14,
    HDMI_ACTIVE_ASPECT_16_9_SP_4_3   = 15,
} HDMI_ACTIVE_ASPECT_E;

/** HDMI extended colorimetry type*/
typedef enum _HDMI_EXTENDED_COLORIMETRY_E 
{
    HDMI_EXTENDED_COLORIMETRY_XV_YCC_601,
    HDMI_EXTENDED_COLORIMETRY_XV_YCC_709,
    HDMI_EXTENDED_COLORIMETRY_S_YCC_601,
    HDMI_EXTENDED_COLORIMETRY_OPYCC_601,
    HDMI_EXTENDED_COLORIMETRY_OPRGB,

    /* The following EC values are only defined in CEA-861-F. */
    HDMI_EXTENDED_COLORIMETRY_BT2020_CONST_LUM,
    HDMI_EXTENDED_COLORIMETRY_BT2020,
    HDMI_EXTENDED_COLORIMETRY_RESERVED,
} HDMI_EXTENDED_COLORIMETRY_E;

/** non-uniform picture scaling */
typedef enum _HDMI_NUPS_E 
{
    HDMI_NUPS_UNKNOWN,
    HDMI_NUPS_HORIZONTAL,
    HDMI_NUPS_VERTICAL,
    HDMI_NUPS_BOTH,
} HDMI_NUPS_E;

/** HDMI rgb quantizaiton range type*/
typedef enum _HDMI_RGB_QUANTIZATION_RANGE_E 
{
    HDMI_QUANTIZATION_RANGE_DEFAULT,
    HDMI_QUANTIZATION_RANGE_LIMITED,
    HDMI_QUANTIZATION_RANGE_FULL,
    HDMI_QUANTIZATION_RANGE_RESERVED,
} HDMI_RGB_QUANTIZATION_RANGE_E;

/** HDMI ycc quantization range type*/
typedef enum _HDMI_YCC_QUANTIZATION_RANGE_E 
{
    HDMI_YCC_QUANTIZATION_RANGE_LIMITED,
    HDMI_YCC_QUANTIZATION_RANGE_FULL,
} HDMI_YCC_QUANTIZATION_RANGE_E;

/** HDMI content type*/
typedef enum _HDMI_CONTENT_TYPE_E 
{
    HDMI_CONTENT_TYPE_GRAPHICS,
    HDMI_CONTENT_TYPE_PHOTO,
    HDMI_CONTENT_TYPE_CINEMA,
    HDMI_CONTENT_TYPE_GAME,
} HDMI_CONTENT_TYPE_E;

/** HDMI static metadata type*/
typedef enum _HDMI_STATIC_METADATA_TYPE_E 
{
    HDMI_STATIC_METADATA_TYPE1 = 1,
} HDMI_STATIC_METADATA_TYPE_E;

/** HDMI eotf type*/
typedef enum _HDMI_EOTF_E 
{
    HDMI_EOTF_TRADITIONAL_GAMMA_SDR,
    HDMI_EOTF_TRADITIONAL_GAMMA_HDR,
    HDMI_EOTF_SMPTE_ST2084,
    HDMI_EOTF_BT_2100_HLG,
} HDMI_EOTF_E;

/** HDMI scan mode type*/
typedef enum _HDMI_AUDIO_CODING_TYPE_E 
{
    HDMI_AUDIO_CODING_TYPE_STREAM = 0,
    HDMI_AUDIO_CODING_TYPE_PCM,
    HDMI_AUDIO_CODING_TYPE_AC3,
    HDMI_AUDIO_CODING_TYPE_MPEG1,
    HDMI_AUDIO_CODING_TYPE_MP3,
    HDMI_AUDIO_CODING_TYPE_MPEG2,
    HDMI_AUDIO_CODING_TYPE_AAC,
    HDMI_AUDIO_CODING_TYPE_DTS,
    HDMI_AUDIO_CODING_TYPE_ATRAC,
    HDMI_AUDIO_CODING_TYPE_ONE_BIT_AUDIO,
    HDMI_AUDIO_CODING_TYPE_DDP,
    HDMI_AUDIO_CODING_TYPE_DTS_HD,
    HDMI_AUDIO_CODING_TYPE_MLP,
    HDMI_AUDIO_CODING_TYPE_DST,
    HDMI_AUDIO_CODING_TYPE_WMA_PRO,
    HDMI_AUDIO_CODING_TYPE_RESERVED,
} HDMI_AUDIO_CODING_TYPE_E;

/** HDMI audio stream type*/
typedef enum _HDMI_AUDIO_STREAM_TYPE_E 
{
    HDMI_AUDIO_STREAM_TYPE_MAS  = MSRT_BIT_MASK(5),
    HDMI_AUDIO_STREAM_TYPE_OBM  = MSRT_BIT_MASK(4),
    HDMI_AUDIO_STREAM_TYPE_HBR  = MSRT_BIT_MASK(3), 
    HDMI_AUDIO_STREAM_TYPE_DST  = MSRT_BIT_MASK(2),
    HDMI_AUDIO_STREAM_TYPE_OBA  = MSRT_BIT_MASK(1),
    HDMI_AUDIO_STREAM_TYPE_AUDS = MSRT_BIT_MASK(0),
} HDMI_AUDIO_STREAM_TYPE_E;

/** HDMI audio sample size type*/
typedef enum _HDMI_AUDIO_SAMPLE_SIZE_E 
{
    HDMI_AUDIO_SAMPLE_SIZE_STREAM = 0,
    HDMI_AUDIO_SAMPLE_SIZE_16,
    HDMI_AUDIO_SAMPLE_SIZE_20,
    HDMI_AUDIO_SAMPLE_SIZE_24,
} HDMI_AUDIO_SAMPLE_SIZE_E;

/** HDMI audio sample frequency*/
typedef enum _HDMI_AUDIO_SAMPLE_FREQUENCY_E 
{
    HDMI_AUDIO_SAMPLE_FREQUENCY_STREAM = 0,
    HDMI_AUDIO_SAMPLE_FREQUENCY_32000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_44100,
    HDMI_AUDIO_SAMPLE_FREQUENCY_48000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_88200,
    HDMI_AUDIO_SAMPLE_FREQUENCY_96000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_176400,
    HDMI_AUDIO_SAMPLE_FREQUENCY_192000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_384000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_768000,
    HDMI_AUDIO_SAMPLE_FREQUENCY_1536000,
} HDMI_AUDIO_SAMPLE_FREQUENCY_E;

/** HDMI color depth type*/
typedef enum _HDMI_COLOR_DEPTH_E
{
    HDMI_COLOR_DEPTH_NOT_INDICATED = 0,
    HDMI_COLOR_DEPTH_24 = 4,
    HDMI_COLOR_DEPTH_30 = 5,
    HDMI_COLOR_DEPTH_36 = 6,
    HDMI_COLOR_DEPTH_48 = 7
} HDMI_COLORDEPTH_E;

/** HDMI pixel packing phase type*/
typedef enum _HDMI_PIXEL_PACKING_PHASE_E
{
    HDMI_PIXEL_PACKING_PHASE_4 = 0,
    HDMI_PIXEL_PACKING_PHASE_1,
    HDMI_PIXEL_PACKING_PHASE_2,
    HDMI_PIXEL_PACKING_PHASE_3
} HDMI_PIXEL_PACKING_PHASE_E;

/** HDMI pixel repeat type*/
typedef enum _HDMI_PIXEL_REPEAT_E
{
    HDMI_PIXEL_REPEAT_X1 = 0,
    HDMI_PIXEL_REPEAT_X2,
    HDMI_PIXEL_REPEAT_X3,
    HDMI_PIXEL_REPEAT_X4,
    HDMI_PIXEL_REPEAT_X5,
    HDMI_PIXEL_REPEAT_X6,
    HDMI_PIXEL_REPEAT_X7,
    HDMI_PIXEL_REPEAT_X8,
    HDMI_PIXEL_REPEAT_X9,
    HDMI_PIXEL_REPEAT_X10
} HDMI_PIXEL_REPEAT_E;

/** HDMI gcp packet*/
typedef struct _HDMI_GCP_PACKET_T 
{
    UINT8 u8_set_avmute;
    UINT8 u8_clear_avmute;
    UINT8 e_color_depth;         /**< refer to @ref HDMI_COLORDEPTH_E*/
    UINT8 e_pixel_packing_phase; /**< refer to @ref HDMI_PIXEL_PACKING_PHASE_E*/
    UINT8 u8_default_phase;
} HDMI_GCP_PACKET_T;

/** HDMI raw amp packet*/
typedef struct _HDMI_RAW_AMP_PACKET_T
{
    UINT8 u8_hb_byte[2];
    UINT8 u8_packet_byte[28];
} HDMI_RAW_AMP_PACKET_T;

/** HDMI raw ntscvbi packet*/
typedef struct _HDMI_RAW_NTSCVBI_PACKET_T
{
    UINT8 u8_hb_byte[2];
    UINT8 u8_packet_byte[28];
} HDMI_RAW_NTSCVBI_PACKET_T;

/** HDMI raw infoframe*/
typedef struct _HDMI_RAW_INFOFRAME_T
{
    UINT8 e_type; /**< refer to @ref HDMI_INFOFRAME_TYPE_E*/
    UINT8 u8_version;
    UINT8 u8_length;
    UINT8 u8_packet_byte[28]; /**< PB0:checksum*/
} HDMI_RAW_INFOFRAME_T;

/** HDMI avi infoframe*/
typedef struct _HDMI_AVI_INFOFRAME_T
{
    UINT8 e_type;    /**< refer to @ref HDMI_INFOFRAME_TYPE_E*/
    UINT8 u8_version;
    UINT8 u8_length;
    UINT8 u8_checksum;
    UINT8 e_colorspace; /**< refer to @ref HDMI_COLORSPACE_E, pixel format: Y1,Y0 RGB or YCbCr indicator*/
    UINT8 e_scan_mode;  /**< refer to @ref HDMI_SCAN_MODE_E*/
    UINT8 e_colorimetry; /**< refer to @ref HDMI_COLORIMETRY_E*/
    UINT8 e_picture_aspect; /**< refer to @ref HDMI_PICTURE_ASPECT_E*/
    UINT8 e_active_aspect; /**< refer to @ref HDMI_ACTIVE_ASPECT_E*/
    BOOL b_itc;
    UINT8 e_extended_colorimetry; /**< refer to @ref HDMI_EXTENDED_COLORIMETRY_E*/
    UINT8 e_ycc_quantization_range; /**< refer to @ref HDMI_YCC_QUANTIZATION_RANGE_E*/
    UINT8 e_nups;  /**< refer to @ref HDMI_NUPS_E*/
    UINT8 u8_video_code;
    UINT8 e_rgb_quantization_range; /**< refer to @ref HDMI_RGB_QUANTIZATION_RANGE_E*/
    UINT8 e_content_type; /**< refer to @ref HDMI_CONTENT_TYPE_E*/
    BOOL b_active_info_present;
    UINT8 u8_pixel_repeat; /**< refer to @ref HDMI_PIXEL_REPEAT_E*/
    UINT8 u8_bar_info_valid;
    UINT16 u16_top_bar;
    UINT16 u16_bottom_bar;
    UINT16 u16_left_bar;
    UINT16 u16_right_bar;
} HDMI_AVI_INFOFRAME_T;

/** DRM Infoframe as per CTA 861.G spec */
typedef struct _HDMI_DRM_INFOFRAME_T 
{
    UINT8 e_type;    /**< refer to @ref HDMI_INFOFRAME_TYPE_E*/
    UINT8 u8_version;
    UINT8 u8_length;
    UINT8 e_eotf; /**< refer to @ref HDMI_EOTF_E*/
    UINT8 e_metadata_type; /**< refer to @ref HDMI_STATIC_METADATA_TYPE_E*/
    struct {
        UINT16 u16_x, u16_y;
    } st_display_primaries[3];
    struct {
        UINT16 u16_x, u16_y;
    } st_white_point;
    UINT16 u16_max_display_mastering_luminance;
    UINT16 u16_min_display_mastering_luminance;
    UINT16 u16_max_cll;
    UINT16 u16_max_fall;
} HDMI_DRM_INFOFRAME_T;

/** HDMI audio infoframe*/
typedef struct _HDMI_AUDIO_INFOFRAME_T
{
    UINT8 e_type;    /**< refer to @ref HDMI_INFOFRAME_TYPE_E*/
    UINT8 u8_version;
    UINT8 u8_length;
    UINT8 u8_checksum;
    UINT8 u8_multi_ch;
    UINT8 u8_channel_cnt;
    UINT8 u8_data_byte3;
    UINT8 e_coding_type; /**< refer to @ref HDMI_AUDIO_CODING_TYPE_E*/
    UINT8 e_sample_size; /**< refer to @ref HDMI_AUDIO_SAMPLE_SIZE_E*/
    UINT8 e_sample_frequency; /**< refer to @ref HDMI_AUDIO_SAMPLE_FREQUENCY_E*/
    UINT8 u8_channel_allocation;
    UINT8 u8_level_shift_value;
    BOOL downmix_inhibit;
    UINT8 u8_lfe_playback;
} HDMI_AUDIO_INFOFRAME_T;

/** HDMI 3D structure type*/
typedef enum _HDMI_3D_STRUCTURE_E 
{
    HDMI_3D_STRUCTURE_INVALID = -1,
    HDMI_3D_STRUCTURE_FRAME_PACKING = 0,
    HDMI_3D_STRUCTURE_FIELD_ALTERNATIVE,
    HDMI_3D_STRUCTURE_LINE_ALTERNATIVE,
    HDMI_3D_STRUCTURE_SIDE_BY_SIDE_FULL,
    HDMI_3D_STRUCTURE_L_DEPTH,
    HDMI_3D_STRUCTURE_L_DEPTH_GFX_GFX_DEPTH,
    HDMI_3D_STRUCTURE_TOP_AND_BOTTOM,
    HDMI_3D_STRUCTURE_SIDE_BY_SIDE_HALF = 8,
} HDMI_3D_STRUCTURE_E;

/** HDMI video format*/
typedef enum _HDMI_VIDEO_FORMAT_E
{
    HDMI_VIDEO_FORMAT_NOT_PRESENTED = 0,
    HDMI_VIDEO_FORMAT_EXTENDED_4K_2K_PRESENT = 1,
    HDMI_VIDEO_FORMAT_3D_PRESENT = 2,
} HDMI_VIDEO_FORMAT_E;

/** HDMI vendor infoframe*/
typedef struct _HDMI_VENDOR_INFOFRAME_T
{
    UINT8 e_type;    /**< refer to @ref HDMI_INFOFRAME_TYPE_E*/
    UINT8 u8_version;
    UINT8 u8_length;
    UINT8 u8_vic;
    UINT8 u8_hdmi_video_format; /**< This value defines the structure of extended video formats. @ref HDMI_VIDEO_FORMAT_E*/
    UINT8 e_3d_struct;  /**< refer to @ref HDMI_3D_STRUCTURE_E*/
    UINT8 u8_3d_ext_data;
} HDMI_VENDOR_INFOFRAME_T;

/** HDMI infoframe packet type*/
typedef struct _HDMI_INFOFRAMES_PACKETS_T
{
    /*packets and infoframes*/
    HDMI_GCP_PACKET_T       st_gcp_packet;
    HDMI_AVI_INFOFRAME_T    st_avi_infoframe;
    HDMI_AUDIO_INFOFRAME_T  st_audio_infoframe;
    HDMI_RAW_INFOFRAME_T    st_raw_drm_infoframe;
    HDMI_VENDOR_INFOFRAME_T st_vendor_infoframe;
} HDMI_INFOFRAMES_PACKETS_T;

/** HDMI EDID flag*/
typedef struct _HDMI_EDID_FLAG_T
{   
    UINT8    u8_hdmi_sink;                 /**< 1 = HDMI sink, 0 = dvi */
    UINT8    u8_color_space;               /**< color space support flag, flag 1 valid. BIT5: YCBCR444 flag; BIT4: YCBCR422 flag.(RGB must be support) */
    UINT8    u8_edid_total_blocks;         /**< block numbers, 128bytes in one block */
    UINT16   u16_preferred_pixel_clk;      /**< EDID Preferred pixel clock rate, u16_preferred_pixel_clk * 10000Hz, ERROR code is 0xFFFF */
    UINT32   u32_preferred_timing;         /**< EDID Preferred Timing (Hact*Vact) */
    UINT8    u8_max_tmds_clk;              /**< HDMI VSDB max tmds clock, u8_max_tmds_clk * 5 Mhz */
    UINT32   u32_max_video_block_timing;   /**< EDID max video block timing (Hact*Vact) */
    UINT8    u8_hdmi_2_0_flag;             /**< 1 = HDMI 2.0 */
}HDMI_EDID_FLAG_T;   

/** HDMI video timing infomation */
typedef struct _HDMI_VIDEO_TIMING_T
{
    UINT8           u8_polarity;     /**< The polarity of the synchronous signal, which points to SYNCPOLARITY_E */
    UINT16          u16_htotal;      /**< Line width（pixels number）                                                         */
    UINT16          u16_vtotal;      /**< Number of rows（lines number）                                                      */
    UINT16          u16_hactive;     /**< The number of valid pixels in a row                                                 */
    UINT16          u16_vactive;     /**< The number of valid pixels in a column                                              */
    UINT16          u16_pixclk;      /**< video clock，unit is 10000Hz                                                        */
    UINT16          u16_vfreq;       /**< video line frequency，unit is 0.01Hz                                                */
    UINT16          u16_hoffset;     /**< h sync start to h active                                                            */
    UINT16          u16_voffset;     /**< v sync start to v active                                                            */
    UINT16          u16_hsyncwidth;  /**< Line sync width（pixels number）                                                    */
    UINT16          u16_vsyncwidth;  /**< Column sync width（lines number）                                                   */
} HDMI_VIDEO_TIMING_T;

#endif //__MS_HDMI_H__

/**
 * @}
 */

