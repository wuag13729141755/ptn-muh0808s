/**
 * @file kernel_fsm_update.h
 *
 * @brief functions for update FSMs in the kernel layer
 */
#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "kernel_status_update.h"
#include "Gsv_kapi.h"
#include "uapi_function_mapper.h"
#include "Gsv_uapi.h"

extern const AvDscPlanVicTables DscPlanVicTable[];

/* PART 1 */
/* Receiver FSM */

/* check RX FSM status */
void KfunCheckRxState(pin AvPort *port)
{
    /* if no new information, return state */
    switch (*port->content.is_ReceiverFsm)
    {
        case AvFsmRxDefault:
            break;
        case AvFsmRxDetect:
            {
                KfunPollingRxStatus(port);
                break;
            }
        case AvFsmRxFreerun:
            {
                KfunPollingRxStatus(port);
                break;
            }
        case AvFsmRxReceiving:
            {
                KfunPollingRxStatus(port);
                break;
            }
        case AvFsmRxReset:
            {
                KfunPollingRxStatus(port);
                break;
            }
        default:
            break;
    }

    return;
}

/* PART 2 */
/* HDCP FSM */
#if AvEnableSimplifyHdcp
void KfunSimpleHdcpManage(AvPort *port)
{
    /* 1. Firstly Find whether source is encrypted or not */
    KfunHdcpDetectSource(port);
    /* 2. No matter source is encrypted or not, sink should also get encrypt info */
    KfunHdcpDetectSink(port);
    /* 3. Sync them */
    KfunSimpleHdcpSync(port);
    return;
}

#else
void KfunCheckRxHdcpState( AvPort *port)
{
    /* check BKSV Ready */
    /* Int.BksvReady */
    /* HDCP Authenticated */
    /* Int.HdcpAuth */
    /* HDCP Error */
    /* Int.HdcpError */

    switch (*port->content.is_HdcpFsm)
    {
        case AvFsmHdcpDefault:
            break;
        case AvFsmHdcpDisable:
        {
            break;
        }
        case AvFsmHdcpFail:
        {
            /* if pr->hpd, go to HDCP_DISABLE*/
            break;
        }
        case AvFsmHdcpUpstreamConnected:
        {
            KfunHdcpDetectSource(port);
            break;
        }
        case AvFsmHdcpStartAuthentication:
        {
            KfunHdcpDetectSource(port);
            break;
        }
        case AvFsmHdcpReadBksv:
        {
            /* allocate a ptr for sink list*/
            /* then use a chain */
            /* check BKSV update */
            KfunHdcpDetectSink(port);
            break;
        }
        case AvFsmHdcpRequestSinkHdcp:
        {
            KfunHdcpDetectSource(port);
            KfunHdcpDetectSink(port);
            break;
        }
        case AvFsmHdcpReceiverMode:
        {
            KfunHdcpDetectSource(port);
            KfunHdcpDetectSink(port);
            break;
        }
        case AvFsmHdcpRepeaterMode:
        {
            KfunHdcpDetectSource(port);
            KfunHdcpDetectSink(port);
            break;
        }
        case AvFsmHdcpReAuthentication:
        {
            KfunHdcpDetectSource(port);
            break;
        }
        case AvFsmHdcpRepeaterReset:
        {
            KfunHdcpDetectSink(port);
            break;
        }
        default:
            break;
    }

    return;
}
#endif

/* PART 3 */
/* Rx Routing FSM */

/* PART 4 */
/* Tx Routing FSM */

/* PART 5 */
/* Plug RX FSM */

void KfunCheckPrState( AvPort *port)
{
    /* State detection based on FSM state */
    /* TMDS clock detection, not interrupt */
    /* HdmiInts.TmdsClkChg */
    /* cable detection, not interrupt */
    /* HdmiInts.CableDetect */
    /* Internal Mute detection*/
    /* HdmiInts.InternMute */
    /* Video PLL Lock */
    /* HdmiInts.VidPllChg */
    /* Check DE regen lock */
    /* HdmiInts.DeRegenLck */
    /* Vsync lock */
    /* HdmiInts.VsyncLck */
    /* Encryption state */
    /* HdmiInts.Encrypted */

    /* TMDS Frequency Change int*/
    /* HdmiInts.NewTmdsFreq */

    /* check routing, if not sink is connected, reset this port */
    #if (AvAllowHpdLowEdidRead == 1)
    /* force to Reset = 0 */
    port->content.rx->Reset = 0;
    #else
    if(port->core.HdmiCore != -1)
        port->content.rx->Reset = 0;
    else
        port->content.rx->Reset = 1;
    #endif

    switch (*port->content.is_PlugRxFsm)
    {
        case AvFsmPlugRxDefault:
            break;
        case AvFsmPlugRxDetect:
        {
            AvUapiRxGetStatus(port);
            break;
        }
        case AvFsmPlugRxInfoUpdate:
            break;
        case AvFsmPlugRxInputLock:
        {
            AvUapiRxGetStatus(port);
            break;
        }
        case AvFsmPlugRxPlugged:
        {
            AvUapiRxGetStatus(port);
            break;
        }
        case AvFsmPlugRxPullDownHpd:
        {
            AvUapiRxGetStatus(port);
            break;
        }
        case AvFsmPlugRxReadTiming:
            break;
        case AvFsmPlugRxReset:
        {
            AvUapiRxGetStatus(port);
            break;
        }
        case AvFsmPlugRxStable:
        {
            AvUapiRxGetStatus(port);
            break;
        }
        default:
            break;
    }
}

/* PART 6 */
/* Plug TX FSM */

void KfunCheckPtState( AvPort *port)
{
    /* HPD Interrupt */
    /* Int.Hpd */
    /* MSEN state */
    /* Int.MonSen */
    /* EDID Ready Interrupt */
    /* Int.EdidReady*/
    /* Vsync Interrupt */
    /* Int.Vsync */

    switch (*port->content.is_PlugTxFsm)
    {
        case AvFsmPlugTxEdidError:
        {
            AvUapiTxGetStatus(port);
            break;
        }
        case AvFsmPlugTxDefault:
            break;
        case AvFsmPlugTxDefaultEdid:
            break;
        case AvFsmPlugTxEdid:
        {
            AvUapiTxGetStatus(port);
            /* AvUapiTxEdidStat(port); */
            break;
        }
        case AvFsmPlugTxEdidManage:
        {
            AvUapiTxGetStatus(port);
            /* AvUapiTxEdidStat(port); */
            break;
        }
        case AvFsmPlugTxHdcp:
        {
            AvUapiTxGetStatus(port);
            AvUapiTxGetHdcpStatus(port);
            break;
        }
        case AvFsmPlugTxHpdAntiDither:
        {
            AvUapiTxGetStatus(port);
            break;
        }
        case AvFsmPlugTxReset:
        {
            AvUapiTxGetStatus(port);
            break;
        }
        case AvFsmPlugTxStable:
        {
            AvUapiTxGetStatus(port);
            AvUapiTxGetHdcpStatus(port);
            break;
        }
        case AvFsmPlugTxTransmitVideo:
        {
            if(port->content.tx->InfoReady == 0)
                AvHandleEvent(port, AvEventPortDownStreamSending, NULL, NULL);
            AvUapiTxGetStatus(port);
            AvUapiTxGetHdcpStatus(port);
            break;
        }
        case AvFsmPlugTxEnableTxCore:
        {
            AvHandleEvent(port, AvEventPortDownStreamConnected, NULL, NULL);
            AvUapiTxGetStatus(port);
            break;
        }
        case AvFsmPlugTxVideoUnlocked:
        {
            AvUapiTxGetStatus(port);
            break;
        }
        default:
            break;
    }
    return;
}

/* PART 7 */
/* CEC FSM state */
void KfunCheckCecState(AvPort *port)
{
#if AvEnableCecFeature /* CEC Related */
    switch (*port->content.is_CecFsm)
    {
        case AvFsmCecDefault:
            break;
        case AvFsmCecNotConnected:
            break;
        case AvFsmCecReset:
            break;
        default:
            AvUapiCecRxGetStatus(port);
            AvUapiCecTxGetStatus(port);
            if(port->content.cec->RxGetFlag)
                KfunCecRxMsgProcess(port);
            break;
    }
#endif
    return;
}

/* Part 8 Video Input and Output */
void KfunCheckLogicVideoTx(AvPort *port)
{
#if GSV2K11
    if(port->content.RouteVideoFromPort != NULL)
        AvUapiCheckLogicVideoTx(port);
#endif
}

void KfunCheckLogicVideoRx(AvPort *port)
{
#if GSV2K11
    if(port->content.RouteVideoToPort != NULL)
        AvUapiCheckLogicVideoRx(port);
#endif
}

/* Part 9 Audio Input and Output */
void KfunCheckLogicAudioTx(AvPort *port)
{
    AvUapiCheckLogicAudioTx(port);
}

void KfunCheckLogicAudioRx(AvPort *port)
{
#if AvEnableAudioTTLInput
    AvUapiCheckLogicAudioRx(port);
#endif
}

