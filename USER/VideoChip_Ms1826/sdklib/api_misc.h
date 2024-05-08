/**
* @file  api_misc.h
* @brief TOP MISC层相关配置
*/

/**
 * @defgroup API_MISC misc layer api
 *
 * @brief TOP MISC层主要完成图像处理外的通路相关配置
 * @{
 */

#ifndef __API_MISC__
#define __API_MISC__

#include "ms_typedef.h"
#include "ms_hdmi.h"

/** 视频通路HDMI_PORT枚举 */
typedef enum _MISC_HDMI_E
{
    MISC_CHAN_HDMI_1 = 0,
    MISC_CHAN_HDMI_2,
    MISC_CHAN_HDMI_3,
    MISC_CHAN_HDMI_4,

    MISC_CHAN_BUTT,
}MISC_HDMI_E;

/** 视频通路前端PATTERN模块枚举 */
typedef enum _MISC_RXPATTERN_E
{
    MISC_RXPATTERN_1 = 0,
    MISC_RXPATTERN_2,
    MISC_RXPATTERN_3,
    MISC_RXPATTERN_4,

    MISC_RXPATTERN_BUTT,
}MISC_RXPATTERN_E;

/** MISC V(I/O)PSS声明，输入输出各分为四个V(I/O)PSS */
typedef enum _MISC_VPSS_E
{
    MISC_VPSS_1 = 0,
    MISC_VPSS_2,
    MISC_VPSS_3,
    MISC_VPSS_4,

    MISC_VPSS_BUTT,
}MISC_VPSS_E;

/** VIPSS端通路枚举，VIPSS1支持10种通路处理，VIPSS2~4支持7种通路处理 */
typedef enum _MISC_VIPSS_MUXTYPE_E
{
    MISC_VIPSS_MUXTYPE_1 = 0,   /**< CSC -> DSC_E -> VWDMA0 */
    MISC_VIPSS_MUXTYPE_2,       /**< CSC -> SPLIT -> DSC_E -> VWDMA0 */
    MISC_VIPSS_MUXTYPE_3,       /**< CSC -> SDN -> DSC_E -> VWDMA0 */
    MISC_VIPSS_MUXTYPE_4,       /**< CSC -> SDN -> VWDMA1 */
    MISC_VIPSS_MUXTYPE_5,       /**< CSC -> 4KSDN -> SDN -> VWDMA1 */
    MISC_VIPSS_MUXTYPE_6,       /**< CSC -> 4KSDN -> SDN -> DSC_E -> VWDMA0 */
    MISC_VIPSS_MUXTYPE_7,       /**< CSC -> 4KSDN -> SPLIT -> DSC_E -> VWDMA0 */
    MISC_VIPSS_MUXTYPE_8,       /**< CSC -> DEINT -> SDN -> VWDMA1 */
    MISC_VIPSS_MUXTYPE_9,       /**< CSC -> DEINT -> SDN -> DSC_E -> VWDMA0 */
    MISC_VIPSS_MUXTYPE_10,      /**< CSC -> DEINT -> SPLIT -> DSC_E -> VWDMA0 */

    MISC_VIPSS_MUXTYPE_BUTT,
}MISC_VIPSS_MUXTYPE_E;

/** Freq meter功能测试时钟源枚举 */
typedef enum _MISC_TEST_CLOCK_E
{
    MISC_TEST_RX1_TMDS_CLK              = 0,
    MISC_TEST_RX2_TMDS_CLK              = 1,
    MISC_TEST_RX3_TMDS_CLK              = 2,
    MISC_TEST_RX4_TMDS_CLK              = 3,
    MISC_TEST_RX1_PIXEL_CLK             = 4,
    MISC_TEST_RX2_PIXEL_CLK             = 5,
    MISC_TEST_RX3_PIXEL_CLK             = 6,
    MISC_TEST_RX4_PIXEL_CLK             = 7,
    MISC_TEST_RX1_AUDIO_CLK             = 8,
    MISC_TEST_RX2_AUDIO_CLK             = 9,
    MISC_TEST_RX3_AUDIO_CLK             = 10,
    MISC_TEST_RX4_AUDIO_CLK             = 11,
    MISC_TEST_TX1_TMDS_CLK              = 12,
    MISC_TEST_TX1_PREP_CLK              = 13,
    MISC_TEST_TX2_TMDS_CLK              = 14,
    MISC_TEST_TX2_PREP_CLK              = 15,
    MISC_TEST_TX3_TMDS_CLK              = 16,
    MISC_TEST_TX3_PREP_CLK              = 17,
    MISC_TEST_TX4_TMDS_CLK              = 18,
    MISC_TEST_TX4_PREP_CLK              = 19,
    MISC_TEST_PLLM_CLK                  = 20,
    MISC_TEST_PLLV1_CLK                 = 21,
    MISC_TEST_PLLV2_CLK                 = 22,
    MISC_TEST_PLLV3_CLK                 = 23,
    MISC_TEST_PLLV4_CLK                 = 24,
    MISC_TEST_PAM_OUT_CLK               = 25,

    MISC_TEST_CLK_BUTT,
} MISC_TEST_CLOCK_E;

