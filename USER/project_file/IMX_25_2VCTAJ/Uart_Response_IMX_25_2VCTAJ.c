#define __UART_RESPONSE_C__
#include "includes.h"

#define TIME_UART_RECEIVE_CMD_TIMEOUT_VALID			30//unit is 1ms, total is 30ms
#define TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID		100//unit is 1ms, total is 300ms..

#define SIZE_UART_RECEIVE_CMD						300
#define SIZE_UART_CMD_MIN_LEN						2

stUartCom_t			g_stUartCommVar, g_stUart1CommVar;
char 				uart_rec_buffer[SIZE_UART_RECEIVE_CMD];
char 				uart1_rec_buffer[SIZE_UART_RECEIVE_CMD];


#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
SemaphoreHandle_t	xSemaphore_Rs232Snd;
SemaphoreHandle_t	xSemaphore_Uart1Snd;
#if 0
SemaphoreHandle_t	xSemaphore_MstSnd;
#endif
#endif
//QueueHandle_t 		xQueue_EdidUpdate = NULL;

void UartCommVarClear(pstUartCom_t pCommVar);

void Uart1ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;

    //if(pUartCommVar->bRecCmdFlg) return;

    if(pUartCommVar->u16RecCnt < (SIZE_UART_RECEIVE_CMD-1))
    {
        pUartCommVar->pRecBuffer[pUartCommVar->u16RecCnt++] = u8RecChar;
    }

    #if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID*4);
    }
    else
    #endif
#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    if(USER_CMD_HEAD(pUartCommVar->pRecBuffer))  //head for user hex
    {
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_VALID);
    }
    else
#endif
    if((pUartCommVar->pRecBuffer[0]=='#')) // head char '#'
    {
        pUartCommVar->bRecCmdFlg = FALSE;
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID);
    }
    else
    {
        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID;
    }
}


void UartProtocalDataReceive(uint8_t u8RecChar)
{
	pstUartCom_t pUartCommVar = &g_stUartCommVar;

	//if(pUartCommVar->bRecCmdFlg) return;

	if(pUartCommVar->u16RecCnt < (SIZE_UART_RECEIVE_CMD-1))
	{
		pUartCommVar->pRecBuffer[pUartCommVar->u16RecCnt++] = u8RecChar;
	}

	#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID*4);
    }
    else
    #endif
    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    if(USER_CMD_HEAD(pUartCommVar->pRecBuffer))  //head for user hex
    {
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_VALID);
    }
    else
    #endif
    if((pUartCommVar->pRecBuffer[0]=='#')) // head char '#'
    {
        pUartCommVar->bRecCmdFlg = FALSE;
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID);
    }
	else
	{
		pUartCommVar->bTimeOutValid = FALSE;
		pUartCommVar->u32RecTimeout = TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID;
	}
}

#if 0
void UartParserMethod(void)
{
	pstUartCom_t 			pUartCommVar = &g_stUartCommVar;
	pstUartCom_t			pGuiCommVar = &g_stGuiCommVar;
	pstUartCom_t			pCommVar;
	pstUartCmdRx_t			pstCmdTbl, pstCmdTblOrg = (pstUartCmdRx_t)&stCmdUartRxTable[0];
	uint8_t 				i;
	pstGuiIp_t				pstGuiIp = &g_stGuiIpAdr;

	if(bGuiIpPrintfFlag)
	{
		CommResMsgSnd((UR_STRING_GUI_IP_GET_SEND_ROOT, pstGuiIp->pu8IpContent));
		bGuiIpPrintfFlag = FALSE;
	}

	if((pUartCommVar->bRecCmdFlg)||(pGuiCommVar->bRecCmdFlg))
	{
		if(pUartCommVar->bRecCmdFlg) pCommVar = pUartCommVar;
		else pCommVar = pGuiCommVar;

		for(i=0; i<g_u8SizeOfstCmdUartRxTable; i++)
		{
			pstCmdTbl = pstCmdTblOrg + i;
			if(!strncmp(pCommVar->pRecBuffer, (char *)pstCmdTbl->cmdStr, strlen(pstCmdTbl->cmdStr)))
			{
				pstCmdTbl->funcResponse((uint8_t *)(pCommVar->pRecBuffer+strlen(pstCmdTbl->cmdStr)), (pCommVar->u16RecCnt-strlen(pstCmdTbl->cmdStr)));
				break;
			}
		}

		UartCommVarClear(pCommVar);
	}
}
#endif

void UartParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUartCommVar;

    #if 0//_ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    MApp_SendPowerOnMessage();
    #endif

    if(pUartCommVar->bRecCmdFlg)
    {

        #if 0//_ENABLE_USER_EDID_LEARNING_FUNCTION
        if(StartEdidCostumFlag)
        {
            mapp_UartUserEdidReceive(pUartCommVar);
        }
        else
        #endif
        #if 0////_ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(USER_CMD_HEAD(pUartCommVar->pRecBuffer))  //head "/-" for user hex
        {
            mapp_UsartUserCmdFun(pUartCommVar);
        }
        else
        #endif
        {
            //mapp_UsartSystemCmdFun(pUartCommVar);
        }

        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(PlatformTimerGet(etUserEdidReceiveTime) == 0&&StartEdidCostumFlag)
    {
        StartEdidCostumFlag = FALSE;
        pUartCommVar->SendString("@TIMEOUT TO SEND EDID\r\n", TRUE);
    }
#endif


#if _ENABLE_SYSTEM_RESET_DELAY_TIME
    if(mapp_GetSysResetMode()&&mapp_CheckAllDataHasSave())
    {
        SYS_SoftReset();
    }
#endif
}

void Uart1ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart1CommVar;

    if(pUartCommVar->bRecCmdFlg)
    {
        #if 0//_ENABLE_USER_EDID_LEARNING_FUNCTION
        if(StartEdidCostumFlag)
        {
            mapp_UartUserEdidReceive(pUartCommVar);
        }
        else
        #endif
        #if 0//_ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(USER_CMD_HEAD(pUartCommVar->pRecBuffer))  //head "/-" for user hex
        {
            mapp_UsartUserCmdFun(pUartCommVar);
        }
        else
        #endif
        {
            //mapp_UsartSystemCmdFun(pUartCommVar);
        }

        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart1_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }
}

void UartVarInit(void)
{
	pstUartCom_t 	pUartCommVar = &g_stUartCommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart2_xSemaphoreInit();
    #else
	xSemaphore_Rs232Snd = xSemaphoreCreateMutex();
    #endif
	//xQueue_EdidUpdate = xQueueCreate(1, sizeof(pstEdidUpgrData_t));
	//if(xQueue_EdidUpdate == 0)
	//{
	//	Msg_SemC(("xQueue_EdidUpdate create failed\r\n"));
	//}

	memset(pUartCommVar, 0, sizeof(stUartCom_t));

	pUartCommVar->pRecBuffer = uart_rec_buffer;

	pUartCommVar->SetBaudRate = uart2_set_baudrate;
	pUartCommVar->SendBytes = uart2_send_bytes_A;
	pUartCommVar->SendString = uart2_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    pUartCommVar->SendPrintf = USER_printf;
#endif

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    StartEdidCostumFlag = FALSE;
    g_u8UserEDIDIndex = 0;
#if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
    g_u8InternalEDIDIndex = 0;
    g_bInternalEdidFlag = FALSE;
#endif
#endif
}

void Uart1VarInit(void)
{
	pstUartCom_t 	puart1CommVar = &g_stUart1CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart1_xSemaphoreInit();
    #else
	xSemaphore_Uart1Snd = xSemaphoreCreateMutex();
    #endif
	memset(puart1CommVar, 0, sizeof(stUartCom_t));

	puart1CommVar->pRecBuffer = uart1_rec_buffer;

    puart1CommVar->SetBaudRate = uart1_set_baudrate;
    puart1CommVar->SendBytes = uart1_send_bytes_A;
    puart1CommVar->SendString = uart1_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    puart1CommVar->SendPrintf = USER_printf;
#endif
}

