#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"
#include "ms1826_app_single_mode.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

stProjectSpecVar_t  stMUH1616S_OUTPUTSpecVar;
static volatile u32 g_MUH1616S_OUTPUT_Timers[emUserTimer_Max];
u8 g_u8SrcChgTxFreezeState = 1;
u8 g_u8TxFreezeState[4];
stLinkSta_t    g_stLinkSta;
stLinkSta_t    g_stPreLinkSta;
bool g_bTxFreezeFlag = FALSE;
u8 g_u8BdAddr = 0x81;

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

const u8 cg_u8ChannelTableTX[16][4]=
{
  //TXIndex,RxPort,AudioPort,DeviceID
    {0,         0,          0,      0},
    {1,         1,          1,      0},
    {2,         2,          2,      0},
    {3,         3,          3,      0},
    {0,         0,          0,      1},
    {1,         1,          1,      1},
    {2,         2,          2,      1},
    {3,         3,          3,      1},
    {0,         0,          0,      2},
    {1,         1,          1,      2},
    {2,         2,          2,      2},
    {3,         3,          3,      2},
    {0,         0,          0,      3},
    {1,         1,          1,      3},
    {2,         2,          2,      3},
    {3,         3,          3,      3},
};

void app_MUH1616S_OUTPUTMS1826UserInit(void);
void app_MUH1616S_OUTPUTLT8644SwitchProcess(void);
void app_MUH1616S_OUTPUTMS1826UserProcess(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时
//------------------------------------------------------------------------------
void mapp_MUH1616S_OUTPUT_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_MUH1616S_OUTPUT_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时
//------------------------------------------------------------------------------
u32 mapp_MUH1616S_OUTPUT_PlatformTimerGet(uint8_t eTimer)
{
    return(g_MUH1616S_OUTPUT_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_MUH1616S_OUTPUT_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_MUH1616S_OUTPUT_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_MUH1616S_OUTPUT_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_MUH1616S_OUTPUT_Timers[i] > 0)
        {
            g_MUH1616S_OUTPUT_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_MUH1616S_OUTPUT_PlatformTimerExpired ( uint8_t timer )           //定时时间到函�?
{
    if ( timer < emUserTimer_Max )
    {
        return( g_MUH1616S_OUTPUT_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void MUH1616S_OUTPUT_1msTimerIrq(void)
{
    mapp_MUH1616S_OUTPUT_PlatformTimerTask();
}

void MUH1616S_OUTPUT_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void MUH1616S_OUTPUT_SpecificVarDefault(void)
{
    MUH1616S_OUTPUT_SpecBasicVarDefault();
    MUH1616S_OUTPUT_SpecCustomEdidVarDefault();
}

void MUH1616S_OUTPUT_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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
#if 0
        pstSpecVar->stBasSpecVar.u8Saturation[i]     = 50;
        pstSpecVar->stBasSpecVar.u8Hue[i]            = 50;
#endif
        pstSpecVar->stBasSpecVar.u8AudioMute[i]      = 0;
        pstSpecVar->stBasSpecVar.u8BlackScreenEnable[i] = 0;
        pstSpecVar->stBasSpecVar.u8PatternSta[i]     = 0;
#if 0
        pstSpecVar->stBasSpecVar.u8RedValue[i]       = 128;
        pstSpecVar->stBasSpecVar.u8GreenValue[i]     = 128;
        pstSpecVar->stBasSpecVar.u8BlueValue[i]      = 128;
#endif
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
    }

    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
#if 0
    pstSpecVar->stBasSpecVar.u8InRs232BaudrateID    = baud_rate_9600;
    pstSpecVar->stBasSpecVar.u8OutRs232BaudrateID   = baud_rate_9600;
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
#endif

#if 0
    for(i=0;i<32;i++)
    {
        memset(&pstSpecVar->stBasSpecVar.stScene[i],0,sizeof(stSceneSaveVar_t));
        pstSpecVar->stBasSpecVar.stScenePollingVar.bPollingListEnable[i] = FALSE;
    }
    pstSpecVar->stBasSpecVar.stScenePollingVar.bPollingEnable    = FALSE;
    pstSpecVar->stBasSpecVar.stScenePollingVar.u16PollingTime    = 5;
#endif
}

void MUH1616S_OUTPUT_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
}

bool MUH1616S_OUTPUT_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        MUH1616S_OUTPUT_SpecBasicVarDefault();
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void MUH1616S_OUTPUT_PrjStatePowerOnInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    System_Default_Edid_table = (iTE_u8 *)_6_1920x1080p60;

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = MUH1616S_OUTPUT_1msTimerIrq;
    pvvFuncMS1826UserFunctionInit = app_MUH1616S_OUTPUTMS1826UserInit;
    pvvFuncMS1826UserFunctionProcess = app_MUH1616S_OUTPUTMS1826UserProcess;

    g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;
    pstBasicVar->bPowerOnSta = TRUE;

    mapp_SetHdmiTx5V(TRUE);
    mapp_RevisionMake();
}

void MUH1616S_OUTPUT_LedWork_Loop(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    static bool sl_bLedSta = FALSE;
    if(!mapp_MUH1616S_OUTPUT_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_LedActive,500);

    if(pstBasicVar->bPowerOnSta)
    {
        if(sl_bLedSta)
        {
            sl_bLedSta = FALSE;
            led_on(led_name_ActiveLed);
//            Uart5_printf("Uart5 ok\r\n");
        }
        else
        {
            sl_bLedSta = TRUE;
            led_off(led_name_ActiveLed);
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
void MUH1616S_OUTPUT_FuncWork_LoopRunning(void)
{
    MUH1616S_OUTPUT_LedWork_Loop();
#if 0
    app_MUH1616S_OUTPUTScenePollingFunction();
#endif
}

void mapp_MUH1616S_OUTPUTStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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
}

void app_MUH1616S_OUTPUTMS1826UserInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i,l_u8RxPort,l_u8TxPort,l_u8Dev;

    for(i=0;i<Def_outport_num;i++)
    {
        l_u8RxPort = (u8)cg_u8ChannelTableTX[i][1];
        l_u8TxPort = (u8)cg_u8ChannelTableTX[i][0];
        l_u8Dev  =   (u8)cg_u8ChannelTableTX[i][3];
        app_MS1826Switch(l_u8Dev,l_u8RxPort,l_u8TxPort);
        app_MS1826TxResolutionChange(l_u8Dev,l_u8TxPort,pstBasicVar->u8TxResolutionID[i]);
        app_MS1826TxConfig(l_u8Dev,l_u8TxPort,pstBasicVar->u8TxFormatSta[i]);
//        app_MS1826TxHdcpEnable(l_u8Dev,i,pstBasicVar->u8TxHdcpMode[i]);       //have bug
//        app_MS1826SetTxColorSpace(l_u8Dev,l_u8TxPort,0);

        app_MUH1616S_OUTPUTPictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Brightness[i],MS1826_ADJUST_BRIGHTNESS);
        app_MUH1616S_OUTPUTPictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Contrast[i],MS1826_ADJUST_CONTRAST);
//        app_MUH1616S_OUTPUTPictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Saturation[i],MS1826_ADJUST_SATURATION);
//        app_MUH1616S_OUTPUTPictureAdjust(l_u8Dev,l_u8TxPort,pstBasicVar->u8Hue[i],MS1826_ADJUST_HUE);
#if 0
        app_MS1826PictureColorConfig(l_u8Dev,l_u8TxPort,
                            pstBasicVar->u8RedValue[i],
                            pstBasicVar->u8GreenValue[i],
                            pstBasicVar->u8BlueValue[i],
                            1);
#endif
        app_MS1826TxDigitalAudioMute(l_u8Dev,l_u8TxPort,pstBasicVar->u8AudioMute[i]);
        app_MUH1616S_OUTPUTSetSplicerPixelDisplay(l_u8Dev,l_u8TxPort,
                pstBasicVar->u16SplicerX[i]+pstBasicVar->u16BorderX[i],
                pstBasicVar->u16SplicerY[i]+pstBasicVar->u16BorderY[i],
                pstBasicVar->u16SplicerWidth[i]-pstBasicVar->u16BorderWidth[i],
                pstBasicVar->u16SplicerHeight[i]-pstBasicVar->u16BorderHeight[i]);
    }
}

void app_MUH1616S_OUTPUTMS1826UserProcess(void)
{
    switch(g_u8SrcChgTxFreezeState)
    {
        case 1:
            {
                u8 l_u8Value[2];

                _video_freeze_multichip(4,0,g_u8TxFreezeState);
                l_u8Value[0] = 1;
                mapp_MUH1616S_OutputSendCmdToInout(eMcuReg_MsgFreezeOk,l_u8Value,1);
                dbg_printf("switch freeze 0x%08x\r\n",g_u8TxFreezeState[0]|(g_u8TxFreezeState[1]<<4)|(g_u8TxFreezeState[2]<<8)|(g_u8TxFreezeState[3]<<12));
//                delay_us(40000);
                g_u8SrcChgTxFreezeState = 0;
                g_u8TxFreezeState[0]=0;
                g_u8TxFreezeState[1]=0;
                g_u8TxFreezeState[2]=0;
                g_u8TxFreezeState[3]=0;
                mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_SwitchFreezeRecoveryDelayTime,300);
                g_bTxFreezeFlag = TRUE;
            }
            break;
        case 2:
            {
                mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_SwitchFreezeRecoveryDelayTime,0);
                g_bTxFreezeFlag = FALSE;
                g_u8SrcChgTxFreezeState = 0;
                g_u8TxFreezeState[0]=0x0F;
                g_u8TxFreezeState[1]=0x0F;
                g_u8TxFreezeState[2]=0x0F;
                g_u8TxFreezeState[3]=0x0F;
                _video_unfreeze_multichip(4,0,g_u8TxFreezeState);
                dbg_printf("switch unfreeze 0x%08x\r\n",g_u8TxFreezeState[0]|(g_u8TxFreezeState[1]<<4)|(g_u8TxFreezeState[2]<<8)|(g_u8TxFreezeState[3]<<12));
            }
            break;
        default:
            {
                g_u8SrcChgTxFreezeState = 0;
            }
            break;
    }
