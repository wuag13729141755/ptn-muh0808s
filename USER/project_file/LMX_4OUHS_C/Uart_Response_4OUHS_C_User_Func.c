#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"
#include "ms1826_app_single_mode.h"

#include "../USER/project_file/LMX_4OUHS_C/mapp_UartCmdList_4OUHS_C.h"

#define BRIGHTNESS_VALUE_MAX    (100)
#define CONTRAST_VALUE_MAX      (100)
#define SATURATION_VALUE_MAX    (100)
#define HUE_VALUE_MAX           (100)

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

//eFunc_GetVersion,
static emCmdFuncStaType mapp_CmdfuncGetVersion(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    mapp_UserData_HdmiSelInit();
    SetUserData_HdmiSel();
    delay_ms(100);
    LMX_4OUHS_C_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();
    delay_ms(100);

    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();
    delay_ms(500);
    #if _ENABLE_SYSTEM_RESET_DELAY_TIME
    mapp_SetSysResetModeDelayTime(1500);
    #endif

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

//eFunc_SwitchInputPort,
static emCmdFuncStaType mapp_CmdfuncSwitchInputPort(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
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
            pstPrjSaveVar->stHdmiSel.u8SwChannel[i] = l_s8Input-1;
            j++;
            pCommVar->SendPrintf("out %d in %d\r\n",i+1,l_s8Input);
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][1]+1;
            app_MS1826Switch(0,l_u8RealPort-1, i);
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

//eFunc_SetTxResolution,
static emCmdFuncStaType mapp_CmdfuncSetTxResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8ResText[20];
    u8 l_u8NumLen=0;
    signed char l_s8Output,l_s8ResID;
    u8 l_u8RealPort;

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
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                app_MS1826TxResolutionChange(0,l_u8RealPort-1,l_s8ResID-1);
                pstBasicVar->u8TxResolutionID[i] = l_s8ResID-1;
                pCommVar->SendPrintf("out %d res %s\r\n",i+1,l_s8ResText);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;
            app_MS1826TxResolutionChange(0,l_u8RealPort-1,l_s8ResID-1);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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

    l_s8Input = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8Input<=4)
    {
        if(l_s8Input == 0)
        {
            u8 i;
            for(i=0;i<4;i++)
            {
                app_ms1826_get_rx_timing(0,cg_u8ChannelTable[i][0],&l_stRxTming);
                pCommVar->SendPrintf("in %d res %dx%d%s@%dHz\r\n",i+1,l_stRxTming.u16HActive,
                                                                          l_stRxTming.u8Interlace==INTR?(l_stRxTming.u16VActive*2):(l_stRxTming.u16VActive),
                                                                          l_stRxTming.u8Interlace==INTR?"I":"",
                                                                          l_stRxTming.u8VClk);
            }
        }
        else
        {
            app_ms1826_get_rx_timing(0,cg_u8ChannelTable[l_s8Input-1][0],&l_stRxTming);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8HdcpMode;
    u8 l_u8RealPort;

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
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                pstBasicVar->u8TxHdcpMode[i] = l_s8HdcpMode;
                pCommVar->SendPrintf("out %d hdcp %s\r\n",i+1,l_s8HdcpMode?"on":"off");

                app_MS1826TxHdcpEnable(0,l_u8RealPort-1,pstBasicVar->u8TxHdcpMode[i]);
            }
        }
        else
        {
            pstBasicVar->u8TxHdcpMode[l_s8Output-1] = l_s8HdcpMode;
            pCommVar->SendPrintf("out %d hdcp %s\r\n",l_s8Output,l_s8HdcpMode?"on":"off");
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;

            app_MS1826TxHdcpEnable(0,l_u8RealPort-1,pstBasicVar->u8TxHdcpMode[l_s8Output-1]);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8TxFormat;
    u8 l_u8RealPort;

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
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                pstBasicVar->u8TxFormatSta[i] = l_s8TxFormat;
                pCommVar->SendPrintf("output hdmi %d format is %s\r\n",i+1,l_s8TxFormat?"hdmi":"dvi");
                app_MS1826TxConfig(0,l_u8RealPort-1,pstBasicVar->u8TxFormatSta[i]);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;
            pstBasicVar->u8TxFormatSta[l_s8Output-1] = l_s8TxFormat;
            pCommVar->SendPrintf("output hdmi %d format is %s\r\n",l_s8Output,l_s8TxFormat?"hdmi":"dvi");
            app_MS1826TxConfig(0,l_u8RealPort-1,pstBasicVar->u8TxFormatSta[l_s8Output-1]);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output,l_s8FreeSta;
    u8 l_u8RealPort;

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
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                pstBasicVar->u8FreezeSta[i] = l_s8FreeSta;
                pCommVar->SendPrintf("output %d freeze %s\r\n",i,l_s8FreeSta?"on":"off");
                app_MS1826FreezeChange(0,l_u8RealPort-1,l_s8FreeSta);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;
            pstBasicVar->u8FreezeSta[l_s8Output-1] = l_s8FreeSta;
            pCommVar->SendPrintf("output %d freeze %s\r\n",l_s8Output,l_s8FreeSta?"on":"off");
            app_MS1826FreezeChange(0,l_u8RealPort-1,l_s8FreeSta);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("output port format is %s\r\n",pstBasicVar->u8TxFormatSta[0]?"dvi":"hdmi");

    return l_state;
}

//eFunc_SwitchFreeze,
static emCmdFuncStaType mapp_CmdfuncSwitchFreeze(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8FreeSta = 0;
    l_u8FreeSta = *l_pCmd;

    if(*(l_pCmd+1)=='.')
    {
        if(l_u8FreeSta>0)
        {
            if(l_u8FreeSta&0x01)
                app_MS1826SwitchFreeze(0,cg_u8ChannelTable[0][0],200);
            if(l_u8FreeSta&0x02)
                app_MS1826SwitchFreeze(0,cg_u8ChannelTable[1][0],200);
            if(l_u8FreeSta&0x04)
                app_MS1826SwitchFreeze(0,cg_u8ChannelTable[2][0],200);
            if(l_u8FreeSta&0x08)
                app_MS1826SwitchFreeze(0,cg_u8ChannelTable[3][0],200);

        #if (_SYSTEM_SUPPORT_RTOS==0)
            _video_freeze_out(0,l_u8FreeSta);
        #endif
            dbg_printf("output 0x%02x freeze\r\n",l_u8FreeSta);
            l_state = emCmdNone;
        }
    }
    else
        l_state = emCmdError;

    return l_state;
}

