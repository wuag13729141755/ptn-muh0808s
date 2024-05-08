/******************************************************
**Copyright (c) 2008 PTN Electronics Limited.All Rights Reserved
**FileName: GlobalStruct.h
**Writer:       wf8421
**Create Date:  2018-4-26
**Rewriter:     wf8421
**Rewrite Date: 2018-4-26
**Description:  结构体定义，枚举定义
**History:
**Version:  V1.0.0
******************************************************/
#ifndef __GLOBAL_STRUCT_H__
#define __GLOBAL_STRUCT_H__

#ifndef NUM_OF_I2C_BUS_MAX
#define NUM_OF_I2C_BUS_MAX				5
#endif
#ifndef NUM_OF_SPI_BUS_MAX
#define NUM_OF_SPI_BUS_MAX				3
#endif
#ifndef NUM_OF_KEYS_MAX
#define NUM_OF_KEYS_MAX					35
#endif
#ifndef NUM_OF_LEDS_MAX
#define NUM_OF_LEDS_MAX					20
#endif
#ifndef NUM_OF_CEC_PINS_MAX
#define NUM_OF_CEC_PINS_MAX				15
#endif
#define IR_REMOTE_KEY_INVALID			0x0

typedef enum _ePinLevel_t_
{
	ePinLevel_Low = 0,
	ePinLevel_High,
}ePinLevel_t;

#if _ENABLE_AUTO_TURN_TYPE
typedef enum _eAutoTurnType_t_
{
	eAutoTurnType_first = 0,
	eAutoTurnType_next,

	eAutoTurnType_max,
}eAutoTurnType_t;
#endif

typedef struct _stGpioCoinfig_t_
{
	uint8_t 				u8PortType;
	uint16_t 				u16GpioPin;
#if (defined(N32G45X))
    GPIO_ModeType           eGpioMode;
#elif (defined(GD32F4XX))
    uint32_t                eGpioMode;
#else
	GPIOMode_TypeDef 		eGpioMode;
#endif

    #if(defined(STM32F2XX_HD))
	GPIOOType_TypeDef		eGpioOType;
	GPIOPuPd_TypeDef		eGpioPuPdType;
    #elif defined(GD32F4XX)
	uint32_t		        eGpioOType;
	uint32_t		        eGpioPuPdType;
	#endif

	ePinLevel_t 			eInitLevel;
}stGpioConfig_t, *pstGpioConfig_t;

/**********************************************
**key struct
***********************************************/
typedef enum _key_detect_mode_t_
{
	key_detect_mode_pre = 0,//send the value when detected
	key_detect_mode_after,//send the value after key release
}key_detect_mode_t;

typedef struct _stKeyComposite_t_
{
	pstGpioConfig_t 		pstComposite[_NUM_SIMUL_KEYS_COMPOSITE_MAX];
	uint16_t 				u16CompositePressTime;
}stKeyComposite_t, *pstKeyComposite_t;

typedef enum _eKeyValSource_t_
{
    eKeyValSource_None = 0,
	eKeyValSource_Panel,
	eKeyValSource_IrRemote,
	eKeyValSource_Uart,

	eKeyValSource_Max,
}eKeyValSource_t;

typedef struct _stKeyValStore_t_
{
	uint8_t 				u8StoreVal;
	eKeyValSource_t			eValSource;
}stKeyValStore_t, *pstKeyValStore_t;

typedef struct _key_val_stored_t_
{
	stKeyValStore_t			StStoreVal[NUM_OF_KEYS_MAX];
	uint8_t 				valid_key_name;
}key_val_stored_t, *pkey_val_stored_t;

typedef struct _stKeyRepeat_t_
{
    uint16_t                u16RepeatSpeed;
    uint16_t                u16RepeatStartDelay;
}stKeyRepeat_t, *pstKeyRepeat_t;

typedef enum _eKeyPressResSta_t_
{
    eKeyPressResSta_None = 0,
    eKeyPressResSta_Hook,

    eKeyPressResSta_Max,
}eKeyPressResSta_t;

typedef eKeyPressResSta_t (*key_response_fun)(pkey_val_stored_t);
typedef void (*key_response_callback)(uint8_t);

typedef struct _stKeyDeclare_t_
{
    pstGpioConfig_t         pstKeyLocate[_NUM_SIMUL_GPIO_PINS_ONE_KEY_MAX];
    key_detect_mode_t       eDetectMode;
    uint16_t                u16LongPressTime;
    stKeyRepeat_t           stRepeat;
    stKeyComposite_t        stCompositeKey[_NUM_COMPOSITE_GROUP_ONE_KEY_MAX];

    key_response_fun        funKeyResponse;

    uint32_t                u32IrValue;

    key_response_callback   funCallBack;
}stKeyDeclare_t, *pstKeyDeclare_t;

