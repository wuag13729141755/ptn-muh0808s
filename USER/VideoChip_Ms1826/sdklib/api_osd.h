/**
* @file  api_osd.h  
* @brief OSDģ��������ݽṹ����������                                                           
*/

/**
 * @defgroup API_OSD osd module api
 *
 * @brief ����OSD��ʾ����
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

/** OSD�����������ĸ�OSD*/
typedef enum _OSD_INDEX_E 
{
    OSD_1 = 0,  /**<  OSD 1����ʾ��һ��OSD */
    OSD_2 = 1,  /**<  OSD 2����ʾ�ڶ���OSD */
    OSD_3 = 2,  /**<  OSD 3����ʾ������OSD */
    OSD_4 = 3,  /**<  OSD 4����ʾ���ĸ�OSD */
    
    OSD_BUTT,
}OSD_INDEX_E;

/** �������ͣ��ĸ��Ӵ��� */
typedef enum _OSD_WIN_E
{
    OSD_WIN_DISPLAY = 0,        /**<  ��ʾ�Ӵ��� */
    OSD_WIN_HIGHLIGHT1 = 1,     /**<  HIGHLIGHT 1 ����*/
    OSD_WIN_HIGHLIGHT2 = 2,     /**<  HIGHLIGHT 2 ����*/
    OSD_WIN_HIGHLIGHT3 = 3,     /**<  HIGHLIGHT 3 ����*/

    OSD_WIN_BUTT,
}OSD_WIN_E;

/** COLORKEY���ͣ�֧������ColorKey */
typedef enum _OSD_COLORKEY_E 
{
    OSD_COLORKEY_0 = 0,     /**< ColorKey 0 */
    OSD_COLORKEY_1 = 1,     /**< ColorKey 1 */
    
    OSD_COLORKEY_BUTT,
}OSD_COLORKEY_E;
    
/** window��ת�Ƕ�*/
typedef enum _OSD_ROTATE_E
{
    OSD_ROTATE0    = 0, /**<  ����ת��Ĭ�ϣ� */
    OSD_ROTATE90   = 1, /**<  ��ת90�� */
    OSD_ROTATE180  = 2, /**<  ��ת180�� */
    OSD_ROTATE270  = 3, /**<  ��ת270�� */
    
    OSD_ROTATE_BUTT,
}OSD_ROTATE_E;

/** windowɫ���������*/
typedef enum _OSD_FILLCOLORTYPE_E
{
    OSD_FILLCOLOR_TYPE0 = 0,    /**<  ��ɫ��䣨Ĭ�ϣ� */
    OSD_FILLCOLOR_TYPE1 = 1,    /**<  ����ɫ���������� */
    OSD_FILLCOLOR_TYPE2 = 2,    /**<  ����ɫ���������� */
    OSD_FILLCOLOR_TYPE3 = 3,    /**<  ����ɫ�����������м� */
    OSD_FILLCOLOR_TYPE4 = 4,    /**<  ����ɫ�����������м� */
    
    OSD_FILLCOLOR_BUTT,
}OSD_FILLCOLORTYPE_E;

/** Font��ɫ���б�*/
typedef enum _OSD_FONT_COLORTAB_E
{
    OSD_PATTERN_COLOR0   = 0,  /**<  ��ɫ���б�0 */
    OSD_PATTERN_COLOR1   = 1,  /**<  ��ɫ���б�1 */
    OSD_PATTERN_COLOR2   = 2,  /**<  ��ɫ���б�2 */
    OSD_PATTERN_COLOR3   = 3,  /**<  ��ɫ���б�3 */
    
    OSD_PATTERN_BUTT,
}OSD_FONT_COLORTAB_E;

/** ͸����ѡ����� */
typedef enum _OSD_TRANS_E
{
    OSD_TRANS_BG    = 0,    /**<  ����͸�� */
    OSD_TRANS_FG    = 1,    /**<  ǰ��͸�� */

    OSD_TRANS_BUTT,
}OSD_TRANS_E;

/** ����ѡ����� */
typedef enum _OSD_HIGHLIGHT_E
{
    OSD_HIGHLIGHT_BG = 0,   /**<  �������� */
    OSD_HIGHLIGHT_FG = 1,   /**<  ǰ������ */

    OSD_HIGHLIGHT_BUTT,
}OSD_HIGHLIGHT_E;

