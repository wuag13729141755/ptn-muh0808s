/**
 * @file kernel_fsm.c
 *
 * @brief kernel fsm functions

 *        this file is used to keep the port structure function run following the protocol
 *
 */

#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "kernel_fsm.h"
void AvFsmFunPlugTxFsm(AvPort *port)
{
    TxVars* tx = port->content.tx;
    HdcpTx* hdcp = port->content.hdcptx;
    bool guard1 = false;
    bool guard2 = false;
    bool guard3 = false;
    bool guard4 = false;
    bool guard5 = false;
    bool guard6 = false;
    guard1 = false;
    guard2 = false;
    guard3 = false;
    guard4 = false;
    guard5 = false;
    guard6 = false;
    switch (*port->content.is_PlugTxFsm)
    {
      case AvFsmPlugTxDefault:
        *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
        KfunPtVarInit(port);
        KfunPtResetPort(port);
        KfunTxSetMuteAv(port);
        break;
      case AvFsmPlugTxDefaultEdid:
        *port->content.is_PlugTxFsm = AvFsmPlugTxEdidManage;
        break;
      case AvFsmPlugTxEdid:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
            KfunPtVarInit(port);
            KfunPtResetPort(port);
            KfunTxSetMuteAv(port);
        }
        else if (port->content.tx->EdidReadSuccess == AV_EDID_FAIL)
        {
            if (tx->IgnoreEdidError == 0)
            {
                *port->content.is_PlugTxFsm = AvFsmPlugTxEdidError;
                KfunTxEdidError(port);
            }
            else if (tx->IgnoreEdidError == 1)
            {
                guard1 = true;
            }
            else
            {
                guard5 = true;
            }
        }
        else
        {
            guard5 = true;
        }
        break;
      case AvFsmPlugTxEdidError:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
            KfunPtVarInit(port);
            KfunPtResetPort(port);
            KfunTxSetMuteAv(port);
        }
        break;
      case AvFsmPlugTxEdidManage:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            KfunTxSinkLost(port);
            *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
            KfunPtVarInit(port);
            KfunPtResetPort(port);
            KfunTxSetMuteAv(port);
        }
        else if ((tx->Lock.PllLock == 1) && (tx->EdidReadSuccess ==
                  AV_EDID_UPDATED))
        {
            *port->content.is_PlugTxFsm = AvFsmPlugTxTransmitVideo;
        }
        else
        {
            KfunCecUpdateFromEdid(port);
        }
        break;
      case AvFsmPlugTxEnableTxCore:
        *port->content.is_PlugTxFsm = AvFsmPlugTxEdid;
        break;
      case AvFsmPlugTxHdcp:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            if (tx->Hpd == AV_HPD_FORCE_LOW)
            {
                tx->Hpd = AV_HPD_HIGH;
                guard2 = true;
            }
            else
            {
                KfunTxSinkLost(port);
                *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
                KfunPtVarInit(port);
                KfunPtResetPort(port);
                KfunTxSetMuteAv(port);
            }
        }
        else if ((hdcp->Authenticated != 0U) || (hdcp->Hdcp2p2TxRunning != 0U))
        {
            *port->content.is_PlugTxFsm = AvFsmPlugTxStable;
            KfunTxClearMuteAv(port);
        }
        else if (tx->Lock.PllLock == 0)
        {
            guard2 = true;
        }
        else
        {
            KfunTxHdcpManage(port);
        }
        break;
      case AvFsmPlugTxHpdAntiDither:
        if ((bool)tx->HpdDelayExpire)
        {
            *port->content.is_PlugTxFsm = AvFsmPlugTxEnableTxCore;
            KfunPtEnableTxCore(port);
        }
        else
        {
            if (tx->Hpd != AV_HPD_HIGH)
            {
                *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
                KfunPtVarInit(port);
                KfunPtResetPort(port);
                KfunTxSetMuteAv(port);
            }
        }
        break;
      case AvFsmPlugTxReset:
        if (tx->Hpd == AV_HPD_HIGH)
        {
            *port->content.is_PlugTxFsm = AvFsmPlugTxHpdAntiDither;
            KfunPtAntiDither(port);
        }
        else
        {
            tx->Hpd = AV_HPD_LOW;
        }
        break;
      case AvFsmPlugTxStable:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            if (tx->Hpd == AV_HPD_FORCE_LOW)
            {
                tx->Hpd = AV_HPD_HIGH;
                guard3 = true;
            }
            else
            {
                KfunTxSinkLost(port);
                *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
                KfunPtVarInit(port);
                KfunPtResetPort(port);
                KfunTxSetMuteAv(port);
            }
        }
        else if (tx->Lock.PllLock == 0)
        {
            guard3 = true;
        }
        else
        {
            KfunTxVideoManage(port);
            KfunTxAudioManage(port);
            KfunTxHdcpManage(port);
        }
        break;
      case AvFsmPlugTxTransmitVideo:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            if (tx->Hpd == AV_HPD_FORCE_LOW)
            {
                tx->Hpd = AV_HPD_HIGH;
                guard4 = true;
            }
            else
            {
                KfunTxSinkLost(port);
                *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
                KfunPtVarInit(port);
                KfunPtResetPort(port);
                KfunTxSetMuteAv(port);
            }
        }
        else if (tx->InfoReady >= TxVideoManageThreshold)
        {
            if (hdcp->HdcpEnabled == 1)
            {
                *port->content.is_PlugTxFsm = AvFsmPlugTxHdcp;
            }
            else if (hdcp->HdcpEnabled == 0)
            {
                *port->content.is_PlugTxFsm = AvFsmPlugTxStable;
                KfunTxClearMuteAv(port);
            }
            else
            {
                guard6 = true;
            }
        }
        else
        {
            guard6 = true;
        }
        break;
      default:
        if ((bool)tx->Lock.PllLock)
        {
            KfunPtEnableTxCore(port);
            *port->content.is_PlugTxFsm = AvFsmPlugTxTransmitVideo;
        }
        else
        {
            if (tx->Hpd != AV_HPD_HIGH)
            {
                if (tx->Hpd == AV_HPD_FORCE_LOW)
                {
                    tx->Hpd = AV_HPD_HIGH;
                    *port->content.is_PlugTxFsm =
                        AvFsmPlugTxVideoUnlocked;
                    KfunTxUnlockProtection(port);
                }
                else
                {
                    KfunTxSinkLost(port);
                    *port->content.is_PlugTxFsm = AvFsmPlugTxReset;
                    KfunPtVarInit(port);
                    KfunPtResetPort(port);
                    KfunTxSetMuteAv(port);
                }
            }
        }
        break;
    }
    if (guard6)
    {
        if (tx->Lock.PllLock == 0)
        {
            guard4 = true;
        }
        else
        {
            KfunTxVideoManage(port);
            KfunTxAudioManage(port);
        }
    }
    if (guard5)
    {
        if (tx->EdidReadSuccess != AV_EDID_RESET)
        {
            if (tx->IgnoreEdidError == 1)
            {
                guard1 = true;
            }
            else
            {
                if (tx->IgnoreEdidError == 0)
                {
                    *port->content.is_PlugTxFsm =
                        AvFsmPlugTxEdidManage;
                }
            }
        }
    }
    if (guard4)
    {
        *port->content.is_PlugTxFsm = AvFsmPlugTxVideoUnlocked;
        KfunTxUnlockProtection(port);
    }
    if (guard3)
    {
        *port->content.is_PlugTxFsm = AvFsmPlugTxVideoUnlocked;
        KfunTxUnlockProtection(port);
    }
    if (guard2)
    {
        *port->content.is_PlugTxFsm = AvFsmPlugTxVideoUnlocked;
        KfunTxUnlockProtection(port);
    }
    if (guard1)
    {
        *port->content.is_PlugTxFsm = AvFsmPlugTxDefaultEdid;
        KfunTxUseDefaultEdid(port);
    }
}
void AvFsmFunPlugRxFsm(AvPort *port)
{
    RxVars* rx = port->content.rx;
    bool guard1 = false;
    bool guard2 = false;
    guard1 = false;
    guard2 = false;
    switch (*port->content.is_PlugRxFsm)
    {
      case AvFsmPlugRxDefault:
        *port->content.is_PlugRxFsm = AvFsmPlugRxReset;
        KfunPrVarInit(port);
        KfunPrResetPort(port);
        KfunPrHpdDown(port);
        break;
      case AvFsmPlugRxDetect:
        if (!(rx->Input5V != 0U))
        {
            guard2 = true;
        }
        else if ((bool)rx->Reset)
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxReset;
            KfunPrVarInit(port);
            KfunPrResetPort(port);
            KfunPrHpdDown(port);
        }
        else if (rx->EdidStatus != AV_EDID_UPDATED)
        {
            guard2 = true;
        }
        else if ((rx->Hpd == AV_HPD_HIGH) && (rx->Lock.PllLock != 0U))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxPlugged;
        }
        else
        {
            KfunManageHpa(port);
        }
        break;
      case AvFsmPlugRxInfoUpdate:
        if (!(rx->Lock.PllLock != 0U))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxDetect;
            KfunManageHpa(port);
        }
        else
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxStable;
            rx->IsFreeRun = 1;
        }
        break;
      case AvFsmPlugRxInputLock:
        if ((bool)rx->Reset)
        {
            KfunRxRoutingClearEndInfo(port);
            *port->content.is_PlugRxFsm = AvFsmPlugRxReset;
            KfunPrVarInit(port);
            KfunPrResetPort(port);
            KfunPrHpdDown(port);
        }
        else if (!(rx->Lock.PllLock != 0U))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxDetect;
            KfunManageHpa(port);
        }
        else if (rx->IsInputStable == 0)
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxPlugged;
        }
        else if ((rx->EdidStatus != AV_EDID_UPDATED) || (rx->Hpd != AV_HPD_HIGH))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxPullDownHpd;
            KfunPrHpdDown(port);
        }
        else if (rx->EdidStatus == AV_EDID_UPDATED)
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxReadTiming;
            KfunStdiReadStatus(port);
        }
        else
        {
        }
        break;
      case AvFsmPlugRxPlugged:
        if (!(rx->Lock.PllLock != 0U))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxDetect;
            KfunManageHpa(port);
        }
        else if ((rx->EdidStatus != AV_EDID_UPDATED) || (rx->Hpd != AV_HPD_HIGH))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxPullDownHpd;
            KfunPrHpdDown(port);
        }
        else
        {
            if (rx->IsInputStable == 1)
            {
                *port->content.is_PlugRxFsm = AvFsmPlugRxInputLock;
            }
        }
        break;
      case AvFsmPlugRxPullDownHpd:
        if ((rx->Input5V != 0U) && (rx->EdidStatus == AV_EDID_UPDATED))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxDetect;
            KfunManageHpa(port);
        }
        else
        {
            KfunPrepareEdid(port);
        }
        break;
      case AvFsmPlugRxReadTiming:
        if (!(rx->Lock.PllLock != 0U))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxDetect;
            KfunManageHpa(port);
        }
        else
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxInfoUpdate;
            KfunRxReadInfo(port);
        }
        break;
      case AvFsmPlugRxReset:
        if (!(rx->Reset != 0U))
        {
            *port->content.is_PlugRxFsm = AvFsmPlugRxPullDownHpd;
            KfunPrHpdDown(port);
        }
        break;
      default:
        if ((bool)rx->Reset)
        {
            KfunRxRoutingClearEndInfo(port);
            rx->IsFreeRun = 0;
            *port->content.is_PlugRxFsm = AvFsmPlugRxReset;
            KfunPrVarInit(port);
            KfunPrResetPort(port);
            KfunPrHpdDown(port);
        }
        else if ((rx->EdidStatus != AV_EDID_UPDATED) || (rx->Input5V == 0))
        {
            KfunRxRoutingClearEndInfo(port);
            KfunPrResetPort(port);
            rx->IsFreeRun = 0;
            *port->content.is_PlugRxFsm = AvFsmPlugRxPullDownHpd;
            KfunPrHpdDown(port);
        }
        else if ((rx->Hpd == AV_HPD_LOW) || (rx->Hpd == AV_HPD_FORCE_LOW) ||
                 (rx->Hpd == AV_HPD_TOGGLE))
        {
            KfunRxRoutingClearEndInfo(port);
            guard1 = true;
        }
        else
        {
            if (!(rx->Lock.PllLock != 0U))
            {
                guard1 = true;
            }
        }
        break;
    }
    if (guard2)
    {
        *port->content.is_PlugRxFsm = AvFsmPlugRxPullDownHpd;
        KfunPrHpdDown(port);
    }
    if (guard1)
    {
        rx->IsFreeRun = 0;
        *port->content.is_PlugRxFsm = AvFsmPlugRxDetect;
        KfunManageHpa(port);
    }
}
void AvFsmFunHdcpFsm(AvPort *port)
{
    RxVars* rx = port->content.rx;
    HdcpRx* hdcp = port->content.hdcp;
    switch (*port->content.is_HdcpFsm)
    {
      case AvFsmHdcp2p2State:
        if ((hdcp->Hdcp2p2Flag == 0) || (rx->IsInputStable == 0))
        {
            *port->content.is_HdcpFsm = AvFsmHdcpDisable;
            KfunHdcpReset(port);
            KfunHdcpMuteAv(port);
            KfunDecryptSink(port);
        }
        else
        {
            KfunHdcp2p2Manage(port);
        }
        break;
      case AvFsmHdcpDefault:
        if ((bool)hdcp->HdcpNeeded)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpDisable;
            KfunHdcpReset(port);
            KfunHdcpMuteAv(port);
            KfunDecryptSink(port);
        }
        break;
      case AvFsmHdcpDisable:
        if (!(hdcp->HdcpNeeded != 0U))
        {
            *port->content.is_HdcpFsm = AvFsmHdcpDefault;
            KfunHdcpVarInit(port);
        }
        else if ((bool)rx->IsInputStable)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpUpstreamConnected;
        }
        else
        {
            if ((bool)hdcp->Hdcp2p2Flag)
            {
                *port->content.is_HdcpFsm = AvFsmHdcp2p2State;
                KfunEncryptSink(port);
            }
        }
        break;
      case AvFsmHdcpFail:
        *port->content.is_HdcpFsm = AvFsmHdcpDisable;
        KfunHdcpReset(port);
        KfunHdcpMuteAv(port);
        KfunDecryptSink(port);
        break;
      case AvFsmHdcpReAuthentication:
        if (rx->Input5V == 0)
        {
            KfunEncryptSink(port);
            *port->content.is_HdcpFsm = AvFsmHdcpFail;
        }
        else
        {
            if ((hdcp->AksvInterrupt != 0U) && (rx->IsInputStable != 0U))
            {
                KfunEncryptSink(port);
                *port->content.is_HdcpFsm = AvFsmHdcpRepeaterMode;
                KfunHdcpUnMuteAv(port);
            }
        }
        break;
      case AvFsmHdcpReadBksv:
        if (rx->IsInputStable == 0)
        {
            KfunUploadSinkInfo(port);
            *port->content.is_HdcpFsm = AvFsmHdcpDisable;
            KfunHdcpReset(port);
            KfunHdcpMuteAv(port);
            KfunDecryptSink(port);
        }
        else if (hdcp->HdcpError != 0)
        {
            KfunUploadSinkInfo(port);
            *port->content.is_HdcpFsm = AvFsmHdcpFail;
        }
        else if (hdcp->SinkNumber == hdcp->SinkTotal)
        {
            KfunUploadSinkInfo(port);
            *port->content.is_HdcpFsm = AvFsmHdcpRepeaterReset;
            KfunPrHpdDown(port);
        }
        else
        {
            KfunReadBksv(port);
            KfunDecryptNewSink(port);
        }
        break;
      case AvFsmHdcpReceiverMode:
        if (hdcp->SinkTotal > 0)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpRequestSinkHdcp;
            KfunEncryptSink(port);
        }
        else
        {
            if (rx->IsInputStable == 0)
            {
                *port->content.is_HdcpFsm = AvFsmHdcpDisable;
                KfunHdcpReset(port);
                KfunHdcpMuteAv(port);
                KfunDecryptSink(port);
            }
        }
        break;
      case AvFsmHdcpRepeaterMode:
        if (hdcp->HdcpError != 0)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpFail;
        }
        else if ((rx->IsInputStable == 0) || (rx->VideoEncrypted == 0))
        {
            *port->content.is_HdcpFsm = AvFsmHdcpDisable;
            KfunHdcpReset(port);
            KfunHdcpMuteAv(port);
            KfunDecryptSink(port);
        }
        else if (hdcp->SinkNumber < hdcp->SinkTotal)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpReadBksv;
            KfunReadBksv(port);
            KfunDecryptNewSink(port);
        }
        else
        {
            KfunCheckEncryption(port);
        }
        break;
      case AvFsmHdcpRepeaterReset:
        if (rx->IsInputStable == 1)
        {
            KfunPrHpdUp(port);
            hdcp->AksvInterrupt = 0;
            *port->content.is_HdcpFsm = AvFsmHdcpReAuthentication;
            KfunHdcpMuteAv(port);
        }
        else
        {
            KfunPrHpdUp(port);
        }
        break;
      case AvFsmHdcpRequestSinkHdcp:
        if (rx->IsInputStable == 0)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpDisable;
            KfunHdcpReset(port);
            KfunHdcpMuteAv(port);
            KfunDecryptSink(port);
        }
        else if (hdcp->HdcpError != 0)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpFail;
        }
        else if (hdcp->SinkNumber < hdcp->SinkTotal)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpReadBksv;
            KfunReadBksv(port);
            KfunDecryptNewSink(port);
        }
        else
        {
        }
        break;
      case AvFsmHdcpStartAuthentication:
        if ((hdcp->SinkNumber == 0) && (hdcp->SinkTotal == 0))
        {
            *port->content.is_HdcpFsm = AvFsmHdcpReceiverMode;
        }
        else
        {
            if (hdcp->SinkTotal > 0)
            {
                *port->content.is_HdcpFsm = AvFsmHdcpRequestSinkHdcp;
                KfunEncryptSink(port);
            }
        }
        break;
      default:
        if (rx->IsInputStable == 0)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpDisable;
            KfunHdcpReset(port);
            KfunHdcpMuteAv(port);
            KfunDecryptSink(port);
        }
        else if ((bool)hdcp->AksvInterrupt)
        {
            *port->content.is_HdcpFsm = AvFsmHdcpStartAuthentication;
        }
        else
        {
            if ((bool)hdcp->Hdcp2p2Flag)
            {
                *port->content.is_HdcpFsm = AvFsmHdcp2p2State;
                KfunEncryptSink(port);
            }
        }
        break;
    }
}
void AvFsmFunReceiverFsm(AvPort *port)
{
    RxVars* rx = port->content.rx;
    bool guard1 = false;
    bool guard2 = false;
    bool guard3 = false;
    guard1 = false;
    guard2 = false;
    guard3 = false;
    switch (*port->content.is_ReceiverFsm)
    {
      case AvFsmRxDefault:
        *port->content.is_ReceiverFsm = AvFsmRxReset;
        KfunRxVarInit(port);
        break;
      case AvFsmRxDetect:
        if (rx->Reset == 1)
        {
            *port->content.is_ReceiverFsm = AvFsmRxReset;
            KfunRxVarInit(port);
        }
        else if (((rx->IsInputStable == 0) && (rx->EnableFreeRun != 0U)) ||
                 (rx->ForceFreeRun != 0U))
        {
            *port->content.is_ReceiverFsm = AvFsmRxFreerun;
            KfunRxEnableFrun(port);
        }
        else
        {
            if (rx->IsInputStable == 1)
            {
                *port->content.is_ReceiverFsm = AvFsmRxReceiving;
                KfunRxClearFlags(port);
            }
        }
        break;
      case AvFsmRxFreerun:
        if (rx->EnableFreeRun == 0)
        {
            KfunRxDisableFrun(port);
            *port->content.is_ReceiverFsm = AvFsmRxDetect;
        }
        else if (rx->Reset == 1)
        {
            KfunRxDisableFrun(port);
            *port->content.is_ReceiverFsm = AvFsmRxReset;
            KfunRxVarInit(port);
        }
        else
        {
            if ((rx->IsInputStable != 0U) && (!(rx->ForceFreeRun != 0U)))
            {
                KfunRxDisableFrun(port);
                *port->content.is_ReceiverFsm = AvFsmRxReceiving;
                KfunRxClearFlags(port);
            }
        }
        break;
      case AvFsmRxReceiving:
        if ((bool)rx->Reset)
        {
            guard1 = true;
        }
        else if ((bool)rx->ForceFreeRun)
        {
            guard2 = true;
        }
        else if (!(rx->IsInputStable != 0U))
        {
            if ((bool)rx->EnableFreeRun)
            {
                guard2 = true;
            }
            else if (!(rx->ForceFreeRun != 0U))
            {
                guard1 = true;
            }
            else
            {
                guard3 = true;
            }
        }
        else
        {
            guard3 = true;
        }
        break;
      default:
        if (rx->Reset == 0)
        {
            *port->content.is_ReceiverFsm = AvFsmRxDetect;
        }
        break;
    }
    if (guard3)
    {
        KfunRxVideoManage(port);
        KfunRxAudioManage(port);
    }
    if (guard2)
    {
        *port->content.is_ReceiverFsm = AvFsmRxFreerun;
        KfunRxEnableFrun(port);
    }
    if (guard1)
    {
        *port->content.is_ReceiverFsm = AvFsmRxReset;
        KfunRxVarInit(port);
    }
}
void AvFsmEnterFunPlugTxFsm(AvPort *port)
{
    *port->content.is_PlugTxFsm = AvFsmPlugTxDefault;
    KfunPtPortInit(port);
}
void AvFsmFunCecFsm(AvPort *port)
{
#if AvEnableCecFeature
    AvCec* cec = port->content.cec;
    TxVars* tx = port->content.tx;
    bool guard1 = false;
    guard1 = false;
    switch (*port->content.is_CecFsm)
    {
      case AvFsmCecAudioControl:
        if (tx->Hpd != AV_HPD_HIGH)
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        else
        {
            *port->content.is_CecFsm = AvFsmCecAudioManage;
        }
        break;
      case AvFsmCecAudioFormat:
        *port->content.is_CecFsm = AvFsmCecAudioManage;
        break;
      case AvFsmCecAudioManage:
        if (cec->CecEnable == 0)
        {
            *port->content.is_CecFsm = AvFsmCecDefault;
        }
        else if (tx->Hpd != AV_HPD_HIGH)
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        else
        {
            if ((cec->EnableAudioAmplifier == AV_CEC_AMP_TO_ENABLE) ||
                    (cec->EnableAudioAmplifier == AV_CEC_AMP_TO_DISABLE))
            {
                *port->content.is_CecFsm = AvFsmCecCmdSystemAudioModetoAll;
            }
            else
            {
                if ((cec->RxGetFlag == 1) && (cec->RxMsg ==
                        AV_CEC_MSG_USER_CONTROL_PRESSED))
                {
                    *port->content.is_CecFsm = AvFsmCecAudioControl;
                }
                else
                {
                    if ((cec->RxGetFlag == 1) && (cec->RxMsg ==
                            AV_CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR))
                    {
                        *port->content.is_CecFsm =
                            AvFsmCecAudioFormat;
                    }
                    else
                    {
                        if ((cec->EnableARC == AV_CEC_ARC_TO_TERMINATE) ||
                                (cec->EnableARC == AV_CEC_ARC_TO_INITIATE) ||
                                ((cec->RxGetFlag == 1) && ((cec->RxMsg ==
                                AV_CEC_MSG_REQUEST_ARC_INITIATION) ||
                                (cec->RxMsg ==
                                 AV_CEC_MSG_REQUEST_ARC_TERMINATION))))
                        {
                            *port->content.is_CecFsm =
                                AvFsmCecCmdAudioARC;
                            cec->ARCTryCount = 0;
                        }
                        else
                        {
                            KfunCecManage(port);
                        }
                    }
                }
            }
        }
        break;
      case AvFsmCecCmdARC:
        if ((tx->Hpd != AV_HPD_HIGH) || (cec->CecEnable == 0))
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        else
        {
            if ((cec->RxGetFlag == 1) && ((cec->RxMsg ==
                    AV_CEC_MSG_REPORT_ARC_INITIATED) || (cec->RxMsg ==
                    AV_CEC_MSG_REPORT_ARC_TERMINATED)))
            {
                *port->content.is_CecFsm = AvFsmCecFunctionalDefault;
            }
            else
            {
                AvHandleEvent(port, AvEventCecArcManage, NULL, NULL);
            }
        }
        break;
      case AvFsmCecCmdActiveSource:
        if ((tx->Hpd != AV_HPD_HIGH) || (cec->CecEnable == 0))
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        else
        {
            if (cec->RxGetFlag == 0)
            {
                *port->content.is_CecFsm = AvFsmCecFunctionalDefault;
            }
        }
        break;
      case AvFsmCecCmdAudioARC:
        if ((cec->EnableAudioAmplifier == AV_CEC_AMP_TO_ENABLE) ||
                (cec->EnableAudioAmplifier == AV_CEC_AMP_TO_DISABLE))
        {
            *port->content.is_CecFsm = AvFsmCecCmdSystemAudioModetoAll;
        }
        else
        {
            if ((cec->RxGetFlag == 1) && ((cec->RxMsg ==
                    AV_CEC_MSG_REPORT_ARC_INITIATED) || (cec->RxMsg ==
                    AV_CEC_MSG_REPORT_ARC_TERMINATED)))
            {
                *port->content.is_CecFsm = AvFsmCecAudioManage;
            }
            else if ((tx->Hpd != AV_HPD_HIGH) || (cec->CecEnable == 0))
            {
                *port->content.is_CecFsm = AvFsmCecNotConnected;
                KfunCecInit(port);
            }
            else
            {
                AvHandleEvent(port, AvEventCecArcManage, NULL, NULL);
            }
        }
        break;
      case AvFsmCecCmdSystemAudioModetoAll:
        if (cec->EnableAudioAmplifier == AV_CEC_AMP_TO_ENABLE)
        {
            cec->EnableAudioAmplifier = AV_CEC_AMP_ENABLED;
            guard1 = true;
        }
        else if (cec->EnableAudioAmplifier == AV_CEC_AMP_TO_DISABLE)
        {
            cec->EnableAudioAmplifier = AV_CEC_AMP_DISABLED;
            guard1 = true;
        }
        else
        {
            if (cec->EnableAudioAmplifier == AV_CEC_AMP_ENABLED)
            {
                guard1 = true;
            }
        }
        break;
      case AvFsmCecDefault:
        if (cec->CecEnable == 1)
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        break;
      case AvFsmCecFunctionalDefault:
        if (cec->CecEnable == 0)
        {
            *port->content.is_CecFsm = AvFsmCecDefault;
        }
        else if (tx->Hpd != AV_HPD_HIGH)
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        else
        {
            if (((cec->RxGetFlag == 1) && (cec->RxMsg ==
                    AV_CEC_MSG_SYSTEM_AUDIO_MODE_REQUEST)) ||
                    ((cec->EnableAudioAmplifier == AV_CEC_AMP_TO_ENABLE) ||
                     (cec->EnableAudioAmplifier == AV_CEC_AMP_TO_DISABLE)))
            {
                *port->content.is_CecFsm = AvFsmCecCmdSystemAudioModetoAll;
            }
            else
            {
                if ((cec->EnableARC == AV_CEC_ARC_TO_TERMINATE) ||
                        (cec->EnableARC == AV_CEC_ARC_TO_INITIATE) ||
                        ((cec->RxGetFlag == 1) && ((cec->RxMsg ==
                        AV_CEC_MSG_REQUEST_ARC_INITIATION) || (cec->RxMsg ==
                        AV_CEC_MSG_REQUEST_ARC_TERMINATION))))
                {
                    *port->content.is_CecFsm = AvFsmCecCmdARC;
                    cec->ARCTryCount = 0;
                }
                else
                {
                    if (cec->EnableAudioAmplifier == AV_CEC_AMP_ENABLED)
                    {
                        AvHandleEvent(port,
                                      AvEventCecSendSetSystemAudioModeToAll,
                                      NULL, NULL);
                        *port->content.is_CecFsm =
                            AvFsmCecAudioManage;
                    }
                    else
                    {
                        KfunCecManage(port);
                    }
                }
            }
        }
        break;
      case AvFsmCecIdle:
        if (cec->CecEnable == 0)
        {
            *port->content.is_CecFsm = AvFsmCecDefault;
        }
        else if (cec->AddrIndex != 15)
        {
            *port->content.is_CecFsm = AvFsmCecReset;
        }
        else if (tx->Hpd != AV_HPD_HIGH)
        {
            *port->content.is_CecFsm = AvFsmCecNotConnected;
            KfunCecInit(port);
        }
        else
        {
            if (tx->PhyAddr != 0)
            {
                *port->content.is_CecFsm = AvFsmCecCmdActiveSource;
                AvHandleEvent(port, AvEventCecSendReportPhysAddress, NULL, NULL);
            }
        }
        break;
      case AvFsmCecNotConnected:
        if (cec->CecEnable == 0)
        {
            cec->TxSendFlag = AV_CEC_TX_IDLE;
            cec->AddrIndex = 0;
            *port->content.is_CecFsm = AvFsmCecDefault;
        }
        else
        {
            if ((tx->Hpd == AV_HPD_HIGH) && (tx->PhyAddr != 0) &&
                    (tx->EdidReadSuccess == AV_EDID_UPDATED))
            {
                cec->TxSendFlag = AV_CEC_TX_IDLE;
                cec->AddrIndex = 0;
                *port->content.is_CecFsm = AvFsmCecReset;
            }
        }
        break;
      case AvFsmCecReset:
        if (cec->CecEnable == 0)
        {
            *port->content.is_CecFsm = AvFsmCecDefault;
        }
        else if (cec->AddrIndex != 15)
        {
            *port->content.is_CecFsm = AvFsmCecTxLogAddr;
            cec->LogicAddrList = 0;
            cec->AddrIndex = 5;
        }
        else if (cec->AddrIndex == 15)
        {
            *port->content.is_CecFsm = AvFsmCecIdle;
            cec->DstAddr = 0x00;
        }
        else
        {
            if (tx->Hpd != AV_HPD_HIGH)
            {
                *port->content.is_CecFsm = AvFsmCecNotConnected;
                KfunCecInit(port);
            }
        }
        break;
      default:
        if ((cec->AddrIndex == 15) && (cec->TxSendFlag == 0))
        {
            *port->content.is_CecFsm = AvFsmCecReset;
        }
        else
        {
            KfunPrepareLogAddr(port);
        }
        break;
    }
    if (guard1)
    {
        AvHandleEvent(port, AvEventCecSendSetSystemAudioModeToAll, NULL, NULL);
        *port->content.is_CecFsm = AvFsmCecAudioManage;
    }
#endif
}
void AvFsmEnterFunPlugRxFsm(AvPort *port)
{
    *port->content.is_PlugRxFsm = AvFsmPlugRxDefault;
}
void AvFsmEnterFunHdcpFsm(AvPort *port)
{
    *port->content.is_HdcpFsm = AvFsmHdcpDefault;
    KfunHdcpVarInit(port);
}
void AvFsmEnterFunReceiverFsm(AvPort *port)
{
    *port->content.is_ReceiverFsm = AvFsmRxDefault;
    KfunRxInit(port);
}
void AvFsmEnterFunCecFsm(AvPort *port)
{
#if AvEnableCecFeature
    *port->content.is_CecFsm = AvFsmCecDefault;
#endif
}
void AvFsmFunCcaTxFsm(AvPort *port)
{
#if AvEnableCcaFeature
    AvCcaTx* ccatx = port->content.ccatx;
    switch (*port->content.is_CcaTxFsm)
    {
      case AvFsmCcaTxCalib:
        if (ccatx->CalibDone == 1)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxSetupChain;
        }
        else
        {
            KfunCcaTxCalibManage(port);
        }
        break;
      case AvFsmCcaTxDefault:
        if (ccatx->Enable == 1)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxInit;
            KfunCcaTxInit(port);
        }
        break;
      case AvFsmCcaTxInit:
        if (ccatx->Enable == 0)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxDefault;
        }
        else
        {
            if (ccatx->Lock == AV_CCA_OUT_LOCK)
            {
                *port->content.is_CcaTxFsm = AvFsmCcaTxCalib;
                KfunCcaTxSetWorkMode(port);
            }
        }
        break;
      case AvFsmCcaTxLinkConnect:
        if (ccatx->Enable == 0)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxDefault;
        }
        else
        {
            if ((ccatx->CalibDone == 0) || (ccatx->LinkAbort == 1))
            {
                *port->content.is_CcaTxFsm = AvFsmCcaTxUnlock;
                KfunCcaTxUnlockProtection(port);
            }
            else
            {
                *port->content.is_CcaTxFsm = AvFsmCcaTxLinkStable;
            }
        }
        break;
      case AvFsmCcaTxLinkStable:
        if (ccatx->Enable == 0)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxDefault;
        }
        else
        {
            if ((ccatx->CalibDone == 0) || (ccatx->LinkAbort == 1))
            {
                *port->content.is_CcaTxFsm = AvFsmCcaTxUnlock;
                KfunCcaTxUnlockProtection(port);
            }
            else
            {
                KfunCcaTxLinkManage(port);
            }
        }
        break;
      case AvFsmCcaTxSetupChain:
        if (ccatx->Enable == 0)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxDefault;
        }
        else
        {
            if (ccatx->CalibDone == 0)
            {
                *port->content.is_CcaTxFsm = AvFsmCcaTxCalib;
                KfunCcaTxSetWorkMode(port);
            }
            else if (ccatx->Lock == AV_CCA_VIDEO_LOCK)
            {
                *port->content.is_CcaTxFsm = AvFsmCcaTxLinkConnect;
            }
            else
            {
                if ((ccatx->CalibDone == 0) || (ccatx->LinkAbort == 1))
                {
                    *port->content.is_CcaTxFsm = AvFsmCcaTxUnlock;
                    KfunCcaTxUnlockProtection(port);
                }
            }
        }
        break;
      default:
        if (ccatx->Enable == 0)
        {
            *port->content.is_CcaTxFsm = AvFsmCcaTxDefault;
        }
        else
        {
            if (ccatx->Lock != AV_CCA_NO_LOCK)
            {
                if (ccatx->CalibDone == 0)
                {
                    *port->content.is_CcaTxFsm = AvFsmCcaTxCalib;
                    KfunCcaTxSetWorkMode(port);
                }
                else
                {
                    if (ccatx->CalibDone == 1)
                    {
                        *port->content.is_CcaTxFsm =
                            AvFsmCcaTxSetupChain;
                    }
                }
            }
        }
        break;
    }
