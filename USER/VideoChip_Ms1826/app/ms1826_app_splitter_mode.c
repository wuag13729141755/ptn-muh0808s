#ifndef _MS1826_APP_SPLITTER_MODE_C_
#define _MS1826_APP_SPLITTER_MODE_C_

#include "ms1826_videoprocess.h"

DMA_COMMON_DEV_T st_test_dma_com_dev;

SPLITTER_VIPSS_T gst_splitter_vipss_dev[DEV_NUM_MS1826][4];
SPLITTER_VOPSS_T gst_splitter_vopss_dev[DEV_NUM_MS1826][4];
SPLITTER_APP_T g_st_app_splitter_dev[DEV_NUM_MS1826];
UINT8 g_u8_splitter_audio_src[DEV_NUM_MS1826] = {AUDIO_HDMI_RX1};
stMs1826Wincfg_T g_stMs1826WinTable[DEV_NUM_MS1826][4];
UINT8 g_u8_vipss_audio_src[DEV_NUM_MS1826] = {AUDIO_HDMI_RX1};

static VOID app_splitter_vipss_instance_init(UINT8 u8dev,SPLITTER_VIPSS_T *pst_splitter_vipss_dev)
{
    api_4ksdn_instance_init(&pst_splitter_vipss_dev->st_sdn4k_dev, g_u8_4ksdn_idx[g_u8_rx_idx[pst_splitter_vipss_dev->u8_vipss_sel]]);
    api_sdn_instance_init(&pst_splitter_vipss_dev->st_splitter_sdn_dev, g_u8_sdn_idx[g_u8_rx_idx[pst_splitter_vipss_dev->u8_vipss_sel]]);
    api_vwdma_instance_init(&pst_splitter_vipss_dev->st_splitter_vwdma_dev, g_u8_dma_idx[g_u8_rx_idx[pst_splitter_vipss_dev->u8_vipss_sel]]);
    api_dma_common_instance_init(&pst_splitter_vipss_dev->st_splitter_com_wdma_dev, g_u8_dma_idx[g_u8_rx_idx[pst_splitter_vipss_dev->u8_vipss_sel]]);
    u8dev = u8dev;
}

static VOID app_splitter_vipss_rx_mux_cfg(UINT8 u8dev,UINT8 u8_vipss_sel, SPLITTER_APP_T *pst_app_splitter_dev)
{
#if (MS1826_SPLITTER_USER_MODE_ENABLE==0)
    if(pst_app_splitter_dev->u8_splitter_mode < MODE_DUAL_UP1_DOWN2)
    {
        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[pst_app_splitter_dev->u8_splitter_mode]];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][pst_app_splitter_dev->u8_splitter_mode].u8_color_space;
    }
    else
#endif
    {
//        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[u8_vipss_sel]];
//        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][u8_vipss_sel].u8_color_space ;
        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[g_u8SplitterDispMapping[u8dev][u8_vipss_sel]]];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][g_u8SplitterDispMapping[u8dev][u8_vipss_sel]].u8_color_space ;
    }
    log("gst_splitter_vipss_dev[%d][%d].st_vipss_mux_dev.u8_src_module = %d\r\n",u8dev,u8_vipss_sel,gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module);

    //vipss rx mux config
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_dst_module = g_u8_misc_module_vipss[u8_vipss_sel];
    api_misc_rx_mux_cfg(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev);

    if(g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace == HDMI_COLORSPACE_YCBCR420)
    {
        //vipss rx csc config
        g_st_vipss_csc[u8dev][u8_vipss_sel].u8_sel = g_u8_csc_idx[u8_vipss_sel];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        api_csc_rx_set(&g_st_vipss_csc[u8dev][u8_vipss_sel]);
        //vipss mux config
        api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_5);       //< CSC -> 4KSDN -> SDN -> VWDMA1 */

        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_idx = g_u8_4ksdn_idx[u8_vipss_sel];
        api_sdn4k_init(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_input_colorSpace = HDMI_COLORSPACE_YCBCR420;
        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_output_colorSpace = HDMI_COLORSPACE_YCBCR444;
        gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_mode = SDN4K_MODE_AUTO;
        api_sdn4k_config(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
        api_sdn4k_enable(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev, MS_ENABLE);
#if (MS1826_SPLITTER_USER_MODE_ENABLE==0)
        if(pst_app_splitter_dev->u8_splitter_mode < MODE_DUAL_UP1_DOWN2)
            Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_vactive = g_st_hdmi_rx[u8dev][pst_app_splitter_dev->u8_splitter_mode].st_rx_video_timing.u16_vactive / 2;
        else
#endif
            Video_HDMIRx_Timing[u8dev][u8_vipss_sel].u16_vactive = g_st_hdmi_rx[u8dev][u8_vipss_sel].st_rx_video_timing.u16_vactive / 2;
    }
    else
    {
        //vipss rx csc config
        g_st_vipss_csc[u8dev][u8_vipss_sel].u8_sel = g_u8_csc_idx[u8_vipss_sel];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        api_csc_rx_set(&g_st_vipss_csc[u8dev][u8_vipss_sel]);

#if (MS1826_SPLITTER_USER_MODE_ENABLE==0)
        if(pst_app_splitter_dev->u8_splitter_mode < MODE_DUAL_UP1_DOWN2 &&
            g_st_hdmi_rx[u8dev][pst_app_splitter_dev->u8_splitter_mode].st_rx_video_timing.u8_polarity % 2 == 0)
        {
            switch(g_st_hdmi_rx[u8dev][pst_app_splitter_dev->u8_splitter_mode].st_rx_video_timing.u16_vactive)
            {
                case 1080:
                    //vipss mux config
                    api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_8);       //< CSC ->DEINT-> SDN -> VWDMA1
                    //vipss rx deint config
                    g_st_deint_dev[u8dev].b_yuv444 = MS_TRUE;
                    g_st_deint_dev[u8dev].b_deint_en = MS_TRUE;
                    g_st_deint_dev[u8dev].u16_vactive = 1080;
                    g_st_deint_dev[u8dev].b_hd_full_mode = MS_TRUE;
                    g_st_deint_dev[u8dev].u32_buff0_start_addr = 0;
                    g_st_deint_dev[u8dev].u32_buff1_start_addr = 4000000;
                    api_deint_init(&g_st_deint_dev[u8dev]);
                    api_deint_config(&g_st_deint_dev[u8dev]);
                    Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_vactive = 1080;
                    break;
                case 480:
                    //vipss mux config
                    api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_8);       //< CSC ->DEINT-> SDN -> VWDMA1
                    //vipss rx deint config
                    g_st_deint_dev[u8dev].b_yuv444 = MS_TRUE;
                    g_st_deint_dev[u8dev].b_deint_en = MS_TRUE;
                    g_st_deint_dev[u8dev].u16_vactive = 480;
                    g_st_deint_dev[u8dev].b_hd_full_mode = false;
                    g_st_deint_dev[u8dev].u32_buff0_start_addr = 0;
                    g_st_deint_dev[u8dev].u32_buff1_start_addr = 4000000;
                    api_deint_init(&g_st_deint_dev[u8dev]);
                    api_deint_config(&g_st_deint_dev[u8dev]);
                    Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_splitter_dev->u8_splitter_mode].st_rx_video_timing.u16_hactive/2;
                    Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_vactive = 480;
                    break;
                case 576:
                    //vipss mux config
                    api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_8);       //< CSC ->DEINT-> SDN -> VWDMA1
                    //vipss rx deint config
                    g_st_deint_dev[u8dev].b_yuv444 = MS_TRUE;
                    g_st_deint_dev[u8dev].b_deint_en = MS_TRUE;
                    g_st_deint_dev[u8dev].u16_vactive = 576;
                    g_st_deint_dev[u8dev].b_hd_full_mode = false;
                    g_st_deint_dev[u8dev].u32_buff0_start_addr = 0;
                    g_st_deint_dev[u8dev].u32_buff1_start_addr = 4000000;
                    api_deint_init(&g_st_deint_dev[u8dev]);
                    api_deint_config(&g_st_deint_dev[u8dev]);
                    Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_splitter_dev->u8_splitter_mode].st_rx_video_timing.u16_hactive/2;
                    Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_vactive = 576;
                    break;
                default:
                    g_st_deint_dev[u8dev].b_deint_en = false;
                    api_deint_config(&g_st_deint_dev[u8dev]);
                    //vipss mux config
                    api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_4);       //< CSC -> SDN -> VWDMA1
                    break;
            }
        }
        else
#endif
        {
            if(g_st_hdmi_rx[u8dev][u8_vipss_sel].st_rx_video_timing.u8_polarity % 2 == 0)
            {
                g_st_deint_dev[u8dev].b_deint_en = false;
                api_deint_config(&g_st_deint_dev[u8dev]);
                Video_HDMIRx_Timing[u8dev][u8_vipss_sel].u16_hactive = g_st_hdmi_rx[u8dev][u8_vipss_sel].st_rx_video_timing.u16_hactive;
                Video_HDMIRx_Timing[u8dev][u8_vipss_sel].u16_vactive = g_st_hdmi_rx[u8dev][u8_vipss_sel].st_rx_video_timing.u16_vactive / 2;

            }
            //vipss mux config
            api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_4);       //< CSC -> SDN -> VWDMA1
        }
    }
}

static VOID app_splitter_vipss_cfg(UINT8 u8dev,UINT8 u8_vipss_sel, SPLITTER_APP_T *pst_app_splitter_dev)
{
    //vipss sdn config
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.u8_sel = g_u8_sdn_idx[u8_vipss_sel];
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.b_enable = MS_TRUE;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.u8_sdn_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.st_video_size.st_InRect.u16_h = pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tInSize.u16_h;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.st_video_size.st_InRect.u16_v = pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tInSize.u16_v;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h = pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tMemSize.u16_h;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v = pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tMemSize.u16_v;
    api_sdn_enable_set(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev);
    log("st_scale_size[%d].tInSize.u16_h==0x%04x\r\n",u8_vipss_sel,pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tInSize.u16_h);
    log("st_scale_size[%d].tInSize.u16_v==0x%04x\r\n",u8_vipss_sel,pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tInSize.u16_v);
    log("st_scale_size[%d].tMemSize.u16_h==0x%04x\r\n",u8_vipss_sel,pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tMemSize.u16_h);
    log("st_scale_size[%d].tMemSize.u16_v==0x%04x\r\n",u8_vipss_sel,pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tMemSize.u16_v);

    api_sdn_scaler_size_set(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev);
    api_sdn_csc_set(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_sdn_dev);

    //vipss vwdma config
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.b_enable = false;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.u16_request_len = 128;
    api_vwdma_request_len_cfg(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);

    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.b_enable = MS_TRUE;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.st_vwdma_in_size.u16_h_size = pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tMemSize.u16_h;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.st_vwdma_in_size.u16_v_size = pst_app_splitter_dev->st_scale_size[u8_vipss_sel].tMemSize.u16_v;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.u8_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[u8_vipss_sel].u32_dma_start_addr_0;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[u8_vipss_sel].u32_dma_start_addr_1;
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[u8_vipss_sel].u32_dma_start_addr_2;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);

    api_vwdma_start_addr_cfg(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);
    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.u16_line_offset = g_st_app_splitter_dev[u8dev].st_scale_size[u8_vipss_sel].tMemSize.u16_h;
    api_vwdma_line_offset_cfg(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);
    api_vwdma_burst_num_cfg(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);

    gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev.u8_vwdma_rotatation_deg = ROTATION_0_DEG;
    api_vwdma_rotation_cfg(&gst_splitter_vipss_dev[u8dev][u8_vipss_sel].st_splitter_vwdma_dev);
}

