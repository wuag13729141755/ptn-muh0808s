#define __UART_RESPONSE_C__
#include "includes.h"

#define TIME_UART_RECEIVE_CMD_TIMEOUT_VALID			10//unit is 1ms,
#define TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID		100//unit is 1ms,

#define SIZE_UART_RECEIVE_CMD						1200

#if (_ENABLE_UART_1 == _MACRO_ENABLE)
stUartCom_t			g_stUart1CommVar;
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
stUartCom_t			g_stUart2CommVar;
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
stUartCom_t			g_stUart3CommVar;
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
stUartCom_t			g_stUart4CommVar;
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
stUartCom_t			g_stUart5CommVar;
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
stUartCom_t			g_stUart6CommVar;
#endif

#if (_ENABLE_UART_1 == _MACRO_ENABLE)
char 				uart1_rec_buffer[SIZE_UART_RECEIVE_CMD];
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
char 				uart2_rec_buffer[SIZE_UART_RECEIVE_CMD];
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
char 				uart3_rec_buffer[SIZE_UART_RECEIVE_CMD];
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
char 				uart4_rec_buffer[SIZE_UART_RECEIVE_CMD];
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
char 				uart5_rec_buffer[SIZE_UART_RECEIVE_CMD];
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
char 				uart6_rec_buffer[SIZE_UART_RECEIVE_CMD];
#endif

#if (!_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
#if (_ENABLE_UART_1 == _MACRO_ENABLE)
SemaphoreHandle_t	xSemaphore_Uart1Snd;
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
SemaphoreHandle_t	xSemaphore_Uart2Snd;
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
SemaphoreHandle_t	xSemaphore_Uart3Snd;
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
SemaphoreHandle_t	xSemaphore_Uart4Snd;
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
SemaphoreHandle_t	xSemaphore_Uart5Snd;
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
SemaphoreHandle_t	xSemaphore_Uart6Snd;
#endif
#endif

void UartCommVarClear(pstUartCom_t pCommVar);
int USER_printf(const char *fmt, ...);

#if (_ENABLE_UART_1 == _MACRO_ENABLE)
/////////////////////////////////////UART1//////////////////////////////////////////
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
    {
        pUartCommVar->bRecCmdFlg = FALSE;
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_VALID);
    }
}

void Uart1ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart1CommVar;

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        mapp_UartUserEdidReceive(pUartCommVar);
    }
    else
#endif
    if(pUartCommVar->bRecCmdFlg)
    {
        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
        {
            mapp_UsartSystemCmdFun(pUartCommVar);
        }

        UartCommVarClear(pUartCommVar);
    }
    else if(pUartCommVar->bIsMultiCmdDetect)
    {
        mapp_MultiCmdMatchFunction(pUartCommVar);

        UartCommVarClear(pUartCommVar);
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
    puart1CommVar->SendPrintf = USER_printf;
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
    Uart1_xSemaphoreTask();
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum1], send_buff, length);
    Uart1_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart1_send_string_A(char *s, bool bFunSelect)
{
    Uart1_xSemaphoreTask();
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum1],s);
    Uart1_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart1_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
