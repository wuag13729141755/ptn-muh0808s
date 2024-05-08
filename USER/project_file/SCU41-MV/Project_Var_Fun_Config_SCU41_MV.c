#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"

#define Dbg_UserFun(x)          //Uart2_printf x

const u8 g_u8Head[3] = {0xFF,0xFF,0xA5};
const stResolutionTable_t TimingTable[emRes_max]=
{
  // HFP HPol HSync HBP  HDst  HTot  VFP VPol VSync VBP VDst VTot,VIC,       PCLK,       VCLK
    { 24,Hneg, 136, 160, 1024, 1344,   3,Vneg,  6, 29,  768,  806,  0, (1344*806)*60/10000,60},  //1024x768@60Hz
    {110,Hpos,  40, 220, 1280, 1650,   5,Vpos,  5, 20,  720,  750,  4,                7425,60},  //1280x720@60Hz
    { 64,Hpos, 112, 256, 1360, 1792,   3,Vpos,  6, 18,  768,  795,  0, (1792*795)*60/10000,60},  //1360x768@60Hz
    { 64,Hpos, 192, 304, 1600, 2160,   1,Vpos,  3, 46, 1200, 1250,  0,(2160*1250)*60/10000,60},  //1600x1200@60Hz
    { 88,Hpos,  44, 148, 1920, 2200,   4,Vpos,  5, 36, 1080, 1125, 16,               14850,60},  //1920x1080@60Hz
    { 48,Hpos,  32,  80, 1920, 2080,   3,Vneg,  6, 26, 1200, 1235,  0,(2080*1235)*60/10000,60},  //1920x1200@60Hz
    {174,Hpos,  88, 298, 3840, 4400,   8,Vpos, 10, 72, 2160, 2250, 95,               29700,30},  //3840x2160@30Hz

#if _ENABLE_RES_1600x900_60
    { 24,Hneg,  80,  96, 1600, 1800,   1,Vneg,  3, 96,  900, 1000,  0,(1800*1000)*60/10000,60},  //1600x900@60Hz
#endif
#if _ENABLE_RES_1600x900_60
    {104,Hneg, 176, 280, 1680, 2240,   3,Vpos,  6, 30, 1050, 1089,  0,(2240*1089)*60/10000,60},  //1680x1050@60Hz
#endif
#if _ENABLE_RES_1400x1050_60
    { 88,Hneg, 144, 232, 1400, 1864,   3,Vpos,  4, 32, 1050, 1089,  0,(1864*1089)*60/10000,60},  //1400x1050@60Hz
#endif
#if _ENABLE_RES_1280x800_60
    { 72,Hneg, 128, 200, 1280, 1680,   3,Vneg,  6, 22,  800,  831,  0, (1680*831)*60/10000,60},  //1280x800@60Hz
#endif
#if _ENABLE_RES_1280x720_50
    {440,Hpos,  40, 220, 1280, 1980,   5,Vpos,  5, 20,  720,  750, 19,                7425,50},  //1280x720@50Hz
#endif
#if _ENABLE_RES_1920x1080_50
    {528,Hpos,  44, 148, 1920, 2640,   4,Vpos,  5, 36, 1080, 1125, 31,               14850,50},  //1920x1080@50Hz
#endif
    #if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
    { 88,Hpos,  44, 148, 1920, 2200,   4,Vpos,  5, 36, 1080, 1125, 16,               14850,60},  //Best for display
    #endif
};

stProjectSpecVar_t  stSCU41_MVSpecVar;
uint16_t            g_u32SCU41_MVEdidUpdateCnt = 0;
bool                g_bLedMntKeyPrssAtPanLked = FALSE;
bool                g_bTvPowerOnOffByRemote = TRUE;

static bool         sg_bChangePowerStateFlag = FALSE;
#if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
static bool         sg_bEnableSwitchPortFlag = FALSE;
static u8           sg_u8CurSwitchPort = 0;
static bool         sg_bEnableSelectPort = FALSE;
static bool         sg_bEnableCheckPort = FALSE;
static u8           sg_u8CheckWinPort = 0;
#if _ENABLE_SWITCH_LED_TIMER
static bool         sg_bEnableSwitchLed = FALSE;
#endif
#endif

static u8           sg_u8InputPortSel = 0;
static u8           sg_u8WinPortSel = 0;
static u8           sg_u8ResizeMode = 0;
static bool         sg_bAudioSrcSelFlag = FALSE;

static bool         sg_bWincfgUpdateFlag = FALSE;

static bool         sg_bDoBypassEdidFlag[_HDMI_INPUT_PORT_USED_NUMBER] = {FALSE};

stWinConfigVar_t    g_stWinConfigVar[4];
stResolutionTable_t g_stTiming;
stUserWindowsConfig_t   g_stUserWinsConfig;
static u8           sg_u8ConfigMode = 0;
stUserWinPixelCfg_t g_stUserWinPixelCfg;
stWindowType_t      g_stUserWinClip;
static bool         g_bWinClipModeSta = FALSE;
u8 g_u8WinLayer[emWinTypeMax] = {0};
u8 g_u8WinLevel = 0;

extern stProjectGlobalAllVar_t g_stPrjGloVar;
extern key_val_stored_t s_tValStd;
void KeyPressCallBack(uint8_t u8KeyIndex);
void SCU41_MVEdidUpdCntLoop(void);

// EF_02_1920x1080-60_8bit_Stereo_for41mv
const uint8_t edid_internal_1[256]=
{
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
    0x55,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
    0x32,0x1C,0x01,0x03,0x80,0x50,0x2D,0x78,
    0x0A,0xEE,0x91,0xA3,0x54,0x4C,0x99,0x26,
    0x0F,0x50,0x54,0x21,0x08,0x00,0x81,0xC0,
    0xD1,0xC0,0xD1,0x00,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x01,0x02,0x3A,
    0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,
    0x45,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20,
    0x6E,0x28,0x55,0x00,0x20,0xC2,0x31,0x00,
    0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x18,
    0x4B,0x1A,0x51,0x10,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,
    0x00,0x48,0x44,0x20,0x56,0x69,0x64,0x65,
    0x6F,0x0A,0x20,0x20,0x20,0x20,0x01,0xDE,
    0x02,0x03,0x23,0x72,0x4B,0x90,0x9F,0x22,
    0x21,0x20,0x05,0x14,0x04,0x13,0x12,0x03,
    0x23,0x09,0x04,0x07,0x83,0x01,0x00,0x00,
    0x67,0x03,0x0C,0x00,0x10,0x00,0x00,0x20,
    0xE2,0x00,0x0F,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF2,
};

// EF_11_3840x2160_30HZ_DeepColor_Stereo_for_41MV
const uint8_t edid_internal_2[256]=
{
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
    0x55,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
    0x32,0x1C,0x01,0x03,0x80,0x73,0x41,0x78,
    0x0A,0xCF,0x74,0xA3,0x57,0x4C,0xB0,0x23,
    0x09,0x48,0x4C,0x01,0x08,0x00,0xD1,0xC0,
    0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x01,0x04,0x74,
    0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,
    0x8A,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,
    0x58,0x2C,0x45,0x00,0xC4,0x8E,0x21,0x00,
    0x00,0x1E,0x00,0x00,0x00,0xFC,0x00,0x48,
    0x44,0x20,0x56,0x69,0x64,0x65,0x6F,0x0A,
    0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFD,
    0x00,0x3B,0x46,0x1F,0x8C,0x3C,0x00,0x0A,
    0x20,0x20,0x20,0x20,0x20,0x20,0x01,0xC0,
    0x02,0x03,0x27,0xF2,0x4E,0x05,0x84,0x13,
    0x14,0x90,0x1F,0x20,0x22,0x5D,0x5F,0x62,
    0x64,0x5E,0x63,0x23,0x09,0x04,0x07,0x83,
    0x01,0x00,0x00,0x67,0x03,0x0C,0x00,0x10,
    0x00,0xB8,0x44,0xE3,0x05,0xFF,0x01,0x8C,
    0x0A,0xD0,0x8A,0x20,0xE0,0x2D,0x10,0x10,
    0x3E,0x96,0x00,0xC4,0x8E,0x21,0x00,0x00,
    0x18,0x8C,0x0A,0xA0,0x14,0x51,0xF0,0x16,
    0x00,0x26,0x7C,0x43,0x00,0xC4,0x8E,0x21,
    0x00,0x00,0x98,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5E,
};

const stWinTypeFactorType_t g_pu8MultiWinTable[emMvMode_max][emWinTypeMax] =
{
    //   Xsf1,Xsf2,Ysf1,Ysf2,Wsf1,Wsf2,Hsf1,Hsf2
    //emMvMode_1Win_Full=0,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PBP,
    {
        {   0, 100, 25, 100, 50,  100, 50,  100},
        {  50, 100, 25, 100, 50,  100, 50,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_3Win_2U1D,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {  25, 100, 50, 100, 50,  100, 50,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_4Win_SameSize,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_2Win_PIP_LU,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100,  0, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PIP_LD,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100, 75, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PIP_RU,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  75, 100,  0, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_2Win_PIP_RD,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  75, 100, 75, 100, 25,  100, 25,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emMvMode_4Win_PBP_3L1R,
    {
        {   6,  16,  7,  32,  9,   16,  9,   16},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   1,  16,  6,  16,  4,   16,  4,   16},
        {   1,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PBP_1L3R,
    {
        {   1,  16,  7,  32,  9,   16,  9,   16},
        {  11,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  6,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PBP_3U1D,
    {
        {   7,  32,  6,  16,  9,   16,  9,   16},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   6,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  1,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PBP_1U3D,
    {
        {   7,  32,  1,  16,  9,   16,  9,   16},
        {   1,  16, 11,  16,  4,   16,  4,   16},
        {   6,  16, 11,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3L,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   1,  16,  6,  16,  4,   16,  4,   16},
        {   1,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3R,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {  11,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  6,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3U,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {   1,  16,  1,  16,  4,   16,  4,   16},
        {   6,  16,  1,  16,  4,   16,  4,   16},
        {  11,  16,  1,  16,  4,   16,  4,   16},
    },
    //emMvMode_4Win_PIP_1F3D,
    {
        {   0, 100,  0, 100,100,  100,100, 100},
        {   1,  16, 11,  16,  4,   16,  4,   16},
        {   6,  16, 11,  16,  4,   16,  4,   16},
        {  11,  16, 11,  16,  4,   16,  4,   16},
    },
    //emMvMode_xWin_ByUser1,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_xWin_ByUser2,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_xWin_ByUser3,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
    //emMvMode_xWin_ByUser4,
    {
        {   0, 100,  0, 100, 50,  100, 50,  100},
        {  50, 100,  0, 100, 50,  100, 50,  100},
        {   0, 100, 50, 100, 50,  100, 50,  100},
        {  50, 100, 50, 100, 50,  100, 50,  100},
    },
};

const stWinTypeFactorType_t g_puResizeModeTable[emResize_MaxNum][emWinTypeMax]=
{
    //emResize_None = 0,
    {
        {   0, 100,  0,  16,  0,    8,  0,    8},
        {   0,   8,  0,  16,  0,    8,  0,    8},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PBP_1,
    {
        {   0, 100,  3,  16,  5,    8,  5,    8},
        {   5,   8,  5,  16,  3,    8,  3,    8},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PBP_2,
    {
        {   0, 100,  2,  16,  6,    8,  6,    8},
        {   6,   8,  6,  16,  2,    8,  2,    8},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PBP_3,
    {
        {   0, 100,  6,  16,  2,    8,  2,    8},
        {   2,   8,  2,  16,  6,    8,  6,    8},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_LU_1,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100,  0, 100,  5,   16,  5,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_LU_2,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100,  0, 100,  6,   16,  6,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_LD_1,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100, 11,  16,  5,   16,  5,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_LD_2,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {   0, 100, 10,  16,  6,   16,  6,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_RU_1,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  11,  16,  0, 100,  5,   16,  5,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_RU_2,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  10,  16,  0, 100,  6,   16,  6,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_RD_1,
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  11,  16, 11,  16,  5,   16,  5,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_2Win_PIP_RD_2,     // 10
    {
        {   0, 100,  0, 100,100,  100,100,  100},
        {  10,  16, 10,  16,  6,   16,  6,   16},
        {   0, 100,  0, 100,  0,  100,  0,  100},
        {   0, 100,  0, 100,  0,  100,  0,  100},
    },
    //emResize_4Win_PBP_3L1R_1,
    {
        {  13,  48,  5,  32, 11,   16, 11,   16},
        {   2,  48,  7,  64,  3,   16,  3,   16},
        {   2,  48, 26,  64,  3,   16,  3,   16},
        {   2,  48, 45,  64,  3,   16,  3,   16},
    },
    //emResize_4Win_PBP_3L1R_2,
    {
        {  10,  48,  4,  32, 12,   16, 12,   16},
        {   2,  48, 10,  64,  2,   16,  2,   16},
        {   2,  48, 28,  64,  2,   16,  2,   16},
        {   2,  48, 46,  64,  2,   16,  2,   16},
    },
    //emResize_4Win_PBP_1L3R_1,
    {
        {   2,  48,  5,  32, 11,   16, 11,   16},
        {  37,  48,  7,  64,  3,   16,  3,   16},
        {  37,  48, 26,  64,  3,   16,  3,   16},
        {  37,  48, 45,  64,  3,   16,  3,   16},
    },
    //emResize_4Win_PBP_1L3R_2,
    {
        {   2,  48,  4,  32, 12,   16, 12,   16},
        {  40,  48, 10,  64,  2,   16,  2,   16},
        {  40,  48, 28,  64,  2,   16,  2,   16},
        {  40,  48, 46,  64,  2,   16,  2,   16},
    },
    //emResize_4Win_PBP_3U1D_1,
    {
        {   5,  32, 13,  48, 11,   16, 11,   16},
        {   7,  64,  2,  48,  3,   16,  3,   16},
        {  26,  64,  2,  48,  3,   16,  3,   16},
        {  45,  64,  2,  48,  3,   16,  3,   16},
    },
    //emResize_4Win_PBP_3U1D_2,
    {
        {   4,  32, 10,  48, 12,   16, 12,   16},
        {  10,  64,  2,  48,  2,   16,  2,   16},
        {  28,  64,  2,  48,  2,   16,  2,   16},
        {  46,  64,  2,  48,  2,   16,  2,   16},
    },
    //emResize_4Win_PBP_1U3D_1,
    {
        {   5,  32,  2,  48, 11,   16, 11,   16},
        {   7,  64, 37,  48,  3,   16,  3,   16},
        {  26,  64, 37,  48,  3,   16,  3,   16},
        {  45,  64, 37,  48,  3,   16,  3,   16},
    },
    //emResize_4Win_PBP_1U3D_2,
    {
        {   4,  32,  2,  48, 12,   16, 12,   16},
        {  10,  64, 40,  48,  2,   16,  2,   16},
        {  28,  64, 40,  48,  2,   16,  2,   16},
        {  46,  64, 40,  48,  2,   16,  2,   16},
    },
};

u8 g_pu8WinLayerLevel[4][emWinTypeMax] =
{
    {1,2,3,4},
    {4,3,2,1},
    {1,2,3,4},
    {1,2,3,4},
};

void SCU41_MV_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);
}

void SCU41_MV_SpecificVarDefault(void)
{
    SCU41_MV_SpecBasicVarDefault();
    SCU41_MV_SpecCustomEdidVarDefault();
}

void SCU41_MV_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar  = &stSCU41_MVSpecVar;

    pstSpecVar->stBasSpecVar.bPowerOnSta    = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock        = FALSE;
    pstSpecVar->stBasSpecVar.bIsSpdifOutOn  = TRUE;
    pstSpecVar->stBasSpecVar.bIsIISOutOn    = TRUE;
    pstSpecVar->stBasSpecVar.u8baudrateId   = baud_rate_9600;

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
    pstSpecVar->stBasSpecVar.bActionToSendCecFlag       = TRUE;
    pstSpecVar->stBasSpecVar.bActionToSendRs232CmdFlag  = TRUE;
    pstSpecVar->stBasSpecVar.u16DisplayTime = 60*10;
    #if _ENABLE_AUTO_POWER_FUNCTION
    pstSpecVar->stBasSpecVar.bActionToAutoPowerOnOffFlag= FALSE;
    #endif
#endif
    pstSpecVar->stBasSpecVar.u8MvMode       = emMvMode_4Win_SameSize;
    pstSpecVar->stBasSpecVar.u8WinSel[emWinType_A]  = emSrcInput_Hdmi1;
    pstSpecVar->stBasSpecVar.u8WinSel[emWinType_B]  = emSrcInput_Hdmi2;
    pstSpecVar->stBasSpecVar.u8WinSel[emWinType_C]  = emSrcInput_Hdmi3;
    pstSpecVar->stBasSpecVar.u8WinSel[emWinType_D]  = emSrcInput_Hdmi4;
    pstSpecVar->stBasSpecVar.u8ResizeMode   = emResize_None;
    pstSpecVar->stBasSpecVar.u8SwapMode     = 0;
    pstSpecVar->stBasSpecVar.u8WindowNum    = emWinNum_4;
    pstSpecVar->stBasSpecVar.u8ResolutionID = emRes_3840x2160_30;
    pstSpecVar->stBasSpecVar.u8AudioSel     = 0;
    pstSpecVar->stBasSpecVar.bAudioMute     = FALSE;
    pstSpecVar->stBasSpecVar.bAudioMix      = TRUE;
    pstSpecVar->stBasSpecVar.u8HdcpMode     = tx_hdcp_force_1_4;
    #if _ENABLE_SWITCH_PORT_AND_AUDIO
    pstSpecVar->stBasSpecVar.bFullModeToSwitchAudio = TRUE;
    #endif
    {
        u8 i;
        for(i=0;i<4;i++)
        {
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8UserSrc[emWinType_A] = emSrcInput_Hdmi1;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartX[emWinType_A]  = 0;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartY[emWinType_A]  = 0;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndX[emWinType_A]    = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndY[emWinType_A]    = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EnWinSta[emWinType_A]    = 0;

            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8UserSrc[emWinType_B] = emSrcInput_Hdmi2;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartX[emWinType_B]  = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartY[emWinType_B]  = 0;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndX[emWinType_B]    = 100;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndY[emWinType_B]    = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EnWinSta[emWinType_B]    = 0;

            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8UserSrc[emWinType_C] = emSrcInput_Hdmi3;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartX[emWinType_C]  = 0;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartY[emWinType_C]  = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndX[emWinType_C]    = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndY[emWinType_C]    = 100;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EnWinSta[emWinType_C]    = 0;

            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8UserSrc[emWinType_D] = emSrcInput_Hdmi4;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartX[emWinType_D]  = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8StartY[emWinType_D]  = 50;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndX[emWinType_D]    = 100;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EndY[emWinType_D]    = 100;
            pstSpecVar->stBasSpecVar.stUserWinsConfig[i].u8EnWinSta[emWinType_D]    = 0;
        }
    }
}

void SCU41_MV_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre2.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre3.pu8EdidContext, System_Default_Edid_table, 256);
    memcpy(pstSpecVar->stCustomEdidPre4.pu8EdidContext, System_Default_Edid_table, 256);
    //memcpy(pstSpecVar->stCustomEdidPre5.pu8EdidContext, System_Default_Edid_table, 256);
}

bool SCU41_MV_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        SCU41_MV_SpecBasicVarDefault();
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
    //if(pstSpecVar->stCustomEdidPre5.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre5,((uint8_t *)&pstSpecVar->stCustomEdidPre5.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre5)))
    //{
    //    memcpy(pstSpecVar->stCustomEdidPre5.pu8EdidContext, System_Default_Edid_table, 256);
    //    pstSpecVar->stCustomEdidPre5.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre5,((uint8_t *)&pstSpecVar->stCustomEdidPre5.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre5));
    //    isSpecVarValid = FALSE;
    //}

    return isSpecVarValid;
}

//  noused
void SCU41_MV_AutoSw_HookFun(uint8_t u8Index)
{
    pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t       pstBasicVar = &pstSpecVar->stBasSpecVar;
    bool                    bIsLockSave = pstBasicVar->bIsLock;

    pstBasicVar->bIsLock = FALSE;
    //KeyPressCallBack(emKeyFunc_Source);
    pstBasicVar->bIsLock = bIsLockSave;
    UNUSED(u8Index);
}

// the status when power on device
void SCU41_MV_PrjStatePowerOnInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;

	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    #if (!_EN_UART_DEBUG_PRINTF)
    pstUartCom_t pUartCommVar = &g_stUartCommVar;
    #endif

#if 1
    mapp_SetFpgaProgramPin(0);
    delay_ms(100);
    mapp_SetFpgaProgramPin(1);
    mapp_SelectFpgaFlashUpdateMode(FALSE);
#endif
    // default edid table
    System_Default_Edid_table = (iTE_u8 *)edid_internal_1;
    memcpy(g_pu8It663xxEdidBuffer, System_Default_Edid_table,256);

    memset(sg_bDoBypassEdidFlag,FALSE,_HDMI_INPUT_PORT_USED_NUMBER);
    //memset((u8*)&g_stWinConfigVar,0,sizeof(stWindowsConfig_t));
    memset((u8*)&g_stTiming,0,sizeof(stResolutionTable_t));
    memset((u8*)&g_stUserWinsConfig,0,sizeof(stUserWindowsConfig_t));
    memset((u8*)&g_stUserWinPixelCfg,0,sizeof(stUserWinPixelCfg_t));
    memset((u8*)&g_stUserWinClip,0,sizeof(stWindowType_t));
    memset((u8*)&g_stWinConfigVar,0,sizeof(stWinConfigVar_t)*4);
    //g_stUserWinClip.u16StartX = 0;
    //g_stUserWinClip.u16StartY = 0;
    //g_stUserWinClip.u16Width  = 960;
    //g_stUserWinClip.u16Height = 540;
    //g_bWinClipModeSta = TRUE;
    memset(g_u8WinLayer,0,emWinTypeMax);
    // auto switch hook
    FuncAutoSwHook = SCU41_MV_AutoSw_HookFun;

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = SCU41_MVEdidUpdCntLoop;
    //pTm1623LedStaInitFunc = mapp_SetStartupLedStateInit;

    #if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
    #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
    pmapp_IrKeyScanFunc = mapp_IrKeyValueScan;
    #endif
    #endif
    pstBasicSpecVar->bPowerOnSta = TRUE;
    //IT663XXProResumeOrSuspend(TRUE); //Suspend wait for edid dip
    IT663XXProResumeOrSuspend(FALSE);

    #if 0//_ENABLE_USB_SWITCH_BY_INPUT_PORT
    mapp_UsbSwitchFunction(pstHdmiSw->u8SwChannel);
    #endif
    #if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
    sg_u8CurSwitchPort = pstHdmiSw->u8SwChannel;
    #endif
    #if _ENABLE_START_UP_TO_ENABLE_EXTERNAL_AUDIO
    PlatformTimerSet(etUser_StartupAudioTimer,10000);
    #endif
    #if (!_EN_UART_DEBUG_PRINTF)
    pUartCommVar->SetBaudRate(mapp_GetRealBaudrate(pstBasicSpecVar->u8baudrateId), DEF_UART_DATA_CONFIG);
    #endif
    // Reset system
    mapp_SetSysResetPin(0);
    delay_ms(50);
    mapp_SetSysResetPin(1);
    delay_ms(50);
#if _ENABLE_CHIP_ITE6615
    if(pstBasicSpecVar->u8HdcpMode != tx_hdcp_force_bypass)
        g_u8OutputHdcpSta = pstBasicSpecVar->u8HdcpMode;
#endif
    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
    {
        u8 i;
        uint8_t *p_edid = NULL;

        for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
        {
            switch(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[i])
            {

                case EDID_MODE_INTERNAL1://get edid from flash
                    p_edid = g_edid_tabel_user_def + (EDID_MODE_INTERNAL1-1)*256;
                    if(is_edid_valid(p_edid))
                    {
                        FillIT663xxEdidDataForSingleChannel((iTE_u8 *)p_edid, i);
                    }
                    else
                    {
                        FillIT663xxEdidDataForSingleChannel((iTE_u8 *)edid_internal_1, i);
                    }
                    break;
                case EDID_MODE_INTERNAL2:
                    p_edid = g_edid_tabel_user_def + (EDID_MODE_INTERNAL2-1)*256;
                    if(is_edid_valid(p_edid))
                    {
                        FillIT663xxEdidDataForSingleChannel((iTE_u8 *)p_edid, i);
                    }
                    else
                    {
                        FillIT663xxEdidDataForSingleChannel((iTE_u8 *)edid_internal_2, i);
                    }
                    break;
                case EDID_MODE_INTERNAL3:
                    sg_bDoBypassEdidFlag[i] = TRUE;
                    break;
                case EDID_MODE_USER1:
                    FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre1.pu8EdidContext, i);
                    break;
                case EDID_MODE_USER2:
                    FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre2.pu8EdidContext, i);
                    break;
                case EDID_MODE_USER3:
                    FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre3.pu8EdidContext, i);
                    break;
                case EDID_MODE_USER4:
                    FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre4.pu8EdidContext, i);
                    break;
                default:
                    break;
            }
        }
    }
    SetSCU41_MVEdidUpdCnt(5000);
    #endif
    g_bIt663xxAutoSwRunFlag = FALSE;

    SCU41_MV_UpdateWindowNum(pstBasicSpecVar->u8MvMode);
}

void SCU41_MV_TxPlugInOrOutPro_Loop(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
    static bool                 bTxHpdCnntedPre = TRUE;

    if(pstPrjRunVar->bTxHpdCnnted[0])
    {
        g_bIt663xxAutoSwRunFlag = FALSE;
        if(!bTxHpdCnntedPre)
        {
        //    SetAutoSwDetectTime(_TIME_AUTO_SWTICH_RES_CHNGED);//(_TIME_AUTO_SWITCH_DETECT*20);
        }
        bTxHpdCnntedPre = TRUE;
    }
    else
    {
        g_bIt663xxAutoSwRunFlag = FALSE;
        bTxHpdCnntedPre = FALSE;
    }
}


void SCU41_MV_LedWork_Loop(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    static bool                 sl_bStarupFlag= FALSE;
    uint8_t                     i;

    //give a moment status when key pressing at panel locked status
    //if(!g_bLedMntKeyPrssAtPanLked)
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            if(sl_bStarupFlag == FALSE)
            {
                sl_bStarupFlag = TRUE;
                SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());

                if(pstBasicSpecVar->u8WindowNum==emWinNum_1)
                {
                    led_on(led_name_hdmi_input_blue_1+pstBasicSpecVar->u8WinSel[emWinType_A]*2);
                }

                mapp_SCU41MVResolutionSwitch(pstBasicSpecVar->u8ResolutionID);
            }

            #if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
            if((sg_bEnableSwitchPortFlag == TRUE)
                ||(sg_bEnableSelectPort == TRUE)
                #if _ENABLE_SWITCH_LED_TIMER
                ||(mapp_SCU41MVGetSwitchLedFlag())
                ||(mapp_SCU41MVGetCheckPortFlag())
                #endif
                )
            {
                return;
            }
            #endif

            if(mapp_SCU41MVGetAudioSrcSwitchStatus())
                return;

            for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
            {
                if(pstBasicSpecVar->u8WindowNum==emWinNum_1)
                {
                    if((pstPrjRunVar->u16RxSignalSta & (1<<i))&&(pstBasicSpecVar->u8WinSel[emWinType_A]!=i))
                    {
                        led_on(led_name_hdmi_input_yellow_1+i*2);
                    }
                    else
                    {
                        led_on(led_name_hdmi_input_blue_1+pstBasicSpecVar->u8WinSel[emWinType_A]*2);
                        led_off(led_name_hdmi_input_yellow_1+i*2);
                    }
                }
                else
                {
                    if((pstPrjRunVar->u16RxSignalSta & (1<<i)))
                    {
                        led_on(led_name_hdmi_input_yellow_1+i*2);
                    }
                    else
                    {
                        led_off(led_name_hdmi_input_yellow_1+i*2);
                    }
                }
            }

            //auto switch led status
            if(pstHdmiSw->bAutoSwitch)
            {
                led_on(led_name_switch_auto);
            }
            else
            {
                led_off(led_name_switch_auto);
            }

            switch(pstBasicSpecVar->u8MvMode)
            {
                case emMvMode_2Win_PIP_RD:
                    {
                        led_on(led_name_multiview_mode_1);
                        led_off(led_name_multiview_mode_2);
                        led_off(led_name_multiview_mode_3);
                    }
                    break;
                case emMvMode_4Win_PBP_1L3R:
                    {
                        led_off(led_name_multiview_mode_1);
                        led_on(led_name_multiview_mode_2);
                        led_off(led_name_multiview_mode_3);
                    }
                    break;
                case emMvMode_4Win_SameSize:
                    {
                        led_off(led_name_multiview_mode_1);
                        led_off(led_name_multiview_mode_2);
                        led_on(led_name_multiview_mode_3);
                    }
                    break;
                default:
                    {
                        led_off(led_name_multiview_mode_1);
                        led_off(led_name_multiview_mode_2);
                        led_off(led_name_multiview_mode_3);
                    }
                    break;
            }
            SCU41_MV_SetWinLedMode(emWinLightMode_On,emWinType_A);
        }
        else
        {
            SCU41_MV_AllLedOff();
        }

        //led power on state
        if(pstBasicSpecVar->bPowerOnSta)
        {
            led_on(led_name_power_Green);
            led_off(led_name_power_red);
        }
        else
        {
            led_on(led_name_power_red);
            led_off(led_name_power_Green);
        }
    }
}

