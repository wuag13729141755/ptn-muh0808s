/**
* @file  api_misc.h
* @brief TOP MISC���������
*/

/**
 * @defgroup API_MISC misc layer api
 *
 * @brief TOP MISC����Ҫ���ͼ�������ͨ·�������
 * @{
 */

#ifndef __API_MISC__
#define __API_MISC__

#include "ms_typedef.h"
#include "ms_hdmi.h"

/** ��Ƶͨ·HDMI_PORTö�� */
typedef enum _MISC_HDMI_E
{
    MISC_CHAN_HDMI_1 = 0,
    MISC_CHAN_HDMI_2,
    MISC_CHAN_HDMI_3,
    MISC_CHAN_HDMI_4,

    MISC_CHAN_BUTT,
}MISC_HDMI_E;

/** ��Ƶͨ·ǰ��PATTERNģ��ö�� */
typedef enum _MISC_RXPATTERN_E
{
    MISC_RXPATTERN_1 = 0,
    MISC_RXPATTERN_2,
    MISC_RXPATTERN_3,
    MISC_RXPATTERN_4,

    MISC_RXPATTERN_BUTT,
}MISC_RXPATTERN_E;

/** MISC V(I/O)PSS�����������������Ϊ�ĸ�V(I/O)PSS */
typedef enum _MISC_VPSS_E
{
    MISC_VPSS_1 = 0,
    MISC_VPSS_2,
    MISC_VPSS_3,
    MISC_VPSS_4,

    MISC_VPSS_BUTT,
}MISC_VPSS_E;

/** VIPSS��ͨ·ö�٣�VIPSS1֧��10��ͨ·����VIPSS2~4֧��7��ͨ·���� */
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

/** Freq meter���ܲ���ʱ��Դö�� */
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

/** MISC���ģ��ö�� */
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

/** MISC��ʱ�ӷ�Ƶϵ��ö�� */
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

/** MISC��Repeatϵ��ö�� */
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

/** ʱ��Դö��*/
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
    MISC_CLKSRC_REFTX1, /**< OSD���REF TX����ʱ��*/
    MISC_CLKSRC_REFTX2, /**< OSD���REF TX����ʱ��*/
    MISC_CLKSRC_REFTX3, /**< OSD���REF TX����ʱ��*/
    MISC_CLKSRC_REFTX4, /**< OSD���REF TX����ʱ��*/

    MISC_CLKSRC_BUTT,
}MISC_CLKSRC_E;

/** �ж�Դö��*/
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
    /*..�����ж�Դ�˴����...*/

    MISC_INTSRC_BUTT,
}MISC_INTSRC_E;

/** VBPATHö�� */
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

/** RXPHYͨ��swap��ʽö�� */
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

/** RXPATTERN TIMINGѡ�� */
typedef enum _MISC_RXPATTERN_TIMING_E
{
    MISC_RXPATTERN_480P = 0,
    MISC_RXPATTERN_720P,
    MISC_RXPATTERN_1080P,
    MISC_RXPATTERN_4K,

    MISC_RXPATTERN_TIMING_BUTT,
}MISC_RXPATTERN_TIMING_E;

