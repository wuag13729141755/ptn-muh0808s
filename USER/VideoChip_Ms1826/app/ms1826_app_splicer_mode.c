#ifndef __MS1826_APP_SPLICER_MODE_C_
#define __MS1826_APP_SPLICER_MODE_C_

#include "ms1826_videoprocess.h"

VIDEOSIZE_T g_u16_splicer_memsize_min[DEV_NUM_MS1826];
SPLICER_VIPSS_T gst_splicer_vipss_dev[DEV_NUM_MS1826][4];
SPLICER_VOPSS_T gst_splicer_vopss_dev[DEV_NUM_MS1826][4];
SPLICER_APP_T   gst_splicer_app_dev[DEV_NUM_MS1826];
UINT8 g_u8_splicer_vipss_sel[DEV_NUM_MS1826] = {{VIPSS1}};
UINT8 u8_h_border_fusion_r[DEV_NUM_MS1826][4] = {{0,0,0,0}};
UINT8 u8_h_border_fusion_l[DEV_NUM_MS1826][4] = {{0,0,0,0}};
UINT8 u8_v_border_fusion_t[DEV_NUM_MS1826][4] = {{0,0,0,0}};
UINT8 u8_v_border_fusion_b[DEV_NUM_MS1826][4] = {{0,0,0,0}};

UINT8 g_u8splicer_mirror[DEV_NUM_MS1826][4] = {{SPLICER_TX1_MIRROR, SPLICER_TX2_MIRROR, SPLICER_TX3_MIRROR, SPLICER_TX4_MIRROR}};

static VOID _splicer_vipss_instance_init(UINT8 u8dev,SPLICER_VIPSS_T *pst_splicer_vipss_dev)
{
	api_sdn_instance_init(&pst_splicer_vipss_dev->st_splicer_sdn_dev, g_u8_sdn_idx[pst_splicer_vipss_dev->u8_vipss_sel]);
	api_vwdma_instance_init(&pst_splicer_vipss_dev->st_splicer_vwdma_dev, g_u8_dma_idx[pst_splicer_vipss_dev->u8_vipss_sel]);
    u8dev = u8dev;
}

static VOID _splicer_vipss_rx_mux_cfg(UINT8 u8dev,UINT8 u8_vipss_sel, SPLICER_APP_T *pst_app_splicer_dev)
{
	//vipss rx sel
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_src_module = g_u8_misc_module_rx[g_u8_HDMI_RX_PROT[pst_app_splicer_dev->u8_src_sel]];
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev.u8_dst_module = u8_vipss_sel;
	api_misc_rx_mux_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_vipss_mux_dev);

	if(g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].u8_color_space == HDMI_COLORSPACE_YCBCR420)
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

		gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_idx = g_u8_4ksdn_idx[u8_vipss_sel];
		api_sdn4k_init(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
		gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_input_colorSpace = HDMI_COLORSPACE_YCBCR420;
		gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_output_colorSpace = HDMI_COLORSPACE_YCBCR444;
		gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev.u8_sdn4k_mode = SDN4K_MODE_AUTO;
		api_sdn4k_config(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev);
		api_sdn4k_enable(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_sdn4k_dev, MS_ENABLE);
		Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive = g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u16_vactive / 2;
	}
	else
	{
		//vipss rx csc config
		g_st_vipss_csc[u8dev][u8_vipss_sel].u8_sel = g_u8_csc_idx[u8_vipss_sel];
		g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_in_colorspace = g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].u8_color_space;
		g_st_vipss_csc[u8dev][u8_vipss_sel].st_csc_config_param.csc_out_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
		api_csc_rx_set(&g_st_vipss_csc[u8dev][u8_vipss_sel]);

		if(g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u8_polarity % 2 == 0)
		{
			switch(g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u16_vactive)
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
					Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive = 1080;
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
					Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u16_hactive / 2;
					Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive = 480;
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
					Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u16_hactive / 2;
					Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive = 576;
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
			if(g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u8_polarity % 2 == 0)
			{
				g_st_deint_dev[u8dev].b_deint_en = false;
				api_deint_config(&g_st_deint_dev[u8dev]);
				Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive = g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u16_hactive;
				Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive = g_st_hdmi_rx[u8dev][pst_app_splicer_dev->u8_src_sel].st_rx_video_timing.u16_vactive / 2;
			}
			//vipss mux config
			api_misc_vipss_mux_cfg(u8_vipss_sel, MISC_VIPSS_MUXTYPE_4);       //< CSC -> SDN -> VWDMA1
		}
	}
}

static VOID _splicer_vipss_cfg(UINT8 u8dev,UINT8 u8_vipss_sel, SPLICER_APP_T *pst_app_splicer_dev)
{
	//vipss sdn config

	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.u8_sel = g_u8_sdn_idx[u8_vipss_sel];
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.b_enable = MS_TRUE;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.u8_sdn_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.st_video_size.st_InRect.u16_h = pst_app_splicer_dev->tSdn_InSize.u16_h;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.st_video_size.st_InRect.u16_v = pst_app_splicer_dev->tSdn_InSize.u16_v;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.st_video_size.st_MemRect.u16_h = pst_app_splicer_dev->tSdn_MemSize.u16_h;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev.st_video_size.st_MemRect.u16_v = pst_app_splicer_dev->tSdn_MemSize.u16_v;
	api_sdn_enable_set(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev);
	api_sdn_scaler_size_set(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev);
	api_sdn_csc_set(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_sdn_dev);

	//vipss vwdma config
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.b_enable = MS_TRUE;
	api_vwdma_enable_set(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev);
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.st_vwdma_in_size.u16_h_size = pst_app_splicer_dev->tSdn_MemSize.u16_h;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.st_vwdma_in_size.u16_v_size = pst_app_splicer_dev->tSdn_MemSize.u16_v;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.u8_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.u16_line_offset = 3840;

	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_0 = pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_0;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_1 = pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_1;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.st_vwdma_start_addr.u32_dma_start_addr_2 = pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_2;

	api_vwdma_start_addr_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev);
	api_vwdma_line_offset_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev);
	api_vwdma_burst_num_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev);

	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.u16_request_len = 128;
	api_vwdma_request_len_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev);

	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev.u8_vwdma_rotatation_deg = 0;
	api_vwdma_rotation_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_vwdma_dev);
}


static VOID _splicer_vopss_instance_init(UINT8 u8dev,SPLICER_VOPSS_T *pst_splicer_vopss_dev)
{
	api_vrdma_instance_init(&pst_splicer_vopss_dev->st_splicer_vrdma_dev, g_u8_dma_idx[pst_splicer_vopss_dev->u8_vopss_sel]);
	api_su_instance_init(&pst_splicer_vopss_dev->st_splicer_su_dev, g_u8_su_idx[pst_splicer_vopss_dev->u8_vopss_sel]);
    u8dev = u8dev;
}

