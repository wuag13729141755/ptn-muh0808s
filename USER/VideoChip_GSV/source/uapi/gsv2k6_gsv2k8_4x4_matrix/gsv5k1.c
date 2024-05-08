
#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "av_user_config_input.h"
#if GSV5K1
#include "gsv5k1.h"
#include "Gsv5K1_lib.h"
#include "Gsv_hal.h"
#include "Gsv_uapi.h"
#include "uapi_function_mapper.h"
#include "gsv5k1_tx_tables.h"
#include "gsv5k1_rx_tables.h"

extern uint8 TrxAvUpStreamFlag; /* 1 = Tx, 0 = Rx */

extern const AvDscPlanVicTables DscPlanVicTable[];
extern const AvVesaTables VesaTimingTables[];
extern const AvCeaTables  CeaTimingTables[];

typedef struct
{
    /* The Rx Port Core1 is connected */
    AvPort *RxCore1Occupied;
    /* The Port CP Core1 is connected */
    AvPort *Cp1CoreOccupied;
    VideoInterrupt Rx1VideoInt;
    VideoInterrupt Rx2VideoInt;
    AudioInterrupt Rx1AudioInt;
    AudioInterrupt Rx2AudioInt;
    HdcpInterrupt  Rx1HdcpInt;
    HdcpInterrupt  Rx2HdcpInt;
#if AvEdidStoredInRam
    uint8  TxAEdidRam[AvEdidMaxSize];
    uint8  TxBEdidRam[AvEdidMaxSize];
#endif
}   Resource;

static Resource InternalResource_5K1[AvGsv5k1DeviceNumber];

#define FindCore1(port)        (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].RxCore1Occupied)
#define Rx1VideoInt(port)      (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Rx1VideoInt)
#define Rx2VideoInt(port)      (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Rx2VideoInt)
#define Rx1AudioInt(port)      (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Rx1AudioInt)
#define Rx2AudioInt(port)      (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Rx2AudioInt)
#define Rx1HdcpInt(port)       (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Rx1HdcpInt)
#define Rx2HdcpInt(port)       (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Rx2HdcpInt)
#define FindCp1Mode(port)      (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].Cp1CoreOccupied)
#if AvEdidStoredInRam
#define TxAEdidRam(port)       (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].TxAEdidRam)
#define TxBEdidRam(port)       (InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex].TxBEdidRam)
#endif

#define Gsv5k1SetVideoPacketFlag(BitName) \
        if((value == 0) && ((port->content.video->AvailableVideoPackets & BitName) != 0))\
            port->content.rx->ChangedVideoPackets = port->content.rx->ChangedVideoPackets | BitName;\
        else if((value != 0) && ((port->content.video->AvailableVideoPackets & BitName) == 0))\
            port->content.rx->ChangedVideoPackets = port->content.rx->ChangedVideoPackets | BitName;\
        if(value == 0)\
            port->content.video->AvailableVideoPackets = port->content.video->AvailableVideoPackets & (~BitName);\
        else\
            port->content.video->AvailableVideoPackets = port->content.video->AvailableVideoPackets | BitName;

#define Gsv5k1SetAudioPacketFlag(BitName) \
        if((value == 0) && ((port->content.audio->AvailableAudioPackets & BitName) != 0))\
            port->content.rx->ChangedAudioPackets = port->content.rx->ChangedAudioPackets | BitName;\
        else if((value != 0) && ((port->content.audio->AvailableAudioPackets & BitName) == 0))\
            port->content.rx->ChangedAudioPackets = port->content.rx->ChangedAudioPackets | BitName;\
        if(value == 0)\
            port->content.audio->AvailableAudioPackets = port->content.audio->AvailableAudioPackets & (~BitName);\
        else\
            port->content.audio->AvailableAudioPackets = port->content.audio->AvailableAudioPackets | BitName;

#define Gsv5k1IntptSt(func, value) \
            GSV5K1_INT_get_RX1_##func##_INT_ST(port,         &value);

#define Gsv5k1IntptRaw(func) \
            GSV5K1_INT_get_RX1_##func##_RAW_ST(port,         &Value);

#define Gsv5k1IntptClr(func) \
            GSV5K1_INT_set_RX1_##func##_CLEAR(port,         1);

/* local functions */
void Gsv5k1GetRx5VStatus(pin AvPort *port);
void Gsv5k1EnableRxHpa(pin AvPort *port);
void Gsv5k1DisableRxHpa(pin AvPort *port);
void Gsv5k1ToggleTxHpd(pin AvPort *port);
void Gsv5k1ToggleTmdsOut(AvPort *port, uint8 Enable);
void Gsv5k1RxManualEQUpdate(AvPort *port, uint8 DefaultEQEnable);
void Gsv5k1SetTxHdcpVersion(pin AvPort *port, HdcpTxSupportType HdmiStyle, uint8 ForceUpdate);
void Gsv5k1UpdateRxCdrBandWidth(pin AvPort *port);
uint8 Gsv5k1TxScdcAction(pin AvPort *port, uint8 WriteEnable, uint8 DevAddr, uint8 RegAddr, uint8 Value);
uint8 Gsv5k1TxDDCError(pin AvPort *port);
void Gsv5k1ManualBksvRead(pin AvPort *port);
AvPort *Gsv5k1FindHdmiRxFront(pin AvPort *port);
AvPort *Gsv5k1FindDscFront(pin AvPort *port);
AvPort *Gsv5k1FindDscPort(pin AvPort *port);
AvVideoCs Gsv5k1ColorCsMapping(AvVideoCs InputCs);
void Gsv5k1ManualCpParameter(AvPort *port);
void Gsv5k1EnableDscOutMonitor(AvPort *port);
void Gsv5k1DisableDscOutMonitor(AvPort *port);
void Gsv5k1CheckDsc420InputSetting(AvPort *port);
void Gsv5k1TxPllUnlockClear(AvPort *port);
uint8 Gsv5k1TweakCrystalFreq(AvPort *port, uint32 value32);
void Gsv5k1MpllProtect(AvPort *port);
void Gsv5k1RpllProtect(AvPort *port);
void Gsv5k1ResetTxFifo(pin AvPort *port);
void Gsv5k1ResetVsp(pin AvPort *port);

/**
 * @brief  device init function
 * @return AvOk if success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiInitDevice(pin AvDevice *device))
{
    //Resource *InternalResource_5K1;
    uint32 i = 0;
    uint32 k = 0;
    uint32 DefaultMapAddress;
    uint32 LaneMapAddress = 0;

    AvPort *port = (AvPort *)device->port;
    Gsv5k1Device *gsv5k1Dev = (Gsv5k1Device *)device->specific;
    DefaultMapAddress = gsv5k1Dev->DeviceAddress;

    /* reset device */
//    GSV5K1_PRIM_set_MAIN_RST(port, 0);
//    GSV5K1_PRIM_set_MAIN_RST(port, 1);
    if(GSV5K1_PRIM_set_MAIN_RST(port, 0) != AvOk)
        return AvError;
    if(GSV5K1_PRIM_set_MAIN_RST(port, 1) != AvOk)
        return AvError;
    for(i=0; i<100; i++)
        GSV5K1_PRIM_set_MAIN_RST(port, 0);

    /* set default i2c address for internal maps */
    AvUapiOutputDebugMessage("Gsv5k1 - setting i2c addresses.");

    /* Write Init Table */
    if(TrxAvUpStreamFlag == 1)
    {
        for(k=0; Gsv5k1TxInitTable[k]!=0xFF; k=k+3)
            AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1TxInitTable[k],Gsv5k1TxInitTable[k+1],0xFF,0,Gsv5k1TxInitTable[k+2]);
    }
    if(TrxAvUpStreamFlag == 0)
    {
        for(k=0; Gsv5k1RxInitTable[k]!=0xFF; k=k+3)
            AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxInitTable[k],Gsv5k1RxInitTable[k+1],0xFF,0,Gsv5k1RxInitTable[k+2]);
    }

#if (Gsv5k1MuxMode == 0)
    Gsv5k1MpllProtect(port);
#endif

    //;RX inital added for adaptive EQ
    for(i=0;i<=1;i++)
    {
        switch(i)
        {
            case 0:
                LaneMapAddress  = ((port->device->index)<<24) | gsv5k1Dev_RxALane0MapAddress;
                break;
            case 1:
                LaneMapAddress  = ((port->device->index)<<24) | gsv5k1Dev_RxBLane0MapAddress;
                break;
            default:
                break;
        }
        LaneMapAddress = DefaultMapAddress | LaneMapAddress;
        for(k=0; Gsv5k1EqTable[k]!=0xFF; k=k+2)
            AvHalI2cWriteField8(LaneMapAddress,Gsv5k1EqTable[k],0xFF,0,Gsv5k1EqTable[k+1]);
    }

    LaneMapAddress = ((port->device->index)<<24) | gsv5k1Dev_TxBPhyMapAddress;
    if(TrxAvUpStreamFlag == 1)
    {
    #if (AvHdmiOnEthernet == 1)
        #if (Gsv5k1TxUseRxBHdmiMode == 1)
        /* Special RxB Ethernet Configuration */
        AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0xA0,0xFF,0,0x00);// Turn Off 5V Control
        LaneMapAddress = ((port->device->index)<<24) | gsv5k1Dev_Rx2HdmiMapAddress;
        AvHalI2cWriteField8(LaneMapAddress,0x24,0xFF,0,0x0F); // use old v lock algorithm for DSC 3BY1 detection
        #endif
        /* Special TxB Ethernet Configuration */
        #if (AvEthTxExternalInductor == 1)
        AvHalI2cWriteField8(LaneMapAddress,0xD1,0xFF,0,0xFE);
            #if (AvCompressionLowThreshold > 145)
        AvHalI2cWriteField8(LaneMapAddress,0xD2,0xFF,0,0x77);
        AvHalI2cWriteField8(LaneMapAddress,0xD3,0xFF,0,0xFC);
        AvHalI2cWriteField8(LaneMapAddress,0xD5,0xFF,0,0xBF);
        AvHalI2cWriteField8(LaneMapAddress,0xD6,0xFF,0,0x03);
        AvHalI2cWriteField8(LaneMapAddress,0xD7,0xFF,0,0x00);
            #else
        AvHalI2cWriteField8(LaneMapAddress,0xD2,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD3,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD5,0xFF,0,0xBE);
        AvHalI2cWriteField8(LaneMapAddress,0xD6,0xFF,0,0x03);
        AvHalI2cWriteField8(LaneMapAddress,0xD7,0xFF,0,0x0E); // Enhance Swing
            #endif
        #else
        AvHalI2cWriteField8(LaneMapAddress,0xD1,0xFF,0,0xFE);
        AvHalI2cWriteField8(LaneMapAddress,0xD2,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD3,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD5,0xFF,0,0xBE);
        AvHalI2cWriteField8(LaneMapAddress,0xD6,0xFF,0,0x03);
        AvHalI2cWriteField8(LaneMapAddress,0xD7,0xFF,0,0x0E); // Enhance Swing
        #endif
    #endif
    }
    if(TrxAvUpStreamFlag == 0)
    {
    #if (AvHdmiOnEthernet == 1)
        /* Special RxB Ethernet Configuration */
        AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0xA0,0xFF,0,0x00);// Turn Off 5V Control
    	LaneMapAddress = ((port->device->index)<<24) | gsv5k1Dev_Rx1HdmiMapAddress;
        AvHalI2cWriteField8(LaneMapAddress,0x24,0xFF,0,0x0F); // use old v lock algorithm for DSC 3BY1 detection
    #endif
        LaneMapAddress = ((port->device->index)<<24) | gsv5k1Dev_TxBPhyMapAddress;
    #if (AvEthCascading == 1)
        /* Special TxB Ethernet Configuration */
        #if (AvEthTxExternalInductor == 1)
        AvHalI2cWriteField8(LaneMapAddress,0xD1,0xFF,0,0xFE);
        AvHalI2cWriteField8(LaneMapAddress,0xD2,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD3,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD5,0xFF,0,0xBE);
        AvHalI2cWriteField8(LaneMapAddress,0xD6,0xFF,0,0x03);
        AvHalI2cWriteField8(LaneMapAddress,0xD7,0xFF,0,0x0E); // Enhance Swing
        #else
        AvHalI2cWriteField8(LaneMapAddress,0xD1,0xFF,0,0xFE);
        AvHalI2cWriteField8(LaneMapAddress,0xD2,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD3,0xFF,0,0xFF);
        AvHalI2cWriteField8(LaneMapAddress,0xD5,0xFF,0,0xBE);
        AvHalI2cWriteField8(LaneMapAddress,0xD6,0xFF,0,0x03);
        AvHalI2cWriteField8(LaneMapAddress,0xD7,0xFF,0,0x0E); // Enhance Swing
        #endif
    #endif
    }
#if (Gsv5k1RomFromOtp == 1)
    /* manual CP clock value setting for DSC decoding stability check */
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE3,0xFF,0,0x00); /* CP manual out clock */
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xF2,0xFF,0,0x01); /* TxA pixel div to 1 and disabled */
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xF3,0xFF,0,0x01); /* TxB pixel div to 1 and disabled */
#endif

    //AvUapiAllocateMemory(sizeof(Resource), (uint32 *)&InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex]);
    device->extension = &InternalResource_5K1[(port->device->index)-g_u8Gsv5k1StartIndex];
    FindCore1(port) = NULL;
    FindCp1Mode(port) = NULL;

#if Gsv5k1MuxMode
    GSV5K1_PRIM_set_RX1_TMDS_BYPASS_EN(port, 1);
    GSV5K1_PRIM_set_TX1_TMDS_BYPASS_EN(port, 1);
    GSV5K1_PRIM_set_TX2_TMDS_BYPASS_EN(port, 1);
#endif

    return AvOk;
}

/**
 * @brief  init rx port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxPortInit(pin AvPort *port))
{
    AvRet ret = AvOk;

    if(port->index <= 3) /* Hdmi Rx */
        Gsv5k1DisableRxHpa(port);
    return ret;
}

/**
 * @brief  init tx port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxPortInit(pin AvPort *port))
{
    AvRet ret = AvOk;
    if(port->index == 4)
    {
        port->core.HdmiCore = 0;
    }
    else if(port->index == 5)
    {
        port->core.HdmiCore = 1;
    }

#if AvEnableCecFeature /* CEC Related */
    /* Gsv5k1 has 2 input ports */
    port->content.cec->InputCount = 2;
#endif

    return ret;
}

/**
 * @brief  enable tx port core
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxEnableCore(pin AvPort *port))
{
    AvRet ret = AvOk;

    if(port->index == 4)
        GSV5K1_PRIM_set_TXA_PWR_DN(port, 0);
    else if(port->index == 5)
        GSV5K1_PRIM_set_TXB_PWR_DN(port, 0);
    else
        ret = AvError;

    return ret;
}

/**
 * @brief  disable tx port core
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxDisableCore(pin AvPort *port))
{
    AvRet ret = AvOk;
    uint32 MainMapAddress   = GSV5K1_TXDIG_MAP_ADDR(port);
    uint32 PhyMapAddress    = GSV5K1_TXPHY_MAP_ADDR(port);

    /* Disable HDCP */
    AvUapiTxDecryptSink(port);
    /* Set HDCP Version Before Reset Tx */
    Gsv5k1SetTxHdcpVersion(port, port->content.hdcptx->HdmiStyle, 0);
    /* Disable Tx TMDS Clock Driver, only enable after video is fully stable */
    Gsv5k1ToggleTmdsOut(port, 0);
    /* Reset Tx Phy SCDC state */
    AvUapiTxSetFeatureSupport(port);
    /* keep Hdmi mode */
if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
    if(port->index == 5)
    {
        AvHalI2cWriteField8(PhyMapAddress,0xD0,0xFF,0,0xFF);
        port->content.tx->HdmiMode = 1;
    }
#endif
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
    if(port->index == 5)
    {
        AvHalI2cWriteField8(PhyMapAddress,0xD0,0xFF,0,0xFF);
        port->content.tx->HdmiMode = 1;
    }
#endif
}
    AvUapiTxSetHdmiModeSupport(port);
#if AvEnableColorMatchInput
if(TrxAvUpStreamFlag == 0)
{
    if(port->index == 4)
        AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x78,0xFF,0x0,0x10);
    else if(port->index == 5)
        AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xA8,0xFF,0x0,0x10);
}
#endif
    AvHalI2cWriteField8(MainMapAddress,0x53,0xFF,0,0x04);//; Tx Color Depth = 8 bits
    AvHalI2cWriteField8(MainMapAddress,0x23,0xFF,0,0x80);//; Manual Pixel Repetition x1
    AvHalI2cWriteField8(MainMapAddress,0x20,0xFF,0,0x00);//; Tx input set to default RGB
    AvHalI2cWriteField8(MainMapAddress,0x21,0xFF,0,0x00);//; Tx output set to default RGB
    AvHalI2cWriteField8(MainMapAddress,0x00,0xFF,0,0x00);//; Mclk Tx 128*fs
    AvHalI2cWriteField8(MainMapAddress,0x50,0xFF,0,0x00);//; Disable AudioInfo/AviInfo packet
    AvHalI2cWriteField8(MainMapAddress,0x51,0xFF,0,0x00);//; Disable GC/AudioSample/N_CTS packet
    AvHalI2cWriteField8(MainMapAddress,0x2B,0x80,0,0x00);//; Disable Tx CSC
    AvHalI2cWriteField8(MainMapAddress,0x1A,0xFF,0,0xB0);//; Audio Mute and default Layout 0
    GSV5K1_TXPKT_set_TX_PKT_UPDATE(port, 0);
    /* HDCP 2.2 Stop on Error */
    GSV5K1_TX2P2_set_TX_HDCP2P2_OP_WHEN_ERROR(port, 1);
    /* Disable Tx SCDC and RR Feature as default */
    //AvHalI2cWriteField8(PhyMapAddress,0x01,0xFF,0,0x00);
    AvHalI2cWriteField8(PhyMapAddress,0x02,0xFF,0,0x00);
    /* Disable Sink SCDC and RR Analysis done */
    AvHalI2cWriteField8(PhyMapAddress,0x03,0xFF,0,0x00);

#if AvEnableHdcp2p2Feature
    /* HDCP 2.2 revocation_check_pass=1 */
    GSV5K1_TX2P2_set_REVOCATION_CHECK_PASS(port, 1);
    /* HDCP 2.2 Stop on reset */
    GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
    /* default DDC Speed */
    GSV5K1_TX2P2_set_HDCP2P2_MASTER_CLK_DIV(port, 0x14A);
    /* HDCP 2.2 i2c_clear_int_rxid_ready=1 */
    Gsv5k1AvUapiTxClearRxidReady(port);
#endif

    /* CEC Recover */
#if AvEnableCecFeature /* CEC Related */
    if(port->content.cec->CecEnable == 1)
    {
        Gsv5k1AvUapiTxCecInit(port);
        Gsv5k1AvUapiTxCecSetLogicalAddr(port);
    }
#endif

#if (Gsv5k1MuxMode == 0)
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

void Gsv5k1ResetTxFifo(pin AvPort *port)
{
    /* reset dc fifo */
    GSV5K1_TXDIG_set_DCFIFO_RESET(port, 1);
    GSV5K1_TXDIG_set_DCFIFO_RESET(port, 0);
    /* reset audio fifo */
    GSV5K1_TXDIG_set_TX_AUDIOFIFO_RESET(port, 1);
    GSV5K1_TXDIG_set_TX_AUDIOFIFO_RESET(port, 0);
    /* reset phy */
    GSV5K1_TXPHY_set_TMDS_SRST_MAN(port, 0);
    GSV5K1_TXPHY_set_TMDS_SRST_MAN(port, 1);
}

void Gsv5k1ResetVsp(pin AvPort *port)
{
    GSV5K1_PRIM_set_VSP_RST(port, 1);
    GSV5K1_PRIM_set_VSP_RST(port, 0);
}

/**
 * @brief  reset port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiResetPort(pin AvPort *port))
{
    AvRet ret = AvOk;

    if(port->type == HdmiRx)
    {
        AvUapiOutputDebugMessage("--------------Rx %d Reset Port------------", port->index+1);
        Gsv5k1DisableRxHpa(port);
        /* EQ reset to default state */
        Gsv5k1RxManualEQUpdate(port, 1);
    }
    else if(port->type == HdmiTx)
    {
        switch(port->index)
        {
            case 4: /* tx port 0 */
            case 5: /* tx port 1 */
                /* Reset Tx */
                Gsv5k1AvUapiTxDisableCore(port);
                Gsv5k1AvUapiTxEnableCore(port);
                /* Edid will be reread 15 times if failed */
                GSV5K1_TXPHY_set_TX_EDID_TRY_TIMES(port,15);
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
uapi AvRet ImplementUapi(Gsv5k1, AvUapiEnablePort(pin AvPort *port))
{
    return AvOk;
}

/**
 * @brief  free run rx port
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxEnableFreeRun(pin AvPort *port, bool enable))
{
    return AvOk;
}

/**
 * @brief  get +5v status of Rx
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGet5VStatus(pin AvPort *port))
{
    AvRet ret = AvOk;

    Gsv5k1GetRx5VStatus(port);

    return ret;
}

/**
 * @brief  get rx port status
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetStatus(pin AvPort *port))
{
    uint8 value;
    uint8 temp;
    uint32 value32;
    uint32 temp32;
    uint8 DeepColor;
    uint32 DefaultMapAddress = GSV5K1_PRIM_MAP_ADDR(port);
    uint8 k = 0;
    AvPort *DscPort = NULL;
    AvRet ret = AvOk;

    Gsv5k1GetRx5VStatus(port);

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
        if(port->content.rx->HpdDelayExpire <= RxHpdDelayExpireThreshold)
            port->content.rx->HpdDelayExpire = port->content.rx->HpdDelayExpire + 1;
        port->content.rx->IsInputStable = 0;
        port->content.video->AvailableVideoPackets = 0;
        port->content.audio->AvailableAudioPackets = 0;
    }

    if((port->core.HdmiCore != -1) && (port->content.rx->Hpd == 1))
    {
        if(port->index == 0)
        {
            GSV5K1_PLL_get_RB_RXA_PLL_VCO_FREQ(port, &value32);
            GSV5K1_PLL_get_RB_RXA_PLL_SER_DIV(port, &value);
            temp = (1<<value) * 5;
            value32 = value32/temp;
        }
        else if(port->index == 1)
        {
            GSV5K1_PLL_get_RB_RXB_PLL_VCO_FREQ(port, &value32);
            GSV5K1_PLL_get_RB_RXB_PLL_SER_DIV(port, &value);
            temp = (1<<value) * 5;
            value32 = value32/temp;
        }
        else
            value32 = 0;

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
        Gsv5k1AvUapiRxGetVideoLock(port);
        /* there is a mapped core existed */
        GSV5K1_RXDIG_get_RB_RX_HDMI_MODE(port, &(port->content.rx->HdmiMode));
        GSV5K1_RXDIG_get_RB_RX_PIXEL_REPETITION(port, &(port->content.video->PixelRepeatValue));
        GSV5K1_RXDIG_get_RB_RX_DEEP_COLOR_MODE(port, &DeepColor);

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
                if(port->content.rx->IsInputStable == 0)
                {
                    Gsv5k1ResetVsp(port);
                    port->content.video->AvailableVideoPackets = 0;
                    port->content.video->timing.Vic = 0;
                    port->content.audio->AvailableAudioPackets = 0;
                    /* when EQ locked to start audio detection,
                       avoid garbage audio packets detection */
                    GSV5K1_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 1);
                    GSV5K1_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 0);
                    /* PLL Default Setting */
#if (Gsv5k1RomFromOtp == 0)
                    GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, &value);
                    if(value == 0)
                    {
                        if(port->index == 0)
                            AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x87);
                        else if(port->index == 1)
                            AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x97);
                        for(k=0; Gsv5k1RxInNon420Table[k]!=0xFF; k=k+3)
                            AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxInNon420Table[k],
                                                Gsv5k1RxInNon420Table[k+1],0xFF,0,Gsv5k1RxInNon420Table[k+2]);
                    }
#endif
                    /* Set EMAB of DSC */
                    DscPort = (AvPort*)(Gsv5k1FindDscPort(port));
                    if(DscPort != NULL)
                    {
                        DscPort->content.dsc->SrcInY   = AV_Y2Y1Y0_YCBCR_444;
                        DscPort->content.dsc->DscInY   = AV_Y2Y1Y0_YCBCR_444;
                        DscPort->content.dsc->SrcInCd  = AV_CD_24;
                        DscPort->content.dsc->DscRatio = AV_DSC_1BY1;
                        DscPort->content.dsc->OutDelay = 0;
                    }
                }
                port->content.rx->IsInputStable = 1;
            }
            else
            {
                port->content.rx->IsInputStable = 0;
                Gsv5k1RxManualEQUpdate(port, 0);
            }
        }

        if(port->content.rx->IsInputStable == 1)
        {
            GSV5K1_RXAUD_get_ACR_PKT_DET(port, &value);
            if(value == 1)
            {
                /* only set APLL lock process after N/CTS is stable */
                GSV5K1_RXDIG_get_RX_APLL_MAN_START_CALC(port, &value);
                if(value == 1)
                {
                    GSV5K1_RXDIG_set_N_CTS_STABLE_DET_START(port, 0);
                    GSV5K1_RXDIG_set_RX_APLL_MAN_START_CALC(port, 0);
                    GSV5K1_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 1);
                    GSV5K1_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 0);
                    port->content.rx->Lock.AudioLock = 0;
                }
                /* Real Apll Lock Process*/
                else
                {
                    GSV5K1_RXAUD_get_RX_AUDIO_PLL_LOCKED(port, &port->content.rx->Lock.AudioLock);
                    if(port->content.rx->Lock.AudioLock)
                    {
                        /* Get internal actual audio mute status */
                        Gsv5k1AvUapiRxGetAudioInternalMute(port);
                        /* Possible Incorrect Garbage Audio Type Recovery */
                        AvHalI2cReadField8(GSV5K1_RXAUD_MAP_ADDR(port), 0x17, 0x1E, 0, &value);
                        if(value != 0)
                        {
                            GSV5K1_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 1);
                            GSV5K1_RXAUD_set_CLEAR_AUDIO_HB_CONTENT(port, 0);
                        }
                        /* Packet Type */
                        AvUapiRxGetPacketType(port);
                        /* Set DSD Audio Mode */
                        GSV5K1_RXAUD_get_MAN_TDM_ENABLE(port, &value);
                        if((port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                           (port->content.audio->AudType == AV_AUD_TYPE_DST))
                            temp = 0;
                        else
                            temp = 1;
                        if(temp != value)
                            GSV5K1_RXAUD_set_MAN_TDM_ENABLE(port, temp);
                        /* Channel Number */
                        GSV5K1_RXDIG_get_RB_RX_AUDIO_LAYOUT(port, &port->content.audio->Layout);
                        if(port->content.audio->Layout == 0)
                        {
                            port->content.audio->ChanNum    = 2;
                            GSV5K1_RXAUD_get_RX_AUDIO_PIN_NUM(port, &value);
                            if((port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                               (port->content.audio->AudType == AV_AUD_TYPE_DST))
                                temp = 1;
                            else
                                temp = 0;
                            if(value != temp)
                                GSV5K1_RXAUD_set_RX_AUDIO_PIN_NUM(port, temp);
                        }
                        else
                        {
                            GSV5K1_RXAUD_get_AUD_IF_PKT_DET(port, &value);
                            if(value == 1)
                            {
                                AvHalI2cReadField8(GSV5K1_RXINFO_MAP_ADDR(port),0x1D,0xFF,0,&value);
                                port->content.audio->ChanNum = (value & 0x07) + 1;
                                /* Check Audio Channel Using Logic Rather than Packet */
                                GSV5K1_RXAUD_get_NUM_CH(port, &value);
                                temp = (value-1)>>1;
                                if((port->content.audio->AudType == AV_AUD_TYPE_HBR) ||
                                   (port->content.audio->AudType == AV_AUD_TYPE_DSD) ||
                                   (port->content.audio->AudType == AV_AUD_TYPE_DST))
                                    temp = 3;
                                GSV5K1_RXAUD_get_RX_AUDIO_PIN_NUM(port, &value);
                                if(value != temp)
                                    GSV5K1_RXAUD_set_RX_AUDIO_PIN_NUM(port, temp);
                            }
                        }
                    }
                }
            }
        }
    }
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

#if (AvHdmiOnEthernet == 1)
    /*  Enable Scramble Input for CCA mode */
    GSV5K1_RXDIG_get_RX_SCDC_SCR_EN_MAN_EN(port, &value);
#if (Gsv5k1TxUseRxBHdmiMode==1)
    if(port->index == 0)
#else
    if(port->index == 1)
#endif
        temp = 1;
    else if(port->type == HdmiRx)
        temp = 0;
    if(value != temp)
    {
        GSV5K1_RXDIG_set_RX_SCDC_SCR_EN_MAN_EN(port, temp);
        GSV5K1_RXDIG_set_RX_SCDC_SCR_EN_MAN(port, temp);
    #if (AvCompressionLowThreshold > 145)
        if(temp == 1)
        {
            GSV5K1_RXDIG_set_RX_SCDC_TMDS_BIT_CLK_RATIO_MAN_EN(port, temp);
            GSV5K1_RXDIG_set_RX_SCDC_TMDS_BIT_CLK_RATIO_MAN(port, temp);
        }
    #endif
    }
#endif

    if((port->content.rx->HdmiMode == 0) && (port->content.rx->IsInputStable))
    {
        port->content.video->Y = AV_Y2Y1Y0_RGB;
        port->content.video->InCs = AV_CS_RGB;
        port->content.video->OutCs = AV_CS_RGB;
    }

    if((port->content.rx->HdmiMode) && (port->content.rx->IsInputStable))
    {
        AvHalI2cReadField8(GSV5K1_RXAUD_MAP_ADDR(port), 0x19, 0xFF, 0, &temp);
        /* AVI_INFO_FRAME */
        value = (temp>>6) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_AV_INFO_FRAME);
        /* AUDIO_INFO */
        value = (temp>>7) & 0x01;
        Gsv5k1SetAudioPacketFlag(AV_BIT_AUDIO_INFO_FRAME);
        AvHalI2cReadField8(GSV5K1_RXAUD_MAP_ADDR(port), 0x18, 0xFF, 0, &temp);
        /* SPD_INFO */
        value = (temp>>0) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_SPD_PACKET);
        /* MS_INFO */
        value = (temp>>1) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_MPEG_PACKET);
        /* VS_INFO */
        value = (temp>>2) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_VS_PACKET);
        /* ACP_PCKT */
        value = (temp>>3) & 0x01;
        Gsv5k1SetAudioPacketFlag(AV_BIT_ACP_PACKET);
        /* ISRC1_PCKT */
        value = (temp>>4) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_ISRC1_PACKET);
        /* ISRC2_PCKT */
        value = (temp>>5) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_ISRC2_PACKET);
        /* Gamut */
        value = (temp>>6) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_GMD_PACKET);
        /* ACR Packet */
        value = (temp>>7) & 0x01;
        Gsv5k1SetAudioPacketFlag(AV_BIT_ACR_PACKET);
        AvHalI2cReadField8(GSV5K1_RXAUD_MAP_ADDR(port), 0x17, 0xFF, 0, &temp);
        /* GC */
        value = (temp>>0) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_GC_PACKET);
        /* HDR_INFO */
        value = (temp>>6) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_HDR_PACKET);
#if 0
        /* EMP Packet */
        value = (temp>>7) & 0x01;
        Gsv5k1SetVideoPacketFlag(AV_BIT_EMP_PACKET);
#endif
        /* Channel Status */
        GSV5K1_RXAUD_get_CSDATA_VALID(port, &value);
        Gsv5k1SetAudioPacketFlag(AV_BIT_AUDIO_CHANNEL_STATUS);
        /* AvMute */
        GSV5K1_INT_get_RX1_AVMUTE_RAW_ST(port, &(port->content.video->Mute.AvMute));
        /* Hdmi Mode */
        GSV5K1_INT_get_RX1_HDMI_MODE_RAW_ST(port, &(port->content.rx->HdmiMode));
        /* Audio Information Protection */
        if((port->content.audio->AudType == AV_AUD_TYPE_UNKNOWN) ||
           (port->content.rx->Lock.AudioLock == 0))
            port->content.audio->AvailableAudioPackets = 0;
    }

    return ret;
}

/**
 * @brief  set HPA = high for corresponding port
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetHpdUp(pin AvPort *port))
{
    AvRet ret = AvOk;

    Gsv5k1EnableRxHpa(port);
    AvUapiOutputDebugMessage("---------------Rx %d Power Up------------", port->index+1);
    return ret;
}

/**
 * @brief  set HPA = low for corresponding port
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetHpdDown(pin AvPort *port))
{
    AvRet ret = AvOk;

    Gsv5k1DisableRxHpa(port);
    AvUapiOutputDebugMessage("--------------Rx %d Power Down-----------", port->index+1);

    return ret;
}

/**
 * @brief  mute tx tmds output
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxMuteTmds(pin AvPort *port, bool mute))
{
    uint8 Enable = 0;
    if(mute == 0)
        Enable = 1;
    Gsv5k1ToggleTmdsOut(port, Enable);
    AvUapiOutputDebugFsm("AvUapiTxMuteTmds");
    return AvOk;
}

/**
 * @brief  get tx port status
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxGetStatus(pin AvPort *port))
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
    FromPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));

    if((port->content.tx->Hpd != AV_HPD_FORCE_LOW) &&
       (port->content.tx->Hpd != AV_HPD_FORCE_HIGH))
    {
        GSV5K1_TXPHY_get_HPD_RAW_STATE(port, &value);
        /* Tx Detection Power Down */
        switch(port->index)
        {
            case 4:
                GSV5K1_INT_get_TXA_HPD_INTR_INT_ST(port, &NewValue);
                if(NewValue == 1)
                {
                    GSV5K1_INT_set_TXA_HPD_INTR_CLEAR(port, 1);
                    value = 0;
                }
                if((value == 1) && (port->content.tx->Hpd == AV_HPD_LOW))
                {
                    GSV5K1_PRIM_set_TXA_TERM_DET_PWR_DN(port, 0);
                    GSV5K1_TXPHY_get_MSEN_RAW_STATE(port, &value);
                    GSV5K1_PRIM_set_TXA_TERM_DET_PWR_DN(port, 1);
                }
                break;
            case 5:
                GSV5K1_INT_get_TXB_HPD_INTR_INT_ST(port, &NewValue);
                if(NewValue == 1)
                {
                    GSV5K1_INT_set_TXB_HPD_INTR_CLEAR(port, 1);
                    value = 0;
                }
                if((value == 1) && (port->content.tx->Hpd == AV_HPD_LOW))
                {
                    GSV5K1_PRIM_set_TXB_TERM_DET_PWR_DN(port, 0);
                    GSV5K1_TXPHY_get_MSEN_RAW_STATE(port, &value);
                    GSV5K1_PRIM_set_TXB_TERM_DET_PWR_DN(port, 1);
                }
                break;
        }
        /* EDID read delay */
        if((port->content.tx->Hpd == 0) && (value == 0))
        {
            GSV5K1_TXPHY_set_TX_HPD_SRC_SEL(port, 3); /* Always Set HPD to 1 */
            GSV5K1_TXPHY_set_TX_HPD_MAN_VALUE(port, 0); /* Toggle HPD to 0 */
        }
        else if((port->content.tx->Hpd == 1) && (value == 1))
        {
            GSV5K1_TXPHY_get_TX_HPD_SRC_SEL(port, &NewValue);
            if(NewValue == 3)
            {
                GSV5K1_TXPHY_set_TX_HPD_SRC_SEL(port, 0); /* Normal HPD Setting */
            }
        }
        port->content.tx->Hpd = (AvHpdState)value;
