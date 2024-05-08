#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/SCU41-MV/mapp_UartCmdList_SCU41_MV.h"

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
extern SemaphoreHandle_t		xSemaphore_Uart3Snd;
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

u8 g_u8IpAddr[4] = {0,0,0,0};
u8 g_u8NetMask[4] = {0,0,0,0};
u8 g_u8NetGate[4] = {0,0,0,0};
u8 g_u8UserMvModeIndex = 0;
u8 g_u8QueryRxPort = 0;
u8 g_u8QueryTxPort = 0;

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
bool g_bSendUartKeyVal[uart_study_key_type_max] = {FALSE};
#endif

void UartCommVarClear(pstUartCom_t pCommVar);
bool isFrontPanelLocked(pkey_val_stored_t ptKeyVal, uint8_t u8KeyIndex);

//uart response string of hdmi input switch
#define UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL            "@AV %d TO %c\r\n"
#define UR_STRING_DEVICE_AUTO_SWITCH_MODE               "@AUTO_SWITCH 1\r\n"
#define UR_STRING_DEVICE_MANUAL_SWITCH_MODE             "@AUTO_SWITCH 0\r\n"
#define UR_STRING_DEVICE_FACTORY_DEFAULT        		"@FACTORY_RESET\r\n"
#define UR_STRING_DEVICE_KEY_PAD_LOCKED        		    "@KEYPAD_LOCK 1\r\n"
#define UR_STRING_DEVICE_EDID_MODE                      "@EDID_MODE %d\r\n"
#define UR_STRING_DEVICE_MULTIVIEW_MODE        		    "@MV_MODE %d\r\n"
#define UR_STRING_DEVICE_GET_WIN_INPUT                  "@VIDEO %d TO %c\r\n"
#define UR_STRING_DEVICE_SWITCH_RESOLUTION              "@OUTPUT_RES %d\r\n"
#define UR_STRING_DEVICE_AUDIO_MUTE                     "@AUDIO_MUTE %d\r\n"
#define UR_STRING_DEVICE_AUDIO_SRC                      "@AUDIO_SRC %d\r\n"
#define UR_STRING_DEVICE_UNSUPPORT_RESIZE               "@Unsupport resize\r\n"
#define UR_STRING_DEVICE_BAUDWIDTH_IS_OVER              "@Baudwidth is over\r\n"
#define UR_STRING_DEVICE_WIN_LEVEL_IS_THE_SAME          "@Win level is the same\r\n"
#define UR_STRING_DEVICE_POWER                          "@POWER %d\r\n"
#define UR_STRING_DEVICE_DISPLAY_ON        		        "@DISPLAY 1\r\n"
#define UR_STRING_DEVICE_DISPLAY_OFF                    "@DISPLAY 0\r\n"
#define UR_STRING_DEVICE_RESIZE_WIN                     "@RESIZE_WIM\r\n"
#define UR_STRING_DEVICE_UNSUPPORT_RESETPORT            "@Unsupport Resetport, Please switch to 4 win mode\r\n"

void KeyPressAtPanelLocked(char *s)
{
	sprintf(s, UR_STRING_DEVICE_KEY_PAD_LOCKED);
}

char GetWinPortText(u8 u8WinPort)
{
    char l_s8WinName;
    switch(u8WinPort)
    {
        case emWinType_A:
            l_s8WinName = 'A';
            break;
        case emWinType_B:
            l_s8WinName = 'B';
            break;
        case emWinType_C:
            l_s8WinName = 'C';
            break;
        case emWinType_D:
            l_s8WinName = 'D';
            break;
        default:
            l_s8WinName = ' ';
            break;
    }
    return l_s8WinName;
}

void KeyPressCallBack(uint8_t u8KeyIndex)
{
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstHdmiSwitchParam_t		pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
	pkey_val_stored_t 			pStd = &s_tValStd;
	char 						pPrintStr[200];
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u16     l_u16CharIndex = 0;

	memset(pPrintStr, 0, 200);//clear

	if(isFrontPanelLocked(pStd, u8KeyIndex))
	{
		KeyPressAtPanelLocked(pPrintStr);
	}
	else
	{
		switch(u8KeyIndex)
		{
			case emKeyFunc_AutoSwitch://source
			    if(pstHdmiSw->bAutoSwitch)
				    sprintf(pPrintStr, UR_STRING_DEVICE_AUTO_SWITCH_MODE);
                else
				    sprintf(pPrintStr, UR_STRING_DEVICE_MANUAL_SWITCH_MODE);
				break;

            case emKeyFunc_NoMatchChipID:
                {
                    sprintf(pPrintStr, "@@@@@@@ Device is Error!!!");
                }
                break;
            case emKeyFunc_MVMode1:
            case emKeyFunc_MVMode2:
            case emKeyFunc_MVMode3:
            case emKeyFunc_MVMode4:
            case emKeyFunc_MVModeUser1:
            case emKeyFunc_MVModeUser2:
            case emKeyFunc_MVModeUser3:
            case emKeyFunc_MVModeUser4:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_MULTIVIEW_MODE,pstBasicSpecVar->u8MvMode+1);
                }
                break;

            case emKeyFunc_SwitchHdmi1:
            case emKeyFunc_SwitchHdmi2:
            case emKeyFunc_SwitchHdmi3:
            case emKeyFunc_SwitchHdmi4:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL,SCU41_MV_GetInputPortSelect()+1,GetWinPortText(SCU41_MV_GetWinPortSelect()));
                    if(pstBasicSpecVar->u8WindowNum == emWinNum_1)
                    {
                        sprintf(pPrintStr+12, UR_STRING_DEVICE_AUDIO_SRC,pstBasicSpecVar->u8AudioSel+1);
                    }
                }
                break;

            case emKeyFunc_GetInputByWinA:
            case emKeyFunc_GetInputByWinB:
            case emKeyFunc_GetInputByWinC:
            case emKeyFunc_GetInputByWinD:
                {
                    sprintf(pPrintStr,
                        UR_STRING_DEVICE_GET_WIN_INPUT,
                        pstBasicSpecVar->u8WinSel[u8KeyIndex-emKeyFunc_GetInputByWinA+emWinType_A]+1,
                        GetWinPortText(u8KeyIndex-emKeyFunc_GetInputByWinA+emWinType_A));
                }
                break;
            case emKeyFunc_SetInputToFullByWinA:
            case emKeyFunc_SetInputToFullByWinB:
            case emKeyFunc_SetInputToFullByWinC:
            case emKeyFunc_SetInputToFullByWinD:
                {
                    //sprintf(pPrintStr, UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL,pstBasicSpecVar->u8WinSel[u8KeyIndex-emKeyFunc_GetInputByWinA]+1,GetWinPortText(u8KeyIndex-emKeyFunc_GetInputByWinA));
                    l_u16CharIndex += sprintf(pPrintStr, UR_STRING_DEVICE_HDMI_SWITCH_CHANNEL,pstBasicSpecVar->u8WinSel[emWinType_A]+1,GetWinPortText(emWinType_A));
                    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, UR_STRING_DEVICE_AUDIO_SRC,pstBasicSpecVar->u8AudioSel+1);
                    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, UR_STRING_DEVICE_MULTIVIEW_MODE,pstBasicSpecVar->u8MvMode+1);
                }
                break;
            case emKeyFunc_ResetPort:
                if(pstBasicSpecVar->u8WindowNum != emWinNum_4)
                {
                    sprintf(pPrintStr,UR_STRING_DEVICE_UNSUPPORT_RESETPORT);
                    break;
                }
            case emKeyFunc_GetOutputSate:
            case emKeyFunc_Swap:
                {
                    l_u16CharIndex += sprintf(pPrintStr,"@VIDEO\r\n");
                    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex,"OUT A B C D\r\n");
                    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex,"IN  %d %d %d %d\r\n",pstBasicSpecVar->u8WinSel[emWinType_A]+1,
                                        pstBasicSpecVar->u8WinSel[emWinType_B]+1,
                                        pstBasicSpecVar->u8WinSel[emWinType_C]+1,
                                        pstBasicSpecVar->u8WinSel[emWinType_D]+1);
                    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex,"@AUDIO_SRC %d\r\n",pstBasicSpecVar->u8AudioSel+1);
                }
                break;
            case emKeyFunc_Resize:
                {
                    sprintf(pPrintStr,UR_STRING_DEVICE_RESIZE_WIN);
                }
                break;
            case emKeyFunc_AudioMute:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_AUDIO_MUTE,pstBasicSpecVar->bAudioMute);
                }
                break;
            case emKeyFunc_Resolution:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_SWITCH_RESOLUTION,pstBasicSpecVar->u8ResolutionID+1);
                }
                break;

            case emKeyFunc_AudioSrcHdmi1:
            case emKeyFunc_AudioSrcHdmi2:
            case emKeyFunc_AudioSrcHdmi3:
            case emKeyFunc_AudioSrcHdmi4:
            case emKeyFunc_AudioSrcExtern:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_AUDIO_SRC,pstBasicSpecVar->u8AudioSel+1);
                }
                break;

            case emKeyFunc_UnsupportResize:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_UNSUPPORT_RESIZE);
                }
                break;

            case emKeyFunc_BaudwidthIsOver:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_BAUDWIDTH_IS_OVER);
                }
                break;

            case emKeyFunc_WinLevelIsTheSame:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_WIN_LEVEL_IS_THE_SAME);
                }
                break;
            case emKeyFunc_PrintPowerState:
                {
                    sprintf(pPrintStr, UR_STRING_DEVICE_POWER,pstBasicSpecVar->bPowerOnSta);
                }
                break;
            case emKeyFunc_DisplayOn://display on
                //pstUartKey += 0;
                sprintf(pPrintStr, UR_STRING_DEVICE_DISPLAY_ON);
                break;

            case emKeyFunc_DisplayOff://display off
                //pstUartKey += 1;
                sprintf(pPrintStr, UR_STRING_DEVICE_DISPLAY_OFF);
                break;

			default:
				break;
		}
	}

	CommResMsgSnd((pPrintStr));
}

typedef enum _emTimingType_
{
    em_Pclk_L       = 0,
    em_Pclk_H       = 1,
    em_Htotal_L     = 2,
    em_Htotal_H     = 3,
    em_HDisplayW_L  = 4,
    em_HDisplayW_H  = 5,
    em_HSyncW_L     = 6,
    em_HSyncW_H     = 7,
    em_HFP_L        = 8,
    em_HFP_H        = 9,
    em_HBP_L        = 10,
    em_HBP_H        = 11,
    em_VTotal_L     = 12,
    em_VTotal_H     = 13,
    em_VDisplayW_L  = 14,
    em_VDisplayW_H  = 15,
    em_VSyncW_L     = 16,
    em_VSyncW_H     = 17,
    em_VFP_L        = 18,
    em_VFP_H        = 19,
    em_VBP_L        = 20,
    em_VBP_H        = 21,
    em_POL_INTERLANCE   = 22,
    em_State        = 23,
    em_HVR2nd_H     = 24,
    em_HVR2nd_L     = 25,
    em_VFP2nd_H     = 26,
    em_VFP2nd_L     = 27,
    em_Vic          = 29,
}emTimingType;

void KeyPrintTiming(bool bIsRxPort, u8 *pu8Timing)
{
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstHdmiSwitchParam_t		pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
	char    pPrintStr[1024];
    u16     l_u16CharIndex = 0;
    u8      l_u8PrintIndex = 0;
    u8     l_u8VFrame;

    if(bIsRxPort == TRUE)
    {
        l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, "=====Query Rx[%d] Timing!=====\r\n",pstHdmiSw->u8SwChannel);
    }
    else
    {
        l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, "=====Query Tx Timing!=====\r\n");
    }

    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " PCLK       ==== %d.%03dMHz\r\n",((pu8Timing[em_Pclk_H]<<8)|(pu8Timing[em_Pclk_L]))/100,((pu8Timing[em_Pclk_H]<<8)|(pu8Timing[em_Pclk_L]))%100);
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " HTotal     ==== %d\r\n",(pu8Timing[em_Htotal_H]<<8)|(pu8Timing[em_Htotal_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " HDEW       ==== %d\r\n",(pu8Timing[em_HDisplayW_H]<<8)|(pu8Timing[em_HDisplayW_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " HFPH       ==== %d\r\n",(pu8Timing[em_HFP_H]<<8)|(pu8Timing[em_HFP_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " HSyncW     ==== %d\r\n",(pu8Timing[em_HSyncW_H]<<8)|(pu8Timing[em_HSyncW_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " HBPH       ==== %d\r\n",(pu8Timing[em_HBP_H]<<8)|(pu8Timing[em_HBP_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " HSyncPol   ==== %d\r\n",(pu8Timing[em_POL_INTERLANCE]>>1)& 0x01);
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VTotal     ==== %d\r\n",(pu8Timing[em_VTotal_H]<<8)|(pu8Timing[em_VTotal_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VDEW       ==== %d\r\n",(pu8Timing[em_VDisplayW_H]<<8)|(pu8Timing[em_VDisplayW_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VFPH       ==== %d\r\n",(pu8Timing[em_VFP_H]<<8)|(pu8Timing[em_VFP_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VSyncW     ==== %d\r\n",(pu8Timing[em_VSyncW_H]<<8)|(pu8Timing[em_VSyncW_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VBPH       ==== %d\r\n",(pu8Timing[em_VBP_H]<<8)|(pu8Timing[em_VBP_L]));
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VSyncPol   ==== %d\r\n",(pu8Timing[em_POL_INTERLANCE]>>2)& 0x01);
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InterLaced ==== %d\r\n",(pu8Timing[em_POL_INTERLANCE])& 0x01);

    if(bIsRxPort == TRUE)
    {
        switch((pu8Timing[em_State])& 0x03)
        {
            case 0:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InColor    ==== RGB\r\n");
                break;
            case 1:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InColor    ==== YCbCr422\r\n");
                break;
            case 2:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InColor    ==== YCbCr444\r\n");
                break;
            case 3:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InColor    ==== YCbCr420\r\n");
                break;
            default:
                break;
        }
        l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InFormat   ==== %s\r\n",((pu8Timing[em_State]>>4)&0x01)?"HDMI":"DVI");
        l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " InHDR      ==== %s\r\n",((pu8Timing[em_State]>>5)&0x01)?"HDR":"SDR");
    }
    else
    {
        switch((pu8Timing[em_State])& 0x03)
        {
            case 0:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColor   ==== RGB\r\n");
                break;
            case 1:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColor   ==== YCbCr422\r\n");
                break;
            case 2:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColor   ==== YCbCr444\r\n");
                break;
            case 3:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColor   ==== YCbCr420\r\n");
                break;
            default:
                break;
        }
        //l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColor   ==== %s\r\n",(pu8Timing[em_State])& 0x03);
        l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColorRange = %s\r\n",((pu8Timing[em_State]>>4)&0x01)?"CEA":"VESA");
        switch((pu8Timing[em_State]>>5)& 0x03)
        {
            case 0:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColorDeepth= 8BIT\r\n");
                break;
            case 1:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColorDeepth= 10BIT\r\n");
                break;
            case 2:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColorDeepth= 12BIT\r\n");
                break;
            case 3:
                l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColorDeepth= 16BIT\r\n");
                break;
            default:
                break;
        }
        //l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " OutColorDeepth= %d\r\n",(pu8Timing[em_State]>>5)& 0x03);
    }
    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " VIC        ==== %d\r\n",(pu8Timing[em_Vic]));
    l_u8VFrame = (u8)(((((pu8Timing[em_Pclk_H]<<8)|(pu8Timing[em_Pclk_L]))*10000)
                    /((pu8Timing[em_Htotal_H]<<8)|(pu8Timing[em_Htotal_L]))
                    /((pu8Timing[em_VTotal_H]<<8)|(pu8Timing[em_VTotal_L])))+0.5);

    l_u16CharIndex += sprintf(pPrintStr+l_u16CharIndex, " Timing     ==== %d x %d @ %dHz\r\n",
                        ((pu8Timing[em_HDisplayW_H]<<8)|(pu8Timing[em_HDisplayW_L])),
                        ((pu8Timing[em_VDisplayW_H]<<8)|(pu8Timing[em_VDisplayW_L])),
                        l_u8VFrame);

    while((l_u16CharIndex))
    {
	    CommResMsgSnd((pPrintStr+(l_u8PrintIndex*256)));
        l_u8PrintIndex++;
        if((l_u8PrintIndex*256)>l_u16CharIndex)
        {
            l_u16CharIndex = 0;
            break;
        }
    }
}

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
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    //pstHdmiSwitchParam_t        pstHdmiSw = &pstPrjSaveVar->stHdmiSel;
    uint8_t                     i;

    pCommVar->SendPrintf(UR_STRING_DEVICE_FACTORY_DEFAULT);

    //hdmi switch status
    mapp_UserData_HdmiSelInit();
    SetUserData_HdmiSel();

    #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
    pstPrjSaveVar->stInputSignalDetectMode.m_u8DetectMode = Def_InDetMode;
    SetUserData_InSignalDetMode();
    #endif
    mapp_UserDataEdidIndexInit();
    SetUserData_EdidIndexSave();

    //uart key send
    for(i=0; i<_NUM_UART_STUDY_KEYS; i++)
    {
        mapp_UserDataUartKeyValInit(i);
        SetUserData_UartKeyVal(i);
        delay_ms(700);
    }

    SCU41_MV_SpecBasicVarDefault();
    SetUserData_SpecBasicVar();
    SCU41_MV_SpecCustomEdidVarDefault();
    for(i=0; i<5; i++)
    {
        SetUserData_SpecCustomEdid(i);
    }

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

//eFunc_SwitchInputPort,
static emCmdFuncStaType mapp_CmdfuncSwitchInputPort(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    u8 l_u8InputIndex=0;
    u8 l_u8OutputIndex=0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd)+1; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if(pCommVar->pRecBuffer[l_u8Index] == 0x0D)
        {
            if(pCommVar->pRecBuffer[l_u8Index+1] == 0x0A)
                break;
        }

        if((pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='T')
            &&(pCommVar->pRecBuffer[l_u8Index]!='O')
            &&(l_u8DataIndex==0))
        {
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
            {
                l_state = emCmdError;
                break;
            }
            l_u8InputIndex = (l_u8InputIndex<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='T')
            &&(pCommVar->pRecBuffer[l_u8Index]!='O')
            &&(l_u8DataIndex==2))
        {
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
            {
                l_state = emCmdError;
                break;
            }
            l_u8OutputIndex = HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)-0x0A;
        }
        else if((pCommVar->pRecBuffer[l_u8Index]=='T')
            ||(pCommVar->pRecBuffer[l_u8Index]=='O')
            //||((pCommVar->pRecBuffer[l_u8Index]!=' ')&&(l_u8DataIndex==0))
            )
        {
            continue;
        }
        else
        {
            l_u8DataIndex++;
        }
    }

    if(l_u8InputIndex>4||l_u8OutputIndex>emWinTypeMax)
    {
        l_state = emCmdOutOfRange;
    }

    if(pstBasicVar->u8WindowNum == emWinNum_3)
    {
        if(l_u8OutputIndex > emWinType_C)
        {
            l_state = emCmdOutOfRange;
        }
    }
    else if(pstBasicVar->u8WindowNum == emWinNum_2)
    {
        if(l_u8OutputIndex > emWinType_B)
        {
            l_state = emCmdOutOfRange;
        }
    }
    else if(pstBasicVar->u8WindowNum == emWinNum_1)
    {
        if(l_u8OutputIndex > emWinType_A)
        {
            l_state = emCmdOutOfRange;
        }
    }

    if(l_state == emCmdSucess)
    {
        //SCU41_MV_SetInputPortSelect(l_u8InputIndex-1);
        //SCU41_MV_SetWinPortSelect(l_u8OutputIndex);
        //pITE663xx_STM32_api->SysKeyIn(emKeyFunc_SwitchHdmi1);
        g_emSourceKeyType = eKeyValSource_Uart;
        SCU41_MV_SaveInputPortKeyPress(l_u8InputIndex-1);
        SCU41_MV_SelectPortToWin(l_u8OutputIndex);
    }
    return l_state;

}

//eFunc_GetInputStutus,
static emCmdFuncStaType mapp_CmdfuncGetInputStutus(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType l_state = emCmdSucess;
    u8 l_u8OutputIndex=0;
    bool l_bCheckAll=FALSE;
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==2)
    {
        l_u8OutputIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-'A');
    }
    else if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==0)
    {
        l_u8OutputIndex = 0;
        l_bCheckAll = TRUE;
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8OutputIndex>emWinTypeMax)
        return emCmdOutOfRange;

    if(l_bCheckAll)
    {
        pCommVar->SendPrintf("@VIDEO\r\n");
        pCommVar->SendPrintf("OUT A B C D\r\n");
        pCommVar->SendPrintf("IN  %d %d %d %d\r\n",pstBasicVar->u8WinSel[emWinType_A]+1,
                            pstBasicVar->u8WinSel[emWinType_B]+1,
                            pstBasicVar->u8WinSel[emWinType_C]+1,
                            pstBasicVar->u8WinSel[emWinType_D]+1);
        pCommVar->SendPrintf("@AUDIO_SRC %d\r\n",pstBasicVar->u8AudioSel+1);
    }
    else
    {
        pCommVar->SendPrintf("@VIDEO %d TO %c\r\n",pstBasicVar->u8WinSel[l_u8OutputIndex]+1,GetWinPortText(l_u8OutputIndex));
    }

    return l_state;
}

//eFunc_Edid_Switch,
static emCmdFuncStaType mapp_CmdfuncEdidSwitch(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8EdidIndex = 0;
    uint8_t l_u8EdidMode = 0;
    u8 l_u8InputIndex=0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd)+1; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if(pCommVar->pRecBuffer[l_u8Index] == 0x0D)
        {
            if(pCommVar->pRecBuffer[l_u8Index+1] == 0x0A)
                break;
        }

        if((pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==0))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
                return emCmdError;
            l_u8InputIndex = (l_u8InputIndex<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==1))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
                return emCmdError;
            l_u8EdidIndex = (l_u8EdidIndex<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else
        {
            l_u8DataIndex++;
        }
    }


    if(l_u8EdidIndex>LOAD_EDID_LAST||l_u8InputIndex>4)
    {
        return emCmdOutOfRange;
    }

    switch(l_u8EdidIndex)
    {
        case 1:
            l_u8EdidMode = EDID_MODE_INTERNAL1;
            break;
        case 2:
            l_u8EdidMode = EDID_MODE_INTERNAL2;
            break;
        case 3:
            l_u8EdidMode = EDID_MODE_INTERNAL3;     // edid bypass
            break;
        case 4:
            l_u8EdidMode = EDID_MODE_USER1;
            break;
        default:
            return emCmdOutOfRange;
    }
    if(l_u8InputIndex == 0)
    {
        u8 l_u8InPort;
        for(l_u8InPort=0;l_u8InPort<_HDMI_INPUT_PORT_USED_NUMBER;l_u8InPort++)
        {
            if(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort] != l_u8EdidMode)
            {
                SCU41_MV_EDIDAssignByID(l_u8EdidMode,l_u8InPort);
            }
            pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8InPort+1,l_u8EdidMode);
        }
    }
    else
    {
        if(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InputIndex-1] != l_u8EdidMode)
        {
                SCU41_MV_EDIDAssignByID(l_u8EdidMode,l_u8InputIndex-1);
            }
        pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8InputIndex,l_u8EdidMode);
    }


    return emCmdSucess;
}

