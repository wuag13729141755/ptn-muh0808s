#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"
#include "ms1826_app_single_mode.h"

#include "../USER/project_file/MUH0808S/mapp_UartCmdList_MUH0808S.h"

#define BRIGHTNESS_VALUE_MAX    (100)
#define CONTRAST_VALUE_MAX      (100)
#define SATURATION_VALUE_MAX    (100)
#define HUE_VALUE_MAX           (100)

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
bool StartEdidCostumFlag;
u8 g_u8UserEDIDIndex;
bool g_bInternalEdidFlag;
#endif
u8 inputchannl[8]={0};
u8 outputchannl[8]={0};
u8 QueryTheResCnt=0;
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
        case emRes_1440x900_60:
            memcpy(ps8ResText,"1440x900@60Hz\0",strlen("1440x900@60Hz\0"));
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
#if _ENABLE_RES_3840x2160_30
        case emRes_3840x2160_30:
            memcpy(ps8ResText,"3840x2160@30Hz\0",strlen("3840x2160@30Hz\0"));
            break;
#endif
#if _ENABLE_RES_3840x2160_60
        case emRes_3840x2160_60:
            memcpy(ps8ResText,"3840x2160@60Hz\0",strlen("3840x2160@60Hz\0"));
            break;
#endif
#if _ENABLE_RES_3840x2160_50
        case emRes_3840x2160_50:
            memcpy(ps8ResText,"3840x2160@50Hz\0",strlen("3840x2160@50Hz\0"));
            break;
#endif
#if _ENABLE_RES_4096x2160_30
        case emRes_4096x2160_30:
            memcpy(ps8ResText,"4096x2160@30Hz\0",strlen("4096x2160@30Hz\0"));
            break;
#endif
#if _ENABLE_RES_4096x2160_60
        case emRes_4096x2160_60:
            memcpy(ps8ResText,"4096x2160@60Hz\0",strlen("4096x2160@60Hz\0"));
            break;
#endif
#if _ENABLE_RES_4096x2160_50
        case emRes_4096x2160_50:
            memcpy(ps8ResText,"4096x2160@50Hz\0",strlen("4096x2160@50Hz\0"));
            break;
#endif

        default:
            break;
    }

}

char *GetColorText(u8 u8ColorID)
{
    switch(u8ColorID)
    {
        case emOsdColor_White:
            {
                return "white";
            }
        case emOsdColor_Red:
            {
                return "red";
            }
        case emOsdColor_Green:
            {
                return "green";
            }
        case emOsdColor_Blue:
            {
                return "blue";
            }
        case emOsdColor_Black:
            {
                return "black";
            }
        default:
            return " ";
    }

}

//eFunc_GetVersion,
static emCmdFuncStaType mapp_CmdfuncGetVersion(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    pCommVar->SendPrintf("mcu ctrl %s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    mapp_UserData_HdmiSelInit();
    SetUserData_HdmiSel();
    delay_ms(100);
    MUH0808S_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();
    delay_ms(100);

    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();
    delay_ms(500);
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
    SYS_SoftReset();

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
//            g_stMUH0808SSwitchTable.u8OutCh[i] = l_s8Input-1;
//            g_stMUH0808SSwitchTable.bUpdateFlag[i] = TRUE;
//            g_stMUH0808SSwitchTable.bCloseEnableFlag[i] = FALSE;
            app_MUH0808SSourceSwitch(i,l_s8Input-1);
            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = l_s8Input-1;
            j++;
            pCommVar->SendPrintf("out %d in %d\r\n",i+1,l_s8Input);
        }
    }

    SetUserData_HdmiSel();

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

//eFunc_SetTxResolution,
static emCmdFuncStaType mapp_CmdfuncSetTxResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
     pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8ResText[20];
    u8 l_u8NumLen=0;
    signed char l_s8Output,l_s8ResID;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','
    l_s8ResID = (char)GetNumofStr(l_pCmd,')',3);

    memset(l_s8ResText,0,20);
    if((l_s8Output<=Def_outport_num)&&(l_s8ResID>=1&&l_s8ResID<=emRes_max))
    {
        GetResolutionText(l_s8ResText,l_s8ResID-1);
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                app_MS1826TxResolutionChange(l_u8Dev,l_u8RealPort-1,l_s8ResID-1);
                pstBasicVar->u8TxResolutionID[i] = l_s8ResID-1;
                pCommVar->SendPrintf("out %d res %s\r\n",i+1,l_s8ResText);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            app_MS1826TxResolutionChange(l_u8Dev,l_u8RealPort-1,l_s8ResID-1);

            app_MUH0808SSourceSwitch(l_s8Output-1,pstPrjSaveVar->stHdmiSel.u8SwChannel[l_s8Output-1]);
            pstBasicVar->u8TxResolutionID[l_s8Output-1] = l_s8ResID-1;
            pCommVar->SendPrintf("out %d res %s\r\n",l_s8Output,l_s8ResText);
        }
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    stTimingTable_t l_stRxTming;
    u8 l_u8Dev;

    l_s8Input = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        if(l_s8Input == 0)
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                l_u8Dev = cg_u8ChannelTable[i][3];
                app_ms1826_get_rx_timing(l_u8Dev,cg_u8ChannelTable[i][0],&l_stRxTming);
                pCommVar->SendPrintf("in %d res %dx%d%s@%dHz\r\n",i+1,l_stRxTming.u16HActive,
                                                                          l_stRxTming.u8Interlace==INTR?(l_stRxTming.u16VActive*2):(l_stRxTming.u16VActive),
                                                                          l_stRxTming.u8Interlace==INTR?"I":"",
                                                                          l_stRxTming.u8VClk);
            }
        }
        else
        {
            l_u8Dev = cg_u8ChannelTable[l_s8Input-1][3];
            app_ms1826_get_rx_timing(l_u8Dev,cg_u8ChannelTable[l_s8Input-1][0],&l_stRxTming);
            pCommVar->SendPrintf("in %d res %dx%d%s@%dHz\r\n",l_s8Input,l_stRxTming.u16HActive,
                                                                      l_stRxTming.u8Interlace==INTR?(l_stRxTming.u16VActive*2):(l_stRxTming.u16VActive),
                                                                      l_stRxTming.u8Interlace==INTR?"I":"",
                                                                      l_stRxTming.u8VClk);
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8HdcpMode;
    u8 l_u8RealPort;
    u8 l_u8Dev;

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
                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->u8TxHdcpMode[i] = l_s8HdcpMode;
                pCommVar->SendPrintf("out %d hdcp %s\r\n",i+1,l_s8HdcpMode?"on":"off");

                app_MS1826TxHdcpEnable(l_u8Dev,l_u8RealPort-1,pstBasicVar->u8TxHdcpMode[i]);
            }
        }
        else
        {
            pstBasicVar->u8TxHdcpMode[l_s8Output-1] = l_s8HdcpMode;
            pCommVar->SendPrintf("out %d hdcp %s\r\n",l_s8Output,l_s8HdcpMode?"on":"off");
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];

            app_MS1826TxHdcpEnable(l_u8Dev,l_u8RealPort-1,pstBasicVar->u8TxHdcpMode[l_s8Output-1]);
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8TxFormat;
    u8 l_u8RealPort;
    u8 l_u8Dev;

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
                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->u8TxFormatSta[i] = l_s8TxFormat;
                pCommVar->SendPrintf("output hdmi %d format is %s\r\n",i+1,l_s8TxFormat?"hdmi":"dvi");
                app_MS1826TxConfig(l_u8Dev,l_u8RealPort-1,pstBasicVar->u8TxFormatSta[i]);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            pstBasicVar->u8TxFormatSta[l_s8Output-1] = l_s8TxFormat;
            pCommVar->SendPrintf("output hdmi %d format is %s\r\n",l_s8Output,l_s8TxFormat?"hdmi":"dvi");
            app_MS1826TxConfig(l_u8Dev,l_u8RealPort-1,pstBasicVar->u8TxFormatSta[l_s8Output-1]);
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("output hdmi %d format is %s\r\n",i+1,pstBasicVar->u8TxFormatSta[i]?"hdmi":"dvi");
            }
        }
        else
        {
            pCommVar->SendPrintf("output hdmi %d format is %s\r\n",l_s8Output,pstBasicVar->u8TxFormatSta[l_s8Output-1]?"hdmi":"dvi");
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_SetFreezeState,
static emCmdFuncStaType mapp_CmdfuncSetFreezeState(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output,l_s8FreeSta;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8FreeSta = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s8FreeSta>=0&&l_s8FreeSta<=1))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->u8FreezeSta[i] = l_s8FreeSta;
                pCommVar->SendPrintf("output %d freeze %s\r\n",i,l_s8FreeSta?"on":"off");
                app_MS1826FreezeChange(l_u8Dev,l_u8RealPort-1,l_s8FreeSta);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            pstBasicVar->u8FreezeSta[l_s8Output-1] = l_s8FreeSta;
            pCommVar->SendPrintf("output %d freeze %s\r\n",l_s8Output,l_s8FreeSta?"on":"off");
            app_MS1826FreezeChange(l_u8Dev,l_u8RealPort-1,l_s8FreeSta);
        }
        //SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetFreezeState
static emCmdFuncStaType mapp_CmdfuncGetFreezeState(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("output port format is %s\r\n",pstBasicVar->u8TxFormatSta[0]?"dvi":"hdmi");

    return l_state;
}

//eFunc_SetBrightness,
static emCmdFuncStaType mapp_CmdfuncSetBrightness(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Brightness;
    u8 l_u8Dev,l_u8RealPort;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s16Brightness = (signed int)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s16Brightness<=BRIGHTNESS_VALUE_MAX))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8Brightness[i] = (u8)l_s16Brightness;
                pCommVar->SendPrintf("output %d brightness %d\r\n",i+1,l_s16Brightness);
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                l_u8RealPort = cg_u8ChannelTableTX[i][0];
                app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Brightness,MS1826_ADJUST_BRIGHTNESS);
            }
        }
        else
        {
            pstBasicVar->u8Brightness[l_s8Output-1] = (u8)l_s16Brightness;
            pCommVar->SendPrintf("output %d brightness %d\r\n",l_s8Output,l_s16Brightness);
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0];
            app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Brightness,MS1826_ADJUST_BRIGHTNESS);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetBrightness,
static emCmdFuncStaType mapp_CmdfuncGetBrightness(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("output %d brightness %d\r\n",i+1,pstBasicVar->u8Brightness[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("output %d brightness %d\r\n",l_s8Output,pstBasicVar->u8Brightness[l_s8Output-1]);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_SetContrast,
static emCmdFuncStaType mapp_CmdfuncSetContrast(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Value;
    u8 l_u8Dev,l_u8RealPort;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s16Value = (signed int)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s16Value<=CONTRAST_VALUE_MAX))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8Contrast[i] = (u8)l_s16Value;
                pCommVar->SendPrintf("output %d contrast %d\r\n",i+1,l_s16Value);
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                l_u8RealPort = cg_u8ChannelTableTX[i][0];
                app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Value,MS1826_ADJUST_CONTRAST);
            }
        }
        else
        {
            pstBasicVar->u8Contrast[l_s8Output-1] = (u8)l_s16Value;
            pCommVar->SendPrintf("output %d contrast %d\r\n",l_s8Output,l_s16Value);
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0];
            app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Value,MS1826_ADJUST_CONTRAST);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_GetContrast,
static emCmdFuncStaType mapp_CmdfuncGetContrast(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("output %d contrast %d\r\n",i+1,pstBasicVar->u8Contrast[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("output %d contrast %d\r\n",l_s8Output,pstBasicVar->u8Contrast[l_s8Output-1]);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_SetSaturation,
static emCmdFuncStaType mapp_CmdfuncSetSaturation(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Value;
    u8 l_u8Dev,l_u8RealPort;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s16Value = (signed int)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s16Value<=SATURATION_VALUE_MAX))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8Saturation[i] = (u8)l_s16Value;
                pCommVar->SendPrintf("output %d saturation %d\r\n",i+1,l_s16Value);
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                l_u8RealPort = cg_u8ChannelTableTX[i][0];
                app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Value,MS1826_ADJUST_SATURATION);
            }
        }
        else
        {
            pstBasicVar->u8Saturation[l_s8Output-1] = (u8)l_s16Value;
            pCommVar->SendPrintf("output %d saturation %d\r\n",l_s8Output,l_s16Value);
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0];
            app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Value,MS1826_ADJUST_SATURATION);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_GetSaturation,
static emCmdFuncStaType mapp_CmdfuncGetSaturation(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("output %d saturation %d\r\n",i+1,pstBasicVar->u8Saturation[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("output %d saturation %d\r\n",l_s8Output,pstBasicVar->u8Saturation[l_s8Output-1]);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_SetHue,
static emCmdFuncStaType mapp_CmdfuncSetHue(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Value;
    u8 l_u8Dev,l_u8RealPort;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s16Value = (signed int)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s16Value<=HUE_VALUE_MAX))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8Hue[i] = (u8)l_s16Value;
                pCommVar->SendPrintf("output %d hue %d\r\n",i+1,l_s16Value);
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                l_u8RealPort = cg_u8ChannelTableTX[i][0];
                app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Value,MS1826_ADJUST_HUE);
            }
        }
        else
        {
            pstBasicVar->u8Hue[l_s8Output-1] = (u8)l_s16Value;
            pCommVar->SendPrintf("output %d hue %d\r\n",l_s8Output,l_s16Value);
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0];
            app_MUH0808SPictureAdjust(l_u8Dev,l_u8RealPort,(u8)l_s16Value,MS1826_ADJUST_HUE);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}