#if GSV5K1
/* Part 9 DSC */
void KfunCheckVideoDsc(AvPort *port)
{
    uint8 i = 0;
    AvPort *SourcePort;
    AvPort *FrontPort;
    AvVideoY InY;
    AvHdcpSt HdcpStat;

    SourcePort = (AvPort*)port->content.RouteVideoFromPort;
    if((SourcePort == NULL) || (port->content.dsc->Enable == 0))
        return;
    if(KfunFindVideoRxFront(port, &FrontPort) == AvOk)
    {
        if(((FrontPort->type == HdmiRx) || (FrontPort->type == CcaRx)) && (port->content.dsc->DscInCompressed == 0))
        {
            if((port->content.dsc->SrcInY    != FrontPort->content.video->Y) ||
               (port->content.dsc->SrcInCs   != FrontPort->content.video->OutCs) ||
               (port->content.dsc->SrcInCd   != FrontPort->content.video->Cd) ||
               (port->content.dsc->SrcInHdmi != FrontPort->content.rx->HdmiMode))
            {
                port->content.dsc->Update    = 0x03;
                port->content.dsc->SrcInY    = FrontPort->content.video->Y;
                port->content.dsc->SrcInCs   = FrontPort->content.video->OutCs;
                port->content.dsc->SrcInCd   = FrontPort->content.video->Cd;
                port->content.dsc->SrcInHdmi = FrontPort->content.rx->HdmiMode;
            }
            HdcpStat = AV_HDCP_NOT_ENCRYPTED;
            if(FrontPort->content.rx->VideoEncrypted)
            {
                if(FrontPort->content.hdcp->Hdcp2p2RxRunning == 1)
                    HdcpStat = AV_HDCP_2P2_ENCRYPTED;
                else
                    HdcpStat = AV_HDCP_1P4_ENCRYPTED;
            }
            if(HdcpStat != port->content.dsc->HdcpStat)
            {
                port->content.dsc->Update    = 0x03;
                port->content.dsc->HdcpStat  = HdcpStat;
            }
        }
    }
    if(((SourcePort->type == HdmiRx) || (SourcePort->type == CcaRx)) &&
       (port->content.dsc->DscInCompressed == 0))
    {
        if(SourcePort->content.video->Y == AV_Y2Y1Y0_YCBCR_422)
            InY = AV_Y2Y1Y0_YCBCR_444;
        else
            InY = SourcePort->content.video->Y;

        if((port->content.dsc->DscInY    != InY) ||
           (port->content.dsc->DscInCs   != SourcePort->content.video->InCs) ||
           (port->content.dsc->DscInVic  != SourcePort->content.video->timing.Vic))
        {
            port->content.dsc->Update = 0x03;
            port->content.dsc->DscInY  = InY;
            port->content.dsc->SrcInY  = SourcePort->content.video->Y;
            port->content.dsc->DscInCs = SourcePort->content.video->InCs;
            port->content.dsc->DscInVic = SourcePort->content.video->timing.Vic;
        }
    }
    else if(SourcePort->type == VideoColor)
    {
        /* Update 420 */
        if((port->content.dsc->DscInY != SourcePort->content.color->ColorOutSpace) ||
           (port->content.dsc->DscInCs != SourcePort->content.color->ColorOutCs) ||
           (port->content.dsc->DscInVic != SourcePort->content.color->ColorInVic))
        {
            port->content.dsc->Update = 0x03;
            port->content.dsc->DscInY  = SourcePort->content.color->ColorOutSpace;
            port->content.dsc->DscInCs = SourcePort->content.color->ColorOutCs;
            port->content.dsc->DscInVic = SourcePort->content.color->ColorInVic;
        }
    }
    /* Update Ratio */
    if((((SourcePort->type == HdmiRx) || (SourcePort->type == CcaRx)) &&
        (port->content.dsc->DscInCompressed == 0)) ||
       (SourcePort->type == VideoColor))
    {
        port->content.dsc->DscRatio = AV_DSC_1BY1;
        for(i=0;DscPlanVicTable[i].InVic!=255;i++)
        {
            if(DscPlanVicTable[i].InVic == port->content.dsc->DscInVic)
            {
                port->content.dsc->DscRatio = DscPlanVicTable[i].Ratio;
                break;
            }
        }
    }

    /* Call Uapi Function */
    AvUapiCheckVideoDsc(port);
}
#endif