static VOID app_splitter_vopss_instance_init(UINT8 u8dev,SPLITTER_VOPSS_T *pst_splitter_vopss_dev)
{
    api_vrdma_instance_init(&pst_splitter_vopss_dev->st_splitter_vrdma_dev, g_u8_dma_idx[g_u8_rx_idx[pst_splitter_vopss_dev->u8_vopss_sel]]);

    api_su_instance_init(&pst_splitter_vopss_dev->st_splitter_su_dev, g_u8_su_idx[g_u8_rx_idx[pst_splitter_vopss_dev->u8_vopss_sel]]);
    u8dev = u8dev;
}

static VOID app_splitter_vopss_cfg(UINT8 u8dev,UINT8 u8_vopss_sel, SPLITTER_APP_T *pst_app_splitter_dev)
{
    MISC_MODULE_CLK_T st_module_clk;

    //vopss VRDMA config
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.b_enable = false;
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.u16_request_len = 128;
    api_vrdma_request_len_cfg(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);

    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.b_enable = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.u8_mem_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.st_vrdma_out_size.u16_h_size = pst_app_splitter_dev->st_scale_size[u8_vopss_sel].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.st_vrdma_out_size.u16_v_size = pst_app_splitter_dev->st_scale_size[u8_vopss_sel].tMemSize.u16_v;

    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.u16_line_offset = gst_splitter_vipss_dev[u8dev][u8_vopss_sel].st_splitter_vwdma_dev.u16_line_offset;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[u8_vopss_sel].u32_dma_start_addr_0;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[u8_vopss_sel].u32_dma_start_addr_1;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[u8_vopss_sel].u32_dma_start_addr_2;

    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_DISABLE;
    api_vrdma_mirror_cfg(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);
    api_vrdma_start_addr_cfg(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);
    api_vrdma_burst_num_cfg(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);
    api_vrdma_line_offset_cfg(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_vrdma_dev);

    // su clk src
    st_module_clk.u8_dst_module = g_u8_misc_su[u8_vopss_sel];
    st_module_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1;
    st_module_clk.u8_clkdiv = MISC_CLKDIV_1;
    api_misc_su_clkin_sel(&st_module_clk);

    //vopss su config
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.u8_sel = g_u8_su_idx[u8_vopss_sel];
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.b_enable = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.u8_su_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_size.st_MemRect.u16_h = pst_app_splitter_dev->st_scale_size[u8_vopss_sel].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_size.st_MemRect.u16_v = pst_app_splitter_dev->st_scale_size[u8_vopss_sel].tMemSize.u16_v;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_size.st_OutRect.u16_h =
        pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_sp - pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_st;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_size.st_OutRect.u16_v =
        pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_sp - pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_st;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_win.u16_hde_st = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_st;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_win.u16_hde_sp = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_sp;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_win.u16_vde_st = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_st;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_video_win.u16_vde_sp = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_sp;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_dis_win.u8_dis_win_en = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_dis_win.u16_hde_st = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_st;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_dis_win.u16_hde_sp = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_sp;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_dis_win.u16_vde_st = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_st;
    gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev.st_dis_win.u16_vde_sp = pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_sp;
    api_su_enable_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev);
    api_su_scaler_size_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev);
    api_su_csc_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev);
    api_su_video_win_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev);
    api_su_display_win_set(&gst_splitter_vopss_dev[u8dev][u8_vopss_sel].st_splitter_su_dev);

    log("st_video_win[%d].u16_hde_st==0x%04x\r\n",u8_vopss_sel,pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_st);
    log("st_video_win[%d].u16_hde_sp==0x%04x\r\n",u8_vopss_sel,pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_hde_sp);
    log("st_video_win[%d].u16_vde_st==0x%04x\r\n",u8_vopss_sel,pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_st);
    log("st_video_win[%d].u16_vde_sp==0x%04x\r\n",u8_vopss_sel,pst_app_splitter_dev->st_video_win[u8_vopss_sel].u16_vde_sp);
}

static VOID _splitter_mode_frc_cfg(UINT8 u8dev)
{
     //DMA config
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_ss_vipss_dev.u8_sel = g_u8_ss_ch_idx[VIPSS1];
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_ss_vipss_dev.u8_vipss_wdma_id_sel = VOPSS1;
    api_ss_vipss_wdma_buf_sel(&gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_ss_vipss_dev);

    gst_splitter_vipss_dev[u8dev][VIPSS2].st_splitter_ss_vipss_dev.u8_sel = g_u8_ss_ch_idx[VIPSS2];
    gst_splitter_vipss_dev[u8dev][VIPSS2].st_splitter_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS2;
    api_ss_vipss_wdma_buf_sel(&gst_splitter_vipss_dev[u8dev][VIPSS2].st_splitter_ss_vipss_dev);

    gst_splitter_vipss_dev[u8dev][VIPSS3].st_splitter_ss_vipss_dev.u8_sel = g_u8_ss_ch_idx[VIPSS3];
    gst_splitter_vipss_dev[u8dev][VIPSS3].st_splitter_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS3;
    api_ss_vipss_wdma_buf_sel(&gst_splitter_vipss_dev[u8dev][VIPSS3].st_splitter_ss_vipss_dev);

    gst_splitter_vipss_dev[u8dev][VIPSS4].st_splitter_ss_vipss_dev.u8_sel = g_u8_ss_ch_idx[VIPSS4];
    gst_splitter_vipss_dev[u8dev][VIPSS4].st_splitter_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS4;
    api_ss_vipss_wdma_buf_sel(&gst_splitter_vipss_dev[u8dev][VIPSS4].st_splitter_ss_vipss_dev);

    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS1];
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev.u8_dma_buf_num_sel = SPLITTER_BUF_NUM;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_ss_vopss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS1];
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS1;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS1;
    api_dma_common_buf_num_set(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_ss_vopss_dev);
    api_ss_vopss_dst_buf_id_sel(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_ss_vopss_dev);

    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS2];
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev.u8_dma_buf_num_sel = SPLITTER_BUF_NUM;
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_ss_vopss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS2];
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS2;
    gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS2;
    api_dma_common_buf_num_set(&gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_ss_vopss_dev);
    api_ss_vopss_dst_buf_id_sel(&gst_splitter_vopss_dev[u8dev][VOPSS2].st_splitter_ss_vopss_dev);

    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS3];
    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev.u8_dma_buf_num_sel = SPLITTER_BUF_NUM;
    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS3;
    gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS3;
    api_dma_common_buf_num_set(&gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_ss_vopss_dev);
    api_ss_vopss_dst_buf_id_sel(&gst_splitter_vopss_dev[u8dev][VOPSS3].st_splitter_ss_vopss_dev);

    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS4];
    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev.u8_dma_buf_num_sel = SPLITTER_BUF_NUM;
    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS4;
    gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS4;
    api_dma_common_buf_num_set(&gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_ss_vopss_dev);
    api_ss_vopss_dst_buf_id_sel(&gst_splitter_vopss_dev[u8dev][VOPSS4].st_splitter_ss_vopss_dev);

}
VOID _splitter_video_freeze(UINT8 u8dev,UINT8 u8_path_sel)
{
    BOOL b_vb_h;
    DMA_COMMON_DEV_T st_dma_dev;
    //video freeze
    __disable_irq();
    gst_splitter_vipss_dev[u8dev][u8_path_sel].st_splitter_com_wdma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
    gst_splitter_vipss_dev[u8dev][u8_path_sel].st_splitter_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
    gst_splitter_vipss_dev[u8dev][u8_path_sel].st_splitter_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;

    st_dma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vipss_dev[u8dev][u8_path_sel].st_splitter_com_wdma_dev);
    g_u16_timer_out = 0;
    do
    {
        g_u16_timer_out ++;
        b_vb_h = api_misc_vb_status_get(g_u8_vb_path[u8_path_sel]);
    }
    while(b_vb_h == false && (g_u16_timer_out < 500));
    api_dma_common_trigger(&st_dma_dev);
    api_dma_common_buf_id_get(&st_dma_dev);
    rdma_buf_id[u8dev] = st_dma_dev.st_dma_status.rdma_buf_id;
    wdma_buf_id[u8dev] = (rdma_buf_id[u8dev] + 1) % 3;
    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = wdma_buf_id[u8dev];
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = rdma_buf_id[u8dev];
    api_dma_comom_buf_id_set(&st_dma_dev);
    api_dma_common_trigger(&st_dma_dev);
    __enable_irq();
}