void SCU41_MV_SpdifIisWork_Loop(void)
{
    pstProjectSpecVar_t pstSpecVar = &stSCU41_MVSpecVar;

    NumIT663XXSpdifOnOff(pstSpecVar->stBasSpecVar.bIsSpdifOutOn);
    NumIT663XXIISOnOff(pstSpecVar->stBasSpecVar.bIsIISOutOn);
}

u8 mapp_SCU41MVGetEdidIndex(u8 u8InPort)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 l_u8EdidMode = 0;

    switch(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[u8InPort])
    {
        case EDID_MODE_INTERNAL1:
            l_u8EdidMode = 1;
            break;
        case EDID_MODE_INTERNAL2:
            l_u8EdidMode = 2;
            break;
        case EDID_MODE_INTERNAL3:
            l_u8EdidMode = 3;
            break;
        case EDID_MODE_USER1:
            l_u8EdidMode = 4;
            break;
        default:
            break;
    }
    return l_u8EdidMode;
}

#if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
void SCU41_MV_EDIDAssignByID(uint8_t u8ID,u8 u8InPort)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    uint8_t                     u8EdidIndex = 0;
    uint8_t *p_edid = NULL;

    u8EdidIndex = u8ID;

    #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
	pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[u8InPort] = u8ID;
    #endif

    switch(u8EdidIndex)
    {
        case EDID_MODE_INTERNAL1://get edid from flash
            p_edid = g_edid_tabel_user_def + (u8EdidIndex-1)*256;
            if(is_edid_valid(p_edid))
            {
                FillIT663xxEdidDataForSingleChannel((iTE_u8 *)p_edid, u8InPort);
            }
            else
            {
                FillIT663xxEdidDataForSingleChannel((iTE_u8 *)edid_internal_1, u8InPort);
            }
            IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,  u8InPort);
            break;
        case EDID_MODE_INTERNAL2:
            p_edid = g_edid_tabel_user_def + (u8EdidIndex-1)*256;
            if(is_edid_valid(p_edid))
            {
                FillIT663xxEdidDataForSingleChannel((iTE_u8 *)p_edid, u8InPort);
            }
            else
                FillIT663xxEdidDataForSingleChannel((iTE_u8 *)edid_internal_2, u8InPort);
            IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,  u8InPort);
            break;
        case EDID_MODE_INTERNAL3:
            //FillIT663xxEdidDataForSingleChannel((iTE_u8 *)g_pu8It6615TxEdidBuffer, u8InPort);
            //IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,      u8InPort);
            sg_bDoBypassEdidFlag[u8InPort] = TRUE;
            break;
        case EDID_MODE_USER1:
            FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre1.pu8EdidContext,  u8InPort);
            IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,  u8InPort);
            break;
        case EDID_MODE_USER2:
            FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre2.pu8EdidContext,  u8InPort);
            IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,  u8InPort);
            break;
        case EDID_MODE_USER3:
            FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre3.pu8EdidContext,  u8InPort);
            IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,  u8InPort);
            break;
        case EDID_MODE_USER4:
            FillIT663xxEdidDataForSingleChannel(pstSpecVar->stCustomEdidPre4.pu8EdidContext,  u8InPort);
            IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer,  u8InPort);
            break;
        default:
            break;
    }
    SetUserData_EdidIndexSave();
}
#endif

void SetSCU41_MVEdidUpdCnt(uint32_t u32Cnt)
{
    g_u32SCU41_MVEdidUpdateCnt = u32Cnt;
}

uint32_t PollSCU41_MVEdidUpdCnt(void)
{
    return g_u32SCU41_MVEdidUpdateCnt;
}

void SCU41_MVEdidUpdCntLoop(void)
{
    if(g_u32SCU41_MVEdidUpdateCnt > 0)
    {
        g_u32SCU41_MVEdidUpdateCnt --;
    }
}

