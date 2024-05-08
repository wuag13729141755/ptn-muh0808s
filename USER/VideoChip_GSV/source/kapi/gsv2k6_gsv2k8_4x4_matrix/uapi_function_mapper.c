/**
 * @file uapi_function_mapper.c
 *
 * @brief low level ports related universal api functions
 */
#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES

#include "av_config.h"
#include "Gsv_kapi.h"
#include "Gsv_uapi.h"
#include "Gsv_hal.h"
#include "av_event_handler.h"
#include "uapi_function_mapper.h"
#include "kernel_status_update.h"

#if AvEnableCecFeature /* CEC Related */
extern const uint8 FlexOpCodes[];
extern const uint8 CecOpCodes[];
#endif
#if AvForceDefaultEdid
extern const uint8 AvDefaultEDID[];
#endif
#if GSV5K1
extern uint8 TrxAvUpStreamFlag; /* 1 = Tx, 0 = Rx */
extern const AvVesaTables VesaTimingTables[];
extern const AvCeaTables CeaTimingTables[];
#endif
vpstu8AvPortTxEdidPoint pvpstu8FuncGsvUserEdidSaveFunction = NULL;

/* PART 1 */
/* Receiver FSM */

/**
 * @brief
 * function to Initialize Rx port parameters, just like following script configuration
 * @return AvOk - success
 * @note
 */
void KfunRxVarInit(pin AvPort *port)
{
    AvUapiRxPortInit(port);
    AvMemset(port->content.video, 0, sizeof(AvVideo));
    AvMemset(port->content.audio, 0, sizeof(AvAudio));
#if AvEnableSimplifyHdcp
    /* when using AvEnableSimplifyHdcp, HDCP FSM will be disabled, so HDCP init is placed here. */
    KfunHdcpReset(port);
    KfunHdcpVarInit(port);
#endif
    return;
}

/**
 * @brief
 * function to Initialize Rx port, just like following script configuration
 * @return AvOk - success
 * @note
 */
void KfunRxInit(pin AvPort *port)
{
    return;
}

void KfunTxfromRxPacketContent(AvPort *RxPort, AvPort *TxPort, uint8 *PktContent, uint16 AvBit,
                               PacketType AvPacket, AvPacketType type, AvRxUpdateType update)
{
    if(type == AvVideoPacketType)
    {
        if(((RxPort->content.video->AvailableVideoPackets & AvBit) != 0) || (update == AvRxForce))
        {
            if((update == AvRxUpdate) || (update == AvRxForce))
            {
                AvUapiRxGetPacketContent(RxPort, AvPacket, PktContent);
                KfunUpdatePortFromPacket(RxPort, AvPacket, PktContent);
            }
            KfunCheckVspPortContent(TxPort, AvPacket, PktContent);
            KfunUpdatePortFromPacket(TxPort, AvPacket, PktContent);
            AvUapiTxSetPacketContent(TxPort, AvPacket, PktContent, 1);
            TxPort->content.video->AvailableVideoPackets =
                TxPort->content.video->AvailableVideoPackets | AvBit;
        }
        else
        {
            AvUapiTxEnableInfoFrames(TxPort, AvBit, false);
            TxPort->content.video->AvailableVideoPackets =
                TxPort->content.video->AvailableVideoPackets & (~AvBit);
        }
    }
    else if(type == AvAudioPacketType)
    {
        if(((RxPort->content.audio->AvailableAudioPackets & AvBit) != 0) || (update == AvRxForce))
        {
            if((update == AvRxUpdate) || (update == AvRxForce))
            {
                AvUapiRxGetPacketContent(RxPort, AvPacket, PktContent);
                KfunUpdatePortFromPacket(RxPort, AvPacket, PktContent);
            }
            KfunCheckVspPortContent(TxPort, AvPacket, PktContent);
            KfunUpdatePortFromPacket(TxPort, AvPacket, PktContent);
            AvUapiTxSetPacketContent(TxPort, AvPacket, PktContent, 1);
            TxPort->content.audio->AvailableAudioPackets =
                TxPort->content.audio->AvailableAudioPackets | AvBit;
        }
        else
        {
            AvUapiTxEnableInfoFrames(TxPort, AvBit, false);
            TxPort->content.audio->AvailableAudioPackets =
                TxPort->content.audio->AvailableAudioPackets & (~AvBit);
        }
    }
}

void KfunRxtoTxPacketContent(AvPort *port, uint8 *PktContent, uint16 AvBit,
                             PacketType AvPacket, AvPacketType type)
{
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    if(type == AvVideoPacketType)
    {
        port->content.rx->ChangedVideoPackets =
            port->content.rx->ChangedVideoPackets & (~AvBit);
        if((port->content.video->AvailableVideoPackets & AvBit) != 0)
        {
            AvUapiRxGetPacketContent(port, AvPacket, PktContent);
            KfunUpdatePortFromPacket(port, AvPacket, PktContent);
        }
        while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
                KfunTxfromRxPacketContent(port, CurrentPort, PktContent, AvBit, AvPacket, type, AvRxUpdate);
            PrevPort = CurrentPort;
            CurrentPort = NULL;
        }
    }
    else if(type == AvAudioPacketType)
    {
        port->content.rx->ChangedAudioPackets =
            port->content.rx->ChangedAudioPackets & (~AvBit);
        if((port->content.audio->AvailableAudioPackets & AvBit) != 0)
        {
            AvUapiRxGetPacketContent(port, AvPacket, PktContent);
            KfunUpdatePortFromPacket(port, AvPacket, PktContent);
        }
        //while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
                KfunTxfromRxPacketContent(port, CurrentPort, PktContent, AvBit, AvPacket, type, AvRxUpdate);
            PrevPort = CurrentPort;
            CurrentPort = NULL;
        }
    }
}

/**
 * @brief
 * function to Manage HDMI Rx Video Interrupts
 * @return AvOk - success
 * @note
 */
void KfunRxVideoManage(pin AvPort *port)
{
    /* receive and manage the video packet content */
    AvRet ret;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    uint8 PktContent[40];
    VideoInterrupt Intpt;

    /* Don't output unless fully Stable and RxReadInfo is done */
    if(port->content.rx->IsFreeRun == 0)
        return;

    /* Step 1. Get the Packet Status */
    ret = AvUapiRxGetVideoPacketStatus(port, &Intpt);
    /* Step 1.1 Clear Interrupt */
    AvUapiRxClearVideoInterrupt(port, &Intpt);

    /* Step 2. Process it if existed */
    if((ret == AvOk) && ((Intpt.VsyncLck == 0)&&(port->device->type != Gsv5k1)) && (Intpt.DeRegenLck == 0) && (Intpt.NewTmds == 0) && (Intpt.BadTmdsClk == 0))
    {
        if(Intpt.AvMute)
        {
            AvUapiRxGetAvMute(port);
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
                {
                    CurrentPort->content.video->Mute.AvMute = port->content.video->Mute.AvMute;
                    AvUapiOutputDebugMessage("from Intpt.AvMute-->");
                    AvUapiTxSetAvMute(CurrentPort);
                    CurrentPort->content.video->AvailableVideoPackets =
                        CurrentPort->content.video->AvailableVideoPackets | AV_BIT_GC_PACKET;
                }
                PktContent[0] = 1;
                AvUapiTxSetPacketContent(CurrentPort, AV_PKT_GC_PACKET, PktContent, 1);
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
        if(Intpt.HdmiModeChg)
        {
            AvUapiRxGetHdmiModeSupport(port);
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
                {

                #if GSV5K1
                    if(port->device->type == Gsv5k1)
                    {
                        if(CurrentPort->content.tx->InfoReady > TxVideoManageThreshold)
                        {
                            CurrentPort->content.tx->Hpd = AV_HPD_FORCE_LOW;
                        }
                        else
                        {
                            #if 1//(AvGsv2kxTXSwitchHDMIAndDVI==0)
                            CurrentPort->content.tx->HdmiMode = port->content.rx->HdmiMode;
                            #endif
                            AvUapiTxSetHdmiModeSupport(CurrentPort);
                        }
                    }
                    else
                #endif
                    if(CurrentPort->content.tx->EdidReadSuccess == AV_EDID_RESET)
                    {
                    #if (AvGsv2kxTXSwitchHDMIAndDVI==0)
                        CurrentPort->content.tx->HdmiMode = port->content.rx->HdmiMode;
                    #endif
                        AvUapiTxSetHdmiModeSupport(CurrentPort);
                    }
                }
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
        if(Intpt.DeRegenLck)
        {
        }
        if(Intpt.VsyncLck)
        {
        }
        if(Intpt.Vid3dDet)
        {
            AvUapiRxGetVideoTiming(port);
        }
        if(Intpt.NewTmds)
        {
        }
        if(Intpt.BadTmdsClk)
        {
        }
        if(Intpt.DeepClrChg)
        {
            AvUapiRxGetHdmiDeepColor(port);
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
                {
                    KfunTxSetColorDepth(port, CurrentPort);
                    AvUapiTxSetHdmiDeepColor(CurrentPort);
                }
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
        if(Intpt.PktErr)
        {
        }
        if((Intpt.AvIfValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_AV_INFO_FRAME) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_AV_INFO_FRAME,
                                    AV_PKT_AV_INFO_FRAME, AvVideoPacketType);
        }
        if((Intpt.SpdValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_SPD_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_SPD_PACKET,
                                    AV_PKT_SPD_PACKET, AvVideoPacketType);
        }
        if((Intpt.HdrValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_HDR_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_HDR_PACKET,
                                    AV_PKT_HDR_PACKET, AvVideoPacketType);
        }
#if 0
        if((Intpt.EmpValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_EMP_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_EMP_PACKET,
                                    AV_PKT_EMP_PACKET, AvVideoPacketType);
        }
#endif
        if((Intpt.MsValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_MPEG_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_MPEG_PACKET,
                                    AV_PKT_MPEG_PACKET, AvVideoPacketType);
        }
        if((Intpt.VsValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_VS_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_VS_PACKET,
                                    AV_PKT_VS_PACKET, AvVideoPacketType);
        }
        if((Intpt.Isrc1Valid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_ISRC1_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_ISRC1_PACKET,
                                    AV_PKT_ISRC1_PACKET, AvVideoPacketType);
        }
        if((Intpt.Isrc2Valid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_ISRC2_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_ISRC2_PACKET,
                                    AV_PKT_ISRC2_PACKET, AvVideoPacketType);
        }
        if((Intpt.GamutValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_GMD_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_GMD_PACKET,
                                    AV_PKT_GMD_PACKET, AvVideoPacketType);
        }
        if((Intpt.GcValid) ||
           ((port->content.rx->ChangedVideoPackets & AV_BIT_GC_PACKET) != 0))
        {
            port->content.rx->ChangedVideoPackets =
                port->content.rx->ChangedVideoPackets & (~AV_BIT_GC_PACKET);
            if((port->content.video->AvailableVideoPackets & AV_BIT_GC_PACKET) != 0)
            {
                AvUapiRxGetAvMute(port);
                AvUapiRxGetHdmiDeepColor(port);
            }
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
                {
                    if((port->content.video->AvailableVideoPackets & AV_BIT_GC_PACKET) != 0)
                    {
                        KfunTxSetColorDepth(port, CurrentPort);
                        AvUapiTxSetHdmiDeepColor(CurrentPort);
                        CurrentPort->content.video->Mute.AvMute = port->content.video->Mute.AvMute;
                        AvUapiOutputDebugMessage("from Intpt.GcValid-->");
                        AvUapiTxSetAvMute(CurrentPort);
                        CurrentPort->content.video->AvailableVideoPackets =
                            CurrentPort->content.video->AvailableVideoPackets | AV_BIT_GC_PACKET;
                        PktContent[0] = 1;
                        AvUapiTxSetPacketContent(CurrentPort, AV_PKT_GC_PACKET, PktContent, 1);
                    }
                    else
                    {
                        CurrentPort->content.video->AvailableVideoPackets =
                            CurrentPort->content.video->AvailableVideoPackets & (~AV_BIT_GC_PACKET);
                        PktContent[0] = 0;
                        AvUapiTxSetPacketContent(CurrentPort, AV_PKT_GC_PACKET, PktContent, 1);
                    }
                }
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
    }

    /* Step 3. Call Layer */
    AvUapiRxVideoManage(port, &Intpt);

}

/**
 * @brief
 * function to Manage HDMI Rx Audio Interrupts
 * @return AvOk - success
 * @note
 */
void KfunRxAudioManage(pin AvPort *port)
{
    AvRet ret;
    /* receive and manage the audio packet content */
    uint8 PktContent[40];
    AvPort* PrevPort = NULL;
    AvPort* CurrentPort = NULL;
    AudioInterrupt Intpt;
    uint8 TxClearAudioContentFlag = 0;

    /* Don't output unless fully Stable and RxReadInfo is done */
    if((port->content.rx->IsFreeRun == 0) || (port->content.rx->Lock.AudioLock == 0))
        return;

    /* Step 1. Get the Packet Status */
    ret = AvUapiRxGetAudioPacketStatus(port, &Intpt);
    /* Step 1.1 Clear Interrupt */
    AvUapiRxClearAudioInterrupt(port, &Intpt);

    /* Step 2. Process it if existed */
    if(ret == AvOk)
    {
        if(Intpt.AudChanMode)
        {
            Intpt.AudIfValid = 1;
            Intpt.CtsThresh = 1;
            Intpt.CsDataValid = 1;
        }
        if(Intpt.InternMute)
        {
            AvUapiRxGetAudioInternalMute(port); /* only detect AV MUTE and Audio Mute */
            AvUapiRxSetAudioInternalMute(port);
        }
        if(Intpt.CtsThresh)
        {
            AvUapiRxGetHdmiAcrInfo(port);
        }
        if(Intpt.AudFifoOv)
        {
        }
        if(Intpt.AudFifoUn)
        {
        }
        if(Intpt.AudFifoNrOv)
        {
        }
        if(Intpt.AudioPktErr)
        {
        }
        if(Intpt.AudModeChg)
        {
            /* when audio mode is changed, always resend audio */
            TxClearAudioContentFlag = 1;
            AvUapiRxGetPacketType(port);
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                CurrentPort->content.audio->AudType = port->content.audio->AudType;
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
        if(Intpt.AudFifoNrUn)
        {
        }
        if(Intpt.AudFlatLine)
        {
            AvUapiRxGetAudioInternalMute(port);
            AvUapiRxSetAudioInternalMute(port);
        }
        if(Intpt.AudSampChg)
        {
            /* when audio mode is changed, always resend audio */
            TxClearAudioContentFlag = 1;
            AvUapiRxGetHdmiAcrInfo(port);
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                CurrentPort->content.audio->NValue = port->content.audio->NValue;
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
        if(Intpt.AudPrtyErr)
        {
        }
        if((Intpt.AcpValid) ||
           ((port->content.rx->ChangedAudioPackets & AV_BIT_ACP_PACKET) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_ACP_PACKET, AV_PKT_ACP_PACKET,
                                    AvAudioPacketType);
        }

        if((Intpt.CsDataValid) ||
           ((port->content.rx->ChangedAudioPackets & AV_BIT_AUDIO_CHANNEL_STATUS) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_AUDIO_CHANNEL_STATUS,
                                    AV_PKT_AUDIO_CHANNEL_STATUS, AvAudioPacketType);
        }
        if((Intpt.AudIfValid) ||
           ((port->content.rx->ChangedAudioPackets & AV_BIT_AUDIO_INFO_FRAME) != 0))
        {
            KfunRxtoTxPacketContent(port, PktContent, AV_BIT_AUDIO_INFO_FRAME,
                                    AV_PKT_AUDIO_INFO_FRAME, AvAudioPacketType);
        }
        if(Intpt.NChange)
        {
            AvUapiRxGetHdmiAcrInfo(port);
            PrevPort = NULL;
            CurrentPort = NULL;
            while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                CurrentPort->content.audio->NValue = port->content.audio->NValue;
                AvUapiTxSetAudNValue(CurrentPort);
                PrevPort = CurrentPort;
                CurrentPort = NULL;
            }
        }
    }

    /* Step 3. Rx Audio has important change, update Tx again */
    if(TxClearAudioContentFlag == 1)
    {
        PrevPort = NULL;
        CurrentPort = NULL;
        while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            CurrentPort->content.audio->AvailableAudioPackets = 0;
            PrevPort = CurrentPort;
            CurrentPort = NULL;
        }
    }

    /* Step 4. Call Silicon Layer */
    AvUapiRxAudioManage(port, &Intpt);

}

/**
 * @brief
 * function to Enable HDMI Rx Freerun
 * @return AvOk - success
 * @note
 */
void KfunRxEnableFrun(pin AvPort *port)
{
    AvUapiRxEnableFreeRun(port, true);
    return;
}

/**
 * @brief
 * function to Disable HDMI Rx Freerun
 * @return AvOk - success
 * @note
 */
void KfunRxDisableFrun(pin AvPort *port)
{
    AvUapiRxEnableFreeRun(port, false);
    return;
}

/**
 * @brief
 * function to Poll Rx Status to update Rx Port vars
 * @return AvOk - success
 * @note
 */
void KfunPollingRxStatus(pout AvPort *port)
{
    /* to update the current port status */
    /* AvUapiRxGetStatus(port); */
    return;
}

/**
 * @brief
 * function to Clear HDMI Rx Flags in Port Vars
 * @return AvOk - success
 * @note
 */
void KfunRxClearFlags(pin AvPort *port)
{
    AvUapiRxClearFlags(port);
    return;
}

/* PART 2 */
/* HDCP FSM */
/**
 * @brief
 * function to Initialize HDCP Variables in Port structure
 * @return AvOk - success
 * @note
 */
void KfunHdcpVarInit(pin AvPort *port)
{
    AvHandleEvent(port, AvEventRxSetHdcpStyle, NULL, NULL);
    AvUapiRxSetHdcpEnable(port);
    return;
}

/**
 * @brief
 * function to Reset HDCP Variables in Port structure
 * @return AvOk - success
 * @note
 */
void KfunHdcpReset(pin AvPort *port)
{
    port->content.hdcp->SinkNumber = 0;
    port->content.hdcp->SinkTotal = 0;
    port->content.hdcp->BksvPtr = 0;
    port->content.hdcp->AksvInterrupt = 0;
    AvUapiRxSetHdcpMode(port);
    AvUapiHdcp2p2Mode(port);
    AvUapiOutputDebugMessage("Port %d: HDCP Reset to REC Mode", port->index);
    return;
}

/**
 * @brief
 * function to Detect Source in HDCP Connection Chain in Port structure
 * @return AvOk - success
 * @note
 */
void KfunHdcpDetectSource(pin AvPort *port)
{
    AvRet ret;
    HdcpInterrupt Intpt;
    AvPort* FrontEnd = (AvPort *)port->content.HdcpSource;

    /* Front End processing for HdmiRx*/
    if((port->type == HdmiRx) || (port->type == CcaRx))
    {
        /* If HdcpNeeded = 0, HDCP is not needed and supported */
        if(port->content.hdcp->HdcpNeeded != AV_HDCP_RX_NOT_SUPPORT)
        {
            /* front end */
            /* or not connected, you don't need to consider hdcp */
            if(!FrontEnd)
            {
                /* 1. Get Rx Status */
                ret = AvUapiRxGetHdcpStatus(port, &Intpt);

                /* 2. Interpret interrupt information */
                if(ret == AvOk)
                {
                    if(Intpt.AksvUpdate)
                    {
                    }
                    if(Intpt.Encrypted)
                    {
                        if(port->content.rx->VideoEncrypted)
                            AvHandleEvent(port, AvEventPortUpStreamEncrypted, NULL, NULL);
                        else
                            AvHandleEvent(port, AvEventPortUpStreamDecrypted, NULL, NULL);
                    }
                }

                /* 3. Clear Interrupt */
                AvUapiRxClearHdcpInterrupt(port, &Intpt);

                /* 4. Hdcp Rx Error Manage */
                if((port->content.rx->VideoEncrypted == 1) &&
                   (port->content.rx->Lock.EqLock == 1) &&
                   (port->content.rx->IsInputStable == 0))
                    port->content.hdcp->HdcpError = port->content.hdcp->HdcpError + 1;
                if(port->content.hdcp->HdcpError >= AvHdcpRxErrorThreshold)
                {
                    port->content.rx->Hpd = AV_HPD_TOGGLE;
                    port->content.hdcp->HdcpError = 0;
                }
            }
        }
    }
}

/**
 * @brief
 * function to Detect Sinks in HDCP Connection Chain in Port structure
 * @return AvOk - success
 * @note
 */
void KfunHdcpDetectSink(pin AvPort *port)
{
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    port->content.hdcp->SinkTotal = 0;

    /* Summarize all the Sinks that support Hdcp */

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            /* Get Tx Hdcp Status */
            AvUapiTxGetHdcpStatus(CurrentPort);
            /* Support HDCP */
            if((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 1))
            {
                /* Increase Sink Total Number */
                port->content.hdcp->SinkTotal++;
            }
                        /* Change HDCP Tx Encryption Version */
            #if AvEnableHdcpMatchInput
            if(port->content.rx->VideoEncrypted == 1)
            {
                /* 2.2 INPUT -> release 1.4 ONLY */
                if(port->content.hdcp->Hdcp2p2RxRunning == 1)
                {
                    if(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_1P4_FAIL_OUT)
                    {
                        CurrentPort->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO_FAIL_OUT;
                        CurrentPort->content.hdcptx->HdcpModeUpdate = 1;
                    }
                    else if(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_1P4_ONLY)
                    {
                        CurrentPort->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO;
                        CurrentPort->content.hdcptx->HdcpModeUpdate = 1;
                    }
                }
                /* 1.4 INPUT -> restrict 2.2 SUPPORT */
                else
                {
                    if(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_AUTO_FAIL_OUT)
                    {
                        CurrentPort->content.hdcptx->HdmiStyle = AV_HDCP_TX_1P4_FAIL_OUT;
                        CurrentPort->content.hdcptx->HdcpModeUpdate = 1;
                    }
                    else if(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_AUTO)
                    {
                        CurrentPort->content.hdcptx->HdmiStyle = AV_HDCP_TX_1P4_ONLY;
                        CurrentPort->content.hdcptx->HdcpModeUpdate = 1;
                    }
                }
            }
            #endif
        }

        PrevPort = CurrentPort;
    }
}

#if AvEnableSimplifyHdcp
void KfunSimpleHdcpSync(pin AvPort *port)
{
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    #if AvMatrix4x4Mode
    /* Don't check Hdmi Rx Port that is not the 1st stage */
    if(port->content.RouteVideoFromPort != NULL)
        return;
    #endif

    port->content.hdcp->SinkTotal = 0;

    /* Summarize all the Sinks that support Hdcp */
    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if(CurrentPort->type == HdmiTx)
        {
            #if AvEnableForceOutHdcp
            if((CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_1P4_ONLY)
                //||(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_1P4_FAIL_OUT)
                ||(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_2P2_ONLY)
                //||(CurrentPort->content.hdcptx->HdmiStyle == AV_HDCP_TX_2P2_FAIL_OUT)
                )
            {
                /* enable sink hdcp */
                if(port->type == HdmiRx)
                {
                    if((*(port->content.is_PlugRxFsm)        == AvFsmPlugRxStable) &&
                       (*(CurrentPort->content.is_PlugTxFsm) == AvFsmPlugTxStable) &&
                       (CurrentPort->content.tx->InfoReady   >= TxHdcpManageThreshold))
                    {
                        AvUapiTxEncryptSink(CurrentPort);
                    }
                }
                else if(port->type == LogicVideoRx)
                {
                    if((*(CurrentPort->content.is_PlugTxFsm) == AvFsmPlugTxStable) &&
                       (CurrentPort->content.tx->InfoReady   >= TxHdcpManageThreshold))
                    {
                        AvUapiTxEncryptSink(CurrentPort);
                    }
                }
            }
            else
            #endif
            {
                /* enable sink hdcp */
                if(port->type == HdmiRx)
                {
                    if((port->content.rx->VideoEncrypted == 1) &&
                       (*(port->content.is_PlugRxFsm)        == AvFsmPlugRxStable) &&
                       (*(CurrentPort->content.is_PlugTxFsm) == AvFsmPlugTxStable) &&
                       (CurrentPort->content.tx->InfoReady   >= TxHdcpManageThreshold))
                    {
                        AvUapiTxEncryptSink(CurrentPort);
                    }
                }
                else if(port->type == LogicVideoRx)
                {
                    if(/*(port->content.rx->VideoEncrypted == 1) &&*/
                       (*(CurrentPort->content.is_PlugTxFsm) == AvFsmPlugTxStable) &&
                       (CurrentPort->content.tx->InfoReady   >= TxHdcpManageThreshold))
                    {
                        AvUapiTxEncryptSink(CurrentPort);
                    }
                }
                /* disable sink hdcp */
                if(port->content.rx->VideoEncrypted == 0)
                {
                    if(CurrentPort->content.tx->HdmiMode == 0)
                    {
                        if(CurrentPort->content.video->Mute.BlkMute == 1)
                        {
                            CurrentPort->content.video->Mute.BlkMute = 0;
                            AvUapiTxSetBlackMute(CurrentPort);
                        }
                    }
                    else if(CurrentPort->content.hdcptx->HdcpEnabled == 1)
                    {
                        AvUapiTxDecryptSink(CurrentPort);
                    }
                }
            }
        }

        PrevPort = CurrentPort;
    }
}
#endif

