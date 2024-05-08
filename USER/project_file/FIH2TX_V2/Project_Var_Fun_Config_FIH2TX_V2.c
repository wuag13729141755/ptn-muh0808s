#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif


stProjectSpecVar_t  stF1H2TX_V2SpecVar;

static volatile u32 g_F1H2TX_V2_Timers[emUserTimer_Max];
bool g_bF1H2TX_V2AutoSwitchFirstStatus = TRUE;
u8 g_u8ActiveSource = 0;
u8 g_u8PreActiveSource = 0;
bool g_bSwitchPortFlag = FALSE;

extern stProjectGlobalAllVar_t g_stPrjGloVar;
extern uint8  LogicOutputSel[AvGsv2k11DeviceNumber];//
extern AvEdidMode EdidHdmiMode;

//================FPGA CONFIG=============================
stResolution_t g_stRxResTable[2];
stWinConfigVar_t g_stWinConfigVar[Def_outport_num];
u8 g_u8Fwversion;    // fpga_tx
u8 g_u8BlackScreenSta = 0;    // 0: freeze  1:blackscreen

const uint8_t edid_F1H2TX_V2_default[256] = // default edid
{
    0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,
    0x55,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
    0x32,0x1C,0x01,0x03,0x80,0x50,0x2D,0x78,
    0x0A,0xEE,0x91,0xA3,0x54,0x4C,0x99,0x26,
    0x0F,0x50,0x54,0x01,0x08,0x00,0xD1,0xC0,
    0xD1,0x00,0x81,0xC0,0x01,0x01,0x01,0x01,
    0x01,0x01,0x01,0x01,0x01,0x01,0x08,0xE8,
    0x00,0x30,0xF2,0x70,0x5A,0x80,0xB0,0x58,
    0x8A,0x00,0x20,0xC2,0x31,0x00,0x00,0x1E,
    0x04,0x74,0x00,0x30,0xF2,0x70,0x5A,0x80,
    0xB0,0x58,0x8A,0x00,0x20,0x52,0x31,0x00,
    0x00,0x1E,0x00,0x00,0x00,0xFD,0x00,0x18,
    0x4B,0x1A,0x51,0x3C,0x00,0x0A,0x20,0x20,
    0x20,0x20,0x20,0x20,0x00,0x00,0x00,0xFC,
    0x00,0x48,0x44,0x20,0x56,0x69,0x64,0x65,
    0x6F,0x0A,0x20,0x20,0x20,0x20,0x01,0xA1,
    0x02,0x03,0x3D,0x70,0x53,0x61,0x60,0x5F,
    0x5E,0x5D,0x66,0x65,0x64,0x63,0x62,0x10,
    0x1F,0x22,0x21,0x20,0x05,0x14,0x04,0x13,
    0x23,0x09,0x1F,0x07,0x83,0x01,0x00,0x00,
    0x67,0x03,0x0C,0x00,0x10,0x00,0xB8,0x78,
    0x67,0xD8,0x5D,0xC4,0x01,0xFF,0x80,0x03,
    0xE2,0x00,0x0F,0xE3,0x05,0xFF,0x01,0xE5,
    0x0E,0x61,0x60,0x66,0x65,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA3,
};

const stWinTypeFactorType_t cg_stF1H2TX_V2ClipWinFactoryType_t[][4]=
{
    //default
    {
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 1x2
    {
        {   0,  50,  0, 100,  50, 100, 100,  100},
        {  50, 100,  0, 100,  50, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 1x3
    {
        {   0,   3,  0, 100,   1,   3, 100,  100},
        {   1,   3,  0, 100,   1,   3, 100,  100},
        {   2,   3,  0, 100,   1,   3, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 1x4
    {
        {   0,   4,  0, 100,   1,   4, 100,  100},
        {   1,   4,  0, 100,   1,   4, 100,  100},
        {   2,   4,  0, 100,   1,   4, 100,  100},
        {   3,   4,  0, 100,   1,   4, 100,  100},
    },
    // 2x2
    {
        {   0,  50,  0, 100,  50, 100,  50,  100},
        {  50, 100,  0, 100,  50, 100,  50,  100},
        {   0,  50, 50, 100,  50, 100,  50,  100},
        {  50, 100, 50, 100,  50, 100,  50,  100},
    },
    // 2x1
    {
        {   0, 100,  0,   2, 100, 100,   1,    2},
        {   0, 100,  1,   2, 100, 100,   1,    2},
        {   0, 100,  0, 100, 100, 100, 100,  100},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 3x1
    {
        {   0, 100,  0,   3, 100, 100,   1,    3},
        {   0, 100,  1,   3, 100, 100,   1,    3},
        {   0, 100,  2,   3, 100, 100,   1,    3},
        {   0, 100,  0, 100, 100, 100, 100,  100},
    },
    // 4x1
    {
        {   0, 100,  0,   4, 100, 100,   1,    4},
        {   0, 100,  1,   4, 100, 100,   1,    4},
        {   0, 100,  2,   4, 100, 100,   1,    4},
        {   0, 100,  3,   4, 100, 100,   1,    4},
    },

};

void mapp_F1H2TX_V2_GsvFunctionInit(void);
void mapp_F1H2TX_V2_GsvFunctionProcess(void);
void mapp_F1H2TX_V2IDT5VInit(void);
void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID,bool bUpdate);
void mapp_F1H2TX_V2StartupToRecoveryState(void);
void mapp_SourceChangeToSetWinProcess(void);
void report_video_timing(void);
void mapp_F1H2TX_V2_SwitcherFunction(void);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_F1H2TX_V2_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_F1H2TX_V2_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_F1H2TX_V2_PlatformTimerGet(uint8_t eTimer)
{
    return(g_F1H2TX_V2_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_F1H2TX_V2_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_F1H2TX_V2_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_F1H2TX_V2_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_F1H2TX_V2_Timers[i] > 0)
        {
            g_F1H2TX_V2_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_F1H2TX_V2_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_F1H2TX_V2_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void F1H2TX_V2_1msTimerIrq(void)
{
    mapp_F1H2TX_V2_PlatformTimerTask();
}

void F1H2TX_V2_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void F1H2TX_V2_SpecificVarDefault(void)
{
    F1H2TX_V2_SpecBasicVarDefault();
    F1H2TX_V2_SpecCustomEdidVarDefault();
}

void F1H2TX_V2_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;

    pstSpecVar->stBasSpecVar.u8TxFormatSta[0] = emTxFormat_Hdmi;
    pstSpecVar->stBasSpecVar.u8TxResolutionID[0] = emRes_3840x2160_60;
    pstSpecVar->stBasSpecVar.u8TxHdcpMode[0] = 0;
    pstSpecVar->stBasSpecVar.u8InputSrc = 0;
}

void F1H2TX_V2_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
}

bool F1H2TX_V2_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        F1H2TX_V2_SpecBasicVarDefault();
        pstSpecVar->stBasSpecVar.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar));

        isSpecVarValid = FALSE;
    }

    if(pstSpecVar->stCustomEdidPre1.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre1,((uint8_t *)&pstSpecVar->stCustomEdidPre1.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre1)))
    {
        memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
        pstSpecVar->stCustomEdidPre1.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stCustomEdidPre1,((uint8_t *)&pstSpecVar->stCustomEdidPre1.u8CheckSum-(uint8_t *)&pstSpecVar->stCustomEdidPre1));

        isSpecVarValid = FALSE;
    }

    return isSpecVarValid;
}

void SetUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8CheckSum = CRC8Calculate((uint8_t *)pstBasicVar, ((uint8_t *)&pstBasicVar->u8CheckSum-(uint8_t *)pstBasicVar));
    SaveUserDataToEep(eEepDataIndex_SpecBlock1,\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)),\
        (uint8_t *)pstBasicVar, sizeof(stBasicSpecVar_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

void SetUserData_SpecCustomEdid(uint8_t u8CustomEdidIndex)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void F1H2TX_V2_PrjStatePowerOnInit(void)
{
    System_Default_Edid_table = (iTE_u8 *)edid_F1H2TX_V2_default;
    memcpy(g_pu8It663xxEdidBuffer, System_Default_Edid_table,256);

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = F1H2TX_V2_1msTimerIrq;
    pvvFuncGsvUserFunctionInit = mapp_F1H2TX_V2_GsvFunctionInit;
    pvvFuncGsvUserFunctionProcess = mapp_F1H2TX_V2_GsvFunctionProcess;
    pvpmappIT6635XUserFunction = mapp_F1H2TX_V2_SwitcherFunction;
    mapp_RevisionMake();

    mapp_SetTx5VControl(TRUE);
}

void F1H2TX_V2_LedWork_Loop(void)
{
    static bool sl_bLedSta = FALSE;
    if(!mapp_F1H2TX_V2_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_LedActive,500);

    if(sl_bLedSta)
    {
        sl_bLedSta = FALSE;
        led_on(led_name_ActiveLed);
    }
    else
    {
        sl_bLedSta = TRUE;
        led_off(led_name_ActiveLed);
    }
}

void mapp_F1H2TX_V2_Gsv2K11FunctionInit(void)
{
    uint8 i=0;
#if GSV2K11
    uint8 CommonBusConfig = AvGsv2k11ComBusCfg;
#endif

    for(i=0;i<AvGsv2k11DeviceNumber;i++)
    {
        if(i==0)LogicOutputSel[i] = 1;
        else LogicOutputSel[i] = 0;

        if(LogicOutputSel[i] == 1)
        {
            // Rx -> LogicTx/Tx
        //    AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][1], AvConnectAV);
            #if AvEnableInternalVideoGen
            AvApiConnectPort(&gsv2k11Ports[i][6], &gsv2k11Ports[i][2], AvConnectVideo);//
            #else
            AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][2], AvConnectVideo);// 2
            #endif
            AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][3], AvConnectAudio);
            AvApiConnectPort(&gsv2k11Ports[i][0],&gsv2k11Ports[i][1], AvConnectAV);
        }
        /* case 2: routing of LogicTx/Rx->TxB */
        else
        {
            #if AvEnableInternalVideoGen
            AvApiConnectPort(&gsv2k11Ports[i][6], &gsv2k11Ports[i][1], AvConnectAV);//
            #else
            AvApiConnectPort(&gsv2k11Ports[i][7], &gsv2k11Ports[i][1], AvConnectVideo);//7
            #endif
            AvApiConnectPort(&gsv2k11Ports[i][8], &gsv2k11Ports[i][1], AvConnectAudio);
        }

    #if AvEnableCecFeature
        if(AudioStatus == 1)
        {
            AvApiConnectPort(&gsv2k11Ports[i][0], &gsv2k11Ports[i][1], AvConnectAudio);
        }
    #endif
    #if AvEnableInternalVideoGen
        gsv2k11Ports[i][6].content.video->timing.Vic = 0x61; /*10x0 1080p60 */ //0x04;//720p
        gsv2k11Ports[i][6].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
        gsv2k11Ports[i][6].content.video->Cd         = AV_CD_24;
        gsv2k11Ports[i][6].content.video->Y          = AV_Y2Y1Y0_RGB;//
        gsv2k11Ports[i][6].content.vg->Pattern       = AV_PT_CHECKBOARD;//AV_PT_RED_RAMP;//AV_PT_COLOR_BAR;//
    #endif

        //if(LogicOutputSel[i]==0)    // Logic audio RX enable
        {
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
        #endif
        }
        //else     // Logic audio TX enable
        {
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
        }
        /* 3.4.5 Video Parallel Bus Input */
        /* CommonBusConfig = 0 to disable, CommonBusConfig = 1~64 for feature setting */

        gsv2k11Ports[i][2].content.lvtx->Config        = CommonBusConfig;
        /* 3.4.5.1 LogicVideoTx Port's Y and InCS
           = AV_Y2Y1Y0_INVALID/AV_CS_AUTO to do no 2011 color processing,
           = Dedicated Color for internal Color/Scaler Processing */
        gsv2k11Ports[i][2].content.video->Y           = AV_Y2Y1Y0_YCBCR_444;//AV_Y2Y1Y0_INVALID; AV_Y2Y1Y0_YCBCR_422;//
        gsv2k11Ports[i][2].content.video->InCs        = AV_CS_YUV_709;//AV_CS_AUTO;
   //     gsv2k11Ports[i][2].content.video->OutCs        = AV_CS_LIM_RGB;
        /* 3.4.5.2 LogicVideoTx Port's Limited Highest Pixel Clock Frequency
          = 600 to output HDMI 2.0 on Parallel bus,
          = 300 to output HDMI 1.4 on Parallel bus,
          = 150 to output 1080p on Parallel bus */
        gsv2k11Ports[i][2].content.video->info.TmdsFreq   = 600;

        /* 3.4.6 Video Parallel Bus Input */
    //     gsv2k11Ports[7].content.video->timing.Vic  = 0x04; /* 720p60 */
        gsv2k11Ports[i][7].content.video->timing.Vic  = 0x10; /* 1080p60 */
        //gsv2k11Ports[7].content.video->timing.Vic  = 0x61; /* 4K60 */
        gsv2k11Ports[i][7].content.video->AvailableVideoPackets = AV_BIT_AV_INFO_FRAME;
        gsv2k11Ports[i][7].content.video->Cd          = AV_CD_24;
        if((ParallelConfigTable[CommonBusConfig*3 + 1] & 0x04) != 0)
        {
            gsv2k11Ports[i][7].content.video->Y         =  AV_Y2Y1Y0_YCBCR_444;//AV_Y2Y1Y0_YCBCR_422;//
            gsv2k11Ports[i][7].content.video->InCs      =  AV_CS_YUV_709;//AV_CS_AUTO;//AV_CS_LIM_YUV_709;//
            gsv2k11Ports[i][7].content.video->OutCs     = AV_CS_YUV_709;
        }
        else
        {
            gsv2k11Ports[i][7].content.video->Y         = AV_Y2Y1Y0_YCBCR_444;//AV_Y2Y1Y0_RGB;
            gsv2k11Ports[i][7].content.video->InCs      = AV_CS_YUV_709;//AV_CS_RGB;
            gsv2k11Ports[i][7].content.video->OutCs     = AV_CS_YUV_709;
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
#ifdef _4K_MODE
        g_stGsv2k11HdmiOutColorSpace[i].Cd      = AV_CD_24;
        g_stGsv2k11HdmiOutColorSpace[i].Y       = AV_Y2Y1Y0_YCBCR_422;
        g_stGsv2k11HdmiOutColorSpace[i].InCs    = AV_CS_YUV_709;
        g_stGsv2k11HdmiOutColorSpace[i].OutCs   = AV_CS_YUV_709;
        g_stGsv2k11ParamChg[i].u8TxColorSpace   = emTxCsType_YCBCR422;
#else
        g_stGsv2k11HdmiOutColorSpace[i].Cd      = AV_CD_24;
        g_stGsv2k11HdmiOutColorSpace[i].Y       = AV_Y2Y1Y0_YCBCR_444;
        g_stGsv2k11HdmiOutColorSpace[i].InCs    = AV_CS_YUV_709;
        g_stGsv2k11HdmiOutColorSpace[i].OutCs   = AV_CS_YUV_709;
        g_stGsv2k11ParamChg[i].u8TxColorSpace   = emTxCsType_YCBCR444;
#endif

//        mapp_GsvHdcpManage(&gsv2k11Ports[i][1],sys_info.out_hdcp_status[i]);
        //GsvSetTxMode(i,sys_info.out_format[i]==0?1:0);
    }
}


