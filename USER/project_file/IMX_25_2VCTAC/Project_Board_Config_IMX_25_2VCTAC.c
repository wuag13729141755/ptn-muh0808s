#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5


const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    // Led
    //eGpioName_LedActive,
    {PORT_GPIOD,        GPIO_Pin_2, GPIO_Mode_Out_PP,       ePinLevel_High},

    //i2c
    //eGpioName_I2cEepromScl,
    {PORT_GPIOB,       GPIO_Pin_14, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOB,       GPIO_Pin_15, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIT66021Scl_1,
    {PORT_GPIOC,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIT66021Sda_1,
    {PORT_GPIOC,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIT66121Scl_1,
    {PORT_GPIOB,        GPIO_Pin_0, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIT66121Sda_1,
    {PORT_GPIOB,        GPIO_Pin_1, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCScl,
    {PORT_GPIOC,        GPIO_Pin_8, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCSda,
    {PORT_GPIOC,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},

    //eGpioName_GS2971Clk,
    {PORT_GPIOC,        GPIO_Pin_9, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_GS2971Cs,
    {PORT_GPIOC,       GPIO_Pin_12, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_GS2971Mosi,
    {PORT_GPIOC,       GPIO_Pin_11, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_GS2971Miso,
    {PORT_GPIOC,       GPIO_Pin_10, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_GS2972Clk,
    {PORT_GPIOB,        GPIO_Pin_3, GPIO_Mode_Out_PP,        ePinLevel_Low},
    //eGpioName_GS2972Cs,
    {PORT_GPIOB,        GPIO_Pin_6, GPIO_Mode_Out_PP,        ePinLevel_Low},
    //eGpioName_GS2972Mosi,
    {PORT_GPIOB,        GPIO_Pin_5, GPIO_Mode_Out_PP,        ePinLevel_Low},
    //eGpioName_GS2972Miso,
    {PORT_GPIOB,        GPIO_Pin_4, GPIO_Mode_Out_PP,        ePinLevel_Low},

    //Reset
    //eGpioName_IT66021Reset1,
    {PORT_GPIOA,        GPIO_Pin_6, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_IT66121Reset1,
    {PORT_GPIOA,        GPIO_Pin_7, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_GS2972Reset
    {PORT_GPIOB,        GPIO_Pin_7, GPIO_Mode_Out_PP,       ePinLevel_High},

};

const stKeyDeclare_t stProjectKeyTable[] =
{
    //key null
    {{NULL, \
    NULL, \
    NULL, \
    NULL}, \
    key_detect_mode_after, \
    KEY_LONG_INVALID, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}},\
    NULL, \
    IR_REMOTE_KEY_INVALID, \
    NULL}
};

const stLedDeclare_t stProjectLedTable[] =
{
    //led_name_ActiveLed = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedActive],             ePinLevel_Low},
    //led_name_End
    {(pstGpioConfig_t)NULL,                                                 ePinLevel_Low},// the last one
};

const stI2cDeclare_t stProjectI2cTable[] =
{
    //eI2cDevice_Eeprom = 0,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromSda],
        I2C_SPEED_TIME_FOR_75K,
        0,
        0,
        },
    //eI2cDevice_IT66021_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda_1],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_IT66121_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66121Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66121Sda_1],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_ALPU_C,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cAlpuCScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cAlpuCSda],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },

    //i2c null
    {   NULL,
        NULL,
        0,
        0,
        0,
        },
};

const stI2CSwitchCtrl_t stProjectI2cSwitchTable[] =
{
    // switch null
    {NULL, NULL},
};

const stSpiDeclare_t stProjectSpiTable[] =
{
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2971Mosi],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2971Miso],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2971Clk],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2971Cs],
        DELAY_TIME_FOR_SPI_FLASH_BYTE,
        DELAY_TIME_FOR_SPI_FLASH_CLK,
        1,
        },
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2972Mosi],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2972Miso],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2972Clk],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_GS2972Cs],
        DELAY_TIME_FOR_SPI_FLASH_BYTE,
        DELAY_TIME_FOR_SPI_FLASH_CLK,
        1,
        },

    //spi null
    {NULL,                                                          NULL,\
     NULL,                                                          NULL,\
     100,                                                           100   },
};

const stRxHpdDetDeclare_t stProjectRxHpdDetTable[] =
{
    {NULL},
};

const stIT663xxRstDeclare_t stProjectIT663xxRstTable[] =
{
    {NULL,                                                      ePinLevel_High},
};

const stIrWorkDeclare_t stProjectIrWorkTable =
{
    //{(pstGpioConfig_t)&stProjectGpioTable[eGpioName_IrSensor], eIrRmProtocol_NEC},
    {NULL, eIrRmProtocol_None},
    NULL,
};

const st5VControl_t stProjectTx5VControlTable[] =
{
    //hdmi output 5v null
    {NULL},
};

const stUartDeclare_t stProjectUartWorkTable[] =
{
#if _ENABLE_UART_1
    //uart port 1
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},
#endif

#if _ENABLE_UART_2
    //uart port 2
    {   _UART_PORT_2,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeout},
#endif

#if _ENABLE_UART_3
    //uart port 3
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeout},
#endif

    //uart port none
    {   _UART_PORT_NONE,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        NULL,
        NULL,
        NULL,
        NULL}
};

const stCecDeclare_t stProjectCecWorkTable[] =
{
    //cec none
    {NULL,                                                          {eCecSrcDisType_Src, FALSE, 0, 0x00}}
};

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
const stNewKeyDeclare_t stProjectNewKeyTable[] =
{
    //emKeyName_Source,
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyInput1],     0,  BIT0,   ePinLevel_Low,  FALSE},// 0

    {(pstGpioConfig_t)NULL,                                         0,  BIT0,   ePinLevel_Low,  FALSE},// the last one
};

#endif


#if _ENABLE_CHIP_GSV_CHIP_SERIES
const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[] =
{
    // none
    {NULL,ePinLevel_Low},
};
#endif

#if _ENABLE_CHIP_MS7200
stChipResetPinCfg_t stProjectMs7200ChipResetTable[]=
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_IT66021Reset1],        ePinLevel_Low},
    // none
    {NULL,ePinLevel_Low},
};
#endif

void app_SetGpioState(u8 u8Gpio,u8 u8Sta)
{
    u8 l_u8EnPort = stProjectGpioTable[u8Gpio].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[u8Gpio].u16GpioPin;

    if(u8Sta)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

bool app_GetGpioState(u8 u8Gpio)
{
    u8 l_u8PortType = stProjectGpioTable[u8Gpio].u8PortType;
    u16 l_u16GpioPin = stProjectGpioTable[u8Gpio].u16GpioPin;

    if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8PortType-PORT_GPIOA],l_u16GpioPin) == Bit_SET)
        return TRUE;
    return FALSE;
}


