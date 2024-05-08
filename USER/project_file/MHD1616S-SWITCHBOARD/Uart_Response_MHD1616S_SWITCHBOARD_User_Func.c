#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/MHD1616S-SWITCHBOARD/mapp_UartCmdList_MHD1616S_SWITCHBOARD.h"


extern const stUartDeclare_t    stProjectUartWorkTable[];
static eCmdFun_t cmdfunctionID = eFunc_NoUse;
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
bool StartEdidCostumFlag;
u8 g_u8UserEDIDIndex;
bool g_bInternalEdidFlag;
#endif

BYTE g_u8MultiCmdDelayCounter = 0;
u8 g_u8Fwversion[6];    // mcu_tx fpga_tx mcu_rx1 fpga_rx1 mcu_rx2 fpga_rx2

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
    pCommVar->SendPrintf("mcu tx v%d.%d.%d\r\n",(g_u8Fwversion[emCardType_Tx*2]>>5)&0x07,(g_u8Fwversion[emCardType_Tx*2]>>3)&0x02,(g_u8Fwversion[emCardType_Tx*2])&0x07);
    pCommVar->SendPrintf("fpga tx v%d.%d.%d\r\n",(g_u8Fwversion[emCardType_Tx*2+1]>>5)&0x07,(g_u8Fwversion[emCardType_Tx*2+1]>>3)&0x02,(g_u8Fwversion[emCardType_Tx*2+1])&0x07);
    pCommVar->SendPrintf("mcu rx v%d.%d.%d\r\n",(g_u8Fwversion[emCardType_Rx1*2]>>5)&0x07,(g_u8Fwversion[emCardType_Rx1*2]>>3)&0x02,(g_u8Fwversion[emCardType_Rx1*2])&0x07);
    pCommVar->SendPrintf("fpga rx v%d.%d.%d\r\n",(g_u8Fwversion[emCardType_Rx1*2+1]>>5)&0x07,(g_u8Fwversion[emCardType_Rx1*2+1]>>3)&0x02,(g_u8Fwversion[emCardType_Rx1*2+1])&0x07);
    pCommVar->SendPrintf("HW ver %s\r\n", _STRING_HW_VER_NUM);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    u8 l_u8SendBuf[1];
    u8 l_u8index;

    for(l_u8index=0;l_u8index<Def_outport_num;l_u8index++)
    {
        pstHdmiSw->u8SwChannel[l_u8index] = l_u8index;
    }

    SetUserData_HdmiSel();
    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();
    MHD1616SSWBD_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();

    l_u8SendBuf[0] = 1;

    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_ResetFactory,l_u8SendBuf,1);
    delay_ms(50);
    mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_SetRxPortEdid,l_u8SendBuf,1);
    delay_ms(50);
    mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_SetRxPortEdid,l_u8SendBuf,1);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,1000);
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
            g_stSwitchTable.u8OutCh[i] = l_s8Input-1;
            g_stSwitchTable.bUpdateFlag[i] = TRUE;
            g_stSwitchTable.bCloseEnableFlag[i] = FALSE;
            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = l_s8Input-1;
            j++;
            pCommVar->SendPrintf("out %d in %d\r\n",i+1,l_s8Input);
        }
    }
    SetUserData_HdmiSel();

    mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_SendInputChannel,pstPrjSaveVar->stHdmiSel.u8SwChannel,Def_outport_num);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
    mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,1000);

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
    pCommVar->SendPrintf("test version %s.%04d%02d%02d%02d%02d%02d\r\n",_STRING_FW_VER_NUM,
                                        g_stTestVersion.u16Year,
                                        g_stTestVersion.u8Month,
                                        g_stTestVersion.u8Date,
                                        g_stTestVersion.u8Hour,
                                        g_stTestVersion.u8minute,
                                        g_stTestVersion.u8Second);

    return emCmdSucess;
}

