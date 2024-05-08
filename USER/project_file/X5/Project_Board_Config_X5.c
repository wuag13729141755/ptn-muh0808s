#include "includes.h"

#define DELAY_TIME_FOR_EEPROM                   55      // 单次 70K      // 平均 61.5k
#define DELAY_TIME_FOR_ITE                      55


const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_VDa0 = 0,
    {PORT_GPIOC,        GPIO_Pin_10,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDb0,
    {PORT_GPIOC,        GPIO_Pin_11,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDc0,
    {PORT_GPIOC,        GPIO_Pin_12,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDd0,
    {PORT_GPIOB,         GPIO_Pin_3,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDe0,
    {PORT_GPIOB,         GPIO_Pin_4,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDf0,
    {PORT_GPIOB,         GPIO_Pin_5,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDg0,
    {PORT_GPIOB,         GPIO_Pin_6,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDCom1,
    {PORT_GPIOC,         GPIO_Pin_6,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDCom2,
    {PORT_GPIOC,         GPIO_Pin_7,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDCom3,
    {PORT_GPIOC,         GPIO_Pin_8,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_VDCom4,
    {PORT_GPIOC,         GPIO_Pin_9,     GPIO_Mode_Out_OD,     ePinLevel_High},

    //eGpioName_ActiveLed
    {PORT_GPIOA,        GPIO_Pin_15,     GPIO_Mode_Out_OD,     ePinLevel_High},

    // Key
    //eGpioName_TxKeyLeft,
    {PORT_GPIOB,        GPIO_Pin_14,     GPIO_Mode_IPU,        ePinLevel_High},
    //eGpioName_TxKeyRight,
    {PORT_GPIOB,        GPIO_Pin_15,     GPIO_Mode_IPU,        ePinLevel_High},
    //eGpioName_RxKeyLeft,
    {PORT_GPIOB,        GPIO_Pin_12,     GPIO_Mode_IPD,        ePinLevel_High},
    //eGpioName_RxKeyRight,
    {PORT_GPIOB,        GPIO_Pin_13,     GPIO_Mode_IPD,        ePinLevel_High},

    // I2C
    //eGpioName_I2cEepromScl,
    {PORT_GPIOC,        GPIO_Pin_15,     GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOC,        GPIO_Pin_14,     GPIO_Mode_Out_OD,     ePinLevel_High},

    //GPIO
    //eGpioName_IO_IROUT,
    {PORT_GPIOA,       GPIO_Pin_4,       GPIO_Mode_Out_OD,     ePinLevel_High},
    //eGpioName_IO_IRIN,
    {PORT_GPIOA,       GPIO_Pin_1,       GPIO_Mode_IPU,        ePinLevel_High},

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
#if 1
    //led_name_VDa0 = 0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDa0],              ePinLevel_Low},
    //led_name_VDb0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDb0],              ePinLevel_Low},
    //led_name_VDc0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDc0],              ePinLevel_Low},
    //led_name_VDd0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDd0],              ePinLevel_Low},
    //led_name_VDe0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDe0],              ePinLevel_Low},
    //led_name_VDf0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDf0],              ePinLevel_Low},
    //led_name_VDg0,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDg0],              ePinLevel_Low},
    //led_name_VDCom1,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDCom1],            ePinLevel_High},
    //led_name_VDCom2,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDCom2],            ePinLevel_High},
    //led_name_VDCom3,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDCom3],            ePinLevel_High},
    //led_name_VDCom4,
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_VDCom4],            ePinLevel_High},
#endif
    //led_name_ActiveLed
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_ActiveLed],         ePinLevel_High},
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_IO_IRIN], eIrRmProtocol_NEC},
    (pstGpioConfig_t)&stProjectGpioTable[eGpioName_IO_IROUT],
};

const st5VControl_t stProjectTx5VControlTable[] =
{
    //hdmi output 5v null
    {NULL},
};

const stUartDeclare_t stProjectUartWorkTable[] =
{
    //uart port 1--GUI
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        TRUE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},
    //uart port 2--RS232
    {   _UART_PORT_2,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        UartProtocalDataReceive,
        UartParserMethod,
        UartVarInit,
        UartVarTimeout},

    #if 0
    //uart port 3--Mstar182
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        NULL,
        NULL,
        MstVarInit,
        NULL},
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_TxKeyLeft],     0,  BIT0,   ePinLevel_Low,  FALSE},// 0
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_TxKeyRight],    0,  BIT1,   ePinLevel_Low,  FALSE},// 0
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_RxKeyLeft],     0,  BIT2,   ePinLevel_Low,  FALSE},// 0
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_RxKeyRight],    0,  BIT3,   ePinLevel_Low,  FALSE},// 0

    {(pstGpioConfig_t)NULL,                                         0,  BIT0,   ePinLevel_Low,  FALSE},// the last one
};

#endif

const stI2CSwitchCtrl_t stProjectI2cSwitchTable[] =
{
    // switch null
    {NULL, NULL},
};

void mapp_GenIOWrite(eProjectGpioName_t eIoName,bool bSta)
{
    u8 l_u8EnPort = stProjectGpioTable[eIoName].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eIoName].u16GpioPin;

    if(bSta)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

void mapp_GenIORead(eProjectGpioName_t eIoName,bool *pbSta)
{
    u8 l_u8EnPort = stProjectGpioTable[eIoName].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eIoName].u16GpioPin;

    *pbSta = (GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin)==0)?FALSE:TRUE;
}

u8 mapp_GetRxKeySta(u8 u8RxPort)
{
    bool l_bRetVal = FALSE;
    mapp_GenIORead((eProjectGpioName_t)(eGpioName_RxKeyLeft+u8RxPort),&l_bRetVal);

    return l_bRetVal?1:0;
}

