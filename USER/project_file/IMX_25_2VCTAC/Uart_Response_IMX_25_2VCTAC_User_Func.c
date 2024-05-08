#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/IMX_25_2VCTAC/mapp_UartCmdList_IMX_25_2VCTAC.h"

typedef void (*pFuncUartRes)(uint8_t *pu8Data, uint16_t u16Len);

typedef struct _stUartCmdRx_t_
{
	char 				*cmdStr;
	pFuncUartRes		funcResponse;
}stUartCmdRx_t, *pstUartCmdRx_t;

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
bool StartEdidCostumFlag;
u8 g_u8UserEDIDIndex;
bool g_bInternalEdidFlag;
#endif

extern stUartCom_t				g_stUartCommVar, g_stUart1CommVar;
extern const stUartDeclare_t 	stProjectUartWorkTable[];
#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
extern SemaphoreHandle_t		xSemaphore_Uart1Snd;
extern SemaphoreHandle_t		xSemaphore_Uart2Snd;
extern SemaphoreHandle_t		xSemaphore_Uart3Snd;
#endif
BYTE g_u8MultiCmdDelayCounter = 0;

static eCmdFun_t cmdfunctionID = eFunc_NoUse;

void UartCommVarClear(pstUartCom_t pCommVar);

//eFunc_GetVersion,
static emCmdFuncStaType mapp_CmdfuncGetVersion(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    return emCmdSucess;
}

//eFunc_Reboot,
static emCmdFuncStaType mapp_CmdfuncReboot(pstUartCom_t pCommVar,u8 cmdnum)
{
    UNUSED(cmdnum);
    pCommVar->SendPrintf(cmd_list[cmdnum].feedbackstring, TRUE);
    delay_ms(100);
    NVIC_PriorityGroupConfig(0x04);

    return emCmdSucess;
}

//eFunc_GetTestVersion,
static emCmdFuncStaType mapp_CmdfuncGetTestVersion(pstUartCom_t pCommVar,u8 cmdnum)
{
    UNUSED(cmdnum);
    UNUSED(pCommVar);
    pCommVar->SendPrintf("test version %s.%04d%02d%02d%02d%02d%02d\r\n",_STRING_FW_VER_NUM,
                                        g_stTestVersion.u16Year,
                                        g_stTestVersion.u8Month,
                                        g_stTestVersion.u8Date,
                                        g_stTestVersion.u8Hour,
                                        g_stTestVersion.u8minute,
                                        g_stTestVersion.u8Second);

    return emCmdSucess;
}

//===============================================================================
//Function process
//===============================================================================
void  mapp_SetCommandFunctionID(eCmdFun_t cmdfuncid)
{
    cmdfunctionID = cmdfuncid;
}

eCmdFun_t  mapp_GetCommandFunctionID(void)
{
    return cmdfunctionID;
}

