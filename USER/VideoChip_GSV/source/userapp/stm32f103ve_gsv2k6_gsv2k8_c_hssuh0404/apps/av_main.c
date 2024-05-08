/**
 * @file av_main.c
 *
 * @brief sample main entry for audio/video based software
 */

#include "includes.h"

#if _ENABLE_CHIP_GSV_CHIP_SERIES
#include "av_main.h"
#include "global_var.h"
#include "gsv_uapi.h"

#if GSV2K6
AvDevice AvGsv2k6_devices[AvGsv2k6DeviceNumber];
Gsv2k6Device gsv2k6_device[AvGsv2k6DeviceNumber];
AvPort gsv2k6Ports[AvGsv2k6DeviceNumber][AvGsv2k6PortNumber];
bool g_bGsv2k6HpdForceChange[AvGsv2k6DeviceNumber][2] = {{FALSE,FALSE}};
uint8 g_u8Gsv2k6StartIndex = 0;
bool g_bGsv2k6InitOk[AvGsv2k1DeviceNumber];
#endif

#if GSV2K8
AvDevice AvGsv2k8_devices[AvGsv2k8DeviceNumber];
Gsv2k8Device gsv2k8_device[AvGsv2k8DeviceNumber];
AvPort gsv2k8Ports[AvGsv2k8DeviceNumber][AvGsv2k8PortNumber];
#if AvGsv2k8VideoRgb_422_444To420
bool g_bGsv2k8TxChange[AvGsv2k8DeviceNumber][4] = {{FALSE,FALSE,FALSE,FALSE}};
uint8 g_u8Gsv2k8ForceCscMode[AvGsv2k8DeviceNumber][4] = {{emTxCscMode_ToNone,emTxCscMode_ToNone,emTxCscMode_ToNone,emTxCscMode_ToNone}};
stAvInfoUartSendMessage g_stTxAvInfo[AvGsv2k8DeviceNumber];
#endif
bool g_bGsv2k8HpdForceChange[AvGsv2k8DeviceNumber][4] = {{FALSE,FALSE,FALSE,FALSE}};
uint8 g_u8Gsv2k8StartIndex = 0;
bool g_bGsv2k8InitOk[AvGsv2k1DeviceNumber];
#endif

#if GSV2K2
AvDevice AvGsv2k2_devices[AvGsv2k2DeviceNumber];
Gsv2k2Device gsv2k2_device[AvGsv2k2DeviceNumber];
AvPort gsv2k2Ports[AvGsv2k2DeviceNumber][AvGsv2k2PortNumber];
#if AvGsv2k2Video420ToRgb_422_444
bool g_bGsv2k2InputIsChange[AvGsv2k2DeviceNumber][2] = {{FALSE,FALSE}};
uint8 g_u8Gsv2k2ForceCscMode[AvGsv2k2DeviceNumber][2] = {{emCscMode_ToNone,emCscMode_ToNone}};
stAvInfoUartSendMessage g_stRxAvInfo[AvGsv2k2DeviceNumber];
#endif
bool g_bGsv2k2HpdForceChange[AvGsv2k2DeviceNumber][2] = {{FALSE,FALSE}};
uint8 g_u8Gsv2k2StartIndex = 0;
bool g_bGsv2k2InitOk[AvGsv2k1DeviceNumber];
#endif

#if GSV2K1
AvDevice AvGsv2k1_devices[AvGsv2k1DeviceNumber];
Gsv2k1Device gsv2k1_device[AvGsv2k1DeviceNumber];
AvPort gsv2k1Ports[AvGsv2k1DeviceNumber][AvGsv2k1PortNumber];
bool g_bGsv2k1HpdForceChange[AvGsv2k1DeviceNumber][1] = {{FALSE}};
uint8 g_u8Gsv2k1StartIndex = 0;
bool g_bGsv2k1InitOk[AvGsv2k1DeviceNumber];
#endif

#if GSV5K1
uint8 TrxAvUpStreamFlag = Gsv5k1WorkMode; /* 1 = Tx, 0 = Rx */
//uint8 TrxAvEnableDetailTiming = 1; /* 1 = Tx, 0 = Rx */
AvDevice AvGsv5k1_devices[AvGsv5k1DeviceNumber];
Gsv5k1Device gsv5k1_device[AvGsv5k1DeviceNumber];
AvPort gsv5k1Ports[AvGsv5k1DeviceNumber][AvGsv5k1PortNumber];
bool g_bGsv5k1HpdForceChange[AvGsv5k1DeviceNumber][1] = {{FALSE}};
bool g_bGsv5k1InitOk[AvGsv5k1DeviceNumber];
uint8 g_u8Gsv5k1StartIndex = 0;
#endif

#if GSV2K11
AvDevice AvGsv2k11_devices[AvGsv2k11DeviceNumber];
Gsv2k11Device gsv2k11_device[AvGsv2k11DeviceNumber];
AvPort gsv2k11Ports[AvGsv2k11DeviceNumber][AvGsv2k11PortNumber];
bool g_bGsv2k11HpdForceChange[AvGsv2k11DeviceNumber][1] = {{FALSE}};
bool g_bGsv2k11InitOk[AvGsv2k11DeviceNumber];
extern uint8  LogicOutputSel[AvGsv2k11DeviceNumber];//
bool g_bGsvStandbyFlag[AvGsv2k11DeviceNumber];
bool g_bPreGsvStbFlag[AvGsv2k11DeviceNumber];
stGsvParamChange_T g_stGsv2k11ParamChg[AvGsv2k11DeviceNumber];
uint8 g_u8Gsv2k11StartIndex = 0;
stGsv2K11ColorSpace_T g_stGsv2k11HdmiOutColorSpace[AvGsv2k11DeviceNumber];

const stTimingTable_t res_table[RES_DEFAULT_MAX]=
{
    //  HDST    HTOT    HBP   HFP HSYNC    HPOL    VDST     VTOT  VBP  VFP VSYNC   VPOL   VCLK   u8VIC      PCLK            interlace
      { 1920,   2080,    80,   48,   32,    Hpos ,   1200,  1235,  26,   3,    6,  Vneg,     60,     0, (2080*1235)*60/10000,    PROG},//1920x1200@60Hz
      { 1920,   2200,   148,   88,   44,    Hpos ,   1080,  1125,  36,   4,    5,  Vpos,     60,    16,                 14850,   PROG},//1920x1080@60Hz
      { 1280,   1650,   220,  110,   40,    Hpos ,    720,   750,  20,   5,    5,  Vpos,     60,     4,                  7425,   PROG},//1280x720@60Hz
      { 1360,   1792,   256,   64,  112,    Hpos ,    768,   795,  18,   3,    6,  Vpos,     60,     0,  (1792*795)*60/10000,    PROG},//1360x768@60Hz
      { 1280,   1688,   248,   48,  112,    Hpos ,   1024,  1066,  38,   1,    3,  Vpos,     60,     0, (1688*1066)*60/10000,    PROG},//1280x1024@60Hz
      { 1024,   1344,   160,   24,  136,    Hneg ,    768,   806,  29,   3,    6,  Vneg,     60,     0,  (1344*806)*60/10000,    PROG},//1024x768@60Hz
      { 1600,   2160,   304,   64,  192,    Hpos ,   1200,  1250,  46,   1,    3,  Vpos,     60,     0, (2160*1250)*60/10000,    PROG},//1600x1200@60Hz
      { 1440,   1904,   232,   80,  152,    Hneg ,    900,   934,  25,   3,    6,  Vpos,     60,     0,  (1904*934)*60/10000,    PROG},//1440x900@60Hz
      { 1600,   1800,    96,   24,   80,    Hpos ,    900,  1000,  96,   1,    3,  Vpos,     60,     0, (1800*1000)*60/10000,    PROG},//1600x900@60Hz
      { 1280,   1980,   220,  440,   40,    Hpos ,    720,   750,  20,   5,    5,  Vpos,     50,    19,  (1980*750)*50/10000,    PROG},//1280x720@50Hz
      { 1920,   2640,   148,  528,   44,    Hpos ,   1080,  1125,  36,   4,    5,  Vpos,     50,    31, (2640*1125)*50/10000,    PROG},//1920x1080@50Hz
#if _ENABLE_RES_3840x2160_30
      { 3840,   4400,   296,  176,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     30,    95,                 29700,   PROG},//3840x2160@30Hz
#endif
#if _ENABLE_RES_3840x2160_60
      { 3840,   4400,   296,  176,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     60,    97,                 59400,   PROG},//3840x2160@60Hz
#endif
#if _ENABLE_RES_3840x2160_50
      { 3840,   5280,   298, 1056,   88,    Hpos ,   2160,  2250,  72,   8,   10,  Vpos,     50,    96,                 59400,   PROG},//3840x2160@50Hz
#endif
};
#endif

u8 g_u8ChipDev = 0;
u8 g_u8GsvChipType[GsvTotalDevNumber] = {0};

uint8 g_u8DeviceAddress[]=
{
    #if (AvGsv2k6DeviceNumber>=1)
    Gsv2k6,
    #endif
    #if (AvGsv2k6DeviceNumber>=2)
    Gsv2k6,
    #endif

    #if (AvGsv2k8DeviceNumber>=1)
    Gsv2k8,
    #endif
    #if (AvGsv2k8DeviceNumber>=2)
    Gsv2k8,
    #endif

    #if (AvGsv2k2DeviceNumber>=1)
    Gsv2k2,
    #endif
    #if (AvGsv2k2DeviceNumber>=2)
    Gsv2k2,
    #endif

    #if (AvGsv2k1DeviceNumber>=1)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=2)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=3)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=4)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=5)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=6)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=7)
    Gsv2k1,
    #endif
    #if (AvGsv2k1DeviceNumber>=8)
    Gsv2k1,
    #endif

    #if (AvGsv2k11DeviceNumber>=1)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=2)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=3)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=4)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=5)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=6)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=7)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=8)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=9)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=10)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=11)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=12)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=13)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=14)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=15)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=16)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=17)
    Gsv2k11,
    #endif
    #if (AvGsv2k11DeviceNumber>=18)
    Gsv2k11,
    #endif

    #if (AvGsv5k1DeviceNumber>=1)
    Gsv5k1,
    #endif
    #if (AvGsv5k1DeviceNumber>=2)
    Gsv5k1,
    #endif
    #if (AvGsv5k1DeviceNumber>=3)
    Gsv5k1,
    #endif
    #if (AvGsv5k1DeviceNumber>=4)
    Gsv5k1,
    #endif
};

void Gsv_SystemReset(void)
{
    //u32 i;
    pstGsvChipResetPinCfg_t pstGsvRst,pstGsvRstOrg = (pstGsvChipResetPinCfg_t)&stProjectGsvChipResetTable[0];

    pstGsvRst = pstGsvRstOrg;
    while(pstGsvRst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstGsvRst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstGsvRst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstGsvRst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstGsvRst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstGsvRst->pstGpioCfgTable->u16GpioPin);
        }

        pstGsvRst ++;
    }
    //for(i=0;i<0x3fff;i++);
    delay_ms(10);
    pstGsvRst = pstGsvRstOrg;
    while(pstGsvRst->pstGpioCfgTable != NULL)   // pull low
    {
        if(pstGsvRst->eRstLevel == ePinLevel_Low)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstGsvRst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstGsvRst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstGsvRst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstGsvRst->pstGpioCfgTable->u16GpioPin);
        }

        pstGsvRst ++;
    }
    //for(i=0;i<0x3fff;i++);
    delay_ms(10);
    pstGsvRst = pstGsvRstOrg;
    while(pstGsvRst->pstGpioCfgTable != NULL)   // push high
    {
        if(pstGsvRst->eRstLevel == ePinLevel_High)
        {
            GPIO_ResetBits(STM32_GPIO_PORT[pstGsvRst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstGsvRst->pstGpioCfgTable->u16GpioPin);
        }
        else
        {
            GPIO_SetBits(STM32_GPIO_PORT[pstGsvRst->pstGpioCfgTable->u8PortType-PORT_GPIOA], pstGsvRst->pstGpioCfgTable->u16GpioPin);
        }

        pstGsvRst ++;
    }
    //for(i=0;i<0x7fff;i++);
    delay_ms(20);
}

