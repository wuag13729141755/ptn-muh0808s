#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"


stProjectSpecVar_t  stMHD1616SSWBDSpecVar;
static bool         sg_bChangePowerStateFlag = FALSE;
stADN4604SwitchTable_T g_stSwitchTable;

stTxEdidData_t g_stTxEdidVar[Def_outport_num];
static volatile u32 g_MHD1616SSWBD_Timers[emUserTimer_Max];

stResolution_t g_stRxResTable[_HDMI_INPUT_PORT_USED_NUMBER];
stLinkSta_t    g_stLinkSta;
stWinConfigVar_t    g_stWinConfigVar[Def_outport_num];
u8 g_u8CardType[3]; // 0:Tx,1:rx1,2:rx2

extern stProjectGlobalAllVar_t g_stPrjGloVar;

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

void MHD1616SSWBD_1msTimerIrq(void);
void mapp_MHD1616SADN4604SwitchProcess(void);
void mapp_MHD1616SSWBDInitAdn4604(void);
void mapp_MHD1616SSWBDCheckCardInfo(void);
void mapp_SourceChangeToSetWinProcess(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_MHD1616SSWBD_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_MHD1616SSWBD_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_MHD1616SSWBD_PlatformTimerGet(uint8_t eTimer)
{
    return(g_MHD1616SSWBD_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_MHD1616SSWBD_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_MHD1616SSWBD_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_MHD1616SSWBD_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_MHD1616SSWBD_Timers[i] > 0)
        {
            g_MHD1616SSWBD_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_MHD1616SSWBD_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_MHD1616SSWBD_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void MHD1616SSWBD_1msTimerIrq(void)
{
    mapp_MHD1616SSWBD_PlatformTimerTask();
}

void MHD1616SSWBD_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);
    memset((u8*)&g_stSwitchTable,0,sizeof(stADN4604SwitchTable_T));
    memset((u8*)&g_stRxResTable,0,sizeof(stResolution_t)*_HDMI_INPUT_PORT_USED_NUMBER);
    memset((u8*)&g_stLinkSta,0,sizeof(stLinkSta_t));


}

void MHD1616SSWBD_SpecificVarDefault(void)
{
    MHD1616SSWBD_SpecBasicVarDefault();
    MHD1616SSWBD_SpecCustomEdidVarDefault();
}

void MHD1616SSWBD_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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

}

void MHD1616SSWBD_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
}

bool MHD1616SSWBD_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        MHD1616SSWBD_SpecBasicVarDefault();
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void MHD1616SSWBD_PrjStatePowerOnInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    pstUartCom_t pUarControltVar = &g_stUart5CommVar;
    pstUartCom_t pUarRemoteVar = &g_stUart4CommVar;

    memset(g_u8CardType,0,3);
    memset(g_stTxEdidVar,0,sizeof(stTxEdidData_t));

    pFuncPrj1MsLoopHook = MHD1616SSWBD_1msTimerIrq;
    pADN4604Process = mapp_MHD1616SADN4604SwitchProcess;
    pADN4604InitFunc = mapp_MHD1616SSWBDInitAdn4604;
    g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;
    mapp_RevisionMake();

    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
            g_stSwitchTable.u8OutCh[i] = pstPrjSaveVar->stHdmiSel.u8SwChannel[i];
            g_stSwitchTable.bUpdateFlag[i] = TRUE;
            g_stSwitchTable.bCloseEnableFlag[i] = FALSE;
            SwitchResolutionToRefreshWindowns(i,pstBasicVar->u8TxResolutionID[i]);
        }
    }

    pUarControltVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicVar->u8InRs232BaudrateID), DEF_UART_DATA_CONFIG);
    pUarRemoteVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicVar->u8OutRs232BaudrateID), DEF_UART_DATA_CONFIG);
    delay_ms(1000);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,2000);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,5000);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,7000);
}

void mapp_MHD1616SSWBDLedProcess(void)
{
    static bool sl_bLedSta = FALSE;
    if(mapp_MHD1616SSWBD_PlatformTimerExpired(emUserTimer_LedActive))
    {
        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_LedActive,500);
        if(sl_bLedSta)
        {
            sl_bLedSta = FALSE;
            led_on(led_name_ActiveState);
        }
        else
        {
            sl_bLedSta = TRUE;
            led_off(led_name_ActiveState);
        }
    }
}

