#ifndef __gsv2k6_h
#define __gsv2k6_h

#include "av_config.h"

#define Gsv2k6ResourceSize  (826+(AvEnableDetailTiming*72)+(AvEnableInternalVideoGen*40)+(AvEnableCecFeature*280/*140*/))

#include "gsv2k6_device.h"

/* supported uapi */
#define Gsv2k6_AvUapiInitDevice
#define Gsv2k6_AvUapiEnablePort
#define Gsv2k6_AvUapiResetPort
#define Gsv2k6_AvUapiRxPortInit
#define Gsv2k6_AvUapiRxEnableFreeRun
#define Gsv2k6_AvUapiRxGetStatus
#define Gsv2k6_AvUapiRxGet5VStatus
#define Gsv2k6_AvUapiRxSetHpdDown
#define Gsv2k6_AvUapiRxSetHpdUp
#define Gsv2k6_AvUapiRxGetVideoPacketStatus
#define Gsv2k6_AvUapiRxGetAudioPacketStatus
#define Gsv2k6_AvUapiRxGetHdcpStatus
#define Gsv2k6_AvUapiRxClearFlags
#define Gsv2k6_AvUapiTxEnableCore
#define Gsv2k6_AvUapiTxDisableCore
#define Gsv2k6_AvUapiTxPortInit
#define Gsv2k6_AvUapiTxGetStatus
#define Gsv2k6_AvUapiTxSendAksv
#define Gsv2k6_AvUapiTxVideoManage
#define Gsv2k6_AvUapiTxAudioManage
#define Gsv2k6_AvUapiRxVideoManage
#define Gsv2k6_AvUapiRxAudioManage
#define Gsv2k6_AvUapiTxSetAudioPackets
#define Gsv2k6_AvUapiRxGetPacketContent
#define Gsv2k6_AvUapiTxSetPacketContent
#define Gsv2k6_AvUapiTxEnableInfoFrames
#define Gsv2k6_AvUapiRxGetHdmiAcrInfo
#define Gsv2k6_AvUapiConnectPort
#define Gsv2k6_AvUapiDisconnectPort
#define Gsv2k6_AvUapiRxClearAudioInterrupt
#define Gsv2k6_AvUapiRxClearVideoInterrupt
#define Gsv2k6_AvUapiRxClearHdcpInterrupt
#define Gsv2k6_AvUapiTxMuteTmds
#define Gsv2k6_AvUapiRxGetAvMute
#define Gsv2k6_AvUapiTxSetAvMute
#define Gsv2k6_AvUapiRxGetHdmiModeSupport
#define Gsv2k6_AvUapiTxSetHdmiModeSupport
#define Gsv2k6_AvUapiTxSetFeatureSupport
#define Gsv2k6_AvUapiRxGetVideoLock
#define Gsv2k6_AvUapiRxGetVideoTiming
#define Gsv2k6_AvUapiTxSetVideoTiming
#define Gsv2k6_AvUapiRxGetHdmiDeepColor
#define Gsv2k6_AvUapiTxSetHdmiDeepColor
#define Gsv2k6_AvUapiRxGetAudioInternalMute
#define Gsv2k6_AvUapiRxSetAudioInternalMute
#define Gsv2k6_AvUapiTxSetAudNValue
#define Gsv2k6_AvUapiRxGetPacketType
#define Gsv2k6_AvUapiTxSetAviInfoFrame
#define Gsv2k6_AvUapiTxReadBksv
#define Gsv2k6_AvUapiRxAddBksv
#define Gsv2k6_AvUapiTxGetBksvTotal
#define Gsv2k6_AvUapiTxSetBlackMute
#define Gsv2k6_AvUapiTxEncryptSink
#define Gsv2k6_AvUapiTxDecryptSink
#define Gsv2k6_AvUapiTxGetHdcpStatus
#define Gsv2k6_AvUapiTxGetBksvReady
#define Gsv2k6_AvUapiRxSetHdcpEnable
#define Gsv2k6_AvUapiRxSetBksvListReady
#define Gsv2k6_AvUapiRxSetHdcpMode
#define Gsv2k6_AvUapiHdcp2p2Mode
#define Gsv2k6_AvUapiTxClearBksvReady
#define Gsv2k6_AvUapiTxClearRxidReady
#define Gsv2k6_AvUapiTxClearHdcpError
#define Gsv2k6_AvUapiRxCheckBksvExisted
#define Gsv2k6_AvUapiTxGetSinkHdcpCapability
#define Gsv2k6_AvUapiTxGetSinkHdcpCapability
#define Gsv2k6_AvUapiCecSendMessage
#define Gsv2k6_AvUapiGetNackCount
#define Gsv2k6_AvUapiTxCecInit
#define Gsv2k6_AvUapiCecRxGetStatus
#define Gsv2k6_AvUapiCecTxGetStatus
#define Gsv2k6_AvUapiTxCecSetPhysicalAddr
#define Gsv2k6_AvUapiTxCecSetLogicalAddr
#define Gsv2k6_AvUapiRxReadEdid
#define Gsv2k6_AvUapiRxWriteEdid
#define Gsv2k6_AvUapiRxSetSpa
#define Gsv2k6_AvUapiTxReadEdid
#define Gsv2k6_AvUapiRxEnableInternalEdid
#define Gsv2k6_AvUapiTxArcEnable
#define Gsv2k6_AvUapiRxHdcp2p2Manage
#define Gsv2k6_AvUapiTxHdcp2p2Manage
#define Gsv2k6_AvUapiCheckLogicAudioTx
#define Gsv2k6_AvUapiCheckLogicAudioRx
#define Gsv2k6_AvUapiCheckVideoScaler
#define Gsv2k6_AvUapiCheckVideoColor
#define Gsv2k6_AvUapiCheckVideoDsc
#define Gsv2k6_AvUapiCheckVideoGen

#if AvEnableCcaFeature
#define Gsv2k6_AvUapiCcaTxGetInfo
#define Gsv2k6_AvUapiCcaRxGetInfo
#define Gsv2k6_AvUapiCcaTxInit
#define Gsv2k6_AvUapiCcaRxInit
#define Gsv2k6_AvUapiCcaTxChainReset
#define Gsv2k6_AvUapiCcaRxChainReset
#define Gsv2k6_AvUapiCcaTxCalibManage
#define Gsv2k6_AvUapiCcaRxCalibManage
#define Gsv2k6_AvUapiCcaTxSetWorkMode
#define Gsv2k6_AvUapiCcaTxEnablePin
#define Gsv2k6_AvUapiCcaRxEnablePin

#endif

#ifdef Gsv2k6_AvUapiTxSetAudioPackets

void Gsv2k6_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv2k6_TxSetAudioInterface (AvPort* port);
void Gsv2k6_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv2k6_TxSetAudChStatSampFreq (AvPort* port);
void Gsv2k6_TxSetAudMCLK (AvPort* port);
AvRet Gsv2k6_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv2k6_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv2k6_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv2k6_TxSetCSC (AvPort *port);
uint8 Gsv2k6_RxGetPixRepValue (AvPort *port);

#endif


#endif
