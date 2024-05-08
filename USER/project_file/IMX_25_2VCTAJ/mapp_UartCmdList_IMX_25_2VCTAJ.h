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
    eFunc_SwitchPowerState,
    eFunc_GetPowerState,
    eFunc_SetRs232Baudrate,
    eFunc_GetRs232Baudrate,
    eFunc_SetAutoSwitchMode,
    eFunc_GetAutoSwitchMode,
    eFunc_SetCECDisplay,
    eFunc_HelpFunction,
    eFunc_GetTestVersion,
    #if _ENABLE_KEYPAD_LOCKED_FUNCTION
    eFunc_SetKeyPadLocked,
    eFunc_GetKeyPadLocked,
    #endif
    #if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
    eFunc_SendUserCecCmd,
    #endif
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
        3,                      // valuelen
        3,                      // cmdoffset
        eFunc_SwitchInputPort,  // eCmdFun
        "@AV",                    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_ShowHelpCommand,       // flag
        "#GET_AV",              //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetInputStutus,   // eCmdFun
        "@AV",             // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength,       // flag
        "#SET_EDID_MODE",       //ucmd
        14,                     // cmdlen
        2,                      // valuelen
        3,                      // cmdoffset
        eFunc_Edid_Switch,      // eCmdFun
        "@EDID_MODE",        // feedbackstring
        },
    { Flag_BackCodeFunc,        // flag
        "#GET_EDID_MODE",       //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_Edid_GetMode,     // eCmdFun
        "@EDID_MODE",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength,       // flag
        "#SET_POWER",           //ucmd
        10,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SwitchPowerState, // eCmdFun
        "@POWER",        // feedbackstring
        },
    { Flag_BackCodeFunc,       // flag
        "#GET_POWER",           //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetPowerState,    // eCmdFun
        "@POWER",              // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength,        // flag
        "#SET_RS232_BAUD",      //ucmd
        15,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
        eFunc_SetRs232Baudrate, // eCmdFun
        "@RS232_BAUD",    // feedbackstring
        },
    { Flag_BackCodeFunc,        // flag
        "#GET_RS232_BAUD",      //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetRs232Baudrate, // eCmdFun
        "@RS232_BAUD",    // feedbackstring
        },
    { Flag_BackCodeFunc|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "#UPLOAD_USER_EDID",    //ucmd
        17,                     // cmdlen
        1,                      // valuelen
        3,                      // cmdoffset
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
    // end
    {Flag_End,  "", 0,  0, 0,  eFunc_Max,  ""},
};


#define Cmdlength(x)    (strlen(cmd_list[x].ucmd)+cmd_list[x].valuelen+cmd_list[x].cmdoffset)

#endif

