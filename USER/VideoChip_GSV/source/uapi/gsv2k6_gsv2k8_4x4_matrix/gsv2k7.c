#include "gsv2k7.h"
#include "Gsv2K7_lib.h"
#include "hal.h"
#include "uapi.h"
#include "uapi_function_mapper.h"
#include "gsv2k7_tables.h"
typedef struct
{
    /* The Rx Port Core1 is connected */
    AvPort *RxCore1Occupied;
    /* The Rx Port Core2 is connected */
    AvPort *RxCore2Occupied;
    VideoInterrupt Rx1VideoInt;
    VideoInterrupt Rx2VideoInt;
    AudioInterrupt Rx1AudioInt;
    AudioInterrupt Rx2AudioInt;
    HdcpInterrupt  Rx1HdcpInt;
    HdcpInterrupt  Rx2HdcpInt;
    AvPort *Cp1CoreOccupied;
    AvPort *Cp2CoreOccupied;
#if AvEdidStoredInRam
    uint8  TxBEdidRam[256];
#endif
#if AvAllowHpdLowEdidRead
    uint8 ScdcScramble[4];
    uint8 ScdcClkRatio[4];
    uint16 ScdcTuneRound[4];
    uint8 ScdcFsm[4];
    uint8 ScdcNewEdid;
#endif
}   Resource;

static Resource InternalResource_2K7;

#define FindCore1(port)        (InternalResource_2K7.RxCore1Occupied)
#define FindCore2(port)        (InternalResource_2K7.RxCore2Occupied)
#define Rx1VideoInt(port)      (InternalResource_2K7.Rx1VideoInt)
#define Rx2VideoInt(port)      (InternalResource_2K7.Rx2VideoInt)
#define Rx1AudioInt(port)      (InternalResource_2K7.Rx1AudioInt)
#define Rx2AudioInt(port)      (InternalResource_2K7.Rx2AudioInt)
#define Rx1HdcpInt(port)       (InternalResource_2K7.Rx1HdcpInt)
#define Rx2HdcpInt(port)       (InternalResource_2K7.Rx2HdcpInt)
#define FindCp1Mode(port)      (InternalResource_2K7.Cp1CoreOccupied)
#define FindCp2Mode(port)      (InternalResource_2K7.Cp2CoreOccupied)
#if AvEdidStoredInRam
#define TxBEdidRam(port)       (InternalResource_2K7.TxBEdidRam)
#endif
#if AvAllowHpdLowEdidRead
#define RxScdcScramble(port)   (InternalResource_2K7.ScdcScramble)
#define RxScdcClkRatio(port)   (InternalResource_2K7.ScdcClkRatio)
#define RxScdcTuneRound(port)  (InternalResource_2K7.ScdcTuneRound)
#define RxScdcFsm(port)        (InternalResource_2K7.ScdcFsm)
#define RxScdcNewEdid(port)    (InternalResource_2K7.ScdcNewEdid)
#endif


#define Gsv2k7SetVideoPacketFlag(BitName) \
        if((value == 0) && ((port->content.video->AvailableVideoPackets & BitName) != 0))\
            port->content.rx->ChangedVideoPackets = port->content.rx->ChangedVideoPackets | BitName;\
        else if((value != 0) && ((port->content.video->AvailableVideoPackets & BitName) == 0))\
            port->content.rx->ChangedVideoPackets = port->content.rx->ChangedVideoPackets | BitName;\
        if(value == 0)\
            port->content.video->AvailableVideoPackets = port->content.video->AvailableVideoPackets & (~BitName);\
        else\
            port->content.video->AvailableVideoPackets = port->content.video->AvailableVideoPackets | BitName;

#define Gsv2k7SetAudioPacketFlag(BitName) \
        if((value == 0) && ((port->content.audio->AvailableAudioPackets & BitName) != 0))\
            port->content.rx->ChangedAudioPackets = port->content.rx->ChangedAudioPackets | BitName;\
        else if((value != 0) && ((port->content.audio->AvailableAudioPackets & BitName) == 0))\
            port->content.rx->ChangedAudioPackets = port->content.rx->ChangedAudioPackets | BitName;\
        if(value == 0)\
            port->content.audio->AvailableAudioPackets = port->content.audio->AvailableAudioPackets & (~BitName);\
        else\
            port->content.audio->AvailableAudioPackets = port->content.audio->AvailableAudioPackets | BitName;

#define gsv2k7IntptSt(func, value) \
        if(port->core.HdmiCore == 0) \
            GSV2K7_INT_get_RX1_##func##_INT_ST(port,         &value); \
        else if(port->core.HdmiCore == 1) \
            GSV2K7_INT_get_RX2_##func##_INT_ST(port,         &value);

#define gsv2k7IntptRaw(func) \
        if(port->core.HdmiCore == 0) \
            GSV2K7_INT_get_RX1_##func##_RAW_ST(port,         &Value); \
        else if(port->core.HdmiCore == 1) \
            GSV2K7_INT_get_RX2_##func##_RAW_ST(port,         &Value);

#define gsv2k7IntptClr(func) \
        if(port->core.HdmiCore == 0) \
            GSV2K7_INT_set_RX1_##func##_CLEAR(port,         1); \
        else if(port->core.HdmiCore == 1) \
            GSV2K7_INT_set_RX2_##func##_CLEAR(port,         1);

/* local functions */
void Gsv2k7GetRx5VStatus(pin AvPort *port);
void Gsv2k7EnableRxHpa(pin AvPort *port);
void Gsv2k7DisableRxHpa(pin AvPort *port);
void Gsv2k7ToggleTxHpd(pin AvPort *port);
void Gsv2k7ToggleTmdsOut(AvPort *port, uint8 Enable);
void Gsv2k7RxManualEQUpdate(AvPort *port, uint8 DefaultEQEnable);
void Gsv2k7SetTxHdcpVersion(pin AvPort *port, HdcpTxSupportType HdmiStyle, uint8 ForceUpdate);
void Gsv2k7UpdateRxCdrBandWidth(pin AvPort *port);
uint8 Gsv2k7TxScdcAction(pin AvPort *port, uint8 WriteEnable, uint8 DevAddr, uint8 RegAddr, uint8 Value);
uint8 Gsv2k7TxDDCError(pin AvPort *port);
void Gsv2k7ManualBksvRead(pin AvPort *port);
AvPort *Gsv2k7FindHdmiRxFront(pin AvPort *port);
void Gsv2k7ResetTxPhy(pin AvPort *port);
void Gsv2k7ResetTxFifo(pin AvPort *port);
AvVideoCs Gsv2k7ColorCsMapping(AvVideoCs InputCs);
void Gsv2k7TxPllUnlockClear(AvPort *port);
uint8 Gsv2k7TweakCrystalFreq(AvPort *port, uint32 value32);
void Gsv2k7MpllProtect(AvPort *port);
void Gsv2k7RpllProtect(AvPort *port);
void Gsv2k7RxScdcFsm(AvPort* port);

/**
 * @brief  device init function
 * @return AvOk if success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiInitDevice(pin AvDevice *device))
{
    //Resource *InternalResource_2K7;
    uint32 i = 0;
    uint32 k = 0;
    uint32 DefaultMapAddress;
    uint32 LaneMapAddress = 0;

    AvPort *port = (AvPort *)device->port;
    Gsv2k7Device *gsv2k7Dev = (Gsv2k7Device *)device->specific;
    DefaultMapAddress = gsv2k7Dev->DeviceAddress;

    /* reset device */
//    GSV2K7_PRIM_set_MAIN_RST(port, 0);
//    GSV2K7_PRIM_set_MAIN_RST(port, 1);
    if(GSV2K7_PRIM_set_MAIN_RST(port, 0) != AvOk)
        return AvError;
    if(GSV2K7_PRIM_set_MAIN_RST(port, 1)!= AvOk)
        return AvError;
    for(i=0; i<100; i++)
        GSV2K7_PRIM_set_MAIN_RST(port, 0);

    /* set default i2c address for internal maps */
    AvUapiOutputDebugMessage("Gsv2k7 - setting i2c addresses.");

    /* Write Init Table */
    for(k=0; Gsv2k7InitTable[k]!=0xFF; k=k+3)
        AvHalI2cWriteField8(DefaultMapAddress | Gsv2k7InitTable[k],Gsv2k7InitTable[k+1],0xFF,0,Gsv2k7InitTable[k+2]);

#if (Gsv2k7PhyBypass == 0)
    Gsv2k7MpllProtect(port);
#endif

    //;RX inital added for adaptive EQ
    for(i=0;i<=3;i++)
    {
        switch(i)
        {
            case 0:
                LaneMapAddress  = gsv2k7Dev_RxALane0MapAddress;
                break;
            case 1:
                LaneMapAddress  = gsv2k7Dev_RxBLane0MapAddress;
                break;
            case 2:
                LaneMapAddress  = gsv2k7Dev_RxCLane0MapAddress;
                break;
            case 3:
                LaneMapAddress  = gsv2k7Dev_RxDLane0MapAddress;
                break;
        }
        LaneMapAddress = DefaultMapAddress | LaneMapAddress;
        for(k=0; Gsv2k7EqTable[k]!=0xFF; k=k+2)
            AvHalI2cWriteField8(LaneMapAddress,Gsv2k7EqTable[k],0xFF,0,Gsv2k7EqTable[k+1]);
    }

    //AvUapiAllocateMemory(sizeof(Resource), (uint32 *)&InternalResource_2K7);
    device->extension = &InternalResource_2K7;
    FindCore1(port) = NULL;
    FindCore2(port) = NULL;
    FindCp1Mode(port) = NULL;
    FindCp2Mode(port) = NULL;
#if AvAllowHpdLowEdidRead
    for(i=0;i<4;i++)
    {
        RxScdcScramble(port)[i]  = 0;
        RxScdcClkRatio(port)[i]  = 0;
        RxScdcTuneRound(port)[i] = 0;

    }
#endif
#if Gsv2k7MuxMode
    GSV2K7_PRIM_set_RX1_TMDS_BYPASS_EN(port, 1);
    GSV2K7_PRIM_set_RX2_TMDS_BYPASS_EN(port, 1);
    GSV2K7_PRIM_set_TX1_TMDS_BYPASS_EN(port, 1);
    GSV2K7_PRIM_set_TX2_TMDS_BYPASS_EN(port, 1);
#endif

    return AvOk;
}

/**
 * @brief  init rx port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxPortInit(pin AvPort *port))
{
    AvRet ret = AvOk;

    if(port->index <= 3) /* Hdmi Rx */
        Gsv2k7DisableRxHpa(port);
    return ret;
}

/**
 * @brief  init tx port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxPortInit(pin AvPort *port))
{
    AvRet ret = AvOk;
    if(port->index == 5)
    {
        port->core.HdmiCore = 1;
    }

#if AvEnableCecFeature /* CEC Related */
    /* Gsv2k7 has 4 input ports */
    port->content.cec->InputCount = 4;
#endif

    return ret;
}

/**
 * @brief  enable tx port core
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxEnableCore(pin AvPort *port))
{
#if AvEnableCecFeature
    /* Setting For ARC audio */
    GSV2K7_PRIM_set_TXA_PWR_DN(port, 0);
#endif
    GSV2K7_PRIM_set_TXB_PWR_DN(port, 0);

    return AvOk;
}

/**
 * @brief  disable tx port core
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxDisableCore(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    /* Disable HDCP */
    AvUapiTxDecryptSink(port);
    /* Set HDCP Version Before Reset Tx */
    Gsv2k7SetTxHdcpVersion(port, port->content.hdcptx->HdmiStyle, 0);
    /* Tx Phy Reset */
    Gsv2k7ResetTxPhy(port);
    /* Reset Tx Phy SCDC state */
    AvUapiTxSetFeatureSupport(port);
    #if (Gsv2k7MuxMode == 0)
    uint32 MainMapAddress = GSV2K7_TXDIG_MAP_ADDR(port);
    /* keep Hdmi mode */
    AvUapiTxSetHdmiModeSupport(port);
    AvHalI2cWriteField8(MainMapAddress,0x53,0xFF,0,0x04);//; Tx Color Depth = 8 bits
    AvHalI2cWriteField8(MainMapAddress,0x23,0xFF,0,0x80);//; Manual Pixel Repetition x1
    AvHalI2cWriteField8(MainMapAddress,0x20,0xFF,0,0x00);//; Tx input set to default RGB
    AvHalI2cWriteField8(MainMapAddress,0x21,0xFF,0,0x00);//; Tx output set to default RGB
    AvHalI2cWriteField8(MainMapAddress,0x00,0xFF,0,0x00);//; Mclk Tx 128*fs
    AvHalI2cWriteField8(MainMapAddress,0x50,0xFF,0,0x00);//; Disable AudioInfo/AviInfo packet
    AvHalI2cWriteField8(MainMapAddress,0x51,0xFF,0,0x00);//; Disable GC/AudioSample/N_CTS packet
    AvHalI2cWriteField8(MainMapAddress,0x2B,0xFF,0,0x00);//; Disable Tx CSC
    AvHalI2cWriteField8(MainMapAddress,0x1A,0xFF,0,0xB0);//; Audio Mute and default Layout 0
    #endif // (Gsv2k7MuxMode == 0)
#endif

    /* CEC Recover */
#if AvEnableCecFeature /* CEC Related */
    if(port->content.cec->CecEnable == 1)
    {
        Gsv2k7AvUapiTxCecInit(port);
        Gsv2k7AvUapiTxCecSetLogicalAddr(port);
    }
#endif

#if (Gsv2k7MuxMode == 0)
    /* Packet information needs to be resent */
    port->content.video->InCs  = AV_CS_RGB;
    port->content.video->OutCs = AV_CS_RGB;
    port->content.video->Y     = AV_Y2Y1Y0_RGB;
    port->content.video->AvailableVideoPackets = 0;
    port->content.audio->AvailableAudioPackets = 0;
    port->content.audio->NValue = 0;
#endif

    return ret;
}

void Gsv2k7ResetTxPhy(pin AvPort *port)
{
    uint32 PhyMapAddress = GSV2K7_TXPHY_MAP_ADDR(port);

    /* Disable Tx TMDS Clock Driver, only enable after video is fully stable */
    Gsv2k7ToggleTmdsOut(port, 0);
    /* HDCP 2.2 Stop on Error */
    GSV2K7_TX2P2_set_TX_HDCP2P2_OP_WHEN_ERROR(port, 1);
    /* Disable Tx SCDC and RR Feature as default */
    //AvHalI2cWriteField8(PhyMapAddress,0x01,0xFF,0,0x00);
    AvHalI2cWriteField8(PhyMapAddress,0x02,0xFF,0,0x00);
    /* Disable Sink SCDC and RR Analysis done */
    AvHalI2cWriteField8(PhyMapAddress,0x03,0xFF,0,0x00);

#if AvEnableHdcp2p2Feature
    /* HDCP 2.2 revocation_check_pass=1 */
    GSV2K7_TX2P2_set_REVOCATION_CHECK_PASS(port, 1);
    /* HDCP 2.2 Stop on reset */
    GSV2K7_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
    /* default DDC Speed */
    GSV2K7_TX2P2_set_HDCP2P2_MASTER_CLK_DIV(port, 0x82);
    /* HDCP 2.2 i2c_clear_int_rxid_ready=1 */
    Gsv2k7AvUapiTxClearRxidReady(port);
#endif

    return;
}

void Gsv2k7ResetTxFifo(pin AvPort *port)
{
#if (Gsv2k7PhyBypass == 0)
    /* reset dc fifo */
    GSV2K7_TXDIG_set_DCFIFO_RESET(port, 1);
    GSV2K7_TXDIG_set_DCFIFO_RESET(port, 0);
    /* reset audio fifo */
    GSV2K7_TXDIG_set_TX_AUDIOFIFO_RESET(port, 1);
    GSV2K7_TXDIG_set_TX_AUDIOFIFO_RESET(port, 0);
    /* reset phy */
    GSV2K7_TXPHY_set_TMDS_SRST_MAN(port, 0);
    GSV2K7_TXPHY_set_TMDS_SRST_MAN(port, 1);
#endif
}

/**
 * @brief  reset port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiResetPort(pin AvPort *port))
{
    AvRet ret = AvOk;

    if (port->type == HdmiRx || port->type == DviRx)
    {
        AvUapiOutputDebugMessage("--------------Rx %d Reset Port------------", port->index+1);
        Gsv2k7DisableRxHpa(port);
        /* EQ reset to default state */
        Gsv2k7RxManualEQUpdate(port, 1);
    }
    else if (port->type == HdmiTx || port->type == DviTx)
    {
        switch(port->index)
        {
            case 4: /* tx port 0 */
            case 5: /* tx port 1 */
                /* Reset Tx */
                Gsv2k7AvUapiTxDisableCore(port);
                Gsv2k7AvUapiTxEnableCore(port);
                /* Reset Edid Segment Ptr */
                GSV2K7_TXPHY_set_TX_DDC_READ_EDID_RAM_PART(port, 0);
                /* Edid will be reread 15 times if failed */
                GSV2K7_TXPHY_set_TX_EDID_TRY_TIMES(port,15);
                break;
            default:
                break;
        }
    }

    return ret;
}

/**
 * @brief  enable port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiEnablePort(pin AvPort *port))
{
    return AvOk;
}

/**
 * @brief  free run rx port
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable))
{
    return AvOk;
}

/**
 * @brief  get +5v status of Rx
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGet5VStatus(pin AvPort *port))
{
    AvRet ret = AvOk;

    Gsv2k7GetRx5VStatus(port);

    return ret;
}

/**
 * @brief  get rx port status
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetStatus(pin AvPort *port))
{
    uint8 value;
    uint8 temp;
    uint32 value32;
    uint32 temp32;
    uint8 DeepColor;
    AvRet ret = AvOk;

    Gsv2k7GetRx5VStatus(port);

    if(port->content.rx->Input5V == 1)
    {
        if((port->content.rx->Hpd == 0) &&
           (port->content.rx->HpdDelayExpire <= RxHpdDelayExpireThreshold))
        {
            port->content.rx->HpdDelayExpire = port->content.rx->HpdDelayExpire + 1;
            AvUapiOutputDebugMessage("Port Rx %d: Lock Delay = %d", port->index, port->content.rx->HpdDelayExpire);
        }
    }
    else
    {
        port->content.rx->HpdDelayExpire = 0;
        port->content.rx->IsInputStable = 0;
        port->content.video->AvailableVideoPackets = 0;
        port->content.audio->AvailableAudioPackets = 0;
    }

    if((port->core.HdmiCore != -1) && (port->content.rx->Hpd == 1))
    {
        Gsv2k7AvUapiRxGetVideoLock(port);

#if (Gsv2k7PhyBypass == 0)
        /* there is a mapped core existed */
        GSV2K7_RXDIG_get_RB_RX_HDMI_MODE(port, &(port->content.rx->HdmiMode));
        GSV2K7_RXDIG_get_RB_RX_PIXEL_REPETITION(port, &(port->content.video->PixelRepeatValue));
        GSV2K7_RXDIG_get_RB_RX_DEEP_COLOR_MODE(port, &DeepColor);
#endif

        switch(port->index)
        {
            case 0:
                GSV2K7_PLL_get_RB_RXA_PLL_VCO_FREQ(port, &value32);
                GSV2K7_PLL_get_RB_RXA_PLL_SER_DIV(port, &value);
                break;
            case 1:
                GSV2K7_PLL_get_RB_RXB_PLL_VCO_FREQ(port, &value32);
                GSV2K7_PLL_get_RB_RXB_PLL_SER_DIV(port, &value);
                break;
            case 2:
                GSV2K7_PLL2_get_RB_RXC_PLL_VCO_FREQ(port, &value32);
                GSV2K7_PLL2_get_RB_RXC_PLL_SER_DIV(port, &value);
                break;
            case 3:
                GSV2K7_PLL2_get_RB_RXD_PLL_VCO_FREQ(port, &value32);
                GSV2K7_PLL2_get_RB_RXD_PLL_SER_DIV(port, &value);
                break;
            default:
                value32 = 0;
        }
        temp = (1<<value) * 5;
        value32 = value32/temp;

        if(port->content.video->info.LaneFreq != value32)
        {
            /* calculate diff */
            if(port->content.video->info.LaneFreq > value32)
                temp32 = port->content.video->info.LaneFreq - value32;
            else
                temp32 = value32 - port->content.video->info.LaneFreq;
            port->content.video->info.LaneFreq = value32;
            /* only reset when diff is too big */
            if((temp32 > 5) || (port->content.video->info.LaneFreq <= 10))
            {
                port->content.rx->Lock.DeRegenLock = 0;
                port->content.rx->Lock.VSyncLock = 0;
                port->content.rx->Lock.PllLock = 0;
                port->content.rx->Lock.EqLock = 0;
                port->content.video->info.TmdsFreq = 0;
            }
        }

        /* Input Lock Stage */
        if(port->content.rx->Lock.PllLock == 1)
        {
            /* remove deep color */
            if(DeepColor == 1) /* 10-bit */
                value32 = (value32<<2)/5;
            else if(DeepColor == 2) /* 12-bit */
                value32 = (value32<<1)/3;
            port->content.video->info.TmdsFreq = value32;

            if((port->content.rx->Lock.DeRegenLock == 1) &&
               (port->content.rx->Lock.EqLock == 1) &&
               (port->content.rx->Lock.VSyncLock == 1))
            {
#if (Gsv2k7PhyBypass == 0)
                if(port->content.rx->IsInputStable == 0)
                {
                    port->content.video->AvailableVideoPackets = 0;
                    port->content.video->timing.Vic = 0;
                    port->content.audio->AvailableAudioPackets = 0;
                    /* when EQ locked to start audio detection,
                       avoid garbage audio packets detection */
                    GSV2K7_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 1);
                    GSV2K7_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 0);
                }
#endif
                port->content.rx->IsInputStable = 1;
#if (AvAllowHpdLowEdidRead == 1)
                AvHalI2cReadField8(GSV2K7_RXDIG_MAP_ADDR(port), 0xE6, 0x06, 0, &value);
                RxScdcScramble(port)[port->index]  = (value>>1) & 0x01;
                RxScdcClkRatio(port)[port->index]  = (value>>2) & 0x01;
#endif
            }
            else
            {
                port->content.rx->IsInputStable = 0;
                Gsv2k7RxManualEQUpdate(port, 0);
            }
        }

        #if (Gsv2k7PhyBypass == 0)
        if(port->content.rx->IsInputStable == 1)
        {
            GSV2K7_RXAUD_get_ACR_PKT_DET(port, &value);
            if(value == 1)
            {
                /* only set APLL lock process after N/CTS is stable */
                GSV2K7_RXDIG_get_RX_APLL_MAN_START_CALC(port, &value);
                if(value == 1)
                {
                    GSV2K7_RXDIG_set_N_CTS_STABLE_DET_START(port, 0);
                    GSV2K7_RXDIG_set_RX_APLL_MAN_START_CALC(port, 0);
                    GSV2K7_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 1);
                    GSV2K7_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 0);
                    port->content.rx->Lock.AudioLock = 0;
                }
                /* Real Apll Lock Process*/
                else
                {
                    GSV2K7_RXAUD_get_RX_AUDIO_PLL_LOCKED(port, &port->content.rx->Lock.AudioLock);
                    if(port->content.rx->Lock.AudioLock)
                    {
                        /* Get internal actual audio mute status */
                        Gsv2k7AvUapiRxGetAudioInternalMute(port);
                        /* Possible Incorrect Garbage Audio Type Recovery */
                        AvHalI2cReadField8(GSV2K7_RXAUD_MAP_ADDR(port), 0x17, 0x1E, 0, &value);
                        if(value != 0)
                        {
                            GSV2K7_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 1);
                            GSV2K7_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 0);
                        }
                        /* Packet Type */
                        AvUapiRxGetPacketType(port);
                        /* Set DSD Audio Mode */
                        GSV2K7_RXAUD_get_MAN_TDM_ENABLE(port, &value);
                        if((port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                           (port->content.audio->AudType == AV_AUD_TYPE_DST))
                            temp = 0;
                        else
                            temp = 1;
                        if(temp != value)
                            GSV2K7_RXAUD_set_MAN_TDM_ENABLE(port, temp);
                        /* Channel Number */
                        GSV2K7_RXDIG_get_RB_RX_AUDIO_LAYOUT(port, &value);
                        if(value == 0)
                        {
                            port->content.audio->ChanNum    = 2;
                            GSV2K7_RXAUD_get_RX_AUDIO_PIN_NUM(port, &value);
                            if((port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                               (port->content.audio->AudType == AV_AUD_TYPE_DST))
                                temp = 1;
                            else
                                temp = 0;
                            if(value != temp)
                                GSV2K7_RXAUD_set_RX_AUDIO_PIN_NUM(port, temp);
                        }
                        else
                        {
                            GSV2K7_RXAUD_get_AUD_IF_PKT_DET(port, &value);
                            if(value == 1)
                            {
                                AvHalI2cReadField8(GSV2K7_RXINFO_MAP_ADDR(port),0x1D,0xFF,0,&value);
                                port->content.audio->ChanNum = (value & 0x07) + 1;
                                /* Check Audio Channel Using Logic Rather than Packet */
                                GSV2K7_RXAUD_get_NUM_CH(port, &value);
                                temp = (value-1)>>1;
                                if((port->content.audio->AudType == AV_AUD_TYPE_HBR) ||
                                   (port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                                   (port->content.audio->AudType == AV_AUD_TYPE_DST))
                                    temp = 3;
                                GSV2K7_RXAUD_get_RX_AUDIO_PIN_NUM(port, &value);
                                if(value != temp)
                                    GSV2K7_RXAUD_set_RX_AUDIO_PIN_NUM(port, temp);
                            }
                        }
                    }
                }
            }
        }
        #endif
    }
#if (Gsv2k7PhyBypass == 0)
    else
    {
        port->content.rx->IsFreeRun = 0;
        port->content.rx->IsInputStable = 0;
        port->content.rx->VideoEncrypted = 0;
        port->content.rx->Lock.AudioLock = 0;
        port->content.rx->Lock.DeRegenLock = 0;
        port->content.rx->Lock.PllLock = 0;
        port->content.rx->Lock.VSyncLock = 0;
        port->content.rx->Lock.EqLock = 0;
        port->content.rx->HdmiMode = 0;
        port->content.video->InCs = AV_CS_AUTO;
        port->content.video->OutCs = AV_CS_AUTO;
    }
#endif

#if (Gsv2k7PhyBypass == 0)
    /* SCDC FSM Update */
    if((port->content.rx->Hpd == 1) && (port->content.rx->IsInputStable == 0))
        Gsv2k7RxScdcFsm(port);

    if((port->content.rx->HdmiMode == 0) && (port->content.rx->IsInputStable))
    {
        port->content.video->Y = AV_Y2Y1Y0_RGB;
        port->content.video->InCs = AV_CS_RGB;
    }

    if((port->content.rx->HdmiMode) && (port->content.rx->IsInputStable))
    {
        AvHalI2cReadField8(GSV2K7_RXAUD_MAP_ADDR(port), 0x19, 0xFF, 0, &temp);
        /* AVI_INFO_FRAME */
        value = (temp>>6) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_AV_INFO_FRAME);
        /* AUDIO_INFO */
        value = (temp>>7) & 0x01;
        Gsv2k7SetAudioPacketFlag(AV_BIT_AUDIO_INFO_FRAME);
        AvHalI2cReadField8(GSV2K7_RXAUD_MAP_ADDR(port), 0x18, 0xFF, 0, &temp);
        /* SPD_INFO */
        value = (temp>>0) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_SPD_PACKET);
        /* MS_INFO */
        value = (temp>>1) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_MPEG_PACKET);
        /* VS_INFO */
        value = (temp>>2) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_VS_PACKET);
        /* ACP_PCKT */
        value = (temp>>3) & 0x01;
        Gsv2k7SetAudioPacketFlag(AV_BIT_ACP_PACKET);
        /* ISRC1_PCKT */
        value = (temp>>4) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_ISRC1_PACKET);
        /* ISRC2_PCKT */
        value = (temp>>5) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_ISRC2_PACKET);
        /* Gamut */
        value = (temp>>6) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_GMD_PACKET);
        /* ACR Packet */
        value = (temp>>7) & 0x01;
        Gsv2k7SetAudioPacketFlag(AV_BIT_ACR_PACKET);
        AvHalI2cReadField8(GSV2K7_RXAUD_MAP_ADDR(port), 0x17, 0xFF, 0, &temp);
        /* GC */
        value = (temp>>0) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_GC_PACKET);
        /* HDR_INFO */
        value = (temp>>6) & 0x01;
        Gsv2k7SetVideoPacketFlag(AV_BIT_HDR_PACKET);
        /* Channel Status */
        GSV2K7_RXAUD_get_CSDATA_VALID(port, &value);
        Gsv2k7SetAudioPacketFlag(AV_BIT_AUDIO_CHANNEL_STATUS);
        if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_get_RX2_AVMUTE_RAW_ST(port, &(port->content.video->Mute.AvMute));
            GSV2K7_INT_get_RX2_HDMI_MODE_RAW_ST(port, &(port->content.rx->HdmiMode));
        }
        /* Audio Information Protection */
        if((port->content.audio->AudType == AV_AUD_TYPE_UNKNOWN) ||
           (port->content.rx->Lock.AudioLock == 0))
            port->content.audio->AvailableAudioPackets = 0;
    }
#endif

    return ret;
}

/**
 * @brief  set HPA = high for corresponding port
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetHpdUp(pin AvPort *port))
{
    AvRet ret = AvOk;

    Gsv2k7EnableRxHpa(port);
    AvUapiOutputDebugMessage("---------------Rx %d Power Up------------", port->index+1);
    return ret;
}

/**
 * @brief  set HPA = low for corresponding port
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetHpdDown(pin AvPort *port))
{
    AvRet ret = AvOk;

    Gsv2k7DisableRxHpa(port);
    AvUapiOutputDebugMessage("--------------Rx %d Power Down-----------", port->index+1);

    return ret;
}

/**
 * @brief  mute tx tmds output
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxMuteTmds(pin AvPort *port, bool mute))
{
    uint8 Enable = 0;
    if(mute == 0)
        Enable = 1;
    Gsv2k7ToggleTmdsOut(port, Enable);
    AvUapiOutputDebugFsm("AvUapiTxMuteTmds");
    return AvOk;
}

/**
 * @brief  get tx port status
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxGetStatus(pin AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    uint8 LocalPllLock = 0;
    uint8 NewValue = 0;
    uint8 Freq3G4Plus = 0;
    uint32 value32 = 0;
    uint32 temp = 0;
    uint8 Packet[31];
    AvPort *FromPort = NULL;
    FromPort = (AvPort*)(Gsv2k7FindHdmiRxFront(port));

    /* 1. Get Tx HPD Status */
    if((port->content.tx->Hpd != AV_HPD_FORCE_LOW) &&
       (port->content.tx->Hpd != AV_HPD_FORCE_HIGH))
    {
        GSV2K7_TXPHY_get_HPD_RAW_STATE(port, &value);
        /* Tx Detection Power Down */
        switch(port->index)
        {
            case 4:
                GSV2K7_INT_get_TXA_HPD_INTR_INT_ST(port, &NewValue);
                if(NewValue == 1)
                {
                    GSV2K7_INT_set_TXA_HPD_INTR_CLEAR(port, 1);
                    value = 0;
                }
                GSV2K7_PRIM_get_TXA_TERM_DET_PWR_DN(port, &NewValue);
                if(NewValue != value)
                    GSV2K7_PRIM_set_TXA_TERM_DET_PWR_DN(port, value);
                break;
            case 5:
                GSV2K7_INT_get_TXB_HPD_INTR_INT_ST(port, &NewValue);
                if(NewValue == 1)
                {
                    GSV2K7_INT_set_TXB_HPD_INTR_CLEAR(port, 1);
                    value = 0;
                }
                GSV2K7_PRIM_get_TXB_TERM_DET_PWR_DN(port, &NewValue);
                if(NewValue != value)
                    GSV2K7_PRIM_set_TXB_TERM_DET_PWR_DN(port, value);
                break;
        }
        port->content.tx->Hpd = (AvHpdState)value;
    }

    /* 2. Local Pll Lock Status Check, pass the test to get LocalPllLock */
    LocalPllLock = 0;
    if((port->content.tx->Hpd != AV_HPD_LOW) && (FromPort != NULL))
    {
        /* 2.1 capture pll lock */
        switch(port->index)
        {
            case 5:
                /* capture pll lock low */
                if(port->content.tx->InfoReady > TxVideoManageThreshold)
                {
                    GSV2K7_PLL_get_RB_TXB_PLL_LOCK_CAPTURED(port, &value);
                    if(value == 0)
                        Gsv2k7TxPllUnlockClear(port);
                }
                else
                {
                    GSV2K7_PLL_get_RB_TXB_PLL_LOCK(port, &value);
                    if((value == 0) && (FromPort->content.rx->Lock.PllLock == 1))
                        Gsv2k7RpllProtect(port);
                }
                if(value == 1)
                {
#if Gsv2k7MuxMode
                    if(FromPort->content.rx->IsInputStable == 1)
#else
                    if(FromPort->content.rx->IsFreeRun == 1)
#endif
                        LocalPllLock = 1;
                }
                break;
        }
        /* 2.2 2006/2008 recover tx port fifo when already locked */
        if(LocalPllLock == 1)
        {
            AvHalI2cReadField8(GSV2K7_TXPHY_MAP_ADDR(port),0xDE,0xFF,0,&value);
            if(value == 0x03)
            {
                /* sign of tx fifo failure, both fifo full and empty */
                GSV2K7_TXPHY_set_TXPORT_FIFO_ERR_CLR(port, 1);
                GSV2K7_TXPHY_set_TXPORT_FIFO_ERR_CLR(port, 0);
                AvHalI2cReadField8(GSV2K7_TXPHY_MAP_ADDR(port),0xDE,0xFF,0,&value);
                if(value == 0x03)
                    Gsv2k7ResetTxFifo(port);
            }
        }
    }

    /* 3. Protect TMDS Output */
    if((port->content.tx->Lock.PllLock == 1) && (LocalPllLock == 0))
    {
        Gsv2k7ToggleTmdsOut(port, 0); /* Disable TMDS Out */
        AvUapiOutputDebugFsm("Port %d: PllUnlock", port->index);
    }

    /* 4. Tx Pll Setting for 6G protection */
    if(LocalPllLock)
    {
        /* 4.1 Get Tx Pll Freq */
        switch(port->index)
        {
            case 5:
                GSV2K7_PLL_get_RB_TXB_PLL_REF_CLK_FREQ(port, &value32);
                /* Reread Ref Clock Frequency */
                if(Gsv2k7TweakCrystalFreq(port, value32) == 1)
                    GSV2K7_PLL_get_RB_TXB_PLL_REF_CLK_FREQ(port, &value32);
                value32 = value32>>8;
                GSV2K7_PLL_get_RB_TXB_PLL_PRE_DIV(port, &value);
                value32 = value32/value;
                GSV2K7_PLL_get_RB_TXB_PLL_POST_DIV(port, &temp);
                value32 = value32 * temp;
                GSV2K7_PLL_get_RB_TXB_PLL_SER_DIV(port, &value);
                temp = (1<<value) * 5;
                value32 = value32/temp;
                break;

        }
        /* 4.1.1 Clock Change Detected */
        if((port->content.tx->InfoReady > TxVideoManageThreshold) &&
           (port->content.video->info.TmdsFreq != value32))
            LocalPllLock = 0;
        /* 4.1.2 TMDS Frequency Update */
        port->content.video->info.TmdsFreq = (uint16)value32;
        /* 4.2 Get Tx Pll Freq */
        switch(port->index)
        {
            case 5:
                GSV2K7_PLL_get_TXB_HDMI2_MODE_AUTO(port, &Freq3G4Plus);
                break;
        }
        /* 4.3 Set Phy Value Action */
        if(Freq3G4Plus == 1)
            NewValue = 1;
        else
            NewValue = 0;
        /* 4.4 Phy Mode Change */
        GSV2K7_TXPHY_get_TX_HDMI2_MODE_MAN(port, &value);
        if(value != NewValue)
        {
            GSV2K7_TXPHY_set_TX_HDMI2_MODE_MAN_EN(port, 1);
            GSV2K7_TXPHY_set_TX_HDMI2_MODE_MAN(port, NewValue);
        }
        /* 4.5 Scramble Change */
        GSV2K7_TXPHY_get_TX_SCR_EN_MAN(port, &value);
        if(value != NewValue)
        {
            GSV2K7_TXPHY_set_TX_SCR_EN_MAN(port, NewValue);
            GSV2K7_TXPHY_set_TX_SCR_EN_MAN_EN(port, 1);
        }
        /* 4.6 SCDC Change */
        GSV2K7_TXPHY_get_EDID_ANALYZE_DONE(port, &value);
        if(value == 0)
            GSV2K7_TXPHY_set_EDID_ANALYZE_DONE(port, 1);
    }

    /* 5. Find Final Lock Status, only pass the test to have Tx Pll Lock */
    if(port->content.tx->InfoReady == 1)
    {
        Gsv2k7ResetTxFifo(port);
        AvUapiTxSetHdmiModeSupport(port);
    }
    if(port->content.tx->InfoReady == TxVideoManageThreshold)
    {
        AvUapiOutputDebugFsm("Port %d: Tx Pixel Frequency = %d",port->index, port->content.video->info.TmdsFreq);
        port->content.tx->InfoReady = port->content.tx->InfoReady + 1;
        /* 5.0 Tx Phy Setting */
        if(port->content.video->info.TmdsFreq > 340)
        {
        #if (AvEnableTxCtsPhySetting == 1)
            AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xF0);
            AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x53);
            AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0F);
        #else
            /* Slew Rate: 0x14/0x34/0x54/0x74 */
            AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x54);//Tx Phy Characater, Preemp = 101
        #endif
        }
        else
        {
        #if (AvEnableTxCtsPhySetting == 1)
            if(port->content.video->info.TmdsFreq > 250)
            {
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xFE);
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x27);
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0F);
            }
            else
            {
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xF0);
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x40);
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0C);
            }
        #else
            /* Slew Rate: 0x10/0x30/0x50/0x70 */
            AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x50);//Tx Phy Characater, Preemp = 100
        #endif
        }
        /* 5.1 Protect for HDCP 2.2 Restart, disable EDID and SCDC in advance */
        GSV2K7_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
        GSV2K7_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 0);
        /* 5.2 Manual Write SCDC Scramble and Clock Divider */
        GSV2K7_TXPHY_set_RX_SCDC_PRESENT(port, 0); /* no disturb on sink PLL lock from DDC */
        if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC) != 0x0)
        {
            Gsv2k7ToggleTmdsOut(port, 0); /* Disable TMDS Out */
            if(NewValue == 1) /* Set SCDC */
                NewValue = 0x03;
            else
                NewValue = 0x00;
            Gsv2k7TxScdcAction(port, 1, 0xA8, 0x01, 0x01);
            Gsv2k7TxScdcAction(port, 1, 0xA8, 0x20, NewValue);
            Gsv2k7ToggleTmdsOut(port, 1); /* Enable TMDS Out */
        }
        else
        {
            GSV2K7_TXPHY_get_TX_TMDS_CLK_DRIVER_ENABLE(port, &value);
            if(value == 0)
                Gsv2k7ToggleTmdsOut(port, 1);
        }
        /* 5.3 Clear PLL Lock Status when entering stable */
        Gsv2k7TxPllUnlockClear(port);
    }

    /* 6. handle SCDC errors */
#if AvEnableTxScdcCedCheck
    if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC) != 0x0)
    {
        if(port->content.tx->InfoReady == TxHdcpManageThreshold)
        {
            GSV2K7_TXPHY_get_TX_SCDC_EN(port, &value);
            if(value == 0)
            {
                GSV2K7_TXPHY_set_TX_SCDC_EN(port, 1);
                GSV2K7_TXPHY_set_TX_RR_CAPABLE(port, 0); /* Auto CED readback */
                GSV2K7_TXPHY_set_RX_SCDC_PRESENT(port, 1); /* SCDC status check enable */
            }
        }
        else if(port->content.tx->InfoReady > TxHdcpManageThreshold)
        {
            GSV2K7_TXPHY_get_RX_SCDC_UPDATE_0(port, &value);
            if(value != 0x00)
            {
                value = value & 0x03; /* set task */
                GSV2K7_TXPHY_set_TX_CLR_UPDATE_FLAG(port, value);
                value = value | 0x80; /* task action */
                AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port),0x07,0xFF,0,value);
                AvUapiOutputDebugFsm("Port %d: Clear Interrupt = %x",port->index, value);
            }
        }
    }
#endif

    /* 7. Update Pll Lock Status*/
    /* reset phy */
    if((port->content.tx->Lock.PllLock == 0) && (LocalPllLock == 1))
    {
        GSV2K7_TXPHY_set_TMDS_SRST_MAN(port, 0);
        GSV2K7_TXPHY_set_TMDS_SRST_MAN(port, 1);
    }
    port->content.tx->Lock.PllLock = LocalPllLock;

#if (Gsv2k7PhyBypass == 1)
    if(port->content.tx->Hpd == AV_HPD_HIGH)
        port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
    else
        port->content.tx->EdidReadSuccess = AV_EDID_RESET;
#else
    if(port->content.tx->Lock.PllLock)
    {
        /* Pixel Repetition for 480i/576i */
        if((port->content.video->info.TmdsFreq  < 0x0f) &&
           (port->content.video->info.TmdsFreq != 0x00))
        {
            port->content.video->ClockMultiplyFactor = 1;
            port->content.video->PixelRepeatValue = 1;
        }
        else
        {
            port->content.video->ClockMultiplyFactor = 0;
            port->content.video->PixelRepeatValue = 0;
        }
        /* Protect DVI->HDMI Output RGB Color Space */
        if(port->content.tx->InfoReady == 1)
        {
            if((FromPort != NULL) &&
               (FromPort->type == HdmiRx) &&
               (FromPort->content.rx->IsFreeRun == 1) &&
               (FromPort->content.rx->HdmiMode == 0) &&
               (port->content.tx->HdmiMode == 1) &&
               ((port->content.video->AvailableVideoPackets & AV_BIT_AV_INFO_FRAME) == 0))
            {
                AvMemset(Packet, 0, PktSize[AV_PKT_AV_INFO_FRAME]);
                Packet[1] = 0x02;
                Packet[2] = 0x0D;
                Gsv2k7_TxSendAVInfoFrame(port, Packet, 1);
            }
        }
        if(port->content.tx->InfoReady >= TxVideoManageThreshold)
        {
            /* Video Black Mute */
            GSV2K7_TXDIG_get_TX_BLACK_VIDEO_EN(port, &value);
            if(value != port->content.video->Mute.BlkMute)
                GSV2K7_TXDIG_set_TX_BLACK_VIDEO_EN(port, port->content.video->Mute.BlkMute);
            /* Audio Mute */
            GSV2K7_TXDIG_get_TX_AUDIO_MUTE(port, &value);
            /* Unmute when Channel Status Available and Audio Mute is 0 */
            FromPort = (AvPort*)(port->content.RouteAudioFromPort);
            if(port->content.audio->AudioMute == 1)
                NewValue = 1;
            else if((FromPort != NULL) && (FromPort->type == LogicAudioRx))
                NewValue = 0;
            else if((port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                    (port->content.audio->AudType == AV_AUD_TYPE_DST) ||
                    ((port->content.audio->AvailableAudioPackets & AV_BIT_AUDIO_CHANNEL_STATUS) != 0))
                NewValue = 0;
            else
                NewValue = 1;
            if(value != NewValue)
                GSV2K7_TXDIG_set_TX_AUDIO_MUTE(port, NewValue);
        }
    }
    else
    {
        port->content.video->AvailableVideoPackets = 0;
        port->content.audio->AvailableAudioPackets = 0;
    }

    /* 9. Edid Status Update */
    GSV2K7_TXPHY_get_TX_EDID_READY_RB(port,    &value);
    if(port->content.tx->IgnoreEdidError != 0)
    {
        /* no need to read EDID if not required */
        port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
    }
    else if(value == 1)
    {
        /* EDID not updated yet, update it */
        if(port->content.tx->EdidReadSuccess == AV_EDID_RESET)
        {
            /* Manually set Edid Read Success, EDID will be read in Event */
            port->content.tx->EdidReadSuccess = AV_EDID_SUCCESS;
        }
    }
    else if((port->content.tx->Hpd == AV_HPD_HIGH) &&
            (port->content.tx->EdidReadSuccess == AV_EDID_RESET))
    {
        /* DDC circuit error */
        value = Gsv2k7TxDDCError(port);
        if(value == 1)
        {
            AvUapiOutputDebugFsm("Port %d: DDC Fail, Abort",port->index);
            port->content.tx->EdidReadFail    = 0;
            port->content.tx->EdidReadSuccess = AV_EDID_FAIL;
#if AvForceDefaultEdid
            port->content.tx->IgnoreEdidError = 1;
#endif
        }
        /* if has already tried 15 times, that means failed */
        GSV2K7_TXPHY_get_TX_EDID_TRY_TIMES(port, &value);
        if(value == 0)
        {
            AvUapiOutputDebugFsm("Port %d: Edid Fail, Retry",port->index);
            /* set flag */
            GSV2K7_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
            Gsv2k7ToggleTxHpd(port);
            if(port->content.tx->EdidReadFail < 255)
                port->content.tx->EdidReadFail = port->content.tx->EdidReadFail + 1;
            if(port->content.tx->EdidReadFail > AvEdidErrorThreshold)
            {
                port->content.tx->EdidReadFail    = 0;
                port->content.tx->EdidReadSuccess = AV_EDID_FAIL;
#if AvForceDefaultEdid
                port->content.tx->IgnoreEdidError = 1;
#endif
            }
        }
    }

    /* 10. Manual BKSV Read Protection */
#if AvEnableHdcp1p4BksvCheck
    if((port->content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
        (port->content.hdcptx->HdcpSupport == 0))
        Gsv2k7ManualBksvRead(port);
#endif

#endif //(Gsv2k7PhyBypass == 0)

    return ret;
}

/**
 * @brief  Tx video management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxVideoManage(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    uint8 NewValue = 0;
    uchar *RegTable;

    /* 1. DVI mode check */
    if(port->content.tx->HdmiMode == 0)
    {
        /* 1.1 Input Color for Black Mute */
        if(port->content.video->Y == AV_Y2Y1Y0_RGB)
            NewValue = 0;
        else
            NewValue = 1;
        GSV2K7_TXDIG_get_TX_VIDEO_INPUT_CS(port, &value);
        if(value != NewValue)
            GSV2K7_TXDIG_set_TX_VIDEO_INPUT_CS(port, NewValue);
        /* 1.2 DVI color space is always RGB */
        GSV2K7_TXDIG_get_TX_VIDEO_OUTPUT_FORMAT(port, &value);
        if(value != 0)
            GSV2K7_TXDIG_set_TX_VIDEO_OUTPUT_FORMAT(port, 0);
        /* 1.3 color manage */
        /* 1.3.1 get CSC table */
        if((port->content.video->Y == AV_Y2Y1Y0_YCBCR_444) ||
           (port->content.video->Y == AV_Y2Y1Y0_YCBCR_422))
        {
            NewValue = 1;
            if((port->content.video->InCs == AV_CS_YUV_709) ||
               (port->content.video->InCs == AV_CS_YCC_709))
                RegTable = (uchar *)CscYcc709FRtoRgbLR;
            else
                RegTable = (uchar *)CscYcc601FRtoRgbLR;
        }
        else
        {
            NewValue = 0;
        }
        /* 1.3.2 check current color setting */
        /* 1.3.2.1 csc enable */
        GSV2K7_TXDIG_get_TX_VIDEO_CSC_ENABLE(port, &value);
        if(value != NewValue)
            GSV2K7_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, NewValue);
        /* 1.3.2.2 csc coeffs */
        if(NewValue == 1)
        {
            AvHalI2cReadField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x2D, 0xFF, 0, &value);
            if(value != RegTable[1])
            {
                AvHalI2cWriteMultiField(GSV2K7_TXDIG_MAP_ADDR(port), 0x2C, 24, RegTable);
                value = (RegTable[0]>>5)&0x03;
                GSV2K7_TXDIG_set_TX_VIDEO_CSC_SCALING_FACTOR(port, value);
            }
        }
    }
    /* 2. GC color depth is fixed to 8-bit */
    if((port->content.tx->HdmiMode == 0) || (port->content.video->info.TmdsFreq > 450))
    {
        GSV2K7_TXDIG_get_TX_GC_CD(port, &value);
        if(value != 4)
            GSV2K7_TXDIG_set_TX_GC_CD(port, 4); /* fixed to 8-bit */
    }

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/**
 * @brief  Tx audio management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxAudioManage(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    uint8 TempStoreCA = 0;
    AvPort *FromPort = NULL;

    /* Protect Audio Type in case of mode switch,
       AV_AUD_TYPE_AUTO means incorrect configuration */
    FromPort = (AvPort*)(port->content.RouteAudioFromPort);
    if((FromPort != NULL) &&
       (FromPort->type == HdmiRx) &&
       (FromPort->content.rx->IsFreeRun == 1) &&
       (FromPort->content.audio->AudType != port->content.audio->AudType))
    {
        port->content.audio->SampFreq = FromPort->content.audio->SampFreq;
        port->content.audio->AudType  = FromPort->content.audio->AudType;
        port->content.audio->NValue   = FromPort->content.audio->NValue;
        AvUapiTxSetAudioPackets(port);
    }
    /* No Audio Output when compressed audio is not supported */
    if((port->content.audio->AudCoding != AV_AUD_FORMAT_LINEAR_PCM) &&
       ((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_COMPRESS_AUDIO) == 0))
    {
        GSV2K7_TXDIG_get_TX_PKT_AUDIO_SAMPLE_ENABLE(port, &value);
        if(value == 1)
            GSV2K7_TXDIG_set_TX_PKT_AUDIO_SAMPLE_ENABLE(port, 0);
    }
    else
    {
        /* Audio Manage: audio might underflow or overflow,
           change AUDIOIF_CC or AUDIOIF_CA will fix it */
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_get_TX1_AUDIOFIFO_FULL_RAW_ST(port, &value);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_get_TX2_AUDIOFIFO_FULL_RAW_ST(port, &value);
        if(value == 1)
        {
            /* toggle AUDIOIF_CC */
            GSV2K7_TXPKT_get_AUD_IF_CA7_0(port, &TempStoreCA);
            value = TempStoreCA + 1;
            GSV2K7_TXPKT_set_AUD_IF_CA7_0(port, value);
            GSV2K7_TXPKT_set_AUD_IF_CA7_0(port, TempStoreCA);
            AvUapiOutputDebugMessage("Port%d: Audio Recover From Fifo Full", port->index);
            if(port->core.HdmiCore == 0)
                GSV2K7_INT_set_TX1_AUDIOFIFO_FULL_CLEAR(port, 1);
            else if(port->core.HdmiCore == 1)
                GSV2K7_INT_set_TX2_AUDIOFIFO_FULL_CLEAR(port, 1);
        }
    }

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/**
 * @brief  Rx video management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt))
{
    return AvOk;
}

/**
 * @brief  Rx audio management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt))
{
#if (Gsv2k7PhyBypass == 0)

    uint8 DsdMan=1, DsdSel=0, HbrMan=1, HbrSel=0;
    uint8 ApllRecalcFlag = 0;

    if(Intpt->AudChanMode)
    {
        ApllRecalcFlag = 1;
    }
    if(Intpt->InternMute)
    {
        Gsv2k7AvUapiRxSetAudioInternalMute(port);
    }
    if(Intpt->CsDataValid)
    {
        switch (port->content.audio->AudFormat)
        {
            case AV_AUD_I2S:
                GSV2K7_RXAUD_set_RX_DIS_I2S_ZERO_COMPR(port, 1);
                break;

            case AV_AUD_SPDIF:
                HbrSel = 1;
                break;

            case AV_AUD_DSD_NORM:
                DsdSel = 1;
                break;

            case AV_AUD_IF_AUTO:
                DsdMan = HbrMan = 0;
                break;

            default:
                break;
        };

        GSV2K7_RXAUD_set_RX_DSD_OUT_MAN_EN(port, DsdMan);
        GSV2K7_RXAUD_set_RX_DSD_OUT_MAN_SEL(port, DsdSel);
        GSV2K7_RXAUD_set_RX_HBR_OUT_MAN_EN(port, HbrMan);
        GSV2K7_RXAUD_set_RX_HBR_OUT_MAN_SEL(port, HbrSel);
    }
    if(Intpt->NChange)
    {
        ApllRecalcFlag = 1;
    }
    if(Intpt->CtsThresh)
    {
        ApllRecalcFlag = 1;
    }
    if(Intpt->AudFifoOv)
    {
        ApllRecalcFlag = 1;
    }
    if(Intpt->AudFifoUn)
    {
        ApllRecalcFlag = 1;
    }
    if(Intpt->AudModeChg)
    {
        /* Switch between DSD/I2S, data rate varies triggers Audio Mute */
        GSV2K7_RXAUD_set_DISABLE_AUDIO_MUTE(port, 1);
        GSV2K7_RXAUD_set_DISABLE_AUDIO_MUTE(port, 0);
    }

    if(ApllRecalcFlag == 1)
    {
        GSV2K7_RXDIG_set_N_CTS_STABLE_DET_START(port, 1);
        GSV2K7_RXDIG_set_RX_APLL_MAN_START_CALC(port, 1);
    }

#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

/**
 * @brief  connect rx and tx
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type))
{
    AvRet ret = AvOk;
    uint8 value1 = 0;
    uint8 value2 = 0;
    uint8 i2c_rx2_port_sel = 0xff;
    uint8 i2c_txb_port_sel = 0xff;
    AvPort *TempPort = NULL;
    AvPort *DeSelPort = NULL;

    /* Step 1. Disconnet invalid port */
    if((ToPort->type == HdmiTx) && (type != AvConnectAudio))  /* Tx core with video connection */
    {
        if(ToPort->content.tx->InfoReady > TxVideoManageThreshold)
            ToPort->content.tx->Hpd = AV_HPD_FORCE_LOW;
        else
            ToPort->content.tx->InfoReady = 0;
        /* not current selection, reset the port */
        if((FromPort != ((AvPort*)ToPort->content.RouteVideoFromPort)) &&
           (FromPort != ((AvPort*)ToPort->content.RouteAudioFromPort)))
        {
            Gsv2k7AvUapiDisconnectPort(ToPort);
        }
        if(ToPort->core.HdmiCore != -1)
            Gsv2k7AvUapiTxDisableCore(ToPort);
    }
    /* Step 1.1 Avoid conflict because Color Port and Scaler Port share logic */
    else if((ToPort->index == 21) || (ToPort->index == 25))
    {
        /* 1.1.1 Find the port index that should be disconnected */
        switch(ToPort->index)
        {
            case 21:
                value1 = 25;
                break;
            case 25:
                value1 = 21;
                break;
        }
        /* 1.1.2 Find the port pointer that should be disconnected */
        value2 = ToPort->device->index;
        TempPort = ToPort->device->port; /* Get 1st Port of the device */
        while((TempPort != NULL) && (TempPort->device->index == value2))
        {
            DeSelPort = (AvPort*)TempPort->content.RouteVideoFromPort;
            if((DeSelPort != NULL) && (DeSelPort->index == value1))
                TempPort->content.RouteVideoFromPort = NULL;
            DeSelPort = (AvPort*)TempPort->content.RouteVideoToPort;
            if((DeSelPort != NULL) && (DeSelPort->index == value1))
                TempPort->content.RouteVideoToPort   = NULL;
            DeSelPort = (AvPort*)TempPort->content.RouteVideoNextSameLevelPort;
            if((DeSelPort != NULL) && (DeSelPort->index == value1))
                TempPort->content.RouteVideoNextSameLevelPort = DeSelPort->content.RouteVideoNextSameLevelPort;
#if (AvAllowHpdLowEdidRead == 0)
            if((TempPort->type == HdmiRx) && (TempPort != FromPort))
                TempPort->content.rx->Hpd = AV_HPD_FORCE_LOW;
#endif
            TempPort = (AvPort*)(TempPort->next);
        }
        /* 1.1.3 Clean the useless port content */
        TempPort = ToPort->device->port; /* Get 1st Port of the device */
        while((TempPort != NULL) && (TempPort->device->index == value2))
        {
            /* port pointer found */
            if(TempPort->index == value1)
            {
                TempPort->content.RouteVideoFromPort = NULL;
                TempPort->content.RouteVideoToPort   = NULL;
                TempPort->content.RouteVideoNextSameLevelPort = NULL;
            }
            TempPort = (AvPort*)(TempPort->next);
        }
    }

    /* Step 2. Get Previous Routing State */
    /* Step 4. Set Current Routing Registers */
    /* Step 5. Set Routing State */
    /* Step 6. Clean Previous and Close Useless Ports */
    /* Step 6.1, Set Tx Port Core Occupation */
    switch(ToPort->index)
    {
        case 5:
            /* Disable Previous Selected RxPort */
            DeSelPort = FindCore2(port);
            if((DeSelPort != NULL) && (DeSelPort->type == HdmiRx))
                DeSelPort->core.HdmiCore = -1;
            /* Enable Current Selected RxPort */
            FindCore2(port) = FromPort;
            FromPort->core.HdmiCore = 1;
            i2c_rx2_port_sel = FromPort->index;
            /* Enable Current Selected TxPort */
            ToPort->core.HdmiCore = 1;
            break;
        case 21:
        case 25:
            GSV2K7_SEC_set_CP2_SRC_SEL(ToPort, 0);
            FindCp2Mode(ToPort) = ToPort;
            break;
    }
    /* 6.2 Disable InfoFrames in advance */
    if(ToPort->type == HdmiTx)
    {
        if(type == AvConnectAudio)
        {
            ToPort->content.audio->AvailableAudioPackets = 0;
            Gsv2k7AvUapiTxEnableInfoFrames (ToPort, ~(AV_BIT_ACR_PACKET |
                                                      AV_BIT_AUDIO_SAMPLE_PACKET), 0);
        }
        else
        {
            ToPort->content.tx->Lock.PllLock = 0;
            ToPort->content.video->AvailableVideoPackets = 0;
            ToPort->content.audio->AvailableAudioPackets = 0;
            Gsv2k7AvUapiTxEnableInfoFrames (ToPort, ~(AV_BIT_GC_PACKET |
                                                AV_BIT_AV_INFO_FRAME |
                                                AV_BIT_ACR_PACKET |
                                                AV_BIT_AUDIO_SAMPLE_PACKET), 0);
        }
    }
    /* Step 6.3 Clock Ratio Update */
#if AvAllowHpdLowEdidRead
    if((FromPort->type == HdmiRx) && (ToPort->type == HdmiTx) && (type != AvConnectAudio))
    {
        GSV2K7_RX2P2_set_RX_DISABLE_ECC_CHECK(FromPort, 1);
        GSV2K7_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(FromPort, 1);
        GSV2K7_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(FromPort, 0);
        //GSV2K7_RXDIG_set_ECC_AVMUTE_MAN_EN(FromPort, 1);
        //GSV2K7_RXDIG_set_ECC_AVMUTE_MAN_EN(FromPort, 0);
        /* reset Rx Scdc/Clock Ratio status when port switching */
        RxScdcFsm(port)[FromPort->index] = 0;
        Gsv2k7RxScdcFsm(FromPort);
    }
#endif

    /* Step 7. Switch Video Port Selection */
    if((FromPort->type == HdmiRx) && (ToPort->type == HdmiTx) && (type != AvConnectAudio))
    {
        GSV2K7_PRIM_set_RX2_CORE_SEL(FromPort,       i2c_rx2_port_sel);
    }
    /* Step 7.1 Switch Audio Port Selection */
    value2 = 2;
    /* Step 7.1.1 Set Audio TTL Routing */
    if(FromPort->index == 10)
    {
        if(ToPort->index == 5)
            value2 = 5;
    }
    else if(FromPort->index == 11)
    {
        if(ToPort->index == 5)
            value2 = 6;
    }
    if(ToPort->content.RouteAudioFromPort == (struct AvPort*)FromPort)
    {
        if(ToPort->index == 5)
            GSV2K7_PRIM_set_TX2_AUD_SRC_SEL(ToPort,    value2);
    }
    /* Step 7.2 Switch Video Port Selection */
