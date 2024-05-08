#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "av_config.h"
#include "av_edid_manage.h"

/* EDID Capability Declare Start */
static const uint8  AvEdidSupportDpm   =
                     AvEdidBitDpmStandby |
                     AvEdidBitDpmSuspend |
                     AvEdidBitDpmActiveOff;
uint16 AvEdidSupportFreq  =
                     AvEdidBitFreq6G    |
                     AvEdidBitFreq4P5G  |
                     AvEdidBitFreq3P75G |
                     AvEdidBitFreq3G    |
                     AvEdidBitFreq2P25G |
                     AvEdidBitFreq1P5G  |
                     AvEdidBitFreq750M  |
                     AvEdidBitFreq270M  |
                     AvEdidBitFreq135M;
static const uint16 AvEdidSupportFrameRate  =
                          AvEdidBitFrameRate60HZ |
                          AvEdidBitFrameRate50HZ |
                          AvEdidBitFrameRate30HZ |
                          AvEdidBitFrameRate25HZ |
                          AvEdidBitFrameRate24HZ |
                          AvEdidBitFrameRate56HZ |
                          AvEdidBitFrameRate67HZ |
                          AvEdidBitFrameRate70HZ |
                          AvEdidBitFrameRate72HZ |
                          AvEdidBitFrameRate75HZ |
                          AvEdidBitFrameRate87HZ |
                          AvEdidBitFrameRate88HZ;
static const uint8  AvEdidSupportColorSpace  =
                           AvEdidBitColorSpaceRGB  |
                           AvEdidBitColorSpaceY444 |
                           AvEdidBitColorSpaceY422 |
                           AvEdidBitColorSpaceY420;
static const uint8  AvEdidSupportColorDepth  =
                           AvEdidBitColorDepth36BIT    |
                           AvEdidBitColorDepth30BIT    |
                           AvEdidBitColorDepthAL       |
                           AvEdidBitColorDepthY444;
static const uint8  AvEdidSupportDisplayMode  =
                           AvEdidBitDisplayModeGAME    |
                           AvEdidBitDisplayModeCINEMA  |
                           AvEdidBitDisplayModePHOTO   |
                           AvEdidBitDisplayModeGRAPHIC;
static const uint8  AvEdidNativeVic                       =  16;
static const uint8  AvEdidVrrMin                          =  0x0;
static const uint8  AvEdidDscFrlRate                      =  0;
static const uint8  AvEdidDscMaxSlices                    =  0;
static const uint8  AvEdidDscTotalChunkKBytes             =  0;
static const uint16 AvEdidVsdbSpa                         =  0x1000;
static const uint16 AvEdidVrrMax                          =  0x0;
static const uint8  AvEdidSupportBasicFeature  =
                            AvEdidBitSupportAudio             |
                          //AvEdidBitSupport3D                |
                            AvEdidBitSupportYuvFullRange      |
                            AvEdidBitSupportRgbFullRange      |
                            AvEdidBitSupportPtOverUnderScan   |
                            AvEdidBitSupportVesaOverUnderScan |
                            AvEdidBitSupportCeaOverUnderScan;
static const uint16 AvEdidSupportVsdbHf  =
                      AvEdidBitSupportSCDC              |
                      //AvEdidBitSupportBelow340MScramble |
                      AvEdidBitSupportScdcReadRequest;
static const uint8  AvEdidSupportDsc  =  0;
static const uint16 AvEdidSupportColorimetry  =
                           AvEdidBitSupportColorxvYcc601    |
                           AvEdidBitSupportColorxvYcc709    |
                           //AvEdidBitSupportColorsYCC601     |
                           //AvEdidBitSupportColorAdobeYcc601 |
                           //AvEdidBitSupportColorAdobeRGB    |
                           AvEdidBitSupportColorBt2020cYCC  |
                           AvEdidBitSupportColorBt2020YCC   |
                           AvEdidBitSupportColorBt2020RGB   |
                           //AvEdidBitSupportColorDciP3       |
                           AvEdidBitSupportHdrGammaSdr      |
                           AvEdidBitSupportHdrGammaHdr      |
                           AvEdidBitSupportHdrGammaSmpte    |
                           AvEdidBitSupportHdrGammaHybrid   |
                           AvEdidBitSupportHdrDolbyVision;
static const uint16 AvEdidSupportAudioFormat  =
                           AvEdidBitAudioFormatLPCM         |
                           AvEdidBitAudioFormatAC3          |
                           AvEdidBitAudioFormatDTS          |
                           AvEdidBitAudioFormatENHANCED_AC3 |
                           AvEdidBitAudioFormatDTSHD;
static const uint8  AvEdidAudioChannelLPCM                =  8;
static const uint8  AvEdidAudioChannelAC3                 =  6;
static const uint8  AvEdidAudioChannelMPEG1               =  2;
static const uint8  AvEdidAudioChannelMP3                 =  2;
static const uint8  AvEdidAudioChannelMPEG2               =  2;
static const uint8  AvEdidAudioChannelAACLC               =  2;
static const uint8  AvEdidAudioChannelDTS                 =  6;
static const uint8  AvEdidAudioChannelATRAC               =  2;
static const uint8  AvEdidAudioChannelDSD                 =  6;
static const uint8  AvEdidAudioChannelENHANCED_AC3        =  8;
static const uint8  AvEdidAudioChannelDTSHD               =  8;
static const uint8  AvEdidAudioChannelMAT                 =  8;
static const uint8  AvEdidAudioChannelDST                 =  8;
static const uint8  AvEdidAudioChannelWMAPRO              =  2;
static const uint8  AvEdidAudioBitWidth        =  AvEdidBitAud16bit | AvEdidBitAud20bit | AvEdidBitAud24bit;
static const uint8  AvEdidAudioSfLPCM          =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfAC3           =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfMPEG1         =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfMP3           =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfMPEG2         =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfAACLC         =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfDTS           =  AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf32KHZ;
static const uint8  AvEdidAudioSfATRAC         =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfDSD           =  AvEdidBitSf44KHZ;
static const uint8  AvEdidAudioSfENHANCED_AC3  =  AvEdidBitSf44KHZ | AvEdidBitSf48KHZ;
static const uint8  AvEdidAudioSfDTSHD         =  AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfMAT           =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfDST           =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint8  AvEdidAudioSfWMAPRO        =  AvEdidBitSf32KHZ | AvEdidBitSf44KHZ | AvEdidBitSf48KHZ | AvEdidBitSf88KHZ |
                                                  AvEdidBitSf96KHZ | AvEdidBitSf176KHZ | AvEdidBitSf192KHZ;
