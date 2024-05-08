#ifndef __KEY_RESPONSE_H__
#define __KEY_RESPONSE_H__

#define KeyFlag_NoUse       0x00
#define KeyFlag_Nomal       0x01
#define KeyFlag_DIPSwitcher 0x02
#define KeyFlag_ReFunction  0x04

typedef enum _emKeyFunction
{
    emKeyFunc_None = 0,
    emKeyFunc_Upscale,
    emKeyFunc_Downscale,
    emKeyFunc_TestPatten,
    emKeyFunc_Edid0,
    emKeyFunc_Edid1,
    emKeyFunc_Edid2,
    emKeyFunc_Edid3,
    emKeyFunc_Edid4,
    emKeyFunc_Edid5,
    emKeyFunc_Edid6,
    emKeyFunc_Edid7,
    emKeyFunc_Edid8,
    emKeyFunc_Edid9,
    emKeyFunc_Edid10,
    emKeyFunc_Edid11,
    emKeyFunc_Edid12,
    emKeyFunc_Edid13,
    emKeyFunc_Edid14,
    emKeyFunc_Edid15,
    emKeyFunc_QueryTxTiming,
    emKeyFunc_QueryRxTiming,

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
void Project_STM32_SysFuncSel(iTE_u8 u8FuncKey);
#endif

#endif
