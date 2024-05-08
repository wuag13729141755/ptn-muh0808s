#ifndef __MS1826_APP_C
#define __MS1826_APP_C
#include "includes.h"
#if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)
#include "ms1826_app.h"
#include "ms1826_videoprocess.h"

UINT8 const g_u8_rx_idx[4]				= {HDMI_RX_A, HDMI_RX_B, HDMI_RX_C, HDMI_RX_D};
UINT8 const g_u8_csc_idx[4]				= {CSC_SEL_1, CSC_SEL_2, CSC_SEL_3, CSC_SEL_4};
UINT8 const g_u8_misc_module_rx[4]		= {MISC_VIDEO_MODULE_RX1, MISC_VIDEO_MODULE_RX2, MISC_VIDEO_MODULE_RX3, MISC_VIDEO_MODULE_RX4};
UINT8 const g_u8_4ksdn_idx[4]			= {SDN4K_1, SDN4K_2, SDN4K_3, SDN4K_4};
UINT8 const g_u8_sdn_idx[4]				= {SDN_SEL_SDN1, SDN_SEL_SDN2, SDN_SEL_SDN3, SDN_SEL_SDN4};
UINT8 const g_u8_dsc_idx[4]				= {DSC_SEL_1, DSC_SEL_2, DSC_SEL_3,	DSC_SEL_4};
UINT8 const g_u8_dma_idx[4]				= {DMA_SEL_PATH1, DMA_SEL_PATH2, DMA_SEL_PATH3,	DMA_SEL_PATH4};
UINT8 const g_u8_dma_dsc_idx[4]			= {DMA_SEL_PATH1_DSC, DMA_SEL_PATH2_DSC, DMA_SEL_PATH3_DSC,	DMA_SEL_PATH4_DSC};
UINT8 const g_u8_misc_module_vipss[4]	= {MISC_VIDEO_MODULE_VIPSS1, MISC_VIDEO_MODULE_VIPSS2, MISC_VIDEO_MODULE_VIPSS3, MISC_VIDEO_MODULE_VIPSS4};
UINT8 const g_u8_misc_module_vopss[4]	= {MISC_VIDEO_MODULE_VOPSS1, MISC_VIDEO_MODULE_VOPSS2, MISC_VIDEO_MODULE_VOPSS3, MISC_VIDEO_MODULE_VOPSS4};
UINT8 const g_u8_su_idx[4]				= {SU_SEL_SU2, SU_SEL_SU4, SU_SEL_SU6, SU_SEL_SU8};
UINT8 const g_u8_su_dsc_idx[4]			= {SU_SEL_SU1, SU_SEL_SU3, SU_SEL_SU5, SU_SEL_SU7};
UINT8 const g_u8_mixer_idx[4]			= {VIDEO_MIXER_1, VIDEO_MIXER_2, VIDEO_MIXER_3,VIDEO_MIXER_4};
UINT8 const g_u8_mixer_layer_src[4]		= {VOPSS1_SU, VOPSS2_SU, VOPSS3_SU,	VOPSS4_SU};
UINT8 const g_u8_mixer_layer_dsc_src[4]	= {VOPSS1_DSC_SU, VOPSS2_DSC_SU, VOPSS3_DSC_SU, VOPSS4_DSC_SU};
UINT8 const g_u8_misc_mixer[4]			= {MISC_VIDEO_MODULE_MIX1, MISC_VIDEO_MODULE_MIX2, MISC_VIDEO_MODULE_MIX3, MISC_VIDEO_MODULE_MIX4};
UINT8 const g_u8_misc_timgen[4]			= {MISC_VIDEO_MODULE_TIMGEN1, MISC_VIDEO_MODULE_TIMGEN2, MISC_VIDEO_MODULE_TIMGEN3, MISC_VIDEO_MODULE_TIMGEN4};
UINT8 const g_u8_misc_su[4]				= {MISC_VIDEO_MODULE_VOPASS1SU1, MISC_VIDEO_MODULE_VOPASS2SU1, MISC_VIDEO_MODULE_VOPASS3SU1, MISC_VIDEO_MODULE_VOPASS4SU1};
UINT8 const g_u8_misc_dsc_su[4]			= {MISC_VIDEO_MODULE_VOPASS1SU0, MISC_VIDEO_MODULE_VOPASS2SU0, MISC_VIDEO_MODULE_VOPASS3SU0, MISC_VIDEO_MODULE_VOPASS4SU0};
UINT8 const g_u8_misc_module_tx[4]		= {MISC_VIDEO_MODULE_TX1, MISC_VIDEO_MODULE_TX2, MISC_VIDEO_MODULE_TX3, MISC_VIDEO_MODULE_TX4};
UINT8 const g_u8_split_idx[4]			= {SPLIT_1,	SPLIT_2, SPLIT_3, SPLIT_4};
UINT8 const g_u8_ss_ch_idx[4]			= {CHANNEL1_without_dsc, CHANNEL2_without_dsc, CHANNEL3_without_dsc, CHANNEL4_without_dsc};
UINT8 const g_u8_ss_ch_dsc_idx[4]		= {CHANNEL1_with_dsc, CHANNEL2_with_dsc, CHANNEL3_with_dsc,	CHANNEL4_with_dsc};
UINT8 const g_u8_vb_path[4]				= {MISC_VBPATH_VOPASS1SU1, MISC_VBPATH_VOPASS2SU1, MISC_VBPATH_VOPASS3SU1, MISC_VBPATH_VOPASS4SU1};
UINT8 const g_u8_vb_dsc_path[4]			= {MISC_VBPATH_VOPASS1SU0, MISC_VBPATH_VOPASS2SU0, MISC_VBPATH_VOPASS3SU0, MISC_VBPATH_VOPASS4SU0};
UINT8 const g_u8_vb_rx[4]				= {MISC_VBPATH_RX1,	MISC_VBPATH_RX2, MISC_VBPATH_RX3, MISC_VBPATH_RX4};
UINT8 const g_u8_vb_vdstmg[4]			= {MISC_VBPATH_VDSTMG1,	MISC_VBPATH_VDSTMG2, MISC_VBPATH_VDSTMG3, MISC_VBPATH_VDSTMG4};
UINT8 const g_u8_intsrc_rx[4]			= {MISC_INTSRC_RX1VDE, MISC_INTSRC_RX2VDE, MISC_INTSRC_RX3VDE, MISC_INTSRC_RX4VDE};
UINT8 const g_u8_intsrc_rxvb[4]			= {MISC_INTSRC_RX1VB, MISC_INTSRC_RX2VB, MISC_INTSRC_RX3VB,	MISC_INTSRC_RX4VB};
UINT8 const g_u8_misc_clksrc[4]			= {MISC_CLKSRC_VDSTMG1,	MISC_CLKSRC_VDSTMG2, MISC_CLKSRC_VDSTMG3, MISC_CLKSRC_VDSTMG4};
UINT8 const g_u8_osd_idx[4]				= {OSD_1, OSD_2, OSD_3, OSD_4};
UINT8 const g_u8_misc_osd_idx[4]		= {MISC_VIDEO_MODULE_OSD1, MISC_VIDEO_MODULE_OSD2, MISC_VIDEO_MODULE_OSD3, MISC_VIDEO_MODULE_OSD4};
UINT8 const g_u8_i2s_idx[4] 			= {DIG_AUDIO_I2S1, DIG_AUDIO_I2S2, DIG_AUDIO_I2S3, DIG_AUDIO_I2S4};
UINT8 const g_u8_spdif_idx[4] 			= {DIG_AUDIO_SPDIF1, DIG_AUDIO_SPDIF2, DIG_AUDIO_SPDIF3, DIG_AUDIO_SPDIF4};
UINT8 const g_u8_i2s_audio_idx[4] 		= {I2S1_AUDIO, I2S2_AUDIO, I2S3_AUDIO, I2S4_AUDIO};
UINT8 const g_u8_spdif_audio_idx[4] 	= {SPIDF1_AUDIO, SPIDF2_AUDIO, SPIDF3_AUDIO, SPIDF4_AUDIO};
UINT8 const g_u8_i2s_audio[4] 			= {I2S1_AUDIO,I2S2_AUDIO,I2S3_AUDIO,I2S4_AUDIO};
UINT8 const g_u8_spdif_audio[4] 		= {SPIDF1_AUDIO,SPIDF2_AUDIO,SPIDF3_AUDIO,SPIDF4_AUDIO};



UINT8 const g_arrOutputTable[6]	=
{
	VFMT_CEA_04_1280x720P_60HZ,
	VFMT_CEA_16_1920x1080P_60HZ,
	VFMT_VESA_234_3840X2160_30,
	VFMT_VESA_71_1024X768_60,
	VFMT_VESA_96_1280X1024_60,
	VFMT_VESA_102_1366X768_60
};

UINT8 const g_u8_HDMI_RX_PROT[4] =
{
	HDMI_RX_A,
	HDMI_RX_B,//HDMI_RX_D,
	HDMI_RX_C,
	HDMI_RX_D,//HDMI_RX_B
};

UINT8 const g_u8_HDMI_TX_PROT[4] =
{
	HDMI_TX_A,
	HDMI_TX_B,
	HDMI_TX_C,
	HDMI_TX_D
};

UINT16 const str_osd_input_channel[4][8] =
{
	96, 97, 99, 100, 17, 0, 0, 0,
	96, 97, 99, 100, 18, 0, 0, 0,
	96, 97, 99, 100, 19, 0, 0, 0,
	96, 97, 99, 100, 20, 0, 0, 0,
};

UINT16 const str_osd_input_channel_audio[4][8] =
{
	96, 97, 99, 100, 17, 0, 0, 95,
	96, 97, 99, 100, 18, 0, 0, 95,
	96, 97, 99, 100, 19, 0, 0, 95,
	96, 97, 99, 100, 20, 0, 0, 95,
};

UINT16 const str_osd_output_channel[4][5] =
{
	96, 98, 99, 100, 17,
	96, 98, 99, 100, 18,
	96, 98, 99, 100, 19,
	96, 98, 99, 100, 20,
};

char const str_osd_res[3][15] =
{
	"1280x720@60Hz ",
	"1920x1080P@60Hz",
	"3840x2160P@30Hz",
};

#if 1//(_ENABLE_CHIP_GSV2011 == _MACRO_DISABLE)
const stTimingTable_t res_ms1826table[]=
{
    //  HDST    HTOT    HBP   HFP HSYNC    HPOL    VDST     VTOT  VBP  VFP VSYNC   VPOL   VCLK   u8VIC      PCLK            interlace
    { 1920,   2080,    80,   48,   32,    Hpos ,   1200,  1235,  26,   3,    6,  Vneg,     60,     0, (2080*1235)*60/10000,    PROG},//1920x1200@60Hz
    { 1920,   2200,   148,   88,   44,    Hpos ,   1080,  1125,  36,   4,    5,  Vpos,     60,    16,                 14850,   PROG},//1920x1080@60Hz
    { 1280,   1650,   220,  110,   40,    Hpos ,    720,   750,  20,   5,    5,  Vpos,     60,     4,                  7425,   PROG},//1280x720@60Hz
    { 1360,   1792,   256,   64,  112,    Hpos ,    768,   795,  18,   3,    6,  Vpos,     60,     0,  (1792*795)*60/10000,    PROG},//1360x768@60Hz
    { 1280,   1688,   248,   48,  112,    Hpos ,   1024,  1066,  38,   1,    3,  Vpos,     60,     0, (1688*1066)*60/10000,    PROG},//1280x1024@60Hz
    { 1024,   1344,   160,   24,  136,    Hneg ,    768,   806,  29,   3,    6,  Vneg,     60,     0,  (1344*806)*60/10000,    PROG},//1024x768@60Hz
    { 1600,   2160,   304,   64,  192,    Hpos ,   1200,  1250,  46,   1,    3,  Vpos,     60,     0, (2160*1250)*60/10000,    PROG},//1600x1200@60Hz
    { 1440,   1904,   232,   80,  152,    Hneg ,    900,   934,  25,   3,    6,  Vpos,     60,     0,  (1904*934)*60/10000,    PROG},//1440x900@60Hz
    { 1600,   1800,    96,   24,   80,    Hpos ,    900,  1000,  96,   1,    3,  Vpos,     60,     0, (1800*1000)*60/10000,    PROG},//1600x900@60Hz
    { 1280,   1980,   220,  440,   40,    Hpos ,    720,   750,  20,   5,    5,  Vpos,     50,    19,  (1980*750)*50/10000,    PROG},//1280x720@50Hz
    { 1920,   2640,   148,  528,   44,    Hpos ,   1080,  1125,  36,   4,    5,  Vpos,     50,    31, (2640*1125)*50/10000,    PROG},//1920x1080@50Hz
#if _ENABLE_RES_3840x2160_30
    { 3840,   4400,   296,  176,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     30,    95,                 29700,   PROG},//3840x2160@30Hz
#endif
#if _ENABLE_RES_3840x2160_60
    { 3840,   4400,   296,  176,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     60,    97,                 59400,   PROG},//3840x2160@60Hz
#endif
#if _ENABLE_RES_3840x2160_50
    { 3840,   5280,   298, 1056,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     50,    96,                 59400,   PROG},//3840x2160@50Hz
#endif
#if _ENABLE_RES_4096x2160_30
    { 4096,   4400,   128,   88,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     30,   100,                 29700,   PROG},//4096x2160@30Hz
#endif
#if _ENABLE_RES_4096x2160_60
    { 4096,   4400,   128,   88,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     60,   102,                 59400,   PROG},//4096x2160@60Hz
#endif
#if _ENABLE_RES_4096x2160_50
    { 4096,   5280,   128,  968,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     50,   101,                 59400,   PROG},//4096x2160@50Hz
#endif
};
#endif

HDMI_RX_DEV_T g_st_hdmi_rx[DEV_NUM_MS1826][4];
HDMI_VIDEO_TIMING_T	Video_HDMIRx_Timing[DEV_NUM_MS1826][4];
MISC_POLARITY_T	g_st_rx_polarity[DEV_NUM_MS1826][4];
CSC_DEV_T g_st_vipss_csc[DEV_NUM_MS1826][4];
BOOL b_RxVideoChg[DEV_NUM_MS1826][4] = {{false}};
UINT8 wdma_buf_id[DEV_NUM_MS1826] = {0};
UINT8 rdma_buf_id[DEV_NUM_MS1826] = {0};

HDMI_TX_DEV_T g_st_hdmi_tx[DEV_NUM_MS1826][4];
HDMI_VIDEO_TIMING_T	g_stVideo_HDMITx_Timing[DEV_NUM_MS1826][4];
MISC_TX_MUX_T g_st_tx_mux[DEV_NUM_MS1826][4];
CSC_DEV_T g_st_txcsc_dev[DEV_NUM_MS1826][4];
VDS_TMG_DEV_T g_st_timgen[DEV_NUM_MS1826][4];
UINT8 hdmi_tx_edid[DEV_NUM_MS1826][4][256]	= {{{0}}};
HDMI_EDID_FLAG_T g_hdmi_edid_flag[DEV_NUM_MS1826][4];
BOOL b_hpd_bak[DEV_NUM_MS1826][4] =	{{FALSE}};
DEINT_DEV_T g_st_deint_dev[DEV_NUM_MS1826];

OSD_DEV_T g_st_osd_dev[DEV_NUM_MS1826][4];
MISC_OSD_MUX_T g_st_misc_osd_mux[DEV_NUM_MS1826][4];
OSD_WINSIZE_T st_osd_win_size[DEV_NUM_MS1826];
OSD_STRING_T st_str[DEV_NUM_MS1826];
OSD_FONT_T st_font[DEV_NUM_MS1826];
char p_str[MS1826_OSD_MAX_LEN] = {{'0'}};
BOOL OSDShown[DEV_NUM_MS1826] = {false};
UINT8 OSDCount[DEV_NUM_MS1826];
BOOL g_user_osd_str_enable[DEV_NUM_MS1826][4] = {{false}};
OSD_WINSIZE_T st_user_osd_win_size[DEV_NUM_MS1826][4];
UINT8 g_user_osd_string_color[DEV_NUM_MS1826][4][3] = {{{0xFF,0x00,0x00}}};
UINT16 g_u16UserStrDispPos[DEV_NUM_MS1826][4][2] = {{{100,100}}};
BOOL g_bUserOsdStringIsAlignRight[DEV_NUM_MS1826][4] = {{false}};
BOOL g_bUserOsdStringDisplayEnable[DEV_NUM_MS1826][4] = {{false}};