/**********************************************
**led struct
***********************************************/
typedef struct _stLedDeclare_t_
{
    pstGpioConfig_t         pstLedLocate;
    ePinLevel_t             eLedOnLevel;
}stLedDeclare_t, *pstLedDeclare_t;

/**********************************************
**i2c struct
***********************************************/
typedef struct _stI2cDeclare_t_
{
    pstGpioConfig_t         pstSclPort;
    pstGpioConfig_t         pstSdaPort;
    uint16_t                u16I2cDelayT;

    u8                      u8SwitchPortDev;
    u8                      u8SwitchPortNum;
}stI2cDeclare_t, *pstI2cDeclare_t;

/**********************************************
**spi struct
***********************************************/
typedef struct _stSpiDeclare_t_
{
    pstGpioConfig_t         pstSpiMosi;
    pstGpioConfig_t         pstSpiMiso;     // 3pin SPI is NULL
    pstGpioConfig_t         pstSpiClk;
    pstGpioConfig_t         pstSpiCs;
    uint16_t                u16SpiDelayByte;
    uint16_t                u16SpiDelayClk;
    uint8_t                 u8SpiClkMode;
}stSpiDeclare_t, *pstSpiDeclare_t;

typedef struct _stRxHpdDetDeclare_t_
{
    pstGpioConfig_t pstDetPort;
}stRxHpdDetDeclare_t,*pstRxHpdDetDeclare_t;

typedef struct _stIT663xxRstDeclare_t_
{
	pstGpioConfig_t 	pstRstLocate;
	ePinLevel_t 		eRstLevel;
}stIT663xxRstDeclare_t, *pstIT663xxRstDeclare_t;

typedef struct _stTx5VControl_t_
{
    pstGpioConfig_t pstControlPort;
}st5VControl_t,*pst5VControl_t;

typedef enum _eIrRmProtocol_t_
{
    eIrRmProtocol_None = 0,
    eIrRmProtocol_NEC,
    eIrRmProtocol_RC5,

    eIrRmProtocol_Max,
}eIrRmProtocol_t;

typedef struct _stIrSensorConfig_t_
{
    pstGpioConfig_t     pstIrSensorPort;

    eIrRmProtocol_t     eIrRmPro;
}stIrSensorConfig_t, *pstIrSensorConfig_t;

typedef struct _stIrWorkDeclare_t_
{
    stIrSensorConfig_t  stIrSensor;
    pstGpioConfig_t     pstIrSenderPort;
}stIrWorkDeclare_t, *pstIrWorkDeclare_t;

typedef void (*uart_protocol_receive)(uint8_t);
typedef void (*uart_cmd_parser)(void);
typedef void (*uart_var_init)(void);
typedef void (*uart_pro_timeout)(void);

