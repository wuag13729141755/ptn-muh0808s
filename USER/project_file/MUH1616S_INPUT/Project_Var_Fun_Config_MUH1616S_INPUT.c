#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"
#include "ms1826_app_single_mode.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

stProjectSpecVar_t  stMUH1616S_INPUTSpecVar;
static volatile u32 g_MUH1616S_INPUT_Timers[emUserTimer_Max];
stLT8644SwitchTable_T g_stMUH1616S_INPUTSwitchTable;
u16 g_u16SrcChgTxFreezeState = 0;
stLinkSta_t    g_stLinkSta;
stLinkSta_t    g_stPreLinkSta;
u8 g_u8SceneStartIndex = 0;
bool g_bMessageFreezeOkFlag = TRUE;
bool g_bStartupOKFlag = FALSE;

extern stProjectGlobalAllVar_t g_stPrjGloVar;

unsigned char const  _1_1024x768p60[256] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0E, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x01, 0x08, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0xD6, 0x18, 0x00, 0x30, 0x41, 0x00, 0x1E, 0x30, 0x30, 0x68,
    0x34, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1C, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20,
    0xB8, 0x28, 0x55, 0x40, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18,
    0x4B, 0x1A, 0x51, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0xFD,
    0x02, 0x03, 0x1C, 0x72, 0x44, 0x84, 0x93, 0x12, 0x03, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00,
    0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x10, 0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8,
};


unsigned char const  _2_1280x720p60[256] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0E, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x01, 0x08, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 0x6E, 0x28,
    0x55, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20,
    0xB8, 0x28, 0x55, 0x40, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18,
    0x4B, 0x1A, 0x51, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0x9A,
    0x02, 0x03, 0x1C, 0x72, 0x44, 0x84, 0x93, 0x12, 0x03, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00,
    0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x10, 0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8,
};
unsigned char const  _3_1360x768p60[256] = //
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0E, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x01, 0x08, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1B, 0x21, 0x50, 0xA0, 0x51, 0x00, 0x1E, 0x30, 0x48, 0x88,
    0x3A, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1C, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20,
    0xB8, 0x28, 0x55, 0x40, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18,
    0x4B, 0x1A, 0x51, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0xA1,
    0x02, 0x03, 0x1C, 0x72, 0x44, 0x84, 0x93, 0x12, 0x03, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00,
    0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x10, 0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8,
};

unsigned char const  _4_1920x1200p60[256] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0A, 0xEE, 0x95, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x21, 0x08, 0x00, 0xD1, 0x00, 0x81, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x28, 0x3C, 0x80, 0xA0, 0x70, 0xB0, 0x23, 0x40, 0x30, 0x20,
    0x36, 0x00, 0x80, 0xB0, 0x74, 0x00, 0x00, 0x1A, 0x9E, 0x20, 0x00, 0x90, 0x51, 0x20, 0x1F, 0x30,
    0x48, 0x80, 0x36, 0x00, 0x20, 0xF4, 0x31, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18,
    0x4B, 0x1A, 0x51, 0x10, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0x96,
    0x02, 0x03, 0x1C, 0x70, 0x44, 0x10, 0x1F, 0x04, 0x13, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00,
    0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x20, 0xE2, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38,

};

unsigned char const  _5_3840x2160p60[256] = //
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78, 0x0A, 0xCF, 0x74, 0xA3, 0x57, 0x4C, 0xB0, 0x23,
    0x09, 0x48, 0x4C, 0x01, 0x08, 0x00, 0xD1, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x74, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
    0x8A, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
    0x58, 0x2C, 0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,
    0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x3B, 0x46, 0x1F, 0x8C, 0x3C, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xC0,
    0x02, 0x03, 0x23, 0xF2, 0x4E, 0x05, 0x84, 0x13, 0x14, 0x90, 0x1F, 0x20, 0x22, 0x5D, 0x5F, 0x62,
    0x64, 0x5E, 0x63, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10,
    0x00, 0x00, 0x44, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0xC4,
    0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xA0, 0x14, 0x51, 0xF0, 0x16, 0x00, 0x26, 0x7C, 0x43,
    0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7,
};

unsigned char const  _5_1600x1200p60[256] = //
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0E, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x01, 0x08, 0x00, 0x81, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x3F, 0x40, 0x30, 0x62, 0xB0, 0x2D, 0x40, 0x70, 0xA8,
    0x34, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1C, 0x01, 0x1D, 0x00, 0xBC, 0x52, 0xD0, 0x1E, 0x20,
    0xB8, 0x28, 0x55, 0x40, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x18,
    0x4B, 0x1A, 0x51, 0x08, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0xF5,
    0x02, 0x03, 0x1C, 0x72, 0x44, 0x84, 0x93, 0x12, 0x03, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00,
    0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0xB8, 0x10, 0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE8,
};

unsigned char const  _6_1920x1080p60[256] =
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x50, 0x2D, 0x78, 0x0A, 0xEE, 0x91, 0xA3, 0x54, 0x4C, 0x99, 0x26,
    0x0F, 0x50, 0x54, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C,
    0x45, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x48, 0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x01, 0x68,
    0x02, 0x03, 0x23, 0x72, 0x4B, 0x90, 0x9F, 0x22, 0x21, 0x20, 0x05, 0x14, 0x04, 0x13, 0x12, 0x03,
    0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10, 0x00, 0x00, 0x20,
    0xE2, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD7,
};

unsigned char const  _7_3840x2160p30[256] = //
{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x32, 0x1C, 0x01, 0x03, 0x80, 0x73, 0x41, 0x78, 0x0A, 0xCF, 0x74, 0xA3, 0x57, 0x4C, 0xB0, 0x23,
    0x09, 0x48, 0x4C, 0x01, 0x08, 0x00, 0xD1, 0xC0, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x74, 0x00, 0x30, 0xF2, 0x70, 0x5A, 0x80, 0xB0, 0x58,
    0x8A, 0x00, 0x20, 0xC2, 0x31, 0x00, 0x00, 0x1E, 0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40,
    0x58, 0x2C, 0x45, 0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x48,
    0x44, 0x20, 0x56, 0x69, 0x64, 0x65, 0x6F, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x00, 0x00, 0x00, 0xFD,
    0x00, 0x3B, 0x46, 0x1F, 0x8C, 0x3C, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xC0,
    0x02, 0x03, 0x23, 0xF2, 0x4E, 0x05, 0x84, 0x13, 0x14, 0x90, 0x1F, 0x20, 0x22, 0x5D, 0x5F, 0x62,
    0x64, 0x5E, 0x63, 0x23, 0x09, 0x1F, 0x07, 0x83, 0x01, 0x00, 0x00, 0x67, 0x03, 0x0C, 0x00, 0x10,
    0x00, 0x00, 0x44, 0x8C, 0x0A, 0xD0, 0x8A, 0x20, 0xE0, 0x2D, 0x10, 0x10, 0x3E, 0x96, 0x00, 0xC4,
    0x8E, 0x21, 0x00, 0x00, 0x18, 0x8C, 0x0A, 0xA0, 0x14, 0x51, 0xF0, 0x16, 0x00, 0x26, 0x7C, 0x43,
    0x00, 0xC4, 0x8E, 0x21, 0x00, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7,
};

const u8 cg_u8ChannelTable[16][4]=
{
  //TXIndex,RxPort,AudioPort,DeviceID
    {0,         2,          0,      0},
    {1,         1,          1,      0},
    {2,         0,          2,      0},
    {3,         3,          3,      0},
    {0,         2,          0,      1},
    {1,         1,          1,      1},
    {2,         0,          2,      1},
    {3,         3,          3,      1},
    {0,         2,          0,      2},
    {1,         1,          1,      2},
    {2,         0,          2,      2},
    {3,         3,          3,      2},
    {0,         2,          0,      3},
    {1,         1,          1,      3},
    {2,         0,          2,      3},
    {3,         3,          3,      3},
};

