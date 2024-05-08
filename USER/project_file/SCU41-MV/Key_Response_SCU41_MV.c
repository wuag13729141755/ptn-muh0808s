#include "includes.h"

extern stProjectGlobalAllVar_t g_stPrjGloVar;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _emKeyValueType
{
    emKeyvalueInput1    = BIT0,//BIT0,
    emKeyvalueInput2    = BIT1,
    emKeyvalueInput3    = BIT2,
    emKeyvalueInput4    = BIT3,
    emKeyvalueSwAuto    = BIT4,
    emKeyvalueWinA      = BIT5,
    emKeyvalueWinB      = BIT6,
    emKeyvalueWinC      = BIT7,
    emKeyvalueWinD      = BIT8,
    emKeyvalueSwap      = BIT9,
    emKeyvalueResize    = BIT10,
    emKeyvalueMode1     = BIT11,
    emKeyvalueMode2     = BIT12,
    emKeyvalueMode3     = BIT13,

}emKeyValueType;

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

eKeyValSource_t g_emSourceKeyType = eKeyValSource_Panel;

#endif

bool isFrontPanelLocked(pkey_val_stored_t ptKeyVal, uint8_t u8KeyIndex)
{
	pstProjectSpecVar_t		pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		pstBasicVar = &pstSpecVar->stBasSpecVar;

    #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
    if(pstBasicVar->bIsLock
        &&(g_emSourceKeyType == eKeyValSource_Panel
        ||g_emSourceKeyType == eKeyValSource_IrRemote))
    {
        switch(u8KeyIndex)
        {
            case emKeyFunc_GetOutputSate:
            case emKeyFunc_NoMatchChipID:
            case emKeyFunc_UnsupportResize:
            case emKeyFunc_BaudwidthIsOver:
            case emKeyFunc_WinLevelIsTheSame:
            case emKeyFunc_PrintPowerState:
                return FALSE;
            default:
                return TRUE;
        }
    }
    #else
	if((pstBasicVar->bIsLock)&&\
		(((ptKeyVal->StStoreVal[u8KeyIndex].eValSource==eKeyValSource_Panel)
		)
		||(ptKeyVal->StStoreVal[u8KeyIndex].eValSource==eKeyValSource_IrRemote)))
	{
		return TRUE;
	}
    #endif

	return FALSE;
}

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
#if 1
#define CtrlComboTime       (3*1000)
#define RepeatComboTime     (1*1000)
#define EdidComboTime       (3*1000)

// 某些条件出发按键的第二功能，这里需要把条件放到相应的项里面
bool unUseMultiKeyFunc(void)
{
    return FALSE;
}

bool OnlySwitchWinPortToFullFlag(void)
{
    if(SCU41_MV_GetSelectPortFlag()== TRUE)
        return TRUE;

    return FALSE;
}

