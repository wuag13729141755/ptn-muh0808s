#ifndef __KEY_RESPONSE_H__
#define __KEY_RESPONSE_H__

#define KeyFlag_NoUse       0x00
#define KeyFlag_Nomal       0x01
#define KeyFlag_DIPSwitcher 0x02
#define KeyFlag_ReFunction  0x04

#define LINE1           1// 0x80
#define LINE2           3// 0xc0
#define LINE3           5// 0x80
#define LINE4           7// 0xc0

#define IN_MAX          10
#define OUT_MAX         20
#define ALL             21
#define LOCK            22
#define EDID            22
//#define CLEAR           23
#define ENTER           25
#define SCENE           26
#define CLEAR           27
#define CHCEK_OUT_STA   31
#define CHCEK_OUT_MAX   39
#define MODE            40

#define DEF_LCD_DISPLAY_TIME        3000
#define DEF_LED_DISPLAY_TIME        3000

typedef enum _emKeyFunction
{
    emKeyFunc_None = 0,
    emKeyFunc_In1,
    emKeyFunc_In2,
    emKeyFunc_In3,
    emKeyFunc_In4,
    emKeyFunc_In5,
    emKeyFunc_In6,
    emKeyFunc_In7,
    emKeyFunc_In8,
//    emKeyFunc_In9,
//    emKeyFunc_In0,
//    emKeyFunc_In10Plus,

    emKeyFunc_Out1,
    emKeyFunc_Out2,
    emKeyFunc_Out3,
    emKeyFunc_Out4,
    emKeyFunc_Out5,
    emKeyFunc_Out6,
    emKeyFunc_Out7,
    emKeyFunc_Out8,
//    emKeyFunc_Out9,
//    emKeyFunc_Out0,
//    emKeyFunc_Out10Plus,

    emKeyFunc_All,
    emKeyFunc_Enter,
    emKeyFunc_Scene,
    emKeyFunc_Clear,
    emKeyFunc_Mode,
    emKeyFunc_Lock,

    emKeyFunc_CheckOutCh1,
    emKeyFunc_CheckOutCh2,
    emKeyFunc_CheckOutCh3,
    emKeyFunc_CheckOutCh4,
    emKeyFunc_CheckOutCh5,
    emKeyFunc_CheckOutCh6,
    emKeyFunc_CheckOutCh7,
    emKeyFunc_CheckOutCh8,
//    emKeyFunc_CheckOutCh9,
//    emKeyFunc_CheckOutCh10,

    emKeyFunc_Max,
    emKeyFunc_repeat,
}emKeyFunction;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef enum _key_counter_index_name_
{
    emKeyName_In1 = 0,
    emKeyName_In2,
    emKeyName_In3,
    emKeyName_In4,
    emKeyName_In5,
    emKeyName_In6,
    emKeyName_In7,
    emKeyName_In8,

    emKeyName_Out1,
    emKeyName_Out2,
    emKeyName_Out3,
    emKeyName_Out4,
    emKeyName_Out5,
    emKeyName_Out6,
    emKeyName_Out7,
    emKeyName_Out8,

    emKeyName_All,
    emKeyName_Lock,
    emKeyName_Mode,
    emKeyName_Enter,
    emKeyName_Scene,
    emKeyName_Clear,

    emKeyName_max,
}key_counter_index_name_t;
#endif

enum StatusValue
{
    Empt = 0,
    InputStatus,
    AvStatus,
    OutputStatus,
    CloseChannelStatus,
    UndoStatus,
    FirstOutputStatsu,
    FirstEnterStatus,
    AllStatsu,
    EnterFirstOutput,
    EnterCheckChannle,   // 10
    EnterSwitchStatus,
    EnterAllSwitchStatus,
    EnterEdidStatus,
    EnterEmptStatus,
    EnterCloseChannle,
    ErrorUseStatus,  //16
    FirstAllKeyState,
    EdidStartStatus,
    EdidCpyStatus,
    EdidEndStatus,  //20
    ClearStatus,
    AllInKeyStatus,
    AllOutKeyStatus,
    FirstOutputSta,
    LockStatus,
    SceneStatus,
    SceneIdxInputStatus,
    EnterSceneCallStatus,
    SceneIdxOutputStatus,
    EnterSceneSaveStatus,
    EnterCheckChannle2,
    ModeStatus,
    ModeIdxInputStatus,
    EnterModeSaveStatus,
    EnterScenePollingStatus,

    LastList,
};

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
typedef enum _action_to_detect_sync_t_
{
    m_action_none = 0,
    m_action_poweron,
    m_action_poweroff,

    m_action_max,
}action_to_detect_sync_t;
#endif

struct KeyBuff{
    uint8_t InputKey[32];
    uint8_t InputCnt;
    uint8_t OutputKey[32];
    uint8_t OutputCnt;
    uint8_t ALLKey;
    uint8_t EDIDKey;
    uint8_t LockKey;
    uint8_t CLEARKey;
    uint8_t ENTERKey;
};

struct UITimeOut{
    uint16_t LEDStar;
    uint16_t LCDStar;
    uint8_t  UIStar;
    uint16_t Flashing;
};

typedef struct
{
	uint8_t Number;
	void (*FunOperate)();
}keyStaProcessLib;

extern const char  LcdNone[16];

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
void mapp_SetKeyFunction(u8 u8KeyFunc);
u8 mapp_GetKeyFunctionNum(void);
void mapp_KeyFunctionProcess(void);
void mapp_NewKeyScan(void);
void mapp_KeyFunctionEvent(u8 u8FuncKey);
#endif
void LCD_RecoverToDefault(uint16_t u16Timeout);
void LED_RecoverToDefault(uint16_t u16Timeout);
void led_handle(void);
void LCD_LineRefresh(char *string, char address);
void mapp_SetLedState(u8 u8LedNum, bool bSta);
void mapp_LCDBacklightEnable(void);

#endif
