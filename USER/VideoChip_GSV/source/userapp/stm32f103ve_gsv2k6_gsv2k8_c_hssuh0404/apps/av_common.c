#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "av_config.h"

#if AvEnableCecFeature /* CEC Related */
uchar  DevicePowerStatus = 0;
char   DeviceName[20] = "GSV Default";
uchar  AudioStatus = 0;
CEC_AUDIO_STATUS Cec_Tx_Audio_Status;
#endif

const uint8 PktSize[20] ={31, /*AV_PKT_AV_INFO_FRAME */
                          14, /*AV_PKT_AUDIO_INFO_FRAME*/
                          31, /*AV_PKT_ACP_PACKET*/
                          31, /*AV_PKT_SPD_PACKET*/
                          31, /*AV_PKT_ISRC1_PACKET*/
                          31, /*AV_PKT_ISRC2_PACKET*/
                          31, /*AV_PKT_GMD_PACKET*/
                          32, /*AV_PKT_GC_PACKET*/
                          17, /*AV_PKT_MPEG_PACKET*/
                        #if GSV5K1
                          31,//10, /*AV_PKT_VS_PACKET*/
                        #else
                          10, /*AV_PKT_VS_PACKET*/
                        #endif
                          5,  /*AV_PKT_AUDIO_CHANNEL_STATUS*/
                          32, /*AV_PKT_AUDIO_SAMPLE_PACKET*/
                          32, /*AV_PKT_ACR_PACKET*/
                          32, /*AV_PKT_EXT_AV_INFO_FRAME*/
                          31, /*AV_PKT_HDR_PACKET*/
                          32, /*AV_PKT_SPARE2_PACKET*/
                          32, /*AV_PKT_SPARE3_PACKET*/
                          32, /*AV_PKT_SPARE4_PACKET*/
                          32, /*AV_PKT_UNKNOWN_PACKET*/
                          32};/*AV_PKT_ALL_PACKETS*/

                       /*  32    44    48     88     96    176    192    768   Undefined */
const uint32 NTable[9] ={4096, 6272, 6144, 12544, 12288, 25088, 24576, 98304,  4096};
const uchar  NIdx[16]  ={ 1,  8,  2,  0,  8,  8,  8,  8,
                                    3,  7,  4,  8,  5,  8,  6,  8};
/* i*3+2 = MCLK ratio, 1 = 256Fs, 3 = 128Fs, 0 = 512Fs */
const uchar ChannelStatusSfTable[] = {
    (uchar)AV_AUD_FS_32KHZ,     3,    1,
    (uchar)AV_AUD_FS_44KHZ,     0,    1,
    (uchar)AV_AUD_FS_48KHZ,     2,    1,
    (uchar)AV_AUD_FS_88KHZ,     8,    1,
    (uchar)AV_AUD_FS_96KHZ,    10,    1,
    (uchar)AV_AUD_FS_176KHZ,   12,    1,
    (uchar)AV_AUD_FS_192KHZ,   14,    1,
    (uchar)AV_AUD_FS_HBR,       9,    1,
    (uchar)AV_AUD_FS_FROM_STRM, 0,    1,
    0xff,                    0xff, 0xff
};

#if AvEnableCecFeature /* CEC Related */

/* Size is FLEX_OP_CODES */
const uchar FlexOpCodes[] = {
/* opcode; standard parameters length, max parameters length, min parameters length   */
    AV_CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR, 4,5,2
};

