#include "includes.h"

#define DELAY_TIME_FOR_SPI_FLASH_BYTE      5
#define DELAY_TIME_FOR_SPI_FLASH_CLK       5

const stGpioConfig_t stProjectGpioTable[eGpioName_Max]=
{
    //eGpioName_LedActive,
    {PORT_GPIOA,          GPIO_Pin_15,  GPIO_Mode_Out_PP, ePinLevel_High},

    //i2c
    //eGpioName_I2cEepromScl
    {PORT_GPIOB,           GPIO_Pin_6,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cEepromSda
    {PORT_GPIOB,           GPIO_Pin_7,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Scl_1,
    {PORT_GPIOB,           GPIO_Pin_1,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Sda_1,
    {PORT_GPIOB,           GPIO_Pin_0,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Scl_2,
    {PORT_GPIOC,           GPIO_Pin_5,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Sda_2,
    {PORT_GPIOC,           GPIO_Pin_4,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Scl_3,
    {PORT_GPIOA,           GPIO_Pin_7,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Sda_3,
    {PORT_GPIOA,           GPIO_Pin_6,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Scl_4,
    {PORT_GPIOA,           GPIO_Pin_5,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cMS1826Sda_4,
    {PORT_GPIOA,           GPIO_Pin_4,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cPCA9539Scl,
    {PORT_GPIOA,           GPIO_Pin_9,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cPCA9539Sda,
    {PORT_GPIOA,          GPIO_Pin_10,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cPCA9539Scl_2,
    {PORT_GPIOA,          GPIO_Pin_11,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cPCA9539Sda_2,
    {PORT_GPIOA,          GPIO_Pin_12,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cAlpuScl,
    {PORT_GPIOC,           GPIO_Pin_7,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_I2cAlpuSda,
    {PORT_GPIOC,           GPIO_Pin_6,  GPIO_Mode_Out_OD, ePinLevel_High},

    //HDMI OUT 5V
    //eGpioName_OUT5V_HDMI1,
    {PORT_GPIOA,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI2,
    {PORT_GPIOA,           GPIO_Pin_0,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI3,
    {PORT_GPIOC,           GPIO_Pin_8,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI4,
    {PORT_GPIOC,           GPIO_Pin_9,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI5,
    {PORT_GPIOC,          GPIO_Pin_10,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI6,
    {PORT_GPIOC,          GPIO_Pin_11,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI7,
    {PORT_GPIOB,           GPIO_Pin_3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI8,
    {PORT_GPIOB,           GPIO_Pin_5,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI9,
    {PORT_GPIOB,           GPIO_Pin_8,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI10,
    {PORT_GPIOB,           GPIO_Pin_9,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI11,
    {PORT_GPIOC,           GPIO_Pin_2,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI12,
    {PORT_GPIOC,           GPIO_Pin_1,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI13,
    {PORT_GPIOC,           GPIO_Pin_0,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI14,
    {PORT_GPIOA,           GPIO_Pin_3,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI15,
    {PORT_GPIOB,          GPIO_Pin_12,  GPIO_Mode_Out_PP, ePinLevel_High},
    //eGpioName_OUT5V_HDMI16,
    {PORT_GPIOB,          GPIO_Pin_13,  GPIO_Mode_Out_PP, ePinLevel_High},

    //Reset
    //eGpioName_MS1826Reset1,
    {PORT_GPIOC,           GPIO_Pin_3,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_MS1826Reset2,
    {PORT_GPIOC,          GPIO_Pin_13,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_MS1826Reset3,
    {PORT_GPIOC,          GPIO_Pin_15,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_MS1826Reset4,
    {PORT_GPIOA,           GPIO_Pin_8,  GPIO_Mode_Out_OD, ePinLevel_High},
    //eGpioName_PCA9539Rst
    {PORT_GPIOA,           GPIO_Pin_2,  GPIO_Mode_Out_PP, ePinLevel_High},

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
    //eI2cDevice_MS1826_1,      // U10
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_3],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_3],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_MS1826_2,      // U13
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_4],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_4],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_MS1826_3,      // U71
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_1],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_1],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_MS1826_4,      // U74
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Scl_2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cMS1826Sda_2],
        I2C_SPEED_TIME_FOR_400K,
        0,
        0,
        },
    //eI2cDevice_PCA9539,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Scl],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Sda],
        I2C_SPEED_TIME_FOR_300K,
        0,
        0,
        },
    //eI2cDevice_PCA9539_2,
    {   (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Scl_2],
        (pstGpioConfig_t)&stProjectGpioTable[eGpioName_I2cPCA9539Sda_2],
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI5]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI6]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI7]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI8]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI9]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI10]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI11]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI12]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI13]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI14]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI15]},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_OUT5V_HDMI16]},
    //hdmi output 5v null
    {NULL},
};

const stUartDeclare_t stProjectUartWorkTable[] =
{
#if (_ENABLE_UART_1 == _MACRO_ENABLE)
    //uart port 1   --- function
    {   _UART_PORT_1,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None}, \
        FALSE,
        Uart1ProtocalDataReceive,
        Uart1ParserMethod,
        Uart1VarInit,
        Uart1VarTimeOut},
#endif
#if (_ENABLE_UART_2 == _MACRO_ENABLE)
    //uart port 2   --- function2
    {   _UART_PORT_2_PD5_6,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart2ProtocalDataReceive,
        Uart2ParserMethod,
        Uart2VarInit,
        Uart2VarTimeOut},
#endif
#if (_ENABLE_UART_3 == _MACRO_ENABLE)
    //uart port 3   --- debug
    {   _UART_PORT_3,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        TRUE,
        Uart3ProtocalDataReceive,
        Uart3ParserMethod,
        Uart3VarInit,
        Uart3VarTimeOut},
#endif
#if (_ENABLE_UART_4 == _MACRO_ENABLE)
    //uart port 4   --- reserve
    {   _UART_PORT_4,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart4ProtocalDataReceive,
        Uart4ParserMethod,
        Uart4VarInit,
        Uart4VarTimeOut},
#endif
#if (_ENABLE_UART_5 == _MACRO_ENABLE)
    //uart port 5 --- gui
    {   _UART_PORT_5,
        {115200, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,
        Uart5ProtocalDataReceive,
        Uart5ParserMethod,
        Uart5VarInit,
        Uart5VarTimeOut},
#endif
#if (_ENABLE_UART_6 == _MACRO_ENABLE)
    //uart port 6   --- NOTUSE
    {   _UART_PORT_6,
        {9600, USART_WordLength_8b, USART_StopBits_1, USART_Parity_No, (USART_Mode_Rx|USART_Mode_Tx), USART_HardwareFlowControl_None},
        FALSE,//TRUE,
        Uart6ProtocalDataReceive,
        Uart6ParserMethod,
        Uart6VarInit,
        Uart6VarTimeOut},
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
    {(pstGpioConfig_t)NULL,                                         0,  BIT0,               ePinLevel_Low,  FALSE},// the last one
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
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset1],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset2],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset3],        ePinLevel_Low},
    {(pstGpioConfig_t)&stProjectGpioTable[eGpioName_MS1826Reset4],        ePinLevel_Low},

    // none
    {NULL,ePinLevel_Low},
};
#endif
#if _EN_CHIP_LT8644_USE
const stDrvLT8644VarConfig stProjectLT8644Config[] =
{
    // end
    {
        0,
        _I2C_TABLE_INDEX_LT8644_0,
        NULL,
        NULL,
        ePinLevel_Low
    }
};
#endif
#if ENABLE_SERIES_LCD_FUNCTION
const stSpiLcdPinCfg_t stProjectSpiLcdPinTable[] =
{
//  LcdBkLight  LcdReset    LcdCs       LcdRs       LcdSclk      LcdData
    {NULL,      NULL,       NULL,       NULL,       NULL,       NULL},
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


