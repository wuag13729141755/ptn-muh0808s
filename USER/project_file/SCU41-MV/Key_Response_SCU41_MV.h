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
    emKeyFunc_AutoSwitch,
    emKeyFunc_WinA,
    emKeyFunc_WinB,
    emKeyFunc_WinC,
    emKeyFunc_WinD,
    emKeyFunc_Swap,                 // 10
    emKeyFunc_Resize,
    emKeyFunc_MVMode1,
    emKeyFunc_MVMode2,
    emKeyFunc_MVMode3,
    emKeyFunc_MVMode4,
    emKeyFunc_SwitchHdmi1,
    emKeyFunc_SwitchHdmi2,
    emKeyFunc_SwitchHdmi3,
    emKeyFunc_SwitchHdmi4,
    emKeyFunc_NoMatchChipID,
    emKeyFunc_GetInputByWinA,       // 20
    emKeyFunc_GetInputByWinB,
    emKeyFunc_GetInputByWinC,
    emKeyFunc_GetInputByWinD,
    emKeyFunc_SetInputToFullByWinA,
    emKeyFunc_SetInputToFullByWinB,
    emKeyFunc_SetInputToFullByWinC,
    emKeyFunc_SetInputToFullByWinD,
    emKeyFunc_GetOutputSate,        // only use to print message
    emKeyFunc_AudioMute,
    emKeyFunc_Resolution,           // 30
    emKeyFunc_MVModeUser1,
    emKeyFunc_MVModeUser2,
    emKeyFunc_MVModeUser3,
    emKeyFunc_MVModeUser4,
    emKeyFunc_AudioSrcHdmi1,
    emKeyFunc_AudioSrcHdmi2,
    emKeyFunc_AudioSrcHdmi3,
    emKeyFunc_AudioSrcHdmi4,
    emKeyFunc_AudioSrcExtern,
    emKeyFunc_DisplayOn,
    emKeyFunc_DisplayOff,
    emKeyFunc_ResetPort,
    emKeyFunc_UnsupportResize,
    emKeyFunc_BaudwidthIsOver,
    emKeyFunc_WinLevelIsTheSame,
    emKeyFunc_PrintPowerState,
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
void mapp_NewKeyScan(void);
#if ((_IR_FUNCTION_USE&_IR_FUNCTION_REMOTE)==_IR_FUNCTION_REMOTE)
void mapp_IrKeyValueScan(u16 u16CustomCode,u8 u8IrCmd,bool bIsRepeat);
#endif
#endif

#endif