/** MISC层各模块枚举 */
typedef enum _MISC_VIDEO_MODULE_E
{
    MISC_VIDEO_MODULE_VIPSS1 = 0,
    MISC_VIDEO_MODULE_VIPSS2,
    MISC_VIDEO_MODULE_VIPSS3,
    MISC_VIDEO_MODULE_VIPSS4,
    MISC_VIDEO_MODULE_SDN1,
    MISC_VIDEO_MODULE_SDN2,
    MISC_VIDEO_MODULE_SDN3,
    MISC_VIDEO_MODULE_SDN4,
    MISC_VIDEO_MODULE_SPLIT1,
    MISC_VIDEO_MODULE_SPLIT2,
    MISC_VIDEO_MODULE_SPLIT3,
    MISC_VIDEO_MODULE_SPLIT4,
    MISC_VIDEO_MODULE_DSC1,
    MISC_VIDEO_MODULE_DSC2,
    MISC_VIDEO_MODULE_DSC3,
    MISC_VIDEO_MODULE_DSC4,
    MISC_VIDEO_MODULE_RX1,                  // 0x10
    MISC_VIDEO_MODULE_RX2,
    MISC_VIDEO_MODULE_RX3,
    MISC_VIDEO_MODULE_RX4,
    MISC_VIDEO_MODULE_TX1,
    MISC_VIDEO_MODULE_TX2,
    MISC_VIDEO_MODULE_TX3,
    MISC_VIDEO_MODULE_TX4,
    MISC_VIDEO_MODULE_MIPI,
    MISC_VIDEO_MODULE_CSC,
    MISC_VIDEO_MODULE_SPLIT,
    MISC_VIDEO_MODULE_4KSDN,
    MISC_VIDEO_MODULE_SDN,
    MISC_VIDEO_MODULE_DEINT,
    MISC_VIDEO_MODULE_DMC,
    MISC_VIDEO_MODULE_MIX1,
    MISC_VIDEO_MODULE_MIX2,                 // 0x20
    MISC_VIDEO_MODULE_MIX3,
    MISC_VIDEO_MODULE_MIX4,
    MISC_VIDEO_MODULE_TIMGEN1,
    MISC_VIDEO_MODULE_TIMGEN2,
    MISC_VIDEO_MODULE_TIMGEN3,
    MISC_VIDEO_MODULE_TIMGEN4,
    MISC_VIDEO_MODULE_PLLV1,
    MISC_VIDEO_MODULE_PLLV2,
    MISC_VIDEO_MODULE_PLLV3,
    MISC_VIDEO_MODULE_PLLV4,
    MISC_VIDEO_MODULE_PLLM,
    MISC_VIDEO_MODULE_VOPSS1,
    MISC_VIDEO_MODULE_VOPSS2,
    MISC_VIDEO_MODULE_VOPSS3,
    MISC_VIDEO_MODULE_VOPSS4,
    MISC_VIDEO_MODULE_VOPASS1SU0,           // 0x30
    MISC_VIDEO_MODULE_VOPASS1SU1,
    MISC_VIDEO_MODULE_VOPASS2SU0,
    MISC_VIDEO_MODULE_VOPASS2SU1,
    MISC_VIDEO_MODULE_VOPASS3SU0,
    MISC_VIDEO_MODULE_VOPASS3SU1,
    MISC_VIDEO_MODULE_VOPASS4SU0,
    MISC_VIDEO_MODULE_VOPASS4SU1,
    MISC_VIDEO_MODULE_OSD1,
    MISC_VIDEO_MODULE_OSD2,
    MISC_VIDEO_MODULE_OSD3,
    MISC_VIDEO_MODULE_OSD4,
    MISC_VIDEO_MODULE_VIPASS1VWDMA0,
    MISC_VIDEO_MODULE_VIPASS1VWDMA1,
    MISC_VIDEO_MODULE_VIPASS2VWDMA0,
    MISC_VIDEO_MODULE_VIPASS2VWDMA1,
    MISC_VIDEO_MODULE_VIPASS3VWDMA0,        // 0x40
    MISC_VIDEO_MODULE_VIPASS3VWDMA1,
    MISC_VIDEO_MODULE_VIPASS4VWDMA0,
    MISC_VIDEO_MODULE_VIPASS4VWDMA1,
    MISC_VIDEO_MODULE_I2SPDIF1,
    MISC_VIDEO_MODULE_I2SPDIF2,
    MISC_VIDEO_MODULE_I2SPDIF3,
    MISC_VIDEO_MODULE_I2SPDIF4,
    MISC_VIDEO_MODULE_VOPASS1VRDMA0,
    MISC_VIDEO_MODULE_VOPASS1VRDMA1,
    MISC_VIDEO_MODULE_VOPASS2VRDMA0,
    MISC_VIDEO_MODULE_VOPASS2VRDMA1,
    MISC_VIDEO_MODULE_VOPASS3VRDMA0,
    MISC_VIDEO_MODULE_VOPASS3VRDMA1,
    MISC_VIDEO_MODULE_VOPASS4VRDMA0,
    MISC_VIDEO_MODULE_VOPASS4VRDMA1,
    MISC_VIDEO_MODULE_RXPATTERN,            // 0x50

    MISC_VIDEO_MODULE_BUTT,
}MISC_VIDEO_MODULE_E;

