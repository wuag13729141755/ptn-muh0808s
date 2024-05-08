#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/IMX_25_2VCTAP/mapp_UartCmdList_IMX_25_2VCTAP.h"


extern const stUartDeclare_t    stProjectUartWorkTable[];
static eCmdFun_t cmdfunctionID = eFunc_NoUse;
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
bool StartEdidCostumFlag;
u8 g_u8UserEDIDIndex;
bool g_bInternalEdidFlag;
#endif

BYTE g_u8MultiCmdDelayCounter = 0;

void UartCommVarClear(pstUartCom_t pCommVar);

void GetResolutionText(char *ps8ResText,u8 u8ResID)
{
    switch(u8ResID)
    {
        case emRes_1920x1200_60:
            memcpy(ps8ResText,"1920x1200@60Hz\0",strlen("1920x1200@60Hz\0"));
            break;
        case emRes_1920x1080_60:
            memcpy(ps8ResText,"1920x1080@60Hz\0",strlen("1920x1080@60Hz\0"));
            break;
        case emRes_1280x720_60:
            memcpy(ps8ResText,"1280x720@60Hz\0",strlen("1280x720@60Hz\0"));
            break;
        case emRes_1360x768_60:
            memcpy(ps8ResText,"1360x768@60Hz\0",strlen("1360x768@60Hz\0"));
            break;
        case emRes_1280x1024_60:
            memcpy(ps8ResText,"1280x1024@60Hz\0",strlen("1280x1024@60Hz\0"));
            break;
        case emRes_1024x768_60:
            memcpy(ps8ResText,"1024x768@60Hz\0",strlen("1024x768@60Hz\0"));
            break;
        case emRes_1600x1200_60:
            memcpy(ps8ResText,"1600x1200@60Hz\0",strlen("1600x1200@60Hz\0"));
            break;
        case emRes_1400x900_60:
            memcpy(ps8ResText,"1400x900@60Hz\0",strlen("1400x900@60Hz\0"));
            break;
        case emRes_1600x900_60:
            memcpy(ps8ResText,"1600x900@60Hz\0",strlen("1600x900@60Hz\0"));
            break;
        case emRes_1280x720_50:
            memcpy(ps8ResText,"1280x720@50Hz\0",strlen("1280x720@50Hz\0"));
            break;
        case emRes_1920x1080_50:
            memcpy(ps8ResText,"1920x1080@50Hz\0",strlen("1920x1080@50Hz\0"));
            break;
        default:
            break;
    }

}

//eFunc_GetVersion,
static emCmdFuncStaType mapp_CmdfuncGetVersion(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    pCommVar->SendPrintf("mcu ctrl %s\r\n",_STRING_FW_VER_NUM);
    pCommVar->SendPrintf("HW ver %s\r\n", _STRING_HW_VER_NUM);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
//    u8 l_u8SendBuf[1];
    u8 l_u8index;

    for(l_u8index=0;l_u8index<Def_outport_num;l_u8index++)
    {
        pstHdmiSw->u8SwChannel[l_u8index] = l_u8index;
    }

    SetUserData_HdmiSel();
    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();
    IMX_25_2VCTAP_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();

//    l_u8SendBuf[0] = 1;

//    mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_CardInfo,1000);
//    mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_TimingChange,1000);
    #if _ENABLE_SYSTEM_RESET_DELAY_TIME
    mapp_SetSysResetModeDelayTime(1500);
    #endif
    pCommVar->SendPrintf("factory reset\r\n");

    return emCmdSucess;
}

//eFunc_Reboot,
static emCmdFuncStaType mapp_CmdfuncReboot(pstUartCom_t pCommVar,u8 cmdnum)
{
    UNUSED(cmdnum);
    pCommVar->SendPrintf(cmd_list[cmdnum].feedbackstring, TRUE);
    delay_ms(100);
    //NVIC_PriorityGroupConfig(0x04);
    SYS_SoftReset();

    return emCmdSucess;
}