//eFunc_SetRxEdidMode,
static emCmdFuncStaType mapp_CmdfuncSetRxEdidMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8EdidID;
    u8 l_u8SendBuf[257];

    memset(l_u8SendBuf,0,257);
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
                memcpy(&l_u8SendBuf[1],_1_1024x768p60,256);
                break;
            case EDID_MODE_INTERNAL2_1280x720_60:
                memcpy(&l_u8SendBuf[1],_2_1280x720p60,256);
                break;
            case EDID_MODE_INTERNAL3_1360x768_60:
                memcpy(&l_u8SendBuf[1],_3_1360x768p60,256);
                break;
            case EDID_MODE_INTERNAL4_1920x1200_60:
                memcpy(&l_u8SendBuf[1],_4_1920x1200p60,256);
                break;
            case EDID_MODE_INTERNAL5_1600x1200_60:
                memcpy(&l_u8SendBuf[1],_5_1600x1200p60,256);
                break;
            case EDID_MODE_INTERNAL6_1920x1080_60:
                memcpy(&l_u8SendBuf[1],_6_1920x1080p60,256);
                break;
            case EDID_MODE_USER1:
                memcpy(&l_u8SendBuf[1],pstSpecVar->stCustomEdidPre1.pu8EdidContext,256);
                break;
            case EDID_MODE_USER2:
                memcpy(&l_u8SendBuf[1],pstSpecVar->stCustomEdidPre2.pu8EdidContext,256);
                break;
            case EDID_MODE_USER3:
                memcpy(&l_u8SendBuf[1],pstSpecVar->stCustomEdidPre3.pu8EdidContext,256);
                break;
            case EDID_MODE_USER4:
                memcpy(&l_u8SendBuf[1],pstSpecVar->stCustomEdidPre4.pu8EdidContext,256);
                break;
            case EDID_MODE_COPYTX1:
            case EDID_MODE_COPYTX2:
            case EDID_MODE_COPYTX3:
            case EDID_MODE_COPYTX4:
            case EDID_MODE_COPYTX5:
            case EDID_MODE_COPYTX6:
            case EDID_MODE_COPYTX7:
            case EDID_MODE_COPYTX8:
            case EDID_MODE_COPYTX9:
            case EDID_MODE_COPYTX10:
            case EDID_MODE_COPYTX11:
            case EDID_MODE_COPYTX12:
            case EDID_MODE_COPYTX13:
            case EDID_MODE_COPYTX14:
            case EDID_MODE_COPYTX15:
            case EDID_MODE_COPYTX16:
                memcpy(&l_u8SendBuf[1],g_stTxEdidVar[l_s8EdidID-EDID_MODE_COPYTX1].u8EdidRecv,256);
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
            }
            l_u8SendBuf[0] = 0;
            mapp_MHD1616SSWBDSendCmdToRXBD(1,eMcuReg_SetRxPortEdid,l_u8SendBuf,257);
            delay_ms(50);
            mapp_MHD1616SSWBDSendCmdToRXBD(2,eMcuReg_SetRxPortEdid,l_u8SendBuf,257);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,4000);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,4000);
        }
        else
        {
            l_u8SendBuf[0] = (l_s8Input-1)%8+1;
            mapp_MHD1616SSWBDSendCmdToRXBD((l_s8Input-1)/8+1,eMcuReg_SetRxPortEdid,l_u8SendBuf,257);
            pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_s8Input-1] = l_s8EdidID;
            pCommVar->SendPrintf("in %d edid %d\r\n",l_s8Input,l_s8EdidID);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,2000);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,2000);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n","in baudrate", mapp_GetRealBaudrate(pstBasicVar->u8InRs232BaudrateID));
    return l_state;
}
//eFunc_SetOutBaudrate,
static emCmdFuncStaType mapp_CmdfuncSetOutBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstUartCom_t pUartCommVar = &g_stUart4CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n","out baudrate", mapp_GetRealBaudrate(pstBasicVar->u8OutRs232BaudrateID));
    return l_state;
}
//eFunc_SetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncSetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock?"lock":"unlock");
    return l_state;
}

