#include "includes.h"

typedef struct _stLcdDisplayVar_t_
{
    bool bLcdEnable[4];
    char pcDispString[4][17];
}stLcdDisplayVar_t;

extern stProjectGlobalAllVar_t g_stPrjGloVar;
#define Enable_LockPanelByAllKey        0
#define ENABLE_AUTO_MODE_TO_SWITCH_CHANNEL  1
struct KeyBuff KeyBuffValue = {{0},0,{0},0,0,0,0,0};
struct UITimeOut UITimeOutBuff = {0,0,0,0};
char  sys_type[16] =         "Matrix Switch   ";
char  sys_name[16] =         "MUH0808S-Pro    ";
const char  LcdNone[16] =    "                ";
uint8_t KeyStatus = 0;
bool g_bEnableLcdBacklight = FALSE;
unsigned char IN_channel = 0;
unsigned char OUT_num = 0;
unsigned char OUT_channel[32] = {0};
bool g_bEnableCheckChannel = FALSE;
stLcdDisplayVar_t g_stLcdVar;

void OperateStatsu(uint8_t keyValue);

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef struct _stKeyScanType   // wf8421 20180120
{
    u8 m_KSTypeGroup;
    u32 m_KSTypeValue;
    emKeyFunction m_KSTypeKeyNomalType;
    bool m_KSTypeComboEnable;
    u32 m_KSTypeComboTime;
    emKeyFunction m_KSTypeKeyComboType;
    bool m_KSTypeMultiFuncEnable;
    bool (*MFCondition)(void);
    emKeyFunction m_KSTypeKeyMultiFuncType;
    u8 m_Flag;
}stKeyScanType;
#endif

#if 1
//#define SrcComboTime        (1000)
#define CtrlComboTime       (3*1000)
#define RepeatComboTime     (1*1000)
#define EdidComboTime       (3*1000)

// 某些条件出发按键的第二功能，这里需要把条件放到相应的项里面
bool unUseMultiKeyFunc(void)
{
    return FALSE;
}