void app_MUH1616S_INPUTMS1826UserInit(void);
void app_MUH1616S_INPUTLT8644SwitchProcess(void);
void app_MUH1616S_INPUTMS1826UserProcess(void);
void app_MUH1616S_INPUTStartupLinkInfoGet(void);
void app_MUH1616S_INPUTCheckLinkStatusChangeProcess(void);
void app_MUH1616S_INPUTCheckInputResolutionChangeProcess(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时
//------------------------------------------------------------------------------
void mapp_MUH1616S_INPUT_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_MUH1616S_INPUT_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时
//------------------------------------------------------------------------------
u32 mapp_MUH1616S_INPUT_PlatformTimerGet(uint8_t eTimer)
{
    return(g_MUH1616S_INPUT_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_MUH1616S_INPUT_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_MUH1616S_INPUT_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_MUH1616S_INPUT_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_MUH1616S_INPUT_Timers[i] > 0)
        {
            g_MUH1616S_INPUT_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_MUH1616S_INPUT_PlatformTimerExpired ( uint8_t timer )           //定时时间到函�?
{
    if ( timer < emUserTimer_Max )
    {
        return( g_MUH1616S_INPUT_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void MUH1616S_INPUT_1msTimerIrq(void)
{
    mapp_MUH1616S_INPUT_PlatformTimerTask();
}

void MUH1616S_INPUT_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void MUH1616S_INPUT_SpecificVarDefault(void)
{
    MUH1616S_INPUT_SpecBasicVarDefault();
    MUH1616S_INPUT_SpecCustomEdidVarDefault();
}

void MUH1616S_INPUT_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    u8 i;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
//    pstSpecVar->stBasSpecVar.bIsDebugEn = FALSE;

    for(i=0;i<Def_outport_num;i++)
    {
        pstSpecVar->stBasSpecVar.u8TxResolutionID[i] = emRes_1920x1080_60;
        pstSpecVar->stBasSpecVar.u8TxFormatSta[i]    = 1;
        pstSpecVar->stBasSpecVar.u8TxHdcpMode[i]     = 0;
        pstSpecVar->stBasSpecVar.u8FreezeSta[i]      = 0;
        pstSpecVar->stBasSpecVar.u8Brightness[i]     = 50;
        pstSpecVar->stBasSpecVar.u8Contrast[i]       = 50;
        pstSpecVar->stBasSpecVar.u8Saturation[i]     = 50;
        pstSpecVar->stBasSpecVar.u8Hue[i]            = 50;
        pstSpecVar->stBasSpecVar.u8AudioMute[i]      = 0;
        pstSpecVar->stBasSpecVar.u8BlackScreenEnable[i] = 0;
        pstSpecVar->stBasSpecVar.u8PatternSta[i]     = 0;
        pstSpecVar->stBasSpecVar.u8RedValue[i]       = 128;
        pstSpecVar->stBasSpecVar.u8GreenValue[i]     = 128;
        pstSpecVar->stBasSpecVar.u8BlueValue[i]      = 128;
        pstSpecVar->stBasSpecVar.u16SplicerX[i]      = 0;
        pstSpecVar->stBasSpecVar.u16SplicerY[i]      = 0;
        pstSpecVar->stBasSpecVar.u16SplicerWidth[i]  = 0;
        pstSpecVar->stBasSpecVar.u16SplicerHeight[i] = 0;
        pstSpecVar->stBasSpecVar.bSplicerEnable[i]   = FALSE;
        pstSpecVar->stBasSpecVar.u16BorderX[i]       = 0;
        pstSpecVar->stBasSpecVar.u16BorderY[i]       = 0;
        pstSpecVar->stBasSpecVar.u16BorderWidth[i]   = 0;
        pstSpecVar->stBasSpecVar.u16BorderHeight[i]  = 0;
        pstSpecVar->stBasSpecVar.stSplicerGroup[0].bGroupTxEnable[i]   = FALSE;
        pstSpecVar->stBasSpecVar.stSplicerGroup[0].u8SplicerPosX[i]    = 0;
        pstSpecVar->stBasSpecVar.stSplicerGroup[0].u8SplicerPosY[i]    = 0;
        pstSpecVar->stBasSpecVar.stSplicerGroup[1].bGroupTxEnable[i]   = FALSE;
        pstSpecVar->stBasSpecVar.stSplicerGroup[1].u8SplicerPosX[i]    = 0;
        pstSpecVar->stBasSpecVar.stSplicerGroup[1].u8SplicerPosY[i]    = 0;
        pstSpecVar->stBasSpecVar.stAutoTimingTable[i].bEnable = FALSE;
        memset(&pstSpecVar->stBasSpecVar.stAutoTimingTable[i].stTimingVar,0,sizeof(stTimingTable_t));
    }
    pstSpecVar->stBasSpecVar.stSplicerGroup[0].u8SplicerWidth   = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[0].u8SplicerHeight  = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[1].u8SplicerWidth   = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[1].u8SplicerHeight  = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[0].u8BorderSizeWidth    = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[0].u8BorderSizeHeight   = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[1].u8BorderSizeWidth    = 0;
    pstSpecVar->stBasSpecVar.stSplicerGroup[1].u8BorderSizeHeight   = 0;

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(System_Default_Edid_table != NULL)
            memcpy(pstSpecVar->stBasSpecVar.u8RxEdid[i], System_Default_Edid_table, 256);
        else
            memcpy(pstSpecVar->stBasSpecVar.u8RxEdid[i], _6_1920x1080p60, 256);

        memset(pstSpecVar->stBasSpecVar.u8RxOsdStr[i],' ',15);
        pstSpecVar->stBasSpecVar.u8RxOsdStrLen[i]   = 0;
        pstSpecVar->stBasSpecVar.bRxOsdStrIsAlignRight[i] = FALSE;
        pstSpecVar->stBasSpecVar.bRxOsdStrEnable[i] = FALSE;
        pstSpecVar->stBasSpecVar.u16RxOsdStrX[i]    = 100;
        pstSpecVar->stBasSpecVar.u16RxOsdStrY[i]    = 100;
        pstSpecVar->stBasSpecVar.u8RxOsdStrColorR[i]= 0xFF;
        pstSpecVar->stBasSpecVar.u8RxOsdStrColorG[i]= 0x00;
        pstSpecVar->stBasSpecVar.u8RxOsdStrColorB[i]= 0x00;
        pstSpecVar->stBasSpecVar.u8InputPatternSta[i] = 0;
    }

    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
    pstSpecVar->stBasSpecVar.u8InRs232BaudrateID    = baud_rate_115200;//baud_rate_9600;
    pstSpecVar->stBasSpecVar.u8OutRs232BaudrateID   = baud_rate_115200;//baud_rate_9600;
    pstSpecVar->stBasSpecVar.u8UartMode             = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[0]    = 192;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[1]    = 168;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[2]    = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[3]    = 178;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[0]   = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[1]   = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[2]   = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[3]   = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[0]   = 192;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[1]   = 168;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[2]   = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[3]   = 1;
    pstSpecVar->stBasSpecVar.stIPVar.u16SrcPort     = 4001;
    pstSpecVar->stBasSpecVar.u8SplitterMode[0]      = 0;
    pstSpecVar->stBasSpecVar.u8SplitterMode[1]      = 0;
    pstSpecVar->stBasSpecVar.u8SplitterMode[2]      = 0;
    pstSpecVar->stBasSpecVar.u8SplitterMode[3]      = 0;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[0][0] = 0;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[0][1] = 1;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[0][2] = 2;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[0][3] = 3;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[1][0] = 0;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[1][1] = 1;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[1][2] = 2;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[1][3] = 3;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[2][0] = 0;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[2][1] = 1;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[2][2] = 2;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[2][3] = 3;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[3][0] = 0;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[3][1] = 1;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[3][2] = 2;
    pstSpecVar->stBasSpecVar.u8SplitterWinSrc[3][3] = 3;

    for(i=0;i<32;i++)
    {
        memset(&pstSpecVar->stBasSpecVar.stScene[i],0,sizeof(stSceneSaveVar_t));
        pstSpecVar->stBasSpecVar.stScenePollingVar.bPollingListEnable[i] = FALSE;
    }
    pstSpecVar->stBasSpecVar.stScenePollingVar.bPollingEnable    = FALSE;
    pstSpecVar->stBasSpecVar.stScenePollingVar.u16PollingTime    = 5;
}

void MUH1616S_INPUT_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
}

bool MUH1616S_INPUT_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        MUH1616S_INPUT_SpecBasicVarDefault();
        pstSpecVar->stBasSpecVar.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar));

        isSpecVarValid = FALSE;
    }

    if(pstSpecVar->stCustomEdidPre1.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre1,((uint8_t *)&pstSpecVar->stCustomEdidPre1.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre1)))
    {
        memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
        pstSpecVar->stCustomEdidPre1.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre1,((uint8_t *)&pstSpecVar->stCustomEdidPre1.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre1));

        isSpecVarValid = FALSE;
    }

    if(pstSpecVar->stCustomEdidPre2.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre2,((uint8_t *)&pstSpecVar->stCustomEdidPre2.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre2)))
    {
        memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
        pstSpecVar->stCustomEdidPre2.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre2,((uint8_t *)&pstSpecVar->stCustomEdidPre2.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre2));

        isSpecVarValid = FALSE;
    }

    if(pstSpecVar->stCustomEdidPre3.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre3,((uint8_t *)&pstSpecVar->stCustomEdidPre3.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre3)))
    {
        memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
        pstSpecVar->stCustomEdidPre3.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre3,((uint8_t *)&pstSpecVar->stCustomEdidPre3.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre3));

        isSpecVarValid = FALSE;
    }

    if(pstSpecVar->stCustomEdidPre4.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre4,((uint8_t *)&pstSpecVar->stCustomEdidPre4.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre4)))
    {
        memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
        pstSpecVar->stCustomEdidPre4.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre4,((uint8_t *)&pstSpecVar->stCustomEdidPre4.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre4));

        isSpecVarValid = FALSE;
    }

    return isSpecVarValid;
}

void SetUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8CheckSum = CRC8Calculate((uint8_t *)pstBasicVar, ((uint8_t *)&pstBasicVar->u8CheckSum-(uint8_t *)pstBasicVar));
    SaveUserDataToEep(eEepDataIndex_SpecBlock1,\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)),\
        (uint8_t *)pstBasicVar, sizeof(stBasicSpecVar_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void MUH1616S_INPUT_PrjStatePowerOnInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartCom_t pUarControltVar = &g_stUart2CommVar;
    pstUartCom_t pUarRemoteVar = &g_stUart1CommVar;

    System_Default_Edid_table = (iTE_u8 *)_6_1920x1080p60;

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = MUH1616S_INPUT_1msTimerIrq;
    pvvFuncMS1826UserFunctionInit = app_MUH1616S_INPUTMS1826UserInit;
    pvvFuncMS1826UserFunctionProcess = app_MUH1616S_INPUTMS1826UserProcess;
    pLT8644Process = app_MUH1616S_INPUTLT8644SwitchProcess;
    pSeriesLcdProcess = led_handle;
    LCD_LineRefresh("System Init...  ",LINE1);
    LCD_RecoverToDefault(20000);   //打开LCD背光倒计时20S
    mapp_LCDBacklightEnable();

    g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;
    pUarControltVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicVar->u8InRs232BaudrateID), DEF_UART_DATA_CONFIG);
    pUarRemoteVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicVar->u8OutRs232BaudrateID), DEF_UART_DATA_CONFIG);
    delay_ms(100);
    pstBasicVar->bPowerOnSta = TRUE;

    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
            g_stMUH1616S_INPUTSwitchTable.u8OutCh[i] = pstPrjSaveVar->stHdmiSel.u8SwChannel[i];
            g_stMUH1616S_INPUTSwitchTable.bUpdateFlag[i] = TRUE;
            g_stMUH1616S_INPUTSwitchTable.bCloseEnableFlag[i] = FALSE;
        }
    }
    if(pstBasicVar->stScenePollingVar.bPollingEnable)
        mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_ScenePolling,40*1000);
    g_bStartupOKFlag = FALSE;

    mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,1500);
    mapp_RevisionMake();
}

void MUH1616S_INPUT_LedWork_Loop(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    static bool sl_bLedSta = FALSE;
    if(!mapp_MUH1616S_INPUT_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_LedActive,500);

    if(pstBasicVar->bPowerOnSta)
    {
        if(sl_bLedSta)
        {
            sl_bLedSta = FALSE;
            led_on(led_name_ActiveLed);

            if(pstBasicVar->stScenePollingVar.bPollingEnable)    // scene polling led flash
                led_on(led_name_LedScene);
        }
        else
        {
            sl_bLedSta = TRUE;
            led_off(led_name_ActiveLed);

            if(pstBasicVar->stScenePollingVar.bPollingEnable)    // scene polling led flash
                led_off(led_name_LedScene);
        }
    }
    else
    {
        u8 i;
        for(i=led_name_LedInput1;i<led_name_End;i++)
        {
            led_off(led_name_ActiveLed+i);
        }

        if(sl_bLedSta)
        {
            sl_bLedSta = FALSE;
            led_on(led_name_ActiveLed);
        }
        else
        {
            sl_bLedSta = TRUE;
            led_off(led_name_ActiveLed);
        }
    }
}

// the status of led when running system
void MUH1616S_INPUT_FuncWork_LoopRunning(void)
{
    MUH1616S_INPUT_LedWork_Loop();
    app_MUH1616S_INPUTScenePollingFunction();
    app_MUH1616S_INPUTStartupLinkInfoGet();
}

void mapp_MUH1616S_INPUTStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char l_s8ResText[20];
    u8 i;

    USER_printf("\r\n");
    USER_printf("System Initializing...\r\n");
    USER_printf("\r\n");
    USER_printf("Initialization Finished!\r\n");
    USER_printf("%s %s\r\n",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
    for(i=0;i<Def_outport_num;i++)
    {
        USER_printf("out %d in %d\r\n",i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
    }
    for(i=0;i<Def_outport_num;i++)
    {
        memset(l_s8ResText,0,20);
        GetResolutionText(l_s8ResText,pstBasicVar->u8TxResolutionID[i]);
        USER_printf("out %d res %s\r\n",i+1,l_s8ResText);
    }
    USER_printf("system on\r\n");
    USER_printf("ok\r\n");
}

void app_MUH1616S_INPUTMS1826UserInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i,l_u8RxPort,l_u8TxPort,l_u8Dev;

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        l_u8RxPort = (u8)cg_u8ChannelTable[i][1];
        l_u8TxPort = (u8)cg_u8ChannelTable[i][0];
        l_u8Dev =   (u8)cg_u8ChannelTable[i][3];
//        app_MS1826Switch(0,l_u8RxPort,i);
        app_single_mode_rx_switch(l_u8Dev,l_u8RxPort, l_u8TxPort);
        dbg_printf("u8_Tx_audio_sel[%d] = %d\r\n",i,u8_Tx_audio_sel[l_u8Dev][l_u8TxPort]);
//        app_MS1826TxResolutionChange(l_u8Dev,l_u8RxPort,1);
        app_MS1826TxConfig(l_u8Dev,l_u8RxPort,1);

//        app_MS1826PictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Brightness[i],MS1826_ADJUST_BRIGHTNESS);
//        app_MS1826PictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Contrast[i],MS1826_ADJUST_CONTRAST);
//        app_MS1826PictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Saturation[i],MS1826_ADJUST_SATURATION);
//        app_MS1826PictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Hue[i],MS1826_ADJUST_HUE);
#if 0
        app_MS1826PictureColorConfig(i/4,,l_u8RxPort,
                            pstBasicVar->u8RedValue[i],
                            pstBasicVar->u8GreenValue[i],
                            pstBasicVar->u8BlueValue[i],
                            1);
#endif
        app_MS1826RxEdidUpdate(l_u8Dev,l_u8RxPort,(pu8)pstBasicVar->u8RxEdid[i],256);

        app_MS1826SetOsdString(l_u8Dev,l_u8TxPort,pstBasicVar->u8RxOsdStrLen[i],pstBasicVar->u8RxOsdStr[i],pstBasicVar->bRxOsdStrIsAlignRight[i]);
        app_MS1826SetUserOsdStringEnable(l_u8Dev,l_u8TxPort,pstBasicVar->bRxOsdStrEnable[i]);
        app_MS1826SetUserOsdStringPosition(l_u8Dev,l_u8TxPort,pstBasicVar->u16RxOsdStrX[i],pstBasicVar->u16RxOsdStrY[i]);
        app_MS1826SetUserStringColor(l_u8Dev,l_u8TxPort,
                pstBasicVar->u8RxOsdStrColorR[i],
                pstBasicVar->u8RxOsdStrColorG[i],
                pstBasicVar->u8RxOsdStrColorB[i]);
    }
}

void app_MUH1616S_INPUTMS1826UserProcess(void)
{
#if 1
    u8 i;
    stTimingTable_t l_stRxTming;

    for(g_u8MS1826DevIndex=0;g_u8MS1826DevIndex<DEV_NUM_MS1826;g_u8MS1826DevIndex++)
    {
        g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;

        for(i=0;i<4;i++)
        {
            if(g_stLinkSta.u8InLinkSta[i+g_u8MS1826DevIndex*4] != app_ms1826_get_rx_signal_sta(g_u8MS1826DevIndex,cg_u8ChannelTable[i+g_u8MS1826DevIndex*4][0]))
            {
                g_stLinkSta.u8InLinkSta[i+g_u8MS1826DevIndex*4] = app_ms1826_get_rx_signal_sta(g_u8MS1826DevIndex,cg_u8ChannelTable[i+g_u8MS1826DevIndex*4][0]);
                g_stLinkSta.bInLinkChangeFlag[i+g_u8MS1826DevIndex*4] = TRUE;
            }
            app_ms1826_get_rx_timing(g_u8MS1826DevIndex,cg_u8ChannelTable[i+g_u8MS1826DevIndex*4][0],&l_stRxTming);
            if((g_stLinkSta.stInTming[i+g_u8MS1826DevIndex*4].u16HActive != l_stRxTming.u16HActive)
                ||(g_stLinkSta.stInTming[i+g_u8MS1826DevIndex*4].u16VActive != l_stRxTming.u16VActive)
                ||(g_stLinkSta.stInTming[i+g_u8MS1826DevIndex*4].u8VClk != l_stRxTming.u8VClk)
                ||(g_stLinkSta.stInTming[i+g_u8MS1826DevIndex*4].u8Interlace != l_stRxTming.u8Interlace))
            {
                memcpy(&g_stLinkSta.stInTming[i+g_u8MS1826DevIndex*4],&l_stRxTming,sizeof(stTimingTable_t));
                g_stLinkSta.bInTimingChangeFlag[i+g_u8MS1826DevIndex*4] = TRUE;
            }
        }
    }
#endif
}

