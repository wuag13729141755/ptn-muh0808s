#include "includes.h"

#ifndef ASSERT
#define ASSERT(expr)    do {} while (0)
#endif

#define COMPATIBILITY_8051      1

#define to_digit(c)     ((c) - '0')
#define is_digit(c)     ((unsigned) to_digit(c) <= 9)

static const char hex16[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

extern const stUartDeclare_t stProjectUartWorkTable[];
/*
 * %[-^][digits][.digits][scd]
 */

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
#if _ENABLE_UART_1
SemaphoreHandle_t   xSemaphore_Uart1Snd;
#endif
#if _ENABLE_UART_2
SemaphoreHandle_t   xSemaphore_Uart2Snd;
#endif
#if _ENABLE_UART_3
SemaphoreHandle_t   xSemaphore_Uart3Snd;
#endif
#if _ENABLE_UART_4
SemaphoreHandle_t   xSemaphore_Uart4Snd;
#endif
#if _ENABLE_UART_5
SemaphoreHandle_t   xSemaphore_Uart5Snd;
#endif
#if _ENABLE_UART_6
SemaphoreHandle_t   xSemaphore_Uart6Snd;
#endif
#endif

bool g_bEnableDbgFlag = TRUE;

#define ZEROPAD     (1 << 0)
#define LADJUST     (1 << 1)
#define CADJUST     (1 << 2)

/*
 * Length
 */
#define LEN_DEFAULT     0
#define LEN_CHAR        1
#define LEN_SHORT       2
#define LEN_LONG        3
#define LEN_LONGLONG    4

#ifndef PRINTF_CHAR_MAX
#define PRINTF_CHAR_MAX     256
#endif

#define NUM(str, n, base)                   \
    do {                                    \
        if (n < base)                       \
            *--str = hex16[n];              \
        else                                \
            do {                            \
                *--s = hex16[n % base];     \
            } while ( (n /= base) );        \
    } while (0)

int Uart_vsnprintf(char *str, size_t size, const char *fmt, va_list ap)
{
    signed int          sInt;
    unsigned int        uInt;
    signed long long    sLongLong;
    unsigned long long  uLongLong;

    int flags, width, prec, length;
    char *s, *p, *dst, *strMax;
    char digit[24];

    ASSERT(str != NULL && fmt != NULL);

    dst = str;
    strMax = str + size;

    while ( (uInt = *fmt++) )
    {

        if (uInt != '%')
        {
            if (dst < strMax)
                *dst++ = uInt;
            continue;
        }

        /* % */

        width = 0;
        length = LEN_DEFAULT;
        flags = 0;
        prec = -1;

    rflag:                /* read flag */
        uInt = *fmt++;

    reswitch:

        switch (uInt)
        {

        case 's':
            s = va_arg(ap, char *);
            if (width != 0 || prec >= 0)
            {
                p = s;
                if (prec < 0)
                    while (*p)
                        p++;
                else
                    while (*p && prec-- > 0)
                        p++;
                prec = p - s;
                break;
            }

            while ( (dst < strMax) && (*dst = *s) )
            {
                s++;
                dst++;
            }
            continue;

        case 'd':
        case 'u':
            p = s = digit + sizeof(digit);
            if (length == LEN_LONGLONG)
            {
                uLongLong = va_arg(ap, unsigned long long);
                if (uInt == 'd')
                {
                    sLongLong = uLongLong;
                    if (sLongLong < 0)
                        uLongLong = -sLongLong;
                    NUM(s, uLongLong, 10);
                    if (sLongLong < 0)
                        *--s = '-';
                }
                else
                {
                    NUM(s, uLongLong, 10);
                }
            }
            else
            {
                if (uInt == 'd')
                {
                    sInt = uInt = va_arg(ap, int);
                    if (sInt < 0)
                        uInt = -sInt;
                    NUM(s, uInt, 10);
                    if (sInt < 0)
                        *--s = '-';
                }
                else
                {
                    uInt = va_arg(ap, int);
                    NUM(s, uInt, 10);
                }
            }

            ASSERT( (p - s) <= (int) sizeof(digit));

            uInt = p - s;
            if (prec < 0 || prec > (int) uInt)
                prec = uInt;
            break;

        case 'p':
        case 'x': case 'X':
            p = s = digit + sizeof(digit);
            if (length == LEN_LONGLONG)
            {
                uLongLong = va_arg(ap, unsigned long long);
                NUM(s, uLongLong, 16);
            }
            else
            {
                uInt = va_arg(ap, int);
                NUM(s, uInt, 16);
            }

            ASSERT( (p - s) <= (int) sizeof(digit));

            uInt = p - s;
            if (prec < 0 || prec > (int) uInt)
                prec = uInt;
            break;

        case 'c':
            *(s = digit) = va_arg(ap, int);
            prec = 1;
            break;

        case '*':  /* must be postive value */
            width = va_arg(ap, int);
            goto rflag;

        case '0':
            flags |= ZEROPAD;
            goto rflag;

        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            width = to_digit(uInt);

            while (is_digit( (uInt = *fmt++) ) )
            {
                /* width = 10 * width + to_digit(uInt); */
                width <<= 1;
                width += (width << 2) + to_digit(uInt);
            }
            goto reswitch;

        case '.':
            uInt = *fmt++;
            if (uInt == '*')
            {
                prec = va_arg(ap, int);
                goto rflag;
            }

            ASSERT(is_digit(uInt));

            if (is_digit(uInt))
            {
                prec = to_digit(uInt);

                while ( is_digit( (uInt = *fmt++) ) )
                    prec = 10 * prec + to_digit(uInt);
            }
            goto reswitch;

        case '-':
            flags |= LADJUST;
            goto rflag;

        case '^':           /* center */
            flags |= CADJUST;
            goto rflag;

    #if COMPATIBILITY_8051
        /* 8051 compatibility */
        case 'b':
        case 'B':
            length = LEN_CHAR;
            goto rflag;

        case 'h':
            length = (length == LEN_SHORT) ? LEN_CHAR : LEN_SHORT;
            goto rflag;

        case 'l':
        case 'L':
            length = (length == LEN_LONG) ? LEN_LONGLONG : LEN_LONG;
            goto rflag;
    #endif
        default:
            ASSERT(uInt == '%');
            if ((dst < strMax) && !(*dst++ = uInt))
                return (dst - str);
            continue;
        } /* switch */


        /* format */
        /* prec now is length of visual part of string s */
        if (width > prec)
        {
            uInt = width - prec;

            if ( !(flags & (LADJUST | CADJUST)) )
            {
                if (uInt > 0)
                {
                    flags = (flags & ZEROPAD) ?  '0' : ' ';
                    do {
                        if (dst < strMax)
                            *dst++ = flags;
                    } while (--uInt > 0);
                }
                while (prec-- > 0)
                {
                    if (dst < strMax)
                        *dst++ = *s++;
                }

            }
            else if (flags & LADJUST)
            {
                while (prec-- > 0 && dst < strMax)
                    *dst++ = *s++;
                while (uInt-- > 0 && dst < strMax)
                    *dst++ = ' ';

            }
            else    /* CADJUST */
            {
                width = uInt / 2;
                uInt -= width;

                while (width-- > 0 && dst < strMax)
                    *dst++ = ' ';
                while (prec-- > 0 && dst < strMax)
                    *dst++ = *s++;
                while (uInt-- > 0 && dst < strMax)
                    *dst++ = ' ';
            }
        }
        else
        {
            while (prec-- > 0 && dst < strMax)
                *dst++ = *s++;
        }
    } /* while */

    *dst = '\0';

    return (dst - str);
}

int Uart_snprintf(char *str, size_t size, const char *fmt, ...)
{
    int ret;
    va_list ap;

    va_start(ap, fmt);
    ret = Uart_vsnprintf(str, size, fmt, ap);
    va_end(ap);

    return ret;
}

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
#if _ENABLE_UART_1
void Uart1_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_Uart1Snd = xSemaphoreCreateMutex();
#endif
}

