#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"


stProjectSpecVar_t  stMHD0404SSpecVar;
static bool         sg_bChangePowerStateFlag = FALSE;
stADN4604SwitchTable_T g_stSwitchTable;

stTxEdidData_t g_stTxEdidVar[Def_outport_num];
static volatile u32 g_MHD0404S_Timers[emUserTimer_Max];

stResolution_t g_stRxResTable[_HDMI_INPUT_PORT_USED_NUMBER];
stLinkSta_t    g_stLinkSta;
stLinkSta_t    g_stPreLinkSta;
stWinConfigVar_t    g_stWinConfigVar[Def_outport_num];
u8 g_u8CardType[3]; // 0:Tx,1:rx1,2:rx2

extern stProjectGlobalAllVar_t g_stPrjGloVar;

#if 0
const stTimingTable_t res_table[]=
{
    //  HDST    HTOT    HBP   HFP HSYNC      HPOL      VDST     VTOT  VBP  VFP VSYNC    VPOL    VCLK   u8VIC        PCLK            interlace
      { 1920,   2080,    80,   48,   32,    Hpos ,   1200,  1235,  26,   3,    6,  Vneg,     60,     0, (2080*1235)*60/10000,    PROG},//1920x1200@60Hz
      { 1920,   2200,   148,   88,   44,    Hpos ,   1080,  1125,  36,   4,    5,  Vpos,     60,    16,                 14850,   PROG},//1920x1080@60Hz
      { 1280,   1650,   220,  110,   40,    Hpos ,  720,     750,  20,   5,    5,  Vpos,     60,     4,                  7425,   PROG},//1280x720@60Hz
      { 1360,   1792,   256,   64,  112,    Hpos ,  768,     795,  18,   3,    6,  Vpos,     60,     0,  (1792*795)*60/10000,    PROG},//1360x768@60Hz
      { 1280,   1688,   248,   48,  112,    Hpos ,   1024,  1066,  38,   1,    3,  Vpos,     60,     0, (1688*1066)*60/10000,    PROG},//1280x1024@60Hz
      { 1024,   1344,   160,   24,  136,    Hneg ,  768,     806,  29,   3,    6,  Vneg,     60,     0,  (1344*806)*60/10000,    PROG},//1024x768@60Hz
      { 1600,   2160,   304,   64,  192,    Hpos ,   1200,  1250,  46,   1,    3,  Vpos,     60,     0, (2160*1250)*60/10000,    PROG},//1600x1200@60Hz
      { 1440,   1904,   232,   80,  152,    Hneg ,  900,     934,  25,   3,    6,  Vpos,     60,     0,  (1904*934)*60/10000,    PROG},//1440x900@60Hz
      { 1600,   1800,    96,   24,   80,    Hpos ,  900,    1000,  96,   1,    3,  Vpos,     60,     0, (1800*1000)*60/10000,    PROG},//1600x900@60Hz
      { 1280,   1980,   220,  440,   40,    Hpos ,  720,     750,  20,   5,    5,  Vpos,     50,    19,  (1980*750)*50/10000,    PROG},//1280x720@50Hz
      { 1920,   2640,   148,  528,   44,    Hpos ,   1080,  1125,  36,   4,    5,  Vpos,     50,    31, (2640*1125)*50/10000,    PROG},//1920x1080@50Hz
};
#endif

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

