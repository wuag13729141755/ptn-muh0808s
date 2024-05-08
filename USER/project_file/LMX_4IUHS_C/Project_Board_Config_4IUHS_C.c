#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5


const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    // Led
    //eGpioName_LedActive,
    {PORT_GPIOA,       GPIO_Pin_15, GPIO_Mode_Out_PP,       ePinLevel_High},

    //i2c
    //eGpioName_I2cEepromScl,
    {PORT_GPIOB,        GPIO_Pin_6, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cEepromSda,
    {PORT_GPIOB,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cMS1826Scl,
    {PORT_GPIOC,        GPIO_Pin_5, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cMS1826Sda,
    {PORT_GPIOC,        GPIO_Pin_4, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCScl,
    {PORT_GPIOC,        GPIO_Pin_7, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_I2cAlpuCSda,
    {PORT_GPIOC,        GPIO_Pin_6, GPIO_Mode_Out_OD,       ePinLevel_High},

    //Reserve SPI
    //eGpioName_SpiFlashClk,
    {PORT_GPIOC,       GPIO_Pin_10, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_SpiFlashCs,
    {PORT_GPIOC,        GPIO_Pin_8, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_SpiFlashMosi,
    {PORT_GPIOC,       GPIO_Pin_11, GPIO_Mode_Out_OD,       ePinLevel_High},
    //eGpioName_SpiFlashMiso,
    {PORT_GPIOC,        GPIO_Pin_9, GPIO_Mode_Out_OD,       ePinLevel_High},

    //HDMI OUT 5V
    //eGpioName_OUT5V_HDMI1,
    {PORT_GPIOA,        GPIO_Pin_7, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_OUT5V_HDMI2,
    {PORT_GPIOA,        GPIO_Pin_6, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_OUT5V_HDMI3,
    {PORT_GPIOA,        GPIO_Pin_5, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_OUT5V_HDMI4,
    {PORT_GPIOA,        GPIO_Pin_4, GPIO_Mode_Out_PP,       ePinLevel_High},

    //AUDIO MUTE
    //eGpioName_AUDIO_MUTE1,
    {PORT_GPIOC,        GPIO_Pin_3, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_AUDIO_MUTE2,
    {PORT_GPIOC,        GPIO_Pin_2, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_AUDIO_MUTE3,
    {PORT_GPIOC,        GPIO_Pin_1, GPIO_Mode_Out_PP,       ePinLevel_High},
    //eGpioName_AUDIO_MUTE4,
    {PORT_GPIOC,        GPIO_Pin_0, GPIO_Mode_Out_PP,       ePinLevel_High},

    //Reset
    //eGpioName_MS1826Reset,
    {PORT_GPIOD,        GPIO_Pin_2, GPIO_Mode_Out_PP,       ePinLevel_High},

    //MS1826_INT
    //eGpioName_MS1826Int,
    {PORT_GPIOB,        GPIO_Pin_8,    GPIO_Mode_IPU,       ePinLevel_High},

    //eGpioName_PortSel1,
    {PORT_GPIOB,       GPIO_Pin_12,    GPIO_Mode_IPU,       ePinLevel_High},
    //eGpioName_PortSel2,
    {PORT_GPIOB,       GPIO_Pin_13,    GPIO_Mode_IPU,       ePinLevel_High},

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
    //eI2cDevice_MS1826,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda],
        I2C_SPEED_TIME_FOR_300K,
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI1]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI2]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI3]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI4]},
    //hdmi output 5v null
    {NULL},
};

const stUartDeclare_t stProjectUartWorkTable[] =
{
#if _ENABLE_UART_1
    //uart port 1
    {   _UART_PORT_1,
        {9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
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
        FALSE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeout},
#endif

#if _ENABLE_UART_3
    //uart port 3
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
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
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_Gsv2001Reset],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif

#if _ENABLE_CHIP_MS1826
const stMS1826ChipResetPinCfg_t stProjectMS1826ChipResetTable[] =
{
//    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset],        ePinLevel_High},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif

void mapp_SetHdmiTx5V(bool bEnable)
{
	pst5VControl_t p5vCtlPort = (pst5VControl_t)stProjectTx5VControlTable;
    while(p5vCtlPort->pstControlPort != NULL)
    {
		if(p5vCtlPort->pstControlPort->eInitLevel == _GPIO_PIN_LEVEL_LOW)
		{
			if(bEnable)
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		else
		{
			if(bEnable)
                GPIO_SetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
			else
                GPIO_ResetBits(STM32_GPIO_PORT[p5vCtlPort->pstControlPort->u8PortType-PORT_GPIOA], p5vCtlPort->pstControlPort->u16GpioPin);
		}
		p5vCtlPort++;
    }
}

void mapp_SetAudioMute(u8 u8AudioID,bool bEnable)
{
    u8 l_u8EnPort = stProjectGpioTable[eGpioName_AUDIO_MUTE1+u8AudioID].u8PortType;
    u16 l_u16EnPin = stProjectGpioTable[eGpioName_AUDIO_MUTE1+u8AudioID].u16GpioPin;
    if(bEnable)
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_SET);
    }
    else
    {
        GPIO_WriteBit(STM32_GPIO_PORT[l_u8EnPort-PORT_GPIOA],l_u16EnPin,Bit_RESET);
    }
}

u8 app_GetPortSel(void)
{
    u8 l_u8Sel1Port = stProjectGpioTable[eGpioName_PortSel1].u8PortType;
    u16 l_u16Sel1Pin = stProjectGpioTable[eGpioName_PortSel1].u16GpioPin;
    u8 l_u8Sel2Port = stProjectGpioTable[eGpioName_PortSel2].u8PortType;
    u16 l_u16Sel2Pin = stProjectGpioTable[eGpioName_PortSel2].u16GpioPin;

    u8 l_u8Value=0;

    l_u8Value = GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8Sel2Port-PORT_GPIOA],l_u16Sel2Pin);
    l_u8Value = (l_u8Value<<1)|GPIO_ReadInputDataBit(STM32_GPIO_PORT[l_u8Sel1Port-PORT_GPIOA],l_u16Sel1Pin);

    return l_u8Value;
}