//eFunc_Edid_GetMode,
static emCmdFuncStaType mapp_CmdfuncEdidGetMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8InputIndex = 0;
    uint8_t l_u8EdidMode = 0;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==2)
    {
        l_u8InputIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30)*10
                        +(pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+2]-0x30);
    }
    else if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8InputIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8InputIndex>4)
        return emCmdOutOfRange;

    if(l_u8InputIndex>0)
    {
        l_u8InputIndex = l_u8InputIndex-1;
        l_u8EdidMode = mapp_SCU41MVGetEdidIndex(l_u8InputIndex);
        pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8InputIndex+1,pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InputIndex]);
    }
    else
    {
        l_u8EdidMode = mapp_SCU41MVGetEdidIndex(0);
        pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,1,l_u8EdidMode);
        l_u8EdidMode = mapp_SCU41MVGetEdidIndex(1);
        pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,2,l_u8EdidMode);
        l_u8EdidMode = mapp_SCU41MVGetEdidIndex(2);
        pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,3,l_u8EdidMode);
        l_u8EdidMode = mapp_SCU41MVGetEdidIndex(3);
        pCommVar->SendPrintf("%s %d %d\r\n",cmd_list[cmdnum].feedbackstring,4,l_u8EdidMode);
        //return emCmdOutOfRange;
    }
    return emCmdSucess;
}

//eFunc_Edid_SendCustomData,
static emCmdFuncStaType mapp_CmdfuncEdidSendCustomData(pstUartCom_t pCommVar,u8 cmdnum)
{
    UNUSED(cmdnum);
    //UNUSED(pCommVar);

    pCommVar->SendPrintf("@USER_EDID READY\r\n");
    #if _ENABLE_USER_EDID_LEARNING_FUNCTION
    pCommVar->SendPrintf("PLEASE SEND EDID DATA IN 10S\r\n");
    PlatformTimerSet(etUserEdidReceiveTime,10000);
    StartEdidCostumFlag = TRUE;
    g_u8UserEDIDIndex = 0;
    g_bInternalEdidFlag =FALSE;
    #endif

    return emCmdSucess;
}

//eFunc_SwitchHDCP,
static emCmdFuncStaType mapp_CmdfuncSwitchHDCP(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8HdcpIndex=0,l_u8TempVal = 0;
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    l_u8TempVal = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    if(l_u8TempVal == 1)//hdcp 1.4
    {
        l_u8HdcpIndex = tx_hdcp_force_1_4;
    }
    else if(l_u8TempVal == 3)//hdcp mode off
    {
        l_u8HdcpIndex = tx_hdcp_force_off;
    }
    else if(l_u8TempVal == 2)   // hdcp2.2
    {
        l_u8HdcpIndex = tx_hdcp_force_2_2;
    }
    else
        l_emRtState = emCmdOutOfRange;

    if(l_emRtState == emCmdSucess)
    {
        pstBasicVar->u8HdcpMode = l_u8HdcpIndex;
        #if _ENABLE_CHIP_ITE6615
        g_bEnableHdcpSwitch = TRUE;
        g_u8OutputHdcpSta = pstBasicVar->u8HdcpMode;
        #endif
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);
        SetUserData_SpecBasicVar();
    }
    return l_emRtState;
}

//eFunc_GetHDCPState,
static emCmdFuncStaType mapp_CmdfuncGetHDCPState(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8HdcpIndex=0;

    if(pstBasicVar->u8HdcpMode == tx_hdcp_force_1_4)
        l_u8HdcpIndex = 1;
    else if(pstBasicVar->u8HdcpMode == tx_hdcp_force_off)
        l_u8HdcpIndex = 3;
    else if(pstBasicVar->u8HdcpMode == tx_hdcp_force_2_2)
        l_u8HdcpIndex = 2;
    else
        return emCmdOutOfRange;
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8HdcpIndex);
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
            mapp_SetPowerState(TRUE,TRUE);
            break;
        case 0:
            mapp_SetPowerState(FALSE,TRUE);
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
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
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
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 1:
            l_u8BaudrateID = baud_rate_115200;
            break;
        case 2:
            l_u8BaudrateID = baud_rate_57600;
            break;
        case 3:
            l_u8BaudrateID = baud_rate_38400;
            break;
        case 4:
            l_u8BaudrateID = baud_rate_19200;
            break;
        case 5:
            l_u8BaudrateID = baud_rate_9600;
            break;
        case 6:
            l_u8BaudrateID = baud_rate_4800;
            break;
        case 7:
            l_u8BaudrateID = baud_rate_2400;
            break;
        default:
            return emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        pstBasicSpecVar->u8baudrateId = l_u8BaudrateID;
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);//mapp_GetRealBaudrate(l_u8BaudrateID));
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
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8BaudrateID=0;

    switch(pstBasicSpecVar->u8baudrateId)
    {
        case baud_rate_115200:
            l_u8BaudrateID = 1;
            break;
        case baud_rate_57600:
            l_u8BaudrateID = 2;
            break;
        case baud_rate_38400:
            l_u8BaudrateID = 3;
            break;
        case baud_rate_19200:
            l_u8BaudrateID = 4;
            break;
        case baud_rate_9600:
            l_u8BaudrateID = 5;
            break;
        case baud_rate_4800:
            l_u8BaudrateID = 6;
            break;
        case baud_rate_2400:
            l_u8BaudrateID = 7;
            break;
        default:
            return emCmdOutOfRange;
            //break;
    }
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8BaudrateID);
    return l_emRtState;
}

//eFunc_SetAutoSwitchMode,
static emCmdFuncStaType mapp_CmdfuncSetAutoSwitchMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
    bool l_bAutoSwitchSta=FALSE;
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            l_bAutoSwitchSta = FALSE;
            break;
        case 1:
            l_bAutoSwitchSta = TRUE;
            g_bAutoSwPowerOnStatus = TRUE;
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }
    if(l_emRtState == emCmdSucess)
    {
        if(l_bAutoSwitchSta!=pstPrjSaveVar->stHdmiSel.bAutoSwitch)
        {
            pstPrjSaveVar->stHdmiSel.bAutoSwitch = l_bAutoSwitchSta;
            SetUserData_HdmiSel();
        }
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_bAutoSwitchSta);
    }
    return emCmdSucess;
}

//eFunc_GetAutoSwitchMode,
static emCmdFuncStaType mapp_CmdfuncGetAutoSwitchMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectGlobalAllVar_t 	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstPrjSaveVar->stHdmiSel.bAutoSwitch);
    return l_emRtState;
}

//eFunc_SetCECDisplay,
static emCmdFuncStaType mapp_CmdfuncSetCECDisplay(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            //l_bAutoSwitchSta = FALSE;
            //pITE663xx_STM32_api->SysKeyIn(emKeyFunc_DisplayOff);
            break;
        case 1:
            //l_bAutoSwitchSta = TRUE;
            //pITE663xx_STM32_api->SysKeyIn(emKeyFunc_DisplayOn);
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }
    return l_emRtState;
}