static VOID _splicer_vopss_cfg(UINT8 u8dev,UINT8 u8_vopss_sel, SPLICER_APP_T *pst_app_splicer_dev)
{
	MISC_MODULE_CLK_T st_module_clk;
	//vopss tx sel
	g_st_tx_mux[u8dev][u8_vopss_sel].u8_video_mux = g_u8_misc_mixer[u8_vopss_sel];
	g_st_tx_mux[u8dev][u8_vopss_sel].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[u8_vopss_sel]];
	api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][u8_vopss_sel]);

	//vopss VRDMA config
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u8_sel = g_u8_dma_idx[u8_vopss_sel];
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.b_enable = MS_TRUE;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u8_mem_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_out_size.u16_h_size = pst_app_splicer_dev->st_winborder[u8_vopss_sel].right - pst_app_splicer_dev->st_winborder[u8_vopss_sel].left;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_out_size.u16_v_size = pst_app_splicer_dev->st_winborder[u8_vopss_sel].bottom - pst_app_splicer_dev->st_winborder[u8_vopss_sel].top;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u16_line_offset = 3840;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_0 =
		pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_0 + pst_app_splicer_dev->st_winborder[u8_vopss_sel].left * 3 +
		pst_app_splicer_dev->st_winborder[u8_vopss_sel].top * gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u16_line_offset * 3;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_1 =
		pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_1 + pst_app_splicer_dev->st_winborder[u8_vopss_sel].left * 3 +
		pst_app_splicer_dev->st_winborder[u8_vopss_sel].top * gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u16_line_offset * 3;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_start_addr.u32_dma_start_addr_2 =
		pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_2 + pst_app_splicer_dev->st_winborder[u8_vopss_sel].left * 3 +
		pst_app_splicer_dev->st_winborder[u8_vopss_sel].top * gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u16_line_offset * 3;

	api_vrdma_enable_set(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev);
	api_vrdma_start_addr_cfg(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev);
	api_vrdma_burst_num_cfg(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev);
	api_vrdma_line_offset_cfg(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev);

	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u16_request_len = 128;
	api_vrdma_request_len_cfg(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev);

	if(pst_app_splicer_dev->u8_tx_mirror == false)
	{
		gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_DISABLE;
	}
	else
	{
		if(pst_app_splicer_dev->u8_tx_mirror & g_u8splicer_mirror[u8dev][u8_vopss_sel])
			gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_ENALBE;
	}
	api_vrdma_mirror_cfg(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev);

	//vopss su config
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.u8_sel = g_u8_su_idx[u8_vopss_sel];
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.b_enable = MS_TRUE;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.u8_su_color_space = CHIP_INTERNAL_PROCESS_COLORSPACE;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_size.st_MemRect.u16_h = gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_out_size.u16_h_size;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_size.st_MemRect.u16_v = gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_vrdma_dev.st_vrdma_out_size.u16_v_size;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_size.st_OutRect.u16_h = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_size.st_OutRect.u16_v = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_win.u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_win.u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_win.u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_video_win.u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_dis_win.u8_dis_win_en = MS_TRUE;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_dis_win.u16_hde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_dis_win.u16_hde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hoffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_hactive;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_dis_win.u16_vde_st = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset;
	gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev.st_dis_win.u16_vde_sp = g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_voffset + g_stVideo_HDMITx_Timing[u8dev][u8_vopss_sel].u16_vactive;

	api_su_enable_set(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev);
	api_su_scaler_size_set(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev);
	api_su_csc_set(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev);
	api_su_video_win_set(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev);
	api_su_display_win_set(&gst_splicer_vopss_dev[u8dev][u8_vopss_sel].st_splicer_su_dev);

	// su clk src
	st_module_clk.u8_dst_module = g_u8_misc_su[u8_vopss_sel];
	st_module_clk.u8_src_clk = MISC_CLKSRC_VDSTMG1 + u8_vopss_sel;
	st_module_clk.u8_clkdiv = MISC_CLKDIV_1;
	api_misc_su_clkin_sel(&st_module_clk);

	//vopss tx csc config
	g_st_txcsc_dev[u8dev][u8_vopss_sel].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[u8_vopss_sel]];
	g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
	g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
	g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
	g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
	g_st_txcsc_dev[u8dev][u8_vopss_sel].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
	api_csc_tx_set(&g_st_txcsc_dev[u8dev][u8_vopss_sel]);
}

static VOID _app_video_splicer_mixer_cfg(UINT8 u8dev,UINT8 u8_vmixer_idx, UINT8 u8_layer)
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

VOID _splicer_video_freeze(UINT8 u8dev,UINT8 u8_vipss_sel)
{
	BOOL b_vb_h;
	//video freeze
	__disable_irq();
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;

	api_dma_comom_buf_id_mode_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev);
	g_u16_timer_out = 0;
	do
	{
		g_u16_timer_out ++;
		b_vb_h = api_misc_vb_status_get(g_u8_vb_path[u8_vipss_sel]);
	}
	while(b_vb_h == false && (g_u16_timer_out < 500));

	api_dma_common_trigger(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev);
	api_dma_common_buf_id_get(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev);

	rdma_buf_id[u8dev] = gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_dma_status.rdma_buf_id;
	wdma_buf_id[u8dev] = (rdma_buf_id[u8dev] + 1) % 3;

	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_frc_cfg.u8_src_buf_id_sel = wdma_buf_id[u8dev];
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_sel = rdma_buf_id[u8dev];

	api_dma_comom_buf_id_set(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev);
	api_dma_common_trigger(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev);
	__enable_irq();
	mculib_delay_ms(50);
}

VOID _splicer_video_unfreeze(UINT8 u8dev,UINT8 u8_vipss_sel)
{
	SS_DEV_T st_vopss_ss_dev;
	DMA_COMMON_DEV_T st_dma_dev;
	st_vopss_ss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS1] | g_u8_ss_ch_idx[VOPSS2] | g_u8_ss_ch_idx[VOPSS3] | g_u8_ss_ch_idx[VOPSS4];
	st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode = FORCE_TRIGGER;
	st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
	api_ss_vopss_trig_mode_cfg(&st_vopss_ss_dev);
	__disable_irq();

	st_dma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
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

	st_dma_dev.u8_sel = g_u8_dma_idx[u8_vipss_sel];
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

	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
	gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
	api_dma_comom_buf_id_mode_cfg(&gst_splicer_vipss_dev[u8dev][u8_vipss_sel].st_splicer_com_wdma_dev);

	g_u16_timer_out = 0;
	api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
	do
	{
		g_u16_timer_out ++;
	}
	while(!api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB) && (g_u16_timer_out < 500));

	api_dma_common_simultaneous_trigger();

	__enable_irq();
}