//eFunc_SwitchInputPort,
static emCmdFuncStaType mapp_CmdfuncSwitchInputPort(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i=0,j=0,l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8Output[Def_outport_num],l_s8Temp;

    memset(l_s8Output,0,Def_outport_num);

    l_s8Input = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;
    do
    {
        if(*l_pCmd == 'a')
        {
            for(i=0;i<Def_outport_num;i++)
            {
                l_s8Output[i] = i;
            }
            break;
        }
        l_s8Temp = (char)GetNumofStr(l_pCmd,',',3);
        l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
        if(l_s8Temp>0&&l_s8Temp<=Def_outport_num)
        {
            l_pCmd = l_pCmd+l_u8NumLen+1;
            l_s8Output[i] =l_s8Temp-1;
            i++;
        }
        else
        {
            l_s8Temp = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            if(l_s8Temp>0&&l_s8Temp<=Def_outport_num)
            {
                l_pCmd = l_pCmd+l_u8NumLen+1;
                l_s8Output[i] =l_s8Temp-1;
                i++;
            }
            else
                break;
        }

        if(i>=Def_outport_num)
            break;
    }
    while(1);

    l_s8Temp = i;
    bubble_sort((u8*)l_s8Output,l_s8Temp,0);

    for(i=0;i<Def_outport_num;i++)
    {
        if(i == l_s8Output[j])
        {
            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = l_s8Input-1;
            j++;
            pCommVar->SendPrintf("out %d in %d\r\n",i+1,l_s8Input);
        }
    }
    SetUserData_HdmiSel();

//    mapp_IMX_25_2VCTAPSendCmdToTXBD(1,eMcuReg_SendInputChannel,pstPrjSaveVar->stHdmiSel.u8SwChannel,Def_outport_num);
//    mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_CardInfo,1000);
//    mapp_IMX_25_2VCTAP_PlatformTimerSet(emUserTimer_TimingChange,1000);

    return l_state;

}

//eFunc_HelpFunction,
static emCmdFuncStaType mapp_CmdfuncHelpFunction(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 i;

    for(i=0;i<MaxCmdNum;i++)
    {
        if(cmd_list[i].eCmdFun==eFunc_Max || cmd_list[i].flag==Flag_End)
        {
            break;
        }
        if((cmd_list[i].flag&Flag_ShowHelpCommand)==Flag_ShowHelpCommand)
        {
            pCommVar->SendPrintf("%s\r\n",cmd_list[i].ucmd);
        }
    }
    return l_emRtState;
}

//eFunc_GetTestVersion,
static emCmdFuncStaType mapp_CmdfuncGetTestVersion(pstUartCom_t pCommVar,u8 cmdnum)
{
    UNUSED(cmdnum);
    pCommVar->SendPrintf("test version %s.%04d%02d%02d%02d%02d%02d\r\n",_STRING_FW_VER_NUM,
                                        g_stTestVersion.u16Year,
                                        g_stTestVersion.u8Month,
                                        g_stTestVersion.u8Date,
                                        g_stTestVersion.u8Hour,
                                        g_stTestVersion.u8minute,
                                        g_stTestVersion.u8Second);

    return emCmdSucess;
}
//eFunc_PowerStandby,
static emCmdFuncStaType mapp_CmdfuncPowerStandby(pstUartCom_t pCommVar,u8 cmdnum)
{
    pCommVar->SendPrintf("system off\r\n");
    mapp_SetPowerState(FALSE);

    return emCmdSucess;
}
//eFunc_PowerWakeup,
static emCmdFuncStaType mapp_CmdfuncPowerWakeup(pstUartCom_t pCommVar,u8 cmdnum)
{
    pCommVar->SendPrintf("system on\r\n");
    mapp_SetPowerState(TRUE);

    return emCmdSucess;
}
//eFunc_GetPowerState,
static emCmdFuncStaType mapp_CmdfuncGetPowerState(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicSpecVar->bPowerOnSta)
    {
        pCommVar->SendPrintf("system on\r\n");
    }
    else
    {
        pCommVar->SendPrintf("system off\r\n");
    }

    return emCmdSucess;
}
//eFunc_SetCtrlBaudrate,
static emCmdFuncStaType mapp_CmdfuncSetCtrlBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
//    pstUartCom_t pUartCommVar = &g_stUart5CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8BuadID;

    l_s8BuadID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8BuadID>=1&&l_s8BuadID<=5)
    {
        pstBasicVar->u8InRs232BaudrateID = l_s8BuadID;
        pCommVar->SendPrintf("%s %d\r\n","in baudrate", mapp_GetRealBaudrate(l_s8BuadID));
        pCommVar->SendPrintf("ok\r\n");
        SetUserData_SpecBasicVar();

        delay_ms(10);
//        pUartCommVar->SetBaudRate(mapp_GetRealBaudrate(l_s8BuadID), DEF_UART_DATA_CONFIG);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_SetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncSetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Lock;

    l_s8Lock = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Lock>=0&&l_s8Lock<=1)
    {
        pstBasicVar->bIsLock = l_s8Lock?TRUE:FALSE;
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s\r\n",l_s8Lock?"lock":"unlock");
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncGetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock?"lock":"unlock");
    return l_state;
}