#if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
//eFunc_SendUserCecCmd,
// #CEC <XX,XX,XX...>
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
        if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==0))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8DevID = (l_u8DevID<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==1))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8Addr = (l_u8Addr<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==2))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8Opcode = (l_u8Opcode<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex>=3))
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

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
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

    pCommVar->SendPrintf("@CEC send to device: %d\r\n", l_u8DevID);
    pCommVar->SendPrintf("  Header : 0x%02x\r\n", l_u8Addr);
    pCommVar->SendPrintf("  Opcode : 0x%02x\r\n", l_u8Opcode);
    if((l_u8DataIndex-3)>0)
    {
        u8 i;
        pCommVar->SendPrintf("  Message: ");
        for(i=0;i<(l_u8DataIndex-3);i++)
        {
            pCommVar->SendPrintf("0x%02x,",l_u8TempData[i]);
        }
        pCommVar->SendPrintf("\r\n");
    }
    delay_ms(5);
    cec_cmd_send_to_device((l_u8DevID!=CEC_OPERATE_INPUT)?l_u8DevID-1: CEC_OPERATE_INPUT,l_u8Addr,l_u8Opcode,l_u8TempData,l_u8DataIndex-3,false);
    //pCommVar->SendPrintf("%s", pCommVar->pRecBuffer);
    return emCmdSucess;
}
#endif

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
            case eFunc_SwitchInputPort:
                {
                    pCommVar->SendPrintf("@Select the input source\r\n");
                    pCommVar->SendPrintf("#SET_AV INPARAM TO OUTPARAM\r\n");
                    pCommVar->SendPrintf("INPARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("OUTPARAM = A ~ D\r\n");
                }
                break;
            case eFunc_GetInputStutus:
                {
                    pCommVar->SendPrintf("@Get the current input source\r\n");
                    pCommVar->SendPrintf("#GET_AV PARAM1\r\n");
                    pCommVar->SendPrintf("NO PARAMETER = GET ALL WINDOWS SELECTED INPUT STATUS\r\n");
                    pCommVar->SendPrintf("PARAM1 =  A ~ D\r\n");
                }
                break;
            case eFunc_Edid_Switch:
                {
                    pCommVar->SendPrintf("@Set the EDID mode\r\n");      // description
                    pCommVar->SendPrintf("#SET_EDID_MODE PARAM1 PARAM2\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - 1920x1080 60HZ PCM 2CH\r\n");
                    pCommVar->SendPrintf("2 - 3840x2160 30HZ PCM 2CH\r\n");
                    pCommVar->SendPrintf("3 - BYPASS\r\n");
                    pCommVar->SendPrintf("4 - USER\r\n");
                }
                break;
            case eFunc_Edid_GetMode:
                {
                    pCommVar->SendPrintf("@Get the EDID mode\r\n");
                    pCommVar->SendPrintf("#GET_EDID_MODE PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_Edid_SendCustomData:
                {
                    pCommVar->SendPrintf("@Upload the user defined EDID\r\n");
                    pCommVar->SendPrintf("#UPLOAD_USER_EDID\r\n");
                }
                break;
            case eFunc_SwitchHDCP:
                {
                    pCommVar->SendPrintf("@Set the HDCP mode for output port\r\n");
                    pCommVar->SendPrintf("#SET_OUTPUT_HDCP PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 3\r\n");
                    pCommVar->SendPrintf("1 - HDCP1.4\r\n");
                    pCommVar->SendPrintf("2 - HDCP2.2\r\n");
                    pCommVar->SendPrintf("3 - OFF\r\n");
                }
                break;
            case eFunc_GetHDCPState:
                {
                    pCommVar->SendPrintf("@Get the HDCP mode of output port\r\n");
                    pCommVar->SendPrintf("#GET_OUTPUT_HDCP\r\n");
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
                    pCommVar->SendPrintf("PARAM = 1 ~ 7\r\n");
                    pCommVar->SendPrintf("1 - 115200\r\n");
                    pCommVar->SendPrintf("2 - 57600\r\n");
                    pCommVar->SendPrintf("3 - 38400\r\n");
                    pCommVar->SendPrintf("4 - 19200\r\n");
                    pCommVar->SendPrintf("5 - 9600\r\n");
                    pCommVar->SendPrintf("6 - 4800\r\n");
                    pCommVar->SendPrintf("7 - 2400\r\n");
                }
                break;
            case eFunc_GetRs232Baudrate:
                {
                    pCommVar->SendPrintf("@Get the RS232 baud rate\r\n");
                    pCommVar->SendPrintf("#GET_RS232_BAUD\r\n");
                }
                break;
            case eFunc_SetAutoSwitchMode:
                {
                    pCommVar->SendPrintf("@Enable/disable auto switching mode\r\n");
                    pCommVar->SendPrintf("#SET_AUTO_SWITCH PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;
            case eFunc_GetAutoSwitchMode:
                {
                    pCommVar->SendPrintf("@Get the auto switching status\r\n");
                    pCommVar->SendPrintf("#GET_AUTO_SWITCH\r\n");
                }
                break;
            case eFunc_SetCECDisplay:
                {
                    pCommVar->SendPrintf("@Power on/off display（Use CEC PORT and RS232 for the the display)\r\n");
                    pCommVar->SendPrintf("#SET_DISPLAY PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - OFF\r\n");
                    pCommVar->SendPrintf("1 - ON\r\n");
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
            #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
            case eFunc_SetOffMsgCounter:
                {
                    pCommVar->SendPrintf("@Set the DISPLAY OFF message loop counter\r\n");
                    pCommVar->SendPrintf("#SET_OFF_CNT PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 2 (loop counter)\r\n");
                }
                break;
            case eFunc_GetOffMsgCounter:
                {
                    pCommVar->SendPrintf("@Get the DISPLAY OFF message loop counter\r\n");
                    pCommVar->SendPrintf("#GET_OFF_CNT\r\n");
                }
                break;
            case eFunc_SetOffMsgCntDelayTime:
                {
                    pCommVar->SendPrintf("@Set the DISPLAY OFF message loop delay time\r\n");
                    pCommVar->SendPrintf("#SET_OFF_DELAY PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 5 ~ 100 (1=100ms)\r\n");
                }
                break;
            case eFunc_GetOffMsgCntDelayTime:
                {
                    pCommVar->SendPrintf("@Get the DISPLAY OFF message loop delay time\r\n");
                    pCommVar->SendPrintf("#GET_OFF_DELAY\r\n");
                }
                break;
            case eFunc_SetDisplayOnAsciiCmd:
                {
                    pCommVar->SendPrintf("@Send the command 'XXXX' to the 3th device while the system enter power on mode.\r\n");
                    pCommVar->SendPrintf("#SET_ON_(PARAM):XXXX\r\n");
                    pCommVar->SendPrintf("PARAM = 01~07\r\n");
                    pCommVar->SendPrintf("01 - 115200\r\n");
                    pCommVar->SendPrintf("02 - 57600\r\n");
                    pCommVar->SendPrintf("03 - 38400\r\n");
                    pCommVar->SendPrintf("04 - 19200\r\n");
                    pCommVar->SendPrintf("05 - 9600\r\n");
                    pCommVar->SendPrintf("06 - 4800\r\n");
                    pCommVar->SendPrintf("07 - 2400\r\n");
                    pCommVar->SendPrintf("XXXX = Command to be send\r\n");
                }
                break;
            case eFunc_SetDisplayOffAsciiCmd:
                {
                    pCommVar->SendPrintf("@Send the command 'XXXX' to the 3th device while the system enter power off mode.\r\n");
                    pCommVar->SendPrintf("#SET_OF_(PARAM):XXXX\r\n");
                    pCommVar->SendPrintf("PARAM = 01~07\r\n");
                    pCommVar->SendPrintf("01 - 115200\r\n");
                    pCommVar->SendPrintf("02 - 57600\r\n");
                    pCommVar->SendPrintf("03 - 38400\r\n");
                    pCommVar->SendPrintf("04 - 19200\r\n");
                    pCommVar->SendPrintf("05 - 9600\r\n");
                    pCommVar->SendPrintf("06 - 4800\r\n");
                    pCommVar->SendPrintf("07 - 2400\r\n");
                    pCommVar->SendPrintf("XXXX = Command to be send\r\n");
                }
                break;
            case eFunc_SetDisplayOnHexCmd:
                {
                    pCommVar->SendPrintf("Send the HEX command 'XXXX' to the 3th device while the system enter power on mode.\r\n");
                    pCommVar->SendPrintf("#SET_ON_(PARAM):XXXX\r\n");
                    pCommVar->SendPrintf("PARAM = 01~07\r\n");
                    pCommVar->SendPrintf("01 - 115200\r\n");
                    pCommVar->SendPrintf("02 - 57600\r\n");
                    pCommVar->SendPrintf("03 - 38400\r\n");
                    pCommVar->SendPrintf("04 - 19200\r\n");
                    pCommVar->SendPrintf("05 - 9600\r\n");
                    pCommVar->SendPrintf("06 - 4800\r\n");
                    pCommVar->SendPrintf("07 - 2400\r\n");
                    pCommVar->SendPrintf("XXXX = Command to be send\r\n");
                }
                break;
            case eFunc_SetDisplayOffHexCmd:
                {
                    pCommVar->SendPrintf("@Send the HEX command 'XX XX' to the 3th device while the system enter power off mode.\r\n");
                    pCommVar->SendPrintf("#SET_OF_(PARAM):XX XX\r\n");
                    pCommVar->SendPrintf("PARAM = 01~07\r\n");
                    pCommVar->SendPrintf("01 - 115200\r\n");
                    pCommVar->SendPrintf("02 - 57600\r\n");
                    pCommVar->SendPrintf("03 - 38400\r\n");
                    pCommVar->SendPrintf("04 - 19200\r\n");
                    pCommVar->SendPrintf("05 - 9600\r\n");
                    pCommVar->SendPrintf("06 - 4800\r\n");
                    pCommVar->SendPrintf("07 - 2400\r\n");
                    pCommVar->SendPrintf("XXXX = Command to be send\r\n");
                }
                break;
            #endif
            #if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
            case eFunc_SetAutoSyncDetectActionByCec:
                {
                    pCommVar->SendPrintf("@Enable/Disable auto detect signal to do CEC action\r\n");
                    pCommVar->SendPrintf("#SET_SYNCACT_CEC PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;
            case eFunc_GetAutoSyncDetectActionByCec:
                {
                    pCommVar->SendPrintf("@Get the CEC action state by auto detect signal\r\n");
                    pCommVar->SendPrintf("#GET_SYNCACT_CEC\r\n");
                }
                break;
            case eFunc_SetAutoSyncDetectActionByRs232:
                {
                    pCommVar->SendPrintf("@Enable/Disable auto detect signal to do RS232 action\r\n");
                    pCommVar->SendPrintf("#SET_SYNCACT_RS232 PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                 break;
            case eFunc_GetAutoSyncDetectActionByRs232:
                {
                    pCommVar->SendPrintf("@Get the RS232 action state by auto detect signal\r\n");
                    pCommVar->SendPrintf("#GET_SYNCACT_RS232\r\n");
                }
                break;
            case eFunc_SetAutoSyncDisplayOffDelayTime:
                {
                    pCommVar->SendPrintf("@Set the time while no signal to do cec and RS232 action\r\n");
                    pCommVar->SendPrintf("#SET_DTIME PARAM1:PARAM2\r\n");
                    pCommVar->SendPrintf("PARAM1 = 0 ~ 30 minus\r\n");
                    pCommVar->SendPrintf("PARAM2 = 0 ~ 1800 second\r\n");
                    pCommVar->SendPrintf("(PS: All the time in 0s ~ 30m)\r\n");
                }
                break;
            case eFunc_GetAutoSyncDisplayOffDelayTime:
                {
                    pCommVar->SendPrintf("@Get the time to do cec and RS232 action\r\n");
                    pCommVar->SendPrintf("#GET_DTIME\r\n");
                }
                break;
            #if _ENABLE_AUTO_POWER_FUNCTION
            case eFunc_SetAutoSyncToPowerOnOff:
                {
                    pCommVar->SendPrintf("@Enable/Disable auto power function\r\n");
                    pCommVar->SendPrintf("#SET_AUTO_POWER PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;
            case eFunc_GetAutoSyncToPowerOnOff:
                {
                    pCommVar->SendPrintf("@Get the auto power function state\r\n");
                    pCommVar->SendPrintf("#GET_AUTO_POWER\r\n");
                }
                break;
            #endif
            #endif

            #if _ENABLE_KEYPAD_LOCKED_FUNCTION
            case eFunc_SetKeyPadLocked:
                {
                    pCommVar->SendPrintf("@Lock/unlock the keypad\r\n");
                    pCommVar->SendPrintf("#SET_KEYPAD_LOCK PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;
            case eFunc_GetKeyPadLocked:
                {
                    pCommVar->SendPrintf("@Get the keypad locking status\r\n");
                    pCommVar->SendPrintf("#GET_KEYPAD_LOCK\r\n");
                }
                break;
            #endif

            #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
            case eFunc_SetInputSignalDetectMode:
                {
                    pCommVar->SendPrintf("@Set the detected mode by input signal\r\n");
                    pCommVar->SendPrintf("#SET_DETECT_MODE PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - TMDS MODE\r\n");
                    pCommVar->SendPrintf("1 - HPD MODE\r\n");
                }
                break;
            case eFunc_GetInputSignalDetectMode:
                {
                    pCommVar->SendPrintf("@Get the detected mode by input signal\r\n");
                    pCommVar->SendPrintf("#GET_DETECT_MODE\r\n");
                }
                break;
            #endif
            #if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
            case eFunc_Edid_LoadInternalEdidData:
                {
                }
                break;
            #endif

            #if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
            case eFunc_LoadIteChipRegisterData:
                {
                }
                break;
            case eFunc_WriteChipRegisterData:
                {
                }
                break;
            case eFunc_ReadChipRegisterData:
                {
                }
                break;
            #endif


            case eFunc_SetResolution:
                {
                    pCommVar->SendPrintf("@Set the output resoultion\r\n");
                    pCommVar->SendPrintf("#SET_OUTPUT_RES PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 8\r\n");
                    pCommVar->SendPrintf("1 - 1024x768 60 HZ\r\n");
                    pCommVar->SendPrintf("2 - 1280x720 60 HZ\r\n");
                    pCommVar->SendPrintf("3 - 1360x768 60 HZ\r\n");
                    pCommVar->SendPrintf("4 - 1600x1200 60 Hz\r\n");
                    pCommVar->SendPrintf("5 - 1920x1080 60 HZ\r\n");
                    pCommVar->SendPrintf("6 - 1920x1200 60 HZ\r\n");
                    pCommVar->SendPrintf("7 - 3840x2160 30 HZ\r\n");
                    pCommVar->SendPrintf("8 - AUTO\r\n");
                }
                break;
            case eFunc_GetResolution:
                {
                    pCommVar->SendPrintf("@Get the output resolution\r\n");
                    pCommVar->SendPrintf("#GET_OUTPUT_RES\r\n");
                }
                break;
            case eFunc_SetMultiviewMode:
                {
                    pCommVar->SendPrintf("@Set multiview mode\r\n");
                    pCommVar->SendPrintf("#SET_MV_MODE PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 20\r\n");
                    pCommVar->SendPrintf("1 - 1 WINDOWS Full\r\n");
                    pCommVar->SendPrintf("2 - 2 WINDOWS PBP\r\n");
                    pCommVar->SendPrintf("3 - 3 WINDOWS 2U1D\r\n");
                    pCommVar->SendPrintf("4 - 4 WINDOWS SAME SIZE\r\n");
                    pCommVar->SendPrintf("5 - 2 WINDOWS PIP LU\r\n");
                    pCommVar->SendPrintf("6 - 2 WINDOWS PIP LD\r\n");
                    pCommVar->SendPrintf("7 - 2 WINDOWS PIP RU\r\n");
                    pCommVar->SendPrintf("8 - 2 WINDOWS PIP RD\r\n");
                    pCommVar->SendPrintf("9 - 4 WINDOWS PBP 3L1R\r\n");
                    pCommVar->SendPrintf("10 - 4 WINDOWS PBP 1L3R\r\n");
                    pCommVar->SendPrintf("11 - 4 WINDOWS PBP 3U1D\r\n");
                    pCommVar->SendPrintf("12 - 4 WINDOWS PBP 1U3D\r\n");
                    pCommVar->SendPrintf("13 - 4 WINDOWS PIP 1F3L\r\n");
                    pCommVar->SendPrintf("14 - 4 WINDOWS PIP 1F3R\r\n");
                    pCommVar->SendPrintf("15 - 4 WINDOWS PIP 1F3U\r\n");
                    pCommVar->SendPrintf("16 - 4 WINDOWS PIP 1F3D\r\n");
                    pCommVar->SendPrintf("17 - USER CONFIG 1\r\n");
                    pCommVar->SendPrintf("18 - USER CONFIG 2\r\n");
                    pCommVar->SendPrintf("19 - USER CONFIG 3\r\n");
                    pCommVar->SendPrintf("20 - USER CONFIG 4\r\n");
                }
                break;
            case eFunc_GetMultiviewMode:
                {
                    pCommVar->SendPrintf("@Get multiview mode\r\n");
                    pCommVar->SendPrintf("#GET_MV_MODE\r\n");
                }
                break;
            case eFunc_SetAudioSource:
                {
                    pCommVar->SendPrintf("@Set the audio output source\r\n");
                    pCommVar->SendPrintf("#SET_AUDIO_SRC PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_GetAudioSource:
                {
                    pCommVar->SendPrintf("@Get the audio output source\r\n");
                    pCommVar->SendPrintf("#GET_AUDIO_SRC\r\n");
                }
                break;

            case eFunc_SetAudioMix:
                {
                    pCommVar->SendPrintf("@Enable/Disable audio mix\r\n");
                    pCommVar->SendPrintf("#SET_AUDIO_MIX PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;
            case eFunc_GetAudioMix:
                {
                    pCommVar->SendPrintf("@Get audio mix status\r\n");
                    pCommVar->SendPrintf("#GET_AUDIO_MIX\r\n");
                }
                break;

            case eFunc_SetAudioMute:
                {
                    pCommVar->SendPrintf("@Mute/Unmute audio\r\n");
                    pCommVar->SendPrintf("#SET_AUDIO_MUTE PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;

            case eFunc_GetAudioMute:
                {
                    pCommVar->SendPrintf("@Get the audio mute status\r\n");
                    pCommVar->SendPrintf("#GET_AUDIO_MUTE\r\n");
                }
                break;

            case eFunc_SetCecSrcMenu:
                {
                    pCommVar->SendPrintf("@Send CEC MENU command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_MENU PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcUp:
                {
                    pCommVar->SendPrintf("@Send CEC UP command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_UP PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcDown:
                {
                    pCommVar->SendPrintf("@Send CEC DOWN command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_DOWN PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcLeft:
                {
                    pCommVar->SendPrintf("@Send CEC LEFT command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_LEFT PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcRight:
                {
                    pCommVar->SendPrintf("@Send CEC RIGHT command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_RIGHT PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcBack:
                {
                    pCommVar->SendPrintf("@Send CEC BACK command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_BACK PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcEnter:
                {
                    pCommVar->SendPrintf("@Send CEC ENTER command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_ENTER PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcOn:
                {
                    pCommVar->SendPrintf("@Send CEC ON command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_ON PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcOff:
                {
                    pCommVar->SendPrintf("@Send CEC OFF command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_OFF PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcStop:
                {
                    pCommVar->SendPrintf("@Send CEC STOP command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_STOP PARAM");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcPlay:
                {
                    pCommVar->SendPrintf("@Send CEC PLAY command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_PLAY PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcPause:
                {
                    pCommVar->SendPrintf("@Send CEC PAUSE command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_PAUSE PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcPrev:
                {
                    pCommVar->SendPrintf("@Send CEC PREV command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_PREV PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcNext:
                {
                    pCommVar->SendPrintf("@Send CEC NEXT command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_NEXT PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcRewind:
                {
                    pCommVar->SendPrintf("@Send CEC rewind command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_REW PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecSrcFastForward:
                {
                    pCommVar->SendPrintf("@Send CEC fast-forward command to source\r\n");
                    pCommVar->SendPrintf("#SET_SRC_FF PARAM\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                }
                break;
            case eFunc_SetCecDisOn:
                {
                    pCommVar->SendPrintf("@Send CEC ON command to displayer\r\n");
                    pCommVar->SendPrintf("#SET_DIS_ON\r\n");
                }
                break;
            case eFunc_SetCecDisOff:
                {
                    pCommVar->SendPrintf("@Send CEC OFF command to displayer\r\n");
                    pCommVar->SendPrintf("#SET_DIS_OFF\r\n");
                }
                break;
            case eFunc_SetCecDisSource:
                {
                    pCommVar->SendPrintf("@Send CEC SOURCE command to displayer\r\n");
                    pCommVar->SendPrintf("#SET_DIS_SOURCE\r\n");
                }
                break;
            case eFunc_SetCecDisMute:
                {
                    pCommVar->SendPrintf("@Send CEC MUTE command to displayer\r\n");
                    pCommVar->SendPrintf("#SET_DIS_MUTE\r\n");
                }
                break;
            case eFunc_SetCecDisVolPlus:
                {
                    pCommVar->SendPrintf("@Send CEC volume plus command to displayer\r\n");
                    pCommVar->SendPrintf("#SET_DIS_VOL+\r\n");
                }
                break;
            case eFunc_SetCecDisVolMinus:
                {
                    pCommVar->SendPrintf("@Send CEC volume minus command to displayer\r\n");
                    pCommVar->SendPrintf("#SET_DIS_VOL-\r\n");
                }
                break;

            case eFunc_EnableMvUserConfig:
                {
                    pCommVar->SendPrintf("@Set Multiview user mode config\r\n");
                    pCommVar->SendPrintf("#SET_MV_USER PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER1\r\n");
                    pCommVar->SendPrintf("2 - USER2\r\n");
                    pCommVar->SendPrintf("3 - USER3\r\n");
                    pCommVar->SendPrintf("4 - USER4\r\n");
                }
                break;
            case eFunc_ConfirmMvUserSetting:
                {
                    pCommVar->SendPrintf("@Confirm the multiview user mode\r\n");
                    pCommVar->SendPrintf("#CFM_MV_USER\r\n");
                }
                break;
            case eFunc_SetSizeAndPositionWinA:
                {
                    pCommVar->SendPrintf("@Set win A size and position\r\n");
                    pCommVar->SendPrintf("#SET_WINA (PARAM) <X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_SetSizeAndPositionWinB:
                {
                    pCommVar->SendPrintf("@Set win B size and position\r\n");
                    pCommVar->SendPrintf("#SET_WINB (PARAM) <X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_SetSizeAndPositionWinC:
                {
                    pCommVar->SendPrintf("@Set win C size and position\r\n");
                    pCommVar->SendPrintf("#SET_WINC (PARAM) <X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_SetSizeAndPositionWinD:
                {
                    pCommVar->SendPrintf("@Set win D size and position\r\n");
                    pCommVar->SendPrintf("#SET_WIND (PARAM) <X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_CfgSizeAndPositionWinA:
                {
                    pCommVar->SendPrintf("@Config win A size and position\r\n");
                    pCommVar->SendPrintf("#CFG_WINA <PARAM1 PARAM2 X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_CfgSizeAndPositionWinB:
                {
                    pCommVar->SendPrintf("@Config win B size and position\r\n");
                    pCommVar->SendPrintf("#CFG_WINB <PARAM1 PARAM2 X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_CfgSizeAndPositionWinC:
                {
                    pCommVar->SendPrintf("@Config win C size and position\r\n");
                    pCommVar->SendPrintf("#CFG_WINC <PARAM1 PARAM2 X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_CfgSizeAndPositionWinD:
                {
                    pCommVar->SendPrintf("@Config win D size and position\r\n");
                    pCommVar->SendPrintf("#CFG_WIND <PARAM1 PARAM2 X1,Y1 X2,Y2>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X1 = 0~100 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y1 = 0~100 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("X2 = 0~100 (Windows end X position)\r\n");
                    pCommVar->SendPrintf("Y2 = 0~100 (Windows end Y position)\r\n");
                }
                break;
            case eFunc_PixelCfgSizeAndPositionWinA:
                {
                    pCommVar->SendPrintf("@Config win A size and position\r\n");
                    pCommVar->SendPrintf("#PCFG_WINA <PARAM1 PARAM2 X,Y W,H>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X = 0~3840 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y = 0~2160 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("W = 0~3840 (Windows width size)\r\n");
                    pCommVar->SendPrintf("H = 0~2160 (Windows height size)\r\n");
                    pCommVar->SendPrintf("X,Y,W,H Max value base on output resolution\r\n");
                }
                break;
            case eFunc_PixelCfgSizeAndPositionWinB:
                {
                    pCommVar->SendPrintf("@Config win B size and position\r\n");
                    pCommVar->SendPrintf("#PCFG_WINB <PARAM1 PARAM2 X,Y W,H>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X = 0~3840 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y = 0~2160 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("W = 0~3840 (Windows width size)\r\n");
                    pCommVar->SendPrintf("H = 0~2160 (Windows height size)\r\n");
                    pCommVar->SendPrintf("X,Y,W,H Max value base on output resolution\r\n");
                }
                break;
            case eFunc_PixelCfgSizeAndPositionWinC:
                {
                    pCommVar->SendPrintf("@Config win C size and position\r\n");
                    pCommVar->SendPrintf("#PCFG_WINC <PARAM1 PARAM2 X,Y W,H>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X = 0~3840 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y = 0~2160 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("W = 0~3840 (Windows width size)\r\n");
                    pCommVar->SendPrintf("H = 0~2160 (Windows height size)\r\n");
                    pCommVar->SendPrintf("X,Y,W,H Max value base on output resolution\r\n");
                }
                break;
            case eFunc_PixelCfgSizeAndPositionWinD:
                {
                    pCommVar->SendPrintf("@Config win D size and position\r\n");
                    pCommVar->SendPrintf("#PCFG_WIND <PARAM1 PARAM2 X,Y W,H>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
                    pCommVar->SendPrintf("X = 0~3840 (Windows start X position)\r\n");
                    pCommVar->SendPrintf("Y = 0~2160 (Windows start Y position)\r\n");
                    pCommVar->SendPrintf("W = 0~3840 (Windows width size)\r\n");
                    pCommVar->SendPrintf("H = 0~2160 (Windows height size)\r\n");
                    pCommVar->SendPrintf("X,Y,W,H Max value base on output resolution\r\n");
                }
                break;
            case eFunc_SaveScreenToUserMode:
                {
                    pCommVar->SendPrintf("@Save screen to user mode\r\n");
                    pCommVar->SendPrintf("#SAVE_SCREEN PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - USER 1\r\n");
                    pCommVar->SendPrintf("2 - USER 2\r\n");
                    pCommVar->SendPrintf("3 - USER 3\r\n");
                    pCommVar->SendPrintf("4 - USER 4\r\n");
                }
            case eFunc_CleanScreen:
                {
                    pCommVar->SendPrintf("@Clean screen\r\n");
                    pCommVar->SendPrintf("#CLR_SCREEN PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 4\r\n");
                    pCommVar->SendPrintf("0 - ALL WINDOWS\r\n");
                    pCommVar->SendPrintf("1 - WIN A\r\n");
                    pCommVar->SendPrintf("2 - WIN B\r\n");
                    pCommVar->SendPrintf("3 - WIN C\r\n");
                    pCommVar->SendPrintf("4 - WIN D\r\n");
                }
                break;
            case eFunc_GetIpAddress:
                {
                    pCommVar->SendPrintf("@Get the IP to access GUI\r\n");
                    pCommVar->SendPrintf("#GET_IP_ADDR\r\n");
                }
                break;
            case eFunc_SetIpAddress:
                {
                    pCommVar->SendPrintf("@Set the GUI IP address\r\n");
                    pCommVar->SendPrintf("#SET_IP <XXX.XXX.XXX.XXX>\r\n");
                    pCommVar->SendPrintf("XXX = 0 ~ 255(IP ADDRESS)\r\n");
                }
                break;
            case eFunc_GiveIpAddress:
                {
                    pCommVar->SendPrintf("@Send the IP address from GUI\r\n");
                    pCommVar->SendPrintf("#GIVE_IP <XXX.XXX.XXX.XXX>\r\n");
                    pCommVar->SendPrintf("XXX = 0 ~ 255(IP ADDRESS)\r\n");
                }
                break;
            case eFunc_SetSwapSource:
                {
                    pCommVar->SendPrintf("@Swap input source\r\n");
                    pCommVar->SendPrintf("#SET_SWAP_SRC\r\n");
                }
                break;
            case eFunc_SetResizeWin:
                {
                    pCommVar->SendPrintf("@Resize display windows\r\n");
                    pCommVar->SendPrintf("#SET_RESIZE_WIM\r\n");
                }
                break;
            case eFunc_GetAllSystemSta:
                {
                    pCommVar->SendPrintf("@Get the system status\r\n");
                    pCommVar->SendPrintf("#GET_STATUS\r\n");
                }
                break;
            case eFunc_SetWinLayerLevel:
                {
                    pCommVar->SendPrintf("@Set the win level\r\n");
                    pCommVar->SendPrintf("#SET_WIN_LEVEL PARAM1 <PARAM2 PARAM3 PARAM4 PARAM5>\r\n");
                    pCommVar->SendPrintf("PARAM1 = 1 ~ 4(Level mode)\r\n");
                    pCommVar->SendPrintf("PARAM2 = 1 ~ 4(WinA Level)\r\n");
                    pCommVar->SendPrintf("PARAM3 = 1 ~ 4(WinB Level)\r\n");
                    pCommVar->SendPrintf("PARAM4 = 1 ~ 4(WinC Level)\r\n");
                    pCommVar->SendPrintf("PARAM5 = 1 ~ 4(WinD Level)\r\n");
                    pCommVar->SendPrintf("(The PARAM2 PARAM3 PARAM4 PARAM5 cannot be the same)\r\n");
                }
                break;
            case eFunc_GetWinLayerLevel:
                {
                    pCommVar->SendPrintf("@Get the win level\r\n");
                    pCommVar->SendPrintf("#GET_WIN_LEVEL PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4(Level mode)\r\n");
                }
                break;
            case eFunc_SetProductName:
                {
                    pCommVar->SendPrintf("@Set the product name\r\n");
                    pCommVar->SendPrintf("#SET_NAME PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = PRODUCT NAME(max lenth = 50)\r\n");
                }
                break;
            case eFunc_GetProductName:
                {
                    pCommVar->SendPrintf("@Get the product name\r\n");
                    pCommVar->SendPrintf("#GET_NAME\r\n");
                }
                break;
            case eFunc_ResetProductName:
                {
                    pCommVar->SendPrintf("@Reset product name\r\n");
                    pCommVar->SendPrintf("#RESET_NAME\r\n");
                }
                break;
            #if _ENABLE_SWITCH_PORT_AND_AUDIO
            case eFunc_SetFullModeToSwitchAudio:
                {
                    pCommVar->SendPrintf("@Set audio switch by input when full mode is select\r\n");
                    pCommVar->SendPrintf("#SET_FULL_SWAUD PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 0 ~ 1\r\n");
                    pCommVar->SendPrintf("0 - DISABLED\r\n");
                    pCommVar->SendPrintf("1 - ENABLED\r\n");
                }
                break;
            case eFunc_GetFullModeToSwitchAudio:
                {
                    pCommVar->SendPrintf("@Get audio switch by input when full mode is select\r\n");
                    pCommVar->SendPrintf("##GET_FULL_SWAUD\r\n");
                }
                break;
            #endif
            case eFunc_GetInputResolution:
                {
                    pCommVar->SendPrintf("@Get the input resolution\r\n");
                    pCommVar->SendPrintf("#GET_INPUT_RES PARAM\r\n");
                    pCommVar->SendPrintf("PARAM = 1 ~ 4\r\n");
                    pCommVar->SendPrintf("1 - HDMI 1\r\n");
                    pCommVar->SendPrintf("2 - HDMI 2\r\n");
                    pCommVar->SendPrintf("3 - HDMI 3\r\n");
                    pCommVar->SendPrintf("4 - HDMI 4\r\n");
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
    pCommVar->SendPrintf("%s",cmd_list[cmdnum].feedbackstring);
    pCommVar->SendPrintf("%s %s\r\n",__DATE__,__TIME__);
    #if 0
    mapp_Ite6615SetSignalInputChange(TRUE);
    #endif
    return emCmdSucess;
}

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
//eFunc_SetOffMsgCounter,
static emCmdFuncStaType mapp_CmdfuncSetUserUartMsgCounter(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off];

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    if(l_u8CmdIndex>=1&&l_u8CmdIndex<=2)
    {
        pstUartKey->u8UartValueSendMessageNum = l_u8CmdIndex;
        SetUserData_UartKeyVal(uart_study_key_type_dis_off);
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);
    }
    else
        l_emRtState = emCmdOutOfRange;
    return l_emRtState;
}

//eFunc_GetOffMsgCounter,
static emCmdFuncStaType mapp_CmdfuncGetOffMsgCounter(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off];

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstUartKey->u8UartValueSendMessageNum);
    return l_emRtState;
}

//eFunc_SetOffMsgCntDelayTime
static emCmdFuncStaType mapp_CmdfuncSetOffMsgCntDelayTime(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0,i=0;
    u16 l_u16Delaytime=0;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off];

    l_u8CmdIndex = strlen(cmd_list[cmdnum].ucmd)+1;
    for(i=0;i<(pCommVar->u16RecCnt-l_u8CmdIndex-cmd_list[cmdnum].cmdoffset+1);i++)
    {
        if(i)
            l_u16Delaytime = l_u16Delaytime*10+(pCommVar->pRecBuffer[l_u8CmdIndex+i]-0x30);
        else
            l_u16Delaytime = (pCommVar->pRecBuffer[l_u8CmdIndex+i]-0x30);
    }

    if(l_u16Delaytime>=5&&l_u16Delaytime<=100)
    {
        pstUartKey->u16UartValueCounterDelayTime = l_u16Delaytime*100;
        SetUserData_UartKeyVal(uart_study_key_type_dis_off);
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u16Delaytime);
    }
    else
        l_emRtState = emCmdOutOfRange;

    return l_emRtState;
}

//eFunc_GetOffMsgCntDelayTime
static emCmdFuncStaType mapp_CmdfuncGetOffMsgCntDelayTime(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off];

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstUartKey->u16UartValueCounterDelayTime/100);
    return l_emRtState;
}

#endif

#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
//eFunc_SetAutoSyncDetectActionByCec,
static emCmdFuncStaType mapp_CmdfuncSetAutoSyncDetectActionByCec(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            pstBasicSpecVar->bActionToSendCecFlag = FALSE;
            break;
        case 1:
            pstBasicSpecVar->bActionToSendCecFlag = TRUE;
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }

    if(l_emRtState == emCmdSucess)
    {
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);
    }
    return l_emRtState;
}

//eFunc_GetAutoSyncDetectActionByCec,
static emCmdFuncStaType mapp_CmdfuncGetAutoSyncDetectActionByCec(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->bActionToSendCecFlag);
    return l_emRtState;
}

//eFunc_SetAutoSyncDetectActionByRs232,
static emCmdFuncStaType mapp_CmdfuncSetAutoSyncDetectActionByRs232(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            pstBasicSpecVar->bActionToSendRs232CmdFlag = FALSE;
            break;
        case 1:
            pstBasicSpecVar->bActionToSendRs232CmdFlag = TRUE;
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }

    if(l_emRtState == emCmdSucess)
    {
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);
    }
    return l_emRtState;
}

//eFunc_GetAutoSyncDetectActionByRs232,
static emCmdFuncStaType mapp_CmdfuncGetAutoSyncDetectActionByRs232(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->bActionToSendRs232CmdFlag);
    return l_emRtState;
}

//eFunc_SetAutoSyncDisplayOffDelayTime,
static emCmdFuncStaType mapp_CmdfuncSetAutoSyncDisplayOffDelayTime(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
    u16 l_u16Minute = 0;
    u16 l_u16Second = 0;
    u16 l_u16TempVal = 0;
    u8 l_u8DataIndex = 0;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    for(l_u8CmdIndex = strlen(cmd_list[cmdnum].ucmd)+1; l_u8CmdIndex < pCommVar->u16RecCnt-2; l_u8CmdIndex++)
    {
        if((pCommVar->pRecBuffer[l_u8CmdIndex]!=':')&&(l_u8DataIndex==0)) //分钟
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8CmdIndex],1))  //是数字
            {
                l_u16Minute = 10*l_u16Minute+(pCommVar->pRecBuffer[l_u8CmdIndex]-0x30);
            }
            else
            {
                return emCmdError;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8CmdIndex]!=':')&&(l_u8DataIndex==1)) //秒钟
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8CmdIndex],1))
            {
                l_u16Second = 10*l_u16Second+(pCommVar->pRecBuffer[l_u8CmdIndex]-0x30);
            }
            #if 0
            else  if((pCommVar->pRecBuffer[l_u8CmdIndex]==0x0d)&&(pCommVar->pRecBuffer[l_u8CmdIndex+1]==0x0a)) //结束符
            {
                break;
            }
            #endif
            else
            {
                return emCmdError;
            }
        }
        else
        {
            l_u8DataIndex++;
        }
    }

    l_u16TempVal = l_u16Minute*60+l_u16Second;
    if(l_u16TempVal<10||l_u16TempVal>1800)
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        pstSpecVar->stBasSpecVar.u16DisplayTime = l_u16TempVal;
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s %d:%d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->u16DisplayTime/60,pstBasicSpecVar->u16DisplayTime%60);
    }
    return l_emRtState;
}

//eFunc_GetAutoSyncDisplayOffDelayTime,
static emCmdFuncStaType mapp_CmdfuncGetAutoSyncDisplayOffDelayTime(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d:%d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->u16DisplayTime/60,pstBasicSpecVar->u16DisplayTime%60);
    return l_emRtState;
}

#if _ENABLE_AUTO_POWER_FUNCTION
//eFunc_SetAutoSyncToPowerOnOff,
static emCmdFuncStaType mapp_CmdfuncSetAutoSyncToPowerOnOff(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            pstBasicSpecVar->bActionToAutoPowerOnOffFlag = FALSE;
            break;
        case 1:
            pstBasicSpecVar->bActionToAutoPowerOnOffFlag = TRUE;
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }

    if(l_emRtState == emCmdSucess)
    {
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);
    }
    return l_emRtState;
}

//eFunc_GetAutoSyncToPowerOnOff,
static emCmdFuncStaType mapp_CmdfuncGetAutoSyncToPowerOnOff(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->bActionToAutoPowerOnOffFlag);
    return l_emRtState;
}
#endif //#if _ENABLE_AUTO_POWER_FUNCTION
#endif //#if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD

#if _ENABLE_KEYPAD_LOCKED_FUNCTION
//eFunc_SetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncSetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            pstBasicSpecVar->bIsLock = FALSE;
            break;
        case 1:
            pstBasicSpecVar->bIsLock = TRUE;
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }

    if(l_emRtState == emCmdSucess)
    {
        SetUserData_SpecBasicVar();
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);
    }
    return l_emRtState;
}

//eFunc_GetKeyPadLocked,
static emCmdFuncStaType mapp_CmdfuncGetKeyPadLocked(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t     pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t       pstBasicSpecVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstBasicSpecVar->bIsLock);
    return l_emRtState;
}

#endif //#if _ENABLE_KEYPAD_LOCKED_FUNCTION

#if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
//eFunc_SetInputSignalDetectMode,
static emCmdFuncStaType mapp_CmdfuncSetInputSignalDetectMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8CmdIndex=0;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstInputSignalDetectMode_t  pstInSignDetode = &pstPrjSaveVar->stInputSignalDetectMode;

    l_u8CmdIndex = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8CmdIndex)
    {
        case 0:
            pstInSignDetode->m_u8DetectMode = emTMDS;
            break;
        case 1:
            pstInSignDetode->m_u8DetectMode = emHPD;
            break;
        default:
            l_emRtState = emCmdOutOfRange;
            break;
    }

    if(l_emRtState == emCmdSucess)
    {
        g_emDetectMode = (emSwitchMode)pstInSignDetode->m_u8DetectMode;
        SetUserData_InSignalDetMode();
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, l_u8CmdIndex);
    }
    return l_emRtState;
}

//eFunc_GetInputSignalDetectMode,
static emCmdFuncStaType mapp_CmdfuncGetInputSignalDetectMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstInputSignalDetectMode_t  pstInSignDetode = &pstPrjSaveVar->stInputSignalDetectMode;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring, pstInSignDetode->m_u8DetectMode);
    return l_emRtState;
}
#endif // #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)

#if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
//eFunc_LoadIteChipRegisterData
static emCmdFuncStaType mapp_CmdfuncLoadIteChipRegisterData(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8TempValue;

    l_u8TempValue = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;
    switch(l_u8TempValue)
    {
        case 0:
            g_bLoadIteChipRegisterFlag = FALSE;
            pCommVar->SendString("@DISABLE LOAD ITE CHIP REGISTER DATA\r\n", TRUE);
            break;
        case 1:
            g_bLoadIteChipRegisterFlag = TRUE;
            pCommVar->SendString("@ENABLE LOAD ITE CHIP REGISTER DATA\r\n", TRUE);
            break;
        default:
            return emCmdOutOfRange;
    }
    return emCmdSucess;
}

// WREG <BA,001,10>
static emCmdFuncStaType mapp_CmdfuncWriteRegisterData(pstUartCom_t pCommVar,u8 cmdnum)
{
    #if 1
    u8 l_u8TempAddr = 0;
    u16 l_u16TempOffset = 0;
    u8 l_u8TempData = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd); l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==0))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8TempAddr = (l_u8TempAddr<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==1))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u16TempOffset = (l_u16TempOffset<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==2))
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
        if((pCommVar->pRecBuffer[l_u8Index]=='>'))
        {
            break;
        }
    }

    mapp_Ite66341RegWriteByte(0,l_u8TempAddr,l_u16TempOffset,l_u8TempData);
    pCommVar->SendPrintf("Write ADD[0x%02x] REG[0x%04x] DATA[0x%02x]\r\n", l_u8TempAddr,l_u16TempOffset,l_u8TempData);
    #endif
    return emCmdSucess;
}

static emCmdFuncStaType mapp_CmdfuncReadRegisterData(pstUartCom_t pCommVar,u8 cmdnum)
{
    #if 1
    u8 l_u8TempAddr = 0;
    u16 l_u16TempOffset = 0;
    u8 l_u8TempData = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;

    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd); l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==0))
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='0'&&(pCommVar->pRecBuffer[l_u8Index+1]=='x'||(pCommVar->pRecBuffer[l_u8Index+1]=='X')))
                l_u8Index += 2;
            if(HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0)==pCommVar->pRecBuffer[l_u8Index])
                return emCmdError;
            l_u8TempAddr = (l_u8TempAddr<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')&&(pCommVar->pRecBuffer[l_u8Index]!='>')&&(l_u8DataIndex==1))
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

        if((pCommVar->pRecBuffer[l_u8Index]=='>'))
        {
            break;
        }
    }

    mapp_Ite66341RegReadByte(0,l_u8TempAddr,l_u16TempOffset,&l_u8TempData);
    pCommVar->SendPrintf("Read ADD[0x%02x] REG[0x%04x] DATA[0x%02x]\r\n", l_u8TempAddr,l_u16TempOffset,l_u8TempData);
    #endif
    return emCmdSucess;
}
#endif // #if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER

//eFunc_SetResolution,
static emCmdFuncStaType mapp_CmdfuncSetResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8ResIndex;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==2)
    {
        l_u8ResIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30)*10
                        +(pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+2]-0x30);
    }
    else if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8ResIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8ResIndex>(max_Resolution)||l_u8ResIndex==0)
    {
        return emCmdOutOfRange;
    }

    pstBasicVar->u8ResolutionID = l_u8ResIndex-1;

    if(pstBasicVar->u8ResolutionID<max_Resolution)
    {
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8ResIndex);
    }
    else
        return emCmdOutOfRange;
    mapp_SCU41MVResolutionSwitch(pstBasicVar->u8ResolutionID);
    SetUserData_SpecBasicVar();

    return emCmdSucess;
}

//eFunc_GetResolution,
static emCmdFuncStaType mapp_CmdfuncGetResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->u8ResolutionID<max_Resolution)
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,pstBasicVar->u8ResolutionID+1);
    else
        return emCmdOutOfRange;
    return emCmdSucess;
}

//eFunc_SetMultiviewMode,
static emCmdFuncStaType mapp_CmdfuncSetMultiviewMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8ModeIndex;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==2)
    {
        l_u8ModeIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30)*10
                        +(pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+2]-0x30);
    }
    else if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8ModeIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8ModeIndex>emMvMode_max||l_u8ModeIndex==0)
    {
        return emCmdOutOfRange;
    }

    pstBasicVar->u8MvMode = l_u8ModeIndex-1;
    SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
    SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
    SCU41_MV_SetMultiViewMode(pstBasicVar->u8MvMode);
    SetUserData_SpecBasicVar();
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8ModeIndex);

    return emCmdSucess;
}

//eFunc_GetMultiviewMode,
static emCmdFuncStaType mapp_CmdfuncGetMultiviewMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pstBasicVar->u8MvMode<emMvMode_max)
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,pstBasicVar->u8MvMode+1);
    else
        return emCmdOutOfRange;
    return emCmdSucess;
}


//eFunc_SetAudioSource,
static emCmdFuncStaType mapp_CmdfuncSetAudioSource(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8AudioSrcIndex;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==2)
    {
        l_u8AudioSrcIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30)*10
                        +(pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+2]-0x30);
    }
    else if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8AudioSrcIndex = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8AudioSrcIndex>5||l_u8AudioSrcIndex==0)
    {
        return emCmdOutOfRange;
    }

    pstBasicVar->u8AudioSel = l_u8AudioSrcIndex-1;
    mapp_SCU41MVSetAudioInput(pstBasicVar->u8AudioSel);
    SetUserData_SpecBasicVar();

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8AudioSrcIndex);

    return emCmdSucess;
}