void Gsv_GlobalVarInit(void)
{
    uint8 i=0;
#if GSV2K6
    for(i=0;i<AvGsv2k6DeviceNumber;i++)
    {
        memset(&AvGsv2k6_devices[i],0,sizeof(AvDevice));
        memset(&gsv2k6_device[i],0,sizeof(Gsv2k6Device));
        memset(&gsv2k6Ports[i][0],0,sizeof(AvPort)*AvGsv2k6PortNumber);
        memset(&g_bGsv2k6HpdForceChange[i][0],FALSE,sizeof(bool)*2);
    }
#endif

#if GSV2K8
    for(i=0;i<AvGsv2k8DeviceNumber;i++)
    {
        memset(&AvGsv2k8_devices[i],0,sizeof(AvDevice));
        memset(&gsv2k8_device[i],0,sizeof(Gsv2k8Device));
        memset(&gsv2k8Ports[i][0],0,sizeof(AvPort)*AvGsv2k8PortNumber);
        #if AvGsv2k8VideoRgb_422_444To420
        memset(&g_bGsv2k8TxChange[i][0],FALSE,sizeof(bool)*4);
        memset(&g_u8Gsv2k8ForceCscMode[i][0],emTxCscMode_ToNone,sizeof(uint8)*4);
        memset(&g_stTxAvInfo[i],0,sizeof(stAvInfoUartSendMessage));
        #endif
        memset(&g_bGsv2k8HpdForceChange[i][0],FALSE,sizeof(bool)*4);
    }
#endif

#if GSV2K2
    for(i=0;i<AvGsv2k2DeviceNumber;i++)
    {
        memset(&AvGsv2k2_devices[i],0,sizeof(AvDevice));
        memset(&gsv2k2_device[i],0,sizeof(Gsv2k2Device));
        memset(&gsv2k2Ports[i][0],0,sizeof(AvPort)*AvGsv2k2PortNumber);
        #if AvGsv2k2Video420ToRgb_422_444
        memset(&g_bGsv2k2InputIsChange[i][0],FALSE,sizeof(bool)*2);
        memset(&g_u8Gsv2k2ForceCscMode[i][0],emCscMode_ToNone,sizeof(uint8)*2);
        memset(&g_stRxAvInfo[i],0,sizeof(stAvInfoUartSendMessage));
        #endif
        memset(&g_bGsv2k2HpdForceChange[i][0],FALSE,sizeof(bool)*2);
    }
#endif
#if GSV2K1
    for(i=0;i<AvGsv2k1DeviceNumber;i++)
    {
        memset(&AvGsv2k1_devices[i],0,sizeof(AvDevice));
        memset(&gsv2k1_device[i],0,sizeof(Gsv2k1Device));
        memset(&gsv2k1Ports[i][0],0,sizeof(AvPort)*AvGsv2k1PortNumber);
        memset(&g_bGsv2k1HpdForceChange[i][0],FALSE,sizeof(bool));
    }
#endif
#if GSV2K11
    for(i=0;i<AvGsv2k11DeviceNumber;i++)
    {
        memset(&AvGsv2k11_devices[i],0,sizeof(AvDevice));
        memset(&gsv2k11_device[i],0,sizeof(Gsv2k11Device));
        memset(&gsv2k11Ports[i][0],0,sizeof(AvPort)*AvGsv2k11PortNumber);
        memset(&g_bGsv2k11HpdForceChange[i][0],FALSE,sizeof(bool));
    }
#endif
#if GSV5K1
    for(i=0;i<AvGsv5k1DeviceNumber;i++)
    {
        memset(&AvGsv5k1_devices[i],0,sizeof(AvDevice));
        memset(&gsv5k1_device[i],0,sizeof(Gsv5k1Device));
        memset(&gsv5k1Ports[i][0],0,sizeof(AvPort)*AvGsv5k1PortNumber);
        memset(&g_bGsv5k1HpdForceChange[i][0],FALSE,sizeof(bool));
    }
#endif

}

#if GSV5K1
void Gsv5k1TxInit(u8 u8Step)
{
    u8 i=0;

    switch(u8Step)
    {
        case 1: // add device
            {
                /* 2.4 connect devices to device declaration */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    AvApiAddDevice(&AvGsv5k1_devices[i], Gsv5k1,
                                   i+AvGsv2k6DeviceNumber+AvGsv2k8DeviceNumber+AvGsv2k2DeviceNumber+AvGsv2k1DeviceNumber+AvGsv2k11DeviceNumber,
                                   (void *)&gsv5k1_device[i], (void *)&gsv5k1Ports[i][0], NULL);
                }
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    #if Gsv5k1TxUseRxBHdmiMode
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][0] ,1 ,HdmiRx);
                    #else
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][0] ,0 ,HdmiRx);
                    #endif
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][1] ,4 ,HdmiTx);
                    #if (AvEnableCcaFeature == 1)
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][2] ,5 ,CcaTx);
                    #else
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][2] ,5 ,HdmiTx);
                    #endif
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][3] ,8 ,LogicAudioTx);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][4] ,16,VideoDsc);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][5] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][6] ,24,VideoColor);
                    #if AvEnableInternalVideoGen_5K1
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][7] ,28,VideoGen);
                    #endif

                /* 3.2 initialize port content */
                    #if (AvEnableCcaFeature == 1)
                    if(gsv5k1Ports[i][2].type == CcaTx)
                        gsv5k1Ports[i][2].content.ccatx->Enable = 1;
                    #endif
                }
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv5k1_devices[i])==AvOk)
                    {
                        g_bGsv5k1InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv5k1InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {
                /* 3.4 routing */
                /* connect the port by video using AvConnectVideo */
                /* connect the port by audio using AvConnectAudio */
                /* connect the port by video and audio using AvConnectAV */

                /* 3.4.1 video routing */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    #if AvEnableInternalVideoGen_5K1
                    AvApiConnectPort(&gsv5k1Ports[i][7], &gsv5k1Ports[i][1], AvConnectAV);
                    AvApiConnectPort(&gsv5k1Ports[i][7], &gsv5k1Ports[i][2], AvConnectAV);
                    #else
                    #if (Gsv5k1DisableTxAHdmi==0)
                    AvApiConnectPort(&gsv5k1Ports[i][0], &gsv5k1Ports[i][1], AvConnectAV);
                    #endif
                    AvApiConnectPort(&gsv5k1Ports[i][0], &gsv5k1Ports[i][2], AvConnectAV);
                    #endif

                    /* 3.4.2 audio routing */
                    /* AvApiConnectPort(&gsv5k1Ports[0], &gsv5k1Ports[3], AvConnectAudio); */
                    /* 3.4.2 ARC Connection, set after rx port connection to avoid conflict */

                    /* 3.4.3 gpio routing */
                    #if (AvEnableCcaFeature == 1)
                    if(gsv5k1Ports[i][2].type == CcaTx)
                        KfunCcaTxEnablePin(&gsv5k1Ports[i][2], AV_BIT_CCA_UART);
                    #endif

                #if AvEnableInternalVideoGen_5K1
                    gsv5k1Ports[i][7].content.video->timing.Vic = 0x10; /*10x0 1080p60 */ //0x04;//720p
                    gsv5k1Ports[i][7].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
                    gsv5k1Ports[i][7].content.video->Cd         = AV_CD_24;
                    gsv5k1Ports[i][7].content.video->Y          = AV_Y2Y1Y0_RGB;//
                    gsv5k1Ports[i][7].content.vg->Pattern       = AV_PT_COLOR_BAR;//AV_PT_RED_RAMP;//AV_PT_COLOR_BAR;//
                #endif
                    #if AvEnableForceOutHdcp
                    gsv5k1Ports[i][1].content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    gsv5k1Ports[i][2].content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    #endif
                }
            }
            break;
        default:
            break;
    }
}

void Gsv5k1RxInit(u8 u8Step)
{
    u8 i=0;
    switch(u8Step)
    {
        case 1: // add device
            {
                /* 2.4 connect devices to device declaration */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    AvApiAddDevice(&AvGsv5k1_devices[i], Gsv5k1,
                                   i+AvGsv2k6DeviceNumber+AvGsv2k8DeviceNumber+AvGsv2k2DeviceNumber+AvGsv2k1DeviceNumber+AvGsv2k11DeviceNumber,
                                   (void *)&gsv5k1_device[i], (void *)&gsv5k1Ports[i][0], NULL);
                }
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    #if (AvEnableCcaFeature == 1)
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][0] ,1 ,CcaRx);
                    #else
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][0] ,1 ,HdmiRx);
                    #endif
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][1] ,4 ,HdmiTx);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][2] ,5 ,HdmiTx);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][3] ,8 ,LogicAudioTx);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][4] ,16,VideoDsc);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][5] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][6] ,24,VideoColor);
                    #if AvEnableInternalVideoGen_5K1
                    AvApiAddPort(&AvGsv5k1_devices[i],&gsv5k1Ports[i][7] ,28,VideoGen);
                    #endif
                    /* 3.2 initialize port content */
                    #if (AvEnableCcaFeature == 1)
                    if(gsv5k1Ports[i][0].type == CcaRx)
                        gsv5k1Ports[i][0].content.ccarx->Enable = 1;
                    #endif

                    #if AvEnableCecFeature
                    AudioStatus = 1;
                    gsv5k1Ports[i][1].content.cec->CecEnable = 1;
                    if(AudioStatus == 0)
                        gsv5k1Ports[i][1].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
                    else
                    {
                        gsv5k1Ports[i][1].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                        gsv5k1Ports[i][1].content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                    }
                    Cec_Tx_Audio_Status.Volume = 30;
                    Cec_Tx_Audio_Status.Mute   = 0;    /*  */
                    Cec_Tx_Audio_Status.AudioMode = 1; /* Audio Mode is ON to meet ARC */
                    Cec_Tx_Audio_Status.AudioRate = 1; /* 100% rate */
                    Cec_Tx_Audio_Status.AudioFormatCode = AV_AUD_FORMAT_LINEAR_PCM; /* Follow Spec */
                    Cec_Tx_Audio_Status.MaxNumberOfChannels = 2; /* Max Channels */
                    Cec_Tx_Audio_Status.AudioSampleRate = 0x07; /* 32KHz/44.1KHz/48KHz */
                    Cec_Tx_Audio_Status.AudioBitLen = 0x01;  /* 16-bit only */
                    Cec_Tx_Audio_Status.MaxBitRate  = 0;  /* default */
                    Cec_Tx_Audio_Status.ActiveSource = 0; /* default */
                    #endif
                }
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv5k1_devices[i])==AvOk)
                    {
                        g_bGsv5k1InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv5k1InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {
                /* 3.4 routing */
                /* connect the port by video using AvConnectVideo */
                /* connect the port by audio using AvConnectAudio */
                /* connect the port by video and audio using AvConnectAV */

                /* 3.4.1 video routing */
                for(i=0;i<AvGsv5k1DeviceNumber;i++)
                {
                    #if AvEnableInternalVideoGen_5K1
                    AvApiConnectPort(&gsv5k1Ports[i][7], &gsv5k1Ports[i][1], AvConnectAV);
                    AvApiConnectPort(&gsv5k1Ports[i][7], &gsv5k1Ports[i][2], AvConnectAV);
                    #else
                    #if (Gsv5k1DisableTxAHdmi==0)
                    AvApiConnectPort(&gsv5k1Ports[i][0], &gsv5k1Ports[i][1], AvConnectAV);
                    #endif
                    AvApiConnectPort(&gsv5k1Ports[i][0], &gsv5k1Ports[i][2], AvConnectAV);
                    #endif

                    /* 3.4.2 audio routing */
                    /* AvApiConnectPort(&gsv5k1Ports[0], &gsv5k1Ports[3], AvConnectAudio); */
                    /* 3.4.2 ARC Connection, set after rx port connection to avoid conflict */
                    #if AvEnableCecFeature
                    if(AudioStatus == 1)
                    {
                        AvApiConnectPort(&gsv5k1Ports[i][1], &gsv5k1Ports[i][3], AvConnectAudio);
                    }
                    #endif

                    /* 3.4.3 gpio routing */
                    #if (AvEnableCcaFeature == 1)
                    if(gsv5k1Ports[i][1].type == CcaRx)
                        KfunCcaTxEnablePin(&gsv5k1Ports[i][1], AV_BIT_CCA_UART);
                    #endif

                #if AvEnableInternalVideoGen_5K1
                    gsv5k1Ports[i][7].content.video->timing.Vic = 0x10; /*10x0 1080p60 */ //0x04;//720p
                    gsv5k1Ports[i][7].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
                    gsv5k1Ports[i][7].content.video->Cd         = AV_CD_24;
                    gsv5k1Ports[i][7].content.video->Y          = AV_Y2Y1Y0_RGB;//
                    gsv5k1Ports[i][7].content.vg->Pattern       = AV_PT_WHITE;//AV_PT_RED_RAMP;//AV_PT_COLOR_BAR;//
                #endif

                    #if AvEnableForceOutHdcp
                    gsv5k1Ports[i][1].content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    gsv5k1Ports[i][2].content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    #endif
                }
            }
            break;
        default:
            break;
    }
}