/** RXPATTERN TYPEö�� */
typedef enum _MISC_RXPATTERN_MODE_E
{
    MISC_RXPATTERN_MODE_PURE_BLACK,             /**< ����ɫͼ             */
    MISC_RXPATTERN_MODE_PURE_BLUE,              /**< ����ɫͼ             */
    MISC_RXPATTERN_MODE_PURE_GREEN,             /**< ����ɫͼ             */
    MISC_RXPATTERN_MODE_PURE_RED,               /**< ����ɫͼ             */
    MISC_RXPATTERN_MODE_PURE_WHITE,             /**< ����ɫͼ             */
    MISC_RXPATTERN_MODE_CROSS_HATCH,            /**< ��ɫ����ͼ           */
    MISC_RXPATTERN_MODE_HOR_RAMP,               /**< ˮƽ�仯�Ļҽ�ͼ      */
    MISC_RXPATTERN_MODE_VER_RAMP,               /**< ��ֱ�仯�Ļҽ�ͼ      */
    MISC_RXPATTERN_MODE_COLOR_BAR,              /**< ��ɫ��               */
    MISC_RXPATTERN_MODE_HOR_GRAY_SCALE,         /**< ˮƽ���λҽ�ͼ        */
    MISC_RXPATTERN_MODE_VER_GRAY_SCALE,         /**< ��ֱ���λҽ�ͼ        */
    MISC_RXPATTERN_MODE_2ND_HOR_GARY_SCALE,     /**< ˮƽ˫���������ҽ�ͼ */
    MISC_RXPATTERN_MODE_PRIMARY_COLOR,          /**< ԭɫͼ                */
    MISC_RXPATTERN_MODE_INIERLACE_BLACK,        /**< ���к�ɫͼ            */
    MISC_RXPATTERN_MODE_INTERLACE_RED,          /**< ���к�ɫͼ            */
    MISC_RXPATTERN_MODE_INTERLACE_GREEN,        /**< ������ɫͼ            */
    MISC_RXPATTERN_MODE_INTERLACE_BLUE,         /**< ������ɫͼ            */

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
    UINT8 u8_audio_src; /**< TX ��Ƶ��Դ��refer to ENUM_MISC_TX_AUDIO_SEL*/
    UINT8 U8_tx_dst;    /**< ѡ��������·TX,refer to MISC_VIDEO_MODULE_E,����ѡMISC_VIDEO_MODULE_TX1/2/3/4*/
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
    BOOL  b_enable;                 /**< ������Ƶ���ʹ�ܿ���*/
    UINT8 u8_audio_src;             /**< TX ������Ƶ��Դ��refer to ENUM_MISC_TX_AUDIO_SEL������ѡRX1_AUDIO/RX2_AUDIO/RX3_AUDIO/RX4_AUDIO*/
    UINT8 u8_digital_audio_dst;     /**< ѡ��������·������Ƶ��refer to ENUM_MISC_DIGITAL_AUDIO_DST_SEL*/
}MISC_DIGITAL_AUDIO_CFG;

/**
 * for MS1826, I2S1 ~ I2S4 and SPDIF1 ~ SPDIF4 are all used;
 * for MS1826A and MS1826B,Only I2S3 and SPDIF3 is used.
*/
typedef struct _MISC_MCLK_CFG_T
{
    UINT8 u8_digital_audio_dst;  /**< ����I2Sͨ·��refer to ENUM_MISC_DIGITAL_AUDIO_DST_SEL,ֻ��ѡDIG_AUDIO_I2S1/2/3/4*/
    UINT8 u8_audio_src;          /**< I2S��Ƶ��Դ��refer to ENUM_MISC_TX_AUDIO_SEL������ѡRX1_AUDIO/RX2_AUDIO/RX3_AUDIO/RX4_AUDIO*/
    UINT8 u8_mclk_en;            /**< MCLK���ʹ��*/
    UINT8 u8_mclk_div;           /**< �����Ƶϵ�������MISC_CLKDIV_E������ѡMISC_CLKDIV_1/2/4/8 */
    BOOL  b_mclk_from_crystal;   /**< mclk src is from rx or crystal */
    UINT32 u32_mclk_freq;        /**< mclk frequency,only used whwen mclk src is from crystal */
}MISC_MCLK_CFG_T;

/** RX PHY P/Nת������*/
typedef struct _MISC_RXPHY_PN_SWAP_T
{
    UINT8 u8_phy_sel;    /**< PHYѡ�����MISC_VIDEO_MODULE_E������ѡMISC_VIDEO_MODULE_RX1/2/3/4*/
    BOOL b_data0_swap_en;/**< DATA0 P/N ת��ʹ��*/
    BOOL b_data1_swap_en;/**< DATA1 P/N ת��ʹ��*/
    BOOL b_data2_swap_en;/**< DATA2 P/N ת��ʹ��*/
}MISC_RXPHY_PN_SWAP_T;

/** RX PHY CHANת������*/
typedef struct _MISC_RXPHY_CHAN_SWAP_T
{
    UINT8 u8_phy_sel;    /**< PHYѡ�����MISC_VIDEO_MODULE_E������ѡMISC_VIDEO_MODULE_RX1/2/3/4*/
    UINT8 u8_swap_mode;  /**< ת��ѡ�񣬸����ⲿ˳��ѡ���Ӧö��ֵ���ڲ�תΪ012˳�����MISC_RXPHY_CHAN_SWAP_E*/
}MISC_RXPHY_CHAN_SWAP_T;

/** оƬ��Ϣ*/
typedef struct _MISC_CHIPID_INFO_T
{
    UINT32 u32_chid_id;    /**< оƬID��Chip ID : 0x6A82A8*/
}MISC_CHIP_INFO_T;