/** MISC层时钟分频系数枚举 */
typedef enum _MISC_CLKDIV_E
{
    MISC_CLKDIV_1 = 0,
    MISC_CLKDIV_2 = 1,
    MISC_CLKDIV_3 = 2,
    MISC_CLKDIV_4 = 3,
    MISC_CLKDIV_5 = 4,
    MISC_CLKDIV_6 = 5,
    MISC_CLKDIV_7 = 6,
    MISC_CLKDIV_8 = 7,
    MISC_CLKDIV_9 = 8,
    MISC_CLKDIV_10 = 9,
    MISC_CLKDIV_11 = 10,
    MISC_CLKDIV_12 = 11,
    MISC_CLKDIV_13 = 12,
    MISC_CLKDIV_14 = 13,
    MISC_CLKDIV_15 = 14,
    MISC_CLKDIV_16 = 15,

    MISC_CLKDIV_BUTT,
}MISC_CLKDIV_E;

/** MISC层Repeat系数枚举 */
typedef enum _MISC_REPEAT_E
{
    MISC_REPEAT_1X = 0,
    MISC_REPEAT_2X = 1,
    MISC_REPEAT_3X = 2,
    MISC_REPEAT_4X = 3,
    MISC_REPEAT_5X = 4,
    MISC_REPEAT_6X = 5,
    MISC_REPEAT_7X = 6,
    MISC_REPEAT_8X = 7,
    MISC_REPEAT_9X = 8,
    MISC_REPEAT_10X = 9,
    MISC_REPEAT_11X = 10,
    MISC_REPEAT_12X = 11,
    MISC_REPEAT_13X = 12,
    MISC_REPEAT_14X = 13,
    MISC_REPEAT_15X = 14,
    MISC_REPEAT_16X = 15,

    MISC_REPEAT_BUTT,
}MISC_REPEAT_E;

/** 时钟源枚举*/
typedef enum _MISC_CLKSRC_E
{
    MISC_CLKSRC_XTAL = 0,
    MISC_CLKSRC_AUDIO_DLLPLL,
    MISC_CLKSRC_PIXELCLK,
    MISC_CLKSRC_TMDSCLK,
    MISC_CLKSRC_RX1,
    MISC_CLKSRC_RX2,
    MISC_CLKSRC_RX3,
    MISC_CLKSRC_RX4,
    MISC_CLKSRC_PLLV1,
    MISC_CLKSRC_PLLV2,
    MISC_CLKSRC_PLLV3,
    MISC_CLKSRC_PLLV4,
    MISC_CLKSRC_VDSTMG1,
    MISC_CLKSRC_VDSTMG2,
    MISC_CLKSRC_VDSTMG3,
    MISC_CLKSRC_VDSTMG4,
    MISC_CLKSRC_REFTX1, /**< OSD输出REF TX输入时钟*/
    MISC_CLKSRC_REFTX2, /**< OSD输出REF TX输入时钟*/
    MISC_CLKSRC_REFTX3, /**< OSD输出REF TX输入时钟*/
    MISC_CLKSRC_REFTX4, /**< OSD输出REF TX输入时钟*/

    MISC_CLKSRC_BUTT,
}MISC_CLKSRC_E;

/** 中断源枚举*/
typedef enum _MISC_INTSRC_E
{
    MISC_INTSRC_RX1 = 0,
    MISC_INTSRC_RX2,
    MISC_INTSRC_RX3,
    MISC_INTSRC_RX4,
    MISC_INTSRC_RX1VB,
    MISC_INTSRC_RX2VB,
    MISC_INTSRC_RX3VB,
    MISC_INTSRC_RX4VB,
    MISC_INTSRC_VDSTMG1VB,
    MISC_INTSRC_VDSTMG2VB,
    MISC_INTSRC_VDSTMG3VB,
    MISC_INTSRC_VDSTMG4VB,
    MISC_INTSRC_TX1,
    MISC_INTSRC_TX2,
    MISC_INTSRC_TX3,
    MISC_INTSRC_TX4,
    MISC_INTSRC_RX1VDE,
    MISC_INTSRC_RX2VDE,
    MISC_INTSRC_RX3VDE,
    MISC_INTSRC_RX4VDE,
    MISC_INTSRC_VDSTMG1VDE,
    MISC_INTSRC_VDSTMG2VDE,
    MISC_INTSRC_VDSTMG3VDE,
    MISC_INTSRC_VDSTMG4VDE,
    MISC_INTSRC_VIPSS1SRCVB,
    MISC_INTSRC_VIPSS2SRCVB,
    MISC_INTSRC_VIPSS3SRCVB,
    MISC_INTSRC_VIPSS4SRCVB,
    MISC_INTSRC_VIPSS1SRCVDE,
    MISC_INTSRC_VIPSS2SRCVDE,
    MISC_INTSRC_VIPSS3SRCVDE,
    MISC_INTSRC_VIPSS4SRCVDE,
    /*..更多中断源此处添加...*/

    MISC_INTSRC_BUTT,
}MISC_INTSRC_E;