void Uart1_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_Uart1Snd, portMAX_DELAY);
#endif
}

void Uart1_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_Uart1Snd);
#endif
}
#endif

#if _ENABLE_UART_2
void Uart2_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_Uart2Snd = xSemaphoreCreateMutex();
#endif
}

void Uart2_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_Uart2Snd, portMAX_DELAY);
#endif
}

void Uart2_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_Uart2Snd);
#endif
}
#endif

#if _ENABLE_UART_3
void Uart3_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_Uart3Snd = xSemaphoreCreateMutex();
#endif
}

void Uart3_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_Uart3Snd, portMAX_DELAY);
#endif
}

void Uart3_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_Uart3Snd);
#endif
}
#endif

#if _ENABLE_UART_4
void Uart4_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_Uart4Snd = xSemaphoreCreateMutex();
#endif
}

void Uart4_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_Uart4Snd, portMAX_DELAY);
#endif
}

void Uart4_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_Uart4Snd);
#endif
}
#endif

#if _ENABLE_UART_5
void Uart5_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_Uart5Snd = xSemaphoreCreateMutex();
#endif
}

void Uart5_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_Uart5Snd, portMAX_DELAY);
#endif
}

void Uart5_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_Uart5Snd);
#endif
}
#endif

#if _ENABLE_UART_6
void Uart6_xSemaphoreInit(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphore_Uart6Snd = xSemaphoreCreateMutex();
#endif
}

