/**
 * @file kapi.c
 *
 * @brief kernel api functions of this audio/video software package \n
 *        this file will also be only header file of this software package when user do porting
 */

#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "Gsv_kapi.h"
#include "Gsv_uapi.h"
#include "kernel_status_update.h"

static AvPort *FirstPort = NULL;
static AvPort *PreviousPort = NULL;
AvFpKeyCommand   AvHookKeyCmd;
AvFpUartCommand  AvHookUartCmd;
AvFpIrdaCommand  AvHookIrdaCmd;
void ClearVideoFromPort(pin AvPort *port);
void ClearAudioFromPort(pin AvPort *port);

/**
 * @brief  hookup user layer access functions
 * @return AvOk - success
 */
kapi AvRet AvKapiHookUserFunctions(pin AvFpKeyCommand keyCmd, pin AvFpUartCommand uartCmd,pin AvFpIrdaCommand IrdaCmd)
{
    AvRet ret = AvOk;
    AvHookKeyCmd = keyCmd;
    AvHookUartCmd = uartCmd;
    AvHookIrdaCmd = IrdaCmd;
    return ret;
}

/**
 * @brief  init software
 * @return none
 * @note
 */
kapi AvRet AvApiInit(void)
{
    AvRet ret = AvOk;
    ret = AvUapiInit();
    return ret;
}

/**
 * @brief  add device
 * @return none
 * @note customer is allowed to add customized device to the loop
 * during initialization, the device's detail should be fed
 */
kapi AvRet AvApiAddDevice(AvDevice *device, AvDeviceType type, uint8 index,
                           void *specific, void *port,  void *extension)
{
    device->type      = type;
    device->index     = index;
    device->specific  = specific;
    device->port      = port;
    device->extension = extension;

    return AvOk;

}

/**
 * @brief  add audio/video port
 * @return none
 * @note customer is allowed to add customized port to the loop
 * during initialzation, the port's AvPort and AvPortType
 * must be set, a dedicated ID will be given to this port
 */
kapi AvRet AvApiAddPort(AvDevice *device, pin AvPort *port, uint16 index, AvPortType type)
{
    if(!FirstPort)
        FirstPort = port;

    port->device = device;
    port->index  = index;
    port->type   = type;

    g_u8ChipDev = device->index;    // add by wf8421 20230113
    g_u8GsvChipType[device->index] = device->type;

    AvApiPortMemAllocate(port);

    if(PreviousPort)
        PreviousPort->next = (struct AvPort*)port;
    PreviousPort = port;
    port->next = NULL;

    return AvOk;

}

/**
 * @brief  init device
 * @return none
 * @note device initialization must be done after AddDevice and AddPort
 * Or else, the port structure will crash.
 */
kapi AvRet AvApiInitDevice(AvDevice *device)
{
    g_u8ChipDev = device->index;    // add by wf8421 20230113
    g_u8GsvChipType[device->index] = device->type;
    return AvUapiInitDevice(device);
//    return AvOk;
}

/**
 * @brief  init fsm
 * @return none
 * @note device initialization must be done after initdevice
 * Or else, the port structure will crash.
 */
kapi AvRet AvApiInitFsm(pin AvPort *port)
{
    switch(port->type)
    {
        case HdmiRx:
        {
            AvKapiFsmEnterFunPlugRxFsm(port);
            AvKapiFsmEnterFunHdcpFsm(port);
            AvKapiFsmEnterFunReceiverFsm(port);
            break;
        }
        case DviRx:
        {
            AvKapiFsmEnterFunPlugRxFsm(port);
            AvKapiFsmEnterFunReceiverFsm(port);
            break;
        }
        case AnalogRx:
        {
            AvKapiFsmEnterFunReceiverFsm(port);
            break;
        }
        case LogicVideoRx:
        {
            AvKapiFsmEnterFunReceiverFsm(port);
            break;
        }
        case LogicAudioRx:
        {
            break;
        }
        case HdmiTx:
        {
            AvKapiFsmEnterFunPlugTxFsm(port);
#if AvEnableCecFeature /* CEC Related */
            AvKapiFsmEnterFunCecFsm(port);
#endif
            break;
        }
        case DviTx:
        {
            AvKapiFsmEnterFunPlugTxFsm(port);
            break;
        }
        #if AvEnableCcaFeature
        case CcaTx:
        {
            AvKapiFsmEnterFunPlugTxFsm(port);
            AvKapiFsmEnterFunCcaTxFsm(port);
            #if AvEnableCecFeature /* CEC Related */
            AvKapiFsmEnterFunCecFsm(port);
            #endif
            break;
        }
        case CcaRx:
        {
            AvKapiFsmEnterFunPlugRxFsm(port);
            AvKapiFsmEnterFunHdcpFsm(port);
            AvKapiFsmEnterFunReceiverFsm(port);
            AvKapiFsmEnterFunCcaRxFsm(port);
            break;
        }
        #endif /* AvEnableCcaFeature */
        default:
            break;
    }
    return AvOk;
}


