#ifndef __gsv2k1_h
#define __gsv2k1_h

#include "av_config.h"

#define Gsv2k1ResourceSize  (520+(AvEnableDetailTiming*48)+(AvEnableInternalVideoGen*40)+(AvEnableInternalAudioGen*24)+(AvEnableInternalClockGen*16)+(AvEnableCecFeature*140))

#include "gsv2k1_device.h"

/* supported uapi */
#define Gsv2k1_AvUapiInitDevice
#define Gsv2k1_AvUapiEnablePort
#define Gsv2k1_AvUapiResetPort
#define Gsv2k1_AvUapiRxPortInit
#define Gsv2k1_AvUapiRxEnableFreeRun
#define Gsv2k1_AvUapiRxGetStatus
#define Gsv2k1_AvUapiRxGet5VStatus
#define Gsv2k1_AvUapiRxSetHpdDown
#define Gsv2k1_AvUapiRxSetHpdUp
#define Gsv2k1_AvUapiRxGetVideoPacketStatus
#define Gsv2k1_AvUapiRxGetAudioPacketStatus
#define Gsv2k1_AvUapiRxGetHdcpStatus
#define Gsv2k1_AvUapiRxClearFlags
#define Gsv2k1_AvUapiTxEnableCore
#define Gsv2k1_AvUapiTxDisableCore
#define Gsv2k1_AvUapiTxPortInit
#define Gsv2k1_AvUapiTxGetStatus
#define Gsv2k1_AvUapiTxSendAksv
#define Gsv2k1_AvUapiTxVideoManage
#define Gsv2k1_AvUapiTxAudioManage
#define Gsv2k1_AvUapiRxVideoManage
#define Gsv2k1_AvUapiRxAudioManage
#define Gsv2k1_AvUapiTxSetAudioPackets
#define Gsv2k1_AvUapiRxGetPacketContent
#define Gsv2k1_AvUapiTxSetPacketContent
#define Gsv2k1_AvUapiTxEnableInfoFrames
#define Gsv2k1_AvUapiRxGetHdmiAcrInfo
#define Gsv2k1_AvUapiConnectPort
#define Gsv2k1_AvUapiDisconnectPort
#define Gsv2k1_AvUapiRxClearAudioInterrupt
#define Gsv2k1_AvUapiRxClearVideoInterrupt
#define Gsv2k1_AvUapiRxClearHdcpInterrupt
#define Gsv2k1_AvUapiTxMuteTmds
#define Gsv2k1_AvUapiRxGetAvMute
#define Gsv2k1_AvUapiTxSetAvMute
#define Gsv2k1_AvUapiRxGetHdmiModeSupport
#define Gsv2k1_AvUapiTxSetHdmiModeSupport
#define Gsv2k1_AvUapiTxSetFeatureSupport
#define Gsv2k1_AvUapiRxGetVideoLock
#define Gsv2k1_AvUapiRxGetVideoTiming
#define Gsv2k1_AvUapiTxSetVideoTiming
#define Gsv2k1_AvUapiRxGetHdmiDeepColor
#define Gsv2k1_AvUapiTxSetHdmiDeepColor
#define Gsv2k1_AvUapiRxGetAudioInternalMute
#define Gsv2k1_AvUapiRxSetAudioInternalMute
#define Gsv2k1_AvUapiTxSetAudNValue
#define Gsv2k1_AvUapiRxGetPacketType
#define Gsv2k1_AvUapiTxSetAviInfoFrame
#define Gsv2k1_AvUapiTxReadBksv
#define Gsv2k1_AvUapiRxAddBksv
#define Gsv2k1_AvUapiTxGetBksvTotal
#define Gsv2k1_AvUapiTxSetBlackMute
#define Gsv2k1_AvUapiTxEncryptSink
#define Gsv2k1_AvUapiTxDecryptSink
#define Gsv2k1_AvUapiTxGetHdcpStatus
#define Gsv2k1_AvUapiTxGetBksvReady
#define Gsv2k1_AvUapiRxSetHdcpEnable
#define Gsv2k1_AvUapiRxSetBksvListReady
#define Gsv2k1_AvUapiRxSetHdcpMode
#define Gsv2k1_AvUapiHdcp2p2Mode
#define Gsv2k1_AvUapiTxClearBksvReady
#define Gsv2k1_AvUapiTxClearRxidReady
#define Gsv2k1_AvUapiTxClearHdcpError
#define Gsv2k1_AvUapiRxCheckBksvExisted
#define Gsv2k1_AvUapiTxGetSinkHdcpCapability
#define Gsv2k1_AvUapiTxGetSinkHdcpCapability
#define Gsv2k1_AvUapiCecSendMessage
#define Gsv2k1_AvUapiGetNackCount
#define Gsv2k1_AvUapiTxCecInit
#define Gsv2k1_AvUapiCecRxGetStatus
#define Gsv2k1_AvUapiCecTxGetStatus
#define Gsv2k1_AvUapiTxCecSetPhysicalAddr
#define Gsv2k1_AvUapiTxCecSetLogicalAddr
#define Gsv2k1_AvUapiRxReadEdid
#define Gsv2k1_AvUapiRxWriteEdid
#define Gsv2k1_AvUapiRxSetSpa
#define Gsv2k1_AvUapiTxReadEdid
#define Gsv2k1_AvUapiRxEnableInternalEdid
#define Gsv2k1_AvUapiTxArcEnable
#define Gsv2k1_AvUapiRxHdcp2p2Manage
#define Gsv2k1_AvUapiTxHdcp2p2Manage
#define Gsv2k1_AvUapiCheckLogicAudioTx
#define Gsv2k1_AvUapiCheckLogicAudioRx
#define Gsv2k1_AvUapiCheckVideoScaler
#define Gsv2k1_AvUapiCheckVideoColor
#define Gsv2k1_AvUapiCheckVideoGen
#define Gsv2k1_AvUapiCheckAudioGen
#define Gsv2k1_AvUapiCheckClockGen


#ifdef Gsv2k1_AvUapiTxSetAudioPackets

void Gsv2k1_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv2k1_TxSetAudioInterface (AvPort* port);
void Gsv2k1_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv2k1_TxSetAudChStatSampFreq (AvPort* port);
void Gsv2k1_TxSetAudMCLK (AvPort* port);
AvRet Gsv2k1_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv2k1_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv2k1_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv2k1_TxSetCSC (AvPort *port);
uint8 Gsv2k1_RxGetPixRepValue (AvPort *port);

#endif


#endif
