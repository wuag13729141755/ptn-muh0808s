#include "includes.h"
#include "ms1826_app.h"
#include "ms1826_videoprocess.h"

BOOL b_power = MS_TRUE;
UINT8 g_u8_rotate = 0;
UINT8 g_u8_osd_menu_level = 0;
UINT8 g_u8_osd_menu_y = 0;

extern UINT8 g_u8_splitter_audio_src[DEV_NUM_MS1826];
extern BOOL b_osd_menu;
extern VOID app_osd_cfg(UINT8 u8Dev);
extern VOID app_osd_menu_drawrect(UINT8 u8Dev,UINT8 u8_level,UINT8 u8_y);
extern UINT8 g_u8_level[2];
extern UINT8 g_u8_bcsh_bri[4];
extern UINT8 g_u8_bcsh_con[4];
extern UINT8 g_u8_app_audio_output[3][4];
extern UINT8 g_u8_time;
extern UINT8 g_u8_ratio;
extern UINT8 g_u8OSDCount;
extern UINT8 g_u8_audio_sw;

UINT8 _remap_ir_key(UINT8 u8_ir_key)
{
	UINT8 u8_key = KEY_NONE;

	switch (u8_ir_key)
	{

		case IR_KEY_AUDIO_MUTE:
			u8_key = KEY_AUDIO_MUTE;
			break;
		case IR_KEY_POWER:
			u8_key = KEY_POWER;
			break;
		case IR_KEY_NUM_1:
			u8_key = KEY_NUM_1;
			break;
		case IR_KEY_NUM_2:
			u8_key = KEY_NUM_2;
			break;
		case IR_KEY_NUM_3:
			u8_key = KEY_NUM_3;
			break;
		case IR_KEY_NUM_4:
			u8_key = KEY_NUM_4;
			break;
		case IR_KEY_NUM_5:
			u8_key = KEY_NUM_5;
			break;
		case IR_KEY_NUM_6:
			u8_key = KEY_NUM_6;
			break;
		case IR_KEY_NUM_7:
			u8_key = KEY_NUM_7;
			break;
		case IR_KEY_NUM_8:
			u8_key = KEY_NUM_8;
			break;
		case IR_KEY_NUM_9:
			u8_key = KEY_NUM_9;
			break;
		case IR_KEY_T:
			u8_key = KEY_RESOLU;
			break;
		case IR_KEY_NUM_0:
			u8_key = KEY_DEFAULT;
			break;
		case IR_KEY_SETUP:
			u8_key = KEY_MENU;
			break;
		case IR_KEY_HOMEPAGE:
			u8_key = KEY_FUNCTION;
			break;
		case IR_KEY_UP:
			u8_key = KEY_UP;
			break;
		case IR_KEY_MENU:
			u8_key = KEY_MODE;
			break;
		case IR_KEY_LEFT:
			u8_key = KEY_LEFT;
			break;
		case IR_KEY_OK:
			u8_key = KEY_OK;
			break;
		case IR_KEY_RIGHT:
			u8_key = KEY_RIGHT;
			break;
		case IR_KEY_RETURN:
			u8_key = KEY_RETURN;
			break;
		case IR_KEY_DOWN:
			u8_key = KEY_DOWN;
			break;
		case IR_KEY_SOURCE:
			u8_key = KEY_RX_VIDEO_SEL;
			break;
		case IR_KEY_VOLUME_ADD:
			u8_key = KEY_AUDIO_ADD;
			break;
		case IR_KEY_VOLUME_SUB:
			u8_key = KEY_AUDIO_SUB;
			break;
		default:
			u8_key = 0xFF;
	}
	return u8_key;
}

