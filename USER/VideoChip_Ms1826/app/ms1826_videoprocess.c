/*********************************************************************
 * Copyright (c) 2023-2024  PTN Electronics Limited. All rights reserved.
 * System  :
 * FileName: ms1826_videoprocess.c
 * Description:
 * Version :
 * Writer  : wf8421
 * Date    : 2023-08-03 18:23
 * History :
 * Date          Version          Rewriter          Description
**********************************************************************/




#include "includes.h"
#include "ms1826_videoprocess.h"

#ifndef _MS1826_STARTUP_TIME
#define _MS1826_STARTUP_TIME (13000)
#endif

extern BOOL b_osd_menu;
extern UINT8 g_u8_time;
extern MISCTIMING_T __CODE g_arrTimingTable[];
extern BOOL g_dsc_enable[DEV_NUM_MS1826];

static UINT8 const g_u8c_sys_edid_default_buf[256]  =
{
    // Explore Semiconductor,   Inc. EDID   Editor V2
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,     // address 0x00
    0x21, 0x57, 0x36, 0x18, 0xBD, 0xE9, 0x02, 0x00,     //
    0x25, 0x1D, 0x01, 0x03, 0x80, 0x35, 0x1D, 0x78,     // address 0x10
    0x62, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,     //
    0x0F, 0x50, 0x54, 0x21, 0x0F, 0x00, 0x81, 0x00,     // address 0x20
    0x81, 0x40, 0x81, 0x80, 0x90, 0x40, 0x95, 0x00,     //
    0x01, 0x01, 0xA9, 0x40, 0xB3, 0x00, 0x02, 0x3A,     // address 0x30
    0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,     //
    0x55, 0x00, 0xE0, 0x0E, 0x11, 0x00, 0x00, 0x5F,     // address 0x40
    0x0E, 0x1F, 0x00, 0x80, 0x51, 0x00, 0x1E, 0x30,     //
    0x40, 0x80, 0x37, 0x00, 0x0F, 0x48, 0x42, 0x00,     // address 0x50
    0x00, 0x1C, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x17,     //
    0x55, 0x1E, 0x64, 0x1E, 0x00, 0x0A, 0x20, 0x20,     // address 0x60
    0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,     //
    0x00, 0x4D, 0x41, 0x43, 0x52, 0x4F, 0x53, 0x49,     // address 0x70
    0x4C, 0x49, 0x43, 0x4F, 0x4E, 0x0A, 0x01, 0x86,     //
    0x02, 0x03, 0x2E, 0xF1, 0x52, 0x02, 0x11, 0x13,     // address 0x80
    0x84, 0x1F, 0x10, 0x03, 0x12, 0x06, 0x15, 0x07,     //
    0x16, 0x05, 0x14, 0x5E, 0x5F, 0x63, 0x64, 0x23,     // address 0x90
    0x09, 0x7F, 0x07, 0x83, 0x7F, 0x00, 0x00, 0x6E,     //
    0x03, 0x0C, 0x00, 0x10, 0x00, 0x78, 0x3C, 0x20,     // address 0xA0
    0x00, 0x80, 0x01, 0x02, 0x03, 0x04, 0x66, 0x21,     //
    0x50, 0xB0, 0x51, 0x00, 0x1B, 0x30, 0x40, 0x70,     // address 0xB0
    0x36, 0x00, 0x0F, 0x48, 0x42, 0x00, 0x00, 0x1E,     //
    0x66, 0x21, 0x56, 0xAA, 0x51, 0x00, 0x1E, 0x30,     // address 0xC0
    0x46, 0x8F, 0x33, 0x00, 0x0F, 0x48, 0x42, 0x00,     //
    0x00, 0x1E, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0,     // address 0xD0
    0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0x10, 0x09,     //
    0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00,     // address 0xE0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     //
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // address 0xF0
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41,     //
};

/*hdcp1.4   rx kvs and key*/
static UINT8 const g_u8c_arr_hdcp_bksv_buf[5] =
{
    0xc5,
    0xe3,
    0x7e,
    0x1a,
    0x88
};
static UINT8 const g_u8c_arr_hdcp_key_buf[280]  =
{
    0xfd, 0x36, 0x27, 0xdd, 0x61, 0xa8, 0x26,       //1
    0xa4, 0x38, 0x36, 0x6d, 0x8b, 0xc0, 0xec,       //2
    0x53, 0xc6, 0xbc, 0x10, 0xb6, 0xa5, 0x3d,       //3
    0x68, 0x88, 0x63, 0xb0, 0xf6, 0x5d, 0x32,       //4
    0x77, 0x68, 0x68, 0x50, 0x96, 0x22, 0xa2,       //5
    0xb3, 0x21, 0x92, 0xb3, 0xf5, 0xfa, 0xee,       //6
    0x72, 0x78, 0x2a, 0xa9, 0xa5, 0x41, 0xc1,       //7
    0x9f, 0x91, 0x5a, 0x55, 0x45, 0xf0, 0x18,       //8
    0xb0, 0xd8, 0x32, 0xb9, 0xc5, 0x1a, 0x46,       //9
    0x02, 0xa0, 0xe0, 0xdb, 0xf1, 0x77, 0x48,       //10
    0x6f, 0x12, 0xbb, 0xcb, 0x27, 0x40, 0xa6,       //11
    0x2d, 0x80, 0x5b, 0x51, 0x4f, 0x23, 0x71,       //12
    0x83, 0xc6, 0x97, 0x4a, 0xe9, 0x2d, 0x47,       //13
    0xa6, 0xd9, 0x83, 0x03, 0x9f, 0x8c, 0xe1,       //14
    0x15, 0xc4, 0x4d, 0x7f, 0x50, 0x58, 0x20,       //15
    0x25, 0x90, 0xcf, 0x71, 0x1f, 0x4c, 0x67,       //16
    0x53, 0x2e, 0x33, 0xb8, 0xd3, 0xa6, 0x33,       //17
    0x88, 0xa7, 0xd8, 0xe6, 0x60, 0x4c, 0x32,       //18
    0x17, 0x31, 0x3a, 0xf2, 0x2c, 0xae, 0xb5,       //19
    0x46, 0x04, 0xd0, 0xea, 0x74, 0x99, 0x03,       //20
    0xdb, 0x5d, 0x81, 0x5c, 0xbe, 0xdb, 0xc9,       //21
    0x99, 0xd5, 0x89, 0x9e, 0xbe, 0xa8, 0x5a,       //22
    0xd2, 0x79, 0x92, 0x97, 0x90, 0xfd, 0xbf,       //23
    0x06, 0x16, 0xd2, 0xae, 0x96, 0xdb, 0x40,       //24
    0xed, 0xf3, 0xe0, 0x85, 0x90, 0xa0, 0x32,       //25
    0x08, 0x7b, 0xeb, 0xff, 0x8f, 0xef, 0x48,       //26
    0x9e, 0x7a, 0x34, 0x54, 0x4b, 0x43, 0x48,       //27
    0xf5, 0x86, 0xd7, 0x23, 0xde, 0x09, 0x38,       //28
    0x4a, 0x65, 0x23, 0x44, 0xd2, 0x32, 0x60,       //29
    0x71, 0xa1, 0x95, 0xad, 0x81, 0xba, 0xb7,       //30
    0x74, 0xd5, 0x66, 0x75, 0xfa, 0xf6, 0xc9,       //31
    0xe5, 0x8f, 0x06, 0xa9, 0xb0, 0xe1, 0xef,       //32
    0x38, 0xa5, 0x73, 0x91, 0xa8, 0x35, 0xdd,       //33
    0x38, 0xa8, 0x14, 0x1b, 0xd1, 0x7a, 0x5d,       //34
    0xbf, 0xa1, 0xe4, 0x51, 0xd0, 0xba, 0xaf,       //35
    0x66, 0x47, 0x1e, 0x28, 0x46, 0xfb, 0xcb,       //36
    0x73, 0x07, 0xe7, 0xe7, 0xfa, 0x59, 0x25,       //37
    0x26, 0x94, 0xc0, 0x50, 0x3a, 0x54, 0x73,       //38
    0x72, 0x13, 0xf9, 0x0c, 0xf3, 0x1a, 0xcb,       //39
    0x53, 0xe1, 0x77, 0xfa, 0x1d, 0x78, 0xcf        //40
};

