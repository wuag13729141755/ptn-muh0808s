#include "includes.h"

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
    //_UART_PORT_4_PC10_11
    { UART4,GPIOC,GPIO_Pin_10, GPIOC,GPIO_Pin_11, GPIO_PinSource10, GPIO_PinSource11,RCC_AHB1Periph_GPIOC, GPIO_AF_UART4, RCC_APB1Periph_UART4, UART4_IRQn,3,0},
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
    //_UART_PORT_4_PC10_11
    { UART3,GPIOC,GPIO_Pin_10, GPIOC,GPIO_Pin_11,   RCU_GPIOA,  RCU_GPIOC,  GPIO_AF_8,   RCU_UART3,  UART3_IRQn, 3,0},
};
#elif (defined(N32G45X))
const stUartGpio_t UART_GPIO_TABLE[] =
{
    //usart 1
    {USART1, GPIOA, GPIO_PIN_9, GPIOA, GPIO_PIN_10, RCC_APB2_PERIPH_USART1, USART1_IRQn,    3, 0},
    //usart 2
    {USART2, GPIOA, GPIO_PIN_2, GPIOA,  GPIO_PIN_3, RCC_APB1_PERIPH_USART2, USART2_IRQn,    3, 0},
    //usart 3
    {USART3, GPIOB,GPIO_PIN_10, GPIOB, GPIO_PIN_11, RCC_APB1_PERIPH_USART3, USART3_IRQn,    3, 0},
    //usart 4
    { UART4, GPIOC,GPIO_PIN_10, GPIOC, GPIO_PIN_11,  RCC_APB1_PERIPH_UART4,  UART4_IRQn,    3, 0},
    //usart 5
    { UART5, GPIOC,GPIO_PIN_12, GPIOD,  GPIO_PIN_2,  RCC_APB1_PERIPH_UART5,  UART5_IRQn,    3, 0},
    //usart 6
    { UART6, GPIOC, GPIO_PIN_0, GPIOC,  GPIO_PIN_1,  RCC_APB2_PERIPH_UART6,  UART6_IRQn,    3, 0},
    //usart 7 pc4 pc5
    { UART7, GPIOC, GPIO_PIN_4, GPIOC,  GPIO_PIN_5,  RCC_APB2_PERIPH_UART7,  UART7_IRQn,    3, 0},
    //_UART_PORT_7_PC2_3
    { UART7, GPIOC, GPIO_PIN_2, GPIOC,  GPIO_PIN_3,  RCC_APB2_PERIPH_UART7,  UART7_IRQn,    3, 0},
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

extern const stUartDeclare_t stProjectUartWorkTable[];

#if 0//def USING_DEBUG
int fputc(int ch, FILE *f)
{
    USART_SendData(USART2, (uint8_t) ch);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
    {}
    return ch;
}
#endif

#if (defined(GD32F4XX))
void USARTx_Configuration(pstUartDeclare_t pstUartPort)
{
    uint8_t                 u8UartIndex = pstUartPort->u8UartPort-_UART_PORT_1;
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
    usart_baudrate_set(pstUartGpio->UARTx,pstUartPort->stUartInitType.BaudRate);
    if(pstUartPort->stUartInitType.Mode&USART_RECEIVE_ENABLE)
    {
        usart_receive_config(pstUartGpio->UARTx, USART_RECEIVE_ENABLE);
    }
    if(pstUartPort->stUartInitType.Mode&USART_TRANSMIT_ENABLE)
    {
        usart_transmit_config(pstUartGpio->UARTx, USART_TRANSMIT_ENABLE);
    }
    usart_word_length_set(pstUartGpio->UARTx,pstUartPort->stUartInitType.WordLength);
    usart_stop_bit_set(pstUartGpio->UARTx,pstUartPort->stUartInitType.StopBits);
    usart_parity_config(pstUartGpio->UARTx,pstUartPort->stUartInitType.Parity);
    if(pstUartPort->stUartInitType.HardwareFlowControl&USART_RTS_ENABLE)
        usart_hardware_flow_rts_config(pstUartGpio->UARTx,USART_RTS_ENABLE);
    else
        usart_hardware_flow_rts_config(pstUartGpio->UARTx,USART_RTS_DISABLE);
    if(pstUartPort->stUartInitType.HardwareFlowControl&USART_CTS_ENABLE)
        usart_hardware_flow_cts_config(pstUartGpio->UARTx,USART_CTS_ENABLE);
    else
        usart_hardware_flow_cts_config(pstUartGpio->UARTx,USART_CTS_DISABLE);

    usart_enable(pstUartGpio->UARTx);
    if(pstUartPort->stUartInitType.Mode&USART_RECEIVE_ENABLE)
    {
        usart_interrupt_enable(pstUartGpio->UARTx, USART_INT_RBNE);
    }
}

#else
void USARTx_Configuration(pstUartDeclare_t pstUartPort)
{
    uint8_t                 u8UartIndex = pstUartPort->u8UartPort-_UART_PORT_1;
    pstUartGpio_t           pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[u8UartIndex];
#if (defined(N32G45X))
    USART_InitType          USART_InitStructure;
    GPIO_InitType           GPIO_InitStructure; //这个不能省
    NVIC_InitType           NVIC_InitStructure;

//    RCC_EnableAPB2PeriphClk(COM_TX_PORT_CLK[COM]|RCC_APB2_PERIPH_AFIO,ENABLE);
    if((pstUartPort->u8UartPort==_UART_PORT_1)
        ||(pstUartPort->u8UartPort==_UART_PORT_6)
        ||(pstUartPort->u8UartPort==_UART_PORT_7))
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
    GPIO_InitPeripheral(pstUartGpio->pRxGpioPort, &GPIO_InitStructure);

    USART_InitStructure.BaudRate            = pstUartPort->stUartInitType.BaudRate;
    USART_InitStructure.WordLength          = pstUartPort->stUartInitType.WordLength;
    USART_InitStructure.StopBits            = pstUartPort->stUartInitType.StopBits;
    USART_InitStructure.Parity              = pstUartPort->stUartInitType.Parity;
    USART_InitStructure.HardwareFlowControl = pstUartPort->stUartInitType.HardwareFlowControl;
    USART_InitStructure.Mode                = pstUartPort->stUartInitType.Mode;
    USART_Init(pstUartGpio->UARTx, &USART_InitStructure);
    USART_Enable(pstUartGpio->UARTx, ENABLE);

//    USART_ConfigInt(pstUartGpio->UARTx, USART_INT_RXDNE, DISABLE);
    USART_ConfigInt(pstUartGpio->UARTx, USART_INT_IDLEF, DISABLE);
    USART_ConfigInt(pstUartGpio->UARTx, USART_INT_RXDNE, ENABLE);
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

    if(pstUartPort->u8UartPort==_UART_PORT_1
    #ifdef STM32F2XX_HD
    ||pstUartPort->u8UartPort == _UART_PORT_6
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
    USART_InitStructure.USART_BaudRate            = pstUartPort->stUartInitType.USART_BaudRate;
    USART_InitStructure.USART_WordLength          = pstUartPort->stUartInitType.USART_WordLength;
    USART_InitStructure.USART_StopBits            = pstUartPort->stUartInitType.USART_StopBits;
    USART_InitStructure.USART_Parity              = pstUartPort->stUartInitType.USART_Parity;
    USART_InitStructure.USART_HardwareFlowControl = pstUartPort->stUartInitType.USART_HardwareFlowControl;
    USART_InitStructure.USART_Mode                = pstUartPort->stUartInitType.USART_Mode;
    USART_Init(pstUartGpio->UARTx, &USART_InitStructure);

//    if(pstUartPort->funUartReceive != NULL)
//    {
//        USART_ITConfig(pstUartGpio->UARTx, USART_IT_RXNE, ENABLE);
//    }
    USART_Cmd(pstUartGpio->UARTx, ENABLE);
    if(pstUartPort->funUartReceive != NULL)
    {
        USART_ITConfig(pstUartGpio->UARTx, USART_IT_RXNE, ENABLE);
    }
#endif

    if(pstUartPort->funUartReceive != NULL)
    {
        /*下面是配置串口1的中断向量*/
        //NVIC_PriorityGroupConfig(INT_DEFAULT_GROUP);
        NVIC_InitStructure.NVIC_IRQChannel = pstUartGpio->IRQChannel;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = pstUartGpio->IRQChannelPreemptionPriority;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = pstUartGpio->IRQChannelSubPriority;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
}
#endif

#if (defined(N32G45X))

/* USART1 Send bytes*/
void USARTx_send_bytes(pstUartDeclare_t pstUartPort, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16             i;
    pstUartGpio_t   pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[pstUartPort->u8UartPort-_UART_PORT_1];

    for(i=0; i<u16Len; i++)
    {
        USART_SendData(pstUartGpio->UARTx, *(pu8Buffer+i));
        while (USART_GetFlagStatus(pstUartGpio->UARTx, USART_FLAG_TXC) == RESET){}
    }
}

/* USART1 Send string*/
void USARTx_send_string(pstUartDeclare_t pstUartPort, char *s)
{
    pstUartGpio_t   pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[pstUartPort->u8UartPort-_UART_PORT_1];

    while(*s != '\0')
    {
        USART_SendData(pstUartGpio->UARTx, *s);
        while (USART_GetFlagStatus(pstUartGpio->UARTx, USART_FLAG_TXC) == RESET){}
        s ++;
    }
}

void USART1_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];
//    if(USART_GetFlagStatus(USART1, USART_FLAG_IDLEF) != RESET)
//    {
//        USART_ClrFlag(USART1,USART_FLAG_IDLEF);
//    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(USART1);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_1)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
        USART_ClrFlag(USART1,USART_FLAG_RXDNE);
    }
}