void app_MUH1616S_INPUTSetSplicerPixelDisplay(u8 u8Dev,u8 u8Tx,u16 u16X,u16 u16Y,u16 u16Width,u16 u16Height)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Value[10];
#if 0
    u8 l_u8Port = (u8)cg_u8ChannelTable[u8Tx][0];

    if(u16X%8)
    {
        u16X -= u16X%8;
    }
    if(u16Y%8)
    {
        u16Y -= u16Y%8;
    }
    if(u16Width%8)
    {
        u16Width -= u16Width%8;
    }
    if(u16Height%8)
    {
        u16Height -= u16Height%8;
    }

    app_MS1826SplicerEnable(u8Dev,l_u8Port,pstBasicVar->bSplicerEnable[u8Tx]);
    app_MS1826SplicerConfig(u8Dev,l_u8Port,u16X,u16Y,u16Width,u16Height);
#endif

    l_u8Value[0] = u8Tx+1;
    l_u8Value[1] = pstBasicVar->bSplicerEnable[u8Tx];
    l_u8Value[2] = u16X&0xFF;
    l_u8Value[3] = (u16X>>8)&0xFF;
    l_u8Value[4] = u16Y&0xFF;
    l_u8Value[5] = (u16Y>>8)&0xFF;
    l_u8Value[6] = u16Width&0xFF;
    l_u8Value[7] = (u16Width>>8)&0xFF;
    l_u8Value[8] = u16Height&0xFF;
    l_u8Value[9] = (u16Height>>8)&0xFF;
    mapp_MUH1616S_InputSendCmdToOutput(eMcuReg_TxWinConfig,l_u8Value,10);
}

void app_MUH1616S_INPUTSetSplicerDisplay(u8 u8Dev,u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
//    stTimingTable_t l_stRxTming;
    u16 l_u16HDisp;
    u16 l_u16VDisp;
    u16 l_u16X;
    u16 l_u16Y;
    u16 l_u16W;
    u16 l_u16H;
    if(u8Index>=(u8H*u8W)||u8H==0||u8W==0)
        return;

//    app_ms1826_get_rx_timing(u8Dev,cg_u8ChannelTable[u8Tx][1],&l_stRxTming);

    l_u16HDisp = app_ms1826_get_display_horizontal(u8Dev,cg_u8ChannelTable[u8Tx][1]);//l_stRxTming.u16HActive;
    l_u16VDisp = app_ms1826_get_display_vertical(u8Dev,cg_u8ChannelTable[u8Tx][1]);//l_stRxTming.u16VActive;

    l_u16X  = (l_u16HDisp/u8H)*(u8Index%u8H);
    l_u16Y  = (l_u16VDisp/u8W)*(u8Index/u8W);
    l_u16W  = l_u16HDisp/u8H;
    l_u16H  = l_u16VDisp/u8W;
    pstBasicVar->u16SplicerX[u8Tx]      = l_u16X;
    pstBasicVar->u16SplicerY[u8Tx]      = l_u16Y;
    pstBasicVar->u16SplicerWidth[u8Tx]  = l_u16W;
    pstBasicVar->u16SplicerHeight[u8Tx] = l_u16H;
    pstBasicVar->bSplicerEnable[u8Tx]   = TRUE;
    dbg_printf("Splicer x = %d\r\n",pstBasicVar->u16SplicerX[u8Tx]);
    dbg_printf("Splicer y = %d\r\n",pstBasicVar->u16SplicerY[u8Tx]);
    dbg_printf("Splicer w = %d\r\n",pstBasicVar->u16SplicerWidth[u8Tx]);
    dbg_printf("Splicer h = %d\r\n",pstBasicVar->u16SplicerHeight[u8Tx]);

    l_u16X += pstBasicVar->u16BorderX[u8Tx];
    l_u16Y += pstBasicVar->u16BorderY[u8Tx];
    l_u16W -= pstBasicVar->u16BorderWidth[u8Tx];
    l_u16H -= pstBasicVar->u16BorderHeight[u8Tx];
    dbg_printf("Border x = %d\r\n",pstBasicVar->u16BorderX[u8Tx]);
    dbg_printf("Border y = %d\r\n",pstBasicVar->u16BorderY[u8Tx]);
    dbg_printf("Border w = %d\r\n",pstBasicVar->u16BorderWidth[u8Tx]);
    dbg_printf("Border h = %d\r\n",pstBasicVar->u16BorderHeight[u8Tx]);

    app_MUH1616S_INPUTSetSplicerPixelDisplay(u8Dev,u8Tx,l_u16X,l_u16Y,l_u16W,l_u16H);

    SetUserData_SpecBasicVar();
}

void app_MUH1616S_INPUTSetSplicerBorder(u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index,u16 u16BorderWidth,u16 u16BorderHeight)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(u8Index==0)  // left up
    {
        pstBasicVar->u16BorderX[u8Tx]       = 0;
        pstBasicVar->u16BorderY[u8Tx]       = 0;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if(u8Index==(u8H*u8W-1))   // right down
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if(((u8Index%u8H)==0)&&((u8Index/u8W)==(u8W-1)))   // left down
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = 0;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if(((u8Index%u8H)==(u8H-1))&&((u8Index/u8W)==0))    // right up
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = 0;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if((u8Index%u8H)==0)   // left side
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = 0;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight<<1;
    }
    else if((u8Index/u8W)==0)   // up side
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = 0;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth<<1;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if((u8Index%u8H)==(u8H-1)) // right side
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight<<1;
    }
    else if((u8Index/u8W)==(u8W-1)) // down side
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth<<1;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else
    {
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth<<1;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight<<1;
    }
    SetUserData_SpecBasicVar();
}

void app_MUH1616S_INPUTPictureAdjust(u8 u8Dev,u8 u8Tx,u8 u8Value,u8 u8Mode)
{
    app_MS1826PictureAdjust(u8Dev,u8Tx,u8Value+78,u8Mode);
}

void app_MUH1616S_INPUTSourceSwitch(u8 u8Out,u8 u8In)
{
    g_stMUH1616S_INPUTSwitchTable.u8OutCh[u8Out] = u8In;
    g_stMUH1616S_INPUTSwitchTable.bUpdateFlag[u8Out] = TRUE;
    g_stMUH1616S_INPUTSwitchTable.bCloseEnableFlag[u8Out] = FALSE;
    g_u16SrcChgTxFreezeState |= (1<<u8Out);
}

void app_MUH1616S_INPUTFreezeOutPort(void)
{
    u8 l_u8TxPath[3]={0,0,0};

    l_u8TxPath[0] = 1;  // mode
    l_u8TxPath[1] = g_u16SrcChgTxFreezeState&0xFF;  // 8~1
    l_u8TxPath[2] = (g_u16SrcChgTxFreezeState>>8)&0xFF;  // 16~9

    mapp_MUH1616S_InputSendCmdToOutput(eMcuReg_SwitchFreeze,l_u8TxPath,3);
    g_u16SrcChgTxFreezeState = 0;
}

void app_MUH1616S_INPUTUnfreezeOutPort(void)
{
    u8 l_u8TxPath[3]={0,0,0};

    l_u8TxPath[0] = 2;  // mode
    l_u8TxPath[1] = 0xFF;  // 8~1
    l_u8TxPath[2] = 0xFF;  // 16~9
    mapp_MUH1616S_InputSendCmdToOutput(eMcuReg_SwitchFreeze,l_u8TxPath,3);
    g_u16SrcChgTxFreezeState = 0;
}

