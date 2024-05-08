#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/FIH2TX/mapp_UartCmdList_FIH2TX.h"

typedef void (*pFuncUartRes)(uint8_t *pu8Data, uint16_t u16Len);

typedef struct _stUartCmdRx_t_
{
	char 				*cmdStr;
	pFuncUartRes		funcResponse;
}stUartCmdRx_t, *pstUartCmdRx_t;

extern stUartCom_t				g_stUartCommVar, g_stUart1CommVar;
extern const stUartDeclare_t 	stProjectUartWorkTable[];
#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
extern SemaphoreHandle_t		xSemaphore_Rs232Snd;
extern SemaphoreHandle_t		xSemaphore_Uart1Snd;
#if 0
extern SemaphoreHandle_t		xSemaphore_MstSnd;
#endif
#endif
//extern QueueHandle_t 			xQueue_EdidUpdate;
extern const stUartCmdRx_t 		stCmdUartRxTable[];
extern const uint8_t 			g_u8SizeOfstCmdUartRxTable;
extern key_val_stored_t 		s_tValStd;
//extern volatile bool 			g_vbIsWaitingForEdidData;
extern bool 					g_bTvPowerOnOffByRemote ;

static eCmdFun_t cmdfunctionID = eFunc_NoUse;
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
bool StartEdidCostumFlag;
u8 g_u8UserEDIDIndex;
bool g_bInternalEdidFlag;
#if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
u8 g_u8InternalEDIDIndex;
#endif
#endif

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
bool g_bSendUartKeyVal[uart_study_key_type_max] = {FALSE};
#endif

void UartCommVarClear(pstUartCom_t pCommVar);
bool isFrontPanelLocked(pkey_val_stored_t ptKeyVal, uint8_t u8KeyIndex);

//uart response string of hdmi input switch
#define UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL_AIRPLAY    "@AV A\r\n"
#define UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL_HDMI1      "@AV PC1\r\n"
#define UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL_HDMI2      "@AV PC2\r\n"
#define UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL_TYPEC      "@AV PC3\r\n"
#define UR_STRING_DEVICE_AUTO_SWITCH_MODE               "@AUTO_SWITCH 1\r\n"
#define UR_STRING_DEVICE_MANUAL_SWITCH_MODE             "@AUTO_SWITCH 0\r\n"
#define UR_STRING_DEVICE_FACTORY_DEFAULT        		"@FACTORY_RESET\r\n"
#define UR_STRING_DEVICE_KEY_PAD_LOCKED        		    "@KEYPAD_LOCK 1\r\n"
#define UR_STRING_DEVICE_DISPLAY_ON        		        "@DISPLAY 1\r\n"
#define UR_STRING_DEVICE_DISPLAY_OFF                    "@DISPLAY 0\r\n"
#define UR_STRING_DEVICE_EDID_MODE                      "@EDID_MODE %d\r\n"
#if _ENABLE_RESET_WIRELESS_POWER
#define UR_STRING_DEVICE_RESET_WIRELESS_POWER           "@RESET WIRELESS DEVICE\r\n"
#endif

bool g_bEnableRs232FeedBackFlag = TRUE;

//eFunc_GetVersion,
static emCmdFuncStaType mapp_CmdfuncGetVersion(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    pCommVar->SendPrintf("@%s",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_FactoryReset,
static emCmdFuncStaType mapp_CmdfuncFactoryReset(pstUartCom_t pCommVar,u8 cmdnum)
{
    F1H2TX_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();
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

//eFunc_SwitchPowerState,
static emCmdFuncStaType mapp_CmdfuncSwitchPowerState(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8PowerState=0;
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    l_u8PowerState = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8PowerState)
    {
        case 1:
            mapp_SetPowerState(TRUE);
            break;
        case 0:
            mapp_SetPowerState(FALSE);
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }
    if(l_emRtState == emCmdSucess)
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8PowerState);
    return l_emRtState;
}

//eFunc_GetPowerState,
static emCmdFuncStaType mapp_CmdfuncGetPowerState(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t			pstSpecVar = &stF1H2TXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->bPowerOnSta);
    return l_emRtState;
}