static UINT8 const u8_tx_ksv_buf[5] =
{
    //##write the Bksv to Tx, and calculate the Km at the same time
    0xaa,

};

static UINT8 const u8_tx_key_buf[280] =
{
    //##write the Bksv to Tx, and calculate the Km at the same time
    0xaa,
};

UINT8 u8_rx_change_bak[DEV_NUM_MS1826][4] = {0};
BOOL b_hpd_bakes[DEV_NUM_MS1826][4] = {{FALSE}};
extern UINT8 g_u8DigitalAudioSta[DEV_NUM_MS1826][4];
BOOL g_bFirstStartupFlag[DEV_NUM_MS1826][4] = {{FALSE}};
UINT16 g_u16Startup2InitTimer[DEV_NUM_MS1826][4] = {{0}};
UINT8 gu8ChipAddr[DEV_NUM_MS1826] = {0xB6};

extern UINT16 ms1826_hal_hdmi_rx_get_tmds_clk(UINT8 u8_idx);
extern UINT32 ms1826_hal_hdmi_rx_get_mdt_ints_ext(UINT8 u8_idx);
extern UINT8 ms1826_hal_hdmi_rx_phy_process(UINT8 u8_idx, BOOL b_rx_tmds_clk_change, UINT16 u16_rx_tmds_clk);
extern VOID ms1826_hal_hdmi_rx_audio_hbr_pll_cfg(UINT8 u8_idx, BOOL b_is_hbr_aud_input);
extern VOID ms1826_hal_hdmi_rx_audio_fifo_reset(UINT8 u8_idx, BOOL b_reset);
extern VOID ms1826_hal_hdmi_rx_hbr_en(UINT8 u8_idx, BOOL b_is_hbr_aud_input);
extern VOID ms1826_hal_vehc_trig(UINT8 u8_vehc_id, BOOL b_imm);

static VOID _ms1826_init(UINT8 u8dev)
{
    UINT8 u8_ddr_status;
    //DDR INIT.
    UINT8 u8_chip_addr = 0;
    u8_chip_addr = api_misc_get_i2c_addr();
    gu8ChipAddr[u8dev] = u8_chip_addr;
    log("[DEBUG] MS1826 Chip Address: 0x%02x\r\n", u8_chip_addr);
    log("[DEBUG] MS1826 Sdk Version: %s\r\n", api_get_sdk_version());

    api_misc_set_i2c_addr(u8_chip_addr);
    DMC_T st_dmc_cfg;
    PLL_DEV_T pll_dev;
#if SEAMLESS_MATRIX_USE_DSC
    if(display_mode[u8dev] == MATRIX_SWITCHER
        ||display_mode[u8dev] == MATRIX_SWITCHER_OUT)
        g_dsc_enable[u8dev] = MS_TRUE;
    else
#endif

        g_dsc_enable[u8dev] = false;
    if(g_dsc_enable[u8dev])
    {
        pll_dev.u32_clk_out = 33325 * 10000;
        pll_dev.u8_pll_idx = PLLM;
        pll_dev.b_osc_input = MS_TRUE;
        api_pll_config(&pll_dev);

        st_dmc_cfg.u32_sdram_spdbin = SDRAM_SPDBIN_1333M;
        st_dmc_cfg.u32_apb_freq = 24;
        u8_ddr_status = api_dmc_init(&st_dmc_cfg);
    }
    else
    {
		if((u8dev==0)||(u8dev==1))          pll_dev.u32_clk_out = 46650 * 10000;

        else pll_dev.u32_clk_out = 46650 * 10000;

        pll_dev.u8_pll_idx = PLLM;
        pll_dev.b_osc_input = MS_TRUE;
        api_pll_config(&pll_dev);

        st_dmc_cfg.u32_sdram_spdbin = 1866;
        st_dmc_cfg.u32_apb_freq = 24;
        u8_ddr_status = api_dmc_init(&st_dmc_cfg);
    }
    switch(u8_ddr_status)
    {
        case MS_ERR_OK:
            break;
        case MS_ERR_FAIL:
            break;
        case MS_ERR_INV_PARM:
            break;
    }
    api_misc_timing_table_init((MISC_TIMING_T *)g_arrTimingTable, get_timing_table_size());
    g_bFirstStartupFlag[u8dev][0] = TRUE;
    g_bFirstStartupFlag[u8dev][1] = TRUE;
    g_bFirstStartupFlag[u8dev][2] = TRUE;
    g_bFirstStartupFlag[u8dev][3] = TRUE;

    g_u16Startup2InitTimer[u8dev][0] = _MS1826_STARTUP_TIME;
    g_u16Startup2InitTimer[u8dev][1] = _MS1826_STARTUP_TIME;
    g_u16Startup2InitTimer[u8dev][2] = _MS1826_STARTUP_TIME;
    g_u16Startup2InitTimer[u8dev][3] = _MS1826_STARTUP_TIME;
}

static VOID _video_port_hdmi_rx_init(UINT8 u8dev)
{
    UINT8 u8_rx_id = 0;
    for(u8_rx_id = 0; u8_rx_id < 4; u8_rx_id ++)
    {
        api_hdmi_rx_instance_init(&g_st_hdmi_rx[u8dev][u8_rx_id], g_u8_HDMI_RX_PROT[u8_rx_id]);
        if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
        {
            api_misc_timing_get(VFMT_CEA_16_1920x1080P_60HZ,&Video_HDMIRx_Timing[u8dev][u8_rx_id]);
            api_misc_timing_get(VFMT_CEA_16_1920x1080P_60HZ,&g_st_hdmi_rx[u8dev][u8_rx_id].st_rx_video_timing);
        }
        else
        {
            memset(&Video_HDMIRx_Timing[u8dev][u8_rx_id],NULL,sizeof(HDMI_VIDEO_TIMING_T));
        }
        g_st_hdmi_rx[u8dev][u8_rx_id].idx = g_u8_HDMI_RX_PROT[u8_rx_id];
        g_st_hdmi_rx[u8dev][u8_rx_id].b_hdcp1_4_enable  = MS_TRUE;
        if(System_Default_Edid_table != NULL)
        {
            g_st_hdmi_rx[u8dev][u8_rx_id].pu8_edid_buf  = (VOID *)System_Default_Edid_table;
            g_st_hdmi_rx[u8dev][u8_rx_id].u16_edid_size = 256;
        }
        else
        {
            g_st_hdmi_rx[u8dev][u8_rx_id].pu8_edid_buf  = (VOID *)g_u8c_sys_edid_default_buf;
            g_st_hdmi_rx[u8dev][u8_rx_id].u16_edid_size = sizeof(g_u8c_sys_edid_default_buf);
        }
        g_st_hdmi_rx[u8dev][u8_rx_id].pu8_bksv_buf  = (UINT8 *)g_u8c_arr_hdcp_bksv_buf;
        g_st_hdmi_rx[u8dev][u8_rx_id].pu8_key_buf = (UINT8 *)g_u8c_arr_hdcp_key_buf;
        g_st_hdmi_rx[u8dev][u8_rx_id].b_hdp_override_en = MS_TRUE;
        g_st_hdmi_rx[u8dev][u8_rx_id].b_timing_stable = false;
        api_hdmi_rx_init(&g_st_hdmi_rx[u8dev][u8_rx_id]);

        api_hdmi_rx_set_hpd(&g_st_hdmi_rx[u8dev][u8_rx_id],TRUE);
    }
}