//eFunc_SetTxResolution,
static emCmdFuncStaType mapp_CmdfuncSetTxResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output,l_s8ResID;
    char l_s8ResText[20];
    u8 l_u8SendBuf[2];

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
                pstBasicVar->u8TxResolutionID[i] = l_s8ResID-1;
                pCommVar->SendPrintf("out %d res %s\r\n",i+1,l_s8ResText);
            }
        }
        else
        {
            pstBasicVar->u8TxResolutionID[l_s8Output-1] = l_s8ResID-1;
            pCommVar->SendPrintf("out %d res %s\r\n",l_s8Output,l_s8ResText);
        }
//        mapp_MHD1616SSWBD_TimingConfig(l_s8Output,l_s8ResID-1);
//        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,4000);
//        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,4000);
        l_u8SendBuf[0] = l_s8Output;
        l_u8SendBuf[1] = l_s8ResID;
        mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_SendResID,l_u8SendBuf,2);
        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,2000);
        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,2000);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
                if((!g_u8CardType[emCardType_Rx2])&&i>=8)
                    break;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8HdcpMode;
    u8 l_u8SendBuf[3];

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
                l_u8SendBuf[0] = i+1;
                l_u8SendBuf[1] = pstBasicVar->u8TxHdcpMode[i];
                l_u8SendBuf[2] = pstBasicVar->u8TxFormatSta[i];
                mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_HdcpAndFormat,l_u8SendBuf,3);
            }
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,1000);
        }
        else
        {
            pstBasicVar->u8TxHdcpMode[l_s8Output-1] = l_s8HdcpMode;
            pCommVar->SendPrintf("out %d hdcp %s\r\n",l_s8Output,l_s8HdcpMode?"on":"off");

            l_u8SendBuf[0] = l_s8Output;
            l_u8SendBuf[1] = pstBasicVar->u8TxHdcpMode[l_s8Output-1];
            l_u8SendBuf[2] = pstBasicVar->u8TxFormatSta[l_s8Output-1];
            mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_HdcpAndFormat,l_u8SendBuf,3);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
            mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,1000);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8TxFormat;
    u8 l_u8SendBuf[3];

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

                l_u8SendBuf[0] = i+1;
                l_u8SendBuf[1] = pstBasicVar->u8TxHdcpMode[i];
                l_u8SendBuf[2] = pstBasicVar->u8TxFormatSta[i];
                mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_HdcpAndFormat,l_u8SendBuf,3);
            }
        }
        else
        {
            pstBasicVar->u8TxFormatSta[l_s8Output-1] = l_s8TxFormat;
            pCommVar->SendPrintf("output hdmi %d format is %s\r\n",l_s8Output,l_s8TxFormat?"dvi":"hdmi");

            l_u8SendBuf[0] = l_s8Output;
            l_u8SendBuf[1] = pstBasicVar->u8TxHdcpMode[l_s8Output-1];
            l_u8SendBuf[2] = pstBasicVar->u8TxFormatSta[l_s8Output-1];
            mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_HdcpAndFormat,l_u8SendBuf,3);
        }
        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_CardInfo,1000);
        mapp_MHD1616SSWBD_PlatformTimerSet(emUserTimer_TimingChange,1000);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
            }
        }
        else
        {
            pstBasicVar->u8ExternalAudioMute[l_s8Output-1] = l_s8ADMute?0:1;;
            pCommVar->SendPrintf("external audio %d is %s\r\n",l_s8Output,pstBasicVar->u8ExternalAudioMute[l_s8Output-1]?"off":"on");
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
                g_stSwitchTable.u8OutCh[i] = pstPrjSaveVar->stHdmiSel.u8SwChannel[i];
                g_stSwitchTable.bUpdateFlag[i] = TRUE;
                g_stSwitchTable.bCloseEnableFlag[i] = FALSE;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pCommVar->SendPrintf("ch  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16\r\n");
    if(g_u8CardType[emCardType_Rx2])
    {
        pCommVar->SendPrintf("in  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c\r\n",
                        g_stLinkSta.u8InLinkSta[0]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[1]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[2]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[3]?'y':'N',
                        g_stLinkSta.u8InLinkSta[4]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[5]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[6]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[7]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[8]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[9]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[10]?'y':'N',
                        g_stLinkSta.u8InLinkSta[11]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[12]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[13]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[14]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[15]?'Y':'N'
                        );
    }
    else
    {
        pCommVar->SendPrintf("in  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c\r\n",
                        g_stLinkSta.u8InLinkSta[0]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[1]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[2]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[3]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[4]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[5]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[6]?'Y':'N',
                        g_stLinkSta.u8InLinkSta[7]?'Y':'N',
                        '-',
                        '-',
                        '-',
                        '-',
                        '-',
                        '-',
                        '-',
                        '-'
                        );

    }
    pCommVar->SendPrintf("out %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c  %c\r\n",
                    g_stLinkSta.u8OutLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[3]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[4]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[5]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[6]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[7]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[8]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[9]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[10]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[11]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[12]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[13]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[14]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[15]?'Y':'N');

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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
                mapp_MHD1616SSWBD_SetAudioSource(i+1,l_s8Src);
            }
        }
        else
        {
            pstBasicVar->u8AudioSource[l_s8Output-1] = l_s8Src;
            pCommVar->SendPrintf("output %d audio in %d\r\n",l_s8Output,l_s8Src);
            mapp_MHD1616SSWBD_SetAudioSource(l_s8Output,l_s8Src);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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

//eFunc_UpdateFpga,
static emCmdFuncStaType mapp_CmdfuncUpdateFpga(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8ID;

    l_s8ID = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8ID>0)&&(l_s8ID<=8))
    {
        pCommVar->SendPrintf("please choose update file\r\n");

    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetRs232Mode,
static emCmdFuncStaType mapp_CmdfuncSetRs232Mode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->u8UartMode==0)
        pCommVar->SendPrintf("rs232 mode: in&out control local\r\n");
    else
        pCommVar->SendPrintf("rs232 mode: in control local&out\r\n");
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
        pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
        pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
        pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SSWBDSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8SendBuf[1];
    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
            l_u8SendBuf[0]=1;
            mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_SendInputChannel,l_u8SendBuf,1);
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
            l_u8SendBuf[0]=0;
            mapp_MHD1616SSWBDSendCmdToTXBD(1,eMcuReg_SendInputChannel,l_u8SendBuf,1);
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