VOID _splitter_video_unfreeze(UINT8 u8dev)
{
    BOOL b_int;
    DMA_COMMON_DEV_T st_dma_dev;
    SS_DEV_T st_vopss_ss_dev;

    st_vopss_ss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS1] | g_u8_ss_ch_idx[VOPSS2] | g_u8_ss_ch_idx[VOPSS3] | g_u8_ss_ch_idx[VOPSS4] ;
    st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
    st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_tmg = VDS_TMG1;
    st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode = FORCE_TRIGGER;
    api_ss_vopss_trig_mode_cfg(&st_vopss_ss_dev);

    __disable_irq();
    st_dma_dev.u8_sel = g_u8_dma_idx[0];
    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 1) % 3;
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
    api_dma_comom_buf_id_set(&st_dma_dev);

    st_dma_dev.u8_sel = g_u8_dma_idx[1];
    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 1) % 3;
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
    api_dma_comom_buf_id_set(&st_dma_dev);

    st_dma_dev.u8_sel = g_u8_dma_idx[2];
    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 1) % 3;
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
    api_dma_comom_buf_id_set(&st_dma_dev);

    st_dma_dev.u8_sel = g_u8_dma_idx[3];
    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 1) % 3;
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
    api_dma_comom_buf_id_set(&st_dma_dev);

    g_u16_timer_out = 0;
    api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
    do
    {
        g_u16_timer_out ++;
    }
    while(!api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB) && (g_u16_timer_out < 500));
    api_ss_vopss_simultaneous_trig(&st_vopss_ss_dev);
    api_video_mixer_layer_only_config(&g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1]);

    st_dma_dev.u8_sel = g_u8_dma_idx[0];
    api_dma_common_fifo_status_get(&st_dma_dev);
    if(st_dma_dev.st_dma_status.b_h2l_status == MS_TRUE)
    {
        st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 2) % 3;
        st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
        api_dma_comom_buf_id_set(&st_dma_dev);
    }

    st_dma_dev.u8_sel = g_u8_dma_idx[1];
    api_dma_common_fifo_status_get(&st_dma_dev);
    if(st_dma_dev.st_dma_status.b_h2l_status == MS_TRUE)
    {
        st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 2) % 3;
        st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
        api_dma_comom_buf_id_set(&st_dma_dev);
    }

    st_dma_dev.u8_sel = g_u8_dma_idx[2];
    api_dma_common_fifo_status_get(&st_dma_dev);
    if(st_dma_dev.st_dma_status.b_h2l_status == MS_TRUE)
    {
        st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 2) % 3;
        st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
        api_dma_comom_buf_id_set(&st_dma_dev);
    }

    st_dma_dev.u8_sel = g_u8_dma_idx[3];
    api_dma_common_fifo_status_get(&st_dma_dev);
    if(st_dma_dev.st_dma_status.b_h2l_status == MS_TRUE)
    {
        st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 2) % 3;
        st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = wdma_buf_id[u8dev];
        api_dma_comom_buf_id_set(&st_dma_dev);
    }

    g_u16_timer_out = 0;
    api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
    do
    {
        g_u16_timer_out ++;
    }
    while(!api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB) && (g_u16_timer_out < 500));
    api_dma_common_simultaneous_trigger();

    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_com_wdma_dev);
    gst_splitter_vipss_dev[u8dev][VIPSS2].st_splitter_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vipss_dev[u8dev][VIPSS2].st_splitter_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vipss_dev[u8dev][VIPSS2].st_splitter_com_wdma_dev);
    gst_splitter_vipss_dev[u8dev][VIPSS3].st_splitter_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vipss_dev[u8dev][VIPSS3].st_splitter_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vipss_dev[u8dev][VIPSS3].st_splitter_com_wdma_dev);
    gst_splitter_vipss_dev[u8dev][VIPSS4].st_splitter_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_splitter_vipss_dev[u8dev][VIPSS4].st_splitter_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    api_dma_comom_buf_id_mode_cfg(&gst_splitter_vipss_dev[u8dev][VIPSS4].st_splitter_com_wdma_dev);

    g_u16_timer_out = 0;
    api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
    do
    {
        g_u16_timer_out ++;
        b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB);
    }
    while((b_int != MS_TRUE) && (g_u16_timer_out < 500));

    api_dma_common_simultaneous_trigger();
    __enable_irq();
}

VOID splitter_video_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute)
{
    BOOL b_int;
#if (MS1826_SPLITTER_USER_MODE_ENABLE==0)
    if(g_st_app_splitter_dev[u8dev].u8_splitter_mode < MODE_DUAL_UP1_DOWN2)
    {
        if(u8_path == g_st_app_splitter_dev[u8dev].u8_splitter_mode)
        {
            u8_path = 0;
        }
        else
        {
            return;
        }
    }
#endif
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.u8_sel = g_u8_su_idx[u8_path];
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.bd_enable = b_mute;
//    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_color.u8VR = 0x80;
//    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_color.u8YG = 0x10;
//    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_color.u8UB = 0x80;
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_color.u8VR = g_st_bg_color.u8_vr;
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_color.u8YG = g_st_bg_color.u8_yg;
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_color.u8UB = g_st_bg_color.u8_ub;

    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.bd_cover = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_size.u16_border_h = gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_size.st_OutRect.u16_h;
    gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_border.border_size.u16_border_v = gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev.st_video_size.st_OutRect.u16_v;
    api_su_video_border_set(&gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev);
    g_u16_timer_out = 0;
    api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
    do
    {
        g_u16_timer_out ++;
        b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB);
    }
    while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
    api_su_trigger(&gst_splitter_vopss_dev[u8dev][u8_path].st_splitter_su_dev);
}

//video mixer setup
static VOID _app_video_splitter_mixer_cfg(UINT8 u8dev,UINT8 u8_vmixer_idx)
{
    MISC_MODULE_CLK_T g_st_module_clk;

    g_st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_MIX1;
    g_st_module_clk.u8_clkdiv = MISC_CLKDIV_1;
    g_st_module_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1;
    api_misc_mixer_clkin_sel(&g_st_module_clk);

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].u8_video_mixer_idx = u8_vmixer_idx;
    api_video_mixer_instance_init(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx], u8_vmixer_idx);

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel = &g_st_mixer_layer_sel[u8dev];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf     = &g_st_mixer_alf_cfg[u8dev];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_bd_attr   = NULL;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_bd_color  = NULL;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_bg_color  = &g_st_bg_color;

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].u8_video_mixer_idx = u8_vmixer_idx;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].b_videomixer_en = MS_TRUE;
    api_video_mixer_bg_color_config(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx]);
    api_video_mixer_enable(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx]);

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer0_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer1_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer2_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer3_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS1_DSC_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS1_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS2_DSC_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS2_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS3_DSC_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS3_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS4_DSC_SU_alf_ratio = 0xFF;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf->u8_VOPSS4_SU_alf_ratio = 0xFF;
    api_video_mixer_layer_config(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx]);
}

#if MS1826_SPLITTER_USER_MODE_ENABLE
VOID app_splitter_mode_config(UINT8 u8dev,SPLITTER_APP_T *pst_app_splitter_dev)
{
    app_splitter_user_mode_config(u8dev,pst_app_splitter_dev);
}
#else
VOID app_splitter_mode_config(UINT8 u8dev,SPLITTER_APP_T *pst_app_splitter_dev)
{
#if 0
    UINT8 u8_i = 0;
    for(u8_i = 0; u8_i < 4; u8_i ++)
    {
        if(g_u8_HDMI_TX_PROT[pst_app_splitter_dev->u8_dst_sel] == g_u8_HDMI_TX_PROT[u8_i])
        {
            Bypass_Tx_Status[u8dev][u8_i] = false;
        }
        else
            Bypass_Tx_Status[u8dev][u8_i] = MS_TRUE;
    }
#endif

//    g_u8SplitterDispMapping[u8dev][0] = g_u8_HDMI_RX_PROT[0];
//    g_u8SplitterDispMapping[u8dev][1] = g_u8_HDMI_RX_PROT[1];
//    g_u8SplitterDispMapping[u8dev][2] = g_u8_HDMI_RX_PROT[2];
//    g_u8SplitterDispMapping[u8dev][3] = g_u8_HDMI_RX_PROT[3];

    gst_splitter_vipss_dev[u8dev][0].st_splitter_vwdma_dev.b_enable = FALSE;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][0].st_splitter_vwdma_dev);
    gst_splitter_vipss_dev[u8dev][1].st_splitter_vwdma_dev.b_enable = FALSE;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][1].st_splitter_vwdma_dev);
    gst_splitter_vipss_dev[u8dev][2].st_splitter_vwdma_dev.b_enable = FALSE;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][2].st_splitter_vwdma_dev);
    gst_splitter_vipss_dev[u8dev][3].st_splitter_vwdma_dev.b_enable = FALSE;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][3].st_splitter_vwdma_dev);

    gst_splitter_vopss_dev[u8dev][0].st_splitter_vrdma_dev.b_enable = FALSE;
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][0].st_splitter_vrdma_dev);
    gst_splitter_vopss_dev[u8dev][1].st_splitter_vrdma_dev.b_enable = FALSE;
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][1].st_splitter_vrdma_dev);
    gst_splitter_vopss_dev[u8dev][2].st_splitter_vrdma_dev.b_enable = FALSE;
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][2].st_splitter_vrdma_dev);
    gst_splitter_vopss_dev[u8dev][3].st_splitter_vrdma_dev.b_enable = FALSE;
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][3].st_splitter_vrdma_dev);
    app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
    app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
    app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
    app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
    if(pst_app_splitter_dev->b_splitter_sw == MS_TRUE)
    {
        _splitter_video_freeze(u8dev,VIPSS1);
        _splitter_video_freeze(u8dev,VIPSS2);
        _splitter_video_freeze(u8dev,VIPSS3);
        _splitter_video_freeze(u8dev,VIPSS4);
        mculib_delay_ms(50);

        g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
        g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
        g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
        g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
        g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
        g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
        g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
        g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);
    }
    else
    {
        UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].st_infoframes_packets.st_avi_infoframe.u8_video_code;

        //vipss1 instance init
        gst_splitter_vipss_dev[u8dev][VIPSS1].u8_vipss_sel = VIPSS1;
        app_splitter_vipss_instance_init(u8dev,&gst_splitter_vipss_dev[u8dev][VIPSS1]);

        //vopss1 instance init
        gst_splitter_vopss_dev[u8dev][VOPSS1].u8_vopss_sel = VOPSS1;
        app_splitter_vopss_instance_init(u8dev,&gst_splitter_vopss_dev[u8dev][VOPSS1]);

        //vipss2 instance init
        gst_splitter_vipss_dev[u8dev][VIPSS2].u8_vipss_sel = VIPSS2;
        app_splitter_vipss_instance_init(u8dev,&gst_splitter_vipss_dev[u8dev][VIPSS2]);

        //vopss2 instance init
        gst_splitter_vopss_dev[u8dev][VOPSS2].u8_vopss_sel = VOPSS2;
        app_splitter_vopss_instance_init(u8dev,&gst_splitter_vopss_dev[u8dev][VOPSS2]);

        //vipss3 instance init
        gst_splitter_vipss_dev[u8dev][VIPSS3].u8_vipss_sel = VIPSS3;
        app_splitter_vipss_instance_init(u8dev,&gst_splitter_vipss_dev[u8dev][VIPSS3]);

        //vopss3 instance init
        gst_splitter_vopss_dev[u8dev][VOPSS3].u8_vopss_sel = VOPSS3;
        app_splitter_vopss_instance_init(u8dev,&gst_splitter_vopss_dev[u8dev][VOPSS3]);

        //vipss4 instance init
        gst_splitter_vipss_dev[u8dev][VIPSS4].u8_vipss_sel = VIPSS4;
        app_splitter_vipss_instance_init(u8dev,&gst_splitter_vipss_dev[u8dev][VIPSS4]);

        //vopss4 instance init
        gst_splitter_vopss_dev[u8dev][VOPSS4].u8_vopss_sel = VOPSS4;
        app_splitter_vopss_instance_init(u8dev,&gst_splitter_vopss_dev[u8dev][VOPSS4]);

        //FRC
        _splitter_mode_frc_cfg(u8dev);

