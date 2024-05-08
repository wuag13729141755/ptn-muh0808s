#include "includes.h"
#if (_ENABLE_CHIP_MS1826 == _MACRO_ENABLE)
#include "ms1826_videoprocess.h"
#include "ms1826_app_menuprocess.h"


extern UINT8 g_u8_splitter_audio_src[DEV_NUM_MS1826];
//APP_OSD_DRAWRECT_T g_st_osd_DrawRect;
BOOL b_osd_menu = false;
UINT8 const g_u8_bcsh_bri[4] = {128,128,128,128};
UINT8 const g_u8_bcsh_con[4] = {128,128,128,128};
UINT8 const g_u8_bcsh_sat[4] = {128,128,128,128};
UINT8 const g_u8_bcsh_hue[4] = {0,0,0,0};
UINT8 g_u8_level[2] = {0,0};
UINT8 g_u8_app_audio_output[3][4] = {{0,1,2,3},{0,1,2,3},{0,1,2,3}};
UINT8 g_u8_time = 0;
UINT8 g_u8_ratio = 100;
UINT8 g_u8_audio_sw = 0;


VOID app_osd_show_fonts(APP_OSD_SHOWFONTS_T *pst_osd_showfonts)
{
    OSD_DEV_T pst_osd_dev;
    OSD_FONT_T pst_osd_font;

    pst_osd_dev.u8_osd_idx = pst_osd_showfonts->u8_osd_idx;

    pst_osd_font.u8_x = pst_osd_showfonts->u8_x;
    pst_osd_font.u8_y = pst_osd_showfonts->u8_y;
    pst_osd_font.u16_length = pst_osd_showfonts->u8_len;//sizeof(pst_osd_showfonts->u16_idx)/sizeof(pst_osd_showfonts->u16_idx[0]);
    pst_osd_font.pu16_idx = (UINT16 *)(pst_osd_showfonts->u16_idx);

    api_osd_font_colortab_set(&pst_osd_dev,pst_osd_showfonts->osd_font_colortab);
    api_osd_show_index(&pst_osd_dev,&pst_osd_font);
}

//画框
VOID app_osd_DrawRect(APP_OSD_DRAWRECT_T * pst_osd_DrawRect,BOOL b_osd_en)
{
    OSD_DEV_T st_osd_dev;
    OSD_WINATTR_T st_osd_winattr;
    OSD_WINSIZE_T st_osd_winsize;
    OSD_TRANS_T st_osd_trans;
    OSD_SCALE_T  st_osd_scale;

    st_osd_dev.u8_osd_idx = pst_osd_DrawRect->u8_osd_idx;
    st_osd_winsize.u8_osd_win = pst_osd_DrawRect->u8_osd_win;
    st_osd_winsize.u16_w = pst_osd_DrawRect->u16_w;
    st_osd_winsize.u16_h = pst_osd_DrawRect->u16_h;
    st_osd_winsize.u16_x = pst_osd_DrawRect->u16_x;
    st_osd_winsize.u16_y = pst_osd_DrawRect->u16_y;
    api_osd_window_position_set(&st_osd_dev,&st_osd_winsize);

    st_osd_winattr.u8_osd_win = pst_osd_DrawRect->u8_osd_win;
    st_osd_winattr.u8_rounded_en = MS_TRUE;
    st_osd_winattr.u8_bd_en = false;
    st_osd_winattr.u8_rounded_radius = 5;
    st_osd_winattr.st_win_fillcolor.b_fill_color_en = MS_TRUE;
    st_osd_winattr.st_win_fillcolor.u8_win_id = pst_osd_DrawRect->u8_osd_win;
    st_osd_winattr.st_win_fillcolor.st_fill_start_color.u8_R = pst_osd_DrawRect->st_fill_start_color.u8_R;
    st_osd_winattr.st_win_fillcolor.st_fill_start_color.u8_G = pst_osd_DrawRect->st_fill_start_color.u8_G;
    st_osd_winattr.st_win_fillcolor.st_fill_start_color.u8_B = pst_osd_DrawRect->st_fill_start_color.u8_B;
    st_osd_winattr.st_win_fillcolor.u8_fill_color_type = OSD_FILLCOLOR_TYPE0;
    st_osd_winattr.u8_attr_rp_en = false;
    api_osd_window_attr_set(&st_osd_dev, &st_osd_winattr);

    st_osd_trans.u8_alpha_ratio = g_u8_ratio*2 + 55;
    st_osd_trans.u8_trans_mode = 0;
    st_osd_trans.b_trans_en = MS_TRUE;
    api_osd_trans_set(&st_osd_dev,&st_osd_trans);

    st_osd_scale.u16_h_ratio = 200;
    st_osd_scale.u16_v_ratio = 200;
    api_osd_font_scale(&st_osd_dev,&st_osd_scale);

    api_osd_colorspace_set(&st_osd_dev,OSD_COLORSPACE_RGB);
    api_osd_window_enable(&st_osd_dev,pst_osd_DrawRect->u8_osd_win,b_osd_en);
}


