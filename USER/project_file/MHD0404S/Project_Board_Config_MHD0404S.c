#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_LedSta,
    {PORT_GPIOB,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_PP,   GPIO_PuPd_DOWN,   ePinLevel_Low},

    //i2c
    //eGpioName_I2cEepromScl
    {PORT_GPIOA,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cEepromSda
    {PORT_GPIOC,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cIT66021Scl1,
    {PORT_GPIOE,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cIT66021Sda1,
    {PORT_GPIOE,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cIT66021Scl2,
    {PORT_GPIOE,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cIT66021Sda2,
    {PORT_GPIOE,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Scl1,
    {PORT_GPIOE,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Sda1,
    {PORT_GPIOE,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Scl2,
    {PORT_GPIOE,       GPIO_Pin_10,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Sda2,
    {PORT_GPIOE,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Scl3,
    {PORT_GPIOE,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Sda3,
    {PORT_GPIOE,       GPIO_Pin_11,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Scl4,
    {PORT_GPIOE,       GPIO_Pin_14,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cGSV2011Sda4,
    {PORT_GPIOE,       GPIO_Pin_13,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cIDT5V49EE902Scl,
    {PORT_GPIOB,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cIDT5V49EE902Sda,
    {PORT_GPIOB,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cAlpuCScl,
    {PORT_GPIOD,       GPIO_Pin_13,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_I2cAlpuCSda,
    {PORT_GPIOD,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_OD, GPIO_PuPd_NOPULL,   ePinLevel_High},

    //Spi
    //eGpioName_SpiFlashClk,
    {PORT_GPIOB,       GPIO_Pin_13,   GPIO_Mode_OUT, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_SpiFlashCs,
    {PORT_GPIOB,       GPIO_Pin_12,   GPIO_Mode_OUT, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_SpiFlashMosi,
    {PORT_GPIOB,       GPIO_Pin_15,   GPIO_Mode_OUT, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_SpiFlashMiso,
    {PORT_GPIOB,       GPIO_Pin_14,    GPIO_Mode_IN, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},

    // reset
    //eGpioName_IT66021Rst,
    {PORT_GPIOC,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_Gsv2011Rst,
    {PORT_GPIOA,        GPIO_Pin_3,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_FpgaRst1,
    {PORT_GPIOA,        GPIO_Pin_0,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_FpgaRst2,
    {PORT_GPIOA,        GPIO_Pin_1,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    // FAN control
    //eGpioName_FANControl,
    {PORT_GPIOE,        GPIO_Pin_2,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // QS3384Q
    //eGpioName_QS3384QBEA1,
    {PORT_GPIOD,        GPIO_Pin_8,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_QS3384QBEB1,
    {PORT_GPIOD,        GPIO_Pin_9,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_QS3384QBEA2,
    {PORT_GPIOD,       GPIO_Pin_10,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_QS3384QBEB2,
    {PORT_GPIOD,       GPIO_Pin_11,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // Hdmi TX 5V
    //eGpioName_HdmiTx5V1,
    {PORT_GPIOA,        GPIO_Pin_4,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_HdmiTx5V2,
    {PORT_GPIOA,        GPIO_Pin_5,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_HdmiTx5V3,
    {PORT_GPIOA,        GPIO_Pin_6,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},
    //eGpioName_HdmiTx5V4,
    {PORT_GPIOA,        GPIO_Pin_7,   GPIO_Mode_OUT, GPIO_OType_PP, GPIO_PuPd_NOPULL,   ePinLevel_High},

    // Hdmi RX HPD
    //eGpioName_HdmiRxHpd1,
    {PORT_GPIOC,       GPIO_Pin_15,    GPIO_Mode_IN, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_HdmiRxHpd2,
    {PORT_GPIOC,        GPIO_Pin_0,    GPIO_Mode_IN, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_HdmiRxHpd3,
    {PORT_GPIOC,        GPIO_Pin_1,    GPIO_Mode_IN, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},
    //eGpioName_HdmiRxHpd4,
    {PORT_GPIOC,        GPIO_Pin_2,    GPIO_Mode_IN, GPIO_OType_PP,     GPIO_PuPd_UP,   ePinLevel_High},

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
    //led_name_hdmi_input_Red = 0,
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
    //eI2cDevice_IT66021_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda1],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_IT66021_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cIT66021Sda2],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_Gsv1016_1,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Scl1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Sda1],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_Gsv1016_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Scl2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Sda2],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_Gsv1016_3,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Scl3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Sda3],
        I2C_SPEED_TIME_FOR_100K,
        0,
        0,
        },
    //eI2cDevice_Gsv1016_4,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Scl4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cGSV2011Sda4],
        I2C_SPEED_TIME_FOR_100K,
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
        I2C_SPEED_TIME_FOR_300K,
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiTx5V1]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiTx5V2]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiTx5V3]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_HdmiTx5V4]},
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
    {   _UART_PORT_4_PC10_11,
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

#if _EN_CHIP_ADN4604_USE
const stDrvADN4604VarConfig stProjectAdn4604Config[] =
{
    {ADN4604_DEV_ADDR_1,_I2C_TABLE_INDEX_ADN4604_1},
    {0,_I2C_TABLE_INDEX_ADN4604_1}
};
#endif
#if _ENABLE_CHIP_GSV_CHIP_SERIES
const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[] =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2011Rst],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
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

void mapp_SetHdmiTx5V(bool bEnable)
{
	pst5VControl_t p5vCtlPort = (pst5VControl_t)stProjectTx5VControlTable;
    while(p5vCtlPort->pstControlPort != NULL)
    {
		if(p5vCtlPort->pstControlPort->eInitLevel == _GPIO_PIN_LEVEL_LOW)
		{
			if(bEnable)
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		else
		{
			if(bEnable)
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		p5vCtlPort++;
    }
}

void mapp_SetFpgaResetPin(u8 u8ID,u8 u8Mode)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_FpgaRst1+u8ID].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_FpgaRst1+u8ID].u16GpioPin;
    if(u8Mode)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

void mapp_SelectFpgaFlashUpdateMode(u8 u8ID,bool bEnable)
{
    u8 l_u8QS3L384APort = stProjectGpioTable[eGpioName_QS3384QBEA1].u8PortType;
    u16 l_u16QS3L384APin = stProjectGpioTable[eGpioName_QS3384QBEA1].u16GpioPin;
    u8 l_u8QS3L384BPort = stProjectGpioTable[eGpioName_QS3384QBEB1].u8PortType;
    u16 l_u16QS3L384BPin = stProjectGpioTable[eGpioName_QS3384QBEB1].u16GpioPin;

    switch(u8ID)
    {
        case 0:
            {
                l_u8QS3L384APort = stProjectGpioTable[eGpioName_QS3384QBEA1].u8PortType;
                l_u16QS3L384APin = stProjectGpioTable[eGpioName_QS3384QBEA1].u16GpioPin;
                l_u8QS3L384BPort = stProjectGpioTable[eGpioName_QS3384QBEB1].u8PortType;
                l_u16QS3L384BPin = stProjectGpioTable[eGpioName_QS3384QBEB1].u16GpioPin;
            }
            break;
        case 1:
            {
                l_u8QS3L384APort = stProjectGpioTable[eGpioName_QS3384QBEA2].u8PortType;
                l_u16QS3L384APin = stProjectGpioTable[eGpioName_QS3384QBEA2].u16GpioPin;
                l_u8QS3L384BPort = stProjectGpioTable[eGpioName_QS3384QBEB2].u8PortType;
                l_u16QS3L384BPin = stProjectGpioTable[eGpioName_QS3384QBEB2].u16GpioPin;
            }
            break;
        default:
            break;
    }

    if(bEnable)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384APort-PORT_GPIOA],l_u16QS3L384APin,Bit_SET);
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384BPort-PORT_GPIOA],l_u16QS3L384BPin,Bit_RESET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384APort-PORT_GPIOA],l_u16QS3L384APin,Bit_RESET);
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8QS3L384BPort-PORT_GPIOA],l_u16QS3L384BPin,Bit_SET);
    }
}

