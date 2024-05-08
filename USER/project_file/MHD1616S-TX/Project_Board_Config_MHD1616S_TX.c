#include "includes.h"
#define DELAY_TIME_FOR_FPGA_FLASH_BYTE      5
#define DELAY_TIME_FOR_FPGA_FLASH_CLK       3

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_ActiveLed = 0,
	{PORT_GPIOA,          GPIO_Pin_15,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_GsvReset,
	{PORT_GPIOC,          GPIO_Pin_13,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_PCA9539Reset,
	{PORT_GPIOA,           GPIO_Pin_8,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_FPGAReset,
	{PORT_GPIOA,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},

    // I2C
    //eGpioName_I2cEepromScl,
    {PORT_GPIOB,           GPIO_Pin_9,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOB,           GPIO_Pin_8,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cPCA9539Scl,
    {PORT_GPIOA,          GPIO_Pin_10,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cPCA9539Sda,
    {PORT_GPIOA,          GPIO_Pin_11,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Scl1,
    {PORT_GPIOC,           GPIO_Pin_5,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Sda1,
    {PORT_GPIOC,           GPIO_Pin_4,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Scl2,
    {PORT_GPIOB,           GPIO_Pin_1,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Sda2,
    {PORT_GPIOB,           GPIO_Pin_0,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Scl3,
    {PORT_GPIOB,          GPIO_Pin_13,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Sda3,
    {PORT_GPIOB,          GPIO_Pin_12,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Scl4,
    {PORT_GPIOB,          GPIO_Pin_15,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cGsv1016Sda4,
    {PORT_GPIOB,          GPIO_Pin_14,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIDT5V49Scl1,
    {PORT_GPIOD,           GPIO_Pin_2,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIDT5V49Sda1,
    {PORT_GPIOA,           GPIO_Pin_0,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIDT5V49Scl2,
    {PORT_GPIOC,          GPIO_Pin_15,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cIDT5V49Sda2,
    {PORT_GPIOC,          GPIO_Pin_14,  GPIO_Mode_Out_OD, ePinLevel_High},
    // I2C sw
    //eGpioName_HC4052SwA1,
    {PORT_GPIOC,          GPIO_Pin_10,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwB1,
    {PORT_GPIOC,          GPIO_Pin_11,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwA2,
    {PORT_GPIOC,          GPIO_Pin_12,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwB2,
    {PORT_GPIOB,           GPIO_Pin_3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwA3,
    {PORT_GPIOB,           GPIO_Pin_4,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwB3,
    {PORT_GPIOB,           GPIO_Pin_5,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwA4,
    {PORT_GPIOB,           GPIO_Pin_6,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_HC4052SwB4,
    {PORT_GPIOB,           GPIO_Pin_7,  GPIO_Mode_Out_PP, ePinLevel_High},

    // SPI
    //eGpioName_FpgaFlashCs,
    {PORT_GPIOC,           GPIO_Pin_8,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashSclk,
    {PORT_GPIOC,          GPIO_Pin_11,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_FpgaFlashMiso,
    {PORT_GPIOC,           GPIO_Pin_9,     GPIO_Mode_IPU, ePinLevel_High},
    //eGpioName_FpgaFlashMosi,
    {PORT_GPIOC,          GPIO_Pin_10,  GPIO_Mode_Out_OD, ePinLevel_High},

    // SPI Switcher
    //eGpioName_SwQS3L384A1,
    {PORT_GPIOC,           GPIO_Pin_0,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B1,
    {PORT_GPIOC,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384A2,
    {PORT_GPIOC,           GPIO_Pin_2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B2,
    {PORT_GPIOC,           GPIO_Pin_3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384A3,
    {PORT_GPIOC,           GPIO_Pin_6,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B3,
    {PORT_GPIOC,           GPIO_Pin_7,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384A4,
    {PORT_GPIOC,           GPIO_Pin_8,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_SwQS3L384B4,
    {PORT_GPIOC,           GPIO_Pin_9,  GPIO_Mode_Out_PP, ePinLevel_High},
    // Hdmi Tx 5V control
    //eGpioName_Hdmi5VTx1,
    {  PORT_PCA,           GPIO_Pin_0,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx2,
    {  PORT_PCA,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx3,
    {  PORT_PCA,           GPIO_Pin_2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx4,
    {  PORT_PCA,           GPIO_Pin_3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx5,
    {  PORT_PCA,           GPIO_Pin_4,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx6,
    {  PORT_PCA,           GPIO_Pin_5,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx7,
    {  PORT_PCA,           GPIO_Pin_6,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx8,
    {  PORT_PCA,           GPIO_Pin_7,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx9,
    {  PORT_PCA,           GPIO_Pin_8,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx10,
    {  PORT_PCA,           GPIO_Pin_9,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx11,
    {  PORT_PCA,          GPIO_Pin_10,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx12,
    {  PORT_PCA,          GPIO_Pin_11,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx13,
    {  PORT_PCA,          GPIO_Pin_12,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx14,
    {  PORT_PCA,          GPIO_Pin_13,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_Hdmi5VTx15,
    {  PORT_PCA,          GPIO_Pin_14,  GPIO_Mode_Out_PP, ePinLevel_High},

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

    //eI2cDevice_PCA9539,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Sda],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Max,
        0,
        },
    //eI2cDevice_Gsv1016_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda1],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_1,
        0,
        },
    //eI2cDevice_Gsv1016_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda1],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_1,
        1,
        },
    //eI2cDevice_Gsv1016_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda1],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_1,
        2,
        },
    //eI2cDevice_Gsv1016_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda1],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_1,
        3,
        },
    //eI2cDevice_Gsv1016_5,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda2],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_2,
        0,
        },
    //eI2cDevice_Gsv1016_6,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda2],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_2,
        1,
        },
    //eI2cDevice_Gsv1016_7,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda2],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_2,
        2,
        },
    //eI2cDevice_Gsv1016_8,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda2],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_2,
        3,
        },
    //eI2cDevice_Gsv1016_9,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda3],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_3,
        0,
        },
    //eI2cDevice_Gsv1016_10,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda3],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_3,
        1,
        },
    //eI2cDevice_Gsv1016_11,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda3],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_3,
        2,
        },
    //eI2cDevice_Gsv1016_12,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda3],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_3,
        3,
        },
    //eI2cDevice_Gsv1016_13,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda4],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_4,
        0,
        },
    //eI2cDevice_Gsv1016_14,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda4],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_4,
        1,
        },
    //eI2cDevice_Gsv1016_15,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda4],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_4,
        2,
        },
    //eI2cDevice_Gsv1016_16,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Scl4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGsv1016Sda4],
        I2C_SPEED_TIME_FOR_200K,
        eSw4052Device_Gsv1016_4,
        3,
        },
    //eI2cDevice_IDT5V49_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIDT5V49Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIDT5V49Sda1],
        I2C_SPEED_TIME_FOR_100K,
        eSw4052Device_Max,
        0,
        },
    //eI2cDevice_IDT5V49_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIDT5V49Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIDT5V49Sda2],
        I2C_SPEED_TIME_FOR_100K,
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
    //eSw4052Device_Gsv1016_1 = 0,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwA1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwB1]
    },

    //eSw4052Device_Gsv1016_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwA2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwB2]
    },
    //eSw4052Device_Gsv1016_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwA3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwB3]
    },
    //eSw4052Device_Gsv1016_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwA4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_HC4052SwB4]
    },

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

void mapp_SetChipResetPinUse9539(u8 u8PinMode, bool bEn)
{
    u8 l_u8EnPort = stProjectGpioTable[u8PinMode].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[u8PinMode].u16GpioPin;

    if((u8PinMode<eGpioName_Hdmi5VTx1)||(u8PinMode>eGpioName_Hdmi5VTx16))
        return;

    if(l_u8EnPort != PORT_PCA)
        return;

    #if (_EN_CHIP_PCA9539_USE == _MACRO_ENABLE)
    if(bEn)
    {
        pca_output_val |= l_u16EnPin;
    }
    else
    {
        pca_output_val &= (~l_u16EnPin);
    }
    #endif
}

void mapp_HdmiTx5VEnable(u8 u8HdmiDev, bool bEn)
{
    mapp_SetChipResetPinUse9539(eGpioName_Hdmi5VTx1+u8HdmiDev,bEn);
}