#if (defined(GD32F4XX))
typedef struct
{
    uint32_t BaudRate; /*!< This member configures the USART communication baud rate.
                                  The baud rate is computed using the following formula:
                                   - IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->BaudRate)))
                                   - FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

    uint16_t WordLength; /*!< Specifies the number of data bits transmitted or received in a frame.
                                    This parameter can be a value of @ref USART_Word_Length */

    uint16_t StopBits; /*!< Specifies the number of stop bits transmitted.
                                  This parameter can be a value of @ref USART_Stop_Bits */

    uint16_t Parity; /*!< Specifies the parity mode.
                                This parameter can be a value of @ref Parity
                                @note When parity is enabled, the computed parity is inserted
                                      at the MSB position of the transmitted data (9th bit when
                                      the word length is set to 9 data bits; 8th bit when the
                                      word length is set to 8 data bits). */

    uint16_t Mode; /*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                              This parameter can be a value of @ref Mode */

    uint16_t HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled
                                             or disabled.
                                             This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitType;
#endif

typedef struct _stUartDeclare_t_
{
    uint8_t                 u8UartPort;
#if (defined(N32G45X))
    USART_InitType          stUartInitType;
#elif (defined(GD32F4XX))
    USART_InitType          stUartInitType;
#else
    USART_InitTypeDef       stUartInitType;
#endif
    bool                    bIsDebugPort;

    uart_protocol_receive   funUartReceive;
    uart_cmd_parser         funUartParser;
    uart_var_init           funUartVarInit;
    uart_pro_timeout        funUartProTimeOut;
}stUartDeclare_t, *pstUartDeclare_t;

typedef enum _eCecSrcDisType_t_
{
    eCecSrcDisType_Src = 0,
    eCecSrcDisType_Dis,

    eCecSrcDisType_Max
}eCecSrcDisType_t;

typedef struct _stCecCnntType_t_
{
    eCecSrcDisType_t        eCecDirType;
    bool                    bEnReceive;
    uint8_t                 u8PortIndex;
    uint8_t                 u8LocalAddr;
}stCecCnntType_t, *pstCecCnntType_t;

typedef struct _stCecDeclare_t_
{
    pstGpioConfig_t         pstCecGpioLoc;
    stCecCnntType_t         stCecCnntType;
}stCecDeclare_t, *pstCecDeclare_t;

#if _ENABLE_NEW_USER_KEY_DEFINE_FUNCTION
typedef struct _stNewKeyDeclare_t_
{
    pstGpioConfig_t         pstNewKeyPort;
    u8                      u8KeyGroup;
    uint32_t                u32KeyValue;
    ePinLevel_t             eNewKeyOnLevel;
    bool                    bIsDipKey;
}stNewKeyDeclare_t, *pstNewKeyDeclare_t;
#endif

typedef struct _stI2CSwitchCtrl_t_
{
    pstGpioConfig_t pstCtrlPortA;
    pstGpioConfig_t pstCtrlPortB;
    #if _ENABLE_SW4052_CS_PIN
    pstGpioConfig_t pstCtrlCsPin;
    #endif
}stI2CSwitchCtrl_t,*pstI2CSwitchCtrl_t;

#if _ENABLE_CHIP_LM74HC138
typedef struct _stDrvLM74LS138Ctrl_t_
{
    pstGpioConfig_t pstCtrlPortA;
    pstGpioConfig_t pstCtrlPortB;
    pstGpioConfig_t pstCtrlPortC;
}stDrvLM74LS138Ctrl_t,*pstDrvLM74LS138Ctrl_t;
#endif

#if _EN_CHIP_ADN4604_USE
typedef struct _stDrvADN4604VarConfig_t_
{
    u8 u8DevAddr;
    u8 u8I2cDevID;
}stDrvADN4604VarConfig,*pstDrvADN4604VarConfig;
#endif
#if (_EN_CHIP_LT8644_USE == _MACRO_ENABLE)
typedef struct _stDrvLT8644VarConfig_t_
{
    u8 u8DevAddr;
    u8 u8I2cDevID;
    pstGpioConfig_t 		pstUpdatePin;
    pstGpioConfig_t 		pstResetPin;
	ePinLevel_t 			eRstLevel;
}stDrvLT8644VarConfig,*pstDrvLT8644VarConfig;
#endif

typedef void (*vvFuncPoint)(void);
typedef void (*vpu8FuncPoint)(uint8_t *);
typedef void (*vu8FuncPoint)(uint8_t);

typedef struct _stFuncPinDeclare_t_
{
	pstGpioConfig_t 		pstFunPinLocate;
	ePinLevel_t 			eFunLevel;
}stFuncPinDeclare_t, *pstFuncPinDeclare_t;


#if _ENABLE_CHIP_GSV_CHIP_SERIES
typedef struct _stGsvChipResetPinCfg_t_
{
	pstGpioConfig_t 		pstGpioCfgTable;
	ePinLevel_t 			eRstLevel;
}stGsvChipResetPinCfg_t, *pstGsvChipResetPinCfg_t;
#endif

#if _ENABLE_CHIP_MS1826
typedef struct _stMS1826ChipResetPinCfg_t_
{
	pstGpioConfig_t 		pstGpioCfgTable;
	ePinLevel_t 			eRstLevel;
}stMS1826ChipResetPinCfg_t, *pstMS1826ChipResetPinCfg_t;
#endif

typedef struct _stChipResetPinCfg_t_
{
	pstGpioConfig_t 		pstGpioCfgTable;
	ePinLevel_t 			eRstLevel;
}stChipResetPinCfg_t, *pstChipResetPinCfg_t;


#if _ENABLE_TIMING_STRUCT_DEFINE
typedef __packed struct _stTimingTable_t_
{
    u16 u16HActive;         // 水平显示宽度
    u16 u16HTotal;          // 水平信号总宽度
    u16 u16HBP;             // 水平消隐后肩
    u16 u16HFP;             // 水平消隐前肩
    u16 u16HSync;            // 水平同步宽度
    u8  u8HPol;             // 水平同步极性

    u16 u16VActive;         // 垂直显示宽度
    u16 u16VTotal;          // 垂直信号总宽度
    u16 u16VBP;             // 垂直消隐后肩
    u16 u16VFP;             // 垂直消隐前肩
    u16 u16VSync;            // 垂直同步宽度
    u8  u8VPol;             // 垂直同步极性

    u8  u8VClk;
    u8  u8Vic;
    u16 u16PClk;                // u16PClk(KHz) = (pixel clk )/10000
	u8  u8Interlace;
}stTimingTable_t,*pstTimingTable_t;
#endif

#if ENABLE_SERIES_LCD_FUNCTION
typedef struct _stSpiLcdPinCfg_t_
{
    pstGpioConfig_t         pstGpioBacklight;
    pstGpioConfig_t         pstGpioReset;
    pstGpioConfig_t         pstGpioCs;
    pstGpioConfig_t         pstGpioRs;
    pstGpioConfig_t         pstGpioSclk;
    pstGpioConfig_t         pstGpioData;
}stSpiLcdPinCfg_t, *pstSpiLcdPinCfg_t;
#endif


#endif