//eFunc_SetBrightness,
static emCmdFuncStaType mapp_CmdfuncSetBrightness(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Brightness;
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
                app_LMX_4OUHS_CPictureAdjust(i,(u8)l_s16Brightness,MS1826_ADJUST_BRIGHTNESS);
            }
        }
        else
        {
            pstBasicVar->u8Brightness[l_s8Output-1] = (u8)l_s16Brightness;
            pCommVar->SendPrintf("output %d brightness %d\r\n",l_s8Output,l_s16Brightness);
            app_LMX_4OUHS_CPictureAdjust(l_s8Output-1,(u8)l_s16Brightness,MS1826_ADJUST_BRIGHTNESS);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Value;
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
                app_LMX_4OUHS_CPictureAdjust(i,(u8)l_s16Value,MS1826_ADJUST_CONTRAST);
            }
        }
        else
        {
            pstBasicVar->u8Contrast[l_s8Output-1] = (u8)l_s16Value;
            pCommVar->SendPrintf("output %d contrast %d\r\n",l_s8Output,l_s16Value);
            app_LMX_4OUHS_CPictureAdjust(l_s8Output-1,(u8)l_s16Value,MS1826_ADJUST_CONTRAST);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Value;
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
                app_LMX_4OUHS_CPictureAdjust(i,(u8)l_s16Value,MS1826_ADJUST_SATURATION);
            }
        }
        else
        {
            pstBasicVar->u8Saturation[l_s8Output-1] = (u8)l_s16Value;
            pCommVar->SendPrintf("output %d saturation %d\r\n",l_s8Output,l_s16Value);
            app_LMX_4OUHS_CPictureAdjust(l_s8Output-1,(u8)l_s16Value,MS1826_ADJUST_SATURATION);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u8 l_u8NumLen=0;
    signed char l_s8Output;
    signed int  l_s16Value;

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
                app_LMX_4OUHS_CPictureAdjust(i,(u8)l_s16Value,MS1826_ADJUST_HUE);
            }
        }
        else
        {
            pstBasicVar->u8Hue[l_s8Output-1] = (u8)l_s16Value;
            pCommVar->SendPrintf("output %d hue %d\r\n",l_s8Output,l_s16Value);
            app_LMX_4OUHS_CPictureAdjust(l_s8Output-1,(u8)l_s16Value,MS1826_ADJUST_HUE);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Input,l_s8EdidID;
    u8 l_u8EdidBuf[256];
    u8 l_u8RealPort;

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
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre3.pu8EdidContext,256);
                break;
            case EDID_MODE_USER4:
                memcpy(l_u8EdidBuf,pstSpecVar->stCustomEdidPre4.pu8EdidContext,256);
                break;
            case EDID_MODE_COPYTX1:
            case EDID_MODE_COPYTX2:
            case EDID_MODE_COPYTX3:
            case EDID_MODE_COPYTX4:
//                memcpy(l_u8EdidBuf,g_stTxEdidVar[l_s8EdidID-EDID_MODE_COPYTX1].u8EdidRecv,256);
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
                app_MS1826RxEdidUpdate(0,l_u8RealPort-1,l_u8EdidBuf,256);
            }
//            mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,4000);
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][1]+1;
            app_MS1826RxEdidUpdate(0,l_u8RealPort-1,l_u8EdidBuf,256);
            pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_s8Input-1] = l_s8EdidID;
            pCommVar->SendPrintf("in %d edid %d\r\n",l_s8Input,l_s8EdidID);
//            mapp_MHD0404S_PlatformTimerSet(emUserTimer_TimingChange,2000);
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