// 按键功能定义，第一组数据跳过，程序会读错数值，具体不知道什么原因。
static stKeyScanType stKeyScanGroup[]=
{
    /*Group     Value               KeyNomalType             ComboEnable  ComboTime     KeyComboType   MultiFuncEnable MFConditionFlag   KeyMultiFuncType   m_Flag*/
    {  0,           0,              emKeyFunc_None,         FALSE,          0,        emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_NoUse},

    {  0,   emKeyvalueInput1,       emKeyFunc_In1,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput2,       emKeyFunc_In2,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput3,       emKeyFunc_In3,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput4,       emKeyFunc_In4,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput5,       emKeyFunc_In5,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput6,       emKeyFunc_In6,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput7,       emKeyFunc_In7,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput8,       emKeyFunc_In8,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},

    {  0,  emKeyvalueOutput1,      emKeyFunc_Out1,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput2,      emKeyFunc_Out2,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput3,      emKeyFunc_Out3,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput4,      emKeyFunc_Out4,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput5,      emKeyFunc_Out5,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput6,      emKeyFunc_Out6,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput7,      emKeyFunc_Out7,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,  emKeyvalueOutput8,      emKeyFunc_Out8,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},

    {  0,      emKeyvalueAll,       emKeyFunc_All,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,     emKeyvalueLock,      emKeyFunc_None,           TRUE,   CtrlComboTime,   emKeyFunc_Lock,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,     emKeyvalueMode,      emKeyFunc_Mode,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvalueEnter,     emKeyFunc_Enter,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvalueScene,     emKeyFunc_Scene,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvalueClear,     emKeyFunc_Clear,          FALSE,   CtrlComboTime,   emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
};

void mapp_KeyValueScan(u32 *tKeyPadVaule)
{
    static emKeyFunction tKeyvFunction=emKeyFunc_None;
    static bool ComboKeyFlag = FALSE;
    static emKeyFunction KeyValue = emKeyFunc_None;
    static emKeyFunction RealKeyValue = emKeyFunc_None;
    static u8 PreKeyScanTypeValue=0xFF;
    static u8 PreKeyScanTypeValue2=0xFF;
    u8 i = 0;

    for(i=1;i<(sizeof(stKeyScanGroup)/sizeof(stKeyScanGroup[0]));i++)
    {
        if(stKeyScanGroup[i].m_Flag==KeyFlag_NoUse)
        {
            continue;
        }
        else
        #if 1
        if(stKeyScanGroup[i].m_Flag&KeyFlag_DIPSwitcher)
        {
            if(tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup] == stKeyScanGroup[i].m_KSTypeValue)
            {
                if((tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup] != PreKeyScanTypeValue)
                    ||PreKeyScanTypeValue2!=PreKeyScanTypeValue)
                {
                    //PreKeyScanTypeValue = tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup];
                    //RS232_printf("PreKeyScanTypeValue[%d]=%d\n",i,PreKeyScanTypeValue);

                    if(PreKeyScanTypeValue2 == PreKeyScanTypeValue) // modify by wf8421 20181210
                        PreKeyScanTypeValue2 = tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup];
                    if(tKeyvFunction == stKeyScanGroup[i].m_KSTypeKeyNomalType
                        ||tKeyvFunction == emKeyFunc_repeat
                        ||((stKeyScanGroup[i].m_KSTypeMultiFuncEnable==TRUE)&&(tKeyvFunction == stKeyScanGroup[i].m_KSTypeKeyMultiFuncType))
                        )
                    {
                        #if 1
                        if(stKeyScanGroup[i].m_KSTypeComboEnable)
                        {
                            if(tKeyvFunction == stKeyScanGroup[i].m_KSTypeKeyNomalType)
                            {
                                PlatformTimerSet(etKEY_CobomTime,stKeyScanGroup[i].m_KSTypeComboTime);
                                ComboKeyFlag = TRUE;
                            }

                            if((ComboKeyFlag == TRUE)&&(PlatformTimerGet(etKEY_CobomTime)==0))
                            {
                                ComboKeyFlag = FALSE;
                                //  按键长按功能，需要长按到一定时间后直接执行动作，所以这里直接给key赋值相应功能。
                                RealKeyValue = stKeyScanGroup[i].m_KSTypeKeyComboType;
                                //RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyComboType === %d\n",i,stKeyScanGroup[i].m_KSTypeKeyComboType);
                                KeyValue = emKeyFunc_None;
                                tKeyvFunction = emKeyFunc_repeat;
                                PreKeyScanTypeValue = tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup];
                                //RS232_printf("PreKeyScanTypeValue[%d]=%d\n",i,PreKeyScanTypeValue);
                                PreKeyScanTypeValue2 = PreKeyScanTypeValue;
                            }
                            else
                                tKeyvFunction = emKeyFunc_repeat;
                        }
                        else
                        #endif
                            tKeyvFunction = emKeyFunc_repeat;
                    }
                    else
                    {
                        {
                            tKeyvFunction = stKeyScanGroup[i].m_KSTypeKeyNomalType;
                            KeyValue = stKeyScanGroup[i].m_KSTypeKeyNomalType;
                        }
                        //RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyNomalType === %d\n",i,stKeyScanGroup[i].m_KSTypeKeyNomalType);
                    }
                    break;
                }
            }
        }
        else
        #endif
        {
            if(tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup] == stKeyScanGroup[i].m_KSTypeValue)
            {
                #if 0
                RS232_printf("\n==================================================\n");
                RS232_printf("stKeyScanGroup[%d].m_KSTypeGroup          === %d\n",i,stKeyScanGroup[i].m_KSTypeGroup);
                RS232_printf("stKeyScanGroup[%d].m_KSTypeValue          === 0x%x\n",i,stKeyScanGroup[i].m_KSTypeValue);
                RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyNomalType   === %d\n",i,stKeyScanGroup[i].m_KSTypeKeyNomalType);
                RS232_printf("stKeyScanGroup[%d].m_KSTypeComboEnable    === %d\n",i,stKeyScanGroup[i].m_KSTypeComboEnable);
                RS232_printf("stKeyScanGroup[%d].m_KSTypeComboTime      === %d\n",i,stKeyScanGroup[i].m_KSTypeComboTime);
                RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyComboType   === %d\n",i,stKeyScanGroup[i].m_KSTypeKeyComboType);
                RS232_printf("stKeyScanGroup[%d].m_KSTypeMultiFuncEnable=== %d\n",i,stKeyScanGroup[i].m_KSTypeMultiFuncEnable);
                RS232_printf("stKeyScanGroup[%d].MFCondition()          === %d\n",i,stKeyScanGroup[i].MFCondition());
                RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyMultiFuncType=== %d\n",i,stKeyScanGroup[i].m_KSTypeKeyMultiFuncType);
                RS232_printf("stKeyScanGroup[%d].m_Flag                 === %d\n",i,stKeyScanGroup[i].m_Flag);
                RS232_printf("==================================================\n");
                #endif
                if(tKeyvFunction == stKeyScanGroup[i].m_KSTypeKeyNomalType
                    ||tKeyvFunction == emKeyFunc_repeat
                    ||((stKeyScanGroup[i].m_KSTypeMultiFuncEnable==TRUE)&&(tKeyvFunction == stKeyScanGroup[i].m_KSTypeKeyMultiFuncType))
                    )
                {
                #if 1
                    if(stKeyScanGroup[i].m_KSTypeComboEnable)
                    {
                        if(tKeyvFunction == stKeyScanGroup[i].m_KSTypeKeyNomalType)
                        {
                            PlatformTimerSet(etKEY_CobomTime,stKeyScanGroup[i].m_KSTypeComboTime);
                            ComboKeyFlag = TRUE;
                        }
                        else if((ComboKeyFlag == FALSE)&&(tKeyvFunction == emKeyFunc_repeat)&&(stKeyScanGroup[i].m_Flag&KeyFlag_ReFunction))
                        { // 按键长按连续发送相关功能
                            PlatformTimerSet(etKEY_CobomTime,100);
                            ComboKeyFlag = TRUE;
                        }

                        if((ComboKeyFlag == TRUE)&&(PlatformTimerGet(etKEY_CobomTime)==0))
                        {
                            ComboKeyFlag = FALSE;
                            //  按键长按功能，需要长按到一定时间后直接执行动作，所以这里直接给key赋值相应功能。
                            RealKeyValue = stKeyScanGroup[i].m_KSTypeKeyComboType;
                            //RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyComboType === %d\n",i,stKeyScanGroup[i].m_KSTypeKeyComboType);
                            KeyValue = emKeyFunc_None;
                            tKeyvFunction = emKeyFunc_repeat;
                        }
                        else
                            tKeyvFunction = emKeyFunc_repeat;
                    }
                    else
                #endif
                        tKeyvFunction = emKeyFunc_repeat;
                }
                else
                {
                    if((stKeyScanGroup[i].m_KSTypeMultiFuncEnable)&&(stKeyScanGroup[i].MFCondition()))
                    {
                        tKeyvFunction = stKeyScanGroup[i].m_KSTypeKeyMultiFuncType;
                        KeyValue = stKeyScanGroup[i].m_KSTypeKeyMultiFuncType;
                    }
                    else
                    {
                        tKeyvFunction = stKeyScanGroup[i].m_KSTypeKeyNomalType;
                        KeyValue = stKeyScanGroup[i].m_KSTypeKeyNomalType;
                    }
                    //RS232_printf("stKeyScanGroup[%d].m_KSTypeKeyNomalType === %d\n",i,stKeyScanGroup[i].m_KSTypeKeyNomalType);
                }
                break;
            }
        }
    }

    if(i>=(sizeof(stKeyScanGroup)/sizeof(stKeyScanGroup[0])))
    {
        if(KeyValue!=emKeyFunc_None)      //  按键松手后再给key赋值相应功能。
        {
            RealKeyValue = KeyValue;
            //RS232_printf("stKeyValue === %d\n",KeyValue);
            KeyValue = emKeyFunc_None;
        }
        else
        {
            //stKeyValue = emKeyTypeNothing;
            KeyValue = emKeyFunc_None;
        }
        PlatformTimerSet(etKEY_CobomTime,0);
        ComboKeyFlag = FALSE;
        tKeyvFunction = emKeyFunc_None;
    }

    if(emKeyFunc_None != RealKeyValue)
    {
        //RS232_printf("RealKeyValue == %d\r\n",RealKeyValue);
        //key_store_val(0,RealKeyValue);
        mapp_SetKeyFunction(RealKeyValue);
        RealKeyValue=emKeyFunc_None;
    }
    else
    {
        mapp_SetKeyFunction(emKeyFunc_None);
    }
}
#endif