void KfunTxUnlockProtection(pio AvPort *port)
{
    port->content.video->AvailableVideoPackets = 0;
    port->content.audio->AvailableAudioPackets = 0;
    port->content.tx->InfoReady = 0;
    AvUapiTxDisableCore(port);
    AvUapiTxEnableCore(port);

    return;
}

/**
 * @brief
 * function to Read Bksv in All HDCP Sinks of HDCP Chain Connection
 * @return AvOk - success
 * @note
 */
void KfunReadBksv(pin AvPort *port)
{
#if 0
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    if(port->content.hdcp->Hdcp2p2Flag == 1)
    {
#if AvEnableHdcp2p2Feature
        /* process to copy Rxid from Tx to Rx */
#endif
        return;
    }
    else
    {
        /* clear BKSV ready */
        AvUapiRxSetBksvListReady(port);

        while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            /* Check whether there are missing sinks or added sinks */
            if(CurrentPort->type == HdmiTx)
            {
                /* Support HDCP */
                if((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 1))
                {
                    /* Bksv flag may be fast or slow, setting GetBksvReady = AvOk
                       means it has arrived, setting GetBksvReady = AvInvalidParameter
                       means Bksv has already been given, no need to get it again */
                    AvRet value = AvUapiTxGetBksvReady(CurrentPort);
    #if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("Fast Copying Bksv Start!!!");
    #endif
                    if(value == AvOk)
                    {
    #if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("Fast Copying Bksv Action!!!");
    #endif
                        /* Copy Bksv */
                        KfunCopyBksv(port, CurrentPort);
                        /* Mute Output */
                        KfunTxSetMuteAv(CurrentPort);
                        /* Clear Bksv Interrupt */
                        AvUapiTxClearBksvReady(CurrentPort);
                        /* Increase Sink Total Number */
                        port->content.hdcp->SinkNumber++;
                    }
    #if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("Fast Copying Bksv End!!!");
    #endif
                }
            }

            PrevPort = CurrentPort;
        }

        /* clear BKSV ready */
        AvUapiRxSetBksvListReady(port);
    }
#else
    UNUSED(port);
#endif

    return;
}

/**
 * @brief
 * function to look up HDCP Source in the HDCP connection
 * @return AvOk - success
 * @note
 */
AvPort *KfunLookupHdcpSource(pin AvPort *port)
{
    return (AvPort*)(port->content.HdcpSource);
}

/**
 * @brief
 * function to upload Sink information to Source
 * @return AvOk - success
 * @note
 */
void KfunUploadSinkInfo(pin AvPort *port)
{
    /* Find front end device and upload it */
    return;
}

/**
 * @brief
 * function to copy Bksv to Rx Bksv Pool
 * @return AvOk - success
 * @note
 */
AvRet KfunCopyBksv(pout AvPort *RxPort, pin AvPort *TxPort)
{
    AvRet ret = AvOk;
#if 0
    uint8  BksvTotal;
    uint8  BksvCount;
    uint8  TempBksv[5];

    /* The Bksv number when while finished */
    AvUapiTxGetBksvTotal(TxPort, &BksvTotal);
    BksvCount = 0;
    while(BksvCount <= BksvTotal)
    {
    AvUapiOutputDebugMessage("Copying Bksv...");
    /* Read one Bksv once, BksvCount should be decreased by uapi */
    AvUapiTxReadBksv(TxPort, TempBksv, BksvCount);
    BksvCount++;
    /* upload to Front end */
    if(AvUapiRxCheckBksvExisted(RxPort, TempBksv) == AvNotAvailable)
        AvUapiRxAddBksv(RxPort, TempBksv,
                        RxPort->content.hdcp->BksvPtr++);
    }
#else
    UNUSED(RxPort);
    UNUSED(TxPort);
#endif
    return ret;
}

/**
 * @brief
 * function to check HDCP source encryption status
 * @return AvOk - success
 * @note
 */
void KfunCheckEncryption(pin AvPort *port)
{
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            if(port->content.video->Mute.AvMute != CurrentPort->content.video->Mute.AvMute)
            {
                CurrentPort->content.video->Mute.AvMute = port->content.video->Mute.AvMute;
                AvUapiTxSetAvMute(CurrentPort);
            }
        }
        PrevPort = CurrentPort;
    }
    return;
}

/**
 * @brief
 * function to check HDCP sink encryption
 * @return AvOk - success
 * @note
 */
void KfunEncryptSink(pin AvPort *port)
{
#if 0
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    if(port->content.hdcp->Hdcp2p2Flag == 0)
    {
        AvUapiRxSetHdcpMode(port);
        AvUapiRxSetBksvListReady(port);
    }

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if(CurrentPort->type == HdmiTx)
        {
            /* Support HDCP */
            if((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport))
            {
                AvUapiTxEncryptSink(CurrentPort);
            }
            if(port->content.hdcp->Hdcp2p2Flag == 1)
            {
                CurrentPort->content.hdcptx->Hdcp2p2Flag = 1;
                AvUapiTxEncryptSink(CurrentPort);
            }
        }

        PrevPort = CurrentPort;
    }
#else
    UNUSED(port);
#endif
}

/**
 * @brief
 * function to decrypt HDCP sink encryption
 * @return AvOk - success
 * @note
 */
void KfunDecryptSink(pin AvPort *port)
{
#if 0
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if(CurrentPort->type == HdmiTx)
        {
            /* Support HDCP */
            if((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 1))
            {
                CurrentPort->content.hdcptx->HdcpEnabled = 0;
                CurrentPort->content.hdcptx->Authenticated = 0;
                CurrentPort->content.hdcptx->Hdcp2p2Flag = 0;
                AvUapiTxDecryptSink(CurrentPort);
            }
        }

        PrevPort = CurrentPort;
    }
#else
    UNUSED(port);
#endif
}

/**
 * @brief
 * function to decrypt new HDCP sink encryption
 * @return AvOk - success
 * @note
 */
void KfunDecryptNewSink(pin AvPort *port)
{
#if 0
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            /* Support HDCP */
            if(((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 1)) &&
               (CurrentPort->content.hdcptx->Authenticated == 0) &&
               (CurrentPort->content.hdcptx->BksvReady == 1))
            {
                AvUapiTxDecryptSink(CurrentPort);
                CurrentPort->content.hdcptx->HdcpEnabled = 0;
            }
        }

        PrevPort = CurrentPort;
    }
#else
    UNUSED(port);
#endif
}

/**
 * @brief
 * function to remove All AvMute in HDCP Txs of HDCP Connection
 * @return AvOk - success
 * @note
 */
void KfunHdcpUnMuteAv(pio AvPort *port)
{
#if 0
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            /* Support HDCP */
            if(((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 1))&&
               (CurrentPort->content.hdcptx->Authenticated == 0))
            {
                KfunTxClearMuteAv(CurrentPort);
            }
        }
        PrevPort = CurrentPort;
    }
#else
    UNUSED(port);
#endif

    return;
}

/**
 * @brief
 * function to set All AvMute in HDCP Tx of HDCP Connection
 * @return AvOk - success
 * @note
 */
void KfunTxSetMuteAv(pio AvPort *port)
{
    /* AvMute */
    if(port->content.tx->HdmiMode)
    {
        port->content.video->Mute.AvMute = 1;
#if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("from Kfun TX SetMuteAv-->");
#endif
        AvUapiTxSetAvMute(port);
        /* BlackMute */
        port->content.video->Mute.BlkMute = 1;
        AvUapiTxSetBlackMute(port);
    }
    return;
}

/**
 * @brief
 * function to clear All AvMute in HDCP Tx of HDCP Connection
 * @return AvOk - success
 * @note
 */
void KfunTxClearMuteAv(pio AvPort *port)
{
    /* AvMute */
    if(port->content.tx->HdmiMode)
    {
        port->content.video->Mute.AvMute = 0;
#if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("from Kfun TX ClearMuteAv-->");
#endif
        AvUapiTxSetAvMute(port);
        /* BlackMute */
        port->content.video->Mute.BlkMute = 0;
        AvUapiTxSetBlackMute(port);
    }
    return;
}

/**
 * @brief
 * function to mute Av in HDCP sinks of connection chain
 * @return AvOk - success
 * @note
 */
void KfunHdcpMuteAv(pio AvPort *port)
{
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            /* Support HDCP */
            if(((CurrentPort->content.hdcptx->HdcpSupport == 1) || (CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 1)) &&
               (CurrentPort->content.hdcptx->Authenticated == 0))
            {
                KfunTxSetMuteAv(CurrentPort);
            }
        }
        PrevPort = CurrentPort;
    }
}

/**
 * @brief
 * function to manage HDCP status of HDMI Tx
 * @return AvOk - success
 * @note
 */
void KfunTxHdcpManage(pio AvPort *port)
{
    if(port->content.hdcptx->HdcpEnabled)
    {
        if(port->content.hdcptx->BksvReady == 1)
        {
            AvUapiTxClearBksvReady(port);
        }
#if AvEnableHdcp2p2Feature
        AvUapiTxHdcp2p2Manage(port);
#endif

    }
    return;
}

/* PART 3 */
/* Transmitter FSM */

/**
 * @brief
 * function to Mute Tx TMDS
 * @return AvOk - success
 * @note
 */
void KfunTxTmdsMute(pout AvPort *port)
{
    AvUapiTxMuteTmds(port, true);
    return;
}

/**
 * @brief
 * function to UnMute Tx TMDS
 * @return AvOk - success
 * @note
 */
void KfunTxTmdsUnmute(pout AvPort *port)
{
    AvUapiTxMuteTmds(port, false);
    return;
}

/**
 * @brief
 * function to Detect Hdmi Tx Status
 * @return AvOk - success
 * @note
 */
void KfunPollingTxStatus(pout AvPort *port)
{
    AvUapiTxDetectMode(port);
    return;
}

/* PART 4 */
/* Tx Routing FSM */

/**
 * @brief
 * function to Initialize Tx Routing Fsm Vars
 * @return AvOk - success
 * @note
 */
void KfunTxRoutingVarInit(pout AvPort *port)
{
    return;
}

/**
 * @brief
 * function to Initialize Tx Routing Fsm Vars
 * @return AvOk - success
 * @note
 */
void KfunRxConnection(pio AvPort *port)
{
    return;
    /* if(port != NULL) */
        /* update all members Hdcp from hardware (rx port only) */
        /* AvUapiRxGetHdcpStatus(port); */
}

/**
 * @brief
 * function to check Tx Connection status
 * @return AvOk - success
 * @note
 */
void KfunTxConnection(pio AvPort *port)
{
    if(port)
    {
        /* HDCP Detect Sink Number will only be done for HdmiRx Port */
        if((port->type == HdmiRx) || (port->type == CcaRx))
        {
            /* Input parameter is still Rx Port */
            /* Find the first TxPort */
            AvPort* TxPort = (AvPort*)(port->content.HdcpNextSinkPort);
            port->content.hdcp->SinkTotal = 0;
            port->content.hdcp->SinkNumber = 0;

            /* Step 1. Update SinkTotal and SinkNumber */
            while(TxPort)
            {
                AvUapiTxGetHdcpStatus(TxPort);
                /* find out whether this is a HDCP needed Tx */
                if((TxPort->content.hdcptx->HdcpSupport == 1) || (TxPort->content.hdcptx->Hdcp2p2SinkSupport == 1))
                    port->content.hdcp->SinkTotal++;
                /* Step 2. Look for the next TxPort */
                TxPort = (AvPort*)(TxPort->content.HdcpNextSinkPort);
            }
        }
    }

}