//eFunc_GetTxEdidData
static emCmdFuncStaType mapp_CmdfuncGetTxEdidData(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    char l_s8Output;
#if 0
    u16 j;
    u8 l_u8Edid[256];

    memset(l_u8Edid,0,256);
#endif

    l_s8Output = (char)GetNumofStr(l_pCmd,')',3);

    if(l_s8Output == 0)
    {
        u8 i;
        for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
        {
            app_MS1826GetTxEdid(0,i);
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
    else if(l_s8Output<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        app_MS1826GetTxEdid(0,l_s8Output-1);
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
            l_u8RealPort = cg_u8ChannelTable[i][0]+1;
            app_MS1826SetUserTiming(0,l_u8RealPort-1,&l_stTiming);
        }
    }
    else if(l_s8Output<=Def_outport_num)
    {
        l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;
        app_MS1826SetUserTiming(0,l_u8RealPort-1,&l_stTiming);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetTxPattern
static emCmdFuncStaType mapp_CmdfuncSetTxPattern(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8TxPatternID;
    u8 l_u8RealPort;

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
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                pstBasicVar->u8PatternSta[i] = l_s8TxPatternID;
                pCommVar->SendPrintf("output %d pattern is %d\r\n",i+1,l_s8TxPatternID);
                if(pstBasicVar->u8PatternSta[i])
                    app_MS1826TxPatternConfig(0,l_u8RealPort-1,pstBasicVar->u8PatternSta[i]-1,1);
                else
                    app_MS1826TxPatternConfig(0,l_u8RealPort-1,0,0);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;
            pstBasicVar->u8PatternSta[l_s8Output-1] = l_s8TxPatternID;
            pCommVar->SendPrintf("output %d pattern is %d\r\n",l_s8Output,l_s8TxPatternID);
            if(pstBasicVar->u8PatternSta[l_s8Output-1])
                app_MS1826TxPatternConfig(0,l_u8RealPort-1,pstBasicVar->u8PatternSta[l_s8Output-1]-1,1);
            else
                app_MS1826TxPatternConfig(0,l_u8RealPort-1,0,0);
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_GetTxPattern,
static emCmdFuncStaType mapp_CmdfuncGetTxPattern(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output;
    u8 l_u8RVal,l_u8GVal,l_u8BVal;
    u8 l_u8RealPort;

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

                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                if((l_u8RVal==0)&&(l_u8GVal==0)&&(l_u8BVal==0))
                {
                    pCommVar->SendPrintf("output %d color bypass\r\n",i+1);
                    app_MS1826PictureColorConfig(0,l_u8RealPort-1,128,128,128,0);
                }
                else
                {
                    pCommVar->SendPrintf("output %d color R%d G%d B%d\r\n",i+1,l_u8RVal,l_u8GVal,l_u8BVal);
                    app_MS1826PictureColorConfig(0,l_u8RealPort-1,l_u8RVal,l_u8GVal,l_u8BVal,1);
                }
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Output-1][0]+1;
            pstBasicVar->u8RedValue[l_s8Output-1]      = l_u8RVal;
            pstBasicVar->u8GreenValue[l_s8Output-1]    = l_u8GVal;
            pstBasicVar->u8BlueValue[l_s8Output-1]     = l_u8BVal;
            if((l_u8RVal==0)&&(l_u8GVal==0)&&(l_u8BVal==0))
            {
                pCommVar->SendPrintf("output %d color bypass\r\n",l_s8Output);
                app_MS1826PictureColorConfig(0,l_u8RealPort-1,128,128,128,0);
            }
            else
            {
                pCommVar->SendPrintf("output %d color R%d G%d B%d\r\n",l_s8Output,l_u8RVal,l_u8GVal,l_u8BVal);
                app_MS1826PictureColorConfig(0,l_u8RealPort-1,l_u8RVal,l_u8GVal,l_u8BVal,1);
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
        for(i=0;i<Def_outport_num;i++)
        {
            l_u8RealPort = cg_u8ChannelTable[i][0]+1;
            app_MS1826SplitConfig(0,l_u8RealPort-1,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
            pCommVar->SendPrintf("split config: port[%d] en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",i+1,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
        }
    }
    else if(l_s8Port<=Def_outport_num)
    {
        l_u8RealPort = cg_u8ChannelTable[l_s8Port-1][0]+1;
        app_MS1826SplitConfig(0,l_u8RealPort-1,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
        pCommVar->SendPrintf("split config: port[%d] en[%d] x[%d] y[%d] w[%d] h[%d]\r\n",l_s8Port,u8Enalbe,l_u16X,l_u16Y,l_u16W,l_u16H);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_SetSplicerConfig,
static emCmdFuncStaType mapp_CmdfuncSetSplicerConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
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
        for(i=0;i<Def_outport_num;i++)
        {
            l_u8RealPort = cg_u8ChannelTable[i][0]+1;
            app_MS1826SplicerEnable(0,l_u8RealPort-1,(u8Enalbe==0)?FALSE:TRUE);
            app_MS1826SplicerConfig(0,l_u8RealPort-1,l_u16X,l_u16Y,l_u16W,l_u16H);
            pCommVar->SendPrintf("splicer config: port[%d] x[%d] y[%d] w[%d] h[%d]\r\n",i+1,l_u16X,l_u16Y,l_u16W,l_u16H);
            pstBasicVar->u16SplicerX[i] = l_u16X;
            pstBasicVar->u16SplicerY[i] = l_u16Y;
            pstBasicVar->u16SplicerWidth[i] = l_u16W;
            pstBasicVar->u16SplicerHeight[i] = l_u16H;
            pstBasicVar->bSplicerEnable[i] = (u8Enalbe==0)?FALSE:TRUE;
        }
    }
    else if(l_s8Port<=Def_outport_num)
    {
        l_u8RealPort = cg_u8ChannelTable[l_s8Port-1][0]+1;
        app_MS1826SplicerEnable(0,l_u8RealPort-1,(u8Enalbe==0)?FALSE:TRUE);
        app_MS1826SplicerConfig(0,l_u8RealPort-1,l_u16X,l_u16Y,l_u16W,l_u16H);
        pCommVar->SendPrintf("splicer config: port[%d] x[%d] y[%d] w[%d] h[%d]\r\n",l_s8Port,l_u16X,l_u16Y,l_u16W,l_u16H);
        pstBasicVar->u16SplicerX[l_s8Port-1] = l_u16X;
        pstBasicVar->u16SplicerY[l_s8Port-1] = l_u16Y;
        pstBasicVar->u16SplicerWidth[l_s8Port-1] = l_u16W;
        pstBasicVar->u16SplicerHeight[l_s8Port-1] = l_u16H;
        pstBasicVar->bSplicerEnable[l_s8Port-1] = (u8Enalbe==0)?FALSE:TRUE;
    }
    else
        l_state = emCmdOutOfRange;

    SetUserData_SpecBasicVar();
    return l_state;
}


//eFunc_GetSplicerConfig,
static emCmdFuncStaType mapp_CmdfuncGetSplicerConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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

//eFunc_SetFrameLock
static emCmdFuncStaType mapp_CmdfuncSetFrameLock(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
//    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
//    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8NumLen=0;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    signed char l_s8Output,l_s8Input;
    u8 l_u8RealPort;

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
                l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][1];
                pCommVar->SendPrintf("output %d frame lock %d\r\n",i+1,l_s8Output);
                #if ENABLE_MS1826_FRAME_LOCK
                app_MS1826FrameLockSelect(0,l_u8RealPort,i);
                #endif
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][1];
            pCommVar->SendPrintf("output %d frame lock %d\r\n",l_s8Output,l_s8Input);
            #if ENABLE_MS1826_FRAME_LOCK
            app_MS1826FrameLockSelect(0,l_u8RealPort,l_s8Output);
            #endif
        }
        SetUserData_SpecBasicVar();
    }
    else
        l_state = emCmdOutOfRange;

    UNUSED(l_u8RealPort);
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

        app_MS1826SetHoldFlag(l_s8TempVal?TRUE:FALSE);
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

    l_s8Input = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);
    l_pCmd = l_pCmd+l_u8NumLen+1;   // skip ','

    l_s8MuteSta = (char)GetNumofStr(l_pCmd,')',3);
    if((l_s8Input<=_HDMI_INPUT_PORT_USED_NUMBER)&&(l_s8MuteSta>=0&&l_s8MuteSta<=1))
    {
        if(l_s8Input == 0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                l_u8RealPort = cg_u8ChannelTable[i][0]+1;
                pCommVar->SendPrintf("hdmi %d video is %s\r\n",i+1,l_s8MuteSta?"mute":"unmute");
                app_MS1826SetVideoMute(0,l_u8RealPort-1,l_s8MuteSta?TRUE:FALSE);
            }
        }
        else
        {
            l_u8RealPort = cg_u8ChannelTable[l_s8Input-1][0]+1;
            pCommVar->SendPrintf("hdmi %d video is %s\r\n",l_s8Input,l_s8MuteSta?"mute":"unmute");
            app_MS1826SetVideoMute(0,l_u8RealPort-1,l_s8MuteSta?TRUE:FALSE);
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
    u8 l_u8NumLen=0;

    l_s8WorkMode = (char)GetNumofStr(l_pCmd,',',3);
    l_u8NumLen = GetLenOfStr(l_pCmd,',',3);

    l_pCmd = l_pCmd+l_u8NumLen+1;
    l_s8SubMode = (char)GetNumofStr(l_pCmd,')',3);
    if(l_s8WorkMode<=4)
    {
        pCommVar->SendPrintf("run mode: %d\r\n",l_s8WorkMode);
        app_MS1826WorkModeSwitch(0,l_s8WorkMode,l_s8SubMode);
    }
    else
        l_state = emCmdOutOfRange;

    return l_state;
}

//eFunc_FactoryReset2
static emCmdFuncStaType mapp_CmdfuncFactoryReset2(pstUartCom_t pCommVar,u8 cmdnum)
{
    pCommVar->SendPrintf("OKEND");
    mapp_UserData_HdmiSelInit();
    SetUserData_HdmiSel();
    delay_ms(100);
    LMX_4OUHS_C_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();
    delay_ms(100);

    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();
    delay_ms(500);
    #if _ENABLE_SYSTEM_RESET_DELAY_TIME
    mapp_SetSysResetModeDelayTime(1500);
    #endif

    return emCmdSucess;
}

//eFunc_CheckOutputCardType,
static emCmdFuncStaType mapp_CmdfuncCheckOutputCardType(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    s8 l_s8Num = 0;

    l_s8Num = (char)GetNumofStr(l_pCmd,'.',3);

    if(l_s8Num>0)
    {
        pCommVar->SendPrintf("MO%02d%s",l_s8Num,_STRING_CARD_TYPE);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdError;

    return l_state;
}

//eFunc_CheckCardFWVersion
static emCmdFuncStaType mapp_CmdfuncCheckCardFWVersion(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    s8 l_s8Num = 0;

    l_s8Num = (char)GetNumofStr(l_pCmd,'.',3);

    if(l_s8Num>0)
    {
        pCommVar->SendPrintf("%sEND",_STRING_FW_VER_NUM);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdError;

    return l_state;
}

#if ENABLE_CARD_YBJ_RECORD
//eFunc_CheckYBJCardType
static emCmdFuncStaType mapp_CmdfuncCheckYBJCardType(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    s8 l_s8Num = 0;

    l_s8Num = (char)GetNumofStr(l_pCmd,'.',3);

    if(l_s8Num>0)
    {
        pCommVar->SendPrintf("ptn17");
        l_state = emCmdNone;
    }
    else
        l_state = emCmdError;

    return l_state;
}

//eFunc_CheckYBJ2CardType
static emCmdFuncStaType mapp_CmdfuncCheckYBJ2CardType(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    s8 l_s8Num = 0;

    l_s8Num = (char)GetNumofStr(l_pCmd,'.',3);

    if(l_s8Num>0)
    {
        pCommVar->SendPrintf("YBOKEND");
        l_state = emCmdNone;
    }
    else
        l_state = emCmdError;

    return l_state;
}
#endif

//eFunc_CheckCardMode
static emCmdFuncStaType mapp_CmdfuncCheckCardMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    s8 l_s8Num = 0;

    l_s8Num = (char)GetNumofStr(l_pCmd,'.',3);

    if(l_s8Num>0)
    {
        pCommVar->SendPrintf("TO%02d%s",l_s8Num,_STRING_CARD_TYPE_NUM);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdError;

    return l_state;
}

//eFunc_CheckGetTxEdid
static emCmdFuncStaType mapp_CmdfuncCheckGetTxEdid(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    s8 l_s8Num = 0;
    u8 l_u8Edid[256];
    u8 l_u8RealPort;

    l_s8Num = (char)GetNumofStr(l_pCmd,'.',3);

    if(l_s8Num>0)
    {
        l_s8Num = (l_s8Num-1)%4+1;
        l_u8RealPort = cg_u8ChannelTable[l_s8Num-1][0]+1;
        app_ms1826_get_tx_edid_data(0,(l_u8RealPort-1)%4,l_u8Edid);
        pCommVar->SendPrintf("OEDID%02d",l_s8Num);
        pCommVar->SendBytes(l_u8Edid,256,FALSE);
        l_state = emCmdNone;
    }
    else
        l_state = emCmdError;

    return l_state;
}

bool app_SwitchResolution(u8 u8Tx,u8 u8ResID)
{
    bool l_bRetVal = FALSE;
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8RealPort = cg_u8ChannelTable[u8Tx-1][0]+1;

    switch(u8ResID)
    {
        case 0:     //720x480i60
            {
                l_bRetVal = TRUE;
            }
            break;
        case 1:     //720x576i50
            {
                l_bRetVal = TRUE;
            }
            break;
        case 2:     //720x480p60
            {
                l_bRetVal = TRUE;
            }
            break;
        case 3:     //720x576p50
            {
                l_bRetVal = TRUE;
            }
            break;
        case 4:     //1280x720p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1280x720_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1280x720_60);
            }
            break;
        case 5:     // 1280x720p59
            {
                l_bRetVal = TRUE;
            }
            break;
        case 6:     //1280x720p50
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1280x720_50;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1280x720_50);
            }
            break;
        case 7:     //1280x720p30
            {
                l_bRetVal = TRUE;
            }
            break;
        case 8:     //1280x720p25
            {
                l_bRetVal = TRUE;
            }
            break;
        case 9:     //1280x720p24
            {
                l_bRetVal = TRUE;
            }
            break;
        case 10:     //1920x1080i60
            {
                l_bRetVal = TRUE;
            }
            break;
        case 11:    //1920x1080i59
            {
                l_bRetVal = TRUE;
            }
            break;
        case 12:    //1920x1080i50
            {
                l_bRetVal = TRUE;
            }
            break;
        case 13:    //1920x1080p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1920x1080_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1920x1080_60);
            }
            break;
        case 14:    //1920x1080p59
            {
                l_bRetVal = TRUE;
            }
            break;
        case 15:    //1920x1080p50
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1920x1080_50;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1920x1080_50);
            }
            break;
        case 16:    //1920x1080p30
            {
                l_bRetVal = TRUE;
            }
            break;
        case 17:    //1920x1080p29
            {
                l_bRetVal = TRUE;
            }
            break;
        case 18:    //1920x1080p25
            {
                l_bRetVal = TRUE;
            }
            break;
        case 19:    //1920x1080p24
            {
                l_bRetVal = TRUE;
            }
            break;
        case 20:    //640x480p60
            {
                l_bRetVal = TRUE;
            }
            break;
        case 21:    //640x480p75
            {
                l_bRetVal = TRUE;
            }
            break;
        case 22:    //800x600p60
            {
                l_bRetVal = TRUE;
            }
            break;
        case 23:    //800x600p75
            {
                l_bRetVal = TRUE;
            }
            break;
        case 24:    //1024x768p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1024x768_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1024x768_60);
            }
            break;
        case 25:    //1024x768p75
            {
                l_bRetVal = TRUE;
            }
            break;
        case 26:    //1280x1024p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1280x1024_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1280x1024_60);
            }
            break;
        case 27:    //1280x1024p75
            {
                l_bRetVal = TRUE;
            }
            break;
        case 28:    //1360x768p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1360x768_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1360x768_60);
            }
            break;
        case 29:    //1366x768p60
            {
                l_bRetVal = TRUE;
            }
            break;
        case 30:    //1400x1050p60
            {
                l_bRetVal = TRUE;
                //app_MS1826TxResolutionChange(0,l_u8RealPort%4-1,emRes_1400x900_60);
            }
            break;
        case 31:    //1600x1200p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1600x1200_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1600x1200_60);
            }
            break;
        case 32:    //1440x900p60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1440x900_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1440x900_60);
            }
            break;
        case 33:    //1440x900p75
            {
                l_bRetVal = TRUE;
            }
            break;
        case 34:
            {
                l_bRetVal = TRUE;
            }
            break;
        case 35:
            {
                l_bRetVal = TRUE;
            }
            break;
        case 36:
            {
                l_bRetVal = TRUE;
            }
            break;
        case 37:    //1920x1200P60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1920x1200_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1920x1200_60);
            }
            break;
        case 38:
            {
                l_bRetVal = TRUE;
            }
            break;
        case 39:    //1600x900P60
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_1600x900_60;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_1600x900_60);
            }
            break;
        case 40:    //3840x2160 30Hz
            {
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_3840x2160_30;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_3840x2160_30);
            }
            break;
        case 44:    //4096x2160 30Hz
            {
                #if _ENABLE_RES_4096x2160_30
                l_bRetVal = TRUE;
                pstBasicVar->u8TxResolutionID[u8Tx-1] = emRes_4096x2160_30;
                app_MS1826TxResolutionChange(0,(l_u8RealPort-1)%4,emRes_4096x2160_30);
                #endif
            }
            break;
        default:
            break;
    }

    SetUserData_SpecBasicVar();
    return l_bRetVal;
}