void mapp_F1H2TX_V2_GsvFunctionInit(void)
{
    mapp_F1H2TX_V2_Gsv2K11FunctionInit();

    EdidHdmiMode = AvEdidModeUserDefine;
}

stTimingTable_t g_stGsvRxTiming;

void mapp_F1H2TX_V2_GetColorSpaceFunction(void)
{
    AvPort *l_port = gsv2k11Ports[0];
    static AvVideoY sl_ePreColorSpace = AV_Y2Y1Y0_INVALID;

    if(l_port->content.video->Y!=sl_ePreColorSpace)
    {
        sl_ePreColorSpace = l_port->content.video->Y;
        switch(l_port->content.video->Y)
        {
            case AV_Y2Y1Y0_RGB:
                g_stRxResTable[0].u8ColorSpace = 1;
                break;
            case AV_Y2Y1Y0_YCBCR_420:
            case AV_Y2Y1Y0_YCBCR_422:
                g_stRxResTable[0].u8ColorSpace = 2;
                break;
            case AV_Y2Y1Y0_YCBCR_444:
            default:
                g_stRxResTable[0].u8ColorSpace = 0;
                break;
        }
        g_stRxResTable[0].bChangeFlag = TRUE;
    }
}

void mapp_F1H2TX_V2_GsvFunctionProcess(void)
{
    mapp_GsvGetVideoOutputTiming(gsv2k11Ports[0],&g_stGsvRxTiming);

    if((g_stGsvRxTiming.u16VActive != g_stRxResTable[0].u16Height)
        ||(g_stGsvRxTiming.u16HActive != g_stRxResTable[0].u16Width)
        ||(g_stGsvRxTiming.u8VClk != g_stRxResTable[0].u16VFreq))
    {
        g_stRxResTable[0].bChangeFlag = TRUE;

        g_stRxResTable[0].u8ScanMode = g_stGsvRxTiming.u8Interlace;
        g_stRxResTable[0].u16Width  = g_stGsvRxTiming.u16HActive;
        g_stRxResTable[0].u16Height = g_stGsvRxTiming.u16VActive;
        g_stRxResTable[0].u16VFreq  = g_stGsvRxTiming.u8VClk;
    }
    mapp_F1H2TX_V2_GetColorSpaceFunction();
}

// the status of led when running system
void F1H2TX_V2_FuncWork_LoopRunning(void)
{
    F1H2TX_V2_LedWork_Loop();

    mapp_F1H2TX_V2StartupToRecoveryState();
    mapp_SourceChangeToSetWinProcess();
    report_video_timing();
}

void mapp_F1H2TX_V2StartupMessage(void)
{
    RS232_printf("\r\n");
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    RS232_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
}

#if 0
void mapp_F1H2TX_V2IDT5VInit(void)
{
    uint8_t tmp, timeout=100;
    uint8_t l_u8DevId=0;

    for(l_u8DevId=0;l_u8DevId<1;l_u8DevId++)
    {
        do {
            delay_ms(5);
            i2c_IDT5V_write_one_byte(l_u8DevId,0x00,0x01);    //mode: Software Mode control
            tmp = i2c_IDT5V_read_one_byte(l_u8DevId,0x00);

        } while((tmp != 0x01) && --timeout);

        if(tmp != 0x01)
        {
            //Uart_printf("IDT_5V[%d]_init Fail\r\n",l_u8DevId);
            return;
        }

        i2c_IDT5V_write_one_byte(l_u8DevId,0x01,0x00);    //Select CONFIG0
        i2c_IDT5V_write_one_byte(l_u8DevId,0x02,0x00);    //outputs enabled
        i2c_IDT5V_write_one_byte(l_u8DevId,0x03,0xFF);    //output not suspend

       //0x96
        i2c_IDT5V_write_one_byte(l_u8DevId,0xC0,0x96);    //crystal/REFIN, auto switch mode, PLL3 normal, SRC0: DIV3, SRC1: PLL2
        //PLL0: 148.5M
        IDT_5V_OutProgramClock(l_u8DevId,0,14850);
        //PLL1: 148.5M
        IDT_5V_OutProgramClock(l_u8DevId,1,14850);
        //PLL2: 148.5M
        IDT_5V_OutProgramClock(l_u8DevId,2,14850);
        //PLL3: 100M
        IDT_5V_OutProgramClock(l_u8DevId,3,14850);

            /*SRC6: PLL1 OUT3 OUT6 OUT0
              SRC5: PLL3 OUT5
              SRC4: PLL0 OUT4
              SRC2: PLL2 OUT1 OUT2
            */
        i2c_IDT5V_write_one_byte(l_u8DevId,0xCC,0xBE);    //SRC6: PLL1, SRC5: PLL3, SRC4: PLL0
        i2c_IDT5V_write_one_byte(l_u8DevId,0xC4,0x5D);    //SRC3: PLL1, SRC2: PLL2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x75,0x00);    //S3=0 - Both from DIV6, S1=0 - Both from DIV2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x7A,0x05);    //OUT5 and OUT5b: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x79,0x05);    //OUT4 and OUT4b: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x78,0x05);    //OUT3 and OUT6: invert clock, LVDS
        i2c_IDT5V_write_one_byte(l_u8DevId,0x76,0x05);    //OUT1 and OUT2: invert clock, LVDS
        //DIV1~6: /2
        i2c_IDT5V_write_one_byte(l_u8DevId,0x88,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x90,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x94,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0x9C,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0xA0,0x0F);
        i2c_IDT5V_write_one_byte(l_u8DevId,0xA8,0x0F);
    }
}
#endif