//eFunc_SaveScene,
static emCmdFuncStaType mapp_CmdfuncSaveScene(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=1)
    {
        pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag = TRUE;
        memcpy(pstBasicVar->stScene[l_s8SceneID-1].u8VideoCh,pstPrjSaveVar->stHdmiSel.u8SwChannel,Def_outport_num);
        pCommVar->SendPrintf("scene %d saved\r\n",l_s8SceneID);
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_CallScene,
static emCmdFuncStaType mapp_CmdfuncCallScene(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID;
    u8 i;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=1)
    {
        if(pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag == TRUE)
        {
            memcpy(pstPrjSaveVar->stHdmiSel.u8SwChannel,pstBasicVar->stScene[l_s8SceneID-1].u8VideoCh,Def_outport_num);

            for(i=0;i<Def_outport_num;i++)
            {
            }
            SetUserData_HdmiSel();
            pCommVar->SendPrintf("scene %d called\r\n",l_s8SceneID);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_DeleteScene,
static emCmdFuncStaType mapp_CmdfuncDeleteScene(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=1)
    {
        if(pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag == TRUE)
        {
            pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag = FALSE;
            SetUserData_SpecBasicVar();
            pCommVar->SendPrintf("scene %d deleted\r\n",l_s8SceneID);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SceneState
static emCmdFuncStaType mapp_CmdfuncSceneState(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8SceneID;
    u8 i;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=0)
    {
        if(l_s8SceneID==0)
        {
            for(i=0;i<32;i++)
            {
                if(pstBasicVar->stScene[i].u8SceneSaveFlag == TRUE)
                {
                    pCommVar->SendPrintf("scene %d is be ready\r\n",i+1);
                }
                else
                {
                    pCommVar->SendPrintf("scene %d is not ready\r\n",i+1);
                }
            }
        }
        else
        {
            if(pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag == TRUE)
            {
                pCommVar->SendPrintf("scene %d is be ready\r\n",l_s8SceneID);
            }
            else
            {
                pCommVar->SendPrintf("scene %d is not ready\r\n",l_s8SceneID);
            }
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetDeviceInfo,
static emCmdFuncStaType mapp_CmdfuncGetDeviceInfo(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s\r\n",pstPrjSaveVar->stPrjNameSet.sPrjName);
    pCommVar->SendPrintf("system %s\r\n",pstBasicVar->bPowerOnSta?"on":"off");
    pCommVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock?"lock":"unlock");

    UNUSED(cmdnum);
    return l_state;
}
//eFunc_SetProductName,
static emCmdFuncStaType mapp_CmdfuncSetProductName(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstProjectNameSet_t			pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Len;

    l_u8Len = my_StrStr(l_pCmd,")")-1-l_pCmd;

    if(l_emRtState == emCmdSucess)
    {
        SetUserData_ProjectName(l_pCmd,l_u8Len);
        pCommVar->SendPrintf("product name:%s\r\n",pstPrjNameSet->sPrjName);
    }
    return l_emRtState;
}
//eFunc_SetIPAddr,
static emCmdFuncStaType mapp_CmdfuncSetIPAddr(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8IpAddr[4];
    u8 l_u8NumLen=0,i=0;

    do
    {
        if(i<3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,'.',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,'.',3);
            l_pCmd = l_pCmd+l_u8NumLen+1;
        }
        else if(i==3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            l_pCmd = l_pCmd+l_u8NumLen;
            if(*l_pCmd != ')')
            {
                l_emRtState = emCmdError;
            }
        }
        i++;

        if(i>=4)
            break;
    }
    while(1);

    if(l_emRtState == emCmdSucess)
    {
        memcpy(pstBasicVar->stIPVar.u8IPAddr,l_s8IpAddr,4);
        pCommVar->SendPrintf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                            pstBasicVar->stIPVar.u8IPAddr[1],
                            pstBasicVar->stIPVar.u8IPAddr[2],
                            pstBasicVar->stIPVar.u8IPAddr[3]);
        SetUserData_SpecBasicVar();
    }

    return l_emRtState;
}
//eFunc_GetIPAddr,
static emCmdFuncStaType mapp_CmdfuncGetIPAddr(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                        pstBasicVar->stIPVar.u8IPAddr[1],
                        pstBasicVar->stIPVar.u8IPAddr[2],
                        pstBasicVar->stIPVar.u8IPAddr[3]);

    return l_emRtState;
}

//eFunc_SetNetMask,
static emCmdFuncStaType mapp_CmdfuncSetNetMask(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8IpAddr[4];
    u8 l_u8NumLen=0,i=0;

    do
    {
        if(i<3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,'.',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,'.',3);
            l_pCmd = l_pCmd+l_u8NumLen+1;
        }
        else if(i==3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            l_pCmd = l_pCmd+l_u8NumLen;
            if(*l_pCmd != ')')
            {
                l_emRtState = emCmdError;
            }
        }
        i++;

        if(i>=4)
            break;
    }
    while(1);

    if(l_emRtState == emCmdSucess)
    {
        memcpy(pstBasicVar->stIPVar.u8NetMask,l_s8IpAddr,4);
        pCommVar->SendPrintf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                            pstBasicVar->stIPVar.u8NetMask[1],
                            pstBasicVar->stIPVar.u8NetMask[2],
                            pstBasicVar->stIPVar.u8NetMask[3]);
        SetUserData_SpecBasicVar();
    }

    return l_emRtState;
}

//eFunc_GetNetMask,
static emCmdFuncStaType mapp_CmdfuncGetNetMask(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                        pstBasicVar->stIPVar.u8NetMask[1],
                        pstBasicVar->stIPVar.u8NetMask[2],
                        pstBasicVar->stIPVar.u8NetMask[3]);

    return l_emRtState;
}

