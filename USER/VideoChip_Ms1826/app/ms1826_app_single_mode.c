#ifndef __MS1826_APP_SINGLE_MODE_C_
#define __MS1826_APP_SINGLE_MODE_C_

#include "includes.h"
#if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)
#include "ms1826_videoprocess.h"


SINGLE_VIPSS_T gst_single_vipss_dev[DEV_NUM_MS1826][4];
SINGLE_VOPSS_T gst_single_vopss_dev[DEV_NUM_MS1826][4];
BOOL b_freeze_status[DEV_NUM_MS1826][4] = {FALSE};
BOOL g_dsc_enable[DEV_NUM_MS1826];
UINT8 b_init[DEV_NUM_MS1826][4] = {0,0,0,0};
SINGLE_APP_T g_st_single_app_dev[DEV_NUM_MS1826][4];
extern VOID ms1826_hal_vehc_trig(UINT8 u8_vehc_idx, BOOL b_imm);
extern UINT16 ms1826_hal_hdmi_rx_get_tmds_clk(UINT8 u8_idx);
extern VOID ms1826_hal_hdmi_rx_phy_power_toggle(UINT8 u8_idx);
extern UINT8 ms1826_hal_hdmi_rx_phy_process(UINT8 u8_idx, BOOL b_rx_tmds_clk_change, UINT16 u16_rx_tmds_clk);
extern UINT32 ms1826_hal_hdmi_rx_get_mdt_ints_ext(UINT8 u8_idx);
extern void app_single_user_splitter_process(UINT8 u8dev,UINT8 u8_idx);

static VOID _single_vipss_instance_init(UINT8 u8dev,SINGLE_VIPSS_T *pst_single_vipss_dev)
{
    UINT8 u8_path_sel = pst_single_vipss_dev->u8_vipss_sel;

    api_sdn_instance_init(&pst_single_vipss_dev->st_single_sdn_dev, g_u8_sdn_idx[u8_path_sel]);

    if(pst_single_vipss_dev->b_is_dsc)
    {
        gst_single_vipss_dev[u8dev][u8_path_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_dsc_idx[u8_path_sel];
        api_vwdma_instance_init(&pst_single_vipss_dev->st_single_vwdma_dev, g_u8_dma_dsc_idx[u8_path_sel]);
        api_dsc_encoder_instance_init(&pst_single_vipss_dev->st_encoder_dev,g_u8_dsc_idx[u8_path_sel]);
    }
    else
    {
        gst_single_vipss_dev[u8dev][u8_path_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
        api_vwdma_instance_init(&pst_single_vipss_dev->st_single_vwdma_dev, g_u8_dma_idx[u8_path_sel]);
    }

    if(g_bSplitterEnable[u8dev][pst_single_vipss_dev->u8_vipss_sel])
    {
        api_4ksdn_instance_init(&pst_single_vipss_dev->st_sdn4k_dev, g_u8_4ksdn_idx[g_u8_rx_idx[pst_single_vipss_dev->u8_vipss_sel]]);
        api_dma_common_instance_init(&pst_single_vipss_dev->st_single_com_wdma_dev, g_u8_dma_idx[g_u8_rx_idx[pst_single_vipss_dev->u8_vipss_sel]]);
    }
}
VOID _single_cfg(UINT8 u8dev,BOOL b_is_dsc,UINT8 u8_vipss_sel)
{
    UINT16 u16_in_size_h[4], u16_in_size_v[4];
#if (MS1826_VIPSS1_DEINT==0)
    u16_in_size_h[u8_vipss_sel] = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_hactive;
    u16_in_size_v[u8_vipss_sel] = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive;
#endif
    //vipss rx sel
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[Mapp_Rx[u8dev][u8_vipss_sel]]];
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_dst_module = g_u8_misc_module_vipss[u8_vipss_sel];
    api_misc_rx_mux_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev);
    log("gst_single_vipss_dev[%d][%d].st_vipss_mux_dev.u8_src_module = %d\r\n",u8dev,u8_vipss_sel,gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module);

    //vipss rx csc config
    g_st_vipss_csc[u8dev][u8_vipss_sel].u8_sel = g_u8_csc_idx[u8_vipss_sel];
    if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u8_color_space == HDMI_COLORSPACE_YCBCR420)
    {
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;//just for bypass
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
    }
    else
    {
        #if 1
        if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
            g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = HDMI_COLORSPACE_RGB;
        else
        #endif
            g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u8_color_space;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.u8_pixel_repeat  = HDMI_PIXEL_REPEAT_X1;
    }
    if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].b_is_hdmi == false)
		g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = HDMI_COLORSPACE_RGB;
    api_csc_rx_set(&g_st_vipss_csc[u8dev][u8_vipss_sel]);
    ms1826_hal_vehc_trig(u8_vipss_sel+4,MS_TRUE);       // 解决上电和插拔输入端，亮度对比度恢复默认的问题

    if(b_is_dsc)
    {
        //vipss mux config
        if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u8_color_space == HDMI_COLORSPACE_YCBCR420)
        {
            api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_6);  /**< CSC -> 4KSDN -> SDN -> DSC_E -> VWDMA0 */
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_idx = g_u8_4ksdn_idx[u8_vipss_sel];
            api_sdn4k_init(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_input_colorSpace = HDMI_COLORSPACE_YCBCR420;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_output_colorSpace = HDMI_COLORSPACE_YCBCR444;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_mode = SDN4K_MODE_AUTO;
            api_sdn4k_config(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
            api_sdn4k_enable(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev, MS_ENABLE);
            u16_in_size_v[u8_vipss_sel] = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive / 2;
        }
        else
        {
            api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_3);  //**< CSC -> SDN -> DSC_E -> VWDMA0 */
//          api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_2);  //**< CSC -> SPLIT -> DSC_E -> VWDMA0 */
        }

        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.u8_sel = g_u8_sdn_idx[u8_vipss_sel];
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.b_enable = MS_TRUE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.u8_sdn_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h = u16_in_size_h[u8_vipss_sel];
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v = u16_in_size_v[u8_vipss_sel];
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h =
            return_minof_three(gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h, g_stVideo_HDMITx_Timing[u8dev][u8_vipss_sel].u16_hactive, MEMORY_1080P_H_SIZE_MAX);
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v =
            return_minof_three(gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v, g_stVideo_HDMITx_Timing[u8dev][u8_vipss_sel].u16_vactive, MEMORY_1080P_V_SIZE_MAX);
        api_sdn_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
        api_sdn_scaler_size_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
        api_sdn_csc_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);

        //vipss dsc config
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.u8_sel = g_u8_dsc_idx[u8_vipss_sel];
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u16_hactive                = gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u16_vactive                = gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u8_color_space             = CHIP_INTERNAL_PROCESS_COLORSPACE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u8_dsc_data_path_clk       = PXL_CLK_DIV2;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u8_dsc_ratio               = DSC_RATIO_4X;
        api_dsc_encoder_enable(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev, MS_TRUE);
        api_dsc_encoder_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev);

        //vipss vwdma config
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_sel                                    = g_u8_dma_dsc_idx[u8_vipss_sel];
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.b_enable                                  = MS_TRUE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_h_size               = gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u16_hactive;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_v_size               = gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u16_vactive;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_color_space                            = CHIP_INTERNAL_PROCESS_COLORSPACE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_line_offset                           = gst_single_vipss_dev[u8dev][u8_vipss_sel].st_encoder_dev.st_dsc_config_param.u16_hactive;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_dsc_ratio                              = DSC_RATIO_4X;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START + 3 * u8_vipss_sel * SINGLE_BUF_MEMORY_1080P_SIZE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = MEMORY_BUF_ADDR_START + 3 * u8_vipss_sel * SINGLE_BUF_MEMORY_1080P_SIZE + SINGLE_BUF_MEMORY_1080P_SIZE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = MEMORY_BUF_ADDR_START + 3 * u8_vipss_sel * SINGLE_BUF_MEMORY_1080P_SIZE + 2 * SINGLE_BUF_MEMORY_1080P_SIZE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_request_len                           = 255;

        api_vwdma_start_addr_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
        api_vwdma_line_offset_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
        api_vwdma_burst_num_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
        api_vwdma_request_len_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
        api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

    }
    else
    {
        //vipss mux config
        if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u8_color_space == HDMI_COLORSPACE_YCBCR420)
        {
            api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_5);  /**< CSC -> 4KSDN -> SDN -> VWDMA1 */
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_idx = g_u8_4ksdn_idx[u8_vipss_sel];
            api_sdn4k_init(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_input_colorSpace = HDMI_COLORSPACE_YCBCR420;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_output_colorSpace = HDMI_COLORSPACE_YCBCR444;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_mode = SDN4K_MODE_AUTO;
            api_sdn4k_config(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
            api_sdn4k_enable(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev, MS_ENABLE);
            u16_in_size_v[u8_vipss_sel] = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive / 2;
        }
        else
        {
            api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_4);  //< CSC -> SDN -> VWDMA1
        }
        #if MS1826_VIPSS1_DEINT
		if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u8_polarity % 2 == 0 && u8_vipss_sel == VIPSS1)
		{
			log("1111111111111 Input is interlace 1111111111111\r\n");
			switch(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u16_vactive*2)
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
					Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive = 1080;
                    log("1080i\r\n");
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
					Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_hactive = g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u16_hactive/2;
					Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive = 480;
                    log("480i\r\n");
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
					Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_hactive = g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u16_hactive/2;
					Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive = 576;
                    log("576i\r\n");
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
		{
			if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u8_polarity % 2 == 0)
			{
				g_st_deint_dev[u8dev].b_deint_en = false;
				api_deint_config(&g_st_deint_dev[u8dev]);
				Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_hactive = g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u16_hactive;
				Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive = g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u16_vactive / 2;

			}
			//vipss mux config
			api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_4);       //< CSC -> SDN -> VWDMA1
		}
        u16_in_size_h[u8_vipss_sel] = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_hactive;
		u16_in_size_v[u8_vipss_sel] = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].u16_vactive;
	#endif
        //vipss sdn config
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.u8_sel = g_u8_sdn_idx[u8_vipss_sel];
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.b_enable = MS_TRUE;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.u8_sdn_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

        if(g_bSplitterEnable[u8dev][u8_vipss_sel])
        {
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tInSize.u16_h;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tInSize.u16_v;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_h;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_v;
            api_sdn_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
            log("st_scale_size[%d].tInSize.u16_h==0x%04x\r\n",u8_vipss_sel,g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tInSize.u16_h);
            log("st_scale_size[%d].tInSize.u16_v==0x%04x\r\n",u8_vipss_sel,g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tInSize.u16_v);
            log("st_scale_size[%d].tMemSize.u16_h==0x%04x\r\n",u8_vipss_sel,g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_h);
            log("st_scale_size[%d].tMemSize.u16_v==0x%04x\r\n",u8_vipss_sel,g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_v);

            api_sdn_scaler_size_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
            api_sdn_csc_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);

            //vipss vwdma config
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.b_enable = false;
            api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_request_len = 255;
            api_vwdma_request_len_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.b_enable = MS_TRUE;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_h_size = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_h;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_v_size = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_v;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = g_st_single_app_dev[u8dev][u8_vipss_sel].st_dma_addr.u32_dma_start_addr_0;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = g_st_single_app_dev[u8dev][u8_vipss_sel].st_dma_addr.u32_dma_start_addr_1;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = g_st_single_app_dev[u8dev][u8_vipss_sel].st_dma_addr.u32_dma_start_addr_2;

            api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

            api_vwdma_start_addr_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_line_offset = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_h;
            api_vwdma_line_offset_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            api_vwdma_burst_num_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_vwdma_rotatation_deg = ROTATION_0_DEG;
            api_vwdma_rotation_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
        }
        else
        {
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h = u16_in_size_h[u8_vipss_sel] ? u16_in_size_h[u8_vipss_sel] : 1920;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v = u16_in_size_v[u8_vipss_sel] > 0 ? u16_in_size_v[u8_vipss_sel] : 1080;
            // I2P
        #if (MS1826_VIPSS1_DEINT==0)
            if(g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][u8_vipss_sel]].st_rx_video_timing.u8_polarity % 2 == 0)
            {
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h = u16_in_size_h[u8_vipss_sel] ? u16_in_size_h[u8_vipss_sel] : 1920;
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v = u16_in_size_v[u8_vipss_sel] > 0 ? u16_in_size_v[u8_vipss_sel] / 2 : 1080;
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h =
                    return_minof_three(u16_in_size_h[u8_vipss_sel], g_stVideo_HDMITx_Timing[u8dev][u8_vipss_sel].u16_hactive, MEMORY_1080P_H_SIZE_MAX);
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v =
                    return_minof_three(u16_in_size_v[u8_vipss_sel] / 2, g_stVideo_HDMITx_Timing[u8dev][u8_vipss_sel].u16_vactive, MEMORY_1080P_V_SIZE_MAX);
            }
            else
        #endif
            {
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h = u16_in_size_h[u8_vipss_sel] ? u16_in_size_h[u8_vipss_sel] : 1920;
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v = u16_in_size_v[u8_vipss_sel] > 0 ? u16_in_size_v[u8_vipss_sel] : 1080;
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h =
                    return_minof_three(gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h, g_stVideo_HDMITx_Timing[u8dev][u8_vipss_sel].u16_hactive, MEMORY_1080P_H_SIZE_MAX);
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v =
                    return_minof_three(gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v, g_stVideo_HDMITx_Timing[u8dev][u8_vipss_sel].u16_vactive, MEMORY_1080P_V_SIZE_MAX);
            }
            api_sdn_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
            api_sdn_scaler_size_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
            api_sdn_csc_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
            //vipss vwdma config
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.b_enable = MS_TRUE;
            api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_h_size =
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_v_size =
                gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_line_offset = gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_request_len                           = 255;

            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START + 3 * u8_vipss_sel * SINGLE_BUF_MEMORY_1080P_SIZE;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = MEMORY_BUF_ADDR_START + 3 * u8_vipss_sel * SINGLE_BUF_MEMORY_1080P_SIZE + SINGLE_BUF_MEMORY_1080P_SIZE;
            gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = MEMORY_BUF_ADDR_START + 3 * u8_vipss_sel * SINGLE_BUF_MEMORY_1080P_SIZE + 2 * SINGLE_BUF_MEMORY_1080P_SIZE;
            api_vwdma_start_addr_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            api_vwdma_line_offset_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            api_vwdma_burst_num_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
            api_vwdma_request_len_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
        }

        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_vwdma_rotatation_deg = ROTATION_0_DEG;
        api_vwdma_rotation_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
   }
}

static VOID _single_vopss_instance_init(UINT8 u8dev,SINGLE_VOPSS_T *pst_single_vopss_dev)
{
    UINT8 u8_path_sel = pst_single_vopss_dev->u8_vopss_sel;

    if(pst_single_vopss_dev->b_is_dsc)
    {
        api_vrdma_instance_init(&pst_single_vopss_dev->st_single_vrdma_dev, g_u8_dma_dsc_idx[u8_path_sel]);
        api_su_instance_init(&pst_single_vopss_dev->st_single_su_dev, g_u8_su_dsc_idx[u8_path_sel]);
        api_dsc_decoder_instance_init(&pst_single_vopss_dev->st_decoder_dev,g_u8_dsc_idx[u8_path_sel]);
    }
    else
    {
        api_vrdma_instance_init(&pst_single_vopss_dev->st_single_vrdma_dev, g_u8_dma_idx[u8_path_sel]);
        api_su_instance_init(&pst_single_vopss_dev->st_single_su_dev, g_u8_su_idx[u8_path_sel]);
    }
    u8dev = u8dev;
}

VOID _single_vopss_cfg(UINT8 u8dev,BOOL b_is_dsc,UINT8 u8_vopss_sel)
{
    MISC_MODULE_CLK_T  st_su_clk, st_mixer_clk;

    if(!b_is_dsc)
    {
        if(g_bSplitterEnable[u8dev][u8_vopss_sel])
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.b_enable = false;
            api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        }
        else
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_ss_vopss_dev.u8_sel              = g_u8_ss_ch_idx[u8_vopss_sel];
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_ss_vopss_dev.u8_vopss_src_vb_sel = u8_vopss_sel;
            api_ss_vopss_src_vb_sel(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_ss_vopss_dev);
        }
        //vopss VRDMA config
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.b_enable = MS_TRUE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_mem_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

#if 0
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size =
            gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size =
            gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset =
            gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 =
            gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 =
            gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 =
            gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2;
#else
        if(g_bSplicerEnable[u8dev][u8_vopss_sel])
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size =
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_width?
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_width:
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size =
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_hight?
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_hight:
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;

            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;

            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 +
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_xpos * 3 +
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_ypos *
                gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset * 3;

            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 +
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_xpos * 3 +
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_ypos *
                gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset * 3;

            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 +
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_xpos * 3 +
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_win_size.u16_ypos *
                gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset * 3;

        }
        else if(g_bSplitterEnable[u8dev][u8_vopss_sel])
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size = g_st_single_app_dev[u8dev][u8_vopss_sel].st_scale_size.tMemSize.u16_h;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size = g_st_single_app_dev[u8dev][u8_vopss_sel].st_scale_size.tMemSize.u16_v;

            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.u16_line_offset;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = g_st_single_app_dev[u8dev][u8_vopss_sel].st_dma_addr.u32_dma_start_addr_0;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = g_st_single_app_dev[u8dev][u8_vopss_sel].st_dma_addr.u32_dma_start_addr_1;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = g_st_single_app_dev[u8dev][u8_vopss_sel].st_dma_addr.u32_dma_start_addr_2;
        }
        else
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 =
                gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2;
        }
#endif

        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_request_len = 255;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_DISABLE;
        api_vrdma_mirror_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_start_addr_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_burst_num_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_line_offset_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_request_len_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        // su clk src
        st_su_clk.u8_dst_module = g_u8_misc_su[u8_vopss_sel];
        st_su_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1 + u8_vopss_sel;
        api_misc_su_clkin_sel(&st_su_clk);

        //vopss su config
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.u8_sel = g_u8_su_idx[u8_vopss_sel];
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.b_enable = MS_TRUE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.u8_su_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
        if(gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h == 0)
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_h = 720;
        }
        else
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_h =
                gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size;//gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        }
        if(gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v == 0)
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_v = 480;
        }
        else
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_v =
                gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size;//gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
        }
        if(g_bSplitterEnable[u8dev][u8_vopss_sel])
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_h =
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_sp - g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_st;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_v =
                g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_sp - g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_st;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_st = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_st;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_sp = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_sp;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_st = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_st;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_sp = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_sp;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u8_dis_win_en = MS_TRUE;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_st = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_st;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_sp = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_sp;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_st = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_st;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_sp = g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_sp;
            log("st_video_win[%d].u16_hde_st==0x%04x\r\n",u8_vopss_sel,g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_st);
            log("st_video_win[%d].u16_hde_sp==0x%04x\r\n",u8_vopss_sel,g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_hde_sp);
            log("st_video_win[%d].u16_vde_st==0x%04x\r\n",u8_vopss_sel,g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_st);
            log("st_video_win[%d].u16_vde_sp==0x%04x\r\n",u8_vopss_sel,g_st_single_app_dev[u8dev][u8_vopss_sel].st_video_win.u16_vde_sp);
        }
        else
        {
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_h = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_v = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u8_dis_win_en = MS_TRUE;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset;
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
        }
        api_su_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_scaler_size_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_csc_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_video_win_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_display_win_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);

        if(g_bSplitterEnable[u8dev][u8_vopss_sel] == FALSE)
        {
            //mixer clk sel
            st_mixer_clk.u8_dst_module = g_u8_misc_mixer[u8_vopss_sel];
            st_mixer_clk.u8_src_clk = g_u8_misc_clksrc[u8_vopss_sel];
            api_misc_mixer_clkin_sel(&st_mixer_clk);
        }
    }
    else
    {
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_ss_vopss_dev.u8_sel               = g_u8_ss_ch_dsc_idx[u8_vopss_sel];
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_ss_vopss_dev.u8_vopss_src_vb_sel = u8_vopss_sel;
        api_ss_vopss_src_vb_sel(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_ss_vopss_dev);

        // vopss dsc config
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev.u8_sel = g_u8_dsc_idx[u8_vopss_sel];
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev.st_dsc_config_param.u16_hactive        = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_h_size;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev.st_dsc_config_param.u16_vactive        = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_v_size;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev.st_dsc_config_param.u8_color_space     = CHIP_INTERNAL_PROCESS_COLORSPACE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev.st_dsc_config_param.u8_dsc_ratio       = DSC_RATIO_4X;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev.st_dsc_config_param.u8_dsc_data_path_clk   = PXL_CLK_DIV2;
//      api_dsc_decoder_enable(&gst_single_vopss_dev[u8_vopss_sel].st_decoder_dev, MS_TRUE);
//      api_dsc_decoder_set(&gst_single_vopss_dev[u8_vopss_sel].st_decoder_dev);

        if(!b_init[u8dev][u8_vopss_sel])
        {
            b_init[u8dev][u8_vopss_sel] = 1;
            api_dsc_decoder_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev);

            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_com_rdma_dev.u8_sel = g_u8_dma_dsc_idx[u8_vopss_sel];
            gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_com_rdma_dev.st_dsc_csc_cfg.u8_csc_mode = CHIP_INTERNAL_PROCESS_COLORSPACE;
            api_dma_common_dsc_csc_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_com_rdma_dev);

        }
#if 0
        else
        {
            api_dsc_decoder_pps_seamless_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev) ;
            //log("XXXXxxxxxxxxxxxx. r\n");
        }
#endif
        api_dsc_decoder_enable(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_decoder_dev, MS_TRUE);

        //vopss VRDMA config
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_sel = g_u8_dma_dsc_idx[u8_vopss_sel];
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.b_enable = MS_TRUE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_mem_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size          = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size          = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_dsc_ratio                              = DSC_RATIO_4X;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset                           = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_request_len = 255;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_DISABLE;
        api_vrdma_mirror_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_start_addr_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_burst_num_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_line_offset_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_request_len_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
        api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);

        //vopss su config
        st_su_clk.u8_dst_module = g_u8_misc_dsc_su[u8_vopss_sel];
        st_su_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1 + u8_vopss_sel;
        api_misc_su_clkin_sel(&st_su_clk);

        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.u8_sel = g_u8_su_dsc_idx[u8_vopss_sel];
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.b_enable = MS_TRUE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.u8_su_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_h   = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_v   = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_h   = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_v   = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_st          = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_sp          = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_st          = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_sp          = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u8_dis_win_en         = MS_TRUE;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_st            = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_sp            = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_st            = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset;
        gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_sp            = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
        api_su_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_scaler_size_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_csc_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_video_win_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
        api_su_display_win_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);

        //mixer clk sel
        st_mixer_clk.u8_dst_module = g_u8_misc_mixer[u8_vopss_sel];
        st_mixer_clk.u8_src_clk = g_u8_misc_clksrc[u8_vopss_sel];
        api_misc_mixer_clkin_sel(&st_mixer_clk);
    }

    //vopss tx csc config
    g_st_txcsc_dev[u8dev][u8_vopss_sel].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8_vopss_sel]];
    g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_in_colorspace   = CHIP_INTERNAL_PROCESS_COLORSPACE;
    g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_in_color_range  = CHIP_INTERNAL_PROCESS_COLORRANGE;
    g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_out_colorspace  = CHIP_TX_CSC_OUT_COLORSPACE;
    g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
    g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.u8_pixel_repeat     = HDMI_PIXEL_REPEAT_X1;
    api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8_vopss_sel]);
}

static VOID _app_video_single_mixer_cfg(UINT8 u8dev,UINT8 u8_vmixer_idx, UINT8 u8_layer)
{
    MISC_MODULE_CLK_T g_st_module_clk;
    g_st_module_clk.u8_dst_module = g_u8_misc_mixer[u8_vmixer_idx];
    g_st_module_clk.u8_clkdiv = MISC_CLKDIV_1;
    g_st_module_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1 + u8_vmixer_idx;
    api_misc_mixer_clkin_sel(&g_st_module_clk);

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].u8_video_mixer_idx = u8_vmixer_idx;
    api_video_mixer_instance_init(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx], u8_vmixer_idx);

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel = &g_st_mixer_layer_sel[u8dev];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_alf = &g_st_mixer_alf_cfg[u8dev];
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_bd_attr  = NULL;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_bd_color = NULL;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_bg_color = &g_st_bg_color;

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].u8_video_mixer_idx = u8_vmixer_idx;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].b_videomixer_en = MS_TRUE;
    api_video_mixer_bg_color_config(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx]);
    api_video_mixer_enable(&g_st_video_mixer_dev[u8dev][u8_vmixer_idx]);

    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer0_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer1_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer2_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer3_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer4_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer5_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer6_src_sel = VOPSS1_DSC_SU;
    g_st_video_mixer_dev[u8dev][u8_vmixer_idx].pst_layer_sel->u8_layer7_src_sel = u8_layer;

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