static VOID _tx_csc_cfg(UINT8 u8dev,UINT8 u8_txcsc_id)
{
    g_st_txcsc_dev[u8dev][u8_txcsc_id].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8_txcsc_id]];
//    api_csc_tx_instance_init(&g_st_txcsc_dev[u8dev][u8_txcsc_id], u8_txcsc_id);
    api_csc_tx_instance_init(&g_st_txcsc_dev[u8dev][u8_txcsc_id], g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8_txcsc_id]]);
    g_st_txcsc_dev[u8dev][u8_txcsc_id].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
    g_st_txcsc_dev[u8dev][u8_txcsc_id].st_csc_config_param.csc_out_color_range  = CHIP_TX_CSC_OUT_COLORRANGE;
    g_st_txcsc_dev[u8dev][u8_txcsc_id].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
    g_st_txcsc_dev[u8dev][u8_txcsc_id].st_csc_config_param.csc_in_colorspace =  CHIP_INTERNAL_PROCESS_COLORSPACE;
    g_st_txcsc_dev[u8dev][u8_txcsc_id].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
    api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8_txcsc_id]);
}

VOID _video_timgen_cfg(UINT8 u8dev,UINT8 u8_timgen_id)
{
    MISC_MODULE_CLK_T st_tmg_src;
    PLL_DEV_T pll_dev;
    MISC_MODULE_CLK_T st_module_clk;

    api_misc_timing_get(g_arrOutputTable[g_u8_output_index[u8dev][u8_timgen_id]],&g_stVideo_HDMITx_Timing[u8dev][u8_timgen_id]);

    st_module_clk.u8_clkdiv = MISC_CLKDIV_1;
    st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_PLLV1 + u8_timgen_id ;
    st_module_clk.u8_src_clk = MISC_CLKSRC_XTAL;
    api_misc_pllv_clkin_sel(&st_module_clk);

    pll_dev.u8_pll_idx = PLLV1 + u8_timgen_id;
    pll_dev.b_osc_input = MS_TRUE;
    pll_dev.u32_clk_out = g_stVideo_HDMITx_Timing[u8dev][u8_timgen_id].u16_pixclk * 10000;
    api_pll_config(&pll_dev);

    st_tmg_src.u8_src_clk = MISC_CLKSRC_PLLV1 + u8_timgen_id;
    st_tmg_src.u8_dst_module = g_u8_misc_timgen[u8_timgen_id];
    api_misc_timgen_clkin_sel(&st_tmg_src);

    api_timgen_instance_init(&g_st_timgen[u8dev][u8_timgen_id], u8_timgen_id);
    g_st_timgen[u8dev][u8_timgen_id].u8_workmode =  FreeRun_Mode;
    g_st_timgen[u8dev][u8_timgen_id].u8_timgen_id = u8_timgen_id;
    g_st_timgen[u8dev][u8_timgen_id].b_timgen_en =  MS_ENABLE;
    g_st_timgen[u8dev][u8_timgen_id].pst_timing = &g_stVideo_HDMITx_Timing[u8dev][u8_timgen_id];

    api_vds_tmg_init(&g_st_timgen[u8dev][u8_timgen_id]);
    api_vds_tmg_config(&g_st_timgen[u8dev][u8_timgen_id]);
}

#if ENABLE_MS1826_FRAME_LOCK
/***********************************************************************
 * FunName : _video_timgen_cfg_frame_lock
 * Writer  : wf8421
 * Date    : 2023-08-03 22:02
 * Description: For timing generator to lock rx clock
 * Parameter: u8_idx: select lock rx;
 *            u8_timgen_id: To lock timing generator
 * Return  : NONE
 * History :
 ***********************************************************************/
VOID _video_timgen_cfg_frame_lock(UINT8 u8dev,UINT8 u8_idx, UINT8 u8_timgen_id)
{
	MISC_MODULE_CLK_T st_tmg_src;
	PLL_DEV_T pll_dev;
	MISC_MODULE_CLK_T st_module_clk;

    log("Timgen %d lock rx %d\r\n",u8_timgen_id,u8_idx);
	api_misc_timing_get(g_arrOutputTable[g_u8_output_index[u8dev][u8_timgen_id]],&g_stVideo_HDMITx_Timing[u8dev][u8_timgen_id]);

	st_module_clk.u8_clkdiv	= MISC_CLKDIV_1;
	st_module_clk.u8_dst_module	= MISC_VIDEO_MODULE_PLLV1 + u8_timgen_id ;
	st_module_clk.u8_src_clk = MISC_CLKSRC_RX1 + u8_idx;		//freerun模式选择 MISC_CLKSRC_XTAL
	api_misc_pllv_clkin_sel(&st_module_clk);

	pll_dev.u8_pll_idx =	PLLV1 + u8_timgen_id;
	pll_dev.b_osc_input = false;				//freerun模式选择晶振输入 MS_TRUE
	pll_dev.u32_clk_in = Video_HDMIRx_Timing[u8dev][u8_idx].u16_pixclk * 10000;	//freerun模式选择晶振输入 无需配置，默认为24M
	pll_dev.u32_clk_out = g_stVideo_HDMITx_Timing[u8dev][u8_timgen_id].u16_pixclk * 10000;
	api_pll_config(&pll_dev);

	st_tmg_src.u8_src_clk =	MISC_CLKSRC_PLLV1 + u8_timgen_id;
	st_tmg_src.u8_dst_module = g_u8_misc_timgen[u8_timgen_id];
	api_misc_timgen_clkin_sel(&st_tmg_src);

	api_timgen_instance_init(&g_st_timgen[u8dev][u8_timgen_id], u8_timgen_id);
	g_st_timgen[u8dev][u8_timgen_id].u8_workmode =	Casecade_Mode;		//freerun模式选择 FreeRun_Mode
	g_st_timgen[u8dev][u8_timgen_id].u8_timgen_id = u8_timgen_id;
	g_st_timgen[u8dev][u8_timgen_id].b_timgen_en =	MS_ENABLE;
	g_st_timgen[u8dev][u8_timgen_id].pst_timing = &g_stVideo_HDMITx_Timing[u8dev][u8_timgen_id];

	api_vds_tmg_init(&g_st_timgen[u8dev][u8_timgen_id]);
	api_vds_tmg_config(&g_st_timgen[u8dev][u8_timgen_id]);
}
#endif