//eFunc_GetHue,
static emCmdFuncStaType mapp_CmdfuncGetHue(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("output %d hue %d\r\n",i+1,pstBasicVar->u8Hue[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("output %d hue %d\r\n",l_s8Output,pstBasicVar->u8Hue[l_s8Output-1]);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetRxEdidMode,
static emCmdFuncStaType mapp_CmdfuncSetRxEdidMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8EdidID;
    u8 l_u8EdidBuf[256];
    u8 l_u8RealPort;
    u8 l_u8Dev;

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
            case EDID_MODE_INTERNAL7_3840x2160_30:
                memcpy(l_u8EdidBuf,_7_3840x2160p30,256);
                break;
            case EDID_MODE_USER1:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre1.pu8EdidContext,256);
                break;
            case EDID_MODE_USER2:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre2.pu8EdidContext,256);
                break;
            case EDID_MODE_USER3:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre3.pu8EdidContext,256);
                break;
            case EDID_MODE_USER4:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre4.pu8EdidContext,256);
                break;
            case EDID_MODE_COPYTX1:
            case EDID_MODE_COPYTX2:
            case EDID_MODE_COPYTX3:
            case EDID_MODE_COPYTX4:
            case EDID_MODE_COPYTX5:
            case EDID_MODE_COPYTX6:
            case EDID_MODE_COPYTX7:
            case EDID_MODE_COPYTX8:
                l_u8RealPort = cg_u8ChannelTableTX[l_s8EdidID-EDID_MODE_COPYTX1][0];
                l_u8Dev = cg_u8ChannelTableTX[l_s8EdidID-EDID_MODE_COPYTX1][3];
                app_ms1826_get_tx_edid_data(l_u8Dev,l_u8RealPort,l_u8EdidBuf);
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
                l_u8RealPort = cg_u8ChannelTable[i][1]+1;
                l_u8Dev = cg_u8ChannelTable[i][3];
                app_MS1826RxEdidUpdate(l_u8Dev,l_u8RealPort-1,l_u8EdidBuf,256);
                memcpy(pstBasicVar->u8RxEdid[i],l_u8EdidBuf,256);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][1]+1;
            l_u8Dev = cg_u8ChannelTable[l_s8Input-1][3];
            app_MS1826RxEdidUpdate(l_u8Dev,l_u8RealPort-1,l_u8EdidBuf,256);
            pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_s8Input-1] = l_s8EdidID;
            pCommVar->SendPrintf("in %d edid %d\r\n",l_s8Input,l_s8EdidID);
            memcpy(pstBasicVar->u8RxEdid[l_s8Input-1],l_u8EdidBuf,256);
        }
        SetUserData_SpecBasicVar();
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

//eFunc_GetTxEdidData
static emCmdFuncStaType mapp_CmdfuncGetTxEdidData(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;
    u8 l_u8Dev;
#if 0
    u16 j;
    u8 l_u8Edid[256];

    memset(l_u8Edid,0,256);
#endif

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);

    if(l_s8Output == 0)
    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
            l_u8Dev = cg_u8ChannelTableTX[i][3];
            app_MS1826GetTxEdid(l_u8Dev,i);
//            app_ms1826_get_tx_edid_data(i,l_u8Edid);
#if 0
            pCommVar->SendPrintf("output %d edid:",i+1);
            for(j=0;j<256;j++)
            {
                if(j%16==0)
                {
                    pCommVar->SendPrintf("\r\n");
                }
                pCommVar->SendPrintf("0x%02x,",l_u8Edid[j]);
            }
            pCommVar->SendPrintf("\r\n");
#endif
        }
    }
    else if(l_s8Output<=Def_outport_num)
    {
        l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
        app_MS1826GetTxEdid(l_u8Dev,l_s8Output-1);
//        app_ms1826_get_tx_edid_data(l_s8Output-1,l_u8Edid);
#if 0
        pCommVar->SendPrintf("output %d edid:",l_s8Output);
        for(j=0;j<256;j++)
        {
            if(j%16==0)
            {
                pCommVar->SendPrintf("\r\n");
            }
            pCommVar->SendPrintf("0x%02x,",l_u8Edid[j]);
        }
        pCommVar->SendPrintf("\r\n");
#endif

    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetTxTiming
static emCmdFuncStaType mapp_CmdfuncSetTxTiming(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;
    u8 l_u8NumLen=0;
    stTimingTable_t l_stTiming;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    memset(&l_stTiming,0,sizeof(stTimingTable_t));

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16HActive = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16HTotal = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16HBP = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16HFP = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16HSync = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u8HPol = (u8)GetNumofStr(l_pCmd,',',1);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',1);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16VActive = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16VTotal = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16VBP = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16VFP = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16VSync = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u8VPol = (u8)GetNumofStr(l_pCmd,',',1);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',1);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u16PClk = (u16)GetNumofStr(l_pCmd,')',5);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',5);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_stTiming.u8VClk = (u8)(l_stTiming.u16PClk*10000/l_stTiming.u16HTotal/l_stTiming.u16VTotal);

    if(l_s8Output == 0)
    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
            l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[i][3];
            app_MS1826SetUserTiming(l_u8Dev,l_u8RealPort-1,&l_stTiming);
        }
    }
    else if(l_s8Output<=Def_outport_num)
    {
        l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
        l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
        app_MS1826SetUserTiming(l_u8Dev,l_u8RealPort-1,&l_stTiming);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetTxPattern
static emCmdFuncStaType mapp_CmdfuncSetTxPattern(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8TxPatternID;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8TxPatternID = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s8TxPatternID<=17))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->u8PatternSta[i] = l_s8TxPatternID;
                pCommVar->SendPrintf("output %d pattern is %d\r\n",i+1,l_s8TxPatternID);
                if(pstBasicVar->u8PatternSta[i])
                    app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,pstBasicVar->u8PatternSta[i]-1,1);
                else
                    app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,0,0);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            pstBasicVar->u8PatternSta[l_s8Output-1] = l_s8TxPatternID;
            pCommVar->SendPrintf("output %d pattern is %d\r\n",l_s8Output,l_s8TxPatternID);
            if(pstBasicVar->u8PatternSta[l_s8Output-1])
                app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,pstBasicVar->u8PatternSta[l_s8Output-1]-1,1);
            else
                app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,0,0);
        }
        SetUserData_SpecBasicVar();
    }
    else if((l_s8Output<=(Def_outport_num+_HDMI_INPUT_PORT_USED_NUMBER+1))&&(l_s8TxPatternID<=17))
    {
        if(l_s8Output == (Def_outport_num+_HDMI_INPUT_PORT_USED_NUMBER+1))
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                l_u8Dev = cg_u8ChannelTable[i][3];
                pCommVar->SendPrintf("input %d pattern is %d\r\n",i+1,l_s8TxPatternID);
                if(l_s8TxPatternID)
                    app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,l_s8TxPatternID-1,1);
                else
                    app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,0,0);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1-Def_outport_num][0]+1;
            l_u8Dev = cg_u8ChannelTable[l_s8Output-1-Def_outport_num][3];
            pCommVar->SendPrintf("input %d pattern is %d\r\n",l_s8Output-Def_outport_num,l_s8TxPatternID);
            if(l_s8TxPatternID)
                app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,l_s8TxPatternID-1,1);
            else
                app_MS1826TxPatternConfig(l_u8Dev,l_u8RealPort-1,0,0);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetTxPattern,