//eFunc_SetRs232Baudrate,
static emCmdFuncStaType mapp_CmdfuncSetRs232Baudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
    u8 l_u8BaudrateID=0;
    pstUartCom_t pUartCommVar = &g_stUartCommVar;
	pstProjectSpecVar_t			pstSpecVar = &stF1H2TXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            l_u8BaudrateID = baud_rate_115200;
            break;
        case 1:
            l_u8BaudrateID = baud_rate_57600;
            break;
        case 2:
            l_u8BaudrateID = baud_rate_38400;
            break;
        case 3:
            l_u8BaudrateID = baud_rate_19200;
            break;
        case 4:
            l_u8BaudrateID = baud_rate_9600;
            break;
        case 5:
            l_u8BaudrateID = baud_rate_4800;
            break;
        case 6:
            l_u8BaudrateID = baud_rate_2400;
            break;
        default:
            return emCmdOutOfRange;
            //break;
    }

    if(l_emRtState == emCmdSucess)
    {
        pstBasicSpecVar->u8baudrateId = l_u8BaudrateID;
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, mapp_GetRealBaudrate(l_u8BaudrateID));
        SetUserData_SpecBasicVar();

        delay_ms(5);
        pUartCommVar->SetBaudRate(mapp_GetRealBaudrate(l_u8BaudrateID), DEF_UART_DATA_CONFIG);
    }
    return emCmdSucess;
}

//eFunc_GetRs232Baudrate,
static emCmdFuncStaType mapp_CmdfuncGetRs232Baudrate(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t			pstSpecVar = &stF1H2TXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8BaudrateID=0;

    switch(pstBasicSpecVar->u8baudrateId)
    {
        case baud_rate_115200:
            l_u8BaudrateID = 0;
            break;
        case baud_rate_57600:
            l_u8BaudrateID = 1;
            break;
        case baud_rate_38400:
            l_u8BaudrateID = 2;
            break;
        case baud_rate_19200:
            l_u8BaudrateID = 3;
            break;
        case baud_rate_9600:
            l_u8BaudrateID = 4;
            break;
        case baud_rate_4800:
            l_u8BaudrateID = 5;
            break;
        case baud_rate_2400:
            l_u8BaudrateID = 6;
            break;
        default:
            return emCmdOutOfRange;
            //break;
    }
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8BaudrateID);
    return l_emRtState;
}

//eFunc_HelpFunction,
static emCmdFuncStaType mapp_CmdfuncHelpFunction(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    if((strncmp(pCommVar->pRecBuffer,cmd_list[cmdnum].ucmd,strlen(cmd_list[cmdnum].ucmd))==0)
        &&(pCommVar->u16RecCnt == strlen(cmd_list[cmdnum].ucmd)+2))
    {
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
    }
    else
    {
    #if 1
        char *subcmd = pCommVar->pRecBuffer+strlen(cmd_list[cmdnum].ucmd)+1;
        u16 subcmdlen = pCommVar->u16RecCnt - strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset-1;
        u8 l_u8SubCmdIndex = 0;
        for(l_u8SubCmdIndex=0; l_u8SubCmdIndex<(sizeof(cmd_list)/sizeof(cmd_list[0]));l_u8SubCmdIndex++)
        {
            if(strncmp(subcmd,cmd_list[l_u8SubCmdIndex].ucmd+1,strlen(cmd_list[l_u8SubCmdIndex].ucmd+1))==0&&(subcmdlen == strlen(cmd_list[l_u8SubCmdIndex].ucmd+1)))
                break;
        }
        switch( cmd_list[l_u8SubCmdIndex].eCmdFun)
        {
            case eFunc_GetVersion:
                {
                    pCommVar->SendPrintf("@Get the firmware version\r\n");   // description
                    pCommVar->SendPrintf("#GET_FIRMWARE_VERSION\r\n");      // formate
                }
                break;
            case eFunc_FactoryReset:
                {
                    pCommVar->SendPrintf("@Factory Default\r\n");
                    pCommVar->SendPrintf("#FACTORY_RESET\r\n");
                }
                break;
            case eFunc_Reboot:
                {
                    pCommVar->SendPrintf("@System reboot\r\n");
                    pCommVar->SendPrintf("#REBOOT\r\n");
                }
                break;
            case eFunc_SwitchPowerState:
                {
                    pCommVar->SendPrintf("@Enter/exit standby mode\r\n");
                    pCommVar->SendPrintf("#SET_POWER PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - STANDBY MODE\r\n");
                    pCommVar->SendPrintf("1 - POWER ON MODE\r\n");
                }
                break;
            case eFunc_GetPowerState:
                {
                    pCommVar->SendPrintf("@Get the standby status\r\n");
                    pCommVar->SendPrintf("#GET_POWER\r\n");
                }
                break;
            case eFunc_SetRs232Baudrate:
                {
                    pCommVar->SendPrintf("@Set the RS232 baud rate\r\n");
                    pCommVar->SendPrintf("#SET_RS232_BAUD PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 6\r\n");
                    pCommVar->SendPrintf("0 - 115200\r\n");
                    pCommVar->SendPrintf("1 - 57600\r\n");
                    pCommVar->SendPrintf("2 - 38400\r\n");
                    pCommVar->SendPrintf("3 - 19200\r\n");
                    pCommVar->SendPrintf("4 - 9600\r\n");
                    pCommVar->SendPrintf("5 - 4800\r\n");
                    pCommVar->SendPrintf("6 - 2400\r\n");
                }
                break;
            case eFunc_GetRs232Baudrate:
                {
                    pCommVar->SendPrintf("@Get the RS232 baud rate\r\n");
                    pCommVar->SendPrintf("#GET_RS232_BAUD\r\n");
                }
                break;
            case eFunc_HelpFunction:
                {
                    pCommVar->SendPrintf("@Get the command details\r\n");
                    pCommVar->SendPrintf("#HELP PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = NO PARAMETER (Get command list)\r\n");
                    pCommVar->SendPrintf("PARAM =  ANY COMMAND (NO '#',Get command description)\r\n");
                }
                break;
            case eFunc_GetTestVersion:
                {
                    pCommVar->SendPrintf("@Get the test firmware version\r\n");
                    pCommVar->SendPrintf("#TESTVERSION\r\n");
                }
                break;

            case eFunc_NoUse:
            case eFunc_Max:
            default:
                l_emRtState = emCmdOutOfRange;
                break;
        }
    #endif
    }
    //pCommVar->SendString(UR_STRING_HDCP_MODE_HDCP_OFF, TRUE);
    return l_emRtState;
}