void app_MUH1616S_INPUTLT8644SwitchProcess(void)
{
    int i;
    uint8_t in,ch=0;
    // map 对应关系

    const u8 ch_in[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//芯片上输出通道选择的输入源端口�?
    const u8 ch_out[16]= {0x57,0x57,0x58,0x58,0x59,0x59,0x5a,0x5a, 0x5b,0x5b,0x5c,0x5c,0x5d,0x5d,0x5e,0x5e};//4位的值表示该通道的输入端口号为哪一�?
    const u8 ch_out_en[16]={0x72,0x72,0x72,0x72,0x72,0x72,0x72,0x72, 0x71,0x71,0x71,0x71,0x71,0x71,0x71,0x71};//控制0-15个输出端口使�?
    const u8 Ka_ch_adjust_in[16]= { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11, 12, 13,14,15};
    const u8 Ka_ch_adjust_out[16]={ 0, 3, 2, 1, 4, 7, 6, 5, 8,11,10, 9, 12, 15,14,13};
//    const u8 Ka_ch_adjust_in[16]= { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};
//    const u8 Ka_ch_adjust_out[16]={ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15};
    static u8 ch_Out_InputNumBack[16]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    unsigned char l_u8Data=0;
    static bool  l_bEnUpdateFlag = FALSE;
    static u8 sl_u16EnSta=0;
//    u8 l_u8TxPort,l_u8Dev;

    taskENTER_CRITICAL();

    for(i=0; i<Def_outport_num; i++)
    {
        if((TRUE == g_stMUH1616S_INPUTSwitchTable.bUpdateFlag[i])/*&&(g_u8SrcChgTxFreezeState==1)*/)
        {
            in=Ka_ch_adjust_in[g_stMUH1616S_INPUTSwitchTable.u8OutCh[i]];
            ch = Ka_ch_adjust_out[i];
            dbg_printf("In[%d] Out[%d]\r\n",in,ch);
            if(g_stMUH1616S_INPUTSwitchTable.bCloseEnableFlag[i] == FALSE)
            {
                if(ch%2)
                {
                    l_u8Data =  ((ch_in[in])<<4) ;
                    l_u8Data |= ch_Out_InputNumBack[ch-1]&0x0F;
                }
                else
                {
                    l_u8Data = ch_Out_InputNumBack[ch+1]<<4;
                    l_u8Data |= ch_in[in];
                }

                write_LT8644(0,ch_out[ch],l_u8Data);
                write_LT8644(1,ch_out[ch],l_u8Data);
                write_LT8644(2,ch_out[ch],l_u8Data);
                write_LT8644(3,ch_out[ch],l_u8Data);

                dbg_printf("8644 ch_out[0x%02x] l_u8Data[0x%02x]\r\n",ch_out[ch],l_u8Data);
                //delay_us(20);
                sl_u16EnSta &= ~(BIT0 << ch);
                write_LT8644(0,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                write_LT8644(1,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                write_LT8644(2,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                write_LT8644(3,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                //delay_us(20);
                ch_Out_InputNumBack[ch] = ch_in[in];//备份这个输出通道下对应的输入端口号
            }
            else
            {
                sl_u16EnSta |= (BIT0 << ch);
                write_LT8644(0,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                write_LT8644(1,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                write_LT8644(2,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                write_LT8644(3,ch_out_en[ch],(uint8_t)((ch>7)?((sl_u16EnSta&0xFF00)>>8):(sl_u16EnSta&0x00FF)));
                //delay_us(20);
            }
            l_bEnUpdateFlag = TRUE;
            g_stMUH1616S_INPUTSwitchTable.bUpdateFlag[i] = FALSE;
        }
    }

    if(l_bEnUpdateFlag&&g_bMessageFreezeOkFlag)
    {
//        app_MUH1616S_INPUTFreezeOutPort();
//        delay_us(150000);
        LT8644UpdatePin();                    //updata刷新数据
//        delay_us(50000);
        app_MUH1616S_INPUTUnfreezeOutPort();
        l_bEnUpdateFlag = FALSE;
        g_bMessageFreezeOkFlag = FALSE;
    }
    taskEXIT_CRITICAL();
}

void app_MUH1616S_INPUTSceneSave(u8 u8SceneID)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    if(u8SceneID<32)
    {
        pstBasicVar->stScene[u8SceneID].u8SceneSaveFlag = TRUE;
        for(i=0;i<Def_outport_num;i++)
        {
            pstBasicVar->stScene[u8SceneID].u16SplicerX[i]      = pstBasicVar->u16SplicerX[i];
            pstBasicVar->stScene[u8SceneID].u16SplicerY[i]      = pstBasicVar->u16SplicerY[i];
            pstBasicVar->stScene[u8SceneID].u16SplicerWidth[i]  = pstBasicVar->u16SplicerWidth[i];
            pstBasicVar->stScene[u8SceneID].u16SplicerHeight[i] = pstBasicVar->u16SplicerHeight[i];
            pstBasicVar->stScene[u8SceneID].bSplicerEnable[i]   = pstBasicVar->bSplicerEnable[i];
            pstBasicVar->stScene[u8SceneID].u16BorderX[i]       = pstBasicVar->u16BorderX[i];
            pstBasicVar->stScene[u8SceneID].u16BorderY[i]       = pstBasicVar->u16BorderY[i];
            pstBasicVar->stScene[u8SceneID].u16BorderWidth[i]   = pstBasicVar->u16BorderWidth[i];
            pstBasicVar->stScene[u8SceneID].u16BorderHeight[i]  = pstBasicVar->u16BorderHeight[i];
        }

        // copy splicer group var
        memcpy(&pstBasicVar->stScene[g_u8SceneStartIndex].stGroupVar[0],&pstBasicVar->stSplicerGroup[0],sizeof(stSplicerGroupVar_t));
        memcpy(&pstBasicVar->stScene[g_u8SceneStartIndex].stGroupVar[1],&pstBasicVar->stSplicerGroup[1],sizeof(stSplicerGroupVar_t));
        // copy source channel
        memcpy(pstBasicVar->stScene[u8SceneID].u8VideoCh,pstPrjSaveVar->stHdmiSel.u8SwChannel,Def_outport_num);
        // save value
        SetUserData_HdmiSel();
        SetUserData_SpecBasicVar();
    }

}

void app_MUH1616S_INPUTSceneCall(u8 u8SceneID)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i,l_u8TxPort,l_u8Dev;
    bool l_bSwSplicerFlag[Def_outport_num];

    memset(l_bSwSplicerFlag,FALSE,Def_outport_num);
    if(u8SceneID<32)
    {
        if(pstBasicVar->stScene[u8SceneID].u8SceneSaveFlag == TRUE)
        {
            // copy source channel
            memcpy(pstPrjSaveVar->stHdmiSel.u8SwChannel,pstBasicVar->stScene[u8SceneID].u8VideoCh,Def_outport_num);
            // copy splicer config var
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u16SplicerX[i]     = pstBasicVar->stScene[u8SceneID].u16SplicerX[i];
                pstBasicVar->u16SplicerY[i]     = pstBasicVar->stScene[u8SceneID].u16SplicerY[i];
                pstBasicVar->u16SplicerWidth[i] = pstBasicVar->stScene[u8SceneID].u16SplicerWidth[i];
                pstBasicVar->u16SplicerHeight[i]= pstBasicVar->stScene[u8SceneID].u16SplicerHeight[i];
                pstBasicVar->u16BorderX[i]      = pstBasicVar->stScene[u8SceneID].u16BorderX[i];
                pstBasicVar->u16BorderY[i]      = pstBasicVar->stScene[u8SceneID].u16BorderY[i];
                pstBasicVar->u16BorderWidth[i]  = pstBasicVar->stScene[u8SceneID].u16BorderWidth[i];
                pstBasicVar->u16BorderHeight[i] = pstBasicVar->stScene[u8SceneID].u16BorderHeight[i];
                if((pstBasicVar->bSplicerEnable[i] != pstBasicVar->stScene[u8SceneID].bSplicerEnable[i])
                        ||(pstBasicVar->stScene[u8SceneID].bSplicerEnable[i] == TRUE))
                {
                    pstBasicVar->bSplicerEnable[i]  = pstBasicVar->stScene[u8SceneID].bSplicerEnable[i];
                    l_bSwSplicerFlag[i] = TRUE;
                }
            }
            // copy splicer group var
            memcpy(&pstBasicVar->stSplicerGroup[0],&pstBasicVar->stScene[g_u8SceneStartIndex].stGroupVar[0],sizeof(stSplicerGroupVar_t));
            memcpy(&pstBasicVar->stSplicerGroup[1],&pstBasicVar->stScene[g_u8SceneStartIndex].stGroupVar[1],sizeof(stSplicerGroupVar_t));

            for(i=0;i<Def_outport_num;i++)
            {
                // switch source
                app_MUH1616S_INPUTSourceSwitch(i,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]);

                // Splicer config
                l_u8TxPort = (u8)cg_u8ChannelTable[i][0];
                l_u8Dev  =   (u8)cg_u8ChannelTable[i][3];
                if(l_bSwSplicerFlag[i])
                {
                    app_MUH1616S_INPUTSetSplicerPixelDisplay(l_u8Dev,l_u8TxPort,
                            pstBasicVar->u16SplicerX[i]+pstBasicVar->u16BorderX[i],
                            pstBasicVar->u16SplicerY[i]+pstBasicVar->u16BorderY[i],
                            pstBasicVar->u16SplicerWidth[i]-pstBasicVar->u16BorderWidth[i],
                            pstBasicVar->u16SplicerHeight[i]-pstBasicVar->u16BorderHeight[i]);
                }
            }
            app_MUH1616S_INPUTFreezeOutPort();
            // save value
            SetUserData_HdmiSel();
            SetUserData_SpecBasicVar();
        }
    }
}

// 场景轮询;
void app_MUH1616S_INPUTScenePollingFunction(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->stScenePollingVar.bPollingEnable)
    {
        u8 i,l_u8TxPort,l_u8Dev,l_u8SceneID;
        bool l_bSwSplicerFlag[Def_outport_num];
        char l_cDisplayText[16];

        if(!mapp_MUH1616S_INPUT_PlatformTimerExpired(emUserTimer_ScenePolling))
            return;
        mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_ScenePolling,pstBasicVar->stScenePollingVar.u16PollingTime*1000);
        memset(l_bSwSplicerFlag,FALSE,Def_outport_num);
        for(l_u8SceneID=0;l_u8SceneID<32;l_u8SceneID++)
        {
            g_u8SceneStartIndex = (g_u8SceneStartIndex+1)%32;  // save current index
            if(pstBasicVar->stScenePollingVar.bPollingListEnable[g_u8SceneStartIndex])
            {
                if(pstBasicVar->stScene[g_u8SceneStartIndex].u8SceneSaveFlag == TRUE)
                {
                    // copy source channel
                    memcpy(pstPrjSaveVar->stHdmiSel.u8SwChannel,pstBasicVar->stScene[g_u8SceneStartIndex].u8VideoCh,Def_outport_num);
                    // copy splicer config var
                    for(i=0;i<Def_outport_num;i++)
                    {
                        pstBasicVar->u16SplicerX[i]     = pstBasicVar->stScene[g_u8SceneStartIndex].u16SplicerX[i];
                        pstBasicVar->u16SplicerY[i]     = pstBasicVar->stScene[g_u8SceneStartIndex].u16SplicerY[i];
                        pstBasicVar->u16SplicerWidth[i] = pstBasicVar->stScene[g_u8SceneStartIndex].u16SplicerWidth[i];
                        pstBasicVar->u16SplicerHeight[i]= pstBasicVar->stScene[g_u8SceneStartIndex].u16SplicerHeight[i];
                        pstBasicVar->u16BorderX[i]      = pstBasicVar->stScene[g_u8SceneStartIndex].u16BorderX[i];
                        pstBasicVar->u16BorderY[i]      = pstBasicVar->stScene[g_u8SceneStartIndex].u16BorderY[i];
                        pstBasicVar->u16BorderWidth[i]  = pstBasicVar->stScene[g_u8SceneStartIndex].u16BorderWidth[i];
                        pstBasicVar->u16BorderHeight[i] = pstBasicVar->stScene[g_u8SceneStartIndex].u16BorderHeight[i];
                        if((pstBasicVar->bSplicerEnable[i] != pstBasicVar->stScene[g_u8SceneStartIndex].bSplicerEnable[i])
                            /*||(pstBasicVar->stScene[g_u8SceneStartIndex].bSplicerEnable[i] == TRUE)*/)
                        {
                            pstBasicVar->bSplicerEnable[i]  = pstBasicVar->stScene[g_u8SceneStartIndex].bSplicerEnable[i];
                            l_bSwSplicerFlag[i] = TRUE;
                        }
                    }
                    // copy splicer group var
                    memcpy(&pstBasicVar->stSplicerGroup[0],&pstBasicVar->stScene[g_u8SceneStartIndex].stGroupVar[0],sizeof(stSplicerGroupVar_t));
                    memcpy(&pstBasicVar->stSplicerGroup[1],&pstBasicVar->stScene[g_u8SceneStartIndex].stGroupVar[1],sizeof(stSplicerGroupVar_t));

                    for(i=0;i<Def_outport_num;i++)
                    {
                        // switch source
                        app_MUH1616S_INPUTSourceSwitch(i,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]);

                        // Splicer config
                        l_u8TxPort = (u8)cg_u8ChannelTable[i][0];
                        l_u8Dev  =   (u8)cg_u8ChannelTable[i][3];
                        if(l_bSwSplicerFlag[i])
                        {
                            app_MUH1616S_INPUTSetSplicerPixelDisplay(l_u8Dev,l_u8TxPort,
                                    pstBasicVar->u16SplicerX[i]+pstBasicVar->u16BorderX[i],
                                    pstBasicVar->u16SplicerY[i]+pstBasicVar->u16BorderY[i],
                                    pstBasicVar->u16SplicerWidth[i]-pstBasicVar->u16BorderWidth[i],
                                    pstBasicVar->u16SplicerHeight[i]-pstBasicVar->u16BorderHeight[i]);
                        }
                    }
                    app_MUH1616S_INPUTFreezeOutPort();
                    // save value
                    SetUserData_HdmiSel();
                    SetUserData_SpecBasicVar();

                    USER_printf("scene polling %d called\r\n",g_u8SceneStartIndex+1);
                    USER_printf("ok\r\n");

                    strncpy(l_cDisplayText,"Scene Polling   ",16);
                    LCD_LineRefresh(l_cDisplayText,LINE1);
                    LCD_LineRefresh((char*)LcdNone,LINE2);
                    strncpy(l_cDisplayText,"Call            ",16);
                    l_cDisplayText[5] = (g_u8SceneStartIndex+1)/10+0x30;
                    l_cDisplayText[6] =(g_u8SceneStartIndex+1)%10+0x30;
                    LCD_LineRefresh(l_cDisplayText,LINE3);
                    LCD_LineRefresh((char*)LcdNone,LINE4);
                    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
                    break;
                }
            }
        }
    }
}

#if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
/******************************************************
///Function Name:   app_MUH1616S_INPUTReadEdidDescriptor1Timing
///Description:     Config output resolution
///Input:           pu8EdidBuf(Edid 数据)
///Output:          pstEdidRes(提取的EDID分辨率信息)
///Return:          NULL
///Others:          NULL
******************************************************/
void app_MUH1616S_INPUTReadEdidDescriptor1Timing(pu8 pu8EdidBuf,pstTimingTable_t pstEdidRes)
{
    u16 l_u16HBlank;
    u16 l_u16VBlank;

    l_u16HBlank             = (pu8EdidBuf[0x39]|((pu8EdidBuf[0x3A]&0x0F)<<8));
    pstEdidRes->u8HPol      = (pu8EdidBuf[0x47]&BIT1)>>1;
    pstEdidRes->u16HFP      = (pu8EdidBuf[0x3E]|(((pu8EdidBuf[0x41]&0xC0)>>6)<<8));
    pstEdidRes->u16HSync    = (pu8EdidBuf[0x3F]|(((pu8EdidBuf[0x41]&0x30)>>4)<<8));
    pstEdidRes->u16HBP      = l_u16HBlank-pstEdidRes->u16HSync-pstEdidRes->u16HFP;
    pstEdidRes->u16HActive  = (pu8EdidBuf[0x38]|((pu8EdidBuf[0x3A]>>4)<<8));
    pstEdidRes->u16HTotal   = pstEdidRes->u16HActive+l_u16HBlank;
    l_u16VBlank             = (((pu8EdidBuf[0x3D]&0x0F)<<8)|(pu8EdidBuf[0x3C]));
    pstEdidRes->u8VPol      = (pu8EdidBuf[0x47]&BIT2)>>2;
    pstEdidRes->u16VFP      = (((pu8EdidBuf[0x40]&0xF0)>>4)|(((pu8EdidBuf[0x41]&0x0C)>>2)<<8));
    pstEdidRes->u16VSync    = ((pu8EdidBuf[0x40]&0x0F)|((pu8EdidBuf[0x41]&0x03)<<8));
    pstEdidRes->u16VBP      = l_u16VBlank-pstEdidRes->u16VFP-pstEdidRes->u16VSync;
    pstEdidRes->u16VActive  = ((((pu8EdidBuf[0x3D]&0xF0)>>4)<<8)|(pu8EdidBuf[0x3B]));
    pstEdidRes->u16VTotal   = pstEdidRes->u16VActive + l_u16VBlank;
    pstEdidRes->u8Vic       = 0;
    pstEdidRes->u16PClk     = (pu8EdidBuf[0x36]|(pu8EdidBuf[0x37]<<8));
    pstEdidRes->u8VClk      = pstEdidRes->u16PClk*10000/(pstEdidRes->u16HTotal)/(pstEdidRes->u16VTotal);
    pstEdidRes->u8Interlace = 0;

    #if 1
    dbg_printf("u8HPol = %d\r\n",pstEdidRes->u8HPol);
    dbg_printf("u16HFP = %d\r\n",pstEdidRes->u16HFP);
    dbg_printf("u8HSync = %d\r\n",pstEdidRes->u16HSync);
    dbg_printf("u16HBP = %d\r\n",pstEdidRes->u16HBP);
    dbg_printf("u16HActive = %d\r\n",pstEdidRes->u16HActive);
    dbg_printf("u16HTotal = %d\r\n",pstEdidRes->u16HTotal);
    dbg_printf("u8VPol = %d\r\n",pstEdidRes->u8VPol);
    dbg_printf("u16VFP = %d\r\n",pstEdidRes->u16VFP);
    dbg_printf("u8VSync = %d\r\n",pstEdidRes->u16VSync);
    dbg_printf("u16VBP = %d\r\n",pstEdidRes->u16VBP);
    dbg_printf("u16VActive = %d\r\n",pstEdidRes->u16VActive);
    dbg_printf("u16VTotal = %d\r\n",pstEdidRes->u16VTotal);
    dbg_printf("u8Vic = %d\r\n",pstEdidRes->u8Vic);
    dbg_printf("u8VClk = %d\r\n",pstEdidRes->u8VClk);
    dbg_printf("u16PClk = %d\r\n",pstEdidRes->u16PClk);
    #endif
}

/***********************************************************************
 * FunName : app_MUH1616S_INPUTAutoResolutionConfig
 * Writer  : wf8421
 * Date    : 2023-08-25 16:40
 * Description: Read tx edid to config resolution
 * Parameter: u8Tx - output port
 * Return  : None
 * History :
 ***********************************************************************/
void app_MUH1616S_INPUTAutoResolutionConfig(u8 u8Tx)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    stTimingTable_t l_stTiming;
    u8 l_u8RealPort,l_u8Dev;
    u8 l_u8EdidBuf[256];
    bool l_bEdidSta = FALSE;

    if(pstBasicVar->stAutoTimingTable[u8Tx].bEnable)
    {
        l_u8RealPort = cg_u8ChannelTable[u8Tx][0]+1;
        l_u8Dev = cg_u8ChannelTable[u8Tx][3];
        app_ms1826_get_tx_edid_data(l_u8Dev,l_u8RealPort,l_u8EdidBuf);
        l_bEdidSta = mapp_EdidLimitTo4K30(l_u8EdidBuf,l_u8EdidBuf,256,FALSE,TRUE);
        if(l_bEdidSta)
        {
            app_MUH1616S_INPUTReadEdidDescriptor1Timing(l_u8EdidBuf,&l_stTiming);
            if(l_stTiming.u16PClk <= 30000)
            {
                app_MS1826SetUserTiming(l_u8Dev,l_u8RealPort-1,&l_stTiming);
                memcpy(&pstBasicVar->stAutoTimingTable[u8Tx].stTimingVar,&l_stTiming,sizeof(stTimingTable_t));
                SetUserData_SpecBasicVar();
            }
            else
            {
                dbg_printf("pclk out of range, change to default resolution\r\n");
                app_MS1826TxResolutionChange(l_u8Dev,l_u8RealPort-1,emRes_1920x1080_60);
            }
        }
        else
        {
            dbg_printf("edid error, change to default resolution\r\n");
            app_MS1826TxResolutionChange(l_u8Dev,l_u8RealPort-1,emRes_1920x1080_60);
        }
    }
}

