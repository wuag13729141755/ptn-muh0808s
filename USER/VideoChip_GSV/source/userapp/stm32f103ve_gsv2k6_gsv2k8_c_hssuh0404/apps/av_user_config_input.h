
#define AvVersion                  "20200429"

#define AvMatrix4x4Mode            0

//==============config for GSV2006==========================
#if _ENABLE_CHIP_GSV2006
#define GSV2K6                     1
#else
#define GSV2K6                     0
#endif
#ifndef AvGsv2k6DeviceNumber
#define AvGsv2k6DeviceNumber       0
#endif
#define AvGsv2k6PortNumber         10
#if GSV2K6
#define Gsv2kResourceSize_2K6      (Gsv2k6ResourceSize*AvGsv2k6DeviceNumber)
#else
#define Gsv2kResourceSize_2K6      0
#endif
#define Gsv2k6AudioBypass          0
#define Gsv2k6IgnoreDviInTx        0
#if AvMatrix4x4Mode
#define Gsv2k6MuxMode              1
#define Gsv2k6DualSplitter         1
#define Gsv2k6In2to1to4Mode        0
#define Gsv2k6SingleInput          0
#define Gsv2k6ArbitaryMode         0
#else
#ifndef Gsv2k6MuxMode
#define Gsv2k6MuxMode              0
#endif
#ifndef Gsv2k6DualSplitter
#define Gsv2k6DualSplitter         0
#endif
#ifndef Gsv2k6In2to1to4Mode
#define Gsv2k6In2to1to4Mode        1
#endif //add by wf8421
#ifndef Gsv2k6SingleInput
#define Gsv2k6SingleInput          0
#endif //add by wf8421
#ifndef Gsv2k6ArbitaryMode
#define Gsv2k6ArbitaryMode         0
#endif //add by wf8421
#endif
//#define Gsv2k6IgnoreDviInTx        0
//#define Gsv2k6RxABtoTxAB           1
//#define Gsv2k6RxABtoTxAC           0
//#define Gsv2k6TxSwapPolarity       1
//#define Gsv2k6TxSwapChannel        1
//==========================================================

//==============config for GSV2008==========================
#if _ENABLE_CHIP_GSV2008
#define GSV2K8                     1
#else
#define GSV2K8                     0
#endif
#ifndef AvGsv2k8DeviceNumber
#define AvGsv2k8DeviceNumber       0
#endif
#if AvMatrix4x4Mode
#define AvGsv2k8PortNumber         10
#else
#define AvGsv2k8PortNumber         12
#endif
#if GSV2K8
#define Gsv2kResourceSize_2K8      (Gsv2k8ResourceSize*AvGsv2k8DeviceNumber)
#else
#define Gsv2kResourceSize_2K8      0
#endif
#define Gsv2k8MuxMode              0
#define Gsv2k8AudioBypass          0
#define Gsv2k8TxSwapPolarity       0
#define Gsv2k8TxSwapChannel        0
//==========================================================

//==============config for GSV2002==========================
#if _ENABLE_CHIP_GSV2002
#define GSV2K2                     1
#else
#define GSV2K2                     0
#endif
#ifndef AvGsv2k2DeviceNumber
#define AvGsv2k2DeviceNumber       0
#endif
#define AvGsv2k2PortNumber         7
#if GSV2K2
#define Gsv2kResourceSize_2K2       (AvGsv2k2DeviceNumber*Gsv2k2ResourceSize)
#else
#define Gsv2kResourceSize_2K2       0
#endif
#define Gsv2k2MuxMode              0
#define Gsv2k2AudioBypass          0
#define Gsv2k2RomFromOtp           0
#define Gsv2k2SingleInput          0
//==========================================================

//==============config for GSV2K1==========================
#if _ENABLE_CHIP_GSV2001
#define GSV2K1                     1
#else
#define GSV2K1                     0
#endif
#ifndef AvGsv2k1DeviceNumber
#define AvGsv2k1DeviceNumber       0
#endif
#define AvGsv2k1PortNumber         10
#if GSV2K1
#define Gsv2kResourceSize_2K1       (AvGsv2k1DeviceNumber*Gsv2k1ResourceSize)
#else
#define Gsv2kResourceSize_2K1       0
#endif
#define Gsv2k1MuxMode              0
#define Gsv2k1AudioBypass          0
#define Gsv2k1RomFromOtp           0
#define Gsv2k1SingleInput          0
//==========================================================

//==============config for GSV2K11==========================
#if _ENABLE_CHIP_GSV2011
#define GSV2K11                     1
#else
#define GSV2K11                     0
#endif
#ifndef AvGsv2k11DeviceNumber
#define AvGsv2k11DeviceNumber       0
#endif
#define AvGsv2k11PortNumber         9
#define Gsv2k11MuxMode              0
#define Gsv2k11AudioBypass          0
#if GSV2K11
#define Gsv2kResourceSize_2K11      (Gsv2k11ResourceSize*AvGsv2k11DeviceNumber)
#else
#define Gsv2kResourceSize_2K11      0
#endif