if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
        if(port->index == 5)
        {
            port->content.tx->Hpd = AV_HPD_HIGH;
            value = 1;
        }
#endif
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
        if(port->index == 5)
        {
            port->content.tx->Hpd = AV_HPD_HIGH;
            value = 1;
        }
#endif
}
    }

    /* Local Pll Lock Status Check, pass the test to get LocalPllLock */
    if((port->content.tx->Hpd != AV_HPD_LOW) && (FromPort != NULL))
    {
        if(port->index == 4)
        {
            /* capture pll lock low */
            if(port->content.tx->InfoReady > TxVideoManageThreshold)
            {
                GSV5K1_PLL_get_RB_TXA_PLL_LOCK_CAPTURED(port, &value);
                if(value == 0)
                    Gsv5k1TxPllUnlockClear(port);
            }
            else
            {
                GSV5K1_PLL_get_RB_TXA_PLL_LOCK(port, &value);
                if((value == 0) && (FromPort->content.rx->Lock.PllLock == 1))
                    Gsv5k1RpllProtect(port);
            }
            if(value == 1)
            {
                if(FromPort->content.rx->IsFreeRun == 1)
                    LocalPllLock = 1;
            }
        }
        else if(port->index == 5)
        {
            /* capture pll lock low */
            if(port->content.tx->InfoReady > TxVideoManageThreshold)
            {
                GSV5K1_PLL_get_RB_TXB_PLL_LOCK_CAPTURED(port, &value);
                if(value == 0)
                    Gsv5k1TxPllUnlockClear(port);
            }
            else
            {
                GSV5K1_PLL_get_RB_TXB_PLL_LOCK(port, &value);
                if((value == 0) && (FromPort->content.rx->Lock.PllLock == 1))
                    Gsv5k1RpllProtect(port);
            }
            if(value == 1)
            {
                if(FromPort->content.rx->IsFreeRun == 1)
                    LocalPllLock = 1;
            }
        }
    }

    /* Protect TMDS Output */
    if((port->content.tx->Lock.PllLock == 1) && (LocalPllLock == 0))
    {
        Gsv5k1ToggleTmdsOut(port, 0); /* Disable TMDS Out */
        AvUapiOutputDebugFsm("Port %d: PllUnlock", port->index);
    }

    /* Tx Pll Setting for 6G protection */
    if(LocalPllLock)
    {
        /* 1. Get Tx Pll Freq */
        if(port->index == 4)
        {
            GSV5K1_PLL_get_RB_TXA_PLL_REF_CLK_FREQ(port, &value32);
            /* Reread Ref Clock Frequency */
            if(Gsv5k1TweakCrystalFreq(port, value32) == 1)
                GSV5K1_PLL_get_RB_TXA_PLL_REF_CLK_FREQ(port, &value32);
            value32 = value32>>8;
            GSV5K1_PLL_get_RB_TXA_PLL_PRE_DIV(port, &value);
            value32 = value32/value;
            GSV5K1_PLL_get_RB_TXA_PLL_POST_DIV(port, &temp);
            value32 = value32 * temp;
            GSV5K1_PLL_get_RB_TXA_PLL_SER_DIV(port, &value);
            temp = (1<<value) * 5;
            value32 = value32/temp;
        }
        else if(port->index == 5)
        {
            GSV5K1_PLL_get_RB_TXB_PLL_REF_CLK_FREQ(port, &value32);
            /* Reread Ref Clock Frequency */
            if(Gsv5k1TweakCrystalFreq(port, value32) == 1)
                GSV5K1_PLL_get_RB_TXB_PLL_REF_CLK_FREQ(port, &value32);
            value32 = value32>>8;
            GSV5K1_PLL_get_RB_TXB_PLL_PRE_DIV(port, &value);
            value32 = value32/value;
            GSV5K1_PLL_get_RB_TXB_PLL_POST_DIV(port, &temp);
            value32 = value32 * temp;
            GSV5K1_PLL_get_RB_TXB_PLL_SER_DIV(port, &value);
            temp = (1<<value) * 5;
            value32 = value32/temp;
        }
        /* 1.1.1 Clock Change Detected */
        if((port->content.tx->InfoReady > TxVideoManageThreshold) &&
           (port->content.video->info.TmdsFreq != value32))
            LocalPllLock = 0;
        /* 1.1.2 TMDS Frequency Update */
        port->content.video->info.TmdsFreq = (uint16)value32;
        /* 1.2 Get Tx Pll Freq */
        if(port->index == 4)
            GSV5K1_PLL_get_TXA_HDMI2_MODE_AUTO(port, &Freq3G4Plus);
        else if(port->index == 5)
            GSV5K1_PLL_get_TXB_HDMI2_MODE_AUTO(port, &Freq3G4Plus);
        /* 2. Set Phy Value Action */
        if(Freq3G4Plus == 1)
            NewValue = 1;
        else
            NewValue = 0;
        /* 3.1 Phy Mode Change */
        GSV5K1_TXPHY_get_TX_HDMI2_MODE_MAN(port, &value);
#if ((AvHdmiOnEthernet == 1) && (AvCompressionLowThreshold > 145))
        if(port->index == 5)
            NewValue = 1; /* always enable scramble for Ethernet port */
#endif
        if(value != NewValue)
        {
            GSV5K1_TXPHY_set_TX_HDMI2_MODE_MAN_EN(port, 1);
            GSV5K1_TXPHY_set_TX_HDMI2_MODE_MAN(port, NewValue);
        }
        /* 3.2 Scramble Change */
        GSV5K1_TXPHY_get_TX_SCR_EN_MAN(port, &value);
if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
        if(port->index == 5)
            NewValue = 1; /* always enable scramble for Ethernet port */
#endif
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
        if(port->index == 5)
            NewValue = 1; /* always enable scramble for Ethernet port */
#endif
}
        if(value != NewValue)
        {
            GSV5K1_TXPHY_set_TX_SCR_EN_MAN(port, NewValue);
            GSV5K1_TXPHY_set_TX_SCR_EN_MAN_EN(port, 1);
        }
        /* 3.3 SCDC Change */
        GSV5K1_TXPHY_get_EDID_ANALYZE_DONE(port, &value);
        if(value == 0)
            GSV5K1_TXPHY_set_EDID_ANALYZE_DONE(port, 1);
    }

    /* Find Final Lock Status, only pass the test to have Tx Pll Lock */
    if(port->content.tx->InfoReady == 1)
    {
        Gsv5k1ResetTxFifo(port);
        AvUapiTxSetHdmiModeSupport(port);
    }
    if(port->content.tx->InfoReady == TxVideoManageThreshold)
    {
        AvUapiOutputDebugFsm("Port %d: Tx Pixel Frequency = %d",port->index, port->content.video->info.TmdsFreq);
        port->content.tx->InfoReady = port->content.tx->InfoReady + 1;
        /* 7.0 Tx Phy Setting */
if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
            if(port->index == 4)
#endif
            {
                if(port->content.video->info.TmdsFreq > 340)
                {
                #if (AvEnableTxCtsPhySetting == 1)
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xF0);
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x53);
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0F);
                #else
                    /* Slew Rate: 0x14/0x34/0x54/0x74 */
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,AvGsvTxPhyValue_GTR340M);//Tx Phy Characater, Preemp = 101
                #endif
                }
                else
                {
                #if (AvEnableTxCtsPhySetting == 1)
                    if(port->content.video->info.TmdsFreq > 250)
                    {
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xFE);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x27);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0F);
                    }
                    else
                    {
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xF0);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x40);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0C);
                    }
                #else
                    /* Slew Rate: 0x10/0x30/0x50/0x70 */
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,AvGsvTxPhyValue_LEQ340M);//Tx Phy Characater, Preemp = 100
                #endif
                }
            }
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
            if(port->index == 4)
#endif
            {
                if(port->content.video->info.TmdsFreq > 340)
                {
                #if (AvEnableTxCtsPhySetting == 1)
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xF0);
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x53);
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0F);
                #else
                    /* Slew Rate: 0x14/0x34/0x54/0x74 */
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,AvGsvTxPhyValue_GTR340M);//Tx Phy Characater, Preemp = 101
                #endif
                }
                else
                {
                #if (AvEnableTxCtsPhySetting == 1)
                    if(port->content.video->info.TmdsFreq > 250)
                    {
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xFE);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x27);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0F);
                    }
                    else
                    {
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD1,0xFF,0,0xF0);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,0x40);
                        AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0x3C,2,0x0C);
                    }
                #else
                    /* Slew Rate: 0x10/0x30/0x50/0x70 */
                    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,AvGsvTxPhyValue_LEQ340M);//Tx Phy Characater, Preemp = 100
                #endif
                }
            }
}
        /* 7.1 Protect for HDCP 2.2 Restart, disable EDID and SCDC in advance */
        GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
        GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 0);
        /* 7.2 Manual Write SCDC Scramble and Clock Divider */
        GSV5K1_TXPHY_set_RX_SCDC_PRESENT(port, 0); /* no disturb on sink PLL lock from DDC */
        if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC) != 0x0)
        {
            Gsv5k1ToggleTmdsOut(port, 0); /* Disable TMDS Out */
            if(NewValue == 1) /* Set SCDC */
                NewValue = 0x03;
            else
                NewValue = 0x00;
            Gsv5k1TxScdcAction(port, 1, 0xA8, 0x01, 0x01);
            Gsv5k1TxScdcAction(port, 1, 0xA8, 0x20, NewValue);
            Gsv5k1ToggleTmdsOut(port, 1); /* Enable TMDS Out */
        }
        else
        {
            GSV5K1_TXPHY_get_TX_TMDS_CLK_DRIVER_ENABLE(port, &value);
            if(value == 0)
                Gsv5k1ToggleTmdsOut(port, 1);
        }
        /* 7.3 Clear PLL Lock Status when entering stable */
        Gsv5k1TxPllUnlockClear(port);
    }

    /* handle SCDC errors */
#if AvEnableTxScdcCedCheck
    if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC) != 0x0)
    {
        if(port->content.tx->InfoReady == TxHdcpManageThreshold)
        {
            GSV5K1_TXPHY_get_TX_SCDC_EN(port, &value);
            if(value == 0)
            {
                GSV5K1_TXPHY_set_TX_SCDC_EN(port, 1);
                GSV5K1_TXPHY_set_TX_RR_CAPABLE(port, 0); /* Auto CED readback */
                GSV5K1_TXPHY_set_RX_SCDC_PRESENT(port, 1); /* SCDC status check enable */
            }
        }
        else if(port->content.tx->InfoReady > TxHdcpManageThreshold)
        {
            GSV5K1_TXPHY_get_RX_SCDC_UPDATE_0(port, &value);
            if(value != 0x00)
            {
                value = value & 0x03; /* set task */
                GSV5K1_TXPHY_set_TX_CLR_UPDATE_FLAG(port, value);
                value = value | 0x80; /* task action */
                AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0x07,0xFF,0,value);
                AvUapiOutputDebugFsm("Port %d: Clear Interrupt = %x",port->index, value);
            }
        }
    }
#endif

    /* Phy Interface Parameter Setting */
if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
    if(port->content.tx->InfoReady == (TxVideoManageThreshold - 1))
    {
        if(port->index == 5)
        {
    #if (AvEthTxExternalInductor == 1)
            if(port->content.video->info.TmdsFreq > 140)
            {
                value = 0x9F;
                temp  = 0xBF;
            }
            else if(port->content.video->info.TmdsFreq > 40)
            {
                value = 0x51;
                temp  = 0xBD;
            }
            else
            {
            // 0x75 for short reach,
            // 0x55 for long reach,
            // 0xE0 for worst cable,
            // 0xD5 for normal
                value = 0xD5;
                temp  = 0xBD;
            }
    #else
            if(port->content.video->info.TmdsFreq > 80)
            {
                value = 0x9F;
                temp  = 0xBF;
            }
            else
            {
                value = 0xFF;// 0xE0 for worst cable
                temp  = 0xBE;
            }
    #endif
            AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD0,0xFF,0,0xEF);
            AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,value);
            AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0xFF,0,temp);
        }
    }
    if((port->content.tx->Lock.PllLock == 0) && (LocalPllLock == 1))
    {
        if(port->index == 5)
        {
            /* low freq 8to16bit conversion */
            if((FromPort != NULL) && (FromPort->type == HdmiRx) &&
                (port->content.RouteVideoFromPort == (struct AvPort*)FromPort))
            {
                GSV5K1_TXDIG_get_TX_GC_CD(port, &value);
                if((port->content.video->info.TmdsFreq < 32) && (value == 4))
                {
                    GSV5K1_TXDIG_set_TX_PKT_GC_ENABLE(port, 1);
                    GSV5K1_TXDIG_set_TX_GC_CD(port, 7); /* modify to 16-bit */
                }
                else if((port->content.video->info.TmdsFreq >= 64) && (value == 7))
                {
                    GSV5K1_TXDIG_set_TX_GC_CD(port, 4); /* modify to 8-bit */
                }
            }
        }
    }
#endif
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
    if(port->content.tx->InfoReady == (TxVideoManageThreshold - 1))
    {
        if(port->index == 5)
        {
    #if (AvEthTxExternalInductor == 1)
            if(port->content.video->info.TmdsFreq > 140)
            {
                value = 0x97;
                temp  = 0xBF;
            }
            else if(port->content.video->info.TmdsFreq > 40)
            {
                value = 0x51;
                temp  = 0xBD;
            }
            else
            {
                value = 0xE0;// 0x75 for short reach, 0x55 for long reach, 0xE0 for worst cable
                temp  = 0xBD;
            }
    #else
            if(port->content.video->info.TmdsFreq > 80)
            {
                value = 0x9F;
                temp  = 0xBF;
            }
            else
            {
                value = 0xFF;// 0xE0 for worst cable
                temp  = 0xBE;
            }
    #endif
            AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD4,0xFF,0,value);
            AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port),0xD5,0xFF,0,temp);
        }
    }
#endif
}

    /* Update Pll Lock Status*/
    if((port->content.tx->Lock.PllLock == 0) && (LocalPllLock == 1))
    {
        GSV5K1_TXPHY_set_TMDS_SRST_MAN(port, 0);
        GSV5K1_TXPHY_set_TMDS_SRST_MAN(port, 1);
    }
    port->content.tx->Lock.PllLock = LocalPllLock;

    if(port->content.tx->Lock.PllLock)
    {
        /* Pixel Repetition for 480i/576i */
        value32 = port->content.video->info.TmdsFreq >> port->content.video->PixelRepeatValue;
        if((value32 < 0x0f) && (value32 != 0x00))
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
                Gsv5k1_TxSendAVInfoFrame(port, Packet, 1);
            }
        }
        if(port->content.tx->InfoReady >= TxVideoManageThreshold)
        {
            /* Video Black Mute */
            GSV5K1_TXDIG_get_TX_BLACK_VIDEO_EN(port, &value);
            if(value != port->content.video->Mute.BlkMute)
                GSV5K1_TXDIG_set_TX_BLACK_VIDEO_EN(port, port->content.video->Mute.BlkMute);
            /* Audio Mute */
            GSV5K1_TXDIG_get_TX_AUDIO_MUTE(port, &value);
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
                GSV5K1_TXDIG_set_TX_AUDIO_MUTE(port, NewValue);
        }
       /* Check Dsc Tx Path Enabling */
        FromPort = (AvPort*)port->content.RouteVideoFromPort;
        if(FromPort != NULL)
        {
            if((FromPort->type == VideoDsc) && (FromPort->content.dsc->DscInCompressed == 0))
                NewValue = 1;
            else
                NewValue = 0;
        }
        else
            NewValue = 0;
        if(port->index == 4)
        {
            GSV5K1_SEC_get_TX1_DSC_EN(port, &value);
            if(value != NewValue)
                GSV5K1_SEC_set_TX1_DSC_EN(port, NewValue);
        }
        else if(port->index == 5)
        {
if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
            NewValue = 1;
#endif
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
            NewValue = 1;
#endif
}
            GSV5K1_SEC_get_TX2_DSC_EN(port, &value);
            if(value != NewValue)
                GSV5K1_SEC_set_TX2_DSC_EN(port, NewValue);
        }
    }
    else
    {
        port->content.video->AvailableVideoPackets = 0;
        port->content.audio->AvailableAudioPackets = 0;
    }

    /* Edid Status Update */
    GSV5K1_TXPHY_get_TX_EDID_READY_RB(port,    &value);
    if(port->content.tx->IgnoreEdidError != 0)
    {
        /* no need to read EDID if not required */
        port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
    }
    else if((value == 1) && (port->content.tx->EdidReadSuccess != AV_EDID_UPDATED))
    {
        /* EDID not updated yet, update it */
        if(port->content.tx->EdidReadSuccess == AV_EDID_RESET)
        {
            /* Manually set Edid Read Success, EDID will be read in Event */
            port->content.tx->EdidReadSuccess = AV_EDID_SUCCESS;
        }
#if (AvEdidStoredInRam == 1)
        /* Increment Edid Segment Ptr */
        GSV5K1_TXPHY_get_TX_DDC_READ_EDID_RAM_PART(port, &NewValue);
        if(NewValue == 0)
        {
            /* HF-EEODB */
            AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0x84, 3, Packet);
            if((Packet[0] == 0xE2) && (Packet[1] == 0x78))
                NewValue = Packet[2];
            else
                AvHalI2cReadField8(GSV5K1_TXEDID_MAP_ADDR(port), 0x7E, 0xFF, 0, &NewValue);
            if((NewValue > 1) && (NewValue < 0x80))
            {
                switch(port->index)
                {
                    case 4:
                        AvMemset(TxAEdidRam(port), 0, AvEdidMaxSize);
                        AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, 256, TxAEdidRam(port));
                        break;
                    case 5:
                        AvMemset(TxBEdidRam(port), 0, AvEdidMaxSize);
                        AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, 256, TxBEdidRam(port));
                        break;
                }
                AvUapiOutputDebugMessage("Port%d: Read Segment 0",port->index);
                GSV5K1_TXPHY_set_TX_DDC_READ_EDID_RAM_PART(port, 1);
                GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 0);
                GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 1);
                port->content.tx->EdidReadSuccess = AV_EDID_RESET;
            }
        }
        else
        {
            switch(port->index)
            {
                case 4:
                    if((TxAEdidRam(port)[0x84] == 0xE2) && (TxAEdidRam(port)[0x85] == 0x78))
                        value = TxAEdidRam(port)[0x86]/2;
                    else
                        value = TxAEdidRam(port)[0x7E]/2;
                    break;
                case 5:
                    if((TxBEdidRam(port)[0x84] == 0xE2) && (TxBEdidRam(port)[0x85] == 0x78))
                        value = TxBEdidRam(port)[0x86]/2;
                    else
                        value = TxBEdidRam(port)[0x7E]/2;
                    break;
            }
            if(NewValue < value)
            {
                if(NewValue >= (AvEdidMaxSize/256))
                    value32 = (AvEdidMaxSize/256) - 1;
                else
                    value32 = NewValue;
                if(value32 != 0)
                {
                    switch(port->index)
                    {
                        case 4:
                            AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, 256, TxAEdidRam(port)+(256*value32));
                            break;
                        case 5:
                            AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, 256, TxBEdidRam(port)+(256*value32));
                            break;
                    }
                }
                NewValue = NewValue + 1;
                AvUapiOutputDebugMessage("Port%d: Read Segment %d",port->index, NewValue);
                GSV5K1_TXPHY_set_TX_DDC_READ_EDID_RAM_PART(port, NewValue);
                GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 0);
                GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 1);
                port->content.tx->EdidReadSuccess = AV_EDID_RESET;
            }
        }
#endif
    }
    else if((port->content.tx->Hpd == AV_HPD_HIGH) &&
            (port->content.tx->EdidReadSuccess == AV_EDID_RESET))
    {
        if(TrxAvUpStreamFlag == 1)
        {
            #if (AvHdmiOnEthernet == 1)
            if(port->index == 5)
            {
                port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
                port->content.tx->EdidSupportFeature = (~(AV_BIT_FEAT_DSC_DECODE | AV_BIT_FEAT_SCDC));
                port->content.tx->HdmiMode = 1;
            }
            else
            #endif
            {/* HDMI Tx Port Edid Process Start */
                GSV5K1_TXPHY_get_TX_HDCP1P4_STATE_RB(port, &value);
                if(value != 1)
                {
                    /* DDC circuit error */
                    value = Gsv5k1TxDDCError(port);
                    if(value == 1)
                    {
                        AvUapiOutputDebugFsm("Port %d: DDC Fail, Abort",port->index);
                        port->content.tx->EdidReadFail    = 0;
                        Gsv5k1ToggleTxHpd(port);
                        #if AvForceDefaultEdid
                        port->content.tx->EdidReadSuccess = AV_EDID_FAIL;
                        port->content.tx->IgnoreEdidError = 1;
                        #endif
                    }
                }
                /* if has already tried 15 times, that means failed */
                GSV5K1_TXPHY_get_TX_EDID_TRY_TIMES(port, &value);
                if(value == 0)
                {
                    AvUapiOutputDebugFsm("Port %d: Edid Fail, Retry",port->index);
                    /* set flag */
                    GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
                    Gsv5k1ToggleTxHpd(port);
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
            }/* HDMI Tx Port Edid Process End */
        }
        if(TrxAvUpStreamFlag == 0)
        {
            #if (AvEthCascading == 1)
            if(port->index == 5)
            {
                port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
                port->content.tx->EdidSupportFeature = (~AV_BIT_FEAT_DSC_DECODE);
                port->content.tx->HdmiMode = 1;
            }
            else
            #endif
            {/* HDMI Tx Port Edid Process Start */
                /* DDC circuit error */
                value = Gsv5k1TxDDCError(port);
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
                GSV5K1_TXPHY_get_TX_EDID_TRY_TIMES(port, &value);
                if(value == 0)
                {
                    AvUapiOutputDebugFsm("Port %d: Edid Fail, Retry",port->index);
                    /* set flag */
                    GSV5K1_TXPHY_set_TX_EDID_TRY_TIMES(port, 15);
                    GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 1);
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
            }/* HDMI Tx Port Edid Process End */
        }
    }

    /* 10. Manual BKSV Read Protection */
#if AvEnableHdcp1p4BksvCheck
    if((port->content.tx->EdidReadSuccess == AV_EDID_UPDATED) &&
        (port->content.hdcptx->HdcpSupport == 0))
        Gsv5k1ManualBksvRead(port);
#endif

    return ret;
}

/**
 * @brief  Tx video management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxVideoManage(pin AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    uint8 NewValue = 0;
    uchar *RegTable;
    uint8 HdrA4Current[2] = {0x00,0x00};
    uint8 HdrC3Current[2] = {0xFF,0xFF};
    uint8 HdrC4Current[2] = {0x00,0x00};
    uint8 HdrA4Correction[2] = {0x00,0x00};
    uint8 HdrC4Correction[2] = {0x00,0x00};
    AvPort *FromPort = NULL;
    AvPort *DscPort = (AvPort*)(Gsv5k1FindDscFront(port));

    /* 1. DVI mode check */
    if(port->content.tx->HdmiMode == 0)
    {
        /* 1.1 Input Color for Black Mute */
        if(port->content.video->Y == AV_Y2Y1Y0_RGB)
            NewValue = 0;
        else
            NewValue = 1;
        GSV5K1_TXDIG_get_TX_VIDEO_INPUT_CS(port, &value);
        if(value != NewValue)
            GSV5K1_TXDIG_set_TX_VIDEO_INPUT_CS(port, NewValue);
        /* 1.2 DVI color space is always RGB */
        GSV5K1_TXDIG_get_TX_VIDEO_OUTPUT_FORMAT(port, &value);
        if(value != 0)
            GSV5K1_TXDIG_set_TX_VIDEO_OUTPUT_FORMAT(port, 0);
        /* 1.3 color manage */
        if(DscPort != NULL)
        {
            /* 1.3.1 get CSC table */
            if((port->content.video->Y == AV_Y2Y1Y0_YCBCR_444) ||
               (port->content.video->Y == AV_Y2Y1Y0_YCBCR_422))
            {
                NewValue = 1;
                if((port->content.video->InCs == AV_CS_YUV_709) ||
                   (port->content.video->InCs == AV_CS_YCC_709))
                    RegTable = (uchar *)CscYcc709ToRgb;
                else
                    RegTable = (uchar *)CscYcc601ToRgb;
            }
            else
            {
                NewValue = 0;
            }
            /* 1.3.2 check current color setting */
            /* 1.3.2.1 csc enable */
            GSV5K1_TXDIG_get_TX_VIDEO_CSC_ENABLE(port, &value);
            if(value != NewValue)
                GSV5K1_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, NewValue);
            /* 1.3.2.2 csc coeffs */
            if(NewValue == 1)
            {
                if((DscPort->content.dsc->SrcInY == AV_Y2Y1Y0_RGB) ||
                   (DscPort->content.dsc->SrcInY == AV_Y2Y1Y0_YCBCR_420))
                    AvHalI2cReadField8(GSV5K1_TXDIG_MAP_ADDR(port), 0x2D, 0xFF, 0, &value);
                else
                    AvHalI2cReadField8(GSV5K1_TXDIG_MAP_ADDR(port), 0x2F, 0xFF, 0, &value);
                if(value != RegTable[1])
                {
                    if((DscPort->content.dsc->SrcInY == AV_Y2Y1Y0_RGB) ||
                       (DscPort->content.dsc->SrcInY == AV_Y2Y1Y0_YCBCR_420))
                    {
                        AvHalI2cWriteMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 24, RegTable);
                    }
                    else
                    {
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2E, 2, (uint8 *)(RegTable)+0);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 2, (uint8 *)(RegTable)+2);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x30, 4, (uint8 *)(RegTable)+4);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x36, 2, (uint8 *)(RegTable)+8);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x34, 2, (uint8 *)(RegTable)+10);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x38, 4, (uint8 *)(RegTable)+12);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x3E, 2, (uint8 *)(RegTable)+16);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x3C, 2, (uint8 *)(RegTable)+18);
                        AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x40, 4, (uint8 *)(RegTable)+20);
                    }
                    value = (RegTable[0]>>5)&0x03;
                    GSV5K1_TXDIG_set_TX_VIDEO_CSC_SCALING_FACTOR(port, value);
                }
            }
        } /* if(DscPort != NULL) */
    }
    else
    {
        if(TrxAvUpStreamFlag == 0)
        {
            /* 1.4 HDR deep color recovery */
            FromPort = (AvPort*)(port->content.RouteVideoFromPort);
            /* Match Case 1: HDR output */
            if(((port->content.video->AvailableVideoPackets & AV_BIT_HDR_PACKET) != 0) &&
            /* Match Case 2: HDMI output port, not compressed output port */
               ((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) == 0) &&
            /* Match Case 3: not RGB output for color conversion */
               (port->content.video->Y != AV_Y2Y1Y0_RGB) &&
            /* Match Case 4: Dsc/Scaler/Color for 8-bit processing */
               ((FromPort->type == VideoDsc) ||
                (FromPort->type == VideoScaler) ||
                (FromPort->type == VideoColor)))
            {
                /* Match Case 5.1: deep color */
                if((port->content.video->Y  == AV_Y2Y1Y0_YCBCR_420) &&
                   ((port->content.video->Cd == AV_CD_30) ||
                    (port->content.video->Cd == AV_CD_36)))
                {
                    HdrA4Correction[0] = 0x00;
                    HdrA4Correction[1] = 0x00;
                    HdrC4Correction[0] = CscTxHdrRecovery[22];
                    HdrC4Correction[1] = CscTxHdrRecovery[23];
                }
                /* Match Case 5.2: YCbCr-422 */
                else if((port->content.video->Y  == AV_Y2Y1Y0_YCBCR_422) ||
                        (port->content.video->Cd == AV_CD_30) ||
                        (port->content.video->Cd == AV_CD_36))
                {
                    HdrA4Correction[0] = CscTxHdrRecovery[22];
                    HdrA4Correction[1] = CscTxHdrRecovery[23];
                    HdrC4Correction[0] = CscTxHdrRecovery[22];
                    HdrC4Correction[1] = CscTxHdrRecovery[23];
                }
                GSV5K1_TXDIG_get_TX_VIDEO_CSC_ENABLE(port, &value);
                AvHalI2cRdMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x40, 2, HdrC3Current);
                if(((HdrC3Current[0] == 0x80) && (HdrC3Current[1] == 0x00)) || (value == 0))
                {
                    /* 1.5 Read Current Status */
                    if(value == 1)
                    {
                        AvHalI2cRdMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x32, 2, HdrA4Current);
                        AvHalI2cRdMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x42, 2, HdrC4Current);
                    }
                    /* 1.6 Compare and Action */
                    if((HdrA4Current[0] != HdrA4Correction[0]) ||
                       (HdrA4Current[1] != HdrA4Correction[1]) ||
                       (HdrC4Current[0] != HdrC4Correction[0]) ||
                       (HdrC4Current[1] != HdrC4Correction[1]))
                    {
                        Gsv5k1_TxSetCSC(port);
                    }
                }
            }
        }
    }
    /* 2. GC color depth is fixed to 8-bit */
    if((port->content.tx->HdmiMode == 0) || (port->content.video->info.TmdsFreq > 450))
    {
        GSV5K1_TXDIG_get_TX_GC_CD(port, &value);
        if(value != 4)
        {
            AvUapiTxSetHdmiDeepColor(port); /* fixed to 8-bit */
            port->content.video->AvailableVideoPackets =
                port->content.video->AvailableVideoPackets & (~AV_BIT_GC_PACKET);
            //AvUapiOutputDebugMessage("Port%d: Trigger GC Resend! by %dMHz", port->index, port->content.video->info.TmdsFreq);
        }
    }
    /* 3. Tx Dither Mode Setting */
    AvHalI2cReadField8(GSV5K1_TXDIG_MAP_ADDR(port),0xE5,0xFF,0,&value);
#if (AvEnableDitherView == 1)
    if((DscPort != NULL) &&
       ((port->content.video->Y != AV_Y2Y1Y0_YCBCR_420) &&
        (port->content.video->Y != AV_Y2Y1Y0_YCBCR_422)) &&  /* No Dither for 420/422 Output */
       (DscPort->content.dsc->DscInCompressed == 1) &&       /* Match 1: HDMI->DSC Compression */
       ((DscPort->content.dsc->SrcInCd == AV_CD_30) ||
        (DscPort->content.dsc->SrcInCd == AV_CD_36)))        /* Match 2: Deep Color Mode */
        NewValue = 0x8C; /* Tx Dither Enabled */
    else
#endif
        NewValue = 0x0D; /* Tx Dither Disabled */
    if(value != NewValue)
        AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0xE5,0xFF,0,NewValue);

    return ret;
}

/**
 * @brief  Tx audio management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxAudioManage(pin AvPort *port))
{
    AvRet ret = AvOk;
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
        GSV5K1_TXDIG_get_TX_PKT_AUDIO_SAMPLE_ENABLE(port, &value);
        if(value == 1)
            GSV5K1_TXDIG_set_TX_PKT_AUDIO_SAMPLE_ENABLE(port, 0);
    }
    else
    {
        /* Audio Manage: audio might underflow or overflow,
           change AUDIOIF_CC or AUDIOIF_CA will fix it */
        if(port->index == 4)
            GSV5K1_INT_get_TX1_AUDIOFIFO_FULL_RAW_ST(port, &value);
        else if(port->index == 5)
            GSV5K1_INT_get_TX2_AUDIOFIFO_FULL_RAW_ST(port, &value);
        if(value == 1)
        {
            /* toggle AUDIOIF_CC */
            GSV5K1_TXPKT_get_AUD_IF_CA7_0(port, &TempStoreCA);
            value = TempStoreCA + 1;
            GSV5K1_TXPKT_set_AUD_IF_CA7_0(port, value);
            GSV5K1_TXPKT_set_AUD_IF_CA7_0(port, TempStoreCA);
            AvUapiOutputDebugMessage("Port%d: Audio Recover From Fifo Full", port->index);
            if(port->index == 4)
                GSV5K1_INT_set_TX1_AUDIOFIFO_FULL_CLEAR(port, 1);
            else if(port->index == 5)
                GSV5K1_INT_set_TX2_AUDIOFIFO_FULL_CLEAR(port, 1);
        }
    }

    return ret;
}

