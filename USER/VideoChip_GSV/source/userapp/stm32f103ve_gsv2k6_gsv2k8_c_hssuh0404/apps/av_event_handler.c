/**
 * @file av_event_handler.c
 *
 * @brief av event handler to handle the events for customers use
 */

#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "av_event_handler.h"

AvEdidMode EdidHdmiMode = AvEdidModeInvalid;
uint8 DownScalingTxA = 0;
uint8 DownScalingTxB = 0;
uint8 AutoSelectInTxA = 0;
uint8 AutoSelectInTxB = 0;
uint8 AudioOutFromCore = 0;
#if GSV2K2
uint8 Gsv2k2RxBPortSel = 0;
#endif

#if GSV2K6
//uint8 DownScalingTxA = 0;
//uint8 DownScalingTxB = 0;
uint8 DownScalingTxC = 0;
uint8 DownScalingTxD = 0;
uint8 RegTxAFrom = 1;
uint8 RegTxBFrom = 1;
uint8 RegTxCFrom = 1;
uint8 RegTxDFrom = 1;
uint8 KeyRxBInput = 0;
#endif

#if GSV2K11
uint8  LogicOutputSel[AvGsv2k11DeviceNumber]  = {0};     /* 1 = HdmiRx-->VideoTx, 0 = VideoRx-->HdmiTx */
#endif

#if AvEnableCcaFeature
uint8 CcaEdidReceivedFlag[AvGsv5k1DeviceNumber] = 0;
#endif


#if GSV5K1
extern uint8 TrxAvUpStreamFlag; /* 1 = Tx, 0 = Rx */
uint8  GetDscStream[AvGsv5k1DeviceNumber] = {0};
uint8  GetCscStream[AvGsv5k1DeviceNumber] = {0};
#endif

#if AvGsv2kxVideoCscForValensRx
AvVideoY  CpsSrcInY  = AV_Y2Y1Y0_RGB;
AvVideoCd CpsSrcInCd = AV_CD_NOT_INDICATED;
AvVideoCs CpsSrcInCs = AV_CS_AUTO;
#if AvGsv2kxVideoHdcp14ForValensRxProblem
u8        u8CpsSrcInHdcp = 0;
#endif
#endif

#if AvEnableCecFeature /* CEC Related */
extern uchar  DevicePowerStatus;
extern uchar  DevicePowerStatus2;
extern char   DeviceName[20];
extern char   DeviceName2[20];
extern uchar  AudioStatus;
extern CEC_AUDIO_STATUS Cec_Tx_Audio_Status;
uint16 AvCecFindRequiredInput (AvPort *port, uint16 SrcPhys);
#endif

/* Edid Related Declare Start */
extern AvEdidReg DevEdidReg;
extern uint16    AvEdidSupportFreq;
extern uint8     AvEdidVesaParamForce;
extern uint8     AvEdidCeaParamForce;
extern uint16    AvEdidCeabParamForce;
extern uint8     AvEdidVesaParamRemove;
extern uint8     AvEdidCeaParamRemove;
extern uint16    AvEdidCeabParamRemove;
//extern uint16    AvEdidSupportAudioFormat;
//extern uint8     AvEdidAudioChannelLPCM;
uint8 InEdid[256];
uint8 SinkEdid[256];
uint8 OutEdid[256];
uint8 *g_pu8GsvSysEdid = NULL;
vpstAvPortFuncPoint pvpstFuncGsvUserEdidFunction = NULL;
#if AvGsv2k11CopyTxEdid
uint8 g_u8DisplayEdid[AvGsv2k11DeviceNumber][256];
#endif

AvRet AvEdidPortManage(AvPort *RxPort);
AvRet AvEdidPortAnalysis(AvPort *port);

extern const AvDscPlanVicTables DscPlanVicTable[];

/* Edid Related Declare End */

/**
 * @brief  process audio/video events
 * @return none
 */
AvRet AvHandleEvent(AvPort *port, AvEvent event, uint8 *wparam, uint8 *pparam)
{
    AvRet ret = AvOk;
    AvPort *TempPort;
#if GSV5K1
    AvPort *DscPort;
    uint8  value;
#endif

#if AvEnableCecFeature /* CEC Related */
    uint8 *RxContent = port->content.cec->RxContent;
    uint8 *TxContent = port->content.cec->TxContent;
    int i;
    uint8 MsgLenValue = 2;
    uint8 *Msg_Len = &MsgLenValue;
    uint16 ActiveSource = 0;
    uint16 InpIndex;
    uint16 Recv_Phy_Addr;
    uint8  DeviceType;
    uint8  Len;
    uint8  Spa[2];
#endif

    switch(event)
    {
        case AvEventRxSetHdcpStyle:
            /* Rx Hdcp Selection:
            AV_HDCP_RX_NOT_SUPPORT = 0,
            AV_HDCP_RX_AUTO        = 1,
            AV_HDCP_RX_FOLLOW_SINK = 2,
            AV_HDCP_RX_1P4_ONLY    = 3,
            AV_HDCP_RX_2P2_ONLY    = 4,
             */
            #if AvMatrix4x4Mode
            if(port->device->type == Gsv2k8)
                port->content.hdcp->HdcpNeeded = AV_HDCP_RX_NOT_SUPPORT;
            else
            #endif
                port->content.hdcp->HdcpNeeded = AV_HDCP_RX_AUTO;
            break;

        case AvEventTxSetHdcpStyle:
            /* Tx Hdcp Selection:
            AV_HDCP_TX_ILLEGAL_NO_HDCP = 0,
            AV_HDCP_TX_AUTO            = 1,
            AV_HDCP_TX_AUTO_FAIL_OUT   = 2,
            AV_HDCP_TX_1P4_ONLY        = 3,
            AV_HDCP_TX_1P4_FAIL_OUT    = 4,
            AV_HDCP_TX_2P2_ONLY        = 5,
            AV_HDCP_TX_2P2_FAIL_OUT    = 6
               Tx Dvi Selection:
            AV_HDCP_TX_DVI_STRICT      = 0,
            AV_HDCP_TX_DVI_LOOSE       = 1
            */
        #if GSV5K1
            if(port->device->type == Gsv5k1)
            {
                if(TrxAvUpStreamFlag == 1)
                {
                    if(port->index == 5)
                    {
                        port->content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                        port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_LOOSE;
                    }
                    else
                    {
                        port->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO_FAIL_OUT;
                        port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_STRICT;
                    }
                }
                if(TrxAvUpStreamFlag == 0)
                {
                    #if (AvEthCascading == 1)
                    if(port->index == 5)
                    {
                        port->content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                        port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_LOOSE;
                    }
                    else
                    #endif
                    {
                        port->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO_FAIL_OUT;
                        port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_STRICT;
                    }
                }
            }
            else
        #endif
            {
                #if AvMatrix4x4Mode
                /* does not take Front End Tx Edid into consideration */
                if(port->device->type == Gsv2k6)
                {
                    port->content.tx->IgnoreEdidError = 1;
                    port->content.hdcptx->HdcpSupport = 0;
                    port->content.hdcptx->HdmiStyle   = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    port->content.hdcptx->DviStyle    = AV_HDCP_TX_DVI_LOOSE;
                }
                else
                {
                    port->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO_FAIL_OUT;
                    port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_LOOSE;
                }
                #else
                #if (!AvEnableForceOutHdcp)
                    #if GSV2K11
                    port->content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_STRICT;
                    #else
                    port->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO_FAIL_OUT;
                    port->content.hdcptx->DviStyle  = AV_HDCP_TX_DVI_STRICT;
                    #endif
                #endif
                #endif
            }
            break;

        case AvEventPortAudioInfoChanged:
            break;

        case AvEventPortVideoInfoChanged:
            #if AvGsv2kxVideoCscForValensRx
            CpsSrcInY    = (AvVideoY)wparam[0];
            CpsSrcInCd   = (AvVideoCd)wparam[1];
            CpsSrcInCs   = (AvVideoCs)wparam[2];
            #if AvGsv2kxVideoHdcp14ForValensRxProblem
            u8CpsSrcInHdcp = (u8)wparam[3];
            #endif
            #endif
            break;

        case AvEventUpStreamConnectNewDownStream:
#if AvNoLinkageMode
#else
            if(KfunFindVideoRxFront(port, &TempPort) == AvOk)
                TempPort->content.rx->EdidStatus = AV_EDID_RESEND;
#endif
            break;

        case AvEventPortDownStreamDisconnected:
#if AvNoLinkageMode
#else
            if(KfunFindVideoRxFront(port, &TempPort) == AvOk)
                TempPort->content.rx->EdidStatus = AV_EDID_SINKLOST;
#endif
            break;

        case AvEventPortDownStreamConnected:
            break;

        case AvEventPortDownStreamSending:
            #if AvMatrix4x4Mode
            if(port->device->type == Gsv2k8)
            {
                if(KfunFindVideoRxFront(port, &TempPort) == AvOk)
                {
                    if(TempPort->type == HdmiRx)
                    {
                        /* HDMI Mode Input Judge */
                        if(port->content.tx->PhyAddr != 0x0000)
                            port->content.tx->HdmiMode = TempPort->content.rx->HdmiMode;
                        else
                            port->content.tx->HdmiMode = 0;
                    }
                }
            }
            #else
            #if GSV5K1
            if(port->device->type != Gsv5k1)
            #endif
            {
                if(KfunFindVideoRxFront(port, &TempPort) == AvOk)
                {
                    if(TempPort->type == HdmiRx)
                    {
                        #if (AvGsv2kxTXSwitchHDMIAndDVI==0)
                        /* HDMI Mode Input Judge */
                        if(TempPort->content.rx->IsInputStable == 1)
                        {
                            if((port->content.tx->IgnoreEdidError == 1) || /* sink is default as HDMI */
                               (port->content.tx->PhyAddr != 0x0000))      /* sink is HDMI */
                                port->content.tx->HdmiMode = TempPort->content.rx->HdmiMode;
                            else
                                port->content.tx->HdmiMode = 0; /* sink is DVI as only option */
                        }
                        else
                            port->content.tx->HdmiMode = 1; /* sink is default to be HDMI */
                        #endif
                        KfunTxSetHdmiModeSupport(port);
                    }
                }
            }
            #endif
            break;

        case AvEventPortUpStreamDisconnected:
            break;

        case AvEventPortUpStreamConnected:
            break;

        case AvEventRxPrepareEdid:
            if(port->content.rx->EdidStatus != AV_EDID_UPDATED)
            {
                #if GSV5K1
                if(port->device->type == Gsv5k1)
                {
                    if((TrxAvUpStreamFlag == 1))
                        AvEdidPortManage(port);
                }
                else
                #endif
                    AvEdidPortManage(port);
                port->content.rx->EdidStatus = AV_EDID_UPDATED;
            }
            break;

        case AvEventPortEdidReady:
            #if GSV5K1
            if(port->device->type == Gsv5k1)
            {
                if(TrxAvUpStreamFlag == 1)
                {
                    #if AvEnableCcaFeature
                    if(port->index == 5)
                        CcaEdidReceivedFlag[port->device->index] = 1;
                    if((port->index == 4) || (port->index == 5))
                    #else
                    /* Only Update TxA Port Edid */
                    if(port->index == 4)
                    #endif
                    {
                        AvEdidPortAnalysis(port);
                        /* Resend Edid */
                        TempPort = port->device->port;
                        if((TempPort->type == HdmiRx) || (TempPort->type == CcaRx))
                            TempPort->content.rx->EdidStatus = AV_EDID_RESEND;
                    }
                }
                if(TrxAvUpStreamFlag == 0)
                {
                    #if (AvEthCascading == 1)
                    if(port->index == 4)
                    #else
                    if(port->type == HdmiTx)
                    #endif
                    {
                        AvEdidPortAnalysis(port);
                        /* Resend Edid */
                        TempPort = port->device->port;
                        if((TempPort->type == HdmiRx) || (TempPort->type == CcaRx))
                        {
                            AvEdidPortManage(TempPort);
                            TempPort->content.rx->EdidStatus = AV_EDID_UPDATED;
                        }
                    }
                }
            }
            else
            #endif
            {
                AvEdidPortAnalysis(port);
                /* Resend Edid */
                if(KfunFindVideoRxFront(port, &TempPort) == AvOk)
                {
                    if(TempPort->type == HdmiRx)
                        TempPort->content.rx->EdidStatus = AV_EDID_RESEND;
                }
            }
            break;

        case AvEventPortEdidReadFail:
            break;

        case AvEventTxDefaultEdid:
            port->content.tx->EdidSupportFeature = 0xffffffff;
            break;

        case AvEventPortUpStreamEncrypted:
            /* Upstream input is encrypted */
            break;

        case AvEventPortUpStreamDecrypted:
            /* Upstream input is not encrypted */
            break;

        #if GSV5K1
        case AvEventPortUpStreamDscCompressed:
            if(port->device->type != Gsv5k1)
                break;
            /* 1. Upstream input is DSC compressed or not */
            /* 2. Loop to find the DSC port in the same device */
            DscPort = (AvPort*)port->device->port;
            value = DscPort->device->index;
            while((DscPort != NULL) && (DscPort->device->index == value))
            {
                /* 3. DscPort Found */
                if(DscPort->type == VideoDsc)
                {
                    /* 4.Set DscPort Params */
                    DscPort->content.dsc->Enable = wparam[0];
                    if(wparam[0] == 1)
                    {
                        GetDscStream[DscPort->device->index] = 1;
                        GetCscStream[DscPort->device->index] = 0;
                        DscPort->content.dsc->DscInCompressed = wparam[0];
                        DscPort->content.dsc->DscRatio  = (AvDscRatio)wparam[1];
                    }
                    else if(wparam[1] == AV_CSC_2BY1)
                    {
                        GetDscStream[DscPort->device->index] = 0;
                        GetCscStream[DscPort->device->index] = 1;
                    }
                    DscPort->content.dsc->DscInVic  = wparam[2];
                    DscPort->content.dsc->DscInY    = (AvVideoY)wparam[3];
                    DscPort->content.dsc->DscInCs   = (AvVideoCs)wparam[4];
                    DscPort->content.dsc->SrcInY    = (AvVideoY)wparam[5];
                    DscPort->content.dsc->SrcInHdmi = wparam[6];
                    DscPort->content.dsc->SrcInCd   = (AvVideoCd)wparam[7];
                    DscPort->content.dsc->SrcInCs   = (AvVideoCs)wparam[8];
                    DscPort->content.dsc->HdcpStat  = (AvHdcpSt)wparam[9];
                    /* 4.1 HDCP Status Update */
                    if(wparam[9] != 0)
                    {
                        port->content.rx->VideoEncrypted = 1;
                        if(wparam[9] == AV_HDCP_2P2_ENCRYPTED)
                            port->content.hdcp->Hdcp2p2RxRunning = 1;
                        else
                            port->content.hdcp->Hdcp2p2RxRunning = 0;
                    }
                    else
                    {
                        port->content.rx->VideoEncrypted = 0;
                        port->content.hdcp->Hdcp2p2RxRunning = 0;
                    }
                    break;
                }
                /* 3.1 Loop to the next port in chain */
                else
                    DscPort = (AvPort*)(DscPort->next);
            }
            break;

        case AvEventPortUpStreamNoDscInfo:
            if(port->device->type != Gsv5k1)
                break;
            GetDscStream[port->device->index] = 0;
            GetCscStream[port->device->index] = 0;
            break;
        #endif

#if AvEnableCecFeature /* CEC Related */

        case AvEventCecRxMessage:
            break;

        case AvEventCecTxDone:
            break;

        case AvEventCecTxTimeout:
            AvHandleEvent(port, AvEventCecTxError, 0, NULL);
            break;

        case AvEventCecTxArbLost:
            AvHandleEvent(port, AvEventCecTxError, 0, NULL);
            break;

            /* when kernel layer finds a valid LogAddr, it tells */
        case AvEventCecLogAddrAlloc:
            /* user can modify the LogAddr by themselves, in this example,
               LogAddr will be allocated to the 1st none-zero value, all the
               unallocated address will be poped here */
            if(*wparam == AvCecLogicAddress)
            {
                port->content.cec->LogAddr = *wparam;
                AvKapiCecSetLogicalAddr(port);
                AvKapiOutputDebugMessage("CEC Port %d: Set Logical Address = %d",
                                         port->index, port->content.cec->LogAddr);
                /* Manual Stop Logic Address Search */
                port->content.cec->AddrIndex = 15;
            }
            break;

        case AvEventCecTxError:
            /* to be inserted */
            break;

        case AvEventCecRxMessageRespond:
            ret = AvOk;
            break;

        case AvEventCecArcManage:
            if(port->content.cec->TxSendFlag == AV_CEC_TX_SEND_SUCCESS)
            {
                if(port->content.cec->ARCTryCount < 5)
                {
                    port->content.cec->ARCTryCount = port->content.cec->ARCTryCount + 1;
                    if(port->content.cec->EnableARC == AV_CEC_ARC_TO_INITIATE)
                        AvHandleEvent(port, AvEventCecSendInitiateARC, NULL, NULL);
                    else if(port->content.cec->EnableARC == AV_CEC_ARC_TO_TERMINATE)
                        AvHandleEvent(port, AvEventCecSendTerminateARC, NULL, NULL);
                }
            }
            break;
#endif

#if AvCecDataSendingOutEvent

        case AvEventCecSendRoutingChange:
            /* 0x80, init of 2.2 routing control */
            /* pparam = NewPort, wparam = OldPort */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 6;
            AV_CEC_SET_HDR_BC(TxContent, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_ROUTE_CHANGE);
            AV_CEC_SET_PHYS_ADDR1(TxContent, ((wparam[0]<<8)+wparam[1]));
            AV_CEC_SET_PHYS_ADDR2(TxContent, ((pparam[0]<<8)+pparam[1]));
            AvKapiOutputDebugMessage("CEC: Sending routing change. From Inp=%04x to Inp=%04x",
                                     ((wparam[0]<<8)+wparam[1]), ((pparam[0]<<8)+pparam[1]));
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendRoutingInformation:
            /* 0x81, response for 2.2 routing control */
            /* after EDID allocation of upper level chain, enable this feature */
            Spa[0] = (port->content.tx->PhyAddr)>>8;
            Spa[1] = (port->content.tx->PhyAddr)&0xff;
            KfunGenerateSourceSpa(port, Spa, (port->content.cec->InputCount>>4));
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 4;
            AV_CEC_SET_HDR_BC(TxContent, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_ROUTE_INFO);
            AV_CEC_SET_PHYS_ADDR1(TxContent, ((Spa[0]<<8)+Spa[1]));
            AvKapiOutputDebugMessage("CEC: Sending routing information. Inp=%04x", ((Spa[0]<<8)+Spa[1]));
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendReportPhysAddress:
            /* 0x84, report physical address */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen=5;
            switch(port->content.cec->LogAddr)
            {
                case AV_CEC_PLAYBACK1_LOG_ADDRESS:
                case AV_CEC_PLAYBACK2_LOG_ADDRESS:
                case AV_CEC_PLAYBACK3_LOG_ADDRESS:
                   DeviceType = AV_CEC_PLAYBACK1_LOG_ADDRESS;
                   break;
                case AV_CEC_RECORD1_LOG_ADDRESS:
                case AV_CEC_RECORD2_LOG_ADDRESS:
                   DeviceType = AV_CEC_RECORD1_LOG_ADDRESS;
                   break;
                case AV_CEC_TUNER1_LOG_ADDRESS:
                case AV_CEC_TUNER2_LOG_ADDRESS:
                case AV_CEC_TUNER3_LOG_ADDRESS:
                case AV_CEC_TUNER4_LOG_ADDRESS:
                   DeviceType = AV_CEC_TUNER1_LOG_ADDRESS;
                   break;
                default:
                   DeviceType = port->content.cec->LogAddr;
                   break;
            }
            AV_CEC_SET_HDR_BC(TxContent, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_REPORT_PHYS_ADDR);
            AV_CEC_SET_PHYS_ADDR1(TxContent, port->content.tx->PhyAddr);
            TxContent[4] = DeviceType;
            AvKapiOutputDebugMessage("CEC: Sending Report phys address=%x", port->content.tx->PhyAddr);
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendActiveSource:
            /* 0x82, send active source, step 2 of 2.1 one touch play */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 4;
            AV_CEC_SET_HDR_BC(TxContent, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_ACTIVE_SRC);
            AV_CEC_SET_PHYS_ADDR1(TxContent, port->content.tx->PhyAddr);
            AvKapiOutputDebugMessage("CEC: Sending active source=%04x", port->content.tx->PhyAddr);
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendInactiveSource:
            /* 0x82, send active source, step 2 of 2.1 one touch play */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 4;
            AV_CEC_SET_HDR_BC(TxContent, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_INACTIVE_SOURCE);
            AV_CEC_SET_PHYS_ADDR1(TxContent, port->content.tx->PhyAddr);
            AvKapiOutputDebugMessage("CEC: Sending Inactive source=%04x", port->content.tx->PhyAddr);
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendFeatureAbort:
            /* 0x00, default response */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 4;
            port->content.cec->TxMsg = AV_CEC_MSG_FEATURE_ABORT;
            /* reverse logical addr */
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), AV_CEC_DST(RxContent));
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_FEATURE_ABORT);
            TxContent[2] = AV_CEC_OPCODE(RxContent);
            TxContent[3] = AV_CEC_ABORT_REASON_REFUSED;
            AvKapiOutputDebugMessage("CEC: Sending feature abort");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendPowerStatus:
            /* 0x90 ,14.2 report power status, response of give power status */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 3;
            port->content.cec->TxMsg = AV_CEC_MSG_REPORT_PWR_STATUS;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), AV_CEC_DST(RxContent));
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_REPORT_PWR_STATUS);
            TxContent[2] = DevicePowerStatus;
            AvKapiOutputDebugMessage("CEC: Sending report power status");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendSetOsdName:
            /* 0x47, 10.2 response for give osd name */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 16;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), AV_CEC_DST(RxContent)); /* Directly addressed */
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_SET_OSD_NAME);
            for (Len=0; (Len<14) && DeviceName[Len]; Len++)
            {
                TxContent[Len+2] = DeviceName[Len];
            }
            AvKapiOutputDebugMessage("CEC: Sending Set OSD name");
            AvKapiCecSendMessage(port);
            break;
        case AvEventCecSendDeviceVendorID:
            /* 0x87, 9.2 vendor id broadcast */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 5;
            AV_CEC_SET_HDR_BC(TxContent, port->content.cec->LogAddr); /* Broadcast */
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_DEVICE_VENDOR_ID);
            TxContent[2] = AvCecGsvVendorIdByte1;
            TxContent[3] = AvCecGsvVendorIdByte2;
            TxContent[4] = AvCecGsvVendorIdByte3;
            AvKapiOutputDebugMessage("CEC: Sending Vendor ID");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendCecVersion:
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 3;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), AV_CEC_DST(RxContent));
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_CEC_VERSION);
            TxContent[2] = 0x05; /* only support CEC 1.4 */
            AvKapiOutputDebugMessage("CEC: Sending CEC Version 1.4");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendFeatureAbortReason:
            /* Abort Reason = wparam */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 4;
            port->content.cec->TxMsg = AV_CEC_MSG_FEATURE_ABORT;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), AV_CEC_DST(RxContent));
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_FEATURE_ABORT);
            TxContent[2] = AV_CEC_OPCODE(RxContent);
            TxContent[3] = (uint8)*wparam;
            AvKapiOutputDebugMessage("CEC: Sending feature abort. Abort Reason=%02x", *wparam);
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendReqActiveDevice:
            /* TV sends request active source */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen=2;
            AV_CEC_SET_HDR_BC(TxContent, port->content.tx->PhyAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_REQ_ACTIVE_SRC);
            AvKapiOutputDebugMessage("CEC MSG: Sending active source request");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendSetSystemAudioModeToTv:
            /* Audio 1.2: response to system audio mode request */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen=3;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_TV_LOG_ADDRESS, AvCecLogicAddress);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE);
            TxContent[2] = AudioStatus;
            AvKapiCecSendMessage(port);
            Cec_Tx_Audio_Status.AudioMode = AudioStatus;
            break;

        case AvEventCecSendSetSystemAudioModeToAll:
            /* Audio 1.2: response to system audio mode request */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen=3;
            AV_CEC_SET_HDR_BC(TxContent, AvCecLogicAddress);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE);
            TxContent[2] = AudioStatus;
            AvKapiCecSendMessage(port);
            Cec_Tx_Audio_Status.AudioMode = AudioStatus;
            break;

        case AvEventCecSendReportAudioStatus:
            /* response of AvEventCecMsgGiveAudioStatus */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 3;
            /* Directly addressed */
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_REPORT_AUDIO_STATUS);
            /* AV_CEC_AUDIO_MUTE_SHIFTER = bit 7 */
            TxContent[2] = Cec_Tx_Audio_Status.Mute << 7;
            /* CEC_AUDIO_MASK_MUTE_BIT = 0x7f */
            TxContent[2] = TxContent[2] | (Cec_Tx_Audio_Status.Volume & 0x7f);
            AvKapiOutputDebugMessage("CEC: Sending audio status");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendReportSystemAudioModeStatus:
            /* response of AvEventCecMsgGiveSystemAudioModeStatus */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 3;
            /* Directly addressed */
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_SYSTEM_AUDIO_MODE_STATUS);
            TxContent[2] = Cec_Tx_Audio_Status.AudioMode;        /* audio mode status      */
            AvKapiOutputDebugMessage("CEC: Sending audio mode status");
            AvKapiCecSendMessage(port);
            break;

        case AvEventCecSendReportShortAudioDecriptor:
            /* response of AvEventCecMsgRequestShortAudioDescriptor */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 5;

            /* Directly addressed */
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_REPORT_SHORT_AUDIO_DESCRIPTOR);

            TxContent[2] = (Cec_Tx_Audio_Status.MaxNumberOfChannels-1) | ( (Cec_Tx_Audio_Status.AudioFormatCode) << 3);
            TxContent[3] = Cec_Tx_Audio_Status.AudioSampleRate;          /* audio  capability 2      */
            TxContent[4] = Cec_Tx_Audio_Status.AudioBitLen;
            AvKapiOutputDebugMessage("CEC: Sending audio capability");
            AvKapiCecSendMessage(port);
            break;