#if (defined(GD32F4XX))
    USARTx_set_baudrate(USART0,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#else
    USARTx_set_baudrate(USART1,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#endif
}
#endif

#if (_ENABLE_UART_2 == _MACRO_ENABLE)
/////////////////////////////////////UART2//////////////////////////////////////////
void Uart2ProtocalDataReceive(uint8_t u8RecChar)
{
	pstUartCom_t pUartCommVar = &g_stUart2CommVar;

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

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        mapp_UartUserEdidReceive(pUartCommVar);
    }
    else
#endif
    if(pUartCommVar->bRecCmdFlg)
    {
        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
        {
            mapp_UsartSystemCmdFun(pUartCommVar);
        }

        UartCommVarClear(pUartCommVar);
    }
    else if(pUartCommVar->bIsMultiCmdDetect)
    {
        mapp_MultiCmdMatchFunction(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }

#if _ENABLE_SYSTEM_RESET_DELAY_TIME
    if(mapp_GetSysResetMode()&&mapp_CheckAllDataHasSave())
    {
        SYS_SoftReset();
    }
#endif

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(PlatformTimerGet(etUserEdidReceiveTime) == 0&&StartEdidCostumFlag)
    {
        StartEdidCostumFlag = FALSE;
        pUartCommVar->SendPrintf("edid update time out\r\n");
        pUartCommVar->SendPrintf("ok\r\n");
    }
#endif

    mapp_FirmwareUpdateTimeOutFunction();
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
    pUartCommVar->SendPrintf = USER_printf;
#endif

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    StartEdidCostumFlag = FALSE;
    g_u8UserEDIDIndex = 0;
#endif
}

void Uart2VarTimeOut(void)
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
    Uart2_xSemaphoreTask();
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum2], send_buff, length);
    Uart2_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart2_send_string_A(char *s, bool bFunSelect)
{
    Uart2_xSemaphoreTask();
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum2],s);
    Uart2_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart2_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
#if (defined(GD32F4XX))
    USARTx_set_baudrate(USART1,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#else
    USARTx_set_baudrate(USART2,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#endif
}
#endif

#if (_ENABLE_UART_3 == _MACRO_ENABLE)
/////////////////////////////////////UART3//////////////////////////////////////////
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

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        mapp_UartUserEdidReceive(pUartCommVar);
    }
    else
#endif
    if(pUartCommVar->bRecCmdFlg)
    {
        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
        {
            mapp_UsartSystemCmdFun(pUartCommVar);
        }

        UartCommVarClear(pUartCommVar);
    }
    else if(pUartCommVar->bIsMultiCmdDetect)
    {
        mapp_MultiCmdMatchFunction(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
}

void Uart3VarInit(void)
{
	pstUartCom_t 	puart1CommVar = &g_stUart3CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart3_xSemaphoreInit();
    #else
	xSemaphore_Uart3Snd = xSemaphoreCreateMutex();
    #endif
	memset(puart1CommVar, 0, sizeof(stUartCom_t));

	puart1CommVar->pRecBuffer = uart3_rec_buffer;

    puart1CommVar->SetBaudRate = uart3_set_baudrate;
    puart1CommVar->SendBytes = uart3_send_bytes_A;
    puart1CommVar->SendString = uart3_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    puart1CommVar->SendPrintf = Uart3_printf;
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

void uart3_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Uart3_xSemaphoreTask();
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum3], send_buff, length);
    Uart3_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart3_send_string_A(char *s, bool bFunSelect)
{
    Uart3_xSemaphoreTask();
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum3],s);
    Uart3_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart3_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
#if (defined(GD32F4XX))
    USARTx_set_baudrate(USART2,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#else
    USARTx_set_baudrate(USART3,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#endif
}
#endif

#if (_ENABLE_UART_4 == _MACRO_ENABLE)
/////////////////////////////////////UART4//////////////////////////////////////////
void Uart4ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pUartCommVar = &g_stUart4CommVar;

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
    {
        pUartCommVar->bRecCmdFlg = FALSE;
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID);
    }
}