#endif
#if GSV2K11
void Gsv2k11Init(u8 u8Step)
{
#if GSV2K11
    uint8 CommonBusConfig = AvGsv2k11ComBusCfg;
#endif
    u8 i=0;

    switch(u8Step)
    {
        case 1: // add device
            {
                /* 2.4 connect devices to device declaration */
                for(i=0;i<AvGsv2k11DeviceNumber;i++)
                    AvApiAddDevice(&AvGsv2k11_devices[i], Gsv2k11,
                                    i+AvGsv2k6DeviceNumber+AvGsv2k8DeviceNumber+AvGsv2k2DeviceNumber+AvGsv2k1DeviceNumber,
                                   (void *)&gsv2k11_device[i], (void *)&gsv2k11Ports[i][0], NULL);
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv2k11DeviceNumber;i++)
                {
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][0] ,0 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][1] ,5 ,HdmiTx);      //*
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][2] ,32,LogicVideoTx);
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][3] ,8 ,LogicAudioTx);
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][4] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][5] ,24,VideoColor);
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][6] ,28,VideoGen);
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][7] ,30,LogicVideoRx);//*
                    AvApiAddPort(&AvGsv2k11_devices[i],&gsv2k11Ports[i][8] ,10,LogicAudioRx);
                }
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv2k11DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv2k11_devices[i])==AvOk)
                    {
                        g_bGsv2k11InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv2k11InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {
                for(i=0;i<AvGsv2k11DeviceNumber;i++)
                {
                    if(LogicOutputSel[i] == 1)
                    {
                    //    AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][1], AvConnectAV);
            //            AvApiConnectPort(&gsv2k11Ports[i][6], &gsv2k11Ports[i][2], AvConnectVideo);// 2
                        AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][2], AvConnectVideo);// 2
                        AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][3], AvConnectAudio);
                    }
                    /* case 2: routing of LogicTx/Rx->TxB */
                    else
                    {
                        AvApiConnectPort(&gsv2k11Ports[i][7], &gsv2k11Ports[i][1], AvConnectVideo);//7
                        AvApiConnectPort(&gsv2k11Ports[i][8], &gsv2k11Ports[i][1], AvConnectAudio);
                    }

                #if AvEnableCecFeature
                    if(AudioStatus == 1)
                    {
                        AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][1], AvConnectAudio);
                    }
                #endif
                #if AvEnableInternalVideoGen
                    gsv2k11Ports[i][6].content.video->timing.Vic = 0x10; /*10x0 1080p60 */ //0x04;//720p
                    gsv2k11Ports[i][6].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
                    gsv2k11Ports[i][6].content.video->Cd         = AV_CD_24;
                    gsv2k11Ports[i][6].content.video->Y          = AV_Y2Y1Y0_RGB;//
                    gsv2k11Ports[i][6].content.vg->Pattern       = AV_PT_RED;//AV_PT_COLOR_BAR;//
                    AvApiConnectPort(&gsv2k11Ports[i][6], &gsv2k11Ports[i][1], AvConnectVideo);
                #endif
            #if AvEnableAudioTTLInput
                #if AvEnableTTLAudioModeI2S
                    gsv2k11Ports[i][8].content.audio->AudioMute    = 0;
                    gsv2k11Ports[i][8].content.audio->AudFormat    = AV_AUD_I2S;
                    gsv2k11Ports[i][8].content.audio->AudType      = AV_AUD_TYPE_ASP;
                    gsv2k11Ports[i][8].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k11Ports[i][8].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k11Ports[i][8].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k11Ports[i][8].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k11Ports[i][8].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k11Ports[i][8].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k11Ports[i][8].content.audio->CatCode      = 0;    /* Default */
                    gsv2k11Ports[i][8].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k11Ports[i][8].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k11Ports[i][8].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k11Ports[i][8].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k11Ports[i][8].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                #else
                    gsv2k11Ports[i][8].content.audio->AudioMute    = 0;
                    gsv2k11Ports[i][8].content.audio->AudFormat    = AV_AUD_SPDIF;
                    gsv2k11Ports[i][8].content.audio->AudType      = AV_AUD_TYPE_SPDIF;
                    gsv2k11Ports[i][8].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k11Ports[i][8].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k11Ports[i][8].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k11Ports[i][8].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k11Ports[i][8].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k11Ports[i][8].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k11Ports[i][8].content.audio->CatCode      = 0;    /* Default */
                    gsv2k11Ports[i][8].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k11Ports[i][8].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k11Ports[i][8].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k11Ports[i][8].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k11Ports[i][8].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                #endif
            #else
                #if AvEnableTTLAudioModeI2S
                    gsv2k11Ports[i][3].content.audio->AudioMute    = 0;
                    gsv2k11Ports[i][3].content.audio->AudFormat    = AV_AUD_I2S;
                    gsv2k11Ports[i][3].content.audio->AudType      = AV_AUD_TYPE_ASP;
                    gsv2k11Ports[i][3].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k11Ports[i][3].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k11Ports[i][3].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k11Ports[i][3].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k11Ports[i][3].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k11Ports[i][3].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k11Ports[i][3].content.audio->CatCode      = 0;    /* Default */
                    gsv2k11Ports[i][3].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k11Ports[i][3].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k11Ports[i][3].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k11Ports[i][3].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k11Ports[i][3].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                #else
                    gsv2k11Ports[i][3].content.audio->AudioMute    = 0;
                    gsv2k11Ports[i][3].content.audio->AudFormat    = AV_AUD_SPDIF;
                    gsv2k11Ports[i][3].content.audio->AudType      = AV_AUD_TYPE_SPDIF;
                    gsv2k11Ports[i][3].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k11Ports[i][3].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k11Ports[i][3].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k11Ports[i][3].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k11Ports[i][3].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k11Ports[i][3].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k11Ports[i][3].content.audio->CatCode      = 0;    /* Default */
                    gsv2k11Ports[i][3].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k11Ports[i][3].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k11Ports[i][3].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k11Ports[i][3].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k11Ports[i][3].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                #endif
            #endif
                    /* 3.4.5 Video Parallel Bus Input */
                    /* CommonBusConfig = 0 to disable, CommonBusConfig = 1~64 for feature setting */
                    gsv2k11Ports[i][2].content.lvtx->Config        = CommonBusConfig;
                    /* 3.4.5.1 LogicVideoTx Port's Y and InCS
                       = AV_Y2Y1Y0_INVALID/AV_CS_AUTO to do no 2011 color processing,
                       = Dedicated Color for internal Color/Scaler Processing */
                    gsv2k11Ports[i][2].content.video->Y           = AV_Y2Y1Y0_YCBCR_444;//AV_Y2Y1Y0_INVALID; AV_Y2Y1Y0_YCBCR_422;//
                    gsv2k11Ports[i][2].content.video->InCs        = AV_CS_LIM_YUV_709;//AV_CS_AUTO;
               //     gsv2k11Ports[i][2].content.video->OutCs        = AV_CS_LIM_RGB;
                    /* 3.4.5.2 LogicVideoTx Port's Limited Highest Pixel Clock Frequency
                      = 600 to output HDMI 2.0 on Parallel bus,
                      = 300 to output HDMI 1.4 on Parallel bus,
                      = 150 to output 1080p on Parallel bus */
                    gsv2k11Ports[i][2].content.video->info.TmdsFreq   = 300;

                    /* 3.4.6 Video Parallel Bus Input */
                //     gsv2k11Ports[7].content.video->timing.Vic  = 0x04; /* 720p60 */
                    gsv2k11Ports[i][7].content.video->timing.Vic  = 0x10; /* 1080p60 */
                    //gsv2k11Ports[7].content.video->timing.Vic  = 0x61; /* 4K60 */
                    gsv2k11Ports[i][7].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
                    gsv2k11Ports[i][7].content.video->Cd          = AV_CD_24;
                    if((ParallelConfigTable[CommonBusConfig*3 + 1] & 0x04) != 0)
                    {
                        gsv2k11Ports[i][7].content.video->Y           =  AV_Y2Y1Y0_YCBCR_444;//AV_Y2Y1Y0_YCBCR_422;//
                        gsv2k11Ports[i][7].content.video->InCs        =  AV_CS_LIM_YUV_709;//AV_CS_AUTO;//AV_CS_LIM_YUV_709;//
                    }
                    else
                    {
                        gsv2k11Ports[i][7].content.video->Y           = AV_Y2Y1Y0_YCBCR_444;//AV_Y2Y1Y0_RGB;
                        gsv2k11Ports[i][7].content.video->InCs        = AV_CS_LIM_YUV_709;//AV_CS_RGB;
                    }
                    gsv2k11Ports[i][7].content.lvrx->Config       = CommonBusConfig;

                    /* 3.4.7 Video Parallel Bus Config */
                    if(1)//(sys_info.out_hdcp_status[i] == 0)    // power up to load hdcp status
                    {
                        gsv2k11Ports[i][7].content.rx->VideoEncrypted = 0;
                        gsv2k11Ports[i][1].content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;
                    }
                    else
                    {
                        gsv2k11Ports[i][7].content.rx->VideoEncrypted = 1;
                        gsv2k11Ports[i][1].content.hdcptx->HdmiStyle = AV_HDCP_TX_1P4_ONLY;
                    }

                    if(LogicOutputSel[i] == 1)
                    {
                        gsv2k11Ports[i][2].content.lvtx->Update     = 1;
                    }
                    else
                    {
                        gsv2k11Ports[i][7].content.lvrx->Update     = 1;
                    }
                    g_stGsv2k11HdmiOutColorSpace[i].Cd      = AV_CD_24;
                    g_stGsv2k11HdmiOutColorSpace[i].Y       = AV_Y2Y1Y0_RGB;
                    g_stGsv2k11HdmiOutColorSpace[i].InCs    = AV_CS_RGB;
                    g_stGsv2k11HdmiOutColorSpace[i].OutCs   = AV_CS_RGB;
                    g_stGsv2k11ParamChg[i].u8TxColorSpace   = emTxCsType_RGB;
                }
            }
            break;
        default:
            break;
    }
}
#endif

