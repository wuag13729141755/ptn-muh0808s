#ifndef __UART_RESPONSE_USER_FUNCTION_H__
#define __UART_RESPONSE_USER_FUNCTION_H__
#include "usart.h"

#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
#define DISPLAY_ON_TEXT             "#SET_ON_"
#define DISPLAY_OFF_TEXT            "#SET_OF_"
#define DISPLAY_HEX_ON_TEXT         "#SET_H_ON_"
#define DISPLAY_HEX_OFF_TEXT        "#SET_H_OF_"
#if _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
#define USER_CMD_SEND_TEXT          "#SEND_A_"
#define USER_CMD_SEND_HEX_TEXT      "#SEND_H_"
#endif

#define USER_CMD_DISPLAY_ON(buffer)         (strncmp(buffer,DISPLAY_ON_TEXT,strlen(DISPLAY_ON_TEXT))==0)
#define USER_CMD_DISPLAY_OFF(buffer)        (strncmp(buffer,DISPLAY_OFF_TEXT,strlen(DISPLAY_OFF_TEXT))==0)
#define USER_HEX_CMD_DISPLAY_ON(buffer)     (strncmp(buffer,DISPLAY_HEX_ON_TEXT,strlen(DISPLAY_HEX_ON_TEXT))==0)
#define USER_HEX_CMD_DISPLAY_OFF(buffer)    (strncmp(buffer,DISPLAY_HEX_OFF_TEXT,strlen(DISPLAY_HEX_OFF_TEXT))==0)
#if _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
#define USER_CMD_SEND(buffer)               (strncmp(buffer,USER_CMD_SEND_TEXT,strlen(USER_CMD_SEND_TEXT))==0)
#define USER_HEX_CMD_SEND(buffer)           (strncmp(buffer,USER_CMD_SEND_HEX_TEXT,strlen(USER_CMD_SEND_HEX_TEXT))==0)
#endif

#if _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
#define USER_CMD_HEAD(buffer)     (USER_CMD_DISPLAY_ON(buffer)\
                                    ||USER_CMD_DISPLAY_OFF(buffer)\
                                    ||USER_HEX_CMD_DISPLAY_ON(buffer)\
                                    ||USER_HEX_CMD_DISPLAY_OFF(buffer)\
                                    ||USER_CMD_SEND(buffer)\
                                    ||USER_HEX_CMD_SEND(buffer))
#else
#define USER_CMD_HEAD(buffer)     (USER_CMD_DISPLAY_ON(buffer)\
                                    ||USER_CMD_DISPLAY_OFF(buffer)\
                                    ||USER_HEX_CMD_DISPLAY_ON(buffer)\
                                    ||USER_HEX_CMD_DISPLAY_OFF(buffer))
#endif
#endif

#if (_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
#define Rs232MsgSnd(x)          Uart2_printf x
#define Uart1MsgSnd(x)			Uart1_printf x
#define Uart3MsgSnd(x)			Uart3_printf x
#else
#define Rs232MsgSnd(x)			xSemaphoreTake(xSemaphore_Rs232Snd, portMAX_DELAY); \
								Uart2_printf x; \
								xSemaphoreGive(xSemaphore_Rs232Snd)
#define Uart1MsgSnd(x)			xSemaphoreTake(xSemaphore_Uart1Snd, portMAX_DELAY); \
								Uart1_printf x; \
								xSemaphoreGive(xSemaphore_Uart1Snd)
#define Uart3MsgSnd(x)			xSemaphoreTake(xSemaphore_Uart3Snd, portMAX_DELAY); \
								Uart3_printf x; \
								xSemaphoreGive(xSemaphore_Uart3Snd)
#endif

#if (_ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT)
#define Rs232SendBytes(x, y)	Uart2_xSemaphoreTask();\
                                USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1], x, y);\
                                Uart2_xSemaphoreGive()
#define Uart1SendBytes(x, y)	Uart1_xSemaphoreTask();\
                                USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0], x, y);\
                                Uart1_xSemaphoreGive()
#define Uart3SendBytes(x, y)	Uart3_xSemaphoreTask();\
                                USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[2], x, y);\
                                Uart3_xSemaphoreGive()

#define Rs232SendString(x)		Uart2_xSemaphoreTask();\
                                USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[1], x);\
                                Uart2_xSemaphoreGive()
#define Uart1SendStrings(x)		Uart1_xSemaphoreTask();\
                                USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[0], x);\
                                Uart1_xSemaphoreGive()