/** VBPATH枚举 */
typedef enum _MISC_VBPATH_E
{
    MISC_VBPATH_RX1 = 0,
    MISC_VBPATH_RX2,
    MISC_VBPATH_RX3,
    MISC_VBPATH_RX4,
    MISC_VBPATH_VDSTMG1,
    MISC_VBPATH_VDSTMG2,
    MISC_VBPATH_VDSTMG3,
    MISC_VBPATH_VDSTMG4,
    MISC_VBPATH_VOPASS1SU0,
    MISC_VBPATH_VOPASS1SU1,
    MISC_VBPATH_VOPASS2SU0,
    MISC_VBPATH_VOPASS2SU1,
    MISC_VBPATH_VOPASS3SU0,
    MISC_VBPATH_VOPASS3SU1,
    MISC_VBPATH_VOPASS4SU0,
    MISC_VBPATH_VOPASS4SU1,

    MISC_VBPATH_BUTT,
}MISC_VBPATH_E;

/** RXPHY通道swap方式枚举 */
typedef enum _MISC_RXPHY_CHAN_SWAP_E
{
    MISC_SWAP_012 = 0,
    MISC_SWAP_120,
    MISC_SWAP_201,
    MISC_SWAP_021,
    MISC_SWAP_102,
    MISC_SWAP_210,

    MISC_SWAP_BUTT,
}MISC_RXPHY_CHAN_SWAP_E;

/** RXPATTERN TIMING选择 */
typedef enum _MISC_RXPATTERN_TIMING_E
{
    MISC_RXPATTERN_480P = 0,
    MISC_RXPATTERN_720P,
    MISC_RXPATTERN_1080P,
    MISC_RXPATTERN_4K,

    MISC_RXPATTERN_TIMING_BUTT,
}MISC_RXPATTERN_TIMING_E;

/** RXPATTERN TYPE枚举 */
typedef enum _MISC_RXPATTERN_MODE_E
{
    MISC_RXPATTERN_MODE_PURE_BLACK,             /**< 纯黑色图             */
    MISC_RXPATTERN_MODE_PURE_BLUE,              /**< 纯蓝色图             */
    MISC_RXPATTERN_MODE_PURE_GREEN,             /**< 纯绿色图             */
    MISC_RXPATTERN_MODE_PURE_RED,               /**< 纯红色图             */
    MISC_RXPATTERN_MODE_PURE_WHITE,             /**< 纯白色图             */
    MISC_RXPATTERN_MODE_CROSS_HATCH,            /**< 黑色方格图           */
    MISC_RXPATTERN_MODE_HOR_RAMP,               /**< 水平变化的灰阶图      */
    MISC_RXPATTERN_MODE_VER_RAMP,               /**< 垂直变化的灰阶图      */
    MISC_RXPATTERN_MODE_COLOR_BAR,              /**< 彩色条               */
    MISC_RXPATTERN_MODE_HOR_GRAY_SCALE,         /**< 水平条形灰阶图        */
    MISC_RXPATTERN_MODE_VER_GRAY_SCALE,         /**< 垂直条形灰阶图        */
    MISC_RXPATTERN_MODE_2ND_HOR_GARY_SCALE,     /**< 水平双向条形逐变灰阶图 */
    MISC_RXPATTERN_MODE_PRIMARY_COLOR,          /**< 原色图                */
    MISC_RXPATTERN_MODE_INIERLACE_BLACK,        /**< 隔行黑色图            */
    MISC_RXPATTERN_MODE_INTERLACE_RED,          /**< 隔行红色图            */
    MISC_RXPATTERN_MODE_INTERLACE_GREEN,        /**< 隔行绿色图            */
    MISC_RXPATTERN_MODE_INTERLACE_BLUE,         /**< 隔行蓝色图            */

    MISC_RXPATTERN_MODE_BUTT,
}MISC_RXPATTERN_MODE_E;
/**
 * for MS1826, I2S1 ~ I2S4 and SPDIF1 ~ SPDIF4 are all used;
 * for MS1826A and MS1826B,Only I2S3 and SPDIF3 is used.
*/
typedef enum _ENUM_MISC_TX_AUDIO_SEL
{
    RX1_AUDIO = 0,
    RX2_AUDIO,
    RX3_AUDIO,
    RX4_AUDIO,
    I2S1_AUDIO,
    SPIDF1_AUDIO,
    I2S2_AUDIO,
    SPIDF2_AUDIO,
    I2S3_AUDIO,
    SPIDF3_AUDIO,
    I2S4_AUDIO,
    SPIDF4_AUDIO,
    TX_AUDIO_BUTT
}ENUM_MISC_TX_AUDIO_SEL;

typedef struct _MISC_TX_AUDIO_SEL
{
    UINT8 u8_audio_src; /**< TX 音频来源，refer to ENUM_MISC_TX_AUDIO_SEL*/
    UINT8 U8_tx_dst;    /**< 选择配置哪路TX,refer to MISC_VIDEO_MODULE_E,仅可选MISC_VIDEO_MODULE_TX1/2/3/4*/
}MISC_TX_AUDIO_SEL;