#if (Gsv2k7PhyBypass == 1)
    AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port), 0x07, 0xFF, 0x0, 0x00);
    AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port), 0x08, 0xFF, 0x0, 0x00);
    switch(FromPort->index)
    {
        case 0:
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port), 0x07, 0xFF, 0x0, 0x04);
            break;
        case 1:
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port), 0x07, 0xFF, 0x0, 0x40);
            break;
        case 2:
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port), 0x08, 0xFF, 0x0, 0x04);
            break;
        case 3:
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port), 0x08, 0xFF, 0x0, 0x40);
            break;
    }
#else
    switch(ToPort->index)
    {
        case 5:
            /* To TxB */
            switch(FromPort->index)
            {
                case 21:
                case 25:
                    i2c_txb_port_sel = 0x03; /* Rx2 - Cp2 - TxB */
                    break;
                default:
                    i2c_txb_port_sel = 0x01;
                    break;
            }
            GSV2K7_SEC_set_TX2_SRC_SEL(FromPort, i2c_txb_port_sel);
            break;
        case 8:
            /* Mute Output */
            GSV2K7_PRIM_set_AUD1_OEN(FromPort, 0xff);
            switch(FromPort->index)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    value2 = 2;
                    break;
                case 5:
                    AvUapiTxArcEnable(FromPort, 1);
                    value2 = 3;
                    break;
                default:
                    value2 = 0;
                    break;
            }
            GSV2K7_PRIM_set_AP1_OUT_SRC_SEL(FromPort, value2);
            break;
        case 9:
            /* Mute Output */
            GSV2K7_PRIM_set_AUD2_OEN(FromPort, 0xff);
            switch(FromPort->index)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    value2 = 2;
                    break;
                case 5:
                    AvUapiTxArcEnable(FromPort, 1);
                    value2 = 3;
                    break;
                default:
                    value2 = 0;
                    break;
            }
            GSV2K7_PRIM_set_AP2_OUT_SRC_SEL(FromPort, value2);
            break;
        case 10:
            GSV2K7_PRIM_set_AUD1_OEN(FromPort, 255);
            break;
        case 11:
            GSV2K7_PRIM_set_AUD2_OEN(FromPort, 255);
            break;
    }
#endif //(Gsv2k7PhyBypass == 0)

#if AvEnableInternalVideoGen
    /* Step 7.3 VideoGen Process */
    if(FromPort->index == 28)
    {
        /* 7.3.1 Select VideoGen Clock Input */
        FromPort->core.HdmiCore = 0;
        GSV2K7_PLL_set_VG_CLK_SRC_SEL(FromPort, 0);
        /* 7.3.2 Select VideoGen Clock Input */
        switch(ToPort->index)
        {
            case 5:
                GSV2K7_SEC_set_TX2_SRC_SEL(ToPort, 4);
                break;
            case 21:
            case 25:
                GSV2K7_SEC_set_CP2_SRC_SEL(ToPort, 1);
                break;
        }
    }
#endif

    /* Step 8. Update Hdmi Mode Status based on core switch */
    if(ToPort->type == HdmiTx)
        AvUapiTxSetHdmiModeSupport(ToPort);

    return ret;
}

/**
 * @brief  disconnect rx and tx
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiDisconnectPort(pin AvPort *port))
{
    if(port->index == 5)
    {
        /* 1. Disconnect Tx Port */
        Gsv2k7ResetTxPhy(port);
        port->core.HdmiCore = -1;
    }
    else if(port->index <= 3)
    {
        /* 1. Disconnect Rx Port */
        Gsv2k7DisableRxHpa(port);
        port->content.rx->Hpd = AV_HPD_LOW;
        port->content.rx->HpdDelayExpire = 0;
        /* 2. Clear Rx's Pointer */
        port->core.HdmiCore = -1;
    }

    return AvOk;
}

/**
 * @brief  Configure Tx Audio Packets
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetAudioPackets(pin AvPort *port))
{
    AvRet ret = AvInvalidParameter;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;

    if((port->content.tx->Hpd != AV_HPD_HIGH) ||
       (port->content.tx->Lock.PllLock == 0) ||
       (port->content.tx->Lock.AudioLock == 0))
    {
        port->content.audio->AvailableAudioPackets =
            port->content.audio->AvailableAudioPackets &
            (0xFFFF - AV_BIT_AUDIO_SAMPLE_PACKET - AV_BIT_AUDIO_INFO_FRAME - AV_BIT_ACR_PACKET);
        Gsv2k7AvUapiTxEnableInfoFrames(port, AV_BIT_AUDIO_SAMPLE_PACKET |
                                   AV_BIT_AUDIO_INFO_FRAME |
                                   AV_BIT_ACR_PACKET, false);
        return ret;
    }

    if(port->index == 5)
    {
        /* Step 1. Select Mclk Source */
        if(port->content.audio->AudType == AV_AUD_TYPE_SPDIF)
            value = 0;
        else
            value = 1;
        GSV2K7_TXDIG_set_TX_AUDIO_EXT_MCLK_ENABLE(port, value);
        /* Step 2. Select Correct Audio Input */
        if(port->content.audio->AudType == AV_AUD_TYPE_UNKNOWN)
            value = 0;
        else
            value = port->content.audio->AudType;
        GSV2K7_TXDIG_set_TX_AUDIO_MODE(port, value);
        /* Step 4. Enable Audio Interface */
        Gsv2k7_TxSetAudioInterface(port);
        /* Step 5. Enable Audio Sampling Frequency */
        Gsv2k7_TxSetAudChStatSampFreq(port);
        /* Step 6. Enable N Value */
        Gsv2k7AvUapiTxSetAudNValue(port);
        /* Finally. Enable the packet */
        Gsv2k7AvUapiTxEnableInfoFrames(port, AV_BIT_AUDIO_SAMPLE_PACKET | AV_BIT_ACR_PACKET, true);
        ret = AvOk;
    }

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/**
 * @brief  Clear Rx Flag indication
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxClearFlags(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    if(port->core.HdmiCore != -1)
    {
        GSV2K7_RXAUD_set_RX_PKT_DET_RST(port, 1);
    }
#endif
    return ret;
}

/**
 * @brief  Read Rx Video Packet Status
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt))
{
#if (Gsv2k7PhyBypass == 0)
    uint8 Value = 0;
    uint8 MapData[42];
    uint16 AvailablePacket = 0;

    AvMemset(Intpt, 0, sizeof(VideoInterrupt));

    /* 1. Read Map Data */
    if(port->core.HdmiCore == 0)
        Value = 0x30;
    else if(port->core.HdmiCore == 1)
        Value = 0x40;
    else
        return AvNotAvailable;
    AvHalI2cReadMultiField(GSV2K7_INT_MAP_ADDR(port),Value,15,MapData+0);
    if(port->core.HdmiCore == 0)
        Value = 0x50;
    else if(port->core.HdmiCore == 1)
        Value = 0x70;
    AvHalI2cReadMultiField(GSV2K7_INT_MAP_ADDR(port),Value,27,MapData+15);

    /* 2. Handle Video Interrupt */
    if(((MapData[6] & 0x10) != 0) ||
       (port->content.video->Mute.AvMute != ((MapData[6]>>5) & 0x01)))
        Intpt->AvMute = 1;
    if(((MapData[5] & 0x10) != 0) ||
       (port->content.rx->HdmiMode != ((MapData[5]>>5) & 0x01)))
        Intpt->HdmiModeChg = 1;
    if(((MapData[8] & 0x01) != 0) ||
       (port->content.rx->Lock.DeRegenLock != ((MapData[8]>>1) & 0x01)))
        Intpt->DeRegenLck = 1;
    if(((MapData[8] & 0x10) != 0) ||
       (port->content.rx->Lock.VSyncLock != ((MapData[8]>>5) & 0x01)))
        Intpt->VsyncLck = 1;
    if(((MapData[9] & 0x01) != 0) ||
       (port->content.video->info.Detect3D != ((MapData[9]>>1) & 0x01)))
        Intpt->Vid3dDet = 1;

    if((MapData[23] & 0x30) != 0)
        Intpt->NewTmds = 1;
    if((MapData[26] & 0x03) != 0)
        Intpt->BadTmdsClk = 1;
    if((MapData[26] & 0x30) != 0)
        Intpt->DeepClrChg = 1;
    if((MapData[20] & 0x03) != 0)
        Intpt->PktErr = 1;

    if(((MapData[0] & 0x01) != 0) || ((MapData[15] & 0x01) != 0))
        Intpt->AvIfValid = 1;
    else
    {
        if((port->content.video->AvailableVideoPackets & AV_BIT_AV_INFO_FRAME) != 0)
        {
            AvHalI2cReadField8(GSV2K7_RXINFO_MAP_ADDR(port),0x00,0xFF,0,&Value);
            if(Value != port->content.rx->Cks.AvCks)
                Intpt->AvIfValid = 1;
        }
    }
    if(((MapData[1] & 0x01) != 0) || ((MapData[16] & 0x01) != 0))
        Intpt->SpdValid = 1;
    if(((MapData[13] & 0x10) != 0) || ((MapData[41] & 0x10) != 0))
        Intpt->HdrValid = 1;
    if(((MapData[1] & 0x10) != 0) || ((MapData[16] & 0x10) != 0))
        Intpt->MsValid = 1;
    if(((MapData[2] & 0x01) != 0) || ((MapData[17] & 0x01) != 0))
        Intpt->VsValid = 1;
    if(((MapData[3] & 0x01) != 0) || ((MapData[18] & 0x01) != 0))
        Intpt->Isrc1Valid = 1;
    if(((MapData[3] & 0x10) != 0) || ((MapData[18] & 0x10) != 0))
        Intpt->Isrc2Valid = 1;
    if(((MapData[4] & 0x01) != 0) || ((MapData[19] & 0x01) != 0))
        Intpt->GamutValid = 1;
    if((MapData[5] & 0x01) != 0)
        Intpt->GcValid = 1;
    else
    {
        Value = (MapData[5]>>1) & 0x01;
        AvailablePacket = port->content.video->AvailableVideoPackets & AV_BIT_GC_PACKET;
        if((AvailablePacket != 0) && (Value == 0))
        {
            Intpt->GcValid = 1;
        }
        else if((AvailablePacket == 0) && (Value != 0))
        {
            Intpt->GcValid = 1;
        }
    }

    if(Intpt->AvMute == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AvMute",port->index);
    if(Intpt->HdmiModeChg == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->HdmiModeChg",port->index);
    if(Intpt->DeRegenLck == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->DeRegenLck",port->index);
    if(Intpt->Vid3dDet == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->Vid3dDet",port->index);
    if(Intpt->NewTmds == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->NewTmds",port->index);
    if(Intpt->BadTmdsClk == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->BadTmdsClk",port->index);
    if(Intpt->DeepClrChg == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->DeepClrChg",port->index);
    if(Intpt->PktErr == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->PktErr",port->index);
    if(Intpt->AvIfValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AvIfValid",port->index);
    if(Intpt->SpdValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->SpdValid",port->index);
    if(Intpt->HdrValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->HdrValid",port->index);
    if(Intpt->MsValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->MsValid",port->index);
    if(Intpt->VsValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->VsValid",port->index);
    if(Intpt->Isrc1Valid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->Isrc1Valid",port->index);
    if(Intpt->Isrc2Valid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->Isrc2Valid",port->index);
    if(Intpt->GamutValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->GamutValid",port->index);
    if(Intpt->GcValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->GcValid",port->index);

#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetPacketType(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    uint8 temp  = 0;

    GSV2K7_RXAUD_get_AUDIO_SAMPLE_PKT_DET(port, &value);
    if(value == 1)
    {
        port->content.audio->AudFormat  = AV_AUD_I2S;
        port->content.audio->AudType    = AV_AUD_TYPE_ASP;
    }
    else
    {
        GSV2K7_RXAUD_get_DSD_AUDIO_PKT_DET(port, &value);
        if(value == 1)
        {
            port->content.audio->AudFormat = AV_AUD_DSD_NORM;
            port->content.audio->AudType   = AV_AUD_TYPE_DSD;
        }
        else
        {
            GSV2K7_RXAUD_get_DST_AUDIO_PKT_DET(port, &value);
            if(value == 1)
            {
                port->content.audio->AudFormat  = AV_AUD_DSD_DST;
                port->content.audio->AudType    = AV_AUD_TYPE_DST;
            }
            else
            {
                GSV2K7_RXAUD_get_HBR_AUDIO_PKT_DET(port, &value);
                if(value == 1)
                {
                    port->content.audio->AudFormat  = AV_AUD_I2S;
                    port->content.audio->AudType = AV_AUD_TYPE_HBR;
                }
                else
                {
                    port->content.audio->AudFormat = AV_AUD_IF_AUTO;
                    port->content.audio->AudType = AV_AUD_TYPE_UNKNOWN;
                }
            }
        }
    }
    /* HBR Special Setting */
    GSV2K7_RXAUD_get_RX_SEL_I2S(port, &value);
    if(port->content.audio->AudType == AV_AUD_TYPE_HBR)
    {
        /* HBR will use SPDIF Out */
        temp = 0x00;
    }
    else
    {
        /* LPCM will use I2S Out, this is default */
        temp = 0x01;
    }
    if(temp != value)
        GSV2K7_RXAUD_set_RX_SEL_I2S(port, temp);

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/**
 * @brief  Read Rx Audio Packets
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt))
{
#if (Gsv2k7PhyBypass == 0)
    uint8 MapData[42];
    uint8 Value = 0;

    AvMemset(Intpt, 0, sizeof(AudioInterrupt));

    /* 1. Read Map Data */
    if(port->content.rx->Lock.AudioLock)
    {
        if(port->core.HdmiCore == 0)
            Value = 0x30;
        else if(port->core.HdmiCore == 1)
            Value = 0x40;
        else
            return AvNotAvailable;
        AvHalI2cReadMultiField(GSV2K7_INT_MAP_ADDR(port),Value,15,MapData+0);
        if(port->core.HdmiCore == 0)
            Value = 0x50;
        else if(port->core.HdmiCore == 1)
            Value = 0x70;
        AvHalI2cReadMultiField(GSV2K7_INT_MAP_ADDR(port),Value,27,MapData+15);
    }
    else
        return AvNotAvailable;

    /* 2. Handle Audio Interrupt */
    if((MapData[7] & 0x01) != 0)
        Intpt->InternMute = 1;
    if((MapData[6] & 0x01) != 0)
        Intpt->AudChanMode = 1;
    else
    {
        Value = (MapData[6]>>1) & 0x01;
        if(port->content.audio->AudioMute == 1)
            Intpt->AudChanMode = 0;
        else if((Value == 1) && (port->content.audio->Layout == 0))
        {
            Intpt->AudChanMode = 1;
        }
        else if((Value == 0) && (port->content.audio->Layout == 1))
        {
            Intpt->AudChanMode = 1;
        }
    }
    if(((MapData[7] & 0x10) != 0) || ((MapData[24] & 0x10) != 0))
       Intpt->CsDataValid = 1;
    if((MapData[20] & 0x30) != 0)
        Intpt->NChange = 1;
    if(((MapData[4] & 0x10) != 0) || ((MapData[21] & 0x01) != 0))
        Intpt->CtsThresh = 1;
    if((MapData[21] & 0x30) != 0)
        Intpt->AudFifoOv = 1;
    if((MapData[22] & 0x03) != 0)
        Intpt->AudFifoUn = 1;
    if((MapData[22] & 0x30) != 0)
        Intpt->AudFifoNrOv = 1;
    if((MapData[19] & 0x30) != 0)
        Intpt->AudioPktErr = 1;
    if((MapData[25] & 0x30) != 0)
        Intpt->AudModeChg = 1;
    /* Detect Packet Type */
    if(Intpt->AudModeChg == 1)
        Gsv2k7AvUapiRxGetPacketType(port);
    if((MapData[23] & 0x03) != 0)
        Intpt->AudFifoNrUn = 1;
    if((MapData[24] & 0x03) != 0)
        Intpt->AudFlatLine = 1;
    if((MapData[23] & 0x30) != 0)
        Intpt->AudSampChg = 1;
    if((MapData[25] & 0x03) != 0)
        Intpt->AudPrtyErr = 1;
    if(((MapData[0] & 0x10) != 0) || ((MapData[15] & 0x10) != 0))
        Intpt->AudIfValid = 1;
    if(((MapData[2] & 0x10) != 0) || ((MapData[17] & 0x10) != 0))
        Intpt->AcpValid = 1;
    /* Audio Channel Status Protect: check channel status is correct or not */
    if((port->content.rx->IsInputStable == 0) || (port->content.audio->AudioMute == 1))
        Intpt->CsDataValid = 0;

    if(Intpt->AudChanMode == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudChanMode",port->index);
    if(Intpt->InternMute == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->InternMute",port->index);
    if(Intpt->CsDataValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->CsDataValid",port->index);
    if(Intpt->NChange == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->NChange",port->index);
    if(Intpt->CtsThresh == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->CtsThresh",port->index);
    if(Intpt->AudFifoOv == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudFifoOv",port->index);
    if(Intpt->AudFifoUn == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudFifoUn",port->index);
    /* Don't care about Near OverFlow or Near UnderFlow */
    /*
    if(Intpt->AudFifoNrOv == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudFifoNrOv",port->index);
    if(Intpt->AudFifoNrUn == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudFifoNrUn",port->index);
    */
    if(Intpt->AudioPktErr == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudioPktErr",port->index);
    if(Intpt->AudModeChg == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudModeChg",port->index);
    /*
    if(Intpt->AudFlatLine == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudFlatLine",port->index);
    */
    if(Intpt->AudSampChg == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudSampChg",port->index);
    if(Intpt->AudPrtyErr == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudPrtyErr",port->index);
    if(Intpt->AudIfValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AudIfValid",port->index);
    if(Intpt->AcpValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->AcpValid",port->index);

#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

/**
 * @brief  Check HDCP Encryption
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    uint8 value1 = 0;
    uint8 Rx2p2Stat = 0;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    uint8 PreHdcpVersion = 0;
    uint8 PreHdcpManual  = 0;
    uint8 HdcpVersion = 0;
    uint8 HdcpManual  = 0;
    uint8 NewHdcpVersion = 0;
    uint8 NewHdcpManual  = 0;

    AvMemset(Intpt, 0, sizeof(HdcpInterrupt));

    if(port == FindCore2(port))
    {
        /* hdcp */
        GSV2K7_INT_get_RX2_AKSV_UPDATE_INT_ST(port,       &Intpt->AksvUpdate);
        GSV2K7_INT_get_RX2_HDCP1P4_ENCRYPTED_INT_ST(port, &Intpt->Encrypted);
        GSV2K7_INT_get_RX2_AKSV_UPDATE_RAW_ST(port,       &port->content.hdcp->AksvInterrupt);
        GSV2K7_INT_get_RX2_HDCP1P4_ENCRYPTED_RAW_ST(port, &port->content.rx->VideoEncrypted);
        GSV2K7_INT_get_RX2_HDCP2P2_RX_AUTH_RAW_ST(port,   &Rx2p2Stat);
#if (AvAllowHpdLowEdidRead == 1)
        if(port->content.rx->IsFreeRun == 1)
        {
            if(port->content.hdcp->Hdcp2p2RxRunning == 1)
            {
                GSV2K7_INT_get_RX1_PKT_PKTERR_INT_ST(port, &value);
                if(value == 1)
                {
                    port->content.hdcp->HdcpError = port->content.hdcp->HdcpError + 1;
                    GSV2K7_INT_set_RX1_PKT_PKTERR_CLEAR(port, 1);
                }
            }
        }
        else if(port->content.rx->IsInputStable == 1)
        {
            GSV2K7_INT_set_RX1_PKT_PKTERR_CLEAR(port, 1);
        }
#endif
    }
    else
    {
        return AvNotSupport;
    }

    if(Intpt->Encrypted)
    {
        if(Rx2p2Stat == 0)
        {
            if(port->content.rx->VideoEncrypted == 1)
                AvUapiOutputDebugMessage("Port Rx %d: Rx is encrypted with HDCP 1.4", (port->index));
        }
        else if(Rx2p2Stat == 1)
        {
            if(port->content.rx->VideoEncrypted == 1)
            {
                AvUapiOutputDebugMessage("Port Rx %d: Rx is encrypted with HDCP 2.2", (port->index));
                if(port->content.rx->Lock.EqLock == 0)
                {
                    GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                    GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
                    AvUapiOutputDebugMessage("Port Rx %d: HDCP 2.2 Reauth Triggered", (port->index));
                }
            }
        }
    }

    /* Check HDCP State */
    GSV2K7_PRIM_get_RX_HDCP_CFG_MAN_EN(port, &PreHdcpManual);
    GSV2K7_PRIM_get_RX_CFG_TO_HDCP2P2(port,  &PreHdcpVersion);
    if(port == FindCore2(port))
    {
        HdcpManual     = (PreHdcpManual>>1)  & 0x01;
        HdcpVersion    = (PreHdcpVersion>>1) & 0x01;
        PreHdcpManual  = PreHdcpManual  & 0x01;
        PreHdcpVersion = PreHdcpVersion & 0x01;
    }
    NewHdcpManual  = HdcpManual;
    NewHdcpVersion = HdcpVersion;
    switch(port->content.hdcp->HdcpNeeded)
    {
        case AV_HDCP_RX_AUTO:
            /* Auto Hdcp Mode */
            NewHdcpManual = 0;
            break;
        case AV_HDCP_RX_FOLLOW_SINK:
            /* default to Auto Rx Mode */
            NewHdcpManual  = 0;
            NewHdcpVersion = 0;
            while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
            {
                if(CurrentPort->type == HdmiTx)
                {
                    if(CurrentPort->content.tx->HdmiMode == 0)
                    {
                        /* only support 1.4 when Dvi sink connected */
                        if(CurrentPort->content.tx->EdidReadSuccess == AV_EDID_UPDATED)
                        {
                            NewHdcpManual  = 1;
                            NewHdcpVersion = 0;
                        }
                    }
                    else if(CurrentPort->content.hdcptx->HdcpSupport == 1)
                    {
                        /* Found HDCP 1.4 only device, set Rx HDCP to 1.4 */
                        GSV2K7_TXPHY_get_TX_HDCP2P2_CAPABILITY_RB(CurrentPort, &value);
                        GSV2K7_TXPHY_get_TX_HDCP2P2_CHECKED(CurrentPort, &value1);
                        if((CurrentPort->content.hdcptx->Hdcp2p2SinkSupport == 0) &&
                           ((value ==0) || (value1 == 0)))
                        {
                            NewHdcpManual  = 1;
                            NewHdcpVersion = 0;
                        }
                    }
                }
                PrevPort = CurrentPort;
            }
            break;
        case AV_HDCP_RX_1P4_ONLY:
            /* Hdcp 1.4 only Mode */
            NewHdcpManual  = 1;
            NewHdcpVersion = 0;
            break;
        case AV_HDCP_RX_2P2_ONLY:
            /* Hdcp 2.2 only Mode */
            NewHdcpManual  = 1;
            NewHdcpVersion = 1;
            break;
    }
    if((NewHdcpManual != HdcpManual) || (NewHdcpVersion != HdcpVersion))
    {
        port->content.rx->Hpd = AV_HPD_TOGGLE;
        if(port == FindCore1(port))
        {
            HdcpManual  = NewHdcpManual  | PreHdcpManual;
            HdcpVersion = NewHdcpVersion | PreHdcpVersion;
        }
        else if(port == FindCore2(port))
        {
            HdcpManual  = (NewHdcpManual<<1)  | PreHdcpManual;
            HdcpVersion = (NewHdcpVersion<<1) | PreHdcpVersion;
        }
        GSV2K7_PRIM_set_RX_HDCP_CFG_MAN_EN(port, HdcpManual);
        GSV2K7_PRIM_set_RX_CFG_TO_HDCP2P2(port,  HdcpVersion);
    }

    /* Update VideoEncrypted Status */
    if(Intpt->Encrypted)
    {
        /* When Found Rx has started being encrypted ,
           start to check Encryption is not stopped by AvMute */
        if(port == FindCore2(port))
            GSV2K7_INT_set_RX2_HDCP1P4_ENCRYPTED_CLEAR(port,1);
    }

#if AvEnableHdcp2p2Feature
    if(Rx2p2Stat == 0x01)
        value = 1;
    else
        value = 0;
    /* IO.0x46[5],0x27[5] = 1, HDCP 2.2 Rx Version */
    /* IO.0x46[4],0x27[4] = 1, HDCP 2.2 Rx is encrypted */
    if(value != port->content.hdcp->Hdcp2p2RxRunning)
    {
        port->content.hdcp->Hdcp2p2RxRunning = value;
        if(value == 1)
            AvUapiOutputDebugMessage("Port Rx %d: Rx is Running HDCP 2.2", (port->index));
        else
            AvUapiOutputDebugMessage("Port Rx %d: Rx is Stopping HDCP 2.2", (port->index));
    }

    /* HDCP 2.2 ECC Check Judge */
    //#if (Gsv2k7MuxMode == 0)
    GSV2K7_RX2P2_get_RX_DISABLE_ECC_CHECK(port, &value);
    value1 = 1 - port->content.rx->IsFreeRun;
    if(value1 != value)
    {
        GSV2K7_RX2P2_set_RX_DISABLE_ECC_CHECK(port, value1);
        AvUapiOutputDebugMessage("Port %d: Rx ECC setting to %d", (port->index), value1);
    }
    //#endif

#endif

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/**
 * @brief  Check Tx HDCP Encryption Capability
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxGetHdcpStatus(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 Desire1p4 = 0;
    uint8 Desire2p2 = 0;
    uint8 Error1 = 0;
    uint8 Error2 = 0;
    uint8 Error3 = 0;
    uint8 Error4 = 0;
    uint8 value = 0;
    uint8 Fsm1p4State = 0;
    uint8 Fsm2p2State = 0;

    /* Set HDCP Version with 2 Steps */
    /* Step 1. Disable Data Encryption */
    if(port->content.hdcptx->HdcpModeUpdate == 1)
    {
        GSV2K7_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 0);
        GSV2K7_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, 0);
        port->content.hdcptx->HdcpModeUpdate = port->content.hdcptx->HdcpModeUpdate + 1;
    }
    /* Step 2. Reset HDCP Tx Version */
    else if(port->content.hdcptx->HdcpModeUpdate == 2)
    {
        Gsv2k7SetTxHdcpVersion(port, port->content.hdcptx->HdmiStyle, 1);
        port->content.hdcptx->HdcpModeUpdate = 0;
    }
    /* 0. HDCP Support */
    if((port->content.hdcptx->HdcpSupport == 0) &&
       (port->content.hdcptx->Hdcp2p2SinkSupport == 0))
        return AvOk;
    GSV2K7_TXPHY_get_TX_HDCP1P4_DESIRED(port, &Desire1p4);
    GSV2K7_TX2P2_get_TX_HDCP2P2_DESIRED(port, &Desire2p2);
    if((Desire1p4 != 0) || (Desire2p2 != 0))
        port->content.hdcptx->HdcpEnabled = 1;
    else
        port->content.hdcptx->HdcpEnabled = 0;
    /* 1. Check HDCP 1.4 Errors */
    GSV2K7_TXPHY_get_TX_HDCP1P4_STATE_RB(port,   &Fsm1p4State);
    GSV2K7_TXPHY_get_TX_HDCP1P4_ERROR_FLAG(port, &Error1);
    GSV2K7_TXPHY_get_TX_HDCP1P4_BKSV_READ_FLAG(port, &port->content.hdcptx->BksvReady);
    /* 1.1 HDCP 1.4 Running check */
    if((Fsm1p4State == 4) && (port->content.hdcptx->Authenticated == 0))
        AvUapiOutputDebugMessage("Port Tx %d: Tx is Running HDCP 1.4", (port->index-4));
    /* 2. Check HDCP 2.2 Errors */
#if AvEnableHdcp2p2Feature
    AvHalI2cReadField8(GSV2K7_TX2P2_MAP_ADDR(port), 0x0D, 0xFF, 0, &Error2);
    AvHalI2cReadField8(GSV2K7_TX2P2_MAP_ADDR(port), 0x0E, 0xFF, 0, &Error3);
    AvHalI2cReadField8(GSV2K7_TX2P2_MAP_ADDR(port), 0x14, 0xFF, 0, &Fsm2p2State);
    if(Fsm2p2State == 0x00)
    {
        AvHalI2cReadField8(GSV2K7_TX2P2_MAP_ADDR(port), 0x13, 0xFF, 0, &value);
        if(value == 0x20)
        {
            Error4 = 1;
            AvUapiOutputDebugMessage("Port%d: Unexpected DDC Error", port->index);
        }
    }
    /* 2.1 HDCP 2.2 no response check */
    if((Fsm1p4State == 6) && /* HDCP 2.2 Selected */
       (Desire2p2 == 1) &&    /* HDCP 2.2 Engine Enabled */
       (Fsm2p2State == 0x02)) /* no HDCP 2.2 FSM response */
        Error4 = 1;
    else if((Error2 != 0) || (Error3 != 0))
        Error4 = 1;
    /* 2.2 HDCP 2.2 Running check */
    if(((Fsm2p2State == 0x80) && (port->content.hdcptx->Hdcp2p2TxRunning == 0)) ||
       ((Fsm2p2State != 0x80) && (port->content.hdcptx->Hdcp2p2TxRunning == 1)))
    {
        port->content.hdcptx->Hdcp2p2TxRunning = (Fsm2p2State>>7) & 0x01;
        if(port->content.hdcptx->Hdcp2p2TxRunning == 1)
            AvUapiOutputDebugMessage("Port Tx %d: Tx is Running HDCP 2.2", (port->index-4));
        else
            AvUapiOutputDebugMessage("Port Tx %d: Tx is Stopping HDCP 2.2 by %x", (port->index-4), Fsm2p2State);
    }
    /* 3. Check Capability */
    if(Fsm1p4State == 6)
        port->content.hdcptx->Hdcp2p2SinkSupport = 1;
#endif
    /* 4. Check Authentication */
    if(((Fsm1p4State == 6) && (Fsm2p2State == 0x80)) || (Fsm1p4State == 4))
        port->content.hdcptx->Authenticated = 1;
    else
        port->content.hdcptx->Authenticated = 0;
    /* 5. Error Found */
    if((Error1 != 0) || (Error4 != 0))
    {
        /* 5.1 Add Error Total */
        port->content.hdcptx->HdcpError = port->content.hdcptx->HdcpError + 1;
        /* 5.2 Clear Errors */
        AvUapiTxClearHdcpError(port);
        /* 5.3 When not locked, disable HDCP */
        if(((port->content.tx->Hpd == 0) && (port->content.hdcptx->HdcpEnabled == 1)) ||
           (port->content.tx->Lock.PllLock == 0))
            AvUapiTxDecryptSink(port);
    }
    /* 6. Disable HDCP encryption or Mute */
    switch(port->content.hdcptx->HdmiStyle)
    {
        case AV_HDCP_TX_AUTO:
            port->content.video->Mute.BlkMute = Error1 | Error4;
            port->content.audio->AudioMute    = Error1 | Error4;
            break;
        case AV_HDCP_TX_2P2_ONLY:
            port->content.video->Mute.BlkMute = Error4;
            port->content.audio->AudioMute    = Error4;
            break;
        case AV_HDCP_TX_1P4_ONLY:
            port->content.video->Mute.BlkMute = Error1;
            port->content.audio->AudioMute    = Error1;
            break;
        case AV_HDCP_TX_AUTO_FAIL_OUT:
            if((port->content.hdcptx->HdcpError == 2) &&
               ((Error1 != 0) || (Error4 != 0)))
            {
                Gsv2k7SetTxHdcpVersion(port, AV_HDCP_TX_1P4_FAIL_OUT, 1);
                AvUapiOutputDebugMessage("Port Tx %d: Tx HDCP 2.2 DownGrade to 1.4", (port->index-4));
            }
            else if((port->content.hdcptx->HdcpError >= AvHdcpTxErrorThreshold) &&
                    ((Desire1p4 == 1) || (Desire2p2 == 1)))
            {
                AvUapiTxDecryptSink(port);
                port->content.hdcptx->HdcpError = AvHdcpTxErrorThreshold;
            }
            break;
        case AV_HDCP_TX_1P4_FAIL_OUT:
        case AV_HDCP_TX_2P2_FAIL_OUT:
            if((port->content.hdcptx->HdcpError >= AvHdcpTxErrorThreshold) &&
               ((Desire1p4 == 1) || (Desire2p2 == 1)))
            {
                AvUapiTxDecryptSink(port);
                port->content.hdcptx->HdcpError = AvHdcpTxErrorThreshold;
            }
            break;
        case AV_HDCP_TX_ILLEGAL_NO_HDCP:
            if((Desire1p4 == 1) || (Desire2p2 == 1))
                AvUapiTxDecryptSink(port);
            break;
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxGetBksvReady(pin AvPort *port))
{
    AvRet ret = AvNotAvailable;
#if (Gsv2k7PhyBypass == 0)
    uint8 value;
    GSV2K7_TXPHY_get_TX_HDCP1P4_BKSV_READ_FLAG(port, &port->content.hdcptx->BksvReady);
    /* Running_rb only set 1 after BKSV flag is cleared which makes it an indicator */
    GSV2K7_TXPHY_get_TX_HDCP1P4_RUNNING_RB(port, &value);
    if(port->content.hdcptx->BksvReady == 1)
        ret = AvOk;
    else if(value == 1)
    {
        /* Bksv read but cleared */
        port->content.hdcptx->BksvReady = 1;
        ret = AvInvalidParameter;
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxClearBksvReady(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    GSV2K7_TXPHY_set_TX_HDCP1P4_CLR_BKSV_FLAG(port, 1);
    port->content.hdcptx->BksvReady = 0;

#if AvEnableSimplifyHdcp
    GSV2K7_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 1);
#endif

#if AvEnableDebugHdcp
    AvUapiOutputDebugMessage("Clear Tx Bksv Ready");
#endif

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxClearRxidReady(pin AvPort *port))
{
    AvRet ret = AvOk;

#if (Gsv2k7PhyBypass == 0)
    GSV2K7_TX2P2_set_TX_HDCP2P2_CLEAR_RXID_LIST_READY(port, 1);
    GSV2K7_TX2P2_set_TX_HDCP2P2_CLEAR_RXID_READY(port, 1);

#if AvEnableDebugHdcp
    AvUapiOutputDebugMessage("Clear Tx RxID Ready");
#endif

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxGetSinkHdcpCapability(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 RbState = 0;
    /* 1. Manual Force HDCP 1.4 Support */
    port->content.hdcptx->HdcpSupport = 1;
    /* 2. Check HDCP 2.2 Capability */
    RbState = Gsv2k7TxScdcAction(port, 0, 0x74, 0x50, 0x00);
    if(RbState == 0x04)
    {
        port->content.hdcptx->Hdcp2p2SinkSupport = 1;
        AvUapiOutputDebugMessage("Port %d: Tx is HDCP 2.2 Capable", port->index);
    }
    else
    {
        port->content.hdcptx->Hdcp2p2SinkSupport = 0;
        AvUapiOutputDebugMessage("Port %d: Tx is HDCP 2.2 Incapable", port->index);
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxClearHdcpError(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    if(port->content.hdcptx->Hdcp2p2SinkSupport)
    {
        GSV2K7_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
        GSV2K7_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 1);
        GSV2K7_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 0);
        GSV2K7_TX2P2_set_HDCP2P2_MASTER_CLK_DIV(port, 0x14A); // lower to 30KHz DDC
        GSV2K7_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 0);
    }
    GSV2K7_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
    AvUapiOutputDebugMessage("Port%d: Clear Tx Hdcp Error%d", port->index, port->content.hdcptx->HdcpError);

#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxHdmiClearKsvFifo(pio AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value))
{
    return AvOk;
}

/**
 * @brief  Read Packet Content
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetPacketContent(pio AvPort *port, PacketType Pkt, uint8 *Content))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;

#if AvEnableInternalVideoGen
    if(port->type == VideoGen)
    {
        switch(Pkt)
        {
            case AV_PKT_AV_INFO_FRAME:
                AvMemset(Content, 0, PktSize[AV_PKT_AV_INFO_FRAME]);
                Content[0] = 0x82; /* AV Info  */
                Content[1] = 0x02; /* Ver2     */
                Content[2] = 0x0D; /* Length   */
                Content[4] = ((uint8)(port->content.video->Y))<<5;
                Content[7] = (uint8)(port->content.video->timing.Vic);
                break;
        }
        return AvOk;
    }
#endif

    switch (Pkt)
    {
        case AV_PKT_HDR_PACKET:
            AvHalI2cRdMultiField(GSV2K7_RXINFO2_MAP_ADDR(port), RxPktID[Pkt], 2, (Content+1));
            AvHalI2cRdMultiField(GSV2K7_RXINFO2_MAP_ADDR(port), RxPktPtr[Pkt], PktSize[Pkt]-3, (Content+3));
            Content[0] = 0x87;
            break;
        case AV_PKT_AUDIO_CHANNEL_STATUS:
            GSV2K7_RXDIG_get_RB_RX_AUDIO_LAYOUT(port, &Content[5]);
            AvHalI2cRdMultiField(GSV2K7_RXAUD_MAP_ADDR(port), RxPktID[Pkt], 5, &Content[0]);
            if(port->core.HdmiCore == 0)
            {
                GSV2K7_INT_get_RX1_CSDATA_VALID_INT_ST(port, &value);
                if (value != 0)
                    GSV2K7_INT_set_RX1_CSDATA_VALID_CLEAR(port, 1);
            }
            else if(port->core.HdmiCore == 1)
            {
                GSV2K7_INT_get_RX2_CSDATA_VALID_INT_ST(port, &value);
                if (value != 0)
                    GSV2K7_INT_set_RX2_CSDATA_VALID_CLEAR(port, 1);
            }
            break;
        case AV_PKT_VS_PACKET:
        case AV_PKT_AV_INFO_FRAME:
        case AV_PKT_SPD_PACKET:
        case AV_PKT_AUDIO_INFO_FRAME:
        case AV_PKT_MPEG_PACKET:
        case AV_PKT_ACP_PACKET:
        case AV_PKT_ISRC1_PACKET:
        case AV_PKT_ISRC2_PACKET:
        case AV_PKT_GMD_PACKET:
            AvHalI2cRdMultiField(GSV2K7_RXINFO_MAP_ADDR(port), RxPktID[Pkt], 3, Content);
            AvHalI2cRdMultiField(GSV2K7_RXINFO_MAP_ADDR(port), RxPktPtr[Pkt], PktSize[Pkt]-3, (Content+3));
            break;
        default:
            ret = AvNotSupport;
            break;
    }
    if(Pkt == AV_PKT_AV_INFO_FRAME)
    {
        port->content.rx->Cks.AvCks = Content[3];
        //AvUapiOutputDebugMessage("Port %d: Av Info [4] = %x, [7] = %x", port->index, Content[4], Content[7]);
    }
#endif //(Gsv2k7PhyBypass == 0)

    return ret;
}

/**
 * @brief  Clear Rx Audio Related Interrupt
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    /* AudChanMode */
    if(Intpt->AudChanMode)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_AUDIO_LAYOUT_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_AUDIO_LAYOUT_CLEAR(port,1);
    }
    /* InternMute */
    if(Intpt->InternMute)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_HDMI_AVMUTE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_HDMI_AVMUTE_CLEAR(port,1);
    }
    /* CsDataValid */
    if(Intpt->CsDataValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_CSDATA_VALID_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_CS_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_CSDATA_VALID_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_CS_CLEAR(port,1);
        }
    }
    /* NChange */
    if(Intpt->NChange)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_N_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_N_CLEAR(port,1);
    }
    /* CtsThresh */
    if(Intpt->CtsThresh)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_ACR_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_CTS_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_ACR_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_CTS_CLEAR(port,1);
        }
    }
    /* AudFifoOv */
    if(Intpt->AudFifoOv)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_OVFL_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_OVFL_CLEAR(port,1);
    }
    /* AudFifoUn */
    if(Intpt->AudFifoUn)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_UNDFL_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_UNDFL_CLEAR(port,1);
    }
    /* AudFifoNrOv */
    if(Intpt->AudFifoNrOv)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_ALMOST_OVFL_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_ALMOST_OVFL_CLEAR(port,1);
    }
    /* AudioPktErr */
    if(Intpt->AudioPktErr)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_AUDPKTERR_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_AUDPKTERR_CLEAR(port,1);
    }
    /* AudModeChg */
    if(Intpt->AudModeChg)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_CHANGE_AUDIO_MODE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_CHANGE_AUDIO_MODE_CLEAR(port,1);
    }
    /* AudFifoNrUn */
    if(Intpt->AudFifoNrUn)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_ALMOST_UNDFL_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_ALMOST_UNDFL_CLEAR(port,1);
    }
    /* AudFlatLine */
    if(Intpt->AudFlatLine)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_FLATLINE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_FLATLINE_CLEAR(port,1);
    }
    /* AudSampChg */
    if(Intpt->AudSampChg)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_NEWFREQ_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_NEWFREQ_CLEAR(port,1);
    }
    /* AudPrtyErr */
    if(Intpt->AudPrtyErr)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_PARITY_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_PARITY_CLEAR(port,1);
    }
    /* AudIfValid */
    if(Intpt->AudIfValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_AUD_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_AUD_IF_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_AUD_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_AUD_IF_CLEAR(port,1);
        }
    }
    /* AcpValid */
    if(Intpt->AcpValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_ACP_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_ACP_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_ACP_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_ACP_CLEAR(port,1);
        }
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/**
 * @brief  Clear Rx Video Related Interrupt
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt))
{
    AvRet ret = AvOk;

#if (Gsv2k7PhyBypass == 0)
    /* AvMute */
    if(Intpt->AvMute)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_AVMUTE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_AVMUTE_CLEAR(port,1);
    }
    /* HdmiModeChg */
    if(Intpt->HdmiModeChg)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_HDMI_MODE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_HDMI_MODE_CLEAR(port,1);
    }
    /* DeRegenLck */
    if(Intpt->DeRegenLck)
    {
    }
    /* VsyncLck */
    if(Intpt->VsyncLck)
    {
    }
    /* Vid3dDet */
    if(Intpt->Vid3dDet)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_VIDEO_3D_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_VIDEO_3D_CLEAR(port,1);
    }
    /* NewTmds */
    if(Intpt->NewTmds)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_NEWFREQ_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_NEWFREQ_CLEAR(port,1);
    }
    /* BadTmdsClk */
    if(Intpt->BadTmdsClk)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_CLKCHANGEDETECTED_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_CLKCHANGEDETECTED_CLEAR(port,1);
    }
    /* DeepClrChg */
    if(Intpt->DeepClrChg)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_DEEP_COLOR_CHANGE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_DEEP_COLOR_CHANGE_CLEAR(port,1);
    }
    /* PktErr */
    if(Intpt->PktErr)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKT_PKTERR_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKT_PKTERR_CLEAR(port,1);
    }
    /* AvIfValid */
    if(Intpt->AvIfValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_AVI_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_AVI_IF_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_AVI_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_AVI_IF_CLEAR(port,1);
        }
    }
    /* SpdValid */
    if(Intpt->SpdValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_SPD_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_SPD_IF_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_SPD_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_SPD_IF_CLEAR(port,1);
        }
    }
    /* HdrValid */
    if(Intpt->HdrValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_DRM_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_DRM_IF_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_DRM_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_DRM_IF_CLEAR(port,1);
        }
    }
    /* MsValid */
    if(Intpt->MsValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_MPEG_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_MPEG_IF_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_MPEG_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_MPEG_IF_CLEAR(port,1);
        }
    }
    /* VsValid */
    if(Intpt->VsValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_VS_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_VS_IF_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_VS_IF_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_VS_IF_CLEAR(port,1);
        }
    }
    /* Isrc1Valid */
    if(Intpt->Isrc1Valid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_ISRC1_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_ISRC1_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_ISRC1_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_ISRC1_CLEAR(port,1);
        }
    }
    /* Isrc2Valid */
    if(Intpt->Isrc2Valid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_ISRC2_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_ISRC2_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_ISRC2_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_ISRC2_CLEAR(port,1);
        }
    }
    /* GamutValid */
    if(Intpt->GamutValid)
    {
        if(port->core.HdmiCore == 0)
        {
            GSV2K7_INT_set_RX1_PKTDET_GAMUT_CLEAR(port,1);
            GSV2K7_INT_set_RX1_PKT_NEW_GAMUT_CLEAR(port,1);
        }
        else if(port->core.HdmiCore == 1)
        {
            GSV2K7_INT_set_RX2_PKTDET_GAMUT_CLEAR(port,1);
            GSV2K7_INT_set_RX2_PKT_NEW_GAMUT_CLEAR(port,1);
        }
    }
    /* GcValid */
    if(Intpt->GcValid)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_PKTDET_GCP_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_PKTDET_GCP_CLEAR(port,1);
    }