const stWinTypeFactorType_t cg_stMHD0404SClipWinFactoryType_t[][4]=
{
    //default
    {
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 1x2
    {
        {   0,  50,  0, 100,  50, 100, 100,  100},
        {  50, 100,  0, 100,  50, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 1x3
    {
        {   0,   3,  0, 100,   1,   3, 100,  100},
        {   1,   3,  0, 100,   1,   3, 100,  100},
        {   2,   3,  0, 100,   1,   3, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 1x4
    {
        {   0,   4,  0, 100,   1,   4, 100,  100},
        {   1,   4,  0, 100,   1,   4, 100,  100},
        {   2,   4,  0, 100,   1,   4, 100,  100},
        {   3,   4,  0, 100,   1,   4, 100,  100},
    },
    // 2x2
    {
        {   0,  50,  0, 100,  50, 100,  50,  100},
        {  50, 100,  0, 100,  50, 100,  50,  100},
        {   0,  50, 50, 100,  50, 100,  50,  100},
        {  50, 100, 50, 100,  50, 100,  50,  100},
    },
    // 2x1
    {
        {   0, 100,  0,   2, 100, 100,   1,    2},
        {   0, 100,  1,   2, 100, 100,   1,    2},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 3x1
    {
        {   0, 100,  0,   3, 100, 100,   1,    3},
        {   0, 100,  1,   3, 100, 100,   1,    3},
        {   0, 100,  2,   3, 100, 100,   1,    3},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 4x1
    {
        {   0, 100,  0,   4, 100, 100,   1,    4},
        {   0, 100,  1,   4, 100, 100,   1,    4},
        {   0, 100,  2,   4, 100, 100,   1,    4},
        {   0, 100,  3,   4, 100, 100,   1,    4},
    },

};

void MHD0404S_1msTimerIrq(void);
void mapp_SourceChangeToSetWinProcess(void);
void report_video_timing(void);
void mapp_MHD0404SIDT5VInit(void);
void mapp_MHD0404STxConnectCheck(void);
void mapp_MHD0404SLinkStatusCheck(void);
void mapp_GsvChipInitStatus(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_MHD0404S_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_MHD0404S_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_MHD0404S_PlatformTimerGet(uint8_t eTimer)
{
    return(g_MHD0404S_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_MHD0404S_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_MHD0404S_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_MHD0404S_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_MHD0404S_Timers[i] > 0)
        {
            g_MHD0404S_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_MHD0404S_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_MHD0404S_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void MHD0404S_1msTimerIrq(void)
{
    mapp_MHD0404S_PlatformTimerTask();
}

void MHD0404S_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);
    memset((u8*)&g_stRxResTable,0,sizeof(stResolution_t)*_HDMI_INPUT_PORT_USED_NUMBER);
    memset((u8*)&g_stLinkSta,0,sizeof(stLinkSta_t));
    memset((u8*)&g_stPreLinkSta,0,sizeof(stLinkSta_t));


}

void MHD0404S_SpecificVarDefault(void)
{
    MHD0404S_SpecBasicVarDefault();
    MHD0404S_SpecCustomEdidVarDefault();
}

void MHD0404S_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    u8 i,j;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
    pstSpecVar->stBasSpecVar.u8InRs232BaudrateID = baud_rate_9600;
    pstSpecVar->stBasSpecVar.u8OutRs232BaudrateID = baud_rate_9600;
    for(i=0;i<32;i++)
    {
        pstSpecVar->stBasSpecVar.stScene[i].u8SceneSaveFlag = 0;
        for(j=0;j<Def_outport_num;j++)
            pstSpecVar->stBasSpecVar.stScene[i].u8VideoCh[j] = 0;
    }
    for(i=0;i<Def_outport_num;i++)
    {
        pstSpecVar->stBasSpecVar.u8TxFormatSta[i] = emTxFormat_Hdmi;
        pstSpecVar->stBasSpecVar.u8TxResolutionID[i] = emRes_1920x1080_60;
        pstSpecVar->stBasSpecVar.u8AudioSource[i] = i+1;
        pstSpecVar->stBasSpecVar.u8TxHdcpMode[i] = 0;
    }
    pstSpecVar->stBasSpecVar.u8AudioSource[Def_outport_num] = Def_outport_num+1;
    pstSpecVar->stBasSpecVar.u8AudioSource[Def_outport_num+1] = Def_outport_num+2;
    pstSpecVar->stBasSpecVar.u8ExternalAudioMute[0] = 0;
    pstSpecVar->stBasSpecVar.u8ExternalAudioMute[1] = 0;
    pstSpecVar->stBasSpecVar.u8UartMode = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[0] = 192;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[1] = 168;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[2] = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8IPAddr[3] = 178;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[0] = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[1] = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[2] = 255;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetMask[3] = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[0] = 192;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[1] = 168;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[2] = 0;
    pstSpecVar->stBasSpecVar.stIPVar.u8NetGate[3] = 1;
    pstSpecVar->stBasSpecVar.stIPVar.u16SrcPort = 4001;
    pstSpecVar->stBasSpecVar.bIsDebugEn = FALSE;
    pstSpecVar->stBasSpecVar.u8SplitMode = eSpMode_None;

}

void MHD0404S_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
}

bool MHD0404S_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        MHD0404S_SpecBasicVarDefault();
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void MHD0404S_PrjStatePowerOnInit(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstUartCom_t pUarControltVar = &g_stUart5CommVar;
    pstUartCom_t pUarRemoteVar = &g_stUart4CommVar;

    memset(g_u8CardType,0,3);
    memset(g_stTxEdidVar,0,sizeof(stTxEdidData_t));
    memset((u8*)&g_stWinConfigVar,0,sizeof(stWinConfigVar_t)*Def_outport_num);
    System_Default_Edid_table = (iTE_u8 *)_6_1920x1080p60;
    pvvFuncGsvUserFunctionInit = mapp_GsvChipInitStatus;

    pvvInitIDTFunc = mapp_MHD0404SIDT5VInit;
    pFuncPrj1MsLoopHook = MHD0404S_1msTimerIrq;
    g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;
    mapp_RevisionMake();

#if 1
    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
            SwitchResolutionToRefreshWindowns(i,pstBasicVar->u8TxResolutionID[i],FALSE);
        }
    }
#endif
#if 1
    mapp_SelectFpgaFlashUpdateMode(0,FALSE);
    mapp_SelectFpgaFlashUpdateMode(1,FALSE);
    mapp_SetFpgaResetPin(0,0);
    mapp_SetFpgaResetPin(1,0);
    delay_ms(100);
    mapp_SetFpgaResetPin(0,1);
    mapp_SetFpgaResetPin(1,1);
#endif

    pUarControltVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicVar->u8InRs232BaudrateID), DEF_UART_DATA_CONFIG);
    pUarRemoteVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicVar->u8OutRs232BaudrateID), DEF_UART_DATA_CONFIG);
    delay_ms(1000);
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_StartupTime,4000);
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,8000);
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_ReportTiming,8000);
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_CheckLinkStatus,8000);
    mapp_SetHdmiTx5V(TRUE);
}

void mapp_MHD0404SMainFunction(void)
{
    mapp_MHD0404SStartupToRecoveryState();
    mapp_SourceChangeToSetWinProcess();
    report_video_timing();
    mapp_MHD0404STxConnectCheck();
    mapp_MHD0404SLinkStatusCheck();
}

void mapp_MHD0404SStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstUartCom_t pUarControltVar = &g_stUart5CommVar;

    pUarControltVar->SendPrintf("\r\n");
    delay_ms(50);
    if(pstPrjSaveVar->stPrjNameSet.m_u8Len != 0)
    {
        pUarControltVar->SendPrintf("product name:%s\r\n",pstPrjSaveVar->stPrjNameSet.sPrjName);
        delay_ms(50);
    }
    pUarControltVar->SendPrintf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                                pstBasicVar->stIPVar.u8IPAddr[1],
                                pstBasicVar->stIPVar.u8IPAddr[2],
                                pstBasicVar->stIPVar.u8IPAddr[3]);
    delay_ms(50);
    pUarControltVar->SendPrintf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                                pstBasicVar->stIPVar.u8NetMask[1],
                                pstBasicVar->stIPVar.u8NetMask[2],
                                pstBasicVar->stIPVar.u8NetMask[3]);
    delay_ms(50);
    pUarControltVar->SendPrintf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                                pstBasicVar->stIPVar.u8NetGate[1],
                                pstBasicVar->stIPVar.u8NetGate[2],
                                pstBasicVar->stIPVar.u8NetGate[3]);
    delay_ms(50);
    pUarControltVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock==1?"lock":"unlock");
    delay_ms(50);
}

void mapp_MHD0404S_SendDataToFpag(pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart2CommVar;
    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
    {
        pstUartCom_t pUart1CommVar = &g_stUart1CommVar;
        pUart1CommVar->SendBytes(pu8Data,u16Len,FALSE);
    }
}

