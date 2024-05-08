#define __PROJECT_VAR_FUN_CONFIG_C__

#include "includes.h"

#define USER_FUNCTION_DEBUG     0

#if USER_FUNCTION_DEBUG
#define DBG_USER_FUN        Uart_printf
#else
#define DBG_USER_FUN(...)
#endif

stProjectSpecVar_t  stEMX5RXSpecVar;
#if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
static bool         sg_bEnableSwitchPortFlag = FALSE;
static u8           sg_u8CurSwitchPort = 0;
#endif

static volatile u32 g_EMX5RX_Timers[emUserTimer_Max];
bool g_bEnableSwitchHdcp = FALSE;
bool g_bReAutherHdcp = FALSE;
u8   g_u8EMX5RX_TxEdidBuf[256];
bool g_bSetLocalIDFlag = FALSE;
bool g_bSetInputIDFlag = FALSE;
u8   g_u8DeviceMode = 1;
u8   g_u8DispID[4] = {0x10,0x10,0x10,0x10};

void mapp_EMX5RX_PlatformTimerInit (void);
void mapp_EMX5RX_LedProcess(void);
void EMX5RX_1msTimerIrq(void);
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
void mapp_IRUserInit(void);
#endif
void mapp_GetDeviceType(void);

//============================================================================================

