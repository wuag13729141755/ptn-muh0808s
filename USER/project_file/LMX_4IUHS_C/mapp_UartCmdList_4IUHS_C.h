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
    eFunc_PowerStandby,
    eFunc_PowerWakeup,
    eFunc_GetPowerState,
    eFunc_HelpFunction,
    eFunc_GetTestVersion,
    eFunc_SetTxResolution,
    eFunc_GetTxResolution,
    eFunc_GetRxResolution,
    eFunc_SetTxHdcpMode,
    eFunc_GetTxHdcpMode,
    eFunc_SetTxFormat,
    eFunc_GetTxFormat,

    eFunc_SetFreezeState,
    eFunc_GetFreezeState,
    eFunc_SwitchFreeze,

    eFunc_SetBrightness,
    eFunc_GetBrightness,
    eFunc_SetContrast,
    eFunc_GetContrast,
    eFunc_SetSaturation,
    eFunc_GetSaturation,
    eFunc_SetHue,
    eFunc_GetHue,

    eFunc_SetRxEdidMode,
    eFunc_GetRxEdidMode,
    eFunc_UpdateUserEdid,
    eFunc_GetTxEdidData,

    eFunc_SetTxTiming,
    eFunc_SetTxPattern,
    eFunc_GetTxPattern,
    eFunc_SetColor,
    eFunc_GetColor,
    eFunc_SetDebugEn,
    eFunc_SetMs1826Hold,
    eFunc_BypassSwitchPort,
    eFunc_ModeSwitch,

    eFunc_CheckOutputCardType,
    eFunc_CheckCardFWVersion,
#if ENABLE_CARD_YBJ_RECORD
    eFunc_CheckYBJCardType,
    eFunc_CheckYBJ2CardType,
#endif
    eFunc_SetCardFunction,
    eFunc_SetSTKCardFunction,
    eFunc_CheckCardMode,
    eFunc_CheckSetEdidData,
    eFunc_FactoryReset2,

    eFunc_Max,
}eCmdFun_t;

typedef enum _emCommandFunctionStatusType_
{
    emCmdError = 0,
    emCmdSucess,
    emCmdOutOfRange,
    emCmdNone,

    emCmdStatusMax,
}emCmdFuncStaType;

typedef struct _stUartCmdList2
{
    u8      flag;           // 标志
    char*   ucmd;           // 指令或者指令开头部分
    u8      cmdlen;          // 指令长度
    u16      valuelen;       // 指令附带参数长度,全词匹配则为0
    u8     cmdoffset;       // 指令偏移量,一般都是指令结尾符的长度 ,根据不同指令格式填写
    eCmdFun_t    eCmdFun;   // 指令功能
    char* feedbackstring;
}stUartCmdList2;

// command list Flag define
#define UserOffset      255
#define MaxCmdNum       255
#define UseStrlen       0
#define NoString        NULL

#define Flag_End            0
#define Flag_BackCodeNomal  0x01
#define Flag_BackCodeFunc   0x02
#define Flag_ShowHelpCommand    0x04
#define Flag_UserValueLength    0x08
#define Flag_NomalMatch     0x10
#define Flag_HeadMatch      0x20
#define Flag_MiddleMatch    0x40
#define Flag_NoFunction     0x80

