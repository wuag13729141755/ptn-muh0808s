#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/MHD0404S/mapp_UartCmdList_MHD0404S.h"


extern const stUartDeclare_t    stProjectUartWorkTable[];
static eCmdFun_t cmdfunctionID = eFunc_NoUse;
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
bool StartEdidCostumFlag;
u8 g_u8UserEDIDIndex;
bool g_bInternalEdidFlag;
#endif

BYTE g_u8MultiCmdDelayCounter = 0;
u8 g_u8Fwversion[2];    // fpga_tx fpga_rx

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
    pCommVar->SendPrintf("fpga tx v%d.%d.%d\r\n",(g_u8Fwversion[0]>>5)&0x07,(g_u8Fwversion[0]>>3)&0x02,(g_u8Fwversion[0])&0x07);
    pCommVar->SendPrintf("fpga rx v%d.%d.%d\r\n",(g_u8Fwversion[1]>>5)&0x07,(g_u8Fwversion[1]>>3)&0x02,(g_u8Fwversion[1])&0x07);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    u8 l_u8index;

    for(l_u8index=0;l_u8index<Def_outport_num;l_u8index++)
    {
        pstHdmiSw->u8SwChannel[l_u8index] = l_u8index;
    }

    SetUserData_HdmiSel();
    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();
    MHD0404S_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();

    mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,1000);
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
//    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
//    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i=0,j=0,l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8Output[Def_outport_num],l_s8Temp;
    bool l_bUpdateFlag = FALSE;

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
//            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = l_s8Input-1;
            j++;
            pCommVar->SendPrintf("out %d in %d\r\n",i+1,l_s8Input);
            l_bUpdateFlag = TRUE;
            mapp_MHD0404SWincfgNoiseMute(TRUE);
//            mapp_MHD0404SSourceChange(i+1,l_s8Input);
            mapp_MHD0404SSplitModeChannelChange(i,l_s8Input-1);
        }
    }
//    SetUserData_HdmiSel();
    if(l_bUpdateFlag)
    {
        mapp_MHD0404SFpgaWinCmdUpdate(10);
        mapp_MHD0404SWincfgNoiseMute(FALSE);
    }
    mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,1000);

    return l_state;

}

