///*****************************************
//  Copyright (C) 2009-2019
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iTE6615_Config.h>
//   @author Kuro.Chung@ite.com.tw
//   @date   2019/06/05
//   @fileversion: iTE6615_MCUSRC_1.03
//******************************************/
#ifndef _iTE6615_CONFIG_H_
#define _iTE6615_CONFIG_H_
#include "Datatype.h"
//#ifndef TRUE
//#define TRUE 1
//#endif
//#ifndef FALSE
//#define FALSE 0
//#endif

// EVB Option //
#ifndef _MCU_8051_EVB_
#define _MCU_8051_EVB_      ITE_FALSE//TRUE // if not ITE EVB, do not set this.
#endif

// COMMON iTE6264/iTE6615 ENABLE //
#ifndef iTE6615
    #define iTE6615 1  // iTE6615
#endif
#ifndef iTE6264
    #define iTE6264 0  // iTE6264
#endif

// AVIInfoframe config by array or content of config.h, If you are using bypass ColorSpace, you may need this
// Please find the iTE6615_DATA.Infoframe.AVI[] array in iTE6615_SYS_FLOW.c for fill infoframe content
#ifndef _AVIInfoframe_Setting_By_Array_
#define _AVIInfoframe_Setting_By_Array_ 0
#endif

// COMMON INPUT CONFIG //
#define _Input_ColorSpace_              ITE6615_YUV422//ITE6615_RGB444              // ITE6615_RGB444, ITE6615_YUV422, ITE6615_YUV444, ITE6615_YUV420
#define _Input_YCC_Colorimetry_         ITE6615_ITU709              // ITU601, ITE6615_ITU709 , dont care If input is RGB, RGB auto set this to 0 (No Data)
#define _Input_RGB_QuantizationRange_   RGB_RANGE_FULL      // RGB_RANGE_LIMIT, RGB_RANGE_FULL
#define _Input_Video_AspectRatio_       AR_16_9//AR_4_3              // AR_4_3, AR_16_9, AR_14_9

// COMMON OUTPUT CONFIG //
#define _Output_ColorSpace_             ITE6615_RGB444//ITE6615_YUV444          // ITE6615_RGB444, ITE6615_YUV422, ITE6615_YUV444, ITE6615_YUV420 ( only if the input color mode is also ITE6615_YUV420 )
#define _Output_ColorDepth_             VID8BIT         // VID8BIT, VID10BIT, VID12BIT
#define _Output_YCC_Colorimetry_        ITE6615_ITU709          // ITU601, ITE6615_ITU709 , dont care If output is RGB, or can be set to 0 (No Data)
#define _Output_YUV_QuantizationRange_  YUV_RANGE_LIMIT // YUV_RANGE_LIMIT, YUV_RANGE_FULL
#define _Output_RGB_QuantizationRange_  RGB_RANGE_FULL	// RGB_RANGE_FULL, RGB_RANGE_LIMIT

// HDMI AVI Infoframe - Extended Colorimetry CONFIG
#define _Output_ExtendedColorimetry_Enable_ 0           // 1 = Enable, 0 = Disable
#define _Output_ExtendedColorimetry_Format_ ITE6615_BT2020cYCC  // ITE6615_AdobeRGB ITE6615_BT2020cYCC ITE6615_BT2020YCC
                                                        // ITE6615_xvYCC601 ITE6615_xvYCC709 ITE6615_sYCC601 ITE6615_AdobeYCC601,

// COMMON INPUT DATA PROCESSING CONFIG
#define _Input_Enable_DualMode_         1 //0              // TTL/LVDS Dual Input = 1,  TTL/LVDS Single Input = 0
#define _Input_Enable_DualMode_Mode_    MODE_ODDEVEN    // MODE_ODDEVEN/MODE_LEFTRIGHT
#define _Input_Enable_HL_Swap_          0               // Input L/H swap function, !!!single-in only for "Low"!!! can't swap by register.

// 6615 TTL INPUT CONFIG
#define _TTL_Enable_DDR_        0   // 1 : Input Enable DDR
#define _TTL_Enable_HalfBus_    0   // 1 : Input Enable Half-Bus
#define _TTL_Enable_BTAFormat_  0   // 1 : Input Enable YCbCr422 BTA-T1004 format
#define _TTL_Enable_SyncEmbedd_ 0   // 1 : Input Enable Sync Embedded (sw control enable runtime)
#define _TTL_Enable_InputYUV422DataRounding_	0	// 1 : Input Enable YUV422 data do rounding if 8b/10b
#define _TTL_Default_YUV422ReceiveBits_	VID12BIT	// YUV422 always getting 12bit data from TTL bus, others by output color depth, you could define TTL Bus bits for 422 here
#define _TTL_Default_RegCtrlLSel_		0			// 0 : DEL/HSyncL/VSyncL select from pixL else from pixH, default 0 (TTL Low Bus DE/Sync come from Low)
#define _TTL_Default_RegCtrlHSel_		1			// 0 : DEH/HSyncH/VSyncH select from pixL else from pixH, default 1 (TTL High Bus DE/Sync come from High)

// this config for enable 6615 embedded-sync functions,
// setting to 0 if customer never use sync-embedded for reduce code size
#ifndef _ENABLE_6615_SYNC_EMBEDDED_FUNCTIONS_
#define _ENABLE_6615_SYNC_EMBEDDED_FUNCTIONS_    0//   1
#endif
#define _TTL_INPUT_LMSwap_      0
#define _TTL_INPUT_YCSwap_      0
#define _TTL_INPUT_RBSwap_      0
#define _TTL_INPUT_PackSwap_    0

/********TTL Clock Latch, Delay/Invert CONFIG************/
// 6615 TTL Clock Latch, Delay/Invert CONFIG
// 6615 ChipID <= 0xF0 Config
#define TTL_EnICLKHInv  0   // 0,1 for input clk invert
#define TTL_EnICLKLInv  0   // 0,1 for input clk invert
#define TTL_ICLKHDly    0   // 0...3 for input clk delay
#define TTL_ICLKLDly    0   // 0...3 for input clk delay

// New version chip edit Clock Delay bits from 3bits to 4bits
// 6615 ChipID >= 0x10 (IT6264G0) Config
#define _0x10_TTL_EnICLKHInv  0   // 0,1 for input clk invert
#define _0x10_TTL_EnICLKLInv  0   // 0,1 for input clk invert
#define _0x10_TTL_ICLKHDly    0   // 0...7 for input clk delay
#define _0x10_TTL_ICLKLDly    0   // 0...7 for input clk delay
/********************************************************/

// 6264 LVDS INPUT CONFIG
#define _LVDS_RxEnDeSSC_    0           // De-SSC function
#define _LVDS_RxSkewDis_    0           // De-Skew function in DCABL mode
#define _LVDS_RxColDep_     1           // 0: 6-bit, 1: 8-bit, 2: 10-bit
#define _LVDS_RxOutMap_     0           // 0: JEIDA, 1: VESA
#define _LVDS_RxInSwap_     0           // Enable LvRx Ch0/1 swap, Ch2/3 swap mode
#define _LVDS_RxStbDENum_   8           // Stable DE numbr

// LVDS Clock Latch, Delay & Invert
#define EnSSCLPCLKInv   0   // 0/1
#define EnSSCHPCLKInv   0   // 0/1
#define SSCLPCLKDly     0   // 0..3
#define SSCHPCLKDly     0   // 0..3

// Audio CONFIG
#define AUDIO_ENABLE    1       // 1 : Enable Audio
#define AUDIO_CHANNELS  2       // channel count
#define AUDIO_FREQ      AUD48K  // Output Audio frequency
#define AUDIO_TYPE      LPCM    // LPCM, NLPCM, DSD, HBR
#define AUDIO_SELECT    SPDIF   //I2S     // I2S or SPDIF
    // I2S Option
    #define I2S_WORD_LENGTH         3       //  0: 16 bits, 1: 18 bits, 2: 20 bits, 3: 24 bits
    #define I2S_FMT_0               1       //  0: standard I2s, 1 : 32-bit I2s
    #define I2S_FMT_1               0       //  0: Left-justified, 1: Right-justified
    #define I2S_FMT_2               0       //  0: Data delay 1T correspond to WS, // 1: No data delay correspond to WS
    #define I2S_FMT_3               0       //  0: WS=0 is left channel // 1: WS=0 is right channel
    #define I2S_FMT_4               0       //  0: MSB shift first, 1: LSB shift first
    // SPDIF Option
    #define SPDIF_SCHMITT_TRIGGER   0       //  SPDIF Schmitt trigger 0:disable, 1:enable
    // Audio Special Mode Config, 1 : Enable
    #define _Audio_Enable_SPDIF_OSMode_             0
    #define _Audio_Enable_TDMAudio_                 0
    #define _Audio_Enable_TDMAudio_ChannelCount_    0
    #define _Audio_Enable_3DAudio_                  0
    #define _Audio_Enable_MSAudio_                  0
    #define _Audio_Enable_MCLKSamplingMode_         0