/******************************************************
///Function Name:   SCU41_MV_EDIDBypassWork_Loop
///Description:     Edid bypass function loop, Best resolution function
///Input:           NULL
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void SCU41_MV_EDIDBypassWork_Loop(void)
{
    u8 l_u8InPort = 0;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

#if _ENABLE_CHIP_ITE6615
    if((PollSCU41_MVEdidUpdCnt() == 0))
    {
        u8 l_pu8EdidBuf[256] = {0};
        SetSCU41_MVEdidUpdCnt(100);

        if((ITE6615_GetReadTxEdidFlag()==TRUE))
        {
            ITE6615_SetReadTxEdidFlag(FALSE);
            cec_get_display_port_phyaddr(4 ,mapp_GetEdidPhysicalAddress(g_pu8It6615TxEdidBuffer));
            //Uart2_printf("PhysicalAddress == 0x%04x\r\n",mapp_GetEdidPhysicalAddress(g_pu8It6615TxEdidBuffer));
            for(l_u8InPort=0;l_u8InPort<_HDMI_INPUT_PORT_USED_NUMBER;l_u8InPort++)
            {
                sg_bDoBypassEdidFlag[l_u8InPort] = TRUE;
            }

            #if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
            {
                pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
                pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
                if(pstBasicSpecVar->u8ResolutionID == emRes_BestByDisplay)
                {
                    mapp_SCU41MVResolutionSwitch(emRes_BestByDisplay);
                }
            }
            #endif
        }

        for(l_u8InPort=0;l_u8InPort<_HDMI_INPUT_PORT_USED_NUMBER;l_u8InPort++)
        {
            if(sg_bDoBypassEdidFlag[l_u8InPort] == TRUE)
            {
                #if _ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL
            	if(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort] == EDID_MODE_INTERNAL3)
            	{
                    //FillIT663xxEdidDataForSingleChannel((iTE_u8 *)g_pu8It6615TxEdidBuffer, l_u8InPort);
                    mapp_EdidLimitTo4K30(g_pu8It6615TxEdidBuffer,l_pu8EdidBuf,256,FALSE,TRUE);
                    //memcpy(l_pu8EdidBuf,g_pu8It6615TxEdidBuffer,128);
                    //memcpy(l_pu8EdidBuf+128,edid_internal_2+128,128);
                    #if 0
                    {
                        u16 i;
                        Uart2_printf("g_pu8It6615TxEdidBuffer:\r\n");
                        for(i=0;i<256;i++)
                        {
                            if(i>0 && i%16==0)
                                Uart2_printf("\r\n");
                            Uart2_printf("0x%02x,",g_pu8It6615TxEdidBuffer[i]);
                        }
                        Uart2_printf("\r\n");
                        Uart2_printf("l_pu8EdidBuf:\r\n");
                        for(i=0;i<256;i++)
                        {
                            if(i>0 && i%16==0)
                                Uart2_printf("\r\n");
                            Uart2_printf("0x%02x,",l_pu8EdidBuf[i]);
                        }
                        Uart2_printf("\r\n");
                    }
                    #endif
                    FillIT663xxEdidDataForSingleChannel((iTE_u8 *)l_pu8EdidBuf, l_u8InPort);
                    IT663XX_EdidSwitchForSingleChannel(eIT663xxGetEdidFrom_Buffer, l_u8InPort);
                }
                #endif
                sg_bDoBypassEdidFlag[l_u8InPort] = FALSE;
            }
        }
    }
#endif
}

void SCU41_MV_SetPowerStateFlag(bool bstate)
{
    sg_bChangePowerStateFlag = bstate;
}

bool SCU41_MV_GetPowerStateFlag(void)
{
    return sg_bChangePowerStateFlag;
}

//When suspend, loop runing
void SCU41_MV_SuspendLoopRunning(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Dev;
    if(!pstBasicSpecVar->bPowerOnSta)
    {
    }

    if(SCU41_MV_GetPowerStateFlag() == TRUE)
    {
        SCU41_MV_SetPowerStateFlag(FALSE);

        for(l_u8Dev=0; l_u8Dev<NUM_IT663XX_USE; l_u8Dev++)
        {
            iT663XX_DevSel(IT663XX_Device[l_u8Dev]);
            //pITE663xx_STM32_api->TxOutDisable();
            pITE663xx_STM32_api->TxAdoDisable();
            ITE663XX_SpdifDisable();
            ITE663XX_I2sDisable();
        }

        #if _ENABLE_CHIP_ITE6615
        mapp_Ite6615SetPowerOffState(TRUE);
        #endif
    }
}

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
bool g_bActToDoPoweroffFlag = FALSE;
void SCU41_MV_DetectSyncTodoAction(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
	static u8 		sl_u8PreRxSignalSta = 0;
    u8 cur_temp = 0;
    bool l_bSignalChangeFlag = FALSE;
    static u8 sl_u8PreRxSel = 0;
    static u8 sl_u8PreActionState = 0xFF;
    static u16 sl_u16StartupSyncDetCnt = 40;  // unit = 50ms

    if(sl_u16StartupSyncDetCnt)
    {
        sl_u16StartupSyncDetCnt--;
        return;
    }

    #if !_ENABLE_AUTO_POWER_FUNCTION
    if(!pstBasicSpecVar->bPowerOnSta)
        return;
    #endif

    if(sl_u8PreRxSel != pstBasicSpecVar->u8WinSel[emWinType_A])
    {
        sl_u8PreRxSel = pstBasicSpecVar->u8WinSel[emWinType_A];
        if((!pstHdmiSw->bAutoSwitch)&&(pstPrjRunVar->u16RxSignalSta>0))
            l_bSignalChangeFlag = TRUE;
    }

    cur_temp = sl_u8PreRxSignalSta^(u8)pstPrjRunVar->u16RxSignalSta;
    if(cur_temp>0||l_bSignalChangeFlag)
    {
        if(!pstHdmiSw->bAutoSwitch)
        {
        #if(_ENABLE_SYNC_TO_SEND_CMD_BY_ONLY_AUTO_MODE == _MACRO_DISABLE)
            if(pstBasicSpecVar->u8WindowNum == emWinNum_1)
            {
                if((((1<<pstBasicSpecVar->u8WinSel[emWinType_A])&cur_temp) == (1<<pstBasicSpecVar->u8WinSel[emWinType_A])) // Current port is selected
                ||l_bSignalChangeFlag)
                {
                    if(l_bSignalChangeFlag)
                    {
                        if(((u8)pstPrjRunVar->u16RxSignalSta)&(1<<pstBasicSpecVar->u8WinSel[emWinType_A]))  // current has sync
                        {
                            if(sl_u8PreActionState != 1)
                            {
                                sl_u8PreActionState = 1;
                                mapp_DetectSyncToDoAction(m_action_poweron);
                                g_bActToDoPoweroffFlag = FALSE;
                                PlatformTimerSet(etDetectDisplayTimer,0);
                            }
                        }
                        else
                        {
                            if(sl_u8PreActionState != 0)
                            {
                                sl_u8PreActionState = 0;
                                //mapp_DetectSyncToDoAction(m_action_poweroff);
                                g_bActToDoPoweroffFlag = TRUE;
                                PlatformTimerSet(etDetectDisplayTimer,pstSpecVar->stBasSpecVar.u16DisplayTime*1000);
                            }
                        }
                    }
                    else
                    {
                        if((cur_temp&(u8)pstPrjRunVar->u16RxSignalSta)&(1<<pstBasicSpecVar->u8WinSel[emWinType_A]))  // current has sync
                        {
                            if(sl_u8PreActionState != 1)
                            {
                                sl_u8PreActionState = 1;
                                mapp_DetectSyncToDoAction(m_action_poweron);
                                g_bActToDoPoweroffFlag = FALSE;
                                PlatformTimerSet(etDetectDisplayTimer,0);
                            }
                        }
                        else
                        {
                            if(sl_u8PreActionState != 0)
                            {
                                sl_u8PreActionState = 0;
                                //mapp_DetectSyncToDoAction(m_action_poweroff);
                                g_bActToDoPoweroffFlag = TRUE;
                                PlatformTimerSet(etDetectDisplayTimer,pstSpecVar->stBasSpecVar.u16DisplayTime*1000);
                            }
                        }
                    }
                }
            }
        #endif
        }
        else
        {
            if((cur_temp&(u8)pstPrjRunVar->u16RxSignalSta)&&(sl_u8PreRxSignalSta == 0)&&((u8)pstPrjRunVar->u16RxSignalSta>0))
            {
                if(sl_u8PreActionState != 1)
                {
                    sl_u8PreActionState = 1;
                    mapp_DetectSyncToDoAction(m_action_poweron);
                    g_bActToDoPoweroffFlag = FALSE;
                    PlatformTimerSet(etDetectDisplayTimer,0);
                }
            }
            else if(((u8)pstPrjRunVar->u16RxSignalSta == 0) && (sl_u8PreRxSignalSta > 0))
            {
                if(sl_u8PreActionState != 0)
                {
                    sl_u8PreActionState = 0;
                    //mapp_DetectSyncToDoAction(m_action_poweroff);
                    g_bActToDoPoweroffFlag = TRUE;
                    PlatformTimerSet(etDetectDisplayTimer,pstSpecVar->stBasSpecVar.u16DisplayTime*1000);
                }
            }
        }
        sl_u8PreRxSignalSta = (u8)pstPrjRunVar->u16RxSignalSta;
    }

    if(g_bActToDoPoweroffFlag)
    {
        if(PlatformTimerExpired(etDetectDisplayTimer))
        {
            g_bActToDoPoweroffFlag = FALSE;
            mapp_DetectSyncToDoAction(m_action_poweroff);
        }
    }
}
#endif

#if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
void SCU41_MV_SetSwitchPort(u8 u8CurPort)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
    u8 i;

    sg_bEnableSwitchPortFlag = TRUE;
    sg_u8CurSwitchPort = u8CurPort;
    PlatformTimerSet(etUser_SwitchPortTimer,1500);
    for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
    {
        if((pstPrjRunVar->u16RxSignalSta & (1<<i))&&(sg_u8CurSwitchPort!=i))
        {
            led_on(led_name_hdmi_input_yellow_1+i*2);
        }
        else
        {
            led_off(led_name_hdmi_input_yellow_1+i*2);
        }

        if(sg_u8CurSwitchPort==i)
        {
            led_on(led_name_hdmi_input_blue_1+i*2);
            led_off(led_name_hdmi_input_yellow_1+i*2);
        }
        else
        {
            led_off(led_name_hdmi_input_blue_1+i*2);
        }
    }
}

u8 SCU41_MV_GetSwitchPort(void)
{
    return sg_u8CurSwitchPort;
}

void SCU41_MV_SetSwitchPortOnly(u8 u8CurPort)
{
    sg_u8CurSwitchPort = u8CurPort;
}

void SCU41_MV_SwitchPortFunction(void)
{
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(sg_bEnableSwitchPortFlag == TRUE)
    {
        if(PlatformTimerExpired(etUser_SwitchPortTimer))
        {
            sg_bEnableSwitchPortFlag = FALSE;
            sg_bEnableSelectPort = FALSE;

            #if (!_ENABLE_SWITCH_LED_TIMER)
            if(pstBasicVar->u8WindowNum == emWinNum_1)
            {
            }
            else
            {
                led_off(led_name_hdmi_input_blue_1+(sg_u8InputPortSel)*2);
                led_off(led_name_window_A+sg_u8WinPortSel);
                delay_ms(50);
                SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
            }
            #endif
            pITE663xx_STM32_api->SysKeyIn(sg_u8InputPortSel+emKeyFunc_SwitchHdmi1);
        }
    }
    else
    {
        if(sg_u8CurSwitchPort!=pstPrjSaveVar->stHdmiSel.u8SwChannel)
        {
            sg_u8CurSwitchPort = pstPrjSaveVar->stHdmiSel.u8SwChannel;
        }

        if(sg_bEnableSelectPort)
        {
            if(PlatformTimerExpired(etUser_SwitchPortTimer))
            {
                sg_bEnableSelectPort = FALSE;
                led_off(led_name_hdmi_input_blue_1+(sg_u8InputPortSel)*2);
                SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
            }
        }
    }

    #if _ENABLE_SWITCH_LED_TIMER
    if(mapp_SCU41MVGetSwitchLedFlag())
    {
        if(PlatformTimerExpired(etUser_SwitchLedTimer))
        {
            PlatformTimerSet(etUser_SwitchLedTimer,0);
            mapp_SCU41MVSetSwitchLedFlag(FALSE);

            if(pstBasicVar->u8WindowNum == emWinNum_1)
            {
            }
            else
            {
                led_off(led_name_hdmi_input_blue_1+(sg_u8InputPortSel)*2);
                led_off(led_name_window_A+sg_u8WinPortSel);
                delay_ms(50);
                SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
            }
        }
    }

    if(mapp_SCU41MVGetCheckPortFlag())
    {
        if(PlatformTimerExpired(etUser_SwitchLedTimer))
        {
            PlatformTimerSet(etUser_SwitchLedTimer,0);
            mapp_SCU41MVSetCheckPortFlag(FALSE);
            led_off(led_name_hdmi_input_blue_1+(mapp_SCU41MVGetCheckWinToSrcPort())*2);
            //SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
        }
    }
    #endif
}

void SCU41_MV_SaveInputPortKeyPress(u8 u8InputPort)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i;
    for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
    {
        if((pstPrjRunVar->u16RxSignalSta & (1<<i))&&(u8InputPort!=i))
        {
            led_on(led_name_hdmi_input_yellow_1+i*2);
        }
        else
        {
            led_off(led_name_hdmi_input_yellow_1+i*2);
        }
    }
    if(pstBasicVar->u8WindowNum == emWinNum_1)
    {
        sg_u8InputPortSel = u8InputPort;
        led_off(led_name_hdmi_input_blue_1);
        led_off(led_name_hdmi_input_blue_2);
        led_off(led_name_hdmi_input_blue_3);
        led_off(led_name_hdmi_input_blue_4);
        led_on(led_name_hdmi_input_blue_1+(u8InputPort)*2);
        sg_u8WinPortSel = emWinType_A;
        sg_bEnableSwitchPortFlag = TRUE;
        PlatformTimerSet(etUser_SwitchPortTimer,500);
    }
    else
    {
        if(sg_bEnableSwitchPortFlag==FALSE)
        {
            sg_u8InputPortSel = u8InputPort;
            led_off(led_name_hdmi_input_blue_1);
            led_off(led_name_hdmi_input_blue_2);
            led_off(led_name_hdmi_input_blue_3);
            led_off(led_name_hdmi_input_blue_4);
            led_on(led_name_hdmi_input_blue_1+(u8InputPort)*2);
            SCU41_MV_SetWinLedMode(emWinLightMode_NomalBlink,SCU41_MV_GetWinPortSelect());
            PlatformTimerSet(etUser_SwitchPortTimer,5000);
            sg_bEnableSelectPort = TRUE;
        }
    }
}


void SCU41_MV_SetInputPortSelect(u8 u8InPort)
{
    sg_u8InputPortSel = u8InPort;
}

u8 SCU41_MV_GetInputPortSelect(void)
{
    return sg_u8InputPortSel;
}

void SCU41_MV_SelectPortToWin(u8 u8Win)
{
    if((sg_bEnableSelectPort)&&(sg_bEnableSwitchPortFlag==FALSE))
    {
        sg_u8WinPortSel = u8Win;
        SCU41_MV_SetWinLedMode(emWinLightMode_Off,SCU41_MV_GetWinPortSelect());
        led_on(led_name_window_A+sg_u8WinPortSel);
        delay_ms(50);
        led_norm_blink(led_name_hdmi_input_blue_1+(sg_u8InputPortSel)*2);
        led_norm_blink(led_name_window_A+sg_u8WinPortSel);
        #if _ENABLE_SWITCH_LED_TIMER
        PlatformTimerSet(etUser_SwitchLedTimer,3500);
        mapp_SCU41MVSetSwitchLedFlag(TRUE);
        #endif
        PlatformTimerSet(etUser_SwitchPortTimer,500);
        sg_bEnableSwitchPortFlag = TRUE;
    }
}

void SCU41_MV_SetWinPortSelect(u8 u8WinPort)
{
    sg_u8WinPortSel = u8WinPort;
}

u8 SCU41_MV_GetWinPortSelect(void)
{
    return sg_u8WinPortSel;
}

bool SCU41_MV_GetSelectPortFlag(void)
{
    return sg_bEnableSelectPort;
}

u8 mapp_SCU41MVGetTheWinOfCurSrc(u8 u8Src)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8WinId;

    for(l_u8WinId = 0; l_u8WinId < emWinTypeMax; l_u8WinId++)
    {
        if(pstBasicVar->u8WinSel[l_u8WinId] == u8Src)
        {
            return l_u8WinId;
        }
    }

    return 0;
}


void mapp_SCU41MVSetCheckPortFlag(bool bEn)
{
    sg_bEnableCheckPort = bEn;
}

bool mapp_SCU41MVGetCheckPortFlag(void)
{
    return sg_bEnableCheckPort;
}

void mapp_SCU41MVSetCheckWinToSrcPort(u8 u8Port)
{
    sg_u8CheckWinPort = u8Port;
}

u8 mapp_SCU41MVGetCheckWinToSrcPort(void)
{
    return sg_u8CheckWinPort;
}

#if _ENABLE_SWITCH_LED_TIMER
void mapp_SCU41MVSetSwitchLedFlag(bool bEn)
{
    sg_bEnableSwitchLed = bEn;
}

bool mapp_SCU41MVGetSwitchLedFlag(void)
{
    return sg_bEnableSwitchLed;
}
#endif

#endif

#define SCU41MV_SIGNAL_EXIST_DETECT_TIMES       20
#define SCU41MV_POWER_ON_DETECT_TIMES           5
void mapp_SCU41MVAutoSwitchProcess(void)
{
	iTE_u8 				i, j, rx_signal_sta = 0, cur_temp = 0, u8IteDevSave;
	iTE_u8 				u8Reg13[NUM_IT663XX_USE][IN_RXn], u8RxTmp;
	iTE_u8 				rx_hotplug_sta = 0;
	static iTE_u8 		pre_rx_signal_sta = 0;
	static bool 		pre_isAutoSwitchMode = TRUE;

	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	static iTE_u8 		pre_rx_hotplug_sta = 0;
	static iTE_u8 		signal_exist_cnt_3[SCU41MV_SIGNAL_EXIST_DETECT_TIMES] = {0}, exist_cnt_x=0;
	static iTE_u8	    u8PowerOnDetectNum = 0;
	#endif

	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalRunVar_t 	pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    #if Project_SCU61E_66341_AUTO_SWITCH
	if(!PollAutoSwDetectStatus()) return;//auto switch detect time
    #endif

	u8IteDevSave = g_it663xx_device;

	//get the value of reg 0x13
	for(i=0; i<NUM_IT663XX_USE; i++)
	{
		iT663XX_DevSel(IT663XX_Device[i]);
		//for(j=IN_RX0; j<IN_RXn; j++)
		j=0;
		{
			if(HRx_Select(j))
			{
				u8Reg13[i][j] = HRx_R(0x13);
			}
		}
	}

	iT663XX_DevSel(u8IteDevSave);

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(isRxHpdDetNull(i))
		{
			if((u8Reg13[i][0]&0x01) ||(u8Reg13[i][0]&0x40))
			{
				rx_hotplug_sta |= (1<<i);
			}
		}
		else
		{
			if((isGetHpdDetConnect(i)) ||(u8Reg13[i][0]&0x40))
			{
				rx_hotplug_sta |= (1<<i);
			}
		}

		#if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)
		//HDMI signal
		if((u8Reg13[i][0]&0x04)&&(u8Reg13[i][0]&0x08)&&\
			(u8Reg13[i][0]&0x10)&&(rx_hotplug_sta&(1<<i)))
		{
			rx_signal_sta |= (1<<i);
		}
		#elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD)
		if(rx_hotplug_sta&(1<<i))
		{
			rx_signal_sta |= (1<<i);
		}
        #elif (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
        if(g_emDetectMode == emHPD)
        {
            if(rx_hotplug_sta&(1<<i))
			{
				rx_signal_sta |= (1<<i);
			}
        }
        else
        {
            //HDMI signal
            if((u8Reg13[i][0]&0x04)&&(u8Reg13[i][0]&0x08)&&\
                (u8Reg13[i][0]&0x10)&&(rx_hotplug_sta&(1<<i)))
            {
                rx_signal_sta |= (1<<i);
            }
        }
		#endif
	}
	#if ((_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_TMDS)\
        ||(_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS))
	cur_temp = pre_rx_hotplug_sta^rx_hotplug_sta;
	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		if(((cur_temp&(1<<i))>0)&&((rx_hotplug_sta&(1<<i))==0))//from 1 to 0
		{
			for(j=0; j<SCU41MV_SIGNAL_EXIST_DETECT_TIMES; j++)
			{
				signal_exist_cnt_3[j] &= (~(1<<i));
			}
			rx_signal_sta &= (~(1<<i));
		}
	}

	pre_rx_hotplug_sta = rx_hotplug_sta;

	signal_exist_cnt_3[exist_cnt_x++] = rx_signal_sta;

	if(exist_cnt_x >= SCU41MV_SIGNAL_EXIST_DETECT_TIMES)
	{
		exist_cnt_x = 0;
	}

	for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
	{
		for(j=0; j<SCU41MV_SIGNAL_EXIST_DETECT_TIMES; j++)
		{
			if(signal_exist_cnt_3[j] & (1<<i))
			{
				rx_signal_sta |= 1<<i;
			}
		}
	}

	if(u8PowerOnDetectNum < SCU41MV_POWER_ON_DETECT_TIMES)
	{
		u8PowerOnDetectNum ++;
		rx_signal_sta = 0;
	}
	#endif

	pstPrjRunVar->u16RxSignalSta = rx_signal_sta;
	//Dbg_UserFun(("Rx port signal status is %d\r\n", rx_signal_sta));

	//u8RxTmp = pstPrjSaveVar->stHdmiSel.u8SwChannel;//the current select
    u8RxTmp = pstBasicVar->u8WinSel[emWinType_A];

	if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch) // isn't the auto switch mode
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = rx_signal_sta;
		return;
	}

    if(pstBasicVar->u8WindowNum != emWinNum_1)
    {
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = rx_signal_sta;
        return ;
    }

	if((!pre_isAutoSwitchMode)&&(pstPrjSaveVar->stHdmiSel.bAutoSwitch))
	{
		pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
		pre_rx_signal_sta = 0;
	}

	if(pre_rx_signal_sta != rx_signal_sta)
	{
		if((g_bAutoSwPowerOnStatus)&&(rx_signal_sta&(1<<u8RxTmp)))
		{
			g_bAutoSwPowerOnStatus = FALSE;
		}
		else
		{
			g_bAutoSwPowerOnStatus = FALSE;
			cur_temp = pre_rx_signal_sta ^ rx_signal_sta;
			//Uart2_printf("rx signal change, rx_signal_sta = 0x%2x, cur_temp = 0x%2x\r\n", rx_signal_sta, cur_temp);
			for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
			{
				if(cur_temp & (1 << i))
				{
					if(rx_signal_sta & (1<<i))
					{
						//Uart2_printf("plug in: auto switch to port %d\r\n", i);

                        g_emSourceKeyType = eKeyValSource_Uart;
                        SCU41_MV_SaveInputPortKeyPress(i);
						pre_rx_signal_sta = rx_signal_sta;
						return;
					}
					else
					{
						if(i == u8RxTmp)
						{
							//iTE_MsgA(("signal missed, switch to next:"));
							for(j=0; j<_HDMI_INPUT_PORT_USED_NUMBER; j++)
							{
								if(rx_signal_sta & (1<<j))
								{
									//Uart2_printf("plug out: auto switch to port %d\r\n", j);

                                    g_emSourceKeyType = eKeyValSource_Uart;
                                    SCU41_MV_SaveInputPortKeyPress(j);

									pre_rx_signal_sta = rx_signal_sta;
									return;
								}
							}
						}
					}
				}
			}
		}
		pre_rx_signal_sta = rx_signal_sta;
	}
}

#if _ENABLE_START_UP_TO_ENABLE_EXTERNAL_AUDIO
void SCU41_MV_SetExternalAudio(void)
{
    static bool sl_bStartupToDoAudioFlag = TRUE;

    if(sl_bStartupToDoAudioFlag == TRUE)
    {
        if(PlatformTimerExpired(etUser_StartupAudioTimer))
        {
            sl_bStartupToDoAudioFlag = FALSE;
            mapp_SetExternalAudioMute(FALSE);
        }
    }
}
#endif

void SCU41_MV_SetWinLedMode(u8 u8Mode,u8 u8WinSel)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    switch(u8Mode)
    {
        case emWinLightMode_On:
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_4:
                    {
                        led_on(led_name_window_A+u8WinSel);
                        led_on(led_name_window_A+(u8WinSel+1)%4);
                        led_on(led_name_window_A+(u8WinSel+2)%4);
                        led_on(led_name_window_A+(u8WinSel+3)%4);
                    }
                    break;
                case emWinNum_3:
                    {
                        led_on(led_name_window_A+u8WinSel);
                        led_on(led_name_window_A+(u8WinSel+1)%3);
                        led_on(led_name_window_A+(u8WinSel+2)%3);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_2:
                    {
                        led_on(led_name_window_A+u8WinSel);
                        led_on(led_name_window_A+(u8WinSel+1)%2);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_1:
                    {
                        led_on(led_name_window_A);
                        led_off(led_name_window_B);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                default:
                    break;
            }
            break;
        case emWinLightMode_Off:
            {
                led_off(led_name_window_A);
                led_off(led_name_window_B);
                led_off(led_name_window_C);
                led_off(led_name_window_D);
            }
            break;
        case emWinLightMode_NomalBlink:
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_4:
                    {
                        led_norm_blink(led_name_window_A+u8WinSel);
                        led_norm_blink(led_name_window_A+(u8WinSel+1)%4);
                        led_norm_blink(led_name_window_A+(u8WinSel+2)%4);
                        led_norm_blink(led_name_window_A+(u8WinSel+3)%4);
                    }
                    break;
                case emWinNum_3:
                    {
                        led_norm_blink(led_name_window_A+u8WinSel);
                        led_norm_blink(led_name_window_A+(u8WinSel+1)%3);
                        led_norm_blink(led_name_window_A+(u8WinSel+2)%3);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_2:
                    {
                        led_norm_blink(led_name_window_A+u8WinSel);
                        led_norm_blink(led_name_window_A+(u8WinSel+1)%2);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_1:
                    {
                        led_on(led_name_window_A);
                        led_off(led_name_window_B);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                default:
                    break;
            }
            break;
        case emWinLightMode_FastBlink:
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_4:
                    {
                        led_fast_blink(led_name_window_A+u8WinSel);
                        led_fast_blink(led_name_window_A+(u8WinSel+1)%4);
                        led_fast_blink(led_name_window_A+(u8WinSel+2)%4);
                        led_fast_blink(led_name_window_A+(u8WinSel+3)%4);
                    }
                    break;
                case emWinNum_3:
                    {
                        led_fast_blink(led_name_window_A+u8WinSel);
                        led_fast_blink(led_name_window_A+(u8WinSel+1)%3);
                        led_fast_blink(led_name_window_A+(u8WinSel+2)%3);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_2:
                    {
                        led_fast_blink(led_name_window_A+u8WinSel);
                        led_fast_blink(led_name_window_A+(u8WinSel+1)%2);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_1:
                    {
                        led_on(led_name_window_A);
                        led_off(led_name_window_B);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                default:
                    break;
            }
            break;
        case emWinLightMode_SlowBlink:
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_4:
                    {
                        led_slow_blink(led_name_window_A+u8WinSel);
                        led_slow_blink(led_name_window_A+(u8WinSel+1)%4);
                        led_slow_blink(led_name_window_A+(u8WinSel+2)%4);
                        led_slow_blink(led_name_window_A+(u8WinSel+3)%4);
                    }
                    break;
                case emWinNum_3:
                    {
                        led_slow_blink(led_name_window_A+u8WinSel);
                        led_slow_blink(led_name_window_A+(u8WinSel+1)%3);
                        led_slow_blink(led_name_window_A+(u8WinSel+2)%3);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_2:
                    {
                        led_slow_blink(led_name_window_A+u8WinSel);
                        led_slow_blink(led_name_window_A+(u8WinSel+1)%2);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_1:
                    {
                        led_on(led_name_window_A);
                        led_off(led_name_window_B);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                default:
                    break;
            }
            break;
        #if _ENABLE_LED_WORK_MOMENT_3S
        case emWinLightMode_Moment_on_3s:
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_4:
                    {
                        //led_moment_on_3s(led_name_window_A+u8WinSel);
                        led_moment_on_3s(led_name_window_A+(u8WinSel+1)%4);
                        led_moment_on_3s(led_name_window_A+(u8WinSel+2)%4);
                        led_moment_on_3s(led_name_window_A+(u8WinSel+3)%4);
                    }
                    break;
                case emWinNum_3:
                    {
                        //led_moment_on_3s(led_name_window_A+u8WinSel);
                        led_moment_on_3s(led_name_window_A+(u8WinSel+1)%3);
                        led_moment_on_3s(led_name_window_A+(u8WinSel+2)%3);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_2:
                    {
                        //led_moment_on_3s(led_name_window_A+u8WinSel);
                        led_moment_on_3s(led_name_window_A+(u8WinSel+1)%2);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_1:
                    {
                        led_on(led_name_window_A);
                        led_off(led_name_window_B);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                default:
                    break;
            }
            break;
        case emWinLightMode_Moment_off_3s:
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_4:
                    {
                        //led_moment_on_3s(led_name_window_A+u8WinSel);
                        led_moment_off_3s(led_name_window_A+(u8WinSel+1)%4);
                        led_moment_off_3s(led_name_window_A+(u8WinSel+2)%4);
                        led_moment_off_3s(led_name_window_A+(u8WinSel+3)%4);
                    }
                    break;
                case emWinNum_3:
                    {
                        //led_moment_on_3s(led_name_window_A+u8WinSel);
                        led_moment_off_3s(led_name_window_A+(u8WinSel+1)%3);
                        led_moment_off_3s(led_name_window_A+(u8WinSel+2)%3);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_2:
                    {
                        //led_moment_on_3s(led_name_window_A+u8WinSel);
                        led_moment_off_3s(led_name_window_A+(u8WinSel+1)%2);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                case emWinNum_1:
                    {
                        led_on(led_name_window_A);
                        led_off(led_name_window_B);
                        led_off(led_name_window_C);
                        led_off(led_name_window_D);
                    }
                    break;
                default:
                    break;
            }
            break;
        #endif

        default:
            break;
    }
}

// the status of led when running system
void SCU41_MV_FuncWork_LoopRunning(void)
{
    SCU41_MV_TxPlugInOrOutPro_Loop();
    SCU41_MV_LedWork_Loop();
    SCU41_MV_SpdifIisWork_Loop();
    SCU41_MV_EDIDBypassWork_Loop();

    #if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
    SCU41_MV_DetectSyncTodoAction();
    #endif

    #if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
    SCU41_MV_SwitchPortFunction();
    #endif
    #if _ENABLE_START_UP_TO_ENABLE_EXTERNAL_AUDIO
    SCU41_MV_SetExternalAudio();
    #endif
    mapp_SCU41MVAutoSwitchProcess();

    mapp_SCU41MVAudioSrcSwitchProcess();
    mapp_SCU41MVSrcChangeToSetWinProcess();
    mapp_SCU41MVGetWinCfgUpdProcess();
    #if 0//_ENABLE_AUTO_POWER_FUNCTION
    mapp_SCU41MVAutoPowerProcess();
    #endif
}

void SetUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

void SCU41_MV_AllLedOff(void)
{
    led_off(led_name_hdmi_input_yellow_1);
    led_off(led_name_hdmi_input_blue_1);
    led_off(led_name_hdmi_input_yellow_2);
    led_off(led_name_hdmi_input_blue_2);
    led_off(led_name_hdmi_input_yellow_3);
    led_off(led_name_hdmi_input_blue_3);
    led_off(led_name_hdmi_input_yellow_4);
    led_off(led_name_hdmi_input_blue_4);
    led_off(led_name_switch_auto);
    led_off(led_name_window_A);
    led_off(led_name_window_B);
    led_off(led_name_window_C);
    led_off(led_name_window_D);
    led_off(led_name_swap);
    led_off(led_name_resize);
    led_off(led_name_multiview_mode_1);
    led_off(led_name_multiview_mode_2);
    led_off(led_name_multiview_mode_3);
    led_off(led_name_window_A);
    led_off(led_name_hdmi_input_yellow_1);
    led_off(led_name_hdmi_input_blue_1);
    led_off(led_name_hdmi_input_yellow_2);
    led_off(led_name_hdmi_input_blue_2);
    led_off(led_name_hdmi_input_yellow_3);
    led_off(led_name_hdmi_input_blue_3);
    led_off(led_name_hdmi_input_yellow_4);
    led_off(led_name_hdmi_input_blue_4);
    led_off(led_name_switch_auto);
    led_off(led_name_window_A);
    led_off(led_name_window_B);
    led_off(led_name_window_C);
    led_off(led_name_window_D);
    led_off(led_name_swap);
    led_off(led_name_resize);
    led_off(led_name_multiview_mode_1);
    led_off(led_name_multiview_mode_2);
    led_off(led_name_multiview_mode_3);
    led_off(led_name_power_Green);
    led_off(led_name_power_red);
}

void mapp_SCU41MV_SetOutPort5V(bool bEnable)
{
	pst5VControl_t p5vCtlPort = (pst5VControl_t)stProjectTx5VControlTable;
    while(p5vCtlPort->pstControlPort != NULL)
    {
		if(p5vCtlPort->pstControlPort->eInitLevel == _GPIO_PIN_LEVEL_LOW)
		{
			if(bEnable)
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		else
		{
			if(bEnable)
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		p5vCtlPort++;
    }
}

void SCU41_MV_SetPowerDownFunction(void)
{
    IT663XXProResumeOrSuspend(TRUE);
    SCU41_MV_SetPowerStateFlag(TRUE);
    SCU41_MV_AllLedOff();
    KeyPressCallBack(emKeyFunc_NoMatchChipID);
    #if _ENABLE_CHIP_ITE6615
    iTE6615_SetTX_Power(POWER_OFF);
    #endif
    mapp_SelectFpgaFlashUpdateMode(TRUE);
    mapp_SCU41MV_SetOutPort5V(FALSE);
    delay_ms(2000);
    mapp_SetSysResetPin(0);
}

void mapp_SCU41_MVStartupMessage(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    Uart3_printf("\r\n");
    RS232_printf("\r\n");
    Uart1_printf("\r\n");
    delay_ms(10);
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    Uart1_printf("System Initializing...\r\n");
    Uart1_printf("\r\n");
    Uart1_printf("Initialization Finished!\r\n");
    //RS232_printf(_STRING_MANF_NAME);
    if(pstPrjSaveVar->stPrjNameSet.m_u8Len)
    {
        RS232_printf(pstPrjSaveVar->stPrjNameSet.sPrjName);
        RS232_printf("\r\n");
        Uart1_printf(pstPrjSaveVar->stPrjNameSet.sPrjName);
        Uart1_printf("\r\n");
    }
    RS232_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
    Uart1_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
    g_emSourceKeyType = eKeyValSource_Uart;
    KeyPressCallBack(emKeyFunc_AutoSwitch);
    KeyPressCallBack(emKeyFunc_Resolution);
    KeyPressCallBack(emKeyFunc_GetOutputSate);
}

void SCU41_MV_UpdateWindowNum(u8 u8MvMode)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    switch(u8MvMode)
    {
        case emMvMode_4Win_SameSize:
        case emMvMode_4Win_PBP_3L1R:
        case emMvMode_4Win_PBP_1L3R:
        case emMvMode_4Win_PBP_3U1D:
        case emMvMode_4Win_PBP_1U3D:
        case emMvMode_4Win_PIP_1F3L:
        case emMvMode_4Win_PIP_1F3R:
        case emMvMode_4Win_PIP_1F3U:
        case emMvMode_4Win_PIP_1F3D:
            pstBasicVar->u8WindowNum = emWinNum_4;
            break;

        case emMvMode_2Win_PBP:
        case emMvMode_2Win_PIP_LU:
        case emMvMode_2Win_PIP_LD:
        case emMvMode_2Win_PIP_RU:
        case emMvMode_2Win_PIP_RD:
            pstBasicVar->u8WindowNum = emWinNum_2;
            break;
        case emMvMode_3Win_2U1D:
            pstBasicVar->u8WindowNum = emWinNum_3;
            break;
        case emMvMode_1Win_Full:
            pstBasicVar->u8WindowNum = emWinNum_1;
            break;
        case emMvMode_xWin_ByUser1:
        case emMvMode_xWin_ByUser2:
        case emMvMode_xWin_ByUser3:
        case emMvMode_xWin_ByUser4:
            {
                pstBasicVar->u8WindowNum = emWinNum_4;
            }
            break;

        default:
            break;
    }
}

void mapp_SCU41MVCopyUserModeSrcToNomalMode(u8 *pu8UserSrc)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Win;

    for(l_u8Win=0;l_u8Win<emWinTypeMax;l_u8Win++)
    {
        pstBasicVar->u8WinSel[l_u8Win] = pu8UserSrc[l_u8Win];
    }
    SetUserData_SpecBasicVar();
}

void SCU41_MV_SetMultiViewMode(u8 u8WinMode)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
    {
        u8 l_u8UserMode = pstBasicVar->u8MvMode - emMvMode_xWin_ByUser1;
        mapp_SCU41MVCopyUserModeSrcToNomalMode(pstBasicVar->stUserWinsConfig[l_u8UserMode].u8UserSrc);
    }
    if(pstBasicVar->u8WindowNum != emWinNum_1)
    {
        led_off(led_name_hdmi_input_blue_1);
        led_off(led_name_hdmi_input_blue_2);
        led_off(led_name_hdmi_input_blue_3);
        led_off(led_name_hdmi_input_blue_4);
    }
    else
    {
        led_on(led_name_hdmi_input_blue_1+pstBasicVar->u8WinSel[emWinType_A]*2);
    }
    Dbg_UserFun(("winmode = %d \r\n",u8WinMode));
    sg_u8ResizeMode = 0;
    mapp_SetCfgMode(emCfgMode_None);
    pstBasicVar->u8ResizeMode = emResize_None;
    SetUserData_SpecBasicVar();
    //Uart2_printf("mapp_SCU41MVMultiWinUpdate 1111\r\n");;
    mapp_SCU41MVMultiWinUpdate((u8)emSrcInputMax,(u8)emWinTypeMax,TRUE);
}

void SCU41_MV_SetDisplayWin(u8 u8Input,u8 u8Win,bool bUpd)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    Dbg_UserFun(("input = %d, win = %d \r\n",u8Input,u8Win));

    //Uart2_printf("mapp_SCU41MVMultiWinUpdate 222\r\n");;
    if((u8Input == emSrcInputMax)||(u8Win==emWinTypeMax))
        mapp_SCU41MVMultiWinUpdate((u8)emSrcInputMax,(u8)emWinTypeMax,bUpd);//(u8Input);
    else
    {
        pstBasicVar->u8WinSel[u8Win] = u8Input;
        mapp_SCU41MVMultiWinUpdate(u8Input,u8Win,bUpd);
    }
    SetUserData_SpecBasicVar();
}

void mapp_SCU41MVSetAudioInput(u8 u8ADSrc)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8AudioSrc = (1<<u8ADSrc);//u8ADSrc+1;

    pstBasicVar->u8AudioSel = u8ADSrc;
    Dbg_UserFun(("AudioSrc = %d\r\n",u8ADSrc));

    mapp_SCU41MV_AudioOutputMute(TRUE);
    SCU41_MV_SendCmd(0x00,FPGA_REG_ADSRC,1,&l_u8AudioSrc);
    SetUserData_SpecBasicVar();
    mapp_SCU41MV_AudioOutputMute(pstBasicVar->bAudioMute);
}

void mapp_SCU41MVSetAudioMix(bool bEn)
{
	//pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	//pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8AudioMixSta;

    //pstBasicVar->bAudioMix = bEn;
    Dbg_UserFun(("AudioMix = %d\r\n",bEn));

    if(bEn)
    {
        l_u8AudioMixSta = 1;
    }
    else
    {
        l_u8AudioMixSta = 0;
    }

    SCU41_MV_SendCmd(0x00,FPGA_REG_ADMIX,1,&l_u8AudioMixSta);
    //SetUserData_SpecBasicVar();
}

void mapp_SCU41MVSetAudioMute(bool bEn)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    //u8 l_u8AudioMuteSta;

    Dbg_UserFun(("AudioMute = %d\r\n",bEn));

    pstBasicVar->bAudioMute = bEn;
    mapp_SCU41MV_AudioOutputMute(bEn);
    SetUserData_SpecBasicVar();
}

void mapp_SCU41MV_AudioOutputMute(bool bEn)
{
    u8 l_u8AudioMuteSta;

    if(bEn)
    {
        l_u8AudioMuteSta = 1;
    }
    else
    {
        l_u8AudioMuteSta = 0;
    }

    SCU41_MV_SendCmd(0x00,FPGA_REG_ADMUTE,1,&l_u8AudioMuteSta);
}

void mapp_SCU41MVAudioSrcSwitchEnable(bool bEn)
{
    #if _ENABLE_AUDIO_SOURCE_SWITCH_TIMER
    if(bEn)
    {
        PlatformTimerSet(etUser_AudioSrcSwitchTimer,3000);
    }
    else
    {
        PlatformTimerSet(etUser_AudioSrcSwitchTimer,0);
    }
    #endif
    sg_bAudioSrcSelFlag = bEn;
}

bool mapp_SCU41MVGetAudioSrcSwitchStatus(void)
{
    return sg_bAudioSrcSelFlag;
}

void mapp_SCU41MVAudioSrcSwitchProcess(void)
{
    #if _ENABLE_AUDIO_SOURCE_SWITCH_TIMER
    if(mapp_SCU41MVGetAudioSrcSwitchStatus())
    {
        if(PlatformTimerExpired(etUser_AudioSrcSwitchTimer))
        {
            mapp_SCU41MVAudioSrcSwitchEnable(FALSE);
        }
    }
    #endif
}

void mapp_SCU41MVChangeWinSize(u8 u8UserMode,
                                        u8 u8WinPort,
                                        u8 u8Src,
                                        u8 u8StartX,
                                        u8 u8StartY,
                                        u8 u8EndX,
                                        u8 u8EndY)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(u8UserMode)
    {
        mapp_SetCfgMode(emCfgMode_None);
        if(u8Src > 0)
        {
            pstBasicVar->stUserWinsConfig[u8UserMode-1].u8UserSrc[u8WinPort] = u8Src-1;
            pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EnWinSta[u8WinPort] = 1;
        }
        else
            pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EnWinSta[u8WinPort] = 0;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8StartX[u8WinPort] = u8StartX;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8StartY[u8WinPort] = u8StartY;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EndX[u8WinPort] = u8EndX;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EndY[u8WinPort] = u8EndY;
        #if 0
        switch(pstBasicVar->u8MvMode)
        {
            case emMvMode_xWin_ByUser1:
            case emMvMode_xWin_ByUser2:
            case emMvMode_xWin_ByUser3:
            case emMvMode_xWin_ByUser4:
                if((pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1)== (u8UserMode-1))
                {
                    SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
                    //SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
                    //SCU41_MV_SetMultiViewMode(pstBasicVar->u8MvMode);
                    mapp_SCU41MVMultiWinUpdate(pstBasicVar->stUserWinsConfig[u8UserMode-1].u8UserSrc[u8WinPort],u8WinPort-1,TRUE);//((u8)emSrcInputMax,TRUE);
                }
            break;
        }
        #endif
        SetUserData_SpecBasicVar();
    }
    else
    {
        mapp_SetCfgMode(emCfgMode_Percent);
        if(u8Src > 0)
        {
            g_stUserWinsConfig.u8UserSrc[u8WinPort] = u8Src-1;
            g_stUserWinsConfig.u8EnWinSta[u8WinPort] = 1;
        }
        else
            g_stUserWinsConfig.u8EnWinSta[u8WinPort] = 0;
        g_stUserWinsConfig.u8StartX[u8WinPort] = u8StartX;
        g_stUserWinsConfig.u8StartY[u8WinPort] = u8StartY;
        g_stUserWinsConfig.u8EndX[u8WinPort] = u8EndX;
        g_stUserWinsConfig.u8EndY[u8WinPort] = u8EndY;
        //SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
        mapp_SCU41MVMultiWinUpdate(g_stUserWinsConfig.u8UserSrc[u8WinPort],u8WinPort,TRUE);//((u8)emSrcInputMax,TRUE);
    }
}

void mapp_SCU41MVChangeCfgPixelWinSize(u8 u8UserMode,
                                        u8 u8WinPort,
                                        u8 u8Src,
                                        u16 u16StartX,
                                        u16 u16StartY,
                                        u16 u16Width,
                                        u16 u16Height)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(u8UserMode)
    {
        mapp_SetCfgMode(emCfgMode_None);
        if(u8Src > 0)
        {
            pstBasicVar->stUserWinsConfig[u8UserMode-1].u8UserSrc[u8WinPort] = u8Src-1;
            pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EnWinSta[u8WinPort] = 1;
        }
        else
            pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EnWinSta[u8WinPort] = 0;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8StartX[u8WinPort] = (u16StartX*100)/g_stTiming.u16HActive;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8StartY[u8WinPort] = (u16StartY*100)/g_stTiming.u16VActive;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EndX[u8WinPort] = ((u16Width+u16StartX)*100)/g_stTiming.u16HActive;
        pstBasicVar->stUserWinsConfig[u8UserMode-1].u8EndY[u8WinPort] = ((u16Height+u16StartY)*100)/g_stTiming.u16VActive;
        SetUserData_SpecBasicVar();
    }
    else
    {
        mapp_SetCfgMode(emCfgMode_Pixel);
        if(u8Src > 0)
        {
            g_stUserWinPixelCfg.u8UserSrc[u8WinPort] = u8Src-1;
            g_stUserWinPixelCfg.u8EnWinSta[u8WinPort] = 1;
        }
        else
            g_stUserWinPixelCfg.u8EnWinSta[u8WinPort] = 0;
        g_stUserWinPixelCfg.u16StartX[u8WinPort] = u16StartX;
        g_stUserWinPixelCfg.u16StartY[u8WinPort] = u16StartY;
        g_stUserWinPixelCfg.u16Width[u8WinPort] = u16Width;
        g_stUserWinPixelCfg.u16Heigh[u8WinPort] = u16Height;
        //SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
        mapp_SCU41MVMultiWinUpdate(g_stUserWinPixelCfg.u8UserSrc[u8WinPort],u8WinPort,TRUE);//((u8)emSrcInputMax,TRUE);
        //UNUSED(u8UserMode);
    }
}

void SCU41_MV_SetResizeFunction(u8 u8WinMode)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 u8ResizeStart = 0;

    switch(u8WinMode)
    {
        case emMvMode_2Win_PBP:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%4;
                u8ResizeStart = emResize_2Win_PBP_1;
            }
            break;

        case emMvMode_2Win_PIP_LU:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_2Win_PIP_LU_1;
            }
            break;
        case emMvMode_2Win_PIP_LD:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_2Win_PIP_LD_1;
            }
            break;
        case emMvMode_2Win_PIP_RU:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_2Win_PIP_RU_1;
            }
            break;
        case emMvMode_2Win_PIP_RD:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_2Win_PIP_RD_1;
            }
            break;
        case emMvMode_4Win_PBP_3L1R:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_4Win_PBP_3L1R_1;
            }
            break;
        case emMvMode_4Win_PBP_1L3R:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_4Win_PBP_1L3R_1;
            }
            break;
        case emMvMode_4Win_PBP_3U1D:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_4Win_PBP_3U1D_1;
            }
            break;
        case emMvMode_4Win_PBP_1U3D:
            {
                Dbg_UserFun(("u8WinMode = %d, sl_u8ResizeNum = %d \r\n",u8WinMode,sg_u8ResizeMode));
                sg_u8ResizeMode = (sg_u8ResizeMode+1)%3;
                u8ResizeStart = emResize_4Win_PBP_1U3D_1;
            }
            break;

        default:
            sg_u8ResizeMode = 0;
            //Uart2_printf("Unsupport Resize!!!\r\n");
            KeyPressCallBack(emKeyFunc_UnsupportResize);
            return;
    }

    if(sg_u8ResizeMode == 0)
        pstBasicVar->u8ResizeMode = emResize_None;
    else
        pstBasicVar->u8ResizeMode = u8ResizeStart+(sg_u8ResizeMode-1);

    SetUserData_SpecBasicVar();
    Dbg_UserFun(("mapp_SCU41MVMultiWinUpdate 333\r\n"));
    mapp_SCU41MVMultiWinUpdate(emSrcInputMax,emWinTypeMax,TRUE);
}

void SCU41_MV_SetSwapFunction(void)
{
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8InShift = _HDMI_INPUT_PORT_USED_NUMBER;
    u8 l_pu8temp[4];

    switch(pstBasicVar->u8WindowNum)
    {
        case emWinNum_2:
            {
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    switch(mapp_GetCfgMode())
                    {
                        case emCfgMode_Pixel:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinPixelCfg.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinPixelCfg.u8UserSrc[emWinType_B];

                                g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_A];
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinPixelCfg.u8UserSrc);
                            }
                            break;
                        default:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinsConfig.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinsConfig.u8UserSrc[emWinType_B];

                                g_stUserWinsConfig.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                g_stUserWinsConfig.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_A];
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinsConfig.u8UserSrc);
                            }
                            break;
                    }
                }
                else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                {
                    u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                    pstUserWindowsConfig_t pstUserWinCfg = &pstBasicVar->stUserWinsConfig[l_u8UserMode];

                    l_pu8temp[emWinType_A] = pstUserWinCfg->u8UserSrc[emWinType_A];
                    l_pu8temp[emWinType_B] = pstUserWinCfg->u8UserSrc[emWinType_B];

                    pstUserWinCfg->u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                    pstUserWinCfg->u8UserSrc[emWinType_B] = l_pu8temp[emWinType_A];
                    mapp_SCU41MVCopyUserModeSrcToNomalMode(pstUserWinCfg->u8UserSrc);
                }
                else
                {
                    l_pu8temp[emWinType_A] = pstBasicVar->u8WinSel[emWinType_A];
                    l_pu8temp[emWinType_B] = pstBasicVar->u8WinSel[emWinType_B];

                    pstBasicVar->u8WinSel[emWinType_A] = l_pu8temp[emWinType_B];
                    pstBasicVar->u8WinSel[emWinType_B] = l_pu8temp[emWinType_A];
                }
                SCU41_MV_SetDisplayWin(emWinTypeMax,emWinTypeMax,TRUE);
            }
            break;
        case emWinNum_3:
            {
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    switch(mapp_GetCfgMode())
                    {
                        case emCfgMode_Pixel:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinPixelCfg.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinPixelCfg.u8UserSrc[emWinType_B];
                                l_pu8temp[emWinType_C] = g_stUserWinPixelCfg.u8UserSrc[emWinType_C];

                                g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_A];
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinPixelCfg.u8UserSrc);
                            }
                            break;
                        default:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinsConfig.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinsConfig.u8UserSrc[emWinType_B];
                                l_pu8temp[emWinType_C] = g_stUserWinsConfig.u8UserSrc[emWinType_C];

                                g_stUserWinsConfig.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                g_stUserWinsConfig.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                                g_stUserWinsConfig.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_A];
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinsConfig.u8UserSrc);
                            }
                            break;
                    }
                }
                else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                {
                    u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                    pstUserWindowsConfig_t pstUserWinCfg = &pstBasicVar->stUserWinsConfig[l_u8UserMode];

                    l_pu8temp[emWinType_A] = pstUserWinCfg->u8UserSrc[emWinType_A];
                    l_pu8temp[emWinType_B] = pstUserWinCfg->u8UserSrc[emWinType_B];
                    l_pu8temp[emWinType_C] = pstUserWinCfg->u8UserSrc[emWinType_C];

                    pstUserWinCfg->u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                    pstUserWinCfg->u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                    pstUserWinCfg->u8UserSrc[emWinType_C] = l_pu8temp[emWinType_A];
                    mapp_SCU41MVCopyUserModeSrcToNomalMode(pstUserWinCfg->u8UserSrc);
                }
                else
                {
                    l_pu8temp[emWinType_A] = pstBasicVar->u8WinSel[emWinType_A];
                    l_pu8temp[emWinType_B] = pstBasicVar->u8WinSel[emWinType_B];
                    l_pu8temp[emWinType_C] = pstBasicVar->u8WinSel[emWinType_C];

                    pstBasicVar->u8WinSel[emWinType_A] = l_pu8temp[emWinType_B];
                    pstBasicVar->u8WinSel[emWinType_B] = l_pu8temp[emWinType_C];
                    pstBasicVar->u8WinSel[emWinType_C] = l_pu8temp[emWinType_A];
                }
                SCU41_MV_SetDisplayWin(emWinTypeMax,emWinTypeMax,TRUE);
            }
            break;
        case emWinNum_4:
            {
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    switch(mapp_GetCfgMode())
                    {
                        case emCfgMode_Pixel:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinPixelCfg.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinPixelCfg.u8UserSrc[emWinType_B];
                                l_pu8temp[emWinType_C] = g_stUserWinPixelCfg.u8UserSrc[emWinType_C];
                                l_pu8temp[emWinType_D] = g_stUserWinPixelCfg.u8UserSrc[emWinType_D];

                                if(pstBasicVar->u8MvMode == emMvMode_4Win_SameSize)
                                {
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_D];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_A];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_C];
                                }
                                else if((pstBasicVar->u8MvMode == emMvMode_4Win_PBP_3L1R)
                                        ||(pstBasicVar->u8MvMode == emMvMode_4Win_PBP_1U3D)
                                        ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3L)
                                        ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3D))
                                {
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_D];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_A];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_B];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_C];
                                }
                                else
                                {
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_D];
                                    g_stUserWinPixelCfg.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_A];
                                }
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinPixelCfg.u8UserSrc);
                            }
                            break;
                        default:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinsConfig.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinsConfig.u8UserSrc[emWinType_B];
                                l_pu8temp[emWinType_C] = g_stUserWinsConfig.u8UserSrc[emWinType_C];
                                l_pu8temp[emWinType_D] = g_stUserWinsConfig.u8UserSrc[emWinType_D];

                                if(pstBasicVar->u8MvMode == emMvMode_4Win_SameSize)
                                {
                                    g_stUserWinsConfig.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_D];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_A];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_C];
                                }
                                else if((pstBasicVar->u8MvMode == emMvMode_4Win_PBP_3L1R)
                                        ||(pstBasicVar->u8MvMode == emMvMode_4Win_PBP_1U3D)
                                        ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3L)
                                        ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3D))
                                {
                                    g_stUserWinsConfig.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_D];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_A];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_B];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_C];
                                }
                                else
                                {
                                    g_stUserWinsConfig.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_D];
                                    g_stUserWinsConfig.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_A];
                                }
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinsConfig.u8UserSrc);
                            }
                            break;
                    }
                }
                else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                {
                    u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                    pstUserWindowsConfig_t pstUserWinCfg = &pstBasicVar->stUserWinsConfig[l_u8UserMode];

                    l_pu8temp[emWinType_A] = pstUserWinCfg->u8UserSrc[emWinType_A];
                    l_pu8temp[emWinType_B] = pstUserWinCfg->u8UserSrc[emWinType_B];
                    l_pu8temp[emWinType_C] = pstUserWinCfg->u8UserSrc[emWinType_C];
                    l_pu8temp[emWinType_D] = pstUserWinCfg->u8UserSrc[emWinType_D];

                    if(pstBasicVar->u8MvMode == emMvMode_4Win_SameSize)
                    {
                        pstUserWinCfg->u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                        pstUserWinCfg->u8UserSrc[emWinType_B] = l_pu8temp[emWinType_D];
                        pstUserWinCfg->u8UserSrc[emWinType_C] = l_pu8temp[emWinType_A];
                        pstUserWinCfg->u8UserSrc[emWinType_D] = l_pu8temp[emWinType_C];
                    }
                    else if((pstBasicVar->u8MvMode == emMvMode_4Win_PBP_3L1R)
                            ||(pstBasicVar->u8MvMode == emMvMode_4Win_PBP_1U3D)
                            ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3L)
                            ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3D))
                    {
                        pstUserWinCfg->u8UserSrc[emWinType_A] = l_pu8temp[emWinType_D];
                        pstUserWinCfg->u8UserSrc[emWinType_B] = l_pu8temp[emWinType_A];
                        pstUserWinCfg->u8UserSrc[emWinType_C] = l_pu8temp[emWinType_B];
                        pstUserWinCfg->u8UserSrc[emWinType_D] = l_pu8temp[emWinType_C];
                    }
                    else
                    {
                        pstUserWinCfg->u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                        pstUserWinCfg->u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                        pstUserWinCfg->u8UserSrc[emWinType_C] = l_pu8temp[emWinType_D];
                        pstUserWinCfg->u8UserSrc[emWinType_D] = l_pu8temp[emWinType_A];
                    }
                    mapp_SCU41MVCopyUserModeSrcToNomalMode(pstUserWinCfg->u8UserSrc);
                }
                else
                {
                    l_pu8temp[emWinType_A] = pstBasicVar->u8WinSel[emWinType_A];
                    l_pu8temp[emWinType_B] = pstBasicVar->u8WinSel[emWinType_B];
                    l_pu8temp[emWinType_C] = pstBasicVar->u8WinSel[emWinType_C];
                    l_pu8temp[emWinType_D] = pstBasicVar->u8WinSel[emWinType_D];

                    if(pstBasicVar->u8MvMode == emMvMode_4Win_SameSize)
                    {
                        pstBasicVar->u8WinSel[emWinType_A] = l_pu8temp[emWinType_B];
                        pstBasicVar->u8WinSel[emWinType_B] = l_pu8temp[emWinType_D];
                        pstBasicVar->u8WinSel[emWinType_C] = l_pu8temp[emWinType_A];
                        pstBasicVar->u8WinSel[emWinType_D] = l_pu8temp[emWinType_C];
                    }
                    else if((pstBasicVar->u8MvMode == emMvMode_4Win_PBP_3L1R)
                            ||(pstBasicVar->u8MvMode == emMvMode_4Win_PBP_1U3D)
                            ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3L)
                            ||(pstBasicVar->u8MvMode == emMvMode_4Win_PIP_1F3D))
                    {
                        pstBasicVar->u8WinSel[emWinType_A] = l_pu8temp[emWinType_D];
                        pstBasicVar->u8WinSel[emWinType_B] = l_pu8temp[emWinType_A];
                        pstBasicVar->u8WinSel[emWinType_C] = l_pu8temp[emWinType_B];
                        pstBasicVar->u8WinSel[emWinType_D] = l_pu8temp[emWinType_C];
                    }
                    else
                    {
                        pstBasicVar->u8WinSel[emWinType_A] = l_pu8temp[emWinType_B];
                        pstBasicVar->u8WinSel[emWinType_B] = l_pu8temp[emWinType_C];
                        pstBasicVar->u8WinSel[emWinType_C] = l_pu8temp[emWinType_D];
                        pstBasicVar->u8WinSel[emWinType_D] = l_pu8temp[emWinType_A];
                    }
                }
                SCU41_MV_SetDisplayWin(emWinTypeMax,emWinTypeMax,TRUE);
            }
            break;
        case emWinNum_1:
            {
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    switch(mapp_GetCfgMode())
                    {
                        case emCfgMode_Pixel:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinPixelCfg.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinPixelCfg.u8UserSrc[emWinType_B];
                                l_pu8temp[emWinType_C] = g_stUserWinPixelCfg.u8UserSrc[emWinType_C];
                                l_pu8temp[emWinType_D] = g_stUserWinPixelCfg.u8UserSrc[emWinType_D];

                                g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_D];
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_A];
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinPixelCfg.u8UserSrc);
                            }
                            break;
                        default:
                            {
                                l_pu8temp[emWinType_A] = g_stUserWinsConfig.u8UserSrc[emWinType_A];
                                l_pu8temp[emWinType_B] = g_stUserWinsConfig.u8UserSrc[emWinType_B];
                                l_pu8temp[emWinType_C] = g_stUserWinsConfig.u8UserSrc[emWinType_C];
                                l_pu8temp[emWinType_D] = g_stUserWinsConfig.u8UserSrc[emWinType_D];

                                g_stUserWinsConfig.u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                                g_stUserWinsConfig.u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                                g_stUserWinsConfig.u8UserSrc[emWinType_C] = l_pu8temp[emWinType_D];
                                g_stUserWinsConfig.u8UserSrc[emWinType_D] = l_pu8temp[emWinType_A];
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinsConfig.u8UserSrc);
                            }
                            break;
                    }
                }
                else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                {
                    u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                    pstUserWindowsConfig_t pstUserWinCfg = &pstBasicVar->stUserWinsConfig[l_u8UserMode];
                    l_pu8temp[emWinType_A] = pstUserWinCfg->u8UserSrc[emWinType_A];
                    l_pu8temp[emWinType_B] = pstUserWinCfg->u8UserSrc[emWinType_B];
                    l_pu8temp[emWinType_C] = pstUserWinCfg->u8UserSrc[emWinType_C];
                    l_pu8temp[emWinType_D] = pstUserWinCfg->u8UserSrc[emWinType_D];

                    pstUserWinCfg->u8UserSrc[emWinType_A] = l_pu8temp[emWinType_B];
                    pstUserWinCfg->u8UserSrc[emWinType_B] = l_pu8temp[emWinType_C];
                    pstUserWinCfg->u8UserSrc[emWinType_C] = l_pu8temp[emWinType_D];
                    pstUserWinCfg->u8UserSrc[emWinType_D] = l_pu8temp[emWinType_A];
                    mapp_SCU41MVCopyUserModeSrcToNomalMode(pstUserWinCfg->u8UserSrc);
                }
                else
                {
                    l_pu8temp[emWinType_A] = pstBasicVar->u8WinSel[emWinType_A];
                    l_pu8temp[emWinType_B] = pstBasicVar->u8WinSel[emWinType_B];
                    l_pu8temp[emWinType_C] = pstBasicVar->u8WinSel[emWinType_C];
                    l_pu8temp[emWinType_D] = pstBasicVar->u8WinSel[emWinType_D];

                    pstBasicVar->u8WinSel[emWinType_A] = l_pu8temp[emWinType_B];
                    pstBasicVar->u8WinSel[emWinType_B] = l_pu8temp[emWinType_C];
                    pstBasicVar->u8WinSel[emWinType_C] = l_pu8temp[emWinType_D];
                    pstBasicVar->u8WinSel[emWinType_D] = l_pu8temp[emWinType_A];
                }
                #if _ENABLE_MV_SOURCE_SWAP
                SCU41_MV_SetDisplayWin(emWinTypeMax,emWinTypeMax,TRUE);
                #else
                SCU41_MV_SetDisplayWin(pstBasicVar->u8WinSel[emWinType_A],emWinType_A,TRUE);
                #endif
                mapp_SCU41MVSetAudioInput(pstBasicVar->u8WinSel[emWinType_A]);
                led_on(led_name_hdmi_input_blue_1+pstBasicVar->u8WinSel[emWinType_A]*2);
                led_off(led_name_hdmi_input_blue_1+((pstBasicVar->u8WinSel[emWinType_A]+1)%l_u8InShift)*2);
                led_off(led_name_hdmi_input_blue_1+((pstBasicVar->u8WinSel[emWinType_A]+2)%l_u8InShift)*2);
                led_off(led_name_hdmi_input_blue_1+((pstBasicVar->u8WinSel[emWinType_A]+3)%l_u8InShift)*2);
            }
            break;
        default:
            break;
    }
    SetUserData_SpecBasicVar();
}

/******************************************************
///Function Name:   SCU41_MV_SendDataToFpag
///Description:     Send data to FPGA
///Input:           pu8Data(数据),u16Len(长度)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void SCU41_MV_SendDataToFpag(pu8 pu8Data, u16 u16Len)
{
	pstUartCom_t pUartCommVar = &g_stUart3CommVar;

    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
}

/******************************************************
///Function Name:   SCU41_MV_SendCmd
///Description:     Send command to FPGA
///Input:           u8Addr(FPGA控制地址),u8Cmd(命令码),
///                 u16Len(数据长度),pu8Data(控制数据)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void SCU41_MV_SendCmd(u8 u8Addr, u8 u8Cmd, u16 u16Len, pu8 pu8Data)
{
    u8 l_u8TotalVal[256];

    memset(l_u8TotalVal,0,256);         // clear buffer

    memcpy(l_u8TotalVal,g_u8Head,3);    // head
    l_u8TotalVal[3] = u8Addr;           // addr
    l_u8TotalVal[4] = u8Cmd;            // cmd

    l_u8TotalVal[5] = (u16Len>>8)&0xFF; // data lenth
    l_u8TotalVal[6] = (u16Len)&0xFF;

    if(pu8Data!=NULL)
    {
        memcpy(l_u8TotalVal+7,pu8Data,u16Len);    // head
    }

    SCU41_MV_SendDataToFpag(l_u8TotalVal,u16Len+7);
}

void mapp_SCU41MVChangeTimingData(pstResolutionTable_t pstResTable,pstTimingConfig_t pstOutTiming)
{
    pstOutTiming->u8HFP_H      = (pstResTable->u16HFP>>8)&0xFF;
    pstOutTiming->u8HFP_L      = (pstResTable->u16HFP)&0xFF;
    pstOutTiming->u8HPol       = pstResTable->u8HPol;
    pstOutTiming->u8HSync      = pstResTable->u8HSync;
    pstOutTiming->u8HBP_H      = (pstResTable->u16HBP>>8)&0xFF;
    pstOutTiming->u8HBP_L      = (pstResTable->u16HBP)&0xFF;
    pstOutTiming->u8HActive_H  = (pstResTable->u16HActive>>8)&0xFF;
    pstOutTiming->u8HActive_L  = (pstResTable->u16HActive)&0xFF;
    pstOutTiming->u8HTotal_H   = (pstResTable->u16HTotal>>8)&0xFF;
    pstOutTiming->u8HTotal_L   = (pstResTable->u16HTotal)&0xFF;
    pstOutTiming->u8VFP_H      = (pstResTable->u16VFP>>8)&0xFF;
    pstOutTiming->u8VFP_L      = (pstResTable->u16VFP)&0xFF;
    pstOutTiming->u8VPol       = pstResTable->u8VPol;
    pstOutTiming->u8VSync      = pstResTable->u8VSync;
    pstOutTiming->u8VBP_H      = (pstResTable->u16VBP>>8)&0xFF;
    pstOutTiming->u8VBP_L      = (pstResTable->u16VBP)&0xFF;
    pstOutTiming->u8VActive_H  = (pstResTable->u16VActive>>8)&0xFF;
    pstOutTiming->u8VActive_L  = (pstResTable->u16VActive)&0xFF;
    pstOutTiming->u8VTotal_H   = (pstResTable->u16VTotal>>8)&0xFF;
    pstOutTiming->u8VTotal_L   = (pstResTable->u16VTotal)&0xFF;
    memcpy(&g_stTiming,pstResTable,sizeof(stResolutionTable_t));
}

#if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
/******************************************************
///Function Name:   mapp_SCU41MV_ReadEdidDescriptor1Timing
///Description:     Config output resolution
///Input:           pu8EdidBuf(Edid 数据)
///Output:          pstEdidRes(提取的EDID分辨率信息)
///Return:          NULL
///Others:          NULL
******************************************************/
void mapp_SCU41MV_ReadEdidDescriptor1Timing(pu8 pu8EdidBuf,pstResolutionTable_t pstEdidRes)
{
    u16 l_u16HBlank;
    u16 l_u16VBlank;

    l_u16HBlank = (pu8EdidBuf[0x39]|((pu8EdidBuf[0x3A]&0x0F)<<8));
    pstEdidRes->u8HPol  = (pu8EdidBuf[0x47]&BIT1)>>1;
    pstEdidRes->u16HFP  = (pu8EdidBuf[0x3E]|(((pu8EdidBuf[0x41]&0xC0)>>6)<<8));
    pstEdidRes->u8HSync = (pu8EdidBuf[0x3F]|(((pu8EdidBuf[0x41]&0x30)>>4)<<8));
    pstEdidRes->u16HBP  = l_u16HBlank-pstEdidRes->u8HSync-pstEdidRes->u16HFP;
    pstEdidRes->u16HActive  = (pu8EdidBuf[0x38]|((pu8EdidBuf[0x3A]>>4)<<8));
    pstEdidRes->u16HTotal   = pstEdidRes->u16HActive+l_u16HBlank;
    l_u16VBlank = (((pu8EdidBuf[0x3D]&0x0F)<<8)|(pu8EdidBuf[0x3C]));
    pstEdidRes->u8VPol  = (pu8EdidBuf[0x47]&BIT2)>>2;
    pstEdidRes->u16VFP  = (((pu8EdidBuf[0x40]&0xF0)>>4)|(((pu8EdidBuf[0x41]&0x0C)>>2)<<8));
    pstEdidRes->u8VSync = ((pu8EdidBuf[0x40]&0x0F)|((pu8EdidBuf[0x41]&0x03)<<8));
    pstEdidRes->u16VBP  = l_u16VBlank-pstEdidRes->u16VFP-pstEdidRes->u8VSync;
    pstEdidRes->u16VActive  = ((((pu8EdidBuf[0x3D]&0xF0)>>4)<<8)|(pu8EdidBuf[0x3B]));
    pstEdidRes->u16VTotal   = pstEdidRes->u16VActive + l_u16VBlank;
    pstEdidRes->u8Vic = 0;
    pstEdidRes->u16PClk = (pu8EdidBuf[0x36]|(pu8EdidBuf[0x37]<<8));
    pstEdidRes->u8VClk = pstEdidRes->u16PClk*10000/(pstEdidRes->u16HTotal)/(pstEdidRes->u16VTotal);

    #if 0
    Uart2_printf("u8HPol = %d\r\n",pstEdidRes->u8HPol);
    Uart2_printf("u16HFP = %d\r\n",pstEdidRes->u16HFP);
    Uart2_printf("u8HSync = %d\r\n",pstEdidRes->u8HSync);
    Uart2_printf("u16HBP = %d\r\n",pstEdidRes->u16HBP);
    Uart2_printf("u16HActive = %d\r\n",pstEdidRes->u16HActive);
    Uart2_printf("u16HTotal = %d\r\n",pstEdidRes->u16HTotal);
    Uart2_printf("u8VPol = %d\r\n",pstEdidRes->u8VPol);
    Uart2_printf("u16VFP = %d\r\n",pstEdidRes->u16VFP);
    Uart2_printf("u8VSync = %d\r\n",pstEdidRes->u8VSync);
    Uart2_printf("u16VBP = %d\r\n",pstEdidRes->u16VBP);
    Uart2_printf("u16VActive = %d\r\n",pstEdidRes->u16VActive);
    Uart2_printf("u16VTotal = %d\r\n",pstEdidRes->u16VTotal);
    Uart2_printf("u8Vic = %d\r\n",pstEdidRes->u8Vic);
    Uart2_printf("u8VClk = %d\r\n",pstEdidRes->u8VClk);
    Uart2_printf("u16PClk = %d\r\n",pstEdidRes->u16PClk);
    #endif
}
#endif

