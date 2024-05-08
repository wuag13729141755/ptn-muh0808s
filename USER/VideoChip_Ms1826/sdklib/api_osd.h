/**
* @file  api_osd.h  
* @brief OSD模块相关数据结构及函数定义                                                           
*/

/**
 * @defgroup API_OSD osd module api
 *
 * @brief 用于OSD显示功能
 * @{
 */

#ifndef __HDMI_OSD_API_H__
#define __HDMI_OSD_API_H__

#include "ms_typedef.h"

typedef enum _OSD_FONTTYPE_E
{
    OSD_1BPP_FONT   = 0,  /**<  osd 1bpp font type */
    OSD_2BPP_FONT   = 1,  /**<  osd 2bpp font type */
    OSD_4BPP_FONT   = 2,   /**<  osd 4bpp font type */

    OSD_BPP_BUTT,
}OSD_FONTTYPE_E;

/** OSD索引，共有四个OSD*/
typedef enum _OSD_INDEX_E 
{
    OSD_1 = 0,  /**<  OSD 1，表示第一个OSD */
    OSD_2 = 1,  /**<  OSD 2，表示第二个OSD */
    OSD_3 = 2,  /**<  OSD 3，表示第三个OSD */
    OSD_4 = 3,  /**<  OSD 4，表示第四个OSD */
    
    OSD_BUTT,
}OSD_INDEX_E;

/** 窗口类型，四个子窗口 */
typedef enum _OSD_WIN_E
{
    OSD_WIN_DISPLAY = 0,        /**<  显示子窗口 */
    OSD_WIN_HIGHLIGHT1 = 1,     /**<  HIGHLIGHT 1 窗口*/
    OSD_WIN_HIGHLIGHT2 = 2,     /**<  HIGHLIGHT 2 窗口*/
    OSD_WIN_HIGHLIGHT3 = 3,     /**<  HIGHLIGHT 3 窗口*/

    OSD_WIN_BUTT,
}OSD_WIN_E;

/** COLORKEY类型，支持两个ColorKey */
typedef enum _OSD_COLORKEY_E 
{
    OSD_COLORKEY_0 = 0,     /**< ColorKey 0 */
    OSD_COLORKEY_1 = 1,     /**< ColorKey 1 */
    
    OSD_COLORKEY_BUTT,
}OSD_COLORKEY_E;
    
/** window旋转角度*/
typedef enum _OSD_ROTATE_E
{
    OSD_ROTATE0    = 0, /**<  不旋转（默认） */
    OSD_ROTATE90   = 1, /**<  旋转90° */
    OSD_ROTATE180  = 2, /**<  旋转180° */
    OSD_ROTATE270  = 3, /**<  旋转270° */
    
    OSD_ROTATE_BUTT,
}OSD_ROTATE_E;

/** window色彩填充类型*/
typedef enum _OSD_FILLCOLORTYPE_E
{
    OSD_FILLCOLOR_TYPE0 = 0,    /**<  纯色填充（默认） */
    OSD_FILLCOLOR_TYPE1 = 1,    /**<  渐变色，从左往右 */
    OSD_FILLCOLOR_TYPE2 = 2,    /**<  渐变色，从上往下 */
    OSD_FILLCOLOR_TYPE3 = 3,    /**<  渐变色，从两边往中间 */
    OSD_FILLCOLOR_TYPE4 = 4,    /**<  渐变色，从上下往中间 */
    
    OSD_FILLCOLOR_BUTT,
}OSD_FILLCOLORTYPE_E;

/** Font颜色组列表*/
typedef enum _OSD_FONT_COLORTAB_E
{
    OSD_PATTERN_COLOR0   = 0,  /**<  颜色组列表0 */
    OSD_PATTERN_COLOR1   = 1,  /**<  颜色组列表1 */
    OSD_PATTERN_COLOR2   = 2,  /**<  颜色组列表2 */
    OSD_PATTERN_COLOR3   = 3,  /**<  颜色组列表3 */
    
    OSD_PATTERN_BUTT,
}OSD_FONT_COLORTAB_E;

/** 透明度选择对象 */
typedef enum _OSD_TRANS_E
{
    OSD_TRANS_BG    = 0,    /**<  背景透明 */
    OSD_TRANS_FG    = 1,    /**<  前景透明 */

    OSD_TRANS_BUTT,
}OSD_TRANS_E;