// 按键功能定义，第一组数据跳过，程序会读错数值，具体不知道什么原因。
static stKeyScanType stKeyScanGroup[]=
{
    /*Group       Value               KeyNomalType             ComboEnable     ComboTime          KeyComboType    MultiFuncEnable       MFConditionFlag       KeyMultiFuncType   m_Flag*/
    {  0,           0,                          emKeyFunc_None,     FALSE,              0,           emKeyFunc_None,    FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_NoUse},
    {  0,   emKeyvalueInput1,                 emKeyFunc_Input1,      TRUE,  CtrlComboTime,   emKeyFunc_AudioSrcHdmi1,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput2,                 emKeyFunc_Input2,      TRUE,  CtrlComboTime,   emKeyFunc_AudioSrcHdmi2,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput3,                 emKeyFunc_Input3,      TRUE,  CtrlComboTime,   emKeyFunc_AudioSrcHdmi3,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput4,                 emKeyFunc_Input4,      TRUE,  CtrlComboTime,   emKeyFunc_AudioSrcHdmi4,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueSwAuto,             emKeyFunc_AutoSwitch,      TRUE,  CtrlComboTime,  emKeyFunc_AudioSrcExtern,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,     emKeyvalueWinA,   emKeyFunc_SetInputToFullByWinA,      TRUE,  CtrlComboTime,  emKeyFunc_GetInputByWinA,    TRUE,  OnlySwitchWinPortToFullFlag,   emKeyFunc_WinA,  KeyFlag_Nomal},
    {  0,     emKeyvalueWinB,   emKeyFunc_SetInputToFullByWinB,      TRUE,  CtrlComboTime,  emKeyFunc_GetInputByWinB,    TRUE,  OnlySwitchWinPortToFullFlag,   emKeyFunc_WinB,  KeyFlag_Nomal},
    {  0,     emKeyvalueWinC,   emKeyFunc_SetInputToFullByWinC,      TRUE,  CtrlComboTime,  emKeyFunc_GetInputByWinC,    TRUE,  OnlySwitchWinPortToFullFlag,   emKeyFunc_WinC,  KeyFlag_Nomal},
    {  0,     emKeyvalueWinD,   emKeyFunc_SetInputToFullByWinD,      TRUE,  CtrlComboTime,  emKeyFunc_GetInputByWinD,    TRUE,  OnlySwitchWinPortToFullFlag,   emKeyFunc_WinD,  KeyFlag_Nomal},
    {  0,     emKeyvalueSwap,                   emKeyFunc_Swap,      TRUE,  CtrlComboTime,       emKeyFunc_ResetPort,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueResize,                 emKeyFunc_Resize,     FALSE,              0,            emKeyFunc_None,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvalueMode1,                emKeyFunc_MVMode1,     FALSE,              0,            emKeyFunc_None,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvalueMode2,                emKeyFunc_MVMode2,     FALSE,              0,            emKeyFunc_None,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvalueMode3,                emKeyFunc_MVMode3,     FALSE,              0,            emKeyFunc_None,   FALSE,            unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
};

void mapp_KeyValueScan(u32 *u32KeyPadVaule)
{
    static emKeyFunction tKeyvFunction=emKeyFunc_None;
    static bool ComboKeyFlag = FALSE;
    static emKeyFunction KeyValue = emKeyFunc_None;
    static emKeyFunction RealKeyValue = emKeyFunc_None;
    static u16 PreKeyScanTypeValue=0xFF;
    static u16 PreKeyScanTypeValue2=0xFF;
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
            if(u32KeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup] == stKeyScanGroup[i].m_KSTypeValue)
            {
                if((u32KeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup] != PreKeyScanTypeValue)
                    ||PreKeyScanTypeValue2!=PreKeyScanTypeValue)
                {
                    //PreKeyScanTypeValue = tKeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup];
                    //RS232_printf("PreKeyScanTypeValue[%d]=%d\n",i,PreKeyScanTypeValue);

                    if(PreKeyScanTypeValue2 == PreKeyScanTypeValue) // modify by wf8421 20181210
                        PreKeyScanTypeValue2 = u32KeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup];
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
                                PreKeyScanTypeValue = u32KeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup];
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
            if(u32KeyPadVaule[stKeyScanGroup[i].m_KSTypeGroup] == stKeyScanGroup[i].m_KSTypeValue)
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
        g_emSourceKeyType = eKeyValSource_Panel;
        pITE663xx_STM32_api->SysKeyIn(RealKeyValue);
        RealKeyValue=emKeyFunc_None;
    }
}
#endif