const u8 g_u8EMX5RX_Disp[4][18][11] =
{
       //a0, b0, c0, d0, e0, f0, g0, C1, C2, C3, C4
    {   // disp1
        {1,  1,  1,  1,  1,  1,  0,  0,  1,  1,  1},    // 0
        {0,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1},    // 1
        {1,  1,  0,  1,  1,  0,  1,  0,  1,  1,  1},    // 2
        {1,  1,  1,  1,  0,  0,  1,  0,  1,  1,  1},    // 3
        {0,  1,  1,  0,  0,  1,  1,  0,  1,  1,  1},    // 4
        {1,  0,  1,  1,  0,  1,  1,  0,  1,  1,  1},    // 5
        {1,  0,  1,  1,  1,  1,  1,  0,  1,  1,  1},    // 6
        {1,  1,  1,  0,  0,  0,  0,  0,  1,  1,  1},    // 7
        {1,  1,  1,  1,  1,  1,  1,  0,  1,  1,  1},    // 8
        {1,  1,  1,  1,  0,  1,  1,  0,  1,  1,  1},    // 9
        {1,  1,  1,  0,  1,  1,  1,  0,  1,  1,  1},    // A
        {0,  0,  1,  1,  1,  1,  1,  0,  1,  1,  1},    // B
        {1,  0,  0,  1,  1,  1,  0,  0,  1,  1,  1},    // C
        {0,  1,  1,  1,  1,  0,  1,  0,  1,  1,  1},    // D
        {1,  0,  0,  1,  1,  1,  1,  0,  1,  1,  1},    // E
        {1,  0,  0,  0,  1,  1,  1,  0,  1,  1,  1},    // F
        {0,  0,  0,  0,  0,  0,  1,  0,  1,  1,  1},    // -
        {0,  0,  0,  1,  1,  1,  0,  0,  1,  1,  1},    // L
    },
    {   // disp2
        {1,  1,  1,  1,  1,  1,  0,  1,  0,  1,  1},    // 0
        {0,  1,  1,  0,  0,  0,  0,  1,  0,  1,  1},    // 1
        {1,  1,  0,  1,  1,  0,  1,  1,  0,  1,  1},    // 2
        {1,  1,  1,  1,  0,  0,  1,  1,  0,  1,  1},    // 3
        {0,  1,  1,  0,  0,  1,  1,  1,  0,  1,  1},    // 4
        {1,  0,  1,  1,  0,  1,  1,  1,  0,  1,  1},    // 5
        {1,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1},    // 6
        {1,  1,  1,  0,  0,  0,  0,  1,  0,  1,  1},    // 7
        {1,  1,  1,  1,  1,  1,  1,  1,  0,  1,  1},    // 8
        {1,  1,  1,  1,  0,  1,  1,  1,  0,  1,  1},    // 9
        {1,  1,  1,  0,  1,  1,  1,  1,  0,  1,  1},    // A
        {0,  0,  1,  1,  1,  1,  1,  1,  0,  1,  1},    // B
        {1,  0,  0,  1,  1,  1,  0,  1,  0,  1,  1},    // C
        {0,  1,  1,  1,  1,  0,  1,  1,  0,  1,  1},    // D
        {1,  0,  0,  1,  1,  1,  1,  1,  0,  1,  1},    // E
        {1,  0,  0,  0,  1,  1,  1,  1,  0,  1,  1},    // F
        {0,  0,  0,  0,  0,  0,  1,  1,  0,  1,  1},    // -
        {0,  0,  0,  1,  1,  1,  0,  1,  0,  1,  1},    // L
    },
    {   // disp3
        {1,  1,  1,  1,  1,  1,  0,  1,  1,  0,  1},    // 0
        {0,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1},    // 1
        {1,  1,  0,  1,  1,  0,  1,  1,  1,  0,  1},    // 2
        {1,  1,  1,  1,  0,  0,  1,  1,  1,  0,  1},    // 3
        {0,  1,  1,  0,  0,  1,  1,  1,  1,  0,  1},    // 4
        {1,  0,  1,  1,  0,  1,  1,  1,  1,  0,  1},    // 5
        {1,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1},    // 6
        {1,  1,  1,  0,  0,  0,  0,  1,  1,  0,  1},    // 7
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  0,  1},    // 8
        {1,  1,  1,  1,  0,  1,  1,  1,  1,  0,  1},    // 9
        {1,  1,  1,  0,  1,  1,  1,  1,  1,  0,  1},    // A
        {0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  1},    // B
        {1,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1},    // C
        {0,  1,  1,  1,  1,  0,  1,  1,  1,  0,  1},    // D
        {1,  0,  0,  1,  1,  1,  1,  1,  1,  0,  1},    // E
        {1,  0,  0,  0,  1,  1,  1,  1,  1,  0,  1},    // F
        {0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  1},    // -
        {0,  0,  0,  1,  1,  1,  0,  1,  1,  0,  1},    // L
    },
    {   // disp4
        {1,  1,  1,  1,  1,  1,  0,  1,  1,  1,  0},    // 0
        {0,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0},    // 1
        {1,  1,  0,  1,  1,  0,  1,  1,  1,  1,  0},    // 2
        {1,  1,  1,  1,  0,  0,  1,  1,  1,  1,  0},    // 3
        {0,  1,  1,  0,  0,  1,  1,  1,  1,  1,  0},    // 4
        {1,  0,  1,  1,  0,  1,  1,  1,  1,  1,  0},    // 5
        {1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0},    // 6
        {1,  1,  1,  0,  0,  0,  0,  1,  1,  1,  0},    // 7
        {1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  0},    // 8
        {1,  1,  1,  1,  0,  1,  1,  1,  1,  1,  0},    // 9
        {1,  1,  1,  0,  1,  1,  1,  1,  1,  1,  0},    // A
        {0,  0,  1,  1,  1,  1,  1,  1,  1,  1,  0},    // B
        {1,  0,  0,  1,  1,  1,  0,  1,  1,  1,  0},    // C
        {0,  1,  1,  1,  1,  0,  1,  1,  1,  1,  0},    // D
        {1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0},    // E
        {1,  0,  0,  0,  1,  1,  1,  1,  1,  1,  0},    // F
        {0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  0},    // -
        {0,  0,  0,  1,  1,  1,  0,  1,  1,  1,  0},    // L
    },
};
void EMX5RX_SpecificVarInit(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;

    memset(pstSpecVar, 0, sizeof(stProjectSpecVar_t));
    pstPrjSaveVar->stSaveSpecVar.pvSpecVar = (void *)pstSpecVar;
    pstPrjSaveVar->stSaveSpecVar.u16SpecVarSize = sizeof(stProjectSpecVar_t);
    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
}