//***********************************************************
//@Function name:    mapp_CommandFunctionProcess
//@Input:   u8 flag 标志 , char *cmd 指令集
//@Output: l_bStatus 是否执行成功
//@Description: 根据功能标志，执行相应的功能函数
//              预留了标志位，还有修改的空间
//***********************************************************
emCmdFuncStaType mapp_CommandFunctionProcess(u8 cmdnum,u8 flag,pstUartCom_t pCommVar)
{
    emCmdFuncStaType l_bStatus=emCmdError;

    UNUSED(flag);
    //UNUSED(cmdnum);
    switch( mapp_GetCommandFunctionID())
    {
        case eFunc_GetVersion:
            {
                l_bStatus = mapp_CmdfuncGetVersion(pCommVar,cmdnum);
            }
            break;
        case eFunc_FactoryReset:
            {
                l_bStatus = mapp_CmdfuncFactoryReset(pCommVar,cmdnum);
            }
            break;
        case eFunc_Reboot:
            {
                l_bStatus = mapp_CmdfuncReboot(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTestVersion:
            {
                l_bStatus = mapp_CmdfuncGetTestVersion(pCommVar,cmdnum);
            }
            break;

        case eFunc_NoUse:
        case eFunc_Max:
        default:
            break;
    }
    if(( mapp_GetCommandFunctionID()!=eFunc_NoUse)&&( mapp_GetCommandFunctionID()!=eFunc_Max))
    {
         mapp_SetCommandFunctionID(eFunc_NoUse);
    }
    return l_bStatus;
}

bool mapp_CmdlenIsOK(u8 index, pstUartCom_t pCommVar)
{
    //RS232_printf("cmdlen:%d\r\n",pCommVar->u16RecCnt);
    if(cmd_list[index].flag&Flag_UserValueLength)
    {
        if((pCommVar->u16RecCnt<=(strlen(cmd_list[index].ucmd)+cmd_list[index].valuelen+cmd_list[index].cmdoffset))
            &&(pCommVar->u16RecCnt>=(strlen(cmd_list[index].ucmd)+cmd_list[index].cmdoffset)))
            return TRUE;
    }
    else
    {
        if(pCommVar->u16RecCnt==(strlen(cmd_list[index].ucmd)+cmd_list[index].valuelen+cmd_list[index].cmdoffset))
            return TRUE;
    }
    return FALSE;
}

void mapp_UsartSystemCmdFun(pstUartCom_t pCommVar)
{
    u8 i;
    u8 l_u8Flag = 0;
    u8 l_u8Cmdnumber = 0;
    emCmdFuncStaType l_bStatus=emCmdError;
    u8 l_u8MatchCnt = 0;
    char *l_ps8StartStr = pCommVar->pRecBuffer;
    char *l_ps8EndStr = l_ps8StartStr;

    //RS232_printf("Cmd:%s",pCommVar->pRecBuffer);
    {
    RecheckCmd:
        l_u8MatchCnt = 0;
        for(i=0;i<MaxCmdNum;i++)
        {
            if(cmd_list[i].eCmdFun==eFunc_Max || cmd_list[i].flag==Flag_End)
            {
                break;
            }
            if((cmd_list[i].flag&Flag_NoFunction)==Flag_NoFunction)
                continue;
            //RS232_printf("cmdlen:%d\r\n",pCommVar->u16RecCnt);
            //RS232_printf("cmdstr:%s\n",cmd_list[i].ucmd);
            //RS232_printf("strlen:%d\n",strlen(cmd_list[i].ucmd));
            //RS232_printf("cmdlen[%d]:%d\n",i,Cmdlength(i));
            if((cmd_list[i].flag&Flag_HeadMatch)==Flag_HeadMatch)
            {
                if(strncmp(l_ps8StartStr,cmd_list[i].ucmd,strlen(cmd_list[i].ucmd))==0)
                {
                    pCommVar->bIsMultiCmdDetect = TRUE;
                    if(pCommVar->u8MatchCmdCounter<MaxCmdlist)
                    {
                        if(*(l_ps8StartStr+strlen(cmd_list[i].ucmd)-1) != ')')        // 指令不是以')'结尾
                        {
                            if(cmd_list[i].valuelen==0)
                                l_ps8EndStr = FindChar(l_ps8StartStr+strlen(cmd_list[i].ucmd),')',Cmdlength(i)-1);    // delet end char self
                            else
                                l_ps8EndStr = FindChar(l_ps8StartStr+strlen(cmd_list[i].ucmd),')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8StartStr/*+strlen(cmd_list[i].ucmd)*/+1; // 指令结尾加上结束符本身
                        }
                        else
                        {
                            l_ps8EndStr = l_ps8StartStr+strlen(cmd_list[i].ucmd);
                            //l_ps8EndStr = FindChar(l_ps8StartStr+strlen(cmd_list[i].ucmd),')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8StartStr/*+strlen(cmd_list[i].ucmd)*/;
                        }
                        memcpy(pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].s8MultiCmd,l_ps8StartStr/*-strlen(cmd_list[i].ucmd)*/,pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);
                        memset(l_ps8StartStr,'0',pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);   // 覆盖已识别的指令
                        l_ps8StartStr = l_ps8StartStr+pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter;
                        dbg_printf("cmd:%s\r\n",pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].s8MultiCmd);
                        dbg_printf("len:%d\r\n",pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);
                    }
                    else
                    {
                        l_u8MatchCnt = 0;
                        break;
                    }
                    pCommVar->u8MatchCmdCounter++;
                    if(pCommVar->u8MatchCmdCounter>MaxCmdlist)
                    {
                        pCommVar->u8MatchCmdCounter=MaxCmdlist;
                    }
                    l_u8MatchCnt++;

                    break;
                }
                else
                {
//                    if(pCommVar->u8MatchCmdCounter==0)
//                        pCommVar->bIsMultiCmdDetect = FALSE;
                    if(cmd_list[i].eCmdFun==eFunc_Max || cmd_list[i].flag==Flag_End)
                    {
                        break;
                    }
                    else
                        continue;
                }
                //break;
            }
            else if((cmd_list[i].flag&Flag_NomalMatch)==Flag_NomalMatch)
            {
#if 0
                if(strcmp(pCommVar->pRecBuffer,cmd_list[i].ucmd)==0)
                {
                    mapp_SetCommandFunctionID(cmd_list[i].eCmdFun);
                    l_u8Flag = cmd_list[i].flag;
                    l_u8Cmdnumber = i;
                    break;
                }
#endif
                if((strncmp(pCommVar->pRecBuffer,cmd_list[i].ucmd,strlen(cmd_list[i].ucmd))==0)
                    &&(mapp_CmdlenIsOK(i,pCommVar))
                    )
                {
                    mapp_SetCommandFunctionID(cmd_list[i].eCmdFun);
                    l_u8Flag = cmd_list[i].flag;
                    l_u8Cmdnumber = i;
                    break;
                }
            }
            else if((cmd_list[i].flag&Flag_MiddleMatch)==Flag_MiddleMatch)
            {
                char *l_ps8tempStr = my_StrStr(l_ps8StartStr,cmd_list[i].ucmd);
                if(l_ps8tempStr != NULL)
                {
                    pCommVar->bIsMultiCmdDetect = TRUE;
                    if(pCommVar->u8MatchCmdCounter<MaxCmdlist)
                    {
                        if(*(l_ps8tempStr-1) != ')')        // 指令不是以')'结尾
                        {
                            if(cmd_list[i].valuelen==0)
                                l_ps8EndStr = FindChar(l_ps8tempStr,')',Cmdlength(i)-1);    // delet end char self
                            else
                                l_ps8EndStr = FindChar(l_ps8tempStr,')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8tempStr+strlen(cmd_list[i].ucmd)+1; // 指令结尾加上结束符本身
                        }
                        else
                        {
                            l_ps8EndStr = l_ps8tempStr;
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8tempStr+strlen(cmd_list[i].ucmd);
                        }
                        memcpy(pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].s8MultiCmd,l_ps8tempStr-strlen(cmd_list[i].ucmd),pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);
                        l_ps8StartStr = l_ps8StartStr+pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter;
                        dbg_printf("cmd:%s\r\n",pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].s8MultiCmd);
                        dbg_printf("len:%d\r\n",pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);
                    }
                    else
                    {
                        l_u8MatchCnt = 0;
                        break;
                    }
                    memset(l_ps8tempStr-strlen(cmd_list[i].ucmd),'0',strlen(cmd_list[i].ucmd));   // 覆盖已识别的指令
                    pCommVar->u8MatchCmdCounter++;
                    if(pCommVar->u8MatchCmdCounter>MaxCmdlist)
                    {
                        pCommVar->u8MatchCmdCounter=MaxCmdlist;
                    }
                    l_u8MatchCnt++;

                    break;
                }
                else
                {
                    if(pCommVar->u8MatchCmdCounter==0)
                        pCommVar->bIsMultiCmdDetect = FALSE;
                }
            }
        }

        if(l_u8MatchCnt>0)
        {
            goto RecheckCmd;
        }

        #if 0
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            if(( mapp_GetCommandFunctionID() != eFunc_SwitchPowerState)
                &&( mapp_GetCommandFunctionID() != eFunc_GetPowerState)
                &&( mapp_GetCommandFunctionID() != eFunc_FactoryReset)
                )
            {
                mapp_SetCommandFunctionID(eFunc_NoUse);
                return;
            }
        }
        #endif

        if(pCommVar->u8MatchCmdCounter)
            l_bStatus = emCmdNone;
        else
            l_bStatus = mapp_CommandFunctionProcess(l_u8Cmdnumber,l_u8Flag, pCommVar);
    }

    if( l_bStatus == emCmdError )
    {
        pCommVar->SendPrintf("err\r\n");
    }
    else if( l_bStatus == emCmdOutOfRange )
    {
        pCommVar->SendPrintf("out of range\r\n");
    }
    else if( l_bStatus == emCmdSucess)
    {
        pCommVar->SendPrintf("ok\r\n");
    }
}