#define PCLK_SHIFT      2
/******************************************************
///Function Name:   mapp_SCU41MVResolutionSwitch
///Description:     Config output resolution
///Input:           u8ResId(分辨率ID)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void mapp_SCU41MVResolutionSwitch(u8 u8ResId)
{
    stTimingConfig_t l_stOutTiming;
    static bool sl_bFirstEnter = TRUE;

    switch(u8ResId)
    {
        case emRes_3840x2160_30:
        case emRes_1920x1080_60:
        case emRes_1920x1200_60:
        case emRes_1600x1200_60:
        case emRes_1360x768_60:
        case emRes_1280x720_60:
        case emRes_1024x768_60:
    #if _ENABLE_RES_1600x900_60
        case emRes_1600x900_60:
    #endif
    #if _ENABLE_RES_1600x900_60
        case emRes_1680x1050_60:
    #endif
    #if _ENABLE_RES_1400x1050_60
        case emRes_1400x1050_60:
    #endif
    #if _ENABLE_RES_1280x800_60
        case emRes_1280x800_60:
    #endif
    #if _ENABLE_RES_1280x720_50
        case emRes_1280x720_50:
    #endif
    #if _ENABLE_RES_1920x1080_50
        case emRes_1920x1080_50:
    #endif
            {
                mapp_SCU41MVChangeTimingData((pstResolutionTable_t)&TimingTable[u8ResId],&l_stOutTiming);
                SCU41_MV_SendCmd(0,FPGA_REG_RES,sizeof(stTimingConfig_t),(pu8)&(l_stOutTiming));
                delay_ms(1000);
                mapp_IDTSetPclk(0,(TimingTable[u8ResId].u16PClk)/PCLK_SHIFT,1);
                #if _ENABLE_CHIP_ITE6615
                mapp_Ite6615SetOutputVic((u16)TimingTable[u8ResId].u8Vic);
                if(sl_bFirstEnter == FALSE)
                {
                    //mapp_Ite6615SetAvMute(TRUE);
                    mapp_Ite6615SetPowerOffState(TRUE);
                }
                #endif
                //mapp_Ite6615SetSignalInputChange(TRUE);
            }
            break;

        #if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
        case emRes_BestByDisplay:
            {
                u8 l_pu8EdidBuf[256] = {0};
                stResolutionTable_t l_stEdidRes;
                bool l_bEdidSta;
                l_bEdidSta = mapp_EdidLimitTo4K30(g_pu8It6615TxEdidBuffer,l_pu8EdidBuf,256,FALSE,TRUE);

                if(l_bEdidSta == TRUE)
                {
                    mapp_SCU41MV_ReadEdidDescriptor1Timing(l_pu8EdidBuf,&l_stEdidRes);
                    mapp_SCU41MVChangeTimingData((pstResolutionTable_t)&l_stEdidRes,&l_stOutTiming);
                    SCU41_MV_SendCmd(0,FPGA_REG_RES,sizeof(stTimingConfig_t),(pu8)&(l_stOutTiming));
                    delay_ms(1000);
                    mapp_IDTSetPclk(0,(l_stEdidRes.u16PClk)/PCLK_SHIFT,1);
                    #if _ENABLE_CHIP_ITE6615
                    mapp_Ite6615SetOutputVic((u16)l_stEdidRes.u8Vic);
                    mapp_Ite6615SetPowerOffState(TRUE);
                    #endif
                }
                else
                {
                    mapp_SCU41MVChangeTimingData((pstResolutionTable_t)&TimingTable[u8ResId],&l_stOutTiming);
                    SCU41_MV_SendCmd(0,FPGA_REG_RES,sizeof(stTimingConfig_t),(pu8)&(l_stOutTiming));
                    delay_ms(1000);
                    mapp_IDTSetPclk(0,(TimingTable[u8ResId].u16PClk)/PCLK_SHIFT,1);
                    #if _ENABLE_CHIP_ITE6615
                    mapp_Ite6615SetOutputVic((u16)TimingTable[u8ResId].u8Vic);
                    if(sl_bFirstEnter == FALSE)
                        mapp_Ite6615SetPowerOffState(TRUE);
                    #endif
                }
            }
            break;
        #endif

        default:
            break;
    }

    if(sl_bFirstEnter == TRUE)
    {
        sl_bFirstEnter = FALSE;
        mapp_Ite6615SetSignalInputChange(TRUE);
    }
    else
    {
        #if 0//_ENABLE_SYSTEM_RESET_DELAY_TIME
            mapp_SetSysResetModeDelayTime(500);
        #else
            //delay_ms(5000);
            //mapp_SCU41MVMultiWinUpdate((u8)emSrcInputMax,emWinTypeMax,TRUE);
            mapp_SCU41MVSetWinCfgUpdateFlag(7000, TRUE);
        #endif
        //mapp_Ite6615SetSignalInputChange(TRUE);
    }
}

void mapp_SCU41MVFpgaWinCmdUpdate(void)
{
    u8 l_u8cmd = 0;
    SCU41_MV_SendCmd(0xFE,FPGA_REG_UPD,1,&l_u8cmd);
}

void mapp_SCU41MVSetWindows(u8 u8Addr,pstWindowsConfig_t pstWinConfigVar)
{
    u8 l_u8WinCmd[34];

    l_u8WinCmd[WIN_Src]         = pstWinConfigVar->u8Src;
    l_u8WinCmd[WIN_Port]        = pstWinConfigVar->u8Port;
    l_u8WinCmd[WIN_Layer_D0]    = pstWinConfigVar->u8Layer_D0;
    l_u8WinCmd[WIN_Layer_D1]    = pstWinConfigVar->u8Layer_D1;
    l_u8WinCmd[WIN_Layer_D2]    = pstWinConfigVar->u8Layer_D2;
    l_u8WinCmd[WIN_Layer_D3]    = pstWinConfigVar->u8Layer_D3;
    l_u8WinCmd[WIN_HSrc_H]      = (pstWinConfigVar->u16Src_H>>8)&0xFF;
    l_u8WinCmd[WIN_HSrc_L]      = (pstWinConfigVar->u16Src_H)&0xFF;
    l_u8WinCmd[WIN_VSrc_H]      = (pstWinConfigVar->u16Src_V>>8)&0xFF;
    l_u8WinCmd[WIN_VSrc_L]      = (pstWinConfigVar->u16Src_V)&0xFF;
    l_u8WinCmd[WIN_XSrc_H]      = (pstWinConfigVar->u16Src_X>>8)&0xFF;
    l_u8WinCmd[WIN_XSrc_L]      = (pstWinConfigVar->u16Src_X)&0xFF;
    l_u8WinCmd[WIN_YSrc_H]      = (pstWinConfigVar->u16Src_Y>>8)&0xFF;
    l_u8WinCmd[WIN_YSrc_L]      = (pstWinConfigVar->u16Src_Y)&0xFF;
    l_u8WinCmd[WIN_HSrcClip_H]  = (pstWinConfigVar->u16SrcClip_H>>8)&0xFF;
    l_u8WinCmd[WIN_HSrcClip_L]  = (pstWinConfigVar->u16SrcClip_H)&0xFF;
    l_u8WinCmd[WIN_VSrcClip_H]  = (pstWinConfigVar->u16SrcClip_V>>8)&0xFF;
    l_u8WinCmd[WIN_VSrcClip_L]  = (pstWinConfigVar->u16SrcClip_V)&0xFF;
    l_u8WinCmd[WIN_XDisplay_H]  = (pstWinConfigVar->u16Display_X>>8)&0xFF;
    l_u8WinCmd[WIN_XDisplay_L]  = (pstWinConfigVar->u16Display_X)&0xFF;
    l_u8WinCmd[WIN_YDisplay_H]  = (pstWinConfigVar->u16Display_Y>>8)&0xFF;
    l_u8WinCmd[WIN_YDisplay_L]  = (pstWinConfigVar->u16Display_Y)&0xFF;
    l_u8WinCmd[WIN_HDisplay_H]  = (pstWinConfigVar->u16Display_H>>8)&0xFF;
    l_u8WinCmd[WIN_HDisplay_L]  = (pstWinConfigVar->u16Display_H)&0xFF;
    l_u8WinCmd[WIN_VDisplay_H]  = (pstWinConfigVar->u16Display_V>>8)&0xFF;
    l_u8WinCmd[WIN_VDisplay_L]  = (pstWinConfigVar->u16Display_V)&0xFF;

    l_u8WinCmd[WIN_HScaler_1]   = (pstWinConfigVar->u32Scaler_H>>24)&0xFF;
    l_u8WinCmd[WIN_HScaler_2]   = (pstWinConfigVar->u32Scaler_H>>16)&0xFF;
    l_u8WinCmd[WIN_HScaler_3]   = (pstWinConfigVar->u32Scaler_H>>8)&0xFF;
    l_u8WinCmd[WIN_HScaler_4]   = (pstWinConfigVar->u32Scaler_H)&0xFF;
    l_u8WinCmd[WIN_VScaler_1]   = (pstWinConfigVar->u32Scaler_V>>24)&0xFF;
    l_u8WinCmd[WIN_VScaler_2]   = (pstWinConfigVar->u32Scaler_V>>16)&0xFF;
    l_u8WinCmd[WIN_VScaler_3]   = (pstWinConfigVar->u32Scaler_V>>8)&0xFF;
    l_u8WinCmd[WIN_VScaler_4]   = (pstWinConfigVar->u32Scaler_V)&0xFF;

    SCU41_MV_SendCmd(u8Addr,FPGA_REG_WIN,34,(pu8)&(l_u8WinCmd));
}

/*========================================================
**切换函数的动作实际上是将窗口的顺序调换，即切换窗口，并不
**是切换当前窗口的源
========================================================*/
void mapp_SCU41MVGetLayerCfg(u8 u8Src,u8 u8WinType,bool bEnWin,pstWindowsConfig_t pstWinCfg)
{
    #if 1
    if(bEnWin)
    {
        switch(u8Src)
        {
            case emSrcInput_Hdmi1:
                {
                    g_u8WinLayer[0] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    g_u8WinLayer[0] = g_u8WinLayer[0]|0x80;
                }
                break;
            case emSrcInput_Hdmi2:
                {
                    g_u8WinLayer[1] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    g_u8WinLayer[1] = g_u8WinLayer[1]|0x80;
                }
                break;
            case emSrcInput_Hdmi3:
                {
                    g_u8WinLayer[2] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    g_u8WinLayer[2] = g_u8WinLayer[2]|0x80;
                }
                break;
            case emSrcInput_Hdmi4:
                {
                    g_u8WinLayer[3] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    g_u8WinLayer[3] = g_u8WinLayer[3]|0x80;
                }
                break;

            default:
                break;
        }
    }
    else
    {
        switch(u8Src)
        {
            case emSrcInput_Hdmi1:
                {
                    //g_u8WinLayer[0] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    //g_u8WinLayer[0] = g_u8WinLayer[0]&0x7F;
                    g_u8WinLayer[0] = 0;
                }
                break;
            case emSrcInput_Hdmi2:
                {
                    //g_u8WinLayer[1] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    //g_u8WinLayer[1] = g_u8WinLayer[1]&0x7F;
                    g_u8WinLayer[1] = 0;
                }
                break;
            case emSrcInput_Hdmi3:
                {
                    //g_u8WinLayer[2] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    //g_u8WinLayer[2] = g_u8WinLayer[2]&0x7F;
                    g_u8WinLayer[2] = 0;
                }
                break;
            case emSrcInput_Hdmi4:
                {
                    //g_u8WinLayer[3] = g_pu8WinLayerLevel[g_u8WinLevel][u8WinType];
                    //g_u8WinLayer[3] = g_u8WinLayer[3]&0x7F;
                    g_u8WinLayer[3] = 0;
                }
                break;

            default:
                break;
        }
    }
#endif
    pstWinCfg->u8Layer_D0 = g_u8WinLayer[0];
    pstWinCfg->u8Layer_D1 = g_u8WinLayer[1];
    pstWinCfg->u8Layer_D2 = g_u8WinLayer[2];
    pstWinCfg->u8Layer_D3 = g_u8WinLayer[3];
}