u8 app_GetCmdResolutionID(u8 u8ResID)
{
    switch(u8ResID)
    {
        case emRes_1920x1200_60:
            return 37;
        case emRes_1920x1080_60:
            return 13;
        case emRes_1280x720_60:
            return 4;
        case emRes_1360x768_60:
            return 28;
        case emRes_1280x1024_60:
            return 26;
        case emRes_1024x768_60:
            return 24;
        case emRes_1600x1200_60:
            return 31;
        case emRes_1440x900_60:
            return 32;
        case emRes_1600x900_60:
            return 39;
        case emRes_1280x720_50:
            return 6;
        case emRes_1920x1080_50:
            return 15;
        case emRes_3840x2160_30:
            return 40;
        #if _ENABLE_RES_4096x2160_30
        case emRes_4096x2160_30:
            return 44;
        #endif
        default:
            return 0xFF;
    }
}
void app_Type1CmdFunction(u8 u8Port,u8 u8Cmd)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8RealPort = cg_u8ChannelTable[u8Port-1][0]+1;

    switch(u8Cmd)
    {
        case 3:
            pstBasicVar->u8TxFormatSta[u8Port-1] = 1;
            dbg_printf("output hdmi %d format is %s\r\n",u8Port,"hdmi");
            app_MS1826TxConfig(0,l_u8RealPort-1,pstBasicVar->u8TxFormatSta[u8Port-1]);
            SetUserData_SpecBasicVar();
            break;
        case 4:
            pstBasicVar->u8TxFormatSta[u8Port-1] = 0;
            dbg_printf("output hdmi %d format is %s\r\n",u8Port,"dvi");
            app_MS1826TxConfig(0,l_u8RealPort-1,pstBasicVar->u8TxFormatSta[u8Port-1]);
            SetUserData_SpecBasicVar();
            break;
        case 6:
            pstBasicVar->u8TxHdcpMode[u8Port-1] = 1;
            dbg_printf("out %d hdcp %s\r\n",u8Port,"on");
            app_MS1826TxHdcpEnable(0,l_u8RealPort-1,pstBasicVar->u8TxHdcpMode[u8Port-1]);
            SetUserData_SpecBasicVar();
            break;
        case 7:
            pstBasicVar->u8TxHdcpMode[u8Port-1] = 0;
            dbg_printf("out %d hdcp %s\r\n",u8Port,"off");
            app_MS1826TxHdcpEnable(0,l_u8RealPort-1,pstBasicVar->u8TxHdcpMode[u8Port-1]);
            SetUserData_SpecBasicVar();
            break;
        case 10:
            pstBasicVar->u8AudioMute[u8Port-1] = 0;
            dbg_printf("out %d digital audio %s\r\n",u8Port,"unmute");
            app_MS1826TxDigitalAudioMute(0,l_u8RealPort-1,pstBasicVar->u8AudioMute[u8Port-1]);
            SetUserData_SpecBasicVar();
            break;
        case 11:
            pstBasicVar->u8AudioMute[u8Port-1] = 1;
            dbg_printf("out %d digital audio %s\r\n",u8Port,"mute");
            app_MS1826TxDigitalAudioMute(0,l_u8RealPort-1,pstBasicVar->u8AudioMute[u8Port-1]);
            SetUserData_SpecBasicVar();
            break;
        case 12:
            //pstBasicVar->u8AudioMute[u8Port-1] = 0;
            dbg_printf("out %d digital audio %s\r\n",u8Port,"unmute");
            mapp_SetAudioMute(l_u8RealPort-1,FALSE);
            //SetUserData_SpecBasicVar();
            break;
        case 13:
            //pstBasicVar->u8AudioMute[u8Port-1] = 1;
            dbg_printf("out %d digital audio %s\r\n",u8Port,"mute");
            mapp_SetAudioMute(l_u8RealPort-1,TRUE);
            //SetUserData_SpecBasicVar();
            break;
        case 16:
            pstBasicVar->u8BlackScreenEnable[u8Port-1] = 1;
            dbg_printf("out %d black screen %s\r\n",u8Port,"disable");
            app_MS1826TxPatternConfig(0,l_u8RealPort-1,0,0);
            mapp_SetAudioMute(u8Port-1,FALSE);
            app_MS1826TxHdmiAudioMute(0,l_u8RealPort-1,0);
            //SetUserData_SpecBasicVar();
            break;
        case 17:
            pstBasicVar->u8BlackScreenEnable[u8Port-1] = 0;
            dbg_printf("out %d black screen %s\r\n",u8Port,"enable");
            app_MS1826TxPatternConfig(0,l_u8RealPort-1,0,1);
            mapp_SetAudioMute(u8Port-1,TRUE);
            app_MS1826TxHdmiAudioMute(0,l_u8RealPort-1,1);
            //SetUserData_SpecBasicVar();
            break;
        case 18:
            dbg_printf("output %d freeze %s\r\n",u8Port,"off");
            pstBasicVar->u8FreezeSta[u8Port-1] = 0;
            app_MS1826FreezeChange(0,l_u8RealPort-1,0);
            break;
        case 19:
            dbg_printf("output %d freeze %s\r\n",u8Port,"on");
            pstBasicVar->u8FreezeSta[u8Port-1] = 1;
            app_MS1826FreezeChange(0,l_u8RealPort-1,1);
            break;

        default:
            break;
    }
}

