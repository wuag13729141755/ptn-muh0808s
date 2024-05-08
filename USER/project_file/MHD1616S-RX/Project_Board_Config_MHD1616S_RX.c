#include "includes.h"
#define DELAY_TIME_FOR_FPGA_FLASH_BYTE      5
#define DELAY_TIME_FOR_FPGA_FLASH_CLK       3

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_ActiveLed = 0,
	{PORT_GPIOA,          GPIO_Pin_15,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_GsvReset,
	{PORT_GPIOC,          GPIO_Pin_13,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_FPGAReset,
	{PORT_GPIOA,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},

    // I2C
    //eGpioName_I2cEepromScl,
    {PORT_GPIOB,           GPIO_Pin_9,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOB,           GPIO_Pin_8,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Scl1,
    {PORT_GPIOB,          GPIO_Pin_14,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Sda1,
    {PORT_GPIOB,          GPIO_Pin_15,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Scl2,
    {PORT_GPIOB,          GPIO_Pin_12,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Sda2,
    {PORT_GPIOB,          GPIO_Pin_13,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Scl3,
    {PORT_GPIOC,           GPIO_Pin_4,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Sda3,
    {PORT_GPIOC,           GPIO_Pin_5,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Scl4,
    {PORT_GPIOB,           GPIO_Pin_0,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIT66021Sda4,
    {PORT_GPIOB,           GPIO_Pin_1,  GPIO_Mode_Out_OD, ePinLevel_High},

    // SPI
    //eGpioName_FpgaFlashCs,
    {PORT_GPIOA,           GPIO_Pin_4,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashSclk,
    {PORT_GPIOA,           GPIO_Pin_5,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashMiso,
    {PORT_GPIOA,           GPIO_Pin_6,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_FpgaFlashMosi,
    {PORT_GPIOA,           GPIO_Pin_7,  GPIO_Mode_Out_OD, ePinLevel_High},

    // SPI Switcher
    //eGpioName_SwQS3L384A1,
    {PORT_GPIOC,           GPIO_Pin_0,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B1,
    {PORT_GPIOC,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384A2,
    {PORT_GPIOC,           GPIO_Pin_2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B2,
    {PORT_GPIOC,           GPIO_Pin_3,  GPIO_Mode_Out_PP, ePinLevel_High},

    // Hdmi Rx Hpd Pin
    //eGpioName_HdmiRxHpd1,
    {PORT_GPIOC,          GPIO_Pin_10,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd2,
    {PORT_GPIOC,          GPIO_Pin_11,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd3,
    {PORT_GPIOC,          GPIO_Pin_12,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd4,
    {PORT_GPIOB,           GPIO_Pin_3,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd5,
    {PORT_GPIOB,           GPIO_Pin_4,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd6,
    {PORT_GPIOB,           GPIO_Pin_5,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd7,
    {PORT_GPIOB,           GPIO_Pin_6,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_HdmiRxHpd8,
    {PORT_GPIOB,           GPIO_Pin_7,     GPIO_Mode_IPU, ePinLevel_High},

    // ID detect
    //eGpioName_IDDetect,
    {PORT_GPIOA,           GPIO_Pin_8,     GPIO_Mode_IPU, ePinLevel_High},

};

const stLedDeclare_t stProjectLedTable[] =
{
    //led_name_ActiveState = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_ActiveLed],          ePinLevel_High},
    //led_name_End
    {(pstGpioConfig_t)NULL,                                                 ePinLevel_Low},// the last one
};

const stI2cDeclare_t stProjectI2cTable[] =
{
    //eeprom
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromScl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cEepromSda],
        I2C_SPEED_TIME_FOR_75K,
        eSw4052Device_Max,
        0,
        },
    //eI2cDevice_It66021_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda1],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Max,
        0,
        },
    //eI2cDevice_It66021_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda2],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Max,
        0,
        },
    //eI2cDevice_It66021_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda3],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Max,
        0,
        },
    //eI2cDevice_It66021_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda4],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Max,
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
    // eSw4052Device_Max switch null
    {   NULL,
        NULL
    },
};

const stSpiDeclare_t stProjectSpiTable[] =
{
    // FPGA Flash eSpiDevice_FpgaFlash
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashMosi],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashMiso],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashSclk],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_FpgaFlashCs],
        DELAY_TIME_FOR_FPGA_FLASH_BYTE,
        DELAY_TIME_FOR_FPGA_FLASH_CLK,
        0,
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
    //uart port 1-- TO BACK BOARD MCU
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},

    //uart port 2--FPGA
    {   _UART_PORT_2,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeout},

    //uart port 3--debug
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
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
    {(pstGpioConfig_t)NULL,                                         0,  BIT0,   ePinLevel_Low,  FALSE},// the last one
};

#endif

const stKeyDeclare_t stProjectKeyTable[] =
{
    //key null
    {{NULL}, \
    key_detect_mode_after, \
    KEY_LONG_INVALID, \
    {KEY_REPEAT_INVALID, KEY_REPEAT_START_DELAY_NONE}, \
    {{{NULL}, KEY_FILTER_TIME}},\
    NULL, \
    IR_REMOTE_KEY_INVALID, \
    NULL}
};

#if _ENABLE_CHIP_GSV_CHIP_SERIES
const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[] =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_GsvReset],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif

bool mapp_GetIDPinState(void)
{
    u8 l_u8PortType = stProjectGpioTable[eGpioName_IDDetect].u8PortType;
    u16 l_u16GpioPin = stProjectGpioTable[eGpioName_IDDetect].u16GpioPin;

    if(GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8PortType-PORT_GPIOA],l_u16GpioPin) == Bit_SET)
        return TRUE;
    return FALSE;
}