static const uint16 AvEdidAudioBitRateAC3      =  640;
static const uint16 AvEdidAudioBitRateMPEG1    =  640;
static const uint16 AvEdidAudioBitRateMP3      =  640;
static const uint16 AvEdidAudioBitRateMPEG2    =  640;
static const uint16 AvEdidAudioBitRateAACLC    =  1536;
static const uint16 AvEdidAudioBitRateDTS      =  1536;
static const uint16 AvEdidAudioBitRateATRAC    =  1536;
static const uint8  AvEdidSpeaker2Ch           =  0x01;
static const uint8  AvEdidSpeaker5p1Ch         =  0x0F;
static const uint8  AvEdidSpeaker7p1Ch         =  0x5F;
/* EDID Capability Declare End */
/* EDID Param Declare Start */
uint16 AvEdidVesaParamForce =  0;
uint8 AvEdidCeaParamForce  =  0;//AvEdidBitCeaSVD | AvEdidBitCeaVSDBHF;
uint16 AvEdidCeabParamForce = 0;//AvEdidBitCeabCDB | AvEdidBitCeabVCDB;
uint16 AvEdidVesaParamRemove = 0;
uint8 AvEdidCeaParamRemove  = 0;
uint16 AvEdidCeabParamRemove = 0;
/* EDID Param Declare End */
/* EDID Table Declare Start */
static const uint8 AvEdidIDManufacturerName[] =  {'U','H','D'};
static const uint8 AvEdidProductName[] =  {'H','D',' ','V','i','d','e','o'};
static const uint8 EdidVesaHeader[] =  {0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x00};
static const uint8 EdidVesaVersion[] =  {0x01,0x03};
static const uint8 EdidVesaColor[] =  {0x23,0xAD,0xA4,0x54,0x4D,0x99,0x26,0x0F,0x47,0x4A};
static const uint8 EdidVesaStdTiming[] =  {0x71,0x4F,0x81,0x00,0x81,0x40,0x81,0x80,0x95,0x00,0x95,0x0F,0xB3,0x00,0xA9,0x40};
static const uint8 EdidDtd6GTiming[] =  {0x08,0xE8,0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,0x8A,0x00,0x50,0x1D,0x74,0x00,0x00,0x1E};
static const uint8 EdidDtd3GTiming[] =  {0x04,0x74,0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,0x8A,0x00,0x50,0x1D,0x74,0x00,0x00,0x1E};
static const uint8 EdidDtd1080pTiming[] =  {0x02,0x3A,0x80,0x18,0x71,0x38,0x2D,0x40,0x58,0x2C,0x45,0x00,0x50,0x1D,0x74,0x00,0x00,0x1E};
static const uint8 EdidDtd720pTiming[] =  {0x01,0x1D,0x00,0x72,0x51,0xD0,0x1E,0x20,0x6E,0x28,0x55,0x00,0xC4,0x8E,0x21,0x00,0x00,0x1E};
static const uint8 EdidDtd1080iTiming[] =  {0x01,0x1D,0x80,0x18,0x71,0x1C,0x16,0x20,0x58,0x2C,0x25,0x00,0xC4,0x8E,0x21,0x00,0x00,0x9E};
static const uint8 EdidDtd480pTiming[] =  {0x8C,0x0A,0xD0,0x8A,0x20,0xE0,0x2D,0x10,0x10,0x3E,0x96,0x00,0x13,0x8E,0x21,0x00,0x00,0x18};
static const uint8 EdidDtd576pTiming[] =  {0x8C,0x0A,0xD0,0x90,0x20,0x40,0x31,0x20,0x0C,0x40,0x55,0x00,0x13,0x8E,0x21,0x00,0x00,0x18};
static const uint8 EdidVesaRangeLimit[] =  {0x00,0x00,0x00,0xFD,0x00,0x32,0x4B,0x0F,0x87,0x0F,0x01,0x00,0x0A,0x20,0x20,0x20,0x20,0x20};
static const uint8 EdidVesaProductName[] =  {0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static const uint8 EdidCeaIdVsdbHdmi[] =  {0x03,0x0C,0x00};
static const uint8 EdidCeaIdVsdbHF[] =  {0xD8,0x5D,0xC4};
static const uint8 EdidCeaDolbyVision[] =  {0x46,0xD0,0x00,0x03,0xB8,0xAB,0x50,0xAF,0x45,0xA4,0xD0,0x26,0x0D,0x14,0x50,0x54,0x67,0x0B,0xA1,0x2C,0x00,0x00,0x00,0x00};
static const uint8 EdidAudioFormatCode[] =  {1<<3,2<<3,3<<3,4<<3,5<<3,6<<3,7<<3,8<<3,9<<3,10<<3,11<<3,12<<3,13<<3,14<<3};
static const uint8 EdidCeaVicList[] =  {97,96,101,102,98,99,100,93,94,95,16,31,4,19,5,20,32,33,34,3,18,7,22};
/* EDID Table Declare End */
/* EDID Structure Init Start */
AvEdidReg DevEdidReg;
void AvEdidFuncStructInit(AvEdidReg *EdidReg)
{
    uint8 i = 0;
    for(i=0;i<23;i++)
        EdidReg->EdidCeaVicCheck[i] = 0;
    for(i=0;i<14;i++)
        EdidReg->EdidCeaAudioCheck[i] = 0;
    for(i=0;i<14;i++)
        EdidReg->EdidCeaAudioSfTable[i] = 0;
    for(i=0;i<14;i++)
        EdidReg->EdidCeaAudioFmtTable[i] = 0;
    for(i=0;i<14;i++)
        EdidReg->EdidCeaAudioByte3[i] = 0;
    for(i=0;i<16;i++)
        EdidReg->VsdbCheckList[i] = 0;
    EdidReg->MaxFrlRate = 0;
    EdidReg->MaxTmdsClk = 0;
    EdidReg->VesaMaxClk = 0;
    EdidReg->MaxCharRate = 0;
    EdidReg->HfVrrMax = 0;
    EdidReg->HfVrrMin = 0;
    EdidReg->HfDscFrlRate = 0;
    EdidReg->HfDscMaxSlices = 0;
    EdidReg->HfDscTotalChunkKBytes = 0;
    for(i=0;i<22;i++)
        EdidReg->VsdbHfCheck[i] = 0;
    for(i=0;i<7;i++)
        EdidReg->VcdbCheck[i] = 0;
    for(i=0;i<4;i++)
        EdidReg->Y420VdbCheck[i] = 0;
    for(i=0;i<4;i++)
        EdidReg->HdrStCheck[i] = 0;
    for(i=0;i<9;i++)
        EdidReg->CdbCheck[i] = 0;
    EdidReg->DolbyVisionCheck = 0;
    return;
}
/* EDID Structure Init End */
/* EDID Function Declare Start */
void AvEdidFuncBulkAddValue(uint8 *table,uint8 total,uint8 start,uint8 value)
{
    uint16  i = 0;
    for(i=0;i<total;i++)
    {
        table[start+i] = table[start+i] + value;
    }
}
void AvEdidFuncBulkDropValue(uint8 *table,uint8 total,uint8 start,uint8 value)
{
    uint16  i = 0;
    for(i=0;i<total;i++)
    {
        if(table[start+i] != 0)
        {
            table[start+i] = table[start+i] - value;
        }
    }
}
void AvEdidFuncBulkWriteData(AvEdidReg *EdidReg,uint8 total,uint8 *FromTable,uint8 FromOffset,uint8 *OutEdid,uint8 ToOffset)
{
    uint8   i = 0;
    for(i=0;i<total;i++)
    {
        OutEdid[ToOffset+i] = FromTable[FromOffset+i];
    }
}
uint16 AvEdidFuncFindCeabTag(uint8 *EdidContent,uint8 TagNumber,uint8 SubVersion)
{
    uint8   bLen = 0;
    uint16  CeabParam = 0;
    uint16  bTag = 0;
    uint16  offset = 0x84;
    if((EdidContent[0x01] != 0xff) || (EdidContent[0x80] != 0x02))
    {
        return 0;
    }
    while(offset <= 0xfd)
    {
        bLen = EdidContent[offset] & 0x1f;
        bTag = (EdidContent[offset]>>5) & 0x07;
        if(bTag == TagNumber)
        {
            if(TagNumber == 3)
            {
                if((SubVersion == 0) &&
                   (EdidContent[offset+1] == EdidCeaIdVsdbHdmi[0]) &&
                   (EdidContent[offset+2] == EdidCeaIdVsdbHdmi[1]) &&
                   (EdidContent[offset+3] == EdidCeaIdVsdbHdmi[2]))
                {
                    CeabParam = (offset<<8) + bLen;
                    return CeabParam;
                }
                else if((SubVersion == 1) &&
                   (EdidContent[offset+1] == EdidCeaIdVsdbHF[0]) &&
                   (EdidContent[offset+2] == EdidCeaIdVsdbHF[1]) &&
                   (EdidContent[offset+3] == EdidCeaIdVsdbHF[2]))
                {
                    CeabParam = (offset<<8) + bLen;
                    return CeabParam;
                }
                else
                {
                    offset = offset + bLen + 1;
                }
            }
            else if(TagNumber == 7)
            {
                if(EdidContent[offset+1] == SubVersion)
                {
                    CeabParam = (offset<<8) + bLen;
                    return CeabParam;
                }
                else
                {
                    offset = offset + bLen + 1;
                }
            }
            else
            {
                CeabParam = (offset<<8) + bLen;
                return CeabParam;
            }
        }
        else if(bLen == 0)
        {
            CeabParam = 0;
            return CeabParam;
        }
        else
        {
            offset = offset + bLen + 1;
        }
    }
    CeabParam = 0;
    return CeabParam;
}
uint8 AvEdidFuncBlockPermit(AvEdidReg *EdidReg,uint16 InCeabParam,uint8 InEdidExist,uint16 SinkCeabParam,uint8 SinkEdidExist)
{
    uint8   Permit = 1;
    if(InEdidExist == 1)
    {
        if((InCeabParam&0x00ff) == 0)
        {
            Permit = 0;
        }
    }
    if(SinkEdidExist == 1)
    {
        if((SinkCeabParam&0x00ff) == 0)
        {
            Permit = 0;
        }
    }
    return Permit;
}
void AvEdidFuncCheckSum(uint8 *OutEdid)
{
    uint8   Total = 0;
    uint8   i = 0;
    for(i=0;i<127;i++)
    {
        Total = Total + OutEdid[i];
    }
    OutEdid[127] = 256 - (Total%256);
    Total = 0;
    for(i=0;i<127;i++)
    {
        Total = Total + OutEdid[128+i];
    }
    OutEdid[255] = 256 - (Total%256);
}
void AvEdidFuncVesaProcess(AvEdidReg *EdidReg,uint8 *InEdid,uint8 *SinkEdid,uint8 *OutEdid)
{
    uint8   InEdidLimitRange = 0;
    uint8   SinkEdidLimitRange = 0;
    uint8   i = 0;
    uint8   j = 0;
    uint8   k = 0;
    uint8   SelectValue = 0;
    uint8   ColorMatch = 0;
    SelectValue = 1;
    if((InEdid[0] == 0x00) && (InEdid[1] == 0xff))
    {
        SelectValue = SelectValue + 1;
    }
    if((SinkEdid[0] == 0x00) && (SinkEdid[1] == 0xff))
    {
        SelectValue = SelectValue + 1;
    }
    for(i=0;i<8;i++)
    {
        OutEdid[i+0] = EdidVesaHeader[i];
    }
    OutEdid[8]  = (((uint8)(AvEdidIDManufacturerName[0]) - (uint8)('A') + 1) << 2) +
                  (((uint8)(AvEdidIDManufacturerName[1]) - (uint8)('A') + 1) >> 3);
    OutEdid[9]  = (((uint8)(AvEdidIDManufacturerName[1]) - (uint8)('A') + 1) << 5)%256 +
                  (((uint8)(AvEdidIDManufacturerName[2]) - (uint8)('A') + 1));
    OutEdid[10] = AvEdidIDProductCode>>8;
    OutEdid[11] = AvEdidIDProductCode&0xff;
    OutEdid[12] = (AvEdidIDSerialNumber>>0)  & 0xff;
    OutEdid[13] = (AvEdidIDSerialNumber>>8)  & 0xff;
    OutEdid[14] = (AvEdidIDSerialNumber>>16) & 0xff;
    OutEdid[15] = (AvEdidIDSerialNumber>>24) & 0xff;
    OutEdid[16] = AvEdidManufactureWeek & 0xff;
    OutEdid[17] = (AvEdidManufactureYear - 1990) & 0xff;
    for(i=0;i<2;i++)
    {
        OutEdid[i+18] = EdidVesaVersion[i];
    }
    if((AvEdidVesaParamForce  & AvEdidBitVesaVideoInputDEF) ||
       (AvEdidVesaParamRemove & AvEdidBitVesaVideoInputDEF))
    {
        OutEdid[20] = 0x80;
    }
    else
    {
        if(SelectValue == 3)
        {
            if(InEdid[20] != SinkEdid[20])
            {
                OutEdid[20] = 0x80;
            }
            else
            {
                OutEdid[20] = InEdid[20];
            }
        }
        else if(SelectValue == 2)
        {
            OutEdid[20] = InEdid[20];
        }
        else
        {
            OutEdid[20] = 0x80;
        }
    }
    if((AvEdidVesaParamForce  & AvEdidBitVesaScreenSIZE) ||
       (AvEdidVesaParamRemove & AvEdidBitVesaScreenSIZE))
    {
        OutEdid[21] = AvEdidScreenHSize;
        OutEdid[22] = AvEdidScreenVSize;
    }
    else
    {
        if(SelectValue == 3)
        {
            if(InEdid[21]  > SinkEdid[21])
            {
                OutEdid[21] = SinkEdid[21];
                OutEdid[22] = SinkEdid[22];
            }
            else
            {
                OutEdid[21] = InEdid[21];
                OutEdid[22] = InEdid[22];
            }
        }
        else if(SelectValue == 2)
        {
            OutEdid[21] = InEdid[21];
            OutEdid[22] = InEdid[22];
        }
        else
        {
            OutEdid[21] = AvEdidScreenHSize;
            OutEdid[22] = AvEdidScreenVSize;
        }
    }
    if((AvEdidVesaParamForce  & AvEdidBitVesaGAMMA) ||
       (AvEdidVesaParamRemove & AvEdidBitVesaGAMMA))
    {
        OutEdid[23] = 0x78;
    }
    else
    {
        if(SelectValue == 3)
        {
            if(InEdid[23] != SinkEdid[23])
            {
                OutEdid[23] = 0x78;
            }
            else
            {
                OutEdid[23] = InEdid[23];
            }
        }
        else if(SelectValue == 2)
        {
            OutEdid[23] = InEdid[23];
        }
        else
        {
            OutEdid[23] = 0x78;
        }
    }
    if(AvEdidVesaParamForce  & AvEdidBitVesaDPM)
    {
        OutEdid[24] = 0x02;
        if(AvEdidSupportDpm & AvEdidBitDpmStandby)
        {
            OutEdid[24] = OutEdid[24] | AvEdidCfgDpmStandby;
        }
        if(AvEdidSupportDpm & AvEdidBitDpmSuspend)
        {
            OutEdid[24] = OutEdid[24] | AvEdidCfgDpmSuspend;
        }
        if(AvEdidSupportDpm & AvEdidBitDpmActiveOff)
        {
            OutEdid[24] = OutEdid[24] | AvEdidCfgDpmActiveOff;
        }
        if(AvEdidSupportDpm & AvEdidBitDpmStandby)
        {
            if(AvEdidSupportColorSpace & (AvEdidBitColorSpaceY422 | AvEdidBitColorSpaceY444))
            {
                OutEdid[24] = OutEdid[24] | (0x03<<3);
            }
            else if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY422)
            {
                OutEdid[24] = OutEdid[24] | (0x02<<3);
            }
            else if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY444)
            {
                OutEdid[24] = OutEdid[24] | (0x01<<3);
            }
        }
        else
        {
            OutEdid[24] = OutEdid[24] | (0x01<<3);
        }
    }
    else if(AvEdidVesaParamRemove & AvEdidBitVesaDPM)
    {
        OutEdid[24] = 0x0a;
    }
    else
    {
        if(SelectValue == 3)
        {
            OutEdid[24] = InEdid[24] & SinkEdid[24];
        }
        else if(SelectValue == 2)
        {
            OutEdid[24] = InEdid[24];
        }
        else
        {
            OutEdid[24] = 0x0a;
        }
    }
    if((AvEdidVesaParamForce  & AvEdidBitVesaColor) ||
       (AvEdidVesaParamRemove & AvEdidBitVesaColor))
    {
        for(i=0;i<10;i++)
        {
            OutEdid[i+25] = EdidVesaColor[i];
        }
    }
    else
    {
        if(SelectValue == 3)
        {
            ColorMatch = 1;
            for(i=0;i<10;i++)
            {
                if(InEdid[25+i] != SinkEdid[25+i])
                {
                    ColorMatch = 0;
                }
            }
            if(ColorMatch == 1)
            {
                for(i=0;i<10;i++)
                {
                    OutEdid[25+i] = InEdid[25+i];
                }
            }
            else
            {
                for(i=0;i<10;i++)
                {
                    OutEdid[i+25] = EdidVesaColor[i];
                }
            }
        }
        else if(SelectValue == 2)
        {
            for(i=0;i<10;i++)
            {
                OutEdid[25+i] = InEdid[25+i];
            }
        }
        else
        {
            for(i=0;i<10;i++)
            {
                OutEdid[i+25] = EdidVesaColor[i];
            }
        }
    }
    if(AvEdidVesaParamForce  & AvEdidBitVesaTiming)
    {
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate60HZ)
        {
            OutEdid[35] = 0x21;
            OutEdid[36] = 0x08;
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate56HZ)
        {
            OutEdid[35] = OutEdid[35] | (1<<1);
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate67HZ)
        {
            OutEdid[35] = OutEdid[35] | (1<<4);
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate70HZ)
        {
            OutEdid[35] = OutEdid[35] | (1<<7);
            OutEdid[36] = OutEdid[36] | (1<<2);
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate72HZ)
        {
            OutEdid[35] = OutEdid[35] | (1<<3);
            OutEdid[36] = OutEdid[36] | (1<<7);
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate75HZ)
        {
            OutEdid[35] = OutEdid[35] | (1<<2);
            OutEdid[36] = OutEdid[36] | 0x63;
            OutEdid[37] = 1<<7;
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate87HZ)
        {
            OutEdid[36] = OutEdid[36] | (1<<4);
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate88HZ)
        {
            OutEdid[35] = OutEdid[35] | (1<<6);
        }
    }
    else if(AvEdidVesaParamRemove & AvEdidBitVesaTiming)
    {
        OutEdid[35] = 1<<5;
        OutEdid[36] = 0;
        OutEdid[37] = 0;
    }
    else
    {
        if(SelectValue == 3)
        {
            OutEdid[35] = InEdid[35] & SinkEdid[35];
            OutEdid[36] = InEdid[36] & SinkEdid[36];
            OutEdid[37] = InEdid[37] & SinkEdid[37];
        }
        else if(SelectValue == 2)
        {
            OutEdid[35] = InEdid[35];
            OutEdid[36] = InEdid[36];
            OutEdid[37] = InEdid[37];
        }
        else
        {
            OutEdid[35] = 1<<5;
            OutEdid[36] = 0;
            OutEdid[37] = 0;
        }
    }

#if 0
    /* Use InEdid's content */
    if(SelectValue == 2)
    {
        AvMemcpy(OutEdid+8,InEdid+8,48);
    }
    else if(SelectValue == 3)
    {
        AvMemcpy(OutEdid+8,SinkEdid+8,48);
    }
#endif

    if(AvEdidVesaParamForce  & AvEdidBitVesaStandardTIMING)
    {
        for(i=0;i<16;i++)
        {
            OutEdid[38+i] = EdidVesaStdTiming[i];
        }
        for(i=0;i<8;i++)
        {
            if((AvEdidSupportFrameRate & AvEdidBitFrameRate60HZ) == 0)
            {
                if((OutEdid[39+i*2]&0x1F) == 0x00)
                {
                    OutEdid[38+i*2] = 0;
                    OutEdid[39+i*2] = 0;
                }
            }
            if((AvEdidSupportFrameRate & AvEdidBitFrameRate75HZ) == 0)
            {
                if((OutEdid[39+i*2]&0x1F) == 0x0F)
                {
                    OutEdid[38+i*2] = 0;
                    OutEdid[39+i*2] = 0;
                }
            }
        }
    }
    else if(AvEdidVesaParamRemove & AvEdidBitVesaStandardTIMING)
    {
        for(i=0;i<16;i++)
        {
            OutEdid[38+i] = 0x00;
        }
    }
    else
    {
        if(SelectValue == 3)
        {
            k = 0;
            for(i=0;i<8;i++)
            {
                for(j=0;j<8;j++)
                {
                    if((SinkEdid[38+j*2] == InEdid[38+i*2]) &&
                       (SinkEdid[39+j*2] == InEdid[39+i*2]))
                    {
                        OutEdid[38+k*2] = SinkEdid[38+j*2];
                        OutEdid[39+k*2] = SinkEdid[39+j*2];
                        k = k + 1;
                    }
                }
            }
        }
        else if(SelectValue == 2)
        {
            for(i=0;i<8;i++)
            {
                OutEdid[38+i*2] = InEdid[38+i*2];
                OutEdid[39+i*2] = InEdid[39+i*2];
            }
        }
        else
        {
            for(i=0;i<16;i++)
            {
                OutEdid[38+i] = EdidVesaStdTiming[i];
            }
        }
    }
    if(AvEdidSupportFreq & AvEdidBitFreq6G)
    {
        EdidReg->VesaMaxClk = 600;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq4P5G)
    {
        EdidReg->VesaMaxClk = 450;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq3P75G)
    {
        EdidReg->VesaMaxClk = 380;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq3G)
    {
        EdidReg->VesaMaxClk = 300;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq2P25G)
    {
        EdidReg->VesaMaxClk = 230;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq1P5G)
    {
        EdidReg->VesaMaxClk = 150;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq750M)
    {
        EdidReg->VesaMaxClk = 80;
    }
    else
    {
        EdidReg->VesaMaxClk = 30;
    }
    InEdidLimitRange = 0;
    SinkEdidLimitRange = 0;
    if(SelectValue >= 2)
    {
        for(i=0;i<3;i++)
        {
            if((InEdid[0x48+i*18+0] == 0x00) &&
               (InEdid[0x48+i*18+1] == 0x00) &&
               (InEdid[0x48+i*18+2] == 0x00) &&
               (InEdid[0x48+i*18+3] == 0xfd))
            {
                InEdidLimitRange = i + 1;
                if((AvEdidVesaParamForce & AvEdidBitVesaMaxClk) == 0)
                {
                    if(EdidReg->VesaMaxClk > InEdid[0x48+i*18+9]*10)
                    {
                        EdidReg->VesaMaxClk = InEdid[0x48+i*18+9]*10;
                    }
                }
            }
        }
#if (Gsv2k8MuxMode == 1)||(Gsv2k2MuxMode == 1)||(Gsv2k6MuxMode == 1)||(Gsv5k1MuxMode == 1)
        if(((InEdid[8] == 0x20) && (InEdid[9] == 0xA3) && (InEdid[10] == 0x30) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x0D) && (InEdid[9] == 0x04) && (InEdid[10] == 0x30) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x20) && (InEdid[9] == 0xA3) && (InEdid[10] == 0x01) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x4D) && (InEdid[9] == 0x79) && (InEdid[10] == 0x01) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x41) && (InEdid[9] == 0x0C) && (InEdid[10] == 0x00) && (InEdid[11] == 0x00)))
        {
            if(EdidReg->VesaMaxClk > 300)
                EdidReg->VesaMaxClk = 300;
        }