/** ������ʾ���뷽ʽ*/
typedef enum _OSD_NUMBER_SHOWALIGN_E
{
    OSD_NUMBER_SHOWALIGN_0 = 0,  /**<  �������ʾ��Ĭ�ϣ� */
    OSD_NUMBER_SHOWALIGN_1,      /**<  �ұ߶�����ʾ */
    OSD_NUMBER_SHOWALIGN_2,      /**<  ������ʾ */
    OSD_NUMBER_SHOWALIGN_3,      /**<  ȫ��ʾ�����㲹0 */
    
    OSD_NUMBER_SHOWALIGN_BUTT,
}OSD_NUMBER_SHOWALIGN_E;

/** OSD �����ɫ�ռ�*/
typedef enum _OSD_COLORSPACE_E
{
    OSD_COLORSPACE_RGB = 0,     /**< OSD RGB��� */
    OSD_COLORSPACE_YCBCR444,    /**< OSD YCBCR444��� */
    OSD_COLORSPACE_YCBCR422,    /**< OSD YCBCR422��� */
    
    OSD_COLORSPACE_BUTT,
}OSD_COLORSPACE_E;

/** OSD RGB��ɫ��Ϣ*/
typedef struct _OSD_COLOR_T
{
    UINT8 u8_R;
    UINT8 u8_G;
    UINT8 u8_B;
}OSD_COLOR_T;

/** OSD ����λ����Ϣ*/
typedef struct _OSD_WINSIZE_T
{
    UINT8 u8_osd_win;   /**<  OSD���ڣ����OSD_WIN_E */
    UINT16 u16_x;       /**<  ˮƽ������ʼλ�ã���λ������ */
    UINT16 u16_y;       /**<  ��ֱ������ʼλ�ã���λ���� */
    UINT16 u16_w;       /**<  ˮƽ����ߴ� */
    UINT16 u16_h;       /**<  ��ֱ����ߴ� */
}OSD_WINSIZE_T;

/** OSD������������������Ϣ*/
typedef struct _OSD_INFO_T
{  
    UINT8 u8_font_width;     /**<  Font��ȣ����32 */
    UINT8 u8_font_height;    /**<  Font�߶ȣ����64 */

    UINT16 u16_1bpp_num;     /**<  1bpp Font�� */
    UINT8 u8_2bpp_num;       /**<  2bpp Font�� */
    UINT8 u8_4bpp_num;       /**<  4bpp Font�� */

    UINT8 u8_colortab_size; /**<  ColorTab���ݳ��� */
    UINT8 *pu8_colortab;    /**<  ColorTab����ָ�� */

    UINT8 *pu8_1bpp_data;    /**<  1bpp Font����ָ�� */
    UINT8 *pu8_2bpp_data;    /**<  2bpp Font����ָ�� */
    UINT8 *pu8_4bpp_data;    /**<  4bpp Font����ָ�� */
}OSD_INFO_T;

/** OSD Transparent����*/
typedef struct _OSD_TRAS_T
{
    BOOL b_trans_en;        /**<  OSD͸��ʹ�� */
    UINT8 u8_trans_mode;    /**<  OSD͸��ģʽѡ��0����͸����1ǰ��͸�� */
    UINT8 u8_alpha_ratio;   /**<  OSD��Video��alpha ratio����Χ0~255 */
}OSD_TRANS_T;

/** OSD windowɫ��������ò���*/
typedef struct _OSD_WIN_FILLCOLOR_T
{
    UINT8 u8_win_id;                    /**<  OSD���ڣ����OSD_WIN_E */
    BOOL b_fill_color_en;               /**<  ɫ�����ʹ�� */ 
    UINT8 u8_fill_color_type;           /**<  ɫ��������ͣ����OSD_FILLCOLORTYPE_E */
    OSD_COLOR_T st_fill_start_color;    /**<  ��ɫ���RGBֵ */
    OSD_COLOR_T st_fill_target_color;   /**<  ����ɫ���Ŀ����ɫRGBֵ */
}OSD_WIN_FILLCOLOR_T;

/** OSD Fade-in/out����*/
typedef struct _OSD_FADE_T
{
    UINT8 u8_alpha_target_ratio;        /**<  OSD����Ƶ��ʾĿ��͸���ȣ���Χ0~255 */
    UINT8 u8_alpha_step;                /**<  OSD����Ƶ��ʾ͸����ÿ֡�����仯ֵ����Χ0~255 */
}OSD_FADE_T;

