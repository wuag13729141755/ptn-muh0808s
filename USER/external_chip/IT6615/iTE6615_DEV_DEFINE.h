///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_DEV_DEFINE.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/


#ifndef _iTE6615_DEV_DEFINE_
#define _iTE6615_DEV_DEFINE_

#define RCLK_TYPICAL 9000

#define MODE_ODDEVEN 0
#define MODE_LEFTRIGHT 1

#define IT6615_HDCP_INIT		0
#define IT6615_HDCP_Success 	1
#define IT6615_HDCP_Fail    	2

#define HIGH           1
#define LOW            0

#define POWER_ON    1
#define POWER_OFF   0

#define LVDS    0
#define TTL     1

#define LVDS1CH            0
#define LVDS2CH            1
#define LVDS4CH            2

#define MAX_PORT_NUM    2
#define SDR 0
#define DDR 1

#define HPD_LOW     0
#define HPD_HIGH    1
#define ITE6615_ENABLE      1
#define ITE6615_DISABLE     0
#define VIDEO_ON    1
#define VIDEO_OFF   0
#define AUDIO_ON    1
#define AUDIO_OFF   0
#define AVMUTE_ON   1
#define AVMUTE_OFF  0

#define MODE_HDMI   0x00
#define MODE_MHL    0x01
#define MODE_DVI    0x02

#define MODE_5V_ON      1
#define MODE_5V_OFF     0
#define TRISTATE_ON     1
#define TRISTATE_OFF    0
#define HDCP_ENABLE     1
#define HDCP_DISABLE    0

#define MODE_POWER_STANDBY  2
#define MODE_POWER_SAVING   1
#define MODE_POWER_NORMAL   0

//#define RAP_CONTENT_OFF     0
//#define RAP_CONTENT_ON      1

#define TMDSCLKVALUE_480P   35
#define TMDSCLKVALUE_1080P  160    // for TMDS > 160 then set RS to 00, otherwise set to 3F


#ifndef BIT0
#define BIT0 0x01
#endif

#ifndef BIT1
#define BIT1 0x02
#endif

#ifndef BIT2
#define BIT2 0x04
#endif

#ifndef BIT3
#define BIT3 0x08
#endif

#ifndef BIT4
#define BIT4 0x10
#endif

#ifndef BIT5
#define BIT5 0x20
#endif

#ifndef BIT6
#define BIT6 0x40
#endif

#ifndef BIT7
#define BIT7 0x80
#endif

#define ITE6615_RGB444    0
#define ITE6615_YUV422    1
#define ITE6615_YUV444    2
#define ITE6615_YUV420    3

#define ITE6615_xvYCC601        0x00
#define ITE6615_xvYCC709        0x10
#define ITE6615_sYCC601         0x20
#define ITE6615_AdobeYCC601     0x30
#define ITE6615_AdobeRGB        0x40
#define ITE6615_BT2020cYCC      0x50
#define ITE6615_BT2020YCC       0x60

#define RGB_RANGE_LIMIT  0
#define RGB_RANGE_FULL   1
#define YUV_RANGE_LIMIT  0
#define YUV_RANGE_FULL   1

#if 0
#define VID8BIT         0
#define VID10BIT        1
#define VID12BIT        2
#define VID16BIT        3
#endif

#if 0
#define AUD32K          0x03
#define AUD44K          0x00
#define AUD48K          0x02
#define AUD64K          0x0B
#define AUD88K          0x08
#define AUD96K          0x0A
#define AUD128K         0x2B
#define AUD176K         0x0C
#define AUD192K         0x0E
#define AUD256K         0x1B
#define AUD352K         0x0D
#define AUD384K         0x05
#define AUD512K         0x3B
#define AUD705K         0x2D
#define AUD768K         0x09
#define AUD1024K        0x35
#define AUD1411K        0x1D
#define AUD1536K        0x15

#define TDM2CH          0x0
#define TDM4CH          0x1
#define TDM6CH          0x2
#define TDM8CH          0x3

#define DynVESA 0
#define DynCEA  1

//#define ITU601  0
//#define ITU709  1
#endif
#define ITE6615_ITU601  0x40
#define ITE6615_ITU709  0x80

#define JEIDA   0
#define VESA    1

#define LVDS6BIT    0x00
#define LVDS8BIT    BIT2
#define LVDS10BIT   BIT3

