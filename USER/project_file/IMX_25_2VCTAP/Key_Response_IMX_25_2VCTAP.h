#ifndef __KEY_RESPONSE_H__
#define __KEY_RESPONSE_H__

#define KeyFlag_NoUse       0x00
#define KeyFlag_Nomal       0x01
#define KeyFlag_DIPSwitcher 0x02
#define KeyFlag_ReFunction  0x04

typedef enum _emKeyFunction
{
    emKeyFunc_None = 0,
    emKeyFunc_Input1,
    emKeyFunc_Input2,
    emKeyFunc_Input3,
    emKeyFunc_Input4,
    emKeyFunc_Input5,
    emKeyFunc_Input6,
    emKeyFunc_Input7,
    emKeyFunc_Input8,
    emKeyFunc_Input9,
    emKeyFunc_Input10,
    emKeyFunc_Input11,
    emKeyFunc_Input12,
    emKeyFunc_Input13,
    emKeyFunc_Input14,
    emKeyFunc_Input15,
    emKeyFunc_Input16,
    emKeyFunc_Output1,
    emKeyFunc_Output2,
    emKeyFunc_Output3,
    emKeyFunc_Output4,
    emKeyFunc_Output5,
    emKeyFunc_Output6,
    emKeyFunc_Output7,
    emKeyFunc_Output8,
    emKeyFunc_Output9,
    emKeyFunc_Output10,
    emKeyFunc_Output11,
    emKeyFunc_Output12,
    emKeyFunc_Output13,
    emKeyFunc_Output14,
    emKeyFunc_Output15,
    emKeyFunc_Output16,

    emKeyFunc_Call,
    emKeyFunc_Switch,
    emKeyFunc_Save,
    emKeyFunc_Mode,
    emKeyFunc_PageUp,
    emKeyFunc_PageDown,

    emKeyFunc_Max,
    emKeyFunc_repeat,
}emKeyFunction;

typedef enum _emKeyTypeDefine
{
    emKeyType_Source = 0,
    emKeyType_DisplayOn,
    emKeyType_DisplayOff,
    emKeyType_EdidDip,
    //emKeyType_AutoSwitch,

    emKeyType_Max,
}emKeyTypeDefine;

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
typedef enum _action_to_detect_sync_t_
{
    m_action_none = 0,
    m_action_poweron,
    m_action_poweroff,

    m_action_max,
}action_to_detect_sync_t;
#endif

extern eKeyValSource_t g_emSourceKeyType;

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
void mapp_DetectSyncToDoAction(uint8_t u8Action);
#endif

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
void mapp_SetKeyFunction(u8 u8KeyFunc);
u8 mapp_GetKeyFunctionNum(void);
void mapp_KeyFunctionProcess(void);
void mapp_NewKeyScan(void);
void Project_KeyFuncEvent(iTE_u8 u8FuncKey);
#endif

#endif