/***********************************************************************
 * FunName : app_MUH1616S_INPUTAutoResolutionProcess
 * Writer  : wf8421
 * Date    : 2023-08-25 17:02
 * Description: auto config resolution process
 * Parameter: None
 * Return  : None
 * History :
 ***********************************************************************/
void app_MUH1616S_INPUTAutoResolutionProcess(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i;
    static u8 sl_u8PreLinkSta[Def_outport_num] = {0,0,0,0,0,0,0,0};

    for(i=0;i<Def_outport_num;i++)
    {
        if(pstBasicVar->stAutoTimingTable[i].bEnable)
        {
            if(g_stLinkSta.u8OutLinkSta[i]!=sl_u8PreLinkSta[i])
            {
                sl_u8PreLinkSta[i] = g_stLinkSta.u8OutLinkSta[i];
                if(!g_stLinkSta.u8OutLinkSta[i])
                {
                    continue;
                }

                app_MUH1616S_INPUTAutoResolutionConfig(i);
            }
        }
    }
}
#endif

void mapp_SendCmdToDevice(u8 u8DevAddr,u8 u8Cmd,pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart4CommVar;
    u8 l_u8SendValue[1100];
    u16 i=0;
    u16 l_u16index=0;

    memset(l_u8SendValue,0,1100);
    l_u8SendValue[l_u16index++] = 0xFF;
    l_u8SendValue[l_u16index++] = 0xFF;
    l_u8SendValue[l_u16index++] = 0xA5;
    l_u8SendValue[l_u16index++] = u8DevAddr;
    l_u8SendValue[l_u16index++] = u8Cmd;
    l_u8SendValue[l_u16index++] = u16Len&0xFF;
    l_u8SendValue[l_u16index++] = (u16Len>>8)&0xFF;
    for(i=0;i<u16Len;i++)
    {
        l_u8SendValue[l_u16index+i] = pu8Data[i];
    }

    pUartCommVar->SendBytes(l_u8SendValue,l_u16index+u16Len,FALSE);
#if 0
    if(g_bEnableDbgFlag)
    {
        pstUartCom_t pUartDbgVar = &g_stUart3CommVar;
        pUartDbgVar->SendBytes(l_u8SendValue,l_u16index+u16Len,FALSE);
    }
#endif
}