//eFunc_GetAudioSource,
static emCmdFuncStaType mapp_CmdfuncGetAudioSource(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,pstBasicVar->u8AudioSel+1);
    return emCmdSucess;
}

//eFunc_SetAudioMix,
static emCmdFuncStaType mapp_CmdfuncSetAudioMix(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>1)
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal == 0)
        pstBasicVar->bAudioMix = FALSE;
    else
        pstBasicVar->bAudioMix = TRUE;
    mapp_SCU41MVSetAudioMix(pstBasicVar->bAudioMix);
    SetUserData_SpecBasicVar();

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_GetAudioMix,
static emCmdFuncStaType mapp_CmdfuncGetAudioMix(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,pstBasicVar->bAudioMix);
    return emCmdSucess;
}

//eFunc_SetAudioMute,
static emCmdFuncStaType mapp_CmdfuncSetAudioMute(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;
    //pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    //pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>1)
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal == 0)
    {
        //pstBasicVar->bAudioMute = FALSE;
        mapp_SCU41MVSetAudioMute(FALSE);
    }
    else
    {
        //pstBasicVar->bAudioMute = TRUE;
        mapp_SCU41MVSetAudioMute(TRUE);
    }
    SetUserData_SpecBasicVar();

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_GetAudioMute,
static emCmdFuncStaType mapp_CmdfuncGetAudioMute(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,pstBasicVar->bAudioMute);
    return emCmdSucess;
}