#if GSV2K1
void Gsv2k1Init(u8 u8Step)
{
    u8 i=0;

    switch(u8Step)
    {
        case 1: // add device
            {
                /* connect devices to device declaration */
                for(i=0;i<AvGsv2k1DeviceNumber;i++)
                    AvApiAddDevice(&AvGsv2k1_devices[i], Gsv2k1,
                                   i+AvGsv2k6DeviceNumber+AvGsv2k8DeviceNumber+AvGsv2k2DeviceNumber,
                                   (void *)&gsv2k1_device[i], (void *)&gsv2k1Ports[i][0], NULL);
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv2k1DeviceNumber;i++)
                {
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][0] ,0 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][1] ,4 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][2] ,5 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][3] ,8 ,LogicAudioTx);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][4] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][5] ,24,VideoColor);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][6], 28,VideoGen);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][7] ,34,AudioGen);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][8] ,36,ClockGen);
                    AvApiAddPort(&AvGsv2k1_devices[i],&gsv2k1Ports[i][9] ,10,LogicAudioRx);
                }

            #if AvEnableCecFeature
                {
                    uint8 j=0;
                    for(i = 0;i<AvGsv2k1DeviceNumber;i++)
                    {
                        gsv2k1Ports[i][1].content.cec->CecEnable = 1;
                        //for(j = 0; j<AudioNumber; j++)
                        {
                            if(AudioStatus == 0)
                                gsv2k1Ports[i][1].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
                            else
                            {
                                gsv2k1Ports[i][1].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                                gsv2k1Ports[i][1].content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                            }
                            Cec_Tx_Audio_Status.Volume = 30;
                            Cec_Tx_Audio_Status.Mute   = 0;    /*  */
                            Cec_Tx_Audio_Status.AudioMode = 1; /* Audio Mode is ON to meet ARC */
                            Cec_Tx_Audio_Status.AudioRate = 1; /* 100% rate */
                            Cec_Tx_Audio_Status.AudioFormatCode = AV_AUD_FORMAT_LINEAR_PCM; /* Follow Spec */
                            Cec_Tx_Audio_Status.MaxNumberOfChannels = 2; /* Max Channels */
                            Cec_Tx_Audio_Status.AudioSampleRate = 0x07; /* 32KHz/44.1KHz/48KHz */
                            Cec_Tx_Audio_Status.AudioBitLen = 0x01;  /* 16-bit only */
                            Cec_Tx_Audio_Status.MaxBitRate  = 0;  /* default */
                            Cec_Tx_Audio_Status.ActiveSource = 0; /* default */
                        }
                    }
                }
            #endif
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv2k1DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv2k1_devices[i])==AvOk)
                    {
                        g_bGsv2k1InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv2k1InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {
                /* 3.4.1.1 Gsv2k1 connection is fixed to RxA->TxA/C, RxB->TxB/D */
                for(i=0;i<AvGsv2k1DeviceNumber;i++)
                {
                    AvApiConnectPort(&gsv2k1Ports[i][0], &gsv2k1Ports[i][1], AvConnectAV);
                    AvApiConnectPort(&gsv2k1Ports[i][0], &gsv2k1Ports[i][2], AvConnectAV);
                    AvApiConnectPort(&gsv2k1Ports[i][0], &gsv2k1Ports[i][3], AvConnectAudio);
                }
            #if AvEnableCecFeature
                {
                    for(i=0;i<AvGsv2k1DeviceNumber;i++)
                    {
                        if(AudioStatus == 1)
                        {
                            AvApiConnectPort(&gsv2k1Ports[i][0], &gsv2k1Ports[i][3], AvConnectAudio);
                        }
                    }
                }
            #endif
                /* 3.4.3 Internal Video Generator*/
            #if AvEnableInternalVideoGen
                for(i=0;i<AvGsv2k1DeviceNumber;i++)
                {
                    gsv2k1Ports[i][6].content.video->timing.Vic = 0x61; /* 4K60 */
                    gsv2k1Ports[i][6].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
                    gsv2k1Ports[i][6].content.video->Cd         = AV_CD_24;
                    gsv2k1Ports[i][6].content.video->Y          = AV_Y2Y1Y0_RGB;
                    gsv2k1Ports[i][6].content.vg->Pattern       = AV_PT_COLOR_BAR;
                }
            #endif

                /* 3.4.4 Audio Insertion */
            #if AvEnableAudioTTLInput
                for(i=0;i<AvGsv2k1DeviceNumber;i++)
                {
                    gsv2k1Ports[i][3].content.audio->AudioMute    = 0;
                    gsv2k1Ports[i][3].content.audio->AudFormat    = AV_AUD_I2S;
                    gsv2k1Ports[i][3].content.audio->AudType      = AV_AUD_TYPE_ASP;
                    gsv2k1Ports[i][3].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k1Ports[i][3].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k1Ports[i][3].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k1Ports[i][3].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k1Ports[i][3].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k1Ports[i][3].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k1Ports[i][3].content.audio->CatCode      = 0;    /* Default */
                    gsv2k1Ports[i][3].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k1Ports[i][3].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k1Ports[i][3].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k1Ports[i][3].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k1Ports[i][3].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                }
            #endif
            }
            break;
        default:
            break;
    }
}
#endif