#endif

#if AvCecDataReadingInEvent

        case AvEventCecReceiveSystemAudioModeStatus:
            /* reg setting from AvEventCecMsgSystemAudioModeStatus */
            Cec_Tx_Audio_Status.AudioMode = RxContent[2];
            break;

        case AvEventCecReceiveSetSystemAudioMode:
            /* reg setting from AvEventCecMsgSetSystemAudioMode */
            Cec_Tx_Audio_Status.Mute = AV_CEC_AUDIO_MUTE_ON;
            if (RxContent[2])
            {
                Cec_Tx_Audio_Status.Mute = AV_CEC_AUDIO_MUTE_OFF;
            }
            Cec_Tx_Audio_Status.AudioMode = RxContent[2];
            break;

        case AvEventCecReceiveAudioStatus:
            /* reg setting from AvEventCecMsgReportAudioStatus */
            /* AV_CEC_AUDIO_MUTE_SHIFTER = bit 7 */
            Cec_Tx_Audio_Status.Mute = RxContent[2] >> 7;
            /* CEC_AUDIO_MASK_MUTE_BIT = 0x7f */
            Cec_Tx_Audio_Status.Volume = RxContent[2] & 0x7f;
            break;

        case AvEventCecReceiveSetAudioRate:
            /* reg setting from AvEventCecMsgSetAudioRate */
            Cec_Tx_Audio_Status.AudioRate = RxContent[2];
            break;

        case AvEventCecReceiveShortAudioDescriptor:
            /* reg setting from AvEventCecMsgReportShortAudioDescriptor */
            /* AV_CEC_AUDIO_FORMAT_ID_SHIFTER = 3 */
            Cec_Tx_Audio_Status.AudioFormatCode = RxContent[2] >> 3;
            /* CEC_AUDIO_MASK_AUDIOFORMATID_BIT = 0x07 */
            Cec_Tx_Audio_Status.MaxNumberOfChannels = RxContent[2] & 0x07;
            (Cec_Tx_Audio_Status.MaxNumberOfChannels)++;
            /* CEC_AUDIO_ENABLE_AUDIOSAMPLERATE_BIT = 0x7f */
            Cec_Tx_Audio_Status.AudioSampleRate = RxContent[3] & 0x7f;
            if ( Cec_Tx_Audio_Status.AudioFormatCode == AV_AUD_FORMAT_LINEAR_PCM)
            {
                Cec_Tx_Audio_Status.AudioBitLen  = RxContent[4];
            }
            else if ( (Cec_Tx_Audio_Status.AudioFormatCode >= AV_AUD_FORMAT_AC3)
                     && (Cec_Tx_Audio_Status.AudioFormatCode <= AV_AUD_FORMAT_AC3))
            {
                Cec_Tx_Audio_Status.MaxBitRate  = ((uint16)RxContent[4]) << 3;
            }
            break;

        case AvEventCecSendActiveSourceToAudio:
            /* reg setting from AvEventCecMsgReportPhyAddr */
            Cec_Tx_Audio_Status.ActiveSource = *wparam;
            break;

        case AvEventCecSendInitiateARC:
            /* called from AvEventCecArcManage */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen=2;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_TV_LOG_ADDRESS, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_INITIATE_ARC);
            AvKapiOutputDebugMessage("CEC: Sending Initiate ARC");
            AvKapiCecSendMessage(port);
            break;
        case AvEventCecSendTerminateARC:
            /* called from AvEventCecArcManage */
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 2;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_TV_LOG_ADDRESS, port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_TERMINATE_ARC);
            AvKapiOutputDebugMessage("CEC: Sending Terminate ARC");
            AvKapiCecSendMessage(port);
            break;
        case AvEventCecSendMenuStatus:
            port->content.cec->TxSendFlag = AV_CEC_TX_TO_SEND;
            port->content.cec->TxLen = 3;
            AV_CEC_SET_HDR_DA(TxContent, AV_CEC_SRC(RxContent), port->content.cec->LogAddr);
            AV_CEC_SET_OPCODE(TxContent, AV_CEC_MSG_MENU_STATUS);
            if(RxContent[2] == 0x00) /* query = deactivate */
                TxContent[2] = 0x00;
            else
                TxContent[2] = 0x01;
            AvKapiOutputDebugMessage("CEC: Sending Menu Status");
            AvKapiCecSendMessage(port);
            break;

#endif