void EMX5RX_SpecificVarDefault(void)
{
    EMX5RX_SpecBasicVarDefault();
}

void EMX5RX_SpecBasicVarDefault(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    //u8 i;

    pstSpecVar->stBasSpecVar.bPowerOnSta = TRUE;
    pstSpecVar->stBasSpecVar.u8TXID = 0x0;
    pstSpecVar->stBasSpecVar.u8RXID = 0x0;
    pstSpecVar->stBasSpecVar.bKeyPadLock = FALSE;
    pstSpecVar->stBasSpecVar.u8InputChannel = 0;
}

bool EMX5RX_SpecVarValidCheck(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    bool                        isSpecVarValid = TRUE;

    if(pstSpecVar->stBasSpecVar.u8CheckSum != CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar)))
    {
        EMX5RX_SpecBasicVarDefault();
        pstSpecVar->stBasSpecVar.u8CheckSum = CRC8Calculate((uint8_t *)&pstSpecVar->stBasSpecVar,((uint8_t *)&pstSpecVar->stBasSpecVar.u8CheckSum-(uint8_t *)&pstSpecVar->stBasSpecVar));

        isSpecVarValid = FALSE;
    }

    return isSpecVarValid;
}

void SaveUserData_SpecBasicVar(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8CheckSum = CRC8Calculate((uint8_t *)pstBasicVar, ((uint8_t *)&pstBasicVar->u8CheckSum-(uint8_t *)pstBasicVar));
#if (_CHIP_EEPROM_TYPE_USE != _EEPROM_CHIP_NO_USE)
    eeprom_write((ADDR_EEPROM_USER_START+((uint8_t *)pstBasicVar-(uint8_t *)pstPrjSaveVar)),
                (uint8_t *)pstBasicVar, sizeof(stBasicSpecVar_t));
#else
    UNUSED(pstPrjSaveVar);
#endif
}

void mapp_EMX5RX_PowerOnInit(void)
{
    pFuncPrj1MsLoopHook = EMX5RX_1msTimerIrq;
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
    pmapp_IrKeyLearnOkFunc = mapp_IRLearnOkFunction;
    pmapp_IrInitFunc = mapp_IRUserInit;
#endif
//    mapp_SetIOMode(emIOMode_IR_Receive);
    mapp_EMX5RX_PlatformTimerSet(emUserTimer_CheckDeviceType,25000);
    if(mapp_GetRxKeySta(0)&&mapp_GetRxKeySta(1))
    {
        g_u8DeviceMode = 1;
    }
    else
    {
        g_u8DeviceMode = 0;
    }
}