//eFunc_SetGateway,
static emCmdFuncStaType mapp_CmdfuncSetGateway(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8IpAddr[4];
    u8 l_u8NumLen=0,i=0;

    do
    {
        if(i<3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,'.',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,'.',3);
            l_pCmd = l_pCmd+l_u8NumLen+1;
        }
        else if(i==3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            l_pCmd = l_pCmd+l_u8NumLen;
            if(*l_pCmd != ')')
            {
                l_emRtState = emCmdError;
            }
        }
        i++;

        if(i>=4)
            break;
    }
    while(1);

    if(l_emRtState == emCmdSucess)
    {
        memcpy(pstBasicVar->stIPVar.u8NetMask,l_s8IpAddr,4);
        pCommVar->SendPrintf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                            pstBasicVar->stIPVar.u8NetGate[1],
                            pstBasicVar->stIPVar.u8NetGate[2],
                            pstBasicVar->stIPVar.u8NetGate[3]);
        SetUserData_SpecBasicVar();
    }

    return l_emRtState;
}

//eFunc_GetGateway,
static emCmdFuncStaType mapp_CmdfuncGetGateway(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                        pstBasicVar->stIPVar.u8NetGate[1],
                        pstBasicVar->stIPVar.u8NetGate[2],
                        pstBasicVar->stIPVar.u8NetGate[3]);

    return l_emRtState;
}