SPLIT_DEV_T g_st_split_var[DEV_NUM_MS1826][4];

UINT8 display_mode[DEV_NUM_MS1826]	= {MATRIX_SWITCHER};//SPLITTER;
UINT8 display_mode_bak[DEV_NUM_MS1826] = {SPLICER};
UINT8 g_u8_output_index[DEV_NUM_MS1826][4] = {{1, 1, 1, 1}};
UINT8 Matrix_Tx_Mapping[DEV_NUM_MS1826][4] = {{0, 1, 2, 3}};
UINT8 Mapp_Rx[DEV_NUM_MS1826][4] =	{{0, 1, 2, 3}};
UINT8 Bypass_Tx_Status[DEV_NUM_MS1826][4] =	{{0, 0, 0, 0}};
UINT8 u8_Tx_audio_sel[DEV_NUM_MS1826][4] = {{0, 0, 0, 0}};
UINT8 g_u8SplitterDispMapping[DEV_NUM_MS1826][4] = {{0, 1, 2, 3}};

UINT8 g_u8TxFormat[DEV_NUM_MS1826][4] = {{MS1826_TX_HDMI, MS1826_TX_HDMI, MS1826_TX_HDMI, MS1826_TX_HDMI}};

HDMI_INFOFRAMES_PACKETS_T	gst_inf_pack[DEV_NUM_MS1826];

VIDEO_MIXER_DEV_T g_st_video_mixer_dev[DEV_NUM_MS1826][4];
VIDEO_MIXER_LAYER_SEL_T	g_st_mixer_layer_sel[DEV_NUM_MS1826];
VIDEO_MIXER_ALF_CONFIG_T g_st_mixer_alf_cfg[DEV_NUM_MS1826];
#if MS1826_VIPSS1_DEINT
VIDEO_MIXER_COLOR_T g_st_bg_color = {0x00, 0x80, 0x80};
#else
VIDEO_MIXER_COLOR_T g_st_bg_color = {0x00, 0x00, 0x00};
#endif

VEHC_DEV_T g_st_vivehc[DEV_NUM_MS1826][4];
VEHC_DEV_T g_st_vovehc[DEV_NUM_MS1826][4];

UINT16 g_u16_timer_out = 0;
UINT8 g_u8_output_index_bak[DEV_NUM_MS1826][4];

BOOL b_audio_mute[DEV_NUM_MS1826][4] = {{FALSE}};
BOOL b_digital_audio_mute[DEV_NUM_MS1826][4] = {{FALSE}};
UINT8 g_u8DigitalAudioSta[DEV_NUM_MS1826][4]={{2,2,2,2}};
UINT8 g_u8MS1826DevIndex = 0;
stMs1826FunctionEvent_t Ms1826event[DEV_NUM_MS1826];
BOOL g_bSplicerEnable[DEV_NUM_MS1826][4] = {{FALSE}};
BOOL g_bSplitterEnable[DEV_NUM_MS1826][4] = {{FALSE}};
#if ENABLE_MS1826_FRAME_LOCK
UINT8 g_u8FrameLockRxTable[DEV_NUM_MS1826][4] = {{0, 1, 2, 3}};
#endif
BOOL g_bPreTimingStable[DEV_NUM_MS1826][4] = {{FALSE}};
static bool sg_bMS1826StandbyFlag[DEV_NUM_MS1826] = {FALSE};
static bool g_bPreMS1826SStbFlag[DEV_NUM_MS1826] = {FALSE};
BOOL g_bDigitalAudioSwByHdmiTx[DEV_NUM_MS1826][4] = {{TRUE, TRUE, TRUE, TRUE}};

extern UINT8 const g_u8_bcsh_bri[4];
extern UINT8 const g_u8_bcsh_con[4];
extern UINT8 const g_u8_bcsh_sat[4];
extern UINT8 const g_u8_bcsh_hue[4];
extern UINT8 g_u8_time;
extern BOOL g_bFirstStartupFlag[DEV_NUM_MS1826][4];
extern UINT8 gu8ChipAddr[DEV_NUM_MS1826];


extern void MS1826_Reset(void);
extern void MS1826_CHipReset(u8 u8DevID);
extern void app_single_win_test(void);

extern UINT32 ms1826_hal_hdmi_rx_get_mdt_ints_ext(UINT8 idx);
extern VOID ms1826_hal_hdmi_rx_phy_power_toggle(UINT8 idx);
extern UINT8 ms1826_hal_hdmi_rx_phy_process(UINT8 idx, BOOL b_rx_tmds_clk_change, UINT16 u16_rx_tmds_clk);
extern UINT16 ms1826_hal_hdmi_rx_get_tmds_clk(UINT8 idx);
extern VOID _single_mode_frc_cfg(UINT8 u8dev);

void app_ms1826_init(UINT8 u8dev,UINT8 u8WorkMode)
{
    UINT8 i;

    wdma_buf_id[u8dev]   = 0;
    rdma_buf_id[u8dev]   = 0;

    OSDShown[u8dev]      = false;
    OSDCount[u8dev]     = 0;

    display_mode[u8dev] = u8WorkMode;//;
    display_mode_bak[u8dev] = SPLICER;
    memset(p_str,' ',MS1826_OSD_MAX_LEN);
    for(i=0;i<4;i++)
    {
        memset(&g_st_rx_polarity[u8dev][i],0,sizeof(MISC_POLARITY_T));
        api_csc_rx_instance_init(&g_st_vipss_csc[u8dev][i],i);
        memset(&g_st_timgen[u8dev][i],0,sizeof(VDS_TMG_DEV_T));
        memset(hdmi_tx_edid[u8dev][i],0,256);
        memset(&g_hdmi_edid_flag[u8dev][i],0,sizeof(HDMI_EDID_FLAG_T));

        b_RxVideoChg[u8dev][i]          = false;
        b_hpd_bak[u8dev][i]             = false;
        g_u8_output_index[u8dev][i]     = 1;
        Matrix_Tx_Mapping[u8dev][i]     = i;
        Bypass_Tx_Status[u8dev][i]      = 0;
        Mapp_Rx[u8dev][i]      = i;
        Bypass_Tx_Status[u8dev][i]      = 0;
        u8_Tx_audio_sel[u8dev][i]       = 0;
        g_u8TxFormat[u8dev][i]          = MS1826_TX_HDMI;
        b_audio_mute[u8dev][i]          = false;
        b_digital_audio_mute[u8dev][i]  = false;
        g_user_osd_str_enable[u8dev][i] = false;
        g_user_osd_string_color[u8dev][i][0]    = 0xFF;
        g_user_osd_string_color[u8dev][i][1]    = 0x00;
        g_user_osd_string_color[u8dev][i][2]    = 0x00;
        g_u16UserStrDispPos[u8dev][i][0]= 100;
        g_u16UserStrDispPos[u8dev][i][1]= 100;
        g_bUserOsdStringIsAlignRight[u8dev][i]  = false;
        g_bUserOsdStringDisplayEnable[u8dev][i] = false;
        g_bSplicerEnable[u8dev][i]      = false;
        g_bSplitterEnable[u8dev][i]     = false;
    #if ENABLE_MS1826_FRAME_LOCK
        g_u8FrameLockRxTable[u8dev][i] = i;
    #endif
        g_bPreTimingStable[u8dev][i] = false;
        g_bDigitalAudioSwByHdmiTx[u8dev][i] = MS_TRUE;
        g_u8SplitterDispMapping[u8dev][i] = i;

        Ms1826event[u8dev].bPortChangeFlag[i] = false;
        Ms1826event[u8dev].u8OutputSel[i] = i;

        Ms1826event[u8dev].bFreezeChangeFlag[i] = false;
        Ms1826event[u8dev].u8FreezeSta[i] = 0;

        Ms1826event[u8dev].bTxResolutionChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxResolutionID[i] = 1;

        Ms1826event[u8dev].bBrightnessAdjustFlag[i] = false;
        Ms1826event[u8dev].u8Brightness[i] = 128;
        Ms1826event[u8dev].bContrastAdjustFlag[i] = false;
        Ms1826event[u8dev].u8Contrast[i] = 128;
        Ms1826event[u8dev].bSaturationAdjustFlag[i] = false;
        Ms1826event[u8dev].u8Saturation[i] = 128;
        Ms1826event[u8dev].bHueAdjustFlag[i] = false;
        Ms1826event[u8dev].u8Hue[i] = 0;

        Ms1826event[u8dev].bRxEdidChangeFlag[i] = false;

        Ms1826event[u8dev].bFreezeSwitchFlag[i] = false;

        Ms1826event[u8dev].bTxCfgChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxFormat[i] = 0;
        Ms1826event[u8dev].u8TxColorSpace[i] = 0;

        Ms1826event[u8dev].bTxHdcp14ChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxHdcp14Sta[i] = 0;
        Ms1826event[u8dev].bReadTxEdidFlag[i] = false;

        Ms1826event[u8dev].bTxDigitalAudioChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxDigitalAudioSta[i] = 0;
        Ms1826event[u8dev].u8TxDigitalAudioSource[i] = i;

        Ms1826event[u8dev].bTxAudioSourceChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxAudioSourceSta[i] = i;

        Ms1826event[u8dev].bTxPatternChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxPatternID[i] = 0;
        Ms1826event[u8dev].u8TxPatternEnable[i] = 0;

        Ms1826event[u8dev].bColorChangeFlag[i] = false;
        Ms1826event[u8dev].u8ColorEnable[i] = 0;
        Ms1826event[u8dev].u8RedValue[i] = 128;
        Ms1826event[u8dev].u8GreenValue[i] = 128;
        Ms1826event[u8dev].u8BlueValue[i] = 128;

        Ms1826event[u8dev].bTxTimingChangeFlag[i] = false;
        Ms1826event[u8dev].bSplitChangeFlag[i] = false;
        Ms1826event[u8dev].u8SplitEnable[i] = 0;

        Ms1826event[u8dev].bBypassPortChangeFlag[i] = false;
        Ms1826event[u8dev].u8BypassOutputSel[i] = i;

        Ms1826event[u8dev].bOsdStringChangeFlag[i] = false;
        memset(Ms1826event[u8dev].u8OsdStr[i],' ',MS1826_OSD_MAX_LEN);

        Ms1826event[u8dev].bVideoMuteChangeFlag[i] = false;
        Ms1826event[u8dev].bVideoMuteSta[i] = false;

        Ms1826event[u8dev].bTxHdmiAudioMuteChangeFlag[i] = false;
        Ms1826event[u8dev].u8TxHdmiAudioMuteSta[i] = 0;

        Ms1826event[u8dev].bSplicerChangeFlag[i] = false;
        Ms1826event[u8dev].stSplicerVar[i].u16X = 0;
        Ms1826event[u8dev].stSplicerVar[i].u16Y = 0;
        Ms1826event[u8dev].stSplicerVar[i].u16W = 0;
        Ms1826event[u8dev].stSplicerVar[i].u16H = 0;
        Ms1826event[u8dev].bSplicerEnable[i] = false;
    }
    Ms1826event[u8dev].bWorkModeSwitch = false;
    Ms1826event[u8dev].u8WorkMode = false;
    Ms1826event[u8dev].u8SubMode = 0;

    Ms1826event[u8dev].u16FreezeDelayTime = 0;
    Ms1826event[u8dev].bFreezeTimeEnd = false;
    sg_bMS1826StandbyFlag[u8dev] = FALSE;
    g_bPreMS1826SStbFlag[u8dev] = FALSE;

}

UINT16 return_minof_three(UINT16 u16dat1, UINT16 u16dat2, UINT16 u16dat3)
{
	UINT16 u16min = (u16dat1 >= u16dat2) ? u16dat2 : u16dat1;

	u16min = (u16min >= u16dat3) ? u16dat3 : u16min;

	return u16min;
}

UINT16 return_com_multiple(UINT16 u16dat, UINT8 u8_data)
{
	UINT16 u16_con_mult;
	if(u16dat % (8 * u8_data) != 0)
	{
		u16_con_mult = u16dat / (8 * u8_data) * (8 * u8_data);
		return u16_con_mult;
	}
	else
		return u16dat;
}

VOID app_video_vipss_trig(UINT8 u8dev,UINT8	u8_vipss_sel)
{
	SS_DEV_T st_split_vipss_ss_dev;

	st_split_vipss_ss_dev.u8_sel = u8_vipss_sel;
	st_split_vipss_ss_dev.u8_vipss_trigger_mode	= FORCE_TRIGGER;
	api_ss_vipss_trig(&st_split_vipss_ss_dev);
    u8dev = u8dev;
}

VOID app_video_vopss_trig(UINT8 u8dev,UINT8	u8_vopss_sel)
{
	SS_DEV_T st_split_vopss_ss_dev;
	st_split_vopss_ss_dev.u8_sel = u8_vopss_sel;
	st_split_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
	st_split_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode	= FORCE_TRIGGER;
	api_ss_vopss_trig_mode_cfg(&st_split_vopss_ss_dev);
	api_ss_vopss_trig(&st_split_vopss_ss_dev);
    u8dev = u8dev;
}

VOID app_digital_audio_cfg(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_digital_audio_id, BOOL b_status)
{
	MISC_DIGITAL_AUDIO_CFG g_st_misc_digital_audio_cfg;
	MISC_MCLK_CFG_T g_st_misc_mclk_cfg;
	g_st_misc_digital_audio_cfg.u8_audio_src = g_u8_HDMI_RX_PROT[Mapp_Rx[u8dev][u8_audio_src]];
	g_st_misc_digital_audio_cfg.u8_digital_audio_dst = u8_digital_audio_id;

	g_st_misc_digital_audio_cfg.b_enable = false;
	api_misc_digital_audio_out_cfg(&g_st_misc_digital_audio_cfg);

//	g_st_misc_mclk_cfg.b_mclk_from_crystal = false;
	g_st_misc_mclk_cfg.u8_mclk_div = MISC_CLKDIV_1;
	g_st_misc_mclk_cfg.u8_digital_audio_dst = u8_digital_audio_id;
	g_st_misc_mclk_cfg.u8_mclk_en = b_status;
	g_st_misc_mclk_cfg.u8_audio_src = g_u8_HDMI_RX_PROT[Mapp_Rx[u8dev][u8_audio_src]];
    g_st_misc_mclk_cfg.b_mclk_from_crystal= false;
	api_misc_digital_mclk_cfg(&g_st_misc_mclk_cfg);

	g_st_misc_digital_audio_cfg.b_enable = b_status;
	api_misc_digital_audio_out_cfg(&g_st_misc_digital_audio_cfg);
}

VOID app_tx_audio_cfg(UINT8 u8dev,UINT8	u8_audio_src, UINT8 u8_tx)
{
	MISC_TX_AUDIO_SEL st_misc_tx_audio_sel;
    log("app_tx_audio_cfg tx%d audio source %d\r\n",u8_tx,u8_audio_src);
    log("g_st_hdmi_tx %d audio source %d\r\n",u8_tx,g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src);
	u8_Tx_audio_sel[u8dev][u8_tx] = u8_audio_src;
    if(g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src == FROM_HDMI_RX_AUDIO)
    {
    	st_misc_tx_audio_sel.U8_tx_dst = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_tx]];
    	st_misc_tx_audio_sel.u8_audio_src = g_u8_HDMI_RX_PROT[Mapp_Rx[u8dev][u8_tx]];
    	api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][u8_tx], MS_TRUE);
    	api_misc_tx_audio_in_cfg(&st_misc_tx_audio_sel);
    	memcpy((&g_st_hdmi_tx[u8dev][u8_tx].st_infoframes_packets.st_audio_infoframe), &g_st_hdmi_rx[u8dev][u8_audio_src].st_packets_infoframes.st_audio_infoframe, sizeof(HDMI_AUDIO_INFOFRAME_T));
    	api_hdmi_tx_audio_change(&g_st_hdmi_tx[u8dev][u8_tx]);
    	api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][u8_tx], b_audio_mute[u8dev][u8_tx]);
    }