#endif //(Gsv2k7PhyBypass == 0)

    return ret;
}


/**
 * @brief  Clear Rx Hdcp Related Interrupt
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    if(Intpt->AksvUpdate)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_INT_set_RX1_AKSV_UPDATE_CLEAR(port,1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_INT_set_RX2_AKSV_UPDATE_CLEAR(port,1);
    }
    /* Intpt->Encrypted is cleared in AvUapiRxGetHdcpStatus() */
#endif //(Gsv2k7PhyBypass == 0)

    return ret;
}

/*===========================================================================
 * Enable/Disable packets and info-frames
 *
 * Entry:   InfoFrames  = Packets to enable or disable ORed together
 *          Enable      = TRUE/FALSE to enable/disable packets or InfoFrames
 *
 * Return:  None
 *
 *===========================================================================*/
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxEnableInfoFrames(AvPort* port, uint16 InfoFrames, bool Enable))
{
    AvRet ret = AvOk;
#if ((Gsv2k7MuxMode == 0) && (Gsv2k7PhyBypass == 0))
    uint32 CurrentShadow = 0;
    uint8 PktEn = 0;
    uint8 PktShadow = 0;

    /* 1. Check Shadow and Enable State */
    if(Enable == 1)
        PktEn = 1;
    else if(Enable == 2)
        PktShadow = 1;

    /* 2. Get Current Shadow State */
    GSV2K7_TXPKT_get_TX_PKT_UPDATE(port, &CurrentShadow);

    /* 3. Set Packet Enable Regs */
    if (InfoFrames & AV_BIT_AV_INFO_FRAME)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0002;
        else
            CurrentShadow = CurrentShadow & 0xfffd;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_AVIIF_PKT_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_AUDIO_INFO_FRAME)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0008;
        else
            CurrentShadow = CurrentShadow & 0xfff7;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_AUDIO_INFO_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ACP_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0020;
        else
            CurrentShadow = CurrentShadow & 0xffdf;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_ACP_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_SPD_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0004;
        else
            CurrentShadow = CurrentShadow & 0xfffb;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_SPDIF_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_GMD_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0100;
        else
            CurrentShadow = CurrentShadow & 0xfeff;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_GM_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_MPEG_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0010;
        else
            CurrentShadow = CurrentShadow & 0xffef;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_MPEG_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_VS_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0001;
        else
            CurrentShadow = CurrentShadow & 0xfffe;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_VSIF_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ISRC1_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0040;
        else
            CurrentShadow = CurrentShadow & 0xffbf;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_ISRC_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ISRC2_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0080;
        else
            CurrentShadow = CurrentShadow & 0xff7f;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_ISRC_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_GC_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0200;
        else
            CurrentShadow = CurrentShadow & 0xfdff;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_GC_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ACR_PACKET)
    {
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_N_CTS_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_AUDIO_SAMPLE_PACKET)
    {
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_AUDIO_SAMPLE_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_HDR_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0400;
        else
            CurrentShadow = CurrentShadow & 0xfbff;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_USER0_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_SPARE2_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0800;
        else
            CurrentShadow = CurrentShadow & 0xf7ff;
        if(PktShadow == 0)
            GSV2K7_TXDIG_set_TX_PKT_USER1_ENABLE(port, PktEn);
    }

    /* 4. Set Packet Shadow Regs */
    GSV2K7_TXPKT_set_TX_PKT_UPDATE(port, CurrentShadow);

#endif
    return ret;
}

/**
 * @brief  Configure Packet Content
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetPacketContent(pio AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn))
{
    AvRet ret = AvOk;
#if ((Gsv2k7MuxMode == 0) && (Gsv2k7PhyBypass == 0))
    uint16 InfoFrameBit = 0;
    if((port->index >= 4) && (port->index <= 7))
    {
        /* Map Packet Bit */
        switch(Pkt)
        {
            case AV_PKT_AV_INFO_FRAME:
                InfoFrameBit = AV_BIT_AV_INFO_FRAME;
                break;
            case AV_PKT_HDR_PACKET:
                InfoFrameBit = AV_BIT_HDR_PACKET;
                break;
            case AV_PKT_VS_PACKET:
                InfoFrameBit = AV_BIT_VS_PACKET;
                break;
            case AV_PKT_SPD_PACKET:
                InfoFrameBit = AV_BIT_SPD_PACKET;
                break;
            case AV_PKT_AUDIO_INFO_FRAME:
                InfoFrameBit = AV_BIT_AUDIO_INFO_FRAME;
                break;
            case AV_PKT_AUDIO_CHANNEL_STATUS:
                break;
            case AV_PKT_MPEG_PACKET:
                InfoFrameBit = AV_BIT_MPEG_PACKET;
                break;
            case AV_PKT_ACP_PACKET:
                InfoFrameBit = AV_BIT_ACP_PACKET;
                break;
            case AV_PKT_ISRC1_PACKET:
                InfoFrameBit = AV_BIT_ISRC1_PACKET;
                break;
            case AV_PKT_ISRC2_PACKET:
                InfoFrameBit = AV_BIT_ISRC2_PACKET;
                break;
            case AV_PKT_GMD_PACKET:
                InfoFrameBit = AV_BIT_GMD_PACKET;
                break;
            case AV_PKT_SPARE2_PACKET:
                InfoFrameBit = AV_BIT_SPARE2_PACKET;
                break;
            case AV_PKT_GC_PACKET:
                InfoFrameBit = AV_BIT_GC_PACKET;
                break;
        }
        switch(Pkt)
        {
            case AV_PKT_AV_INFO_FRAME:
                Gsv2k7_TxSendAVInfoFrame(port, Content, PktEn);
                break;
            case AV_PKT_AUDIO_CHANNEL_STATUS:
                GSV2K7_TXDIG_set_TX_AUDIO_CS_OVERRIDE(port, 1);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_FORMAT(port,port->content.audio->Consumer);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_COPYRIGHT(port,        port->content.audio->Copyright);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_FORMAT_EXTRA(port,        port->content.audio->Emphasis);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_MODE(port,       port->content.audio->ClkAccur);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_CATEGORY_CODE(port, port->content.audio->CatCode);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_SOURCE_NUMBER(port, port->content.audio->SrcNum);
                GSV2K7_TXDIG_set_TX_AUDIO_CS_WORD_LENGTH(port,   port->content.audio->WordLen);
                GSV2K7_TXDIG_set_TX_AUDIO_LAYOUT_MAN_VALUE(port, port->content.audio->Layout);
                Gsv2k7_TxSetAudChStatSampFreq(port);
                /* Audio UnMute */
                GSV2K7_TXDIG_set_TX_AUDIO_MUTE(port, 0);
                break;
            case AV_PKT_HDR_PACKET:
            case AV_PKT_SPARE2_PACKET:
                Gsv2k7AvUapiTxEnableInfoFrames(port, InfoFrameBit, 2);
                AvHalI2cWrMultiField(GSV2K7_TXDIG_MAP_ADDR(port), TxPktPtr[Pkt], PktSize[Pkt], Content);
                Gsv2k7AvUapiTxEnableInfoFrames(port, InfoFrameBit, PktEn);
                break;
            case AV_PKT_VS_PACKET:
            case AV_PKT_SPD_PACKET:
            case AV_PKT_AUDIO_INFO_FRAME:
            case AV_PKT_MPEG_PACKET:
            case AV_PKT_ACP_PACKET:
            case AV_PKT_ISRC1_PACKET:
            case AV_PKT_ISRC2_PACKET:
            case AV_PKT_GMD_PACKET:
                Gsv2k7AvUapiTxEnableInfoFrames(port, InfoFrameBit, 2);
                AvHalI2cWrMultiField(GSV2K7_TXPKT_MAP_ADDR(port), TxPktPtr[Pkt], PktSize[Pkt], Content);
                Gsv2k7AvUapiTxEnableInfoFrames(port, InfoFrameBit, PktEn);
                break;
            case AV_PKT_GC_PACKET:
                if(port->content.tx->HdmiMode == 0)
                    Gsv2k7AvUapiTxEnableInfoFrames(port, AV_BIT_GC_PACKET, 0);
                else
                    Gsv2k7AvUapiTxEnableInfoFrames(port, AV_BIT_GC_PACKET, PktEn);
                break;

            default:
                ret = AvNotSupport;
                break;
        }
    }
#endif
    return ret;
}

/**
 * @brief  Get N and CTS Value
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetHdmiAcrInfo (AvPort *port))
{
#if (Gsv2k7PhyBypass == 1)
    return AvOk;
#else
    uint8   value;
    if(port == FindCore2(port))
        GSV2K7_INT_get_RX2_PKTDET_ACR_RAW_ST(port, &value);
    else
        return AvNotAvailable;

    if(value == 1)
    {
        /* Reading dummy CTS is for protection of reading N */
        /* directly reading N will result in 0 */
        /* GSV2K7_RXDIG_get_RX_AUDIO_CTS(port, &port->content.audio->CTSValue); */
        GSV2K7_RXDIG_get_RB_RX_AUDIO_N(port, &port->content.audio->NValue);
        if (port->content.audio->NValue != 0)
        {
            return AvOk;
        }
    }
    return AvError;
#endif //(Gsv2k7PhyBypass == 0)

}

/*********************** Local Functions **************************/

void Gsv2k7DisableRxHpa(pin AvPort *port)
{
    uint8 temp;
    uint32 HdcpFsmState = 0x0002;
    uint8 PreviousValue;
    GSV2K7_PRIM_get_RX_TMDSPLL_EN_OVERRIDE(port, &temp);
    PreviousValue = temp;

#if (Gsv2k7PhyBypass == 0)
    #if (AvAllowHpdLowEdidRead==0)
    switch(port->index)
    {
        case 0: /* rx port A */
            GSV2K7_RXRPT_set_RXA_EDID_EN(port, 0);
            break;
        case 1: /* rx port B */
            GSV2K7_RXRPT_set_RXB_EDID_EN(port, 0);
            break;
        case 2: /* rx port C */
            GSV2K7_RXRPT_set_RXC_EDID_EN(port, 0);
            break;
        case 3: /* rx port D */
            GSV2K7_RXRPT_set_RXD_EDID_EN(port, 0);
            break;
        default:
            return;
    }
    #endif

    #if AvEnableHdcp2p2Feature
    if(port->core.HdmiCore == 1)
    {
        GSV2K7_RX2P2_get_RX_HDCP2P2_FSM_STATE(port,       &HdcpFsmState);
        if(HdcpFsmState != 0x002)
        {
            GSV2K7_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(port, 1);
            GSV2K7_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(port, 0);
        }
    }
    #endif
    if(port->content.hdcp->HdcpNeeded == AV_HDCP_RX_NOT_SUPPORT)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_PRIM_set_RX1_HDCP_EN(port, 0);
        else if(port->core.HdmiCore == 1)
            GSV2K7_PRIM_set_RX2_HDCP_EN(port, 0);
    }
#endif //(Gsv2k7PhyBypass == 0)

    switch(port->index)
    {
        case 0: /* rx port 0 */
            temp = temp&0xFE;
            break;
        case 1: /* rx port 1 */
            temp = temp&0xFD;
            break;
        case 2:
            temp = temp&0xFB;
            break;
        case 3:
            temp = temp&0xF7;
            break;
        default:
            return;
    }
    if(temp != PreviousValue)
    {
        /* HDMI Interface Power Down */
        GSV2K7_PRIM_set_RX_HPD_MAN_VALUE(port, temp);        /* HPA=0 */
        GSV2K7_PRIM_set_RX_RTERM_EN(port, temp);             /* RTERM = 0 */
        GSV2K7_PRIM_set_RX_TMDSPLL_EN_OVERRIDE(port, temp);  /* CLK=0 */
        GSV2K7_PRIM_set_RX_CHANNEL_EN_OVERRIDE(port, temp);  /* CHANNEL=0 */
        AvUapiOutputDebugMessage("Port %d: Disable Rx Hpa %x...", port->index, temp);
    }

    return;
}

void Gsv2k7EnableRxHpa(pin AvPort *port)
{
    uint8 temp;
    uint8 PreviousValue;
    GSV2K7_PRIM_get_RX_TMDSPLL_EN_OVERRIDE(port, &temp);
    PreviousValue = temp;

#if (Gsv2k7PhyBypass == 0)
    #if (AvAllowHpdLowEdidRead==0)
    switch(port->index)
    {
        case 0: /* rx port 0 */
            GSV2K7_RXRPT_set_RXA_EDID_EN(port, 1);
            break;
        case 1: /* rx port 1 */
            GSV2K7_RXRPT_set_RXB_EDID_EN(port, 1);
            break;
        case 2: /* rx port 2 */
            GSV2K7_RXRPT_set_RXC_EDID_EN(port, 1);
            break;
        case 3: /* rx port 3 */
            GSV2K7_RXRPT_set_RXD_EDID_EN(port, 1);
            break;
        default:
            return;
    }
    #endif

    if(port->content.hdcp->HdcpNeeded != AV_HDCP_RX_NOT_SUPPORT)
    {
        if(port->core.HdmiCore == 0)
            GSV2K7_PRIM_set_RX1_HDCP_EN(port, 1);
        else if(port->core.HdmiCore == 1)
            GSV2K7_PRIM_set_RX2_HDCP_EN(port, 1);
    }
#endif //(Gsv2k7PhyBypass == 0)

    /* HDMI Interface Power Up */
    switch(port->index)
    {
        case 0: /* rx port A */
            temp = temp|0x01;
            break;
        case 1: /* rx port B */
            temp = temp|0x02;
            break;
        case 2: /* rx port C */
            temp = temp|0x04;
            break;
        case 3: /* rx port D */
            temp = temp|0x08;
            break;
        default:
            return;
    }
    if(temp != PreviousValue)
    {
        GSV2K7_PRIM_set_RX_RTERM_EN(port, temp);             /* RTERM = 1 */
#if (AvAllowHpdLowEdidRead == 1)
        if(port->core.HdmiCore != -1)
        {
            GSV2K7_RX2P2_set_RX_DISABLE_ECC_CHECK(port, 1);
            AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port), 0xD9, 0xFF, 0x0, 0x00);
            RxScdcFsm(port)[port->index] = 7;
            AvUapiOutputDebugMessage("Port %d: Auto SCDC", port->index);
        }
        if(RxScdcNewEdid(port) == 1)
        {
            GSV2K7_PRIM_set_RX_HPD_MAN_VALUE(port, 0xf);
            RxScdcNewEdid(port) = 0;
        }
        else
#endif
            GSV2K7_PRIM_set_RX_HPD_MAN_VALUE(port, temp);        /* HPA=1 */
        GSV2K7_PRIM_set_RX_TMDSPLL_EN_OVERRIDE(port, temp);  /* CLK=1 */
        GSV2K7_PRIM_set_RX_CHANNEL_EN_OVERRIDE(port, temp);  /* CHANNEL=1 */
        AvUapiOutputDebugMessage("Port %d: Enable Rx Hpa %x...", port->index, temp);
    }
}