/**
 * @brief  Rx video management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxVideoManage(pin AvPort *port, VideoInterrupt* Intpt))
{
    return AvOk;
}

/**
 * @brief  Rx audio management
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxAudioManage(pin AvPort *port, AudioInterrupt* Intpt))
{
    uint8 DsdMan=1, DsdSel=0, HbrMan=1, HbrSel=0;
    uint8 ApllRecalcFlag = 0;
    AvRet ret = AvOk;

    if(Intpt->AudChanMode)
    {
        ApllRecalcFlag = 1;
    }
    if(Intpt->InternMute)
    {
        Gsv5k1AvUapiRxSetAudioInternalMute(port);
        ret = AvOk;
    }
    if(Intpt->CsDataValid)
    {
        switch (port->content.audio->AudFormat)
        {
            case AV_AUD_I2S:
                GSV5K1_RXAUD_set_RX_DIS_I2S_ZERO_COMPR(port, 1);
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

        GSV5K1_RXAUD_set_RX_DSD_OUT_MAN_EN(port, DsdMan);
        GSV5K1_RXAUD_set_RX_DSD_OUT_MAN_SEL(port, DsdSel);
        GSV5K1_RXAUD_set_RX_HBR_OUT_MAN_EN(port, HbrMan);
        GSV5K1_RXAUD_set_RX_HBR_OUT_MAN_SEL(port, HbrSel);
        ret = AvOk;
    }
    if(Intpt->NChange)
    {
        ApllRecalcFlag = 1;
        ret = AvOk;
    }
    if(Intpt->CtsThresh)
    {
        //ApllRecalcFlag = 1;
        ret = AvOk;
    }
    if(Intpt->AudFifoOv)
    {
        ApllRecalcFlag = 1;
        ret = AvOk;
    }
    if(Intpt->AudFifoUn)
    {
        ApllRecalcFlag = 1;
        ret = AvOk;
    }
    if(Intpt->AudFifoNrOv)
        ret = AvOk;
    if(Intpt->AudioPktErr)
        ret = AvOk;
    if(Intpt->AudModeChg)
    {
        /* Switch between DSD/I2S, data rate varies triggers Audio Mute */
        GSV5K1_RXAUD_set_DISABLE_AUDIO_MUTE(port, 1);
        GSV5K1_RXAUD_set_DISABLE_AUDIO_MUTE(port, 0);
        ret = AvOk;
    }
    if(Intpt->AudFifoNrUn)
        ret = AvOk;
    /*
    if(Intpt->AudFlatLine)
        ret = AvOk;
    */
    if(Intpt->AudSampChg)
    {
        ret = AvOk;
    }
    if(Intpt->AudPrtyErr)
    {
        ret = AvOk;
    }
    if(Intpt->AudIfValid)
    {
        ret = AvOk;
    }
    if(Intpt->AcpValid)
    {
        ret = AvOk;
    }

    if(ApllRecalcFlag == 1)
    {
        GSV5K1_RXDIG_set_N_CTS_STABLE_DET_START(port, 1);
        GSV5K1_RXDIG_set_RX_APLL_MAN_START_CALC(port, 1);
    }

    return ret;
}

/**
 * @brief  connect rx and tx
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiConnectPort(pin AvPort *FromPort, pin AvPort *ToPort, AvConnectType type))
{
    AvRet ret = AvOk;
    uint8 value1 = 0;
    uint8 value2 = 0;
    uint32 k = 0;
    AvPort *TempPort = NULL;
    AvPort *DeSelPort = NULL;
    uint32 DefaultMapAddress = GSV5K1_PRIM_MAP_ADDR(FromPort);

    /* 1. Disconnet invalid port */
    value2 = ToPort->device->index;
    if((ToPort->type == HdmiTx) && (type != AvConnectAudio))  /* Tx core with video connection */
    {
        if(ToPort->content.tx->InfoReady > TxVideoManageThreshold)
            ToPort->content.tx->Hpd = AV_HPD_FORCE_LOW;
        else
            ToPort->content.tx->InfoReady = 0;
        /* not current selection, reset the port */
        if((FindCore1(ToPort) != FromPort) && /* not current selection */
           (FromPort->type == HdmiRx))
        {
            if(FindCore1(ToPort) != NULL)
                Gsv5k1AvUapiDisconnectPort(FindCore1(ToPort));
#if (Gsv5k1RomFromOtp == 0)
            GSV5K1_PRIM_get_ONLY_USE_RX1_EN(FromPort, &value1);
            if(value1 == 0)
            {
                if(FromPort->index == 0)
                    AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x87);
                else if(FromPort->index == 1)
                    AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x97);
                for(k=0; Gsv5k1RxInNon420Table[k]!=0xFF; k=k+3)
                    AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxInNon420Table[k],
                                        Gsv5k1RxInNon420Table[k+1],0xFF,0,Gsv5k1RxInNon420Table[k+2]);
            }
#endif
        }
        Gsv5k1AvUapiTxDisableCore(ToPort);
    }
    /* 1.1 Avoid conflict because Color Port and Scaler Port share logic */
    else if((ToPort->index == 20) || (ToPort->index == 24))
    {
        /* 1.1.1 Find the port index that should be disconnected */
        switch(ToPort->index)
        {
            case 20:
                value1 = 24;
                break;
            case 24:
                value1 = 20;
                break;
        }
        /* 1.1.2 Find the port pointer that should be disconnected */
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
if(TrxAvUpStreamFlag == 0)
{
    /* 1.2 Clean DSC port if not used */
    if((FromPort->type == HdmiRx) && (ToPort->type != VideoDsc))
    {
        value1 = 16;
        TempPort = ToPort->device->port; /* Get 1st Port of the device */
        while((TempPort != NULL) && (TempPort->device->index == value2))
        {
            if(TempPort->index == value1)
            {
                TempPort->content.RouteVideoFromPort = NULL;
                TempPort->content.RouteVideoToPort   = NULL;
                TempPort->content.RouteVideoNextSameLevelPort = NULL;
            }
            else
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
            }
            TempPort = (AvPort*)(TempPort->next);
        }
    }
}

    /* Step 2. Set port infrastructure content */
    if(ToPort->type == HdmiTx)
    {
        if(FromPort->type == HdmiRx)
            FromPort->core.HdmiCore = 0;   /* switch to Rx1 Core */
        /* 2.1 Disable InfoFrames in advance */
        if(type == AvConnectAudio)
        {
            ToPort->content.audio->AvailableAudioPackets = 0;
            Gsv5k1AvUapiTxEnableInfoFrames (ToPort, (AV_BIT_ACR_PACKET |
                                              AV_BIT_AUDIO_SAMPLE_PACKET), 0);
        }
        else
        {
            ToPort->content.tx->Lock.PllLock = 0;
            ToPort->content.video->AvailableVideoPackets = 0;
            ToPort->content.audio->AvailableAudioPackets = 0;
            Gsv5k1AvUapiTxEnableInfoFrames (ToPort, 0xffff, 0);
        }
    }

    /* Step 3. Switch Port Selection */
    switch(ToPort->index)
    {
        case 4:
            /* To TxA */
            switch(FromPort->index)
            {
                case 0:
                case 1:
                    /* From Rx Port */
                    if(type != AvConnectAudio)
                    {
                        GSV5K1_PRIM_set_RX1_CORE_SEL(FromPort, FromPort->index);
                        GSV5K1_SEC_set_TX1_SRC_SEL(FromPort, 0);
                    }
                    if(ToPort->content.RouteAudioFromPort == (struct AvPort*)FromPort)
                        GSV5K1_PRIM_set_TX1_AUD_SRC_SEL(FromPort, 1);
                    break;
                case 10:
                    GSV5K1_PRIM_set_AUD1_OEN(FromPort, 0xff);
                    GSV5K1_PRIM_set_AP1_OUT_SRC_SEL(FromPort, 0);
                    GSV5K1_PRIM_set_TX1_AUD_SRC_SEL(FromPort, 5);
                    break;
                case 16:
                    /* From Dsc Port */
                    FromPort->content.dsc->OutDelay = 0x00; /* Reset FromPort OutDelay */
                    GSV5K1_SEC_set_TX1_SRC_SEL(FromPort, 3);
                    break;
                case 20:
                    /* From Scaler Port */
                    GSV5K1_SEC_set_TX1_SRC_SEL(FromPort, 2);
                    break;
                case 24:
                    /* From Color Port */
                    GSV5K1_SEC_set_TX1_SRC_SEL(FromPort, 2);
                    break;
                case 28:
                    /* From VG */
                    GSV5K1_SEC_set_TX1_SRC_SEL(FromPort, 4);
                    break;
            }
            break;
        case 5:
            /* To TxB */
            switch(FromPort->index)
            {
                case 0:
                case 1:
                    /* From Rx Port */
                    if(type != AvConnectAudio)
                    {
                        GSV5K1_PRIM_set_RX1_CORE_SEL(FromPort, FromPort->index);
                        GSV5K1_SEC_set_TX2_SRC_SEL(FromPort, 0);
                    }
                    if(ToPort->content.RouteAudioFromPort == (struct AvPort*)FromPort)
                        GSV5K1_PRIM_set_TX2_AUD_SRC_SEL(FromPort, 1);
                    break;
                case 10:
                    GSV5K1_PRIM_set_AUD1_OEN(FromPort, 0xff);
                    GSV5K1_PRIM_set_AP1_OUT_SRC_SEL(FromPort, 0);
                    GSV5K1_PRIM_set_TX2_AUD_SRC_SEL(FromPort, 5);
                    break;
                case 16:
                    /* From Dsc Port */
                    FromPort->content.dsc->OutDelay = 0x00; /* Reset FromPort OutDelay */
                    GSV5K1_SEC_set_TX2_SRC_SEL(FromPort, 3);
                    break;
                case 20:
                    /* From Scaler Port */
                    GSV5K1_SEC_set_TX2_SRC_SEL(FromPort, 2);
                    break;
                case 24:
                    /* From Color Port */
                    GSV5K1_SEC_set_TX2_SRC_SEL(FromPort, 2);
                    break;
                case 28:
                    /* From VG */
                    GSV5K1_SEC_set_TX2_SRC_SEL(FromPort, 4);
                    break;
            }
            break;
        case 8:
            /* Mute Output */
            GSV5K1_PRIM_set_AUD1_OEN(FromPort, 0xff);
            switch(FromPort->index)
            {
                case 0:
                case 1:
                    value1 = 1;
                    break;
                case 4:
                    AvUapiTxArcEnable(FromPort, 1);
                    value1 = 3;
                    break;
                case 5:
                    AvUapiTxArcEnable(FromPort, 1);
                    value1 = 4;
                    break;
                default:
                    value1 = 0;
                    break;
            }
            GSV5K1_PRIM_set_AP1_OUT_SRC_SEL(FromPort, value1);
            break;
        case 10:
            GSV5K1_PRIM_set_AUD1_OEN(FromPort, 255);
            break;
        case 16:
            /* To Dsc Port */
            switch(FromPort->index)
            {
                case 0:
                case 1:
                    /* From Rx Port */
                    GSV5K1_SEC_set_VP_SRC_SEL(FromPort, 0);
                    break;
                case 24:
                    /* From CP */
                    GSV5K1_SEC_set_VP_SRC_SEL(FromPort, 2);
                    break;
                case 28:
                    /* From VG */
                    GSV5K1_SEC_set_VP_SRC_SEL(FromPort, 3);
                    break;
            }

            if(ToPort->content.dsc->DscInCompressed == 1)
            {
                GSV5K1_SEC_set_VP_MODE(FromPort, 0); /* Decode Mode */
                //GSV5K1_SEC_set_DSC_MODE(FromPort, 0);
            }
            else
            {
                GSV5K1_SEC_set_VP_MODE(FromPort, 1); /* Encode Mode */
                //GSV5K1_SEC_set_DSC_MODE(FromPort, 1);
            }
            GSV5K1_DSC_set_DSC_VIC(FromPort, 0); /* Reset VIC */
            ToPort->content.dsc->OutDelay = 0;
            break;
        case 20:
            /* To Scaler Port */
            switch(FromPort->index)
            {
                case 0:
                case 1:
                    /* From Rx Port */
                    GSV5K1_SEC_set_CP_SRC_SEL(FromPort, 0);
                    break;
                case 16:
                    /* From VP */
                    GSV5K1_SEC_set_CP_SRC_SEL(FromPort, 2);
                    break;
                case 28:
                    /* From VG */
                    GSV5K1_SEC_set_CP_SRC_SEL(FromPort, 3);
                    break;
            }
            GSV5K1_VSP_set_CP1_VID_IN(FromPort, 0);
            break;
        case 24:
            /* To Color Port */
            switch(FromPort->index)
            {
                case 0:
                case 1:
                    /* From Rx Port */
                    GSV5K1_SEC_set_CP_SRC_SEL(FromPort, 0);
                    break;
                case 16:
                    /* From VP */
                    GSV5K1_SEC_set_CP_SRC_SEL(FromPort, 2);
                    break;
                case 28:
                    /* From VG */
                    GSV5K1_SEC_set_CP_SRC_SEL(FromPort, 3);
                    break;
            }
            GSV5K1_VSP_set_CP1_VID_IN(FromPort, 0);
            break;
    }
    if(ToPort->type == VideoScaler)
        AvMemset((void*)(&ToPort->content.scaler->timing), 0, sizeof(VideoTiming));
    if(ToPort->type == VideoColor)
        AvMemset((void*)(&ToPort->content.color->timing), 0, sizeof(VideoTiming));
    if((FromPort->type == VideoDsc) || (ToPort->type == VideoDsc))
    {
        if(FromPort->type == VideoDsc)
            FromPort->content.dsc->OutDelay = 0x00; /* Reset FromPort OutDelay */
        if(ToPort->type == VideoDsc)
            ToPort->content.dsc->OutDelay = 0x00; /* Reset FromPort OutDelay */
        GSV5K1_DSC_set_DSC_MEM_EMAB(FromPort, 3);/* enable DSC output reset */
        Gsv5k1ResetVsp(FromPort);
    }

    /* Step 4. Set Core Occupied */
    if(FromPort->index < 4)
        FindCore1(FromPort) = FromPort;
    if((ToPort->index == 20) || (ToPort->index == 24))
        FindCp1Mode(ToPort) = ToPort;

    /* Step 5. Set 5V Detection Mode */
    //GSV5K1_PRIM_set_RX_5V_DET_MAN_EN(FromPort, 0);
if(TrxAvUpStreamFlag == 0)
{
    /* Front End is always 5V enabled */
    GSV5K1_PRIM_set_RX_5V_DET_MAN_EN(FromPort, 2);
}

#if AvEnableInternalVideoGen
    /* Step 7. VideoGen Process */
    if(FromPort->index == 28)
    {
        /* 7.1 Select VideoGen Clock Input */
        FromPort->core.HdmiCore = 0; /* use Rx1 */
        GSV5K1_PLL_set_VG_CLK_SRC_SEL(FromPort, 0);
        /* 7.2 Select VideoGen Clock Input */
        if(ToPort->index == 4)
            GSV5K1_SEC_set_TX1_SRC_SEL(FromPort, 4);
        else if(ToPort->index == 5)
            GSV5K1_SEC_set_TX2_SRC_SEL(FromPort, 4);
    }
    else
    {
        if((FromPort->core.HdmiCore != -1) &&
           ((FromPort->index == 0) || (FromPort->index == 1)))
            GSV5K1_RXDIG_set_VPLL_MANUAL_MODE(FromPort, 0);
    }
#endif

    return ret;
}

/**
 * @brief  disconnect rx and tx
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiDisconnectPort(pin AvPort *port))
{
    if((port->index == 4) || (port->index == 5))
    {
        /* 1. Disconnect Tx Port */
        Gsv5k1AvUapiTxDisableCore(port);
        port->core.HdmiCore = -1;
    }
    else if(port->index <= 3)
    {
        /* 1. Disconnect Rx Port */
        Gsv5k1DisableRxHpa(port);
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
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetAudioPackets(pin AvPort *port))
{
    AvRet ret = AvInvalidParameter;
    uint8 value = 0;

    if((port->content.tx->Hpd != AV_HPD_HIGH) ||
       (port->content.tx->Lock.PllLock == 0) ||
       (port->content.tx->Lock.AudioLock == 0))
    {
        port->content.audio->AvailableAudioPackets =
            port->content.audio->AvailableAudioPackets &
            (0xFFFF - AV_BIT_AUDIO_SAMPLE_PACKET - AV_BIT_AUDIO_INFO_FRAME - AV_BIT_ACR_PACKET);
        Gsv5k1AvUapiTxEnableInfoFrames(port, AV_BIT_AUDIO_SAMPLE_PACKET |
                                   AV_BIT_AUDIO_INFO_FRAME |
                                   AV_BIT_ACR_PACKET, 0);
        return ret;
    }

    if((port->index == 4) || (port->index == 5))
    {
        /* Step 1. Select Mclk Source */
        if(port->content.audio->AudType == AV_AUD_TYPE_SPDIF)
            value = 0;
        else
            value = 1;
        GSV5K1_TXDIG_set_TX_AUDIO_EXT_MCLK_ENABLE(port, value);
        /* Step 2. Select Correct Audio Input */
        if(port->content.audio->AudType == AV_AUD_TYPE_UNKNOWN)
            value = 0;
        else
            value = port->content.audio->AudType;
        GSV5K1_TXDIG_set_TX_AUDIO_MODE(port, value);
        /* Step 4. Enable Audio Interface */
        Gsv5k1_TxSetAudioInterface(port);
        /* Step 5. Enable Audio Sampling Frequency */
        Gsv5k1_TxSetAudChStatSampFreq(port);
        /* Step 6. Enable N Value */
        Gsv5k1AvUapiTxSetAudNValue(port);
        /* Finally. Enable the packet */
        Gsv5k1AvUapiTxEnableInfoFrames(port, AV_BIT_AUDIO_SAMPLE_PACKET | AV_BIT_ACR_PACKET, 1);
        ret = AvOk;
    }

    return ret;
}

/**
 * @brief  Clear Rx Flag indication
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxClearFlags(pio AvPort *port))
{
    AvRet ret = AvOk;
    if(port->core.HdmiCore != -1)
    {
        GSV5K1_RXAUD_set_RX_PKT_DET_RST(port, 1);
    }
    return ret;
}

/**
 * @brief  Read Rx Video Packet Status
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetVideoPacketStatus(pin AvPort *port, VideoInterrupt* Intpt))
{
    uint8 Value = 0;
    uint8 MapData[43];
    uint16 AvailablePacket = 0;

    AvMemset(Intpt, 0, sizeof(VideoInterrupt));

    /* 1. Read Map Data */
    if(port->core.HdmiCore == 0)
        Value = 0x30;
    else
        return AvNotAvailable;
    AvHalI2cReadMultiField(GSV5K1_INT_MAP_ADDR(port),Value,15,MapData+0);
    if(port->core.HdmiCore == 0)
        Value = 0x50;
    AvHalI2cReadMultiField(GSV5K1_INT_MAP_ADDR(port),Value,28,MapData+15);

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
            AvHalI2cReadField8(GSV5K1_RXINFO_MAP_ADDR(port),0x00,0xFF,0,&Value);
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
    if(((MapData[14] & 0x10) != 0) || ((MapData[42] & 0x01) != 0))
        Intpt->EmpValid = 1;
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
    if(Intpt->EmpValid == 1)
            AvUapiOutputDebugMessage("Port%d: Intpt->EmpValid",port->index);
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

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetPacketType(pin AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    uint8 temp  = 0;

    GSV5K1_RXAUD_get_AUDIO_SAMPLE_PKT_DET(port, &value);
    if(value == 1)
    {
        port->content.audio->AudFormat  = AV_AUD_I2S;
        port->content.audio->AudType    = AV_AUD_TYPE_ASP;
    }
    else
    {
        GSV5K1_RXAUD_get_DSD_AUDIO_PKT_DET(port, &value);
        if(value == 1)
        {
            port->content.audio->AudFormat  = AV_AUD_DSD_NORM;
            port->content.audio->AudType    = AV_AUD_TYPE_DSD;
        }
        else
        {
            GSV5K1_RXAUD_get_DST_AUDIO_PKT_DET(port, &value);
            if(value == 1)
            {
                port->content.audio->AudFormat  = AV_AUD_DSD_DST;
                port->content.audio->AudType    = AV_AUD_TYPE_DST;
            }
            else
            {
                GSV5K1_RXAUD_get_HBR_AUDIO_PKT_DET(port, &value);
                if(value == 1)
                {
                    port->content.audio->AudFormat  = AV_AUD_I2S;
                    port->content.audio->AudType    = AV_AUD_TYPE_HBR;
                }
                else
                {
                    port->content.audio->AudFormat = AV_AUD_IF_AUTO;
                    port->content.audio->AudType   = AV_AUD_TYPE_UNKNOWN;
                }
            }
        }
    }
    /* HBR Special Setting */
    GSV5K1_RXAUD_get_RX_SEL_I2S(port, &value);
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
        GSV5K1_RXAUD_set_RX_SEL_I2S(port, temp);
    return ret;
}

/**
 * @brief  Read Rx Audio Packets
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetAudioPacketStatus(pin AvPort *port, AudioInterrupt* Intpt))
{
    uint8 MapData[42];
    uint8 Value = 0;

    AvMemset(Intpt, 0, sizeof(AudioInterrupt));

    /* 1. Read Map Data */
    if(port->content.rx->Lock.AudioLock)
    {
        if(port->core.HdmiCore == 0)
            Value = 0x30;
        else
            return AvNotAvailable;
        AvHalI2cReadMultiField(GSV5K1_INT_MAP_ADDR(port),Value,15,MapData+0);
        if(port->core.HdmiCore == 0)
            Value = 0x50;
        else if(port->core.HdmiCore == 1)
            Value = 0x70;
        AvHalI2cReadMultiField(GSV5K1_INT_MAP_ADDR(port),Value,27,MapData+15);
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
        Gsv5k1AvUapiRxGetPacketType(port);
    if((MapData[23] & 0x03) != 0)
        Intpt->AudFifoNrUn = 1;
    /*
    if((MapData[24] & 0x03) != 0)
        Intpt->AudFlatLine = 1;
    */
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

    return AvOk;
}

/**
 * @brief  Check HDCP Encryption
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetHdcpStatus(pin AvPort *port, HdcpInterrupt* Intpt))
{
    AvRet ret = AvOk;
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

#if ((AvHdmiOnEthernet == 1))
        /* Dedicated RxB is used as Ethernet Port, HDCP is sent through InfoFrame */
        /* No Update of Hdcp Status */
        #if (Gsv5k1TxUseRxBHdmiMode==1)
        if(port->index == 0)
        #else
        if(port->index == 1)
        #endif
        {
            if(port->content.rx->IsInputStable == 0)
                port->content.rx->VideoEncrypted = 0;
            return AvOk;
        }
#endif

    AvMemset(Intpt, 0, sizeof(HdcpInterrupt));

    if(port == FindCore1(port))
    {
        /* hdcp */
        GSV5K1_INT_get_RX1_AKSV_UPDATE_INT_ST(port,       &Intpt->AksvUpdate);
        GSV5K1_INT_get_RX1_HDCP1P4_ENCRYPTED_INT_ST(port, &Intpt->Encrypted);
        GSV5K1_INT_get_RX1_AKSV_UPDATE_RAW_ST(port,       &port->content.hdcp->AksvInterrupt);
        GSV5K1_INT_get_RX1_HDCP1P4_ENCRYPTED_RAW_ST(port, &port->content.rx->VideoEncrypted);
        GSV5K1_INT_get_RX1_HDCP2P2_RX_AUTH_RAW_ST(port,   &Rx2p2Stat);
    }
    else
    {
        return AvNotSupport;
    }

    if(Intpt->Encrypted)
    {
        port->content.rx->ChangedVideoPackets = port->content.rx->ChangedVideoPackets | AV_BIT_AV_INFO_FRAME;
        if(Rx2p2Stat == 0)
        {
            if(port->content.rx->VideoEncrypted == 1)
                AvUapiOutputDebugMessage("Port Rx %d: Rx is encrypted with HDCP 1.4", (port->index));
        }
        else if(Rx2p2Stat == 1)
        {
            if(port->content.rx->VideoEncrypted == 1)
                AvUapiOutputDebugMessage("Port Rx %d: Rx is encrypted with HDCP 2.2", (port->index));
        }
    }

    /* Check HDCP State */
    GSV5K1_PRIM_get_RX_HDCP_CFG_MAN_EN(port, &PreHdcpManual);
    GSV5K1_PRIM_get_RX_CFG_TO_HDCP2P2(port,  &PreHdcpVersion);
    if(port == FindCore1(port))
    {
        HdcpManual     = PreHdcpManual  & 0x01;
        HdcpVersion    = PreHdcpVersion & 0x01;
        PreHdcpManual  = PreHdcpManual  & 0x02;
        PreHdcpVersion = PreHdcpVersion & 0x02;
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
                        GSV5K1_TXPHY_get_TX_HDCP2P2_CAPABILITY_RB(CurrentPort, &value);
                        GSV5K1_TXPHY_get_TX_HDCP2P2_CHECKED(CurrentPort, &value1);
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
        GSV5K1_PRIM_set_RX_HDCP_CFG_MAN_EN(port, HdcpManual);
        GSV5K1_PRIM_set_RX_CFG_TO_HDCP2P2(port,  HdcpVersion);
    }

    /* Update VideoEncrypted Status */
    if(Intpt->Encrypted)
    {
        /* When Found Rx has started being encrypted ,
           start to check Encryption is not stopped by AvMute */
        if(port == FindCore1(port))
            GSV5K1_INT_set_RX1_HDCP1P4_ENCRYPTED_CLEAR(port,1);
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
    GSV5K1_RX2P2_get_RX_DISABLE_ECC_CHECK(port, &value);
    value1 = 1 - port->content.rx->IsInputStable;
    if(value1 != value)
        GSV5K1_RX2P2_set_RX_DISABLE_ECC_CHECK(port, value1);

#endif

    return ret;
}

/**
 * @brief  Check Tx HDCP Encryption Capability
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxGetHdcpStatus(pin AvPort *port))
{
    AvRet ret = AvOk;
    uint8 Desire1p4 = 0;
    uint8 Desire2p2 = 0;
    uint8 Error1 = 0;
    uint8 Error2 = 0;
    uint8 Error3 = 0;
    uint8 Error4 = 0;
    uint8 value = 0;
    uint8 Fsm1p4State = 0;
    uint8 Fsm2p2State = 0;

if(TrxAvUpStreamFlag == 1)
{
#if (AvHdmiOnEthernet == 1)
    if(port->index == 5)
    {
        port->content.hdcptx->HdcpSupport = 1;
        return ret;
    }
#endif
}
if(TrxAvUpStreamFlag == 0)
{
#if (AvEthCascading == 1)
    if(port->index == 5)
    {
        port->content.hdcptx->HdcpSupport = 1;
        return ret;
    }
#endif
}
    /* Set HDCP Version with 2 Steps */
    /* Step 1. Disable Data Encryption */
    if(port->content.hdcptx->HdcpModeUpdate == 1)
    {
        GSV5K1_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 0);
        GSV5K1_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, 0);
        port->content.hdcptx->HdcpModeUpdate = port->content.hdcptx->HdcpModeUpdate + 1;
    }
    /* Step 2. Reset HDCP Tx Version */
    else if(port->content.hdcptx->HdcpModeUpdate == 2)
    {
        Gsv5k1SetTxHdcpVersion(port, port->content.hdcptx->HdmiStyle, 1);
        port->content.hdcptx->HdcpModeUpdate = 0;
    }
    /* 0. HDCP Support */
    if((port->content.hdcptx->HdcpSupport == 0) &&
       (port->content.hdcptx->Hdcp2p2SinkSupport == 0))
        return AvOk;
    GSV5K1_TXPHY_get_TX_HDCP1P4_DESIRED(port, &Desire1p4);
    GSV5K1_TX2P2_get_TX_HDCP2P2_DESIRED(port, &Desire2p2);
    if((Desire1p4 != 0) || (Desire2p2 != 0))
        port->content.hdcptx->HdcpEnabled = 1;
    else
        port->content.hdcptx->HdcpEnabled = 0;
    /* 1. Check HDCP 1.4 Errors */
    GSV5K1_TXPHY_get_TX_HDCP1P4_STATE_RB(port,   &Fsm1p4State);
    GSV5K1_TXPHY_get_TX_HDCP1P4_ERROR_FLAG(port, &Error1);
    GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV_READ_FLAG(port, &port->content.hdcptx->BksvReady);
    /* 1.1 HDCP 1.4 Running check */
    if((Fsm1p4State == 4) && (port->content.hdcptx->Authenticated == 0))
        AvUapiOutputDebugMessage("Port Tx %d: Tx is Running HDCP 1.4", (port->index-4));
    /* 2. Check HDCP 2.2 Errors */
#if AvEnableHdcp2p2Feature
    AvHalI2cReadField8(GSV5K1_TX2P2_MAP_ADDR(port), 0x0D, 0xFF, 0, &Error2);
    AvHalI2cReadField8(GSV5K1_TX2P2_MAP_ADDR(port), 0x0E, 0xFF, 0, &Error3);
    AvHalI2cReadField8(GSV5K1_TX2P2_MAP_ADDR(port), 0x14, 0xFF, 0, &Fsm2p2State);
    if(Fsm2p2State == 0x00)
    {
        AvHalI2cReadField8(GSV5K1_TX2P2_MAP_ADDR(port), 0x13, 0xFF, 0, &value);
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
            AvUapiOutputDebugMessage("Port Tx %d: Tx is Stopping HDCP 2.2", (port->index-4));
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
    else
    {
        if(port->content.video->Mute.BlkMute == 1)
        {
            if((Fsm1p4State == 0x06) && ((Fsm2p2State & 0x81) == 0x00))
                Error4 = 1;
            if((Fsm1p4State == 0x03) || (Fsm1p4State == 0x05))
                Error1 = 1;
        }
    }
    /* 6. Disable HDCP encryption or Mute */
    switch(port->content.hdcptx->HdmiStyle)
    {
        case AV_HDCP_TX_AUTO:
            port->content.video->Mute.BlkMute = Error1 | Error4;
            port->content.audio->AudioMute    = Error1 | Error4;
#if Gsv5k1MuxMode
            if((port->content.hdcptx->HdcpError >= AvHdcpTxErrorThreshold) ||
               (port->content.hdcptx->HdcpSupport == 0))
                Gsv5k1ToggleTmdsOut(port, 0);
#else
            if(port->content.hdcptx->HdcpSupport == 0)
                Gsv5k1ToggleTmdsOut(port, 0);
#endif
            break;
        case AV_HDCP_TX_2P2_ONLY:
            port->content.video->Mute.BlkMute = Error4;
            port->content.audio->AudioMute    = Error4;
#if Gsv5k1MuxMode
            if((port->content.hdcptx->HdcpError >= AvHdcpTxErrorThreshold) ||
               (port->content.hdcptx->Hdcp2p2SinkSupport == 0))
                Gsv5k1ToggleTmdsOut(port, 0);
#else
            if(port->content.hdcptx->Hdcp2p2SinkSupport == 0)
                Gsv5k1ToggleTmdsOut(port, 0);
#endif
            break;
        case AV_HDCP_TX_1P4_ONLY:
            port->content.video->Mute.BlkMute = Error1;
            port->content.audio->AudioMute    = Error1;
#if Gsv5k1MuxMode
            if((port->content.hdcptx->HdcpError >= AvHdcpTxErrorThreshold) ||
               (port->content.hdcptx->HdcpSupport == 0))
                Gsv5k1ToggleTmdsOut(port, 0);
#else
            if(port->content.hdcptx->HdcpSupport == 0)
                Gsv5k1ToggleTmdsOut(port, 0);
#endif
            break;
        case AV_HDCP_TX_AUTO_FAIL_OUT:
            if((port->content.hdcptx->HdcpError == 2) &&
               ((Error1 != 0) || (Error4 != 0)))
            {
                Gsv5k1SetTxHdcpVersion(port, AV_HDCP_TX_1P4_FAIL_OUT, 1);
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
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxGetBksvReady(pin AvPort *port))
{
    AvRet ret = AvNotAvailable;
    uint8 value;
    GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV_READ_FLAG(port, &port->content.hdcptx->BksvReady);
    /* Running_rb only set 1 after BKSV flag is cleared which makes it an indicator */
    GSV5K1_TXPHY_get_TX_HDCP1P4_RUNNING_RB(port, &value);
    if(port->content.hdcptx->BksvReady == 1)
        ret = AvOk;
    else if(value == 1)
    {
        /* Bksv read but cleared */
        port->content.hdcptx->BksvReady = 1;
        ret = AvInvalidParameter;
    }
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxClearBksvReady(pin AvPort *port))
{
    AvRet ret = AvOk;
    GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_BKSV_FLAG(port, 1);
    port->content.hdcptx->BksvReady = 0;
    /* The frame should only be enabled after bksv is cleared */
    /* original start */
    /* GSV5K1_TXDIG_set_TX_HDCP1P4_FRAME_ENCRYPTION(port, 1); */
    /* original end */

#if AvEnableSimplifyHdcp
    GSV5K1_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 1);
#endif

#if AvEnableDebugHdcp
    AvUapiOutputDebugMessage("Clear Tx Bksv Ready");
#endif

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxClearRxidReady(pin AvPort *port))
{
    AvRet ret = AvOk;

    GSV5K1_TX2P2_set_TX_HDCP2P2_CLEAR_RXID_LIST_READY(port, 1);
    GSV5K1_TX2P2_set_TX_HDCP2P2_CLEAR_RXID_READY(port, 1);

#if AvEnableDebugHdcp
    AvUapiOutputDebugMessage("Clear Tx RxID Ready");
#endif

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxGetSinkHdcpCapability(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 RbState = 0;
    /* 1. Manual Force HDCP 1.4 Support */
    port->content.hdcptx->HdcpSupport = 1;
    /* 2. Check HDCP 2.2 Capability */
    RbState = Gsv5k1TxScdcAction(port, 0, 0x74, 0x50, 0x00);
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
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxClearHdcpError(pin AvPort *port))
{
    AvRet ret = AvOk;
    if(port->content.hdcptx->Hdcp2p2SinkSupport)
    {
        GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
        GSV5K1_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 1);
        GSV5K1_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 0);
        GSV5K1_TX2P2_set_HDCP2P2_MASTER_CLK_DIV(port, 0x14A); // lower to 30KHz DDC
        GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 0);
    }
    GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
    AvUapiOutputDebugMessage("Port%d: Clear Tx Hdcp Error%d", port->index, port->content.hdcptx->HdcpError);

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxReadBksv(pio AvPort *port, uint8 *Value, uint8 Count))
{
    AvRet ret = AvOk;
    uint8 ReadBksv[5];
    /* only 1 sink */
    if(Count == 0)
    {
        //TxMain_get_BKSV(port, ReadBksv);
        GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV0(port,&ReadBksv[0]);
        GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV1(port,&ReadBksv[1]);
        GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV2(port,&ReadBksv[2]);
        GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV3(port,&ReadBksv[3]);
        GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV4(port,&ReadBksv[4]);
        /* BKSV is inverted */
        Value[0] = ReadBksv[4];
        Value[1] = ReadBksv[3];
        Value[2] = ReadBksv[2];
        Value[3] = ReadBksv[1];
        Value[4] = ReadBksv[0];
    }
    else
    {
        AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 5*(Count-1), 5, Value);
    }
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxHdmiClearKsvFifo(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 i,j;
    uint8 ZeroBuf[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                          0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                         };
    uint8 RdBuf[10];
    uint8 StartReg = 0x80;

    AvMemset(RdBuf, 0, sizeof(RdBuf));

    for (i=0,j=0; (j<=127)&&(i<5); i++,j+=26)
    {
        GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, i);
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg, 64, ZeroBuf);
        AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+64, 10, RdBuf);
        if (AvMemcmp(RdBuf, ZeroBuf, 10)==0)
        {
            break;
        }
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+64, 64, ZeroBuf);
    }
    GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, 0);
    AvUapiOutputDebugMessage("Clear Rx KSV Fifo");

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxCheckBksvExisted(pin AvPort *port, uint8 *Bksv))
{
    AvRet ret = AvNotAvailable;
    uint16 BytePtrStart = 0;
    uint8 MapNumber = 0;
    uint16 Position = 0;
    uint8  Value[5];
    uint8 StartReg = 0x80;

    for(Position=0;Position<(port->content.hdcp->SinkTotal);Position++)
    {
        /* Step 1. Calculate the position and map number */
        BytePtrStart = Position*5;
        MapNumber = (uint8)(BytePtrStart>>7);

        /* Step 2. Read the value */
        GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber);
        if(Position == 25)
        {
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 3, Value);
            GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber+1);
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+0x00, 2, Value+3);
        }
        else if(Position == 51)
        {
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 1, Value);
            GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber+1);
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+0x00, 4, Value+1);
        }
        else if(Position == 76)
        {
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 4, Value);
            GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber+1);
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+0x00, 1, Value+4);
        }
        else
            AvHalI2cReadMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 5, Value);
        /* Step 3. Compare the value */
        if(AvMemcmp(Bksv, Value, 5) == 0)
            ret = AvOk;
        else
            ret = AvNotAvailable;
    }

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxAddBksv(pio AvPort *port, uint8 *Value, uint8 Position))
{
    AvRet ret = AvOk;
    uint16 BytePtrStart = Position*5;
    uint8 MapNumber = (uint8)(BytePtrStart>>7);
    uint8 StartReg = 0x80;

    AvUapiOutputDebugMessage("Add BKSV to Rx No.%d", Position);

    /* Special KSV Number will be considered following spec */
    GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber);
    if(Position == 25)
    {
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 3, Value);
        GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber+1);
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+0x00, 2, Value+3);
    }
    else if(Position == 51)
    {
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 1, Value);
        GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber+1);
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+0x00, 4, Value+1);
    }
    else if(Position == 76)
    {
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 4, Value);
        GSV5K1_RXRPT_set_HDCP_MAP_SEL(port, MapNumber+1);
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+0x00, 1, Value+4);
    }
    else
        AvHalI2cWriteMultiField(GSV5K1_RXRPT_MAP_ADDR(port), StartReg+(uint8)(BytePtrStart&0x007F), 5, Value);

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxGetBksvTotal(pio AvPort *port, uint8 *Value))
{
    AvRet ret = AvOk;
    GSV5K1_TXPHY_get_TX_HDCP1P4_BKSV_COUNT(port, Value);
    /* TxMain_set_BKSV_FLAG_INT(port, 1); */
    return ret;
}