//	app_digital_audio_cfg(u8_audio_src, g_u8_i2s_idx[u8_tx], b_digital_audio_mute[u8dev][u8_tx]?FALSE:TRUE);
//	app_digital_audio_cfg(u8_audio_src, g_u8_spdif_idx[u8_tx], b_digital_audio_mute[u8dev][u8_tx]?FALSE:TRUE);
}

VOID app_hdmi_input_hdmi_output_cfg(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_tx)
{
	MISC_TX_AUDIO_SEL st_misc_tx_audio_sel;
    log("app_hdmi_input_hdmi_output_cfg tx%d audio source %d\r\n",u8_tx,u8_audio_src);
	u8_Tx_audio_sel[u8dev][u8_tx] = u8_audio_src;
	st_misc_tx_audio_sel.U8_tx_dst = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_tx]];
	st_misc_tx_audio_sel.u8_audio_src = g_u8_HDMI_RX_PROT[Mapp_Rx[u8dev][u8_tx]];
    g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_HDMI_RX_AUDIO;
	api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][u8_tx], MS_TRUE);
	api_misc_tx_audio_in_cfg(&st_misc_tx_audio_sel);
	memcpy((&g_st_hdmi_tx[u8dev][u8_tx].st_infoframes_packets.st_audio_infoframe), &g_st_hdmi_rx[u8dev][u8_audio_src].st_packets_infoframes.st_audio_infoframe, sizeof(HDMI_AUDIO_INFOFRAME_T));
	api_hdmi_tx_audio_change(&g_st_hdmi_tx[u8dev][u8_tx]);
	api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][u8_tx], b_audio_mute[u8dev][u8_tx]);

}

VOID display_mode_cfg(UINT8 u8dev,UINT8 u8Mode,UINT8 u8SubMode)
{
	UINT8 u8_i;

    OSDShown[u8dev] = false;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
    display_mode_bak[u8dev] = display_mode[u8dev];
    display_mode[u8dev] = u8Mode;
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_A], false);
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_B], false);
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_C], false);
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_D], false);
    video_ms1826_reset(u8dev,display_mode[u8dev], display_mode_bak[u8dev]);
    switch (display_mode[u8dev])
    {
        case SPLITTER:
            g_st_app_splitter_dev[u8dev].b_splitter_sw = false;
            g_st_app_splitter_dev[u8dev].u8_splitter_mode = u8SubMode;
            app_splitter_mode_config(u8dev,&g_st_app_splitter_dev[u8dev]);
            for(u8_i = 0; u8_i < 4; u8_i ++)
            {
                if(g_u8_HDMI_TX_PROT[g_st_app_splitter_dev[u8dev].u8_dst_sel] != g_u8_HDMI_TX_PROT[u8_i])
                {
                    //switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);
                }
            }
			log("[INFO] MS1826 function is VIDEO_SPLITTER.");
            break;
        case SPLICER:
            gst_splicer_app_dev[u8dev].b_splicer_sw = false;
            gst_splicer_app_dev[u8dev].u8_tx_mirror = false;
            gst_splicer_app_dev[u8dev].u8_splicer_mode = u8SubMode;
            app_splicer_mode_config(u8dev,&gst_splicer_app_dev[u8dev]);
			log("[INFO] MS1826 function is VIDEO_SPLICER.");
            break;
        case MATRIX_SWITCHER:
        case MATRIX_SWITCHER_OUT:
            app_single_mode_config(u8dev);
			log("[INFO] MS1826 function is SEAMLESS_MATRIX_SWITCHER.");
            break;
        case MS1826_BYPASS_MATRIX_SWITCHER:
            for(u8_i = 0; u8_i < 4; u8_i ++)
            {
                if(Bypass_Tx_Status[u8dev][u8_i] == 0)
                {
                    Bypass_Tx_Status[u8dev][u8_i] = 1;
                    switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);
                }
            }
			log("[INFO] MS1826 function is BYPASS_MATRIX_SWITCHER.");
            break;
        default:
            break;
    }
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_A], MS_TRUE);
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_B], MS_TRUE);
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_C], MS_TRUE);
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][HDMI_TX_D], MS_TRUE);
    log("[INFO] Work mode switch.\r\n");
}

VOID app_mode_switch_cfg(UINT8 u8dev,UINT8 u8Mode,UINT8 u8SubMode)
{
	UINT8 u8_i;

    OSDShown[u8dev] = false;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
    display_mode_bak[u8dev] = display_mode[u8dev];
    display_mode[u8dev] = u8Mode;
    switch (display_mode[u8dev])
    {
        case SPLITTER:
            g_st_app_splitter_dev[u8dev].b_splitter_sw = MS_TRUE;
            g_st_app_splitter_dev[u8dev].u8_splitter_mode = u8SubMode;
            app_splitter_mode_config(u8dev,&g_st_app_splitter_dev[u8dev]);
            for(u8_i = 0; u8_i < 4; u8_i ++)
            {
                if(g_u8_HDMI_TX_PROT[g_st_app_splitter_dev[u8dev].u8_dst_sel] != g_u8_HDMI_TX_PROT[u8_i])
                {
                    //switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);
                }
            }
			log("[INFO] MS1826 function is VIDEO_SPLITTER.");
            break;
        case SPLICER:
			gst_splicer_app_dev[u8dev].b_splicer_sw = MS_TRUE;
			gst_splicer_app_dev[u8dev].u8_tx_mirror = false;
            app_splicer_mode_config(u8dev,&gst_splicer_app_dev[u8dev]);

			log("[INFO] MS1826 function is VIDEO_SPLICER.");
            break;
        case MATRIX_SWITCHER:
        case MATRIX_SWITCHER_OUT:
            app_single_mode_switch_config(u8dev,FALSE);
//            app_single_mode_config(u8dev);
			app_single_mode_rx_switch(u8dev,Mapp_Rx[u8dev][0], 0);
			app_single_mode_rx_switch(u8dev,Mapp_Rx[u8dev][1], 1);
			app_single_mode_rx_switch(u8dev,Mapp_Rx[u8dev][2], 2);
			app_single_mode_rx_switch(u8dev,Mapp_Rx[u8dev][3], 3);
			log("[INFO] MS1826 function is SEAMLESS_MATRIX_SWITCHER.");
            break;
        case MS1826_BYPASS_MATRIX_SWITCHER:
            for(u8_i = 0; u8_i < 4; u8_i ++)
            {
                if(Bypass_Tx_Status[u8dev][u8_i] == 0)
                {
                    Bypass_Tx_Status[u8dev][u8_i] = 1;
                    switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);
                }
            }
			log("[INFO] MS1826 function is BYPASS_MATRIX_SWITCHER.");
            break;
        default:
            break;
    }
}

VOID app_digital_input_hdmi_output_cfg(UINT8 u8dev,UINT8 g_u8_digital_idx,UINT8 u8_tx)
{
	MISC_TX_AUDIO_SEL st_misc_tx_audio_sel;
	MISC_DIGITAL_AUDIO_CFG g_st_misc_digital_audio_cfg;
	MISC_MCLK_CFG_T g_st_misc_mclk_cfg;
	UINT8 u8_digital_audio_dst;

    log("app_digital_input_hdmi_output_cfg tx%d audio source %d\r\n",u8_tx,g_u8_digital_idx);

	if(g_u8_digital_idx == I2S1_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_I2S_PAD;
	}
	else if(g_u8_digital_idx == SPIDF1_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_SPDIF_PAD;
	}
	else if(g_u8_digital_idx == I2S2_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_I2S_PAD;
	}
	else if(g_u8_digital_idx == SPIDF2_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_SPDIF_PAD;
	}
	else if(g_u8_digital_idx == I2S3_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_I2S_PAD;
	}
	else if(g_u8_digital_idx == SPIDF3_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_SPDIF_PAD;
	}
	else if(g_u8_digital_idx == I2S4_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_I2S_PAD;
	}
	else if(g_u8_digital_idx == SPIDF4_AUDIO)
	{
		g_st_hdmi_tx[u8dev][u8_tx].u8_audio_src = FROM_SPDIF_PAD;
	}

	if(g_u8_digital_idx == I2S1_AUDIO)
		u8_digital_audio_dst = DIG_AUDIO_I2S1;
	else if(g_u8_digital_idx == I2S2_AUDIO)
		u8_digital_audio_dst = DIG_AUDIO_I2S2;
	else if(g_u8_digital_idx == I2S3_AUDIO)
		u8_digital_audio_dst = DIG_AUDIO_I2S3;
	else if(g_u8_digital_idx == I2S4_AUDIO)
		u8_digital_audio_dst = DIG_AUDIO_I2S4;

	g_st_misc_digital_audio_cfg.u8_audio_src = RX1_AUDIO+Mapp_Rx[u8dev][u8_tx];
	g_st_misc_digital_audio_cfg.u8_digital_audio_dst = u8_digital_audio_dst;
	g_st_misc_digital_audio_cfg.b_enable = false;
	api_misc_digital_audio_out_cfg(&g_st_misc_digital_audio_cfg);

	g_st_hdmi_tx[u8dev][u8_tx].u8_txIndex = g_u8_HDMI_TX_PROT[u8_tx];


	api_hdmi_tx_audio_change(&g_st_hdmi_tx[u8dev][u8_tx]);

	st_misc_tx_audio_sel.U8_tx_dst = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_tx]];
	st_misc_tx_audio_sel.u8_audio_src = g_u8_digital_idx;
	api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][u8_tx], MS_TRUE);
	api_misc_tx_audio_in_cfg(&st_misc_tx_audio_sel);
	api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][u8_tx], false);

// modify by wf8421 20230718 to disable mclk
	g_st_misc_mclk_cfg.u8_mclk_div = MISC_CLKDIV_1;
	g_st_misc_mclk_cfg.u8_digital_audio_dst = u8_digital_audio_dst;
	g_st_misc_mclk_cfg.u8_mclk_en = false;
	g_st_misc_mclk_cfg.u8_audio_src = g_u8_HDMI_RX_PROT[Mapp_Rx[u8dev][u8_tx]];
	g_st_misc_mclk_cfg.b_mclk_from_crystal = false;
	api_misc_digital_mclk_cfg(&g_st_misc_mclk_cfg);
}

VOID app_bcsh_adjust(UINT8 u8dev,UINT8 u8_vehc_idx)
{
	if(u8_vehc_idx < VEHC_VIPSS_1)
	{
//		api_vehc_instance_init(&g_st_vovehc[u8_vehc_idx], u8_vehc_idx);
		if(CHIP_INTERNAL_PROCESS_COLORSPACE == HDMI_COLORSPACE_RGB)
		{
			g_st_vovehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx;
			g_st_vovehc[u8dev][u8_vehc_idx].b_yuv = false;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN;

			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.s8_bcsh_bri = g_u8_bcsh_bri[u8_vehc_idx]-128;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_con = g_u8_bcsh_con[u8_vehc_idx];
		}
		else
		{
			g_st_vovehc[u8dev][u8_vehc_idx].b_yuv = MS_TRUE;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN | SAT_EN | HUE_EN;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.s8_bcsh_bri = g_u8_bcsh_bri[u8_vehc_idx] - 128;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_con = g_u8_bcsh_con[u8_vehc_idx];
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_sat = g_u8_bcsh_sat[u8_vehc_idx];
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_hue = g_u8_bcsh_hue[u8_vehc_idx];
		}
//		api_vehc_bcsh_config(&g_st_vovehc[u8dev][u8_vehc_idx]);

	}

	else if(u8_vehc_idx < VEHC_NUM)
	{
		u8_vehc_idx = u8_vehc_idx % 4;
//		api_vehc_instance_init(&g_st_vivehc[u8dev][u8_vehc_idx], u8_vehc_idx);
		if(CHIP_INTERNAL_PROCESS_COLORSPACE == HDMI_COLORSPACE_RGB)
		{
			g_st_vivehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx+VEHC_VIPSS_1;
			g_st_vivehc[u8dev][u8_vehc_idx].b_yuv = false;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.s8_bcsh_bri = g_u8_bcsh_bri[u8_vehc_idx]-128;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_con = g_u8_bcsh_con[u8_vehc_idx];
		}
		else
		{
			g_st_vivehc[u8dev][u8_vehc_idx].b_yuv = MS_TRUE;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN | SAT_EN | HUE_EN;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.s8_bcsh_bri = g_u8_bcsh_bri[u8_vehc_idx] - 128;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_con = g_u8_bcsh_con[u8_vehc_idx];
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_sat = g_u8_bcsh_sat[u8_vehc_idx];
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_hue = g_u8_bcsh_hue[u8_vehc_idx];
		}
//		api_vehc_bcsh_config(&g_st_vivehc[u8dev][u8_vehc_idx]);
	}
	else
		return;
}

BOOL video_ms1826_reset(UINT8 u8dev,UINT8 u8_dis_cur, UINT8 u8_dis_bak)
{
    MS1826_CHipReset(u8dev);
    switch(u8_dis_cur)
    {
        case SPLITTER:
//            if(u8_dis_bak == MATRIX_SWITCHER
//                ||u8_dis_bak == MATRIX_SWITCHER_OUT)
            {
                video_process_cfg(u8dev);
            }
			break;
        case SPLICER:
//            if(u8_dis_bak == MATRIX_SWITCHER
//                ||u8_dis_bak == MATRIX_SWITCHER_OUT)
            {
                video_process_cfg(u8dev);
            }
			break;
        case MATRIX_SWITCHER:
        case MATRIX_SWITCHER_OUT:
            video_process_cfg(u8dev);
			break;
        case MS1826_BYPASS_MATRIX_SWITCHER:
//            if(u8_dis_bak == MATRIX_SWITCHER
//                ||u8_dis_bak == MATRIX_SWITCHER_OUT)
            {
                video_process_cfg(u8dev);
            }
			break;

    }
    return MS_TRUE;
}

VOID _video_timgen_delay(UINT8 u8dev,UINT8 u8_ms)
{
	mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0x0f);	//关闭4路timgen
	mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0x3f);	//打开timgen1和timgen2
	mculib_delay_ms(u8_ms);
	mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0xff);	//打开timgen3和timgen4
}

//----------------------------------------------------------------------------------

VOID app_ms1826_picture_adjust(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_value,u8 u8_adjust_mode)
{
//    u8_vehc_idx = u8_vehc_idx+VEHC_VIPSS_1;
	if(u8_vehc_idx < VEHC_VIPSS_1)
	{
//		api_vehc_instance_init(&g_st_vovehc[u8_vehc_idx], u8_vehc_idx);
        g_st_vovehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx;
		if(CHIP_INTERNAL_PROCESS_COLORSPACE == HDMI_COLORSPACE_RGB)
		{
			g_st_vovehc[u8dev][u8_vehc_idx].b_yuv = false;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN /*| SAT_EN | HUE_EN*/;

		}
		else
		{
			g_st_vovehc[u8dev][u8_vehc_idx].b_yuv = MS_TRUE;
			g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN | SAT_EN | HUE_EN;
		}
        if(((u8_adjust_mode&MS1826_ADJUST_BRIGHTNESS)==MS1826_ADJUST_BRIGHTNESS)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&BRI_EN))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.s8_bcsh_bri = (INT8)(u8_value-128);
        }
        if(((u8_adjust_mode&MS1826_ADJUST_CONTRAST)==MS1826_ADJUST_CONTRAST)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&CON_EN))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_con = u8_value;
        }
        if(((u8_adjust_mode&MS1826_ADJUST_SATURATION)==MS1826_ADJUST_SATURATION)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&SAT_EN))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_sat = u8_value;
        }
        if(((u8_adjust_mode&MS1826_ADJUST_HUE)==MS1826_ADJUST_HUE)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&HUE_EN))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_hue = u8_value;
        }
		api_vehc_bcsh_config(&g_st_vovehc[u8dev][u8_vehc_idx]);

	}
	else if(u8_vehc_idx < VEHC_NUM)
	{
		u8_vehc_idx = u8_vehc_idx % 4;
        g_st_vivehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx+VEHC_VIPSS_1;
//		api_vehc_instance_init(&g_st_vivehc[u8_vehc_idx], u8_vehc_idx);
		if(CHIP_INTERNAL_PROCESS_COLORSPACE == HDMI_COLORSPACE_RGB)
		{
			g_st_vivehc[u8dev][u8_vehc_idx].b_yuv = false;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN;
		}
		else
		{
			g_st_vivehc[u8dev][u8_vehc_idx].b_yuv = MS_TRUE;
			g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag = BRI_EN | CON_EN | SAT_EN | HUE_EN;
		}
        if(((u8_adjust_mode&MS1826_ADJUST_BRIGHTNESS)==MS1826_ADJUST_BRIGHTNESS)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&BRI_EN))
        {
            g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.s8_bcsh_bri = (INT8)(u8_value-128);
        }
        if(((u8_adjust_mode&MS1826_ADJUST_CONTRAST)==MS1826_ADJUST_CONTRAST)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&CON_EN))
        {
            g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_con = u8_value;
        }
        if(((u8_adjust_mode&MS1826_ADJUST_SATURATION)==MS1826_ADJUST_SATURATION)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&SAT_EN))
        {
            g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_sat = u8_value;
        }
        if(((u8_adjust_mode&MS1826_ADJUST_HUE)==MS1826_ADJUST_HUE)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_en_flag&HUE_EN))
        {
            g_st_vivehc[u8dev][u8_vehc_idx].st_bcsh.u8_bcsh_hue = u8_value;
        }
		api_vehc_bcsh_config(&g_st_vivehc[u8dev][u8_vehc_idx]);
	}
	else
		return;
}