//        g_st_tx_mux[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[pst_app_splitter_dev->u8_dst_sel]];
//        g_st_tx_mux[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][pst_app_splitter_dev->u8_dst_sel]);
        g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
        g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
        g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
        g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
        g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
        g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
        g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
        g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);

        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][pst_app_splitter_dev->u8_dst_sel];
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_color_depth = HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_audio_src = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_pixclk;
        memcpy((&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel],false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel]);
        _video_port_hdmi_tx_driver_config(u8dev,pst_app_splitter_dev->u8_dst_sel);

        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel],MS_TRUE);
        _app_video_splitter_mixer_cfg(u8dev,VIDEO_MIXER_1);

        //vopss tx csc para
        g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[pst_app_splitter_dev->u8_dst_sel]];
        g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
        g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
        g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
        api_csc_tx_set(&g_st_txcsc_dev[u8dev][pst_app_splitter_dev->u8_dst_sel]);
    }
    switch(pst_app_splitter_dev->u8_splitter_mode)
    {
        case MODE_SIGNLE_1:
        case MODE_SIGNLE_2:
        case MODE_SIGNLE_3:
        case MODE_SIGNLE_4:
            app_splitter_vipss_rx_mux_cfg(u8dev,0, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][pst_app_splitter_dev->u8_splitter_mode].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h =
                return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_4K_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v =
                return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_4K_V_SIZE_MAX);


            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_4K_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_4K_SIZE;
            app_splitter_vipss_cfg(u8dev,0, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[0]);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,0, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_splitter_audio_src[u8dev] = pst_app_splitter_dev->u8_splitter_mode;
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER Single Mode %d\r\n",pst_app_splitter_dev->u8_splitter_mode+1);
            break;
        case MODE_DUAL_UP1_DOWN2:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[g_u8_rx_idx[0]]);
            app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[g_u8_rx_idx[1]]);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_UP1_DOWN2\r\n");
            break;
        case MODE_DUAL_UP1_DOWN2_4_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 * 4 / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 * 4 / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st =  g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_UP1_DOWN2_4_3\r\n");
            break;
        case MODE_DUAL_UP3_DOWN4:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[3];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);

            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 2;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][2];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_UP3_DOWN4\r\n");
            break;
        case MODE_DUAL_UP3_DOWN4_4_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 * 4 / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 * 4 / 3;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[3];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 2;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][2];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_UP3_DOWN4_4_3\r\n");
            break;
        case MODE_DUAL_LEFT1_RIGHT2:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_UP3_DOWN4_4_3\r\n");
            break;
        case MODE_DUAL_LEFT1_RIGHT2_4_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_LEFT1_RIGHT2_4_3\r\n");
            break;
        case MODE_DUAL_LEFT3_RIGHT4:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[3];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 2;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][2];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_LEFT1_RIGHT2_4_3\r\n");
            break;
        case MODE_DUAL_LEFT3_RIGHT4_4_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st =  g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[3];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 2;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][2];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_DUAL_LEFT3_RIGHT4_4_3\r\n");
            break;
        case MODE_PIP_12_1_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_PIP_12_1_3\r\n");
            break;
        case MODE_PIP_21_1_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 1;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][1];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_PIP_21_1_3\r\n");
            break;
        case MODE_PIP_34_1_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[3];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 2;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][2];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_PIP_34_1_3\r\n");
            break;
        case MODE_PIP_43_1_3:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[3];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 3;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][3];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_PIP_43_1_3\r\n");
            break;
        case MODE_THREE_EQUALIZE:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_THREE_EQUALIZE\r\n");
            break;
        case MODE_FOUR_MAIN1_SUB234:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_FOUR_MAIN1_SUB234\r\n");
            break;
        case MODE_FOUR_MAIN2_SUB134:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);


            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 1;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][1];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_FOUR_MAIN2_SUB134\r\n");
            break;
        case MODE_FOUR_MAIN3_SUB124:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);

            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);


            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 2;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][2];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_FOUR_MAIN3_SUB124\r\n");
            break;
        case MODE_FOUR_MAIN4_SUB123:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);


            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 3;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][3];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_FOUR_MAIN4_SUB123\r\n");
            break;
        case MODE_FOUR_H_SUB1234:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);


            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_FOUR_H_SUB1234\r\n");
            break;
        case MODE_FOUR_EQUALIZE:
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS4, pst_app_splitter_dev);
            //scale size
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][0]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[0].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[0].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[0].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);



            //window size
            pst_app_splitter_dev->st_video_win[0].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[0].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[0].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[0].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;

            //addr config
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][1]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[1].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[1].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[1].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[1].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[1].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[1].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
            pst_app_splitter_dev->st_video_win[1].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;

            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[0].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            //scale size
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][2]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[2].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[2].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[2].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            //window size
            pst_app_splitter_dev->st_video_win[2].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
            pst_app_splitter_dev->st_video_win[2].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[2].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            //addr config
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[1].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;


            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_hactive;
            pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][3]].u16_vactive;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            pst_app_splitter_dev->st_scale_size[3].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[3].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[3].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);


            pst_app_splitter_dev->st_video_win[3].u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            pst_app_splitter_dev->st_video_win[3].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
            pst_app_splitter_dev->st_video_win[3].u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            pst_app_splitter_dev->st_video_win[3].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 = pst_app_splitter_dev->st_dma_addr[2].u32_dma_start_addr_2 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_1080P_SIZE;
            pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_2 = pst_app_splitter_dev->st_dma_addr[3].u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_1080P_SIZE;

            app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
            app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);

            app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
            app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
            mculib_delay_ms(50);
            app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
            app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = g_u8_mixer_layer_src[0];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = g_u8_mixer_layer_src[1];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = g_u8_mixer_layer_src[2];
            g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = g_u8_mixer_layer_src[3];
            _splitter_video_unfreeze(u8dev);
            //audio cfg
            g_u8_vipss_audio_src[u8dev] = 0;
            g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][0];
            app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], pst_app_splitter_dev->u8_dst_sel);
            log("[INFO] SPLITTER MODE_FOUR_EQUALIZE\r\n");
            break;
    }
    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x01);
}
#endif
#if MS1826_SPLITTER_USER_MODE_ENABLE
VOID app_splitter_process(UINT8 u8dev,UINT8 idx)
{
    app_splitter_user_process(u8dev,idx);
}
#else
VOID app_splitter_process(UINT8 u8dev,UINT8 idx)
{
    switch(g_st_app_splitter_dev[u8dev].u8_splitter_mode)
    {
        case MODE_SIGNLE_1:
        case MODE_SIGNLE_2:
        case MODE_SIGNLE_3:
        case MODE_SIGNLE_4:
            //scale size
            if(idx == g_st_app_splitter_dev[u8dev].u8_splitter_mode)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[0].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][idx].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[0].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][idx].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[0].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[0].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[0].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[0].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[0].tOutSize.u16_h, MEMORY_4K_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[0].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[0].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[0].tOutSize.u16_v, MEMORY_4K_V_SIZE_MAX);
                idx = 0;
            }
            else
            {

                return;
            }
            break;
        case MODE_DUAL_UP1_DOWN2:
            if(idx == 0 || idx == 1)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_UP3_DOWN4:
            if(idx == 2 || idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_UP1_DOWN2_4_3:
            if(idx == 0 || idx == 1)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 * 4 / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_UP3_DOWN4_4_3:
            if(idx == 2 || idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 * 4 / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_LEFT1_RIGHT2:
            if(idx == 0 || idx == 1)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_LEFT3_RIGHT4:
            if(idx == 2 || idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_LEFT1_RIGHT2_4_3:
            if(idx == 0 || idx == 1)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_DUAL_LEFT3_RIGHT4_4_3:
            if(idx == 2 || idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_PIP_12_1_3:
            if(idx == 0)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else if(idx == 1)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_PIP_34_1_3:
            if(idx == 2)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else if(idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_PIP_21_1_3:
            if(idx == 1)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else if(idx == 0)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_PIP_43_1_3:
            if(idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else if(idx == 2)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                //scale size
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                return;
            }
            break;
        case MODE_THREE_EQUALIZE:
            if(idx == 0 || idx == 1 || idx == 2)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            break;
        case MODE_FOUR_MAIN1_SUB234:
        case MODE_FOUR_MAIN2_SUB134:
        case MODE_FOUR_MAIN3_SUB124:
        case MODE_FOUR_MAIN4_SUB123:
            if(idx == g_st_app_splitter_dev[u8dev].u8_splitter_mode - MODE_FOUR_MAIN1_SUB234)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            break;
        case MODE_FOUR_H_SUB1234:
            if(idx == 0 || idx == 3)
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            else
            {
                app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
                g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            }
            break;
        case MODE_FOUR_EQUALIZE:
            app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
            g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
            g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
            g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2;
            g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2;
            g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
            g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
            break;
    }

    //vipss sdn config
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_InRect.u16_h = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_InRect.u16_v = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;

    api_sdn_scaler_size_set(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev);
    //vipss vwdma config
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_in_size.u16_h_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_in_size.u16_v_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.u16_line_offset = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;

    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_0;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_1;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_2;
    api_vwdma_start_addr_cfg(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev);
    api_vwdma_line_offset_cfg(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev);
    api_vwdma_burst_num_cfg(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev);


     //vopss VRDMA config
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.b_enable = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.u16_line_offset = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_out_size.u16_h_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_out_size.u16_v_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_0;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_1;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_2;

    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);
    api_vrdma_start_addr_cfg(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);
    api_vrdma_burst_num_cfg(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);
    api_vrdma_line_offset_cfg(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);


    //vopss su config
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_su_dev.st_video_size.st_MemRect.u16_h = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_su_dev.st_video_size.st_MemRect.u16_v = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;
    api_su_scaler_size_set(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_su_dev);

    app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[idx]);
    app_video_vopss_trig(u8dev,g_u8_ss_ch_idx[idx]);
    mculib_delay_ms(200);
}
#endif

VOID app_splitter_rx_rotate(UINT8 u8dev,UINT8 idx, UINT8 u8_rotate)
{
    UINT16 u16_h_size;
    UINT16 u16_v_size;
    //rotate input clock must be lower than 148.5M
    if(Video_HDMIRx_Timing[u8dev][idx].u16_pixclk > 14900)
            return;

    _splitter_video_freeze(u8dev,VIPSS1);
    //vipss sdn config
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.u8_sel = g_u8_sdn_idx[VIPSS1];
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.b_enable = MS_TRUE;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.u8_sdn_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_InRect.u16_h =
    Video_HDMIRx_Timing[u8dev][idx].u16_hactive  > 0 ? Video_HDMIRx_Timing[u8dev][idx].u16_hactive : 1920;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_InRect.u16_v =
    Video_HDMIRx_Timing[u8dev][idx].u16_vactive  > 0 ? Video_HDMIRx_Timing[u8dev][idx].u16_vactive : 1080;
    if(u8_rotate % 2 > 0)
    {
        gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h =
                return_minof_three(gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_InRect.u16_h, g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive, 1080);
        gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v =
                return_minof_three(gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_InRect.u16_v, g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive, 1080);
    }
    else
    {
        gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h =
                return_minof_three(gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_InRect.u16_h, g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive, 1920);
        gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v =
                return_minof_three(gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_InRect.u16_v, g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive, 1080);
    }

    api_sdn_scaler_size_set(&gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev);

    //vipss vwdma config
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.u8_sel = g_u8_dma_idx[VIPSS1];
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.b_enable = MS_TRUE;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_in_size.u16_h_size =
            gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_in_size.u16_v_size =
            gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.u8_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.u16_line_offset = gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h;

    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = 0;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 + SINGLE_BUF_MEMORY_4K_SIZE;
    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 + SINGLE_BUF_MEMORY_4K_SIZE;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev);

    gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev.u8_vwdma_rotatation_deg = (u8_rotate == 2) ? 0: u8_rotate;
    api_vwdma_rotation_cfg(&gst_splitter_vipss_dev[u8dev][VIPSS1].st_splitter_vwdma_dev);

    app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[VIPSS1]);

     //vopss VRDMA config
    u16_h_size = gst_splitter_vipss_dev[u8dev][VOPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h;
    u16_v_size = gst_splitter_vipss_dev[u8dev][VOPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.u8_sel = g_u8_dma_idx[VOPSS1];
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.b_enable = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.u8_mem_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_out_size.u16_h_size = (u8_rotate % 2 > 0) ? u16_v_size : u16_h_size;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_out_size.u16_v_size = (u8_rotate % 2 > 0) ? u16_h_size : u16_v_size;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.u16_line_offset =
        gst_splitter_vipss_dev[u8dev][VOPSS1].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 =
        gst_splitter_vipss_dev[u8dev][VOPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 =
        gst_splitter_vipss_dev[u8dev][VOPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1;
	gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 =
		gst_splitter_vipss_dev[u8dev][VOPSS1].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2;

    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.u8_mirror_cfg = u8_rotate == 2 ? DMA_H_V_MIRROR_ALL_ENALBE : DMA_H_V_MIRROR_ALL_DISABLE;
    api_vrdma_mirror_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev);
    api_vrdma_start_addr_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev);
    api_vrdma_burst_num_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev);
    api_vrdma_line_offset_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev);
    api_vrdma_mirror_cfg(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev);

    //vopss su config
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.u8_sel = g_u8_su_idx[VOPSS1];
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.b_enable = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.u8_su_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_video_size.st_MemRect.u16_h =
        gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_out_size.u16_h_size;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_video_size.st_MemRect.u16_v =
             gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_vrdma_dev.st_vrdma_out_size.u16_v_size;

    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_video_win.u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_video_win.u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_video_win.u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset ;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_video_win.u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_dis_win.u8_dis_win_en = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_dis_win.u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_dis_win.u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_dis_win.u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset;
    gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev.st_dis_win.u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive;

    api_su_enable_set(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev);
    api_su_scaler_size_set(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev);
    api_su_csc_set(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev);
    api_su_video_win_set(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev);
    api_su_display_win_set(&gst_splitter_vopss_dev[u8dev][VOPSS1].st_splitter_su_dev);

    _splitter_video_unfreeze(u8dev);

}

