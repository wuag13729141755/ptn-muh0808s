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

typedef u8 (*pu8pCmdFunc)(char*,u16);
typedef struct _stUartList_t_
{
    u8 u8Cmd;
    pu8pCmdFunc pCmdFun;
}stUartList_t;

#endif