#if 1
void mapp_MultiCmdMatchFunction(pstUartCom_t pCommVar)
{
    u8 i/*,j*/;
    char *l_ps8Cmdbuf = NULL;  //g_CMMFCmd;
    //u8 l_u8RankingCmdCnt[MaxCmdlist];
    static u8 sl_u8CmdIndex = 0;
    u8 l_u8Flag = 0;
    u8 l_u8Cmdnumber = 0;
    emCmdFuncStaType l_bStatus=emCmdError;

    if(g_u8MultiCmdDelayCounter)
    {
        g_u8MultiCmdDelayCounter--;
        return;
    }

    l_ps8Cmdbuf = pCommVar->stMultiCmdType[sl_u8CmdIndex].s8MultiCmd;
    //dbg_printf("init: %s\r\n",l_ps8Cmdbuf);
    for(i=0; i<MaxCmdNum ; i++)
    {
        if(pCommVar->stMultiCmdType[sl_u8CmdIndex].u8CmdCounter!=0)
        {
            if(strncmp(l_ps8Cmdbuf,cmd_list[i].ucmd,strlen(cmd_list[i].ucmd))==NULL)    // 识别命令
            {
                mapp_SetCommandFunctionID(cmd_list[i].eCmdFun);
                l_u8Flag = cmd_list[i].flag;
                l_u8Cmdnumber = i;
                pCommVar->bIsMultiCmdDetect = TRUE;
                memcpy(pCommVar->pRecBuffer, l_ps8Cmdbuf,pCommVar->stMultiCmdType[sl_u8CmdIndex].u8CmdCounter);
                pCommVar->u16RecCnt = pCommVar->stMultiCmdType[sl_u8CmdIndex].u8CmdCounter;
                sl_u8CmdIndex++;
                break;
            }
            else
            {
                pCommVar->bIsMultiCmdDetect = FALSE;
            }
        }
    }

    l_bStatus = mapp_CommandFunctionProcess(l_u8Cmdnumber,l_u8Flag, pCommVar);
    if(i >= MaxCmdNum||(sl_u8CmdIndex>=MaxCmdlist)||(sl_u8CmdIndex>=pCommVar->u8MatchCmdCounter))
    {
        pCommVar->bIsMultiCmdDetect = FALSE;
        sl_u8CmdIndex = 0;
        pCommVar->u8MatchCmdCounter= 0;
        memset(pCommVar->stMultiCmdType,0,sizeof(stMultiCmdType_t)*MaxCmdlist);     // clean buffer
        //_Printf("end: %d\r\n",0);
    }
    if( l_bStatus == emCmdError )
    {
        pCommVar->SendPrintf("err\r\n");
    }
    else if( l_bStatus == emCmdOutOfRange )
    {
        pCommVar->SendPrintf("out of range\r\n");
    }
    else if( l_bStatus == emCmdSucess)
    {
        pCommVar->SendPrintf("ok\r\n");
    }
}
#endif

