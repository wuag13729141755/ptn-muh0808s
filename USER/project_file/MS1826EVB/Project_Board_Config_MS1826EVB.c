#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5


const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    // Led
    //eGpioName_LedHdmiIn1,
    {PORT_GPIOC,       GPIO_Pin_13, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiIn2,
    {PORT_GPIOC,       GPIO_Pin_14, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiIn3,
    {PORT_GPIOC,       GPIO_Pin_15, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiIn4,
    {PORT_GPIOC,        GPIO_Pin_0, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiOut1,
    {PORT_GPIOC,        GPIO_Pin_1, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiOut2,
    {PORT_GPIOC,        GPIO_Pin_2, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiOut3,
    {PORT_GPIOC,        GPIO_Pin_3, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedHdmiOut4,
    {PORT_GPIOA,        GPIO_Pin_0, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedPwrOn,
    {PORT_GPIOB,        GPIO_Pin_9, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_LedPwrOff,
    {PORT_GPIOB,        GPIO_Pin_8, GPIO_Mode_Out_PP,       ePinLevel_High},

    //key
    //eGpioName_Key1,
    {PORT_GPIOA,        GPIO_Pin_8,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key2,
    {PORT_GPIOC,        GPIO_Pin_9,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key3,
    {PORT_GPIOC,        GPIO_Pin_8,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key4,
    {PORT_GPIOC,        GPIO_Pin_7,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key5,
    {PORT_GPIOC,        GPIO_Pin_6,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key6,
    {PORT_GPIOB,       GPIO_Pin_15,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key7,
    {PORT_GPIOB,       GPIO_Pin_14,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_Key8,
    {PORT_GPIOB,       GPIO_Pin_13,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_KeyPwr,
    {PORT_GPIOA,        GPIO_Pin_1,    GPIO_Mode_IPU,       ePinLevel_High},

    //i2c
	//eGpioName_I2cEepromScl,
    {PORT_GPIOC,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOC,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cMS1826Scl,
    {PORT_GPIOB,        GPIO_Pin_6, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cMS1826Sda,
    {PORT_GPIOB,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},

    //AUDIO
    //eGpioName_I2SSwitch1,
    {PORT_GPIOA,        GPIO_Pin_4, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_I2SSwitch2,
    {PORT_GPIOA,        GPIO_Pin_5, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_I2SSwitch3,
    {PORT_GPIOA,        GPIO_Pin_6, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_I2SSwitch4,
    {PORT_GPIOA,        GPIO_Pin_7, GPIO_Mode_Out_PP,       ePinLevel_High},

    //eGpioName_SpdifSwitch1,
    {PORT_GPIOB,        GPIO_Pin_0, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_SpdifSwitch2,
    {PORT_GPIOB,        GPIO_Pin_1, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_SpdifSwitch3,
    {PORT_GPIOB,        GPIO_Pin_2, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_SpdifSwitch4,
    {PORT_GPIOB,       GPIO_Pin_10, GPIO_Mode_Out_PP,       ePinLevel_High},

    //Reset
    //eGpioName_MS1826Reset,
    {PORT_GPIOB,        GPIO_Pin_5, GPIO_Mode_Out_PP,       ePinLevel_High},

    //MS1826_INT
    //eGpioName_MS1826Int,
    {PORT_GPIOC,       GPIO_Pin_12,    GPIO_Mode_IPU,       ePinLevel_High},

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
    //led_name_LedHdmiIn1 = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiIn1],            ePinLevel_Low},
    //led_name_LedHdmiIn2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiIn2],            ePinLevel_Low},
    //led_name_LedHdmiIn3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiIn3],            ePinLevel_Low},
    //led_name_LedHdmiIn4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiIn4],            ePinLevel_Low},
    //led_name_LedHdmiOut1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiOut1],           ePinLevel_Low},
    //led_name_LedHdmiOut2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiOut2],           ePinLevel_Low},
    //led_name_LedHdmiOut3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiOut3],           ePinLevel_Low},
    //led_name_LedHdmiOut4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedHdmiOut4],           ePinLevel_Low},
    //led_name_LedPwrOn,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedPwrOn],              ePinLevel_Low},
    //led_name_LedPwrOff,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_LedPwrOff],             ePinLevel_Low},

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
    //eI2cDevice_MS1826,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda],
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
    //emKeyName_HdmiMode1 = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key1],          0,  BIT0,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_HdmiMode2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key2],          0,  BIT1,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_HdmiMode3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key3],          0,  BIT2,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_HdmiMode4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key4],          0,  BIT3,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_Resolution,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key5],          0,  BIT4,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_SplicingMode,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key6],          0,  BIT5,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_AudioSwitch,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key7],          0,  BIT6,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_WorkMode,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Key8],          0,  BIT7,   ePinLevel_Low,  FALSE},// 0
    //emKeyName_Power,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_KeyPwr],        0,  BIT8,   ePinLevel_Low,  FALSE},// 0

    {(pstGpioConfig_t)NULL,                                         0,  BIT0,   ePinLevel_Low,  FALSE},// the last one
};

#endif


#if _ENABLE_CHIP_GSV_CHIP_SERIES
const stGsvChipResetPinCfg_t stProjectGsvChipResetTable[] =
{
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif

#if _ENABLE_CHIP_MS1826
const stMS1826ChipResetPinCfg_t stProjectMS1826ChipResetTable[] =
{
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif

void mapp_SetAudioMode(u8 u8AudioID,bool bIsInput)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_I2SSwitch1+u8AudioID].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_I2SSwitch1+u8AudioID].u16GpioPin;
    if(bIsInput)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}