//eFunc_GetInputStutus,
static emCmdFuncStaType mapp_CmdfuncGetInputStutus(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i;
    for(i=0;i<Def_outport_num;i++)
    {
        pCommVar->SendPrintf("out %d in %d\r\n",i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
    }

    return l_state;
}
//eFunc_GetOneInputStutus
static emCmdFuncStaType mapp_CmdfuncGetOneInputStutus(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i;
    char *l_pCmd = pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Output<=Def_outport_num)
    {
        if(l_s8Output==0)
        {
            for(i=0;i<Def_outport_num;i++)
            {
                pCommVar->SendPrintf("out %d in %d\r\n",i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
            }
        }
        else
        {
            pCommVar->SendPrintf("out %d in %d\r\n",l_s8Output,pstPrjSaveVar->stHdmiSel.u8SwChannel[l_s8Output-1]+1);
        }
    }
    else
        l_state = emCmdOutOfRange;
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
    UNUSED(pCommVar);
    pCommVar->SendPrintf("test version %s.%04d%02d%02d%02d%02d%02d\r\n",_STRING_FW_VER_NUM,
                                        g_stTestVersion.u16Year,
                                        g_stTestVersion.u8Month,
                                        g_stTestVersion.u8Date,
                                        g_stTestVersion.u8Hour,
                                        g_stTestVersion.u8minute,
                                        g_stTestVersion.u8Second);
    {
        static u8 l_u8ID=1;
        mapp_GetFpgaVersion(l_u8ID);
        if(l_u8ID==0xFE)
            l_u8ID = 1;
        else if(l_u8ID == 4)
            l_u8ID= 0xFE;
        else
            l_u8ID++;
    }

    return emCmdSucess;
}

//eFunc_SetRxEdidMode,
static emCmdFuncStaType mapp_CmdfuncSetRxEdidMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8EdidID;
    u8 l_u8EdidBuf[256];

    memset(l_u8EdidBuf,0,256);
    l_s8Input = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8EdidID = (char)GetNumofStr(l_pCmd,')',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
    if(l_s8EdidID>0&&l_s8EdidID<EDID_MODE_MAX&&l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        switch(l_s8EdidID)
        {
            case EDID_MODE_INTERNAL1_1024x768_60:
                memcpy(l_u8EdidBuf,_1_1024x768p60,256);
                break;
            case EDID_MODE_INTERNAL2_1280x720_60:
                memcpy(l_u8EdidBuf,_2_1280x720p60,256);
                break;
            case EDID_MODE_INTERNAL3_1360x768_60:
                memcpy(l_u8EdidBuf,_3_1360x768p60,256);
                break;
            case EDID_MODE_INTERNAL4_1920x1200_60:
                memcpy(l_u8EdidBuf,_4_1920x1200p60,256);
                break;
            case EDID_MODE_INTERNAL5_1600x1200_60:
                memcpy(l_u8EdidBuf,_5_1600x1200p60,256);
                break;
            case EDID_MODE_INTERNAL6_1920x1080_60:
                memcpy(l_u8EdidBuf,_6_1920x1080p60,256);
                break;
            case EDID_MODE_USER1:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre1.pu8EdidContext,256);
                break;
            case EDID_MODE_USER2:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre2.pu8EdidContext,256);
                break;
            case EDID_MODE_USER3:
                memcpy(&l_u8EdidBuf,pstSpecVar->stCustomEdidPre3.pu8EdidContext,256);
                break;
            case EDID_MODE_USER4:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre4.pu8EdidContext,256);
                break;
            case EDID_MODE_COPYTX1:
            case EDID_MODE_COPYTX2:
            case EDID_MODE_COPYTX3:
            case EDID_MODE_COPYTX4:
                memcpy(l_u8EdidBuf,g_stTxEdidVar[l_s8EdidID-EDID_MODE_COPYTX1].u8EdidRecv,256);
                break;
            default:
                break;
        }
    }
    else
        l_state = emCmdOutOfRange;

    if(l_state == emCmdSucess)
    {
        if(l_s8Input==0)
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[i] = l_s8EdidID;
                pCommVar->SendPrintf("in %d edid %d\r\n",i+1,l_s8EdidID);
                it66021EdidChange(i,l_u8EdidBuf);
            }
            mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,4000);
        }
        else
        {
            it66021EdidChange(l_s8Input-1,l_u8EdidBuf);
            pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_s8Input-1] = l_s8EdidID;
            pCommVar->SendPrintf("in %d edid %d\r\n",l_s8Input,l_s8EdidID);
            mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,2000);
        }
        SetUserData_EdidIndexSave();
    }
    return l_state;

}


//eFunc_GetRxEdidMode,
static emCmdFuncStaType mapp_CmdfuncGetRxEdidMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input;

    l_s8Input = (char)GetNumofStr(l_pCmd,')',3);

    if(l_s8Input == 0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            pCommVar->SendPrintf("in %d edid %d\r\n",i+1,pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[i]);
        }
    }
    else if(l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        pCommVar->SendPrintf("in %d edid %d\r\n",l_s8Input,pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_s8Input-1]);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_UpdateUserEdid,
static emCmdFuncStaType mapp_CmdfuncUpdateUserEdid(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8UserEDID;

    l_s8UserEDID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8UserEDID>0&&l_s8UserEDID<=4)
    {
        pCommVar->SendPrintf("please send edid file in 15s\r\n");
        PlatformTimerSet(etUserEdidReceiveTime,15000);
        #if _ENABLE_USER_EDID_LEARNING_FUNCTION
        StartEdidCostumFlag = TRUE;
        g_u8UserEDIDIndex = l_s8UserEDID-1;
        #endif
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstUartCom_t pUartCommVar = &g_stUart5CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
        pUartCommVar->SetBaudRate(mapp_GetRealBaudrate(l_s8BuadID), DEF_UART_DATA_CONFIG);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetCtrlBaudrate,
static emCmdFuncStaType mapp_CmdfuncGetCtrlBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n","in baudrate", mapp_GetRealBaudrate(pstBasicVar->u8InRs232BaudrateID));
    return l_state;
}
//eFunc_SetOutBaudrate,
static emCmdFuncStaType mapp_CmdfuncSetOutBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstUartCom_t pUartCommVar = &g_stUart4CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8BuadID;

    l_s8BuadID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8BuadID>=1&&l_s8BuadID<=5)
    {
        pstBasicVar->u8OutRs232BaudrateID = l_s8BuadID;
        pCommVar->SendPrintf("%s %d\r\n","out baudrate", mapp_GetRealBaudrate(l_s8BuadID));
        pCommVar->SendPrintf("ok\r\n");
        SetUserData_SpecBasicVar();

        delay_ms(10);
        pUartCommVar->SetBaudRate(mapp_GetRealBaudrate(l_s8BuadID), DEF_UART_DATA_CONFIG);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetOutBaudrate,
static emCmdFuncStaType mapp_CmdfuncGetOutBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n","out baudrate", mapp_GetRealBaudrate(pstBasicVar->u8OutRs232BaudrateID));
    return l_state;
}
//eFunc_SetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncSetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock?"lock":"unlock");
    return l_state;
}