#if 0
    if(g_bTxFreezeFlag)
    {
        if(mapp_MUH1616S_OUTPUT_PlatformTimerExpired(emUserTimer_SwitchFreezeRecoveryDelayTime))
        {
            g_bTxFreezeFlag = 0;
            g_u8TxFreezeState[0]=0x0F;
            g_u8TxFreezeState[1]=0x0F;
            g_u8TxFreezeState[2]=0x0F;
            g_u8TxFreezeState[3]=0x0F;
            _video_unfreeze_multichip(4,0,g_u8TxFreezeState);
            dbg_printf("switch unfreeze delay recovery\r\n");
        }
    }
#endif

    for(g_u8MS1826DevIndex=0;g_u8MS1826DevIndex<DEV_NUM_MS1826;g_u8MS1826DevIndex++)
    {
        g_u8MS1826_I2CDevID = (u8)cg_stMs1826DevCfgTable[g_u8MS1826DevIndex].u8I2cIndex;
        if(g_u8MS1826DevIndex==0)
        {
            g_stLinkSta.u8OutLinkSta[0] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[0][0]);
            g_stLinkSta.u8OutLinkSta[1] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[1][0]);
            g_stLinkSta.u8OutLinkSta[2] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[2][0]);
            g_stLinkSta.u8OutLinkSta[3] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[3][0]);
        }
        else if(g_u8MS1826DevIndex==1)
        {
            g_stLinkSta.u8OutLinkSta[4] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[4][0]);
            g_stLinkSta.u8OutLinkSta[5] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[5][0]);
            g_stLinkSta.u8OutLinkSta[6] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[6][0]);
            g_stLinkSta.u8OutLinkSta[7] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[7][0]);
        }
        else if(g_u8MS1826DevIndex==2)
        {
            g_stLinkSta.u8OutLinkSta[8] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[8][0]);
            g_stLinkSta.u8OutLinkSta[9] = app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[9][0]);
            g_stLinkSta.u8OutLinkSta[10]= app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[10][0]);
            g_stLinkSta.u8OutLinkSta[11]= app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[11][0]);
        }
        else if(g_u8MS1826DevIndex==3)
        {
            g_stLinkSta.u8OutLinkSta[12]= app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[12][0]);
            g_stLinkSta.u8OutLinkSta[13]= app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[13][0]);
            g_stLinkSta.u8OutLinkSta[14]= app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[14][0]);
            g_stLinkSta.u8OutLinkSta[15]= app_ms1826_get_tx_hpd_sta(g_u8MS1826DevIndex,cg_u8ChannelTableTX[15][0]);
        }
    }
}