void app_ms1826_colortemp_adjust(UINT8 u8dev,BOOL b_en,UINT8 u8_vehc_idx, UINT8 u8_r_gain, UINT8 u8_g_gain, UINT8 u8_b_gain)
{
    VEHC_DYN_T l_st_vodyn;
	if(u8_vehc_idx < VEHC_VIPSS_1)
	{
        g_st_vovehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx;
//        api_vehc_instance_init(&g_st_vovehc[u8_vehc_idx], u8_vehc_idx);

    }
#if 0
    else if(u8_vehc_idx < VEHC_NUM)
    {
		u8_vehc_idx = u8_vehc_idx % 4;
        g_st_vovehc[0].u8_vehc_idx = u8_vehc_idx;
        api_vehc_instance_init(&g_st_vovehc[0], u8_vehc_idx);
    }
#endif
    else
        return;

    l_st_vodyn.b_en = b_en;
    l_st_vodyn.u8_dyn_vr_gain = u8_r_gain;
    l_st_vodyn.u8_dyn_yg_gain = u8_g_gain;
    l_st_vodyn.u8_dyn_ub_gain = u8_b_gain;
    l_st_vodyn.s16_dyn_vr_offset = 1023;
    l_st_vodyn.s16_dyn_yg_offset = 1023;
    l_st_vodyn.s16_dyn_ub_offset = 1023;
    api_vehc_dyn_config(&g_st_vovehc[u8dev][u8_vehc_idx],&l_st_vodyn);
}

void api_ms1826_user_timing_get(pstTimingTable_t pstUsrTiming, HDMI_VIDEO_TIMING_T * pstMs1826Timing)
{
    if((pstUsrTiming->u8HPol == Hpos)&&(pstUsrTiming->u8VPol == Vpos)&&(pstUsrTiming->u8Interlace == PROG))
        pstMs1826Timing->u8_polarity = ProgrVPosHPos;
    else if((pstUsrTiming->u8HPol == Hpos)&&(pstUsrTiming->u8VPol == Vneg)&&(pstUsrTiming->u8Interlace == PROG))
        pstMs1826Timing->u8_polarity = ProgrVNegHPos;
    else if((pstUsrTiming->u8HPol == Hneg)&&(pstUsrTiming->u8VPol == Vpos)&&(pstUsrTiming->u8Interlace == PROG))
        pstMs1826Timing->u8_polarity = ProgrVPosHNeg;
    else if((pstUsrTiming->u8HPol == Hneg)&&(pstUsrTiming->u8VPol == Vneg)&&(pstUsrTiming->u8Interlace == PROG))
        pstMs1826Timing->u8_polarity = ProgrVNegHNeg;
    else if((pstUsrTiming->u8HPol == Hpos)&&(pstUsrTiming->u8VPol == Vpos)&&(pstUsrTiming->u8Interlace == INTR))
        pstMs1826Timing->u8_polarity = InterVPosHPos;
    else if((pstUsrTiming->u8HPol == Hpos)&&(pstUsrTiming->u8VPol == Vneg)&&(pstUsrTiming->u8Interlace == INTR))
        pstMs1826Timing->u8_polarity = InterVNegHPos;
    else if((pstUsrTiming->u8HPol == Hneg)&&(pstUsrTiming->u8VPol == Vpos)&&(pstUsrTiming->u8Interlace == INTR))
        pstMs1826Timing->u8_polarity = InterVPosHNeg;
    else if((pstUsrTiming->u8HPol == Hneg)&&(pstUsrTiming->u8VPol == Vneg)&&(pstUsrTiming->u8Interlace == INTR))
        pstMs1826Timing->u8_polarity = InterVNegHNeg;

    pstMs1826Timing->u16_htotal     = pstUsrTiming->u16HTotal;
    pstMs1826Timing->u16_vtotal     = pstUsrTiming->u16VTotal;
    pstMs1826Timing->u16_hactive    = pstUsrTiming->u16HActive;
    pstMs1826Timing->u16_vactive    = pstUsrTiming->u16VActive;
    pstMs1826Timing->u16_pixclk     = pstUsrTiming->u16PClk;
    pstMs1826Timing->u16_vfreq      = pstUsrTiming->u8VClk*100;
    pstMs1826Timing->u16_hoffset    = pstUsrTiming->u16HBP+pstUsrTiming->u16HSync;
    pstMs1826Timing->u16_voffset    = pstUsrTiming->u16VBP+pstUsrTiming->u16VSync;
    pstMs1826Timing->u16_hsyncwidth = pstUsrTiming->u16HSync;
    pstMs1826Timing->u16_vsyncwidth = pstUsrTiming->u16VSync;
}

void app_ms1826_output_resolution(UINT8 u8dev,UINT8 u8TxID,UINT8 u8ResID)
{
	PLL_DEV_T         pll_dev;
	MISC_MODULE_CLK_T st_module_clk;
	VDS_TMG_DEV_T     st_timgen_dev;
    MISC_POLARITY_T   l_stTxPolarity;

	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], false);

//	api_misc_timing_get(g_arrOutputTable[u8ResID], &g_stVideo_HDMITx_Timing[u8TxID]);
    if(u8ResID>=(sizeof(res_ms1826table)/sizeof(res_ms1826table[0])))
    {
        log("Unsupport resolution \r\n");
        return;
    }
    api_ms1826_user_timing_get((pstTimingTable_t)&res_ms1826table[u8ResID],&g_stVideo_HDMITx_Timing[u8dev][u8TxID]);
    log("u8_polarity = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u8_polarity);
    log("u16_htotal  = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_htotal);
    log("u16_vtotal  = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vtotal);
    log("u16_hactive = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hactive);
    log("u16_vactive = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vactive);
    log("u16_pixclk  = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_pixclk);
    log("u16_vfreq   = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vfreq);
    log("u16_hoffset = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hoffset);
    log("u16_voffset = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_voffset);
    log("u16_hsyncwidth = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hsyncwidth);
    log("u16_vsyncwidth = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vsyncwidth);
	st_timgen_dev.pst_timing = &g_stVideo_HDMITx_Timing[u8dev][u8TxID];
	st_timgen_dev.u8_timgen_id = TIMGEN_1+u8TxID;
	st_timgen_dev.b_timgen_en = MS_TRUE;
#if ENABLE_MS1826_FRAME_LOCK
    if(ms1826_hal_hdmi_rx_get_tmds_clk(g_u8FrameLockRxTable[u8dev][u8TxID]))  //
	    st_timgen_dev.u8_workmode  = Casecade_Mode;
    else
#endif
	    st_timgen_dev.u8_workmode  = FreeRun_Mode;

	pll_dev.b_osc_input = MS_TRUE;
	pll_dev.u8_pll_idx = PLLV1 + u8TxID;
	pll_dev.u32_clk_out = st_timgen_dev.pst_timing->u16_pixclk * 10000;

	st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_TIMGEN1 + u8TxID;
	st_module_clk.u8_src_clk = MISC_CLKSRC_PLLV1 + u8TxID;
	st_module_clk.u8_clkdiv = MISC_CLKDIV_1;

    l_stTxPolarity.u8_chan =  MISC_VIDEO_MODULE_TIMGEN1+u8TxID;
    switch(g_stVideo_HDMITx_Timing[u8dev][u8TxID].u8_polarity)
    {
        case ProgrVNegHNeg:
        case InterVNegHNeg:
            l_stTxPolarity.u8_hsync_flip_en = MS_TRUE;
            l_stTxPolarity.u8_vsync_flip_en = MS_TRUE;
            break;
        case ProgrVNegHPos:
        case InterVNegHPos:
            l_stTxPolarity.u8_hsync_flip_en = false;
            l_stTxPolarity.u8_vsync_flip_en = MS_TRUE;
            break;
        case ProgrVPosHNeg:
        case InterVPosHNeg:
            l_stTxPolarity.u8_hsync_flip_en = MS_TRUE;
            l_stTxPolarity.u8_vsync_flip_en = false;
            break;
        case ProgrVPosHPos:
        case InterVPosHPos:
            l_stTxPolarity.u8_hsync_flip_en = false;
            l_stTxPolarity.u8_vsync_flip_en = false;
            break;
    }

    if((display_mode[u8dev] == MATRIX_SWITCHER)
        ||(display_mode[u8dev] == MATRIX_SWITCHER_OUT))
    {
	    SINGLE_PROCESS(u8dev,u8TxID);
//	    app_single_mode_config(u8dev);
        if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
        {
            _single_mode_frc_cfg(u8dev);
        }
    }
	api_pll_config(&pll_dev);
	api_misc_timgen_clkin_sel(&st_module_clk);
	api_vds_tmg_config(&st_timgen_dev);
    api_misc_vdstmg_polarity_cfg(&l_stTxPolarity);

	g_st_hdmi_tx[u8dev][u8TxID].u8_txIndex = g_u8_HDMI_TX_PROT[u8TxID];
	g_st_hdmi_tx[u8dev][u8TxID].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8TxID];
	g_st_hdmi_tx[u8dev][u8TxID].b_full_edid = FALSE;
	g_st_hdmi_tx[u8dev][u8TxID].u8_color_depth = HDMI_COLOR_DEPTH_24;
	g_st_hdmi_tx[u8dev][u8TxID].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
//	g_st_hdmi_tx[u8dev][u8TxID].u8_audio_src = FROM_HDMI_RX_AUDIO;
	g_st_hdmi_tx[u8dev][u8TxID].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
	g_st_hdmi_tx[u8dev][u8TxID].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_pixclk;
	memcpy((&g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
    g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets.st_avi_infoframe.u8_video_code = res_ms1826table[u8ResID].u8Vic;

#if MS1826_PATTERN_SWITCH_ENABLE
    g_st_txcsc_dev[u8dev][u8TxID].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8TxID]];
    if(g_st_vovehc[u8dev][u8TxID].st_pattern.b_test_pattern_en == TRUE)
    {
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_color_range = CSC_YUV444_BT709;
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_colorspace =  HDMI_COLORSPACE_YCBCR444;
    }
    else
    {
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_colorspace   = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_color_range  = CHIP_INTERNAL_PROCESS_COLORRANGE;
    }
    g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_out_color_range  = CHIP_TX_CSC_OUT_COLORRANGE;
    g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
    g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
    api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8TxID]);
#endif

    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], false);
	api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8TxID]);
    _video_port_hdmi_tx_driver_config(u8dev,u8TxID);
	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], MS_TRUE);

	OSDShown[u8dev] = MS_TRUE;
	sys_show_osd(u8dev,OSDShown[u8dev], 1<<u8TxID);
}

void app_ms1826_rx_edid_update(UINT8 u8dev,UINT8 u8RxID,UINT8 *pu8EdidBuf,UINT16 u16Size)
{
    api_hdmi_rx_set_hpd(&g_st_hdmi_rx[u8dev][u8RxID],FALSE);
    if(is_edid_valid(pu8EdidBuf))
    {
        mapp_EdidLimitTo4K30(pu8EdidBuf,pu8EdidBuf,u16Size,MS1826_RX_SUPPORT_INTERLACE,MS1826_RX_SUPPORT_ONLY_PCM);
        api_hdmi_rx_edid_update(&g_st_hdmi_rx[u8dev][u8RxID],pu8EdidBuf,u16Size);
    }
//    else
//        api_hdmi_rx_edid_update(&g_st_hdmi_rx[u8dev][u8RxID],pu8EdidBuf,u16Size);

#if 1
    {
        UINT16 i;
        log("input %d edid:",u8RxID);
        for(i=0;i<256;i++)
        {
            if(i%16==0)
            {
                dbg_printf("\r\n");
            }
            dbg_printf("0x%02x,",pu8EdidBuf[i]);
        }
        dbg_printf("\r\n");
    }
#endif
    mculib_delay_ms(100);//wu 50
    api_hdmi_rx_set_hpd(&g_st_hdmi_rx[u8dev][u8RxID],TRUE);
}

void app_ms1826_tx_format_config(UINT8 u8dev,UINT8 u8TxID, BOOL bIsHDMI, UINT8 u8ColorSpace)
{
    UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets.st_avi_infoframe.u8_video_code;
	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], false);
    if(bIsHDMI)
    {
        g_u8TxFormat[u8dev][u8TxID] = MS1826_TX_HDMI;
    	gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace = u8ColorSpace;
    	if(gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace == HDMI_COLORSPACE_RGB)
    		gst_inf_pack[u8dev].st_avi_infoframe.e_colorimetry	= HDMI_COLORIMETRY_NONE;
    	if(gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace == HDMI_COLORSPACE_YCBCR444)
    		gst_inf_pack[u8dev].st_avi_infoframe.e_colorimetry	= HDMI_COLORIMETRY_ITU_601;
        g_st_hdmi_tx[u8dev][u8TxID].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8TxID];
        g_st_hdmi_tx[u8dev][u8TxID].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
//        g_st_hdmi_tx[u8dev][u8TxID].u8_audio_src = FROM_HDMI_RX_AUDIO;
    }
    else
    {
        g_u8TxFormat[u8dev][u8TxID] = MS1826_TX_DVI;
    	gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace = HDMI_COLORSPACE_RGB;
		gst_inf_pack[u8dev].st_avi_infoframe.e_colorimetry	= HDMI_COLORIMETRY_NONE;
        g_st_hdmi_tx[u8dev][u8TxID].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8TxID];
    }

	g_st_hdmi_tx[u8dev][u8TxID].u8_txIndex = g_u8_HDMI_TX_PROT[u8TxID];
	g_st_hdmi_tx[u8dev][u8TxID].b_full_edid = FALSE;
    g_st_hdmi_tx[u8dev][u8TxID].u8_color_depth = HDMI_COLOR_DEPTH_24;
	g_st_hdmi_tx[u8dev][u8TxID].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
	g_st_hdmi_tx[u8dev][u8TxID].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_pixclk;
	memcpy((&g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
    g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;

#if MS1826_PATTERN_SWITCH_ENABLE
    g_st_txcsc_dev[u8dev][u8TxID].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8TxID]];
    if(g_st_vovehc[u8dev][u8TxID].st_pattern.b_test_pattern_en == TRUE)
    {
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_color_range = CSC_YUV444_BT709;
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_colorspace =  HDMI_COLORSPACE_YCBCR444;
    }
    else
    {
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_colorspace   = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_color_range  = CHIP_INTERNAL_PROCESS_COLORRANGE;
    }
    g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_out_color_range  = CHIP_TX_CSC_OUT_COLORRANGE;
    g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
    g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
    api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8TxID]);
#endif

    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], false);
	api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8TxID]);
    _video_port_hdmi_tx_driver_config(u8dev,u8TxID);
	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], MS_TRUE);
}

void app_ms1826_tx_hdcp14_enable(UINT8 u8dev,UINT8 u8TxID, BOOL bEn)
{
#if 1
    if(api_hdmi_tx_hdcp14_init(&g_st_hdmi_tx[u8dev][u8TxID]))
    {
        log("api_hdmi_tx_hdcp14_init OK\r\n");
    }
#endif
    mculib_delay_ms(20);

    if(bEn)
    {
//        if(api_hdmi_tx_hdcp14_get_status(&g_st_hdmi_tx[u8TxID]))
        {
            api_hdmi_tx_hdcp14_enable(&g_st_hdmi_tx[u8dev][u8TxID],bEn);
        }
//        else
//            api_hdmi_tx_hdcp14_enable(&g_st_hdmi_tx[u8TxID],FALSE);
    }
    else
    {
        api_hdmi_tx_hdcp14_enable(&g_st_hdmi_tx[u8dev][u8TxID],bEn);
    }
}