void MHD1616SSWBD_AllLedOff(void)
{
}

void mapp_MHD1616SSWBDMainFunction(void)
{
    mapp_MHD1616SSWBDLedProcess();
    mapp_MHD1616SSWBDStartupToRecoveryState();
    mapp_MHD1616SSWBDCheckCardInfo();
    mapp_SourceChangeToSetWinProcess();
}


void mapp_MHD1616SSWBDStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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

void mapp_MHD1616SSWBDInitAdn4604(void)
{
    u8 i;

    for(i=0;i<DEV_NUM_ADN4604;i++)
    {
        write_4604(i,0x00,0x01);      // Reset ADN4604 register
        delay_us(3000);
        write_4604(i,0x81,ADN4604_MAP_SELECT);      // First register mapping : 0,high->low  1,low->high
        delay_us(3000);
        write_4604(i,0xF0,0x00);      // Enable in and out port resistance
        delay_us(3000);
        write_4604(i,0x12,0x00);      // signal not invert
        delay_us(3000);
        write_4604(i,0x13,0x00);      // signal not invert
        delay_us(3000);
        {
            u8 j;
            for(j=0x30;j<=0x4E;j=j+2)
            {
                write_4604(i,j,0xFF);       //Tx Advandced Drive 0
                delay_us(2000);
                write_4604(i,j+1,0xFF);   //Tx Advandced Drive 1
                delay_us(2000);
            }
        }
//            dbg_printf("read_4604 [%d][0xff]=[%d]\r\n",i,read_4604(i,0xFF));
//            dbg_printf("read_4604 [%d][0xfe]=[%d]\r\n",i,read_4604(i,0xFE));
    }
}