#if 0
void avCloseLogicVideo(uint8 port, uint8 sta)
{
    AvPort* TempPort = FirstPort[port-1];

    GSV2011portSet(port);
    AvHalI2cWriteField8((((TempPort->device->index)<<24) | AvGenerateDeviceAddress(0x00,0x01,0xB0,0x14)), 0x00, 0x1, 0x0, sta);
}
#endif
void videoFreezeRelease(void);

/**
 * @brief  update all ports status
 * @return none
 * @note
 */
kapi AvRet AvApiUpdate(void)
{
    AvPort* TempPort = FirstPort;
    uint8 OldState;

    while(TempPort)
    {
        g_u8ChipDev = TempPort->device->index;
        g_u8GsvChipType[TempPort->device->index] = TempPort->device->type;
        /* Make Key Response and Uart Response faster */
        //AvUserUartCmd(FirstPort);
        //AvUserKeyCmd(FirstPort);
        //AvUserIrdaCmd(FirstPort);
        switch(TempPort->type)
        {
            case HdmiRx:
            case DviRx:
                OldState = *TempPort->content.is_PlugRxFsm;
                KfunCheckPrState(TempPort);
                AvKapiFsmFunPlugRxFsm(TempPort);
                ReportPlugRxFsm(TempPort, OldState);
                OldState = *TempPort->content.is_ReceiverFsm;
                KfunCheckRxState(TempPort);
                AvKapiFsmFunReceiverFsm(TempPort);
                ReportReceiverFsm(TempPort, OldState);
                OldState = *TempPort->content.is_HdcpFsm;
                KfunCheckHdcpState(TempPort);
                AvKapiFsmFunHdcpFsm(TempPort);
                ReportHdcpFsm(TempPort, OldState);
                break;
            case AnalogRx:
            case LogicVideoRx:
//                KfunCheckRxState(TempPort);
//                AvKapiFsmFunReceiverFsm(TempPort);
                KfunCheckLogicVideoRx(TempPort);
            #if (GSV2K11&&AvEnableSimplifyHdcp)
                KfunSimpleHdcpSync(TempPort);
            #endif

                break;
            case LogicAudioRx:
#if AvEnableAudioTTLInput
                KfunCheckLogicAudioRx(TempPort);
#endif
                break;
            case HdmiTx:
                OldState = *TempPort->content.is_PlugTxFsm;
                KfunCheckPtState(TempPort);
                AvKapiFsmFunPlugTxFsm(TempPort);
                ReportPlugTxFsm(TempPort, OldState);
#if AvEnableCecFeature /* CEC Related */
                OldState = *TempPort->content.is_CecFsm;
                KfunCheckCecState(TempPort);
                AvKapiFsmFunCecFsm(TempPort);
                ReportCecFsm(TempPort, OldState);
#endif
                break;
            case DviTx:
                KfunCheckPtState(TempPort);
                AvKapiFsmFunPlugTxFsm(TempPort);
                break;
            #if AvEnableCcaFeature
            case CcaTx:
                OldState = *TempPort->content.is_PlugTxFsm;
                KfunCheckPtState(TempPort);
                AvKapiFsmFunPlugTxFsm(TempPort);
                ReportPlugTxFsm(TempPort, OldState);
                OldState = *TempPort->content.is_CcaTxFsm;
                KfunCheckCcaTxState(TempPort);
                AvKapiFsmFunCcaTxFsm(TempPort);
                ReportCcaTxFsm(TempPort, OldState);
                break;
            case CcaRx:
                OldState = *TempPort->content.is_PlugRxFsm;
                KfunCheckPrState(TempPort);
                AvKapiFsmFunPlugRxFsm(TempPort);
                ReportPlugRxFsm(TempPort, OldState);
                OldState = *TempPort->content.is_ReceiverFsm;
                KfunCheckRxState(TempPort);
                AvKapiFsmFunReceiverFsm(TempPort);
                ReportReceiverFsm(TempPort, OldState);
                OldState = *TempPort->content.is_HdcpFsm;
                KfunCheckHdcpState(TempPort);
                AvKapiFsmFunHdcpFsm(TempPort);
                ReportHdcpFsm(TempPort, OldState);
                OldState = *TempPort->content.is_CcaRxFsm;
                KfunCheckCcaRxState(TempPort);
                AvKapiFsmFunCcaRxFsm(TempPort);
                ReportCcaRxFsm(TempPort, OldState);
                break;
            #endif /* AvEnableCcaFeature */
            case AnalogTx:
            case LogicVideoTx:
                KfunCheckLogicVideoTx(TempPort);
                break;
            case LogicAudioTx:
                KfunCheckLogicAudioTx(TempPort);
                break;
            case VideoScaler:
                KfunCheckVideoScaler(TempPort);
                break;
            case VideoColor:
                KfunCheckVideoColor(TempPort);
                break;
            #if AvEnableInternalVideoGen
            case VideoGen:
                KfunCheckVideoGen(TempPort);
                break;
            #endif
            #if AvEnableInternalAudioGen
            case AudioGen:
                KfunCheckAudioGen(TempPort);
                break;
            #endif
            #if AvEnableInternalClockGen
            case ClockGen:
                KfunCheckClockGen(TempPort);
                break;
            #endif
            #if GSV5K1
            case VideoDsc:
                KfunCheckVideoDsc(TempPort);
                break;
            #endif
        }
        if(!TempPort->next)
            break;
        else
            TempPort = (AvPort*)TempPort->next;
    }

    return AvOk;
}