void Uart4ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart4CommVar;

    #if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        mapp_UartUserEdidReceive(pUartCommVar);
    }
    else
    #endif
    if(pUartCommVar->bRecCmdFlg)
    {
//        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
            mapp_Usart4SystemCmdFun(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
#if 0
    else if(pUartCommVar->bIsMultiCmdDetect)
    {
        mapp_MultiCmdMatchFunction(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
#endif
}

void Uart4VarInit(void)
{
	pstUartCom_t 	puart1CommVar = &g_stUart4CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart4_xSemaphoreInit();
    #else
	xSemaphore_Uart4Snd = xSemaphoreCreateMutex();
    #endif
	memset(puart1CommVar, 0, sizeof(stUartCom_t));

	puart1CommVar->pRecBuffer = uart4_rec_buffer;

    puart1CommVar->SetBaudRate = uart4_set_baudrate;
    puart1CommVar->SendBytes = uart4_send_bytes_A;
    puart1CommVar->SendString = uart4_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    puart1CommVar->SendPrintf = Uart4_printf;
#endif
}

void Uart4VarTimeOut(void)
{
	pstUartCom_t		pGuiCommVar = &g_stUart4CommVar;

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

void uart4_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Uart4_xSemaphoreTask();
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum4], send_buff, length);
    Uart4_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart4_send_string_A(char *s, bool bFunSelect)
{
    Uart4_xSemaphoreTask();
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum4],s);
    Uart4_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart4_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
#if (defined(GD32F4XX))
    USARTx_set_baudrate(UART3,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);

#else
    USARTx_set_baudrate(UART4,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#endif
}
#endif

#if (_ENABLE_UART_5 == _MACRO_ENABLE)
/////////////////////////////////////UART5//////////////////////////////////////////
void Uart5ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pUartCommVar = &g_stUart5CommVar;

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
    {
        pUartCommVar->bRecCmdFlg = FALSE;
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = (TIME_UART_RECEIVE_CMD_TIMEOUT_VALID);
    }
}

void Uart5ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart5CommVar;

#if _ENABLE_USER_EDID_LEARNING_FUNCTION
    if(StartEdidCostumFlag)
    {
        mapp_UartUserEdidReceive(pUartCommVar);
    }
    else