void USART2_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetIntStatus(USART2, USART_INT_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(USART2);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_2)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
        USART_ClrIntPendingBit(USART2, USART_INT_RXDNE);
    }
}

void USART3_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetFlagStatus(USART3, USART_FLAG_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(USART3);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_3)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
        USART_ClrFlag(USART3,USART_FLAG_RXDNE);
    }
}

#if _ENABLE_UART_4
void UART4_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(USART_GetFlagStatus(UART4, USART_FLAG_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(UART4);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_4)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_5
void UART5_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(USART_GetFlagStatus(UART5, USART_FLAG_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(UART5);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_5)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_6
void UART6_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(USART_GetFlagStatus(UART6, USART_FLAG_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(UART6);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_6)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_7
void UART7_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if(USART_GetFlagStatus(UART7, USART_FLAG_RXDNE) != RESET)
    {
        temp = USART_ReceiveData(UART7);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if((pstUart->u8UartPort == _UART_PORT_7)
                ||(pstUart->u8UartPort == _UART_PORT_7_PC2_3))
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#elif (defined(GD32F4XX))
/* USART1 Send bytes*/
void USARTx_send_bytes(pstUartDeclare_t pstUartPort, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16             i;
    pstUartGpio_t   pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[pstUartPort->u8UartPort-_UART_PORT_1];

    for(i=0; i<u16Len; i++)
    {
        usart_data_transmit(pstUartGpio->UARTx, *(pu8Buffer+i));
        while(RESET == usart_flag_get(pstUartGpio->UARTx, USART_FLAG_TC));
    }
}

/* USART1 Send string*/
void USARTx_send_string(pstUartDeclare_t pstUartPort, char *s)
{
    pstUartGpio_t   pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[pstUartPort->u8UartPort-_UART_PORT_1];

    while(*s != '\0')
    {
        usart_data_transmit(pstUartGpio->UARTx, *(s));
        while(RESET == usart_flag_get(pstUartGpio->UARTx, USART_FLAG_TC));
        s ++;
    }
}

void USART0_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if((RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
        &&(RESET != usart_flag_get(USART0, USART_FLAG_RBNE)))
    {
        temp = usart_data_receive(USART0)&0xFF;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_1)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}

void USART1_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if((RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
        &&(RESET != usart_flag_get(USART1, USART_FLAG_RBNE)))
    {
        temp = usart_data_receive(USART1);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_2
                ||pstUart->u8UartPort == _UART_PORT_2_PD5_6
            )
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}

void USART2_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if((RESET != usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE))
        &&(RESET != usart_flag_get(USART2, USART_FLAG_RBNE)))
    {
        temp = usart_data_receive(USART2);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_3
                ||pstUart->u8UartPort == _UART_PORT_3_PD8_9
            )
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}

#else

/* USART1 Send bytes*/
void USARTx_send_bytes(pstUartDeclare_t pstUartPort, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16             i;
    pstUartGpio_t   pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[pstUartPort->u8UartPort-_UART_PORT_1];

    for(i=0; i<u16Len; i++)
    {
    #ifdef STM32F2XX_HD
        while(!(pstUartGpio->UARTx->SR & USART_FLAG_TXE));
        pstUartGpio->UARTx->DR = *(pu8Buffer+i);
        while(!(pstUartGpio->UARTx->SR & USART_FLAG_TC));
    #else
        USART_SendData(pstUartGpio->UARTx, *(pu8Buffer+i));
        while (USART_GetFlagStatus(pstUartGpio->UARTx, USART_FLAG_TC) == RESET){}
    #endif
    }
}

/* USART1 Send string*/
void USARTx_send_string(pstUartDeclare_t pstUartPort, char *s)
{
    pstUartGpio_t   pstUartGpio = (pstUartGpio_t)&UART_GPIO_TABLE[pstUartPort->u8UartPort-_UART_PORT_1];

    while(*s != '\0')
    {
    #ifdef STM32F2XX_HD
        while(!(pstUartGpio->UARTx->SR & USART_FLAG_TXE));
        pstUartGpio->UARTx->DR = *s;
        while(!(pstUartGpio->UARTx->SR & USART_FLAG_TC));
    #else
        USART_SendData(pstUartGpio->UARTx, *s);
        while (USART_GetFlagStatus(pstUartGpio->UARTx, USART_FLAG_TC) == RESET){}
    #endif
        s ++;
    }
}

void USART1_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        temp = USART1->DR;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_1)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}