VOID app_splitter_mode_audio_src(UINT8 u8dev)
{
    switch (g_st_app_splitter_dev[u8dev].u8_splitter_mode)
    {
    case MODE_SIGNLE_1:
        if(g_u8_splitter_audio_src[u8dev] == 0)
            g_u8_splitter_audio_src[u8dev] = 4;
        else
            g_u8_splitter_audio_src[u8dev] = 0;
        break;
    case MODE_SIGNLE_2:
        if(g_u8_splitter_audio_src[u8dev] == 1)
            g_u8_splitter_audio_src[u8dev] = 4;
        else
            g_u8_splitter_audio_src[u8dev] = 1;
        break;
    case MODE_SIGNLE_3:
        if(g_u8_splitter_audio_src[u8dev] == 2)
            g_u8_splitter_audio_src[u8dev] = 4;
        else
            g_u8_splitter_audio_src[u8dev] = 2;
        break;
    case MODE_SIGNLE_4:
        if(g_u8_splitter_audio_src[u8dev] == 3)
            g_u8_splitter_audio_src[u8dev] = 4;
        else
            g_u8_splitter_audio_src[u8dev] = 3;
        break;
    case MODE_THREE_EQUALIZE:
        g_u8_vipss_audio_src[u8dev] ++;
		if(g_u8_vipss_audio_src[u8dev] >= 3)
			g_u8_vipss_audio_src[u8dev] = 0;
		g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][g_u8_vipss_audio_src[u8dev]];
        break;
    case MODE_FOUR_MAIN1_SUB234:
    case MODE_FOUR_MAIN2_SUB134:
    case MODE_FOUR_MAIN3_SUB124:
    case MODE_FOUR_MAIN4_SUB123:
    case MODE_FOUR_EQUALIZE:
    case MODE_FOUR_H_SUB1234:
        g_u8_vipss_audio_src[u8dev] ++;
		if(g_u8_vipss_audio_src[u8dev] >= 4)
			g_u8_vipss_audio_src[u8dev] = 0;
		g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][g_u8_vipss_audio_src[u8dev]];
        break;
    case MODE_DUAL_LEFT1_RIGHT2:
    case MODE_DUAL_LEFT1_RIGHT2_4_3:
    case MODE_DUAL_UP1_DOWN2:
    case MODE_DUAL_UP1_DOWN2_4_3:
    case MODE_PIP_12_1_3:
    case MODE_PIP_21_1_3:
        g_u8_vipss_audio_src[u8dev] ++;
		if(g_u8_vipss_audio_src[u8dev] >= 2)
			g_u8_vipss_audio_src[u8dev] = 0;
		g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][g_u8_vipss_audio_src[u8dev]];
        break;
    case MODE_DUAL_LEFT3_RIGHT4:
    case MODE_DUAL_LEFT3_RIGHT4_4_3:
    case MODE_DUAL_UP3_DOWN4:
    case MODE_DUAL_UP3_DOWN4_4_3:
    case MODE_PIP_34_1_3:
    case MODE_PIP_43_1_3:
        g_u8_vipss_audio_src[u8dev] ++;
		if(g_u8_vipss_audio_src[u8dev] == 4)
			g_u8_vipss_audio_src[u8dev] = 2;
		g_u8_splitter_audio_src[u8dev] = g_u8SplitterDispMapping[u8dev][g_u8_vipss_audio_src[u8dev]];
        break;
    }
    if(g_u8_splitter_audio_src[u8dev] < 4)
    {
        b_audio_mute[u8dev][0] = false;
        b_audio_mute[u8dev][1] = false;
        b_audio_mute[u8dev][2] = false;
        b_audio_mute[u8dev][3] = false;
    }
    else
    {
        b_audio_mute[u8dev][0] = MS_TRUE;
        b_audio_mute[u8dev][1] = MS_TRUE;
        b_audio_mute[u8dev][2] = MS_TRUE;
        b_audio_mute[u8dev][3] = MS_TRUE;
    }
    app_tx_audio_cfg(u8dev,g_u8_splitter_audio_src[u8dev], g_st_app_splitter_dev[u8dev].u8_dst_sel);
    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}

VOID app_splitter_output_resolution(UINT8 u8dev,UINT8 u8_vic, SPLITTER_APP_T *pst_app_splitter_dev)
{
    PLL_DEV_T         pll_dev;
    MISC_MODULE_CLK_T st_module_clk;
    VDS_TMG_DEV_T     st_timgen_dev;
    UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].st_infoframes_packets.st_avi_infoframe.u8_video_code;

    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel], false);
    api_misc_timing_get(u8_vic, &g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel]);
    st_timgen_dev.pst_timing = &g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel];
    st_timgen_dev.u8_timgen_id = TIMGEN_1;
    st_timgen_dev.u8_workmode  = FreeRun_Mode;

    pll_dev.b_osc_input = MS_TRUE;
    pll_dev.u8_pll_idx = PLLV1;
    pll_dev.u32_clk_out = st_timgen_dev.pst_timing->u16_pixclk * 10000;

    st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_TIMGEN1;
    st_module_clk.u8_src_clk = MISC_CLKSRC_PLLV1;
    st_module_clk.u8_clkdiv = MISC_CLKDIV_1;

    api_pll_config(&pll_dev);
    api_misc_timgen_clkin_sel(&st_module_clk);
    api_vds_tmg_config(&st_timgen_dev);
    app_splitter_mode_config(u8dev,pst_app_splitter_dev);

    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_txIndex = g_u8_HDMI_TX_PROT[pst_app_splitter_dev->u8_dst_sel];
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][pst_app_splitter_dev->u8_dst_sel];
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].b_full_edid = FALSE;
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_color_depth = HDMI_COLOR_DEPTH_24;
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_audio_src = FROM_HDMI_RX_AUDIO;
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_pixclk;
    memcpy((&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
    g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel],false);
    api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel]);
    _video_port_hdmi_tx_driver_config(u8dev,pst_app_splitter_dev->u8_dst_sel);

    api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][pst_app_splitter_dev->u8_dst_sel], MS_TRUE);
    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}
VOID app_splitter_osd_init(UINT8 u8dev)
{
    UINT8 idx = 0;
    for(idx = 0; idx < 4; idx ++)
    {
        g_st_misc_osd_mux[u8dev][idx].b_enable = MS_TRUE;
        g_st_misc_osd_mux[u8dev][idx].u8_osd_module = g_u8_misc_osd_idx[idx];
        g_st_misc_osd_mux[u8dev][idx].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[g_st_app_splitter_dev[u8dev].u8_dst_sel]];
        api_misc_osd_timing_mux_cfg(&g_st_misc_osd_mux[u8dev][idx]);
    }
}