#if AvCecMessageEvent
        case AvEventCecMsgRouteChange:
            /* from RxMsg AV_CEC_MSG_ROUTE_CHANGE */
            if ((AV_CEC_BROADCAST(RxContent)) &&
                (AV_CEC_PHYS_ADDR2(RxContent) == port->content.tx->PhyAddr))
                /* New address is me */
            {
                Spa[0] = port->content.tx->PhyAddr>>8;
                Spa[1] = port->content.tx->PhyAddr & 0xff;
                AvHandleEvent(port, AvEventCecSendRoutingInformation, Spa, NULL);
                AvKapiOutputDebugMessage("CEC: Routing change. Old PA=%04x  New PA=%04x",
                                         AV_CEC_PHYS_ADDR1(RxContent),
                                         AV_CEC_PHYS_ADDR2(RxContent));
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgRouteInfo:
            /* from RxMsg AV_CEC_MSG_ROUTE_INFO */
            if (AV_CEC_BROADCAST(RxContent))
            {
                Spa[0] = port->content.tx->PhyAddr>>8;
                Spa[1] = port->content.tx->PhyAddr & 0xff;
                AvHandleEvent(port, AvEventCecSendRoutingInformation, Spa, NULL);
                AvKapiOutputDebugMessage("CEC: Routing Info. Active Route=%04x",
                                         AV_CEC_PHYS_ADDR1(RxContent));
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgActiveSource:
            /* from RxMsg AV_CEC_MSG_ACTIVE_SRC */
            /* when getting ACTIVE_SRC, power up at first */
            if (AV_CEC_BROADCAST(RxContent))
            {
                Recv_Phy_Addr  = AV_CEC_PHYS_ADDR1(RxContent);
                InpIndex = AvCecFindRequiredInput (port, Recv_Phy_Addr);
                /* Low 4 bits = Input Total, High 4 bits = Current Active Input */
                /* InpIndex is within InputCount Range, and not invalid value 0 */
                if((InpIndex <= ((port->content.cec->InputCount)&0xf)) &&
                   (InpIndex != 0))
                {
                    AvKapiOutputDebugMessage("************Active Source Request From %04x (Port %d)",
                                             Recv_Phy_Addr, InpIndex);
#if AvCecLinkageMode
                    /* Active Input needs to be switched */
                    if(InpIndex != (port->content.cec->InputCount>>4))
                    {
                        AvKapiOutputDebugMessage("CEC Source try to connect Rx%d - Tx%d", InpIndex, port->index-3);
                        /* Find Device's first port (Rx Port) */
                        TempPort = (AvPort*)port->device->port;
                        port->content.cec->InputCount = (InpIndex<<4) | (port->content.cec->InputCount & 0xf);
                        AvApiConnectPort(&TempPort[InpIndex-1], &TempPort[2], AvConnectAV);
                    }
                    /* Current Active Input is already streaming  */
                    else
                    {
                        AvKapiOutputDebugMessage("Current Input %d is already streaming", InpIndex);
                    }
#endif
                }
                else
                {
                    AvKapiOutputDebugMessage("CEC: ignore");
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgGivePhyAddr:
            /* from RxMsg AV_CEC_MSG_GIVE_PHYS_ADDR */
            /* only report phyaddr of self */
            if (AV_CEC_DST(RxContent) == port->content.cec->LogAddr)
            {
                AvKapiOutputDebugMessage("CEC: Give physical address");
                AvHandleEvent(port, AvEventCecSendReportPhysAddress, 0, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgSetStreamPath:
            /* from RxMsg AV_CEC_MSG_SET_STRM_PATH */
            if (AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("************Set Stream Path Request to Source=%04x", AV_CEC_PHYS_ADDR1(RxContent));
                if (AV_CEC_PHYS_ADDR1(RxContent)==port->content.tx->PhyAddr)
                {
                    AvHandleEvent(port, AvEventCecSendActiveSource, 0, NULL);
                }
                else
                {
#if AvCecLinkageMode
                    /* Switch to appropriate input */
                    InpIndex = AvCecFindRequiredInput (port, AV_CEC_PHYS_ADDR1(RxContent));
                    if (InpIndex <= port->content.cec->InputCount)
                    {
                        AvHandleEvent(port, AvEventCecSendActiveSource, 0, NULL);
                        /* Set Stream requires a new Source */
                        if(InpIndex != (port->content.cec->InputCount>>4))
                        {
                            /* Find Device's first port (Rx Port) */
                            AvKapiOutputDebugMessage("CEC TV try to connect Rx%d - Tx%d", InpIndex, port->index-3);
                            TempPort = (AvPort*)port->device->port;
                            port->content.cec->InputCount = (InpIndex<<4) | (port->content.cec->InputCount & 0xf);
                            AvApiConnectPort(&TempPort[InpIndex-1], &TempPort[2], AvConnectAV);
                        }
                    }
#endif
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgStandby:
            /* from RxMsg AV_CEC_MSG_STANDBY */
            port->content.cec->RxMsg = AV_CEC_MSG_STANDBY;
            port->content.cec->RxLen = 0;
            ret = AvHandleEvent(port, AvEventCecRxMessageRespond,
                                    Msg_Len,
                                    NULL);
            if ( ret == AvOk )
            {
                AvKapiOutputDebugMessage("CEC: To be in Standby mode");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: is already in Standby mode, ignore");
            }
            break;

        case AvEventCecMsgAbort:
            /* from RxMsg AV_CEC_MSG_ABORT */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Abort message");
                AvHandleEvent(port, AvEventCecSendFeatureAbort, 0, NULL);
            }
            else
            {
                 AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgFeatureAbort:
            /* from RxMsg AV_CEC_MSG_FEATURE_ABORT */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Feature abort message");
                /*
                if ( RxContent[2] == 0x72 )
                {
                    port->content.cec->SendAudioCecFlag = 0;
                }
                */
            }
            else
            {
                 AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgGivePowerStatus:
            /* from RxMsg AV_CEC_MSG_GIVE_PWR_STATUS */
            /* when getting GIVE_PWR_STATUS, first to power up, then to REPORT_PWR_STATUS */
            AvHandleEvent(port, AvEventCecRxMessageRespond,Msg_Len, NULL);
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Give power status message");
                AvHandleEvent(port, AvEventCecSendPowerStatus, 0, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgReportPowerStatus:
            /* from RxMsg AV_CEC_MSG_REPORT_PWR_STATUS */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: get reporting power status message");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgGiveOsdName:
            /* from RxMsg AV_CEC_MSG_GIVE_OSD_NAME */
            if ((!AV_CEC_BROADCAST(RxContent)) &&
                (AV_CEC_SRC(RxContent) != AV_CEC_BROADCAST_LOG_ADDRESS ))
            {
                AvKapiOutputDebugMessage("CEC: Give OSD name message");
                AvHandleEvent(port, AvEventCecSendSetOsdName, 0, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgGetVendorId:
            /* from RxMsg AV_CEC_MSG_GET_VENDOR_ID */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Get Vendor ID");
                AvHandleEvent(port, AvEventCecSendDeviceVendorID, 0, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

       case AvEventCecMsgGetCecVersion:
           /* from RxMsg AV_CEC_MSG_GET_CEC_VERSION */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Get Cec Version");
                AvHandleEvent(port, AvEventCecSendCecVersion, 0, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgRequestArcInitiation:
            /* from RxMsg AV_CEC_MSG_REQUEST_ARC_INITIATION */
            i = 0;
            if((port->content.tx->PhyAddr >> 8)&0x000F)i++;
            if((port->content.tx->PhyAddr >> 4)&0x000F)i++;
            if(port->content.tx->PhyAddr & 0x000F)i++;

            /* ARC is directly from TV */
            if ( (AV_CEC_SRC(RxContent) == AV_CEC_TV_LOG_ADDRESS) &&
                 (!AV_CEC_BROADCAST(RxContent)) && ( i == 0))
            {

                AvKapiOutputDebugMessage("CEC: Request ARC Initiation message");
                ret = AvHandleEvent(port, AvEventCecSendInitiateARC, NULL, NULL);
                port->content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;

                if( ret != AvOk )
                {
                    AvHandleEvent(port, AvEventCecSendFeatureAbortReason, 0, NULL);
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgReportArcInitiated:
            /* from RxMsg AV_CEC_MSG_REPORT_ARC_INITIATED */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Report ARC Initiated message");
                port->content.cec->EnableARC = AV_CEC_ARC_INITIATED;
                if(AudioStatus == 0)
                {
                    port->content.cec->EnableARC = AV_CEC_ARC_TO_TERMINATE;
                    port->content.cec->ARCTryCount = 0;
                }
                else
                    AvHandleEvent(port, AvEventCecSendSetSystemAudioModeToTv, NULL, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgRequestArcTermination:
            /* from RxMsg AV_CEC_MSG_REQUEST_ARC_TERMINATION */
            if ( (AV_CEC_SRC(RxContent) == AV_CEC_TV_LOG_ADDRESS) &&
                 (!AV_CEC_BROADCAST(RxContent)) )
            {
                AvKapiOutputDebugMessage("CEC: Request ARC Termination message");
                ret = AvHandleEvent(port, AvEventCecSendTerminateARC, NULL, NULL);
                port->content.cec->EnableARC = AV_CEC_ARC_TO_TERMINATE;
                if( ret != AvOk )
                {
                    AvHandleEvent(port, AvEventCecSendFeatureAbortReason, 0, NULL);
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgReportArcTerminated:
            /* from RxMsg AV_CEC_MSG_REPORT_ARC_TERMINATED */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Report ARC Terminated message");
                if(AudioStatus == 1)
                {
                    port->content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                    port->content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                    port->content.cec->ARCTryCount = 0;
                }
                else
                    AvHandleEvent(port, AvEventCecSendSetSystemAudioModeToTv, NULL, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgUserControlPressed:
            /* from RxMsg AV_CEC_MSG_USER_CONTROL_PRESSED */
            /* when getting USER_CONTROL_PRESSED, UI Command is POWER ON, then to power up */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: User Control Pressed with UI Command = 0x%x", RxContent[2]);
                switch(RxContent[2])
                {
                    case 0x41: /* Volume Up */
                        if(Cec_Tx_Audio_Status.Volume <= 100)
                            Cec_Tx_Audio_Status.Volume = Cec_Tx_Audio_Status.Volume + 10;
                        AvHandleEvent(port, AvEventCecSendReportAudioStatus, 0, NULL);
                        break;
                    case 0x42: /* Volume Down */
                        if(Cec_Tx_Audio_Status.Volume >= 10)
                            Cec_Tx_Audio_Status.Volume = Cec_Tx_Audio_Status.Volume - 10;
                        AvHandleEvent(port, AvEventCecSendReportAudioStatus, 0, NULL);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgUserControlReleased:
            /* from RxMsg AV_CEC_MSG_USER_CONTROL_RELEASED */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: User Control Released with UI Command = 0x%x", RxContent[2]);
                switch(RxContent[2])
                {
                    case 0x41: /* Volume Up */
                        AvHandleEvent(port, AvEventCecSendReportAudioStatus, 0, NULL);
                        break;
                    case 0x42: /* Volume Down */
                        AvHandleEvent(port, AvEventCecSendReportAudioStatus, 0, NULL);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgImageViewOn:
        case AvEventCecMsgTextViewOn:
            /* when getting IMAGE_VIEW_ON or TEXT_VIEW_ON to power up */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Image/Text View On");
                AvHandleEvent(port, AvEventCecSendReqActiveDevice, 0, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgSystemAudioModeRequest:
            if (!AV_CEC_BROADCAST(RxContent))
            {
                if ( port->content.cec->RxLen == 2 )
                {
                    AvHandleEvent(port, AvEventCecSendSetSystemAudioModeToTv, NULL, NULL);
                }
                else if ( port->content.cec->RxLen == 4)
                {
                    InpIndex = AvCecFindRequiredInput (port, AV_CEC_PHYS_ADDR1(RxContent));
                    AvKapiOutputDebugMessage("CEC: Active Source is %04x (Port %d)",
                                             AV_CEC_PHYS_ADDR1(RxContent), InpIndex);
                    AvHandleEvent(port, AvEventCecSendSetSystemAudioModeToAll, NULL, NULL);
                }
                else
                {
                    AvKapiOutputDebugMessage("CEC: parameters number is wrong");
                    AvKapiOutputDebugMessage("CEC: ignore");
                }
                /* Audio ARC Protection */
                if(AudioStatus == 0)
                    port->content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
                else
                {
                    port->content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                    port->content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                }
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");

            }
            break;

        case AvEventCecMsgDeckControl:
            /* from RxMsg AV_CEC_MSG_DECK_CONTROL */
            /*================================================
             * when getting DECK_CONTROL
             * to power up
             * no action to [Deck Control Mode], and
             * not to response by <Deck Status> [Deck Info]
             *================================================*/
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: DECK CONTROL message");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgPlay:
            /* from RxMsg AV_CEC_MSG_PLAY */
            /*===============================================
             * when getting PLAY
             * to power up
             * no action to [Play Mode], and
             * not to response by <Deck Status> [Deck Info]
             *===============================================*/
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: PLAY message");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");

            }
            break;

        case AvEventCecMsgGiveAudioStatus:
            /* from RxMsg AV_CEC_MSG_GIVE_AUDIO_STATUS */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: give audio status");
                AvHandleEvent(port, AvEventCecSendReportAudioStatus, 0, NULL);
                /* SendReportAudioStatus(RxContent); */
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgGiveSystemAudioModeStatus:
            /* from RxMsg AV_CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: give system audio mode status");
                AvHandleEvent(port, AvEventCecSendReportSystemAudioModeStatus, 0, NULL);
                /* SendReportSystemAudioModeStatus(RxContent); */
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgRequestShortAudioDescriptor:
            /* from RxMsg AV_CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: give short audio descriptor");
                AvHandleEvent(port, AvEventCecSendReportShortAudioDecriptor, 0, NULL);
                /* SendReportShortAudioDecriptor(RxContent); */
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgSystemAudioModeStatus:
            /* from RxMsg AV_CEC_MSG_SYSTEM_AUDIO_MODE_STATUS */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvHandleEvent(port, AvEventCecReceiveSystemAudioModeStatus, 0, NULL);
                /* ReceiveSystemAudioModeStatus(RxContent); */
                AvKapiOutputDebugMessage("CEC: receiving message about system audio mode status");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgSetSystemAudioMode:
            /* from RxMsg AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                ret = AvHandleEvent(port, AvEventCecReceiveSetSystemAudioMode, 0, NULL);
                /* ret = ReceiveSetSystemAudioMode(RxContent); */
                /*
                if ( ret == AvOk )
                {
                    port->content.cec->RxMsg = AV_CEC_MSG_SET_SYSTEM_AUDIO_MODE;
                    port->content.cec->RxLen = 0;
                    *Msg_Len = *Msg_Len +  port->content.cec->RxLen;
                    ret = AvHandleEvent(port, AvEventCecRxMessageRespond,
                                               Msg_Len,
                                               NULL);
                }
                */
                AvKapiOutputDebugMessage("CEC receiving message about setting system audio mode");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgReportAudioStatus:
            /* from RxMsg AV_CEC_MSG_REPORT_AUDIO_STATUS */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvHandleEvent(port, AvEventCecReceiveAudioStatus, 0, NULL);
                /* ReceiveAudioStatus(RxContent); */
                AvKapiOutputDebugMessage("CEC: receiving message about audio status");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgSetAudioRate:
            /* from RxMsg AV_CEC_MSG_SET_AUDIO_RATE */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvHandleEvent(port, AvEventCecReceiveSetAudioRate, 0, NULL);
                /* ReceiveSetAudioRate(RxContent); */
                AvKapiOutputDebugMessage("CEC: receiving message about audio rate");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgReportShortAudioDescriptor:
            /* from RxMsg AV_CEC_MSG_REPORT_SHORT_AUDIO_DESCRIPTOR */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: receiving message about short audio descriptor");
                AvHandleEvent(port, AvEventCecReceiveShortAudioDescriptor, 0, NULL);
                /* ReceiveShortAudioDescriptor(RxContent); */
             }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgReportPhyAddr:
            /* from RxMsg AV_CEC_MSG_REPORT_PHYS_ADDR */
            if (AV_CEC_BROADCAST(RxContent))
            {
                ActiveSource = (((uint16)(RxContent[2])<<8)+
                                (uint16)(RxContent[3]));
                /*
                param = (uint32)ActiveSource;
                AvHandleEvent(port, AvEventCecSendActiveSourceToAudio, &param, NULL);
                */
                /* SendActiveSourceToAudio(ActiveSource); */
                AvKapiOutputDebugMessage("CEC: source %x reports phy addr",ActiveSource);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgMenuRequest:
            /* from RxMsg AV_CEC_MSG_MENU_REQUEST */
            if (!AV_CEC_BROADCAST(RxContent))
            {
                AvKapiOutputDebugMessage("CEC: Receive menu request %x", RxContent[2]);
                AvHandleEvent(port, AvEventCecSendMenuStatus, NULL, NULL);
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgRequestActiveSource:
            /* from RxMsg AV_CEC_MSG_REQ_ACTIVE_SRC */
            if((AV_CEC_BROADCAST(RxContent)) &&
               (port->content.cec->LogAddr != AV_CEC_TV_LOG_ADDRESS))
            {
                AvHandleEvent(port, AvEventCecSendActiveSource, NULL, NULL);
            }
            else
            {
                 AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgSetMenuLanguage:
            /* from RxMsg AV_CEC_MSG_SET_MENU_LANGUAGE */
            if(AV_CEC_BROADCAST(RxContent)&&
               (port->content.cec->LogAddr == AV_CEC_TV_LOG_ADDRESS))
            {
                AvKapiOutputDebugMessage("CEC: Set menu language");
            }
            else
            {
                AvKapiOutputDebugMessage("CEC: ignore");
            }
            break;

        case AvEventCecMsgDeviceVendorId:
            AvKapiOutputDebugMessage("CEC: ignore");
            break;

#endif

        default:
            ret = AvNotAvailable;
            break;
    }
    UNUSED(wparam);
    UNUSED(pparam);

    return ret;
}

#if AvEnableCecFeature /* CEC Related */
uint16 AvCecFindRequiredInput (AvPort *port, uint16 SrcPhys)
{
    /* PhyAddr order is fixed */
    uint16 i;
    uint8  shift = 12;
    uint16 Mask=0xf000, Mask2=0;

    for (i=0; i<4; i++)
    {
        if ((port->content.tx->PhyAddr & Mask) == 0)
        {
            break;
        }
        Mask2 |= Mask;
        Mask >>= 4;
        if(shift == 0)
            return (0xff);
        else
            shift = shift - 4;
    }

    if ((port->content.tx->PhyAddr & Mask2) == (SrcPhys & Mask2))
    {
        i = (SrcPhys>>shift) & 0x0f;
        return(i);
    }
    return (0xff);
}
#endif /* CEC Related */

AvRet AvEdidPortManage(AvPort *RxPort)
{
    uint8 SpaLocation = 0;
    uint8 SpaValue[2];
    uint8 SinkNumber = 0; /* default to force mode */
    uint8 i = 0;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    #if AvMatrix4x4Mode
    /* Quit if not Front End Gsv2k6 */
    if(RxPort->device->type != Gsv2k6)
        return AvOk;
    #endif

    /* 1. Clear RAM header, save time */
    AvMemset(InEdid, 0, 256);
    AvMemset(SinkEdid, 0, 256);
    AvMemset(OutEdid, 0, 256);
    /* 1.1 Set Edid Parameter */
    switch(EdidHdmiMode)
    {
        case AvEdidModeAuto:
            AvEdidSupportFreq = AvEdidBitFreq6G    |
                                AvEdidBitFreq4P5G  |
                                AvEdidBitFreq3P75G |
                                AvEdidBitFreq3G    |
                                AvEdidBitFreq2P25G |
                                AvEdidBitFreq1P5G  |
                                AvEdidBitFreq750M  |
                                AvEdidBitFreq270M  |
                                AvEdidBitFreq135M;
            AvEdidCeaParamForce  =  0;
            AvEdidCeaParamRemove =  0;
            break;
        case AvEdidModeStd:
             AvEdidSupportFreq = AvEdidBitFreq6G    |
                                AvEdidBitFreq4P5G  |
                                AvEdidBitFreq3P75G |
                                AvEdidBitFreq3G    |
                                AvEdidBitFreq2P25G |
                                AvEdidBitFreq1P5G  |
                                AvEdidBitFreq750M  |
                                AvEdidBitFreq270M  |
                                AvEdidBitFreq135M;
            AvEdidCeaParamForce  =  AvEdidBitCeaSVD | AvEdidBitCeaVSDBHF;
            AvEdidCeaParamRemove =  0;
            break;
        case AvEdidModeCopy:
            CurrentPort = (AvPort*)(RxPort->device->port);
            for(i=0;i<6;i++)
            {
                if((CurrentPort[i].type == HdmiTx) &&
                   (CurrentPort[i].content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
                   (CurrentPort[i].content.tx->IgnoreEdidError == 0))
                {
                    KfunTxReadEdid(&CurrentPort[i],OutEdid);
                    SpaLocation = 0x01;
                    SpaValue[0] = 0xff;
                    SpaValue[1] = 0xff;
                    KfunRxWriteEdid(RxPort, OutEdid, SpaLocation, SpaValue);
                    #if 0
                    AvKapiOutputDebugMessage("Rx %d Generated Edid Start: ",RxPort->index+1);
                    for(i=0;i<16;i++)
                    {
                        AvKapiOutputDebugMessage("0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,"\
                                                 "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                                 OutEdid[i*16+0],OutEdid[i*16+1],OutEdid[i*16+2],OutEdid[i*16+3],
                                                 OutEdid[i*16+4],OutEdid[i*16+5],OutEdid[i*16+6],OutEdid[i*16+7],
                                                 OutEdid[i*16+8],OutEdid[i*16+9],OutEdid[i*16+10],OutEdid[i*16+11],
                                                 OutEdid[i*16+12],OutEdid[i*16+13],OutEdid[i*16+14],OutEdid[i*16+15]);
                    }
                    #endif
                    RxPort->content.rx->EdidStatus = AV_EDID_UPDATED;
                    return AvOk;
                }
            }
            break;
        case AvEdidModeUserDefine:
            {
                if(pvpstFuncGsvUserEdidFunction != NULL)
                {
                    pvpstFuncGsvUserEdidFunction(RxPort);
                    return AvOk;
                }
                else if(g_pu8GsvSysEdid != NULL)
                {
                    SpaLocation = 0x01;
                    SpaValue[0] = 0xff;
                    SpaValue[1] = 0xff;
                    KfunRxWriteEdid(RxPort, g_pu8GsvSysEdid, SpaLocation, SpaValue);
                    RxPort->content.rx->EdidStatus = AV_EDID_UPDATED;
                    return AvOk;
                }
                else if(System_Default_Edid_table != NULL)
                {
                    SpaLocation = 0x01;
                    SpaValue[0] = 0xff;
                    SpaValue[1] = 0xff;
                    KfunRxWriteEdid(RxPort, System_Default_Edid_table, SpaLocation, SpaValue);
                    RxPort->content.rx->EdidStatus = AV_EDID_UPDATED;
                    return AvOk;
                }
                else
                {
                    AvEdidSupportFreq = AvEdidBitFreq6G    |
                                        AvEdidBitFreq4P5G  |
                                        AvEdidBitFreq3P75G |
                                        AvEdidBitFreq3G    |
                                        AvEdidBitFreq2P25G |
                                        AvEdidBitFreq1P5G  |
                                        AvEdidBitFreq750M  |
                                        AvEdidBitFreq270M  |
                                        AvEdidBitFreq135M;
                    AvEdidCeaParamForce  =  AvEdidBitCeaSVD | AvEdidBitCeaVSDBHDMI | AvEdidBitCeaVSDBHF;
                    AvEdidCeaParamRemove =  0;
                }
            }
            break;
        default:
            AvEdidSupportFreq = AvEdidBitFreq3G    |
                                AvEdidBitFreq2P25G |
                                AvEdidBitFreq1P5G  |
                                AvEdidBitFreq750M  |
                                AvEdidBitFreq270M  |
                                AvEdidBitFreq135M;
            AvEdidCeaParamForce  =  0;
            AvEdidCeaParamRemove =  AvEdidBitCeaVSDBHF;
            break;
    }

#if GSV5K1
    if(RxPort->device->type == Gsv5k1)
    {
        AvPort* TxAPort;
        #if AvEnableCcaFeature
        AvPort* TxBPort;
        #endif
        /* 2. Look up front port */
        TxAPort = &RxPort[1];
        #if AvEnableCcaFeature
        TxBPort = &RxPort[2];
        #endif
        /* 2.1 Read TxA Port if connected */
        if((TxAPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
           (TxAPort->content.tx->IgnoreEdidError == 0))
            KfunTxReadEdid(TxAPort,InEdid);
        #if AvEnableCcaFeature
        if((TxBPort->type == CcaTx) &&
           (CcaEdidReceivedFlag[TxBPort->device->index] == 1) &&
           (TxBPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
        {
            if((TxAPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
               (TxAPort->content.tx->IgnoreEdidError == 0))
                KfunTxReadEdid(TxBPort,SinkEdid);
            else
                KfunTxReadEdid(TxBPort,InEdid);
        }
        #elif 0//AvEnableExternalUart
        if(sParser == S_EDID_STABLE)
        {
            if((TxAPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
               (TxAPort->content.tx->IgnoreEdidError == 0))
                AvMemcpy(SinkEdid,UartSinkEdid,256);
            else
                AvMemcpy(InEdid,UartSinkEdid,256);
        }
        #endif
    }
    else
#endif
    {
        /* 2. Look up front port */
        CurrentPort = NULL;
        PrevPort = NULL;
        SinkNumber = 0;
        /* loop to the end of the same level connections */
        /* Merge every 2 port Edids a time */
        while(KfunFindVideoNextTxEnd(RxPort, &PrevPort, &CurrentPort) == AvOk)
        {
            if((CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
               (CurrentPort->content.tx->IgnoreEdidError == 0))
            {
                #if (GSV2K6&&(!AvMatrix4x4Mode))
                if(SinkNumber != 4)
                    SinkNumber = SinkNumber + 1;
                #else
                if(SinkNumber != 2)
                    SinkNumber = SinkNumber + 1;
                #endif
                AvKapiOutputDebugMessage("Rx %d Edid Manage: Read Tx %d EDID",RxPort->index+1, CurrentPort->index-3);
                /* find the next Edid Ram Ptr */
                if(SinkNumber == 1)
                    KfunTxReadEdid(CurrentPort,InEdid);
                else
                {
                    KfunTxReadEdid(CurrentPort,SinkEdid);
                    /* Merge Edids */
                    AvEdidFuncStructInit(&DevEdidReg);
                    AvMemset(OutEdid, 0, 256);
                    AvEdidFuncVesaProcess(&DevEdidReg,InEdid,SinkEdid,OutEdid);
                    AvEdidFuncCeaProcess(&DevEdidReg,InEdid,SinkEdid,OutEdid);
                    AvMemcpy(InEdid,OutEdid,256);
                    AvMemset(SinkEdid, 0, 256);
                }
            }
            PrevPort = CurrentPort;
        }

        /* 3. Final Merge Process */
        if(SinkNumber <= 1)
        {
            AvEdidFuncStructInit(&DevEdidReg);
            //AvMemset(OutEdid, 0, 256);
            AvEdidFuncVesaProcess(&DevEdidReg,InEdid,SinkEdid,OutEdid);
            AvEdidFuncCeaProcess(&DevEdidReg,InEdid,SinkEdid,OutEdid);
        }
    }
    AvEdidFuncCheckSum(OutEdid);

    /* 4. Find Spa Location and generate its own SPA for source */
    SpaLocation = KfunFindCecSPAFromEdid(OutEdid, SpaValue);
    KfunGenerateSourceSpa(RxPort, SpaValue, 0);

    #if 0
    AvKapiOutputDebugMessage("Rx %d Generated Edid Start: ",RxPort->index+1);
    for(i=0;i<16;i++)
    {
        AvKapiOutputDebugMessage("0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,"\
                                 "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                 OutEdid[i*16+0],OutEdid[i*16+1],OutEdid[i*16+2],OutEdid[i*16+3],
                                 OutEdid[i*16+4],OutEdid[i*16+5],OutEdid[i*16+6],OutEdid[i*16+7],
                                 OutEdid[i*16+8],OutEdid[i*16+9],OutEdid[i*16+10],OutEdid[i*16+11],
                                 OutEdid[i*16+12],OutEdid[i*16+13],OutEdid[i*16+14],OutEdid[i*16+15]);
    }
    #endif
    /* 5. write Edid, set rx->EdidStatus to AV_EDID_NEEDUPDATE */
    KfunRxWriteEdid(RxPort, OutEdid, SpaLocation, SpaValue);
    RxPort->content.rx->EdidStatus = AV_EDID_UPDATED;

    return AvOk;
}

AvRet AvEdidPortAnalysis(AvPort *port)
{
    uint8 i=0;
    KfunTxReadEdid(port,InEdid);
#if AvGsv2k11CopyTxEdid
    memcpy(g_u8DisplayEdid[port->device->index],InEdid,256);
#endif
    AvEdidFuncStructInit(&DevEdidReg);
    AvEdidFunFullAnalysis(&DevEdidReg,InEdid);

    AvKapiOutputDebugMessage("Tx %d Read Edid Start: ",port->index);
    for(i=0;i<16;i++)
    {
        AvKapiOutputDebugMessage("0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,"\
                                 "0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,",
                                 InEdid[i*16+0], InEdid[i*16+1], InEdid[i*16+2], InEdid[i*16+3],
                                 InEdid[i*16+4], InEdid[i*16+5], InEdid[i*16+6], InEdid[i*16+7],
                                 InEdid[i*16+8], InEdid[i*16+9], InEdid[i*16+10],InEdid[i*16+11],
                                 InEdid[i*16+12],InEdid[i*16+13],InEdid[i*16+14],InEdid[i*16+15]);
    }

    if(DevEdidReg.MaxTmdsClk == 0)
    {
        DevEdidReg.MaxTmdsClk  = DevEdidReg.VesaMaxClk;
        DevEdidReg.MaxCharRate = DevEdidReg.VesaMaxClk;
    }
    /* Feature Support */
    port->content.tx->EdidSupportFeature = AV_BIT_FEAT_1G5;
    if((DevEdidReg.VsdbHfCheck[3] == 1) || (DevEdidReg.MaxTmdsClk > 340) || (DevEdidReg.MaxCharRate > 340) || (DevEdidReg.VesaMaxClk > 340))
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_SCDC;
    if(DevEdidReg.VsdbHfCheck[8] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_RR;
    if(DevEdidReg.VsdbHfCheck[7] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_B340MSCR;
    if(DevEdidReg.VsdbHfCheck[1] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_36B420;
    if(DevEdidReg.VsdbHfCheck[2] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_30B420;
    if(DevEdidReg.MaxCharRate >= 600)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_6G;
    if(DevEdidReg.MaxCharRate >= 450)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_4G5;
    if(DevEdidReg.MaxCharRate >= 370)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_3G75;
    if(DevEdidReg.MaxTmdsClk >= 300)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_3G;
    if(DevEdidReg.MaxTmdsClk >= 225)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_2G25;
    if((DevEdidReg.HdrStCheck[0] != 0) ||
       (DevEdidReg.HdrStCheck[1] != 0) ||
       (DevEdidReg.HdrStCheck[2] != 0) ||
       (DevEdidReg.HdrStCheck[3] != 0))
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_HDR;
    if(DevEdidReg.Y420VdbCheck[0] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_4K50_420;
    if(DevEdidReg.Y420VdbCheck[1] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_4K60_420;
    if(DevEdidReg.Y420VdbCheck[2] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_4KS50_420;
    if(DevEdidReg.Y420VdbCheck[3] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_4KS60_420;
    if(DevEdidReg.CdbCheck[0] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YUV_601;
    if(DevEdidReg.CdbCheck[1] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YUV_709;
    if((DevEdidReg.CdbCheck[5] == 1) || (DevEdidReg.CdbCheck[6] == 1))
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YCC_2020;
    if(DevEdidReg.CdbCheck[7] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_RGB_2020;
    if(DevEdidReg.VcdbCheck[0] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YUV_FULL_RANGE;
    if(DevEdidReg.VcdbCheck[1] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_RGB_FULL_RANGE;
    if(DevEdidReg.VcdbCheck[4] == 1)    // add by wf8421 20230129
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YUV_422;
    if(DevEdidReg.VcdbCheck[5] == 1)    // add by wf8421 20230129
    {
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YUV_601;
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_YUV_709;
    }
    if(DevEdidReg.VsdbCheckList[11] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_10B_DEEP_COLOR;
    if(DevEdidReg.VsdbCheckList[10] == 1)
        port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_12B_DEEP_COLOR;
    for(i=1;i<14;i++)
    {
        if(DevEdidReg.EdidCeaAudioCheck[i] == 1)
        {
            port->content.tx->EdidSupportFeature = port->content.tx->EdidSupportFeature | AV_BIT_FEAT_COMPRESS_AUDIO;
            break;
        }
    }

    return AvOk;
}

#if AvGsv2k8VideoRgb_422_444To420
#define MessageLen      8
void AvUartCmdToSendInputStatus(uint8 u8dev,
                                        uint8 u8PortId,
                                        uint8 u8ColorData,
                                        uint8 u8ColorSpace,
                                        uint8 u8Vic)
{
    uint8 l_u8CheckSum = 0;
    pstAvInfoUartSendMessage l_pstAvInfo = &g_stTxAvInfo[u8dev];

    l_pstAvInfo->u8PortId     = u8PortId;
    l_pstAvInfo->u8ColorData  = u8ColorData;
    l_pstAvInfo->u8ColorSpace = u8ColorSpace;
    l_pstAvInfo->u8Vic        = u8Vic;

    l_u8CheckSum = (uint8)CRC8Calculate((uint8 *)l_pstAvInfo,sizeof(stAvInfoUartSendMessage)-1);
    if((l_u8CheckSum != l_pstAvInfo->u8CheckSum)||(g_bGsv2k8TxChange[u8dev][u8PortId]==TRUE))
    {
        uint8 l_u8Mssage[MessageLen];
        pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_2)//_CSC_AV_INFO_UART_PORT)
            {
                break;
            }
            pstUart ++;
        }

        if(pstUart->u8UartPort == _UART_PORT_NONE)
        {
            return;
        }
        memset(l_u8Mssage,0,MessageLen);

        l_pstAvInfo->u8CheckSum = l_u8CheckSum;
        g_bGsv2k8TxChange[u8dev][u8PortId] = FALSE;

        l_u8Mssage[0] = 'p';
        l_u8Mssage[1] = 't';
        l_u8Mssage[2] = 'n';
        l_u8Mssage[3] = u8PortId;
        l_u8Mssage[4] = u8ColorData;
        l_u8Mssage[5] = u8ColorSpace;
        l_u8Mssage[6] = u8Vic;
        l_u8Mssage[7] = l_u8CheckSum;
        //add send message

        USARTx_send_bytes(pstUart,(uint8_t*)l_u8Mssage,MessageLen);
        AvKapiOutputDebugMessage("!!!!!Send Dev[%d] Csc message!!!!!",u8dev);
        AvKapiOutputDebugMessage("!!!!!        u8PortId = %d !!!!!!!",u8PortId);
        AvKapiOutputDebugMessage("!!!!!     u8ColorData = %d !!!!!!!",u8ColorData);
        AvKapiOutputDebugMessage("!!!!!    u8ColorSpace = %d !!!!!!!",u8ColorSpace);
        AvKapiOutputDebugMessage("!!!!!           u8Vic = %d !!!!!!!",u8Vic);
    }
}

void AvUserCheckTxLinkSta(AvPort *RxPort, AvPort *TxPort)
{
    static bool PreTxHpdSta[AvGsv2k8DeviceNumber][2] = {{FALSE,FALSE}};
    bool TxHpdSta[AvGsv2k8DeviceNumber][2] = {{FALSE,FALSE}};
    if((TxPort->content.tx->Hpd == AV_HPD_HIGH)
        ||(TxPort->content.tx->Hpd == AV_HPD_FORCE_HIGH))
    {
        TxHpdSta[TxPort->device->index][TxPort->index-4] = TRUE;
    }
    else
    {
        TxHpdSta[TxPort->device->index][TxPort->index-4] = FALSE;
    }

    if(PreTxHpdSta[TxPort->device->index][TxPort->index-4]
        != TxHpdSta[TxPort->device->index][TxPort->index-4])
    {
        PreTxHpdSta[TxPort->device->index][TxPort->index-4] = TxHpdSta[TxPort->device->index][TxPort->index-4];

        if(TxHpdSta[TxPort->device->index][TxPort->index-4] == TRUE)
        {
            g_bGsv2k8TxChange[RxPort->device->index][RxPort->index] = TRUE;
        }
    }
}
#endif

RouteStat AvPortRoutingPolicy(AvPort *RxPort, AvPort *TxPort)
{
    RouteStat TxConnectStyle = ROUTE_NO_CONNECT;
    /* 1. RxPort - TxPort connection */
    if((RxPort->type == HdmiRx) && (TxPort->type == HdmiTx))
    {
        if((TxPort->content.tx->EdidReadSuccess != AV_EDID_UPDATED) ||
           (TxPort->content.tx->Hpd != AV_HPD_HIGH))
            return ROUTE_NO_CONNECT;

        #if ((AvGsv2kxVideoCscForValensTx)&&(!AvGsv2kxVideoDSCEnable))
        /* 1.2.1 Rx is 3G+, go into vsp */
        if(RxPort->content.video->info.LaneFreq >= 310)
            TxConnectStyle = ROUTE_2_R_C_T;
        /* 1.2.2 Rx is 3G-, bypass */
        else
        #endif
        /* 1.1 6G Tx Port */
        if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC) && /* support SCDC */
           (TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_6G))     /* support 6G */
        {
            /* 1.1.1 default bypass */
            TxConnectStyle = ROUTE_1_R_T;
        }
        /* 1.2 3G Tx Port */
        else if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_3G)) /* support 3G */
        {
            /* 1.2.1 Rx is 6G, go into vsp, if no 420, then downscale */
            if(RxPort->content.video->info.TmdsFreq >= 580)
            {
                switch(RxPort->content.video->timing.Vic)
                {
                    case 96:  /* 4K50 */
                    case 106:
                        if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4K50_420)
                            TxConnectStyle = ROUTE_2_R_C_T;
                        else
                            TxConnectStyle = ROUTE_3_R_S_T;
                        break;
                    case 97:  /* 4K60 */
                    case 107:
                        if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4K60_420)
                            TxConnectStyle = ROUTE_2_R_C_T;
                        else
                            TxConnectStyle = ROUTE_3_R_S_T;
                        break;
                    case 101: /* 4KS50 */
                        if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4KS50_420)
                            TxConnectStyle = ROUTE_2_R_C_T;
                        else
                            TxConnectStyle = ROUTE_3_R_S_T;
                        break;
                    case 102: /* 4KS60 */
                        if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4KS60_420)
                            TxConnectStyle = ROUTE_2_R_C_T;
                        else
                            TxConnectStyle = ROUTE_3_R_S_T;
                        break;
                    case 0:
                        TxConnectStyle = ROUTE_NO_CONNECT;
                        break;
                    default: /* down scale to output */
                        TxConnectStyle = ROUTE_3_R_S_T;
                        break;
                }
            }
            /* 1.2.2 Rx is 3G */
            else if(RxPort->content.video->info.TmdsFreq >= 290)
            {
                /* 1.2.2.1 bypass 420 if support, or else, downscale */
                if(RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
                {
                    /* 1.2.2.1.1 only do 420 if support format */
                    if((RxPort->content.video->InCs == AV_CS_YUV_601) ||
                       (RxPort->content.video->InCs == AV_CS_YUV_709) ||
                       (RxPort->content.video->InCs == AV_CS_YCC_601) ||
                       (RxPort->content.video->InCs == AV_CS_YCC_709) ||
                       (RxPort->content.video->InCs == AV_CS_SYCC_601) ||
                       (RxPort->content.video->InCs == AV_CS_ADOBE_YCC_601) ||
                       (RxPort->content.video->InCs == AV_CS_BT2020_YCC) ||
                       (RxPort->content.video->InCs == AV_CS_BT2020_RGB) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_BT2020_YCC) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_BT2020_RGB) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_YUV_601) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_YUV_709) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_YCC_601) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_YCC_709) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_SYCC_601) ||
                       (RxPort->content.video->InCs == AV_CS_LIM_ADOBE_YCC_601))
                    {
                        switch(RxPort->content.video->timing.Vic)
                        {
                            case 96:  /* 4K50 */
                            case 106:
                                if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4K50_420)
                                    TxConnectStyle = ROUTE_1_R_T;
                                else
                                    TxConnectStyle = ROUTE_3_R_S_T;
                                break;
                            case 97:  /* 4K60 */
                            case 107:
                                if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4K60_420)
                                    TxConnectStyle = ROUTE_1_R_T;
                                else
                                    TxConnectStyle = ROUTE_3_R_S_T;
                                break;
                            case 101: /* 4KS50 */
                                if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4KS50_420)
                                    TxConnectStyle = ROUTE_1_R_T;
                                else
                                    TxConnectStyle = ROUTE_3_R_S_T;
                                break;
                            case 102: /* 4KS60 */
                                if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4KS60_420)
                                    TxConnectStyle = ROUTE_1_R_T;
                                else
                                    TxConnectStyle = ROUTE_3_R_S_T;
                                break;
                            case 0:
                                TxConnectStyle = ROUTE_NO_CONNECT;
                                break;
                            default: /* down scale to output */
                                TxConnectStyle = ROUTE_3_R_S_T;
                                break;
                        }
                    }
                    /* 1.2.2.1.2 still bypass it */
                    else
                        TxConnectStyle = ROUTE_1_R_T;
                }
                else
                    TxConnectStyle = ROUTE_1_R_T;
            }
            /* 1.2.3 Rx is below 3G */
            else
                TxConnectStyle = ROUTE_1_R_T;
        }
        /* 1.3 1G5 Tx Port */
        else if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_1G5)) /* support 1G5 */
        {
            /* 1.3.2 Rx is 6G, go into vsp, downscale */
            if(RxPort->content.video->info.TmdsFreq >= 580)
                TxConnectStyle = ROUTE_3_R_S_T;
            /* 1.3.3 Rx is 3G */
            else if(RxPort->content.video->info.TmdsFreq >= 290)
                TxConnectStyle = ROUTE_3_R_S_T;
            /* 1.3.4 Rx is below 3G */
            else
                TxConnectStyle = ROUTE_1_R_T;
        }
        /* 1.4 Don't care, direct output */
        else
            TxConnectStyle = ROUTE_1_R_T;
    }
#if AvEnableVideoLogicBus
    /* 2. RxPort - LogicTxPort connection */
    else if((RxPort->type == HdmiRx) && (TxPort->type == LogicVideoTx))
    {
        /* 2.1 UDP mode */
        if((ParallelConfigTable[TxPort->content.lvtx->Config*3 + 2] & 0x20) != 0)
            TxConnectStyle = ROUTE_4_R_V;
        /* 2.2 480i/576i -> 480p/576p conversion for low frequency protection */
        else if((RxPort->content.video->timing.Interlaced == 1) &&
                (RxPort->content.video->info.TmdsFreq < 45))
            TxConnectStyle = ROUTE_6_R_S_V;
        /* 2.3 RxPort->Downscaler->LogicVideoTx with possible CSC for 1 pixel/clk mode */
        else if(((RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420) &&
                 (RxPort->content.video->info.TmdsFreq > 340)) &&
                ((ParallelConfigTable[TxPort->content.lvtx->Config*3 + 0] & 0x30) == 0))
            TxConnectStyle = ROUTE_6_R_S_V;
        /* 2.4 HDMI 2.0 -> HDMI 1.4 conversion required */
        else if(RxPort->content.video->info.TmdsFreq > TxPort->content.video->info.TmdsFreq)
            TxConnectStyle = ROUTE_6_R_S_V;
        /* 2.5 YCbCr 422 Output */
        else if(((ParallelConfigTable[TxPort->content.lvtx->Config*3 + 1] & 0x04) != 0) ||
                ((ParallelConfigTable[TxPort->content.lvtx->Config*3 + 2] & 0x40) != 0) ||
                (TxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_422))
        {
            /* 2.5.1 Direct connection between RxPort->LogicVideoTx with no CSC */
            if(((RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_444) ||
                (RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_422)) &&
               (TxPort->content.video->InCs == AV_CS_AUTO))
                TxConnectStyle = ROUTE_4_R_V;
            /* 2.5.2 RxPort->CSC->LogicVideoTx */
            else
                TxConnectStyle = ROUTE_5_R_C_V;
        }
        /* 2.6 Color Processing is needed */
        else if(((TxPort->content.video->Y != AV_Y2Y1Y0_INVALID) &&
                 (RxPort->content.video->Y != TxPort->content.video->Y)) ||
                ((TxPort->content.video->InCs != AV_CS_AUTO) &&
                 (RxPort->content.video->InCs != TxPort->content.video->InCs)))
        {
            /* 2.6.1 420 always do downscaling for color space conversion */
            if(RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
                TxConnectStyle = ROUTE_6_R_S_V;
            else
                TxConnectStyle = ROUTE_5_R_C_V;
        }
        /* 2.7 Direct RxPort->LogicVideoTx */
        else
            TxConnectStyle = ROUTE_4_R_V;
    }
    /* 3. LogicRxPort - TxPort connection */
    else if((RxPort->type == LogicVideoRx) && (TxPort->type == HdmiTx))
    {
        /* 3.1 UDP mode */
        if((ParallelConfigTable[TxPort->content.lvtx->Config*3 + 2] & 0x20) != 0)
            TxConnectStyle = ROUTE_7_V_T;
        /* 3.2 YCbCr 422 Input */
        if((ParallelConfigTable[RxPort->content.lvrx->Config*3 + 1] & 0x04) != 0)
        {
            /* 3.2.2 Direct connection between RxPort->LogicVideoTx with no CSC */
            TxConnectStyle = ROUTE_7_V_T;
        }
        /* 3.3 Direct LogicVideoRx->TxPort */
        else
            TxConnectStyle = ROUTE_7_V_T;
    }
#endif
    else
    {
        return ROUTE_NO_CONNECT;
    }

#if !(AvGsv2kxVideoDSCEnable)
    if((TxConnectStyle == ROUTE_3_R_S_T)
        ||(TxConnectStyle == ROUTE_2_R_C_T)
        )
    {
        TxConnectStyle = ROUTE_1_R_T;
    }
    else if((TxConnectStyle == ROUTE_6_R_S_V)
        ||(TxConnectStyle == ROUTE_5_R_C_V)
        )
    {
        TxConnectStyle = ROUTE_4_R_V;
    }
    else if((TxConnectStyle == ROUTE_9_V_S_T)
        ||(TxConnectStyle == ROUTE_8_V_C_T)
        )
    {
        TxConnectStyle = ROUTE_7_V_T;
    }
#endif //add by wf8421

    return TxConnectStyle;
}

#if GSV5K1
RouteStat AvPortRoutingPolicy5k1(AvPort *RxPort, AvPort *TxPort, AvPort *DscPort)
{
    RouteStat TxConnectStyle = ROUTE_NO_CONNECT;

    if(TrxAvUpStreamFlag == 1)
    {
        uint8 i = 0;
        AvDscRatio VicPlan = AV_DSC_1BY1;
        uint16 BlankLen = 0;
        /* 0.1 Check RxPort is valid output */
        if((RxPort->type != HdmiRx) && (RxPort->type != CcaRx))
            return ROUTE_NO_CONNECT;
        /* 0.2 Check TxPort is valid output */
        if(TxPort->index == 5)
        {
            TxPort->content.tx->EdidSupportFeature = TxPort->content.tx->EdidSupportFeature | AV_BIT_FEAT_DSC_ENCODE;
            TxPort->content.tx->EdidSupportFeature = TxPort->content.tx->EdidSupportFeature & (~AV_BIT_FEAT_DSC_DECODE);
            /* 0.2.1 cascade CCAs, bypass */
            if(RxPort->type == CcaRx)
            {
                return ROUTE_1_R_T;
            }
        }
        else if(TxPort->type == HdmiTx)
        {
            if((TxPort->content.tx->EdidReadSuccess != AV_EDID_UPDATED) ||
               (TxPort->content.tx->Hpd != AV_HPD_HIGH))
                return ROUTE_NO_CONNECT;
        }
        else
        {
            return ROUTE_NO_CONNECT;
        }
        /* 1.0 Dsc Decode and Encode Found */
        /* 1.0.1 Dsc Encode */
        if(TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE)
        {
            /* 1.0.1.1 TxB fix the routing Rx-CP-DSC-TxB for Threshold above Timing */
            if(RxPort->content.video->info.LaneFreq > AvCompressionLowThreshold)
            {
                /* 1.0.1.1.1 Default Plan is 420 compression */
                VicPlan = AV_CSC_2BY1;
                BlankLen = RxPort->content.video->timing.HSync + RxPort->content.video->timing.HBack;
                if(RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
                    BlankLen = BlankLen << 1;
                /* 1.0.1.1.2 Find Individual Plan */
                for(i=0;DscPlanVicTable[i].InVic!=255;i++)
                {
                    if(DscPlanVicTable[i].InVic == RxPort->content.video->timing.Vic)
                    {
                        VicPlan = DscPlanVicTable[i].Ratio;
                        break;
                    }
                }
                /* 1.0.1.1.3 Plan Routing Action */
                if((RxPort->content.video->timing.Interlaced == 1) ||
                   ((BlankLen < 120) && (BlankLen != 0)))
                    TxConnectStyle = ROUTE_1_R_T;
                else
                {
                    switch(VicPlan)
                    {
                        case AV_DSC_2BY1:
                        case AV_DSC_3BY1:
                        case AV_DSC_4BY1:
                        case AV_DSC_6BY1:
                        case AV_DSC_8BY1:
                            TxConnectStyle = ROUTE_5_R_C_D_T;
                            break;
                        case AV_CSC_2BY1:
                            TxConnectStyle = ROUTE_2_R_C_T;
                            break;
                        default:
                            TxConnectStyle = ROUTE_1_R_T;
                            break;
                    }
                }
            }
            /* 1.0.1.2 TxB directly get stream from Rx for Threshold below timing */
            else
                TxConnectStyle = ROUTE_1_R_T;
        }
        /* 1.1 Tx Port is HDMI, don't occupy internal block */
        else
        {
            /* 1.1.1 default bypass */
            TxConnectStyle = ROUTE_1_R_T;
        }
    }
    if(TrxAvUpStreamFlag == 0)
    {
        /* 0.1 Check RxPort is valid output */
        if((RxPort->type != HdmiRx) && (RxPort->type != CcaRx))
            return ROUTE_NO_CONNECT;
#if (AvEthCascading == 1)
        /* 0.2 Check TxPort is valid output */
        if(TxPort->index == 5)
        {
            TxPort->content.tx->EdidSupportFeature = TxPort->content.tx->EdidSupportFeature | AV_BIT_FEAT_DSC_ENCODE;
            TxPort->content.tx->EdidSupportFeature = TxPort->content.tx->EdidSupportFeature & (~AV_BIT_FEAT_DSC_DECODE);
            /* 0.2.1 cascade CCAs, bypass */
            if(RxPort->type == CcaRx)
            {
                return ROUTE_1_R_T;
            }
        }
#endif
        /* 1.1 TxA Port */
        else if(TxPort->type == HdmiTx)
        {
            if((TxPort->content.tx->EdidReadSuccess != AV_EDID_UPDATED) ||
               (TxPort->content.tx->Hpd != AV_HPD_HIGH))
                return ROUTE_NO_CONNECT;
            else if(GetDscStream[DscPort->device->index] == 1)
            {
                /* 1.1.1 4K Input but downstream only supports 2K */
                if((DscPort->content.dsc->DscInVic >= 93) && (DscPort->content.dsc->DscInVic < 127)) /* DSC 4K Timing */
                {
                    switch(DscPort->content.dsc->DscInVic)
                    {
                        case 93:  /* 4K@24Hz */
                        case 98:  /* 4K SMPTE@24Hz */
                        case 95:  /* 4K@30Hz */
                        case 100: /* 4K SMPTE@30Hz */
                        case 94:  /* 4K@25Hz */
                        case 99:  /* 4K SMPTE@25Hz */
                            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_3G) == 0) /* no 3G Support */
                                TxConnectStyle = ROUTE_8_R_D_S_T;
                            else
                                TxConnectStyle = ROUTE_6_R_D_T;
                            break;
                        case 97:  /* 4K@60Hz, 600MHz */
                        case 107: /* 4K@60Hz, 600MHz */
                            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_6G) != 0) /* 6G Support */
                                TxConnectStyle = ROUTE_6_R_D_T;
                            else if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4K60_420) != 0) /* 420 Support */
                                TxConnectStyle = ROUTE_7_R_D_C_T;
                            else
                                TxConnectStyle = ROUTE_8_R_D_S_T;
                            break;
                        case 96:  /* 4K@50Hz, 600MHz */
                        case 106: /* 4K@50Hz, 600MHz */
                            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_6G) != 0) /* 6G Support */
                                TxConnectStyle = ROUTE_6_R_D_T;
                            else if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4K50_420) != 0) /* 420 Support */
                                TxConnectStyle = ROUTE_7_R_D_C_T;
                            else
                                TxConnectStyle = ROUTE_8_R_D_S_T;
                            break;
                        case 101: /* 4096x2160@50Hz, 600MHz */
                            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_6G) != 0) /* 6G Support */
                                TxConnectStyle = ROUTE_6_R_D_T;
                            else if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4KS50_420) != 0) /* 420 Support */
                                TxConnectStyle = ROUTE_7_R_D_C_T;
                            else
                                TxConnectStyle = ROUTE_8_R_D_S_T;
                            break;
                        case 102: /* 4096x2160@60Hz, 600MHz */
                            if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_6G) != 0) /* 6G Support */
                                TxConnectStyle = ROUTE_6_R_D_T;
                            else if((TxPort->content.tx->EdidSupportFeature & AV_BIT_FEAT_4KS60_420) != 0) /* 420 Support */
                                TxConnectStyle = ROUTE_7_R_D_C_T;
                            else
                                TxConnectStyle = ROUTE_8_R_D_S_T;
                            break;
                        default:
                            TxConnectStyle = ROUTE_6_R_D_T;
                            break;
                    }
                }
                /* 1.1.2 use Rx-DSC-TxA as default */
                else
                    TxConnectStyle = ROUTE_6_R_D_T;
            }
            else if(GetCscStream[DscPort->device->index] == 1)
            {
                /* 1.2 CSC 420->444 Conversion */
                TxConnectStyle = ROUTE_2_R_C_T;
            }
            else
                TxConnectStyle = ROUTE_1_R_T;
        }
        else
            return ROUTE_NO_CONNECT;
    }

    return TxConnectStyle;
}
#endif

RouteStat AvPortRoutingMap(AvPort *RxPort, AvPort *TxPort, AvPort *ScalerPort, AvPort *ColorPort)
{

    RouteStat TxCurrentStyle = ROUTE_NO_CONNECT;
    if(TxPort->type == HdmiTx)
    {
        if(TxPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
        {
            if(RxPort->type == HdmiRx)
                TxCurrentStyle = ROUTE_1_R_T;
            else if(RxPort->type == LogicVideoRx)
                TxCurrentStyle = ROUTE_7_V_T;
        }
        else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)ColorPort)
        {
            if(ColorPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            {
                if(RxPort->type == HdmiRx)
                    TxCurrentStyle = ROUTE_2_R_C_T;
                else if(RxPort->type == LogicVideoRx)
                    TxCurrentStyle = ROUTE_8_V_C_T;
            }
            else
                TxCurrentStyle = ROUTE_NO_CONNECT;
        }
        else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)ScalerPort)
        {
            if(ScalerPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            {
                if(RxPort->type == HdmiRx)
                    TxCurrentStyle = ROUTE_3_R_S_T;
                else if(RxPort->type == LogicVideoRx)
                    TxCurrentStyle = ROUTE_9_V_S_T;
            }
            else
                TxCurrentStyle = ROUTE_NO_CONNECT;
        }
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else if(TxPort->type == LogicVideoTx)
    {
        if(TxPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_4_R_V;
        else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)ColorPort)
        {
            if(ColorPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
                TxCurrentStyle = ROUTE_5_R_C_V;
        }
        else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)ScalerPort)
        {
            if(ScalerPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
                TxCurrentStyle = ROUTE_6_R_S_V;
        }
    }
    return TxCurrentStyle;
}

#if GSV5K1
RouteStat AvPortRoutingMap5k1(AvPort *RxPort, AvPort *TxPort, AvPort *ScalerPort, AvPort *ColorPort, AvPort *DscPort)
{
    RouteStat TxCurrentStyle = ROUTE_NO_CONNECT;
    if(TxPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
        TxCurrentStyle = ROUTE_1_R_T;
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)ColorPort)
    {
        if(ColorPort->content.RouteVideoFromPort == (struct AvPort*)DscPort)
            TxCurrentStyle = ROUTE_7_R_D_C_T;
        else if(ColorPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_2_R_C_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)ScalerPort)
    {
        if(ScalerPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_3_R_S_T;
        else if(ScalerPort->content.RouteVideoFromPort == (struct AvPort*)DscPort)
            TxCurrentStyle = ROUTE_8_R_D_S_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)DscPort)
    {
        if(DscPort->content.RouteVideoFromPort == (struct AvPort*)ColorPort)
            TxCurrentStyle = ROUTE_5_R_C_D_T;
        else if(DscPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_6_R_D_T;
        else if(DscPort->content.RouteVideoFromPort == (struct AvPort*)ScalerPort)
            TxCurrentStyle = ROUTE_4_R_S_D_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else
        TxCurrentStyle = ROUTE_NO_CONNECT;
    return TxCurrentStyle;
}
#endif


RouteStat AvPortRoutingMap2k6(AvPort *RxPort, AvPort *TxPort, AvPort *Scaler1Port,
                                    AvPort *Color1Port,AvPort *Scaler2Port, AvPort *Color2Port)
{

    RouteStat TxCurrentStyle = ROUTE_NO_CONNECT;
    if(TxPort->content.RouteVideoFromPort == (struct AvPort*)RxPort)
        TxCurrentStyle = ROUTE_1_R_T;
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)Color1Port)
    {
        if(Color1Port->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_2_R_C_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)Scaler1Port)
    {
        if(Scaler1Port->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_3_R_S_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)Color2Port)
    {
        if(Color2Port->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_2_R_C_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else if(TxPort->content.RouteVideoFromPort == (struct AvPort*)Scaler2Port)
    {
        if(Scaler2Port->content.RouteVideoFromPort == (struct AvPort*)RxPort)
            TxCurrentStyle = ROUTE_3_R_S_T;
        else
            TxCurrentStyle = ROUTE_NO_CONNECT;
    }
    else
        TxCurrentStyle = ROUTE_NO_CONNECT;
    return TxCurrentStyle;
}

void AvPortSetRouting(AvPort *RxPort, AvPort *TxPort, AvPort *ColorPort, AvPort *ScalerPort, uint8 TxConnectStyle)
{
    uint8 MessageFlag = 0;
    switch(TxConnectStyle)
    {
        case ROUTE_1_R_T:
        case ROUTE_4_R_V:
        case ROUTE_7_V_T:
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxConnectStyle == ROUTE_1_R_T)
                {
                    AvKapiOutputDebugMessage("New Route: Rx%d-Tx%d",RxPort->index+1, TxPort->index-3);
                }
                else if(TxConnectStyle == ROUTE_4_R_V)
                {
                    AvKapiOutputDebugMessage("New Route: Rx%d-LogicVideoTx",RxPort->index+1);
                }
                else
                {
                    AvKapiOutputDebugMessage("New Route: LogicVideoRx-Tx%d",TxPort->index-3);
                }
            }
            break;
        case ROUTE_2_R_C_T:
        case ROUTE_5_R_C_V:
        case ROUTE_8_V_C_T:
            if(ColorPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    ColorPort,  AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)ColorPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ColorPort, TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxConnectStyle == ROUTE_2_R_C_T)
                {
                    AvKapiOutputDebugMessage("New Route: Rx%d-Color-Tx%d",RxPort->index+1, TxPort->index-3);
                }
                else if(TxConnectStyle == ROUTE_5_R_C_V)
                {
                    AvKapiOutputDebugMessage("New Route: Rx%d-Color-LogicVideoTx",RxPort->index+1);
                }
                else
                {
                    AvKapiOutputDebugMessage("New Route: LogicVideoRx-Color-Tx%d",TxPort->index-3);
                }
            }
            break;
        case ROUTE_3_R_S_T:
        case ROUTE_6_R_S_V:
        case ROUTE_9_V_S_T:
            if(ScalerPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    ScalerPort, AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)ScalerPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ScalerPort, TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxConnectStyle == ROUTE_3_R_S_T)
                {
                    AvKapiOutputDebugMessage("New Route: Rx%d-Scaler-Tx%d",RxPort->index+1, TxPort->index-3);
                }
                else if(TxConnectStyle == ROUTE_6_R_S_V)
                {
                    AvKapiOutputDebugMessage("New Route: Rx%d-Scaler-LogicVideoTx",RxPort->index+1);
                }
                else
                {
                    AvKapiOutputDebugMessage("New Route: LogicVideoRx-Scaler-Tx%d", TxPort->index-3);
                }
            }
            break;
    }
}