/** PIXEL REPEAT����*/
typedef struct _MISC_REPEAT_T
{
    UINT8 u8_module_sel;   /**< TX��ģ��ѡ�����MISC_PAD_MODE_E��MISC_VIDEO_MODULE_TX1/2/3/4/MIPI*/
    UINT8 u8_repeat_num;   /**< repeat num�����MISC_REPEAT_E*/
}MISC_REPEAT_CFG_T;

/** Field Det����*/
typedef struct _MISC_FIELDET_CFG_T
{
    UINT8 u8_module_sel;   /**< VIPSSģ��ѡ�����MISC_PAD_MODE_E��MISC_VIDEO_MODULE_VIPSS1/2/3/4*/
    BOOL b_fieldet_en;     /**< Field̽��ʹ�� */
}MISC_FIELDET_CFG_T;

/** �ж��������*/
typedef struct _MISC_INT_CFG_T
{
    UINT8 u8_int_src;     /**< �ж�Դѡ�����MISC_INTSRC_E*/
    BOOL b_src_to_pad;    /**< �ж������PAD */
    BOOL b_pad_en;        /**< �ж�pad���ʹ�� */
    BOOL b_sync_low;      /**< PAD�͵�ƽ��Чʹ�� */
}MISC_INT_CFG_T;

/** RX PHY SWAP����*/
typedef struct _MISC_RXPHY_SWAP_T
{
    MISC_RXPHY_PN_SWAP_T st_rxphy_pn_swap;      /**< RXPHYͨ����PN swap�ṹ�� */
    MISC_RXPHY_CHAN_SWAP_T st_rxphy_chan_swap;  /**< RXPHYͨ���� swap�ṹ�� */
}MISC_RXPHY_SWAP_T;

/** HDMIRX������������� */
typedef struct _MISC_RX_POLARITY_T
{
    UINT8 u8_chan;          /** ͨ��ѡ��������MISC_VIDEO_MODULE_E */
    UINT8 u8_hsync_flip_en; /** HSYNC���Է�תʹ�� */
    UINT8 u8_vsync_flip_en; /** VSYNC���Է�תʹ�� */
}MISC_POLARITY_T;

/** OSD���ͨ·ѡ�� */
typedef struct _MISC_OSD_MUX_T
{
    BOOL b_enable;        /**< ͨ·�Ƿ�ѡ��ʹ�� */
    UINT8 u8_osd_module;  /**< OSDģ��ѡ�����MISC_VIDEO_MODULE_E��MISC_VIDEO_MODULE_OSD1/2/3/4 */
    UINT8 u8_tx_module;   /**< TXģ��ѡ�����MISC_VIDEO_MODULE_E��MISC_VIDEO_MODULE_TX1/2/3/4 */
}MISC_OSD_MUX_T;

/** TX���ͨ·ѡ�� */
typedef struct _MISC_TX_MUX_T
{
    UINT8 u8_tx_module;  /**< TXģ��ѡ�����MISC_VIDEO_MODULE_E��MISC_VIDEO_MODULE_TX1/2/3/4 */
    UINT8 u8_video_mux;  /**< TX VIDEO MUXѡ�����MISC_VIDEO_MODULE_E��MISC_VIDEO_MODULE_RX1/2/3/4 �� MISC_VIDEO_MODULE_MIXER1/2/3/4 */
}MISC_TX_MUX_T;

/** VIPSS����MUXѡ�� */
typedef struct _MISC_VIDEO_MUX_T
{
    UINT8 u8_dst_module;    /**< �����õ�ģ��ѡ�����MISC_VIDEO_MODULE_E */
    UINT8 u8_src_module;    /**< ģ�����ƵԴѡ�����MISC_VIDEO_MODULE_E */
}MISC_VIDEO_MUX_T;

/** ģ��ʱ��Դѡ��*/
typedef struct _MISC_MODULE_CLK_T
{
    UINT8 u8_dst_module;    /**< ѡ������õ�ģ�飬���MISC_VIDEO_MODULE_E*/
    UINT8 u8_src_clk;       /**< ѡ��ʱ��Դ�����MISC_CLKSRC_E*/
    UINT8 u8_clkdiv;        /**< ʱ�ӷ�Ƶϵ�������MISC_CLKDIV_E��Ĭ�ϲ���Ƶ*/
}MISC_MODULE_CLK_T;