// u8Mode: 1 freeze, 2 unfreeze
void app_MUH1616S_OUTPUTSwitchSourceFreeze(u8 u8Mode,u16 u16FreezeSta)
{
    g_u8SrcChgTxFreezeState = u8Mode;
    g_u8TxFreezeState[0] = u16FreezeSta&0x000F;
    g_u8TxFreezeState[1] = (u16FreezeSta>>4)&0x000F;
    g_u8TxFreezeState[2] = (u16FreezeSta>>8)&0x000F;
    g_u8TxFreezeState[3] = (u16FreezeSta>>12)&0x000F;

#if (_SYSTEM_SUPPORT_RTOS==0)
    if(g_u8SrcChgTxFreezeState == 1)
    {
        _video_freeze_multichip(4,0,g_u8TxFreezeState);
    }
    else if(g_u8SrcChgTxFreezeState == 2)
    {
        _video_unfreeze_multichip(4,0,g_u8TxFreezeState);
        g_u8SrcChgTxFreezeState = 0;
    }
#endif
}

void app_MUH1616S_OUTPUTSetSplicerPixelDisplay(u8 u8Dev,u8 u8Tx,u16 u16X,u16 u16Y,u16 u16Width,u16 u16Height)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Port = (u8)cg_u8ChannelTableTX[u8Tx][0];

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
}

