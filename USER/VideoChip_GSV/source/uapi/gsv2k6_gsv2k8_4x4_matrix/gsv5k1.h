#ifndef __gsv5k1_h
#define __gsv5k1_h

#include "av_config.h"

#define Gsv5k1ResourceSize (600+(AvEnableDetailTiming*160)+(AvEnableInternalVideoGen*40)+(AvEnableInternalAudioGen*24)+(AvEnableInternalClockGen*16)+(AvEnableCecFeature*140))

#include "gsv5k1_device.h"

/* supported uapi */
#define Gsv5k1_AvUapiInitDevice
#define Gsv5k1_AvUapiEnablePort
#define Gsv5k1_AvUapiResetPort
#define Gsv5k1_AvUapiRxPortInit
#define Gsv5k1_AvUapiRxEnableFreeRun
#define Gsv5k1_AvUapiRxGetStatus
#define Gsv5k1_AvUapiRxGet5VStatus
#define Gsv5k1_AvUapiRxSetHpdDown
#define Gsv5k1_AvUapiRxSetHpdUp
#define Gsv5k1_AvUapiRxGetVideoPacketStatus
#define Gsv5k1_AvUapiRxGetAudioPacketStatus
#define Gsv5k1_AvUapiRxGetHdcpStatus
#define Gsv5k1_AvUapiRxClearFlags
#define Gsv5k1_AvUapiTxEnableCore
#define Gsv5k1_AvUapiTxDisableCore
#define Gsv5k1_AvUapiTxPortInit
#define Gsv5k1_AvUapiTxGetStatus
#define Gsv5k1_AvUapiTxSendAksv
#define Gsv5k1_AvUapiTxVideoManage
#define Gsv5k1_AvUapiTxAudioManage
#define Gsv5k1_AvUapiRxVideoManage
#define Gsv5k1_AvUapiRxAudioManage
#define Gsv5k1_AvUapiTxSetAudioPackets
#define Gsv5k1_AvUapiRxGetPacketContent
#define Gsv5k1_AvUapiTxSetPacketContent
#define Gsv5k1_AvUapiTxEnableInfoFrames
#define Gsv5k1_AvUapiRxGetHdmiAcrInfo
#define Gsv5k1_AvUapiConnectPort
#define Gsv5k1_AvUapiDisconnectPort
#define Gsv5k1_AvUapiRxClearAudioInterrupt
#define Gsv5k1_AvUapiRxClearVideoInterrupt
#define Gsv5k1_AvUapiRxClearHdcpInterrupt
#define Gsv5k1_AvUapiTxMuteTmds
#define Gsv5k1_AvUapiRxGetAvMute
#define Gsv5k1_AvUapiTxSetAvMute
#define Gsv5k1_AvUapiRxGetHdmiModeSupport
#define Gsv5k1_AvUapiTxSetHdmiModeSupport
#define Gsv5k1_AvUapiTxSetFeatureSupport
#define Gsv5k1_AvUapiRxGetVideoLock
#define Gsv5k1_AvUapiRxGetVideoTiming
#define Gsv5k1_AvUapiTxSetVideoTiming
#define Gsv5k1_AvUapiRxGetHdmiDeepColor
#define Gsv5k1_AvUapiTxSetHdmiDeepColor
#define Gsv5k1_AvUapiRxGetAudioInternalMute
#define Gsv5k1_AvUapiRxSetAudioInternalMute
#define Gsv5k1_AvUapiTxSetAudNValue
#define Gsv5k1_AvUapiRxGetPacketType
#define Gsv5k1_AvUapiTxSetAviInfoFrame
#define Gsv5k1_AvUapiTxReadBksv
#define Gsv5k1_AvUapiRxAddBksv
#define Gsv5k1_AvUapiTxGetBksvTotal
#define Gsv5k1_AvUapiTxSetBlackMute
#define Gsv5k1_AvUapiTxEncryptSink
#define Gsv5k1_AvUapiTxDecryptSink
#define Gsv5k1_AvUapiTxGetHdcpStatus
#define Gsv5k1_AvUapiTxGetBksvReady
#define Gsv5k1_AvUapiRxSetHdcpEnable
#define Gsv5k1_AvUapiRxSetBksvListReady
#define Gsv5k1_AvUapiRxSetHdcpMode
#define Gsv5k1_AvUapiHdcp2p2Mode
#define Gsv5k1_AvUapiTxClearBksvReady
#define Gsv5k1_AvUapiTxClearRxidReady
#define Gsv5k1_AvUapiTxClearHdcpError
#define Gsv5k1_AvUapiRxCheckBksvExisted
#define Gsv5k1_AvUapiTxGetSinkHdcpCapability
#define Gsv5k1_AvUapiTxGetSinkHdcpCapability
#define Gsv5k1_AvUapiCecSendMessage
#define Gsv5k1_AvUapiGetNackCount
#define Gsv5k1_AvUapiTxCecInit
#define Gsv5k1_AvUapiCecRxGetStatus
#define Gsv5k1_AvUapiCecTxGetStatus
#define Gsv5k1_AvUapiTxCecSetPhysicalAddr
#define Gsv5k1_AvUapiTxCecSetLogicalAddr
#define Gsv5k1_AvUapiRxReadEdid
#define Gsv5k1_AvUapiRxWriteEdid
#define Gsv5k1_AvUapiRxSetSpa
#define Gsv5k1_AvUapiTxReadEdid
#define Gsv5k1_AvUapiRxEnableInternalEdid
#define Gsv5k1_AvUapiTxArcEnable
#define Gsv5k1_AvUapiRxHdcp2p2Manage
#define Gsv5k1_AvUapiTxHdcp2p2Manage
#define Gsv5k1_AvUapiCheckLogicAudioTx
#define Gsv5k1_AvUapiCheckLogicAudioRx
#define Gsv5k1_AvUapiCheckVideoScaler
#define Gsv5k1_AvUapiCheckVideoColor
#define Gsv5k1_AvUapiCheckVideoGen
#define Gsv5k1_AvUapiCheckVideoDsc

#if AvEnableCcaFeature
#define Gsv5k1_AvUapiCcaTxGetInfo
#define Gsv5k1_AvUapiCcaRxGetInfo
#define Gsv5k1_AvUapiCcaTxInit
#define Gsv5k1_AvUapiCcaRxInit
#define Gsv5k1_AvUapiCcaTxChainReset
#define Gsv5k1_AvUapiCcaRxChainReset
#define Gsv5k1_AvUapiCcaTxCalibManage
#define Gsv5k1_AvUapiCcaRxCalibManage
#define Gsv5k1_AvUapiCcaTxSetWorkMode
#define Gsv5k1_AvUapiCcaTxEnablePin
#define Gsv5k1_AvUapiCcaRxEnablePin
#endif
#define Gsv5k1_AvUapiCheckAudioGen
#define Gsv5k1_AvUapiCheckClockGen

#ifdef Gsv5k1_AvUapiTxSetAudioPackets

void Gsv5k1_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv5k1_TxSetAudioInterface (AvPort* port);
void Gsv5k1_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv5k1_TxSetAudChStatSampFreq (AvPort* port);
void Gsv5k1_TxSetAudMCLK (AvPort* port);
AvRet Gsv5k1_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv5k1_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv5k1_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv5k1_TxSetCSC (AvPort *port);
uint8 Gsv5k1_RxGetPixRepValue (AvPort *port);

#endif


#endif