#if 0
void MstVarInit(void)
{
	xSemaphore_MstSnd = xSemaphoreCreateMutex();
	//MstSendString(MST_STRING_CMD_VERSION_QUERY);
}
#endif

void UartVarTimeout(void)
{
	pstUartCom_t		pUartCommVar = &g_stUartCommVar;

	if(pUartCommVar->u32RecTimeout > 0)
	{
		pUartCommVar->u32RecTimeout --;
		if(pUartCommVar->u32RecTimeout == 0)
		{
			if(pUartCommVar->bTimeOutValid)
			{
				{
					pUartCommVar->bRecCmdFlg = TRUE;
				}
			}
			else
			{
				if(!pUartCommVar->bRecCmdFlg)
				{
					memset(pUartCommVar->pRecBuffer, 0, SIZE_UART_RECEIVE_CMD);
					pUartCommVar->u16RecCnt = 0;
				}
			}
		}
	}
}

void Uart1VarTimeOut(void)
{
	pstUartCom_t		pGuiCommVar = &g_stUart1CommVar;

	if(pGuiCommVar->u32RecTimeout > 0)
	{
		pGuiCommVar->u32RecTimeout --;
		if(pGuiCommVar->u32RecTimeout == 0)
		{
			if(pGuiCommVar->bTimeOutValid)
			{
				{
					pGuiCommVar->bRecCmdFlg = TRUE;
				}
			}
			else
			{
				if(!pGuiCommVar->bRecCmdFlg)
				{
					memset(pGuiCommVar->pRecBuffer, 0, SIZE_UART_RECEIVE_CMD);
					pGuiCommVar->u16RecCnt = 0;
				}
			}
		}
	}
}

void UartCommVarClear(pstUartCom_t pCommVar)
{
	pCommVar->bTimeOutValid = FALSE;
	pCommVar->u32RecTimeout = 0;
	pCommVar->u16RecCnt = 0;
	memset(pCommVar->pRecBuffer, 0, SIZE_UART_RECEIVE_CMD);
	pCommVar->bRecCmdFlg = FALSE;
}

void uart2_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1], send_buff, length);
    UNUSED(bFunSelect);
}

void uart2_send_string_A(char *s, bool bFunSelect)
{
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[1],s);
    UNUSED(bFunSelect);
}

void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0], send_buff, length);
    UNUSED(bFunSelect);
}

void uart1_send_string_A(char *s, bool bFunSelect)
{
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[0],s);
    UNUSED(bFunSelect);
}

int USER_printf(const char *fmt, ...)
{
	int 				iret;
    char 				szPrint[256];
    va_list 			ap;

    //if(!g_bEnableRs232FeedBackFlag)
    //    return (0);
    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, 256, fmt, ap);
    va_end(ap);

	uart2_send_bytes_A((uint8_t *)szPrint, iret,FALSE);

    return iret;
}

void uart2_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
    USARTx_set_baudrate(USART2,
                USART_BaudRate,
                USART_WordLength,
                USART_StopBits,
                USART_Parity,
                USART_HardwareFlowControl,
                USART_Mode);
}

void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
    USARTx_set_baudrate(USART1,
                USART_BaudRate,
                USART_WordLength,
                USART_StopBits,
                USART_Parity,
                USART_HardwareFlowControl,
                USART_Mode);
}

//========================================================
//函数名称: mdrv_USART_SendByte
//输入参数: USARTx:输出串口位置；Data:发送的数据
//输出参数:
//功能说明: 串口数据发送
//备    注:
//========================================================
#if (defined(N32G45X))
void mdrv_USART_SendByte(USART_Module* USARTx, u16 Data)
{
    USART_SendData(USARTx, Data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXC) == RESET)
    {}
}
#else
void mdrv_USART_SendByte(USART_TypeDef* USARTx, u16 Data)
{
    USART_SendData(USARTx, Data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
    {}

    #if 0
    while (!(USARTx->SR & USART_FLAG_TXE));
    USARTx->DR = (Data & (uint16_t)0x01FF);
    #endif
}
#endif