UINT16 STR_OsdMenu_CN[][7] =
{
    0,105,106,15 ,107,108,109,//亮度/对比度
    0,0  ,101,102,103,104,  0  ,//音频选择
    0,0  ,110,111,112,113,  0  ,//系统设置
    0,0  ,151,0  ,0   ,152, 0  ,//信息
};

UINT16 STR_MODE_CN[][4] =
{
    155,156,153,154,//分割模式
    157,158,153,154,//拼接模式
    159,160,163,164,//无缝矩阵
    161,162,163,164,//直通矩阵
};



UINT16 STR_clear[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
UINT16 STR_Bri_CN[3] = {105,0,106}; //亮度
UINT16 STR_Con_CN[3] = {107,108,109};//对比度
UINT16 STR_audio_input_CN[3][12] = {0,40,36,45,41,96,98,0,0,150,0,149,
                                    0,41,18,51,96,98,0,0,0,150,0,149,
                                    0,51,48,36,41,38,96,98,0,150,0,149,};


UINT16 STR_hdmi_input_CN[12][12] = {0,40,36,45,41,0,41,46,17,0,149,0,
                                    0,40,36,45,41,0,41,46,18,0,149,0,
                                    0,40,36,45,41,0,41,46,19,0,149,0,
                                    0,40,36,45,41,0,41,46,20,0,149,0,
                                    0,41,18,51,0,41,46,17,0,0,149,0,
                                    0,41,18,51,0,41,46,18,0,0,149,0,
                                    0,41,18,51,0,41,46,19,0,0,149,0,
                                    0,41,18,51,0,41,46,20,0,0,149,0,
                                    51,48,36,41,38,0,41,46,17,0,149,0,
                                    51,48,36,41,38,0,41,46,18,0,149,0,
                                    51,48,36,41,38,0,41,46,19,0,149,0,
                                    51,48,36,41,38,0,41,46,20,0,149,0,};//音频输入





UINT16 STR_audiooutput_CN[12][10] ={40,36,45,41,0,47,53,52,17,0,
                                    40,36,45,41,0,47,53,52,18,0,
                                    40,36,45,41,0,47,53,52,19,0,
                                    40,36,45,41,0,47,53,52,20,0,//hdmi
                                    41,18,51,0,47,53,52,17,0,0,
                                    41,18,51,0,47,53,52,18,0,0,
                                    41,18,51,0,47,53,52,19,0,0,
                                    41,18,51,0,47,53,52,20,0,0,//i2s
                                    51,48,36,41,38,0,47,53,52,17,
                                    51,48,36,41,38,0,47,53,52,18,
                                    51,48,36,41,38,0,47,53,52,19,
                                    51,48,36,41,38,0,47,53,52,20,};//spdif

UINT16 STR_Dormancy_CN[4] = {138,139,140,141};//休眠时间
UINT16 STR_Transparency_CN[3] = {142,143,144};//透明度
UINT16 STR_init_CN[3] = {145,146,147};//初始化
UINT16 STR_time_CN[3][4] ={ 0, 0,21,83,
                            0,17,21,83,
                            0,19,16,83,};//


UINT16 STR_Progress[8] =
{
    114,116,117,118,119,120,121,132,
};


VOID STR_Progress_CFG(UINT8 u8_progress)
{
    UINT8 u8_grade,idx;
    u8_grade = u8_progress/16 + 1;
    for(idx = 1 ; idx < 7 ; idx ++)
    {
        if(idx < u8_grade)
            STR_Progress[idx] = 115;
        else if(idx > u8_grade)
            STR_Progress[idx] = 131;
        else
            STR_Progress[idx] = 131 - (u8_progress%16);
    }
    if(u8_progress > 96)
        STR_Progress[7] = 233 - u8_progress;
    else
        STR_Progress[7] = 137;

}

//VOID app_osd_fonts_set(UINT8 u8_x,UINT8 u8_y,UINT16 * p_osd_fonts)
//{
//  APP_OSD_SHOWFONTS_T st_osd_showfonts;
//  st_osd_showfonts.u8_x = 15;
//  st_osd_showfonts.u8_y = 1;
//  st_osd_showfonts.u16_idx = p_osd_fonts;
//  st_osd_showfonts.u8_len = sizeof(p_osd_fonts)/sizeof(p_osd_fonts[0]);
//  app_osd_show_fonts(&st_osd_showfonts);
//}

VOID app_osd_fonts_clear(VOID)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;
    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_clear;
    st_osd_showfonts.u8_len = sizeof(STR_clear)/sizeof(STR_clear[0]);
    app_osd_show_fonts(&st_osd_showfonts);
    st_osd_showfonts.u8_y = 3;
    app_osd_show_fonts(&st_osd_showfonts);
    st_osd_showfonts.u8_y = 5;
    app_osd_show_fonts(&st_osd_showfonts);
    st_osd_showfonts.u8_y = 7;
    app_osd_show_fonts(&st_osd_showfonts);
    st_osd_showfonts.u8_y = 9;
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_system_time_cfg(UINT8 u8_time)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;
    st_osd_showfonts.u8_x = 15;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_time_CN[u8_time];
    st_osd_showfonts.u8_len = sizeof(STR_time_CN[u8_time])/sizeof(STR_time_CN[u8_time][0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_system_Progress_cfg(UINT8 u8_ratio)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;
    STR_Progress_CFG(100 - u8_ratio);
    st_osd_showfonts.u8_x = 15;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_Progress;
    st_osd_showfonts.u8_len = sizeof(STR_Progress)/sizeof(STR_Progress[0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_system_cfg(VOID)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_Dormancy_CN;
    st_osd_showfonts.u8_len = sizeof(STR_Dormancy_CN)/sizeof(STR_Dormancy_CN[0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_Transparency_CN;
    st_osd_showfonts.u8_len = sizeof(STR_Transparency_CN)/sizeof(STR_Transparency_CN[0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 5;
    st_osd_showfonts.u16_idx = STR_init_CN;
    st_osd_showfonts.u8_len = sizeof(STR_init_CN)/sizeof(STR_init_CN[0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_audio_in_cfg(UINT8 u8_y,UINT8 u8_audio_output)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = u8_y;
    st_osd_showfonts.u16_idx = STR_hdmi_input_CN[u8_audio_output];
    st_osd_showfonts.u8_len = sizeof(STR_hdmi_input_CN[u8_audio_output])/sizeof(STR_hdmi_input_CN[u8_audio_output][0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_hdmi_out_cfg(VOID)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[0];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[4])/sizeof(STR_audiooutput_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 5;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[1];//STR_Audio_input_CN[1];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[5])/sizeof(STR_audiooutput_CN[1][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 7;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[2];//STR_Audio_input_CN[2];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[6])/sizeof(STR_audiooutput_CN[2][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 9;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[3];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[7])/sizeof(STR_audiooutput_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_i2s_out_cfg(VOID)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[4];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[4])/sizeof(STR_audiooutput_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 5;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[5];//STR_Audio_input_CN[1];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[5])/sizeof(STR_audiooutput_CN[1][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 7;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[6];//STR_Audio_input_CN[2];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[6])/sizeof(STR_audiooutput_CN[2][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 9;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[7];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[7])/sizeof(STR_audiooutput_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_spdif_out_cfg(VOID)
{

    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[8];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[4])/sizeof(STR_audiooutput_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 5;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[9];//STR_Audio_input_CN[1];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[5])/sizeof(STR_audiooutput_CN[1][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 7;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[10];//STR_Audio_input_CN[2];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[6])/sizeof(STR_audiooutput_CN[2][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 22;
    st_osd_showfonts.u8_y = 9;
    st_osd_showfonts.u16_idx = STR_audiooutput_CN[11];
    st_osd_showfonts.u8_len = sizeof(STR_audiooutput_CN[7])/sizeof(STR_audiooutput_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_audio_sw_cfg(UINT8 u8_sw)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_audio_input_CN[u8_sw];
    st_osd_showfonts.u8_len = sizeof(STR_audio_input_CN[u8_sw])/sizeof(STR_audio_input_CN[u8_sw][0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_bri_con_cfg(UINT8 u8_bcsh_bri,UINT8 u8_bcsh_con)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    STR_Progress_CFG(u8_bcsh_bri);
    st_osd_showfonts.u8_x = 13;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_Progress;
    st_osd_showfonts.u8_len = sizeof(STR_Progress)/sizeof(STR_Progress[0]);
    app_osd_show_fonts(&st_osd_showfonts);

    STR_Progress_CFG(u8_bcsh_con);
    st_osd_showfonts.u8_x = 13;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_Progress;
    st_osd_showfonts.u8_len = sizeof(STR_Progress)/sizeof(STR_Progress[0]);
    app_osd_show_fonts(&st_osd_showfonts);
}

VOID app_osd_bcsh_cfg(VOID)
{
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_Bri_CN;
    st_osd_showfonts.u8_len = sizeof(STR_Bri_CN)/sizeof(STR_Bri_CN[0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 10;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_Con_CN;
    st_osd_showfonts.u8_len = sizeof(STR_Con_CN)/sizeof(STR_Con_CN[0]);
    app_osd_show_fonts(&st_osd_showfonts);
}


VOID app_osd_info_cfg(UINT8 u8dev)
{
    OSD_DEV_T st_osd_dev;
    OSD_STRING_T st_osd_string;
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_dev.u8_osd_idx = OSD_1;

    st_osd_showfonts.u8_x = 18;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_MODE_CN[display_mode[u8dev]];
    st_osd_showfonts.u8_len = sizeof(STR_MODE_CN[display_mode[u8dev]])/sizeof(STR_MODE_CN[display_mode[u8dev]][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_string.pstr = MS1826_4X4DEMO_SW_VERSION;
    st_osd_string.u8_x = 10;
    st_osd_string.u8_y = 3;
    api_osd_show_string(&st_osd_dev,&st_osd_string);
}


VOID app_osd_cfg(UINT8 u8dev)
{
    OSD_DEV_T st_osd_dev;
    OSD_WINSIZE_T st_osd_win_size;
    APP_OSD_DRAWRECT_T st_osd_drawrect;
    APP_OSD_SHOWFONTS_T st_osd_showfonts;
    st_osd_dev.u8_osd_idx = OSD_1;
    api_osd_clear(&st_osd_dev);

    api_osd_window_enable(&g_st_osd_dev[u8dev][1], OSD_WIN_DISPLAY, false);
    api_osd_trigger(&g_st_osd_dev[u8dev][1]);

    api_osd_window_enable(&g_st_osd_dev[u8dev][2], OSD_WIN_DISPLAY, false);
    api_osd_trigger(&g_st_osd_dev[u8dev][2]);

    api_osd_window_enable(&g_st_osd_dev[u8dev][3], OSD_WIN_DISPLAY, false);
    api_osd_trigger(&g_st_osd_dev[u8dev][3]);

    g_st_misc_osd_mux[u8dev][0].b_enable = MS_TRUE;
    g_st_misc_osd_mux[u8dev][0].u8_osd_module = MISC_VIDEO_MODULE_OSD1;
    g_st_misc_osd_mux[u8dev][0].u8_tx_module = MISC_VIDEO_MODULE_TX1;
    api_misc_osd_timing_mux_cfg(&g_st_misc_osd_mux[u8dev][0]);

    st_osd_showfonts.u8_osd_idx = OSD_1;
    st_osd_showfonts.osd_font_colortab = OSD_PATTERN_COLOR2;

    st_osd_win_size.u8_osd_win = OSD_WIN_DISPLAY;
    st_osd_win_size.u16_w = 35;
    st_osd_win_size.u16_h = 15;
    st_osd_win_size.u16_x = (g_stVideo_HDMITx_Timing[u8dev][0].u16_hactive - 35 * 16) / 2;//10;
    st_osd_win_size.u16_y = (g_stVideo_HDMITx_Timing[u8dev][0].u16_vactive - 15 * 18) / 2;//10;
    api_osd_window_position_set(&g_st_osd_dev[u8dev][0], &st_osd_win_size);

    st_osd_drawrect.u8_osd_idx = OSD_1;
    st_osd_drawrect.u8_osd_win = OSD_WIN_HIGHLIGHT1;
    st_osd_drawrect.st_fill_start_color.u8_R = 0xff;
    st_osd_drawrect.st_fill_start_color.u8_G = 0xff;
    st_osd_drawrect.st_fill_start_color.u8_B = 0xff;
    st_osd_drawrect.u16_x = 1;
    st_osd_drawrect.u16_y = 1;
    st_osd_drawrect.u16_w = 16 * 35;
    st_osd_drawrect.u16_h = 18 * 15;
    app_osd_DrawRect(&st_osd_drawrect,MS_TRUE); //背景框

    st_osd_showfonts.u8_x = 1;
    st_osd_showfonts.u8_y = 1;
    st_osd_showfonts.u16_idx = STR_OsdMenu_CN[0];
    st_osd_showfonts.u8_len = sizeof(STR_OsdMenu_CN[0])/sizeof(STR_OsdMenu_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 1;
    st_osd_showfonts.u8_y = 3;
    st_osd_showfonts.u16_idx = STR_OsdMenu_CN[1];
    st_osd_showfonts.u8_len = sizeof(STR_OsdMenu_CN[0])/sizeof(STR_OsdMenu_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 1;
    st_osd_showfonts.u8_y = 5;
    st_osd_showfonts.u16_idx = STR_OsdMenu_CN[2];
    st_osd_showfonts.u8_len = sizeof(STR_OsdMenu_CN[0])/sizeof(STR_OsdMenu_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

    st_osd_showfonts.u8_x = 1;
    st_osd_showfonts.u8_y = 7;
    st_osd_showfonts.u16_idx = STR_OsdMenu_CN[3];
    st_osd_showfonts.u8_len = sizeof(STR_OsdMenu_CN[0])/sizeof(STR_OsdMenu_CN[0][0]);
    app_osd_show_fonts(&st_osd_showfonts);

//  app_osd_system_cfg();

}

VOID app_osd_drawrect_win(UINT8 u8_x,UINT8 u8_y,UINT8 u8_len)
{
    APP_OSD_DRAWRECT_T st_osd_drawrect;
    st_osd_drawrect.u8_osd_idx = OSD_1;
    st_osd_drawrect.u8_osd_win = OSD_WIN_HIGHLIGHT2;
    st_osd_drawrect.st_fill_start_color.u8_R = 0x00;
    st_osd_drawrect.st_fill_start_color.u8_G = 0xb9;
    st_osd_drawrect.st_fill_start_color.u8_B = 0xff;
    st_osd_drawrect.u16_x = 16 * u8_x;
    st_osd_drawrect.u16_y = 9 + 18 * u8_y;
    st_osd_drawrect.u16_w = 16 * u8_len;
    st_osd_drawrect.u16_h = 18 * 2;
    app_osd_DrawRect(&st_osd_drawrect,MS_TRUE); //背景框
}

VOID app_osd_menu_drawrect(UINT8 u8dev,UINT8 u8_level,UINT8 u8_y)
{
    api_misc_osd_timing_mux_cfg(&g_st_misc_osd_mux[u8dev][0]);
    if(u8_level == 0)
    {
        g_u8_level[0] = u8_y;
        app_osd_drawrect_win(1,u8_y * 2,8);
        app_osd_fonts_clear();
        if(u8_y == 0)
        {
            app_osd_bcsh_cfg();
//          app_osd_bri_con_cfg(g_u8_bcsh_bri,g_u8_bcsh_con);
        }
        else if(u8_y == 1)
        {
            app_osd_audio_sw_cfg(g_u8_audio_sw);
            if(g_u8_audio_sw == 0)
                app_osd_hdmi_out_cfg();
            else if(g_u8_audio_sw == 1)
                app_osd_i2s_out_cfg();
            else if(g_u8_audio_sw == 2)
                app_osd_spdif_out_cfg();
            app_osd_audio_in_cfg(3,g_u8_app_audio_output[g_u8_audio_sw][0]);
            app_osd_audio_in_cfg(5,g_u8_app_audio_output[g_u8_audio_sw][1]);
            app_osd_audio_in_cfg(7,g_u8_app_audio_output[g_u8_audio_sw][2]);
            app_osd_audio_in_cfg(9,g_u8_app_audio_output[g_u8_audio_sw][3]);
        }
        else if(u8_y == 2)
        {
            app_osd_system_time_cfg(g_u8_time);
            app_osd_system_Progress_cfg(g_u8_ratio);
            app_osd_system_cfg();
        }
        else if(u8_y == 3)
        {
            app_osd_info_cfg(u8dev);
        }
    }
    else if(u8_level == 1)
    {
        if(g_u8_level[0] == 0)
        {
            app_osd_drawrect_win(9 , u8_y * 2  , 15);
//          app_osd_bri_con_cfg(g_u8_bcsh_bri,g_u8_bcsh_con);
        }
        else if(g_u8_level[0] == 1)
        {
            app_osd_drawrect_win(9 , u8_y * 2  , 24);
            app_osd_audio_sw_cfg(g_u8_audio_sw);
            if(g_u8_audio_sw == 0)
                app_osd_hdmi_out_cfg();
            else if(g_u8_audio_sw == 1)
                app_osd_i2s_out_cfg();
            else if(g_u8_audio_sw == 2)
                app_osd_spdif_out_cfg();
            app_osd_audio_in_cfg(3,g_u8_app_audio_output[g_u8_audio_sw][0]);
            app_osd_audio_in_cfg(5,g_u8_app_audio_output[g_u8_audio_sw][1]);
            app_osd_audio_in_cfg(7,g_u8_app_audio_output[g_u8_audio_sw][2]);
            app_osd_audio_in_cfg(9,g_u8_app_audio_output[g_u8_audio_sw][3]);
        }
        else if(g_u8_level[0] == 2)
        {
            app_osd_drawrect_win(9 , u8_y * 2  , 15);
            app_osd_system_time_cfg(g_u8_time);
            app_osd_system_Progress_cfg(g_u8_ratio);
        }
    }





}
#endif