VOID app_splitter_osd_config(UINT8 u8dev)
{
    st_str[u8dev].u8_x = 0;
    st_str[u8dev].u8_y = 0;
    st_font[u8dev].u8_x = 0;
    st_font[u8dev].u8_y = 0;
    st_osd_win_size[u8dev].u8_osd_win = OSD_WIN_DISPLAY;
    st_osd_win_size[u8dev].u16_w = 15;
    st_osd_win_size[u8dev].u16_h = 4;
    app_splitter_osd_init(u8dev);
    switch(g_st_app_splitter_dev[u8dev].u8_splitter_mode)
    {
        case MODE_SIGNLE_1:
        case MODE_SIGNLE_2:
        case MODE_SIGNLE_3:
        case MODE_SIGNLE_4:
            st_osd_win_size[u8dev].u16_x = 100;
            st_osd_win_size[u8dev].u16_y = 100;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_st_app_splitter_dev[u8dev].u8_splitter_mode)
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_st_app_splitter_dev[u8dev].u8_splitter_mode];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_st_app_splitter_dev[u8dev].u8_splitter_mode];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_st_app_splitter_dev[u8dev].u8_splitter_mode);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);
            break;
        case MODE_DUAL_UP1_DOWN2:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_UP1_DOWN2_4_3:
            st_osd_win_size[u8dev].u16_x = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_UP3_DOWN4:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_UP3_DOWN4_4_3:
            st_osd_win_size[u8dev].u16_x = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 / 3 * 4) / 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_LEFT1_RIGHT2:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_LEFT1_RIGHT2_4_3:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 + 50;
            st_osd_win_size[u8dev].u16_y = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_LEFT3_RIGHT4:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_DUAL_LEFT3_RIGHT4_4_3:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 + 50;
            st_osd_win_size[u8dev].u16_y = (g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive - g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 / 4 * 3) / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_PIP_12_1_3:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_PIP_21_1_3:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_PIP_34_1_3:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != 3)
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[3];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[3];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,3);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_PIP_43_1_3:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);
            break;
        case MODE_THREE_EQUALIZE:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);
            break;
        case MODE_FOUR_MAIN1_SUB234:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][3], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][3], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][3], &st_str[u8dev]);
            app_osd_trigger(u8dev,3);
            break;
        case MODE_FOUR_MAIN2_SUB134:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][3], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][3], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][3], &st_str[u8dev]);
            app_osd_trigger(u8dev,3);
            break;
        case MODE_FOUR_MAIN3_SUB124:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][3], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][3], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][3], &st_str[u8dev]);
            app_osd_trigger(u8dev,3);
            break;
        case MODE_FOUR_MAIN4_SUB123:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 3 * 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][3], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][3], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][3], &st_str[u8dev]);
            app_osd_trigger(u8dev,3);
            break;
        case MODE_FOUR_H_SUB1234:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 3 * 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][3], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][3], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][3], &st_str[u8dev]);
            app_osd_trigger(u8dev,3);
            break;
        case MODE_FOUR_EQUALIZE:
            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][0])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][0]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][0]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][0], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][0]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][0], &st_str[u8dev]);
            app_osd_trigger(u8dev,0);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 + 50;
            st_osd_win_size[u8dev].u16_y = 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][1], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][1])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][1]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][1]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][1], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][1]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][1], &st_str[u8dev]);
            app_osd_trigger(u8dev,1);

            st_osd_win_size[u8dev].u16_x = 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][2], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][2])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][2]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][2]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][2], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][2]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][2], &st_str[u8dev]);
            app_osd_trigger(u8dev,2);

            st_osd_win_size[u8dev].u16_x = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive / 2 + 50;
            st_osd_win_size[u8dev].u16_y = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive / 2 + 50;
            api_osd_window_position_set(&g_st_osd_dev[u8dev][3], &st_osd_win_size[u8dev]);

            if(g_u8_splitter_audio_src[u8dev] != g_u8SplitterDispMapping[u8dev][3])
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[g_u8SplitterDispMapping[u8dev][3]];
            }
            else
            {
                st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel_audio[g_u8SplitterDispMapping[u8dev][3]];
            }
            st_font[u8dev].u16_length = 8;
            api_osd_show_index(&g_st_osd_dev[u8dev][3], &st_font[u8dev]);

            st_str[u8dev].u8_y = 1;
            set_osd_res(u8dev,g_u8SplitterDispMapping[u8dev][3]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][3], &st_str[u8dev]);
            app_osd_trigger(u8dev,3);
            break;
    }
}

//---------------------------------------------------------------------------------------------
void app_splitter_win_format_cfg(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bSubModeSw)
{
//    UINT8 u8Rx = pstMs1826WinTable->u8Rx;
    UINT8 u8Tx = pstMs1826WinTable->u8Tx;

//    g_st_app_splitter_dev[u8dev].u8_splitter_mode = MODE_FOUR_EQUALIZE;

    gst_splitter_vipss_dev[u8dev][u8WinID].st_splitter_vwdma_dev.b_enable = FALSE;
    api_vwdma_enable_set(&gst_splitter_vipss_dev[u8dev][u8WinID].st_splitter_vwdma_dev);

    gst_splitter_vopss_dev[u8dev][u8WinID].st_splitter_vrdma_dev.b_enable = FALSE;
    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][u8WinID].st_splitter_vrdma_dev);
    app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[u8WinID]);
    if(bSubModeSw == MS_TRUE)
    {
        _splitter_video_freeze(u8dev,VIPSS1+u8WinID);
        mculib_delay_ms(50);

        g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
        g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
        g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
        g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
        g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
        g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
        g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
        g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);
    }
    else
    {
        UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets.st_avi_infoframe.u8_video_code;

        //vipss1 instance init
        gst_splitter_vipss_dev[u8dev][VIPSS1+u8WinID].u8_vipss_sel = VIPSS1+u8WinID;
        app_splitter_vipss_instance_init(u8dev,&gst_splitter_vipss_dev[u8dev][VIPSS1+u8WinID]);

        //vopss1 instance init
        gst_splitter_vopss_dev[u8dev][VOPSS1+u8WinID].u8_vopss_sel = VOPSS1+u8WinID;
        app_splitter_vopss_instance_init(u8dev,&gst_splitter_vopss_dev[u8dev][VOPSS1+u8WinID]);

        //FRC
        _splitter_mode_frc_cfg(u8dev);

//        g_st_tx_mux[u8dev][u8Tx].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8Tx]];
//        g_st_tx_mux[u8dev][u8Tx].u8_video_mux = g_u8_misc_mixer[0];//MISC_VIDEO_MODULE_MIX1+u8Tx;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8Tx]);

        g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
        g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
        g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
        g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
        g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
        g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
        g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
        g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);

        g_st_hdmi_tx[u8dev][u8Tx].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8Tx];
        g_st_hdmi_tx[u8dev][u8Tx].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][u8Tx].u8_color_depth = HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][u8Tx].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
        g_st_hdmi_tx[u8dev][u8Tx].u8_audio_src = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][u8Tx].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][u8Tx].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_pixclk;
        memcpy((&g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8Tx],false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8Tx]);
        _video_port_hdmi_tx_driver_config(u8dev,u8Tx);

        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8Tx],MS_TRUE);
        _app_video_splitter_mixer_cfg(u8dev,VIDEO_MIXER_1+u8Tx);

        //vopss tx csc para
        g_st_txcsc_dev[u8dev][u8Tx].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8Tx]];
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
        api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8Tx]);
    }
    app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1+u8WinID, &g_st_app_splitter_dev[u8dev]);
}