void app_MUH1616S_OUTPUTSetSplicerDisplay(u8 u8Dev,u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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

    l_u16HDisp = app_ms1826_get_display_horizontal(u8Dev,cg_u8ChannelTableTX[u8Tx][1]);
    l_u16VDisp = app_ms1826_get_display_vertical(u8Dev,cg_u8ChannelTableTX[u8Tx][1]);
    if(l_u16HDisp>1920)
        l_u16HDisp = 1920;
    if(l_u16VDisp>1080)
        l_u16VDisp = 1080;

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

    app_MUH1616S_OUTPUTSetSplicerPixelDisplay(u8Dev,u8Tx,l_u16X,l_u16Y,l_u16W,l_u16H);

    SetUserData_SpecBasicVar();
}

void app_MUH1616S_OUTPUTSetSplicerBorder(u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index,u16 u16BorderWidth,u16 u16BorderHeight)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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
        pstBasicVar->u16BorderX[u8Tx]       = u16BorderWidth;
        pstBasicVar->u16BorderY[u8Tx]       = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if(((u8Index%u8H)==0)&&((u8Index/u8W)==(u8W-1)))   // left down
    {
        pstBasicVar->u16BorderX[u8Tx]       = 0;
        pstBasicVar->u16BorderY[u8Tx]       = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if(((u8Index%u8H)==(u8H-1))&&((u8Index/u8W)==0))    // right up
    {
        pstBasicVar->u16BorderX[u8Tx]       = u16BorderWidth;
        pstBasicVar->u16BorderY[u8Tx]       = 0;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if((u8Index%u8H)==0)   // left side
    {
        pstBasicVar->u16BorderX[u8Tx]       = 0;
        pstBasicVar->u16BorderY[u8Tx]       = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight<<1;
    }
    else if((u8Index/u8W)==0)   // up side
    {
        pstBasicVar->u16BorderX[u8Tx]       = u16BorderWidth;
        pstBasicVar->u16BorderY[u8Tx]       = 0;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth<<1;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else if((u8Index%u8H)==(u8H-1)) // right side
    {
        pstBasicVar->u16BorderX[u8Tx]       = u16BorderWidth;
        pstBasicVar->u16BorderY[u8Tx]       = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight<<1;
    }
    else if((u8Index/u8W)==(u8W-1)) // down side
    {
        pstBasicVar->u16BorderX[u8Tx]       = u16BorderWidth;
        pstBasicVar->u16BorderY[u8Tx]       = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth<<1;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight;
    }
    else
    {
        pstBasicVar->u16BorderX[u8Tx]       = u16BorderWidth;
        pstBasicVar->u16BorderY[u8Tx]       = u16BorderHeight;
        pstBasicVar->u16BorderWidth[u8Tx]   = u16BorderWidth<<1;
        pstBasicVar->u16BorderHeight[u8Tx]  = u16BorderHeight<<1;
    }
    SetUserData_SpecBasicVar();
}

void app_MUH1616S_OUTPUTPictureAdjust(u8 u8Dev,u8 u8Tx,u8 u8Value,u8 u8Mode)
{
    app_MS1826PictureAdjust(u8Dev,u8Tx,u8Value+78,u8Mode);
}

void app_MUH1616S_OUTPUTSourceSwitch(u8 u8Out,u8 u8In)
{
    u8 l_u8RxPort,l_u8TxPort,l_u8Dev;

    l_u8RxPort = cg_u8ChannelTableTX[u8In][1];
    l_u8TxPort = cg_u8ChannelTableTX[u8Out][0];
    l_u8Dev = cg_u8ChannelTableTX[u8Out][3];
    app_MS1826Switch(l_u8Dev,l_u8RxPort, l_u8TxPort);
}

#if 0
void app_MUH1616S_OUTPUTSceneSave(u8 u8SceneID)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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

void app_MUH1616S_OUTPUTSceneCall(u8 u8SceneID)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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
                app_MUH1616S_OUTPUTSourceSwitch(i,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]);

                // Splicer config
                l_u8TxPort = (u8)cg_u8ChannelTableTX[i][0];
                l_u8Dev  =   (u8)cg_u8ChannelTableTX[i][3];
                if(l_bSwSplicerFlag[i])
                {
                    app_MUH1616S_OUTPUTSetSplicerPixelDisplay(l_u8Dev,l_u8TxPort,
                            pstBasicVar->u16SplicerX[i]+pstBasicVar->u16BorderX[i],
                            pstBasicVar->u16SplicerY[i]+pstBasicVar->u16BorderY[i],
                            pstBasicVar->u16SplicerWidth[i]-pstBasicVar->u16BorderWidth[i],
                            pstBasicVar->u16SplicerHeight[i]-pstBasicVar->u16BorderHeight[i]);
                }
            }
            // save value
            SetUserData_HdmiSel();
            SetUserData_SpecBasicVar();
        }
    }
}