static emCmdFuncStaType mapp_CmdfuncGetTxPattern(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("output %d pattern is %d\r\n",i+1,pstBasicVar->u8PatternSta[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("output %d pattern is %d\r\n",l_s8Output,pstBasicVar->u8PatternSta[l_s8Output-1]);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetColor,
static emCmdFuncStaType mapp_CmdfuncSetColor(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output;
    u8 l_u8RVal,l_u8GVal,l_u8BVal;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8RVal = (u8)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8GVal = (u8)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8BVal = (u8)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8RedValue[i]      = l_u8RVal;
                pstBasicVar->u8GreenValue[i]    = l_u8GVal;
                pstBasicVar->u8BlueValue[i]     = l_u8BVal;

                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                if((l_u8RVal==0)&&(l_u8GVal==0)&&(l_u8BVal==0))
                {
                    pCommVar->SendPrintf("output %d color bypass\r\n",i+1);
                    app_MS1826PictureColorConfig(l_u8Dev,l_u8RealPort-1,128,128,128,0);
                }
                else
                {
                    pCommVar->SendPrintf("output %d color R%d G%d B%d\r\n",i+1,l_u8RVal,l_u8GVal,l_u8BVal);
                    app_MS1826PictureColorConfig(l_u8Dev,l_u8RealPort-1,l_u8RVal,l_u8GVal,l_u8BVal,1);
                }
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            pstBasicVar->u8RedValue[l_s8Output-1]      = l_u8RVal;
            pstBasicVar->u8GreenValue[l_s8Output-1]    = l_u8GVal;
            pstBasicVar->u8BlueValue[l_s8Output-1]     = l_u8BVal;
            if((l_u8RVal==0)&&(l_u8GVal==0)&&(l_u8BVal==0))
            {
                pCommVar->SendPrintf("output %d color bypass\r\n",l_s8Output);
                app_MS1826PictureColorConfig(l_u8Dev,l_u8RealPort-1,128,128,128,0);
            }
            else
            {
                pCommVar->SendPrintf("output %d color R%d G%d B%d\r\n",l_s8Output,l_u8RVal,l_u8GVal,l_u8BVal);
                app_MS1826PictureColorConfig(l_u8Dev,l_u8RealPort-1,l_u8RVal,l_u8GVal,l_u8BVal,1);
            }
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetColor,
static emCmdFuncStaType mapp_CmdfuncGetColor(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                if((pstBasicVar->u8RedValue[i]==0)&&(pstBasicVar->u8GreenValue[i]==0)&&(pstBasicVar->u8BlueValue[i]==0))
                {
                    pCommVar->SendPrintf("output %d color bypass\r\n",i+1);
                }
                else
                {
                    pCommVar->SendPrintf("output %d color R%d G%d B%d\r\n",i+1,pstBasicVar->u8RedValue[i],pstBasicVar->u8GreenValue[i],pstBasicVar->u8BlueValue[i]);
                }
            }
        }
        else
        {
            if((pstBasicVar->u8RedValue[l_s8Output-1]==0)&&(pstBasicVar->u8GreenValue[l_s8Output-1]==0)&&(pstBasicVar->u8BlueValue[l_s8Output-1]==0))
            {
                pCommVar->SendPrintf("output %d color bypass\r\n",l_s8Output);
            }
            else
            {
                pCommVar->SendPrintf("output %d color R%d G%d B%d\r\n",l_s8Output,pstBasicVar->u8RedValue[l_s8Output-1],pstBasicVar->u8GreenValue[l_s8Output-1],pstBasicVar->u8BlueValue[l_s8Output-1]);
            }
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
//eFunc_SetSplitConfig
static emCmdFuncStaType mapp_CmdfuncSetSplitConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Port;
    u8 l_u8NumLen=0;
    u8 l_u8RealPort,u8Enalbe;
    u16 l_u16X,l_u16Y,l_u16W,l_u16H;

    l_s8Port = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    u8Enalbe = (u16)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16X = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16Y = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16W = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16H = (u16)GetNumofStr(l_pCmd,')',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    if(l_s8Port == 0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            l_u8RealPort = cg_u8ChannelTable[i][0]+1;
            app_MS1826SplitConfig(i/4,l_u8RealPort-1,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
            pCommVar->SendPrintf("split config: port[%d] en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",i+1,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
        }
    }
    else if(l_s8Port<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        l_u8RealPort = cg_u8ChannelTable[l_s8Port-1][0]+1;
        app_MS1826SplitConfig((l_s8Port-1)/4,l_u8RealPort-1,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
        pCommVar->SendPrintf("split config: port[%d] en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",l_s8Port,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetSplicerConfig,
static emCmdFuncStaType mapp_CmdfuncSetSplicerConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Port;
    u8 l_u8NumLen=0;
    u8 /*l_u8RealPort,*/u8Enalbe;
    u16 l_u16X,l_u16Y,l_u16W,l_u16H;
    u8 l_u8Dev;

    l_s8Port = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    u8Enalbe = (u16)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16X = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16Y = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16W = (u16)GetNumofStr(l_pCmd,',',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u16H = (u16)GetNumofStr(l_pCmd,')',4);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',4);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    if(l_s8Port == 0)
    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
//            l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
            l_u8Dev = cg_u8ChannelTableTX[i][3];
//            app_MS1826SplicerEnable(l_u8Dev,l_u8RealPort-1,(u8Enalbe==0)?FALSE:TRUE);
//            app_MS1826SplicerConfig(l_u8Dev,l_u8RealPort-1,l_u16X,l_u16Y,l_u16W,l_u16H);
            pstBasicVar->u16SplicerX[i] = l_u16X;
            pstBasicVar->u16SplicerY[i] = l_u16Y;
            pstBasicVar->u16SplicerWidth[i] = l_u16W;
            pstBasicVar->u16SplicerHeight[i] = l_u16H;
            pstBasicVar->bSplicerEnable[i] = (u8Enalbe==0)?FALSE:TRUE;
            app_MUH0808SSetSplicerPixelDisplay(l_u8Dev,i,l_u16X,l_u16Y,l_u16W,l_u16H);
            pCommVar->SendPrintf("splicer config: port[%d] x[%d] y[%d] w[%d] h[%d]\r\n",i+1,l_u16X,l_u16Y,l_u16W,l_u16H);
        }
    }
    else if(l_s8Port<=Def_outport_num)
    {
//        l_u8RealPort = cg_u8ChannelTableTX[l_s8Port-1][0]+1;
        l_u8Dev = cg_u8ChannelTableTX[l_s8Port-1][3];
//        app_MS1826SplicerEnable(l_u8Dev,l_u8RealPort-1,(u8Enalbe==0)?FALSE:TRUE);
//        app_MS1826SplicerConfig(l_u8Dev,l_u8RealPort-1,l_u16X,l_u16Y,l_u16W,l_u16H);
        pstBasicVar->u16SplicerX[l_s8Port-1] = l_u16X;
        pstBasicVar->u16SplicerY[l_s8Port-1] = l_u16Y;
        pstBasicVar->u16SplicerWidth[l_s8Port-1] = l_u16W;
        pstBasicVar->u16SplicerHeight[l_s8Port-1] = l_u16H;
        pstBasicVar->bSplicerEnable[l_s8Port-1] = (u8Enalbe==0)?FALSE:TRUE;
        app_MUH0808SSetSplicerPixelDisplay(l_u8Dev,l_s8Port-1,l_u16X,l_u16Y,l_u16W,l_u16H);
        pCommVar->SendPrintf("splicer config: port[%d] x[%d] y[%d] w[%d] h[%d]\r\n",l_s8Port,l_u16X,l_u16Y,l_u16W,l_u16H);
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}


//eFunc_GetSplicerConfig,
static emCmdFuncStaType mapp_CmdfuncGetSplicerConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Port;

    l_s8Port = (char)GetNumofStr(l_pCmd,')',3);

    if(l_s8Port == 0)
    {
        u8 i;
        for(i=0;i<Def_outport_num;i++)
        {
            pCommVar->SendPrintf("splicer config: port[%d] en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",i+1,
                    pstBasicVar->bSplicerEnable[i],
                    pstBasicVar->u16SplicerX[i],
                    pstBasicVar->u16SplicerY[i],
                    pstBasicVar->u16SplicerWidth[i],
                    pstBasicVar->u16SplicerHeight[i]);
        }
    }
    else if(l_s8Port<=Def_outport_num)
    {
        pCommVar->SendPrintf("splicer config: port[%d] en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",l_s8Port,
                    pstBasicVar->bSplicerEnable[l_s8Port-1],
                    pstBasicVar->u16SplicerX[l_s8Port-1],
                    pstBasicVar->u16SplicerY[l_s8Port-1],
                    pstBasicVar->u16SplicerWidth[l_s8Port-1],
                    pstBasicVar->u16SplicerHeight[l_s8Port-1]);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetSplicerMode,
static emCmdFuncStaType mapp_CmdfuncSetSplicerMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;
    u8 l_u8NumLen=0;
    u8 l_u8W,l_u8H;

    l_s8Group = (char)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8H = (u16)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8W = (u16)GetNumofStr(l_pCmd,')',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    if(l_s8Group == 0)
    {
        u8 i;
        for(i=0;i<2;i++)
        {
            pstBasicVar->stSplicerGroup[i].u8SplicerWidth     = l_u8W;
            pstBasicVar->stSplicerGroup[i].u8SplicerHeight    = l_u8H;
            pCommVar->SendPrintf("splicer mode group %d is %dx%d\r\n",i+1,l_u8H,l_u8W);
        }
    }
    else if(l_s8Group<=2)
    {
        pstBasicVar->stSplicerGroup[l_s8Group-1].u8SplicerWidth     = l_u8W;
        pstBasicVar->stSplicerGroup[l_s8Group-1].u8SplicerHeight    = l_u8H;
        pCommVar->SendPrintf("splicer mode group %d is %dx%d\r\n",l_s8Group,l_u8H,l_u8W);
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}

//eFunc_GetSplicerMode,
static emCmdFuncStaType mapp_CmdfuncGetSplicerMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;

    l_s8Group = (u16)GetNumofStr(l_pCmd,')',2);

    if(l_s8Group == 0)
    {
        u8 i;
        for(i=0;i<2;i++)
        {
            pCommVar->SendPrintf("splicer mode group %d is %dx%d\r\n",i+1,pstBasicVar->stSplicerGroup[i].u8SplicerHeight,
                    pstBasicVar->stSplicerGroup[i].u8SplicerWidth);
        }
    }
    else if(l_s8Group<=2)
    {
        pCommVar->SendPrintf("splicer mode group %d is %dx%d\r\n",l_s8Group,pstBasicVar->stSplicerGroup[l_s8Group-1].u8SplicerHeight,
                pstBasicVar->stSplicerGroup[l_s8Group-1].u8SplicerWidth);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetSplicerBorder,
static emCmdFuncStaType mapp_CmdfuncSetSplicerBorder(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;
    u8 l_u8NumLen=0;
    u8 l_u8W,l_u8H;
    u8 j;

    l_s8Group = (char)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8H = (u16)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8W = (u16)GetNumofStr(l_pCmd,')',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    if(l_s8Group == 0)
    {
        if(l_u8W<100&&l_u8H<100)
        {
            for(j=0;j<2;j++)
            {
                pstBasicVar->stSplicerGroup[j].u8BorderSizeWidth  = l_u8W;
                pstBasicVar->stSplicerGroup[j].u8BorderSizeHeight = l_u8H;
                pCommVar->SendPrintf("splicer border set group %d v %d h %d\r\n",j+1,l_u8H,l_u8W);
            }
        }
        else
            l_state = emCmdOutOfRange;
    }
    else if(l_s8Group<=2)
    {
        if(l_u8W<100&&l_u8H<100)
        {
            pstBasicVar->stSplicerGroup[l_s8Group-1].u8BorderSizeWidth  = l_u8W;
            pstBasicVar->stSplicerGroup[l_s8Group-1].u8BorderSizeHeight = l_u8H;
            pCommVar->SendPrintf("splicer border set group %d v %d h %d\r\n",l_s8Group,l_u8H,l_u8W);
        }
        else
            l_state = emCmdOutOfRange;
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}

//eFunc_GetSplicerBorder,
static emCmdFuncStaType mapp_CmdfuncGetSplicerBorder(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;
    u8 l_u8W,l_u8H;

    l_s8Group = (u16)GetNumofStr(l_pCmd,')',2);

    if(l_s8Group == 0)
    {
        u8 i;
        for(i=0;i<2;i++)
        {
            l_u8W = pstBasicVar->stSplicerGroup[i].u8BorderSizeWidth;
            l_u8H = pstBasicVar->stSplicerGroup[i].u8BorderSizeHeight;
            pCommVar->SendPrintf("splicer border set group %d v %d h %d\r\n",i+1,l_u8H,l_u8W);
        }
    }
    else if(l_s8Group<=Def_outport_num)
    {
        l_u8W = pstBasicVar->stSplicerGroup[l_s8Group-1].u8BorderSizeWidth;
        l_u8H = pstBasicVar->stSplicerGroup[l_s8Group-1].u8BorderSizeHeight;
        pCommVar->SendPrintf("splicer border set group %d v %d h %d\r\n",l_s8Group,l_u8H,l_u8W);
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}

//eFunc_SetSplicerPosition,

static emCmdFuncStaType mapp_CmdfuncSetSplicerPosition(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;
    u8 l_u8NumLen=0;
    u8 l_u8Port;
    u8 l_u8X,l_u8Y,l_u8W,l_u8H,l_u8Index;
    u8 l_u8Dev,i;

    l_s8Group = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Port = (u16)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8X = (u16)GetNumofStr(l_pCmd,',',3);  //l_u8Y
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Y = (u16)GetNumofStr(l_pCmd,')',3);  //l_u8X
    l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    if((l_s8Group<=2&&l_s8Group>0)&&(l_u8Port<=Def_outport_num&&l_u8Port>0))
    {
        l_u8W = (pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].u8SplicerWidth);
        l_u8H = (pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].u8SplicerHeight);
        if((l_u8X>0&&l_u8X<=l_u8H)&&(l_u8Y>0&&l_u8Y<=l_u8W))
        {
            l_u8Index = (l_u8X-1)*l_u8W+(l_u8Y-1);
			for(i=0;i<Def_outport_num;i++)
			{
				if((pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].u8SplicerPosX[i]==l_u8X)&&(pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].u8SplicerPosY[i]==l_u8Y))
				{
					if(i==l_u8Port-1) break;
					else   pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].bGroupTxEnable[i]= FALSE;
				}
			}
//			pCommVar->SendPrintf("l_u8Index = %d\r\n",l_u8Index);
//			pCommVar->SendPrintf("l_u8Y = %d\r\n",l_u8Y);
//			pCommVar->SendPrintf("l_u8X = %d\r\n",l_u8X);
//			pCommVar->SendPrintf("l_u8H = %d\r\n",l_u8H);
//			pCommVar->SendPrintf("l_u8W = %d\r\n",l_u8W);
            l_u8Dev = cg_u8ChannelTableTX[l_u8Port-1][3];
            app_MUH0808SSetSplicerBorder(l_u8Port-1,l_u8W,l_u8H,l_u8Index,
                        pstBasicVar->stSplicerGroup[l_s8Group-1].u8BorderSizeWidth,pstBasicVar->stSplicerGroup[l_s8Group-1].u8BorderSizeHeight);
            app_MUH0808SSetSplicerDisplay(l_u8Dev,l_u8Port-1,l_u8H,l_u8W,l_u8Index,l_u8X,l_u8Y);
            pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].u8SplicerPosX[l_u8Port-1] = l_u8X;
            pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].u8SplicerPosY[l_u8Port-1] = l_u8Y;
            pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].bGroupTxEnable[l_u8Port-1] = TRUE;
//            pCommVar->SendPrintf("splicer position:\r\n"
//                                 "    group: %d,\r\n"
//                                 "    port: %d,\r\n"
//                                 "    xpos: %d,\r\n"
//                                 "    ypos: %d,\r\n",l_s8Group,l_u8Port,l_u8X,l_u8Y);
			pCommVar->SendPrintf("splicer\r\n");
        }
        else
            l_state = emCmdOutOfRange;
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}

//eFunc_GetSplicerPosition,
static emCmdFuncStaType mapp_CmdfuncGetSplicerPosition(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;
    u8 l_u8X,l_u8Y;
    u8 i,j,status;

    l_s8Group = (u16)GetNumofStr(l_pCmd,')',2);

    if(l_s8Group == 0)
    {
        for(j=0;j<2;j++)
        {
            for(i=0;i<Def_outport_num;i++)
            {
                if(pstSpecVar->stBasSpecVar.stSplicerGroup[j].bGroupTxEnable[i] == TRUE)
                {
                    l_u8X = pstBasicVar->stSplicerGroup[j].u8SplicerPosX[i];
                    l_u8Y = pstBasicVar->stSplicerGroup[j].u8SplicerPosY[i];
                    pCommVar->SendPrintf("splicer position:\r\n"
                                         "    group: %d,\r\n"
                                         "    port: %d,\r\n"
                                         "    xpos: %d,\r\n"
                                         "    ypos: %d;\r\n",j+1,i+1,l_u8X,l_u8Y);
                }
            }
        }
    }
    else if(l_s8Group<=2)
    {
        for(i=0;i<Def_outport_num;i++)
        {
            if(pstSpecVar->stBasSpecVar.stSplicerGroup[l_s8Group-1].bGroupTxEnable[i] == TRUE)
            {
				status=1;
                l_u8X = pstBasicVar->stSplicerGroup[l_s8Group-1].u8SplicerPosX[i];
                l_u8Y = pstBasicVar->stSplicerGroup[l_s8Group-1].u8SplicerPosY[i];
                pCommVar->SendPrintf("splicer position:\r\n"
                                     "    group: %d,\r\n"
                                     "    port: %d,\r\n"
                                     "    xpos: %d,\r\n"
                                     "    ypos: %d;\r\n",l_s8Group,i+1,l_u8X,l_u8Y);
            }
        }
		if(status!=1)    pCommVar->SendPrintf("splicer clear group %d\r\n",l_s8Group);

    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}

//eFunc_SetSplicerClear
static emCmdFuncStaType mapp_CmdfuncSetSplicerClear(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;
    u8 l_u8Dev;
    u8 i;

    l_s8Group = (u16)GetNumofStr(l_pCmd,')',2);

    if(l_s8Group == 0)
    {
        for(i=0;i<Def_outport_num;i++)
        {
            if(pstBasicVar->stSplicerGroup[0].bGroupTxEnable[i] == TRUE)
            {
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->stSplicerGroup[0].bGroupTxEnable[i] = FALSE;
                pstBasicVar->bSplicerEnable[i]   = FALSE;
                app_MS1826SplicerEnable(l_u8Dev,cg_u8ChannelTableTX[i][0],FALSE);
            }
            if(pstBasicVar->stSplicerGroup[1].bGroupTxEnable[i] == TRUE)
            {
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->stSplicerGroup[1].bGroupTxEnable[i] = FALSE;
                pstBasicVar->bSplicerEnable[i]   = FALSE;
                app_MS1826SplicerEnable(l_u8Dev,cg_u8ChannelTableTX[i][0],FALSE);
            }
        }
        pCommVar->SendPrintf("splicer clear group %d\r\n",1);
        pCommVar->SendPrintf("splicer clear group %d\r\n",2);
    }
    else if(l_s8Group<=Def_outport_num)
    {
        for(i=0;i<Def_outport_num;i++)
        {
            if(pstBasicVar->stSplicerGroup[l_s8Group-1].bGroupTxEnable[i] == TRUE)
            {
                l_u8Dev = cg_u8ChannelTableTX[i][3];
                pstBasicVar->stSplicerGroup[l_s8Group-1].bGroupTxEnable[i] = FALSE;
                pstBasicVar->bSplicerEnable[i]   = FALSE;
                app_MS1826SplicerEnable(l_u8Dev,cg_u8ChannelTableTX[i][0],FALSE);
            }
        }
        pCommVar->SendPrintf("splicer clear group %d\r\n",l_s8Group);
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}

//eFunc_SetFrameLock
static emCmdFuncStaType mapp_CmdfuncSetFrameLock(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
//    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
//    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8Input;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8Input = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&((l_s8Input>0)&&(l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                l_u8RealPort = cg_u8ChannelTableTX[l_s8Input-1][1];
                l_u8Dev = cg_u8ChannelTableTX[l_s8Input-1][3];
                pCommVar->SendPrintf("output %d frame lock %d\r\n",i+1,l_s8Output);
                #if ENABLE_MS1826_FRAME_LOCK
                app_MS1826FrameLockSelect(l_u8Dev,l_u8RealPort,i);
                #endif
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Input-1][1];
            l_u8Dev = cg_u8ChannelTableTX[l_s8Input-1][3];
            pCommVar->SendPrintf("output %d frame lock %d\r\n",l_s8Output,l_s8Input);
            #if ENABLE_MS1826_FRAME_LOCK
            app_MS1826FrameLockSelect(l_u8Dev,l_u8RealPort,l_s8Output);
            #endif
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    UNUSED(l_u8RealPort);
    UNUSED(l_u8Dev);
    return l_state;
}

//eFunc_SetMs1826Hold
static emCmdFuncStaType mapp_CmdfuncSetMs1826Hold(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8TempVal;

    l_s8TempVal = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8TempVal<=1)
    {
        pCommVar->SendPrintf("ms1826 hold: %s\r\n",l_s8TempVal?"en":"dis");

//        app_MS1826SetHoldFlag(l_s8TempVal?TRUE:FALSE);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_BypassSwitchPort
static emCmdFuncStaType mapp_CmdfuncBypassSwitchPort(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    u8 i=0,j=0,l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8Output[Def_outport_num],l_s8Temp;
    u8 l_u8RealPort;

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
            j++;
            pCommVar->SendPrintf("out %d bypass in %d\r\n",i+1,l_s8Input);
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][1]+1;
            app_MS1826BypassSwitch(0,l_u8RealPort-1, i);
        }
    }

    return l_state;

}

//eFunc_SetVideoMute
static emCmdFuncStaType mapp_CmdfuncSetVideoMute(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Input,l_s8MuteSta;
    u8 l_u8RealPort;
    u8 l_u8Dev;

    l_s8Input = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8MuteSta = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)&&(l_s8MuteSta>=0&&l_s8MuteSta<=1))
    {
        if(l_s8Input == 0)
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                l_u8RealPort = cg_u8ChannelTable[i][0];
                l_u8Dev = cg_u8ChannelTable[i][3];
                pCommVar->SendPrintf("hdmi %d video is %s\r\n",i+1,l_s8MuteSta?"mute":"unmute");
                app_MS1826SetVideoMute(l_u8Dev,l_u8RealPort,l_s8MuteSta?TRUE:FALSE);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][0];
            l_u8Dev = cg_u8ChannelTable[l_s8Input-1][3];
            pCommVar->SendPrintf("hdmi %d video is %s\r\n",l_s8Input,l_s8MuteSta?"mute":"unmute");
            app_MS1826SetVideoMute(l_u8Dev,l_u8RealPort,l_s8MuteSta?TRUE:FALSE);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_ModeSwitch
static emCmdFuncStaType mapp_CmdfuncModeSwitch(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8WorkMode;
    char l_s8SubMode;
    char l_s8ChipIndex;
    u8 l_u8NumLen=0;

    l_s8ChipIndex = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;

    l_s8WorkMode = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);

    l_pCmd = l_pCmd+l_u8NumLen+1;
    l_s8SubMode = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8WorkMode<=4)&&(l_s8ChipIndex>=1&&l_s8ChipIndex<=2))
    {
        pCommVar->SendPrintf("group %d run mode: %d\r\n",l_s8ChipIndex,l_s8WorkMode);
        app_MS1826WorkModeSwitch(l_s8ChipIndex-1,l_s8WorkMode,l_s8SubMode);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetCtrlBaudrate,
static emCmdFuncStaType mapp_CmdfuncSetCtrlBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstUartCom_t pUartCommVar = &g_stUart2CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n","in baudrate", mapp_GetRealBaudrate(pstBasicVar->u8InRs232BaudrateID));
    return l_state;
}
//eFunc_SetOutBaudrate,
static emCmdFuncStaType mapp_CmdfuncSetOutBaudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n","out baudrate", mapp_GetRealBaudrate(pstBasicVar->u8OutRs232BaudrateID));
    return l_state;
}

//eFunc_SetRs232Mode,
static emCmdFuncStaType mapp_CmdfuncSetRs232Mode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->u8UartMode==1)
        pCommVar->SendPrintf("rs232 mode: in&out control local\r\n");
    else
        pCommVar->SendPrintf("rs232 mode: in control local&out\r\n");
    return l_state;
}

