#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5


const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    // Led
    //eGpioName_LedActive,
    {PORT_GPIOA,        GPIO_Pin_1, GPIO_Mode_Out_PP,       ePinLevel_High},

    //i2c
    //eGpioName_I2cEepromScl,
    {PORT_GPIOB,        GPIO_Pin_6, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOB,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Scl_1,
    {PORT_GPIOB,        GPIO_Pin_1, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Sda_1,
    {PORT_GPIOB,        GPIO_Pin_0, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Scl_2,
    {PORT_GPIOC,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Sda_2,
    {PORT_GPIOC,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Scl_3,
    {PORT_GPIOA,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Sda_3,
    {PORT_GPIOA,        GPIO_Pin_6, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Scl_4,
    {PORT_GPIOA,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Sda_4,
    {PORT_GPIOA,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Scl_5,
    {PORT_GPIOC,       GPIO_Pin_12, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2001Sda_5,
    {PORT_GPIOB,        GPIO_Pin_3, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCScl,
    {PORT_GPIOC,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCSda,
    {PORT_GPIOC,        GPIO_Pin_8, GPIO_Mode_Out_OD,       ePinLevel_High},

    //Reset
    //eGpioName_Gsv2001Reset1,
    {PORT_GPIOA,        GPIO_Pin_0, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_Gsv2001Reset2,
    {PORT_GPIOC,        GPIO_Pin_3, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_Gsv2001Reset3,
    {PORT_GPIOC,        GPIO_Pin_2, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_Gsv2001Reset4,
    {PORT_GPIOC,        GPIO_Pin_1, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_Gsv2001Reset5,
    {PORT_GPIOC,        GPIO_Pin_0, GPIO_Mode_Out_OD,       ePinLevel_High},

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
    //eI2cDevice_GSV2001_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Sda_1],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_GSV2001_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Scl_2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Sda_2],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_GSV2001_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Scl_3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Sda_3],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_GSV2001_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Scl_4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Sda_4],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_GSV2001_5,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Scl_5],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2001Sda_5],
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
    //uart port 1
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        NULL,/*Uart1ParserMethod,*/
        Uart1VarInit,
        Uart1VarTimeOut},

    //uart port 2
    {   _UART_PORT_2,
        {UART2_BAUDRATE, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        UartProtocalDataReceive,
        UartParserMethod,
        UartVarInit,
        UartVarTimeout},

    //uart port 3
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        NULL,
        NULL,
        NULL,
        NULL},

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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset1],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset2],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset3],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset4],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset5],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif


