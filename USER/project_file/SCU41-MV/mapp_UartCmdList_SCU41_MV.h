#ifndef __CMD_LIST_H__
#define __CMD_LIST_H__

typedef enum
{
    eFunc_NoUse = 0,
    eFunc_GetVersion,
    eFunc_FactoryReset,
    eFunc_Reboot,
    eFunc_SwitchInputPort,
    eFunc_GetInputStutus,
    eFunc_Edid_Switch,
    eFunc_Edid_GetMode,
    eFunc_Edid_SendCustomData,
    eFunc_SwitchHDCP,
    eFunc_GetHDCPState,
    eFunc_SwitchPowerState,
    eFunc_GetPowerState,
    eFunc_SetRs232Baudrate,
    eFunc_GetRs232Baudrate,
    eFunc_SetAutoSwitchMode,
    eFunc_GetAutoSwitchMode,
    eFunc_SetCECDisplay,
    eFunc_HelpFunction,
    eFunc_GetTestVersion,
    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    eFunc_SetOffMsgCounter,
    eFunc_GetOffMsgCounter,
    eFunc_SetOffMsgCntDelayTime,
    eFunc_GetOffMsgCntDelayTime,
    eFunc_SetDisplayOnAsciiCmd,
    eFunc_SetDisplayOffAsciiCmd,
    eFunc_SetDisplayOnHexCmd,
    eFunc_SetDisplayOffHexCmd,
    #endif
    #if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
    eFunc_SetAutoSyncDetectActionByCec,
    eFunc_GetAutoSyncDetectActionByCec,
    eFunc_SetAutoSyncDetectActionByRs232,
    eFunc_GetAutoSyncDetectActionByRs232,
    eFunc_SetAutoSyncDisplayOffDelayTime,
    eFunc_GetAutoSyncDisplayOffDelayTime,
    #if _ENABLE_AUTO_POWER_FUNCTION
    eFunc_SetAutoSyncToPowerOnOff,
    eFunc_GetAutoSyncToPowerOnOff,
    #endif
    #endif
    #if _ENABLE_KEYPAD_LOCKED_FUNCTION
    eFunc_SetKeyPadLocked,
    eFunc_GetKeyPadLocked,
    #endif

    #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
    eFunc_SetInputSignalDetectMode,
    eFunc_GetInputSignalDetectMode,
    #endif

    #if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
    eFunc_Edid_LoadInternalEdidData,
    #endif

    #if 0//_ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
    eFunc_LoadIteChipRegisterData,
    eFunc_WriteChipRegisterData,
    eFunc_ReadChipRegisterData,
    #endif
    #if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
    eFunc_SendUserCecCmd,
    #endif
    eFunc_SetResolution,
    eFunc_GetResolution,
    eFunc_SetMultiviewMode,
    eFunc_GetMultiviewMode,
    eFunc_SetAudioSource,
    eFunc_GetAudioSource,
    eFunc_SetAudioMix,
    eFunc_GetAudioMix,
    eFunc_SetAudioMute,
    eFunc_GetAudioMute,

    eFunc_SetCecSrcMenu,
    eFunc_SetCecSrcUp,
    eFunc_SetCecSrcDown,
    eFunc_SetCecSrcLeft,
    eFunc_SetCecSrcRight,
    eFunc_SetCecSrcBack,
    eFunc_SetCecSrcEnter,
    eFunc_SetCecSrcOn,
    eFunc_SetCecSrcOff,
    eFunc_SetCecSrcStop,
    eFunc_SetCecSrcPlay,
    eFunc_SetCecSrcPause,
    eFunc_SetCecSrcPrev,
    eFunc_SetCecSrcNext,
    eFunc_SetCecSrcRewind,
    eFunc_SetCecSrcFastForward,
    eFunc_SetCecDisOn,
    eFunc_SetCecDisOff,
    eFunc_SetCecDisSource,
    eFunc_SetCecDisMute,
    eFunc_SetCecDisVolPlus,
    eFunc_SetCecDisVolMinus,

    eFunc_EnableMvUserConfig,
    eFunc_ConfirmMvUserSetting,
    eFunc_SetSizeAndPositionWinA,
    eFunc_SetSizeAndPositionWinB,
    eFunc_SetSizeAndPositionWinC,
    eFunc_SetSizeAndPositionWinD,
    eFunc_CfgSizeAndPositionWinA,
    eFunc_CfgSizeAndPositionWinB,
    eFunc_CfgSizeAndPositionWinC,
    eFunc_CfgSizeAndPositionWinD,
    eFunc_PixelCfgSizeAndPositionWinA,
    eFunc_PixelCfgSizeAndPositionWinB,
    eFunc_PixelCfgSizeAndPositionWinC,
    eFunc_PixelCfgSizeAndPositionWinD,
    eFunc_SaveScreenToUserMode,
    eFunc_CleanScreen,
    eFunc_GetIpAddress,
    eFunc_SetIpAddress,
    eFunc_GiveIpAddress,
    eFunc_SetSwapSource,
    eFunc_SetResizeWin,
    eFunc_GetAllSystemSta,
    eFunc_SetWinLayerLevel,
    eFunc_GetWinLayerLevel,
    eFunc_SetProductName,
    eFunc_GetProductName,
    eFunc_ResetProductName,
    #if _ENABLE_SWITCH_PORT_AND_AUDIO
    eFunc_SetFullModeToSwitchAudio,
    eFunc_GetFullModeToSwitchAudio,
    #endif
    eFunc_GetInputResolution,
    eFunc_GetTxTiming,
    eFunc_GetRxTiming,
    eFunc_AppLink,
    eFunc_GetDisplayEdidPhysicalAddress,
    eFunc_GetDisplayEdid,
    eFunc_SavePixelToUserMode,

    eFunc_Max,
}eCmdFun_t;

