#ifndef __gsv2k7_h
#define __gsv2k7_h

#include "av_config.h"

#define Gsv2k7ResourceSize  (804+(AvEnableDetailTiming*72)+(AvEnableInternalVideoGen*40)+(AvEnableCecFeature*140))

#include "gsv2k7_device.h"

/* supported uapi */
#define Gsv2k7_AvUapiInitDevice
#define Gsv2k7_AvUapiEnablePort
#define Gsv2k7_AvUapiResetPort
#define Gsv2k7_AvUapiRxPortInit
#define Gsv2k7_AvUapiRxEnableFreeRun
#define Gsv2k7_AvUapiRxGetStatus
#define Gsv2k7_AvUapiRxGet5VStatus
#define Gsv2k7_AvUapiRxSetHpdDown
#define Gsv2k7_AvUapiRxSetHpdUp
#define Gsv2k7_AvUapiRxGetVideoPacketStatus
#define Gsv2k7_AvUapiRxGetAudioPacketStatus
#define Gsv2k7_AvUapiRxGetHdcpStatus
#define Gsv2k7_AvUapiRxClearFlags
#define Gsv2k7_AvUapiTxEnableCore
#define Gsv2k7_AvUapiTxDisableCore
#define Gsv2k7_AvUapiTxPortInit
#define Gsv2k7_AvUapiTxGetStatus
#define Gsv2k7_AvUapiTxSendAksv
#define Gsv2k7_AvUapiTxVideoManage
#define Gsv2k7_AvUapiTxAudioManage
#define Gsv2k7_AvUapiRxVideoManage
#define Gsv2k7_AvUapiRxAudioManage
#define Gsv2k7_AvUapiTxSetAudioPackets
#define Gsv2k7_AvUapiRxGetPacketContent
#define Gsv2k7_AvUapiTxSetPacketContent
#define Gsv2k7_AvUapiTxEnableInfoFrames
#define Gsv2k7_AvUapiRxGetHdmiAcrInfo
#define Gsv2k7_AvUapiConnectPort
#define Gsv2k7_AvUapiDisconnectPort
#define Gsv2k7_AvUapiRxClearAudioInterrupt
#define Gsv2k7_AvUapiRxClearVideoInterrupt
#define Gsv2k7_AvUapiRxClearHdcpInterrupt
#define Gsv2k7_AvUapiTxMuteTmds
#define Gsv2k7_AvUapiRxGetAvMute
#define Gsv2k7_AvUapiTxSetAvMute
#define Gsv2k7_AvUapiRxGetHdmiModeSupport
#define Gsv2k7_AvUapiTxSetHdmiModeSupport
#define Gsv2k7_AvUapiTxSetFeatureSupport
#define Gsv2k7_AvUapiRxGetVideoLock
#define Gsv2k7_AvUapiRxGetVideoTiming
#define Gsv2k7_AvUapiTxSetVideoTiming
#define Gsv2k7_AvUapiRxGetHdmiDeepColor
#define Gsv2k7_AvUapiTxSetHdmiDeepColor
#define Gsv2k7_AvUapiRxGetAudioInternalMute
#define Gsv2k7_AvUapiRxSetAudioInternalMute
#define Gsv2k7_AvUapiTxSetAudNValue
#define Gsv2k7_AvUapiRxGetPacketType
#define Gsv2k7_AvUapiTxSetAviInfoFrame
#define Gsv2k7_AvUapiTxReadBksv
#define Gsv2k7_AvUapiRxAddBksv
#define Gsv2k7_AvUapiTxGetBksvTotal
#define Gsv2k7_AvUapiTxSetBlackMute
#define Gsv2k7_AvUapiTxEncryptSink
#define Gsv2k7_AvUapiTxDecryptSink
#define Gsv2k7_AvUapiTxGetHdcpStatus
#define Gsv2k7_AvUapiTxGetBksvReady
#define Gsv2k7_AvUapiRxSetHdcpEnable
#define Gsv2k7_AvUapiRxSetBksvListReady
#define Gsv2k7_AvUapiRxSetHdcpMode
#define Gsv2k7_AvUapiHdcp2p2Mode
#define Gsv2k7_AvUapiTxClearBksvReady
#define Gsv2k7_AvUapiTxClearRxidReady
#define Gsv2k7_AvUapiTxClearHdcpError
#define Gsv2k7_AvUapiRxCheckBksvExisted
#define Gsv2k7_AvUapiTxGetSinkHdcpCapability
#define Gsv2k7_AvUapiTxGetSinkHdcpCapability
#define Gsv2k7_AvUapiCecSendMessage
#define Gsv2k7_AvUapiGetNackCount
#define Gsv2k7_AvUapiTxCecInit
#define Gsv2k7_AvUapiCecRxGetStatus
#define Gsv2k7_AvUapiCecTxGetStatus
#define Gsv2k7_AvUapiTxCecSetPhysicalAddr
#define Gsv2k7_AvUapiTxCecSetLogicalAddr
#define Gsv2k7_AvUapiRxReadEdid
#define Gsv2k7_AvUapiRxWriteEdid
#define Gsv2k7_AvUapiRxSetSpa
#define Gsv2k7_AvUapiTxReadEdid
#define Gsv2k7_AvUapiRxEnableInternalEdid
#define Gsv2k7_AvUapiTxArcEnable
#define Gsv2k7_AvUapiRxHdcp2p2Manage
#define Gsv2k7_AvUapiTxHdcp2p2Manage
#define Gsv2k7_AvUapiCheckLogicAudioTx
#define Gsv2k7_AvUapiCheckLogicAudioRx
#define Gsv2k7_AvUapiCheckVideoScaler
#define Gsv2k7_AvUapiCheckVideoColor
#define Gsv2k7_AvUapiCheckVideoGen

#ifdef Gsv2k7_AvUapiTxSetAudioPackets

void Gsv2k7_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv2k7_TxSetAudioInterface (AvPort* port);
void Gsv2k7_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv2k7_TxSetAudChStatSampFreq (AvPort* port);
void Gsv2k7_TxSetAudMCLK (AvPort* port);
AvRet Gsv2k7_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv2k7_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv2k7_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv2k7_TxSetCSC (AvPort *port);
uint8 Gsv2k7_RxGetPixRepValue (AvPort *port);

#endif

enum {
    SWITCH_RX12_TO_TXAB = 0,
    SWITCH_RX12_TO_TXBA = 1,
    SPLITTER_RX1_TO_TXAB = 2,
    SPLITTER_RX2_TO_TXAB = 3,
};

#endif