#ifndef AvGsv2k11ComBusCfg
#define AvGsv2k11ComBusCfg          3
#endif

#if GSV2K11
#define AvEnableVideoLogicBus       1
#ifndef RES_DEFAULT_MAX
#define RES_DEFAULT_MAX             11
#endif
#endif

#define Gsv2k11I2SBitAlignment     0
//=========================================================

//==============config for GSV5K1==========================
#if _ENABLE_CHIP_GSV5100
#define GSV5K1                     1
#else
#define GSV5K1                     0
#endif
#ifndef AvGsv5k1DeviceNumber
#define AvGsv5k1DeviceNumber       0
#endif

#define AvGsv5k1PortNumber         8

#define Gsv5k1MuxMode              0
#define Gsv5k1AudioBypass          0
#define Gsv5k1RomFromOtp           0
#if GSV5K1
#define Gsv5kResourceSize_5K1      (Gsv5k1ResourceSize*AvGsv5k1DeviceNumber)
#else
#define Gsv5kResourceSize_5K1      0
#endif

#define Gsv5k1RX                   0
#define Gsv5k1TX                   1

#ifndef Gsv5k1WorkMode
#define Gsv5k1WorkMode             Gsv5k1TX
#endif

#ifndef Gsv5k1RxSetTxBHdmiMode
#define Gsv5k1RxSetTxBHdmiMode     0
#endif

#ifndef Gsv5k1TxUseRxBHdmiMode
#define Gsv5k1TxUseRxBHdmiMode     0
#endif

#ifndef Gsv5k1DisableTxAHdmi
#define Gsv5k1DisableTxAHdmi       0
#endif

#ifndef Gsv5k1UseFastEqMode
#define Gsv5k1UseFastEqMode        0
#endif

#ifndef Gsv5k1SingleInput
#define Gsv5k1SingleInput          0
#endif

#if GSV5K1
#define AvEthTxExternalInductor    1
#define AvEthRxExternalInductor    1
#define AvEthExternalPullUpResitor 0

#ifndef AvHdmiOnEthernet
#define AvHdmiOnEthernet           1
#endif
#define AvEnableCcaFeature         0
#if Gsv5k1RxSetTxBHdmiMode
#define AvEthCascading             0
#else
#define AvEthCascading             1
#endif
#define AvCompressionLowThreshold  55
#define AvEnableHdcpMatchInput     0
#define AvEnableColorMatchInput    0
#define AvEnableDitherView         1
#else
#define AvEthTxExternalInductor    0
#define AvEthRxExternalInductor    0
#define AvEthExternalPullUpResitor 0

#define AvHdmiOnEthernet           0
#define AvEnableCcaFeature         0
#define AvEthCascading             0
#define AvCompressionLowThreshold  55
#define AvEnableHdcpMatchInput     0
#define AvEnableColorMatchInput    0
#define AvEnableDitherView         0
#endif
//=========================================================

//==============config for common==========================
#define Gsv2kResourceSize          (Gsv2kResourceSize_2K6+Gsv2kResourceSize_2K8+Gsv2kResourceSize_2K2+Gsv2kResourceSize_2K1+Gsv2kResourceSize_2K11+Gsv5kResourceSize_5K1)
#define GsvTotalDevNumber          (AvGsv2k6DeviceNumber+AvGsv2k8DeviceNumber+AvGsv2k2DeviceNumber+AvGsv2k1DeviceNumber+AvGsv2k11DeviceNumber+AvGsv5k1DeviceNumber)

#ifndef AvForceDefaultEdid
#define AvForceDefaultEdid         0
#endif
#define AvStrictEdidRule           0
#define AvEdidMaxSize              256
#define AvEnableCecFeature         0
#define AvCecLinkageMode           0
#define AvCecMessageEvent          AvEnableCecFeature
#define AvCecDataReadingInEvent    AvEnableCecFeature
#define AvCecDataSendingOutEvent   AvEnableCecFeature
#define AvCecGsvVendorIdByte1      0xa5
#define AvCecGsvVendorIdByte2      0xa5
#define AvCecGsvVendorIdByte3      0xa5
#define AvCecLogicAddress          0x5
#ifndef AvEnableAudioTTLInput
#define AvEnableAudioTTLInput      0
#endif
#ifndef AvEnableInternalVideoGen
#define AvEnableInternalVideoGen   0
#endif
#ifndef AvEnableHdcp2p2Feature
#define AvEnableHdcp2p2Feature     1
#endif
#ifndef AvEnableSimplifyHdcp
#define AvEnableSimplifyHdcp       1
#endif

#ifndef AvEnableForceOutHdcp
#define AvEnableForceOutHdcp       0
#endif
#ifndef AvEnableTTLAudioModeI2S
#define AvEnableTTLAudioModeI2S     0
#endif

#define AvEnableUartInput          0
#define AvEnableIntegrityCheck     0
#define AvEnableKeyInput           0
#define AvKeyDelayThreshold        1
#define AvIrdaFunctionInput        0