const stUartCmdList2 cmd_list[]=
{
    { Flag_HeadMatch|Flag_ShowHelpCommand,       /*flag*/
        "(ver)",/*ucmd*/
        UseStrlen,              /*cmdlen*/
        0,                      // valuelen
        0,                      /*cmdoffset*/
        eFunc_GetVersion,       /*eCmdFun*/
        _STRING_FW_VER_NUM,           /*feedbackstring*/
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,       // flag
        "(reset)",                 //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_FactoryReset,     // eCmdFun
        "factory reset",        // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,        // flag
        "(reboot)",              //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_Reboot,           // eCmdFun
        "system reboot",    // feedbackstring
        },
    //(sw,01,01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16)
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       //flag
        "(sw,",              //ucmd
        4,                      // cmdlen
        50,                      // valuelen
        1,                      // cmdoffset
        eFunc_SwitchInputPort,  // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,       // flag
        "(get,sw)",              //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetInputStutus,   // eCmdFun
        " ",             // feedbackstring
        },

    { Flag_HeadMatch|Flag_ShowHelpCommand,       // flag
        "(standby)",           //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_PowerStandby, // eCmdFun
        " ",        // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,       // flag
        "(wakeup)",           //ucmd
        UseStrlen,                     // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_PowerWakeup, // eCmdFun
        " ",        // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,       // flag
        "(get,power)",           //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetPowerState,    // eCmdFun
        " ",              // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(help)",                //ucmd
        5,                      // cmdlen
        40,                     // valuelen
        2,                      // cmdoffset
        eFunc_HelpFunction,     // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_HeadMatch,        // flag
        "(info,testversion)",         //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetTestVersion,   // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,o.res,",     //ucmd
        11,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetTxResolution,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(get,o.res,",     //ucmd
        11,              // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetTxResolution,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(get,i.res,",     //ucmd
        11,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetRxResolution,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,o.hdcp,",     //ucmd
        12,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetTxHdcpMode,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,o.hdcp,",     //ucmd
        12,              // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetTxHdcpMode,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,o.format,",     //ucmd
        14,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetTxFormat,      // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,o.format,",     //ucmd
        14,              // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetTxFormat,      // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,freeze,",         //ucmd
        12,                     // cmdlen
        1,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetFreezeState,   // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,freeze,",         //ucmd
        12,              // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetFreezeState,   // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "G0",                   //ucmd
        2,                      // cmdlen
        1,                      // valuelen
        2,                      // cmdoffset
        eFunc_SwitchFreeze,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,brightness,",         //ucmd
        16,                     // cmdlen
        6,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetBrightness,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,brightness,",     //ucmd
        16,                     // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetBrightness,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,contrast,",       //ucmd
        14,                     // cmdlen
        6,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetContrast,      // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,contrast,",       //ucmd
        14,                     // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetContrast,      // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,saturation,",     //ucmd
        16,                     // cmdlen
        6,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetSaturation,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,saturation,",     //ucmd
        16,                     // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetSaturation,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,hue,",            //ucmd
        9,                      // cmdlen
        6,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetHue,           // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand|Flag_UserValueLength,     // flag
        "(get,hue,",            //ucmd
        8,                      // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_GetHue,           // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "(edid,config,",        //ucmd
        13,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetRxEdidMode,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(get,i.edid,",         //ucmd
        12,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetRxEdidMode,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(update,edid,",        //ucmd
        13,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_UpdateUserEdid,   // eCmdFun
        " ",    // feedbackstring
        },

    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(get,o.edid,",         //ucmd
        12,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetTxEdidData,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(set,timing,",         //ucmd
        12,                     // cmdlen
        60,                     // valuelen
        2,                      // cmdoffset
        eFunc_SetTxTiming,      // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(set,pattern,",         //ucmd
        13,                     // cmdlen
        6,                     // valuelen
        2,                      // cmdoffset
        eFunc_SetTxPattern,      // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(get,pattern,",         //ucmd
        13,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetTxPattern,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "(set,color,",        //ucmd
        11,                     // cmdlen
        13,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetColor,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(get,color,",         //ucmd
        11,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetColor,    // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(debug,",     //ucmd
        7,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetDebugEn,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,ms1826hold,",     //ucmd
        16,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetMs1826Hold,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       //flag
        "(set,bypass,",              //ucmd
        12,                      // cmdlen
        50,                      // valuelen
        1,                      // cmdoffset
        eFunc_BypassSwitchPort,  // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       //flag
        "(set,workmode,",              //ucmd
        14,                      // cmdlen
        5,                      // valuelen
        1,                      // cmdoffset
        eFunc_ModeSwitch,  // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "RFF.",                   //ucmd
        4,                      // cmdlen
        10,                      // valuelen
        0,                      // cmdoffset
        eFunc_FactoryReset2,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "FFF.",                   //ucmd
        4,                      // cmdlen
        10,                      // valuelen
        0,                      // cmdoffset
        eFunc_FactoryReset2,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "M",                   //ucmd
        1,                      // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_CheckOutputCardType,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "V",                   //ucmd
        1,                      // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_CheckCardFWVersion,     // eCmdFun
        " ",                    // feedbackstring
        },
#if ENABLE_CARD_YBJ_RECORD
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "K",                   //ucmd
        1,                      // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_CheckYBJCardType,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "B",                   //ucmd
        1,                      // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_CheckYBJ2CardType,     // eCmdFun
        " ",                    // feedbackstring
        },
#endif
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "T",                   //ucmd
        1,                      // cmdlen
        2,                      // valuelen
        2,                      // cmdoffset
        eFunc_CheckCardMode,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "S",                   //ucmd
        1,                      // cmdlen
        300,                      // valuelen
        2,                      // cmdoffset
        eFunc_CheckSetEdidData,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        ":",                   //ucmd
        1,                      // cmdlen
        50,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetCardFunction,     // eCmdFun
        " ",                    // feedbackstring
        },
    { Flag_NomalMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "P",                   //ucmd
        1,                      // cmdlen
        50,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetSTKCardFunction,     // eCmdFun
        " ",                    // feedbackstring
        },


    // end
    {Flag_End,  "", 0,  0, 0,  eFunc_Max,  ""},
};


#define Cmdlength(x)    (strlen(cmd_list[x].ucmd)+cmd_list[x].valuelen+cmd_list[x].cmdoffset)

#endif