VOID _single_mode_frc_cfg(UINT8 u8dev)
{
    //VIPSS instance
    gst_single_vipss_dev[u8dev][VIPSS1].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vipss_dev[u8dev][VIPSS1].u8_vipss_sel = VIPSS1;
    _single_vipss_instance_init(u8dev,&gst_single_vipss_dev[u8dev][VIPSS1]);

    gst_single_vipss_dev[u8dev][VIPSS2].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vipss_dev[u8dev][VIPSS2].u8_vipss_sel = VIPSS2;
    _single_vipss_instance_init(u8dev,&gst_single_vipss_dev[u8dev][VIPSS2]);

    gst_single_vipss_dev[u8dev][VIPSS3].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vipss_dev[u8dev][VIPSS3].u8_vipss_sel = VIPSS3;
    _single_vipss_instance_init(u8dev,&gst_single_vipss_dev[u8dev][VIPSS3]);

    gst_single_vipss_dev[u8dev][VIPSS4].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vipss_dev[u8dev][VIPSS4].u8_vipss_sel = VIPSS4;
    _single_vipss_instance_init(u8dev,&gst_single_vipss_dev[u8dev][VIPSS4]);

    //VOPSS instance
    gst_single_vopss_dev[u8dev][VOPSS1].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vopss_dev[u8dev][VOPSS1].u8_vopss_sel = VOPSS1;
    _single_vopss_instance_init(u8dev,&gst_single_vopss_dev[u8dev][VOPSS1]);

    gst_single_vopss_dev[u8dev][VOPSS2].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vopss_dev[u8dev][VOPSS2].u8_vopss_sel = VOPSS2;
    _single_vopss_instance_init(u8dev,&gst_single_vopss_dev[u8dev][VOPSS2]);

    gst_single_vopss_dev[u8dev][VOPSS3].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vopss_dev[u8dev][VOPSS3].u8_vopss_sel = VOPSS3;
    _single_vopss_instance_init(u8dev,&gst_single_vopss_dev[u8dev][VOPSS3]);

    gst_single_vopss_dev[u8dev][VOPSS4].b_is_dsc = g_dsc_enable[u8dev];
    gst_single_vopss_dev[u8dev][VOPSS4].u8_vopss_sel = VOPSS4;
    _single_vopss_instance_init(u8dev,&gst_single_vopss_dev[u8dev][VOPSS4]);

    //DMA config
    gst_single_vipss_dev[u8dev][VIPSS1].st_single_ss_vipss_dev.u8_sel = (gst_single_vipss_dev[u8dev][VIPSS1].b_is_dsc)?g_u8_ss_ch_dsc_idx[VIPSS1]:g_u8_ss_ch_idx[VIPSS1];
    gst_single_vipss_dev[u8dev][VIPSS1].st_single_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS1;
    api_ss_vipss_wdma_buf_sel(&gst_single_vipss_dev[u8dev][VIPSS1].st_single_ss_vipss_dev);

    gst_single_vipss_dev[u8dev][VIPSS2].st_single_ss_vipss_dev.u8_sel = (gst_single_vipss_dev[u8dev][VIPSS2].b_is_dsc)?g_u8_ss_ch_dsc_idx[VIPSS2]:g_u8_ss_ch_idx[VIPSS2];
    gst_single_vipss_dev[u8dev][VIPSS2].st_single_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS2;
    api_ss_vipss_wdma_buf_sel(&gst_single_vipss_dev[u8dev][VIPSS2].st_single_ss_vipss_dev);

    gst_single_vipss_dev[u8dev][VIPSS3].st_single_ss_vipss_dev.u8_sel = (gst_single_vipss_dev[u8dev][VIPSS3].b_is_dsc)?g_u8_ss_ch_dsc_idx[VIPSS3]:g_u8_ss_ch_idx[VIPSS3];
    gst_single_vipss_dev[u8dev][VIPSS3].st_single_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS3;
    api_ss_vipss_wdma_buf_sel(&gst_single_vipss_dev[u8dev][VIPSS3].st_single_ss_vipss_dev);

    gst_single_vipss_dev[u8dev][VIPSS4].st_single_ss_vipss_dev.u8_sel = (gst_single_vipss_dev[u8dev][VIPSS4].b_is_dsc)?g_u8_ss_ch_dsc_idx[VIPSS4]:g_u8_ss_ch_idx[VIPSS4];
    gst_single_vipss_dev[u8dev][VIPSS4].st_single_ss_vipss_dev.u8_vipss_wdma_id_sel = VIPSS4;
    api_ss_vipss_wdma_buf_sel(&gst_single_vipss_dev[u8dev][VIPSS4].st_single_ss_vipss_dev);

    gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.u8_sel                         = (gst_single_vopss_dev[u8dev][VOPSS1].b_is_dsc)?g_u8_dma_dsc_idx[VOPSS1]:g_u8_dma_idx[VOPSS1];
    gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.u8_dma_buf_num_sel             = SINGLE_BUF_NUM;
    gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext       = false;

#if ENABLE_MS1826_FRC_MODE_SWITCH
    //add by wf8421 20230726
    if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
    {
        if(g_stVideo_HDMITx_Timing[u8dev][VOPSS1].u16_vfreq>=6000)
            gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = L2H_OR_EQUAL;
        else
            gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = H2L;
        gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = MANU_MODE;
    }
    else
        gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = AUTO_MODE;
    api_dma_common_frame_rate_cfg(&gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev);
#endif

    gst_single_vopss_dev[u8dev][VOPSS1].st_single_ss_vopss_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS1].b_is_dsc)?g_u8_ss_ch_dsc_idx[VOPSS1]:g_u8_ss_ch_idx[VOPSS1];
    gst_single_vopss_dev[u8dev][VOPSS1].st_single_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS1;
    api_dma_common_buf_num_set(&gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_single_vopss_dev[u8dev][VOPSS1].st_single_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_single_vopss_dev[u8dev][VOPSS1].st_single_ss_vopss_dev);
    gst_single_vopss_dev[u8dev][VOPSS1].st_single_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS1;
    api_ss_vopss_dst_buf_id_sel(&gst_single_vopss_dev[u8dev][VOPSS1].st_single_ss_vopss_dev);

    gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS2].b_is_dsc)?g_u8_dma_dsc_idx[VOPSS2]:g_u8_dma_idx[VOPSS2];
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.u8_dma_buf_num_sel = SINGLE_BUF_NUM;
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
#if ENABLE_MS1826_FRC_MODE_SWITCH
    //add by wf8421 20230726
    if(display_mode[u8dev] == MATRIX_SWITCHER_OUT /*&& !g_bSplicerEnable[u8dev][VOPSS2]*/)
    {
        if(g_stVideo_HDMITx_Timing[u8dev][VOPSS2].u16_vfreq>=6000)
            gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = L2H_OR_EQUAL;
        else
            gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = H2L;
        gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = MANU_MODE;
    }
    else
        gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = AUTO_MODE;
    api_dma_common_frame_rate_cfg(&gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev);
#endif
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_ss_vopss_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS2].b_is_dsc)?g_u8_ss_ch_dsc_idx[VOPSS2]:g_u8_ss_ch_idx[VOPSS2];
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS2;
    api_dma_common_buf_num_set(&gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_single_vopss_dev[u8dev][VOPSS2].st_single_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_single_vopss_dev[u8dev][VOPSS2].st_single_ss_vopss_dev);
    gst_single_vopss_dev[u8dev][VOPSS2].st_single_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS2;
    api_ss_vopss_dst_buf_id_sel(&gst_single_vopss_dev[u8dev][VOPSS2].st_single_ss_vopss_dev);

    gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS3].b_is_dsc)?g_u8_dma_dsc_idx[VOPSS3]:g_u8_dma_idx[VOPSS3];
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.u8_dma_buf_num_sel = SINGLE_BUF_NUM;
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
#if ENABLE_MS1826_FRC_MODE_SWITCH
    //add by wf8421 20230726
    if(display_mode[u8dev] == MATRIX_SWITCHER_OUT /*&& !g_bSplicerEnable[u8dev][VOPSS3]*/)
    {
        if(g_stVideo_HDMITx_Timing[u8dev][VOPSS3].u16_vfreq>=6000)
            gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = L2H_OR_EQUAL;
        else
            gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = H2L;
        gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = MANU_MODE;
    }
    else
        gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = AUTO_MODE;
    api_dma_common_frame_rate_cfg(&gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev);
#endif
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_ss_vopss_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS3].b_is_dsc)?g_u8_ss_ch_dsc_idx[VOPSS3]:g_u8_ss_ch_idx[VOPSS3];
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS3;
    api_dma_common_buf_num_set(&gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_single_vopss_dev[u8dev][VOPSS3].st_single_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_single_vopss_dev[u8dev][VOPSS3].st_single_ss_vopss_dev);
    gst_single_vopss_dev[u8dev][VOPSS3].st_single_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS3;
    api_ss_vopss_dst_buf_id_sel(&gst_single_vopss_dev[u8dev][VOPSS3].st_single_ss_vopss_dev);

    gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS4].b_is_dsc)?g_u8_dma_dsc_idx[VOPSS4]:g_u8_dma_idx[VOPSS4];
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.u8_dma_buf_num_sel = SINGLE_BUF_NUM;
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
#if ENABLE_MS1826_FRC_MODE_SWITCH
    //add by wf8421 20230726
    if(display_mode[u8dev] == MATRIX_SWITCHER_OUT)
    {
        if(g_stVideo_HDMITx_Timing[u8dev][VOPSS4].u16_vfreq>=6000)
            gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = L2H_OR_EQUAL;
        else
            gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate = H2L;
        gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = MANU_MODE;
    }
    else
        gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev.st_frame_rate_cfg.u8_frame_rate_mode = AUTO_MODE;
    api_dma_common_frame_rate_cfg(&gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev);
#endif
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_ss_vopss_dev.u8_sel = (gst_single_vopss_dev[u8dev][VOPSS4].b_is_dsc)?g_u8_ss_ch_dsc_idx[VOPSS4]:g_u8_ss_ch_idx[VOPSS4];
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_ss_vopss_dev.u8_vopss_src_vb_sel = VIPSS4;
    api_dma_common_buf_num_set(&gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev);
    api_dma_comom_buf_id_mode_cfg(&gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev);
    api_dma_comom_buf_frc_ctl_ext(&gst_single_vopss_dev[u8dev][VOPSS4].st_single_com_rdma_dev);
    api_ss_vopss_src_vb_sel(&gst_single_vopss_dev[u8dev][VOPSS4].st_single_ss_vopss_dev);
    gst_single_vopss_dev[u8dev][VOPSS4].st_single_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS4;
    api_ss_vopss_dst_buf_id_sel(&gst_single_vopss_dev[u8dev][VOPSS4].st_single_ss_vopss_dev);
}

VOID single_video_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute)
{
    BOOL b_int;

//    log("single_video_mute TX%d mute sta:%s\r\n",u8_path,b_mute?"TRUE":"FALSE");
    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.u8_sel = g_dsc_enable[u8dev] ? g_u8_su_dsc_idx[u8_path]: g_u8_su_idx[u8_path];
    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.bd_enable = b_mute;

    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.border_color.u8VR = g_st_bg_color.u8_vr;
    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.border_color.u8YG = g_st_bg_color.u8_yg;
    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.border_color.u8UB = g_st_bg_color.u8_ub;

    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.bd_cover = MS_TRUE;
    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.border_size.u16_border_h = g_stVideo_HDMITx_Timing[u8dev][u8_path].u16_hactive;
    gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev.st_video_border.border_size.u16_border_v = g_stVideo_HDMITx_Timing[u8dev][u8_path].u16_vactive;
    api_su_video_border_set(&gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev);
    g_u16_timer_out = 0;
    api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB + u8_path);
    do
    {
        g_u16_timer_out ++;
        b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB + u8_path);
    }
    while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
    api_su_trigger(&gst_single_vopss_dev[u8dev][u8_path].st_single_su_dev);
}

VOID _single_video_freeze(UINT8 u8dev,UINT8 u8_path_sel)
{
    BOOL b_vb_h;
    DMA_COMMON_DEV_T st_dma_dev;
    //video freeze
    __disable_irq();
    if(g_dsc_enable[u8dev])
        gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[u8_path_sel];
    else
        gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
    gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
    gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;

    if(g_dsc_enable[u8dev])
        st_dma_dev.u8_sel = g_u8_dma_dsc_idx[u8_path_sel];
    else
        st_dma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
    api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev);
    g_u16_timer_out = 0;
    do
    {
        g_u16_timer_out ++;
        if(g_dsc_enable[u8dev])
            b_vb_h = api_misc_vb_status_get(g_u8_vb_dsc_path[u8_path_sel]);
        else
            b_vb_h = api_misc_vb_status_get(g_u8_vb_path[u8_path_sel]);
    }
    while(b_vb_h == false && (g_u16_timer_out < 500));

    b_freeze_status[u8dev][u8_path_sel] = TRUE;
    api_dma_common_buf_id_get(&st_dma_dev);
    wdma_buf_id[u8dev] = st_dma_dev.st_dma_status.wdma_buf_id;
    rdma_buf_id[u8dev] = st_dma_dev.st_dma_status.rdma_buf_id;
    wdma_buf_id[u8dev] = (rdma_buf_id[u8dev] + 1) % 3;
    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = wdma_buf_id[u8dev];
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = rdma_buf_id[u8dev];
    api_dma_comom_buf_id_set(&st_dma_dev);
    api_dma_common_trigger(&st_dma_dev);
    __enable_irq();
}

VOID _single_video_unfreeze(UINT8 u8dev,UINT8 u8_path_sel)
{
    BOOL b_int;
    DMA_COMMON_DEV_T st_dma_dev;
    SS_DEV_T st_vopss_ss_dev;

    if(g_dsc_enable[u8dev])
        st_vopss_ss_dev.u8_sel = g_u8_ss_ch_dsc_idx[u8_path_sel];
    else
        st_vopss_ss_dev.u8_sel = g_u8_ss_ch_idx[u8_path_sel];
    st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode = NEXT_VB_RISING_EDGE_TRIGGER;
    st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
    st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_tmg = VDS_TMG1 + u8_path_sel;

    api_ss_vopss_trig_mode_cfg(&st_vopss_ss_dev);

    __disable_irq();

    if(g_dsc_enable[u8dev])
        st_dma_dev.u8_sel = g_u8_dma_dsc_idx[u8_path_sel];
    else
        st_dma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
    api_dma_common_fifo_status_get(&st_dma_dev);


    if(g_dsc_enable[u8dev])
        st_dma_dev.u8_sel = g_u8_dma_dsc_idx[u8_path_sel];
    else
        st_dma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];

    if(st_dma_dev.st_dma_status.b_h2l_status == MS_TRUE)
    {
        api_dma_common_buf_id_get(&st_dma_dev);
        wdma_buf_id[u8dev] = st_dma_dev.st_dma_status.wdma_buf_id;

    }

    st_dma_dev.st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 0) % 3;
    st_dma_dev.st_frc_cfg.u8_dst_buf_id_sel = (wdma_buf_id[u8dev] + 2) % 3;
    api_dma_comom_buf_id_set(&st_dma_dev);

    g_u16_timer_out = 0;
    api_misc_int_status_clr(g_u8_vb_vdstmg[u8_path_sel]);
    do
    {
        g_u16_timer_out ++;
        b_int = api_misc_int_status_get(g_u8_vb_vdstmg[u8_path_sel]);
    }
    while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
    api_dma_common_trigger(&st_dma_dev);
    api_dma_common_simultaneous_trigger();

    if(g_dsc_enable[u8dev])
    {
        api_dsc_decoder_pps_seamless_set(&gst_single_vopss_dev[u8dev][u8_path_sel].st_decoder_dev);
        gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[u8_path_sel];
    }
    else
    {
        gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[u8_path_sel];
    }
    gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
    api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8dev][u8_path_sel].st_single_com_wdma_dev);

    g_u16_timer_out = 0;
    api_misc_int_status_clr(g_u8_vb_vdstmg[u8_path_sel]);
    do
    {
        g_u16_timer_out ++;
        b_int = api_misc_int_status_get(g_u8_vb_vdstmg[u8_path_sel]);
    }
    while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
    api_dma_common_trigger(&st_dma_dev);
    b_freeze_status[u8dev][u8_path_sel] = FALSE;

    api_ss_vopss_trig(&st_vopss_ss_dev);
    __enable_irq();
}

