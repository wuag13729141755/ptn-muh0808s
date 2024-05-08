/*****************************************************************************
Copyright       : PTN. Co., Ltd.
File name       : boot.c
Description     : 主要实现系统升级检测
Author          : wf8421
Version         : V1.0.0
Date            : 2021-6-23
History         :
*****************************************************************************/
#include  <stdarg.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <stdint.h>
#include "boot.h"

#define DEBUG_PORT              emUartNum_5

#ifdef STM32F2XX_HD
const stUartGpio_t UART_GPIO_TABLE[] =
{
    //usart 1
    {USART1,GPIOA, GPIO_Pin_9, GPIOA,GPIO_Pin_10,  GPIO_PinSource9, GPIO_PinSource10,RCC_AHB1Periph_GPIOA,GPIO_AF_USART1,RCC_APB2Periph_USART1,USART1_IRQn,3,0},
    //usart 2
    {USART2,GPIOA, GPIO_Pin_2, GPIOA, GPIO_Pin_3,  GPIO_PinSource2,  GPIO_PinSource3,RCC_AHB1Periph_GPIOA,GPIO_AF_USART2,RCC_APB1Periph_USART2,USART2_IRQn,3,0},
    //usart 3
    {USART3,GPIOB,GPIO_Pin_10, GPIOB,GPIO_Pin_11, GPIO_PinSource10, GPIO_PinSource11,RCC_AHB1Periph_GPIOB,GPIO_AF_USART3,RCC_APB1Periph_USART3,USART3_IRQn,3,0},
    //uart 4
    { UART4,GPIOA, GPIO_Pin_0, GPIOA, GPIO_Pin_1,  GPIO_PinSource0,  GPIO_PinSource1,RCC_AHB1Periph_GPIOA, GPIO_AF_UART4, RCC_APB1Periph_UART4, UART4_IRQn,3,0},
    //uart 5
    { UART5,GPIOC,GPIO_Pin_12, GPIOD, GPIO_Pin_2, GPIO_PinSource12,  GPIO_PinSource2,RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, GPIO_AF_UART5, RCC_APB1Periph_UART5, UART5_IRQn,3,0},
    //usart 6
    {USART6,GPIOC, GPIO_Pin_6, GPIOC, GPIO_Pin_7,  GPIO_PinSource6,  GPIO_PinSource7,RCC_AHB1Periph_GPIOC,GPIO_AF_USART6,RCC_APB2Periph_USART6,USART6_IRQn,3,0},
    //_UART_PORT_3_PD8_9
    {USART3,GPIOD, GPIO_Pin_8, GPIOD, GPIO_Pin_9,  GPIO_PinSource8,  GPIO_PinSource9,RCC_AHB1Periph_GPIOD,GPIO_AF_USART3,RCC_APB1Periph_USART3,USART3_IRQn,3,0},
    //_UART_PORT_2_PD5_6
    {USART2,GPIOD, GPIO_Pin_5, GPIOD, GPIO_Pin_6,  GPIO_PinSource5,  GPIO_PinSource6,RCC_AHB1Periph_GPIOD,GPIO_AF_USART2,RCC_APB1Periph_USART2,USART2_IRQn,3,0},
};
#elif (defined(GD32F4XX))
const stUartGpio_t UART_GPIO_TABLE[] =
{
    //usart 1
    {USART0,GPIOA, GPIO_Pin_9, GPIOA,GPIO_Pin_10,   RCU_GPIOA,  RCU_GPIOA,  GPIO_AF_7,   RCU_USART0, USART0_IRQn,3,0},
    //usart 2
    {USART1,GPIOA, GPIO_Pin_2, GPIOA, GPIO_Pin_3,   RCU_GPIOA,  RCU_GPIOA,  GPIO_AF_7,   RCU_USART1, USART1_IRQn,3,0},
    //usart 3
    {USART2,GPIOB,GPIO_Pin_10, GPIOB,GPIO_Pin_11,   RCU_GPIOB,  RCU_GPIOB,  GPIO_AF_7,   RCU_USART2, USART2_IRQn,3,0},
    //uart 4
    { UART3,GPIOA, GPIO_Pin_0, GPIOA, GPIO_Pin_1,   RCU_GPIOA,  RCU_GPIOA,  GPIO_AF_8,   RCU_UART3,  UART3_IRQn, 3,0},
    //uart 5
    { UART4,GPIOC,GPIO_Pin_12, GPIOD, GPIO_Pin_2,   RCU_GPIOC,  RCU_GPIOD,  GPIO_AF_8,   RCU_UART4,  UART4_IRQn, 3,0},
    //usart 6
    {USART5,GPIOC, GPIO_Pin_6, GPIOC, GPIO_Pin_7,   RCU_GPIOC,  RCU_GPIOC,  GPIO_AF_8,   RCU_USART5, USART5_IRQn,3,0},
    //_UART_PORT_3_PD8_9
    {USART2,GPIOD, GPIO_Pin_8, GPIOD, GPIO_Pin_9,   RCU_GPIOD,  RCU_GPIOD,  GPIO_AF_7,   RCU_USART2, USART2_IRQn,3,0},
    //_UART_PORT_2_PD5_6
    {USART1,GPIOD, GPIO_Pin_5, GPIOD, GPIO_Pin_6,   RCU_GPIOD,  RCU_GPIOD,  GPIO_AF_7,   RCU_USART1, USART1_IRQn,3,0},
};
#elif (defined(N32G45X))
const stUartGpio_t UART_GPIO_TABLE[] =
{
    //usart 1
    {USART1, GPIOA, GPIO_PIN_9, GPIO_PIN_10,RCC_APB2_PERIPH_USART1, USART1_IRQn,    3, 0},
    //usart 2
    {USART2, GPIOA, GPIO_PIN_2, GPIO_PIN_3, RCC_APB1_PERIPH_USART2, USART2_IRQn,    3, 0},
    //usart 3
    {USART3, GPIOB, GPIO_PIN_10,GPIO_PIN_11,RCC_APB1_PERIPH_USART3, USART3_IRQn,    3, 0},
    //usart 4
    {UART4,  GPIOB, GPIO_PIN_10,GPIO_PIN_11,RCC_APB1_PERIPH_USART3, USART3_IRQn,    3, 0}
};
#else
const stUartGpio_t UART_GPIO_TABLE[] =
{
    //usart 1
    {USART1, GPIOA, GPIO_Pin_9, GPIO_Pin_10,RCC_APB2Periph_USART1, USART1_IRQn, 3, 0},
    //usart 2
    {USART2, GPIOA, GPIO_Pin_2, GPIO_Pin_3, RCC_APB1Periph_USART2, USART2_IRQn, 3, 0},
    //usart 3
    {USART3, GPIOB, GPIO_Pin_10,GPIO_Pin_11,RCC_APB1Periph_USART3, USART3_IRQn, 3, 0}
};
#endif