//========================================================================================
// add by wf8421 20190820
//------------------------------------------------------------------------------
// Function:    mapp_SCU41TSMV_PlatformTimerSet
// Description: 设置相关功能定时器时间
//------------------------------------------------------------------------------
void mapp_EMX5RX_PlatformTimerSet( uint8_t index, u32 m_sec )
{
    switch ( index )
    {
        default:
            if (index<emUserTimer_Max)
            {
                g_EMX5RX_Timers[index] = m_sec;
            }
            break;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_SCU41TSMV_PlatformTimerGet
// Description: 获取相关功能定时器时间
//------------------------------------------------------------------------------
u32 mapp_EMX5RX_PlatformTimerGet(uint8_t eTimer)
{
    return(g_EMX5RX_Timers[eTimer]);
}

//------------------------------------------------------------------------------
// Function:    mapp_SCU41TSMV_PlatformTimerInit
// Description: 定时器初始化
//------------------------------------------------------------------------------
void mapp_EMX5RX_PlatformTimerInit (void)
{
    int i;
    //initializer timer counters in array
    for ( i = 0; i < emUserTimer_Max; i++ )
    {
        g_EMX5RX_Timers[ i ] = 0;
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_SCU41TSMV_PlatformTimerTask
// Description: 定时器递减函数
//------------------------------------------------------------------------------
void mapp_EMX5RX_PlatformTimerTask(void)
{
    int i;
    for(i=0;i<emUserTimer_Max;i++)
    {
        if(g_EMX5RX_Timers[i] > 0)
        {
            g_EMX5RX_Timers[i]--;
        }
    }
}

//------------------------------------------------------------------------------
// Function:    mapp_SCU41TSMV_PlatformTimerExpired
// Description: 相关功能定时器时间是否为0
//------------------------------------------------------------------------------
uint8_t mapp_EMX5RX_PlatformTimerExpired ( uint8_t timer )		   //定时时间到函数
{
    if ( timer < emUserTimer_Max )
    {
        return( g_EMX5RX_Timers[ timer ] == 0);			   //定时时间到时返回1
    }

    return( 0 );										   //定时时间还没到时返回0
}

void EMX5RX_1msTimerIrq(void)
{
    mapp_EMX5RX_PlatformTimerTask();
    mapp_EMX5RX_LedProcess();
}

void mapp_EMX5RX_LedProcess(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    u8 i;
    static u8 sl_u8Dis = 0;
    static u16 sl_u16LoopTime = 0;

    if(pstSpecVar->stBasSpecVar.bPowerOnSta)
    {
        //if(mapp_EMX5RX_PlatformTimerExpired(emUserTimer_WorkSta))
        {
            //mapp_EMX5RX_PlatformTimerSet(emUserTimer_WorkSta,1);

            for(i=0;i<led_name_max_number-1;i++)
            {
                pstLedDeclare_t pLedN = (pstLedDeclare_t)&stProjectLedTable[i];
                if(pLedN->pstLedLocate != NULL)
                {
                    if(g_u8EMX5RX_Disp[sl_u8Dis][g_u8DispID[sl_u8Dis]][i])
                    {
                        GPIO_WriteBit(STM32_GPIO_PORT[pLedN->pstLedLocate->u8PortType-PORT_GPIOA],
                                    pLedN->pstLedLocate->u16GpioPin,
                                    pLedN->eLedOnLevel==ePinLevel_High?Bit_SET: Bit_RESET);
                    }
                    else
                    {
                        GPIO_WriteBit(STM32_GPIO_PORT[pLedN->pstLedLocate->u8PortType-PORT_GPIOA],
                                    pLedN->pstLedLocate->u16GpioPin,
                                    (!(pLedN->eLedOnLevel))==ePinLevel_High?Bit_SET: Bit_RESET);
                    }
                }
            }
            sl_u8Dis++;
            if(sl_u8Dis==4)
                sl_u8Dis = 0;

            if(sl_u16LoopTime++>=500)
            {
                pstLedDeclare_t pLedN = (pstLedDeclare_t)&stProjectLedTable[led_name_ActiveLed];
                sl_u16LoopTime = 0;
                if(GPIO_ReadOutputDataBit(STM32_GPIO_PORT[pLedN->pstLedLocate->u8PortType-PORT_GPIOA],
                                    pLedN->pstLedLocate->u16GpioPin))
                {
                    GPIO_WriteBit(STM32_GPIO_PORT[pLedN->pstLedLocate->u8PortType-PORT_GPIOA],
                                pLedN->pstLedLocate->u16GpioPin,
                                Bit_RESET);
                }
                else
                {
                    GPIO_WriteBit(STM32_GPIO_PORT[pLedN->pstLedLocate->u8PortType-PORT_GPIOA],
                                pLedN->pstLedLocate->u16GpioPin,
                                Bit_SET);
                }
            }
        }
    }
    else
    {
    }
}

void mapp_EMX5RX_MainFunction(void)
{
//    mapp_EMX5RX_LedProcess();
//    mapp_KeyFunctionProcess();
    mapp_GetDeviceType();

    mapp_EMX5RX_SetIDProcess();
}


void mapp_EMX5RX_StartupMessage(void)
{
    Uart_printf("\r\n");
    Uart_printf("System Initializing...\r\n");
    Uart_printf("\r\n");
    Uart_printf("Initialization Finished!\r\n");
    Uart_printf("<%s %s",_STRING_FW_VERSION,_STRING_FW_VER_NUM);
    Uart_printf("<%s",_STRING_TEST_FW_VERSION);
}

#define VALUE_LEN       800
/*
**| SCODE | MCMD | SCMD | LEN | DATA0~DATAN | ECODE |
**| 0xF5  |      |      |     |             | 0x5F  |
*/
void mapp_EMX5RX_UartToCmd(u8 u8Mcmd, u8 u8Scmd, u16 u16Len, pu8 pu8Data,bool bTowByteLen)
{
    pstUartCom_t    puartCommVar = &g_stUartCommVar;
    u8 l_u8SendValue[VALUE_LEN];
    u16 i=0;
    u16 l_u16index=0;

    if(u16Len>VALUE_LEN)
        u16Len = VALUE_LEN;
    memset(l_u8SendValue,0,VALUE_LEN);
    l_u8SendValue[l_u16index++] = 0xF5;
    l_u8SendValue[l_u16index++] = u8Mcmd;
    l_u8SendValue[l_u16index++] = u8Scmd;
    l_u8SendValue[l_u16index++] = u16Len&0xFF;
    if(bTowByteLen)
    {
        l_u8SendValue[l_u16index++] = (u16Len>>8)&0xFF;
    }
    for(i=0;i<u16Len;i++)
    {
        l_u8SendValue[l_u16index+i] = pu8Data[i];
    }
    l_u8SendValue[l_u16index+u16Len] = 0x5F;

    puartCommVar->SendBytes(l_u8SendValue,l_u16index+1+u16Len,FALSE);
    {
        pstUartCom_t    puart1CommVar = &g_stUart1CommVar;
        puart1CommVar->SendBytes(l_u8SendValue,l_u16index+1+u16Len,FALSE);
    }
}


void mapp_EMX5RX_SendCMDToSigmastar(u8 u8Cmd,u16 u16Len,pu8 pu8Data)
{
    u8 l_u8Mcmd = 0;
    u8 l_u8Scmd = 0;
    bool l_bTowByteLen = FALSE;

    switch(u8Cmd)
    {
        case emCmd2Sigma_Ok:
            {
                l_u8Mcmd = 0xFF;
                l_u8Scmd = 0x00;
                u16Len = 0;
            }
            break;
        case emCmd2Sigma_Error:
            {
                l_u8Mcmd = 0xFF;
                l_u8Scmd = 0x01;
                u16Len = 0;
            }
            break;
        case emCmd2Sigma_SwitchInput:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x03;
            }
            break;
        case emCmd2Sigma_GetInputID:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x04;
            }
            break;
        case emCmd2Sigma_GetLocalID:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x05;
            }
            break;
        case emCmd2Sigma_SetLocalID:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x06;
            }
            break;
        case emCmd2Sigma_SetKeyLock:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x07;
            }
            break;
        case emCmd2Sigma_SetEncrypt:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x08;
            }
            break;
        case emCmd2Sigma_SetEdidData:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x0A;
            }
            break;
        case emCmd2Sigma_SetUartDataBypass:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0xFF;
                l_bTowByteLen = TRUE;
            }
            break;
        case emCmd2Sigma_SetIOMode:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x0B;
            }
            break;
        case emCmd2Sigma_SetIOOutStatus:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x0C;
            }
            break;
        case emCmd2Sigma_GetIOInputStatus:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x0D;
            }
            break;
        case emCmd2Sigma_SendIRData:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x0E;
                l_bTowByteLen = TRUE;
            }
            break;
        case emCmd2Sigma_GetVersion:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x19;
            }
            break;
        case emCmd2Sigma_GetDeviceType:
            {
                l_u8Mcmd = 0x03;
                l_u8Scmd = 0x20;
            }
            break;

        default:
            break;
    }
    mapp_EMX5RX_UartToCmd(l_u8Mcmd,l_u8Scmd,u16Len,pu8Data,l_bTowByteLen);
}