void app_ms1826_tx_get_edid(UINT8 u8dev,UINT8 u8TxID,UINT8 *pu8Edidbuf)
{
    api_hdmi_tx_get_edid(&g_st_hdmi_tx[u8dev][u8TxID],pu8Edidbuf,&g_hdmi_edid_flag[u8dev][u8TxID],FALSE);
}

void app_ms1826_tx_timing_config(UINT8 u8dev,UINT8 u8TxID,pstTimingTable_t pstTiming)
{
	PLL_DEV_T         pll_dev;
	MISC_MODULE_CLK_T st_module_clk;
	VDS_TMG_DEV_T     st_timgen_dev;
    MISC_POLARITY_T   l_stTxPolarity;

	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], false);
#if MS1826_PATTERN_SWITCH_ENABLE
        g_st_txcsc_dev[u8dev][u8TxID].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8TxID]];
        if(g_st_vovehc[u8dev][u8TxID].st_pattern.b_test_pattern_en == TRUE)
        {
            g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_color_range = CSC_YUV444_BT709;
            g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_colorspace =  HDMI_COLORSPACE_YCBCR444;
        }
        else
        {
            g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_colorspace   = CHIP_INTERNAL_PROCESS_COLORSPACE;
            g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_in_color_range  = CHIP_INTERNAL_PROCESS_COLORRANGE;
        }
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_out_color_range  = CHIP_TX_CSC_OUT_COLORRANGE;
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
        g_st_txcsc_dev[u8dev][u8TxID].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
        api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8TxID]);
#endif

    api_ms1826_user_timing_get((pstTimingTable_t)pstTiming,&g_stVideo_HDMITx_Timing[u8dev][u8TxID]);
    g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets.st_vendor_infoframe.u8_vic = 0;
    log("u8_polarity = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u8_polarity);
    log("u16_htotal  = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_htotal);
    log("u16_vtotal  = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vtotal);
    log("u16_hactive = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hactive);
    log("u16_vactive = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vactive);
    log("u16_pixclk  = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_pixclk);
    log("u16_vfreq   = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vfreq);
    log("u16_hoffset = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hoffset);
    log("u16_voffset = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_voffset);
    log("u16_hsyncwidth = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hsyncwidth);
    log("u16_vsyncwidth = %d\r\n",g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vsyncwidth);
	st_timgen_dev.pst_timing = &g_stVideo_HDMITx_Timing[u8dev][u8TxID];
	st_timgen_dev.u8_timgen_id = TIMGEN_1+u8TxID;
	st_timgen_dev.b_timgen_en = MS_TRUE;
#if ENABLE_MS1826_FRAME_LOCK
    if(ms1826_hal_hdmi_rx_get_tmds_clk(g_u8FrameLockRxTable[u8dev][u8TxID]))
        st_timgen_dev.u8_workmode  = Casecade_Mode;
    else
#endif
	    st_timgen_dev.u8_workmode  = FreeRun_Mode;

	pll_dev.b_osc_input = MS_TRUE;
	pll_dev.u8_pll_idx = PLLV1 + u8TxID;
	pll_dev.u32_clk_out = st_timgen_dev.pst_timing->u16_pixclk * 10000;

	st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_TIMGEN1 + u8TxID;
	st_module_clk.u8_src_clk = MISC_CLKSRC_PLLV1 + u8TxID;
	st_module_clk.u8_clkdiv = MISC_CLKDIV_1;

    l_stTxPolarity.u8_chan =  MISC_VIDEO_MODULE_TIMGEN1+u8TxID;
    switch(g_stVideo_HDMITx_Timing[u8dev][u8TxID].u8_polarity)
    {
        case ProgrVNegHNeg:
        case InterVNegHNeg:
            l_stTxPolarity.u8_hsync_flip_en = MS_TRUE;
            l_stTxPolarity.u8_vsync_flip_en = MS_TRUE;
            break;
        case ProgrVNegHPos:
        case InterVNegHPos:
            l_stTxPolarity.u8_hsync_flip_en = false;
            l_stTxPolarity.u8_vsync_flip_en = MS_TRUE;
            break;
        case ProgrVPosHNeg:
        case InterVPosHNeg:
            l_stTxPolarity.u8_hsync_flip_en = MS_TRUE;
            l_stTxPolarity.u8_vsync_flip_en = false;
            break;
        case ProgrVPosHPos:
        case InterVPosHPos:
            l_stTxPolarity.u8_hsync_flip_en = false;
            l_stTxPolarity.u8_vsync_flip_en = false;
            break;
    }

    if((display_mode[u8dev] == MATRIX_SWITCHER)
        ||(display_mode[u8dev] == MATRIX_SWITCHER_OUT))
    {
	    SINGLE_PROCESS(u8dev,u8TxID);
    }
	api_pll_config(&pll_dev);
	api_misc_timgen_clkin_sel(&st_module_clk);
	api_vds_tmg_config(&st_timgen_dev);
    api_misc_vdstmg_polarity_cfg(&l_stTxPolarity);

	g_st_hdmi_tx[u8dev][u8TxID].u8_txIndex = g_u8_HDMI_TX_PROT[u8TxID];
	g_st_hdmi_tx[u8dev][u8TxID].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8TxID];
	g_st_hdmi_tx[u8dev][u8TxID].b_full_edid = FALSE;
	g_st_hdmi_tx[u8dev][u8TxID].u8_color_depth = HDMI_COLOR_DEPTH_24;
	g_st_hdmi_tx[u8dev][u8TxID].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
//	g_st_hdmi_tx[u8dev][u8TxID].u8_audio_src = FROM_HDMI_RX_AUDIO;
	g_st_hdmi_tx[u8dev][u8TxID].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
	g_st_hdmi_tx[u8dev][u8TxID].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_pixclk;
	memcpy((&g_st_hdmi_tx[u8dev][u8TxID].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], false);
	api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8TxID]);
    _video_port_hdmi_tx_driver_config(u8dev,u8TxID);
	api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8TxID], MS_TRUE);

	OSDShown[u8dev] = MS_TRUE;
	sys_show_osd(u8dev,OSDShown[u8dev], 1<<u8TxID);
}

UINT16 app_ms1826_get_display_horizontal(UINT8 u8dev,UINT8 u8TxID)
{
    return g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_hactive;
}

UINT16 app_ms1826_get_display_vertical(UINT8 u8dev,UINT8 u8TxID)
{
    return g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vactive;
}

UINT16 app_ms1826_get_display_frame_rate(UINT8 u8dev,UINT8 u8TxID)
{
    return g_stVideo_HDMITx_Timing[u8dev][u8TxID].u16_vfreq;
}

void app_ms1826_audio_input_switch(UINT8 u8dev,UINT8 u8_audio_src, UINT8 u8_tx)
{
    if(u8_audio_src == MS1826_AUDIO_SRC_HDMI1)
    {
        app_hdmi_input_hdmi_output_cfg(u8dev,RX1_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_HDMI2)
    {
        app_hdmi_input_hdmi_output_cfg(u8dev,RX2_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_HDMI3)
    {
        app_hdmi_input_hdmi_output_cfg(u8dev,RX3_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_HDMI4)
    {
        app_hdmi_input_hdmi_output_cfg(u8dev,RX4_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_I2S1)
    {
        app_digital_input_hdmi_output_cfg(u8dev,I2S1_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_I2S2)
    {
        app_digital_input_hdmi_output_cfg(u8dev,I2S2_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_I2S3)
    {
        app_digital_input_hdmi_output_cfg(u8dev,I2S3_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_I2S4)
    {
        app_digital_input_hdmi_output_cfg(u8dev,I2S4_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_SPDIF1)
    {
        app_digital_input_hdmi_output_cfg(u8dev,SPIDF1_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_SPDIF2)
    {
        app_digital_input_hdmi_output_cfg(u8dev,SPIDF2_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_SPDIF3)
    {
        app_digital_input_hdmi_output_cfg(u8dev,SPIDF3_AUDIO,u8_tx);
    }
    else if(u8_audio_src == MS1826_AUDIO_SRC_DIGITAL_SPDIF4)
    {
        app_digital_input_hdmi_output_cfg(u8dev,SPIDF4_AUDIO,u8_tx);
    }
}

void app_ms1826_audio_output_mute(UINT8 u8dev,UINT8	u8_audio_src, UINT8 u8_tx, BOOL bIsMute)
{
    b_digital_audio_mute[u8dev][u8_tx] = bIsMute;
    app_ms1826_digital_output_hdmi_input_cfg(u8dev,u8_audio_src,u8_tx);
}

void app_ms1826_digital_output_hdmi_input_cfg(UINT8 u8dev,UINT8	u8_audio_src, UINT8 u8_tx)
{
	app_digital_audio_cfg(u8dev,u8_audio_src, g_u8_i2s_idx[u8_tx], b_digital_audio_mute[u8dev][u8_tx]?FALSE:TRUE);
	app_digital_audio_cfg(u8dev,u8_audio_src, g_u8_spdif_idx[u8_tx], b_digital_audio_mute[u8dev][u8_tx]?FALSE:TRUE);
}

void app_ms1826_pattern_config(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_pattern_id,BOOL b_enable)
{
    u8 l_u8Vic;
	if(u8_vehc_idx < VEHC_VIPSS_1)
	{
        g_st_vovehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx;
        g_st_vovehc[u8dev][u8_vehc_idx].b_yuv = false;
        if(b_enable==0)
            g_st_vovehc[u8dev][u8_vehc_idx].st_pattern.b_test_pattern_en = FALSE;
        else
            g_st_vovehc[u8dev][u8_vehc_idx].st_pattern.b_test_pattern_en = TRUE;
        g_st_vovehc[u8dev][u8_vehc_idx].st_pattern.u8_pattern_sel = u8_pattern_id;

        g_st_hdmi_tx[u8dev][u8_vehc_idx].u8_txIndex = g_u8_HDMI_TX_PROT[u8_vehc_idx];
        g_st_hdmi_tx[u8dev][u8_vehc_idx].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8_vehc_idx];
        g_st_hdmi_tx[u8dev][u8_vehc_idx].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][u8_vehc_idx].u8_color_depth = HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][u8_vehc_idx].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
    //  g_st_hdmi_tx[u8dev][u8_vehc_idx].u8_audio_src = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][u8_vehc_idx].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][u8_vehc_idx].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8_vehc_idx].u16_pixclk;
        l_u8Vic = g_st_hdmi_tx[u8dev][u8_vehc_idx].st_infoframes_packets.st_avi_infoframe.u8_video_code;
        memcpy((&g_st_hdmi_tx[u8dev][u8_vehc_idx].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][u8_vehc_idx].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8Vic;
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_vehc_idx], false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_vehc_idx]);
        _video_port_hdmi_tx_driver_config(u8dev,u8_vehc_idx);

        api_vehc_pattern_config(&g_st_vovehc[u8dev][u8_vehc_idx]);

        g_st_txcsc_dev[u8dev][u8_vehc_idx].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8_vehc_idx]];
        if(b_enable)
        {
            g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.csc_in_color_range = CSC_YUV444_BT709;
            g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.csc_in_colorspace =  HDMI_COLORSPACE_YCBCR444;
        }
        else
        {
            g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.csc_in_colorspace   = CHIP_INTERNAL_PROCESS_COLORSPACE;
            g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.csc_in_color_range  = CHIP_INTERNAL_PROCESS_COLORRANGE;
        }
        g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.csc_out_color_range  = CHIP_TX_CSC_OUT_COLORRANGE;
        g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
        g_st_txcsc_dev[u8dev][u8_vehc_idx].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
        api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8_vehc_idx]);

        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_vehc_idx], MS_TRUE);
	}
}

void app_ms18226_rx_pattern_config(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_pattern_id,BOOL b_enable)
{
    MISC_RXPATTERN_T l_stRxPatternVar;

    l_stRxPatternVar.u8_pattern_idx = u8_vehc_idx;
    l_stRxPatternVar.b_enable = b_enable;
    l_stRxPatternVar.u8_timing = MISC_RXPATTERN_1080P;
    l_stRxPatternVar.u8_pattern = u8_pattern_id;
    api_misc_rxpattern_cfg(&l_stRxPatternVar);
}

UINT8 app_ms1826_get_tx_hpd_sta(u8 u8dev,UINT8 u8_tx)
{
    return b_hpd_bak[u8dev][u8_tx]?1:0;
}

UINT8 app_ms1826_get_rx_signal_sta(u8 u8dev,UINT8 u8_rx)
{
    return g_st_hdmi_rx[u8dev][u8_rx].b_timing_stable?1:0;
}

void app_ms1826_get_rx_timing(u8 u8dev,UINT8 u8_rx,pstTimingTable_t pstRxTiming)
{
    pstRxTiming->u16HActive = Video_HDMIRx_Timing[u8dev][u8_rx].u16_hactive;
    pstRxTiming->u16HTotal  = Video_HDMIRx_Timing[u8dev][u8_rx].u16_htotal;
    pstRxTiming->u16HBP     = Video_HDMIRx_Timing[u8dev][u8_rx].u16_hoffset-Video_HDMIRx_Timing[u8dev][u8_rx].u16_hsyncwidth;
    pstRxTiming->u16HFP     = Video_HDMIRx_Timing[u8dev][u8_rx].u16_vactive;
    pstRxTiming->u16HSync   = Video_HDMIRx_Timing[u8dev][u8_rx].u16_hsyncwidth;

    pstRxTiming->u16VActive = Video_HDMIRx_Timing[u8dev][u8_rx].u16_vactive;
    pstRxTiming->u16VTotal  = Video_HDMIRx_Timing[u8dev][u8_rx].u16_vtotal;
    pstRxTiming->u16VBP     = Video_HDMIRx_Timing[u8dev][u8_rx].u16_voffset-Video_HDMIRx_Timing[u8dev][u8_rx].u16_vsyncwidth;
    pstRxTiming->u16VFP     = Video_HDMIRx_Timing[u8dev][u8_rx].u16_vactive;
    pstRxTiming->u16VSync   = Video_HDMIRx_Timing[u8dev][u8_rx].u16_vsyncwidth;

    pstRxTiming->u8VClk     = (Video_HDMIRx_Timing[u8dev][u8_rx].u16_vfreq+10)/100;
    pstRxTiming->u16PClk    = Video_HDMIRx_Timing[u8dev][u8_rx].u16_pixclk;
    pstRxTiming->u8Interlace= Video_HDMIRx_Timing[u8dev][u8_rx].u8_polarity%2?PROG:INTR;

    switch(Video_HDMIRx_Timing[u8dev][u8_rx].u8_polarity)
    {
        case ProgrVNegHNeg:
        case InterVNegHNeg:
            pstRxTiming->u8HPol = Hneg;
            pstRxTiming->u8VPol = Vneg;
            break;
        case ProgrVNegHPos:
        case InterVNegHPos:
            pstRxTiming->u8HPol = Hpos;
            pstRxTiming->u8VPol = Vneg;
            break;
        case ProgrVPosHNeg:
        case InterVPosHNeg:
            pstRxTiming->u8HPol = Hneg;
            pstRxTiming->u8VPol = Vpos;
            break;
        case ProgrVPosHPos:
        case InterVPosHPos:
            pstRxTiming->u8HPol = Hpos;
            pstRxTiming->u8VPol = Vpos;
            break;
    }
}

void app_ms1826_get_tx_edid_data(u8 u8dev,UINT8 u8_tx,UINT8* pu8_edidbuf)
{
    if(g_st_hdmi_tx[u8dev][u8_tx].pu8_edid_buf != NULL)
    {
        memcpy(pu8_edidbuf,g_st_hdmi_tx[u8dev][u8_tx].pu8_edid_buf,256);
    }
    else
        memset(pu8_edidbuf,0,256);
}

void app_ms1826_tx_split_config(u8 u8dev,u8 u8WinID,u8 u8Update,u16 u16X,u16 u16Y,u16 u16W,u16 u16H)
{
#if 1
    if(display_mode[u8dev] == SPLITTER)
    {
        stMs1826Wincfg_T l_stMS1826WinCfg;

        l_stMS1826WinCfg.u8Rx = g_u8SplitterDispMapping[u8dev][u8WinID];
        l_stMS1826WinCfg.u8Tx = 0;
        l_stMS1826WinCfg.u16X = u16X;
        l_stMS1826WinCfg.u16Y = u16Y;
        l_stMS1826WinCfg.u16W = u16W;
        l_stMS1826WinCfg.u16H = u16H;

        app_splitter_user_mode_win_config(u8dev,u8WinID,&l_stMS1826WinCfg,false,u8Update?MS_TRUE:false,TRUE);
    }
#endif
//    void app_single_user_splitter_mode_test(void);
//    app_single_user_splitter_mode_test();

}

void app_ms1826_tx_split_update(u8 u8dev,u8 u8Path)
{
    if(display_mode[u8dev] == SPLITTER)
    {
        app_splitter_user_mode_win_update(u8dev,false,u8Path);
    }
}

void app_ms1826_get_osd_string(u8 u8dev,u8 u8Rx)
{
    memcpy(p_str,Ms1826event[u8dev].u8OsdStr[u8Rx],MS1826_OSD_MAX_LEN);
}

void app_ms1826_display_user_string(u8 u8dev,u8 u8Rx)
{
//    if(g_user_osd_str_enable[u8dev][u8Rx])
//    {
//        OSDShown[u8dev] = MS_TRUE;
//        sys_show_osd(u8dev,OSDShown[u8dev], (1<<u8Rx));
//        g_u8_time = 3;
//    }

    g_bUserOsdStringDisplayEnable[u8dev][u8Rx] = TRUE;
}

void app_ms1826_set_display_user_string_enable(u8 u8dev,u8 u8Rx, bool bEnable)
{
    g_user_osd_str_enable[u8dev][u8Rx] = bEnable;

    g_bUserOsdStringDisplayEnable[u8dev][u8Rx] = TRUE;
//    if(bEnable)
//    {
//        OSDShown[u8dev] = MS_TRUE;
//        sys_show_osd(u8dev,OSDShown[u8dev], (1<<u8Rx));
//        g_u8_time = 3;
//    }
//    else
//    {
//        OSDShown[u8dev] = false;
//        sys_show_osd(u8dev,OSDShown[u8dev], (1<<u8Rx));
//        g_u8_time = 3;
//    }
}

void app_ms1826_set_user_string_color(u8 u8dev,u8 u8Rx,u8 u8R,u8 u8G,u8 u8B)
{
    g_user_osd_string_color[u8dev][u8Rx][0] = u8R;
    g_user_osd_string_color[u8dev][u8Rx][1] = u8G;
    g_user_osd_string_color[u8dev][u8Rx][2] = u8B;

    g_bUserOsdStringDisplayEnable[u8dev][u8Rx] = TRUE;
//    if(g_user_osd_str_enable[u8dev][u8Rx])
//    {
//        OSDShown[u8dev] = MS_TRUE;
//        sys_show_osd(u8dev,OSDShown[u8dev], (1<<u8Rx));
//        g_u8_time = 3;
//    }
}

void app_ms1826_set_user_string_position(u8 u8dev,u8 u8Rx,u16 u16X,u16 u16Y)
{
    g_u16UserStrDispPos[u8dev][u8Rx][0] = u16X;
    g_u16UserStrDispPos[u8dev][u8Rx][1] = u16Y;

    g_bUserOsdStringDisplayEnable[u8dev][u8Rx] = TRUE;
//    if(g_user_osd_str_enable[u8dev][u8Rx])
//    {
//        OSDShown[u8dev] = MS_TRUE;
//        sys_show_osd(u8dev,OSDShown[u8dev], (1<<u8Rx));
//        g_u8_time = 3;
//    }
}

void app_ms1826_user_string_display(u8 u8dev)
{
    u8 i;
    for(i=0;i<4;i++)
    {
        if(g_bUserOsdStringDisplayEnable[u8dev][i])
        {
            g_bUserOsdStringDisplayEnable[u8dev][i] = FALSE;

            if(g_user_osd_str_enable[u8dev][i])
            {
                OSDShown[u8dev] = MS_TRUE;
                sys_show_osd(u8dev,OSDShown[u8dev], (1<<i));
                g_u8_time = 3;
            }
            else
            {
                OSDShown[u8dev] = false;
                sys_show_osd(u8dev,OSDShown[u8dev], (1<<i));
                g_u8_time = 3;
            }
        }
    }
}

void app_ms1826_tx_splicer_config(u8 u8dev,u8 u8PortIndex,u16 u16X,u16 u16Y,u16 u16W,u16 u16H)
{
    stMs1826Wincfg_T stMs1826WinTable;

    stMs1826WinTable.u8Rx = Mapp_Rx[u8dev][u8PortIndex];
    stMs1826WinTable.u8Tx = u8PortIndex;
    stMs1826WinTable.u16X = u16X;
    stMs1826WinTable.u16Y = u16Y;
    stMs1826WinTable.u16W = u16W;
    stMs1826WinTable.u16H = u16H;

    app_single_user_mode_win_config(u8dev,&stMs1826WinTable,TRUE);
}

#if ENABLE_MS1826_FRAME_LOCK
void app_ms1826_frame_lock_select(u8 u8dev,u8 u8RxIndex,u8 u8TxIndex)
{
    g_u8FrameLockRxTable[u8dev][u8TxIndex] = u8RxIndex;

    if(ms1826_hal_hdmi_rx_get_tmds_clk(g_u8FrameLockRxTable[u8dev][u8TxIndex]))
    {
        _video_timgen_cfg_frame_lock(u8dev,u8RxIndex,TIMGEN_1+u8TxIndex);
    }
    else
    {
        _video_timgen_cfg(u8dev,TIMGEN_1+u8TxIndex);
    }
}
#endif

void app_ms1826_peaking_adjust(UINT8 u8dev,UINT8 u8_vehc_idx,UINT8 u8_value,u8 u8_adjust_mode)
{
    if(u8_vehc_idx < VEHC_VIPSS_1)
    {
        g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.b_pk_en = MS_TRUE;
        if(((u8_adjust_mode&MS1826_ADJUST_SHARPNESS)==MS1826_ADJUST_SHARPNESS)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.b_pk_en&MS_TRUE))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.u8_pk_gain = u8_value;
        }
        if(((u8_adjust_mode&MS1826_ADJUST_NOISE)==MS1826_ADJUST_NOISE)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.b_pk_en&MS_TRUE))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.u8_pk_nkl = u8_value;
        }

        api_vehc_peaking_config(&g_st_vovehc[u8dev][u8_vehc_idx]);
    }
    else if(u8_vehc_idx < VEHC_NUM)
    {
        u8_vehc_idx = u8_vehc_idx % 4;
        g_st_vivehc[u8dev][u8_vehc_idx].u8_vehc_idx = u8_vehc_idx+VEHC_VIPSS_1;
        g_st_vivehc[u8dev][u8_vehc_idx].st_peaking.b_pk_en = MS_TRUE;
        if(((u8_adjust_mode&MS1826_ADJUST_SHARPNESS)==MS1826_ADJUST_SHARPNESS)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.b_pk_en&MS_TRUE))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.u8_pk_gain = u8_value;
        }
        if(((u8_adjust_mode&MS1826_ADJUST_NOISE)==MS1826_ADJUST_NOISE)
            &&(g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.b_pk_en&MS_TRUE))
        {
            g_st_vovehc[u8dev][u8_vehc_idx].st_peaking.u8_pk_nkl = u8_value;
        }

        api_vehc_peaking_config(&g_st_vivehc[u8dev][u8_vehc_idx]);
    }
    else
        return;

}

