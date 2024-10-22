#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"
#include "ms1826_app_single_mode.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

stProjectSpecVar_t  stLMX_4OUHS_CSpecVar;
static volatile u32 g_LMX_4OUHS_C_Timers[emUserTimer_Max];

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

const u8 cg_u8ChannelTable[4][3]=
{
  //TXIndex,RxPort,AudioPort
    {0,         0,          1},
    {1,         3,          0},
    {2,         2,          3},
    {3,         1,          2},
};

void app_LMX_4OUHS_CMS1826UserInit(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_LMX_4OUHS_C_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_LMX_4OUHS_C_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_LMX_4OUHS_C_PlatformTimerGet(uint8_t eTimer)
{
    return(g_LMX_4OUHS_C_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_LMX_4OUHS_C_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_LMX_4OUHS_C_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_LMX_4OUHS_C_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_LMX_4OUHS_C_Timers[i] > 0)
        {
            g_LMX_4OUHS_C_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_LMX_4OUHS_C_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_LMX_4OUHS_C_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void LMX_4OUHS_C_1msTimerIrq(void)
{
    mapp_LMX_4OUHS_C_PlatformTimerTask();
}

void LMX_4OUHS_C_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void LMX_4OUHS_C_SpecificVarDefault(void)
{
    LMX_4OUHS_C_SpecBasicVarDefault();
    LMX_4OUHS_C_SpecCustomEdidVarDefault();
}

void LMX_4OUHS_C_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
        pstSpecVar->stBasSpecVar.u16BorderSizeW[i]   = 0;
        pstSpecVar->stBasSpecVar.u16BorderSizeH[i]   = 0;
    }
}

void LMX_4OUHS_C_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
}

bool LMX_4OUHS_C_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        LMX_4OUHS_C_SpecBasicVarDefault();
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void LMX_4OUHS_C_PrjStatePowerOnInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    System_Default_Edid_table = (iTE_u8 *)_6_1920x1080p60;

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = LMX_4OUHS_C_1msTimerIrq;
//    pvvFuncMS1826UserFunctionProcess = mapp_4OUHS_MS1826FunctionProcess;
    pvvFuncMS1826UserFunctionInit = app_LMX_4OUHS_CMS1826UserInit;
    g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;

    mapp_SetHdmiTx5V(TRUE);
    mapp_RevisionMake();
}

void LMX_4OUHS_C_LedWork_Loop(void)
{
    static bool sl_bLedSta = FALSE;
    if(!mapp_LMX_4OUHS_C_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_LMX_4OUHS_C_PlatformTimerSet(emUserTimer_LedActive,500);

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

// the status of led when running system
void LMX_4OUHS_C_FuncWork_LoopRunning(void)
{
    LMX_4OUHS_C_LedWork_Loop();
}

void mapp_LMX_4OUHS_CStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char l_s8ResText[20];
    u8 i;

    RS232_printf("\r\n");
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    RS232_printf("%s %s\r\n",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
    for(i=0;i<Def_outport_num;i++)
    {
        memset(l_s8ResText,0,20);
        GetResolutionText(l_s8ResText,pstBasicVar->u8TxResolutionID[i]);
        RS232_printf("out %d res %s\r\n",i+1,l_s8ResText);
        RS232_printf("out %d in %d\r\n",i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
    }
}

void app_LMX_4OUHS_CMS1826UserInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i,l_u8Port;

    for(i=0;i<Def_outport_num;i++)
    {
        l_u8Port = (u8)cg_u8ChannelTable[pstPrjSaveVar->stHdmiSel.u8SwChannel[i]][1];
        app_MS1826Switch(0,l_u8Port,i);
        app_MS1826TxResolutionChange(0,i,pstBasicVar->u8TxResolutionID[i]);
        app_MS1826TxConfig(0,i,pstBasicVar->u8TxFormatSta[i]);
//        app_MS1826TxHdcpEnable(0,i,pstBasicVar->u8TxHdcpMode[i]);       //no hdcp key

        app_LMX_4OUHS_CPictureAdjust(i,pstBasicVar->u8Brightness[i],MS1826_ADJUST_BRIGHTNESS);
        app_LMX_4OUHS_CPictureAdjust(i,pstBasicVar->u8Contrast[i],MS1826_ADJUST_CONTRAST);
        app_LMX_4OUHS_CPictureAdjust(i,pstBasicVar->u8Contrast[i],MS1826_ADJUST_SATURATION);
        app_LMX_4OUHS_CPictureAdjust(i,pstBasicVar->u8Hue[i],MS1826_ADJUST_HUE);
#if 0
        app_MS1826PictureColorConfig(0,i,
                            pstBasicVar->u8RedValue[i],
                            pstBasicVar->u8GreenValue[i],
                            pstBasicVar->u8BlueValue[i],
                            1);
#endif
        app_MS1826TxDigitalAudioMute(0,i,pstBasicVar->u8AudioMute[i]);
        app_LMX_4OUHS_CSetSplicerPixelDisplay(i,
                pstBasicVar->u16SplicerX[i]+pstBasicVar->u16BorderX[i],
                pstBasicVar->u16SplicerY[i]+pstBasicVar->u16BorderY[i],
                pstBasicVar->u16SplicerWidth[i]-pstBasicVar->u16BorderWidth[i],
                pstBasicVar->u16SplicerHeight[i]-pstBasicVar->u16BorderHeight[i]);

        mapp_SetAudioMute(i,FALSE);
    }
}

void app_LMX_4OUHS_CSetSplicerPixelDisplay(u8 u8Tx,u16 u16X,u16 u16Y,u16 u16Width,u16 u16Height)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
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

    app_MS1826SplicerEnable(0,l_u8Port,pstBasicVar->bSplicerEnable[u8Tx]);
    app_MS1826SplicerConfig(0,l_u8Port,u16X,u16Y,u16Width,u16Height);
}

void app_LMX_4OUHS_CSetSplicerDisplay(u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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

    l_u16HDisp = app_ms1826_get_display_horizontal(0,cg_u8ChannelTable[u8Tx][0]);
    l_u16VDisp = app_ms1826_get_display_vertical(0,cg_u8ChannelTable[u8Tx][0]);

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

    app_LMX_4OUHS_CSetSplicerPixelDisplay(u8Tx,l_u16X,l_u16Y,l_u16W,l_u16H);

    SetUserData_SpecBasicVar();
}

void app_LMX_4OUHS_CSetSplicerBorder(u8 u8Tx,u8 u8H,u8 u8W,u8 u8Index,u16 u16BorderWidth,u16 u16BorderHeight)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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

void app_LMX_4OUHS_CPictureAdjust(u8 u8Tx,u8 u8Value,u8 u8Mode)
{
    app_MS1826PictureAdjust(0,u8Tx,u8Value+78,u8Mode);
}