//eFunc_SetTxExtAudio,
static emCmdFuncStaType mapp_CmdfuncSetTxExtAudio(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8ADOn;
    u8 l_u8RealPort,u8dev;

    l_s8Output = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8ADOn = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Output<=Def_outport_num)&&(l_s8ADOn>=0&&l_s8ADOn<=1))
    {
        if(l_s8Output == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicVar->u8AudioMute[i] = l_s8ADOn?0:1;
                pCommVar->SendPrintf("external audio %d is %s\r\n",i+1,pstBasicVar->u8AudioMute[i]?"off":"on");
                l_u8RealPort = cg_u8ChannelTableTX[i][0]+1;
                u8dev = cg_u8ChannelTableTX[i][3];
                app_MS1826TxDigitalAudioMute(u8dev,l_u8RealPort-1,pstBasicVar->u8AudioMute[l_s8Output-1]);
            }
        }
        else
        {
            pstBasicVar->u8AudioMute[l_s8Output-1] = l_s8ADOn?0:1;;
            pCommVar->SendPrintf("external audio %d is %s\r\n",l_s8Output,pstBasicVar->u8AudioMute[l_s8Output-1]?"off":"on");
            l_u8RealPort = cg_u8ChannelTableTX[l_s8Output-1][0]+1;
            u8dev = cg_u8ChannelTableTX[l_s8Output-1][3];
            app_MS1826TxDigitalAudioMute(u8dev,l_u8RealPort-1,pstBasicVar->u8AudioMute[l_s8Output-1]);
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
                pCommVar->SendPrintf("external audio %d is %s\r\n",i+1,pstBasicVar->u8AudioMute[i]?"off":"on");
            }
        }
        else
        {
            pCommVar->SendPrintf("external audio %d is %s\r\n",l_s8Output,pstBasicVar->u8AudioMute[l_s8Output-1]?"off":"on");
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
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=1)
    {
        app_MUH0808SSceneSave(l_s8SceneID-1);
        pCommVar->SendPrintf("scene %d saved\r\n",l_s8SceneID);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_CallScene,
static emCmdFuncStaType mapp_CmdfuncCallScene(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID;

    l_s8SceneID = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8SceneID<=32&&l_s8SceneID>=1)
    {
        if(pstBasicVar->stScene[l_s8SceneID-1].u8SceneSaveFlag == TRUE)
        {
            app_MUH0808SSceneCall(l_s8SceneID-1);

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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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

//eFunc_ScenePolling,
static emCmdFuncStaType mapp_CmdfuncScenePolling(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8PollingEn;

    l_s8PollingEn = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8PollingEn<=1&&l_s8PollingEn>=0)
    {
        if(l_s8PollingEn)
        {
            pstBasicVar->stScenePollingVar.bPollingEnable = TRUE;
            g_u8SceneStartIndex = 0;
            pCommVar->SendPrintf("scene polling enable\r\n");
        }
        else
        {
            pstBasicVar->stScenePollingVar.bPollingEnable = FALSE;
            g_u8SceneStartIndex = 0;
            pCommVar->SendPrintf("scene polling disable\r\n");
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_ScenePollingTime,
static emCmdFuncStaType mapp_CmdfuncScenePollingTime(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s16SceneTime;

    l_s16SceneTime = (char)GetNumofStr(l_pCmd,')',4);
    if(l_s16SceneTime<=9999&&l_s16SceneTime>=5)
    {
        pstBasicVar->stScenePollingVar.u16PollingTime = l_s16SceneTime;
        pCommVar->SendPrintf("scene polling time %ds\r\n",l_s16SceneTime);
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetScenePollingTime
static emCmdFuncStaType mapp_CmdfuncGetScenePollingTime(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("scene polling time %ds\r\n",pstBasicVar->stScenePollingVar.u16PollingTime);

    UNUSED(cmdnum);
    return l_state;
}

//eFunc_GetScenePollingSta,
static emCmdFuncStaType mapp_CmdfuncGetScenePollingSta(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->stScenePollingVar.bPollingEnable)
    {
        pCommVar->SendPrintf("scene polling enable\r\n");
    }
    else
    {
        pCommVar->SendPrintf("scene polling disable\r\n");
    }

    UNUSED(cmdnum);
    return l_state;
}
#if 0
//eFunc_SetPollingList,
static emCmdFuncStaType mapp_CmdfuncSetPollingList(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i=0,j=0,l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID[32],l_s8Temp;
    char l_cShowText[100];

    memset(l_cShowText,0,100);
    memset(l_s8SceneID,0xFF,32);

//    l_s8SceneID = (char)GetNumofStr(l_pCmd,',',3);
//    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
//    l_pCmd = l_pCmd+l_u8NumLen+1;
    do
    {
        if(*l_pCmd == 'a')
        {
            for(i=0;i<32;i++)
            {
                l_s8SceneID[i] = i;
            }
            break;
        }
        l_s8Temp = (char)GetNumofStr(l_pCmd,',',3);
        l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
        if(l_s8Temp>0&&l_s8Temp<=32)
        {
            l_pCmd = l_pCmd+l_u8NumLen+1;
            l_s8SceneID[i] =l_s8Temp-1;
            i++;
        }
        else
        {
            l_s8Temp = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            if(l_s8Temp>0&&l_s8Temp<=32)
            {
                l_pCmd = l_pCmd+l_u8NumLen+1;
                l_s8SceneID[i] =l_s8Temp-1;
                i++;
            }
            else
                break;
        }

        if(i>=32)
            break;
    }
    while(1);

    l_s8Temp = i;
    bubble_sort((u8*)l_s8SceneID,l_s8Temp,0);

    l_u8NumLen = 0;
    memset(pstBasicVar->stScenePollingVar.bPollingListEnable,FALSE,32);
    for(i=0;i<32;i++)
    {
        if(i == l_s8SceneID[j])
        {
            pstBasicVar->stScenePollingVar.bPollingListEnable[i] = TRUE;
            if((i+1)/10)
            {
                l_cShowText[l_u8NumLen++] = (i+1)/10+'0';
            }
            l_cShowText[l_u8NumLen++] = (i+1)%10+'0';
            l_cShowText[l_u8NumLen++] = ',';
            j++;
        }
    }
    if(l_u8NumLen)
    {
        if(l_cShowText[l_u8NumLen-1] == ',')
        {
            l_cShowText[l_u8NumLen-1] = 0;
        }
        pCommVar->SendPrintf("scene polling list %s\r\n",l_cShowText);
    }
    else
    {
        pCommVar->SendPrintf("scene polling list empty\r\n");
    }
    UNUSED(cmdnum);
    return l_state;
}

//eFunc_GetPollingList,
static emCmdFuncStaType mapp_CmdfuncGetPollingList(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i,l_u8TextLen=0;
    char l_cShowText[100];

    memset(l_cShowText,0,100);
    for(i=0;i<32;i++)
    {
        if(pstBasicVar->stScenePollingVar.bPollingListEnable[i])
        {
            if((i+1)/10)
            {
                l_cShowText[l_u8TextLen++] = (i+1)/10+'0';
            }
            l_cShowText[l_u8TextLen++] = (i+1)%10+'0';
            l_cShowText[l_u8TextLen++] = ',';
        }
    }

    if(l_u8TextLen)
    {
        if(l_cShowText[l_u8TextLen-1] == ',')
        {
            l_cShowText[l_u8TextLen-1] = 0;
        }
        pCommVar->SendPrintf("scene polling list %s\r\n",l_cShowText);
    }
    else
    {
        pCommVar->SendPrintf("scene polling list empty\r\n");
    }

    UNUSED(cmdnum);
    return l_state;
}
#endif
//eFunc_SetPollingList,
static emCmdFuncStaType mapp_CmdfuncSetPollingList(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i=0,j=0,l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8SceneID[32],l_s8Temp;
    char l_cShowText[100];

    memset(l_cShowText,0,100);
    memset(l_s8SceneID,0xFF,32);

//    l_s8SceneID = (char)GetNumofStr(l_pCmd,',',3);
//    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
//    l_pCmd = l_pCmd+l_u8NumLen+1;
    do
    {
        if(*l_pCmd == 'a')
        {
            for(i=0;i<32;i++)
            {
                l_s8SceneID[i] = i;
            }
            break;
        }
        l_s8Temp = (char)GetNumofStr(l_pCmd,',',3);
        l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
        if(l_s8Temp>0&&l_s8Temp<=32)
        {
            l_pCmd = l_pCmd+l_u8NumLen+1;
            l_s8SceneID[i] =l_s8Temp-1;
            i++;
        }
        else
        {
            l_s8Temp = (char)GetNumofStr(l_pCmd,')',3);
            l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
            if(l_s8Temp>0&&l_s8Temp<=32)
            {
                l_pCmd = l_pCmd+l_u8NumLen+1;
                l_s8SceneID[i] =l_s8Temp-1;
                i++;
            }
            else
                break;
        }

        if(i>=32)
            break;
    }
    while(1);

    l_s8Temp = i;
//    bubble_sort((u8*)l_s8SceneID,l_s8Temp,0);

    l_u8NumLen = 0;
    memset(pstBasicVar->stScenePollingVar.bPollingListEnable,FALSE,32);
    memset(pstBasicVar->stScenePollingVar.u8PollingListSave,0,32);
//    for(i=0;i<32;i++)
//    {
//        if(i == l_s8SceneID[j])
//        {
//            pstBasicVar->stScenePollingVar.bPollingListEnable[i] = TRUE;
//            if((i+1)/10)
//            {
//                l_cShowText[l_u8NumLen++] = (i+1)/10+'0';
//				 l_cShowText[l_u8NumLen++] = (i+1)%10+'0';
//				  l_cShowText[l_u8NumLen++] = ',';
//            }
//            l_cShowText[l_u8NumLen++] = (i+1)%10+'0';
//            l_cShowText[l_u8NumLen++] = ',';
//            j++;
//        }
//    }
	while(l_s8SceneID[j]!=0xff)
	{
		pstBasicVar->stScenePollingVar.bPollingListEnable[l_s8SceneID[j]] = TRUE;
        pstBasicVar->stScenePollingVar.u8PollingListSave[j]=l_s8SceneID[j]+1;
            if((l_s8SceneID[j]+1)/10)
            {
                l_cShowText[l_u8NumLen++] = (l_s8SceneID[j]+1)/10+'0';
				 l_cShowText[l_u8NumLen++] = (l_s8SceneID[j]+1)%10+'0';
				  l_cShowText[l_u8NumLen++] = ',';
            }
			else
			{
            l_cShowText[l_u8NumLen++] = (l_s8SceneID[j]+1)%10+'0';
            l_cShowText[l_u8NumLen++] = ',';
			}
			j++;
			if(j==32) break;

	}
    if(l_u8NumLen)
    {
        if(l_cShowText[l_u8NumLen-1] == ',')
        {
            l_cShowText[l_u8NumLen-1] = 0;
        }
        pCommVar->SendPrintf("scene polling list %s\r\n",l_cShowText);
    }
    else
    {
        pCommVar->SendPrintf("scene polling list empty\r\n");
    }
    UNUSED(cmdnum);
    SetUserData_SpecBasicVar();
    return l_state;
}


//eFunc_GetPollingList,
static emCmdFuncStaType mapp_CmdfuncGetPollingList(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i,l_u8TextLen=0;
    char l_cShowText[100];
    u8 temp;

    memset(l_cShowText,0,100);
    for(i=0;i<32;i++)
    {
//        if(pstBasicVar->stScenePollingVar.bPollingListEnable[i])
         if(pstBasicVar->stScenePollingVar.u8PollingListSave[i]!=0)
        {
            temp=pstBasicVar->stScenePollingVar.u8PollingListSave[i];
            if((temp)/10)
            {
                l_cShowText[l_u8TextLen++] = (temp)/10+'0';
				l_cShowText[l_u8TextLen++] = (temp)%10+'0';
				l_cShowText[l_u8TextLen++] = ',';
            }
			else
			{
            l_cShowText[l_u8TextLen++] = (temp)%10+'0';
            l_cShowText[l_u8TextLen++] = ',';
			}
        }
    }

    if(l_u8TextLen)
    {
        if(l_cShowText[l_u8TextLen-1] == ',')
        {
            l_cShowText[l_u8TextLen-1] = 0;
        }
        pCommVar->SendPrintf("scene polling list %s\r\n",l_cShowText);
    }
    else
    {
        pCommVar->SendPrintf("scene polling list empty\r\n");
    }

    UNUSED(cmdnum);
    return l_state;
}
//eFunc_GetDeviceInfo,
static emCmdFuncStaType mapp_CmdfuncGetDeviceInfo(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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

    pCommVar->SendPrintf("ch  1 2 3 4 5 6 7 8\r\n");
    pCommVar->SendPrintf("in  %c %c %c %c %c %c %c %c\r\n",
                    g_stLinkSta.u8InLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[3]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[4]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[5]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[6]?'Y':'N',
                    g_stLinkSta.u8InLinkSta[7]?'Y':'N'
                    );
    pCommVar->SendPrintf("out %c %c %c %c %c %c %c %c\r\n",
                    g_stLinkSta.u8OutLinkSta[0]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[1]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[2]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[3]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[4]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[5]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[6]?'Y':'N',
                    g_stLinkSta.u8OutLinkSta[7]?'Y':'N'
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

    l_u8Len = my_StrStr(l_pCmd,")")-l_pCmd;

    if(l_emRtState == emCmdSucess)
    {
        char l_cDisplayText[16];

        memset(l_cDisplayText,' ',16);
        SetUserData_ProjectName(l_pCmd,l_u8Len);
        pCommVar->SendPrintf("product name:%s\r\n",pstPrjNameSet->sPrjName);

        if(pstPrjNameSet->sPrjName[0]!=0&&pstPrjNameSet->sPrjName[1]!=0)
        {
            strncpy(l_cDisplayText, (char*)pstPrjNameSet->sPrjName,l_u8Len);
        }
        else
        {
            strncpy(l_cDisplayText, LcdNone,16);
        }
        LCD_LineRefresh(l_cDisplayText,LINE2);
    }
    return l_emRtState;
}
//eFunc_SetIPAddr,
static emCmdFuncStaType mapp_CmdfuncSetIPAddr(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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

//eFunc_SetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncSetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Lock;
    u8 l_u8DisplayText[16];

    l_s8Lock = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Lock>=0&&l_s8Lock<=1)
    {
        pstBasicVar->bIsLock = l_s8Lock?TRUE:FALSE;
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s\r\n",l_s8Lock?"lock":"unlock");

        if(pstBasicVar->bIsLock)
        {
            strncpy((char *)l_u8DisplayText,"System Locked    ",16);
        }
        else
        {
            strncpy((char *)l_u8DisplayText,"System Unlocked    ",16);
            LED_RecoverToDefault(DEF_LED_DISPLAY_TIME);
        }
        mapp_LCDBacklightEnable();
        LCD_LineRefresh((char *)l_u8DisplayText,LINE3);
        LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
        mapp_SetLedState(led_name_LedLock,TRUE);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncGetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s\r\n",pstBasicVar->bIsLock?"lock":"unlock");
    return l_state;
}

#if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
//eFunc_SendUserCecCmd,
// (cec,XX,XX,XX...)
static emCmdFuncStaType mapp_CmdfuncSendUserCecCmd(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8DevID = 0;
    u8 l_u8Addr = 0;
    u8 l_u8Opcode = 0;
    u8 l_u8TempData[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd); l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==0))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8DevID = (l_u8DevID<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==1))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8Addr = (l_u8Addr<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==2))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8Opcode = (l_u8Opcode<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex>=3))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8TempData[l_u8DataIndex-3] = (l_u8TempData[l_u8DataIndex-3]<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else
        {
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]==')')
        {
            break;
        }
    }

    if(l_u8DevID>5)
        return emCmdOutOfRange;
    if(l_u8DevID == 0)
        l_u8DevID = CEC_OPERATE_INPUT;

    if(l_u8DataIndex<3)
        l_u8DataIndex = 3;

    pCommVar->SendPrintf("cec send to device: %d\r\n", l_u8DevID);
    pCommVar->SendPrintf("  header : 0x%02x\r\n", l_u8Addr);
    pCommVar->SendPrintf("  opcode : 0x%02x\r\n", l_u8Opcode);
    if((l_u8DataIndex-3)>0)
    {
        u8 i;
        pCommVar->SendPrintf("  message: ");
        for(i=0;i<(l_u8DataIndex-3);i++)
        {
            pCommVar->SendPrintf("0x%02x,",l_u8TempData[i]);
        }
        pCommVar->SendPrintf("\r\n");
    }
    delay_ms(5);
    cec_cmd_send_to_device(l_u8DevID-1,l_u8Addr,l_u8Opcode,l_u8TempData,l_u8DataIndex-3,false);
    //pCommVar->SendPrintf("%s", pCommVar->pRecBuffer);
    return emCmdSucess;
}
#endif

//eFunc_UpdateMcu,
static emCmdFuncStaType mapp_CmdfuncUpdateMcu(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8ID;

    l_s8ID = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8ID>0)&&(l_s8ID<=1))
    {
        UpdateFunction(0,0,NULL,0);
        pCommVar->SendPrintf("please choose update file\r\n");
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_BaudrateMatch
static emCmdFuncStaType mapp_CmdfuncBaudrateMatch(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    pCommVar->SendPrintf("Baud rate matching succeeded\r\n");

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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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

//eFunc_SetOSDString,
static emCmdFuncStaType mapp_CmdfuncSetOSDString(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;
    u8 l_u8NumLen,i;
    char l_s8Str[15];
    u8 l_u8RealPort,l_u8Dev;

    l_u8Port = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    for(i=0;i<(pCommVar->pRecBuffer+pCommVar->u16RecCnt-l_pCmd);i++)
    {
        if(l_pCmd[i]==')')
        {
            l_u8NumLen = i;
            break;
        }
    }
    memset(l_s8Str,0,15);
    memcpy(l_s8Str,l_pCmd,l_u8NumLen);

    if((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
    {

        l_u8RealPort = cg_u8ChannelTable[l_u8Port-1][0];
        l_u8Dev = cg_u8ChannelTable[l_u8Port-1][3];
        app_MS1826SetOsdString(l_u8Dev,l_u8RealPort,l_u8NumLen,(u8 *)l_s8Str,pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1]);
        memset(pstBasicVar->u8RxOsdStr[l_u8Port-1],0,15);
        memcpy(pstBasicVar->u8RxOsdStr[l_u8Port-1], (u8 *)l_s8Str,l_u8NumLen);
        pstBasicVar->u8RxOsdStrLen[l_u8Port-1] = l_u8NumLen;
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("display input %d string: %s\r\n",l_u8Port,pstBasicVar->u8RxOsdStr[l_u8Port-1]);
        l_state = emCmdSucess;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetOSDString,
static emCmdFuncStaType mapp_CmdfuncGetOSDString(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;

    l_u8Port = (u8)GetNumofStr(l_pCmd,')',2);

    if(l_u8Port==0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            pCommVar->SendPrintf("display input %d string: %s\r\n",l_u8Port,pstBasicVar->u8RxOsdStr[i]);
        }
    }
    else if((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
    {
        pCommVar->SendPrintf("display input %d string: %s\r\n",l_u8Port,pstBasicVar->u8RxOsdStr[l_u8Port-1]);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetOSDStringEnable,
static emCmdFuncStaType mapp_CmdfuncSetOSDStringEnable(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;
    u8 l_u8Enable;
    u8 l_u8NumLen;
    u8 l_u8RealPort,l_u8Dev;

    l_u8Port = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Enable = (u8)GetNumofStr(l_pCmd,')',2);
    if(((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
        &&(l_u8Enable<=1))
    {
        if(l_u8Enable)
            pstBasicVar->bRxOsdStrEnable[l_u8Port-1] = TRUE;
        else
            pstBasicVar->bRxOsdStrEnable[l_u8Port-1] = FALSE;

        l_u8RealPort = cg_u8ChannelTable[l_u8Port-1][0];
        l_u8Dev = cg_u8ChannelTable[l_u8Port-1][3];
        app_MS1826SetUserOsdStringEnable(l_u8Dev,l_u8RealPort,pstBasicVar->bRxOsdStrEnable[l_u8Port-1]);
        pCommVar->SendPrintf("input %d osd: %s\r\n",l_u8Port,pstBasicVar->bRxOsdStrEnable[l_u8Port-1]?"enable":"disable");
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetOSDStringEnable,
static emCmdFuncStaType mapp_CmdfuncGetOSDStringEnable(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;

    l_u8Port = (u8)GetNumofStr(l_pCmd,')',2);

    if((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
    {
        pCommVar->SendPrintf("input %d osd: %s\r\n",l_u8Port,pstBasicVar->bRxOsdStrEnable[l_u8Port-1]?"enable":"disable");
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetOSDStringColor,
static emCmdFuncStaType mapp_CmdfuncSetOSDStringColor(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;
    u8 l_u8ColorID;
    u8 l_u8NumLen;
    u8 l_u8RealPort,l_u8Dev;

    l_u8Port = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8ColorID = (u8)GetNumofStr(l_pCmd,')',2);
    if(((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
        &&(l_u8ColorID>=1&&l_u8ColorID<=5))
    {
        switch(l_u8ColorID)
        {
            case emOsdColor_White:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0xFF;
                break;
            case emOsdColor_Red:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0x00;
                break;
            case emOsdColor_Green:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0x00;
                break;
            case emOsdColor_Blue:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0xFF;
                break;
            case emOsdColor_Black:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0x00;
                break;
            default:
                break;
        }
        l_u8RealPort = cg_u8ChannelTable[l_u8Port-1][0];
        l_u8Dev = cg_u8ChannelTable[l_u8Port-1][3];
        app_MS1826SetUserStringColor(l_u8Dev,l_u8RealPort,
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1],
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1],
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]);
        pCommVar->SendPrintf("input %d osd color: %s\r\n",l_u8Port,GetColorText(l_u8ColorID));
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetOSDStringColor,
static emCmdFuncStaType mapp_CmdfuncGetOSDStringColor(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;
    u8 l_u8ColorID;

    l_u8Port = (u8)GetNumofStr(l_pCmd,')',2);

    if(l_u8Port==0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            if((pstBasicVar->u8RxOsdStrColorR[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0xFF))
            {
                l_u8ColorID = emOsdColor_White;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0x00))
            {
                l_u8ColorID = emOsdColor_Red;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0x00))
            {
                l_u8ColorID = emOsdColor_Green;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0xFF))
            {
                l_u8ColorID = emOsdColor_Blue;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0x00))
            {
                l_u8ColorID = emOsdColor_Black;
            }

            pCommVar->SendPrintf("input %d osd color: %s\r\n",i+1,GetColorText(l_u8ColorID));
        }
    }
    else if((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
    {
        if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0xFF))
        {
            l_u8ColorID = emOsdColor_White;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0x00))
        {
            l_u8ColorID = emOsdColor_Red;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0x00))
        {
            l_u8ColorID = emOsdColor_Green;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0xFF))
        {
            l_u8ColorID = emOsdColor_Blue;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0x00))
        {
            l_u8ColorID = emOsdColor_Black;
        }

        pCommVar->SendPrintf("input %d osd color: %s\r\n",l_u8Port,GetColorText(l_u8ColorID));
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_SetOSDStringPosition,
static emCmdFuncStaType mapp_CmdfuncSetOSDStringPosition(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;
    u8 l_u8Pos;
    u8 l_u8NumLen;
    u8 l_u8RealPort,l_u8Dev;

    l_u8Port = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Pos = (u8)GetNumofStr(l_pCmd,')',2);
    if(((l_u8Port>=1)&&(l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER))
        &&((l_u8Pos>=1)&&(l_u8Pos<=4)))
    {
        switch(l_u8Pos)
        {
            case emOsdPos_LeftUp:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 100;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 100;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = FALSE;
                break;
            case emOsdPos_RightUp:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 1300;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 100;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = TRUE;
                break;
            case emOsdPos_LeftDown:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 100;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 930;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = FALSE;
                break;
            case emOsdPos_RightDown:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 1300;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 930;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = TRUE;
                break;
            default:
                break;
        }

        l_u8RealPort = cg_u8ChannelTable[l_u8Port-1][0];
        l_u8Dev = cg_u8ChannelTable[l_u8Port-1][3];
        app_MS1826SetUserOsdStringPosition(l_u8Dev,l_u8RealPort,pstBasicVar->u16RxOsdStrX[l_u8Port-1],pstBasicVar->u16RxOsdStrY[l_u8Port-1]);
        app_MS1826SetOsdString(l_u8Dev,l_u8RealPort,pstBasicVar->u8RxOsdStrLen[l_u8Port-1],(u8 *)pstBasicVar->u8RxOsdStr[l_u8Port-1],pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1]);
        pCommVar->SendPrintf("input %d osd position: %d\r\n",l_u8Port,l_u8Pos);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetOSDStringPosition,
static emCmdFuncStaType mapp_CmdfuncGetOSDStringPosition(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port;
    u8 l_u8Pos;

    l_u8Port = (u8)GetNumofStr(l_pCmd,')',2);

    if(l_u8Port==0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            if(pstBasicVar->u16RxOsdStrX[i]==100&&pstBasicVar->u16RxOsdStrY[i]==100)
            {
                l_u8Pos = emOsdPos_LeftUp;
            }
            else if(pstBasicVar->u16RxOsdStrX[i]==1300&&pstBasicVar->u16RxOsdStrY[i]==100)
            {
                l_u8Pos = emOsdPos_RightUp;
            }
            else if(pstBasicVar->u16RxOsdStrX[i]==100&&pstBasicVar->u16RxOsdStrY[i]==930)
            {
                l_u8Pos = emOsdPos_LeftDown;
            }
            else if(pstBasicVar->u16RxOsdStrX[i]==1300&&pstBasicVar->u16RxOsdStrY[i]==930)
            {
                l_u8Pos = emOsdPos_RightDown;
            }
            pCommVar->SendPrintf("input %d osd position: %d\r\n",i+1,l_u8Pos);
        }
    }
    else if((l_u8Port>=1)&&(l_u8Port<=16))
    {
        if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==100&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==100)
        {
            l_u8Pos = emOsdPos_LeftUp;
        }
        else if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==1300&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==100)
        {
            l_u8Pos = emOsdPos_RightUp;
        }
        else if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==100&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==930)
        {
            l_u8Pos = emOsdPos_LeftDown;
        }
        else if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==1300&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==930)
        {
            l_u8Pos = emOsdPos_RightDown;
        }
        pCommVar->SendPrintf("input %d osd position: %d\r\n",l_u8Port,l_u8Pos);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
u8 StringReplaceGb2312char(u8 *dest,u8 *src,char c,u8 len)
{
   u8 count = 0;
  
   while(len > 0)
   {
      if(*src <128)
      {
         *dest++ =  *src++;
      }
     else
     {   
        *dest++ = c;
         src+=2;
         len--; 
     }
     count++;
  }
   return count;
}

char FindcharOffset(char *buf,char Taildata,u8 len)
{    
    unsigned char bitcnt=0;
    while(*buf!=Taildata)
    {
        buf++;
        bitcnt++;
        if(bitcnt>len)
            return 0;
    }
   return        bitcnt;
}
//eFunc_SetOSD,
static emCmdFuncStaType mapp_CmdfuncSetOSD(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port,l_u8Pos,l_u8ColorID,l_u8Enable;
    u8 l_u8NumLen,i;
    char l_s8Str[16];
    u8 l_u8RealPort,l_u8Dev;
	u8 l_u8emptyStr[30];
	
    l_u8Port = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Enable = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Pos = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8ColorID = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    for(i=0;i<(pCommVar->pRecBuffer+pCommVar->u16RecCnt-l_pCmd);i++)
    {
        if(l_pCmd[i]==')')
        {
            l_u8NumLen = i;
            break;
        }
    }
	memset(l_u8emptyStr,0,30);
    memcpy(l_u8emptyStr,l_pCmd,l_u8NumLen);
    memset(l_s8Str,0,16);
	  u8 l_u8NumLen_z = 0;
	 for(u8 j=0; j<l_u8NumLen; j++)
    {
       if(l_u8emptyStr[j] == 0xA3) //'��'
       {
            l_s8Str[l_u8NumLen_z] = 0x29; //')'
            l_u8NumLen_z += 1;
       }
       else if (l_u8emptyStr[j] == 0xA9)
       {
            l_u8emptyStr[j] = 0;
       }
       else
       {
            l_s8Str[l_u8NumLen_z] = l_u8emptyStr[j];
            l_u8NumLen_z += 1;
       }
    }
	
  

    if((l_u8Port>=1&&l_u8Port<=_HDMI_INPUT_PORT_USED_NUMBER)
        &&(l_u8Pos>=1&&l_u8Pos<=4)
        &&(l_u8ColorID>=1&&l_u8ColorID<=5)
        &&(l_u8Enable<=1))
    {
        if(l_u8Enable)
            pstBasicVar->bRxOsdStrEnable[l_u8Port-1] = TRUE;
        else
            pstBasicVar->bRxOsdStrEnable[l_u8Port-1] = FALSE;

        switch(l_u8Pos)
        {
            case emOsdPos_LeftUp:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 100;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 100;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = FALSE;
                break;
            case emOsdPos_RightUp:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 1300;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 100;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = TRUE;
                break;
            case emOsdPos_LeftDown:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 100;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 930;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = FALSE;
                break;
            case emOsdPos_RightDown:
                pstBasicVar->u16RxOsdStrX[l_u8Port-1] = 1300;
                pstBasicVar->u16RxOsdStrY[l_u8Port-1] = 930;
                pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1] = TRUE;
                break;
            default:
                break;
        }

        switch(l_u8ColorID)
        {
            case emOsdColor_White:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0xFF;
                break;
            case emOsdColor_Red:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0x00;
                break;
            case emOsdColor_Green:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0xFF;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0x00;
                break;
            case emOsdColor_Blue:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0xFF;
                break;
            case emOsdColor_Black:
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1] = 0x00;
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1] = 0x00;
                break;
            default:
                break;
        }

        l_u8RealPort = cg_u8ChannelTable[l_u8Port-1][0];
        l_u8Dev = cg_u8ChannelTable[l_u8Port-1][3];
        app_MS1826SetUserOsdStringEnable(l_u8Dev,l_u8RealPort,pstBasicVar->bRxOsdStrEnable[l_u8Port-1]);
        app_MS1826SetUserStringColor(l_u8Dev,l_u8RealPort,
                pstBasicVar->u8RxOsdStrColorR[l_u8Port-1],
                pstBasicVar->u8RxOsdStrColorG[l_u8Port-1],
                pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]);
        app_MS1826SetUserOsdStringPosition(l_u8Dev,l_u8RealPort,pstBasicVar->u16RxOsdStrX[l_u8Port-1],pstBasicVar->u16RxOsdStrY[l_u8Port-1]);
        app_MS1826SetOsdString(l_u8Dev,l_u8RealPort,l_u8NumLen_z,(u8 *)l_s8Str,pstBasicVar->bRxOsdStrIsAlignRight[l_u8Port-1]);
        memset(pstBasicVar->u8RxOsdStr[l_u8Port-1],0,15);
        memcpy(pstBasicVar->u8RxOsdStr[l_u8Port-1], (u8 *)l_s8Str,l_u8NumLen_z);
        pstBasicVar->u8RxOsdStrLen[l_u8Port-1] = l_u8NumLen_z;
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("input %d osd:\r\n"
                            "%s,\r\n"
                            "pos:%d,\r\n"
                            "color:%s,\r\n"
                            "string:%s,\r\n",
                            l_u8Port,
                            pstBasicVar->bRxOsdStrEnable[l_u8Port-1]?"enable":"disable",
                            l_u8Pos,
                            GetColorText(l_u8ColorID),
                            pstBasicVar->u8RxOsdStr[l_u8Port-1]);
        l_state = emCmdSucess;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_GetOSD,
static emCmdFuncStaType mapp_CmdfuncGetOSD(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Port,l_u8Pos,l_u8ColorID;

    l_u8Port = (u8)GetNumofStr(l_pCmd,')',2);

    if(l_u8Port==0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            if(pstBasicVar->u16RxOsdStrX[i]==100&&pstBasicVar->u16RxOsdStrY[i]==100)
            {
                l_u8Pos = emOsdPos_LeftUp;
            }
            else if(pstBasicVar->u16RxOsdStrX[i]==1300&&pstBasicVar->u16RxOsdStrY[i]==100)
            {
                l_u8Pos = emOsdPos_RightUp;
            }
            else if(pstBasicVar->u16RxOsdStrX[i]==100&&pstBasicVar->u16RxOsdStrY[i]==930)
            {
                l_u8Pos = emOsdPos_LeftDown;
            }
            else if(pstBasicVar->u16RxOsdStrX[i]==1300&&pstBasicVar->u16RxOsdStrY[i]==930)
            {
                l_u8Pos = emOsdPos_RightDown;
            }

            if((pstBasicVar->u8RxOsdStrColorR[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0xFF))
            {
                l_u8ColorID = emOsdColor_White;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0x00))
            {
                l_u8ColorID = emOsdColor_Red;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0xFF)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0x00))
            {
                l_u8ColorID = emOsdColor_Green;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0xFF))
            {
                l_u8ColorID = emOsdColor_Blue;
            }
            else if((pstBasicVar->u8RxOsdStrColorR[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorG[i]==0x00)
                &&(pstBasicVar->u8RxOsdStrColorB[i]==0x00))
            {
                l_u8ColorID = emOsdColor_Black;
            }
            pCommVar->SendPrintf("input %d osd:\r\n"
                                "%s,\r\n"
                                "pos:%d,\r\n"
                                "color:%s,\r\n"
                                "string:%s,\r\n",
                                i+1,
                                pstBasicVar->bRxOsdStrEnable[i]?"enable":"disable",
                                l_u8Pos,
                                GetColorText(l_u8ColorID),
                                pstBasicVar->u8RxOsdStr[i]);
        }
    }
    else if((l_u8Port>=1)&&(l_u8Port<=16))
    {
        if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==100&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==100)
        {
            l_u8Pos = emOsdPos_LeftUp;
        }
        else if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==1300&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==100)
        {
            l_u8Pos = emOsdPos_RightUp;
        }
        else if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==100&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==930)
        {
            l_u8Pos = emOsdPos_LeftDown;
        }
        else if(pstBasicVar->u16RxOsdStrX[l_u8Port-1]==1300&&pstBasicVar->u16RxOsdStrY[l_u8Port-1]==930)
        {
            l_u8Pos = emOsdPos_RightDown;
        }
        if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0xFF))
        {
            l_u8ColorID = emOsdColor_White;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0x00))
        {
            l_u8ColorID = emOsdColor_Red;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0xFF)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0x00))
        {
            l_u8ColorID = emOsdColor_Green;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0xFF))
        {
            l_u8ColorID = emOsdColor_Blue;
        }
        else if((pstBasicVar->u8RxOsdStrColorR[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorG[l_u8Port-1]==0x00)
            &&(pstBasicVar->u8RxOsdStrColorB[l_u8Port-1]==0x00))
        {
            l_u8ColorID = emOsdColor_Black;
        }
        pCommVar->SendPrintf("input %d osd:\r\n"
                            "%s,\r\n"
                            "pos:%d,\r\n"
                            "color:%s,\r\n"
                            "string:%s,\r\n",
                            l_u8Port,
                            pstBasicVar->bRxOsdStrEnable[l_u8Port-1]?"enable":"disable",
                            l_u8Pos,
                            GetColorText(l_u8ColorID),
                            pstBasicVar->u8RxOsdStr[l_u8Port-1]);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetMS1826AVPort,
static emCmdFuncStaType mapp_CmdfuncSetMS1826AVPort(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
//    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
//    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    u8 i=0,j=0,l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8Output[Def_outport_num],l_s8Temp;
    u8 l_u8RealRxPort,l_u8RealTxPort,l_u8Dev;

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
            pCommVar->SendPrintf("av %d to %d\r\n",l_s8Input,i+1);
            l_u8RealRxPort = cg_u8ChannelTable[l_s8Input-1][0];
            l_u8RealTxPort = cg_u8ChannelTable[i][0];
            l_u8Dev = cg_u8ChannelTable[l_s8Input-1][3];
            app_MS1826Switch(l_u8Dev,l_u8RealRxPort, l_u8RealTxPort);
        }
    }
