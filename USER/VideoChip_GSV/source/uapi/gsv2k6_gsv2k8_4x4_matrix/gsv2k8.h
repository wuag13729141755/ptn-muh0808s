#ifndef __gsv2k8_h
#define __gsv2k8_h

#include "av_config.h"

#define Gsv2k8ResourceSize  (804+(AvEnableDetailTiming*72)+(AvEnableInternalVideoGen*40)+(AvEnableCecFeature*140))

#include "gsv2k8_device.h"

/* supported uapi */
#define Gsv2k8_AvUapiInitDevice
#define Gsv2k8_AvUapiEnablePort
#define Gsv2k8_AvUapiResetPort
#define Gsv2k8_AvUapiRxPortInit
#define Gsv2k8_AvUapiRxEnableFreeRun
#define Gsv2k8_AvUapiRxGetStatus
#define Gsv2k8_AvUapiRxGet5VStatus
#define Gsv2k8_AvUapiRxSetHpdDown
#define Gsv2k8_AvUapiRxSetHpdUp
#define Gsv2k8_AvUapiRxGetVideoPacketStatus
#define Gsv2k8_AvUapiRxGetAudioPacketStatus
#define Gsv2k8_AvUapiRxGetHdcpStatus
#define Gsv2k8_AvUapiRxClearFlags
#define Gsv2k8_AvUapiTxEnableCore
#define Gsv2k8_AvUapiTxDisableCore
#define Gsv2k8_AvUapiTxPortInit
#define Gsv2k8_AvUapiTxGetStatus
#define Gsv2k8_AvUapiTxSendAksv
#define Gsv2k8_AvUapiTxVideoManage
#define Gsv2k8_AvUapiTxAudioManage
#define Gsv2k8_AvUapiRxVideoManage
#define Gsv2k8_AvUapiRxAudioManage
#define Gsv2k8_AvUapiTxSetAudioPackets
#define Gsv2k8_AvUapiRxGetPacketContent
#define Gsv2k8_AvUapiTxSetPacketContent
#define Gsv2k8_AvUapiTxEnableInfoFrames
#define Gsv2k8_AvUapiRxGetHdmiAcrInfo
#define Gsv2k8_AvUapiConnectPort
#define Gsv2k8_AvUapiDisconnectPort
#define Gsv2k8_AvUapiRxClearAudioInterrupt
#define Gsv2k8_AvUapiRxClearVideoInterrupt
#define Gsv2k8_AvUapiRxClearHdcpInterrupt
#define Gsv2k8_AvUapiTxMuteTmds
#define Gsv2k8_AvUapiRxGetAvMute
#define Gsv2k8_AvUapiTxSetAvMute
#define Gsv2k8_AvUapiRxGetHdmiModeSupport
#define Gsv2k8_AvUapiTxSetHdmiModeSupport
#define Gsv2k8_AvUapiTxSetFeatureSupport
#define Gsv2k8_AvUapiRxGetVideoLock
#define Gsv2k8_AvUapiRxGetVideoTiming
#define Gsv2k8_AvUapiTxSetVideoTiming
#define Gsv2k8_AvUapiRxGetHdmiDeepColor
#define Gsv2k8_AvUapiTxSetHdmiDeepColor
#define Gsv2k8_AvUapiRxGetAudioInternalMute
#define Gsv2k8_AvUapiRxSetAudioInternalMute
#define Gsv2k8_AvUapiTxSetAudNValue
#define Gsv2k8_AvUapiRxGetPacketType
#define Gsv2k8_AvUapiTxSetAviInfoFrame
#define Gsv2k8_AvUapiTxReadBksv
#define Gsv2k8_AvUapiRxAddBksv
#define Gsv2k8_AvUapiTxGetBksvTotal
#define Gsv2k8_AvUapiTxSetBlackMute
#define Gsv2k8_AvUapiTxEncryptSink
#define Gsv2k8_AvUapiTxDecryptSink
#define Gsv2k8_AvUapiTxGetHdcpStatus
#define Gsv2k8_AvUapiTxGetBksvReady
#define Gsv2k8_AvUapiRxSetHdcpEnable
#define Gsv2k8_AvUapiRxSetBksvListReady
#define Gsv2k8_AvUapiRxSetHdcpMode
#define Gsv2k8_AvUapiHdcp2p2Mode
#define Gsv2k8_AvUapiTxClearBksvReady
#define Gsv2k8_AvUapiTxClearRxidReady
#define Gsv2k8_AvUapiTxClearHdcpError
#define Gsv2k8_AvUapiRxCheckBksvExisted
#define Gsv2k8_AvUapiTxGetSinkHdcpCapability
#define Gsv2k8_AvUapiTxGetSinkHdcpCapability
#define Gsv2k8_AvUapiCecSendMessage
#define Gsv2k8_AvUapiGetNackCount
#define Gsv2k8_AvUapiTxCecInit
#define Gsv2k8_AvUapiCecRxGetStatus
#define Gsv2k8_AvUapiCecTxGetStatus
#define Gsv2k8_AvUapiTxCecSetPhysicalAddr
#define Gsv2k8_AvUapiTxCecSetLogicalAddr
#define Gsv2k8_AvUapiRxReadEdid
#define Gsv2k8_AvUapiRxWriteEdid
#define Gsv2k8_AvUapiRxSetSpa
#define Gsv2k8_AvUapiTxReadEdid
#define Gsv2k8_AvUapiRxEnableInternalEdid
#define Gsv2k8_AvUapiTxArcEnable
#define Gsv2k8_AvUapiRxHdcp2p2Manage
#define Gsv2k8_AvUapiTxHdcp2p2Manage
#define Gsv2k8_AvUapiCheckLogicAudioTx
#define Gsv2k8_AvUapiCheckLogicAudioRx
#define Gsv2k8_AvUapiCheckVideoScaler
#define Gsv2k8_AvUapiCheckVideoColor
#define Gsv2k8_AvUapiCheckVideoGen

#ifdef Gsv2k8_AvUapiTxSetAudioPackets

void Gsv2k8_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv2k8_TxSetAudioInterface (AvPort* port);
void Gsv2k8_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv2k8_TxSetAudChStatSampFreq (AvPort* port);
void Gsv2k8_TxSetAudMCLK (AvPort* port);
AvRet Gsv2k8_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv2k8_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv2k8_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv2k8_TxSetCSC (AvPort *port);
uint8 Gsv2k8_RxGetPixRepValue (AvPort *port);

#endif
uint8 Gsv2k8GetRxClockLockStatus(pin AvPort *port);

enum {
    SWITCH_RX12_TO_TXAB = 0,
    SWITCH_RX12_TO_TXBA = 1,
    SPLITTER_RX1_TO_TXAB = 2,
    SPLITTER_RX2_TO_TXAB = 3,
};

#endif