//eFunc_SetTxResolution,
static emCmdFuncStaType mapp_CmdfuncSetTxResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output,l_s8ResID;
    char l_s8ResText[20];

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8ResID = (char)GetNumofStr(l_pCmd,')',3);

    memset(l_s8ResText,0,20);
    if((l_s8Output<=Def_outport_num)&&(l_s8ResID>=1&&l_s8ResID<=11))
    {
        GetResolutionText(l_s8ResText,l_s8ResID-1);
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                mapp_MHD0404SResolutionConfig(i,l_s8ResID-1,(i==(Def_outport_num-1))?TRUE:FALSE);
                pstBasicVar->u8TxResolutionID[i] = l_s8ResID-1;
                pCommVar->SendPrintf("out %d res %s\r\n",i+1,l_s8ResText);
            }
        }
        else
        {
            mapp_MHD0404SResolutionConfig(l_s8Output-1,l_s8ResID-1,TRUE);
            pstBasicVar->u8TxResolutionID[l_s8Output-1] = l_s8ResID-1;
            pCommVar->SendPrintf("out %d res %s\r\n",l_s8Output,l_s8ResText);
        }
        //mapp_MHD0404S_TimingConfig(l_s8Output,l_s8ResID-1);
        mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,1000);
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetTxResolution,
static emCmdFuncStaType mapp_CmdfuncGetTxResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;
    char l_s8ResText[20];


    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Output<=Def_outport_num)
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                memset(l_s8ResText,0,20);
                GetResolutionText(l_s8ResText,pstBasicVar->u8TxResolutionID[i]);
                pCommVar->SendPrintf("out %d res %s\r\n",i+1,l_s8ResText);
            }
        }
        else
        {
            memset(l_s8ResText,0,20);
            GetResolutionText(l_s8ResText,pstBasicVar->u8TxResolutionID[l_s8Output-1]);
            pCommVar->SendPrintf("out %d res %s\r\n",l_s8Output,l_s8ResText);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetRxResolution,
static emCmdFuncStaType mapp_CmdfuncGetRxResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input;

    l_s8Input = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        if(l_s8Input == 0)
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                pCommVar->SendPrintf("in %d res %dx%d%s@%dHz\r\n",i+1,g_stRxResTable[i].u16Width,
                                                                      g_stRxResTable[i].u8ScanMode?(g_stRxResTable[i].u16Height*2):(g_stRxResTable[i].u16Height),
                                                                      g_stRxResTable[i].u8ScanMode?"I":"",
                                                                      g_stRxResTable[i].u16VFreq);
            }
        }
        else
        {
            pCommVar->SendPrintf("in %d res %dx%d%s@%dHz\r\n",l_s8Input,g_stRxResTable[l_s8Input-1].u16Width,
                                                                      g_stRxResTable[l_s8Input-1].u8ScanMode?(g_stRxResTable[l_s8Input-1].u16Height*2):(g_stRxResTable[l_s8Input-1].u16Height),
                                                                      g_stRxResTable[l_s8Input-1].u8ScanMode?"I":"",
                                                                      g_stRxResTable[l_s8Input-1].u16VFreq);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_SetTxHdcpMode,
static emCmdFuncStaType mapp_CmdfuncSetTxHdcpMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8HdcpMode;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8HdcpMode = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s8HdcpMode>=0&&l_s8HdcpMode<=1))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8TxHdcpMode[i] = l_s8HdcpMode;
                pCommVar->SendPrintf("out %d hdcp %s\r\n",i+1,l_s8HdcpMode?"on":"off");

                GsvSetTxHdcpStatus(i,pstBasicVar->u8TxHdcpMode[i]);
            }
        }
        else
        {
            pstBasicVar->u8TxHdcpMode[l_s8Output-1] = l_s8HdcpMode;
            pCommVar->SendPrintf("out %d hdcp %s\r\n",l_s8Output,l_s8HdcpMode?"on":"off");

            GsvSetTxHdcpStatus(l_s8Output-1,pstBasicVar->u8TxHdcpMode[l_s8Output-1]);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetTxHdcpMode,
static emCmdFuncStaType mapp_CmdfuncGetTxHdcpMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Output<=Def_outport_num)
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pCommVar->SendPrintf("out %d hdcp %s\r\n",i+1,pstBasicVar->u8TxHdcpMode[i]?"on":"off");
            }
        }
        else
        {
            pCommVar->SendPrintf("out %d hdcp %s\r\n",l_s8Output,pstBasicVar->u8TxHdcpMode[l_s8Output-1]?"on":"off");
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetTxFormat,
static emCmdFuncStaType mapp_CmdfuncSetTxFormat(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8TxFormat;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8TxFormat = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s8TxFormat>=0&&l_s8TxFormat<=1))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8TxFormatSta[i] = l_s8TxFormat;
                pCommVar->SendPrintf("output hdmi %d format is %s\r\n",i+1,l_s8TxFormat?"dvi":"hdmi");
                GsvSetTxMode(i,pstBasicVar->u8TxFormatSta[i]?0:1);
            }
        }
        else
        {
            pstBasicVar->u8TxFormatSta[l_s8Output-1] = l_s8TxFormat;
            pCommVar->SendPrintf("output hdmi %d format is %s\r\n",l_s8Output,l_s8TxFormat?"dvi":"hdmi");
            GsvSetTxMode(l_s8Output-1,pstBasicVar->u8TxFormatSta[l_s8Output-1]?0:1);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetTxFormat,
static emCmdFuncStaType mapp_CmdfuncGetTxFormat(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Output<=Def_outport_num)
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pCommVar->SendPrintf("output hdmi %d format is %s\r\n",i+1,pstBasicVar->u8TxFormatSta[i]?"dvi":"hdmi");
            }
        }
        else
        {
            pCommVar->SendPrintf("output hdmi %d format is %s\r\n",l_s8Output,pstBasicVar->u8TxFormatSta[l_s8Output-1]?"dvi":"hdmi");
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetTxExtAudio,
static emCmdFuncStaType mapp_CmdfuncSetTxExtAudio(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8ADMute;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8ADMute = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=2)&&(l_s8ADMute>=0&&l_s8ADMute<=1))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<2;i++)
            {
                pstBasicVar->u8ExternalAudioMute[i] = l_s8ADMute?0:1;
                pCommVar->SendPrintf("external audio %d is %s\r\n",i+1,pstBasicVar->u8ExternalAudioMute[i]?"off":"on");
                mapp_MHD0404S_SetAudioSource(Def_outport_num+i,pstBasicVar->u8ExternalAudioMute[i]?0:pstPrjSaveVar->stHdmiSel.u8SwChannel[Def_outport_num-2+i]);
            }
        }
        else
        {
            pstBasicVar->u8ExternalAudioMute[l_s8Output-1] = l_s8ADMute?0:1;;
            pCommVar->SendPrintf("external audio %d is %s\r\n",l_s8Output,pstBasicVar->u8ExternalAudioMute[l_s8Output-1]?"off":"on");
            mapp_MHD0404S_SetAudioSource(Def_outport_num+l_s8Output-1,pstBasicVar->u8ExternalAudioMute[l_s8Output-1]?0:pstPrjSaveVar->stHdmiSel.u8SwChannel[Def_outport_num-2+l_s8Output-1]);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetTxExtAudio,
static emCmdFuncStaType mapp_CmdfuncGetTxExtAudio(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Output<=2)
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<2;i++)
            {
                pCommVar->SendPrintf("external audio %d is %s\r\n",i+1,pstBasicVar->u8ExternalAudioMute[i]?"off":"on");
            }
        }
        else
        {
            pCommVar->SendPrintf("external audio %d is %s\r\n",l_s8Output,pstBasicVar->u8ExternalAudioMute[l_s8Output-1]?"off":"on");
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SaveScene,
static emCmdFuncStaType mapp_CmdfuncSaveScene(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=1)
    {
        pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag = TRUE;
        pstBasicVar->stScene[l_s8SceneID-1].u8SplitMode = pstBasicVar->u8SplitMode;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
            pstBasicVar->u8SplitMode = pstBasicVar->stScene[l_s8SceneID-1].u8SplitMode;

            for(i=0;i<Def_outport_num;i++)
            {
                mapp_MHD0404SWincfgNoiseMute(TRUE);
//                mapp_MHD0404SSourceChange(i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
                mapp_MHD0404SSplitModeChannelChange(i,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]);
            }
            SetUserData_HdmiSel();

            mapp_MHD0404SFpgaWinCmdUpdate(10);
            mapp_MHD0404SWincfgNoiseMute(FALSE);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s\r\n",pstPrjSaveVar->stPrjNameSet.sPrjName);
    pCommVar->SendPrintf("system %s\r\n",pstBasicVar->bPowerOnSta?"on":"off");
    pCommVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock?"lock":"unlock");

    UNUSED(cmdnum);
    return l_state;
}
//eFunc_GetLinkInfo,
static emCmdFuncStaType mapp_CmdfuncGetLinkInfo(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;

    pCommVar->SendPrintf("ch  1 2 3 4\r\n");
    pCommVar->SendPrintf("in  %c %c %c %c\r\n",
                    g_stLinkSta.u8InLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[3]?'Y':'N'
                    );
    pCommVar->SendPrintf("out %c %c %c %c\r\n",
                    g_stLinkSta.u8OutLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[3]?'Y':'N'
                    );

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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
//eFunc_SetTxAudioSource,
static emCmdFuncStaType mapp_CmdfuncSetTxAudioSource(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8Src;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8Src = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s8Src>=1&&l_s8Src<=18))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8AudioSource[i] = l_s8Src;
                pCommVar->SendPrintf("output %d audio in %d\r\n",i+1,l_s8Src);
                mapp_MHD0404S_SetAudioSource(i+1,l_s8Src);
            }
        }
        else
        {
            pstBasicVar->u8AudioSource[l_s8Output-1] = l_s8Src;
            pCommVar->SendPrintf("output %d audio in %d\r\n",l_s8Output,l_s8Src);
            mapp_MHD0404S_SetAudioSource(l_s8Output,l_s8Src);
        }

        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetTxAudioSource,