#if GSV2K2
void Gsv2k2Init(u8 u8Step)
{
    u8 i=0;

    switch(u8Step)
    {
        case 1: // add device
            {
                /* connect devices to device declaration */
                for(i=0;i<AvGsv2k2DeviceNumber;i++)
                    AvApiAddDevice(&AvGsv2k2_devices[i], Gsv2k2,
                                   i+AvGsv2k6DeviceNumber+AvGsv2k8DeviceNumber,
                                   (void *)&gsv2k2_device[i], (void *)&gsv2k2Ports[i][0], NULL);
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv2k2DeviceNumber;i++)
                {
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][0] ,0 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][1] ,1 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][2] ,4 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][3] ,5 ,HdmiTx);
                #if AvEnableAudioTTLInput
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][4] ,10,LogicAudioRx);
                #else
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][4] ,8 ,LogicAudioTx);
                #endif
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][5] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv2k2_devices[i],&gsv2k2Ports[i][6] ,24,VideoColor);
                }

            #if AvEnableCecFeature
                {
                    uint8 j=0;
                    for(i = 0;i<AvGsv2k2DeviceNumber;i++)
                    {
                        gsv2k2Ports[i][2].content.cec->CecEnable = 1;
                        //for(j = 0; j<AudioNumber; j++)
                        {
                            if(AudioStatus == 0)
                                gsv2k2Ports[i][2].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
                            else
                            {
                                gsv2k2Ports[i][2].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                                gsv2k2Ports[i][2].content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                            }
                            Cec_Tx_Audio_Status.Volume = 30;
                            Cec_Tx_Audio_Status.Mute   = 0;    /*  */
                            Cec_Tx_Audio_Status.AudioMode = 1; /* Audio Mode is ON to meet ARC */
                            Cec_Tx_Audio_Status.AudioRate = 1; /* 100% rate */
                            Cec_Tx_Audio_Status.AudioFormatCode = AV_AUD_FORMAT_LINEAR_PCM; /* Follow Spec */
                            Cec_Tx_Audio_Status.MaxNumberOfChannels = 2; /* Max Channels */
                            Cec_Tx_Audio_Status.AudioSampleRate = 0x07; /* 32KHz/44.1KHz/48KHz */
                            Cec_Tx_Audio_Status.AudioBitLen = 0x01;  /* 16-bit only */
                            Cec_Tx_Audio_Status.MaxBitRate  = 0;  /* default */
                            Cec_Tx_Audio_Status.ActiveSource = 0; /* default */
                        }
                    }
                }
            #endif
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv2k2DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv2k2_devices[i])==AvOk)
                    {
                        g_bGsv2k2InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv2k2InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {
                for(i=0;i<AvGsv2k2DeviceNumber;i++)
                {
                #if AvEnableAudioTTLInput
                    AvApiConnectPort(&gsv2k2Ports[i][0], &gsv2k2Ports[i][2], AvConnectVideo);
                    AvApiConnectPort(&gsv2k2Ports[i][0], &gsv2k2Ports[i][3], AvConnectVideo);
                    AvApiConnectPort(&gsv2k2Ports[i][4], &gsv2k2Ports[i][2], AvConnectAudio);
                    AvApiConnectPort(&gsv2k2Ports[i][4], &gsv2k2Ports[i][3], AvConnectAudio);
                #else
                    AvApiConnectPort(&gsv2k2Ports[i][0], &gsv2k2Ports[i][2], AvConnectAV);
                    AvApiConnectPort(&gsv2k2Ports[i][0], &gsv2k2Ports[i][3], AvConnectAV);
                    AvApiConnectPort(&gsv2k2Ports[i][0], &gsv2k2Ports[i][4], AvConnectAudio);
                #endif
                }
                /* 3.4.2 audio routing */
            #if AvEnableCecFeature
                {
                    uint8 j=0;
                    for(i=0;i<AvGsv2k2DeviceNumber;i++)
                    {
                        for(j=0;j<AudioNumber;j++)
                        {
                            if(AudioStatus[j] == 1)
                            {
                                AvApiConnectPort(&gsv2k2Ports[i][0], &gsv2k2Ports[i][4], AvConnectAudio);
                            }
                        }
                    }
                }
            #endif

                /* 3.4.3 Internal Video Generator*/
            #if AvEnableInternalVideoGen
                for(i=0;i<AvGsv2k2DeviceNumber;i++)
                {
                    AvApiAddPort(&AvGsv2k2_devices[0],&gsv2k2Ports[i][11],28,VideoGen);
                    gsv2k2Ports[i][11].content.video->timing.Vic = 0x61; /* 4K60 */
                    gsv2k2Ports[i][11].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
                    gsv2k2Ports[i][11].content.video->Cd         = AV_CD_24;
                    gsv2k2Ports[i][11].content.video->Y          = AV_Y2Y1Y0_RGB;
                    gsv2k2Ports[i][11].content.vg->Pattern       = AV_PT_CHECKBOARD;
                    AvApiConnectPort(&gsv2k2Ports[i][11], &gsv2k2Ports[i][5], AvConnectVideo);
                }
            #endif
                /* 3.4.4 Audio Insertion */
            #if AvEnableAudioTTLInput
                for(i=0;i<AvGsv2k2DeviceNumber;i++)
                {
                    gsv2k2Ports[i][4].content.audio->AudioMute    = 0;
                    gsv2k2Ports[i][4].content.audio->AudFormat    = AV_AUD_I2S;
                    gsv2k2Ports[i][4].content.audio->AudType      = AV_AUD_TYPE_ASP;
                    gsv2k2Ports[i][4].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k2Ports[i][4].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k2Ports[i][4].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k2Ports[i][4].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k2Ports[i][4].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k2Ports[i][4].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k2Ports[i][4].content.audio->CatCode      = 0;    /* Default */
                    gsv2k2Ports[i][4].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k2Ports[i][4].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k2Ports[i][4].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k2Ports[i][4].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k2Ports[i][4].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                }
            #endif
            }
            break;
        default:
            break;
    }
}
#endif
#if GSV2K6
void Gsv2k6Init(u8 u8Step)
{
    u8 i=0;

    switch(u8Step)
    {
        case 1: // add device
            {
                /* connect devices to device declaration */
                for(i=0;i<AvGsv2k6DeviceNumber;i++)
                    AvApiAddDevice(&AvGsv2k6_devices[i], Gsv2k6, i, (void *)&gsv2k6_device[i],
                                   (void *)&gsv2k6Ports[i][0], NULL);
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv2k6DeviceNumber;i++)
                {
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][0] ,0 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][1] ,1 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][2] ,4 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][3] ,5 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][4] ,6 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][5] ,7 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][6] ,8 ,LogicAudioTx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][7] ,9 ,LogicAudioTx);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][8] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv2k6_devices[i],&gsv2k6Ports[i][9] ,24,VideoColor);
                }

                #if AvEnableCecFeature
                {
                    uint8 j=0;
                    for(i=0;i<AvGsv2k6DeviceNumber;i++)
                    {
                        gsv2k6Ports[i][2].content.cec->CecEnable = 1;
                        //for(j = 0; j<AudioNumber; j++)
                        {
                            if(AudioStatus == 0)
                                gsv2k6Ports[i][2].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
                            else
                            {
                                gsv2k6Ports[i][2].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                                gsv2k6Ports[i][2].content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                            }
                            Cec_Tx_Audio_Status.Volume = 30;
                            Cec_Tx_Audio_Status.Mute   = 0;    /*  */
                            Cec_Tx_Audio_Status.AudioMode = 1; /* Audio Mode is ON to meet ARC */
                            Cec_Tx_Audio_Status.AudioRate = 1; /* 100% rate */
                            Cec_Tx_Audio_Status.AudioFormatCode = AV_AUD_FORMAT_LINEAR_PCM; /* Follow Spec */
                            Cec_Tx_Audio_Status.MaxNumberOfChannels = 2; /* Max Channels */
                            Cec_Tx_Audio_Status.AudioSampleRate = 0x07; /* 32KHz/44.1KHz/48KHz */
                            Cec_Tx_Audio_Status.AudioBitLen = 0x01;  /* 16-bit only */
                            Cec_Tx_Audio_Status.MaxBitRate  = 0;  /* default */
                            Cec_Tx_Audio_Status.ActiveSource = 0; /* default */
                        }
                    }
                }
                #endif
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv2k6DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv2k6_devices[i])==AvOk)
                    {
                        g_bGsv2k6InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv2k6InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {
                /* 3.4.1.1 Gsv2k6 connection is fixed to RxA->TxA/C, RxB->TxB/D */
                for(i=0;i<AvGsv2k6DeviceNumber;i++)
                {
                    AvApiConnectPort(&gsv2k6Ports[i][0], &gsv2k6Ports[i][2], AvConnectAV);
                    AvApiConnectPort(&gsv2k6Ports[i][1], &gsv2k6Ports[i][3], AvConnectAV);
                    AvApiConnectPort(&gsv2k6Ports[i][0], &gsv2k6Ports[i][4], AvConnectAV);
                    AvApiConnectPort(&gsv2k6Ports[i][1], &gsv2k6Ports[i][5], AvConnectAV);
                }
            }
            break;
        default:
            break;
    }
}
#endif
#if GSV2K8
void Gsv2k8Init(u8 u8Step)
{
    u8 i=0;

    switch(u8Step)
    {
        case 1: // add device
            {
                /* connect devices to device declaration */
                #if AvMatrix4x4Mode
                for(i=0;i<AvGsv2k8DeviceNumber;i++)
                    AvApiAddDevice(&AvGsv2k8_devices[i], Gsv2k8, i+AvGsv2k6DeviceNumber, (void *)&gsv2k8_device[i],
                                   (void *)&gsv2k8Ports[i][0], NULL);
                #else
                for(i=0;i<AvGsv2k8DeviceNumber;i++)
                    AvApiAddDevice(&AvGsv2k8_devices[i], Gsv2k8, i+AvGsv2k6DeviceNumber, (void *)&gsv2k8_device[i],
                                   (void *)&gsv2k8Ports[i][0], NULL);
                #endif
            }
            break;
        case 2: // add port
            {
                /* 3. Port Level Declaration */
                /* 3.1 init devices and port structure, must declare in number order */
                /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx, 16-19 DSC,
                   20-23 Scaler, 24-27 Color, 28 VideoGen */
                for(i=0;i<AvGsv2k8DeviceNumber;i++)
                {
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][0] ,0 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][1] ,1 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][2] ,2 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][3] ,3 ,HdmiRx);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][4] ,4 ,HdmiTx);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][5] ,5 ,HdmiTx);
                #if AvMatrix4x4Mode
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][6] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][7] ,21,VideoScaler);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][8] ,24,VideoColor);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][9] ,25,VideoColor);
                #else
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][6] ,8 ,LogicAudioTx);
                    #if AvEnableAudioTTLInput
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][7] ,11,LogicAudioRx);
                    #else
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][7] ,9 ,LogicAudioTx);
                    #endif
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][8] ,20,VideoScaler);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][9] ,21,VideoScaler);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][10],24,VideoColor);
                    AvApiAddPort(&AvGsv2k8_devices[i],&gsv2k8Ports[i][11],25,VideoColor);
                #endif
                }

                #if (AvEnableCecFeature&&(!AvMatrix4x4Mode))
                {
                    uint8 j=0;
                    for(i=0;i<AvGsv2k8DeviceNumber;i++)
                    {
                        gsv2k8Ports[i][4].content.cec->CecEnable = 1;
                        gsv2k8Ports[i][5].content.cec->CecEnable = 1;
                    }
                    for(i = 0;i<AvGsv2k8DeviceNumber;i++)
                    {
                        if(AudioStatus == 0)
                            gsv2k8Ports[i][4].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_DISABLE;
                        else
                        {
                            gsv2k8Ports[i][4].content.cec->EnableAudioAmplifier = AV_CEC_AMP_TO_ENABLE;
                            gsv2k8Ports[i][4].content.cec->EnableARC = AV_CEC_ARC_TO_INITIATE;
                        }

                        //for(j = 0; j<AudioNumber; j++)
                        {
                            Cec_Tx_Audio_Status.Volume = 30;
                            Cec_Tx_Audio_Status.Mute   = 0;    /*  */
                            Cec_Tx_Audio_Status.AudioMode = 1; /* Audio Mode is ON to meet ARC */
                            Cec_Tx_Audio_Status.AudioRate = 1; /* 100% rate */
                            Cec_Tx_Audio_Status.AudioFormatCode = AV_AUD_FORMAT_LINEAR_PCM; /* Follow Spec */
                            Cec_Tx_Audio_Status.MaxNumberOfChannels = 2; /* Max Channels */
                            Cec_Tx_Audio_Status.AudioSampleRate = 0x07; /* 32KHz/44.1KHz/48KHz */
                            Cec_Tx_Audio_Status.AudioBitLen = 0x01;  /* 16-bit only */
                            Cec_Tx_Audio_Status.MaxBitRate  = 0;  /* default */
                            Cec_Tx_Audio_Status.ActiveSource = 0; /* default */
                        }
                    }
                }
                #endif

                #if (AvEnableAudioTTLInput&&(!AvMatrix4x4Mode))
                for(i=0;i<AvGsv2k8DeviceNumber;i++)
                {
                    gsv2k8Ports[i][7].content.audio->AudioMute    = 0;
                    gsv2k8Ports[i][7].content.audio->AudFormat    = AV_AUD_I2S;
                    gsv2k8Ports[i][7].content.audio->AudType      = AV_AUD_TYPE_ASP;
                    gsv2k8Ports[i][7].content.audio->AudCoding    = AV_AUD_FORMAT_LINEAR_PCM;
                    gsv2k8Ports[i][7].content.audio->AudMclkRatio = AV_MCLK_256FS;
                    gsv2k8Ports[i][7].content.audio->Layout       = 0;    /* 2 channel Layout = 0 */
                    gsv2k8Ports[i][7].content.audio->Consumer     = 0;    /* Consumer */
                    gsv2k8Ports[i][7].content.audio->Copyright    = 0;    /* Copyright asserted */
                    gsv2k8Ports[i][7].content.audio->Emphasis     = 0;    /* No Emphasis */
                    gsv2k8Ports[i][7].content.audio->CatCode      = 0;    /* Default */
                    gsv2k8Ports[i][7].content.audio->SrcNum       = 0;    /* Refer to Audio InfoFrame */
                    gsv2k8Ports[i][7].content.audio->ChanNum      = 2;    /* Audio Channel Count */
                    gsv2k8Ports[i][7].content.audio->SampFreq     = AV_AUD_FS_48KHZ; /* Sample Frequency */
                    gsv2k8Ports[i][7].content.audio->ClkAccur     = 0;    /* Level 2 */
                    gsv2k8Ports[i][7].content.audio->WordLen      = 0x0B; /* 24-bit word length */
                }
                #endif
            }
            break;
        case 3:     //Init Device
            {
                /* 3.3 init fsms */
                for(i=0;i<AvGsv2k8DeviceNumber;i++)
                {
                    if(AvApiInitDevice(&AvGsv2k8_devices[i])==AvOk)
                    {
                        g_bGsv2k8InitOk[i] = true;
                    }
                    else
                    {
                        g_bGsv2k8InitOk[i] = false;
                    }
                }
                //AvApiPortStart();
            }
            break;
        case 4:     // Port routing
            {

                #if AvMatrix4x4Mode
                /* 3.4.1.2 Gsv2k6 connection with Gsv2k8 is fixed */
                AvApiConnectPort(&gsv2k6Ports[0][2], &gsv2k8Ports[0][0], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[0][3], &gsv2k8Ports[0][1], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[0][4], &gsv2k8Ports[1][1], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[0][5], &gsv2k8Ports[1][0], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[1][2], &gsv2k8Ports[0][2], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[1][3], &gsv2k8Ports[0][3], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[1][4], &gsv2k8Ports[1][2], AvConnectAV);
                AvApiConnectPort(&gsv2k6Ports[1][5], &gsv2k8Ports[1][3], AvConnectAV);
                #else
                /* 3.4.1.1 Gsv2k8 connection is fixed to RxD->TxA, RxD->TxB */
                for(i=0;i<AvGsv2k8DeviceNumber;i++)
                {
                    AvApiConnectPort(&gsv2k8Ports[i][3], &gsv2k8Ports[i][4], AvConnectAV);
                    AvApiConnectPort(&gsv2k8Ports[i][3], &gsv2k8Ports[i][5], AvConnectAV);
                }
                #endif
            }
            break;
        default:
            break;
    }
}
#endif

