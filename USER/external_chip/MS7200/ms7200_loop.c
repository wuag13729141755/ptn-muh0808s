#include "includes.h"
#if _ENABLE_CHIP_MS7200
#include "MS7200/inc/ms7200.h"
#include "MS7200/inc/ms7200_loop.h"

extern u8 const AllPrjDefEdidTabl[256];

static UINT16 g_u16_tmds_clk[DEV_NUM_MS7200] = {0};
static BOOL g_b_rxphy_status[DEV_NUM_MS7200] = {MS_TRUE};
static BOOL g_b_input_valid[DEV_NUM_MS7200] = {MS_FALSE};
static UINT8 g_u8_rx_stable_timer_count[DEV_NUM_MS7200] = {0};
#define RX_STABLE_TIMEOUT (2)
static VIDEOTIMING_T g_t_hdmirx_timing[DEV_NUM_MS7200];
static HDMI_CONFIG_T g_t_hdmirx_infoframe[DEV_NUM_MS7200];
//static DVOUT_CONFIG_T g_t_dvout_config = { DVOUT_CS_MODE_RGB, DVOUT_BW_MODE_16_24BIT, DVOUT_DR_MODE_SDR, DVOUT_SY_MODE_HSVSDE };
static DVOUT_CONFIG_T g_t_dvout_config = { MS7200_CS_MODE, MS7200_BW_MODE, MS7200_DR_MODE, MS7200_SY_MODE };
u8 g_u8MS7200NumIndex = 0;

const stMS7200Config_T cg_stMs7200DevCfgTable[] =
{
    {   _I2C_TABLE_INDEX_MS7200_0,
        MS7200_DEV_ADDR,
        MS7200_CS_MODE,
        MS7200_BW_MODE,
        MS7200_DR_MODE,
        MS7200_SY_MODE},
#if (DEV_NUM_MS7200>1)
    {   _I2C_TABLE_INDEX_MS7200_1,
        MS7200_DEV_ADDR_1,
        MS7200_CS_MODE_1,
        MS7200_BW_MODE_1,
        MS7200_DR_MODE_1,
        MS7200_SY_MODE_1},
#endif
#if (DEV_NUM_MS7200>2)
    {   _I2C_TABLE_INDEX_MS7200_2,
        MS7200_DEV_ADDR_2,
        MS7200_CS_MODE_2,
        MS7200_BW_MODE_2,
        MS7200_DR_MODE_2,
        MS7200_SY_MODE_2},
#endif
#if (DEV_NUM_MS7200>3)
    {   _I2C_TABLE_INDEX_MS7200_3,
        MS7200_DEV_ADDR_3,
        MS7200_CS_MODE_3,
        MS7200_BW_MODE_3,
        MS7200_DR_MODE_3,
        MS7200_SY_MODE_3},
#endif
#if (DEV_NUM_MS7200>4)
    {   _I2C_TABLE_INDEX_MS7200_4,
        MS7200_DEV_ADDR_4,
        MS7200_CS_MODE_4,
        MS7200_BW_MODE_4,
        MS7200_DR_MODE_4,
        MS7200_SY_MODE_4},
#endif
#if (DEV_NUM_MS7200>5)
    {   _I2C_TABLE_INDEX_MS7200_5,
        MS7200_DEV_ADDR_5,
        MS7200_CS_MODE_5,
        MS7200_BW_MODE_5,
        MS7200_DR_MODE_5,
        MS7200_SY_MODE_5},
#endif
};

static void ms7200_var_init(u8 u8DevId)
{
    g_t_dvout_config.u8_cs_mode = cg_stMs7200DevCfgTable[u8DevId].u8CSMode;
    g_t_dvout_config.u8_bw_mode = cg_stMs7200DevCfgTable[u8DevId].u8BWMode;
    g_t_dvout_config.u8_dr_mode = cg_stMs7200DevCfgTable[u8DevId].u8DRMode;
    g_t_dvout_config.u8_sy_mode = cg_stMs7200DevCfgTable[u8DevId].u8SYMode;

    g_u16_tmds_clk[u8DevId] = 0;
    g_b_rxphy_status[u8DevId] = MS_TRUE;
    g_b_input_valid[u8DevId] = MS_FALSE;
    g_u8_rx_stable_timer_count[u8DevId] = 0;
    memset(&g_t_hdmirx_timing[u8DevId],0,sizeof(VIDEOTIMING_T));
    memset(&g_t_hdmirx_infoframe[u8DevId],0,sizeof(HDMI_CONFIG_T));
}