void app_ms1826_tx_output_enable(u8 u8dev,u8 u8Tx,bool bSta)
{
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8Tx], bSta);
}

void app_ms1826_set_splitter_mode(u8 u8dev,u8 u8Mode)
{
#if 1
    if(u8Mode==0)
    {
        if(display_mode[u8dev]!=cg_stMs1826DevCfgTable[u8dev].u8WorkMode)
        {
            //display_mode_cfg(u8dev,cg_stMs1826DevCfgTable[u8dev].u8WorkMode,0);
            app_mode_switch_cfg(u8dev,cg_stMs1826DevCfgTable[u8dev].u8WorkMode,0);
        }
    }
    else
    {
        if(display_mode[u8dev]==SPLITTER)
        {
            g_st_app_splitter_dev[u8dev].b_splitter_sw = MS_TRUE;
            g_st_app_splitter_dev[u8dev].u8_splitter_mode = u8Mode;
            app_splitter_mode_config(u8dev,&g_st_app_splitter_dev[u8dev]);
        }
        else
        {
            //display_mode_cfg(u8dev,SPLITTER,u8Mode);
            app_mode_switch_cfg(u8dev,SPLITTER,u8Mode);
        }
    }
#else
    extern void app_single_user_splitter_mode_test(void);
    app_single_user_splitter_mode_test();
#endif
}

void app_ms1826_reg_read(u8 u8dev,u16 u16AddrOffect,u8 *u8RVal)
{
    u8 l_u8OldDev = g_u8MS1826_I2CDevID;
    g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[u8dev].u8I2cIndex;
    *u8RVal = mculib_i2c_read_16bidx8bval(gu8ChipAddr[u8dev],u16AddrOffect);
    g_u8MS1826_I2CDevID = l_u8OldDev;
}

void app_ms1826_reg_write(u8 u8dev,u16 u16AddrOffect,u8 u8RVal)
{
    u8 l_u8OldDev = g_u8MS1826_I2CDevID;
    g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[u8dev].u8I2cIndex;
    mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],u16AddrOffect,u8RVal);
    g_u8MS1826_I2CDevID = l_u8OldDev;
}

//--------------------------------------------------------------------------------
void app_MS1826Switch(u8 u8dev,u8 u8Rx, u8 u8Tx)
{
    Ms1826event[u8dev].u8OutputSel[u8Tx] = u8Rx;
    Ms1826event[u8dev].bPortChangeFlag[u8Tx] = TRUE;
}

void app_MS1826BypassSwitch(u8 u8dev,u8 u8Rx, u8 u8Tx)
{
    Ms1826event[u8dev].u8BypassOutputSel[u8Tx] = u8Rx;
    Ms1826event[u8dev].bBypassPortChangeFlag[u8Tx] = TRUE;
}

void app_MS1826FreezeChange(u8 u8dev,u8 u8Tx, u8 u8Sta)
{
    Ms1826event[u8dev].bFreezeChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8FreezeSta[u8Tx] = u8Sta;
}

void app_MS1826TxResolutionChange(u8 u8dev,u8 u8Tx, u8 u8ResID)
{
    Ms1826event[u8dev].bTxResolutionChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxResolutionID[u8Tx] = u8ResID;
}

void app_MS1826SwitchFreeze(u8 u8dev,u8 u8Tx, u16 u16Time)
{
    Ms1826event[u8dev].bFreezeSwitchFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u16FreezeDelayTime = u16Time;  //time unit 1ms
    Ms1826event[u8dev].bFreezeTimeEnd = FALSE;
}

void app_MS1826SwFreezeTimer(u8 u8dev)
{
    if(Ms1826event[u8dev].u16FreezeDelayTime)
    {
        Ms1826event[u8dev].u16FreezeDelayTime--;
        if(Ms1826event[u8dev].u16FreezeDelayTime==0)
        {
            Ms1826event[u8dev].bFreezeTimeEnd = TRUE;
        }
    }
}

void app_MS1826PictureAdjust(u8 u8dev,u8 u8Tx,u8 u8Value,u8 u8AdjMode)
{
    if((u8AdjMode&MS1826_ADJUST_BRIGHTNESS)==MS1826_ADJUST_BRIGHTNESS)
    {
        Ms1826event[u8dev].bBrightnessAdjustFlag[u8Tx] = TRUE;
        Ms1826event[u8dev].u8Brightness[u8Tx] = u8Value;
    }
    if((u8AdjMode&MS1826_ADJUST_CONTRAST)==MS1826_ADJUST_CONTRAST)
    {
        Ms1826event[u8dev].bContrastAdjustFlag[u8Tx] = TRUE;
        Ms1826event[u8dev].u8Contrast[u8Tx] = u8Value;
    }
    if((u8AdjMode&MS1826_ADJUST_SATURATION)==MS1826_ADJUST_SATURATION)
    {
        Ms1826event[u8dev].bSaturationAdjustFlag[u8Tx] = TRUE;
        Ms1826event[u8dev].u8Saturation[u8Tx] = u8Value;
    }
    if((u8AdjMode&MS1826_ADJUST_HUE)==MS1826_ADJUST_HUE)
    {
        Ms1826event[u8dev].bHueAdjustFlag[u8Tx] = TRUE;
        Ms1826event[u8dev].u8Hue[u8Tx] = u8Value;
    }
}

void app_MS1826TxConfig(u8 u8dev,u8 u8Tx,u8 u8Value)
{
    Ms1826event[u8dev].bTxCfgChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxFormat[u8Tx] = u8Value;
    Ms1826event[u8dev].u8TxColorSpace[u8Tx] = CHIP_TX_CSC_OUT_COLORSPACE;
}

void app_MS1826SetTxColorSpace(u8 u8dev,u8 u8Tx,u8 u8Value)
{
    Ms1826event[u8dev].bTxCfgChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxColorSpace[u8Tx] = u8Value;
}

void app_MS1826TxHdcpEnable(u8 u8dev,u8 u8Tx,u8 u8Value)
{
    Ms1826event[u8dev].bTxHdcp14ChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxHdcp14Sta[u8Tx] = u8Value;
}

void app_MS1826RxEdidUpdate(u8 u8dev,u8 u8Tx,pu8 pu8Edidbuf,u16 u16Size)
{
    Ms1826event[u8dev].bRxEdidChangeFlag[u8Tx] = TRUE;
    memcpy(Ms1826event[u8dev].u8EdidBuf[u8Tx],pu8Edidbuf,u16Size);
}

void app_MS1826GetTxEdid(u8 u8dev,u8 u8Tx)
{
    Ms1826event[u8dev].bReadTxEdidFlag[u8Tx] = TRUE;
}

void app_MS1826TxDigitalAudioMute(u8 u8dev,u8 u8Tx,u8 u8Value)
{
    Ms1826event[u8dev].bTxDigitalAudioChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxDigitalAudioSta[u8Tx] = u8Value;
}

void app_MS1826TxDigitalAudioSourceSelect(u8 u8dev,u8 u8Tx,u8 u8AudioSource)
{
    Ms1826event[u8dev].bTxDigitalAudioChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxDigitalAudioSource[u8Tx] = u8AudioSource;
    g_bDigitalAudioSwByHdmiTx[u8dev][u8Tx] = FALSE;
}

void app_MS1826TxAudioSourceSwitch(u8 u8dev,u8 u8Tx,u8 u8Value)
{
    Ms1826event[u8dev].bTxAudioSourceChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxAudioSourceSta[u8Tx] = u8Value;
}

void app_MS1826SetUserTiming(u8 u8dev,u8 u8Tx,pstTimingTable_t pstTiming)
{
    Ms1826event[u8dev].bTxTimingChangeFlag[u8Tx] = TRUE;
    memcpy(&Ms1826event[u8dev].stTiming[u8Tx],pstTiming,sizeof(stTimingTable_t));
}

void app_MS1826TxPatternConfig(u8 u8dev,u8 u8Tx,u8 u8PatternID,u8 u8En)
{
    Ms1826event[u8dev].bTxPatternChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxPatternID[u8Tx] = u8PatternID;
    Ms1826event[u8dev].u8TxPatternEnable[u8Tx] = u8En;
}

void app_MS1826PictureColorConfig(u8 u8dev,u8 u8Tx,u8 u8RVal,u8 u8GVal,u8 u8BVal,u8 u8En)
{
    Ms1826event[u8dev].bColorChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8RedValue[u8Tx]        = u8RVal;
    Ms1826event[u8dev].u8GreenValue[u8Tx]      = u8GVal;
    Ms1826event[u8dev].u8BlueValue[u8Tx]       = u8BVal;
    Ms1826event[u8dev].u8ColorEnable[u8Tx]     = u8En;
}

