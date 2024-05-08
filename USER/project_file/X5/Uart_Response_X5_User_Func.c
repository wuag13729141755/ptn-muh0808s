#define __UART_RESPON_USER_FUNC_C__

#include "includes.h"

#include "../USER/project_file/X5/mapp_UartCmdList_X5.h"

typedef void (*pFuncUartRes)(uint8_t *pu8Data, uint16_t u16Len);


extern const stUartDeclare_t 	stProjectUartWorkTable[];

static eIntCmdFun_t eIntCmdfunctionID = eIntFunc_NoUse;

//===============================================================================

//eIntFunc_CmdOk,
static emCmdFuncStaType mapp_IntCmdfuncCmdOk(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    UNUSED(pCommVar);
    UNUSED(cmdnum);
    return emCmdSucess;
}

//eIntFunc_CmdError,
static emCmdFuncStaType mapp_IntCmdfuncCmdError(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    UNUSED(pCommVar);
    UNUSED(cmdnum);
    return emCmdSucess;
}
//eIntFunc_SwitchInputID,
static emCmdFuncStaType mapp_IntCmdfuncSwitchInputID(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t		    pstSpecVar = &stEMX5RXSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8TXID = pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen];
    pstBasicVar->u8InputChannel = pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+1];

    if(pstBasicVar->bKeyPadLock == TRUE)
    {
        g_u8DispID[0] = 0xF;
        g_u8DispID[1] = 0xF;
        g_u8DispID[2] = 0xF;
        g_u8DispID[3] = 0xF;
    }
    else
    {
        if(pstBasicVar->u8TXID>=100)
        {
            g_u8DispID[2] = 12; // C
            g_u8DispID[3] = 17; // L
        }
        else
        {
            g_u8DispID[2] = (pstBasicVar->u8TXID)/10;
            g_u8DispID[3] = (pstBasicVar->u8TXID)%10;
        }
    }
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,1,NULL);

    return l_emRtState;
}
//eIntFunc_GetInputID,
static emCmdFuncStaType mapp_IntCmdfuncGetInputID(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    UNUSED(pCommVar);
    UNUSED(cmdnum);
    return emCmdSucess;
}
//eIntFunc_GetLocalID,
static emCmdFuncStaType mapp_IntCmdfuncGetLocalID(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    UNUSED(pCommVar);
    UNUSED(cmdnum);
    return emCmdSucess;
}
//eIntFunc_SetLocalID,
static emCmdFuncStaType mapp_IntCmdfuncSetLocalID(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t		    pstSpecVar = &stEMX5RXSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;


    if(pstBasicVar->bKeyPadLock == TRUE)
    {
        g_u8DispID[0] = 0xF;
        g_u8DispID[1] = 0xF;
        g_u8DispID[2] = 0xF;
        g_u8DispID[3] = 0xF;
    }
    else
    {
        if(g_u8DeviceMode==0)
        {
            pstBasicVar->u8TXID = pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen];
            if(pstBasicVar->u8TXID>=100)
            {
                g_u8DispID[2] = 12; // C
                g_u8DispID[3] = 17; // L
            }
            else
            {
                g_u8DispID[2] = (pstBasicVar->u8TXID)/10;
                g_u8DispID[3] = (pstBasicVar->u8TXID)%10;
            }
        }
        else
        {
            pstBasicVar->u8RXID = pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen];
            if(pstBasicVar->u8RXID>=100)
            {
                g_u8DispID[0] = 12; // C
                g_u8DispID[1] = 17; // L
            }
            else
            {
                g_u8DispID[0] = (pstBasicVar->u8RXID)/10;
                g_u8DispID[1] = (pstBasicVar->u8RXID)%10;
            }
        }
    }
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,1,NULL);
    mapp_EMX5RX_PlatformTimerSet(emUserTimer_IDSetTimer,500);
    return l_emRtState;
}
//eIntFunc_SetKeyLock,
static emCmdFuncStaType mapp_IntCmdfuncSetKeyLock(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->bKeyPadLock = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen]==0)? FALSE : TRUE;

    if(pstBasicVar->bKeyPadLock == TRUE)
    {
        g_u8DispID[0] = 0xF;
        g_u8DispID[1] = 0xF;
        g_u8DispID[2] = 0xF;
        g_u8DispID[3] = 0xF;
    }
    else
    {
        if(pstBasicVar->u8RXID>=100)
        {
            g_u8DispID[0] = 12; // C
            g_u8DispID[1] = 17; // L
        }
        else
        {
            g_u8DispID[0] = (pstBasicVar->u8RXID)/10;
            g_u8DispID[1] = (pstBasicVar->u8RXID)%10;
        }
        if(pstBasicVar->u8TXID>=100)
        {
            g_u8DispID[2] = 12; // C
            g_u8DispID[3] = 17; // L
        }
        else
        {
            g_u8DispID[2] = (pstBasicVar->u8TXID)/10;
            g_u8DispID[3] = (pstBasicVar->u8TXID)%10;
        }
    }
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,1,NULL);
    mapp_EMX5RX_PlatformTimerSet(emUserTimer_IDSetTimer,500);
    return l_emRtState;
}

