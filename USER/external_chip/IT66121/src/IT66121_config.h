///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <config.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2014/01/07
//   @fileversion: ITE_HDMITX_SAMPLE_3.17
//******************************************/
#ifndef _IT66121_CONFIG_H_
#define _IT66121_CONFIG_H_

//#include "IT66121_typedef.h"

#ifndef IT66121_DEV_NUM
#define IT66121_DEV_NUM             1
#endif

#ifndef IT66121_PIN_ADDR_LOW0
#define IT66121_PIN_ADDR_LOW0       1
#endif

#ifndef IT66121_PIN_ADDR_LOW1
#define IT66121_PIN_ADDR_LOW1       1
#endif

#ifndef IT66121_PIN_ADDR_LOW2
#define IT66121_PIN_ADDR_LOW2       1
#endif

#ifndef IT66121_PIN_ADDR_LOW3
#define IT66121_PIN_ADDR_LOW3       1
#endif

#define HDMITX_MAX_DEV_COUNT IT66121_DEV_NUM

#ifdef EXTERN_HDCPROM
#pragma message("Defined EXTERN_HDCPROM")
#endif // EXTERN_HDCPROM

#define SUPPORT_EDID
//#define SUPPORT_HDCP
//#define SUPPORT_SHA
//#define SUPPORT_AUDIO_MONITOR
#define AudioOutDelayCnt 250

// #define SUPPORT_CEC


//////////////////////////////////////////////////////////////////////////////////////////
// Video Configuration
//////////////////////////////////////////////////////////////////////////////////////////
// 2010/01/26 added a option to disable HDCP.
#define SUPPORT_OUTPUTYUV
#define SUPPORT_OUTPUTRGB
// #define DISABLE_HDMITX_CSC

#define SUPPORT_INPUTRGB
#define SUPPORT_INPUTYUV444
#define SUPPORT_INPUTYUV422
// #define SUPPORT_SYNCEMBEDDED
// #define SUPPORT_DEGEN



//#define DISABLE_HDMITX_CSC

//#define INPUT_COLOR_MODE F_MODE_RGB444
//#define INPUT_COLOR_MODE F_MODE_YUV422
//#define INPUT_COLOR_MODE F_MODE_YUV444

#ifndef INPUT_COLOR_MODE
#define INPUT_COLOR_MODE	    (F_MODE_YUV422|F_VIDMODE_ITU709|F_VIDMODE_0_255|F_VIDMODE_EN_UDFILT|F_VIDMODE_EN_DITHER ) //|F_VIDMODE_ITU709|F_VIDMODE_0_255
#endif

#define INPUT_A_COLOR_MODE_YUV444    	(F_MODE_YUV422) //F_MODE_YUV444

#ifndef INPUT_YCBCR422_DATA_BIT
#define INPUT_YCBCR422_DATA_BIT         16
#endif

#if (INPUT_YCBCR422_DATA_BIT == 16)
#define NON_SEQUENTIAL_YCBCR422
#endif

#ifndef INPUT_COLOR_DEPTH
#define INPUT_COLOR_DEPTH   24 		// 16//
#endif
// #define INPUT_COLOR_DEPTH 30
// #define INPUT_COLOR_DEPTH 36

#ifndef OUTPUT_COLOR_MODE
//#define OUTPUT_COLOR_MODE F_MODE_YUV422
//#define OUTPUT_COLOR_MODE F_MODE_YUV444
#define OUTPUT_COLOR_MODE F_MODE_RGB444
#endif

//#define OUTPUT_3D_MODE Frame_Pcaking
//#define OUTPUT_3D_MODE Top_and_Botton
//#define OUTPUT_3D_MODE Side_by_Side

// #define INV_INPUT_ACLK
// #define INV_INPUT_PCLK