void app_MS1826SplitConfig(u8 u8dev,u8 u8PortID,u8 u8Enable,u16 u16X,u16 u16Y,u16 u16W,u16 u16H)
{
    Ms1826event[u8dev].bSplitChangeFlag[u8PortID] = TRUE;
    Ms1826event[u8dev].u8SplitEnable[u8PortID]=u8Enable;
    Ms1826event[u8dev].stSplitPosision[u8PortID].u16_horizontal_start=u16X;
    Ms1826event[u8dev].stSplitPosision[u8PortID].u16_horizontal_active=u16W;
    Ms1826event[u8dev].stSplitPosision[u8PortID].u16_vertical_start=u16Y;
    Ms1826event[u8dev].stSplitPosision[u8PortID].u16_vertical_active=u16H;
}

void app_MS1826WorkModeSwitch(u8 u8dev,u8 u8WorkMode,u8 u8SubMode)
{
    Ms1826event[u8dev].bWorkModeSwitch = TRUE;
    Ms1826event[u8dev].u8WorkMode = u8WorkMode;
    Ms1826event[u8dev].u8SubMode = u8SubMode;
}

void app_MS1826SetOsdString(u8 u8dev,u8 u8Rx,u8 u8Len,u8 *pu8Str,bool bIsAlignRight)
{
    Ms1826event[u8dev].bOsdStringChangeFlag[u8Rx] = TRUE;
    Ms1826event[u8dev].bOsdStringIsAlignRight[u8Rx] = bIsAlignRight;
    g_bUserOsdStringIsAlignRight[u8dev][u8Rx] = bIsAlignRight;
    memset(Ms1826event[u8dev].u8OsdStr[u8Rx],' ',MS1826_OSD_MAX_LEN);
    if(g_bUserOsdStringIsAlignRight[u8dev][u8Rx])
    {
        memcpy(Ms1826event[u8dev].u8OsdStr[u8Rx]+(MS1826_OSD_MAX_LEN-u8Len),pu8Str,u8Len);
    }
    else
        memcpy(Ms1826event[u8dev].u8OsdStr[u8Rx],pu8Str,u8Len);
}

void app_MS1826SetUserOsdStringEnable(u8 u8dev,u8 u8Rx, bool bEnable)
{
    Ms1826event[u8dev].bOsdStringDisplayChangeFlag[u8Rx]    = TRUE;;
    Ms1826event[u8dev].bOsdStringDisplayEnable[u8Rx]        = bEnable;
}

void app_MS1826SetUserStringColor(u8 u8dev,u8 u8Rx, u8 u8R,u8 u8G,u8 u8B)
{
    Ms1826event[u8dev].bOsdStringColorChangeFlag[u8Rx]  = TRUE;
    Ms1826event[u8dev].bOsdStringColorRed[u8Rx]         = u8R;
    Ms1826event[u8dev].bOsdStringColorGreen[u8Rx]       = u8G;
    Ms1826event[u8dev].bOsdStringColorBlue[u8Rx]        = u8B;
}

void app_MS1826SetUserOsdStringPosition(u8 u8dev,u8 u8Rx, u16 u16PosX,u16 u16PosY)
{
    Ms1826event[u8dev].bOsdStringPositionChangeFlag[u8Rx]   = TRUE;
    Ms1826event[u8dev].u16OsdStringPositionX[u8Rx]          = u16PosX;
    Ms1826event[u8dev].u16OsdStringPositionY[u8Rx]          = u16PosY;
}

void app_MS1826SetVideoMute(u8 u8dev,u8 u8Rx,bool bSta)
{
    Ms1826event[u8dev].bVideoMuteChangeFlag[u8Rx] = TRUE;
    Ms1826event[u8dev].bVideoMuteSta[u8Rx] = bSta;
}

void app_MS1826TxHdmiAudioMute(u8 u8dev,u8 u8Tx,u8 u8Value)
{
    Ms1826event[u8dev].bTxHdmiAudioMuteChangeFlag[u8Tx] = TRUE;
    Ms1826event[u8dev].u8TxHdmiAudioMuteSta[u8Tx] = u8Value;
}

void app_MS1826SplicerConfig(u8 u8dev,u8 u8PortID,u16 u16X,u16 u16Y,u16 u16W,u16 u16H)
{
    Ms1826event[u8dev].bSplicerChangeFlag[u8PortID] = TRUE;
    Ms1826event[u8dev].stSplicerVar[u8PortID].u16X=u16X;
    Ms1826event[u8dev].stSplicerVar[u8PortID].u16W=u16W;
    Ms1826event[u8dev].stSplicerVar[u8PortID].u16Y=u16Y;
    Ms1826event[u8dev].stSplicerVar[u8PortID].u16H=u16H;
}

void app_MS1826SplicerEnable(u8 u8dev,u8 u8PortID,bool bEn)
{
    Ms1826event[u8dev].bSplicerChangeFlag[u8PortID] = TRUE;
    Ms1826event[u8dev].bSplicerEnable[u8PortID]=bEn;
}

#if ENABLE_MS1826_FRAME_LOCK
void app_MS1826FrameLockSelect(u8 u8dev,u8 u8RxIndex,u8 u8TxIndex)
{
    Ms1826event[u8dev].bTxFrameLockChangeFlag[u8TxIndex] = TRUE;
    Ms1826event[u8dev].u8TxFrameLockRxSelect[u8TxIndex]=u8RxIndex;
}
#endif

void app_MS1826SetSplitterMode(u8 u8dev,u8 u8Mode)
{
    Ms1826event[u8dev].bSplitterModeChangeFlag = TRUE;
    Ms1826event[u8dev].u8SplitterMode = u8Mode;
}

void app_MS1826SetSplitterUpdate(u8 u8dev,u8 u8Path)
{
    Ms1826event[u8dev].bSplitUpdateChangeFlag = TRUE;
    Ms1826event[u8dev].u8SplitPath = u8Path;
}

void app_MS1826FunctionProcess(u8 u8dev)
{
    u8 i;
    static u8 sl_u8FreezeFlag = 0;
    u8 l_u8StartFreezeFlag = 0;

    for(i=0;i<4;i++)
    {
        if(Ms1826event[u8dev].bPortChangeFlag[i])   //视频切换需要先单独配置完成，否则声音通道对应错误
        {
            switch(display_mode[u8dev])
            {
                case MATRIX_SWITCHER:
                case MATRIX_SWITCHER_OUT:
                    {
                        if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
                        {
                            SINGLE_PROCESS(u8dev,i);
                        }
                        app_single_mode_rx_switch(u8dev,Ms1826event[u8dev].u8OutputSel[i], i);
                        g_u8SplitterDispMapping[u8dev][i] = Ms1826event[u8dev].u8OutputSel[i];
                    }
                    break;
                case SPLITTER:
                    {
                        app_splitter_mode_rx_switch(u8dev,Ms1826event[u8dev].u8OutputSel[i], i);
                    }
                    break;
            }
            log("TX[%d] source switch to %d\r\n",i,Ms1826event[u8dev].u8OutputSel[i]);
            Ms1826event[u8dev].bPortChangeFlag[i] = FALSE;
        }

        if(Ms1826event[u8dev].bSplicerChangeFlag[i])
        {
            Ms1826event[u8dev].bSplicerChangeFlag[i] = FALSE;
            g_bSplicerEnable[u8dev][i] = Ms1826event[u8dev].bSplicerEnable[i];
            app_ms1826_tx_splicer_config(u8dev,i,
                        Ms1826event[u8dev].stSplicerVar[i].u16X,
                        Ms1826event[u8dev].stSplicerVar[i].u16Y,
                        Ms1826event[u8dev].stSplicerVar[i].u16W,
                        Ms1826event[u8dev].stSplicerVar[i].u16H);

            log("TX[%d] splicer: X:%d Y:%d W:%d H:%d\r\n",i,
                        Ms1826event[u8dev].stSplicerVar[i].u16X,
                        Ms1826event[u8dev].stSplicerVar[i].u16Y,
                        Ms1826event[u8dev].stSplicerVar[i].u16W,
                        Ms1826event[u8dev].stSplicerVar[i].u16H);
        }
#if ENABLE_MS1826_FRAME_LOCK
        if(Ms1826event[u8dev].bTxFrameLockChangeFlag[i])
        {
            Ms1826event[u8dev].bTxFrameLockChangeFlag[i] = FALSE;
            app_ms1826_frame_lock_select(u8dev,Ms1826event[u8dev].u8TxFrameLockRxSelect[i],i);
            log("TX[0x%02x] set frame lock to %d\r\n",i,Ms1826event[u8dev].u8TxFrameLockRxSelect[i]);
        }
#endif
    }

    for(i=0;i<4;i++)
    {
        if(Ms1826event[u8dev].bBypassPortChangeFlag[i])
        {
            Matrix_Tx_Mapping[u8dev][i] = Ms1826event[u8dev].u8BypassOutputSel[i];
            switch_bypass_channel(u8dev,Ms1826event[u8dev].u8BypassOutputSel[i], i);
            log("TX[%d] source bypass switch to %d\r\n",i,Ms1826event[u8dev].u8BypassOutputSel[i]);
            Ms1826event[u8dev].bBypassPortChangeFlag[i] = FALSE;
        }
        if(Ms1826event[u8dev].bFreezeChangeFlag[i])
        {
            Ms1826event[u8dev].bFreezeChangeFlag[i] = FALSE;
            if(Ms1826event[u8dev].u8FreezeSta[i])
            {
                if((display_mode[u8dev] == MATRIX_SWITCHER_OUT)
                    ||(display_mode[u8dev] == MATRIX_SWITCHER))
                    _single_video_freeze(u8dev,i);
                else if(display_mode[u8dev] == SPLITTER)
                    _splitter_video_freeze(u8dev,i);
                else if(display_mode[u8dev] == SPLICER)
                    _splicer_video_freeze(u8dev,i);
                log("TX[%d] freeze\r\n",i);
            }
            else
            {
                if((display_mode[u8dev] == MATRIX_SWITCHER_OUT)
                    ||(display_mode[u8dev] == MATRIX_SWITCHER))
                    _single_video_unfreeze(u8dev,i);
                else if(display_mode[u8dev] == SPLITTER)
                    _splitter_video_unfreeze(u8dev);
                else if(display_mode[u8dev] == SPLICER)
                    _splicer_video_unfreeze(u8dev,i);
                log("TX[%d] unfreeze\r\n",i);
            }
        }
        if(Ms1826event[u8dev].bTxCfgChangeFlag[i])
        {
            Ms1826event[u8dev].bTxCfgChangeFlag[i] = FALSE;
            app_ms1826_tx_format_config(u8dev,i,Ms1826event[u8dev].u8TxFormat[i]?TRUE:FALSE,Ms1826event[u8dev].u8TxColorSpace[i]);
            log("TX[%d] format: %d\r\n",i,Ms1826event[u8dev].u8TxFormat[i]);
        }
        if(Ms1826event[u8dev].bTxHdcp14ChangeFlag[i])
        {
            Ms1826event[u8dev].bTxHdcp14ChangeFlag[i] = FALSE;
            app_ms1826_tx_hdcp14_enable(u8dev,i,Ms1826event[u8dev].u8TxHdcp14Sta[i]?TRUE:FALSE);
            log("TX[%d] HDCP status: %d\r\n",i,Ms1826event[u8dev].u8TxHdcp14Sta[i]);
        }
        if(Ms1826event[u8dev].bReadTxEdidFlag[i])
        {
            u8 u8Edid[256];
            u16 j;
            Ms1826event[u8dev].bReadTxEdidFlag[i] = FALSE;
            app_ms1826_tx_get_edid(u8dev,i,u8Edid);
            dbg_printf("output %d edid:",i);
            for(j=0;j<256;j++)
            {
                if(j%16==0)
                {
                    dbg_printf("\r\n");
                }
                dbg_printf("0x%02x,",u8Edid[j]);
            }
            dbg_printf("\r\n");
        }
        if(Ms1826event[u8dev].bTxTimingChangeFlag[i])
        {
            Ms1826event[u8dev].bTxTimingChangeFlag[i] = FALSE;
            app_ms1826_tx_timing_config(u8dev,i,&Ms1826event[u8dev].stTiming[i]);
        }

        if(Ms1826event[u8dev].bFreezeSwitchFlag[i])
        {
            Ms1826event[u8dev].bFreezeSwitchFlag[i] = FALSE;
            sl_u8FreezeFlag |= 1<<i;
            l_u8StartFreezeFlag = 1;
        }

        if(Ms1826event[u8dev].bTxDigitalAudioChangeFlag[i])
        {
            Ms1826event[u8dev].bTxDigitalAudioChangeFlag[i] = FALSE;
            if(g_bDigitalAudioSwByHdmiTx[u8dev][i])
                Ms1826event[u8dev].u8TxDigitalAudioSource[i] = u8_Tx_audio_sel[u8dev][i];
            app_ms1826_audio_output_mute(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[i],i,Ms1826event[u8dev].u8TxDigitalAudioSta[i]?TRUE:FALSE);
            log("TX[%d] digital audio source: %d\r\n",i,Ms1826event[u8dev].u8TxDigitalAudioSource[i]);
            log("TX[%d] digital audio mute: %s\r\n",i,Ms1826event[u8dev].u8TxDigitalAudioSta[i]?"TRUE":"FALSE");
        }
        if(Ms1826event[u8dev].bTxPatternChangeFlag[i])
        {
            Ms1826event[u8dev].bTxPatternChangeFlag[i] = FALSE;
            app_ms1826_pattern_config(u8dev,i,Ms1826event[u8dev].u8TxPatternID[i],Ms1826event[u8dev].u8TxPatternEnable[i]?TRUE:FALSE);
//            app_ms18226_rx_pattern_config(u8dev,i,Ms1826event[u8dev].u8TxPatternID[i],Ms1826event[u8dev].u8TxPatternEnable[i]?TRUE:FALSE);
        }

        if(Ms1826event[u8dev].bColorChangeFlag[i])
        {
            Ms1826event[u8dev].bColorChangeFlag[i] = FALSE;
            app_ms1826_colortemp_adjust(u8dev,Ms1826event[u8dev].u8ColorEnable[i]?TRUE:FALSE,
                                        i,
                                        Ms1826event[u8dev].u8RedValue[i],
                                        Ms1826event[u8dev].u8GreenValue[i],
                                        Ms1826event[u8dev].u8BlueValue[i]);
        }

        if(Ms1826event[u8dev].bSplitChangeFlag[i])
        {
            Ms1826event[u8dev].bSplitChangeFlag[i] = FALSE;

            app_ms1826_tx_split_config(u8dev,i,
                                Ms1826event[u8dev].u8SplitEnable[i],
                                Ms1826event[u8dev].stSplitPosision[i].u16_horizontal_start,
                                Ms1826event[u8dev].stSplitPosision[i].u16_vertical_start,
                                Ms1826event[u8dev].stSplitPosision[i].u16_horizontal_active,
                                Ms1826event[u8dev].stSplitPosision[i].u16_vertical_active);
            log("TX[%d] split config: en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",i,Ms1826event[u8dev].u8SplitEnable[i],
                                Ms1826event[u8dev].stSplitPosision[i].u16_horizontal_start,
                                Ms1826event[u8dev].stSplitPosision[i].u16_vertical_start,
                                Ms1826event[u8dev].stSplitPosision[i].u16_horizontal_active,
                                Ms1826event[u8dev].stSplitPosision[i].u16_vertical_active);
        }

        if(Ms1826event[u8dev].bBrightnessAdjustFlag[i])
        {
            Ms1826event[u8dev].bBrightnessAdjustFlag[i] = FALSE;
//            app_ms1826_picture_adjust(u8dev,i,Ms1826event[u8dev].u8Brightness[i],MS1826_ADJUST_BRIGHTNESS);
            app_ms1826_picture_adjust(u8dev,i+VEHC_VIPSS_1,Ms1826event[u8dev].u8Brightness[i],MS1826_ADJUST_BRIGHTNESS);
            log("TX[%d] u8Brightness: %d\r\n",i,Ms1826event[u8dev].u8Brightness[i]);
        }
        if(Ms1826event[u8dev].bContrastAdjustFlag[i])
        {
            Ms1826event[u8dev].bContrastAdjustFlag[i] = FALSE;
//            app_ms1826_picture_adjust(u8dev,i,Ms1826event[u8dev].u8Contrast[i],MS1826_ADJUST_CONTRAST);
            app_ms1826_picture_adjust(u8dev,i+VEHC_VIPSS_1,Ms1826event[u8dev].u8Contrast[i],MS1826_ADJUST_CONTRAST);
            log("TX[%d]   u8Contrast: %d\r\n",i,Ms1826event[u8dev].u8Contrast[i]);
        }
        if(Ms1826event[u8dev].bSaturationAdjustFlag[i])
        {
            Ms1826event[u8dev].bSaturationAdjustFlag[i] = FALSE;
//            app_ms1826_picture_adjust(u8dev,i,Ms1826event[u8dev].u8Saturation[i],MS1826_ADJUST_SATURATION);
            app_ms1826_picture_adjust(u8dev,i+VEHC_VIPSS_1,Ms1826event[u8dev].u8Saturation[i],MS1826_ADJUST_SATURATION);
            log("TX[%d] u8Saturation: %d\r\n",i,Ms1826event[u8dev].u8Saturation[i]);
        }
        if(Ms1826event[u8dev].bHueAdjustFlag[i])
        {
            Ms1826event[u8dev].bHueAdjustFlag[i] = FALSE;
//            app_ms1826_picture_adjust(u8dev,i,Ms1826event[u8dev].u8Hue[i],MS1826_ADJUST_HUE);
            app_ms1826_picture_adjust(u8dev,i+VEHC_VIPSS_1,Ms1826event[u8dev].u8Hue[i],MS1826_ADJUST_HUE);
            log("TX[%d]        u8Hue: %d\r\n",i,Ms1826event[u8dev].u8Hue[i]);
        }
        if(Ms1826event[u8dev].bTxAudioSourceChangeFlag[i])
        {
            Ms1826event[u8dev].bTxAudioSourceChangeFlag[i] = FALSE;
//            if(g_bFirstStartupFlag[u8dev][i] == FALSE)
            {
                app_ms1826_audio_input_switch(u8dev,Ms1826event[u8dev].u8TxAudioSourceSta[i],i);
                log("TX[%d] audio source: %d\r\n",i,Ms1826event[u8dev].u8TxAudioSourceSta[i]);
            }
        }

        if(Ms1826event[u8dev].bTxHdmiAudioMuteChangeFlag[i])
        {
            Ms1826event[u8dev].bTxHdmiAudioMuteChangeFlag[i] = FALSE;
            if(Ms1826event[u8dev].u8TxHdmiAudioMuteSta[i]==0)
            {
                b_audio_mute[u8dev][i] = FALSE;
            }
            else
            {
                b_audio_mute[u8dev][i] = TRUE;
            }
            api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][i], b_audio_mute[u8dev][i]);
        }