//================================================================================
// FPGA config
//================================================================================
void mapp_F1H2TX_V2_SendDataToFpag(pu8 pu8Data, u16 u16Len)
{
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;
    pUartCommVar->SendBytes(pu8Data,u16Len,FALSE);
    {
        pstUartCom_t pUart1CommVar = &g_stUart2CommVar;
        pUart1CommVar->SendBytes(pu8Data,u16Len,FALSE);
    }
}

/******************************************************
///Function Name:   send_cmd_to_fpga
///Description:     Send command to FPGA
///Input:           u8Addr(FPGA控制地址),u8Cmd(命令码),
///                 u16Len(数据长度),pu8Data(控制数据)
///Output:          NULL
///Return:          NULL
///Others:          NULL
******************************************************/
void send_cmd_to_fpga(uint8_t u8Addr,uint8_t u8Cmd,uint8_t* pu8Data,uint16_t u16Len)
{
    uint8_t l_u8Buff[100]={0xff,0xff,0xa5,0xfe,0xcc,0x01,0x00,0xaa};

    if(u16Len>99)return ;
    l_u8Buff[3]=u8Addr;
    l_u8Buff[4]=u8Cmd;

    l_u8Buff[5]=u16Len;
    l_u8Buff[6]=u16Len>>8;
    if(pu8Data!=NULL)
    {
        memcpy(l_u8Buff+7,pu8Data,u16Len);    // head
    }

    mapp_F1H2TX_V2_SendDataToFpag(l_u8Buff,7+u16Len);
}

void mapp_F1H2TX_V2_SetAudioSource(u8 u8Out, u8 u8AudioSrc)
{
    u8 l_u8Temp[2];

//    l_u8Temp[0] = u8AudioSrc;
//    send_cmd_to_fpga(u8Out,eFpgaRegAudioSwitch,l_u8Temp,1);
    l_u8Temp[0] = u8Out;
    l_u8Temp[1] = u8AudioSrc;
    send_cmd_to_fpga(0xFE,eFpgaRegAudioSwitch,l_u8Temp,2);
}

void mapp_F1H2TX_V2WinDataCopy(u8 *u8Data, pstWindowsConfig_t pstWincfgDat)
{
    u8Data[0]  = pstWincfgDat->u8SrcAddr;
    u8Data[1]  = pstWincfgDat->u8LayerID;
    u8Data[2]  = pstWincfgDat->u8Layer_D0;
    u8Data[3]  = pstWincfgDat->u8Layer_D1;
    u8Data[4]  = pstWincfgDat->u8Layer_D2;
    u8Data[5]  = pstWincfgDat->u8Layer_D3;
    u8Data[6]  = pstWincfgDat->u16SrcAct_H&0xFF;
    u8Data[7]  = (pstWincfgDat->u16SrcAct_H>>8)&0xFF;
    u8Data[8]  = pstWincfgDat->u16SrcAct_V&0xFF;
    u8Data[9]  = (pstWincfgDat->u16SrcAct_V>>8)&0xFF;
    u8Data[10]  = pstWincfgDat->u16Src_X&0xFF;
    u8Data[11]  = (pstWincfgDat->u16Src_X>>8)&0xFF;
    u8Data[12]  = pstWincfgDat->u16Src_Y&0xFF;
    u8Data[13]  = (pstWincfgDat->u16Src_Y>>8)&0xFF;
    u8Data[14]  = pstWincfgDat->u16Src_H&0xFF;
    u8Data[15]  = (pstWincfgDat->u16Src_H>>8)&0xFF;
    u8Data[16]  = pstWincfgDat->u16Src_V&0xFF;
    u8Data[17]  = (pstWincfgDat->u16Src_V>>8)&0xFF;

    u8Data[18]  = pstWincfgDat->u32Wnd_X&0xFF;
    u8Data[19]  = (pstWincfgDat->u32Wnd_X>>8)&0xFF;
    u8Data[20]  = (pstWincfgDat->u32Wnd_X>>16)&0xFF;
    u8Data[21]  = (pstWincfgDat->u32Wnd_X>>24)&0xFF;
    u8Data[22]  = pstWincfgDat->u32Wnd_Y&0xFF;
    u8Data[23]  = (pstWincfgDat->u32Wnd_Y>>8)&0xFF;
    u8Data[24]  = (pstWincfgDat->u32Wnd_Y>>16)&0xFF;
    u8Data[25]  = (pstWincfgDat->u32Wnd_Y>>24)&0xFF;

    u8Data[26]  = pstWincfgDat->u32Wnd_H&0xFF;
    u8Data[27]  = (pstWincfgDat->u32Wnd_H>>8)&0xFF;
    u8Data[28]  = (pstWincfgDat->u32Wnd_H>>16)&0xFF;
    u8Data[29]  = (pstWincfgDat->u32Wnd_H>>24)&0xFF;
    u8Data[30]  = pstWincfgDat->u32Wnd_V&0xFF;
    u8Data[31]  = (pstWincfgDat->u32Wnd_V>>8)&0xFF;
    u8Data[32]  = (pstWincfgDat->u32Wnd_V>>16)&0xFF;
    u8Data[33]  = (pstWincfgDat->u32Wnd_V>>24)&0xFF;

    u8Data[34]  = pstWincfgDat->u32Srf_H&0xFF;
    u8Data[35]  = (pstWincfgDat->u32Srf_H>>8)&0xFF;
    u8Data[36]  = (pstWincfgDat->u32Srf_H>>16)&0xFF;
    u8Data[37]  = (pstWincfgDat->u32Srf_H>>24)&0xFF;
    u8Data[38]  = pstWincfgDat->u32Srf_V&0xFF;
    u8Data[39]  = (pstWincfgDat->u32Srf_V>>8)&0xFF;
    u8Data[40]  = (pstWincfgDat->u32Srf_V>>16)&0xFF;
    u8Data[41]  = (pstWincfgDat->u32Srf_V>>24)&0xFF;
    u8Data[42]  = pstWincfgDat->u8SrcInput;
}