/**
 * @brief  Change the routing of ports
 * @return none
 * @note
 */
kapi AvRet AvApiConnectPort(pin AvPort *FromPort, pout AvPort *ToPort, AvConnectType type)
{
    AvPort *TempPort = NULL;

    g_u8ChipDev = FromPort->device->index;    // add by wf8421 20230113
    g_u8GsvChipType[FromPort->device->index] = FromPort->device->type;
    /* Step 1. Assign Video Relation */
    if((type == AvConnectVideo) || (type == AvConnectAV))
    {
        switch(FromPort->type)
        {
            case HdmiRx:
            case AnalogRx:
            case HdmiTx:
            case AnalogTx:
            case CcaRx:
            case CcaTx:
            case VideoScaler:
            case VideoColor:
            case VideoGen:
            case VideoDsc:
                if(FromPort->type == VideoDsc)
                {
                    FromPort->content.dsc->Enable = 1;
                }
            case DviRx:
            case LogicVideoRx:
            case DviTx:
            case LogicVideoTx:
                ClearVideoFromPort(ToPort);
                /* To Port has already been assigned, the new added one is placed to be front */
                if((FromPort->content.RouteVideoToPort) &&
                   (FromPort->content.RouteVideoToPort != (struct AvPort*)ToPort))
                    ToPort->content.RouteVideoNextSameLevelPort = FromPort->content.RouteVideoToPort;
                FromPort->content.RouteVideoToPort = (struct AvPort*)(ToPort);
                ToPort->content.RouteVideoFromPort = (struct AvPort*)FromPort;
                if((ToPort->type == HdmiTx) || (ToPort->type == CcaTx))
                {
                    ToPort->content.video->AvailableVideoPackets = 0;
                    /* ToPort->content.tx->Hpd = AV_HPD_FORCE_LOW; */
                    AvHandleEvent(FromPort, AvEventUpStreamConnectNewDownStream, NULL, NULL);
                }
                break;
//            case LogicAudioRx:
//            case LogicAudioTx:
            default:
                break;

        }
    }
    /* Step 2. Assign Audio Relation */
    if((type == AvConnectAudio) || (type == AvConnectAV))
    {
        switch(FromPort->type)
        {
            case HdmiRx:
            case HdmiTx:
            case LogicAudioRx:
            case LogicAudioTx:
            case CcaRx:
            case CcaTx:
            case AnalogTx:
            case AnalogRx:
            #if AvEnableInternalAudioGen
            case AudioGen:
            #endif
                ClearAudioFromPort(ToPort);
                /* To Port has already been assigned, the new added one is placed to be front */
                if((FromPort->content.RouteAudioToPort) &&
                   (FromPort->content.RouteAudioToPort != (struct AvPort*)ToPort))
                    ToPort->content.RouteAudioNextSameLevelPort = FromPort->content.RouteAudioToPort;
                FromPort->content.RouteAudioToPort = (struct AvPort*)ToPort;
                ToPort->content.RouteAudioFromPort = (struct AvPort*)FromPort;
                if((ToPort->type == HdmiTx) || (ToPort->type == CcaTx))
                {
                    ToPort->content.audio->AvailableAudioPackets = 0;
                }
                break;
//            case DviTx:
//            case LogicVideoTx:
//            case DviRx:
//            case LogicVideoRx:
            default:
                break;

        }
    }
    /* Step 3. Hdcp Update Chain */
    if((type == AvConnectVideo) || (type == AvConnectAV))
    {
        switch(FromPort->type)
        {
            case HdmiRx:
            case HdmiTx:
            case AnalogTx:
            case AnalogRx:
            case VideoScaler:
            case VideoDsc:
            case VideoColor:
            case VideoGen:
            case CcaRx:
            case CcaTx:
            case DviTx:
            case LogicVideoTx:
            case DviRx:
            case LogicVideoRx:
                /* by default, the source port should be connected, right after the port  */
                /* is connected, its HdcpSource will be assigned. But if the sink port    */
                /* is placed in advance, its HdcpSource is null, just look into the chain */
                /* to find it. */
                if(FromPort->content.HdcpSource)
                {
                    TempPort = (AvPort*)(FromPort->content.HdcpSource);
                    while(TempPort->content.HdcpSource)
                    {
                        TempPort = (AvPort*)(TempPort->content.HdcpSource);
                    }
                    ToPort->content.HdcpSource = (struct AvPort*)TempPort;
                }
                else
                   ToPort->content.HdcpSource = FromPort->content.HdcpSource;
                break;
            case LogicAudioRx:
            case LogicAudioTx:
                FromPort->content.HdcpSource = NULL;
                break;
        }
    }

    /* Step 4. Uapi Connect */
    #if AvMatrix4x4Mode
    if(FromPort->device->index == ToPort->device->index)
    #endif
    	AvUapiConnectPort(FromPort, ToPort, type);

    return AvOk;
}