void sys_default_hdmi_video_config(u8 u8DevId)
{
    //g_t_hdmi_infoframe[u8DevId].u8_hdmi_flag = MS_TRUE;
    g_t_hdmirx_infoframe[u8DevId].u8_vic = 0;
    //g_t_hdmi_infoframe[u8DevId].u16_video_clk = 7425;
    g_t_hdmirx_infoframe[u8DevId].u8_clk_rpt = HDMI_X1CLK;
    g_t_hdmirx_infoframe[u8DevId].u8_scan_info = HDMI_OVERSCAN;
    g_t_hdmirx_infoframe[u8DevId].u8_aspect_ratio = HDMI_16X9;
    g_t_hdmirx_infoframe[u8DevId].u8_color_space = HDMI_RGB;
    g_t_hdmirx_infoframe[u8DevId].u8_color_depth = HDMI_COLOR_DEPTH_8BIT;
    g_t_hdmirx_infoframe[u8DevId].u8_colorimetry = HDMI_COLORIMETRY_709;
}

void sys_default_hdmi_vendor_specific_config(u8 u8DevId)
{
    g_t_hdmirx_infoframe[u8DevId].u8_video_format = HDMI_NO_ADD_FORMAT;
    g_t_hdmirx_infoframe[u8DevId].u8_4Kx2K_vic = HDMI_4Kx2K_30HZ;
    g_t_hdmirx_infoframe[u8DevId].u8_3D_structure = HDMI_FRAME_PACKING;
}

void sys_default_hdmi_audio_config(u8 u8DevId)
{
    g_t_hdmirx_infoframe[u8DevId].u8_audio_mode = HDMI_AUD_MODE_AUDIO_SAMPLE;
    g_t_hdmirx_infoframe[u8DevId].u8_audio_rate = HDMI_AUD_RATE_48K;
    g_t_hdmirx_infoframe[u8DevId].u8_audio_bits = HDMI_AUD_LENGTH_16BITS;
    g_t_hdmirx_infoframe[u8DevId].u8_audio_channels = HDMI_AUD_2CH;
    g_t_hdmirx_infoframe[u8DevId].u8_audio_speaker_locations = 0;
}