#if 0
u16 mapp_SCU41MVSrcClipStartXLeft(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = pstClipWin->u16StartX+pstSrcWin->u16StartX;
    }
    else
    {
        l_u16Retvale = pstSrcWin->u16StartX;
    }

    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipStartYLeft(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = pstSrcWin->u16StartY+pstClipWin->u16StartY;
    }
    else
    {
        l_u16Retvale = pstClipWin->u16StartY;
    }
    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipWidthLeft(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = ((pstClipWin->u16StartX+pstClipWin->u16Width)>(pstSrcWin->u16Width>>1))?(pstSrcWin->u16Width>>1) : pstClipWin->u16Width;
    }
    else
    {
        l_u16Retvale = (pstSrcWin->u16Width>>1);
    }
    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipHeigtLeft(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = pstClipWin->u16Height;
    }
    else
    {
        l_u16Retvale = pstSrcWin->u16Height;
    }
    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipStartXRight(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = pstClipWin->u16StartX;
    }
    else
    {
        l_u16Retvale = pstSrcWin->u16StartX;
    }

    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipStartYRight(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = pstClipWin->u16StartY;
    }
    else
    {
        l_u16Retvale = pstSrcWin->u16StartY;
    }
    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipWidthRight(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = ((pstClipWin->u16StartX+pstClipWin->u16Width)>(pstSrcWin->u16Width>>1))
            ?(pstClipWin->u16StartX+pstClipWin->u16Width)-(pstSrcWin->u16Width>>1)
            :0;
    }
    else
    {
        l_u16Retvale = pstSrcWin->u16Width>>1;
    }
    return l_u16Retvale;
}