void mapp_EMX5RX_SetIDProcess(void)
{
    if(!mapp_EMX5RX_PlatformTimerExpired(emUserTimer_IDSetTimer))
    {
        return;
    }
    if(g_bSetLocalIDFlag)
    {
        pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
        pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

        g_bSetLocalIDFlag = FALSE;
        mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_SetLocalID,1,&(pstBasicVar->u8RXID));
    }
    if(g_bSetInputIDFlag)
    {
        pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
        pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
        u8 l_u8Temp[2];

        g_bSetInputIDFlag = FALSE;

        l_u8Temp[0] = pstBasicVar->u8TXID;

        if(g_u8DeviceMode==0)
        {
            mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_SetLocalID,1,l_u8Temp);
        }
        else
        {
            l_u8Temp[1] = pstBasicVar->u8InputChannel;
            mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_SwitchInput,2,l_u8Temp);
        }
    }
}

void mapp_EMX5RX_SetLocalID(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    g_bSetLocalIDFlag = TRUE;

    g_u8DispID[0] = (pstBasicVar->u8RXID)/10;
    g_u8DispID[1] = (pstBasicVar->u8RXID)%10;
    mapp_EMX5RX_PlatformTimerSet(emUserTimer_IDSetTimer,500);
}

void mapp_EMX5RX_SetInputID(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    g_bSetInputIDFlag = TRUE;

    g_u8DispID[2] = (pstBasicVar->u8TXID)/10;
    g_u8DispID[3] = (pstBasicVar->u8TXID)%10;
    mapp_EMX5RX_PlatformTimerSet(emUserTimer_IDSetTimer,500);
}