VOID _video_port_hdmi_tx_driver_config(UINT8 u8dev,u8 tx_id)
{
#if MS1826_TX_DRIVER_USER_CONFIG_ENABLE
//    HDMI_TX_DRI_PRE_PRE_EMPHASIS_T st_tx_driver_pre_emphasis;
    HDMI_TX_VOLT_LEVEL_T st_tx_volt_level;

//    st_tx_driver_pre_emphasis.u8_txIndex = tx_id;
//    st_tx_driver_pre_emphasis.u8_tx_symon = 1;
//    st_tx_driver_pre_emphasis.u8_tx_traon = 1;
//    st_tx_driver_pre_emphasis.u8_tx_trbon = 1;
//    api_hdmi_tx_set_driver_pre_emphasis(&st_tx_driver_pre_emphasis);

    st_tx_volt_level.u8_txIndex = tx_id;
    st_tx_volt_level.u8_sup_tx_lvl = MS1826_DRIVER_VALUE_TX_LEVEL;
    st_tx_volt_level.u8_sup_ck_lvl = MS1826_DRIVER_VALUE_CLOCK_LEVEL;
    api_hdmi_tx_set_driver_volt_level(&st_tx_volt_level);
#endif
    UNUSED(tx_id);
    UNUSED(u8dev);
}
VOID _video_port_hdmi_tx_init(UINT8 u8dev)
{
    UINT8 tx_id = 0;
    gst_inf_pack[u8dev].st_avi_infoframe.u8_pixel_repeat =  HDMI_PIXEL_REPEAT_X1;
    gst_inf_pack[u8dev].st_avi_infoframe.e_scan_mode =  HDMI_SCAN_MODE_OVERSCAN;
    gst_inf_pack[u8dev].st_avi_infoframe.e_picture_aspect = HDMI_PICTURE_ASPECT_16_9;
    gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
    if(gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace == HDMI_COLORSPACE_RGB)
        gst_inf_pack[u8dev].st_avi_infoframe.e_colorimetry  = HDMI_COLORIMETRY_NONE;
    if(gst_inf_pack[u8dev].st_avi_infoframe.e_colorspace == HDMI_COLORSPACE_YCBCR444)
        gst_inf_pack[u8dev].st_avi_infoframe.e_colorimetry  = HDMI_COLORIMETRY_ITU_601;
    gst_inf_pack[u8dev].st_avi_infoframe.e_extended_colorimetry = HDMI_EXTENDED_COLORIMETRY_OPRGB;
    for(tx_id = 0; tx_id < 4; tx_id ++)
    {
        api_hdmi_tx_instance_init(&g_st_hdmi_tx[u8dev][tx_id], g_u8_HDMI_TX_PROT[tx_id]);
        api_misc_timing_get(g_arrOutputTable[g_u8_output_index[u8dev][tx_id]],&g_stVideo_HDMITx_Timing[u8dev][tx_id]);
        g_st_hdmi_tx[u8dev][tx_id].u8_txIndex =  g_u8_HDMI_TX_PROT[tx_id];
        g_st_hdmi_tx[u8dev][tx_id].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][tx_id];
        g_st_hdmi_tx[u8dev][tx_id].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][tx_id].u8_color_depth =  HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][tx_id].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
        g_st_hdmi_tx[u8dev][tx_id].u8_audio_src  = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][tx_id].u8_video_clk_src  = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][tx_id].u32_tmds_clk  = g_stVideo_HDMITx_Timing[u8dev][tx_id].u16_pixclk;
        g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf  = (VOID *)hdmi_tx_edid[u8dev][tx_id];
        g_st_hdmi_tx[u8dev][tx_id].pu8_bksv_buf  = (UINT8 *)u8_tx_ksv_buf;
        g_st_hdmi_tx[u8dev][tx_id].pu8_key_buf = (UINT8 *)u8_tx_key_buf;
        g_st_hdmi_tx[u8dev][tx_id].pst_edid_info = (VOID *)&g_hdmi_edid_flag[u8dev][tx_id];
        api_hdmi_tx_init(&g_st_hdmi_tx[u8dev][tx_id]);
        memcpy((&g_st_hdmi_tx[u8dev][tx_id].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][tx_id].st_infoframes_packets.st_avi_infoframe.u8_video_code = g_arrOutputTable[g_u8_output_index[u8dev][tx_id]];
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][tx_id],false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][tx_id]);
        _video_port_hdmi_tx_driver_config(u8dev,tx_id);
        _tx_csc_cfg(u8dev,tx_id);

        g_st_tx_mux[u8dev][tx_id].u8_tx_module = MISC_VIDEO_MODULE_TX1 + tx_id;
        g_st_tx_mux[u8dev][tx_id].u8_video_mux = MISC_VIDEO_MODULE_MIX1  + tx_id;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][tx_id]);
        api_vehc_instance_init(&g_st_vovehc[u8dev][tx_id], tx_id);
        app_bcsh_adjust(u8dev,tx_id);
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][tx_id], MS_TRUE);
    }

}

VOID _digital_audio_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute)
{
    UINT8 tx_id;
    for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
    {
        if(Mapp_Rx[u8dev][tx_id] == u8_path)
        {
            if(g_u8DigitalAudioSta[u8dev][Mapp_Rx[u8dev][tx_id]]!=((UINT8)b_mute))
            {
                g_u8DigitalAudioSta[u8dev][Mapp_Rx[u8dev][tx_id]] = ((UINT8)b_mute);
                if(b_mute)
                {
                    if(g_bDigitalAudioSwByHdmiTx[u8dev][tx_id])
                        Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id] = u8_Tx_audio_sel[u8dev][tx_id];
                    app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_i2s_idx[tx_id], FALSE);
                    app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_spdif_idx[tx_id], FALSE);
	
                }
                else
                {
                    if(g_bDigitalAudioSwByHdmiTx[u8dev][tx_id])
                        Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id] = u8_Tx_audio_sel[u8dev][tx_id];
                    app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_i2s_idx[tx_id], b_digital_audio_mute[u8dev][tx_id]?FALSE:TRUE);
                    app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_spdif_idx[tx_id], b_digital_audio_mute[u8dev][tx_id]?FALSE:TRUE);
                }
            }
        }
    }
}

VOID VIDEOMUTE(UINT8 u8dev,UINT8 u8_path, BOOL b_mute)
{
    UINT8 tx_id;
//    log("VIDEOMUTE[%d] sta:%d\r\n",u8_path,b_mute);
    if(display_mode[u8dev]== MATRIX_SWITCHER_OUT)
        _digital_audio_mute(u8dev,u8_path,b_mute);
    switch(display_mode[u8dev])
    {
        case MATRIX_SWITCHER:
        case MATRIX_SWITCHER_OUT:
            for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
            {
                if(Mapp_Rx[u8dev][tx_id] == u8_path)
                {
                    if(b_freeze_status[u8dev][tx_id]==FALSE)    //add by wf8421 20230523
                    {
                        single_video_mute(u8dev,tx_id, b_mute);
                    }
                }
            }
            break;
        case SPLITTER:
            for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
            {
                if(g_u8SplitterDispMapping[u8dev][tx_id] == u8_path)
                {
                    splitter_video_mute(u8dev,tx_id, b_mute);
                }
            }
            break;
        case SPLICER:
            splicer_video_mute(u8dev,u8_path, b_mute);
            break;
    }
}