VOID app_single_mode_config(UINT8 u8dev)
{
    UINT8 u8_i = 0;
    b_init[u8dev][0] = 0;
    b_init[u8dev][1] = 0;
    b_init[u8dev][2] = 0;
    b_init[u8dev][3] = 0;
    Bypass_Tx_Status[u8dev][0] = 0;
    Bypass_Tx_Status[u8dev][1] = 0;
    Bypass_Tx_Status[u8dev][2] = 0;
    Bypass_Tx_Status[u8dev][3] = 0;

    _single_mode_frc_cfg(u8dev);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS1);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS2);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS3);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS4);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[0]:g_u8_ss_ch_idx[0]);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[1]:g_u8_ss_ch_idx[1]);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[2]:g_u8_ss_ch_idx[2]);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[3]:g_u8_ss_ch_idx[3]);
    mculib_delay_ms(50);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS1);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS2);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS3);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS4);
    for(u8_i = 0; u8_i < 4; u8_i ++)
    {
        u8 l_u8VIC = g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code;
        //tx audio config
        app_tx_audio_cfg(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);

         //tx video config
        g_st_tx_mux[u8dev][u8_i].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_i]];
        g_st_tx_mux[u8dev][u8_i].u8_video_mux = g_u8_misc_mixer[u8_i];
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8_i]);

        g_st_hdmi_tx[u8dev][u8_i].u8_txIndex = g_u8_HDMI_TX_PROT[u8_i];
        g_st_hdmi_tx[u8dev][u8_i].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8_i];
        g_st_hdmi_tx[u8dev][u8_i].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][u8_i].u8_color_depth = HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][u8_i].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
        g_st_hdmi_tx[u8dev][u8_i].u8_audio_src = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][u8_i].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][u8_i].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8_i].u16_pixclk;
        memcpy((&g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i],false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_i]);
        _video_port_hdmi_tx_driver_config(u8dev,u8_i);
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i],MS_TRUE);
        _app_video_single_mixer_cfg(u8dev,g_u8_mixer_idx[u8_i], g_dsc_enable[u8dev]?g_u8_mixer_layer_dsc_src[u8_i]:g_u8_mixer_layer_src[u8_i]);
    }

    if(g_dsc_enable[u8dev])
	    app_video_vopss_trig(u8dev,g_u8_ss_ch_dsc_idx[0] | g_u8_ss_ch_dsc_idx[1] | g_u8_ss_ch_dsc_idx[2] | g_u8_ss_ch_dsc_idx[3]);
    else
        app_video_vopss_trig(u8dev,g_u8_ss_ch_idx[0] | g_u8_ss_ch_idx[1] | g_u8_ss_ch_idx[2] | g_u8_ss_ch_idx[3]);


    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}

#if 1
VOID app_single_mode_rx_switch(UINT8 u8dev,UINT8 u8_rx, UINT8 u8_tx)
{
    _single_video_freeze(u8dev,u8_tx);
    Mapp_Rx[u8dev][u8_tx] = u8_rx;
    _single_cfg(u8dev,g_dsc_enable[u8dev],u8_tx);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[u8_tx]:g_u8_ss_ch_idx[u8_tx]);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],u8_tx);
//  mculib_delay_ms(200);
//  mculib_delay_ms(200);

    _single_video_unfreeze(u8dev,u8_tx);
//    if(g_st_hdmi_rx[u8dev][u8_rx].u8_rx_state != RX_STATE_RUNNING)
//        VIDEOMUTE(u8dev,u8_rx, TRUE);
//    else
//        VIDEOMUTE(u8dev,u8_rx, FALSE);

    //tx audio config
    app_tx_audio_cfg(u8dev, u8_rx, u8_tx);
    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F & 1<<u8_tx);
}

#else
VOID app_single_mode_rx_switch(UINT8 u8dev,UINT8 u8_rx, UINT8 u8_tx)
{
	BOOL b_int;
    //tx audio config
	__disable_irq();
	app_tx_audio_cfg(u8dev,Matrix_Tx_Mapping[u8dev][u8_tx], u8_tx);

	api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
	g_u16_timer_out = 0;
	do
	{
		g_u16_timer_out ++;
		b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB);
	}
	while(b_int != MS_TRUE && (g_u16_timer_out < 500));

	g_st_video_mixer_dev[u8dev][g_u8_mixer_idx[u8_tx]].pst_layer_sel->u8_layer7_src_sel = g_dsc_enable[u8dev] ? g_u8_mixer_layer_dsc_src[u8_rx]:g_u8_mixer_layer_src[u8_rx];
	api_video_mixer_layer_only_config(&g_st_video_mixer_dev[u8dev][g_u8_mixer_idx[u8_tx]]);
	__enable_irq();
	OSDShown[u8dev] = MS_TRUE;
	sys_show_osd(u8dev,OSDShown[u8dev], 0x0F & 1 << u8_tx);
	log("[INFO] SEAMLESS_MATRIX_SWITCHER switch.\r\n");
	log("[INFO] HDMI TX: \r\n", u8_tx + 1);
	log("[INFO] HDMI RX: \r\n", Matrix_Tx_Mapping[u8dev][u8_tx] + 1);
}
#endif

VOID app_single_output_resolution(UINT8 u8dev)
{
    UINT8 i;
    PLL_DEV_T         pll_dev;
    MISC_MODULE_CLK_T st_module_clk;
    VDS_TMG_DEV_T     st_timgen_dev;
    for(i = 0 ; i < 4 ; i ++)
    {
        if(g_u8_output_index_bak[u8dev][i] == g_u8_output_index[u8dev][i])
            continue;

        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][i], false);

        api_misc_timing_get(g_arrOutputTable[g_u8_output_index[u8dev][i]], &g_stVideo_HDMITx_Timing[u8dev][i]);
        st_timgen_dev.pst_timing = &g_stVideo_HDMITx_Timing[u8dev][i];
        st_timgen_dev.u8_timgen_id = i;
        st_timgen_dev.b_timgen_en = MS_TRUE;
    #if ENABLE_MS1826_FRAME_LOCK
        if(ms1826_hal_hdmi_rx_get_tmds_clk(g_u8FrameLockRxTable[u8dev][i]))
            st_timgen_dev.u8_workmode  = Casecade_Mode;
        else
    #endif
            st_timgen_dev.u8_workmode  = FreeRun_Mode;

        pll_dev.b_osc_input = MS_TRUE;
        pll_dev.u8_pll_idx = PLLV1 + i;
        pll_dev.u32_clk_out = st_timgen_dev.pst_timing->u16_pixclk * 10000;

        st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_TIMGEN1 + i;
        st_module_clk.u8_src_clk = MISC_CLKSRC_PLLV1 + i;
        st_module_clk.u8_clkdiv = MISC_CLKDIV_1;

        SINGLE_PROCESS(u8dev,i);
        api_pll_config(&pll_dev);
        api_misc_timgen_clkin_sel(&st_module_clk);
        api_vds_tmg_config(&st_timgen_dev);

        g_st_hdmi_tx[u8dev][i].u8_txIndex = g_u8_HDMI_TX_PROT[i];
        g_st_hdmi_tx[u8dev][i].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][i];
        g_st_hdmi_tx[u8dev][i].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][i].u8_color_depth = HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][i].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
//      g_st_hdmi_tx[u8dev][i].u8_audio_src = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][i].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][i].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][i].u16_pixclk;
        memcpy((&g_st_hdmi_tx[u8dev][i].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][i].st_infoframes_packets.st_avi_infoframe.u8_video_code = g_arrOutputTable[g_u8_output_index[u8dev][i]];
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][i], false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][i]);
        _video_port_hdmi_tx_driver_config(u8dev,i);
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][i], MS_TRUE);
    }
    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}

VOID SINGLE_PROCESS(UINT8 u8dev,UINT8 u8_idx)
{

        UINT8 b_int = 0;
        SS_DEV_T st_split_vopss_ss_dev;
        st_split_vopss_ss_dev.u8_sel = g_dsc_enable[u8dev] ? g_u8_ss_ch_dsc_idx[u8_idx]:g_u8_ss_ch_idx[u8_idx];
        st_split_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
        st_split_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode = FORCE_TRIGGER;

        _single_cfg(u8dev,g_dsc_enable[u8dev],u8_idx);
        app_video_vipss_trig(u8dev,g_dsc_enable[u8dev] ? g_u8_ss_ch_dsc_idx[u8_idx]:g_u8_ss_ch_idx[u8_idx]);
        _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],u8_idx);
        g_u16_timer_out = 0;
        api_misc_int_status_clr(g_u8_vb_vdstmg[u8_idx]);
        do
        {
            g_u16_timer_out ++;
            b_int = api_misc_int_status_get(g_u8_vb_vdstmg[u8_idx]);
        }
        while((b_int != MS_TRUE) && (g_u16_timer_out < 500));

        if(g_dsc_enable[u8dev])
        {
            api_dsc_decoder_pps_seamless_set(&gst_single_vopss_dev[u8dev][u8_idx].st_decoder_dev);
        }

        g_u16_timer_out = 0;
        api_misc_int_status_clr(g_u8_vb_vdstmg[u8_idx]);
        do
        {
            g_u16_timer_out ++;
            b_int = api_misc_int_status_get(g_u8_vb_vdstmg[u8_idx]);
        }
        while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
        api_ss_vopss_trig_mode_cfg(&st_split_vopss_ss_dev);
        api_ss_vopss_trig(&st_split_vopss_ss_dev);
    
}

VOID app_single_osd_init(UINT8 u8dev)
{
    UINT8 u8_idx = 0;
    for(u8_idx = 0; u8_idx < 4; u8_idx ++)
    {
        g_st_misc_osd_mux[u8dev][u8_idx].b_enable = MS_TRUE;
        g_st_misc_osd_mux[u8dev][u8_idx].u8_osd_module = g_u8_misc_osd_idx[u8_idx];
        g_st_misc_osd_mux[u8dev][u8_idx].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_idx]];
        api_misc_osd_timing_mux_cfg(&g_st_misc_osd_mux[u8dev][u8_idx]);
    }
}

VOID app_single_osd_config(UINT8 u8dev,UINT8 u8_index)
{
    UINT8 u8_i = 0;
    st_str[u8dev].u8_x = 0;
    st_str[u8dev].u8_y = 0;
    st_font[u8dev].u8_x = 0;
    st_font[u8dev].u8_y = 0;
    st_osd_win_size[u8dev].u8_osd_win = OSD_WIN_DISPLAY;
    st_osd_win_size[u8dev].u16_w = 15;
    st_osd_win_size[u8dev].u16_h = 4;
    st_osd_win_size[u8dev].u16_x = 100;
    st_osd_win_size[u8dev].u16_y = 100;
    app_single_osd_init(u8dev);
    for(u8_i = 0; u8_i < 4; u8_i ++)
    {
        //if(u8_index & 0x01)
        if((u8_index>>Mapp_Rx[u8dev][u8_i])&0x01)
        {
            api_osd_window_position_set(&g_st_osd_dev[u8dev][u8_i], &st_osd_win_size[u8dev]);
            st_font[u8dev].u8_y = 0;
            st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[Mapp_Rx[u8dev][u8_i]];
            st_font[u8dev].u16_length = 5;
            api_osd_show_index(&g_st_osd_dev[u8dev][u8_i], &st_font[u8dev]);

            st_font[u8dev].u8_y = 1;
            st_font[u8dev].pu16_idx = (UINT16 *)str_osd_output_channel[u8_i];
            st_font[u8dev].u16_length = 5;
            api_osd_show_index(&g_st_osd_dev[u8dev][u8_i], &st_font[u8dev]);

            st_str[u8dev].u8_y = 2;
            set_osd_res(u8dev,Mapp_Rx[u8dev][u8_i]);
            st_str[u8dev].u16_len = 15;
            api_osd_show_length_string(&g_st_osd_dev[u8dev][u8_i], &st_str[u8dev]);
            app_osd_trigger(u8dev,u8_i);
        }
        //u8_index >>= 1;
    }
}

VOID ms1826_hal_osd_coltab_load(UINT8 u8_osd_idx,UINT8 u8_length,UINT8 *p_u8_value);

