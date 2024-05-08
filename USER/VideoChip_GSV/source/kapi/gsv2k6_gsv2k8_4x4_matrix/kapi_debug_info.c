#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "kernel_fsm.h"
#include "Gsv_kapi.h"
#include "Gsv_uapi.h"

void PrintHdcpFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_HdcpFsm != OldState)
    {
        switch(*Port->content.is_HdcpFsm)
        {
           case AvFsmHdcpDefault:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpDefault",Port->device->index,Port->index);
            break;

           case AvFsmHdcpDisable:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpDisable",Port->device->index,Port->index);
            break;

           case AvFsmHdcpFail:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpFail",Port->device->index,Port->index);
            break;

           case AvFsmHdcp2p2State:
            AvUapiOutputDebugFsm("Dev%d,Port%d: Hdcp2p2State",Port->device->index,Port->index);
            break;

           case AvFsmHdcpReAuthentication:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpReAuthentication",Port->device->index,Port->index);
            break;

           case AvFsmHdcpReadBksv:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpReadBksv",Port->device->index,Port->index);
            break;

           case AvFsmHdcpReceiverMode:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpReceiverMode",Port->device->index,Port->index);
            break;

           case AvFsmHdcpRepeaterMode:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpRepeaterMode",Port->device->index,Port->index);
            break;

           case AvFsmHdcpRepeaterReset:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpRepeaterReset",Port->device->index,Port->index);
            break;

           case AvFsmHdcpRequestSinkHdcp:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpRequestSinkHdcp",Port->device->index,Port->index);
            break;

           case AvFsmHdcpStartAuthentication:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpStartAuthentication",Port->device->index,Port->index);
            break;

           case AvFsmHdcpUpstreamConnected:
            AvUapiOutputDebugFsm("Dev%d,Port%d: HdcpUpstreamConnected",Port->device->index,Port->index);
            break;
        }
    }
}

void PrintPlugTxFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_PlugTxFsm != OldState)
    {
        switch (*Port->content.is_PlugTxFsm)
        {
          case AvFsmPlugTxEdidError:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxEdidError", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxDefault:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxDefault", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxDefaultEdid:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxDefaultEdid", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxEdid:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxEdid", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxEdidManage:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxEdidManage", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxEnableTxCore:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxEnableTxCore", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxHdcp:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxHdcp", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxHpdAntiDither:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxHpdAntiDither", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxReset:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxReset", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxStable:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxStable", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxTransmitVideo:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxTransmitVideo", Port->device->index,Port->index);
            break;
          case AvFsmPlugTxVideoUnlocked:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugTxVideoUnlocked", Port->device->index,Port->index);
            break;
        }
    }
}

void PrintPlugRxFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_PlugRxFsm != OldState)
    {
        switch (*Port->content.is_PlugRxFsm)
        {
          case AvFsmPlugRxDefault:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxDefault", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxDetect:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxDetect", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxInfoUpdate:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxInfoUpdate", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxInputLock:
              AvHandleEvent(Port, AvEventPortUpStreamConnected, NULL, NULL);
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxInputLock", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxPlugged:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxPlugged", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxPullDownHpd:
              if(Port->content.rx->Input5V == 0)
                  AvHandleEvent(Port, AvEventPortUpStreamDisconnected, NULL, NULL);
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxPullDownHpd", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxReadTiming:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxReadTiming", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxReset:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxReset", Port->device->index,Port->index);
            break;
          case AvFsmPlugRxStable:
              AvUapiOutputDebugFsm("Dev%d,Port%d: PlugRxStable", Port->device->index,Port->index);
            break;
        }
    }
}

void PrintReceiverFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_ReceiverFsm != OldState)
    {
        switch (*Port->content.is_ReceiverFsm)
        {
          case AvFsmRxDefault:
            AvUapiOutputDebugFsm("Dev%d,Port%d: RxDefault", Port->device->index,Port->index);
            break;
          case AvFsmRxDetect:
            AvUapiOutputDebugFsm("Dev%d,Port%d: RxDetect", Port->device->index,Port->index);
            break;
          case AvFsmRxFreerun:
            AvUapiOutputDebugFsm("Dev%d,Port%d: RxFreerun", Port->device->index,Port->index);
            break;
          case AvFsmRxReceiving:
            AvUapiOutputDebugFsm("Dev%d,Port%d: RxReceiving", Port->device->index,Port->index);
            break;
          case AvFsmRxReset:
            AvUapiOutputDebugFsm("Dev%d,Port%d: RxReset", Port->device->index,Port->index);
            break;
        }
    }
}