/******************************************************
///Function Name:   send_cmd_to_fpga
///Description:     Send command to FPGA
///Input:           u8Addr(FPGA控制地址),u8Cmd(命令码),
///                 u16Len(数据长度),pu8Data(控制数据)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void send_cmd_to_fpga(uint8_t u8Addr,uint8_t u8Cmd,uint8_t* pu8Data,uint16_t u16Len)
{
    uint8_t l_u8Buff[100]={0xff,0xff,0xa5,0xfe,0xcc,0x01,0x00,0xaa};

    if(u16Len>99)return ;
    l_u8Buff[3]=u8Addr;
    l_u8Buff[4]=u8Cmd;

    l_u8Buff[5]=u16Len;
    l_u8Buff[6]=u16Len>>8;
    if(pu8Data!=NULL)
    {
        memcpy(l_u8Buff+7,pu8Data,u16Len);    // head
    }

    mapp_MHD0404S_SendDataToFpag(l_u8Buff,7+u16Len);
}

void mapp_MHD0404S_SetAudioSource(u8 u8Out, u8 u8AudioSrc)
{
    u8 l_u8Temp[2];

//    l_u8Temp[0] = u8AudioSrc;
//    send_cmd_to_fpga(u8Out,eFpgaRegAudioSwitch,l_u8Temp,1);
    l_u8Temp[0] = u8Out;
    l_u8Temp[1] = u8AudioSrc;
    send_cmd_to_fpga(0xFE,eFpgaRegAudioSwitch,l_u8Temp,2);
}

void mapp_MHD0404SWinDataCopy(u8 *u8Data, pstWindowsConfig_t pstWincfgDat)
{
    u8Data[0]  = pstWincfgDat->u8SrcAddr;
    u8Data[1]  = pstWincfgDat->u8LayerID;
    u8Data[2]  = pstWincfgDat->u8Layer_D0;
    u8Data[3]  = pstWincfgDat->u8Layer_D1;
    u8Data[4]  = pstWincfgDat->u8Layer_D2;
    u8Data[5]  = pstWincfgDat->u8Layer_D3;
    u8Data[6]  = pstWincfgDat->u16SrcAct_H&0xFF;
    u8Data[7]  = (pstWincfgDat->u16SrcAct_H>>8)&0xFF;
    u8Data[8]  = pstWincfgDat->u16SrcAct_V&0xFF;
    u8Data[9]  = (pstWincfgDat->u16SrcAct_V>>8)&0xFF;
    u8Data[10]  = pstWincfgDat->u16Src_X&0xFF;
    u8Data[11]  = (pstWincfgDat->u16Src_X>>8)&0xFF;
    u8Data[12]  = pstWincfgDat->u16Src_Y&0xFF;
    u8Data[13]  = (pstWincfgDat->u16Src_Y>>8)&0xFF;
    u8Data[14]  = pstWincfgDat->u16Src_H&0xFF;
    u8Data[15]  = (pstWincfgDat->u16Src_H>>8)&0xFF;
    u8Data[16]  = pstWincfgDat->u16Src_V&0xFF;
    u8Data[17]  = (pstWincfgDat->u16Src_V>>8)&0xFF;

    u8Data[18]  = pstWincfgDat->u32Wnd_X&0xFF;
    u8Data[19]  = (pstWincfgDat->u32Wnd_X>>8)&0xFF;
    u8Data[20]  = (pstWincfgDat->u32Wnd_X>>16)&0xFF;
    u8Data[21]  = (pstWincfgDat->u32Wnd_X>>24)&0xFF;
    u8Data[22]  = pstWincfgDat->u32Wnd_Y&0xFF;
    u8Data[23]  = (pstWincfgDat->u32Wnd_Y>>8)&0xFF;
    u8Data[24]  = (pstWincfgDat->u32Wnd_Y>>16)&0xFF;
    u8Data[25]  = (pstWincfgDat->u32Wnd_Y>>24)&0xFF;

    u8Data[26]  = pstWincfgDat->u32Wnd_H&0xFF;
    u8Data[27]  = (pstWincfgDat->u32Wnd_H>>8)&0xFF;
    u8Data[28]  = (pstWincfgDat->u32Wnd_H>>16)&0xFF;
    u8Data[29]  = (pstWincfgDat->u32Wnd_H>>24)&0xFF;
    u8Data[30]  = pstWincfgDat->u32Wnd_V&0xFF;
    u8Data[31]  = (pstWincfgDat->u32Wnd_V>>8)&0xFF;
    u8Data[32]  = (pstWincfgDat->u32Wnd_V>>16)&0xFF;
    u8Data[33]  = (pstWincfgDat->u32Wnd_V>>24)&0xFF;

    u8Data[34]  = pstWincfgDat->u32Srf_H&0xFF;
    u8Data[35]  = (pstWincfgDat->u32Srf_H>>8)&0xFF;
    u8Data[36]  = (pstWincfgDat->u32Srf_H>>16)&0xFF;
    u8Data[37]  = (pstWincfgDat->u32Srf_H>>24)&0xFF;
    u8Data[38]  = pstWincfgDat->u32Srf_V&0xFF;
    u8Data[39]  = (pstWincfgDat->u32Srf_V>>8)&0xFF;
    u8Data[40]  = (pstWincfgDat->u32Srf_V>>16)&0xFF;
    u8Data[41]  = (pstWincfgDat->u32Srf_V>>24)&0xFF;
    u8Data[42]  = pstWincfgDat->u8SrcInput;
}

u16 mapp_MHD0404SGetClipWinStartX(u8 u8Type,u8 u8Output,u16 u16Width)
{
    u16 l_u16Temp = 0;

    if(cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8StX_sf2 == 0)
    {
        l_u16Temp = 0;
    }
    else
    {
        l_u16Temp = (cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8StX_sf1*u16Width)
                    /cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8StX_sf2;
    }

    if(l_u16Temp > u16Width)
    {
        l_u16Temp = u16Width;
    }

    return l_u16Temp;
}

u16 mapp_MHD0404SGetClipWinStartY(u8 u8Type,u8 u8Output,u16 u16Height)
{
    u16 l_u16Temp = 0;

    if(cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8StY_sf2 == 0)
    {
        l_u16Temp = 0;
    }
    else
    {
        l_u16Temp = (cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8StY_sf1*u16Height)
                    /cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8StY_sf2;
    }

    if(l_u16Temp > u16Height)
    {
        l_u16Temp = u16Height;
    }

    return l_u16Temp;
}

u16 mapp_MHD0404SGetClipWinWidth(u8 u8Type,u8 u8Output,u16 u16Width)
{
    u16 l_u16Temp = u16Width;

    if(cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8Width_sf2 == 0)
    {
        l_u16Temp = u16Width;
    }
    else
    {
        l_u16Temp = (cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8Width_sf1*u16Width)
                    /cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8Width_sf2;
    }

    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
    }

    if(l_u16Temp > u16Width)
    {
        l_u16Temp = u16Width;
    }

    return l_u16Temp;
}

u16 mapp_MHD0404SGetClipWinHeight(u8 u8Type,u8 u8Output,u16 u16Height)
{
    u16 l_u16Temp = u16Height;

    if(cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8Height_sf2 == 0)
    {
        l_u16Temp = u16Height;
    }
    else
    {
        l_u16Temp = (cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8Height_sf1*u16Height)
                    /cg_stMHD0404SClipWinFactoryType_t[u8Type][u8Output].u8Height_sf2;
    }

    return l_u16Temp;
}

void mapp_MHD0404SWindowCfg(uint8_t out_ch,uint8_t in_ch)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Temp[50];
    stWindowsConfig_t stWinConfig;
    uint8_t layer_map[]={0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03};
    u8 l_u8Mode = pstBasicSpecVar->u8SplitMode;

    stWinConfig.u8SrcAddr = 0;
    stWinConfig.u8LayerID = layer_map[out_ch-1];
    stWinConfig.u8Layer_D0 = 0x00;
    stWinConfig.u8Layer_D1 = 0x00;
    stWinConfig.u8Layer_D2 = 0x00;
    stWinConfig.u8Layer_D3 = 0x00;
    switch(stWinConfig.u8LayerID)
    {
        case 0x01:
            stWinConfig.u8Layer_D0 = in_ch?0x80:0x00;
            break;
        case 0x02:
            stWinConfig.u8Layer_D1 = in_ch?0x80:0x00;
            break;
        case 0x03:
            stWinConfig.u8Layer_D2 = in_ch?0x80:0x00;
            break;
        case 0x04:
            stWinConfig.u8Layer_D3 = in_ch?0x80:0x00;
            break;
        default:
            break;
    }
    stWinConfig.u16SrcAct_H = (g_stRxResTable[in_ch-1].u16Width)?(g_stRxResTable[in_ch-1].u16Width):1920;
    stWinConfig.u16SrcAct_V = (g_stRxResTable[in_ch-1].u16Height)?(g_stRxResTable[in_ch-1].u16Height):1080;
//    stWinConfig.u16Src_X    = g_stWinConfigVar[out_ch-1].stClipWin.u16StartX;
//    stWinConfig.u16Src_Y    = g_stWinConfigVar[out_ch-1].stClipWin.u16StartY;
//    stWinConfig.u16Src_H    = (g_stWinConfigVar[out_ch-1].stClipWin.u16Width)?(g_stWinConfigVar[out_ch-1].stClipWin.u16Width):(stWinConfig.u16SrcAct_H);
//    stWinConfig.u16Src_V    = (g_stWinConfigVar[out_ch-1].stClipWin.u16Height)?(g_stWinConfigVar[out_ch-1].stClipWin.u16Height):(stWinConfig.u16SrcAct_V);
    stWinConfig.u16Src_X    = mapp_MHD0404SGetClipWinStartX(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_H);
    stWinConfig.u16Src_Y    = mapp_MHD0404SGetClipWinStartY(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_V);
    stWinConfig.u16Src_H    = mapp_MHD0404SGetClipWinWidth(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_H);
    stWinConfig.u16Src_V    = mapp_MHD0404SGetClipWinHeight(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_V);
    stWinConfig.u32Wnd_X    = 0;
    stWinConfig.u32Wnd_Y    = 0;
    stWinConfig.u32Wnd_H    = g_stWinConfigVar[out_ch-1].stDstWin.u16Width;
    stWinConfig.u32Wnd_V    = g_stWinConfigVar[out_ch-1].stDstWin.u16Height;
    stWinConfig.u32Srf_H    = (((uint64_t)stWinConfig.u16Src_H<<16))/(uint64_t)stWinConfig.u32Wnd_H;
    stWinConfig.u32Srf_V    = (((uint64_t)stWinConfig.u16Src_V<<16))/(uint64_t)stWinConfig.u32Wnd_V;
    stWinConfig.u8SrcInput  = in_ch;

//    dbg_printf("data size  == %d \r\n",sizeof(stWindowsConfig_t));

    mapp_MHD0404SWinDataCopy(l_u8Temp,&stWinConfig);
    send_cmd_to_fpga(out_ch,eFpgaRegOutWinCfg,l_u8Temp,43);

    return;
}

void mapp_MHD0404SFpgaWinCmdUpdate(u32 u32DelayMs)
{
    u8 l_u8cmd = 1;
    delay_ms_cpu(u32DelayMs);
    send_cmd_to_fpga(0xFE,eFpgaRegOutUpdate,&l_u8cmd,1);
}

void mapp_MHD0404S_TimingConfig(uint8_t ch,uint8_t idx)
{
    u8 l_u8Temp[50];
//    if(/*idx==0||*/idx>=(sizeof(res_table)/sizeof(res_table[0])))
//        return ;
    memset(l_u8Temp,0,50);