/**
 * for MS1826, I2S1 ~ I2S4 and SPDIF1 ~ SPDIF4 are all used;
 * for MS1826A and MS1826B,Only I2S3 and SPDIF3 is used.
*/
typedef enum _ENUM_MISC_DIGITAL_AUDIO_DST_SEL
{
    DIG_AUDIO_I2S1,
    DIG_AUDIO_I2S2,
    DIG_AUDIO_I2S3,
    DIG_AUDIO_I2S4,
    DIG_AUDIO_SPDIF1,
    DIG_AUDIO_SPDIF2,
    DIG_AUDIO_SPDIF3,
    DIG_AUDIO_SPDIF4,
    DIG_AUDIO_BUTT
}ENUM_MISC_DIGITAL_AUDIO_DST_SEL;

/**
 * for MS1826, I2S1 ~ I2S4 and SPDIF1 ~ SPDIF4 are all used;
 * for MS1826A and MS1826B,Only I2S3 and SPDIF3 is used.
*/
typedef struct _MISC_DIGITAL_AUDIO_CFG
{
    BOOL  b_enable;                 /**< 数字音频输出使能控制*/
    UINT8 u8_audio_src;             /**< TX 数字音频来源，refer to ENUM_MISC_TX_AUDIO_SEL，仅可选RX1_AUDIO/RX2_AUDIO/RX3_AUDIO/RX4_AUDIO*/
    UINT8 u8_digital_audio_dst;     /**< 选择配置哪路数字音频，refer to ENUM_MISC_DIGITAL_AUDIO_DST_SEL*/
}MISC_DIGITAL_AUDIO_CFG;

/**
 * for MS1826, I2S1 ~ I2S4 and SPDIF1 ~ SPDIF4 are all used;
 * for MS1826A and MS1826B,Only I2S3 and SPDIF3 is used.
*/
typedef struct _MISC_MCLK_CFG_T
{
    UINT8 u8_digital_audio_dst;  /**< 配置I2S通路，refer to ENUM_MISC_DIGITAL_AUDIO_DST_SEL,只可选DIG_AUDIO_I2S1/2/3/4*/
    UINT8 u8_audio_src;          /**< I2S音频来源，refer to ENUM_MISC_TX_AUDIO_SEL，仅可选RX1_AUDIO/RX2_AUDIO/RX3_AUDIO/RX4_AUDIO*/
    UINT8 u8_mclk_en;            /**< MCLK输出使能*/
    UINT8 u8_mclk_div;           /**< 输出分频系数，详见MISC_CLKDIV_E，仅可选MISC_CLKDIV_1/2/4/8 */
    BOOL  b_mclk_from_crystal;   /**< mclk src is from rx or crystal */
    UINT32 u32_mclk_freq;        /**< mclk frequency,only used whwen mclk src is from crystal */
}MISC_MCLK_CFG_T;

/** RX PHY P/N转换设置*/
typedef struct _MISC_RXPHY_PN_SWAP_T
{
    UINT8 u8_phy_sel;    /**< PHY选择，详见MISC_VIDEO_MODULE_E，仅可选MISC_VIDEO_MODULE_RX1/2/3/4*/
    BOOL b_data0_swap_en;/**< DATA0 P/N 转换使能*/
    BOOL b_data1_swap_en;/**< DATA1 P/N 转换使能*/
    BOOL b_data2_swap_en;/**< DATA2 P/N 转换使能*/
}MISC_RXPHY_PN_SWAP_T;

/** RX PHY CHAN转换设置*/
typedef struct _MISC_RXPHY_CHAN_SWAP_T
{
    UINT8 u8_phy_sel;    /**< PHY选择，详见MISC_VIDEO_MODULE_E，仅可选MISC_VIDEO_MODULE_RX1/2/3/4*/
    UINT8 u8_swap_mode;  /**< 转换选择，根据外部顺序选择对应枚举值，内部转为012顺序，详见MISC_RXPHY_CHAN_SWAP_E*/
}MISC_RXPHY_CHAN_SWAP_T;

/** 芯片信息*/
typedef struct _MISC_CHIPID_INFO_T
{
    UINT32 u32_chid_id;    /**< 芯片ID，Chip ID : 0x6A82A8*/
}MISC_CHIP_INFO_T;

/** PIXEL REPEAT设置*/
typedef struct _MISC_REPEAT_T
{
    UINT8 u8_module_sel;   /**< TX端模块选择，详见MISC_PAD_MODE_E中MISC_VIDEO_MODULE_TX1/2/3/4/MIPI*/
    UINT8 u8_repeat_num;   /**< repeat num，详见MISC_REPEAT_E*/
}MISC_REPEAT_CFG_T;

/** Field Det功能*/
typedef struct _MISC_FIELDET_CFG_T
{
    UINT8 u8_module_sel;   /**< VIPSS模块选择，详见MISC_PAD_MODE_E中MISC_VIDEO_MODULE_VIPSS1/2/3/4*/
    BOOL b_fieldet_en;     /**< Field探测使能 */
}MISC_FIELDET_CFG_T;

/** 中断输出设置*/
typedef struct _MISC_INT_CFG_T
{
    UINT8 u8_int_src;     /**< 中断源选择，详见MISC_INTSRC_E*/
    BOOL b_src_to_pad;    /**< 中断输出到PAD */
    BOOL b_pad_en;        /**< 中断pad输出使能 */
    BOOL b_sync_low;      /**< PAD低电平有效使能 */
}MISC_INT_CFG_T;