kapi AvRet AvApiPortMemAllocate(pin AvPort *port)
{
    /* Layer 0. Port Structure */
    port->core.DpllCore = -1;
    port->core.HdmiCore = -1;
    port->core.OsdCore  = -1;
    port->core.VspCore  = -1;
    /* Layer 1. AvPort */
    port->content.ID = NULL;
    port->content.is_PlugRxFsm = NULL;
    port->content.is_active_PlugRxFsm = NULL;
    port->content.is_ReceiverFsm = NULL;
    port->content.is_active_ReceiverFsm = NULL;
    port->content.is_HdcpFsm = NULL;
    port->content.is_active_HdcpFsm = NULL;
    port->content.is_PlugTxFsm = NULL;
    port->content.is_active_PlugTxFsm = NULL;
    port->content.is_TxRoutingFsm = NULL;
    port->content.is_active_TxRoutingFsm = NULL;
#if AvEnableCecFeature /* CEC Related */
    port->content.is_CecFsm = NULL;
    port->content.is_active_CecFsm = NULL;
    port->content.cec = NULL;
#endif /* CEC Related */
#if AvEnableCcaFeature /* CCA Related */
    port->content.is_CcaTxFsm = NULL;
    port->content.is_active_CcaTxFsm = NULL;
    port->content.is_CcaRxFsm = NULL;
    port->content.is_active_CcaRxFsm = NULL;
    port->content.ccatx = NULL;
    port->content.ccarx = NULL;
#endif /* CCA Related */
    port->content.rx = NULL; /* Rx FSM vars */
    port->content.tx = NULL; /* Tx FSM vars */
    port->content.hdcp = NULL; /* Hdcp vars */
    port->content.hdcptx = NULL; /* Hdcp vars */
    port->content.video = NULL;
    port->content.audio = NULL;
    port->content.dsc = NULL;
    port->content.scaler = NULL;
    port->content.color = NULL;
#if AvEnableInternalVideoGen
    port->content.vg = NULL;
#endif
#if AvEnableInternalAudioGen
    port->content.ag = NULL;
#endif
#if AvEnableInternalClockGen
    port->content.cg = NULL;
#endif
    port->content.HdcpSource = NULL;
    port->content.HdcpNextSinkPort = NULL;
    port->content.RouteVideoFromPort = NULL;
    port->content.RouteVideoNextSameLevelPort = NULL;
    port->content.RouteVideoToPort = NULL;
    port->content.RouteAudioFromPort = NULL;
    port->content.RouteAudioNextSameLevelPort = NULL;
    port->content.RouteAudioToPort = NULL;

    /* Layer 2. AvContent Details */
    switch(port->type)
    {
        case HdmiRx:
        {
            AvUapiAllocateMemory(sizeof(uint16),    (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_PlugRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_PlugRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_HdcpFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_HdcpFsm));
            AvUapiAllocateMemory(sizeof(RxVars),    (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(HdcpRx),    (uint32 *)&(port->content.hdcp));
            AvUapiAllocateMemory(sizeof(AvVideo),   (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),   (uint32 *)&(port->content.audio));
            break;
        }
        case DviRx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_PlugRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_PlugRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(RxVars),   (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            break;
        }
        case AnalogRx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(RxVars),   (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            break;
        }
        case LogicVideoRx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(RxVars),   (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
#if AvEnableVideoLogicBus
            AvUapiAllocateMemory(sizeof(AvLogicVideo),(uint32 *)&(port->content.lvrx));
#endif
            break;
        }
        case LogicAudioRx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            break;
        }
        case HdmiTx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_PlugTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_PlugTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_HdcpFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_HdcpFsm));