u16 mapp_SCU41MVSrcClipHeigtRight(pstWindowType_t pstSrcWin, pstWindowType_t pstClipWin,bool bEnClip)
{
    u16 l_u16Retvale;

    if(bEnClip)
    {
        l_u16Retvale = pstClipWin->u16Height;
    }
    else
    {
        l_u16Retvale = pstSrcWin->u16Height;
    }
    return l_u16Retvale;
}
#endif

/******************************************************
///Function Name:   mapp_SCU41MVWindowConfigure
///Description:     Send windows config data to FPGA
///Input:           u8SrcPort(输入源),u8WinNum(第几个窗口),
///                 pstSrcWin(输入源信息),pstDstWin(开窗信息)
///                 pstClipWin(信号裁剪信息),bEnClip(使能裁剪),
///                 bEnWin(使能窗口)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
#if 0
void mapp_SCU41MVWindowConfigure(u8 u8SrcPort,
                               u8 u8WinNum,
                               pstWindowType_t pstSrcWin,
                               pstWindowType_t pstDstWin,
                               pstWindowType_t pstClipWin,
                               bool bEnClip,
                               bool bEnWin)
{
    stWindowsConfig_t stWinConfig;

    stWinConfig.u8Src           = 0x00;
    #if 0
    if(!bEnWin)
        stWinConfig.u8Port          = 0;//u8SrcPort+1+_HDMI_INPUT_PORT_USED_NUMBER;
    else
    #endif
        stWinConfig.u8Port          = u8SrcPort+1;
    mapp_SCU41MVGetLayerCfg(u8SrcPort,u8WinNum,bEnWin,&stWinConfig);
    //bEnClip = TRUE;
    stWinConfig.u16Src_H        = pstSrcWin->u16Width>>1;
    stWinConfig.u16Src_V        = pstSrcWin->u16Height;
    //stWinConfig.u16Src_X        = pstSrcWin->u16StartX;
    //stWinConfig.u16Src_Y        = pstSrcWin->u16StartY;
    stWinConfig.u16Src_X        = mapp_SCU41MVSrcClipStartXLeft(pstSrcWin,pstClipWin,bEnClip);
    stWinConfig.u16Src_Y        = mapp_SCU41MVSrcClipStartYLeft(pstSrcWin,pstClipWin,bEnClip);

    stWinConfig.u16SrcClip_H    = mapp_SCU41MVSrcClipWidthLeft(pstSrcWin,pstClipWin,bEnClip);
    stWinConfig.u16SrcClip_V    = mapp_SCU41MVSrcClipHeigtLeft(pstSrcWin,pstClipWin,bEnClip);

    stWinConfig.u16Display_H    = pstDstWin->u16Width>>1;
    stWinConfig.u16Display_V    = pstDstWin->u16Height;
    stWinConfig.u16Display_X    = pstDstWin->u16StartX;
    stWinConfig.u16Display_Y    = pstDstWin->u16StartY;

    //stWinConfig.u32Scaler_H     = ((u32)stWinConfig.u16Src_H<<11)/stWinConfig.u16Display_H;
    //stWinConfig.u32Scaler_V     = ((u32)stWinConfig.u16Src_V<<11)/stWinConfig.u16Display_V;
    stWinConfig.u32Scaler_H     = ((u32)stWinConfig.u16SrcClip_H<<11)/stWinConfig.u16Display_H;
    stWinConfig.u32Scaler_V     = ((u32)stWinConfig.u16SrcClip_V<<11)/stWinConfig.u16Display_V;
    mapp_SCU41MVSetWindows(0x01,&stWinConfig);
    //stWinConfig.u16Src_X        = pstSrcWin->u16StartX;//+(pstSrcWin->u16Width>>1);
    //stWinConfig.u16Src_Y        = pstSrcWin->u16StartY;

    stWinConfig.u16Src_X        = mapp_SCU41MVSrcClipStartXRight(pstSrcWin,pstClipWin,bEnClip);
    stWinConfig.u16Src_Y        = mapp_SCU41MVSrcClipStartYRight(pstSrcWin,pstClipWin,bEnClip);

    stWinConfig.u16SrcClip_H    = mapp_SCU41MVSrcClipWidthRight(pstSrcWin,pstClipWin,bEnClip);
    stWinConfig.u16SrcClip_V    = mapp_SCU41MVSrcClipHeigtRight(pstSrcWin,pstClipWin,bEnClip);
    stWinConfig.u16Display_X    = pstDstWin->u16StartX+(pstDstWin->u16Width>>1);
    stWinConfig.u16Display_Y    = pstDstWin->u16StartY;
    mapp_SCU41MVSetWindows(0x02,&stWinConfig);
}
#else
void mapp_SCU41MVWindowConfigure(u8 u8SrcPort,
                               u8 u8WinNum,
                               pstWindowType_t pstSrcWin,
                               pstWindowType_t pstDstWin,
                               pstWindowType_t pstClipWin,
                               bool bEnClip,
                               bool bEnWin)
{
    stWindowsConfig_t stWinConfig;
    u32 l_u32Hsf=0;
    u32 l_u32Vsf=0;
    u32 l_u32ResShift_H=0;
    u32 l_u32ResShift_V=0;
    u16 l_u16Px0=0,l_u16Px1=0;
    u16 l_u16Py0=0,l_u16Py1=0;
    u16 l_u16SrcX0=0,l_u16SrcX1=0;
    u16 l_u16SrcY0=0,l_u16SrcY1=0;
    u16 l_u16rHsrc=0,l_u16rVsrc=0;

    #if 0
    if(pstSrcWin->u16Width>=3840&&pstSrcWin->u16Height>=2160)
    {
        if(pstClipWin->u16StartX==0
            &&pstClipWin->u16StartY==0
            &&pstClipWin->u16Width==0
            &&pstClipWin->u16Height==0)
        {
            pstClipWin->u16StartX = 2;
            pstClipWin->u16Width = 4;
        }
    }
    #endif

    stWinConfig.u8Src           = 0x00;
    #if 0
    if(!bEnWin)
        stWinConfig.u8Port          = 0;//u8SrcPort+1+_HDMI_INPUT_PORT_USED_NUMBER;
    else
    #endif
        stWinConfig.u8Port          = u8SrcPort+1;
    mapp_SCU41MVGetLayerCfg(u8SrcPort,u8WinNum,bEnWin,&stWinConfig);

    stWinConfig.u16Src_H        = pstSrcWin->u16Width>>1;
    stWinConfig.u16Src_V        = pstSrcWin->u16Height;

    l_u16rHsrc = pstSrcWin->u16Width-pstClipWin->u16Width;
    l_u16rVsrc = pstSrcWin->u16Height-pstClipWin->u16Height;

    l_u32Hsf = ((u32)(l_u16rHsrc)<<11)/pstDstWin->u16Width;
    l_u32Vsf = ((u32)(l_u16rVsrc)<<11)/pstDstWin->u16Height;

    l_u16Px0 = 0;
    if(pstDstWin->u16Width)
        l_u16Px1 = (pstDstWin->u16Width>>1)-1;
    else
        l_u16Px1 = 0;
    l_u16Py0 = 0;
    if(pstDstWin->u16Height)
        l_u16Py1 = pstDstWin->u16Height-1;
    else
        l_u16Py1 = 0;

    l_u16SrcX0 = ((u32)l_u16Px0*l_u32Hsf)>>11;
    l_u16SrcY0 = ((u32)l_u16Py0*l_u32Vsf)>>11;
    l_u16SrcX1 = ((u32)l_u16Px1*l_u32Hsf)>>11;
    l_u16SrcY1 = ((u32)l_u16Py1*l_u32Vsf)>>11;

    l_u32ResShift_H = (l_u16Px0*l_u32Hsf)%2048;
    l_u32ResShift_V = (l_u16Py0*l_u32Vsf)%2048;

    //%对起点进行边界延伸并保证起点坐标为偶数
    if(l_u16SrcX0%2)        //%若起点坐标为奇数则向左延伸一列像素
    {
        l_u16SrcX0 -= 1;
        l_u32ResShift_H += 2048;
    }

    //%结束点坐标考虑边界延伸并确保对输入裁剪的宽度是偶数。
    l_u16SrcX1 += 1;        //%结束点坐标先向右延伸一列像素

    if(l_u16SrcX1%2==0)     //如果结束点水平坐标为偶数
    {
        l_u16SrcX1 += 1;
    }

    if(l_u16SrcX1 > (l_u16rHsrc-1))        //防止结束点水平方向坐标超出信号源边界
    {
        l_u16SrcX1 = (l_u16rHsrc-1);
    }

    l_u16SrcY1 += 1;

    if(l_u16SrcY1 > (l_u16rVsrc-1))
    {
        l_u16SrcY1 = (l_u16rVsrc-1);
    }

    stWinConfig.u16Src_X        = l_u16SrcX0+pstClipWin->u16StartX;
    stWinConfig.u16Src_Y        = l_u16SrcY0+pstClipWin->u16StartY;

    #if 0
    if(((pstDstWin->u16Width>=3840)&&(pstDstWin->u16Height>=2160))
        &&((pstSrcWin->u16Width<3840)&&(pstSrcWin->u16Height<2160)))
    {
        stWinConfig.u16SrcClip_H        = l_u16SrcX1-l_u16SrcX0+1+2;
    }
    else
    #endif
    {
        stWinConfig.u16SrcClip_H        = l_u16SrcX1-l_u16SrcX0+1;
    }
    stWinConfig.u16SrcClip_V        = l_u16SrcY1-l_u16SrcY0+1;

    stWinConfig.u16Display_H    = (pstDstWin->u16Width>>1);
    stWinConfig.u16Display_V    = (pstDstWin->u16Height);
    stWinConfig.u16Display_X    = pstDstWin->u16StartX;
    stWinConfig.u16Display_Y    = pstDstWin->u16StartY;

    stWinConfig.u32Scaler_H     = (l_u32ResShift_H<<16)|l_u32Hsf;
    stWinConfig.u32Scaler_V     = (l_u32ResShift_V<<16)|l_u32Vsf;

    mapp_SCU41MVSetWindows(0x01,&stWinConfig);

    l_u32Hsf = ((u32)(l_u16rHsrc)<<11)/pstDstWin->u16Width;
    l_u32Vsf = ((u32)(l_u16rVsrc)<<11)/pstDstWin->u16Height;

    l_u16Px0 = pstDstWin->u16Width>>1;
    if(pstDstWin->u16Width)
        l_u16Px1 = (pstDstWin->u16Width)-1;
    else
        l_u16Px1 = 0;

    l_u16Py0 = 0;
    if(pstDstWin->u16Height)
        l_u16Py1 = pstDstWin->u16Height-1;
    else
        l_u16Py1 = 0;

    l_u16SrcX0 = ((u32)l_u16Px0*l_u32Hsf)>>11;
    l_u16SrcY0 = ((u32)l_u16Py0*l_u32Vsf)>>11;
    l_u16SrcX1 = ((u32)l_u16Px1*l_u32Hsf)>>11;
    l_u16SrcY1 = ((u32)l_u16Py1*l_u32Vsf)>>11;

    l_u32ResShift_H = (l_u16Px0*l_u32Hsf)%2048;
    l_u32ResShift_V = (l_u16Py0*l_u32Vsf)%2048;
    if(l_u16SrcX0%2)
    {
        l_u16SrcX0 -= 1;
        l_u32ResShift_H += 2048;
    }

    //%结束点坐标考虑边界延伸并确保对输入裁剪的宽度是偶数。
    l_u16SrcX1 += 1;        //%结束点坐标先向右延伸一列像素

    if(l_u16SrcX1%2==0)
    {
        l_u16SrcX1 += 1;
    }

    if(l_u16SrcX1 > (l_u16rHsrc-1))
    {
        l_u16SrcX1 = (l_u16rHsrc-1);
    }

    l_u16SrcY1 += 1;

    if(l_u16SrcY1 > (l_u16rVsrc-1))
    {
        l_u16SrcY1 = (l_u16rVsrc-1);
    }

    stWinConfig.u16Src_X        = l_u16SrcX0+pstClipWin->u16StartX;
    stWinConfig.u16Src_Y        = l_u16SrcY0+pstClipWin->u16StartY;

    stWinConfig.u16SrcClip_H    = l_u16SrcX1-l_u16SrcX0+1;
    stWinConfig.u16SrcClip_V    = l_u16SrcY1-l_u16SrcY0+1;

    stWinConfig.u16Display_H    = (pstDstWin->u16Width>>1);
    stWinConfig.u16Display_V    = (pstDstWin->u16Height);

    stWinConfig.u16Display_X    = pstDstWin->u16StartX+(pstDstWin->u16Width>>1);
    stWinConfig.u16Display_Y    = pstDstWin->u16StartY;

    stWinConfig.u32Scaler_H     = (l_u32ResShift_H<<16)|l_u32Hsf;
    stWinConfig.u32Scaler_V     = (l_u32ResShift_V<<16)|l_u32Vsf;
    mapp_SCU41MVSetWindows(0x02,&stWinConfig);
}
#endif

u16 mapp_SCU41MVGetMultiWinStartX(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf1*g_stTiming.u16HActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf1*g_stTiming.u16HActive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8StX_sf2);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinStartY(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf1*g_stTiming.u16VActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf1*g_stTiming.u16VActive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8StY_sf2);
    }

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }
    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinWidth(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf1*g_stTiming.u16HActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf1*g_stTiming.u16HActive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8Width_sf2);
    }


    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
        //l_u16Temp += 4-(l_u16Temp%4);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinHeight(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf2==0)
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf1*g_stTiming.u16VActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf1*g_stTiming.u16VActive)
                     /g_pu8MultiWinTable[u8Type][u8DispWin].u8Height_sf2);
    }

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinUserModeStartX(u8 u8UserType, BYTE u8DispWin)
{
    u16 l_u16Temp;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    l_u16Temp = (((pstBasicVar->stUserWinsConfig[u8UserType].u8StartX[u8DispWin])*g_stTiming.u16HActive)
                /100);

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinUserModeStartY(u8 u8UserType, BYTE u8DispWin)
{
    u16 l_u16Temp;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    l_u16Temp = (((pstBasicVar->stUserWinsConfig[u8UserType].u8StartY[u8DispWin])*g_stTiming.u16VActive)
                /100);

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinUserModeWidth(u8 u8UserType, BYTE u8DispWin)
{
    u16 l_u16Temp;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    l_u16Temp = ((abs(pstBasicVar->stUserWinsConfig[u8UserType].u8EndX[u8DispWin]
                -pstBasicVar->stUserWinsConfig[u8UserType].u8StartX[u8DispWin])*g_stTiming.u16HActive)
                /100);

    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
        //l_u16Temp += 4-(l_u16Temp%4);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetMultiWinUserModeHeight(u8 u8UserType, BYTE u8DispWin)
{
    u16 l_u16Temp;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    l_u16Temp = ((abs(pstBasicVar->stUserWinsConfig[u8UserType].u8EndY[u8DispWin]
                -pstBasicVar->stUserWinsConfig[u8UserType].u8StartY[u8DispWin])*g_stTiming.u16VActive)
                /100);

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetResizeWinStartX(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_puResizeModeTable[u8Type][u8DispWin].u8StX_sf2==0)
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8StX_sf1*g_stTiming.u16HActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8StX_sf1*g_stTiming.u16HActive)
                     /g_puResizeModeTable[u8Type][u8DispWin].u8StX_sf2);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetResizeWinStartY(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_puResizeModeTable[u8Type][u8DispWin].u8StY_sf2==0)
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8StY_sf1*g_stTiming.u16VActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8StY_sf1*g_stTiming.u16VActive)
                     /g_puResizeModeTable[u8Type][u8DispWin].u8StY_sf2);
    }
    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetResizeWinWidth(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_puResizeModeTable[u8Type][u8DispWin].u8Width_sf2==0)
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8Width_sf1*g_stTiming.u16HActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8Width_sf1*g_stTiming.u16HActive)
                     /g_puResizeModeTable[u8Type][u8DispWin].u8Width_sf2);
    }

    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
        //l_u16Temp += 4-(l_u16Temp%4);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetResizeWinHeight(u8 u8Type, BYTE u8DispWin)
{
    u16 l_u16Temp;

    if(g_puResizeModeTable[u8Type][u8DispWin].u8Height_sf2==0)
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8Height_sf1*g_stTiming.u16VActive)
                     /100);
    }
    else
    {
        l_u16Temp = ((g_puResizeModeTable[u8Type][u8DispWin].u8Height_sf1*g_stTiming.u16VActive)
                     /g_puResizeModeTable[u8Type][u8DispWin].u8Height_sf2);
    }
    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserConfigWinStartX(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = (((g_stUserWinsConfig.u8StartX[u8DispWin])*g_stTiming.u16HActive)
                /100);

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserConfigWinStartY(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = (((g_stUserWinsConfig.u8StartY[u8DispWin])*g_stTiming.u16VActive)
                /100);

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }
    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserConfigWinWidth(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = ((abs(g_stUserWinsConfig.u8EndX[u8DispWin]
                -g_stUserWinsConfig.u8StartX[u8DispWin])*g_stTiming.u16HActive)
                /100);

    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
        //l_u16Temp += 4-(l_u16Temp%4);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserConfigWinHeight(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = ((abs(g_stUserWinsConfig.u8EndY[u8DispWin]
                -g_stUserWinsConfig.u8StartY[u8DispWin])*g_stTiming.u16VActive)
                /100);

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserWinPixelCfgStartX(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = g_stUserWinPixelCfg.u16StartX[u8DispWin];

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserWinPixelCfgStartY(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = g_stUserWinPixelCfg.u16StartY[u8DispWin];

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserWinPixelCfgWidth(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = g_stUserWinPixelCfg.u16Width[u8DispWin];

    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
        //l_u16Temp += 4-(l_u16Temp%4);
    }

    if(l_u16Temp > g_stTiming.u16HActive)
    {
        l_u16Temp = g_stTiming.u16HActive;
    }

    return l_u16Temp;
}

u16 mapp_SCU41MVGetUserWinPixelCfgHeight(BYTE u8DispWin)
{
    u16 l_u16Temp;

    l_u16Temp = g_stUserWinPixelCfg.u16Heigh[u8DispWin];

    if(l_u16Temp > g_stTiming.u16VActive)
    {
        l_u16Temp = g_stTiming.u16VActive;
    }

    return l_u16Temp;
}

bool mapp_SCU41MVPixelOutOfRange(u16 u16StartX,u16 u16StartY,u16 u16Width,u16 u16Height)
{
    if(((u16StartX+u16Width) > g_stTiming.u16HActive)
        ||((u16StartY+u16Height) > g_stTiming.u16VActive))
    {
        return TRUE;
    }
    return FALSE;
}

bool mapp_SCU41MVGetWindowSize(u8 u8Type, BYTE u8DispWin, pstWindowType_t pstDispWin)
{
    bool l_bSta = TRUE;
    u16 l_u16StartX;
    u16 l_u16StartY;
    u16 l_u16Width;
    u16 l_u16Height;

    switch(u8Type)
    {
        case emMvMode_1Win_Full:
        case emMvMode_2Win_PBP:
        case emMvMode_3Win_2U1D:
        case emMvMode_4Win_SameSize:
        case emMvMode_2Win_PIP_LU:
        case emMvMode_2Win_PIP_LD:
        case emMvMode_2Win_PIP_RU:
        case emMvMode_2Win_PIP_RD:
        case emMvMode_4Win_PBP_3L1R:
        case emMvMode_4Win_PBP_1L3R:
        case emMvMode_4Win_PBP_3U1D:
        case emMvMode_4Win_PBP_1U3D:
        case emMvMode_4Win_PIP_1F3L:
        case emMvMode_4Win_PIP_1F3R:
        case emMvMode_4Win_PIP_1F3U:
        case emMvMode_4Win_PIP_1F3D:
            {
                l_u16StartX   = mapp_SCU41MVGetMultiWinStartX(u8Type,u8DispWin);
                l_u16StartY   = mapp_SCU41MVGetMultiWinStartY(u8Type,u8DispWin);
                l_u16Width    = mapp_SCU41MVGetMultiWinWidth(u8Type,u8DispWin);
                l_u16Height   = mapp_SCU41MVGetMultiWinHeight(u8Type,u8DispWin);
            }
            break;
        case emMvMode_xWin_ByUser1:
        case emMvMode_xWin_ByUser2:
        case emMvMode_xWin_ByUser3:
        case emMvMode_xWin_ByUser4:
            {
                l_u16StartX   = mapp_SCU41MVGetMultiWinUserModeStartX((u8)(u8Type-emMvMode_xWin_ByUser1),u8DispWin);
                l_u16StartY   = mapp_SCU41MVGetMultiWinUserModeStartY((u8)(u8Type-emMvMode_xWin_ByUser1),u8DispWin);
                l_u16Width    = mapp_SCU41MVGetMultiWinUserModeWidth((u8)(u8Type-emMvMode_xWin_ByUser1),u8DispWin);
                l_u16Height   = mapp_SCU41MVGetMultiWinUserModeHeight((u8)(u8Type-emMvMode_xWin_ByUser1),u8DispWin);
            }
            break;

        default:
            l_u16StartX   = 0;
            l_u16StartY   = 0;
            l_u16Width    = 0;
            l_u16Height   = 0;
            break;
    }

    if(mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_bSta = FALSE;
    }
    else
    {
        pstDispWin->u16StartX   = l_u16StartX;
        pstDispWin->u16StartY   = l_u16StartY;
        pstDispWin->u16Width    = l_u16Width;
        pstDispWin->u16Height   = l_u16Height;
    }

    return l_bSta;
}

bool mapp_SCU41MVGetResize(u8 u8SubType, BYTE u8DispWin, pstWindowType_t pstDispWin)
{
    bool l_bSta = TRUE;
    u16 l_u16StartX;
    u16 l_u16StartY;
    u16 l_u16Width;
    u16 l_u16Height;

    switch(u8SubType)
    {
        case emResize_2Win_PBP_1:
        case emResize_2Win_PBP_2:
        case emResize_2Win_PBP_3:
        case emResize_2Win_PIP_LU_1:
        case emResize_2Win_PIP_LU_2:
        case emResize_2Win_PIP_LD_1:
        case emResize_2Win_PIP_LD_2:
        case emResize_2Win_PIP_RU_1:
        case emResize_2Win_PIP_RU_2:
        case emResize_2Win_PIP_RD_1:
        case emResize_2Win_PIP_RD_2:
        case emResize_4Win_PBP_3L1R_1:
        case emResize_4Win_PBP_3L1R_2:
        case emResize_4Win_PBP_1L3R_1:
        case emResize_4Win_PBP_1L3R_2:
        case emResize_4Win_PBP_3U1D_1:
        case emResize_4Win_PBP_3U1D_2:
        case emResize_4Win_PBP_1U3D_1:
        case emResize_4Win_PBP_1U3D_2:
            {
                l_u16StartX   = mapp_SCU41MVGetResizeWinStartX(u8SubType,u8DispWin);
                l_u16StartY   = mapp_SCU41MVGetResizeWinStartY(u8SubType,u8DispWin);
                l_u16Width    = mapp_SCU41MVGetResizeWinWidth(u8SubType,u8DispWin);
                l_u16Height   = mapp_SCU41MVGetResizeWinHeight(u8SubType,u8DispWin);
            }
            break;

        default:
            l_u16StartX   = 0;
            l_u16StartY   = 0;
            l_u16Width    = 0;
            l_u16Height   = 0;
            break;
    }

    if(mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_bSta = FALSE;
    }
    else
    {
        pstDispWin->u16StartX   = l_u16StartX;
        pstDispWin->u16StartY   = l_u16StartY;
        pstDispWin->u16Width    = l_u16Width;
        pstDispWin->u16Height   = l_u16Height;
    }

    return l_bSta;

}

bool mapp_SCU41MVGetCfgSize( BYTE u8DispWin, pstWindowType_t pstDispWin)
{
    bool l_bSta = TRUE;
    u16 l_u16StartX;
    u16 l_u16StartY;
    u16 l_u16Width;
    u16 l_u16Height;

    l_u16StartX   = mapp_SCU41MVGetUserConfigWinStartX(u8DispWin);
    l_u16StartY   = mapp_SCU41MVGetUserConfigWinStartY(u8DispWin);
    l_u16Width    = mapp_SCU41MVGetUserConfigWinWidth(u8DispWin);
    l_u16Height   = mapp_SCU41MVGetUserConfigWinHeight(u8DispWin);

    if(mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_bSta = FALSE;
    }
    else
    {
        pstDispWin->u16StartX   = l_u16StartX;
        pstDispWin->u16StartY   = l_u16StartY;
        pstDispWin->u16Width    = l_u16Width;
        pstDispWin->u16Height   = l_u16Height;
    }

    return l_bSta;
}

bool mapp_SCU41MVGetPixelCfgSize( BYTE u8DispWin, pstWindowType_t pstDispWin)
{
    bool l_bSta = TRUE;
    u16 l_u16StartX;
    u16 l_u16StartY;
    u16 l_u16Width;
    u16 l_u16Height;

    l_u16StartX   = mapp_SCU41MVGetUserWinPixelCfgStartX(u8DispWin);
    l_u16StartY   = mapp_SCU41MVGetUserWinPixelCfgStartY(u8DispWin);
    l_u16Width    = mapp_SCU41MVGetUserWinPixelCfgWidth(u8DispWin);
    l_u16Height   = mapp_SCU41MVGetUserWinPixelCfgHeight(u8DispWin);

    if(mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_bSta = FALSE;
    }
    else
    {
        pstDispWin->u16StartX   = l_u16StartX;
        pstDispWin->u16StartY   = l_u16StartY;
        pstDispWin->u16Width    = l_u16Width;
        pstDispWin->u16Height   = l_u16Height;
    }

    return l_bSta;
}

void mapp_SCU41MVGetSourceSize(u8 u8Src, pstWindowType_t pstSrcWin)
{
    stWindowType_t l_stSrcType;
    u32 l_u32Pclk;

    it66021_GetInputTiming(u8Src,&l_stSrcType.u16Width,&l_stSrcType.u16Height,&l_u32Pclk);
    l_stSrcType.u16StartX = 0;
    l_stSrcType.u16StartY = 0;

    pstSrcWin->u16StartX    = l_stSrcType.u16StartX;
    pstSrcWin->u16StartY    = l_stSrcType.u16StartY;
    pstSrcWin->u16Width     = l_stSrcType.u16Width;
    pstSrcWin->u16Height    = l_stSrcType.u16Height;
}

void mapp_SCU41MVGetSourceClipSize(bool bClipWin, pstWindowType_t pstClipWin)
{
    if(bClipWin)
    {
        pstClipWin->u16StartX    = g_stUserWinClip.u16StartX;
        pstClipWin->u16StartY    = g_stUserWinClip.u16StartY;
        pstClipWin->u16Width     = g_stUserWinClip.u16Width;
        pstClipWin->u16Height    = g_stUserWinClip.u16Height;
    }
    else
    {
        pstClipWin->u16StartX    = 0;
        pstClipWin->u16StartY    = 0;
        pstClipWin->u16Width     = 0;
        pstClipWin->u16Height    = 0;
    }
}

bool mapp_SCU41MVMultiWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable)
{
    stWindowType_t l_stSrcType;
    stWindowType_t l_stDstType;
    stWindowType_t l_stCipType;
    bool l_bWinClipSta = g_bWinClipModeSta;

    mapp_SCU41MVGetSourceSize(u8Src,&l_stSrcType);
    mapp_SCU41MVGetWindowSize(u8WinMode,u8WinType,&l_stDstType);
    mapp_SCU41MVGetSourceClipSize(l_bWinClipSta,&l_stCipType);
    Dbg_UserFun(("+++mapp_SCU41MVMultiWinCfg\n"));
    Dbg_UserFun(("u8WinMode == %d\n",u8WinMode));
    Dbg_UserFun(("Input[%d]: %d x %d \r\n",u8Src,l_stSrcType.u16Width,l_stSrcType.u16Height));
    Dbg_UserFun(("Output[%d]: %d,%d %d x %d \r\n",u8WinType,l_stDstType.u16StartX,l_stDstType.u16StartY,l_stDstType.u16Width,l_stDstType.u16Height));
    if(!mapp_SCU41MVLimitBaudwidth(u8Src,u8WinType,&l_stSrcType,&l_stDstType))
    {
        Dbg_UserFun(("!!!Baudwidth is over\n"));
        //CommResMsgSnd(("!!!Baudwidth is over :Src[%d] Win[%d]\n",u8Src,u8WinType));
        KeyPressCallBack(emKeyFunc_BaudwidthIsOver);
        return FALSE;
    }
    #if 0
    g_stWinConfigVar[u8WinType].u8SrcPort = u8Src;
    g_stWinConfigVar[u8WinType].u8WinNum = u8WinType;
    memcpy(&g_stWinConfigVar[u8WinType].stSrcWin,&l_stSrcType,sizeof(stWindowType_t));
    memcpy(&g_stWinConfigVar[u8WinType].stDstWin,&l_stDstType,sizeof(stWindowType_t));
    memcpy(&g_stWinConfigVar[u8WinType].stClipWin,&l_stCipType,sizeof(stWindowType_t));
    g_stWinConfigVar[u8WinType].bEnClip = l_bWinClipSta;
    g_stWinConfigVar[u8WinType].bEnWin = bWinEnable;
    #else
    mapp_SCU41MVWindowConfigure(u8Src,u8WinType,&l_stSrcType,&l_stDstType,&l_stCipType,l_bWinClipSta,bWinEnable);
    #endif
    return TRUE;
}

bool mapp_SCU41MVResizeWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable)
{
    stWindowType_t l_stSrcType;
    stWindowType_t l_stDstType;
    stWindowType_t l_stCipType;
    bool l_bWinClipSta = g_bWinClipModeSta;

    mapp_SCU41MVGetSourceSize(u8Src,&l_stSrcType);
    mapp_SCU41MVGetResize(u8WinMode,u8WinType,&l_stDstType);
    mapp_SCU41MVGetSourceClipSize(l_bWinClipSta,&l_stCipType);
    Dbg_UserFun(("+++mapp_SCU41MVResizeWinCfg\n"));
    Dbg_UserFun(("u8WinMode == %d\n",u8WinMode));
    Dbg_UserFun(("Input[%d]: %d x %d \r\n",u8Src,l_stSrcType.u16Width,l_stSrcType.u16Height));
    Dbg_UserFun(("Output[%d]: %d,%d %d x %d \r\n",u8WinType,l_stDstType.u16StartX,l_stDstType.u16StartY,l_stDstType.u16Width,l_stDstType.u16Height));
    if(!mapp_SCU41MVLimitBaudwidth(u8Src,u8WinType,&l_stSrcType,&l_stDstType))
    {
        Dbg_UserFun(("!!!Baudwidth is over\n"));
        //CommResMsgSnd(("!!!Baudwidth is over :Src[%d] Win[%d]\n",u8Src,u8WinType));
        KeyPressCallBack(emKeyFunc_BaudwidthIsOver);
        return FALSE;
    }
    mapp_SCU41MVWindowConfigure(u8Src,u8WinType,&l_stSrcType,&l_stDstType,&l_stCipType,l_bWinClipSta,bWinEnable);
    return TRUE;
}

bool mapp_SCU41MVConfigModeWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable)
{
    stWindowType_t l_stSrcType;
    stWindowType_t l_stDstType;
    stWindowType_t l_stCipType;
    bool l_bWinClipSta = g_bWinClipModeSta;

    mapp_SCU41MVGetSourceSize(u8Src,&l_stSrcType);
    mapp_SCU41MVGetCfgSize(u8WinType,&l_stDstType);
    mapp_SCU41MVGetSourceClipSize(l_bWinClipSta,&l_stCipType);
    Dbg_UserFun(("+++mapp_SCU41MVConfigModeWinCfg\n"));
    Dbg_UserFun(("u8WinMode == %d\n",u8WinMode));
    Dbg_UserFun(("Input[%d]: %d x %d \r\n",u8Src,l_stSrcType.u16Width,l_stSrcType.u16Height));
    Dbg_UserFun(("Output[%d]: %d,%d %d x %d \r\n",u8WinType,l_stDstType.u16StartX,l_stDstType.u16StartY,l_stDstType.u16Width,l_stDstType.u16Height));
    if(!mapp_SCU41MVLimitBaudwidth(u8Src,u8WinType,&l_stSrcType,&l_stDstType))
    {
        Dbg_UserFun(("!!!Baudwidth is over\n"));
        //CommResMsgSnd(("!!!Baudwidth is over :Src[%d] Win[%d]\n",u8Src,u8WinType));
        KeyPressCallBack(emKeyFunc_BaudwidthIsOver);
        return FALSE;
    }
    mapp_SCU41MVWindowConfigure(u8Src,u8WinType,&l_stSrcType,&l_stDstType,&l_stCipType,l_bWinClipSta,bWinEnable);
    UNUSED(u8WinMode);
    return TRUE;
}

bool mapp_SCU41MVPixelModeWinCfg(u8 u8Src, u8 u8WinType, u8 u8WinMode, bool bWinEnable)
{
    stWindowType_t l_stSrcType;
    stWindowType_t l_stDstType;
    stWindowType_t l_stCipType;
    bool l_bWinClipSta = g_bWinClipModeSta;

    mapp_SCU41MVGetSourceSize(u8Src,&l_stSrcType);
    mapp_SCU41MVGetPixelCfgSize(u8WinType,&l_stDstType);
    mapp_SCU41MVGetSourceClipSize(l_bWinClipSta,&l_stCipType);
    Dbg_UserFun(("+++mapp_SCU41MVPixelModeWinCfg\n"));
    Dbg_UserFun(("u8WinMode == %d\n",u8WinMode));
    Dbg_UserFun(("Input[%d]: %d x %d \r\n",u8Src,l_stSrcType.u16Width,l_stSrcType.u16Height));
    Dbg_UserFun(("Output[%d]: %d,%d %d x %d \r\n",u8WinType,l_stDstType.u16StartX,l_stDstType.u16StartY,l_stDstType.u16Width,l_stDstType.u16Height));
    if(!mapp_SCU41MVLimitBaudwidth(u8Src,u8WinType,&l_stSrcType,&l_stDstType))
    {
        Dbg_UserFun(("!!!Baudwidth is over\n"));
        //CommResMsgSnd(("!!!Baudwidth is over :Src[%d] Win[%d]\n",u8Src,u8WinType));
        KeyPressCallBack(emKeyFunc_BaudwidthIsOver);
        return FALSE;
    }
    mapp_SCU41MVWindowConfigure(u8Src,u8WinType,&l_stSrcType,&l_stDstType,&l_stCipType,l_bWinClipSta,bWinEnable);
    UNUSED(u8WinMode);
    return TRUE;
}

void mapp_SCU41MVWinConfigVarSend(u8 u8WinNum)
{
    u8 i;
    u8 l_u8Win;

    for(i=0;i<emSrcInputMax;i++)
    {
        l_u8Win = g_stWinConfigVar[i].u8WinNum;
        if(g_stWinConfigVar[i].bEnWin)
        {
            g_u8WinLayer[l_u8Win] = g_pu8WinLayerLevel[g_u8WinLevel][i];
            g_u8WinLayer[l_u8Win] = g_u8WinLayer[l_u8Win]|0x80;
        }
        else
        {
            g_u8WinLayer[l_u8Win] = g_pu8WinLayerLevel[g_u8WinLevel][i];
            g_u8WinLayer[l_u8Win] = g_u8WinLayer[l_u8Win]&0x7F;
        }
    }


    if(u8WinNum == emWinTypeMax)
    {
        for(i=0;i<emWinTypeMax;i++)
        {
            mapp_SCU41MVWindowConfigure(g_stWinConfigVar[i].u8SrcPort,
                                        g_stWinConfigVar[i].u8WinNum,
                                        &g_stWinConfigVar[i].stSrcWin,
                                        &g_stWinConfigVar[i].stDstWin,
                                        &g_stWinConfigVar[i].stClipWin,
                                        g_stWinConfigVar[i].bEnClip,
                                        g_stWinConfigVar[i].bEnWin);
        }
    }
    else
    {
        mapp_SCU41MVWindowConfigure(g_stWinConfigVar[u8WinNum].u8SrcPort,
                                    g_stWinConfigVar[u8WinNum].u8WinNum,
                                    &g_stWinConfigVar[u8WinNum].stSrcWin,
                                    &g_stWinConfigVar[u8WinNum].stDstWin,
                                    &g_stWinConfigVar[u8WinNum].stClipWin,
                                    g_stWinConfigVar[u8WinNum].bEnClip,
                                    g_stWinConfigVar[u8WinNum].bEnWin);
    }
}

void mapp_SCU41MVMultiWinUpdate(u8 u8Src,u8 u8WinType,bool bUpd)
{
    u8 l_u8Win;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    bool l_bWinEnable = FALSE;
    bool l_bWinCfgEnable = FALSE;

    if(u8Src == emSrcInputMax||u8WinType==emWinTypeMax)
    {
        for(l_u8Win=0;l_u8Win<emWinTypeMax;l_u8Win++)
        {
            l_bWinEnable = FALSE;
            switch(pstBasicVar->u8WindowNum)
            {
                case emWinNum_1:
                    {
                        if(l_u8Win<=emWinType_A)
                            l_bWinEnable = TRUE;
                    }
                    break;
                case emWinNum_2:
                    {
                        if(l_u8Win<=emWinType_B)
                            l_bWinEnable = TRUE;
                    }
                    break;
                case emWinNum_3:
                    {
                        if(l_u8Win<=emWinType_C)
                            l_bWinEnable = TRUE;
                    }
                    break;
                case emWinNum_4:
                    {
                        if(l_u8Win<=emWinType_D)
                            l_bWinEnable = TRUE;
                    }
                    break;
                default:
                    l_bWinEnable = FALSE;
                    break;
            }

            if(mapp_GetCfgMode() != emCfgMode_None)
            {
                l_bWinEnable = TRUE;

                switch(mapp_GetCfgMode())
                {
                    case emCfgMode_Pixel:
                        l_bWinCfgEnable = mapp_SCU41MVPixelModeWinCfg(g_stUserWinPixelCfg.u8UserSrc[l_u8Win],
                                                    l_u8Win,
                                                    0,
                                                    l_bWinEnable);
                        break;
                    default:
                        l_bWinCfgEnable = mapp_SCU41MVConfigModeWinCfg(g_stUserWinsConfig.u8UserSrc[l_u8Win],
                                                    l_u8Win,
                                                    0,
                                                    l_bWinEnable);
                        break;
                }
            }
            else if(pstBasicVar->u8ResizeMode!=emResize_None)
            {
                l_bWinCfgEnable = mapp_SCU41MVResizeWinCfg(pstBasicVar->u8WinSel[l_u8Win],l_u8Win,pstBasicVar->u8ResizeMode,l_bWinEnable);
            }
            else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
            {
                u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                l_bWinCfgEnable = mapp_SCU41MVMultiWinCfg(pstBasicVar->stUserWinsConfig[l_u8UserMode].u8UserSrc[l_u8Win],
                                        l_u8Win,
                                        pstBasicVar->u8MvMode,
                                        (bool)pstBasicVar->stUserWinsConfig[l_u8UserMode].u8EnWinSta[l_u8Win]
                                        /*l_bWinEnable*/);
            }
            else
            {
                l_bWinCfgEnable = mapp_SCU41MVMultiWinCfg(pstBasicVar->u8WinSel[l_u8Win],
                                        l_u8Win,
                                        pstBasicVar->u8MvMode,
                                        l_bWinEnable);
            }
            //l_bWinCfgEnable = TRUE;
        }

        //mapp_SCU41MVWinConfigVarSend(emWinTypeMax);
        if(l_bWinCfgEnable&&bUpd)
        {
            l_bWinCfgEnable = FALSE;
            mapp_SCU41MVFpgaWinCmdUpdate();
        }
    }
    else
    {
        l_u8Win = u8WinType;
        switch(pstBasicVar->u8WindowNum)
        {
            case emWinNum_1:
                {
                    if(l_u8Win<=emWinType_A)
                        l_bWinEnable = TRUE;
                }
                break;
            case emWinNum_2:
                {
                    if(l_u8Win<=emWinType_B)
                        l_bWinEnable = TRUE;
                }
                break;
            case emWinNum_3:
                {
                    if(l_u8Win<=emWinType_C)
                        l_bWinEnable = TRUE;
                }
                break;
            case emWinNum_4:
                {
                    if(l_u8Win<=emWinType_D)
                        l_bWinEnable = TRUE;
                }
                break;
            default:
                l_bWinEnable = FALSE;
                break;
        }

        if(mapp_GetCfgMode() != emCfgMode_None)
        {
            l_bWinEnable = TRUE;

            switch(mapp_GetCfgMode())
            {
                case emCfgMode_Pixel:
                    l_bWinCfgEnable = mapp_SCU41MVPixelModeWinCfg(g_stUserWinPixelCfg.u8UserSrc[l_u8Win],
                                                l_u8Win,
                                                0,
                                                l_bWinEnable);
                    break;
                default:
                    l_bWinCfgEnable = mapp_SCU41MVConfigModeWinCfg(g_stUserWinsConfig.u8UserSrc[l_u8Win],
                                                l_u8Win,
                                                0,
                                                l_bWinEnable);
                    break;
            }
        }
        else if(pstBasicVar->u8ResizeMode!=emResize_None)
        {
            l_bWinCfgEnable = mapp_SCU41MVResizeWinCfg(pstBasicVar->u8WinSel[l_u8Win],l_u8Win,pstBasicVar->u8ResizeMode,l_bWinEnable);
        }
        else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
        {
            u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
            l_bWinCfgEnable = mapp_SCU41MVMultiWinCfg(pstBasicVar->stUserWinsConfig[l_u8UserMode].u8UserSrc[l_u8Win],
                                    l_u8Win,
                                    pstBasicVar->u8MvMode,
                                    (bool)pstBasicVar->stUserWinsConfig[l_u8UserMode].u8EnWinSta[l_u8Win]
                                    /*l_bWinEnable*/);
        }
        else
        {
            l_bWinCfgEnable = mapp_SCU41MVMultiWinCfg(pstBasicVar->u8WinSel[l_u8Win],
                                    l_u8Win,
                                    pstBasicVar->u8MvMode,
                                    l_bWinEnable);
        }
        //l_bWinCfgEnable = TRUE;
        //mapp_SCU41MVWinConfigVarSend(l_u8Win);

        if(l_bWinCfgEnable&&bUpd)
        {
            l_bWinCfgEnable = FALSE;
            mapp_SCU41MVFpgaWinCmdUpdate();
        }
    }
}