VOID get_sdn_memsize_min(UINT8 u8dev)
{
	g_u16_splicer_memsize_min[u8dev].u16_h = (g_stVideo_HDMITx_Timing[u8dev][0].u16_hactive < g_stVideo_HDMITx_Timing[u8dev][1].u16_hactive) ? (g_stVideo_HDMITx_Timing[u8dev][0].u16_hactive) : (g_stVideo_HDMITx_Timing[u8dev][1].u16_hactive);
	g_u16_splicer_memsize_min[u8dev].u16_v = (g_stVideo_HDMITx_Timing[u8dev][0].u16_vactive < g_stVideo_HDMITx_Timing[u8dev][1].u16_vactive) ? (g_stVideo_HDMITx_Timing[u8dev][0].u16_vactive) : (g_stVideo_HDMITx_Timing[u8dev][1].u16_vactive);

	if(gst_splicer_app_dev[u8dev].u8_splicer_mode > SPLICER_MODE_2X1)
	{
		g_u16_splicer_memsize_min[u8dev].u16_h = (g_u16_splicer_memsize_min[u8dev].u16_h < g_stVideo_HDMITx_Timing[u8dev][2].u16_hactive) ? (g_u16_splicer_memsize_min[u8dev].u16_h) : (g_stVideo_HDMITx_Timing[u8dev][2].u16_hactive);
		g_u16_splicer_memsize_min[u8dev].u16_v = (g_u16_splicer_memsize_min[u8dev].u16_v < g_stVideo_HDMITx_Timing[u8dev][2].u16_vactive) ? (g_u16_splicer_memsize_min[u8dev].u16_v) : (g_stVideo_HDMITx_Timing[u8dev][2].u16_vactive);
	}
	if(gst_splicer_app_dev[u8dev].u8_splicer_mode > SPLICER_MODE_3X1)
	{
		g_u16_splicer_memsize_min[u8dev].u16_h = (g_u16_splicer_memsize_min[u8dev].u16_h < g_stVideo_HDMITx_Timing[u8dev][3].u16_hactive) ? (g_u16_splicer_memsize_min[u8dev].u16_h) : (g_stVideo_HDMITx_Timing[u8dev][3].u16_hactive);
		g_u16_splicer_memsize_min[u8dev].u16_v = (g_u16_splicer_memsize_min[u8dev].u16_v < g_stVideo_HDMITx_Timing[u8dev][3].u16_vactive) ? (g_u16_splicer_memsize_min[u8dev].u16_v) : (g_stVideo_HDMITx_Timing[u8dev][3].u16_vactive);
	}

}