#if AvEnableCecFeature /* CEC Related */
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_CecFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_CecFsm));
            AvUapiAllocateMemory(sizeof(AvCec),    (uint32 *)&(port->content.cec));
#endif /* CEC Related */
            AvUapiAllocateMemory(sizeof(TxVars),   (uint32 *)&(port->content.tx));
            AvUapiAllocateMemory(sizeof(HdcpTx),   (uint32 *)&(port->content.hdcptx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            /* av_platf_enter_internal_HdcpFsm(port); */
            break;
        }
        case DviTx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_PlugTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_PlugTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(TxVars),   (uint32 *)&(port->content.tx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            break;
        }
        case AnalogTx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(TxVars),   (uint32 *)&(port->content.tx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            break;
        }
        case VideoDsc:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(AvDsc),    (uint32 *)&(port->content.dsc));
            break;
        }
        case VideoScaler:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(AvScaler), (uint32 *)&(port->content.scaler));
            break;
        }
        case VideoColor:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(AvColor),  (uint32 *)&(port->content.color));
            break;
        }
#if AvEnableInternalVideoGen
        case VideoGen:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(RxVars),   (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvVideoGen),(uint32 *)&(port->content.vg));
            break;
        }
#endif

#if AvEnableInternalAudioGen
        case AudioGen:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(RxVars),   (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            AvUapiAllocateMemory(sizeof(AvAudioGen),(uint32 *)&(port->content.ag));
            break;
        }
#endif
#if AvEnableInternalClockGen
        case ClockGen:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(AvClockGen),(uint32 *)&(port->content.cg));
            break;
        }