/**
 * @brief  Read Packet Content
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetPacketContent(pio AvPort *port, PacketType Pkt, uint8 *Content))
{
    AvRet ret = AvOk;
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
#if 0
        case AV_PKT_EMP_PACKET:
#endif
            AvHalI2cRdMultiField(GSV5K1_RXINFO2_MAP_ADDR(port), RxPktID[Pkt], 2, (Content+1));
            AvHalI2cRdMultiField(GSV5K1_RXINFO2_MAP_ADDR(port), RxPktPtr[Pkt], PktSize[Pkt]-3, (Content+3));
            if(Pkt == AV_PKT_HDR_PACKET)
                Content[0] = 0x87;
#if 0
            else if(Pkt == AV_PKT_EMP_PACKET)
                Content[0] = 0x7F;
#endif
            break;
        case AV_PKT_AUDIO_CHANNEL_STATUS:
            GSV5K1_RXDIG_get_RB_RX_AUDIO_LAYOUT(port, &Content[5]);
            AvHalI2cRdMultiField(GSV5K1_RXAUD_MAP_ADDR(port), RxPktID[Pkt], 5, &Content[0]);
            GSV5K1_INT_get_RX1_CSDATA_VALID_INT_ST(port, &value);
            if (value != 0)
                GSV5K1_INT_set_RX1_CSDATA_VALID_CLEAR(port, 1);
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
            AvHalI2cRdMultiField(GSV5K1_RXINFO_MAP_ADDR(port), RxPktID[Pkt], 3, Content);
            AvHalI2cRdMultiField(GSV5K1_RXINFO_MAP_ADDR(port), RxPktPtr[Pkt], PktSize[Pkt]-3, (Content+3));
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
    return ret;
}

/**
 * @brief  Clear Rx Audio Related Interrupt
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxClearAudioInterrupt(pio AvPort *port, AudioInterrupt* Intpt))
{
    AvRet ret = AvOk;

    /* AudChanMode */
    if(Intpt->AudChanMode)
    {
        GSV5K1_INT_set_RX1_AUDIO_LAYOUT_CLEAR(port,1);
    }
    /* InternMute */
    if(Intpt->InternMute)
    {
        GSV5K1_INT_set_RX1_HDMI_AVMUTE_CLEAR(port,1);
    }
    /* CsDataValid */
    if(Intpt->CsDataValid)
    {
        GSV5K1_INT_set_RX1_CSDATA_VALID_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_CS_CLEAR(port,1);
    }
    /* NChange */
    if(Intpt->NChange)
    {
        GSV5K1_INT_set_RX1_PKT_N_CLEAR(port,1);
    }
    /* CtsThresh */
    if(Intpt->CtsThresh)
    {
        GSV5K1_INT_set_RX1_PKTDET_ACR_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_CTS_CLEAR(port,1);
    }
    /* AudFifoOv */
    if(Intpt->AudFifoOv)
    {
        GSV5K1_INT_set_RX1_PKT_OVFL_CLEAR(port,1);
    }
    /* AudFifoUn */
    if(Intpt->AudFifoUn)
    {
        GSV5K1_INT_set_RX1_PKT_UNDFL_CLEAR(port,1);
    }
    /* AudFifoNrOv */
    if(Intpt->AudFifoNrOv)
    {
        GSV5K1_INT_set_RX1_PKT_ALMOST_OVFL_CLEAR(port,1);
    }
    /* AudioPktErr */
    if(Intpt->AudioPktErr)
    {
        GSV5K1_INT_set_RX1_PKT_AUDPKTERR_CLEAR(port,1);
    }
    /* AudModeChg */
    if(Intpt->AudModeChg)
    {
        GSV5K1_INT_set_RX1_CHANGE_AUDIO_MODE_CLEAR(port,1);
    }
    /* AudFifoNrUn */
    if(Intpt->AudFifoNrUn)
    {
        GSV5K1_INT_set_RX1_PKT_ALMOST_UNDFL_CLEAR(port,1);
    }
    /* AudFlatLine */
    /*
    if(Intpt->AudFlatLine)
    {
        GSV5K1_INT_set_RX1_PKT_FLATLINE_CLEAR(port,1);
    }
    */
    /* AudSampChg */
    if(Intpt->AudSampChg)
    {
        GSV5K1_INT_set_RX1_PKT_NEWFREQ_CLEAR(port,1);
    }
    /* AudPrtyErr */
    if(Intpt->AudPrtyErr)
    {
        GSV5K1_INT_set_RX1_PKT_PARITY_CLEAR(port,1);
    }
    /* AudIfValid */
    if(Intpt->AudIfValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_AUD_IF_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_AUD_IF_CLEAR(port,1);
    }
    /* AcpValid */
    if(Intpt->AcpValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_ACP_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_ACP_CLEAR(port,1);
    }

    return ret;
}

/**
 * @brief  Clear Rx Video Related Interrupt
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxClearVideoInterrupt(pio AvPort *port, VideoInterrupt* Intpt))
{
    AvRet ret = AvOk;

    /* AvMute */
    if(Intpt->AvMute)
    {
        GSV5K1_INT_set_RX1_AVMUTE_CLEAR(port,1);
    }
    /* HdmiModeChg */
    if(Intpt->HdmiModeChg)
    {
        GSV5K1_INT_set_RX1_HDMI_MODE_CLEAR(port,1);
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
        GSV5K1_INT_set_RX1_VIDEO_3D_CLEAR(port,1);
    }
    /* NewTmds */
    if(Intpt->NewTmds)
    {
        GSV5K1_INT_set_RX1_PKT_NEWFREQ_CLEAR(port,1);
    }
    /* BadTmdsClk */
    if(Intpt->BadTmdsClk)
    {
        GSV5K1_INT_set_RX1_CLKCHANGEDETECTED_CLEAR(port,1);
    }
    /* DeepClrChg */
    if(Intpt->DeepClrChg)
    {
        GSV5K1_INT_set_RX1_DEEP_COLOR_CHANGE_CLEAR(port,1);
    }
    /* PktErr */
    if(Intpt->PktErr)
    {
        GSV5K1_INT_set_RX1_PKT_PKTERR_CLEAR(port,1);
    }
    /* AvIfValid */
    if(Intpt->AvIfValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_AVI_IF_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_AVI_IF_CLEAR(port,1);
    }
    /* SpdValid */
    if(Intpt->SpdValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_SPD_IF_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_SPD_IF_CLEAR(port,1);
    }
    /* HdrValid */
    if(Intpt->HdrValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_DRM_IF_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_DRM_IF_CLEAR(port,1);
    }
    /* EmpValid */
    if(Intpt->EmpValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_SPR_CLEAR(port, 1);
        GSV5K1_INT_set_RX1_PKT_NEW_SPR_PKT_CLEAR(port, 1);
    }
    /* MsValid */
    if(Intpt->MsValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_MPEG_IF_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_MPEG_IF_CLEAR(port,1);
    }
    /* VsValid */
    if(Intpt->VsValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_VS_IF_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_VS_IF_CLEAR(port,1);
    }
    /* Isrc1Valid */
    if(Intpt->Isrc1Valid)
    {
        GSV5K1_INT_set_RX1_PKTDET_ISRC1_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_ISRC1_CLEAR(port,1);
    }
    /* Isrc2Valid */
    if(Intpt->Isrc2Valid)
    {
        GSV5K1_INT_set_RX1_PKTDET_ISRC2_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_ISRC2_CLEAR(port,1);
    }
    /* GamutValid */
    if(Intpt->GamutValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_GAMUT_CLEAR(port,1);
        GSV5K1_INT_set_RX1_PKT_NEW_GAMUT_CLEAR(port,1);
    }
    /* GcValid */
    if(Intpt->GcValid)
    {
        GSV5K1_INT_set_RX1_PKTDET_GCP_CLEAR(port,1);
    }

    return ret;
}


/**
 * @brief  Clear Rx Hdcp Related Interrupt
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxClearHdcpInterrupt(pio AvPort *port, HdcpInterrupt* Intpt))
{
    AvRet ret = AvOk;

    if(Intpt->AksvUpdate)
    {
        Gsv5k1IntptClr(AKSV_UPDATE);
    }
    /* Intpt->Encrypted is cleared in AvUapiRxGetHdcpStatus() */

    return ret;
}

/**
 * @brief  Enable/Disable packets and info-frames
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxEnableInfoFrames(AvPort* port, uint16 InfoFrames, uint8 Enable))
{
    AvRet ret = AvOk;
#if Gsv5k1MuxMode
#else
    uint32 CurrentShadow = 0;
    uint8 PktEn = 0;
    uint8 PktShadow = 0;

    /* 1. Check Shadow and Enable State */
    if(Enable == 1)
        PktEn = 1;
    else if(Enable == 2)
        PktShadow = 1;

    /* 2. Get Current Shadow State */
    GSV5K1_TXPKT_get_TX_PKT_UPDATE(port, &CurrentShadow);

    /* 3. Set Packet Enable Regs */
    if (InfoFrames & AV_BIT_AV_INFO_FRAME)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0002;
        else
            CurrentShadow = CurrentShadow & 0xfffd;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_AVIIF_PKT_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_AUDIO_INFO_FRAME)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0008;
        else
            CurrentShadow = CurrentShadow & 0xfff7;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_AUDIO_INFO_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ACP_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0020;
        else
            CurrentShadow = CurrentShadow & 0xffdf;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_ACP_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_SPD_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0004;
        else
            CurrentShadow = CurrentShadow & 0xfffb;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_SPDIF_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_GMD_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0100;
        else
            CurrentShadow = CurrentShadow & 0xfeff;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_GM_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_MPEG_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0010;
        else
            CurrentShadow = CurrentShadow & 0xffef;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_MPEG_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_VS_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0001;
        else
            CurrentShadow = CurrentShadow & 0xfffe;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_VSIF_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ISRC1_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0040;
        else
            CurrentShadow = CurrentShadow & 0xffbf;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_ISRC_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ISRC2_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0080;
        else
            CurrentShadow = CurrentShadow & 0xff7f;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_ISRC_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_GC_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0200;
        else
            CurrentShadow = CurrentShadow & 0xfdff;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_GC_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_ACR_PACKET)
    {
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_N_CTS_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_AUDIO_SAMPLE_PACKET)
    {
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_AUDIO_SAMPLE_ENABLE(port, PktEn);
    }
    if (InfoFrames & AV_BIT_HDR_PACKET)
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0400;
        else
            CurrentShadow = CurrentShadow & 0xfbff;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_USER0_ENABLE(port, PktEn);
    }
#if 0
    if (InfoFrames & AV_BIT_EMP_PACKET)
#else
    if (InfoFrames & AV_BIT_SPARE2_PACKET)
#endif
    {
        if(PktShadow == 1)
            CurrentShadow = CurrentShadow | 0x0800;
        else
            CurrentShadow = CurrentShadow & 0xf7ff;
        if(PktShadow == 0)
            GSV5K1_TXDIG_set_TX_PKT_USER1_ENABLE(port, PktEn);
    }

    /* 4. Set Packet Shadow Regs */
    GSV5K1_TXPKT_set_TX_PKT_UPDATE(port, CurrentShadow);

#endif
    return ret;
}

/**
 * @brief  Configure Packet Content
 * @return AvOk: success
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetPacketContent(pio AvPort *port, PacketType Pkt, uint8 *Content, uint8 PktEn))
{
    AvRet ret = AvOk;
#if Gsv5k1MuxMode
#else
    uint16 InfoFrameBit = 0;
    if((port->index == 4) || (port->index == 5))
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
#if 0
            case AV_PKT_EMP_PACKET:
                InfoFrameBit = AV_BIT_EMP_PACKET;
                break;
#else
            case AV_PKT_SPARE2_PACKET:
                InfoFrameBit = AV_BIT_SPARE2_PACKET;
                break;
#endif
            case AV_PKT_GC_PACKET:
                InfoFrameBit = AV_BIT_GC_PACKET;
                break;
        }
        switch(Pkt)
        {
            case AV_PKT_AV_INFO_FRAME:
                Gsv5k1_TxSendAVInfoFrame(port, Content, PktEn);
                break;
            case AV_PKT_AUDIO_CHANNEL_STATUS:
                GSV5K1_TXDIG_set_TX_AUDIO_CS_OVERRIDE(port, 1);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_FORMAT(port,port->content.audio->Consumer);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_COPYRIGHT(port,        port->content.audio->Copyright);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_FORMAT_EXTRA(port,        port->content.audio->Emphasis);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_MODE(port,       port->content.audio->ClkAccur);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_CATEGORY_CODE(port, port->content.audio->CatCode);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_SOURCE_NUMBER(port, port->content.audio->SrcNum);
                GSV5K1_TXDIG_set_TX_AUDIO_CS_WORD_LENGTH(port,   port->content.audio->WordLen);
                GSV5K1_TXDIG_set_TX_AUDIO_LAYOUT_MAN_VALUE(port, port->content.audio->Layout);
                Gsv5k1_TxSetAudChStatSampFreq(port);
                /* Audio UnMute */
                GSV5K1_TXDIG_set_TX_AUDIO_MUTE(port, 0);
                break;
            case AV_PKT_HDR_PACKET:
                if(TrxAvUpStreamFlag == 0)
                {
                    /* In case of Rx HDR recovery */
                    Gsv5k1_TxSetCSC(port);
                }
#if 0
            case AV_PKT_EMP_PACKET:
#else
            case AV_PKT_SPARE2_PACKET:
#endif
                Gsv5k1AvUapiTxEnableInfoFrames(port, InfoFrameBit, 2);
                AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), TxPktPtr[Pkt], PktSize[Pkt], Content);
                Gsv5k1AvUapiTxEnableInfoFrames(port, InfoFrameBit, PktEn);
                break;
            case AV_PKT_VS_PACKET:
            case AV_PKT_SPD_PACKET:
            case AV_PKT_AUDIO_INFO_FRAME:
                /* Audio Infoframe WorkAround for 0x20~0x31 CA */
                if((Content[7]>0x1F) && (Pkt == AV_PKT_AUDIO_INFO_FRAME))
                {
                    AvHalI2cWrMultiField(GSV5K1_TXCEC_MAP_ADDR(port), TxPktPtr[AV_PKT_SPARE3_PACKET], PktSize[Pkt], Content);
                    GSV5K1_TXDIG_set_TX_PKT_USER2_ENABLE(port, 1);
                    Content[0] = 0x00;
                    Content[7] = 0x13;
                }
                else
                {
                    GSV5K1_TXDIG_set_TX_PKT_USER2_ENABLE(port, 0);
                }
            case AV_PKT_MPEG_PACKET:
            case AV_PKT_ACP_PACKET:
            case AV_PKT_ISRC1_PACKET:
            case AV_PKT_ISRC2_PACKET:
            case AV_PKT_GMD_PACKET:
                Gsv5k1AvUapiTxEnableInfoFrames(port, InfoFrameBit, 2);
                AvHalI2cWrMultiField(GSV5K1_TXPKT_MAP_ADDR(port), TxPktPtr[Pkt], PktSize[Pkt], Content);
                Gsv5k1AvUapiTxEnableInfoFrames(port, InfoFrameBit, PktEn);
                break;
            case AV_PKT_GC_PACKET:
                if(port->content.tx->HdmiMode == 0)
                    Gsv5k1AvUapiTxEnableInfoFrames(port, AV_BIT_GC_PACKET, 0);
                else
                    Gsv5k1AvUapiTxEnableInfoFrames(port, AV_BIT_GC_PACKET, PktEn);
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
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetHdmiAcrInfo (AvPort *port))
{
    uint8   value;

    if(port == FindCore1(port))
        GSV5K1_INT_get_RX1_PKTDET_ACR_RAW_ST(port, &value);
    else
        return AvNotAvailable;

    if(value == 1)
    {
        /* Reading dummy CTS is for protection of reading N */
        /* directly reading N will result in 0 */
        /* GSV5K1_RXDIG_get_RX_AUDIO_CTS(port, &port->content.audio->CTSValue); */
        GSV5K1_RXDIG_get_RB_RX_AUDIO_N(port, &port->content.audio->NValue);
        if (port->content.audio->NValue != 0)
        {
            return AvOk;
        }
    }
    return AvError;
}

/*********************** Local Functions **************************/

void Gsv5k1DisableRxHpa(pin AvPort *port)
{
    uint8 temp;
    uint8 PreviousValue;
    GSV5K1_PRIM_get_RX_RTERM_EN(port, &temp);
    PreviousValue = temp;

#if (AvAllowHpdLowEdidRead==0)
    switch(port->index)
    {
        case 0: /* rx port 0 */
            GSV5K1_RXRPT_set_RXA_EDID_EN(port, 0);
            break;
        case 1: /* rx port 1 */
            GSV5K1_RXRPT_set_RXB_EDID_EN(port, 0);
            break;
        default:
            return;
    }
#endif

    if(port->core.HdmiCore == 0)
    {
#if AvEnableHdcp2p2Feature
        if(port->content.rx->Input5V == 0)
        {
            GSV5K1_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(port, 1);
            GSV5K1_RX2P2_set_RX_HDCP2P2_FSM_DISABLE(port, 0);
        }
        else
        {
            GSV5K1_RXDIG_set_ECC_AVMUTE_MAN_EN(port, 1);
            GSV5K1_RXDIG_set_ECC_AVMUTE_MAN_EN(port, 0);
        }
#endif
        if(port->content.hdcp->HdcpNeeded == AV_HDCP_RX_NOT_SUPPORT)
            GSV5K1_PRIM_set_RX1_HDCP_EN(port, 0);
    }

    switch(port->index)
    {
        case 0: /* rx port 0 */
            temp = temp&0xFE;
            GSV5K1_APLL_set_RXA_CKIO_EN_Z0(port, 0);
            break;
        case 1: /* rx port 1 */
            temp = temp&0xFD;
            GSV5K1_APLL_set_RXB_CKIO_EN_Z0(port, 0);
            break;
        default:
            return;
    }
    if(temp != PreviousValue)
    {
        /* HDMI Interface Power Down */
#if AvAllowHpdLowEdidRead
        GSV5K1_PRIM_set_RX_HPD_MAN_VALUE(port, 0x0);         /* HPA=0 */
        GSV5K1_PRIM_set_RX_RTERM_EN(port, 0x0);              /* RTERM = 0 */
#else
        GSV5K1_PRIM_set_RX_HPD_MAN_VALUE(port, temp);        /* HPA=0 */
        GSV5K1_PRIM_set_RX_RTERM_EN(port, temp);             /* RTERM = 0 */
#endif
        GSV5K1_PRIM_set_RX_TMDSPLL_EN_OVERRIDE(port, 0x00);  /* CLK=0 */
        GSV5K1_PRIM_set_RX_CHANNEL_EN_OVERRIDE(port, 0x00);  /* CHANNEL=0 */
        AvUapiOutputDebugMessage("Port %d: Disable Rx Hpa %d...", port->index, temp);
    }

    return;
}

void Gsv5k1EnableRxHpa(pin AvPort *port)
{
    uint8 temp;
    uint8 PreiousValue;
    GSV5K1_PRIM_get_RX_RTERM_EN(port, &temp);
    PreiousValue = temp;

#if (AvAllowHpdLowEdidRead==0)
    switch(port->index)
    {
        case 0: /* rx port 0 */
            GSV5K1_RXRPT_set_RXA_EDID_EN(port, 1);
            break;
        case 1: /* rx port 1 */
            GSV5K1_RXRPT_set_RXB_EDID_EN(port, 1);
            break;
        default:
            return;
    }
#endif

    if(port->content.hdcp->HdcpNeeded != AV_HDCP_RX_NOT_SUPPORT)
    {
        if(port->core.HdmiCore == 0)
            GSV5K1_PRIM_set_RX1_HDCP_EN(port, 1);
    }

    switch(port->index)
    {
        case 0: /* rx port 0 */
            temp = temp|0x01;
            GSV5K1_APLL_set_RXA_CKIO_EN_Z0(port, 1);
            break;
        case 1: /* rx port 1 */
            temp = temp|0x02;
            GSV5K1_APLL_set_RXB_CKIO_EN_Z0(port, 1);
            break;
        default:
            return;
    }
    if(temp != PreiousValue)
    {
        /* HDMI Interface Power Up */
#if AvAllowHpdLowEdidRead
        GSV5K1_PRIM_set_RX_RTERM_EN(port, 0xf);              /* RTERM = 1 */
        GSV5K1_PRIM_set_RX_HPD_MAN_VALUE(port, 0xf);         /* HPA=1 */
#else
        GSV5K1_PRIM_set_RX_RTERM_EN(port, temp);             /* RTERM = 1 */
        GSV5K1_PRIM_set_RX_HPD_MAN_VALUE(port, temp);        /* HPA=1 */
#endif
        GSV5K1_PRIM_set_RX_TMDSPLL_EN_OVERRIDE(port, 0x03);  /* CLK=1 */
        GSV5K1_PRIM_set_RX_CHANNEL_EN_OVERRIDE(port, 0x03);  /* CHANNEL=1 */
        AvUapiOutputDebugMessage("Port %d: Enable Rx Hpa %d...", port->index, temp);
    }
}

void Gsv5k1GetRx5VStatus(pin AvPort *port)
{
    uint8 value = 0;
    switch(port->index)
    {
        case 0: /* rx port 0 */
            GSV5K1_INT_get_RXA_CABLE_DETECT_RAW_ST(port, &value);
            break;
        case 1: /* rx port 1 */
#if ((AvHdmiOnEthernet == 1)&&(Gsv5k1TxUseRxBHdmiMode==0))
            value = 1;
#else
            GSV5K1_INT_get_RXB_CABLE_DETECT_RAW_ST(port, &value);
#endif
            break;
        default:
            break;
    }
    port->content.rx->Input5V = value;

}

void Gsv5k1ToggleTmdsOut(AvPort *port, uint8 Enable)
{
    uint8 BulkWrite = 0x70;
    uint8 StermEnable = 1;

    if(Enable == 1)
    {
        if(port->type == HdmiTx)
        {
            BulkWrite = 0xff;
            GSV5K1_TXPHY_set_TX_EN_STERM_CLOCK(port, Enable);
        }
    }
    GSV5K1_TXPHY_set_TX_EN_STERM_LANE0(port, StermEnable);
    GSV5K1_TXPHY_set_TX_EN_STERM_LANE1(port, StermEnable);
    GSV5K1_TXPHY_set_TX_EN_STERM_LANE2(port, StermEnable);
    /* TMDS CLK, Lane0, Lane1, Lane2, Ser0, Ser1, Ser2 */
    //GSV5K1_TXPHY_set_TX_TMDS_CLK_DRIVER_ENABLE(port, Enable);
    AvHalI2cWriteField8(GSV5K1_TXPHY_MAP_ADDR(port), 0xF2, 0xFF, 0x0, BulkWrite);

    if(Enable == 1)
        AvUapiOutputDebugFsm("Port %d: Set TMDS to 1...", port->index);
        //AvUapiOutputDebugMessage("Port %d: Set TMDS to 1...", port->index);
    else
        AvUapiOutputDebugFsm("Port %d: Set TMDS to 0...", port->index);
        //AvUapiOutputDebugMessage("Port %d: Set TMDS to 0...", port->index);
}

void Gsv5k1SetTxHdcpVersion(pin AvPort *port, HdcpTxSupportType HdmiStyle, uint8 ForceUpdate)
{
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
        case 4:
            GSV5K1_PRIM_get_TXA_CFG_TO_HDCP2P2(port,  &PrevHdcpValue);
            GSV5K1_PRIM_get_TXA_HDCP_CFG_MAN_EN(port, &PrevHdcpMan);
            break;
        case 5:
            GSV5K1_PRIM_get_TXB_CFG_TO_HDCP2P2(port,  &PrevHdcpValue);
            GSV5K1_PRIM_get_TXB_HDCP_CFG_MAN_EN(port, &PrevHdcpMan);
            break;
    }
    if((PrevHdcpValue != HdcpValue) || (PrevHdcpMan != HdcpMan) || (ForceUpdate == 1))
    {
        GSV5K1_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 0);
        GSV5K1_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, 0);
        if(port->index == 4)
        {
            GSV5K1_PRIM_set_TXA_HDCP_EN(port, 0);
            GSV5K1_PRIM_set_TXA_CFG_TO_HDCP2P2(port,  HdcpValue);
            GSV5K1_PRIM_set_TXA_HDCP_CFG_MAN_EN(port, HdcpMan);
            GSV5K1_PRIM_set_TXA_HDCP_EN(port, 1);
        }
        else if(port->index == 5)
        {
            GSV5K1_PRIM_set_TXB_HDCP_EN(port, 0);
            GSV5K1_PRIM_set_TXB_CFG_TO_HDCP2P2(port,  HdcpValue);
            GSV5K1_PRIM_set_TXB_HDCP_CFG_MAN_EN(port, HdcpMan);
            GSV5K1_PRIM_set_TXB_HDCP_EN(port, 1);
        }
        Gsv5k1ToggleTxHpd(port);
    }
    if(HdmiStyle == AV_HDCP_TX_ILLEGAL_NO_HDCP)
        HdcpValue = 0;
    else
        HdcpValue = 1;
    GSV5K1_TXPHY_set_TX_HDCP1P4_ENC_EN(port, HdcpValue);
    GSV5K1_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, HdcpValue);
}

void Gsv5k1ToggleTxHpd(pin AvPort *port)
{
    if(port->content.tx->EdidReadSuccess == AV_EDID_UPDATED)
        GSV5K1_TXPHY_set_TX_NOT_READ_EDID_EN(port, 1);
    GSV5K1_TXPHY_set_TX_HPD_SRC_SEL(port, 3); /* Always Set HPD to 1 */
    GSV5K1_TXPHY_set_HPD_FILTER_EN(port, 0);
    GSV5K1_TXPHY_set_TX_HPD_MAN_VALUE(port, 0); /* Toggle HPD to 0 */
    AvUapiOutputDebugFsm("Port %d: Toggle Tx HPD...", port->index);
    GSV5K1_TXPHY_set_TX_HPD_MAN_VALUE(port, 1); /* Toggle HPD to 1 */
    GSV5K1_TXPHY_set_HPD_FILTER_EN(port, 1);
    GSV5K1_TXPHY_set_TX_HPD_SRC_SEL(port, 0); /* Normal HPD Setting */
    if(port->content.tx->EdidReadSuccess == AV_EDID_UPDATED)
    {
        GSV5K1_TXPHY_set_TX_EDID_TRY_TIMES(port, 0);
        GSV5K1_TXPHY_set_TX_NOT_READ_EDID_EN(port, 0);
    }
}

void Gsv5k1UpdateRxCdrBandWidth(pin AvPort *port)
{
    uint8 value;
    uint32 PhyPageAddr;
    /* Step 1. Read Ser Div */
    if(port->index == 0)
        GSV5K1_PLL_get_RB_RXA_PLL_SER_DIV(port, &value);
    else if(port->index == 1)
        GSV5K1_PLL_get_RB_RXB_PLL_SER_DIV(port, &value);
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
    PhyPageAddr = GSV5K1_RXLN0_MAP_ADDR(port);
    AvHalI2cWriteField8(PhyPageAddr,0x03,0xFF,0,value); //set cdr bandwith
}

uint8 Gsv5k1TxScdcAction(pin AvPort *port, uint8 WriteEnable, uint8 DevAddr, uint8 RegAddr, uint8 Value)
{
    uint8 round;
    uint8 PageAddr;
    uint8 DoneFlag;
    /* 1. Write Page Address and Rega Address, Value */
    if(WriteEnable)
        PageAddr = DevAddr & 0xFE;
    else
        PageAddr = DevAddr | 0x01;

    GSV5K1_TXPHY_set_RX_DDC_UDP_DEV_ADDR(port, PageAddr);
    GSV5K1_TXPHY_set_RX_DDC_UDP_SUB_ADDR(port, RegAddr);
    if(WriteEnable)
        GSV5K1_TXPHY_set_RX_DDC_UDP_WRITE_DATA(port, Value);
    GSV5K1_TXPHY_set_RX_DDC_UDP_ACCESS_REQ(port, 1);
    /* 3. Wait For Complete */
    for(round=0;round<0x18;round++)
    {
        GSV5K1_TXPHY_get_RX_DDC_UDP_DONE(port, &DoneFlag);
        if(DoneFlag == 1)
            break;
    }
    /* 4. Handle Return Value */
    if((WriteEnable == 1) || (DoneFlag == 0))
        round = 0xff;
    else
    {
        GSV5K1_TXPHY_get_RX_DDC_UDP_READ_DATA(port, &round);
    }
    return round;
}

uint8 Gsv5k1TxDDCError(pin AvPort *port)
{
    uint32 value32 = 0;
    uint8  value   = 0;
    GSV5K1_TXPHY_set_RD_CPU_PC_TRIGGER(port, 0);
    GSV5K1_TXPHY_set_RD_CPU_PC_TRIGGER(port, 1);
    GSV5K1_TXPHY_get_RB_CPU_PC_CAPTURED(port, &value32);
    GSV5K1_TXPHY_get_TX_EDID_DDC_ERROR_RB(port, &value);
    if(value == 1)
        GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
    if((value == 1) || (value32 == 0xf0))
        return 1;
    else
        return 0;
}

void Gsv5k1ManualBksvRead(pin AvPort *port)
{
#if AvEnableHdcp1p4BksvCheck
    uint8 i = 0;
    uint8 j = 0;
    uint8 OneNumber;
    uint8 RdBksv[5];

    /* 0. Disable HDCP 1.4 and 2.2 FSM */
    GSV5K1_TXPHY_set_TX_HDCP1P4_DESIRED(port, 0);
    GSV5K1_TX2P2_set_TX_HDCP2P2_DESIRED(port, 0);
    /* 1. Read Sink Bksv */
    for(i=0;i<5;i++)
        RdBksv[i] = Gsv5k1TxScdcAction(port, 0, 0x74, i, 0x00);
    AvUapiOutputDebugMessage("Port %d: BKSV = %x,%x,%x,%x,%x", port->index,
                             RdBksv[0], RdBksv[1], RdBksv[2], RdBksv[3], RdBksv[4]);
    /* 2. Check HDCP 2.2 Capability */
    j = Gsv5k1TxScdcAction(port, 0, 0x74, 0x50, 0x00);
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

void Gsv5k1_TxSetAudioInterface (AvPort* port)
{
    uint8 I2SBlockEnable = 0;
    uint8 SpdifBlockEnable = 0;
    uint8 DsdBlockEnable = 0;
    uint8 EncodeStyle = 0;
    uint8 NumCh = 8;   /* default 8 channels */
    uint8 NumPins = 3; /* default 4 pins */
    uint8 NumSlot = 2; /* default 2 slots */
    /* Default 128Fs MCLK */
    GSV5K1_TXDIG_set_TX_AUDIO_MCLK_FS_RATIO(port, port->content.audio->AudMclkRatio);

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
            GSV5K1_TXDIG_set_TX_AUDIO_I2S_FORMAT(port, 0);
            GSV5K1_TXDIG_set_TX_AUDIO_PAPB_SYNC(port, 0);
            GSV5K1_TXDIG_set_TX_AUDIO_PAPB_SYNC(port, 1);
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
    GSV5K1_TXDIG_set_TX_AUDIO_I2S_BLOCK_ENABLE(port, I2SBlockEnable);
    /* currently disabled, only useful when AP to Tx */
    GSV5K1_TXDIG_set_TX_AUDIO_SPDIF_ENABLE(port, SpdifBlockEnable);
    /* DSD and DST */
    GSV5K1_TXDIG_set_TDM_DESER_NUM_PINS(port, NumPins);
    GSV5K1_TXDIG_set_TDM_DESER_NUM_SLOT(port, NumSlot);
    GSV5K1_TXDIG_set_TDM_DESER_NUM_CH(port, NumCh);
    GSV5K1_TXDIG_set_DSD_EN(port, DsdBlockEnable);
    /* Encode Style */
    GSV5K1_TXDIG_set_TX_AUDIO_ENCODE_STYLE(port, EncodeStyle);

    return;
}

void Gsv5k1_TxSetAudChStatSampFreq (AvPort* port)
{
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
            GSV5K1_TXDIG_set_TX_AUDIO_I2S_MAN_SF(port, AudioSfTable[i+1]);
        }
        else
        {
            GSV5K1_TXDIG_set_TX_AUDIO_I2S_MAN_SF(port, 9);
        }
    }

}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetAudNValue(AvPort* port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    if((port->index == 4) || (port->index == 5))
    {
        //GSV5K1_TXDIG_set_TX_AUDIO_MAN_N(port, port->content.audio->NValue);
        value = (port->content.audio->NValue & 0x000ff) >> 0;
        AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port), 0x05, 0xFF, 0x0, value);
        value = (port->content.audio->NValue & 0x0ff00) >> 8;
        AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port), 0x06, 0xFF, 0x0, value);
        value = (port->content.audio->NValue & 0xff0000) >> 16;
        AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port), 0x07, 0x0F, 0x0, value);
        /* reset audio fifo */
        GSV5K1_TXPKT_get_AUD_IF_CA7_0(port, &value);
        GSV5K1_TXPKT_set_AUD_IF_CA7_0(port, value+1);
        GSV5K1_TXPKT_set_AUD_IF_CA7_0(port, value);
    }
    return ret;
}

