/**
******************************************************************************
* @file    ms1826_timing_table.h
* @author  
* @version V1.0.0
* @date    
* @brief   input/output timing define declare
* @history     
*
* Copyright (c) 2009-2014, MacroSilicon Technology Co.,Ltd.
******************************************************************************/
#ifndef __MACROSILICON_MS1826_TIMING_TABLE_H__
#define __MACROSILICON_MS1826_TIMING_TABLE_H__

#include "ms_typedef.h"
#include "ms_hdmi.h"

typedef enum _E_SYNC_POLARITY_
{
   ProgrVNegHNeg = 0x01,
   ProgrVNegHPos = 0x03,
   ProgrVPosHNeg = 0x05,
   ProgrVPosHPos = 0x07,

   InterVNegHNeg = 0x00,
   InterVNegHPos = 0x02,
   InterVPosHNeg = 0x04,
   InterVPosHPos = 0x06
}SYNCPOLARITY_E;

typedef struct _T_MS1826_MISC_TIMING_
{
    UINT16                 u16_vic;      
    HDMI_VIDEO_TIMING_T   st_timing;
} MISCTIMING_T;

typedef enum _E_MS1826_VIDEO_FORMAT_
{
    VFMT_CEA_NULL                        = 0,   
    VFMT_CEA_01_640x480P_60HZ            = 1,   
    VFMT_CEA_02_720x480P_60HZ            = 2,  //has been include in timing table
    VFMT_CEA_03_720x480P_60HZ            = 3,   
    VFMT_CEA_04_1280x720P_60HZ           = 4,  //include 
    VFMT_CEA_05_1920x1080I_60HZ          = 5,  //include 
    VFMT_CEA_06_720x480I_60HZ            = 6,  //include 
    VFMT_CEA_07_720x480I_60HZ            = 7,   
    VFMT_CEA_08_720x240P_60HZ            = 8,   
    VFMT_CEA_09_720x240P_60HZ            = 9,   
    VFMT_CEA_10_720x480I_60HZ            = 10,  
    VFMT_CEA_11_720x480I_60HZ            = 11,  
    VFMT_CEA_12_720x240P_60HZ            = 12,  
    VFMT_CEA_13_720x240P_60HZ            = 13,  
    VFMT_CEA_14_1440x480P_60HZ           = 14,  
    VFMT_CEA_15_1440x480P_60HZ           = 15,  
    VFMT_CEA_16_1920x1080P_60HZ          = 16,  //include
    VFMT_CEA_17_720x576P_50HZ            = 17,  //include
    VFMT_CEA_18_720x576P_50HZ            = 18,  
    VFMT_CEA_19_1280x720P_50HZ           = 19,  //include
    VFMT_CEA_20_1920x1080I_50HZ          = 20,  //include
    VFMT_CEA_21_720x576I_50HZ            = 21,  //include
    VFMT_CEA_22_720x576I_50HZ            = 22,  
    VFMT_CEA_23_720x288P_50HZ            = 23,  
    VFMT_CEA_24_720x288P_50HZ            = 24,  
    VFMT_CEA_25_720x576I_50HZ            = 25,  
    VFMT_CEA_26_720x576I_50HZ            = 26,  
    VFMT_CEA_27_720x288P_50HZ            = 27,  
    VFMT_CEA_28_720x288P_50HZ            = 28,  
    VFMT_CEA_29_1440x576P_50HZ           = 29,  
    VFMT_CEA_30_1440x576P_50HZ           = 30,  
    VFMT_CEA_31_1920x1080P_50HZ          = 31, //include 
    VFMT_CEA_32_1920x1080P_24HZ          = 32, //include 
    VFMT_CEA_33_1920x1080P_25HZ          = 33, //include 
    VFMT_CEA_34_1920x1080P_30HZ          = 34, //include 
    VFMT_CEA_35_2880x480P_60HZ           = 35,  
    VFMT_CEA_36_2880x480P_60HZ           = 36,  
    VFMT_CEA_37_2880x576P_50HZ           = 37,  
    VFMT_CEA_38_2880x576P_50HZ           = 38,  
    VFMT_CEA_39_1920x1080I_50HZ          = 39, //include
    VFMT_CEA_60_1280x720P_24HZ           = 60,  
    VFMT_CEA_61_1280x720P_25HZ           = 61,  
    VFMT_CEA_62_1280x720P_30HZ           = 62,

    //                                    
    VFMT_VESA_64_640X480_60              = 64,
    VFMT_VESA_65_640X480_75              = 65,  
    VFMT_VESA_66_800X600_60              = 66,  
    VFMT_VESA_67_800X600_72              = 67,  
    VFMT_VESA_68_800X600_75              = 68,  
    VFMT_VESA_69_800X600_85              = 69,  
    VFMT_VESA_70_800X600_120_RB          = 70,  
    VFMT_VESA_71_1024X768_60             = 71,  
    VFMT_VESA_72_1024X768_70             = 72,  
    VFMT_VESA_73_1024X768_75             = 73,  
    VFMT_VESA_74_1024X768_85             = 74,  
    VFMT_VESA_75_1024X768_120_RB         = 75,  
    VFMT_VESA_76_1152X864_60             = 76,  
    VFMT_VESA_77_1152X864_75             = 77,  
    VFMT_VESA_78_1280X600_60             = 78,  
    VFMT_VESA_79_1280X720_60_DMT         = 79,             
    VFMT_VESA_80_1280X720_60_CVT         = 80,  
    VFMT_VESA_81_1280X720_60_CVT_RB      = 81,  
    VFMT_VESA_82_1280X720_75_CVT         = 82,  
    VFMT_VESA_83_1280X720_85_CVT         = 83,  
    VFMT_VESA_84_1280X768_60             = 84,  
    VFMT_VESA_85_1280X768_60_RB          = 85,  
    VFMT_VESA_86_1280X768_75             = 86,  
    VFMT_VESA_87_1280X800_60             = 87,  
    VFMT_VESA_88_1280X800_75             = 88,  
    VFMT_VESA_89_1280X800_85             = 89,  
    VFMT_VESA_90_1280X800_120_RB         = 90,  
    VFMT_VESA_91_1280X960_60_DMT         = 91,  
    VFMT_VESA_92_1280X960_60_CVT         = 92,  
    VFMT_VESA_93_1280X960_75_CVT         = 93,  
    VFMT_VESA_94_1280X960_85             = 94,  
    VFMT_VESA_95_1280X960_120_RB         = 95,  
    VFMT_VESA_96_1280X1024_60            = 96,  
    VFMT_VESA_97_1280X1024_75            = 97,  
    VFMT_VESA_98_1280X1024_85            = 98,  
    VFMT_VESA_99_1280X1024_120_RB        = 99,  
    VFMT_VESA_100_1360X768_60            = 100,   
    VFMT_VESA_101_1360X768_120_RB        = 101,   
    VFMT_VESA_102_1366X768_60            = 102,   
    VFMT_VESA_103_1400X1050_60           = 103,   
    VFMT_VESA_104_1400X1050_75           = 104,   
    VFMT_VESA_105_1400X1050_85           = 105,   
    VFMT_VESA_106_1400X1050_120_RB       = 106,   
    VFMT_VESA_107_1440X900_60_DMT        = 107,   
    VFMT_VESA_108_1440X900_75            = 108,   
    VFMT_VESA_109_1440X900_85            = 109,   
    VFMT_VESA_110_1440X900_120_RB        = 110,   
    VFMT_VESA_111_1600X900_60_CVT        = 111,   
    VFMT_VESA_112_1600X900_60_DMT_RB     = 112,   
    VFMT_VESA_113_1600X900_75_CVT        = 113,   
    VFMT_VESA_114_1600X900_85_CVT        = 114,                                                                                                        
    VFMT_VESA_115_1600X1200_60           = 115,                                                                                                    
    VFMT_VESA_116_1600X1200_70           = 116,                                                                                                    
    VFMT_VESA_117_1600X1200_75           = 117,                                                                                                    
    VFMT_VESA_118_1600X1200_85           = 118,                                                                                                   
    VFMT_VESA_119_1600X1200_120_RB       = 119,                                                                                                       
    VFMT_VESA_120_1680X1050_60           = 120,                                                                                                     
    VFMT_VESA_121_1680X1050_60_RB        = 121,                                                                                                         
    VFMT_VESA_122_1680X1050_75           = 122,                                                                                                   
    VFMT_VESA_123_1680X1050_85           = 123,                                                                                                   
    VFMT_VESA_124_1680X1050_120_RB       = 124,                                                                                                        
    VFMT_VESA_125_1792X1344_60           = 125,                                                                                                      
    VFMT_VESA_126_1792X1344_75           = 126,                                                                                                   
    VFMT_VESA_127_1856X1392_60           = 127,                                                                                                   
    VFMT_VESA_128_1856X1392_75           = 128,                                                                                                  
    VFMT_VESA_129_1920X1080_60_DMT       = 129,                                                                                                  
    VFMT_VESA_130_1920X1080_60_CVT       = 130,                                                                                                  
    VFMT_VESA_131_1920X1080_60_CVT_RB    = 131,                                                                                                       
    VFMT_VESA_132_1920X1200_60_CVT       = 132,                                                                                                     
    VFMT_VESA_133_1920X1200_60_DMT_RB    = 133,                                                                                                      
    VFMT_VESA_134_1920X1200_75           = 134,                                                                                               
    VFMT_VESA_135_1920X1200_85           = 135,                                                                                               
    VFMT_VESA_136_1920X1440_60           = 136,                                                                                              
    VFMT_VESA_137_1920X1440_75           = 137,                                                                                               
    VFMT_VESA_138_1920X1440_85_CVT       = 138,                                                                                                  
    VFMT_VESA_139_2048X1536_60_CVT       = 139,                                                                                               
    VFMT_VESA_140_2048X1536_75_CVT       = 140,                                                                                                                                    
    VFMT_VESA_141_2048X1536_85_CVT       = 141,                                                                                                 
    VFMT_VESA_142_2560X1600_60           = 142,  
    VFMT_VESA_143_800X480_60             = 143,
    VFMT_VESA_144_640X350_85,
    VFMT_VESA_145_640X360_50,
    VFMT_VESA_146_640X360_60,
    VFMT_VESA_147_640X360_75,
    VFMT_VESA_148_640X360_85,
    VFMT_VESA_149_640X400_50,
    VFMT_VESA_150_640X400_60,
    VFMT_VESA_151_640X400_75,
    VFMT_VESA_152_640X400_85,
    VFMT_VESA_153_640X480_50,
    VFMT_VESA_154_640X480_72,
    VFMT_VESA_155_640X480_85,
    VFMT_VESA_156_720X400_85,
    VFMT_VESA_157_768X480_50,
    VFMT_VESA_158_768X480_60,
    VFMT_VESA_159_768X480_75,
    VFMT_VESA_160_768X480_85,
    VFMT_VESA_161_768X480_60_REDUCE,
    VFMT_VESA_162_800X600_50,
    VFMT_VESA_163_800X600_56,
    VFMT_VESA_164_848X480_50,
    VFMT_VESA_165_848X480_60,
    VFMT_VESA_166_848X480_75,
    VFMT_VESA_167_848X480_85,
    VFMT_VESA_168_848X480_60_REDUCE,
    VFMT_VESA_169_1024X576_50,
    VFMT_VESA_170_1024X576_60,
    VFMT_VESA_171_1024X576_75,
    VFMT_VESA_172_1024X576_85,
    VFMT_VESA_173_1024X640_50,
    VFMT_VESA_174_1024X640_60,
    VFMT_VESA_175_1024X640_75,
    VFMT_VESA_176_1024X640_85,
    VFMT_VESA_177_1024X768_50,
    VFMT_VESA_178_1280X720_50,
    VFMT_VESA_179_1280X768_85,
    VFMT_VESA_180_1280X960_50,
    VFMT_VESA_181_1280X1024_50,
    VFMT_VESA_182_1400X1050_50,
    VFMT_VESA_183_1600X900_50,
    VFMT_VESA_184_1600X1000_50,
    VFMT_VESA_185_1600X1000_60,
    VFMT_VESA_186_1600X1000_75,
    VFMT_VESA_187_1600X1000_85,
    VFMT_VESA_188_1600X1200_50,
    VFMT_VESA_189_1800X1350_50,
    VFMT_VESA_190_1800X1350_60,
    VFMT_VESA_191_1800X1350_75,
    VFMT_VESA_192_1800X1350_85,
    VFMT_VESA_193_1920X1080_75,
    VFMT_VESA_194_1920X1080_85,
    VFMT_VESA_195_1920X1200_50,

    VFMT_VESA_220_2200X2160_30 = 220,

    VFMT_VESA_230_3840X2160_23 = 230,
    VFMT_VESA_231_3840X2160_24 = 231,
    VFMT_VESA_232_3840X2160_25 = 232,
    VFMT_VESA_233_3840X2160_29 = 233,
    VFMT_VESA_234_3840X2160_30 = 234,
    VFMT_VESA_235_4096X2160_24 = 235,
    VFMT_VESA_236_4096X2160_25 = 236,
    VFMT_VESA_237_4096X2160_30 = 237,
    VFMT_VESA_238_4096X2160_23 = 238,
    VFMT_VESA_239_4096X2160_29 = 239,
    VFMT_VESA_250_3840X2160_60 = 250,
    VFMT_VESA_250_3840X2160_50 = 251,
    VFMT_VESA_559_2048X1536_13 = 252,
    VFMT_CEA_34_1920x1080P_10HZ = 253,
    VFMT_CEA_19_1280x720P_18HZ = 254,
    VFMT_CEA_19_2048x1152P_10HZ= 242,
    VFMT_CEA_194_7680x4320P_24HZ= 243,
    SELF_DEF_7680x1200P_30HZ= 244,
    SELF_DEF_4096x1200P_60HZ= 255,

    VFMT_INVALID                         = 0xFFFF
}MS1826_VIDEOFORMAT_E;

UINT16 get_timing_table_size(void);
VOID* get_timing_table(UINT16* pu16_size);

#endif // __MACROSILICON_MS1826_TIMING_TABLE_H__