//eMcuReg_CardType            = 0x01,
u8 mapp_TxFuncCardType(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    dbg_printf("Tx Card type %d\r\n",pCmdBuf[7]);
    g_u8CardType[emCardType_Tx] = pCmdBuf[7];
    return (u8)l_emRtState;
}

//eMcuReg_Version             = 0x02,
u8 mapp_TxFuncGetVersion(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    g_u8Fwversion[emCardType_Tx*2] = pCmdBuf[7];
    g_u8Fwversion[emCardType_Tx*2+1] = pCmdBuf[8];
    dbg_printf("Tx mcu version 0x%x\r\n",pCmdBuf[7]);
    dbg_printf("Tx fpga version 0x%x\r\n",pCmdBuf[8]);

    return (u8)l_emRtState;
}
//eMcuReg_LinkSta             = 0x03,
u8 mapp_TxFuncLinkSta(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 i;
    u16 l_u16TxLinkSta;
    u8 l_u8OutLinkSta[Def_outport_num];

    l_u16TxLinkSta = pCmdBuf[7]|(pCmdBuf[8]<<8);
//    dbg_printf("Tx link sta 0x%04x\r\n",l_u16TxLinkSta);

    for(i=0;i<Def_outport_num;i++)
    {
        if(l_u16TxLinkSta&(1<<i))
            l_u8OutLinkSta[i] = 1;
        else
            l_u8OutLinkSta[i] = 0;

        if(l_u8OutLinkSta[i]!=g_stLinkSta.u8OutLinkSta[i])
        {
            if(l_u8OutLinkSta)
            {
                g_stTxEdidVar[i].u8RecvOk=FALSE;
            }
            g_stLinkSta.u8OutLinkSta[i] = l_u8OutLinkSta[i];
//        dbg_printf("Tx[%d] link sta %d\r\n",i+1,g_stLinkSta.u8OutLinkSta[i]);
        }
    }
    return (u8)l_emRtState;
}
//eMcuReg_HdcpAndFormat       = 0x04,
u8 mapp_TxFuncHdcpAndFormat(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}
//eMcuReg_TxResolution        = 0x05,
u8 mapp_TxFuncTxResolution(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8TxPort = pCmdBuf[7];

    if(l_u8TxPort<=Def_outport_num)
    {
        if(l_u8TxPort==0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                g_stLinkSta.u8ResCfgOk[i] = TRUE;;
            }
        }
        else
        {
            g_stLinkSta.u8ResCfgOk[l_u8TxPort-1] = TRUE;;
        }
    }

    return (u8)l_emRtState;
}
//eMcuReg_TxWinConfig         = 0x06,
u8 mapp_TxFuncTxWinConfig(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8TxPort = pCmdBuf[7];

    if(l_u8TxPort<=Def_outport_num)
    {
        if(l_u8TxPort==0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                g_stLinkSta.u8WinCfgOk[i] = TRUE;;
            }
        }
        else
        {
            g_stLinkSta.u8WinCfgOk[l_u8TxPort-1] = TRUE;;
        }
    }

    return (u8)l_emRtState;
}
//eMcuReg_GetTxPortEdid       = 0x07,
u8 mapp_TxFuncGetTxPortEdid(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
//    u8 l_u8edidbuf[256];
    u8 l_u8TxPort = pCmdBuf[7];

    if(l_u8TxPort>0&&l_u8TxPort<=Def_outport_num)
    {
        memcpy(g_stTxEdidVar[l_u8TxPort-1].u8EdidRecv,&pCmdBuf[8],256);
//        {
//            u16 j;
//            dbg_printf("Tx Prot[%d] Edid:\r\n",l_u8TxPort);
//			for(j=0;j<256;j++)
//			{
//				dbg_printf("%02x ",l_u8edidbuf[j]);
//				if((j+1)%16==0)
//					dbg_printf("\r\n");
//			}
//			dbg_printf("\r\n");
//        }
        g_stTxEdidVar[l_u8TxPort-1].u8RecvOk=TRUE;
    }
    return (u8)l_emRtState;
}
//eMcuReg_ResetFactory        = 0x08,
u8 mapp_TxFuncResetFactory(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    return (u8)l_emRtState;
}
//eMcuReg_SetBritness         = 0x09,
u8 mapp_TxFuncSetBritness(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}