//    SetUserData_HdmiSel();

    return l_state;

}
//eFunc_GetMS1826AVPort,
static emCmdFuncStaType mapp_CmdfuncGetMS1826AVPort(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    return l_state;
}

//eFunc_SetSplitMode,
static emCmdFuncStaType mapp_CmdfuncSetSplitMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Group,l_u8Mode;
    u8 l_u8NumLen,i,l_u8Dev;

    l_u8Group = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_u8Mode = (u8)GetNumofStr(l_pCmd,')',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',2);

    if((l_u8Group<=2)
        &&(l_u8Mode<=16))
    {
        if(l_u8Group==0)
        {
            for(i=0;i<2;i++)
            {
                l_u8Dev = cg_u8ChannelTable[i*4][3];
                app_MS1826SetSplitterMode(l_u8Dev,l_u8Mode);
                pstBasicVar->u8SplitterMode[i] = l_u8Mode;
                SetUserData_SpecBasicVar();
                pCommVar->SendPrintf("group %d splitter mode is %d\r\n",
                                    i+1,
                                    pstBasicVar->u8SplitterMode[i]);
            }
        }
        else
        {
            l_u8Dev = cg_u8ChannelTable[(l_u8Group-1)*4][3];
            app_MS1826SetSplitterMode(l_u8Dev,l_u8Mode);
            pstBasicVar->u8SplitterMode[l_u8Group-1] = l_u8Mode;
            SetUserData_SpecBasicVar();
            pCommVar->SendPrintf("group %d splitter mode is %d\r\n",
                                l_u8Group,
                                pstBasicVar->u8SplitterMode[l_u8Group-1]);
        }
        l_state = emCmdSucess;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetSplitMode,
static emCmdFuncStaType mapp_CmdfuncGetSplitMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Group,i;

    l_u8Group = (u8)GetNumofStr(l_pCmd,')',2);

    if(l_u8Group<=2)
    {
        if(l_u8Group==0)
        {
            for(i=0;i<2;i++)
            {
                pCommVar->SendPrintf("group %d splitter mode is %d\r\n",
                                    i+1,
                                    pstBasicVar->u8SplitterMode[i]);
            }
        }
        else
        {
            pCommVar->SendPrintf("group %d splitter mode is %d\r\n",
                                l_u8Group,
                                pstBasicVar->u8SplitterMode[l_u8Group-1]);
        }
    }
    return l_state;
}

//eFunc_SetSplitUpdate
static emCmdFuncStaType mapp_CmdfuncSetSplitUpdate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8Group,l_u8Path=0;
    u8 l_u8NumLen,i;

    l_u8Group = (u8)GetNumofStr(l_pCmd,',',2);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',2);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    for(i=0;i<3;i++)
    {
        if((l_pCmd[i]!=',')&&(l_pCmd[i]!=')'))
        {
            if(l_pCmd[i]=='0'&&(l_pCmd[i+1]=='x'||(l_pCmd[i+1]=='X')))
                i += 2;
            if(HexAndAsciiChange(l_pCmd[i],0)==l_pCmd[i])
                return emCmdError;
            l_u8Path = (l_u8Path<<4)+HexAndAsciiChange(l_pCmd[i],0);
        }
        if(l_pCmd[i]==')')
        {
            break;
        }
    }

    if((l_u8Group<=4)&&(l_u8Path<=0x0F))
    {
        if(l_u8Group==0)
        {
            for(i=0;i<4;i++)
            {
                app_MS1826SetSplitterUpdate(i,l_u8Path);
                pCommVar->SendPrintf("group %d splitter update: 0x%x\r\n",i+1,l_u8Path);
            }
        }
        else
        {
            app_MS1826SetSplitterUpdate(l_u8Group-1,l_u8Path);
            pCommVar->SendPrintf("group %d splitter update: 0x%x\r\n",l_u8Group,l_u8Path);
        }
        l_state = emCmdSucess;
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}
//eFunc_SetSplitAVSwitch,
static emCmdFuncStaType mapp_CmdfuncSetSplitAVSwitch(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8Output,l_s8Group;
    u8 l_u8RealRxPort,l_u8RealTxPort;

    l_s8Group = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;

    l_s8Input = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,')',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;

    if(((l_s8Input<=4)&&(l_s8Input>0))
        &&((l_s8Output<=4)&&(l_s8Output>0))
        &&((l_s8Group<=2)&&(l_s8Group>0)))
    {
        if(pstBasicVar->u8SplitterMode[(l_s8Group-1)])
        {
#if 0
            u8 l_u8WinId;

            // change old source
            for(l_u8WinId = 0; l_u8WinId < 4; l_u8WinId++)
            {
                if(pstBasicVar->u8SplitterWinSrc[l_s8Group-1][l_u8WinId] == (l_s8Input-1)%4)
                {
                    pstBasicVar->u8SplitterWinSrc[l_s8Group-1][l_u8WinId] = pstBasicVar->u8SplitterWinSrc[l_s8Group-1][(l_s8Output-1)%4];
                    break;
                }
            }
#endif
            pstBasicVar->u8SplitterWinSrc[l_s8Group-1][(l_s8Output-1)%4]  = (l_s8Input-1)%4;
            pCommVar->SendPrintf("group %d splitter video map:\r\n"
                                 "A B C D\r\n"
                                 "%d %d %d %d;\r\n",l_s8Group,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][0]+1,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][1]+1,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][2]+1,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][3]+1);
            l_u8RealRxPort = cg_u8ChannelTable[(l_s8Input-1)*l_s8Group][1];
            l_u8RealTxPort = cg_u8ChannelTable[(l_s8Output-1)*l_s8Group][0];
            app_MS1826Switch(l_s8Group-1,l_u8RealRxPort, l_u8RealTxPort);
            SetUserData_SpecBasicVar();
        }
        else
        {
            pCommVar->SendPrintf("group %d non splitter mode\r\n",l_s8Group);
        }
    }
    else
    {
        l_state = emCmdOutOfRange;
    }

    return l_state;

}
//eFunc_GetSplitAVSwitch,
static emCmdFuncStaType mapp_CmdfuncGetSplitAVSwitch(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Group;

    l_s8Group = (char)GetNumofStr(l_pCmd,')',3);

    if(((l_s8Group<=2)))
    {
        if(l_s8Group == 0)
        {
            pCommVar->SendPrintf("group %d splitter video map:\r\n"
                                 "A B C D\r\n"
                                 "%d %d %d %d;\r\n",1,
                                 pstBasicVar->u8SplitterWinSrc[0][0]+1,
                                 pstBasicVar->u8SplitterWinSrc[0][1]+1,
                                 pstBasicVar->u8SplitterWinSrc[0][2]+1,
                                 pstBasicVar->u8SplitterWinSrc[0][3]+1);
            pCommVar->SendPrintf("group %d splitter video map:\r\n"
                                 "A B C D\r\n"
                                 "%d %d %d %d;\r\n",2,
                                 pstBasicVar->u8SplitterWinSrc[1][0]+1,
                                 pstBasicVar->u8SplitterWinSrc[1][1]+1,
                                 pstBasicVar->u8SplitterWinSrc[1][2]+1,
                                 pstBasicVar->u8SplitterWinSrc[1][3]+1);
        }
        else
        {
            pCommVar->SendPrintf("group %d splitter video map:\r\n"
                                 "A B C D\r\n"
                                 "%d %d %d %d;\r\n",l_s8Group,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][0]+1,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][1]+1,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][2]+1,
                                 pstBasicVar->u8SplitterWinSrc[l_s8Group-1][3]+1);
        }
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_WriteChipRegisterData,
// (wreg,1,001,10)
static emCmdFuncStaType mapp_CmdfuncWriteChipRegisterData(pstUartCom_t pCommVar,u8 cmdnum)
{
    #if 1
    u8 l_u8TempAddr = 0;
    u16 l_u16TempOffset = 0;
    u8 l_u8TempData = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd); l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==0))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8TempAddr = (l_u8TempAddr<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==1))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u16TempOffset = (l_u16TempOffset<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==2))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8TempData = (l_u8TempData<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else
        {
            l_u8DataIndex++;
        }
        if((pCommVar->pRecBuffer[l_u8Index]==')'))
        {
            break;
        }
    }

    app_ms1826_reg_write(l_u8TempAddr,l_u16TempOffset,l_u8TempData);
    pCommVar->SendPrintf("Write DEV[0x%02x] REG[0x%04x] DATA[0x%02x]\r\n", l_u8TempAddr,l_u16TempOffset,l_u8TempData);
    #endif
    return emCmdSucess;
}