//eIntFunc_SetEncrypt
static emCmdFuncStaType mapp_IntCmdfuncSetEncrypt(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Encrypt = 1;//mapp_GetChipIDMatchFlag() ? 0 : 1;

    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_SetEncrypt,1,&l_u8Encrypt);

    return l_emRtState;
}
//eIntFunc_SetEdidData,
static emCmdFuncStaType mapp_IntCmdfuncSetEdidData(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8EdidBuf[256];

    memcpy(l_u8EdidBuf,&pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen],255);
    //mapp_EMX5RX_GsvWriteUserEdid(l_u8EdidBuf);
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,0,NULL);
    return l_emRtState;
}

//eIntFunc_SetUartDataBypass,
static emCmdFuncStaType mapp_IntCmdfuncSetUartDataBypass(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstUartCom_t    puartPassCommVar = &g_stUartCommVar;
    pstUartCom_t    puartDbgCommVar = &g_stUart1CommVar;
    u8 l_u8Buf[1024];
    u16 l_u16Len = 0;

    memset(l_u8Buf,0,1024);
    l_u16Len = pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen]|(pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+1]<<8);
    memcpy(l_u8Buf,&pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+2],l_u16Len);

    puartPassCommVar->SendBytes(l_u8Buf,l_u16Len,FALSE);
    puartDbgCommVar->SendBytes(l_u8Buf,l_u16Len,FALSE);

    return l_emRtState;
}

//eIntFunc_SetIOMode,
static emCmdFuncStaType mapp_IntCmdfuncSetIOMode(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    pstBasicVar->u8IOMode = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen]);


    mapp_SetIOMode(pstBasicVar->u8IOMode);
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,0,NULL);

    return l_emRtState;
}

//eIntFunc_SetIOOutStatus,
static emCmdFuncStaType mapp_IntCmdfuncSetIOOutStatus(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
//    u8 l_u8Temp = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen]==0)? 0 : 1;

//    mapp_SetIOOutputSta(l_u8Temp);
//    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,0,NULL);

    return l_emRtState;
}

//eIntFunc_GetIOInputStatus,
static emCmdFuncStaType mapp_IntCmdfuncGetIOInputStatus(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
//    u8 l_u8Temp = mapp_GetIOInputSta() ? 0 : 1;

//    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_GetIOInputStatus,1,&l_u8Temp);

    return l_emRtState;
}

//eIntFunc_SendIRData,
static emCmdFuncStaType mapp_IntCmdfuncSendIRData(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8Buf[1024];
    u16 l_u16Len = 0;

    memset(l_u8Buf,0,1024);
    l_u16Len = pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen]|(pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+1]<<8);
    memcpy(l_u8Buf,&pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+2],l_u16Len);

    mapp_SendIrData(l_u8Buf,l_u16Len);
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,0,NULL);

    return l_emRtState;
}

//eIntFunc_SetUartBaudrate
static emCmdFuncStaType mapp_IntCmdfuncSetUartBaudrate(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
#if 0
    pstUartCom_t    l_puartCommVar = &g_stUart1CommVar;
    u32 l_u32Baud;
    u16 l_u16WordLengh;
    u16 l_u16StopBits;
    u16 l_u16Parity;

    l_u32Baud = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen])
               |(pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+1]<<8)
               |(pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+2]<<16)
               |(pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+3]<<24);
    l_u16WordLengh = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+4]);
    l_u16StopBits = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+5]);
    l_u16Parity = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen+6]);

    switch(l_u16WordLengh)
    {
        case 8:
            l_u16WordLengh = USART_WordLength_8b;
            break;
        case 9:
            l_u16WordLengh = USART_WordLength_9b;
            break;
        default:
            l_u16WordLengh = USART_WordLength_8b;
            break;
    }
    l_u16StopBits = l_u16StopBits==2? USART_StopBits_2: USART_StopBits_1;
    switch(l_u16Parity)
    {
        case 1:
            l_u16Parity = USART_Parity_Odd;
            break;
        case 2:
            l_u16Parity = USART_Parity_Even;
            break;
        case 0:
        default:
            l_u16Parity = USART_Parity_No;
            break;
    }

    l_puartCommVar->SetBaudRate(l_u32Baud,l_u16WordLengh,l_u16StopBits,l_u16Parity,(USART_Mode_Rx|USART_Mode_Tx),USART_HardwareFlowControl_None);
#endif
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Ok,0,NULL);

    return l_emRtState;
}