#ifndef RxHpdDelayExpireThreshold
#define RxHpdDelayExpireThreshold  3
#endif //add by wf8421
#define RxLockDelayExpireThreshold 1
#define RxEQDelayExpireThreshold   2
#ifndef TxVideoManageThreshold
#define TxVideoManageThreshold     2
#endif
#ifndef TxHdcpManageThreshold
#define TxHdcpManageThreshold      5// 30    // 80
#endif
#define AvDontCareEdidSpa          0
#define AvNoLinkageMode            1
#define AvAllowHpdLowEdidRead      0
#define AvEnableDetailTiming       1
#define AvHdcpRxErrorThreshold     10
#define AvHdcpTxErrorThreshold     5
#define AvEdidStoredInRam          0
#define AvEdidSameNoAcknowledge    1
#define AvEdidErrorThreshold       3//10
#define AvEnableHdcp1p4BksvCheck   0
#define AvEnableTxScdcCedCheck     0
#define AvEnableSwitchRxPortsWithoutTMDSTermDisabling   0
#define AvEnableTxCtsPhySetting    0
#ifndef AvEnableInternalAudioGen
#define AvEnableInternalAudioGen   0
#endif
#ifndef AvEnableInternalClockGen
#define AvEnableInternalClockGen   0
#endif

#if _ENABLE_GSV_CHIP_DEBUG_MESSAGE
#define EnableGsvChipDebug         1
#else
#define EnableGsvChipDebug         0
#endif //add by wf8421

#if EnableGsvChipDebug
#define AvEnableDebugMessage       1
#define AvEnableDebugHdcp          1
#define AvEnableDebugFsm           1
#define AvEnableDebugUapi          1
#define AvEnableDebugAVIF          1
#else
#define AvEnableDebugMessage       0
#define AvEnableDebugHdcp          0
#define AvEnableDebugFsm           0
#define AvEnableDebugUapi          0
#define AvEnableDebugAVIF          0
#endif

#ifndef AvAllowHpdLowPixelClockRead
#define AvAllowHpdLowPixelClockRead     0
#endif

#ifndef AvEnableSourceChangeUseDemo
#define AvEnableSourceChangeUseDemo     0
#endif

#ifndef AvGsv2k2Video420ToRgb_422_444
#define AvGsv2k2Video420ToRgb_422_444   (0&&GSV2K2)
#endif

#ifndef AvGsv2k8VideoRgb_422_444To420
#define AvGsv2k8VideoRgb_422_444To420   (0&&GSV2K8)
#endif

#ifndef AvGsv2kxVideoCscForValensTx
#define AvGsv2kxVideoCscForValensTx     (0)
#endif

#ifndef AvGsv2kxVideoCscForValensRx
#define AvGsv2kxVideoCscForValensRx     (0)
#endif

#ifndef AvGsv2kxVideoHdcp14ForValensTxProblem
#define AvGsv2kxVideoHdcp14ForValensTxProblem   (0)
#endif

#ifndef AvGsv2kxVideoHdcp14ForValensRxProblem
#define AvGsv2kxVideoHdcp14ForValensRxProblem   (0)
#endif

#ifndef AvGsv2kxTxHdcpFailOut
#define AvGsv2kxTxHdcpFailOut                   (0)
#endif

#ifndef AvGsv2kxVideoDSCEnable
#define AvGsv2kxVideoDSCEnable                  (1)
#endif

#ifndef AvGsv2kxVideoForceOutputHdmi20Enable    // 解决莫尼迪这类信号分析仪，不支持SCDC的EDID，但是机器支持HDMI2.0的机器
#define AvGsv2kxVideoForceOutputHdmi20Enable    (0)
#endif

#ifndef AvEnableInternalVideoGen_5K1
#define AvEnableInternalVideoGen_5K1    (AvEnableInternalVideoGen)
#endif

#define AvGsv2k8PortRxNumber     4
#define AvGsv2k8AudioNumber      2

#define AvGsv2k2PortRxNumber     1
#define AvGsv2k2AudioNumber      1

#if GSV2K8
#define PortRxNumber     AvGsv2k8PortRxNumber
#define AudioNumber      AvGsv2k8AudioNumber
#else   // 2k2, 2k6
#define PortRxNumber     AvGsv2k2PortRxNumber
#define AudioNumber      AvGsv2k2AudioNumber
#endif

#ifndef AvGsvTxPhyValue_GTR340M
#define AvGsvTxPhyValue_GTR340M                 (0x54)
#endif

#ifndef AvGsvTxPhyValue_LEQ340M
#define AvGsvTxPhyValue_LEQ340M                 (0x50)
#endif

#ifndef AvGsv2k11CopyTxEdid
#define AvGsv2k11CopyTxEdid                     0
#endif

#ifndef AvGsv2kxTXSwitchHDMIAndDVI
#define AvGsv2kxTXSwitchHDMIAndDVI              (0)
#endif

#ifndef AvGsv2kxTXDVIFornatWithOutHDCP
#define AvGsv2kxTXDVIFornatWithOutHDCP          (0)
#endif

#ifndef AvGsv2k11VideoDSCHRxToHTxEnable
#define AvGsv2k11VideoDSCHRxToHTxEnable         (0)
#endif