void mapp_SCU41MVSrcChangeToSetWinProcess(void)
{
    u8 l_u8Src,l_u8Win;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    bool l_bWinEnable = FALSE;
    bool l_bWinCfgEnable = FALSE;
    static u16 sl_u16StartupCnt = 60;  // unit = 50ms
    static bool sl_bStarupToUpdateAudio = TRUE;

    if(sl_u16StartupCnt)
    {
        sl_u16StartupCnt--;

        if(sl_u16StartupCnt == 30)
        {
            #if 0//_ENABLE_EDID_SWITCH_A_SINGLE_CHANNEL    // 解决上电的时候，笔记本不出信号的问题
            pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
            pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
            SCU41_MV_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[emSrcInput_Hdmi1],emSrcInput_Hdmi1);
            SCU41_MV_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[emSrcInput_Hdmi2],emSrcInput_Hdmi2);
            SCU41_MV_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[emSrcInput_Hdmi3],emSrcInput_Hdmi3);
            SCU41_MV_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[emSrcInput_Hdmi4],emSrcInput_Hdmi4);
            #endif
        }
        return;
    }

    for(l_u8Src = 0;l_u8Src<_HDMI_INPUT_PORT_USED_NUMBER;l_u8Src++)
    {
        if(mapp_it66021_GetVideoChangeSta(l_u8Src) != emIt66021_VidChgNone)
        {
            for(l_u8Win=0;l_u8Win<emWinTypeMax;l_u8Win++)
            {
                l_bWinEnable = FALSE;
                switch(pstBasicVar->u8WindowNum)
                {
                    case emWinNum_1:
                        {
                            if(l_u8Win<=emWinType_A)
                                l_bWinEnable = TRUE;
                        }
                        break;
                    case emWinNum_2:
                        {
                            if(l_u8Win<=emWinType_B)
                                l_bWinEnable = TRUE;
                        }
                        break;
                    case emWinNum_3:
                        {
                            if(l_u8Win<=emWinType_C)
                                l_bWinEnable = TRUE;
                        }
                        break;
                    case emWinNum_4:
                        {
                            if(l_u8Win<=emWinType_D)
                                l_bWinEnable = TRUE;
                        }
                        break;
                    default:
                        l_bWinEnable = FALSE;
                        break;
                }

                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    l_bWinEnable = TRUE;
                }

                if(mapp_it66021_GetVideoChangeSta(l_u8Src)==emIt66021_VidChgeVidOn)
                {
                }
                else if(mapp_it66021_GetVideoChangeSta(l_u8Src)==emIt66021_VidChgeVidoff)
                {
                    l_bWinEnable = FALSE;
                }

                //if(pstBasicVar->u8WinSel[l_u8Win]==l_u8Src)
                {
                    if(mapp_GetCfgMode() != emCfgMode_None)
                    {
                        switch(mapp_GetCfgMode())
                        {
                            case emCfgMode_Pixel:
                                if(g_stUserWinPixelCfg.u8UserSrc[l_u8Win]==l_u8Src)
                                {
                                    l_bWinCfgEnable = mapp_SCU41MVPixelModeWinCfg(g_stUserWinPixelCfg.u8UserSrc[l_u8Win],
                                                                                    l_u8Win,
                                                                                    0,
                                                                                    l_bWinEnable);
                                }
                                break;
                            default:
                                if(g_stUserWinsConfig.u8UserSrc[l_u8Win]==l_u8Src)
                                {
                                    l_bWinCfgEnable = mapp_SCU41MVConfigModeWinCfg(g_stUserWinsConfig.u8UserSrc[l_u8Win],
                                                                                    l_u8Win,
                                                                                    0,
                                                                                    l_bWinEnable);
                                }
                                break;
                        }
                    }
                    else if(pstBasicVar->u8ResizeMode!=emResize_None)
                    {
                        if(pstBasicVar->u8WinSel[l_u8Win]==l_u8Src)
                        {
                            l_bWinCfgEnable = mapp_SCU41MVResizeWinCfg(l_u8Src,
                                                                        l_u8Win,
                                                                        pstBasicVar->u8ResizeMode,
                                                                        l_bWinEnable);
                        }
                    }
                    else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                    {
                        u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                        if(pstBasicVar->stUserWinsConfig[l_u8UserMode].u8UserSrc[l_u8Win]==l_u8Src)
                        {
                            l_bWinCfgEnable = mapp_SCU41MVMultiWinCfg(pstBasicVar->stUserWinsConfig[l_u8UserMode].u8UserSrc[l_u8Win],
                                                    l_u8Win,
                                                    pstBasicVar->u8MvMode,
                                                    (bool)(pstBasicVar->stUserWinsConfig[l_u8UserMode].u8EnWinSta[l_u8Win]&&l_bWinEnable)
                                                    );
                        }
                    }
                    else
                    {
                        if(pstBasicVar->u8WinSel[l_u8Win]==l_u8Src)
                        {
                            l_bWinCfgEnable = mapp_SCU41MVMultiWinCfg(pstBasicVar->u8WinSel[l_u8Win],
                                                    l_u8Win,
                                                    pstBasicVar->u8MvMode,
                                                    l_bWinEnable);
                        }
                    }

                    //mapp_SCU41MVWinConfigVarSend(l_u8Win);
                }
            }
            mapp_it66021_SetVideoChangeSta(l_u8Src, (u8)emIt66021_VidChgNone);
        }
    }

    if(l_bWinCfgEnable)
    {
        l_bWinCfgEnable = FALSE;
        mapp_SCU41MVFpgaWinCmdUpdate();
    }

    if(sl_bStarupToUpdateAudio)
    {
        sl_bStarupToUpdateAudio = FALSE;
        mapp_SCU41MVSetAudioMute(pstBasicVar->bAudioMute);
        mapp_SCU41MVSetAudioInput(pstBasicVar->u8AudioSel);
        mapp_SCU41MVSetAudioMix(pstBasicVar->bAudioMix);
    }
}