//    dbg_printf("u16HActive  == %d \r\n",res_table[idx].u16HActive);
//    dbg_printf("u16HTotal   == %d \r\n",res_table[idx].u16HTotal);
//    dbg_printf("u16HBP      == %d \r\n",res_table[idx].u16HBP);
//    dbg_printf("u16HFP      == %d \r\n",res_table[idx].u16HFP);
//    dbg_printf("u8HSync     == %d \r\n",res_table[idx].u16HSync);
//    dbg_printf("u8HPol      == %d \r\n",res_table[idx].u8HPol);
//    dbg_printf("u16VActive  == %d \r\n",res_table[idx].u16VActive);
//    dbg_printf("u16VTotal   == %d \r\n",res_table[idx].u16VTotal);
//    dbg_printf("u16VBP      == %d \r\n",res_table[idx].u16VBP);
//    dbg_printf("u16VFP      == %d \r\n",res_table[idx].u16VFP);
//    dbg_printf("u8VSync     == %d \r\n",res_table[idx].u16VSync);
//    dbg_printf("u8VPol      == %d \r\n",res_table[idx].u8VPol);
//    dbg_printf("u8VClk      == %d \r\n",res_table[idx].u8VClk);
//    dbg_printf("u16PClk     == %d \r\n",res_table[idx].u16PClk);
//    dbg_printf("u8Vic       == %d \r\n",res_table[idx].u8Vic);
    l_u8Temp[0]  = res_table[idx].u16HActive&0xFF;
    l_u8Temp[1]  = (res_table[idx].u16HActive>>8)&0xFF;
    l_u8Temp[2]  = res_table[idx].u16HTotal&0xFF;
    l_u8Temp[3]  = (res_table[idx].u16HTotal>>8)&0xFF;
    l_u8Temp[4]  = res_table[idx].u16HBP&0xFF;
    l_u8Temp[5]  = (res_table[idx].u16HBP>>8)&0xFF;
    l_u8Temp[6]  = res_table[idx].u16HFP&0xFF;
    l_u8Temp[7]  = (res_table[idx].u16HFP>>8)&0xFF;
    l_u8Temp[8]  = res_table[idx].u16HSync&0xFF;
    l_u8Temp[9]  = (res_table[idx].u16HSync>>8)&0xFF;
    l_u8Temp[10] = res_table[idx].u8HPol&0xFF;
    l_u8Temp[11] = res_table[idx].u16VActive&0xFF;
    l_u8Temp[12] = (res_table[idx].u16VActive>>8)&0xFF;
    l_u8Temp[13] = res_table[idx].u16VTotal&0xFF;
    l_u8Temp[14] = (res_table[idx].u16VTotal>>8)&0xFF;
    l_u8Temp[15] = res_table[idx].u16VBP&0xFF;
    l_u8Temp[16] = (res_table[idx].u16VBP>>8)&0xFF;
    l_u8Temp[17] = res_table[idx].u16VFP&0xFF;
    l_u8Temp[18] = (res_table[idx].u16VFP>>8)&0xFF;
    l_u8Temp[19] = res_table[idx].u16VSync&0xFF;
    l_u8Temp[20] = (res_table[idx].u16VSync>>8)&0xFF;
    l_u8Temp[21] = res_table[idx].u8VPol&0xFF;

    send_cmd_to_fpga(ch,eFpgaRegOutTiming,l_u8Temp,22);
}