VOID app_user_osd_config(UINT8 u8dev,UINT8 u8_index)
{
    UINT8 u8_i = 0;
    st_str[u8dev].u8_x = 0;
    st_str[u8dev].u8_y = 0;
    st_font[u8dev].u8_x = 0;
    st_font[u8dev].u8_y = 0;
//    st_osd_win_size[u8dev].u8_osd_win = OSD_WIN_DISPLAY;
//    st_osd_win_size[u8dev].u16_w = 15;
//    st_osd_win_size[u8dev].u16_h = 4;
//    st_osd_win_size[u8dev].u16_x = 100;
//    st_osd_win_size[u8dev].u16_y = 100;
    app_single_osd_init(u8dev);
    for(u8_i = 0; u8_i < 4; u8_i ++)
    {
        //if(u8_index & 0x01)
        if((u8_index>>Mapp_Rx[u8dev][u8_i])&0x01)
        {
            if(g_user_osd_str_enable[u8dev][Mapp_Rx[u8dev][u8_i]])
            {
                st_user_osd_win_size[u8dev][u8_i].u8_osd_win = OSD_WIN_DISPLAY;
                st_user_osd_win_size[u8dev][u8_i].u16_w = MS1826_OSD_MAX_LEN;
                st_user_osd_win_size[u8dev][u8_i].u16_h = 4;
                st_user_osd_win_size[u8dev][u8_i].u16_x = g_u16UserStrDispPos[u8dev][Mapp_Rx[u8dev][u8_i]][0];
                st_user_osd_win_size[u8dev][u8_i].u16_y = g_u16UserStrDispPos[u8dev][Mapp_Rx[u8dev][u8_i]][1];
                api_osd_window_position_set(&g_st_osd_dev[u8dev][u8_i], &st_user_osd_win_size[u8dev][u8_i]);
                st_str[u8dev].u8_y = 0;
                app_ms1826_get_osd_string(u8dev,Mapp_Rx[u8dev][u8_i]);
                st_str[u8dev].pstr = p_str;
                st_str[u8dev].u16_len = MS1826_OSD_MAX_LEN;
                api_osd_show_length_string(&g_st_osd_dev[u8dev][u8_i], &st_str[u8dev]);
                app_osd_trigger(u8dev,u8_i);
            }
            OSD_COLOR_TABLE[u8_i*16*3+3] = g_user_osd_string_color[u8dev][Mapp_Rx[u8dev][u8_i]][0];
            OSD_COLOR_TABLE[u8_i*16*3+4] = g_user_osd_string_color[u8dev][Mapp_Rx[u8dev][u8_i]][1];
            OSD_COLOR_TABLE[u8_i*16*3+5] = g_user_osd_string_color[u8dev][Mapp_Rx[u8dev][u8_i]][2];
            log("OSD_COLOR_TABLE[%d] == %x\r\n",u8_i*16*3+3,OSD_COLOR_TABLE[u8_i*16*3+3]);
            log("OSD_COLOR_TABLE[%d] == %x\r\n",u8_i*16*3+4,OSD_COLOR_TABLE[u8_i*16*3+4]);
            log("OSD_COLOR_TABLE[%d] == %x\r\n",u8_i*16*3+5,OSD_COLOR_TABLE[u8_i*16*3+5]);
            ms1826_hal_osd_coltab_load(u8_i,g_u8_color_table_size,OSD_COLOR_TABLE);
            api_osd_font_colortab_set(&g_st_osd_dev[u8dev][u8_i],OSD_PATTERN_COLOR0+u8_i);
            app_osd_trigger(u8dev,u8_i);
        }
    }
}

VOID _video_freeze(UINT8 u8dev,UINT8 u8_path)
{
    BOOL b_vb_h;
    DMA_COMMON_DEV_T st_dma_dev[4];
    UINT8 i;

    //video freeze
    __disable_irq();

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            if(g_dsc_enable[u8dev])
                gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[i];
            else
                gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[i];
            gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
            gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;

            if(g_dsc_enable[u8dev])
                st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
            else
                st_dma_dev[i].u8_sel = g_u8_dma_idx[i];
            api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev);
        }
    }

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            g_u16_timer_out = 0;
            do
            {
                g_u16_timer_out ++;
                if(g_dsc_enable[u8dev])
                    b_vb_h = api_misc_vb_status_get(g_u8_vb_dsc_path[i]);
                else
                    b_vb_h = api_misc_vb_status_get(g_u8_vb_path[i]);
            }
            while(b_vb_h == false && (g_u16_timer_out < 500));
        }
    }

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            b_freeze_status[u8dev][i] = TRUE;
            api_dma_common_buf_id_get(&st_dma_dev[i]);
            wdma_buf_id[u8dev] = st_dma_dev[i].st_dma_status.wdma_buf_id;
            rdma_buf_id[u8dev] = st_dma_dev[i].st_dma_status.rdma_buf_id;
            wdma_buf_id[u8dev] = (rdma_buf_id[u8dev] + 1) % 3;
            st_dma_dev[i].st_frc_cfg.u8_src_buf_id_sel = wdma_buf_id[u8dev];
            st_dma_dev[i].st_frc_cfg.u8_dst_buf_id_sel = rdma_buf_id[u8dev];
            api_dma_comom_buf_id_set(&st_dma_dev[i]);
            api_dma_common_trigger(&st_dma_dev[i]);
        }
    }
    __enable_irq();
}

VOID _video_freeze_out(UINT8 u8dev,UINT8 u8_path)
{
    UINT8 i;
    //video freeze
    __disable_irq();
    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            if(g_dsc_enable[u8dev])
                gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[i];
            else
                gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[i];
            gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
            gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;

            api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev);
//            api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][i],MS_TRUE);
		}
        b_freeze_status[u8dev][i] = TRUE;
	}
	api_dma_common_simultaneous_trigger();
    __enable_irq();
}

VOID _video_freeze_multichip(UINT8 u8ChipNum,UINT8 u8StartDev,UINT8* pu8_path)
{
    BOOL b_vb_h;
    DMA_COMMON_DEV_T st_dma_dev[4];
    UINT8 i,j,u8_num;

    //video freeze
    __disable_irq();

	u8_num = u8ChipNum;

    for(j=0;j<u8_num;j++)
    {
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
        for(i=0;i<4;i++)
        {
            if(pu8_path[j]&(1<<i))
            {
                if(g_dsc_enable[u8StartDev+j])
                    gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[i];
                else
                    gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[i];
                gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
                gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;

                if(g_dsc_enable[u8StartDev+j])
                    st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
                else
                    st_dma_dev[i].u8_sel = g_u8_dma_idx[i];
                api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev);
            }
        }
    }

    for(j=0;j<u8_num;j++)
    {
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
        for(i=0;i<4;i++)
        {
            if(pu8_path[j]&(1<<i))
            {
                g_u16_timer_out = 0;
                do
                {
                    g_u16_timer_out ++;
                    if(g_dsc_enable[u8StartDev+j])
                        b_vb_h = api_misc_vb_status_get(g_u8_vb_dsc_path[i]);
                    else
                        b_vb_h = api_misc_vb_status_get(g_u8_vb_path[i]);
                }
                while(b_vb_h == false && (g_u16_timer_out < 500));
            }
        }
    }

    for(j=0;j<u8_num;j++)
    {
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
        for(i=0;i<4;i++)
        {
            if(pu8_path[j]&(1<<i))
            {
                b_freeze_status[u8StartDev+j][i] = TRUE;
                api_dma_common_buf_id_get(&st_dma_dev[i]);
                wdma_buf_id[u8StartDev+j] = st_dma_dev[i].st_dma_status.wdma_buf_id;
                rdma_buf_id[u8StartDev+j] = st_dma_dev[i].st_dma_status.rdma_buf_id;
                wdma_buf_id[u8StartDev+j] = (rdma_buf_id[u8StartDev+j] + 1) % 3;
                st_dma_dev[i].st_frc_cfg.u8_src_buf_id_sel = wdma_buf_id[u8StartDev+j];
                st_dma_dev[i].st_frc_cfg.u8_dst_buf_id_sel = rdma_buf_id[u8StartDev+j];
                api_dma_comom_buf_id_set(&st_dma_dev[i]);
                api_dma_common_trigger(&st_dma_dev[i]);
            }
        }
    }
    __enable_irq();
}

VOID _video_unfreeze(UINT8 u8dev,UINT8 u8_path)
{
    UINT8 i;
    BOOL b_int;
    DMA_COMMON_DEV_T st_dma_dev[4];
    SS_DEV_T st_vopss_ss_dev[4];

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            if(g_dsc_enable[u8dev])
                st_vopss_ss_dev[i].u8_sel = g_u8_ss_ch_dsc_idx[i];
            else
                st_vopss_ss_dev[i].u8_sel = g_u8_ss_ch_idx[i];
            st_vopss_ss_dev[i].st_vopss_trigger_mode.u8_trigger_mode = NEXT_VB_RISING_EDGE_TRIGGER;
            st_vopss_ss_dev[i].st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
            st_vopss_ss_dev[i].st_vopss_trigger_mode.u8_trigger_tmg = VDS_TMG1 + i;

            api_ss_vopss_trig_mode_cfg(&st_vopss_ss_dev[i]);
        }
    }
    __disable_irq();

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            if(g_dsc_enable[u8dev])
                st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
            else
                st_dma_dev[i].u8_sel = g_u8_dma_idx[i];
            api_dma_common_fifo_status_get(&st_dma_dev[i]);

            if(g_dsc_enable[u8dev])
                st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
            else
                st_dma_dev[i].u8_sel = g_u8_dma_idx[i];

            if(st_dma_dev[i].st_dma_status.b_h2l_status == MS_TRUE)
            {
                api_dma_common_buf_id_get(&st_dma_dev[i]);
                wdma_buf_id[u8dev] = st_dma_dev[i].st_dma_status.wdma_buf_id;
            }

            st_dma_dev[i].st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 0) % 3;
            st_dma_dev[i].st_frc_cfg.u8_dst_buf_id_sel = (wdma_buf_id[u8dev] + 2) % 3;

            api_dma_comom_buf_id_set(&st_dma_dev[i]);

            api_misc_int_status_clr(g_u8_vb_vdstmg[i]);
        }
    }

    g_u16_timer_out = 0;
    do
    {
        g_u16_timer_out ++;
        for(i=0;i<4;i++)
        {
            if(u8_path&(1<<i))
            {
                b_int = api_misc_int_status_get(g_u8_vb_vdstmg[i]);
            }
        }
    }
    while((b_int != MS_TRUE) && (g_u16_timer_out < 500));

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            api_dma_common_trigger(&st_dma_dev[i]);
            api_dma_common_simultaneous_trigger();

            if(g_dsc_enable[u8dev])
            {
                api_dsc_decoder_pps_seamless_set(&gst_single_vopss_dev[u8dev][i].st_decoder_dev);
                gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[i];
            }
            else
            {
                gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[i];
            }
            gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
            gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
            api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev);
        }
    }

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            g_u16_timer_out = 0;
            api_misc_int_status_clr(g_u8_vb_vdstmg[i]);
            do
            {
                g_u16_timer_out ++;
                b_int = api_misc_int_status_get(g_u8_vb_vdstmg[i]);
            }
            while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
            api_dma_common_trigger(&st_dma_dev[i]);
            b_freeze_status[u8dev][i] = FALSE;
//            api_ss_vopss_trig(&st_vopss_ss_dev[i]);
        }
    }

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            api_ss_vopss_trig(&st_vopss_ss_dev[i]);
        }
    }
    __enable_irq();
}

VOID _video_unfreeze_out(UINT8 u8dev,UINT8 u8_path)
{
    UINT8 i;
	BOOL b_int;
	DMA_COMMON_DEV_T st_dma_dev[4];
	UINT16 u8_rx_clock = 0;
    UINT32 u32_rx_mdt_ints = 0;

    mculib_delay_ms(100);

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
			u8_rx_clock = ms1826_hal_hdmi_rx_get_tmds_clk(Mapp_Rx[u8dev][i]);
			if(u8_rx_clock == 0)
			{
                b_freeze_status[u8dev][i] = FALSE;
				VIDEOMUTE(u8dev,i, MS_TRUE);
			}

            u32_rx_mdt_ints = ms1826_hal_hdmi_rx_get_mdt_ints_ext(Mapp_Rx[u8dev][i]);
			api_hdmi_rx_update_state(&g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][i]]);
            if(u32_rx_mdt_ints & 0x3045)
                ms1826_hal_hdmi_rx_phy_process(Mapp_Rx[u8dev][i], g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][i]].st_rx_ints.b_clk_change, u8_rx_clock);
			ms1826_hal_hdmi_rx_phy_process(Mapp_Rx[u8dev][i], g_st_hdmi_rx[u8dev][Mapp_Rx[u8dev][i]].st_rx_ints.b_clk_change, u8_rx_clock);
		}
    }
	__disable_irq();
	mculib_delay_ms(50);
    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
            if(g_dsc_enable[u8dev])
        	    st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
            else
                st_dma_dev[i].u8_sel = g_u8_dma_idx[i];

			api_dma_common_buf_id_get(&st_dma_dev[i]);
			wdma_buf_id[u8dev] = st_dma_dev[i].st_dma_status.wdma_buf_id;


        	st_dma_dev[i].st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8dev] + 1) % 2;
        	st_dma_dev[i].st_frc_cfg.u8_dst_buf_id_sel = (wdma_buf_id[u8dev] + 0) % 2;
        	api_dma_comom_buf_id_set(&st_dma_dev[i]);

        }
    }

	g_u16_timer_out = 0;
	api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
	do
	{
		g_u16_timer_out ++;
		b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB);
	}
	while((b_int != MS_TRUE) && (g_u16_timer_out < 500));

    api_dma_common_simultaneous_trigger();

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
        	api_dsc_decoder_pps_seamless_set(&gst_single_vopss_dev[u8dev][i].st_decoder_dev);

        	gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[i];
        	gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
        	gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
        	api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8dev][i].st_single_com_wdma_dev);
			b_freeze_status[u8dev][i] = FALSE;
        }
    }

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

    for(i=0;i<4;i++)
    {
        if(u8_path&(1<<i))
        {
			u8_rx_clock = ms1826_hal_hdmi_rx_get_tmds_clk(Mapp_Rx[u8dev][i]);
			if(u8_rx_clock != 0)
			{
                log(" unfreeze have signal to unmute video RX%d\r\n",Mapp_Rx[u8dev][i]);
                b_freeze_status[u8dev][i] = FALSE;
				VIDEOMUTE(u8dev,Mapp_Rx[u8dev][i], false);

//                api_hdmi_tx_audio_mute(&g_st_hdmi_tx[u8dev][i],false);
			}
		}
    }
}

