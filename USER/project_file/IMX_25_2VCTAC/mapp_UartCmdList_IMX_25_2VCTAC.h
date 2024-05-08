#ifndef __CMD_LIST_H__
#define __CMD_LIST_H__

typedef enum
{
    eFunc_NoUse = 0,
    eFunc_GetVersion,
    eFunc_FactoryReset,
    eFunc_Reboot,
    eFunc_PowerStandby,
    eFunc_PowerWakeup,
    eFunc_GetPowerState,
    eFunc_HelpFunction,
    eFunc_GetTestVersion,

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

    // end
    {Flag_End,  "", 0,  0, 0,  eFunc_Max,  ""},
};


#define Cmdlength(x)    (strlen(cmd_list[x].ucmd)+cmd_list[x].valuelen+cmd_list[x].cmdoffset)

#endif