void mapp_MUH1616S_INPUTSendDataToOutBoard(pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart4CommVar;

    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
}

void mapp_MUH1616S_InputSendCmdToOutput(u8 u8cmd,u8 *pu8Data, u16 u16Len)
{
    mapp_SendCmdToDevice(0x81,u8cmd,pu8Data,u16Len);
    if((u8cmd != eCheck_GetTxLinkSta)&&(u8cmd != eCheck_GetTxVersion)&&(g_bStartupOKFlag))
    {
        mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,3000);
    }
}

void app_MUH1616S_INPUTStartupLinkInfoGet(void)
{
    static eCheckCardInfoType_t sl_eCheckIndex = eCheck_GetTxVersion;

    if(!mapp_MUH1616S_INPUT_PlatformTimerExpired(emUserTimer_StartupLinkInfo))
        return;
    switch(sl_eCheckIndex)
    {
        case eCheck_GetTxVersion:
            {
                u8 l_u8Value[2];

                l_u8Value[0] = 01;
                g_bStartupOKFlag = TRUE;
                mapp_MUH1616S_InputSendCmdToOutput(eMcuReg_Version,l_u8Value,1);
                sl_eCheckIndex = eCheck_GetTxLinkSta;
                mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,500);
            }
            break;
        case eCheck_GetTxLinkSta:
            {
                u8 l_u8Value[2];

                g_bStartupOKFlag = TRUE;
                l_u8Value[0] = 01;
                mapp_MUH1616S_InputSendCmdToOutput(eMcuReg_LinkSta,l_u8Value,1);
                sl_eCheckIndex = eCheck_ReportLinkStaChange;
                mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,100);
            }
            break;

        case eCheck_ReportLinkStaChange:
            {
                sl_eCheckIndex = eCheck_ReportInputResolutionChange;
                app_MUH1616S_INPUTCheckLinkStatusChangeProcess();
                mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,500);
            }
            break;
        case eCheck_ReportInputResolutionChange:
            {
                sl_eCheckIndex = eCheck_GetTxLinkSta;
                app_MUH1616S_INPUTCheckInputResolutionChangeProcess();
                mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,2500);
            }
            break;

        default:
            break;
    }
}

void app_MUH1616S_INPUTCheckLinkStatusChangeProcess(void)
{
    u8 i;
    bool l_bSendLinkSta = FALSE;

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(g_stLinkSta.bInLinkChangeFlag[i])
        {
            g_stLinkSta.bInLinkChangeFlag[i] = FALSE;
            l_bSendLinkSta = TRUE;
        }
    }

    for(i=0;i<Def_outport_num;i++)
    {
        if(g_stLinkSta.bOutLinkChangeFlag[i])
        {
            g_stLinkSta.bOutLinkChangeFlag[i] = FALSE;
            l_bSendLinkSta = TRUE;
        }
    }

    if(l_bSendLinkSta)
    {
        l_bSendLinkSta = FALSE;
        USER_printf("ch  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16\r\n");
        USER_printf("in  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c\r\n",
                    g_stLinkSta.u8InLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[3]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[4]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[5]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[6]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[7]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[8]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[9]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[10]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[11]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[12]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[13]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[14]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[15]?'Y':'N');

        USER_printf("out %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c\r\n",
                    g_stLinkSta.u8OutLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[3]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[4]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[5]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[6]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[7]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[8]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[9]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[10]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[11]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[12]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[13]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[14]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[15]?'Y':'N');
        USER_printf("ok\r\n");
    }

}

void app_MUH1616S_INPUTCheckInputResolutionChangeProcess(void)
{
    u8 i;

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(g_stLinkSta.bInTimingChangeFlag[i])
        {
            g_stLinkSta.bInTimingChangeFlag[i] = FALSE;
            USER_printf("in %d res %dx%d%s@%dHz\r\n",i+1,g_stLinkSta.stInTming[i].u16HActive,
                        g_stLinkSta.stInTming[i].u8Interlace==INTR?(g_stLinkSta.stInTming[i].u16VActive*2):(g_stLinkSta.stInTming[i].u16VActive),
                        g_stLinkSta.stInTming[i].u8Interlace==INTR?"I":"",
                        g_stLinkSta.stInTming[i].u8VClk);
            USER_printf("ok\r\n");
        }
    }
}