#if (defined(GD32F4XX))
void USARTx_Configuration(u8 u8UartNum,u32 u32Buadrate)
{
    uint8_t                 u8UartIndex = u8UartNum-emUartNum_1;
    pstUartGpio_t           pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[u8UartIndex];

    nvic_irq_enable(pstUartGpio->IRQChannel, pstUartGpio->IRQChannelPreemptionPriority, pstUartGpio->IRQChannelSubPriority);
    rcu_periph_clock_enable((rcu_periph_enum)pstUartGpio->u32RccAhbPeriph_GpioTx);
    rcu_periph_clock_enable((rcu_periph_enum)pstUartGpio->u32RccAhbPeriph_GpioRx);

    /* enable USART clock */
    rcu_periph_clock_enable((rcu_periph_enum)pstUartGpio->u32RccApbPeriph);
    /* connect port to USARTx_Tx */
    gpio_af_set(pstUartGpio->pGpioPort, pstUartGpio->u8AFConfig, pstUartGpio->u16GpioTx);
    /* connect port to USARTx_Rx */
    gpio_af_set(pstUartGpio->pRxGpioPort, pstUartGpio->u8AFConfig, pstUartGpio->u16GpioRx);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(pstUartGpio->pGpioPort, GPIO_MODE_AF, GPIO_PUPD_PULLUP,pstUartGpio->u16GpioTx);
    gpio_output_options_set(pstUartGpio->pGpioPort, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,pstUartGpio->u16GpioTx);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(pstUartGpio->pRxGpioPort, GPIO_MODE_AF, GPIO_PUPD_PULLUP,pstUartGpio->u16GpioRx);
    gpio_output_options_set(pstUartGpio->pRxGpioPort, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,pstUartGpio->u16GpioRx);

    /* USART configure */
    usart_deinit(pstUartGpio->UARTx);
    usart_baudrate_set(pstUartGpio->UARTx,u32Buadrate);
    usart_receive_config(pstUartGpio->UARTx, USART_RECEIVE_ENABLE);
    usart_transmit_config(pstUartGpio->UARTx, USART_TRANSMIT_ENABLE);
    usart_word_length_set(pstUartGpio->UARTx,USART_WordLength_8b);
    usart_stop_bit_set(pstUartGpio->UARTx,USART_StopBits_1);
    usart_parity_config(pstUartGpio->UARTx,USART_Parity_No);
    usart_hardware_flow_rts_config(pstUartGpio->UARTx,USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(pstUartGpio->UARTx,USART_CTS_DISABLE);

    usart_enable(pstUartGpio->UARTx);
//    if(pstUartPort->stUartInitType.Mode&USART_RECEIVE_ENABLE)
//    {
//        usart_interrupt_enable(pstUartGpio->UARTx, USART_INT_RBNE);
//    }
}

#else
void USARTx_Configuration(u8 u8UartNum,u32 u32Buadrate)
{
    uint8_t                 u8UartIndex = u8UartNum-emUartNum_1;
    pstUartGpio_t           pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[u8UartIndex];
#if (defined(N32G45X))
    USART_InitType          USART_InitStructure;
    GPIO_InitType           GPIO_InitStructure; //这个不能省
    NVIC_InitType           NVIC_InitStructure;

    if(u8UartNum==emUartNum_1)
    {
        RCC_EnableAPB2PeriphClk(pstUartGpio->u32RccApbPeriph, ENABLE);
    }
    else
    {
        RCC_EnableAPB1PeriphClk(pstUartGpio->u32RccApbPeriph, ENABLE);
    }

    GPIO_InitStructure.Pin   = pstUartGpio->u16GpioTx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitPeripheral(pstUartGpio->pGpioPort, &GPIO_InitStructure);

    GPIO_InitStructure.Pin   = pstUartGpio->u16GpioRx;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitPeripheral(pstUartGpio->pGpioPort, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = u32Buadrate;
    USART_InitStructure.WordLength          = USART_WordLength_8b;
    USART_InitStructure.StopBits            = USART_StopBits_1;
    USART_InitStructure.Parity              = USART_Parity_No;
    USART_InitStructure.HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.Mode                = USART_Mode_Rx|USART_Mode_Tx;
    USART_Init(pstUartGpio->UARTx, &USART_InitStructure);
    USART_Enable(pstUartGpio->UARTx, ENABLE);

//    USART_ConfigInt(pstUartGpio->UARTx, USART_INT_RXDNE, DISABLE);
    USART_ConfigInt(pstUartGpio->UARTx, USART_INT_IDLEF, DISABLE);
//    USART_ConfigInt(pstUartGpio->UARTx, USART_INT_RXDNE, ENABLE);
#else
    USART_InitTypeDef       USART_InitStructure;
    GPIO_InitTypeDef        GPIO_InitStructure; //这个不能省
    NVIC_InitTypeDef        NVIC_InitStructure;

    /*下面是配置串口1的时钟*/
    #ifdef STM32F2XX_HD
    RCC_AHB1PeriphClockCmd(pstUartGpio->u32RccAhbPeriph_Gpio,ENABLE);
    GPIO_PinAFConfig(pstUartGpio->pGpioPort,pstUartGpio->u8GpioTxPinSrc,pstUartGpio->u8AFConfig);
    GPIO_PinAFConfig(pstUartGpio->pRxGpioPort,pstUartGpio->u8GpioRxPinSrc,pstUartGpio->u8AFConfig);
    #endif

    if(u8UartNum==emUartNum_1
    #ifdef STM32F2XX_HD
    ||u8UartNum == emUartNum_6
    #endif
    )
    {
        RCC_APB2PeriphClockCmd(pstUartGpio->u32RccApbPeriph, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(pstUartGpio->u32RccApbPeriph, ENABLE);
    }

    /*下面是配置串口1的管脚*/
    #ifdef STM32F2XX_HD
    /* Configure USARTx Tx as alternate function push-pull                */
    GPIO_InitStructure.GPIO_Pin   = pstUartGpio->u16GpioTx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(pstUartGpio->pGpioPort, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = pstUartGpio->u16GpioRx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(pstUartGpio->pRxGpioPort, &GPIO_InitStructure);
    #else
    /* Configure USARTx Tx as alternate function push-pull                */
    GPIO_InitStructure.GPIO_Pin   = pstUartGpio->u16GpioTx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(pstUartGpio->pGpioPort, &GPIO_InitStructure);
    /* Configure USARTx Rx as input floating                             */
    GPIO_InitStructure.GPIO_Pin   = pstUartGpio->u16GpioRx;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(pstUartGpio->pGpioPort, &GPIO_InitStructure);
    #endif

    /*下面是配置串口1的工作模式即使能*/
    USART_InitStructure.USART_BaudRate            = u32Buadrate;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx|USART_Mode_Tx;
    USART_Init(pstUartGpio->UARTx, &USART_InitStructure);

//    if(pstUartPort->funUartReceive != NULL)
//    {
//        USART_ITConfig(pstUartGpio->UARTx, USART_IT_RXNE, ENABLE);
//    }
    USART_Cmd(pstUartGpio->UARTx, ENABLE);
#endif

//    if(pstUartPort->funUartReceive != NULL)
//    {
//        /*下面是配置串口1的中断向量*/
//        //NVIC_PriorityGroupConfig(INT_DEFAULT_GROUP);
//        NVIC_InitStructure.NVIC_IRQChannel = pstUartGpio->IRQChannel;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pstUartGpio->IRQChannelPreemptionPriority;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = pstUartGpio->IRQChannelSubPriority;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);
//    }
}
#endif
#if (defined(N32G45X))
void USARTx_DirectSend_bytes(USART_Module * pstUart, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16 i;

    for(i=0; i<u16Len; i++)
    {
        USART_SendData(pstUart, *(pu8Buffer+i));
        while (USART_GetFlagStatus(pstUart, USART_FLAG_TXC) == RESET){}
    }
}
#elif (defined(GD32F4XX))
void USARTx_DirectSend_bytes(uint32_t pstUart, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16 i;

    for(i=0; i<u16Len; i++)
    {
        usart_data_transmit(pstUart, *(pu8Buffer+i));
        while(RESET == usart_flag_get(pstUart, USART_FLAG_TC));
    }
}

#else
void USARTx_DirectSend_bytes(USART_TypeDef * pstUart, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16 i;

    for(i=0; i<u16Len; i++)
    {
    #ifdef STM32F2XX_HD
        while(!(pstUart->SR & USART_FLAG_TXE));
        pstUart->DR = *(pu8Buffer+i);
        while(!(pstUart->SR & USART_FLAG_TC));
    #else
        USART_SendData(pstUart, *(pu8Buffer+i));
        while (USART_GetFlagStatus(pstUart, USART_FLAG_TC) == RESET){}
    #endif
    }
}
#endif

int dbg_printf(const char *fmt, ...)
{
    int                 iret;
    char                szPrint[256];
    va_list             ap;
    uint8_t             u8UartIndex = DEBUG_PORT-emUartNum_1;
    pstUartGpio_t       pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[u8UartIndex];

    va_start(ap, fmt);
    iret = vsnprintf(szPrint, 256, fmt, ap);
    va_end(ap);

#if 0
    USARTx_DirectSend_bytes(pstUartGpio->UARTx, (uint8_t *)szPrint, iret);
#else
    UNUSED(pstUartGpio);
#endif

    return iret;
}

/*****************************************************************************
Function        : uint8_t check_update(uint32_t addr)
Description     : 检测升级标志，跳转应用区
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
uint8_t check_update(uint32_t addr)
{
    uint8_t flag = 0;
    uint32_t update_flag = 0;
    uint32_t *tmp;
    tmp = (uint32_t *)addr;
    update_flag = *tmp;
    if(update_flag == 0x11223344) {
        dbg_printf("system update!!!\r\n");
        flag = 1;
    } else { // if(update_flag == 0x44332211)
        flag = 0;
    }
    return flag;
}

/*****************************************************************************
Function        : int main()
Description     : C库主函数
Calls           : null
Called By       : null
Table Accessed  : null
Table Updated   : null
Input           : null
Output          : null
Return          : null
Others          : null
*****************************************************************************/
int main(void)
{
    USARTx_Configuration(emUartNum_5,115200);
    if(check_update(DM_FW_FLAG_ADDR) == 0x01) {
        to_copy_app(APP_START_ADDR,DM_FW_DATA_ADDR,DM_FW_BYTES_MAX);
        dbg_printf("to copy app!!!\r\n");
    }
    to_jump_app(APP_START_ADDR);
    dbg_printf("************************Warning**************************\r\n");
    dbg_printf("Program file error, please use the downloader to re-burn!\r\n");
    for(;;) {
        ;
    }
}


/**
 * @brief Assert failed function by user.
 * @param file The name of the call that failed.
 * @param line The source line number of the call that failed.
 */
#ifdef USE_FULL_ASSERT
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    dbg_printf("Wrong parameters value: file %s on line %d\r\n", file, line);
    while (1) {
    }
}
#endif // USE_FULL_ASSERT