// 场景轮询;
void app_MUH1616S_OUTPUTScenePollingFunction(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->stScenePollingVar.bPollingEnable)
    {
        u8 i,l_u8TxPort,l_u8Dev,l_u8SceneID;
        bool l_bSwSplicerFlag[Def_outport_num];

        if(!mapp_MUH1616S_OUTPUT_PlatformTimerExpired(emUserTimer_ScenePolling))
            return;
        mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_ScenePolling,pstBasicVar->stScenePollingVar.u16PollingTime*1000);
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
                        app_MUH1616S_OUTPUTSourceSwitch(i,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]);

                        // Splicer config
                        l_u8TxPort = (u8)cg_u8ChannelTableTX[i][0];
                        l_u8Dev  =   (u8)cg_u8ChannelTableTX[i][3];
                        if(l_bSwSplicerFlag[i])
                        {
                            app_MUH1616S_OUTPUTSetSplicerPixelDisplay(l_u8Dev,l_u8TxPort,
                                    pstBasicVar->u16SplicerX[i]+pstBasicVar->u16BorderX[i],
                                    pstBasicVar->u16SplicerY[i]+pstBasicVar->u16BorderY[i],
                                    pstBasicVar->u16SplicerWidth[i]-pstBasicVar->u16BorderWidth[i],
                                    pstBasicVar->u16SplicerHeight[i]-pstBasicVar->u16BorderHeight[i]);
                        }
                    }
                    // save value
                    SetUserData_HdmiSel();
                    SetUserData_SpecBasicVar();

                    USER_printf("scene polling %d called\r\n",g_u8SceneStartIndex+1);
                    USER_printf("ok\r\n");
                    break;
                }
            }
        }
    }
}
#endif