/** 高亮选择对象 */
typedef enum _OSD_HIGHLIGHT_E
{
    OSD_HIGHLIGHT_BG = 0,   /**<  背景高亮 */
    OSD_HIGHLIGHT_FG = 1,   /**<  前景高亮 */

    OSD_HIGHLIGHT_BUTT,
}OSD_HIGHLIGHT_E;

/** 数字显示对齐方式*/
typedef enum _OSD_NUMBER_SHOWALIGN_E
{
    OSD_NUMBER_SHOWALIGN_0 = 0,  /**<  左对齐显示（默认） */
    OSD_NUMBER_SHOWALIGN_1,      /**<  右边对齐显示 */
    OSD_NUMBER_SHOWALIGN_2,      /**<  居中显示 */
    OSD_NUMBER_SHOWALIGN_3,      /**<  全显示，不足补0 */
    
    OSD_NUMBER_SHOWALIGN_BUTT,
}OSD_NUMBER_SHOWALIGN_E;

/** OSD 输出颜色空间*/
typedef enum _OSD_COLORSPACE_E
{
    OSD_COLORSPACE_RGB = 0,     /**< OSD RGB输出 */
    OSD_COLORSPACE_YCBCR444,    /**< OSD YCBCR444输出 */
    OSD_COLORSPACE_YCBCR422,    /**< OSD YCBCR422输出 */
    
    OSD_COLORSPACE_BUTT,
}OSD_COLORSPACE_E;

/** OSD RGB颜色信息*/
typedef struct _OSD_COLOR_T
{
    UINT8 u8_R;
    UINT8 u8_G;
    UINT8 u8_B;
}OSD_COLOR_T;

/** OSD 窗口位置信息*/
typedef struct _OSD_WINSIZE_T
{
    UINT8 u8_osd_win;   /**<  OSD窗口，详见OSD_WIN_E */
    UINT16 u16_x;       /**<  水平方向起始位置，单位：像素 */
    UINT16 u16_y;       /**<  垂直方向起始位置，单位：行 */
    UINT16 u16_w;       /**<  水平方向尺寸 */
    UINT16 u16_h;       /**<  垂直方向尺寸 */
}OSD_WINSIZE_T;

/** OSD基础数据内容配置信息*/
typedef struct _OSD_INFO_T
{  
    UINT8 u8_font_width;     /**<  Font宽度，最大32 */
    UINT8 u8_font_height;    /**<  Font高度，最大64 */

    UINT16 u16_1bpp_num;     /**<  1bpp Font数 */
    UINT8 u8_2bpp_num;       /**<  2bpp Font数 */
    UINT8 u8_4bpp_num;       /**<  4bpp Font数 */

    UINT8 u8_colortab_size; /**<  ColorTab数据长度 */
    UINT8 *pu8_colortab;    /**<  ColorTab数据指针 */

    UINT8 *pu8_1bpp_data;    /**<  1bpp Font数据指针 */
    UINT8 *pu8_2bpp_data;    /**<  2bpp Font数据指针 */
    UINT8 *pu8_4bpp_data;    /**<  4bpp Font数据指针 */
}OSD_INFO_T;

/** OSD Transparent设置*/
typedef struct _OSD_TRAS_T
{
    BOOL b_trans_en;        /**<  OSD透明使能 */
    UINT8 u8_trans_mode;    /**<  OSD透明模式选择，0背景透明，1前景透明 */
    UINT8 u8_alpha_ratio;   /**<  OSD与Video的alpha ratio，范围0~255 */
}OSD_TRANS_T;

/** OSD window色彩填充配置参数*/
typedef struct _OSD_WIN_FILLCOLOR_T
{
    UINT8 u8_win_id;                    /**<  OSD窗口，详见OSD_WIN_E */
    BOOL b_fill_color_en;               /**<  色彩填充使能 */ 
    UINT8 u8_fill_color_type;           /**<  色彩填充类型，详见OSD_FILLCOLORTYPE_E */
    OSD_COLOR_T st_fill_start_color;    /**<  纯色填充RGB值 */
    OSD_COLOR_T st_fill_target_color;   /**<  渐进色填充目标颜色RGB值 */
}OSD_WIN_FILLCOLOR_T;

/** OSD Fade-in/out功能*/
typedef struct _OSD_FADE_T
{
    UINT8 u8_alpha_target_ratio;        /**<  OSD与视频显示目标透明比，范围0~255 */
    UINT8 u8_alpha_step;                /**<  OSD与视频显示透明比每帧定量变化值，范围0~255 */
}OSD_FADE_T;

