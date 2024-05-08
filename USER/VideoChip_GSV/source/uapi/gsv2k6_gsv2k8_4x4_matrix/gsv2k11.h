#ifndef __gsv2k11_h
#define __gsv2k11_h

#include "av_config.h"

#define Gsv2k11ResourceSize  (560+(AvEnableDetailTiming*48)+(AvEnableInternalVideoGen*40)+(AvEnableInternalAudioGen*24)+(AvEnableInternalClockGen*24)+(AvEnableCecFeature*72))

#include "gsv2k11_device.h"

/* supported uapi */
#define Gsv2k11_AvUapiInitDevice
#define Gsv2k11_AvUapiEnablePort
#define Gsv2k11_AvUapiResetPort
#define Gsv2k11_AvUapiRxPortInit
#define Gsv2k11_AvUapiRxEnableFreeRun
#define Gsv2k11_AvUapiRxGetStatus
#define Gsv2k11_AvUapiRxGet5VStatus
#define Gsv2k11_AvUapiRxSetHpdDown
#define Gsv2k11_AvUapiRxSetHpdUp
#define Gsv2k11_AvUapiRxGetVideoPacketStatus
#define Gsv2k11_AvUapiRxGetAudioPacketStatus
#define Gsv2k11_AvUapiRxGetHdcpStatus
#define Gsv2k11_AvUapiRxClearFlags
#define Gsv2k11_AvUapiTxEnableCore
#define Gsv2k11_AvUapiTxDisableCore
#define Gsv2k11_AvUapiTxPortInit
#define Gsv2k11_AvUapiTxGetStatus
#define Gsv2k11_AvUapiTxSendAksv
#define Gsv2k11_AvUapiTxVideoManage
#define Gsv2k11_AvUapiTxAudioManage
#define Gsv2k11_AvUapiRxVideoManage
#define Gsv2k11_AvUapiRxAudioManage
#define Gsv2k11_AvUapiTxSetAudioPackets
#define Gsv2k11_AvUapiRxGetPacketContent
#define Gsv2k11_AvUapiTxSetPacketContent
#define Gsv2k11_AvUapiTxEnableInfoFrames
#define Gsv2k11_AvUapiRxGetHdmiAcrInfo
#define Gsv2k11_AvUapiConnectPort
#define Gsv2k11_AvUapiDisconnectPort
#define Gsv2k11_AvUapiRxClearAudioInterrupt
#define Gsv2k11_AvUapiRxClearVideoInterrupt
#define Gsv2k11_AvUapiRxClearHdcpInterrupt
#define Gsv2k11_AvUapiTxMuteTmds
#define Gsv2k11_AvUapiRxGetAvMute
#define Gsv2k11_AvUapiTxSetAvMute
#define Gsv2k11_AvUapiRxGetHdmiModeSupport
#define Gsv2k11_AvUapiTxSetHdmiModeSupport
#define Gsv2k11_AvUapiTxSetFeatureSupport
#define Gsv2k11_AvUapiRxGetVideoLock
#define Gsv2k11_AvUapiRxGetVideoTiming
#define Gsv2k11_AvUapiTxSetVideoTiming
#define Gsv2k11_AvUapiRxGetHdmiDeepColor
#define Gsv2k11_AvUapiTxSetHdmiDeepColor
#define Gsv2k11_AvUapiRxGetAudioInternalMute
#define Gsv2k11_AvUapiRxSetAudioInternalMute
#define Gsv2k11_AvUapiTxSetAudNValue
#define Gsv2k11_AvUapiRxGetPacketType
#define Gsv2k11_AvUapiTxSetAviInfoFrame
#define Gsv2k11_AvUapiTxReadBksv
#define Gsv2k11_AvUapiRxAddBksv
#define Gsv2k11_AvUapiTxGetBksvTotal
#define Gsv2k11_AvUapiTxSetBlackMute
#define Gsv2k11_AvUapiTxEncryptSink
#define Gsv2k11_AvUapiTxDecryptSink
#define Gsv2k11_AvUapiTxGetHdcpStatus
#define Gsv2k11_AvUapiTxGetBksvReady
#define Gsv2k11_AvUapiRxSetHdcpEnable
#define Gsv2k11_AvUapiRxSetBksvListReady
#define Gsv2k11_AvUapiRxSetHdcpMode
#define Gsv2k11_AvUapiHdcp2p2Mode
#define Gsv2k11_AvUapiTxClearBksvReady
#define Gsv2k11_AvUapiTxClearRxidReady
#define Gsv2k11_AvUapiTxClearHdcpError
#define Gsv2k11_AvUapiRxCheckBksvExisted
#define Gsv2k11_AvUapiTxGetSinkHdcpCapability
#define Gsv2k11_AvUapiTxGetSinkHdcpCapability
#define Gsv2k11_AvUapiCecSendMessage
#define Gsv2k11_AvUapiGetNackCount
#define Gsv2k11_AvUapiTxCecInit
#define Gsv2k11_AvUapiCecRxGetStatus
#define Gsv2k11_AvUapiCecTxGetStatus
#define Gsv2k11_AvUapiTxCecSetPhysicalAddr
#define Gsv2k11_AvUapiTxCecSetLogicalAddr
#define Gsv2k11_AvUapiRxReadEdid
#define Gsv2k11_AvUapiRxWriteEdid
#define Gsv2k11_AvUapiRxSetSpa
#define Gsv2k11_AvUapiTxReadEdid
#define Gsv2k11_AvUapiRxEnableInternalEdid
#define Gsv2k11_AvUapiTxArcEnable
#define Gsv2k11_AvUapiRxHdcp2p2Manage
#define Gsv2k11_AvUapiTxHdcp2p2Manage
#define Gsv2k11_AvUapiCheckLogicVideoTx
#define Gsv2k11_AvUapiCheckLogicVideoRx
#define Gsv2k11_AvUapiCheckLogicAudioTx
#define Gsv2k11_AvUapiCheckLogicAudioRx
#define Gsv2k11_AvUapiCheckVideoScaler
#define Gsv2k11_AvUapiCheckVideoColor
#define Gsv2k11_AvUapiCheckVideoGen
#define Gsv2k11_AvUapiCheckAudioGen
#define Gsv2k11_AvUapiCheckClockGen


#ifdef Gsv2k11_AvUapiTxSetAudioPackets

void Gsv2k11_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv2k11_TxSetAudioInterface (AvPort* port);
void Gsv2k11_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv2k11_TxSetAudChStatSampFreq (AvPort* port);
void Gsv2k11_TxSetAudMCLK (AvPort* port);
AvRet Gsv2k11_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv2k11_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv2k11_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv2k11_TxSetCSC (AvPort *port);
uint8 Gsv2k11_RxGetPixRepValue (AvPort *port);

#endif


#endif