void mapp_NewKeyScan(void)
{
    u32 l_u32KeypadValue[3] = {0,0,0};
    u16 l_u16KeyValue = 0;
    static u32 sl_u32PreKeyValue[3] = {0,0,0};
    //u8 l_u8AllKeyValue[2] = {0,0};
    u8 i;
    pstNewKeyDeclare_t l_pstKey = (pstNewKeyDeclare_t)&stProjectNewKeyTable[0];

    if(!(PlatformTimerExpired(etKEY_Detect)))
        return;
    for(i=0;i<emKeyName_max;i++)
    {
        pstGpioConfig_t l_psKeyPort = (pstGpioConfig_t)l_pstKey->pstNewKeyPort; // get key pin port

        if(l_pstKey->pstNewKeyPort != NULL)     // if port value not null
        {
            if(IS_MCU_GPIO(l_pstKey->pstNewKeyPort->u8PortType))    // is mcu pin?
            {
                l_u16KeyValue = GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_pstKey->pstNewKeyPort->u8PortType-PORT_GPIOA], l_pstKey->pstNewKeyPort->u16GpioPin);
                if(l_pstKey->eNewKeyOnLevel == ePinLevel_High)
                    l_u32KeypadValue[l_pstKey->u8KeyGroup] = (l_u16KeyValue == 1) ? (l_u32KeypadValue[l_pstKey->u8KeyGroup]|l_pstKey->u32KeyValue) : l_u32KeypadValue[l_pstKey->u8KeyGroup];
                else
                    l_u32KeypadValue[l_pstKey->u8KeyGroup] = (l_u16KeyValue == 0) ? (l_u32KeypadValue[l_pstKey->u8KeyGroup]|l_pstKey->u32KeyValue) : l_u32KeypadValue[l_pstKey->u8KeyGroup];
            }
            #if _EN_CHIP_TM1623_USE
            else if(IS_TM1623_KVAL(l_pstKey->pstNewKeyPort->u8PortType))    // is TM1623 PIN?
            {
                if((TM1623_ksg_reg_val[TM1623_DEV_INDEX(l_psKeyPort->u8PortType)][TM1623_PORT_NUM(l_psKeyPort->u8PortType)] & l_psKeyPort->u16GpioPin)!=0)
                {
#if 0
                    dbg_printf("TM1623_ksg_reg_val[%d][%d] = 0x%04x\n",TM1623_DEV_INDEX(l_psKeyPort->u8PortType),
                                TM1623_PORT_NUM(l_psKeyPort->u8PortType),
                                TM1623_ksg_reg_val[TM1623_DEV_INDEX(l_psKeyPort->u8PortType)][TM1623_PORT_NUM(l_psKeyPort->u8PortType)]);
#endif
                    l_u32KeypadValue[l_pstKey->u8KeyGroup] |= l_pstKey->u32KeyValue;
                }
            }
            #endif
        }
        else
            break;

        l_pstKey++;
    }


    for(i=0;i<3;i++)
    {
        if(sl_u32PreKeyValue[i] != l_u32KeypadValue[i])
        {
            sl_u32PreKeyValue[i] = l_u32KeypadValue[i];
            //RS232_printf("sl_u16KeyValue[%d] = 0x%04x\n",i,sl_u32PreKeyValue[i]);
        }
    }
    mapp_KeyValueScan(l_u32KeypadValue);
    PlatformTimerSet(etKEY_Detect,50);
}

static u8 sg_u8KeyFunctionName = emKeyFunc_None;
void mapp_SetKeyFunction(u8 u8KeyFunc)
{
    sg_u8KeyFunctionName = u8KeyFunc;
}

u8 mapp_GetKeyFunctionNum(void)
{
    return sg_u8KeyFunctionName;
}

void mapp_KeyFunctionProcess(void)
{
    mapp_NewKeyScan();
    mapp_KeyFunctionEvent(mapp_GetKeyFunctionNum());
    MUH0808SPro_FuncWork_LoopRunning();
}

void mapp_KeyFunctionEvent(u8 u8FuncKey)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SProSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(!pstBasicVar->bPowerOnSta)
    {
        return;
    }
    if((mapp_GetKeyFunctionNum()!=emKeyFunc_None)
            &&u8FuncKey!=emKeyFunc_Lock)
    {

        if(pstBasicVar->bIsLock)
        {
            u8 l_u8DisplayText[] = "System Locked    ";
            mapp_SetKeyFunction(emKeyFunc_None);
            //mapp_LCDBacklightEnable();
            LCD_LineRefresh((char *)l_u8DisplayText,LINE3);
            LCD_RecoverToDefault(1000);
            mapp_SetLedState(led_name_LedLock,TRUE);
            return;
        }
    }

	switch(u8FuncKey)
	{
        case emKeyFunc_In1:
        case emKeyFunc_In2:
        case emKeyFunc_In3:
        case emKeyFunc_In4:
        case emKeyFunc_In5:
        case emKeyFunc_In6:
        case emKeyFunc_In7:
        case emKeyFunc_In8:
            OperateStatsu(u8FuncKey-emKeyFunc_In1+1);
			break;

        case emKeyFunc_Out1:
        case emKeyFunc_Out2:
        case emKeyFunc_Out3:
        case emKeyFunc_Out4:
        case emKeyFunc_Out5:
        case emKeyFunc_Out6:
        case emKeyFunc_Out7:
        case emKeyFunc_Out8:
            OperateStatsu(u8FuncKey-emKeyFunc_Out1+IN_MAX+1);
            break;

        case emKeyFunc_All:
            dbg_printf("Key all\r\n");
            OperateStatsu(ALL);
            break;
        case emKeyFunc_Enter:
            dbg_printf("Key enter\r\n");
            OperateStatsu(ENTER);
            break;
        case emKeyFunc_Scene:
            dbg_printf("Key scene\r\n");
            OperateStatsu(SCENE);
            break;
        case emKeyFunc_Clear:
            dbg_printf("Key clear\r\n");
            OperateStatsu(CLEAR);
            break;
        case emKeyFunc_Mode:
            dbg_printf("Key mode\r\n");
            OperateStatsu(MODE);
            break;
        case emKeyFunc_Lock:
            dbg_printf("Key lock\r\n");
            OperateStatsu(LOCK);
            break;

        case emKeyFunc_CheckOutCh1:
        case emKeyFunc_CheckOutCh2:
        case emKeyFunc_CheckOutCh3:
        case emKeyFunc_CheckOutCh4:
        case emKeyFunc_CheckOutCh5:
        case emKeyFunc_CheckOutCh6:
        case emKeyFunc_CheckOutCh7:
        case emKeyFunc_CheckOutCh8:
            OperateStatsu(CHCEK_OUT_STA+u8FuncKey-emKeyFunc_CheckOutCh1);
            break;
		default:
			break;
	}

    if(mapp_GetKeyFunctionNum()!=emKeyFunc_None)
    {
        mapp_SetKeyFunction(emKeyFunc_None);
        mapp_LCDBacklightEnable();
    }
}