void mapp_MHD0404SResolutionConfig(u8 u8Out,u8 u8ResID,bool bUpdate)
{
    const u8 cl_u8ChMap[16] = {0,0,2,2};
    mapp_MHD0404S_TimingConfig(u8Out+1,u8ResID);
    mapp_IDTSetPclk(u8Out/8,res_table[u8ResID].u16PClk,cl_u8ChMap[u8Out]);
    SwitchResolutionToRefreshWindowns(u8Out,u8ResID,bUpdate);
}

void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID,bool bUpdate)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    mapp_MHD0404SWincfgNoiseMute(TRUE);
    switch(u8Out)
    {
        case 0:
        case 1:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i].stDstWin.u16Height= res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i);
                    mapp_MHD0404SWindowCfg(i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
                    pstBasicSpecVar->u8TxResolutionID[i]=u8ResID;
                }
            }
            break;
        case 2:
        case 3:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+2].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+2].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                    mapp_GsvSetTimingChange(i+2);
                    mapp_MHD0404SWindowCfg(i+3,pstPrjSaveVar->stHdmiSel.u8SwChannel[i+2]+1);
                    pstBasicSpecVar->u8TxResolutionID[i+2]=u8ResID;
                }
            }
            break;

        default:
            break;
    }

    if(bUpdate)
    {
        mapp_MHD0404SFpgaWinCmdUpdate(10);
        mapp_MHD0404SWincfgNoiseMute(FALSE);
    }
}

void mapp_MHD0404SSourceChange(u8 u8Out,u8 u8In)
{
    mapp_MHD0404SWindowCfg(u8Out,u8In);
    mapp_MHD0404S_SetAudioSource(u8Out,u8In);
    if(u8Out==(Def_outport_num-1))
    {
        mapp_MHD0404S_SetAudioSource(Def_outport_num+1,u8In);
    }
    else if(u8Out==(Def_outport_num))
    {
        mapp_MHD0404S_SetAudioSource(Def_outport_num+2,u8In);
    }
}

void mapp_MHD0404SWincfgNoiseMute(bool bMute)
{
    u8 l_u8Data[1];

    l_u8Data[0] = bMute;
    send_cmd_to_fpga(0x01,eFpgaRegWinCfgAudioMuteEn,l_u8Data,1);
}

/* **volume rang 0~0x7C 0x7C=0db 0=-124db** */
void mapp_MHD0404SVolumeAdjust(u8 u8Out,u8 u8Volume)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8Data[2];

    l_u8Addr = u8Out;
    l_u8Cmd = eFpgaRegVolumeAdj;
    l_u8Data[0] = u8Volume;
    send_cmd_to_fpga(l_u8Addr,l_u8Cmd,l_u8Data,1);
}

void mapp_GetFpgaVersion(u8 u8Addr)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8Data = 0;

    l_u8Addr = u8Addr;
    l_u8Cmd = eFpgaRegGetVersion;
    send_cmd_to_fpga(l_u8Addr,l_u8Cmd,&l_u8Data,1);
}