AvRet Gsv5k1_TxSendAVInfoFrame (AvPort *port, uint8 *AviPkt, uint8 PktEn)
{
    AvRet ret = AvOk;
    AvVideoY InY = AV_Y2Y1Y0_RGB;
    AvVideoY OutY = AV_Y2Y1Y0_RGB;
    uint8 Value;
    uint8 Packet[31];
    AvPort *FromPort = NULL;
    AvPort *DscPort = (AvPort*)(Gsv5k1FindDscFront(port));

    if((port->index == 4) || (port->index == 5))
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
#if AvEnableInternalVideoGen
               FromPort = Gsv5k1FindHdmiRxFront(port);
               if(FromPort->type == VideoGen)
               {
                   InY = AV_Y2Y1Y0_RGB;
                   port->content.video->InCs = AV_CS_RGB;
               }
               else
#endif
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
            if(TrxAvUpStreamFlag == 0)
            {
            #if AvEnableColorMatchInput
                if((DscPort != NULL) && /* Front Dsc Decoder Port Exists */
                   (DscPort->type == VideoDsc) && /* It is Dsc Port */
                   (DscPort->content.dsc->DscInCompressed == 1)) /* DSC input is compressed */
                {
                    if(DscPort->content.dsc->SrcInY != AV_Y2Y1Y0_YCBCR_420)
                    {
                        OutY = DscPort->content.dsc->SrcInY;
                        port->content.video->Y = DscPort->content.dsc->SrcInY;
                        port->content.video->OutCs = DscPort->content.dsc->SrcInCs;
                    }
                }
                FromPort = (AvPort*)(port->content.RouteVideoFromPort);
                if(FromPort->type == VideoColor)
                {
                    if((FromPort->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420) &&
                       (FromPort->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_444))
                    {
                        DscPort = (AvPort*)(Gsv5k1FindDscPort(port));
                        if(DscPort != NULL)
                        {
                            OutY = DscPort->content.dsc->SrcInY;
                            port->content.video->Y = DscPort->content.dsc->SrcInY;
                            port->content.video->OutCs = DscPort->content.dsc->SrcInCs;
                        }
                    }
                    else if((FromPort->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420) &&
                            (FromPort->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420))
                    {
                        port->content.video->Y = FromPort->content.color->ColorOutSpace;
                        port->content.video->OutCs = port->content.video->InCs;
                        OutY = port->content.video->Y;
                    }
                }
            #else
                DscPort = (AvPort*)(Gsv5k1FindDscPort(port));
                if((DscPort != NULL) && /* Front Dsc Decoder Port Exists */
                   (DscPort->type == VideoDsc) && /* It is Dsc Port */
                   (DscPort->content.dsc->DscInVic != 0) && /* Compression enabled */
                   (DscPort->content.dsc->SrcInY == AV_Y2Y1Y0_RGB) && /* source RGB input */
                   ((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) == 0)) /* Hdmi Output Port */
                {
                    if(port->content.video->Y != AV_Y2Y1Y0_YCBCR_420)
                    {
                        OutY = DscPort->content.dsc->SrcInY;
                        port->content.video->Y = DscPort->content.dsc->SrcInY;
                        port->content.video->OutCs = DscPort->content.dsc->SrcInCs;
                    }
                }
            #endif
            }

            if(TrxAvUpStreamFlag == 1)
            {
            #if (AvHdmiOnEthernet == 1)
                if(port->index == 5)
                {
                    FromPort = (AvPort*)(port->content.RouteVideoFromPort);
                    if(FromPort->type != HdmiRx)
                    {
                        if(port->content.video->Y == AV_Y2Y1Y0_YCBCR_422)
                        {
                            InY  = AV_Y2Y1Y0_YCBCR_444;
                            OutY = AV_Y2Y1Y0_YCBCR_444;
                        }
                    }
                    /* Source HDCP Status Update */
                    FromPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));
                    if(FromPort != NULL)
                    {
                        if(FromPort->content.rx->VideoEncrypted == 1)
                        {
                            if(FromPort->content.hdcp->Hdcp2p2RxRunning == 1)
                                SET_UPSTREAM_HDCP(Packet,AV_HDCP_2P2_ENCRYPTED);
                            else
                                SET_UPSTREAM_HDCP(Packet,AV_HDCP_1P4_ENCRYPTED);
                        }
                        else
                            SET_UPSTREAM_HDCP(Packet,AV_HDCP_NOT_ENCRYPTED);
                    }
                }
            #endif
            }
            if(TrxAvUpStreamFlag == 0)
            {
            #if (AvEthCascading == 1)
                if(port->index == 5)
                {
                    FromPort = (AvPort*)(port->content.RouteVideoFromPort);
                    if(FromPort->type != HdmiRx)
                    {
                        if(port->content.video->Y == AV_Y2Y1Y0_YCBCR_422)
                        {
                            InY  = AV_Y2Y1Y0_YCBCR_444;
                            OutY = AV_Y2Y1Y0_YCBCR_444;
                        }
                    }
                    /* Source HDCP Status Update */
                    FromPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));
                    if(FromPort != NULL)
                    {
                        if(FromPort->content.rx->VideoEncrypted == 1)
                        {
                            if(FromPort->content.hdcp->Hdcp2p2RxRunning == 1)
                                SET_UPSTREAM_HDCP(Packet,AV_HDCP_2P2_ENCRYPTED);
                            else
                                SET_UPSTREAM_HDCP(Packet,AV_HDCP_1P4_ENCRYPTED);
                        }
                        else
                            SET_UPSTREAM_HDCP(Packet,AV_HDCP_NOT_ENCRYPTED);
                    }
                }
            #endif
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
            GSV5K1_TXDIG_set_TX_VIDEO_INPUT_CS(port, 0);
        else
            GSV5K1_TXDIG_set_TX_VIDEO_INPUT_CS(port, 1);
        /* Step 2.2 Don't Enable CSC in Dsc Encode Mode */
        if((GET_AVIF_LEN(AviPkt) == 0x1B) && (GET_DSC_EN(AviPkt) != 0))
            port->content.video->OutCs = AV_CS_AUTO;
        /* Step 2.3 CSC Setting */
        Gsv5k1_TxSetCSC(port);
        /* Step 2.4 Scan Info Processing */
        FromPort = (AvPort*)(port->content.RouteVideoFromPort);
        if(FromPort->type == VideoScaler)
        {
            Value = GET_AVIF_SC(Packet);
            /* Remove Overscan after downscaling */
            if(Value == 0x01)
                SET_AVIF_SC(Packet,0);
        }
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
                GSV5K1_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, 0);
                break;
            case 2:
                SET_AVIF_M(Packet, 2);
                GSV5K1_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, 1);
                break;
            default:
                SET_AVIF_M(Packet, 0);
                GSV5K1_TXDIG_set_TX_VIDEO_ASPECT_RATIO(port, 0);
                break;
        }
        /* Step 4. Pixel Repeat */
        Gsv5k1_TxSetManualPixelRepeat(port);
        /* Step 5. Set Y */
        FromPort = (AvPort*)(port->content.RouteVideoFromPort);
        if((FromPort->type == HdmiRx) &&
           (FromPort->content.video->Y == AV_Y2Y1Y0_YCBCR_422))
            Value = 1;
        else
            Value = 0;
        GSV5K1_TXDIG_set_TX_VIDEO_INPUT_FORMAT(port, Value);
        switch (OutY)
        {
            case AV_Y2Y1Y0_RGB:
                SET_AVIF_Y(Packet, 0);
                //SET_AVIF_C(Packet, 0);
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
        if(OutY != AV_Y2Y1Y0_RGB)
        {
            SET_AVIF_C(Packet,  (port->content.video->OutCs & 0x03));
            SET_AVIF_EC(Packet, ((port->content.video->OutCs >> 2) & 0x07));
        }
        GSV5K1_TXDIG_set_TX_VIDEO_OUTPUT_FORMAT(port, Value);
        /* Step 6. Set AVI Infoframe Content  */
        /* Step 6.1 Protect Internal VESA Timing Vic */
        if((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) == 0)
        {
            Value = GET_AVIF_VIC(Packet);
            if(Value > 127)
                SET_AVIF_VIC(Packet,0);
        }
        GSV5K1_TXPKT_set_AVI_IF_TYPE(port, 0x82);
        GSV5K1_TXPKT_set_AVI_IF_VER(port, Packet[1]);
        GSV5K1_TXPKT_set_AVI_IF_LEN(port, Packet[2]);
        Gsv5k1AvUapiTxEnableInfoFrames(port, AV_BIT_AV_INFO_FRAME, 2); /* enable AVI infoframe */
        AvHalI2cWrMultiField(GSV5K1_TXPKT_MAP_ADDR(port),
                             TxPktPtr[AV_PKT_AV_INFO_FRAME],
                             PktSize[AV_PKT_AV_INFO_FRAME]-3,
                             Packet+3);
        Gsv5k1AvUapiTxEnableInfoFrames(port, AV_BIT_AV_INFO_FRAME, PktEn); /* enable AVI infoframe */
        if(PktEn)
        {
            AvUapiOutputDebugFsm("Port%d: Y1Y0 = %d",port->index, port->content.video->Y);
            AvUapiOutputDebugFsm("Port%d: Tx Vic = %x", port->index, Packet[7]);
        }
    }

    return ret;
}

AvRet Gsv5k1_TxSetManualPixelRepeat(AvPort *port)
{
    AvRet ret = AvInvalidParameter;
    uint8 Factor = port->content.video->ClockMultiplyFactor;
    uint8 PrValue = port->content.video->PixelRepeatValue;
    /* 4x clock PrValue mapping */
    if(PrValue >= 3)
        PrValue = 2;
    if ((Factor > 4) || (PrValue > 3))
    {
        return ret;
    }
    /* no clock pixel repetition */
    else
    {
        PrValue = 0;
    }
    GSV5K1_TXDIG_set_TX_VIDEO_TMDS_CLK_RATIO_MAN(port, PrValue);
    GSV5K1_TXDIG_set_TX_VIDEO_PR_TO_RX_MAN(port, PrValue);
    GSV5K1_TXDIG_set_TX_VIDEO_PR_MODE(port, 2);

    ret = AvOk;
    return ret;
}

AvRet Gsv5k1_TxSetCSC (AvPort *port)
{
    AvRet ret = AvOk;
    uint8 i;
    uint8 HdrRecoveryFlag = 0;
    uint8 RgbRecoveryFlag = 0;
    uint8 CscMatchFlag = 0;
    AvPort *DscPort = (AvPort*)(Gsv5k1FindDscFront(port));
    AvPort *FromPort = (AvPort*)(port->content.RouteVideoFromPort);
    AvVideoCs InCs  = Gsv5k1ColorCsMapping(port->content.video->InCs);
    AvVideoCs OutCs = Gsv5k1ColorCsMapping(port->content.video->OutCs);

if(TrxAvUpStreamFlag == 0)
{
    /* HDR deep color recovery */
    /* Match Case 1: HDR output */
    if(((port->content.video->AvailableVideoPackets & AV_BIT_HDR_PACKET) != 0) &&
    /* Match Case 2: HDMI output port, not compressed output port */
       ((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) == 0) &&
    /* Match Case 3: YCbCr-422 or deep color */
       ((port->content.video->Y != AV_Y2Y1Y0_RGB) &&
        ((port->content.video->Y  == AV_Y2Y1Y0_YCBCR_422) ||
         (port->content.video->Cd == AV_CD_30) ||
         (port->content.video->Cd == AV_CD_36))) &&
    /* Match Case 4: Dsc/Scaler/Color for 8-bit processing */
       ((FromPort->type == VideoDsc) ||
        (FromPort->type == VideoScaler) ||
        (FromPort->type == VideoColor)))
    {
        HdrRecoveryFlag = 1;
    }
    /* RGB recovery */
    /* Match Case 1: HDMI output port, not compressed output port */
    if(((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) == 0) &&
    /* Match Case 2: Dsc/Scaler/Color for 8-bit processing */
       ((FromPort->type == VideoDsc) ||
        (FromPort->type == VideoScaler) ||
        (FromPort->type == VideoColor)) &&
    /* Match Case 3: YCbCr->RGB conversion */
       (((InCs != AV_CS_RGB) && (InCs != AV_CS_LIM_RGB)) &&
        ((OutCs == AV_CS_RGB) || (OutCs == AV_CS_LIM_RGB))))
    {
        RgbRecoveryFlag = 1;
    }
}

    /* 1. Use Tx CSC to do Pin Swapping is needed */
    if((DscPort != NULL) && /* Front Dsc Decoder Port Exists */
       (DscPort->type == VideoDsc) && /* It is Dsc Port */
       (DscPort->content.dsc->DscInCompressed == 1) && /* DSC input is compressed */
       ((port->content.RouteVideoFromPort == (struct AvPort*)(DscPort)) || /* Dsc directly to Tx */
        (FromPort->type == VideoScaler))) /* Dsc->Scaler->Tx */
    {
        if((OutCs == AV_CS_RGB) || (OutCs == AV_CS_LIM_RGB))
        {
            /* 1.1 Pin Swapping and CSC are done at the same time */
            for (i=0; CscTables[i].ConvTable; i++)
            {
                if ((CscTables[i].InCs  == AV_CS_YCC_709) &&
                    (CscTables[i].OutCs == OutCs))
                {
                    AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 24, (uint8 *)(CscTables[i].ConvTable));
                    AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x2B,0xFF,0,(CscTables[i].ConvTable[0] & 0xE0));
                    if(RgbRecoveryFlag == 1)
                    {
                        AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x33,0xFF,0,(CscTables[i].ConvTable[7] + 16));
                        AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x43,0xFF,0,(CscTables[i].ConvTable[23] + 2));
                    }
                    break;
                }
            }
        }
        else if(DscPort->content.dsc->SrcInY != AV_Y2Y1Y0_RGB)
        {
            /* 1.2 Default Pin Swapping */
            AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 24, (uint8 *)(CscTxYCbCrPinSwapping));
            if(HdrRecoveryFlag == 1)
            {
                AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x32, 2, (uint8 *)(&CscTxHdrRecovery[22]));
                AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x42, 2, (uint8 *)(&CscTxHdrRecovery[22]));
            }
            AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x2B,0xFF,0,(CscTxYCbCrPinSwapping[0] & 0xE0));
        }
        else
        {
            /* 1.3 Disable CSC */
            GSV5K1_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, 0);
        }
    }
    /* 2. Normal Case of disabling CSC */
    else if ((InCs == OutCs) || (InCs == AV_CS_AUTO) || (OutCs == AV_CS_AUTO) ||
             (port->content.video->Y == AV_Y2Y1Y0_YCBCR_420))
    {
        if(HdrRecoveryFlag == 1)
        {
            AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 24, (uint8 *)(CscTxHdrRecovery));
            AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x2B,0xFF,0,(CscTxHdrRecovery[0] & 0xE0));
        }
        else
        {
            GSV5K1_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, 0);
        }
    }
    /* 3. Normal Case of enabling CSC */
    else
    {
        GSV5K1_TXDIG_set_TX_VIDEO_CSC_ENABLE(port, 0);
        for (i=0; CscTables[i].ConvTable; i++)
        {
            if ((CscTables[i].InCs  == InCs) &&
                (CscTables[i].OutCs == OutCs))
            {
                AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 24, (uint8 *)(CscTables[i].ConvTable));
                AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x2B,0xFF,0,(CscTables[i].ConvTable[0] & 0xE0));
                if(RgbRecoveryFlag == 1)
                {
                    AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x33,0xFF,0,(CscTables[i].ConvTable[7] + 16));
                    AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x43,0xFF,0,(CscTables[i].ConvTable[23] + 2));
                }
                CscMatchFlag = 1;
                break;
            }
        }
        if((CscMatchFlag == 0) && (HdrRecoveryFlag == 1))
        {
            AvHalI2cWrMultiField(GSV5K1_TXDIG_MAP_ADDR(port), 0x2C, 24, (uint8 *)(CscTxHdrRecovery));
            AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(port),0x2B,0xFF,0,(CscTxHdrRecovery[0] & 0xE0));
        }
    }
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetAvMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    GSV5K1_RXDIG_get_RB_RX_AVMUTE_STATE(port, &(port->content.video->Mute.AvMute));
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetAvMute(pio AvPort *port))
{
    AvRet ret = AvOk;
#if Gsv5k1MuxMode
    GSV5K1_TXDIG_set_TX_MUX_MODE_AVMUTE_MAN(port, 0);
    GSV5K1_TXDIG_set_TX_MUX_MODE_AVMUTE_MAN_EN(port, 0);
#else
    if((port->index == 4) || (port->index == 5))
    {
        if(port->content.video->Mute.AvMute == 1)
        {
            GSV5K1_TXDIG_set_TX_GC_CLEAR_AVMUTE(port, 0);
            GSV5K1_TXDIG_set_TX_GC_SET_AVMUTE(port, 1);
            AvUapiOutputDebugMessage("Port %d: AVMUTE Enable...", port->index);
        }
        else
        {
            GSV5K1_TXDIG_set_TX_GC_SET_AVMUTE(port, 0);
            GSV5K1_TXDIG_set_TX_GC_CLEAR_AVMUTE(port, 1);
            AvUapiOutputDebugMessage("Port %d: AVMUTE Disable...", port->index);
        }
    }
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetHdmiModeSupport(pio AvPort *port))
{
    AvRet ret = AvOk;
    GSV5K1_RXDIG_get_RB_RX_HDMI_MODE(port, &(port->content.rx->HdmiMode));
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetHdmiModeSupport(pio AvPort *port))
{
    AvRet ret = AvOk;
    if((port->content.tx->HdmiMode == 1) ||
       (port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) != 0)
    {
        GSV5K1_TXDIG_set_TX_HDMI_MODE_OVERRIDE(port, 1);
        GSV5K1_TXDIG_set_TX_HDMI_MODE_MAN(port, 1);
    }
    else
    {
        GSV5K1_TXDIG_set_TX_HDMI_MODE_OVERRIDE(port, 1);
        GSV5K1_TXDIG_set_TX_HDMI_MODE_MAN(port, 0);
    }
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetFeatureSupport(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    if(port->content.tx->EdidSupportFeature & AV_BIT_FEAT_SCDC)
        value = 1;
    else
        value = 0;
    /* Only Enable Tx SCDC, no RR mode */
    GSV5K1_TXPHY_set_TX_SCDC_EN(port, 0);
    GSV5K1_TXPHY_set_RX_SCDC_PRESENT(port, value);
    /* Disable Tx RR, only use polling to check Scdc Flag */
    GSV5K1_TXPHY_set_TX_RR_CAPABLE(port, 1);
    GSV5K1_TXPHY_set_RX_RR_CAPABLE(port, 0);
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetVideoLock(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 tmds_pll_lock_flag;
    uint8 de_regen_lock_flag;
    uint8 v_sync_lock_flag;
    uint8 value;
    uint8 pb_data[3];
    /* Step 1. Check the lock status */
    GSV5K1_RXDIG_get_RB_RX_TMDS_PLL_LOCKED (port, &(port->content.rx->Lock.PllLock));
    GSV5K1_RXDIG_get_RB_RX_HS_LOCKED(port, &(port->content.rx->Lock.DeRegenLock));
    GSV5K1_RXDIG_get_RB_RX_VS_LOCKED(port, &(port->content.rx->Lock.VSyncLock));
    if(port->content.rx->Lock.PllLock == 0)
        Gsv5k1RpllProtect(port);
    /* Step 2. Check whether tmds_pll_lock value has changed */
    switch(port->index)
    {
        case 0:
            GSV5K1_INT_get_RXA_TMDSPLL_LOCK_DET_INT_ST(port, &tmds_pll_lock_flag);
            if(tmds_pll_lock_flag)
            {
                GSV5K1_INT_set_RXA_TMDSPLL_LOCK_DET_CLEAR(port, 1);
            }
            break;
        case 1:
            GSV5K1_INT_get_RXB_TMDSPLL_LOCK_DET_INT_ST(port, &tmds_pll_lock_flag);
            if(tmds_pll_lock_flag)
            {
                GSV5K1_INT_set_RXB_TMDSPLL_LOCK_DET_CLEAR(port, 1);
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
    if(FindCore1(port) == port)
    {
        GSV5K1_INT_get_RX1_HS_LOCKED_INT_ST(port, &de_regen_lock_flag);
        GSV5K1_INT_get_RX1_VS_LOCKED_INT_ST(port, &v_sync_lock_flag);
        /* 3.1 VRR mode detection protection */
        if(((port->content.rx->Lock.VSyncLock == 0) && (port->content.rx->Lock.DeRegenLock == 1)) ||
           ((v_sync_lock_flag == 1) && (de_regen_lock_flag == 0)))
        {
            GSV5K1_RXAUD_get_SPR_PKT_DET(port, &value);
            if(value == 1)
            {
                v_sync_lock_flag = 0;
                port->content.rx->Lock.VSyncLock = 1;
            }
            else
            {
                GSV5K1_RXAUD_get_SPD_IF_PKT_DET(port, &value);
                if(value == 1)
                {
                    AvHalI2cReadMultiField(GSV5K1_RXINFO_MAP_ADDR(port), 0x2b, 3, pb_data);
                    if((pb_data[0] == 0x1A) && (pb_data[1] == 0x00) && (pb_data[2] == 0x00))
                    {
                        v_sync_lock_flag = 0;
                        port->content.rx->Lock.VSyncLock = 1;
                    }
                }
            }
        }
        if((de_regen_lock_flag == 1) || (v_sync_lock_flag == 1))
        {
            port->content.rx->Lock.DeRegenLock = 0;
            port->content.rx->Lock.VSyncLock = 0;
            GSV5K1_INT_set_RX1_HS_LOCKED_CLEAR(port, 1);
            GSV5K1_INT_set_RX1_VS_LOCKED_CLEAR(port, 1);
        }
    }
#if Gsv5k1MuxMode
    if((port->content.video->info.TmdsFreq < 100) && (port->content.video->info.TmdsFreq > 90))
    {
        port->content.rx->Lock.DeRegenLock = port->content.rx->Lock.PllLock;
        port->content.rx->Lock.VSyncLock   = port->content.rx->Lock.PllLock;
    }
#endif
    /* Step 4. Manual Reset EQ when PLL Lock Status is lost */
    if(tmds_pll_lock_flag)
    {
        AvUapiOutputDebugMessage("Port %d: TMDS PLL Unlock ST triggers EQ ReRun", port->index);
        /* EQ reset to default state */
        Gsv5k1RxManualEQUpdate(port, 1);
    }
    /* Step 5. Update IsInputStable */
    if((port->content.rx->Lock.DeRegenLock == 0) ||
       (port->content.rx->Lock.VSyncLock   == 0) ||
       (port->content.rx->Lock.PllLock     == 0))
        port->content.rx->IsInputStable = 0;

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetVideoTiming(pio AvPort *port))
{
    AvPort *FromPort;
    if(KfunFindVideoRxFront(port, &FromPort) == AvOk)
    {
        /* no need to set if Rx is not stable */
        if(FromPort->content.rx->IsInputStable == 0)
            return AvOk;
    }

    GSV5K1_TXDIG_set_MAN_POL_HS(port, 1 - port->content.video->timing.HPolarity);
    GSV5K1_TXDIG_set_MAN_POL_VS(port, 1 - port->content.video->timing.VPolarity);

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetVideoTiming(pio AvPort *port))
{
    AvRet ret = AvOk;

    GSV5K1_RXDIG_get_RB_RX_INTERLACED(port, &port->content.video->timing.Interlaced);
#if AvEnableDetailTiming
    uint32 TempValue = 0;
    GSV5K1_RXDIG_get_RB_RX_H_SYNC_WIDTH(port, &TempValue);
    port->content.video->timing.HSync = TempValue;
    GSV5K1_RXDIG_get_RB_RX_H_BACK_PORCH(port, &TempValue);
    port->content.video->timing.HBack = TempValue;
    //GSV5K1_RXDIG_get_RB_RX_H_FRONT_PORCH(port, &TempValue);
    //port->content.video->timing.HFront = TempValue;
    GSV5K1_RXDIG_get_RB_RX_V_SYNC_WIDTH_0(port, &TempValue);
    port->content.video->timing.VSync = TempValue>>1;
    GSV5K1_RXDIG_get_RB_RX_V_BACK_PORCH_0(port, &TempValue);
    if(TempValue%2 == 1)
        TempValue = TempValue + 1;
    port->content.video->timing.VBack = TempValue>>1;
    //GSV5K1_RXDIG_get_RB_RX_V_FRONT_PORCH_0(port, &TempValue);
    //port->content.video->timing.VFront = TempValue>>1;
    GSV5K1_RXDIG_get_RB_RX_H_TOTAL_WIDTH(port,&TempValue);
    port->content.video->timing.HTotal  = TempValue;
    GSV5K1_RXDIG_get_RB_RX_V_ACTIVE_HEIGHT_0(port,&TempValue);
    if(port->content.video->timing.Interlaced)
        port->content.video->timing.VActive = TempValue<<1;
    else
        port->content.video->timing.VActive = TempValue;
    GSV5K1_RXDIG_get_RB_RX_V_TOTAL_0 (port,&TempValue);
    port->content.video->timing.VTotal  = TempValue>>1;
    GSV5K1_RXDIG_get_RB_RX_H_ACTIVE_WIDTH(port,&TempValue);
    port->content.video->timing.HActive = TempValue;
    TempValue = ((port->content.video->info.TmdsFreq)*1000000)/
                 ((port->content.video->timing.HTotal) *
                  (port->content.video->timing.VTotal));
    port->content.video->timing.FrameRate = TempValue+1;
#endif //AvEnableDetailTiming

    GSV5K1_RXDIG_get_RB_RX_HS_POL(port, &port->content.video->timing.HPolarity);
    GSV5K1_RXDIG_get_RB_RX_VS_POL(port, &port->content.video->timing.VPolarity);
    /* 3D */
    GSV5K1_RXAUD_get_RB_RX_VIDEO_3D(port, &port->content.video->info.Detect3D);

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetHdmiDeepColor(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 Value;
    GSV5K1_RXDIG_get_RB_RX_DEEP_COLOR_MODE(port, &Value);
    if(Value == 0)
        port->content.video->Cd = AV_CD_24;
    else if(Value == 1)
        port->content.video->Cd = AV_CD_30;
    else if(Value == 2)
        port->content.video->Cd = AV_CD_36;
    else
        port->content.video->Cd = AV_CD_NOT_INDICATED;
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetHdmiDeepColor(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    AvPort *FromPort;
    AvPort *DscPort;
    FromPort = (AvPort*)port->content.RouteVideoFromPort;
    DscPort = (AvPort*)(Gsv5k1FindDscFront(port));

    if(FromPort != NULL)
    {
        if(DscPort != NULL)
        {
            if(DscPort->content.dsc->DscInCompressed == 1)
            {
#if (AvEnableColorMatchInput == 0)
                if((FromPort->type == VideoScaler) || (FromPort->type == VideoColor))
                    value = 4; /* Rx->VP(Clock Up)->CP(Clock Down)->Tx, 24-bit only */
                else if(port->content.video->info.TmdsFreq > 450)
                    value = 4;
                else
                {
                    if(port->content.video->Cd == AV_CD_30)
                        value = 5;
                    else if(port->content.video->Cd == AV_CD_36)
                        value = 6;
                    else
                        value = 4;
                }
#else
                if(DscPort->content.dsc->SrcInCd == AV_CD_30)
                    value = 5;
                else if(DscPort->content.dsc->SrcInCd == AV_CD_36)
                    value = 6;
                else
                    value = 4;
#endif
            }
            else
                value = 4;
        }
        else if(FromPort->type == VideoScaler)
            value = 4; /* Transmit 24-bit */
        else if(FromPort->type == VideoColor)
        {
            value = 4; /* Set to 8-bit to make it /4 clock convinient */
            if(TrxAvUpStreamFlag == 0)
            {
            #if (AvEnableColorMatchInput == 1)
                if((FromPort->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420) &&
                   (FromPort->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_444))
                {
                    DscPort = (AvPort*)(Gsv5k1FindDscPort(port));
                    if(DscPort != NULL)
                    {
                        if(DscPort->content.dsc->SrcInCd == AV_CD_30)
                            value = 5;
                        else if(DscPort->content.dsc->SrcInCd == AV_CD_36)
                            value = 6;
                    }
                }
            #endif
            }
        }
        else
        {
    #if Gsv5k1MuxMode
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
    /* Ethernet Port Frequency Protection */
    if(TrxAvUpStreamFlag == 1)
    {
    #if (AvHdmiOnEthernet == 1)
        if((port->content.video->info.TmdsFreq > AvCompressionLowThreshold) && (port->index == 5))
            value = 4;
        if((FromPort->type == HdmiRx) && (port->content.video->info.TmdsFreq < 32) && (value == 4) &&
           ((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) != 0))
            value = 7;
    #endif
    }
    if(TrxAvUpStreamFlag == 0)
    {
    #if (AvEthCascading == 1)
        if((port->content.video->info.TmdsFreq > AvCompressionLowThreshold) && (port->index == 5))
            value = 4;
        if((FromPort->type == HdmiRx) && (port->content.video->info.TmdsFreq < 32) && (value == 4) &&
           ((port->content.tx->EdidSupportFeature & AV_BIT_FEAT_DSC_ENCODE) != 0))
            value = 7;
    #endif
    }
    GSV5K1_TXDIG_set_TX_GC_CD(port, value);
    //AvUapiOutputDebugMessage("Port%d: Actual GC CD = %d", port->index, value);

    if(TrxAvUpStreamFlag == 0)
    {
        /* Manual Deep Color Setting for Tx PLL */
    #if AvEnableColorMatchInput
        if((DscPort != NULL) &&
           (DscPort->content.dsc->DscInCompressed == 1) &&
           (FromPort != NULL) &&
           (FromPort->content.RouteVideoFromPort == (struct AvPort*)(DscPort)) &&
           ((FromPort->type == VideoScaler) || (FromPort->type == VideoColor)))
        {
            if(value == 5) /* 30-bit color depth */
                value = 0x14;
            else if(value == 6)  /* 36-bit color depth */
                value = 0x18;
            else
                value = 0x10;
            if(port->index == 4)
            {
                if(value == 0x10)
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x77,0xFF,0x0,0x00);
                }
                else
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x78,0xFF,0x0,value);
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x77,0xFF,0x0,0x80);
                }
            }
            else if(port->index == 5)
            {
                if(value == 0x10)
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xA7,0xFF,0x0,0x00);
                }
                else
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xA8,0xFF,0x0,value);
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xA7,0xFF,0x0,0x80);
                }
            }
        }
    #endif
    }
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxGetAudioInternalMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value = 0;
    if(port == FindCore1(port))
    {
        GSV5K1_INT_get_RX1_HDMI_AVMUTE_RAW_ST(port, &value);
        if(value == 0)
        {
            GSV5K1_INT_get_RX1_PKT_FLATLINE_RAW_ST(port, &value);
            if(value == 1)
                GSV5K1_INT_set_RX1_PKT_FLATLINE_CLEAR(port,1);
        }
    }
    if(port->content.audio->AudioMute != value)
    {
        GSV5K1_RXAUD_set_RX_MUTE_AUDIO(port, value);
        port->content.audio->AudioMute = value;
        AvUapiOutputDebugMessage("Port%d: Audio Mute %d", port->index, value);
    }
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetAudioInternalMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    return ret;
}

uint8 Gsv5k1_RxGetPixRepValue(AvPort *port)
{
    uint8 RetVal;
    GSV5K1_RXDIG_get_RX_REPETITION_MAN_EN(port, &RetVal);

    if (RetVal == 1)
    {
        GSV5K1_RXDIG_get_RX_REPETITION_MAN(port, &RetVal);
    }
    else
    {
        GSV5K1_RXDIG_get_RB_RX_PIXEL_REPETITION(port, &RetVal);
    }
    return RetVal;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxSetBlackMute(pio AvPort *port))
{
    AvRet ret = AvOk;
    if((port->content.video->InCs == AV_CS_RGB) ||
       (port->content.video->InCs == AV_CS_DEFAULT_RGB) ||
       (port->content.video->InCs == AV_CS_ADOBE_RGB) ||
       (port->content.video->InCs == AV_CS_LIM_RGB) ||
       (port->content.video->InCs == AV_CS_LIM_ADOBE_RGB) ||
       (port->content.video->InCs == AV_CS_AUTO))
        GSV5K1_TXDIG_set_TX_VIDEO_INPUT_CS(port, 0);
    else
        GSV5K1_TXDIG_set_TX_VIDEO_INPUT_CS(port, 1);
    GSV5K1_TXDIG_set_TX_BLACK_VIDEO_EN(port, port->content.video->Mute.BlkMute);
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxEncryptSink(pin AvPort *port))
{
    AvRet ret = AvOk;
    uint8 value;
    uint8 DviWithHdcp   = 0;
    uint8 Hdcp1p4Enable = 0;
    uint8 Hdcp2p2Enable = 0;

    GSV5K1_TXPHY_get_TX_TMDS_CLK_DRIVER_ENABLE(port, &value);
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
                {
                    DviWithHdcp   = 1;
                    if(port->content.hdcptx->Hdcp2p2SinkSupport == 1)
                    {
                        GSV5K1_TXPHY_set_TX_NOT_READ_EDID_EN(port, 1);
                        if(port->content.hdcptx->HdmiStyle == AV_HDCP_TX_AUTO_FAIL_OUT)
                            Gsv5k1SetTxHdcpVersion(port, AV_HDCP_TX_1P4_FAIL_OUT, 0);
                        else
                            Gsv5k1SetTxHdcpVersion(port, AV_HDCP_TX_1P4_ONLY, 0);
                        GSV5K1_TXPHY_set_TX_NOT_READ_EDID_EN(port, 0);
                    }
                }
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
        GSV5K1_TXPHY_get_TX_HDCP1P4_DESIRED(port, &value);
        if(value != Hdcp1p4Enable)
        {
            GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
            GSV5K1_TXPHY_set_TX_HDCP1P4_DESIRED(port, Hdcp1p4Enable);
            GSV5K1_TXPHY_set_TX_HDCP1P4_ENC_EN(port, Hdcp1p4Enable);
            if(Hdcp1p4Enable)
                AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP 1.4 Cipher Enable", (port->index-4));
        }
    #if AvEnableHdcp2p2Feature
        GSV5K1_TX2P2_get_TX_HDCP2P2_DESIRED(port, &value);
        if(value != Hdcp2p2Enable)
        {
            GSV5K1_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 1);
            GSV5K1_TX2P2_set_TX_HDCP2P2_CLEAR_ERROR(port, 0);
            GSV5K1_TX2P2_set_TX_HDCP2P2_DESIRED(port, Hdcp2p2Enable);
            GSV5K1_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, Hdcp2p2Enable);
            if(Hdcp2p2Enable)
                AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP 2.2 Cipher Enable", (port->index-4));
        }
    #endif