#define Uart3SendStrings(x)		Uart3_xSemaphoreTask();\
                                USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[2], x);\
                                Uart3_xSemaphoreGive()
#else

#define Rs232SendBytes(x, y)	xSemaphoreTake(xSemaphore_Rs232Snd, portMAX_DELAY); \
								USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[1], x, y); \
								xSemaphoreGive(xSemaphore_Rs232Snd)
#define Uart1SendBytes(x, y)	xSemaphoreTake(xSemaphore_Uart1Snd, portMAX_DELAY); \
								USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[0], x, y); \
								xSemaphoreGive(xSemaphore_Uart1Snd)
#define Uart3SendBytes(x, y)	xSemaphoreTake(xSemaphore_Uart3Snd, portMAX_DELAY); \
								USARTx_send_bytes((pstUartDeclare_t)&stProjectUartWorkTable[2], x, y); \
								xSemaphoreGive(xSemaphore_Uart3Snd)
#define Rs232SendString(x)		xSemaphoreTake(xSemaphore_Rs232Snd, portMAX_DELAY); \
								USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[1], x); \
								xSemaphoreGive(xSemaphore_Rs232Snd)
#define Uart1SendStrings(x)		xSemaphoreTake(xSemaphore_Uart1Snd, portMAX_DELAY); \
								USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[0], x); \
								xSemaphoreGive(xSemaphore_Uart1Snd)
#define Uart3SendStrings(x)		xSemaphoreTake(xSemaphore_Uart3Snd, portMAX_DELAY); \
								USARTx_send_string((pstUartDeclare_t)&stProjectUartWorkTable[2], x); \
								xSemaphoreGive(xSemaphore_Uart3Snd)
#endif

#define CommResMsgSnd(x)		Rs232MsgSnd(x);Uart1MsgSnd(x)
								//Rs232MsgSnd(x)
#define CommResSendBytes(x, y)	Rs232SendBytes(x, y)//GuiSendBytes(x, y);\
								//Rs232SendBytes(x, y)
extern bool g_bEnableRs232FeedBackFlag;
//#define USER_PRINTF(x)          {if(g_bEnableRs232FeedBackFlag) CommResMsgSnd(x);}
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
extern bool StartEdidCostumFlag;
extern u8 g_u8UserEDIDIndex;
extern bool g_bInternalEdidFlag;
#if 0//_ENABLE_INTERNAL_EDID_LEARNING_FORM_UART
extern u8 g_u8InternalEDIDIndex;
#endif
#endif
extern u8 g_u8IpAddr[4];
extern u8 g_u8NetMask[4];
extern u8 g_u8NetGate[4];
extern u8 g_u8QueryRxPort;
extern u8 g_u8QueryTxPort;

#ifdef __UART_RESPON_USER_FUNC_C__
#define INTERFACE
#else
#define INTERFACE       extern
#endif

INTERFACE void mapp_UsartSystemCmdFun(pstUartCom_t pCommVar);
INTERFACE char GetWinPortText(u8 u8WinPort);

#if 0//_ENABLE_USER_EDID_LEARNING_FUNCTION
INTERFACE void mapp_UartUserEdidReceive(puart_com_t pUartCommVar);
#endif
#if _ENABLE_UART_LEARNING_MESSAGE_FUNCTION
INTERFACE void mapp_UsartUserCmdFun(pstUartCom_t pCommVar);
INTERFACE bool mapp_UserUartLearningMessageFunction(u8 u8Index, pstUartCom_t pCommVar, u8 u8HeadIndex, bool bIsHex);
INTERFACE void UserHexDataSendFunction(u8 u8Port, u32 u32Baud, u16 u16Len, char *pData);
INTERFACE void UserDataSendFunction(u8 u8Port, u32 u32Baud, u16 u16Len, char *pData);
INTERFACE void MApp_SendPowerOnMessage(void);
#if _ENABLE_UART_SEND_USER_MESSAGE_FUNCTION
INTERFACE bool mapp_UartSendUserMessage(pstUartCom_t pCommVar, u8 u8HeadIndex, bool bIsHex);
#endif
#endif
#if _ENABLE_USER_EDID_LEARNING_FUNCTION
INTERFACE void mapp_UartUserEdidReceive(pstUartCom_t pUartCommVar);
#endif
INTERFACE void mapp_SetPowerState(bool bPwrSta,bool bSendCecRs232);
INTERFACE uint32_t mapp_GetRealBaudrate(uint8_t u8BaudrateID);

#undef INTERFACE
#endif

