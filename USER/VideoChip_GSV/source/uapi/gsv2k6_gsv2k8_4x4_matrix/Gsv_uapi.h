/**
 * @file uapi.h
 *
 * @brief low level ports related universal api functions
 */

#ifndef __uapi_h
#define __uapi_h

#include "av_config.h"
#include "Gsv_hal.h"

#if GSV2K6
#include "gsv2k6.h"
#endif
#if GSV2K8
#include "gsv2k8.h"
#endif
#if GSV2K2
#include "gsv2k2.h"
#endif
#if GSV2K1
#include "gsv2k1.h"
#endif
#if GSV2K11
#include "gsv2k11.h"
#endif
#if GSV5K1
#include "gsv5k1.h"
#endif


/* define uapi as internal identification key word */
#define uapi

/* macro to delcare and implement uapi functions for each device */
#define DeclareUapi(device, uapiName)   device##uapiName
#define ImplementUapi(device, uapiName) device##uapiName
#define CallUapi(device, uapiName)      device##uapiName

extern  AvFpI2cRead        AvHookI2cRead;
extern  AvFpI2cWrite       AvHookI2cWrite;
extern  AvFpUartSendByte   AvHookUartTxByte;
extern  AvFpUartGetByte    AvHookUartRxByte;
extern  AvFpGetMilliSecond AvHookGetMilliSecond;
extern  AvFpGetKey         AvHookGetKey;
extern  AvFpGetIrda        AvHookGetIrda;
/* Video Interrupt */
typedef struct
{
    uint8 AvMute;
    uint8 HdmiModeChg;
    uint8 DeRegenLck;
    uint8 VsyncLck;
    uint8 Vid3dDet;
    uint8 NewTmds;
    uint8 BadTmdsClk;
    uint8 DeepClrChg;
    uint8 PktErr;
    uint8 AvIfValid;
    uint8 SpdValid;
    uint8 HdrValid;
    uint8 MsValid;
    uint8 VsValid;
    uint8 Isrc1Valid;
    uint8 Isrc2Valid;
    uint8 GamutValid;
    uint8 GcValid;
    uint8 EmpValid;
}   VideoInterrupt;

/* Audio Interrupt */
typedef struct
{
    uint8 AudChanMode;
    uint8 InternMute;
    uint8 CsDataValid;
    uint8 NChange;
    uint8 CtsThresh;
    uint8 AudFifoOv;
    uint8 AudFifoUn;
    uint8 AudFifoNrOv;
    uint8 AudioPktErr;
    uint8 AudModeChg;
    uint8 AudFifoNrUn;
    uint8 AudFlatLine;
    uint8 AudSampChg;
    uint8 AudPrtyErr;
    uint8 AudIfValid;
    uint8 AcpValid;
}   AudioInterrupt;

/* Hdcp Interrupt */
typedef struct
{
    uint8 AksvUpdate;
    uint8 Encrypted;
}   HdcpInterrupt;


/* exported functions */
#ifdef __cplusplus
extern "C" {
#endif

uapi AvRet AvUapiInit(void);
uapi AvRet AvUapiHookBspFunctions(pin AvFpI2cRead i2cRd, pin AvFpI2cWrite i2cWr, pin AvFpUartSendByte uartTxB, pin AvFpUartGetByte uartRxB, pin AvFpGetMilliSecond getMs, pin AvFpGetKey getKey,pin AvFpGetIrda getIrda);
uapi AvRet AvUapiHookBspFunctionsInit(pin AvFpI2cRead i2cRd, pin AvFpI2cWrite i2cWr);
uapi AvRet AvUapiOuputDbgMsg(pin schar *FormattedString, ...); /* do not call this api, call macro "AvUapiOutputDebugMessage()" */

#if  AvEnableDebugMessage
#define AvUapiOutputDebugMessage AvUapiOuputDbgMsg
#else
#define AvUapiOutputDebugMessage(...)
#endif
#if AvEnableDebugFsm
#define AvUapiOutputDebugFsm AvUapiOuputDbgMsg
#else
#define AvUapiOutputDebugFsm(...)
#endif

uapi AvRet AvUapiAllocateMemory(pin uint32 bytes, pout uint32 *bufferAddress);

uapi AvRet AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type);
uapi AvRet AvUapiDisconnectPort(pin AvPort *port);

uapi AvRet AvUapiInitDevice(pio AvDevice *device);
uapi AvRet AvUapiResetPort(pio AvPort *port);
uapi AvRet AvUapiEnablePort(pio AvPort *port);

uapi AvRet AvUapiRxPortInit(pio AvPort *port);
uapi AvRet AvUapiRxGetStatus(pio AvPort *port);
uapi AvRet AvUapiRxEnableFreeRun(pio AvPort *port, bool enable);
uapi AvRet AvUapiRxGetHdcpStatus(pio AvPort *port, HdcpInterrupt* Intpt);
uapi AvRet AvUapiRxGetVideoPacketStatus(pio AvPort *port, VideoInterrupt* Intpt);
uapi AvRet AvUapiRxGetAudioPacketStatus(pio AvPort *port, AudioInterrupt* Intpt);
uapi AvRet AvUapiRxGet5VStatus(pio AvPort *port);
uapi AvRet AvUapiRxSetHpdDown(pio AvPort *port);
uapi AvRet AvUapiRxSetHpdUp(pio AvPort *port);
uapi AvRet AvUapiRxClearFlags(pio AvPort *port);
uapi AvRet AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content);
uapi AvRet AvUapiRxGetHdmiAcrInfo(pio AvPort *port);
uapi AvRet AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt);
uapi AvRet AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt);
uapi AvRet AvUapiRxClearHdcpInterrupt(pio AvPort *port,  HdcpInterrupt* Intpt);

uapi AvRet AvUapiTxPortInit(pio AvPort *port);
uapi AvRet AvUapiTxEnableCore(pin AvPort *port);
uapi AvRet AvUapiTxDisableCore(pin AvPort *port);
uapi AvRet AvUapiTxGetStatus(pio AvPort *port);
uapi AvRet AvUapiTxMuteTmds(pio AvPort *port, bool mute);
uapi AvRet AvUapiTxGetSinkHdcpCapability(pio AvPort *port);
uapi AvRet AvUapiTxSetAudioPackets(pin AvPort *port);
uapi AvRet AvUapiTxVideoManage(pio AvPort *port);
uapi AvRet AvUapiTxAudioManage(pio AvPort *port);
uapi AvRet AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn);
uapi AvRet AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, bool Enable);
uapi AvRet AvUapiTxGetHdcpStatus(pin AvPort *port);
uapi AvRet AvUapiTxGetBksvReady(pin AvPort *port);