/* Size is CEC_OP_CODES */
const uint8 CecOpCodes[] = {
        AV_CEC_MSG_STANDBY, 2,         AV_CEC_MSG_ROUTE_CHANGE, 6,
        AV_CEC_MSG_ROUTE_INFO, 4,      AV_CEC_MSG_ACTIVE_SRC, 4,
        AV_CEC_MSG_GIVE_PHYS_ADDR, 2,  AV_CEC_MSG_REPORT_PHYS_ADDR, 5,
        AV_CEC_MSG_SET_STRM_PATH, 4,   AV_CEC_MSG_ABORT, 2,
        AV_CEC_MSG_FEATURE_ABORT, 4,   AV_CEC_MSG_INITIATE_ARC , 2,
        AV_CEC_MSG_REPORT_ARC_INITIATED, 2, AV_CEC_MSG_REPORT_ARC_TERMINATED, 2,
        AV_CEC_MSG_REQUEST_ARC_INITIATION, 2, AV_CEC_MSG_REQUEST_ARC_TERMINATION, 2,
        AV_CEC_MSG_TERMINATE_ARC, 2,            AV_CEC_MSG_IMAGE_VIEW_ON, 2,
        AV_CEC_MSG_TUNER_STEP_INC, 2,           AV_CEC_MSG_TUNER_STEP_DEC, 2,
        AV_CEC_MSG_GIVE_TUNER_DEV_STATUS, 3,    AV_CEC_MSG_RECORD_STATUS, 3,
        AV_CEC_MSG_RECORD_OFF, 2,           AV_CEC_MSG_TEXT_VIEW_ON, 2,
        AV_CEC_MSG_RECORD_TV_SCREEN, 2,     AV_CEC_MSG_GIVE_DECK_STATUS, 3,
        AV_CEC_MSG_DECK_STATUS, 3,          AV_CEC_MSG_SET_MENU_LANGUAGE, 5,
        AV_CEC_MSG_CLR_ANALOGUE_TIMER, 13,  AV_CEC_MSG_SET_ANALOGUE_TIMER, 13,
        AV_CEC_MSG_PLAY, 3,                 AV_CEC_MSG_DECK_CONTROL, 3,
        AV_CEC_MSG_TIMER_CLEARED_STATUS, 3, AV_CEC_MSG_USER_CONTROL_RELEASED, 2,
        AV_CEC_MSG_GIVE_OSD_NAME, 2,   AV_CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS, 2,
        AV_CEC_MSG_GIVE_AUDIO_STATUS, 2,    AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE, 3,
        AV_CEC_MSG_REPORT_AUDIO_STATUS, 3,  AV_CEC_MSG_SYSTEM_AUDIO_MODE_STATUS, 3,
        AV_CEC_MSG_REQ_ACTIVE_SRC, 2,       AV_CEC_MSG_DEVICE_VENDOR_ID, 5,
        AV_CEC_MSG_VENDOR_REMOTE_BTN_UP, 2, AV_CEC_MSG_GET_VENDOR_ID, 2,
        AV_CEC_MSG_MENU_REQUEST, 3,         AV_CEC_MSG_MENU_STATUS, 3,
        AV_CEC_MSG_GIVE_PWR_STATUS, 2,      AV_CEC_MSG_REPORT_PWR_STATUS, 3,
        AV_CEC_MSG_GET_MENU_LANGUAGE, 2,    AV_CEC_MSG_SEL_ANALOGUE_SERVICE, 6,
        AV_CEC_MSG_SEL_DIGITAL_SERVICE, 9,  AV_CEC_MSG_SET_DIGITAL_TIMER, 16,
        AV_CEC_MSG_CLR_DIGITAL_TIMER, 16,   AV_CEC_MSG_SET_AUDIO_RATE, 3,
        AV_CEC_MSG_INACTIVE_SOURCE, 4,      AV_CEC_MSG_CEC_VERSION, 3,
        AV_CEC_MSG_GET_CEC_VERSION, 2,      AV_CEC_MSG_CLR_EXTERNAL_TIMER, 13,
        AV_CEC_MSG_SET_EXTERNAL_TIMER, 13,
        AV_CEC_MSG_REPORT_SHORT_AUDIO_DESCRIPTOR, 5,
        AV_CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR, 4,
        AV_CEC_MSG_USER_CONTROL_PRESSED,           3
};
#endif

const uint8 ChanCount[32] =
{
        1, 2, 2, 3, 2, 3, 3, 4,
        3, 4, 4, 5, 4, 5, 5, 6,
        5, 6, 6, 7, 3, 4, 4, 5,
        4, 5, 5, 6, 5, 6, 6, 7
};

const AvVideoAspectRatio ARTable[108] =
{
    AV_AR_NOT_INDICATED,  AV_AR_4_3,  AV_AR_4_3,  AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, /* VIC 0 ~ 7 */
    AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, /* VIC 8 ~ 15 */
    AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  /* VIC 16 ~ 23 */
    AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_16_9, /* VIC 24 ~ 31 */
    AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_16_9, /* VIC 32 ~ 39 */
    AV_AR_16_9, AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, /* VIC 40 ~ 47 */
    AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, /* VIC 48 ~ 55 */
    AV_AR_4_3,  AV_AR_16_9, AV_AR_4_3,  AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, AV_AR_16_9, /* VIC 56 ~ 63 */
    AV_AR_16_9,                                                                                     /* VIC 64 */
    AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27,/* VIC 65 ~ 72 */
    AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27,/* VIC 73 ~ 80 */
    AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27,/* VIC 81 ~ 88 */
    AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27,                                             /* VIC 89 ~ 92 */
    AV_AR_16_9,  AV_AR_16_9,  AV_AR_16_9,  AV_AR_16_9,  AV_AR_16_9,                                 /* VIC 93 ~ 97 */
    AV_AR_256_135, AV_AR_256_135, AV_AR_256_135, AV_AR_256_135, AV_AR_256_135,                      /* VIC 98 ~ 102 */
    AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27, AV_AR_64_27                                 /* VIC 103 ~ 107 */
};