void app_Type4CmdFunction(u8 u8Port,u8 u8Cmd)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8RealPort = cg_u8ChannelTable[u8Port-1][0]+1;

    switch(u8Cmd)
    {
        case 0:
            if(pstBasicVar->u8Brightness[u8Port-1]>0)
                pstBasicVar->u8Brightness[u8Port-1]--;
            dbg_printf("output %d brightness %d\r\n",u8Port,pstBasicVar->u8Brightness[u8Port-1]);
            app_LMX_4OUHS_CPictureAdjust(l_u8RealPort-1,pstBasicVar->u8Brightness[u8Port-1],MS1826_ADJUST_BRIGHTNESS);
            SetUserData_SpecBasicVar();
            break;
        case 1:
            if(pstBasicVar->u8Brightness[u8Port-1]<100)
                pstBasicVar->u8Brightness[u8Port-1]++;
            dbg_printf("output %d brightness %d\r\n",u8Port,pstBasicVar->u8Brightness[u8Port-1]);
            app_LMX_4OUHS_CPictureAdjust(l_u8RealPort-1,pstBasicVar->u8Brightness[u8Port-1],MS1826_ADJUST_BRIGHTNESS);
            SetUserData_SpecBasicVar();
            break;
        case 10:
            if(pstBasicVar->u8Contrast[u8Port-1]>0)
                pstBasicVar->u8Contrast[u8Port-1]--;
            dbg_printf("output %d contrast %d\r\n",u8Port,pstBasicVar->u8Contrast[u8Port-1]);
            app_LMX_4OUHS_CPictureAdjust(l_u8RealPort-1,pstBasicVar->u8Contrast[u8Port-1],MS1826_ADJUST_CONTRAST);
            SetUserData_SpecBasicVar();
            break;
        case 11:
            if(pstBasicVar->u8Contrast[u8Port-1]<100)
                pstBasicVar->u8Contrast[u8Port-1]++;
            dbg_printf("output %d contrast %d\r\n",u8Port,pstBasicVar->u8Contrast[u8Port-1]);
            app_LMX_4OUHS_CPictureAdjust(l_u8RealPort-1,pstBasicVar->u8Contrast[u8Port-1],MS1826_ADJUST_CONTRAST);
            SetUserData_SpecBasicVar();
            break;
        case 20:
            if(pstBasicVar->u8Saturation[u8Port-1]>0)
                pstBasicVar->u8Saturation[u8Port-1]--;
            dbg_printf("output %d saturation %d\r\n",u8Port,pstBasicVar->u8Saturation[u8Port-1]);
            app_LMX_4OUHS_CPictureAdjust(l_u8RealPort-1,pstBasicVar->u8Saturation[u8Port-1],MS1826_ADJUST_SATURATION);
            SetUserData_SpecBasicVar();
            break;
        case 21:
            if(pstBasicVar->u8Saturation[u8Port-1]<100)
                pstBasicVar->u8Saturation[u8Port-1]++;
            dbg_printf("output %d saturation %d\r\n",u8Port,pstBasicVar->u8Saturation[u8Port-1]);
            app_LMX_4OUHS_CPictureAdjust(l_u8RealPort-1,pstBasicVar->u8Saturation[u8Port-1],MS1826_ADJUST_SATURATION);
            SetUserData_SpecBasicVar();
            break;

        default:
            break;
    }
}