static VOID _splicer_mode_cfg(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev)
{
	get_sdn_memsize_min(u8dev);
	switch(pst_app_splicer_dev->u8_splicer_mode)
	{
//		case SPLICER_MODE_1X1:
//			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
//			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);
//
//			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
//			{
//				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
//			}
//			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v * 2 && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
//			{
//				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
//			}
//
//			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 1);
//
//			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
//			app_video_vipss_trig(CHANNEL1_without_dsc);
//			mculib_delay_ms(50);
//
//			pst_app_splicer_dev->st_winborder[0].left = 0;
//			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h;
//			pst_app_splicer_dev->st_winborder[0].top = 0;
//			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v;
//
//			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
//
//			//vopss tx csc config
//			g_st_txcsc_dev[u8dev][VOPSS2].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[VOPSS2]];
//			g_st_txcsc_dev[u8dev][VOPSS2].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
//			g_st_txcsc_dev[u8dev][VOPSS2].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
//			g_st_txcsc_dev[u8dev][VOPSS2].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
//			g_st_txcsc_dev[u8dev][VOPSS2].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
//			g_st_txcsc_dev[u8dev][VOPSS2].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
//			api_csc_tx_set(&g_st_txcsc_dev[u8dev][VOPSS2]);
//
//			g_st_tx_mux[u8dev][VOPSS2].u8_video_mux = g_u8_misc_mixer[VOPSS2];
//			g_st_tx_mux[u8dev][VOPSS2].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[VOPSS2]];
//			api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][VOPSS2]);
//
//			//vopss tx csc config
//			g_st_txcsc_dev[u8dev][VOPSS3].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[VOPSS3]];
//			g_st_txcsc_dev[u8dev][VOPSS3].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
//			g_st_txcsc_dev[u8dev][VOPSS3].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
//			g_st_txcsc_dev[u8dev][VOPSS3].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
//			g_st_txcsc_dev[u8dev][VOPSS3].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
//			g_st_txcsc_dev[u8dev][VOPSS3].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
//			api_csc_tx_set(&g_st_txcsc_dev[u8dev][VOPSS3]);
//
//			g_st_tx_mux[u8dev][VOPSS3].u8_video_mux = g_u8_misc_mixer[VOPSS3];
//			g_st_tx_mux[u8dev][VOPSS3].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[VOPSS3]];
//			api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][VOPSS3]);
//
//			//vopss tx csc config
//			g_st_txcsc_dev[u8dev][VOPSS4].u8_sel = g_u8_csc_idx[g_u8_HDMI_TX_PROT[VOPSS4]];
//			g_st_txcsc_dev[u8dev][VOPSS4].st_csc_config_param.csc_in_colorspace = CHIP_INTERNAL_PROCESS_COLORSPACE;
//			g_st_txcsc_dev[u8dev][VOPSS4].st_csc_config_param.csc_in_color_range = CHIP_INTERNAL_PROCESS_COLORRANGE;
//			g_st_txcsc_dev[u8dev][VOPSS4].st_csc_config_param.csc_out_colorspace = CHIP_TX_CSC_OUT_COLORSPACE;
//			g_st_txcsc_dev[u8dev][VOPSS4].st_csc_config_param.csc_out_color_range = CHIP_TX_CSC_OUT_COLORRANGE;
//			g_st_txcsc_dev[u8dev][VOPSS4].st_csc_config_param.u8_pixel_repeat = HDMI_PIXEL_REPEAT_X1;
//			api_csc_tx_set(&g_st_txcsc_dev[u8dev][VOPSS4]);
//
//			g_st_tx_mux[u8dev][VOPSS4].u8_video_mux = g_u8_misc_mixer[VOPSS4];
//			g_st_tx_mux[u8dev][VOPSS4].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[VOPSS4]];
//			api_misc_tx_mux_cfg(&g_st_tx_mux[u8dev][VOPSS4]);
//			log("[INFO] MS1826 mode is SPLICER_MODE_1X1.");
//			break;
		case SPLICER_MODE_2X2:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, 2 * g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, 2 * g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v * 2 && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 2);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);

			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 + u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[2].left = u8_h_border_fusion_l[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 - u8_h_border_fusion_r[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 + u8_v_border_fusion_t[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v + u8_v_border_fusion_b[u8dev][2] * 8;

			pst_app_splicer_dev->st_winborder[3].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 + u8_h_border_fusion_l[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 + u8_v_border_fusion_t[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][3] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);

			pst_app_splicer_dev->st_winborder[1].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].top, 1);
			pst_app_splicer_dev->st_winborder[1].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[1].bottom, 1);
			pst_app_splicer_dev->st_winborder[2].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].left, 1);
			pst_app_splicer_dev->st_winborder[2].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].right, 1);

			pst_app_splicer_dev->st_winborder[2].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].top, 1);
			pst_app_splicer_dev->st_winborder[2].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[2].bottom, 1);
			pst_app_splicer_dev->st_winborder[3].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[3].left, 1);
			pst_app_splicer_dev->st_winborder[3].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[3].top, 1);
			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS3, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS4, pst_app_splicer_dev);
			log("[INFO] MS1826 mode is SPLICER_MODE_2X2.\r\n");
			break;
		case SPLICER_MODE_1X2:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, 2 * g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 2);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);


			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 + u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);

			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			log("[INFO] MS1826 mode is SPLICER_MODE_1X2.");
			break;
		case SPLICER_MODE_1X3:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, 3 * g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 3);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);

			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_r[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_l[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_t[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_b[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;


			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 3 - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 3 + u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 3 * 2 - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[2].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 3 * 2 + u8_h_border_fusion_l[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].top = u8_v_border_fusion_t[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v -u8_v_border_fusion_b[u8dev][2] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);

			pst_app_splicer_dev->st_winborder[1].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].top, 1);
			pst_app_splicer_dev->st_winborder[1].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[1].bottom, 1);
			pst_app_splicer_dev->st_winborder[2].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].left, 1);
			pst_app_splicer_dev->st_winborder[2].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].right, 1);
			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS3, pst_app_splicer_dev);
			log("[INFO] MS1826 mode is SPLICER_MODE_1X3.\r\n");
			break;
		case SPLICER_MODE_1X4:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, 4 * g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 4);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);

			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 + u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[2].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 2 + u8_h_border_fusion_l[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].right = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 * 3 - u8_h_border_fusion_r[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].top = u8_v_border_fusion_t[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][2] * 8;

			pst_app_splicer_dev->st_winborder[3].left = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 * 3 + u8_h_border_fusion_l[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].top = u8_v_border_fusion_t[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][3] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);

			pst_app_splicer_dev->st_winborder[1].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].top, 1);
			pst_app_splicer_dev->st_winborder[1].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[1].bottom, 1);
			pst_app_splicer_dev->st_winborder[2].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].left, 1);
			pst_app_splicer_dev->st_winborder[2].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].right, 1);

			pst_app_splicer_dev->st_winborder[2].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].top, 1);
			pst_app_splicer_dev->st_winborder[2].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[2].bottom, 1);
			pst_app_splicer_dev->st_winborder[3].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[3].left, 1);
			pst_app_splicer_dev->st_winborder[3].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[3].top, 1);
			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS3, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS4, pst_app_splicer_dev);
			log("[INFO] MS1826 mode is SPLICER_MODE_1X4.\r\n");
			break;
		case SPLICER_MODE_2X1:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, 2 * g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v * 2 && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 1);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);

			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 4 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 / 8 / 2;

			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 + u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);
			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			log("[INFO] MS1826 mode is SPLICER_MODE_2X1.\r\n");
			break;
		case SPLICER_MODE_3X1:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, 3 * g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v * 3 && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 1);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);

			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_r[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 2)
				u8_h_border_fusion_l[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 6 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_t[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 2)
				u8_v_border_fusion_b[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 6 / 8 / 2;

			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 3 - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 3 + u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 3 * 2 - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[2].left = u8_h_border_fusion_l[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 3 * 2 + u8_v_border_fusion_t[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][2] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);

			pst_app_splicer_dev->st_winborder[1].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].top, 1);
			pst_app_splicer_dev->st_winborder[1].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[1].bottom, 1);
			pst_app_splicer_dev->st_winborder[2].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].left, 1);
			pst_app_splicer_dev->st_winborder[2].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].right, 1);
			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS3, pst_app_splicer_dev);

			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_1, g_u8_mixer_layer_src[0]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_2, g_u8_mixer_layer_src[1]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_3, g_u8_mixer_layer_src[2]);
			log("[INFO] MS1826 mode is SPLICER_MODE_3X1.\r\n");
			break;
		case SPLICER_MODE_4X1:
			pst_app_splicer_dev->tSdn_MemSize.u16_h = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_h, g_u16_splicer_memsize_min[u8dev].u16_h, MEMORY_4K_H_SIZE_MAX);
			pst_app_splicer_dev->tSdn_MemSize.u16_v = return_minof_three(pst_app_splicer_dev->tSdn_InSize.u16_v, 4 * g_u16_splicer_memsize_min[u8dev].u16_v, MEMORY_4K_V_SIZE_MAX);

			if(pst_app_splicer_dev->tSdn_InSize.u16_v > pst_app_splicer_dev->tSdn_MemSize.u16_v && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}
			if(pst_app_splicer_dev->tSdn_MemSize.u16_v < g_u16_splicer_memsize_min[u8dev].u16_v * 4 && pst_app_splicer_dev->tSdn_MemSize.u16_h > 2048)
			{
				pst_app_splicer_dev->tSdn_MemSize.u16_h = 2048;
			}

			//pst_app_splicer_dev->tSdn_MemSize.u16_h = return_com_multiple(pst_app_splicer_dev->tSdn_MemSize.u16_h, 1);

			_splicer_vipss_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
			app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
			mculib_delay_ms(50);

			if(u8_h_border_fusion_r[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][0] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][0] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][1] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][1] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][2] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][2] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			if(u8_h_border_fusion_r[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_r[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_h_border_fusion_l[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 2)
				u8_h_border_fusion_l[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_h / 8 / 8 / 2;
			if(u8_v_border_fusion_t[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_t[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;
			if(u8_v_border_fusion_b[u8dev][3] * 8 > pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 2)
				u8_v_border_fusion_b[u8dev][3] = pst_app_splicer_dev->tSdn_MemSize.u16_v / 8 / 8 / 2;

			pst_app_splicer_dev->st_winborder[0].left = u8_h_border_fusion_l[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].top = u8_v_border_fusion_t[u8dev][0] * 8;
			pst_app_splicer_dev->st_winborder[0].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 - u8_v_border_fusion_b[u8dev][0] * 8;

			pst_app_splicer_dev->st_winborder[1].left = u8_h_border_fusion_l[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 + u8_v_border_fusion_t[u8dev][1] * 8;
			pst_app_splicer_dev->st_winborder[1].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 - u8_v_border_fusion_b[u8dev][1] * 8;

			pst_app_splicer_dev->st_winborder[2].left = u8_h_border_fusion_l[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 2 + u8_v_border_fusion_t[u8dev][2] * 8;
			pst_app_splicer_dev->st_winborder[2].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 * 3 - u8_v_border_fusion_b[u8dev][2] * 8;

			pst_app_splicer_dev->st_winborder[3].left = u8_h_border_fusion_l[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].right = pst_app_splicer_dev->tSdn_MemSize.u16_h - u8_h_border_fusion_r[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].top = pst_app_splicer_dev->tSdn_MemSize.u16_v / 4 * 3 + u8_v_border_fusion_t[u8dev][3] * 8;
			pst_app_splicer_dev->st_winborder[3].bottom = pst_app_splicer_dev->tSdn_MemSize.u16_v - u8_v_border_fusion_b[u8dev][3] * 8;

			pst_app_splicer_dev->st_winborder[0].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[0].right, 1);
			pst_app_splicer_dev->st_winborder[0].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[0].bottom, 1);
			pst_app_splicer_dev->st_winborder[1].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].left, 1);
			pst_app_splicer_dev->st_winborder[1].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].right, 1);

			pst_app_splicer_dev->st_winborder[1].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[1].top, 1);
			pst_app_splicer_dev->st_winborder[1].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[1].bottom, 1);
			pst_app_splicer_dev->st_winborder[2].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].left, 1);
			pst_app_splicer_dev->st_winborder[2].right 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].right, 1);

			pst_app_splicer_dev->st_winborder[2].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[2].top, 1);
			pst_app_splicer_dev->st_winborder[2].bottom = return_com_multiple(pst_app_splicer_dev->st_winborder[2].bottom, 1);
			pst_app_splicer_dev->st_winborder[3].left 	= return_com_multiple(pst_app_splicer_dev->st_winborder[3].left, 1);
			pst_app_splicer_dev->st_winborder[3].top 	= return_com_multiple(pst_app_splicer_dev->st_winborder[3].top, 1);

			_splicer_vopss_cfg(u8dev,VOPSS1, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS2, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS3, pst_app_splicer_dev);
			_splicer_vopss_cfg(u8dev,VOPSS4, pst_app_splicer_dev);
			log("[INFO] MS1826 mode is SPLICER_MODE_4X1.\r\n");
			break;
	}
	mculib_delay_ms(50);
}