//eMcuReg_SendRxTiming        = 0x0C,
u8 mapp_TxFuncSendRxTiming(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}

//eMcuReg_SendResID           = 0x0D,
u8 mapp_TxFuncSendResID(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}

//eMcuReg_SendInputChannel    = 0x0E,
u8 mapp_TxFuncSendInputChannel(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}

stUartList_t Uart1TxCmdTable[]=
{
    {eMcuReg_CardType,      mapp_TxFuncCardType},
    {eMcuReg_Version,       mapp_TxFuncGetVersion},
    {eMcuReg_LinkSta,       mapp_TxFuncLinkSta},
    {eMcuReg_HdcpAndFormat, mapp_TxFuncHdcpAndFormat},
    {eMcuReg_TxResolution,  mapp_TxFuncTxResolution},
    {eMcuReg_TxWinConfig,   mapp_TxFuncTxWinConfig},
    {eMcuReg_GetTxPortEdid, mapp_TxFuncGetTxPortEdid},
    {eMcuReg_ResetFactory,  mapp_TxFuncResetFactory},
    {eMcuReg_SetBritness,   mapp_TxFuncSetBritness},
    {eMcuReg_SendRxTiming,  mapp_TxFuncSendRxTiming},
    {eMcuReg_SendResID,     mapp_TxFuncSendResID},
    {eMcuReg_SendInputChannel,  mapp_TxFuncSendInputChannel},

    //end
    {0,NULL},
};


//eMcuReg_CardType            = 0x01,
u8 mapp_RxFuncCardType(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8RxDev = pCmdBuf[3];

    dbg_printf("Rxdev[%d] Card type %d\r\n",pCmdBuf[3],pCmdBuf[7]);
    g_u8CardType[l_u8RxDev] = pCmdBuf[7];

    return (u8)l_emRtState;
}