u16 mapp_F1H2TX_V2GetClipWinStartX(u8 u8Type,u8 u8Output,u16 u16Width)
{
    u16 l_u16Temp = 0;

    if(cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8StX_sf2 == 0)
    {
        l_u16Temp = 0;
    }
    else
    {
        l_u16Temp = (cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8StX_sf1*u16Width)
                    /cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8StX_sf2;
    }

    if(l_u16Temp > u16Width)
    {
        l_u16Temp = u16Width;
    }

    return l_u16Temp;
}

u16 mapp_F1H2TX_V2GetClipWinStartY(u8 u8Type,u8 u8Output,u16 u16Height)
{
    u16 l_u16Temp = 0;

    if(cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8StY_sf2 == 0)
    {
        l_u16Temp = 0;
    }
    else
    {
        l_u16Temp = (cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8StY_sf1*u16Height)
                    /cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8StY_sf2;
    }

    if(l_u16Temp > u16Height)
    {
        l_u16Temp = u16Height;
    }

    return l_u16Temp;
}

u16 mapp_F1H2TX_V2GetClipWinWidth(u8 u8Type,u8 u8Output,u16 u16Width)
{
    u16 l_u16Temp = u16Width;

    if(cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8Width_sf2 == 0)
    {
        l_u16Temp = u16Width;
    }
    else
    {
        l_u16Temp = (cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8Width_sf1*u16Width)
                    /cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8Width_sf2;
    }

    if(l_u16Temp%4)
    {
        l_u16Temp -= l_u16Temp%4;
    }

    if(l_u16Temp > u16Width)
    {
        l_u16Temp = u16Width;
    }

    return l_u16Temp;
}

u16 mapp_F1H2TX_V2GetClipWinHeight(u8 u8Type,u8 u8Output,u16 u16Height)
{
    u16 l_u16Temp = u16Height;

    if(cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8Height_sf2 == 0)
    {
        l_u16Temp = u16Height;
    }
    else
    {
        l_u16Temp = (cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8Height_sf1*u16Height)
                    /cg_stF1H2TX_V2ClipWinFactoryType_t[u8Type][u8Output].u8Height_sf2;
    }

    return l_u16Temp;
}

void mapp_F1H2TX_V2WindowCfg(uint8_t out_ch,uint8_t in_ch)
{
//    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
//    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Temp[50];
    stWindowsConfig_t stWinConfig;
    uint8_t layer_map[]={0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03,0x01,0x03};
    u8 l_u8Mode = 0;

    stWinConfig.u8SrcAddr = 0;
    stWinConfig.u8LayerID = layer_map[out_ch-1];
    stWinConfig.u8Layer_D0 = 0x00;
    stWinConfig.u8Layer_D1 = 0x00;
    stWinConfig.u8Layer_D2 = 0x00;
    stWinConfig.u8Layer_D3 = 0x00;
    switch(stWinConfig.u8LayerID)
    {
        case 0x01:
            stWinConfig.u8Layer_D0 = in_ch?0x80:0x00;
            break;
        case 0x02:
            stWinConfig.u8Layer_D1 = in_ch?0x80:0x00;
            break;
        case 0x03:
            stWinConfig.u8Layer_D2 = in_ch?0x80:0x00;
            break;
        case 0x04:
            stWinConfig.u8Layer_D3 = in_ch?0x80:0x00;
            break;
        default:
            break;
    }
    stWinConfig.u16SrcAct_H = (g_stRxResTable[in_ch-1].u16Width)?(g_stRxResTable[in_ch-1].u16Width):1920;
    stWinConfig.u16SrcAct_V = (g_stRxResTable[in_ch-1].u16Height)?(g_stRxResTable[in_ch-1].u16Height):1080;
//    stWinConfig.u16Src_X    = g_stWinConfigVar[out_ch-1].stClipWin.u16StartX;
//    stWinConfig.u16Src_Y    = g_stWinConfigVar[out_ch-1].stClipWin.u16StartY;
//    stWinConfig.u16Src_H    = (g_stWinConfigVar[out_ch-1].stClipWin.u16Width)?(g_stWinConfigVar[out_ch-1].stClipWin.u16Width):(stWinConfig.u16SrcAct_H);
//    stWinConfig.u16Src_V    = (g_stWinConfigVar[out_ch-1].stClipWin.u16Height)?(g_stWinConfigVar[out_ch-1].stClipWin.u16Height):(stWinConfig.u16SrcAct_V);
    stWinConfig.u16Src_X    = mapp_F1H2TX_V2GetClipWinStartX(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_H);
    stWinConfig.u16Src_Y    = mapp_F1H2TX_V2GetClipWinStartY(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_V);
    stWinConfig.u16Src_H    = mapp_F1H2TX_V2GetClipWinWidth(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_H);
    stWinConfig.u16Src_V    = mapp_F1H2TX_V2GetClipWinHeight(l_u8Mode,out_ch-1,stWinConfig.u16SrcAct_V);
    stWinConfig.u32Wnd_X    = 0;
    stWinConfig.u32Wnd_Y    = 0;
    stWinConfig.u32Wnd_H    = g_stWinConfigVar[out_ch-1].stDstWin.u16Width;
    stWinConfig.u32Wnd_V    = g_stWinConfigVar[out_ch-1].stDstWin.u16Height;
    stWinConfig.u32Srf_H    = (((uint64_t)stWinConfig.u16Src_H<<16))/(uint64_t)stWinConfig.u32Wnd_H;
    stWinConfig.u32Srf_V    = (((uint64_t)stWinConfig.u16Src_V<<16))/(uint64_t)stWinConfig.u32Wnd_V;
    stWinConfig.u8SrcInput  = in_ch;

//    dbg_printf("data size  == %d \r\n",sizeof(stWindowsConfig_t));

    mapp_F1H2TX_V2WinDataCopy(l_u8Temp,&stWinConfig);
    send_cmd_to_fpga(out_ch,eFpgaRegOutWinCfg,l_u8Temp,43);

    return;
}

void mapp_F1H2TX_V2FpgaWinCmdUpdate(u32 u32DelayMs)
{
    u8 l_u8cmd = 1;
    delay_ms_cpu(u32DelayMs);
    send_cmd_to_fpga(0xFE,eFpgaRegOutUpdate,&l_u8cmd,1);
}