const uchar VideoGenVicTable[] = {
    0x10, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, /* 1080p60 */
    0x61, 0x5F, 0x01, 0x00, 0x00, 0x00, 0x00, /* 4K60  */
    0x5F, 0x5F, 0x02, 0x00, 0x00, 0x00, 0x00, /* 4K30  */
    0x60, 0x5E, 0x01, 0x00, 0x00, 0x00, 0x00, /* 4K50  */
    0x5E, 0x5E, 0x02, 0x00, 0x00, 0x00, 0x00, /* 4K25  */
    0x5D, 0x5D, 0x02, 0x00, 0x00, 0x00, 0x00, /* 4K24  */
    0x62, 0x62, 0x02, 0x00, 0x00, 0x00, 0x00, /* 4KS24 */
    0x04, 0x04, 0x08, 0x00, 0x00, 0x00, 0x00, /* 720p  */
    0x02, 0x03, 0x16, 0x00, 0x00, 0x00, 0x00, /* 480p  */
    0x11, 0x11, 0x16, 0x00, 0x00, 0x00, 0x00, /* 576p  */
    0x06, 0x06, 0x16, 0x00, 0x00, 0x00, 0x00, /* 480i  */
    0x15, 0x15, 0x16, 0x00, 0x00, 0x00, 0x00, /* 576i  */
    0x05, 0x05, 0x08, 0x00, 0x00, 0x00, 0x00, /* 1080i60 */
    0x14, 0x14, 0x08, 0x00, 0x00, 0x00, 0x00, /* 1080i50 */
    0x27, 0x27, 0x08, 0x00, 0x00, 0x00, 0x00, /* 1080i50s */
    0xFF
};

#if GSV2K11
const uint8 ParallelConfigTable[] = {
    0x00, 0x00, 0x00,    // 0: Invalid Setting for disabled Parallel bus
    0x04, 0x03, 0x82,    // 1: Index 21, 4x single pixel 12-bit 444
    0x14, 0x01, 0x82,    // 2: Index 61, 4x dual pixel 8-bit 444
//    0x04, 0x05, 0xC1,    // 3: Index 58-1, HIS mode in DDR mode
    0x01, 0x01, 0x00,    // 3:  TTL 24-bit, RGB/YCbCr 444, MODE A, SDR mode
// 	    0x01, 0x05, 0x00,    // 3:   0x01, 0x01, 0x00, YUV
    0x14, 0x01, 0xA2,    // 4: 4x dual pixel UDP mode using LVDS
    0x01, 0x07, 0x01,    // 5: TTL 36-bit, DDR mode, YCbCr 444
    0x01, 0x15, 0x10,    // 6: TTL BT.1120 16-bit, SDR mode, YCbCr 422
    0x11, 0x01, 0x01,    // 7: TTL 48-bit, DDR mode
    0x01, 0x15, 0x11,    // 8: TTL BT.1120 16-bit, DDR mode, YCbCr 422
    0x11, 0x01, 0x21,    // 9: dual pixel UDP mode using TTL
    0x01, 0x15, 0x00,    // 10: TTL BT.1120 16-bit, SDR mode, YCbCr 422, separate sync
    0x01, 0x11, 0x00,    // 11: TTL 24-bit, RGB/YCbCr 444, MODE B, SDR mode
    0x01, 0x01, 0x01,    // 12: TTL 24-bit, RGB/YCbCr 444, MODE A, DDR mode
    0x01, 0x11, 0x01,    // 13: TTL 24-bit, RGB/YCbCr 444, MODE B, DDR mode
    0xFF, 0xFF, 0xFF
};
#endif

#if GSV5K1
/************************ Cea Timing Starts Here ************************/
/*
  AvCeaTables:Vic,HActive,VActive,Interlaced,HTotal,HBlank,HFront,Hsync,HBack,VTotal,VBlank,VFront,VSync,Vback,VFreq
*/