void Gsv_Init(void)
{
    uint8 i=0;
    Gsv_GlobalVarInit();
    Gsv_SystemReset();

    /* 1. Low Level Hardware Level Initialization */
    /* 1.1 init bsp support (user speficic) */
    //BspInit();

    /* 1.2 init software package and hookup user's bsp functions */
    AvApiInit();
    //AvApiHookBspFunctions(&BspI2cRead, &BspI2cWrite, &BspUartSendByte,
    //                      &BspUartGetByte, &BspGetMilliSecond, &BspGetKey, &BspIrdaGetByte);
    //AvApiHookUserFunctions(&ListenToKeyCommand, &ListenToUartCommand, &ListenToIrdaCommand);
    AvApiHookBspFunctionsInit(&BspI2cRead, &BspI2cWrite);

    /* 2. Device Level Declaration */
    /* 2.1 total devices */
    /* it must be declared in AvDevice */

    /* 2.2 specific devices and ports */
    /* they must be able to be linked to the device in 1. */

    /* 2.3 init device address in 2.2 */
    #if GSV2K6||GSV2K8||GSV2K2||GSV2K1||GSV2K11||GSV5K1
    {
        for(i=0;i<GsvTotalDevNumber;i++)
        {
            switch(g_u8DeviceAddress[i])
            {
                #if GSV2K6
                case Gsv2k6:
                    {
                        static uint8 sl_u8FirstInitGsv2k6 = 1;

                        if(sl_u8FirstInitGsv2k6)
                        {
                            sl_u8FirstInitGsv2k6 = 0;
                            g_u8Gsv2k6StartIndex = i;
                        }
                        gsv2k6_device[i].DeviceAddress = AvGenerateDeviceAddress(i,0x01,0xB0,0x00);
                    }
                    break;
                #endif
                #if GSV2K8
                case Gsv2k8:
                    {
                        static uint8 sl_u8FirstInitGsv2k8 = 1;

                        if(sl_u8FirstInitGsv2k8)
                        {
                            sl_u8FirstInitGsv2k8 = 0;
                            g_u8Gsv2k8StartIndex = i;
                        }
                        gsv2k8_device[i-AvGsv2k6DeviceNumber].DeviceAddress = AvGenerateDeviceAddress(i,0x01,0xB0,0x00);
                    }
                    break;
                #endif
                #if GSV2K2
                case Gsv2k2:
                    {
                        static uint8 sl_u8FirstInitGsv2k2 = 1;

                        if(sl_u8FirstInitGsv2k2)
                        {
                            sl_u8FirstInitGsv2k2 = 0;
                            g_u8Gsv2k2StartIndex = i;
                        }
                        gsv2k2_device[i-AvGsv2k6DeviceNumber-AvGsv2k8DeviceNumber].DeviceAddress = AvGenerateDeviceAddress(i,0x01,0xB0,0x00);
                    }
                    break;
                #endif
                #if GSV2K1
                case Gsv2k1:
                    {
                        static uint8 sl_u8FirstInitGsv2k1 = 1;

                        if(sl_u8FirstInitGsv2k1)
                        {
                            sl_u8FirstInitGsv2k1 = 0;
                            g_u8Gsv2k1StartIndex = i;
                        }
                        gsv2k1_device[i-AvGsv2k6DeviceNumber-AvGsv2k8DeviceNumber-AvGsv2k2DeviceNumber].DeviceAddress = AvGenerateDeviceAddress(i,0x01,0xB0,0x00);
                    }
                    break;
                #endif
                #if GSV2K11
                case Gsv2k11:
                    {
                        static uint8 sl_u8FirstInitGsv2k11 = 1;

                        if(sl_u8FirstInitGsv2k11)
                        {
                            sl_u8FirstInitGsv2k11 = 0;
                            g_u8Gsv2k11StartIndex = i;
                        }
                        gsv2k11_device[i-AvGsv2k6DeviceNumber-AvGsv2k8DeviceNumber-AvGsv2k2DeviceNumber-AvGsv2k1DeviceNumber].DeviceAddress = AvGenerateDeviceAddress(i,0x01,0xB0,0x00);
                    }
                    break;
                #endif

                #if GSV5K1
                case Gsv5k1:
                    {
                        static uint8 sl_u8FirstInitGsv5k1 = 1;

                        if(sl_u8FirstInitGsv5k1)
                        {
                            sl_u8FirstInitGsv5k1 = 0;
                            g_u8Gsv5k1StartIndex = i;
                        }
                        gsv5k1_device[i-AvGsv2k6DeviceNumber-AvGsv2k8DeviceNumber-AvGsv2k2DeviceNumber-AvGsv2k1DeviceNumber-AvGsv2k11DeviceNumber].DeviceAddress = AvGenerateDeviceAddress(i,0x01,0xB0,0x00);
                    }
                    break;
                #endif
                default:
                    break;
            }
        }
    }
    #endif

    /* 2.4 connect devices to device declaration */
    #if GSV2K6
    Gsv2k6Init(1);
    #endif
#if GSV2K8
    Gsv2k8Init(1);
#endif

    #if GSV2K2
    Gsv2k2Init(1);
    #endif

    #if GSV2K1
    Gsv2k1Init(1);
    #endif

    #if GSV2K11
    Gsv2k11Init(1);
    #endif

#if GSV5K1
    if(TrxAvUpStreamFlag == 1)
    {
        Gsv5k1TxInit(1);
    }
    else if(TrxAvUpStreamFlag == 0)
    {
        Gsv5k1RxInit(1);
    }
#endif
    /* 3. Port Level Declaration */
    /* 3.1 init devices and port structure, must declare in number order */
    /* 0-3 HdmiRx, 4-7 HdmiTx, 8-9 TTLTx, 10-11 TTLRx,
       20-23 Scaler, 24-27 Color */
    /* 3.1.1 Add Rx Ports */
    #if GSV2K6
    Gsv2k6Init(2);
    #endif
    /* 3.1.2 Add Tx Ports */
    #if GSV2K8
    Gsv2k8Init(2);
    #endif
#if GSV2K2
    Gsv2k2Init(2);
#endif

#if GSV2K1
    Gsv2k1Init(2);
#endif
#if GSV2K11
    Gsv2k11Init(2);
#endif

#if GSV5K1
    if(TrxAvUpStreamFlag == 1)
    {
        Gsv5k1TxInit(2);
    }
    else if(TrxAvUpStreamFlag == 0)
    {
        Gsv5k1RxInit(2);
    }
#endif

    /* 3.3 init fsms */
    #if GSV2K6
    Gsv2k6Init(3);
    #endif
    #if GSV2K8
    Gsv2k8Init(3);
    #endif
    #if GSV2K2
    Gsv2k2Init(3);
    #endif
    #if GSV2K1
    Gsv2k1Init(3);
    #endif

    #if GSV2K11
    Gsv2k11Init(3);
    #endif
    #if GSV5K1
    if(TrxAvUpStreamFlag == 1)
    {
        Gsv5k1TxInit(3);
    }
    else if(TrxAvUpStreamFlag == 0)
    {
        Gsv5k1RxInit(3);
    }
    #endif
    AvApiPortStart();

    /* 3.4 routing */
    /* connect the port by video using AvConnectVideo */
    /* connect the port by audio using AvConnectAudio */
    /* connect the port by video and audio using AvConnectAV */

    /* 3.4.1 video routing */
    #if GSV2K6
    Gsv2k6Init(4);
    #endif
    #if GSV2K8
    Gsv2k8Init(4);
    #endif
#if GSV2K2
    Gsv2k2Init(4);
#endif


#if GSV2K1
    Gsv2k1Init(4);
#endif

#if GSV2K11
    Gsv2k11Init(4);
#endif
#if GSV5K1
    if(TrxAvUpStreamFlag == 1)
    {
        Gsv5k1TxInit(4);
    }
    else if(TrxAvUpStreamFlag == 0)
    {
        Gsv5k1RxInit(4);
    }
#endif
}