#define DM_FW_BYTES_MIX         (15*1024)
#define DM_FW_BYTES_MAX         (192*1024)  //192KB
#define DM_FW_DATA_ADDR         0x8040000
#define DM_FW_FLAG_ADDR         0x8070000
#define PAGE_SIZE               0x800

#define DM_FW_FLAG_INVALID      0x44332211
#define DM_FW_FLAG_VALID        0x11223344
#define DM_FW_SIZE_ADDR         (DM_FW_FLAG_ADDR+4)

typedef struct _stUpdateVar_t_
{
    u8 u8UpdateMode;
    u8 u8UpdateFlag;
    u32 u32TotalBytes;
    u32 u32PageIndex;
    u16 u16PackIndex;
    u8 u8LastData[1024];
}stUpdateVar_t;

stUpdateVar_t g_stUpdateVar;

uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len)
{
    uint16_t i,j=0;
    fmc_state_enum f_sta;
    uint32_t* buff;
    image_header_t packet;

    static uint8_t LeftBytesTab[4]={0};
    static uint8_t LeftBytes=0;
//    static uint8_t frist_packet=1;

    switch(step)
    {
        case 0:
//            dbg_printf("iap updata mode %d\r\n",g_stUpdateVar.u8UpdateMode);
            dbg_printf("iap start Erase Sector\r\n");
            i=0;
            j=((GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR)>>3)-(GD32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)>>3));
            fmc_unlock();
            fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
            do{
                f_sta = fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)+(i<<3));
                if(f_sta != FMC_READY)
                {
                    dbg_printf("f_sta == %d\r\n",f_sta);
                    fmc_lock();
                    return 0;
                }
                i++;
            }while(i<=j);
            fmc_lock();
            dbg_printf("Erase Sector finish\r\n");
            g_stUpdateVar.u32TotalBytes = 0;
//            g_stUpdateVar.u8UpdateFlag=1;
            LeftBytes=0;
            memset(LeftBytesTab,0,4);
//            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
            g_stUpdateVar.u32PageIndex = 0;
            break;
        case 1:
            if(g_stUpdateVar.u32PageIndex<FW_VECT_OFFSET)
            {
                g_stUpdateVar.u32PageIndex +=len;
                return 0;
            }
            else
                g_stUpdateVar.u32PageIndex +=len;

            //printf("LeftBytes[%d]\r\n",LeftBytes);
            if(LeftBytes)
            {
                while(LeftBytes<=3)
                {
                    if(len>j)
                    {
                        LeftBytesTab[LeftBytes++] = *(data+j);
                    }
                    else
                    {
                        LeftBytesTab[LeftBytes++] = 0xFF;
                    }
                    j++;
                }

                fmc_unlock();
                fmc_word_program(DM_FW_DATA_ADDR+g_stUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
                fmc_lock();
                //printf("d=%08x\r\n",*(u32 *)LeftBytesTab);
                LeftBytes =0;

                g_stUpdateVar.u32TotalBytes=g_stUpdateVar.u32TotalBytes+4;

                if (len >j)
                {
                    /* update data pointer */
                    data = (data+j);
                    len = len -j;
                }
            }


            buff = (uint32_t*)(data);

            if((g_stUpdateVar.u32TotalBytes  + len) > DM_FW_BYTES_MAX)
            {
               return 0;
            }
            fmc_unlock();
            for(i = 0; i < len/4; i++)
            {
                f_sta = fmc_word_program(DM_FW_DATA_ADDR+g_stUpdateVar.u32TotalBytes+i*4,  *(buff+i));
                if(f_sta != FMC_READY)
                {
                    dbg_printf("E=%d\r\n",f_sta);
                    fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR));    //Parameter
                    fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
                    break;
                }
            }
            //??
            i=len%4;
            //dbg_printf("i[%d]\r\n",i);
            if (i>0)
            {
                LeftBytes=0;
                for(;i>0;i--)
                {
                    LeftBytesTab[LeftBytes++] = *(data+ len-i);
                    // dbg_printf("%02X ",*(data+ len-i));
                }
                g_stUpdateVar.u32TotalBytes +=len-i;
            }
            else
                g_stUpdateVar.u32TotalBytes +=len;

            if(len<1024)
                memcpy(g_stUpdateVar.u8LastData,data,len);
            fmc_lock();
            //printf(".");
            break;
        case 2:
            fmc_unlock();

//            fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR));

            g_stUpdateVar.u32TotalBytes +=len;
            if(g_stUpdateVar.u32TotalBytes>DM_FW_BYTES_MIX)
            {
                memcpy((uint8_t*)&packet,g_stUpdateVar.u8LastData, sizeof(image_header_t));
                dbg_printf("iap end\r\n");
                dbg_printf("total_bytes [%d]\r\n",g_stUpdateVar.u32TotalBytes );
                dbg_printf("ih_magic [0x%x]\r\n",packet.ih_magic );
                dbg_printf("ih_arch [%d]\r\n",packet.ih_arch );
                dbg_printf("ih_name [%s]\r\n",packet.ih_name );
                if((packet.ih_magic==0x4e545047)&&(packet.ih_arch==2)&&!strncmp((char*)packet.ih_name,_STRING_FACTORY_MODEL,strlen(_STRING_FACTORY_MODEL)))
                {
                    dbg_printf("data check ok\r\n");

                    f_sta =fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
                    dbg_printf("f_sta=%d\r\n",f_sta);
                    fmc_word_program(DM_FW_SIZE_ADDR, g_stUpdateVar.u32TotalBytes );
                }
//                else
//                {
//                    printf("data check fail\r\n");
//                }
            }
            else
            {
                dbg_printf("iap err\r\n");
                fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
            }
            fmc_lock();
            g_stUpdateVar.u8UpdateFlag=0;
            SYS_SoftReset();
            break;
    }
    return 1;
}

uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len)
{
    uint8_t l_u8RetVal = 1;

    mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_StartupLinkInfo,10000);
    g_stUpdateVar.u8UpdateMode = u8Id;
    switch(u8Step)
    {
        case 0:
            {
                mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
                g_stUpdateVar.u8UpdateFlag=1;
                if(u8Id==eUpdID_InputMCU)
                {
                    l_u8RetVal = mcu_file_updata(0,0,0);
                }
                else if(u8Id==eUpdID_OutputMCU)
                {
                    u8 l_u8SendBuf[1];

                    l_u8SendBuf[0] = 1;
                    mapp_MUH1616S_InputSendCmdToOutput(eMcuReg_UpdateMcu,l_u8SendBuf,1);
                }
                g_stUpdateVar.u16PackIndex = 0;
            }
            break;
        case 1:
            {
                g_stUpdateVar.u16PackIndex++;
//                dbg_printf("len[%d]\r\n",u16Len);
                if(u8Id==eUpdID_InputMCU)
                {
                    l_u8RetVal=mcu_file_updata(1,pu8Data,u16Len);
                }
                else if(u8Id==eUpdID_OutputMCU)
                {
#if 0
                    if(u16Len<30)
                    {
                        pstUartCom_t pUartDbgVar = &g_stUart3CommVar;
                        dbg_printf("cmd: ");
                        pUartDbgVar->SendBytes(pu8Data,u16Len,FALSE);
                    }
                    else
#endif
                        mapp_MUH1616S_INPUTSendDataToOutBoard(pu8Data,u16Len);
                }
            }
            break;
        case 2:
            {
                if(u8Id==eUpdID_InputMCU)
                {
                    l_u8RetVal=mcu_file_updata(2,pu8Data,u16Len);
                }
                else if(u8Id==eUpdID_OutputMCU)
                {
                    mapp_MUH1616S_INPUTSendDataToOutBoard(pu8Data,u16Len);
                }
                g_stUpdateVar.u8UpdateFlag=0;
            }
            break;
        default:
            break;
    }

    return l_u8RetVal;
}

u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar)
{
    if(g_stUpdateVar.u8UpdateFlag==1)
    {
        mapp_MUH1616S_INPUT_PlatformTimerSet(emUserTimer_FirmwareUpdate,2000);
        UpdateFunction(g_stUpdateVar.u8UpdateMode,1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);

        pComVar->SendPrintf("pack %d\r\n",g_stUpdateVar.u16PackIndex);
        return 1;
    }
    else
        return 0;
}

void mapp_FirmwareUpdateTimeOutFunction(void)
{
    if(g_stUpdateVar.u8UpdateFlag==1&&mapp_MUH1616S_INPUT_PlatformTimerExpired(emUserTimer_FirmwareUpdate))
    {
        UpdateFunction(g_stUpdateVar.u8UpdateMode,2,NULL,0);
    }
}