AvRet KfunTxSinkLost(AvPort *port)
{
    AvHandleEvent(port, AvEventPortDownStreamDisconnected, NULL, NULL);
    return AvOk;
}

/* PART 5 */
/* Plug RX FSM */

/**
 * @brief
 * function to Initialize Plug Rx Vars
 * @return AvOk - success
 * @note
 */
void KfunPrVarInit(pin AvPort *port)
{
    port->content.rx->Reset = 1;
    port->content.rx->EnableFreeRun = 1;
    port->content.rx->ForceFreeRun = 0;
    port->content.rx->IsFreeRun = 0;
    port->content.rx->IsInputStable = 0;
    port->content.rx->VideoEncrypted = 0;
    port->content.rx->EdidStatus = AV_EDID_RESET;
    port->content.rx->Hpd = AV_HPD_LOW;
    port->content.rx->HpdDelayExpire = 0;
    port->content.rx->Lock.AudioLock = 0;
    port->content.rx->Lock.DeRegenLock = 0;
    port->content.rx->Lock.PllLock = 0;
    port->content.rx->Lock.VSyncLock = 0;
    port->content.rx->HdmiMode = 0;

    return;
}

/**
 * @brief
 * function to Reset Plug Rx Port
 * @return AvOk - success
 * @note
 */
void KfunPrResetPort(pin AvPort *port)
{
    AvUapiResetPort(port);
    return;
}

/**
 * @brief
 * function to Plug Rx Hpd Down
 * @return AvOk - success
 * @note
 */
void KfunPrHpdDown(pin AvPort *port)
{
    #if AvMatrix4x4Mode
    if(port->device->type == Gsv2k8)
    {
        AvUapiRxSetHpdUp(port);
        port->content.rx->Hpd = AV_HPD_HIGH;
        port->content.rx->HpdDelayExpire = RxHpdDelayExpireThreshold;
    }
    else
    #endif
    {
        {
            AvUapiRxSetHpdDown(port);
            port->content.rx->Hpd = AV_HPD_LOW;
            port->content.rx->HpdDelayExpire = 0;
            port->content.hdcp->HdcpError = 0;
        }
    }
    return;
}

/**
 * @brief
 * function to Plug Rx Hpd Up
 * @return AvOk - success
 * @note
 */
void KfunPrHpdUp(pin AvPort *port)
{
    AvUapiRxSetHpdUp(port);
    port->content.rx->Hpd = AV_HPD_HIGH;
    port->content.rx->HpdDelayExpire = 0;
    return;
}

void KfunManageHpa(pin AvPort *port)
{
    AvPort *PrevPort = NULL;
    AvPort *CurrentPort = NULL;
    uint8 ValidSinkFlag = 0;

#if (AvAllowHpdLowEdidRead == 1)
    CurrentPort = port->device->port; /* Get 1st Port of the device */
    while((CurrentPort != NULL) && (CurrentPort->device->index == port->device->index))
    {
        if((CurrentPort->type == HdmiTx) && (CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
        {
            ValidSinkFlag = 1;
        }
        CurrentPort = (AvPort*)(CurrentPort->next);
    }
    CurrentPort = NULL;
#endif
#if AvForceDefaultEdid
    if(port->content.rx->EdidStatus == AV_EDID_NEEDUPDATE)
        port->content.rx->EdidStatus = AV_EDID_UPDATED;
#endif

    /* Toggle will cause HPD to Drive Low */
    if(port->content.rx->Hpd == AV_HPD_TOGGLE)
    {
        KfunPrHpdDown(port);
    }

    /* EDID Related HPA */
    /* wait until EDID is updated to assert HPA */
    if((port->content.rx->EdidStatus == AV_EDID_RESEND) ||
        (port->content.rx->EdidStatus == AV_EDID_UPDATED))
    {
        if(port->content.rx->Hpd == AV_HPD_LOW)
        {
            if((port->content.rx->HpdDelayExpire >= RxHpdDelayExpireThreshold)
                #if AvMatrix4x4Mode
                ||(port->device->type == Gsv2k8)
                #endif
                )
            {
                /* Only Pull UP HPA when Valid Sink is available */
                while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
                {
                    if((CurrentPort->type == HdmiTx) || (CurrentPort->type == DviTx))
                    {
                        if(CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED)
                            ValidSinkFlag = 1;
                    }
                    else if(CurrentPort->type == LogicVideoTx)
                    {
                        ValidSinkFlag = 1;
                    }
                    else if(CurrentPort->type == CcaTx)
                    {
                        ValidSinkFlag = 1;
                    }
                    PrevPort = CurrentPort;
                }
                /* Check Audio Output */
                PrevPort = NULL;
                CurrentPort = NULL;
                while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
                {
                    if(CurrentPort->type == LogicAudioTx)
                        ValidSinkFlag = 1;
                    PrevPort = CurrentPort;
                }
                #if AvMatrix4x4Mode
                /* Front End Gsv2k6 RxPort always enabled */
                if(port->device->type == Gsv2k6)
                    ValidSinkFlag = 1;
                #endif
                #if (AvEthCascading == 1)
                /* Cascading does not need to have local Hdmi Output */
                if(port->device->type == Gsv5k1)
                    ValidSinkFlag = 1;
                #endif
                /* Pull Up Action */
                if(ValidSinkFlag == 1)
                {
                    KfunPrHpdUp(port);
                    port->content.rx->EdidStatus = AV_EDID_UPDATED;
                }
            }
            #if AvAllowHpdLowPixelClockRead
            else if(port->content.rx->Input5V != 0U)
            {
                KfunPrHpdUp(port);
            }
            #endif
        }
    }
    else
    {
        if((port->content.rx->Hpd == AV_HPD_HIGH) ||
           (port->content.rx->Hpd == AV_HPD_FORCE_LOW))
            KfunPrHpdDown(port);
        else if(port->content.rx->Hpd == AV_HPD_FORCE_HIGH)
            KfunPrHpdUp(port);
    }
}

/**
 * @brief
 * function to Prepare Edid
 * @return AvOk - success
 * @note
 */
void KfunPrepareEdid(pin AvPort *port)
{
    if(port->content.rx->EdidStatus != AV_EDID_UPDATED)
        AvHandleEvent(port, AvEventRxPrepareEdid, NULL, NULL);
}

/**
 * @brief
 * function to Read Stdi Information
 * @return AvOk - success
 * @note
 */
void KfunStdiReadStatus(pin AvPort *port)
{
    AvUapiRxReadStdi(port);
    return;
}

/**
 * @brief
 * function to Read Rx Information
 * @return AvOk - success
 * @note
 */
void KfunRxReadInfo(pin AvPort *port)
{
    AvPort *PrevPort;
    AvPort *CurrentPort;
    AvUapiRxReadInfo(port);
    AvUapiRxGetVideoTiming(port);
#if GSV5K1
    if(port->device->type == Gsv5k1)
        KfunRxVicFilter(port);
#endif
    AvUapiRxGetHdmiDeepColor(port);
    PrevPort = NULL;
    CurrentPort = NULL;
    while(((KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)/*&&(port->device->type!=Gsv5k1)*/)
        #if 0//GSV5K1
        ||((KfunFindVideoNextNode(port, &PrevPort, &CurrentPort) == AvOk)&&(port->device->type==Gsv5k1))
        #endif
        )
    {
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            AvMemcpy(&(CurrentPort->content.video->timing),
                     &(port->content.video->timing),
                     sizeof(VideoTiming));
            KfunTxSetColorDepth(port, CurrentPort);
            AvUapiTxSetVideoTiming(CurrentPort);
        }
        PrevPort = CurrentPort;
    }
    return;
}

/* PART 6 */
/* Plug TX FSM */

/**
 * @brief
 * function to Plug Tx Fsm Var Initialization
 * @return AvOk - success
 * @note
 */
void KfunPtVarInit(pout AvPort *port)
{
    port->content.hdcptx->HdcpModeUpdate = 0;
    port->content.hdcptx->HdcpError = 0;
    port->content.hdcptx->BksvReady = 0;
    port->content.hdcptx->Authenticated = 0;
    port->content.hdcptx->HdcpSupport = 0;
    port->content.hdcptx->Hdcp2p2SinkSupport = 0;
    port->content.hdcptx->HdcpEnabled = 0;
    port->content.tx->Lock.PllLock = 0;
    port->content.tx->Lock.AudioLock = 0;
    port->content.tx->Lock.DeRegenLock = 0;
    port->content.tx->Lock.VSyncLock = 0;
    port->content.tx->InfoReady = 0;
    port->content.tx->EdidReadFail = 0;
    port->content.tx->EdidReadSuccess = AV_EDID_RESET;
    port->content.tx->IgnoreEdidError = 0; /* for test without EDID fun */
    port->content.tx->Hpd = AV_HPD_LOW; /* RELEASE_LOW */
    port->content.tx->HpdDelayExpire = 0;
    port->content.video->AvailableVideoPackets = 0;
    port->content.audio->AvailableAudioPackets = 0;
    AvHandleEvent(port, AvEventTxSetHdcpStyle, NULL, NULL);
    return;
}

/**
 * @brief
 * function to Plug Tx Port Initialization
 * @return AvOk - success
 * @note
 */
void KfunPtPortInit(pout AvPort *port)
{
    AvUapiTxPortInit(port);
    return;
}

/**
 * @brief
 * function to Reset Port for HDMI Tx Port
 * @return AvOk - success
 * @note
 */
void KfunPtResetPort(pout AvPort *port)
{
    /* no Tx Audio detection, enable it directly */
    port->content.tx->Lock.AudioLock = 1;
    port->content.tx->EdidReadFail = 0;
    port->content.tx->EdidReadSuccess = AV_EDID_RESET;
    port->content.tx->Hpd = AV_HPD_LOW; /* RELEASE_LOW */
    port->content.tx->InfoReady = 0;
    AvUapiResetPort(port);
#if AvEnableHdcp2p2Feature
    port->content.hdcptx->Hdcp2p2TxRunning = 0;
#endif
    return;
}

/**
 * @brief
 * function to Enable HDMI Tx Core
 * @return AvOk - success
 * @note
 */
void KfunPtEnableTxCore(pout AvPort *port)
{
    /* Low Freq Protection against Tx HPD */
    AvPort *SourcePort;
    AvUapiTxEnableCore(port);
    if(KfunFindVideoRxFront(port, &SourcePort) == AvOk)
    {
        if(SourcePort->content.rx->IsInputStable == 1)
            AvUapiTxSetVideoTiming(port);
    }
    return;
}

/**
 * @brief
 * function to do Anti Dither FSM in Plug Tx Fsm
 * @return AvOk - success
 * @note
 */
void KfunPtAntiDither(pout AvPort *port)
{
    /* Temp fix, should call event or setting timer to decide the delay */
    port->content.tx->HpdDelayExpire = 1;
    return;
}

/**
 * @brief
 * function to Manage Video in HDMI Tx
 * @return AvOk - success
 * @note
 */
AvRet KfunTxVideoManage(pout AvPort *port)
{
    AvRet ret;
    uint16 DifferentVideoPackets = 0;
    uint16 VspGenVideoPackets = 0;
    AvPort *SourcePort;
    AvPort *UpperPort;
    uint8 PktContent[40];
    uint8 UpdateSlot = 0;

#if (Gsv2k1MuxMode != 0)
    if(port->device->type == Gsv2k1)
    {
        /* InfoReady Delay Implement */
        if(port->content.tx->InfoReady <= TxHdcpManageThreshold)
        {
            port->content.tx->InfoReady = port->content.tx->InfoReady + 1;
        }
        return AvOk;
    }
#endif
    /* 1. Get Source information */
    /* if source does not exist, no need to update information at all */
    /* 2. if source exists, follow Rx's status, compare difference */
    /* Firstly carry bulk info from Front Port, then update the port */
    /* 3. if source port is vsp port, update information accordingly */
    if(KfunFindVideoRxFront(port, &SourcePort) == AvOk)
    {
        #if AvMatrix4x4Mode
        /* Use the information of Rx in the same device */
        UpperPort = (AvPort*)port->content.RouteVideoFromPort;
        while((UpperPort != NULL) && (UpperPort->device->index == port->device->index))
        {
            SourcePort = UpperPort;
            UpperPort = (AvPort*)UpperPort->content.RouteVideoFromPort;
        }
        #endif
        /* Restore UpperPort */
        UpperPort = (AvPort*)port->content.RouteVideoFromPort;
        ret = AvOk;
        /* InfoReady Delay Implement */
        if(port->content.tx->InfoReady <= TxHdcpManageThreshold)
        {
            port->content.tx->InfoReady = port->content.tx->InfoReady + 1;
            //AvUapiOutputDebugMessage("Port%d: InfoReady = %d", port->index, port->content.tx->InfoReady);
        }
        DifferentVideoPackets = SourcePort->content.video->AvailableVideoPackets ^
                                port->content.video->AvailableVideoPackets;
        #if GSV5K1
        if(port->device->type == Gsv5k1)
        {
            /* If Rx is in DVI mode, Generate AVI Infoframe to send DSC related information */
            if(((SourcePort->type == HdmiRx) || (SourcePort->type == CcaRx)) &&
               (SourcePort->content.rx->HdmiMode == 0))
            {
                /* Remove useless DifferentVideoPackets as Dvi has no InfoFrame */
                if((DifferentVideoPackets & AV_BIT_AV_INFO_FRAME) != 0)
                    DifferentVideoPackets = DifferentVideoPackets & (~AV_BIT_AV_INFO_FRAME);
                /* Enable Video Packet Generation when not enabled */
                else
                    VspGenVideoPackets = VspGenVideoPackets | AV_BIT_AV_INFO_FRAME;
            }
        }
        #endif
        if(SourcePort != UpperPort)
        {
            /* Check Hdmi Core Update Status */
            switch(port->core.HdmiCore)
            {
                case 0:
                    UpdateSlot = 0x01;
                    break;
                case 1:
                    UpdateSlot = 0x02;
                    break;
            }
            switch(UpperPort->type)
            {
                case VideoScaler:
                    if((UpperPort->content.scaler->Update & UpdateSlot) != 0)
                    {
                        UpperPort->content.scaler->Update = (UpperPort->content.scaler->Update & (~UpdateSlot));
                        VspGenVideoPackets = VspGenVideoPackets | AV_BIT_AV_INFO_FRAME;
                    }
                    break;
                case VideoDsc:
                    if((UpperPort->content.dsc->Update & UpdateSlot) != 0)
                    {
                        UpperPort->content.dsc->Update = (UpperPort->content.dsc->Update & (~UpdateSlot));
                        DifferentVideoPackets = DifferentVideoPackets | AV_BIT_AV_INFO_FRAME;
                        DifferentVideoPackets = DifferentVideoPackets | AV_BIT_GC_PACKET;
                        if((UpperPort->content.dsc->DscInVic == 93) ||
                           (UpperPort->content.dsc->DscInVic == 94) ||
                           (UpperPort->content.dsc->DscInVic == 95) ||
                           (UpperPort->content.dsc->DscInVic == 103) ||
                           (UpperPort->content.dsc->DscInVic == 104) ||
                           (UpperPort->content.dsc->DscInVic == 105) ||
                           (UpperPort->content.dsc->DscInVic == 240) ||
                           (UpperPort->content.dsc->DscInVic == 241) ||
                           (UpperPort->content.dsc->DscInVic == 242) ||
                           (UpperPort->content.dsc->DscInVic == 243) ||
                           (port->content.dsc->DscInVic == 93) ||
                           (port->content.dsc->DscInVic == 94) ||
                           (port->content.dsc->DscInVic == 95) ||
                           (port->content.dsc->DscInVic == 103) ||
                           (port->content.dsc->DscInVic == 104) ||
                           (port->content.dsc->DscInVic == 105) ||
                           (port->content.dsc->DscInVic == 240) ||
                           (port->content.dsc->DscInVic == 241) ||
                           (port->content.dsc->DscInVic == 242) ||
                           (port->content.dsc->DscInVic == 243))
                            VspGenVideoPackets = VspGenVideoPackets | AV_BIT_VS_PACKET;
                    }
                    break;
                case VideoColor:
                    if((UpperPort->content.color->Update & UpdateSlot) != 0)
                    {
                        UpperPort->content.color->Update = (UpperPort->content.color->Update & (~UpdateSlot));
                        VspGenVideoPackets = VspGenVideoPackets | AV_BIT_AV_INFO_FRAME;
                    }
                    break;
                default:
                    break;
            }
        }
        /* Direct Connect, Keep Color Always Correct */
        else if(((SourcePort->content.video->AvailableVideoPackets & AV_BIT_AV_INFO_FRAME) != 0) &&
                ((SourcePort->content.video->Y != port->content.video->Y) && (SourcePort->type != LogicVideoRx)))
        {
            port->content.video->Y     = SourcePort->content.video->Y;
            port->content.video->InCs  = SourcePort->content.video->InCs;
            DifferentVideoPackets      = DifferentVideoPackets | AV_BIT_AV_INFO_FRAME;
        }
        if((DifferentVideoPackets & AV_BIT_AV_INFO_FRAME) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_AV_INFO_FRAME,
                                      AV_PKT_AV_INFO_FRAME, AvVideoPacketType, AvRxUpdate);
        }
        else if((VspGenVideoPackets & AV_BIT_AV_INFO_FRAME) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_AV_INFO_FRAME,
                                      AV_PKT_AV_INFO_FRAME, AvVideoPacketType, AvRxForce);
        }
        if((DifferentVideoPackets & AV_BIT_SPD_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_SPD_PACKET,
                                      AV_PKT_SPD_PACKET, AvVideoPacketType, AvRxUpdate);
        }
        if((DifferentVideoPackets & AV_BIT_HDR_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_HDR_PACKET,
                                      AV_PKT_HDR_PACKET, AvVideoPacketType, AvRxUpdate);
        }
#if 0
        if((DifferentVideoPackets & AV_BIT_EMP_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_EMP_PACKET,
                                      AV_PKT_EMP_PACKET, AvVideoPacketType, AvRxUpdate);
        }
#endif
        if((DifferentVideoPackets & AV_BIT_MPEG_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_MPEG_PACKET,
                                      AV_PKT_MPEG_PACKET, AvVideoPacketType, AvRxUpdate);
        }
        if((DifferentVideoPackets & AV_BIT_VS_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_VS_PACKET,
                                      AV_PKT_VS_PACKET, AvVideoPacketType, AvRxUpdate);
        }
        else if((VspGenVideoPackets & AV_BIT_VS_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_VS_PACKET,
                                      AV_PKT_VS_PACKET, AvVideoPacketType, AvRxForce);
        }
        if((DifferentVideoPackets & AV_BIT_ISRC1_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_ISRC1_PACKET,
                                      AV_PKT_ISRC1_PACKET, AvVideoPacketType, AvRxUpdate);
        }
        if((DifferentVideoPackets & AV_BIT_ISRC2_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_ISRC2_PACKET,
                                      AV_PKT_ISRC2_PACKET, AvVideoPacketType, AvRxUpdate);
        }
        if((DifferentVideoPackets & AV_BIT_GMD_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_GMD_PACKET,
                                      AV_PKT_GMD_PACKET, AvVideoPacketType, AvRxUpdate);
        }
        if((DifferentVideoPackets & AV_BIT_GC_PACKET) != 0)
        {
            if((SourcePort->content.video->AvailableVideoPackets & AV_BIT_GC_PACKET) != 0)
            {
                AvUapiRxGetHdmiDeepColor(SourcePort);
                KfunTxSetColorDepth(SourcePort, port);
                AvUapiTxSetHdmiDeepColor(port);
                port->content.video->AvailableVideoPackets =
                    port->content.video->AvailableVideoPackets | AV_BIT_GC_PACKET;
                PktContent[0] = 1;
                AvUapiTxSetPacketContent(port, AV_PKT_GC_PACKET, PktContent, 1);
            }
            else
            {
                port->content.video->AvailableVideoPackets =
                    port->content.video->AvailableVideoPackets & (~AV_BIT_GC_PACKET);
                PktContent[0] = 0;
                AvUapiTxSetPacketContent(port, AV_PKT_GC_PACKET, PktContent, 1);
            }
        }
        if((SourcePort->content.rx->HdmiMode == 0) && (SourcePort->type == HdmiRx))
        {
            switch(UpperPort->type)
            {
                case VideoColor:
                    UpperPort->content.color->ColorInSpace = AV_Y2Y1Y0_RGB;
                    UpperPort->content.color->ColorInCs    = AV_CS_RGB;
                    port->content.video->Y     = AV_Y2Y1Y0_YCBCR_420;
                    port->content.video->InCs  = AV_CS_YUV_709;
                    break;
                case VideoScaler:
                    #if GSV5K1
                    if(UpperPort->device->type == Gsv5k1)
                    {
                        UpperPort->content.scaler->ScalerInSpace = AV_Y2Y1Y0_RGB;;
                        UpperPort->content.scaler->ScalerInCs = AV_CS_RGB;
                        UpperPort->content.scaler->ScalerOutSpace = AV_Y2Y1Y0_RGB;;
                        UpperPort->content.scaler->ScalerOutCs = AV_CS_RGB;
                    }
                    else
                    #endif
                    {
                        UpperPort->content.scaler->ColorSpace = AV_Y2Y1Y0_RGB;
                    }
                    port->content.video->Y     = AV_Y2Y1Y0_RGB;
                    port->content.video->InCs  = AV_CS_RGB;
                    break;
                case VideoDsc:
                    UpperPort->content.dsc->SrcInY = AV_Y2Y1Y0_RGB;
                    UpperPort->content.dsc->SrcInCd = AV_CD_24;
                    UpperPort->content.dsc->SrcInHdmi = 0;
                    UpperPort->content.dsc->SrcInCs = AV_CS_RGB;
                    port->content.video->Y     = AV_Y2Y1Y0_YCBCR_444;
                    port->content.video->InCs  = AV_CS_YUV_709;
                    break;
                default:
                    port->content.video->Y     = AV_Y2Y1Y0_RGB;
                    port->content.video->InCs  = AV_CS_RGB;
                    break;
            }
        }
        /* Call Silicon Layer */
        AvUapiTxVideoManage(port);
    }
    else
    {
        ret = AvNotAvailable;
    }
    return ret;
}