#endif
    }
    if(SelectValue == 3)
    {
        for(i=0;i<3;i++)
        {
            if((SinkEdid[0x48+i*18+0] == 0x00) &&
               (SinkEdid[0x48+i*18+1] == 0x00) &&
               (SinkEdid[0x48+i*18+2] == 0x00) &&
               (SinkEdid[0x48+i*18+3] == 0xfd))
            {
                SinkEdidLimitRange = i + 1;
                if((AvEdidVesaParamForce & AvEdidBitVesaMaxClk) == 0)
                {
                    if(EdidReg->VesaMaxClk > SinkEdid[0x48+i*18+9]*10)
                    {
                        EdidReg->VesaMaxClk = SinkEdid[0x48+i*18+9]*10;
                    }
                }
            }
        }
#if (Gsv2k8MuxMode == 1)||(Gsv2k2MuxMode == 1)||(Gsv2k6MuxMode == 1)||(Gsv5k1MuxMode == 1)
        if(((SinkEdid[8] == 0x20) && (SinkEdid[9] == 0xA3) && (SinkEdid[10] == 0x30) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x0D) && (SinkEdid[9] == 0x04) && (SinkEdid[10] == 0x30) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x20) && (SinkEdid[9] == 0xA3) && (SinkEdid[10] == 0x01) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x4D) && (SinkEdid[9] == 0x79) && (SinkEdid[10] == 0x01) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x41) && (SinkEdid[9] == 0x0C) && (SinkEdid[10] == 0x00) && (SinkEdid[11] == 0x00)))
        {
            if(EdidReg->VesaMaxClk > 300)
                EdidReg->VesaMaxClk = 300;
        }
#endif
    }
#if 0
    if((SelectValue >= 2) && (InEdidLimitRange == 0) && (SinkEdidLimitRange == 0))
    {
        EdidReg->VesaMaxClk = 150;
    }
#endif
    if(AvEdidCeaParamForce & AvEdidBitCeaVSDBHF)
    {
        EdidReg->VesaMaxClk = 600;
    }
    SelectValue = 1;
    if(InEdidLimitRange != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidLimitRange != 0)
    {
        SelectValue = SelectValue + 1;
    }
    AvEdidFuncDtdWrite(EdidReg, 1, OutEdid, 54);
    AvEdidFuncDtdWrite(EdidReg, 2, OutEdid, 72);
    for(i=0;i<18;i++)
    {
        OutEdid[90+i] = EdidVesaRangeLimit[i];
    }
    if(AvEdidSupportFrameRate & AvEdidBitFrameRate24HZ)
    {
        OutEdid[90+5] = 24;
    }
    else if(AvEdidSupportFrameRate & AvEdidBitFrameRate25HZ)
    {
        OutEdid[90+5] = 25;
    }
    else if(AvEdidSupportFrameRate & AvEdidBitFrameRate30HZ)
    {
        OutEdid[90+5] = 30;
    }
#if 0
    else
    {
        OutEdid[90+5] = 48;
    }
#endif
    if(AvEdidSupportFrameRate & AvEdidBitFrameRate88HZ)
    {
        OutEdid[90+6] = 88;
    }
    else if(AvEdidSupportFrameRate & AvEdidBitFrameRate87HZ)
    {
        OutEdid[90+6] = 87;
    }
#if 0
    else
    {
        OutEdid[90+6] = 85;
    }
#endif
    OutEdid[90+9] = EdidReg->VesaMaxClk/10;
    if(SelectValue >= 2)
    {
        k = 0;
        for(i=0;i<3;i++)
        {
            if((InEdid[0x48+i*18+0] == 0x00) &&
               (InEdid[0x48+i*18+1] == 0x00) &&
               (InEdid[0x48+i*18+2] == 0x00) &&
               (InEdid[0x48+i*18+3] == 0xfd))
            {
                k = 0x48+i*18;
            }
        }
        if(k != 0)
        {
            OutEdid[90+7] = InEdid[k+7];
            OutEdid[90+8] = InEdid[k+8];
            if(InEdid[k+5] > OutEdid[90+5])
            {
                OutEdid[90+5] = InEdid[k+5];
            }
            if(InEdid[k+6] < OutEdid[90+6])
            {
                OutEdid[90+6] = InEdid[k+6];
            }
        }
    }
    if(SelectValue == 3)
    {
        k = 0;
        for(i=0;i<3;i++)
        {
            if((SinkEdid[0x48+i*18+0] == 0x00) &&
               (SinkEdid[0x48+i*18+1] == 0x00) &&
               (SinkEdid[0x48+i*18+2] == 0x00) &&
               (SinkEdid[0x48+i*18+3] == 0xfd))
            {
                k = 0x48+i*18;
            }
        }
        if(k != 0)
        {
            if(SinkEdid[k+5] > OutEdid[90+5])
            {
                OutEdid[90+5] = SinkEdid[k+5];
            }
            if(SinkEdid[k+6] < OutEdid[90+6])
            {
                OutEdid[90+6] = SinkEdid[k+6];
            }
            if(SinkEdid[k+7] > OutEdid[90+7])
            {
                OutEdid[90+7] = SinkEdid[k+7];
            }
            if(SinkEdid[k+8] < OutEdid[90+8])
            {
                OutEdid[90+8] = SinkEdid[k+8];
            }
        }
    }
    for(i=0;i<18;i++)
    {
        OutEdid[108+i] = EdidVesaProductName[i];
    }
    for(i=0;i<sizeof(AvEdidProductName);i++)
    {
        OutEdid[108+5+i] = (uint8)(AvEdidProductName[i]);
    }
#if 0
    /* Use InEdid's content */
    if(SelectValue == 2)
    {
        for(i=0;i<3;i++)
        {
            if((InEdid[0x48+i*18+0] == 0x00) &&
               (InEdid[0x48+i*18+1] == 0x00) &&
               (InEdid[0x48+i*18+2] == 0x00) &&
               (InEdid[0x48+i*18+3] == 0xfc))
            {
                AvMemcpy(OutEdid+108,InEdid+0x48+i*18,18);
            }
        }
    }
    else if(SelectValue == 3)
    {
        for(i=0;i<3;i++)
        {
            if((SinkEdid[0x48+i*18+0] == 0x00) &&
               (SinkEdid[0x48+i*18+1] == 0x00) &&
               (SinkEdid[0x48+i*18+2] == 0x00) &&
               (SinkEdid[0x48+i*18+3] == 0xfc))
            {
                AvMemcpy(OutEdid+108,SinkEdid+0x48+i*18,18);
            }
        }
    }
#endif
}
void AvEdidFunFullAnalysis(AvEdidReg *EdidReg,uint8 *InEdid)
{
    uint16  iCeabParam = 0;
    uint8   i = 0;
    uint8   j = 0;
    uint8   iEdidOffset = 0;
    uint8   iEdidLen    = 0;
    uint8   VicNum = 0;
    for(i=0;i<3;i++)
    {
        if((InEdid[0x48+i*18+0] == 0x00) &&
           (InEdid[0x48+i*18+1] == 0x00) &&
           (InEdid[0x48+i*18+2] == 0x00) &&
           (InEdid[0x48+i*18+3] == 0xfd))
        {
            EdidReg->VesaMaxClk = InEdid[0x48+i*18+9]*10;
        }
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x02, 0);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    for(i=0;i<iEdidLen;i++)
    {
        VicNum = InEdid[iEdidOffset+1+i] & 0x7f;
        for(j=0;j<23;j++)
        {
            if(VicNum == EdidCeaVicList[j])
            {
                EdidReg->EdidCeaVicCheck[j] = 1;
            }
        }
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x01, 0);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabAudioRead(EdidReg, InEdid, iEdidLen, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x03, 0);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabVsdbHdmiRead(EdidReg, InEdid, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x03, 1);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabVsdbHfRead(EdidReg, InEdid, iCeabParam);
    }
    if((InEdid[0x83] & 0x10) != 0)
    {
        EdidReg->VcdbCheck[4] = EdidReg->VcdbCheck[4] + 1;
    }
    if((InEdid[0x83] & 0x20) != 0)
    {
        EdidReg->VcdbCheck[5] = EdidReg->VcdbCheck[5] + 1;
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 0);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabVcdbRead(EdidReg, InEdid, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 6);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabHdrStRead(EdidReg, InEdid, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 14);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabY420VdbRead(EdidReg, InEdid, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 5);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabCdbRead(EdidReg, InEdid, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 15);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabY420CmdbRead(EdidReg, InEdid, iEdidOffset);
    }
    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 1);
    iEdidOffset   = (iCeabParam>>8)&0xff;
    iEdidLen      = (iCeabParam>>0)&0xff;
    if(iEdidOffset != 0)
    {
        AvEdidFuncCeabVsvdbRead(EdidReg, InEdid, iEdidOffset);
    }
    if(EdidReg->VesaMaxClk > EdidReg->MaxCharRate)
    {
        EdidReg->MaxCharRate = EdidReg->VesaMaxClk;
    }
    else if(EdidReg->VesaMaxClk < EdidReg->MaxCharRate)
    {
        EdidReg->VesaMaxClk = EdidReg->MaxCharRate;
    }
    if(((InEdid[8] == 0x20) && (InEdid[9] == 0xA3) && (InEdid[10] == 0x30) && (InEdid[11] == 0x00)) ||
       ((InEdid[8] == 0x0D) && (InEdid[9] == 0x04) && (InEdid[10] == 0x30) && (InEdid[11] == 0x00)) ||
       ((InEdid[8] == 0x20) && (InEdid[9] == 0xA3) && (InEdid[10] == 0x01) && (InEdid[11] == 0x00)) ||
       ((InEdid[8] == 0x4D) && (InEdid[9] == 0x79) && (InEdid[10] == 0x01) && (InEdid[11] == 0x00)) ||
       ((InEdid[8] == 0x41) && (InEdid[9] == 0x0C) && (InEdid[10] == 0x00) && (InEdid[11] == 0x00)))
    {
        if(EdidReg->MaxTmdsClk > 300)
            EdidReg->MaxTmdsClk = 300;
        if(EdidReg->MaxCharRate > 300)
            EdidReg->MaxCharRate = 300;
        if(EdidReg->VesaMaxClk > 300)
            EdidReg->VesaMaxClk = 300;
    }
}
void AvEdidFuncCeaProcess(AvEdidReg *EdidReg,uint8 *InEdid,uint8 *SinkEdid,uint8 *OutEdid)
{
    uint16  iCeabParam = 0;
    uint16  sCeabParam = 0;
    uint8   i = 0;
    uint8   oLen    = 0;
    uint8   oOffset = 0;
    uint8   TotalDtd = 0;
    uint16  OutEdidCeabOffset = 0;
    uint8   VsdbHfForceFlag = 0;
    uint8   Db420ForceFlag = 0;
    uint8   HdrForceFlag = 0;
    uint8   Y420CmdbCount = 0;
    uint8   Y420VdbCount = 0;
    uint8   InEdidExist = 0;
    uint8   SinkEdidExist = 0;
    uint8   BlockPermit = 0;
    OutEdid[0x7E] = 0x01;
    OutEdid[0x80] = 0x02;
    OutEdid[0x81] = 0x03;
    if((InEdid[0x01] == 0xff) && (InEdid[0x80] == 0x02))
    {
        InEdidExist   = 1;
    }
    if((SinkEdid[0x01] == 0xff) && (SinkEdid[0x80] == 0x02))
    {
        SinkEdidExist = 1;
    }
    if((InEdidExist == 0) && (SinkEdidExist == 0))
    {
        if(AvEdidSupportFreq & (AvEdidBitFreq6G | AvEdidBitFreq4P5G | AvEdidBitFreq3P75G))
        {
            VsdbHfForceFlag = 1;
        }
        if((AvEdidSupportFreq & AvEdidBitFreq3G) & (AvEdidSupportColorSpace & AvEdidBitColorSpaceY420))
        {
            Db420ForceFlag = 1;
        }
        if(AvEdidSupportColorimetry & (AvEdidBitSupportHdrGammaSdr | AvEdidBitSupportHdrGammaHdr | AvEdidBitSupportHdrGammaSmpte | AvEdidBitSupportHdrGammaHybrid))
        {
            HdrForceFlag = 1;
        }
    }
    if(InEdidExist == 1)
    {
#if (Gsv2k8MuxMode == 1)||(Gsv2k2MuxMode == 1)||(Gsv2k6MuxMode == 1)||(Gsv5k1MuxMode == 1)
        if(((InEdid[8] == 0x20) && (InEdid[9] == 0xA3) && (InEdid[10] == 0x30) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x0D) && (InEdid[9] == 0x04) && (InEdid[10] == 0x30) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x20) && (InEdid[9] == 0xA3) && (InEdid[10] == 0x01) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x4D) && (InEdid[9] == 0x79) && (InEdid[10] == 0x01) && (InEdid[11] == 0x00)) ||
           ((InEdid[8] == 0x41) && (InEdid[9] == 0x0C) && (InEdid[10] == 0x00) && (InEdid[11] == 0x00)))
        {
            if(EdidReg->MaxTmdsClk > 300)
                EdidReg->MaxTmdsClk = 300;
            if(EdidReg->MaxCharRate > 300)
                EdidReg->MaxCharRate = 300;
            AvEdidCeaParamRemove =  AvEdidCeaParamRemove | AvEdidBitCeaVSDBHF;
        }