VOID _video_unfreeze_multichip(UINT8 u8ChipNum,UINT8 u8StartDev,UINT8* pu8_path)
{
    UINT8 i, j, u8_num;
    BOOL b_int;
    DMA_COMMON_DEV_T st_dma_dev[4];
    SS_DEV_T st_vopss_ss_dev[4];
	u8 l_u8OldDev = g_u8MS1826DevIndex;

	u8_num = u8ChipNum;

	for(j=0;j<u8_num;j++)
	{
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
		for(i=0;i<4;i++)
		{
			if(pu8_path[j]&(1<<i))
			{
				if(g_dsc_enable[u8StartDev+j])
					st_vopss_ss_dev[i].u8_sel = g_u8_ss_ch_dsc_idx[i];
				else
					st_vopss_ss_dev[i].u8_sel = g_u8_ss_ch_idx[i];
				st_vopss_ss_dev[i].st_vopss_trigger_mode.u8_trigger_mode = NEXT_VB_RISING_EDGE_TRIGGER;
				st_vopss_ss_dev[i].st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
				st_vopss_ss_dev[i].st_vopss_trigger_mode.u8_trigger_tmg = VDS_TMG1 + i;

				api_ss_vopss_trig_mode_cfg(&st_vopss_ss_dev[i]);
			}
		}
	}
    __disable_irq();

	for(j=0;j<u8_num;j++)
	{
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
		for(i=0;i<4;i++)
		{
			if(pu8_path[j]&(1<<i))
			{
				if(g_dsc_enable[u8StartDev+j])
					st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
				else
					st_dma_dev[i].u8_sel = g_u8_dma_idx[i];
				api_dma_common_fifo_status_get(&st_dma_dev[i]);

				if(g_dsc_enable[u8StartDev+j])
					st_dma_dev[i].u8_sel = g_u8_dma_dsc_idx[i];
				else
					st_dma_dev[i].u8_sel = g_u8_dma_idx[i];

				if(st_dma_dev[i].st_dma_status.b_h2l_status == MS_TRUE)
				{
					api_dma_common_buf_id_get(&st_dma_dev[i]);
					wdma_buf_id[u8StartDev+j] = st_dma_dev[i].st_dma_status.wdma_buf_id;
				}

				st_dma_dev[i].st_frc_cfg.u8_src_buf_id_sel = (wdma_buf_id[u8StartDev+j] + 0) % 3;
				st_dma_dev[i].st_frc_cfg.u8_dst_buf_id_sel = (wdma_buf_id[u8StartDev+j] + 1) % 3;

				api_dma_comom_buf_id_set(&st_dma_dev[i]);

				api_misc_int_status_clr(g_u8_vb_vdstmg[i]);
			}
		}
	}

	for(j=0;j<u8_num;j++)
	{
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
		g_u16_timer_out = 0;
		do
		{
			g_u16_timer_out ++;
			b_int = api_misc_int_status_get(g_u8_vb_vdstmg[0]);
		}
		while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
	}

	for(j=0;j<u8_num;j++)
	{
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
		for(i=0;i<4;i++)
		{
			if(pu8_path[j]&(1<<i))
			{
				api_dma_common_trigger(&st_dma_dev[i]);
				api_dma_common_simultaneous_trigger();

				if(g_dsc_enable[u8StartDev+j])
				{
					api_dsc_decoder_pps_seamless_set(&gst_single_vopss_dev[u8StartDev+j][i].st_decoder_dev);
					gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_dsc_idx[i];
				}
				else
				{
					gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.u8_sel = g_u8_dma_idx[i];
				}
				gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
				gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
				api_dma_comom_buf_id_mode_cfg(&gst_single_vipss_dev[u8StartDev+j][i].st_single_com_wdma_dev);
			}
		}
	}

	for(j=0;j<u8_num;j++)
	{
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
		for(i=0;i<4;i++)
		{

			api_dma_common_trigger(&st_dma_dev[i]);
			b_freeze_status[u8StartDev+j][i] = FALSE;
		}
	}

	for(j=0;j<u8_num;j++)
	{
		g_u8MS1826DevIndex = u8StartDev + j;
		g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
		for(i=0;i<4;i++)
		{
			if(pu8_path[j]&(1<<i))
			{
				api_ss_vopss_trig(&st_vopss_ss_dev[i]);
			}
		}
	}
    __enable_irq();
	g_u8MS1826DevIndex = l_u8OldDev;
}

VOID app_single_mode_switch_config(UINT8 u8dev,BOOL bIsOutCfg)
{
    UINT8 u8_i = 0;
    b_init[u8dev][0] = 0;
    b_init[u8dev][1] = 0;
    b_init[u8dev][2] = 0;
    b_init[u8dev][3] = 0;
    Bypass_Tx_Status[u8dev][0] = 0;
    Bypass_Tx_Status[u8dev][1] = 0;
    Bypass_Tx_Status[u8dev][2] = 0;
    Bypass_Tx_Status[u8dev][3] = 0;

    _video_freeze(u8dev,0x0F);

    _single_mode_frc_cfg(u8dev);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS1);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS2);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS3);
    _single_cfg(u8dev,g_dsc_enable[u8dev],VIPSS4);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[0]:g_u8_ss_ch_idx[0]);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[1]:g_u8_ss_ch_idx[1]);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[2]:g_u8_ss_ch_idx[2]);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[3]:g_u8_ss_ch_idx[3]);
    mculib_delay_ms(50);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS1);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS2);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS3);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],VOPSS4);

    if(bIsOutCfg)
    {
        for(u8_i = 0; u8_i < 4; u8_i ++)
        {
            u8 l_u8VIC = g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code;
            //tx audio config
            app_tx_audio_cfg(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);

             //tx video config
            g_st_tx_mux[u8dev][u8_i].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_i]];
            g_st_tx_mux[u8dev][u8_i].u8_video_mux = g_u8_misc_mixer[u8_i];
            api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8_i]);

            g_st_hdmi_tx[u8dev][u8_i].u8_txIndex = g_u8_HDMI_TX_PROT[u8_i];
            g_st_hdmi_tx[u8dev][u8_i].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8_i];
            g_st_hdmi_tx[u8dev][u8_i].b_full_edid = FALSE;
            g_st_hdmi_tx[u8dev][u8_i].u8_color_depth = HDMI_COLOR_DEPTH_24;
            g_st_hdmi_tx[u8dev][u8_i].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
            g_st_hdmi_tx[u8dev][u8_i].u8_audio_src = FROM_HDMI_RX_AUDIO;
            g_st_hdmi_tx[u8dev][u8_i].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
            g_st_hdmi_tx[u8dev][u8_i].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8_i].u16_pixclk;
            memcpy((&g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
            g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
            api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i],false);
            api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_i]);
            _video_port_hdmi_tx_driver_config(u8dev,u8_i);
            api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i],MS_TRUE);
            _app_video_single_mixer_cfg(u8dev,g_u8_mixer_idx[u8_i], g_dsc_enable[u8dev]?g_u8_mixer_layer_dsc_src[u8_i]:g_u8_mixer_layer_src[u8_i]);
        }
    }
    else
    {
        for(u8_i = 0; u8_i < 4; u8_i ++)
        {
            app_tx_audio_cfg(u8dev,Matrix_Tx_Mapping[u8dev][u8_i], u8_i);

             //tx video config
            g_st_tx_mux[u8dev][u8_i].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_i]];
            g_st_tx_mux[u8dev][u8_i].u8_video_mux = g_u8_misc_mixer[u8_i];
            api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8_i]);
            _app_video_single_mixer_cfg(u8dev,g_u8_mixer_idx[u8_i], g_dsc_enable[u8dev]?g_u8_mixer_layer_dsc_src[u8_i]:g_u8_mixer_layer_src[u8_i]);
        }
    }

    if(g_dsc_enable[u8dev])
	    app_video_vopss_trig(u8dev,g_u8_ss_ch_dsc_idx[0] | g_u8_ss_ch_dsc_idx[1] | g_u8_ss_ch_dsc_idx[2] | g_u8_ss_ch_dsc_idx[3]);
    else
        app_video_vopss_trig(u8dev,g_u8_ss_ch_idx[0] | g_u8_ss_ch_idx[1] | g_u8_ss_ch_idx[2] | g_u8_ss_ch_idx[3]);

    _video_unfreeze(u8dev,0x0F);

    OSDShown[u8dev] = MS_TRUE;
    sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}


#if 1
void app_single_win_cfg(UINT8 u8dev,stMs1826Wincfg_T* pstMs1826WinCfg,BOOL bUpdate)
{
    UINT8 u8Rx  = pstMs1826WinCfg->u8Rx;
    UINT8 u8Tx  = pstMs1826WinCfg->u8Tx;
    UINT16 u16X = pstMs1826WinCfg->u16X;
    UINT16 u16Y = pstMs1826WinCfg->u16Y;
    UINT16 u16W = pstMs1826WinCfg->u16W;
    UINT16 u16H = pstMs1826WinCfg->u16H;
//    u8 l_u8VIC = g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets.st_avi_infoframe.u8_video_code;
//	MISC_MODULE_CLK_T st_module_clk;

    g_st_single_app_dev[u8dev][u8Tx].st_win_size.u16_xpos = u16X;
    g_st_single_app_dev[u8dev][u8Tx].st_win_size.u16_ypos = u16Y;
    g_st_single_app_dev[u8dev][u8Tx].st_win_size.u16_width = u16W;
    g_st_single_app_dev[u8dev][u8Tx].st_win_size.u16_hight = u16H;

//    mculib_i2c_write_16bidx8bval(0xb6,0x4004 + u8Tx * 0x100, 0x00);    // freerun lock
//    mculib_i2c_write_16bidx8bval(0xb6,0x4004 + u8Tx * 0x100, 0x11);    // once lock
    _single_video_freeze(u8dev,u8Tx);
    Mapp_Rx[u8dev][u8Tx] = u8Rx;
    _single_cfg(u8dev,g_dsc_enable[u8dev],u8Tx);
    app_video_vipss_trig(u8dev,g_dsc_enable[u8dev]?g_u8_ss_ch_dsc_idx[u8Tx]:g_u8_ss_ch_idx[u8Tx]);
    _single_vopss_cfg(u8dev,g_dsc_enable[u8dev],u8Tx);

    //tx audio config
    app_tx_audio_cfg(u8dev,Matrix_Tx_Mapping[u8dev][u8Tx], u8Tx);

    _single_video_unfreeze(u8dev,u8Tx);

    log("[INFO] app_single_win_cfg rx[%d] tx[%d] x[%d] y[%d] w[%d] h[%d] update[%d]\r\n",u8Rx,u8Tx,u16X,u16Y,u16W,u16H,bUpdate);
}

void app_single_user_mode_win_config(UINT8 u8dev,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bUpdate)
{
    app_single_win_cfg(u8dev, pstMs1826WinTable,bUpdate);
}

void app_single_win_test(void)
{
    stMs1826Wincfg_T stMs1826WinTable[4];

    stMs1826WinTable[0].u8Rx = Mapp_Rx[0][0];
    stMs1826WinTable[0].u8Tx = 0;
    stMs1826WinTable[0].u16X = 0;
    stMs1826WinTable[0].u16Y = 0;
    stMs1826WinTable[0].u16W = gst_single_vipss_dev[0][0].st_single_sdn_dev.st_video_size.st_MemRect.u16_h/2;
    stMs1826WinTable[0].u16H = gst_single_vipss_dev[0][0].st_single_sdn_dev.st_video_size.st_MemRect.u16_v/2;

    stMs1826WinTable[1].u8Rx = Mapp_Rx[0][1];
    stMs1826WinTable[1].u8Tx = 1;
    stMs1826WinTable[1].u16X = gst_single_vipss_dev[0][1].st_single_sdn_dev.st_video_size.st_MemRect.u16_h/2;
    stMs1826WinTable[1].u16Y = 0;
    stMs1826WinTable[1].u16W = gst_single_vipss_dev[0][1].st_single_sdn_dev.st_video_size.st_MemRect.u16_h/2;
    stMs1826WinTable[1].u16H = gst_single_vipss_dev[0][1].st_single_sdn_dev.st_video_size.st_MemRect.u16_v/2;

    stMs1826WinTable[2].u8Rx = Mapp_Rx[0][2];
    stMs1826WinTable[2].u8Tx = 2;
    stMs1826WinTable[2].u16X = 0;
    stMs1826WinTable[2].u16Y = gst_single_vipss_dev[0][2].st_single_sdn_dev.st_video_size.st_MemRect.u16_v/2;
    stMs1826WinTable[2].u16W = gst_single_vipss_dev[0][2].st_single_sdn_dev.st_video_size.st_MemRect.u16_h/2;
    stMs1826WinTable[2].u16H = gst_single_vipss_dev[0][2].st_single_sdn_dev.st_video_size.st_MemRect.u16_v/2;

    stMs1826WinTable[3].u8Rx = Mapp_Rx[0][3];
    stMs1826WinTable[3].u8Tx = 3;
    stMs1826WinTable[3].u16X = gst_single_vipss_dev[0][3].st_single_sdn_dev.st_video_size.st_MemRect.u16_h/2;
    stMs1826WinTable[3].u16Y = gst_single_vipss_dev[0][3].st_single_sdn_dev.st_video_size.st_MemRect.u16_v/2;
    stMs1826WinTable[3].u16W = gst_single_vipss_dev[0][3].st_single_sdn_dev.st_video_size.st_MemRect.u16_h/2;
    stMs1826WinTable[3].u16H = gst_single_vipss_dev[0][3].st_single_sdn_dev.st_video_size.st_MemRect.u16_v/2;
    app_single_user_mode_win_config(0,&stMs1826WinTable[0],TRUE);
    app_single_user_mode_win_config(0,&stMs1826WinTable[1],TRUE);
    app_single_user_mode_win_config(0,&stMs1826WinTable[2],TRUE);
    app_single_user_mode_win_config(0,&stMs1826WinTable[3],TRUE);
}