#if GSV5K1
void AvPortSetRouting5k1(AvPort *RxPort, AvPort *TxPort, AvPort *ColorPort, AvPort *ScalerPort, AvPort *DscPort, uint8 TxConnectStyle)
{
    uint8 MessageFlag = 0;
    switch(TxConnectStyle)
    {
        case ROUTE_1_R_T:
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
                AvKapiOutputDebugMessage("New Route: Rx%d-Tx%d",RxPort->index+1, TxPort->index-3);
            break;
        case ROUTE_2_R_C_T:
            if(ColorPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    ColorPort,  AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)ColorPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ColorPort, TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
                AvKapiOutputDebugMessage("New Route: Rx%d-Color-Tx%d",RxPort->index+1, TxPort->index-3);
            break;
        case ROUTE_3_R_S_T:
            if(ScalerPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    ScalerPort, AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)ScalerPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ScalerPort, TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
                AvKapiOutputDebugMessage("New Route: Rx%d-Scaler-Tx%d",RxPort->index+1, TxPort->index-3);
            break;
        case ROUTE_4_R_S_D_T:
            if(ScalerPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    ScalerPort,  AvConnectVideo);
            }
            if(DscPort->content.RouteVideoFromPort != (struct AvPort*)ScalerPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ScalerPort,DscPort,    AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)DscPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(DscPort,   TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
                AvKapiOutputDebugMessage("New Route: Rx%d-Scaler-Dsc-Tx%d",RxPort->index+1, TxPort->index-3);
            break;
        case ROUTE_5_R_C_D_T:
            if(ColorPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,    ColorPort,  AvConnectVideo);
            }
            if(DscPort->content.RouteVideoFromPort != (struct AvPort*)ColorPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ColorPort, DscPort, AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)DscPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(DscPort,   TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxPort->type == CcaTx)
                    AvKapiOutputDebugMessage("New Route: Rx%d-Color-Dsc-CcaTx",RxPort->index+1);
                else
                    AvKapiOutputDebugMessage("New Route: Rx%d-Color-Dsc-Tx%d",RxPort->index+1, TxPort->index-3);
            }
            break;
        case ROUTE_6_R_D_T:
            if(DscPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,     DscPort,    AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)DscPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(DscPort,    TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxPort->type == CcaTx)
                    AvKapiOutputDebugMessage("New Route: Rx%d-Dsc-CcaTx", RxPort->index+1);
                else if(RxPort->type == CcaRx)
                    AvKapiOutputDebugMessage("New Route: CcaRx-Dsc-Tx%d", TxPort->index-3);
                else
                    AvKapiOutputDebugMessage("New Route: Rx%d-Dsc-Tx%d",RxPort->index+1, TxPort->index-3);
            }
            break;
        case ROUTE_7_R_D_C_T:
            if(DscPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,     DscPort,    AvConnectVideo);
            }
            if(ColorPort->content.RouteVideoFromPort != (struct AvPort*)DscPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(DscPort,    ColorPort,  AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)ColorPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ColorPort,    TxPort,   AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxPort->type == CcaRx)
                    AvKapiOutputDebugMessage("New Route: CcaRx-Dsc-Color-Tx%d", TxPort->index-3);
                else
                    AvKapiOutputDebugMessage("New Route: Rx%d-Dsc-Color-Tx%d",RxPort->index+1, TxPort->index-3);
            }
            break;
        case ROUTE_8_R_D_S_T:
            if(DscPort->content.RouteVideoFromPort != (struct AvPort*)RxPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(RxPort,     DscPort,    AvConnectVideo);
            }
            if(ScalerPort->content.RouteVideoFromPort != (struct AvPort*)DscPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(DscPort,    ScalerPort, AvConnectVideo);
            }
            if(TxPort->content.RouteVideoFromPort != (struct AvPort*)ScalerPort)
            {
                MessageFlag = 1;
                AvApiConnectPort(ScalerPort, TxPort,     AvConnectVideo);
            }
            if(MessageFlag == 1)
            {
                if(TxPort->type == CcaTx)
                    AvKapiOutputDebugMessage("New Route: Rx%d-Dsc-Scaler-CcaTx", RxPort->index+1);
                else if(RxPort->type == CcaRx)
                    AvKapiOutputDebugMessage("New Route: CcaRx-Dsc-Scaler-Tx%d", TxPort->index-3);
                else
                    AvKapiOutputDebugMessage("New Route: Rx%d-Dsc-Scaler-Tx%d",RxPort->index+1, TxPort->index-3);
            }
            break;
    }
}