/** ģ��ʹ��*/
typedef struct _MISC_MODULE_ENABLE_T
{
    UINT8 u8_module;    /**< ѡ��ģ�飬���MISC_VIDEO_MODULE_E*/
    BOOL b_enable;      /**< TOP��ģ��ʹ�ܣ�����rstb&clkλ*/
}MISC_MODULE_ENABLE_T;

/** RXPATTERN���ò��� */
typedef struct _MISC_RXPATTERN_T
{
    UINT8 u8_pattern_idx;/**< PATTERN���������MISC_RXPATTERN_E*/
    BOOL b_enable;      /**< PATTERNʹ��*/
    UINT8 u8_timing;    /**< ���Timingѡ�����MISC_RXPATTERN_TIMING_E*/
    UINT8 u8_pattern;   /**< ���Patternѡ�����MISC_RXPATTERN_MODE_E*/
}MISC_RXPATTERN_T;

/** ʱ��Ƶ�ʲ���*/
typedef struct _MISC_TEST_FREQ_T
{
    BOOL b_freq_en;         /**< FreqMeterʹ��*/
    UINT8 u8_clk_sel;       /**< ѡ�����ʱ��Դ�����MISC_TEST_CLOCK_E */
}MISC_TEST_FREQ_T;

typedef struct _MISC_TIMING_T
{
    UINT16           u16_vic;      /**< VIDEO TIMIMG������Ϣ�����MS1826_VIDEOFORMAT_E*/
    HDMI_VIDEO_TIMING_T   st_timing;
}MISC_TIMING_T;

typedef enum _ENUM_MISC_CHIP_ADDR_SEL_E
{
    CHIP_ADDR_B6 = 0xb6,
    CHIP_ADDR_BE = 0xbe
}ENUM_MISC_CHIP_ADDR;

/**
* @brief        ����оƬ��i2c��ַ
* @param [in]   u8_address : i2c���ʷ�ʽʱ��i2c��ַ������0xb6��0xbe,refer to ENUM_MISC_CHIP_ADDR������SASEL = 0,i2c��ַΪ0xb6��SASEL = 1,i2c��ַΪ0xbe
*
* @return       ERR_INV_PARM if u8_address is wrong\n
*               MS_ERR_OK if init successed
* @note
*/
ERR_TYPE_E api_misc_set_i2c_addr(UINT8 u8_address);

/**
* @brief        ��ȡ��ǰоƬ��i2c��ַ
* @return       UINT8 оƬ��ַ
*
*/
UINT8 api_misc_get_i2c_addr(VOID);

/**
* @brief        оƬ��Ϣ��ȡ
* @param [in]   pst_chip_info��оƬ��Ϣ�ṹ��ָ��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_chip_info_get(MISC_CHIP_INFO_T *pst_chip_info);

/**
* @brief        VIDEO TIMING���ʼ����APP����VIDEO TIMING Table��
* @param [in]   pst_timing_table��Timing��ָ��
*               u8_timing_num��Timing����Timing����
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timing_table_init(MISC_TIMING_T* pst_timing_table, UINT16 u16_timing_num);

/**
* @brief        ����������ȡTiming��Ϣ
* @param [in]   u8_vic��VIDEO TIMIMG������Ϣ�����MS1826_VIDEOFORMAT_E
*               ptTiming��Timing���ݴ洢�ṹ��ָ��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timing_get(UINT8 u8_vic, HDMI_VIDEO_TIMING_T * pst_timing);

/**
* @brief        TX��������Ƶ����
* @param [in]   ������Ƶѡ��refer to ST_MISC_TX_AUDIO_SEL
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_tx_audio_in_cfg(MISC_TX_AUDIO_SEL *pst_misc_tx_audio_sel);

/**
* @brief        ������Ƶ�������
* @param [in]   ������Ƶ����ѡ�refer to ST_MISC_DIGITAL_AUDIO_CFG
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_digital_audio_out_cfg(MISC_DIGITAL_AUDIO_CFG *pst_misc_digital_audio_cfg);

/**
* @brief        ������ƵMCLK��Ƶ����
* @param [in]   ������Ƶѡ��refer to ST_MISC_TX_AUDIO_SEL
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_digital_mclk_cfg(MISC_MCLK_CFG_T *pst_misc_mclk_cfg);

/**
* @brief        VIPSS�� Field Det��������
* @param [in]   pst_fieldet_cfg��Field�������ýṹ��ָ�룬���MISC_FIELDET_CFG_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_field_cfg(MISC_FIELDET_CFG_T *pst_fieldet_cfg);

/**
* @brief        �ڲ��ж��������
* @param [in]   pst_int_cfg���ж����ýṹ�����ָ�룬���MISC_INT_CFG_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_int_cfg(MISC_INT_CFG_T *pst_int_cfg);

/**
* @brief        �ڲ��ж�״̬��ѯ
* @param [in]   u8_int_src:�ж�Դ,���MISC_INTSRC_Eö��
* @return       false-���жϣ�MS_TRUEE-�жϲ���
* @note
*/
BOOL api_misc_int_status_get(UINT8 u8_int_src);

