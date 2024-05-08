#ifndef __CMD_LIST_H__
#define __CMD_LIST_H__


typedef enum _emCommandFunctionStatusType_
{
    emCmdError = 0,
    emCmdSucess,
    emCmdOutOfRange,

    emCmdStatusMax,
}emCmdFuncStaType;

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

//==========================================================================
typedef enum
{
    eIntFunc_NoUse = 0,
    eIntFunc_CmdOk,
    eIntFunc_CmdError,
    eIntFunc_SwitchInputID,
    eIntFunc_GetInputID,
    eIntFunc_GetLocalID,
    eIntFunc_SetLocalID,
    eIntFunc_SetKeyLock,
    eIntFunc_SetEncrypt,
    eIntFunc_SetEdidData,
    eIntFunc_SetUartDataBypass,
    eIntFunc_SetIOMode,
    eIntFunc_SetIOOutStatus,
    eIntFunc_GetIOInputStatus,
    eIntFunc_SendIRData,
    eIntFunc_SetUartBaudrate,
    eIntFunc_GetDeviceVersion,
    eIntFunc_GetDeviceType,

    eIntFunc_Max,
}eIntCmdFun_t;

typedef struct _stUartIntCmdList
{
    u8      flag;               // 鏍囧織
    u8      pu8cmd[100];        // 鎸囦护鎴栬€呮寚浠ゅ紑澶撮儴鍒�
    u8      cmdlen;             // 鎸囦护闀垮害
    u16     valuelen;           // 鎸囦护闄勫甫鍙傛暟闀垮害,鍏ㄨ瘝鍖归厤鍒欎负0
    u8      cmdoffset;          // 鎸囦护鍋忕Щ閲�,涓€鑸�閮芥槸鎸囦护缁撳熬绗︾殑闀垮害 ,鏍规嵁涓嶅悓鎸囦护鏍煎紡濉�鍐�
    eIntCmdFun_t    eCmdFun;    // 鎸囦护鍔熻兘
    char* feedbackstring;
}stUartIntCmdList;


stUartIntCmdList InternalCmd_list[]=
{
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0xFF,0x00,0x00,0x5F},     /*pu8cmd*/
        5,                              /*cmdlen*/
        0,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_CmdOk,                 /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0xFF,0x01,0x00,0x5F},     /*pu8cmd*/
        5,                              /*cmdlen*/
        0,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_CmdError,              /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x03,0x02},          /*pu8cmd*/
        4,                              /*cmdlen*/
        2,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SwitchInputID,         /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x04,0x02},          /*pu8cmd*/
        4,                              /*cmdlen*/
        2,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_GetInputID,            /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x05,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_GetLocalID,            /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x06,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetLocalID,            /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x07,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetKeyLock,            /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x08,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetEncrypt,            /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x0A,0xFF},          /*pu8cmd*/
        4,                              /*cmdlen*/
        256,                            /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetEdidData,           /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0xFF},               /*pu8cmd*/
        3,                              /*cmdlen*/
        1000,                           /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetUartDataBypass,     /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x0B,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetIOMode,             /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x0C,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetIOOutStatus,        /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x0D,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_GetIOInputStatus,      /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x0E},               /*pu8cmd*/
        3,                              /*cmdlen*/
        600,                            /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SendIRData,            /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x0F,0x07},          /*pu8cmd*/
        4,                              /*cmdlen*/
        1,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_SetUartBaudrate,      /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x19,0x03},          /*pu8cmd*/
        4,                              /*cmdlen*/
        3,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_GetDeviceVersion,      /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },
    { Flag_BackCodeNomal,               /*flag*/
        {0xF5,0x03,0x20,0x01},          /*pu8cmd*/
        4,                              /*cmdlen*/
        3,                              /*valuelen*/
        1,                              /*cmdoffset*/
        eIntFunc_GetDeviceType,         /*eCmdFun*/
        " ",                            /*feedbackstring*/
        },


    // end
    {Flag_End,  {0}, 0,  0, 0,  eIntFunc_Max,  ""},
};

#define Cmdlength(x)    (strlen(cmd_list[x].ucmd)+cmd_list[x].valuelen+cmd_list[x].cmdoffset)

#endif