#ifdef SUPPORT_SYNCEMBEDDED
    // #define INPUT_SIGNAL_TYPE (T_MODE_SYNCEMB)                 // 16 bit sync embedded
    // #define INPUT_SIGNAL_TYPE (T_MODE_SYNCEMB | T_MODE_CCIR656) // 8 bit sync embedded
    #define INPUT_SIGNAL_TYPE (T_MODE_SYNCEMB|T_MODE_INDDR|T_MODE_PCLKDIV2) // 16 bit sync embedded DDR
    // #define INPUT_SIGNAL_TYPE (T_MODE_SYNCEMB|T_MODE_INDDR)      // 8  bit sync embedded DDR

    #define SUPPORT_INPUTYUV422
    #ifdef INPUT_COLOR_MODE
    #undef INPUT_COLOR_MODE
    #endif // INPUT_COLOR_MODE
    #define INPUT_COLOR_MODE F_MODE_YUV422
#else

    #define INPUT_SIGNAL_TYPE 0 // 24 bit sync seperate
    //#define INPUT_SIGNAL_TYPE ( T_MODE_DEGEN_IT66121 )
    //#define INPUT_SIGNAL_TYPE ( T_MODE_INDDR)
    //#define INPUT_SIGNAL_TYPE ( T_MODE_SYNCEMB)
    //#define INPUT_SIGNAL_TYPE ( T_MODE_CCIR656 | T_MODE_SYNCEMB )
#endif


#if defined(SUPPORT_INPUTYUV444) || defined(SUPPORT_INPUTYUV422)
#define SUPPORT_INPUTYUV
#endif

#ifdef SUPPORT_SYNCEMBEDDED
#pragma message("defined SUPPORT_SYNCEMBEDDED for Sync Embedded timing input or CCIR656 input.")
#endif


//////////////////////////////////////////////////////////////////////////////////////////
// Audio Configuration
//////////////////////////////////////////////////////////////////////////////////////////

// #define SUPPORT_HBR_AUDIO
#define USE_SPDIF_CHSTAT
#ifndef SUPPORT_HBR_AUDIO
    #define INPUT_SAMPLE_FREQ AUDFS_48KHz
    #define INPUT_SAMPLE_FREQ_HZ 48000L
    #define OUTPUT_CHANNEL 2 // 3 // 4 // 5//6 //7 //8

    #define CNOFIG_INPUT_AUDIO_TYPE T_AUDIO_LPCM
    // #define CNOFIG_INPUT_AUDIO_TYPE T_AUDIO_NLPCM
    //#define CONFIG_INPUT_AUDIO_SPDIF FALSE // I2S
//     #define CONFIG_INPUT_AUDIO_SPDIF SPDIF // SPDIF

    // #define I2S_FORMAT 0x00 // 24bit I2S audio
    #define I2S_FORMAT 0x01 // 32bit I2S audio
    // #define I2S_FORMAT 0x02 // 24bit I2S audio, right justify
    // #define I2S_FORMAT 0x03 // 32bit I2S audio, right justify

#else // SUPPORT_HBR_AUDIO

    #define INPUT_SAMPLE_FREQ AUDFS_768KHz
    #define INPUT_SAMPLE_FREQ_HZ 768000L
    #define OUTPUT_CHANNEL 8
    #define CNOFIG_INPUT_AUDIO_TYPE T_AUDIO_HBR
//    #define CONFIG_INPUT_AUDIO_SPDIF I2S // I2S
    // #define CONFIG_INPUT_AUDIO_SPDIF TRUE // SPDIF
    #define I2S_FORMAT 0x47 // 32bit audio
#endif



//////////////////////////////////////////////////////////////////////////////////////////
// Audio Monitor Configuration
//////////////////////////////////////////////////////////////////////////////////////////

// #define HDMITX_AUTO_MONITOR_INPUT
// #define HDMITX_INPUT_INFO

#ifdef  HDMITX_AUTO_MONITOR_INPUT
#define HDMITX_INPUT_INFO
#endif

//////////////////////////////////////////////////////////////////////////////////////////
// Reduce Source Clock Jitter
//////////////////////////////////////////////////////////////////////////////////////////
 //#define REDUCE_HDMITX_SRC_JITTER
#endif