/** RX PHY SWAP配置*/
typedef struct _MISC_RXPHY_SWAP_T
{
    MISC_RXPHY_PN_SWAP_T st_rxphy_pn_swap;      /**< RXPHY通道内PN swap结构体 */
    MISC_RXPHY_CHAN_SWAP_T st_rxphy_chan_swap;  /**< RXPHY通道间 swap结构体 */
}MISC_RXPHY_SWAP_T;

/** HDMIRX端输出极性配置 */
typedef struct _MISC_RX_POLARITY_T
{
    UINT8 u8_chan;          /** 通道选择，详见详见MISC_VIDEO_MODULE_E */
    UINT8 u8_hsync_flip_en; /** HSYNC极性翻转使能 */
    UINT8 u8_vsync_flip_en; /** VSYNC极性翻转使能 */
}MISC_POLARITY_T;

/** OSD输出通路选择 */
typedef struct _MISC_OSD_MUX_T
{
    BOOL b_enable;        /**< 通路是否选择使能 */
    UINT8 u8_osd_module;  /**< OSD模块选择，详见MISC_VIDEO_MODULE_E中MISC_VIDEO_MODULE_OSD1/2/3/4 */
    UINT8 u8_tx_module;   /**< TX模块选择，详见MISC_VIDEO_MODULE_E中MISC_VIDEO_MODULE_TX1/2/3/4 */
}MISC_OSD_MUX_T;

/** TX输出通路选择 */
typedef struct _MISC_TX_MUX_T
{
    UINT8 u8_tx_module;  /**< TX模块选择，详见MISC_VIDEO_MODULE_E中MISC_VIDEO_MODULE_TX1/2/3/4 */
    UINT8 u8_video_mux;  /**< TX VIDEO MUX选择，详见MISC_VIDEO_MODULE_E中MISC_VIDEO_MODULE_RX1/2/3/4 或 MISC_VIDEO_MODULE_MIXER1/2/3/4 */
}MISC_TX_MUX_T;

/** VIPSS输入MUX选择 */
typedef struct _MISC_VIDEO_MUX_T
{
    UINT8 u8_dst_module;    /**< 待配置的模块选择，详见MISC_VIDEO_MODULE_E */
    UINT8 u8_src_module;    /**< 模块的视频源选择，详见MISC_VIDEO_MODULE_E */
}MISC_VIDEO_MUX_T;

/** 模块时钟源选择*/
typedef struct _MISC_MODULE_CLK_T
{
    UINT8 u8_dst_module;    /**< 选择待配置的模块，详见MISC_VIDEO_MODULE_E*/
    UINT8 u8_src_clk;       /**< 选择时钟源，详见MISC_CLKSRC_E*/
    UINT8 u8_clkdiv;        /**< 时钟分频系数，详见MISC_CLKDIV_E，默认不分频*/
}MISC_MODULE_CLK_T;

/** 模块使能*/
typedef struct _MISC_MODULE_ENABLE_T
{
    UINT8 u8_module;    /**< 选择模块，详见MISC_VIDEO_MODULE_E*/
    BOOL b_enable;      /**< TOP层模块使能，控制rstb&clk位*/
}MISC_MODULE_ENABLE_T;

/** RXPATTERN配置参数 */
typedef struct _MISC_RXPATTERN_T
{
    UINT8 u8_pattern_idx;/**< PATTERN索引，详见MISC_RXPATTERN_E*/
    BOOL b_enable;      /**< PATTERN使能*/
    UINT8 u8_timing;    /**< 输出Timing选择，详见MISC_RXPATTERN_TIMING_E*/
    UINT8 u8_pattern;   /**< 输出Pattern选择，详见MISC_RXPATTERN_MODE_E*/
}MISC_RXPATTERN_T;

/** 时钟频率测试*/
typedef struct _MISC_TEST_FREQ_T
{
    BOOL b_freq_en;         /**< FreqMeter使能*/
    UINT8 u8_clk_sel;       /**< 选择测试时钟源，详见MISC_TEST_CLOCK_E */
}MISC_TEST_FREQ_T;

typedef struct _MISC_TIMING_T
{
    UINT16           u16_vic;      /**< VIDEO TIMIMG描述信息，详见MS1826_VIDEOFORMAT_E*/
    HDMI_VIDEO_TIMING_T   st_timing;
}MISC_TIMING_T;

typedef enum _ENUM_MISC_CHIP_ADDR_SEL_E
{
    CHIP_ADDR_B6 = 0xb6,
    CHIP_ADDR_BE = 0xbe
}ENUM_MISC_CHIP_ADDR;

/**
* @brief        设置芯片的i2c地址
* @param [in]   u8_address : i2c访问方式时，i2c地址可以是0xb6或0xbe,refer to ENUM_MISC_CHIP_ADDR，引脚SASEL = 0,i2c地址为0xb6，SASEL = 1,i2c地址为0xbe
*
* @return       ERR_INV_PARM if u8_address is wrong\n
*               MS_ERR_OK if init successed
* @note
*/
ERR_TYPE_E api_misc_set_i2c_addr(UINT8 u8_address);

/**
* @brief        获取当前芯片的i2c地址
* @return       UINT8 芯片地址
*
*/
UINT8 api_misc_get_i2c_addr(VOID);

