#include "includes.h"

extern const stUartDeclare_t stProjectUartWorkTable[];

#if _SYSTEM_SUPPORT_RTOS
void vTaskUart(void *pvParameters)
{
	pstUartDeclare_t	pstUart, pstUartOrg = (pstUartDeclare_t)&stProjectUartWorkTable[0];

	(void)pvParameters;

	for(;;)
	{
		pstUart = pstUartOrg;

		while(pstUart->u8UartPort != _UART_PORT_NONE)
		{
			if(pstUart->funUartParser != NULL)
			{
				pstUart->funUartParser();
			}
			pstUart ++;
		}

		vTaskDelay(10);
	}
}
#else
void vTaskUart(void *pvParameters)
{
	pstUartDeclare_t	pstUart, pstUartOrg = (pstUartDeclare_t)&stProjectUartWorkTable[0];

	(void)pvParameters;

	pstUart = pstUartOrg;

	while(pstUart->u8UartPort != _UART_PORT_NONE)
	{
		if(pstUart->funUartParser != NULL)
		{
			pstUart->funUartParser();
		}
		pstUart ++;
	}
}

#endif

void drv_UartTaskInit(void)
{
	pstUartDeclare_t 	pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

	while(pstUart->u8UartPort != _UART_PORT_NONE)
	{
		if(pstUart->funUartVarInit != NULL)
		{
			pstUart->funUartVarInit();
		}
		USARTx_Configuration(pstUart);

		pstUart ++;
	}
}