void mapp_LCDBacklightEnable(void)
{
    mapp_MUH0808SPro_PlatformTimerSet(emUserTimer_LcdBacklightTimeout,15000);
    g_bEnableLcdBacklight = TRUE;
    mapp_BacklightEN(TRUE);
}

void mapp_LCDBacklightMuteProcess(void)
{
    if(g_bEnableLcdBacklight)
    {
        if(mapp_MUH0808SPro_PlatformTimerExpired(emUserTimer_LcdBacklightTimeout))
        {
            mapp_BacklightEN(FALSE);
            g_bEnableLcdBacklight = FALSE;
        }
    }
}

/*----------------------------------------------------------------------------
Function: LCD line refresh
----------------------------------------------------------------------------*/
void LCD_LineRefresh(char *string, char address)
{
//    unsigned char l_u8StrDis[16];
//    memset(l_u8StrDis,' ',16);
//    strncpy((char*)l_u8StrDis,string,16);
//    display_string_8x16(address,1,0,l_u8StrDis);
    switch(address)
    {
        case LINE1:
            g_stLcdVar.bLcdEnable[0] = TRUE;
            memset(g_stLcdVar.pcDispString[0],' ',16);
            strncpy((char*)g_stLcdVar.pcDispString[0],string,16);
            break;
        case LINE2:
            g_stLcdVar.bLcdEnable[1] = TRUE;
            memset(g_stLcdVar.pcDispString[1],' ',16);
            strncpy((char*)g_stLcdVar.pcDispString[1],string,16);
            break;
        case LINE3:
            g_stLcdVar.bLcdEnable[2] = TRUE;
            memset(g_stLcdVar.pcDispString[2],' ',16);
            strncpy((char*)g_stLcdVar.pcDispString[2],string,16);
            break;
        case LINE4:
            g_stLcdVar.bLcdEnable[3] = TRUE;
            memset(g_stLcdVar.pcDispString[3],' ',16);
            strncpy((char*)g_stLcdVar.pcDispString[3],string,16);
            break;
        default:
            break;
    }
}

void mapp_SetLedState(u8 u8LedNum, bool bSta)
{
    if(bSta)
    {
        led_on(u8LedNum);
    }
    else
    {
        led_off(u8LedNum);
    }
}

void InputStatusHandle(void)
{
    char l_cDisplayText[16];
    u8 i = 0;

    KeyBuffValue.ALLKey = 0;
    KeyBuffValue.CLEARKey = 0;
    KeyBuffValue.EDIDKey = 0;
    KeyBuffValue.ENTERKey = 0;
    KeyBuffValue.OutputCnt = 0;
    for(i = 0; i < 32; i++)
    {
        KeyBuffValue.OutputKey[i] = 0;
    }
#if 0
    for (i = 0; i < 4; i++)
    {
        for(j = 0; j < 3; j++)
        {
            TM_DriverLEDPin.LEDByteSaveBuff[i][j] = 0;
        }
    }
#endif
    UITimeOutBuff.Flashing = 0;
    LCD_LineRefresh((char*)LcdNone,LINE3);
    LCD_LineRefresh((char*)LcdNone,LINE4);
    for(i=led_name_LedInput1;i<=led_name_LedEnter;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
    IN_channel  = KeyBuffValue.InputKey[0];
    mapp_SetLedState(led_name_LedInput1+IN_channel-1,TRUE);

    strncpy(l_cDisplayText, sys_type,16);   // LCD 屏幕显示内容
    LCD_LineRefresh(l_cDisplayText, LINE1);
    strncpy(l_cDisplayText, "In              ",16);
    l_cDisplayText[3] =	IN_channel/10+0x30;
    l_cDisplayText[4] =IN_channel%10+0x30;
    LCD_LineRefresh(l_cDisplayText,LINE2);

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);   //打开LCD背光倒计时8S
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
    UITimeOutBuff.UIStar = 1;
}

