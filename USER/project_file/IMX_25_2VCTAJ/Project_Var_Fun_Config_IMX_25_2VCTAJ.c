#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

stProjectSpecVar_t  stIMX_25_2VCTAJSpecVar;

bool g_bIMX_25_2VCTAJAutoSwitchFirstStatus = TRUE;
static volatile u32 g_IMX_25_2VCTAJ_Timers[emUserTimer_Max];
u8 g_u8ActiveSource = 0;
u8 g_u8PreActiveSource = 0;
bool g_bSwitchPortFlag = FALSE;
extern AvEdidMode EdidHdmiMode;
bool g_bIMX_25_2VCTAJGsv2k1SwitchFlag = FALSE;

extern stProjectGlobalAllVar_t g_stPrjGloVar;

const uint8_t edid_IMX_25_2VCTAJ_default[256] = // default edid
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

void mapp_IMX_25_2VCTAJ_GsvFunctionInit(void);
void mapp_IMX_25_2VCTAJ_GsvFunctionProcess(void);
void mapp_IMX_25_2VCTAJ_SwitcherFunction(void);
void mapp_IMX_25_2VCTAJ_SetPort(u8 u8Port);

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAJ_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_IMX_25_2VCTAJ_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_IMX_25_2VCTAJ_PlatformTimerGet(uint8_t eTimer)
{
    return(g_IMX_25_2VCTAJ_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAJ_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_IMX_25_2VCTAJ_Timers[ i ] = 0;
    }

    //PlatformTimerSet(etKEY_Detect,5000);
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_IMX_25_2VCTAJ_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_IMX_25_2VCTAJ_Timers[i] > 0)
        {
            g_IMX_25_2VCTAJ_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_MHD1616STX_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_IMX_25_2VCTAJ_PlatformTimerExpired ( uint8_t timer )           //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_IMX_25_2VCTAJ_Timers[ timer ] == 0);              //定时时间到时返回1
    }

    return( 0 );                                           //定时时间还没到时返回0
}
//========================================================================================
void IMX_25_2VCTAJ_1msTimerIrq(void)
{
    mapp_IMX_25_2VCTAJ_PlatformTimerTask();
}

void IMX_25_2VCTAJ_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAJSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);

}

void IMX_25_2VCTAJ_SpecificVarDefault(void)
{
    IMX_25_2VCTAJ_SpecBasicVarDefault();
    IMX_25_2VCTAJ_SpecCustomEdidVarDefault();
}

void IMX_25_2VCTAJ_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAJSpecVar;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.bIsLock = FALSE;
}

void IMX_25_2VCTAJ_SpecCustomEdidVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAJSpecVar;

    memcpy(pstSpecVar->stCustomEdidPre1.pu8EdidContext, System_Default_Edid_table, 256);
}

bool IMX_25_2VCTAJ_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAJSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        IMX_25_2VCTAJ_SpecBasicVarDefault();
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
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAJSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAJSpecVar;
    pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

    pstEdidOp = pstEdidOrg+u8CustomEdidIndex;
    pstEdidOp->u8CheckSum = CRC8Calculate((uint8_t *)pstEdidOp, ((uint8_t *)&pstEdidOp->u8CheckSum-(uint8_t *)pstEdidOp));
    SaveUserDataToEep((eEepDataIndex_t)(eEepDataIndex_SpecBlock2+u8CustomEdidIndex),\
        (ADDR_EEPROM_USER_START+((uint8_t *)&pstPrjSaveVar->stSaveSpecVar-(uint8_t *)pstPrjSaveVar)+((uint8_t *)pstEdidOp-(uint8_t *)pstSpecVar)),\
        (uint8_t *)pstEdidOp, sizeof(stCustomEdid_t), TIME_SAVE_TO_EEPROM_IMMEDIATELY, FALSE);
}

// the status when power on device
void IMX_25_2VCTAJ_PrjStatePowerOnInit(void)
{
    System_Default_Edid_table = (iTE_u8 *)edid_IMX_25_2VCTAJ_default;

    // prj 1ms loop hook
    pFuncPrj1MsLoopHook = IMX_25_2VCTAJ_1msTimerIrq;
    pvvFuncGsvUserFunctionInit = mapp_IMX_25_2VCTAJ_GsvFunctionInit;
    pvvFuncGsvUserFunctionProcess = mapp_IMX_25_2VCTAJ_GsvFunctionProcess;
}

void IMX_25_2VCTAJ_LedWork_Loop(void)
{
    static bool sl_bLedSta = FALSE;
    if(!mapp_IMX_25_2VCTAJ_PlatformTimerExpired(emUserTimer_LedActive))
        return;
    mapp_IMX_25_2VCTAJ_PlatformTimerSet(emUserTimer_LedActive,500);

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

void mapp_IMX_25_2VCTAJ_Gsv2K1FunctionInit(void)
{
    EdidHdmiMode = AvEdidModeUserDefine;
}

void mapp_IMX_25_2VCTAJ_GsvFunctionInit(void)
{
    mapp_IMX_25_2VCTAJ_Gsv2K1FunctionInit();
}

void mapp_IMX_25_2VCTAJ_GsvFunctionProcess(void)
{
    if(g_bIMX_25_2VCTAJGsv2k1SwitchFlag)
    {
        u8 i;
        for(i=0;i<AvGsv2k1DeviceNumber;i++)
        {
            KfunPrHpdDown(&gsv2k1Ports[i][0]);
            delay_ms(200);
            KfunPrHpdUp(&gsv2k1Ports[i][0]);
        }
        g_bIMX_25_2VCTAJGsv2k1SwitchFlag = FALSE;
    }
}

// the status of led when running system
void IMX_25_2VCTAJ_FuncWork_LoopRunning(void)
{
    IMX_25_2VCTAJ_LedWork_Loop();
}

void mapp_IMX_25_2VCTAJStartupMessage(void)
{
    RS232_printf("\r\n");
    RS232_printf("System Initializing...\r\n");
    RS232_printf("\r\n");
    RS232_printf("Initialization Finished!\r\n");
    RS232_printf("@%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
}