//---------------------------------------------------------------------------------------------
//video mixer setup
static VOID _app_video_user_splitter_mode_mixer_cfg(UINT8 u8dev,UINT8 u8_vmixer_idx)
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

#if 1
static VOID app_single_vipss_rx_mux_cfg(UINT8 u8dev,UINT8 u8_vipss_sel, SINGLE_APP_T *pst_app_single_dev)
{
#if 0
    if(pst_app_single_dev->u8_single_mode < MODE_DUAL_UP1_DOWN2)
    {
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[pst_app_single_dev->u8_single_mode]];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][pst_app_single_dev->u8_single_mode].u8_color_space;
    }
    else
#endif
    {
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[u8_vipss_sel]];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][u8_vipss_sel].u8_color_space ;
    }
    log("gst_single_vipss_dev[%d][%d].st_vipss_mux_dev.u8_src_module = %d\r\n",u8dev,u8_vipss_sel,gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module);

    //vipss rx mux config
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_dst_module = g_u8_misc_module_vipss[u8_vipss_sel];
    api_misc_rx_mux_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev);

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

        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_idx = g_u8_4ksdn_idx[u8_vipss_sel];
        api_sdn4k_init(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_input_colorSpace = HDMI_COLORSPACE_YCBCR420;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_output_colorSpace = HDMI_COLORSPACE_YCBCR444;
        gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_mode = SDN4K_MODE_AUTO;
        api_sdn4k_config(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
        api_sdn4k_enable(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev, MS_ENABLE);
        Video_HDMIRx_Timing[u8dev][u8_vipss_sel].u16_vactive = g_st_hdmi_rx[u8dev][u8_vipss_sel].st_rx_video_timing.u16_vactive / 2;
    }
    else
    {
        //vipss rx csc config
        g_st_vipss_csc[u8dev][u8_vipss_sel].u8_sel = g_u8_csc_idx[u8_vipss_sel];
        g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        api_csc_rx_set(&g_st_vipss_csc[u8dev][u8_vipss_sel]);

#if 0
        if(pst_app_single_dev->u8_single_mode < MODE_DUAL_UP1_DOWN2 &&
            g_st_hdmi_rx[u8dev][pst_app_single_dev->u8_single_mode].st_rx_video_timing.u8_polarity % 2 == 0)
        {
            switch(g_st_hdmi_rx[u8dev][pst_app_single_dev->u8_single_mode].st_rx_video_timing.u16_vactive)
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
                    Video_HDMIRx_Timing[u8dev][pst_app_single_dev->u8_single_mode].u16_vactive = 1080;
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
                    Video_HDMIRx_Timing[u8dev][pst_app_single_dev->u8_single_mode].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_single_dev->u8_single_mode].st_rx_video_timing.u16_hactive/2;
                    Video_HDMIRx_Timing[u8dev][pst_app_single_dev->u8_single_mode].u16_vactive = 480;
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
                    Video_HDMIRx_Timing[u8dev][pst_app_single_dev->u8_single_mode].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_single_dev->u8_single_mode].st_rx_video_timing.u16_hactive/2;
                    Video_HDMIRx_Timing[u8dev][pst_app_single_dev->u8_single_mode].u16_vactive = 576;
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
#endif

void app_single_win_format_cfg(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bSubModeSw)
{
//    UINT8 u8Rx = pstMs1826WinTable->u8Rx;
    UINT8 u8Tx = pstMs1826WinTable->u8Tx;

    gst_single_vipss_dev[u8dev][u8WinID].st_single_vwdma_dev.b_enable = FALSE;
    api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8WinID].st_single_vwdma_dev);

    gst_single_vopss_dev[u8dev][u8WinID].st_single_vrdma_dev.b_enable = FALSE;
    api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8WinID].st_single_vrdma_dev);
    app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[u8WinID]);
    if(bSubModeSw == MS_TRUE)
    {
        _video_freeze(u8dev,1<<(VIPSS1+u8WinID));
        mculib_delay_ms(50);

//        g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
//        g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
//        g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
//        g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
//        g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
//        g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
//        g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
//        g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);
        g_st_tx_mux[u8dev][u8Tx].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8Tx]];
        g_st_tx_mux[u8dev][u8Tx].u8_video_mux = g_u8_misc_mixer[u8Tx];
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8Tx]);
    }
    else
    {
        UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets.st_avi_infoframe.u8_video_code;

//        _video_freeze(u8dev,1<<(VIPSS1+u8Rx));
//        mculib_delay_ms(50);

        //vipss1 instance init
        gst_single_vipss_dev[u8dev][VIPSS1+u8WinID].u8_vipss_sel = VIPSS1+u8WinID;
        _single_vipss_instance_init(u8dev,&gst_single_vipss_dev[u8dev][VIPSS1+u8WinID]);

        //vopss1 instance init
        gst_single_vopss_dev[u8dev][VOPSS1+u8WinID].u8_vopss_sel = VOPSS1+u8WinID;
        _single_vopss_instance_init(u8dev,&gst_single_vopss_dev[u8dev][VOPSS1+u8WinID]);

        //FRC
        _single_mode_frc_cfg(u8dev);

//        g_st_tx_mux[u8dev][0].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[0]];
//        g_st_tx_mux[u8dev][0].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][0]);
//        g_st_tx_mux[u8dev][1].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[1]];
//        g_st_tx_mux[u8dev][1].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][1]);
//        g_st_tx_mux[u8dev][2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[2]];
//        g_st_tx_mux[u8dev][2].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][2]);
//        g_st_tx_mux[u8dev][3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[3]];
//        g_st_tx_mux[u8dev][3].u8_video_mux = MISC_VIDEO_MODULE_MIX1;
//        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][3]);

        g_st_tx_mux[u8dev][u8Tx].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8Tx]];
        g_st_tx_mux[u8dev][u8Tx].u8_video_mux = g_u8_misc_mixer[u8Tx];//MISC_VIDEO_MODULE_MIX1+u8Tx;
        api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8Tx]);

        g_st_hdmi_tx[u8dev][u8Tx].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][u8Tx];
        g_st_hdmi_tx[u8dev][u8Tx].b_full_edid = FALSE;
        g_st_hdmi_tx[u8dev][u8Tx].u8_color_depth = HDMI_COLOR_DEPTH_24;
        g_st_hdmi_tx[u8dev][u8Tx].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
        g_st_hdmi_tx[u8dev][u8Tx].u8_audio_src = FROM_HDMI_RX_AUDIO;
        g_st_hdmi_tx[u8dev][u8Tx].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
        g_st_hdmi_tx[u8dev][u8Tx].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_pixclk;
        memcpy((&g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][u8Tx].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8Tx],false);
        api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8Tx]);
        _video_port_hdmi_tx_driver_config(u8dev,u8Tx);

        api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8Tx],MS_TRUE);
        _app_video_user_splitter_mode_mixer_cfg(u8dev,VIDEO_MIXER_1);

        //vopss tx csc para
        g_st_txcsc_dev[u8dev][u8Tx].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8Tx]];
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
        g_st_txcsc_dev[u8dev][u8Tx].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
        api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8Tx]);
    }
    app_single_vipss_rx_mux_cfg(u8dev,VIPSS1+u8WinID, &g_st_single_app_dev[u8dev][u8WinID]);
}

#if 1
static VOID app_single_vopss_splitter_cfg(UINT8 u8dev,UINT8 u8_vopss_sel, SINGLE_APP_T *pst_app_single_dev)
{
    MISC_MODULE_CLK_T st_module_clk;

    //vopss VRDMA config
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.b_enable = false;
    api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_request_len = 255;
    api_vrdma_request_len_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);

    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.b_enable = MS_TRUE;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_mem_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size = pst_app_single_dev->st_scale_size.tMemSize.u16_h;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size = pst_app_single_dev->st_scale_size.tMemSize.u16_v;

    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u16_line_offset = gst_single_vipss_dev[u8dev][u8_vopss_sel].st_single_vwdma_dev.u16_line_offset;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = pst_app_single_dev->st_dma_addr.u32_dma_start_addr_0;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = pst_app_single_dev->st_dma_addr.u32_dma_start_addr_1;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = pst_app_single_dev->st_dma_addr.u32_dma_start_addr_2;

    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_DISABLE;
    api_vrdma_mirror_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
    api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
    api_vrdma_start_addr_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
    api_vrdma_burst_num_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);
    api_vrdma_line_offset_cfg(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_vrdma_dev);

    // su clk src
    st_module_clk.u8_dst_module = g_u8_misc_su[u8_vopss_sel];
    st_module_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1;
    st_module_clk.u8_clkdiv = MISC_CLKDIV_1;
    api_misc_su_clkin_sel(&st_module_clk);

    //vopss su config
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.u8_sel = g_u8_su_idx[u8_vopss_sel];
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.b_enable = MS_TRUE;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.u8_su_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_h = pst_app_single_dev->st_scale_size.tMemSize.u16_h;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_MemRect.u16_v = pst_app_single_dev->st_scale_size.tMemSize.u16_v;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_h =
        pst_app_single_dev->st_video_win.u16_hde_sp - pst_app_single_dev->st_video_win.u16_hde_st;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_size.st_OutRect.u16_v =
        pst_app_single_dev->st_video_win.u16_vde_sp - pst_app_single_dev->st_video_win.u16_vde_st;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_st = pst_app_single_dev->st_video_win.u16_hde_st;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_hde_sp = pst_app_single_dev->st_video_win.u16_hde_sp;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_st = pst_app_single_dev->st_video_win.u16_vde_st;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_video_win.u16_vde_sp = pst_app_single_dev->st_video_win.u16_vde_sp;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u8_dis_win_en = MS_TRUE;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_st = pst_app_single_dev->st_video_win.u16_hde_st;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_hde_sp = pst_app_single_dev->st_video_win.u16_hde_sp;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_st = pst_app_single_dev->st_video_win.u16_vde_st;
    gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev.st_dis_win.u16_vde_sp = pst_app_single_dev->st_video_win.u16_vde_sp;
    api_su_enable_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
    api_su_scaler_size_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
    api_su_csc_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
    api_su_video_win_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);
    api_su_display_win_set(&gst_single_vopss_dev[u8dev][u8_vopss_sel].st_single_su_dev);

    log("st_video_win[%d].u16_hde_st==0x%04x\r\n",u8_vopss_sel,pst_app_single_dev->st_video_win.u16_hde_st);
    log("st_video_win[%d].u16_hde_sp==0x%04x\r\n",u8_vopss_sel,pst_app_single_dev->st_video_win.u16_hde_sp);
    log("st_video_win[%d].u16_vde_st==0x%04x\r\n",u8_vopss_sel,pst_app_single_dev->st_video_win.u16_vde_st);
    log("st_video_win[%d].u16_vde_sp==0x%04x\r\n",u8_vopss_sel,pst_app_single_dev->st_video_win.u16_vde_sp);
}

static VOID app_single_vipss_splitter_cfg(UINT8 u8dev,UINT8 u8_vipss_sel, SINGLE_APP_T *pst_app_single_dev)
{
    //vipss sdn config
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.u8_sel = g_u8_sdn_idx[u8_vipss_sel];
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.b_enable = MS_TRUE;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.u8_sdn_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_h = pst_app_single_dev->st_scale_size.tInSize.u16_h;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_InRect.u16_v = pst_app_single_dev->st_scale_size.tInSize.u16_v;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_h = pst_app_single_dev->st_scale_size.tMemSize.u16_h;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev.st_video_size.st_MemRect.u16_v = pst_app_single_dev->st_scale_size.tMemSize.u16_v;
    api_sdn_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
    log("st_scale_size[%d].tInSize.u16_h==0x%04x\r\n",u8_vipss_sel,pst_app_single_dev->st_scale_size.tInSize.u16_h);
    log("st_scale_size[%d].tInSize.u16_v==0x%04x\r\n",u8_vipss_sel,pst_app_single_dev->st_scale_size.tInSize.u16_v);
    log("st_scale_size[%d].tMemSize.u16_h==0x%04x\r\n",u8_vipss_sel,pst_app_single_dev->st_scale_size.tMemSize.u16_h);
    log("st_scale_size[%d].tMemSize.u16_v==0x%04x\r\n",u8_vipss_sel,pst_app_single_dev->st_scale_size.tMemSize.u16_v);

    api_sdn_scaler_size_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);
    api_sdn_csc_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_sdn_dev);

    //vipss vwdma config
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.b_enable = false;
    api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_request_len = 255;
    api_vwdma_request_len_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.b_enable = MS_TRUE;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_h_size = pst_app_single_dev->st_scale_size.tMemSize.u16_h;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_in_size.u16_v_size = pst_app_single_dev->st_scale_size.tMemSize.u16_v;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;

    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = pst_app_single_dev->st_dma_addr.u32_dma_start_addr_0;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = pst_app_single_dev->st_dma_addr.u32_dma_start_addr_1;
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = pst_app_single_dev->st_dma_addr.u32_dma_start_addr_2;
    api_vwdma_enable_set(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

    api_vwdma_start_addr_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u16_line_offset = g_st_single_app_dev[u8dev][u8_vipss_sel].st_scale_size.tMemSize.u16_h;
    api_vwdma_line_offset_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
    api_vwdma_burst_num_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);

    gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev.u8_vwdma_rotatation_deg = ROTATION_0_DEG;
    api_vwdma_rotation_cfg(&gst_single_vipss_dev[u8dev][u8_vipss_sel].st_single_vwdma_dev);
}
#endif

