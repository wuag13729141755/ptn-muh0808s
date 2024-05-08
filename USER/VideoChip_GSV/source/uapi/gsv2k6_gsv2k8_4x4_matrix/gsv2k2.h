#ifndef __gsv2k2_h
#define __gsv2k2_h

#include "av_config.h"

#define Gsv2k2ResourceSize  (600+(AvEnableDetailTiming*72)+(AvEnableInternalVideoGen*40)+(AvEnableInternalAudioGen*24)+(AvEnableInternalClockGen*16)+(AvEnableCecFeature*140))

#include "gsv2k2_device.h"

/* supported uapi */
#define Gsv2k2_AvUapiInitDevice
#define Gsv2k2_AvUapiEnablePort
#define Gsv2k2_AvUapiResetPort
#define Gsv2k2_AvUapiRxPortInit
#define Gsv2k2_AvUapiRxEnableFreeRun
#define Gsv2k2_AvUapiRxGetStatus
#define Gsv2k2_AvUapiRxGet5VStatus
#define Gsv2k2_AvUapiRxSetHpdDown
#define Gsv2k2_AvUapiRxSetHpdUp
#define Gsv2k2_AvUapiRxGetVideoPacketStatus
#define Gsv2k2_AvUapiRxGetAudioPacketStatus
#define Gsv2k2_AvUapiRxGetHdcpStatus
#define Gsv2k2_AvUapiRxClearFlags
#define Gsv2k2_AvUapiTxEnableCore
#define Gsv2k2_AvUapiTxDisableCore
#define Gsv2k2_AvUapiTxPortInit
#define Gsv2k2_AvUapiTxGetStatus
#define Gsv2k2_AvUapiTxSendAksv
#define Gsv2k2_AvUapiTxVideoManage
#define Gsv2k2_AvUapiTxAudioManage
#define Gsv2k2_AvUapiRxVideoManage
#define Gsv2k2_AvUapiRxAudioManage
#define Gsv2k2_AvUapiTxSetAudioPackets
#define Gsv2k2_AvUapiRxGetPacketContent
#define Gsv2k2_AvUapiTxSetPacketContent
#define Gsv2k2_AvUapiTxEnableInfoFrames
#define Gsv2k2_AvUapiRxGetHdmiAcrInfo
#define Gsv2k2_AvUapiConnectPort
#define Gsv2k2_AvUapiDisconnectPort
#define Gsv2k2_AvUapiRxClearAudioInterrupt
#define Gsv2k2_AvUapiRxClearVideoInterrupt
#define Gsv2k2_AvUapiRxClearHdcpInterrupt
#define Gsv2k2_AvUapiTxMuteTmds
#define Gsv2k2_AvUapiRxGetAvMute
#define Gsv2k2_AvUapiTxSetAvMute
#define Gsv2k2_AvUapiRxGetHdmiModeSupport
#define Gsv2k2_AvUapiTxSetHdmiModeSupport
#define Gsv2k2_AvUapiTxSetFeatureSupport
#define Gsv2k2_AvUapiRxGetVideoLock
#define Gsv2k2_AvUapiRxGetVideoTiming
#define Gsv2k2_AvUapiTxSetVideoTiming
#define Gsv2k2_AvUapiRxGetHdmiDeepColor
#define Gsv2k2_AvUapiTxSetHdmiDeepColor
#define Gsv2k2_AvUapiRxGetAudioInternalMute
#define Gsv2k2_AvUapiRxSetAudioInternalMute
#define Gsv2k2_AvUapiTxSetAudNValue
#define Gsv2k2_AvUapiRxGetPacketType
#define Gsv2k2_AvUapiTxSetAviInfoFrame
#define Gsv2k2_AvUapiTxReadBksv
#define Gsv2k2_AvUapiRxAddBksv
#define Gsv2k2_AvUapiTxGetBksvTotal
#define Gsv2k2_AvUapiTxSetBlackMute
#define Gsv2k2_AvUapiTxEncryptSink
#define Gsv2k2_AvUapiTxDecryptSink
#define Gsv2k2_AvUapiTxGetHdcpStatus
#define Gsv2k2_AvUapiTxGetBksvReady
#define Gsv2k2_AvUapiRxSetHdcpEnable
#define Gsv2k2_AvUapiRxSetBksvListReady
#define Gsv2k2_AvUapiRxSetHdcpMode
#define Gsv2k2_AvUapiHdcp2p2Mode
#define Gsv2k2_AvUapiTxClearBksvReady
#define Gsv2k2_AvUapiTxClearRxidReady
#define Gsv2k2_AvUapiTxClearHdcpError
#define Gsv2k2_AvUapiRxCheckBksvExisted
#define Gsv2k2_AvUapiTxGetSinkHdcpCapability
#define Gsv2k2_AvUapiTxGetSinkHdcpCapability
#define Gsv2k2_AvUapiCecSendMessage
#define Gsv2k2_AvUapiGetNackCount
#define Gsv2k2_AvUapiTxCecInit
#define Gsv2k2_AvUapiCecRxGetStatus
#define Gsv2k2_AvUapiCecTxGetStatus
#define Gsv2k2_AvUapiTxCecSetPhysicalAddr
#define Gsv2k2_AvUapiTxCecSetLogicalAddr
#define Gsv2k2_AvUapiRxReadEdid
#define Gsv2k2_AvUapiRxWriteEdid
#define Gsv2k2_AvUapiRxSetSpa
#define Gsv2k2_AvUapiTxReadEdid
#define Gsv2k2_AvUapiRxEnableInternalEdid
#define Gsv2k2_AvUapiTxArcEnable
#define Gsv2k2_AvUapiRxHdcp2p2Manage
#define Gsv2k2_AvUapiTxHdcp2p2Manage
#define Gsv2k2_AvUapiCheckLogicAudioTx
#define Gsv2k2_AvUapiCheckLogicAudioRx
#define Gsv2k2_AvUapiCheckVideoScaler
#define Gsv2k2_AvUapiCheckVideoColor
#define Gsv2k2_AvUapiCheckVideoGen