static VOID _splicer_mode_frc_cfg(UINT8 u8dev)
{
	//VIPSS instance
	gst_splicer_vipss_dev[u8dev][g_u8_splicer_vipss_sel[u8dev]].u8_vipss_sel = g_u8_splicer_vipss_sel[u8dev];
	_splicer_vipss_instance_init(u8dev,&gst_splicer_vipss_dev[u8dev][g_u8_splicer_vipss_sel[u8dev]]);
	//VOPSS instance
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_vrdma_dev.u8_sel = g_u8_dma_idx[VOPSS1];
	gst_splicer_vopss_dev[u8dev][VOPSS1].u8_vopss_sel = VOPSS1;
	_splicer_vopss_instance_init(u8dev,&gst_splicer_vopss_dev[u8dev][VOPSS1]);
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_vrdma_dev.u8_sel = g_u8_dma_idx[VOPSS2];
	gst_splicer_vopss_dev[u8dev][VOPSS2].u8_vopss_sel = VOPSS2;
	_splicer_vopss_instance_init(u8dev,&gst_splicer_vopss_dev[u8dev][VOPSS2]);
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_vrdma_dev.u8_sel = g_u8_dma_idx[VOPSS3];
	gst_splicer_vopss_dev[u8dev][VOPSS3].u8_vopss_sel = VOPSS3;
	_splicer_vopss_instance_init(u8dev,&gst_splicer_vopss_dev[u8dev][VOPSS3]);
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_vrdma_dev.u8_sel = g_u8_dma_idx[VOPSS4];
	gst_splicer_vopss_dev[u8dev][VOPSS4].u8_vopss_sel = VOPSS4;
	_splicer_vopss_instance_init(u8dev,&gst_splicer_vopss_dev[u8dev][VOPSS4]);

	gst_splicer_vipss_dev[u8dev][g_u8_splicer_vipss_sel[u8dev]].st_splicer_com_wdma_dev.u8_sel = g_u8_dma_idx[g_u8_splicer_vipss_sel[u8dev]];
	gst_splicer_vipss_dev[u8dev][g_u8_splicer_vipss_sel[u8dev]].st_splicer_ss_vipss_dev.u8_sel = g_u8_dma_idx[g_u8_splicer_vipss_sel[u8dev]];
	gst_splicer_vipss_dev[u8dev][g_u8_splicer_vipss_sel[u8dev]].st_splicer_ss_vipss_dev.u8_vipss_wdma_id_sel = VOPSS1;
	api_ss_vipss_wdma_buf_sel(&gst_splicer_vipss_dev[u8dev][g_u8_splicer_vipss_sel[u8dev]].st_splicer_ss_vipss_dev);

	//DMA config
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS1];
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev.u8_dma_buf_num_sel = SPLICER_BUF_NUM;
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_AUTO;
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_AUTO;
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = false;
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_ss_vopss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS1];
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_ss_vopss_dev.u8_vopss_src_vb_sel = g_u8_splicer_vipss_sel[u8dev];
	gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS1;
	api_dma_common_buf_num_set(&gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev);
	api_dma_comom_buf_id_mode_cfg(&gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev);
	api_dma_comom_buf_frc_ctl_ext(&gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_com_rdma_dev);
	api_ss_vopss_src_vb_sel(&gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_ss_vopss_dev);
	api_ss_vopss_dst_buf_id_sel(&gst_splicer_vopss_dev[u8dev][VOPSS1].st_splicer_ss_vopss_dev);

	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS2];
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev.u8_dma_buf_num_sel = SPLICER_BUF_NUM;
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = MS_TRUE;
	api_dma_common_buf_num_set(&gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev);
	api_dma_comom_buf_id_mode_cfg(&gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev);
	api_dma_comom_buf_frc_ctl_ext(&gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_com_rdma_dev);
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_ss_vopss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS2];
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_ss_vopss_dev.u8_vopss_src_vb_sel = VOPSS2;
	gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS1;
	api_ss_vopss_src_vb_sel(&gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_ss_vopss_dev);
	api_ss_vopss_dst_buf_id_sel(&gst_splicer_vopss_dev[u8dev][VOPSS2].st_splicer_ss_vopss_dev);

	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS3];
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev.u8_dma_buf_num_sel = SPLICER_BUF_NUM;
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = MS_TRUE;
	api_dma_common_buf_num_set(&gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev);
	api_dma_comom_buf_id_mode_cfg(&gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev);
	api_dma_comom_buf_frc_ctl_ext(&gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_com_rdma_dev);
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_ss_vopss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS3];
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_ss_vopss_dev.u8_vopss_src_vb_sel = VOPSS3;
	gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS1;
	api_ss_vopss_src_vb_sel(&gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_ss_vopss_dev);
	api_ss_vopss_dst_buf_id_sel(&gst_splicer_vopss_dev[u8dev][VOPSS3].st_splicer_ss_vopss_dev);

	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev.u8_sel = g_u8_dma_idx[VOPSS4];
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev.u8_dma_buf_num_sel = SPLICER_BUF_NUM;
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev.st_frc_cfg.u8_src_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev.st_frc_cfg.u8_dst_buf_id_mode = DMA_BUF_SWITCH_MANU;
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev.st_frc_cfg.b_frc_ctl_ext = MS_TRUE;
	api_dma_common_buf_num_set(&gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev);
	api_dma_comom_buf_id_mode_cfg(&gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev);
	api_dma_comom_buf_frc_ctl_ext(&gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_com_rdma_dev);
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_ss_vopss_dev.u8_sel = g_u8_ss_ch_idx[VOPSS4];
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_ss_vopss_dev.u8_vopss_src_vb_sel = VOPSS4;
	gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_ss_vopss_dev.u8_vopss_dst_buf_id_sel = VOPSS1;
	api_ss_vopss_src_vb_sel(&gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_ss_vopss_dev);
	api_ss_vopss_dst_buf_id_sel(&gst_splicer_vopss_dev[u8dev][VOPSS4].st_splicer_ss_vopss_dev);
}