#define NUM_LOOP    10
void mapp_MHD0404SStartupToRecoveryState(void)
{
    static bool sl_bStartupFlag = TRUE;
    static eStartupLoopType_t sl_eStartupLoop = eSt_None;
    static u8 sl_u8LoopCnt = 0;

    if(!mapp_MHD0404S_PlatformTimerExpired(emUserTimer_StartupTime))
        return;

    if(sl_bStartupFlag)
    {
        sl_bStartupFlag = FALSE;
        sl_eStartupLoop = eSt_SInit;
    }

    switch(sl_eStartupLoop)
    {
        case eSt_SInit:
            {
                sl_eStartupLoop++;
            }
            break;
        case eSt_GetTxVersion:
            {
                dbg_printf("Get Tx Version\r\n");
                mapp_GetFpgaVersion(1);
                delay_ms(50);
                mapp_MHD0404S_PlatformTimerSet(emUserTimer_StartupTime,200);
                sl_u8LoopCnt++;
                if(g_u8Fwversion[0]||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_SetTxResolution:
            {
//                pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
//                pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
                pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
                pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
                u8 i;

                dbg_printf("Set Tx Resolution\r\n");
                for(i=0;i<Def_outport_num;i++)
                {
                    mapp_MHD0404SResolutionConfig(i,pstBasicVar->u8TxResolutionID[i],(i==(Def_outport_num-1))?TRUE:FALSE);
                    //mapp_MHD0404SWindowCfg(i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);

//                    mapp_MHD0404S_SetAudioSource(i+1,(pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1));

//                    if((i+1)==(Def_outport_num-1))
//                    {
//                        mapp_MHD0404S_SetAudioSource(Def_outport_num+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
//                    }
//                    else if((i+1)==(Def_outport_num))
//                    {
//                        mapp_MHD0404S_SetAudioSource(Def_outport_num+2,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
//                    }
                    //delay_ms(50);
                }
                //mapp_MHD0404SFpgaWinCmdUpdate(10);
                delay_ms(50);
                mapp_MHD0404S_PlatformTimerSet(emUserTimer_StartupTime,500);
                sl_eStartupLoop++;
            }
            break;

        case eSt_GetRxVersion:
            {
                dbg_printf("Get Rx Version\r\n");
                mapp_GetFpgaVersion(0xFE);
                delay_ms(100);
                mapp_MHD0404S_PlatformTimerSet(emUserTimer_StartupTime,200);
                sl_u8LoopCnt++;
                if((g_u8Fwversion[1])||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_SetTxVolume:
            {
                u8 i;

                dbg_printf("Set Tx Volume\r\n");
                for(i=0;i<Def_outport_num;i++)
                {
                    mapp_MHD0404SVolumeAdjust(i+1,0x7C);
                }
                mapp_MHD0404S_PlatformTimerSet(emUserTimer_StartupTime,200);
                sl_eStartupLoop++;
            }
            break;
        case eSt_None:
        default:
            break;
    }
}

void mapp_SourceChangeToSetWinProcess(void)
{
    u8 l_u8Src,l_u8OutIdex;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    bool l_bUpdateFlag = FALSE;
    u16 l_u16HActive,l_u16VActive;
    u32 l_u32Pclk;
    u8  l_u8Vfreq=0;

    if(!mapp_MHD0404S_PlatformTimerExpired(emUserTimer_TimingChange))
        return;
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,500);

    for(l_u8Src = 0;l_u8Src<_HDMI_INPUT_PORT_USED_NUMBER;l_u8Src++)
    {
        if(mapp_it66021_GetVideoChangeSta(l_u8Src) != emIt66021_VidChgNone)
        {
            it66021_GetInputVFreq(l_u8Src,&l_u8Vfreq);
            g_stRxResTable[l_u8Src].u8ScanMode = it66021_GetInputTiming(l_u8Src,&l_u16HActive,&l_u16VActive,&l_u32Pclk);
            g_stRxResTable[l_u8Src].u16Width  = l_u16HActive;
            g_stRxResTable[l_u8Src].u16Height = l_u16VActive;
            g_stRxResTable[l_u8Src].u16VFreq  = l_u8Vfreq;
            for(l_u8OutIdex = 0; l_u8OutIdex<Def_outport_num;l_u8OutIdex++)
            {
                if(pstPrjSaveVar->stHdmiSel.u8SwChannel[l_u8OutIdex] == (l_u8Src))
                {
                    dbg_printf("source[%d] change to reconfig out[%d]\r\n",l_u8Src,l_u8OutIdex);
                    l_bUpdateFlag = TRUE;
                    mapp_MHD0404SWincfgNoiseMute(TRUE);
                    mapp_MHD0404SWindowCfg(l_u8OutIdex+1,(l_u8Src+1));
                    mapp_MHD0404S_SetAudioSource(l_u8OutIdex+1,(l_u8Src+1));

                    if((l_u8OutIdex+1)==(Def_outport_num-1))
                    {
                        mapp_MHD0404S_SetAudioSource(Def_outport_num+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[l_u8OutIdex]+1);
                    }
                    else if((l_u8OutIdex+1)==(Def_outport_num))
                    {
                        mapp_MHD0404S_SetAudioSource(Def_outport_num+2,pstPrjSaveVar->stHdmiSel.u8SwChannel[l_u8OutIdex]+1);
                    }
//                    delay_ms(50);
                }
            }
            if(l_bUpdateFlag)
            {
                mapp_MHD0404SFpgaWinCmdUpdate(10);
                mapp_MHD0404SWincfgNoiseMute(FALSE);
            }
            if(mapp_it66021_GetVideoChangeSta(l_u8Src) == emIt66021_VidChgeVidOn)
                g_stLinkSta.u8InLinkSta[l_u8Src] = 1;
            else
                g_stLinkSta.u8InLinkSta[l_u8Src] = 0;
            mapp_it66021_SetVideoChangeSta(l_u8Src, (u8)emIt66021_VidChgNone);
        }
    }
}

#define REPORT_TIMING_TIME  3000
void report_video_timing(void)
{
    uint8_t buff[20];
    uint16_t i,j;
    u16 l_u16HActive,l_u16VActive;
    u32 l_u32Pclk;
    u8  l_u8Vfreq=0;

    if(!mapp_MHD0404S_PlatformTimerExpired(emUserTimer_ReportTiming))
        return;
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_ReportTiming,REPORT_TIMING_TIME);

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        it66021_GetInputVFreq(i,&l_u8Vfreq);
        g_stRxResTable[i].u8ScanMode = it66021_GetInputTiming(i,&l_u16HActive,&l_u16VActive,&l_u32Pclk);
        g_stRxResTable[i].u16Width  = l_u16HActive;
        g_stRxResTable[i].u16Height = l_u16VActive;
        g_stRxResTable[i].u16VFreq  = l_u8Vfreq;

        j=0;
        buff[j++] = i+1; //ch
        buff[j++] = l_u16HActive >> 8;
        buff[j++] = l_u16HActive & 0xFF;

        buff[j++] = l_u16VActive >> 8;
        buff[j++] = l_u16VActive & 0xFF;

        send_cmd_to_fpga(0xFE,eFpgaRegReportInputTiming,buff,5);
    }
}

void mapp_MHD0404SIDT5VInit(void)
{
    uint8_t tmp, timeout=100;
    uint8_t l_u8DevId=0;

    for(l_u8DevId=0;l_u8DevId<1;l_u8DevId++)
    {
        do {
            delay_ms(5);
            i2c_IDT5V_write_one_byte(l_u8DevId,0x00,0x01);    //mode: Software Mode control
            tmp = i2c_IDT5V_read_one_byte(l_u8DevId,0x00);

        } while((tmp != 0x01) && --timeout);

        if(tmp != 0x01)
        {
            //Uart_printf("IDT_5V[%d]_init Fail\r\n",l_u8DevId);
            return;
        }

        i2c_IDT5V_write_one_byte(l_u8DevId,0x01,0x00);    //Select CONFIG0
        i2c_IDT5V_write_one_byte(l_u8DevId,0x02,0x00);    //outputs enabled
        i2c_IDT5V_write_one_byte(l_u8DevId,0x03,0xFF);    //output not suspend

       //0x96
        i2c_IDT5V_write_one_byte(l_u8DevId,0xC0,0x96);    //crystal/REFIN, auto switch mode, PLL3 normal, SRC0: DIV3, SRC1: PLL2
        //PLL0: 148.5M
        i2c_IDT5V_write_one_byte(l_u8DevId,0x10,0x04);    //D=3
        i2c_IDT5V_write_one_byte(l_u8DevId,0x18,0x16);    //N=11
        i2c_IDT5V_write_one_byte(l_u8DevId,0x1C,0x00);    //A=0, M=2*N=22 >10~8026
        //PLL1: 148.5M-FORCE ON
        i2c_IDT5V_write_one_byte(l_u8DevId,0x28,0x04);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x30,0x2c);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x34,0x00);
        //PLL2: 148.5M
        i2c_IDT5V_write_one_byte(l_u8DevId,0x3C,0x10); //Loop Parameter
        i2c_IDT5V_write_one_byte(l_u8DevId,0x40,0x04);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x48,0x2c);  //N=297
        i2c_IDT5V_write_one_byte(l_u8DevId,0x4C,0x80);    //Spread Spectrum Enable
        //PLL3: 100M
        IDT_5V_OutProgramClock(l_u8DevId,3,10000);