#if AvEnableCcaFeature
#define Gsv2k2_AvUapiCcaTxGetInfo
#define Gsv2k2_AvUapiCcaRxGetInfo
#define Gsv2k2_AvUapiCcaTxInit
#define Gsv2k2_AvUapiCcaRxInit
#define Gsv2k2_AvUapiCcaTxChainReset
#define Gsv2k2_AvUapiCcaRxChainReset
#define Gsv2k2_AvUapiCcaTxCalibManage
#define Gsv2k2_AvUapiCcaRxCalibManage
#define Gsv2k2_AvUapiCcaTxSetWorkMode
#define Gsv2k2_AvUapiCcaTxEnablePin
#define Gsv2k2_AvUapiCcaRxEnablePin

#endif

#ifdef Gsv2k2_AvUapiTxSetAudioPackets

void Gsv2k2_TxEnableInfoFrames (AvPort* port, uint16 InfoFrames, bool Enable);
void Gsv2k2_TxSetAudioInterface (AvPort* port);
void Gsv2k2_TxWriteAudIfPacket (AvPort* port, uint8 *Packet);
void Gsv2k2_TxSetAudChStatSampFreq (AvPort* port);
void Gsv2k2_TxSetAudMCLK (AvPort* port);
AvRet Gsv2k2_TxAudInputEnable (AvPort *port, TxAudioInterface Interface, bool Enable);
AvRet Gsv2k2_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn);
AvRet Gsv2k2_TxSetManualPixelRepeat(AvPort *port);
AvRet Gsv2k2_TxSetCSC (AvPort *port);
uint8 Gsv2k2_RxGetPixRepValue (AvPort *port);

#endif
uint8 Gsv2k2GetRxClockLockStatus(pin AvPort *port);


#endif