VOID video_process_init(UINT8 u8dev)
{
    b_hpd_bak[u8dev][0] = FALSE;
    b_hpd_bak[u8dev][1] = FALSE;
    b_hpd_bak[u8dev][2] = FALSE;
    b_hpd_bak[u8dev][3] = FALSE;
    _ms1826_init(u8dev);
    _video_port_hdmi_rx_init(u8dev);
    _video_timgen_cfg(u8dev,TIMGEN_1);
    _video_timgen_cfg(u8dev,TIMGEN_2);
    _video_timgen_cfg(u8dev,TIMGEN_3);
    _video_timgen_cfg(u8dev,TIMGEN_4);
    mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0x0f);   
    mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0xff);   
    g_st_app_splitter_dev[u8dev].u8_dst_sel = 0;
    g_st_app_splitter_dev[u8dev].b_splitter_sw  = false;
    gst_splicer_app_dev[u8dev].b_splicer_sw = false;
    gst_splicer_app_dev[u8dev].u8_tx_mirror = false;
    switch(display_mode[u8dev])
    {
        case MATRIX_SWITCHER:
        case MATRIX_SWITCHER_OUT:
            app_single_mode_config(u8dev);
            if(display_mode[u8dev] == MATRIX_SWITCHER)
                log("[INFO] MS1826 function is SEAMLESS_MATRIX_SWITCHER.\r\n");
            else
                log("[INFO] MS1826 function is SEAMLESS_MATRIX_SWITCHER_OUT.\r\n");

            app_single_mode_rx_switch(u8dev,0, 0);
            app_single_mode_rx_switch(u8dev,1, 1);
            app_single_mode_rx_switch(u8dev,2, 2);
            app_single_mode_rx_switch(u8dev,3, 3);
            break;
        case SPLITTER:
            app_splitter_mode_config(u8dev,&g_st_app_splitter_dev[u8dev]);
            log("[INFO] MS1826 function is VIDEO_SPLITTER.\r\n");
            break;
        case SPLICER:
            app_splicer_mode_config(u8dev,&gst_splicer_app_dev[u8dev]);
            log("[INFO] MS1826 function is VIDEO_SPLICER.\r\n");
            break;
        case MS1826_BYPASS_MATRIX_SWITCHER:
            Bypass_Tx_Status[u8dev][0]  = 1;
            Bypass_Tx_Status[u8dev][1]  = 1;
            Bypass_Tx_Status[u8dev][2]  = 1;
            Bypass_Tx_Status[u8dev][3]  = 1;
            log("[INFO] MS1826 function is BYPASS_MATRIX_SWITCHER.\r\n");
            break;
    }
    VIDEOMUTE(u8dev,HDMI_RX_A, MS_TRUE);
    VIDEOMUTE(u8dev,HDMI_RX_B, MS_TRUE);
    VIDEOMUTE(u8dev,HDMI_RX_C, MS_TRUE);
    VIDEOMUTE(u8dev,HDMI_RX_D, MS_TRUE);
	_video_port_hdmi_tx_init(u8dev);
}

VOID video_process_cfg(UINT8 u8dev)
{
    _ms1826_init(u8dev);
    _video_port_hdmi_rx_init(u8dev);
    _video_timgen_cfg(u8dev,TIMGEN_1);
    _video_timgen_cfg(u8dev,TIMGEN_2);
    _video_timgen_cfg(u8dev,TIMGEN_3);
    _video_timgen_cfg(u8dev,TIMGEN_4);
	mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0x0f);	
	mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x34,0xff);	
    app_osd_init(u8dev);
    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
    _video_port_hdmi_tx_init(u8dev);
}

VOID _video_misc_process(UINT8 u8dev,UINT8 u8_rx_id)
{
    UINT8 u8_vipss_id = 0;
    UINT8 u8_vipss_mapping = 0;
    BOOL  bSetVipssCscFlag = false;

    //require positive polarity input
    g_st_rx_polarity[u8dev][u8_rx_id].u8_chan = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[u8_rx_id]];
    switch(Video_HDMIRx_Timing[u8dev][u8_rx_id].u8_polarity)
    {
        case ProgrVNegHNeg:
        case InterVNegHNeg:
            g_st_rx_polarity[u8dev][u8_rx_id].u8_hsync_flip_en  = MS_TRUE;
            g_st_rx_polarity[u8dev][u8_rx_id].u8_vsync_flip_en  = MS_TRUE;
            break;
        case ProgrVNegHPos:
        case InterVNegHPos:
            g_st_rx_polarity[u8dev][u8_rx_id].u8_hsync_flip_en  = false;
            g_st_rx_polarity[u8dev][u8_rx_id].u8_vsync_flip_en  = MS_TRUE;
            break;
        case ProgrVPosHNeg:
        case InterVPosHNeg:
            g_st_rx_polarity[u8dev][u8_rx_id].u8_hsync_flip_en  = MS_TRUE;
            g_st_rx_polarity[u8dev][u8_rx_id].u8_vsync_flip_en  = false;
            break;
        case ProgrVPosHPos:
        case InterVPosHPos:
            g_st_rx_polarity[u8dev][u8_rx_id].u8_hsync_flip_en  = false;
            g_st_rx_polarity[u8dev][u8_rx_id].u8_vsync_flip_en  = false;
            break;
    }
    api_misc_rx_polarity_cfg(&g_st_rx_polarity[u8dev][u8_rx_id]);

    api_hdmi_rx_get_avi_infoframe(&g_st_hdmi_rx[u8dev][u8_rx_id]);

    for(u8_vipss_id = 0; u8_vipss_id < 4; u8_vipss_id ++)
    {
        if(display_mode[u8dev] == MATRIX_SWITCHER
            ||display_mode[u8dev] == MATRIX_SWITCHER_OUT)
            u8_vipss_mapping = u8_rx_id;
        else
            u8_vipss_mapping = g_u8_HDMI_RX_PROT[u8_rx_id];

        // modify by wf8421 20231009
        bSetVipssCscFlag = false;
        switch(display_mode[u8dev])
        {
            case MATRIX_SWITCHER:
            case MATRIX_SWITCHER_OUT:
                {
                    if(Mapp_Rx[u8dev][u8_vipss_id] == u8_vipss_mapping)
                        bSetVipssCscFlag = MS_TRUE;
                }
                break;
            case SPLITTER:
                {
                    if(g_u8SplitterDispMapping[u8dev][u8_vipss_id] == u8_vipss_mapping)
                        bSetVipssCscFlag = MS_TRUE;
                }
                break;
            case SPLICER:
                {
                    if(Mapp_Rx[u8dev][u8_vipss_id] == u8_vipss_mapping)
                        bSetVipssCscFlag = MS_TRUE;
                }
                break;
        }
        if(bSetVipssCscFlag)
        {
            g_st_vipss_csc[u8dev][u8_vipss_id].u8_sel = g_u8_csc_idx[u8_vipss_id];
            g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_colorspace    =
                g_st_hdmi_rx[u8dev][u8_rx_id].st_packets_infoframes.st_avi_infoframe.e_colorspace;
            if(g_st_hdmi_rx[u8dev][u8_rx_id].st_packets_infoframes.st_avi_infoframe.e_colorspace == HDMI_COLORSPACE_RGB)
            {
                if(g_st_hdmi_rx[u8dev][u8_rx_id].st_packets_infoframes.st_avi_infoframe.e_rgb_quantization_range <  HDMI_QUANTIZATION_RANGE_FULL)
                {
                    g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_color_range = CSC_RGB_16_235;
                }
                else
                {
                    g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_color_range = CSC_RGB_0_255;
                }
            }
            else if(g_st_hdmi_rx[u8dev][u8_rx_id].st_packets_infoframes.st_avi_infoframe.e_colorspace > HDMI_COLORSPACE_RGB &&
                    g_st_hdmi_rx[u8dev][u8_rx_id].st_packets_infoframes.st_avi_infoframe.e_colorspace < HDMI_COLORSPACE_RESERVED4)
            {
                if(g_st_hdmi_rx[u8dev][u8_rx_id].st_packets_infoframes.st_avi_infoframe.e_colorimetry == HDMI_COLORIMETRY_ITU_709)
                {
                    g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_color_range = CSC_YUV444_BT709;
                }
                else
                {
                    g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_color_range = CSC_YUV444_BT601;
                }
            }
            if(g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_colorspace == HDMI_COLORSPACE_YCBCR420)
            {
                g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
                g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
            }
            if(g_st_hdmi_rx[u8dev][u8_rx_id].b_is_hdmi == false)
			    g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_in_colorspace = HDMI_COLORSPACE_RGB;
            g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
            g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.csc_out_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
            g_st_vipss_csc[u8dev][u8_vipss_id].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
            api_csc_rx_set(&g_st_vipss_csc[u8dev][u8_vipss_id]);
            ms1826_hal_vehc_trig(u8_vipss_id+VEHC_VIPSS_1,MS_TRUE);     
        }
    }
}