// AVI Infoframe IT Content Type
#define _Infoframe_ITContent_ 0
#define _Infoframe_ITContent_Type_ 0 // 0 = Graphics, 1 = Photo, 2 = Cinema, 3 = Game

// InfoFrame : Output Dynamic Range and Mastering (HDR) Option
// Infoframe content please find global variable iTE6615_DATA.Infoframe.DRM for fill it
// And use iTE_6615Customer_Set_DRM_PRK(u8 Enable) for enable/disable in runtime
#define _Infoframe_Enable_DRM_  0   // Enable Ouput DRM Infoframe
#define _Infoframe_DRM_ZERO_    0   // 1 : Setting all DRM PB to 0

// Infoframe : Source Product Description Infoframe Option
// Infoframe content please find global variable iTE6615_DATA.Infoframe.SPD in for fill it
#define _Infoframe_Enable_SPD_  0   // Enable Ouput SPD Infoframe

// InfoFrame : 3D Infoframe Setting
#define _Infoframe_Enable3D_                        0   // 1 : Enable 3D Infoframe for 3D format output (Default VSIF 1.4)
#define _Infoframe_Enable3D_DualView_               0   // 1 : Enable 3D Dual-View Signaling (not real 3D) (VSIF 2.x)
#define _Infoframe_Enable3D_DisparityData_present_  0   // 1 : Enable 3D OSD Disparity Indication (the depth informataion for real 3D) (VSIF 2.x)
#define _Infoframe_Enable3D_ViewDependency_         0   // 1 : Enable 3D Independent View Signaling (the dependency information for real 3D) (VSIF 2.x)
#define _Infoframe_3D_Structure_    _3D_FramePacking    // _3D_FramePacking, _3D_SideBySide_Full, _3D_TopBottom, _3D_SideBySide_Half

// HDCP
#define _HDCP_Enable_               1   // 1 : HDCP Enable
#define _HDCP_Enable_RepeaterMode_  0   // 1 : HDCP SUPPORT DOWNSTREAM IS REPEATER

#ifndef	_HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_
#define _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_	0	// 1 : enable 6615 as repeater as repeater code, if you dont care code size, setting to 1
#endif

#ifndef _HDCP_Enable_iTE6615_AS_HDCPRepeater_
#define _HDCP_Enable_iTE6615_AS_HDCPRepeater_		0	// 1 : enable 6615 as repeater as repeater, you must enable _HDCP_Enable_iTE6615_AS_HDCPRepeater_Code_ then enable this.
#endif

#ifndef _HDCP_KSVFifo_Size_
#define _HDCP_KSVFifo_Size_			50	// KSV device size, 50/5 = 10 = max support ksv list device
#endif

// for enable 6805 call back function or not
#ifndef _Enable_6805_CB_
#define _Enable_6805_CB_ FALSE
#endif

// Enable Could receive VFrontPorchChange Option
#ifndef _Enable_VFrontProchChange_      // 1 = could receive V Front change Timing, 0 = receive normal timing (default)
#define _Enable_VFrontProchChange_  0   // VFrontPorchChange have two limitation,
#endif                                  // 1. Dual input mode can't be left-right mode
                                        // 2. YUV420 couldn't convert to other color space