//mapp_IntCmdfuncGetDeviceVersion
static emCmdFuncStaType mapp_IntCmdfuncGetDeviceVersion(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
    u8 l_u8VerNum[3];
    u8 l_str[] = _STRING_FW_VER_NUM;
    l_u8VerNum[0] = l_str[1]-'0';
    l_u8VerNum[1] = l_str[3]-'0';
    l_u8VerNum[2] = l_str[5]-'0';
    mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_GetVersion,3,l_u8VerNum);
//    mapp_EMX3_SendCMDToSigmastar(emCmd2Sigma_Ok,0,NULL);

    return l_emRtState;
}

//eIntFunc_GetDeviceType
static emCmdFuncStaType mapp_IntCmdfuncGetDeviceType(pstUartCom_t pCommVar, uint8_t cmdnum)
{
    emCmdFuncStaType  l_emRtState = emCmdSucess;
	pstProjectSpecVar_t		    pstSpecVar = &stEMX5RXSpecVar;
	pstBasicSpecVar_t		    pstBasicVar = &pstSpecVar->stBasSpecVar;

    g_u8DeviceMode = (pCommVar->pRecBuffer[InternalCmd_list[cmdnum].cmdlen]);

    if(pstBasicVar->bKeyPadLock == TRUE)
    {
        g_u8DispID[0] = 0xF;
        g_u8DispID[1] = 0xF;
        g_u8DispID[2] = 0xF;
        g_u8DispID[3] = 0xF;
    }
    else
    {
        if(g_u8DeviceMode==0)
        {
            if(pstBasicVar->u8TXID>=100)
            {
                g_u8DispID[2] = 12; // C
                g_u8DispID[3] = 17; // L
            }
            else
            {
                g_u8DispID[2] = (pstBasicVar->u8TXID)/10;
                g_u8DispID[3] = (pstBasicVar->u8TXID)%10;
            }
        }
        else
        {
            if(pstBasicVar->u8RXID>=100)
            {
                g_u8DispID[0] = 12; // C
                g_u8DispID[1] = 17; // L
            }
            else
            {
                g_u8DispID[0] = (pstBasicVar->u8RXID)/10;
                g_u8DispID[1] = (pstBasicVar->u8RXID)%10;
            }
        }
    }

    return l_emRtState;
}

//===============================================================================
//Function process
//===============================================================================
void mapp_SetIntCmdFunctionID(eIntCmdFun_t cmdfuncid)
{
    eIntCmdfunctionID = cmdfuncid;
}

eIntCmdFun_t mapp_GetIntCmdFunctionID(void)
{
    return eIntCmdfunctionID;
}