/**
 * @brief
 * function to Manage Audio in HDMI Tx
 * @return AvOk - success
 * @note
 */
AvRet KfunTxAudioManage(pout AvPort *port)
{
    AvRet ret;
    uint16 DifferentAudioPackets = 0;
    AvPort *SourcePort;
    #if AvMatrix4x4Mode
    AvPort *UpperPort;
    #endif
    uint8 PktContent[40];
    uint8 TxAudioMute = 0;

#if (Gsv2k1MuxMode != 0)
    if(port->device->type == Gsv2k1)
    {
        return AvOk;
    }
#endif

    /* Don't check audio for DVI Tx Port */
    if(port->content.tx->HdmiMode == 0)
    {
        port->content.audio->AvailableAudioPackets = 0;
        return AvOk;
    }

    /* 1. Get Source information */
    /* if source does not exist, no need to update information at all */
    /* 2. if source exists, follow Rx's status, compare difference */
    /* Firstly carry bulk info from Front Port, then update the port */
    if(KfunFindAudioRxFront(port, &SourcePort) == AvOk)
    {
        #if AvMatrix4x4Mode
        /* Use the information of Rx in the same device */
        UpperPort = (AvPort*)port->content.RouteAudioFromPort;
        while((UpperPort != NULL) && (UpperPort->device->index == port->device->index))
        {
            SourcePort = UpperPort;
            UpperPort = (AvPort*)UpperPort->content.RouteAudioFromPort;
        }
        #endif
        ret = AvOk;
        if(SourcePort->type == LogicAudioRx)
            return AvOk;
        DifferentAudioPackets = SourcePort->content.audio->AvailableAudioPackets ^
                                port->content.audio->AvailableAudioPackets;
        if((DifferentAudioPackets & AV_BIT_AUDIO_CHANNEL_STATUS) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_AUDIO_CHANNEL_STATUS,
                                      AV_PKT_AUDIO_CHANNEL_STATUS, AvAudioPacketType, AvRxUpdate);
        }
        if((DifferentAudioPackets & AV_BIT_AUDIO_INFO_FRAME) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_AUDIO_INFO_FRAME,
                                      AV_PKT_AUDIO_INFO_FRAME, AvAudioPacketType, AvRxUpdate);
        }
        if((DifferentAudioPackets & AV_BIT_ACP_PACKET) != 0)
        {
            KfunTxfromRxPacketContent(SourcePort, port, PktContent, AV_BIT_ACP_PACKET,
                                      AV_PKT_ACP_PACKET, AvAudioPacketType, AvRxUpdate);
        }
        if(DifferentAudioPackets != 0)
        {
            TxAudioMute = port->content.audio->AudioMute;
            AvMemcpy(port->content.audio, SourcePort->content.audio, sizeof(AvAudio));
            port->content.audio->AudioMute = TxAudioMute;
            AvUapiTxSetAudioPackets(port);
        }
        /* Call Silicon Layer */
        AvUapiTxAudioManage(port);

    }
    else
    {
        ret = AvNotAvailable;
    }
    return ret;
}

/* lookup the connection to find which is the port before */
/**
 * @brief
 * function to Look Up From Port in the connection chain
 * @return AvOk - success
 * @note
 */
AvPort* KfunLookupFromPort(AvPort *port)
{
    if((port->type == LogicAudioRx) || (port->type == LogicAudioTx))
    {
        /* lookup audio port */
        return (AvPort*)(port->content.RouteAudioFromPort);
    }
    else
    {
        /* lookup video port */
        return (AvPort*)(port->content.RouteVideoFromPort);
    }
}

/**
 * @brief
 * function to Initialize Rx Routing Related Vars
 * @return AvOk - success
 * @note
 */
void KfunRxRoutingVarInit(pout AvPort *port)
{
    return;
}

void KfunRxRoutingClearEndInfo(pin AvPort *port)
{
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        /* Check whether there are missing sinks or added sinks */
        if((CurrentPort->type == HdmiTx) || (CurrentPort->type == CcaTx))
        {
            CurrentPort->content.video->AvailableVideoPackets = 0x0;
            CurrentPort->content.audio->AvailableAudioPackets = 0x0;
            CurrentPort->content.tx->ChangedVideoPackets = 0xffff;
            CurrentPort->content.tx->ChangedAudioPackets = 0xffff;
        }
        PrevPort = CurrentPort;
    }

    return;
}

/**
 * @brief  Find next Tx End Audio Node in the signal chain
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindAudioNextTxEnd(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteAudioToPort;

    /* Loop through all nodes in the level */
    while(TempNode)
    {
        /* 1. First Node Contains Next Level */
        /* This TempNode will never be CompareNode */
        if(TempNode->content.RouteAudioToPort)
        {
            /* CompareNode is Found in the next level */
            if(KfunFindAudioNextTxEnd(TempNode, CompareNode, FoundNode) == AvOk)
            {
                /* Next Node is Found */
                if(*FoundNode)
                    return AvOk;
                /* CompareNode is Found, but NextNode is not Found */
                /* set CompareNode to be NULL, and return any End Port other than NULL in the next loop */
                else
                {
                    *CompareNode = NULL;
                    ret = AvOk;
                    TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                    continue;
                }
            }
            /* CompareNode is not Found, go to the next same level node */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                continue;
            }
        }

        /* 2. First Port Does Not Contain Next Level */
        else
        {
            /* 2.1 If CompareNode Found, Next Port is The Result */
            if(TempNode == *CompareNode)
            {
                *CompareNode = NULL;
                TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                if(!TempNode)
                    return AvNotAvailable;
                else
                {
                    ret = AvOk;
                    continue;
                }
            }
            /* 2.2 No Next Level Nodes available, the Node itself is an End */
            /* Return This Node as the wanted Node */
            else if((!(*CompareNode)) && (TempNode))
            {
                *FoundNode = TempNode;
                return AvOk;
            }
            /* 2.3 Both CompareNode and TempNode are not Found */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                continue;
            }
        }
    }
    /* If the port in this level is totally searched */
    return ret;
}

/**
 * @brief  Find all connected Audio Node in the signal chain
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindAudioNextNode(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteAudioToPort;

    /* Loop through all nodes in the level */
    while(TempNode)
    {
        /* 1. Feed the Current Node */
        /* Return This Node as the wanted Node */
        if((!(*CompareNode)) && (TempNode))
        {
            *FoundNode = TempNode;
            return AvOk;
        }
        /* 2. First Port Does Not Contain Next Level */
        else if(!(TempNode->content.RouteAudioToPort))
        {
            /* 2.1 If CompareNode Found, Next Port is The Result */
            if(TempNode == *CompareNode)
            {
                *CompareNode = NULL; /* indicate CompareNode is found */
                ret = AvOk;
                TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                continue;
            }
            /* 2.2 Both CompareNode and TempNode are not Found */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                continue;
            }
        }
        /* 3. First Node Contains Next Level */
        /* This TempNode will never be CompareNode */
        else
        {
            /* 3.1 The Parent Node is the Compare Node */
            if(TempNode == *CompareNode)
            {
                *CompareNode = NULL; /* indicate CompareNode is found */
                ret = AvOk;
            }
            /* 3.2 CompareNode is Found in the next level */
            if(KfunFindAudioNextNode(TempNode, CompareNode, FoundNode) == AvOk)
            {
                /* Next Node is Found */
                if(*FoundNode)
                    return AvOk;
                /* CompareNode is Found, but NextNode is not Found */
                /* set CompareNode to be NULL, and return any End Port other than NULL in the next loop */
                else
                {
                    *CompareNode = NULL;
                    ret = AvOk;
                    TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                    continue;
                }
            }
            /* CompareNode is not Found, go to the next same level node */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteAudioNextSameLevelPort;
                continue;
            }
        }

    }
    /* If the port in this level is totally searched */
    if(!TempNode)
      ret = AvNotAvailable;
    return ret;
}

/**
 * @brief  Find Rx Front Audio Node in the signal chain
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindAudioRxFront(AvPort* CurrentNode, AvPort* *FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteAudioFromPort;

    /* 1. No Front at all */
    if(!TempNode)
        return ret;

    /* 2. Loop through all nodes in the level */
    while(TempNode)
    {
        *FoundNode = TempNode;
        TempNode = (AvPort*)TempNode->content.RouteAudioFromPort;
        ret = AvOk;
    }

    return ret;
}

/**
 * @brief  Find next Tx End Video Node in the signal chain
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindVideoNextTxEnd(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteVideoToPort;

    /* Loop through all nodes in the level */
    while(TempNode)
    {
        /* 1. First Node Contains Next Level */
        /* This TempNode will never be CompareNode */
        if(TempNode->content.RouteVideoToPort)
        {
            /* CompareNode is Found in the next level */
            if(KfunFindVideoNextTxEnd(TempNode, CompareNode, FoundNode) == AvOk)
            {
                /* Next Node is Found */
                if(*FoundNode)
                    return AvOk;
                /* CompareNode is Found, but NextNode is not Found */
                /* set CompareNode to be NULL, and return any End Port other than NULL in the next loop */
                else
                {
                    *CompareNode = NULL;
                    ret = AvOk;
                    TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                    continue;
                }
            }
            /* CompareNode is not Found, go to the next same level node */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                continue;
            }
        }

        /* 2. First Port Does Not Contain Next Level */
        else
        {
            /* 2.1 If CompareNode Found, Next Port is The Result */
            if(TempNode == *CompareNode)
            {
                *CompareNode = NULL;
                TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                if(!TempNode)
                    return AvNotAvailable;
                else
                {
                    ret = AvOk;
                    continue;
                }
            }
            /* 2.2 No Next Level Nodes available, the Node itself is an End */
            /* Return This Node as the wanted Node */
            else if((!(*CompareNode)) && (TempNode))
            {
                *FoundNode = TempNode;
                return AvOk;
            }
            /* 2.3 Both CompareNode and TempNode are not Found */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                continue;
            }
        }
    }
    /* If the port in this level is totally searched */
    return ret;
}

/**
 * @brief  Find all connected Video Node in the signal chain
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindVideoNextNode(AvPort* CurrentNode, AvPort* *CompareNode, AvPort* *FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteVideoToPort;

    /* Loop through all nodes in the level */
    while(TempNode)
    {
        /* 1. Feed the Current Node */
        /* Return This Node as the wanted Node */
        if((!(*CompareNode)) && (TempNode))
        {
            *FoundNode = TempNode;
            return AvOk;
        }
        /* 2. First Port Does Not Contain Next Level */
        else if(!(TempNode->content.RouteVideoToPort))
        {
            /* 2.1 If CompareNode Found, Next Port is The Result */
            if(TempNode == *CompareNode)
            {
                *CompareNode = NULL; /* indicate CompareNode is found */
                ret = AvOk;
                TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                continue;
            }
            /* 2.2 Both CompareNode and TempNode are not Found */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                continue;
            }
        }
        /* 3. First Node Contains Next Level */
        /* This TempNode will never be CompareNode */
        else
        {
            /* 3.1 The Parent Node is the Compare Node */
            if(TempNode == *CompareNode)
            {
                *CompareNode = NULL; /* indicate CompareNode is found */
                ret = AvOk;
            }
            /* 3.2 CompareNode is Found in the next level */
            if(KfunFindVideoNextNode(TempNode, CompareNode, FoundNode) == AvOk)
            {
                /* Next Node is Found */
                if(*FoundNode)
                    return AvOk;
                /* CompareNode is Found, but NextNode is not Found */
                /* set CompareNode to be NULL, and return any End Port other than NULL in the next loop */
                else
                {
                    *CompareNode = NULL;
                    ret = AvOk;
                    TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                    continue;
                }
            }
            /* CompareNode is not Found, go to the next same level node */
            else
            {
                TempNode = (AvPort*)TempNode->content.RouteVideoNextSameLevelPort;
                continue;
            }
        }

    }
    /* If the port in this level is totally searched */
    if(!TempNode)
      ret = AvNotAvailable;
    return ret;
}

/**
 * @brief  Find Rx Front Video Node in the signal chain
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindVideoRxFront(AvPort* CurrentNode, AvPort** FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteVideoFromPort;

    /* 1. No Front at all */
    if(!TempNode)
        return ret;

    /* 2. Loop through all nodes in the level */
    while(TempNode)
    {
        *FoundNode = TempNode;
        TempNode = (AvPort*)TempNode->content.RouteVideoFromPort;
        ret = AvOk;
    }

    return ret;
}

#if AvMatrix4x4Mode
/**
 * @brief  Find Rx Front Video Node in the signal chain of the same device
 * @return AvOk: Found CompareNode
 *         AvNotAvailable: Not Found CompareNode
 */
AvRet KfunFindVideoRxFrontInSameDevice(AvPort* CurrentNode, AvPort** FoundNode)
{
    AvRet ret = AvNotAvailable;
    AvPort* TempNode = (AvPort*)CurrentNode->content.RouteVideoFromPort;

    /* 1. No Front at all */
    if(!TempNode)
        return ret;

    /* 2. Loop through all nodes in the level */
    while((TempNode) && (CurrentNode->device->index == TempNode->device->index))
    {
        *FoundNode = TempNode;
        TempNode = (AvPort*)TempNode->content.RouteVideoFromPort;
        ret = AvOk;
    }

    return ret;
}
#endif

