#ifndef __PRINTF_H_
#define __PRINTF_H_

extern bool g_bEnableDbgFlag;

int Uart_vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
int Uart_snprintf(char *str, size_t size, const char *fmt, ...);
#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
void Uart1_xSemaphoreInit(void);
void Uart1_xSemaphoreTask(void);
void Uart1_xSemaphoreGive(void);
void Uart2_xSemaphoreInit(void);
void Uart2_xSemaphoreTask(void);
void Uart2_xSemaphoreGive(void);
void Uart3_xSemaphoreInit(void);
void Uart3_xSemaphoreTask(void);
void Uart3_xSemaphoreGive(void);
#if _ENABLE_UART_4
void Uart4_xSemaphoreInit(void);
void Uart4_xSemaphoreTask(void);
void Uart4_xSemaphoreGive(void);
#endif

#if _ENABLE_UART_5
void Uart5_xSemaphoreInit(void);
void Uart5_xSemaphoreTask(void);
void Uart5_xSemaphoreGive(void);
#endif

#if _ENABLE_UART_6
void Uart6_xSemaphoreInit(void);
void Uart6_xSemaphoreTask(void);
void Uart6_xSemaphoreGive(void);
#endif

#endif
int Uart_printf(const char *fmt, ...);
int Uart1_printf(const char *fmt, ...);
int Uart2_printf(const char *fmt, ...);
int Uart3_printf(const char *fmt, ...);
#if _ENABLE_UART_4
int Uart4_printf(const char *fmt, ...);
#endif
#if _ENABLE_UART_5
int Uart5_printf(const char *fmt, ...);
#endif
#if _ENABLE_UART_6
int Uart6_printf(const char *fmt, ...);
#endif

int Uartx_printf(pstUartDeclare_t pstUart, const char *fmt, ...);
int dbg_printf(const char *fmt, ...);
int UTL_strncmp( const char *string1, const char *string2, int count );
size_t UTL_strlen(const char *pStr);
int UTL_strcmp(const char *string1, const char *string2);

#endif





































