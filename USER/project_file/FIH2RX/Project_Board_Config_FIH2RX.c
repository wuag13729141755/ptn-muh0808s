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
    //eGpioName_I2cGSV2011Scl_1,
    {PORT_GPIOB,        GPIO_Pin_1, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2011Sda_1,
    {PORT_GPIOB,        GPIO_Pin_0, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2011Scl_2,
    {PORT_GPIOC,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV2011Sda_2,
    {PORT_GPIOA,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV5100Scl,
    {PORT_GPIOC,        GPIO_Pin_8, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV5100Sda,
    {PORT_GPIOC,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV5100DDCScl,
    {PORT_GPIOA,        GPIO_Pin_0, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cGSV5100DDCSda,
    {PORT_GPIOC,        GPIO_Pin_3, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIT66021Scl,
    {PORT_GPIOC,       GPIO_Pin_12, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIT66021Sda,
    {PORT_GPIOB,        GPIO_Pin_3, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIDT5V49002Scl,
    {PORT_GPIOA,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cIDT5V49002Sda,
    {PORT_GPIOA,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCScl,
    {PORT_GPIOC,       GPIO_Pin_10, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCSda,
    {PORT_GPIOC,       GPIO_Pin_11, GPIO_Mode_Out_OD,       ePinLevel_High},

    //Reserve SPI
    //eGpioName_SpiFlashClk,
    {PORT_GPIOB,       GPIO_Pin_15, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_SpiFlashCs,
    {PORT_GPIOB,       GPIO_Pin_12, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_SpiFlashMosi,
    {PORT_GPIOB,       GPIO_Pin_14, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_SpiFlashMiso,
    {PORT_GPIOB,       GPIO_Pin_13, GPIO_Mode_Out_OD,       ePinLevel_High},

    //Reserve IO
    //eGpioName_IO1,
    {PORT_GPIOC,        GPIO_Pin_1,    GPIO_Mode_IPD,       ePinLevel_High},
    //eGpioName_IO2,
    {PORT_GPIOC,        GPIO_Pin_0,    GPIO_Mode_IPD,       ePinLevel_High},
    //eGpioName_IO3,
    {PORT_GPIOC,        GPIO_Pin_6,    GPIO_Mode_IPD,       ePinLevel_High},

    //Reset
    //eGpioName_IT66021Reset,
    {PORT_GPIOB,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_Gsv2011Reset1,
    {PORT_GPIOC,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_Gsv2011Reset2,
    {PORT_GPIOA,        GPIO_Pin_6, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_CS5340Reset,
    {PORT_GPIOB,        GPIO_Pin_8, GPIO_Mode_Out_OD,       ePinLevel_High},

    //Hdmi
    //eGpioName_HDMI5VEn,
    {PORT_GPIOC,        GPIO_Pin_9, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_Gsv5100HpdCtrl,
    {PORT_GPIOC,        GPIO_Pin_2, GPIO_Mode_Out_PP,       ePinLevel_High},


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

    //eI2cDevice_GSV2011_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Sda_1],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_GSV2011_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Scl_2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Sda_2],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_GSV5100,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV5100Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV5100Sda],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_GSV5100DDC,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV5100DDCScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV5100DDCSda],
        I2C_SPEED_TIME_FOR_75K,
        0,
        0,
        },
    //eI2cDevice_IT66021,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda],
        I2C_SPEED_TIME_FOR_200K,
        0,
        0,
        },
    //eI2cDevice_IDT5V49EE902,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIDT5V49EE902Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIDT5V49EE902Sda],
        I2C_SPEED_TIME_FOR_100K,
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
    // Flash
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashMosi],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashMiso],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashClk],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashCs],
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
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeout},

    //uart port 3
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeout},

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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2011Reset1],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2011Reset2],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif


void mapp_SetTx5VControl(bool bSta)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_HDMI5VEn].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_HDMI5VEn].u16GpioPin;
    if(bSta)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