#else
        /* when 2.2 repeater is in AUTO mode, BKSV is automatically cleared */
        /* so tx frame encryption can be enabled directly in this mode */
        if(port->content.hdcptx->Hdcp2p2Flag == 1)
        {
            GSV5K1_TXPHY_set_TX_HDCP1P4_FRAME_ENCRYPTION(port, 1);
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
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxDecryptSink(pin AvPort *port))
{
    AvRet ret = AvOk;
    port->content.hdcptx->HdcpEnabled = 0;
    port->content.hdcptx->Authenticated = 0;
    port->content.hdcptx->HdcpError = 0;

    GSV5K1_TXPHY_set_TX_HDCP1P4_ENC_EN(port, 0);
    GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_BKSV_FLAG(port, 1);
    GSV5K1_TXPHY_set_TX_HDCP1P4_DESIRED(port, 0);
    AvUapiOutputDebugMessage("Tx Port %d: Set Tx HDCP Cipher Disable", (port->index-4));

#if AvEnableHdcp2p2Feature
    GSV5K1_TX2P2_set_TX_HDCP2P2_ENCRYPTION_ENABLE(port, 0);
    GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 1);
    GSV5K1_TX2P2_set_TX_HDCP2P2_DESIRED(port, 0);
    GSV5K1_TX2P2_set_TX_HDCP2P2_FSM_DISABLE(port, 0);
    /* Possible Force HDCP 2.2 Reset for compatibility */
    /*
    if(port->content.hdcptx->Hdcp2p2TxRunning == 1)
        Gsv5k1TxScdcAction(port, 1, 0x74, 0x14, 0x00);
    */
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetHdcpEnable(pin AvPort *port))
{
    AvRet ret = AvOk;
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetBksvListReady(AvPort *port))
{
    AvRet ret = AvOk;
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

    GSV5K1_RXRPT_set_RX_KSV_LIST_NUM(port, port->content.hdcp->SinkTotal);
    GSV5K1_RXRPT_set_RX_KSV_LIST_READY(port, On);
    GSV5K1_RXRPT_set_RX_KSV_LIST_READY_CLR(port, Clear);

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetHdcpMode(pin AvPort *port))
{
    AvRet ret = AvOk;
    /* GSV5k1 has only 1 Hdmi Core */
    if(port->core.HdmiCore == 0)
    {
        if(port->content.hdcp->SinkTotal == 0)
        {
            GSV5K1_RXRPT_set_RX_HDCP1P4_BCAPS(port, AV_BCAPS_RECEIVER_MODE);
            GSV5K1_RXRPT_set_RX_HDCP1P4_BSTATUS(port, AV_BSTATUS_RECEIVER_MODE);
            AvUapiOutputDebugMessage("Port %d: Set BCaps/Status REC mode", port->index);
        }
        else
        {
#if AvEnableSimplifyHdcp
            GSV5K1_RXRPT_set_RX_HDCP1P4_BCAPS(port, AV_BCAPS_RECEIVER_MODE);
            GSV5K1_RXRPT_set_RX_HDCP1P4_BSTATUS(port, AV_BSTATUS_RECEIVER_MODE);
#else
            GSV5K1_RXRPT_set_RX_HDCP1P4_BCAPS(port, AV_BCAPS_REPEATER_MODE);
            GSV5K1_RXRPT_set_RX_HDCP1P4_BSTATUS(port, AV_BSTATUS_REPEATER_MODE);
#endif
            AvUapiOutputDebugMessage("Port %d: Set BCaps/Status REP mode", port->index);
        }
    }

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiHdcp2p2Mode(pin AvPort *port))
{
    AvRet ret = AvOk;
    /* GSV5k1 has only 1 Hdmi Core */
    if(port->core.HdmiCore == 0)
        GSV5K1_RX2P2_set_RX_HDCP2P2_REPEATER_ENABLE(port, 0);
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxGetHdcpEnableStatus(pin AvPort *port))
{
    AvRet ret = AvOk;

    return ret;
}

/**
 * @brief  init cec port
 * @return AvOk: success
 * @note
 */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxCecInit(pin AvPort *port))
{
#if AvEnableCecFeature /* CEC Related */
    GSV5K1_TXCEC_set_CEC_POWER_MODE(port, port->content.cec->CecEnable);
    if(port->content.cec->CecEnable == 1)
    {
        GSV5K1_TXCEC_set_CEC_TX_RETRY(port, 3);
        GSV5K1_TXCEC_set_CEC_RX_3BUFFERS_ENABLE(port, 1);
    }
#endif /* CEC Related */
    return AvOk;

}

AvRet ImplementUapi(Gsv5k1, AvUapiTxCecSetPhysicalAddr(AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    uint32 phyaddr = port->content.tx->PhyAddr;
    GSV5K1_TXCEC_set_CEC_PHY_ADDR(port, phyaddr);
#endif
    return ret;
}

/**
 * @brief  Set the logical address according to structure
 * @return AvOk: success
 */
AvRet ImplementUapi(Gsv5k1, AvUapiTxCecSetLogicalAddr(AvPort *port))
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
            GSV5K1_TXCEC_set_CEC_LOGIC_ADDR0(port, LogAddr);
            DevBit = 0x01;
            break;
        case 1:
            GSV5K1_TXCEC_set_CEC_LOGIC_ADDR1(port, LogAddr);
            DevBit = 0x02;
            break;
        case 2:
            GSV5K1_TXCEC_set_CEC_LOGIC_ADDR2(port, LogAddr);
            DevBit = 0x04;
            break;
        default:
            ret = AvInvalidParameter;
            break;
    }

    GSV5K1_TXCEC_get_CEC_LOGIC_ADDR_SELECTION(port, &Mask);
    Mask &= (~DevBit);
    if (Enable)
    {
        Mask |= DevBit;
    }
    if (ret == AvOk)
    {
        GSV5K1_TXCEC_set_CEC_LOGIC_ADDR_SELECTION(port, Mask);
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

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCecRxGetStatus (AvPort* port))
{
    AvRet ret = AvNotAvailable;
#if AvEnableCecFeature /* CEC Related */
    uint8 BufferOrder = 0;
    uint8 SelectOrder = 0;
    uint8 value0, value1, value2;
    uint8 position = 0;

    /* 1. Find if there is message existed */
    GSV5K1_TXCEC_get_CEC_RX_BUF0_READY(port, &value0);
    GSV5K1_TXCEC_get_CEC_RX_BUF1_READY(port, &value1);
    GSV5K1_TXCEC_get_CEC_RX_BUF2_READY(port, &value2);
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
    GSV5K1_TXCEC_get_CEC_RX_BUF0_TIMESTAMP(port, &BufferOrder);
    if(BufferOrder != 0)
    {
        SelectOrder = BufferOrder;
        position = 0;
    }
    GSV5K1_TXCEC_get_CEC_RX_BUF1_TIMESTAMP(port, &BufferOrder);
    if((SelectOrder == 0) ||
       ((BufferOrder != 0) &&
       (BufferOrder < SelectOrder)))
    {
        SelectOrder = BufferOrder;
        position = 1;
    }
    GSV5K1_TXCEC_get_CEC_RX_BUF2_TIMESTAMP(port, &BufferOrder);
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
            AvHalI2cReadMultiField(GSV5K1_TXCEC_MAP_ADDR(port), 0x15, 17,
                                   port->content.cec->RxContent);
            GSV5K1_TXCEC_get_CEC_RX_BUF0_FRAME_LENGTH(port, &port->content.cec->RxLen);
            /* CEC buffer1 flag need to do toggling */
            GSV5K1_TXCEC_set_CEC_RX_CLEAR_BUF0(port,1);
            GSV5K1_TXCEC_set_CEC_RX_CLEAR_BUF0(port,0);
            ret = AvOk;
            break;
        case CEC_RX_BUFFER2:
            AvHalI2cReadMultiField(GSV5K1_TXCEC_MAP_ADDR(port), 0x27, 17,
                                   port->content.cec->RxContent);
            GSV5K1_TXCEC_get_CEC_RX_BUF1_FRAME_LENGTH(port, &port->content.cec->RxLen);
            /* CEC buffer2 flag need to do toggling */
            GSV5K1_TXCEC_set_CEC_RX_CLEAR_BUF1(port,1);
            GSV5K1_TXCEC_set_CEC_RX_CLEAR_BUF1(port,0);
            ret = AvOk;
            break;
        case CEC_RX_BUFFER3:
            AvHalI2cReadMultiField(GSV5K1_TXCEC_MAP_ADDR(port), 0x38, 17,
                                   port->content.cec->RxContent);
            GSV5K1_TXCEC_get_CEC_RX_BUF2_FRAME_LENGTH(port, &port->content.cec->RxLen);
            /* CEC buffer3 flag need to do toggling */
            GSV5K1_TXCEC_set_CEC_RX_CLEAR_BUF2(port,1);
            GSV5K1_TXCEC_set_CEC_RX_CLEAR_BUF2(port,0);
            ret = AvOk;
            break;
        default:
            break;
    }
#endif
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCecSendMessage(pin AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    uint8 value;
    GSV5K1_TXCEC_get_CEC_TX_ENABLE(port, &value);
    if(value == 1)
        GSV5K1_TXCEC_set_CEC_TX_ENABLE(port, 0);

    if(port->content.cec->TxLen > 16)
    {
        ret = AvInvalidParameter;
    }
    else
    {
        /* clear all the interrupts */
        if(port->index == 4)
        {
            GSV5K1_INT_set_TX1_TX_CEC_READY_CLEAR(port,          1);
            GSV5K1_INT_set_TX1_TX_ARBITRATION_LOST_CLEAR(port,   1);
            GSV5K1_INT_set_TX1_TX_RETRY_TIMEOUT_CLEAR(port,      1);
        }
        else if(port->index == 5)
        {
            GSV5K1_INT_set_TX2_TX_CEC_READY_CLEAR(port,          1);
            GSV5K1_INT_set_TX2_TX_ARBITRATION_LOST_CLEAR(port,   1);
            GSV5K1_INT_set_TX2_TX_RETRY_TIMEOUT_CLEAR(port,      1);
        }

        /* CecTxState = CEC_TX_STATE_BUSY; */
        /* 1. Action */
        AvHalI2cWriteMultiField(GSV5K1_TXCEC_MAP_ADDR(port),
                                0x00,
                                port->content.cec->TxLen,
                                port->content.cec->TxContent);
        GSV5K1_TXCEC_set_CEC_TX_FRAME_LENGTH(port, port->content.cec->TxLen);
        GSV5K1_TXCEC_set_CEC_TX_ENABLE(port, 1);
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

uapi AvRet ImplementUapi(Gsv5k1, AvUapiGetNackCount(pin AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    GSV5K1_TXCEC_get_CEC_TX_NACK_COUNTER(port, &port->content.cec->NackCount);
#endif /* CEC Related */
    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCecTxGetStatus(pin AvPort *port))
{
    AvRet ret = AvOk;
#if AvEnableCecFeature /* CEC Related */
    if(port->index == 4)
    {
        GSV5K1_INT_get_TX1_TX_CEC_READY_INT_ST(port,        &port->content.cec->TxReady);
        GSV5K1_INT_get_TX1_TX_ARBITRATION_LOST_INT_ST(port, &port->content.cec->ArbLost);
        GSV5K1_INT_get_TX1_TX_RETRY_TIMEOUT_INT_ST(port,    &port->content.cec->Timeout);
    }
    else if(port->index == 5)
    {
        GSV5K1_INT_get_TX2_TX_CEC_READY_INT_ST(port,        &port->content.cec->TxReady);
        GSV5K1_INT_get_TX2_TX_ARBITRATION_LOST_INT_ST(port, &port->content.cec->ArbLost);
        GSV5K1_INT_get_TX2_TX_RETRY_TIMEOUT_INT_ST(port,    &port->content.cec->Timeout);
    }

    GSV5K1_TXCEC_get_TX_LOWDRIVE_COUNTER(port,              &port->content.cec->LowDriveCount);
    GSV5K1_TXCEC_get_CEC_TX_NACK_COUNTER(port,              &port->content.cec->NackCount);

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
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count))
{
    AvRet ret = AvOk;
    return ret;
}

/* Rx Write Edid */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxWriteEdid(pio AvPort *port, uint8 *Value, uint16 Count))
{
    AvRet ret = AvOk;
    /* using Edid1 */
    AvUapiOutputDebugMessage("Rx%d Writes EDID RAM1, [255]=0x%x .",port->index+1, Value[255]);
    /* Edid1 selection */
    GSV5K1_RXRPT_set_RX_EDID_RAM_SEL(port, 0);
    GSV5K1_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 0);
    AvHalI2cWriteMultiField(GSV5K1_HDMI_EDID_ADDR(port), 0, 256, Value);
    if(Count > 256)
    {
        GSV5K1_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 1);
        AvHalI2cWriteMultiField(GSV5K1_HDMI_EDID_ADDR(port), 0, 256, (Value+256));
    }
    return ret;
}

/* Rx Write SPA */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxSetSpa(pio AvPort *port, uint8 SpaLocation, uint8 *SpaValue))
{
    AvRet ret = AvOk;

#if (AvDontCareEdidSpa == 1)
    /* set SPA pointer to invalid 0x01 */
    GSV5K1_RXRPT_set_RX_EDID1_SPA_LOC_LSB(port, 0);
    GSV5K1_RXRPT_set_RX_EDID1_SPA_LOC_LSB(port, 1);
    GSV5K1_RXRPT_set_RX_EDID2_SPA_LOC_MSB(port, 0);
    GSV5K1_RXRPT_set_RX_EDID2_SPA_LOC_LSB(port, 1);
    /* set SPA data for invalid 0x01 to be 0xff/0xff */
    GSV5K1_RXRPT_set_RXB_SPA(port, 0xffff);
    GSV5K1_RXRPT_set_RXC_SPA(port, 0xffff);
    GSV5K1_RXRPT_set_RXD_SPA(port, 0xffff);
#else //(AvDontCareEdidSpa == 0)
    uint8 value = 0;
    uint8 RamSel = 0;
    uint16 Spa = 0;
    uint8 SpaShift = 0;

    /* 1. Set SPA Location */
    /* using Edid1 */
    GSV5K1_RXRPT_set_RX_EDID1_SPA_LOC_LSB(port, SpaLocation);
    RamSel = 0;
    if(SpaLocation < 128)
    {
        GSV5K1_RXRPT_set_RX_EDID_RAM_SEL(port, RamSel);
        GSV5K1_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 0);
        Spa = (SpaValue[0]<<8)+SpaValue[1];
        if(port->index == 0)
            AvHalI2cWriteMultiField(GSV5K1_HDMI_EDID_ADDR(port), SpaLocation, 2, SpaValue);
        else if(port->index == 1)
            GSV5K1_RXRPT_set_RXB_SPA(port, Spa);
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
    GSV5K1_RXRPT_set_RX_EDID_RAM_SEL(port, RamSel);
    GSV5K1_RXRPT_set_RX_EDID_RAM_PAGE_SEL(port, 0);
    Spa = Spa + 1; /* e.g. XX10 */
    SpaValue[0] = (Spa<<SpaShift)>>8;
    SpaValue[1] = (Spa<<SpaShift)&0xff;
    AvHalI2cWriteMultiField(GSV5K1_HDMI_EDID_ADDR(port), SpaLocation, 2, SpaValue);
    /* RxPort 1, write Reg */
    Spa = Spa + 1; /* e.g. XX20 */
    if(port->index == 1)
        GSV5K1_RXRPT_set_RXB_SPA(port, (Spa<<SpaShift));

#endif
    return ret;
}

/* Tx Read Edid */
uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxReadEdid(pio AvPort *port, uint8 *Value, uint16 Count))
{
    AvRet ret = AvOk;
    uint8 ReadSuccess = 0;
    uint16 i = 0;
    uint8 CheckSum = 0;
    uint8 FsmState = 0;
    uint8 value = 0;

#if AvEdidStoredInRam
    /* Copy RAM into Edid Readback */
    if((port->type == HdmiTx) && (port->content.tx->EdidReadSuccess == AV_EDID_UPDATED))
    {
        switch(port->index)
        {
            case 4:
                AvMemcpy(Value, TxAEdidRam(port), Count);
                AvUapiOutputDebugMessage("Port%d: ReRead Edid From TxARam",port->index);
                return ret;
            case 5:
                AvMemcpy(Value, TxBEdidRam(port), Count);
                AvUapiOutputDebugMessage("Port%d: ReRead Edid From TxBRam",port->index);
                return ret;
        }
    }
#endif

    /* protect EDID reread from Tx Port Reset */
    while((ReadSuccess == 0) && (i < 3000))
    {
        GSV5K1_TXPHY_get_TX_EDID_READY_RB(port, &ReadSuccess);
        if(ReadSuccess == 0)
        {
            GSV5K1_TXPHY_get_TX_HDCP1P4_STATE_RB(port,   &FsmState);
            if(FsmState == 1) /* DDC Reading EDID */
            {
                value = Gsv5k1TxDDCError(port);
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
#if (AvEdidStoredInRam == 1)
        GSV5K1_TXPHY_get_TX_DDC_READ_EDID_RAM_PART(port, &value);
        if(value != 0)
        {
            if(value >= (AvEdidMaxSize/256))
                value = (AvEdidMaxSize/256) - 1;
            if(value != 0)
            {
                if(value != ((AvEdidMaxSize/256)-1))
                    AvMemset(Value+(256*(value+1)), 0, AvEdidMaxSize-(256*(value+1)));
            }
            switch(port->index)
            {
                case 4:
                    AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, 256, TxAEdidRam(port)+(256*value));
                    AvMemcpy(Value, TxAEdidRam(port), Count);
                    break;
                case 5:
                    AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, 256, TxBEdidRam(port)+(256*value));
                    AvMemcpy(Value, TxBEdidRam(port), Count);
                    break;
            }
        }
        else
        {
            AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, Count, Value);
            AvMemset(Value+256, 0, AvEdidMaxSize-256);
        }
        AvUapiOutputDebugMessage("Port%d: Store Edid %d",port->index, value);
#else
        AvHalI2cReadMultiField(GSV5K1_TXEDID_MAP_ADDR(port), 0, Count, Value);
#endif
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
        AvMemset(Value, 0, AvEdidMaxSize);
#if AvForceDefaultEdid
        port->content.tx->IgnoreEdidError = 1;
#else
        /* reread the Edid */
        GSV5K1_TXPHY_set_TX_EDID_TRY_TIMES(port,15);
        GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 0);
        GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 1);
#endif
        return AvError;
    }

    if(port->content.tx->EdidReadSuccess != AV_EDID_UPDATED)
    {
#if AvEnableHdcp1p4BksvCheck
        if(port->content.hdcptx->HdcpSupport == 0)
        {
            /* Check Bskv for HDCP 1.4 Support Capability */
            GSV5K1_TXPHY_set_EDID_ANALYZE_DONE(port, 1);
            Gsv5k1ManualBksvRead(port);
            GSV5K1_TXPHY_set_EDID_ANALYZE_DONE(port, 0);
        }
#else
        AvUapiTxGetSinkHdcpCapability(port);
#endif
#if AvEdidStoredInRam
        #if (AvEdidMaxSize == 512)
        /* CEA block reorder */
        uint8 EdidBuffer[128];
        if(Value[0x7E] > 1)
        {
            if(Value[0x80] != 0x02)
            {
                if(Value[0x100] == 0x02)
                {
                    AvMemcpy(EdidBuffer,Value+0x80,128);
                    AvMemcpy(Value+0x80,Value+0x100,128);
                    AvMemcpy(Value+0x100,EdidBuffer,128);
                }
                else if((Value[0x180] == 0x02) && (Value[0x7E] == 3))
                {
                    AvMemcpy(EdidBuffer,Value+0x80,128);
                    AvMemcpy(Value+0x80,Value+0x180,128);
                    AvMemcpy(Value+0x180,EdidBuffer,128);
                }
            }
        }
        /* Write Read Edid into RAM */
        switch(port->index)
        {
            case 4:
                AvMemcpy(TxAEdidRam(port), Value, AvEdidMaxSize);
                AvUapiOutputDebugMessage("Port%d: Write Edid To TxARam",port->index);
                break;
            case 5:
                AvMemcpy(TxBEdidRam(port), Value, AvEdidMaxSize);
                AvUapiOutputDebugMessage("Port%d: Write Edid To TxBRam",port->index);
                break;
        }
        #endif
#endif
        /* Manually disable Edid Read Success, EDID will be read in Event */
        port->content.tx->EdidReadSuccess = AV_EDID_UPDATED;
    }
    else
    {
        ret = AvInvalidParameter;
    }

    return ret;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxEnableInternalEdid(pio AvPort *port))
{
    /* 1. enable internal Edid */
    switch(port->index)
    {
        case 0: /* rx port 0 */
            GSV5K1_RXRPT_set_RXA_EDID_RAM_SEL(port, 0);
            GSV5K1_RXRPT_set_RX_EDID_CHECKSUM_RECALC(port, 1);
            GSV5K1_RXRPT_set_RXA_EDID_EN(port, 1);
            break;

        case 1: /* rx port 1 */
            GSV5K1_RXRPT_set_RXB_EDID_RAM_SEL(port, 0);
            GSV5K1_RXRPT_set_RX_EDID_CHECKSUM_RECALC(port, 1);
            GSV5K1_RXRPT_set_RXB_EDID_EN(port, 1);
            break;

        default:
            break;
    }

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxArcEnable(pin AvPort *port, uint8 value))
{
    if(port->index == 4)
        GSV5K1_PRIM_set_TXA_ARC_PWR_DN(port, 1-value);
    else if(port->index == 5)
        GSV5K1_PRIM_set_TXB_ARC_PWR_DN(port, 1-value);
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiRxHdcp2p2Manage(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiTxHdcp2p2Manage(pin AvPort *port))
{
    return AvOk;
}

AvPort *Gsv5k1FindHdmiRxFront(pin AvPort *port)
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

AvPort *Gsv5k1FindDscFront(pin AvPort *port)
{
    AvPort *FromPort = (AvPort *)port->content.RouteVideoFromPort;
    while(FromPort != NULL)
    {
        if(FromPort->type == VideoDsc)
            return FromPort;
        else
            FromPort = (AvPort *)FromPort->content.RouteVideoFromPort;
    }
    return NULL;
}

AvPort *Gsv5k1FindDscPort(pin AvPort *port)
{
    AvPort *TempPort = (AvPort *)port->device->port;
    while((TempPort != NULL) && /*  */
          (TempPort->device->index == port->device->index))
    {
        if(TempPort->type == VideoDsc)
            return TempPort;
        else
            TempPort = (AvPort*)(TempPort->next);
    }
    return NULL;
}

AvVideoCs Gsv5k1ColorCsMapping(AvVideoCs InputCs)
{
    AvVideoCs ReturnCs = InputCs;
    if(InputCs == AV_CS_YUV_709)
        ReturnCs = AV_CS_YCC_709;
    else if(InputCs == AV_CS_YUV_601)
        ReturnCs = AV_CS_YCC_601;
    else if(InputCs == AV_CS_LIM_YUV_709)
        ReturnCs = AV_CS_YCC_709;
    else if(InputCs == AV_CS_LIM_YUV_601)
        ReturnCs = AV_CS_YCC_601;
    else if(InputCs == AV_CS_DEFAULT_RGB)
        ReturnCs = AV_CS_RGB;
    else
        ReturnCs = InputCs;
    return ReturnCs;
}

void Gsv5k1ManualCpParameter(AvPort *port)
{
    AvPort *FromPort = (AvPort*)(port->content.RouteVideoFromPort);
    uint32 OldParam = 0;
    uint32 temp = 0;
    uint8  Y420Flag = 0;

    if((FromPort->type == HdmiRx) && (FromPort->core.HdmiCore != -1))
    {
        /* YCbCr 420 mode detection */
        AvHalI2cReadField8(GSV5K1_RXAUD_MAP_ADDR(FromPort),0x19,0x40,0,&Y420Flag);
        if(Y420Flag == 0x40)
        {
            AvHalI2cReadField8(GSV5K1_RXINFO_MAP_ADDR(FromPort),0x01,0xE0,0,&Y420Flag);
            if((Y420Flag == 0x60) && (port->type == VideoScaler))
                Y420Flag = 1;
            else
                Y420Flag = 0;
        }
        else
            Y420Flag = 0;
        GSV5K1_RXDIG_get_RB_RX_H_ACTIVE_WIDTH(FromPort, &temp);
        temp = temp << Y420Flag;
        GSV5K1_VSP_get_VIN_H_ACTIVE(port, &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_H_ACTIVE(port, temp);
        GSV5K1_RXDIG_get_RB_RX_H_TOTAL_WIDTH(FromPort, &temp);
        temp = temp << Y420Flag;
        GSV5K1_VSP_get_VIN_H_TOTAL(port,  &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_H_TOTAL(port,  temp);
        GSV5K1_RXDIG_get_RB_RX_H_FRONT_PORCH(FromPort, &temp);
        temp = temp << Y420Flag;
        GSV5K1_VSP_get_VIN_H_FRONT(port,  &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_H_FRONT(port,  temp);
        GSV5K1_RXDIG_get_RB_RX_H_SYNC_WIDTH(FromPort, &temp);
        temp = temp << Y420Flag;
        GSV5K1_VSP_get_VIN_H_SYNC(port,   &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_H_SYNC(port,   temp);
        GSV5K1_RXDIG_get_RB_RX_H_BACK_PORCH(FromPort, &temp);
        temp = temp << Y420Flag;
        GSV5K1_VSP_get_VIN_H_BACK(port,   &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_H_BACK(port,   temp);
        GSV5K1_RXDIG_get_RB_RX_V_TOTAL_0(FromPort, &temp);
        temp = temp/2;
        GSV5K1_VSP_get_VIN_V_TOTAL(port,  &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_V_TOTAL(port,  temp);
        GSV5K1_RXDIG_get_RB_RX_V_ACTIVE_HEIGHT_0(FromPort, &temp);
        GSV5K1_VSP_get_VIN_V_ACTIVE(port, &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_V_ACTIVE(port, temp);
        GSV5K1_RXDIG_get_RB_RX_V_FRONT_PORCH_0(FromPort, &temp);
        temp = temp/2;
        GSV5K1_VSP_get_VIN_V_FRONT(port,  &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_V_FRONT(port,  temp);
        GSV5K1_RXDIG_get_RB_RX_V_SYNC_WIDTH_0(FromPort, &temp);
        temp = temp/2;
        GSV5K1_VSP_get_VIN_V_SYNC(port,   &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_V_SYNC(port,   temp);
        GSV5K1_RXDIG_get_RB_RX_V_BACK_PORCH_0(FromPort, &temp);
        if(temp%2 == 1)
            temp = temp + 1;
        temp = temp/2;
        GSV5K1_VSP_get_VIN_V_BACK(port,   &OldParam);
        if(OldParam != temp)
            GSV5K1_VSP_set_VIN_V_BACK(port,   temp);
    }
}

void Gsv5k1EnableDscOutMonitor(AvPort *port)
{
    uint8 value = 0x00;
    GSV5K1_PRIM_set_ONLY_USE_RX1_EN(port, 0); /* Use free RxA PLL to monitor */
    GSV5K1_PRIM_set_RX2_CORE_SEL(port, 0); /* Rx2 Core points to RxA */
    GSV5K1_PRIM_set_RX2_CORE_EN(port, 1); /* Enable Rx2 Core */
    AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port),0xB4,0xFF,0,0x02);
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0x23,0xFF,0,0x03);
    AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port),0x7C,0xFF,0,0x60);
    AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port),0x60,0xFF,0,0x89);
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0x17,0xFF,0,0x80);
    switch(port->content.dsc->DscRatio)
    {
        case AV_DSC_2BY1:
            value = 0x20;
            break;
        case AV_DSC_3BY1:
        case AV_DSC_6BY1:
            value = 0x30;
            break;
        case AV_DSC_4BY1:
        case AV_DSC_8BY1:
            value = 0x40;
            break;
        default:
            value = 0x10;
            break;
    }
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0x18,0xFF,0,value);
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0xE2,0xFF,0,0x90);
    GSV5K1_VSP_set_SCAL1_CAL_DLY_THRESHOLD(port, 0); /* Strict CP Delay Threshold */
    GSV5K1_SEC_set_CP_SRC_SEL(port, 2); /* DSC decrypt to CP */
    GSV5K1_SEC_set_CP_MODE(port, 3); /* 444 downscaler works in this mode */
}

void Gsv5k1DisableDscOutMonitor(AvPort *port)
{
    GSV5K1_PRIM_set_ONLY_USE_RX1_EN(port, 1); /* Disable RxA PLL to monitor */
    GSV5K1_PRIM_set_RX2_CORE_SEL(port, 7); /* Rx2 Core points to invalid RxPort */
    GSV5K1_PRIM_set_RX2_CORE_EN(port, 0); /* Disable Rx2 Core */
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0x23,0xFF,0,0x40);
    AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port),0x7C,0xFF,0,0x00);
    AvHalI2cWriteField8(GSV5K1_APLL_MAP_ADDR(port),0x60,0xFF,0,0x88);
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0x17,0xFF,0,0x00);
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0x18,0xFF,0,0x10);
    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port), 0xE2,0xFF,0,0x00);
    GSV5K1_VSP_set_SCAL1_CAL_DLY_THRESHOLD(port, 5); /* Strict CP Delay Threshold */
    GSV5K1_SEC_set_CP_SRC_SEL(port, 0); /* CP to Rx1 by default */
}

void Gsv5k1CheckDsc420InputSetting(AvPort *port)
{
    AvPort *RxPort = NULL;
    AvPort *DscPort = NULL;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;
    uint8  value = 0;
    uint8  AutoFlag = 1;
    uint32 k = 0;
    uint32 DefaultMapAddress = GSV5K1_PRIM_MAP_ADDR(port);

    RxPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));
    DscPort = (AvPort*)(Gsv5k1FindDscPort(port));
    /* 1. Check Rx is stable and 420 is reliable */
    if((RxPort != NULL) &&
       ((RxPort->type == HdmiRx) || (RxPort->type == VideoGen)) &&
       (RxPort->content.rx->IsFreeRun == 1))
    {
        /* 1.1 Check Manual Flag */
        GSV5K1_SEC_get_CP_MODE(port,&value);
if(TrxAvUpStreamFlag == 1)
{
        if((RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420) && (value == 0))
            AutoFlag = 0;
        else
            AutoFlag = 1;
}
if(TrxAvUpStreamFlag == 0)
{
        if((RxPort->content.video->Y == AV_Y2Y1Y0_YCBCR_420) && (value == 0))
            AutoFlag = 0;
#if AvEnableColorMatchInput
        else if((port->content.RouteVideoToPort != NULL) &&
                (port->content.RouteVideoFromPort == (struct AvPort*)(DscPort)) &&
                (DscPort->content.RouteVideoFromPort == (struct AvPort*)(RxPort)) &&
                (DscPort->content.dsc->DscInCompressed == 1) &&
                ((DscPort->content.dsc->SrcInCd == AV_CD_30) ||
                 (DscPort->content.dsc->SrcInCd == AV_CD_36)))
            /* Rx->VP->CP->Tx */
            AutoFlag = 2;
#endif
        else
            AutoFlag = 1;
}
        /* 1.2 Check Set or Not */
        GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, &value);
#if AvEnableColorMatchInput
        if(value == 0)
        {
            AvHalI2cReadField8(GSV5K1_PLL_MAP_ADDR(port),0xE0,0xFF,0,&value);
            if(value == 0x84)
                value = 2;
            else
                value = 0;
        }