void mapp_MHD1616SADN4604SwitchProcess(void)
{
    int i;
    uint8_t in,ch=0;
    //ADN4604 map 对应关系
    const u8 ch_in[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//芯片上输出通道选择的输入源端口号
#if ADN4604_MAP_SELECT
    const u8 ch_out[16]= {0x98,0x98,0x99,0x99,0x9a,0x9a,0x9b,0x9b,0x9c,0x9c,0x9d,0x9d,0x9e,0x9e,0x9f,0x9f};//4位的值表示该通道的输入端口号为哪一个
#else
    const u8 ch_out[16]= {0x90,0x90,0x91,0x91,0x92,0x92,0x93,0x93,0x94,0x94,0x95,0x95,0x96,0x96,0x97,0x97};//4位的值表示该通道的输入端口号为哪一个
#endif
    const u8 ch_out_en[16]={0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2C,0x2d,0x2e,0x2f};//控制0-15个输出端口使能0x34配置值
    static u8 ch_Out_InputNumBack[16]= {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    const u8 Ka_ch_adjust_in[16]={ 7, 6, 5, 4, 3, 2, 1, 0, 15,14,13,12,11,10, 9, 8};
    const u8 Ka_ch_adjust_out[16]={15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    unsigned char Data=0;
    bool  l_bEnUpdateFlag = FALSE;

    taskENTER_CRITICAL();

    for(i=0; i<Def_outport_num; i++)
    {
        if(TRUE == g_stSwitchTable.bUpdateFlag[i])
        {
//            mapp_MHD1616SSWBDWindowCfg(i+1,(g_stSwitchTable.u8OutCh[i]+1));
            mapp_MHD1616SSWBD_SetAudioSource(i+1,g_stSwitchTable.u8OutCh[i]+1);
            if(i==14)
            {
                mapp_MHD1616SSWBD_SetAudioSource(Def_outport_num+1,g_stSwitchTable.u8OutCh[i]+1);
            }
            else if(i==15)
            {
                mapp_MHD1616SSWBD_SetAudioSource(Def_outport_num+2,g_stSwitchTable.u8OutCh[i]+1);
            }
            in=Ka_ch_adjust_in[g_stSwitchTable.u8OutCh[i]];
            ch = Ka_ch_adjust_out[i];
            if(g_stSwitchTable.bCloseEnableFlag[i] == FALSE)
            {
                write_4604(0,ch_out_en[ch],0x04);//首先失能对应的输出通道号
                //delay_us(20);
                if(ch % 2)
                {//是为奇数则找到自己同寄存器的低4位值
                    Data =  ((ch_in[in])<<4) ;     //高4位
                    Data |= ch_Out_InputNumBack[ch - 1];//低4位
                }
                else
                {//是为偶数则找到自己同寄存器的高4位值
                    Data =  ((ch_Out_InputNumBack[ch + 1])<<4) ;       //高4位
                    Data |= ch_in[in];          //低4位
                }
                write_4604(0,ch_out[ch],Data);
                //delay_us(20);
                write_4604(0,ch_out_en[ch],0x70|ADN4604_PE_VAR);
                //delay_us(20);
                ch_Out_InputNumBack[ch] = ch_in[in];//备份这个输出通道下对应的输入端口号
            }
            else
            {
                write_4604(0,ch_out_en[ch],0x04);
                //delay_us(20);
            }
            l_bEnUpdateFlag = TRUE;
            g_stSwitchTable.bUpdateFlag[i] = FALSE;
        }
    }

    if(l_bEnUpdateFlag)
    {
        write_4604(0,0x80,0x01);                    //updata刷新数据
        //mapp_MHD1616SSWBDFpgaWinCmdUpdate(10);
    }
    taskEXIT_CRITICAL();
}

void mapp_MHD1616SSWBD_SendDataToFpag(pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart2CommVar;
    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
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

    mapp_MHD1616SSWBD_SendDataToFpag(l_u8Buff,7+u16Len);
}

void mapp_MHD1616SSWBD_SetAudioSource(u8 u8Out, u8 u8AudioSrc)
{
    u8 l_u8Temp[2];

    l_u8Temp[0] = u8Out;
    l_u8Temp[1] = u8AudioSrc;
    send_cmd_to_fpga(0xFD,0x3B,l_u8Temp,2);
}

void mapp_SendCmdToInOutCard(u8 u8DevAddr,u8 u8Cmd,pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;
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
    {
        pstUartCom_t pUartdbgVar = &g_stUart5CommVar;
        pUartdbgVar->SendBytes(l_u8SendValue,l_u16index+u16Len,FALSE);
    }
#endif
}

void mapp_SendDataToInOutCard(pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;

    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
}

void mapp_MHD1616SSWBDSendCmdToTXBD(u8 u8TxID,u8 u8cmd,u8 *pu8Data, u16 u16Len)
{
    mapp_SendCmdToInOutCard(0x80|u8TxID,u8cmd,pu8Data,u16Len);
}

void mapp_MHD1616SSWBDSendCmdToRXBD(u8 u8TxID,u8 u8cmd,u8 *pu8Data, u16 u16Len)
{
    mapp_SendCmdToInOutCard(u8TxID,u8cmd,pu8Data,u16Len);
}

void mapp_MHD1616SSWBDWinDataCopy(u8 *u8Data, pstWindowsConfig_t pstWincfgDat)
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

void mapp_MHD1616SSWBDWindowCfg(uint8_t out_ch,uint8_t in_ch)
{
    u8 l_u8Temp[50];
    stWindowsConfig_t stWinConfig;
    uint8_t layer_map[]={0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03};

    stWinConfig.u8SrcAddr = out_ch;
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
    stWinConfig.u16Src_X    = 0;
    stWinConfig.u16Src_Y    = 0;
    stWinConfig.u16Src_H    = stWinConfig.u16SrcAct_H;
    stWinConfig.u16Src_V    = stWinConfig.u16SrcAct_V;
    stWinConfig.u32Wnd_X    = 0;
    stWinConfig.u32Wnd_Y    = 0;
    stWinConfig.u32Wnd_H    = g_stWinConfigVar[out_ch-1].stDstWin.u16Width;
    stWinConfig.u32Wnd_V    = g_stWinConfigVar[out_ch-1].stDstWin.u16Height;
    stWinConfig.u32Srf_H    = (((uint64_t)stWinConfig.u16Src_H<<16))/(uint64_t)stWinConfig.u32Wnd_H;
    stWinConfig.u32Srf_V    = (((uint64_t)stWinConfig.u16Src_V<<16))/(uint64_t)stWinConfig.u32Wnd_V;
    stWinConfig.u8SrcInput  = in_ch;

//    dbg_printf("data size  == %d \r\n",sizeof(stWindowsConfig_t));

    mapp_MHD1616SSWBDWinDataCopy(l_u8Temp,&stWinConfig);
    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_TxWinConfig,l_u8Temp,43);

    return;
}

void mapp_MHD1616SSWBDFpgaWinCmdUpdate(u32 u32DelayMs)
{
    u8 l_u8cmd = 1;
    delay_ms_cpu(u32DelayMs);
    //send_cmd_to_fpga(0xFE,eFpgaRegOutUpdate,&l_u8cmd,1);
    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_UpdateWincfg,&l_u8cmd,1);
}

void mapp_MHD1616SSWBD_TimingConfig(uint8_t ch,uint8_t idx)
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
    l_u8Temp[0]  = ch;
    l_u8Temp[1]  = res_table[idx].u16HActive&0xFF;
    l_u8Temp[2]  = (res_table[idx].u16HActive>>8)&0xFF;
    l_u8Temp[3]  = res_table[idx].u16HTotal&0xFF;
    l_u8Temp[4]  = (res_table[idx].u16HTotal>>8)&0xFF;
    l_u8Temp[5]  = res_table[idx].u16HBP&0xFF;
    l_u8Temp[6]  = (res_table[idx].u16HBP>>8)&0xFF;
    l_u8Temp[7]  = res_table[idx].u16HFP&0xFF;
    l_u8Temp[8]  = (res_table[idx].u16HFP>>8)&0xFF;
    l_u8Temp[9]  = res_table[idx].u16HSync&0xFF;
    l_u8Temp[10]  = (res_table[idx].u16HSync>>8)&0xFF;
    l_u8Temp[11] = res_table[idx].u8HPol&0xFF;
    l_u8Temp[12] = res_table[idx].u16VActive&0xFF;
    l_u8Temp[13] = (res_table[idx].u16VActive>>8)&0xFF;
    l_u8Temp[14] = res_table[idx].u16VTotal&0xFF;
    l_u8Temp[15] = (res_table[idx].u16VTotal>>8)&0xFF;
    l_u8Temp[16] = res_table[idx].u16VBP&0xFF;
    l_u8Temp[17] = (res_table[idx].u16VBP>>8)&0xFF;
    l_u8Temp[18] = res_table[idx].u16VFP&0xFF;
    l_u8Temp[19] = (res_table[idx].u16VFP>>8)&0xFF;
    l_u8Temp[20] = res_table[idx].u16VSync&0xFF;
    l_u8Temp[21] = (res_table[idx].u16VSync>>8)&0xFF;
    l_u8Temp[22] = res_table[idx].u8VPol&0xFF;
    l_u8Temp[23] = res_table[idx].u8VClk&0xFF;
    l_u8Temp[24] = res_table[idx].u8Vic&0xFF;
    l_u8Temp[25] = res_table[idx].u16PClk&0xFF;
    l_u8Temp[26] = (res_table[idx].u16PClk>>8)&0xFF;
    l_u8Temp[27] = res_table[idx].u8Interlace&0xFF;

    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_TxResolution,l_u8Temp,28);
}