void mapp_SetPowerState(bool bPwrSta)
{
	pstProjectSpecVar_t			pstSpecVar = &stIMX_25_2VCTACSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
            SetUserData_SpecBasicVar();
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
            SetUserData_SpecBasicVar();
        }
    }
}

void mapp_Usart1SystemCmdFun(pstUartCom_t pCommVar)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
//    u8 l_u8ID=0;
    if((pCommVar->pRecBuffer[0]==0xFF)&&(pCommVar->pRecBuffer[1]==0xFF)&&(pCommVar->pRecBuffer[2]==0xA5))
    {
        l_u8Addr = pCommVar->pRecBuffer[3];
        l_u8Cmd = pCommVar->pRecBuffer[4];

        dbg_printf("addr[0x%x],cmd[0x%x]\r\n",l_u8Addr,l_u8Cmd);
    }
}

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar)
{
    BYTE CostumEdidbuffer[256] = {0};
    u16 i;
    if(StartEdidCostumFlag)
    {
//        pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
//        pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
        pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTACSpecVar;
        //pstEdidUpgrData_t           pstEdidUpgr;
        pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;

        for(i=0;(i<pUartCommVar->u16RecCnt)&&(i<256);i++)
        {
            CostumEdidbuffer[i] = pUartCommVar->pRecBuffer[i];

            #if 0
            if((i>0)&&(i%8==0))
                RS232_printf("\n");
            RS232_printf("0x%02x,",pUartCommVar->pRecBuffer[i]);
            #endif
        }
        // 128 bit edid
        if( (pUartCommVar->u16RecCnt==128) && (CostumEdidbuffer[0x7e]==00))
        {
            StartEdidCostumFlag=FALSE;
            PlatformTimerSet(etUserEdidReceiveTime,0);
            if(CostumEdidbuffer[0]==0x00
                ||CostumEdidbuffer[1]==0xFF
                ||CostumEdidbuffer[2]==0xFF
                ||CostumEdidbuffer[3]==0xFF
                ||CostumEdidbuffer[4]==0xFF
                ||CostumEdidbuffer[5]==0xFF
                ||CostumEdidbuffer[6]==0xFF
                ||CostumEdidbuffer[7]==0x00
                )
            {
                if(g_u8UserEDIDIndex < emUserEdidMax)
                {
                    pstEdidOp = pstEdidOrg + (g_u8UserEDIDIndex);
                    memset(pstEdidOp, 0, sizeof(stCustomEdid_t));
                    memcpy(pstEdidOp->pu8EdidContext, CostumEdidbuffer, 128);
                    SetUserData_SpecCustomEdid(g_u8UserEDIDIndex);
                    pUartCommVar->SendPrintf("edid %d updated\r\n",g_u8UserEDIDIndex+1);
                    pUartCommVar->SendPrintf("ok\r\n");
                    UartCommVarClear(pUartCommVar);
                }
            }
            else
            {
                pUartCommVar->SendPrintf("err\r\n");
                UartCommVarClear(pUartCommVar);
            }
            return;
        }
        //256 edid
        if(pUartCommVar->u16RecCnt>=255)        //save edid to flash     //edidstore
        {
            StartEdidCostumFlag=FALSE;
            PlatformTimerSet(etUserEdidReceiveTime,0);
            if(CostumEdidbuffer[0]==0x00
                ||CostumEdidbuffer[1]==0xFF
                ||CostumEdidbuffer[2]==0xFF
                ||CostumEdidbuffer[3]==0xFF
                ||CostumEdidbuffer[4]==0xFF
                ||CostumEdidbuffer[5]==0xFF
                ||CostumEdidbuffer[6]==0xFF
                ||CostumEdidbuffer[7]==0x00
                )
            {
                if(g_u8UserEDIDIndex < emUserEdidMax)
                {
                    pstEdidOp = pstEdidOrg + (g_u8UserEDIDIndex);
                    memset(pstEdidOp, 0, sizeof(stCustomEdid_t));
                    memcpy(pstEdidOp->pu8EdidContext, CostumEdidbuffer, 256);
                    SetUserData_SpecCustomEdid(g_u8UserEDIDIndex);
                    pUartCommVar->SendPrintf("edid %d updated\r\n",g_u8UserEDIDIndex+1);
                    pUartCommVar->SendPrintf("ok\r\n");
                    UartCommVarClear(pUartCommVar);
                }
            }
            else
            {
                pUartCommVar->SendPrintf("err\r\n");
                UartCommVarClear(pUartCommVar);
            }
            return;
        }
    }
}
#endif