/** OSD window���ò���*/
typedef struct _OSD_WINATTR_T
{
    UINT8 u8_osd_win;        /**<  OSD���ڣ����OSD_WIN_E */
    UINT8 u8_bd_en;          /**<  OSD ���ڱ߿�ʹ�� */
    UINT8 u8_bd_width;       /**<  OSD ���ڱ߿��ȣ���Χ0~7 */
    OSD_COLOR_T st_bd_color; /**<  OSD ���ڱ߿�RGB��ɫ */

    UINT8 u8_rounded_en;     /**<  OSD ����Բ��ʹ�� */
    UINT8 u8_rounded_radius; /**<  OSD ����Բ�ǰ뾶ֵ����Χ0~15 */
    
    OSD_WIN_FILLCOLOR_T st_win_fillcolor;   /**< OSD�������Ч�����ò��� */

    UINT8 u8_rotate_degrees; /**<  OSD ������ת�Ƕȣ����OSD_ROTATE_E */

    UINT8 u8_attr_rp_en;     /**<  osd font attribute replace enable, only effect to highlight window */
    UINT8 u8_attr_rp_val;    /**<  osd replace font attribute   enum to OSD_FONTPATTERNCOLOR_E */
}OSD_WINATTR_T;

/** OSD Font���������ò���*/
typedef struct _OSD_FONTHL_T
{
    BOOL b_highlight_en;            /**<  ��������ʾ����ʹ�� */
    UINT8 u8_highlight_mode;        /**<  ������ģʽѡ��0����������1ǰ������ */
    UINT8 u8_highlight_line;        /**<  �����к�ѡ�� */
    OSD_COLOR_T st_highlight_color; /**<  ������RGB��ɫֵ */
}OSD_FONTHL_T;

/** OSD ColorKey�������ò���*/
typedef struct _OSD_COLKEY_T
{
    BOOL b_colkey_en;               /**<  ColorKeyʹ�� */
    UINT8 u8_colkey_sel;            /**<  ColorKey����ѡ�����OSD_COLORKEY_E*/
    UINT8 u8_alf_ratio;             /**<  ColorKey��alf��������Χ0~255 */
    OSD_COLOR_T st_colkey_color;    /**<  ColorKeyֵ */
}OSD_COLKEY_T;

/** OSD Scale�������ò���*/
typedef struct _OSD_SCALE_T
{
    UINT16 u16_h_ratio;     /**<  ˮƽ����Scale������1x-100 2x-200 3x-300... */
    UINT16 u16_v_ratio;     /**<  ��ֱ����Scale������1x-100 2x-200 3x-300... */
}OSD_SCALE_T;

/** OSD Icon��ʾ����*/
typedef struct _OSD_ICON_T
{
    UINT8 u8_x;         /**<  ˮƽλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT8 u8_y;         /**<  ��ֱλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT16 u16_idx;     /**<  Icon��ʾ���� */
    UINT8 u8_icon_size; /**<  Icon��size������λΪweight������λΪheight */
}OSD_ICON_T;

/** OSD String��ʾ����*/
typedef struct _OSD_STRING_T
{
    UINT8 u8_x;     /**<  ˮƽλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT8 u8_y;     /**<  ��ֱλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT16 u16_len;   /**<  String�ַ������� */
    CHAR* pstr;     /**<  String����ָ�� */
}OSD_STRING_T;

/** OSD Fonts��ʾ����*/
typedef struct _OSD_FONT_T
{
    UINT8 u8_x;         /**<  ˮƽλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT8 u8_y;         /**<  ��ֱλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT16 u16_length;  /**<  Font���ݳ��� */
    UINT16* pu16_idx;   /**<  Font��ʾ����ָ�� */
}OSD_FONT_T;

/** OSD Number��ʾ����*/
typedef struct _OSD_NUMBER_T
{
    UINT8 u8_x;             /**<  ˮƽλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT8 u8_y;             /**<  ��ֱλ�ã���Display Windowλ��Ϊ��׼�� */
    UINT16 u16_number;      /**<  Number��ֵ����Χ0~65535 */
    UINT8 u8_number_maxlen; /**<  Number��ֵ���������λ������Χ1~5��Ĭ��5 */
    UINT8 u8_show_align;    /**<  ������ʾ�����ʽ�����OSD_NUMBER_SHOWALIGN_E */ 
}OSD_NUMBER_T;

/** OSD����������ʾ����*/
typedef struct _OSD_DATA_SHOW_T
{
    OSD_NUMBER_T st_num;        /**<  ��ֵ��ʾ�����ṹ�� */
    OSD_STRING_T st_string;     /**<  �ַ�����ʾ�����ṹ�� */
    OSD_ICON_T   st_icon;       /**<  ͼ����ʾ�����ṹ�� */
    OSD_FONT_T   st_font;       /**<  Font��ʾ�����ṹ�� */
}OSD_DATA_SHOW_T;