#endif

        case LogicVideoTx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(TxVars),   (uint32 *)&(port->content.tx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
#if AvEnableVideoLogicBus
            AvUapiAllocateMemory(sizeof(AvLogicVideo),(uint32 *)&(port->content.lvtx));
#endif
            break;
        }
        case LogicAudioTx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            break;
        }
        #if AvEnableCcaFeature
        case CcaTx:
        {
            AvUapiAllocateMemory(sizeof(uint16),   (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_PlugTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_PlugTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_TxRoutingFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_HdcpFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_HdcpFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_CcaTxFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_CcaTxFsm));
    #if AvEnableCecFeature /* CEC Related */
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_CecFsm));
            AvUapiAllocateMemory(sizeof(uint8),    (uint32 *)&(port->content.is_active_CecFsm));
            AvUapiAllocateMemory(sizeof(AvCec),    (uint32 *)&(port->content.cec));
    #endif /* CEC Related */
            AvUapiAllocateMemory(sizeof(TxVars),   (uint32 *)&(port->content.tx));
            AvUapiAllocateMemory(sizeof(HdcpTx),   (uint32 *)&(port->content.hdcptx));
            AvUapiAllocateMemory(sizeof(AvVideo),  (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),  (uint32 *)&(port->content.audio));
            AvUapiAllocateMemory(sizeof(AvCcaTx),   (uint32 *)&(port->content.ccatx));
            break;
        }
        case CcaRx:
        {
            AvUapiAllocateMemory(sizeof(uint16),    (uint32 *)&(port->content.ID));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_PlugRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_PlugRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_ReceiverFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_HdcpFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_HdcpFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_CcaRxFsm));
            AvUapiAllocateMemory(sizeof(uint8),     (uint32 *)&(port->content.is_active_CcaRxFsm));
            AvUapiAllocateMemory(sizeof(RxVars),    (uint32 *)&(port->content.rx));
            AvUapiAllocateMemory(sizeof(HdcpRx),    (uint32 *)&(port->content.hdcp));
            AvUapiAllocateMemory(sizeof(AvVideo),   (uint32 *)&(port->content.video));
            AvUapiAllocateMemory(sizeof(AvAudio),   (uint32 *)&(port->content.audio));
            AvUapiAllocateMemory(sizeof(AvCcaRx),   (uint32 *)&(port->content.ccarx));
            break;
        }
        #endif /* AvEnableCcaFeature */
    }

    return AvOk;

}

void ClearVideoFromPort(pin AvPort *port)
{
    /* clarify the routing in AvPort structure and AvDevice structure */
    AvPort *AbandonRxPort = NULL;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    /* 1. Find abandoned rx port and tx port */
    AbandonRxPort = port;
    while(AbandonRxPort->content.RouteVideoFromPort != NULL)
    {
        AbandonRxPort = (AvPort*)AbandonRxPort->content.RouteVideoFromPort;
        /* 1.1.1 Remove video ToPort in FromPort */
        if(AbandonRxPort->content.RouteVideoToPort == (struct AvPort*)port)
            AbandonRxPort->content.RouteVideoToPort = port->content.RouteVideoNextSameLevelPort;
        if(AbandonRxPort->content.RouteVideoNextSameLevelPort == (struct AvPort*)port)
            AbandonRxPort->content.RouteVideoNextSameLevelPort = port->content.RouteVideoNextSameLevelPort;
    }
    /* 1.1.2 Remove video ToPort in the chain of FromPort */
    while(((port->device->type != Gsv5k1)&&(port->device->type != Gsv2k11))&&(KfunFindVideoNextNode(AbandonRxPort, &PrevPort, &CurrentPort) == AvOk))
    {
        /* 1.2 Found ToPort in previous chain connection, remove it */
        if(CurrentPort->content.RouteVideoToPort == (struct AvPort*)port)
            CurrentPort->content.RouteVideoToPort = port->content.RouteVideoNextSameLevelPort;
        if(CurrentPort->content.RouteVideoNextSameLevelPort == (struct AvPort*)port)
            CurrentPort->content.RouteVideoNextSameLevelPort = port->content.RouteVideoNextSameLevelPort;
        /* 1.2.1 Next Loop */
        PrevPort = CurrentPort;
        CurrentPort = NULL;
    }
#if (GSV2K11||GSV5K1)
    if((port->device->type == Gsv5k1)||(port->device->type == Gsv2k11))
    {
        AbandonRxPort = port->device->port; /* Get 1st Port of the device */
        while((AbandonRxPort != NULL) && (AbandonRxPort->device->index == port->device->index))
        {
            /* 1.2 Remove video ToPort in FromPort */
            if(AbandonRxPort->content.RouteVideoToPort == (struct AvPort*)port)
                AbandonRxPort->content.RouteVideoToPort = port->content.RouteVideoNextSameLevelPort;
            if(AbandonRxPort->content.RouteVideoNextSameLevelPort == (struct AvPort*)port)
                AbandonRxPort->content.RouteVideoNextSameLevelPort = port->content.RouteVideoNextSameLevelPort;
            AbandonRxPort = (AvPort*)(AbandonRxPort->next);
        }
    }
#endif
    /* 2. clear tx's pointers, in case of already cleared by kapi layer, add protection */
    port->content.RouteVideoFromPort = NULL;
    port->content.RouteVideoNextSameLevelPort = NULL;
}