void mapp_MHD1616SSWBDResolutionConfig(u8 u8Out,u8 u8ResID)
{
    mapp_MHD1616SSWBD_TimingConfig(u8Out+1,u8ResID);
    SwitchResolutionToRefreshWindowns(u8Out,u8ResID);
}

void mapp_MHD1616SSWBDSendTimingToTx(void)
{
    u8 l_u8Temp[100];
    u8 i;
    memset(l_u8Temp,0,100);

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        l_u8Temp[0+i*4] = g_stRxResTable[i].u16Width&0xFF;
        l_u8Temp[1+i*4] = (g_stRxResTable[i].u16Width>>8)&0xFF;
        l_u8Temp[2+i*4] = g_stRxResTable[i].u16Height&0xFF;
        l_u8Temp[3+i*4] = (g_stRxResTable[i].u16Height>>8)&0xFF;
    }
    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_SendRxTiming,l_u8Temp,64);
}

void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID)
{
    u8 i;

    switch(u8Out)
    {
        case 0:
        case 1:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i].stDstWin.u16Height= res_table[u8ResID].u16VActive;
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
                }
            }
            break;
        case 4:
        case 5:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+4].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+4].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                }
            }
            break;
        case 6:
        case 7:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+6].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+6].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                }
            }
            break;
        case 8:
        case 9:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+8].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+8].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                }
            }
            break;
        case 10:
        case 11:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+10].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+10].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                }
            }
            break;
        case 12:
        case 13:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+12].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+12].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                }
            }
            break;
        case 14:
        case 15:
            {
                for(i=0;i<2;i++)
                {
                    g_stWinConfigVar[i+14].stDstWin.u16Width = res_table[u8ResID].u16HActive;
                    g_stWinConfigVar[i+14].stDstWin.u16Height = res_table[u8ResID].u16VActive;
                }
            }
            break;
        default:
            break;
    }
}