static VOID _splicer_audio_cfg(UINT8 u8dev,UINT8 u8_audio_src)
{
	UINT8 u8_i;
	for(u8_i = 0; u8_i < 4; u8_i++)
	{
		app_tx_audio_cfg(u8dev,u8_audio_src, u8_i);
	}
}
VOID splicer_video_mute(UINT8 u8dev,UINT8 u8_path, BOOL b_mute)
{
	UINT8 u8_i = 0;
	BOOL b_int;
	if(u8_path == gst_splicer_app_dev[u8dev].u8_src_sel)
	{
		for(u8_i = 0; u8_i < 4; u8_i++)
		{
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.bd_enable = b_mute;
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.u8_sel = g_u8_su_idx[u8_i];
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.border_color.u8VR = g_st_bg_color.u8_vr;
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.border_color.u8YG = g_st_bg_color.u8_yg;
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.border_color.u8UB = g_st_bg_color.u8_ub;
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.bd_cover = MS_TRUE;
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.border_size.u16_border_h = g_stVideo_HDMITx_Timing[u8dev][u8_i].u16_hactive;
			gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev.st_video_border.border_size.u16_border_v = g_stVideo_HDMITx_Timing[u8dev][u8_i].u16_vactive;
			api_su_video_border_set(&gst_splicer_vopss_dev[u8dev][u8_i].st_splicer_su_dev);
		}
		g_u16_timer_out = 0;
		api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
		do
		{
			g_u16_timer_out ++;
			b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB);
		}
		while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
		api_su_trigger(&gst_splicer_vopss_dev[u8dev][0].st_splicer_su_dev);
		api_su_trigger(&gst_splicer_vopss_dev[u8dev][1].st_splicer_su_dev);
		api_su_trigger(&gst_splicer_vopss_dev[u8dev][2].st_splicer_su_dev);
		api_su_trigger(&gst_splicer_vopss_dev[u8dev][3].st_splicer_su_dev);
	}
}
VOID app_splicer_rx_smeless_switch(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev)
{
	_splicer_video_freeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
	_splicer_vipss_rx_mux_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
	pst_app_splicer_dev->tSdn_InSize.u16_h = Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive > 0 ? Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive : 1920;
	pst_app_splicer_dev->tSdn_InSize.u16_v = Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive > 0 ? Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive : 1080;
	_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
	switch(pst_app_splicer_dev->u8_splicer_mode)
	{
//		case SPLICER_MODE_1X1:
		case SPLICER_MODE_2X2:
		case SPLICER_MODE_1X4:
		case SPLICER_MODE_4X1:
			break;
		case SPLICER_MODE_1X2:
		case SPLICER_MODE_2X1:
			Bypass_Tx_Status[u8dev][2] = 1;
			Matrix_Tx_Mapping[u8dev][2] = gst_splicer_app_dev[u8dev].u8_src_sel;
			switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][2], 2);

			Bypass_Tx_Status[u8dev][3] = 1;
			Matrix_Tx_Mapping[u8dev][3] = gst_splicer_app_dev[u8dev].u8_src_sel;
			switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][3], 3);
			break;
		case SPLICER_MODE_1X3:
		case SPLICER_MODE_3X1:
			Bypass_Tx_Status[u8dev][3] = 1;
			Matrix_Tx_Mapping[u8dev][3] = gst_splicer_app_dev[u8dev].u8_src_sel;
			switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][3], 3);
			break;
	}
	_splicer_video_unfreeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
	_splicer_audio_cfg(u8dev,pst_app_splicer_dev->u8_src_sel);
	OSDShown[u8dev] = MS_TRUE;
	sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}


VOID app_splicer_output_resolution(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev)
{
	UINT8 i=0;
	PLL_DEV_T         pll_dev;
	MISC_MODULE_CLK_T st_module_clk;
	VDS_TMG_DEV_T     st_splicer_timgen_dev;
	SS_DEV_T          st_vopss_ss_dev;

	for(i = 0 ; i < 4 ; i ++)
	{
        UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][i].st_infoframes_packets.st_avi_infoframe.u8_video_code;

		if(g_u8_output_index_bak[u8dev][i] == g_u8_output_index[u8dev][i])
			continue;

		if(i == 2 && (pst_app_splicer_dev->u8_splicer_mode >= SPLICER_MODE_1X2 && pst_app_splicer_dev->u8_splicer_mode <= SPLICER_MODE_2X1))
			continue;

		if(i == 3 && (pst_app_splicer_dev->u8_splicer_mode >= SPLICER_MODE_1X2 && pst_app_splicer_dev->u8_splicer_mode <= SPLICER_MODE_3X1))
			continue;

		_splicer_video_freeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);

		api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][i], false);
		api_misc_timing_get(g_arrOutputTable[g_u8_output_index[u8dev][i]], &g_stVideo_HDMITx_Timing[u8dev][i]);
		st_splicer_timgen_dev.pst_timing = &g_stVideo_HDMITx_Timing[u8dev][i];
		st_splicer_timgen_dev.b_timgen_en  = MS_ENABLE;
		st_splicer_timgen_dev.u8_timgen_id = TIMGEN_1 + i;
		st_splicer_timgen_dev.u8_workmode  = FreeRun_Mode;

		st_vopss_ss_dev.u8_sel = g_u8_ss_ch_idx[i];
		st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode = FORCE_TRIGGER;
		st_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;

//		pll_dev.u32_clk_in =
		pll_dev.b_osc_input = MS_TRUE;
		pll_dev.u8_pll_idx = PLLV1 + i;
		pll_dev.u32_clk_out = st_splicer_timgen_dev.pst_timing->u16_pixclk * 10000;

		st_module_clk.u8_dst_module = MISC_VIDEO_MODULE_TIMGEN1 + i;
		st_module_clk.u8_src_clk = MISC_CLKSRC_PLLV1 + i;
		st_module_clk.u8_clkdiv = MISC_CLKDIV_1;

		api_pll_config(&pll_dev);
		api_misc_timgen_clkin_sel(&st_module_clk);
		api_vds_tmg_config(&st_splicer_timgen_dev);
		_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
		api_ss_vopss_trig(&st_vopss_ss_dev);

		g_st_hdmi_tx[u8dev][i].u8_txIndex = g_u8_HDMI_TX_PROT[i];
		g_st_hdmi_tx[u8dev][i].b_is_hdmi = (BOOL)g_u8TxFormat[u8dev][i];
		g_st_hdmi_tx[u8dev][i].b_full_edid = FALSE ;
		g_st_hdmi_tx[u8dev][i].u8_color_depth = HDMI_COLOR_DEPTH_24;
		g_st_hdmi_tx[u8dev][i].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
//		g_st_hdmi_tx[u8dev][i].u8_audio_src = FROM_HDMI_RX_AUDIO;
		g_st_hdmi_tx[u8dev][i].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
		g_st_hdmi_tx[u8dev][i].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][i].u16_pixclk;
		memcpy((&g_st_hdmi_tx[u8dev][i].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
        g_st_hdmi_tx[u8dev][i].st_infoframes_packets.st_avi_infoframe.u8_video_code = l_u8VIC;
		api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][i], false);
		api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][i]);
        _video_port_hdmi_tx_driver_config(u8dev,i);
		api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][i], MS_TRUE);

		_splicer_video_unfreeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
	}

	OSDShown[u8dev] = MS_TRUE;
	sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}