//        i2c_IDT5V_write_one_byte(l_u8DevId,0x5C,0x04);//D=2   D>1~127
//        i2c_IDT5V_write_one_byte(l_u8DevId,0x64,0x2c);//N=22  N>12~4095

            /*SRC6: PLL1 OUT3 OUT6
              SRC5: PLL3 OUT5
              SRC4: PLL0 OUT4
              SRC2: PLL2 OUT1 OUT2
            */
        i2c_IDT5V_write_one_byte(l_u8DevId,0xCC,0xBE);    //SRC6: PLL1, SRC5: PLL3, SRC4: PLL0
        i2c_IDT5V_write_one_byte(l_u8DevId,0xC4,0x5D);    //SRC3: PLL1, SRC2: PLL2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x75,0x00);    //S3=0 - Both from DIV6, S1=0 - Both from DIV2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x7A,0x05);    //OUT5 and OUT5b: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x79,0x05);    //OUT4 and OUT4b: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x78,0x05);    //OUT3 and OUT6: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x76,0x05);    //OUT1 and OUT2: invert clock, LVDS
        //DIV1~6: /2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x88,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x90,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x94,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x9C,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0xA0,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0xA8,0x0F);
    }
}

void mapp_MHD0404STxConnectCheck(void)
{
    u8 i;
    if(!mapp_MHD0404S_PlatformTimerExpired(emUserTimer_CheckTxConnect))
        return;
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_CheckTxConnect,500);

    for(i=0;i<Def_outport_num;i++)
    {
        if(g_stLinkSta.u8OutLinkSta[i] != GsvGetTxHpdStatus(i))
        {
            g_stLinkSta.u8OutLinkSta[i] = GsvGetTxHpdStatus(i);
        }
    }
}

void mapp_MHD0404SLinkStatusCheck(void)
{
    u8 i;
    bool l_bLinkChange = FALSE;
    pstUartCom_t pComVar = &g_stUart5CommVar;

    if(!mapp_MHD0404S_PlatformTimerExpired(emUserTimer_CheckLinkStatus))
        return;
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_CheckLinkStatus,2500);

    for(i=0;i<Def_outport_num;i++)
    {
        if(g_stLinkSta.u8OutLinkSta[i] != g_stPreLinkSta.u8OutLinkSta[i])
        {
            g_stPreLinkSta.u8OutLinkSta[i] = g_stLinkSta.u8OutLinkSta[i];
            l_bLinkChange = TRUE;
        }
    }

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(g_stLinkSta.u8InLinkSta[i] != g_stPreLinkSta.u8InLinkSta[i])
        {
            g_stPreLinkSta.u8InLinkSta[i] = g_stLinkSta.u8InLinkSta[i];
            l_bLinkChange = TRUE;
        }
    }

    if(l_bLinkChange)
    {
        pComVar->SendPrintf("ch  1 2 3 4\r\n");
        pComVar->SendPrintf("in  %c %c %c %c\r\n",
                        g_stLinkSta.u8InLinkSta[0]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[1]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[2]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[3]?'Y':'N'
                        );
        pComVar->SendPrintf("out %c %c %c %c\r\n",
                        g_stLinkSta.u8OutLinkSta[0]?'Y':'N',
                        g_stLinkSta.u8OutLinkSta[1]?'Y':'N',
                        g_stLinkSta.u8OutLinkSta[2]?'Y':'N',
                        g_stLinkSta.u8OutLinkSta[3]?'Y':'N'
                        );
    }
}