#define NUM_LOOP    15
void mapp_MHD1616SSWBDStartupToRecoveryState(void)
{
    static bool sl_bStartupFlag = TRUE;
    static eStartupLoopType_t sl_eStartupLoop = eSt_None;
    static u8 sl_u8LoopCnt = 0;

    if(!mapp_MHD1616SSWBD_PlatformTimerExpired(emUserTimer_StartupTime))
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
        case eSt_GetTxCardType:
            {
                dbg_printf("Get Tx Card Type\r\n");
                mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_CardType,NULL,0);
                delay_ms(50);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,200);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
                sl_u8LoopCnt++;
                if(g_u8CardType[emCardType_Tx]||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_GetTxVersion:
            {
                dbg_printf("Get Tx Version\r\n");
                mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_Version,NULL,0);
                delay_ms(50);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,200);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
                sl_u8LoopCnt++;
                if(g_u8Fwversion[emCardType_Tx*2+1]||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_SetTxResolution:
            {
#if 0
                pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
                pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
                pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
                pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
                u8 i;

                dbg_printf("Set Tx Resolution\r\n");
                for(i=0;i<Def_outport_num;i++)
                {
                    mapp_MHD1616SSWBDResolutionConfig(i,pstBasicVar->u8TxResolutionID[i]);
                    mapp_MHD1616SSWBDWindowCfg(i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
                }
                mapp_MHD1616SSWBDFpgaWinCmdUpdate(10);
                delay_ms(50);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,2000);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,5000);
#endif
                sl_eStartupLoop++;
            }
            break;
        case eSt_SetTxConfigEnd:
            {
                bool l_bInitOk=TRUE;
#if 0
                u8 i;

                for(i=0;i<Def_outport_num;i++)
                {
                    if((!g_stLinkSta.u8WinCfgOk[i])/*||(!g_stLinkSta.u8ResCfgOk[i])*/)
                    {
                        l_bInitOk = FALSE;
                    }
                }
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,500);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
#endif
                sl_u8LoopCnt++;
                dbg_printf("Check Tx config end: %d\r\n",sl_u8LoopCnt);
                if(l_bInitOk)
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;


        case eSt_GetRx1CardType:
            {
                dbg_printf("Get Rx1 Card Type\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_CardType,NULL,0);
                delay_ms(50);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,200);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
                sl_u8LoopCnt++;
                if(g_u8CardType[emCardType_Rx1]||(sl_u8LoopCnt>NUM_LOOP))
                {
                    if(g_u8CardType[emCardType_Rx1]==0)
                        sl_eStartupLoop = eSt_GetRx2CardType;
                    else
                        sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_GetRx1Version:
            {
                dbg_printf("Get Rx Version\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_Version,NULL,0);
                delay_ms(100);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,200);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
                sl_u8LoopCnt++;
                if((g_u8Fwversion[emCardType_Rx1*2+1])||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_GetRx2CardType:
            {
                dbg_printf("Get Rx2 Card Type\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_CardType,NULL,0);
                delay_ms(50);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,200);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
                sl_u8LoopCnt++;
                if(g_u8CardType[emCardType_Rx2]||(sl_u8LoopCnt>NUM_LOOP))
                {
                    if(g_u8CardType[emCardType_Rx2]==0)
                        sl_eStartupLoop = eSt_None;
                    else
                        sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_GetRx2Version:
            {
                dbg_printf("Get Rx2 Version\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_Version,NULL,0);
                delay_ms(100);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_StartupTime,200);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
                sl_u8LoopCnt++;
                if((g_u8Fwversion[emCardType_Rx2*2+1])||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;

        case eSt_None:
        default:
            break;
    }
}

void mapp_MHD1616SSWBDCheckCardInfo(void)
{
    static eCheckCardInfoType_t sl_eCheckIndex = eCheck_TxLinkSta;

    if(!mapp_MHD1616SSWBD_PlatformTimerExpired(emUserTimer_CardInfo))
        return;

    switch(sl_eCheckIndex)
    {
        case eCheck_TxLinkSta:
            {
//                dbg_printf("Get Tx LinkSta\r\n");
                mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_LinkSta,NULL,0);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,300);
                sl_eCheckIndex++;
            }
            break;

        case eCheck_Rx1LinkSta:
            {
//                dbg_printf("Get Rx1 LinkSta\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_LinkSta,NULL,0);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,300);
                sl_eCheckIndex++;
            }
            break;
        case eCheck_Rx1InputTinming:
            {
//                dbg_printf("Get Rx1 input Timing\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_GetRxResolution,NULL,0);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,500);
                sl_eCheckIndex++;
            }
            break;
        case eCheck_Rx2LinkSta:
            {
//                dbg_printf("Get Rx2 LinkSta\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_LinkSta,NULL,0);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,300);
                sl_eCheckIndex++;
            }
            break;
        case eCheck_Rx2InputTinming:
            {
//                dbg_printf("Get Rx2 input Timing\r\n");
                mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_GetRxResolution,NULL,0);
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,500);
                sl_eCheckIndex++;
            }
            break;
        case eCheck_GetTxEdid:
            {
                static u8 sl_u8TxPort = 0;
                u8 l_u8SendBuf[1];

                if(g_stTxEdidVar[sl_u8TxPort].u8RecvOk==FALSE)
                {
                    l_u8SendBuf[0] = sl_u8TxPort+1;
                    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_GetTxPortEdid,l_u8SendBuf,1);
                    //delay_ms(100);
                    g_stTxEdidVar[sl_u8TxPort].u8RecvOk=TRUE;
                    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,500);
                }
                sl_u8TxPort++;
                if(sl_u8TxPort>=Def_outport_num)
                {
                    sl_eCheckIndex++;
                    sl_u8TxPort = 0;
                }
            }
            break;
        case eCheck_End:
        default:
            sl_eCheckIndex = eCheck_TxLinkSta;
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,3000);
            break;
    }
}