void FirstOutputStaHandle(void)
{
	u8  outputkeytempbuff = 0, i = 0;
    char l_cDisplayText[16];

    for(i=led_name_LedInput1;i<=led_name_LedAll;i++)
    {
        mapp_SetLedState(i,FALSE);
    }

    mapp_SetLedState(led_name_LedOutput1+KeyBuffValue.OutputKey[0]-1,TRUE);

    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    UITimeOutBuff.UIStar = 1;

    outputkeytempbuff = KeyBuffValue.OutputKey[0];    // 转换成1-32数字
    strncpy(l_cDisplayText, sys_type,16);   // LCD 屏幕显示内容
    LCD_LineRefresh(l_cDisplayText, LINE1);
    strncpy(l_cDisplayText, "Out             ",16);
    l_cDisplayText[4] = outputkeytempbuff/10+0x30;
    l_cDisplayText[5] = outputkeytempbuff%10+0x30;
    LCD_LineRefresh(l_cDisplayText,LINE3);
}
void OutputStatusHandle(void)
{
    char l_cDisplayText[16];
	u8  outputkeytempbuff = 0;

	outputkeytempbuff = KeyBuffValue.OutputKey[KeyBuffValue.OutputCnt - 1]-10;
    mapp_SetLedState(led_name_LedOutput1+outputkeytempbuff-1,TRUE);

	strncpy(l_cDisplayText, sys_type,16);   // LCD 屏幕显示内容
	LCD_LineRefresh(l_cDisplayText, LINE1);
	strncpy(l_cDisplayText, "Out             ",16);
	l_cDisplayText[4] = outputkeytempbuff/10+0x30;
	l_cDisplayText[5] = outputkeytempbuff%10+0x30;
	LCD_LineRefresh(l_cDisplayText,LINE3);

	LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
	UITimeOutBuff.UIStar = 1;
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}
void AllOutKeyStaHandle(void)
{
	u8 CNT = 0;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8  i = 0;//ByteCNT = 0, BitCNT = 0,;
    char l_cDisplayText[16];

    for(CNT = led_name_LedOutput1; CNT<=led_name_LedOutput8;CNT++)
        mapp_SetLedState(CNT,TRUE);

    mapp_SetLedState(led_name_LedAll,TRUE);
    IN_channel = KeyBuffValue.InputKey[KeyBuffValue.InputCnt-1];

    for(i=0;i<Def_outport_num;i++)
    {
         OUT_channel[i] = 0;
        if(IN_channel>0)
        {
            app_MUH0808SProSourceSwitch(i,IN_channel-1);
            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = IN_channel-1;
            USER_printf("out %d in %d\r\n",i+1,IN_channel);
        }
    }

    SetUserData_HdmiSel();

	strncpy(l_cDisplayText, sys_type,16);   // LCD 屏幕显示内容
	LCD_LineRefresh(l_cDisplayText, LINE1);
	strncpy(l_cDisplayText, "Out  All        ",16);
	LCD_LineRefresh(l_cDisplayText,LINE3);
    strncpy(l_cDisplayText, "Switch OK       ",16);
    LCD_LineRefresh(l_cDisplayText,LINE4);

	LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void ClearStatusHandle(void)
{
    u8 i = 0;
    char l_cDisplayText[16];

    for(i=led_name_LedInput1;i<=led_name_LedScene;i++)
    {
        mapp_SetLedState(i,FALSE);
    }

    mapp_SetLedState(led_name_LedClear,TRUE);

    strncpy(l_cDisplayText, "Clear           ",16);
    LCD_LineRefresh(l_cDisplayText,LINE2);
    strncpy(l_cDisplayText, LcdNone,16);   //LCD显示
    LCD_LineRefresh(l_cDisplayText,LINE3);
    strncpy(l_cDisplayText, LcdNone,16);   //LCD显示
    LCD_LineRefresh(l_cDisplayText,LINE4);

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);

}
void EnterCheckChannleHandle(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8  i = 0;

    for(i=led_name_LedInput1;i<=led_name_LedEnter;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
    mapp_SetLedState(led_name_LedEnter,TRUE);

    OUT_num = KeyBuffValue.OutputKey[0];
    mapp_SetLedState(led_name_LedOutput1+OUT_num-1,TRUE);
    mapp_SetLedState(led_name_LedInput1+pstPrjSaveVar->stHdmiSel.u8SwChannel[OUT_num-1],TRUE);
    USER_printf("out %d in %d\r\n",OUT_num,pstPrjSaveVar->stHdmiSel.u8SwChannel[OUT_num-1]+1);
    g_bEnableCheckChannel = TRUE;
    OUT_num =  0;
    KeyStatus = Empt;  // 按键状态清零

//    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterCheckChannleHandle2(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8  i = 0;

    for(i=led_name_LedInput1;i<=led_name_LedEnter;i++)
    {
        mapp_SetLedState(i,FALSE);
    }

    OUT_num = KeyBuffValue.OutputKey[0];
    mapp_SetLedState(led_name_LedOutput1+OUT_num-1,TRUE);
    mapp_SetLedState(led_name_LedInput1+pstPrjSaveVar->stHdmiSel.u8SwChannel[OUT_num-1],TRUE);

    USER_printf("out %d in %d\r\n",OUT_num,pstPrjSaveVar->stHdmiSel.u8SwChannel[OUT_num-1]+1);
    g_bEnableCheckChannel = TRUE;
    OUT_num =  0;
    KeyStatus = Empt;  // 按键状态清零

    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterSwitchStatusHandle(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i = 0;
    char l_cDisplayText[16];

    mapp_SetLedState(led_name_LedEnter,TRUE);

    IN_channel = KeyBuffValue.InputKey[0];

    if (KeyBuffValue.OutputCnt == 1)  // 1V1
    {
    	OUT_channel[0] = KeyBuffValue.OutputKey[0] - 10;

        if (OUT_channel[0] > 0)
        {
            app_MUH0808SProSourceSwitch(OUT_channel[0]-1,IN_channel-1);
            pstPrjSaveVar->stHdmiSel.u8SwChannel[OUT_channel[0]-1] = IN_channel-1;
            USER_printf("out %d in %d\r\n",OUT_channel[0],IN_channel);
        }
    }
    else
    {
        for ( i = 0; i < KeyBuffValue.OutputCnt; i++ )
        {
            OUT_channel[i] = KeyBuffValue.OutputKey[i] - 10;

            if (OUT_channel[i] > 0)
            {
                app_MUH0808SProSourceSwitch(OUT_channel[i]-1,IN_channel-1);
                pstPrjSaveVar->stHdmiSel.u8SwChannel[OUT_channel[i]-1] = IN_channel-1;
                USER_printf("out %d in %d\r\n",OUT_channel[i],IN_channel);
            }
        }

        for(i = 0; i < KeyBuffValue.OutputCnt; i++)
        {
            if(OUT_channel[i] > 0)
            {
            }
         }
    }

    for(i=0;i<Def_outport_num;i++)   OUT_channel[i] = 0;
    SetUserData_HdmiSel();

    strncpy(l_cDisplayText, "Switch OK       ",16);
    LCD_LineRefresh(l_cDisplayText,LINE4);

    UITimeOutBuff.Flashing = 120;  //开闪烁
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterAllSwitchStatusHandle(void)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8  i = 0;//ByteCNT = 0, BitCNT = 0,;
    char l_cDisplayText[16];

    mapp_SetLedState(led_name_LedEnter,TRUE);
    IN_channel = KeyBuffValue.InputKey[KeyBuffValue.InputCnt-1];

    for(i=0;i<Def_outport_num;i++)
    {
         OUT_channel[i] = 0;
        if(IN_channel>0)
        {
            app_MUH0808SProSourceSwitch(i,IN_channel-1);
            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = IN_channel-1;
            USER_printf("out %d in %d\r\n",i+1,IN_channel);
        }
    }

    SetUserData_HdmiSel();

    strncpy(l_cDisplayText, "Switch OK       ",16);
    LCD_LineRefresh(l_cDisplayText,LINE4);

    UITimeOutBuff.Flashing = 120;  // 开闪烁
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);

    KeyStatus = Empt;  // 按键状态清零
}

void EnterEmptStatusHandle(void)
{
    mapp_SetLedState(led_name_LedEnter,TRUE);
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}
void ErrorUseStaHandle(void)
{
    char l_cDisplayText[16];
	memcpy(l_cDisplayText, sys_type,16);   //LCD显示
	LCD_LineRefresh(l_cDisplayText,LINE1);
	strncpy(l_cDisplayText, "ERROR           ",16);
	LCD_LineRefresh(l_cDisplayText,LINE2);
    strncpy(l_cDisplayText, LcdNone,16);   //LCD显示
    LCD_LineRefresh(l_cDisplayText,LINE3);
    strncpy(l_cDisplayText, LcdNone,16);   //LCD显示
    LCD_LineRefresh(l_cDisplayText,LINE4);

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
	UITimeOutBuff.UIStar = 1;
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void LockStatusHandle(void)
{
	u8 i = 0;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SProSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char l_cDisplayText[16];

    for(i=led_name_LedInput1;i<=led_name_LedClear;i++)
    {
        mapp_SetLedState(i,FALSE);
    }

    if(!pstBasicVar->bIsLock)
	{
        mapp_SetLedState(led_name_LedLock,TRUE);
		strncpy(l_cDisplayText, sys_type,16);   //LCD显示
		LCD_LineRefresh(l_cDisplayText,LINE1);
		strncpy(l_cDisplayText, "System Locked   ",16);
		LCD_LineRefresh(l_cDisplayText,LINE3);

        LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
//        LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
		pstBasicVar->bIsLock=TRUE;
	}
	else
	{
        mapp_SetLedState(led_name_LedLock,FALSE);
		strncpy(l_cDisplayText, sys_type,16);   //LCD显示
		LCD_LineRefresh(l_cDisplayText,LINE1);
		strncpy(l_cDisplayText, "System Unlocked ",16);
		LCD_LineRefresh(l_cDisplayText,LINE3);

        LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
        LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
		pstBasicVar->bIsLock=FALSE;
	}
    SetUserData_SpecBasicVar();
}

void SceneStatusHandle(void)
{
    u8  i = 0;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SProSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char l_cDisplayText[16];

    for(i=led_name_LedInput1;i<led_name_End;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
    mapp_SetLedState(led_name_LedScene,TRUE);
	LCD_LineRefresh("Scene Select    ",LINE1);
    LCD_LineRefresh((char*)LcdNone,LINE2);
    LCD_LineRefresh((char*)LcdNone,LINE3);
    LCD_LineRefresh((char*)LcdNone,LINE4);
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);

    if(pstBasicVar->stScenePollingVar.bPollingEnable)
    {
        pstBasicVar->stScenePollingVar.bPollingEnable = FALSE;
        g_u8SceneStartIndex = 0;
        USER_printf("scene polling disable\r\n");
        strncpy(l_cDisplayText, "Polling disable ",16);
        LCD_LineRefresh(l_cDisplayText,LINE3);

        LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
        LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
    }
}
void SceneIdxInputStatusHandle(void)
{
    char l_cDisplayText[16];
    u8 i = 0;
    for(i=led_name_LedInput1;i<=led_name_LedEnter;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
	IN_channel = KeyBuffValue.InputKey[0];
    mapp_SetLedState(led_name_LedInput1+KeyBuffValue.InputKey[0]-1,TRUE);

    strncpy(l_cDisplayText, "Call:           ",16);
    l_cDisplayText[5] =	IN_channel/10+0x30;
    l_cDisplayText[6] =IN_channel%10+0x30;
    LCD_LineRefresh(l_cDisplayText,LINE2);
    KeyStatus = SceneIdxInputStatus;

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterSceneCallStatusHandle(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SProSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char l_cDisplayText[16];

    mapp_SetLedState(led_name_LedEnter,TRUE);
    IN_channel = KeyBuffValue.InputKey[0];
    if(IN_channel<=32&&IN_channel>=1)
    {
        if(pstBasicVar->stScene[IN_channel-1].u8SceneSaveFlag == TRUE)
        {
            app_MUH0808SProSceneCall(IN_channel-1);

            USER_printf("scene %d called\r\n",IN_channel);
        }
    }

    strncpy(l_cDisplayText, "OK              ",16);
    LCD_LineRefresh(l_cDisplayText,LINE3);

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void SceneIdxOutputStatusHandle(void)
{
    char l_cDisplayText[16];
    u8 i = 0;
    for(i=led_name_LedInput1;i<=led_name_LedEnter;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
	IN_channel = KeyBuffValue.OutputKey[0];
    mapp_SetLedState(led_name_LedOutput1+KeyBuffValue.OutputKey[0]-1,TRUE);

    strncpy(l_cDisplayText, "Save:           ",16);
    l_cDisplayText[5] =	IN_channel/10+0x30;
    l_cDisplayText[6] =IN_channel%10+0x30;
    LCD_LineRefresh(l_cDisplayText,LINE2);
    KeyStatus = SceneIdxOutputStatus;

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterSceneSaveStatusHandle(void)
{
    char l_cDisplayText[16];
    mapp_SetLedState(led_name_LedEnter,TRUE);
    IN_channel = KeyBuffValue.OutputKey[0];
    if(IN_channel<=32&&IN_channel>=1)
    {
        app_MUH0808SProSceneSave(IN_channel-1);
        USER_printf("scene %d saved\r\n",IN_channel);
    }

    strncpy(l_cDisplayText, "OK              ",16);
    LCD_LineRefresh(l_cDisplayText,LINE3);

//    UITimeOutBuff.Flashing = 120;  // 开闪烁
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void ModeChangeStatusHandle(void)
{
    u8  i = 0;
    char l_cDisplayText[16];

    for(i=led_name_LedInput1;i<led_name_End;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
    mapp_SetLedState(led_name_LedMode,TRUE);
	strncpy(l_cDisplayText, "Mode Select    ",16);
	LCD_LineRefresh(l_cDisplayText,LINE1);
    strncpy(l_cDisplayText, LcdNone,16);
    LCD_LineRefresh(l_cDisplayText,LINE2);
    strncpy(l_cDisplayText, LcdNone,16);   //LCD显示
    LCD_LineRefresh(l_cDisplayText,LINE3);
    strncpy(l_cDisplayText, LcdNone,16);   //LCD显示
    LCD_LineRefresh(l_cDisplayText,LINE4);
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void ModeIdxInputStatusHandle(void)
{
    char l_cDisplayText[16];
    u8 i = 0;
    for(i=led_name_LedInput1;i<=led_name_LedEnter;i++)
    {
        mapp_SetLedState(i,FALSE);
    }
	IN_channel = KeyBuffValue.InputKey[0];
    mapp_SetLedState(led_name_LedMode,TRUE);
    mapp_SetLedState(led_name_LedInput1+KeyBuffValue.InputKey[0]-1,TRUE);

    strncpy(l_cDisplayText, "Save:           ",16);
    l_cDisplayText[5] =	IN_channel/10+0x30;
    l_cDisplayText[6] =IN_channel%10+0x30;
    LCD_LineRefresh(l_cDisplayText,LINE2);
    KeyStatus = ModeIdxInputStatus;

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterModeSaveStatusHandle(void)
{
    char l_cDisplayText[16];

    mapp_SetLedState(led_name_LedEnter,TRUE);
    IN_channel = KeyBuffValue.InputKey[0];
    if(IN_channel<=32&&IN_channel>=1)
    {
        app_MUH0808SProSceneSave(IN_channel-1);
        USER_printf("scene %d saved\r\n",IN_channel);
    }

    strncpy(l_cDisplayText, "OK              ",16);
    LCD_LineRefresh(l_cDisplayText,LINE3);

//    UITimeOutBuff.Flashing = 120;  // 开闪烁
    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

void EnterScenePollingHandle(void)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SProSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char l_cDisplayText[16];

    mapp_SetLedState(led_name_LedEnter,TRUE);
    if(!pstBasicVar->stScenePollingVar.bPollingEnable)
    {
        pstBasicVar->stScenePollingVar.bPollingEnable = TRUE;
        g_u8SceneStartIndex = 0;
        USER_printf("scene polling enable\r\n");
        strncpy(l_cDisplayText, "Polling enable  ",16);
        LCD_LineRefresh(l_cDisplayText,LINE3);

        LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
        LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
    }
    else
    {
        pstBasicVar->stScenePollingVar.bPollingEnable = FALSE;
        g_u8SceneStartIndex = 0;
        USER_printf("scene polling disable\r\n");
        strncpy(l_cDisplayText, "Polling disable ",16);
        LCD_LineRefresh(l_cDisplayText,LINE3);
    }
    SetUserData_SpecBasicVar();

    LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
    LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
}

static const keyStaProcessLib UIStaProcessList[LastList]=
{
    {InputStatus, InputStatusHandle},
    {FirstOutputSta, FirstOutputStaHandle},
    {OutputStatus, OutputStatusHandle},
//    {FirstAllKeyState, FirstAllKeyStateHandle},
    {AllOutKeyStatus, AllOutKeyStaHandle},
//    {AllInKeyStatus, AllInKeyStaHandle},
//    {EdidStartStatus, EdidStartStaHandle},
//    {EdidCpyStatus, EdidCpyStatusHandle},
//    {EdidEndStatus, EdidEndStatusHandle},
    {ClearStatus, ClearStatusHandle},
    {EnterCheckChannle, EnterCheckChannleHandle},
    {EnterSwitchStatus, EnterSwitchStatusHandle},
//    {EnterAllSwitchStatus, EnterAllSwitchStatusHandle},
//    {EnterEdidStatus, EnterEdidStatusHandle},
    {EnterEmptStatus, EnterEmptStatusHandle},
    {ErrorUseStatus, ErrorUseStaHandle},
    {LockStatus, LockStatusHandle},
    {SceneStatus,SceneStatusHandle},
	{SceneIdxInputStatus,SceneIdxInputStatusHandle},
    {EnterSceneCallStatus,EnterSceneCallStatusHandle},
    {SceneIdxOutputStatus,SceneIdxOutputStatusHandle},
    {EnterSceneSaveStatus,EnterSceneSaveStatusHandle},
    {EnterCheckChannle2,EnterCheckChannleHandle2},
    {ModeStatus,ModeChangeStatusHandle},
    {ModeIdxInputStatus,ModeIdxInputStatusHandle},
    {EnterModeSaveStatus,EnterModeSaveStatusHandle},
    {EnterScenePollingStatus,EnterScenePollingHandle},
};

void ClearUIDataFunc(void)
{
     uint8_t i = 0;

    KeyStatus = Empt;  // 按键状态清零
//    KeyBuffValue ={{0},0,{0},0,0,0,0,0};  //清除保存的按键值
    KeyBuffValue.ALLKey = 0;
    KeyBuffValue.CLEARKey = 0;
    KeyBuffValue.EDIDKey = 0;
    KeyBuffValue.ENTERKey = 0;

    KeyBuffValue.OutputCnt = 0;
    KeyBuffValue.InputCnt = 0;

    for(i = 0; i < Def_outport_num; i++)
    {
        KeyBuffValue.OutputKey[i] = 0;
    }

    for(i = 0; i < _HDMI_INPUT_PORT_USED_NUMBER; i++)
    {
        KeyBuffValue.InputKey[i] = 0;
    }

}

void OperateStatsu(uint8_t keyValue)
{
    unsigned char i = 0;
    uint8_t erroFlag = 0;

    dbg_printf("OperateStatsu keyValue == %d\r\n",keyValue);

    if(keyValue >0 && keyValue <= IN_MAX)//1~10 in
    {
        for (i = 0; i < _HDMI_INPUT_PORT_USED_NUMBER; i++)
        {
            if (  i+1 == keyValue)
            {
                erroFlag = 1;

                KeyBuffValue.InputKey[0] = keyValue;
                KeyBuffValue.InputCnt = 1;

                if (KeyStatus == Empt ||
                KeyStatus == InputStatus ||
                KeyStatus == EnterSwitchStatus ||
                KeyStatus == EnterAllSwitchStatus ||
                KeyStatus == AllOutKeyStatus ||
                KeyStatus == EnterEmptStatus ||
                KeyStatus == ErrorUseStatus ||
                KeyStatus == FirstAllKeyState ||
                KeyStatus == FirstOutputSta)
                {
                    KeyStatus = InputStatus;
                }
                else if(KeyStatus == SceneStatus)
                    KeyStatus = SceneIdxInputStatus;
                else if(KeyStatus == SceneIdxInputStatus)
                    KeyStatus = SceneIdxInputStatus;
                else if(KeyStatus == ModeStatus)
                    KeyStatus = ModeIdxInputStatus;
                else if(KeyStatus == ModeIdxInputStatus)
                    KeyStatus = ModeIdxInputStatus;
                else
                    KeyStatus = ErrorUseStatus;
            }
        }
        if(erroFlag != 1)
            KeyStatus = ErrorUseStatus;
    }
    else if(keyValue > IN_MAX && keyValue <=OUT_MAX) //11~20 out
    {
        for (i = 0; i < Def_outport_num; i++ )
        {
            if (   i+11  == keyValue)
            {
                erroFlag = 1;
                if (KeyStatus == Empt ||KeyStatus == FirstOutputSta)
                {
                    KeyStatus = FirstOutputSta;
                    KeyBuffValue.OutputKey[0] = keyValue - IN_MAX;
                    //KeyBuffValue.OutputCnt++;
                }
                else if (KeyStatus == InputStatus || KeyStatus == OutputStatus )
                {
                    KeyStatus = OutputStatus;
                    KeyBuffValue.OutputKey[KeyBuffValue.OutputCnt] =  keyValue;
                    KeyBuffValue.OutputCnt++;
                }
//                else if(KeyStatus == SceneStatus)
//                {
//                    KeyStatus = SceneIdxOutputStatus;
//                    KeyBuffValue.OutputKey[0] = keyValue-IN_MAX;
//                }
//                else if (KeyStatus == EdidCpyStatus || KeyStatus == AllInKeyStatus)
//                {
//                    KeyStatus = EdidEndStatus;
//                    KeyBuffValue.OutputKey[KeyBuffValue.OutputCnt] = keyValue;
//                    KeyBuffValue.OutputCnt++;
//                }
                else
                    KeyStatus = ErrorUseStatus;
            }
        }
        if(erroFlag != 1)
            KeyStatus = ErrorUseStatus;
    }
    else if(keyValue > 20 && keyValue < 29)//21~29
    {
        switch(keyValue)
        {
            case  ALL://21
                {
                    KeyBuffValue.ALLKey = keyValue;
                    if (KeyStatus == Empt)
                        KeyStatus = FirstAllKeyState;
                    else if (KeyStatus == InputStatus )
                        KeyStatus = AllOutKeyStatus;
                    else if (KeyStatus == EdidStartStatus )
                        KeyStatus = AllInKeyStatus;
                    else
                        KeyStatus = ErrorUseStatus;
                }
                break;

            case LOCK:
                {
                    KeyBuffValue.LockKey = keyValue;
                    KeyStatus = LockStatus;
                }
                break;
            case  CLEAR:
                {
                    KeyBuffValue.CLEARKey = keyValue;
                    KeyStatus = ClearStatus;
                }
                break;

            case ENTER:
                {
                    KeyBuffValue.ENTERKey = keyValue;
                    if (KeyStatus == OutputStatus )
                        KeyStatus = EnterSwitchStatus;
//                    else if (KeyStatus == AllOutKeyStatus)
//                        KeyStatus = EnterAllSwitchStatus;
                    else if (KeyStatus == SceneIdxInputStatus)
                        KeyStatus = EnterSceneCallStatus;
                    else if (KeyStatus == Empt)
                        KeyStatus = EnterEmptStatus;
                    else if (KeyStatus == FirstOutputSta)
                        KeyStatus = EnterCheckChannle;
//                    else if (KeyStatus == SceneIdxOutputStatus)
//                        KeyStatus = EnterSceneSaveStatus;
                    else if(KeyStatus == ModeIdxInputStatus)
                        KeyStatus = EnterModeSaveStatus;
                    else if (KeyStatus == SceneStatus)
                        KeyStatus = EnterScenePollingStatus;
                    else
                        KeyStatus = ErrorUseStatus;
                }
                break;

            case SCENE:
                {
                    KeyStatus = SceneStatus;
                }
                break;
            default:
                break;
        }
    }
    else if(keyValue >= CHCEK_OUT_STA && keyValue <= CHCEK_OUT_MAX)
    {
        KeyBuffValue.OutputKey[0] = keyValue - CHCEK_OUT_STA+1;
        KeyStatus = EnterCheckChannle2;
    }
    else if(keyValue ==MODE )
    {
        KeyStatus = ModeStatus;
    }

    for (i = 0; i < LastList; i++)
    {
        if(KeyStatus == UIStaProcessList[i].Number)
        {
            if(UIStaProcessList[i].FunOperate != NULL)
            {
                UIStaProcessList[i].FunOperate();
            }
        }
    }
}

void LCD_RecoverToDefault(uint16_t u16Timeout)
{
    UITimeOutBuff.LCDStar = 1;
    mapp_MUH0808SPro_PlatformTimerSet(emUserTimer_LcdRefreshTime,u16Timeout);
}

void LED_RecoverToDefault(uint16_t u16Timeout)
{
    UITimeOutBuff.LEDStar = 1;
    mapp_MUH0808SPro_PlatformTimerSet(emUserTimer_LedRefreshTime,u16Timeout);
}

void led_handle(void)
{
    uint8_t i;

    mapp_LCDBacklightMuteProcess();
    for(i=0;i<4;i++)
    {
        if(g_stLcdVar.bLcdEnable[i])
        {
            g_stLcdVar.bLcdEnable[i] = FALSE;
            switch(i)
            {
                case 0:
                    display_string_8x16(LINE1,1,0,(unsigned char*)g_stLcdVar.pcDispString[i]);
                    break;
                case 1:
                    display_string_8x16(LINE2,1,0,(unsigned char*)g_stLcdVar.pcDispString[i]);
                    break;
                case 2:
                    display_string_8x16(LINE3,1,0,(unsigned char*)g_stLcdVar.pcDispString[i]);
                    break;
                case 3:
                    display_string_8x16(LINE4,1,0,(unsigned char*)g_stLcdVar.pcDispString[i]);
                    break;
                default:
                    break;
            }
        }
    }

    if(UITimeOutBuff.LEDStar)
    {
        if(mapp_MUH0808SPro_PlatformTimerExpired(emUserTimer_LedRefreshTime))
        {
            for(i=led_name_LedInput1;i<led_name_End;i++)
            {
                mapp_SetLedState(i,FALSE);
            }
            UITimeOutBuff.LEDStar = 0;
        }
    }

    if(UITimeOutBuff.LCDStar)
    {
        if(mapp_MUH0808SPro_PlatformTimerExpired(emUserTimer_LcdRefreshTime))
        {
            pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
            pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
            pstProjectNameSet_t         pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;
            char l_cDisplayText[16];

            UITimeOutBuff.LCDStar = 0;

            LCD_LineRefresh(sys_type,LINE1);
            if(pstPrjNameSet->sPrjName[0]!=0&&pstPrjNameSet->sPrjName[1]!=0)
            {
                strncpy(l_cDisplayText, (char*)pstPrjNameSet->sPrjName,16);
            }
            else
            {
                strncpy(l_cDisplayText, LcdNone,16);
            }
            LCD_LineRefresh(l_cDisplayText,LINE2);
            LCD_LineRefresh((char*)LcdNone,LINE3);
            LCD_LineRefresh((char*)LcdNone,LINE4);

            if(KeyStatus != Empt)
            {
                KeyStatus = Empt;
            }
        }
    }
}