void mapp_F1H2TX_V2_TimingConfig(uint8_t ch,uint8_t idx)
{
    u8 l_u8Temp[50];
//    if(/*idx==0||*/idx>=(sizeof(res_table)/sizeof(res_table[0])))
//        return ;
    memset(l_u8Temp,0,50);

//    dbg_printf("u16HActive  == %d \r\n",res_table[idx].u16HActive);
//    dbg_printf("u16HTotal   == %d \r\n",res_table[idx].u16HTotal);
//    dbg_printf("u16HBP      == %d \r\n",res_table[idx].u16HBP);
//    dbg_printf("u16HFP      == %d \r\n",res_table[idx].u16HFP);
//    dbg_printf("u8HSync     == %d \r\n",res_table[idx].u16HSync);
//    dbg_printf("u8HPol      == %d \r\n",res_table[idx].u8HPol);
//    dbg_printf("u16VActive  == %d \r\n",res_table[idx].u16VActive);
//    dbg_printf("u16VTotal   == %d \r\n",res_table[idx].u16VTotal);
//    dbg_printf("u16VBP      == %d \r\n",res_table[idx].u16VBP);
//    dbg_printf("u16VFP      == %d \r\n",res_table[idx].u16VFP);
//    dbg_printf("u8VSync     == %d \r\n",res_table[idx].u16VSync);
//    dbg_printf("u8VPol      == %d \r\n",res_table[idx].u8VPol);
//    dbg_printf("u8VClk      == %d \r\n",res_table[idx].u8VClk);
//    dbg_printf("u16PClk     == %d \r\n",res_table[idx].u16PClk);
//    dbg_printf("u8Vic       == %d \r\n",res_table[idx].u8Vic);
    l_u8Temp[0]  = res_table[idx].u16HActive&0xFF;
    l_u8Temp[1]  = (res_table[idx].u16HActive>>8)&0xFF;
    l_u8Temp[2]  = res_table[idx].u16HTotal&0xFF;
    l_u8Temp[3]  = (res_table[idx].u16HTotal>>8)&0xFF;
    l_u8Temp[4]  = res_table[idx].u16HBP&0xFF;
    l_u8Temp[5]  = (res_table[idx].u16HBP>>8)&0xFF;
    l_u8Temp[6]  = res_table[idx].u16HFP&0xFF;
    l_u8Temp[7]  = (res_table[idx].u16HFP>>8)&0xFF;
    l_u8Temp[8]  = res_table[idx].u16HSync&0xFF;
    l_u8Temp[9]  = (res_table[idx].u16HSync>>8)&0xFF;
    l_u8Temp[10] = res_table[idx].u8HPol&0xFF;
    l_u8Temp[11] = res_table[idx].u16VActive&0xFF;
    l_u8Temp[12] = (res_table[idx].u16VActive>>8)&0xFF;
    l_u8Temp[13] = res_table[idx].u16VTotal&0xFF;
    l_u8Temp[14] = (res_table[idx].u16VTotal>>8)&0xFF;
    l_u8Temp[15] = res_table[idx].u16VBP&0xFF;
    l_u8Temp[16] = (res_table[idx].u16VBP>>8)&0xFF;
    l_u8Temp[17] = res_table[idx].u16VFP&0xFF;
    l_u8Temp[18] = (res_table[idx].u16VFP>>8)&0xFF;
    l_u8Temp[19] = res_table[idx].u16VSync&0xFF;
    l_u8Temp[20] = (res_table[idx].u16VSync>>8)&0xFF;
    l_u8Temp[21] = res_table[idx].u8VPol&0xFF;

    send_cmd_to_fpga(ch,eFpgaRegOutTiming,l_u8Temp,22);
}

void mapp_F1H2TX_V2ResolutionConfig(u8 u8Out,u8 u8ResID,bool bUpdate)
{
    const u8 cl_u8ChMap[1] = {3};
    mapp_F1H2TX_V2_TimingConfig(u8Out+1,u8ResID);
#if _ENABLE_CHIP_IDT5P49V6965
    mapp_IDTSetPclk(u8Out/8,(res_table[u8ResID].u16PClk/4),cl_u8ChMap[u8Out]);
#endif
    SwitchResolutionToRefreshWindowns(u8Out,u8ResID,bUpdate);
}

void SwitchResolutionToRefreshWindowns(u8 u8Out,u8 u8ResID,bool bUpdate)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    g_stWinConfigVar[0].stDstWin.u16Width = res_table[u8ResID].u16HActive;
    g_stWinConfigVar[0].stDstWin.u16Height= res_table[u8ResID].u16VActive;
    mapp_GsvSetTimingChange(1);
    mapp_GsvSetTimingID(1,u8ResID);
    mapp_F1H2TX_V2WindowCfg(1,pstBasicSpecVar->u8InputSrc+1);
    pstBasicSpecVar->u8TxResolutionID[0]=u8ResID;

    if(bUpdate)
    {
        mapp_F1H2TX_V2FpgaWinCmdUpdate(10);
    }
}

void mapp_F1H2TX_V2SourceChange(u8 u8Out,u8 u8In)
{
    mapp_F1H2TX_V2WindowCfg(u8Out,u8In);
    mapp_F1H2TX_V2_SetAudioSource(u8Out,u8In);
}

void mapp_GetFpgaVersion(u8 u8Addr)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8Data = 0;

    l_u8Addr = u8Addr;
    l_u8Cmd = eFpgaRegGetVersion;
    send_cmd_to_fpga(l_u8Addr,l_u8Cmd,&l_u8Data,1);
}

void mapp_SetFreezeTime(u16 u16Time)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8Data[2];

    l_u8Addr = 0xFE;
    l_u8Cmd = 0x55;
    l_u8Data[0] = u16Time&0xff;
    l_u8Data[1] = (u16Time>>8)&0xff;
    send_cmd_to_fpga(l_u8Addr,l_u8Cmd,l_u8Data,2);
}

void mapp_SetFreeze(u8 u8FreezeSta)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8Data[1];

    l_u8Addr = 0xFE;
    l_u8Cmd = 0x56;
    l_u8Data[0] = u8FreezeSta&0xff;
    send_cmd_to_fpga(l_u8Addr,l_u8Cmd,l_u8Data,1);
}