//eFunc_GetTestVersion,
static emCmdFuncStaType mapp_CmdfuncGetTestVersion(pstUartCom_t pCommVar,u8 cmdnum)
{
    UNUSED(cmdnum);
    UNUSED(pCommVar);
    pCommVar->SendPrintf(cmd_list[cmdnum].feedbackstring);
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
        case eFunc_SwitchPowerState:
            {
                l_bStatus = mapp_CmdfuncSwitchPowerState(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetPowerState:
            {
                l_bStatus = mapp_CmdfuncGetPowerState(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetRs232Baudrate:
            {
                l_bStatus = mapp_CmdfuncSetRs232Baudrate(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetRs232Baudrate:
            {
                l_bStatus = mapp_CmdfuncGetRs232Baudrate(pCommVar,cmdnum);
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
	pstProjectSpecVar_t			pstSpecVar = &stF1H2TXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    //RS232_printf("Cmd:%s",pCommVar->pRecBuffer);
    if((pCommVar->pRecBuffer[pCommVar->u16RecCnt-2]==0x0D)&&(pCommVar->pRecBuffer[pCommVar->u16RecCnt-1]==0x0A))
    {
        for(i=0;i<MaxCmdNum;i++)
        {
            if(cmd_list[i].eCmdFun==eFunc_Max || cmd_list[i].flag==Flag_End)
            {
                break;
            }
            if(cmd_list[i].flag==Flag_NoFunction)
                continue;
            //RS232_printf("cmdlen:%d\r\n",pCommVar->u16RecCnt);
            //RS232_printf("cmdstr:%s\n",cmd_list[i].ucmd);
            //RS232_printf("strlen:%d\n",strlen(cmd_list[i].ucmd));
            //RS232_printf("cmdlen[%d]:%d\n",i,Cmdlength(i));
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

        #if 1
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

        l_bStatus = mapp_CommandFunctionProcess(l_u8Cmdnumber,l_u8Flag, pCommVar);
    }

    if( l_bStatus == emCmdError )
    {
        pCommVar->SendString("COMMAND ERROR\r\n",TRUE);
        //RS232_printf("%s!\r\n",pCommVar->pRecBuffer);
    }
    else if( l_bStatus == emCmdOutOfRange )
    {
        pCommVar->SendString("OUT OF RANGE\r\n",TRUE);
        //RS232_printf("%s!\r\n",pCommVar->pRecBuffer);
    }
}

void mapp_SetPowerState(bool bPwrSta)
{
	pstProjectSpecVar_t			pstSpecVar = &stF1H2TXSpecVar;
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
        case baud_rate_4800:
            l_u32Baudrate = 4800;
            break;
        case baud_rate_2400:
            l_u32Baudrate = 2400;
            break;
        default:
            break;
    }

    return l_u32Baudrate;
}