static BOOL _diff_compare(UINT16 a, UINT16 b)
{
    UINT16 u16diff = (a > b) ? a - b : b - a;
    return (u16diff < 10)?TRUE:FALSE;
}

static BOOL _timing_compare(HDMI_VIDEO_TIMING_T *pstTiming1, HDMI_VIDEO_TIMING_T *pstTiming2)
{
    if (pstTiming1->u8_polarity == pstTiming2->u8_polarity &&
        _diff_compare(pstTiming1->u16_htotal,   pstTiming2->u16_htotal) &&
        _diff_compare(pstTiming1->u16_vtotal,   pstTiming2->u16_vtotal) &&
        _diff_compare(pstTiming1->u16_hactive, pstTiming2->u16_hactive) &&
        _diff_compare(pstTiming1->u16_vactive, pstTiming2->u16_vactive) &&
        _diff_compare(pstTiming1->u16_pixclk,   pstTiming2->u16_pixclk) &&
        _diff_compare(pstTiming1->u16_vfreq, pstTiming2->u16_vfreq) &&
        _diff_compare(pstTiming1->u16_hoffset, pstTiming2->u16_hoffset) &&
        _diff_compare(pstTiming1->u16_voffset, pstTiming2->u16_voffset) &&
        _diff_compare(pstTiming1->u16_hsyncwidth,   pstTiming2->u16_hsyncwidth) &&
        _diff_compare(pstTiming1->u16_vsyncwidth,   pstTiming2->u16_vsyncwidth))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

static VOID _video_tx_process(UINT8 u8dev)
{
    BOOL b_hpd = FALSE;
    UINT8 tx_id = 0;

    for(tx_id = 0; tx_id < 4; tx_id ++)
    {
        b_hpd = api_hdmi_tx_get_hpd_status(&g_st_hdmi_tx[u8dev][tx_id]);
        if(b_hpd != b_hpd_bak[u8dev][tx_id])
        {
            b_hpd_bak[u8dev][tx_id]  = b_hpd;
            if(b_hpd)
            {
                switch(tx_id)
                {
                    case 0:
                        api_hdmi_tx_get_edid(&g_st_hdmi_tx[u8dev][tx_id],g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,&g_hdmi_edid_flag[u8dev][tx_id],FALSE);
                        break;
                    case 1:
                        api_hdmi_tx_get_edid(&g_st_hdmi_tx[u8dev][tx_id],g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,&g_hdmi_edid_flag[u8dev][tx_id],FALSE);
                        break;
                    case 2:
                        api_hdmi_tx_get_edid(&g_st_hdmi_tx[u8dev][tx_id],g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,&g_hdmi_edid_flag[u8dev][tx_id],FALSE);
                        break;
                    case 3:
                        api_hdmi_tx_get_edid(&g_st_hdmi_tx[u8dev][tx_id],g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,&g_hdmi_edid_flag[u8dev][tx_id],FALSE);
                        break;
                }
            }
            else
            {
                switch(tx_id)
                {
                    case 0:
                        memset(g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,0,256);
                        break;
                    case 1:
                        memset(g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,0,256);
                        break;
                    case 2:
                        memset(g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,0,256);
                        break;
                    case 3:
                        memset(g_st_hdmi_tx[u8dev][tx_id].pu8_edid_buf,0,256);
                        break;
                }
            }
        }
    }
}

static VOID _video_process(UINT8 u8dev,UINT8 u8_idx)
{
    UINT8 tx_id = 0;


    api_hdmi_rx_task(&g_st_hdmi_rx[u8dev][u8_idx]);
    if(api_hdmi_rx_is_video_changed(&g_st_hdmi_rx[u8dev][u8_idx]))
    {
        VIDEOMUTE(u8dev,u8_idx, TRUE);
        if(api_hdmi_rx_get_video_timing(&g_st_hdmi_rx[u8dev][u8_idx]) == MS_ERR_OK)
        {
            g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable = TRUE;

            if(_timing_compare(&Video_HDMIRx_Timing[u8dev][u8_idx],  &(g_st_hdmi_rx[u8dev][u8_idx].st_rx_video_timing)) == FALSE)
            {
                memcpy(&Video_HDMIRx_Timing[u8dev][u8_idx],&(g_st_hdmi_rx[u8dev][u8_idx].st_rx_video_timing),sizeof(HDMI_VIDEO_TIMING_T));
                if(Video_HDMIRx_Timing[u8dev][u8_idx].u8_polarity % 2 == 0)
                {
                    Video_HDMIRx_Timing[u8dev][u8_idx].u16_vactive /= 2;
                }
                b_RxVideoChg[u8dev][u8_idx] = TRUE;
            }
            else
            {
                if(b_RxVideoChg[u8dev][u8_idx] == false)
                {
                    b_RxVideoChg[u8dev][u8_idx] = TRUE;
                }
            }
        }
        else
        {
            memset(&Video_HDMIRx_Timing[u8dev][u8_idx],NULL,sizeof(HDMI_VIDEO_TIMING_T));
            g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable = FALSE;
        }
        OSDShown[u8dev] = MS_TRUE;
        sys_show_osd(u8dev,OSDShown[u8dev], 1<<u8_idx);
        _video_misc_process(u8dev,u8_idx);
    }
    else
    {
        if(g_st_hdmi_rx[u8dev][u8_idx].u8_rx_state == RX_STATE_RUNNING && b_RxVideoChg[u8dev][u8_idx])
        {
            b_RxVideoChg[u8dev][u8_idx] = FALSE;
            switch(display_mode[u8dev])
            {
                case MATRIX_SWITCHER:
                case MATRIX_SWITCHER_OUT:
                    for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
                    {
                        if(Mapp_Rx[u8dev][tx_id] == u8_idx)
                        {
                            SINGLE_PROCESS(u8dev,tx_id);
                        }
                    }
                    break;
            }
            mculib_delay_ms(200);
            video_bypass_tx_cfg(u8dev,u8_idx);
            VIDEOMUTE(u8dev,u8_idx, FALSE);
        }
    }

    for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
    {
        #if 1
        if(Mapp_Rx[u8dev][tx_id] == u8_idx)
        {
            if(g_u16Startup2InitTimer[u8dev][tx_id]==0)
            {
                if(g_bFirstStartupFlag[u8dev][tx_id])
                {
                    g_bFirstStartupFlag[u8dev][tx_id] = FALSE;
                    app_ms1826_audio_input_switch(u8dev,Ms1826event[u8dev].u8TxAudioSourceSta[tx_id],tx_id);
                    api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][tx_id], b_audio_mute[u8dev][tx_id]);
                }
            }
        }
        #endif
    }


    if(g_st_hdmi_rx[u8dev][u8_idx].u8_rx_state != RX_STATE_RUNNING)
        VIDEOMUTE(u8dev,u8_idx, TRUE);
}

void ms1826_startup_timer_counter(UINT8 u8dev)
{
    u8 i;

    for(i=0;i<4;i++)
    {
        if(g_u16Startup2InitTimer[u8dev][i])
            g_u16Startup2InitTimer[u8dev][i]--;
    }
}