/** OSDģ��������Ϣ����*/
typedef struct _OSD_DEV_T
{
    UINT8 u8_osd_idx;                   /**< OSD���������OSD_INDEX_E */

    BOOL b_osd_enable;                  /**< OSDʹ����� */
    UINT8 u8_font_colortab;             /**< OSDFont��ɫģʽ�����OSD_FONT_COLORTAB_E */
    UINT8 u8_osd_color_space;           /**< OSD�����ɫ�ռ� */
    OSD_INFO_T st_osd_info;             /**< OSD������Ϣ */
    OSD_WINSIZE_T st_win_loc;           /**< OSD����λ����Ϣ*/
    OSD_WINATTR_T st_win_attr;          /**< OSD�������� */
    OSD_FONTHL_T st_font_hl;            /**< OSDFont�������� */
    OSD_COLKEY_T st_colkey;             /**< OSDColkey���� */
    OSD_SCALE_T st_scale;               /**< OSDScale���� */
    OSD_DATA_SHOW_T st_show_data;       /**< OSD��ʾ���� */
    OSD_TRANS_T st_osd_trans;           /**< OSD Transparency�������� */
    OSD_FADE_T st_osd_fade;             /**< OSD Fade-in/out�������� */
}OSD_DEV_T;

/**
* @brief        OSD ��ʼ��ģ��������Ϣ
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
                pst_osd_info����ʼ��OSDģ��ṹ��ָ�룬���OSD_INFO_T�ṹ������
* @return       specific error types if call failed
* @note         ���ڳ�ʼ��OSDģ�����������
*               ����������ϢΪpst_osd_dev->st_osd_info�ṹ��
*/
ERR_TYPE_E api_osd_init(OSD_DEV_T *pst_osd_dev, OSD_INFO_T *pst_osd_info);

/**
* @brief        OSD ��ʼ��ʵ��
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx 
*               e_osd_idx��OSD���������OSD_INDEX_Eö��
* @return       specific error types if call failed
* @note                    
*/
ERR_TYPE_E api_osd_instance_init(OSD_DEV_T *pst_osd_dev, UINT8 u8_osd_idx);

/**
* @brief        OSD ʹ�ܿ���
* @param [in]   pst_osd_dev��OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx       
* @return       ��
* @note
*/
VOID api_osd_enable(OSD_DEV_T *pst_osd_dev);

/**
* @brief        OSD ������ʾ
* @param [in]   pst_osd_dev��OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx       
* @return       ��
* @note         ��Ҫ��������ʹ�õĹ��ܽӿ�ʹ�øýӿ���ɴ���
*/
VOID api_osd_trigger(OSD_DEV_T *pst_osd_dev);

/**
* @brief        OSD ������RAM
* @param [in]   pst_osd_dev��OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_clear(OSD_DEV_T *pst_osd_dev);

/**
* @brief        OSD ��ʾʹ��
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               b_enable: ѡ��OSD�Ƿ���ʾ
* @return       specific error types if call failed
* @note         ʹ������ʾOSD���ڣ�ʧ��������OSD���� 
*/
ERR_TYPE_E api_osd_show_enable(OSD_DEV_T *pst_osd_dev, BOOL b_enable);

/**
* @brief        OSD ������ʾʹ��
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               u8_win_id����ʾ����ѡ�����OSD_WIN_Eö��
*               b_enable: ѡ�񴰿��Ƿ���ʾ
* @return       specific error types if call failed
* @note         need trigger before effective
*/
ERR_TYPE_E api_osd_window_enable(OSD_DEV_T *pst_osd_dev, UINT8 u8_win_id, BOOL b_enable);

/**
* @brief        OSD ������������
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_win_attr�������������ýṹ��ָ�룬���OSD_WINATTR_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_window_attr_set(OSD_DEV_T *pst_osd_dev, OSD_WINATTR_T* pst_win_attr);

/**
* @brief        OSD ����λ������
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_win_loc������λ����Ϣ�ṹ��ָ�룬���OSD_WINSIZE_T�ṹ������
* @return       specific error types if call failed
* @note         win_height < 128 && win_width < 256 && win_height*win_width < 4096
*               need trigger before effective
*/
ERR_TYPE_E api_osd_window_position_set(OSD_DEV_T *pst_osd_dev, OSD_WINSIZE_T *pst_win_loc);