#endif
#if GSV2K6
AvPort* AllocateFromRxPort(AvPort *RxAPort, AvPort *RxBPort, AvPort *TxPort)
{
    switch(TxPort->index)
    {
        case 4:
            if(RegTxAFrom == 1)
                return RxAPort;
            else
                return RxBPort;
        case 5:
            if(RegTxBFrom == 1)
                return RxAPort;
            else
                return RxBPort;
        case 6:
            if(RegTxCFrom == 1)
                return RxAPort;
            else
                return RxBPort;
        case 7:
            if(RegTxDFrom == 1)
                return RxAPort;
            else
                return RxBPort;
        default:
            return NULL;
    }
}
RouteStat MergeConnectStyle(RouteStat Style1, RouteStat Style2)
{
    if((Style1 == ROUTE_3_R_S_T) || (Style2 == ROUTE_3_R_S_T))
        return ROUTE_3_R_S_T;
    else if((Style1 == ROUTE_2_R_C_T) || (Style2 == ROUTE_2_R_C_T))
        return ROUTE_2_R_C_T;
    else if((Style1 == ROUTE_1_R_T) || (Style2 == ROUTE_1_R_T))
        return ROUTE_1_R_T;
    else
        return ROUTE_NO_CONNECT;
}
#endif

AvRet AvPortConnectUpdate(AvDevice *Device)
{
    g_u8ChipDev = Device->index;    // add by wf8421 20230113
    g_u8GsvChipType[Device->index] = Device->type;

#if GSV2K8
    /* 0. only process Gsv2k8 device */
    if(Device->type == Gsv2k8)
    {
        AvPort *RxAPort;
        AvPort *TxAPort;
        AvPort *TxBPort;
        AvPort *Rx1Port;
        AvPort *Rx2Port;
        AvPort *Color1Port;
        AvPort *Scaler1Port;
        AvPort *Color2Port;
        AvPort *Scaler2Port;
        //uint8  i = 0;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        RouteStat TxAConnectStyle = ROUTE_NO_CONNECT;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        RouteStat TxBConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxACurrentStyle = ROUTE_NO_CONNECT;
        RouteStat TxBCurrentStyle = ROUTE_NO_CONNECT;
        //for(i=0;i<AvGsv2k8DeviceNumber;i++)
        {
            /* Decide the vsp management */
            /* 1. Prepare the ports */
            //RxAPort     = (AvPort*)(Device[i].port);
            RxAPort     = (AvPort*)Device->port;
            TxAPort     = &RxAPort[4];
            TxBPort     = &RxAPort[5];
            #if AvMatrix4x4Mode
            Scaler1Port = &RxAPort[6];
            Scaler2Port = &RxAPort[7];
            Color1Port  = &RxAPort[8];
            Color2Port  = &RxAPort[9];
            #else
            Scaler1Port = &RxAPort[8];
            Scaler2Port = &RxAPort[9];
            Color1Port  = &RxAPort[10];
            Color2Port  = &RxAPort[11];
            #endif
            /* 1.1 Find Valid RxPort */
            /* 1.1.2 Check RxA is valid or not */
            /* 2. Find Feasible Routing Solution */
            /* 3. Find Tx Current Routing Map */
            #if AvMatrix4x4Mode
            if(KfunFindVideoRxFrontInSameDevice(TxAPort, &Rx1Port) == AvOk)
            #else
            if(KfunFindVideoRxFront(TxAPort, &Rx1Port) == AvOk)
            #endif
            {
                TxACurrentStyle = AvPortRoutingMap(Rx1Port, TxAPort, Scaler1Port, Color1Port);
                if(Rx1Port->content.rx->IsInputStable != 1)
                    TxAConnectStyle = TxACurrentStyle;
                else
                    TxAConnectStyle = AvPortRoutingPolicy(Rx1Port, TxAPort);
            }
            #if AvMatrix4x4Mode
            if(KfunFindVideoRxFrontInSameDevice(TxBPort, &Rx2Port) == AvOk)
            #else
            if(KfunFindVideoRxFront(TxBPort, &Rx2Port) == AvOk)
            #endif
            {
                TxBCurrentStyle = AvPortRoutingMap(Rx2Port, TxBPort, Scaler2Port, Color2Port);
                if(Rx2Port->content.rx->IsInputStable != 1)
                    TxBConnectStyle = TxBCurrentStyle;
                else
                    TxBConnectStyle = AvPortRoutingPolicy(Rx2Port, TxBPort);
            }
            /* 4. New Plan for Connection */
            if((Rx1Port == Rx2Port) && /* 1to2 splitter mode */
               (((TxAConnectStyle == ROUTE_3_R_S_T) && (TxBConnectStyle == ROUTE_2_R_C_T)) || /* CP is configured in different modes */
                ((TxBConnectStyle == ROUTE_3_R_S_T) && (TxAConnectStyle == ROUTE_2_R_C_T))))  /* CP is configured in different modes */
            {
                TxAConnectStyle = ROUTE_3_R_S_T;
                TxBConnectStyle = ROUTE_3_R_S_T;
            }
            /* 5. Action */
            if((TxACurrentStyle != TxAConnectStyle) && (TxAConnectStyle != ROUTE_NO_CONNECT))
                AvPortSetRouting(Rx1Port, TxAPort, Color1Port, Scaler1Port, TxAConnectStyle);
            if((TxBCurrentStyle != TxBConnectStyle) && (TxBConnectStyle != ROUTE_NO_CONNECT))
                AvPortSetRouting(Rx2Port, TxBPort, Color2Port, Scaler2Port, TxBConnectStyle);

            #if AvGsv2k8VideoRgb_422_444To420
            AvUserCheckTxLinkSta(Rx1Port, TxAPort);
            AvUserCheckTxLinkSta(Rx2Port, TxBPort);
            #endif
        }
    }
#endif

#if (GSV2K6&&(!AvMatrix4x4Mode))
    /* 0. only process Gsv2k6 device */
    if(Device->type == Gsv2k6)
    {
        AvPort *RxAPort;
        AvPort *RxBPort;
        AvPort *RxPort;
        AvPort *TxAPort;
        AvPort *TxBPort;
        AvPort *TxCPort;
        AvPort *TxDPort;
        AvPort *AudioPort;
        AvPort *Scaler1Port;
        AvPort *Color1Port;
//        AvPort *Scaler2Port;
//        AvPort *Color2Port;
        /* 1-bypass, 2-color, 3-scale */
        RouteStat TxAConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxBConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxCConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxDConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxACurrentStyle = ROUTE_NO_CONNECT;
        RouteStat TxBCurrentStyle = ROUTE_NO_CONNECT;
        RouteStat TxCCurrentStyle = ROUTE_NO_CONNECT;
        RouteStat TxDCurrentStyle = ROUTE_NO_CONNECT;
//        RouteStat Tx1ConnectStyle = ROUTE_NO_CONNECT;
//        RouteStat Tx2ConnectStyle = ROUTE_NO_CONNECT;

        /* Decide the vsp management */
        /* 1. Prepare the ports */
        RxAPort     = (AvPort*)Device->port;
        RxBPort     = &RxAPort[1];
        TxAPort     = &RxAPort[2];
        TxBPort     = &RxAPort[3];
        TxCPort     = &RxAPort[4];
        TxDPort     = &RxAPort[5];
        AudioPort   = &RxAPort[6];
        Scaler1Port = &RxAPort[8];
        Color1Port  = &RxAPort[9];
//        Scaler2Port = &RxAPort[9];
//        Color2Port  = &RxAPort[10];

        /* 1.1.1 Find Current Rx Input Selection */
        if(KfunFindAudioRxFront(AudioPort, &RxPort) == AvOk)
        {
            #if AvEnableSourceChangeUseDemo
            /* Switch Input */
            if(RxPort->index != KeyRxBInput)
            {
                RxPort = &RxAPort[KeyRxBInput];
                AvApiConnectPort(RxPort, AudioPort, AvConnectAV);
                //RxInLedOut(KeyRxBInput);
            }
            #endif
        }
        /* 2. Find Feasible Routing Solution */
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxAPort);
        TxAConnectStyle = AvPortRoutingPolicy(RxPort, TxAPort);
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxBPort);
        TxBConnectStyle = AvPortRoutingPolicy(RxPort, TxBPort);
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxCPort);
        TxCConnectStyle = AvPortRoutingPolicy(RxPort, TxCPort);
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxDPort);
        TxDConnectStyle = AvPortRoutingPolicy(RxPort, TxDPort);
        /* 3. Find Tx Current Routing Map */
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxAPort);
        TxACurrentStyle = AvPortRoutingMap2k6(RxPort, TxAPort, Scaler1Port, Color1Port, Scaler1Port, Color1Port);
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxBPort);
        TxBCurrentStyle = AvPortRoutingMap2k6(RxPort, TxBPort, Scaler1Port, Color1Port, Scaler1Port, Color1Port);
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxCPort);
        TxCCurrentStyle = AvPortRoutingMap2k6(RxPort, TxCPort, Scaler1Port, Color1Port, Scaler1Port, Color1Port);
        RxPort = AllocateFromRxPort(RxAPort, RxBPort, TxDPort);
        TxDCurrentStyle = AvPortRoutingMap2k6(RxPort, TxDPort, Scaler1Port, Color1Port, Scaler1Port, Color1Port);
        /* 4. New Plan for Connection */
        if((TxAConnectStyle == ROUTE_3_R_S_T) ||
           (TxBConnectStyle == ROUTE_3_R_S_T) ||
           (TxCConnectStyle == ROUTE_3_R_S_T) ||
           (TxDConnectStyle == ROUTE_3_R_S_T))
        {
            if(TxAConnectStyle == ROUTE_2_R_C_T)
                TxAConnectStyle = ROUTE_3_R_S_T;
            if(TxBConnectStyle == ROUTE_2_R_C_T)
                TxBConnectStyle = ROUTE_3_R_S_T;
            if(TxCConnectStyle == ROUTE_2_R_C_T)
                TxCConnectStyle = ROUTE_3_R_S_T;
            if(TxDConnectStyle == ROUTE_2_R_C_T)
                TxDConnectStyle = ROUTE_3_R_S_T;
        }

        /* 5. Action */
        if((TxACurrentStyle != TxAConnectStyle) && (TxAConnectStyle != ROUTE_NO_CONNECT))
        {
                AvPortSetRouting(RxAPort, TxAPort, Color1Port, Scaler1Port, TxAConnectStyle);
        }
        if((TxBCurrentStyle != TxBConnectStyle) && (TxBConnectStyle != ROUTE_NO_CONNECT))
        {
                AvPortSetRouting(RxAPort, TxBPort, Color1Port, Scaler1Port, TxBConnectStyle);
        }
        if((TxCCurrentStyle != TxCConnectStyle) && (TxCConnectStyle != ROUTE_NO_CONNECT))
        {
                AvPortSetRouting(RxAPort, TxCPort, Color1Port, Scaler1Port, TxCConnectStyle);
        }
        if((TxDCurrentStyle != TxDConnectStyle) && (TxDConnectStyle != ROUTE_NO_CONNECT))
        {
                AvPortSetRouting(RxAPort, TxDPort, Color1Port, Scaler1Port, TxDConnectStyle);
        }
    }