const AvCeaTables CeaTimingTables[] = {
    {60,1280,720,0,3300,2020,1760,40,220,750,30,5,5,20,24},   //720p24
    {61,1280,720,0,3960,2680,2420,40,220,750,30,5,5,20,25},   //720p25
    {62,1280,720,0,3300,2020,1760,40,220,750,30,5,5,20,30},   //720p30
    {32,1920,1080,0,2750,830,638,44,148,1125,45,4,5,36,24},  //1080p24
    {33,1920,1080,0,2640,720,528,44,148,1125,45,4,5,36,25},  //1080p25
    {34,1920,1080,0,2200,280,88,44,148,1125,45,4,5,36,30},  //1080p30
    {93,3840,2160,0,5500,1660,1276,88,296,2250,90,8,10,72,24}, //4k24
    {94,3840,2160,0,5280,1440,1056,88,296,2250,90,8,10,72,25}, //4k25
    {95,3840,2160,0,4400,560,176,88,296,2250,90,8,10,72,30},  //4k30
    {98,4096,2160,0,5500,1404,1020,88,296,2250,90,8,10,72,24}, //4k24 smpte
    {99,4096,2160,0,5280,1184,968,88,128,2250,90,8,10,72,25}, //4k25 smpte
    {100,4096,2160,0,4400,304,88,88,128,2250,90,8,10,72,30}, //4k30 smpte
    {17,720,576,0,864,144,12,64,68,625,49,5,5,39,50},      //576p50
    {19,1280,720,0,1980,700,440,40,220,750,30,5,5,20,50},    //720p50
    {20,1920,1080,1,2640,720,528,44,148,1125,23,2,5,15,50},  //1080i50
    {21,720,576,1,864,288,24,126,138,625,25,2,3,19,50},      //576i50
    {31,1920,1080,0,2640,720,528,44,148,1125,45,4,5,36,50},  //1080p50
    {39,1920,1080,1,2304,384,32,168,184,1250,85,23,5,57,50},  //1080i50
    {96,3840,2160,0,5280,1440,1056,88,296,2250,90,8,10,72,50}, //4k50
    {101,4096,2160,0,5280,1184,968,88,128,2250,90,8,10,72,50},//4k50 smpte
    {1,640,480,0,800,160,16,96,48,525,45,10,2,33,60},       //640x480@60
    {2,720,480,0,858,138,16,62,60,525,45,9,6,30,60},       //480p60
    {4,1280,720,0,1650,370,110,40,220,750,30,5,5,20,60},     //720p60
    {5,1920,1080,1,2200,280,88,44,148,1125,23,2,5,15,60},   //1080i60
    {6,720,480,1,858,138,38,124,114,525,45,4,3,15,60},       //480i60
    {16,1920,1080,0,2200,280,88,44,148,1125,45,4,5,36,60},  //1080p60
    {97,3840,2160,0,4400,560,176,88,296,2250,90,8,10,72,60},  //4k60
    {102,4096,2160,0,4400,304,88,88,128,2250,90,8,10,72,60}, //4k smpte
    {240,3840,2160,0,4400,560,176,88,296,2250,90,7,10,73,30}, //3840x2160@30 (tmall timing)
    {241,3840,2160,0,5500,1660,1020,88,296,2250,90,7,10,73,24}, //3840x2160@24 (tmall timing)
    {242,3840,2160,0,5280,1440,1056,88,296,2250,90,7,10,73,25}, //3840x2160@25 (tmall timing)
    {243,4096,2160,0,5500,1404,1020,88,296,2250,90,7,10,73,24}, //4096x2160@30 (tmall timing)
    {255,255,255,0, 0,0,   0,0,0,  0,0,   0,0,0,   0}
};
/************************ Cea Timing Ends Here  ************************/

/************************ Vesa Timing Starts Here ************************/
/*
  AvVesaTables: Vic,VTotal,VActive,VBlank,VFront,VSync,VBack,HTotal,HActive,HBlank,HFront,HSync,HBack
*/