VOID ms7200_media_service(u8 u8DevId)
{
    UINT8 u8_rxphy_status = 0;
    UINT32 u32_int_status = 0;
    HDMI_CONFIG_T t_hdmi_infoframe;

    //detect 5v to pull up/down hpd
    ms7200_hdmirx_hpd_config(ms7200_hdmirx_source_connect_detect(), NULL);

    //reconfig rxphy when input clk change
    u32_int_status = ms7200_hdmirx_interrupt_get_and_clear(RX_INT_INDEX_HDMI, FREQ_LOCK_ISTS | FREQ_UNLOCK_ISTS | CLK_CHANGE_ISTS, MS_TRUE);
    if (u32_int_status)
    {
        if (u32_int_status & CLK_CHANGE_ISTS)
        {
            if (abs(ms7200_hdmirx_input_clk_get() - g_u16_tmds_clk[u8DevId]) > 1000)
            {
                g_b_rxphy_status[u8DevId] = MS_FALSE;
            }
        }
        else if ((u32_int_status & (FREQ_LOCK_ISTS | FREQ_UNLOCK_ISTS)) == FREQ_LOCK_ISTS)
        {
            g_b_rxphy_status[u8DevId] = MS_TRUE;
        }
        if (!g_b_rxphy_status[u8DevId])
        {
            ms7200_dvout_audio_config(MS_FALSE);
            ms7200_dvout_video_config(MS_FALSE);
            u8_rxphy_status = ms7200_hdmirx_rxphy_config(&g_u16_tmds_clk[u8DevId]);
//            g_b_rxphy_status = u8_rxphy_status ? (u8_rxphy_status & 0x01) : MS_TRUE;
            g_b_rxphy_status[u8DevId] = u8_rxphy_status ? ((u8_rxphy_status & 0x01)?MS_TRUE:MS_FALSE) : MS_TRUE;
            g_b_input_valid[u8DevId] = MS_FALSE;
            g_u8_rx_stable_timer_count[u8DevId] = 0;
            MS7200_LOG2("g_u16_tmds_clk = ", g_u16_tmds_clk[u8DevId]);
            MS7200_LOG2("g_b_rxphy_status = ", g_b_rxphy_status[u8DevId]);
        }
    }
    if (g_u16_tmds_clk[u8DevId] < 500)
    {
        return;
    }

    //reset pdec when mdt change
    u32_int_status = ms7200_hdmirx_interrupt_get_and_clear(RX_INT_INDEX_MDT, MDT_STB_ISTS | MDT_USTB_ISTS, MS_TRUE);
    if (u32_int_status)
    {
        if (!(g_b_input_valid[u8DevId] && (u32_int_status == MDT_STB_ISTS)))
        {
            ms7200_dvout_audio_config(MS_FALSE);
            ms7200_dvout_video_config(MS_FALSE);
            ms7200_hdmirx_audio_config(MS_TRUE, g_u16_tmds_clk[u8DevId]);
            ms7200_hdmirx_core_reset(HDMI_RX_CTRL_PDEC | HDMI_RX_CTRL_AUD);
            ms7200_hdmirx_interrupt_get_and_clear(RX_INT_INDEX_PDEC, PDEC_ALL_ISTS, MS_TRUE);
            g_u8_rx_stable_timer_count[u8DevId] = 0;
            if (g_b_input_valid[u8DevId])
            {
                g_b_input_valid[u8DevId] = MS_FALSE;
                MS7200_LOG("timing unstable");
            }
        }
    }

    //get input timing status when mdt stable
    if (!g_b_input_valid[u8DevId])
    {
        if (g_u8_rx_stable_timer_count[u8DevId] < RX_STABLE_TIMEOUT)
        {
            g_u8_rx_stable_timer_count[u8DevId]++;
            return;
        }
        g_b_input_valid[u8DevId] = ms7200_hdmirx_input_timing_get(&g_t_hdmirx_timing[u8DevId]);
        MS7200_LOG2("u8_polarity    = ",g_t_hdmirx_timing[u8DevId].u8_polarity);
        MS7200_LOG2("u16_htotal     = ",g_t_hdmirx_timing[u8DevId].u16_htotal);
        MS7200_LOG2("u16_vtotal     = ",g_t_hdmirx_timing[u8DevId].u16_vtotal);
        MS7200_LOG2("u16_hactive    = ",g_t_hdmirx_timing[u8DevId].u16_hactive);
        MS7200_LOG2("u16_vactive    = ",g_t_hdmirx_timing[u8DevId].u16_vactive);
        MS7200_LOG2("u16_pixclk     = ",g_t_hdmirx_timing[u8DevId].u16_pixclk);
        MS7200_LOG2("u16_vfreq      = ",g_t_hdmirx_timing[u8DevId].u16_vfreq);
        MS7200_LOG2("u16_hoffset    = ",g_t_hdmirx_timing[u8DevId].u16_hoffset);
        MS7200_LOG2("u16_voffset    = ",g_t_hdmirx_timing[u8DevId].u16_voffset);
        MS7200_LOG2("u16_hsyncwidth = ",g_t_hdmirx_timing[u8DevId].u16_hsyncwidth);
        MS7200_LOG2("u16_vsyncwidth = ",g_t_hdmirx_timing[u8DevId].u16_vsyncwidth);
        if (g_b_input_valid[u8DevId])
        {
            u32_int_status = ms7200_hdmirx_interrupt_get_and_clear(RX_INT_INDEX_PDEC, PDEC_ALL_ISTS, MS_TRUE);
            ms7200_hdmirx_input_infoframe_get(&g_t_hdmirx_infoframe[u8DevId]);
            g_t_hdmirx_infoframe[u8DevId].u16_video_clk = g_u16_tmds_clk[u8DevId];
            if (!(u32_int_status & AVI_RCV_ISTS))
            {
                sys_default_hdmi_video_config(u8DevId);
            }
            if (!(u32_int_status & AIF_RCV_ISTS))
            {
//                sys_default_hdmi_vendor_specific_config();
                sys_default_hdmi_audio_config(u8DevId);
            }
            if (!(u32_int_status & VSI_RCV_ISTS))
            {
                sys_default_hdmi_vendor_specific_config(u8DevId);
            }

            ms7200_hdmirx_video_config(&g_t_hdmirx_infoframe[u8DevId]);
            ms7200_hdmirx_audio_config(MS_TRUE, g_u16_tmds_clk[u8DevId]);
            ms7200_dvout_video_config(MS_TRUE);
            ms7200_dvout_audio_config(MS_TRUE);
            MS7200_LOG("timing stable");
        }
        else
        {
            MS7200_LOG("timing error");
        }
        return;
    }

    //reconfig system when input packet change
    u32_int_status = ms7200_hdmirx_interrupt_get_and_clear(RX_INT_INDEX_PDEC, PDEC_ALL_ISTS, MS_TRUE);
    if (u32_int_status & (AVI_CKS_CHG_ISTS | AIF_CKS_CHG_ISTS | VSI_CKS_CHG_ISTS))
    {
        ms7200_hdmirx_input_infoframe_get(&t_hdmi_infoframe);
        t_hdmi_infoframe.u16_video_clk = g_u16_tmds_clk[u8DevId];
        if (!(u32_int_status & AVI_RCV_ISTS))
        {
            sys_default_hdmi_video_config(u8DevId);
        }
        if (!(u32_int_status & AIF_RCV_ISTS))
        {
//            sys_default_hdmi_vendor_specific_config(u8DevId);
            sys_default_hdmi_audio_config(u8DevId);
        }
        if (!(u32_int_status & VSI_RCV_ISTS))
        {
            sys_default_hdmi_vendor_specific_config(u8DevId);
        }
        if (memcmp(&t_hdmi_infoframe, &g_t_hdmirx_infoframe[u8DevId], sizeof(HDMI_CONFIG_T)) != 0)
        {
            ms7200_dvout_audio_config(MS_FALSE);
            ms7200_dvout_video_config(MS_FALSE);
            ms7200_hdmirx_video_config(&t_hdmi_infoframe);
            ms7200_hdmirx_audio_config(MS_TRUE, g_u16_tmds_clk[u8DevId]);
            ms7200_dvout_video_config(MS_TRUE);
            ms7200_dvout_audio_config(MS_TRUE);
            g_t_hdmirx_infoframe[u8DevId] = t_hdmi_infoframe;
            MS7200_LOG("infoframe change");
        }
    }

    //reconfig audio when acr change or audio fifo error
    if (u32_int_status & (ACR_CTS_CHG_ISTS | ACR_N_CHG_ISTS))
    {
        ms7200_hdmirx_audio_config(MS_TRUE, g_u16_tmds_clk[u8DevId]);
        //MS7200_LOG("acr change");
    }
    else
    {
        u32_int_status = ms7200_hdmirx_audio_fifo_status_get();
        if (u32_int_status & AFIF_THS_PASS_STS)
        {
            if (u32_int_status & (AFIF_UNDERFL_STS | AFIF_OVERFL_STS))
            {
                ms7200_hdmirx_audio_config(MS_FALSE, g_u16_tmds_clk[u8DevId]);
                MS7200_LOG("audio fifo reset");
            }
        }
    }
}