#endif
#if GSV2K2
    if(Device->type == Gsv2k2)
    {
        AvPort *RxAPort;
        AvPort *RxBPort;
        AvPort *TxAPort;
        AvPort *TxBPort;
        AvPort *RxPort;
        AvPort *ColorPort;
        AvPort *ScalerPort;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        volatile RouteStat TxAConnectStyle = ROUTE_NO_CONNECT;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        volatile RouteStat TxBConnectStyle = ROUTE_NO_CONNECT;
        volatile RouteStat TxACurrentStyle = ROUTE_NO_CONNECT;
        volatile RouteStat TxBCurrentStyle = ROUTE_NO_CONNECT;

        /* Decide the vsp management */
        /* 1. Prepare the ports */
        RxAPort    = (AvPort*)Device->port;
        RxBPort    = &RxAPort[1];
        TxAPort    = &RxAPort[2];
        TxBPort    = &RxAPort[3];
        ScalerPort = &RxAPort[5];
        ColorPort  = &RxAPort[6];
        /* 1.1 Find Valid RxPort */
        /* 1.1.1 Find Current Rx Input Selection */
        if(KfunFindVideoRxFront(TxAPort, &RxPort) == AvOk)
        {
            #if AvEnableSourceChangeUseDemo
            if(RxPort->index == 0) /* RxA Selected */
            {
                if(Gsv2k2RxBPortSel == 1)
                {
                    RxPort = RxBPort;
                    AvApiConnectPort(RxBPort, TxAPort, AvConnectAV);
                    AvApiConnectPort(RxBPort, TxBPort, AvConnectAV);
                }
            }
            else if(RxPort->index == 1) /* RxB Selected */
            {
                if(Gsv2k2RxBPortSel == 0)
                {
                    RxPort = RxAPort;
                    AvApiConnectPort(RxAPort, TxAPort, AvConnectAV);
                    AvApiConnectPort(RxAPort, TxBPort, AvConnectAV);
                }
            }
            #else
            UNUSED(RxBPort);
            #endif
        }
        /* 1.1.2 Check RxA is valid or not */
        if(RxPort->content.rx->IsInputStable != 1)
        {
            //return AvOk;
            goto Gsv2k2ActionEnd;
        }
        /* 2. Find Feasible Routing Solution */
        if(TxAPort->type == HdmiTx)
            TxAConnectStyle = AvPortRoutingPolicy(RxPort, TxAPort);
        if(TxBPort->type == HdmiTx)
            TxBConnectStyle = AvPortRoutingPolicy(RxPort, TxBPort);
        if(((TxAConnectStyle == ROUTE_2_R_C_T) && (TxBConnectStyle == ROUTE_3_R_S_T)) ||
           ((TxBConnectStyle == ROUTE_2_R_C_T) && (TxAConnectStyle == ROUTE_3_R_S_T)))
        {
             TxBConnectStyle = ROUTE_3_R_S_T;
             TxAConnectStyle = ROUTE_3_R_S_T;
        }
        /* 3. Find TxA Current Routing Map */
        TxACurrentStyle = AvPortRoutingMap(RxPort, TxAPort, ScalerPort, ColorPort);
        if((TxACurrentStyle != TxAConnectStyle) && (TxAConnectStyle != ROUTE_NO_CONNECT))
        {
            AvPortSetRouting(RxPort, TxAPort, ColorPort, ScalerPort, TxAConnectStyle);
        }
        /* 4. Find TxB Current Routing Map */
        TxBCurrentStyle = AvPortRoutingMap(RxPort, TxBPort, ScalerPort, ColorPort);
        if((TxBCurrentStyle != TxBConnectStyle) && (TxBConnectStyle != ROUTE_NO_CONNECT))
        {
            AvPortSetRouting(RxPort, TxBPort, ColorPort, ScalerPort, TxBConnectStyle);
        }
    }
    Gsv2k2ActionEnd:
#endif
#if GSV2K1
    if(Device->type == Gsv2k1)
    {
         AvPort *TxAPort;
        AvPort *TxBPort;
        AvPort *RxPort;
        AvPort *ColorPort;
        AvPort *ScalerPort;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        RouteStat TxAConnectStyle = ROUTE_NO_CONNECT;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        RouteStat TxBConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxACurrentStyle = ROUTE_NO_CONNECT;
        RouteStat TxBCurrentStyle = ROUTE_NO_CONNECT;

        /* Decide the vsp management */
        /* 1. Prepare the ports */
        RxPort     = (AvPort*)Device->port;
        TxAPort    = &RxPort[1];
        TxBPort    = &RxPort[2];
        ScalerPort = &RxPort[4];
        ColorPort  = &RxPort[5];
        /* 1.1 Find Valid RxPort */
        /* 1.1.2 Check RxA is valid or not */
        if(RxPort->content.rx->IsInputStable != 1)
        {
            //return AvOk;
            goto Gsv2k1ActionEnd;
        }
        /* 2. Find Feasible Routing Solution */
        if(TxAPort->type == HdmiTx)
            TxAConnectStyle = AvPortRoutingPolicy(RxPort, TxAPort);
        if(TxBPort->type == HdmiTx)
            TxBConnectStyle = AvPortRoutingPolicy(RxPort, TxBPort);
        if(((TxAConnectStyle == ROUTE_2_R_C_T) && (TxBConnectStyle == ROUTE_3_R_S_T)) ||
           ((TxBConnectStyle == ROUTE_2_R_C_T) && (TxAConnectStyle == ROUTE_3_R_S_T)))
        {
             TxBConnectStyle = ROUTE_3_R_S_T;
             TxAConnectStyle = ROUTE_3_R_S_T;
        }
    #if 0//(AvEnableTxSameResolution == 1)
        else if((TxAConnectStyle != ROUTE_NO_CONNECT) &&
                (TxBConnectStyle != ROUTE_NO_CONNECT) &&
                (TxAConnectStyle != TxBConnectStyle))
        {
            if(TxAConnectStyle == ROUTE_1_R_T)
                TxAConnectStyle = TxBConnectStyle;
            else
                TxBConnectStyle = TxAConnectStyle;
        }
    #endif
        /* 3. Find TxA Current Routing Map */
        TxACurrentStyle = AvPortRoutingMap(RxPort, TxAPort, ScalerPort, ColorPort);
        if((TxACurrentStyle != TxAConnectStyle) && (TxAConnectStyle != ROUTE_NO_CONNECT))
        {
            AvPortSetRouting(RxPort, TxAPort, ColorPort, ScalerPort, TxAConnectStyle);
        }
        /* 4. Find TxB Current Routing Map */
        TxBCurrentStyle = AvPortRoutingMap(RxPort, TxBPort, ScalerPort, ColorPort);
        if((TxBCurrentStyle != TxBConnectStyle) && (TxBConnectStyle != ROUTE_NO_CONNECT))
        {
            AvPortSetRouting(RxPort, TxBPort, ColorPort, ScalerPort, TxBConnectStyle);
        }

    }
    Gsv2k1ActionEnd:
#endif
#if GSV2K11
    if(Device->type == Gsv2k11)
    {
        AvPort *TxPort;
        AvPort *RxPort;
        AvPort *VideoTxPort;
        AvPort *AudioTxPort;
        AvPort *VideoRxPort;
        AvPort *AudioRxPort;
        AvPort *ColorPort;
        AvPort *ScalerPort;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        volatile RouteStat TxConnectStyle = ROUTE_NO_CONNECT;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        volatile RouteStat TxCurrentStyle = ROUTE_NO_CONNECT;

        /* Decide the vsp management */
        /* 1. Prepare the ports */
        RxPort      = (AvPort*)Device->port;
        TxPort      = &RxPort[1];
        VideoTxPort = &RxPort[2];
        AudioTxPort = &RxPort[3];
        VideoRxPort = &RxPort[7];
        AudioRxPort = &RxPort[8];
        ScalerPort  = &RxPort[4];
        ColorPort   = &RxPort[5];
        /* 1.1 Find Valid RxPort */
        /* 1.1.1 Find Current Rx Input Selection */
        if(KfunFindVideoRxFront(TxPort, &RxPort) == AvOk)
        {
            if(RxPort->type == HdmiRx)
            {
                if(LogicOutputSel[RxPort->device->index] == 0)
                {
                    RxPort = VideoRxPort;
                    AvApiConnectPort(VideoRxPort, TxPort, AvConnectVideo);
                    AvApiConnectPort(AudioRxPort, TxPort, AvConnectAudio);
                    VideoRxPort->content.lvrx->Update     = 1;
                }
            }
            else if(RxPort->type == LogicVideoRx)
            {
                if(LogicOutputSel[RxPort->device->index] == 1)
                {
                    RxPort = (AvPort*)Device->port;
                    AvApiConnectPort(RxPort, TxPort,      AvConnectAV);
                    AvApiConnectPort(RxPort, VideoTxPort, AvConnectVideo);
                    AvApiConnectPort(RxPort, AudioTxPort, AvConnectAudio);
                    VideoTxPort->content.lvtx->Update     = 1;
                }
            }
        }
        /* 1.1.2 Check RxA is valid or not */
        if(RxPort->content.rx->IsInputStable != 1)
    	{
            return AvOk;
    	}
        /* 2. Find Feasible Routing Solution */
        if(LogicOutputSel[RxPort->device->index] == 1)
        {
            /* 2.1 Find RxPort-VideoTxPort Current Routing Map */
        #if AvGsv2k11VideoDSCHRxToHTxEnable
            /* 2.2 Find RxPort-TxPort Routing Map */
            TxConnectStyle = AvPortRoutingPolicy(RxPort, TxPort);
            TxCurrentStyle = AvPortRoutingMap(RxPort, TxPort, ScalerPort, ColorPort);
            if((TxCurrentStyle != TxConnectStyle) && (TxConnectStyle != ROUTE_NO_CONNECT))
                AvPortSetRouting(RxPort, TxPort, ColorPort, ScalerPort, TxConnectStyle);
        #else
//            TxConnectStyle = AvPortRoutingPolicy(RxPort, VideoTxPort);
//            TxCurrentStyle = AvPortRoutingMap(RxPort, VideoTxPort, ScalerPort, ColorPort);
//            if((TxCurrentStyle != TxConnectStyle) && (TxConnectStyle != ROUTE_NO_CONNECT))
//                AvPortSetRouting(RxPort, VideoTxPort, ColorPort, ScalerPort, TxConnectStyle);
        #endif
        }
        else
        {
            /* 2.2 Find VideoRxPort-RxPort Current Routing Map */
            TxConnectStyle = AvPortRoutingPolicy(VideoRxPort, TxPort);
            TxCurrentStyle = AvPortRoutingMap(VideoRxPort, TxPort, ScalerPort, ColorPort);
            if((TxCurrentStyle != TxConnectStyle) && (TxConnectStyle != ROUTE_NO_CONNECT))
    		{
                AvPortSetRouting(VideoRxPort, TxPort, ColorPort, ScalerPort, TxConnectStyle);
    		}
        }
    }
#endif
#if GSV5K1
    if(Device->type == Gsv5k1)
    {
        AvPort *TxAPort;
        AvPort *TxBPort;
        AvPort *RxPort;
        AvPort *DscPort;
        AvPort *ColorPort;
        AvPort *ScalerPort;
#if 0//(AvEnableInternalVideoGen == 1)
        AvPort *VgPort;
#endif
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        RouteStat TxAConnectStyle = ROUTE_NO_CONNECT;
        /* 1-bypass, 2-color, 3-scale, 4-color-scale */
        RouteStat TxBConnectStyle = ROUTE_NO_CONNECT;
        RouteStat TxACurrentStyle = ROUTE_NO_CONNECT;
        RouteStat TxBCurrentStyle = ROUTE_NO_CONNECT;

        /* 0. only process Gsv5k1 device */
        /* Decide the vsp management */
        /* 1. Prepare the ports */
        RxPort     = (AvPort*)Device->port;
        TxAPort    = &RxPort[1];
        TxBPort    = &RxPort[2];
        DscPort    = &RxPort[4];
        ScalerPort = &RxPort[5];
        ColorPort  = &RxPort[6];
#if 0//(AvEnableInternalVideoGen == 1)
        VgPort     = &RxPort[7];
#endif
        /* 1.1 Find Valid RxPort */
#if 0//(AvEnableInternalVideoGen == 1)
        if(RxPort->content.rx->Input5V == 1)
        {
            if(TxBPort->content.RouteVideoFromPort == (struct AvPort*)VgPort)
                AvApiConnectPort(RxPort, TxBPort, AvConnectVideo);
        }
        else
        {
            if(TxBPort->content.RouteVideoFromPort != (struct AvPort*)VgPort)
                AvApiConnectPort(VgPort, TxBPort, AvConnectVideo);
        }
#endif
        if(RxPort->content.rx->IsInputStable == 1)
        {
            /* 1.1.1 Check Dsc Port Request */
            TxAPort->content.tx->EdidSupportFeature = TxAPort->content.tx->EdidSupportFeature & (~AV_BIT_FEAT_DSC_ENCODE);
            TxAPort->content.tx->EdidSupportFeature = TxAPort->content.tx->EdidSupportFeature & (~AV_BIT_FEAT_DSC_DECODE);
            #if Gsv5k1RxSetTxBHdmiMode
            TxBPort->content.tx->EdidSupportFeature = TxBPort->content.tx->EdidSupportFeature & (~AV_BIT_FEAT_DSC_ENCODE);
            #else
            TxBPort->content.tx->EdidSupportFeature = TxBPort->content.tx->EdidSupportFeature | AV_BIT_FEAT_DSC_ENCODE;
            #endif
            TxBPort->content.tx->EdidSupportFeature = TxBPort->content.tx->EdidSupportFeature & (~AV_BIT_FEAT_DSC_DECODE);
            /* 2. Find Feasible Routing Solution */
            TxAConnectStyle = AvPortRoutingPolicy5k1(RxPort, TxAPort, DscPort);
            TxBConnectStyle = AvPortRoutingPolicy5k1(RxPort, TxBPort, DscPort);
            /* 3. Find TxA Current Routing Map */
            TxACurrentStyle = AvPortRoutingMap5k1(RxPort, TxAPort, ScalerPort, ColorPort, DscPort);
            if((TxACurrentStyle != TxAConnectStyle) && (TxAConnectStyle != ROUTE_NO_CONNECT))
            {
                AvPortSetRouting5k1(RxPort, TxAPort, ColorPort, ScalerPort, DscPort, TxAConnectStyle);
            }
            /* 4. Find TxB Current Routing Map */
            TxBCurrentStyle = AvPortRoutingMap5k1(RxPort, TxBPort, ScalerPort, ColorPort, DscPort);
            if((TxBCurrentStyle != TxBConnectStyle) && (TxBConnectStyle != ROUTE_NO_CONNECT))
            {
                AvPortSetRouting5k1(RxPort, TxBPort, ColorPort, ScalerPort, DscPort, TxBConnectStyle);
            }
        }
    }
#endif

    return AvOk;
}

#endif