#if 0
        if(Ms1826event[u8dev].bSplicerChangeFlag[i])
        {
            Ms1826event[u8dev].bSplicerChangeFlag[i] = FALSE;
            g_bSplicerEnable[u8dev][i] = Ms1826event[u8dev].bSplicerEnable[i];
            app_ms1826_tx_splicer_config(u8dev,i,
                        Ms1826event[u8dev].stSplicerVar[i].u16X,
                        Ms1826event[u8dev].stSplicerVar[i].u16Y,
                        Ms1826event[u8dev].stSplicerVar[i].u16W,
                        Ms1826event[u8dev].stSplicerVar[i].u16H);

            log("TX[%d] splicer: X:%d Y:%d W:%d H:%d\r\n",i,
                        Ms1826event[u8dev].stSplicerVar[i].u16X,
                        Ms1826event[u8dev].stSplicerVar[i].u16Y,
                        Ms1826event[u8dev].stSplicerVar[i].u16W,
                        Ms1826event[u8dev].stSplicerVar[i].u16H);
        }
    #if ENABLE_MS1826_FRAME_LOCK
        if(Ms1826event[u8dev].bTxFrameLockChangeFlag[i])
        {
            Ms1826event[u8dev].bTxFrameLockChangeFlag[i] = FALSE;
            app_ms1826_frame_lock_select(u8dev,Ms1826event[u8dev].u8TxFrameLockRxSelect[i],i);
            log("TX[0x%02x] set frame lock to %d\r\n",i,Ms1826event[u8dev].u8TxFrameLockRxSelect[i]);
        }
    #endif
#endif
        if(Ms1826event[u8dev].bTxResolutionChangeFlag[i])
        {
            app_ms1826_output_resolution(u8dev,i,Ms1826event[u8dev].u8TxResolutionID[i]);
            log("TX[%d] resolution ID: %d\r\n",i,Ms1826event[u8dev].u8TxResolutionID[i]);
            Ms1826event[u8dev].bTxResolutionChangeFlag[i] = FALSE;
        }
    }

    for(i=0;i<4;i++)
    {
        if(Ms1826event[u8dev].bRxEdidChangeFlag[i])
        {
            Ms1826event[u8dev].bRxEdidChangeFlag[i] = FALSE;
            app_ms1826_rx_edid_update(u8dev,i,Ms1826event[u8dev].u8EdidBuf[i],256);
        }
        if(Ms1826event[u8dev].bOsdStringChangeFlag[i])
        {
            Ms1826event[u8dev].bOsdStringChangeFlag[i] = FALSE;
            log("TX[0x%02x] set string data\r\n",i);
            app_ms1826_display_user_string(u8dev,i);
        }
        if(Ms1826event[u8dev].bVideoMuteChangeFlag[i])
        {
            Ms1826event[u8dev].bVideoMuteChangeFlag[i] = FALSE;
            VIDEOMUTE(u8dev,i,Ms1826event[u8dev].bVideoMuteSta[i]);
        }
        if(Ms1826event[u8dev].bOsdStringDisplayChangeFlag[i])
        {
            Ms1826event[u8dev].bOsdStringDisplayChangeFlag[i] = FALSE;
            log("TX[0x%02x] set string %s\r\n",i,Ms1826event[u8dev].bOsdStringDisplayEnable[i]?"Enable":"Disable");
            app_ms1826_set_display_user_string_enable(u8dev,i,Ms1826event[u8dev].bOsdStringDisplayEnable[i]);
        }
        if(Ms1826event[u8dev].bOsdStringColorChangeFlag[i])
        {
            Ms1826event[u8dev].bOsdStringColorChangeFlag[i] = FALSE;
            log("TX[0x%02x] set string color [%d,%d,%d]\r\n",i,
                    Ms1826event[u8dev].bOsdStringColorRed[i],
                    Ms1826event[u8dev].bOsdStringColorGreen[i],
                    Ms1826event[u8dev].bOsdStringColorBlue[i]);
            app_ms1826_set_user_string_color(u8dev,i,
                    Ms1826event[u8dev].bOsdStringColorRed[i],
                    Ms1826event[u8dev].bOsdStringColorGreen[i],
                    Ms1826event[u8dev].bOsdStringColorBlue[i]);
        }
        if(Ms1826event[u8dev].bOsdStringPositionChangeFlag[i])
        {
            Ms1826event[u8dev].bOsdStringPositionChangeFlag[i] = FALSE;
            log("TX[0x%02x] set string position [%d,%d]\r\n",i,
                    Ms1826event[u8dev].u16OsdStringPositionX[i],
                    Ms1826event[u8dev].u16OsdStringPositionY[i]);
            app_ms1826_set_user_string_position(u8dev,i,
                    Ms1826event[u8dev].u16OsdStringPositionX[i],
                    Ms1826event[u8dev].u16OsdStringPositionY[i]);
        }
    }
    app_ms1826_user_string_display(u8dev);

    if(sl_u8FreezeFlag)
    {
        if(l_u8StartFreezeFlag)
        {
            l_u8StartFreezeFlag = 0;
            _video_freeze_out(u8dev,sl_u8FreezeFlag);
//            if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
//                _video_freeze_out(u8dev,sl_u8FreezeFlag);
//            else if(display_mode[u8dev] == MATRIX_SWITCHER)
//                _video_freeze(u8dev,sl_u8FreezeFlag);
            log("TX[0x%02x] sw freeze\r\n",sl_u8FreezeFlag);
        }

        if(Ms1826event[u8dev].bFreezeTimeEnd)
        {
            Ms1826event[u8dev].bFreezeSwitchFlag[0] = FALSE;
            Ms1826event[u8dev].bFreezeSwitchFlag[1] = FALSE;
            Ms1826event[u8dev].bFreezeSwitchFlag[2] = FALSE;
            Ms1826event[u8dev].bFreezeSwitchFlag[3] = FALSE;
            log("TX[0x%02x] sw unfreeze\r\n",sl_u8FreezeFlag);
            if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
                _video_unfreeze_out(u8dev,sl_u8FreezeFlag);
            else if(display_mode[u8dev] == MATRIX_SWITCHER)
                _video_unfreeze(u8dev,sl_u8FreezeFlag);
            Ms1826event[u8dev].bFreezeTimeEnd = FALSE;
            sl_u8FreezeFlag = 0;
        }
    }
    if(Ms1826event[u8dev].bWorkModeSwitch)
    {
        Ms1826event[u8dev].bWorkModeSwitch = FALSE;
        display_mode_cfg(u8dev,Ms1826event[u8dev].u8WorkMode,Ms1826event[u8dev].u8SubMode);
    }

    if(Ms1826event[u8dev].bSplitterModeChangeFlag)
    {
        Ms1826event[u8dev].bSplitterModeChangeFlag = FALSE;
        app_ms1826_set_splitter_mode(u8dev,Ms1826event[u8dev].u8SplitterMode);
    }

    if(Ms1826event[u8dev].bSplitUpdateChangeFlag)
    {
        Ms1826event[u8dev].bSplitUpdateChangeFlag = FALSE;
        app_ms1826_tx_split_update(u8dev,Ms1826event[u8dev].u8SplitPath);
    }
}

void app_MS1826SetStandby(u8 u8dev,bool bOn)
{
    sg_bMS1826StandbyFlag[u8dev] = bOn;
}

bool app_MS1826GetStandby(u8 u8dev)
{
    return sg_bMS1826StandbyFlag[u8dev];
}

void app_MS1826StandbyProcess(u8 u8dev)
{
    if(g_bPreMS1826SStbFlag[u8dev] != sg_bMS1826StandbyFlag[u8dev])
    {
        u8 i;
        if(sg_bMS1826StandbyFlag[u8dev])
        {
            for(i=0;i<4;i++)
            {
                app_ms1826_tx_output_enable(u8dev,i,FALSE);
                if(g_bDigitalAudioSwByHdmiTx[u8dev][i])
                    Ms1826event[u8dev].u8TxDigitalAudioSource[i] = u8_Tx_audio_sel[u8dev][i];
                app_ms1826_audio_output_mute(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[i],i,TRUE);
            }
        }
        else
        {
            for(i=0;i<4;i++)
            {
                app_ms1826_tx_output_enable(u8dev,i,TRUE);
                if(g_bDigitalAudioSwByHdmiTx[u8dev][i])
                    Ms1826event[u8dev].u8TxDigitalAudioSource[i] = u8_Tx_audio_sel[u8dev][i];
                app_ms1826_audio_output_mute(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[i],i,Ms1826event[u8dev].u8TxDigitalAudioSta[i]?TRUE:FALSE);
            }
        }
        g_bPreMS1826SStbFlag[u8dev] = sg_bMS1826StandbyFlag[u8dev];
    }
}
int ms1826_printfwu(const char *fmt, ...)
{
	  int                 i,iret;
    char                szPrint[1024];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

//    if(!g_bEnableDbgFlag)
//        return 0;

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
    #if (_DEBUG_PORT_SEL==_UART_PORT_1)
        if(pstUart->u8UartPort == _UART_PORT_1)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_2)
        if(pstUart->u8UartPort == _UART_PORT_2)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_3)
        if(pstUart->u8UartPort == _UART_PORT_3)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_4)
        if(pstUart->u8UartPort == _UART_PORT_4)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_5)
        if(pstUart->u8UartPort == _UART_PORT_5)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_6)
        if(pstUart->u8UartPort == _UART_PORT_6)
    #else
        if(pstUart->bIsDebugPort)
    #endif
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }
#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
   #if _ENABLE_UART_1
   if(pstUart->u8UartPort == _UART_PORT_1)
       Uart1_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_2
   if(pstUart->u8UartPort == _UART_PORT_2)
       Uart2_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_3
   if(pstUart->u8UartPort == _UART_PORT_3)
       Uart3_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_4
   if(pstUart->u8UartPort == _UART_PORT_4)
       Uart4_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_5
   if(pstUart->u8UartPort == _UART_PORT_5)
       Uart5_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_6
   if(pstUart->u8UartPort == _UART_PORT_6)
       Uart6_xSemaphoreTask();
   else
   #endif
       UNUSED(pstUart);
#endif

    iret = sprintf((char*)szPrint,"Dev[%d]", g_u8MS1826DevIndex);
    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    va_start(ap, fmt);
//    iret = vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    iret = Uart_vsnprintf(szPrint, 1024, fmt, ap);
    va_end(ap);

//    dbg_printf("Dev[%d]",g_u8MS1826DevIndex);


    for (i = 0; i < iret; i++)
    {
//        dbg_printf("%c",szPrint[i]);
        USARTx_send_bytes(pstUart, (uint8_t *)&szPrint[i], 1);
    }

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    #if _ENABLE_UART_1
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_2
    if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_3
    if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_4
    if(pstUart->u8UartPort == _UART_PORT_4)
        Uart4_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_5
    if(pstUart->u8UartPort == _UART_PORT_5)
        Uart5_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_6
    if(pstUart->u8UartPort == _UART_PORT_6)
        Uart6_xSemaphoreGive();
    else
    #endif
        UNUSED(pstUart);
#endif

    return iret;
}
int ms1826_printf(const char *fmt, ...)
{
    int                 i,iret;
    char                szPrint[1024];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(!g_bEnableDbgFlag)
        return 0;

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
    #if (_DEBUG_PORT_SEL==_UART_PORT_1)
        if(pstUart->u8UartPort == _UART_PORT_1)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_2)
        if(pstUart->u8UartPort == _UART_PORT_2)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_3)
        if(pstUart->u8UartPort == _UART_PORT_3)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_4)
        if(pstUart->u8UartPort == _UART_PORT_4)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_5)
        if(pstUart->u8UartPort == _UART_PORT_5)
    #elif (_DEBUG_PORT_SEL==_UART_PORT_6)
        if(pstUart->u8UartPort == _UART_PORT_6)
    #else
        if(pstUart->bIsDebugPort)
    #endif
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }
#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
   #if _ENABLE_UART_1
   if(pstUart->u8UartPort == _UART_PORT_1)
       Uart1_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_2
   if(pstUart->u8UartPort == _UART_PORT_2)
       Uart2_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_3
   if(pstUart->u8UartPort == _UART_PORT_3)
       Uart3_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_4
   if(pstUart->u8UartPort == _UART_PORT_4)
       Uart4_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_5
   if(pstUart->u8UartPort == _UART_PORT_5)
       Uart5_xSemaphoreTask();
   else
   #endif
   #if _ENABLE_UART_6
   if(pstUart->u8UartPort == _UART_PORT_6)
       Uart6_xSemaphoreTask();
   else
   #endif
       UNUSED(pstUart);
#endif

    iret = sprintf((char*)szPrint,"Dev[%d]", g_u8MS1826DevIndex);
    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    va_start(ap, fmt);
//    iret = vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    iret = Uart_vsnprintf(szPrint, 1024, fmt, ap);
    va_end(ap);

//    dbg_printf("Dev[%d]",g_u8MS1826DevIndex);


    for (i = 0; i < iret; i++)
    {
//        dbg_printf("%c",szPrint[i]);
        USARTx_send_bytes(pstUart, (uint8_t *)&szPrint[i], 1);
    }

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    #if _ENABLE_UART_1
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_2
    if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_3
    if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_4
    if(pstUart->u8UartPort == _UART_PORT_4)
        Uart4_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_5
    if(pstUart->u8UartPort == _UART_PORT_5)
        Uart5_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_6
    if(pstUart->u8UartPort == _UART_PORT_6)
        Uart6_xSemaphoreGive();
    else
    #endif
        UNUSED(pstUart);
#endif

    return iret;
}
#endif  //#if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)

#endif  //#ifndef __MS1826_APP_C