typedef enum _emCommandFunctionStatusType_
{
    emCmdError = 0,
    emCmdSucess,
    emCmdOutOfRange,

    emCmdStatusMax,
}emCmdFuncStaType;

typedef struct _stUartCmdList2
{
    u8      flag;           // 标志
    char*   ucmd;           // 指令或者指令开头部分
    u8      cmdlen;          // 指令长度
    u8      valuelen;       // 指令附带参数长度,全词匹配则为0
    u8     cmdoffset;       // 指令偏移量,一般都是指令结尾符的长度 ,根据不同指令格式填写
    eCmdFun_t    eCmdFun;   // 指令功能
    char* feedbackstring;
}stUartCmdList2;

// command list Flag define
#define UserOffset      255
#define MaxCmdNum       255
#define UseStrlen       0
#define NoString        NULL

#define Flag_End        0
#define Flag_BackCodeNomal  0x01
#define Flag_BackCodeValue  0x02
#define Flag_BackCodeFunc   0x04
#define Flag_ShowHelpCommand    0x08
#define Flag_UserValueLength    0x10
#define Flag_NoFunction     0x20
//extern stUartCmdList2 cmd_list[];

stUartCmdList2 cmd_list[]=
{
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,       /*flag*/
        "#GET_FIRMWARE_VERSION",/*ucmd*/
        UseStrlen,              /*cmdlen*/
        0,                      // valuelen
        2,                      /*cmdoffset*/
        eFunc_GetVersion,       /*eCmdFun*/
        _STRING_FW_VER_NUM,           /*feedbackstring*/
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,       // flag
        "#FACTORY_RESET",                 //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_FactoryReset,     // eCmdFun
        "@FACTORY RESET",        // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,        // flag
        "#REBOOT",              //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_Reboot,           // eCmdFun
        "@REBOOT",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,       //flag
        "#SET_AV",              //ucmd
        7,                      // cmdlen
        7,                      // valuelen
        3,                      // cmdoffset
        eFunc_SwitchInputPort,  // eCmdFun
        "@AV",                    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "#GET_AV",              //ucmd
        7,              // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetInputStutus,   // eCmdFun
        "@AV",             // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "#SET_EDID_MODE",       //ucmd
        14,                     // cmdlen
        5,                      // valuelen
        3,                      // cmdoffset
        eFunc_Edid_Switch,      // eCmdFun
        "@EDID_MODE",        // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "#GET_EDID_MODE",       //ucmd
        14,              // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_Edid_GetMode,     // eCmdFun
        "@EDID_MODE",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "#SET_OUTPUT_HDCP",     //ucmd
        16,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SwitchHDCP,       // eCmdFun
        "@OUTPUT_HDCP",             // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_ShowHelpCommand,       // flag
        "#GET_OUTPUT_HDCP",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetHDCPState,     // eCmdFun
        "@OUTPUT_HDCP",                    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "#SET_POWER",           //ucmd
        10,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SwitchPowerState, // eCmdFun
        "@POWER",        // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_ShowHelpCommand,       // flag
        "#GET_POWER",           //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetPowerState,    // eCmdFun
        "@POWER",              // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "#SET_RS232_BAUD",      //ucmd
        15,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetRs232Baudrate, // eCmdFun
        "@RS232_BAUD",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_ShowHelpCommand,        // flag
        "#GET_RS232_BAUD",      //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetRs232Baudrate, // eCmdFun
        "@RS232_BAUD",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_ShowHelpCommand,        // flag
        "#UPLOAD_USER_EDID",    //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_Edid_SendCustomData,    // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "#SET_AUTO_SWITCH",     //ucmd
        16,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAutoSwitchMode,    // eCmdFun
        "@AUTO_SWITCH",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_ShowHelpCommand,        // flag
        "#GET_AUTO_SWITCH",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAutoSwitchMode,    // eCmdFun
        "@AUTO_SWITCH",    // feedbackstring
        },
    #if 0
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "#SET_DISPLAY",         //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCECDisplay,    // eCmdFun
        "@DISPLAY",    // feedbackstring
        },
    #endif
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "#HELP",                //ucmd
        5,                      // cmdlen
        40,                     // valuelen
        2,                      // cmdoffset
        eFunc_HelpFunction,     // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_BackCodeFunc,        // flag
        "#TESTVERSION",         //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetTestVersion,   // eCmdFun
        _STRING_TEST_FW_VERSION,    // feedbackstring
        },
    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_OFF_CNT",         //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetOffMsgCounter,     // eCmdFun
        "@OFF_CNT",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_OFF_CNT",         //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetOffMsgCounter, // eCmdFun
        "@OFF_CNT",             // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_OFF_DELAY",       //ucmd
        14,                     // cmdlen
        3,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetOffMsgCntDelayTime,    // eCmdFun
        "@OFF_DELAY",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_OFF_DELAY",       //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetOffMsgCntDelayTime,    // eCmdFun
        "@OFF_DELAY",         // feedbackstring
        },
    { Flag_NoFunction|Flag_ShowHelpCommand,     // flag
        "#SET_ON",              //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetDisplayOnAsciiCmd, // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NoFunction|Flag_ShowHelpCommand,     // flag
        "#SET_OF",              //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetDisplayOffAsciiCmd,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NoFunction|Flag_ShowHelpCommand,     // flag
        "#SET_H_ON",            //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetDisplayOnHexCmd,   // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NoFunction|Flag_ShowHelpCommand,     // flag
        "#SET_H_OF",            //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetDisplayOffHexCmd,  // eCmdFun
        " ",                    // feedbackstring
        },
    #endif

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SYNCACT_CEC",     //ucmd
        16,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAutoSyncDetectActionByCec,    // eCmdFun
        "@SYNCACT_CEC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_SYNCACT_CEC",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAutoSyncDetectActionByCec,    // eCmdFun
        "@SYNCACT_CEC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SYNCACT_RS232",   //ucmd
        18,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAutoSyncDetectActionByRs232,    // eCmdFun
        "@SYNCACT_RS232",       // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_SYNCACT_RS232",   //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAutoSyncDetectActionByRs232,    // eCmdFun
        "@SYNCACT_RS232",       // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DTIME",   //ucmd
        16,                     // cmdlen
        7,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAutoSyncDisplayOffDelayTime,    // eCmdFun
        "@DTIME",       // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_DTIME",   //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAutoSyncDisplayOffDelayTime,    // eCmdFun
        "@DTIME",       // feedbackstring
        },
    #if _ENABLE_AUTO_POWER_FUNCTION
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_AUTO_POWER",   //ucmd
        15,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAutoSyncToPowerOnOff,    // eCmdFun
        "@AUTO_POWER",       // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_AUTO_POWER",   //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAutoSyncToPowerOnOff,    // eCmdFun
        "@AUTO_POWER",       // feedbackstring
        },
    #endif