#if AvAllowHpdLowPixelClockRead
//5V输入检测，检测有5V则拉高对应输入的HPD
void mapp_GsvPowerManage(void)
{
    u8 i;
    u8 l_u8Port;
    static u8 sl_u8LoopCnt = 10;    // unit 10ms

    if(sl_u8LoopCnt)
    {
        sl_u8LoopCnt--;
        return;
    }
    sl_u8LoopCnt = 10;  // 100ms run to function

#if (AvMatrix4x4Mode)
    for(i=0;i<AvGsv2k8DeviceNumber;i++)
    {
        //AvDevice *l_pstDevice = &AvGsv2k8_devices[i];
    }
#elif GSV2K8
    for(i=0;i<AvGsv2k8DeviceNumber;i++)
    {
        AvDevice *l_pstDevice = &AvGsv2k8_devices[i];
        AvPort *l_pstAvPort = (AvPort *)l_pstDevice->port;
        static u8 sl_u8Pre5VSta[4] = {0,0,0,0};

        for(l_u8Port=0;l_u8Port<4;l_u8Port++)
        {
            AvPort *l_pstRxPort = &l_pstAvPort[l_u8Port];
            if(sl_u8Pre5VSta[l_u8Port] != l_pstRxPort->content.rx->Input5V)
            {
                sl_u8Pre5VSta[l_u8Port] = l_pstRxPort->content.rx->Input5V;
                if(l_pstRxPort->content.rx->Input5V != 0)
                {
                    KfunPrHpdUp(l_pstRxPort);
                }
                else
                {
                    KfunPrHpdDown(l_pstRxPort);
                }
            }

            if(g_bGsv2k8HpdForceChange[i][l_u8Port])
            {
                if((l_pstRxPort->content.rx->Hpd == AV_HPD_EDID_TOGGLE)
                    ||(l_pstRxPort->content.rx->Hpd == AV_HPD_HIGH))
                {
                    KfunPrHpdDown(l_pstRxPort);
                    sl_u8Pre5VSta[l_u8Port] = 0;
                }
            }
        }
    }
#elif GSV2K6
    for(i=0;i<AvGsv2k6DeviceNumber;i++)
    {
        AvDevice *l_pstDevice = &AvGsv2k6_devices[i];
        AvPort *l_pstAvPort = (AvPort *)l_pstDevice->port;
        static u8 sl_u8PreRx5VSta[2] = {0,0};

        for(l_u8Port=0;l_u8Port<2;l_u8Port++)
        {
            AvPort *l_pstRxPort = &l_pstAvPort[l_u8Port];
            if(sl_u8PreRx5VSta[l_u8Port] != l_pstRxPort->content.rx->Input5V)
            {
                sl_u8PreRx5VSta[l_u8Port] = l_pstRxPort->content.rx->Input5V;
                if(l_pstRxPort->content.rx->Input5V != 0)
                {
                    KfunPrHpdUp(l_pstRxPort);
                }
                else
                {
                    KfunPrHpdDown(l_pstRxPort);
                }
            }

            if(g_bGsv2k6HpdForceChange[i][l_u8Port])
            {
                if((l_pstRxPort->content.rx->Hpd == AV_HPD_EDID_TOGGLE)
                    ||(l_pstRxPort->content.rx->Hpd == AV_HPD_HIGH))
                {
                    KfunPrHpdDown(l_pstRxPort);
                    sl_u8PreRx5VSta[l_u8Port] = 0;
                }
            }
        }
    }
#elif GSV2K2
    #if 1
    for(i=0;i<AvGsv2k2DeviceNumber;i++)
    {
        AvDevice *l_pstDevice = &AvGsv2k2_devices[i];
        AvPort *l_pstAvPort = (AvPort *)l_pstDevice->port;
        static u8 sl_u8PreRx5VSta[2] = {0,0};

        for(l_u8Port=0;l_u8Port<2;l_u8Port++)
        {
            AvPort *l_pstRxPort = &l_pstAvPort[l_u8Port];
            if(sl_u8PreRx5VSta[l_u8Port] != l_pstRxPort->content.rx->Input5V)
            {
                sl_u8PreRx5VSta[l_u8Port] = l_pstRxPort->content.rx->Input5V;
                if(l_pstRxPort->content.rx->Input5V != 0)
                {
                    KfunPrHpdUp(l_pstRxPort);
                }
                else
                {
                    KfunPrHpdDown(l_pstRxPort);
                }
            }

            if(g_bGsv2k2HpdForceChange[i][l_u8Port])
            {
                if((l_pstRxPort->content.rx->Hpd == AV_HPD_EDID_TOGGLE)
                    ||(l_pstRxPort->content.rx->Hpd == AV_HPD_HIGH))
                {
                    KfunPrHpdDown(l_pstRxPort);
                    sl_u8PreRx5VSta[l_u8Port] = 0;
                }
            }
        }
    }
    #endif
#elif GSV2K1
    for(i=0;i<AvGsv2k1DeviceNumber;i++)
    {
        AvDevice *l_pstDevice = &AvGsv2k1_devices[i];
        AvPort *l_pstAvPort = (AvPort *)l_pstDevice->port;
        static u8 sl_u8PreRx5VSta[1] = {0};

        for(l_u8Port=0;l_u8Port<1;l_u8Port++)
        {
            AvPort *l_pstRxPort = &l_pstAvPort[l_u8Port];
            if(sl_u8PreRx5VSta[l_u8Port] != l_pstRxPort->content.rx->Input5V)
            {
                sl_u8PreRx5VSta[l_u8Port] = l_pstRxPort->content.rx->Input5V;
                if(l_pstRxPort->content.rx->Input5V != 0)
                {
                    KfunPrHpdUp(l_pstRxPort);
                }
                else
                {
                    KfunPrHpdDown(l_pstRxPort);
                }
            }

            if(g_bGsv2k1HpdForceChange[i][l_u8Port])
            {
                if((l_pstRxPort->content.rx->Hpd == AV_HPD_EDID_TOGGLE)
                    ||(l_pstRxPort->content.rx->Hpd == AV_HPD_HIGH))
                {
                    KfunPrHpdDown(l_pstRxPort);
                    sl_u8PreRx5VSta[l_u8Port] = 0;
                }
            }
        }
    }
#endif

}
#endif

/**
 * @brief  sample main entry for audio/video based software
 * @return  return 0
 */
int GsvMain(void)
{
    u8 i;
    /* 4. routine */
    /* call update api to enter into audio/video software loop */
    AvApiUpdate();
#if (AvMatrix4x4Mode)
    for(i=0;i<AvGsv2k8DeviceNumber;i++)
    {
        AvPortConnectUpdate(&AvGsv2k8_devices[i]);
    }
#else
    #if GSV2K8
    for(i=0;i<AvGsv2k8DeviceNumber;i++)
    {
        AvPortConnectUpdate(&AvGsv2k8_devices[i]);
    }
    #endif
    #if GSV2K6
    for(i=0;i<AvGsv2k6DeviceNumber;i++)
    {
        AvPortConnectUpdate(&AvGsv2k6_devices[i]);
    }
    #endif
#endif
    #if GSV2K2
    for(i=0;i<AvGsv2k2DeviceNumber;i++)
    {
        AvPortConnectUpdate(&AvGsv2k2_devices[i]);
    }
    #endif
    #if GSV2K1
    for(i=0;i<AvGsv2k1DeviceNumber;i++)
    {
        AvPortConnectUpdate(&AvGsv2k1_devices[i]);
    }
    #endif
    #if GSV2K11
    for(i=0;i<AvGsv2k11DeviceNumber;i++)
    {
        if(mapp_GsvGetStandby(i))
        {
            Gsv_StandbyProcess(i);
            break;
        }
        AvPortConnectUpdate(&AvGsv2k11_devices[i]);

        GsvParamChangeProcess(i);
        Gsv_StandbyProcess(i);
    }
    #endif

    #if GSV5K1
    for(i=0;i<AvGsv5k1DeviceNumber;i++)
    {
        AvPortConnectUpdate(&AvGsv5k1_devices[i]);
    }
    #endif
    return 0;
}

//=============================================
//  set input/output port support hdcp type
void mapp_GsvHdcpManage(AvPort *port, u8 u8HdcpType)
{
    if(port->type == HdmiRx)
    {
        if(u8HdcpType == 1)
        {
            port->content.hdcp->HdcpNeeded = AV_HDCP_RX_1P4_ONLY;
        }
        else if(u8HdcpType == 2)
        {
            port->content.hdcp->HdcpNeeded = AV_HDCP_RX_2P2_ONLY;
        }
        else if(u8HdcpType == 3)
        {
            port->content.hdcp->HdcpNeeded = AV_HDCP_RX_AUTO;
        }
        else
        {
            port->content.hdcp->HdcpNeeded = AV_HDCP_RX_NOT_SUPPORT;
        }
        /* Force Rx DDC Rerun */
    #if AvAllowHpdLowPixelClockRead
        #if GSV2K8
        g_bGsv2k8HpdForceChange[port->device->index][port->index] = TRUE;
        #elif GSV2K6
        g_bGsv2k6HpdForceChange[port->device->index][port->index] = TRUE;
        #elif GSV2K1
        g_bGsv2k1HpdForceChange[port->device->index][port->index] = TRUE;
        #else // if GSV2K2
        g_bGsv2k2HpdForceChange[port->device->index][port->index] = TRUE;
        #endif
        port->content.rx->Hpd = AV_HPD_EDID_TOGGLE;
    #else
        port->content.rx->Hpd = AV_HPD_TOGGLE;
    #endif
    }
    else if(port->type == HdmiTx)
    {
        #if GSV2K11
        AvPort *l_pFromPort = (AvPort*)port->content.RouteVideoFromPort;
        #endif
        if(u8HdcpType == 1)
        {
            #if AvGsv2kxTxHdcpFailOut
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_1P4_FAIL_OUT;
            #else
            #if AvGsv2kxVideoHdcp14ForValensTxProblem
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_2P2_ONLY;
            #else
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_1P4_ONLY;
            #endif
            #endif

            #if GSV2K11
            l_pFromPort->content.rx->VideoEncrypted = 1;
            AvUapiTxEncryptSink(port);
            #endif
        }
        else if(u8HdcpType == 2)
        {
            #if AvGsv2kxTxHdcpFailOut
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_2P2_FAIL_OUT;
            #else
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_2P2_ONLY;
            #endif

            #if GSV2K11
            l_pFromPort->content.rx->VideoEncrypted = 1;
            AvUapiTxEncryptSink(port);
            #endif
        }
        else if(u8HdcpType == 3)
        {
            #if AvGsv2kxTxHdcpFailOut
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO_FAIL_OUT;
            #else
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_AUTO;
            #endif

            #if GSV2K11
            l_pFromPort->content.rx->VideoEncrypted = 1;
            AvUapiTxEncryptSink(port);
            #endif
        }
        else
        {
            port->content.hdcptx->HdmiStyle = AV_HDCP_TX_ILLEGAL_NO_HDCP;

            #if GSV2K11
            l_pFromPort->content.rx->VideoEncrypted = 0;
            AvUapiTxDecryptSink(port);
            #endif
        }
        /* Force Tx HPD rerun */
        port->content.hdcptx->HdcpModeUpdate = 1;
        #if 0//AvGsv2kxVideoHdcp14ForValensTxProblem
        port->content.video->AvailableVideoPackets = (~AV_BIT_AV_INFO_FRAME);
        #endif
        port->content.video->AvailableVideoPackets =
            port->content.video->AvailableVideoPackets & (~AV_BIT_AV_INFO_FRAME);
    }

}

//=============================================
// hdcp ver: 0 No hdcp, 1 Hdcp 1.4, 2 hdcp 2.2
u8 mapp_GsvGetRxHdcpVer(AvPort *port)
{
    u8 l_u8HdcpType = 0;

    if(port->type == HdmiRx)
    {
        if(port->content.rx->VideoEncrypted != 0)
        {
//            if(port->content.hdcp->Hdcp2p2Flag != 0)
            if(port->content.hdcp->Hdcp2p2RxRunning != 0)
                l_u8HdcpType = AV_HDCP_2P2_ENCRYPTED;
            else
                l_u8HdcpType = AV_HDCP_1P4_ENCRYPTED;
        }
        else
            l_u8HdcpType = AV_HDCP_NOT_ENCRYPTED;
    }

    return l_u8HdcpType;
}

//=============================================
//
void mapp_GsvTxTmdsMute(AvPort *port,bool bMute)
{
    if(bMute)
    {
        KfunTxTmdsMute(port);
    }
    else
    {
        KfunTxTmdsUnmute(port);
    }
}

#if GSV2K11
void mapp_GsvVideoInTxOutputTimingConfig(AvPort *port,u8 u8TimingID)
{
    AvPort *l_pAvPort = &port[7];

    if(g_bGsv2k11InitOk[port->device->index] == false)
        return;

    if(u8TimingID<RES_DEFAULT_MAX)
    {
        l_pAvPort->content.video->timing.Vic         = res_table[u8TimingID].u8Vic;
        l_pAvPort->content.video->timing.HPolarity   = res_table[u8TimingID].u8HPol;
        l_pAvPort->content.video->timing.VPolarity   = res_table[u8TimingID].u8VPol;
        l_pAvPort->content.video->timing.Interlaced  = res_table[u8TimingID].u8Interlace;
    #if AvEnableDetailTiming
        l_pAvPort->content.video->timing.HActive     = res_table[u8TimingID].u16HActive;
        l_pAvPort->content.video->timing.VActive     = res_table[u8TimingID].u16VActive;
        l_pAvPort->content.video->timing.HTotal      = res_table[u8TimingID].u16HTotal;
        l_pAvPort->content.video->timing.VTotal      = res_table[u8TimingID].u16VTotal;
        l_pAvPort->content.video->timing.FrameRate   = res_table[u8TimingID].u8VClk;
        l_pAvPort->content.video->timing.HSync       = res_table[u8TimingID].u16HSync;
        l_pAvPort->content.video->timing.HBack       = res_table[u8TimingID].u16HBP;
        l_pAvPort->content.video->timing.VSync       = res_table[u8TimingID].u16VSync;
        l_pAvPort->content.video->timing.VBack       = res_table[u8TimingID].u16VBP;
    #endif
        l_pAvPort->content.lvrx->Update              = 1;

    }
    else
    {
        l_pAvPort->content.video->timing.Vic         = 0;
        l_pAvPort->content.lvrx->Update              = 1;
    }

    AvUapiOutputDebugMessage("TX[%d] Tx Timing change to %d\n",port->device->index,u8TimingID);
}