VOID app_splicer_mode_config(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev)
{
	UINT8 u8_i = 0;

	if(pst_app_splicer_dev->b_splicer_sw != MS_TRUE)
	{
		gst_splicer_vipss_dev[u8dev][0].st_splicer_vwdma_dev.u8_sel = g_u8_dma_idx[0];
		gst_splicer_vipss_dev[u8dev][0].st_splicer_vwdma_dev.b_enable = false;
		api_vwdma_enable_set(&gst_splicer_vipss_dev[u8dev][0].st_splicer_vwdma_dev);
		gst_splicer_vipss_dev[u8dev][1].st_splicer_vwdma_dev.u8_sel = g_u8_dma_idx[1];
		gst_splicer_vipss_dev[u8dev][1].st_splicer_vwdma_dev.b_enable = false;
		api_vwdma_enable_set(&gst_splicer_vipss_dev[u8dev][1].st_splicer_vwdma_dev);
		gst_splicer_vipss_dev[u8dev][2].st_splicer_vwdma_dev.u8_sel = g_u8_dma_idx[2];
		gst_splicer_vipss_dev[u8dev][2].st_splicer_vwdma_dev.b_enable = false;
		api_vwdma_enable_set(&gst_splicer_vipss_dev[u8dev][2].st_splicer_vwdma_dev);
		gst_splicer_vipss_dev[u8dev][3].st_splicer_vwdma_dev.u8_sel = g_u8_dma_idx[3];
		gst_splicer_vipss_dev[u8dev][3].st_splicer_vwdma_dev.b_enable = false;
		api_vwdma_enable_set(&gst_splicer_vipss_dev[u8dev][3].st_splicer_vwdma_dev);
		app_video_vipss_trig(u8dev,CHANNEL1_without_dsc);
		app_video_vipss_trig(u8dev,CHANNEL2_without_dsc);
		app_video_vipss_trig(u8dev,CHANNEL3_without_dsc);
		app_video_vipss_trig(u8dev,CHANNEL4_without_dsc);
		_splicer_mode_frc_cfg(u8dev);
	}
	else
	{
		switch(pst_app_splicer_dev->u8_splicer_mode)
		{
			case SPLICER_MODE_1X2:
			case SPLICER_MODE_2X1:
				Bypass_Tx_Status[u8dev][2] = 1;
				Bypass_Tx_Status[u8dev][3] = 1;
				Matrix_Tx_Mapping[u8dev][2] = gst_splicer_app_dev[u8dev].u8_src_sel;
				Matrix_Tx_Mapping[u8dev][3] = gst_splicer_app_dev[u8dev].u8_src_sel;
				switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][2], 2);
				switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][3], 3);
				break;
			case SPLICER_MODE_1X3:
			case SPLICER_MODE_3X1:
				Bypass_Tx_Status[u8dev][3] = 1;
				Matrix_Tx_Mapping[u8dev][3] = gst_splicer_app_dev[u8dev].u8_src_sel;
				switch_bypass_channel(u8dev,Matrix_Tx_Mapping[u8dev][3], 3);
				break;
		}
		_splicer_video_freeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
	}
	_splicer_vipss_rx_mux_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
	pst_app_splicer_dev->tSdn_InSize.u16_h = Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive > 0 ? Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive : 1920;
	pst_app_splicer_dev->tSdn_InSize.u16_v = Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive > 0 ? Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive : 1080;
	pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_0 = MEMORY_BUF_ADDR_START;
	pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_1 = MEMORY_BUF_ADDR_START + SINGLE_BUF_MEMORY_4K_SIZE;
	pst_app_splicer_dev->st_wdma_addr.u32_dma_start_addr_2 = MEMORY_BUF_ADDR_START + SINGLE_BUF_MEMORY_4K_SIZE * 2;

    Mapp_Rx[u8dev][g_u8_splicer_vipss_sel[u8dev]] = g_u8_HDMI_RX_PROT[pst_app_splicer_dev->u8_src_sel];

    switch(pst_app_splicer_dev->u8_splicer_mode)
	{
//		case SPLICER_MODE_1X1:
//			for(u8_i =0; u8_i < 4; u8_i++)
//			{
//				if(Bypass_Tx_Status[u8dev][u8_i] == 1)
//				{
//					g_st_hdmi_tx[u8dev][u8_i].u8_txIndex = g_u8_HDMI_TX_PROT[u8_i];
//					g_st_hdmi_tx[u8dev][u8_i].b_is_hdmi = TRUE;
//					g_st_hdmi_tx[u8dev][u8_i].b_full_edid = 0;
//					g_st_hdmi_tx[u8dev][u8_i].u8_color_depth = HDMI_COLOR_DEPTH_24;
//					g_st_hdmi_tx[u8dev][u8_i].u8_audio_mode = HDMI_AUDIO_STREAM_TYPE_MAS;
//					g_st_hdmi_tx[u8dev][u8_i].u8_audio_src = FROM_HDMI_RX_AUDIO;
//					g_st_hdmi_tx[u8dev][u8_i].u8_video_clk_src = FROM_RX_OR_PLL_PIXEL_CLK;
//					g_st_hdmi_tx[u8dev][u8_i].u32_tmds_clk = g_stVideo_HDMITx_Timing[u8dev][u8_i].u16_pixclk;
//					memcpy((&g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets), &gst_inf_pack[u8dev], sizeof(HDMI_INFOFRAMES_PACKETS_T));
//					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], false);
//					mculib_delay_ms(200);
//					api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_i]);
//					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], MS_TRUE);
//					Bypass_Tx_Status[u8dev][u8_i] = 0;
//				}
//			}
//			_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
//			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_1, g_u8_mixer_layer_src[0]);
//			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_2, g_u8_mixer_layer_src[0]);
//			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_3, g_u8_mixer_layer_src[0]);
//			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_4, g_u8_mixer_layer_src[0]);
//			_splicer_video_unfreeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
//			break;
		case SPLICER_MODE_2X2:
		case SPLICER_MODE_4X1:
		case SPLICER_MODE_1X4:
			for(u8_i =0; u8_i < 4; u8_i++)
			{
				if(Bypass_Tx_Status[u8dev][u8_i] == 1)
				{
                    UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code;
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
					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], false);
					mculib_delay_ms(200);
					api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_i]);
                    _video_port_hdmi_tx_driver_config(u8dev,u8_i);
					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], MS_TRUE);
					Bypass_Tx_Status[u8dev][u8_i] = 0;
				}
			}
			_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_1, g_u8_mixer_layer_src[0]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_2, g_u8_mixer_layer_src[1]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_3, g_u8_mixer_layer_src[2]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_4, g_u8_mixer_layer_src[3]);
			_splicer_video_unfreeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
			break;
		case SPLICER_MODE_1X2:
		case SPLICER_MODE_2X1:
			for(u8_i =0; u8_i < 2; u8_i++)
			{
				if(Bypass_Tx_Status[u8dev][u8_i] == 1)
				{
                    UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code;
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
					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], false);
					mculib_delay_ms(200);
					api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_i]);
                    _video_port_hdmi_tx_driver_config(u8dev,u8_i);
					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], MS_TRUE);
					Bypass_Tx_Status[u8dev][u8_i] = 0;
				}
			}
			_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_1, g_u8_mixer_layer_src[0]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_2, g_u8_mixer_layer_src[1]);
			_splicer_video_unfreeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
			Bypass_Tx_Status[u8dev][2] = 1;
			Bypass_Tx_Status[u8dev][3] = 1;
			Matrix_Tx_Mapping[u8dev][2] = gst_splicer_app_dev[u8dev].u8_src_sel;
			Matrix_Tx_Mapping[u8dev][3] = gst_splicer_app_dev[u8dev].u8_src_sel;
			break;
		case SPLICER_MODE_1X3:
		case SPLICER_MODE_3X1:
			for(u8_i =0; u8_i < 3; u8_i++)
			{
				if(Bypass_Tx_Status[u8dev][u8_i] == 1)
				{
                    UINT8 l_u8VIC = g_st_hdmi_tx[u8dev][u8_i].st_infoframes_packets.st_avi_infoframe.u8_video_code;
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
					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], false);
					mculib_delay_ms(200);
					api_hdmi_tx_config_output(&g_st_hdmi_tx[u8dev][u8_i]);
                    _video_port_hdmi_tx_driver_config(u8dev,u8_i);
					api_hdmi_tx_output_en(&g_st_hdmi_tx[u8dev][u8_i], MS_TRUE);
					Bypass_Tx_Status[u8dev][u8_i] = 0;
				}
			}
			_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_1, g_u8_mixer_layer_src[0]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_2, g_u8_mixer_layer_src[1]);
			_app_video_splicer_mixer_cfg(u8dev,VIDEO_MIXER_3, g_u8_mixer_layer_src[2]);
			_splicer_video_unfreeze(u8dev,g_u8_splicer_vipss_sel[u8dev]);
			Bypass_Tx_Status[u8dev][3] = 1;
			Matrix_Tx_Mapping[u8dev][3] = gst_splicer_app_dev[u8dev].u8_src_sel;
			break;
	}
	_splicer_audio_cfg(u8dev,pst_app_splicer_dev->u8_src_sel);
	OSDShown[u8dev] = MS_TRUE;
	sys_show_osd(u8dev,OSDShown[u8dev], 0x0F);
}