//eFunc_ReadChipRegisterData,
// (rreg,1,001)
static emCmdFuncStaType mapp_CmdfuncReadChipRegisterData(pstUartCom_t pCommVar,u8 cmdnum)
{
    #if 1
    u8 l_u8TempAddr = 0;
    u16 l_u16TempOffset = 0;
    u8 l_u8TempData = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;

    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd); l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==0))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8TempAddr = (l_u8TempAddr<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!=')')&&(l_u8DataIndex==1))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u16TempOffset = (l_u16TempOffset<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else
        {
            l_u8DataIndex++;
        }

        if((pCommVar->pRecBuffer[l_u8Index]==')'))
        {
            break;
        }
    }

    app_ms1826_reg_read(l_u8TempAddr,l_u16TempOffset,&l_u8TempData);
    pCommVar->SendPrintf("Read DEV[0x%02x] REG[0x%04x] DATA[0x%02x]\r\n", l_u8TempAddr,l_u16TempOffset,l_u8TempData);
    #endif
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
//@Input:   u8 flag ��־ , char *cmd ָ�
//@Output: l_bStatus �Ƿ�ִ�гɹ�
//@Description: ���ݹ��ܱ�־��ִ����Ӧ�Ĺ��ܺ���
//              Ԥ���˱�־λ�������޸ĵĿռ�
//***********************************************************
emCmdFuncStaType mapp_CommandFunctionProcess(u8 cmdnum,u8 flag,pstUartCom_t pCommVar)
{
    emCmdFuncStaType l_bStatus=emCmdError;

    UNUSED(flag);
    //UNUSED(cmdnum);
    switch( mapp_GetCommandFunctionID())
    {
        case eFunc_BaudrateMatch:
            {
                l_bStatus = mapp_CmdfuncBaudrateMatch(pCommVar,cmdnum);
            }
            break;
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
        case eFunc_SetFreezeState:
            {
                l_bStatus = mapp_CmdfuncSetFreezeState(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetFreezeState:
            {
                l_bStatus = mapp_CmdfuncGetFreezeState(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetBrightness:
            {
                l_bStatus = mapp_CmdfuncSetBrightness(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetBrightness:
            {
                l_bStatus = mapp_CmdfuncGetBrightness(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetContrast:
            {
                l_bStatus = mapp_CmdfuncSetContrast(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetContrast:
            {
                l_bStatus = mapp_CmdfuncGetContrast(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSaturation:
            {
                l_bStatus = mapp_CmdfuncSetSaturation(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetSaturation:
            {
                l_bStatus = mapp_CmdfuncGetSaturation(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetHue:
            {
                l_bStatus = mapp_CmdfuncSetHue(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetHue:
            {
                l_bStatus = mapp_CmdfuncGetHue(pCommVar,cmdnum);
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
        case eFunc_GetTxEdidData:
            {
                l_bStatus = mapp_CmdfuncGetTxEdidData(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetTxTiming:
            {
                l_bStatus = mapp_CmdfuncSetTxTiming(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetTxPattern:
            {
                l_bStatus = mapp_CmdfuncSetTxPattern(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxPattern:
            {
                l_bStatus = mapp_CmdfuncGetTxPattern(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetColor:
            {
                l_bStatus = mapp_CmdfuncSetColor(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetColor:
            {
                l_bStatus = mapp_CmdfuncGetColor(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetDebugEn:
            {
                l_bStatus = mapp_CmdfuncSetDebugEn(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplitConfig:
            {
                l_bStatus = mapp_CmdfuncSetSplitConfig(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplicerConfig:
            {
                l_bStatus = mapp_CmdfuncSetSplicerConfig(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetSplicerConfig:
            {
                l_bStatus = mapp_CmdfuncGetSplicerConfig(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetFrameLock:
            {
                l_bStatus = mapp_CmdfuncSetFrameLock(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplicerMode:
            {
                l_bStatus = mapp_CmdfuncSetSplicerMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetSplicerMode:
            {
                l_bStatus = mapp_CmdfuncGetSplicerMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplicerBorder:
            {
                l_bStatus = mapp_CmdfuncSetSplicerBorder(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetSplicerBorder:
            {
                l_bStatus = mapp_CmdfuncGetSplicerBorder(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplicerPosition:
            {
                l_bStatus = mapp_CmdfuncSetSplicerPosition(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetSplicerPosition:
            {
                l_bStatus = mapp_CmdfuncGetSplicerPosition(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSplicerClear:
            {
                l_bStatus = mapp_CmdfuncSetSplicerClear(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetMs1826Hold:
            {
                l_bStatus = mapp_CmdfuncSetMs1826Hold(pCommVar,cmdnum);
            }
            break;
        case eFunc_BypassSwitchPort:
            {
                l_bStatus = mapp_CmdfuncBypassSwitchPort(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetVideoMute:
            {
                l_bStatus = mapp_CmdfuncSetVideoMute(pCommVar,cmdnum);
            }
            break;
        case eFunc_ModeSwitch:
            {
                l_bStatus = mapp_CmdfuncModeSwitch(pCommVar,cmdnum);
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
        case eFunc_ScenePolling:
            {
                l_bStatus = mapp_CmdfuncScenePolling(pCommVar,cmdnum);
            }
            break;
        case eFunc_ScenePollingTime:
            {
                l_bStatus = mapp_CmdfuncScenePollingTime(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetScenePollingTime:
            {
                l_bStatus = mapp_CmdfuncGetScenePollingTime(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetScenePollingSta:
            {
                l_bStatus = mapp_CmdfuncGetScenePollingSta(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetPollingList:
            {
                l_bStatus = mapp_CmdfuncSetPollingList(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetPollingList:
            {
                l_bStatus = mapp_CmdfuncGetPollingList(pCommVar,cmdnum);
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
        #if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
        case  eFunc_SendUserCecCmd:
            {
                l_bStatus = mapp_CmdfuncSendUserCecCmd(pCommVar,cmdnum);
            }
            break;
        #endif
        case eFunc_UpdateMcu:
            {
                l_bStatus = mapp_CmdfuncUpdateMcu(pCommVar,cmdnum);
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
        case eFunc_SetOSDString:
            {
                l_bStatus = mapp_CmdfuncSetOSDString(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOSDString:
            {
                l_bStatus = mapp_CmdfuncGetOSDString(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetOSDStringEnable:
            {
                l_bStatus = mapp_CmdfuncSetOSDStringEnable(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOSDStringEnable:
            {
                l_bStatus = mapp_CmdfuncGetOSDStringEnable(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetOSDStringColor:
            {
                l_bStatus = mapp_CmdfuncSetOSDStringColor(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOSDStringColor:
            {
                l_bStatus = mapp_CmdfuncGetOSDStringColor(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetOSDStringPosition:
            {
                l_bStatus = mapp_CmdfuncSetOSDStringPosition(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOSDStringPosition:
            {
                l_bStatus = mapp_CmdfuncGetOSDStringPosition(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetOSD:
            {
                l_bStatus = mapp_CmdfuncSetOSD(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOSD:
            {
                l_bStatus = mapp_CmdfuncGetOSD(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetMS1826AVPort:
            {
                l_bStatus = mapp_CmdfuncSetMS1826AVPort(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetMS1826AVPort:
            {
                l_bStatus = mapp_CmdfuncGetMS1826AVPort(pCommVar,cmdnum);
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
        case eFunc_SetSplitUpdate:
            {
                l_bStatus = mapp_CmdfuncSetSplitUpdate(pCommVar,cmdnum);
            }
            break;
        case  eFunc_SetSplitAVSwitch:
            {
                l_bStatus = mapp_CmdfuncSetSplitAVSwitch(pCommVar,cmdnum);
            }
            break;
        case  eFunc_GetSplitAVSwitch:
            {
                l_bStatus = mapp_CmdfuncGetSplitAVSwitch(pCommVar,cmdnum);
            }
            break;
        case  eFunc_WriteChipRegisterData:
            {
                l_bStatus = mapp_CmdfuncWriteChipRegisterData(pCommVar,cmdnum);
            }
            break;
        case  eFunc_ReadChipRegisterData:
            {
                l_bStatus = mapp_CmdfuncReadChipRegisterData(pCommVar,cmdnum);
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
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
                        if(*(l_ps8StartStr+strlen(cmd_list[i].ucmd)-1) != ')')        // ָ�����')'��β
                        {
                            if(cmd_list[i].valuelen==0)
                                l_ps8EndStr = FindChar(l_ps8StartStr+strlen(cmd_list[i].ucmd),')',Cmdlength(i)-1);    // delet end char self
                            else
                                l_ps8EndStr = FindChar(l_ps8StartStr+strlen(cmd_list[i].ucmd),')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8StartStr/*+strlen(cmd_list[i].ucmd)*/+1; // ָ���β���Ͻ���������
                        }
                        else
                        {
                            l_ps8EndStr = l_ps8StartStr+strlen(cmd_list[i].ucmd);
                            //l_ps8EndStr = FindChar(l_ps8StartStr+strlen(cmd_list[i].ucmd),')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8StartStr/*+strlen(cmd_list[i].ucmd)*/;
                        }
                        memcpy(pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].s8MultiCmd,l_ps8StartStr/*-strlen(cmd_list[i].ucmd)*/,pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);
                        memset(l_ps8StartStr,'0',pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter);   // ������ʶ���ָ��
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
                        if(*(l_ps8tempStr-1) != ')')        // ָ�����')'��β
                        {
                            if(cmd_list[i].valuelen==0)
                                l_ps8EndStr = FindChar(l_ps8tempStr,')',Cmdlength(i)-1);    // delet end char self
                            else
                                l_ps8EndStr = FindChar(l_ps8tempStr,')',Cmdlength(i));
                            pCommVar->stMultiCmdType[pCommVar->u8MatchCmdCounter].u8CmdCounter = (char*)l_ps8EndStr-l_ps8tempStr+strlen(cmd_list[i].ucmd)+1; // ָ���β���Ͻ���������
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
                    memset(l_ps8tempStr-strlen(cmd_list[i].ucmd),'0',strlen(cmd_list[i].ucmd));   // ������ʶ���ָ��
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


        if(pCommVar->u8MatchCmdCounter)
            l_bStatus = emCmdNone;
        else
        {
        #if 1
            if(!pstBasicVar->bPowerOnSta)
            {
                if(( mapp_GetCommandFunctionID() != eFunc_PowerWakeup)
                    &&( mapp_GetCommandFunctionID() != eFunc_GetPowerState)
                    &&( mapp_GetCommandFunctionID() != eFunc_GetLinkInfo)
                    &&( mapp_GetCommandFunctionID() != eFunc_FactoryReset)
                    )
                {
                    mapp_SetCommandFunctionID(eFunc_NoUse);
                    return;
                }
            }
        #endif
            l_bStatus = mapp_CommandFunctionProcess(l_u8Cmdnumber,l_u8Flag, pCommVar);
        }
    }

    if( l_bStatus == emCmdError )
    {
        pstUartCom_t                pCom1Var = &g_stUart1CommVar;
        pstUartCom_t                pCom2Var = &g_stUart2CommVar;
        pstUartCom_t                pCom3Var = &g_stUart5CommVar;

        if(pstBasicVar->u8UartMode==0)
            pCommVar->SendPrintf("err\r\n");
        else
        {
            if((pCommVar==pCom1Var)||(pCommVar==pCom3Var))
                pCom2Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
            else if(pCommVar==pCom2Var)
                pCom1Var->SendBytes((uint8_t*)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i;
    char *l_ps8Cmdbuf = NULL;  //g_CMMFCmd;
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
            if(strncmp(l_ps8Cmdbuf,cmd_list[i].ucmd,strlen(cmd_list[i].ucmd))==NULL)    // ʶ������
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
    #if 1
    if(!pstBasicVar->bPowerOnSta)
    {
        if(( mapp_GetCommandFunctionID() != eFunc_PowerWakeup)
            &&( mapp_GetCommandFunctionID() != eFunc_GetPowerState)
            &&( mapp_GetCommandFunctionID() != eFunc_GetLinkInfo)
            &&( mapp_GetCommandFunctionID() != eFunc_FactoryReset)
            )
        {
            mapp_SetCommandFunctionID(eFunc_NoUse);
            pCommVar->bIsMultiCmdDetect = FALSE;
            sl_u8CmdIndex = 0;
            pCommVar->u8MatchCmdCounter= 0;
            memset(pCommVar->stMultiCmdType,0,sizeof(stMultiCmdType_t)*MaxCmdlist);     // clean buffer
            return;
        }
    }
    #endif
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
	pstProjectSpecVar_t			pstSpecVar = &stMUH0808SSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
            app_MS1826SetStandby(2,FALSE);
            app_MS1826SetStandby(3,FALSE);
            SetUserData_SpecBasicVar();

            LCD_LineRefresh((char *)LcdNone,LINE1);
            LCD_LineRefresh((char *)LcdNone,LINE2);
            LCD_LineRefresh((char *)LcdNone,LINE3);
            LCD_LineRefresh((char *)LcdNone,LINE4);
            LCD_RecoverToDefault(DEF_LCD_DISPLAY_TIME);
            mapp_LCDBacklightEnable();
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
            app_MS1826SetStandby(2,TRUE);
            app_MS1826SetStandby(3,TRUE);
            SetUserData_SpecBasicVar();
            LCD_LineRefresh((char *)LcdNone,LINE1);
            LCD_LineRefresh((char *)LcdNone,LINE2);
            LCD_LineRefresh((char *)LcdNone,LINE3);
            LCD_LineRefresh((char *)LcdNone,LINE4);
        }
    }
}

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar)
{
    BYTE CostumEdidbuffer[256] = {0};
    u16 i;
    if(StartEdidCostumFlag)
    {
        pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
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
void mapp_linkStateCheck(void)
{
    pstUartCom_t 			pUartCommVar2 = &g_stUart2CommVar;
    pstProjectSpecVar_t         pstSpecVar = &stMUH0808SSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 i,sta=0;
    bool InputSourceStateChanges=FALSE;
    u8   InputSourceValue=0,l_u8Dev;
    stTimingTable_t l_stRxTming;
    if(!mapp_MUH0808S_PlatformTimerExpired(emUserTimer_ChecklinkState))
        return;
    mapp_MUH0808S_PlatformTimerSet(emUserTimer_ChecklinkState,1000);
    for(i=0;i<8;i++)
    {
		if(outputchannl[i]!=g_stLinkSta.u8OutLinkSta[i])
		{
			outputchannl[i]=g_stLinkSta.u8OutLinkSta[i];
			sta=1;
	    }
        if(inputchannl[i]!=g_stLinkSta.u8InLinkSta[i])
        {
			sta=1;
            inputchannl[i]=g_stLinkSta.u8InLinkSta[i];
            InputSourceStateChanges=TRUE;
            InputSourceValue=i;
        }

    }
		if(sta)
		{
            QueryTheResCnt++;
            if(QueryTheResCnt<3) return;
            pUartCommVar2->SendPrintf("ch  1 2 3 4 5 6 7 8\r\n");
            pUartCommVar2->SendPrintf("in  %c %c %c %c %c %c %c %c\r\n",
            g_stLinkSta.u8InLinkSta[0]?'Y':'N',
            g_stLinkSta.u8InLinkSta[1]?'Y':'N',
            g_stLinkSta.u8InLinkSta[2]?'Y':'N',
            g_stLinkSta.u8InLinkSta[3]?'Y':'N',
            g_stLinkSta.u8InLinkSta[4]?'Y':'N',
            g_stLinkSta.u8InLinkSta[5]?'Y':'N',
            g_stLinkSta.u8InLinkSta[6]?'Y':'N',
            g_stLinkSta.u8InLinkSta[7]?'Y':'N'
            );
           pUartCommVar2->SendPrintf("out %c %c %c %c %c %c %c %c\r\n",
            g_stLinkSta.u8OutLinkSta[0]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[1]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[2]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[3]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[4]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[5]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[6]?'Y':'N',
            g_stLinkSta.u8OutLinkSta[7]?'Y':'N'
            );
            pUartCommVar2->SendPrintf("ok\r\n");
            if(InputSourceStateChanges)
            {
                delay_ms(500);
                if(g_stLinkSta.u8InLinkSta[InputSourceValue])
                {
                    l_u8Dev = cg_u8ChannelTable[InputSourceValue][3];
                    app_ms1826_get_rx_timing(l_u8Dev,cg_u8ChannelTable[InputSourceValue][0],&l_stRxTming);
                    pUartCommVar2->SendPrintf("in %d res %dx%d%s@%dHz ok\r\n",InputSourceValue+1,l_stRxTming.u16HActive,
                                                          l_stRxTming.u8Interlace==INTR?(l_stRxTming.u16VActive*2):(l_stRxTming.u16VActive),
                                                          l_stRxTming.u8Interlace==INTR?"I":"",
                                                          l_stRxTming.u8VClk);
                }
                else pUartCommVar2->SendPrintf("in %d res 0x0@0Hz ok\r\n",InputSourceValue+1);
            }
        }
		

    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(g_stLinkSta.bInTimingChangeFlag[i])
        {
            g_stLinkSta.bInTimingChangeFlag[i] = FALSE;
            USER_printf("in %d res %dx%d%s@%dHz\r\n",i+1,g_stLinkSta.stInTming[i].u16HActive,
                        g_stLinkSta.stInTming[i].u8Interlace==INTR?(g_stLinkSta.stInTming[i].u16VActive*2):(g_stLinkSta.stInTming[i].u16VActive),
                        g_stLinkSta.stInTming[i].u8Interlace==INTR?"I":"",
                        g_stLinkSta.stInTming[i].u8VClk);
            USER_printf("ok\r\n");
        }
    }
		
}