/**
* @brief        芯片信息获取
* @param [in]   pst_chip_info：芯片信息结构体指针
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_chip_info_get(MISC_CHIP_INFO_T *pst_chip_info);

/**
* @brief        VIDEO TIMING表初始化（APP定义VIDEO TIMING Table）
* @param [in]   pst_timing_table：Timing表指针
*               u8_timing_num：Timing表中Timing个数
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timing_table_init(MISC_TIMING_T* pst_timing_table, UINT16 u16_timing_num);

/**
* @brief        基于索引获取Timing信息
* @param [in]   u8_vic：VIDEO TIMIMG描述信息，详见MS1826_VIDEOFORMAT_E
*               ptTiming：Timing数据存储结构体指针
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timing_get(UINT8 u8_vic, HDMI_VIDEO_TIMING_T * pst_timing);

/**
* @brief        TX端输入音频配置
* @param [in]   输入音频选择，refer to ST_MISC_TX_AUDIO_SEL
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_tx_audio_in_cfg(MISC_TX_AUDIO_SEL *pst_misc_tx_audio_sel);

/**
* @brief        数字音频输出配置
* @param [in]   数字音频配置选项，refer to ST_MISC_DIGITAL_AUDIO_CFG
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_digital_audio_out_cfg(MISC_DIGITAL_AUDIO_CFG *pst_misc_digital_audio_cfg);

/**
* @brief        数字音频MCLK分频配置
* @param [in]   输入音频选择，refer to ST_MISC_TX_AUDIO_SEL
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_digital_mclk_cfg(MISC_MCLK_CFG_T *pst_misc_mclk_cfg);

/**
* @brief        VIPSS端 Field Det功能配置
* @param [in]   pst_fieldet_cfg：Field功能配置结构体指针，详见MISC_FIELDET_CFG_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_field_cfg(MISC_FIELDET_CFG_T *pst_fieldet_cfg);

/**
* @brief        内部中断输出配置
* @param [in]   pst_int_cfg：中断配置结构体参数指针，详见MISC_INT_CFG_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_int_cfg(MISC_INT_CFG_T *pst_int_cfg);

/**
* @brief        内部中断状态查询
* @param [in]   u8_int_src:中断源,详见MISC_INTSRC_E枚举
* @return       false-无中断，MS_TRUEE-中断产生
* @note
*/
BOOL api_misc_int_status_get(UINT8 u8_int_src);

/**
* @brief        内部中断状态清除
* @param [in]   u8_int_src:中断源,详见MISC_INTSRC_E枚举
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_int_status_clr(UINT8 u8_int_src);

/**
* @brief        VB状态查询
* @param [in]   u8_vb_src:VBPATH选择,详见MISC_VBPATH_E枚举
* @return       false-VB状态0，MS_TRUE-VB状态1
* @note
*/
BOOL api_misc_vb_status_get(UINT8 u8_vb_src);