const AvVesaTables VesaTimingTables[] = {
    {220,1235,1200,35,3,6,26,2080,1920,160,48,32,80},    // 1920x1200 reduced blanking
    {221,1245,1200,45,3,6,36,2592,1920,672,136,200,336}, // 1920x1200
    {222,1080,1050,30,3,6,21,1840,1680,160,48,32,80},    // 1680x1050 reduced blanking
    {223,1089,1050,39,3,6,30,2240,1680,560,104,176,280}, // 1680x1050
    {224,1250,1200,50,1,3,46,2160,1600,560,64,192,304},  // 1600x1200
    {225,1000,900,100,1,3,96,1800,1600,200,24,80,96},    // 1600x900  reduced blanking
    {226,926,900,26,3,6,17,1600,1440,160,48,32,80},      // 1440x900  reduced blanking
    {227,934,900,34,3,6,25,1904,1440,464,80,152,232},    // 1440x900
    {228,1080,1050,30,3,4,23,1560,1400,160,48,32,80},    // 1400x1050 reduced blanking
    {229,1089,1050,39,3,4,32,1864,1400,464,88,144,232},  // 1400x1050
    {230,800,768,32,1,3,28,1500,1366,134,14,56,64},      // 1366x768  reduced blanking
    {231,798,768,30,3,3,24,1792,1366,426,70,143,213},    // 1366x768
    {232,795,768,27,3,6,18,1792,1360,432,64,112,256},    // 1360x768
    {233,1066,1024,42,1,3,38,1688,1280,408,48,112,248},  // 1280x1024
    {234,823,800,23,3,6,14,1440,1280,160,48,32,80},      // 1280x800  reduced blanking
    {235,831,800,31,3,6,22,1680,1280,400,72,128,200},    // 1280x800
    {236,790,768,22,3,7,12,1440,1280,160,48,32,80},      // 1280x768  reduced blanking
    {237,798,768,30,3,7,20,1664,1280,384,64,128,192},    // 1280x768
    //{19 ,750,720,30,5,5,20,1980,1280,700,440,40,220},    // 1280x720@60Hz
    //{4  ,750,720,30,5,5,20,1650,1280,370,110,40,220},    // 1280x720@50Hz
    {238,806,768,38,3,6,29,1344,1024,320,24,136,160},    // 1024x768
    {239,1111,1080,31,3,5,23,2080,1920,160,48,32,80},    // 1920x1080 reduced blanking
    {244,1493,1440,53,3,5,45,3488,2560,928,192,272,464}, // 2560x1440
    {245,1493,1440,53,3,10,40,4688,3440,1248,256,368,624}, // 3440x1440
    {246,1658,1600,58,3,10,45,4704,3440,1264,256,376,632}, // 3440x1600
    {247,1658,1600,58,3,6,49,3504,2560,944,192,280,472}, // 2560x1600
    {248,1592,1536,56,3,4,49,2800,2048,752,152,224,376}, // 2048x1536
    {249,1120,1080,40,3,10,27,2736,2048,688,128,216,344}, // 2048x1080
    {250,1493,1440,53,3,4,46,2608,1920,688,136,208,344}, // 1920x1440
    {255,255,255,0 ,0,0, 0,   0,   0,  0, 0,  0,  0}
};

/************************  Vesa Timing Ends Here  ************************/