void app_splitter_win_cfg(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinCfg,BOOL bIsDsc,BOOL bUpdate)
{
    SPLITTER_APP_T *pst_app_splitter_dev = &g_st_app_splitter_dev[u8dev];
    UINT8 u8Rx  = pstMs1826WinCfg->u8Rx;
    UINT8 u8Tx  = pstMs1826WinCfg->u8Tx;
    UINT16 u16X = pstMs1826WinCfg->u16X;
    UINT16 u16Y = pstMs1826WinCfg->u16Y;
    UINT16 u16W = pstMs1826WinCfg->u16W;
    UINT16 u16H = pstMs1826WinCfg->u16H;

    if(u8WinID>4)
    {
        log("Window id error\r\n");
        return;
    }

    //scale size
    pst_app_splitter_dev->st_scale_size[u8WinID].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][u8WinID]].u16_hactive;
    pst_app_splitter_dev->st_scale_size[u8WinID].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][u8WinID]].u16_vactive;
    pst_app_splitter_dev->st_scale_size[u8WinID].tOutSize.u16_h = ((/*(u16W==0)||*/(u16W>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive):(u16W));
    pst_app_splitter_dev->st_scale_size[u8WinID].tOutSize.u16_v = ((/*(u16H==0)||*/(u16H>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive):(u16H));
    pst_app_splitter_dev->st_scale_size[u8WinID].tMemSize.u16_h = return_minof_three(pst_app_splitter_dev->st_scale_size[u8WinID].tInSize.u16_h, pst_app_splitter_dev->st_scale_size[u8WinID].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
    pst_app_splitter_dev->st_scale_size[u8WinID].tMemSize.u16_v = return_minof_three(pst_app_splitter_dev->st_scale_size[u8WinID].tInSize.u16_v, pst_app_splitter_dev->st_scale_size[u8WinID].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

    //window size
    pst_app_splitter_dev->st_video_win[u8WinID].u16_hde_st =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hoffset +
            ((u16X < g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive)?(u16X):(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive));
    pst_app_splitter_dev->st_video_win[u8WinID].u16_hde_sp =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hoffset +
            ((/*(u16W==0)||*/(u16W>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive):(u16W+u16X));
    pst_app_splitter_dev->st_video_win[u8WinID].u16_vde_st =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_voffset +
            ((u16Y < g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive)?(u16Y):(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive));
    pst_app_splitter_dev->st_video_win[u8WinID].u16_vde_sp =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_voffset +
            ((/*(u16H==0)||*/(u16H>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive):(u16H+u16Y));

    // if sp out of range
    if((pst_app_splitter_dev->st_video_win[u8WinID].u16_hde_sp-g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hoffset)>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive)
    {
        pst_app_splitter_dev->st_video_win[u8WinID].u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive;
    }

    if((pst_app_splitter_dev->st_video_win[u8WinID].u16_vde_sp-g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_voffset)>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive)
    {
        pst_app_splitter_dev->st_video_win[u8WinID].u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive;
    }
    //addr config
    pst_app_splitter_dev->st_dma_addr[u8WinID].u32_dma_start_addr_0 = (MEMORY_BUF_ADDR_START)+3*u8WinID*(SINGLE_BUF_MEMORY_1080P_SIZE);
    pst_app_splitter_dev->st_dma_addr[u8WinID].u32_dma_start_addr_1 = (MEMORY_BUF_ADDR_START)+3*u8WinID*(SINGLE_BUF_MEMORY_1080P_SIZE) + (SINGLE_BUF_MEMORY_1080P_SIZE);
    pst_app_splitter_dev->st_dma_addr[u8WinID].u32_dma_start_addr_2 = (MEMORY_BUF_ADDR_START)+3*u8WinID*(SINGLE_BUF_MEMORY_1080P_SIZE) + 2*(SINGLE_BUF_MEMORY_1080P_SIZE);

    log("g_u8SplitterDispMapping[%d]      == %d\r\n",u8WinID,g_u8SplitterDispMapping[u8dev][u8WinID]);

    log("st_scale_size[%d].tInSize.u16_h  == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_scale_size[u8WinID].tInSize.u16_h);
    log("st_scale_size[%d].tInSize.u16_h  == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_scale_size[u8WinID].tInSize.u16_v);
    log("st_scale_size[%d].tOutSize.u16_h == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_scale_size[u8WinID].tOutSize.u16_h);
    log("st_scale_size[%d].tOutSize.u16_v == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_scale_size[u8WinID].tOutSize.u16_v);
    log("st_scale_size[%d].tMemSize.u16_h == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_scale_size[u8WinID].tMemSize.u16_h);
    log("st_scale_size[%d].tMemSize.u16_v == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_scale_size[u8WinID].tMemSize.u16_v);

    log("st_video_win[%d].u16_hde_st      == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_video_win[u8WinID].u16_hde_st);
    log("st_video_win[%d].u16_hde_sp      == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_video_win[u8WinID].u16_hde_sp);
    log("st_video_win[%d].u16_vde_st      == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_video_win[u8WinID].u16_vde_st);
    log("st_video_win[%d].u16_vde_sp      == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_video_win[u8WinID].u16_vde_sp);

    log("st_dma_addr[%d].u32_dma_start_addr_0 == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_dma_addr[u8WinID].u32_dma_start_addr_0);
    log("st_dma_addr[%d].u32_dma_start_addr_1 == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_dma_addr[u8WinID].u32_dma_start_addr_1);
    log("st_dma_addr[%d].u32_dma_start_addr_2 == 0x%08x\r\n",u8WinID,pst_app_splitter_dev->st_dma_addr[u8WinID].u32_dma_start_addr_2);
    if(bUpdate)
    {
        app_splitter_vipss_cfg(u8dev,VIPSS1+u8WinID, pst_app_splitter_dev);

        app_video_vipss_trig(u8dev,(bIsDsc==MS_TRUE)?g_u8_ss_ch_dsc_idx[u8WinID]:g_u8_ss_ch_idx[u8WinID]);
        mculib_delay_ms(50);
        app_splitter_vopss_cfg(u8dev,VOPSS1+u8WinID, pst_app_splitter_dev);

//        g_st_tx_mux[u8dev][u8WinID].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8WinID]];
//        g_st_tx_mux[u8dev][u8WinID].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8WinID]);

        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[3]:g_u8_mixer_layer_src[3];
        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[2]:g_u8_mixer_layer_src[2];
        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[1]:g_u8_mixer_layer_src[1];
        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[0]:g_u8_mixer_layer_src[0];
        _splitter_video_unfreeze(u8dev);
    }
    log("[INFO] app_splitter_win_cfg rx[%d] tx[%d] win[%d] x[%d] y[%d] w[%d] h[%d] dsc[%d] update[%d]\r\n",u8Rx,u8Tx,u8WinID,u16X,u16Y,u16W,u16H,bIsDsc,bUpdate);
}

void app_splitter_user_mode_win_update(UINT8 u8dev,BOOL bIsDsc,UINT8 u8Path)
{
    SPLITTER_APP_T *pst_app_splitter_dev = &g_st_app_splitter_dev[u8dev];

    if(u8Path&0x01)
        app_splitter_vipss_cfg(u8dev,VIPSS1, pst_app_splitter_dev);
    if(u8Path&0x02)
        app_splitter_vipss_cfg(u8dev,VIPSS2, pst_app_splitter_dev);
    if(u8Path&0x04)
        app_splitter_vipss_cfg(u8dev,VIPSS3, pst_app_splitter_dev);
    if(u8Path&0x08)
        app_splitter_vipss_cfg(u8dev,VIPSS4, pst_app_splitter_dev);

    if(u8Path&0x01)
        app_video_vipss_trig(u8dev,(bIsDsc==MS_TRUE)?g_u8_ss_ch_dsc_idx[0]:g_u8_ss_ch_idx[0]);
    if(u8Path&0x02)
        app_video_vipss_trig(u8dev,(bIsDsc==MS_TRUE)?g_u8_ss_ch_dsc_idx[1]:g_u8_ss_ch_idx[1]);
    if(u8Path&0x04)
        app_video_vipss_trig(u8dev,(bIsDsc==MS_TRUE)?g_u8_ss_ch_dsc_idx[2]:g_u8_ss_ch_idx[2]);
    if(u8Path&0x08)
        app_video_vipss_trig(u8dev,(bIsDsc==MS_TRUE)?g_u8_ss_ch_dsc_idx[3]:g_u8_ss_ch_idx[3]);
    mculib_delay_ms(50);
    if(u8Path&0x01)
        app_splitter_vopss_cfg(u8dev,VOPSS1, pst_app_splitter_dev);
    if(u8Path&0x02)
        app_splitter_vopss_cfg(u8dev,VOPSS2, pst_app_splitter_dev);
    if(u8Path&0x04)
        app_splitter_vopss_cfg(u8dev,VOPSS3, pst_app_splitter_dev);
    if(u8Path&0x08)
        app_splitter_vopss_cfg(u8dev,VOPSS4, pst_app_splitter_dev);

//    g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
//    g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//    api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
//    g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
//    g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//    api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
//    g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
//    g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//    api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
//    g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
//    g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//    api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);

    g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[3]:g_u8_mixer_layer_src[3];
    g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[2]:g_u8_mixer_layer_src[2];
    g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[1]:g_u8_mixer_layer_src[1];
    g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[0]:g_u8_mixer_layer_src[0];
    _splitter_video_unfreeze(u8dev);

}

void app_splitter_user_mode_win_config(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bIsDsc,BOOL bUpdate,BOOL bIsModeSw)
{
    app_splitter_win_format_cfg(u8dev,u8WinID,pstMs1826WinTable,bIsModeSw);
    app_splitter_win_cfg(u8dev,u8WinID,pstMs1826WinTable,bIsDsc,bUpdate);
}

const stWinTypeFactorType_t g_pu8MultiWinTable[emMvMode_max][4] =
{
    //   Xsf1,Xsf2,Ysf1,Ysf2,Wsf1,Wsf2,Hsf1,Hsf2
    //emMvMode_1Win_Full=0,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PBP,
    {
        {   0, 100, 25, 100, 50,  100, 50,  100},
        {  50, 100, 25, 100, 50,  100, 50,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_3Win_2U1D,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {  25, 100, 50, 100, 50,  100, 50,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_4Win_SameSize,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_2Win_PIP_LU,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100,  0, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PIP_LD,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100, 75, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PIP_RU,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  75, 100,  0, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PIP_RD,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  75, 100, 75, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_4Win_PBP_3L1R,
    {
        {   6,  16,  7,  32,  9,   16,  9,   16},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   1,  16,  6,  16,  4,   16,  4,   16},
        {   1,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PBP_1L3R,
    {
        {   1,  16,  7,  32,  9,   16,  9,   16},
        {  11,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  6,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PBP_3U1D,
    {
        {   7,  32,  6,  16,  9,   16,  9,   16},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   6,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  1,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PBP_1U3D,
    {
        {   7,  32,  1,  16,  9,   16,  9,   16},
        {   1,  16, 11,  16,  4,   16,  4,   16},
        {   6,  16, 11,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3L,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   1,  16,  6,  16,  4,   16,  4,   16},
        {   1,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3R,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {  11,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  6,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3U,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   6,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  1,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3D,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {   1,  16, 11,  16,  4,   16,  4,   16},
        {   6,  16, 11,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_xWin_ByUser1,
//    {
//        {   0, 100,  0, 100, 50,  100, 50,  100},
//        {  50, 100,  0, 100, 50,  100, 50,  100},
//        {   0, 100, 50, 100, 50,  100, 50,  100},
//        {  50, 100, 50, 100, 50,  100, 50,  100},
//    },
    //Three equalize
//    {
//        {   0,   3,   0,   3,   1,  3,   1,   1},
//        {   1,   3,   0,   3,   1,  3,   1,   1},
//        {   2,   3,   0,   3,   1,  3,   1,   1},
//        {   0,   1,   0,   1,   0,  1,   0,   1},
//    },
    //1L3R_Tile
//    {
//        {   0,   3,   0,   3,   2,  3,   1,   1},
//        {   2,   3,   0,   3,   1,  3,   1,   3},
//        {   2,   3,   1,   3,   1,  3,   1,   3},
//        {   2,   3,   2,   3,   1,  3,   1,   3},
//    },
    // 1R3L_Tile
//    {
//        {   1,   3,   0,   3,   2,  3,   1,   1},
//        {   0,   3,   0,   3,   1,  3,   1,   3},
//        {   0,   3,   1,   3,   1,  3,   1,   3},
//        {   0,   3,   2,   3,   1,  3,   1,   3},
//    },
    // 1U3D_Tile
//    {
//        {   0,   3,   0,   3,   1,  1,   2,   3},
//        {   0,   3,   2,   3,   1,  3,   1,   3},
//        {   1,   3,   2,   3,   1,  3,   1,   3},
//        {   2,   3,   2,   3,   1,  3,   1,   3},
//    },
    // 1D3U_Tile
    {
        {   0,   3,   1,   3,   1,  1,   2,   3},
        {   0,   3,   0,   3,   1,  3,   1,   3},
        {   1,   3,   0,   3,   1,  3,   1,   3},
        {   2,   3,   0,   3,   1,  3,   1,   3},
    },

    //emMvMode_xWin_ByUser2,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_xWin_ByUser3,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_xWin_ByUser4,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
};

UINT16 app_GetMultiWinStartX(UINT8 u8dev, UINT8 u8Type, UINT8 u8DispWin)
{
    UINT16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf2);
    }

    if(l_u16Temp%8)
    {
        l_u16Temp -= l_u16Temp%8;
    }

    if(l_u16Temp > g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive)
    {
        l_u16Temp = g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive;
    }

    return l_u16Temp;
}

UINT16 app_GetMultiWinStartY(UINT8 u8dev, UINT8 u8Type, UINT8 u8DispWin)
{
    UINT16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf2);
    }

    if(l_u16Temp%8)
    {
        l_u16Temp -= l_u16Temp%8;
    }

    if(l_u16Temp > g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive)
    {
        l_u16Temp = g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive;
    }
    return l_u16Temp;
}

UINT16 app_GetMultiWinWidth(UINT8 u8dev, UINT8 u8Type, UINT8 u8DispWin)
{
    UINT16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf2);
    }


    if(l_u16Temp%8)
    {
        l_u16Temp -= l_u16Temp%8;
    }

    if(l_u16Temp > g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive)
    {
        l_u16Temp = g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_hactive;
    }

    return l_u16Temp;
}

UINT16 app_GetMultiWinHeight(UINT8 u8dev, UINT8 u8Type, UINT8 u8DispWin)
{
    UINT16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf1*g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf2);
    }

    if(l_u16Temp%8)
    {
        l_u16Temp -= l_u16Temp%8;
    }

    if(l_u16Temp > g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive)
    {
        l_u16Temp = g_stVideo_HDMITx_Timing[u8dev][u8DispWin].u16_vactive;
    }

    return l_u16Temp;
}

void app_splitter_user_mode_config(UINT8 u8dev,SPLITTER_APP_T *pst_app_splitter_dev)
{
    UINT8 l_u8Mode = pst_app_splitter_dev->u8_splitter_mode;

    g_stMs1826WinTable[u8dev][0].u8Rx = g_u8SplitterDispMapping[u8dev][0];
    g_stMs1826WinTable[u8dev][0].u8Tx = 0;
    g_stMs1826WinTable[u8dev][0].u16X = app_GetMultiWinStartX(u8dev,l_u8Mode,0);
    g_stMs1826WinTable[u8dev][0].u16Y = app_GetMultiWinStartY(u8dev,l_u8Mode,0);
    g_stMs1826WinTable[u8dev][0].u16W = app_GetMultiWinWidth( u8dev,l_u8Mode,0);
    g_stMs1826WinTable[u8dev][0].u16H = app_GetMultiWinHeight(u8dev,l_u8Mode,0);
    app_tx_audio_cfg(u8dev,g_stMs1826WinTable[u8dev][0].u8Rx, 0);

    g_stMs1826WinTable[u8dev][1].u8Rx = g_u8SplitterDispMapping[u8dev][1];
    g_stMs1826WinTable[u8dev][1].u8Tx = 0;
    g_stMs1826WinTable[u8dev][1].u16X = app_GetMultiWinStartX(u8dev,l_u8Mode,1);
    g_stMs1826WinTable[u8dev][1].u16Y = app_GetMultiWinStartY(u8dev,l_u8Mode,1);
    g_stMs1826WinTable[u8dev][1].u16W = app_GetMultiWinWidth( u8dev,l_u8Mode,1);
    g_stMs1826WinTable[u8dev][1].u16H = app_GetMultiWinHeight(u8dev,l_u8Mode,1);
    app_tx_audio_cfg(u8dev,g_stMs1826WinTable[u8dev][1].u8Rx, 1);

    g_stMs1826WinTable[u8dev][2].u8Rx = g_u8SplitterDispMapping[u8dev][2];
    g_stMs1826WinTable[u8dev][2].u8Tx = 0;
    g_stMs1826WinTable[u8dev][2].u16X = app_GetMultiWinStartX(u8dev,l_u8Mode,2);
    g_stMs1826WinTable[u8dev][2].u16Y = app_GetMultiWinStartY(u8dev,l_u8Mode,2);
    g_stMs1826WinTable[u8dev][2].u16W = app_GetMultiWinWidth( u8dev,l_u8Mode,2);
    g_stMs1826WinTable[u8dev][2].u16H = app_GetMultiWinHeight(u8dev,l_u8Mode,2);
    app_tx_audio_cfg(u8dev,g_stMs1826WinTable[u8dev][2].u8Rx, 2);

    g_stMs1826WinTable[u8dev][3].u8Rx = g_u8SplitterDispMapping[u8dev][3];
    g_stMs1826WinTable[u8dev][3].u8Tx = 0;
    g_stMs1826WinTable[u8dev][3].u16X = app_GetMultiWinStartX(u8dev,l_u8Mode,3);
    g_stMs1826WinTable[u8dev][3].u16Y = app_GetMultiWinStartY(u8dev,l_u8Mode,3);
    g_stMs1826WinTable[u8dev][3].u16W = app_GetMultiWinWidth( u8dev,l_u8Mode,3);
    g_stMs1826WinTable[u8dev][3].u16H = app_GetMultiWinHeight(u8dev,l_u8Mode,3);
    app_tx_audio_cfg(u8dev,g_stMs1826WinTable[u8dev][3].u8Rx, 3);

    app_splitter_user_mode_win_config(u8dev,0,&g_stMs1826WinTable[u8dev][0],FALSE,FALSE,pst_app_splitter_dev->b_splitter_sw);
    app_splitter_user_mode_win_config(u8dev,1,&g_stMs1826WinTable[u8dev][1],FALSE,FALSE,pst_app_splitter_dev->b_splitter_sw);
    app_splitter_user_mode_win_config(u8dev,2,&g_stMs1826WinTable[u8dev][2],FALSE,FALSE,pst_app_splitter_dev->b_splitter_sw);
    app_splitter_user_mode_win_config(u8dev,3,&g_stMs1826WinTable[u8dev][3],FALSE,FALSE,pst_app_splitter_dev->b_splitter_sw);
    app_splitter_user_mode_win_update(u8dev,FALSE,0x0F);
}

void app_splitter_user_process(UINT8 u8dev,UINT8 idx)
{
    app_splitter_vipss_rx_mux_cfg(u8dev,idx, &g_st_app_splitter_dev[u8dev]);
//    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][idx].u16_hactive;
//    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][idx].u16_vactive;
    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive;
    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v = Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive;
#if 0
    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h =
            (((g_stMs1826WinTable[u8dev][idx].u16W>g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive))
            ?(g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_hactive):(g_stMs1826WinTable[u8dev][idx].u16W));
    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v =
            (((g_stMs1826WinTable[u8dev][idx].u16H>g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive))
            ?(g_stVideo_HDMITx_Timing[u8dev][g_st_app_splitter_dev[u8dev].u8_dst_sel].u16_vactive):(g_stMs1826WinTable[u8dev][idx].u16H));
#endif
//    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][idx].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
//    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][idx].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);
    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_hactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
    g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][g_u8SplitterDispMapping[u8dev][idx]].u16_vactive, g_st_app_splitter_dev[u8dev].st_scale_size[idx].tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

    log("g_u8SplitterDispMapping[%d]      == %d\r\n",idx,g_u8SplitterDispMapping[u8dev][idx]);
    log("2 st_scale_size[%d].tInSize.u16_h  == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_scale_size[idx].tInSize.u16_h);
    log("2 st_scale_size[%d].tInSize.u16_h  == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_scale_size[idx].tInSize.u16_v);
    log("2 st_scale_size[%d].tOutSize.u16_h == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_scale_size[idx].tOutSize.u16_h);
    log("2 st_scale_size[%d].tOutSize.u16_v == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_scale_size[idx].tOutSize.u16_v);
    log("2 st_scale_size[%d].tMemSize.u16_h == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_scale_size[idx].tMemSize.u16_h);
    log("2 st_scale_size[%d].tMemSize.u16_v == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_scale_size[idx].tMemSize.u16_v);

    log("2 st_video_win[%d].u16_hde_st == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_video_win[idx].u16_hde_st);
    log("2 st_video_win[%d].u16_hde_sp == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_video_win[idx].u16_hde_sp);
    log("2 st_video_win[%d].u16_vde_st == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_video_win[idx].u16_vde_st);
    log("2 st_video_win[%d].u16_vde_sp == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_video_win[idx].u16_vde_sp);

    log("2 st_dma_addr[%d].u32_dma_start_addr_0 == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_dma_addr[idx].u32_dma_start_addr_0);
    log("2 st_dma_addr[%d].u32_dma_start_addr_1 == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_dma_addr[idx].u32_dma_start_addr_1);
    log("2 st_dma_addr[%d].u32_dma_start_addr_2 == 0x%08x\r\n",idx,g_st_app_splitter_dev->st_dma_addr[idx].u32_dma_start_addr_2);

    //vipss sdn config
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_InRect.u16_h = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_h;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_InRect.u16_v = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tInSize.u16_v;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_h = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev.st_video_size.st_MemRect.u16_v = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;

    api_sdn_scaler_size_set(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_sdn_dev);
    //vipss vwdma config
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_in_size.u16_h_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_in_size.u16_v_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.u16_line_offset = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;

    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_0;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_1;
    gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_2;
    api_vwdma_start_addr_cfg(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev);
    api_vwdma_line_offset_cfg(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev);
    api_vwdma_burst_num_cfg(&gst_splitter_vipss_dev[u8dev][idx].st_splitter_vwdma_dev);


     //vopss VRDMA config
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.b_enable = MS_TRUE;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.u16_line_offset = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_out_size.u16_h_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_out_size.u16_v_size = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_0;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_1;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = g_st_app_splitter_dev[u8dev].st_dma_addr[idx].u32_dma_start_addr_2;

    api_vrdma_enable_set(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);
    api_vrdma_start_addr_cfg(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);
    api_vrdma_burst_num_cfg(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);
    api_vrdma_line_offset_cfg(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_vrdma_dev);

    //vopss su config
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_su_dev.st_video_size.st_MemRect.u16_h = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_h;
    gst_splitter_vopss_dev[u8dev][idx].st_splitter_su_dev.st_video_size.st_MemRect.u16_v = g_st_app_splitter_dev[u8dev].st_scale_size[idx].tMemSize.u16_v;
    api_su_scaler_size_set(&gst_splitter_vopss_dev[u8dev][idx].st_splitter_su_dev);

    app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[idx]);
    app_video_vopss_trig(u8dev,g_u8_ss_ch_idx[idx]);
    mculib_delay_ms(200);

}

VOID app_splitter_mode_rx_switch(UINT8 u8dev,UINT8 u8_rx, UINT8 u8_tx)
{
#if 0
    u8 l_u8WinId;

    // change old source
    for(l_u8WinId = 0; l_u8WinId < 4; l_u8WinId++)
    {
        if(g_u8SplitterDispMapping[u8dev][l_u8WinId] == u8_rx)
        {
            g_u8SplitterDispMapping[u8dev][l_u8WinId] = g_u8SplitterDispMapping[u8dev][u8_tx];
            break;
        }
    }
#endif

    g_u8SplitterDispMapping[u8dev][u8_tx] = u8_rx;

    g_st_app_splitter_dev[u8dev].b_splitter_sw = MS_TRUE;
    app_splitter_user_mode_config(u8dev,&g_st_app_splitter_dev[u8dev]);
}

void app_splitter_user_mode_test(void)
{
    stMs1826Wincfg_T stMs1826WinTable[4];

    stMs1826WinTable[0].u8Rx = g_u8SplitterDispMapping[0][0];
    stMs1826WinTable[0].u8Tx = 0;
    stMs1826WinTable[0].u16X = 0;
    stMs1826WinTable[0].u16Y = 0;
    stMs1826WinTable[0].u16W = 960;
    stMs1826WinTable[0].u16H = 540;

    stMs1826WinTable[1].u8Rx = g_u8SplitterDispMapping[0][1];
    stMs1826WinTable[1].u8Tx = 0;
    stMs1826WinTable[1].u16X = 960;
    stMs1826WinTable[1].u16Y = 0;
    stMs1826WinTable[1].u16W = 960;
    stMs1826WinTable[1].u16H = 540;

    stMs1826WinTable[2].u8Rx = g_u8SplitterDispMapping[0][2];
    stMs1826WinTable[2].u8Tx = 0;
    stMs1826WinTable[2].u16X = 0;
    stMs1826WinTable[2].u16Y = 540;
    stMs1826WinTable[2].u16W = 960;
    stMs1826WinTable[2].u16H = 540;

    stMs1826WinTable[3].u8Rx = g_u8SplitterDispMapping[0][3];
    stMs1826WinTable[3].u8Tx = 0;
    stMs1826WinTable[3].u16X = 960;
    stMs1826WinTable[3].u16Y = 540;
    stMs1826WinTable[3].u16W = 960;
    stMs1826WinTable[3].u16H = 540;

    app_splitter_user_mode_win_config(0,0,&stMs1826WinTable[0],FALSE,TRUE,TRUE);
    app_splitter_user_mode_win_config(0,1,&stMs1826WinTable[1],FALSE,TRUE,TRUE);
    app_splitter_user_mode_win_config(0,2,&stMs1826WinTable[2],FALSE,TRUE,TRUE);
    app_splitter_user_mode_win_config(0,3,&stMs1826WinTable[3],FALSE,TRUE,TRUE);
}

#endif