/**
* @brief        RXPHY P/N转换
* @param [in]   pst_rxphy_pn_swap：RXPHY 通道内PN swap结构体指针，详见MISC_RXPHY_PN_SWAP_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rxphy_pn_swap(MISC_RXPHY_PN_SWAP_T *pst_rxphy_pn_swap);

/**
* @brief        RXPHY CHAN转换
* @param [in]   pst_rxphy_chan_swap：RXPHY 通道间swap结构体指针，详见MISC_RXPHY_CHAN_SWAP_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rxphy_chan_swap(MISC_RXPHY_CHAN_SWAP_T *pst_rxphy_chan_swap);

/**
* @brief        RX输出极性配置
* @param [in]   pst_rx_polarity：HDMIRX 模块输出极性配置参数结构体指针
*               related para: pst_rx_polarity->u8_chan: 选择HDMIRX模块，取值范围:MISC_VIDEO_MODULE_RX1/2/3/4
*                             pst_vdstmg_polarity->u8_hsync_flip_en:HSYNC 翻转使能
*                             pst_vdstmg_polarity->u8_vsync_flip_en:VSYNC 翻转使能
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rx_polarity_cfg(MISC_POLARITY_T *pst_rx_polarity);

/**
* @brief        VDSTMG输出极性配置
* @param [in]   pst_vdstmg_polarity:VDSTMG 模块输出极性配置参数结构体指针
*               related para: pst_vdstmg_polarity->u8_chan: 选择VDSTMG模块，取值范围:MISC_VIDEO_MODULE_TIMGEN1/2/3/4
*                             pst_vdstmg_polarity->u8_hsync_flip_en:HSYNC 翻转使能
*                             pst_vdstmg_polarity->u8_vsync_flip_en:VSYNC 翻转使能
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_vdstmg_polarity_cfg(MISC_POLARITY_T *pst_vdstmg_polarity);

/**
* @brief      VIPSS端MUX选择
* @param [in] pst_video_mux：VIPSS端MUX通路选择结构体指针
*             related para: pst_video_mux->u8_dst_module:取值范围:MISC_VIDEO_MODULE_VIPSS1/2/3/4
*                           pst_video_mux->u8_src_module:视频输入源选择，取值范围:MISC_VIDEO_MODULE_RX1、2、3、4或MISC_VIDEO_MODULE_RXPATTERN
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rx_mux_cfg(MISC_VIDEO_MUX_T *pst_video_mux);

/**
* @brief      TX端MUX选择
* @param [in] pst_video_mux：TX端音视频MUX通路选择，详见MISC_TX_MUX_T结构体描述
*             related para: pst_video_mux->u8_tx_module: TX模块选择，取值范围:MISC_VIDEO_MODULE_TX1/2/3/4
*                           pst_video_mux->u8_video_mux: TX VIDEO MUX选择，取值范围:MISC_VIDEO_MODULE_RX1/2/3/4 或 MISC_VIDEO_MODULE_MIXER1/2/3/4
*                           pst_video_mux->u8_audio_mux: TX AUDIO MUX选择，取值范围:MISC_VIDEO_MODULE_RX1/2/3/4 或 MISC_VIDEO_MODULE_I2SPDIF1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_tx_mux_cfg(MISC_TX_MUX_T *pst_video_mux);

/**
* @brief      VIPSS内部PATH选择
* @param [in] u8_vipss_idx：选择VIPSS模块，取值范围:MISC_VIDEO_MODULE_VIPSS1/2/3/4
*             u8_vipss_type：VIPSS MUX通路选择，详见MISC_VIPSS_MUXTYPE_E枚举
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_vipss_mux_cfg(UINT8 u8_vipss_idx, UINT8 u8_vipss_type);

/**
* @brief      osd输出通路选择
* @param [in] pst_misc_osd_mux：OSD输出端口选择结构体指针，详见MISC_OSD_MUX_T结构体描述
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_osd_timing_mux_cfg(MISC_OSD_MUX_T *pst_misc_osd_mux);

/**
* @brief      VIPSS 像素时钟分频输出
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para: pst_module_clk->u8_dst_module：选择VIPSS模块，取值范围:MISC_VIDEO_MODULE_VIPSS1/2/3/4
*                           pst_module_clk->u8_clkdiv：取值范围:MISC_CLKDIV_1~MISC_CLKDIV_16
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_vipss_clkout_cfg(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      timgen输入sync选择
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para: pst_module_clk->u8_dst_module：选择TIMGEN模块，取值范围:MISC_VIDEO_MODULE_TIMGEN1/2/3/4
*                           pst_module_clk->u8_src_clk：时钟源，取值范围:MISC_CLKSRC_RX1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timgen_syncin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      PLLV输入时钟选择
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para:pst_module_clk->u8_dst_module：选择PLLV模块，取值范围:MISC_VIDEO_MODULE_PLLV1/2/3/4
*                          pst_module_clk->u8_src_clk：时钟源，取值范围:MISC_CLKSRC_XTAL 或 MISC_CLKSRC_RX1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_pllv_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      TIMGEN输入时钟选择
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para:pst_module_clk->u8_dst_module：选择TIMGEN模块，取值范围:MISC_VIDEO_MODULE_TIMGEN1/2/3/4
*                          pst_module_clk->u8_src_clk：时钟源，取值范围:MISC_CLKSRC_PLLV1/2/3/4 或 MISC_CLKSRC_RX1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timgen_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      SU输入时序选择
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para:pst_module_clk->u8_dst_module：选择SU模块，取值范围:MISC_VIDEO_MODULE_VOPASS1SU0~MISC_VIDEO_MODULE_VOPASS4SU1
*                          pst_module_clk->u8_src_clk：时钟源，取值范围:MISC_CLKSRC_VDSTMG1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_su_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      Mixer输入时序选择
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para:pst_module_clk->u8_dst_module：选择MIXER模块，取值范围:MISC_VIDEO_MODULE_MIX1/2/3/4
*                          pst_module_clk->u8_src_clk：时钟源，取值范围:MISC_CLKSRC_VDSTMG1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_mixer_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      BYPASS下TXPLL时钟选择
* @param [in] pst_module_clk：时钟配置参数结构体指针
*             related para:pst_module_clk->u8_dst_module：选择TX模块，取值范围:MISC_VIDEO_MODULE_TX1/2/3/4
*                          pst_module_clk->u8_src_clk：时钟源，取值范围:MISC_CLKSRC_PIXELCLK 或 MISC_CLKSRC_TMDSCLK
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_bypass_txpll_clksel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      HDMIRX端Pattern配置
* @param [in] pst_pattern：pattern配置参数指针，详见MISC_RXPATTERN_T结构体描述
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rxpattern_cfg(MISC_RXPATTERN_T *pst_pattern);

/**
* @brief      Freqmeter功能配置
* @param [in] pst_test_freq：freq meter参数结构体指针，详见MISC_TEST_FREQ_T结构体描述
* @return     false：失败
*             MS_TRUE：成功
* @note
*/
ERR_TYPE_E api_misc_freqmeter_cfg(MISC_TEST_FREQ_T *pst_test_freq);

/**
* @brief      获取测量频率值
* @param [in] VOID
* @return     频率值，单位：HZ
*
* @note
*/
UINT32 api_misc_freq_get(VOID);

/***************************************************************
*  Function name: api_get_sdk_version
*  Description:   获取SDK版本
*  Input parameters:
*
*  Output parameters:
*  Returned value: version string
***************************************************************/
char* api_get_sdk_version(VOID);

#endif // _MISC_H__

/**
 * @}
 */

