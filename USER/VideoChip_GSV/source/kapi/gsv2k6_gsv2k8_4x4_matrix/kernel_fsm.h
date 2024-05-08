/**
 * @file kernel_fsm.h
 *
 * @brief kernel fsm functions

 *        this file is used to keep the port structure function run following the protocol
 *
 */
#ifndef __kernel_fsm_h
#define __kernel_fsm_h

/* user code (top of header file) */
#include "Gsv_kapi.h"
#include "av_event_handler.h"
#include "uapi_function_mapper.h"
#include "kernel_status_update.h"

#define AvFsmCecCmdSystemAudioModetoAll ((uint8)7U)
#define AvFsmHdcpStartAuthentication ((uint8)11U)
#define AvFsmHdcpUpstreamConnected ((uint8)12U)
#define AvFsmCecFunctionalDefault ((uint8)9U)
#define AvFsmHdcpReAuthentication ((uint8)5U)
#define AvFsmHdcpRequestSinkHdcp ((uint8)10U)
#define AvFsmPlugTxHpdAntiDither ((uint8)8U)
#define AvFsmPlugTxTransmitVideo ((uint8)11U)
#define AvFsmPlugTxVideoUnlocked ((uint8)12U)
#define AvFsmCecCmdActiveSource ((uint8)5U)
#define AvFsmPlugTxEnableTxCore ((uint8)6U)
#define AvFsmHdcpRepeaterReset ((uint8)9U)
#define AvFsmPlugRxPullDownHpd ((uint8)6U)
#define AvFsmPlugTxDefaultEdid ((uint8)2U)
#define AvFsmCcaRxCalib      ((uint8)1U)
#define AvFsmCcaRxDefault    ((uint8)2U)
#define AvFsmCcaRxInit       ((uint8)3U)
#define AvFsmCcaRxLinkConnect ((uint8)4U)
#define AvFsmCcaRxLinkStable ((uint8)5U)
#define AvFsmCcaRxSetupChain ((uint8)6U)
#define AvFsmCcaRxUnlock     ((uint8)7U)
#define AvFsmCcaTxCalib      ((uint8)1U)
#define AvFsmCcaTxDefault    ((uint8)2U)
#define AvFsmCcaTxInit       ((uint8)3U)
#define AvFsmCcaTxLinkConnect ((uint8)4U)
#define AvFsmCcaTxLinkStable ((uint8)5U)
#define AvFsmCcaTxSetupChain ((uint8)6U)
#define AvFsmCcaTxUnlock     ((uint8)7U)
#define AvFsmCecAudioControl ((uint8)1U)
#define AvFsmCecAudioFormat  ((uint8)2U)
#define AvFsmCecAudioManage  ((uint8)3U)
#define AvFsmCecCmdARC       ((uint8)4U)
#define AvFsmCecCmdAudioARC  ((uint8)6U)
#define AvFsmCecDefault      ((uint8)8U)
#define AvFsmCecIdle         ((uint8)10U)
#define AvFsmCecNotConnected ((uint8)11U)
#define AvFsmCecReset        ((uint8)12U)
#define AvFsmCecTxLogAddr    ((uint8)13U)
#define AvFsmHdcp2p2State    ((uint8)1U)
#define AvFsmHdcpDefault     ((uint8)2U)
#define AvFsmHdcpDisable     ((uint8)3U)
#define AvFsmHdcpFail        ((uint8)4U)
#define AvFsmHdcpReadBksv    ((uint8)6U)
#define AvFsmHdcpReceiverMode ((uint8)7U)
#define AvFsmHdcpRepeaterMode ((uint8)8U)
#define AvFsmNoActiveChild ((uint8)0U)
#define AvFsmPlugRxDefault   ((uint8)1U)
#define AvFsmPlugRxDetect    ((uint8)2U)
#define AvFsmPlugRxInfoUpdate ((uint8)3U)
#define AvFsmPlugRxInputLock ((uint8)4U)
#define AvFsmPlugRxPlugged   ((uint8)5U)
#define AvFsmPlugRxReadTiming ((uint8)7U)
#define AvFsmPlugRxReset     ((uint8)8U)
#define AvFsmPlugRxStable    ((uint8)9U)
#define AvFsmPlugTxDefault   ((uint8)1U)
#define AvFsmPlugTxEdid      ((uint8)3U)
#define AvFsmPlugTxEdidError ((uint8)4U)
#define AvFsmPlugTxEdidManage ((uint8)5U)
#define AvFsmPlugTxHdcp      ((uint8)7U)
#define AvFsmPlugTxReset     ((uint8)9U)
#define AvFsmPlugTxStable    ((uint8)10U)
#define AvFsmRxDefault       ((uint8)1U)
#define AvFsmRxDetect        ((uint8)2U)
#define AvFsmRxFreerun       ((uint8)3U)
#define AvFsmRxReceiving     ((uint8)4U)
#define AvFsmRxReset         ((uint8)5U)
void AvFsmFunPlugTxFsm(AvPort *port);
void AvFsmFunPlugRxFsm(AvPort *port);
void AvFsmFunHdcpFsm(AvPort *port);
void AvFsmFunReceiverFsm(AvPort *port);
void AvFsmEnterFunPlugTxFsm(AvPort *port);
void AvFsmFunCecFsm(AvPort *port);
void AvFsmEnterFunPlugRxFsm(AvPort *port);
void AvFsmEnterFunHdcpFsm(AvPort *port);
void AvFsmEnterFunReceiverFsm(AvPort *port);
void AvFsmEnterFunCecFsm(AvPort *port);
void AvFsmFunCcaTxFsm(AvPort *port);
void AvFsmFunCcaRxFsm(AvPort *port);
void AvFsmEnterFunCcaTxFsm(AvPort *port);
void AvFsmEnterFunCcaRxFsm(AvPort *port);

#ifdef __cplusplus
    }
#endif
#endif