VOID _key_process_0(UINT8 u8_key)
{
	UINT8 u8_i = 0;
	UINT8 g_u8_output_index_bak[4];
	if(b_power && b_osd_menu == MS_FALSE)
	{
		g_bOSDShown[0] = MS_FALSE;
		sys_show_osd(g_bOSDShown[0], 0x0F);
	}
	switch (u8_key)
	{
		case KEY_NUM_1:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					if(Bypass_Tx_Status[0][0] == MS_FALSE)
					{
						g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
						g_st_app_splitter_dev[0].u8_splitter_mode ++;
						if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_SIGNLE_4)
							g_st_app_splitter_dev[0].u8_splitter_mode = MODE_SIGNLE_1;
						app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					}
					else
					{
						Matrix_Tx_Mapping[0][0] ++;
						if(Matrix_Tx_Mapping[0][0] > 3)
							Matrix_Tx_Mapping[0][0] = 0;
						switch_bypass_channel(0,Matrix_Tx_Mapping[0][0], 0);
					}
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].u8_tx_mirror = gst_splicer_app_dev[0].u8_tx_mirror ^ SPLICER_TX1_MIRROR;
					app_splicer_mirror_config(0,HDMI_TX_A, gst_splicer_app_dev[0].u8_tx_mirror & SPLICER_TX1_MIRROR);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					Vipss_Mapping_Rx[0] ++;
					if(Vipss_Mapping_Rx[0] > 3)
						Vipss_Mapping_Rx[0] = 0;
					app_single_mode_rx_switch(0,Vipss_Mapping_Rx[0], 0);
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					Matrix_Tx_Mapping[0][0] ++;
					if(Matrix_Tx_Mapping[0][0] > 3)
						Matrix_Tx_Mapping[0][0] = 0;
					switch_bypass_channel(0,Matrix_Tx_Mapping[0][0], 0);
					break;
				default:
					break;
			}
			LOG("[INFO] HDMI TX1 SWITCH.\r\n");
			break;
		case KEY_NUM_2:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					if(Bypass_Tx_Status[0][1] == MS_FALSE)
					{
						g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
						g_st_app_splitter_dev[0].u8_splitter_mode ++;
						if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_SIGNLE_4)
							g_st_app_splitter_dev[0].u8_splitter_mode = MODE_SIGNLE_1;
						app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					}
					else
					{
						Matrix_Tx_Mapping[0][1] ++;
						if(Matrix_Tx_Mapping[0][1] > 3)
							Matrix_Tx_Mapping[0][1] = 0;
						switch_bypass_channel(0,Matrix_Tx_Mapping[0][1], 1);
					}
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].u8_tx_mirror = gst_splicer_app_dev[0].u8_tx_mirror ^ SPLICER_TX2_MIRROR;
					app_splicer_mirror_config(0,HDMI_TX_B, gst_splicer_app_dev[0].u8_tx_mirror & SPLICER_TX2_MIRROR);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					Vipss_Mapping_Rx[1] ++;
					if(Vipss_Mapping_Rx[1] > 3)
						Vipss_Mapping_Rx[1] = 0;
					app_single_mode_rx_switch(0,Vipss_Mapping_Rx[1], 1);
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					Matrix_Tx_Mapping[0][1] ++;
					if(Matrix_Tx_Mapping[0][1] > 3)
						Matrix_Tx_Mapping[0][1] = 0;
					switch_bypass_channel(0,Matrix_Tx_Mapping[0][1], 1);
					break;
				default:
					break;
			}
			LOG("[INFO] HDMI TX2 SWITCH.\r\n");
			break;
		case KEY_NUM_3:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					if(Bypass_Tx_Status[0][2] == MS_FALSE)
					{
						g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
						g_st_app_splitter_dev[0].u8_splitter_mode ++;
						if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_SIGNLE_4)
							g_st_app_splitter_dev[0].u8_splitter_mode = MODE_SIGNLE_1;
						app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					}
					else
					{
						Matrix_Tx_Mapping[0][2] ++;
						if(Matrix_Tx_Mapping[0][2] > 3)
							Matrix_Tx_Mapping[0][2] = 0;
						switch_bypass_channel(0,Matrix_Tx_Mapping[0][2], 2);
					}
					break;
				case MS1826_VIDEO_SPLICER:
					if(gst_splicer_app_dev[0].u8_splicer_mode < SPLICER_MODE_1X3)
					{
						Matrix_Tx_Mapping[0][2] ++;
						if(Matrix_Tx_Mapping[0][2] > 3)
							Matrix_Tx_Mapping[0][2] = 0;
						switch_bypass_channel(0,Matrix_Tx_Mapping[0][2], 2);
					}
					else
					{
						gst_splicer_app_dev[0].u8_tx_mirror = gst_splicer_app_dev[0].u8_tx_mirror ^ SPLICER_TX3_MIRROR;
						app_splicer_mirror_config(0,HDMI_TX_C, gst_splicer_app_dev[0].u8_tx_mirror & SPLICER_TX3_MIRROR);
					}
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					Vipss_Mapping_Rx[2] ++;
					if(Vipss_Mapping_Rx[2] > 3)
						Vipss_Mapping_Rx[2] = 0;
					app_single_mode_rx_switch(0,Vipss_Mapping_Rx[2], 2);
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					Matrix_Tx_Mapping[0][2] ++;
					if(Matrix_Tx_Mapping[0][2] > 3)
						Matrix_Tx_Mapping[0][2] = 0;
					switch_bypass_channel(0,Matrix_Tx_Mapping[0][2], 2);
					break;
				default:
					break;
			}
			LOG("[INFO] HDMI TX3 SWITCH.\r\n");
			break;
		case KEY_NUM_4:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					if(Bypass_Tx_Status[0][3] == MS_FALSE)
					{
						g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
						g_st_app_splitter_dev[0].u8_splitter_mode ++;
						if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_SIGNLE_4)
							g_st_app_splitter_dev[0].u8_splitter_mode = MODE_SIGNLE_1;
						app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					}
					else
					{
						Matrix_Tx_Mapping[0][3] ++;
						if(Matrix_Tx_Mapping[0][3] > 3)
							Matrix_Tx_Mapping[0][3] = 0;
						switch_bypass_channel(0,Matrix_Tx_Mapping[0][3], 3);
					}
					break;
				case MS1826_VIDEO_SPLICER:
					if(gst_splicer_app_dev[0].u8_splicer_mode < SPLICER_MODE_2X2)
					{
						Matrix_Tx_Mapping[0][3] ++;
						if(Matrix_Tx_Mapping[0][3] > 3)
							Matrix_Tx_Mapping[0][3] = 0;
						switch_bypass_channel(0,Matrix_Tx_Mapping[0][3], 3);
					}
					else
					{
						gst_splicer_app_dev[0].u8_tx_mirror = gst_splicer_app_dev[0].u8_tx_mirror ^ SPLICER_TX4_MIRROR;
						app_splicer_mirror_config(0,HDMI_TX_D, gst_splicer_app_dev[0].u8_tx_mirror & SPLICER_TX4_MIRROR);
					}
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					Vipss_Mapping_Rx[3] ++;
					if(Vipss_Mapping_Rx[3] > 3)
						Vipss_Mapping_Rx[3] = 0;
					app_single_mode_rx_switch(0,Vipss_Mapping_Rx[3], 3);
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					Matrix_Tx_Mapping[0][3] ++;
					if(Matrix_Tx_Mapping[0][3] > 3)
						Matrix_Tx_Mapping[0][3] = 0;
					switch_bypass_channel(0,Matrix_Tx_Mapping[0][3], 3);
					break;
				default:
					break;
			}
			LOG("[INFO] HDMI TX4 SWITCH.\r\n");
			break;
		case KEY_FUNCTION:
			g_bOSDShown[0] = MS_FALSE;
			sys_show_osd(0,g_bOSDShown[0], 0x0F);
            g_u8_display_mode_bak[0] = g_u8_display_mode[0];
			g_u8_display_mode[0] ++;
			if(g_u8_display_mode[0] > MS1826_BYPASS_MATRIX_SWITCHER)
				g_u8_display_mode[0] = MS1826_VIDEO_SPLITTER;
			api_hdmi_tx_output_en(&g_st_hdmi_tx[0][HDMI_TX_A], MS_FALSE);
			api_hdmi_tx_output_en(&g_st_hdmi_tx[0][HDMI_TX_B], MS_FALSE);
			api_hdmi_tx_output_en(&g_st_hdmi_tx[0][HDMI_TX_C], MS_FALSE);
			api_hdmi_tx_output_en(&g_st_hdmi_tx[0][HDMI_TX_D], MS_FALSE);
            video_ms1826_reset(0,g_u8_display_mode[0], g_u8_display_mode_bak[0]);
            switch (g_u8_display_mode[0])
            {
                case MS1826_VIDEO_SPLITTER:
                    g_st_app_splitter_dev[0].b_splitter_sw = MS_FALSE;
                    app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
                    for(u8_i = 0; u8_i < 4; u8_i ++)
                    {
                        if(g_u8_HDMI_TX_PROT[g_st_app_splitter_dev[0].u8_dst_sel] != g_u8_HDMI_TX_PROT[u8_i])
                        {
                            switch_bypass_channel(0,Matrix_Tx_Mapping[0][u8_i], u8_i);
                        }
                    }
					LOG("[INFO] MS1826 function is VIDEO_SPLITTER.\r\n");
                    break;
                case MS1826_VIDEO_SPLICER:
                    gst_splicer_app_dev[0].b_splicer_sw = MS_FALSE;
                    gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
                    app_splicer_mode_config(&gst_splicer_app_dev[0]);
                    switch(gst_splicer_app_dev[0].u8_splicer_mode)
                    {
                        case SPLICER_MODE_1X2:
                        case SPLICER_MODE_2X1:
                            switch_bypass_channel(0,Matrix_Tx_Mapping[0][2], 2);
                            switch_bypass_channel(0,Matrix_Tx_Mapping[0][3], 3);
                            break;
                        case SPLICER_MODE_1X3:
                        case SPLICER_MODE_3X1:
                            switch_bypass_channel(0,Matrix_Tx_Mapping[0][3], 3);
                            break;
                    }
					LOG("[INFO] MS1826 function is VIDEO_SPLICER.\r\n");
                    break;
                case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
                    app_single_mode_config(0);
					LOG("[INFO] MS1826 function is SEAMLESS_MATRIX_SWITCHER.\r\n");
                    break;
                case MS1826_BYPASS_MATRIX_SWITCHER:
                    for(u8_i = 0; u8_i < 4; u8_i ++)
                    {
                        if(Bypass_Tx_Status[0][u8_i] == 0)
                        {
                            Bypass_Tx_Status[0][u8_i] = 1;
                            switch_bypass_channel(0,Matrix_Tx_Mapping[0][u8_i], u8_i);
                        }
                    }
					LOG("[INFO] MS1826 function is BYPASS_MATRIX_SWITCHER.\r\n");
                    break;
                default:
                    break;
            }
            api_hdmi_tx_output_en(0,&g_st_hdmi_tx[0][HDMI_TX_A], MS_TRUE);
            api_hdmi_tx_output_en(0,&g_st_hdmi_tx[0][HDMI_TX_B], MS_TRUE);
            api_hdmi_tx_output_en(0,&g_st_hdmi_tx[0][HDMI_TX_C], MS_TRUE);
            api_hdmi_tx_output_en(0,&g_st_hdmi_tx[0][HDMI_TX_D], MS_TRUE);
            LOG("[INFO] Work mode switch.\r\n");
			break;
		case KEY_MODE:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
					g_st_app_splitter_dev[0].u8_splitter_mode ++;
					if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_FOUR_EQUALIZE)
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_SIGNLE_1;
					app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
					gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
					gst_splicer_app_dev[0].u8_splicer_mode ++;
					if(gst_splicer_app_dev[0].u8_splicer_mode > SPLICER_MODE_4X1)
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_1X2;

					app_splicer_mode_config(0,&gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] SPLICER MODE SWITCH.\r\n");
			break;
		case KEY_AUDIO:
		case KEY_AUDIO_ADD:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					app_splitter_mode_audio_src(0);
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].u8_src_sel ++;
					if(gst_splicer_app_dev[0].u8_src_sel > 3)
						gst_splicer_app_dev[0].u8_src_sel = 0;
					app_splicer_rx_smeless_switch(0,&gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] AUDIO CHANNAL ADD.\r\n");
			break;
		case KEY_RESOLU:
			g_bOSDShown[0] = MS_FALSE;
			sys_show_osd(0,g_bOSDShown[0], 0x0F);
			memcpy(g_u8_output_index_bak,g_u8_output_index,4);
			g_u8_output_index[0] ++;
			if(g_u8_output_index[0] > 5)
				g_u8_output_index[0] = 0;

			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					app_splitter_output_resolution(0,g_arrOutputTable[g_u8_output_index[0]],&g_st_app_splitter_dev[0]);
					break;
				case MS1826_VIDEO_SPLICER:
					app_splicer_output_resolution(0, &gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					app_single_output_resolution(0);
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] OUTPUT RESOLUTION SWITCH.\r\n");
			break;
		case KEY_POWER:
			b_power = !b_power;
			if(b_power)
			{
//				PORT_SetBits(POWER_ON_OFF_PORT, POWER_ON_OFF_PIN);
//				HDMI_POWERON_LED(TRUE);
//				HDMI_POWEROFF_LED(FALSE);
				LOG("[INFO] POWER ON.\r\n");
//				PORT_ResetBits(RESET_PORT, RESET_PIN);
//				Ddl_Delay1ms(2);
//				PORT_SetBits(RESET_PORT, RESET_PIN);
//				Ddl_Delay1ms(1);
				video_process_init(0);
			}
			else
			{
				api_hdmi_tx_audio_mute(0,&g_st_hdmi_tx[0][0], MS_TRUE);
				api_hdmi_tx_audio_mute(0,&g_st_hdmi_tx[0][1], MS_TRUE);
				api_hdmi_tx_audio_mute(0,&g_st_hdmi_tx[0][2], MS_TRUE);
				api_hdmi_tx_audio_mute(0,&g_st_hdmi_tx[0][3], MS_TRUE);
				app_digital_audio_cfg(0,0, g_u8_i2s_idx[0], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_i2s_idx[1], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_i2s_idx[2], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_i2s_idx[3], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_spdif_idx[0], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_spdif_idx[1], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_spdif_idx[2], MS_FALSE);
				app_digital_audio_cfg(0,0, g_u8_spdif_idx[3], MS_FALSE);

//				PORT_ResetBits(POWER_ON_OFF_PORT, POWER_ON_OFF_PIN);
//				HDMI_POWERON_LED(FALSE);
//				HDMI_POWEROFF_LED(TRUE);
//				HDMI_RX_LED1(FALSE);
//				HDMI_RX_LED2(FALSE);
//				HDMI_RX_LED3(FALSE);
//				HDMI_RX_LED4(FALSE);
//				HDMI_TX_LED1(FALSE);
//				HDMI_TX_LED2(FALSE);
//				HDMI_TX_LED3(FALSE);
//				HDMI_TX_LED4(FALSE);
				LOG("[INFO] POWER OFF.\r\n");
			}
			break;
		case KEY_UP:
			if(b_osd_menu == MS_TRUE)
			{
				if(g_u8_osd_menu_level == 0)
				{
					g_u8_osd_menu_y--;
					if(g_u8_osd_menu_y > 3)
						g_u8_osd_menu_y = 3;
					app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
					app_osd_trigger(0,OSD_1);
				}
				else if(g_u8_osd_menu_level == 1)
				{
					if(g_u8_level[0] == 0)
					{
						g_u8_osd_menu_y--;
						if(g_u8_osd_menu_y > 1)
							g_u8_osd_menu_y = 1;
						app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
						app_osd_trigger(0,OSD_1);
					}
					else if(g_u8_level[0] == 1)
					{
						g_u8_osd_menu_y--;
						if(g_u8_osd_menu_y > 4)
							g_u8_osd_menu_y = 4;
						app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
						app_osd_trigger(0,OSD_1);
					}
					else if(g_u8_level[0] == 2)
					{
						g_u8_osd_menu_y--;
						if(g_u8_osd_menu_y > 2)
							g_u8_osd_menu_y = 2;
						app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
						app_osd_trigger(0,OSD_1);
					}
				}
			}
			else
			{
				switch (g_u8_display_mode[0])
				{
					case MS1826_VIDEO_SPLITTER:
						break;
					case MS1826_VIDEO_SPLICER:
//						if(u8_v_border_fusion != 0)
//						{
//						u8_v_border_fusion --;
//								gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
//							app_splicer_mode_config(&gst_splicer_app_dev[0]);
//						}
						break;
					case MS1826_SEAMLESS_MATRIX_SWITCHER:
                    case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
						break;
					case MS1826_BYPASS_MATRIX_SWITCHER:
						break;
					default:
						break;
				}
			}
			LOG("[INFO] UP.\r\n");
			break;
		case KEY_DOWN:
			if(b_osd_menu == MS_TRUE)
			{
				if(g_u8_osd_menu_level == 0)
				{
					g_u8_osd_menu_y++;
					if(g_u8_osd_menu_y > 3)
						g_u8_osd_menu_y = 0;
					app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
					app_osd_trigger(0,OSD_1);
				}
				else if(g_u8_osd_menu_level == 1)
				{
					if(g_u8_level[0] == 0)
					{
						g_u8_osd_menu_y++;
						if(g_u8_osd_menu_y > 1)
							g_u8_osd_menu_y = 0;
						app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
						app_osd_trigger(0,OSD_1);
					}
					else if(g_u8_level[0] == 1)
					{
						g_u8_osd_menu_y++;
						if(g_u8_osd_menu_y > 4)
							g_u8_osd_menu_y = 0;
						app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
						app_osd_trigger(0,OSD_1);
					}
					else if(g_u8_level[0] == 2)
					{
						g_u8_osd_menu_y++;
						if(g_u8_osd_menu_y > 2)
							g_u8_osd_menu_y = 0;
						app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
						app_osd_trigger(0,OSD_1);
					}
				}
			}
			else
			{
				switch (g_u8_display_mode[0])
				{
					case MS1826_VIDEO_SPLITTER:
						break;
					case MS1826_VIDEO_SPLICER:
//						if(u8_v_border_fusion != 255)
//						{
//							u8_v_border_fusion ++;
//							gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
//							app_splicer_mode_config(&gst_splicer_app_dev[0]);
//						}
						break;
					case MS1826_SEAMLESS_MATRIX_SWITCHER:
                    case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
						break;
					case MS1826_BYPASS_MATRIX_SWITCHER:
						break;
					default:
						break;
				}
			}
			LOG("[INFO] DOWN.\r\n");
			break;
		case KEY_LEFT:
			if(b_osd_menu == MS_TRUE)
			{
				if(g_u8_osd_menu_level == 1)
				{
					if(g_u8_level[0] == 0)
					{
						if(g_u8_osd_menu_y == 0)
						{
							g_u8_bcsh_bri[0]--;
							if(g_u8_bcsh_bri[0] > 100)
								g_u8_bcsh_bri[0] = 0;
						}
						else if(g_u8_osd_menu_y == 1)
						{
							g_u8_bcsh_con[0]--;
							if(g_u8_bcsh_con[0] > 100)
								g_u8_bcsh_con[0] = 0;
						}

						app_bcsh_adjust(0,VEHC_VOPSS_1);
						app_bcsh_adjust(0,VEHC_VOPSS_2);
						app_bcsh_adjust(0,VEHC_VOPSS_3);
						app_bcsh_adjust(0,VEHC_VOPSS_4);
					}
					else if(g_u8_level[0] == 1)
					{
						if(g_u8_osd_menu_y == 0)
						{
							g_u8_audio_sw--;
							if(g_u8_audio_sw > 2)
								g_u8_audio_sw = 2;
						}
						else
						{
							g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] --;

							if(g_u8_audio_sw == 0)
							{
								if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] > 11)
									g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] = 11;

								if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] < 4)
									app_hdmi_input_hdmi_output_cfg(0,g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1],g_u8_osd_menu_y - 1);
								else if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] >= 4 && g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] < 8)
									app_digital_input_hdmi_output_cfg(0,g_u8_i2s_audio_idx[g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] - 4],g_u8_osd_menu_y - 1);
								else if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] >= 8 )
									app_digital_input_hdmi_output_cfg(0,g_u8_spdif_audio_idx[g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] - 8],g_u8_osd_menu_y - 1);
							}
							else
							{
								if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] > 3)
									g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] = 3;

								if(g_u8_audio_sw == 1)
									app_digital_audio_cfg(0,g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1],g_u8_i2s_idx[g_u8_osd_menu_y - 1],MS_TRUE);
								else if(g_u8_audio_sw == 2)
									app_digital_audio_cfg(0,g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1],g_u8_spdif_idx[g_u8_osd_menu_y - 1],MS_TRUE);
							}
						}
					}
					else if(g_u8_level[0] == 2)
					{
						if(g_u8_osd_menu_y == 0)
						{
							g_u8_time -- ;
							if(g_u8_time > 2)
								g_u8_time = 2;
						}
						else if(g_u8_osd_menu_y == 1)
						{
							g_u8_ratio++;
							if(g_u8_ratio > 100)
								g_u8_ratio = 100;
						}
					}
					app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
					app_osd_trigger(0,OSD_1);
				}
			}
			else
			{
				switch (g_u8_display_mode[0])
				{
					case MS1826_VIDEO_SPLITTER:
						break;
					case MS1826_VIDEO_SPLICER:
//						if(u8_h_border_fusion != 0)
						{
//							u8_h_border_fusion --;
//							gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
//							app_splicer_mode_config(&gst_splicer_app_dev[0]);
						}
						break;
					case MS1826_SEAMLESS_MATRIX_SWITCHER:
                    case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
						break;
					case MS1826_BYPASS_MATRIX_SWITCHER:
						break;
					default:
						break;
				}
			}
			LOG("[INFO] LEFT.\r\n");
			break;
		case KEY_RIGHT:
			if(b_osd_menu == MS_TRUE)
			{
				if(g_u8_osd_menu_level == 1)
				{
					if(g_u8_level[0] == 0)
					{
						if(g_u8_osd_menu_y == 0)
						{
							g_u8_bcsh_bri[0]++;
							if(g_u8_bcsh_bri[0] > 100)
								g_u8_bcsh_bri[0] = 100;
						}
						else if(g_u8_osd_menu_y == 1)
						{
							g_u8_bcsh_con[0]++;
							if(g_u8_bcsh_con[0] > 100)
								g_u8_bcsh_con[0] = 100;
						}

						app_bcsh_adjust(0,VEHC_VOPSS_1);
						app_bcsh_adjust(0,VEHC_VOPSS_2);
						app_bcsh_adjust(0,VEHC_VOPSS_3);
						app_bcsh_adjust(0,VEHC_VOPSS_4);
					}
					else if(g_u8_level[0] == 1)
					{
						if(g_u8_osd_menu_y == 0)
						{
							g_u8_audio_sw++;
							if(g_u8_audio_sw > 2)
								g_u8_audio_sw = 0;
						}
						else
						{
							g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] ++;

							if(g_u8_audio_sw == 0)
							{
								if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] > 11)
									g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] = 0;

								if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] < 4)
									app_hdmi_input_hdmi_output_cfg(0,g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1],g_u8_osd_menu_y - 1);
								else if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] >= 4 && g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] < 8)
									app_digital_input_hdmi_output_cfg(0,g_u8_i2s_audio_idx[g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] - 4],g_u8_osd_menu_y - 1);
								else if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] >= 8 )
									app_digital_input_hdmi_output_cfg(0,g_u8_spdif_audio_idx[g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] - 8],g_u8_osd_menu_y - 1);
							}
							else
							{
								if(g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] > 3)
									g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1] = 0;

								if(g_u8_audio_sw == 1)
									app_digital_audio_cfg(0,g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1],g_u8_i2s_idx[g_u8_osd_menu_y - 1],MS_TRUE);
								else if(g_u8_audio_sw == 2)
									app_digital_audio_cfg(0,g_u8_app_audio_output[g_u8_audio_sw][g_u8_osd_menu_y - 1],g_u8_spdif_idx[g_u8_osd_menu_y - 1],MS_TRUE);
							}
						}
					}
					else if(g_u8_level[0] == 2)
					{
						if(g_u8_osd_menu_y == 0)
						{
							g_u8_time ++ ;
							if(g_u8_time > 2)
								g_u8_time = 0;
						}
						else if(g_u8_osd_menu_y == 1)
						{
							g_u8_ratio--;
							if(g_u8_ratio < 20)
								g_u8_ratio = 20;
						}
					}
					app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_osd_menu_y);
					app_osd_trigger(0,OSD_1);
				}
			}
			else
			{
				switch (g_u8_display_mode[0])
				{
					case MS1826_VIDEO_SPLITTER:
						break;
					case MS1826_VIDEO_SPLICER:
//						if(u8_h_border_fusion != 255)
//						{
//							u8_h_border_fusion ++;
//							gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
//							app_splicer_mode_config(&gst_splicer_app_dev[0]);
//						}
						break;
					case MS1826_SEAMLESS_MATRIX_SWITCHER:
                    case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
						break;
					case MS1826_BYPASS_MATRIX_SWITCHER:
						break;
						default:
						break;
				}
			}
			LOG("[INFO] RIGHT.\r\n");
			break;
		case KEY_OK:
			if(b_osd_menu == MS_TRUE)
			{
				if(g_u8_osd_menu_level == 0 && g_u8_osd_menu_y < 3)
				{
					g_u8_osd_menu_level = 1;
					g_u8_osd_menu_y = 0;
					app_osd_menu_drawrect(0,g_u8_osd_menu_level,0);
					app_osd_trigger(0,OSD_1);
				}
				if(g_u8_osd_menu_level == 1)
				{
					if(g_u8_osd_menu_y == 2 && g_u8_level[0] == 2)
					{
						mculib_user_reset();
						NVIC_SystemReset();
					}
				}
			}
			else
			{
				switch (g_u8_display_mode[0])
				{
					case MS1826_VIDEO_SPLITTER:
						if(g_st_app_splitter_dev[0].u8_splitter_mode < MODE_DUAL_UP1_DOWN2)
						{
							g_u8_rotate++;
							if(g_u8_rotate > 3)
								g_u8_rotate = 0;
							app_splitter_rx_rotate(0,g_st_app_splitter_dev[0].u8_splitter_mode, g_u8_rotate);
						}
						break;
					case MS1826_VIDEO_SPLICER:
						break;
					case MS1826_SEAMLESS_MATRIX_SWITCHER:
                    case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
						break;
					case MS1826_BYPASS_MATRIX_SWITCHER:
						break;
					default:
						break;
				}
			}
			LOG("[INFO] OK.\r\n");
			break;
		case KEY_RETURN:
			if(b_osd_menu == MS_TRUE)
			{
				if(g_u8_osd_menu_level == 1)
				{
					g_u8_osd_menu_y = g_u8_level[0];
					g_u8_osd_menu_level = 0;
					app_osd_menu_drawrect(0,g_u8_osd_menu_level,g_u8_level[0]);
					app_osd_trigger(0,OSD_1);
				}
				else if(g_u8_osd_menu_level == 0)
				{
					b_osd_menu = MS_FALSE;
//					api_osd_window_enable(&g_st_osd_dev[0], OSD_WIN_DISPLAY, MS_FALSE);
//					api_osd_trigger(&g_st_osd_dev[0]);
					g_bOSDShown[0] = MS_FALSE;
					sys_show_osd(0,g_bOSDShown[0], 0x0F);
					g_bOSDShown[0] = MS_TRUE;
					sys_show_osd(0,g_bOSDShown[0], 0x0F);
				}
			}

			LOG("[INFO] RETURN.\r\n");
			break;
		case KEY_MENU:
			if(b_osd_menu == MS_FALSE && g_u8_display_mode[0] != MS1826_BYPASS_MATRIX_SWITCHER)
			{
				b_osd_menu = MS_TRUE;
				g_u8_osd_menu_level = 0;
				g_u8_osd_menu_y = 0;
				app_osd_cfg(0);
				app_osd_menu_drawrect(0,0,0);
//				app_splicer_osd_init(0);
				app_osd_trigger(0,OSD_1);
			}
			LOG("[INFO] MENU.\r\n");
			break;
		case KEY_RX_VIDEO_SEL:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].u8_src_sel ++;
					if(gst_splicer_app_dev[0].u8_src_sel > 3)
						gst_splicer_app_dev[0].u8_src_sel = 0;
					app_splicer_rx_smeless_switch(0,&gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] RX VIDEO SEL.\r\n");
			break;
		case KEY_AUDIO_MUTE:
			b_audio_mute[0][0] = !b_audio_mute[0][0];
			b_audio_mute[0][1] = !b_audio_mute[0][1];
			b_audio_mute[0][2] = !b_audio_mute[0][2];
			b_audio_mute[0][3] = !b_audio_mute[0][3];
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					app_tx_audio_cfg(0,g_u8_splitter_audio_src[0], g_st_app_splitter_dev[0].u8_dst_sel);
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][1], 1);
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][2], 2);
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][3], 3);
					break;
				case MS1826_VIDEO_SPLICER:
					app_tx_audio_cfg(0,gst_splicer_app_dev[0].u8_src_sel, 0);
					app_tx_audio_cfg(0,gst_splicer_app_dev[0].u8_src_sel, 1);
					switch(gst_splicer_app_dev[0].u8_splicer_mode)
					{
						case SPLICER_MODE_2X2:
						case SPLICER_MODE_1X4:
						case SPLICER_MODE_4X1:
							app_tx_audio_cfg(0,gst_splicer_app_dev[0].u8_src_sel, 2);
							app_tx_audio_cfg(0,gst_splicer_app_dev[0].u8_src_sel, 3);
							break;
						case SPLICER_MODE_1X2:
						case SPLICER_MODE_2X1:
							app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][2], 2);
							app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][3], 3);
							break;
						case SPLICER_MODE_1X3:
						case SPLICER_MODE_3X1:
							app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][3], 3);
							break;
					}
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
				case MS1826_BYPASS_MATRIX_SWITCHER:
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][0], 0);
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][1], 1);
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][2], 2);
					app_tx_audio_cfg(0,Matrix_Tx_Mapping[0][3], 3);
					break;
				default:
					break;
			}
			LOG("[INFO] AUDIO MUTE.\r\n");
			break;
		case KEY_DEFAULT:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					if(g_st_app_splitter_dev[0].u8_splitter_mode != MODE_FOUR_EQUALIZE)
					{
						g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_FOUR_EQUALIZE;
						app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					}
					break;
				case MS1826_VIDEO_SPLICER:
					if(gst_splicer_app_dev[0].u8_splicer_mode != SPLICER_MODE_2X2)
					{
						gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
						gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_2X2;
						app_splicer_mode_config(&gst_splicer_app_dev[0]);
					}
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] Mode default.\r\n");
			break;
		case KEY_NUM_5:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
					g_st_app_splitter_dev[0].u8_splitter_mode ++;
					if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_DUAL_UP3_DOWN4_4_3 || g_st_app_splitter_dev[0].u8_splitter_mode < MODE_DUAL_UP1_DOWN2)
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_DUAL_UP1_DOWN2;
					app_splitter_mode_config(0,0,&g_st_app_splitter_dev[0]);
					break;
				case MS1826_VIDEO_SPLICER:
					if(gst_splicer_app_dev[0].u8_splicer_mode != SPLICER_MODE_2X2)
					{
						gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
						gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_2X2;
						app_splicer_mode_config(0,&gst_splicer_app_dev[0]);
					}
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] KEY_NUM_5.\r\n");
			break;
		case KEY_NUM_6:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
					g_st_app_splitter_dev[0].u8_splitter_mode ++;
					if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_DUAL_LEFT3_RIGHT4_4_3 || g_st_app_splitter_dev[0].u8_splitter_mode < MODE_DUAL_LEFT1_RIGHT2)
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_DUAL_LEFT1_RIGHT2;
					app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
					gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
					if(gst_splicer_app_dev[0].u8_splicer_mode != SPLICER_MODE_1X2)
					{
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_1X2;
					}
					else
					{
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_2X1;
					}
					app_splicer_mode_config(0,&gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] KEY_NUM_6.\r\n");
			break;
		case KEY_NUM_7:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
					g_st_app_splitter_dev[0].u8_splitter_mode ++;
					if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_PIP_43_1_3 || g_st_app_splitter_dev[0].u8_splitter_mode < MODE_PIP_12_1_3)
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_PIP_12_1_3;
					app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
					gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
					if(gst_splicer_app_dev[0].u8_splicer_mode != SPLICER_MODE_1X3)
					{
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_1X3;
					}
					else
					{
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_3X1;
					}
					app_splicer_mode_config(0,&gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] KEY_NUM_7.\r\n");
			break;
		case KEY_NUM_8:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					if(g_st_app_splitter_dev[0].u8_splitter_mode != MODE_THREE_EQUALIZE)
					{
						g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_THREE_EQUALIZE;
						app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					}
					break;
				case MS1826_VIDEO_SPLICER:
					gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
					gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
					if(gst_splicer_app_dev[0].u8_splicer_mode != SPLICER_MODE_1X4)
					{
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_1X4;
					}
					else
					{
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_4X1;
					}
					app_splicer_mode_config(0,&gst_splicer_app_dev[0]);
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] KEY_NUM_8.\r\n");
			break;
		case KEY_NUM_9:
			switch (g_u8_display_mode[0])
			{
				case MS1826_VIDEO_SPLITTER:
					g_st_app_splitter_dev[0].b_splitter_sw = MS_TRUE;
					g_st_app_splitter_dev[0].u8_splitter_mode ++;
					if(g_st_app_splitter_dev[0].u8_splitter_mode > MODE_FOUR_EQUALIZE || g_st_app_splitter_dev[0].u8_splitter_mode < MODE_FOUR_MAIN1_SUB234)
						g_st_app_splitter_dev[0].u8_splitter_mode = MODE_FOUR_MAIN1_SUB234;
					app_splitter_mode_config(0,&g_st_app_splitter_dev[0]);
					break;
				case MS1826_VIDEO_SPLICER:
					if(gst_splicer_app_dev[0].u8_splicer_mode != SPLICER_MODE_2X2)
					{
						gst_splicer_app_dev[0].b_splicer_sw = MS_TRUE;
						gst_splicer_app_dev[0].u8_tx_mirror = MS_FALSE;
						gst_splicer_app_dev[0].u8_splicer_mode = SPLICER_MODE_2X2;
						app_splicer_mode_config(0,&gst_splicer_app_dev[0]);
					}
					break;
				case MS1826_SEAMLESS_MATRIX_SWITCHER:
                case MS1826_SEAMLESS_MATRIX_SWITCHER_OUT:
					break;
				case MS1826_BYPASS_MATRIX_SWITCHER:
					break;
				default:
					break;
			}
			LOG("[INFO] KEY_NUM_9.\r\n");
			break;
		default:
			break;
	}
	if(b_power)
	{
		if(g_st_hdmi_rx[0][0].u8_rx_state == RX_STATE_RUNNING)
			_video_mute(0,0, FALSE);
		else
			_video_mute(0,0, TRUE);
		if(g_st_hdmi_rx[0][1].u8_rx_state == RX_STATE_RUNNING)
			_video_mute(0,1, FALSE);
		else
			_video_mute(0,1, TRUE);
		if(g_st_hdmi_rx[0][2].u8_rx_state == RX_STATE_RUNNING)
			_video_mute(0,2, FALSE);
		else
			_video_mute(0,2, TRUE);
		if(g_st_hdmi_rx[0][3].u8_rx_state == RX_STATE_RUNNING)
			_video_mute(0,3, FALSE);
		else
			_video_mute(0,3, TRUE);
	}
	mculib_user_save();
}

VOID ir_key_service(VOID)
{
	UINT8 u8_ir_remap = 0;
	UINT8 u8_key = mculib_ir_detect();
	if (u8_key == IR_KEY_OTHER)
	{
		return;
	}
	u8_ir_remap = _remap_ir_key(u8_key);
	LOG("Ir_u8_key = %d\r\n", u8_key);
	_key_process_0(u8_ir_remap);

}

VOID key_service(VOID)
{
	UINT8 u8key = mculib_key_detect();
	if (u8key != KEY_NONE)
	{
		LOG("[KEY] u8key = %d\r\n", u8key);
		_key_process_0(u8key);
	}
}