void ClearAudioFromPort(pin AvPort *port)
{
    /* clarify the routing in AvPort structure and AvDevice structure */
    AvPort *AbandonRxPort = NULL;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    /* 1. Find abandoned rx port and tx port */
    AbandonRxPort = port;
    while(AbandonRxPort->content.RouteAudioFromPort != NULL)
    {
        AbandonRxPort = (AvPort*)AbandonRxPort->content.RouteAudioFromPort;
        /* 1.1.1 Remove audio ToPort in FromPort */
        if(AbandonRxPort->content.RouteAudioToPort == (struct AvPort*)port)
            AbandonRxPort->content.RouteAudioToPort = port->content.RouteAudioNextSameLevelPort;
        if(AbandonRxPort->content.RouteAudioNextSameLevelPort == (struct AvPort*)port)
            AbandonRxPort->content.RouteAudioNextSameLevelPort = port->content.RouteAudioNextSameLevelPort;
    }
    /* 1.1.2 Remove audio ToPort in the chain of FromPort */
    while(((port->device->type != Gsv5k1)&&(port->device->type != Gsv2k11))&&(KfunFindAudioNextNode(AbandonRxPort, &PrevPort, &CurrentPort) == AvOk))
    {
        /* 1.2 Found ToPort in previous chain connection, remove it */
        if(CurrentPort->content.RouteAudioToPort == (struct AvPort*)port)
            CurrentPort->content.RouteAudioToPort = port->content.RouteAudioNextSameLevelPort;
        if(CurrentPort->content.RouteAudioNextSameLevelPort == (struct AvPort*)port)
            CurrentPort->content.RouteAudioNextSameLevelPort = port->content.RouteAudioNextSameLevelPort;
        /* 1.2.1 Next Loop */
        PrevPort = CurrentPort;
        CurrentPort = NULL;
    }
#if (GSV2K11||GSV5K1)
    if((port->device->type == Gsv5k1)||(port->device->type == Gsv2k11))
    {
        AbandonRxPort = port->device->port; /* Get 1st Port of the device */
        while((AbandonRxPort != NULL) && (AbandonRxPort->device->index == port->device->index))
        {
            /* 1.2 Remove audio ToPort in FromPort */
            if(AbandonRxPort->content.RouteAudioToPort == (struct AvPort*)port)
                AbandonRxPort->content.RouteAudioToPort = port->content.RouteAudioNextSameLevelPort;
            if(AbandonRxPort->content.RouteAudioNextSameLevelPort == (struct AvPort*)port)
                AbandonRxPort->content.RouteAudioNextSameLevelPort = port->content.RouteAudioNextSameLevelPort;
            AbandonRxPort = (AvPort*)(AbandonRxPort->next);
        }
    }
#endif
    /* 2. clear tx's pointers, in case of already cleared by kapi layer, add protection */
    port->content.RouteAudioFromPort = NULL;
    port->content.RouteAudioNextSameLevelPort = NULL;
}

/**
 * @brief  add audio/video port
 * @return none
 * @note this function is used for initializing port FSMs.
 */
kapi AvRet AvApiPortStart(void)
{
    AvPort *TempPort = FirstPort;
    while(TempPort)
    {
        g_u8ChipDev = TempPort->device->index;    // add by wf8421 20230113
        g_u8GsvChipType[TempPort->device->index] = TempPort->device->type;
        AvApiInitFsm(TempPort);
        TempPort = (AvPort*)TempPort->next;
    }
    return AvOk;
}

#if AvEnableCecFeature /* CEC Related */
kapi AvRet AvKapiCecSendMessage(AvPort *port)
{
    return KfunCecSendMessage(port);
}

kapi AvRet AvKapiCecSetPhysicalAddr(AvPort *port)
{
    return KfunCecSetPhysicalAddr(port);
}

kapi AvRet AvKapiCecSetLogicalAddr(AvPort *port)
{
    return KfunCecSetLogicalAddr(port);
}

kapi AvRet AvKapiArcEnable(AvPort *port, uint8 value)
{
    return AvUapiTxArcEnable(port, value);
}

#endif /* CEC Related */

#endif