#define NUM_LOOP    10
void mapp_F1H2TX_V2StartupToRecoveryState(void)
{
    static bool sl_bStartupFlag = TRUE;
    static eStartupLoopType_t sl_eStartupLoop = eSt_None;
    static u8 sl_u8LoopCnt = 0;

    if(!mapp_F1H2TX_V2_PlatformTimerExpired(emUserTimer_StartupTime))
        return;

    if(sl_bStartupFlag)
    {
        sl_bStartupFlag = FALSE;
        sl_eStartupLoop = eSt_SInit;
    }

    switch(sl_eStartupLoop)
    {
        case eSt_SInit:
            {
                sl_eStartupLoop++;
            }
            break;
        case eSt_GetTxVersion:
            {
                dbg_printf("Get Tx Version\r\n");
                mapp_GetFpgaVersion(1);
                delay_ms(50);
                mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_StartupTime,200);
                sl_u8LoopCnt++;
                if(g_u8Fwversion||(sl_u8LoopCnt>NUM_LOOP))
                {
                    sl_eStartupLoop++;
                    sl_u8LoopCnt = 0;
                }
            }
            break;
        case eSt_SetTxResolution:
            {
                pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
                pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
                u8 i;

                dbg_printf("Set Tx Resolution\r\n");
                for(i=0;i<Def_outport_num;i++)
                {
                    mapp_F1H2TX_V2ResolutionConfig(i,pstBasicVar->u8TxResolutionID[i],(i==(Def_outport_num-1))?TRUE:FALSE);
                }
                delay_ms(50);
                mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_StartupTime,500);
                mapp_SetFreezeTime(1);
                mapp_SetFreeze(0);
                sl_eStartupLoop++;
            }
            break;
        case eSt_None:
        default:
            break;
    }
}

bool g_bReportTimingFlag[2] = {FALSE,FALSE};

#define DET_INPUT_CNT   100
#define SRC_CHG_LOOP_CNT    10
void mapp_SourceChangeToSetWinProcess(void)
{
    u8 l_u8Src,l_u8OutIdex;
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    bool l_bUpdateFlag = FALSE;
    static u8 sl_u8SrcChgCnt[2] = {0,0};
    static u16 sl_u16SrcLossCnt[2] = {0,0};

    if(!mapp_F1H2TX_V2_PlatformTimerExpired(emUserTimer_TimingChange))
        return;
    mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_TimingChange,SRC_CHG_LOOP_CNT);

    for(l_u8Src = 0;l_u8Src<_HDMI_INPUT_PORT_USED_NUMBER;l_u8Src++)
    {
        {
            if(g_stRxResTable[l_u8Src].bChangeFlag)
                sl_u8SrcChgCnt[l_u8Src]++;
            if((sl_u8SrcChgCnt[l_u8Src]>=DET_INPUT_CNT)
                ||((g_stRxResTable[l_u8Src].u16Height==0)&&(g_stRxResTable[l_u8Src].bChangeFlag)))
            //if(g_stRxResTable[l_u8Src].bChangeFlag)
            {
                sl_u8SrcChgCnt[l_u8Src] = 0;
                for(l_u8OutIdex = 0; l_u8OutIdex<Def_outport_num;l_u8OutIdex++)
                {
                    if(pstBasicSpecVar->u8InputSrc == (l_u8Src))
                    {
                        dbg_printf("source[%d] change to reconfig out[%d]\r\n",l_u8Src,l_u8OutIdex);
                        l_bUpdateFlag = TRUE;
                        mapp_F1H2TX_V2WindowCfg(l_u8OutIdex+1,(l_u8Src+1));
                        mapp_F1H2TX_V2_SetAudioSource(l_u8OutIdex+1,(l_u8Src+1));
//                        g_u8BlackScreenSta = 0;
                    }
                }
//                g_stRxResTable[l_u8Src].bChangeFlag = FALSE;
//                g_bReportTimingFlag[l_u8Src] = TRUE;
//                mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_ReportTiming,0);
//                report_video_timing();
                if(l_bUpdateFlag)
                {
                    mapp_F1H2TX_V2FpgaWinCmdUpdate(10);
                }
                g_stRxResTable[l_u8Src].bChangeFlag = FALSE;
                g_bReportTimingFlag[l_u8Src] = TRUE;
                mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_ReportTiming,1);
            }
        }

        // detect sync loss to set black screen
#if 1
        if((g_stRxResTable[l_u8Src].u16Width==0)&&(g_stRxResTable[l_u8Src].u16Height==0))
        {
            sl_u16SrcLossCnt[l_u8Src]++;
            if(sl_u16SrcLossCnt[l_u8Src] >= (5000/SRC_CHG_LOOP_CNT))
            {
                if(pstBasicSpecVar->u8InputSrc == (l_u8Src))
                {
                    if(g_u8BlackScreenSta==1)
                    {
                        g_u8BlackScreenSta = 0;
                        mapp_SetFreeze(0);
                    }
                }
                sl_u16SrcLossCnt[l_u8Src] = 0;
            }
        }
        else
        {
            sl_u16SrcLossCnt[l_u8Src] = 0;
        }
#endif
    }
}

#define REPORT_TIMING_TIME  500
void report_video_timing(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TX_V2SpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    uint8_t buff[20];
    uint16_t i,j;
    u16 l_u16HActive,l_u16VActive;
    u8  l_u8Vfreq=0;

    if(!mapp_F1H2TX_V2_PlatformTimerExpired(emUserTimer_ReportTiming))
        return;
    mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_ReportTiming,REPORT_TIMING_TIME);

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        {
            if(g_stRxResTable[i].bChangeFlag)
                continue;
            if(g_bReportTimingFlag[i]==FALSE)
                continue;
            g_bReportTimingFlag[i] = FALSE;
            l_u8Vfreq = g_stGsvRxTiming.u8VClk;
            l_u16HActive = g_stGsvRxTiming.u16HActive;
            l_u16VActive = g_stGsvRxTiming.u16VActive;
            g_stRxResTable[i].u8ScanMode = g_stGsvRxTiming.u8Interlace;
            g_stRxResTable[i].u16Width  = l_u16HActive;
            g_stRxResTable[i].u16Height = l_u16VActive;
            g_stRxResTable[i].u16VFreq  = l_u8Vfreq;
        }

        if(pstBasicSpecVar->u8InputSrc == (i))
        {
            if((g_stRxResTable[i].u16Width==0)&&(g_stRxResTable[i].u16Height==0))
            {
                if(g_u8BlackScreenSta==0)
                {
                    g_u8BlackScreenSta = 1;
                    mapp_SetFreeze(1);
                }
            }
            else
            {
                if(g_u8BlackScreenSta==1)
                {
                    g_u8BlackScreenSta = 0;
                    mapp_SetFreeze(0);
                }
            }
        }

        j=0;
        buff[j++] = i+1; //ch
        buff[j++] = l_u16HActive >> 8;
        buff[j++] = l_u16HActive & 0xFF;

        buff[j++] = l_u16VActive >> 8;
        buff[j++] = l_u16VActive & 0xFF;
        if((l_u16HActive == 0)&&(l_u16VActive == 0))
            buff[j++] = 0;
        else
            buff[j++] = g_stRxResTable[0].u8ColorSpace;