//eFunc_SetCecSrcMenu,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcMenu(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_SETUP_MENU,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcUp,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcUp(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_UP,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcDown,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcDown(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_DOWN,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcLeft,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcLeft(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_LEFT,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcRight,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcRight(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_RIGHT,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcBack,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcBack(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_EXIT,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcEnter,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcEnter(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_SELECT,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcOn,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcOn(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_POWER_ON_FUNCTION,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcOff,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcOff(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_POWER_OFF_FUNCTION,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcStop,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcStop(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_STOP,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcPlay,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcPlay(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_PLAY,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcPause,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcPause(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_PAUSE,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcPrev,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcPrev(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_BACKWARD,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcNext,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcNext(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_FORWARD,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcRewind,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcRewind(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_REWIND,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecSrcFastForward,
static emCmdFuncStaType mapp_CmdfuncSetCecSrcFastForward(pstUartCom_t pCommVar,u8 cmdnum)
{
    uint8_t l_u8TempVal;

    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }

    cec_cmd_send_to_src(l_u8TempVal-1,CEC_RC_FAST_FORWARD,FALSE);
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);

    return emCmdSucess;
}

//eFunc_SetCecDisOn,
static emCmdFuncStaType mapp_CmdfuncSetCecDisOn(pstUartCom_t pCommVar,u8 cmdnum)
{
    cec_cmd_tv_poweron(4,FALSE);
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_SetCecDisOff,
static emCmdFuncStaType mapp_CmdfuncSetCecDisOff(pstUartCom_t pCommVar,u8 cmdnum)
{
    cec_cmd_tv_poweroff(4,FALSE);
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_SetCecDisSource,
static emCmdFuncStaType mapp_CmdfuncSetCecDisSource(pstUartCom_t pCommVar,u8 cmdnum)
{
    //cec_cmd_send_to_src(4,CEC_RC_INPUT_SELECT,FALSE);
    cec_cmd_tv_source(4,FALSE);
    delay_ms(1000);
    cec_cmd_tv_source(4,FALSE);
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_SetCecDisMute,
static emCmdFuncStaType mapp_CmdfuncSetCecDisMute(pstUartCom_t pCommVar,u8 cmdnum)
{
    cec_cmd_tv_volmute(4,FALSE);
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_SetCecDisVolPlus,
static emCmdFuncStaType mapp_CmdfuncSetCecDisVolPlus(pstUartCom_t pCommVar,u8 cmdnum)
{
    cec_cmd_tv_volup(4,FALSE);
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_SetCecDisVolMinus,
static emCmdFuncStaType mapp_CmdfuncSetCecDisVolMinus(pstUartCom_t pCommVar,u8 cmdnum)
{
    cec_cmd_tv_voldown(4,FALSE);
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_EnableMvUserConfig,
static emCmdFuncStaType mapp_CmdfuncEnableMvUserConfig(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8TempVal=0;
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    l_u8TempVal = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;

    if(l_u8TempVal>=1&&l_u8TempVal<=4)
    {
        g_u8UserMvModeIndex = l_u8TempVal-1;
    }
    else
        l_emRtState = emCmdOutOfRange;

    if(l_emRtState == emCmdSucess)
    {
        pCommVar->SendPrintf("@Set multiview mode config by user %d\r\n",g_u8UserMvModeIndex+1);
        //pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,g_u8UserMvModeIndex+1);
    }
    return l_emRtState;
}

//eFunc_ConfirmMvUserSetting
static emCmdFuncStaType mapp_CmdfuncConfirmMvUserSetting(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t		    pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    switch(pstBasicVar->u8MvMode)
    {
        case emMvMode_xWin_ByUser1:
        case emMvMode_xWin_ByUser2:
        case emMvMode_xWin_ByUser3:
        case emMvMode_xWin_ByUser4:
            if((pstBasicVar->u8MvMode-emMvMode_xWin_ByUser1) == g_u8UserMvModeIndex)
            {
                SCU41_MV_UpdateWindowNum(pstBasicVar->u8MvMode);
                //SCU41_MV_SetWinLedMode(emWinLightMode_On,SCU41_MV_GetWinPortSelect());
                //SCU41_MV_SetMultiViewMode(pstBasicVar->u8MvMode);
                mapp_SCU41MVMultiWinUpdate((u8)emSrcInputMax,emWinTypeMax,TRUE);
            }
        break;
    }
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);
    return l_emRtState;
}

//eFunc_SetSizeAndPositionWinA,
static emCmdFuncStaType mapp_CmdfuncSetSizeAndPositionWinA(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==0)
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==1)
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==2)
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==3)
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==4)
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if((l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(g_u8UserMvModeIndex+1,
                                    emWinType_A,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s %d <%d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_SetSizeAndPositionWinB,
static emCmdFuncStaType mapp_CmdfuncSetSizeAndPositionWinB(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==0)
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==1)
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==2)
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==3)
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==4)
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if((l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(g_u8UserMvModeIndex+1,
                                    emWinType_B,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s %d <%d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_SetSizeAndPositionWinC,
static emCmdFuncStaType mapp_CmdfuncSetSizeAndPositionWinC(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==0)
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==1)
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==2)
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==3)
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==4)
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if((l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(g_u8UserMvModeIndex+1,
                                    emWinType_C,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s %d <%d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_SetSizeAndPositionWinD,
static emCmdFuncStaType mapp_CmdfuncSetSizeAndPositionWinD(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==0)
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==1)
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==2)
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==3)
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==4)
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if((l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(g_u8UserMvModeIndex+1,
                                    emWinType_D,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s %d <%d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_CfgSizeAndPositionWinA,
static emCmdFuncStaType mapp_CmdfuncCfgSizeAndPositionWinA(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#CFG_WINA <1 1 xx1,yy1 xx2,yy2>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(l_u8MvUser,
                                    emWinType_A,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_CfgSizeAndPositionWinB,
static emCmdFuncStaType mapp_CmdfuncCfgSizeAndPositionWinB(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#CFG_WINB <1 1 xx1,yy1 xx2,yy2>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(l_u8MvUser,
                                    emWinType_B,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_CfgSizeAndPositionWinC,
static emCmdFuncStaType mapp_CmdfuncCfgSizeAndPositionWinC(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#CFG_WINC <1 1 xx1,yy1 xx2,yy2>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(l_u8MvUser,
                                    emWinType_C,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_CfgSizeAndPositionWinD,
static emCmdFuncStaType mapp_CmdfuncCfgSizeAndPositionWinD(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u8 l_u8StartX = 0;
    u8 l_u8StartY = 0;
    u8 l_u8EndX = 0;
    u8 l_u8EndY = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#CFG_WIND <1 1 xx1,yy1 xx2,yy2>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartX = l_u8StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8StartY = l_u8StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndX = l_u8EndX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8EndY = l_u8EndY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||(l_u8StartX>100)||(l_u8StartY>100)||(l_u8EndX>100)||(l_u8EndY>100))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeWinSize(l_u8MvUser,
                                    emWinType_D,
                                    l_u8Src,
                                    l_u8StartX,
                                    l_u8StartY,
                                    l_u8EndX,
                                    l_u8EndY);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u8StartX,
                            l_u8StartY,
                            l_u8EndX,
                            l_u8EndY);
    }
    return l_emRtState;
}

//eFunc_PixelCfgSizeAndPositionWinA,
static emCmdFuncStaType mapp_CmdfuncPixelCfgSizeAndPositionWinA(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u16 l_u16StartX = 0;
    u16 l_u16StartY = 0;
    u16 l_u16Width = 0;
    u16 l_u16Height = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#PCFG_WINA <1 1 x,y w,h>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartX = l_u16StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartY = l_u16StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Width = l_u16Width*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Height = l_u16Height*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeCfgPixelWinSize(l_u8MvUser,
                                    emWinType_A,
                                    l_u8Src,
                                    l_u16StartX,
                                    l_u16StartY,
                                    l_u16Width,
                                    l_u16Height);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u16StartX,
                            l_u16StartY,
                            l_u16Width,
                            l_u16Height);
    }
    return l_emRtState;
}

//eFunc_PixelCfgSizeAndPositionWinB,
static emCmdFuncStaType mapp_CmdfuncPixelCfgSizeAndPositionWinB(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u16 l_u16StartX = 0;
    u16 l_u16StartY = 0;
    u16 l_u16Width = 0;
    u16 l_u16Height = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#PCFG_WINB <1 1 x,y w,h>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartX = l_u16StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartY = l_u16StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Width = l_u16Width*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Height = l_u16Height*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeCfgPixelWinSize(l_u8MvUser,
                                    emWinType_B,
                                    l_u8Src,
                                    l_u16StartX,
                                    l_u16StartY,
                                    l_u16Width,
                                    l_u16Height);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u16StartX,
                            l_u16StartY,
                            l_u16Width,
                            l_u16Height);
    }
    return l_emRtState;
}

//eFunc_PixelCfgSizeAndPositionWinC,
static emCmdFuncStaType mapp_CmdfuncPixelCfgSizeAndPositionWinC(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u16 l_u16StartX = 0;
    u16 l_u16StartY = 0;
    u16 l_u16Width = 0;
    u16 l_u16Height = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#PCFG_WINC <1 1 x,y w,h>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartX = l_u16StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartY = l_u16StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Width = l_u16Width*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Height = l_u16Height*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeCfgPixelWinSize(l_u8MvUser,
                                    emWinType_C,
                                    l_u8Src,
                                    l_u16StartX,
                                    l_u16StartY,
                                    l_u16Width,
                                    l_u16Height);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u16StartX,
                            l_u16StartY,
                            l_u16Width,
                            l_u16Height);
    }
    return l_emRtState;
}

//eFunc_PixelCfgSizeAndPositionWinD,
static emCmdFuncStaType mapp_CmdfuncPixelCfgSizeAndPositionWinD(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8MvUser = 0;
    u8 l_u8Src = 0;
    u16 l_u16StartX = 0;
    u16 l_u16StartY = 0;
    u16 l_u16Width = 0;
    u16 l_u16Height = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    //#PCFG_WIND <1 1 x,y w,h>
    for(l_u8Index = l_u8HeadIndex; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8MvUser = (l_u8MvUser<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u8Src = (l_u8Src<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartX = l_u16StartX*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16StartY = l_u16StartY*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Width = l_u16Width*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==5))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_u16Height = l_u16Height*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if(((l_u8MvUser>4))||(l_u8Src>4)||mapp_SCU41MVPixelOutOfRange(l_u16StartX,l_u16StartY,l_u16Width,l_u16Height))
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        mapp_SCU41MVChangeCfgPixelWinSize(l_u8MvUser,
                                    emWinType_D,
                                    l_u8Src,
                                    l_u16StartX,
                                    l_u16StartY,
                                    l_u16Width,
                                    l_u16Height);
        pCommVar->SendPrintf("%s <%d %d %d,%d %d,%d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8MvUser,
                            l_u8Src,
                            l_u16StartX,
                            l_u16StartY,
                            l_u16Width,
                            l_u16Height);
    }
    return l_emRtState;
}

//eFunc_SaveScreenToUserMode
static emCmdFuncStaType mapp_CmdfuncSaveScreenToUserMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8TempVal=0;
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    l_u8TempVal = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        u8 l_u8Win;
        for(l_u8Win=0;l_u8Win<emWinTypeMax;l_u8Win++)
        {
            mapp_SCU41MVChangeWinSize(  l_u8TempVal,
                                        l_u8Win,
                                        g_stUserWinsConfig.u8UserSrc[l_u8Win]+1,
                                        g_stUserWinsConfig.u8StartX[l_u8Win],
                                        g_stUserWinsConfig.u8StartY[l_u8Win],
                                        g_stUserWinsConfig.u8EndX[l_u8Win],
                                        g_stUserWinsConfig.u8EndY[l_u8Win]);
        }
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);
    }
    return l_emRtState;
}

//eFunc_SavePixelToUserMode
static emCmdFuncStaType mapp_CmdfuncSavePixelToUserMode(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8TempVal=0;
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    l_u8TempVal = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;

    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        u8 l_u8Win;
        for(l_u8Win=0;l_u8Win<emWinTypeMax;l_u8Win++)
        {
            mapp_SCU41MVChangeCfgPixelWinSize(  l_u8TempVal,
                                        l_u8Win,
                                        g_stUserWinPixelCfg.u8UserSrc[l_u8Win]+1,
                                        g_stUserWinPixelCfg.u16StartX[l_u8Win],
                                        g_stUserWinPixelCfg.u16StartY[l_u8Win],
                                        g_stUserWinPixelCfg.u16Width[l_u8Win],
                                        g_stUserWinPixelCfg.u16Heigh[l_u8Win]);
        }
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);
    }
    return l_emRtState;
}

//eFunc_CleanScreen
static emCmdFuncStaType mapp_CmdfuncCleanScreen(pstUartCom_t pCommVar,u8 cmdnum)
{
    u8 l_u8TempVal=0;
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    l_u8TempVal = pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30;

    if(l_u8TempVal>4)
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        //u8 l_u8Win;
        if(l_u8TempVal==0)
            mapp_SCU41MVCleanWin(emWinTypeMax);
        else
            mapp_SCU41MVCleanWin(l_u8TempVal-1);
        pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);
    }
    return l_emRtState;
}

//eFunc_GetIpAddress,
static emCmdFuncStaType mapp_CmdfuncGetIpAddress(pstUartCom_t pCommVar,u8 cmdnum)
{
    pCommVar->SendPrintf("@IP_ADDR: %d.%d.%d.%d\r\n",g_u8IpAddr[0],g_u8IpAddr[1],g_u8IpAddr[2],g_u8IpAddr[3]);
    pCommVar->SendPrintf("@SUBNET_MASK: %d.%d.%d.%d\r\n",g_u8NetMask[0],g_u8NetMask[1],g_u8NetMask[2],g_u8NetMask[3]);
    pCommVar->SendPrintf("@GATEWAY: %d.%d.%d.%d\r\n",g_u8NetGate[0],g_u8NetGate[1],g_u8NetGate[2],g_u8NetGate[3]);
    //CommResMsgSnd(("@IP_ADDR: %d.%d.%d.%d\r\n",g_u8IpAddr[0],g_u8IpAddr[1],g_u8IpAddr[2],g_u8IpAddr[3]));
    //CommResMsgSnd(("@SUBNET_MASK: %d.%d.%d.%d\r\n",g_u8NetMask[0],g_u8NetMask[1],g_u8NetMask[2],g_u8NetMask[3]));
    //CommResMsgSnd(("@GATEWAY: %d.%d.%d.%d\r\n",g_u8NetGate[0],g_u8NetGate[1],g_u8NetGate[2],g_u8NetGate[3]));

    return emCmdSucess;
}

//eFunc_SetIpAddress,
static emCmdFuncStaType mapp_CmdfuncSetIpAddress(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_pu8Ip[4] = {0,0,0,0};
    u8 l_pu8Mask[4] = {0,0,0,0};
    u8 l_pu8Gate[4] = {0,0,0,0};
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8ModeIndex = 0;

    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd)+1; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==0))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[0] = l_pu8Ip[0]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[0] = l_pu8Mask[0]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[0] = l_pu8Gate[0]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==1))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[1] = l_pu8Ip[1]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[1] = l_pu8Mask[1]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[1] = l_pu8Gate[1]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==2))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[2] = l_pu8Ip[2]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[2] = l_pu8Mask[2]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[2] = l_pu8Gate[2]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex>=3))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[3] = l_pu8Ip[3]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[3] = l_pu8Mask[3]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[3] = l_pu8Gate[3]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]=='<')||(pCommVar->pRecBuffer[l_u8Index]==' '))
        {
            continue;
        }
        else
        {
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            l_u8ModeIndex++;
            if(l_u8ModeIndex<3)
            {
                l_u8DataIndex = 0;
            }
            else
                break;
        }
    }

    for(l_u8Index=0;l_u8Index<4;l_u8Index++)
    {
        if((l_pu8Ip[l_u8Index]>255)||(l_pu8Mask[l_u8Index]>255)||(l_pu8Gate[l_u8Index]>255))
        {
            l_emRtState = emCmdOutOfRange;
            break;
        }
    }

    if(l_emRtState == emCmdSucess)
    {
        memcpy(g_u8IpAddr,l_pu8Ip,4);
        memcpy(g_u8NetMask,l_pu8Mask,4);
        memcpy(g_u8NetGate,l_pu8Gate,4);
        pCommVar->SendPrintf("%s <%d.%d.%d.%d> <%d.%d.%d.%d> <%d.%d.%d.%d>\r\n",cmd_list[cmdnum].feedbackstring,
                                        g_u8IpAddr[0],g_u8IpAddr[1],g_u8IpAddr[2],g_u8IpAddr[3],
                                        g_u8NetMask[0],g_u8NetMask[1],g_u8NetMask[2],g_u8NetMask[3],
                                        g_u8NetGate[0],g_u8NetGate[1],g_u8NetGate[2],g_u8NetGate[3]);
    }
    return l_emRtState;
}