#endif
    if(pUartCommVar->bRecCmdFlg)
    {
        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
            mapp_UsartSystemCmdFun(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
    else if(pUartCommVar->bIsMultiCmdDetect)
    {
        mapp_MultiCmdMatchFunction(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
//    mapp_IMX_25_2VCTAPMainFunction();
//    mapp_FirmwareUpdateTimeOutFunction();

}

void Uart5VarInit(void)
{
	pstUartCom_t 	puart1CommVar = &g_stUart5CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart5_xSemaphoreInit();
    #else
	xSemaphore_Uart5Snd = xSemaphoreCreateMutex();
    #endif
	memset(puart1CommVar, 0, sizeof(stUartCom_t));

	puart1CommVar->pRecBuffer = uart5_rec_buffer;

    puart1CommVar->SetBaudRate = uart5_set_baudrate;
    puart1CommVar->SendBytes = uart5_send_bytes_A;
    puart1CommVar->SendString = uart5_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    puart1CommVar->SendPrintf = Uart5_printf;
#endif
}

void Uart5VarTimeOut(void)
{
	pstUartCom_t		pGuiCommVar = &g_stUart5CommVar;

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

void uart5_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Uart5_xSemaphoreTask();
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum5], send_buff, length);
    Uart5_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart5_send_string_A(char *s, bool bFunSelect)
{
    Uart5_xSemaphoreTask();
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum5],s);
    Uart5_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart5_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
#if (defined(GD32F4XX))
    USARTx_set_baudrate(UART4,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);

#else
    USARTx_set_baudrate(UART5,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#endif
}
#endif

#if (_ENABLE_UART_6 == _MACRO_ENABLE)
/////////////////////////////////////UART6//////////////////////////////////////////
void Uart6ProtocalDataReceive(uint8_t u8RecChar)
{
    pstUartCom_t pUartCommVar = &g_stUart6CommVar;

    //if(pUartCommVar->bRecCmdFlg) return;

    if(pUartCommVar->u16RecCnt < (SIZE_UART_RECEIVE_CMD-1))
    {
        pUartCommVar->pRecBuffer[pUartCommVar->u16RecCnt++] = u8RecChar;
    }

    {
        pUartCommVar->bTimeOutValid = TRUE;
        pUartCommVar->u32RecTimeout = TIME_UART_RECEIVE_CMD_TIMEOUT_INVALID;
    }
}

void Uart6ParserMethod(void)
{
    //add your response to uart message code
    //this function will be call in main loop
	pstUartCom_t 			pUartCommVar = &g_stUart6CommVar;

    if(pUartCommVar->bRecCmdFlg)
    {
        if(!mapp_FirmwareUpdateProcess(pUartCommVar))
            mapp_UsartSystemCmdFun(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
    else if(pUartCommVar->bIsMultiCmdDetect)
    {
        mapp_MultiCmdMatchFunction(pUartCommVar);

        UartCommVarClear(pUartCommVar);
    }
}

void Uart6VarInit(void)
{
	pstUartCom_t 	puart1CommVar = &g_stUart6CommVar;

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart6_xSemaphoreInit();
    #else
	xSemaphore_Uart6Snd = xSemaphoreCreateMutex();
    #endif
	memset(puart1CommVar, 0, sizeof(stUartCom_t));

	puart1CommVar->pRecBuffer = uart5_rec_buffer;

    puart1CommVar->SetBaudRate = uart6_set_baudrate;
    puart1CommVar->SendBytes = uart6_send_bytes_A;
    puart1CommVar->SendString = uart6_send_string_A;
#if (_ENABLE_USER_PRINTF_FUNCTION==_MACRO_ENABLE)
    puart1CommVar->SendPrintf = Uart6_printf;
#endif
}

void Uart6VarTimeOut(void)
{
	pstUartCom_t		pGuiCommVar = &g_stUart6CommVar;

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

void uart6_send_bytes_A(uint8_t *send_buff, uint16_t length, bool bFunSelect)
{
    Uart6_xSemaphoreTask();
	USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum6], send_buff, length);
    Uart6_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart6_send_string_A(char *s, bool bFunSelect)
{
    Uart6_xSemaphoreTask();
    USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[eUartNum6],s);
    Uart6_xSemaphoreGive();
    UNUSED(bFunSelect);
}

void uart6_set_baudrate(uint32_t USART_BaudRate,uint16_t USART_WordLength,uint16_t USART_StopBits,uint16_t USART_Parity,uint16_t USART_Mode,uint16_t USART_HardwareFlowControl)
{
#if (defined(GD32F4XX))
    USARTx_set_baudrate(USART5,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);

#else
    USARTx_set_baudrate(USART6,
            USART_BaudRate,
            USART_WordLength,
            USART_StopBits,
            USART_Parity,
            USART_Mode,
            USART_HardwareFlowControl);
#endif
}
#endif

///////////////////////////////////////////////////////////////////////////////////
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
    pstProjectSpecVar_t         pstSpecVar = &stMUH1616S_INPUTSpecVar;
    pstBasicSpecVar_t           pstBasicVar = &pstSpecVar->stBasSpecVar;

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, 256, fmt, ap);
    va_end(ap);

    if(pstBasicVar->u8UartMode==0)
        uart1_send_bytes_A((uint8_t *)szPrint, iret,FALSE);
    uart2_send_bytes_A((uint8_t *)szPrint, iret,FALSE);
    if(g_bEnableDbgFlag)
    {
        uart3_send_bytes_A((uint8_t *)szPrint, iret,FALSE);
    }
    return iret;
}

//========================================================
//函数名称: mdrv_USART_SendByte
//输入参数: USARTx:输出串口位置；Data:发送的数据
//输出参数:
//功能说明: 串口数据发送
//备    注:
//========================================================
#if (defined(GD32F4XX))
void mdrv_USART_SendByte(uint32_t USARTx, u16 Data)
{
    usart_data_transmit(USARTx, Data);
    while(RESET == usart_flag_get(USARTx, USART_FLAG_TC));
}
#else
void mdrv_USART_SendByte(USART_TypeDef* USARTx, u16 Data)
{
    USART_SendData(USARTx, Data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
    {}
}
#endif