static VOID video_rx_out(UINT8 u8dev,UINT8 u8_idx)
{
    UINT8 tx_id = 0;
    UINT16 u8_rx_clock = 0;
    UINT32 u32_rx_mdt_ints = 0;
	static UINT32 u32_mute_cnt[4] = {0};


    u8_rx_clock = ms1826_hal_hdmi_rx_get_tmds_clk(u8_idx);
    if(u8_rx_clock == 0)
    {
        VIDEOMUTE(u8dev,u8_idx, MS_TRUE);
		u32_mute_cnt[u8_idx] = 0;
        g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable = FALSE;
    }
    else
    {
		//if(u8_rx_change_bak[u8dev][u8_idx] == 1)
		{
			if(u32_mute_cnt[u8_idx] > 10)
			{
                VIDEOMUTE(u8dev,u8_idx, false);

#if 1
                if(!g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable)
                {
					mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x4004 + u8_idx * 0x100, 0x10);    // freerun lock
					mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x4004 + u8_idx * 0x100, 0x11);    // once lock
#if ENABLE_MS1826_OUT_MODE_RELOAD_EDID
                    log("[INFO] hdmi %d video change reload edid\r\n", u8_idx);
                    api_hdmi_rx_edid_update(&g_st_hdmi_rx[u8dev][u8_idx],(UINT8 *)g_u8c_sys_edid_default_buf,256);
#endif
                }
#endif
                g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable = TRUE;
			}
			else
			{
				u32_mute_cnt[u8_idx] ++;
			}
		}
    }

    for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
    {
        if(Mapp_Rx[u8dev][tx_id] == u8_idx)
        {
            if(g_u16Startup2InitTimer[u8dev][tx_id]==0)
            {
                if(g_bFirstStartupFlag[u8dev][tx_id])
                {
                    g_bFirstStartupFlag[u8dev][tx_id] = FALSE;
                    api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][tx_id], b_audio_mute[u8dev][tx_id]);
                }
            }
        }
    }

#if 1//ENABLE_MS1826_FRAME_LOCK
    if(g_bPreTimingStable[u8dev][u8_idx] != g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable)
    {
        g_bPreTimingStable[u8dev][u8_idx] = g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable;

        #if ENABLE_MS1826_FRAME_LOCK
        for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
        {
            if(g_u8FrameLockRxTable[u8dev][tx_id] == u8_idx)
            {
                if(g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable)
                {
                    _video_timgen_cfg_frame_lock(u8dev,u8_idx,TIMGEN_1+tx_id);
                }
                else
                {
                    _video_timgen_cfg(u8dev,TIMGEN_1+tx_id);
                }
            }
        }
        #endif

    }
#endif

    if(u8_rx_change_bak[u8dev][u8_idx] == 1)
    {
		u32_rx_mdt_ints = ms1826_hal_hdmi_rx_get_mdt_ints_ext(u8_idx);
		api_hdmi_rx_update_state(&g_st_hdmi_rx[u8dev][u8_idx]);
		if(u32_rx_mdt_ints & 0x3045)
			ms1826_hal_hdmi_rx_phy_process(u8_idx, g_st_hdmi_rx[u8dev][u8_idx].st_rx_ints.b_clk_change, u8_rx_clock);
		if(g_st_hdmi_rx[u8dev][u8_idx].st_rx_pdec_sts.b_afif_underflow || g_st_hdmi_rx[u8dev][u8_idx].st_rx_pdec_sts.b_afif_overflow)
		{
			ms1826_hal_hdmi_rx_audio_hbr_pll_cfg(u8_idx, g_st_hdmi_rx[u8dev][u8_idx].st_rx_pdec_sts.b_hbr_rcv);
			ms1826_hal_hdmi_rx_audio_fifo_reset(u8_idx, MS_TRUE);
			mculib_delay_ms(1);
			ms1826_hal_hdmi_rx_audio_fifo_reset(u8_idx, false);
			mculib_delay_ms(1);
			ms1826_hal_hdmi_rx_hbr_en(u8_idx, g_st_hdmi_rx[u8dev][u8_idx].st_rx_pdec_sts.b_hbr_rcv);
		}
		api_hdmi_rx_config(&g_st_hdmi_rx[u8dev][u8_idx]);
		if(api_hdmi_rx_is_audio_changed(&g_st_hdmi_rx[u8dev][u8_idx]))
		{
		    log("[INFO] hdmi %d audio change\r\n", u8_idx);
			#if 1
			for(tx_id = 0; tx_id < 4; tx_id ++)
			{
				if(u8_Tx_audio_sel[u8dev][tx_id] == u8_idx)
				{
                    if(g_st_hdmi_tx[u8dev][tx_id].u8_audio_src == FROM_HDMI_RX_AUDIO)
                    {
    					api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][tx_id], MS_TRUE);
    					if(g_bFirstStartupFlag[u8dev][tx_id] == TRUE)
    					{
                            if(g_bDigitalAudioSwByHdmiTx[u8dev][tx_id])
                                Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id] = u8_Tx_audio_sel[u8dev][tx_id];
                            app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_i2s_idx[tx_id], FALSE);
                            app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_spdif_idx[tx_id], FALSE);
                        }
    					memcpy((&g_st_hdmi_tx[u8dev][tx_id].st_infoframes_packets.st_audio_infoframe), &g_st_hdmi_rx[u8dev][u8_idx].st_packets_infoframes.st_audio_infoframe, sizeof(HDMI_AUDIO_INFOFRAME_T));
    					api_hdmi_tx_audio_change(&g_st_hdmi_tx[u8dev][tx_id]);
    					if(g_bFirstStartupFlag[u8dev][tx_id] == FALSE)
                            api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][tx_id], b_audio_mute[u8dev][tx_id]);
    					log("[INFO] hdmi b_audio_mute %d audio change config\r\n", b_audio_mute[u8dev][tx_id]);
    					log("[INFO] hdmi %d audio change config\r\n", u8_idx);
                    }
				}
			}
			#endif
		}
        return ;
    }
    #if MS1826_RX_MANUAL_EQ_ENABLE
    api_hdmi_rx_manual_eq(&g_st_hdmi_rx[u8dev][u8_idx],MS1826_RX_EQ_VALUE_CH1,MS1826_RX_EQ_VALUE_CH2,MS1826_RX_EQ_VALUE_CH3,MS_TRUE);
    #endif
    api_hdmi_rx_task(&g_st_hdmi_rx[u8dev][u8_idx]);
    #if MS1826_RX_MANUAL_EQ_ENABLE
    api_hdmi_rx_manual_eq(&g_st_hdmi_rx[u8dev][u8_idx],MS1826_RX_EQ_VALUE_CH1,MS1826_RX_EQ_VALUE_CH2,MS1826_RX_EQ_VALUE_CH3,MS_TRUE);
    #endif

    if(api_hdmi_rx_is_audio_changed(&g_st_hdmi_rx[u8dev][u8_idx]))
    {
        log("[INFO] hdmi %d audio change\r\n", u8_idx);
#if 1
        for(tx_id = 0; tx_id < 4; tx_id ++)
        {
            if(u8_Tx_audio_sel[u8dev][tx_id] == u8_idx)
            {
                if(g_st_hdmi_tx[u8dev][tx_id].u8_audio_src == FROM_HDMI_RX_AUDIO)
                {
                    api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][tx_id], MS_TRUE);
                    if(g_bFirstStartupFlag[u8dev][tx_id] == TRUE)
                    {
                        if(g_bDigitalAudioSwByHdmiTx[u8dev][tx_id])
                            Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id] = u8_Tx_audio_sel[u8dev][tx_id];
                        app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_i2s_idx[tx_id], FALSE);
                        app_digital_audio_cfg(u8dev,Ms1826event[u8dev].u8TxDigitalAudioSource[tx_id], g_u8_spdif_idx[tx_id], FALSE);
                    }
                    memcpy((&g_st_hdmi_tx[u8dev][tx_id].st_infoframes_packets.st_audio_infoframe), &g_st_hdmi_rx[u8dev][u8_idx].st_packets_infoframes.st_audio_infoframe, sizeof(HDMI_AUDIO_INFOFRAME_T));
                    api_hdmi_tx_audio_change(&g_st_hdmi_tx[u8dev][tx_id]);
                    if(g_bFirstStartupFlag[u8dev][tx_id] == FALSE)
                        api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][tx_id], b_audio_mute[u8dev][tx_id]);
                    log("[INFO] hdmi b_audio_mute %d audio change config\r\n", b_audio_mute[u8dev][tx_id]);
                    log("[INFO] hdmi %d audio change config\r\n", u8_idx);
                }
            }
        }
#endif
    }

    if(api_hdmi_rx_is_video_changed(&g_st_hdmi_rx[u8dev][u8_idx]))
    {
        log("[INFO] hdmi %d video change\r\n", u8_idx);
        log("RX Change: %d  \r\n", u8_idx + 1);
        log("[INFO] hdmi %d video  mute on \r\n", u8_idx);
		VIDEOMUTE(u8dev,u8_idx, MS_TRUE);
        if(api_hdmi_rx_get_video_timing(&g_st_hdmi_rx[u8dev][u8_idx]) == MS_ERR_OK)
        {
            log("[INFO] hdmi%d video timing ok\r\n", u8_idx);
            if(_timing_compare(&Video_HDMIRx_Timing[u8dev][u8_idx],  &(g_st_hdmi_rx[u8dev][u8_idx].st_rx_video_timing)) == FALSE)
            {
                log("[INFO] hdmi%d video timing not the same\r\n", u8_idx);
                memcpy(&Video_HDMIRx_Timing[u8dev][u8_idx],&(g_st_hdmi_rx[u8dev][u8_idx].st_rx_video_timing),sizeof(HDMI_VIDEO_TIMING_T));
                if(Video_HDMIRx_Timing[u8dev][u8_idx].u8_polarity % 2 == 0)
                {
                    Video_HDMIRx_Timing[u8dev][u8_idx].u16_vactive /= 2;
                }
                b_RxVideoChg[u8dev][u8_idx] = TRUE;
            }
            else
            {
                log("[INFO] hdmi%d video timing same b_RxVideoChg = %d\r\n", u8_idx,b_RxVideoChg[u8dev][u8_idx]);
                if(b_RxVideoChg[u8dev][u8_idx] == false)
                {
                    b_RxVideoChg[u8dev][u8_idx] = TRUE;
                }
            }

        }
        else
        {
            memset(&Video_HDMIRx_Timing[u8dev][u8_idx],NULL,sizeof(HDMI_VIDEO_TIMING_T));

            g_st_hdmi_rx[u8dev][u8_idx].b_timing_stable = FALSE;
        }
        OSDShown[u8dev] = MS_TRUE;
        sys_show_osd(u8dev,OSDShown[u8dev], 1<<u8_idx);
        _video_misc_process(u8dev,u8_idx);
    }
    else
    {
        if(g_st_hdmi_rx[u8dev][u8_idx].u8_rx_state == RX_STATE_RUNNING && b_RxVideoChg[u8dev][u8_idx])
        {
            log("Video change rx[%d]\r\n",u8_idx);
            u8_rx_change_bak[u8dev][u8_idx] = 1;
            b_RxVideoChg[u8dev][u8_idx] = FALSE;
            switch(display_mode[u8dev])
            {
                case MATRIX_SWITCHER:
                case MATRIX_SWITCHER_OUT:
                    for(tx_id = 0 ; tx_id < 4 ; tx_id ++)
                    {
                        if(Mapp_Rx[u8dev][tx_id] == u8_idx)
                        {
                            mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x4004 + u8_idx * 0x100, 0x10);    // freerun lock
						    mculib_i2c_write_16bidx8bval(gu8ChipAddr[u8dev],0x4004 + u8_idx * 0x100, 0x11);    // once lock
                            SINGLE_PROCESS(u8dev,tx_id);
                        }
                    }
                    break;
            }

        }
    }
}

static VOID _video_tx_hdcp_process(UINT8 u8dev)
{
    BOOL b_hpd = FALSE;
    UINT8 tx_id = 0;
    static BOOL b_hdcp_status = FALSE;

    for(tx_id = 0; tx_id < 4; tx_id ++)
    {
        if(b_hdcp_status && tx_id == 0)
        {
            if(api_hdmi_tx_hdcp14_get_status(&g_st_hdmi_tx[u8dev][tx_id]))
            {
//              log("[INFO] HDCP success...\r\n");
            }
            else
            {
//              log("[INFO] HDCP failed...\r\n");
            }
        }

        b_hpd = api_hdmi_tx_get_hpd_status(&g_st_hdmi_tx[u8dev][tx_id]);
        if(b_hpd != b_hpd_bakes[u8dev][tx_id])
        {
            b_hpd_bakes[u8dev][tx_id]    = b_hpd;
            if(b_hpd)
            {
                switch(tx_id)
                {
                    case 0:

                        log("[INFO] HDMI Tx1 connected....\r\n");
                        break;
                    case 1:
                        log("[INFO] HDMI Tx2 connected....\r\n");
                        break;
                    case 2:
                        log("[INFO] HDMI Tx3 connected....\r\n");
                        break;
                    case 3:
                        log("[INFO] HDMI Tx4 connected....\r\n");
                        break;
                }
            }
            else
            {
                switch(tx_id)
                {
                    case 0:
                        b_hdcp_status = FALSE;
                        log("[INFO] HDMI Tx1 disconnected....\r\n");
                        break;
                    case 1:
                        log("[INFO] HDMI Tx2 disconnected...\r\n.");
                        break;
                    case 2:
                        log("[INFO] HDMI Tx3 disconnected....\r\n");
                        break;
                    case 3:
                        log("[INFO] HDMI Tx4 disconnected...\r\n.");
                        break;
                }
            }
        }
    }
}

VOID video_process(UINT8 u8dev)
{
    UINT8 u8_idx = 0;

    _video_tx_process(u8dev);
    _video_tx_hdcp_process(u8dev);



    for(u8_idx = 0; u8_idx < 4; u8_idx++)
    {
        if(display_mode[u8dev]==MATRIX_SWITCHER_OUT)
            video_rx_out(u8dev,u8_idx);
        else
            _video_process(u8dev,u8_idx);
    }
}

VOID osd_service(UINT8 u8dev)
{
    UINT8 u8_osd_time;

    if(g_u8_time == 0)
        u8_osd_time =10;
    else if(g_u8_time == 1)
        u8_osd_time =30;
    else if(g_u8_time == 2)
        u8_osd_time =60;
    else if(g_u8_time == 3)
        return;

    if (!OSDShown[u8dev] && !b_osd_menu)
    {
        OSDCount[u8dev] = 0;
        return;
    }

    OSDCount[u8dev] ++;

    if (OSDCount[u8dev] > u8_osd_time) // count every 500ms
    {
        OSDCount[u8dev] = 0;
        sys_show_osd(u8dev, FALSE, 0x0F);
        OSDShown[u8dev] = FALSE;
    }
}