//eFunc_GiveIpAddress,
static emCmdFuncStaType mapp_CmdfuncGiveIpAddress(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_pu8Ip[4] = {0,0,0,0};
    u8 l_pu8Mask[4] = {0,0,0,0};
    u8 l_pu8Gate[4] = {0,0,0,0};
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8ModeIndex = 0;

    for(l_u8Index = strlen(cmd_list[cmdnum].ucmd)+1; l_u8Index<pCommVar->u16RecCnt; l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==0))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[0] = l_pu8Ip[0]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[0] = l_pu8Mask[0]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[0] = l_pu8Gate[0]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==1))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[1] = l_pu8Ip[1]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[1] = l_pu8Mask[1]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[1] = l_pu8Gate[1]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex==2))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[2] = l_pu8Ip[2]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[2] = l_pu8Mask[2]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[2] = l_pu8Gate[2]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!='.')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(l_u8DataIndex>=3))
        {
            if(!isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
            {
                l_emRtState = emCmdError;
                break;
            }
            if(l_u8ModeIndex == 0)
            {
                l_pu8Ip[3] = l_pu8Ip[3]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 1)
            {
                l_pu8Mask[3] = l_pu8Mask[3]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else if(l_u8ModeIndex == 2)
            {
                l_pu8Gate[3] = l_pu8Gate[3]*10+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]=='<')||(pCommVar->pRecBuffer[l_u8Index]==' '))
        {
            continue;
        }
        else
        {
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            l_u8ModeIndex++;
            if(l_u8ModeIndex<3)
            {
                l_u8DataIndex = 0;
            }
            else
                break;
        }
    }

    for(l_u8Index=0;l_u8Index<4;l_u8Index++)
    {
        if((l_pu8Ip[l_u8Index]>255)||(l_pu8Mask[l_u8Index]>255)||(l_pu8Gate[l_u8Index]>255))
        {
            l_emRtState = emCmdOutOfRange;
            break;
        }
    }

    if(l_emRtState == emCmdSucess)
    {
        if((l_pu8Ip[0]!=l_pu8Gate[0])||(l_pu8Ip[1]!=l_pu8Gate[1])||(l_pu8Ip[2]!=l_pu8Gate[2]))
        {
            pCommVar->SendPrintf("The IP or GATEWAY is error!!\r\n");
        }
        else
        {
            memcpy(g_u8IpAddr,l_pu8Ip,4);
            memcpy(g_u8NetMask,l_pu8Mask,4);
            memcpy(g_u8NetGate,l_pu8Gate,4);
            pCommVar->SendPrintf("@IP_ADDR: %d.%d.%d.%d\r\n",g_u8IpAddr[0],g_u8IpAddr[1],g_u8IpAddr[2],g_u8IpAddr[3]);
            pCommVar->SendPrintf("@SUBNET_MASK: %d.%d.%d.%d\r\n",g_u8NetMask[0],g_u8NetMask[1],g_u8NetMask[2],g_u8NetMask[3]);
            pCommVar->SendPrintf("@GATEWAY: %d.%d.%d.%d\r\n",g_u8NetGate[0],g_u8NetGate[1],g_u8NetGate[2],g_u8NetGate[3]);
            //CommResMsgSnd(("@IP_ADDR: %d.%d.%d.%d\r\n",g_u8IpAddr[0],g_u8IpAddr[1],g_u8IpAddr[2],g_u8IpAddr[3]));
            //CommResMsgSnd(("@SUBNET_MASK: %d.%d.%d.%d\r\n",g_u8NetMask[0],g_u8NetMask[1],g_u8NetMask[2],g_u8NetMask[3]));
            //CommResMsgSnd(("@GATEWAY: %d.%d.%d.%d\r\n",g_u8NetGate[0],g_u8NetGate[1],g_u8NetGate[2],g_u8NetGate[3]));
        }
    }
    return l_emRtState;
}

//eFunc_SetSwapSource,
static emCmdFuncStaType mapp_CmdfuncSetSwapSource(pstUartCom_t pCommVar,u8 cmdnum)
{
    g_emSourceKeyType = eKeyValSource_Uart;
    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);
    pITE663xx_STM32_api->SysKeyIn(emKeyFunc_Swap);

    return emCmdSucess;
}

//eFunc_SetResizeWin,
static emCmdFuncStaType mapp_CmdfuncSetResizeWin(pstUartCom_t pCommVar,u8 cmdnum)
{
    g_emSourceKeyType = eKeyValSource_Uart;
    pITE663xx_STM32_api->SysKeyIn(emKeyFunc_Resize);
    //pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);

    return emCmdSucess;
}

//eFunc_GetAllSystemSta
static emCmdFuncStaType mapp_CmdfuncGetAllSystemSta(pstUartCom_t pCommVar,u8 cmdnum)
{
    pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
    pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8BaudrateID=0;
    u8 l_u8HdcpIndex=0;

    if(pstPrjSaveVar->stPrjNameSet.m_u8Len)
    {
        RS232_printf(pstPrjSaveVar->stPrjNameSet.sPrjName);
        RS232_printf("\r\n");
        Uart1_printf(pstPrjSaveVar->stPrjNameSet.sPrjName);
        Uart1_printf("\r\n");
    }
    pCommVar->SendPrintf("@%s",_STRING_FW_VER_NUM);
    pCommVar->SendPrintf("@VIDEO\r\n");
    pCommVar->SendPrintf("OUT A B C D\r\n");
    pCommVar->SendPrintf("IN  %d %d %d %d\r\n",pstBasicVar->u8WinSel[emWinType_A]+1,
                        pstBasicVar->u8WinSel[emWinType_B]+1,
                        pstBasicVar->u8WinSel[emWinType_C]+1,
                        pstBasicVar->u8WinSel[emWinType_D]+1);
    pCommVar->SendPrintf("@AUDIO_SRC %d\r\n",pstBasicVar->u8AudioSel+1);
    pCommVar->SendPrintf("%s %d\r\n","@OUTPUT_RES",pstBasicVar->u8ResolutionID+1);
    pCommVar->SendPrintf("%s %d\r\n","@AUTO_SWITCH", pstPrjSaveVar->stHdmiSel.bAutoSwitch);
    pCommVar->SendPrintf("%s %d %d\r\n","@EDID_MODE",1,mapp_SCU41MVGetEdidIndex(0));
    pCommVar->SendPrintf("%s %d %d\r\n","@EDID_MODE",2,mapp_SCU41MVGetEdidIndex(1));
    pCommVar->SendPrintf("%s %d %d\r\n","@EDID_MODE",3,mapp_SCU41MVGetEdidIndex(2));
    pCommVar->SendPrintf("%s %d %d\r\n","@EDID_MODE",4,mapp_SCU41MVGetEdidIndex(3));
    pCommVar->SendPrintf("%s %d\r\n","@KEYPAD_LOCK", pstBasicVar->bIsLock);

    switch(pstBasicVar->u8baudrateId)
    {
        case baud_rate_115200:
            l_u8BaudrateID = 1;
            break;
        case baud_rate_57600:
            l_u8BaudrateID = 2;
            break;
        case baud_rate_38400:
            l_u8BaudrateID = 3;
            break;
        case baud_rate_19200:
            l_u8BaudrateID = 4;
            break;
        case baud_rate_9600:
            l_u8BaudrateID = 5;
            break;
        case baud_rate_4800:
            l_u8BaudrateID = 6;
            break;
        case baud_rate_2400:
            l_u8BaudrateID = 7;
            break;
        default:
            break;
    }
    pCommVar->SendPrintf("%s %d\r\n","@RS232_BAUD", l_u8BaudrateID);
    pCommVar->SendPrintf("%s %d\r\n","@MV_MODE",pstBasicVar->u8MvMode+1);

    if(pstBasicVar->u8HdcpMode == tx_hdcp_force_1_4)
        l_u8HdcpIndex = 1;
    else if(pstBasicVar->u8HdcpMode == tx_hdcp_force_off)
        l_u8HdcpIndex = 3;
    else //if(pstBasicVar->u8HdcpMode == tx_hdcp_force_2_2)
        l_u8HdcpIndex = 2;

    pCommVar->SendPrintf("%s %d\r\n","@OUTPUT_HDCP",l_u8HdcpIndex);

    pCommVar->SendPrintf("%s %d\r\n","@AUDIO_MIX",pstBasicVar->bAudioMix);
    pCommVar->SendPrintf("%s %d\r\n","@AUDIO_MUTE",pstBasicVar->bAudioMute);
    pCommVar->SendPrintf("%s %d\r\n","@FULL_SWAUD",pstBasicVar->bFullModeToSwitchAudio);

    pCommVar->SendPrintf("%s %d\r\n","@SYNCACT_CEC", pstBasicVar->bActionToSendCecFlag);
    pCommVar->SendPrintf("%s %d\r\n","@SYNCACT_RS232", pstBasicVar->bActionToSendRs232CmdFlag);
    pCommVar->SendPrintf("%s %d\r\n","@AUTO_POWER", pstBasicVar->bActionToAutoPowerOnOffFlag);
    pCommVar->SendPrintf("%s %d:%d\r\n","@DTIME", pstBasicVar->u16DisplayTime/60,pstBasicVar->u16DisplayTime%60);

    pCommVar->SendPrintf("@IP_ADDR: %d.%d.%d.%d\r\n",g_u8IpAddr[0],g_u8IpAddr[1],g_u8IpAddr[2],g_u8IpAddr[3]);
    pCommVar->SendPrintf("@SUBNET_MASK: %d.%d.%d.%d\r\n",g_u8NetMask[0],g_u8NetMask[1],g_u8NetMask[2],g_u8NetMask[3]);
    pCommVar->SendPrintf("@GATEWAY: %d.%d.%d.%d\r\n",g_u8NetGate[0],g_u8NetGate[1],g_u8NetGate[2],g_u8NetGate[3]);

    return emCmdSucess;
}

//eFunc_SetWinLayerLevel,
static emCmdFuncStaType mapp_CmdfuncSetWinLayerLevel(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8LvId = 0;
    u8 l_u8Lv1 = 0;
    u8 l_u8Lv2 = 0;
    u8 l_u8Lv3 = 0;
    u8 l_u8Lv4 = 0;
    u8 l_u8Index = 0;
    u8 l_u8DataIndex = 0;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;
    u8 l_pu8Temp[4];

    for(l_u8Index = l_u8HeadIndex; l_u8Index<(pCommVar->u16RecCnt-2); l_u8Index++)
    {
        if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==0))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==0)
            {
                l_u8LvId = (l_u8LvId<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==1)
            {
                l_u8Lv1 = l_u8Lv1*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==2))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==2)
            {
                l_u8Lv2 = l_u8Lv2*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==3))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==3)
            {
                l_u8Lv3 = l_u8Lv3*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else if((pCommVar->pRecBuffer[l_u8Index]!=',')
            &&(pCommVar->pRecBuffer[l_u8Index]!=' ')
            &&(pCommVar->pRecBuffer[l_u8Index]!='<')
            &&(pCommVar->pRecBuffer[l_u8Index]!='>')
            &&(l_u8DataIndex==4))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1)&&l_u8DataIndex==4)
            {
                l_u8Lv4 = l_u8Lv4*10+(pCommVar->pRecBuffer[l_u8Index]-0x30);
            }
            else
            {
                l_emRtState = emCmdError;
                break;
            }
        }
        else
        {
            if(pCommVar->pRecBuffer[l_u8Index]=='<')
                continue;
            l_u8DataIndex++;
        }

        if(pCommVar->pRecBuffer[l_u8Index]=='>')
        {
            break;
        }
    }

    if((l_u8LvId>4||l_u8LvId==0)||(l_u8Lv1>4)||(l_u8Lv2>4)||(l_u8Lv3>4)||(l_u8Lv4>4))
    {
        l_emRtState = emCmdOutOfRange;
    }


    l_pu8Temp[0] = l_u8Lv1;
    l_pu8Temp[1] = l_u8Lv2;
    l_pu8Temp[2] = l_u8Lv3;
    l_pu8Temp[3] = l_u8Lv4;

    if(l_emRtState == emCmdSucess)
    {
        if(mapp_CheckTheSameValue(l_pu8Temp,4)==0)
        {
            mapp_SCU41MVSetLayerLevel(l_u8LvId-1,l_u8Lv1,l_u8Lv2,l_u8Lv3,l_u8Lv4);
        }
        pCommVar->SendPrintf("%s %d <%d %d %d %d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8LvId,
                            l_u8Lv1,
                            l_u8Lv2,
                            l_u8Lv3,
                            l_u8Lv4);
    }
    return l_emRtState;
}

//eFunc_GetWinLayerLevel,
static emCmdFuncStaType mapp_CmdfuncGetWinLayerLevel(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8LvId = 0;
    u8 l_u8Index = 0;

    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    for(l_u8Index = l_u8HeadIndex; l_u8Index<(pCommVar->u16RecCnt-2); l_u8Index++)
    {
        if(isNumberCmd(&pCommVar->pRecBuffer[l_u8Index],1))
        {
            l_u8LvId = (l_u8LvId<<4)+HexAndAsciiChange(pCommVar->pRecBuffer[l_u8Index],0);
        }
        else
        {
            l_emRtState = emCmdError;
            break;
        }
    }

    if(l_u8LvId>4||l_u8LvId==0)
    {
        l_emRtState = emCmdOutOfRange;
    }

    if(l_emRtState == emCmdSucess)
    {
        pCommVar->SendPrintf("%s %d <%d %d %d %d>\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            l_u8LvId,
                            g_pu8WinLayerLevel[l_u8LvId-1][emWinType_A],
                            g_pu8WinLayerLevel[l_u8LvId-1][emWinType_B],
                            g_pu8WinLayerLevel[l_u8LvId-1][emWinType_C],
                            g_pu8WinLayerLevel[l_u8LvId-1][emWinType_D]);
    }
    return l_emRtState;
}

//eFunc_SetProductName,
static emCmdFuncStaType mapp_CmdfuncSetProductName(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstProjectNameSet_t			pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;
    u8 l_u8HeadIndex = strlen(cmd_list[cmdnum].ucmd)+1;

    // #SET_NAME:xxxxxx
    if(((pCommVar->u16RecCnt-2)-l_u8HeadIndex) >= SIZE_PRJ_NAME_CHARS_MAX)
        l_emRtState = emCmdOutOfRange;

    if(l_emRtState == emCmdSucess)
    {
        SetUserData_ProjectName((char*)&pCommVar->pRecBuffer[l_u8HeadIndex],(pCommVar->u16RecCnt-2)-l_u8HeadIndex);
        pCommVar->SendPrintf("%s %s\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            pstPrjNameSet->sPrjName);
    }
    return l_emRtState;
}

//eFunc_GetProductName,
static emCmdFuncStaType mapp_CmdfuncGetProductName(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstProjectNameSet_t			pstPrjNameSet = &pstPrjSaveVar->stPrjNameSet;

    // #GET_NAME
    if(l_emRtState == emCmdSucess)
    {
        pCommVar->SendPrintf("%s %s\r\n",
                            cmd_list[cmdnum].feedbackstring,
                            pstPrjNameSet->sPrjName);
    }
    return l_emRtState;
}

//eFunc_ResetProductName,
static emCmdFuncStaType mapp_CmdfuncResetProductName(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8TempVal = 0;
    // #RESET_NAME
    if(l_emRtState == emCmdSucess)
    {
        mapp_UserDataProjectNameInit();
        SetUserData_ProjectName((char*)&l_u8TempVal,0);
        pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);
    }
    return l_emRtState;
}

#if _ENABLE_SWITCH_PORT_AND_AUDIO
//eFunc_SetFullModeToSwitchAudio,
static emCmdFuncStaType mapp_CmdfuncSetFullModeToSwitchAudio(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8TempVal;

    // #SET_FULL_SWAUD 1
    if(pCommVar->u16RecCnt-strlen(cmd_list[cmdnum].ucmd)-cmd_list[cmdnum].cmdoffset==1)
    {
        l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    }
    else
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal>1)
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal == 0)
        pstBasicVar->bFullModeToSwitchAudio = FALSE;
    else
        pstBasicVar->bFullModeToSwitchAudio = TRUE;
    SetUserData_SpecBasicVar();

    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal);
    return l_emRtState;
}

//eFunc_GetFullModeToSwitchAudio,
static emCmdFuncStaType mapp_CmdfuncGetFullModeToSwitchAudio(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    // #GET_FULL_SWAUD
    pCommVar->SendPrintf("%s %d\r\n",cmd_list[cmdnum].feedbackstring,pstBasicVar->bFullModeToSwitchAudio);
    return l_emRtState;
}
#endif

//eFunc_GetInputResolution
static emCmdFuncStaType mapp_CmdfuncGetInputResolution(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8TempVal;
    u32 l_u32Pclk = 0;
    u16 u16Width = 0;
    u16 u16Height = 0;
    u8 l_u8VFeq = 0;
    u8 l_u8Interlace = 0;

    l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    if(l_u8TempVal>4)
    {
        return emCmdOutOfRange;
    }

    if(l_u8TempVal==0)
    {
        u8 i;
        for(i = 0;i<4;i++)
        {
            l_u8Interlace=it66021_GetInputTiming(i,&u16Width,&u16Height,&l_u32Pclk);
            it66021_GetInputVFreq(i,&l_u8VFeq);
            if(l_u8Interlace)
                u16Height += u16Height;
            pCommVar->SendPrintf("%s:%d %dx%d%s %dHz\r\n",cmd_list[cmdnum].feedbackstring,i+1,u16Width,u16Height,l_u8Interlace?"I":"",l_u8VFeq);
        }
    }
    else
    {
        l_u8Interlace=it66021_GetInputTiming(l_u8TempVal-1,&u16Width,&u16Height,&l_u32Pclk);
        it66021_GetInputVFreq(l_u8TempVal-1,&l_u8VFeq);
        if(l_u8Interlace)
            u16Height += u16Height;
        pCommVar->SendPrintf("%s:%d %dx%d%s %dHz\r\n",cmd_list[cmdnum].feedbackstring,l_u8TempVal,u16Width,u16Height,l_u8Interlace?"I":"",l_u8VFeq);
    }
    return l_emRtState;
}

//eFunc_GetTxTiming,
static emCmdFuncStaType mapp_CmdfuncGetTxTiming(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    u8 l_u8TempVal;

    l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }
    g_u8QueryTxPort = l_u8TempVal-1;
    g_emSourceKeyType = eKeyValSource_Uart;
    pITE663xx_STM32_api->SysKeyIn(emKeyFunc_QueryTxTiming);
    return l_emRtState;
}

//eFunc_GetRxTiming,
static emCmdFuncStaType mapp_CmdfuncGetRxTiming(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8TempVal;

    l_u8TempVal = (pCommVar->pRecBuffer[strlen(cmd_list[cmdnum].ucmd)+1]-0x30);
    if(l_u8TempVal>4||l_u8TempVal==0)
    {
        return emCmdOutOfRange;
    }
    g_u8QueryRxPort = l_u8TempVal-1;
    g_emSourceKeyType = eKeyValSource_Uart;
    pITE663xx_STM32_api->SysKeyIn(emKeyFunc_QueryRxTiming);
    return l_emRtState;
}
//eFunc_AppLink
static emCmdFuncStaType mapp_CmdfuncAppLink(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    pCommVar->SendPrintf("%s\r\n",cmd_list[cmdnum].feedbackstring);
    return l_emRtState;
}

//eFunc_GetDisplayEdidPhysicalAddress
static emCmdFuncStaType mapp_CmdfuncGetDisplayEdidPhysicalAddress(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u16 l_u16PhyAddr;
    #if _ENABLE_CHIP_ITE6615
    l_u16PhyAddr=mapp_GetEdidPhysicalAddress(g_pu8It6615TxEdidBuffer);
    #endif

    pCommVar->SendPrintf("%s 0x%04x\r\n",cmd_list[cmdnum].feedbackstring,l_u16PhyAddr);
    return l_emRtState;
}

