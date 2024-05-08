#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       3

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_LedSta,
    {PORT_GPIOA,        GPIO_Pin_2,   GPIO_Mode_OUT, GPIO_OType_PP,   GPIO_PuPd_DOWN,   ePinLevel_Low},

    //i2c
    //eGpioName_I2cEepromScl
    {PORT_GPIOA,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cEepromSda
    {PORT_GPIOC,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cADN4604Scl,
    {PORT_GPIOB,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cADN4604Sda,
    {PORT_GPIOB,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //Spi
    //eGpioName_SpiFlashClk,
    {PORT_GPIOB,       GPIO_Pin_13,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiFlashCs,
    {PORT_GPIOB,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiFlashMosi,
    {PORT_GPIOB,       GPIO_Pin_15,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_SpiFlashMiso,
    {PORT_GPIOB,       GPIO_Pin_14,    GPIO_Mode_IN, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // reset
    //eGpioName_ADN4604Rst,
    {PORT_GPIOB,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    // FAN control
    //eGpioName_FANControl,
    {PORT_GPIOE,        GPIO_Pin_2,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
};

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
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
#else
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
#endif

const stLedDeclare_t stProjectLedTable[] =
{
    //led_name_ActiveState = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedSta],          ePinLevel_High},
    //led_name_End
    {(pstGpioConfig_t)NULL,                                                 ePinLevel_Low},// the last one
};

const stI2cDeclare_t stProjectI2cTable[] =
{
    //eeprom
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromSda],
        I2C_SPEED_TIME_FOR_75K,
        0,
        0,
        },

    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cADN4604Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cADN4604Sda],
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

const stSpiDeclare_t stProjectSpiTable[] =
{
    // Flash
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashMosi],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashMiso],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashClk],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_SpiFlashCs],
        DELAY_TIME_FOR_SPI_FLASH_BYTE,
        DELAY_TIME_FOR_SPI_FLASH_CLK,
        0,
        },

    //spi null
    {NULL,                                                          NULL,\
     NULL,                                                          NULL,\
     100,                                                           100   },
};

const stRxHpdDetDeclare_t stProjectRxHpdDetTable[] =
{
    //hdmi input hpd null
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
    //uart port 1   --- to in/out board
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},
    //uart port 2   --- to CPLD
    {   _UART_PORT_2_PD5_6,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//FALSE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeOut},
    //uart port 3   --- to GUI
    {   _UART_PORT_3,
        {9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeOut},
    //uart port 4   --- to RS232 IN
    {   _UART_PORT_4,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart4ProtocalDataReceive,
        Uart4ParserMethod,
        Uart4VarInit,
        Uart4VarTimeOut},
    //uart port 5   --- to RS232 OUT
    {   _UART_PORT_5,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        Uart5ProtocalDataReceive,
        Uart5ParserMethod,
        Uart5VarInit,
        Uart5VarTimeOut},
    //uart port 6   --- to Key
    {   _UART_PORT_6,
        {9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart6ProtocalDataReceive,
        Uart6ParserMethod,
        Uart6VarInit,
        Uart6VarTimeOut},

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
//    pstNewKeyPort    u8KeyGroup       u32KeyValue     eNewKeyOnLevel      bIsDipKey
    {(pstGpioConfig_t)NULL,                                         0,  BIT0,   ePinLevel_Low,  FALSE},// the last one
};
#endif

const stI2CSwitchCtrl_t stProjectI2cSwitchTable[] =
{
    // switch null
    {NULL, NULL},
};
#if _EN_CHIP_ADN4604_USE
const stDrvADN4604VarConfig stProjectAdn4604Config[] =
{
    {ADN4604_DEV_ADDR_1,_I2C_TABLE_INDEX_ADN4604_1},
    {0,_I2C_TABLE_INDEX_ADN4604_1}
};
#endif

void mapp_SetFanState(bool bEn)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_FANControl].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_FANControl].u16GpioPin;

    if(bEn)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}