void mapp_MHD0404SSplitModeChannelChange(u8 u8Out,u8 u8In)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 l_u8SwitchCnt = 0,i;

    switch(pstBasicVar->u8SplitMode)
    {
        default:
        case eSpMode_None:
            pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            break;
        case eSpMode_1x2:
            if(u8Out<2)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 2;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
        case eSpMode_1x3:
            if(u8Out<3)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 3;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
        case eSpMode_1x4:
            if(u8Out<4)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[3] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 4;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
        case eSpMode_2x2:
            if(u8Out<4)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[3] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 4;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
        case eSpMode_2x1:
            if(u8Out<2)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 2;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
        case eSpMode_3x1:
            if(u8Out<3)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 3;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
        case eSpMode_4x1:
            if(u8Out<4)
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[0] = u8In;
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[3] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                l_u8SwitchCnt = 4;
            }
            else
            {
                pstPrjSaveVar->stHdmiSel.u8SwChannel[u8Out] = u8In;
            }
            break;
    }

    if(l_u8SwitchCnt)
    {
        for(i=0;i<l_u8SwitchCnt;i++)
        {
            mapp_MHD0404SSourceChange(i+1,u8In+1);
        }
    }
    else
    {
        mapp_MHD0404SSourceChange(u8Out+1,u8In+1);
    }
    SetUserData_HdmiSel();
}

#define DM_FW_BYTES_MIX         (15*1024)
#define DM_FW_BYTES_MAX         (192*1024)  //100KB
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

uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len)
{
    uint8_t l_u8RetVal = 1;

    mapp_MHD0404S_PlatformTimerSet(emUserTimer_ReportTiming,10000);
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,10000);
    g_stUpdateVar.u8UpdateMode = u8Id;
    switch(u8Step)
    {
        case 0:
            {
                mapp_MHD0404S_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
                g_stUpdateVar.u8UpdateFlag=1;
                if(u8Id==eUpdID_MCU)
                {
                    l_u8RetVal = mcu_file_updata(0,0,0);
                }
                else if(u8Id==eUpdID_RXFPGA||u8Id==eUpdID_TXFPGA)
                {
                    l_u8RetVal = fpga_file_updata(0,0,0);
                }
                g_stUpdateVar.u16PackIndex = 0;
            }
            break;
        case 1:
            {
                g_stUpdateVar.u16PackIndex++;
                if(u8Id==eUpdID_MCU)
                {
                    l_u8RetVal=mcu_file_updata(1,pu8Data,u16Len);
                }
                else if(u8Id==eUpdID_RXFPGA||u8Id==eUpdID_TXFPGA)
                {
                    l_u8RetVal = fpga_file_updata(1,pu8Data,u16Len);
                }
            }
            break;
        case 2:
            {
                if(u8Id==eUpdID_MCU)
                {
                    l_u8RetVal=mcu_file_updata(2,pu8Data,u16Len);
                }
                else if(u8Id==eUpdID_RXFPGA||u8Id==eUpdID_TXFPGA)
                {
                    l_u8RetVal = fpga_file_updata(2,0,0);
                }
                g_stUpdateVar.u8UpdateFlag=0;
            }
            break;
        default:
            break;
    }

    return l_u8RetVal;
}

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
            do{
                f_sta = fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR)+(i<<3));
                if(f_sta != 0)
                {
                    fmc_lock();
                    return 0;
                }
                i++;
            }while(i>j);
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

u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar)
{
    if(g_stUpdateVar.u8UpdateFlag==1)
    {
        mapp_MHD0404S_PlatformTimerSet(emUserTimer_FirmwareUpdate,2000);
        UpdateFunction(g_stUpdateVar.u8UpdateMode,1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);

        pComVar->SendPrintf("pack %d\r\n",g_stUpdateVar.u16PackIndex);
        return 1;
    }
    else
        return 0;
}

void mapp_FirmwareUpdateTimeOutFunction(void)
{
    if(g_stUpdateVar.u8UpdateFlag==1&&mapp_MHD0404S_PlatformTimerExpired(emUserTimer_FirmwareUpdate))
    {
        UpdateFunction(g_stUpdateVar.u8UpdateMode,2,NULL,0);
    }
}

#if 1
//fpga update
uint8_t fpga_file_updata(uint8_t step ,uint8_t *data ,uint16_t len)
{
    image_header_t packet;
    if(step==0)
    {
        sf_FlashSelect(0);
        mapp_SelectFpgaFlashUpdateMode(g_stUpdateVar.u8UpdateMode-eUpdID_RXFPGA,TRUE);
        delay_ms(5);
        sf_FlashInit();
        g_stUpdateVar.u32TotalBytes = 0;
        g_stUpdateVar.u32PageIndex = 0;
        dbg_printf("start fpga update!\r\n");
    }
    else if(step==1)
    {
        if(len<1024)
        {
            memcpy(g_stUpdateVar.u8LastData,data,len);

            memcpy((uint8_t*)&packet,g_stUpdateVar.u8LastData, sizeof(image_header_t));
            if((packet.ih_magic==0x4e545047))
            {
                dbg_printf("iap end\r\n");
                dbg_printf("total_bytes [%d]\r\n",g_stUpdateVar.u32TotalBytes );
                dbg_printf("ih_magic [0x%x]\r\n",packet.ih_magic );
                dbg_printf("ih_arch [%d]\r\n",packet.ih_arch );
                dbg_printf("ih_name [%s]\r\n",packet.ih_name );
            }
            else
                sf_WriteBuffer(data,g_stUpdateVar.u32TotalBytes,len);
        }
        else
            sf_WriteBuffer(data,g_stUpdateVar.u32TotalBytes,len);
        g_stUpdateVar.u32TotalBytes +=len;
    }
    else if(step==2)
    {
        dbg_printf("total_bytes [%d]\r\n",g_stUpdateVar.u32TotalBytes );
        dbg_printf("fpga updata end\r\n");
        mapp_SelectFpgaFlashUpdateMode(g_stUpdateVar.u8UpdateMode-eUpdID_RXFPGA,FALSE);
        SYS_SoftReset();
    }
    return 0;
}
#endif

void mapp_GsvChipInitStatus(void)
{
    u8 i;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    for(i=0;i<Def_outport_num;i++)
    {
//        pstBasicVar->u8TxHdcpMode[i] = l_s8HdcpMode;
        GsvSetTxHdcpStatus(i,pstBasicVar->u8TxHdcpMode[i]);
        GsvSetTxMode(i,pstBasicVar->u8TxFormatSta[i]?0:1);
    }
}