static emCmdFuncStaType mapp_CmdfuncGetTxAudioSource(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Output<=Def_outport_num)
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pCommVar->SendPrintf("output %d audio in %d\r\n",i+1,pstBasicVar->u8AudioSource[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("output %d audio in %d\r\n",l_s8Output,pstBasicVar->u8AudioSource[l_s8Output-1]);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetDebugEn
static emCmdFuncStaType mapp_CmdfuncSetDebugEn(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    if((l_s8ID>0)&&(l_s8ID<=1))
    {
        UpdateFunction(eUpdID_MCU,0,NULL,0);
        pCommVar->SendPrintf("please choose update file\r\n");
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_UpdateFpga,
static emCmdFuncStaType mapp_CmdfuncUpdateFpga(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8ID;

    l_s8ID = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8ID>0)&&(l_s8ID<=2))
    {
        if(l_s8ID==1)
            l_s8ID = eUpdID_RXFPGA;
        else
            l_s8ID = eUpdID_TXFPGA;
        pCommVar->SendPrintf("please choose update file\r\n");
        UpdateFunction(l_s8ID,0,NULL,0);

    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetRs232Mode,
static emCmdFuncStaType mapp_CmdfuncSetRs232Mode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Mode;

    l_s8Mode = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Mode>=1)&&(l_s8Mode<=2))
    {
        if(l_s8Mode==1)
            pCommVar->SendPrintf("rs232 mode: in&out control local\r\n");
        else
            pCommVar->SendPrintf("rs232 mode: in control local&out\r\n");
        pstBasicVar->u8UartMode = l_s8Mode-1;
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetRs232Mode,
static emCmdFuncStaType mapp_CmdfuncGetRs232Mode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->u8UartMode==1)
        pCommVar->SendPrintf("rs232 mode: in&out control local\r\n");
    else
        pCommVar->SendPrintf("rs232 mode: in control local&out\r\n");
    return l_state;
}

//eFunc_SetSplitMode,
static emCmdFuncStaType mapp_CmdfuncSetSplitMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Mode;
    u8 i;

    l_s8Mode = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Mode<eSpMode_Max)
    {
        switch(l_s8Mode)
        {
            default:
            case eSpMode_None:
                pCommVar->SendPrintf("split mode: off\r\n");
                break;
            case eSpMode_1x2:
                pCommVar->SendPrintf("split mode: 1x2\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
            case eSpMode_1x3:
                pCommVar->SendPrintf("split mode: 1x3\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
            case eSpMode_1x4:
                pCommVar->SendPrintf("split mode: 1x4\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[3] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
            case eSpMode_2x2:
                pCommVar->SendPrintf("split mode: 2x2\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[3] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
            case eSpMode_2x1:
                pCommVar->SendPrintf("split mode: 2x1\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
            case eSpMode_3x1:
                pCommVar->SendPrintf("split mode: 3x1\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
            case eSpMode_4x1:
                pCommVar->SendPrintf("split mode: 4x1\r\n");
                pstPrjSaveVar->stHdmiSel.u8SwChannel[1] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[2] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                pstPrjSaveVar->stHdmiSel.u8SwChannel[3] = pstPrjSaveVar->stHdmiSel.u8SwChannel[0];
                break;
        }
        pstBasicVar->u8SplitMode = l_s8Mode;
        for(i=0;i<Def_outport_num;i++)
        {
            mapp_MHD0404SWincfgNoiseMute(TRUE);
//            mapp_MHD0404SSourceChange(i+1,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]+1);
            mapp_MHD0404SSplitModeChannelChange(i,pstPrjSaveVar->stHdmiSel.u8SwChannel[i]);
        }
        mapp_MHD0404SFpgaWinCmdUpdate(10);
        mapp_MHD0404SWincfgNoiseMute(FALSE);
        SetUserData_SpecBasicVar();
        SetUserData_HdmiSel();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetSplitMode,
static emCmdFuncStaType mapp_CmdfuncGetSplitMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    switch(pstBasicVar->u8SplitMode)
    {
        default:
        case eSpMode_None:
            pCommVar->SendPrintf("split mode: off\r\n");
            break;
        case eSpMode_1x2:
            pCommVar->SendPrintf("split mode: 1x2\r\n");
            break;
        case eSpMode_1x3:
            pCommVar->SendPrintf("split mode: 1x3\r\n");
            break;
        case eSpMode_1x4:
            pCommVar->SendPrintf("split mode: 1x4\r\n");
            break;
        case eSpMode_2x2:
            pCommVar->SendPrintf("split mode: 2x2\r\n");
            break;
        case eSpMode_2x1:
            pCommVar->SendPrintf("split mode: 2x1\r\n");
            break;
        case eSpMode_3x1:
            pCommVar->SendPrintf("split mode: 3x1\r\n");
            break;
        case eSpMode_4x1:
            pCommVar->SendPrintf("split mode: 4x1\r\n");
            break;
    }
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
        case eFunc_GetInputStutus:
            {
                l_bStatus = mapp_CmdfuncGetInputStutus(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOneInputStutus:
            {
                l_bStatus = mapp_CmdfuncGetOneInputStutus(pCommVar,cmdnum);
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
        case eFunc_SetRxEdidMode:
            {
                l_bStatus = mapp_CmdfuncSetRxEdidMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetRxEdidMode:
            {
                l_bStatus = mapp_CmdfuncGetRxEdidMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_UpdateUserEdid:
            {
                l_bStatus = mapp_CmdfuncUpdateUserEdid(pCommVar,cmdnum);
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
        case eFunc_SetCtrlBaudrate:
            {
                l_bStatus = mapp_CmdfuncSetCtrlBaudrate(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetCtrlBaudrate:
            {
                l_bStatus = mapp_CmdfuncGetCtrlBaudrate(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetOutBaudrate:
            {
                l_bStatus = mapp_CmdfuncSetOutBaudrate(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOutBaudrate:
            {
                l_bStatus = mapp_CmdfuncGetOutBaudrate(pCommVar,cmdnum);
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
        case eFunc_SetTxResolution:
            {
                l_bStatus = mapp_CmdfuncSetTxResolution(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxResolution:
            {
                l_bStatus = mapp_CmdfuncGetTxResolution(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetRxResolution:
            {
                l_bStatus = mapp_CmdfuncGetRxResolution(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetTxHdcpMode:
            {
                l_bStatus = mapp_CmdfuncSetTxHdcpMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxHdcpMode:
            {
                l_bStatus = mapp_CmdfuncGetTxHdcpMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetTxFormat:
            {
                l_bStatus = mapp_CmdfuncSetTxFormat(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxFormat:
            {
                l_bStatus = mapp_CmdfuncGetTxFormat(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetTxExtAudio:
            {
                l_bStatus = mapp_CmdfuncSetTxExtAudio(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxExtAudio:
            {
                l_bStatus = mapp_CmdfuncGetTxExtAudio(pCommVar,cmdnum);
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
        case eFunc_GetLinkInfo:
            {
                l_bStatus = mapp_CmdfuncGetLinkInfo(pCommVar,cmdnum);
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
        case eFunc_SetTxAudioSource:
            {
                l_bStatus = mapp_CmdfuncSetTxAudioSource(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxAudioSource:
            {
                l_bStatus = mapp_CmdfuncGetTxAudioSource(pCommVar,cmdnum);
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
        case eFunc_UpdateFpga:
            {
                l_bStatus = mapp_CmdfuncUpdateFpga(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetRs232Mode:
            {
                l_bStatus = mapp_CmdfuncSetRs232Mode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetRs232Mode:
            {
                l_bStatus = mapp_CmdfuncGetRs232Mode(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplitMode:
            {
                l_bStatus = mapp_CmdfuncSetSplitMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetSplitMode:
            {
                l_bStatus = mapp_CmdfuncGetSplitMode(pCommVar,cmdnum);
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
        pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
        pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
        pstUartCom_t                pCom4Var = &g_stUart4CommVar;
        pstUartCom_t                pCom5Var = &g_stUart5CommVar;
        pstUartCom_t                pCom6Var = &g_stUart6CommVar;

        if(pstBasicVar->u8UartMode==0)
            pCommVar->SendPrintf("err\r\n");
        else
        {
            if((pCommVar==pCom5Var)||(pCommVar==pCom6Var))
                pCom4Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
            else if(pCommVar==pCom4Var)
                pCom5Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
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
        pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
        pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
        pstUartCom_t                pCom4Var = &g_stUart4CommVar;
        pstUartCom_t                pCom5Var = &g_stUart5CommVar;
        pstUartCom_t                pCom6Var = &g_stUart6CommVar;

        if(pstBasicVar->u8UartMode==0)
            pCommVar->SendPrintf("err\r\n");
        else
        {
            if((pCommVar==pCom5Var)||(pCommVar==pCom6Var))
                pCom4Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
            else if(pCommVar==pCom4Var)
                pCom5Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
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
        pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD0404SSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
            for(i=0;i<Def_outport_num;i++)
            {
                mapp_GsvSetStandby(i,FALSE);
            }
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
            for(i=0;i<Def_outport_num;i++)
            {
                mapp_GsvSetStandby(i,TRUE);
            }
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

void mapp_Usart2SystemCmdFun(pstUartCom_t pCommVar)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u8 l_u8ID=0;
    if((pCommVar->pRecBuffer[0]==0xFF)&&(pCommVar->pRecBuffer[1]==0xFF)&&(pCommVar->pRecBuffer[2]==0xA5))
    {
        l_u8Addr = pCommVar->pRecBuffer[3];
        l_u8Cmd = pCommVar->pRecBuffer[4];

        dbg_printf("addr[0x%x],cmd[0x%x]\r\n",l_u8Addr,l_u8Cmd);
        switch(l_u8Cmd)
        {
            case eFpgaRegGetVersion:
                {
                    if(l_u8Addr==0x01)
                    {
                        l_u8ID = 0;
                    }
                    else
                    {
                        l_u8ID = 1;
                    }
                    g_u8Fwversion[l_u8ID] = pCommVar->pRecBuffer[7];
                    dbg_printf("ver[0x%x]\r\n",g_u8Fwversion[l_u8ID]);
                }
                break;
            default:
                break;
        }
    }
}

