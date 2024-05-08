#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/MHD1616S-RX/mapp_UartCmdList_MHD1616S_RX.h"

extern stUartCom_t				g_stUart1CommVar, g_stUart2CommVar,g_stUart3CommVar;
extern const stUartDeclare_t 	stProjectUartWorkTable[];

u8 g_u8VersionReceive = 0x00;


void UartCommVarClear(pstUartCom_t pCommVar);


void mapp_SetPowerState(bool bPwrSta)
{
	pstProjectSpecVar_t			pstSpecVar = &stMHD1616SRXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    if(bPwrSta)
    {
        if(!pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = TRUE;
//            SetUserData_SpecBasicVar();
        }
    }
    else
    {
        if(pstBasicSpecVar->bPowerOnSta)
        {
            pstBasicSpecVar->bPowerOnSta = FALSE;
//            SetUserData_SpecBasicVar();
        }
    }
}

void mapp_Usart2SystemCmdFun(pstUartCom_t pCommVar)
{
    u8 l_u8Addr;
    u8 l_u8Cmd;
    if((pCommVar->pRecBuffer[0]==0xFF)&&(pCommVar->pRecBuffer[1]==0xFF)&&(pCommVar->pRecBuffer[2]==0xA5))
    {
        l_u8Addr = pCommVar->pRecBuffer[3];
        l_u8Cmd = pCommVar->pRecBuffer[4];

        dbg_printf("addr[0x%x],cmd[0x%x]\r\n",l_u8Addr,l_u8Cmd);
        switch(l_u8Cmd)
        {
            case 0x03://eFpgaRegGetVersion:
                {
                    g_u8VersionReceive = pCommVar->pRecBuffer[7];
                    dbg_printf("g_u8VersionReceive[0x%x]\r\n",g_u8VersionReceive);
                }
                break;
            default:
                break;
        }
    }
}

//eMcuReg_CardType            = 0x01,
u8 mapp_FuncCardType(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Sendbuf[1];
    l_u8Sendbuf[0] = 1;
    mapp_MHD1616SRXSendCmdToSWBD(eMcuReg_CardType,l_u8Sendbuf,1);

    return (u8)l_emRtState;
}

//eMcuReg_Version             = 0x02,
u8 mapp_FuncGetVersion(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Sendbuf[2];
    l_u8Sendbuf[0] = VERSION_STR(_STRING_FW_VER_NUM);
    l_u8Sendbuf[1] = g_u8VersionReceive;
    mapp_MHD1616SRXSendCmdToSWBD(eMcuReg_Version,l_u8Sendbuf,2);

    return (u8)l_emRtState;
}
//eMcuReg_LinkSta             = 0x03,
u8 mapp_FuncLinkSta(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8LinkSta = 0;
    u8 i;
    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        if(mapp_it66021_GetInputHpdSta(i)==TRUE)
        {
            l_u8LinkSta = l_u8LinkSta|(1<<i);
        }
    }
    mapp_MHD1616SRXSendCmdToSWBD(eMcuReg_LinkSta,&l_u8LinkSta,1);

    return (u8)l_emRtState;
}

//eMcuReg_ResetFactory        = 0x08,
u8 mapp_FuncResetFactory(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8ResetFlag = pCmdBuf[7];

    if(l_u8ResetFlag)
    {
        MHD1616SRX_SpecBasicVarDefault();
        SetUserData_SpecBasicVar();
        #if _ENABLE_SYSTEM_RESET_DELAY_TIME
        mapp_SetSysResetModeDelayTime(1500);
        #endif
    }
    return (u8)l_emRtState;
}
//eMcuReg_PowerState = 0x0B
u8 mapp_FuncPowerState(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8PowerFlag = pCmdBuf[7];

    if(l_u8PowerFlag)
        mapp_SetPowerState(TRUE);
    else
        mapp_SetPowerState(FALSE);
    return (u8)l_emRtState;
}
//eMcuReg_SetDebugEn = 0xF1
u8 mapp_FuncSetDebugEn(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SRXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8bgSta = pCmdBuf[7];

    pstBasicSpecVar->bIsDebugEn = l_u8bgSta?TRUE:FALSE;
    SetUserData_SpecBasicVar();
    g_bEnableDbgFlag = pstBasicSpecVar->bIsDebugEn;
    dbg_printf("Dbg State :%s\r\n",l_u8bgSta?"En":"Dis");

    return (u8)l_emRtState;
}

