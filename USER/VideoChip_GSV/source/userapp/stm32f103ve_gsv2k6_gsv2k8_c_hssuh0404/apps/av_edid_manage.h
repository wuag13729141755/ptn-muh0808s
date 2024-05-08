#include "av_config.h"
#ifndef __av_edid_manage_h
#define __av_edid_manage_h
#define AvEdidIDProductCode        0x0001
#define AvEdidIDSerialNumber       0x00000001
#define AvEdidManufactureYear      2018
#define AvEdidManufactureWeek      8
#define AvEdidScreenHSize          0x79
#define AvEdidScreenVSize          0x44
#define AvEdidBitVesaGENERAL                 1<<0
#define AvEdidBitVesaVideoInputDEF           1<<1
#define AvEdidBitVesaDPM                     1<<2
#define AvEdidBitVesaGAMMA                   1<<3
#define AvEdidBitVesaScreenSIZE              1<<4
#define AvEdidBitVesaColor                   1<<5
#define AvEdidBitVesaTiming                  1<<6
#define AvEdidBitVesaStandardTIMING          1<<7
#define AvEdidBitVesaMaxClk                  1<<8
#define AvEdidBitCeaAUDIO                     1<<0
#define AvEdidBitCeaSVD                       1<<1
#define AvEdidBitCeaNATIVE                    1<<2
#define AvEdidBitCeaVSDBHDMI                  1<<3
#define AvEdidBitCeaVSDBHF                    1<<4
#define AvEdidBitCeaSPA                       1<<5
#define AvEdidBitCeabVCDB                     1<<0
#define AvEdidBitCeabVSVDB                    1<<1
#define AvEdidBitCeabVDDDB                    1<<2
#define AvEdidBitCeabCDB                      1<<3
#define AvEdidBitCeabHDR_STATIC               1<<4
#define AvEdidBitCeabHDR_DYNAMIC              1<<5
#define AvEdidBitCeabVFPDB                    1<<6
#define AvEdidBitCeabY420VDB                  1<<7
#define AvEdidBitCeabY420CMDB                 1<<8
#define AvEdidBitCeabVSADB                    1<<9
#define AvEdidBitCeabADB                      1<<10
#define AvEdidBitCeabRCDB                     1<<11
#define AvEdidBitCeabSLDB                     1<<12
#define AvEdidBitCeabDTD                      1<<13
#define AvEdidBitCeabHDR_DOLBYVISION          1<<14
#define AvEdidBitDpmStandby                   1<<0
#define AvEdidBitDpmSuspend                   1<<1
#define AvEdidBitDpmActiveOff                 1<<2
#define AvEdidBitFreq6G                       1<<0
#define AvEdidBitFreq4P5G                     1<<1
#define AvEdidBitFreq3P75G                    1<<2
#define AvEdidBitFreq3G                       1<<3
#define AvEdidBitFreq2P25G                    1<<4
#define AvEdidBitFreq1P5G                     1<<5
#define AvEdidBitFreq750M                     1<<6
#define AvEdidBitFreq270M                     1<<7
#define AvEdidBitFreq135M                     1<<8
#define AvEdidBitFrameRate60HZ                1<<0
#define AvEdidBitFrameRate50HZ                1<<1
#define AvEdidBitFrameRate30HZ                1<<2
#define AvEdidBitFrameRate25HZ                1<<3
#define AvEdidBitFrameRate24HZ                1<<4
#define AvEdidBitFrameRate56HZ                1<<5
#define AvEdidBitFrameRate67HZ                1<<6
#define AvEdidBitFrameRate70HZ                1<<7
#define AvEdidBitFrameRate72HZ                1<<8
#define AvEdidBitFrameRate75HZ                1<<9
#define AvEdidBitFrameRate87HZ                1<<10
#define AvEdidBitFrameRate88HZ                1<<11
#define AvEdidBitColorSpaceRGB                1<<0
#define AvEdidBitColorSpaceY444               1<<1
#define AvEdidBitColorSpaceY422               1<<2
#define AvEdidBitColorSpaceY420               1<<3
#define AvEdidBitColorDepth48BIT              1<<0
#define AvEdidBitColorDepth36BIT              1<<1
#define AvEdidBitColorDepth30BIT              1<<2
#define AvEdidBitColorDepthAL                 1<<3
#define AvEdidBitColorDepthY444               1<<4
#define AvEdidBitColorDepthDUAL_DVI           1<<5
#define AvEdidBitDisplayModeGAME              1<<0
#define AvEdidBitDisplayModeCINEMA            1<<1
#define AvEdidBitDisplayModePHOTO             1<<2
#define AvEdidBitDisplayModeGRAPHIC           1<<3
#define AvEdidBitSupportSCDC                  1<<0
#define AvEdidBitSupport3DOsdDisparity        1<<1
#define AvEdidBitSupport3DDualView            1<<2
#define AvEdidBitSupport3DIndepedentView      1<<3
#define AvEdidBitSupportBelow340MScramble     1<<4
#define AvEdidBitSupportScdcReadRequest       1<<5
#define AvEdidBitSupportCcbpci                1<<6
#define AvEdidBitSupportMdelta                1<<7
#define AvEdidBitSupportCinemaVrr             1<<8
#define AvEdidBitSupportCnmVrr                1<<9
#define AvEdidBitSupportFva                   1<<10
#define AvEdidBitSupportAllm                  1<<11
#define AvEdidBitSupportFapa                  1<<12
#define AvEdidBitSupportFrl                   1<<13
#define AvEdidBitSupportDsc1p2                1<<0
#define AvEdidBitSupportDscNative420          1<<1
#define AvEdidBitSupportDscAllBpp             1<<2
#define AvEdidBitSupportDsc16bpc              1<<3
#define AvEdidBitSupportDsc12bpc              1<<4
#define AvEdidBitSupportDsc10bpc              1<<5
#define AvEdidBitSupportColorxvYcc601         1<<0
#define AvEdidBitSupportColorxvYcc709         1<<1
#define AvEdidBitSupportColorsYCC601          1<<2
#define AvEdidBitSupportColorAdobeYcc601      1<<3
#define AvEdidBitSupportColorAdobeRGB         1<<4
#define AvEdidBitSupportColorBt2020cYCC       1<<5
#define AvEdidBitSupportColorBt2020YCC        1<<6
#define AvEdidBitSupportColorBt2020RGB        1<<7
#define AvEdidBitSupportColorDciP3            1<<8
#define AvEdidBitSupportHdrGammaSdr           1<<9
#define AvEdidBitSupportHdrGammaHdr           1<<10
#define AvEdidBitSupportHdrGammaSmpte         1<<11
#define AvEdidBitSupportHdrGammaHybrid        1<<12
#define AvEdidBitSupportHdrDolbyVision        1<<13
#define AvEdidBitSupportAudio                 1<<0
#define AvEdidBitSupport3D                    1<<1
#define AvEdidBitSupportYuvFullRange          1<<2
#define AvEdidBitSupportRgbFullRange          1<<3
#define AvEdidBitSupportVesaOverUnderScan     1<<4
#define AvEdidBitSupportCeaOverUnderScan      1<<5
#define AvEdidBitSupportPtOverUnderScan       1<<6
#define AvEdidBitAudioFormatLPCM              1<<0
#define AvEdidBitAudioFormatAC3               1<<1
#define AvEdidBitAudioFormatMPEG1             1<<2
#define AvEdidBitAudioFormatMP3               1<<3
#define AvEdidBitAudioFormatMPEG2             1<<4
#define AvEdidBitAudioFormatAACLC             1<<5
#define AvEdidBitAudioFormatDTS               1<<6
#define AvEdidBitAudioFormatATRAC             1<<7
#define AvEdidBitAudioFormatDSD               1<<8
#define AvEdidBitAudioFormatENHANCED_AC3      1<<9
#define AvEdidBitAudioFormatDTSHD             1<<10
#define AvEdidBitAudioFormatMAT               1<<11
#define AvEdidBitAudioFormatDST               1<<12
#define AvEdidBitAudioFormatWMAPRO            1<<13
#define AvEdidBitSf32KHZ               1<<0
#define AvEdidBitSf44KHZ               1<<1
#define AvEdidBitSf48KHZ               1<<2
#define AvEdidBitSf88KHZ               1<<3
#define AvEdidBitSf96KHZ               1<<4
#define AvEdidBitSf176KHZ              1<<5
#define AvEdidBitSf192KHZ              1<<6
#define AvEdidBitAud16bit              1<<0
#define AvEdidBitAud20bit              1<<1
#define AvEdidBitAud24bit              1<<2
#define AvEdidCfgDpmStandby            1<<7
#define AvEdidCfgDpmSuspend            1<<6
#define AvEdidCfgDpmActiveOff          1<<5
#define AvEdidCfgVsdbAl                1<<7
#define AvEdidCfgVsdb48bit             1<<6
#define AvEdidCfgVsdb36bit             1<<5
#define AvEdidCfgVsdb30bit             1<<4
#define AvEdidCfgVsdbY444              1<<3
#define AvEdidCfgVsdbDualDvi           1<<0
#define AvEdidCfgModeGame              1<<3
#define AvEdidCfgModeCinema            1<<2
#define AvEdidCfgModePhoto             1<<1
#define AvEdidCfgModeGraphic           1<<0
#define AvEdidCfgHfScdcPresent         1<<7
#define AvEdidCfgHfReadRequest         1<<6
#define AvEdidCfgHfCcbpci              1<<4
#define AvEdidCfgHf340MScramble        1<<3
#define AvEdidCfgHfIndpendentView      1<<2
#define AvEdidCfgHfDualView            1<<1
#define AvEdidCfgHf3DOsdDisparity      1<<0
#define AvEdidCfgHf48bit420            1<<2
#define AvEdidCfgHf36bit420            1<<1
#define AvEdidCfgHf30bit420            1<<0
#define AvEdidCfgHfMdelta              1<<5
#define AvEdidCfgHfCinemaVrr           1<<4
#define AvEdidCfgHfCnmVrr              1<<3
#define AvEdidCfgHfFva                 1<<2
#define AvEdidCfgHfAllm                1<<1
#define AvEdidCfgHfFapa                1<<0
#define AvEdidCfgHfDsc1p2              1<<7
#define AvEdidCfgHfDscNative420        1<<6
#define AvEdidCfgHfDscAllBpp           1<<3
#define AvEdidCfgHfDsc16bpc            1<<2
#define AvEdidCfgHfDsc12bpc            1<<1
#define AvEdidCfgHfDsc10bpc            1<<0
#define AvEdidCfgYuvFullRange          1<<7
#define AvEdidCfgRgbFullRange          1<<6
#define AvEdidCfgPtOverUnderScan       3<<4
#define AvEdidCfgVesaOverUnderScan     3<<2
#define AvEdidCfgCeaOverUnderScan      3<<0
#define AvEdidCfgVesaOnlyUnderScan     2<<2
#define AvEdidCfgCeaOnlyUnderScan      2<<0
#define AvEdidCfgPtNoOverUnderScan     0<<4
#define AvEdidCfgHdrGammaSdr           1<<0
#define AvEdidCfgHdrGammaHdr           1<<1
#define AvEdidCfgHdrGammaSmpte         1<<2
#define AvEdidCfgHdrGammaHybrid        1<<3
#define AvEdidCfgColorxvYcc601         1<<0
#define AvEdidCfgColorxvYcc709         1<<1
#define AvEdidCfgColorsYCC601          1<<2
#define AvEdidCfgColorAdobeYcc601      1<<3
#define AvEdidCfgColorAdobeRGB         1<<4
#define AvEdidCfgColorBt2020cYCC       1<<5
#define AvEdidCfgColorBt2020YCC        1<<6
#define AvEdidCfgColorBt2020RGB        1<<7
#define AvEdidCfgColorDciP3            1<<7
/* EDID Structure Declare Start */
typedef struct
{
    uint8  EdidCeaVicCheck [23];
    uint8  EdidCeaAudioCheck [14];
    uint8  EdidCeaAudioSfTable [14];
    uint8  EdidCeaAudioFmtTable [14];
    uint8  EdidCeaAudioByte3 [14];
    uint8  VsdbCheckList [16];
    uint8  MaxFrlRate;
    uint16 MaxTmdsClk;
    uint16 VesaMaxClk;
    uint16 MaxCharRate;
    uint16 HfVrrMax;
    uint8  HfVrrMin;
    uint8  HfDscFrlRate;
    uint8  HfDscMaxSlices;
    uint8  HfDscTotalChunkKBytes;
    uint8  VsdbHfCheck [22];
    uint8  VcdbCheck [7];
    uint8  Y420VdbCheck [4];
    uint8  HdrStCheck [4];
    uint8  CdbCheck [9];
    uint8  DolbyVisionCheck;
} AvEdidReg;
/* EDID Structure Declare End */
    void AvEdidFuncStructInit(AvEdidReg *EdidReg);
    void AvEdidFuncBulkAddValue(uint8 *table,uint8 total,uint8 start,uint8 value);
    void AvEdidFuncBulkDropValue(uint8 *table,uint8 total,uint8 start,uint8 value);
    void AvEdidFuncBulkWriteData(AvEdidReg *EdidReg,uint8 total,uint8 *FromTable,uint8 FromOffset,uint8 *OutEdid,uint8 ToOffset);
    uint16 AvEdidFuncFindCeabTag(uint8 *EdidContent,uint8 TagNumber,uint8 SubVersion);
    uint8 AvEdidFuncBlockPermit(AvEdidReg *EdidReg,uint16 InCeabParam,uint8 InEdidExist,uint16 SinkCeabParam,uint8 SinkEdidExist);
    void AvEdidFuncCheckSum(uint8 *OutEdid);
    void AvEdidFuncVesaProcess(AvEdidReg *EdidReg,uint8 *InEdid,uint8 *SinkEdid,uint8 *OutEdid);
    void AvEdidFunFullAnalysis(AvEdidReg *EdidReg,uint8 *InEdid);
    void AvEdidFuncCeaProcess(AvEdidReg *EdidReg,uint8 *InEdid,uint8 *SinkEdid,uint8 *OutEdid);
    void AvEdidFuncDtdWrite(AvEdidReg *EdidReg,uint8 BlockNum,uint8 *OutEdid,uint8 offset);
    uint8 AvEdidFuncCeabSVD(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabAudio(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    void AvEdidFuncCeabAudioRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidLen,uint8 EdidOffset);
    void AvEdidFuncCeabVsdbHdmiRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset);
    void AvEdidFuncCeabVsdbHfRead(AvEdidReg *EdidReg,uint8 *Edid,uint16 EdidParam);
    void AvEdidFuncCeabVcdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset);
    void AvEdidFuncCeabY420VdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset);
    void AvEdidFuncCeabVsvdbRead(AvEdidReg *EdidReg,uint8 *InEdid,uint8 iEdidOffset);
    void AvEdidFuncCeabY420CmdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset);
    void AvEdidFuncCeabHdrStRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset);
    void AvEdidFuncCeabCdbRead(AvEdidReg *EdidReg,uint8 *Edid,uint8 EdidOffset);
    uint8 AvEdidFuncCeabVsdbHdmi(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabVsdbHf(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabVcdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabY420Cmdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabY420Vdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabDolbyVisionSt(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabHdrSt(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
    uint8 AvEdidFuncCeabCdb(AvEdidReg *EdidReg,uint8 *InEdid,uint16 InCeabParam,uint8 *SinkEdid,uint16 SinkCeabParam,uint8 *OutEdid,uint8 OutEdidOffset);
#endif