//        buff[j++] = g_u8BlackScreenSta;

        send_cmd_to_fpga(0xFE,eFpgaRegReportInputTiming,buff,6);
    }
}

u8 g_u8SourceTable[][2]=
{
    {emSrcInput_Hdmi1,0},
    {emSrcInput_Hdmi2,1},
//    {emSrcInput_Hdmi3,2},
};

extern u8 dev_state;

//#define F1H2TX_SIGNAL_EXIST_DETECT_TIMES       20
//#define F1H2TX_POWER_ON_DETECT_TIMES           5
void mapp_F1H2TX_V2_AutoSwitcheProcess(void)
{
    u8              i, j, rx_signal_sta = 0, cur_temp = 0;
    u8              u8RxTmp=0;
    u8              rx_hotplug_sta = 0;
    static u8       pre_rx_signal_sta = 0;
    static bool     pre_isAutoSwitchMode = TRUE;

    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    //pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;
    //pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(!mapp_F1H2TX_V2_PlatformTimerExpired(emUserTimer_AutoSwitch))        // unit 10ms
    {
        return;
    }

    mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_AutoSwitch,_TIME_AUTO_SWITCH_DETECT);

    for(i=0; i<emSrcInput_Max; i++)
    {
        if(it6635_is_5v_present(i) == 1)
        {
            cur_temp = it6635_is_clock_detected(i);
        }
        u8RxTmp |= cur_temp<<i;
        cur_temp = 0;
    }
    for(i=0; i<emSrcInput_Max; i++)
    {
        if(it6635_is_5v_present(i))
        {
            rx_hotplug_sta |= (1<<g_u8SourceTable[i][1]);
        }

        if(rx_hotplug_sta&(1<<g_u8SourceTable[i][1]))
        {
            rx_signal_sta |= (1<<g_u8SourceTable[i][1]);
        }
    }
//    g_u8InoutPortSta = rx_signal_sta;

    //Dbg_UserFun(("Rx port signal status is %d\r\n", rx_signal_sta));

    u8RxTmp = pstPrjSaveVar->stHdmiSel.u8SwChannel;

    if(!pstPrjSaveVar->stHdmiSel.bAutoSwitch) // isn't the auto switch mode
    {
        pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
        pre_rx_signal_sta = rx_signal_sta;
        return;
    }

    if((!pre_isAutoSwitchMode)&&(pstPrjSaveVar->stHdmiSel.bAutoSwitch))
    {
        pre_isAutoSwitchMode = pstPrjSaveVar->stHdmiSel.bAutoSwitch;
        pre_rx_signal_sta = 0;
    }

    if(pre_rx_signal_sta != rx_signal_sta)
    {
        if((g_bF1H2TX_V2AutoSwitchFirstStatus)&&(rx_signal_sta&(1<<u8RxTmp)))
        {
            g_bF1H2TX_V2AutoSwitchFirstStatus = FALSE;
        }
        else
        {
            g_bF1H2TX_V2AutoSwitchFirstStatus = FALSE;
            cur_temp = pre_rx_signal_sta ^ rx_signal_sta;
            //Uart2_printf("rx signal change, rx_signal_sta = 0x%2x, cur_temp = 0x%2x\r\n", rx_signal_sta, cur_temp);
            for(i=0; i<emSrcInput_Max; i++)
            {
                if(cur_temp & (1 << i))
                {
                    if(rx_signal_sta & (1<<i))
                    {
                        //it6635_set_active_port(g_u8SourceTable[i][1]);
                        mapp_F1H2TX_V2_SetPort(i);
                        pstPrjSaveVar->stHdmiSel.u8SwChannel = i;
                        Uart_printf("Switch to HDMI %d\r\n",i+1);
                        pre_rx_signal_sta = rx_signal_sta;
                        return;
                    }
                    else
                    {
                        if(i == u8RxTmp)
                        {
                            //iTE_MsgA(("signal missed, switch to next:"));
                            for(j=0; j<emSrcInput_Max; j++)
                            {
                                if(rx_signal_sta & (1<<j))
                                {
                                    //it6635_set_active_port(g_u8SourceTable[j][1]);
                                    mapp_F1H2TX_V2_SetPort(j);
                                    pstPrjSaveVar->stHdmiSel.u8SwChannel = j;
                                    Uart_printf("Switch to HDMI %d\r\n",i+1);
                                    pre_rx_signal_sta = rx_signal_sta;
                                    return;
                                }
                            }

                            if(j>=emSrcInput_Max)
                            {
//                                mapp_SetRst66353Control(FALSE);
//                                delay_us(1000000);
//                                mapp_SetRst66353Control(TRUE);
//                                it6635_init();
//                                dev_state = DEV_FW_VAR_INIT;
//                                Uart_printf("No input reset it66353\r\n");
                                Uart_printf("it66353 No input to replug rxhpd\r\n");
                            }
                        }
                    }
                }
            }
        }
        pre_rx_signal_sta = rx_signal_sta;
    }
}
#define SW_DELAY_TIME       1000

extern void _tx_power_down(void);
extern void _tx_power_on(void);

void mapp_F1H2TX_V2_SwitchPortProcess(void)
{
    if(!mapp_F1H2TX_V2_PlatformTimerExpired(emUserTimer_SwitchPortTime))
    {
        return;
    }
    if((g_u8ActiveSource != g_u8PreActiveSource)||g_bSwitchPortFlag)
    {
        g_u8PreActiveSource = g_u8ActiveSource;
        g_bSwitchPortFlag = FALSE;

        it6635_toggle_hpd(SW_DELAY_TIME);
        _tx_power_down();
        delay_ms(SW_DELAY_TIME);
        it6635_set_active_port(g_u8SourceTable[g_u8ActiveSource][1]);
        //it6635_set_rx_hpd(1);

//        g_bF1H2TXGsv2k1SwitchFlag = TRUE;
        _tx_power_on();
        DBG_USER_FUN("Active port = P%d\r\n",g_u8SourceTable[g_u8ActiveSource][1]);
    }
}

void mapp_F1H2TX_V2_SetPort(u8 u8Port)
{
    g_u8ActiveSource = u8Port;
    //g_bSwitchPortFlag = TRUE;
//    it6635_set_active_port(2);

    mapp_F1H2TX_V2_PlatformTimerSet(emUserTimer_SwitchPortTime,500);
}

u8 mapp_F1H2TX_V2_GetActivePort(void)
{
    return g_u8ActiveSource;
}

void mapp_F1H2TX_V2_SwitcherFunction(void)
{
    mapp_F1H2TX_V2_AutoSwitcheProcess();
    mapp_F1H2TX_V2_SwitchPortProcess();
}