#define ENABLE_HPD_POLLING          1   // This define for reduce HPD reponse time for HDCP2.2 Comliance Test
                                        // Because the timing of 6615 between HPD low (100ms when compliance test)to Unplug stage is very critical when Compliance Test
										// If not in time reset 6615 and re-auth HDCP2.2 then fail
										// If your system is single-thread-multi-chips, the 6615 time slice may not enough to reponse HPD-LOW
										// The timing between HPD low to change sys to Unplug should be as soon as possible
										// and 6615 hardware have HPD de-glitch, HPD INT occur when HPD LOW 50~75ms so delay to Unplug stage more
										// If your system is single-thread-multi-chips (like 6805+6615 MCU sample code) and you need HDCP2.2 Compliance Test, setting to 1
										// or If your system is multi-thread-os-like platform, you should concern polling 6615 must polling every 50ms per time, setting to 0
#define ENABLE_HDCP22_SWITCH        ITE_TRUE
#define ENABLE_OUTPUT_HDCP_SWITCH   ITE_TRUE
#define ENABLE_OUTPUT_HDCP22_FOR_NON_4K   ITE_TRUE

// other
#define _ENABLE_EDID_PARSING_DETAIL_    ITE_FALSE  // for parsing EDID detail and show EDID log, default FALSE

// CEC Config //
#define _ENABLE_ITE6615_CEC_        ITE_FALSE   // code not ready yet
#define _ENABLE_CEC_I2C_    ITE_TRUE
#define CEC_VENDOR_ID       0x0000
#define iTE6615CECGPIOid    1    // for identife iTE6805_CEC use Pin 1.0 & 1.1

//////////////////////////////////////////////////////////////////////////////////////////
// Other Config
//////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_HDMITX 0x98
#define ADDR_LVDSRX 0x2C
#define ADDR_EDID   0xA8    //Software programmable I2C address of iTE6805 EDID RAM
#define ADDR6615_CEC    0x9C    //Software programmable I2C address of iTE6805 CEC

#ifndef _ENABLE_OUTPUT_SUPPORT_HDMI20
#define _ENABLE_OUTPUT_SUPPORT_HDMI20   ITE_FALSE
#endif
#define _ENABLE_AUTO_SET_VIC            ITE_FALSE
#define _ENABLE_4K_FORCE_SET_VIC_0      ITE_FALSE

#define _ENABLE_SKIP_NO_AUDIO_INTERRUPT ITE_TRUE

//////////////////////////////////////////////////////////////////////////////////////////
// Other
//////////////////////////////////////////////////////////////////////////////////////////

// hardware RD option
#define config_DisR0ShortRead   1
#define config_DisRiShortRead   1
#define config_LClimit          0
#define config_HDCP_EnableAutoMute 1    // 6265 set to 1, RD set to 0
#define config_EnAutoReAuth     0
#define config_KeepOutOpt       1
#define config_EnSyncDetChk     0
#define config_HDCP_Enable1p1   1         // RD different to code
#define config_EnSyncDet2FailInt    0
#define config_EnRiChk2DoneInt  0
#define config_RecChStSel       0
#define config_SCDCFlagByte     0

#if _ENABLE_FOURCE_ON_IT6615_STATE
#define config_ForceRxOn        1
#define config_ForceVOut        1
#define config_ForceTMDSStb     1
#else
#define config_ForceRxOn        0
#define config_ForceVOut        0
#define config_ForceTMDSStb     0
#endif

#define config_VidLatEdge       0
#define config_AudLatEdge       0

#define config_DisH2Auto        1       // add by wf8421 20190225

#define config_EnDDRInAutoSwap  1
#define config_InSyncAutoSwapOff 0
#define config_EnHDCP2MsgPoll10ms 0
#define config_LvRxDeSSCCLKSel 1
#define config_EnHDCP2TxPollRdyOnly 0   //TRUE: wait RxKSVListRdy when (MSGSize not 0 && Ready=1) for HDCP2Tx CTS 1B-02
// message when complier
#if (_MCU_8051_EVB_==ITE_TRUE)
#if ((!iTE6264)&&(!iTE6615))
//#error message("no define iTE6264 and iTE6615")
#endif
#if (iTE6264)
//#pragma message("Defined for iTE6264")
#endif
#if (iTE6615)
//#pragma message("Defined for iTE6615")
#endif
#endif


#endif // _CONFIG_H_