void mapp_SetCfgMode(u8 u8CfgMode)
{
    sg_u8ConfigMode = u8CfgMode;
}

u8 mapp_GetCfgMode(void)
{
    return sg_u8ConfigMode;
}

void mapp_SCU41MVCleanWin(u8 u8Win)
{
    u8 l_pu8Data[6]={0x00,0x00,0x00,0x00,0x00,0x00};
    u8 l_u8winID;

    if(u8Win == emWinTypeMax)
    {
        for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
        {
            g_u8WinLayer[l_u8winID] = 0;
        }
        SCU41_MV_SendCmd(0x01,FPGA_REG_WIN,6,l_pu8Data);
        SCU41_MV_SendCmd(0x02,FPGA_REG_WIN,6,l_pu8Data);
        mapp_SCU41MVFpgaWinCmdUpdate();
    }
    else
    {
        switch(u8Win)
        {
            case emWinType_A:
                for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
                {
                    if((g_u8WinLayer[l_u8winID]&0x7F) == g_pu8WinLayerLevel[g_u8WinLevel][emWinType_A])
                    {
                        g_u8WinLayer[l_u8winID] = 0;//g_u8WinLayer1&0x7F;
                    }
                    l_pu8Data[2+l_u8winID] = g_u8WinLayer[l_u8winID];
                }
                break;
            case emWinType_B:
                for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
                {
                    if((g_u8WinLayer[l_u8winID]&0x7F) == g_pu8WinLayerLevel[g_u8WinLevel][emWinType_B])
                    {
                        g_u8WinLayer[l_u8winID] = 0;//g_u8WinLayer1&0x7F;
                    }
                    l_pu8Data[2+l_u8winID] = g_u8WinLayer[l_u8winID];
                }
                break;
            case emWinType_C:
                for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
                {
                    if((g_u8WinLayer[l_u8winID]&0x7F) == g_pu8WinLayerLevel[g_u8WinLevel][emWinType_C])
                    {
                        g_u8WinLayer[l_u8winID] = 0;//g_u8WinLayer1&0x7F;
                    }
                    l_pu8Data[2+l_u8winID] = g_u8WinLayer[l_u8winID];
                }
                break;
            case emWinType_D:
                for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
                {
                    if((g_u8WinLayer[l_u8winID]&0x7F) == g_pu8WinLayerLevel[g_u8WinLevel][emWinType_D])
                    {
                        g_u8WinLayer[l_u8winID] = 0;//g_u8WinLayer1&0x7F;
                    }
                    l_pu8Data[2+l_u8winID] = g_u8WinLayer[l_u8winID];
                }
                break;
            default:
                break;
        }

        SCU41_MV_SendCmd(0x01,FPGA_REG_WIN,6,l_pu8Data);
        SCU41_MV_SendCmd(0x02,FPGA_REG_WIN,6,l_pu8Data);
        mapp_SCU41MVFpgaWinCmdUpdate();
    }
}

u16 mapp_CheckTheSameValue(u8 *pu8Data,u16 u16len)
{
    u16 l_u16RtVal = 0;
    u8 l_u8Temp = 0;
    u16 i,l_u16Index;

    l_u16Index = 0;
    l_u8Temp = pu8Data[l_u16Index];
    for(i=1;i<u16len;i++)
    {
        if(l_u8Temp == pu8Data[i])
        {
            l_u16RtVal = i;
            break;
        }
        else
        {
            if(i==(u16len-1))
            {
                l_u16Index++;
                if(l_u16Index>=u16len)
                    break;
                l_u8Temp = pu8Data[l_u16Index];
                i = l_u16Index;
            }
        }
    }

    return l_u16RtVal;
}

void mapp_SCU41MVSetLayerLevel(u8 u8ID,u8 u8Lv1,u8 u8Lv2,u8 u8Lv3,u8 u8Lv4)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_pu8Temp[4];
    u8 l_pu8Data[6]={0x00,0x00,0x00,0x00,0x00,0x00};
    u8 l_u8winID;

    l_pu8Temp[0] = u8Lv1;
    l_pu8Temp[1] = u8Lv2;
    l_pu8Temp[2] = u8Lv3;
    l_pu8Temp[3] = u8Lv4;

    if(mapp_CheckTheSameValue(l_pu8Temp,4)>0)
    {
        u16 l_u16Index = mapp_CheckTheSameValue(l_pu8Temp,4);
        Dbg_UserFun(("!!!Level is the same %d\n",l_pu8Temp[l_u16Index]));
        KeyPressCallBack(emKeyFunc_WinLevelIsTheSame);
        return;
    }

    g_u8WinLevel = u8ID;
    g_pu8WinLayerLevel[u8ID][emWinType_A] = u8Lv1;
    g_pu8WinLayerLevel[u8ID][emWinType_B] = u8Lv2;
    g_pu8WinLayerLevel[u8ID][emWinType_C] = u8Lv3;
    g_pu8WinLayerLevel[u8ID][emWinType_D] = u8Lv4;

    for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
    {
        g_u8WinLayer[l_u8winID] = (g_u8WinLayer[l_u8winID]&0x80)|g_pu8WinLayerLevel[u8ID][l_u8winID];
    }

    for(l_u8winID=0;l_u8winID<emWinTypeMax;l_u8winID++)
    {
        l_pu8Data[2+l_u8winID] = g_u8WinLayer[pstBasicVar->u8WinSel[l_u8winID]];
    }
    SCU41_MV_SendCmd(0x01,FPGA_REG_WIN,6,l_pu8Data);
    SCU41_MV_SendCmd(0x02,FPGA_REG_WIN,6,l_pu8Data);
    mapp_SCU41MVFpgaWinCmdUpdate();
}

bool mapp_SCU41MVIsMvUserMode(u8 u8MvMode)
{
    bool l_bSta = FALSE;

    switch(u8MvMode)
    {
        case emMvMode_xWin_ByUser1:
        case emMvMode_xWin_ByUser2:
        case emMvMode_xWin_ByUser3:
        case emMvMode_xWin_ByUser4:
            l_bSta = TRUE;
            break;
        default:
            break;
    }
    return l_bSta;
}

bool mapp_SCU41MVLimitBaudwidth(u8 u8Src,u8 u8Win,pstWindowType_t pstSrcWin,pstWindowType_t pstDstWin)
{
    u8 l_u8InVfeq = 0,l_u8Index;
    static u16 sl_u16InBaudwidth[emWinTypeMax]={0};
    static u16 sl_u16OutBaudwidth[emWinTypeMax]={0};
    static bool sl_bFirstFlag = TRUE;
    u16 l_u16TempVal = 0;
    bool l_bRetSta = FALSE;

    #define Max_Baudwidth  256

    if(sl_bFirstFlag)
    {
        sl_bFirstFlag = FALSE;
        memset(sl_u16InBaudwidth,0,emWinTypeMax);
        memset(sl_u16OutBaudwidth,0,emWinTypeMax);
    }

    it66021_GetInputVFreq(u8Src,&l_u8InVfeq);
    if(pstSrcWin->u16Height > pstDstWin->u16Height)
    {
        sl_u16InBaudwidth[u8Win]    = ((pstDstWin->u16Height*pstDstWin->u16Width*l_u8InVfeq/1000000)*16)/100;
        sl_u16OutBaudwidth[u8Win]   = ((pstDstWin->u16Height*pstDstWin->u16Width*g_stTiming.u8VClk/1000000)*16)/100;
    }
    else
    {
        sl_u16InBaudwidth[u8Win]    = ((pstSrcWin->u16Height*pstSrcWin->u16Width*l_u8InVfeq/1000000)*16)/100;
        sl_u16OutBaudwidth[u8Win]   = ((pstSrcWin->u16Height*pstSrcWin->u16Width*g_stTiming.u8VClk/1000000)*16)/100;
    }
    for(l_u8Index = 0;l_u8Index<emWinTypeMax;l_u8Index++)
    {
        l_u16TempVal += (sl_u16InBaudwidth[l_u8Index]+sl_u16OutBaudwidth[l_u8Index]);
    }

    if(((l_u16TempVal*100)/Max_Baudwidth)<80)
    {
        l_bRetSta = TRUE;
    }

    #undef Max_Baudwidth

    return l_bRetSta;
}

void mapp_SCU41MVSetWinCfgUpdateFlag(u32 u32DlyTime, bool bEn)
{
    sg_bWincfgUpdateFlag = bEn;
    #if _ENABLE_WIN_CONFIG_UPD_TIMER
    PlatformTimerSet(etUser_WinCfgUpdTimer,u32DlyTime);
    #endif
}

bool mapp_SCU41MVGetWinCfgUpdateFlag(void)
{
    if(sg_bWincfgUpdateFlag && PlatformTimerExpired(etUser_WinCfgUpdTimer))
    {
        mapp_SCU41MVSetWinCfgUpdateFlag(0,FALSE);
        return TRUE;
    }
    else
        return FALSE;
}

void mapp_SCU41MVGetWinCfgUpdProcess(void)
{
    if(mapp_SCU41MVGetWinCfgUpdateFlag())
    {
        mapp_SCU41MVMultiWinUpdate((u8)emSrcInputMax,emWinTypeMax,TRUE);
    #if _ENABLE_CHIP_ITE6615
        mapp_Ite6615SetSignalInputChange(TRUE);
        #if _ENABLE_THE_BEST_RESOLUTION_BY_DISPLAY
        {
            pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
            pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
            if(pstBasicVar->bPowerOnSta)
            {
                if(mapp_Ite6615GetPowerState())
                {
                    mapp_Ite6615SetPowerOffState(FALSE);
                }
            }
        }
        #endif
    #endif
    }
}