VOID app_splicer_process(UINT8 u8dev,SPLICER_APP_T *pst_app_splicer_dev)
{
	_splicer_vipss_rx_mux_cfg(u8dev,g_u8_splicer_vipss_sel[u8dev], pst_app_splicer_dev);
	pst_app_splicer_dev->tSdn_InSize.u16_h = Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive > 0 ? Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_hactive : 1920;
	pst_app_splicer_dev->tSdn_InSize.u16_v = Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive > 0 ? Video_HDMIRx_Timing[u8dev][pst_app_splicer_dev->u8_src_sel].u16_vactive : 1080;
	_splicer_mode_cfg(u8dev,&gst_splicer_app_dev[u8dev]);
	switch(pst_app_splicer_dev->u8_splicer_mode)
	{
//		case SPLICER_MODE_1X1:
//			app_video_vopss_trig(CHANNEL1_without_dsc);
//			break;
		case SPLICER_MODE_2X2:
		case SPLICER_MODE_4X1:
		case SPLICER_MODE_1X4:
			app_video_vopss_trig(u8dev,CHANNEL1_without_dsc | CHANNEL2_without_dsc | CHANNEL3_without_dsc | CHANNEL4_without_dsc);
			break;
		case SPLICER_MODE_1X2:
		case SPLICER_MODE_2X1:
			app_video_vopss_trig(u8dev,CHANNEL1_without_dsc | CHANNEL2_without_dsc);
			break;
		case SPLICER_MODE_1X3:
		case SPLICER_MODE_3X1:
			app_video_vopss_trig(u8dev,CHANNEL1_without_dsc | CHANNEL2_without_dsc | CHANNEL3_without_dsc);
			break;
	}
	_splicer_audio_cfg(u8dev,pst_app_splicer_dev->u8_src_sel);
	mculib_delay_ms(200);
}

VOID app_splicer_mirror_config(UINT8 u8dev,UINT8 u8_tx_sel, BOOL b_mirror_en)
{
	BOOL b_int;
	SS_DEV_T st_split_vopss_ss_dev;
	st_split_vopss_ss_dev.u8_sel = g_u8_ss_ch_idx[u8_tx_sel];
	st_split_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_sel = TRIGGER_BY_VOPSS_TRIGGER;
	st_split_vopss_ss_dev.st_vopss_trigger_mode.u8_trigger_mode	= FORCE_TRIGGER;
	api_ss_vopss_trig_mode_cfg(&st_split_vopss_ss_dev);
	if(b_mirror_en)
	{
		gst_splicer_vopss_dev[u8dev][u8_tx_sel].st_splicer_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_ENALBE;
	}
	else
	{
		gst_splicer_vopss_dev[u8dev][u8_tx_sel].st_splicer_vrdma_dev.u8_mirror_cfg = DMA_H_V_MIRROR_ALL_DISABLE;
	}
	api_vrdma_mirror_cfg(&gst_splicer_vopss_dev[u8dev][u8_tx_sel].st_splicer_vrdma_dev);
	g_u16_timer_out = 0;
	api_misc_int_status_clr(MISC_INTSRC_VDSTMG1VB);
	do
	{
		g_u16_timer_out ++;
		b_int = api_misc_int_status_get(MISC_INTSRC_VDSTMG1VB);
	}
	while((b_int != MS_TRUE) && (g_u16_timer_out < 500));
	api_ss_vopss_trig(&st_split_vopss_ss_dev);
}

VOID app_splicer_osd_init(UINT8 u8dev)
{
	UINT8 idx = 0;
	for(idx = 0; idx < 4; idx ++)
	{
		g_st_misc_osd_mux[u8dev][idx].b_enable = MS_TRUE;
		g_st_misc_osd_mux[u8dev][idx].u8_osd_module = g_u8_misc_osd_idx[idx];
		g_st_misc_osd_mux[u8dev][idx].u8_tx_module = g_u8_misc_module_tx[g_u8_HDMI_TX_PROT[idx]];
		api_misc_osd_timing_mux_cfg(&g_st_misc_osd_mux[u8dev][idx]);
	}
}

VOID app_splicer_osd_config(UINT8 u8dev)
{
	UINT8 idx = 0;
	st_str[u8dev].u8_x = 0;
	st_str[u8dev].u8_y = 0;
	st_font[u8dev].u8_x = 0;
	st_font[u8dev].u8_y = 0;
	st_osd_win_size[u8dev].u8_osd_win = OSD_WIN_DISPLAY;
	st_osd_win_size[u8dev].u16_w = 15;
	st_osd_win_size[u8dev].u16_h = 4;
	st_osd_win_size[u8dev].u16_x = 100;
	st_osd_win_size[u8dev].u16_y = 100;
	app_splicer_osd_init(u8dev);
	for(idx = 0; idx < 4; idx ++)
	{
		api_osd_window_position_set(&g_st_osd_dev[u8dev][idx], &st_osd_win_size[u8dev]);
		st_font[u8dev].u8_y = 0;
		st_font[u8dev].pu16_idx = (UINT16 *)str_osd_input_channel[gst_splicer_app_dev[u8dev].u8_src_sel];
		st_font[u8dev].u16_length = 5;
		api_osd_show_index(&g_st_osd_dev[u8dev][idx], &st_font[u8dev]);

		st_font[u8dev].u8_y = 1;
		st_font[u8dev].pu16_idx = (UINT16 *)str_osd_output_channel[idx];
		st_font[u8dev].u16_length = 5;
		api_osd_show_index(&g_st_osd_dev[u8dev][idx], &st_font[u8dev]);

		st_str[u8dev].u8_y = 3;
		set_osd_res(u8dev,gst_splicer_app_dev[u8dev].u8_src_sel);
		st_str[u8dev].u16_len = 15;
		api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
	}

	switch(gst_splicer_app_dev[u8dev].u8_splicer_mode)
	{
		case SPLICER_MODE_1X2:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 1X2";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 2; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
		case SPLICER_MODE_1X3:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 1X3";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 3; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
		case SPLICER_MODE_1X4:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 1X4";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 4; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
		case SPLICER_MODE_2X1:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 2X1";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 2; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
		case SPLICER_MODE_3X1:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 3X1";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 3; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
		case SPLICER_MODE_4X1:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 4X1";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 4; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
		case SPLICER_MODE_2X2:
			st_str[u8dev].u8_y = 2;
			st_str[u8dev].pstr = "MODE: 2X2";
			st_str[u8dev].u16_len = 9;
			for(idx = 0; idx < 4; idx ++)
			{
				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
				app_osd_trigger(u8dev,idx);
			}
			break;
//		case SPLICER_MODE_1X1:
//			st_str[u8dev].u8_y = 2;
//			st_str[u8dev].pstr = "MODE: 1X1";
//			st_str[u8dev].u16_len = 9;
//			for(idx = 0; idx < 4; idx ++)
//			{
//				api_osd_show_length_string(&g_st_osd_dev[u8dev][idx], &st_str[u8dev]);
//				app_osd_trigger(u8dev,idx);
//			}
//			break;
	}
}
#endif