void mapp_NewKeyScan(void)
{
    u32 l_u32KeypadValue[3] = {0,0,0};
    u16 l_u16KeyValue = 0;
    static u32 sl_u32PreKeyValue[3] = {0,0,0};
    u8 i;
    pstNewKeyDeclare_t l_pstKey = (pstNewKeyDeclare_t)&stProjectNewKeyTable[0];

    if(!(PlatformTimerExpired(etKEY_Detect)))
        return;
    for(i=0;i<emKeyName_max;i++)
    {
        pstGpioConfig_t l_psKeyPort = (pstGpioConfig_t)l_pstKey->pstNewKeyPort;
        if(l_pstKey->pstNewKeyPort != NULL)
        {
            if((l_psKeyPort->u8PortType>=PORT_GPIOA)&&(l_psKeyPort->u8PortType<=PORT_GPIOG))
            {
                l_u16KeyValue = GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_pstKey->pstNewKeyPort->u8PortType-PORT_GPIOA], l_pstKey->pstNewKeyPort->u16GpioPin);
                if(l_pstKey->eNewKeyOnLevel == ePinLevel_High)
                    l_u32KeypadValue[l_pstKey->u8KeyGroup] = (l_u16KeyValue == 1) ? (l_u32KeypadValue[l_pstKey->u8KeyGroup]|l_pstKey->u32KeyValue) : l_u32KeypadValue[l_pstKey->u8KeyGroup];
                else
                    l_u32KeypadValue[l_pstKey->u8KeyGroup] = (l_u16KeyValue == 0) ? (l_u32KeypadValue[l_pstKey->u8KeyGroup]|l_pstKey->u32KeyValue) : l_u32KeypadValue[l_pstKey->u8KeyGroup];
            }
            #if _EN_CHIP_TM1623_USE
            else if((l_psKeyPort->u8PortType>=PORT_TM1623_KVAL_0)&&(l_psKeyPort->u8PortType<=PORT_TM1623_KVAL_4))
            {
                //TM1623_Read_Key();
                if((TM1623_ksg_reg_val[TM1623_PORT_KS1+l_psKeyPort->u8PortType-PORT_TM1623_KVAL_0] & l_psKeyPort->u16GpioPin)!=0)
                {
                    //RS232_printf("TM1623_ksg_reg_val = 0x%04x\n",TM1623_ksg_reg_val[TM1623_PORT_KS1+l_psKeyPort->u8PortType-PORT_TM1623_KVAL_0]);
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
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
#define CustomCode      0x02FF
typedef struct _stIrKeyScanType   // wf8421 20190222
{
    u16 m_u16CustomerCode;
    u8  m_u8CmdCode;
    u8  m_u8IrKeyNomalFuncID;
    bool  m_bEnableRepeat;
    u16  m_u16IrKeyRepeatTime;
    u8  m_u8RepeatFuncID;
    bool m_bEnableMultiFunc;
    bool (*MFCondition)(void);
    u8 m_u8IrKeyMultiFuncID;
    u8  m_Flag;
}stIrKeyScanType;

// 某些条件出发按键的第二功能，这里需要把条件放到相应的项里面
bool unUseMultiIrKeyFunc(void)
{
    return FALSE;
}

bool IrKeyOnlySwitchWinKeyToFullFlag(void)
{
    if(SCU41_MV_GetSelectPortFlag()== TRUE)
        return TRUE;

    return FALSE;
}

stIrKeyScanType g_stIrKeyScanTable[] =
{
//  customercode    cmd     NomalFuncID         EnableRepeat RepeatTime  RepeatFuncID    EnableMultiFunc  MFCondition     MultiFuncID  Flag
    {0,                0,                   emKeyFunc_None,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x20,                 emKeyFunc_Input1,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x62,                 emKeyFunc_Input2,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xE2,                 emKeyFunc_Input3,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xC2,                 emKeyFunc_Input4,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x00,             emKeyFunc_AutoSwitch,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x60,   emKeyFunc_SetInputToFullByWinA,     FALSE,       0,        emKeyFunc_None,  TRUE,   IrKeyOnlySwitchWinKeyToFullFlag, emKeyFunc_WinA, KeyFlag_NoUse},
    {CustomCode,    0xE0,   emKeyFunc_SetInputToFullByWinB,     FALSE,       0,        emKeyFunc_None,  TRUE,   IrKeyOnlySwitchWinKeyToFullFlag, emKeyFunc_WinB, KeyFlag_NoUse},
    {CustomCode,    0xA2,   emKeyFunc_SetInputToFullByWinC,     FALSE,       0,        emKeyFunc_None,  TRUE,   IrKeyOnlySwitchWinKeyToFullFlag, emKeyFunc_WinC, KeyFlag_NoUse},
    {CustomCode,    0x58,   emKeyFunc_SetInputToFullByWinD,     FALSE,       0,        emKeyFunc_None,  TRUE,   IrKeyOnlySwitchWinKeyToFullFlag, emKeyFunc_WinD, KeyFlag_NoUse},
    {CustomCode,    0x32,                   emKeyFunc_Swap,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x78,                 emKeyFunc_Resize,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xF8,              emKeyFunc_AudioMute,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xBA,             emKeyFunc_Resolution,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xF0,                emKeyFunc_MVMode1,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xB2,                emKeyFunc_MVMode4,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x48,                emKeyFunc_MVMode2,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xC8,                emKeyFunc_MVMode3,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x8A,            emKeyFunc_MVModeUser1,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x68,            emKeyFunc_MVModeUser2,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0xE8,            emKeyFunc_MVModeUser3,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
    {CustomCode,    0x0A,            emKeyFunc_MVModeUser4,     FALSE,       0,        emKeyFunc_None,  FALSE,   unUseMultiIrKeyFunc, emKeyFunc_None, KeyFlag_NoUse},
};

void mapp_IrKeyValueScan(u16 u16CustomCode,u8 u8IrCmd,bool bIsRepeat)
{
    u8 i;

    //dbg_printf("Ir Get:CusCode = 0x%04x, u8IrCmd = 0x%02x, bIsRepeat = %d \r\n",u16CustomCode,u8IrCmd,bIsRepeat);
    for(i=1;i<sizeof(g_stIrKeyScanTable)/sizeof(g_stIrKeyScanTable[0]);i++)
    {
        if(u16CustomCode != g_stIrKeyScanTable[i].m_u16CustomerCode)
            break;
        if(u8IrCmd == g_stIrKeyScanTable[i].m_u8CmdCode)
        {
            if(bIsRepeat)
            {
                // add repeat function
            }
            else
            {
                g_emSourceKeyType = eKeyValSource_IrRemote;
                if(g_stIrKeyScanTable[i].m_bEnableMultiFunc && (g_stIrKeyScanTable[i].MFCondition()==TRUE))
                {
                    pITE663xx_STM32_api->SysKeyIn(g_stIrKeyScanTable[i].m_u8IrKeyMultiFuncID);
                }
                else
                {
                    pITE663xx_STM32_api->SysKeyIn(g_stIrKeyScanTable[i].m_u8IrKeyNomalFuncID);
                }
            }
        }
    }

}
#endif

#else

#endif

//the response function for 'pITE663xx_STM32_api->SysKeyIn(x)'
void Project_STM32_SysFuncSel(iTE_u8 u8FuncKey)
{
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstHdmiSwitchParam_t		pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
    if(!pstBasicVar->bPowerOnSta)
    {
        if(u8FuncKey != emKeyFunc_None)
        {
            mapp_SetPowerState(TRUE,TRUE);
            KeyPressCallBack(emKeyFunc_PrintPowerState);
        }
        return;
    }
	if(isFrontPanelLocked(&s_tValStd,u8FuncKey))
	{
        KeyPressCallBack(u8FuncKey);
        return;
	}
    #endif

	switch(u8FuncKey)
	{
		case emKeyFunc_Input1:
        case emKeyFunc_Input2:
        case emKeyFunc_Input3:
        case emKeyFunc_Input4:
            {
                SCU41_MV_SaveInputPortKeyPress(u8FuncKey-emKeyFunc_Input1);
            }
			break;
        case emKeyFunc_WinA:
        case emKeyFunc_WinB:
        case emKeyFunc_WinC:
        case emKeyFunc_WinD:
            {
                if(pstBasicVar->u8WindowNum == emWinNum_3)
                {
                    if(u8FuncKey >= emKeyFunc_WinD)
                        break;
                }
                else if(pstBasicVar->u8WindowNum == emWinNum_2)
                {
                    if(u8FuncKey >= emKeyFunc_WinC)
                        break;
                }
                else if(pstBasicVar->u8WindowNum == emWinNum_1)
                {
                    break;
                }
                SCU41_MV_SelectPortToWin(u8FuncKey-emKeyFunc_WinA);
            }
			break;

        case emKeyFunc_GetInputByWinA:
        case emKeyFunc_GetInputByWinB:
        case emKeyFunc_GetInputByWinC:
        case emKeyFunc_GetInputByWinD:
            {
                if(pstBasicVar->u8WindowNum == emWinNum_3)
                {
                    if(u8FuncKey >= emKeyFunc_GetInputByWinD)
                        break;
                }
                else if(pstBasicVar->u8WindowNum == emWinNum_2)
                {
                    if(u8FuncKey >= emKeyFunc_GetInputByWinC)
                        break;
                }
                else if(pstBasicVar->u8WindowNum == emWinNum_1)
                {
                    break;
                }
                led_off(led_name_hdmi_input_yellow_1+(pstBasicVar->u8WinSel[u8FuncKey-emKeyFunc_GetInputByWinA])*2);
                led_off(led_name_window_A+((u8FuncKey-emKeyFunc_GetInputByWinA)+1)%emWinTypeMax);
                led_off(led_name_window_A+((u8FuncKey-emKeyFunc_GetInputByWinA)+2)%emWinTypeMax);
                led_off(led_name_window_A+((u8FuncKey-emKeyFunc_GetInputByWinA)+3)%emWinTypeMax);
                #if _ENABLE_SWITCH_LED_TIMER
                mapp_SCU41MVSetCheckPortFlag(TRUE);
                mapp_SCU41MVSetCheckWinToSrcPort(pstBasicVar->u8WinSel[u8FuncKey-emKeyFunc_GetInputByWinA]);
                PlatformTimerSet(etUser_SwitchLedTimer,3000);
                led_on(led_name_hdmi_input_blue_1+(pstBasicVar->u8WinSel[u8FuncKey-emKeyFunc_GetInputByWinA])*2);
                #else
                #if _ENABLE_LED_WORK_MOMENT_3S
                led_moment_on_3s(led_name_hdmi_input_blue_1+(pstBasicVar->u8WinSel[u8FuncKey-emKeyFunc_GetInputByWinA])*2);
                SCU41_MV_SetWinLedMode(emWinLightMode_Moment_off_3s,u8FuncKey-emKeyFunc_GetInputByWinA);
                #endif
                #endif
                KeyPressCallBack(u8FuncKey);
            }
            break;
        case emKeyFunc_SetInputToFullByWinA:
        case emKeyFunc_SetInputToFullByWinB:
        case emKeyFunc_SetInputToFullByWinC:
        case emKeyFunc_SetInputToFullByWinD:
            {
                u8 l_u8OldWin=0;
                #if _ENABLE_MV_SOURCE_SWAP
                u8 l_u8Temp=0;
                #endif
                if(pstBasicVar->u8WindowNum == emWinNum_3)
                {
                    if(u8FuncKey >= emKeyFunc_SetInputToFullByWinD)
                        break;
                }
                else if(pstBasicVar->u8WindowNum == emWinNum_2)
                {
                    if(u8FuncKey >= emKeyFunc_SetInputToFullByWinC)
                        break;
                }
                else if(pstBasicVar->u8WindowNum == emWinNum_1)
                {
                    break;
                }
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    mapp_SetCfgMode(emCfgMode_None);
                }
                if(pstBasicVar->u8ResizeMode!=emResize_None)
                {
                    pstBasicVar->u8ResizeMode = emResize_None;
                }
                sg_u8ResizeMode = 0;
                pstBasicVar->u8MvMode = emMvMode_1Win_Full;
                l_u8OldWin = u8FuncKey-emKeyFunc_SetInputToFullByWinA+emWinType_A;
                #if _ENABLE_MV_SOURCE_SWAP
                l_u8Temp = pstBasicVar->u8WinSel[emWinType_A];
                #endif
                pstBasicVar->u8WinSel[emWinType_A] = pstBasicVar->u8WinSel[l_u8OldWin];
                #if _ENABLE_MV_SOURCE_SWAP
                pstBasicVar->u8WinSel[l_u8OldWin] = l_u8Temp;
                #endif
                SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
                SCU41_MV_SetWinLedMode(emWinLightMode_On,emWinType_A);
                #if _ENABLE_MV_SOURCE_SWAP
                //SCU41_MV_SetDisplayWin(pstBasicVar->u8WinSel[emWinType_A],emWinType_A,FALSE);
                //SCU41_MV_SetDisplayWin(pstBasicVar->u8WinSel[l_u8OldWin],l_u8OldWin,TRUE);
                SCU41_MV_SetDisplayWin(emSrcInputMax,emWinTypeMax,TRUE);
                #else
                SCU41_MV_SetDisplayWin(pstBasicVar->u8WinSel[emWinType_A],emWinType_A,TRUE);
                #endif
                //SCU41_MV_SetMultiViewMode(pstBasicVar->u8MvMode);
                if(pstBasicVar->u8WindowNum == emWinNum_1)
                {
                    #if _ENABLE_SWITCH_PORT_AND_AUDIO
                    if(pstBasicVar->bFullModeToSwitchAudio == TRUE)
                    #endif
                        mapp_SCU41MVSetAudioInput(pstBasicVar->u8WinSel[emWinType_A]);
                }
                led_on(led_name_hdmi_input_blue_1+pstBasicVar->u8WinSel[emWinType_A]*2);
                SetUserData_SpecBasicVar();
                KeyPressCallBack(u8FuncKey);
            }
            break;
        case emKeyFunc_Swap:
            {
                led_moment_on(led_name_swap);
                delay_ms(10);
                SCU41_MV_SetSwapFunction();
                KeyPressCallBack(u8FuncKey);
            }
			break;
        case emKeyFunc_Resize:
            {
                led_moment_on(led_name_resize);
                SCU41_MV_SetResizeFunction(pstBasicVar->u8MvMode);
                KeyPressCallBack(u8FuncKey);
            }
			break;
        case emKeyFunc_MVMode1:
        case emKeyFunc_MVMode2:
        case emKeyFunc_MVMode3:
        case emKeyFunc_MVMode4:
        case emKeyFunc_MVModeUser1:
        case emKeyFunc_MVModeUser2:
        case emKeyFunc_MVModeUser3:
        case emKeyFunc_MVModeUser4:
            {
                if(u8FuncKey == emKeyFunc_MVMode1)
                {
                    pstBasicVar->u8MvMode = emMvMode_2Win_PIP_RD;
                }
                else if(u8FuncKey == emKeyFunc_MVMode2)
                {
                    pstBasicVar->u8MvMode = emMvMode_4Win_PBP_1L3R;
                }
                else if(u8FuncKey == emKeyFunc_MVMode3)
                {
                    pstBasicVar->u8MvMode = emMvMode_4Win_SameSize;
                }
                else if(u8FuncKey == emKeyFunc_MVMode4)
                {
                    pstBasicVar->u8MvMode = emMvMode_2Win_PBP;
                }
                else if(u8FuncKey == emKeyFunc_MVModeUser1)
                {
                    pstBasicVar->u8MvMode = emMvMode_xWin_ByUser1;
                }
                else if(u8FuncKey == emKeyFunc_MVModeUser2)
                {
                    pstBasicVar->u8MvMode = emMvMode_xWin_ByUser2;
                }
                else if(u8FuncKey == emKeyFunc_MVModeUser3)
                {
                    pstBasicVar->u8MvMode = emMvMode_xWin_ByUser3;
                }
                else if(u8FuncKey == emKeyFunc_MVModeUser4)
                {
                    pstBasicVar->u8MvMode = emMvMode_xWin_ByUser4;
                }
                SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
                SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
                SCU41_MV_SetMultiViewMode(pstBasicVar->u8MvMode);
                SetUserData_SpecBasicVar();
                KeyPressCallBack(u8FuncKey);
            }
			break;
		case emKeyFunc_AutoSwitch:
            #if _ENABLE_SWITCH_PORT_AFTER_A_FEW_TIME_LATER
            if(SCU41_MV_GetSelectPortFlag() == TRUE)
                break;
            #endif
			if(pstHdmiSw->bAutoSwitch)
			{
				//save switch mode
                pstHdmiSw->bAutoSwitch = FALSE;
                SetUserData_HdmiSel();
				//uart write back
				KeyPressCallBack(emKeyFunc_AutoSwitch);
                led_off(led_name_switch_auto);
			}
			else
			{
                g_bAutoSwPowerOnStatus = TRUE;
                pstHdmiSw->bAutoSwitch= TRUE;
                SetUserData_HdmiSel();
				//uart write back
				KeyPressCallBack(emKeyFunc_AutoSwitch);
                led_on(led_name_switch_auto);
			}
			break;

        case emKeyFunc_SwitchHdmi1:
        case emKeyFunc_SwitchHdmi2:
        case emKeyFunc_SwitchHdmi3:
        case emKeyFunc_SwitchHdmi4:
            {
                #if _ENABLE_MV_SOURCE_SWAP
                u8 l_u8Temp,l_u8OldWin;
                l_u8OldWin = mapp_SCU41MVGetTheWinOfCurSrc(SCU41_MV_GetInputPortSelect());
                #endif
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    switch(mapp_GetCfgMode())
                    {
                        case emCfgMode_Pixel:
                            {
                                #if _ENABLE_MV_SOURCE_SWAP
                                l_u8Temp = g_stUserWinPixelCfg.u8UserSrc[SCU41_MV_GetWinPortSelect()];
                                g_stUserWinPixelCfg.u8UserSrc[l_u8OldWin] = l_u8Temp;
                                #endif
                                g_stUserWinPixelCfg.u8UserSrc[SCU41_MV_GetWinPortSelect()] = SCU41_MV_GetInputPortSelect();
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinPixelCfg.u8UserSrc);
                            }
                            break;
                        default:
                            {
                                #if _ENABLE_MV_SOURCE_SWAP
                                l_u8Temp = g_stUserWinsConfig.u8UserSrc[SCU41_MV_GetWinPortSelect()];
                                g_stUserWinsConfig.u8UserSrc[l_u8OldWin] = l_u8Temp;
                                #endif
                                g_stUserWinsConfig.u8UserSrc[SCU41_MV_GetWinPortSelect()] = SCU41_MV_GetInputPortSelect();
                                mapp_SCU41MVCopyUserModeSrcToNomalMode(g_stUserWinsConfig.u8UserSrc);
                            }
                            break;
                    }
                }
                else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                {
                    u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                    pstUserWindowsConfig_t pstUserWinCfg = &pstBasicVar->stUserWinsConfig[l_u8UserMode];

                    #if _ENABLE_MV_SOURCE_SWAP
                    l_u8Temp = pstUserWinCfg->u8UserSrc[SCU41_MV_GetWinPortSelect()];
                    pstUserWinCfg->u8UserSrc[l_u8OldWin] = l_u8Temp;
                    #endif
                    pstUserWinCfg->u8UserSrc[SCU41_MV_GetWinPortSelect()] = SCU41_MV_GetInputPortSelect();
                    mapp_SCU41MVCopyUserModeSrcToNomalMode(pstUserWinCfg->u8UserSrc);
                }
                else
                {
                    #if _ENABLE_MV_SOURCE_SWAP
                    l_u8Temp = pstBasicVar->u8WinSel[SCU41_MV_GetWinPortSelect()];
                    pstBasicVar->u8WinSel[l_u8OldWin] = l_u8Temp;
                    #endif
                    pstBasicVar->u8WinSel[SCU41_MV_GetWinPortSelect()] = SCU41_MV_GetInputPortSelect();
                }
                #if _ENABLE_MV_SOURCE_SWAP
                SCU41_MV_SetDisplayWin(SCU41_MV_GetInputPortSelect(),SCU41_MV_GetWinPortSelect(),FALSE);
                SCU41_MV_SetDisplayWin(pstBasicVar->u8WinSel[l_u8OldWin],l_u8OldWin,TRUE);
                #else
                SCU41_MV_SetDisplayWin(SCU41_MV_GetInputPortSelect(),SCU41_MV_GetWinPortSelect(),TRUE);
                #endif
                if(pstBasicVar->u8WindowNum== emWinNum_1)
                {
                    #if _ENABLE_SWITCH_PORT_AND_AUDIO
                    if(pstBasicVar->bFullModeToSwitchAudio == TRUE)
                    #endif
                    {
                        mapp_SCU41MVSetAudioInput(SCU41_MV_GetInputPortSelect());
                    }
                }
                SetUserData_SpecBasicVar();
                KeyPressCallBack(u8FuncKey);
            }
			break;
        case emKeyFunc_AudioMute:
            {
                if(pstBasicVar->bAudioMute)
                {
                    //pstBasicVar->bAudioMute = FALSE;
                    mapp_SCU41MVSetAudioMute(FALSE);
                }
                else
                {
                    //pstBasicVar->bAudioMute = TRUE;
                    mapp_SCU41MVSetAudioMute(TRUE);
                }
				KeyPressCallBack(emKeyFunc_AudioMute);
            }
			break;
        case emKeyFunc_Resolution:
            {
                pstBasicVar->u8ResolutionID = (pstBasicVar->u8ResolutionID+1)%(max_Resolution);
				KeyPressCallBack(emKeyFunc_Resolution);
                mapp_SCU41MVResolutionSwitch(pstBasicVar->u8ResolutionID);
                SetUserData_SpecBasicVar();
            }
			break;

        case emKeyFunc_AudioSrcHdmi1:
        case emKeyFunc_AudioSrcHdmi2:
        case emKeyFunc_AudioSrcHdmi3:
        case emKeyFunc_AudioSrcHdmi4:
        //case emKeyFunc_AudioSrcExtern:
            {
                pstProjectGlobalRunVar_t    pstPrjRunVar = &pstPrjAllVar->stPrjRunVar;
                u8 i;

                if(pstBasicVar->u8WindowNum== emWinNum_1)
                {
                    #if _ENABLE_SWITCH_PORT_AND_AUDIO
                    if(pstBasicVar->bFullModeToSwitchAudio == TRUE)
                    #endif
                        return;
                }
                for(i=0; i<_HDMI_INPUT_PORT_USED_NUMBER; i++)
                {
                    if((pstPrjRunVar->u16RxSignalSta & (1<<i))&&((u8FuncKey-emKeyFunc_AudioSrcHdmi1)!=i))
                    {
                        led_on(led_name_hdmi_input_yellow_1+i*2);
                    }
                    else
                    {
                        led_off(led_name_hdmi_input_yellow_1+i*2);
                    }
                }
                mapp_SCU41MVAudioSrcSwitchEnable(TRUE);
                mapp_SCU41MVSetAudioInput(u8FuncKey-emKeyFunc_AudioSrcHdmi1);
                led_moment_on_3s(led_name_hdmi_input_blue_1+(u8FuncKey-emKeyFunc_AudioSrcHdmi1)*2);
				KeyPressCallBack(u8FuncKey);
            }
            break;
        case emKeyFunc_AudioSrcExtern:
            {
                if(pstBasicVar->u8WindowNum== emWinNum_1)
                {
                    #if _ENABLE_SWITCH_PORT_AND_AUDIO
                    if(pstBasicVar->bFullModeToSwitchAudio == TRUE)
                    #endif
                        return;
                }
                led_off(led_name_switch_auto);
                mapp_SCU41MVAudioSrcSwitchEnable(TRUE);
                mapp_SCU41MVSetAudioInput(u8FuncKey-emKeyFunc_AudioSrcHdmi1);
                led_norm_blink(led_name_switch_auto);
				KeyPressCallBack(u8FuncKey);
            }
            break;
        case emKeyFunc_ResetPort:
            {
                if(pstBasicVar->u8WindowNum != emWinNum_4)
                {
                    KeyPressCallBack(u8FuncKey);
                    break;
                }
                led_moment_on(led_name_swap);
                if(mapp_GetCfgMode() != emCfgMode_None)
                {
                    switch(mapp_GetCfgMode())
                    {
                        case emCfgMode_Pixel:
                            {
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_A] = emSrcInput_Hdmi1;
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_B] = emSrcInput_Hdmi2;
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_C] = emSrcInput_Hdmi3;
                                g_stUserWinPixelCfg.u8UserSrc[emWinType_D] = emSrcInput_Hdmi4;
                            }
                            break;
                        default:
                            {
                                g_stUserWinsConfig.u8UserSrc[emWinType_A] = emSrcInput_Hdmi1;
                                g_stUserWinsConfig.u8UserSrc[emWinType_B] = emSrcInput_Hdmi2;
                                g_stUserWinsConfig.u8UserSrc[emWinType_C] = emSrcInput_Hdmi3;
                                g_stUserWinsConfig.u8UserSrc[emWinType_D] = emSrcInput_Hdmi4;
                            }
                            break;
                    }
                }
                else if(mapp_SCU41MVIsMvUserMode(pstBasicVar->u8MvMode))
                {
                    u8 l_u8UserMode = pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1;
                    pstUserWindowsConfig_t pstUserWinCfg = &pstBasicVar->stUserWinsConfig[l_u8UserMode];

                    pstUserWinCfg->u8UserSrc[emWinType_A] = emSrcInput_Hdmi1;
                    pstUserWinCfg->u8UserSrc[emWinType_B] = emSrcInput_Hdmi2;
                    pstUserWinCfg->u8UserSrc[emWinType_C] = emSrcInput_Hdmi3;
                    pstUserWinCfg->u8UserSrc[emWinType_D] = emSrcInput_Hdmi4;
                }
                else
                {
                    pstBasicVar->u8WinSel[emWinType_A] = emSrcInput_Hdmi1;
                    pstBasicVar->u8WinSel[emWinType_B] = emSrcInput_Hdmi2;
                    pstBasicVar->u8WinSel[emWinType_C] = emSrcInput_Hdmi3;
                    pstBasicVar->u8WinSel[emWinType_D] = emSrcInput_Hdmi4;
                }
                SCU41_MV_SetDisplayWin((u8)emSrcInputMax,(u8)emWinTypeMax,TRUE);
                if(pstBasicVar->u8WindowNum== emWinNum_1)
                {
                    mapp_SCU41MVSetAudioInput(pstBasicVar->u8WinSel[emWinType_A]);
                }
                SetUserData_SpecBasicVar();
                KeyPressCallBack(u8FuncKey);
            }
            break;
        case emKeyFunc_QueryTxTiming:
            {
                u8 l_u8TxTiming[30];
                u8 l_u8Vic;

                memset(l_u8TxTiming,0,30);
                l_u8Vic = IT663XX_GetTxTiming(g_u8QueryTxPort,l_u8TxTiming);
                l_u8TxTiming[29] = l_u8Vic;
                KeyPrintTiming(FALSE,l_u8TxTiming);
            }
            break;
        case emKeyFunc_QueryRxTiming:
            {
                u8 l_u8RxTiming[30];
                u8 l_u8Vic;

                memset(l_u8RxTiming,0,30);
                l_u8Vic = IT663XX_GetRxTiming(g_u8QueryRxPort,l_u8RxTiming);
                l_u8RxTiming[29] = l_u8Vic;
                KeyPrintTiming(TRUE,l_u8RxTiming);
            }
            break;

		default:
			break;
	}
}

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
void mapp_DetectSyncToDoAction(uint8_t u8Action)
{
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    #endif
    if(u8Action == m_action_poweron)
    {
        #if _ENABLE_AUTO_POWER_FUNCTION
        if(pstBasicSpecVar->bActionToAutoPowerOnOffFlag == TRUE)
        {
            mapp_SetPowerState(TRUE,FALSE);
        }
        #endif
        if(pstBasicSpecVar->bActionToSendCecFlag == TRUE)
			cec_cmd_tv_poweron(CEC_OPERATE_OUTPUT, FALSE);
        #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(pstBasicSpecVar->bActionToSendRs232CmdFlag == TRUE)
            g_bSendUartKeyVal[uart_study_key_type_dis_on] = TRUE;

        if(pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_on].u16KeyValueLen == 0)
        #endif
        {
            if((pstBasicSpecVar->bActionToSendRs232CmdFlag == TRUE)
                ||(pstBasicSpecVar->bActionToSendCecFlag == TRUE)
                #if _ENABLE_AUTO_POWER_FUNCTION
                ||(pstBasicSpecVar->bActionToAutoPowerOnOffFlag == TRUE)
                #endif
                )
                {
                    #if _ENABLE_AUTO_POWER_FUNCTION
                    KeyPressCallBack(emKeyFunc_PrintPowerState);
                    #else
                    KeyPressCallBack(emKeyFunc_DisplayOn);
                    #endif
                }
        }
    }
    else if(u8Action == m_action_poweroff)
    {
        #if _ENABLE_AUTO_POWER_FUNCTION
        if(pstBasicSpecVar->bActionToAutoPowerOnOffFlag == TRUE)
        {
            mapp_SetPowerState(FALSE,FALSE);
        }
        #endif

        if(pstBasicSpecVar->bActionToSendCecFlag == TRUE)
            cec_cmd_tv_poweroff(CEC_OPERATE_OUTPUT, FALSE);

        #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(pstBasicSpecVar->bActionToSendRs232CmdFlag == TRUE)
            g_bSendUartKeyVal[uart_study_key_type_dis_off] = TRUE;

        if(pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off].u16KeyValueLen == 0)
        #endif
        {
            if((pstBasicSpecVar->bActionToSendRs232CmdFlag == TRUE)
                ||(pstBasicSpecVar->bActionToSendCecFlag == TRUE)
                #if _ENABLE_AUTO_POWER_FUNCTION
                ||(pstBasicSpecVar->bActionToAutoPowerOnOffFlag == TRUE)
                #endif
                )
                {
                    #if _ENABLE_AUTO_POWER_FUNCTION
                    KeyPressCallBack(emKeyFunc_PrintPowerState);
                    #else
                    KeyPressCallBack(emKeyFunc_DisplayOff);
                    #endif
                }
        }
    }
    else
    {
        // none
    }
}
#endif

