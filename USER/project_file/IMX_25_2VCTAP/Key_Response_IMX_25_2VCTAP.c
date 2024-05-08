#include "includes.h"

extern stProjectGlobalAllVar_t g_stPrjGloVar;
#define Enable_LockPanelByAllKey        0
#define ENABLE_AUTO_MODE_TO_SWITCH_CHANNEL  1

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _emKeyValueType
{
    emKeyvalueInput1       = BIT0,
    emKeyvalueInput2       = BIT3,
    emKeyvalueInput3       = BIT6,
    emKeyvalueInput4       = BIT9,
    emKeyvalueInput5       = BIT12,
    emKeyvalueInput6       = BIT15,
    emKeyvalueInput7       = BIT18,
    emKeyvalueInput8       = BIT2,
    emKeyvalueInput9       = BIT8,
    emKeyvalueInput10      = BIT14,
    emKeyvalueInput11      = BIT1,
    emKeyvalueInput12      = BIT4,
    emKeyvalueInput13      = BIT7,
    emKeyvalueInput14      = BIT10,
    emKeyvalueInput15      = BIT13,
    emKeyvalueInput16      = BIT16,
    emKeyvalueCall         = BIT5,
    emKeyvalueSave         = BIT11,
    emKeyvaluePageUp       = BIT17,

    emKeyvalueOutput1      = BIT0,
    emKeyvalueOutput2      = BIT3,
    emKeyvalueOutput3      = BIT6,
    emKeyvalueOutput4      = BIT9,
    emKeyvalueOutput5      = BIT12,
    emKeyvalueOutput6      = BIT15,
    emKeyvalueOutput7      = BIT18,
    emKeyvalueOutput8      = BIT2,
    emKeyvalueOutput9      = BIT8,
    emKeyvalueOutput10     = BIT14,
    emKeyvalueOutput11     = BIT1,
    emKeyvalueOutput12     = BIT4,
    emKeyvalueOutput13     = BIT7,
    emKeyvalueOutput14     = BIT10,
    emKeyvalueOutput15     = BIT13,
    emKeyvalueOutput16     = BIT16,
    emKeyvalueSwitch       = BIT5,
    emKeyvalueMode         = BIT11,
    emKeyvaluePageDown     = BIT17,
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
	pstProjectSpecVar_t		pstSpecVar = &stIMX_25_2VCTAPSpecVar;
	pstBasicSpecVar_t		pstBasicVar = &pstSpecVar->stBasSpecVar;

    #if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
    if((pstBasicVar->bIsLock))
    {
        switch(u8KeyIndex)
        {
            default:
                return FALSE;
                //break;
        }
    }
    #else
	if((pstBasicVar->bIsLock)&&\
		(((ptKeyVal->StStoreVal[u8KeyIndex].eValSource==eKeyValSource_Panel)
		//&&(u8KeyIndex != 3)
		)
		||(ptKeyVal->StStoreVal[u8KeyIndex].eValSource==eKeyValSource_IrRemote)))
	{
		return TRUE;
	}
    #endif

	return FALSE;
}

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
const static stKeyScanType stKeyScanGroup[]=
{
    /*Group     Value               KeyNomalType             ComboEnable  ComboTime     KeyComboType   MultiFuncEnable MFConditionFlag   KeyMultiFuncType   m_Flag*/
    {  0,           0,              emKeyFunc_None,        FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_NoUse},
    {  0,   emKeyvalueInput1,      emKeyFunc_Input1,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput2,      emKeyFunc_Input2,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput3,      emKeyFunc_Input3,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput4,      emKeyFunc_Input4,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput5,      emKeyFunc_Input5,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput6,      emKeyFunc_Input6,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput7,      emKeyFunc_Input7,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput8,      emKeyFunc_Input8,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput9,      emKeyFunc_Input9,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput10,    emKeyFunc_Input10,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput11,    emKeyFunc_Input11,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput12,    emKeyFunc_Input12,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput13,    emKeyFunc_Input13,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput14,    emKeyFunc_Input14,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput15,    emKeyFunc_Input15,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,   emKeyvalueInput16,    emKeyFunc_Input16,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,      emKeyvalueCall,       emKeyFunc_Call,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,      emKeyvalueSave,       emKeyFunc_Save,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  0,    emKeyvaluePageUp,     emKeyFunc_PageUp,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput1,    emKeyFunc_Output1,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput2,    emKeyFunc_Output2,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput3,    emKeyFunc_Output3,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput4,    emKeyFunc_Output4,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput5,    emKeyFunc_Output5,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput6,    emKeyFunc_Output6,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput7,    emKeyFunc_Output7,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput8,    emKeyFunc_Output8,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,   emKeyvalueOutput9,    emKeyFunc_Output9,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput10,   emKeyFunc_Output10,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput11,   emKeyFunc_Output11,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput12,   emKeyFunc_Output12,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput13,   emKeyFunc_Output13,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput14,   emKeyFunc_Output14,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput15,   emKeyFunc_Output15,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvalueOutput16,   emKeyFunc_Output16,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,    emKeyvalueSwitch,     emKeyFunc_Switch,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,      emKeyvalueMode,       emKeyFunc_Mode,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
    {  1,  emKeyvaluePageDown,   emKeyFunc_PageDown,       FALSE,           0,         emKeyFunc_None,    FALSE,   unUseMultiKeyFunc,   emKeyFunc_None,  KeyFlag_Nomal},
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
        mapp_SetKeyFunction(RealKeyValue);
        g_emSourceKeyType = eKeyValSource_Panel;
        RealKeyValue=emKeyFunc_None;
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
        pstGpioConfig_t l_psKeyPort = (pstGpioConfig_t)l_pstKey->pstNewKeyPort;
        if(l_pstKey->pstNewKeyPort != NULL)
        {
            if((l_psKeyPort->u8PortType>=PORT_GPIOA)&&(l_psKeyPort->u8PortType<=PORT_GPIOG))
            {
                //if(l_pstKey->bIsDipKey == TRUE)
                {
                    #if (defined(GD32F4XX))
                    l_u16KeyValue = (u16)gpio_input_bit_get(STM32_GPIO_PORT[l_pstKey->pstNewKeyPort->u8PortType-PORT_GPIOA], l_pstKey->pstNewKeyPort->u16GpioPin);
                    #else
                    l_u16KeyValue = GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_pstKey->pstNewKeyPort->u8PortType-PORT_GPIOA], l_pstKey->pstNewKeyPort->u16GpioPin);
                    #endif
                    if(l_pstKey->eNewKeyOnLevel == ePinLevel_High)
                        l_u32KeypadValue[l_pstKey->u8KeyGroup] = (l_u16KeyValue == 1) ? (l_u32KeypadValue[l_pstKey->u8KeyGroup]|l_pstKey->u32KeyValue) : l_u32KeypadValue[l_pstKey->u8KeyGroup];
                    else
                        l_u32KeypadValue[l_pstKey->u8KeyGroup] = (l_u16KeyValue == 0) ? (l_u32KeypadValue[l_pstKey->u8KeyGroup]|l_pstKey->u32KeyValue) : l_u32KeypadValue[l_pstKey->u8KeyGroup];
                }
            }
            #if _EN_CHIP_TM1623_USE
            else if(IS_TM1623_KVAL(l_psKeyPort->u8PortType))
            {
                if((TM1623_ksg_reg_val[TM1623_DEV_INDEX(l_psKeyPort->u8PortType)][TM1623_PORT_NUM(l_psKeyPort->u8PortType)] & l_psKeyPort->u16GpioPin)!=0)
                {
//                    RS232_printf("TM1623_ksg_reg_val[%d] = 0x%04x\n",TM1623_DEV_INDEX(l_psKeyPort->u8PortType),TM1623_ksg_reg_val[TM1623_DEV_INDEX(l_psKeyPort->u8PortType)][TM1623_PORT_NUM(l_psKeyPort->u8PortType)] & l_psKeyPort->u16GpioPin);
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
            RS232_printf("sl_u16KeyValue[%d] = 0x%04x\n",i,sl_u32PreKeyValue[i]);
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
    Project_KeyFuncEvent(mapp_GetKeyFunctionNum());
    mapp_IMX_25_2VCTAPMainFunction();
}

static u8 sg_u8KeySta[emKeyFunc_Max] = {0}; // only for test
static u16 sg_u16KeyLedStay[emKeyFunc_Max] = {0}; // only for test
#define KEY_LED_STAY_CNT        100     // unit 10ms
void Project_KeyFuncEvent(iTE_u8 u8FuncKey)
{
	switch(u8FuncKey)
	{
        case emKeyFunc_Input1:
        case emKeyFunc_Input2:
        case emKeyFunc_Input3:
        case emKeyFunc_Input4:
        case emKeyFunc_Input5:
        case emKeyFunc_Input6:
        case emKeyFunc_Input7:
        case emKeyFunc_Input8:
        case emKeyFunc_Input9:
        case emKeyFunc_Input10:
        case emKeyFunc_Input11:
        case emKeyFunc_Input12:
        case emKeyFunc_Input13:
        case emKeyFunc_Input14:
        case emKeyFunc_Input15:
        case emKeyFunc_Input16:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedInput1B+(u8FuncKey-emKeyFunc_Input1)*3);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    sg_u16KeyLedStay[u8FuncKey] = 0;
                    led_on(led_name_LedInput1B+(u8FuncKey-emKeyFunc_Input1)*3);
                }
    	    }
            break;
        case emKeyFunc_Output1:
        case emKeyFunc_Output2:
        case emKeyFunc_Output3:
        case emKeyFunc_Output4:
        case emKeyFunc_Output5:
        case emKeyFunc_Output6:
        case emKeyFunc_Output7:
        case emKeyFunc_Output8:
        case emKeyFunc_Output9:
        case emKeyFunc_Output10:
        case emKeyFunc_Output11:
        case emKeyFunc_Output12:
        case emKeyFunc_Output13:
        case emKeyFunc_Output14:
        case emKeyFunc_Output15:
        case emKeyFunc_Output16:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedOutput1B+(u8FuncKey-emKeyFunc_Output1)*3);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedOutput1B+(u8FuncKey-emKeyFunc_Output1)*3);
                }
    	    }
            break;

        case emKeyFunc_Call:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedCallB);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedCallB);
                }
            }
            break;
        case emKeyFunc_Switch:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedSwitchB);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedSwitchB);
                }
            }
            break;
        case emKeyFunc_Save:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedSaveB);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedSaveB);
                }
            }
            break;
        case emKeyFunc_Mode:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedModeB);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedModeB);
                }
            }
            break;
        case emKeyFunc_PageUp:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedPageUpB);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedPageUpB);
                }
            }
            break;
        case emKeyFunc_PageDown:
            {
                Uart_printf("u8FuncKey = %d\n",u8FuncKey);
                if(sg_u8KeySta[u8FuncKey])
                {
                    sg_u8KeySta[u8FuncKey] = 0;
                    led_off(led_name_LedPageDownB);
                }
                else
                {
                    sg_u8KeySta[u8FuncKey] = 1;
                    led_on(led_name_LedPageDownB);
                }
            }
            break;

		default:
			break;
	}

    if(mapp_GetKeyFunctionNum()!=emKeyFunc_None)
    {
        mapp_SetKeyFunction(emKeyFunc_None);
    }

    {   // auto mute key led
        u8 l_u8KIndex = 0;

        for(l_u8KIndex = 0;l_u8KIndex<emKeyFunc_Max;l_u8KIndex++)
        {
            if(sg_u8KeySta[l_u8KIndex])
            {
                if(++sg_u16KeyLedStay[l_u8KIndex]>KEY_LED_STAY_CNT)
                {
                    sg_u8KeySta[l_u8KIndex] = 0;
                    sg_u16KeyLedStay[l_u8KIndex] = 0;
                    led_off(led_name_LedInput1B+(l_u8KIndex-emKeyFunc_Input1)*3);
                }
            }
        }
    }
}