void mapp_SetIOMode(u8 u8Mode)
{
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    switch(u8Mode)
    {
        case emIOMode_IR_Receive:
            pstBasicVar->u8IOMode = emIOMode_IR_Receive;
            SetIrLearnMode(eIrWorkMode_Study);
            break;
        case emIOMode_IR_Send:
            pstBasicVar->u8IOMode = emIOMode_IR_Send;
            SetIrLearnMode(eIrWorkMode_None);
            break;
        case emIOMode_IO:
            pstBasicVar->u8IOMode = emIOMode_IO;
            break;
        default:
            pstBasicVar->u8IOMode = emIOMode_None;
            break;
    }
}

#if ((_IR_FUNCTION_USE&_IR_FUNCTION_KEY_STUDY)==_IR_FUNCTION_KEY_STUDY)
void mapp_IRUserInit(void)
{
    SetIrLearnMode(eIrWorkMode_Study);
}

void mapp_SendIrData(u8 *pu8IrData, u16 u16Len)
{
//    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
//    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u16 l_u16Len=0;
    u16 l_u16Value[300];

//    if(pstBasicVar->u8IOMode != emIOMode_IR_Send)
//        return;

    l_u16Len = U8ToU16MsbCovert(pu8IrData,l_u16Value,u16Len);
    mapp_IrSendDirect(l_u16Value,l_u16Len);
    SetIrLearnMode(eIrWorkMode_Study);
}

void mapp_IRLearnOkFunction(void)
{
//    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
//    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Value[600];
    u16 l_16Len=0;

//    if(pstBasicVar->u8IOMode != emIOMode_IR_Receive)
//        return;

    if(g_u16IrLearnCnt<9)
    {
//        g_u8IrLearnMode = eIrWorkMode_Learn;
        SetIrLearnMode(eIrWorkMode_Study);
        return;
    }
    l_16Len = U16ToU8MsbCovert(g_u16IrLearnKeyData,l_u8Value,g_u16IrLearnCnt);
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_SendIRData,l_16Len,l_u8Value);
//    g_u8IrLearnMode = eIrWorkMode_Learn;
    SetIrLearnMode(eIrWorkMode_Study);
}
#endif

void mapp_GetDeviceType(void)
{
    static u8 sl_u8StartupFlag = 1;

    if(!mapp_EMX5RX_PlatformTimerExpired(emUserTimer_CheckDeviceType))
    {
        return;
    }

    if(sl_u8StartupFlag)
    {
        sl_u8StartupFlag = 0;
        mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_GetDeviceType,0,NULL);
    }
}