#endif
    }
    if(SinkEdidExist == 1)
    {
#if (Gsv2k8MuxMode == 1)||(Gsv2k2MuxMode == 1)||(Gsv2k6MuxMode == 1)||(Gsv5k1MuxMode == 1)
        if(((SinkEdid[8] == 0x20) && (SinkEdid[9] == 0xA3) && (SinkEdid[10] == 0x30) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x0D) && (SinkEdid[9] == 0x04) && (SinkEdid[10] == 0x30) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x20) && (SinkEdid[9] == 0xA3) && (SinkEdid[10] == 0x01) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x4D) && (SinkEdid[9] == 0x79) && (SinkEdid[10] == 0x01) && (SinkEdid[11] == 0x00)) ||
           ((SinkEdid[8] == 0x41) && (SinkEdid[9] == 0x0C) && (SinkEdid[10] == 0x00) && (SinkEdid[11] == 0x00)))
        {
            if(EdidReg->MaxTmdsClk > 300)
                EdidReg->MaxTmdsClk = 300;
            if(EdidReg->MaxCharRate > 300)
                EdidReg->MaxCharRate = 300;
            AvEdidCeaParamRemove =  AvEdidCeaParamRemove | AvEdidBitCeaVSDBHF;
        }
#endif
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = 0x84;
    if(AvEdidCeaParamForce  & AvEdidBitCeaSVD)
    {
        oLen       = AvEdidFuncCeabSVD(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeaParamRemove & AvEdidBitCeaSVD)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x02, 0);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x02, 0);
        oLen       = AvEdidFuncCeabSVD(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = OutEdidCeabOffset;
    if(AvEdidCeaParamForce  & AvEdidBitCeaAUDIO)
    {
        oLen       = AvEdidFuncCeabAudio(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeaParamRemove & AvEdidBitCeaAUDIO)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x01, 0);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x01, 0);
        oLen       = AvEdidFuncCeabAudio(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = OutEdidCeabOffset;
    if(AvEdidCeaParamForce  & AvEdidBitCeaVSDBHDMI)
    {
        oLen       = AvEdidFuncCeabVsdbHdmi(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeaParamRemove & AvEdidBitCeaVSDBHDMI)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x03, 0);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x03, 0);
        oLen       = AvEdidFuncCeabVsdbHdmi(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    if(AvEdidSupportFreq & (AvEdidBitFreq6G | AvEdidBitFreq4P5G | AvEdidBitFreq3P75G))
    {
        iCeabParam = 0;
        sCeabParam = 0;
        oLen    = 0;
        oOffset = OutEdidCeabOffset;
        if((AvEdidCeaParamForce  & AvEdidBitCeaVSDBHF) || (VsdbHfForceFlag == 1))
        {
            oLen       = AvEdidFuncCeabVsdbHf(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
        }
        else if(AvEdidCeaParamRemove & AvEdidBitCeaVSDBHF)
        {
            oLen       = 0;
        }
        else
        {
            iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x03, 1);
            sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x03, 1);
            BlockPermit = AvEdidFuncBlockPermit(EdidReg, iCeabParam, InEdidExist, sCeabParam, SinkEdidExist);
            if(BlockPermit == 1)
            {
                oLen   = AvEdidFuncCeabVsdbHf(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
            }
        }
        if(oLen>0)
        {
            OutEdidCeabOffset = oOffset + 1 + oLen;
        }
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = OutEdidCeabOffset;
    if(AvEdidCeabParamForce  & AvEdidBitCeabVCDB)
    {
        oLen       = AvEdidFuncCeabVcdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeabParamRemove & AvEdidBitCeabVCDB)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x07, 0);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x07, 0);
        oLen       = AvEdidFuncCeabVcdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = OutEdidCeabOffset;
    if(AvEdidCeabParamForce  & AvEdidBitCeabCDB)
    {
        oLen       = AvEdidFuncCeabCdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeabParamRemove & AvEdidBitCeabCDB)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x07, 5);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x07, 5);
        BlockPermit = AvEdidFuncBlockPermit(EdidReg, iCeabParam, InEdidExist, sCeabParam, SinkEdidExist);
        if(BlockPermit == 1)
        {
            oLen   = AvEdidFuncCeabCdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
        }
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = OutEdidCeabOffset;
    if((AvEdidCeabParamForce  & AvEdidBitCeabHDR_STATIC) || (HdrForceFlag == 1))
    {
        oLen       = AvEdidFuncCeabHdrSt(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeabParamRemove & AvEdidBitCeabHDR_STATIC)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x07, 6);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x07, 6);
        BlockPermit = AvEdidFuncBlockPermit(EdidReg, iCeabParam, InEdidExist, sCeabParam, SinkEdidExist);
        if(BlockPermit == 1)
        {
            oLen   = AvEdidFuncCeabHdrSt(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
        }
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY420)
    {
        iCeabParam = 0;
        sCeabParam = 0;
        oLen    = 0;
        oOffset = OutEdidCeabOffset;
        if(AvEdidSupportFreq & AvEdidBitFreq6G)
        {
            if((AvEdidCeabParamForce  & AvEdidBitCeabY420CMDB) || (Db420ForceFlag == 1))
            {
                oLen   = AvEdidFuncCeabY420Cmdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
            }
            else if(AvEdidCeabParamRemove & AvEdidBitCeabY420CMDB)
            {
                oLen   = 0;
            }
            else
            {
                if(InEdidExist == 0)
                {
                    Y420CmdbCount = 2;
                    Y420VdbCount  = 2;
                }
                else if(SinkEdidExist == 0)
                {
                    Y420CmdbCount = 1;
                    Y420VdbCount  = 1;
                }
                else
                {
                    Y420CmdbCount = 0;
                    Y420VdbCount  = 0;
                }
                if(InEdidExist == 1)
                {
                    iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x07, 14);
                    if((iCeabParam&0x00ff) == 0)
                    {
                        iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 15);
                        if((iCeabParam&0x00ff) != 0)
                        {
                            Y420CmdbCount = Y420CmdbCount + 1;
                            Y420VdbCount  = Y420VdbCount + 1;
                        }
                    }
                    else
                    {
                        Y420VdbCount  = Y420VdbCount + 1;
                    }
                }
                if(SinkEdidExist == 1)
                {
                    sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x07, 14);
                    if((sCeabParam&0x00ff) == 0)
                    {
                        sCeabParam    = AvEdidFuncFindCeabTag(SinkEdid,   0x07, 15);
                        if((sCeabParam&0x00ff) != 0)
                        {
                            Y420CmdbCount = Y420CmdbCount + 1;
                            Y420VdbCount  = Y420VdbCount + 1;
                        }
                    }
                    else
                    {
                        Y420VdbCount  = Y420VdbCount + 1;
                    }
                }
                if(Y420CmdbCount == 2)
                {
                    oLen   = AvEdidFuncCeabY420Cmdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
                }
                else if(Y420VdbCount == 2)
                {
                    oLen   = AvEdidFuncCeabY420Vdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
                }
            }
        }
        else if(AvEdidSupportFreq & (AvEdidBitFreq4P5G | AvEdidBitFreq3P75G | AvEdidBitFreq3G))
        {
            if((AvEdidCeabParamForce  & AvEdidBitCeabY420VDB) || (Db420ForceFlag == 1))
            {
                oLen   = AvEdidFuncCeabY420Vdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
            }
            else if(AvEdidCeabParamRemove & AvEdidBitCeabY420VDB)
            {
                oLen   = 0;
            }
            else
            {
                iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x07, 14);
                if((iCeabParam&0x00ff) == 0)
                {
                    iCeabParam    = AvEdidFuncFindCeabTag(InEdid,   0x07, 15);
                }
                sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x07, 14);
                if((sCeabParam&0x00ff) == 0)
                {
                    sCeabParam    = AvEdidFuncFindCeabTag(SinkEdid,   0x07, 15);
                }
                BlockPermit = AvEdidFuncBlockPermit(EdidReg, iCeabParam, InEdidExist, sCeabParam, SinkEdidExist);
                if(BlockPermit == 1)
                {
                    oLen   = AvEdidFuncCeabY420Vdb(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
                }
            }
        }
        if(oLen>0)
        {
            OutEdidCeabOffset = oOffset + 1 + oLen;
        }
    }
    iCeabParam = 0;
    sCeabParam = 0;
    oLen    = 0;
    oOffset = OutEdidCeabOffset;
    if(AvEdidCeabParamForce  & AvEdidBitCeabHDR_DOLBYVISION)
    {
        oLen       = AvEdidFuncCeabDolbyVisionSt(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
    }
    else if(AvEdidCeabParamRemove & AvEdidBitCeabHDR_DOLBYVISION)
    {
        oLen       = 0;
    }
    else
    {
        iCeabParam = AvEdidFuncFindCeabTag(InEdid,   0x07, 1);
        sCeabParam = AvEdidFuncFindCeabTag(SinkEdid, 0x07, 1);
        BlockPermit = AvEdidFuncBlockPermit(EdidReg, iCeabParam, InEdidExist, sCeabParam, SinkEdidExist);
        if(BlockPermit == 1)
        {
            oLen   = AvEdidFuncCeabDolbyVisionSt(EdidReg, InEdid, iCeabParam, SinkEdid, sCeabParam, OutEdid, oOffset);
        }
    }
    if(oLen>0)
    {
        OutEdidCeabOffset = oOffset + 1 + oLen;
    }
    OutEdid[0x82] = OutEdidCeabOffset - 0x80;
    TotalDtd = (0xfd - OutEdidCeabOffset)/18;

    if(OutEdidCeabOffset > 255)
    {
        TotalDtd = 0;
    }
    else if(TotalDtd > 4)
    {
        TotalDtd = 4;
    }
    OutEdid[0x83] = TotalDtd;
    if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY422)
    {
        OutEdid[0x83] = OutEdid[0x83] | 0x10;
    }
    if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY444)
    {
        OutEdid[0x83] = OutEdid[0x83] | 0x20;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupportAudio)
    {
        OutEdid[0x83] = OutEdid[0x83] | 0x40;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupportCeaOverUnderScan)
    {
        OutEdid[0x83] = OutEdid[0x83] | 0x80;
    }
    for(i=0;i<TotalDtd;i++)
    {
        AvEdidFuncDtdWrite(EdidReg, i+1, OutEdid, OutEdidCeabOffset+i*18);
    }
    OutEdidCeabOffset = OutEdidCeabOffset + 18*TotalDtd;
    oLen = 0xff - OutEdidCeabOffset;
    for(i=0;i<oLen;i++)
    {
        OutEdid[OutEdidCeabOffset+i] = 0;
    }
}
void AvEdidFuncDtdWrite(AvEdidReg *EdidReg,uint8 BlockNum,uint8 *OutEdid,uint8 offset)
{
    uint16  MaxClk   = 0;
    MaxClk = EdidReg->VesaMaxClk;
    if(EdidReg->MaxTmdsClk > MaxClk)
    {
        MaxClk = EdidReg->MaxTmdsClk;
    }
    if(EdidReg->MaxCharRate > MaxClk)
    {
        MaxClk = EdidReg->MaxCharRate;
    }
    if(BlockNum == 1)
    {
        if(MaxClk == 600)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd6GTiming,    0, OutEdid, offset);
        }
        else if(MaxClk >= 290)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd3GTiming,    0, OutEdid, offset);
        }
        else if(MaxClk >= 150)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080pTiming, 0, OutEdid, offset);
        }
        else
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd720pTiming,  0, OutEdid, offset);
        }
    }
    else if(BlockNum == 2)
    {
        if(MaxClk == 600)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd3GTiming,    0, OutEdid, offset);
        }
        else if(MaxClk >= 290)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080pTiming, 0, OutEdid, offset);
        }
        else if(MaxClk >= 150)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd720pTiming,  0, OutEdid, offset);
        }
        else
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080iTiming, 0, OutEdid, offset);
        }
    }
    else if(BlockNum == 3)
    {
        if(MaxClk == 600)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080pTiming, 0, OutEdid, offset);
        }
        else if(MaxClk >= 290)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd720pTiming,  0, OutEdid, offset);
        }
        else if(MaxClk >= 150)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080iTiming, 0, OutEdid, offset);
        }
        else
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd480pTiming,  0, OutEdid, offset);
        }
    }
    else
    {
        if(MaxClk == 600)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080iTiming, 0, OutEdid, offset);
        }
        else if(MaxClk >= 290)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd1080iTiming, 0, OutEdid, offset);
        }
        else if(MaxClk >= 150)
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd480pTiming,  0, OutEdid, offset);
        }
        else
        {
            AvEdidFuncBulkWriteData(EdidReg, 18, (uint8*)EdidDtd576pTiming,  0, OutEdid, offset);
        }
    }
}
uint8 AvEdidFuncCeabSVD(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   InEdidLen      = (InCeabParam>>0)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SinkEdidLen    = (SinkCeabParam>>0)&0xff;
    uint8   i = 0;
    uint8   j = 0;
    uint8   InEdidNativeVic   = 0;
    uint8   SinkEdidNativeVic = 0;
    uint8   SelectValue = 0;
    uint8   OutNativeVic = 0;
    uint8   bLen = 0;
    uint8   VicNum = 0;
    uint8   NativeFlag = 0;
    if(AvEdidSupportFreq & AvEdidBitFreq6G)
    {
        AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 4, 0, 1);
#if 0
        EdidReg->EdidCeaVicCheck[3] = 0;/* remove 4096x2160@60Hz */
        EdidReg->EdidCeaVicCheck[6] = 0;/* remove 4096x2160@30Hz */
#endif
    }
    if(AvEdidSupportFreq & (AvEdidBitFreq4P5G | AvEdidBitFreq3P75G | AvEdidBitFreq3G))
    {
        AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 6, 4, 1);
        if((AvEdidSupportColorSpace & AvEdidBitColorSpaceY420) && ((AvEdidSupportFreq & AvEdidBitFreq6G) == 0))
        {
            AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 4, 0, 1);
        }
    }
    if(AvEdidSupportFreq & (AvEdidBitFreq2P25G | AvEdidBitFreq1P5G))
    {
        AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 2, 10, 1);
    }
    if(AvEdidSupportFreq & AvEdidBitFreq750M)
    {
        AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 7, 12, 1);
    }
    if(AvEdidSupportFreq & AvEdidBitFreq270M)
    {
        AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 2, 19, 1);
    }
    if(AvEdidSupportFreq & AvEdidBitFreq135M)
    {
        AvEdidFuncBulkAddValue(EdidReg->EdidCeaVicCheck, 2, 21, 1);
    }
    if(InEdidOffset != 0)
    {
        for(i=0;i<InEdidLen;i++)
        {
            VicNum = InEdid[InEdidOffset+1+i] & 0x7f;
            NativeFlag = InEdid[InEdidOffset+1+i] & 0x80;
            if(NativeFlag != 0)
            {
                InEdidNativeVic = VicNum;
            }
            for(j=0;j<23;j++)
            {
                if(VicNum == EdidCeaVicList[j])
                {
                    EdidReg->EdidCeaVicCheck[j] = EdidReg->EdidCeaVicCheck[j] + 1;
                }
            }
        }
    }
    if(SinkEdidOffset != 0)
    {
        for(i=0;i<SinkEdidLen;i++)
        {
            VicNum = SinkEdid[SinkEdidOffset+1+i] & 0x7f;
            NativeFlag = SinkEdid[SinkEdidOffset+1+i] & 0x80;
            if(NativeFlag != 0)
            {
                SinkEdidNativeVic = VicNum;
            }
            for(j=0;j<23;j++)
            {
                if(VicNum == EdidCeaVicList[j])
                {
                    EdidReg->EdidCeaVicCheck[j] = EdidReg->EdidCeaVicCheck[j] + 1;
                }
            }
        }
    }
	if((AvEdidSupportFrameRate & AvEdidBitFrameRate24HZ) == 0)
    {
        EdidReg->EdidCeaVicCheck[4] = 0;
        EdidReg->EdidCeaVicCheck[7] = 0;
        EdidReg->EdidCeaVicCheck[16] = 0;
    }
    if((AvEdidSupportFrameRate & AvEdidBitFrameRate25HZ) == 0)
    {
        EdidReg->EdidCeaVicCheck[5] = 0;
        EdidReg->EdidCeaVicCheck[8] = 0;
        EdidReg->EdidCeaVicCheck[17] = 0;
    }
    if((AvEdidSupportFrameRate & AvEdidBitFrameRate30HZ) == 0)
    {
        EdidReg->EdidCeaVicCheck[6] = 0;
        EdidReg->EdidCeaVicCheck[9] = 0;
        EdidReg->EdidCeaVicCheck[18] = 0;
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(AvEdidCeaParamRemove & AvEdidBitCeaNATIVE)
    {
        OutNativeVic = 0;
    }
    else
    {
        OutNativeVic = AvEdidNativeVic;
        if((AvEdidCeaParamForce  & AvEdidBitCeaNATIVE) == 0)
        {
            if(InEdidNativeVic != 0)
            {
                OutNativeVic = InEdidNativeVic;
            }
            else if(SinkEdidNativeVic != 0)
            {
                OutNativeVic = SinkEdidNativeVic;
            }
        }
    }
    for(i=0;i<23;i++)
    {
        if(EdidReg->EdidCeaVicCheck[i] == SelectValue)
        {
            bLen = bLen + 1;
            if(EdidCeaVicList[i] == OutNativeVic)
            {
                OutEdid[OutEdidOffset+bLen] = EdidCeaVicList[i] | 0x80;
            }
            else
            {
                OutEdid[OutEdidOffset+bLen] = EdidCeaVicList[i];
            }
        }
    }
    OutEdid[OutEdidOffset] = (0x02<<5) + bLen;
    return bLen;
}
uint8 AvEdidFuncCeabAudio(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   InEdidLen      = (InCeabParam>>0)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SinkEdidLen    = (SinkCeabParam>>0)&0xff;
    uint8   i = 0;
    uint8   SelectValue = 0;
    uint8   bLen = 0;
    if((AvEdidSupportBasicFeature & AvEdidBitSupportAudio) == 0)
    {
        return 0;
    }
    EdidReg->EdidCeaAudioSfTable[0]   = AvEdidAudioSfLPCM;
    EdidReg->EdidCeaAudioSfTable[1]   = AvEdidAudioSfAC3;
    EdidReg->EdidCeaAudioSfTable[2]   = AvEdidAudioSfMPEG1;
    EdidReg->EdidCeaAudioSfTable[3]   = AvEdidAudioSfMP3;
    EdidReg->EdidCeaAudioSfTable[4]   = AvEdidAudioSfMPEG2;
    EdidReg->EdidCeaAudioSfTable[5]   = AvEdidAudioSfAACLC;
    EdidReg->EdidCeaAudioSfTable[6]   = AvEdidAudioSfDTS;
    EdidReg->EdidCeaAudioSfTable[7]   = AvEdidAudioSfATRAC;
    EdidReg->EdidCeaAudioSfTable[8]   = AvEdidAudioSfDSD;
    EdidReg->EdidCeaAudioSfTable[9]   = AvEdidAudioSfENHANCED_AC3;
    EdidReg->EdidCeaAudioSfTable[10]  = AvEdidAudioSfDTSHD;
    EdidReg->EdidCeaAudioSfTable[11]  = AvEdidAudioSfMAT;
    EdidReg->EdidCeaAudioSfTable[12]  = AvEdidAudioSfDST;
    EdidReg->EdidCeaAudioSfTable[13]  = AvEdidAudioSfWMAPRO;
    EdidReg->EdidCeaAudioFmtTable[0]  = AvEdidAudioChannelLPCM  - 1;
    EdidReg->EdidCeaAudioFmtTable[1]  = AvEdidAudioChannelAC3   - 1;
    EdidReg->EdidCeaAudioFmtTable[2]  = AvEdidAudioChannelMPEG1 - 1;
    EdidReg->EdidCeaAudioFmtTable[3]  = AvEdidAudioChannelMP3   - 1;
    EdidReg->EdidCeaAudioFmtTable[4]  = AvEdidAudioChannelMPEG2 - 1;
    EdidReg->EdidCeaAudioFmtTable[5]  = AvEdidAudioChannelAACLC - 1;
    EdidReg->EdidCeaAudioFmtTable[6]  = AvEdidAudioChannelDTS   - 1;
    EdidReg->EdidCeaAudioFmtTable[7]  = AvEdidAudioChannelATRAC - 1;
    EdidReg->EdidCeaAudioFmtTable[8]  = AvEdidAudioChannelDSD   - 1;
    EdidReg->EdidCeaAudioFmtTable[9]  = AvEdidAudioChannelENHANCED_AC3 - 1;
    EdidReg->EdidCeaAudioFmtTable[10] = AvEdidAudioChannelDTSHD - 1;
    EdidReg->EdidCeaAudioFmtTable[11] = AvEdidAudioChannelMAT   - 1;
    EdidReg->EdidCeaAudioFmtTable[12] = AvEdidAudioChannelDST   - 1;
    EdidReg->EdidCeaAudioFmtTable[13] = AvEdidAudioChannelWMAPRO - 1;
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatLPCM)
    {
        EdidReg->EdidCeaAudioCheck[0]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatAC3)
    {
        EdidReg->EdidCeaAudioCheck[1]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatMPEG1)
    {
        EdidReg->EdidCeaAudioCheck[2]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatMP3)
    {
        EdidReg->EdidCeaAudioCheck[3]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatMPEG2)
    {
        EdidReg->EdidCeaAudioCheck[4]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatAACLC)
    {
        EdidReg->EdidCeaAudioCheck[5]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatDTS)
    {
        EdidReg->EdidCeaAudioCheck[6]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatATRAC)
    {
        EdidReg->EdidCeaAudioCheck[7]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatDSD)
    {
        EdidReg->EdidCeaAudioCheck[8]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatENHANCED_AC3)
    {
        EdidReg->EdidCeaAudioCheck[9]     = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatDTSHD)
    {
        EdidReg->EdidCeaAudioCheck[10]    = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatMAT)
    {
        EdidReg->EdidCeaAudioCheck[11]    = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatDST)
    {
        EdidReg->EdidCeaAudioCheck[12]    = 1;
    }
    if(AvEdidSupportAudioFormat & AvEdidBitAudioFormatWMAPRO)
    {
        EdidReg->EdidCeaAudioCheck[13]    = 1;
    }
    EdidReg->EdidCeaAudioByte3[0]     = AvEdidAudioBitWidth;
    EdidReg->EdidCeaAudioByte3[1]     = AvEdidAudioBitRateAC3>>3;
    EdidReg->EdidCeaAudioByte3[2]     = AvEdidAudioBitRateMPEG1>>3;
    EdidReg->EdidCeaAudioByte3[3]     = AvEdidAudioBitRateMP3>>3;
    EdidReg->EdidCeaAudioByte3[4]     = AvEdidAudioBitRateMPEG2>>3;
    EdidReg->EdidCeaAudioByte3[5]     = AvEdidAudioBitRateAACLC>>3;
    EdidReg->EdidCeaAudioByte3[6]     = AvEdidAudioBitRateDTS>>3;
    EdidReg->EdidCeaAudioByte3[7]     = AvEdidAudioBitRateATRAC>>3;
    EdidReg->EdidCeaAudioByte3[8]     = AvEdidAudioBitWidth>>3;
    EdidReg->EdidCeaAudioByte3[9]     = 0x50;
    EdidReg->EdidCeaAudioByte3[10]    = 0x00;
    EdidReg->EdidCeaAudioByte3[11]    = 0x00;
    EdidReg->EdidCeaAudioByte3[12]    = 0x01;
    EdidReg->EdidCeaAudioByte3[13]    = 0x00;
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabAudioRead(EdidReg, InEdid, InEdidLen, InEdidOffset);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabAudioRead(EdidReg, SinkEdid, SinkEdidLen, SinkEdidOffset);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    for(i=0;i<14;i++)
    {
        if(EdidReg->EdidCeaAudioCheck[i] == SelectValue)
        {
            bLen = bLen + 1;
            OutEdid[OutEdidOffset+bLen*3-2] = ((i+1)<<3) | EdidReg->EdidCeaAudioFmtTable[i];
            OutEdid[OutEdidOffset+bLen*3-1] = EdidReg->EdidCeaAudioSfTable[i];
            OutEdid[OutEdidOffset+bLen*3-0] = EdidReg->EdidCeaAudioByte3[i];
        }
    }
    bLen = bLen*3;
    OutEdid[OutEdidOffset] = (0x01<<5) + bLen;
    OutEdid[OutEdidOffset+bLen+1] = 0x83;
    OutEdid[OutEdidOffset+bLen+3] = 0x00;
    OutEdid[OutEdidOffset+bLen+4] = 0x00;
    if(EdidReg->EdidCeaAudioCheck[0] >= SelectValue)
    {
        if(EdidReg->EdidCeaAudioFmtTable[0] >= 7)
        {
            OutEdid[OutEdidOffset+bLen+2] = AvEdidSpeaker7p1Ch;
        }
        else if(EdidReg->EdidCeaAudioFmtTable[0] >= 5)
        {
            OutEdid[OutEdidOffset+bLen+2] = AvEdidSpeaker5p1Ch;
        }
        else
        {
            OutEdid[OutEdidOffset+bLen+2] = AvEdidSpeaker2Ch;
        }
        bLen = bLen+4;
    }
    return bLen;
}
void AvEdidFuncCeabAudioRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidLen,uint8 EdidOffset)
{
    uint8   i = 0;
    uint8   j = 0;
    uint8   oLen = 0;
    uint8   oLenExt = 0;
    uint8   oTotal = 0;
    uint8   oOffset = 0;
    uint8   FmtNum = 0;
    uint8   ChnNum = 0;
    uint8 AudioCheck [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8 AudioChn   [] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    oLen = EdidLen/3;
    if((Edid[EdidOffset+1+EdidLen] & 0xe0) == 0x20)
    {
        oLenExt = (Edid[EdidOffset+1+EdidLen] & 0x1f)/3;
    }
    oTotal = oLen + oLenExt;
    for(i=0;i<oTotal;i++)
    {
        if(i < oLen)
        {
            oOffset = EdidOffset+i*3;
        }
        else
        {
            oOffset = EdidOffset+EdidLen+(i-oLen)*3+1;
        }
        FmtNum = Edid[oOffset+1] & 0x78;
        ChnNum = Edid[oOffset+1] & 0x07;
        for(j=0;j<14;j++)
        {
            if(FmtNum == EdidAudioFormatCode[j])
            {
                if(AudioCheck[j] == 0)
                {
                    AudioCheck[j] = 1;
                    AudioChn[j] = ChnNum;
                }
                EdidReg->EdidCeaAudioSfTable[j] = EdidReg->EdidCeaAudioSfTable[j] & Edid[oOffset+2];
                if(ChnNum > AudioChn[j])
                {
                    AudioChn[j] = ChnNum;
                }
                if(j == 0)
                {
                    EdidReg->EdidCeaAudioByte3[0] = EdidReg->EdidCeaAudioByte3[0] & Edid[oOffset+3];
                }
                else if(j < 7)
                {
                    if(Edid[oOffset+3] < EdidReg->EdidCeaAudioByte3[j])
                    {
                        EdidReg->EdidCeaAudioByte3[j] = Edid[oOffset+3];
                    }
                }
                else
                {
                    EdidReg->EdidCeaAudioByte3[j] = Edid[oOffset+3];
                }
            }
        }
    }
    for(i=0;i<14;i++)
    {
        if(AudioCheck[i] != 0)
        {
            EdidReg->EdidCeaAudioCheck[i] = EdidReg->EdidCeaAudioCheck[i] + 1;
            if(AudioChn[i] < EdidReg->EdidCeaAudioFmtTable[i])
            {
                EdidReg->EdidCeaAudioFmtTable[i] = AudioChn[i];
            }
        }
    }
}
void AvEdidFuncCeabVsdbHdmiRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset)
{
    uint8   i = 0;
    uint8   bLen = 0;
    uint8   List4K = 0;
    if((Edid[EdidOffset+4] != 0) || (Edid[EdidOffset+5] != 0))
    {
        EdidReg->VsdbCheckList[15] = EdidReg->VsdbCheckList[15] + 1;
    }
    if((Edid[EdidOffset+0] & 0x1f) <= 6)
    {
        if(EdidReg->VesaMaxClk != 0)
        {
            EdidReg->MaxTmdsClk = EdidReg->VesaMaxClk;
        }
        return;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgVsdbAl)
    {
        EdidReg->VsdbCheckList[8] = EdidReg->VsdbCheckList[8] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgVsdb48bit)
    {
        EdidReg->VsdbCheckList[9] = EdidReg->VsdbCheckList[9] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgVsdb36bit)
    {
        EdidReg->VsdbCheckList[10] = EdidReg->VsdbCheckList[10] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgVsdb30bit)
    {
        EdidReg->VsdbCheckList[11] = EdidReg->VsdbCheckList[11] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgVsdbY444)
    {
        EdidReg->VsdbCheckList[12] = EdidReg->VsdbCheckList[12] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgVsdbDualDvi)
    {
        EdidReg->VsdbCheckList[13] = EdidReg->VsdbCheckList[13] + 1;
    }
    if(Edid[EdidOffset+8] & AvEdidCfgModeGame)
    {
        EdidReg->VsdbCheckList[0] = EdidReg->VsdbCheckList[0] + 1;
    }
    if(Edid[EdidOffset+8] & AvEdidCfgModeCinema)
    {
        EdidReg->VsdbCheckList[1] = EdidReg->VsdbCheckList[1] + 1;
    }
    if(Edid[EdidOffset+8] & AvEdidCfgModePhoto)
    {
        EdidReg->VsdbCheckList[2] = EdidReg->VsdbCheckList[2] + 1;
    }
    if(Edid[EdidOffset+8] & AvEdidCfgModeGraphic)
    {
        EdidReg->VsdbCheckList[3] = EdidReg->VsdbCheckList[3] + 1;
    }
    if(Edid[EdidOffset+8] & 0x20)
    {
        bLen = 10;
        if(Edid[EdidOffset+8] & 0x80)
        {
            bLen = bLen + 2;
        }
        if(Edid[EdidOffset+8] & 0x40)
        {
            bLen = bLen + 2;
        }
        List4K = (Edid[EdidOffset+bLen]>>5) & 0x07;
        for(i=0;i<List4K;i++)
        {
            if(Edid[EdidOffset+bLen+1+i] == 1)
            {
                EdidReg->VsdbCheckList[4] = EdidReg->VsdbCheckList[4] + 1;
            }
            if(Edid[EdidOffset+bLen+1+i] == 2)
            {
                EdidReg->VsdbCheckList[5] = EdidReg->VsdbCheckList[5] + 1;
            }
            if(Edid[EdidOffset+bLen+1+i] == 3)
            {
                EdidReg->VsdbCheckList[6] = EdidReg->VsdbCheckList[6] + 1;
            }
            if(Edid[EdidOffset+bLen+1+i] == 4)
            {
                EdidReg->VsdbCheckList[7] = EdidReg->VsdbCheckList[7] + 1;
            }
        }
        if(Edid[EdidOffset+bLen-1] & 0x80)
        {
            EdidReg->VsdbCheckList[14] = EdidReg->VsdbCheckList[14] + 1;
        }
    }
    if((((Edid[EdidOffset+7]*5) < EdidReg->MaxTmdsClk) || (EdidReg->MaxTmdsClk == 0)) && (Edid[EdidOffset+7] != 0))
    {
        EdidReg->MaxTmdsClk = Edid[EdidOffset+7]*5;
    }
}
void AvEdidFuncCeabVsdbHfRead(AvEdidReg *EdidReg,uint8 *Edid,uint16 EdidParam)
{
    uint16  value = 0;
    uint8   EdidOffset = (EdidParam>>8)&0xff;
    uint8   EdidLen    = (EdidParam>>0)&0xff;
    if(Edid[EdidOffset+7] & AvEdidCfgHf48bit420)
    {
        EdidReg->VsdbHfCheck[0] = EdidReg->VsdbHfCheck[0] + 1;
    }
    if(Edid[EdidOffset+7] & AvEdidCfgHf36bit420)
    {
        EdidReg->VsdbHfCheck[1] = EdidReg->VsdbHfCheck[1] + 1;
    }
    if(Edid[EdidOffset+7] & AvEdidCfgHf30bit420)
    {
        EdidReg->VsdbHfCheck[2] = EdidReg->VsdbHfCheck[2] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHfScdcPresent)
    {
        EdidReg->VsdbHfCheck[3] = EdidReg->VsdbHfCheck[3] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHf3DOsdDisparity)
    {
        EdidReg->VsdbHfCheck[4] = EdidReg->VsdbHfCheck[4] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHfDualView)
    {
        EdidReg->VsdbHfCheck[5] = EdidReg->VsdbHfCheck[5] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHfIndpendentView)
    {
        EdidReg->VsdbHfCheck[6] = EdidReg->VsdbHfCheck[6] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHf340MScramble)
    {
        EdidReg->VsdbHfCheck[7] = EdidReg->VsdbHfCheck[7] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHfReadRequest)
    {
        EdidReg->VsdbHfCheck[8] = EdidReg->VsdbHfCheck[8] + 1;
    }
    if(Edid[EdidOffset+6] & AvEdidCfgHfCcbpci)
    {
        EdidReg->VsdbHfCheck[20] = EdidReg->VsdbHfCheck[20] + 1;
    }
    value = Edid[EdidOffset+5]*5;
    if((EdidReg->MaxCharRate > value) || (EdidReg->MaxCharRate == 0))
    {
        EdidReg->MaxCharRate = value;
        EdidReg->VesaMaxClk  = value;
    }
    if(EdidLen > 7)
    {
        if(Edid[EdidOffset+8] & AvEdidCfgHfMdelta)
        {
            EdidReg->VsdbHfCheck[9] = EdidReg->VsdbHfCheck[9] + 1;
        }
        if(Edid[EdidOffset+8] & AvEdidCfgHfCinemaVrr)
        {
            EdidReg->VsdbHfCheck[10] = EdidReg->VsdbHfCheck[10] + 1;
        }
        if(Edid[EdidOffset+8] & AvEdidCfgHfCnmVrr)
        {
            EdidReg->VsdbHfCheck[11] = EdidReg->VsdbHfCheck[11] + 1;
        }
        if(Edid[EdidOffset+8] & AvEdidCfgHfFva)
        {
            EdidReg->VsdbHfCheck[12] = EdidReg->VsdbHfCheck[12] + 1;
        }
        if(Edid[EdidOffset+8] & AvEdidCfgHfAllm)
        {
            EdidReg->VsdbHfCheck[13] = EdidReg->VsdbHfCheck[13] + 1;
        }
        if(Edid[EdidOffset+8] & AvEdidCfgHfFapa)
        {
            EdidReg->VsdbHfCheck[14] = EdidReg->VsdbHfCheck[14] + 1;
        }
        if(Edid[EdidOffset+11] & AvEdidCfgHfDsc1p2)
        {
            EdidReg->VsdbHfCheck[15] = EdidReg->VsdbHfCheck[15] + 1;
        }
        if(Edid[EdidOffset+11] & AvEdidCfgHfDscNative420)
        {
            EdidReg->VsdbHfCheck[16] = EdidReg->VsdbHfCheck[16] + 1;
        }
        if(Edid[EdidOffset+11] & AvEdidCfgHfDsc16bpc)
        {
            EdidReg->VsdbHfCheck[17] = EdidReg->VsdbHfCheck[17] + 1;
        }
        if(Edid[EdidOffset+11] & AvEdidCfgHfDsc12bpc)
        {
            EdidReg->VsdbHfCheck[18] = EdidReg->VsdbHfCheck[18] + 1;
        }
        if(Edid[EdidOffset+11] & AvEdidCfgHfDsc10bpc)
        {
            EdidReg->VsdbHfCheck[19] = EdidReg->VsdbHfCheck[19] + 1;
        }
        if(Edid[EdidOffset+11] & AvEdidCfgHfDscAllBpp)
        {
            EdidReg->VsdbHfCheck[21] = EdidReg->VsdbHfCheck[21] + 1;
        }
        value = Edid[EdidOffset+7]>>4;
        if((EdidReg->MaxFrlRate > value) || (EdidReg->MaxFrlRate == 0))
        {
            EdidReg->MaxFrlRate = value;
        }
        value = ((Edid[EdidOffset+9]&0xC0)<<2) + Edid[EdidOffset+10];
        if((EdidReg->HfVrrMax > value) || (EdidReg->HfVrrMax == 0))
        {
            EdidReg->HfVrrMax = value;
        }
        value = Edid[EdidOffset+9] & 0x3F;
        if((value > EdidReg->HfVrrMin) || (EdidReg->HfVrrMin == 0))
        {
            EdidReg->HfVrrMin = value;
        }
        value = (Edid[EdidOffset+12]>>4) & 0x0F;
        if((value < EdidReg->HfDscFrlRate) || (EdidReg->HfDscFrlRate == 0))
        {
            EdidReg->HfDscFrlRate = value;
        }
        value = Edid[EdidOffset+12] & 0x0F;
        if((value < EdidReg->HfDscMaxSlices) || (EdidReg->HfDscMaxSlices == 0))
        {
            EdidReg->HfDscMaxSlices = value;
        }
        value = Edid[EdidOffset+13] & 0x3F;
        if((value < EdidReg->HfDscTotalChunkKBytes) || (EdidReg->HfDscTotalChunkKBytes == 0))
        {
            EdidReg->HfDscTotalChunkKBytes = value;
        }
    }
    else
    {
        EdidReg->HfVrrMax = 0;
        EdidReg->HfVrrMin = 0;
        EdidReg->HfDscFrlRate = 0;
        EdidReg->HfDscMaxSlices = 0;
        EdidReg->HfDscTotalChunkKBytes = 0;
    }
}
void AvEdidFuncCeabVcdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset)
{
    if(Edid[EdidOffset+2] & AvEdidCfgYuvFullRange)
    {
        EdidReg->VcdbCheck[0] = EdidReg->VcdbCheck[0] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgRgbFullRange)
    {
        EdidReg->VcdbCheck[1] = EdidReg->VcdbCheck[1] + 1;
    }
    if((Edid[EdidOffset+2] & AvEdidCfgVesaOverUnderScan) == 0x0C)
    {
        EdidReg->VcdbCheck[2] = EdidReg->VcdbCheck[2] + 1;
    }
    if((Edid[EdidOffset+2] & AvEdidCfgCeaOverUnderScan) == 0x03)
    {
        EdidReg->VcdbCheck[3] = EdidReg->VcdbCheck[3] + 1;
    }
    if((Edid[EdidOffset+2] & AvEdidCfgPtOverUnderScan) == 0x30)
    {
        EdidReg->VcdbCheck[6] = EdidReg->VcdbCheck[6] + 1;
    }
}
void AvEdidFuncCeabY420VdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset)
{
    uint8   BlockSize = 0;
    uint8   i = 0;
    BlockSize = (Edid[EdidOffset+0] & 0x1F) - 1;
    for(i=0;i<BlockSize;i++)
    {
        if(Edid[EdidOffset+i+2] == 96)
        {
            EdidReg->Y420VdbCheck[0] = EdidReg->Y420VdbCheck[0] + 1;
        }
        else if(Edid[EdidOffset+i+2] == 97)
        {
            EdidReg->Y420VdbCheck[1] = EdidReg->Y420VdbCheck[1] + 1;
        }
        else if(Edid[EdidOffset+i+2] == 101)
        {
            EdidReg->Y420VdbCheck[2] = EdidReg->Y420VdbCheck[2] + 1;
        }
        else if(Edid[EdidOffset+i+2] == 102)
        {
            EdidReg->Y420VdbCheck[3] = EdidReg->Y420VdbCheck[3] + 1;
        }
    }
}
void AvEdidFuncCeabVsvdbRead(AvEdidReg *EdidReg,uint8 *InEdid,uint8 iEdidOffset)
{
    if((InEdid[iEdidOffset+2] == EdidCeaDolbyVision[0]) && (InEdid[iEdidOffset+3] == EdidCeaDolbyVision[1]) && (InEdid[iEdidOffset+4] == EdidCeaDolbyVision[2]))
    {
        EdidReg->DolbyVisionCheck = EdidReg->DolbyVisionCheck + 1;
    }
}
void AvEdidFuncCeabY420CmdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset)
{
    uint16  SvdParam = 0;
    uint8   BlockSize = 0;
    uint8   i = 0;
    uint8   SvdOffset = 0;
    uint8   SvdLen = 0;
    SvdParam  = AvEdidFuncFindCeabTag(Edid,   0x02, 0);
    SvdOffset = (SvdParam>>8) & 0xff;
    SvdLen    = SvdParam & 0xff;
    BlockSize = (Edid[EdidOffset+0] & 0x1F) - 1;
    if(SvdLen > BlockSize*8)
    {
        SvdLen = BlockSize*8;
    }
    if(SvdLen != 0)
    {
        for(i=0;i<SvdLen;i++)
        {
            if((Edid[SvdOffset+i+1] == 96)  && (Edid[EdidOffset+2+(i/8)] & (1<<(i%8))))
            {
                EdidReg->Y420VdbCheck[0] = EdidReg->Y420VdbCheck[0] + 1;
            }
            if((Edid[SvdOffset+i+1] == 97)  && (Edid[EdidOffset+2+(i/8)] & (1<<(i%8))))
            {
                EdidReg->Y420VdbCheck[1] = EdidReg->Y420VdbCheck[1] + 1;
            }
            if((Edid[SvdOffset+i+1] == 101) && (Edid[EdidOffset+2+(i/8)] & (1<<(i%8))))
            {
                EdidReg->Y420VdbCheck[2] = EdidReg->Y420VdbCheck[2] + 1;
            }
            if((Edid[SvdOffset+i+1] == 102) && (Edid[EdidOffset+2+(i/8)] & (1<<(i%8))))
            {
                EdidReg->Y420VdbCheck[3] = EdidReg->Y420VdbCheck[3] + 1;
            }
        }
    }
}
void AvEdidFuncCeabHdrStRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset)
{
    if(Edid[EdidOffset+2] & AvEdidCfgHdrGammaSdr)
    {
        EdidReg->HdrStCheck[0] = EdidReg->HdrStCheck[0] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgHdrGammaHdr)
    {
        EdidReg->HdrStCheck[1] = EdidReg->HdrStCheck[1] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgHdrGammaSmpte)
    {
        EdidReg->HdrStCheck[2] = EdidReg->HdrStCheck[2] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgHdrGammaHybrid)
    {
        EdidReg->HdrStCheck[3] = EdidReg->HdrStCheck[3] + 1;
    }
}
void AvEdidFuncCeabCdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset)
{
    if(Edid[EdidOffset+2] & AvEdidCfgColorxvYcc601)
    {
        EdidReg->CdbCheck[0] = EdidReg->CdbCheck[0] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorxvYcc709)
    {
        EdidReg->CdbCheck[1] = EdidReg->CdbCheck[1] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorsYCC601)
    {
        EdidReg->CdbCheck[2] = EdidReg->CdbCheck[2] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorAdobeYcc601)
    {
        EdidReg->CdbCheck[3] = EdidReg->CdbCheck[3] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorAdobeRGB)
    {
        EdidReg->CdbCheck[4] = EdidReg->CdbCheck[4] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorBt2020cYCC)
    {
        EdidReg->CdbCheck[5] = EdidReg->CdbCheck[5] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorBt2020YCC)
    {
        EdidReg->CdbCheck[6] = EdidReg->CdbCheck[6] + 1;
    }
    if(Edid[EdidOffset+2] & AvEdidCfgColorBt2020RGB)
    {
        EdidReg->CdbCheck[7] = EdidReg->CdbCheck[7] + 1;
    }
    if(Edid[EdidOffset+3] & AvEdidCfgColorDciP3)
    {
        EdidReg->CdbCheck[8] = EdidReg->CdbCheck[8] + 1;
    }
}
uint8 AvEdidFuncCeabVsdbHdmi(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    uint8   bLen = 0;
    uint8   VicLen = 0;
    if(AvEdidSupportDisplayMode & AvEdidBitDisplayModeGAME)
    {
        EdidReg->VsdbCheckList[0] = 1;
    }
    if(AvEdidSupportDisplayMode & AvEdidBitDisplayModeCINEMA)
    {
        EdidReg->VsdbCheckList[1] = 1;
    }
    if(AvEdidSupportDisplayMode & AvEdidBitDisplayModePHOTO)
    {
        EdidReg->VsdbCheckList[2] = 1;
    }
    if(AvEdidSupportDisplayMode & AvEdidBitDisplayModeGRAPHIC)
    {
        EdidReg->VsdbCheckList[3] = 1;
    }
    if(AvEdidSupportFreq & AvEdidBitFreq3G)
    {
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate30HZ)
        {
            EdidReg->VsdbCheckList[4] = 1;
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate25HZ)
        {
            EdidReg->VsdbCheckList[5] = 1;
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate24HZ)
        {
            EdidReg->VsdbCheckList[6] = 1;
            EdidReg->VsdbCheckList[7] = 1;
        }
    }
    if(AvEdidSupportColorDepth & AvEdidBitColorDepthAL)
    {
        EdidReg->VsdbCheckList[8] = 1;
    }
    if(AvEdidSupportColorDepth & AvEdidBitColorDepth48BIT)
    {
        EdidReg->VsdbCheckList[9] = 1;
    }
    if(AvEdidSupportColorDepth & AvEdidBitColorDepth36BIT)
    {
        EdidReg->VsdbCheckList[10] = 1;
    }
    if(AvEdidSupportColorDepth & AvEdidBitColorDepth30BIT)
    {
        EdidReg->VsdbCheckList[11] = 1;
    }
    if(AvEdidSupportColorDepth & AvEdidBitColorDepthY444)
    {
        EdidReg->VsdbCheckList[12] = 1;
    }
    if(AvEdidSupportColorDepth & AvEdidBitColorDepthDUAL_DVI)
    {
        EdidReg->VsdbCheckList[13] = 1;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupport3D)
    {
        EdidReg->VsdbCheckList[14] = 1;
    }
    if(AvEdidVsdbSpa != 0)
    {
        EdidReg->VsdbCheckList[15] = 1;
    }
    if(AvEdidSupportFreq & (AvEdidBitFreq6G | AvEdidBitFreq4P5G | AvEdidBitFreq3P75G | AvEdidBitFreq3G))
    {
        EdidReg->MaxTmdsClk = 300;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq2P25G)
    {
        EdidReg->MaxTmdsClk = 225;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq1P5G)
    {
        EdidReg->MaxTmdsClk = 150;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq750M)
    {
        EdidReg->MaxTmdsClk = 75;
    }
    else
    {
        EdidReg->MaxTmdsClk = 30;
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabVsdbHdmiRead(EdidReg, InEdid, InEdidOffset);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabVsdbHdmiRead(EdidReg, SinkEdid, SinkEdidOffset);
    }
    if(AvEdidCeaParamForce & AvEdidBitCeaVSDBHF)
    {
        EdidReg->MaxTmdsClk = 300;
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    OutEdid[OutEdidOffset+1] = EdidCeaIdVsdbHdmi[0];
    OutEdid[OutEdidOffset+2] = EdidCeaIdVsdbHdmi[1];
    OutEdid[OutEdidOffset+3] = EdidCeaIdVsdbHdmi[2];
    if((SelectValue == 1) ||
       ((AvEdidCeaParamForce & AvEdidBitCeaSPA) != 0) ||
       ((AvEdidCeaParamRemove & AvEdidBitCeaSPA) != 0))
    {
        OutEdid[OutEdidOffset+4] = (AvEdidVsdbSpa>>8) & 0xff;
        OutEdid[OutEdidOffset+5] = (AvEdidVsdbSpa>>0) & 0xff;
    }
    else
    {
        if(InEdidOffset != 0)
        {
            OutEdid[OutEdidOffset+4] = InEdid[InEdidOffset+4];
            OutEdid[OutEdidOffset+5] = InEdid[InEdidOffset+5];
        }
        else if(SinkEdidOffset != 0)
        {
            OutEdid[OutEdidOffset+4] = SinkEdid[SinkEdidOffset+4];
            OutEdid[OutEdidOffset+5] = SinkEdid[SinkEdidOffset+5];
        }
        else
        {
            OutEdid[OutEdidOffset+4] = 0x10;
            OutEdid[OutEdidOffset+5] = 0x00;
        }
    }
    if(EdidReg->VsdbCheckList[8] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgVsdbAl;
    }
    if(EdidReg->VsdbCheckList[9] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgVsdb48bit;
    }
    if(EdidReg->VsdbCheckList[10] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgVsdb36bit;
    }
    if(EdidReg->VsdbCheckList[11] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgVsdb30bit;
    }
    if(EdidReg->VsdbCheckList[12] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgVsdbY444;
    }
    if(EdidReg->VsdbCheckList[13] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgVsdbDualDvi;
    }
    OutEdid[OutEdidOffset+7] = EdidReg->MaxTmdsClk/5;
    if(EdidReg->VsdbCheckList[0] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgModeGame;
    }
    if(EdidReg->VsdbCheckList[1] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgModeCinema;
    }
    if(EdidReg->VsdbCheckList[2] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgModePhoto;
    }
    if(EdidReg->VsdbCheckList[3] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgModeGraphic;
    }
    OutEdid[OutEdidOffset+9] = 0x08;
    if(EdidReg->VsdbCheckList[4] == SelectValue)
    {
        OutEdid[OutEdidOffset+11+VicLen] = 1;
        VicLen = VicLen + 1;
    }
    if(EdidReg->VsdbCheckList[5] == SelectValue)
    {
        OutEdid[OutEdidOffset+11+VicLen] = 2;
        VicLen = VicLen + 1;
    }
    if(EdidReg->VsdbCheckList[6] == SelectValue)
    {
        OutEdid[OutEdidOffset+11+VicLen] = 3;
        VicLen = VicLen + 1;
    }
    if(EdidReg->VsdbCheckList[7] == SelectValue)
    {
        OutEdid[OutEdidOffset+11+VicLen] = 4;
        VicLen = VicLen + 1;
    }
    OutEdid[OutEdidOffset+10] = OutEdid[OutEdidOffset+10] | (VicLen<<5);
    if(EdidReg->VsdbCheckList[14] == SelectValue)
    {
        OutEdid[OutEdidOffset+9] = OutEdid[OutEdidOffset+9] | (1<<7);
    }
    if((EdidReg->VsdbCheckList[14] == SelectValue) || (VicLen != 0))
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | (1<<5);
    }
    bLen = 10+VicLen;
    OutEdid[OutEdidOffset] = (0x03<<5) + bLen;
    return bLen;
}
uint8 AvEdidFuncCeabVsdbHf(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    if(AvEdidSupportFreq & AvEdidBitFreq6G)
    {
        EdidReg->MaxCharRate = 600;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq4P5G)
    {
        EdidReg->MaxCharRate = 450;
    }
    else if(AvEdidSupportFreq & AvEdidBitFreq3P75G)
    {
        EdidReg->MaxCharRate = 375;
    }
    else
    {
        EdidReg->MaxCharRate = 0;
    }
    EdidReg->MaxFrlRate = 0;
    EdidReg->HfVrrMax = AvEdidVrrMax;
    EdidReg->HfVrrMin = AvEdidVrrMin;
    EdidReg->HfDscFrlRate = AvEdidDscFrlRate;
    EdidReg->HfDscMaxSlices = AvEdidDscMaxSlices;
    EdidReg->HfDscTotalChunkKBytes = AvEdidDscTotalChunkKBytes;
    if(AvEdidSupportFreq & AvEdidBitFreq6G)
    {
        if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY420)
        {
            if(AvEdidSupportColorDepth & AvEdidBitColorDepth48BIT)
            {
                EdidReg->VsdbHfCheck[0] = 1;
            }
        }
    }
    if(AvEdidSupportFreq & AvEdidBitFreq4P5G)
    {
        if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY420)
        {
            if(AvEdidSupportColorDepth & AvEdidBitColorDepth36BIT)
            {
                EdidReg->VsdbHfCheck[1] = 1;
            }
        }
    }
    if(AvEdidSupportFreq & AvEdidBitFreq3P75G)
    {
        if(AvEdidSupportColorSpace & AvEdidBitColorSpaceY420)
        {
            if(AvEdidSupportColorDepth & AvEdidBitColorDepth30BIT)
            {
                EdidReg->VsdbHfCheck[2] = 1;
            }
        }
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportSCDC)
    {
        EdidReg->VsdbHfCheck[3] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupport3DOsdDisparity)
    {
        EdidReg->VsdbHfCheck[4] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupport3DDualView)
    {
        EdidReg->VsdbHfCheck[5] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupport3DIndepedentView)
    {
        EdidReg->VsdbHfCheck[6] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportBelow340MScramble)
    {
        EdidReg->VsdbHfCheck[7] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportScdcReadRequest)
    {
        EdidReg->VsdbHfCheck[8] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportMdelta)
    {
        EdidReg->VsdbHfCheck[9] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportCinemaVrr)
    {
        EdidReg->VsdbHfCheck[10] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportCnmVrr)
    {
        EdidReg->VsdbHfCheck[11] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportFva)
    {
        EdidReg->VsdbHfCheck[12] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportAllm)
    {
        EdidReg->VsdbHfCheck[13] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportFapa)
    {
        EdidReg->VsdbHfCheck[14] = 1;
    }
    if(AvEdidSupportDsc & AvEdidBitSupportDsc1p2)
    {
        EdidReg->VsdbHfCheck[15] = 1;
    }
    if(AvEdidSupportDsc & AvEdidBitSupportDscNative420)
    {
        EdidReg->VsdbHfCheck[16] = 1;
    }
    if(AvEdidSupportDsc & AvEdidBitSupportDsc16bpc)
    {
        EdidReg->VsdbHfCheck[17] = 1;
    }
    if(AvEdidSupportDsc & AvEdidBitSupportDsc12bpc)
    {
        EdidReg->VsdbHfCheck[18] = 1;
    }
    if(AvEdidSupportDsc & AvEdidBitSupportDsc10bpc)
    {
        EdidReg->VsdbHfCheck[19] = 1;
    }
    if(AvEdidSupportVsdbHf & AvEdidBitSupportCcbpci)
    {
        EdidReg->VsdbHfCheck[20] = 1;
    }
    if(AvEdidSupportDsc & AvEdidBitSupportDscAllBpp)
    {
        EdidReg->VsdbHfCheck[21] = 1;
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabVsdbHfRead(EdidReg, InEdid, InCeabParam);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabVsdbHfRead(EdidReg, SinkEdid, SinkCeabParam);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    OutEdid[OutEdidOffset+0] = (3<<5) | 13;
    OutEdid[OutEdidOffset+1] = 0xD8;
    OutEdid[OutEdidOffset+2] = 0x5D;
    OutEdid[OutEdidOffset+3] = 0xC4;
    OutEdid[OutEdidOffset+4] = 0x01;
    if(EdidReg->VsdbHfCheck[0] == SelectValue)
    {
        OutEdid[OutEdidOffset+7] = OutEdid[OutEdidOffset+7] | AvEdidCfgHf48bit420;
    }
    if(EdidReg->VsdbHfCheck[1] == SelectValue)
    {
        OutEdid[OutEdidOffset+7] = OutEdid[OutEdidOffset+7] | AvEdidCfgHf36bit420;
    }
    if(EdidReg->VsdbHfCheck[2] == SelectValue)
    {
        OutEdid[OutEdidOffset+7] = OutEdid[OutEdidOffset+7] | AvEdidCfgHf30bit420;
    }
    if(EdidReg->VsdbHfCheck[3] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgHfScdcPresent;
    }
    if(EdidReg->VsdbHfCheck[4] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgHf3DOsdDisparity;
    }
    if(EdidReg->VsdbHfCheck[5] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgHfDualView;
    }
    if(EdidReg->VsdbHfCheck[6] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgHfIndpendentView;
    }
    if(EdidReg->VsdbHfCheck[7] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgHf340MScramble;
    }
    if(EdidReg->VsdbHfCheck[8] == SelectValue)
    {
        OutEdid[OutEdidOffset+6] = OutEdid[OutEdidOffset+6] | AvEdidCfgHfReadRequest;
    }
    if(EdidReg->VsdbHfCheck[9] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgHfMdelta;
    }
    if(EdidReg->VsdbHfCheck[10] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgHfCinemaVrr;
    }
    if(EdidReg->VsdbHfCheck[11] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgHfCnmVrr;
    }
    if(EdidReg->VsdbHfCheck[12] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgHfFva;
    }
    if(EdidReg->VsdbHfCheck[13] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgHfAllm;
    }
    if(EdidReg->VsdbHfCheck[14] == SelectValue)
    {
        OutEdid[OutEdidOffset+8] = OutEdid[OutEdidOffset+8] | AvEdidCfgHfFapa;
    }
    if(EdidReg->VsdbHfCheck[15] == SelectValue)
    {
        OutEdid[OutEdidOffset+11] = OutEdid[OutEdidOffset+11] | AvEdidCfgHfDsc1p2;
    }
    if(EdidReg->VsdbHfCheck[16] == SelectValue)
    {
        OutEdid[OutEdidOffset+11] = OutEdid[OutEdidOffset+11] | AvEdidCfgHfDscNative420;
    }
    if(EdidReg->VsdbHfCheck[17] == SelectValue)
    {
        OutEdid[OutEdidOffset+11] = OutEdid[OutEdidOffset+11] | AvEdidCfgHfDsc16bpc;
    }
    if(EdidReg->VsdbHfCheck[18] == SelectValue)
    {
        OutEdid[OutEdidOffset+11] = OutEdid[OutEdidOffset+11] | AvEdidCfgHfDsc12bpc;
    }
    if(EdidReg->VsdbHfCheck[19] == SelectValue)
    {
        OutEdid[OutEdidOffset+11] = OutEdid[OutEdidOffset+11] | AvEdidCfgHfDsc10bpc;
    }
    if(EdidReg->VsdbHfCheck[20] == SelectValue)
    {
        OutEdid[OutEdidOffset+6]  =  OutEdid[OutEdidOffset+6] | AvEdidCfgHfCcbpci;
    }
    if(EdidReg->VsdbHfCheck[21] == SelectValue)
    {
        OutEdid[OutEdidOffset+11] = OutEdid[OutEdidOffset+11] | AvEdidCfgHfDscAllBpp;
    }
    OutEdid[OutEdidOffset+5] = EdidReg->MaxCharRate/5;
    OutEdid[OutEdidOffset+7] = OutEdid[OutEdidOffset+7] | (EdidReg->MaxFrlRate << 4);
    OutEdid[OutEdidOffset+9] = ((EdidReg->HfVrrMax & 0x300)>>2) | EdidReg->HfVrrMin;
    OutEdid[OutEdidOffset+10] = EdidReg->HfVrrMax & 0xff;
    OutEdid[OutEdidOffset+12] = (EdidReg->HfDscFrlRate<<4) | EdidReg->HfDscMaxSlices;
    OutEdid[OutEdidOffset+13] = EdidReg->HfDscTotalChunkKBytes & 0x3f;
    return 13;
}
uint8 AvEdidFuncCeabVcdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    if(AvEdidSupportBasicFeature & AvEdidBitSupportYuvFullRange)
    {
        EdidReg->VcdbCheck[0] = 1;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupportRgbFullRange)
    {
        EdidReg->VcdbCheck[1] = 1;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupportVesaOverUnderScan)
    {
        EdidReg->VcdbCheck[2] = 1;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupportCeaOverUnderScan)
    {
        EdidReg->VcdbCheck[3] = 1;
    }
    if(AvEdidSupportBasicFeature & AvEdidBitSupportPtOverUnderScan)
    {
        EdidReg->VcdbCheck[6] = 1;
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabVcdbRead(EdidReg, InEdid, InEdidOffset);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabVcdbRead(EdidReg, SinkEdid, SinkEdidOffset);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    OutEdid[OutEdidOffset+0] = (7<<5) | 0x02;
    OutEdid[OutEdidOffset+1] = 0x00;
    if(EdidReg->VcdbCheck[2] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgVesaOverUnderScan;
    }
    else
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgVesaOnlyUnderScan;
    }
    if(EdidReg->VcdbCheck[3] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgCeaOverUnderScan;
    }
    else
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgCeaOnlyUnderScan;
    }
    if(EdidReg->VcdbCheck[6] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgPtOverUnderScan;
    }
    else
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgPtNoOverUnderScan;
    }
    if(EdidReg->VcdbCheck[0] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgYuvFullRange;
    }
    if(EdidReg->VcdbCheck[1] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgRgbFullRange;
    }
    return 2;
}
uint8 AvEdidFuncCeabY420Cmdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint16  SvdParam = 0;
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    uint8   i = 0;
    uint8   bLen = 1;
    uint8   MarkLen = bLen;
    uint8   SvdOffset = 0;
    uint8   SvdLen = 0;
    if(AvEdidSupportFreq & AvEdidBitFreq6G)
    {
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate50HZ)
        {
            EdidReg->Y420VdbCheck[0] = 1;
            EdidReg->Y420VdbCheck[2] = 1;
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate60HZ)
        {
            EdidReg->Y420VdbCheck[1] = 1;
            EdidReg->Y420VdbCheck[3] = 1;
        }
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabY420CmdbRead(EdidReg, InEdid, InEdidOffset);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabY420CmdbRead(EdidReg, SinkEdid, SinkEdidOffset);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    SvdParam = AvEdidFuncFindCeabTag(OutEdid,   0x02, 0);
    SvdOffset    = (SvdParam>>8) & 0xff;
    SvdLen       = SvdParam & 0xff;
    if(SvdLen != 0)
    {
        for(i=0;i<SvdLen;i++)
        {
            if(((OutEdid[SvdOffset+i+1] == 96)  && (EdidReg->Y420VdbCheck[0] == SelectValue)) ||
               ((OutEdid[SvdOffset+i+1] == 97)  && (EdidReg->Y420VdbCheck[1] == SelectValue)) ||
               ((OutEdid[SvdOffset+i+1] == 101) && (EdidReg->Y420VdbCheck[2] == SelectValue)) ||
               ((OutEdid[SvdOffset+i+1] == 102) && (EdidReg->Y420VdbCheck[3] == SelectValue)))
            {
                    OutEdid[OutEdidOffset+bLen+1] = OutEdid[OutEdidOffset+bLen+1] | (1<<(i%8));
                MarkLen = bLen + 1;
            }
            if(i%8 == 7)
            {
                bLen = bLen + 1;
            }
        }
    }
    else
    {
        return 0;
    }
    if(MarkLen == 1)
    {
        return 0;
    }
    OutEdid[OutEdidOffset+0] = (7<<5) | MarkLen;
    OutEdid[OutEdidOffset+1] = 0x0F;
    return MarkLen;
}
uint8 AvEdidFuncCeabY420Vdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    uint8   bLen = 1;
    if(AvEdidSupportFreq & (AvEdidBitFreq6G | AvEdidBitFreq4P5G | AvEdidBitFreq3P75G | AvEdidBitFreq3G))
    {
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate50HZ)
        {
            EdidReg->Y420VdbCheck[0] = 1;
            EdidReg->Y420VdbCheck[2] = 1;
        }
        if(AvEdidSupportFrameRate & AvEdidBitFrameRate60HZ)
        {
            EdidReg->Y420VdbCheck[1] = 1;
            EdidReg->Y420VdbCheck[3] = 1;
        }
    }
    if(InEdidOffset != 0)
    {
        if(InEdid[InEdidOffset+1] == 0x0F)
        {
            AvEdidFuncCeabY420CmdbRead(EdidReg, InEdid, InEdidOffset);
        }
        else if(InEdid[InEdidOffset+1] == 0x0E)
        {
            AvEdidFuncCeabY420VdbRead(EdidReg, InEdid, InEdidOffset);
        }
    }
    if(SinkEdidOffset != 0)
    {
        if(SinkEdid[SinkEdidOffset+1] == 0x0F)
        {
            AvEdidFuncCeabY420CmdbRead(EdidReg, SinkEdid, SinkEdidOffset);
        }
        else if(SinkEdid[SinkEdidOffset+1] == 0x0E)
        {
            AvEdidFuncCeabY420VdbRead(EdidReg, SinkEdid, SinkEdidOffset);
        }
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(EdidReg->Y420VdbCheck[0] == SelectValue)
    {
        OutEdid[OutEdidOffset+bLen+1] = 96;
        bLen = bLen + 1;
    }
    if(EdidReg->Y420VdbCheck[1] == SelectValue)
    {
        OutEdid[OutEdidOffset+bLen+1] = 97;
        bLen = bLen + 1;
    }
    if(EdidReg->Y420VdbCheck[2] == SelectValue)
    {
        OutEdid[OutEdidOffset+bLen+1] = 101;
        bLen = bLen + 1;
    }
    if(EdidReg->Y420VdbCheck[3] == SelectValue)
    {
        OutEdid[OutEdidOffset+bLen+1] = 102;
        bLen = bLen + 1;
    }
    if(bLen == 1)
    {
        return 0;
    }
    OutEdid[OutEdidOffset+0] = (7<<5) | bLen;
    OutEdid[OutEdidOffset+1] = 0x0E;
    return bLen;
}
uint8 AvEdidFuncCeabDolbyVisionSt(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   InEdidLen      = InCeabParam&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   BlockLen       = 0;
    uint8   SelectValue    = 0;
    if(AvEdidSupportColorimetry & AvEdidBitSupportHdrDolbyVision)
    {
        EdidReg->DolbyVisionCheck = EdidReg->DolbyVisionCheck + 1;
    }
    else
    {
        return 0;
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabVsvdbRead(EdidReg, InEdid, InEdidOffset);
        InEdidOffset = InEdidOffset + 2;
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabVsvdbRead(EdidReg, SinkEdid, SinkEdidOffset);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(EdidReg->DolbyVisionCheck == SelectValue)
    {
        if(SelectValue == 1)
        {
            AvEdidFuncBulkWriteData(EdidReg, 10, (uint8*)EdidCeaDolbyVision, 0, OutEdid, OutEdidOffset+2);
            BlockLen = 25;
        }
        else
        {
            BlockLen = InEdidLen;
            AvEdidFuncBulkWriteData(EdidReg, InEdidLen, InEdid, InEdidOffset, OutEdid, OutEdidOffset+2);
        }
    }
    OutEdid[OutEdidOffset+0] = (7<<5) | BlockLen;
    OutEdid[OutEdidOffset+1] = 0x01;
    return BlockLen;
}
uint8 AvEdidFuncCeabHdrSt(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    if(AvEdidSupportColorimetry & AvEdidBitSupportHdrGammaSdr)
    {
        EdidReg->HdrStCheck[0] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportHdrGammaHdr)
    {
        EdidReg->HdrStCheck[1] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportHdrGammaSmpte)
    {
        EdidReg->HdrStCheck[2] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportHdrGammaHybrid)
    {
        EdidReg->HdrStCheck[3] = 1;
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabHdrStRead(EdidReg, InEdid, InEdidOffset);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabHdrStRead(EdidReg, SinkEdid, SinkEdidOffset);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(EdidReg->HdrStCheck[0] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgHdrGammaSdr;
    }
    if(EdidReg->HdrStCheck[1] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgHdrGammaHdr;
    }
    if(EdidReg->HdrStCheck[2] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgHdrGammaSmpte;
    }
    if(EdidReg->HdrStCheck[3] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgHdrGammaHybrid;
    }
    OutEdid[OutEdidOffset+0] = (7<<5) | 0x03;
    OutEdid[OutEdidOffset+1] = 0x06;
    OutEdid[OutEdidOffset+3] = 0x01;
    return 3;
}
/* EDID Function Declare End */
uint8 AvEdidFuncCeabCdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset)
{
    uint8   InEdidOffset   = (InCeabParam>>8)&0xff;
    uint8   SinkEdidOffset = (SinkCeabParam>>8)&0xff;
    uint8   SelectValue = 0;
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorxvYcc601)
    {
        EdidReg->CdbCheck[0] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorxvYcc709)
    {
        EdidReg->CdbCheck[1] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorsYCC601)
    {
        EdidReg->CdbCheck[2] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorAdobeYcc601)
    {
        EdidReg->CdbCheck[3] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorAdobeRGB)
    {
        EdidReg->CdbCheck[4] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorBt2020cYCC)
    {
        EdidReg->CdbCheck[5] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorBt2020YCC)
    {
        EdidReg->CdbCheck[6] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorBt2020RGB)
    {
        EdidReg->CdbCheck[7] = 1;
    }
    if(AvEdidSupportColorimetry & AvEdidBitSupportColorDciP3)
    {
        EdidReg->CdbCheck[8] = 1;
    }
    if(InEdidOffset != 0)
    {
        AvEdidFuncCeabCdbRead(EdidReg, InEdid, InEdidOffset);
    }
    if(SinkEdidOffset != 0)
    {
        AvEdidFuncCeabCdbRead(EdidReg, SinkEdid, SinkEdidOffset);
    }
    SelectValue = 1;
    if(InEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(SinkEdidOffset != 0)
    {
        SelectValue = SelectValue + 1;
    }
    if(EdidReg->CdbCheck[0] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorxvYcc601;
    }
    if(EdidReg->CdbCheck[1] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorxvYcc709;
    }
    if(EdidReg->CdbCheck[2] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorsYCC601;
    }
    if(EdidReg->CdbCheck[3] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorAdobeYcc601;
    }
    if(EdidReg->CdbCheck[4] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorAdobeRGB;
    }
    if(EdidReg->CdbCheck[5] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorBt2020cYCC;
    }
    if(EdidReg->CdbCheck[6] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorBt2020YCC;
    }
    if(EdidReg->CdbCheck[7] == SelectValue)
    {
        OutEdid[OutEdidOffset+2] = OutEdid[OutEdidOffset+2] | AvEdidCfgColorBt2020RGB;
    }
    if(EdidReg->CdbCheck[8] == SelectValue)
    {
        OutEdid[OutEdidOffset+3] = OutEdid[OutEdidOffset+3] | AvEdidCfgColorDciP3;
    }
    OutEdid[OutEdidOffset+0] = (7<<5) | 0x03;
    OutEdid[OutEdidOffset+1] = 0x05;
    return 3;
}

#endif
