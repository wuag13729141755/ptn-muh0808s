#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

stProjectSpecVar_t  stF1H2TXSpecVar;

bool g_bF1H2TXAutoSwitchFirstStatus = TRUE;
static volatile u32 g_F1H2TX_Timers[emUserTimer_Max];
u8 g_u8ActiveSource = 0;
u8 g_u8PreActiveSource = 0;
bool g_bSwitchPortFlag = FALSE;
extern AvEdidMode EdidHdmiMode;
bool g_bF1H2TXGsv2k1SwitchFlag = FALSE;

extern stProjectGlobalAllVar_t g_stPrjGloVar;

const uint8_t edid_F1H2TX_default[256] = // default edid
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

void mapp_F1H2TX_GsvFunctionInit(void);
void mapp_F1H2TX_GsvFunctionProcess(void);
void mapp_F1H2TX_SwitcherFunction(void);
void mapp_F1H2TX_SetPort(u8 u8Port);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_F1H2TX_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_F1H2TX_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_F1H2TX_PlatformTimerGet(uint8_t eTimer)
{
    return(g_F1H2TX_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_F1H2TX_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_F1H2TX_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_F1H2TX_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_F1H2TX_Timers[i] > 0)
        {
            g_F1H2TX_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_F1H2TX_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_F1H2TX_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void F1H2TX_1msTimerIrq(void)
{
    mapp_F1H2TX_PlatformTimerTask();
}

void F1H2TX_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void F1H2TX_SpecificVarDefault(void)
{
    F1H2TX_SpecBasicVarDefault();
    F1H2TX_SpecCustomEdidVarDefault();
}

void F1H2TX_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
}

void F1H2TX_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
}

bool F1H2TX_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        F1H2TX_SpecBasicVarDefault();
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
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stF1H2TXSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void F1H2TX_PrjStatePowerOnInit(void)
{
    System_Default_Edid_table = (iTE_u8 *)edid_F1H2TX_default;
    memcpy(g_pu8It663xxEdidBuffer, System_Default_Edid_table,256);

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = F1H2TX_1msTimerIrq;
    pvvFuncGsvUserFunctionInit = mapp_F1H2TX_GsvFunctionInit;
    pvvFuncGsvUserFunctionProcess = mapp_F1H2TX_GsvFunctionProcess;
    pvpmappIT6635XUserFunction = mapp_F1H2TX_SwitcherFunction;
}

void F1H2TX_LedWork_Loop(void)
{
    static bool sl_bLedSta = FALSE;
    if(!mapp_F1H2TX_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_F1H2TX_PlatformTimerSet(emUserTimer_LedActive,500);

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

void mapp_F1H2TX_Gsv2K1FunctionInit(void)
{
    EdidHdmiMode = AvEdidModeUserDefine;
}


void mapp_F1H2TX_GsvFunctionInit(void)
{
    mapp_F1H2TX_Gsv2K1FunctionInit();
}

void mapp_F1H2TX_GsvFunctionProcess(void)
{
    if(g_bF1H2TXGsv2k1SwitchFlag)
    {
        u8 i;
        for(i=0;i<AvGsv2k1DeviceNumber;i++)
        {
            KfunPrHpdDown(&gsv2k1Ports[i][0]);
            delay_ms(200);
            KfunPrHpdUp(&gsv2k1Ports[i][0]);
        }
        g_bF1H2TXGsv2k1SwitchFlag = FALSE;
    }
}

// the status of led when running system
void F1H2TX_FuncWork_LoopRunning(void)
{
    F1H2TX_LedWork_Loop();
}

void mapp_F1H2TXStartupMessage(void)
{
    RS232_printf("\r\n");
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    RS232_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
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
void mapp_F1H2TX_AutoSwitcheProcess(void)
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

    if(!mapp_F1H2TX_PlatformTimerExpired(emUserTimer_AutoSwitch))        // unit 10ms
    {
        return;
    }

    mapp_F1H2TX_PlatformTimerSet(emUserTimer_AutoSwitch,_TIME_AUTO_SWITCH_DETECT);

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
        if((g_bF1H2TXAutoSwitchFirstStatus)&&(rx_signal_sta&(1<<u8RxTmp)))
        {
            g_bF1H2TXAutoSwitchFirstStatus = FALSE;
        }
        else
        {
            g_bF1H2TXAutoSwitchFirstStatus = FALSE;
            cur_temp = pre_rx_signal_sta ^ rx_signal_sta;
            //Uart2_printf("rx signal change, rx_signal_sta = 0x%2x, cur_temp = 0x%2x\r\n", rx_signal_sta, cur_temp);
            for(i=0; i<emSrcInput_Max; i++)
            {
                if(cur_temp & (1 << i))
                {
                    if(rx_signal_sta & (1<<i))
                    {
                        //it6635_set_active_port(g_u8SourceTable[i][1]);
                        mapp_F1H2TX_SetPort(i);
                        pstPrjSaveVar->stHdmiSel.u8SwChannel = i;
                        Uart_printf("Switch to HDMI %d\r\n",i+1);
                        pre_rx_signal_sta = rx_signal_sta;
                        g_bF1H2TXGsv2k1SwitchFlag = TRUE;
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
                                    mapp_F1H2TX_SetPort(j);
                                    pstPrjSaveVar->stHdmiSel.u8SwChannel = j;
                                    Uart_printf("Switch to HDMI %d\r\n",i+1);
                                    pre_rx_signal_sta = rx_signal_sta;
                                    g_bF1H2TXGsv2k1SwitchFlag = TRUE;
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
                                g_bF1H2TXGsv2k1SwitchFlag = TRUE;
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

void mapp_F1H2TX_SwitchPortProcess(void)
{
    if(!mapp_F1H2TX_PlatformTimerExpired(emUserTimer_SwitchPortTime))
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

void mapp_F1H2TX_SetPort(u8 u8Port)
{
    g_u8ActiveSource = u8Port;
    //g_bSwitchPortFlag = TRUE;
//    it6635_set_active_port(2);

    mapp_F1H2TX_PlatformTimerSet(emUserTimer_SwitchPortTime,500);
}

u8 mapp_F1H2TX_GetActivePort(void)
{
    return g_u8ActiveSource;
}

void mapp_F1H2TX_SwitcherFunction(void)
{
    mapp_F1H2TX_AutoSwitcheProcess();
    mapp_F1H2TX_SwitchPortProcess();
}