void mapp_SourceChangeToSetWinProcess(void)
{
    u8 l_u8Src,l_u8OutIdex;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    bool l_bUpdateRes = FALSE;

    if(!mapp_MHD1616SSWBD_PlatformTimerExpired(emUserTimer_TimingChange))
        return;
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,500);

    for(l_u8Src = 0;l_u8Src<_HDMI_INPUT_PORT_USED_NUMBER;l_u8Src++)
    {
        if(g_stRxResTable[l_u8Src].bChangeFlag)
        {
            for(l_u8OutIdex = 0; l_u8OutIdex<Def_outport_num;l_u8OutIdex++)
            {
                if(pstPrjSaveVar->stHdmiSel.u8SwChannel[l_u8OutIdex] == (l_u8Src))
                {
                    //mapp_MHD1616SSWBDWindowCfg(l_u8OutIdex+1,(l_u8Src+1));
                    mapp_MHD1616SSWBD_SetAudioSource(l_u8OutIdex+1,(l_u8Src+1));

                    if((l_u8OutIdex+1)==14)
                    {
                        mapp_MHD1616SSWBD_SetAudioSource(Def_outport_num+1,g_stSwitchTable.u8OutCh[l_u8OutIdex]+1);
                    }
                    else if((l_u8OutIdex+1)==15)
                    {
                        mapp_MHD1616SSWBD_SetAudioSource(Def_outport_num+2,g_stSwitchTable.u8OutCh[l_u8OutIdex]+1);
                    }
                    delay_ms(50);
                }
            }

            l_bUpdateRes= TRUE;
            //mapp_MHD1616SSWBDFpgaWinCmdUpdate(10);
            g_stRxResTable[l_u8Src].bChangeFlag = FALSE;
        }
    }

    if(l_bUpdateRes)
    {
        mapp_MHD1616SSWBDSendTimingToTx();
    }
}