/** OSD window配置参数*/
typedef struct _OSD_WINATTR_T
{
    UINT8 u8_osd_win;        /**<  OSD窗口，详见OSD_WIN_E */
    UINT8 u8_bd_en;          /**<  OSD 窗口边框使能 */
    UINT8 u8_bd_width;       /**<  OSD 窗口边框宽度，范围0~7 */
    OSD_COLOR_T st_bd_color; /**<  OSD 窗口边框RGB颜色 */

    UINT8 u8_rounded_en;     /**<  OSD 窗口圆角使能 */
    UINT8 u8_rounded_radius; /**<  OSD 窗口圆角半径值，范围0~15 */
    
    OSD_WIN_FILLCOLOR_T st_win_fillcolor;   /**< OSD窗口填充效果配置参数 */

    UINT8 u8_rotate_degrees; /**<  OSD 窗口旋转角度，详见OSD_ROTATE_E */

    UINT8 u8_attr_rp_en;     /**<  osd font attribute replace enable, only effect to highlight window */
    UINT8 u8_attr_rp_val;    /**<  osd replace font attribute   enum to OSD_FONTPATTERNCOLOR_E */
}OSD_WINATTR_T;

/** OSD Font高亮行配置参数*/
typedef struct _OSD_FONTHL_T
{
    BOOL b_highlight_en;            /**<  高亮行显示功能使能 */
    UINT8 u8_highlight_mode;        /**<  高亮行模式选择，0背景高亮，1前景高亮 */
    UINT8 u8_highlight_line;        /**<  高亮行号选择 */
    OSD_COLOR_T st_highlight_color; /**<  高亮行RGB颜色值 */
}OSD_FONTHL_T;

/** OSD ColorKey功能配置参数*/
typedef struct _OSD_COLKEY_T
{
    BOOL b_colkey_en;               /**<  ColorKey使能 */
    UINT8 u8_colkey_sel;            /**<  ColorKey索引选择，详见OSD_COLORKEY_E*/
    UINT8 u8_alf_ratio;             /**<  ColorKey的alf比例，范围0~255 */
    OSD_COLOR_T st_colkey_color;    /**<  ColorKey值 */
}OSD_COLKEY_T;

/** OSD Scale功能配置参数*/
typedef struct _OSD_SCALE_T
{
    UINT16 u16_h_ratio;     /**<  水平方向Scale比例，1x-100 2x-200 3x-300... */
    UINT16 u16_v_ratio;     /**<  垂直方向Scale比例，1x-100 2x-200 3x-300... */
}OSD_SCALE_T;

/** OSD Icon显示参数*/
typedef struct _OSD_ICON_T
{
    UINT8 u8_x;         /**<  水平位置（以Display Window位置为基准） */
    UINT8 u8_y;         /**<  垂直位置（以Display Window位置为基准） */
    UINT16 u16_idx;     /**<  Icon显示索引 */
    UINT8 u8_icon_size; /**<  Icon的size，高四位为weight，低四位为height */
}OSD_ICON_T;

/** OSD String显示参数*/
typedef struct _OSD_STRING_T
{
    UINT8 u8_x;     /**<  水平位置（以Display Window位置为基准） */
    UINT8 u8_y;     /**<  垂直位置（以Display Window位置为基准） */
    UINT16 u16_len;   /**<  String字符串长度 */
    CHAR* pstr;     /**<  String数据指针 */
}OSD_STRING_T;

/** OSD Fonts显示参数*/
typedef struct _OSD_FONT_T
{
    UINT8 u8_x;         /**<  水平位置（以Display Window位置为基准） */
    UINT8 u8_y;         /**<  垂直位置（以Display Window位置为基准） */
    UINT16 u16_length;  /**<  Font数据长度 */
    UINT16* pu16_idx;   /**<  Font显示索引指针 */
}OSD_FONT_T;

/** OSD Number显示参数*/
typedef struct _OSD_NUMBER_T
{
    UINT8 u8_x;             /**<  水平位置（以Display Window位置为基准） */
    UINT8 u8_y;             /**<  垂直位置（以Display Window位置为基准） */
    UINT16 u16_number;      /**<  Number数值，范围0~65535 */
    UINT8 u8_number_maxlen; /**<  Number数值的最大数字位数，范围1~5，默认5 */
    UINT8 u8_show_align;    /**<  数字显示对齐格式，详见OSD_NUMBER_SHOWALIGN_E */ 
}OSD_NUMBER_T;