#if AvEnableCecFeature /* CEC Related */
void PrintCecFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_CecFsm != OldState)
    {
        switch (*Port->content.is_CecFsm)
        {
          case AvFsmCecDefault:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecDefault", Port->device->index,Port->index);
            break;
          case AvFsmCecIdle:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecIdle", Port->device->index,Port->index);
            break;
          case AvFsmCecNotConnected:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecNotConnected", Port->device->index,Port->index);
            break;
          case AvFsmCecReset:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecReset", Port->device->index,Port->index);
            break;
          case AvFsmCecTxLogAddr:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecTxLogAddr", Port->device->index,Port->index);
            break;
          case AvFsmCecAudioControl:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecAudioControl", Port->device->index,Port->index);
            break;
          case AvFsmCecAudioFormat:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecAudioFormat", Port->device->index,Port->index);
            break;
          case AvFsmCecAudioManage:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecAudioManage", Port->device->index,Port->index);
            break;
          case AvFsmCecCmdARC:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecCmdARC", Port->device->index,Port->index);
            break;
          case AvFsmCecCmdAudioARC:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecCmdAudioARC", Port->device->index,Port->index);
            break;
          case AvFsmCecCmdSystemAudioModetoAll:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecCmdSystemAudioModetoAll", Port->device->index,Port->index);
            break;
          case AvFsmCecFunctionalDefault:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecFunctionalDefault", Port->device->index,Port->index);
            break;
          case AvFsmCecCmdActiveSource:
            AvUapiOutputDebugFsm("Dev%d,Port%d: CecCmdActiveSource", Port->device->index,Port->index);
            break;
        }
    }
}
#endif /* CEC Related */

#if AvEnableCcaFeature /* CCA Related */
void PrintCcaTxFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_CcaTxFsm != OldState)
    {
        switch (*Port->content.is_CcaTxFsm)
        {
            case AvFsmCcaTxCalib:
                AvUapiOutputDebugFsm("Port%d: CcaTxCalib", Port->index);
                break;
            case AvFsmCcaTxDefault:
                AvUapiOutputDebugFsm("Port%d: CcaTxDefault", Port->index);
                break;
            case AvFsmCcaTxInit:
                AvUapiOutputDebugFsm("Port%d: CcaTxInit", Port->index);
                break;
            case AvFsmCcaTxLinkConnect:
                AvUapiOutputDebugFsm("Port%d: CcaTxLinkConnect", Port->index);
                break;
            case AvFsmCcaTxLinkStable:
                AvUapiOutputDebugFsm("Port%d: CcaTxLinkStable", Port->index);
                break;
            case AvFsmCcaTxSetupChain:
                AvUapiOutputDebugFsm("Port%d: CcaTxSetupChain", Port->index);
                break;
            case AvFsmCcaTxUnlock:
                AvUapiOutputDebugFsm("Port%d: CcaTxUnlock", Port->index);
                break;
        }
    }
}

void PrintCcaRxFsm(AvPort *Port, uint8 OldState)
{
    if(*Port->content.is_CcaRxFsm != OldState)
    {
        switch (*Port->content.is_CcaRxFsm)
        {
            case AvFsmCcaRxCalib:
                AvUapiOutputDebugFsm("Port%d: CcaRxCalib", Port->index);
                break;
            case AvFsmCcaRxDefault:
                AvUapiOutputDebugFsm("Port%d: CcaRxDefault", Port->index);
                break;
            case AvFsmCcaRxInit:
                AvUapiOutputDebugFsm("Port%d: CcaRxInit", Port->index);
                break;
            case AvFsmCcaRxLinkConnect:
                AvUapiOutputDebugFsm("Port%d: CcaRxLinkConnect", Port->index);
                break;
            case AvFsmCcaRxLinkStable:
                AvUapiOutputDebugFsm("Port%d: CcaRxLinkStable", Port->index);
                break;
            case AvFsmCcaRxSetupChain:
                AvUapiOutputDebugFsm("Port%d: CcaRxSetupChain", Port->index);
                break;
            case AvFsmCcaRxUnlock:
                AvUapiOutputDebugFsm("Port%d: CcaRxUnlock", Port->index);
                break;
        }
    }
}
#endif  /* CCA Related */

#endif

