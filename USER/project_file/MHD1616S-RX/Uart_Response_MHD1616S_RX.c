#define __UART_RESPONSE_C__
#include "includes.h"

#define TIME_UART_RECEIVE_CMD_TIMEOUT_VALID			20//unit is 1ms, total is 30ms

#define SIZE_UART_RECEIVE_CMD						1200//300

stUartCom_t			g_stUart2CommVar, g_stUart1CommVar,g_stUart3CommVar;
char 				uart2_rec_buffer[SIZE_UART_RECEIVE_CMD];
char 				uart1_rec_buffer[SIZE_UART_RECEIVE_CMD];
char 				uart3_rec_buffer[SIZE_UART_RECEIVE_CMD];

#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
SemaphoreHandle_t	xSemaphore_Uart1Snd;
SemaphoreHandle_t	xSemaphore_Uart2Snd;
SemaphoreHandle_t	xSemaphore_Uart3Snd;
#endif
//QueueHandle_t 		xQueue_EdidUpdate = NULL;

void UartCommVarClear(pstUartCom_t pCommVar);

void Uart1ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pUartCommVar = &g_stUart1CommVar;

    if(pUartCommVar->u16RecCnt < (SIZE_UART_RECEIVE_CMD-1))
    {
        pUartCommVar->pRecBuffer[pUartCommVar->u16RecCnt++] = u8RecChar;
    }

    {
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = TIME_UART_RECEIVE_CMD_TIMEOUT_VALID;
    }
}

void Uart1ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart1CommVar;

    if(pUartCommVar->bRecCmdFlg)
    {
        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
            mapp_UsartSystemCmdFun(pUartCommVar);
        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart1_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }
    mapp_FirmwareUpdateTimeOutFunction();
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
    puart1CommVar->SendPrintf = Uart1_printf;
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
void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0], send_buff, length);
    UNUSED(bFunSelect);
}

void uart1_send_string_A(char *s, bool bFunSelect)
{
    //Uart1SendStrings(s);
    UNUSED(bFunSelect);
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

void Uart2ProtocalDataReceive(uint8_t u8RecChar)
{
	pstUartCom_t pUartCommVar = &g_stUart2CommVar;

	//if(pUartCommVar->bRecCmdFlg) return;

	if(pUartCommVar->u16RecCnt < (SIZE_UART_RECEIVE_CMD-1))
	{
		pUartCommVar->pRecBuffer[pUartCommVar->u16RecCnt++] = u8RecChar;
	}

	{
		pUartCommVar->bTimeOutValid = TRUE;
		pUartCommVar->u32RecTimeout = TIME_UART_RECEIVE_CMD_TIMEOUT_VALID;
	}
}

void Uart2ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart2CommVar;

    if(pUartCommVar->bRecCmdFlg)
    {
        mapp_Usart2SystemCmdFun(pUartCommVar);

        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart2_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }

#if _ENABLE_SYSTEM_RESET_DELAY_TIME
    if(mapp_GetSysResetMode()&&mapp_CheckAllDataHasSave())
    {
        SYS_SoftReset();
    }
#endif
}

void Uart2VarInit(void)
{
	pstUartCom_t 	pUartCommVar = &g_stUart2CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart2_xSemaphoreInit();
    #else
	xSemaphore_Uart2Snd = xSemaphoreCreateMutex();
    #endif

	memset(pUartCommVar, 0, sizeof(stUartCom_t));

	pUartCommVar->pRecBuffer = uart2_rec_buffer;

	pUartCommVar->SetBaudRate = uart2_set_baudrate;
	pUartCommVar->SendBytes = uart2_send_bytes_A;
	pUartCommVar->SendString = uart2_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    pUartCommVar->SendPrintf = Uart2_printf;
#endif

}


void Uart2VarTimeout(void)
{
	pstUartCom_t		pUartCommVar = &g_stUart2CommVar;

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

void uart2_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1], send_buff, length);
    UNUSED(bFunSelect);
}

void uart2_send_string_A(char *s, bool bFunSelect)
{
    //Rs232SendString(s);
    UNUSED(bFunSelect);
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

void Uart3ProtocalDataReceive(uint8_t u8RecChar)
{
	pstUartCom_t pUartCommVar = &g_stUart3CommVar;


	if(pUartCommVar->u16RecCnt < (SIZE_UART_RECEIVE_CMD-1))
	{
		pUartCommVar->pRecBuffer[pUartCommVar->u16RecCnt++] = u8RecChar;
	}

	{
		pUartCommVar->bTimeOutValid = TRUE;
		pUartCommVar->u32RecTimeout = TIME_UART_RECEIVE_CMD_TIMEOUT_VALID;
	}
}

void Uart3ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart3CommVar;

    if(pUartCommVar->bRecCmdFlg)
    {

        mapp_UsartSystemCmdFun(pUartCommVar);

        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart3_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }
}

void Uart3VarInit(void)
{
	pstUartCom_t 	pUartCommVar = &g_stUart3CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart3_xSemaphoreInit();
    #else
	xSemaphore_Uart3Snd = xSemaphoreCreateMutex();
    #endif

	memset(pUartCommVar, 0, sizeof(stUartCom_t));

	pUartCommVar->pRecBuffer = uart3_rec_buffer;

	pUartCommVar->SetBaudRate = uart3_set_baudrate;
	pUartCommVar->SendBytes = uart3_send_bytes_A;
	pUartCommVar->SendString = uart3_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    pUartCommVar->SendPrintf = Uart3_printf;
#endif

}

void Uart3VarTimeout(void)
{
	pstUartCom_t		pUartCommVar = &g_stUart3CommVar;

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

void uart3_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[2], send_buff, length);
    UNUSED(bFunSelect);
}

void uart3_send_string_A(char *s, bool bFunSelect)
{
    UNUSED(bFunSelect);
}

void uart3_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
    USARTx_set_baudrate(USART3,
                USART_BaudRate,
                USART_WordLength,
                USART_StopBits,
                USART_Parity,
                USART_HardwareFlowControl,
                USART_Mode);
}

void UartCommVarClear(pstUartCom_t pCommVar)
{
	pCommVar->bTimeOutValid = FALSE;
	pCommVar->u32RecTimeout = 0;
	pCommVar->u16RecCnt = 0;
	memset(pCommVar->pRecBuffer, 0, SIZE_UART_RECEIVE_CMD);
	pCommVar->bRecCmdFlg = FALSE;
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
}
#endif