void Gsv2k7GetRx5VStatus(pin AvPort *port)
{
    uint8 value = 0;
    switch(port->index)
    {
        case 0: /* rx port 0 */
            GSV2K7_INT_get_RXA_CABLE_DETECT_RAW_ST(port, &value);
            break;
        case 1: /* rx port 1 */
            GSV2K7_INT_get_RXB_CABLE_DETECT_RAW_ST(port, &value);
            break;
        case 2: /* rx port 2 */
            GSV2K7_INT_get_RXC_CABLE_DETECT_RAW_ST(port, &value);
            break;
        case 3: /* rx port 3 */
            GSV2K7_INT_get_RXD_CABLE_DETECT_RAW_ST(port, &value);
            break;
        default:
            break;
    }
    port->content.rx->Input5V = value;
}

void Gsv2k7ToggleTmdsOut(AvPort *port, uint8 Enable)
{
#if (Gsv2k7PhyBypass == 1)
    uint8 BulkWrite = 0xff;
#else
    uint8 BulkWrite = 0x70;
    if(Enable == 1)
    {
        if(port->type == HdmiTx)
            BulkWrite = 0xff;
    }
    GSV2K7_TXPHY_set_TX_EN_STERM_CLOCK(port, Enable);
    GSV2K7_TXPHY_set_TX_EN_STERM_LANE0(port, Enable);
    GSV2K7_TXPHY_set_TX_EN_STERM_LANE1(port, Enable);
    GSV2K7_TXPHY_set_TX_EN_STERM_LANE2(port, Enable);
#endif

    /* TMDS CLK, Lane0, Lane1, Lane2, Ser0, Ser1, Ser2 */
    //GSV2K7_TXPHY_set_TX_TMDS_CLK_DRIVER_ENABLE(port, Enable);
    AvHalI2cWriteField8(GSV2K7_TXPHY_MAP_ADDR(port), 0xF2, 0xFF, 0x0, BulkWrite);
    if(Enable == 1)
        AvUapiOutputDebugFsm("Port %d: Set TMDS to 1...", port->index);
        //AvUapiOutputDebugMessage("Port %d: Set TMDS to 1...", port->index);
    else
        AvUapiOutputDebugFsm("Port %d: Set TMDS to 0...", port->index);
        //AvUapiOutputDebugMessage("Port %d: Set TMDS to 0...", port->index);
}

void Gsv2k7SetTxHdcpVersion(pin AvPort *port, HdcpTxSupportType HdmiStyle, uint8 ForceUpdate)
{
#if (Gsv2k7PhyBypass == 0)
    uint8 HdcpMan = 0;
    uint8 HdcpValue = 0;
    uint8 PrevHdcpMan = 0;
    uint8 PrevHdcpValue = 0;
    /* Set Tx HDCP Version */
    switch(HdmiStyle)
    {
        case AV_HDCP_TX_1P4_ONLY:
        case AV_HDCP_TX_1P4_FAIL_OUT:
            HdcpMan   = 1;
            HdcpValue = 0;
            break;
        case AV_HDCP_TX_2P2_ONLY:
        case AV_HDCP_TX_2P2_FAIL_OUT:
            HdcpMan   = 1;
            HdcpValue = 1;
            break;
    }
    switch(port->index)
    {
        case 5:
            GSV2K7_PRIM_get_TXB_CFG_TO_HDCP2P2(port,  &PrevHdcpValue);
            GSV2K7_PRIM_get_TXB_HDCP_CFG_MAN_EN(port, &PrevHdcpMan);
            break;
    }
    if((PrevHdcpValue != HdcpValue) || (PrevHdcpMan != HdcpMan) || (ForceUpdate == 1))
    {
        GSV2K7_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 0);
        GSV2K7_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, 0);
        switch(port->index)
        {
            case 5:
                GSV2K7_PRIM_set_TXB_HDCP_EN(port, 0);
                GSV2K7_PRIM_set_TXB_CFG_TO_HDCP2P2(port,  HdcpValue);
                GSV2K7_PRIM_set_TXB_HDCP_CFG_MAN_EN(port, HdcpMan);
                GSV2K7_PRIM_set_TXB_HDCP_EN(port, 1);
                break;
        }
        Gsv2k7ToggleTxHpd(port);
    }
    if(HdmiStyle == AV_HDCP_TX_ILLEGAL_NO_HDCP)
        HdcpValue = 0;
    else
        HdcpValue = 1;
    GSV2K7_TXPHY_set_TX_HDCP1P4_ENC_EN(port, HdcpValue);
    GSV2K7_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, HdcpValue);
#endif //(Gsv2k7PhyBypass == 0)

}

void Gsv2k7ToggleTxHpd(pin AvPort *port)
{
#if (Gsv2k7PhyBypass == 0)
    AvUapiOutputDebugFsm("Port %d: Toggle Tx HPD...", port->index);
    GSV2K7_TXPHY_set_TX_HPD_SRC_SEL(port, 3); /* Always Set HPD to 1 */
    GSV2K7_TXPHY_set_TX_HPD_MAN_VALUE(port, 0); /* Toggle HPD to 0 */
    GSV2K7_TXPHY_set_TX_HPD_MAN_VALUE(port, 1); /* Toggle HPD to 1 */
    GSV2K7_TXPHY_set_TX_HPD_SRC_SEL(port, 0); /* Normal HPD Setting */
#endif //(Gsv2k7PhyBypass == 0)

}

void Gsv2k7UpdateRxCdrBandWidth(pin AvPort *port)
{
    uint8 value;
    uint32 PhyPageAddr;
    /* Step 1. Read Ser Div */
    switch(port->index)
    {
        case 0:
            GSV2K7_PLL_get_RB_RXA_PLL_SER_DIV(port, &value);
            break;
        case 1:
            GSV2K7_PLL_get_RB_RXB_PLL_SER_DIV(port, &value);
            break;
        case 2:
            GSV2K7_PLL2_get_RB_RXC_PLL_SER_DIV(port, &value);
            break;
        case 3:
            GSV2K7_PLL2_get_RB_RXD_PLL_SER_DIV(port, &value);
            break;
    }
    /* Step 2. Set CDR bandwidth */
    switch(value)
    {
        case 0:
            value = 0x03;
            break;
        case 1:
            value = 0x03; // R1 = 0x01
            break;
        default:
            value = 0x03; // R1 = 0x00
            break;
    }
    PhyPageAddr = GSV2K7_RXLN0_MAP_ADDR(port);
#if  AvEnableRxCdrCedCheck
    if((port->content.video->info.LaneFreq > 250) &&
       (port->content.video->info.LaneFreq < 320))
    {
        AvHalI2cWriteField8(PhyPageAddr,0x0C,0xFF,0,0x03); //VGA To Max
        value = 0x00;
    }
#endif
    AvHalI2cWriteField8(PhyPageAddr,0x03,0xFF,0,value); //set cdr bandwith
}

uint8 Gsv2k7TxScdcAction(pin AvPort *port, uint8 WriteEnable, uint8 DevAddr, uint8 RegAddr, uint8 Value)
{
    uint8 round = 0;
#if (Gsv2k7PhyBypass == 0)
    uint8 PageAddr;
    uint8 DoneFlag;
    /* 1. Write Page Address and Rega Address, Value */
    if(WriteEnable)
        PageAddr = DevAddr & 0xFE;
    else
        PageAddr = DevAddr | 0x01;

    GSV2K7_TXPHY_set_RX_DDC_UDP_DEV_ADDR(port, PageAddr);
    GSV2K7_TXPHY_set_RX_DDC_UDP_SUB_ADDR(port, RegAddr);
    if(WriteEnable)
        GSV2K7_TXPHY_set_RX_DDC_UDP_WRITE_DATA(port, Value);
    GSV2K7_TXPHY_set_RX_DDC_UDP_ACCESS_REQ(port, 1);
    /* 3. Wait For Complete */
    for(round=0;round<0x18;round++)
    {
        GSV2K7_TXPHY_get_RX_DDC_UDP_DONE(port, &DoneFlag);
        if(DoneFlag == 1)
            break;
    }
    /* 4. Handle Return Value */
    if((WriteEnable == 1) || (DoneFlag == 0))
        round = 0xff;
    else
    {
        GSV2K7_TXPHY_get_RX_DDC_UDP_READ_DATA(port, &round);
    }
#endif //(Gsv2k7PhyBypass == 0)

    return round;
}

uint8 Gsv2k7TxDDCError(pin AvPort *port)
{
#if (Gsv2k7PhyBypass == 0)
    uint32 value32 = 0;
    uint8  value   = 0;
    GSV2K7_TXPHY_set_RD_CPU_PC_TRIGGER(port, 0);
    GSV2K7_TXPHY_set_RD_CPU_PC_TRIGGER(port, 1);
    GSV2K7_TXPHY_get_RB_CPU_PC_CAPTURED(port, &value32);
    GSV2K7_TXPHY_get_TX_EDID_DDC_ERROR_RB(port, &value);
    if(value == 1)
        GSV2K7_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
    if((value == 1) || (value32 == 0xf0))
        return 1;
    else
#endif //(Gsv2k7PhyBypass == 0)
        return 0;
}

void Gsv2k7ManualBksvRead(pin AvPort *port)
{
#if AvEnableHdcp1p4BksvCheck
    uint8 i = 0;
    uint8 j = 0;
    uint8 OneNumber;
    uint8 RdBksv[5];

    /* 0. Disable HDCP 1.4 and 2.2 FSM */
    GSV2K7_TXPHY_set_TX_HDCP1P4_DESIRED(port, 0);
    GSV2K7_TX2P2_set_TX_HDCP2P2_DESIRED(port, 0);
    /* 1. Read Sink Bksv */
    for(i=0;i<5;i++)
        RdBksv[i] = Gsv2k7TxScdcAction(port, 0, 0x74, i, 0x00);
    AvUapiOutputDebugMessage("Port %d: BKSV = %x,%x,%x,%x,%x", port->index,
                             RdBksv[0], RdBksv[1], RdBksv[2], RdBksv[3], RdBksv[4]);
    /* 2. Check HDCP 2.2 Capability */
    j = Gsv2k7TxScdcAction(port, 0, 0x74, 0x50, 0x00);
    if(j == 0x04)
        port->content.hdcptx->Hdcp2p2SinkSupport = 1;
    else
        port->content.hdcptx->Hdcp2p2SinkSupport = 0;
    /* 3. Check Bksv validity */
    OneNumber = 0;
    for(i=0;i<5;i++)
        for(j=0;j<8;j++)
            if(((RdBksv[i]>>j) & 0x01) == 0x01)
                OneNumber++;
    /* 4. Judge Tx HDCP Support */
    if(OneNumber == 20)
    {
        port->content.hdcptx->HdcpSupport = 1;
    }
    else
    {
        port->content.hdcptx->HdcpSupport = 0;
        port->content.hdcptx->Hdcp2p2SinkSupport = 0;
    }
    /* 5. Report HDCP Capability */
    if(port->content.hdcptx->HdcpSupport == 1)
        AvUapiOutputDebugMessage("Port Tx %d: Tx is HDCP 1.4 Capable", (port->index-3));
    if(port->content.hdcptx->Hdcp2p2SinkSupport == 1)
        AvUapiOutputDebugMessage("Port Tx %d: Tx is HDCP 2.2 Capable", (port->index-3));
#endif
}

/* Audio Part */

void Gsv2k7_TxSetAudioInterface (AvPort* port)
{
#if (Gsv2k7PhyBypass == 0)
    uint8 I2SBlockEnable = 0;
    uint8 SpdifBlockEnable = 0;
    uint8 DsdBlockEnable = 0;
    uint8 EncodeStyle = 0;
    uint8 NumCh = 8;   /* default 8 channels */
    uint8 NumPins = 3; /* default 4 pins */
    uint8 NumSlot = 2; /* default 2 slots */
    /* Default 128Fs MCLK */
    GSV2K7_TXDIG_set_TX_AUDIO_MCLK_FS_RATIO(port, port->content.audio->AudMclkRatio);

    switch(port->content.audio->AudType)
    {
        case AV_AUD_TYPE_AUTO:
        case AV_AUD_TYPE_ASP:
            I2SBlockEnable = 0xf;
            break;
        case AV_AUD_TYPE_SPDIF:
            SpdifBlockEnable = 1;
            break;
        case AV_AUD_TYPE_HBR:
            I2SBlockEnable = 0xf;
            EncodeStyle = 0; /* Style = 0 for HBR's SPDIF input mode */
            GSV2K7_TXDIG_set_TX_AUDIO_I2S_FORMAT(port, 0);
            GSV2K7_TXDIG_set_TX_AUDIO_PAPB_SYNC(port, 0);
            GSV2K7_TXDIG_set_TX_AUDIO_PAPB_SYNC(port, 1);
            break;
        case AV_AUD_TYPE_DSD:
            NumCh = port->content.audio->ChanNum;
            if(port->content.audio->Layout == 1)
                NumPins = 3;
            else
                NumPins = 1;
            NumSlot = NumCh/(NumPins+1);
            switch(NumCh)
            {
                case 2:
                    DsdBlockEnable = 0x03;
                    break;
                case 4:
                    DsdBlockEnable = 0x0f;
                    break;
                case 6:
                    DsdBlockEnable = 0x3f;
                    break;
                default:
                    DsdBlockEnable = 0xff;
                    break;
            }
            break;
        case AV_AUD_TYPE_DST:
            DsdBlockEnable = 0x0;
            break;
        case AV_AUD_TYPE_UNKNOWN:
        default:
            break;
    }
    /* I2S default */
    GSV2K7_TXDIG_set_TX_AUDIO_I2S_BLOCK_ENABLE(port, I2SBlockEnable);
    /* currently disabled, only useful when AP to Tx */
    GSV2K7_TXDIG_set_TX_AUDIO_SPDIF_ENABLE(port, SpdifBlockEnable);
    /* DSD and DST */
    GSV2K7_TXDIG_set_TDM_DESER_NUM_PINS(port, NumPins);
    GSV2K7_TXDIG_set_TDM_DESER_NUM_SLOT(port, NumSlot);
    GSV2K7_TXDIG_set_TDM_DESER_NUM_CH(port, NumCh);
    GSV2K7_TXDIG_set_DSD_EN(port, DsdBlockEnable);
    /* Encode Style */
    GSV2K7_TXDIG_set_TX_AUDIO_ENCODE_STYLE(port, EncodeStyle);
#endif //(Gsv2k7PhyBypass == 0)

    return;
}

void Gsv2k7_TxSetAudChStatSampFreq (AvPort* port)
{
#if (Gsv2k7PhyBypass == 0)
    uchar i;
    AvAudioSampleFreq SampFreq = port->content.audio->SampFreq;

    i = LookupValue8 ((uchar *)AudioSfTable, (uchar)SampFreq, 0xff, 2);
    if (AudioSfTable[i] != 0xff)
    {
        /*==========================
         * If from register
         *=========================*/
        if (SampFreq != AV_AUD_FS_FROM_STRM)
        {
            GSV2K7_TXDIG_set_TX_AUDIO_I2S_MAN_SF(port, AudioSfTable[i+1]);
        }
        else
        {
            GSV2K7_TXDIG_set_TX_AUDIO_I2S_MAN_SF(port, 9);
        }
    }
#endif //(Gsv2k7PhyBypass == 0)
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetAudNValue(AvPort* port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    if((port->index >= 4) && (port->index <= 7))
    {
        //GSV2K7_TXDIG_set_TX_AUDIO_MAN_N(port, port->content.audio->NValue);
        value = (port->content.audio->NValue & 0x000ff) >> 0;
        AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x05, 0xFF, 0x0, value);
        value = (port->content.audio->NValue & 0x0ff00) >> 8;
        AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x06, 0xFF, 0x0, value);
        value = (port->content.audio->NValue & 0x0ff00) >> 16;
        AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port), 0x07, 0x0F, 0x0, value);
        /* reset audio fifo */
        GSV2K7_TXDIG_set_TX_AUDIOFIFO_RESET(port, 1);
        GSV2K7_TXDIG_set_TX_AUDIOFIFO_RESET(port, 0);
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

AvRet Gsv2k7_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn)
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    AvVideoY InY = AV_Y2Y1Y0_RGB;
    AvVideoY OutY = AV_Y2Y1Y0_RGB;
    uint8 Value;
    uint8 Packet[31];
    AvPort *FromPort = NULL;

    if((port->index >= 4) && (port->index <= 7))
    {
        AvMemcpy(Packet, AviPkt, PktSize[AV_PKT_AV_INFO_FRAME]);
        /* Step 1. Configure Color Space */
        if(port->content.tx->HdmiMode == 0)
        {
            InY = port->content.video->Y;
            OutY = AV_Y2Y1Y0_RGB;
            if((port->content.video->InCs == AV_CS_LIM_RGB) ||
               (port->content.video->InCs == AV_CS_DEFAULT_RGB) ||
               (port->content.video->InCs == AV_CS_RGB) ||
               (port->content.video->InCs == AV_CS_AUTO))
            {
                port->content.video->OutCs = port->content.video->InCs;
            }
            else
            {
                port->content.video->OutCs = AV_CS_LIM_RGB;
            }
        }
        else
        {
            /* decide input color space */
            if((port->content.video->InCs == AV_CS_YUV_601)  ||
               (port->content.video->InCs == AV_CS_YUV_709)  ||
               (port->content.video->InCs == AV_CS_YCC_601)  ||
               (port->content.video->InCs == AV_CS_YCC_709)  ||
               (port->content.video->InCs == AV_CS_SYCC_601) ||
               (port->content.video->InCs == AV_CS_ADOBE_YCC_601) ||
               (port->content.video->InCs == AV_CS_BT2020_YCC)    ||
               (port->content.video->InCs == AV_CS_BT2020_RGB)    ||
               (port->content.video->InCs == AV_CS_LIM_YUV_601)  ||
               (port->content.video->InCs == AV_CS_LIM_YUV_709)  ||
               (port->content.video->InCs == AV_CS_LIM_YCC_601)  ||
               (port->content.video->InCs == AV_CS_LIM_YCC_709)  ||
               (port->content.video->InCs == AV_CS_LIM_SYCC_601) ||
               (port->content.video->InCs == AV_CS_LIM_ADOBE_YCC_601) ||
               (port->content.video->InCs == AV_CS_LIM_BT2020_RGB) ||
               (port->content.video->InCs == AV_CS_LIM_BT2020_YCC))
            {
                InY = AV_Y2Y1Y0_YCBCR_444;
            }
            /* decide output color space */
            OutY = port->content.video->Y;
            if(port->content.video->Y != AV_Y2Y1Y0_YCBCR_420)
            {
                if((port->content.video->OutCs == AV_CS_YUV_601)  ||
                   (port->content.video->OutCs == AV_CS_YUV_709)  ||
                   (port->content.video->OutCs == AV_CS_YCC_601)  ||
                   (port->content.video->OutCs == AV_CS_YCC_709)  ||
                   (port->content.video->OutCs == AV_CS_SYCC_601) ||
                   (port->content.video->OutCs == AV_CS_ADOBE_YCC_601) ||
                   (port->content.video->OutCs == AV_CS_BT2020_YCC)    ||
                   (port->content.video->OutCs == AV_CS_BT2020_RGB)    ||
                   (port->content.video->OutCs == AV_CS_LIM_YUV_601)  ||
                   (port->content.video->OutCs == AV_CS_LIM_YUV_709)  ||
                   (port->content.video->OutCs == AV_CS_LIM_YCC_601)  ||
                   (port->content.video->OutCs == AV_CS_LIM_YCC_709)  ||
                   (port->content.video->OutCs == AV_CS_LIM_SYCC_601) ||
                   (port->content.video->OutCs == AV_CS_LIM_ADOBE_YCC_601) ||
                   (port->content.video->OutCs == AV_CS_LIM_BT2020_RGB) ||
                   (port->content.video->OutCs == AV_CS_LIM_BT2020_YCC))
                {
                    if(port->content.video->Y == AV_Y2Y1Y0_RGB)
                        OutY = AV_Y2Y1Y0_YCBCR_444;
                }
                else
                    OutY = AV_Y2Y1Y0_RGB;
            }
            /* Step 2. Color Range */
            /* 2.0.1 YUV Full Range */
            if(OutY != AV_Y2Y1Y0_RGB)
            {
                SET_AVIF_Q(Packet,0);
                Value = (port->content.video->OutCs)>>6;
                SET_AVIF_YQ(Packet,Value);
            }
            /* 2.0.2 RGB Full Range */
            else
            {
                SET_AVIF_YQ(Packet,0);
                Value = (port->content.video->OutCs)>>6;
                SET_AVIF_Q(Packet,Value);
            }
        }
        /* Step 2.1 For Black Mute Choose Correct Color */
        if(InY == AV_Y2Y1Y0_RGB)
            GSV2K7_TXDIG_set_TX_VIDEO_INPUT_CS(port, 0);
        else
            GSV2K7_TXDIG_set_TX_VIDEO_INPUT_CS(port, 1);
        /* Step 2.2 Scan Info Processing */
        FromPort = (AvPort*)(port->content.RouteVideoFromPort);
        if(FromPort->type == VideoScaler)
        {
            Value = GET_AVIF_SC(Packet);
            /* Remove Overscan after downscaling */
            if(Value == 0x01)
                SET_AVIF_SC(Packet,0);
        }
        /* Step 2.3 CSC Setting */
        Gsv2k7_TxSetCSC(port);
        /* Step 3. Aspect Ratio */
        if(port->content.video->timing.Vic <= 107)
        {
            Value = port->content.video->timing.Vic;
            port->content.video->AspectRatio = ARTable[Value];
        }
        switch (port->content.video->AspectRatio)
        {
            case 1:
                SET_AVIF_M(Packet, 1);
                GSV2K7_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, 0);
                break;
            case 2:
                SET_AVIF_M(Packet, 2);
                GSV2K7_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, 1);
                break;
            default:
                SET_AVIF_M(Packet, 0);
                GSV2K7_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, 0);
                break;
        }
        /* Step 4. Pixel Repeat */
        Gsv2k7_TxSetManualPixelRepeat(port);
        /* Step 5. Set Y */
        FromPort = (AvPort*)(port->content.RouteVideoFromPort);
        if((FromPort->type == HdmiRx) &&
           (FromPort->content.video->Y == AV_Y2Y1Y0_YCBCR_422))
            Value = 1;
        else
            Value = 0;
        GSV2K7_TXDIG_set_TX_VIDEO_INPUT_FORMAT(port, Value);
        switch (OutY)
        {
            case AV_Y2Y1Y0_RGB:
                SET_AVIF_Y(Packet, 0);
                SET_AVIF_C(Packet, 0);
                Value = 0;
                break;
            case AV_Y2Y1Y0_YCBCR_422:
                SET_AVIF_Y(Packet, 1);
                Value = 3;
                break;
            case AV_Y2Y1Y0_YCBCR_444:
                SET_AVIF_Y(Packet, 2);
                Value = 1;
                break;
            case AV_Y2Y1Y0_YCBCR_420:
                SET_AVIF_Y(Packet, 3);
                Value = 1;
                break;
            default:
                SET_AVIF_Y(Packet, 3);
                Value = 0; /* This is an incorrect setting */
                break;
        }
        GSV2K7_TXDIG_set_TX_VIDEO_OUTPUT_FORMAT(port, Value);
        /* Step 6. Set AVI Infoframe Content  */
        GSV2K7_TXPKT_set_AVI_IF_TYPE(port, 0x82);
        GSV2K7_TXPKT_set_AVI_IF_VER(port, Packet[1]);
        GSV2K7_TXPKT_set_AVI_IF_LEN(port, Packet[2]);
        Gsv2k7AvUapiTxEnableInfoFrames(port, AV_BIT_AV_INFO_FRAME, 2); /* enable AVI infoframe */
        AvHalI2cWrMultiField(GSV2K7_TXPKT_MAP_ADDR(port),
                             TxPktPtr[AV_PKT_AV_INFO_FRAME],
                             PktSize[AV_PKT_AV_INFO_FRAME]-3,
                             Packet+3);
        Gsv2k7AvUapiTxEnableInfoFrames(port, AV_BIT_AV_INFO_FRAME, PktEn); /* enable AVI infoframe */
        if(PktEn)
        {
            AvUapiOutputDebugFsm("Port%d: Y1Y0 = %d",port->index, port->content.video->Y);
        }
    }
#endif //(Gsv2k7PhyBypass == 0)

    return ret;
}

AvRet Gsv2k7_TxSetManualPixelRepeat(AvPort *port)
{
    AvRet ret = AvInvalidParameter;
#if (Gsv2k7PhyBypass == 0)
    uint8 Factor = port->content.video->ClockMultiplyFactor;
    uint8 PrValue = port->content.video->PixelRepeatValue;
    if ((Factor > 4) || (PrValue > 3))
    {
        return ret;
    }
    GSV2K7_TXDIG_set_TX_VIDEO_TMDS_CLK_RATIO_MAN(port, PrValue);
    GSV2K7_TXDIG_set_TX_VIDEO_PR_TO_RX_MAN(port, PrValue);
    GSV2K7_TXDIG_set_TX_VIDEO_PR_MODE(port, 2);
    ret = AvOk;
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

AvRet Gsv2k7_TxSetCSC (AvPort *port)
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 i;
    AvVideoCs InCs  = Gsv2k7ColorCsMapping(port->content.video->InCs);
    AvVideoCs OutCs = Gsv2k7ColorCsMapping(port->content.video->OutCs);

    if ((InCs == OutCs) || (InCs == AV_CS_AUTO) || (OutCs == AV_CS_AUTO) ||
        (port->content.video->Y == AV_Y2Y1Y0_YCBCR_420))
    {
        GSV2K7_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, 0);
        ret = AvOk;
    }
    else
    {
        for (i=0; CscTables[i].ConvTable; i++)
        {
            if ((CscTables[i].InCs  == InCs) &&
                (CscTables[i].OutCs == OutCs))
            {
                AvHalI2cWrMultiField(GSV2K7_TXDIG_MAP_ADDR(port), 0x2C, 24, (uint8 *)(CscTables[i].ConvTable));
                AvHalI2cWriteField8(GSV2K7_TXDIG_MAP_ADDR(port),0x2B,0xFF,0,(CscTables[i].ConvTable[0] & 0xE0));
                ret = AvOk;
                break;
            }
        }
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetAvMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    GSV2K7_RXDIG_get_RB_RX_AVMUTE_STATE(port, &(port->content.video->Mute.AvMute));
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetAvMute(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 1)
#elif (Gsv2k7MuxMode == 1)
    GSV2K7_TXDIG_set_TX_MUX_MODE_AVMUTE_MAN(port, 0);
    GSV2K7_TXDIG_set_TX_MUX_MODE_AVMUTE_MAN_EN(port, 0);
#else
    if((port->index >= 4) && (port->index <= 5))
    {
        if(port->content.video->Mute.AvMute == 1)
        {
            GSV2K7_TXDIG_set_TX_GC_CLEAR_AVMUTE(port, 0);
            GSV2K7_TXDIG_set_TX_GC_SET_AVMUTE(port, 1);
            AvUapiOutputDebugMessage("Port %d: AVMUTE Enable...", port->index);
        }
        else
        {
            GSV2K7_TXDIG_set_TX_GC_SET_AVMUTE(port, 0);
            GSV2K7_TXDIG_set_TX_GC_CLEAR_AVMUTE(port, 1);
            AvUapiOutputDebugMessage("Port %d: AVMUTE Disable...", port->index);
        }
    }
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetHdmiModeSupport(pio AvPort *port))
{
    AvRet ret = AvOk;
    GSV2K7_RXDIG_get_RB_RX_HDMI_MODE(port, &(port->content.rx->HdmiMode));
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetHdmiModeSupport(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    if(port->content.tx->HdmiMode == 1)
    {
        GSV2K7_TXDIG_set_TX_HDMI_MODE_OVERRIDE(port, 1);
        GSV2K7_TXDIG_set_TX_HDMI_MODE_MAN(port, 1);
    }
    else
    {
        GSV2K7_TXDIG_set_TX_HDMI_MODE_OVERRIDE(port, 1);
        GSV2K7_TXDIG_set_TX_HDMI_MODE_MAN(port, 0);
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetFeatureSupport(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC)
        value = 1;
    else
        value = 0;
    /* Only Enable Tx SCDC, no RR mode */
    GSV2K7_TXPHY_set_TX_SCDC_EN(port, 0);
    GSV2K7_TXPHY_set_RX_SCDC_PRESENT(port, value);
    /* Disable Tx RR, only use polling to check Scdc Flag */
    GSV2K7_TXPHY_set_TX_RR_CAPABLE(port, 1);
    GSV2K7_TXPHY_set_RX_RR_CAPABLE(port, 0);
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetVideoLock(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 tmds_pll_lock_flag;
    uint8 de_regen_lock_flag;
    uint8 v_sync_lock_flag;
    /* Step 1. Check the lock status */
#if (Gsv2k7PhyBypass == 1)
    switch(port->index)
    {
        case 0:
            GSV2K7_INT_get_RXA_TMDSPLL_LOCK_DET_RAW_ST(port, &tmds_pll_lock_flag);
            break;
        case 1:
            GSV2K7_INT_get_RXB_TMDSPLL_LOCK_DET_RAW_ST(port, &tmds_pll_lock_flag);
            break;
        case 2:
            GSV2K7_INT_get_RXC_TMDSPLL_LOCK_DET_RAW_ST(port, &tmds_pll_lock_flag);
            break;
        case 3:
            GSV2K7_INT_get_RXD_TMDSPLL_LOCK_DET_RAW_ST(port, &tmds_pll_lock_flag);
            break;
    }
    port->content.rx->Lock.PllLock = tmds_pll_lock_flag;
#else
    GSV2K7_RXDIG_get_RB_RX_TMDS_PLL_LOCKED (port, &(port->content.rx->Lock.PllLock));
    GSV2K7_RXDIG_get_RB_RX_HS_LOCKED(port, &(port->content.rx->Lock.DeRegenLock));
    GSV2K7_RXDIG_get_RB_RX_VS_LOCKED(port, &(port->content.rx->Lock.VSyncLock));
#endif

#if (AvAllowHpdLowEdidRead == 0)
    if(port->content.rx->Lock.PllLock == 0)
        Gsv2k7RpllProtect(port);
#endif
    /* Step 2. Check whether tmds_pll_lock value has changed */
    switch(port->index)
    {
        case 0:
            GSV2K7_INT_get_RXA_TMDSPLL_LOCK_DET_INT_ST(port, &tmds_pll_lock_flag);
            if(tmds_pll_lock_flag)
            {
                GSV2K7_INT_set_RXA_TMDSPLL_LOCK_DET_CLEAR(port, 1);
            }
            break;
        case 1:
            GSV2K7_INT_get_RXB_TMDSPLL_LOCK_DET_INT_ST(port, &tmds_pll_lock_flag);
            if(tmds_pll_lock_flag)
            {
                GSV2K7_INT_set_RXB_TMDSPLL_LOCK_DET_CLEAR(port, 1);
            }
            break;
        case 2:
            GSV2K7_INT_get_RXC_TMDSPLL_LOCK_DET_INT_ST(port, &tmds_pll_lock_flag);
            if(tmds_pll_lock_flag)
            {
                GSV2K7_INT_set_RXC_TMDSPLL_LOCK_DET_CLEAR(port, 1);
            }
            break;
        case 3:
            GSV2K7_INT_get_RXD_TMDSPLL_LOCK_DET_INT_ST(port, &tmds_pll_lock_flag);
            if(tmds_pll_lock_flag)
            {
                GSV2K7_INT_set_RXD_TMDSPLL_LOCK_DET_CLEAR(port, 1);
            }
            break;
        default:
            break;
    }
    if(tmds_pll_lock_flag)
    {
        port->content.rx->Lock.PllLock = 0;
        port->content.rx->Lock.EqLock = 0;
        port->content.rx->Lock.AudioLock = 0;
    }
    /* Step 3. Check whether is locked */
#if (Gsv2k7PhyBypass == 0)
    if(FindCore2(port) == port)
    {
        GSV2K7_INT_get_RX2_HS_LOCKED_INT_ST(port, &de_regen_lock_flag);
        GSV2K7_INT_get_RX2_VS_LOCKED_INT_ST(port, &v_sync_lock_flag);
        if((de_regen_lock_flag == 1) || (v_sync_lock_flag == 1))
        {
            port->content.rx->Lock.DeRegenLock = 0;
            port->content.rx->Lock.VSyncLock = 0;
            GSV2K7_INT_set_RX2_HS_LOCKED_CLEAR(port, 1);
            GSV2K7_INT_set_RX2_VS_LOCKED_CLEAR(port, 1);
        }
    }
#endif //(Gsv2k7PhyBypass == 0)
#if ((Gsv2k7MuxMode == 1) || (Gsv2k7PhyBypass == 1))
    #if (AvAllowHpdLowEdidRead == 0)
    port->content.rx->Lock.DeRegenLock = port->content.rx->Lock.PllLock;
    port->content.rx->Lock.VSyncLock   = port->content.rx->Lock.PllLock;
    #endif
#endif
    /* Step 4. Manual Reset EQ when PLL Lock Status is lost */
    if(tmds_pll_lock_flag)
    {
        AvUapiOutputDebugMessage("Port %d: TMDS PLL Unlock ST triggers EQ ReRun", port->index);
        /* EQ reset to default state */
        Gsv2k7RxManualEQUpdate(port, 1);
    }
    /* Step 5. Update IsInputStable */
    if((port->content.rx->Lock.DeRegenLock == 0) ||
       (port->content.rx->Lock.VSyncLock   == 0) ||
       (port->content.rx->Lock.PllLock     == 0))
        port->content.rx->IsInputStable = 0;

    return ret;
}

void Gsv2k7RxScdcFsm(AvPort* port)
{
#if AvAllowHpdLowEdidRead
    uint8 value1 = 0;
    uint8 value2 = 0;
    uint8 CurrentScdcFsm       = RxScdcFsm(port)[port->index];
    uint8 CurrentScdcScramble  = RxScdcScramble(port)[port->index];
    uint8 CurrentScdcClkRatio  = RxScdcClkRatio(port)[port->index];
    uint16 CurrentScdcTuneRound = RxScdcTuneRound(port)[port->index];

    if(port->core.HdmiCore == 1)
    {
        GSV2K7_RXDIG_get_BG_HDCP_ENC_FRAME(port, &value2);
        switch(CurrentScdcFsm)
        {
            case 0: /* Default State */
                value1 = 0xA0 | (CurrentScdcScramble << 4) | (CurrentScdcClkRatio << 6);
                AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(FromPort), 0xD9, 0xFF, 0x0, value1);
                CurrentScdcTuneRound = 0;
                CurrentScdcFsm = 1;
                AvUapiOutputDebugMessage("Port %d: SCDC FSM -> Init State, %d, %d", port->index, CurrentScdcClkRatio, value2);
                break;
            case 1: /* Initial State */
                CurrentScdcTuneRound = CurrentScdcTuneRound + 1;
                if(CurrentScdcTuneRound > RxScdcDelayExpireThreshold)
                {
                    CurrentScdcScramble = 1 - CurrentScdcScramble;
                    CurrentScdcClkRatio = 1 - CurrentScdcClkRatio;
                    value1 = 0xA0 | (CurrentScdcScramble << 4) | (CurrentScdcClkRatio << 6);
                    AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(FromPort), 0xD9, 0xFF, 0x0, value1);
                    if(value2 == 1)
                    {
                        GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                        GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
                    }
                    CurrentScdcTuneRound = 0;
                    CurrentScdcFsm = 2;
                    AvUapiOutputDebugMessage("Port %d: SCDC FSM -> Second Choice, %d", port->index, value2);
                }
                break;
            case 2: /* Second Choice */
                CurrentScdcTuneRound = CurrentScdcTuneRound + 1;
                if(CurrentScdcTuneRound > RxScdcDelayExpireThreshold)
                {
                    CurrentScdcScramble = 1 - CurrentScdcScramble;
                    CurrentScdcClkRatio = 1 - CurrentScdcClkRatio;
                    value1 = 0xA0 | (CurrentScdcScramble << 4) | (CurrentScdcClkRatio << 6);
                    AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(FromPort), 0xD9, 0xFF, 0x0, value1);
                    CurrentScdcTuneRound = 0;
                    CurrentScdcFsm = 3;
                    AvUapiOutputDebugMessage("Port %d: SCDC FSM -> Toggle Term Entry, %d", port->index, value2);
                }
                break;
            case 3: /* Toggle Term Entry */
                GSV2K7_PRIM_get_RX_RTERM_EN(port, &value1);
                switch(port->index)
                {
                    case 0:
                        value1 = value1 & 0x0E;
                        break;
                    case 1:
                        value1 = value1 & 0x0D;
                        break;
                    case 2:
                        value1 = value1 & 0x0B;
                        break;
                    case 3:
                        value1 = value1 & 0x07;
                        break;
                    default:
                        return;
                }
                GSV2K7_PRIM_set_RX_RTERM_EN(port, value1);
                CurrentScdcFsm = 4;
                AvUapiOutputDebugMessage("Port %d: SCDC FSM -> Toggle Term Exit %d", port->index, value2);
                break;
            case 4: /* Toggle Term Exit */
                GSV2K7_PRIM_get_RX_RTERM_EN(port, &value1);
                switch(port->index)
                {
                    case 0:
                        value1 = value1 | 0x01;
                        break;
                    case 1:
                        value1 = value1 | 0x02;
                        break;
                    case 2:
                        value1 = value1 | 0x04;
                        break;
                    case 3:
                        value1 = value1 | 0x08;
                        break;
                    default:
                        return;
                }
                GSV2K7_PRIM_set_RX_RTERM_EN(port, value1);
                if(value2 == 1)
                {
                    GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                    GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
                }
                CurrentScdcFsm = 5;
                AvUapiOutputDebugMessage("Port %d: SCDC FSM -> Init State, Retry, %d", port->index, value2);
                break;
            case 5: /* Initial State, Retry */
                CurrentScdcTuneRound = CurrentScdcTuneRound + 1;
                if(CurrentScdcTuneRound > RxScdcDelayExpireThreshold)
                {
                    CurrentScdcScramble = 1 - CurrentScdcScramble;
                    CurrentScdcClkRatio = 1 - CurrentScdcClkRatio;
                    value1 = 0xA0 | (CurrentScdcScramble << 4) | (CurrentScdcClkRatio << 6);
                    AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(FromPort), 0xD9, 0xFF, 0x0, value1);
                    if(value2 == 1)
                    {
                        GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                        GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
                    }
                    CurrentScdcTuneRound = 0;
                    CurrentScdcFsm = 6;
                    AvUapiOutputDebugMessage("Port %d: SCDC FSM -> Second Choice, Retry, %d", port->index, value2);
                }
                break;
            case 6: /* Second Choice, Retry */
                CurrentScdcTuneRound = CurrentScdcTuneRound + 1;
                if(CurrentScdcTuneRound > RxScdcDelayExpireThreshold)
                {
                    CurrentScdcScramble = 1 - CurrentScdcScramble;
                    CurrentScdcClkRatio = 1 - CurrentScdcClkRatio;
                    AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(FromPort), 0xD9, 0xFF, 0x0, 0x00);
                    if(value2 == 1)
                    {
                        GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                        GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
                    }
                    CurrentScdcTuneRound = 0;
                    CurrentScdcFsm = 7;
                    port->content.rx->Hpd = AV_HPD_TOGGLE;
                    AvUapiOutputDebugMessage("Port %d: SCDC FSM -> HPD, %d", port->index, value2);
                }
                break;
            case 7: /* HPD */
                break;
        }
    }
    else
    {
        if(CurrentScdcFsm != 0)
        {
            CurrentScdcFsm = 0;
            CurrentScdcTuneRound = 0;
        }
    }

    RxScdcScramble(port)[port->index]  = CurrentScdcScramble;
    RxScdcClkRatio(port)[port->index]  = CurrentScdcClkRatio;
    RxScdcTuneRound(port)[port->index] = CurrentScdcTuneRound;
    RxScdcFsm(port)[port->index]       = CurrentScdcFsm;

#endif
    return;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetVideoTiming(pio AvPort *port))
{
#if (Gsv2k7PhyBypass == 0)
    AvPort *FromPort;
    if(KfunFindVideoRxFront(port, &FromPort) == AvOk)
    {
        /* no need to set if Rx is not stable */
        if(FromPort->content.rx->IsInputStable == 0)
            return AvOk;
    }

    GSV2K7_TXDIG_set_MAN_POL_HS(port, 1 - port->content.video->timing.HPolarity);
    GSV2K7_TXDIG_set_MAN_POL_VS(port, 1 - port->content.video->timing.VPolarity);
#endif //(Gsv2k7PhyBypass == 1)
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetVideoTiming(pio AvPort *port))
{
    AvRet ret = AvOk;

#if (Gsv2k7PhyBypass == 0)
    #if AvEnableDetailTiming
    uint32 TempValue = 0;
    GSV2K7_RXDIG_get_RB_RX_INTERLACED(port, &port->content.video->timing.Interlaced);
    GSV2K7_RXDIG_get_RB_RX_H_TOTAL_WIDTH(port,&TempValue);
    port->content.video->timing.HTotal  = TempValue;
    GSV2K7_RXDIG_get_RB_RX_V_ACTIVE_HEIGHT_0(port,&TempValue);
    if(port->content.video->timing.Interlaced)
        port->content.video->timing.VActive = TempValue<<1;
    else
        port->content.video->timing.VActive = TempValue;
    GSV2K7_RXDIG_get_RB_RX_V_TOTAL_0 (port,&TempValue);
    port->content.video->timing.VTotal  = TempValue>>1;
    GSV2K7_RXDIG_get_RB_RX_H_ACTIVE_WIDTH(port,&TempValue);
    port->content.video->timing.HActive = TempValue;
    TempValue = ((port->content.video->info.TmdsFreq)*1000000)/
                 ((port->content.video->timing.HTotal) *
                  (port->content.video->timing.VTotal));
    port->content.video->timing.FrameRate = TempValue+1;
    #endif //AvEnableDetailTiming

    GSV2K7_RXDIG_get_RB_RX_HS_POL(port, &port->content.video->timing.HPolarity);
    GSV2K7_RXDIG_get_RB_RX_VS_POL(port, &port->content.video->timing.VPolarity);
    /* 3D */
    GSV2K7_RXAUD_get_RB_RX_VIDEO_3D(port, &port->content.video->info.Detect3D);
#endif

    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetHdmiDeepColor(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 Value;
    GSV2K7_RXDIG_get_RB_RX_DEEP_COLOR_MODE(port, &Value);
    if(Value == 0)
        port->content.video->Cd = AV_CD_24;
    else if(Value == 1)
        port->content.video->Cd = AV_CD_30;
    else if(Value == 2)
        port->content.video->Cd = AV_CD_36;
    else
        port->content.video->Cd = AV_CD_NOT_INDICATED;
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetHdmiDeepColor(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value = 0;
    AvPort *FromPort;
    FromPort = (AvPort*)port->content.RouteVideoFromPort;
    if(FromPort != NULL)
    {
        if(FromPort->type == VideoScaler)
            value = 4; /* Transmit 24-bit */
        else if(FromPort->type == VideoColor)
            value = 4; /* Set to 8-bit to make it /4 clock convinient */
        else
        {
    #if Gsv2k7MuxMode
        /* Mux Mode only supports PLL = 1 ratio, no need to extend to 1.25 and 1.5 */
            value = 4;
    #else
        if(port->content.video->Cd == AV_CD_30)
            value = 5;
        else if(port->content.video->Cd == AV_CD_36)
            value = 6;
        else
            value = 4;
    #endif
        }
    }
    /* DVI protection */
    if(port->content.tx->HdmiMode == 0)
        value = 4;
    GSV2K7_TXDIG_set_TX_GC_CD(port, value);
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxGetAudioInternalMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetAudioInternalMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    return ret;
}

uint8 Gsv2k7_RxGetPixRepValue(AvPort *port)
{
    uint8 RetVal = 0;
#if (Gsv2k7PhyBypass == 0)
    GSV2K7_RXDIG_get_RX_REPETITION_MAN_EN(port, &RetVal);

    if (RetVal == 1)
    {
        GSV2K7_RXDIG_get_RX_REPETITION_MAN(port, &RetVal);
    }
    else
    {
        GSV2K7_RXDIG_get_RB_RX_PIXEL_REPETITION(port, &RetVal);
    }
#endif //(Gsv2k7PhyBypass == 0)
    return RetVal;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxSetBlackMute(pio AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    if((port->content.video->InCs == AV_CS_RGB) ||
       (port->content.video->InCs == AV_CS_DEFAULT_RGB) ||
       (port->content.video->InCs == AV_CS_ADOBE_RGB) ||
       (port->content.video->InCs == AV_CS_LIM_RGB) ||
       (port->content.video->InCs == AV_CS_LIM_ADOBE_RGB) ||
       (port->content.video->InCs == AV_CS_AUTO))
        GSV2K7_TXDIG_set_TX_VIDEO_INPUT_CS(port, 0);
    else
        GSV2K7_TXDIG_set_TX_VIDEO_INPUT_CS(port, 1);
    GSV2K7_TXDIG_set_TX_BLACK_VIDEO_EN(port, port->content.video->Mute.BlkMute);
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxEncryptSink(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 value;
    uint8 DviWithHdcp   = 0;
    uint8 Hdcp1p4Enable = 0;
    uint8 Hdcp2p2Enable = 0;

    GSV2K7_TXPHY_get_TX_TMDS_CLK_DRIVER_ENABLE(port, &value);
    if((value == 0) || (port->content.tx->IgnoreEdidError == 1))
        return AvNotAvailable;

    /* Dvi PreProcess */
    if((port->content.tx->HdmiMode == 0) && (port->content.hdcptx->HdcpSupport == 1))
    {
        switch(port->content.hdcptx->HdmiStyle)
        {
            case AV_HDCP_TX_AUTO:
            case AV_HDCP_TX_AUTO_FAIL_OUT:
                if(port->content.hdcptx->DviStyle == AV_HDCP_TX_DVI_LOOSE)
                    DviWithHdcp   = 1;
                break;
            case AV_HDCP_TX_1P4_ONLY:
            case AV_HDCP_TX_1P4_FAIL_OUT:
                DviWithHdcp   = 1;
                break;
            default:
                break;
        }
    }
    /* Hdmi Style Select */
    if((port->content.tx->HdmiMode == 1) || (DviWithHdcp == 1))
    {
        switch(port->content.hdcptx->HdmiStyle)
        {
            case AV_HDCP_TX_ILLEGAL_NO_HDCP:
                break;
            case AV_HDCP_TX_AUTO:
                Hdcp1p4Enable = 1;
                Hdcp2p2Enable = 1;
                break;
            case AV_HDCP_TX_AUTO_FAIL_OUT:
                if(port->content.hdcptx->HdcpError < AvHdcpTxErrorThreshold)
                {
                    Hdcp1p4Enable = 1;
                    Hdcp2p2Enable = 1;
                }
                break;
            case AV_HDCP_TX_1P4_ONLY:
                Hdcp1p4Enable = 1;
                break;
            case AV_HDCP_TX_1P4_FAIL_OUT:
                if(port->content.hdcptx->HdcpError < AvHdcpTxErrorThreshold)
                    Hdcp1p4Enable = 1;
                break;
            case AV_HDCP_TX_2P2_ONLY:
                Hdcp2p2Enable = 1;
                break;
            case AV_HDCP_TX_2P2_FAIL_OUT:
                if(port->content.hdcptx->HdcpError < AvHdcpTxErrorThreshold)
                    Hdcp2p2Enable = 1;
                break;
        }
        /* No HDCP Support Process */
        if((port->content.hdcptx->HdcpSupport == 0) &&
           (port->content.hdcptx->Hdcp2p2SinkSupport == 0))
        {
            port->content.video->Mute.BlkMute = 1;
            port->content.audio->AudioMute    = 1;
            Hdcp1p4Enable = 0;
            Hdcp2p2Enable = 0;
        }
        if(port->content.hdcptx->HdcpModeUpdate != 0)
        {
            Hdcp1p4Enable = 0;
            Hdcp2p2Enable = 0;
        }
#if AvEnableSimplifyHdcp
        GSV2K7_TXPHY_get_TX_HDCP1P4_DESIRED(port, &value);
        if(value != Hdcp1p4Enable)
        {
            GSV2K7_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
            GSV2K7_TXPHY_set_TX_HDCP1P4_DESIRED(port, Hdcp1p4Enable);
            GSV2K7_TXPHY_set_TX_HDCP1P4_ENC_EN(port, Hdcp1p4Enable);
            if(Hdcp1p4Enable)
                AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP 1.4 Cipher Enable", (port->index-4));
        }
    #if AvEnableHdcp2p2Feature
        GSV2K7_TX2P2_get_TX_HDCP2P2_DESIRED(port, &value);
        if(value != Hdcp2p2Enable)
        {
            GSV2K7_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 1);
            GSV2K7_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 0);
            GSV2K7_TX2P2_set_TX_HDCP2P2_DESIRED(port, Hdcp2p2Enable);
            if(Hdcp2p2Enable)
                AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP 2.2 Cipher Enable", (port->index-4));
        }
    #endif
#else
        /* when 2.2 repeater is in AUTO mode, BKSV is automatically cleared */
        /* so tx frame encryption can be enabled directly in this mode */
        if(port->content.hdcptx->Hdcp2p2Flag == 1)
        {
            GSV2K7_TXPHY_set_TX_HDCP1P4_FRAME_ENCRYPTION(port, 1);
            AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP Cipher Enable", (port->index-4));
        }
#endif
    }
    /* Dvi Style Select */
    else
    {
        value = 0;
        switch(port->content.hdcptx->DviStyle)
        {
            case AV_HDCP_TX_DVI_STRICT:
                value = 1;
                break;
            case AV_HDCP_TX_DVI_LOOSE:
                value = 0;
                break;
        }
        if(value != port->content.video->Mute.BlkMute)
        {
            port->content.video->Mute.BlkMute = value;
            AvUapiTxSetBlackMute(port);
        }
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxDecryptSink(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    port->content.hdcptx->HdcpEnabled = 0;
    port->content.hdcptx->Authenticated = 0;
    port->content.hdcptx->HdcpError = 0;

    GSV2K7_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 0);
    GSV2K7_TXPHY_set_TX_HDCP1P4_CLR_BKSV_FLAG(port, 1);
    GSV2K7_TXPHY_set_TX_HDCP1P4_DESIRED(port, 0);
    AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP Cipher Disable", (port->index-4));

    #if AvEnableHdcp2p2Feature
    GSV2K7_TX2P2_set_TX_HDCP2P2_DESIRED(port, 0);
    #endif
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetHdcpEnable(pin AvPort *port))
{
    AvRet ret = AvOk;
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetBksvListReady(AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    uint8 On = 0;
    uint8 Clear = 0;
    if(port->content.hdcp->SinkNumber == port->content.hdcp->SinkTotal)
    {
        On = 1;
        Clear = 0;
#if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("Set Rx BKSV Ready");
#endif
    }
    else
    {
        On = 0;
        Clear = 1;
#if AvEnableDebugHdcp
        AvUapiOutputDebugMessage("Clear Rx BKSV Ready");
#endif
    }

    GSV2K7_RXRPT_set_RX_KSV_LIST_NUM(port, port->content.hdcp->SinkTotal);
    GSV2K7_RXRPT_set_RX_KSV_LIST_READY(port, On);
    GSV2K7_RXRPT_set_RX_KSV_LIST_READY_CLR(port, Clear);
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetHdcpMode(pin AvPort *port))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    /* GSV2k7 has only 1 Hdmi Core */
    if(port->core.HdmiCore == 1)
    {
        if(port->content.hdcp->SinkTotal == 0)
        {
            GSV2K7_RXRPT_set_RX_HDCP1P4_BCAPS(port, AV_BCAPS_RECEIVER_MODE);
            GSV2K7_RXRPT_set_RX_HDCP1P4_BSTATUS(port, AV_BSTATUS_RECEIVER_MODE);
            AvUapiOutputDebugMessage("Port %d: Set BCaps/Status REC mode", port->index);
        }
        else
        {
#if AvEnableSimplifyHdcp
            GSV2K7_RXRPT_set_RX_HDCP1P4_BCAPS(port, AV_BCAPS_RECEIVER_MODE);
            GSV2K7_RXRPT_set_RX_HDCP1P4_BSTATUS(port, AV_BSTATUS_RECEIVER_MODE);
#else
            GSV2K7_RXRPT_set_RX_HDCP1P4_BCAPS(port, AV_BCAPS_REPEATER_MODE);
            GSV2K7_RXRPT_set_RX_HDCP1P4_BSTATUS(port, AV_BSTATUS_REPEATER_MODE);
#endif
            AvUapiOutputDebugMessage("Port %d: Set BCaps/Status REP mode", port->index);
        }
    }
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiHdcp2p2Mode(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxGetHdcpEnableStatus(pin AvPort *port))
{
    return AvOk;
}

/**
 * @brief  init cec port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxCecInit(pin AvPort *port))
{
#if AvEnableCecFeature /* CEC Related */
    GSV2K7_TXCEC_set_CEC_POWER_MODE(port, port->content.cec->CecEnable);
    if(port->content.cec->CecEnable == 1)
    {
        GSV2K7_TXCEC_set_CEC_TX_RETRY(port, 3);
        GSV2K7_TXCEC_set_CEC_RX_3BUFFERS_ENABLE(port, 1);
    }
#endif /* CEC Related */
    return AvOk;

}

AvRet ImplementUapi(Gsv2k7, AvUapiTxCecSetPhysicalAddr(AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    uint32 phyaddr = port->content.tx->PhyAddr;
    GSV2K7_TXCEC_set_CEC_PHY_ADDR(port, phyaddr);
#endif
    return ret;
}

/*============================================================================
 * Set the logical address for one of 3 logical devices
 *
 * Entry:   LogAddr = Logical address for the device
 *          DevId   = The device to set the logical address to. (0, 1 or 2)
 *          Enable  = TRUE to enable the logical device
 *                    FALSE to disable the logical device
 *
 * Return:  AvOk
 *          AvInvalidParameter if DevId is larger than 2
 *
 *===========================================================================*/
AvRet ImplementUapi(Gsv2k7, AvUapiTxCecSetLogicalAddr(AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    uint8 Mask=0, DevBit=0;
    uint8 LogAddr = port->content.cec->LogAddr;
    uint8 DevId = 0;
    uint8 Enable = 1;

    LogAddr &= 0xf;
    switch (DevId)
    {
        case 0:
            GSV2K7_TXCEC_set_CEC_LOGIC_ADDR0(port, LogAddr);
            DevBit = 0x01;
            break;
        case 1:
            GSV2K7_TXCEC_set_CEC_LOGIC_ADDR1(port, LogAddr);
            DevBit = 0x02;
            break;
        case 2:
            GSV2K7_TXCEC_set_CEC_LOGIC_ADDR2(port, LogAddr);
            DevBit = 0x04;
            break;
        default:
            ret = AvInvalidParameter;
            break;
    }

    GSV2K7_TXCEC_get_CEC_LOGIC_ADDR_SELECTION(port, &Mask);
    Mask &= (~DevBit);
    if (Enable)
    {
        Mask |= DevBit;
    }
    if (ret == AvOk)
    {
        GSV2K7_TXCEC_set_CEC_LOGIC_ADDR_SELECTION(port, Mask);
    }
#endif /* CEC Related */
    return ret;
}



/*============================================================================
 *
 * Entry:   CecInts = CEC interrupts
 *
 * Return:  None
 *
 *
 *===========================================================================*/

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCecRxGetStatus (AvPort* port))
{
    AvRet ret = AvNotAvailable;
#if AvEnableCecFeature /* CEC Related */
    uint8 BufferOrder = 0;
    uint8 SelectOrder = 0;
    uint8 value0, value1, value2;
    uint8 position = 0;

    /* 1. Find if there is message existed */
    GSV2K7_TXCEC_get_CEC_RX_BUF0_READY(port, &value0);
    GSV2K7_TXCEC_get_CEC_RX_BUF1_READY(port, &value1);
    GSV2K7_TXCEC_get_CEC_RX_BUF2_READY(port, &value2);
    if((value0 == 0) &&
       (value1 == 0) &&
       (value2 == 0))
    {
        port->content.cec->RxGetFlag = 0;
        return ret;
    }
    else
        port->content.cec->RxGetFlag = 1;

    /* 2. Find the Lowest Value */
    GSV2K7_TXCEC_get_CEC_RX_BUF0_TIMESTAMP(port, &BufferOrder);
    if(BufferOrder != 0)
    {
        SelectOrder = BufferOrder;
        position = 0;
    }
    GSV2K7_TXCEC_get_CEC_RX_BUF1_TIMESTAMP(port, &BufferOrder);
    if((SelectOrder == 0) ||
       ((BufferOrder != 0) &&
       (BufferOrder < SelectOrder)))
    {
        SelectOrder = BufferOrder;
        position = 1;
    }
    GSV2K7_TXCEC_get_CEC_RX_BUF2_TIMESTAMP(port, &BufferOrder);
    if((SelectOrder == 0) ||
       ((BufferOrder != 0) &&
       (BufferOrder < SelectOrder)))
    {
        SelectOrder = BufferOrder;
        position = 2;
    }

    /* 3. Dump Rx Data */
    switch(position)
    {
        case CEC_RX_BUFFER1:
            AvHalI2cReadMultiField(GSV2K7_TXCEC_MAP_ADDR(port), 0x15, 17,
                                   port->content.cec->RxContent);
            GSV2K7_TXCEC_get_CEC_RX_BUF0_FRAME_LENGTH(port, &port->content.cec->RxLen);
            /* CEC buffer1 flag need to do toggling */
            GSV2K7_TXCEC_set_CEC_RX_CLEAR_BUF0(port,1);
            GSV2K7_TXCEC_set_CEC_RX_CLEAR_BUF0(port,0);
            ret = AvOk;
            break;
        case CEC_RX_BUFFER2:
            AvHalI2cReadMultiField(GSV2K7_TXCEC_MAP_ADDR(port), 0x27, 17,
                                   port->content.cec->RxContent);
            GSV2K7_TXCEC_get_CEC_RX_BUF1_FRAME_LENGTH(port, &port->content.cec->RxLen);
            /* CEC buffer2 flag need to do toggling */
            GSV2K7_TXCEC_set_CEC_RX_CLEAR_BUF1(port,1);
            GSV2K7_TXCEC_set_CEC_RX_CLEAR_BUF1(port,0);
            ret = AvOk;
            break;
        case CEC_RX_BUFFER3:
            AvHalI2cReadMultiField(GSV2K7_TXCEC_MAP_ADDR(port), 0x38, 17,
                                   port->content.cec->RxContent);
            GSV2K7_TXCEC_get_CEC_RX_BUF2_FRAME_LENGTH(port, &port->content.cec->RxLen);
            /* CEC buffer3 flag need to do toggling */
            GSV2K7_TXCEC_set_CEC_RX_CLEAR_BUF2(port,1);
            GSV2K7_TXCEC_set_CEC_RX_CLEAR_BUF2(port,0);
            ret = AvOk;
            break;
        default:
            break;
    }
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCecSendMessage(pin AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    uint8 value;
    GSV2K7_TXCEC_get_CEC_TX_ENABLE(port, &value);
    if(value == 1)
        GSV2K7_TXCEC_set_CEC_TX_ENABLE(port, 0);

    if(port->content.cec->TxLen > 16)
    {
        ret = AvInvalidParameter;
    }
    else
    {
        /* clear all the interrupts */
        if(port->index == 4)
        {
            GSV2K7_INT_set_TX1_TX_CEC_READY_CLEAR(port,          1);
            GSV2K7_INT_set_TX1_TX_ARBITRATION_LOST_CLEAR(port,   1);
            GSV2K7_INT_set_TX1_TX_RETRY_TIMEOUT_CLEAR(port,      1);
        }
        else if(port->index == 5)
        {
            GSV2K7_INT_set_TX2_TX_CEC_READY_CLEAR(port,          1);
            GSV2K7_INT_set_TX2_TX_ARBITRATION_LOST_CLEAR(port,   1);
            GSV2K7_INT_set_TX2_TX_RETRY_TIMEOUT_CLEAR(port,      1);
        }

        /* CecTxState = CEC_TX_STATE_BUSY; */
        /* 1. Action */
        AvHalI2cWriteMultiField(GSV2K7_TXCEC_MAP_ADDR(port),
                                0x00,
                                port->content.cec->TxLen,
                                port->content.cec->TxContent);
        GSV2K7_TXCEC_set_CEC_TX_FRAME_LENGTH(port, port->content.cec->TxLen);
        GSV2K7_TXCEC_set_CEC_TX_ENABLE(port, 1);
        AvUapiOutputDebugMessage("CEC: %x Opcode %x Message %x Sent!!!!!",
                                 port->content.cec->TxContent[0],
                                 port->content.cec->TxContent[1],
                                 port->content.cec->TxContent[2]);
        /* 2. Cleaning */
        /* TxSendFlag = 2 means the data is sent out and waiting response */
        port->content.cec->TxSendFlag = AV_CEC_TX_WAIT_RESPONSE;
        ret = AvOk;
    }
#endif

    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiGetNackCount(pin AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    GSV2K7_TXCEC_get_CEC_TX_NACK_COUNTER(port, &port->content.cec->NackCount);
#endif /* CEC Related */
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCecTxGetStatus(pin AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    if(port->index == 4)
    {
        GSV2K7_INT_get_TX1_TX_CEC_READY_INT_ST(port,        &port->content.cec->TxReady);
        GSV2K7_INT_get_TX1_TX_ARBITRATION_LOST_INT_ST(port, &port->content.cec->ArbLost);
        GSV2K7_INT_get_TX1_TX_RETRY_TIMEOUT_INT_ST(port,    &port->content.cec->Timeout);
    }
    else if(port->index == 5)
    {
        GSV2K7_INT_get_TX2_TX_CEC_READY_INT_ST(port,        &port->content.cec->TxReady);
        GSV2K7_INT_get_TX2_TX_ARBITRATION_LOST_INT_ST(port, &port->content.cec->ArbLost);
        GSV2K7_INT_get_TX2_TX_RETRY_TIMEOUT_INT_ST(port,    &port->content.cec->Timeout);
    }

    GSV2K7_TXCEC_get_TX_LOWDRIVE_COUNTER(port,              &port->content.cec->LowDriveCount);
    GSV2K7_TXCEC_get_CEC_TX_NACK_COUNTER(port,              &port->content.cec->NackCount);

    /* data is sent anyway, find the response */
    if(port->content.cec->TxSendFlag == AV_CEC_TX_WAIT_RESPONSE)
    {
        if(port->content.cec->TxReady)
            port->content.cec->TxSendFlag = AV_CEC_TX_SEND_SUCCESS;
        else if(port->content.cec->Timeout)
            port->content.cec->TxSendFlag = AV_CEC_TX_SEND_FAIL;
        else if(port->content.cec->ArbLost)
            AvUapiCecSendMessage(port);
    }

    /*
    AvUapiOutputDebugMessage("CEC: TxReady = %d.\r\n", port->content.cec->TxReady);
    AvUapiOutputDebugMessage("CEC: ArbLost = %d.\r\n", port->content.cec->ArbLost);
    AvUapiOutputDebugMessage("CEC: Timeout = %d.\r\n", port->content.cec->Timeout);
    */
#endif /* CEC Related */

    return ret;
}

/* EDID functions */
/* Rx Read Edid */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count))
{
    AvRet ret = AvOk;
    return ret;
}

/* Rx Write Edid */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count))
{
    AvRet ret = AvOk;
#if (Gsv2k7PhyBypass == 0)
    /* using Edid1 */
    AvUapiOutputDebugMessage("Rx%d Writes EDID RAM1, [255]=0x%x .",port->index+1, Value[255]);
    /* Edid1 selection */
    GSV2K7_RXRPT_set_RX_EDID_RAM_SEL(port, 0);
    GSV2K7_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 0);
    AvHalI2cWriteMultiField(GSV2K7_HDMI_EDID_ADDR(port), 0, 256, Value);
    if(Count > 256)
    {
        GSV2K7_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 1);
        AvHalI2cWriteMultiField(GSV2K7_HDMI_EDID_ADDR(port), 0, 256, (Value+256));
    }
    #if (AvAllowHpdLowEdidRead == 1)
    RxScdcNewEdid(port) = 0;
    #endif
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

/* Rx Write SPA */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue))
{
    AvRet ret = AvOk;
#if AvDontCareEdidSpa
    /* set SPA pointer to invalid 0x01 */
    GSV2K7_RXRPT_set_RX_EDID1_SPA_LOC_MSB(port, 0);
    GSV2K7_RXRPT_set_RX_EDID1_SPA_LOC_LSB(port, 1);
    GSV2K7_RXRPT_set_RX_EDID2_SPA_LOC_MSB(port, 0);
    GSV2K7_RXRPT_set_RX_EDID2_SPA_LOC_LSB(port, 1);
    /* set SPA data for invalid 0x01 to be 0xff/0xff */
    GSV2K7_RXRPT_set_RXB_SPA(port, 0xffff);
    GSV2K7_RXRPT_set_RXC_SPA(port, 0xffff);
    GSV2K7_RXRPT_set_RXD_SPA(port, 0xffff);
#else //(AvDontCareEdidSpa==0)
    uint8 value = 0;
    uint8 RamSel = 0;
    uint16 Spa = 0;
    uint8 SpaShift = 0;

    /* 1. Set SPA Location */
    /* using Edid1 */
    if(port->core.HdmiCore == 0)
    {
        GSV2K7_RXRPT_set_RX_EDID1_SPA_LOC_LSB(port, SpaLocation);
        RamSel = 0;
    }
    /* using Edid2 */
    else if(port->core.HdmiCore == 1)
    {
        GSV2K7_RXRPT_set_RX_EDID2_SPA_LOC_LSB(port, SpaLocation);
        RamSel = 1;
    }
    else
        return ret;

    if(SpaLocation < 128)
    {
        GSV2K7_RXRPT_set_RX_EDID_RAM_SEL(port, RamSel);
        GSV2K7_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 0);
        Spa = (SpaValue[0]<<8)+SpaValue[1];
        if(port->index == 0)
            AvHalI2cWriteMultiField(GSV2K7_HDMI_EDID_ADDR(port), SpaLocation, 2, SpaValue);
        else if(port->index == 1)
            GSV2K7_RXRPT_set_RXB_SPA(port, Spa);
        else if(port->index == 2)
            GSV2K7_RXRPT_set_RXC_SPA(port, Spa);
        else if(port->index == 3)
            GSV2K7_RXRPT_set_RXD_SPA(port, Spa);
        return ret;
    }
    /* 2. Generate all Rx Spa values */
    Spa = (SpaValue[0]<<8)+SpaValue[1];
    for(value=0;value<16;value=value+4)
    {
        if((Spa&0x000f) != 0)
        {
            SpaShift = value;
            Spa = Spa&0xfff0;
            break;
        }
        else
        {
            Spa = Spa>>4;
        }
    }
    /* 2.1 Wrong Spa Protection */
    if(value == 16)
        return AvError;

    /* 3. write SPA value */
    /* RxPort 0, write Edid Ram */
    GSV2K7_RXRPT_set_RX_EDID_RAM_SEL(port, RamSel);
    GSV2K7_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 0);
    Spa = Spa + 1; /* e.g. XX10 */
    SpaValue[0] = (Spa<<SpaShift)>>8;
    SpaValue[1] = (Spa<<SpaShift)&0xff;
    AvHalI2cWriteMultiField(GSV2K7_HDMI_EDID_ADDR(port), SpaLocation, 2, SpaValue);
    /* RxPort 1, write Reg */
    Spa = Spa + 1; /* e.g. XX20 */
    if(port->index == 1)
        GSV2K7_RXRPT_set_RXB_SPA(port, (Spa<<SpaShift));
    /* RxPort 2, write Reg */
    Spa = Spa + 1; /* e.g. XX30 */
    if(port->index == 2)
        GSV2K7_RXRPT_set_RXC_SPA(port, (Spa<<SpaShift));
    /* RxPort 3, write Reg */
    Spa = Spa + 1; /* e.g. XX40 */
    if(port->index == 3)
        GSV2K7_RXRPT_set_RXD_SPA(port, (Spa<<SpaShift));