//eMcuReg_Version             = 0x02,
u8 mapp_RxFuncGetVersion(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8RxDev = pCmdBuf[3];

    if(l_u8RxDev>=1&&l_u8RxDev<=2)
    {
        if(l_u8RxDev==1)
        {
            g_u8Fwversion[emCardType_Rx1*2] = pCmdBuf[7];
            g_u8Fwversion[emCardType_Rx1*2+1] = pCmdBuf[8];
        }
        else if(l_u8RxDev==2)
        {
            g_u8Fwversion[emCardType_Rx2*2] = pCmdBuf[7];
            g_u8Fwversion[emCardType_Rx2*2+1] = pCmdBuf[8];
        }
        dbg_printf("Rxdev[%d] mcu version 0x%x\r\n",l_u8RxDev,pCmdBuf[7]);
        dbg_printf("Rxdev[%d] fpga version 0x%x\r\n",l_u8RxDev,pCmdBuf[8]);
    }
    return (u8)l_emRtState;
}
//eMcuReg_LinkSta             = 0x03,
u8 mapp_RxFuncLinkSta(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8RxDev = pCmdBuf[3];
    u8 i;
    u8 l_u8RxLinkSta;
    l_u8RxLinkSta = pCmdBuf[7];
//    dbg_printf("Rxdev[%d] link sta 0x%04x\r\n",l_u8RxDev,l_u8RxLinkSta);

    if(l_u8RxDev>=1&&l_u8RxDev<=2)
    {
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER/2;i++)
        {
            if(l_u8RxLinkSta&(1<<i))
                g_stLinkSta.u8InLinkSta[i+(l_u8RxDev-1)*8] = 1;
            else
                g_stLinkSta.u8InLinkSta[i+(l_u8RxDev-1)*8] = 0;

//            dbg_printf("Rxdev[%d] link sta %d\r\n",i+1+(l_u8RxDev-1)*8,g_stLinkSta.u8InLinkSta[i+(l_u8RxDev-1)*8]);
        }
    }

    return (u8)l_emRtState;
}
//eMcuReg_GetRxResolution         = 0x81,
u8 mapp_RxFuncGetRxResolution(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8RxDev = pCmdBuf[3];
    u16 l_u16TimWidth,l_u16TimHeight;
    u8 l_u8VFreq,l_u8ScanMode;
    u8 i;
    bool l_bSendTimingFlag = FALSE;
    if((l_u8RxDev>=1)&&(l_u8RxDev<=2))
    {
        l_u8RxDev = l_u8RxDev-1;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER/2;i++)
        {
            l_u16TimWidth  = pCmdBuf[7+i*6]|(pCmdBuf[8+i*6]<<8);
            l_u16TimHeight = pCmdBuf[9+i*6]|(pCmdBuf[10+i*6]<<8);
            l_u8VFreq      = pCmdBuf[11+i*6];
            l_u8ScanMode   = pCmdBuf[12+i*6];
            if((l_u16TimWidth!=g_stRxResTable[l_u8RxDev*8+i].u16Width)
                ||(l_u16TimHeight!=g_stRxResTable[l_u8RxDev*8+i].u16Height)
                ||(l_u8VFreq!=g_stRxResTable[l_u8RxDev*8+i].u16VFreq)
                ||(l_u8ScanMode!=g_stRxResTable[l_u8RxDev*8+i].u8ScanMode))
            {
                g_stRxResTable[l_u8RxDev*8+i].u16Width  = l_u16TimWidth;
                g_stRxResTable[l_u8RxDev*8+i].u16Height = l_u16TimHeight;
                g_stRxResTable[l_u8RxDev*8+i].u16VFreq  = l_u8VFreq;
                g_stRxResTable[l_u8RxDev*8+i].u8ScanMode= l_u8ScanMode;
                g_stRxResTable[l_u8RxDev*8+i].bChangeFlag = TRUE;
                l_bSendTimingFlag = TRUE;
            }
//            dbg_printf("RxDev[%d] RxPort[%d] u16Width 0x%x\r\n",l_u8RxDev,l_u8RxDev*8+i,g_stRxResTable[l_u8RxDev*8+i].u16Width);
//            dbg_printf("RxDev[%d] RxPort[%d] u16Height 0x%x\r\n",l_u8RxDev,l_u8RxDev*8+i,g_stRxResTable[l_u8RxDev*8+i].u16Height);
        }
        if(l_bSendTimingFlag)
        {
            //mapp_MHD1616SSWBDSendTimingToTx();
        }
    }
    return (u8)l_emRtState;
}
//eMcuReg_SetRxPortEdid       = 0x82,
u8 mapp_RxFuncSetRxPortEdid(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}