//eMcuReg_GetRxResolution = 0x81
u8 mapp_FuncGetRxResolution(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8SendBuf[50];
    memset(l_u8SendBuf,0,50);
//    g_stRxResolution[i].u16HActive;
//    g_stRxResolution[i].u16VActive;
    memcpy(l_u8SendBuf,g_stRxResolution,48);
    mapp_MHD1616SRXSendCmdToSWBD(eMcuReg_GetRxResolution,l_u8SendBuf,48);

    return (u8)l_emRtState;
}

//eMcuReg_SetRxPortEdid = 0x82
u8 mapp_FuncSetRxPortEdid(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616SRXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8RxPort = pCmdBuf[7];

    {
        u16 j;
        dbg_printf("Tx Prot[%d] Edid:\r\n",l_u8RxPort);
		for(j=0;j<256;j++)
		{
			dbg_printf("%02x ",pCmdBuf[8+j]);
			if((j+1)%16==0)
				dbg_printf("\r\n");
		}
		dbg_printf("\r\n");
    }

    if(/*l_u8RxPort>0&&*/l_u8RxPort<=_HDMI_INPUT_PORT_USED_NUMBER)
    {
        if(l_u8RxPort==0)
        {
            u8 i;
            for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
            {
                memcpy(pstBasicSpecVar->u8InputEdid[i],&pCmdBuf[8],256);
                it66021EdidChange(i,pstBasicSpecVar->u8InputEdid[i]);
            }
        }
        else
        {
            memcpy(pstBasicSpecVar->u8InputEdid[l_u8RxPort-1],&pCmdBuf[8],256);
            it66021EdidChange(l_u8RxPort-1,pstBasicSpecVar->u8InputEdid[l_u8RxPort-1]);
        }
    }
    SetUserData_SpecBasicVar();

    return (u8)l_emRtState;
}

//eMcuReg_UpdateMcu           = 0xF2,
u8 mapp_FuncUpdateMcu(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    mcu_file_updata(0,NULL,0);

    return (u8)l_emRtState;
}
//eMcuReg_UpdateFpga          = 0xF3,
u8 mapp_FuncUpdateFpga(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}

///========================================================///
stUartList_t Uart1CmdTable[]=
{
    {eMcuReg_CardType,          mapp_FuncCardType},
    {eMcuReg_Version,           mapp_FuncGetVersion},
    {eMcuReg_LinkSta,           mapp_FuncLinkSta},
    {eMcuReg_ResetFactory,      mapp_FuncResetFactory},
    {eMcuReg_PowerState,        mapp_FuncPowerState},
    {eMcuReg_SetDebugEn,        mapp_FuncSetDebugEn},
    {eMcuReg_GetRxResolution,   mapp_FuncGetRxResolution},
    {eMcuReg_SetRxPortEdid,     mapp_FuncSetRxPortEdid},
    {eMcuReg_UpdateMcu,         mapp_FuncUpdateMcu},
    {eMcuReg_UpdateFpga,        mapp_FuncUpdateFpga},
    //end
    {0,NULL},
};

void mapp_UsartSystemCmdFun(pstUartCom_t pComVar)
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
            dbg_printf("addr[0x%x],cmd[0x%x] cmdlen[0x%x]\r\n",l_u8Addr,l_u8Cmd,l_u16CmdLen);

            if(l_u8Addr == g_u8BdAddr)
            {
                for(i=0;i<=255;i++)
                {
                    if((Uart1CmdTable[i].u8Cmd == 0)||(Uart1CmdTable[i].pCmdFun == NULL))
                        break;
                    if((l_u8Cmd == Uart1CmdTable[i].u8Cmd)&&(Uart1CmdTable[i].pCmdFun!=NULL))
                    {
                        l_bStatus = (emCmdFuncStaType)Uart1CmdTable[i].pCmdFun(l_ps8CmdBuf,(l_u16CmdLen+7));
                        if(l_bStatus == emCmdError)
                        {
                        }
                        break;
                    }
                }
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