AvRet KfunCheckVspPortContent(pin AvPort *port, PacketType Pkt, uint8 *Content)
{
    AvRet ret = AvOk;
    AvPort *SourcePort;
    uint8 VicUpdateFlag = 0;
#if GSV2K11
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
#endif
#if GSV5K1
    AvPort *RxPort;
    uint8 DscSourceInValid = 0;
    uint8 CpUpdateFlag = 0;
#endif
    SourcePort = (AvPort*)port->content.RouteVideoFromPort;
    while((SourcePort != NULL) &&
          ((SourcePort->type == VideoScaler) ||
          (SourcePort->type == VideoDsc)     ||
           (SourcePort->type == VideoColor)))
    {
        switch(Pkt)
        {
            case AV_PKT_AV_INFO_FRAME:
            {
                switch(SourcePort->type)
                {
                    case VideoScaler:
                        KfunCheckVideoScaler(SourcePort);
                        if(VicUpdateFlag == 0)
                        {
                            if(SourcePort->content.scaler->ScalerOutVic != 0)
                            {
                                SET_AVIF_VIC(Content,SourcePort->content.scaler->ScalerOutVic);
                                port->content.video->timing.Vic = SourcePort->content.scaler->ScalerOutVic;
                                #if GSV2K11
                                /* Deinterlacer */
                                if(port->device->type!=Gsv5k1)
                                {
                                    if((port->content.video->timing.Vic == 2) || (port->content.video->timing.Vic == 17))
                                    {
                                        SET_AVIF_PR(Content, 0);
                                    }
                                }
                                #endif
                                VicUpdateFlag = 1;
                            }
                        #if GSV5K1
                            if(port->device->type==Gsv5k1)
                            {
                                 /* this is already a output-compatible color space,
                                DSC could only make it better by matching the front end */
                                SET_AVIF_Y(Content,SourcePort->content.scaler->ScalerOutSpace);
                                SET_AVIF_C(Content,(SourcePort->content.scaler->ScalerOutCs & 0x03));
                                SET_AVIF_EC(Content,((SourcePort->content.scaler->ScalerOutCs>>2) & 0x07));
                                SET_AVIF_Q(Content,((SourcePort->content.scaler->ScalerOutCs>>6) & 0x03));
                                port->content.video->Y = SourcePort->content.scaler->ScalerOutSpace;
                                port->content.video->InCs = SourcePort->content.scaler->ScalerOutCs;
                                port->content.video->OutCs = port->content.video->InCs;
                                CpUpdateFlag = 1;
                            }
                        #endif
                            if((port->device->type!=Gsv5k1))
                            {
                                if(SourcePort->content.scaler->ColorSpace == AV_Y2Y1Y0_YCBCR_420)
                                {
                                    port->content.video->Y = AV_Y2Y1Y0_YCBCR_444;
                                    SET_AVIF_Y(Content,AV_Y2Y1Y0_YCBCR_444);
                                }
                            #if GSV2K11
                                else
                                {
                                    while(KfunFindVideoNextTxEnd(SourcePort, &PrevPort, &CurrentPort) == AvOk)
                                    {
                                        if(CurrentPort->type == LogicVideoTx)
                                        {
                                            if(CurrentPort->content.video->Y != AV_Y2Y1Y0_INVALID)
                                            {
                                                SET_AVIF_Y(Content,CurrentPort->content.video->Y);
                                                port->content.video->Y = CurrentPort->content.video->Y;
                                            }
                                            if(CurrentPort->content.video->InCs != AV_CS_AUTO)
                                            {
                                                SET_AVIF_C(Content,(CurrentPort->content.video->InCs & 0x03));
                                                SET_AVIF_EC(Content,((CurrentPort->content.video->InCs>>2) & 0x07));
                                                SET_AVIF_Q(Content,((CurrentPort->content.video->InCs>>6) & 0x03));
                                                port->content.video->InCs = CurrentPort->content.video->InCs;
                                            }
                                        }
                                        PrevPort = CurrentPort;
                                        CurrentPort = NULL;
                                    }
                                }
                            #endif
                            }
                        }
                        break;
                    #if GSV5K1
                    case VideoDsc:
                        if(port->device->type != Gsv5k1)
                            break;
                        KfunCheckVideoDsc(SourcePort);
                        if(VicUpdateFlag == 0)
                        {
                            if(SourcePort->content.dsc->DscInVic != 0)
                            {
                                SET_AVIF_VIC(Content,SourcePort->content.dsc->DscInVic);
                                port->content.video->timing.Vic = SourcePort->content.dsc->DscInVic;
                                VicUpdateFlag = 1;
                            }
                        }
                        if(SourcePort->content.dsc->DscInCompressed == 1)
                        {
                            /* Resend Color Depth according to InfoFrame */
                            port->content.video->AvailableVideoPackets =
                                port->content.video->AvailableVideoPackets & (~AV_BIT_GC_PACKET);
                            SET_AVIF_LEN(Content,0x0D);
                            SET_DSC_EN(Content,0);
                            SET_DSC_RATIO(Content,0);
                            SET_DSC_Y(Content,0);
                            SET_DSC_CS(Content,0);
                            SET_DSC_VIC(Content,0);
                            SET_DSC_SRC_Y(Content,0);
                            SET_DSC_SRC_HDMI(Content,0);
                            SET_DSC_HDCP(Content,0);
                            SET_DSC_SRC_CD(Content,0);
                            SET_DSC_SRC_CS(Content,0);
                            /* Code comes to here after checking Color Port if Rx->DSC->Color->Tx,
                               so this update is to make display match front input */
                            switch(SourcePort->content.dsc->SrcInCs)
                            {
                                case AV_CS_LIM_RGB:
                                    DscSourceInValid = 1;
                                    break;
                                case AV_CS_RGB:
                                    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_RGB_FULL_RANGE)
                                        DscSourceInValid = 1;
                                    break;
                                case AV_CS_YUV_601:
                                case AV_CS_YCC_601:
                                    if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YUV_601) &&
                                       (port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE))
                                        DscSourceInValid = 1;
                                    break;
                                case AV_CS_YUV_709:
                                case AV_CS_YCC_709:
                                    if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YUV_709) &&
                                       (port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YUV_FULL_RANGE))
                                        DscSourceInValid = 1;
                                    break;
                                case AV_CS_LIM_YUV_601:
                                case AV_CS_LIM_YCC_601:
                                    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YUV_601)
                                        DscSourceInValid = 1;
                                    break;
                                case AV_CS_LIM_YUV_709:
                                case AV_CS_LIM_YCC_709:
                                    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YUV_709)
                                        DscSourceInValid = 1;
                                    break;
                                case AV_CS_LIM_BT2020_RGB:
                                case AV_CS_BT2020_RGB:
                                    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_RGB_2020)
                                        DscSourceInValid = 1;
                                    break;
                                case AV_CS_LIM_BT2020_YCC:
                                case AV_CS_BT2020_YCC:
                                    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_YCC_2020)
                                        DscSourceInValid = 1;
                                    break;
                                default:
                                    break;
                            }
                            if(DscSourceInValid == 1)
                            {
                                /* Match HDMI Output color space */
                                if(CpUpdateFlag == 0)
                                {
                                    port->content.video->Y = SourcePort->content.dsc->DscInY;
                                    SET_AVIF_Y(Content,SourcePort->content.dsc->DscInY);
                                }
                                port->content.video->OutCs = SourcePort->content.dsc->SrcInCs;
                                SET_AVIF_C(Content,(SourcePort->content.dsc->SrcInCs & 0x03));
                                SET_AVIF_EC(Content,((SourcePort->content.dsc->SrcInCs>>2) & 0x07));
                                SET_AVIF_Q(Content,((SourcePort->content.dsc->SrcInCs>>6) & 0x03));
                            }
                        }
                        else
                        {
                            SET_AVIF_LEN(Content,0x1B);
                            SET_DSC_EN(Content,       SourcePort->content.dsc->Enable);
                            SET_DSC_RATIO(Content,    SourcePort->content.dsc->DscRatio);
                            SET_DSC_Y(Content,        SourcePort->content.dsc->DscInY);
                            SET_DSC_CS(Content,       SourcePort->content.dsc->DscInCs);
                            SET_DSC_VIC(Content,      SourcePort->content.dsc->DscInVic);
                            SET_DSC_SRC_Y(Content,    SourcePort->content.dsc->SrcInY);
                            SET_DSC_SRC_HDMI(Content, SourcePort->content.dsc->SrcInHdmi);
                            SET_DSC_SRC_CD(Content,   SourcePort->content.dsc->SrcInCd);
                            SET_DSC_SRC_CS(Content,   SourcePort->content.dsc->SrcInCs);
                            SET_DSC_HDCP(Content,     SourcePort->content.dsc->HdcpStat);
                        }
                        break;
                    #endif
                    case VideoColor:
                        KfunCheckVideoColor(SourcePort);
                        if(VicUpdateFlag == 0)
                        {
                            if(SourcePort->content.color->ColorInVic != 0)
                            {
                                SET_AVIF_VIC(Content,SourcePort->content.color->ColorInVic);
                                port->content.video->timing.Vic = SourcePort->content.color->ColorInVic;
                                VicUpdateFlag = 1;
                            }
                        }
                        SET_AVIF_Y(Content,SourcePort->content.color->ColorOutSpace);
                        SET_AVIF_C(Content,(SourcePort->content.color->ColorOutCs & 0x03));
                        SET_AVIF_EC(Content,((SourcePort->content.color->ColorOutCs>>2) & 0x07));
                        SET_AVIF_Q(Content,((SourcePort->content.color->ColorOutCs>>6) & 0x03));
                        port->content.video->Y = SourcePort->content.color->ColorOutSpace;
                        port->content.video->InCs = SourcePort->content.color->ColorOutCs;
                    #if GSV5K1
                        if(port->device->type==Gsv5k1)
                        {
                            port->content.video->OutCs = port->content.video->InCs;
                            if(TrxAvUpStreamFlag == 1)
                            {
                                if((SourcePort == (AvPort*)(port->content.RouteVideoFromPort)) &&
                                   (SourcePort->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420) &&
                                   (SourcePort->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
                                   ((port->index == 5) || (port->type == CcaTx)))
                                {
                                    SET_AVIF_LEN(Content,0x1B);
                                    SET_DSC_EN(Content,       SourcePort->content.color->Enable);
                                    SET_DSC_RATIO(Content,    AV_CSC_2BY1);
                                    SET_DSC_Y(Content,        SourcePort->content.color->ColorOutSpace);
                                    SET_DSC_CS(Content,       SourcePort->content.color->ColorOutCs);
                                    SET_DSC_VIC(Content,      SourcePort->content.color->ColorInVic);
                                    if(KfunFindVideoRxFront(port, &RxPort) == AvOk)
                                    {
                                        SET_DSC_SRC_Y(Content,    RxPort->content.video->Y);
                                        SET_DSC_SRC_HDMI(Content, RxPort->content.rx->HdmiMode);
                                        SET_DSC_SRC_CD(Content,   RxPort->content.video->Cd);
                                        SET_DSC_SRC_CS(Content,   RxPort->content.video->InCs);
                                        SET_DSC_HDCP(Content,     RxPort->content.rx->VideoEncrypted);
                                    }
                                }
                            }
                            else if(TrxAvUpStreamFlag == 0)
                            {
                                if((SourcePort == (AvPort*)(port->content.RouteVideoFromPort)) &&
                                   (SourcePort->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420) &&
                                   (SourcePort->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420))
                                {
                                    SET_AVIF_LEN(Content,0x0D);
                                    SET_DSC_EN(Content,0);
                                    SET_DSC_RATIO(Content,0);
                                    SET_DSC_Y(Content,0);
                                    SET_DSC_CS(Content,0);
                                    SET_DSC_VIC(Content,0);
                                    SET_DSC_SRC_Y(Content,0);
                                    SET_DSC_SRC_HDMI(Content,0);
                                    SET_DSC_HDCP(Content,0);
                                    SET_DSC_SRC_CD(Content,0);
                                    SET_DSC_SRC_CS(Content,0);
                                }
                            }
                            CpUpdateFlag = 1;
                        }
                    #endif
                        break;
                    default:
                        break;
                }
                break;
            }// AV_PKT_AV_INFO_FRAME
            case AV_PKT_VS_PACKET:
            {
                switch(SourcePort->type)
                {
                    case VideoDsc:
                        if((Content[2] == 0x05) && (Content[3] == 0x00) && (Content[4] == 0x03) && (Content[5] == 0x0C))
                        {
                            Content[0] = 0x81;
                            Content[1] = 0x01;
                            Content[2] = 0x05;
                            Content[3] = 0x00;
                            Content[4] = 0x03;
                            Content[5] = 0x0C;
                            Content[6] = 0x00;
                            Content[7] = 0x20; /* 4K Timing Existed */
                            switch(SourcePort->content.dsc->DscInVic)
                            {
                                case 95: /* 4K30 */
                                    Content[8] = 1;
                                    break;
                                case 94: /* 4K25 */
                                    Content[8] = 2;
                                    break;
                                case 93: /* 4K24 */
                                    Content[8] = 3;
                                    break;
                                case 98: /* 4K24 SMPTE */
                                    Content[8] = 4;
                                    break;
                                default:
                                    Content[8] = 0;
                                    break;
                            }
                        }
                        break;
                }
                break;
            }// AV_PKT_VS_PACKET
            default:
                break;
        }

        /* Find upper port in the chain list*/
        SourcePort = (AvPort*)SourcePort->content.RouteVideoFromPort;
    }
    return ret;
}

/**
 * @brief
 * function to update Port Structure from Video Packet
 * @return AvOk - success
 * @note
 */