#endif
    #if _ENABLE_KEYPAD_LOCKED_FUNCTION
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_KEYPAD_LOCK",     //ucmd
        16,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetKeyPadLocked,  // eCmdFun
        "@KEYPAD_LOCK",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_KEYPAD_LOCK",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetKeyPadLocked,  // eCmdFun
        "@KEYPAD_LOCK",         // feedbackstring
        },
    #endif
    #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_DETECT_MODE",     //ucmd
        16,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetInputSignalDetectMode,  // eCmdFun
        "@DETECT_MODE",         // feedbackstring
        },
    { Flag_BackCodeNomal,     // flag
        "#GET_DETECT_MODE",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetInputSignalDetectMode,  // eCmdFun
        "@DETECT_MODE",         // feedbackstring
        },
    #endif
    #if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
    { Flag_BackCodeFunc|Flag_UserValueLength,        // flag
        "#UPLOAD_INTERNAL_EDID",    //ucmd
        21,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_Edid_LoadInternalEdidData,    // eCmdFun
        " ",    // feedbackstring
        },
    #endif
    #if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
    { Flag_BackCodeFunc|Flag_UserValueLength,        // flag
        "#LOAD_ITE_REG",    //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_LoadIteChipRegisterData,    // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength,        // flag
        "#WREG <",                //ucmd
        7,                      // cmdlen
        20,                      // valuelen
        2,                      // cmdoffset
        eFunc_WriteChipRegisterData,    // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength,        // flag
        "#RREG <",                //ucmd
        7,                      // cmdlen
        20,                      // valuelen
        2,                      // cmdoffset
        eFunc_ReadChipRegisterData,    // eCmdFun
        " ",    // feedbackstring
        },
    #endif
    #if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
    // #CEC <XX,XX,XX,XX,XX,XX,XX,XX,XX,XX,XX,XX,XX,XX,XX,XX>
    { Flag_BackCodeFunc|Flag_UserValueLength,        // flag
        "#CEC <",                   //ucmd
        6,                          // cmdlen
        60,                         // valuelen
        2,                          // cmdoffset
        eFunc_SendUserCecCmd,       // eCmdFun
        " ",                        // feedbackstring
        },
    #endif

    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_OUTPUT_RES",     //ucmd
        15,                     // cmdlen
        2,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetResolution,  // eCmdFun
        "@OUTPUT_RES",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_OUTPUT_RES",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetResolution,  // eCmdFun
        "@OUTPUT_RES",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_MV_MODE",     //ucmd
        12,                     // cmdlen
        2,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetMultiviewMode,  // eCmdFun
        "@MV_MODE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_MV_MODE",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetMultiviewMode,  // eCmdFun
        "@MV_MODE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_AUDIO_SRC",       //ucmd
        14,                     // cmdlen
        2,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAudioSource,  // eCmdFun
        "@AUDIO_SRC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_AUDIO_SRC",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAudioSource,  // eCmdFun
        "@AUDIO_SRC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_AUDIO_MIX",       //ucmd
        14,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAudioMix,  // eCmdFun
        "@AUDIO_MIX",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_AUDIO_MIX",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAudioMix,  // eCmdFun
        "@AUDIO_MIX",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_AUDIO_MUTE",       //ucmd
        15,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetAudioMute,  // eCmdFun
        "@AUDIO_MUTE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_AUDIO_MUTE",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAudioMute,  // eCmdFun
        "@AUDIO_MUTE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_MENU",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcMenu,  // eCmdFun
        "@SRC_MENU",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_UP",       //ucmd
        11,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcUp,  // eCmdFun
        "@SRC_UP",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_DOWN",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcDown,  // eCmdFun
        "@SRC_DOWN",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_LEFT",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcLeft,  // eCmdFun
        "@SRC_LEFT",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_RIGHT",       //ucmd
        14,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcRight,  // eCmdFun
        "@SRC_RIGHT",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_BACK",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcBack,  // eCmdFun
        "@SRC_BACK",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_ENTER",       //ucmd
        14,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcEnter,  // eCmdFun
        "@SRC_ENTER",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_ON",       //ucmd
        11,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcOn,  // eCmdFun
        "@SRC_ON",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_OFF",       //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcOff,  // eCmdFun
        "@SRC_OFF",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_STOP",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcStop,  // eCmdFun
        "@SRC_STOP",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_PLAY",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcPlay,  // eCmdFun
        "@SRC_PLAY",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_PAUSE",       //ucmd
        14,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcPause,  // eCmdFun
        "@SRC_PAUSE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_PREV",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcPrev,  // eCmdFun
        "@SRC_PREV",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_NEXT",       //ucmd
        13,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcNext,  // eCmdFun
        "@SRC_NEXT",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_REW",       //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcRewind,  // eCmdFun
        "@SRC_REW",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_SRC_FF",       //ucmd
        11,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetCecSrcFastForward,  // eCmdFun
        "@SRC_FF",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DIS_ON",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCecDisOn,  // eCmdFun
        "@DIS_ON",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DIS_OFF",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCecDisOff,  // eCmdFun
        "@DIS_OFF",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DIS_SOURCE",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCecDisSource,  // eCmdFun
        "@DIS_SOURCE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DIS_MUTE",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCecDisMute,  // eCmdFun
        "@DIS_MUTE/UNMUTE",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DIS_VOL+",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCecDisVolPlus,  // eCmdFun
        "@DIS_VOL+",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_DIS_VOL-",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCecDisVolMinus,  // eCmdFun
        "@DIS_VOL-",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_MV_USER",       //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_EnableMvUserConfig,  // eCmdFun
        "@SET_MV_USER",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#CFM_MV_USER",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_ConfirmMvUserSetting,  // eCmdFun
        "@CFM_MV_USER",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_WINA",       //ucmd
        9,                     // cmdlen
        20,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetSizeAndPositionWinA,  // eCmdFun
        "@WINA",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_WINB",       //ucmd
        9,                     // cmdlen
        20,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetSizeAndPositionWinB,  // eCmdFun
        "@WINB",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_WINC",       //ucmd
        9,                     // cmdlen
        20,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetSizeAndPositionWinC,  // eCmdFun
        "@WINC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_WIND",       //ucmd
        9,                     // cmdlen
        20,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetSizeAndPositionWinD,  // eCmdFun
        "@WIND",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#CFG_WINA",       //ucmd
        9,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_CfgSizeAndPositionWinA,  // eCmdFun
        "@CFG_WINA",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#CFG_WINB",       //ucmd
        9,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_CfgSizeAndPositionWinB,  // eCmdFun
        "@CFG_WINB",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#CFG_WINC",       //ucmd
        9,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_CfgSizeAndPositionWinC,  // eCmdFun
        "@CFG_WINC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#CFG_WIND",       //ucmd
        9,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_CfgSizeAndPositionWinD,  // eCmdFun
        "@CFG_WIND",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#PCFG_WINA",       //ucmd
        10,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_PixelCfgSizeAndPositionWinA,  // eCmdFun
        "@PCFG_WINA",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#PCFG_WINB",       //ucmd
        10,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_PixelCfgSizeAndPositionWinB,  // eCmdFun
        "@PCFG_WINB",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#PCFG_WINC",       //ucmd
        10,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_PixelCfgSizeAndPositionWinC,  // eCmdFun
        "@PCFG_WINC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#PCFG_WIND",       //ucmd
        10,                     // cmdlen
        25,                      // valuelen
        3,                      // cmdoffset
        eFunc_PixelCfgSizeAndPositionWinD,  // eCmdFun
        "@PCFG_WIND",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SAVE_SCREEN",       //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SaveScreenToUserMode,  // eCmdFun
        "@SAVE_SCREEN",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#CLR_SCREEN",       //ucmd
        11,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_CleanScreen,  // eCmdFun
        "@CLR_SCREEN",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_IP_ADDR",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetIpAddress,  // eCmdFun
        "@IP_ADDR:",         // feedbackstring
        },

    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_IP",       //ucmd
        7,                     // cmdlen
        60,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetIpAddress,  // eCmdFun
        "@SET_IP",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#GIVE_IP",       //ucmd
        8,                     // cmdlen
        60,                      // valuelen
        2,                      // cmdoffset
        eFunc_GiveIpAddress,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#SET_SWAP_SRC",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetSwapSource,  // eCmdFun
        "@SWAP_SRC",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#SET_RESIZE_WIM",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetResizeWin,  // eCmdFun
        "@RESIZE_WIM",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_ShowHelpCommand,     // flag
        "#GET_STATUS",       //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetAllSystemSta,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_WIN_LEVEL",       //ucmd
        14,                     // cmdlen
        12,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetWinLayerLevel,  // eCmdFun
        "@WIN_LEVEL",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#GET_WIN_LEVEL",       //ucmd
        14,                     // cmdlen
        2,                      // valuelen
        3,                      // cmdoffset
        eFunc_GetWinLayerLevel,  // eCmdFun
        "@WIN_LEVEL",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SET_NAME",       //ucmd
        9,                     // cmdlen
        SIZE_PRJ_NAME_CHARS_MAX,                     // valuelen
        3,                      // cmdoffset
        eFunc_SetProductName,  // eCmdFun
        "@NAME",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#GET_NAME",       //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetProductName,   // eCmdFun
        "@NAME",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#RESET_NAME",       //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_ResetProductName,  // eCmdFun
        "@RESET_NAME",         // feedbackstring
        },
    #if _ENABLE_SWITCH_PORT_AND_AUDIO
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#SET_FULL_SWAUD",       //ucmd
        15,                     // cmdlen
        1,                     // valuelen
        3,                      // cmdoffset
        eFunc_SetFullModeToSwitchAudio,  // eCmdFun
        "@FULL_SWAUD",         // feedbackstring
        },
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#GET_FULL_SWAUD",       //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetFullModeToSwitchAudio,  // eCmdFun
        "@FULL_SWAUD",         // feedbackstring
        },
    #endif
    { Flag_BackCodeNomal|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "#GET_INPUT_RES",       //ucmd
        15,                     // cmdlen
        1,                     // valuelen
        3,                      // cmdoffset
        eFunc_GetInputResolution,  // eCmdFun
        "@INPUT_RES",         // feedbackstring
        },
    { Flag_BackCodeNomal,        // flag
        "#GET_TX_TIMING",                   //ucmd
        14,                          // cmdlen
        1,                         // valuelen
        3,                          // cmdoffset
        eFunc_GetTxTiming,       // eCmdFun
        " ",                        // feedbackstring
        },
    { Flag_BackCodeNomal,        // flag
        "#GET_RX_TIMING",                   //ucmd
        14,                          // cmdlen
        1,                         // valuelen
        3,                          // cmdoffset
        eFunc_GetRxTiming,       // eCmdFun
        " ",                        // feedbackstring
        },
    { Flag_BackCodeNomal,        // flag
        ">AppConnect",                   //ucmd
        UseStrlen,                          // cmdlen
        0,                         // valuelen
        2,                          // cmdoffset
        eFunc_AppLink,       // eCmdFun
        "<ConnectionSuccessful",                        // feedbackstring
        },
    { Flag_BackCodeNomal,        // flag
        ">GetEdidPhyAddr",                   //ucmd
        UseStrlen,                          // cmdlen
        0,                         // valuelen
        2,                          // cmdoffset
        eFunc_GetDisplayEdidPhysicalAddress,       // eCmdFun
        "<PhysicalAddress",                        // feedbackstring
        },

    { Flag_BackCodeNomal,        // flag
        ">GetDisplayEdid",                   //ucmd
        UseStrlen,                          // cmdlen
        0,                         // valuelen
        2,                          // cmdoffset
        eFunc_GetDisplayEdid,       // eCmdFun
        "<GetDisplayEdid",                        // feedbackstring
        },

    { Flag_BackCodeNomal|Flag_UserValueLength,     // flag
        "#SAVE_PIXEL",       //ucmd
        11,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SavePixelToUserMode,  // eCmdFun
        "@SAVE_PIXEL",         // feedbackstring
        },


    // end
    {Flag_End,  "", 0,  0, 0,  eFunc_Max,  ""},
};


#define Cmdlength(x)    (strlen(cmd_list[x].ucmd)+cmd_list[x].valuelen+cmd_list[x].cmdoffset)

#endif