void app_Type7CmdFunction(pstUartCom_t pCommVar,u8 u8Port,u8 u8Cmd)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8Value = 0;
    u8 l_u8RealPort;

    switch(u8Cmd)
    {
        case 0:
            l_u8Value = app_GetCmdResolutionID(pstBasicVar->u8TxResolutionID[u8Port-1]);
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 1:
            l_u8Value = pstBasicVar->u8Brightness[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 2:
            l_u8Value = pstBasicVar->u8Contrast[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 3:
            l_u8Value = pstBasicVar->u8Saturation[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 4:
            l_u8Value = pstBasicVar->u8Hue[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 6:
            l_u8Value = pstBasicVar->u8TxHdcpMode[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 7:
            l_u8Value = pstBasicVar->u8TxFormatSta[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 8:
            break;
        case 9:
            l_u8Value = pstBasicVar->u8AudioMute[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 11:
            l_u8Value = pstBasicVar->u8BlackScreenEnable[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 12:
            l_u8Value = pstBasicVar->u8FreezeSta[u8Port-1];
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 13:
            l_u8RealPort = cg_u8ChannelTable[u8Port-1][0]+1;
            l_u8Value = app_ms1826_get_tx_hpd_sta(0,l_u8RealPort-1);
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 14:
            l_u8RealPort = cg_u8ChannelTable[u8Port-1][1]+1;
            l_u8Value = app_ms1826_get_rx_signal_sta(0,l_u8RealPort-1);
            pCommVar->SendPrintf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            dbg_printf("P%02d%02d%03dEND",u8Port,u8Cmd,l_u8Value);
            break;
        case 28:
            pCommVar->SendPrintf("P%02d%02d%sEND",u8Port,u8Cmd,"B2301OAA0");
            dbg_printf("P%02d%02d%sEND",u8Port,u8Cmd,"B2301OAA0");
            break;
        default:
            break;
    }
}

void app_SplicerCmdFunction(u8 u8Port,char* pu8Cmd)
{
    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8X=0;
    u8 l_u8Y=0;
    u8 l_u8Index=0;

    if((*(pu8Cmd)=='A')&&((*(pu8Cmd+1)=='L')||(*(pu8Cmd+1)=='L')))
    {
        ;
    }
    else if((*(pu8Cmd+1)<'1')||(*(pu8Cmd+1)>'9')||(*(pu8Cmd+2)<'1')||(*(pu8Cmd+2)>'9'))
        return;

    switch(*(pu8Cmd))
    {
        case 'A':    // full screen
            {
                if(((*(pu8Cmd+1)=='L')&&(*(pu8Cmd+2)=='L'))||((*(pu8Cmd+1)=='1')&&(*(pu8Cmd+2)=='1')))
                {
                    //close splicer mode
//                    app_LMX_4OUHS_CSetSplicerDisplay(u8Port,1,1,0);
                    pstBasicVar->bSplicerEnable[u8Port-1]   = FALSE;
                    app_MS1826SplicerEnable(0,cg_u8ChannelTable[u8Port-1][0],FALSE);
                    dbg_printf("output %d close splicer mode\r\n",u8Port);
                    SetUserData_SpecBasicVar();
                }
            }
            break;
        case 'B':    //2*2
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>2||l_u8X>2))
                    break;
                l_u8Index = (l_u8Y-1)*2+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,2,2,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,2,2,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,2,2,l_u8Index);
            }
            break;
        case 'C':    //3*3
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>3||l_u8X>3))
                    break;
                l_u8Index = (l_u8Y-1)*3+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,3,3,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,3,3,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,3,3,l_u8Index);
            }
            break;
        case 'D':    //4*4
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>4||l_u8X>4))
                    break;
                l_u8Index = (l_u8Y-1)*4+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,4,4,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,4,4,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,4,4,l_u8Index);
            }
            break;
        case 'E':    //5*5
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>5||l_u8X>5))
                    break;
                l_u8Index = (l_u8Y-1)*5+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,5,5,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,5,5,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,5,5,l_u8Index);
            }
            break;
        case 'F':    //6*6
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>6||l_u8X>6))
                    break;
                l_u8Index = (l_u8Y-1)*6+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,6,6,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,6,6,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,6,6,l_u8Index);
            }
            break;
        case 'G':    //7*7
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>7||l_u8X>7))
                    break;
                l_u8Index = (l_u8Y-1)*7+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,7,7,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,7,7,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,7,7,l_u8Index);
            }
            break;
        case 'H':    //8*8
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>8||l_u8X>8))
                    break;
                l_u8Index = (l_u8Y-1)*8+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,8,8,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,8,8,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,8,8,l_u8Index);
            }
            break;
        case 'J':    //1*2
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>1||l_u8X>2))
                    break;
                l_u8Index = (l_u8Y-1)*1+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,1,2,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,1,2,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,1,2,l_u8Index);
            }
            break;
        case 'K':    //1*3
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>1||l_u8X>3))
                    break;
                l_u8Index = (l_u8Y-1)*1+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,1,3,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,1,3,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,1,3,l_u8Index);
            }
            break;
        case 'L':    //1*4
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>1||l_u8X>4))
                    break;
                l_u8Index = (l_u8Y-1)*1+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,1,4,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,1,4,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,1,4,l_u8Index);
            }
            break;
        case 'M':    //2*3
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>2||l_u8X>3))
                    break;
                l_u8Index = (l_u8Y-1)*2+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,2,3,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,2,3,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,2,3,l_u8Index);
            }
            break;
        case 'N':    //2*4
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>2||l_u8X>4))
                    break;
                l_u8Index = (l_u8Y-1)*2+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,2,4,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,2,4,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,2,4,l_u8Index);
            }
            break;
        case 'O':    //3*4
            {
                l_u8Y = HexAndAsciiChange(*(pu8Cmd+1),0);
                l_u8X = HexAndAsciiChange(*(pu8Cmd+2),0);
                if((l_u8Y==0||l_u8X==0)||(l_u8Y>3||l_u8X>4))
                    break;
                l_u8Index = (l_u8Y-1)*3+(l_u8X-1);

                dbg_printf("output %d splicer mode %dx%d index %d\r\n",u8Port,3,4,l_u8Index);
                app_LMX_4OUHS_CSetSplicerBorder(u8Port-1,3,4,l_u8Index,pstBasicVar->u16BorderSizeW[u8Port-1],pstBasicVar->u16BorderSizeH[u8Port-1]);
                app_LMX_4OUHS_CSetSplicerDisplay(u8Port-1,3,4,l_u8Index);
            }
            break;
        default:
            break;
    }
}