AvRet KfunUpdatePortFromPacket(pin AvPort *port, PacketType Pkt, uint8 *Content)
{
    AvRet ret = AvOk;
    uint8 Value = 0;
    uint8 i = 0;
    uint8 AviC = 0, AviY = 0, AviQ = 0, AviEC = 0;
    uint8 ChanAlloc = 0;
    AvPort *FromPort = (AvPort*)(port->content.RouteVideoFromPort);
#if GSV5K1
    //AvPort *FromPort;
    AvVideoCs AviCs = AV_CS_AUTO;
    uint8 DscContent[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#endif

    switch (Pkt)
    {
        case AV_PKT_VS_PACKET:
            if((Content[4] == 0x46) && (Content[5] == 0xD0) && (Content[6] == 0x00))
            {
                AvUapiOutputDebugMessage("Port%d: DolbyVision Detected ", port->index);
            }
            else if((Content[4] == 0x8b) && (Content[5] == 0x84) && (Content[6] == 0x90))
            {
                AvUapiOutputDebugMessage("Port%d: HDR10+ Detected ", port->index);
            }
            else if((Content[4] == 0x03) && (Content[5] == 0x0C) && (Content[6] == 0x00))
            {
                /* 3D related update */
                port->content.video->info.Detect3D = GET_3D_STATUS(Content);
                port->content.video->info.Format3D = (Vid3DFormat)GET_3D_FORMAT(Content);
            }
            #if GSV5K1
            if(port->device->type == Gsv5k1)
                break;
            #endif
            if((port->type == HdmiRx) &&
               ((port->content.video->info.TmdsFreq > 310) || (port->content.video->info.TmdsFreq < 280)))
            {
                SET_4K_FORMAT(Content, 0);
                break;
            }
            Value = GET_4K_STATUS(Content);
            if(Value == 1)
            {
                Value = GET_4K_FORMAT(Content);
                switch(Value)
                {
                    case 1: /* 4K30 */
                        port->content.video->timing.Vic = 95;
                        break;
                    case 2: /* 4K25 */
                        port->content.video->timing.Vic = 94;
                        break;
                    case 3: /* 4K24 */
                        port->content.video->timing.Vic = 93;
                        break;
                    case 4: /* 4K24 SMPTE */
                        port->content.video->timing.Vic = 98;
                        break;
                }
            }
            break;
        case AV_PKT_AV_INFO_FRAME:
            if(port->type == LogicVideoRx)
                break;
            #if AvEnableDebugAVIF
            AvUapiOutputDebugMessage("Port index[%d]:",port->index);
            AvUapiOutputDebugMessage("Rx AVIF: 0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                            Content[0],Content[1],Content[2],Content[3],
                                            Content[4],Content[5],Content[6],Content[7]);
            AvUapiOutputDebugMessage("         0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                            Content[8],Content[9],Content[10],Content[11],
                                            Content[12],Content[13],Content[14],Content[15]);
            AvUapiOutputDebugMessage("         0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                            Content[16],Content[17],Content[18],Content[19],
                                            Content[20],Content[21],Content[22],Content[23]);
            AvUapiOutputDebugMessage("         0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x",
                                            Content[24],Content[25],Content[26],Content[27],
                                            Content[28],Content[29],Content[30]);
            #endif

        #if GSV5K1
            if(port->device->type==Gsv5k1)
            {
                 /* Set Header for DVI Protection */
                if(Content[1] != 0x02)
                {
                    Content[0] = 0x82;
                    Content[1] = 0x02;
                    Content[2] = 0x0D;
                }
                /* VIC */
                if(port->content.video->timing.Vic == 0)
                    KfunRxVicFilter(port);
                SET_AVIF_VIC(Content,port->content.video->timing.Vic);

                /* Color Space */
                AviY  = GET_AVIF_Y(Content);
                if(AviY == 0)
                    AviQ = GET_AVIF_Q(Content);
                else
                {
                    AviEC = GET_AVIF_EC(Content);
                    AviC  = GET_AVIF_C(Content);
                    AviQ  = GET_AVIF_YQ(Content);
                }
                port->content.video->Y = (AvVideoY)(AviY);
                /* Protect Cs Error From Y Information */
                if((port->content.video->Y != AV_Y2Y1Y0_RGB) && (AviC == 0x00))
                    AviC = AviC | 0x02;
                AviCs = (AvVideoCs)((AviQ<<6)+AviC);
                if(AviC == 3)
                    AviCs = (AvVideoCs)(AviCs + (AviEC<<2));
                if(AviCs == 0x00)
                {
                    if(port->content.video->Y != AV_Y2Y1Y0_RGB)
                        AviCs = AV_CS_YUV_709;
                }
                /* Update Port Info */
                if((port->type == HdmiRx) || (port->type == CcaRx))
                {
                    /* default color for DVI */
                    if(port->content.rx->HdmiMode == 0)
                        AviCs = AV_CS_RGB;
                    port->content.video->InCs  = AviCs;
                    port->content.video->OutCs = AviCs;
                }
                else if((port->type == HdmiTx) || (port->type == CcaTx))
                {
                    FromPort = (AvPort*)port->content.RouteVideoFromPort;
                    switch(FromPort->type)
                    {
                        case HdmiRx:
                        case DviRx:
                        case CcaRx:
                            port->content.video->InCs = FromPort->content.video->OutCs;
                            break;
                        case VideoDsc:
                            port->content.video->InCs = FromPort->content.dsc->DscInCs;
                            break;
                        case VideoColor:
                            port->content.video->InCs = FromPort->content.color->ColorOutCs;
                            break;
                        case VideoScaler:
                            port->content.video->InCs = FromPort->content.scaler->ScalerOutCs;
                            /* Scaler will never generate 420 as output */
                            if(port->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
                                port->content.video->Y = AV_Y2Y1Y0_YCBCR_444;
                            break;
                    }
                    port->content.video->OutCs  = AviCs;
                    /* Convert RGB when EDID Fail */
                    /*
                    if(port->content.tx->IgnoreEdidError == 1)
                    {
                        port->content.video->OutCs = AV_CS_RGB;
                        port->content.video->Y = AV_Y2Y1Y0_RGB;
                    }
                    */
                }
            }
            else
        #endif
            {
                /* VIC */
                Value = GET_AVIF_VIC(Content);
                if(Value != 0)
                {
                    port->content.video->timing.Vic = Value;
                    AvUapiOutputDebugMessage("Port%d: Get Vic = [%x]", port->index, Value);
                }
                /* Color Space */
                AviY  = GET_AVIF_Y(Content);
                if(AviY == 0)
                    AviQ = GET_AVIF_Q(Content);
                else
                {
                    AviEC = GET_AVIF_EC(Content);
                    AviC  = GET_AVIF_C(Content);
                    AviQ  = GET_AVIF_YQ(Content);
                }
                port->content.video->Y = (AvVideoY)AviY;
                /* Protect Cs Error From Y Information */
                if((port->content.video->Y != AV_Y2Y1Y0_RGB) && (AviC == 0x00))
                    AviC = AviC | 0x02;
                port->content.video->InCs = (AvVideoCs)((AviQ<<6)+AviC);
                if(AviC == 3)
                    port->content.video->InCs = (AvVideoCs)(port->content.video->InCs + (AviEC<<2));
                if(port->content.video->InCs == 0x00)
                {
                    if(port->content.video->Y != AV_Y2Y1Y0_RGB)
                        port->content.video->InCs = AV_CS_YUV_709;
                }
                if(FromPort->type != LogicVideoRx)
                    port->content.video->OutCs = port->content.video->InCs;
            }

        #if GSV5K1
            if(((port->device->type==Gsv5k1)&&((((port->type == HdmiRx) || (port->type == CcaRx)) && (port->content.rx->HdmiMode == 1)) ||
               ((port->type == HdmiTx) || (port->type == CcaTx))))||(port->device->type!=Gsv5k1))
        #endif
            {
                switch (GET_AVIF_M(Content))
                {
                    case 1:
                        port->content.video->AspectRatio = AV_AR_4_3;
                        break;
                    case 2:
                        port->content.video->AspectRatio = AV_AR_16_9;
                        break;
                    default:
                        if (port->content.video->timing.Vic <= 107)
                            port->content.video->AspectRatio = ARTable[port->content.video->timing.Vic];
                        else
                            port->content.video->AspectRatio = AV_AR_NOT_INDICATED;
                        break;
                }
            }
            /* Pixel Repetition */
            port->content.video->PixelRepeatValue = GET_AVIF_PR(Content);

            #if AvGsv2kxVideoCscForValensRx
            {
                uint8 CpsContent[4] = {0x00, 0x00, 0x00, 0x00};

                /* Compression Information */
                CpsContent[0] = GET_CPS_SRC_Y(Content);    /* Src In Y */
                CpsContent[1] = GET_CPS_SRC_CD(Content);   /* Src Color Depth */
                CpsContent[2] = GET_CPS_SRC_CS(Content);   /* Src Color Space */
                #if 0//AvGsv2kxVideoHdcp14ForValensRxProblem
                //CpsContent[3] = GET_CPS_SRC_HDCP(Content);  /* Src HDCP Version */
                #endif
                AvHandleEvent(port, AvEventPortVideoInfoChanged, CpsContent, NULL);
                AvUapiOutputDebugMessage("Port%d: Get Compression Information", port->index);
                AvUapiOutputDebugMessage("RX Src Color Mode  == %d", CpsContent[0]);
                AvUapiOutputDebugMessage("RX Src Color Depth == %d", CpsContent[1]);
                AvUapiOutputDebugMessage("RX Src Color Space == %d", CpsContent[2]);

                #if AvGsv2kxVideoHdcp14ForValensRxProblem
                if(port->index == 1)
                {
                    //AvPort *FromPort = (AvPort*)(port->content.RouteVideoFromPort);
                    if(port->type == HdmiRx)
                    {
                        CpsContent[3] = GET_CPS_SRC_HDCP(Content);  /* Src HDCP Version */
                        AvUapiOutputDebugMessage("RX Src HDCP == %d", CpsContent[3]);
                        if(CpsContent[3] != 0)
                        {
                            port->content.rx->VideoEncrypted = 1;
                            if(Value == AV_HDCP_2P2_ENCRYPTED)
                                port->content.hdcp->Hdcp2p2RxRunning = 1;
                            else
                                port->content.hdcp->Hdcp2p2RxRunning = 0;
                        }
                        else
                        {
                            port->content.rx->VideoEncrypted = 0;
                            port->content.hdcp->Hdcp2p2RxRunning = 0;
                        }
                    }
                }
                #endif
            }
            #endif
        #if GSV5K1
             /* Dsc Information */
            if(port->device->type == Gsv5k1)
            {
                if(((port->type == HdmiRx) || (port->type == CcaRx)) &&
                   (port->content.rx->HdmiMode == 1))
                {
                    Value = GET_AVIF_LEN(Content);
                    /* Contain DSC Information */
                    if(Value == 0x1B)
                    {
                        DscContent[0] = GET_DSC_EN(Content);       /* Dsc Enable */
                        DscContent[1] = GET_DSC_RATIO(Content);    /* Dsc Ratio */
                        DscContent[2] = GET_DSC_VIC(Content);      /* Dsc Vic */
                        DscContent[3] = GET_DSC_Y(Content);        /* Dsc In Y */
                        DscContent[4] = GET_DSC_CS(Content);       /* Dsc In CS */
                        DscContent[5] = GET_DSC_SRC_Y(Content);    /* Src In Y */
                        DscContent[6] = GET_DSC_SRC_HDMI(Content); /* Src Hdmi Mode */
                        DscContent[7] = GET_DSC_SRC_CD(Content);   /* Src Color Depth */
                        DscContent[8] = GET_DSC_SRC_CS(Content);   /* Src Color Space */
                        DscContent[9] = GET_DSC_HDCP(Content);     /* Src Hdcp Status */
                        AvHandleEvent(port, AvEventPortUpStreamDscCompressed, DscContent, NULL);
                        AvUapiOutputDebugMessage("Port%d: Get Dsc Compression Information", port->index);
                    }
                    else
                    {
                    #if AvEnableDetailTiming
                        /* Double Check Input Timing, in case of incorrect Vesa Vic input, only for HDMI Input */
                        if((port->type == HdmiRx) || (port->type == CcaRx))
                        {
                            if(port->content.rx->IsInputStable == 1)
                                KfunRxReadInfo(port);
                            if(port->content.video->timing.Vic == 0)
                                SET_AVIF_VIC(Content,port->content.video->timing.Vic);
                        }
                    #endif
                        /* HDCP Status without DSC Info */
                        if(port->index == 1)
                        {
                            Value = GET_UPSTREAM_HDCP(Content);
                            if(Value != 0)
                            {
                                port->content.rx->VideoEncrypted = 1;
                                if(Value == AV_HDCP_2P2_ENCRYPTED)
                                    port->content.hdcp->Hdcp2p2RxRunning = 1;
                                else
                                    port->content.hdcp->Hdcp2p2RxRunning = 0;
                            }
                            else
                            {
                                port->content.rx->VideoEncrypted = 0;
                                port->content.hdcp->Hdcp2p2RxRunning = 0;
                            }
                        }
                        /* Dsc Content default to zero in function call */
                        AvHandleEvent(port, AvEventPortUpStreamNoDscInfo, DscContent, NULL);
                        AvUapiOutputDebugMessage("Port%d: No Dsc Information Found", port->index);
                    }
                }
            }
        #endif

            break;
        case AV_PKT_SPD_PACKET:
            break;
        case AV_PKT_HDR_PACKET:
            break;
        case AV_PKT_AUDIO_INFO_FRAME:
            /*==================================================
             * Adjust CC field according to CA since TX device
             * uses CC to decide on number of channels to send
             *=================================================*/
            if (GET_AUDIF_CC(Content) == 0)
            {
                /* Get Input Audio Packet Type */
                if (port->content.audio->AudFormat == AV_AUD_IF_AUTO)
                {
                    switch (port->content.audio->AudType)
                    {
                        case AV_AUD_TYPE_HBR:
                            port->content.audio->AudFormat = AV_AUD_I2S;
                            break;
                        case AV_AUD_TYPE_DSD:
                            port->content.audio->AudFormat = AV_AUD_DSD_NORM;
                            break;
                        case AV_AUD_TYPE_DST:
                            port->content.audio->AudFormat = AV_AUD_DSD_DST;
                            break;
                        case AV_AUD_TYPE_ASP:
                        default:
                            port->content.audio->AudFormat = AV_AUD_I2S;
                            break;
                    }
                }

                /* Get Audio Channel Number */
                ChanAlloc = GET_AUDIF_CA(Content);
                if (ChanAlloc < 32)
                {
                    port->content.audio->ChanNum = ChanCount[ChanAlloc];
                    SET_AUDIF_CC(Content, port->content.audio->ChanNum);
                }
            }
            else
            {
                port->content.audio->ChanNum = GET_AUDIF_CC(Content) + 1;
            }
            /* Get Audio Coding Type */
            Value = GET_AUDIF_CT(Content);
        #if GSV5K1
            if(port->device->type==Gsv5k1)
            {
                if(Value == 0)
                    Value = 1;
                port->content.audio->AudCoding = (AvAudioCoding)Value;
            }
            else
        #endif
            {
                if(Value != 0)
                    port->content.audio->AudCoding = (AvAudioCoding)Value;
            }
            break;
        case AV_PKT_MPEG_PACKET:
            break;
        case AV_PKT_ACP_PACKET:
            break;
        case AV_PKT_ISRC1_PACKET:
            break;
        case AV_PKT_ISRC2_PACKET:
            break;
        case AV_PKT_GMD_PACKET:
            break;
        case AV_PKT_AUDIO_CHANNEL_STATUS:
            port->content.audio->Consumer   = GET_CHST_PROFF_APP(Content);
            port->content.audio->Copyright  = GET_CHST_COPYRIGHT(Content);
            port->content.audio->Emphasis   = GET_CHST_EMPHASIS(Content);
            port->content.audio->CatCode    = GET_CHST_CATG_CODE(Content);
            port->content.audio->SrcNum     = GET_CHST_SRC_NUM(Content);
            port->content.audio->ClkAccur   = GET_CHST_CLK_ACCUR(Content);
            port->content.audio->WordLen    = GET_CHST_4BIT_WORD_LEN(Content);
            port->content.audio->Layout     = Content[5];

            if((port->content.audio->Consumer & 0x2) == 0)
                port->content.audio->AudCoding = AV_AUD_FORMAT_LINEAR_PCM;
            else
                port->content.audio->AudCoding = AV_AUD_FORMAT_COMPRESSED_AUDIO;

            Value = GET_CHST_CH_NUM(Content);
            if(Value != 0)
                port->content.audio->ChanNum = Value;

            Value = GET_CHST_SAMP_FREQ(Content);
            i = 0;
            while (ChannelStatusSfTable[i+1] != 0xff)
            {
                if (ChannelStatusSfTable[i+1] == Value)
                {
                    if(port->content.audio->SampFreq != (AvAudioSampleFreq)ChannelStatusSfTable[i])
                    {
                        port->content.audio->SampFreq = (AvAudioSampleFreq)ChannelStatusSfTable[i];
                        switch(port->content.audio->SampFreq)
                        {
                            case AV_AUD_FS_32KHZ:
                                AvUapiOutputDebugMessage("Port%d: 32KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_44KHZ:
                                AvUapiOutputDebugMessage("Port%d: 44KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_48KHZ:
                                AvUapiOutputDebugMessage("Port%d: 48KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_88KHZ:
                                AvUapiOutputDebugMessage("Port%d: 88KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_96KHZ:
                                AvUapiOutputDebugMessage("Port%d: 96KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_176KHZ:
                                AvUapiOutputDebugMessage("Port%d: 176KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_192KHZ:
                                AvUapiOutputDebugMessage("Port%d: 192KHz Detected", port->index);
                                break;
                            case AV_AUD_FS_HBR:
                                AvUapiOutputDebugMessage("Port%d: HBR Detected", port->index);
                                break;
                            case AV_AUD_FS_FROM_STRM:
                                AvUapiOutputDebugMessage("Port%d: Audio From Stream Detected", port->index);
                                break;
                        }
                    }
                    break;
                }
                i+=3;
            }
            break;
        default:
            ret = AvNotSupport;
            break;
    }

    return ret;
}

#if AvEnableCecFeature /* CEC Related */

/**
 * @brief
 * function to Validize Message Size
 * @return AvOk - success
 * @note
 */
AvRet ValidMsgSize (CecMessage Opcode, uint8 MsgSize)
{
    uint8 i;
    AvRet ret = AvOk;
    uint8 FixedLen = 0;

    switch(Opcode)
    {
        case AV_CEC_MSG_VENDOR_COMMAND:
            if ( MsgSize <= 2 )
            {
                ret = AvInvalidParameter;
            }
            break;
        case AV_CEC_MSG_VENDOR_REMOTE_BTN_DOWN:
            if ( MsgSize <= 2 )
            {
                ret = AvInvalidParameter;
            }
            break;
        case AV_CEC_MSG_SET_OSD_NAME:
            if ( MsgSize <= 2 )
            {
                ret = AvInvalidParameter;
            }
            break;
        case AV_CEC_MSG_SET_OSD_STRING:
            if (  MsgSize <= 3 )
            {
                ret = AvInvalidParameter;
            }
            break;
        case AV_CEC_MSG_TUNER_DEV_STATUS:
            ret = AvInvalidParameter;
            if ( ( MsgSize == 7) || (  MsgSize == 10))
            {
                ret = AvOk;
            }
            break;
        case AV_CEC_MSG_RECORD_ON:
            if (  MsgSize <= 3 )
            {
                ret = AvInvalidParameter;
            }
            break;
        case AV_CEC_MSG_SET_TIMER_PROG_TITLE:
            if ( MsgSize <= 2)
            {
                ret = AvInvalidParameter;
            }
            break;
        case AV_CEC_MSG_TIMER_STATUS:
            ret = AvInvalidParameter;
            if ( ( MsgSize == 3) || (  MsgSize == 5))
            {
                ret = AvOk;
            }
            break;
        case AV_CEC_MSG_VENDOR_CMD_WITH_ID:
            if (  MsgSize <= 5)
            {
                ret = AvInvalidParameter;
            }
            break;
        default:
            FixedLen = 1;
            for(i = 0; i < AV_FLEX_OP_CODES; i+=4 )
            {
                if (Opcode == FlexOpCodes[i])
                {
                    if (MsgSize < FlexOpCodes[i+3])
                    {
                        ret = AvInvalidParameter;
                    }
                    FixedLen = 0;
                }
            }
            break;
    }

    if ( FixedLen )
    {
        for (i=0; (i+1) < AV_CEC_OP_CODES; i+=2)
        {
            if (Opcode == CecOpCodes[i])
            {
                if (MsgSize < CecOpCodes[i+1])
                {
                    ret = AvInvalidParameter;
                }
                break;
            }
        }
    }
    else
    {
        AvUapiOutputDebugMessage("CEC: Msg size = %d", MsgSize);
        if(ret == AvInvalidParameter)
        {
            AvUapiOutputDebugMessage("CEC: Invalid msg size is %d,Opcode 0x%x.", MsgSize, Opcode);
            AvUapiOutputDebugMessage("CEC: take no action");
        }
    }
    return ret;
}

/**
 * @brief
 * function to Process Rx CEC Message
 * @return AvOk - success
 * @note
 */
AvRet KfunCecRxMsgProcess(AvPort *port)
{
    AvRet ret = AvNotAvailable;
    uint8* Msg = port->content.cec->RxContent;
    port->content.cec->RxMsg = (CecMessage)(AV_CEC_OPCODE(Msg));

    /* if(AvUapiGetCecStatus(port) != AvOk) */
    if(port->content.cec->RxGetFlag == 0)
        return AvNotAvailable;

    AvUapiOutputDebugMessage("CEC: Message received. Opcode=0x%x Src=%d Dst=%d Data=%x",
                             port->content.cec->RxMsg, AV_CEC_SRC(Msg), AV_CEC_DST(Msg), Msg[2]);

    if (ValidMsgSize(port->content.cec->RxMsg, port->content.cec->RxLen) == AvOk)
    {
        /*
        AvUapiOutputDebugMessage("CEC: Message received. Opcode=0x%x Src=%d Dst=%d\n\r",
                                 AV_CEC_OPCODE(Msg), AV_CEC_SRC(Msg), AV_CEC_DST(Msg));
        */

        switch (AV_CEC_OPCODE(Msg))
        {
            case AV_CEC_MSG_ROUTE_CHANGE:
                AvHandleEvent(port, AvEventCecMsgRouteChange, Msg, NULL);
                break;

            case AV_CEC_MSG_ROUTE_INFO:
                AvHandleEvent(port, AvEventCecMsgRouteInfo, Msg, NULL);
                break;

            case AV_CEC_MSG_ACTIVE_SRC:
                AvHandleEvent(port, AvEventCecMsgActiveSource, Msg, NULL);
                break;


            case AV_CEC_MSG_GIVE_PHYS_ADDR:
                AvHandleEvent(port, AvEventCecMsgGivePhyAddr, Msg, NULL);
                break;

            case AV_CEC_MSG_SET_STRM_PATH:
                AvHandleEvent(port, AvEventCecMsgSetStreamPath, Msg, NULL);
                break;

            case AV_CEC_MSG_STANDBY:
                AvHandleEvent(port, AvEventCecMsgStandby, Msg, NULL);
                break;

            case AV_CEC_MSG_ABORT:
                AvHandleEvent(port, AvEventCecMsgAbort, Msg, NULL);
                break;

            case AV_CEC_MSG_FEATURE_ABORT:
                AvHandleEvent(port, AvEventCecMsgFeatureAbort, Msg, NULL);
                break;

            case AV_CEC_MSG_GIVE_PWR_STATUS:
                AvHandleEvent(port, AvEventCecMsgGivePowerStatus, Msg, NULL);
                break;

            case AV_CEC_MSG_REPORT_PWR_STATUS:
                AvHandleEvent(port, AvEventCecMsgReportPowerStatus, Msg, NULL);
                break;

            case AV_CEC_MSG_GIVE_OSD_NAME:
                AvHandleEvent(port, AvEventCecMsgGiveOsdName, Msg, NULL);
                break;

            case AV_CEC_MSG_GET_VENDOR_ID:
                AvHandleEvent(port, AvEventCecMsgGetVendorId, Msg, NULL);
                break;

            case AV_CEC_MSG_GET_CEC_VERSION:
                AvHandleEvent(port, AvEventCecMsgGetCecVersion, Msg, NULL);
                break;

            case AV_CEC_MSG_REQUEST_ARC_INITIATION:
                AvKapiArcEnable(port, 1);
                /* port->content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE; */
                AvHandleEvent(port, AvEventCecMsgRequestArcInitiation, Msg, NULL);
                break;

            case AV_CEC_MSG_REPORT_ARC_INITIATED:
                AvKapiArcEnable(port, 1);
                port->content.cec->EnableARC = AV_CEC_ARC_INITIATED;
                AvHandleEvent(port, AvEventCecMsgReportArcInitiated, Msg, NULL);
                break;

            case AV_CEC_MSG_REQUEST_ARC_TERMINATION:
                AvKapiArcEnable(port, 0);
                /* port->content.cec->EnableARC = AV_CEC_ARC_TO_TERMINATE; */
                AvHandleEvent(port, AvEventCecMsgRequestArcTermination, Msg, NULL);
                break;

            case AV_CEC_MSG_REPORT_ARC_TERMINATED:
                AvKapiArcEnable(port, 0);
                port->content.cec->EnableARC = AV_CEC_ARC_TERMINATED;
                AvHandleEvent(port, AvEventCecMsgReportArcTerminated, Msg, NULL);
                break;

            case AV_CEC_MSG_USER_CONTROL_PRESSED:
                AvHandleEvent(port, AvEventCecMsgUserControlPressed, Msg, NULL);
                break;

            case AV_CEC_MSG_USER_CONTROL_RELEASED:
                AvHandleEvent(port, AvEventCecMsgUserControlReleased, Msg, NULL);
                break;
            case AV_CEC_MSG_IMAGE_VIEW_ON:
                AvHandleEvent(port, AvEventCecMsgImageViewOn, Msg, NULL);
                break;

            case AV_CEC_MSG_TEXT_VIEW_ON:
                AvHandleEvent(port, AvEventCecMsgTextViewOn, Msg, NULL);
                break;

            case AV_CEC_MSG_SYSTEM_AUDIO_MODE_REQUEST:
                AvHandleEvent(port, AvEventCecMsgSystemAudioModeRequest, Msg, NULL);
                break;

            case AV_CEC_MSG_DECK_CONTROL:
                AvHandleEvent(port, AvEventCecMsgDeckControl, Msg, NULL);
                break;

            case AV_CEC_MSG_PLAY:
                AvHandleEvent(port, AvEventCecMsgPlay, Msg, NULL);
                break;

            case AV_CEC_MSG_GIVE_AUDIO_STATUS:
                AvHandleEvent(port, AvEventCecMsgGiveAudioStatus, Msg, NULL);
                break;

            case AV_CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS:
                AvHandleEvent(port, AvEventCecMsgGiveSystemAudioModeStatus, Msg, NULL);
                break;

            case AV_CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR:
                AvHandleEvent(port, AvEventCecMsgRequestShortAudioDescriptor, Msg, NULL);
                break;

            case AV_CEC_MSG_SYSTEM_AUDIO_MODE_STATUS:
                AvHandleEvent(port, AvEventCecMsgSystemAudioModeStatus, Msg, NULL);
                break;

            case AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE:
                AvHandleEvent(port, AvEventCecMsgSetSystemAudioMode, Msg, NULL);
                break;

            case AV_CEC_MSG_REPORT_AUDIO_STATUS:
                AvHandleEvent(port, AvEventCecMsgReportAudioStatus, Msg, NULL);
                break;

            case AV_CEC_MSG_SET_AUDIO_RATE:
                AvHandleEvent(port, AvEventCecMsgSetAudioRate, Msg, NULL);
                break;

            case AV_CEC_MSG_REPORT_SHORT_AUDIO_DESCRIPTOR:
                AvHandleEvent(port, AvEventCecMsgReportShortAudioDescriptor, Msg, NULL);
                break;

            case AV_CEC_MSG_REPORT_PHYS_ADDR:
                AvHandleEvent(port, AvEventCecMsgReportPhyAddr, Msg, NULL);
                break;

            case AV_CEC_MSG_MENU_REQUEST:
                AvHandleEvent(port, AvEventCecMsgMenuRequest, Msg, NULL);
                break;

            case AV_CEC_MSG_REQ_ACTIVE_SRC:
                AvHandleEvent(port, AvEventCecMsgRequestActiveSource, Msg, NULL);
                break;

            case AV_CEC_MSG_SET_MENU_LANGUAGE:
                AvHandleEvent(port, AvEventCecMsgSetMenuLanguage, Msg, NULL);
                break;

            case AV_CEC_MSG_DEVICE_VENDOR_ID:
                AvHandleEvent(port, AvEventCecMsgDeviceVendorId, Msg, NULL);
                break;

            default:
                if ((!AV_CEC_BROADCAST(Msg)) &&
                   (AV_CEC_OPCODE(Msg) != AV_CEC_MSG_CDC_MESSAGE) )
                {
                    AvHandleEvent(port, AvEventCecSendFeatureAbort, Msg, NULL);
                    /* SendFeatureAbort(Msg); */
                }
                else
                {
                    AvUapiOutputDebugMessage("CEC: Take no action");
                }
                break;
        }

    }
    else
    {
        switch (AV_CEC_OPCODE(Msg))
        {
            case AV_CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR:
            if ( (!AV_CEC_BROADCAST(Msg)) && (port->content.cec->RxLen > 2 ))
                {
                    AvHandleEvent(port, AvEventCecSendFeatureAbortReason, Msg, NULL);
                    /* SendFeatureAbortReason(Msg,0); */
                }
                else
                {
                     AvUapiOutputDebugMessage("CEC: Take no action");
                }
                break;
            default:
                AvUapiOutputDebugMessage("CEC: Take no action");
                break;

        }

    }
    return ret;
}

/**
 * @brief
 * function to Send CEC Message
 * @return AvOk - success
 * @note
 */
AvRet KfunCecSendMessage(AvPort *port)
{
    AvRet ret = AvOk;
    port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
    AvUapiCecSendMessage(port);
    return ret;
}

/**
 * @brief
 * function to Set System Audio Mode
 * @return AvOk - success
 * @note
 */
void SendSetSystemAudioMode (AvPort* port, uint8 status)
{
    port->content.cec->TxLen=3;
    port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
    AV_CEC_SET_HDR_DA(port->content.cec->TxContent,
                   0x00,
                   port->content.cec->LogAddr);
    AV_CEC_SET_OPCODE(port->content.cec->TxContent,
                   AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE);
    port->content.cec->TxContent[2] = status;
    AvUapiCecSendMessage(port);
}

/**
 * @brief
 * function to perform logical address allocation
 * @return AvOk - success
 * @note
 */
AvRet CecAllocateLogAddr(AvPort* port)
{
    AvRet ret;

    if(*port->content.is_CecFsm != AvFsmCecTxLogAddr)
        return AvNotAvailable;

    /*=========================================
     * Copy message parameters to local vars
     *========================================*/
    port->content.cec->TxLen = 1;
    port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;

    /*=========================================
     * Kick-off transmission
     *========================================*/
    port->content.cec->TxContent[0] = (port->content.cec->AddrIndex & 0xf) |
                                     ((port->content.cec->AddrIndex & 0xf) << 4);
    ret = AvUapiCecSendMessage(port);
    return ret;
}

/**
 * @brief
 * function to List all active logical addresses in the system
 * @return AvOk - success
 * @note
 */
AvRet KfunPrepareLogAddr (AvPort* port)
{
    uint8 ErrCode = 0;
    uint8 FoundLogAddr = 0;
    /* AvUapiOutputDebugMessage("CEC: TxSendFlag = %d.", port->content.cec->TxSendFlag); */

    if(port->content.cec->TxSendFlag == AV_CEC_TX_TO_SEND)
    {
        AvUapiOutputDebugMessage("CEC: Found TxSendFlag = 1!!!!!!!!");
        return AvOk;
    }
    else if((port->content.cec->TxSendFlag == AV_CEC_TX_WAIT_RESPONSE) ||
            (port->content.cec->TxSendFlag == AV_CEC_TX_SEND_SUCCESS) ||
            (port->content.cec->TxSendFlag == AV_CEC_TX_SEND_FAIL))
    {
        /* when there are CEC feedback information, process it */
        if(port->content.cec->TxReady ||
            port->content.cec->ArbLost ||
            port->content.cec->Timeout)
        {
            /* Clear the Send Flag */
            port->content.cec->TxSendFlag = AV_CEC_TX_IDLE;
            /*======================================
             * Acknowledged, log address is taken
             *=====================================*/
            if (port->content.cec->TxReady)
            {
                port->content.cec->LogicAddrList |= (1<<(port->content.cec->AddrIndex));
                AvUapiOutputDebugMessage("CEC Port %d: %d Exists",
                                         port->index, port->content.cec->AddrIndex);
                /* force 0x05 for ARC */
#if (AvCecLogicAddress == 5)
                if(port->content.cec->AddrIndex == 5)
                {
                    FoundLogAddr = port->content.cec->AddrIndex;
                    AvHandleEvent(port, AvEventCecLogAddrAlloc, &FoundLogAddr, NULL);
                }
#endif
                if(port->content.cec->AddrIndex < 15)
                    port->content.cec->AddrIndex++;
            }
            else if(port->content.cec->Timeout)
            {
                AvUapiGetNackCount(port);
                /*=============================================
                 * No response, logical addr available, exit
                 *============================================*/
                if (port->content.cec->NackCount == (3 + 1)) /* CEC_RETRY_COUNT = 3 */
                {
                    /* find the avaialbe addr */
                    FoundLogAddr = port->content.cec->AddrIndex;
                    AvHandleEvent(port, AvEventCecLogAddrAlloc, &FoundLogAddr, NULL);
                    AvUapiOutputDebugMessage("CEC Port %d: %d Available",
                                             port->index, port->content.cec->AddrIndex);
                    if(port->content.cec->AddrIndex < 15)
                        port->content.cec->AddrIndex++;
                }
                else
                {
                    ErrCode = 0x10;                 /* Try same logical addr again */
                }
            }
            else if (port->content.cec->ArbLost)
            {
                ErrCode = 0x11;                     /* Try same logical addr again */
            }

            /*====================================
             * If need to resend
             *===================================*/
            if (ErrCode)
            {
                port->content.cec->RetryCount++;
                /* resend previous index again */
                if (port->content.cec->RetryCount > 3) /* CEC_RETRY_COUNT = 3 */
                {
                    port->content.cec->RetryCount = 0;
                    FoundLogAddr = port->content.cec->AddrIndex;
                    AvHandleEvent(port, AvEventCecLogAddrAlloc, &FoundLogAddr, NULL);
                    if(port->content.cec->AddrIndex < 15)
                        port->content.cec->AddrIndex++;
                    AvUapiOutputDebugMessage("CEC Port %d: %d Available",
                                             port->index, port->content.cec->AddrIndex);
                }
                else
                {
                    CecAllocateLogAddr(port);
                }
            }
            else
            {
                port->content.cec->RetryCount = 0;
            }
        }
    }
    else
    {
        if(CecAllocateLogAddr(port) == AvOk)
        {
            port->content.cec->RetryCount = 0;
            port->content.cec->NackCount  = 0;
        }
    }

    return AvOk;
}

/**
 * @brief
 * function to Initialize CEC FSM
 * @return AvOk - success
 * @note
 */
AvRet KfunCecInit (AvPort* port)
{
    AvUapiTxCecInit(port);
    if(port->content.cec->EnableAudioAmplifier == AV_CEC_AMP_ENABLED)
        port->content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
    if(port->content.cec->EnableAudioAmplifier == AV_CEC_AMP_DISABLED)
        port->content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
    if(port->content.cec->EnableARC == AV_CEC_ARC_INITIATED)
        port->content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
    return AvOk;
}

/**
 * @brief
 * function to Set Physical Address
 * @return AvOk - success
 * @note
 */
AvRet KfunCecSetPhysicalAddr(pin AvPort *port)
{
    AvUapiTxCecSetPhysicalAddr(port);
    return AvOk;
}

/**
 * @brief
 * function to Set Logical Address
 * @return AvOk - success
 * @note
 */
AvRet KfunCecSetLogicalAddr(pin AvPort *port)
{
    AvUapiTxCecSetLogicalAddr(port);
    return AvOk;
}

AvRet KfunCecManage(AvPort *port)
{
    AvRet ret = AvOk;
    uint8 CurrentInput = port->content.cec->InputCount>>4;
    AvPort *CurrentRx = KfunLookupFromPort(port);
    uint8 NewSpa[2];
    uint8 OldSpa[2];
    NewSpa[0] = (port->content.tx->PhyAddr)>>8;
    NewSpa[1] = (port->content.tx->PhyAddr)&0xff;
    OldSpa[0] = NewSpa[0];
    OldSpa[1] = NewSpa[1];

    /* 1. Check CEC line is free */
    if((port->content.cec->TxSendFlag == AV_CEC_TX_WAIT_RESPONSE) ||
       (port->content.cec->TxSendFlag == AV_CEC_TX_TO_SEND))
        return AvNotAvailable;

    /* 2. front port exists */
    if(CurrentRx)
    {
        /* only act when input is stable */
        if(CurrentRx->content.rx->IsInputStable == 1)
        {
            /* 2.1 generate the phy for source */
            KfunGenerateSourceSpa(CurrentRx, NewSpa, 0);
            /* 2.2.1 source not indicated */
            if(CurrentInput == 0)
            {
                port->content.cec->InputCount = ((CurrentRx->index + 1)<<4) | port->content.cec->InputCount;
#if AvCecLinkageMode
                AvHandleEvent(port, AvEventCecSendRoutingInformation, NewSpa, NULL);
#endif
            }
            /* 2.2.2 new source */
            else if(CurrentInput != (CurrentRx->index + 1))
            {
                KfunGenerateSourceSpa(CurrentRx, OldSpa, CurrentInput);
#if AvCecLinkageMode
                AvHandleEvent(port, AvEventCecSendRoutingChange, OldSpa, NewSpa);
#endif
                port->content.cec->InputCount = ((CurrentRx->index + 1)<<4) | (port->content.cec->InputCount & 0x0f);
            }
        }
    }
    /* no rx */
    else
    {
        /* but already assigned CurrentInput */
        if(CurrentInput != 0)
        {
#if AvCecLinkageMode
            AvHandleEvent(port, AvEventCecSendInactiveSource, NULL, NULL);
#endif
            port->content.cec->InputCount = port->content.cec->InputCount & 0x0f;
        }
    }

    return ret;
}

#endif /* CEC Related */

#if AvEnableCcaFeature /* CCA Related */
/* PART 8 */
/* CCA TX FSM */
void KfunCcaTxInit(AvPort *port)
{
    uint8 Enable = port->content.ccatx->Enable;

    AvUapiCcaTxInit(port);
    AvMemset(port->content.ccatx, 0, sizeof(AvCcaTx));
    port->content.ccatx->Enable = Enable;
    AvUapiCcaTxChainReset(port);
    return;
}
void KfunCcaTxCalibManage(AvPort *port)
{
    AvUapiCcaTxCalibManage(port);
    return;
}
void KfunCcaTxLinkManage(AvPort *port)
{
    uint8 Edid[AvEdidMaxSize];
    uint8 Spa[2] = {0x00,0x00};

    if(port->content.ccatx->CcaEdidStatus == AV_EDID_SUCCESS)
    {
        AvUapiTxReadEdid(port, Edid, AvEdidMaxSize);
        KfunFindCecSPAFromEdid(Edid, Spa);
        if((Edid[6] == 0xFF) && (Edid[7] == 0x00))
        {
#if AvEdidSameNoAcknowledge
            if((Edid[127] != port->content.tx->EdidCks[0]) ||
                (Edid[255] != port->content.tx->EdidCks[1]))
            {
                port->content.tx->EdidCks[0] = Edid[127];
                port->content.tx->EdidCks[1] = Edid[255];
                AvHandleEvent(port, AvEventPortEdidReady, Edid, Spa);
            }
            else
            {
                if(KfunFindVideoRxFront(port, &SourcePort) == AvOk)
                {
                    if((SourcePort->type == HdmiRx) &&
                       (SourcePort->content.rx->Hpd == AV_HPD_LOW))
                        AvHandleEvent(port, AvEventPortEdidReady, Edid, Spa);
                }
            }
#else
            AvHandleEvent(port, AvEventPortEdidReady, Edid, Spa);
#endif
        }
        port->content.ccatx->CcaEdidStatus = AV_EDID_UPDATED;
    }
    return;
}
void KfunCcaTxUnlockProtection(AvPort *port)
{
    AvUapiCcaTxChainReset(port);
    return;
}
void KfunCcaTxSetWorkMode(AvPort *port)
{
    AvUapiCcaTxSetWorkMode(port);
    return;
}

void KfunCcaTxEnablePin(AvPort *port, uint8 SlotName)
{
    AvUapiCcaTxEnablePin(port, SlotName);
    return;
}

/* PART 9 */
/* CCA RX FSM */
void KfunCcaRxInit(AvPort *port)
{
    uint8 Enable = port->content.ccarx->Enable;

    AvUapiCcaRxInit(port);
    AvMemset(port->content.ccarx, 0, sizeof(AvCcaRx));
    port->content.ccarx->Enable = Enable;
    return;
}
void KfunCcaRxCalibManage(AvPort *port)
{
    AvUapiCcaRxCalibManage(port);
    return;
}
void KfunCcaRxLinkManage(AvPort *port)
{
    return;
}
void KfunCcaRxUnlockProtection(AvPort *port)
{
    AvUapiCcaRxChainReset(port);
    return;
}

void KfunCcaRxEnablePin(AvPort *port, uint8 SlotName)
{
    AvUapiCcaRxEnablePin(port, SlotName);
    return;
}
#endif /* CCA Related */

AvRet KfunCecUpdateFromEdid(AvPort *port)
{
    uint8 Edid[AvEdidMaxSize];
    uint8 Spa[2] = {0x00,0x00};
    uint8 SpaLocation = 0x00;
    AvPort *SourcePort = NULL;

    /* Edid Already Read, return */
    if(port->content.tx->EdidReadSuccess == AV_EDID_UPDATED)
        return AvOk;

#if GSV5K1
    if(port->device->type == Gsv5k1)
    {
        if(TrxAvUpStreamFlag == 1)
        {
            #if ((AvHdmiOnEthernet == 1) || (AvEnableCcaFeature == 1))
            if(port->index == 5)
            {
                port->content.tx->HdmiMode = 1;
                return AvOk;
            }
            #endif
        }
        if(TrxAvUpStreamFlag == 0)
        {
            #if (AvEthCascading == 1)
            if(port->index == 5)
            {
                port->content.tx->HdmiMode = 1;
                return AvOk;
            }
            #endif
        }
    }
#endif

    if(port->type == HdmiTx)
    {
        AvUapiTxReadEdid(port, Edid, AvEdidMaxSize);

        if(pvpstu8FuncGsvUserEdidSaveFunction != NULL)
        {
            pvpstu8FuncGsvUserEdidSaveFunction(port, Edid, 256);
        }

        /* Incorrect Edid, Return */
        if((Edid[6] != 0xFF) || (Edid[7] != 0x00))
        {
            AvUapiOutputDebugFsm("Port %d: Edid Checksum Fail, Abort",port->index);
#if AvForceDefaultEdid
            port->content.tx->IgnoreEdidError = 1;
            KfunTxUseDefaultEdid(port);
#else
            port->content.tx->Hpd = AV_HPD_FORCE_LOW;
#endif
            return AvError;
        }
        SpaLocation = KfunFindCecSPAFromEdid(Edid, Spa);
        if((SpaLocation > 0x82) && (SpaLocation < 0xFE))
            port->content.tx->PhyAddr = (Spa[0]<<8) | Spa[1];
        else
            port->content.tx->PhyAddr = 0x0000;
    #if AvEnableCecFeature /* CEC Related */
        AvKapiCecSetPhysicalAddr(port);
    #endif

    if(port->device->type == Gsv5k1)
    {
        if(((Spa[0] == 0) && (Spa[1] == 0)) ||
           (SpaLocation < 0x82) || (SpaLocation > 0xFE))
            port->content.tx->HdmiMode = 0;
        else
            port->content.tx->HdmiMode = 1;
    }
    else
    {
        #if (AvGsv2kxTXSwitchHDMIAndDVI==0)
        if(((Spa[0] == 0) && (Spa[1] == 0)) ||
           (SpaLocation < 0x82) || (SpaLocation > 0xFE))
            port->content.tx->HdmiMode = 0;
        else
            port->content.tx->HdmiMode = 1;
        #endif
    }
    #if AvEdidSameNoAcknowledge
        if((Edid[127] != port->content.tx->EdidCks[0]) ||
           (Edid[255] != port->content.tx->EdidCks[1]))
        {
            port->content.tx->EdidCks[0] = Edid[127];
            port->content.tx->EdidCks[1] = Edid[255];
            AvHandleEvent(port, AvEventPortEdidReady, Edid, Spa);
        }
        else
        {
            if(KfunFindVideoRxFront(port, &SourcePort) == AvOk)
            {
                if((SourcePort->type == HdmiRx) &&
                   (SourcePort->content.rx->Hpd == AV_HPD_LOW))
                    AvHandleEvent(port, AvEventPortEdidReady, Edid, Spa);
            }
        }
    #else
        AvHandleEvent(port, AvEventPortEdidReady, Edid, Spa);
    #endif
    }
    AvUapiTxSetHdmiModeSupport(port);
    AvUapiTxSetFeatureSupport(port);

    return AvOk;
}

/**
 * @brief
 * function to use Default EDID
 * @return AvOk - success
*/
AvRet KfunTxUseDefaultEdid(pin AvPort *port)
{
    /* Allow user to customize the code */
    AvHandleEvent(port, AvEventTxDefaultEdid, NULL, NULL);
    if(port->device->type == Gsv5k1)
    {
        port->content.tx->HdmiMode = 1;
    }
    else
    {
        #if (AvGsv2kxTXSwitchHDMIAndDVI==0)
        /* Force Tx to output HDMI mode when using default Edid */
        port->content.tx->HdmiMode = 1;
        #endif
    }
    AvUapiTxSetHdmiModeSupport(port);
    return AvOk;
}

AvRet KfunTxEdidError(pin AvPort *port)
{
    /* Allow user to customize the code */
    AvHandleEvent(port, AvEventPortEdidReadFail, NULL, NULL);
    return AvOk;
}

void KfunTxReadEdid(AvPort *port, uint8 *ReadData)
{
    AvUapiTxReadEdid(port, ReadData, AvEdidMaxSize);
}

void KfunRxWriteEdid(pio AvPort *port, uint8 *MergedEdid, uint8 SpaLocation, uint8 *SpaValue)
{
    uint16 len = 256;
    if(MergedEdid[0x7E] == 0x02)
        len = 384;
    else if(MergedEdid[0x7E] == 0x03)
        len = 512;
    AvUapiRxWriteEdid(port, MergedEdid, len);
    AvUapiRxSetSpa(port, SpaLocation, SpaValue);
    AvUapiRxEnableInternalEdid(port);

    #if 1
    {
        u8 i;
        AvKapiOutputDebugMessage("Rx %d Generated Edid Start: ",port->index+1);
        for(i=0;i<16;i++)
        {
            AvKapiOutputDebugMessage("0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,"\
                                     "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                     MergedEdid[i*16+0],MergedEdid[i*16+1],MergedEdid[i*16+2],MergedEdid[i*16+3],
                                     MergedEdid[i*16+4],MergedEdid[i*16+5],MergedEdid[i*16+6],MergedEdid[i*16+7],
                                     MergedEdid[i*16+8],MergedEdid[i*16+9],MergedEdid[i*16+10],MergedEdid[i*16+11],
                                     MergedEdid[i*16+12],MergedEdid[i*16+13],MergedEdid[i*16+14],MergedEdid[i*16+15]);
        }
    }
    #endif
}

void KfunHdcp2p2Manage(pio AvPort *port)
{
#if AvEnableHdcp2p2Feature
    /* report some information about HDCP 2.2 although it is automatic */
    AvUapiRxHdcp2p2Manage(port);
#endif
}

uint8 KfunFindCecSPAFromEdid(uint8 *currptr, uint8 *spa)
{
    uint8 i = 0;
    uint8 offset = 0;
    uint8 DTDstart = 0;
    uint8 data1, data2, data3;
    uint32 vsdbid;
    uint8 ret = 0x01;

    /* Step 0. clear 2 bytes of SPA */
    spa[0] = 0xFF;
    spa[1] = 0xFF;
    /* Step 1. Check if the 128~255 bytes exist, return if not exist */
    if(currptr[0x7E] == 0)
        return ret;
    /* Step 2. check if 128~255 is a CEA block, only support version 3 */
    if((currptr[0x80] != 0x02) || (currptr[0x81] != 0x03))
        return ret;
    /* Step 3. find the DTD starter, SPA exists before DTD */
    if(currptr[0x82] >= 4)
        DTDstart = 0x80 + currptr[0x82];
    else
        DTDstart = 0xff;
    /* Step 4. set the offset of first data block */
    offset = 0x84;

    /* loop to find SPA */
    while(offset < DTDstart)
    {
        /* VSDB block found */
        if((currptr[offset]&0xe0) == 0x60)
        {
            data1 = currptr[offset+1];
            data2 = currptr[offset+2];
            data3 = currptr[offset+3];
            vsdbid = (data1) | (data2 << 8) | (data3 << 16);
            /* 24-bit IEEE Registeration */
            if (vsdbid != TAG_VSDB_IDENTIFIER)
                return ret;
            else
            {
                spa[0] = currptr[offset+4];
                spa[1] = currptr[offset+5];
                AvUapiOutputDebugMessage("SPA Found =%02x,%02x", spa[0], spa[1]);
                return (offset+4);
            }
        }
        /* other blocks or invalid blocks found */
        else
        {
            i = currptr[offset]&0x1f;
            /* invalid block found, quit */
            if(i == 0)
                return ret;
            else
                offset = offset + i + 1;
        }
    }

    return ret;
}

void KfunGenerateSourceSpa(AvPort *port, uint8 *Spa, uint8 DedicateAddr)
{
    AvPort *DefaultPort = (AvPort*)port->device->port;
    uint8 NewAddr;
    /* 1. set inserted value: rule - index number + 1 */
    if(DedicateAddr == 0)
        NewAddr = 1 + port->index - DefaultPort->index;
    else
        NewAddr = DedicateAddr;
    /* 2. find existed shift */
    /* 1.1 fully occupied */
    if((Spa[1] & 0x0f) != 0x00)
        return;
    /* 1.2 other cases */
    else if(Spa[1] & 0xf0)
        Spa[1] = Spa[1] | NewAddr;
    else if(Spa[0] & 0x0f)
        Spa[1] = NewAddr<<4;
    else if(Spa[0] & 0xf0)
        Spa[0] = Spa[0] | NewAddr;
    else
        Spa[0] = NewAddr<<4;

    return;
}

void KfunTxSetColorDepth(AvPort *RxPort, AvPort *TxPort)
{
    AvVideoCd InColorDepth = AV_CD_24;
    uint8 TxCoreIn4KValid = 0;
    uint8 TxCoreInColorSpace = AV_Y2Y1Y0_YCBCR_444;
    AvPort *FromPort = NULL;
    /* 1. Set Default Color Depth */
#if 1
    if((RxPort->device->type == Gsv5k1)&&(RxPort->content.rx->IsInputStable == 1))
    {
        FromPort = (AvPort *)TxPort->content.RouteVideoFromPort;
        while(FromPort != NULL)
        {
            if((FromPort->type == HdmiRx) || (FromPort->type == CcaRx))
            {
                /* 1.1 Set Default Color Depth */
                TxCoreInColorSpace = FromPort->content.video->Y;
                InColorDepth = FromPort->content.video->Cd;
                TxPort->content.video->Cd = InColorDepth;
                if(FromPort->content.video->info.TmdsFreq > 275)
                    TxCoreIn4KValid = 1;
                break;
            }
            else if(FromPort->type == VideoDsc)
            {
                if(FromPort->content.dsc->DscInCompressed == 1)
                {
                    /* 1.2 Set Default Color Depth */
                    TxCoreInColorSpace = FromPort->content.dsc->SrcInY;
                    InColorDepth = FromPort->content.dsc->SrcInCd;
                    TxPort->content.video->Cd = InColorDepth;
                    if((FromPort->content.dsc->DscInVic >= 93) &&
                       (FromPort->content.dsc->DscInVic <= 107))
                        TxCoreIn4KValid = 1;
                }
                break;
            }
            else
                FromPort = (AvPort *)FromPort->content.RouteVideoFromPort;
        }
    }
    else
    {
        TxPort->content.video->Cd = RxPort->content.video->Cd;
        InColorDepth = RxPort->content.video->Cd;
        //TxPort->content.video->Cd = RxPort->content.video->Cd;
    }
#endif
    /* 2. 12-bit process */
    if(InColorDepth == AV_CD_36)    //(RxPort->content.video->Cd == AV_CD_36)
    {
        /* 2.1 normal deep color 12-bit */
        if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_12B_DEEP_COLOR) == 0)
            TxPort->content.video->Cd = AV_CD_24;
        /* 2.2 clock divider required, 450M */
        else if(TxCoreIn4KValid == 1)   //(RxPort->content.video->info.TmdsFreq > 275)
        {
            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4G5) == 0)
                TxPort->content.video->Cd = AV_CD_24;
            else if((TxCoreInColorSpace == AV_Y2Y1Y0_YCBCR_420) &&  //(RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420) &&
                    ((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_36B420) == 0))
                TxPort->content.video->Cd = AV_CD_24;
        }
    }
    /* 3. 10-bit process */
    if(InColorDepth == AV_CD_30)    //(RxPort->content.video->Cd == AV_CD_30)
    {
        /* 3.1 normal deep color 10-bit */
        if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_10B_DEEP_COLOR) == 0)
            TxPort->content.video->Cd = AV_CD_24;
        /* 3.2 clock divider required, 375M */
        else if(TxCoreIn4KValid == 1)   //(RxPort->content.video->info.TmdsFreq > 275)
        {
            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_3G75) == 0)
                TxPort->content.video->Cd = AV_CD_24;
            else if((TxCoreInColorSpace == AV_Y2Y1Y0_YCBCR_420) &&  //(RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420) &&
                    ((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_30B420) == 0))
                TxPort->content.video->Cd = AV_CD_24;
        }
    }
    #if AvGsv2k8VideoRgb_422_444To420
    if(g_u8Gsv2k8ForceCscMode[RxPort->device->index][RxPort->index] != emTxCscMode_ToNone)
    {
        switch(g_u8Gsv2k8ForceCscMode[RxPort->device->index][RxPort->index])
        {
            case emTxCscMode_To420:
                //OutY = AV_Y2Y1Y0_YCBCR_420;
            case emTxCscMode_ToOnlyCD8Bit:
                TxPort->content.video->Cd = AV_CD_24;
                break;
            default:
                //OutY = AV_Y2Y1Y0_YCBCR_420;
                //port->content.video->Cd = AV_CD_24;
                break;
        }
    }
    #endif

#if AvGsv2k2Video420ToRgb_422_444
    if(g_bGsv2k2InputIsChange[RxPort->device->index][RxPort->index])
    {
        switch(g_stRxAvInfo[RxPort->device->index].u8ColorData)
        {
            case AV_CD_24:
                {
                    TxPort->content.video->Cd = AV_CD_24;
                }
                break;
            case AV_CD_30:
                {
                    TxPort->content.video->Cd = AV_CD_30;
                }
                break;
            case AV_CD_36:
                {
                    TxPort->content.video->Cd = AV_CD_36;
                }
                break;
            case AV_CD_48:
                {
                    TxPort->content.video->Cd = AV_CD_48;
                }
                break;
            default:
                break;
        }
    }
#endif
    /* 4. deep color protection for frequency around AvCompressionLowThreshold */
    if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE)
    {
        FromPort = (AvPort *)TxPort->content.RouteVideoFromPort;
        if(((FromPort->type == HdmiRx) || (FromPort->type == CcaRx)) &&
           (FromPort->content.video->info.LaneFreq > AvCompressionLowThreshold))
            TxPort->content.video->Cd = AV_CD_24;
    }

    AvUapiOutputDebugMessage("Tx Port %d: Color Depth = %d", TxPort->index, TxPort->content.video->Cd);
}