#if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
/******************************************************
///Function Name:   app_MUH1616S_OUTPUTReadEdidDescriptor1Timing
///Description:     Config output resolution
///Input:           pu8EdidBuf(Edid 数据)
///Output:          pstEdidRes(提取的EDID分辨率信�?
///Return:          NULL
///Others:          NULL
******************************************************/
void app_MUH1616S_OUTPUTReadEdidDescriptor1Timing(pu8 pu8EdidBuf,pstTimingTable_t pstEdidRes)
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
 * FunName : app_MUH1616S_OUTPUTAutoResolutionConfig
 * Writer  : wf8421
 * Date    : 2023-08-25 16:40
 * Description: Read tx edid to config resolution
 * Parameter: u8Tx - output port
 * Return  : None
 * History :
 ***********************************************************************/
void app_MUH1616S_OUTPUTAutoResolutionConfig(u8 u8Tx)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    stTimingTable_t l_stTiming;
    u8 l_u8RealPort,l_u8Dev;
    u8 l_u8EdidBuf[256];
    bool l_bEdidSta = FALSE;

    if(pstBasicVar->stAutoTimingTable[u8Tx].bEnable)
    {
        l_u8RealPort = cg_u8ChannelTableTX[u8Tx][0]+1;
        l_u8Dev = cg_u8ChannelTableTX[u8Tx][3];
        app_ms1826_get_tx_edid_data(l_u8Dev,l_u8RealPort,l_u8EdidBuf);
        l_bEdidSta = mapp_EdidLimitTo4K30(l_u8EdidBuf,l_u8EdidBuf,256,FALSE,TRUE);
        if(l_bEdidSta)
        {
            app_MUH1616S_OUTPUTReadEdidDescriptor1Timing(l_u8EdidBuf,&l_stTiming);
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
 * FunName : app_MUH1616S_OUTPUTAutoResolutionProcess
 * Writer  : wf8421
 * Date    : 2023-08-25 17:02
 * Description: auto config resolution process
 * Parameter: None
 * Return  : None
 * History :
 ***********************************************************************/
void app_MUH1616S_OUTPUTAutoResolutionProcess(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_OUTPUTSpecVar;
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

                app_MUH1616S_OUTPUTAutoResolutionConfig(i);
            }
        }
    }
}
#endif