void mapp_GsvSetTimingChange(u8 u8ID)
{
    //NewVic[u8ID] = u8VIC;
    g_stGsv2k11ParamChg[u8ID].u8TxTimingChangeFlag = 1;
}

void mapp_GsvSetTimingID(u8 u8ID,u8 u8ResID)
{
    //NewVic[u8ID] = u8VIC;
    g_stGsv2k11ParamChg[u8ID].u8TxTimingID = u8ResID;
}

void mapp_GsvTimingConfig(u8 u8ChipID,u8 u8TimingID)
{
    if(g_bGsv2k11InitOk[u8ChipID] == false)
        return;
    mapp_GsvVideoInTxOutputTimingConfig(gsv2k11Ports[u8ChipID],u8TimingID);
}

void mapp_GsvSetStandby(u8 u8ID,bool bOn)
{
    g_bGsvStandbyFlag[u8ID] = bOn;
}

bool mapp_GsvGetStandby(u8 u8ID)
{
    return g_bPreGsvStbFlag[u8ID];
}

void Gsv_StandbyProcess(u8 u8ID)
{
    if(g_bPreGsvStbFlag[u8ID] != g_bGsvStandbyFlag[u8ID])
    {
        if(g_bGsvStandbyFlag[u8ID])
        {
            mapp_GsvTxTmdsMute(&gsv2k11Ports[u8ID][1],true);
        }
        else
        {
            mapp_GsvTxTmdsMute(&gsv2k11Ports[u8ID][1],false);
            mapp_GsvSetTimingChange(u8ID);      // add by wf8421 20220610 to make sure     success for output enable
        }
        g_bPreGsvStbFlag[u8ID] = g_bGsvStandbyFlag[u8ID];
    }
}

void GSV_GetTxStaFunction(AvPort *port)
{
    if(port->type == HdmiTx)
    {
        if(g_stGsv2k11ParamChg[port->device->index].u8TxHpdSta != port->content.tx->Hpd)
        {
            if(port->content.tx->Hpd)
            {
                //sys_temp.out_hpd[port->device->index] = 1;

                GsvSetTxHdcpStatus(port->device->index,g_stGsv2k11ParamChg[port->device->index].u8TxHdcpSta);
            }
            else
            {
                //sys_temp.out_hpd[port->device->index] = 0;

            }
            g_stGsv2k11ParamChg[port->device->index].u8TxHpdSta = port->content.tx->Hpd;
        }
    }
}

void Gsv2k11SetTxHdmiMode(u8 u8ID,u8 u8TxMode)
{
    AvPort *port = &gsv2k11Ports[u8ID][1];
    port->content.tx->HdmiMode = u8TxMode;
    KfunTxSetHdmiModeSupport(port);
}

void GsvSetTxMode(u8 u8ID,u8 u8TxMode)
{
    g_stGsv2k11ParamChg[u8ID].u8TxHdmiMode = u8TxMode;
    g_stGsv2k11ParamChg[u8ID].u8TxFormatChange = 1;
}

void GsvSetTxHdcpStatus(u8 u8ID,u8 u8TxMode)
{
    g_stGsv2k11ParamChg[u8ID].u8TxHdcpSta = u8TxMode;
    g_stGsv2k11ParamChg[u8ID].u8TxHdcpChangeFlag = 1;
}

u8 GsvGetTxHpdStatus(u8 u8ID)
{
    return g_stGsv2k11ParamChg[u8ID].u8TxHpdSta;
}

void Gsv2k11SetTxHdmiColorSpace(u8 u8ID,u8 u8TxCsMode)
{
    AvPort *port = &gsv2k11Ports[u8ID][1];

    if(port->content.tx->HdmiMode==1)
    {
        switch(u8TxCsMode)
        {
            case emTxCsType_RGB:
//                port->content.video->Y = AV_Y2Y1Y0_RGB;
//                port->content.video->OutCs = AV_CS_RGB;
//                port->content.video->Cd = AV_CD_24;
                g_stGsv2k11HdmiOutColorSpace[u8ID].Cd      = AV_CD_24;
                g_stGsv2k11HdmiOutColorSpace[u8ID].Y       = AV_Y2Y1Y0_RGB;
                g_stGsv2k11HdmiOutColorSpace[u8ID].OutCs   = AV_CS_RGB;
                break;
            case emTxCsType_YCBCR444:
//                port->content.video->Y = AV_Y2Y1Y0_YCBCR_444;
//                port->content.video->OutCs = AV_CS_YUV_709;
//                port->content.video->Cd = AV_CD_24;
                g_stGsv2k11HdmiOutColorSpace[u8ID].Cd      = AV_CD_24;
                g_stGsv2k11HdmiOutColorSpace[u8ID].Y       = AV_Y2Y1Y0_YCBCR_444;
                g_stGsv2k11HdmiOutColorSpace[u8ID].OutCs   = AV_CS_YUV_709;
                break;
            case emTxCsType_YCBCR422:
//                port->content.video->Y = AV_Y2Y1Y0_YCBCR_422;
//                port->content.video->OutCs = AV_CS_YUV_709;
//                port->content.video->Cd = AV_CD_30;
                g_stGsv2k11HdmiOutColorSpace[u8ID].Cd      = AV_CD_30;
                g_stGsv2k11HdmiOutColorSpace[u8ID].Y       = AV_Y2Y1Y0_YCBCR_422;
                g_stGsv2k11HdmiOutColorSpace[u8ID].OutCs   = AV_CS_YUV_709;
                break;
        }
    }
    else
    {
//        port->content.video->Y = AV_Y2Y1Y0_RGB;
//        port->content.video->OutCs = AV_CS_RGB;
        g_stGsv2k11HdmiOutColorSpace[u8ID].Y       = AV_Y2Y1Y0_RGB;
        g_stGsv2k11HdmiOutColorSpace[u8ID].OutCs   = AV_CS_RGB;
    }
    KfunTxSetHdmiModeSupport(port);
}

void GsvParamChangeProcess(u8 u8ID)
{
    if(g_stGsv2k11ParamChg[u8ID].u8TxFormatChange)
    {
        Gsv2k11SetTxHdmiMode(u8ID,g_stGsv2k11ParamChg[u8ID].u8TxHdmiMode);
        g_stGsv2k11ParamChg[u8ID].u8TxFormatChange = 0;
    }

    if(g_stGsv2k11ParamChg[u8ID].u8TxHdcpChangeFlag)
    {
        g_stGsv2k11ParamChg[u8ID].u8TxHdcpChangeFlag = 0;
        mapp_GsvHdcpManage(&gsv2k11Ports[u8ID][1],g_stGsv2k11ParamChg[u8ID].u8TxHdcpSta);
    }

    if(g_stGsv2k11ParamChg[u8ID].u8TxTimingChangeFlag)
    {
        g_stGsv2k11ParamChg[u8ID].u8TxTimingChangeFlag = 0;
        mapp_GsvTimingConfig(u8ID,g_stGsv2k11ParamChg[u8ID].u8TxTimingID);
    }

    if(g_stGsv2k11ParamChg[u8ID].u8TxColorSpaceChangeFlag)
    {
        g_stGsv2k11ParamChg[u8ID].u8TxColorSpaceChangeFlag = 0;
        Gsv2k11SetTxHdmiColorSpace(u8ID,g_stGsv2k11ParamChg[u8ID].u8TxColorSpace);
    }
    GSV_GetTxStaFunction(&gsv2k11Ports[u8ID][1]);
}


void mapp_GsvGetVideoOutputTiming(AvPort *port,pstTimingTable_t pstRxTiming)
{
    AvPort *l_pAvPort = &port[0];

//    if(g_bGsv2k11InitOk[port->device->index] == false)
//        return;

    if(l_pAvPort->content.rx->IsInputStable == 1)
    {
        pstRxTiming->u8Vic          = l_pAvPort->content.video->timing.Vic;
        pstRxTiming->u8HPol         = l_pAvPort->content.video->timing.HPolarity;
        pstRxTiming->u8VPol         = l_pAvPort->content.video->timing.VPolarity;
        pstRxTiming->u8Interlace    = l_pAvPort->content.video->timing.Interlaced;
        #if AvEnableDetailTiming
        pstRxTiming->u16HActive     = l_pAvPort->content.video->timing.HActive;
        pstRxTiming->u16VActive     = l_pAvPort->content.video->timing.VActive;
        pstRxTiming->u16HTotal      = l_pAvPort->content.video->timing.HTotal;
        pstRxTiming->u16VTotal      = l_pAvPort->content.video->timing.VTotal;
        pstRxTiming->u8VClk         = l_pAvPort->content.video->timing.FrameRate;
        pstRxTiming->u16HSync       = l_pAvPort->content.video->timing.HSync;
        pstRxTiming->u16HBP         = l_pAvPort->content.video->timing.HBack;
        pstRxTiming->u16VSync       = l_pAvPort->content.video->timing.VSync;
        pstRxTiming->u16VBP         = l_pAvPort->content.video->timing.VBack;
        #endif
    }
    else
    {
        pstRxTiming->u8Vic          = 0;
        pstRxTiming->u8HPol         = 0;
        pstRxTiming->u8VPol         = 0;
        pstRxTiming->u8Interlace    = 0;
        #if AvEnableDetailTiming
        pstRxTiming->u16HActive     = 0;
        pstRxTiming->u16VActive     = 0;
        pstRxTiming->u16HTotal      = 0;
        pstRxTiming->u16VTotal      = 0;
        pstRxTiming->u8VClk         = 0;
        pstRxTiming->u16HSync       = 0;
        pstRxTiming->u16HBP         = 0;
        pstRxTiming->u16VSync       = 0;
        pstRxTiming->u16VBP         = 0;
        #endif
    }

#if 0
    AvUapiOutputDebugMessage("RX[%d] Timing change\n",port->device->index);
    AvUapiOutputDebugMessage("u8Vic\n",pstRxTiming->u8Vic);
    AvUapiOutputDebugMessage("u8HPol\n",pstRxTiming->u8HPol);
    AvUapiOutputDebugMessage("u8VPol\n",pstRxTiming->u8VPol);
    AvUapiOutputDebugMessage("u8Interlace\n",pstRxTiming->u8Interlace);
#if AvEnableDetailTiming
    AvUapiOutputDebugMessage("u16HActive\n",pstRxTiming->u16HActive);
    AvUapiOutputDebugMessage("u16VActive\n",pstRxTiming->u16VActive);
    AvUapiOutputDebugMessage("u16HTotal\n",pstRxTiming->u16HTotal);
    AvUapiOutputDebugMessage("u16VTotal\n",pstRxTiming->u16VTotal);
    AvUapiOutputDebugMessage("u8VClk\n",pstRxTiming->u8VClk);
    AvUapiOutputDebugMessage("u16HSync\n",pstRxTiming->u16HSync);
    AvUapiOutputDebugMessage("u16HBP\n",pstRxTiming->u16HBP);
    AvUapiOutputDebugMessage("u16VSync\n",pstRxTiming->u16VSync);
    AvUapiOutputDebugMessage("u16VBP\n",pstRxTiming->u16VBP);
#endif
#endif
}

#endif

#endif