#if 0
#define I2S         0
#define SPDIF       1
#endif

#if 0
#define LPCM        0
#define NLPCM       1
#define HBR         2
#define DSD         3

#define TDM2CH      0x0
#define TDM4CH      0x1
#define TDM6CH      0x2
#define TDM8CH      0x3

#define NOCSC       0
#define RGB2YUV     2
#define YUV2RGB     3
#endif

#define _3D_FramePacking    0
#define _3D_SideBySide_Full 3
#define _3D_TopBottom       6
#define _3D_SideBySide_Half 8

#if 0
#define DDC75K      0
#define DDC125K     1
#define DDC312K     2
#endif

#define AUD16BIT    0x2
#define AUD18BIT    0x4
#define AUD20BIT    0x3
#define AUD24BIT    0xB

#define AUDCAL1     0x4
#define AUDCAL2     0x0
#define AUDCAL3     0x8

//#define abs(x) (((x)>=0)?(x):(-(x)))
//#define PROG 1
//#define INTR 0
//#define Vneg 0
//#define Hneg 0
//#define Vpos 1
//#define Hpos 1

#define AR_4_3      0x09
#define AR_16_9     0x0A
#define AR_14_9     0x0B
#define AR_64_27    0x08
#define AR_256_135  0x08

typedef enum {
    iTE6615_STATECEC_None=0,
    iTE6615_STATECEC_ReadyToFire,
    iTE6615_STATECEC_TXFail,
    iTE6615_STATECEC_TXDone,
    iTE6615_STATECEC_RXCheck,
} iTE6615_STATEC_Type;

typedef enum iTE6615_STATES_Type {
    iTE6615_STATES_Unplug = 0,
    iTE6615_STATES_HPDOn,
    iTE6615_STATES_WaitRxSen,
    iTE6615_STATES_RxSenON,
} iTE6615_STATES_Type ;

typedef enum iTE6615_STATEV_Type {
    iTE6615_STATEV_Reset = 0,
    iTE6615_STATEV_WaitForVidIn,
    iTE6615_STATEV_VidInStable,
    iTE6615_STATEV_VidOutEnable,
} iTE6615_STATEV_Type ;

typedef enum iTE6615_STATEA_Type {
    iTE6615_STATEA_Reset = 0,
    iTE6615_STATEA_WaitForAudIn,
    iTE6615_STATEA_AudInStable,
} iTE6615_STATEA_Type ;

typedef enum STATEH_Type {
    STATEH_Reset = 0,
    STATEH_CPGoing,
    STATEH_Done,
    STATEH_Fail,

    #if ENABLE_OUTPUT_HDCP_SWITCH
    STATEH_Disable,     //  add by wf8421 20190301
    #endif
} STATEH_Type ;

typedef enum hdcp_cmd_type
{
    CMD_HDCP_NONE,
    CMD_HDCP_START,
    CMD_HDCP_AUTH_RETRY,
    CMD_HDCP_POLLING_BCAPS,
    CMD_HDCP_KSV_CHK,
}hdcp_cmd_type;

typedef enum avi_cmd_type
{
    AVICMD_NONE,
    AVICMD_WAIT_FOR_AUDIO,
    AVICMD_SYS_UNPLUG,
    AVICMD_SHOW_AUD_INFO,
    AVICMD_SHOW_VID_INFO,
}avi_cmd_type;



typedef struct {
    u16 HActive ;
    u16 VActive ;
    u16 HTotal ;
    u16 VTotal ;
    u32 PCLK ;
    u16 HFrontPorch ;
    u16 HSyncWidth ;
    u16 HBackPorch ;
    u16 VFrontPorch ;
    u16 VSyncWidth ;
    u16 VBackPorch ;
    u32 FrameRate ;
    u8 ScanMode ;
    u8 VPolarity ;
    u8 HPolarity ;
    u8 PixRpt;
    u8 PicAR;
    char *format;
    u16 VIC;
} iTE6615_VTiming, *piTE6615_VTiming ;


struct _sink_cap {
    u8 YCCQCap;
    u8 RGBQCap;
    u8 ExtColCap;
    u8 HDR_EOTF;
    u8 HDMIVer;
    u8 VSIF1_MAX_TMDS;
    u8 VSIF2_MAX_TMDS;
    u8 max_TMDS_clk;