stUartList_t Uart1RxCmdTable[]=
{
    {eMcuReg_CardType,      mapp_RxFuncCardType},
    {eMcuReg_Version,       mapp_RxFuncGetVersion},
    {eMcuReg_LinkSta,       mapp_RxFuncLinkSta},
    {eMcuReg_GetRxResolution,mapp_RxFuncGetRxResolution},
    {eMcuReg_SetRxPortEdid, mapp_RxFuncSetRxPortEdid},

    //end
    {0,NULL},
};

void mapp_Usart1SystemCmdFun(pstUartCom_t pComVar)
{
    emCmdFuncStaType l_bStatus=emCmdError;
    u8 l_u8Addr;
    u8 l_u8Cmd;
    u16 l_u16CmdLen;
    u8 i;
    char *l_ps8CmdBuf = pComVar->pRecBuffer;

    while(*l_ps8CmdBuf)
    {
        if((*(l_ps8CmdBuf)==0xFF)&&(*(l_ps8CmdBuf+1)==0xFF)&&(*(l_ps8CmdBuf+2)==0xA5))
        {
            l_u8Addr = *(l_ps8CmdBuf+3);
            l_u8Cmd = *(l_ps8CmdBuf+4);
            l_u16CmdLen = *(l_ps8CmdBuf+5)|(*(l_ps8CmdBuf+6)<<8);
//            dbg_printf("addr[0x%x],cmd[0x%x] cmdlen[0x%x]\r\n",l_u8Addr,l_u8Cmd,l_u16CmdLen);

            switch(l_u8Addr)
            {
                case 0x81:
                    {
                        for(i=0;i<=255;i++)
                        {
                            if((Uart1TxCmdTable[i].u8Cmd == 0)||(Uart1TxCmdTable[i].pCmdFun == NULL))
                                break;
                            if((l_u8Cmd == Uart1TxCmdTable[i].u8Cmd)&&(Uart1TxCmdTable[i].pCmdFun!=NULL))
                            {
                                l_bStatus = (emCmdFuncStaType)Uart1TxCmdTable[i].pCmdFun(l_ps8CmdBuf,(l_u16CmdLen+7));
                                if(l_bStatus == emCmdError)
                                {
                                }
                                break;
                            }
                        }
                    }
                    break;

                case 0x01:
                case 0x02:
                    {
                        for(i=0;i<=255;i++)
                        {
                            if((Uart1RxCmdTable[i].u8Cmd == 0)||(Uart1RxCmdTable[i].pCmdFun == NULL))
                                break;
                            if((l_u8Cmd == Uart1RxCmdTable[i].u8Cmd)&&(Uart1RxCmdTable[i].pCmdFun!=NULL))
                            {
                                l_bStatus = (emCmdFuncStaType)Uart1RxCmdTable[i].pCmdFun(l_ps8CmdBuf,(l_u16CmdLen+7));
                                if(l_bStatus == emCmdError)
                                {
                                }
                                break;
                            }
                        }
                    }
                    break;
                default:
                    break;
            }

            l_ps8CmdBuf += l_u16CmdLen+7;
        }
        else
        {
            l_ps8CmdBuf++;
        }

        if(l_ps8CmdBuf>=(pComVar->pRecBuffer+pComVar->u16RecCnt))
            break;
    }
}

