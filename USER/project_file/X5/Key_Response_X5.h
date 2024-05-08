#ifndef __KEY_RESPONSE_H__
#define __KEY_RESPONSE_H__

#define KeyFlag_NoUse       0x00
#define KeyFlag_Nomal       0x01
#define KeyFlag_DIPSwitcher 0x02
#define KeyFlag_ReFunction  0x04

typedef enum _emKeyFunction
{
    emKeyFunc_None = 0,
    emKeyFunc_TxIDHPlus,
    emKeyFunc_TxIDLPlus,
    emKeyFunc_RxIDHPlus,
    emKeyFunc_RxIDLPlus,

    emKeyFunc_Max,
    emKeyFunc_repeat,
}emKeyFunction;


#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
typedef enum _action_to_detect_sync_t_
{
    m_action_none = 0,
    m_action_poweron,
    m_action_poweroff,

    m_action_max,
}action_to_detect_sync_t;
#endif

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
void mapp_DetectSyncToDoAction(uint8_t u8Action);
#endif

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
void mapp_SetKeyFunction(u8 u8KeyFunc);
u8 mapp_GetKeyFunctionNum(void);
void mapp_KeyFunctionProcess(void);
void mapp_NewKeyScan(void);
void Project_STM32_SysFuncSel(iTE_u8 u8FuncKey);
#endif

#endif