/** OSD数据内容显示参数*/
typedef struct _OSD_DATA_SHOW_T
{
    OSD_NUMBER_T st_num;        /**<  数值显示参数结构体 */
    OSD_STRING_T st_string;     /**<  字符串显示参数结构体 */
    OSD_ICON_T   st_icon;       /**<  图标显示参数结构体 */
    OSD_FONT_T   st_font;       /**<  Font显示参数结构体 */
}OSD_DATA_SHOW_T;

/** OSD模块配置信息参数*/
typedef struct _OSD_DEV_T
{
    UINT8 u8_osd_idx;                   /**< OSD索引，详见OSD_INDEX_E */

    BOOL b_osd_enable;                  /**< OSD使能输出 */
    UINT8 u8_font_colortab;             /**< OSDFont颜色模式，详见OSD_FONT_COLORTAB_E */
    UINT8 u8_osd_color_space;           /**< OSD输出颜色空间 */
    OSD_INFO_T st_osd_info;             /**< OSD基础信息 */
    OSD_WINSIZE_T st_win_loc;           /**< OSD窗口位置信息*/
    OSD_WINATTR_T st_win_attr;          /**< OSD窗口属性 */
    OSD_FONTHL_T st_font_hl;            /**< OSDFont高亮设置 */
    OSD_COLKEY_T st_colkey;             /**< OSDColkey设置 */
    OSD_SCALE_T st_scale;               /**< OSDScale设置 */
    OSD_DATA_SHOW_T st_show_data;       /**< OSD显示数据 */
    OSD_TRANS_T st_osd_trans;           /**< OSD Transparency功能设置 */
    OSD_FADE_T st_osd_fade;             /**< OSD Fade-in/out功能设置 */
}OSD_DEV_T;

/**
* @brief        OSD 初始化模块配置信息
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
                pst_osd_info：初始化OSD模块结构体指针，详见OSD_INFO_T结构体描述
* @return       specific error types if call failed
* @note         用于初始化OSD模块的数据配置
*               数据配置信息为pst_osd_dev->st_osd_info结构体
*/
ERR_TYPE_E api_osd_init(OSD_DEV_T *pst_osd_dev, OSD_INFO_T *pst_osd_info);

/**
* @brief        OSD 初始化实例
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx 
*               e_osd_idx：OSD索引，详见OSD_INDEX_E枚举
* @return       specific error types if call failed
* @note                    
*/
ERR_TYPE_E api_osd_instance_init(OSD_DEV_T *pst_osd_dev, UINT8 u8_osd_idx);

/**
* @brief        OSD 使能控制
* @param [in]   pst_osd_dev：OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx       
* @return       无
* @note
*/
VOID api_osd_enable(OSD_DEV_T *pst_osd_dev);

/**
* @brief        OSD 触发显示
* @param [in]   pst_osd_dev：OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx       
* @return       无
* @note         需要触发才能使用的功能接口使用该接口完成触发
*/
VOID api_osd_trigger(OSD_DEV_T *pst_osd_dev);

/**
* @brief        OSD 清索引RAM
* @param [in]   pst_osd_dev：OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_clear(OSD_DEV_T *pst_osd_dev);

/**
* @brief        OSD 显示使能
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               b_enable: 选择OSD是否显示
* @return       specific error types if call failed
* @note         使能则显示OSD窗口，失能则隐藏OSD窗口 
*/
ERR_TYPE_E api_osd_show_enable(OSD_DEV_T *pst_osd_dev, BOOL b_enable);

/**
* @brief        OSD 窗口显示使能
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               u8_win_id：显示窗口选择，详见OSD_WIN_E枚举
*               b_enable: 选择窗口是否显示
* @return       specific error types if call failed
* @note         need trigger before effective
*/
ERR_TYPE_E api_osd_window_enable(OSD_DEV_T *pst_osd_dev, UINT8 u8_win_id, BOOL b_enable);

/**
* @brief        OSD 窗口属性设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_win_attr：窗口属性配置结构体指针，详见OSD_WINATTR_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_window_attr_set(OSD_DEV_T *pst_osd_dev, OSD_WINATTR_T* pst_win_attr);

/**
* @brief        OSD 窗口位置设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_win_loc：窗口位置信息结构体指针，详见OSD_WINSIZE_T结构体描述
* @return       specific error types if call failed
* @note         win_height < 128 && win_width < 256 && win_height*win_width < 4096
*               need trigger before effective
*/
ERR_TYPE_E api_osd_window_position_set(OSD_DEV_T *pst_osd_dev, OSD_WINSIZE_T *pst_win_loc);