#endif
        if(value != AutoFlag)
        {
            /* enable DSC output reset */
            GSV5K1_PRIM_set_VSP_RST(port, 1);
if(TrxAvUpStreamFlag == 1)
{
            /* detail setting */
            if(AutoFlag == 1)
            {
                if(RxPort->index == 0)
                    AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x87);
                else if(RxPort->index == 1)
                    AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x97);
                for(k=0; Gsv5k1RxInNon420Table[k]!=0xFF; k=k+3)
                    AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxInNon420Table[k],
                                        Gsv5k1RxInNon420Table[k+1],0xFF,0,Gsv5k1RxInNon420Table[k+2]);
            }
            else
            {
                /* Set Rx PLL Ratio */
                value = 0x20; /* 420 input conversion */

                if(RxPort->index == 0)
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x47,0xFF,0x0,0x80);
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x48,0xFF,0x0,value);
                    for(k=0; Gsv5k1RxAIn420Table[k]!=0xFF; k=k+3)
                        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxAIn420Table[k],
                                            Gsv5k1RxAIn420Table[k+1],0xFF,0,Gsv5k1RxAIn420Table[k+2]);
                }
                else if(RxPort->index == 1)
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x17,0xFF,0x0,0x80);
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x18,0xFF,0x0,value);
                    for(k=0; Gsv5k1RxBIn420Table[k]!=0xFF; k=k+3)
                        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxBIn420Table[k],
                                            Gsv5k1RxBIn420Table[k+1],0xFF,0,Gsv5k1RxBIn420Table[k+2]);
                }
            }
            /* VP/Tx Output Clock Manual Setting */
            if(DscPort != NULL)
            {
                CurrentPort = NULL;
                PrevPort = NULL;
                while(KfunFindVideoNextTxEnd(DscPort, &PrevPort, &CurrentPort) == AvOk)
                {
                    if(CurrentPort->type == HdmiTx)
                    {
                        /* 1. Enable VP Manual Output */
                        if(AutoFlag == 1)
                            value = 0x00;
                        else
                            value = 0x81;
                        AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE1,0xFF,0x0,value);
                        /* 2. Enable when Tx is Dsc Output */
                        if(AutoFlag == 1)
                            value = 0x00;
                        else if(DscPort->content.dsc->DscInCompressed == 0)
                            value = 0x8A;
                        else
                            value = 0x00;
                        if(CurrentPort->index == 4)
                        {
                            AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE4,0xFF,0x0,value);
                        }
                        else if(CurrentPort->index == 5)
                        {
                            AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE5,0xFF,0x0,value);
                        }
                        /* 3. Resend GC Packet */
                        CurrentPort->content.video->AvailableVideoPackets =
                            CurrentPort->content.video->AvailableVideoPackets & (~AV_BIT_GC_PACKET);
                    }
                    PrevPort = CurrentPort;
                }
            }
}
if(TrxAvUpStreamFlag == 0)
{
            /* detail setting */
            if(AutoFlag == 1)
            {
                if(RxPort->index == 0)
                    AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x87);
                else if(RxPort->index == 1)
                    AvHalI2cWriteField8(DefaultMapAddress,0x01,0xFF,0,0x97);
                for(k=0; Gsv5k1RxInNon420Table[k]!=0xFF; k=k+3)
                    AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxInNon420Table[k],
                                        Gsv5k1RxInNon420Table[k+1],0xFF,0,Gsv5k1RxInNon420Table[k+2]);
            }
            else
            {
                /* Set Rx PLL Ratio */
#if AvEnableColorMatchInput
                if(AutoFlag == 2)
                {
                    switch(DscPort->content.dsc->DscRatio)
                    {
                        case AV_DSC_8BY1:
                            value = 0x80;
                            break;
                        case AV_DSC_6BY1:
                            value = 0x60;
                            break;
                        case AV_DSC_4BY1:
                            value = 0x40;
                            break;
                        case AV_DSC_3BY1:
                            value = 0x30;
                            break;
                        case AV_DSC_2BY1:
                            value = 0x20;
                            break;
                    }
                }
                else
#endif
                    value = 0x20; /* 420 input conversion */

                if(RxPort->index == 0)
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x47,0xFF,0x0,0x80);
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x48,0xFF,0x0,value);
                    for(k=0; Gsv5k1RxAIn420Table[k]!=0xFF; k=k+3)
                        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxAIn420Table[k],
                                            Gsv5k1RxAIn420Table[k+1],0xFF,0,Gsv5k1RxAIn420Table[k+2]);
                }
                else if(RxPort->index == 1)
                {
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x17,0xFF,0x0,0x80);
                    AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x18,0xFF,0x0,value);
                    for(k=0; Gsv5k1RxBIn420Table[k]!=0xFF; k=k+3)
                        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1RxBIn420Table[k],
                                            Gsv5k1RxBIn420Table[k+1],0xFF,0,Gsv5k1RxBIn420Table[k+2]);
                }
#if AvEnableColorMatchInput
                if(AutoFlag == 2)
                {
                    for(k=0; Gsv5k1Rx420DcTable[k]!=0xFF; k=k+3)
                        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1Rx420DcTable[k],
                                            Gsv5k1Rx420DcTable[k+1],0xFF,0,Gsv5k1Rx420DcTable[k+2]);
                }
#endif
            }
            /* VP/Tx Output Clock Manual Setting */
            if(DscPort != NULL)
            {
                CurrentPort = NULL;
                PrevPort = NULL;
                while(KfunFindVideoNextTxEnd(DscPort, &PrevPort, &CurrentPort) == AvOk)
                {
                    if(CurrentPort->type == HdmiTx)
                    {
                        /* 1. Enable VP Manual Output */
                        if(AutoFlag == 1)
                            value = 0x00;
                        else
                            value = 0x81;
                        AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE1,0xFF,0x0,value);
                        /* 2. Enable when Tx is Dsc Output */
                        if(AutoFlag == 1)
                            value = 0x00;
                        else if(DscPort->content.dsc->DscInCompressed == 0)
                            value = 0x8A;
#if AvEnableColorMatchInput
                        else if(AutoFlag == 2)
                            value = 0x8D;
#endif
                        else
                            value = 0x00;
                        if(CurrentPort->index == 4)
                        {
                            AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE4,0xFF,0x0,value);
                            if(value == 0x8D)
                            {
                                AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x78,0xFF,0x0,0x10);
                                AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x77,0xFF,0x0,0x80);
                                GSV5K1_TXDIG_set_TX_GC_CD(CurrentPort, 4);
                            }
                        }
                        else if(CurrentPort->index == 5)
                        {
                            AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xE5,0xFF,0x0,value);
                            if(value == 0x8D)
                            {
                                AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xA8,0xFF,0x0,0x10);
                                AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xA7,0xFF,0x0,0x80);
                                GSV5K1_TXDIG_set_TX_GC_CD(CurrentPort, 4);
                            }
                        }
                        /* 3. Resend GC Packet */
                        CurrentPort->content.video->AvailableVideoPackets =
                            CurrentPort->content.video->AvailableVideoPackets & (~AV_BIT_GC_PACKET);
                    }
                    PrevPort = CurrentPort;
                }
            }
}
            /* disable DSC output reset */
            GSV5K1_PRIM_set_VSP_RST(port, 0);
            DscPort->content.dsc->OutDelay = 0;
        }
    }
}

void Gsv5k1TxPllUnlockClear(AvPort *port)
{
    switch(port->index)
    {
        case 4: /* TxA */
            GSV5K1_PLL_set_TXA_PLL_LOCK_CLEAR(port, 1);
            GSV5K1_PLL_set_TXA_PLL_LOCK_CLEAR(port, 0);
            break;
        case 5: /* TxB */
            GSV5K1_PLL_set_TXB_PLL_LOCK_CLEAR(port, 1);
            GSV5K1_PLL_set_TXB_PLL_LOCK_CLEAR(port, 0);
            break;
    }
}

uint8 Gsv5k1TweakCrystalFreq(AvPort *port, uint32 value32)
{
    uint8 CrystalFreq = 0;
    /* Check Frequency with 0xff/0x00 end */
    uint8 value = value32 & 0xff;
    if((value == 0x00) || (value == 0xff))
    {
        /* Change Crystal Frequency */
        AvHalI2cReadField8(GSV5K1_PLL_MAP_ADDR(port),0x01,0xFF,0,&CrystalFreq);
        if(CrystalFreq == 0x00)
            AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x01,0xFF,0,0x0F);
        else
            AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0x01,0xFF,0,0x00);
        return 1;
    }
    return 0;
}

void Gsv5k1MpllProtect(AvPort *port)
{
    uint8 TxADefaultRouting;
    uint8 i;
    uint8 value;
    uint8 ValidMpllFreq = 0;
    /* Step 1. Store TxA Default Routing */
    AvHalI2cReadField8(GSV5K1_SEC_MAP_ADDR(port),0x05,0x07,0,&TxADefaultRouting);
    /* Step 2. Write TxA Default Routing to VG */
    AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0x05,0x07,0,0x04);
    /* Step 3. Delay and read MPLL measure state */
    while(ValidMpllFreq == 0)
    {
        for(i=0;i<15;i++)
            AvHalI2cReadField8(GSV5K1_PLL_MAP_ADDR(port),0x9C,0xFF,0,&value);
        while(value != 0x20)
            AvHalI2cReadField8(GSV5K1_PLL_MAP_ADDR(port),0x9C,0xFF,0,&value);
        /* Step 4. Measure MPLL Frequency */
        AvHalI2cReadField8(GSV5K1_PLL_MAP_ADDR(port),0x91,0xFF,0,&value);
        /* Step 5. MPLL Frequency Recovery */
        if((value < 0x81) || (value > 0x95))
        {
            AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0xC5,0xFF,0,0xCC);
            AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0xC6,0xFF,0,0x01);
            AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0xC5,0xFF,0,0x0C);
        }
        else
            ValidMpllFreq = 1;
    }
    /* Step 6. Write TxA Default Routing to VG */
    AvHalI2cWriteField8(GSV5K1_SEC_MAP_ADDR(port),0x05,0x07,0,TxADefaultRouting);
}

void Gsv5k1RpllProtect(AvPort *port)
{
    uint32 PllRb1 = 0;
    uint32 PllRb2 = 0;
    uint32 PllDiff = 0;
    /* 1. Read Back Comparison Value */
    switch(port->index)
    {
        case 0:
            GSV5K1_PLL_get_RB_RXA_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV5K1_PLL_get_RB_RXA_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
            break;
        case 1:
            GSV5K1_PLL_get_RB_RXB_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV5K1_PLL_get_RB_RXB_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
            break;
        case 4:
            GSV5K1_PLL_get_RB_TXA_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV5K1_PLL_get_RB_TXA_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
            break;
        case 5:
            GSV5K1_PLL_get_RB_TXB_PLL_REF_PRE_DIV_FREQ(port,  &PllRb1);
            GSV5K1_PLL_get_RB_TXB_PLL_VCO_POST_DIV_FREQ(port, &PllRb2);
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
                GSV5K1_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, 1);
                GSV5K1_APLL_set_RXA_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 1:
                GSV5K1_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, 1);
                GSV5K1_APLL_set_RXB_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 4:
                GSV5K1_APLL_set_TXA_RPLL_FASTLOCK_CTL(port, 1);
                GSV5K1_APLL_set_TXA_RPLL_FASTLOCK_CTL(port, 0);
                break;
            case 5:
                GSV5K1_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, 1);
                GSV5K1_APLL_set_TXB_RPLL_FASTLOCK_CTL(port, 0);
                break;
        }
    }

}

void Gsv5k1RxManualEQUpdate(AvPort *port, uint8 DefaultEQEnable)
{
    uint8  EqDoneFlag = 0;
    uint8  EthernetPortFlag = 0;
    uint8  value = 0;
    uint8  i = 0;
    uint32 PhyPageAddr;
    uint8  EQParam[7]; /* 0x0C, 0x0D, 0x0E, 0x0F, 0x61, 0x55, 0x56 */
    uint8  RbEQParam[4];
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
    /* RxB is used as Ethernet Input Port */
#if (AvHdmiOnEthernet == 1)
    #if Gsv5k1TxUseRxBHdmiMode
    if(port->index == 0)
    #else
    if(port->index == 1)
    #endif
    {
        #if Gsv5k1UseFastEqMode
        EthernetPortFlag = 0;
        EnhancedEqMode = 0;
        #else
        EthernetPortFlag = 1;
        EnhancedEqMode = 1;
        #endif
    }
#endif
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
        PhyPageAddr = GSV5K1_RXLN0_MAP_ADDR(port);
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
                    /* 2.1.2.1 Low Temperature protection in Enhanced mode */
                    AvHalI2cReadField8(PhyPageAddr,0x0E,0xFF,0,&value);
                    if(value == 0x81)
                    {
                        switch(port->index)
                        {
                            case 0:
                                AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x01,0,&value);
                                if(value != 0)
                                {
                                    AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x01,0,0x00);
                                    return;
                                }
                                break;
                            case 1:
                                AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x02,0,&value);
                                if(value != 0)
                                {
                                    AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x02,0,0x00);
                                    return;
                                }
                                break;
                        }
                    }
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
                            EnhancedEqMode = 1;
                            EnhancedFirstRound = 1;
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
                switch(port->index)
                {
                    case 0:
                        AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x01,0,&value);
                        if(value == 0)
                        {
                            AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x01,0,0x01);
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                        }
                        break;
                    case 1:
                        AvHalI2cReadField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x02,0,&value);
                        if(value == 0)
                        {
                            AvHalI2cWriteField8(GSV5K1_PRIM_MAP_ADDR(port),0xB6,0x02,0,0x02);
                            port->content.rx->EQDelayExpire = 0;
                            EnhancedEqMode = 1;
                        }
                        break;
                }
                if((port->content.rx->EQDelayExpire == 0) && (port->content.hdcp->Hdcp2p2RxRunning == 1))
                {
                    GSV5K1_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 1);
                    GSV5K1_RX2P2_set_RX_HDCP2P2_REAUTH_REQUEST(port, 0);
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
        /* Manual lower internal resistor to lower current for short CAT cable */
        if((EthernetPortFlag == 1) &&
           ((BestEyeArea[0] > 9500) || (BestEyeArea[1] > 9500) || (BestEyeArea[2] > 9500)))
        {
            EQParam[0] = 0x07;
            EQParam[1] = 0x07;
            EQParam[2] = 0x07;
            AvHalI2cWriteMultiField(GSV5K1_RXLN0_MAP_ADDR(port),0x55,3,EQParam);
            AvHalI2cWriteField8(GSV5K1_RXLN0_MAP_ADDR(port),0x0D,0xFF,0,0x00);
            AvHalI2cWriteField8(GSV5K1_RXLN0_MAP_ADDR(port),0x3F,0xFF,0,0x22);
            AvHalI2cWriteField8(GSV5K1_RXLN0_MAP_ADDR(port),0x3F,0xFF,0,0x33);
        }
        /* Step 2.2.2 EQ Done and Eye Found */
        /* Step 2.2.2.1 Reset Rx Core Digital Logic for power cycle stability */
        if(port->content.rx->EQDelayExpire == 1)
        {
            GSV5K1_RXDIG_set_DCFIFO_RECENTER(port, 1);
            GSV5K1_RXAUD_set_RX_AUD_FIFO_RST(port, 1);
            GSV5K1_RXAUD_set_RX_AUD_FIFO_RST(port, 0);
        }
        /* Step 2.2.2.2 EQ Lock Set and Exit EQ Process */
        if(port->content.rx->EQDelayExpire >= RxEQDelayExpireThreshold)
        {
            GSV5K1_INT_set_RX1_HS_LOCKED_CLEAR(port, 1);
            GSV5K1_INT_set_RX1_VS_LOCKED_CLEAR(port, 1);
            /* H Lock detection protection */
            /*
            GSV5K1_RXDIG_get_RB_RX_HS_LOCKED(port, &value);
            if((value == 0) && (EthernetPortFlag == 1))
                port->content.rx->EQDelayExpire = 0;
            else
            */
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
            if(EthernetPortFlag == 1)
                EQParam[1] = 0x07;
            else if((EnhancedFirstRound == 1) || (DefaultEQEnable == 1))
                EQParam[1] = 0x07;
            else
                EQParam[1] = 0x81;
            EQParam[2] = 0x81;
            EQParam[3] = 0x81;
            EQParam[0] = 0x81;
            EQParam[4] = 0x01; /* Short Check Time */
        }
        else if(port->content.video->info.LaneFreq < 250)
        {
            EQParam[1] = 0x07;
            EQParam[2] = 0x01;
            EQParam[3] = 0x01;
            EQParam[0] = 0x03;
            if(port->content.video->info.LaneFreq < 60)
            {
                EQParam[0] = 0x00;
                EQParam[4] = 0x01;
            }
            else if(port->content.video->info.LaneFreq < 170)
                EQParam[4] = 0x03;
            else
                EQParam[4] = 0x0F;
        }
        else if(port->content.video->info.LaneFreq < 470)
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
        /* Step 2.3.1.2 Decide EQ Manual Setting */
        if((port->core.HdmiCore != -1) && (EthernetPortFlag == 0) &&
           (EnhancedEqMode == 1) && (EnhancedFirstRound == 0))
        {
            AvHalI2cReadMultiField(PhyPageAddr, 0x0C, 4, RbEQParam);
            if((RbEQParam[0] == EQParam[0]) &&
               (RbEQParam[1] == EQParam[1]) &&
               (RbEQParam[2] == EQParam[2]) &&
               (RbEQParam[3] == EQParam[3]))
            {
                /* Manual EQ Setting for possible false SCDC recovery on clock ratio/scramble*/
                /* RxDIG.0xD9 Registers includes:
                GSV5K1_RXDIG_get_RX_SCDC_TMDS_BIT_CLK_RATIO_MAN_EN(port, pval);
                GSV5K1_RXDIG_set_RX_SCDC_TMDS_BIT_CLK_RATIO_MAN_EN(port, val);
                GSV5K1_RXDIG_get_RX_SCDC_TMDS_BIT_CLK_RATIO_MAN(port, pval);
                GSV5K1_RXDIG_set_RX_SCDC_TMDS_BIT_CLK_RATIO_MAN(port, val);
                GSV5K1_RXDIG_get_RX_SCDC_SCR_EN_MAN_EN(port, pval);
                GSV5K1_RXDIG_set_RX_SCDC_SCR_EN_MAN_EN(port, val);
                GSV5K1_RXDIG_get_RX_SCDC_SCR_EN_MAN(port, pval);
                GSV5K1_RXDIG_set_RX_SCDC_SCR_EN_MAN(port, val);
                 */
                AvHalI2cReadField8(GSV5K1_RXDIG_MAP_ADDR(port),0xD9,0xFF,0,&value);
                if(value == 0xF0)
                    value = 0xA0;
                else
                    value = 0xF0;
                AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0xD9,0xFF,0,value);
                AvUapiOutputDebugMessage("Port Rx %d: Manual SCDC = %x", port->index, value);
            }
        }
        /* Step 2.3.2 Set Lane Registers */
        PhyPageAddr = GSV5K1_RXLN0_MAP_ADDR(port);
        /* Step 2.3.2.1 Update CDR Bandwidth */
        AvHalI2cWriteField8(PhyPageAddr,0x3F,0xFF,0,0x22); //Disable EQ
        if((port->core.HdmiCore != -1) && (EthernetPortFlag == 0) &&
           ((EnhancedEqMode == 0) || (EnhancedFirstRound == 1)))
        {
            AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0xD9,0xFF,0,0x00);
        }
        Gsv5k1UpdateRxCdrBandWidth(port);
        AvHalI2cWriteField8(PhyPageAddr,0x3F,0xFF,0,0x32); //Enable Clock Detection
        /* Step 2.3.2.2 Set EQ calculation time */
        AvHalI2cWriteMultiField(PhyPageAddr,0x0C,4,EQParam);
        AvHalI2cWriteField8(PhyPageAddr,0x61,0xFF,0,EQParam[4]);
        if(EthernetPortFlag == 1)
        {
            EQParam[0] = 0x07;
#if (AvCompressionLowThreshold > 145)
            EQParam[1] = 0x07;
            EQParam[2] = 0x05;
#else
            EQParam[1] = 0x05;
            EQParam[2] = 0x00;
#endif
            AvHalI2cWriteMultiField(GSV5K1_RXLN0_MAP_ADDR(port),0x55,3,EQParam);
        }
        /* Step 2.3.2.3 Enhanced EQ Mode */
        if(EnhancedEqMode == 1)
        {
            AvUapiOutputDebugMessage("Port Rx %d: Restart Adaptive EQ in Enhanced Mode", port->index);
        }
        else
        {
            AvUapiOutputDebugMessage("Port Rx %d: Reset Adaptive EQ in Fast Mode", port->index);
        }
        /* Step 2.3.2.4 Run EQ */
        AvHalI2cWriteField8(PhyPageAddr,0x3F,0xFF,0,0x33); //EQ starts to run
        /* Step 2.3.2.5 Reset Lane Fifo */
        switch(port->index)
        {
            case 0:
                GSV5K1_SEC_set_RXA_FIFO_EN(port, 0);
                GSV5K1_SEC_set_RXA_FIFO_EN(port, 1);
                break;
            case 1:
                GSV5K1_SEC_set_RXB_FIFO_EN(port, 0);
                GSV5K1_SEC_set_RXB_FIFO_EN(port, 1);
                break;
        }
        /* Step 2.4 Start EQ Calibration */
        port->content.rx->EQDelayExpire = 1;
    }

    UNUSED(BestEyeScore[0]);
    return;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckLogicAudioRx(pin AvPort *port))
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
                GSV5K1_TXDIG_get_TX_AUDIO_SPDIF_SF_RB(CurrentPort, &value);
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
                (((CurrentPort->content.audio->AvailableAudioPackets) &
                 (AV_BIT_AUDIO_INFO_FRAME | AV_BIT_AUDIO_CHANNEL_STATUS | AV_BIT_AUDIO_SAMPLE_PACKET | AV_BIT_ACR_PACKET))
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
                    if((CurrentPort->content.audio->AudCoding == AV_AUD_FORMAT_LINEAR_PCM) || (CurrentPort->content.audio->AudCoding == 0x00))
                        CurrentPort->content.audio->Consumer = CurrentPort->content.audio->Consumer & 0x01;
                    else
                        CurrentPort->content.audio->Consumer = CurrentPort->content.audio->Consumer | 0x02;
                    AvUapiTxSetPacketContent(CurrentPort, AV_PKT_AUDIO_CHANNEL_STATUS, PktContent,1);
                    /* 2.2.4 Set AV_BIT_AUDIO_SAMPLE_PACKET, AV_BIT_ACR_PACKET */
                    AvUapiTxSetAudioPackets(CurrentPort);
                }
            /* 1.3 Clear Tx Audio Fifo Status */
            if(CurrentPort->core.HdmiCore == 0)
                GSV5K1_INT_set_TX1_AUDIOFIFO_FULL_CLEAR(CurrentPort, 1);
            else if(CurrentPort->core.HdmiCore == 1)
                GSV5K1_INT_set_TX2_AUDIOFIFO_FULL_CLEAR(CurrentPort, 1);
        }
        PrevPort = CurrentPort;
    }
#endif
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckLogicAudioTx(pin AvPort *port))
{
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
            if((TempPort->content.rx->Lock.AudioLock == 0) || (TempPort->content.audio->AudioMute == 1))
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
        GSV5K1_PRIM_get_AUD1_OEN(port, &OldValue);
        if(NewValue != OldValue)
        {
            GSV5K1_PRIM_set_AUD1_OEN(port, NewValue);
            if((NewValue == 0) && (port->content.audio->WordLen != 0))
            {
                GSV5K1_RXAUD_set_WORD_LEN_MAN_EN(TempPort, 1);
                GSV5K1_RXAUD_set_WORD_LEN_MAN(TempPort, port->content.audio->WordLen);
            }
        }
    }
    /* Step 3. check mclk ratio for I2S output */
    if((NewValue == 0) && (TempPort->type == HdmiRx))
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
        GSV5K1_RXDIG_get_RX_AUDIO_MCLK_X_N(TempPort, &OldValue);
        if(NewValue != OldValue)
            GSV5K1_RXDIG_set_RX_AUDIO_MCLK_X_N(TempPort, NewValue);
    }

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckVideoDsc(pin AvPort *port))
{
    uint8 value = 0;
    uint8 NewValue = 0;
    uint8 DscCfgVic = 0;
    uint8 NewDscCfgFlag = 0;
    AvVideoY TempY = AV_Y2Y1Y0_YCBCR_444;
    uchar *ConfigDscRegName = NULL;
    uchar *ConfigDscRegValue = NULL;
    uchar *ConfigVspRegName = NULL;
    uchar *ConfigVspRegValue = NULL;
    AvPort *CurrentPort = NULL;
    AvPort *PrevPort = NULL;

    /* 1. Check Enable*/
    if((port->content.dsc->Enable == 0) ||
       (port->content.dsc->DscRatio == AV_DSC_1BY1) ||
       (port->content.dsc->DscRatio == AV_CSC_2BY1) ||
       (port->content.RouteVideoToPort == NULL) ||
       (port->content.dsc->DscInVic == 0))
        return AvOk;
    /* 2. Check Ratio */
    GSV5K1_SEC_get_DSC_COM_RATIO(port, &value);
    NewValue = port->content.dsc->DscRatio & 0x03;
    if(value != NewValue)
    {
        GSV5K1_SEC_set_DSC_COM_RATIO(port, NewValue);
        Gsv5k1ResetVsp(port);
    }
    /* 3. Check Vic */
    /* 3.1 Get Config Vic */
    DscCfgVic = port->content.dsc->DscInVic;
    for(value=0;DscPlanVicTable[value].InVic!=255;value++)
    {
        if(DscPlanVicTable[value].InVic == port->content.dsc->DscInVic)
        {
            DscCfgVic = DscPlanVicTable[value].DscVic;
            break;
        }
    }
    /* 3.2 Set Vic Value */
    GSV5K1_DSC_get_DSC_VIC(port, &value);
    if(value != DscCfgVic)
    {
        GSV5K1_DSC_set_DSC_VIC(port, DscCfgVic);
        GSV5K1_DSC_set_DSC_MEM_EMAB(port, 3);/* enable DSC output reset */
        Gsv5k1ResetVsp(port);
        AvUapiOutputDebugFsm("DSC Vic = %x", DscCfgVic);
        port->content.dsc->OutDelay = 0;
        NewDscCfgFlag = 1;
    }
    /* 4.1 4K will have half horizontal downscaling automatically */
    /* VIC = 102, 4096x2160@60Hz does not support horizontal downscaling */
    /* 420 HDMI input has already been changed to 444 by CP,
       DSC input is always YCbCr444 color space */
    /* Optimize 4K Performance by enabling Hactive Half Enable */
    if((port->content.dsc->DscRatio == AV_DSC_6BY1) || (port->content.dsc->DscRatio == AV_DSC_8BY1))
        NewValue = 1;
    else
        NewValue = 0;
    GSV5K1_DSC_get_DSC_H_WAVELET_HALF_EN(port, &value);
    if(value != NewValue)
        GSV5K1_DSC_set_DSC_H_WAVELET_HALF_EN(port, NewValue);
#if (Gsv5k1RomFromOtp == 1)
    /* 4.2 420 10-bit/12b-bit Fix, directly feed to VP */
    /* 4.2.1 4K deep color mode found, no div2 */
    if(((port->content.dsc->DscInVic >= 93) && (port->content.dsc->DscInVic < 128)) &&
       (port->content.dsc->DscInY == AV_Y2Y1Y0_YCBCR_420) &&
       ((port->content.dsc->SrcInCd == AV_CD_30) ||
        (port->content.dsc->SrcInCd == AV_CD_36)))
        NewValue = 0;
#endif
    GSV5K1_SEC_get_DSC_H_DIV2_EN(port, &value);
    if(value != NewValue)
        GSV5K1_SEC_set_DSC_H_DIV2_EN(port, NewValue);

if(TrxAvUpStreamFlag == 1)
{
#if (Gsv5k1RomFromOtp == 0)
    /* 4.2.2 Manual Set Clock Ratio for 420 in R1 */
    GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, &value);
    if(value == 0)
        NewValue = 0x80 | ((((port->content.dsc->DscRatio & 0x03) + 2) * (1 + NewValue)) - 1);
    else
        NewValue = 0x00;
    AvHalI2cReadField8(GSV5K1_PLL_MAP_ADDR(port),0xEC,0xFF,0,&value);
    if(value != NewValue)
    {
        AvHalI2cWriteField8(GSV5K1_PLL_MAP_ADDR(port),0xEC,0xFF,0,NewValue);
        AvUapiOutputDebugFsm("DSC Ratio = %x", NewValue);
        Gsv5k1ResetVsp(port);
    }
#endif
}
    /* 5. Special Timing Fix */
    /* 5.1 Check Special Regs */
    NewValue = 0;
if(TrxAvUpStreamFlag == 1)
{
    if(port->content.dsc->DscInCompressed == 0)
    {
        /* 5.1.1 Only two formats in the table */
        if(port->content.dsc->DscInY != AV_Y2Y1Y0_YCBCR_420)
            TempY = AV_Y2Y1Y0_YCBCR_444;
        else
            TempY = AV_Y2Y1Y0_YCBCR_420;
        /* 5.1.2 Check Table */
        for(value=0;TxDscTables[value].DscMemTimingTable;value++)
        {
            if((TxDscTables[value].Vic   == port->content.dsc->DscInVic) &&
               (TxDscTables[value].Y     == TempY) &&
               (TxDscTables[value].Ratio == port->content.dsc->DscRatio))
            {
                ConfigDscRegName  = (uchar *)EncodeDscMemDscRegTable;
                ConfigDscRegValue = (uchar *)TxDscTables[value].DscMemTimingTable;
                ConfigVspRegName  = (uchar *)EncodeVspMemDscRegTable;
                ConfigVspRegValue = (uchar *)TxDscTables[value].VspMemTimingTable;
                NewValue = 1;
                break;
            }
        }
        GSV5K1_DSC_get_DSC_VIC_MANUAL_EN(port, &value);
        if(value != NewValue)
            NewDscCfgFlag = 1;
        /* 5.1.3 Write needed Registers for extra supported formats */
        if(NewDscCfgFlag == 1)
        {
            if(NewValue == 1)
            {
                /* 5.1.3.1 Write Dsc Map */
                value = 0;
                while(ConfigDscRegName[value] != 0xFF)
                {
                    AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port),ConfigDscRegName[value],0xFF,0,ConfigDscRegValue[value]);
                    value = value + 1;
                }
                /* 5.1.3.2 Write Vsp Map */
                value = 0;
                while(ConfigVspRegName[value] != 0xFF)
                {
                    AvHalI2cWriteField8(GSV5K1_VSP_MAP_ADDR(port),ConfigVspRegName[value],0xFF,0,ConfigVspRegValue[value]);
                    value = value + 1;
                }
            }
            /* 5.1.4 Clear Registers for extra supported formats */
            else
            {
                GSV5K1_DSC_get_DSC_VIC_MANUAL_EN(port, &value);
                /* moving from manual setting to auto setting */
                if(value == 1)
                {
                    GSV5K1_DSC_set_DSC_VIC_MANUAL_EN(port, 0);
                    GSV5K1_DSC_set_DSC_INIT_OUT_DELAY_EN(port, 0);
                    GSV5K1_DSC_set_DSC_QP_CLEAR_EN(port, 0);
                    GSV5K1_DSC_set_DSC_SLICE_CTL_MAN_EN(port, 0);
                    GSV5K1_DSC_set_DSC_SLICE_MULTIPLEX_DELAY_EN(port, 0);
                    GSV5K1_DSC_set_DSC_DECODER_LAST_LINE_REP_EN(port, 0);
                    GSV5K1_DSC_set_DSC_SEQ_OUTPUT_MAN_EN(port, 0);
                    GSV5K1_DSC_set_DSC_RC_CTL_MAN_EN(port, 0);
                    GSV5K1_DSC_set_DSC_ENCODER_DELAY_EN(port, 0);
                }
            }
        }
    }
} /* TrxAvUpStreamFlag == 1 */
if(TrxAvUpStreamFlag == 0)
{
    if(port->content.dsc->DscInCompressed == 1)
    {
        /* 5.1.1 Only two formats in the table */
        if(port->content.dsc->DscInY != AV_Y2Y1Y0_YCBCR_420)
            TempY = AV_Y2Y1Y0_YCBCR_444;
        else
            TempY = AV_Y2Y1Y0_YCBCR_420;
        /* 5.1.2 Check Table */
        for(value=0;RxDscTables[value].DscMemTimingTable;value++)
        {
            if((RxDscTables[value].Vic   == port->content.dsc->DscInVic) &&
               (RxDscTables[value].Y     == TempY) &&
               (RxDscTables[value].Ratio == port->content.dsc->DscRatio))
            {
                ConfigDscRegName  = (uchar *)DecodeDscMemDscRegTable;
                ConfigDscRegValue = (uchar *)RxDscTables[value].DscMemTimingTable;
                ConfigVspRegName  = (uchar *)DecodeVspMemDscRegTable;
                ConfigVspRegValue = (uchar *)RxDscTables[value].VspMemTimingTable;
                NewValue = 1;
                break;
            }
        }
        GSV5K1_DSC_get_DSC_VIC_MANUAL_EN(port, &value);
        if(value != NewValue)
            NewDscCfgFlag = 1;
        /* 5.1.3 Write needed Registers for extra supported formats */
        if(NewDscCfgFlag == 1)
        {
            if(NewValue == 1)
            {
                /* 5.1.3.1 Write Dsc Map */
                value = 0;
                while(ConfigDscRegName[value] != 0xFF)
                {
                    AvHalI2cWriteField8(GSV5K1_DSC_MAP_ADDR(port),ConfigDscRegName[value],0xFF,0,ConfigDscRegValue[value]);
                    value = value + 1;
                }
                /* 5.1.3.2 Write Vsp Map */
                value = 0;
                while(ConfigVspRegName[value] != 0xFF)
                {
                    AvHalI2cWriteField8(GSV5K1_VSP_MAP_ADDR(port),ConfigVspRegName[value],0xFF,0,ConfigVspRegValue[value]);
                    value = value + 1;
                }
            }
            /* 5.1.4 Clear Registers for extra supported formats */
            else
            {
                GSV5K1_DSC_get_DSC_VIC_MANUAL_EN(port, &value);
                /* moving from manual setting to auto setting */
                if(value == 1)
                {
                    GSV5K1_DSC_set_DSC_VIC_MANUAL_EN(port, 0);
                    GSV5K1_DSC_set_DSC_INIT_OUT_DELAY_EN(port, 0);
                    GSV5K1_DSC_set_DSC_QP_CLEAR_EN(port, 0);
                    GSV5K1_DSC_set_DSC_SLICE_CTL_MAN_EN(port, 0);
                    GSV5K1_DSC_set_DSC_SLICE_MULTIPLEX_DELAY_EN(port, 0);
                    GSV5K1_DSC_set_DSC_DECODER_LAST_LINE_REP_EN(port, 0);
                    GSV5K1_DSC_set_DSC_SEQ_OUTPUT_MAN_EN(port, 0);
                    GSV5K1_DSC_set_DSC_RC_CTL_MAN_EN(port, 0);
                    GSV5K1_DSC_set_DSC_ENCODER_DELAY_EN(port, 0);
                }
            }
        }
    }
}  /* TrxAvUpStreamFlag == 0 */

    /* 5.4 Tx Output Stability Check */
    if(port->content.dsc->DscInCompressed == 1)
    {
        CurrentPort = NULL;
        while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
        {
            /* 5.4.1 Stability check is only for Color block is not ocuupied case */
            /* when DSC decrypted data is fed to CP, it does not to check */
            if(CurrentPort->type == HdmiTx) /* not CP, so CP must be free */
            {
#if (Gsv5k1RomFromOtp == 1)
                /* 5.4.2 check CP setting, should use manual */
                GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, &value);
                if(value == 1)
                    Gsv5k1EnableDscOutMonitor(port);
                /* 5.4.3 check output stability */
                else if(CurrentPort->content.tx->Lock.PllLock == 1)
                {
                    AvHalI2cReadField8(GSV5K1_VSP_MAP_ADDR(port),0x90,0xFF,0,&value);
                    if(port->content.dsc->OutDelay == 0)
                        port->content.dsc->OutDelay = value;
                    else if(value != port->content.dsc->OutDelay)
                    {
                        GSV5K1_SEC_set_TX1_SRC_SEL(port, 0); /* reset route */
                        GSV5K1_SEC_set_TX1_SRC_SEL(port, 3); /* only possible route */
                        port->content.dsc->OutDelay = value;
                        AvUapiOutputDebugMessage("Recover DSC Decompression Output Stability %x", value);
                        GSV5K1_PLL_set_TXA_PLL_LOCK_CLEAR(port, 1);
                        GSV5K1_PLL_set_TXA_PLL_LOCK_CLEAR(port, 0);
                    }
                }
#else
                /* 5.5.1 DSC MPR1 OutDelay Fix */
                if(CurrentPort->content.tx->Lock.PllLock == 1)
                {
                    if(port->content.dsc->OutDelay < TxVideoManageThreshold)
                    {
                        port->content.dsc->OutDelay = port->content.dsc->OutDelay + 1;
                        GSV5K1_DSC_set_DSC_MEM_EMAB(port, 3);/* enable DSC output reset */
                    }
                    else
                    {
                        GSV5K1_DSC_get_DSC_MEM_EMAB(port, &value);
                        if(value != 2)
                            GSV5K1_DSC_set_DSC_MEM_EMAB(port, 2);/* disable DSC output reset */
                    }
                }
#endif
            }
            PrevPort = CurrentPort;
            CurrentPort = NULL;
        }
    }
    /* 6. Check Pixel Repetition and Path Enable */
    CurrentPort = NULL;
    while(KfunFindVideoNextTxEnd(port, &PrevPort, &CurrentPort) == AvOk)
    {
        if(CurrentPort->type == HdmiTx)
        {
            /* 6.1 Pixel Repetition */
            AvHalI2cReadField8(GSV5K1_TXDIG_MAP_ADDR(CurrentPort),0x23,0xFF,0,&value);
            if(value != 0x80)
                AvHalI2cWriteField8(GSV5K1_TXDIG_MAP_ADDR(CurrentPort),0x23,0xFF,0,0x80);
        }
        PrevPort = CurrentPort;
        CurrentPort = NULL;
    }

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckVideoScaler(pin AvPort *port))
{
    AvPort *FromPort = (AvPort*)port->content.RouteVideoFromPort;
    uint8 value1 = 0;
    uint8 NewValue1 = 0;
    uint8 value2 = 0;
    uint8 NewValue2 = 0;
    uint8 InVic = 0;
    uint8 OutVic = 0;
    AvVideoCs InCs  = AV_CS_YUV_709;
    AvVideoCs OutCs = AV_CS_YUV_709;
    uint32 temp = 0;
    uchar *RegTable;
    uint8 CurrentCscTable[24];

    /* 0. Check Core Connection */
    if((port != FindCp1Mode(port)) ||
       (port->content.RouteVideoToPort == NULL))
    {
        GSV5K1_SEC_get_CP_MODE(port,&value1);
        if((value1 == 2) || (value1 == 3))
            GSV5K1_SEC_set_CP_MODE(port,4);
        return AvOk;
    }
    /* 1.1 Check Work Status */
#if (Gsv5k1RomFromOtp == 1)
    GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, &value1);
    if(value1 == 0)
        Gsv5k1DisableDscOutMonitor(port);