void USART2_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp = USART2->DR;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_2
            #ifdef STM32F2XX_HD
            ||pstUart->u8UartPort == _UART_PORT_2_PD5_6
            #endif
            )
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}

void USART3_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        temp = USART3->DR;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_3
            #ifdef STM32F2XX_HD
            ||pstUart->u8UartPort == _UART_PORT_3_PD8_9
            #endif
            )
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#ifdef STM32F2XX_HD
#if _ENABLE_UART_4
void UART4_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        temp = UART4->DR;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_4
                #if ((defined(STM32F2XX_HD))||defined(GD32F4XX))
                ||pstUart->u8UartPort == _UART_PORT_4_PC10_11
                #endif
            )
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_5
void UART5_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        temp = UART5->DR;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_5)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_6
void USART6_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        temp = USART6->DR;

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_6)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif
#elif defined(GD32F4XX)
#if _ENABLE_UART_4
void UART3_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if((RESET != usart_interrupt_flag_get(UART3, USART_INT_FLAG_RBNE))
        &&(RESET != usart_flag_get(UART3, USART_FLAG_RBNE)))
    {
        temp = usart_data_receive(UART3);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if((pstUart->u8UartPort == _UART_PORT_4)
                ||(pstUart->u8UartPort == _UART_PORT_4_PC10_11))
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_5
void UART4_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if((RESET != usart_interrupt_flag_get(UART4, USART_INT_FLAG_RBNE))
        &&(RESET != usart_flag_get(UART4, USART_FLAG_RBNE)))
    {
        temp = usart_data_receive(UART4);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_5)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#if _ENABLE_UART_6
void USART5_IRQHandler(void)
{
    uint8_t temp;
    pstUartDeclare_t    pstUart = (pstUartDeclare_t)&stProjectUartWorkTable[0];

    if((RESET != usart_interrupt_flag_get(USART5, USART_INT_FLAG_RBNE))
        &&(RESET != usart_flag_get(USART5, USART_FLAG_RBNE)))
    {
        temp = usart_data_receive(USART5);

        while(pstUart->u8UartPort != _UART_PORT_NONE)
        {
            if(pstUart->u8UartPort == _UART_PORT_6)
            {
                pstUart->funUartReceive(temp);
                break;
            }
            pstUart ++;
        }
    }
}
#endif

#endif
#if (defined(N32G45X))
void USARTx_DirectSend_bytes(USART_Module * pstUart, uint8_t *pu8Buffer, uint16_t u16Len)
{
    u16 i;

    for(i=0; i<u16Len; i++)
    {
        //while (USART_GetFlagStatus(pstUart, USART_FLAG_TXDE) == RESET);
        USART_SendData(pstUart, *(pu8Buffer+i));
        while (USART_GetFlagStatus(pstUart, USART_FLAG_TXC) == RESET){};
    }
}

void USARTx_set_baudrate(USART_Module* USARTx,
                uint32_t USART_BaudRate,
                uint16_t USART_WordLength,
                uint16_t USART_StopBits,
                uint16_t USART_Parity,
                uint16_t USART_Mode,
                uint16_t USART_HardwareFlowControl)
{
    USART_InitType USART_InitStructure;

    /*下面是配置串口x的工作模式即使能*/
    USART_InitStructure.BaudRate            = USART_BaudRate;
    USART_InitStructure.WordLength          = USART_WordLength;
    USART_InitStructure.StopBits            = USART_StopBits;
    USART_InitStructure.Parity              = USART_Parity;
    USART_InitStructure.HardwareFlowControl = USART_HardwareFlowControl;
    USART_InitStructure.Mode                = USART_Mode;

    USART_Init(USARTx, &USART_InitStructure);
    USART_ConfigInt(USARTx, USART_INT_RXDNE, ENABLE);
    USART_Enable(USARTx, ENABLE);
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

void USARTx_set_baudrate(uint32_t USARTx,
                uint32_t USART_BaudRate,
                uint16_t USART_WordLength,
                uint16_t USART_StopBits,
                uint16_t USART_Parity,
                uint16_t USART_Mode,
                uint16_t USART_HardwareFlowControl)
{

    /* USART configure */
    usart_deinit(USARTx);
    usart_baudrate_set(USARTx,USART_BaudRate);
    usart_receive_config(USARTx, USART_Mode&USART_RECEIVE_ENABLE);
    usart_transmit_config(USARTx, USART_Mode&USART_TRANSMIT_ENABLE);
    usart_word_length_set(USARTx,USART_WordLength);
    usart_stop_bit_set(USARTx,USART_StopBits);
    usart_parity_config(USARTx,USART_Parity);
    if(USART_HardwareFlowControl&USART_RTS_ENABLE)
        usart_hardware_flow_rts_config(USARTx,USART_RTS_ENABLE);
    else
        usart_hardware_flow_rts_config(USARTx,USART_RTS_DISABLE);
    if(USART_HardwareFlowControl&USART_CTS_ENABLE)
        usart_hardware_flow_cts_config(USARTx,USART_CTS_ENABLE);
    else
        usart_hardware_flow_cts_config(USARTx,USART_CTS_DISABLE);
    usart_enable(USARTx);

    usart_interrupt_enable(USARTx, USART_INT_RBNE);
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

void USARTx_set_baudrate(USART_TypeDef* USARTx,
                uint32_t USART_BaudRate,
                uint16_t USART_WordLength,
                uint16_t USART_StopBits,
                uint16_t USART_Parity,
                uint16_t USART_Mode,
                uint16_t USART_HardwareFlowControl)
{
    USART_InitTypeDef USART_InitStructure;

    /*下面是配置串口x的工作模式即使能*/
    USART_InitStructure.USART_BaudRate            = USART_BaudRate;
    USART_InitStructure.USART_WordLength          = USART_WordLength;
    USART_InitStructure.USART_StopBits            = USART_StopBits;
    USART_InitStructure.USART_Parity              = USART_Parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl;
    USART_InitStructure.USART_Mode                = USART_Mode;

    USART_Init(USARTx, &USART_InitStructure);
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    USART_Cmd(USARTx, ENABLE);
}
#endif