uapi AvRet AvUapiRxGetAvMute(pio AvPort *port);
uapi AvRet AvUapiTxSetAvMute(pio AvPort *port);
uapi AvRet AvUapiTxSetBlackMute(pio AvPort *port);
uapi AvRet AvUapiRxGetHdmiModeSupport(pio AvPort *port);
uapi AvRet AvUapiTxSetHdmiModeSupport(pio AvPort *port);
uapi AvRet AvUapiTxSetFeatureSupport(pio AvPort *port);
uapi AvRet AvUapiRxGetVideoLock(pio AvPort *port);
uapi AvRet AvUapiRxGetVideoTiming(pio AvPort *port);
uapi AvRet AvUapiTxSetVideoTiming(pio AvPort *port);
uapi AvRet AvUapiRxGetHdmiDeepColor(pio AvPort *port);
uapi AvRet AvUapiTxSetHdmiDeepColor(pio AvPort *port);
uapi AvRet AvUapiRxGetAudioInternalMute(pio AvPort *port);
uapi AvRet AvUapiRxSetAudioInternalMute(pio AvPort *port);
uapi AvRet AvUapiTxSetAudNValue(pio AvPort *port);
uapi AvRet AvUapiRxGetPacketType(pio AvPort *port);
uapi AvRet AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count);
uapi AvRet AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position);
uapi AvRet AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value);
uapi AvRet AvUapiTxEncryptSink(pin AvPort *port);
uapi AvRet AvUapiTxDecryptSink(pin AvPort *port);
uapi AvRet AvUapiRxSetHdcpEnable(pin AvPort *port);
uapi AvRet AvUapiRxSetBksvListReady(pin AvPort *port);
uapi AvRet AvUapiRxSetHdcpMode(pin AvPort *port);
uapi AvRet AvUapiHdcp2p2Mode(pin AvPort *port);
uapi AvRet AvUapiTxClearBksvReady(pin AvPort *port);
uapi AvRet AvUapiTxClearRxidReady(pin AvPort *port);
uapi AvRet AvUapiTxClearHdcpError(pin AvPort *port);
uapi AvRet AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv);
uapi AvRet AvUapiCecSendMessage(pin AvPort *port);
uapi AvRet AvUapiGetNackCount(pin AvPort *port);
uapi AvRet AvUapiTxCecInit(pin AvPort *port);
uapi AvRet AvUapiCecRxGetStatus(pin AvPort *port);
uapi AvRet AvUapiCecTxGetStatus(pin AvPort *port);
uapi AvRet AvUapiTxCecSetPhysicalAddr(AvPort *port);
uapi AvRet AvUapiTxCecSetLogicalAddr(AvPort *port);
uapi AvRet AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count);
uapi AvRet AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count);
uapi AvRet AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue);
uapi AvRet AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count);
uapi AvRet AvUapiRxEnableInternalEdid(pio AvPort *port);
uapi AvRet AvUapiTxArcEnable(pio AvPort *port, uint8 value);
uapi AvRet AvUapiRxHdcp2p2Manage(pin AvPort *port);
uapi AvRet AvUapiTxHdcp2p2Manage(pin AvPort *port);
uapi AvRet AvUapiCheckLogicAudioTx(pio AvPort *port);
uapi AvRet AvUapiCheckLogicAudioRx(pio AvPort *port);
uapi AvRet AvUapiCheckVideoScaler(pio AvPort *port);
uapi AvRet AvUapiCheckVideoColor(pio AvPort *port);
uapi AvRet AvUapiCheckVideoGen(pio AvPort *port);
uapi AvRet AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt);
uapi AvRet AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt);
uapi AvRet AvUapiTxDetectMode(pin AvPort *port);
uapi AvRet AvUapiRxReadStdi(pin AvPort *port);
uapi AvRet AvUapiRxReadInfo(pin AvPort *port);

#if GSV5K1
uapi AvRet AvUapiCheckVideoDsc(pio AvPort *port);
#if AvEnableCcaFeature /* CCA Related */
uapi AvRet AvUapiCcaTxGetInfo(pio AvPort *port);
uapi AvRet AvUapiCcaRxGetInfo(pio AvPort *port);
uapi AvRet AvUapiCcaTxInit(pio AvPort *port);
uapi AvRet AvUapiCcaRxInit(pio AvPort *port);
uapi AvRet AvUapiCcaTxChainReset(pio AvPort *port);
uapi AvRet AvUapiCcaRxChainReset(pio AvPort *port);
uapi AvRet AvUapiCcaTxCalibManage(pio AvPort *port);
uapi AvRet AvUapiCcaRxCalibManage(pio AvPort *port);
uapi AvRet AvUapiCcaTxSetWorkMode(pio AvPort *port);
uapi AvRet AvUapiCcaTxEnablePin(pio AvPort *port, uint8 SlotName);
uapi AvRet AvUapiCcaRxEnablePin(pio AvPort *port, uint8 SlotName);
#endif /* CCA Related */
#endif

#if GSV2K11
uapi AvRet AvUapiCheckLogicVideoTx(pio AvPort *port);
uapi AvRet AvUapiCheckLogicVideoRx(pio AvPort *port);
uapi AvRet AvUapiCheckAudioGen(pio AvPort *port);
uapi AvRet AvUapiCheckClockGen(pio AvPort *port);
uapi AvRet AvUapiRxEdidStat(pin AvPort *port);
#endif
/* device uapi function delaration */

