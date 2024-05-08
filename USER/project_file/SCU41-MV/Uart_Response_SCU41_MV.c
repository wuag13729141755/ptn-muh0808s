#define __UART_RESPONSE_C__
#include "includes.h"

#define TIME_UART_RECEIVE_CMD_TIMEOUT_VALID			50//unit is 1ms, total is 30ms
#define TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID		50//100//unit is 1ms, total is 300ms..

#define SIZE_UART_RECEIVE_CMD						300
#define SIZE_UART_CMD_MIN_LEN						2

stUartCom_t			g_stUartCommVar, g_stUart1CommVar,g_stUart3CommVar;
char 				uart_rec_buffer[SIZE_UART_RECEIVE_CMD];
char 				uart1_rec_buffer[SIZE_UART_RECEIVE_CMD];
char 				uart3_rec_buffer[SIZE_UART_RECEIVE_CMD];
stEdidUpgrData_t	g_stEdidUpgr;
//bool				g_bTimeoutGuiEdidw = FALSE, g_bTimeoutRs232Edidw = FALSE;
//bool				bGuiIpPrintfFlag = FALSE;
//volatile bool 		g_vbIsWaitingForEdidData = FALSE;

#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
SemaphoreHandle_t	xSemaphore_Rs232Snd;
SemaphoreHandle_t	xSemaphore_Uart1Snd;
SemaphoreHandle_t	xSemaphore_Uart3Snd;
#endif
//QueueHandle_t 		xQueue_EdidUpdate = NULL;

void UartCommVarClear(pstUartCom_t pCommVar);

//===============================UART2=========================================
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
    if((pUartCommVar->pRecBuffer[0]=='#')||(pUartCommVar->pRecBuffer[0]=='>')) // head char '#'
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

void UartParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUartCommVar;

    #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
    MApp_SendPowerOnMessage();
    #endif

    if(pUartCommVar->bRecCmdFlg)
    {

        #if _ENABLE_USER_EDID_LEARNING_FUNCTION
        if(StartEdidCostumFlag)
        {
            mapp_UartUserEdidReceive(pUartCommVar);
        }
        else
        #endif
        #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(USER_CMD_HEAD(pUartCommVar->pRecBuffer))  //head "/-" for user hex
        {
            mapp_UsartUserCmdFun(pUartCommVar);
        }
        else
        #endif
        {
            mapp_UsartSystemCmdFun(pUartCommVar);
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
    if(mapp_GetSysResetMode())
    {
        SYS_SoftReset();
    }
#endif
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
	//memset(&g_stEdidUpgr, 0, sizeof(stEdidUpgrData_t));

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
//=============================================================================

//===============================UART1=========================================
// GUI
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
    if((pUartCommVar->pRecBuffer[0]=='#')||(pUartCommVar->pRecBuffer[0]=='>')) // head char '#'
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

void Uart1ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart1CommVar;

    if(pUartCommVar->bRecCmdFlg)
    {
        #if _ENABLE_USER_EDID_LEARNING_FUNCTION
        if(StartEdidCostumFlag)
        {
            mapp_UartUserEdidReceive(pUartCommVar);
        }
        else
        #endif
        #if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
        if(USER_CMD_HEAD(pUartCommVar->pRecBuffer))  //head "/-" for user hex
        {
            mapp_UsartUserCmdFun(pUartCommVar);
        }
        else
        #endif
        {
            mapp_UsartSystemCmdFun(pUartCommVar);
        }

        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart1_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }
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
    puart1CommVar->SendPrintf = USER1_printf;//Uart1_printf;
#endif
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

//=============================================================================

//===============================UART3=========================================
// FPGA
void Uart3ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pUartCommVar = &g_stUart3CommVar;

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
    if((pUartCommVar->pRecBuffer[0]=='#')||(pUartCommVar->pRecBuffer[0]=='>')) // head char '#'
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

void Uart3ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart3CommVar;

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

void Uart3VarInit(void)
{
	pstUartCom_t 	puartCommVar = &g_stUart3CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart3_xSemaphoreInit();
    #else
	xSemaphore_Uart3Snd = xSemaphoreCreateMutex();
    #endif
	memset(puartCommVar, 0, sizeof(stUartCom_t));

	puartCommVar->pRecBuffer = uart3_rec_buffer;

    puartCommVar->SetBaudRate = uart3_set_baudrate;
    puartCommVar->SendBytes = uart3_send_bytes_A;
    puartCommVar->SendString = uart3_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    puartCommVar->SendPrintf = Uart3_printf;
#endif
}

void Uart3VarTimeOut(void)
{
	pstUartCom_t		pGuiCommVar = &g_stUart3CommVar;

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

    if(g_bEnableRs232FeedBackFlag)
    {
	    Rs232SendBytes((uint8_t *)szPrint, iret);
    }
    Uart1SendBytes((uint8_t *)szPrint, iret);
    return iret;
}

void uart2_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Rs232SendBytes(send_buff, length);
    Uart1SendBytes(send_buff, length);
    UNUSED(bFunSelect);
}

void uart2_send_string_A(char *s, bool bFunSelect)
{
    Rs232SendString(s);
    Uart1SendStrings(s);
    UNUSED(bFunSelect);
}

void uart2_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
    USART_InitTypeDef USART_InitStructure;

    /*下面是配置串口2的工作模式即使能*/
    USART_InitStructure.USART_BaudRate            = USART_BaudRate;
    USART_InitStructure.USART_WordLength          = USART_WordLength;
    USART_InitStructure.USART_StopBits            = USART_StopBits;
    USART_InitStructure.USART_Parity              = USART_Parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl;
    USART_InitStructure.USART_Mode                = USART_Mode;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

int USER1_printf(const char *fmt, ...)
{
	int 				iret;
    char 				szPrint[256];
    va_list 			ap;

    //if(!g_bEnableRs232FeedBackFlag)
    //    return (0);
    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, 256, fmt, ap);
    va_end(ap);

    if(g_bEnableRs232FeedBackFlag)
    {
	    Rs232SendBytes((uint8_t *)szPrint, iret);
    }
    Uart1SendBytes((uint8_t *)szPrint, iret);
    return iret;
}

void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Rs232SendBytes(send_buff, length);
    Uart1SendBytes(send_buff, length);
    UNUSED(bFunSelect);
}

void uart1_send_string_A(char *s, bool bFunSelect)
{
    Rs232SendString(s);
    Uart1SendStrings(s);
    UNUSED(bFunSelect);
}

void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
    USART_InitTypeDef USART_InitStructure;

    /*下面是配置串口2的工作模式即使能*/
    USART_InitStructure.USART_BaudRate            = USART_BaudRate;
    USART_InitStructure.USART_WordLength          = USART_WordLength;
    USART_InitStructure.USART_StopBits            = USART_StopBits;
    USART_InitStructure.USART_Parity              = USART_Parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl;
    USART_InitStructure.USART_Mode                = USART_Mode;

    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void uart3_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Uart3SendBytes(send_buff, length);
    UNUSED(bFunSelect);
}

void uart3_send_string_A(char *s, bool bFunSelect)
{
    Uart3SendStrings(s);
    UNUSED(bFunSelect);
}

void uart3_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
    USART_InitTypeDef USART_InitStructure;

    /*下面是配置串口2的工作模式即使能*/
    USART_InitStructure.USART_BaudRate            = USART_BaudRate;
    USART_InitStructure.USART_WordLength          = USART_WordLength;
    USART_InitStructure.USART_StopBits            = USART_StopBits;
    USART_InitStructure.USART_Parity              = USART_Parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl;
    USART_InitStructure.USART_Mode                = USART_Mode;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
}

//========================================================
//函数名称: mdrv_USART_SendByte
//输入参数: USARTx:输出串口位置；Data:发送的数据
//输出参数:
//功能说明: 串口数据发送
//备    注:
//========================================================
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