void Uart6_xSemaphoreTask(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreTake(xSemaphore_Uart6Snd, portMAX_DELAY);
#endif
}

void Uart6_xSemaphoreGive(void)
{
#if _SYSTEM_SUPPORT_RTOS
    xSemaphoreGive(xSemaphore_Uart6Snd);
#endif
}
#endif

#endif

int Uart_printf(const char *fmt, ...)
{
    int                 i, iret;
    char                szPrint[PRINTF_CHAR_MAX]/*, ch*/;
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

//    if(!g_bEnableDbgFlag)
//        return 0;

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->bIsDebugPort)
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    #if _ENABLE_UART_1
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_2
    if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_3
    if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_4
    if(pstUart->u8UartPort == _UART_PORT_4)
        Uart4_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_5
    if(pstUart->u8UartPort == _UART_PORT_5)
        Uart5_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_6
    if(pstUart->u8UartPort == _UART_PORT_6)
        Uart6_xSemaphoreTask();
    else
    #endif
        UNUSED(pstUart);
#endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    for (i = 0; i < iret; i++)
    {
        #if 0
        if(szPrint[i]=='\n')
        {
            ch = '\r';
            USARTx_send_bytes(pstUart, (uint8_t *)&ch, 1);
        }
        #endif
        USARTx_send_bytes(pstUart, (uint8_t *)&szPrint[i], 1);
    }

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    #if _ENABLE_UART_1
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_2
    if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_3
    if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_4
    if(pstUart->u8UartPort == _UART_PORT_4)
        Uart4_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_5
    if(pstUart->u8UartPort == _UART_PORT_5)
        Uart5_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_6
    if(pstUart->u8UartPort == _UART_PORT_6)
        Uart6_xSemaphoreGive();
    else
    #endif
        UNUSED(pstUart);
#endif

    return iret;
}

#if _ENABLE_UART_1
int Uart1_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[PRINTF_CHAR_MAX];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];


    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->u8UartPort == _UART_PORT_1)
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart1_xSemaphoreTask();
    #endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart1_xSemaphoreGive();
    #endif

    return iret;
}
#endif

#if _ENABLE_UART_2
int Uart2_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[PRINTF_CHAR_MAX];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];


    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->u8UartPort == _UART_PORT_2
            #ifdef STM32F2XX_HD
            ||pstUart->u8UartPort == _UART_PORT_2_PD5_6
            #endif
        )
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart2_xSemaphoreTask();
    #endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart2_xSemaphoreGive();
    #endif

    return iret;
}
#endif

#if _ENABLE_UART_3
int Uart3_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[PRINTF_CHAR_MAX];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->u8UartPort == _UART_PORT_3
        #if _ENABLE_UART_3_PD8_PD9
        ||pstUart->u8UartPort == _UART_PORT_3_PD8_9
        #endif
            )
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart3_xSemaphoreTask();
    #endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart3_xSemaphoreGive();
    #endif

    return iret;
}
#endif

#if _ENABLE_UART_4
int Uart4_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[PRINTF_CHAR_MAX];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->u8UartPort == _UART_PORT_4)
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart4_xSemaphoreTask();
    #endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart4_xSemaphoreGive();
    #endif

    return iret;
}
#endif

#if _ENABLE_UART_5
int Uart5_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[PRINTF_CHAR_MAX];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->u8UartPort == _UART_PORT_5)
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart5_xSemaphoreTask();
    #endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart5_xSemaphoreGive();
    #endif

    return iret;
}
#endif