#define DM_FW_BYTES_MIX         (15*1024)

#define DM_FW_BYTES_MAX         (128*1024)  //100KB
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

stUpdateVar_t g_stMcuUpdateVar;

uint8_t UpdateFunction(uint8_t u8Id,uint8_t u8Step ,uint8_t *pu8Data ,uint16_t u16Len)
{
    uint8_t l_u8RetVal = 1;

    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,10000);
    g_stMcuUpdateVar.u8UpdateMode = u8Id;
    switch(u8Step)
    {
        case 0:
            {
                mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
                g_stMcuUpdateVar.u8UpdateFlag=1;
                if(u8Id==eUpdID_SwitchBDMCU)
                {
                    l_u8RetVal = mcu_file_updata(0,0,0);
                }
                else if(u8Id==eUpdID_RX1BDMCU)
                {
                    u8 l_u8SendBuf[1];

                    l_u8SendBuf[0] = 1;
                    mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_UpdateMcu,l_u8SendBuf,1);
                }
                else if(u8Id==eUpdID_RX2BDMCU)
                {
                    u8 l_u8SendBuf[1];

                    l_u8SendBuf[0] = 1;
                    mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_UpdateMcu,l_u8SendBuf,1);
                }
                else if(u8Id==eUpdID_TXBDMCU)
                {
                    u8 l_u8SendBuf[1];

                    l_u8SendBuf[0] = 1;
                    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_UpdateMcu,l_u8SendBuf,1);
                }
                g_stMcuUpdateVar.u16PackIndex = 0;
            }
            break;
        case 1:
            {
                g_stMcuUpdateVar.u16PackIndex++;
                if(u8Id==eUpdID_SwitchBDMCU)
                {
                    l_u8RetVal=mcu_file_updata(1,pu8Data,u16Len);
                }
                else
                {
                    mapp_SendDataToInOutCard(pu8Data,u16Len);
                }
            }
            break;
        case 2:
            {
                if(u8Id==eUpdID_SwitchBDMCU)
                {
                    l_u8RetVal=mcu_file_updata(2,pu8Data,u16Len);
                }
                else
                {
                    //mapp_SendDataToInOutCard(pu8Data,u16Len);
                }
                g_stMcuUpdateVar.u8UpdateFlag=0;
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
//            dbg_printf("iap updata mode %d\r\n",g_stMcuUpdateVar.u8UpdateMode);
            dbg_printf("iap start Erase Sector\r\n");
            fmc_unlock();
            f_sta = fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_DATA_ADDR));
            if(f_sta != 0)
            {
                fmc_lock();
                return 0;
            }
            fmc_lock();
            dbg_printf("Erase Sector finish\r\n");
            g_stMcuUpdateVar.u32TotalBytes = 0;
//            g_stMcuUpdateVar.u8UpdateFlag=1;
            LeftBytes=0;
            memset(LeftBytesTab,0,4);
//            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_FirmwareUpdate,10000);
            g_stMcuUpdateVar.u32PageIndex = 0;
            break;
        case 1:
            if(g_stMcuUpdateVar.u32PageIndex<FW_VECT_OFFSET)
            {
                g_stMcuUpdateVar.u32PageIndex +=len;
                return 0;
            }
            else
                g_stMcuUpdateVar.u32PageIndex +=len;