/* Part 10 Scaler */
void KfunCheckVideoScaler(AvPort *port)
{
#if GSV5K1
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    uint32 EdidSupportFeature = 0xffffffff;
    AvVideoY NewColorOutSpace;
    AvVideoCs NewColorOutCs;
#endif

    AvPort *SourcePort = (AvPort*)port->content.RouteVideoFromPort;
    if(SourcePort == NULL)
        return;
    else if(port->content.RouteVideoToPort == NULL)
    {
        port->content.scaler->ScalerInVic = 0;
        #if GSV5K1
        if(port->device->type == Gsv5k1)
        {
            port->content.scaler->ScalerInSpace  = AV_Y2Y1Y0_INVALID;
            port->content.scaler->ScalerOutSpace = AV_Y2Y1Y0_INVALID;
            port->content.scaler->ScalerInCs     = AV_CS_AUTO;
            port->content.scaler->ScalerOutCs    = AV_CS_AUTO;
        }
        else
        #endif
        {
            port->content.scaler->ColorSpace  = AV_Y2Y1Y0_INVALID;
        }
    }
    else if(SourcePort->type == HdmiRx
        || (SourcePort->type == CcaRx)
        #if AvEnableInternalVideoGen
        || (SourcePort->type == VideoGen)
        #endif
        )
    {
        /* Update Vic */
        if(port->content.scaler->ScalerInVic != SourcePort->content.video->timing.Vic)
        {
            port->content.scaler->Update = 0x03;
            port->content.scaler->ScalerInVic = SourcePort->content.video->timing.Vic;
        }
    #if GSV5K1
        if(port->device->type == Gsv5k1)
        {
            /* Input ColorSpace */
            if(port->content.scaler->ScalerInSpace != SourcePort->content.video->Y)
            {
                port->content.scaler->Update = 0x03;
                port->content.scaler->ScalerInSpace = SourcePort->content.video->Y;
            }
            if(port->content.scaler->ScalerInCs != SourcePort->content.video->InCs)
            {
                port->content.scaler->Update = 0x03;
                port->content.scaler->ScalerInCs = SourcePort->content.video->InCs;
            }
            /* Check DSC Existing in the chain */
            CurrentPort = (AvPort*)(port->content.RouteVideoToPort);
            if((CurrentPort != NULL) && ((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx)))
                CurrentPort = (AvPort *)(CurrentPort->content.RouteVideoNextSameLevelPort);
            /* Output ColorSpace */
            if((CurrentPort != NULL) && (CurrentPort->type == VideoDsc))
            {
                /* DSC encoder will always use YCbCr444 */
                NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
            }
            else
            {
                /* Normal process, swap 420 and 444 */
                if(port->content.scaler->ScalerInSpace != AV_Y2Y1Y0_YCBCR_420)
                    NewColorOutSpace = AV_Y2Y1Y0_YCBCR_420;
                else
                    NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
            }
            if(NewColorOutSpace != port->content.scaler->ScalerOutSpace)
            {
                port->content.scaler->Update = 0x03;
                port->content.scaler->ScalerOutSpace = NewColorOutSpace;
            }

            /* Check Downstream Full Range Support */
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if(((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx)) &&
                   (CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
                    EdidSupportFeature = EdidSupportFeature & CurrentPort->content.tx->EdidSupportFeature;
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
            /* If DownStream is DSC Encoding, always use YCbCr color space */
            CurrentPort = (AvPort*)port->content.RouteVideoToPort;
            if(CurrentPort->type == VideoDsc)
            {
                NewColorOutCs = AV_CS_YUV_709;
            }
            /* Normal HDMI Output */
            else
            {
                if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                {
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                        NewColorOutCs = AV_CS_YUV_709;
                    else
                        NewColorOutCs = AV_CS_YUV_601;
                }
                else
                {
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                        NewColorOutCs = AV_CS_LIM_YUV_709;
                    else
                        NewColorOutCs = AV_CS_LIM_YUV_601;
                }
            }
            /* Update CS when new State */
            if(port->content.scaler->ScalerOutCs != NewColorOutCs)
            {
                port->content.scaler->Update = 0x03;
                port->content.scaler->ScalerOutCs = NewColorOutCs;
            }
        }
    #endif
        else
        {
            if(port->content.scaler->ColorSpace != SourcePort->content.video->Y)
            {
                port->content.scaler->Update = 0x03;
                port->content.scaler->ColorSpace = SourcePort->content.video->Y;
            }
        }
    }
#if GSV5K1
    else if((port->device->type == Gsv5k1)&&(SourcePort->type == VideoDsc))
    {
        /* Input Vic */
        if(port->content.scaler->ScalerInVic != SourcePort->content.dsc->DscInVic)
        {
            port->content.scaler->Update = 0x03;
            port->content.scaler->ScalerInVic = SourcePort->content.dsc->DscInVic;
        }
        /* Input ColorSpace */
        if(port->content.scaler->ScalerInSpace != SourcePort->content.dsc->DscInY)
        {
            port->content.scaler->Update = 0x03;
            port->content.scaler->ScalerInSpace = SourcePort->content.dsc->DscInY;
        }
        if(port->content.scaler->ScalerInCs != SourcePort->content.dsc->DscInCs)
        {
            port->content.scaler->Update = 0x03;
            port->content.scaler->ScalerInCs = SourcePort->content.dsc->DscInCs;
        }

        /* Check Downstream Full Range Support */
        while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            if(((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))&&
               (CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
                EdidSupportFeature = EdidSupportFeature & CurrentPort->content.tx->EdidSupportFeature;
            PrevPort = CurrentPort;
            CurrentPort = NULL;
        }
        /* 1. DSC Out Color Space = YCbCr444 forever,
           No change of Color Space from CP when DSC->CP including 420 */
        if(SourcePort->content.dsc->SrcInY == AV_Y2Y1Y0_YCBCR_420)
            NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
        else
            NewColorOutSpace = SourcePort->content.dsc->DscInY;
        /* 2. DSC Out CSC will be done only when changing to 420 */
        if(SourcePort->content.dsc->SrcInY == AV_Y2Y1Y0_YCBCR_420)
        {
            /* Need to do CSC, only 4 output options, 601/709 x Full/Limited */
            /* ColorOutCs is always capable of output on HDMI Txs */
            /* 2.1 ColorInCs reformat */
            switch(port->content.scaler->ScalerInCs)
            {
                case AV_CS_YUV_601:
                    port->content.scaler->ScalerInCs = AV_CS_YUV_601;
                    break;
                case AV_CS_YUV_709:
                    port->content.scaler->ScalerInCs = AV_CS_YUV_709;
                    break;
                case AV_CS_LIM_YUV_601:
                    port->content.scaler->ScalerInCs = AV_CS_LIM_YUV_601;
                    break;
                case AV_CS_LIM_YUV_709:
                    port->content.scaler->ScalerInCs = AV_CS_LIM_YUV_709;
                    break;
            }
            /* 2.2 ColorOutCs reformat */
            switch(SourcePort->content.dsc->SrcInCs)
            {
                case AV_CS_YUV_601:
                case AV_CS_YCC_601:
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                        NewColorOutCs = AV_CS_YUV_601;
                    else
                        NewColorOutCs = AV_CS_LIM_YUV_601;
                    break;
                case AV_CS_YUV_709:
                case AV_CS_YCC_709:
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_YUV_709;
                        else
                            NewColorOutCs = AV_CS_YUV_601;
                    }
                    else
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_LIM_YUV_709;
                        else
                            NewColorOutCs = AV_CS_LIM_YUV_601;
                    }
                    break;
                case AV_CS_LIM_YUV_601:
                case AV_CS_LIM_YCC_601:
                    NewColorOutCs = AV_CS_LIM_YUV_601;
                    break;
                case AV_CS_LIM_YUV_709:
                case AV_CS_LIM_YCC_709:
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                        NewColorOutCs = AV_CS_LIM_YUV_709;
                    else
                        NewColorOutCs = AV_CS_LIM_YUV_601;
                    break;
                default:
                    NewColorOutCs = port->content.scaler->ScalerInCs;
                    break;
            }
        }
        /* No CP CSC enabled for non-420 color space, use Tx CSC instead */
        else
        {
            NewColorOutCs = port->content.scaler->ScalerInCs;
        }
        /* 3.1 Color Space Check */
        if(port->content.scaler->ScalerOutSpace != NewColorOutSpace)
        {
            port->content.scaler->Update = 0x03;
            port->content.scaler->ScalerOutSpace = NewColorOutSpace;
        }
        /* 3.2 Update CS Check */
        if(port->content.scaler->ScalerOutCs != NewColorOutCs)
        {
            port->content.scaler->Update = 0x03;
            port->content.scaler->ScalerOutCs = NewColorOutCs;
        }
    }
#endif

    /* Call Uapi Function */
    AvUapiCheckVideoScaler(port);
}

/* Part 11 Color */
void KfunCheckVideoColor(AvPort *port)
{
    AvPort *SourcePort = (AvPort*)port->content.RouteVideoFromPort;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    uint32 EdidSupportFeature = 0xffffffff;
    AvVideoY NewColorOutSpace;
    AvVideoCs NewColorOutCs;
#if GSV2K11
    AvPort *l_pLogicVideoTxPort = NULL;
    uint8  LogicVideoTxFlag = 0;
#endif
    if(SourcePort == NULL)
        return;
    else if(port->content.RouteVideoToPort == NULL)
    {
        port->content.color->ColorInVic    = 0;
        port->content.color->ColorInSpace  = AV_Y2Y1Y0_INVALID;
        port->content.color->ColorOutSpace = AV_Y2Y1Y0_INVALID;
        port->content.color->ColorInCs     = AV_CS_AUTO;
        port->content.color->ColorOutCs    = AV_CS_AUTO;
    }
    else if((SourcePort->type == HdmiRx)
        || (SourcePort->type == CcaRx)
        #if AvEnableInternalVideoGen
        || (SourcePort->type == VideoGen)
        #endif
        ||(SourcePort->type == LogicVideoRx)
        )
    {
        if(port->device->type != Gsv5k1)
        {
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if((CurrentPort->type == HdmiTx) &&
                   (CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
                    EdidSupportFeature = EdidSupportFeature & CurrentPort->content.tx->EdidSupportFeature;
            #if GSV2K11
                else if(CurrentPort->type == LogicVideoTx)
                {
                    LogicVideoTxFlag = 1;
                    l_pLogicVideoTxPort = CurrentPort;
                }
            #endif
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
        /* Input Vic */
        if(port->content.color->ColorInVic != SourcePort->content.video->timing.Vic)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorInVic = SourcePort->content.video->timing.Vic;
        }
        /* Input ColorSpace */
        if(port->content.color->ColorInSpace != SourcePort->content.video->Y)
        {
            port->content.color->Update = 0x03;
            #if AvEnableInternalVideoGen
            if(SourcePort->type == VideoGen)
                port->content.color->ColorInSpace = AV_Y2Y1Y0_RGB;
            else
            #endif
            port->content.color->ColorInSpace = SourcePort->content.video->Y;
        }
        if(port->content.color->ColorInCs != SourcePort->content.video->InCs)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorInCs = SourcePort->content.video->InCs;
        }
        /* Output ColorSpace */
    #if AvGsv2kxVideoCscForValensTx
         /* Source-Valens-Sink Edit Start */
        if(SourcePort->content.video->info.TmdsFreq >= 580)
            /* Convert 6G to 3G */
            NewColorOutSpace = AV_Y2Y1Y0_YCBCR_420;
        else
            /* Crop to 8-bit for 3G+ */
            NewColorOutSpace = port->content.color->ColorInSpace;
        /* Source-Valens-Sink Edit End */
    #else
      #if GSV5K1
        if(port->device->type == Gsv5k1)
        {
             /* Check DSC Existing in the chain */
            CurrentPort = (AvPort*)(port->content.RouteVideoToPort);
            if((CurrentPort != NULL) && ((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx)))
                CurrentPort = (AvPort *)(CurrentPort->content.RouteVideoNextSameLevelPort);
            /* Output ColorSpace */
            if((CurrentPort != NULL) &&
               (CurrentPort->type == VideoDsc) &&
               (CurrentPort->content.RouteVideoToPort != NULL))
            {
            #if (Gsv5k1RomFromOtp == 1)
                if(((port->content.color->ColorInVic >= 93) && (port->content.dsc->DscInVic < 128)) &&
                   (port->content.color->ColorInSpace == AV_Y2Y1Y0_YCBCR_420) &&
                   ((SourcePort->content.video->Cd == AV_CD_30) || (SourcePort->content.video->Cd == AV_CD_36)))
                    /* 4K 420 deep color will bypass CP processing */
                    NewColorOutSpace = AV_Y2Y1Y0_YCBCR_420;
                else
            #endif
                    /* DSC encoder will always use YCbCr444 */
                    NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
            }
            else
            {
                /* Normal process, swap 420 and 444 */
                if(port->content.color->ColorInSpace != AV_Y2Y1Y0_YCBCR_420)
                    NewColorOutSpace = AV_Y2Y1Y0_YCBCR_420;
                else
                    NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
            }
        }
        else
      #endif
        #if GSV2K11
        if(LogicVideoTxFlag == 1)
        {
            /* Parallel bus YCbCr 422 output for YCbCr 444 fixed output*/
            if(((ParallelConfigTable[l_pLogicVideoTxPort->content.lvtx->Config*3 + 1] & 0x04) != 0) ||
               ((ParallelConfigTable[l_pLogicVideoTxPort->content.lvtx->Config*3 + 2] & 0x40) != 0) ||
               (l_pLogicVideoTxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_422))
                NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
            else if(l_pLogicVideoTxPort->content.video->Y == AV_Y2Y1Y0_INVALID)
                NewColorOutSpace = SourcePort->content.video->Y;
            else if((l_pLogicVideoTxPort->content.video->Y == AV_Y2Y1Y0_RGB) &&
                    (SourcePort->content.video->Y == AV_Y2Y1Y0_YCBCR_420))
                NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
            else
                NewColorOutSpace = l_pLogicVideoTxPort->content.video->Y;
        }
        else
        #endif
        {
            if(port->content.color->ColorInSpace != AV_Y2Y1Y0_YCBCR_420)
                NewColorOutSpace = AV_Y2Y1Y0_YCBCR_420;
            else
                NewColorOutSpace = AV_Y2Y1Y0_YCBCR_444;
        }
    #endif

        if(NewColorOutSpace != port->content.color->ColorOutSpace)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorOutSpace = NewColorOutSpace;
        }

        if(port->device->type != Gsv2k11)
        {
            /* Check Downstream Full Range Support */
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if(((CurrentPort->type == HdmiTx)||(CurrentPort->type == CcaTx)) &&
                   (CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
                    EdidSupportFeature = EdidSupportFeature & CurrentPort->content.tx->EdidSupportFeature;
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
            #if GSV5K1
            /* If DownStream is DSC Encoding, always use YCbCr color space */
            CurrentPort = (AvPort*)port->content.RouteVideoToPort;
            /* Normal HDMI Output */
            if((port->device->type == Gsv5k1))
            {
                if(CurrentPort->type == VideoDsc)
                {
                    if(SourcePort->content.video->Y == AV_Y2Y1Y0_RGB)
                        NewColorOutCs = AV_CS_YUV_709;
                    else
                        NewColorOutCs = port->content.color->ColorInCs;
                }
                else if(((SourcePort->content.video->AvailableVideoPackets & AV_BIT_HDR_PACKET) == 0) ||
                (SourcePort->content.video->Y == AV_Y2Y1Y0_RGB))
                {
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_YUV_709;
                        else
                            NewColorOutCs = AV_CS_YUV_601;
                    }
                    else
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_LIM_YUV_709;
                        else
                            NewColorOutCs = AV_CS_LIM_YUV_601;
                    }
                }
                else
                    /* no CSC for HDR */
                    NewColorOutCs = port->content.color->ColorInCs;
            }
            else
            #endif
            {
                if(((SourcePort->content.video->AvailableVideoPackets & AV_BIT_HDR_PACKET) == 0) ||
                    (SourcePort->content.video->Y == AV_Y2Y1Y0_RGB))
                {
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_YUV_709;
                        else
                            NewColorOutCs = AV_CS_YUV_601;
                    }
                    else
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_LIM_YUV_709;
                        else
                            NewColorOutCs = AV_CS_LIM_YUV_601;
                    }
                }
                else
                    /* no CSC for HDR */
                    NewColorOutCs = port->content.color->ColorInCs;
            }
            /* Update CS when new State */
            if(port->content.color->ColorOutCs != NewColorOutCs)
            {
                port->content.color->Update = 0x03;
                port->content.color->ColorOutCs = NewColorOutCs;
            }
        }
        else
        {
            #if GSV2K11
            if(LogicVideoTxFlag == 1)
            {
                if(((SourcePort->content.video->InCs == AV_CS_LIM_BT2020_YCC) ||
                    (SourcePort->content.video->InCs == AV_CS_LIM_BT2020_RGB)) &&
                   (NewColorOutSpace == AV_Y2Y1Y0_RGB))
                {
                    port->content.color->ColorInCs  = AV_CS_LIM_YUV_601;
                }
                /* No CP CSC for 420 input, bypass CSC */
                if(SourcePort->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
                    NewColorOutCs = SourcePort->content.video->InCs;
                else if(l_pLogicVideoTxPort->content.video->InCs == AV_CS_AUTO)
                {
                    if((NewColorOutSpace != AV_Y2Y1Y0_RGB) && (SourcePort->content.video->Y == AV_Y2Y1Y0_RGB))
                        NewColorOutCs = AV_CS_LIM_YUV_709;
                    else if((NewColorOutSpace == AV_Y2Y1Y0_RGB) && (SourcePort->content.video->Y != AV_Y2Y1Y0_RGB))
                        NewColorOutCs = AV_CS_LIM_RGB;
                    else
                        NewColorOutCs = SourcePort->content.video->InCs;
                }
                else
                    NewColorOutCs = l_pLogicVideoTxPort->content.video->InCs;
            }
            else
            #endif
            if(((SourcePort->content.video->AvailableVideoPackets & AV_BIT_HDR_PACKET) == 0) ||
               (SourcePort->content.video->Y == AV_Y2Y1Y0_RGB))
            {
                if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                {
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                        NewColorOutCs = AV_CS_YUV_709;
                    else
                        NewColorOutCs = AV_CS_YUV_601;
                }
                else
                {
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                        NewColorOutCs = AV_CS_LIM_YUV_709;
                    else
                        NewColorOutCs = AV_CS_LIM_YUV_601;
                }
            }
            else
                /* no CSC for HDR */
                NewColorOutCs = port->content.color->ColorInCs;
            /* Update CS when new State */
            if(port->content.color->ColorOutCs != NewColorOutCs)
            {
                port->content.color->Update = 0x03;
                port->content.color->ColorOutCs = NewColorOutCs;
            }
        }
    }
#if GSV5K1
    else if((port->device->type == Gsv5k1)&&(SourcePort->type == VideoDsc))
    {
        /* Input Vic */
        if(port->content.color->ColorInVic != SourcePort->content.dsc->DscInVic)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorInVic = SourcePort->content.dsc->DscInVic;
        }
        /* Input ColorSpace */
        if(port->content.color->ColorInSpace != SourcePort->content.dsc->DscInY)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorInSpace = SourcePort->content.dsc->DscInY;
        }
        if(port->content.color->ColorInCs != SourcePort->content.dsc->DscInCs)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorInCs = SourcePort->content.dsc->DscInCs;
        }

        /* Check Downstream Full Range Support */
        while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            if(((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))&&
               (CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
                EdidSupportFeature = EdidSupportFeature & CurrentPort->content.tx->EdidSupportFeature;
            PrevPort = CurrentPort;
            CurrentPort = NULL;
        }
        /* 1. DSC->Color will do YCbCr444 to YCbCr420 forever */
        NewColorOutSpace = AV_Y2Y1Y0_YCBCR_420;
        /* 2. DSC Out CSC will be done only when changing to 420 */
        if(SourcePort->content.dsc->SrcInY == AV_Y2Y1Y0_YCBCR_420)
        {
            /* Need to do CSC, only 4 output options, 601/709 x Full/Limited */
            /* ColorOutCs is always capable of output on HDMI Txs */
            /* 2.1 ColorOutCs reformat */
            switch(SourcePort->content.dsc->SrcInCs)
            {
                case AV_CS_YUV_601:
                case AV_CS_YCC_601:
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                        NewColorOutCs = AV_CS_YUV_601;
                    else
                        NewColorOutCs = AV_CS_LIM_YUV_601;
                    break;
                case AV_CS_YUV_709:
                case AV_CS_YCC_709:
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE)
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_YUV_709;
                        else
                            NewColorOutCs = AV_CS_YUV_601;
                    }
                    else
                    {
                        if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                            NewColorOutCs = AV_CS_LIM_YUV_709;
                        else
                            NewColorOutCs = AV_CS_LIM_YUV_601;
                    }
                    break;
                case AV_CS_LIM_YUV_601:
                case AV_CS_LIM_YCC_601:
                    NewColorOutCs = AV_CS_LIM_YUV_601;
                    break;
                case AV_CS_LIM_YUV_709:
                case AV_CS_LIM_YCC_709:
                    if(EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                        NewColorOutCs = AV_CS_LIM_YUV_709;
                    else
                        NewColorOutCs = AV_CS_LIM_YUV_601;
                    break;
                default:
                    NewColorOutCs = port->content.color->ColorInCs;
                    break;
            }
        }
        /* No CP CSC enabled for non-420 color space, use Tx CSC instead */
        else
        {
            NewColorOutCs = port->content.color->ColorInCs;
        }
        /* 3.1 Color Space Check */
        if(port->content.color->ColorOutSpace != NewColorOutSpace)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorOutSpace = NewColorOutSpace;
        }
        /* 3.2 Update CS Check */
        if(port->content.color->ColorOutCs != NewColorOutCs)
        {
            port->content.color->Update = 0x03;
            port->content.color->ColorOutCs = NewColorOutCs;
        }
    }