#endif
    return ret;
}

/* Tx Read Edid */
uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count))
{
    AvRet ret = AvOk;
    uint8 ReadSuccess = 0;
    uint16 i = 0;
    uint8 CheckSum = 0;
    uint8 FsmState = 0;
    uint8 value = 0;

#if AvEdidStoredInRam
    /* Copy RAM into Edid Readback */
    if(port->content.tx->EdidReadSuccess == AV_EDID_UPDATED)
    {
        switch(port->index)
        {
            case 5:
                AvMemcpy(Value, TxBEdidRam(port), 256);
                AvUapiOutputDebugMessage("Port%d: ReRead Edid From TxBRam",port->index);
                return ret;
        }
    }
#endif

#if (Gsv2k7PhyBypass == 0)
    /* protect EDID reread from Tx Port Reset */
    while((ReadSuccess == 0) && (i < 3000))
    {
        GSV2K7_TXPHY_get_TX_EDID_READY_RB(port, &ReadSuccess);
        if(ReadSuccess == 0)
        {
            GSV2K7_TXPHY_get_TX_HDCP1P4_STATE_RB(port,   &FsmState);
            if(FsmState == 1) /* DDC Reading EDID */
            {
                value = Gsv2k7TxDDCError(port);
                if(value == 1)
                {
                    AvUapiOutputDebugMessage("Port%d: ReRead Edid Failure",port->index);
                    break;
                }
                else
                    i = i + 1;
            }
            else
                i = i + 1;
        }
    }
    /* Return if Edid read Failure or Illegal Edid */
    if(ReadSuccess == 1)
    {
        AvHalI2cReadMultiField(GSV2K7_TXEDID_MAP_ADDR(port), 0, Count, Value);
        ReadSuccess = 0;
        for(i=0;i<127;i++)
            CheckSum = CheckSum + Value[i];
        CheckSum = 256 - (CheckSum%256);
        if(CheckSum == Value[127])
        {
            if(Value[126] == 1)
            {
                CheckSum = 0;
                for(i=128;i<255;i++)
                    CheckSum = CheckSum + Value[i];
                CheckSum = 256 - (CheckSum%256);
                if(CheckSum == Value[255])
                    ReadSuccess = 1;
            }
            else
                ReadSuccess = 1;
        }
    }
    if(ReadSuccess == 0)
    {
        AvUapiOutputDebugMessage("Port%d: Read Edid Failure",port->index);
        AvMemset(Value, 0, 256);
#if AvForceDefaultEdid
        port->content.tx->IgnoreEdidError = 1;
#else
        /* reread the Edid */
        GSV2K7_TXPHY_set_TX_EDID_TRY_TIMES(port,15);
        GSV2K7_TXPHY_set_TX_EDID_RE_READ(port, 0);
        GSV2K7_TXPHY_set_TX_EDID_RE_READ(port, 1);
#endif
        return AvError;
    }

    if(port->content.tx->EdidReadSuccess != AV_EDID_UPDATED)
    {
#if AvEnableHdcp1p4BksvCheck
        if(port->content.hdcptx->HdcpSupport == 0)
        {
            /* Check Bskv for HDCP 1.4 Support Capability */
            GSV2K7_TXPHY_set_EDID_ANALYZE_DONE(port, 1);
            Gsv2k7ManualBksvRead(port);
            GSV2K7_TXPHY_set_EDID_ANALYZE_DONE(port, 0);
        }
#else
        AvUapiTxGetSinkHdcpCapability(port);
#endif
#if AvEdidStoredInRam
        /* Write Read Edid into RAM */
        switch(port->index)
        {
            case 5:
                AvMemcpy(TxBEdidRam(port), Value, 256);
                AvUapiOutputDebugMessage("Port%d: Write Edid To TxBRam",port->index);
                break;
        }
#endif
        /* Manually disable Edid Read Success, EDID will be read in Event */
        port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
    }
    else
    {
        ret = AvInvalidParameter;
    }
#endif //(Gsv2k7PhyBypass == 0)
    return ret;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxEnableInternalEdid(pio AvPort *port))
{
#if (Gsv2k7PhyBypass == 0)
    /* 2. enable internal Edid */
    GSV2K7_RXRPT_set_RXA_EDID_RAM_SEL(port, 0);
    GSV2K7_RXRPT_set_RXB_EDID_RAM_SEL(port, 0);
    GSV2K7_RXRPT_set_RXC_EDID_RAM_SEL(port, 0);
    GSV2K7_RXRPT_set_RXD_EDID_RAM_SEL(port, 0);
    GSV2K7_RXRPT_set_RX_EDID_CHECKSUM_RECALC(port, 1);
    GSV2K7_RXRPT_set_RXA_EDID_EN(port, 1);
    GSV2K7_RXRPT_set_RXB_EDID_EN(port, 1);
    GSV2K7_RXRPT_set_RXC_EDID_EN(port, 1);
    GSV2K7_RXRPT_set_RXD_EDID_EN(port, 1);
#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxArcEnable(pin AvPort *port, uint8 value))
{
    if(port->index == 5)
        GSV2K7_PRIM_set_TXA_ARC_PWR_DN(port, 1-value);
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiRxHdcp2p2Manage(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiTxHdcp2p2Manage(pin AvPort *port))
{
    return AvOk;
}

AvPort *Gsv2k7FindHdmiRxFront(pin AvPort *port)
{
    AvPort *FromPort = (AvPort *)port->content.RouteVideoFromPort;
    while(FromPort != NULL)
    {
#if AvEnableInternalVideoGen
        if((FromPort->type == HdmiRx) || (FromPort->type == VideoGen))
#else
        if(FromPort->type == HdmiRx)
#endif
            return FromPort;
        else
            FromPort = (AvPort *)FromPort->content.RouteVideoFromPort;
    }
    return NULL;
}

AvVideoCs Gsv2k7ColorCsMapping(AvVideoCs InputCs)
{
    AvVideoCs ReturnCs = InputCs;
    if(InputCs == AV_CS_YUV_709)
        ReturnCs = AV_CS_YCC_709;
    else if(InputCs == AV_CS_YUV_601)
        ReturnCs = AV_CS_YCC_601;
    else if(InputCs == AV_CS_LIM_YUV_709)
        ReturnCs = AV_CS_LIM_YCC_709;
    else if(InputCs == AV_CS_LIM_YUV_601)
        ReturnCs = AV_CS_LIM_YCC_601;
    else if(InputCs == AV_CS_DEFAULT_RGB)
        ReturnCs = AV_CS_RGB;
    else
        ReturnCs = InputCs;
    return ReturnCs;
}

void Gsv2k7TxPllUnlockClear(AvPort *port)
{
    switch(port->index)
    {
        case 5: /* TxB */
            GSV2K7_PLL_set_TXB_PLL_LOCK_CLEAR(port, 1);
            GSV2K7_PLL_set_TXB_PLL_LOCK_CLEAR(port, 0);
            break;
    }
}

uint8 Gsv2k7TweakCrystalFreq(AvPort *port, uint32 value32)
{
    uint8 CrystalFreq = 0;
    /* Check Frequency with 0xff/0x00 end */
    uint8 value = value32 & 0xff;
    if((value == 0x00) || (value == 0xff))
    {
        /* Change Crystal Frequency */
        AvHalI2cReadField8(GSV2K7_PLL_MAP_ADDR(port),0x01,0xFF,0,&CrystalFreq);
        if(CrystalFreq == 0x00)
            AvHalI2cWriteField8(GSV2K7_PLL_MAP_ADDR(port),0x01,0xFF,0,0x0F);
        else
            AvHalI2cWriteField8(GSV2K7_PLL_MAP_ADDR(port),0x01,0xFF,0,0x00);
        return 1;
    }
    return 0;
}

void Gsv2k7MpllProtect(AvPort *port)
{
    uint8 TxBDefaultRouting;
    uint8 i;
    uint8 value;
    uint8 ValidMpllFreq = 0;
    /* Step 1. Store TxB Default Routing */
    AvHalI2cReadField8(GSV2K7_SEC_MAP_ADDR(port),0x05,0x70,0,&TxBDefaultRouting);
    /* Step 2. Write TxB Default Routing to VG */
    AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port),0x05,0xFF,0,0x20);
    /* Step 3. Delay and read MPLL measure state */
    while(ValidMpllFreq == 0)
    {
        for(i=0;i<15;i++)
            AvHalI2cReadField8(GSV2K7_PLL_MAP_ADDR(port),0xCC,0xFF,0,&value);
        while(value != 0x20)
            AvHalI2cReadField8(GSV2K7_PLL_MAP_ADDR(port),0xCC,0xFF,0,&value);
        /* Step 4. Measure MPLL Frequency */
        AvHalI2cReadField8(GSV2K7_PLL_MAP_ADDR(port),0xC1,0xFF,0,&value);
        /* Step 5. MPLL Frequency Recovery */
        if((value < 0x81) || (value > 0x95))
        {
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port),0xC5,0xFF,0,0xCC);
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port),0xC6,0xFF,0,0x01);
            AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port),0xC5,0xFF,0,0x0C);
        }
        else
            ValidMpllFreq = 1;
    }
    /* Step 6. Write TxB Default Routing to VG */
    AvHalI2cWriteField8(GSV2K7_SEC_MAP_ADDR(port),0x05,0x70,0,TxBDefaultRouting);
}

void Gsv2k7RpllProtect(AvPort *port)
{
    uint32 PllRb1 = 0;
    uint32 PllRb2 = 0;
    uint32 PllDiff = 0;
    /* 1. Read Back Comparison Value */
    switch(port->index)
    {
        case 0:
            GSV2K7_PLL_get_RB_RXA_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV2K7_PLL_get_RB_RXA_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
            break;
        case 1:
            GSV2K7_PLL_get_RB_RXB_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV2K7_PLL_get_RB_RXB_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
            break;
        case 2:
            GSV2K7_PLL2_get_RB_RXC_PLL_REF_PRE_DIV_FREQ(port, &PllRb1);
            GSV2K7_PLL2_get_RB_RXC_PLL_VCO_POST_DIV_FREQ(port,&PllRb2);
            break;
        case 3:
            GSV2K7_PLL2_get_RB_RXD_PLL_REF_PRE_DIV_FREQ(port, &PllRb1);
            GSV2K7_PLL2_get_RB_RXD_PLL_VCO_POST_DIV_FREQ(port,&PllRb2);
            break;
        case 5:
            GSV2K7_PLL_get_RB_TXB_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV2K7_PLL_get_RB_TXB_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
            break;
    }
    /* 2. Compare Diff */
    if(PllRb1 > PllRb2)
        PllDiff = PllRb1 - PllRb2;
    else
        PllDiff = PllRb2 - PllRb1;
    /* 3. Diff Action */
    if(PllDiff>10)
    {
        switch(port->index)
        {
            case 0:
                GSV2K7_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, 1);
                GSV2K7_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 1:
                GSV2K7_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, 1);
                GSV2K7_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 2:
                GSV2K7_APLL_set_RXC_RPLL_FASTLOCK_CTL(port, 1);
                GSV2K7_APLL_set_RXC_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 3:
                GSV2K7_APLL_set_RXD_RPLL_FASTLOCK_CTL(port, 1);
                GSV2K7_APLL_set_RXD_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 5:
                GSV2K7_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, 1);
                GSV2K7_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, 0);
                break;
        }
    }
}

void Gsv2k7RxManualEQUpdate(AvPort *port, uint8 DefaultEQEnable)
{
    uint8  EqDoneFlag = 0;
    uint8  value = 0;
    uint8  i = 0;
    uint32 PhyPageAddr;
    uint8  EQParam[5]; /* 0x0C, 0x0D, 0x0E, 0x0F, 0x61 */
    uint8  RbEyeResult[2];
    uint16 BestEyeArea[3];
    uint16 BestEyeWidth[3];
    uint16 BestEyeScore[3];
    uint8  EnhancedEqMode = 0;
    uint8  EnhancedFirstRound = 0;
    /* Step 1. Protocol Check Start */
    if(DefaultEQEnable)
    {
        port->content.rx->EQDelayExpire = 0;
        port->content.rx->Lock.EqLock   = 0;
    }
    else if(port->content.rx->Lock.EqLock == 1)
    {
        return;
    }
    /* Step 2. EQ loop result check */
    /* Step 2.2 Check EQ Result */
    if(port->content.rx->EQDelayExpire >= 2)
    {
        /* Step 2.1 No need to recheck eye when already done */
        EqDoneFlag = 3;
    }
    else if(port->content.rx->EQDelayExpire == 1)
    {
        EqDoneFlag = 0;
        PhyPageAddr = GSV2K7_RXLN0_MAP_ADDR(port);
        for(i=0;i<3;i++)
        {
            AvHalI2cReadField8(PhyPageAddr,0x20,0xFF,0,&value);
            if(value == 0x10)
            {
                /* Step 2.1.1.1 Get Best Eye, if not large enough, use enhanced EQ mode to recheck */
                AvHalI2cReadMultiField(PhyPageAddr, 0x92, 2, RbEyeResult);
                BestEyeArea[i] = (RbEyeResult[0]<<8) + RbEyeResult[1];
                if(BestEyeArea[i] > 6000) /* Super Good Phase */
                    BestEyeScore[i] = 100;
                else /* Mark Score Phase */
                    BestEyeScore[i] = BestEyeArea[i]/60;
                /* Step 2.1.1.2 Check Eye Width, if not large enough use enhanced EQ mode to recheck */
                AvHalI2cReadField8(PhyPageAddr,0x96,0xFF,0,&value);
                BestEyeWidth[i] = value;
                /* Step 2.1.2 Check Valid Phase, rerun when no valid phase */
                AvHalI2cReadField8(PhyPageAddr,0x3A,0xFF,0,&value);
                if(value == 1)
                {
#if (AvAllowHpdLowEdidRead == 0)
                    /* 2.1.2.1 Low Temperature protection in Enhanced mode */
                    AvHalI2cReadField8(PhyPageAddr,0x0E,0xFF,0,&value);
                    if(value == 0x81)
                    {
                        switch(port->index)
                        {
                            case 0:
                                AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x01,0,&value);
                                if(value != 0)
                                {
                                    AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x01,0,0x00);
                                    return;
                                }
                                break;
                            case 1:
                                AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x02,0,&value);
                                if(value != 0)
                                {
                                    AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x02,0,0x00);
                                    return;
                                }
                                break;
                            case 2:
                                AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x04,0,&value);
                                if(value != 0)
                                {
                                    AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x04,0,0x00);
                                    return;
                                }
                                break;
                            case 3:
                                AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x08,0,&value);
                                if(value != 0)
                                {
                                    AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x08,0,0x00);
                                    return;
                                }
                                break;
                        }
                    }
#endif
                    /* 2.1.2.2 Normal Reset EQ process */
                    port->content.rx->EQDelayExpire = 0;
                    EnhancedEqMode = 1;
                }
                else
                {
                    AvHalI2cReadField8(PhyPageAddr,0x0E,0xFF,0,&value);
                    /* Step 2.1.3 Fast EQ mode */
                    if(value == 0x01)
                    {
                        /* Step 2.0.3.1 Eye is not good enough, check again */
                        if((BestEyeArea[i] <= 3000) || (BestEyeWidth[i] <= 12))
                        {
                            port->content.rx->EQDelayExpire = 0;
                            if(RxScdcFsm(port)[port->index] == 7)
                            {
                                EnhancedEqMode = 1;
                                EnhancedFirstRound = 1;
                            }
                            else
                            {
                                RxScdcTuneRound(port)[port->index] = RxScdcDelayExpireThreshold;
                                Gsv2k7RxScdcFsm(port);
                            }
                            break;
                        }
                        else /* Step 2.1.3.2 Lane EQ check pass */
                            EqDoneFlag = EqDoneFlag + 1;
                    }
                    /* Step 2.1.4 Enhanced EQ mode */
                    else
                    {
                        /* Step 2.1.4.1 Eye is not good enough, check again */
                        if((BestEyeArea[i] <= 1200) || (BestEyeWidth[i] <= 8))
                        {
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                            break;
                        }
                        else /* Step 2.1.4.2 Lane EQ check pass */
                            EqDoneFlag = EqDoneFlag + 1;
                    }
                }
            }
            else
            {
#if (AvAllowHpdLowEdidRead == 0)
                switch(port->index)
                {
                    case 0:
                        AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x01,0,&value);
                        if(value == 0)
                        {
                            AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x01,0,0x01);
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                        }
                        break;
                    case 1:
                        AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x02,0,&value);
                        if(value == 0)
                        {
                            AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x02,0,0x02);
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                        }
                        break;
                    case 2:
                        AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x04,0,&value);
                        if(value == 0)
                        {
                            AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x04,0,0x04);
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                        }
                        break;
                    case 3:
                        AvHalI2cReadField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x08,0,&value);
                        if(value == 0)
                        {
                            AvHalI2cWriteField8(GSV2K7_PRIM_MAP_ADDR(port),0xB6,0x08,0,0x08);
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                        }
                        break;
                }
