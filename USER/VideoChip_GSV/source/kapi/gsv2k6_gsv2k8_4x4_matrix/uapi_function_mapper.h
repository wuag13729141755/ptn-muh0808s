/**
 * @file uapi_function_mapper.h
 *
 * @brief low level ports related universal api functions
 */


#ifndef _UAPI_FUNCTION_MAPPER_H
#define _UAPI_FUNCTION_MAPPER_H

#ifdef __cplusplus
    extern "C" {
#endif

typedef enum
{
    AvVideoPacketType,
    AvAudioPacketType
} AvPacketType;

typedef enum
{
    AvRxUpdate,
    AvRxForce,
    AvRxKeep
} AvRxUpdateType;

/* PART 1 */
/* Receiver FSM */
void KfunRxVarInit(pin AvPort *port);
void KfunRxInit(pin AvPort *port);
void KfunRxVideoManage(pin AvPort *port);
void KfunRxAudioManage(pin AvPort *port);
void KfunRxEnableFrun(pin AvPort *port);
void KfunRxDisableFrun(pin AvPort *port);
void KfunRxClearFlags(pin AvPort *port);
void KfunPollingRxStatus(pin AvPort *port);

/* PART 2 */
/* HDCP FSM */
void KfunHdcpVarInit(pin AvPort *port);
void KfunHdcpDetectSource(pin AvPort *port);
void KfunHdcpDetectSink(pin AvPort *port);
void KfunSimpleHdcpSync(pin AvPort *port);
void KfunReadBksv(pin AvPort *port);
AvPort *KfunLookupHdcpSource(pin AvPort *port);
void KfunUploadSinkInfo(pin AvPort *port);
void KfunCheckEncryption(pin AvPort *port);
void KfunEncryptSink(pin AvPort *port);
void KfunDecryptSink(pin AvPort *port);
void KfunDecryptNewSink(pin AvPort *port);
void KfunRxConnection(pio AvPort *port);
void KfunTxConnection(pio AvPort *port);
void KfunHdcpUnMuteAv(pio AvPort *port);
void KfunHdcpMuteAv(pio AvPort *port);
AvRet KfunCopyBksv(pout AvPort *RxPort, pin AvPort *TxPort);
void KfunHdcpReset(pio AvPort *port);
void KfunTxHdcpManage(pio AvPort *port);
void KfunTxSetMuteAv(pio AvPort *port);
void KfunTxClearMuteAv(pio AvPort *port);
void KfunHdcp2p2Manage(pio AvPort *port);
void KfunTxUnlockProtection(pio AvPort *port);

/* PART 3 */
/* Transmitter FSM */
void KfunTxTmdsMute(pout AvPort *port);
void KfunTxTmdsUnmute(pout AvPort *port);
AvRet KfunTxVideoManage(pout AvPort *port);
AvRet KfunTxAudioManage(pout AvPort *port);
void KfunPollingTxStatus(pout AvPort *port);
AvRet KfunTxUseDefaultEdid(pin AvPort *port);
AvRet KfunTxEdidError(pin AvPort *port);
uint8 KfunFindCecSPAFromEdid(uint8 *currptr, uint8 *spa);
void KfunGenerateSourceSpa(AvPort *port, uint8 *Spa, uint8 DedicateAddr);
AvRet KfunCecUpdateFromEdid(AvPort *port);
AvRet KfunTxSinkLost(AvPort *port);
void KfunTxSetColorDepth(AvPort *RxPort, AvPort *TxPort);
void KfunTxSetHdmiModeSupport(pout AvPort *port);

/* PART 4 */
/* Routing FSM */
void KfunTxRoutingVarInit(pout AvPort *port);
void KfunRxRoutingVarInit(pout AvPort *port);
void KfunRxRoutingClearEndInfo(pin AvPort *port);

/* PART 5 */
/* Plug RX FSM */
void KfunPrVarInit(pin AvPort *port);
void KfunPrResetPort(pin AvPort *port);
void KfunPrHpdDown(pin AvPort *port);
void KfunPrHpdUp(pin AvPort *port);
void KfunManageHpa(pin AvPort *port);
void KfunPrepareEdid(pin AvPort *port);
void KfunStdiReadStatus(pin AvPort *port);
void KfunRxReadInfo(pin AvPort *port);
void KfunRxWriteEdid(pio AvPort *port, uint8 *MergedEdid, uint8 SpaLocation, uint8 *SpaValue);
void KfunTxReadEdid(AvPort *port, uint8 *ReadData);
void KfunRxVicFilter(pin AvPort *port);

/* PART 6 */
/* Plug TX FSM */
void KfunPtVarInit(pout AvPort *port);
void KfunPtPortInit(pout AvPort *port);
void KfunPtResetPort(pout AvPort *port);
void KfunPtEnableTxCore(pout AvPort *port);
void KfunPtAntiDither(pout AvPort *port);
void KfunTransmitVideo(pout AvPort *port);
AvPort* KfunLookupFromPort(AvPort *port);

#if AvEnableCecFeature /* CEC Related */
/* PART 7 */
AvRet ValidMsgSize (CecMessage Opcode, uchar MsgSize);
void SendSetSystemAudioMode (AvPort* port, uint8 status);
AvRet KfunCecInit(AvPort *port);
AvRet KfunCecRxMsgProcess(AvPort *port);
AvRet KfunPrepareLogAddr(AvPort *port);
AvRet KfunCecSendMessage(AvPort *port);
AvRet KfunCecSetPhysicalAddr(AvPort *port);
AvRet KfunCecSetLogicalAddr(AvPort *port);
AvRet KfunCecManage(AvPort *port);
#endif

#if AvEnableCcaFeature
/* PART 8 */
/* CCA TX FSM */
void KfunCcaTxInit(AvPort *port);
void KfunCcaTxCalibManage(AvPort *port);
void KfunCcaTxLinkManage(AvPort *port);
void KfunCcaTxUnlockProtection(AvPort *port);
void KfunCcaTxSetWorkMode(AvPort *port);
void KfunCcaTxEnablePin(AvPort *port, uint8 SlotName);

/* PART 9 */
/* CCA RX FSM */
void KfunCcaRxInit(AvPort *port);
void KfunCcaRxCalibManage(AvPort *port);
void KfunCcaRxLinkManage(AvPort *port);
void KfunCcaRxUnlockProtection(AvPort *port);
void KfunCcaRxEnablePin(AvPort *port, uint8 SlotName);
#endif

/* Look Up Port */
AvRet KfunUpdatePortFromPacket(pin AvPort *port, PacketType pkt, uint8 *Content);
AvRet KfunCheckVspPortContent(pin AvPort *port, PacketType Pkt, uint8 *Content);
AvRet KfunFindAudioNextTxEnd(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode);
AvRet KfunFindAudioNextNode(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode);
AvRet KfunFindAudioRxFront(AvPort* CurrentNode, AvPort** FoundNode);
AvRet KfunFindVideoNextTxEnd(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode);
AvRet KfunFindVideoNextNode(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode);
AvRet KfunFindVideoRxFront(AvPort* CurrentNode, AvPort** FoundNode);
AvRet KfunFindVideoRxFrontInSameDevice(AvPort* CurrentNode, AvPort** FoundNode);

#ifdef __cplusplus
    }
#endif

#endif
