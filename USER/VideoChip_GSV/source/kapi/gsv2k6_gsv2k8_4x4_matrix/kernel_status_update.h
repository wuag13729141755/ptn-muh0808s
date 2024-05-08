/**
 * @file kernel_fsm_update.h
 *
 * @brief functions for update FSMs in the kernel layer
 */

#ifndef __kernel_fsm_update_h
#define __kernel_fsm_update_h

#include "av_config.h"
#include "kernel_fsm.h"
#include "uapi_function_mapper.h"
/**
 * @brief
 * Check status for Rx FSM
 * @return void
 * @note
 */
void KfunCheckRxState(pin AvPort *port);

#if AvEnableSimplifyHdcp
/**
 * @brief
 * Manage Simple Hdcp
 * @return void
 * @note
 */
void KfunSimpleHdcpManage(AvPort *port);
#define KfunCheckHdcpState KfunSimpleHdcpManage
#else
/**
 * @brief
 * Check status for Hdcp FSM
 * @return void
 * @note
 */
void KfunCheckRxHdcpState(pin AvPort *port);
#define KfunCheckHdcpState KfunCheckRxHdcpState
#endif

/**
 * @brief
 * Check status for Tx FSM
 * @return void
 * @note
 */
void KfunCheckTxState(pout AvPort *port);

/**
 * @brief
 * Check status for Tx Routing FSM
 * @return void
 * @note
 */
void KfunCheckTxRoutingState(pout AvPort *port);

/**
 * @brief
 * Check status for Rx Routing FSM
 * @return void
 * @note
 */
void KfunCheckRxRoutingState(pin AvPort *port);

/**
 * @brief
 * Check status for Pr FSM
 * @return void
 * @note
 */
void KfunCheckPrState(pin AvPort *port);

/**
 * @brief
 * Check status for Pt FSM
 * @return void
 * @note
 */
void KfunCheckPtState(pout AvPort *port);

/**
 * @brief
 * Polling status for Color space FSM
 * @return void
 * @note
 */
void KfunTxcsPollingCs(pout AvPort *port);

/**
 * @brief
 * Polling status for Cec FSM
 * @return void
 * @note
 */
void KfunCheckCecState(pout AvPort *port);

/**
 * @brief
 * Initialization of Pr FSM variables
 * @return void
 * @note
 */
void KfunPrVarInit(pin AvPort *port);

/**
 * @brief
 * Initialization of Pt FSM variables
 * @return void
 * @note
 */
void KfunPtVarInit(pout AvPort *port);

/**
 * @brief
 * Initialization of Rx FSM variables
 * @return void
 * @note
 */
void KfunRxVarInit(pin AvPort *port);

/**
 * @brief
 * Initialization of Tx FSM variables
 * @return void
 * @note
 */
void KfunTxVarInit(pout AvPort *port);

/**
 * @brief
 * Initialization of HDCP FSM variables
 * @return void
 * @note
 */
void KfunHdcpVarInit(pin AvPort *port);

/**
 * @brief
 * Initialization of Routing FSM variables
 * @return void
 * @note
 */
void KfunRoutingVarInit(pout AvPort *port);

/**
 * @brief
 * Check RxStat from Routed Rx port's FSM, update routed TX Port
 * @return AvOk - success
 * @note
 */
AvRet KfunRxStatToTx(pin AvPort *RxPort, pout AvPort *TxPort);

/**
 * @brief
 * Lookup port ptr from Tx port structure
 * @return AvOk - success
 * @note
 */
AvPort *KfunLookupRxFromTx(pout AvPort *port);

/**
 * @brief
 * configure default EDID to Rx Port
 * @return AvOk - success
 * @note
 */
AvRet KfunDefaultEdid(pin AvPort *port);

/**
 * @brief
 * Check Audio Logic Out TTL configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckLogicVideoTx(AvPort *port);

/**
 * @brief
 * Check Audio Logic In TTL configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckLogicVideoRx(AvPort *port);

/**
 * @brief
 * Check Audio Logic Out TTL configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckLogicAudioTx(AvPort *port);

/**
 * @brief
 * Check Audio Logic In TTL configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckLogicAudioRx(AvPort *port);

/**
 * @brief
 * Check Video DSC configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckVideoDsc(AvPort *port);

/**
 * @brief
 * Check Video Scaler configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckVideoScaler(AvPort *port);

/**
 * @brief
 * Check Video Color configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckVideoColor(AvPort *port);

/**
 * @brief
 * Check Video Gen configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckVideoGen(AvPort *port);

#if AvEnableCcaFeature
void KfunCheckCcaRxState(AvPort *port);
void KfunCheckCcaTxState(AvPort *port);
#endif

/**
 * @brief
 * Check Audio Gen configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckAudioGen(AvPort *port);

/**
 * @brief
 * Check Clock Gen configuration
 * @return AvOk - success
 * @note
 */
void KfunCheckClockGen(AvPort *port);

#ifdef __cplusplus
    }
#endif
#endif