//eFunc_SetNetConfig,
static emCmdFuncStaType mapp_CmdfuncSetNetConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd = pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8IpAddr[4],l_s8Mask[4],l_s8Gate[4];
    u8 l_u8NumLen=0,i=0;

    do
    {
        if(i<3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,'.',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,'.',3);
            l_pCmd = l_pCmd+l_u8NumLen+1;
        }
        else if(i==3)
        {
            l_s8IpAddr[i] = (char)GetNumofStr(l_pCmd,',',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
            l_pCmd = l_pCmd+l_u8NumLen;
            if(*l_pCmd != ',')
            {
                l_emRtState = emCmdError;
            }
            l_pCmd = l_pCmd+1;
        }
        else if(i<7)
        {
            l_s8Mask[i-4] = (char)GetNumofStr(l_pCmd,'.',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,'.',3);
            l_pCmd = l_pCmd+l_u8NumLen+1;
        }
        else if(i==7)
        {
            l_s8Mask[i-4] = (char)GetNumofStr(l_pCmd,',',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
            l_pCmd = l_pCmd+l_u8NumLen;
            if(*l_pCmd != ',')
            {
                l_emRtState = emCmdError;
            }
            l_pCmd = l_pCmd+1;
        }
        else if(i<11)
        {
            l_s8Gate[i-8] = (char)GetNumofStr(l_pCmd,'.',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,'.',3);
            l_pCmd = l_pCmd+l_u8NumLen+1;
        }
        else if(i==11)
        {
            l_s8Gate[i-8] = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            l_pCmd = l_pCmd+l_u8NumLen;
            if(*l_pCmd != ')')
            {
                l_emRtState = emCmdError;
            }
        }
        i++;

        if(i>=12)
            break;
    }
    while(1);

    if(l_emRtState == emCmdSucess)
    {
        memcpy(pstBasicVar->stIPVar.u8IPAddr,l_s8IpAddr,4);
        memcpy(pstBasicVar->stIPVar.u8NetMask,l_s8Mask,4);
        memcpy(pstBasicVar->stIPVar.u8NetGate,l_s8Gate,4);
        pCommVar->SendPrintf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                            pstBasicVar->stIPVar.u8IPAddr[1],
                            pstBasicVar->stIPVar.u8IPAddr[2],
                            pstBasicVar->stIPVar.u8IPAddr[3]);
        pCommVar->SendPrintf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                            pstBasicVar->stIPVar.u8NetMask[1],
                            pstBasicVar->stIPVar.u8NetMask[2],
                            pstBasicVar->stIPVar.u8NetMask[3]);
        pCommVar->SendPrintf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                            pstBasicVar->stIPVar.u8NetGate[1],
                            pstBasicVar->stIPVar.u8NetGate[2],
                            pstBasicVar->stIPVar.u8NetGate[3]);
        SetUserData_SpecBasicVar();
    }

    return l_emRtState;
}