const AvDscPlanVicTables DscPlanVicTable[] =
{
    /* 4K Timing */
    {93,  93,  AV_DSC_4BY1}, /* 4K@24Hz */
    {98,  98,  AV_DSC_4BY1}, /* 4K SMPTE@24Hz */
    {97,  97,  AV_DSC_8BY1}, /* 4K@60Hz, 600MHz */
    {107, 97,  AV_DSC_8BY1}, /* 4K@60Hz, 600MHz */
    {95,  95,  AV_DSC_4BY1}, /* 4K@30Hz */
    {100, 100, AV_DSC_4BY1}, /* 4K SMPTE@30Hz */
    {102, 102, AV_DSC_4BY1}, /* 4096x2160@60Hz, 600MHz, 2 only */
    {94,  94,  AV_DSC_4BY1}, /* 4K@25Hz */
    {96,  96,  AV_DSC_8BY1}, /* 4K@50Hz, 600MHz */
    {106, 96,  AV_DSC_8BY1}, /* 4K@50Hz, 600MHz */
    {99,  99,  AV_DSC_4BY1}, /* 4K SMPTE@25Hz */
    {101, 101, AV_DSC_8BY1}, /* 4096x2160@50Hz, 600MHz */
    /* CEA Timing */
    {60,  60,  AV_DSC_1BY1}, /* 720p@24Hz, 59MHz */
    {65,  60,  AV_DSC_1BY1}, /* 720p@24Hz, 59MHz */
    {61,  61,  AV_DSC_1BY1}, /* 720p@25Hz, 74MHz */
    {66,  61,  AV_DSC_1BY1}, /* 720p@25Hz, 74MHz */
    {62,  62,  AV_DSC_1BY1}, /* 720p@30Hz, 74MHz */
    {67,  62,  AV_DSC_1BY1}, /* 720p@30Hz, 74MHz */
    {19,  19,  AV_DSC_1BY1}, /* 720p@50Hz,  74MHz */
    {68,  19,  AV_DSC_1BY1}, /* 720p@50Hz,  74MHz */
    {4,   4,   AV_DSC_1BY1}, /* 720p@60Hz,  74MHz */
    {69,  4,   AV_DSC_1BY1}, /* 720p@60Hz,  74MHz */
    {32,  32,  AV_DSC_1BY1}, /* 1080p@24Hz, 74MHz */
    {72,  32,  AV_DSC_1BY1}, /* 1080p@24Hz, 74MHz */
    {33,  31,  AV_DSC_1BY1}, /* 1080p@25Hz, 74MHz */
    {73,  31,  AV_DSC_1BY1}, /* 1080p@25Hz, 74MHz */
    {34,  16,  AV_DSC_1BY1}, /* 1080p@30Hz, 74MHz */
    {74,  16,  AV_DSC_1BY1}, /* 1080p@30Hz, 74MHz */
    {31,  31,  AV_CSC_2BY1}, /* 1080p@50Hz, 148MHz */
    {75,  31,  AV_CSC_2BY1}, /* 1080p@50Hz, 148MHz */
    {16,  16,  AV_CSC_2BY1}, /* 1080p@60Hz, 148MHz */
    {76,  16,  AV_CSC_2BY1}, /* 1080p@60Hz, 148MHz */
    /* VESA Timing */
    {220, 220, AV_CSC_2BY1}, // 1920x1200@60Hz (reduced blanking), 2
    {221, 221, AV_CSC_2BY1}, // 1920x1200@60Hz, 2/3/4
    {222, 222, AV_CSC_2BY1}, // 1680x1050@60Hz (reduced blanking), 2
    {223, 223, AV_CSC_2BY1}, // 1680x1050@60Hz, 2/3/4
    {224, 224, AV_CSC_2BY1}, // 1600x1200@60Hz, 2/3/4
    {225, 225, AV_CSC_2BY1}, // 1600x900@60Hz (reduced blanking), 2/3
    {226, 226, AV_CSC_2BY1}, // 1440x900@60Hz (reduced blanking), 2
    {227, 227, AV_CSC_2BY1}, // 1440x900@60Hz, 2/4
    {228, 228, AV_CSC_2BY1}, // 1400x1050@60Hz (reduced blanking), 2
    {229, 229, AV_CSC_2BY1}, // 1400x1050@60Hz, 2/3/4
    {232, 232, AV_CSC_2BY1}, // 1360x768@60Hz, 2/3/4
    {233, 233, AV_CSC_2BY1}, // 1280x1024@60Hz, 2/4
    {234, 234, AV_CSC_2BY1}, // 1280x800@60Hz (reduced blanking), 2
    {235, 235, AV_CSC_2BY1}, // 1280x800@60Hz, 2/3/4
    {236, 236, AV_CSC_2BY1}, // 1280x768@60Hz (reduced blanking), 2
    {237, 237, AV_CSC_2BY1}, // 1280x768@60Hz, 2/4
    {238, 238, AV_CSC_2BY1}, // 1024x768@60Hz, 2/4
    {239, 239, AV_CSC_2BY1}, // 1920x1080@60Hz (reduced blanking), 2
    {240, 240, AV_DSC_4BY1}, // 4K@30Hz (reduced blanking), 2
    {241, 241, AV_DSC_4BY1}, // 4K@24Hz (reduced blanking), 2
    {242, 242, AV_DSC_4BY1}, // 4K@25Hz (reduced blanking), 2
    {243, 243, AV_DSC_4BY1}, // 4K@24Hz (reduced blanking), 2
    {244, 244, AV_DSC_4BY1}, // 2560x1440
    {245, 245, AV_DSC_4BY1}, // 3440x1440
    {246, 246, AV_DSC_4BY1}, // 3440x1600
    {247, 247, AV_DSC_4BY1}, // 2560x1600
    {248, 248, AV_DSC_4BY1}, // 2048x1536
    {249, 249, AV_DSC_4BY1}, // 2048x1080
    {250, 250, AV_DSC_4BY1}, // 1920x1440
    /* Default Setting */
    {255, 255, AV_CSC_2BY1}  // default bypass
};
#endif

#endif