#if 0
            if((g_stMcuUpdateVar.u8UpdateMode==2)&&(frist_packet))
            {
                frist_packet=0;
                if(len>64)
                {
                    for(i=0;i<64;i++)
                    {
                        dbg_printf("%c",*(data+i));
                    }

                    data = (data+64);
                    len=len-64;
                }
                else
                    return 0;

            }
#endif

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
                fmc_word_program(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes,*(u32 *)LeftBytesTab);
                fmc_lock();
                //printf("d=%08x\r\n",*(u32 *)LeftBytesTab);
                LeftBytes =0;

                g_stMcuUpdateVar.u32TotalBytes=g_stMcuUpdateVar.u32TotalBytes+4;

                if (len >j)
                {
                    /* update data pointer */
                    data = (data+j);
                    len = len -j;
                }
            }


            buff = (uint32_t*)(data);

            if((g_stMcuUpdateVar.u32TotalBytes  + len) > DM_FW_BYTES_MAX)
            {
               return 0;
            }
            fmc_unlock();
            for(i = 0; i < len/4; i++)
            {
                f_sta = fmc_word_program(DM_FW_DATA_ADDR+g_stMcuUpdateVar.u32TotalBytes+i*4,  *(buff+i));
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

                    g_stMcuUpdateVar.u32TotalBytes +=len-i;
            }
            else
                g_stMcuUpdateVar.u32TotalBytes +=len;

            if(len<1024)
                memcpy(g_stMcuUpdateVar.u8LastData,data,len);
            fmc_lock();
            //printf(".");
            break;
        case 2:
            fmc_unlock();

            fmc_sector_erase(GD32F4FLASH_GetFlashSector(DM_FW_FLAG_ADDR));

            g_stMcuUpdateVar.u32TotalBytes +=len;
            if(g_stMcuUpdateVar.u32TotalBytes>DM_FW_BYTES_MIX)
            {
                memcpy((uint8_t*)&packet,g_stMcuUpdateVar.u8LastData, sizeof(image_header_t));
                dbg_printf("iap end\r\n");
                dbg_printf("total_bytes [%d]\r\n",g_stMcuUpdateVar.u32TotalBytes );
                dbg_printf("ih_magic [0x%x]\r\n",packet.ih_magic );
                dbg_printf("ih_arch [%d]\r\n",packet.ih_arch );
                dbg_printf("ih_name [%s]\r\n",packet.ih_name );
                if((packet.ih_magic==0x4e545047)&&(packet.ih_arch==2)&&!strncmp((char*)packet.ih_name,_STRING_FACTORY_MODEL,strlen(_STRING_FACTORY_MODEL)))
                {
                    dbg_printf("data check ok\r\n");

                    f_sta =fmc_word_program(DM_FW_FLAG_ADDR, DM_FW_FLAG_VALID);
                    dbg_printf("f_sta=%d\r\n",f_sta);
                    fmc_word_program(DM_FW_SIZE_ADDR, g_stMcuUpdateVar.u32TotalBytes );
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
            g_stMcuUpdateVar.u8UpdateFlag=0;
            SYS_SoftReset();
            break;
    }
    return 1;
}

u8 mapp_FirmwareUpdateProcess(pstUartCom_t pComVar)
{
    if(g_stMcuUpdateVar.u8UpdateFlag==1)
    {
        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_FirmwareUpdate,2000);
//        mcu_file_updata(1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);
        UpdateFunction(g_stMcuUpdateVar.u8UpdateMode,1,(uint8_t*)pComVar->pRecBuffer,pComVar->u16RecCnt);

        pComVar->SendPrintf("pack %d\r\n",g_stMcuUpdateVar.u16PackIndex);
        return 1;
    }
    else
        return 0;
}

void mapp_FirmwareUpdateTimeOutFunction(void)
{
    if(g_stMcuUpdateVar.u8UpdateFlag==1&&mapp_MHD1616SSWBD_PlatformTimerExpired(emUserTimer_FirmwareUpdate))
    {
//        mcu_file_updata(2,NULL,0);
        UpdateFunction(g_stMcuUpdateVar.u8UpdateMode,2,NULL,0);
    }
}