void ms7200_init(void)
{
    u8 l_u8ChipConnectSta;
    ms7200_SystemReset();
    for(g_u8MS7200NumIndex=0;g_u8MS7200NumIndex<DEV_NUM_MS7200;g_u8MS7200NumIndex++)
    {
        g_u8MS7200_DeviceID = cg_stMs7200DevCfgTable[g_u8MS7200NumIndex].u8I2cIndex;
        ms7200_var_init(g_u8MS7200NumIndex);
        l_u8ChipConnectSta = ms7200_chip_connect_detect(cg_stMs7200DevCfgTable[g_u8MS7200NumIndex].u8DevAddr);
        MS7200_LOG2("ms7200 chip connect = ", l_u8ChipConnectSta);
//        MS7200_LOG(ms7200_sdk_version());

        ms7200_hdmirx_init();
        //ms7200_hdmirx_hdcp_init(u8_rx_ksv_buf, u8_rx_key_buf);
        ms7200_dvout_init(&g_t_dvout_config, MS7200_AUDIO_OUT_SPDIF);
        ms7200_dvout_clk_driver_set(MS7200_CLK_DRIVER);
        ms7200_dvout_phase_adjust(MS7200_CLK_INV,MS7200_PHASE_DELAY);
        ms7200_hdmirx_hpd_config(MS_FALSE, (UINT8*)AllPrjDefEdidTabl);
    }
}

void ms7200_loop(void)
{
    static u8 sl_u8LoopCnt=0;

    if(sl_u8LoopCnt<3)
    {
        sl_u8LoopCnt++;
        return;
    }
    sl_u8LoopCnt = 0;

    for(g_u8MS7200NumIndex=0;g_u8MS7200NumIndex<DEV_NUM_MS7200;g_u8MS7200NumIndex++)
    {
        g_u8MS7200_DeviceID = cg_stMs7200DevCfgTable[g_u8MS7200NumIndex].u8I2cIndex;
        ms7200_media_service(g_u8MS7200NumIndex);
    }
}

#endif