#endif
}
void AvFsmFunCcaRxFsm(AvPort *port)
{
#if AvEnableCcaFeature
    AvCcaRx* ccarx = port->content.ccarx;
    switch (*port->content.is_CcaRxFsm)
    {
      case AvFsmCcaRxCalib:
        if (ccarx->CalibDone == 1)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxSetupChain;
        }
        else
        {
            if (ccarx->Lock == AV_CCA_NO_LOCK)
            {
                *port->content.is_CcaRxFsm = AvFsmCcaRxUnlock;
                KfunCcaRxUnlockProtection(port);
                ccarx->CalibDone = 0;
            }
            else
            {
                KfunCcaRxCalibManage(port);
            }
        }
        break;
      case AvFsmCcaRxDefault:
        if (ccarx->Enable == 1)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxInit;
            KfunCcaRxInit(port);
        }
        break;
      case AvFsmCcaRxInit:
        if (ccarx->Enable == 0)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxDefault;
        }
        else
        {
            if (ccarx->Lock != AV_CCA_NO_LOCK)
            {
                *port->content.is_CcaRxFsm = AvFsmCcaRxCalib;
            }
        }
        break;
      case AvFsmCcaRxLinkConnect:
        if (ccarx->Enable == 0)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxDefault;
        }
        else
        {
            if ((ccarx->Lock == AV_CCA_NO_LOCK) || (ccarx->Lock ==
                    AV_CCA_IN_LOCK) || (ccarx->LinkAbort == 1))
            {
                *port->content.is_CcaRxFsm = AvFsmCcaRxUnlock;
                KfunCcaRxUnlockProtection(port);
                ccarx->CalibDone = 0;
            }
            else
            {
                if (ccarx->Lock == AV_CCA_VIDEO_LOCK)
                {
                    *port->content.is_CcaRxFsm = AvFsmCcaRxLinkStable;
                }
            }
        }
        break;
      case AvFsmCcaRxLinkStable:
        if (ccarx->Enable == 0)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxDefault;
        }
        else
        {
            if ((ccarx->Lock != AV_CCA_VIDEO_LOCK) || (ccarx->LinkAbort == 1))
            {
                *port->content.is_CcaRxFsm = AvFsmCcaRxUnlock;
                KfunCcaRxUnlockProtection(port);
                ccarx->CalibDone = 0;
            }
            else
            {
                KfunCcaRxLinkManage(port);
            }
        }
        break;
      case AvFsmCcaRxSetupChain:
        if (ccarx->Enable == 0)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxDefault;
        }
        else
        {
            if (ccarx->CalibDone == 0)
            {
                *port->content.is_CcaRxFsm = AvFsmCcaRxCalib;
            }
            else if ((ccarx->Lock == AV_CCA_VIDEO_LOCK) || (ccarx->Lock ==
                      AV_CCA_LINK_LOCK))
            {
                *port->content.is_CcaRxFsm = AvFsmCcaRxLinkConnect;
            }
            else
            {
                if ((ccarx->Lock == AV_CCA_NO_LOCK) || (ccarx->LinkAbort == 1))
                {
                    *port->content.is_CcaRxFsm = AvFsmCcaRxUnlock;
                    KfunCcaRxUnlockProtection(port);
                    ccarx->CalibDone = 0;
                }
            }
        }
        break;
      default:
        if (ccarx->Enable == 0)
        {
            *port->content.is_CcaRxFsm = AvFsmCcaRxDefault;
        }
        else
        {
            if (ccarx->Lock != AV_CCA_NO_LOCK)
            {
                if (ccarx->CalibDone == 0)
                {
                    *port->content.is_CcaRxFsm = AvFsmCcaRxCalib;
                }
                else
                {
                    if (ccarx->CalibDone == 1)
                    {
                        *port->content.is_CcaRxFsm =
                            AvFsmCcaRxSetupChain;
                    }
                }
            }
        }
        break;
    }
#endif
}
void AvFsmEnterFunCcaTxFsm(AvPort *port)
{
#if AvEnableCcaFeature
    *port->content.is_CcaTxFsm = AvFsmCcaTxDefault;
#endif
}
void AvFsmEnterFunCcaRxFsm(AvPort *port)
{
#if AvEnableCcaFeature
    *port->content.is_CcaRxFsm = AvFsmCcaRxDefault;
#endif
}

#endif