#if _ENABLE_UART_6
int Uart6_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[PRINTF_CHAR_MAX];
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        if(pstUart->u8UartPort == _UART_PORT_6)
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart6_xSemaphoreTask();
    #endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    #if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    Uart6_xSemaphoreGive();
    #endif

    return iret;
}
#endif

int Uartx_printf(pstUartDeclare_t pstUart, const char *fmt, ...)
{
    int iret;
    char szPrint[PRINTF_CHAR_MAX];
    va_list ap;

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    USARTx_send_bytes(pstUart, (uint8_t *)szPrint, iret);

    return iret;
}

int dbg_printf(const char *fmt, ...)
{
    int                 i, iret;
    char                szPrint[PRINTF_CHAR_MAX]/*, ch*/;
    va_list             ap;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(!g_bEnableDbgFlag)
        return 0;

    while(pstUart->u8UartPort != _UART_PORT_NONE)
    {
        #if (_DEBUG_PORT_SEL==_UART_PORT_1)
        if(pstUart->u8UartPort == _UART_PORT_1)
        #elif (_DEBUG_PORT_SEL==_UART_PORT_2)
        if(pstUart->u8UartPort == _UART_PORT_2)
        #elif (_DEBUG_PORT_SEL==_UART_PORT_3)
        if(pstUart->u8UartPort == _UART_PORT_3)
        #elif (_DEBUG_PORT_SEL==_UART_PORT_4)
        if(pstUart->u8UartPort == _UART_PORT_4)
        #elif (_DEBUG_PORT_SEL==_UART_PORT_5)
        if(pstUart->u8UartPort == _UART_PORT_5)
        #elif (_DEBUG_PORT_SEL==_UART_PORT_6)
        if(pstUart->u8UartPort == _UART_PORT_6)
        #else
        if(pstUart->bIsDebugPort)
        #endif
        {
            break;
        }
        pstUart ++;
    }

    if(pstUart->u8UartPort == _UART_PORT_NONE)
    {
        return 0;
    }

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    #if _ENABLE_UART_1
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_2
    if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_3
    if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_4
    if(pstUart->u8UartPort == _UART_PORT_4)
        Uart4_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_5
    if(pstUart->u8UartPort == _UART_PORT_5)
        Uart5_xSemaphoreTask();
    else
    #endif
    #if _ENABLE_UART_6
    if(pstUart->u8UartPort == _UART_PORT_6)
        Uart6_xSemaphoreTask();
    else
    #endif
        UNUSED(pstUart);
#endif

    va_start(ap, fmt);
    iret = Uart_vsnprintf(szPrint, PRINTF_CHAR_MAX, fmt, ap);
    va_end(ap);

    for (i = 0; i < iret; i++)
    {
        USARTx_send_bytes(pstUart, (uint8_t *)&szPrint[i], 1);
    }

#if _ENABLE_UART_SEMAPHORE_FUNCTION_BY_PRINT
    #if _ENABLE_UART_1
    if(pstUart->u8UartPort == _UART_PORT_1)
        Uart1_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_2
    if(pstUart->u8UartPort == _UART_PORT_2)
        Uart2_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_3
    if(pstUart->u8UartPort == _UART_PORT_3)
        Uart3_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_4
    if(pstUart->u8UartPort == _UART_PORT_4)
        Uart4_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_5
    if(pstUart->u8UartPort == _UART_PORT_5)
        Uart5_xSemaphoreGive();
    else
    #endif
    #if _ENABLE_UART_6
    if(pstUart->u8UartPort == _UART_PORT_6)
        Uart6_xSemaphoreGive();
    else
    #endif
        UNUSED(pstUart);
#endif

    return iret;
}

int UTL_strcmp(const char *string1, const char *string2)
{
    int iRet, i;

    i = 0;
    while(string1[i] || string2[i])
    {
        iRet = string1[i] - string2[i];
        if(iRet)
        {
            return iRet;
        }
        i++;
    }
    return 0;
}
int UTL_strncmp( const char *string1, const char *string2, int count )
{
    int iRet, i;

    for(i=0;i<count;i++)
    {
        iRet = string1[i] - string2[i];
        if(iRet)
        {
            return iRet;
        }
    }
    return 0;
}

size_t UTL_strlen(const char *pStr)
{
    size_t i=0;

    while(pStr[i])
    {
        i++;
    }
    return i;
}