void KfunTxSetHdmiModeSupport(pout AvPort *port)
{
    if((port->type == HdmiTx)|| (port->type == CcaTx))
    {
        AvUapiTxSetHdmiModeSupport(port);
        port->content.video->AvailableVideoPackets =
            port->content.video->AvailableVideoPackets & (~AV_BIT_AV_INFO_FRAME);
    }
}

#if GSV5K1
void KfunRxVicFilter(pin AvPort *port)
{
    uint8 Value = 0;
    uint16 i = 0;
    /* 1. Check Vesa Timing */
    for(i=0;VesaTimingTables[i].Vic != 255;i++)
    {
        if((VesaTimingTables[i].VTotal  == port->content.video->timing.VTotal) &&
           (VesaTimingTables[i].VSync   == port->content.video->timing.VSync)  &&
           (VesaTimingTables[i].VBack   == port->content.video->timing.VBack)  &&
           (VesaTimingTables[i].VActive == port->content.video->timing.VActive) &&
           (((VesaTimingTables[i].HTotal  == port->content.video->timing.HTotal) &&
             (VesaTimingTables[i].HSync   == port->content.video->timing.HSync) &&
             (VesaTimingTables[i].HBack   == port->content.video->timing.HBack) &&
             (VesaTimingTables[i].HActive == port->content.video->timing.HActive)) ||
            ((VesaTimingTables[i].HTotal  == port->content.video->timing.HTotal*2) &&
             (VesaTimingTables[i].HSync   == port->content.video->timing.HSync*2) &&
             (VesaTimingTables[i].HBack   == port->content.video->timing.HBack*2) &&
             (VesaTimingTables[i].HActive == port->content.video->timing.HActive*2))))
        {
            port->content.video->timing.Vic = VesaTimingTables[i].Vic;
            Value = 1;
            break;
        }
    }
    /* 2. Check Cea Timing */
    if(Value == 0)
    {
        for(i=0;CeaTimingTables[i].Vic != 255;i++)
        {
            if((CeaTimingTables[i].VTotal  == port->content.video->timing.VTotal) &&
               (CeaTimingTables[i].VSync  == port->content.video->timing.VSync) &&
               (CeaTimingTables[i].VBack  == port->content.video->timing.VBack) &&
               (CeaTimingTables[i].VActive == port->content.video->timing.VActive) &&
               (((CeaTimingTables[i].HTotal  == port->content.video->timing.HTotal) &&
                 (CeaTimingTables[i].HSync   == port->content.video->timing.HSync) &&
                 (CeaTimingTables[i].HBack   == port->content.video->timing.HBack) &&
                 (CeaTimingTables[i].HActive == port->content.video->timing.HActive)) ||
                ((CeaTimingTables[i].HTotal  == port->content.video->timing.HTotal*2) &&
                 (CeaTimingTables[i].HSync   == port->content.video->timing.HSync*2) &&
                 (CeaTimingTables[i].HBack   == port->content.video->timing.HBack*2) &&
                 (CeaTimingTables[i].HActive == port->content.video->timing.HActive*2))) &&
               (CeaTimingTables[i].Interlaced == port->content.video->timing.Interlaced) &&
               ((CeaTimingTables[i].VFreq   == port->content.video->timing.FrameRate) ||
                (CeaTimingTables[i].VFreq   == (port->content.video->timing.FrameRate - 1))))
            {
                port->content.video->timing.Vic = CeaTimingTables[i].Vic;
                Value = 1;
                break;
            }
        }
    }
    /* 3. Correct Incorrect Vic */
    if(Value == 0)
    {
        for(i=0;VesaTimingTables[i].Vic != 255;i++)
        {
            if(port->content.video->timing.Vic == VesaTimingTables[i].Vic)
                port->content.video->timing.Vic = 0;
        }
        for(i=0;CeaTimingTables[i].Vic != 255;i++)
        {
            if(port->content.video->timing.Vic == CeaTimingTables[i].Vic)
                port->content.video->timing.Vic = 0;
        }
    }

    AvUapiOutputDebugFsm("Port%d: Filtered Vic = %x", port->index, port->content.video->timing.Vic);
    return;
}
#endif

#endif