//eFunc_SetCardFunction
static emCmdFuncStaType mapp_CmdfuncSetCardFunction(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdNone;
    char *l_pCmd= pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd);
    u16 l_u16Port = 0;
    u8 l_u8CmdType = 0;
    u8 l_u8Cmd = 0;
    l_u16Port = HexAndAsciiChange(*l_pCmd,0)*100+HexAndAsciiChange(*(l_pCmd+1),0)*10+HexAndAsciiChange(*(l_pCmd+2),0);
    l_u16Port = (l_u16Port-1)%4+1;

    if(*(l_pCmd+3) == 'P')
    {
        app_SplicerCmdFunction(l_u16Port,l_pCmd+4);
        pCommVar->SendPrintf("OKEND");
    }
    else
    {
        l_u8CmdType = HexAndAsciiChange(*(l_pCmd+3),0)*10+HexAndAsciiChange(*(l_pCmd+4),0);
        l_u8Cmd = HexAndAsciiChange(*(l_pCmd+5),0)*10+HexAndAsciiChange(*(l_pCmd+6),0);

        switch(l_u8CmdType)
        {
            case 1:
                pCommVar->SendPrintf("OKEND");
                app_Type1CmdFunction(l_u16Port,l_u8Cmd);
                break;
            case 4:
                pCommVar->SendPrintf("OKEND");
                app_Type4CmdFunction(l_u16Port,l_u8Cmd);
                break;
            case 6:
                {
                    switch(l_u8Cmd)
                    {
                        case 17:
//                            break;
                        case 18:
                            {
                                pCommVar->SendPrintf("OKEND");
                                mapp_UserData_HdmiSelInit();
                                SetUserData_HdmiSel();
                                delay_ms(100);
                                LMX_4OUHS_C_SpecBasicVarDefault();
                                SetUserData_SpecBasicVar();
                                delay_ms(100);

                                mapp_UserDataEdidIndexInit();
                                SetUserData_EdidIndexSave();
                                delay_ms(500);
                                #if _ENABLE_SYSTEM_RESET_DELAY_TIME
                                mapp_SetSysResetModeDelayTime(1500);
                                #endif
                            }
                            break;
                    }
                }
                break;
            case 7:
                //pCommVar->SendPrintf("OKEND");
                app_Type7CmdFunction(pCommVar,l_u16Port,l_u8Cmd);
                break;
            case 8:
                if(app_SwitchResolution((u8)l_u16Port,l_u8Cmd))
                {
                    pCommVar->SendPrintf("OKEND");
                }
                break;
            case 10:
                {
                    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
                    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

                    pstBasicVar->u16BorderSizeW[l_u16Port-1] = l_u8Cmd;
                    SetUserData_SpecBasicVar();
                    pCommVar->SendPrintf("OKEND");
                }
                break;
            case 11:
                {
                    pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
                    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

                    pstBasicVar->u16BorderSizeH[l_u16Port-1] = l_u8Cmd;
                    SetUserData_SpecBasicVar();
                    pCommVar->SendPrintf("OKEND");
                }
                break;
            default:
                break;
        }
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
        case eFunc_SwitchFreeze:
            {
                l_bStatus = mapp_CmdfuncSwitchFreeze(pCommVar,cmdnum);
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
        case eFunc_FactoryReset2:
            {
                l_bStatus = mapp_CmdfuncFactoryReset2(pCommVar,cmdnum);
            }
            break;
        case eFunc_CheckOutputCardType:
            {
                l_bStatus = mapp_CmdfuncCheckOutputCardType(pCommVar,cmdnum);
            }
            break;
        case eFunc_CheckCardFWVersion:
            {
                l_bStatus = mapp_CmdfuncCheckCardFWVersion(pCommVar,cmdnum);
            }
            break;
#if ENABLE_CARD_YBJ_RECORD
        case eFunc_CheckYBJCardType:
            {
                l_bStatus = mapp_CmdfuncCheckYBJCardType(pCommVar,cmdnum);
            }
            break;
        case eFunc_CheckYBJ2CardType:
            {
                l_bStatus = mapp_CmdfuncCheckYBJ2CardType(pCommVar,cmdnum);
            }
            break;
#endif
        case eFunc_SetCardFunction:
            {
                l_bStatus = mapp_CmdfuncSetCardFunction(pCommVar,cmdnum);
            }
            break;
        case eFunc_CheckCardMode:
            {
                l_bStatus = mapp_CmdfuncCheckCardMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_CheckGetTxEdid:
            {
                l_bStatus = mapp_CmdfuncCheckGetTxEdid(pCommVar,cmdnum);
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
	pstProjectSpecVar_t			pstSpecVar = &stLMX_4OUHS_CSpecVar;
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
        pstProjectSpecVar_t         pstSpecVar = &stLMX_4OUHS_CSpecVar;
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

