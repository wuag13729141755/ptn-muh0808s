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
    eFunc_GetOneInputStutus,
    eFunc_SetRxEdidMode,
    eFunc_GetRxEdidMode,
    eFunc_UpdateUserEdid,
    eFunc_PowerStandby,
    eFunc_PowerWakeup,
    eFunc_GetPowerState,
    eFunc_SetCtrlBaudrate,
    eFunc_GetCtrlBaudrate,
    eFunc_SetOutBaudrate,
    eFunc_GetOutBaudrate,
    eFunc_HelpFunction,
    eFunc_GetTestVersion,
    eFunc_SetKeyPadLocked,
    eFunc_GetKeyPadLocked,
    eFunc_SetTxResolution,
    eFunc_GetTxResolution,
    eFunc_GetRxResolution,
    eFunc_SetTxHdcpMode,
    eFunc_GetTxHdcpMode,
    eFunc_SetTxFormat,
    eFunc_GetTxFormat,
    eFunc_SetTxExtAudio,
    eFunc_GetTxExtAudio,
    eFunc_SaveScene,
    eFunc_CallScene,
    eFunc_DeleteScene,
    eFunc_SceneState,
    eFunc_GetDeviceInfo,
    eFunc_GetLinkInfo,
    eFunc_SetProductName,
    eFunc_SetTxAudioSource,
    eFunc_GetTxAudioSource,

    //////
    eFunc_SetIPAddr,
    eFunc_GetIPAddr,
    eFunc_SetNetMask,
    eFunc_GetNetMask,
    eFunc_SetGateway,
    eFunc_GetGateway,
    eFunc_SetNetConfig,
    eFunc_GetNetConfig,
    eFunc_SetDebugEn,
    eFunc_UpdateMcu,
    eFunc_UpdateFpga,
    eFunc_SetRs232Mode,
    eFunc_GetRs232Mode,
    eFunc_SetSplitMode,
    eFunc_GetSplitMode,

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
    u16      flag;           // 标志
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

#define Flag_End            0
#define Flag_BackCodeNomal  0x01
#define Flag_BackCodeFunc   0x02
#define Flag_ShowHelpCommand    0x04
#define Flag_UserValueLength    0x08
#define Flag_WholeWord      0x10
#define Flag_HeadMatch      0x20
#define Flag_MiddleMatch    0x40
#define Flag_NoFunction     0x80

stUartCmdList2 cmd_list[]=
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
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "(get,ch,",              //ucmd
        UseStrlen,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetOneInputStutus,   // eCmdFun
        " ",             // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,       // flag
        "(edid,config,",       //ucmd
        13,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetRxEdidMode,      // eCmdFun
        " ",        // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(get,i.edid,",       //ucmd
        12,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetRxEdidMode,     // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(update,edid,",       //ucmd
        13,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_UpdateUserEdid,     // eCmdFun
        " ",    // feedbackstring
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
        "(set,i.baud,",      //ucmd
        12,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetCtrlBaudrate, // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,        // flag
        "(get,i.baud)",      //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetCtrlBaudrate, // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,        // flag
        "(set,o.baud,",      //ucmd
        12,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetOutBaudrate, // eCmdFun
        " ",    // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,        // flag
        "(get,o.baud)",      //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetOutBaudrate, // eCmdFun
        " ",    // feedbackstring
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
        "(lock,",     //ucmd
        6,                     // cmdlen
        1,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetKeyPadLocked,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,lock)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetKeyPadLocked,  // eCmdFun
        " ",         // feedbackstring
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
        1,                      // cmdoffset
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
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(get,o.hdcp,",     //ucmd
        12,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetTxHdcpMode,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,o.format,",     //ucmd
        14,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetTxFormat,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(get,o.format,",     //ucmd
        14,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetTxFormat,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,ext.audio,",     //ucmd
        16,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetTxExtAudio,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(get,,ext.audio,",     //ucmd
        16,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetTxExtAudio,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(scene,save,",     //ucmd
        12,                     // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SaveScene,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(scene,call,",     //ucmd
        12,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_CallScene,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(scene,del,",     //ucmd
        11,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_DeleteScene,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(scene,sta,",     //ucmd
        11,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SceneState,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(info,dev)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetDeviceInfo,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(info,link)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetLinkInfo,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(rename,product,",     //ucmd
        16,              // cmdlen
        20,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetProductName,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,ip,",     //ucmd
        8,              // cmdlen
        20,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetIPAddr,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,ip)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetIPAddr,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,mask,",     //ucmd
        10,              // cmdlen
        20,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetNetMask,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,mask)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetNetMask,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,gateway,",     //ucmd
        13,              // cmdlen
        20,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetGateway,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,gateway)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetGateway,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,netcfg,",     //ucmd
        12,              // cmdlen
        50,                      // valuelen
        4,                      // cmdoffset
        eFunc_SetNetConfig,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,netcfg)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetNetConfig,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,audiosrc,",     //ucmd
        14,                     // cmdlen
        5,                      // valuelen
        2,                      // cmdoffset
        eFunc_SetTxAudioSource,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(get,audiosrc,",     //ucmd
        14,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_GetTxAudioSource,  // eCmdFun
        " ",         // feedbackstring
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
        "(update,mcu,",     //ucmd
        12,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_UpdateMcu,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(update,fpga,",     //ucmd
        13,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_UpdateFpga,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,uart,",     //ucmd
        10,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetRs232Mode,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,uart)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetRs232Mode,  // eCmdFun
        " ",         // feedbackstring
    },
    { Flag_HeadMatch|Flag_UserValueLength|Flag_ShowHelpCommand,     // flag
        "(set,split,",     //ucmd
        11,              // cmdlen
        2,                      // valuelen
        1,                      // cmdoffset
        eFunc_SetSplitMode,  // eCmdFun
        " ",         // feedbackstring
        },
    { Flag_HeadMatch|Flag_ShowHelpCommand,     // flag
        "(get,split)",     //ucmd
        UseStrlen,              // cmdlen
        0,                      // valuelen
        0,                      // cmdoffset
        eFunc_GetSplitMode,  // eCmdFun
        " ",         // feedbackstring
        },

    // end
    {Flag_End,  "", 0,  0, 0,  eFunc_Max,  ""},
};


#define Cmdlength(x)    (strlen(cmd_list[x].ucmd)+cmd_list[x].valuelen+cmd_list[x].cmdoffset)

typedef u8 (*pu8pCmdFunc)(char*,u16);
typedef struct _stUartList_t_
{
    u8 u8Cmd;
    pu8pCmdFunc pCmdFun;
}stUartList_t;


#endif