void app_user_splitter_win_cfg(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinCfg,BOOL bIsDsc,BOOL bUpdate)
{
    UINT8 u8Rx  = pstMs1826WinCfg->u8Rx;
    UINT8 u8Tx  = pstMs1826WinCfg->u8Tx;
    UINT16 u16X = pstMs1826WinCfg->u16X;
    UINT16 u16Y = pstMs1826WinCfg->u16Y;
    UINT16 u16W = pstMs1826WinCfg->u16W;
    UINT16 u16H = pstMs1826WinCfg->u16H;
    SINGLE_APP_T *pst_app_signle_dev = &g_st_single_app_dev[u8dev][u8WinID];

//    app_splitter_vipss_rx_mux_cfg(u8dev,VIPSS1+u8Rx, pst_app_signle_dev);
    //scale size
    pst_app_signle_dev->st_scale_size.tInSize.u16_h = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8WinID]].u16_hactive;
    pst_app_signle_dev->st_scale_size.tInSize.u16_v = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8WinID]].u16_vactive;
    pst_app_signle_dev->st_scale_size.tOutSize.u16_h = (((u16W==0)||(u16W>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive):(u16W));
    pst_app_signle_dev->st_scale_size.tOutSize.u16_v = (((u16H==0)||(u16H>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive):(u16H));
    pst_app_signle_dev->st_scale_size.tMemSize.u16_h = return_minof_three(pst_app_signle_dev->st_scale_size.tInSize.u16_h, pst_app_signle_dev->st_scale_size.tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
    pst_app_signle_dev->st_scale_size.tMemSize.u16_v = return_minof_three(pst_app_signle_dev->st_scale_size.tInSize.u16_v, pst_app_signle_dev->st_scale_size.tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

    //window size
    pst_app_signle_dev->st_video_win.u16_hde_st =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hoffset +
            ((u16X < g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive)?(u16X):(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive));
    pst_app_signle_dev->st_video_win.u16_hde_sp =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hoffset +
            (((u16W==0)||(u16W>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_hactive):(u16W+u16X));
    pst_app_signle_dev->st_video_win.u16_vde_st =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_voffset +
            ((u16Y < g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive)?(u16Y):(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive));
    pst_app_signle_dev->st_video_win.u16_vde_sp =
            g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_voffset +
            (((u16H==0)||(u16H>g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive))?(g_stVideo_HDMITx_Timing[u8dev][u8Tx].u16_vactive):(u16H+u16Y));

    //addr config
    pst_app_signle_dev->st_dma_addr.u32_dma_start_addr_0 = (MEMORY_BUF_ADDR_START)+3*u8WinID*(SINGLE_BUF_MEMORY_1080P_SIZE);
    pst_app_signle_dev->st_dma_addr.u32_dma_start_addr_1 = (MEMORY_BUF_ADDR_START)+3*u8WinID*(SINGLE_BUF_MEMORY_1080P_SIZE) + (SINGLE_BUF_MEMORY_1080P_SIZE);
    pst_app_signle_dev->st_dma_addr.u32_dma_start_addr_2 = (MEMORY_BUF_ADDR_START)+3*u8WinID*(SINGLE_BUF_MEMORY_1080P_SIZE) + 2*(SINGLE_BUF_MEMORY_1080P_SIZE);


    if(bUpdate)
    {
//        _single_cfg(u8dev,bIsDsc,VIPSS1+u8Rx);
        app_single_vipss_splitter_cfg(u8dev,VIPSS1+u8WinID, pst_app_signle_dev);

        app_video_vipss_trig(u8dev,(bIsDsc==MS_TRUE)?g_u8_ss_ch_dsc_idx[u8WinID]:g_u8_ss_ch_idx[u8WinID]);
        mculib_delay_ms(50);
//        _single_vopss_cfg(u8dev,bIsDsc,VOPSS1+u8Rx);
        app_single_vopss_splitter_cfg(u8dev,VOPSS1+u8WinID, pst_app_signle_dev);


        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer7_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[3]:g_u8_mixer_layer_src[3];
        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer6_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[2]:g_u8_mixer_layer_src[2];
        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer5_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[1]:g_u8_mixer_layer_src[1];
        g_st_video_mixer_dev[u8dev][VIDEO_MIXER_1].pst_layer_sel->u8_layer4_src_sel = (bIsDsc==MS_TRUE)?g_u8_mixer_layer_dsc_src[0]:g_u8_mixer_layer_src[0];
        _video_unfreeze(u8dev,0xFF);
    }
    log("[INFO] app_single_win_cfg rx[%d] tx[%d] x[%d] y[%d] w[%d] h[%d] dsc[%d] update[%d]\r\n",u8Rx,u8Tx,u16X,u16Y,u16W,u16H,bIsDsc,bUpdate);
}

void app_single_user_splitter_mode_win_config(UINT8 u8dev,UINT8 u8WinID,stMs1826Wincfg_T* pstMs1826WinTable,BOOL bUpdate,BOOL bIsModeSw)
{
    app_single_win_format_cfg(u8dev,u8WinID,pstMs1826WinTable,bIsModeSw);
    app_user_splitter_win_cfg(u8dev,u8WinID,pstMs1826WinTable,g_dsc_enable[u8dev],bUpdate);
}

void app_single_user_splitter_process(UINT8 u8dev,UINT8 u8_idx)
{
    app_single_vipss_rx_mux_cfg(u8dev,VIPSS1+u8_idx,&g_st_single_app_dev[u8dev][u8_idx]);
    g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tInSize.u16_h = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_idx]].u16_hactive;
    g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tInSize.u16_v = Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_idx]].u16_vactive;
#if 0
    g_st_single_app_dev[u8dev][u8_idx].st_scale_size[u8_idx].tOutSize.u16_h =
            (((g_stMs1826WinTable[u8dev][u8_idx].u16W>g_stVideo_HDMITx_Timing[u8dev][g_st_single_app_dev[u8dev][u8_idx].u8_dst_sel].u16_hactive))
            ?(g_stVideo_HDMITx_Timing[u8dev][g_st_single_app_dev[u8dev][u8_idx].u8_dst_sel].u16_hactive):(g_stMs1826WinTable[u8dev][u8_idx].u16W));
    g_st_single_app_dev[u8dev][u8_idx].st_scale_size[u8_idx].tOutSize.u16_v =
            (((g_stMs1826WinTable[u8dev][u8_idx].u16H>g_stVideo_HDMITx_Timing[u8dev][g_st_single_app_dev[u8dev][u8_idx].u8_dst_sel].u16_vactive))
            ?(g_stVideo_HDMITx_Timing[u8dev][g_st_single_app_dev[u8dev][u8_idx].u8_dst_sel].u16_vactive):(g_stMs1826WinTable[u8dev][u8_idx].u16H));
#endif
    g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h = return_minof_three(Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_idx]].u16_hactive, g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tOutSize.u16_h, MEMORY_1080P_H_SIZE_MAX);
    g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_v = return_minof_three(Video_HDMIRx_Timing[u8dev][Mapp_Rx[u8dev][u8_idx]].u16_vactive, g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tOutSize.u16_v, MEMORY_1080P_V_SIZE_MAX);

    log("2 st_scale_size[%d].tInSize.u16_h  == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tInSize.u16_h);
    log("2 st_scale_size[%d].tInSize.u16_h  == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tInSize.u16_v);
    log("2 st_scale_size[%d].tOutSize.u16_h == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tOutSize.u16_h);
    log("2 st_scale_size[%d].tOutSize.u16_v == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tOutSize.u16_v);
    log("2 st_scale_size[%d].tMemSize.u16_h == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h);
    log("2 st_scale_size[%d].tMemSize.u16_v == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_v);

    log("2 st_video_win[%d].u16_hde_st == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_video_win.u16_hde_st);
    log("2 st_video_win[%d].u16_hde_sp == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_video_win.u16_hde_sp);
    log("2 st_video_win[%d].u16_vde_st == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_video_win.u16_vde_st);
    log("2 st_video_win[%d].u16_vde_sp == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_video_win.u16_vde_sp);

    log("2 st_dma_addr[%d].u32_dma_start_addr_0 == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_0);
    log("2 st_dma_addr[%d].u32_dma_start_addr_1 == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_1);
    log("2 st_dma_addr[%d].u32_dma_start_addr_2 == 0x%08x\r\n",u8_idx,g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_2);

    //vipss sdn config
    gst_single_vipss_dev[u8dev][u8_idx].st_single_sdn_dev.st_video_size.st_InRect.u16_h = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tInSize.u16_h;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_sdn_dev.st_video_size.st_InRect.u16_v = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tInSize.u16_v;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_sdn_dev.st_video_size.st_MemRect.u16_h = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_sdn_dev.st_video_size.st_MemRect.u16_v = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_v;

    api_sdn_scaler_size_set(&gst_single_vipss_dev[u8dev][u8_idx].st_single_sdn_dev);
    //vipss vwdma config
    gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev.st_vwdma_in_size.u16_h_size = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev.st_vwdma_in_size.u16_v_size = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_v;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev.u16_line_offset = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h;

    gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_0;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_1;
    gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_2;
    api_vwdma_start_addr_cfg(&gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev);
    api_vwdma_line_offset_cfg(&gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev);
    api_vwdma_burst_num_cfg(&gst_single_vipss_dev[u8dev][u8_idx].st_single_vwdma_dev);


     //vopss VRDMA config
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.b_enable = MS_TRUE;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.u16_line_offset = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.st_vrdma_out_size.u16_h_size = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.st_vrdma_out_size.u16_v_size = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_v;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 = g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_0;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 = g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_1;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 = g_st_single_app_dev[u8dev][u8_idx].st_dma_addr.u32_dma_start_addr_2;

    api_vrdma_enable_set(&gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev);
    api_vrdma_start_addr_cfg(&gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev);
    api_vrdma_burst_num_cfg(&gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev);
    api_vrdma_line_offset_cfg(&gst_single_vopss_dev[u8dev][u8_idx].st_single_vrdma_dev);

    //vopss su config
    gst_single_vopss_dev[u8dev][u8_idx].st_single_su_dev.st_video_size.st_MemRect.u16_h = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_h;
    gst_single_vopss_dev[u8dev][u8_idx].st_single_su_dev.st_video_size.st_MemRect.u16_v = g_st_single_app_dev[u8dev][u8_idx].st_scale_size.tMemSize.u16_v;
    api_su_scaler_size_set(&gst_single_vopss_dev[u8dev][u8_idx].st_single_su_dev);

    app_video_vipss_trig(u8dev,g_u8_ss_ch_idx[u8_idx]);
    app_video_vopss_trig(u8dev,g_u8_ss_ch_idx[u8_idx]);
    mculib_delay_ms(200);

}

void app_single_user_splitter_mode_test(void)
{
    stMs1826Wincfg_T stMs1826WinTable[4];

    stMs1826WinTable[0].u8Rx = Mapp_Rx[0][0];
    stMs1826WinTable[0].u8Tx = 0;
    stMs1826WinTable[0].u16X = 0;
    stMs1826WinTable[0].u16Y = 0;
    stMs1826WinTable[0].u16W = 960;
    stMs1826WinTable[0].u16H = 540;
    g_bSplitterEnable[0][0] = TRUE;

    stMs1826WinTable[1].u8Rx = Mapp_Rx[0][1];
    stMs1826WinTable[1].u8Tx = 0;
    stMs1826WinTable[1].u16X = 960;
    stMs1826WinTable[1].u16Y = 0;
    stMs1826WinTable[1].u16W = 960;
    stMs1826WinTable[1].u16H = 540;
    g_bSplitterEnable[0][1] = TRUE;

    stMs1826WinTable[2].u8Rx = Mapp_Rx[0][1];
    stMs1826WinTable[2].u8Tx = 0;
    stMs1826WinTable[2].u16X = 0;
    stMs1826WinTable[2].u16Y = 540;
    stMs1826WinTable[2].u16W = 960;
    stMs1826WinTable[2].u16H = 540;
    g_bSplitterEnable[0][2] = TRUE;

    stMs1826WinTable[3].u8Rx = Mapp_Rx[0][3];
    stMs1826WinTable[3].u8Tx = 0;
    stMs1826WinTable[3].u16X = 960;
    stMs1826WinTable[3].u16Y = 540;
    stMs1826WinTable[3].u16W = 960;
    stMs1826WinTable[3].u16H = 540;
    g_bSplitterEnable[0][3] = TRUE;

    app_single_user_splitter_mode_win_config(0,0,&stMs1826WinTable[0],TRUE,TRUE);
    app_single_user_splitter_mode_win_config(0,1,&stMs1826WinTable[1],TRUE,TRUE);
    app_single_user_splitter_mode_win_config(0,2,&stMs1826WinTable[2],TRUE,TRUE);
    app_single_user_splitter_mode_win_config(0,3,&stMs1826WinTable[3],TRUE,TRUE);
}

#endif
#endif

#endif  //#ifndef __MS1826_APP_SINGLE_MODE_C_

