#ifndef  __MACROSILICON_MS1826_GUI_FONT_H__
#define  __MACROSILICON_MS1826_GUI_FONT_H__
//#include "ms1826_app.h"

#ifndef _countof
#define _countof(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))
#endif

#define MUSIC_ICON      0

#define FONT_WIDTH      16
#define FONT_HEIGHT     18

#define FONT_1BPP_NUM   165
#define FONT_2BPP_NUM   0
#define FONT_4BPP_NUM   0

extern UINT8 OSD_COLOR_TABLE[];
extern UINT8 const OSD_1BPP_FONT_TABLE[];
extern UINT8 const OSD_2BPP_FONT_TABLE[];
extern UINT8 const OSD_4BPP_FONT_TABLE[];


extern UINT8  g_u8_color_table_size;
extern UINT16 g_u16_1bpp_font_table_size;
extern UINT16 g_u16_2bpp_font_table_size;
extern UINT16 g_u16_4bpp_font_table_size;

#endif //__MACROSILICON_MS1823_GUI_FONT_H__