#endif
    /* Call Uapi Function */
    AvUapiCheckVideoColor(port);
}

/* Part 12 Cca Tx */
#if AvEnableCcaFeature
void KfunCheckCcaTxState(AvPort *port)
{
    switch (*port->content.is_CcaTxFsm)
    {
        case AvFsmCcaTxDefault:
            break;
        case AvFsmCcaTxCalib:
            AvUapiCcaTxGetInfo(port);
            break;
        case AvFsmCcaTxInit:
            AvUapiCcaTxGetInfo(port);
            break;
        case AvFsmCcaTxLinkConnect:
            AvUapiCcaTxGetInfo(port);
            break;
        case AvFsmCcaTxLinkStable:
            AvUapiCcaTxGetInfo(port);
            break;
        case AvFsmCcaTxSetupChain:
            AvUapiCcaTxGetInfo(port);
            break;
        case AvFsmCcaTxUnlock:
            AvUapiCcaTxGetInfo(port);
            break;
    }
    return;
}
#endif /* AvEnableCcaFeature */

/* Part 13 Cca Rx */
#if AvEnableCcaFeature
void KfunCheckCcaRxState(AvPort *port)
{
    switch (*port->content.is_CcaRxFsm)
    {
        case AvFsmCcaRxDefault:
            break;
        case AvFsmCcaRxCalib:
            AvUapiCcaRxGetInfo(port);
            break;
        case AvFsmCcaRxInit:
            AvUapiCcaRxGetInfo(port);
            break;
        case AvFsmCcaRxLinkConnect:
            AvUapiCcaRxGetInfo(port);
            break;
        case AvFsmCcaRxLinkStable:
            AvUapiCcaRxGetInfo(port);
            break;
        case AvFsmCcaRxSetupChain:
            AvUapiCcaRxGetInfo(port);
            break;
        case AvFsmCcaRxUnlock:
            AvUapiCcaRxGetInfo(port);
            break;
        default:
            break;
    }
    return;
}
#endif /* AvEnableCcaFeature */


/* Part 12 VideoGen */
void KfunCheckVideoGen(AvPort *port)
{
    /* Call Uapi Function */
#if GSV2K11
    if(((port->device->type == Gsv2k11)&&(port->content.RouteVideoToPort != NULL))||(port->device->type != Gsv2k11))
#endif
    AvUapiCheckVideoGen(port);
}

/* Part 13 AudioGen */
void KfunCheckAudioGen(AvPort *port)
{
    /* Call Uapi Function */
#if GSV2K11
    if((port->device->type == Gsv2k11)&&(port->content.RouteVideoToPort != NULL))
        AvUapiCheckAudioGen(port);
#endif
}

/* Part 14 ClockGen */
void KfunCheckClockGen(AvPort *port)
{
    /* Call Uapi Function */
#if GSV2K11
    if((port->device->type == Gsv2k11)||(port->device->type == Gsv2k1))
        AvUapiCheckClockGen(port);
#endif
}

#endif