//eFunc_GetDisplayEdid
static emCmdFuncStaType mapp_CmdfuncGetDisplayEdid(pstUartCom_t pCommVar,u8 cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_pu8EdidBuf[256];

    #if _ENABLE_CHIP_ITE6615
    pCommVar->SendPrintf("%s:\r\n",cmd_list[cmdnum].feedbackstring);
    mapp_EdidLimitTo4K30(g_pu8It6615TxEdidBuffer,l_pu8EdidBuf,256,FALSE,TRUE);
    {
        u16 i;
        pCommVar->SendPrintf("Original Edid Data:\r\n");
        for(i=0;i<256;i++)
        {
            if(i>0 && i%16==0)
                pCommVar->SendPrintf("\r\n");
            pCommVar->SendPrintf("0x%02x,",g_pu8It6615TxEdidBuffer[i]);
        }
        pCommVar->SendPrintf("\r\n");
        pCommVar->SendPrintf("After Limit Edid Data:\r\n");
        for(i=0;i<256;i++)
        {
            if(i>0 && i%16==0)
                pCommVar->SendPrintf("\r\n");
            pCommVar->SendPrintf("0x%02x,",l_pu8EdidBuf[i]);
        }
        pCommVar->SendPrintf("\r\n");
    }
    #endif
    return l_emRtState;
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
        case eFunc_Edid_Switch:
            {
                l_bStatus = mapp_CmdfuncEdidSwitch(pCommVar,cmdnum);
            }
            break;
        case eFunc_Edid_GetMode:
            {
                l_bStatus = mapp_CmdfuncEdidGetMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_Edid_SendCustomData:
            {
                l_bStatus = mapp_CmdfuncEdidSendCustomData(pCommVar,cmdnum);
            }
            break;
        case eFunc_SwitchHDCP:
            {
                l_bStatus = mapp_CmdfuncSwitchHDCP(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetHDCPState:
            {
                l_bStatus = mapp_CmdfuncGetHDCPState(pCommVar,cmdnum);
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
        case eFunc_SetAutoSwitchMode:
            {
                l_bStatus = mapp_CmdfuncSetAutoSwitchMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAutoSwitchMode:
            {
                l_bStatus = mapp_CmdfuncGetAutoSwitchMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCECDisplay:
            {
                l_bStatus = mapp_CmdfuncSetCECDisplay(pCommVar,cmdnum);
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
        #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        case eFunc_SetOffMsgCounter:
            {
                l_bStatus = mapp_CmdfuncSetUserUartMsgCounter(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOffMsgCounter:
            {
                l_bStatus = mapp_CmdfuncGetOffMsgCounter(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetOffMsgCntDelayTime:
            {
                l_bStatus = mapp_CmdfuncSetOffMsgCntDelayTime(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetOffMsgCntDelayTime:
            {
                l_bStatus = mapp_CmdfuncGetOffMsgCntDelayTime(pCommVar,cmdnum);
            }
            break;
        #endif

        #if _ENABLE_DETECT_SYNC_TO_SEND_CEC_AND_RS232CMD
        case eFunc_SetAutoSyncDetectActionByCec:
            {
                l_bStatus = mapp_CmdfuncSetAutoSyncDetectActionByCec(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAutoSyncDetectActionByCec:
            {
                l_bStatus = mapp_CmdfuncGetAutoSyncDetectActionByCec(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetAutoSyncDetectActionByRs232:
            {
                l_bStatus = mapp_CmdfuncSetAutoSyncDetectActionByRs232(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAutoSyncDetectActionByRs232:
            {
                l_bStatus = mapp_CmdfuncGetAutoSyncDetectActionByRs232(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetAutoSyncDisplayOffDelayTime:
            {
                l_bStatus = mapp_CmdfuncSetAutoSyncDisplayOffDelayTime(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAutoSyncDisplayOffDelayTime:
            {
                l_bStatus = mapp_CmdfuncGetAutoSyncDisplayOffDelayTime(pCommVar,cmdnum);
            }
            break;
        #if _ENABLE_AUTO_POWER_FUNCTION
        case eFunc_SetAutoSyncToPowerOnOff:
            {
                l_bStatus = mapp_CmdfuncSetAutoSyncToPowerOnOff(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAutoSyncToPowerOnOff:
            {
                l_bStatus = mapp_CmdfuncGetAutoSyncToPowerOnOff(pCommVar,cmdnum);
            }
            break;
        #endif
        #endif
        #if _ENABLE_KEYPAD_LOCKED_FUNCTION
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
        #endif

        #if (_DETECT_TYPE_AUTO_SWITCH == _AUTO_SWITCH_DETECT_TYPE_HPD_AND_TMDS)
        case eFunc_SetInputSignalDetectMode:
            {
                l_bStatus = mapp_CmdfuncSetInputSignalDetectMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetInputSignalDetectMode:
            {
                l_bStatus = mapp_CmdfuncGetInputSignalDetectMode(pCommVar,cmdnum);
            }
            break;
        #endif
        #if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
        case eFunc_Edid_LoadInternalEdidData:
            {
                l_bStatus = mapp_CmdfuncLoadInternalEdidData(pCommVar,cmdnum);
            }
            break;
        #endif

        #if _ENABLE_DEBUG_LOAD_ITE_CHIP_REGISTER
        case eFunc_LoadIteChipRegisterData:
            {
                l_bStatus = mapp_CmdfuncLoadIteChipRegisterData(pCommVar,cmdnum);
            }
            break;
        case eFunc_WriteChipRegisterData:
            {
                l_bStatus = mapp_CmdfuncWriteRegisterData(pCommVar,cmdnum);
            }
            break;
        case eFunc_ReadChipRegisterData:
            {
                l_bStatus = mapp_CmdfuncReadRegisterData(pCommVar,cmdnum);
            }
            break;
        #endif

        #if _ENABLE_USER_DEFINE_CEC_CMD_TO_DEVICE
        case eFunc_SendUserCecCmd:
            {
                l_bStatus = mapp_CmdfuncSendUserCecCmd(pCommVar,cmdnum);
            }
            break;
        #endif
        case eFunc_SetResolution:
            {
                l_bStatus = mapp_CmdfuncSetResolution(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetResolution:
            {
                l_bStatus = mapp_CmdfuncGetResolution(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetMultiviewMode:
            {
                l_bStatus = mapp_CmdfuncSetMultiviewMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetMultiviewMode:
            {
                l_bStatus = mapp_CmdfuncGetMultiviewMode(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetAudioSource:
            {
                l_bStatus = mapp_CmdfuncSetAudioSource(pCommVar,cmdnum);
            }
            break;

        case eFunc_GetAudioSource:
            {
                l_bStatus = mapp_CmdfuncGetAudioSource(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetAudioMix:
            {
                l_bStatus = mapp_CmdfuncSetAudioMix(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAudioMix:
            {
                l_bStatus = mapp_CmdfuncGetAudioMix(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetAudioMute:
            {
                l_bStatus = mapp_CmdfuncSetAudioMute(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAudioMute:
            {
                l_bStatus = mapp_CmdfuncGetAudioMute(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetCecSrcMenu:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcMenu(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcUp:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcUp(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcDown:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcDown(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcLeft:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcLeft(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcRight:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcRight(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcBack:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcBack(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcEnter:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcEnter(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcOn:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcOn(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcOff:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcOff(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcStop:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcStop(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcPlay:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcPlay(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcPause:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcPause(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcPrev:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcPrev(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcNext:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcNext(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcRewind:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcRewind(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecSrcFastForward:
            {
                l_bStatus = mapp_CmdfuncSetCecSrcFastForward(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecDisOn:
            {
                l_bStatus = mapp_CmdfuncSetCecDisOn(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecDisOff:
            {
                l_bStatus = mapp_CmdfuncSetCecDisOff(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecDisSource:
            {
                l_bStatus = mapp_CmdfuncSetCecDisSource(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecDisMute:
            {
                l_bStatus = mapp_CmdfuncSetCecDisMute(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecDisVolPlus:
            {
                l_bStatus = mapp_CmdfuncSetCecDisVolPlus(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetCecDisVolMinus:
            {
                l_bStatus = mapp_CmdfuncSetCecDisVolMinus(pCommVar,cmdnum);
            }
            break;
        case eFunc_EnableMvUserConfig:
            {
                l_bStatus = mapp_CmdfuncEnableMvUserConfig(pCommVar,cmdnum);
            }
            break;
        case eFunc_ConfirmMvUserSetting:
            {
                l_bStatus = mapp_CmdfuncConfirmMvUserSetting(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSizeAndPositionWinA:
            {
                l_bStatus = mapp_CmdfuncSetSizeAndPositionWinA(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSizeAndPositionWinB:
            {
                l_bStatus = mapp_CmdfuncSetSizeAndPositionWinB(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSizeAndPositionWinC:
            {
                l_bStatus = mapp_CmdfuncSetSizeAndPositionWinC(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSizeAndPositionWinD:
            {
                l_bStatus = mapp_CmdfuncSetSizeAndPositionWinD(pCommVar,cmdnum);
            }
            break;
        case eFunc_CfgSizeAndPositionWinA:
            {
                l_bStatus = mapp_CmdfuncCfgSizeAndPositionWinA(pCommVar,cmdnum);
            }
            break;
        case eFunc_CfgSizeAndPositionWinB:
            {
                l_bStatus = mapp_CmdfuncCfgSizeAndPositionWinB(pCommVar,cmdnum);
            }
            break;
        case eFunc_CfgSizeAndPositionWinC:
            {
                l_bStatus = mapp_CmdfuncCfgSizeAndPositionWinC(pCommVar,cmdnum);
            }
            break;
        case eFunc_CfgSizeAndPositionWinD:
            {
                l_bStatus = mapp_CmdfuncCfgSizeAndPositionWinD(pCommVar,cmdnum);
            }
            break;
        case eFunc_PixelCfgSizeAndPositionWinA:
            {
                l_bStatus = mapp_CmdfuncPixelCfgSizeAndPositionWinA(pCommVar,cmdnum);
            }
            break;
        case eFunc_PixelCfgSizeAndPositionWinB:
            {
                l_bStatus = mapp_CmdfuncPixelCfgSizeAndPositionWinB(pCommVar,cmdnum);
            }
            break;
        case eFunc_PixelCfgSizeAndPositionWinC:
            {
                l_bStatus = mapp_CmdfuncPixelCfgSizeAndPositionWinC(pCommVar,cmdnum);
            }
            break;
        case eFunc_PixelCfgSizeAndPositionWinD:
            {
                l_bStatus = mapp_CmdfuncPixelCfgSizeAndPositionWinD(pCommVar,cmdnum);
            }
            break;
        case eFunc_SaveScreenToUserMode:
            {
                l_bStatus = mapp_CmdfuncSaveScreenToUserMode(pCommVar,cmdnum);
            }
            break;
        case eFunc_CleanScreen:
            {
                l_bStatus = mapp_CmdfuncCleanScreen(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetIpAddress:
            {
                l_bStatus = mapp_CmdfuncGetIpAddress(pCommVar,cmdnum);
            }
            break;

        case eFunc_SetIpAddress:
            {
                l_bStatus = mapp_CmdfuncSetIpAddress(pCommVar,cmdnum);
            }
            break;
        case eFunc_GiveIpAddress:
            {
                l_bStatus = mapp_CmdfuncGiveIpAddress(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetSwapSource:
            {
                l_bStatus = mapp_CmdfuncSetSwapSource(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetResizeWin:
            {
                l_bStatus = mapp_CmdfuncSetResizeWin(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetAllSystemSta:
            {
                l_bStatus = mapp_CmdfuncGetAllSystemSta(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetWinLayerLevel:
            {
                l_bStatus = mapp_CmdfuncSetWinLayerLevel(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetWinLayerLevel:
            {
                l_bStatus = mapp_CmdfuncGetWinLayerLevel(pCommVar,cmdnum);
            }
            break;
        case eFunc_SetProductName:
            {
                l_bStatus = mapp_CmdfuncSetProductName(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetProductName:
            {
                l_bStatus = mapp_CmdfuncGetProductName(pCommVar,cmdnum);
            }
            break;
        case eFunc_ResetProductName:
            {
                l_bStatus = mapp_CmdfuncResetProductName(pCommVar,cmdnum);
            }
            break;
    #if _ENABLE_SWITCH_PORT_AND_AUDIO
        case eFunc_SetFullModeToSwitchAudio:
            {
                l_bStatus = mapp_CmdfuncSetFullModeToSwitchAudio(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetFullModeToSwitchAudio:
            {
                l_bStatus = mapp_CmdfuncGetFullModeToSwitchAudio(pCommVar,cmdnum);
            }
            break;
    #endif
        case eFunc_GetInputResolution:
            {
                l_bStatus = mapp_CmdfuncGetInputResolution(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetTxTiming:
            {
                l_bStatus = mapp_CmdfuncGetTxTiming(pCommVar,cmdnum);
            }
            break;

        case eFunc_GetRxTiming:
            {
                l_bStatus = mapp_CmdfuncGetRxTiming(pCommVar,cmdnum);
            }
            break;
        case eFunc_AppLink:
            {
                l_bStatus = mapp_CmdfuncAppLink(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetDisplayEdidPhysicalAddress:
            {
                l_bStatus = mapp_CmdfuncGetDisplayEdidPhysicalAddress(pCommVar,cmdnum);
            }
            break;
        case eFunc_GetDisplayEdid:
            {
                l_bStatus = mapp_CmdfuncGetDisplayEdid(pCommVar,cmdnum);
            }
            break;

        case eFunc_SavePixelToUserMode:
            {
                l_bStatus = mapp_CmdfuncSavePixelToUserMode(pCommVar,cmdnum);
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
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
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


#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
void mapp_UsartUserCmdFun(pstUartCom_t pCommVar)
{
    bool l_bCmdStatus = TRUE;
	//pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	//pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
	//pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	//pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;

	if(USER_CMD_DISPLAY_ON(pCommVar->pRecBuffer))
	{
        //pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_on];
        l_bCmdStatus=mapp_UserUartLearningMessageFunction(uart_study_key_type_dis_on,pCommVar,strlen(DISPLAY_ON_TEXT),FALSE);
        #if 0
        if(l_bCmdStatus==TRUE)
        {
            #if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
            pCommVar->SendPrintf("@BAUDRATE: %d\r\n",pstUartKey->u32BaudRate);
            //pCommVar->SendPrintf("@DELAY TIME: %d s\r\n", pstUartKey->u16UartValueSendDelayTime);
            #endif
            pCommVar->SendString("@DISPLAY ON TO SEND:", TRUE);
            pCommVar->SendBytes((uint8_t *)pstUartKey->sKeyValueBuffer,
                                pstUartKey->u16KeyValueLen,
                                TRUE);
            pCommVar->SendString("\r\n", TRUE);
        }
        #endif
    }
    else if(USER_CMD_DISPLAY_OFF(pCommVar->pRecBuffer))
    {
        //pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off];
        l_bCmdStatus=mapp_UserUartLearningMessageFunction(uart_study_key_type_dis_off,pCommVar,strlen(DISPLAY_OFF_TEXT),FALSE);
        #if 0
        if(l_bCmdStatus==TRUE)
        {
            #if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
            pCommVar->SendPrintf("@BAUDRATE: %d\r\n",pstUartKey->u32BaudRate);
            //pCommVar->SendPrintf("@DELAY TIME: %d s\r\n", pstUartKey->u16UartValueSendDelayTime);
            #endif
            pCommVar->SendString("@DISPLAY OFF TO SEND:", TRUE);
            pCommVar->SendBytes((uint8_t *)pstUartKey->sKeyValueBuffer,
                                pstUartKey->u16KeyValueLen,
                                TRUE);
            pCommVar->SendString("\r\n", TRUE);
        }
        #endif
    }
    else if(USER_HEX_CMD_DISPLAY_ON(pCommVar->pRecBuffer))
	{
        //pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_on];
        l_bCmdStatus=mapp_UserUartLearningMessageFunction(uart_study_key_type_dis_on,pCommVar,strlen(DISPLAY_HEX_ON_TEXT),TRUE);
        #if 0
        if(l_bCmdStatus==TRUE)
        {
            #if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
            pCommVar->SendPrintf("@BAUDRATE: %d\r\n",pstUartKey->u32BaudRate);
            //pCommVar->SendPrintf("@DELAY TIME: %d s\r\n", pstUartKey->u16UartValueSendDelayTime);
            #endif
            pCommVar->SendString("@DISPLAY ON HEX TO SEND:", TRUE);
            pCommVar->SendBytes((uint8_t *)pstUartKey->sKeyValueBuffer,
                                pstUartKey->u16KeyValueLen,
                                TRUE);
            pCommVar->SendString("\r\n", TRUE);
        }
        #endif
    }
    else if(USER_HEX_CMD_DISPLAY_OFF(pCommVar->pRecBuffer))
    {
        //pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[uart_study_key_type_dis_off];
        l_bCmdStatus=mapp_UserUartLearningMessageFunction(uart_study_key_type_dis_off,pCommVar,strlen(DISPLAY_HEX_OFF_TEXT),TRUE);
        #if 0
        if(l_bCmdStatus==TRUE)
        {
            #if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
            pCommVar->SendPrintf("@BAUDRATE: %d\r\n",pstUartKey->u32BaudRate);
            //pCommVar->SendPrintf("@DELAY TIME: %d s\r\n", pstUartKey->u16UartValueSendDelayTime);
            #endif
            pCommVar->SendString("@DISPLAY OFF HEX TO SEND:", TRUE);
            pCommVar->SendBytes((uint8_t *)pstUartKey->sKeyValueBuffer,
                                pstUartKey->u16KeyValueLen,
                                TRUE);
            pCommVar->SendString("\r\n", TRUE);
        }
        #endif
    }
    #if _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
    else if(USER_CMD_SEND(pCommVar->pRecBuffer))
    {
        l_bCmdStatus = mapp_UartSendUserMessage(pCommVar, strlen(USER_CMD_SEND_TEXT), FALSE);
    }
    else if(USER_HEX_CMD_SEND(pCommVar->pRecBuffer))
    {
        l_bCmdStatus = mapp_UartSendUserMessage(pCommVar, strlen(USER_CMD_SEND_HEX_TEXT), TRUE);
    }
    #endif


    if(l_bCmdStatus == FALSE)
    {
        pCommVar->SendString("COMMAND ERROR\r\n",TRUE);
    }
}

bool mapp_UserHexDataCheck(char *s8Data, u16 u16Len)
{
    u16 i;
    bool l_bRet = TRUE;

    if(((u16Len-2)%3)==0)
    {
        for(i=0; i<u16Len; i=i+3)
        {
            if((HexAndAsciiChange(*(s8Data+i),0)!=*(s8Data+i))&&(HexAndAsciiChange(*(s8Data+i+1),0)!=*(s8Data+i+1)))
            {
                if((*(s8Data+i+2)!=0x20)&&(i+2!=u16Len))
                {
                    l_bRet = FALSE;
                    break;
                }
            }
            else
            {
                l_bRet = FALSE;
                break;
            }
        }
    }
    else
        l_bRet = FALSE;

    return l_bRet;
}

bool mapp_UserUartLearningMessageFunction(u8 u8Index, pstUartCom_t pCommVar, u8 u8HeadIndex, bool bIsHex)
{
    u16 l_u16Delaytime=0;
    u8 l_u8Baud=0;
    u8 l_u8DataIndex=0;
    u8 l_u8HeadIndex = u8HeadIndex;
    u16 l_u16Index;
    uint16_t u16StrSendLen;
	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
	pstUartKeyValue_t 			pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[u8Index];

    if(u8Index>=_NUM_UART_STUDY_KEYS)
        return FALSE;

    for(l_u16Index=l_u8HeadIndex ;l_u16Index<pCommVar->u16RecCnt;l_u16Index++)
    {
        if((pCommVar->pRecBuffer[l_u16Index] != '_')&&(pCommVar->pRecBuffer[l_u16Index] != ':')&&(l_u8DataIndex<1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u16Index],1)&&l_u8DataIndex==0)
            {
                if((l_u16Index-l_u8HeadIndex)>0)
                    l_u8Baud = l_u8Baud*10+(pCommVar->pRecBuffer[l_u16Index]-0x30);
                else
                    l_u8Baud = (pCommVar->pRecBuffer[l_u16Index]-0x30);
                if(l_u8Baud>7||((l_u16Index-l_u8HeadIndex)>=2))
                {
                    return FALSE;
                }
            }
            #if 0
            else if(isNumberCmd(&pCommVar->pRecBuffer[l_u16Index],1)&&l_u8DataIndex==1)
            {
                if((l_u16Index-l_u8HeadIndex)>0)
                    l_u16Delaytime = l_u16Delaytime*10+(pCommVar->pRecBuffer[l_u16Index]-0x30);
                else
                    l_u16Delaytime = (pCommVar->pRecBuffer[l_u16Index]-0x30);
                if(l_u16Delaytime>999||((l_u16Index-l_u8HeadIndex)>=3))
                {
                    return FALSE;
                }
            }
            #endif
            else
            {
                break;
            }
        }
        else if(l_u8DataIndex==1)
        {
            u16StrSendLen = pCommVar->u16RecCnt - l_u16Index;
            if(bIsHex == TRUE)
            {
                if(!mapp_UserHexDataCheck(&pCommVar->pRecBuffer[l_u16Index], u16StrSendLen))
                {
                    return FALSE;
                }
            }
            if(u16StrSendLen > 0)
            {
                switch(l_u8Baud)
                {
                    case 1:
                        pstUartKey->u32BaudRate= 115200;
                        break;
                    case 2:
                        pstUartKey->u32BaudRate = 57600;
                        break;
                    case 3:
                        pstUartKey->u32BaudRate = 38400;
                        break;
                    case 4:
                        pstUartKey->u32BaudRate = 19200;
                        break;
                    case 5:
                        pstUartKey->u32BaudRate = 9600;
                        break;
                    case 6:
                        pstUartKey->u32BaudRate = 4800;
                        break;
                    case 7:
                        pstUartKey->u32BaudRate = 2400;
                        break;
                    default:
                        //RS232_printf("l_u8Baud == %d\r\n",l_u8Baud);
                        return FALSE;
                        //break;
                }
                if(u16StrSendLen > SIZE_UART_KEY_VALUE_MAX)
                    return FALSE;

                pstUartKey->bUartValueHexOrAsciiFlag = bIsHex;
                pstUartKey->u16KeyValueLen = u16StrSendLen;
                pstUartKey->u16UartValueSendDelayTime = l_u16Delaytime;
                memset(pstUartKey->sKeyValueBuffer, 0, SIZE_UART_KEY_VALUE_MAX);
                memcpy(pstUartKey->sKeyValueBuffer, &pCommVar->pRecBuffer[l_u16Index], u16StrSendLen);

                SetUserData_UartKeyVal(u8Index);

                if(bIsHex)
                {
                    UserHexDataSendFunction(0,pstUartKey->u32BaudRate, u16StrSendLen,&pCommVar->pRecBuffer[l_u16Index]);
                }
                else
                {
                    UserDataSendFunction(0,pstUartKey->u32BaudRate, u16StrSendLen,&pCommVar->pRecBuffer[l_u16Index]);
                }
            }
            break;
        }
        else
        {
            l_u8DataIndex++;
            l_u8HeadIndex = l_u16Index+1; // 跳过"_"时，l_u16Index再加了个1
            continue;
        }
    }
    return TRUE;
}

void UserHexDataSendFunction(u8 u8Port, u32 u32Baud, u16 u16Len, char *pData)
{
    u8 i;
    USART_TypeDef *port;
    #if (_EN_UART_DEBUG_PRINTF == _MACRO_DISABLE)
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    #endif

    if(u8Port == 1)
    {
        port = USART1;
        uart1_set_baudrate(u32Baud, DEF_UART_DATA_CONFIG);
        delay_ms(10);
    }
    else if(u8Port == 0)
    {
        port = USART2;
        uart2_set_baudrate(u32Baud, DEF_UART_DATA_CONFIG);
        delay_ms(10);
    }
    for(i=0;i<u16Len;i=i+3)
    {
        mdrv_USART_SendByte(port,(HexAndAsciiChange(*(pData+i),0)<<4)|(HexAndAsciiChange(*(pData+i+1),0)));
        if(*(pData+i+2)!=0x20)
        {
            break;
        }
    }

    if(u8Port == 0)
    {
        delay_ms(10);
        #if (_EN_UART_DEBUG_PRINTF)
        uart2_set_baudrate(UART2_BAUDRATE, DEF_UART_DATA_CONFIG);
        #else
        uart2_set_baudrate(mapp_GetRealBaudrate(pstBasicSpecVar->u8baudrateId), DEF_UART_DATA_CONFIG);
        #endif
    }
    else if(u8Port == 1)
    {
        delay_ms(10);
        uart1_set_baudrate(mapp_GetRealBaudrate(baud_rate_115200), DEF_UART_DATA_CONFIG);
    }
}

void UserDataSendFunction(u8 u8Port, u32 u32Baud, u16 u16Len, char *pData)
{
    u8 i;
    USART_TypeDef *port;
    #if (_EN_UART_DEBUG_PRINTF == _MACRO_DISABLE)
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    #endif

    if(u8Port == 1)
    {
        port = USART1;
        uart1_set_baudrate(u32Baud, DEF_UART_DATA_CONFIG);
        delay_ms(10);
    }
    else if(u8Port == 0)
    {
        port = USART2;
        uart2_set_baudrate(u32Baud, DEF_UART_DATA_CONFIG);
        delay_ms(10);
    }

    for(i=0;i<u16Len;i++)
    {
        mdrv_USART_SendByte(port,*(pData+i));
    }

    if(u8Port == 0)
    {
        delay_ms(10);
        #if (_EN_UART_DEBUG_PRINTF)
        uart2_set_baudrate(UART2_BAUDRATE, DEF_UART_DATA_CONFIG);
        #else
        uart2_set_baudrate(mapp_GetRealBaudrate(pstBasicSpecVar->u8baudrateId), DEF_UART_DATA_CONFIG);
        #endif
    }
    else if(u8Port == 1)
    {
        delay_ms(10);
        uart1_set_baudrate(mapp_GetRealBaudrate(baud_rate_115200), DEF_UART_DATA_CONFIG);
    }
}

void MApp_SendPowerOnMessage(void)
{
    u8 l_u8index;
    static bool sl_bPreSendUartKeyVal[uart_study_key_type_max] = {FALSE};

	pstProjectGlobalAllVar_t	pstPrjAllVar = &g_stPrjGloVar;
	pstProjectGlobalSaveVar_t	pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
    static u8 sl_u8SendCounter[uart_study_key_type_max] = {0};

    for(l_u8index=(uart_study_key_type_t)0; l_u8index<uart_study_key_type_max; l_u8index++)
    {
        pstUartKeyValue_t           pstUartKey = &pstPrjSaveVar->stUartStudyKeyData[l_u8index];
        if(g_bSendUartKeyVal[l_u8index] == TRUE)
        {
            if(sl_bPreSendUartKeyVal[l_u8index] != g_bSendUartKeyVal[l_u8index])
            {
                sl_bPreSendUartKeyVal[l_u8index] = g_bSendUartKeyVal[l_u8index];
                PlatformTimerSet(etUserMsgTimer0+l_u8index , pstUartKey->u16UartValueSendDelayTime*1000);
                sl_u8SendCounter[l_u8index] = pstUartKey->u8UartValueSendMessageNum;
            }

            if(PlatformTimerGet(etUserMsgTimer0+l_u8index)!=0)
            {
                continue;
            }

            if(pstUartKey->u16KeyValueLen != 0)
            {
                delay_ms(20);
                {
                    if(pstUartKey->bUartValueHexOrAsciiFlag == TRUE)
                    {
                        UserHexDataSendFunction(0,
                                    pstUartKey->u32BaudRate,
                                    pstUartKey->u16KeyValueLen,
                                    pstUartKey->sKeyValueBuffer);
                    }
                    else
                    {
                        UserDataSendFunction(0,
                                    pstUartKey->u32BaudRate,
                                    pstUartKey->u16KeyValueLen,
                                    pstUartKey->sKeyValueBuffer);
                    }
                    delay_ms(20);
                }
                if(pstUartKey->u8UartValueSendMessageNum>0)
                {
                    if(sl_u8SendCounter[l_u8index]>0)
                    {
                        sl_u8SendCounter[l_u8index]--;
                        PlatformTimerSet(etUserMsgTimer0+l_u8index , pstUartKey->u16UartValueCounterDelayTime);
                    }
                }
                //bPrintedUartStudyKey[l_u8index] = TRUE;
            }

            if((sl_u8SendCounter[l_u8index]==0)||(pstUartKey->u16KeyValueLen==0))
            {
                g_bSendUartKeyVal[l_u8index] = FALSE;
                sl_bPreSendUartKeyVal[l_u8index] = g_bSendUartKeyVal[l_u8index];
            }
        }
        else
        {
            //bPrintedUartStudyKey[l_u8index] = FALSE;
        }
    }
}
#if _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
bool mapp_UartSendUserMessage(pstUartCom_t pCommVar, u8 u8HeadIndex, bool bIsHex)
{
    //u16 l_u16Delaytime=0;
    u8 l_u8Baud=0;
    u8 l_u8DataIndex=0;
    u8 l_u8HeadIndex = u8HeadIndex;
    u16 l_u16Index;
    uint16_t u16StrSendLen;
    u32 l_u32RealBaudrate = 0;

    for(l_u16Index=l_u8HeadIndex ;l_u16Index<pCommVar->u16RecCnt;l_u16Index++)
    {
        if((pCommVar->pRecBuffer[l_u16Index] != '_')&&(pCommVar->pRecBuffer[l_u16Index] != ':')&&(l_u8DataIndex<1))
        {
            if(isNumberCmd(&pCommVar->pRecBuffer[l_u16Index],1)&&l_u8DataIndex==0)
            {
                if((l_u16Index-l_u8HeadIndex)>0)
                    l_u8Baud = l_u8Baud*10+(pCommVar->pRecBuffer[l_u16Index]-0x30);
                else
                    l_u8Baud = (pCommVar->pRecBuffer[l_u16Index]-0x30);
                if(l_u8Baud>7||((l_u16Index-l_u8HeadIndex)>=2))
                {
                    return FALSE;
                }
            }
            #if 0
            else if(isNumberCmd(&pCommVar->pRecBuffer[l_u16Index],1)&&l_u8DataIndex==1)
            {
                if((l_u16Index-l_u8HeadIndex)>0)
                    l_u16Delaytime = l_u16Delaytime*10+(pCommVar->pRecBuffer[l_u16Index]-0x30);
                else
                    l_u16Delaytime = (pCommVar->pRecBuffer[l_u16Index]-0x30);
                if(l_u16Delaytime>999||((l_u16Index-l_u8HeadIndex)>=3))
                {
                    return FALSE;
                }
            }
            #endif
            else
            {
                break;
            }
        }
        else if(l_u8DataIndex==1)
        {
            u16StrSendLen = pCommVar->u16RecCnt - l_u16Index;
            if(bIsHex == TRUE)
            {
                if(!mapp_UserHexDataCheck(&pCommVar->pRecBuffer[l_u16Index], u16StrSendLen))
                {
                    return FALSE;
                }
            }
            if(u16StrSendLen > 0)
            {
                switch(l_u8Baud)
                {
                    case 1:
                        l_u32RealBaudrate = 115200;
                        break;
                    case 2:
                        l_u32RealBaudrate = 57600;
                        break;
                    case 3:
                        l_u32RealBaudrate = 38400;
                        break;
                    case 4:
                        l_u32RealBaudrate = 19200;
                        break;
                    case 5:
                        l_u32RealBaudrate = 9600;
                        break;
                    case 6:
                        l_u32RealBaudrate = 4800;
                        break;
                    case 7:
                        l_u32RealBaudrate = 2400;
                        break;
                    default:
                        //RS232_printf("l_u8Baud == %d\r\n",l_u8Baud);
                        return FALSE;
                        //break;
                }

                if(u16StrSendLen>(SIZE_UART_KEY_VALUE_MAX))
                {
                    return FALSE;
                }

                if(bIsHex)
                {
                    UserHexDataSendFunction(0,l_u32RealBaudrate, u16StrSendLen,&pCommVar->pRecBuffer[l_u16Index]);
                }
                else
                {
                    UserDataSendFunction(0,l_u32RealBaudrate, u16StrSendLen,&pCommVar->pRecBuffer[l_u16Index]);
                }
            }
            break;
        }
        else
        {
            l_u8DataIndex++;
            l_u8HeadIndex = l_u16Index+1; // 跳过"_"时，l_u16Index再加了个1
            continue;
        }
    }
    return TRUE;
}
#endif

#endif

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar)
{
    BYTE CostumEdidbuffer[256] = {0};
    u16 i;
    if(StartEdidCostumFlag)
    {
        pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
        pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
        pstProjectSpecVar_t         pstSpecVar = &stSCU41_MVSpecVar;
        pstCustomEdid_t             pstEdidOp, pstEdidOrg = &pstSpecVar->stCustomEdidPre1;
        u8 l_u8InPort = 0;

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
                #if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
                if(g_bInternalEdidFlag == TRUE)
                {
                    g_bInternalEdidFlag = FALSE;
                    #if _ENABLE_INTERNAL_EDID_SAVE_TO_MCU_FLASH
                    mdrv_FlashWrite(FLASH_ADDR_INTERNAL_EDID+g_u8InternalEDIDIndex*256,CostumEdidbuffer,128);
                    //STMFLASH_Write(EDID_START_ADDR_SAME_WITH_BOOT+g_u8InternalEDIDIndex*256,(uint16_t *)CostumEdidbuffer,128);
                    RS232_printf("FLASH WRITE OK\n");
                    #endif
                }
                else
                #endif
                {
                    if(g_u8UserEDIDIndex < emUserEdidMax)
                    {
                        pstEdidOp = pstEdidOrg + (g_u8UserEDIDIndex);
                        memset(pstEdidOp, 0, sizeof(stCustomEdid_t));
                        memcpy(pstEdidOp->pu8EdidContext, CostumEdidbuffer, 128);
                        SetUserData_SpecCustomEdid(g_u8UserEDIDIndex);
                        pUartCommVar->SendPrintf("OK\r\n");
                        #if 1
                        for(l_u8InPort=0;l_u8InPort<_HDMI_INPUT_PORT_USED_NUMBER;l_u8InPort++)
                        {
                            if((pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort] >= EDID_MODE_USER1)
                                &&(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort] <= EDID_MODE_USER_MAX))
                            {
                                SCU41_MV_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort],l_u8InPort);
                            }
                        }
                        #endif
                    }
                }
            }
            else
            {
                pUartCommVar->SendPrintf("ERROR\r\n");
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
                #if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
                if(g_bInternalEdidFlag == TRUE)
                {
                    g_bInternalEdidFlag = FALSE;
                    #if _ENABLE_INTERNAL_EDID_SAVE_TO_MCU_FLASH
                    mdrv_FlashWrite(FLASH_ADDR_INTERNAL_EDID+g_u8InternalEDIDIndex*256,CostumEdidbuffer,128);
                    //STMFLASH_Write(EDID_START_ADDR_SAME_WITH_BOOT+g_u8InternalEDIDIndex*256,(uint16_t *)CostumEdidbuffer,128);
                    RS232_printf("FLASH WRITE OK\n");
                    #endif
                }
                else
                #endif
                {

                    if(g_u8UserEDIDIndex < emUserEdidMax)
                    {
                        pstEdidOp = pstEdidOrg + (g_u8UserEDIDIndex);
                        memset(pstEdidOp, 0, sizeof(stCustomEdid_t));
                        memcpy(pstEdidOp->pu8EdidContext, CostumEdidbuffer, 256);
                        SetUserData_SpecCustomEdid(g_u8UserEDIDIndex);
                        pUartCommVar->SendPrintf("OK\r\n");

                        #if 1
                        for(l_u8InPort=0;l_u8InPort<_HDMI_INPUT_PORT_USED_NUMBER;l_u8InPort++)
                        {
                            if((pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort] >= EDID_MODE_USER1)
                                &&(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort] <= EDID_MODE_USER_MAX))
                            {
                                SCU41_MV_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidChannelMode[l_u8InPort],l_u8InPort);
                            }
                        }
                        #endif

                    }
                }
            }
            else
            {
                pUartCommVar->SendPrintf("ERROR\r\n");
            }
            return;
        }
    }
}
#endif

void mapp_SetPowerState(bool bPwrSta,bool bSendCecRs232)
{
	pstProjectSpecVar_t			pstSpecVar = &stSCU41_MVSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
            gCurSysStatus = 0x00;
            //IT663XXProResumeOrSuspend(FALSE);
            if(bSendCecRs232)
            {
                cec_cmd_tv_poweron(CEC_OPERATE_OUTPUT, FALSE);
                delay_ms(20);//wait for power supply stable
                #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
                if(pstBasicSpecVar->bActionToSendRs232CmdFlag == TRUE)
                    g_bSendUartKeyVal[uart_study_key_type_dis_on] = TRUE;
                #endif
            }
            SCU41_MV_AllLedOff();
            SetUserData_SpecBasicVar();

            mapp_SCU41MV_AudioOutputMute(pstBasicSpecVar->bAudioMute);
            #if _ENABLE_CHIP_ITE6615
            mapp_Ite6615SetPowerOffState(FALSE);
            #endif
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
            //IT663XXProResumeOrSuspend(TRUE);
            //SCU41_MV_SetPowerStateFlag(TRUE);
            if(bSendCecRs232)
            {
                cec_cmd_tv_poweroff(CEC_OPERATE_OUTPUT, FALSE);
                delay_ms(50);//wait for cec power off to be sent
                #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
                if(pstBasicSpecVar->bActionToSendRs232CmdFlag == TRUE)
                    g_bSendUartKeyVal[uart_study_key_type_dis_off] = TRUE;
                #endif
            }
            mapp_SCU41MV_AudioOutputMute(TRUE);
            SCU41_MV_AllLedOff();
            SetUserData_SpecBasicVar();
            led_on(led_name_power_red);
            led_off(led_name_power_Green);
            #if _ENABLE_CHIP_ITE6615
            mapp_Ite6615SetPowerOffState(TRUE);
            #endif
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