/**
* @brief        OSD ������ת
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               u8_rotate_degrees��������ת�Ƕ����ã����OSD_ROTATE_Eö��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_window_rotate_set(OSD_DEV_T *pst_osd_dev, UINT8 u8_rotate_degrees);

/**
* @brief        OSD ����ɫ�����
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_win_fillcolor������ɫ�����ṹ��ָ�룬���OSD_WIN_FILLCOLOR_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_window_fillcolor_set(OSD_DEV_T *pst_osd_dev, OSD_WIN_FILLCOLOR_T *pst_win_fillcolor);

/**
* @brief        OSD ͸����������
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_osd_trans��OSD͸�����ܽṹ��ָ�룬���OSD_TRANS_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_trans_set(OSD_DEV_T *pst_osd_dev, OSD_TRANS_T *pst_osd_trans);

/**
* @brief        OSD fade in/out����
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_osd_fade��OSD ���뽥�����ܽṹ��ָ�룬���OSD_FADE_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_fade_set(OSD_DEV_T *pst_osd_dev, OSD_FADE_T *pst_osd_fade);

/**
* @brief        OSD Font Scale����
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_scale��Font scale���ܽṹ��ָ�룬���OSD_SCALE_T�ṹ������
* @return       specific error types if call failed
* @note         need trigger before effective
*/
ERR_TYPE_E api_osd_font_scale(OSD_DEV_T *pst_osd_dev, OSD_SCALE_T* pst_scale);

/**
* @brief        OSD Font Color Tableѡ��
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               u8_pattern_color��Font��ʾColor Tableѡ�����OSD_FONT_COLORTAB_Eö��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_font_colortab_set(OSD_DEV_T *pst_osd_dev, UINT8 u8_pattern_color);

/**
* @brief        OSD Font�����й�������
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_font_hl�������й������ýṹ��ָ�룬���OSD_FONTHL_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_font_highlight_set(OSD_DEV_T *pst_osd_dev, OSD_FONTHL_T *pst_font_hl);

/**
* @brief        OSD ColorKey����
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_colkey��Color key���ܽṹ��ָ�룬���OSD_COLKEY_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_colorkey_set(OSD_DEV_T *pst_osd_dev, OSD_COLKEY_T *pst_colkey);

/**
* @brief        OSD �����ɫ�ռ�����
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               u8_color_space��OSD��ʾcolor space���ã����OSD_COLORSPACE_Eö��
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_colorspace_set(OSD_DEV_T *pst_osd_dev, UINT8 u8_color_space);

/**
* @brief        OSD ��ʾ�ַ���
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_string : string��ʾ��Ϣ����ָ�룬���OSD_STRING_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_show_string(OSD_DEV_T *pst_osd_dev, OSD_STRING_T *pst_string);

/**
* @brief        OSD ��ʾһ�������ַ���
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_string : string��ʾ��Ϣ����ָ�룬���OSD_STRING_T�ṹ������
* @return       specific error types if call failed
* @note
*/
ERR_TYPE_E api_osd_show_length_string(OSD_DEV_T *pst_osd_dev, OSD_STRING_T *pst_string);

/**
* @brief        OSD ��ʾ����
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_num : number��ʾ��Ϣ����ָ�룬���OSD_NUMBER_T�ṹ������
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_show_number(OSD_DEV_T *pst_osd_dev, OSD_NUMBER_T *pst_num);

/**
* @brief        OSD ��ʾͼ��
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_icon : icon��ʾ��Ϣ����ָ�룬���OSD_ICON_T�ṹ������
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_show_icon(OSD_DEV_T *pst_osd_dev, OSD_ICON_T *pst_icon);

/**
* @brief        OSD ��ʾfont
* @param [in]   pst_osd_dev : OSD DEV����ָ�룬��������pst_osd_dev->u8_osd_idx
*               pst_font : Font��ʾ��Ϣ����ָ�룬���OSD_FONT_T�ṹ������
* @return       specific error types if call failed
* @note         
*/
ERR_TYPE_E api_osd_show_index(OSD_DEV_T *pst_osd_dev, OSD_FONT_T *pst_font);

/**
* @brief        OSD Font������ʾ����
* @param [in]   pst_osd_dev : OSD DEV����ָ��     
* @return       ��
* @note         Font index��ʾ���º���Ҫload trigger����Ч
*/
ERR_TYPE_E api_osd_index_load_trigger(OSD_DEV_T *pst_osd_dev);


#endif // __HDMI_OSD_API_H__

/**
 * @}
 */