void mapp_SendCmdToInput(u8 u8DevAddr,u8 u8Cmd,pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart5CommVar;
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

void mapp_MUH1616S_OutputSendCmdToInout(u8 u8cmd,u8 *pu8Data, u16 u16Len)
{
    mapp_SendCmdToInput(g_u8BdAddr,u8cmd,pu8Data,u16Len);
}

//------------------------------------------------------------------------
#define DM_FW_BYTES_MIX         (15*1024)
#define DM_FW_BYTES_MAX         (190*1024)  //100KB
#define DM_FW_DATA_ADDR         0x8033800
#define DM_FW_FLAG_ADDR         0x8063000
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

int8_t FLASH_If_Erase(uint32_t StartSector)
{
  uint32_t FlashAddress;

  FlashAddress = StartSector;

  while (FlashAddress <   DM_FW_FLAG_ADDR)
  {
	if (FLASH_EraseOnePage(FlashAddress) == FLASH_COMPL)
	{
		FlashAddress += PAGE_SIZE;

			  IWDG_CntReload( 0xff0);
	}
	else
	{
		return -1;
	}
  }

  return 0;
}

uint8_t mcu_file_updata(uint8_t step ,uint8_t *data ,uint16_t len)
{
	uint16_t i,j=0;
	FLASH_STS f_sta;
	uint32_t* buff;
	image_header_t packet;

	static uint8_t LeftBytesTab[4]={0};
	static uint8_t LeftBytes=0;

	switch(step)
	{
		case 0:
//			dbg_printf("iap updata mode %d\r\n",sys_temp.mcu_updata_mode);
			dbg_printf("iap start Erase Sector\r\n");
			FLASH_Unlock();
			if(FLASH_If_Erase(DM_FW_DATA_ADDR) != 0)        /* 1Kbyte *3 */
			{
				FLASH_Lock();
				return 0;
			}
			FLASH_Lock();
			dbg_printf("Erase Sector finish\r\n");
			g_stUpdateVar.u32TotalBytes = 0;
			g_stUpdateVar.u8UpdateFlag=1;
			LeftBytes=0;
			memset(LeftBytesTab,0,4);
            g_stUpdateVar.u32PageIndex = 0;
            mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
			break;
		case 1:
//            g_stUpdateVar.u16PackIndex++;
//			dbg_printf("len[%d]\r\n",len);

            if(g_stUpdateVar.u32PageIndex<FW_VECT_OFFSET)
            {
                g_stUpdateVar.u32PageIndex +=len;
                return 0;
            }
            else
                g_stUpdateVar.u32PageIndex +=len;
//			dbg_printf("LeftBytes[%d]\r\n",LeftBytes);
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

				FLASH_Unlock();
				FLASH_ProgramWord(DM_FW_DATA_ADDR+g_stUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
				FLASH_Lock();
				//dbg_printf("d=%08x\r\n",*(u32 *)LeftBytesTab);
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
			FLASH_Unlock();
			for(i = 0; i < len/4; i++)
			{
				f_sta = FLASH_ProgramWord(DM_FW_DATA_ADDR+g_stUpdateVar.u32TotalBytes+i*4,  *(buff+i));
				if(f_sta != FLASH_COMPL)
				{
					dbg_printf("E=%d\r\n",f_sta);
					FLASH_EraseOnePage(DM_FW_FLAG_ADDR);	//Parameter
					FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
					break;
				}
			}
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

			FLASH_Lock();
			//dbg_printf(".");
			break;
		case 2:
			FLASH_Unlock();


			FLASH_EraseOnePage(DM_FW_FLAG_ADDR);  //Parameter

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

					f_sta =FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
					dbg_printf("f_sta=%d\r\n",f_sta);
					FLASH_ProgramWord(DM_FW_SIZE_ADDR, g_stUpdateVar.u32TotalBytes );
				}
//				  else
//				  {
//					  dbg_printf("data check fail\r\n");
//				  }
			}
			else
			{
				  dbg_printf("iap err\r\n");
				  FLASH_ProgramWord(DM_FW_FLAG_ADDR, DM_FW_FLAG_INVALID);
			}
			FLASH_Lock();
			g_stUpdateVar.u8UpdateFlag=0;
			RCC_EnableAPB1PeriphClk(RCC_APB1_PERIPH_WWDG,ENABLE);
			WWDG_Enable(0x40);
			break;
	}
	return 1;
}

uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len)
{
    uint8_t l_u8RetVal = 1;

//    mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_ReportTiming,10000);
//    mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_TimingChange,10000);
    g_stUpdateVar.u8UpdateMode = u8Id;
    switch(u8Step)
    {
        case 0:
            {
                mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
                g_stUpdateVar.u8UpdateFlag=1;
                //if(u8Id==eUpdID_MCU)
                {
                    l_u8RetVal = mcu_file_updata(0,0,0);
                }
                g_stUpdateVar.u16PackIndex = 0;
            }
            break;
        case 1:
            {
                g_stUpdateVar.u16PackIndex++;
                //if(u8Id==eUpdID_MCU)
                {
                    l_u8RetVal=mcu_file_updata(1,pu8Data,u16Len);
                }
            }
            break;
        case 2:
            {
                //if(u8Id==eUpdID_MCU)
                {
                    l_u8RetVal=mcu_file_updata(2,pu8Data,u16Len);
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
        mapp_MUH1616S_OUTPUT_PlatformTimerSet(emUserTimer_FirmwareUpdate,2000);
        UpdateFunction(g_stUpdateVar.u8UpdateMode,1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);

//        pComVar->SendPrintf("pack %d\r\n",g_stUpdateVar.u16PackIndex);
        dbg_printf("pack %d\r\n",g_stUpdateVar.u16PackIndex);
        return 1;
    }
    else
        return 0;
}

void mapp_FirmwareUpdateTimeOutFunction(void)
{
    if(g_stUpdateVar.u8UpdateFlag==1&&mapp_MUH1616S_OUTPUT_PlatformTimerExpired(emUserTimer_FirmwareUpdate))
    {
        UpdateFunction(g_stUpdateVar.u8UpdateMode,2,NULL,0);
    }
}