/**
* @brief        �ڲ��ж�״̬���
* @param [in]   u8_int_src:�ж�Դ,���MISC_INTSRC_Eö��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_int_status_clr(UINT8 u8_int_src);

/**
* @brief        VB״̬��ѯ
* @param [in]   u8_vb_src:VBPATHѡ��,���MISC_VBPATH_Eö��
* @return       false-VB״̬0��MS_TRUE-VB״̬1
* @note
*/
BOOL api_misc_vb_status_get(UINT8 u8_vb_src);

/**
* @brief        RXPHY P/Nת��
* @param [in]   pst_rxphy_pn_swap��RXPHY ͨ����PN swap�ṹ��ָ�룬���MISC_RXPHY_PN_SWAP_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rxphy_pn_swap(MISC_RXPHY_PN_SWAP_T *pst_rxphy_pn_swap);

/**
* @brief        RXPHY CHANת��
* @param [in]   pst_rxphy_chan_swap��RXPHY ͨ����swap�ṹ��ָ�룬���MISC_RXPHY_CHAN_SWAP_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rxphy_chan_swap(MISC_RXPHY_CHAN_SWAP_T *pst_rxphy_chan_swap);

/**
* @brief        RX�����������
* @param [in]   pst_rx_polarity��HDMIRX ģ������������ò����ṹ��ָ��
*               related para: pst_rx_polarity->u8_chan: ѡ��HDMIRXģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_RX1/2/3/4
*                             pst_vdstmg_polarity->u8_hsync_flip_en:HSYNC ��תʹ��
*                             pst_vdstmg_polarity->u8_vsync_flip_en:VSYNC ��תʹ��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rx_polarity_cfg(MISC_POLARITY_T *pst_rx_polarity);

/**
* @brief        VDSTMG�����������
* @param [in]   pst_vdstmg_polarity:VDSTMG ģ������������ò����ṹ��ָ��
*               related para: pst_vdstmg_polarity->u8_chan: ѡ��VDSTMGģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_TIMGEN1/2/3/4
*                             pst_vdstmg_polarity->u8_hsync_flip_en:HSYNC ��תʹ��
*                             pst_vdstmg_polarity->u8_vsync_flip_en:VSYNC ��תʹ��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_vdstmg_polarity_cfg(MISC_POLARITY_T *pst_vdstmg_polarity);

/**
* @brief      VIPSS��MUXѡ��
* @param [in] pst_video_mux��VIPSS��MUXͨ·ѡ��ṹ��ָ��
*             related para: pst_video_mux->u8_dst_module:ȡֵ��Χ:MISC_VIDEO_MODULE_VIPSS1/2/3/4
*                           pst_video_mux->u8_src_module:��Ƶ����Դѡ��ȡֵ��Χ:MISC_VIDEO_MODULE_RX1��2��3��4��MISC_VIDEO_MODULE_RXPATTERN
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rx_mux_cfg(MISC_VIDEO_MUX_T *pst_video_mux);

/**
* @brief      TX��MUXѡ��
* @param [in] pst_video_mux��TX������ƵMUXͨ·ѡ�����MISC_TX_MUX_T�ṹ������
*             related para: pst_video_mux->u8_tx_module: TXģ��ѡ��ȡֵ��Χ:MISC_VIDEO_MODULE_TX1/2/3/4
*                           pst_video_mux->u8_video_mux: TX VIDEO MUXѡ��ȡֵ��Χ:MISC_VIDEO_MODULE_RX1/2/3/4 �� MISC_VIDEO_MODULE_MIXER1/2/3/4
*                           pst_video_mux->u8_audio_mux: TX AUDIO MUXѡ��ȡֵ��Χ:MISC_VIDEO_MODULE_RX1/2/3/4 �� MISC_VIDEO_MODULE_I2SPDIF1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_tx_mux_cfg(MISC_TX_MUX_T *pst_video_mux);

/**
* @brief      VIPSS�ڲ�PATHѡ��
* @param [in] u8_vipss_idx��ѡ��VIPSSģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_VIPSS1/2/3/4
*             u8_vipss_type��VIPSS MUXͨ·ѡ�����MISC_VIPSS_MUXTYPE_Eö��
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_vipss_mux_cfg(UINT8 u8_vipss_idx, UINT8 u8_vipss_type);

/**
* @brief      osd���ͨ·ѡ��
* @param [in] pst_misc_osd_mux��OSD����˿�ѡ��ṹ��ָ�룬���MISC_OSD_MUX_T�ṹ������
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_osd_timing_mux_cfg(MISC_OSD_MUX_T *pst_misc_osd_mux);

/**
* @brief      VIPSS ����ʱ�ӷ�Ƶ���
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para: pst_module_clk->u8_dst_module��ѡ��VIPSSģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_VIPSS1/2/3/4
*                           pst_module_clk->u8_clkdiv��ȡֵ��Χ:MISC_CLKDIV_1~MISC_CLKDIV_16
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_vipss_clkout_cfg(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      timgen����syncѡ��
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para: pst_module_clk->u8_dst_module��ѡ��TIMGENģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_TIMGEN1/2/3/4
*                           pst_module_clk->u8_src_clk��ʱ��Դ��ȡֵ��Χ:MISC_CLKSRC_RX1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timgen_syncin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      PLLV����ʱ��ѡ��
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para:pst_module_clk->u8_dst_module��ѡ��PLLVģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_PLLV1/2/3/4
*                          pst_module_clk->u8_src_clk��ʱ��Դ��ȡֵ��Χ:MISC_CLKSRC_XTAL �� MISC_CLKSRC_RX1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_pllv_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      TIMGEN����ʱ��ѡ��
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para:pst_module_clk->u8_dst_module��ѡ��TIMGENģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_TIMGEN1/2/3/4
*                          pst_module_clk->u8_src_clk��ʱ��Դ��ȡֵ��Χ:MISC_CLKSRC_PLLV1/2/3/4 �� MISC_CLKSRC_RX1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_timgen_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      SU����ʱ��ѡ��
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para:pst_module_clk->u8_dst_module��ѡ��SUģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_VOPASS1SU0~MISC_VIDEO_MODULE_VOPASS4SU1
*                          pst_module_clk->u8_src_clk��ʱ��Դ��ȡֵ��Χ:MISC_CLKSRC_VDSTMG1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_su_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      Mixer����ʱ��ѡ��
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para:pst_module_clk->u8_dst_module��ѡ��MIXERģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_MIX1/2/3/4
*                          pst_module_clk->u8_src_clk��ʱ��Դ��ȡֵ��Χ:MISC_CLKSRC_VDSTMG1/2/3/4
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_mixer_clkin_sel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      BYPASS��TXPLLʱ��ѡ��
* @param [in] pst_module_clk��ʱ�����ò����ṹ��ָ��
*             related para:pst_module_clk->u8_dst_module��ѡ��TXģ�飬ȡֵ��Χ:MISC_VIDEO_MODULE_TX1/2/3/4
*                          pst_module_clk->u8_src_clk��ʱ��Դ��ȡֵ��Χ:MISC_CLKSRC_PIXELCLK �� MISC_CLKSRC_TMDSCLK
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_bypass_txpll_clksel(MISC_MODULE_CLK_T *pst_module_clk);

/**
* @brief      HDMIRX��Pattern����
* @param [in] pst_pattern��pattern���ò���ָ�룬���MISC_RXPATTERN_T�ṹ������
* @return     specific error types if call failed
* @note
*/
ERR_TYPE_E api_misc_rxpattern_cfg(MISC_RXPATTERN_T *pst_pattern);

/**
* @brief      Freqmeter��������
* @param [in] pst_test_freq��freq meter�����ṹ��ָ�룬���MISC_TEST_FREQ_T�ṹ������
* @return     false��ʧ��
*             MS_TRUE���ɹ�
* @note
*/
ERR_TYPE_E api_misc_freqmeter_cfg(MISC_TEST_FREQ_T *pst_test_freq);

/**
* @brief      ��ȡ����Ƶ��ֵ
* @param [in] VOID
* @return     Ƶ��ֵ����λ��HZ
*
* @note
*/
UINT32 api_misc_freq_get(VOID);

/***************************************************************
*  Function name: api_get_sdk_version
*  Description:   ��ȡSDK�汾
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