#endif
                if((port->content.rx->EQDelayExpire == 0) && (port->content.hdcp->Hdcp2p2RxRunning == 1))
                {
                    GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                    GSV2K7_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
                }
            }
            PhyPageAddr = PhyPageAddr + 1;
        }
    }
    if(EqDoneFlag == 3)
    {
        /* Step 2.2.1 Check wheter 3 lanes have all found eyes */
        for(i=0;i<3;i++)
        {
            if(port->content.rx->EQDelayExpire == 1)
            {
                AvUapiOutputDebugMessage("Lane %d eye score  = %d", i, BestEyeScore[i]);
                AvUapiOutputDebugMessage("Lane %d eye width  = %d", i, BestEyeWidth[i]);
            }
        }
        /* Step 2.2.2 EQ Done and Eye Found */
        /* Step 2.2.2.1 Reset Rx Core Digital Logic for power cycle stability */
        if(port->content.rx->EQDelayExpire == 1)
        {
            GSV2K7_RXDIG_set_DCFIFO_RECENTER(port, 1);
            GSV2K7_RXAUD_set_RX_AUD_FIFO_RST(port, 1);
            GSV2K7_RXAUD_set_RX_AUD_FIFO_RST(port, 0);
        }
        /* Step 2.2.2.2 EQ Lock Set and Exit EQ Process */
        if(port->content.rx->EQDelayExpire >= RxEQDelayExpireThreshold)
        {
            if(port->core.HdmiCore == 0)
            {
                GSV2K7_INT_set_RX1_HS_LOCKED_CLEAR(port, 1);
                GSV2K7_INT_set_RX1_VS_LOCKED_CLEAR(port, 1);
            }
            else if(port->core.HdmiCore == 1)
            {
                GSV2K7_INT_set_RX2_HS_LOCKED_CLEAR(port, 1);
                GSV2K7_INT_set_RX2_VS_LOCKED_CLEAR(port, 1);
            }
            port->content.rx->Lock.EqLock = 1;
        }
        /* Step 2.2.2.3 EQ Done Timer Delay */
        else
        {
            port->content.rx->EQDelayExpire = port->content.rx->EQDelayExpire + 1;
            AvUapiOutputDebugMessage("Port Rx %d: EQ Timer = %d",port->index,port->content.rx->EQDelayExpire);
        }
    }
    /* Step 2.2.4 EQ ongoing */
    else if(port->content.rx->EQDelayExpire != 0)
    {
        return;
    }
    /* Step 2.3 loop to change EQ setting */
    if(port->content.rx->EQDelayExpire == 0)
    {
        /* Step 2.3.1.1 Decide EQ Parameter Plan */
        if(EnhancedEqMode == 1)
        {
            if(EnhancedFirstRound == 1)
                EQParam[1] = 0x07;
            else
                EQParam[1] = 0x81;
            EQParam[2] = 0x81;
            EQParam[3] = 0x81;
            EQParam[0] = 0x81;
            EQParam[4] = 0x01; /* Short Check Time */
        }
        else if(port->content.video->info.TmdsFreq < 250)
        {
            EQParam[0] = 0x03;
            EQParam[1] = 0x07;
            EQParam[2] = 0x01;
            EQParam[3] = 0x01;
            if(port->content.video->info.LaneFreq < 60)
                EQParam[4] = 0x01;
            else if(port->content.video->info.LaneFreq < 170)
                EQParam[4] = 0x03;
            else
                EQParam[4] = 0x0F;
        }
        else if(port->content.video->info.TmdsFreq < 470)
        {
            EQParam[0] = 0x03;
            EQParam[1] = 0x81;
            EQParam[2] = 0x01;
            EQParam[3] = 0x01;
            EQParam[4] = 0x0F;
        }
        else // 470 above timing
        {
            EQParam[0] = 0x03;
            EQParam[1] = 0x81;
            EQParam[2] = 0x01;
            EQParam[3] = 0x81;
            EQParam[4] = 0x0F;
        }
        /* Step 2.3.2 Set Lane Registers */
        PhyPageAddr = GSV2K7_RXLN0_MAP_ADDR(port);
        /* Step 2.3.2.1 Update CDR Bandwidth */
        AvHalI2cWriteField8(PhyPageAddr,0x3F,0xFF,0,0x22); //Disable EQ
        Gsv2k7UpdateRxCdrBandWidth(port);
        AvHalI2cWriteField8(PhyPageAddr,0x3F,0xFF,0,0x32); //Enable Clock Detection
        /* Step 2.3.2.2 Set EQ calculation time */
        AvHalI2cWriteMultiField(PhyPageAddr,0x0C,4,EQParam);
        AvHalI2cWriteField8(PhyPageAddr,0x61,0xFF,0,EQParam[4]);
        /* Step 2.3.2.3 Enhanced EQ Mode */
        if(EnhancedEqMode == 1)
        {
            AvUapiOutputDebugMessage("Port Rx %d: Restart Adpative EQ in Enhanced Mode", port->index);
        }
        else
        {
            AvUapiOutputDebugMessage("Port Rx %d: Reset Adpative EQ in Fast Mode", port->index);
        }
        /* Step 2.3.2.4 Run EQ */
        AvHalI2cWriteField8(PhyPageAddr,0x3F,0xFF,0,0x33); //EQ starts to run
        /* Step 2.3.2.5 Reset Lane Fifo */
        switch(port->index)
        {
            case 0:
                GSV2K7_SEC_set_RXA_FIFO_EN(port, 0);
                GSV2K7_SEC_set_RXA_FIFO_EN(port, 1);
                break;
            case 1:
                GSV2K7_SEC_set_RXB_FIFO_EN(port, 0);
                GSV2K7_SEC_set_RXB_FIFO_EN(port, 1);
                break;
            case 2:
                GSV2K7_SEC_set_RXC_FIFO_EN(port, 0);
                GSV2K7_SEC_set_RXC_FIFO_EN(port, 1);
                break;
            case 3:
                GSV2K7_SEC_set_RXD_FIFO_EN(port, 0);
                GSV2K7_SEC_set_RXD_FIFO_EN(port, 1);
                break;
        }
        /* Step 2.4 Start EQ Calibration */
        port->content.rx->EQDelayExpire = 1;
    }

    return;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckLogicAudioRx(pin AvPort *port))
{
#if AvEnableAudioTTLInput
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    uint8  PktContent[12] = {0x84,0x01,0x0A,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    uint8  i = 0;
    uint8  value = 0;

    while(KfunFindAudioNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        if(CurrentPort->type == HdmiTx)
        {
            /* 1. Routine Check */
            /* 1.1 Check Tx Sampling Frequency in SPDIF */
            if((CurrentPort->content.audio->AudFormat == AV_AUD_SPDIF) && (CurrentPort->core.HdmiCore != -1))
            {
                GSV2K7_TXDIG_get_TX_AUDIO_SPDIF_SF_RB(CurrentPort, &value);
                i = 0;
                while(ChannelStatusSfTable[i] != 0xff)
                {
                    if(ChannelStatusSfTable[i+1] == value)
                    {
                        port->content.audio->SampFreq = (AvAudioSampleFreq)(ChannelStatusSfTable[i]);
                        break;
                    }
                    i = i+3;
                }
            }
            /* 1.2 Self Capability Declare */
            port->content.audio->NValue = NTable[port->content.audio->SampFreq];
            port->content.audio->AvailableAudioPackets =
                AV_BIT_AUDIO_INFO_FRAME | AV_BIT_AUDIO_CHANNEL_STATUS | AV_BIT_AUDIO_SAMPLE_PACKET | AV_BIT_ACR_PACKET;
            /* 1.3 HDMI Tx Output is Stable to send Audio */
            if((CurrentPort->content.tx->InfoReady >= 1) &&
                /* Case 1, Audio N Value Difference */
               ((port->content.audio->NValue != CurrentPort->content.audio->NValue) ||
                /* Case 2, Sample Frequency Difference */
                (port->content.audio->SampFreq != CurrentPort->content.audio->SampFreq) ||
                /* Case 3, Channel Number Difference */
                (port->content.audio->ChanNum != CurrentPort->content.audio->ChanNum) ||
                /* Case 4, Channel Status SrcNum Difference */
                (port->content.audio->SrcNum != CurrentPort->content.audio->SrcNum) ||
                /* Case 5, Channel Status Word Length Difference */
                (port->content.audio->WordLen != CurrentPort->content.audio->WordLen) ||
                /* Case 6, Channel Status Word Length Difference */
                (port->content.audio->Consumer != CurrentPort->content.audio->Consumer) ||
                /* Case 7, Audio Coding Difference */
                (port->content.audio->AudCoding != CurrentPort->content.audio->AudCoding) ||
                /* Case 8, Audio Type Difference */
                (port->content.audio->AudType != CurrentPort->content.audio->AudType) ||
                /* Case 9, Audio Format Difference */
                (port->content.audio->AudFormat != CurrentPort->content.audio->AudFormat) ||
                /* Case 10, Audio Mclk Ratio Difference */
                (port->content.audio->AudMclkRatio != CurrentPort->content.audio->AudMclkRatio) ||
                /* Case 11, Audio Layout Difference */
                (port->content.audio->Layout != CurrentPort->content.audio->Layout) ||
                /* Case 12, Audio Packet Flag Difference */
                ((CurrentPort->content.audio->AvailableAudioPackets) &
                 (AV_BIT_AUDIO_INFO_FRAME | AV_BIT_AUDIO_CHANNEL_STATUS | AV_BIT_AUDIO_SAMPLE_PACKET | AV_BIT_ACR_PACKET)
                 != (AV_BIT_AUDIO_INFO_FRAME | AV_BIT_AUDIO_CHANNEL_STATUS | AV_BIT_AUDIO_SAMPLE_PACKET | AV_BIT_ACR_PACKET))))
                {
                    /* 2.2.1 Copy Audio Content Information */
                    AvMemcpy(CurrentPort->content.audio, port->content.audio, sizeof(AvAudio));
                    /* 2.2.2 Set AV_PKT_AUDIO_INFO_FRAME */
                    SET_AUDIF_CC(PktContent, CurrentPort->content.audio->ChanNum-1);
                    switch(CurrentPort->content.audio->ChanNum)
                    {
                        case 8:
                        case 7:
                            SET_AUDIF_CA(PktContent, 0x13);
                            break;
                        case 6:
                            SET_AUDIF_CA(PktContent, 0x0B);
                            break;
                        case 5:
                            SET_AUDIF_CA(PktContent, 0x07);
                            break;
                        case 4:
                            SET_AUDIF_CA(PktContent, 0x03);
                            break;
                        case 3:
                            SET_AUDIF_CA(PktContent, 0x01);
                            break;
                        default:
                            break;
                    }
                    AvUapiTxSetPacketContent(CurrentPort, AV_PKT_AUDIO_INFO_FRAME, PktContent,1);
                    /* 2.2.3 Set AV_PKT_AUDIO_CHANNEL_STATUS */
                    AvUapiTxSetPacketContent(CurrentPort, AV_PKT_AUDIO_CHANNEL_STATUS, PktContent,1);
                    /* 2.2.4 Set AV_BIT_AUDIO_SAMPLE_PACKET, AV_BIT_ACR_PACKET */
                    AvUapiTxSetAudioPackets(CurrentPort);
                }
            /* 1.3 Clear Tx Audio Fifo Status */
            if(CurrentPort->core.HdmiCore == 0)
                GSV2K7_INT_set_TX1_AUDIOFIFO_FULL_CLEAR(CurrentPort, 1);
            else if(CurrentPort->core.HdmiCore == 1)
                GSV2K7_INT_set_TX2_AUDIOFIFO_FULL_CLEAR(CurrentPort, 1);
        }
        PrevPort = CurrentPort;
    }
#endif
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckLogicAudioTx(pin AvPort *port))
{
#if (Gsv2k7PhyBypass == 0)
    AvPort *TempPort;
    uint8 i = 0;
    uint8 OldValue = 0;
    uint8 NewValue = 0xff;
    /* Step 1. Setup New Status */
    TempPort = (AvPort*)(port->content.RouteAudioFromPort);
    if(TempPort != NULL)
    {
        if(TempPort->type == HdmiRx)
        {
            if(TempPort->content.rx->Lock.AudioLock == 0)
                NewValue = 0xff;
            else if((TempPort->content.audio->AudCoding == AV_AUD_FORMAT_LINEAR_PCM) ||
                    (TempPort->content.audio->AudCoding == 0x00))
                NewValue = 0;
            else
                NewValue = 0xef;
        }
        else if(TempPort->type == HdmiTx)
            NewValue = 0xef;
    }
    if(port->content.audio->AudioMute == 1)
        NewValue = 0xff;
    /* Step 2. Get Current TriState Status and Update Pin Status*/
    if(port->index == 8) /* OUT AP1 */
    {
        GSV2K7_PRIM_get_AUD1_OEN(port, &OldValue);
        if(NewValue != OldValue)
        {
            GSV2K7_PRIM_set_AUD1_OEN(port, NewValue);
            if((NewValue == 0) && (port->content.audio->WordLen != 0))
            {
                GSV2K7_RXAUD_set_WORD_LEN_MAN_EN(TempPort, 1);
                GSV2K7_RXAUD_set_WORD_LEN_MAN(TempPort, port->content.audio->WordLen);
            }
        }
    }
    else if(port->index == 9) /* OUT AP2 */
    {
        GSV2K7_PRIM_get_AUD2_OEN(port, &OldValue);
        if(NewValue != OldValue)
        {
            GSV2K7_PRIM_set_AUD2_OEN(port, NewValue);
            if((NewValue == 0) && (port->content.audio->WordLen != 0))
            {
                GSV2K7_RXAUD_set_WORD_LEN_MAN_EN(TempPort, 1);
                GSV2K7_RXAUD_set_WORD_LEN_MAN(TempPort, port->content.audio->WordLen);
            }
        }
    }
    /* Step 3. check mclk ratio for I2S output */
    if(NewValue == 0)
    {
        /* 3.1 Get Mclk ratio from ChannelStatusSfTable*/
        i = 0;
        while (ChannelStatusSfTable[i+1] != 0xff)
        {
            if (ChannelStatusSfTable[i] == TempPort->content.audio->SampFreq)
            {
                NewValue = ChannelStatusSfTable[i+2];
                break;
            }
            i = i+3;
        }
        /* 3.2 Set Mclk if incorrect */
        GSV2K7_RXDIG_get_RX_AUDIO_MCLK_X_N(TempPort, &OldValue);
        if(NewValue != OldValue)
            GSV2K7_RXDIG_set_RX_AUDIO_MCLK_X_N(TempPort, NewValue);
    }
#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckVideoScaler(pin AvPort *port))
{
#if (Gsv2k7PhyBypass == 0)
    uint8 Cp1OccupyFlag = 0;
    uint8 Cp2OccupyFlag = 0;
    uint8 value1 = 0;
    uint8 NewValue1 = 0;
    uint8 value2 = 0;
    uint8 NewValue2 = 0;
    uint8 Vic = 0;

    /* 0. Check Core Connection */
    if(port == FindCp1Mode(port))
    {
        GSV2K7_SEC_get_TX1_SRC_SEL(port, &value1);
        if(value1 == 0x02)
            Cp1OccupyFlag = 1;
        else if(value1 == 0x03)
            Cp2OccupyFlag = 1;
    }
    if(port == FindCp2Mode(port))
    {
        GSV2K7_SEC_get_TX2_SRC_SEL(port, &value2);
        if(value2 == 0x02)
            Cp1OccupyFlag = 1;
        else if(value2 == 0x03)
            Cp2OccupyFlag = 1;
    }
    /* 0.1 leave routine when not used */
    if((Cp1OccupyFlag == 0) && (Cp2OccupyFlag == 0))
        return AvOk;

    /* 1. Disable CSC */
    if(Cp1OccupyFlag == 1)
        GSV2K7_SEC_get_CP1_CSC_BYPASS(port, &value1);
    else if(Cp2OccupyFlag == 1)
        GSV2K7_SEC_get_CP2_CSC_BYPASS(port, &value1);
    if(value1 == 0)
    {
        if(Cp1OccupyFlag == 1)
            GSV2K7_SEC_set_CP1_CSC_BYPASS(port, 1);
        else if(Cp2OccupyFlag == 1)
            GSV2K7_SEC_set_CP2_CSC_BYPASS(port, 1);
    }
    /* 2. Vic Mapping */
    switch(port->content.scaler->ScalerInVic)
    {
        case 93: /* 4K@24Hz */
            Vic = 93;
            port->content.scaler->ScalerOutVic = 32;
            break;
        case 98: /* 4K SMPTE@24Hz */
            Vic = 98;
            port->content.scaler->ScalerOutVic = 32;
            break;
        case 97:

        case 107: /* 4K@60Hz */
            Vic = 97;
            port->content.scaler->ScalerOutVic = 16;
            break;
        case 95: /* 4K@30Hz */
            Vic = 95;
            port->content.scaler->ScalerOutVic = 34;
            break;
        case 100: /* 4K SMPTE@30Hz */
            Vic = 100;
            port->content.scaler->ScalerOutVic = 34;
            break;
        case 102: /* 4096x2160@60Hz*/
            Vic = 102;
            port->content.scaler->ScalerOutVic = 16;
            break;
        case 94: /* 4K@25Hz */
            Vic = 94;
            port->content.scaler->ScalerOutVic = 33;
            break;
        case 96:
        case 106: /* 4K@50Hz */
            Vic = 96;
            port->content.scaler->ScalerOutVic = 31;
            break;
        case 99: /* 4K SMPTE@25Hz */
            Vic = 99;
            port->content.scaler->ScalerOutVic = 33;
            break;
        case 101: /* 4096x2160@50Hz*/
            Vic = 101;
            port->content.scaler->ScalerOutVic = 31;
            break;
        default:
            port->content.scaler->ScalerOutVic = 0;
            if(Cp1OccupyFlag == 1)
                GSV2K7_VSP_set_CP1_VID_IN(port, 0);
            else if(Cp2OccupyFlag == 1)
                GSV2K7_VSP_set_CP2_VID_IN(port, 0);
            return AvError;
    }
    /* 3. Check Vid In */
    if(Cp1OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP1_VID_IN(port, &value1);
        if(value1 != Vic)
        {
            GSV2K7_VSP_set_CP1_VID_IN(port, Vic);
            GSV2K7_VSP_set_CP1_VIN_PARAM_MAN_EN(port, 0);
            GSV2K7_VSP_set_CP1_VOUT_PARAM_MAN_EN(port, 0);
        }
    }
    else if(Cp2OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP2_VID_IN(port, &value1);
        if(value1 != Vic)
        {
            GSV2K7_VSP_set_CP2_VID_IN(port, Vic);
            GSV2K7_VSP_set_CP2_VIN_PARAM_MAN_EN(port, 0);
            GSV2K7_VSP_set_CP2_VOUT_PARAM_MAN_EN(port, 0);
        }
    }
    /* 4. Check Function Enable State */
    if(Cp1OccupyFlag == 1)
        GSV2K7_SEC_get_CP1_MODE(port,&value1);
    else if(Cp2OccupyFlag == 1)
        GSV2K7_SEC_get_CP2_MODE(port,&value1);

    switch(port->content.scaler->ColorSpace)
    {
        case AV_Y2Y1Y0_YCBCR_420:
            NewValue1 = 2; /* 420 downscaler */
            break;
        default:
            NewValue1 = 3; /* 444 downscaler */
            break;
    }
    if(value1 != NewValue1)
    {
        if(Cp1OccupyFlag == 1)
            GSV2K7_SEC_set_CP1_MODE(port, NewValue1);
        else if(Cp2OccupyFlag == 1)
            GSV2K7_SEC_set_CP2_MODE(port, NewValue1);
    }
    /* 5. Set Function Mode */
    if(Cp1OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP1_VOUT_CHN_SWAP(port, &value1);
        GSV2K7_VSP_get_CP1_VIN_CHN_SWAP(port, &value2);
    }
    else if(Cp2OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP2_VOUT_CHN_SWAP(port, &value1);
        GSV2K7_VSP_get_CP2_VIN_CHN_SWAP(port, &value2);
    }
    switch(port->content.scaler->ColorSpace)
    {
        case AV_Y2Y1Y0_YCBCR_420:
            NewValue1 = 4; /* 420 downscaler */
            NewValue2 = 0;
            break;
        default:
            NewValue1 = 0;
            NewValue2 = 0;
            break;
    }
    if((value1 != NewValue1) || (value2 != NewValue2))
    {
        if(Cp1OccupyFlag == 1)
        {
            GSV2K7_VSP_set_CP1_VOUT_CHN_SWAP(port, NewValue1);
            GSV2K7_VSP_set_CP1_VIN_CHN_SWAP(port,  NewValue2);
        }
        else if(Cp2OccupyFlag == 1)
        {
            GSV2K7_VSP_set_CP2_VOUT_CHN_SWAP(port, NewValue1);
            GSV2K7_VSP_set_CP2_VIN_CHN_SWAP(port,  NewValue2);
        }
    }
#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckVideoColor(pin AvPort *port))
{
#if (Gsv2k7PhyBypass == 0)
    uint8 Cp1OccupyFlag = 0;
    uint8 Cp2OccupyFlag = 0;
    uint8 value1 = 0;
    uint8 value2 = 0;
    uint8 NewValue1 = 0;
    uint8 NewValue2 = 0;
    AvVideoCs InCs  = AV_CS_YUV_709;
    AvVideoCs OutCs = AV_CS_YUV_709;
    uint32 temp = 0;
    uint32 OldParam = 0;
    uchar *RegTable;
    uint8 CurrentCscTable[24];
    AvPort *FromPort = NULL;
    /* 0. Check Core Connection */
    if(port == FindCp1Mode(port))
    {
        GSV2K7_SEC_get_TX1_SRC_SEL(port, &value1);
        if(value1 == 0x02)
            Cp1OccupyFlag = 1;
        else if(value1 == 0x03)
            Cp2OccupyFlag = 1;
    }
    if(port == FindCp2Mode(port))
    {
        GSV2K7_SEC_get_TX2_SRC_SEL(port, &value2);
        if(value2 == 0x02)
            Cp1OccupyFlag = 1;
        else if(value2 == 0x03)
            Cp2OccupyFlag = 1;
    }
    /* 0.1 leave routine when not used */
    if((Cp1OccupyFlag == 0) && (Cp2OccupyFlag == 0))
        return AvOk;

    /* 1. Check Vic */
    if(Cp1OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP1_VID_IN(port, &value1);
        if(value1 != port->content.color->ColorInVic)
            GSV2K7_VSP_set_CP1_VID_IN(port, port->content.color->ColorInVic);
    }
    else if(Cp2OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP2_VID_IN(port, &value1);
        if(value1 != port->content.color->ColorInVic)
            GSV2K7_VSP_set_CP2_VID_IN(port, port->content.color->ColorInVic);
    }
    /* 2. Check Input Color Space, Convert to YUV anyway */
    if(Cp1OccupyFlag == 1)
        GSV2K7_SEC_get_CP1_CSC_BYPASS(port, &value1);
    else if(Cp2OccupyFlag == 1)
        GSV2K7_SEC_get_CP2_CSC_BYPASS(port, &value1);
    if((port->content.color->ColorInSpace == AV_Y2Y1Y0_RGB) &&
       (port->content.color->ColorOutSpace != AV_Y2Y1Y0_RGB))
        NewValue1 = 0;
    else
        NewValue1 = 1;
    if(value1 != NewValue1)
    {
        if(Cp1OccupyFlag == 1)
            GSV2K7_SEC_set_CP1_CSC_BYPASS(port, NewValue1);
        else if(Cp2OccupyFlag == 1)
            GSV2K7_SEC_set_CP2_CSC_BYPASS(port, NewValue1);
    }
    if(NewValue1 == 0)
    {
        /* 3. Set CSC Value */
        InCs  = Gsv2k7ColorCsMapping(port->content.color->ColorInCs);
        OutCs = Gsv2k7ColorCsMapping(port->content.color->ColorOutCs);
        /* 3.1 Find Correct Table */
        RegTable = (uchar *)CscRgbToYcc709;
        for(temp=0;CscTables[temp].ConvTable;temp++)
        {
            if((CscTables[temp].InCs  == InCs) &&
               (CscTables[temp].OutCs == OutCs))
            {
                RegTable = (uchar *)CscTables[temp].ConvTable;
                break;
            }
        }
        if(CscTables[temp].OutCs == AV_CS_AUTO)
        {
            if(value1 == 0)
            {
                if(Cp1OccupyFlag == 1)
                    GSV2K7_SEC_set_CP1_CSC_BYPASS(port, 1);
                else if(Cp2OccupyFlag == 1)
                    GSV2K7_SEC_set_CP2_CSC_BYPASS(port, 1);
            }
        }
        else
        {
            /* Csc Start Address */
            if(Cp1OccupyFlag == 1)
                value2 = 0x20;
            else if(Cp2OccupyFlag == 1)
                value2 = 0x39;
            /* 3.2 Write Table */
            AvHalI2cReadMultiField(GSV2K7_VSP_MAP_ADDR(port), value2, 24, CurrentCscTable);
            NewValue1 = 0;
            for(temp=0;temp<24;temp++)
            {
                if(CurrentCscTable[temp] != RegTable[temp])
                {
                    NewValue1 = 1;
                    break;
                }
            }
            if(NewValue1 == 1)
            {
                AvHalI2cWriteMultiField(GSV2K7_VSP_MAP_ADDR(port), value2, 24, RegTable);
                value1 = (RegTable[0]>>5)&0x03;
                if(Cp1OccupyFlag == 1)
                    GSV2K7_VSP_set_CP1_CSC_MODE(port, value1);
                else if(Cp2OccupyFlag == 1)
                    GSV2K7_VSP_set_CP2_CSC_MODE(port, value1);
            }
        }
    }
    /* 4. Check Function Enable State */
    if(Cp1OccupyFlag == 1)
        GSV2K7_SEC_get_CP1_MODE(port,&value1);
    else if(Cp2OccupyFlag == 1)
        GSV2K7_SEC_get_CP2_MODE(port,&value1);
    if((port->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
       (port->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420))
        NewValue1 = 1; /* 444 to 420 */
    else if((port->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420) &&
            (port->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420))
        NewValue1 = 0; /* 420 to 444 */
    else
        NewValue1 = 4; /* bypass */
    if(value1 != NewValue1)
    {
        if(Cp1OccupyFlag == 1)
            GSV2K7_SEC_set_CP1_MODE(port, NewValue1);
        else if(Cp2OccupyFlag == 1)
            GSV2K7_SEC_set_CP2_MODE(port, NewValue1);
    }
    /* 5. Set Function Mode */
    if(Cp1OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP1_VOUT_CHN_SWAP(port, &value1);
        GSV2K7_VSP_get_CP1_VIN_CHN_SWAP(port, &value2);
    }
    else if(Cp2OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP2_VOUT_CHN_SWAP(port, &value1);
        GSV2K7_VSP_get_CP2_VIN_CHN_SWAP(port, &value2);
    }
    NewValue1 = 0;
    NewValue2 = 0;
    if((port->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
       (port->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420))
        NewValue2 = 3; /* 444 to 420 */
    else if((port->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420) &&
            (port->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420))
        NewValue1 = 4; /* 420 to 444 */
    if((value1 != NewValue1) || (value2 != NewValue2))
    {
        if(Cp1OccupyFlag == 1)
        {
            GSV2K7_VSP_set_CP1_VOUT_CHN_SWAP(port, NewValue1);
            GSV2K7_VSP_set_CP1_VIN_CHN_SWAP(port,  NewValue2);
        }
        else if(Cp2OccupyFlag == 1)
        {
            GSV2K7_VSP_set_CP2_VOUT_CHN_SWAP(port, NewValue1);
            GSV2K7_VSP_set_CP2_VIN_CHN_SWAP(port,  NewValue2);
        }
    }
    /* 6. Check Detail Timing Setting */
    if(((port->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
        (port->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420)) ||
       ((port->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420) &&
        (port->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420)))
        NewValue1 = 1;
    else
        NewValue1 = 0;
    temp = 0;
    while(Gsv2k7VideoColorAutoVicTable[temp] != 0xff)
    {
        /* 6.1 Found Auto Conversion Vic */
        if(port->content.color->ColorInVic == Gsv2k7VideoColorAutoVicTable[temp])
        {
            NewValue1 = 0;
            break;
        }
        temp = temp + 1;
    }
    if(Cp1OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP1_VIN_PARAM_MAN_EN(port, &value1);
        if(value1 != NewValue1)
            GSV2K7_VSP_set_CP1_VIN_PARAM_MAN_EN(port, NewValue1);
    }
    else if(Cp2OccupyFlag == 1)
    {
        GSV2K7_VSP_get_CP2_VIN_PARAM_MAN_EN(port, &value1);
        if(value1 != NewValue1)
            GSV2K7_VSP_set_CP2_VIN_PARAM_MAN_EN(port, NewValue1);
    }
    /* 6.2 Use Manual Conversion Vic */
    FromPort = (AvPort*)(port->content.RouteVideoFromPort);
    if((NewValue1 == 1) &&
       (FromPort->type == HdmiRx) &&
       (FromPort->core.HdmiCore != -1))
    {
        GSV2K7_RXDIG_get_RB_RX_H_ACTIVE_WIDTH(FromPort, &temp);
        GSV2K7_VSP_get_VIN_H_ACTIVE(port, &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_H_ACTIVE(port, temp);
        GSV2K7_RXDIG_get_RB_RX_H_TOTAL_WIDTH(FromPort, &temp);
        GSV2K7_VSP_get_VIN_H_TOTAL(port,  &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_H_TOTAL(port,  temp);
        GSV2K7_RXDIG_get_RB_RX_H_FRONT_PORCH(FromPort, &temp);
        GSV2K7_VSP_get_VIN_H_FRONT(port,  &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_H_FRONT(port,  temp);
        GSV2K7_RXDIG_get_RB_RX_H_SYNC_WIDTH(FromPort, &temp);
        GSV2K7_VSP_get_VIN_H_SYNC(port,   &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_H_SYNC(port,   temp);
        GSV2K7_RXDIG_get_RB_RX_H_BACK_PORCH(FromPort, &temp);
        GSV2K7_VSP_get_VIN_H_BACK(port,   &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_H_BACK(port,   temp);
        GSV2K7_RXDIG_get_RB_RX_V_TOTAL_0(FromPort, &temp);
        temp = temp/2;
        GSV2K7_VSP_get_VIN_V_TOTAL(port,  &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_V_TOTAL(port,  temp);
        GSV2K7_RXDIG_get_RB_RX_V_ACTIVE_HEIGHT_0(FromPort, &temp);
        GSV2K7_VSP_get_VIN_V_ACTIVE(port, &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_V_ACTIVE(port, temp);
        GSV2K7_RXDIG_get_RB_RX_V_FRONT_PORCH_0(FromPort, &temp);
        temp = temp/2;
        GSV2K7_VSP_get_VIN_V_FRONT(port,  &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_V_FRONT(port,  temp);
        GSV2K7_RXDIG_get_RB_RX_V_SYNC_WIDTH_0(FromPort, &temp);
        temp = temp/2;
        GSV2K7_VSP_get_VIN_V_SYNC(port,   &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_V_SYNC(port,   temp);
        GSV2K7_RXDIG_get_RB_RX_V_BACK_PORCH_0(FromPort, &temp);
        if(temp%2 == 1)
            temp = temp + 1;
        temp = temp/2;
        GSV2K7_VSP_get_VIN_V_BACK(port,   &OldParam);
        if(OldParam != temp)
            GSV2K7_VSP_set_VIN_V_BACK(port,   temp);
    }
#endif //(Gsv2k7PhyBypass == 0)
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckVideoGen(pin AvPort *port))
{
#if AvEnableInternalVideoGen
    AvPort *TempPort = NULL;
    uint8  i = 0;
    uint8  Value = 0;
    uint8  NewValue = 0;
    /* 1. Check Enable */
    TempPort = (AvPort*)(port->content.RouteVideoToPort);
    if(TempPort == NULL)
        NewValue = 0;
    else
        NewValue = 1;
    if(port->core.HdmiCore != -1)
    {
        GSV2K7_RXDIG_get_VPLL_MANUAL_MODE(port, &Value);
        if(Value != NewValue)
            GSV2K7_RXDIG_set_VPLL_MANUAL_MODE(port, NewValue);
    }
    GSV2K7_VSP_get_DG_EN(port, &Value);
    if(Value != NewValue)
    {
        GSV2K7_VSP_set_DG_EN(port, NewValue);
        if(NewValue == 0)
            GSV2K7_VSP_set_DG_VID(port, 0);
    }

    /* 1.1 Enable VideoGen Stable after configuration */
    port->content.rx->IsFreeRun = NewValue;

    if(NewValue == 1)
    {
        /* 2. Check Vic */
        i = 0;
        while(VideoGenVicTable[i] != 0xff)
        {
            if(VideoGenVicTable[i] == port->content.video->timing.Vic)
            {
                NewValue = VideoGenVicTable[i+1];
                GSV2K7_VSP_get_DG_VID(port, &Value);
                if(Value != NewValue)
                {
                    GSV2K7_VSP_set_DG_VID(port, NewValue);
                    if((port->core.HdmiCore == 0) || (port->core.HdmiCore == 1))
                    {
                        if(port->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
                            NewValue = VideoGenVicTable[i+2]<<1;
                        else
                            NewValue = VideoGenVicTable[i+2];
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x5D,0xFF,0,NewValue);
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x5E,0xFF,0,VideoGenVicTable[i+3]);
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x5F,0xFF,0,VideoGenVicTable[i+4]);
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x60,0xFF,0,VideoGenVicTable[i+5]);
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x61,0xFF,0,VideoGenVicTable[i+6]);
                        Value = NewValue | 0x20;
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x5D,0xFF,0,Value);
                        AvHalI2cWriteField8(GSV2K7_RXDIG_MAP_ADDR(port),0x5D,0xFF,0,NewValue);
                    }
                }
                break;
            }
            i = i+7;
        }
        /* 3. Check Pattern */
        GSV2K7_VSP_get_DG_PATTERN(port, &Value);
        if(Value != port->content.vg->Pattern)
            GSV2K7_VSP_set_DG_PATTERN(port, port->content.vg->Pattern);
        /* 4. Check 420 */
        GSV2K7_VSP_get_DG_420_EN(port, &Value);
        if(port->content.video->Y == AV_Y2Y1Y0_YCBCR_420)
            NewValue = 1;
        else
            NewValue = 0;
        if(Value != NewValue)
            GSV2K7_VSP_set_DG_420_EN(port, NewValue);
    }

#endif
    return AvOk;
}


uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckAudioGen(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv2k7, AvUapiCheckClockGen(pin AvPort *port))
{
    return AvOk;
}

#undef FindCore1
#undef FindCore2
#undef Rx1VideoInt
#undef Rx2VideoInt
#undef Rx1AudioInt
#undef Rx2AudioInt
#undef Rx1HdcpInt
#undef Rx2HdcpInt
#undef FindCp1Mode
#undef FindCp2Mode