    // support 420
    u8 yuv420_svd_or_cmdb;

    // 2.0
    u8 scdc_present;
    u8 rr_capable;
    u8 lte_340M_csc_scramble;
    u8 independent_view;
    u8 daul_view2;
    u8 osd_3d_disparity;
    u8 dc_48bit_420;
    u8 dc_36bit_420;
    u8 dc_30bit_420;

    u8 dc_48bit;
    u8 dc_36bit;
    u8 dc_30bit;
    u8 dc_y444;
};

// struct
// Data need to be Global, setting here

typedef struct _iTE6615_GlobalDATA_
{
    u8 ChipID;
    iTE6615_STATEC_Type STATEC;    // CEC
    iTE6615_STATEV_Type STATEV;
    iTE6615_STATEA_Type STATEA;
    STATEH_Type STATEH;
    iTE6615_STATES_Type STATES;


    u8 Flag_HDCP2x_Enable;
    u8 Flag_Force_Set_VIC_0;
    u8 Flag_SYS_Tick_Enable;
    u8 Flag_EDID_Parser_Ready;
    u8 Flag_HDCPFailResetTX;
    u8 DumpREG;

    u16 VIndex; // Video Table Index
    u32 PCLK;
    u32 RCLK;
    u32 ICLK;
    u32 VCLK;
    u32 TXCLK;
    u32 TEMP_TXCLK;
    u32 VCLK_expected;

    #if iTE6264
    u32 SSCPCLKL;
    u32 SSCPCLKH;
    #endif

    u16 HDCP2FailStatus;
    u8 Flag_Reset6615;  // for 6805+6615 reset 6615
    u8 Flag_Reset6805;  // for 6805+6615 reset 6805

    #if _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_
    u8 BStatus[2];
    u8 HDCP_Enable_iTE6615_AS_HDCPRepeater;
    u8 HDCP_Enable_HDCPVersion;
    _CB6615_HDCP_ CB_HDCP;
    #endif

    u8 data_buffer_is_EDID;
    u8 Flag_NoNeedEDID;
    u8 KSV_List[_HDCP_KSVFifo_Size_];

    u8 Info_HDCPBStatus[2];
    u8 Info_HDCPRxInfo[2];
    u8 Info_HDCPDownstreamBKSV[5];
    u8 Info_HDCPDownstreamCount;
    u8 Info_HDCPDownstreamDepth;
    u8 Info_HDCPStatus;
    struct {
        // PKT = HB 3bytes + DB 28bytes
        u8 AVI[5];  // DB1~DB5, PB1 = Y2 Y1 Y0 A0 B1 B0 S1 S0 in CEA SPEC
        u8 DRM[26]; // DB1~DB26
        u8 SPD[31]; // HB0~HB2 + DB0~DB27
    }Infoframe;

    struct {

        // HDMI/DVI
        u8 EnHDMI;

        // AVIInfoframe setting by array or not
        u8 AVIInfoframe_Setting_By_Array;

        // Input Setting
        u8 Input_ColorSpace;
        u8 Input_YCC_Colorimetry;
        u8 Input_RGB_QuantizationRange;
        u8 Input_Video_AspectRatio;

        // Output Setting
        u8 Output_ColorSpace;
        u8 Output_ColorDepth;
        u8 Output_YCC_Colorimetry;
        u8 Output_RGB_QuantizationRange;
        u8 Output_YUV_QuantizationRange;

        // Output Extend Colorimetry Setting
        u8 Output_ExtendedColorimetry_Enable;
        u8 Output_ExtendedColorimetry_Format;   // ITE6615_xvYCC601 ITE6615_xvYCC709 ITE6615_sYCC601 ITE6615_AdobeYCC601,
                                                // ITE6615_AdobeRGB ITE6615_BT2020cYCC ITE6615_BT2020YCC
        // IT contetn config of AVI Infoframe
        u8 Infoframe_ITContent;
        u8 Infoframe_ITContent_Type;

        // Input setting : TTL/LVDS Single/Dual  (Common config)
        u8 Input_Enable_DualMode;       // LVDS setting to 4 Channel will auto config to Dual
        u8 Input_Enable_DualMode_Mode;  // Input Mode 0: Even-Odd mode, 1: Left-Right Mode
        u8 Input_Enable_HL_Swap;        // Input L/H swap function

        // Input setting : TTL config   ----- 6615 config -----
        u8 TTL_Enable_DDR;
        u8 TTL_Enable_HalfBus;
        u8 TTL_Enable_BTAFormat;       // Input Enable YCbCr422 BTA-T1004 format
        u8 TTL_Enable_SyncEmbedd;      // Input Enable Sync Embedded
        u8 TTL_Default_YUV422ReceiveBits;

        // Input setting : LVDS config  ----- 6264 config -----
        u8 LVDS_RxChNum;    // LVDS1CH: 1-channel, LVDS2CH: 2-channel, LVDS4CH: 4-channel
        u8 LVDS_RxEnDeSSC;
        u8 LVDS_RxSkewDis;
        u8 LVDS_RxColDep;   // 0: 6-bit, 1: 8-bit, 2: 10-bit
        u8 LVDS_RxOutMap;   // 0: JEIDA, 1: VESA
        u8 LVDS_RxInSwap;
        u8 LVDS_RxStbDENum;

        // 3D Infoframe Option
        u8 Infoframe_Enable3D;                        // for VSIF 1.4
        u8 Infoframe_Enable3D_DualView;               // for VSIF 2.x
        u8 Infoframe_Enable3D_DisparityData_present;  // for VSIF 2.x
        u8 Infoframe_Enable3D_ViewDependency;         // for VSIF 2.x
        u8 Infoframe_3D_Structure;                    // 3D Structure

        // Dynamic Range and Mastering InfoFrame Option
        u8 Infoframe_Enable_DRM;
        u8 Infoframe_DRM_EOTF;
        u8 Infoframe_DRM_ZERO;

        // Source Product Description (SPD) Infoframe Option
        u8 Infoframe_Enable_SPD;

        // other mode
        u8 Enable_DEOnly_Mode;    // H2TX DEOnly Mode

        // Software Audio Option
        u8 Audio_Enable;
        u8 Audio_Type;
        u8 Audio_Frequency;
        u8 Audio_ChannelCount;
        // Hardward output setting
        u8 Audio_Output_Sel;        // SPDIF/I2S
        u8 Audio_I2C_HardwareFormat;
        // I2S option
        u8 Audio_I2S_WordLength;
        // Audio Special Mode Option
        u8 Audio_Enable_SPDIF_OSMode;       // SPDIF Over-Sampling Mode
        u8 Audio_Enable_TDMAudio;           // If TRUE, AudSel must be I2S and AudType cannot be DSD and EnAudGen must be TRUE for Internal AudGen
        u8 Audio_Enable_TDMAudio_ChannelCount;
        u8 Audio_Enable_3DAudio;            // only for LPCM and DSD when AudCh is more than 8
        u8 Audio_Enable_MSAudio;            // only for LPCM/NLPCM and DSD when AudCh is less than or equal to 8
        u8 Audio_Enable_MCLKSamplingMode;   // SPDIF MCLK-Sampling Mode

        // hdcp
        u8 HDCP_Enable;
        u8 HDCP_Enable_RepeaterMode;

        // V front porch change option
        u8 Enable_VFrontProchChange;

        u8 u8DDCSpeed ;

        u8 DisLockPR;   // TRUE: for ATC format 6,7,21,22
        u8 ManuallPR;
        u8 AutoVIC;
        u8 PixRpt;
        u8 EnH2InLBBypass;
    }config;

    struct _sink_cap sink;

      struct {
        u8 HDMI_VIC;

        u8 AFESPEED;

        u8 HDCP2Sel;
        u8 RxHDMIMode;
        u8 HDCPFireCnt;

        #if _ENABLE_ITE6615_CEC_
        u8 cec_time_unit;
        u8 phy_addrL;
        u8 phy_addrH;
        #endif

        u8 rx_key[5];

    }vars;

    struct
    {
        u8 sys_tick_inc;
        enum hdcp_cmd_type hdcp_cmd;
        u16 hdcp_cmd_count;
        u16 hdcp_cmd_tick;
        u16 hdcp_cmd_timeout;

        enum avi_cmd_type avi_cmd;
        u16 avi_cmd_tick;
        u16 avi_cmd_timeout;
    }task;

    u8 data_buffer[64+80*4]; // for RxID list, edid parsing and sha calc

} iTE6615_GlobalDATA;


#endif

