#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/MHD1616S-TX/mapp_UartCmdList_MHD1616S_TX.h"

extern stUartCom_t              g_stUart1CommVar, g_stUart2CommVar,g_stUart3CommVar;
extern const stUartDeclare_t    stProjectUartWorkTable[];

BYTE g_u8MultiCmdDelayCounter = 0;

u8 g_u8VersionReceive = 0x00;

void UartCommVarClear(pstUartCom_t pCommVar);

void mapp_SetPowerState(bool bPwrSta)
{
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
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
            case eFpgaRegGetVersion:
                {
                    g_u8VersionReceive = pCommVar->pRecBuffer[7];
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
    mapp_MHD1616STXSendCmdToSWBD(eMcuReg_CardType,l_u8Sendbuf,1);

    return (u8)l_emRtState;
}

//eMcuReg_Version             = 0x02,
u8 mapp_FuncGetVersion(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Sendbuf[2];
    l_u8Sendbuf[0] = VERSION_STR(_STRING_FW_VER_NUM);
    l_u8Sendbuf[1] = g_u8VersionReceive;
    mapp_MHD1616STXSendCmdToSWBD(eMcuReg_Version,l_u8Sendbuf,2);

    return (u8)l_emRtState;
}
//eMcuReg_LinkSta             = 0x03,
u8 mapp_FuncLinkSta(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Sendbuf[2];
    u16 l_u16LinkSta = 0;
    u8 i;
    for(i=0;i<Def_outport_num;i++)
    {
        if(GsvGetTxHpdStatus(i))
        {
            l_u16LinkSta = l_u16LinkSta|(1<<i);
        }
    }
    l_u8Sendbuf[0] = (l_u16LinkSta)&0xFF;
    l_u8Sendbuf[1] = (l_u16LinkSta>>8)&0xFF;

    mapp_MHD1616STXSendCmdToSWBD(eMcuReg_LinkSta,l_u8Sendbuf,2);

    return (u8)l_emRtState;
}
//eMcuReg_HdcpAndFormat       = 0x04,
u8 mapp_FuncHdcpAndFormat(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8TxPort = pCmdBuf[7];
    u8 l_u8HdcpMode;
    u8 l_u8TxFormat;

    l_u8HdcpMode = pCmdBuf[8];
    l_u8TxFormat = pCmdBuf[9];

    if((l_u8TxPort>Def_outport_num)||(l_u8HdcpMode>1)||(l_u8TxFormat>1))
    {
        l_emRtState = emCmdOutOfRange;
        dbg_printf("Out of range\r\n");
    }
    if(l_emRtState == emCmdSucess)
    {
        if(l_u8TxPort==0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicSpecVar->u8TxHdcpMode[i] = l_u8HdcpMode;
                pstBasicSpecVar->u8TxFormat[i] = l_u8TxFormat==0?1:0;
                GsvSetTxMode(i,pstBasicSpecVar->u8TxFormat[i]);
                GsvSetTxHdcpStatus(i,pstBasicSpecVar->u8TxHdcpMode[i]);
            }
        }
        else
        {
            pstBasicSpecVar->u8TxHdcpMode[l_u8TxPort-1] = l_u8HdcpMode;
            pstBasicSpecVar->u8TxFormat[l_u8TxPort-1] = l_u8TxFormat==0?1:0;
            GsvSetTxMode(l_u8TxPort-1,pstBasicSpecVar->u8TxFormat[l_u8TxPort-1]);
            GsvSetTxHdcpStatus(l_u8TxPort-1,pstBasicSpecVar->u8TxHdcpMode[l_u8TxPort-1]);
        }
        SetUserData_SpecBasicVar();

    }
    return (u8)l_emRtState;
}
//eMcuReg_TxResolution        = 0x05,
u8 mapp_FuncTxResolution(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8TxPort = pCmdBuf[7];
    u16 l_u16CmdLen = pCmdBuf[5]|(pCmdBuf[6]<<8);
    u8 l_u8Sendbuf[2];

    if(l_u8TxPort>Def_outport_num)
        l_emRtState = emCmdError;

    if(l_emRtState == emCmdSucess)
    {
        if(l_u8TxPort==0)
        {
            u8 i;
            for(i=0;i<Def_outport_num;i++)
            {
                memcpy(&pstBasicSpecVar->stCurTimingVar[i],&pCmdBuf[8],l_u16CmdLen-1);
                mapp_MHD1616STXResolutionConfig2(i,&pstBasicSpecVar->stCurTimingVar[i]);
            }
            dbg_printf("l_u8TxPort  == %d \r\n",l_u8TxPort);
            dbg_printf("u16HActive  == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16HActive);
            dbg_printf("u16HTotal   == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16HTotal);
            dbg_printf("u16HBP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16HBP);
            dbg_printf("u16HFP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16HFP);
            dbg_printf("u8HSync     == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16HSync);
            dbg_printf("u8HPol      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u8HPol);
            dbg_printf("u16VActive  == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16VActive);
            dbg_printf("u16VTotal   == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16VTotal);
            dbg_printf("u16VBP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16VBP);
            dbg_printf("u16VFP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16VFP);
            dbg_printf("u8VSync     == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16VSync);
            dbg_printf("u8VPol      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u8VPol);
            dbg_printf("u8VClk      == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u8VClk);
            dbg_printf("u8Vic       == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u8Vic);
            dbg_printf("u16PClk     == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u16PClk);
            dbg_printf("u8Interlace == %d \r\n",pstBasicSpecVar->stCurTimingVar[0].u8Interlace);
        }
        else
        {
            memcpy(&pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1],&pCmdBuf[8],l_u16CmdLen-1);
            mapp_MHD1616STXResolutionConfig2(l_u8TxPort-1,&pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1]);
            dbg_printf("l_u8TxPort  == %d \r\n",l_u8TxPort);
            dbg_printf("u16HActive  == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16HActive);
            dbg_printf("u16HTotal   == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16HTotal);
            dbg_printf("u16HBP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16HBP);
            dbg_printf("u16HFP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16HFP);
            dbg_printf("u8HSync     == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16HSync);
            dbg_printf("u8HPol      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u8HPol);
            dbg_printf("u16VActive  == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16VActive);
            dbg_printf("u16VTotal   == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16VTotal);
            dbg_printf("u16VBP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16VBP);
            dbg_printf("u16VFP      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16VFP);
            dbg_printf("u8VSync     == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16VSync);
            dbg_printf("u8VPol      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u8VPol);
            dbg_printf("u8VClk      == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u8VClk);
            dbg_printf("u8Vic       == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u8Vic);
            dbg_printf("u16PClk     == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u16PClk);
            dbg_printf("u8Interlace == %d \r\n",pstBasicSpecVar->stCurTimingVar[l_u8TxPort-1].u8Interlace);
        }
        SetUserData_SpecBasicVar();
        l_u8Sendbuf[0] = l_u8TxPort;
        l_u8Sendbuf[1] = 1;
        mapp_MHD1616STXSendCmdToSWBD(eMcuReg_TxResolution,l_u8Sendbuf,2);
    }
    return (u8)l_emRtState;
}
//eMcuReg_TxWinConfig         = 0x06,
u8 mapp_FuncTxWinConfig(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8TxPort = pCmdBuf[7];
    u16 l_u16CmdLen = pCmdBuf[5]|(pCmdBuf[6]<<8);
    u8 l_u8Sendbuf[2];

    if((l_u8TxPort==0)||(l_u8TxPort>Def_outport_num))
        l_emRtState = emCmdError;

    if(l_emRtState == emCmdSucess)
    {
        memcpy(&pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1],&pCmdBuf[7],l_u16CmdLen);
        dbg_printf("u8SrcAddr   == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8SrcAddr);
        dbg_printf("u8LayerID   == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8LayerID);
        dbg_printf("u8Layer_D0  == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8Layer_D0);
        dbg_printf("u8Layer_D1  == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8Layer_D1);
        dbg_printf("u8Layer_D2  == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8Layer_D2);
        dbg_printf("u8Layer_D3  == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8Layer_D3);
        dbg_printf("u16SrcAct_H == 0x%04x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u16SrcAct_H);
        dbg_printf("u16SrcAct_V == 0x%04x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u16SrcAct_V);
        dbg_printf("u16Src_X    == 0x%04x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u16Src_X);
        dbg_printf("u16Src_Y    == 0x%04x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u16Src_Y);
        dbg_printf("u16Src_H    == 0x%04x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u16Src_H);
        dbg_printf("u16Src_V    == 0x%04x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u16Src_V);
        dbg_printf("u32Wnd_X    == 0x%08x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u32Wnd_X);
        dbg_printf("u32Wnd_Y    == 0x%08x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u32Wnd_Y);
        dbg_printf("u32Wnd_H    == 0x%08x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u32Wnd_H);
        dbg_printf("u32Wnd_V    == 0x%08x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u32Wnd_V);
        dbg_printf("u32Srf_H    == 0x%08x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u32Srf_H);
        dbg_printf("u32Srf_V    == 0x%08x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u32Srf_V);
        dbg_printf("u8SrcInput  == 0x%02x \r\n",pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1].u8SrcInput);
        mapp_MHD1616STXWindowCfg2(l_u8TxPort,&pstBasicSpecVar->stWinCfgVar[l_u8TxPort-1]);
        SetUserData_SpecBasicVar();
        l_u8Sendbuf[0] = l_u8TxPort;
        l_u8Sendbuf[1] = 1;
        mapp_MHD1616STXSendCmdToSWBD(eMcuReg_TxWinConfig,l_u8Sendbuf,2);
    }
    return (u8)l_emRtState;
}
//eMcuReg_GetTxPortEdid       = 0x07,
u8 mapp_FuncGetTxPortEdid(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Sendbuf[257];
    u8 l_u8TxPort = pCmdBuf[7];

    memset(l_u8Sendbuf,0,257);
    if(l_u8TxPort>0&&l_u8TxPort<=Def_outport_num)
    {
        l_u8Sendbuf[0] = l_u8TxPort;
        memcpy(&l_u8Sendbuf[1],g_u8DisplayEdid[l_u8TxPort-1],256);
        mapp_MHD1616STXSendCmdToSWBD(eMcuReg_GetTxPortEdid,l_u8Sendbuf,257);
    }
    return (u8)l_emRtState;
}
//eMcuReg_ResetFactory        = 0x08,
u8 mapp_FuncResetFactory(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8ResetFlag = pCmdBuf[7];

    if(l_u8ResetFlag)
    {
        MHD1616STX_SpecBasicVarDefault();
        SetUserData_SpecBasicVar();
        #if _ENABLE_SYSTEM_RESET_DELAY_TIME
        mapp_SetSysResetModeDelayTime(1500);
        #endif
    }
    return (u8)l_emRtState;
}
//eMcuReg_SetBritness         = 0x09,
u8 mapp_FuncSetBritness(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;

    return (u8)l_emRtState;
}

//eMcuReg_UpdateWincfg = 0x0A
u8 mapp_FuncUpdateWincfg(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8UpdateFlag = pCmdBuf[7];

    if(l_u8UpdateFlag)
    {
        mapp_MHD1616STXFpgaWinCmdUpdate(10);
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
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8bgSta = pCmdBuf[7];

    dbg_printf("Dbg State :%s\r\n",l_u8bgSta?"En":"Dis");
    pstBasicSpecVar->bIsDebugEn = l_u8bgSta?TRUE:FALSE;
    SetUserData_SpecBasicVar();
    g_bEnableDbgFlag = pstBasicSpecVar->bIsDebugEn;

    return (u8)l_emRtState;
}

//eMcuReg_SendRxTiming        = 0x0C,
u8 mapp_FuncSendRxTiming(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u16 l_u16TimWidth,l_u16TimHeight;
//    u8 l_u8Sendbuf[2];
    u8 i;
    for(i=0;i<_HDMI_INPUT_PORT_USED_NUMBER;i++)
    {
        l_u16TimWidth  = pCmdBuf[7+i*4]|(pCmdBuf[8+i*4]<<8);
        l_u16TimHeight = pCmdBuf[9+i*4]|(pCmdBuf[10+i*4]<<8);
        if((l_u16TimWidth!=g_stRxResTable[i].u16Width)
            ||(l_u16TimHeight!=g_stRxResTable[i].u16Height))
        {
            g_stRxResTable[i].u16Width  = l_u16TimWidth;
            g_stRxResTable[i].u16Height = l_u16TimHeight;
            g_stRxResTable[i].bChangeFlag = TRUE;
        }
        dbg_printf("RxPort[%d] u16Width 0x%x\r\n",i+1,g_stRxResTable[i].u16Width);
        dbg_printf("RxPort[%d] u16Height 0x%x\r\n",i+1,g_stRxResTable[i].u16Height);
//        l_u8Sendbuf[0] = 1;
        //mapp_MHD1616STXSendCmdToSWBD(eMcuReg_SendRxTiming,l_u8Sendbuf,1);
    }

    return (u8)l_emRtState;
}

//eMcuReg_SendResID           = 0x0D,
u8 mapp_FuncSendResID(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 l_u8TxPort = pCmdBuf[7];
    u8 l_u8ResId = pCmdBuf[8];
    u8 i;

    if(l_u8TxPort<=Def_outport_num)
    {
        if(l_u8TxPort==0)
        {
            for(i=0;i<Def_outport_num;i++)
            {
                pstBasicSpecVar->u8ResolutionId[i] = l_u8ResId-1;
                mapp_MHD1616STXResolutionConfig(i,l_u8ResId-1);
            }
        }
        else
        {
            pstBasicSpecVar->u8ResolutionId[i] = l_u8ResId-1;
            mapp_MHD1616STXResolutionConfig(l_u8TxPort-1,l_u8ResId-1);
        }
        SetUserData_SpecBasicVar();
    }

    return (u8)l_emRtState;
}

//eMcuReg_SendInputChannel           = 0x0E,
u8 mapp_FuncSendInputChannel(char *pCmdBuf,u16 u16CmdLen)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stMHD1616STXSpecVar;
    pstBasicSpecVar_t           pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
    u8 i;

    for(i=0;i<Def_outport_num;i++)
    {
        pstBasicSpecVar->u8TxSrc[i] = pCmdBuf[7+i];
        mapp_MHD1616STXWindowCfg(i+1,pstBasicSpecVar->u8TxSrc[i]+1);
    }
    SetUserData_SpecBasicVar();
    mapp_MHD1616STXFpgaWinCmdUpdate(10);

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
    {eMcuReg_CardType,      mapp_FuncCardType},
    {eMcuReg_Version,       mapp_FuncGetVersion},
    {eMcuReg_LinkSta,       mapp_FuncLinkSta},
    {eMcuReg_HdcpAndFormat, mapp_FuncHdcpAndFormat},
    {eMcuReg_TxResolution,  mapp_FuncTxResolution},
    {eMcuReg_TxWinConfig,   mapp_FuncTxWinConfig},
    {eMcuReg_GetTxPortEdid, mapp_FuncGetTxPortEdid},
    {eMcuReg_ResetFactory,  mapp_FuncResetFactory},
    {eMcuReg_SetBritness,   mapp_FuncSetBritness},
    {eMcuReg_UpdateWincfg,  mapp_FuncUpdateWincfg},
    {eMcuReg_PowerState,    mapp_FuncPowerState},
    {eMcuReg_SetDebugEn,    mapp_FuncSetDebugEn},
    {eMcuReg_SendRxTiming,  mapp_FuncSendRxTiming},
    {eMcuReg_SendResID,     mapp_FuncSendResID},
    {eMcuReg_SendInputChannel,  mapp_FuncSendInputChannel},
    {eMcuReg_UpdateMcu,     mapp_FuncUpdateMcu},
    {eMcuReg_UpdateFpga,    mapp_FuncUpdateFpga},


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