/**
* @brief        OSD 窗口旋转
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               u8_rotate_degrees：窗口旋转角度设置，详见OSD_ROTATE_E枚举
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_window_rotate_set(OSD_DEV_T *pst_osd_dev, UINT8 u8_rotate_degrees);

/**
* @brief        OSD 窗口色彩填充
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_win_fillcolor：窗口色彩填充结构体指针，详见OSD_WIN_FILLCOLOR_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_window_fillcolor_set(OSD_DEV_T *pst_osd_dev, OSD_WIN_FILLCOLOR_T *pst_win_fillcolor);

/**
* @brief        OSD 透明功能设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_osd_trans：OSD透明功能结构体指针，详见OSD_TRANS_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_trans_set(OSD_DEV_T *pst_osd_dev, OSD_TRANS_T *pst_osd_trans);

/**
* @brief        OSD fade in/out设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_osd_fade：OSD 渐入渐出功能结构体指针，详见OSD_FADE_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_fade_set(OSD_DEV_T *pst_osd_dev, OSD_FADE_T *pst_osd_fade);

/**
* @brief        OSD Font Scale设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_scale：Font scale功能结构体指针，详见OSD_SCALE_T结构体描述
* @return       specific error types if call failed
* @note         need trigger before effective
*/
ERR_TYPE_E api_osd_font_scale(OSD_DEV_T *pst_osd_dev, OSD_SCALE_T* pst_scale);

/**
* @brief        OSD Font Color Table选择
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               u8_pattern_color：Font显示Color Table选择，详见OSD_FONT_COLORTAB_E枚举
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_font_colortab_set(OSD_DEV_T *pst_osd_dev, UINT8 u8_pattern_color);

/**
* @brief        OSD Font高亮行功能配置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_font_hl：高亮行功能配置结构体指针，详见OSD_FONTHL_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_font_highlight_set(OSD_DEV_T *pst_osd_dev, OSD_FONTHL_T *pst_font_hl);

/**
* @brief        OSD ColorKey设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_colkey：Color key功能结构体指针，详见OSD_COLKEY_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_colorkey_set(OSD_DEV_T *pst_osd_dev, OSD_COLKEY_T *pst_colkey);

/**
* @brief        OSD 输出颜色空间设置
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               u8_color_space：OSD显示color space设置，详见OSD_COLORSPACE_E枚举
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_colorspace_set(OSD_DEV_T *pst_osd_dev, UINT8 u8_color_space);

/**
* @brief        OSD 显示字符串
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_string : string显示信息参数指针，详见OSD_STRING_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_show_string(OSD_DEV_T *pst_osd_dev, OSD_STRING_T *pst_string);

/**
* @brief        OSD 显示一定长度字符串
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_string : string显示信息参数指针，详见OSD_STRING_T结构体描述
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_show_length_string(OSD_DEV_T *pst_osd_dev, OSD_STRING_T *pst_string);

/**
* @brief        OSD 显示数字
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_num : number显示信息参数指针，详见OSD_NUMBER_T结构体描述
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_show_number(OSD_DEV_T *pst_osd_dev, OSD_NUMBER_T *pst_num);

/**
* @brief        OSD 显示图标
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_icon : icon显示信息参数指针，详见OSD_ICON_T结构体描述
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_show_icon(OSD_DEV_T *pst_osd_dev, OSD_ICON_T *pst_icon);

/**
* @brief        OSD 显示font
* @param [in]   pst_osd_dev : OSD DEV参数指针，关联参数pst_osd_dev->u8_osd_idx
*               pst_font : Font显示信息参数指针，详见OSD_FONT_T结构体描述
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_show_index(OSD_DEV_T *pst_osd_dev, OSD_FONT_T *pst_font);

/**
* @brief        OSD Font加载显示触发
* @param [in]   pst_osd_dev : OSD DEV参数指针     
* @return       无
* @note         Font index显示更新后需要load trigger后生效
*/
ERR_TYPE_E api_osd_index_load_trigger(OSD_DEV_T *pst_osd_dev);


#endif // __HDMI_OSD_API_H__

/**
 * @}
 */