#if GSV2K6 /* GSV2K6 uapi definition start */
#ifdef Gsv2k6_AvUapiConnectPort
    AvRet DeclareUapi(Gsv2k6, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiDisconnectPort
    AvRet DeclareUapi(Gsv2k6, AvUapiDisconnectPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiDisconnectPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiInitDevice
    AvRet DeclareUapi(Gsv2k6, AvUapiInitDevice(pin AvDevice *device));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiInitDevice(pin AvDevice *device)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxPortInit
    AvRet DeclareUapi(Gsv2k6, AvUapiRxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxPortInit
    AvRet DeclareUapi(Gsv2k6, AvUapiTxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiEnablePort
    AvRet DeclareUapi(Gsv2k6, AvUapiEnablePort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiEnablePort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxEnableFreeRun
    AvRet DeclareUapi(Gsv2k6, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxEnableCore
    AvRet DeclareUapi(Gsv2k6, AvUapiTxEnableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxEnableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxDisableCore
    AvRet DeclareUapi(Gsv2k6, AvUapiTxDisableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxDisableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxMuteTmds
    AvRet DeclareUapi(Gsv2k6, AvUapiTxMuteTmds(pin AvPort *port, bool mute));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxMuteTmds(pin AvPort *port, bool mute)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxGetStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxVideoManage
    AvRet DeclareUapi(Gsv2k6, AvUapiTxVideoManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxVideoManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxAudioManage
    AvRet DeclareUapi(Gsv2k6, AvUapiTxAudioManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxAudioManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxVideoManage
    AvRet DeclareUapi(Gsv2k6, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxAudioManage
    AvRet DeclareUapi(Gsv2k6, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiResetPort
    AvRet DeclareUapi(Gsv2k6, AvUapiResetPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiResetPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetHpdUp
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHpdUp(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHpdUp(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGet5VStatus(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGet5VStatus(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHpdDown(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHpdDown(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetAudioPackets
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetAudioPackets(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetAudioPackets(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetAudioPacketStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxClearFlags
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearFlags(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearFlags(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetVideoPacketStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetPacketContent
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetPacketContent
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxEnableInfoFrames
    AvRet DeclareUapi(Gsv2k6, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetHdmiAcrInfo
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdmiAcrInfo (pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdmiAcrInfo (pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxClearAudioInterrupt
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxClearVideoInterrupt
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxClearHdcpInterrupt
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetAvMute
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetAvMute
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetFeatureSupport
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetFeatureSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetFeatureSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetVideoLock
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetVideoLock(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetVideoLock(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetVideoTiming
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetVideoTiming
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetAudioInternalMute
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetAudioInternalMute
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetAudNValue
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetAudNValue(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetAudNValue(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxGetPacketType
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetPacketType(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxGetPacketType(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetHdcpStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetHdcpStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxReadBksv
    AvRet DeclareUapi(Gsv2k6, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxAddBksv
    AvRet DeclareUapi(Gsv2k6, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxGetBksvTotal
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxSetBlackMute
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetBlackMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxSetBlackMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxEncryptSink
    AvRet DeclareUapi(Gsv2k6, AvUapiTxEncryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxEncryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxDecryptSink
    AvRet DeclareUapi(Gsv2k6, AvUapiTxDecryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxDecryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxGetBksvReady
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetHdcpEnable
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHdcpEnable(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHdcpEnable(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetBksvListReady
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetBksvListReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetBksvListReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHdcpMode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetHdcpMode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k6, AvUapiHdcp2p2Mode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiHdcp2p2Mode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxClearBksvReady
    AvRet DeclareUapi(Gsv2k6, AvUapiTxClearBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxClearBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxClearRxidReady
    AvRet DeclareUapi(Gsv2k6, AvUapiTxClearRxidReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxClearRxidReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxClearHdcpError
    AvRet DeclareUapi(Gsv2k6, AvUapiTxClearHdcpError(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxClearHdcpError(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxCheckBksvExisted
    AvRet DeclareUapi(Gsv2k6, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxGetSinkHdcpCapability
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetSinkHdcpCapability(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxGetSinkHdcpCapability(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCecSendMessage
    AvRet DeclareUapi(Gsv2k6, AvUapiCecSendMessage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCecSendMessage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiGetNackCount
    AvRet DeclareUapi(Gsv2k6, AvUapiGetNackCount(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiGetNackCount(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxCecInit
    AvRet DeclareUapi(Gsv2k6, AvUapiTxCecInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxCecInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCecRxGetStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiCecRxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCecRxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCecTxGetStatus
    AvRet DeclareUapi(Gsv2k6, AvUapiCecTxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCecTxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxCecSetPhysicalAddr
    AvRet DeclareUapi(Gsv2k6, AvUapiTxCecSetPhysicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxCecSetPhysicalAddr(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxCecSetLogicalAddr
    AvRet DeclareUapi(Gsv2k6, AvUapiTxCecSetLogicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxCecSetLogicalAddr(AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxReadEdid
    AvRet DeclareUapi(Gsv2k6, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxWriteEdid
    AvRet DeclareUapi(Gsv2k6, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxSetSpa
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxReadEdid
    AvRet DeclareUapi(Gsv2k6, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxEnableInternalEdid
    AvRet DeclareUapi(Gsv2k6, AvUapiRxEnableInternalEdid(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxEnableInternalEdid(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxArcEnable
    AvRet DeclareUapi(Gsv2k6, AvUapiTxArcEnable(pio AvPort *port, uint8 value));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxArcEnable(pio AvPort *port, uint8 value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiRxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k6, AvUapiRxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiRxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiTxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k6, AvUapiTxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiTxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCheckLogicAudioTx
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckLogicAudioTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckLogicAudioTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCheckLogicAudioRx
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckLogicAudioRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckLogicAudioRx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCheckVideoScaler
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckVideoScaler(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckVideoScaler(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCheckVideoColor
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckVideoColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckVideoColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k6_AvUapiCheckVideoGen
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckVideoGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k6, AvUapiCheckVideoGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#endif /* GSV2K6 uapi definition end */

#if GSV2K8 /* GSV2K8 uapi definition start */
#ifdef Gsv2k8_AvUapiConnectPort
    AvRet DeclareUapi(Gsv2k8, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiDisconnectPort
    AvRet DeclareUapi(Gsv2k8, AvUapiDisconnectPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiDisconnectPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiInitDevice
    AvRet DeclareUapi(Gsv2k8, AvUapiInitDevice(pin AvDevice *device));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiInitDevice(pin AvDevice *device)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxPortInit
    AvRet DeclareUapi(Gsv2k8, AvUapiRxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxPortInit
    AvRet DeclareUapi(Gsv2k8, AvUapiTxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiEnablePort
    AvRet DeclareUapi(Gsv2k8, AvUapiEnablePort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiEnablePort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxEnableFreeRun
    AvRet DeclareUapi(Gsv2k8, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxEnableCore
    AvRet DeclareUapi(Gsv2k8, AvUapiTxEnableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxEnableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxDisableCore
    AvRet DeclareUapi(Gsv2k8, AvUapiTxDisableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxDisableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxMuteTmds
    AvRet DeclareUapi(Gsv2k8, AvUapiTxMuteTmds(pin AvPort *port, bool mute));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxMuteTmds(pin AvPort *port, bool mute)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxGetStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxVideoManage
    AvRet DeclareUapi(Gsv2k8, AvUapiTxVideoManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxVideoManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxAudioManage
    AvRet DeclareUapi(Gsv2k8, AvUapiTxAudioManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxAudioManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxVideoManage
    AvRet DeclareUapi(Gsv2k8, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxAudioManage
    AvRet DeclareUapi(Gsv2k8, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiResetPort
    AvRet DeclareUapi(Gsv2k8, AvUapiResetPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiResetPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetHpdUp
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHpdUp(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHpdUp(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGet5VStatus(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGet5VStatus(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHpdDown(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHpdDown(pin AvPort *port)) {return AvNotSupport;}
#endif

#ifdef Gsv2k8_AvUapiTxSetAudioPackets
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetAudioPackets(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetAudioPackets(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetAudioPacketStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxClearFlags
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearFlags(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearFlags(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetVideoPacketStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetPacketContent
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetPacketContent
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxEnableInfoFrames
    AvRet DeclareUapi(Gsv2k8, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetHdmiAcrInfo
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdmiAcrInfo (pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdmiAcrInfo (pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxClearAudioInterrupt
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxClearVideoInterrupt
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxClearHdcpInterrupt
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetAvMute
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetAvMute
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetFeatureSupport
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetFeatureSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetFeatureSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetVideoLock
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetVideoLock(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetVideoLock(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetVideoTiming
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetVideoTiming
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetAudioInternalMute
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetAudioInternalMute
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetAudNValue
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetAudNValue(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetAudNValue(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxGetPacketType
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetPacketType(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxGetPacketType(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetHdcpStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetHdcpStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxReadBksv
    AvRet DeclareUapi(Gsv2k8, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxAddBksv
    AvRet DeclareUapi(Gsv2k8, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxGetBksvTotal
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxSetBlackMute
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetBlackMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxSetBlackMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxEncryptSink
    AvRet DeclareUapi(Gsv2k8, AvUapiTxEncryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxEncryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxDecryptSink
    AvRet DeclareUapi(Gsv2k8, AvUapiTxDecryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxDecryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxGetBksvReady
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetHdcpEnable
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHdcpEnable(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHdcpEnable(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetBksvListReady
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetBksvListReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetBksvListReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHdcpMode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetHdcpMode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k8, AvUapiHdcp2p2Mode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiHdcp2p2Mode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxClearBksvReady
    AvRet DeclareUapi(Gsv2k8, AvUapiTxClearBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxClearBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxClearRxidReady
    AvRet DeclareUapi(Gsv2k8, AvUapiTxClearRxidReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxClearRxidReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxClearHdcpError
    AvRet DeclareUapi(Gsv2k8, AvUapiTxClearHdcpError(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxClearHdcpError(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxCheckBksvExisted
    AvRet DeclareUapi(Gsv2k8, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxGetSinkHdcpCapability
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetSinkHdcpCapability(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxGetSinkHdcpCapability(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCecSendMessage
    AvRet DeclareUapi(Gsv2k8, AvUapiCecSendMessage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCecSendMessage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiGetNackCount
    AvRet DeclareUapi(Gsv2k8, AvUapiGetNackCount(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiGetNackCount(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxCecInit
    AvRet DeclareUapi(Gsv2k8, AvUapiTxCecInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxCecInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCecRxGetStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiCecRxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCecRxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCecTxGetStatus
    AvRet DeclareUapi(Gsv2k8, AvUapiCecTxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCecTxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxCecSetPhysicalAddr
    AvRet DeclareUapi(Gsv2k8, AvUapiTxCecSetPhysicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxCecSetPhysicalAddr(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxCecSetLogicalAddr
    AvRet DeclareUapi(Gsv2k8, AvUapiTxCecSetLogicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxCecSetLogicalAddr(AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxReadEdid
    AvRet DeclareUapi(Gsv2k8, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxWriteEdid
    AvRet DeclareUapi(Gsv2k8, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxSetSpa
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxReadEdid
    AvRet DeclareUapi(Gsv2k8, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxEnableInternalEdid
    AvRet DeclareUapi(Gsv2k8, AvUapiRxEnableInternalEdid(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxEnableInternalEdid(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxArcEnable
    AvRet DeclareUapi(Gsv2k8, AvUapiTxArcEnable(pio AvPort *port, uint8 value));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxArcEnable(pio AvPort *port, uint8 value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiRxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k8, AvUapiRxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiRxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiTxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k8, AvUapiTxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiTxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCheckLogicAudioTx
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckLogicAudioTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckLogicAudioTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCheckLogicAudioRx
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckLogicAudioRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckLogicAudioRx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCheckVideoScaler
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckVideoScaler(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckVideoScaler(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCheckVideoColor
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckVideoColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckVideoColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k8_AvUapiCheckVideoGen
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckVideoGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k8, AvUapiCheckVideoGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#endif /* GSV2K8 uapi definition end */

#if GSV2K2 /* GSV2K2 uapi definition start */
#ifdef Gsv2k2_AvUapiConnectPort
    AvRet DeclareUapi(Gsv2k2, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiDisconnectPort
    AvRet DeclareUapi(Gsv2k2, AvUapiDisconnectPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiDisconnectPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiInitDevice
    AvRet DeclareUapi(Gsv2k2, AvUapiInitDevice(pin AvDevice *device));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiInitDevice(pin AvDevice *device)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxPortInit
    AvRet DeclareUapi(Gsv2k2, AvUapiRxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxPortInit
    AvRet DeclareUapi(Gsv2k2, AvUapiTxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiEnablePort
    AvRet DeclareUapi(Gsv2k2, AvUapiEnablePort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiEnablePort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxEnableFreeRun
    AvRet DeclareUapi(Gsv2k2, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxEnableCore
    AvRet DeclareUapi(Gsv2k2, AvUapiTxEnableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxEnableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxDisableCore
    AvRet DeclareUapi(Gsv2k2, AvUapiTxDisableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxDisableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxMuteTmds
    AvRet DeclareUapi(Gsv2k2, AvUapiTxMuteTmds(pin AvPort *port, bool mute));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxMuteTmds(pin AvPort *port, bool mute)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxGetStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxVideoManage
    AvRet DeclareUapi(Gsv2k2, AvUapiTxVideoManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxVideoManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxAudioManage
    AvRet DeclareUapi(Gsv2k2, AvUapiTxAudioManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxAudioManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxVideoManage
    AvRet DeclareUapi(Gsv2k2, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxAudioManage
    AvRet DeclareUapi(Gsv2k2, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiResetPort
    AvRet DeclareUapi(Gsv2k2, AvUapiResetPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiResetPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetHpdUp
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHpdUp(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHpdUp(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGet5VStatus(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGet5VStatus(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHpdDown(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHpdDown(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetAudioPackets
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetAudioPackets(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetAudioPackets(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetAudioPacketStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxClearFlags
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearFlags(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearFlags(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetVideoPacketStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetPacketContent
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetPacketContent
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxEnableInfoFrames
    AvRet DeclareUapi(Gsv2k2, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetHdmiAcrInfo
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdmiAcrInfo (pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdmiAcrInfo (pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxClearAudioInterrupt
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxClearVideoInterrupt
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxClearHdcpInterrupt
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetAvMute
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetAvMute
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetFeatureSupport
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetFeatureSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetFeatureSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetVideoLock
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetVideoLock(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetVideoLock(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetVideoTiming
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetVideoTiming
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetAudioInternalMute
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetAudioInternalMute
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetAudNValue
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetAudNValue(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetAudNValue(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxGetPacketType
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetPacketType(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxGetPacketType(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetHdcpStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetHdcpStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxReadBksv
    AvRet DeclareUapi(Gsv2k2, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxAddBksv
    AvRet DeclareUapi(Gsv2k2, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxGetBksvTotal
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxSetBlackMute
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetBlackMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxSetBlackMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxEncryptSink
    AvRet DeclareUapi(Gsv2k2, AvUapiTxEncryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxEncryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxDecryptSink
    AvRet DeclareUapi(Gsv2k2, AvUapiTxDecryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxDecryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxGetBksvReady
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetHdcpEnable
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHdcpEnable(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHdcpEnable(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetBksvListReady
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetBksvListReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetBksvListReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHdcpMode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetHdcpMode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k2, AvUapiHdcp2p2Mode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiHdcp2p2Mode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxClearBksvReady
    AvRet DeclareUapi(Gsv2k2, AvUapiTxClearBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxClearBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxClearRxidReady
    AvRet DeclareUapi(Gsv2k2, AvUapiTxClearRxidReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxClearRxidReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxClearHdcpError
    AvRet DeclareUapi(Gsv2k2, AvUapiTxClearHdcpError(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxClearHdcpError(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxCheckBksvExisted
    AvRet DeclareUapi(Gsv2k2, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxGetSinkHdcpCapability
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetSinkHdcpCapability(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxGetSinkHdcpCapability(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCecSendMessage
    AvRet DeclareUapi(Gsv2k2, AvUapiCecSendMessage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCecSendMessage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiGetNackCount
    AvRet DeclareUapi(Gsv2k2, AvUapiGetNackCount(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiGetNackCount(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxCecInit
    AvRet DeclareUapi(Gsv2k2, AvUapiTxCecInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxCecInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCecRxGetStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiCecRxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCecRxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCecTxGetStatus
    AvRet DeclareUapi(Gsv2k2, AvUapiCecTxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCecTxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxCecSetPhysicalAddr
    AvRet DeclareUapi(Gsv2k2, AvUapiTxCecSetPhysicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxCecSetPhysicalAddr(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxCecSetLogicalAddr
    AvRet DeclareUapi(Gsv2k2, AvUapiTxCecSetLogicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxCecSetLogicalAddr(AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxReadEdid
    AvRet DeclareUapi(Gsv2k2, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxWriteEdid
    AvRet DeclareUapi(Gsv2k2, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxSetSpa
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxReadEdid
    AvRet DeclareUapi(Gsv2k2, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxEnableInternalEdid
    AvRet DeclareUapi(Gsv2k2, AvUapiRxEnableInternalEdid(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxEnableInternalEdid(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxArcEnable
    AvRet DeclareUapi(Gsv2k2, AvUapiTxArcEnable(pio AvPort *port, uint8 value));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxArcEnable(pio AvPort *port, uint8 value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiRxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k2, AvUapiRxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiTxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k2, AvUapiTxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiTxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCheckLogicAudioTx
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckLogicAudioTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckLogicAudioTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCheckLogicAudioRx
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckLogicAudioRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckLogicAudioRx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCheckVideoScaler
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckVideoScaler(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckVideoScaler(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCheckVideoColor
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckVideoColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckVideoColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k2_AvUapiCheckVideoGen
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckVideoGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiCheckVideoGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#endif /* GSV2K2 uapi definition end */

#if GSV2K1 /* GSV2K1 uapi definition start */
#ifdef Gsv2k1_AvUapiConnectPort
    AvRet DeclareUapi(Gsv2k1, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiDisconnectPort
    AvRet DeclareUapi(Gsv2k1, AvUapiDisconnectPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiDisconnectPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiInitDevice
    AvRet DeclareUapi(Gsv2k1, AvUapiInitDevice(pin AvDevice *device));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiInitDevice(pin AvDevice *device)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxPortInit
    AvRet DeclareUapi(Gsv2k1, AvUapiRxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k2, AvUapiRxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxPortInit
    AvRet DeclareUapi(Gsv2k1, AvUapiTxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiEnablePort
    AvRet DeclareUapi(Gsv2k1, AvUapiEnablePort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiEnablePort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxEnableFreeRun
    AvRet DeclareUapi(Gsv2k1, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxEnableCore
    AvRet DeclareUapi(Gsv2k1, AvUapiTxEnableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxEnableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxDisableCore
    AvRet DeclareUapi(Gsv2k1, AvUapiTxDisableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxDisableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxMuteTmds
    AvRet DeclareUapi(Gsv2k1, AvUapiTxMuteTmds(pin AvPort *port, bool mute));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxMuteTmds(pin AvPort *port, bool mute)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxGetStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxVideoManage
    AvRet DeclareUapi(Gsv2k1, AvUapiTxVideoManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxVideoManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxAudioManage
    AvRet DeclareUapi(Gsv2k1, AvUapiTxAudioManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxAudioManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxVideoManage
    AvRet DeclareUapi(Gsv2k1, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxAudioManage
    AvRet DeclareUapi(Gsv2k1, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiResetPort
    AvRet DeclareUapi(Gsv2k1, AvUapiResetPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiResetPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetHpdUp
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHpdUp(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHpdUp(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGet5VStatus(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGet5VStatus(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHpdDown(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHpdDown(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetAudioPackets
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetAudioPackets(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetAudioPackets(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetAudioPacketStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxClearFlags
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearFlags(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearFlags(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetVideoPacketStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetPacketContent
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetPacketContent
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxEnableInfoFrames
    AvRet DeclareUapi(Gsv2k1, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetHdmiAcrInfo
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdmiAcrInfo (pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdmiAcrInfo (pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxClearAudioInterrupt
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxClearVideoInterrupt
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxClearHdcpInterrupt
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetAvMute
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetAvMute
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetFeatureSupport
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetFeatureSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetFeatureSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetVideoLock
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetVideoLock(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetVideoLock(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetVideoTiming
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetVideoTiming
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetAudioInternalMute
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetAudioInternalMute
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetAudNValue
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetAudNValue(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetAudNValue(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxGetPacketType
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetPacketType(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxGetPacketType(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetHdcpStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetHdcpStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxReadBksv
    AvRet DeclareUapi(Gsv2k1, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxAddBksv
    AvRet DeclareUapi(Gsv2k1, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxGetBksvTotal
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxSetBlackMute
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetBlackMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxSetBlackMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxEncryptSink
    AvRet DeclareUapi(Gsv2k1, AvUapiTxEncryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxEncryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxDecryptSink
    AvRet DeclareUapi(Gsv2k1, AvUapiTxDecryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxDecryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxGetBksvReady
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetHdcpEnable
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHdcpEnable(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHdcpEnable(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetBksvListReady
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetBksvListReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetBksvListReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHdcpMode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetHdcpMode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k1, AvUapiHdcp2p2Mode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiHdcp2p2Mode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxClearBksvReady
    AvRet DeclareUapi(Gsv2k1, AvUapiTxClearBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxClearBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxClearRxidReady
    AvRet DeclareUapi(Gsv2k1, AvUapiTxClearRxidReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxClearRxidReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxClearHdcpError
    AvRet DeclareUapi(Gsv2k1, AvUapiTxClearHdcpError(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxClearHdcpError(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxCheckBksvExisted
    AvRet DeclareUapi(Gsv2k1, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxGetSinkHdcpCapability
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetSinkHdcpCapability(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxGetSinkHdcpCapability(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCecSendMessage
    AvRet DeclareUapi(Gsv2k1, AvUapiCecSendMessage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCecSendMessage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiGetNackCount
    AvRet DeclareUapi(Gsv2k1, AvUapiGetNackCount(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiGetNackCount(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxCecInit
    AvRet DeclareUapi(Gsv2k1, AvUapiTxCecInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxCecInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCecRxGetStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiCecRxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCecRxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCecTxGetStatus
    AvRet DeclareUapi(Gsv2k1, AvUapiCecTxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCecTxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxCecSetPhysicalAddr
    AvRet DeclareUapi(Gsv2k1, AvUapiTxCecSetPhysicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxCecSetPhysicalAddr(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxCecSetLogicalAddr
    AvRet DeclareUapi(Gsv2k1, AvUapiTxCecSetLogicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxCecSetLogicalAddr(AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxReadEdid
    AvRet DeclareUapi(Gsv2k1, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxWriteEdid
    AvRet DeclareUapi(Gsv2k1, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxSetSpa
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxReadEdid
    AvRet DeclareUapi(Gsv2k1, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxEnableInternalEdid
    AvRet DeclareUapi(Gsv2k1, AvUapiRxEnableInternalEdid(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxEnableInternalEdid(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxArcEnable
    AvRet DeclareUapi(Gsv2k1, AvUapiTxArcEnable(pio AvPort *port, uint8 value));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxArcEnable(pio AvPort *port, uint8 value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiRxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k1, AvUapiRxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiRxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiTxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k1, AvUapiTxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiTxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCheckLogicAudioTx
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckLogicAudioTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckLogicAudioTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCheckLogicAudioRx
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckLogicAudioRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckLogicAudioRx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCheckVideoScaler
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckVideoScaler(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckVideoScaler(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCheckVideoColor
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckVideoColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckVideoColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k1_AvUapiCheckVideoGen
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckVideoGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k1, AvUapiCheckVideoGen(pio AvPort *port)) {return AvNotSupport;}
#endif
#endif

#if GSV2K11 /* GSV2K11 uapi definition start */
#ifdef Gsv2k11_AvUapiConnectPort
    AvRet DeclareUapi(Gsv2k11, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiDisconnectPort
    AvRet DeclareUapi(Gsv2k11, AvUapiDisconnectPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiDisconnectPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiInitDevice
    AvRet DeclareUapi(Gsv2k11, AvUapiInitDevice(pin AvDevice *device));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiInitDevice(pin AvDevice *device)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxPortInit
    AvRet DeclareUapi(Gsv2k11, AvUapiRxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxPortInit
    AvRet DeclareUapi(Gsv2k11, AvUapiTxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiEnablePort
    AvRet DeclareUapi(Gsv2k11, AvUapiEnablePort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiEnablePort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxEnableFreeRun
    AvRet DeclareUapi(Gsv2k11, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxEnableCore
    AvRet DeclareUapi(Gsv2k11, AvUapiTxEnableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxEnableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxDisableCore
    AvRet DeclareUapi(Gsv2k11, AvUapiTxDisableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxDisableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxMuteTmds
    AvRet DeclareUapi(Gsv2k11, AvUapiTxMuteTmds(pin AvPort *port, bool mute));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxMuteTmds(pin AvPort *port, bool mute)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxGetStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxVideoManage
    AvRet DeclareUapi(Gsv2k11, AvUapiTxVideoManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxVideoManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxAudioManage
    AvRet DeclareUapi(Gsv2k11, AvUapiTxAudioManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxAudioManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxVideoManage
    AvRet DeclareUapi(Gsv2k11, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxAudioManage
    AvRet DeclareUapi(Gsv2k11, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiResetPort
    AvRet DeclareUapi(Gsv2k11, AvUapiResetPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiResetPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetHpdUp
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHpdUp(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHpdUp(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGet5VStatus(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGet5VStatus(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHpdDown(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHpdDown(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetAudioPackets
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetAudioPackets(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetAudioPackets(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetAudioPacketStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxClearFlags
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearFlags(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearFlags(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetVideoPacketStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetPacketContent
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetPacketContent
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxEnableInfoFrames
    AvRet DeclareUapi(Gsv2k11, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetHdmiAcrInfo
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdmiAcrInfo (pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdmiAcrInfo (pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxClearAudioInterrupt
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxClearVideoInterrupt
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxClearHdcpInterrupt
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetAvMute
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetAvMute
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetHdmiModeSupport
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetFeatureSupport
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetFeatureSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetFeatureSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetVideoLock
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetVideoLock(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetVideoLock(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetVideoTiming
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetVideoTiming
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetHdmiDeepColor
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetAudioInternalMute
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetAudioInternalMute
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetAudNValue
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetAudNValue(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetAudNValue(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxGetPacketType
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetPacketType(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxGetPacketType(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxGetHdcpStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetHdcpStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetHdcpStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxReadBksv
    AvRet DeclareUapi(Gsv2k11, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxAddBksv
    AvRet DeclareUapi(Gsv2k11, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxGetBksvTotal
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxSetBlackMute
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetBlackMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxSetBlackMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxEncryptSink
    AvRet DeclareUapi(Gsv2k11, AvUapiTxEncryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxEncryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxDecryptSink
    AvRet DeclareUapi(Gsv2k11, AvUapiTxDecryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxDecryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxGetBksvReady
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetHdcpEnable
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHdcpEnable(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHdcpEnable(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetBksvListReady
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetBksvListReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetBksvListReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHdcpMode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetHdcpMode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiHdcp2p2Mode
    AvRet DeclareUapi(Gsv2k11, AvUapiHdcp2p2Mode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiHdcp2p2Mode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxClearBksvReady
    AvRet DeclareUapi(Gsv2k11, AvUapiTxClearBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxClearBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxClearRxidReady
    AvRet DeclareUapi(Gsv2k11, AvUapiTxClearRxidReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxClearRxidReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxClearHdcpError
    AvRet DeclareUapi(Gsv2k11, AvUapiTxClearHdcpError(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxClearHdcpError(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxCheckBksvExisted
    AvRet DeclareUapi(Gsv2k11, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxGetSinkHdcpCapability
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetSinkHdcpCapability(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxGetSinkHdcpCapability(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCecSendMessage
    AvRet DeclareUapi(Gsv2k11, AvUapiCecSendMessage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCecSendMessage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiGetNackCount
    AvRet DeclareUapi(Gsv2k11, AvUapiGetNackCount(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiGetNackCount(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxCecInit
    AvRet DeclareUapi(Gsv2k11, AvUapiTxCecInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxCecInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCecRxGetStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiCecRxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCecRxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCecTxGetStatus
    AvRet DeclareUapi(Gsv2k11, AvUapiCecTxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCecTxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxCecSetPhysicalAddr
    AvRet DeclareUapi(Gsv2k11, AvUapiTxCecSetPhysicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxCecSetPhysicalAddr(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxCecSetLogicalAddr
    AvRet DeclareUapi(Gsv2k11, AvUapiTxCecSetLogicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxCecSetLogicalAddr(AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxReadEdid
    AvRet DeclareUapi(Gsv2k11, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxWriteEdid
    AvRet DeclareUapi(Gsv2k11, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxSetSpa
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxReadEdid
    AvRet DeclareUapi(Gsv2k11, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxEnableInternalEdid
    AvRet DeclareUapi(Gsv2k11, AvUapiRxEnableInternalEdid(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxEnableInternalEdid(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxArcEnable
    AvRet DeclareUapi(Gsv2k11, AvUapiTxArcEnable(pio AvPort *port, uint8 value));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxArcEnable(pio AvPort *port, uint8 value)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiRxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k11, AvUapiRxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiRxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiTxHdcp2p2Manage
    AvRet DeclareUapi(Gsv2k11, AvUapiTxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiTxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCheckLogicAudioTx
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicAudioTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicAudioTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCheckLogicAudioRx
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicAudioRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicAudioRx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCheckVideoScaler
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckVideoScaler(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckVideoScaler(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCheckVideoColor
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckVideoColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckVideoColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCheckVideoGen
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckVideoGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckVideoGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#ifdef Gsv2k11_AvUapiCheckAudioGen
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckAudioGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckAudioGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#ifdef Gsv2k11_AvUapiCheckClockGen
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckClockGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckClockGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#ifdef Gsv2k11_AvUapiCheckLogicVideoTx
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicVideoTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicVideoTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv2k11_AvUapiCheckLogicVideoRx
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicVideoRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv2k11, AvUapiCheckLogicVideoRx(pio AvPort *port)) {return AvNotSupport;}
#endif

#endif /* GSV2K11 uapi definition end */

#if GSV5K1 /* GSV5K1 uapi definition start */
#ifdef Gsv5k1_AvUapiConnectPort
    AvRet DeclareUapi(Gsv5k1, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiDisconnectPort
    AvRet DeclareUapi(Gsv5k1, AvUapiDisconnectPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiDisconnectPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiInitDevice
    AvRet DeclareUapi(Gsv5k1, AvUapiInitDevice(pin AvDevice *device));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiInitDevice(pin AvDevice *device)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxPortInit
    AvRet DeclareUapi(Gsv5k1, AvUapiRxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxPortInit
    AvRet DeclareUapi(Gsv5k1, AvUapiTxPortInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxPortInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiEnablePort
    AvRet DeclareUapi(Gsv5k1, AvUapiEnablePort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiEnablePort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxEnableFreeRun
    AvRet DeclareUapi(Gsv5k1, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxEnableCore
    AvRet DeclareUapi(Gsv5k1, AvUapiTxEnableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxEnableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxDisableCore
    AvRet DeclareUapi(Gsv5k1, AvUapiTxDisableCore(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxDisableCore(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxMuteTmds
    AvRet DeclareUapi(Gsv5k1, AvUapiTxMuteTmds(pin AvPort *port, bool mute));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxMuteTmds(pin AvPort *port, bool mute)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxGetStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxVideoManage
    AvRet DeclareUapi(Gsv5k1, AvUapiTxVideoManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxVideoManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxAudioManage
    AvRet DeclareUapi(Gsv5k1, AvUapiTxAudioManage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxAudioManage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxVideoManage
    AvRet DeclareUapi(Gsv5k1, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxAudioManage
    AvRet DeclareUapi(Gsv5k1, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiResetPort
    AvRet DeclareUapi(Gsv5k1, AvUapiResetPort(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiResetPort(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetHpdUp
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHpdUp(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHpdUp(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGet5VStatus(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGet5VStatus(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetHpdDown
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHpdDown(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHpdDown(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetAudioPackets
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetAudioPackets(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetAudioPackets(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetAudioPacketStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxClearFlags
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearFlags(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearFlags(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetVideoPacketStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetHdcpStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetPacketContent
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetPacketContent
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetPacketContent(pin AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxEnableInfoFrames
    AvRet DeclareUapi(Gsv5k1, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxEnableInfoFrames(pin AvPort* port, uint16 InfoFrames, uint8 Enable)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetHdmiAcrInfo
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdmiAcrInfo (pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdmiAcrInfo (pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxClearAudioInterrupt
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxClearVideoInterrupt
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxClearHdcpInterrupt
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetAvMute
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetAvMute
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetAvMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetAvMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetHdmiModeSupport
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetHdmiModeSupport
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetHdmiModeSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetHdmiModeSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetFeatureSupport
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetFeatureSupport(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetFeatureSupport(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetVideoLock
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetVideoLock(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetVideoLock(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetVideoTiming
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetVideoTiming
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetVideoTiming(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetVideoTiming(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetHdmiDeepColor
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetHdmiDeepColor
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetHdmiDeepColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetHdmiDeepColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetAudioInternalMute
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetAudioInternalMute
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetAudioInternalMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetAudioInternalMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetAudNValue
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetAudNValue(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetAudNValue(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxGetPacketType
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetPacketType(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxGetPacketType(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxGetHdcpStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetHdcpStatus(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetHdcpStatus(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxReadBksv
    AvRet DeclareUapi(Gsv5k1, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxAddBksv
    AvRet DeclareUapi(Gsv5k1, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxGetBksvTotal
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxSetBlackMute
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetBlackMute(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxSetBlackMute(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxEncryptSink
    AvRet DeclareUapi(Gsv5k1, AvUapiTxEncryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxEncryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxDecryptSink
    AvRet DeclareUapi(Gsv5k1, AvUapiTxDecryptSink(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxDecryptSink(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxGetBksvReady
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetHdcpEnable
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHdcpEnable(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHdcpEnable(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetBksvListReady
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetBksvListReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetBksvListReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetHdcpMode
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHdcpMode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetHdcpMode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiHdcp2p2Mode
    AvRet DeclareUapi(Gsv5k1, AvUapiHdcp2p2Mode(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiHdcp2p2Mode(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxClearBksvReady
    AvRet DeclareUapi(Gsv5k1, AvUapiTxClearBksvReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxClearBksvReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxClearRxidReady
    AvRet DeclareUapi(Gsv5k1, AvUapiTxClearRxidReady(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxClearRxidReady(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxClearHdcpError
    AvRet DeclareUapi(Gsv5k1, AvUapiTxClearHdcpError(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxClearHdcpError(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxCheckBksvExisted
    AvRet DeclareUapi(Gsv5k1, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxGetSinkHdcpCapability
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetSinkHdcpCapability(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxGetSinkHdcpCapability(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCecSendMessage
    AvRet DeclareUapi(Gsv5k1, AvUapiCecSendMessage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCecSendMessage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiGetNackCount
    AvRet DeclareUapi(Gsv5k1, AvUapiGetNackCount(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiGetNackCount(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxCecInit
    AvRet DeclareUapi(Gsv5k1, AvUapiTxCecInit(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxCecInit(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCecRxGetStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiCecRxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCecRxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCecTxGetStatus
    AvRet DeclareUapi(Gsv5k1, AvUapiCecTxGetStatus(AvPort* port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCecTxGetStatus(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxCecSetPhysicalAddr
    AvRet DeclareUapi(Gsv5k1, AvUapiTxCecSetPhysicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxCecSetPhysicalAddr(AvPort* port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxCecSetLogicalAddr
    AvRet DeclareUapi(Gsv5k1, AvUapiTxCecSetLogicalAddr(AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxCecSetLogicalAddr(AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxReadEdid
    AvRet DeclareUapi(Gsv5k1, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxWriteEdid
    AvRet DeclareUapi(Gsv5k1, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxSetSpa
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxReadEdid
    AvRet DeclareUapi(Gsv5k1, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxEnableInternalEdid
    AvRet DeclareUapi(Gsv5k1, AvUapiRxEnableInternalEdid(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxEnableInternalEdid(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxArcEnable
    AvRet DeclareUapi(Gsv5k1, AvUapiTxArcEnable(pio AvPort *port, uint8 value));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxArcEnable(pio AvPort *port, uint8 value)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiRxHdcp2p2Manage
    AvRet DeclareUapi(Gsv5k1, AvUapiRxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiRxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiTxHdcp2p2Manage
    AvRet DeclareUapi(Gsv5k1, AvUapiTxHdcp2p2Manage(pin AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiTxHdcp2p2Manage(pin AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCheckLogicAudioTx
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckLogicAudioTx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckLogicAudioTx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCheckLogicAudioRx
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckLogicAudioRx(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckLogicAudioRx(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCheckVideoScaler
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoScaler(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoScaler(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCheckVideoColor
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoColor(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoColor(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCheckVideoGen
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#ifdef Gsv5k1_AvUapiCheckVideoDsc
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoDsc(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckVideoDsc(pio AvPort *port)) {return AvNotSupport;}
#endif
#if AvEnableCcaFeature
#ifdef Gsv5k1_AvUapiCcaTxGetInfo
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxGetInfo(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxGetInfo(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaRxGetInfo
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxGetInfo(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxGetInfo(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaTxInit
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxInit(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxInit(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaRxInit
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxInit(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxInit(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaTxChainReset
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxChainReset(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxChainReset(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaRxChainReset
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxChainReset(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxChainReset(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaTxCalibManage
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxCalibManage(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxCalibManage(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaRxCalibManage
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxCalibManage(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxCalibManage(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaTxSetWorkMode
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxSetWorkMode(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxSetWorkMode(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaTxEnablePin
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxEnablePin(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaTxEnablePin(pio AvPort *port)) {return AvNotSupport;}
#endif
#ifdef Gsv5k1_AvUapiCcaRxEnablePin
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxEnablePin(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCcaRxEnablePin(pio AvPort *port)) {return AvNotSupport;}
#endif
#endif

#ifdef Gsv5k1_AvUapiCheckAudioGen
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckAudioGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckAudioGen(pio AvPort *port)) {return AvNotSupport;}
#endif

#ifdef Gsv5k1_AvUapiCheckClockGen
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckClockGen(pio AvPort *port));
#else
    AvRet DeclareUapi(Gsv5k1, AvUapiCheckClockGen(pio AvPort *port)) {return AvNotSupport;}
#endif



#endif /* GSV5K1 uapi definition end */

#define MathAbs(a,b)     ((a)>=(b) ? (a-b) : (b-a))
#define MathMin(a,b)     ((a)<=(b) ? (a) : (b))
#define MathMax(a,b)     ((a)>=(b) ? (a) : (b))

/*========================================
 * AVI info frame macros
 *=======================================*/
#define SET_AVIF_BYTE1(Y,A,B,S)         ((Y<<5) | (A<<4) | (B<<2) | S)
#define SET_AVIF_BYTE2(C,M,R)           ((C<<6) | (M<<4) | R)
#define SET_AVIF_BYTE3(ITC,EC,Q,SC)     ((ITC<<7)| (EC<<4) | (Q<<2) | SC)
#define SET_AVIF_BYTE4(VIC)             (VIC)
#define SET_AVIF_BYTE5(PR)              (PR)

#define SET_AVIF_VERSION(Ptr, Ver)      (Ptr[1]=Ver)
#define SET_AVIF_LEN(Ptr, Len)          (Ptr[2]=Len)
#define SET_AVIF_Y(Ptr,Y)               (Ptr[4]=(Ptr[4]&(~0xE0))|(Y<<5))
#define SET_AVIF_PR(Ptr,PR)             (Ptr[8]=(Ptr[8]&(~0x0f))|PR)
#define SET_AVIF_VIC(Ptr,VIC)           (Ptr[7]=VIC)
#define SET_AVIF_C(Ptr,C)               (Ptr[5]=(Ptr[5]&(~0xc0))|(C<<6))
#define SET_AVIF_M(Ptr,M)               (Ptr[5]=(Ptr[5]&(~0x30))|(M<<4))
#define SET_AVIF_R(Ptr,R)               (Ptr[5]=(Ptr[5]&(~0x0f))|R)
#define SET_AVIF_EC(Ptr,EC)             (Ptr[6]=(Ptr[6]&(~0x70))|(EC<<4))
#define SET_AVIF_Q(Ptr,Q)               (Ptr[6]=(Ptr[6]&(~0x0c))|(Q<<2))
#define SET_AVIF_YQ(Ptr,YQ)             (Ptr[8]=(Ptr[8]&(~0xC0))|(YQ<<6))
#define SET_AVIF_A(Ptr,A)               (Ptr[4]=(Ptr[4]&(~0x10))|(A<<4))
#define SET_AVIF_B(Ptr,B)               (Ptr[4]=(Ptr[4]&(~0x0c))|(B<<2))
#define SET_AVIF_SC(Ptr,SC)             (Ptr[4]=(Ptr[4]&(~0x03))|SC)

// add by wf8421 20230129
#define SET_DSC_EN(Ptr,EN)              (Ptr[27]=((Ptr[27]&(~0x08))|(EN<<3)))
#define SET_DSC_RATIO(Ptr,RATIO)        (Ptr[27]=((Ptr[27]&(~0x07))|(RATIO)))
#define SET_DSC_Y(Ptr,Y)                (Ptr[27]=((Ptr[27]&(~0x30))|(Y<<4)))
#define SET_DSC_SRC_Y(Ptr,Y)            (Ptr[24]=((Ptr[24]&(~0xC0))|(Y<<6)))
#define SET_DSC_SRC_HDMI(Ptr,HDMI)      (Ptr[24]=((Ptr[24]&(~0x08))|(HDMI<<3)))
#define SET_DSC_HDCP(Ptr,HDCP)          (Ptr[24]=((Ptr[24]&(~0x03))|(HDCP)))
#define SET_DSC_SRC_CD(Ptr,CD)          (Ptr[23]=CD)
#define SET_DSC_SRC_CS(Ptr,CS)          (Ptr[22]=CS)
#define SET_DSC_VIC(Ptr,VIC)            (Ptr[26]=VIC)
#define SET_DSC_CS(Ptr,CS)              (Ptr[25]=CS)
#define SET_UPSTREAM_HDCP(Ptr,HDCP)     (Ptr[16]=(Ptr[16]&(~0xC0))|(HDCP<<6))

#if (AvGsv2kxVideoCscForValensTx||AvGsv2kxVideoCscForValensRx)
#define SET_CPS_SRC_Y(Ptr,Y)            (Ptr[9]=((Ptr[9]&(~0xC0))|(Y<<6)))
#define SET_CPS_SRC_CD(Ptr,CD)          (Ptr[10]=CD)
#define SET_CPS_SRC_CS(Ptr,CS)          (Ptr[11]=CS)
#endif

#if AvGsv2kxVideoHdcp14ForValensTxProblem
#define SET_CPS_SRC_HDCP(Ptr,tHDCP)     (Ptr[12]=tHDCP)
#endif

#if 0
#define SET_DSC_EN(Ptr,EN)              (Ptr[27]=((Ptr[27]&(~0x08))|(EN<<3)))
#define SET_DSC_RATIO(Ptr,RATIO)        (Ptr[27]=((Ptr[27]&(~0x07))|(RATIO)))
#define SET_DSC_Y(Ptr,Y)                (Ptr[27]=((Ptr[27]&(~0x30))|(Y<<4)))
#define SET_DSC_SRC_Y(Ptr,Y)            (Ptr[24]=((Ptr[24]&(~0xC0))|(Y<<6)))
#define SET_DSC_SRC_HDMI(Ptr,HDMI)      (Ptr[24]=((Ptr[24]&(~0x08))|(HDMI<<3)))
#define SET_DSC_HDCP(Ptr,HDCP)          (Ptr[24]=((Ptr[24]&(~0x03))|(HDCP)))
#define SET_DSC_SRC_CD(Ptr,CD)          (Ptr[23]=CD)
#define SET_DSC_SRC_CS(Ptr,CS)          (Ptr[22]=CS)
#define SET_DSC_VIC(Ptr,VIC)            (Ptr[26]=VIC)
#define SET_DSC_CS(Ptr,CS)              (Ptr[25]=CS)
#define SET_UPSTREAM_HDCP(Ptr,HDCP)     (Ptr[16]=(Ptr[16]&(~0xC0))|(HDCP<<6))
#endif

#define GET_AVIF_VERSION(Ptr)           ( Ptr[1] & 0xff)
#define GET_AVIF_LEN(Ptr)               ( Ptr[2] & 0x1f)
#define GET_AVIF_Y(Ptr)                 ((Ptr[4] & 0xE0) >> 5)
#define GET_AVIF_A(Ptr)                 ((Ptr[4] & 0x10) >> 4)
#define GET_AVIF_B(Ptr)                 ((Ptr[4] & 0x0c) >> 2)
#define GET_AVIF_S(Ptr)                 ( Ptr[4] & 0x03)
#define GET_AVIF_C(Ptr)                 ((Ptr[5] & 0xc0) >> 6)
#define GET_AVIF_M(Ptr)                 ((Ptr[5] & 0x30) >> 4)
#define GET_AVIF_R(Ptr)                 ( Ptr[5] & 0x0f)
#define GET_AVIF_ITC(Ptr)               ((Ptr[6] & 0x80) >> 7)
#define GET_AVIF_EC(Ptr)                ((Ptr[6] & 0x70) >> 4)
#define GET_AVIF_Q(Ptr)                 ((Ptr[6] & 0x0c) >> 2)
#define GET_AVIF_SC(Ptr)                ( Ptr[6] & 0x03)
#define GET_AVIF_VIC(Ptr)               ( Ptr[7] & 0xff)
#define GET_AVIF_PR(Ptr)                ( Ptr[8] & 0x0f)
#define GET_AVIF_YQ(Ptr)                ((Ptr[8] & 0xC0) >> 6)

// add by wf8421 20230129
#define GET_DSC_EN(Ptr)                 ((Ptr[27] & 0x08) >> 3)
#define GET_DSC_RATIO(Ptr)              ( Ptr[27] & 0x07)
#define GET_DSC_Y(Ptr)                  ((Ptr[27] & 0x30) >> 4)
#define GET_DSC_SRC_Y(Ptr)              ((Ptr[24] & 0xC0) >> 6)
#define GET_DSC_SRC_HDMI(Ptr)           ((Ptr[24] & 0x08) >> 3)
#define GET_DSC_HDCP(Ptr)               ((Ptr[24] & 0x03))
#define GET_DSC_SRC_CD(Ptr)             ( Ptr[23] & 0xff)
#define GET_DSC_SRC_CS(Ptr)             ( Ptr[22] & 0xff)
#define GET_DSC_VIC(Ptr)                ( Ptr[26] & 0xff)
#define GET_DSC_CS(Ptr)                 ( Ptr[25] & 0xff)
#define GET_UPSTREAM_HDCP(Ptr)          ((Ptr[16] & 0xc0) >> 6)

#if AvGsv2kxVideoCscForValensRx
#define GET_CPS_SRC_Y(Ptr)              ((Ptr[9] & 0xC0) >> 6)
#define GET_CPS_SRC_CD(Ptr)             ( Ptr[10] & 0xff)
#define GET_CPS_SRC_CS(Ptr)             ( Ptr[11] & 0xff)
#endif

#if AvGsv2kxVideoHdcp14ForValensRxProblem
#define GET_CPS_SRC_HDCP(Ptr)           ( Ptr[12] & 0xff)
#endif
#if 0
#define GET_DSC_EN(Ptr)                 ((Ptr[27] & 0x08) >> 3)
#define GET_DSC_RATIO(Ptr)              ( Ptr[27] & 0x07)
#define GET_DSC_Y(Ptr)                  ((Ptr[27] & 0x30) >> 4)
#define GET_DSC_SRC_Y(Ptr)              ((Ptr[24] & 0xC0) >> 6)
#define GET_DSC_SRC_HDMI(Ptr)           ((Ptr[24] & 0x08) >> 3)
#define GET_DSC_HDCP(Ptr)               ((Ptr[24] & 0x03))
#define GET_DSC_SRC_CD(Ptr)             ( Ptr[23] & 0xff)
#define GET_DSC_SRC_CS(Ptr)             ( Ptr[22] & 0xff)
#define GET_DSC_VIC(Ptr)                ( Ptr[26] & 0xff)
#define GET_DSC_CS(Ptr)                 ( Ptr[25] & 0xff)
#define GET_UPSTREAM_HDCP(Ptr)          ((Ptr[16] & 0xc0) >> 6)
#endif

/*===========================================================
 * Vendor Specific info frame macros
 * 3D and Extended Resolution (4Kx2K) do not happen together
 *===========================================================*/
#define GET_VSIF_HVFRMT(Ptr)            ((Ptr[7] & 0xE0) >> 5)
#define GET_VSIF_3D_STRUCT(Ptr)         ((Ptr[8] & 0xF0) >> 4)
#define GET_VSIF_HDMI_VIC(Ptr)          ( Ptr[8] )
#define GET_VSIF_3D_STATUS(Ptr)         ((Ptr[7] & 0x40) >> 6)

#define GET_VSIF_LEN(Ptr)               ( Ptr[2] & 0x1F)
#define GET_3D_STATUS(Ptr)              ((Ptr[7] & 0x40) >> 6)
#define GET_3D_FORMAT(Ptr)              ((Ptr[8] & 0xF0) >> 4)
#define GET_4K_STATUS(Ptr)              ((Ptr[7] & 0x20) >> 5)
#define GET_4K_FORMAT(Ptr)              ( Ptr[8] )

#define SET_VSIF_LEN(Ptr, Len)          (Ptr[2] = Len)
#define SET_VSIF_CHECKSUM(Ptr, Chk)     (Ptr[3] = Chk)
#define SET_3D_STATUS(Ptr, Sts)         (Ptr[7] = (Ptr[7]&(~0x40))|(Sts<<6))
#define SET_3D_FORMAT(Ptr, Fmt)         (Ptr[8] = (Fmt & 0x0F) << 4)
#define SET_4K_STATUS(Ptr, Sts)         (Ptr[7] = (Ptr[7]&(~0x20))|(Sts<<5))
#define SET_4K_FORMAT(Ptr, Vic)         (Ptr[8] = Vic)
/*========================================
 * Audio info frame macros
 *=======================================*/
#define SET_AUDIF_CC(Ptr,cc)            (Ptr[4]=(Ptr[4]&(~0x07))|(cc&7))
#define SET_AUDIF_CA(Ptr,ca)            (Ptr[7] = ca)

#define GET_AUDIF_VER(Ptr)              (Ptr[1])
#define GET_AUDIF_CC(Ptr)               (Ptr[4]&0x7)
#define GET_AUDIF_CT(Ptr)               ((Ptr[4]>>4)&0xf)
#define GET_AUDIF_SS(Ptr)               (Ptr[5]&0x3)
#define GET_AUDIF_SF(Ptr)               ((Ptr[5]>>2)&0x7)
#define GET_AUDIF_CT_X(Ptr)             (Ptr[6])
#define GET_AUDIF_CA(Ptr)               (Ptr[7])
#define GET_AUDIF_LSV(Ptr)              ((Ptr[8]>>3)&0xf)
#define GET_AUDIF_DM_INH(Ptr)           ((Ptr[8]>>7)&0x1)

/*========================================
 * Channel status macros
 *=======================================*/
#define GET_CHST_PROFF_APP(Ptr)         (Ptr[0]&3)
    /*#define GET_CHST_AUD_SAMP_TYPE(Ptr)     ((Ptr[0]>>1) & 0x01) */
#define GET_CHST_COPYRIGHT(Ptr)         ((Ptr[0]>>2) & 0x01)
#define GET_CHST_EMPHASIS(Ptr)          ((Ptr[0]>>3) & 0x07)
#define GET_CHST_MODE(Ptr)              ((Ptr[0]>>6) & 0x03)
#define GET_CHST_CATG_CODE(Ptr)         (Ptr[1])
#define GET_CHST_SRC_NUM(Ptr)           (Ptr[2] & 0x0f)
#define GET_CHST_CH_NUM(Ptr)            ((Ptr[2]>>4) & 0x0f)
#define GET_CHST_SAMP_FREQ(Ptr)         (Ptr[3] & 0x0f)
#define GET_CHST_CLK_ACCUR(Ptr)         ((Ptr[3]>>4) & 0x03)
#define GET_CHST_RESERV0(Ptr)           ((Ptr[3]>>6) & 0x03)
#define GET_CHST_MAX_WORD_LEN(Ptr)      (Ptr[4] & 0x01)
#define GET_CHST_WORD_LEN(Ptr)          ((Ptr[4]>>1) & 0x07)
#define GET_CHST_4BIT_WORD_LEN(Ptr)     (Ptr[4] & 0x0f)

#define GET_N_VALUE(Ptr)                ((((UINT32)(Ptr[0]))<<16) | (((UINT32)(Ptr[1]))<< 8) | ((UINT32)(Ptr[1])))


#ifdef __cplusplus
}
#endif
#endif
