#define __UART_RESPONSE_C__
#include "includes.h"

#define TIME_UART_RECEIVE_CMD_TIMEOUT_VALID			30//unit is 1ms, total is 30ms
#define TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID		100//unit is 1ms, total is 300ms..

#define SIZE_UART_RECEIVE_CMD						1024
#define SIZE_UART_CMD_MIN_LEN						2

stUartCom_t			g_stUartCommVar, g_stUart1CommVar;
char 				uart_rec_buffer[SIZE_UART_RECEIVE_CMD];
char 				uart1_rec_buffer[SIZE_UART_RECEIVE_CMD];

#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
SemaphoreHandle_t	xSemaphore_Rs232Snd;
SemaphoreHandle_t	xSemaphore_Uart1Snd;
#endif

void UartCommVarClear(pstUartCom_t pCommVar);

void UART_ReceiveData_IT(pstUartCom_t pstUartCom, uint8_t u8RecChar)
{
    pstUartCom->pRecBuffer[pstUartCom->u16RecCnt] = (u8) u8RecChar;
    {
        pstUartCom->u32RecTimeout = 20;
        pstUartCom->bTimeOutValid = TRUE;
    }

    pstUartCom->u16RecCnt++;
    if(pstUartCom->u16RecCnt>=SIZE_UART_RECEIVE_CMD)
        pstUartCom->u16RecCnt = SIZE_UART_RECEIVE_CMD;
}

void Uart1ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pstUartCom = &g_stUart1CommVar;

    //UART_ReceiveData_IT(pstUartCom,u8RecChar);
    pstUartCom->pRecBuffer[pstUartCom->u16RecCnt] = (u8) u8RecChar;
    {
        pstUartCom->u32RecTimeout = 20;
        pstUartCom->bTimeOutValid = TRUE;
    }

    pstUartCom->u16RecCnt++;
    if(pstUartCom->u16RecCnt>=SIZE_UART_RECEIVE_CMD)
        pstUartCom->u16RecCnt = SIZE_UART_RECEIVE_CMD;
}


void UartProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pstUartCom = &g_stUartCommVar;

    //UART_ReceiveData_IT(pstUartCom,u8RecChar);
    pstUartCom->pRecBuffer[pstUartCom->u16RecCnt] = (u8) u8RecChar;
    {
        pstUartCom->u32RecTimeout = 20;
        pstUartCom->bTimeOutValid = TRUE;
    }

    pstUartCom->u16RecCnt++;
    if(pstUartCom->u16RecCnt>=SIZE_UART_RECEIVE_CMD)
        pstUartCom->u16RecCnt = SIZE_UART_RECEIVE_CMD;
}

void UartParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUartCommVar;

    if(pUartCommVar->bRecCmdFlg)
    {
        {
            mapp_UsartSystemCmdFun(pUartCommVar);
        }

        pUartCommVar->bTimeOutValid = FALSE;
        pUartCommVar->u32RecTimeout = 0;
        pUartCommVar->u16RecCnt = 0;
        memset(uart_rec_buffer, 0, SIZE_UART_RECEIVE_CMD);
        pUartCommVar->bRecCmdFlg = FALSE;
    }

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

void UartVarInit(void)
{
	pstUartCom_t 	pUartCommVar = &g_stUartCommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart2_xSemaphoreInit();
    #else
	xSemaphore_Rs232Snd = xSemaphoreCreateMutex();
    #endif
	memset(pUartCommVar, 0, sizeof(stUartCom_t));
	pUartCommVar->pRecBuffer = uart_rec_buffer;

	pUartCommVar->SetBaudRate = uart2_set_baudrate;
	pUartCommVar->SendBytes = uart2_send_bytes_A;
	pUartCommVar->SendString = uart2_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    pUartCommVar->SendPrintf = USER_printf;
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
    puart1CommVar->SendPrintf = Uart1_printf;
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
    USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1],(uint8_t *)send_buff, length);
    UNUSED(bFunSelect);
}

void uart2_send_string_A(char *s, bool bFunSelect)
{
    while(*s)
    {
        USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1],(uint8_t *)s, 1);
        s++;
    }
    UNUSED(bFunSelect);
}

void uart1_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0],(uint8_t *)send_buff, length);
    UNUSED(bFunSelect);
}

void uart1_send_string_A(char *s, bool bFunSelect)
{
    while(*s)
    {
        USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0],(uint8_t *)s, 1);
        s++;
    }
    UNUSED(bFunSelect);
}

int USER_printf(const char *fmt, ...)
{
	int 				iret;
    char 				szPrint[256];
    va_list 			ap;

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, 256, fmt, ap);
    va_end(ap);

    USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0],(uint8_t *)szPrint, iret);
    USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1],(uint8_t *)szPrint, iret);

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