//eFunc_GetNetConfig,
static emCmdFuncStaType mapp_CmdfuncGetNetConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("ip %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8IPAddr[0],
                        pstBasicVar->stIPVar.u8IPAddr[1],
                        pstBasicVar->stIPVar.u8IPAddr[2],
                        pstBasicVar->stIPVar.u8IPAddr[3]);
    pCommVar->SendPrintf("mask %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetMask[0],
                        pstBasicVar->stIPVar.u8NetMask[1],
                        pstBasicVar->stIPVar.u8NetMask[2],
                        pstBasicVar->stIPVar.u8NetMask[3]);
    pCommVar->SendPrintf("gateway %d.%d.%d.%d\r\n",pstBasicVar->stIPVar.u8NetGate[0],
                        pstBasicVar->stIPVar.u8NetGate[1],
                        pstBasicVar->stIPVar.u8NetGate[2],
                        pstBasicVar->stIPVar.u8NetGate[3]);

    return l_emRtState;
}

//eFunc_SetDebugEn
static emCmdFuncStaType mapp_CmdfuncSetDebugEn(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8DebugEn;

    l_s8DebugEn = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8DebugEn<=1)
    {
        pCommVar->SendPrintf("debug: %s\r\n",l_s8DebugEn?"en":"dis");

        pstBasicVar->bIsDebugEn = l_s8DebugEn?TRUE:FALSE;
        SetUserData_SpecBasicVar();
        g_bEnableDbgFlag = pstBasicVar->bIsDebugEn;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_UpdateMcu,
static emCmdFuncStaType mapp_CmdfuncUpdateMcu(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8ID;

    l_s8ID = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8ID>0)&&(l_s8ID<=4))
    {
        UpdateFunction(l_s8ID,0,NULL,0);
        pCommVar->SendPrintf("please choose update file\r\n");
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
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
        case eFunc_SwitchInputPort:
            {
                l_bStatus = mapp_CmdfuncSwitchInputPort(pCommVar,cmdnum);
            }
            break;
        case eFunc_HelpFunction:
            {
                l_bStatus = mapp_CmdfuncHelpFunction(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTestVersion:
            {
                l_bStatus = mapp_CmdfuncGetTestVersion(pCommVar,cmdnum);
            }
            break;
        case eFunc_PowerStandby:
            {
                l_bStatus = mapp_CmdfuncPowerStandby(pCommVar,cmdnum);
            }
            break;
        case eFunc_PowerWakeup:
            {
                l_bStatus = mapp_CmdfuncPowerWakeup(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetPowerState:
            {
                l_bStatus = mapp_CmdfuncGetPowerState(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetKeyPadLocked:
            {
                l_bStatus = mapp_CmdfuncSetKeyPadLocked(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetKeyPadLocked:
            {
                l_bStatus = mapp_CmdfuncGetKeyPadLocked(pCommVar,cmdnum);
            }
            break;
        case eFunc_SaveScene:
            {
                l_bStatus = mapp_CmdfuncSaveScene(pCommVar,cmdnum);
            }
            break;
        case eFunc_CallScene:
            {
                l_bStatus = mapp_CmdfuncCallScene(pCommVar,cmdnum);
            }
            break;
        case eFunc_DeleteScene:
            {
                l_bStatus = mapp_CmdfuncDeleteScene(pCommVar,cmdnum);
            }
            break;
        case eFunc_SceneState:
            {
                l_bStatus = mapp_CmdfuncSceneState(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetDeviceInfo:
            {
                l_bStatus = mapp_CmdfuncGetDeviceInfo(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetProductName:
            {
                l_bStatus = mapp_CmdfuncSetProductName(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetIPAddr:
            {
                l_bStatus = mapp_CmdfuncSetIPAddr(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetIPAddr:
            {
                l_bStatus = mapp_CmdfuncGetIPAddr(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetNetMask:
            {
                l_bStatus = mapp_CmdfuncSetNetMask(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetNetMask:
            {
                l_bStatus = mapp_CmdfuncGetNetMask(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetGateway:
            {
                l_bStatus = mapp_CmdfuncSetGateway(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetGateway:
            {
                l_bStatus = mapp_CmdfuncGetGateway(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetNetConfig:
            {
                l_bStatus = mapp_CmdfuncSetNetConfig(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetNetConfig:
            {
                l_bStatus = mapp_CmdfuncGetNetConfig(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetDebugEn:
            {
                l_bStatus = mapp_CmdfuncSetDebugEn(pCommVar,cmdnum);
            }
            break;
        case eFunc_UpdateMcu:
            {
                l_bStatus = mapp_CmdfuncUpdateMcu(pCommVar,cmdnum);
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
            else if((cmd_list[i].flag&Flag_WholeWord)==Flag_WholeWord)
            {
                if(strcmp(pCommVar->pRecBuffer,cmd_list[i].ucmd)==0)
                {
                    mapp_SetCommandFunctionID(cmd_list[i].eCmdFun);
                    l_u8Flag = cmd_list[i].flag;
                    l_u8Cmdnumber = i;
                    break;
                }
            }
#if 1
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
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8tempStr/*+strlen(cmd_list[i].ucmd)*/+1; // 指令结尾加上结束符本身
                        }
                        else
                        {
                            //l_ps8EndStr = l_ps8tempStr;
                            l_ps8EndStr = FindChar(l_ps8tempStr,')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8tempStr/*+strlen(cmd_list[i].ucmd)*/;
                        }
                        memcpy(pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].s8MultiCmd,l_ps8tempStr/*-strlen(cmd_list[i].ucmd)*/,pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);
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
#endif
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
        pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
        pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
//        pstUartCom_t                pCom4Var = &g_stUart4CommVar;
//        pstUartCom_t                pCom5Var = &g_stUart5CommVar;
//        pstUartCom_t                pCom6Var = &g_stUart6CommVar;

        if(pstBasicVar->u8UartMode==0)
            pCommVar->SendPrintf("err\r\n");
        else
        {
#if 0
            if((pCommVar==pCom5Var)||(pCommVar==pCom6Var))
                pCom4Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
            else if(pCommVar==pCom4Var)
                pCom5Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
#endif
        }
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
        pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
        pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
//        pstUartCom_t                pCom4Var = &g_stUart4CommVar;
//        pstUartCom_t                pCom5Var = &g_stUart5CommVar;
//        pstUartCom_t                pCom6Var = &g_stUart6CommVar;

        if(pstBasicVar->u8UartMode==0)
            pCommVar->SendPrintf("err\r\n");
        else
        {
#if 0
            if((pCommVar==pCom5Var)||(pCommVar==pCom6Var))
                pCom4Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
            else if(pCommVar==pCom4Var)
                pCom5Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
#endif
        }
    }
    else if( l_bStatus == emCmdOutOfRange )
    {
        pCommVar->SendPrintf("out of range\r\n");
        //RS232_printf("%s!\r\n",pCommVar->pRecBuffer);
    }
    else if( l_bStatus == emCmdSucess)
    {
        pCommVar->SendPrintf("ok\r\n");
        //RS232_printf("%s!\r\n",pCommVar->pRecBuffer);
    }
}
#endif

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar)
{
    BYTE CostumEdidbuffer[256] = {0};
    u16 i;
    if(StartEdidCostumFlag)
    {
//        pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
//        pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
        pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
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

void mapp_SetPowerState(bool bPwrSta)
{
    pstProjectSpecVar_t         pstSpecVar = &stIMX_25_2VCTAPSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
        }
    }
}

uint32_t mapp_GetRealBaudrate(uint8_t u8BaudrateID)
{
    uint32_t l_u32Baudrate= 0;
    switch(u8BaudrateID)
    {
        case baud_rate_115200:
            l_u32Baudrate = 115200;
            break;
        case baud_rate_57600:
            l_u32Baudrate = 57600;
            break;
        case baud_rate_38400:
            l_u32Baudrate = 38400;
            break;
        case baud_rate_19200:
            l_u32Baudrate = 19200;
            break;
        case baud_rate_9600:
            l_u32Baudrate = 9600;
            break;
        default:
            l_u32Baudrate = 9600;
            break;
    }

    return l_u32Baudrate;
}