#else
    Gsv5k1CheckDsc420InputSetting(port);
#endif
    /* 2. Vic Mapping */
    switch(port->content.scaler->ScalerInVic)
    {
        case 93: /* 4K@24Hz */
        case 103:
            InVic = 93;
            OutVic = 32;
            break;
        case 114: /* 4K@48Hz */
        case 116:
            InVic = 93;
            OutVic = 111;
            break;
        case 98: /* 4K SMPTE@24Hz */
            InVic = 98;
            OutVic = 32;
            break;
        case 115: /* 4K@SMPTE@48Hz */
            InVic = 98;
            OutVic = 111;
            break;
        case 97:
        case 107: /* 4K@60Hz */
            InVic = 97;
            OutVic = 16;
            break;
        case 118:
        case 120: /* 4K@120Hz */
            InVic = 97;
            OutVic = 63;
            break;
        case 95: /* 4K@30Hz */
        case 105:
            InVic = 95;
            OutVic = 34;
            break;
        case 100: /* 4K SMPTE@30Hz */
            InVic = 100;
            OutVic = 34;
            break;
        case 102: /* 4096x2160@60Hz*/
            InVic = 102;
            OutVic = 16;
            break;
        case 219: /* 4096x2160@120Hz*/
            InVic = 102;
            OutVic = 63;
            break;
        case 94: /* 4K@25Hz */
        case 104:
            InVic = 94;
            OutVic = 33;
            break;
        case 96:
        case 106: /* 4K@50Hz */
            InVic = 96;
            OutVic = 31;
            break;
        case 117:
        case 119: /* 4K@100Hz */
            InVic = 96;
            OutVic = 64;
            break;
        case 99: /* 4K SMPTE@25Hz */
            InVic = 99;
            OutVic = 33;
            break;
        case 101: /* 4096x2160@50Hz*/
            InVic = 101;
            OutVic = 31;
            break;
        case 218: /* 4096x2160@100Hz*/
            InVic = 101;
            OutVic = 64;
            break;
        default:
            InVic = 0;
            OutVic = 0;
            break;
    }
    /* 3. Check Vid In */
    GSV5K1_VSP_get_CP1_VID_IN(port, &value1);
    if(value1 != InVic)
    {
        port->content.scaler->ScalerOutVic = OutVic;
        GSV5K1_VSP_set_CP1_VID_IN(port, InVic);
    }
    /* 3.1 Check Manual Setting */
    GSV5K1_VSP_get_SCAL1_VIN_PARAM_MAN_EN(port, &value1);
    if((FromPort->type == HdmiRx) && (InVic == 0) &&
       (FromPort->content.rx->IsFreeRun == 1))
        value2 = 1;
    else
        value2 = 0;
    if(value1 != value2)
    {
        GSV5K1_VSP_set_SCAL1_VIN_PARAM_MAN_EN(port, value2);
        GSV5K1_VSP_set_SCAL1_VOUT_PARAM_SEL(port, value2);
    }
    /* 3.2 Manual Timing Setting */
#if AvEnableDetailTiming
    if((FromPort->type == HdmiRx) && (FromPort->core.HdmiCore != -1) &&
       (FromPort->content.rx->IsFreeRun == 1) && (OutVic == 0))
    {
        if((FromPort->content.video->timing.HActive != port->content.scaler->timing.HActive) ||
           (FromPort->content.video->timing.VActive != port->content.scaler->timing.VActive) ||
           (FromPort->content.video->timing.HTotal  != port->content.scaler->timing.HTotal) ||
           (FromPort->content.video->timing.VTotal  != port->content.scaler->timing.VTotal) ||
           (FromPort->content.video->timing.HSync   != port->content.scaler->timing.HSync) ||
           (FromPort->content.video->timing.VSync   != port->content.scaler->timing.VSync) ||
           (FromPort->content.video->timing.HBack   != port->content.scaler->timing.HBack) ||
           (FromPort->content.video->timing.VBack   != port->content.scaler->timing.VBack))
        {
            Gsv5k1ManualCpParameter(port);
            AvMemcpy((void*)(&port->content.scaler->timing), (void*)(&FromPort->content.video->timing), sizeof(VideoTiming));
        }
    }
#endif
    /* 4. Check Function Enable State */
    GSV5K1_SEC_get_CP_MODE(port,&value1);
    switch(port->content.scaler->ScalerInSpace)
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
        GSV5K1_SEC_set_CP_MODE(port, NewValue1);
    }
    /* 5. Set Function Mode */
    GSV5K1_VSP_get_CP1_VOUT_CHN_SWAP(port, &value1);
    GSV5K1_VSP_get_CP1_VIN_CHN_SWAP(port, &value2);
    switch(port->content.scaler->ScalerInSpace)
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
        GSV5K1_VSP_set_CP1_VOUT_CHN_SWAP(port, NewValue1);
        GSV5K1_VSP_set_CP1_VIN_CHN_SWAP(port,  NewValue2);
    }
    /* 6. Disable CSC anyway in Scaler mode */
    GSV5K1_SEC_get_CP_CSC_BYPASS(port, &value1);
    if(port->content.scaler->ScalerInSpace == AV_Y2Y1Y0_RGB)
        NewValue1 = 0;
    else
        NewValue1 = 1;
    if(value1 != NewValue1)
        GSV5K1_SEC_set_CP_CSC_BYPASS(port, NewValue1);
    if(NewValue1 == 0)
    {
        /* 7. Set CSC Value */
        InCs  = Gsv5k1ColorCsMapping(port->content.scaler->ScalerInCs);
        OutCs = Gsv5k1ColorCsMapping(port->content.scaler->ScalerOutCs);
        /* 7.1 Find Correct Table */
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
                GSV5K1_SEC_set_CP_CSC_BYPASS(port, 1);
        }
        else
        {
            /* 7.2 Write Table */
            AvHalI2cReadMultiField(GSV5K1_VSP_MAP_ADDR(port), 0x20, 24, CurrentCscTable);
            NewValue1 = 0;
            for(temp=1;temp<24;temp++)
            {
                if(CurrentCscTable[temp] != RegTable[temp])
                {
                    NewValue1 = 1;
                    break;
                }
            }
            if(NewValue1 == 1)
            {
                AvHalI2cWriteMultiField(GSV5K1_VSP_MAP_ADDR(port), 0x20, 24, RegTable);
                value1 = (RegTable[0]>>5)&0x03;
                GSV5K1_VSP_set_CP1_CSC_MODE(port, value1);
            }
        }
    }

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckVideoColor(pin AvPort *port))
{
    uint8 value1 = 0;
    uint8 value2 = 0;
    uint8 NewValue1 = 0;
    uint8 NewValue2 = 0;
    AvVideoCs InCs  = AV_CS_YUV_709;
    AvVideoCs OutCs = AV_CS_YUV_709;
    uint32 temp = 0;
    uchar *RegTable;
    uint8 CurrentCscTable[24];
    AvPort *ToPort = NULL;
    AvPort *FromPort = NULL;
    uint8 Dsc420CompressMode = 0;

    if((port != FindCp1Mode(port)) ||
       (port->content.RouteVideoToPort == NULL))
    {
        GSV5K1_SEC_get_CP_MODE(port,&value1);
        if((value1 == 0) || (value1 == 1))
        {
            GSV5K1_SEC_set_CP_MODE(port,4);
            Gsv5k1ResetVsp(port);
        }
        return AvOk;
    }
    /* 0. Check Work Status */
#if (Gsv5k1RomFromOtp == 1)
    GSV5K1_PRIM_get_ONLY_USE_RX1_EN(port, &value1);
    if(value1 == 0)
        Gsv5k1DisableDscOutMonitor(port);
#else
    Gsv5k1CheckDsc420InputSetting(port);
#endif
    /* 1. Check Vic */
    GSV5K1_VSP_get_CP1_VID_IN(port, &value1);
    if(value1 != port->content.color->ColorInVic)
        GSV5K1_VSP_set_CP1_VID_IN(port, port->content.color->ColorInVic);
    /* 2. Check Input Color Space, Convert to YUV anyway */
    GSV5K1_SEC_get_CP_CSC_BYPASS(port, &value1);
    if((port->content.color->ColorInSpace == AV_Y2Y1Y0_RGB) &&
       (port->content.color->ColorOutSpace != AV_Y2Y1Y0_RGB))
        NewValue1 = 0;
    else
        NewValue1 = 1;
    if(value1 != NewValue1)
        GSV5K1_SEC_set_CP_CSC_BYPASS(port, NewValue1);
    if(NewValue1 == 0)
    {
        /* 3. Set CSC Value */
        InCs  = Gsv5k1ColorCsMapping(port->content.color->ColorInCs);
        OutCs = Gsv5k1ColorCsMapping(port->content.color->ColorOutCs);
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
#if AvEnableInternalVideoGen
        FromPort = (AvPort*)(port->content.RouteVideoFromPort);
        if(FromPort->type == VideoGen)
            RegTable = (uchar *)CscRgbFRtoYcc709FR;
#endif
        if(CscTables[temp].OutCs == AV_CS_AUTO)
        {
            if(value1 == 0)
                GSV5K1_SEC_set_CP_CSC_BYPASS(port, 1);
        }
        else
        {
            /* 3.2 Write Table */
            AvHalI2cReadMultiField(GSV5K1_VSP_MAP_ADDR(port), 0x20, 24, CurrentCscTable);
            NewValue1 = 0;
            for(temp=1;temp<24;temp++)
            {
                if(CurrentCscTable[temp] != RegTable[temp])
                {
                    NewValue1 = 1;
                    break;
                }
            }
            if(NewValue1 == 1)
            {
                AvHalI2cWriteMultiField(GSV5K1_VSP_MAP_ADDR(port), 0x20, 24, RegTable);
                value1 = (RegTable[0]>>5)&0x03;
                GSV5K1_VSP_set_CP1_CSC_MODE(port, value1);
            }
        }
    }
    /* 4. Check Function Enable State */
    GSV5K1_SEC_get_CP_MODE(port,&value1);
    if((port->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
       (port->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420))
        NewValue1 = 1; /* 444 to 420 */
    else if((port->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420) &&
            (port->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420))
        NewValue1 = 0; /* 420 to 444 */
    else
        NewValue1 = 4; /* bypass */
#if (Gsv5k1RomFromOtp == 1)
    /* 4.1 420 10-bit/12b-bit Fix, directly feed to VP */
    if(TrxAvUpStreamFlag == 1)
    {
        FromPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));
        if(FromPort != NULL)
        {
            /* 4.2.1 4K deep color mode found, no div2 */
            if(((port->content.color->ColorInVic >= 93) && (port->content.dsc->DscInVic < 128)) &&
               (port->content.color->ColorInSpace == AV_Y2Y1Y0_YCBCR_420) &&
               ((FromPort->content.video->Cd == AV_CD_30) || (FromPort->content.video->Cd == AV_CD_36)))
                Dsc420CompressMode = 1;
        }
    }
    if(TrxAvUpStreamFlag == 0)
    {
        FromPort = (AvPort*)(Gsv5k1FindDscFront(port));
        if(FromPort != NULL)
        {
            /* 4.2.1 4K deep color mode found, no div2 */
            if(((FromPort->content.dsc->DscInVic >= 93) && (FromPort->content.dsc->DscInVic < 128)) &&
               (port->content.color->ColorInSpace == AV_Y2Y1Y0_YCBCR_420) &&
               ((FromPort->content.dsc->SrcInCd == AV_CD_30) || (FromPort->content.dsc->SrcInCd == AV_CD_36)))
                Dsc420CompressMode = 1;
        }
    }
    if(Dsc420CompressMode == 1)
        NewValue1 = 4; /* bypass */
#endif
    /* 4.2 CP Mode Action */
    if(value1 != NewValue1)
    {
        GSV5K1_SEC_set_CP_MODE(port, NewValue1);
        Gsv5k1ResetVsp(port);
    }
    /* 5. Set Function Mode */
    GSV5K1_VSP_get_CP1_VOUT_CHN_SWAP(port, &value1);
    GSV5K1_VSP_get_CP1_VIN_CHN_SWAP(port, &value2);

    /* Color Port is occupied by DSC as front processing */
    if(TrxAvUpStreamFlag == 1)
    {
        ToPort = (AvPort*)(Gsv5k1FindDscPort(port));
        /* Channel Swap of DSC Mode */
        if((ToPort->content.RouteVideoFromPort == (struct AvPort*)(port)) &&
           (ToPort->content.RouteVideoToPort != NULL))
        {
            switch(port->content.color->ColorInSpace)
            {
                case AV_Y2Y1Y0_YCBCR_420:
                    NewValue1 = 1;
                    NewValue2 = 0;
                    break;
                case AV_Y2Y1Y0_YCBCR_444:
                case AV_Y2Y1Y0_YCBCR_422:
                    NewValue1 = 0;
                    NewValue2 = 2;
                    break;
                case AV_Y2Y1Y0_RGB:
                    NewValue1 = 0;
                    NewValue2 = 0;
                    break;
            }
        }
        /* Channel Swap of Normal Hdmi Mode */
        else
        {
            NewValue1 = 0;
            NewValue2 = 0;
            if((port->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
               (port->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420))
                NewValue2 = 3; /* 444 to 420 */
            else if((port->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420) &&
                    (port->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420))
                NewValue1 = 4; /* 420 to 444 */
        }
    } /* TrxAvUpStreamFlag == 1 */
    if(TrxAvUpStreamFlag == 0)
    {
        FromPort = (AvPort*)(Gsv5k1FindDscFront(port));
        /* Channel Swap of DSC Mode */
        if((FromPort != NULL) &&
           (FromPort->content.RouteVideoToPort == (struct AvPort*)(port)))
        {
            switch(port->content.color->ColorOutSpace)
            {
                case AV_Y2Y1Y0_YCBCR_420:
                    NewValue1 = 0;
                    if(FromPort->content.dsc->SrcInY == AV_Y2Y1Y0_RGB)
                        NewValue2 = 3;
                    else
                        NewValue2 = 1;
                    break;
                case AV_Y2Y1Y0_YCBCR_444:
                case AV_Y2Y1Y0_YCBCR_422:
                    /* use TX CSC to do channel swap */
                    NewValue1 = 0;
                    NewValue2 = 0;
                    break;
                case AV_Y2Y1Y0_RGB:
                    NewValue1 = 0;
                    NewValue2 = 0;
                    break;
            }

        }
        /* Channel Swap of Normal Hdmi Mode */
        else
        {
            NewValue1 = 0;
            NewValue2 = 0;
            if((port->content.color->ColorOutSpace == AV_Y2Y1Y0_YCBCR_420) &&
               (port->content.color->ColorInSpace  != AV_Y2Y1Y0_YCBCR_420))
                NewValue2 = 3; /* 444 to 420 */
            else if((port->content.color->ColorOutSpace != AV_Y2Y1Y0_YCBCR_420) &&
                    (port->content.color->ColorInSpace  == AV_Y2Y1Y0_YCBCR_420))
                NewValue1 = 4; /* 420 to 444 */
        }
    } /* TrxAvUpStreamFlag == 0 */
    /* 420 compress mode, no channel swap */
    if(Dsc420CompressMode == 1)
    {
        NewValue1 = 0;
        NewValue2 = 0;
    }
    if((value1 != NewValue1) || (value2 != NewValue2))
    {
        GSV5K1_VSP_set_CP1_VOUT_CHN_SWAP(port, NewValue1);
        GSV5K1_VSP_set_CP1_VIN_CHN_SWAP(port,  NewValue2);
    }
    /* 5.1 Check FromPort and ToPort to judge pixel processing mode */
    FromPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));
    ToPort = (AvPort*)(Gsv5k1FindDscPort(port));
    GSV5K1_VSP_get_CP1_DITHER_OUT_BW(port, &value1);
#if (AvEnableDitherView == 1)
    if((FromPort != NULL) &&
       (ToPort != NULL) &&
       (FromPort->type == HdmiRx) &&
       (ToPort->type == VideoDsc) && /* Match 1: Rx->CP->VP */
       (ToPort->content.dsc->DscInCompressed == 0) && /* Match 2: HDMI->DSC Compression */
       //((FromPort->content.video->AvailableVideoPackets & AV_BIT_HDR_PACKET) == 0) && /* Match 3.1: no HDR Input */
       ((FromPort->content.video->Y  == AV_Y2Y1Y0_YCBCR_422) ||  /* Match 3.2: 422 Input */
        (FromPort->content.video->Cd == AV_CD_30) ||
        (FromPort->content.video->Cd == AV_CD_36)))   /* Match 3.3: Deep Color Mode */
    {
        NewValue1 = 1;    /* Dither enabled */
        NewValue2 = 0x00; /* Dither enabled */
    }
    else
#endif
    {
        NewValue1 = 0;    /* Dither disabled */
        NewValue2 = 0x2A; /* Dither disabled */
    }
    if(value1 != NewValue1)
    {
        GSV5K1_VSP_set_CP1_DITHER_OUT_BW(port, NewValue1);
        GSV5K1_VSP_set_CP1_DITHER_MODE(port, NewValue2);
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
    while(Gsv5k1VideoColorAutoVicTable[temp] != 0xff)
    {
        /* 6.1 Found Auto Conversion Vic */
        if(port->content.color->ColorInVic == Gsv5k1VideoColorAutoVicTable[temp])
        {
            NewValue1 = 0;
            break;
        }
        temp = temp + 1;
    }
    GSV5K1_VSP_get_CP1_VIN_PARAM_MAN_EN(port, &value1);
    if(value1 != NewValue1)
        GSV5K1_VSP_set_CP1_VIN_PARAM_MAN_EN(port, NewValue1);
    /* 6.2 Use Manual Conversion Vic */
#if AvEnableDetailTiming
    FromPort = (AvPort*)(port->content.RouteVideoFromPort);
    if((FromPort->type == HdmiRx) && (FromPort->core.HdmiCore != -1) &&
       (FromPort->content.rx->IsFreeRun == 1) && (NewValue1 == 1))
    {
        if((FromPort->content.video->timing.HActive != port->content.color->timing.HActive) ||
           (FromPort->content.video->timing.VActive != port->content.color->timing.VActive) ||
           (FromPort->content.video->timing.HTotal  != port->content.color->timing.HTotal) ||
           (FromPort->content.video->timing.VTotal  != port->content.color->timing.VTotal) ||
           (FromPort->content.video->timing.HSync   != port->content.color->timing.HSync) ||
           (FromPort->content.video->timing.VSync   != port->content.color->timing.VSync) ||
           (FromPort->content.video->timing.HBack   != port->content.color->timing.HBack) ||
           (FromPort->content.video->timing.VBack   != port->content.color->timing.VBack))
        {
            Gsv5k1ManualCpParameter(port);
            AvMemcpy((void*)(&port->content.color->timing), (void*)(&FromPort->content.video->timing), sizeof(VideoTiming));
        }
    }
#endif

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckVideoGen(pin AvPort *port))
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
        GSV5K1_RXDIG_get_VPLL_MANUAL_MODE(port, &Value);
        if(Value != NewValue)
            GSV5K1_RXDIG_set_VPLL_MANUAL_MODE(port, NewValue);
    }
    GSV5K1_VSP_get_DG_EN(port, &Value);
    if(Value != NewValue)
    {
        GSV5K1_VSP_set_DG_EN(port, NewValue);
        if(NewValue == 0)
            GSV5K1_VSP_set_DG_VID(port, 0);
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
                GSV5K1_VSP_get_DG_VID(port, &Value);
                if(Value != NewValue)
                {
                    GSV5K1_VSP_set_DG_VID(port, NewValue);
                    if(port->core.HdmiCore == 0)
                    {
                        NewValue = VideoGenVicTable[i+2];
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x5D,0xFF,0,NewValue);
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x5E,0xFF,0,VideoGenVicTable[i+3]);
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x5F,0xFF,0,VideoGenVicTable[i+4]);
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x60,0xFF,0,VideoGenVicTable[i+5]);
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x61,0xFF,0,VideoGenVicTable[i+6]);
                        Value = NewValue | 0x20;
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x5D,0xFF,0,Value);
                        AvHalI2cWriteField8(GSV5K1_RXDIG_MAP_ADDR(port),0x5D,0xFF,0,NewValue);
                    }
                }
                break;
            }
            i = i+7;
        }
        /* 3. Check Pattern */
        GSV5K1_VSP_get_DG_PATTERN(port, &Value);
        if(Value != port->content.vg->Pattern)
            GSV5K1_VSP_set_DG_PATTERN(port, port->content.vg->Pattern);
        /* 4. Check 420 */
        GSV5K1_VSP_get_DG_420_EN(port, &Value);
        NewValue = 0;
        if(Value != NewValue)
            GSV5K1_VSP_set_DG_420_EN(port, NewValue);
    }

#endif
    return AvOk;
}

#if (AvEnableCcaFeature == 1) /* CCA Related */

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaTxInit(pin AvPort *port))
{
    uint32 DefaultMapAddress = GSV5K1_PRIM_MAP_ADDR(port);
    uint8 k = 0;

    for(k=0; Gsv5k1CcaTxInitTable[k]!=0xFF; k=k+3)
        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1CcaTxInitTable[k],Gsv5k1CcaTxInitTable[k+1],0xFF,0,Gsv5k1CcaTxInitTable[k+2]);

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaRxInit(pin AvPort *port))
{
    uint32 DefaultMapAddress = GSV5K1_PRIM_MAP_ADDR(port);
    uint8 k = 0;

    for(k=0; Gsv5k1CcaRxInitTable[k]!=0xFF; k=k+3)
        AvHalI2cWriteField8(DefaultMapAddress | Gsv5k1CcaRxInitTable[k],Gsv5k1CcaRxInitTable[k+1],0xFF,0,Gsv5k1CcaRxInitTable[k+2]);

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaTxGetInfo(pin AvPort *port))
{
    uint8  value = 0;
    uint8  EdidCks[2] = {0x00,0x00};
    uint32 LinkErrorCount = 0;
    uint32 LinkErrorSample1 = 0;
    uint32 LinkErrorSample2 = 0;
    uint8  SrcFsmRestart = 0;
    AvPort *FromPort = NULL;
    FromPort = (AvPort*)(Gsv5k1FindHdmiRxFront(port));

    /* default value */
    port->content.ccatx->LinkAbort = 1- port->content.tx->Lock.PllLock;
    /* 1. tmds data lane check */
    GSV5K1_CCA_get_SRC_START_CALIBRATE_SWING(port, &value);
    if((value == 1) && (port->content.tx->Lock.PllLock == 0))
        GSV5K1_CCA_set_SRC_START_CALIBRATE_SWING(port, 0);
    else if((value == 0) && (port->content.tx->Lock.PllLock == 1))
        SrcFsmRestart = 1;
    /* 2. end calibrate swing */
    GSV5K1_CCA_get_RB_SRC_END_CALIBRATE_SWING(port, &port->content.ccatx->CalibDone);
    //if(port->content.ccatx->CalibDone == 0)
    //    AvUapiOutputDebugFsm("CCA Src Calibration is not Done");
    /* 3. Clock Lock */
    GSV5K1_CCA_get_RB_SRC_CCA_TX_PLL_LOCK(port, &value);
    if((value == 1) && (port->content.tx->Lock.PllLock == 1))
    {
        GSV5K1_CCA_get_RB_SRC_PRBS_PHASE_LOCKED(port, &value);
        if(value == 1)
        {
            if((FromPort->type == VideoGen) ||
               (((FromPort->type == HdmiRx) || (FromPort->type == CcaRx)) &&
                (FromPort->content.rx->IsInputStable == 1)))
            {
                if(port->content.ccatx->Lock != AV_CCA_VIDEO_LOCK)
                {
                    GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
                    GSV5K1_TXPHY_set_TX_EDID_TRY_TIMES(port,15);
                }
                port->content.ccatx->Lock = AV_CCA_VIDEO_LOCK;
            }
            else
                port->content.ccatx->Lock = AV_CCA_LINK_LOCK;
        }
        else
        {
            if((port->content.ccatx->Lock == AV_CCA_VIDEO_LOCK) ||
               (port->content.ccatx->Lock == AV_CCA_LINK_LOCK))
            {
                GSV5K1_CCA_set_SRC_FSM_RESET(port, 1);
                GSV5K1_CCA_set_SRC_FSM_RESET(port, 0);
            }
            port->content.ccatx->Lock = AV_CCA_OUT_LOCK;
        }
    }
    else
        port->content.ccatx->Lock = AV_CCA_NO_LOCK;
    /* 4. get downstream edid status */
    if((port->content.ccatx->Lock == AV_CCA_LINK_LOCK) ||
       (port->content.ccatx->Lock == AV_CCA_VIDEO_LOCK))
    {
        /* 4.1 monitor link status */
        GSV5K1_CCA_get_RB_SRC_PRBS_ERROR_CNT(port, &LinkErrorSample1);
        GSV5K1_CCA_get_RB_SRC_PRBS_ERROR_CNT(port, &LinkErrorSample2);
        LinkErrorCount = LinkErrorSample2 - LinkErrorSample1;
        if(LinkErrorCount > 0x400)
        {
            SrcFsmRestart = 1;
        }
        else
        /* 4.2 check edid */
        {
            GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
            GSV5K1_TXPHY_get_TX_HDCP1P4_STATE_RB(port,&value);
            if(value != 1)
            {
                EdidCks[0] = Gsv5k1TxScdcAction(port, 0, 0xA8, 0xDE, 0x00);
                EdidCks[1] = Gsv5k1TxScdcAction(port, 0, 0xA8, 0xDF, 0x00);
                if(((port->content.tx->EdidCks[0] != EdidCks[0]) ||
                    (port->content.tx->EdidCks[1] != EdidCks[1])) &&
                   ((EdidCks[0] != 0x00) || (EdidCks[1] != 0x00)))
                    value = 1;
                else
                    value = 0;
                /* 4.2.1 get new downstream EDID */
                if(value == 1)
                {
                    /* reread the Edid, clear ddc errors */
                    GSV5K1_TXPHY_set_TX_HDCP1P4_CLR_ERR(port, 1);
                    GSV5K1_TXPHY_set_TX_EDID_TRY_TIMES(port,15);
                    GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 0);
                    GSV5K1_TXPHY_set_TX_EDID_RE_READ(port, 1);
                    port->content.ccatx->CcaEdidStatus = AV_EDID_SUCCESS;
                }
            }
        }
    }
    /* 5. Restart CCA Source FSM */
    if(SrcFsmRestart == 1)
    {
        GSV5K1_CCA_set_SRC_START_CALIBRATE_SWING(port, 0);
        GSV5K1_SEC_set_CCA_TX_RST(port, 1);
        GSV5K1_SEC_set_CCA_TX_RST(port, 0);
        GSV5K1_CCA_set_SRC_DDC_ENABLE(port, 0);
        GSV5K1_CCA_set_SRC_DDC_ENABLE(port, 1);
        GSV5K1_CCA_set_SRC_FSM_RESET(port, 1);
        GSV5K1_CCA_set_SRC_START_CALIBRATE_SWING(port, 1);
        GSV5K1_CCA_set_SRC_FSM_RESET(port, 0);
    }

    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaRxGetInfo(pin AvPort *port))
{
    uint8  value = 0;
    uint32 LinkErrorCount = 0;
    uint32 LinkErrorSample1 = 0;
    uint32 LinkErrorSample2 = 0;
    uint8  SnkFsmRestart = 0;
    //uint8  NewValue = 0;
    /* 1. Check CCA State */
    //GSV5K1_CCA_get_SNK_FSM_RESET(port, &value);
    //NewValue = 1 - port->content.rx->IsInputStable;
    //if(value != NewValue)
    //{
    //    GSV5K1_CCA_set_SNK_MAN_PD_MDRIVER_EN(port, NewValue);
    //    GSV5K1_CCA_set_SNK_MAN_PD_EDRIVER_EN(port, NewValue);
    //    GSV5K1_CCA_set_SNK_FSM_RESET(port, NewValue);
    //}
    /* 2. end calibrate swing */
    GSV5K1_CCA_get_RB_SNK_END_CALIBRATE_SWING(port, &value);
    if((port->content.ccarx->CalibDone == 0) && (value ==1))
    {
        port->content.rx->Lock.EqLock = 0;
        Gsv5k1RxManualEQUpdate(port, 1);
    }
    port->content.ccarx->CalibDone = value;
    /* 3. Clock Lock */
    GSV5K1_CCA_get_RB_SNK_CCA_CDR_LOCK(port, &value);
    if(value == 1)
    {
        GSV5K1_CCA_get_RB_SNK_PRBS_PHASE_LOCKED(port, &value);
        if(value == 1)
        {
            if(port->content.rx->IsInputStable == 1)
                port->content.ccarx->Lock = AV_CCA_VIDEO_LOCK;
            else
                port->content.ccarx->Lock = AV_CCA_LINK_LOCK;
        }
        else
            port->content.ccarx->Lock = AV_CCA_IN_LOCK;
    }
    else
        port->content.ccarx->Lock = AV_CCA_NO_LOCK;
    /* 4. Edid Status Update */
    if((port->content.ccarx->Lock == AV_CCA_LINK_LOCK) ||
       (port->content.ccarx->Lock == AV_CCA_VIDEO_LOCK))
    {
        /* 4.1 monitor link status */
        GSV5K1_CCA_get_RB_SNK_PRBS_ERROR_CNT(port, &LinkErrorSample1);
        GSV5K1_CCA_get_RB_SNK_PRBS_ERROR_CNT(port, &LinkErrorSample2);
        LinkErrorCount = LinkErrorSample2 - LinkErrorSample1;
        if(LinkErrorCount > 0x400)
        {
            SnkFsmRestart = 1;
        }
        /* 4.2 update edid status */
        else
        {
            GSV5K1_RXSCDC_set_SCDC_MFS0(port, port->content.ccarx->EdidCks[0]);
            GSV5K1_RXSCDC_set_SCDC_MFS1(port, port->content.ccarx->EdidCks[1]);
        }
    }
    /* 5. Restart CCA Source FSM */
    if(SnkFsmRestart == 1)
    {
        GSV5K1_CCA_set_SNK_FSM_RESET(port, 1);
        GSV5K1_CCA_set_SNK_FSM_RESET(port, 0);
    }
    /* 6. Print Current Sch FSM */
    /*
    uint8  CcaRxSchFsm[2];
    uint16 CcaFsmPrint;
    AvHalI2cReadMultiField(GSV5K1_CCA_MAP_ADDR(port), 0xEA, 2, &CcaRxSchFsm[0]);
    CcaFsmPrint = (CcaRxSchFsm[1]<<8) + CcaRxSchFsm[0];
    AvUapiOutputDebugFsm("CcaRx = %x",CcaFsmPrint);
    */
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaTxChainReset(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaRxChainReset(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaTxCalibManage(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaRxCalibManage(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaTxSetWorkMode(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaTxEnablePin(pio AvPort *port, uint8 SlotName))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCcaRxEnablePin(pio AvPort *port, uint8 SlotName))
{
    return AvOk;
}


#endif /* CCA Related */

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckAudioGen(pin AvPort *port))
{
    return AvOk;
}

uapi AvRet ImplementUapi(Gsv5k1, AvUapiCheckClockGen(pin AvPort *port))
{
    return AvOk;
}

#undef FindCore1
#undef Rx1VideoInt
#undef Rx2VideoInt
#undef Rx1AudioInt
#undef Rx2AudioInt
#undef Rx1HdcpInt
#undef Rx2HdcpInt
#undef FindCp1Mode

#endif  // endif GSV5K1
#endif