//***********************************************************
//@Function name:    mapp_CommandFunctionProcess
//@Input:   u8 flag 标志 , char *cmd 指令集
//@Output: l_bStatus 是否执行成功
//@Description: 根据功能标志，执行相应的功能函数
//              预留了标志位，还有修改的空间
//***********************************************************
emCmdFuncStaType mapp_IntCmdFunctionProcess(u8 cmdnum,u8 flag,pstUartCom_t pCommVar)
{
    emCmdFuncStaType l_bStatus = emCmdError;

    UNUSED(flag);
    //UNUSED(cmdnum);
    switch( mapp_GetIntCmdFunctionID())
    {
        case eIntFunc_CmdOk:
            l_bStatus = mapp_IntCmdfuncCmdOk(pCommVar,cmdnum);
            break;
        case eIntFunc_CmdError:
            {
                l_bStatus = mapp_IntCmdfuncCmdError(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SwitchInputID:
            {
                l_bStatus = mapp_IntCmdfuncSwitchInputID(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_GetInputID:
            {
                l_bStatus = mapp_IntCmdfuncGetInputID(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_GetLocalID:
            {
                l_bStatus = mapp_IntCmdfuncGetLocalID(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetLocalID:
            {
                l_bStatus = mapp_IntCmdfuncSetLocalID(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetKeyLock:
            {
                l_bStatus = mapp_IntCmdfuncSetKeyLock(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetEncrypt:
            {
                l_bStatus = mapp_IntCmdfuncSetEncrypt(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetEdidData:
            {
                l_bStatus = mapp_IntCmdfuncSetEdidData(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetUartDataBypass:
            {
                l_bStatus = mapp_IntCmdfuncSetUartDataBypass(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetIOMode:
            {
                l_bStatus = mapp_IntCmdfuncSetIOMode(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetIOOutStatus:
            {
                l_bStatus = mapp_IntCmdfuncSetIOOutStatus(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_GetIOInputStatus:
            {
                l_bStatus = mapp_IntCmdfuncGetIOInputStatus(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SendIRData:
            {
                l_bStatus = mapp_IntCmdfuncSendIRData(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_SetUartBaudrate:
            {
                l_bStatus = mapp_IntCmdfuncSetUartBaudrate(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_GetDeviceVersion:
            {
                l_bStatus = mapp_IntCmdfuncGetDeviceVersion(pCommVar,cmdnum);
            }
            break;
        case eIntFunc_GetDeviceType:
            {
                l_bStatus = mapp_IntCmdfuncGetDeviceType(pCommVar,cmdnum);
            }
            break;

        case eIntFunc_NoUse:
        case eIntFunc_Max:
        default:
            break;
    }
    if(( mapp_GetIntCmdFunctionID()!=eIntFunc_NoUse)&&( mapp_GetIntCmdFunctionID()!=eIntFunc_Max))
    {
         mapp_SetIntCmdFunctionID(eIntFunc_NoUse);
    }
    return l_bStatus;
}

void mapp_UsartSystemCmdFun(pstUartCom_t pCommVar)
{
    u8 i;
    u8 l_u8Flag = 0;
    u8 l_u8Cmdnumber = 0;
    emCmdFuncStaType l_bStatus=emCmdError;

    if((pCommVar->pRecBuffer[0]==0xF5)&&(pCommVar->pRecBuffer[pCommVar->u16RecCnt-1]==0x5F))
    {
        for(i=0;i<MaxCmdNum;i++)
        {
            //g_stUart1CommVar->SendBytes(InternalCmd_list[i].pu8cmd,InternalCmd_list[i].cmdlen);
            if(InternalCmd_list[i].eCmdFun==eIntFunc_Max || InternalCmd_list[i].flag==Flag_End)
            {
                break;
            }
            if(InternalCmd_list[i].flag==Flag_NoFunction)
                continue;
            if((buffer_compare((pu8)pCommVar->pRecBuffer,InternalCmd_list[i].pu8cmd,InternalCmd_list[i].cmdlen)==TRUE))
            {
                mapp_SetIntCmdFunctionID(InternalCmd_list[i].eCmdFun);
                l_u8Flag = InternalCmd_list[i].flag;
                l_u8Cmdnumber = i;
                break;
            }
        }

        l_bStatus = mapp_IntCmdFunctionProcess(l_u8Cmdnumber,l_u8Flag, pCommVar);
    }

    if( l_bStatus == emCmdError )
    {
//        pstUartCom_t    puartPassCommVar = &g_stUartCommVar;
        pstUartCom_t    puartDbgCommVar = &g_stUart1CommVar;
//        puartPassCommVar->SendBytes((u8 *)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
//        RS232_printf("Cmd error\r\n");
        puartDbgCommVar->SendBytes((u8 *)pCommVar->pRecBuffer,pCommVar->u16RecCnt,FALSE);
//        mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Error,1,NULL);
    }
    else if( l_bStatus == emCmdOutOfRange )
    {
        //pCommVar->SendPrintf("Out Of Range\r\n");
        //RS232_printf("%s!\r\n",pCommVar->pRecBuffer);
        mapp_EMX5RX_SendCMDToSigmastar(emCmd2Sigma_Error,1,NULL);
    }
}

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar)
{
    BYTE CostumEdidbuffer[256] = {0};
    u16 i;
    if(StartEdidCostumFlag)
    {
        pstProjectGlobalAllVar_t    pstPrjAllVar = &g_stPrjGloVar;
        pstProjectGlobalSaveVar_t   pstPrjSaveVar = &pstPrjAllVar->stPrjSaveVar;
        pstProjectSpecVar_t         pstSpecVar = &stEMX5RXSpecVar;
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
                        pUartCommVar->SendPrintf("<SetUpdateEdid_True\r\n");
                        #if 1
                        if((pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex >= EDID_MODE_USER1)
                            &&(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex <= EDID_MODE_USER_MAX))
                        {
                            EMX5RX_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex);
                        }
                        #endif
                    }
                }
            }
            else
            {
                pUartCommVar->SendPrintf("<SetUpdateEdid_False\r\n");
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
                        pUartCommVar->SendPrintf("<SetUpdateEdid_True\r\n");
                        #if 1
                        if((pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex >= EDID_MODE_USER1)
                            &&(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex <= EDID_MODE_USER_MAX))
                        {
                            EMX5RX_EDIDAssignByID(pstPrjSaveVar->stEdidSelIndex.u8EdidFlashIndex);
                        }
                        #endif
                    }
                }
            }
            else
            {
                pUartCommVar->SendPrintf("<SetUpdateEdid_False\r\n");
            }
            return;
        }
    }
}
#endif

void mapp_SetPowerState(bool bPwrSta)
{
	pstProjectSpecVar_t			pstSpecVar = &stEMX5RXSpecVar;
	pstBasicSpecVar_t			pstBasicSpecVar = &pstSpecVar->stBasSpecVar;
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

